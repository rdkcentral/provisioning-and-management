#include "cosa_x_rdk_dns_failover_dml.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "safec_lib_common.h"
#include <telemetry_busmessage_sender.h>

#define DNS_FAILOVER_CONFIG_FILE      "/nvram/dns_failover.conf"
#define DNS_FAILOVER_CONFIG_TEMP_FILE "/nvram/dns_failover.conf.tmp"
#define DNS_FAILOVER_RESOLVER_SOURCE_SIZE 256

#define DNS_FAILOVER_T2_ENABLED  "SYS_INFO_DNS_failover_config_enabled"
#define DNS_FAILOVER_T2_DISABLED "SYS_INFO_DNS_failover_config_disabled"
#define DNS_FAILOVER_T2_REJECTED "SYS_INFO_DNS_config_rejected"
#define DNS_FAILOVER_T2_FAILURE   "SYS_INFO_DNS_Failure_Detected"
#define DNS_FAILOVER_T2_STARTED   "SYS_INFO_DNS_unbound_failover_started"
#define DNS_FAILOVER_T2_STOPPED   "SYS_INFO_DNS_unbound_failover_stopped"
#define DNS_FAILOVER_T2_RECOVERED "SYS_INFO_DNS_resolver_recovered"

#define DNS_FAILOVER_STATUS_VALUE_SIZE 32
#define DNS_FAILOVER_TIMESTAMP_SIZE    32

typedef struct
{
    ULONG Enable;
    char  ResolverSource[DNS_FAILOVER_RESOLVER_SOURCE_SIZE];
    ULONG ReplyDeadline;
    ULONG FailureEpisodeGap;
    ULONG FailureThreshold;
    ULONG MonitorTick;
    ULONG VerifyTimeout;
    ULONG VerifyAttempts;
    ULONG VerifyCooldown;
    ULONG RecoveryInitial;
    ULONG RecoveryMax;
    ULONG RecoverySuccessThreshold;
    ULONG ResolverReload;
} DNS_FAILOVER_CONFIG;

static const DNS_FAILOVER_CONFIG g_dnsFailoverDefaults =
{
    1,
    "/etc/resolv.conf",
    2000,
    5000,
    3,
    250,
    1500,
    1,
    10000,
    15000,
    60000,
    2,
    5000
};

static DNS_FAILOVER_CONFIG g_dnsFailoverActive;
static DNS_FAILOVER_CONFIG g_dnsFailoverCandidate;
static BOOL g_dnsFailoverLoaded = FALSE;

typedef struct
{
    char  State[DNS_FAILOVER_STATUS_VALUE_SIZE];
    ULONG ActiveResolverCount;
    char  LastFailureTime[DNS_FAILOVER_TIMESTAMP_SIZE];
    char  LastRecoveryTime[DNS_FAILOVER_TIMESTAMP_SIZE];
    char  LastVerificationResult[DNS_FAILOVER_STATUS_VALUE_SIZE];
} DNS_FAILOVER_STATUS;

static DNS_FAILOVER_STATUS g_dnsFailoverStatus =
{
    "Healthy",
    0,
    "0001-01-01T00:00:00Z",
    "0001-01-01T00:00:00Z",
    "None"
};

static void
DNSFailover_ReportRejected(const char* reason)
{
    CcspTraceError(("DNS failover configuration rejected: %s\n", reason));
    t2_event_s(DNS_FAILOVER_T2_REJECTED, reason);
}

static BOOL
DNSFailover_CopyString(char* destination, size_t destinationSize, const char* source)
{
    errno_t rc = strcpy_s(destination, destinationSize, source);

    if (rc != EOK)
    {
        ERR_CHK(rc);
        return FALSE;
    }

    return TRUE;
}

static void
DNSFailover_SetTimestamp(char* destination, size_t destinationSize)
{
    time_t currentTime = time(NULL);
    struct tm utcTime;

    if (currentTime == (time_t)-1 || !gmtime_r(&currentTime, &utcTime) ||
        strftime(destination, destinationSize, "%Y-%m-%dT%H:%M:%SZ", &utcTime) == 0)
    {
        DNSFailover_CopyString(destination, destinationSize, "0001-01-01T00:00:00Z");
    }
}

