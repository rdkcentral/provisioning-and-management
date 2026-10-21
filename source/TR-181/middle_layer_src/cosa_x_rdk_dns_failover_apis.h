#ifndef _COSA_X_RDK_DNS_FAILOVER_APIS_H
#define _COSA_X_RDK_DNS_FAILOVER_APIS_H

#include "cosa_apis.h"

#define DNS_FAILOVER_CONFIG_FILE      "/nvram/dns_failover.conf"
#define DNS_FAILOVER_CONFIG_TEMP_FILE "/nvram/dns_failover.conf.tmp"
#define DNS_FAILOVER_MANAGER_PID_FILE "/tmp/dns_failover_manager.pid"
#define DNS_FAILOVER_STATUS_FILE      "/tmp/dns_failover_status.txt"
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

typedef struct
{
    char  State[DNS_FAILOVER_STATUS_VALUE_SIZE];
    ULONG ActiveResolverCount;
    char  LastFailureTime[DNS_FAILOVER_TIMESTAMP_SIZE];
    char  LastRecoveryTime[DNS_FAILOVER_TIMESTAMP_SIZE];
    char  LastVerificationResult[DNS_FAILOVER_STATUS_VALUE_SIZE];
} DNS_FAILOVER_STATUS;

/* Loads the active config from NVRAM on first use; safe to call from every DML entry point. */
void DNSFailover_EnsureLoaded(void);

/* Accessors into the module-private active/candidate/status state owned by apis.c. */
DNS_FAILOVER_CONFIG* DNSFailover_GetActiveConfig(void);
DNS_FAILOVER_CONFIG* DNSFailover_GetCandidateConfig(void);
DNS_FAILOVER_STATUS* DNSFailover_GetRuntimeStatus(void);

BOOL DNSFailover_LoadConfig(DNS_FAILOVER_CONFIG* config, BOOL* usedOverride);
BOOL DNSFailover_WriteConfig(const DNS_FAILOVER_CONFIG* config);
BOOL DNSFailover_ValidateConfig(const DNS_FAILOVER_CONFIG* config, const char** invalidParameter);
const char* DNSFailover_GetValidationReason(const DNS_FAILOVER_CONFIG* config, const char* invalidParameter);

BOOL DNSFailover_SetUlongByName(DNS_FAILOVER_CONFIG* config, const char* name, ULONG value);
BOOL DNSFailover_GetUlongByName(const DNS_FAILOVER_CONFIG* config, const char* name, ULONG* value);

void DNSFailover_ReadStatusFile(DNS_FAILOVER_STATUS* status);
void DNSFailover_NotifyManager(void);
void DNSFailover_ReportRejected(const char* reason);

BOOL DNSFailover_CopyString(char* destination, size_t destinationSize, const char* source);
void DNSFailover_SetTimestamp(char* destination, size_t destinationSize);
ULONG DNSFailover_ReturnString(const char* source, char* destination, ULONG* destinationSize);
BOOL DNSFailover_ParseUlong(const char* value, ULONG* result);

#endif
