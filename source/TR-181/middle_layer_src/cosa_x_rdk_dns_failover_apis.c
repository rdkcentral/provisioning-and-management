#include "cosa_x_rdk_dns_failover_apis.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "safec_lib_common.h"
#include <telemetry_busmessage_sender.h>

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

static const DNS_FAILOVER_STATUS g_dnsFailoverStatusDefaults =
{
    "Disabled",
    0,
    "0001-01-01T00:00:00Z",
    "0001-01-01T00:00:00Z",
    "None"
};

static DNS_FAILOVER_STATUS g_dnsFailoverStatus =
{
    "Disabled",
    0,
    "0001-01-01T00:00:00Z",
    "0001-01-01T00:00:00Z",
    "None"
};

DNS_FAILOVER_CONFIG*
DNSFailover_GetActiveConfig(void)
{
    return &g_dnsFailoverActive;
}

DNS_FAILOVER_CONFIG*
DNSFailover_GetCandidateConfig(void)
{
    return &g_dnsFailoverCandidate;
}

DNS_FAILOVER_STATUS*
DNSFailover_GetRuntimeStatus(void)
{
    return &g_dnsFailoverStatus;
}

static char*
DNSFailover_TrimWhitespace(char* value)
{
    char* end;

    while (*value && isspace((unsigned char)*value)) value++;
    end = value + strlen(value);
    while (end > value && isspace((unsigned char)end[-1])) end--;
    *end = '\0';
    return value;
}

static BOOL
DNSFailover_IsValidStatusState(const char* state)
{
    return strcmp(state, "Disabled") == 0 ||
        strcmp(state, "Healthy") == 0 ||
        strcmp(state, "Suspect") == 0 ||
        strcmp(state, "Failed") == 0 ||
        strcmp(state, "FailoverActive") == 0;
}

static BOOL
DNSFailover_IsValidTimestamp(const char* value)
{
    static const ULONG digitPositions[] = {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18};
    static const char separators[] = {'-', '-', 'T', ':', ':', 'Z'};
    static const ULONG separatorIndexes[] = {4, 7, 10, 13, 16, 19};
    ULONG i;

    if (strlen(value) != 20) return FALSE;

    for (i = 0; i < sizeof(digitPositions) / sizeof(digitPositions[0]); i++)
    {
        if (!isdigit((unsigned char)value[digitPositions[i]])) return FALSE;
    }

    for (i = 0; i < sizeof(separatorIndexes) / sizeof(separatorIndexes[0]); i++)
    {
        if (value[separatorIndexes[i]] != separators[i]) return FALSE;
    }

    return TRUE;
}

void
DNSFailover_ReadStatusFile(DNS_FAILOVER_STATUS* status)
{
    FILE* file;
    char line[256];

    *status = g_dnsFailoverStatusDefaults;
    file = fopen(DNS_FAILOVER_STATUS_FILE, "r");
    if (!file) return;

    while (fgets(line, sizeof(line), file))
    {
        char* separator;
        char* name;
        char* value;

        if (!strchr(line, '\n') && !feof(file))
        {
            int character;
            while ((character = fgetc(file)) != '\n' && character != EOF) {}
            continue;
        }

        name = DNSFailover_TrimWhitespace(line);
        if (!name[0] || name[0] == '#') continue;

        separator = strchr(name, '=');
        if (!separator) continue;
        *separator = '\0';
        name = DNSFailover_TrimWhitespace(name);
        value = DNSFailover_TrimWhitespace(separator + 1);
        if (!value[0]) continue;

        if (strcmp(name, "State") == 0)
        {
            if (DNSFailover_IsValidStatusState(value))
                DNSFailover_CopyString(status->State, sizeof(status->State), value);
        }
        else if (strcmp(name, "ActiveResolverCount") == 0)
        {
            ULONG resolverCount;
            if (DNSFailover_ParseUlong(value, &resolverCount))
                status->ActiveResolverCount = resolverCount;
        }
        else if (strcmp(name, "LastFailureTime") == 0 && DNSFailover_IsValidTimestamp(value))
        {
            DNSFailover_CopyString(status->LastFailureTime, sizeof(status->LastFailureTime), value);
        }
        else if (strcmp(name, "LastRecoveryTime") == 0 && DNSFailover_IsValidTimestamp(value))
        {
            DNSFailover_CopyString(status->LastRecoveryTime, sizeof(status->LastRecoveryTime), value);
        }
        else if (strcmp(name, "LastVerificationResult") == 0 &&
            strlen(value) < sizeof(status->LastVerificationResult))
        {
            DNSFailover_CopyString(status->LastVerificationResult,
                sizeof(status->LastVerificationResult), value);
        }
    }

    fclose(file);
}

void
DNSFailover_ReportRejected(const char* reason)
{
    CcspTraceError(("DNS failover configuration rejected: %s\n", reason));
    t2_event_s(DNS_FAILOVER_T2_REJECTED, reason);
}

BOOL
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

void
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

ULONG
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

BOOL
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

BOOL
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

BOOL
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

BOOL
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

const char*
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

BOOL
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

BOOL
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

void
DNSFailover_NotifyManager(void)
{
    char pidText[32];
    char* end = NULL;
    long pidValue;
    int extraByte;
    FILE* pidFile = fopen(DNS_FAILOVER_MANAGER_PID_FILE, "r");

    if (!pidFile)
    {
        CcspTraceWarning(("DNS failover manager PID file %s is unavailable: %s\n",
            DNS_FAILOVER_MANAGER_PID_FILE, strerror(errno)));
        return;
    }

    if (!fgets(pidText, sizeof(pidText), pidFile))
    {
        CcspTraceWarning(("DNS failover manager PID file %s is empty or unreadable\n",
            DNS_FAILOVER_MANAGER_PID_FILE));
        fclose(pidFile);
        return;
    }

    if (!feof(pidFile) && !strchr(pidText, '\n'))
    {
        CcspTraceWarning(("DNS failover manager PID in %s is too long\n",
            DNS_FAILOVER_MANAGER_PID_FILE));
        fclose(pidFile);
        return;
    }

    if (strchr(pidText, '\n') &&
        ((extraByte = fgetc(pidFile)) != EOF || ferror(pidFile)))
    {
        CcspTraceWarning(("DNS failover manager PID file %s contains extra data\n",
            DNS_FAILOVER_MANAGER_PID_FILE));
        fclose(pidFile);
        return;
    }
    fclose(pidFile);

    errno = 0;
    pidValue = strtol(pidText, &end, 10);
    if (errno == ERANGE || end == pidText || pidValue <= 1 || pidValue > INT_MAX)
    {
        CcspTraceWarning(("DNS failover manager PID file %s contains an invalid PID\n",
            DNS_FAILOVER_MANAGER_PID_FILE));
        return;
    }

    while (*end && isspace((unsigned char)*end))
    {
        ++end;
    }
    if (*end)
    {
        CcspTraceWarning(("DNS failover manager PID file %s contains trailing data\n",
            DNS_FAILOVER_MANAGER_PID_FILE));
        return;
    }

    if (kill((pid_t)pidValue, SIGUSR1) != 0)
    {
        CcspTraceWarning(("Failed to send SIGUSR1 to DNS failover manager PID %ld: %s\n",
            pidValue, strerror(errno)));
        return;
    }

    CcspTraceInfo(("Sent SIGUSR1 to DNS failover manager PID %ld after config commit\n",
        pidValue));
}

void
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