static ULONG
DNSFailover_ReturnString(const char* source, char* destination, ULONG* destinationSize)
{
    size_t required;

    if (!source || !destination || !destinationSize) return (ULONG)-1;

    required = strlen(source) + 1;
    if (*destinationSize < required)
    {
        *destinationSize = (ULONG)required;
        return 1;
    }

    return DNSFailover_CopyString(destination, *destinationSize, source) ? 0 : (ULONG)-1;
}

static BOOL
DNSFailover_ParseUlong(const char* value, ULONG* result)
{
    char* end = NULL;
    unsigned long parsed;

    if (!value || !value[0] || !result || value[0] == '-')
    {
        return FALSE;
    }

    errno = 0;
    parsed = strtoul(value, &end, 10);
    if (errno != 0 || end == value || *end != '\0' || parsed > ULONG_MAX)
    {
        return FALSE;
    }

    *result = (ULONG)parsed;
    return TRUE;
}

static BOOL
DNSFailover_SetUlongByName(DNS_FAILOVER_CONFIG* config, const char* name, ULONG value)
{
    if (strcmp(name, "Enable") == 0) config->Enable = value;
    else if (strcmp(name, "ReplyDeadline") == 0) config->ReplyDeadline = value;
    else if (strcmp(name, "FailureEpisodeGap") == 0) config->FailureEpisodeGap = value;
    else if (strcmp(name, "FailureThreshold") == 0) config->FailureThreshold = value;
    else if (strcmp(name, "MonitorTick") == 0) config->MonitorTick = value;
    else if (strcmp(name, "VerifyTimeout") == 0) config->VerifyTimeout = value;
    else if (strcmp(name, "VerifyAttempts") == 0) config->VerifyAttempts = value;
    else if (strcmp(name, "VerifyCooldown") == 0) config->VerifyCooldown = value;
    else if (strcmp(name, "RecoveryInitial") == 0) config->RecoveryInitial = value;
    else if (strcmp(name, "RecoveryMax") == 0) config->RecoveryMax = value;
    else if (strcmp(name, "RecoverySuccessThreshold") == 0) config->RecoverySuccessThreshold = value;
    else if (strcmp(name, "ResolverReload") == 0) config->ResolverReload = value;
    else return FALSE;

    return TRUE;
}

static BOOL
DNSFailover_GetUlongByName(const DNS_FAILOVER_CONFIG* config, const char* name, ULONG* value)
{
    if (strcmp(name, "Enable") == 0) *value = config->Enable;
    else if (strcmp(name, "ReplyDeadline") == 0) *value = config->ReplyDeadline;
    else if (strcmp(name, "FailureEpisodeGap") == 0) *value = config->FailureEpisodeGap;
    else if (strcmp(name, "FailureThreshold") == 0) *value = config->FailureThreshold;
    else if (strcmp(name, "MonitorTick") == 0) *value = config->MonitorTick;
    else if (strcmp(name, "VerifyTimeout") == 0) *value = config->VerifyTimeout;
    else if (strcmp(name, "VerifyAttempts") == 0) *value = config->VerifyAttempts;
    else if (strcmp(name, "VerifyCooldown") == 0) *value = config->VerifyCooldown;
    else if (strcmp(name, "RecoveryInitial") == 0) *value = config->RecoveryInitial;
    else if (strcmp(name, "RecoveryMax") == 0) *value = config->RecoveryMax;
    else if (strcmp(name, "RecoverySuccessThreshold") == 0) *value = config->RecoverySuccessThreshold;
    else if (strcmp(name, "ResolverReload") == 0) *value = config->ResolverReload;
    else return FALSE;

    return TRUE;
}

