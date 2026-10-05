#ifndef _COSA_X_RDK_DNS_FAILOVER_DML_H
#define _COSA_X_RDK_DNS_FAILOVER_DML_H

#include "cosa_apis.h"

BOOL
DNSFailover_GetParamUlongValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        ULONG*      puLong
    );

ULONG
DNSFailover_GetParamStringValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        char*       pValue,
        ULONG*      pUlSize
    );

BOOL
DNSFailover_SetParamUlongValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        ULONG       uValue
    );

BOOL
DNSFailover_SetParamStringValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        char*       pString
    );

BOOL
DNSFailover_Validate
    (
        ANSC_HANDLE hInsContext,
        char*       pReturnParamName,
        ULONG*      puLength
    );

ULONG DNSFailover_Commit(ANSC_HANDLE hInsContext);
ULONG DNSFailover_Rollback(ANSC_HANDLE hInsContext);

BOOL
DNSFailoverStatus_GetParamUlongValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        ULONG*      puLong
    );

ULONG
DNSFailoverStatus_GetParamStringValue
    (
        ANSC_HANDLE hInsContext,
        char*       ParamName,
        char*       pValue,
        ULONG*      pUlSize
    );

BOOL DNSFailoverStatus_SetState(const char* state);
void DNSFailoverStatus_SetActiveResolverCount(ULONG resolverCount);
void DNSFailoverTelemetry_ReportFailure(ULONG resolverCount, const char* addressFamilies);
void DNSFailoverTelemetry_ReportVerification(const char* result, const char* resolverIp, const char* addressFamily);
void DNSFailoverTelemetry_ReportUnboundFailover(BOOL enabled);
void DNSFailoverTelemetry_ReportRecovery(const char* resolverIp, const char* addressFamily);

#endif