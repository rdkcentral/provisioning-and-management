/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef  _COSA_DHCPV4_WEBCONFIG_APIS_H
#define  _COSA_DHCPV4_WEBCONFIG_APIS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "ansc_status.h"
#include "cosa_apis.h"
#include "cosa_dhcpv4_apis.h"
#include "cosa_dhcpv4_internal.h"
#include "cosa_lanmanagement_apis.h"
#include "cosa_lanmanagement_internal.h"
#include "cosa_x_cisco_com_devicecontrol_apis.h"
#include "cosa_x_cisco_com_devicecontrol_internal.h"
#include "webconfig_framework.h"
#include "macbinding_webconfig_param.h"
#include "lan_webconfig_param.h"

#define EMPTY_BLOB  199
#define NAME_MAX_SIZE 32
#define VAL_MAX_SIZE  256
#define DHCPV4_CACHE_MAX_NUM_OF_PARAM 1024

#define DHCPV4_NUM_STATIC_HOSTS "dhcp_num_static_hosts"
#define DHCPV4_STATIC_HOSTS_INSNUM "dhcp_static_host_insNum_"
#define DHCPV4_STATIC_HOST "dhcp_static_host_"
#define DHCPV4_COMMENTS_LAN_CLIENTS "lan_clients_"
#define DHCPV4_COMMENTS_LAN_CLIENTS_COUNT "lan_clients_count"
#define DHCPV4_COMMENTS "lan_clients_mac_"
#define DHCPV4_STATIC_HOST_ALIAS "dhcp_static_host_alias_"


#define DHCPV4_SERVER_ENABLED "dhcp_server_enabled"
#define DHCPV4_SERVER_START_IP "dhcp_start"
#define DHCPV4_SERVER_END_IP "dhcp_end"
#define DHCPV4_LEASE_TIME "dhcp_lease_time"
#define DHCPV4_LAN_IP "lan_ipaddr"
#define DHCPV4_LAN_NETMASK "lan_netmask"


typedef struct Dhcpv4Cache
{
    char cmd[NAME_MAX_SIZE];
    char val[VAL_MAX_SIZE];
}Dhcpv4_Cache_t;

/**
* @brief Validate an IP address string.
*
* This function checks whether the provided IP address string is a valid IPv4 address
* using standard format validation.
*
* @param[in] ipAddress - Pointer to a null-terminated string containing the IP address to validate.
*
* @return The validation status of the IP address.
* @retval VALID_IP if the IP address is valid.
* @retval INVALID_IP if the IP address is invalid or NULL.
*
*/
int CheckIpIsValid( char *ipAddress );

/**
* @brief Convert error code to error message string.
*
* This function converts a numeric error code into a human-readable error message string.
*
* @param[in] errCode - The error code to convert.
*                      \n Possible values include: INVALID_IP, INVALID_MAC, STATIC_IP_NOT_IN_LANRANGE,
*                      LAN_PARAM_IP_RANGE_INVALID, LAN_PARAM_NETMASK_INVALID, LAN_PARAM_LEASETIME_INVALID,
*                      COSA_SYNCHRONIZE_FAILED, LAN_PARAM_GATEWAY_IP_INVALID.
*
* @return Pointer to a null-terminated string containing the error message.
*
*/
char* ConvertErrCodeToErrMsg(int errCode);

/**
* @brief Synchronize DHCPv4 static clients with COSA data model.
*
* This function synchronizes the DHCPv4 static client address entries from the system configuration
* into the COSA data model objects.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval -1 if the operation fails.
*
*/
int Dhcpv4_StaticClients_Synchronize();

/**
* @brief Process webconfig request for static client entries.
*
* This function is the callback API to execute static client blob requests received from webconfig.
*
* @param[in] Data - Pointer to macbindingdoc_t structure containing the static client configuration data.
*
* @return Pointer to an Err structure containing the result of the operation.
* @retval BLOB_EXEC_SUCCESS if the operation is successful.
* @retval VALIDATION_FALIED if validation fails.
* @retval SYSCFG_FAILURE if system configuration operation fails.
* @retval COSA_SYNCHRONIZE_FAILED if synchronization with COSA objects fails.
*
*/
pErr Process_StaticClients_WebConfigRequest(void *Data);

/**
* @brief Rollback static clients configuration to previous state.
*
* This function is the callback to rollback static client configuration when blob execution fails.
*
* @return The status of the operation.
* @retval 0 if the rollback is successful.
*
*/
int rollback_StaticClients();

/**
* @brief Free resources allocated for static clients blob data.
*
* This function is the callback to free resources allocated during static clients blob processing.
*
* @param[in] arg - Pointer to execData structure containing the blob execution data to be freed.
*
* @return None.
*/
void FreeResources_StaticClients(void *arg);