static BOOL
DNSFailover_ValidateConfig(const DNS_FAILOVER_CONFIG* config, const char** invalidParameter)
{
    if (config->Enable > 1) *invalidParameter = "Enable";
    else if (config->ReplyDeadline == 0) *invalidParameter = "ReplyDeadline";
    else if (config->FailureEpisodeGap == 0) *invalidParameter = "FailureEpisodeGap";
    else if (config->FailureThreshold == 0) *invalidParameter = "FailureThreshold";
    else if (config->MonitorTick == 0) *invalidParameter = "MonitorTick";
    else if (config->VerifyTimeout == 0) *invalidParameter = "VerifyTimeout";
    else if (config->VerifyAttempts == 0) *invalidParameter = "VerifyAttempts";
    else if (config->VerifyCooldown == 0) *invalidParameter = "VerifyCooldown";
    else if (config->RecoveryInitial == 0) *invalidParameter = "RecoveryInitial";
    else if (config->RecoveryMax == 0) *invalidParameter = "RecoveryMax";
    else if (config->RecoverySuccessThreshold == 0) *invalidParameter = "RecoverySuccessThreshold";
    else if (config->ResolverReload == 0) *invalidParameter = "ResolverReload";
    else if (!config->ResolverSource[0]) *invalidParameter = "ResolverSource";
    else if (config->RecoveryInitial > config->RecoveryMax) *invalidParameter = "RecoveryInitial";
    else return TRUE;

    return FALSE;
}

static const char*
DNSFailover_GetValidationReason
    (
        const DNS_FAILOVER_CONFIG* config,
        const char*                invalidParameter
    )
{
    if (strcmp(invalidParameter, "Enable") == 0)
    {
        return "Enable must be 0 or 1";
    }
    if (strcmp(invalidParameter, "ResolverSource") == 0)
    {
        return "ResolverSource must not be empty";
    }
    if (strcmp(invalidParameter, "RecoveryInitial") == 0 &&
        config->RecoveryInitial > config->RecoveryMax)
    {
        return "RecoveryInitial must be less than or equal to RecoveryMax";
    }

    return "numeric configuration values must be greater than zero";
}

static BOOL
DNSFailover_LoadConfig(DNS_FAILOVER_CONFIG* config, BOOL* usedOverride)
{
    FILE* file;
    char line[512];

    *config = g_dnsFailoverDefaults;
    *usedOverride = FALSE;

    file = fopen(DNS_FAILOVER_CONFIG_FILE, "r");
    if (!file)
    {
        if (errno != ENOENT)
        {
            CcspTraceError(("Failed to open %s: %s\n", DNS_FAILOVER_CONFIG_FILE, strerror(errno)));
        }
        return errno == ENOENT;
    }

    while (fgets(line, sizeof(line), file))
    {
        char* separator;
        char* newline;
        ULONG parsed;

        newline = strpbrk(line, "\r\n");
        if (newline) *newline = '\0';
        if (!line[0]) continue;

        separator = strchr(line, '=');
        if (!separator)
        {
            fclose(file);
            return FALSE;
        }

        *separator = '\0';
        ++separator;

        if (strcmp(line, "ResolverSource") == 0)
        {
            if (!separator[0] || !DNSFailover_CopyString(config->ResolverSource,
                    sizeof(config->ResolverSource), separator))
            {
                fclose(file);
                return FALSE;
            }
        }
        else
        {
            if (!DNSFailover_ParseUlong(separator, &parsed) ||
                !DNSFailover_SetUlongByName(config, line, parsed))
            {
                fclose(file);
                return FALSE;
            }
        }

        *usedOverride = TRUE;
    }

    if (ferror(file))
    {
        fclose(file);
        return FALSE;
    }

    fclose(file);
    return TRUE;
}

