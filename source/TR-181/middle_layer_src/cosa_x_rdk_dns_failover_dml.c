#include "cosa_x_rdk_dns_failover_dml.h"
#include "cosa_x_rdk_dns_failover_apis.h"

#include <string.h>

#include <telemetry_busmessage_sender.h>

BOOL
DNSFailover_GetParamUlongValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        ULONG*      puLong
    )
{
    BOOL result;

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || !puLong) return FALSE;
    result = DNSFailover_GetUlongByName(DNSFailover_GetCandidateConfig(), ParamName, puLong);
    if (result)
    {
        CcspTraceInfo(("DNS failover config get: %s = %lu\n", ParamName, *puLong));
    }
    return result;
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
    ULONG result;

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || !pValue || !pUlSize || strcmp(ParamName, "ResolverSource") != 0)
    {
        return (ULONG)-1;
    }

    result = DNSFailover_ReturnString(DNSFailover_GetCandidateConfig()->ResolverSource, pValue, pUlSize);
    if (result == 0)
    {
        CcspTraceInfo(("DNS failover config get: %s = %s\n", ParamName, pValue));
    }
    return result;
}

BOOL
DNSFailover_SetParamUlongValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        ULONG       uValue
    )
{
    DNS_FAILOVER_CONFIG* candidate = DNSFailover_GetCandidateConfig();

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || !DNSFailover_SetUlongByName(candidate, ParamName, uValue))
    {
        DNSFailover_ReportRejected("unsupported numeric parameter");
        return FALSE;
    }

    if ((strcmp(ParamName, "Enable") == 0 && uValue > 1) ||
        (strcmp(ParamName, "Enable") != 0 && uValue == 0))
    {
        *candidate = *DNSFailover_GetActiveConfig();
        DNSFailover_ReportRejected(strcmp(ParamName, "Enable") == 0 ?
            "Enable must be 0 or 1" :
            "numeric configuration values must be greater than zero");
        return FALSE;
    }

    CcspTraceInfo(("DNS failover config staged: %s = %lu\n", ParamName, uValue));
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
    DNS_FAILOVER_CONFIG* candidate = DNSFailover_GetCandidateConfig();

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || strcmp(ParamName, "ResolverSource") != 0 || !pString || !pString[0] ||
        strlen(pString) >= sizeof(candidate->ResolverSource) ||
        strchr(pString, '\n') || strchr(pString, '\r') || strchr(pString, '='))
    {
        DNSFailover_ReportRejected("ResolverSource");
        return FALSE;
    }

    if (!DNSFailover_CopyString(candidate->ResolverSource, sizeof(candidate->ResolverSource), pString))
    {
        return FALSE;
    }

    CcspTraceInfo(("DNS failover config staged: %s = %s\n", ParamName, pString));
    return TRUE;
}

BOOL
DNSFailover_Validate
    (
        ANSC_HANDLE hInsContext,
        char*       pReturnParamName,
        ULONG*      puLength
    )
{
    DNS_FAILOVER_CONFIG* candidate = DNSFailover_GetCandidateConfig();
    const char* invalidParameter = NULL;
    size_t required;

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (DNSFailover_ValidateConfig(candidate, &invalidParameter))
    {
        CcspTraceInfo(("DNS failover config validation passed\n"));
        return TRUE;
    }

    DNSFailover_ReportRejected(DNSFailover_GetValidationReason(candidate, invalidParameter));
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
    DNS_FAILOVER_CONFIG* active = DNSFailover_GetActiveConfig();
    DNS_FAILOVER_CONFIG* candidate = DNSFailover_GetCandidateConfig();
    const char* invalidParameter = NULL;
    ULONG previousEnable;

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!DNSFailover_ValidateConfig(candidate, &invalidParameter))
    {
        DNSFailover_ReportRejected(DNSFailover_GetValidationReason(candidate, invalidParameter));
        return 1;
    }

    if (!DNSFailover_WriteConfig(candidate))
    {
        DNSFailover_ReportRejected("NVRAM write failed");
        return 1;
    }

    previousEnable = active->Enable;
    *active = *candidate;

    /* The file is committed; notify the manager to reload the complete config. */
    DNSFailover_NotifyManager();

    if (previousEnable != active->Enable)
    {
        t2_event_s(active->Enable ? DNS_FAILOVER_T2_ENABLED : DNS_FAILOVER_T2_DISABLED, "override");
    }

    CcspTraceInfo(("DNS failover config committed to NVRAM: Enable=%lu, ResolverSource=%s, ReplyDeadline=%lu, FailureEpisodeGap=%lu, FailureThreshold=%lu, MonitorTick=%lu, VerifyTimeout=%lu, VerifyAttempts=%lu, VerifyCooldown=%lu, RecoveryInitial=%lu, RecoveryMax=%lu, RecoverySuccessThreshold=%lu, ResolverReload=%lu\n",
        active->Enable, active->ResolverSource, active->ReplyDeadline, active->FailureEpisodeGap,
        active->FailureThreshold, active->MonitorTick, active->VerifyTimeout, active->VerifyAttempts,
        active->VerifyCooldown, active->RecoveryInitial, active->RecoveryMax, active->RecoverySuccessThreshold,
        active->ResolverReload));

    return 0;
}

ULONG
DNSFailover_Rollback(ANSC_HANDLE hInsContext)
{
    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();
    *DNSFailover_GetCandidateConfig() = *DNSFailover_GetActiveConfig();
    CcspTraceInfo(("DNS failover config rolled back to active config\n"));
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
    DNS_FAILOVER_STATUS status;

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || !puLong || strcmp(ParamName, "ActiveResolverCount") != 0)
    {
        return FALSE;
    }

    DNSFailover_ReadStatusFile(&status);
    *puLong = status.ActiveResolverCount;
    CcspTraceInfo(("DNS failover status get: %s = %lu\n", ParamName, *puLong));
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
    DNS_FAILOVER_STATUS status;
    const char* value = NULL;

    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName) return (ULONG)-1;

    DNSFailover_ReadStatusFile(&status);

    if (strcmp(ParamName, "State") == 0) value = status.State;
    else if (strcmp(ParamName, "LastFailureTime") == 0) value = status.LastFailureTime;
    else if (strcmp(ParamName, "LastRecoveryTime") == 0) value = status.LastRecoveryTime;
    else if (strcmp(ParamName, "LastVerificationResult") == 0) value = status.LastVerificationResult;
    else return (ULONG)-1;

    CcspTraceInfo(("DNS failover status get: %s = %s\n", ParamName, value));
    return DNSFailover_ReturnString(value, pValue, pUlSize);
}