/**
* @brief Process webconfig request for LAN configuration.
*
* This function is the callback API to execute LAN blob requests received from webconfig.
*
* @param[in] Data - Pointer to landoc_t structure containing the LAN configuration data.
*
* @return Pointer to an Err structure containing the result of the operation.
* @retval BLOB_EXEC_SUCCESS if the operation is successful.
* @retval VALIDATION_FALIED if validation fails.
* @retval SYSCFG_FAILURE if system configuration operation fails.
* @retval COSA_SYNCHRONIZE_FAILED if synchronization with COSA objects fails.
*
*/
pErr Process_Lan_WebConfigRequest(void *Data);

/**
* @brief Rollback LAN configuration to previous state.
*
* This function is the callback to rollback LAN configuration when blob execution fails.
*
* @return The status of the operation.
* @retval 0 if the rollback is successful.
*
*/
int rollback_Lan();

/**
* @brief Free resources allocated for LAN blob data.
*
* This function is the callback to free resources allocated during LAN blob processing.
*
* @param[in] arg - Pointer to execData structure containing the blob execution data to be freed.
*
* @return None.
*
*/
void FreeResources_Lan(void *arg);

/**
* @brief Lock the static clients mutex.
*
* This function acquires the mutex lock for protecting static clients configuration operations.
*
* @return The status of the mutex lock operation.
* @retval 0 if the lock is successfully acquired.
* @retval non-zero error code otherwise.
*
*/
int Dhcpv4_StaticClients_MutexLock();

/**
* @brief Unlock the static clients mutex.
*
* This function releases the mutex lock for static clients configuration operations.
*
* @return The status of the mutex unlock operation.
* @retval 0 if the lock is successfully released.
* @retval non-zero error code otherwise.
*
*/
int Dhcpv4_StaticClients_MutexUnLock();

/**
* @brief Try to lock the static clients mutex without blocking.
*
* This function attempts to acquire the static clients mutex lock without blocking.
* If the mutex is already locked, it returns immediately.
*
* @return The status of the mutex trylock operation.
* @retval 0 if the lock is successfully acquired.
* @retval non-zero error code otherwise.
*
*/
int Dhcpv4_StaticClients_MutexTryLock();

/**
* @brief Lock the LAN configuration mutex.
*
* This function acquires the mutex lock for protecting LAN configuration operations.
*
* @return The status of the mutex lock operation.
* @retval 0 if the lock is successfully acquired.
* @retval non-zero  error code otherwise.
*
*/
int Dhcpv4_Lan_MutexLock();

/**
* @brief Unlock the LAN configuration mutex.
*
* This function releases the mutex lock for LAN configuration operations.
*
* @return The status of the mutex unlock operation.
* @retval 0 if the lock is successfully released.
* @retval non-zero  error code otherwise.
*
*/
int Dhcpv4_Lan_MutexUnLock();

/**
* @brief Try to lock the LAN configuration mutex without blocking.
*
* This function attempts to acquire the LAN configuration mutex lock without blocking.
* If the mutex is already locked, it returns immediately.
*
* @return The status of the mutex trylock operation.
* @retval 0 if the lock is successfully acquired.
* @retval non-zero error code otherwise.
*
*/
int Dhcpv4_Lan_MutexTryLock();

/**
* @brief Validate DHCP lease time value.
*
* This function checks whether the provided lease time is valid according to supported time units.
* Valid lease times must be multiples of weeks (604800s), days (86400s), hours (3600s), or minutes (60s),
* or fall within the range of 120-999 seconds, or be set to -1 for forever.
*
* @param[in] leasetime - The lease time value in seconds to validate.
*                        \n Valid values: multiples of 60, 3600, 86400, or 604800; 120-999; or (uint32_t)-1.
*
* @return The validation status.
* @retval TRUE if the lease time is valid.
* @retval FALSE if the lease time is invalid.
*
*/
BOOL Dhcpv4_Lan_CheckIsValidLeaseTime(uint32_t leasetime);

#if defined (WIFI_MANAGE_SUPPORTED)
/**
* @brief Check if IP address is within Managed WiFi address range.
*
* This function determines whether the specified IP address falls within the Managed WiFi
* bridge IP address range.
*
* @param[in] ulAddr - The IP address in hexadecimal format to check.
*
* @return The status of the operation.
* @retval TRUE if the address is within the Managed WiFi range.
* @retval FALSE if the address is outside the Managed WiFi range.
*
*/
int Dhcpv4_Lan_Ip_IsInManageWiFi_AddrRange(ULONG ulAddr);
#endif /*WIFI_MANAGE_SUPPORTED*/
#endif