static BOOL
DNSFailover_WriteConfig(const DNS_FAILOVER_CONFIG* config)
{
    FILE* file = fopen(DNS_FAILOVER_CONFIG_TEMP_FILE, "w");
    BOOL success = TRUE;

    if (!file)
    {
        CcspTraceError(("Failed to open %s: %s\n", DNS_FAILOVER_CONFIG_TEMP_FILE, strerror(errno)));
        return FALSE;
    }

    if (fprintf(file,
            "Enable=%lu\n"
            "ResolverSource=%s\n"
            "ReplyDeadline=%lu\n"
            "FailureEpisodeGap=%lu\n"
            "FailureThreshold=%lu\n"
            "MonitorTick=%lu\n"
            "VerifyTimeout=%lu\n"
            "VerifyAttempts=%lu\n"
            "VerifyCooldown=%lu\n"
            "RecoveryInitial=%lu\n"
            "RecoveryMax=%lu\n"
            "RecoverySuccessThreshold=%lu\n"
            "ResolverReload=%lu\n",
            config->Enable,
            config->ResolverSource,
            config->ReplyDeadline,
            config->FailureEpisodeGap,
            config->FailureThreshold,
            config->MonitorTick,
            config->VerifyTimeout,
            config->VerifyAttempts,
            config->VerifyCooldown,
            config->RecoveryInitial,
            config->RecoveryMax,
            config->RecoverySuccessThreshold,
            config->ResolverReload) < 0)
    {
        success = FALSE;
    }

    if (success && fflush(file) != 0) success = FALSE;
    if (success && fsync(fileno(file)) != 0) success = FALSE;
    if (fclose(file) != 0) success = FALSE;

    if (!success || rename(DNS_FAILOVER_CONFIG_TEMP_FILE, DNS_FAILOVER_CONFIG_FILE) != 0)
    {
        CcspTraceError(("Failed to write %s: %s\n", DNS_FAILOVER_CONFIG_FILE, strerror(errno)));
        unlink(DNS_FAILOVER_CONFIG_TEMP_FILE);
        return FALSE;
    }

    return TRUE;
}

static void
DNSFailover_EnsureLoaded(void)
{
    BOOL usedOverride = FALSE;
    const char* invalidParameter = NULL;

    if (g_dnsFailoverLoaded) return;

    if (!DNSFailover_LoadConfig(&g_dnsFailoverActive, &usedOverride) ||
        !DNSFailover_ValidateConfig(&g_dnsFailoverActive, &invalidParameter))
    {
        DNSFailover_ReportRejected(invalidParameter ?
            DNSFailover_GetValidationReason(&g_dnsFailoverActive, invalidParameter) :
            "NVRAM configuration could not be parsed");
        g_dnsFailoverActive = g_dnsFailoverDefaults;
        usedOverride = FALSE;
    }

    g_dnsFailoverCandidate = g_dnsFailoverActive;
    g_dnsFailoverLoaded = TRUE;

    DNSFailover_CopyString(g_dnsFailoverStatus.State,
        sizeof(g_dnsFailoverStatus.State),
        g_dnsFailoverActive.Enable ? "Healthy" : "Disabled");

    if (g_dnsFailoverActive.Enable)
    {
        t2_event_s(DNS_FAILOVER_T2_ENABLED, usedOverride ? "override" : "default");
    }
    else
    {
        t2_event_s(DNS_FAILOVER_T2_DISABLED, usedOverride ? "override" : "default");
    }
}

BOOL
DNSFailover_GetParamUlongValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        ULONG*      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || !puLong) return FALSE;
    return DNSFailover_GetUlongByName(&g_dnsFailoverCandidate, ParamName, puLong);
}

ULONG
DNSFailover_GetParamStringValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        char*       pValue,
        ULONG*      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || !pValue || !pUlSize || strcmp(ParamName, "ResolverSource") != 0)
    {
        return (ULONG)-1;
    }

    return DNSFailover_ReturnString(g_dnsFailoverCandidate.ResolverSource, pValue, pUlSize);
}

BOOL
DNSFailover_SetParamUlongValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        ULONG       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || !DNSFailover_SetUlongByName(&g_dnsFailoverCandidate, ParamName, uValue))
    {
        DNSFailover_ReportRejected("unsupported numeric parameter");
        return FALSE;
    }

    if ((strcmp(ParamName, "Enable") == 0 && uValue > 1) ||
        (strcmp(ParamName, "Enable") != 0 && uValue == 0))
    {
        g_dnsFailoverCandidate = g_dnsFailoverActive;
        DNSFailover_ReportRejected(strcmp(ParamName, "Enable") == 0 ?
            "Enable must be 0 or 1" :
            "numeric configuration values must be greater than zero");
        return FALSE;
    }

    return TRUE;
}

