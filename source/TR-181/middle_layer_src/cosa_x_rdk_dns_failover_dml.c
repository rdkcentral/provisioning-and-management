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
    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();

    if (!ParamName || !puLong) return FALSE;
    return DNSFailover_GetUlongByName(DNSFailover_GetCandidateConfig(), ParamName, puLong);
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

    return DNSFailover_ReturnString(DNSFailover_GetCandidateConfig()->ResolverSource, pValue, pUlSize);
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

    return DNSFailover_CopyString(candidate->ResolverSource, sizeof(candidate->ResolverSource), pString);
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
    DNS_FAILOVER_STATUS* status = DNSFailover_GetRuntimeStatus();
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

    DNSFailover_CopyString(status->State, sizeof(status->State),
        active->Enable ? "Healthy" : "Disabled");

    if (previousEnable != active->Enable)
    {
        t2_event_s(active->Enable ? DNS_FAILOVER_T2_ENABLED : DNS_FAILOVER_T2_DISABLED, "override");
    }

    return 0;
}

ULONG
DNSFailover_Rollback(ANSC_HANDLE hInsContext)
{
    UNREFERENCED_PARAMETER(hInsContext);
    DNSFailover_EnsureLoaded();
    *DNSFailover_GetCandidateConfig() = *DNSFailover_GetActiveConfig();
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

    return DNSFailover_ReturnString(value, pValue, pUlSize);
}

BOOL
DNSFailoverStatus_SetState(const char* state)
{
    DNS_FAILOVER_STATUS* status = DNSFailover_GetRuntimeStatus();

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

    return DNSFailover_CopyString(status->State, sizeof(status->State), state);
}

void
DNSFailoverStatus_SetActiveResolverCount(ULONG resolverCount)
{
    DNSFailover_EnsureLoaded();
    DNSFailover_GetRuntimeStatus()->ActiveResolverCount = resolverCount;
}

void
DNSFailoverTelemetry_ReportFailure(ULONG resolverCount, const char* addressFamilies)
{
    DNS_FAILOVER_STATUS* status = DNSFailover_GetRuntimeStatus();

    DNSFailover_EnsureLoaded();
    status->ActiveResolverCount = resolverCount;
    DNSFailover_CopyString(status->State, sizeof(status->State), "Failed");
    DNSFailover_SetTimestamp(status->LastFailureTime, sizeof(status->LastFailureTime));

    CcspTraceWarning(("DNS complete failure confirmed: resolvers=%lu families=%s time=%s\n",
        resolverCount, addressFamilies ? addressFamilies : "unknown",
        status->LastFailureTime));
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
    DNS_FAILOVER_STATUS* status = DNSFailover_GetRuntimeStatus();

    DNSFailover_EnsureLoaded();

    if (!result || !result[0] || strlen(result) >= sizeof(status->LastVerificationResult))
    {
        CcspTraceError(("DNS active verification reported an invalid result\n"));
        return;
    }

    DNSFailover_CopyString(status->LastVerificationResult,
        sizeof(status->LastVerificationResult), result);
    CcspTraceWarning(("DNS active verification: result=%s resolver=%s family=%s\n",
        result, resolverIp ? resolverIp : "unknown",
        addressFamily ? addressFamily : "unknown"));
}

void
DNSFailoverTelemetry_ReportUnboundFailover(BOOL enabled)
{
    DNS_FAILOVER_STATUS* status = DNSFailover_GetRuntimeStatus();

    DNSFailover_EnsureLoaded();
    DNSFailover_CopyString(status->State, sizeof(status->State), enabled ? "FailoverActive" : "Healthy");
    CcspTraceWarning(("DNS Unbound failover %s\n", enabled ? "started" : "stopped"));
    t2_event_d(enabled ? DNS_FAILOVER_T2_STARTED : DNS_FAILOVER_T2_STOPPED, 1);
}

void
DNSFailoverTelemetry_ReportRecovery(const char* resolverIp, const char* addressFamily)
{
    DNS_FAILOVER_STATUS* status = DNSFailover_GetRuntimeStatus();

    DNSFailover_EnsureLoaded();
    DNSFailover_CopyString(status->State, sizeof(status->State), "Healthy");
    DNSFailover_SetTimestamp(status->LastRecoveryTime, sizeof(status->LastRecoveryTime));

    CcspTraceWarning(("DNS resolver recovered: resolver=%s family=%s time=%s\n",
        resolverIp ? resolverIp : "unknown",
        addressFamily ? addressFamily : "unknown",
        status->LastRecoveryTime));
    t2_event_d(DNS_FAILOVER_T2_RECOVERED, 1);
}
