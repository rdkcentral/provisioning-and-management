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