BOOL
DNSFailover_SetParamStringValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        char*       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || strcmp(ParamName, "ResolverSource") != 0 || !pString || !pString[0] ||
        strlen(pString) >= sizeof(g_dnsFailoverCandidate.ResolverSource) ||
        strchr(pString, '\n') || strchr(pString, '\r') || strchr(pString, '='))
    {
        DNSFailover_ReportRejected("ResolverSource");
        return FALSE;
    }

    return DNSFailover_CopyString(g_dnsFailoverCandidate.ResolverSource,
        sizeof(g_dnsFailoverCandidate.ResolverSource), pString);
}

BOOL
DNSFailover_Validate
    (
        ANSC_HANDLE hInsContext,
        char*       pReturnParamName,
        ULONG*      puLength
    )
{
    const char* invalidParameter = NULL;
    size_t required;

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (DNSFailover_ValidateConfig(&g_dnsFailoverCandidate, &invalidParameter))
    {
        return TRUE;
    }

    DNSFailover_ReportRejected(DNSFailover_GetValidationReason(
        &g_dnsFailoverCandidate, invalidParameter));
    if (pReturnParamName && puLength)
    {
        required = strlen(invalidParameter) + 1;
        if (*puLength >= required)
        {
            DNSFailover_CopyString(pReturnParamName, *puLength, invalidParameter);
        }
        *puLength = (ULONG)required;
    }

    return FALSE;
}

ULONG
DNSFailover_Commit(ANSC_HANDLE hInsContext)
{
    const char* invalidParameter = NULL;
    ULONG previousEnable;

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!DNSFailover_ValidateConfig(&g_dnsFailoverCandidate, &invalidParameter))
    {
        DNSFailover_ReportRejected(DNSFailover_GetValidationReason(
            &g_dnsFailoverCandidate, invalidParameter));
        return 1;
    }

    if (!DNSFailover_WriteConfig(&g_dnsFailoverCandidate))
    {
        DNSFailover_ReportRejected("NVRAM write failed");
        return 1;
    }

    previousEnable = g_dnsFailoverActive.Enable;
    g_dnsFailoverActive = g_dnsFailoverCandidate;

    DNSFailover_CopyString(g_dnsFailoverStatus.State,
        sizeof(g_dnsFailoverStatus.State),
        g_dnsFailoverActive.Enable ? "Healthy" : "Disabled");

    if (previousEnable != g_dnsFailoverActive.Enable)
    {
        t2_event_s(g_dnsFailoverActive.Enable ? DNS_FAILOVER_T2_ENABLED : DNS_FAILOVER_T2_DISABLED,
            "override");
    }

    return 0;
}

ULONG
DNSFailover_Rollback(ANSC_HANDLE hInsContext)
{
    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();
    g_dnsFailoverCandidate = g_dnsFailoverActive;
    return 0;
}

BOOL
DNSFailoverStatus_GetParamUlongValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        ULONG*      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || !puLong || strcmp(ParamName, "ActiveResolverCount") != 0)
    {
        return FALSE;
    }

    *puLong = g_dnsFailoverStatus.ActiveResolverCount;
    return TRUE;
}

ULONG
DNSFailoverStatus_GetParamStringValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        char*       pValue,
        ULONG*      pUlSize
    )
{
    const char* value = NULL;

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName) return (ULONG)-1;

    if (strcmp(ParamName, "State") == 0) value = g_dnsFailoverStatus.State;
    else if (strcmp(ParamName, "LastFailureTime") == 0) value = g_dnsFailoverStatus.LastFailureTime;
    else if (strcmp(ParamName, "LastRecoveryTime") == 0) value = g_dnsFailoverStatus.LastRecoveryTime;
    else if (strcmp(ParamName, "LastVerificationResult") == 0) value = g_dnsFailoverStatus.LastVerificationResult;
    else return (ULONG)-1;

    return DNSFailover_ReturnString(value, pValue, pUlSize);
}

BOOL
DNSFailoverStatus_SetState(const char* state)
{
    DNSFailover_EnsureLoaded();

    if (!state ||
        (strcmp(state, "Disabled") != 0 &&
         strcmp(state, "Healthy") != 0 &&
         strcmp(state, "Suspect") != 0 &&
         strcmp(state, "Failed") != 0 &&
         strcmp(state, "FailoverActive") != 0))
    {
        CcspTraceError(("Invalid DNS failover runtime state: %s\n",
            state ? state : "null"));
        return FALSE;
    }

    return DNSFailover_CopyString(g_dnsFailoverStatus.State,
        sizeof(g_dnsFailoverStatus.State), state);
}

void
DNSFailoverStatus_SetActiveResolverCount(ULONG resolverCount)
{
    DNSFailover_EnsureLoaded();
    g_dnsFailoverStatus.ActiveResolverCount = resolverCount;
}

void
DNSFailoverTelemetry_ReportFailure(ULONG resolverCount, const char* addressFamilies)
{
    DNSFailover_EnsureLoaded();
    g_dnsFailoverStatus.ActiveResolverCount = resolverCount;
    DNSFailover_CopyString(g_dnsFailoverStatus.State,
        sizeof(g_dnsFailoverStatus.State), "Failed");
    DNSFailover_SetTimestamp(g_dnsFailoverStatus.LastFailureTime,
        sizeof(g_dnsFailoverStatus.LastFailureTime));

    CcspTraceWarning(("DNS complete failure confirmed: resolvers=%lu families=%s time=%s\n",
        resolverCount, addressFamilies ? addressFamilies : "unknown",
        g_dnsFailoverStatus.LastFailureTime));
    t2_event_d(DNS_FAILOVER_T2_FAILURE, 1);
}

void
DNSFailoverTelemetry_ReportVerification
    (
        const char* result,
        const char* resolverIp,
        const char* addressFamily
    )
{
    DNSFailover_EnsureLoaded();

    if (!result || !result[0] || strlen(result) >= sizeof(g_dnsFailoverStatus.LastVerificationResult))
    {
        CcspTraceError(("DNS active verification reported an invalid result\n"));
        return;
    }

    DNSFailover_CopyString(g_dnsFailoverStatus.LastVerificationResult,
        sizeof(g_dnsFailoverStatus.LastVerificationResult), result);
    CcspTraceWarning(("DNS active verification: result=%s resolver=%s family=%s\n",
        result, resolverIp ? resolverIp : "unknown",
        addressFamily ? addressFamily : "unknown"));
}

void
DNSFailoverTelemetry_ReportUnboundFailover(BOOL enabled)
{
    DNSFailover_EnsureLoaded();
    DNSFailover_CopyString(g_dnsFailoverStatus.State,
        sizeof(g_dnsFailoverStatus.State), enabled ? "FailoverActive" : "Healthy");
    CcspTraceWarning(("DNS Unbound failover %s\n", enabled ? "started" : "stopped"));
    t2_event_d(enabled ? DNS_FAILOVER_T2_STARTED : DNS_FAILOVER_T2_STOPPED, 1);
}

void
DNSFailoverTelemetry_ReportRecovery(const char* resolverIp, const char* addressFamily)
{
    DNSFailover_EnsureLoaded();
    DNSFailover_CopyString(g_dnsFailoverStatus.State,
        sizeof(g_dnsFailoverStatus.State), "Healthy");
    DNSFailover_SetTimestamp(g_dnsFailoverStatus.LastRecoveryTime,
        sizeof(g_dnsFailoverStatus.LastRecoveryTime));

    CcspTraceWarning(("DNS resolver recovered: resolver=%s family=%s time=%s\n",
        resolverIp ? resolverIp : "unknown",
        addressFamily ? addressFamily : "unknown",
        g_dnsFailoverStatus.LastRecoveryTime));
    t2_event_d(DNS_FAILOVER_T2_RECOVERED, 1);
}