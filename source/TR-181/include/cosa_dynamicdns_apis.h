/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2021 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#if defined(DDNS_BROADBANDFORUM)

#ifndef  _COSA_DYNAMICDNS_APIS_H
#define  _COSA_DYNAMICDNS_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

typedef struct
_COSA_DML_DDNS_CLIENT
{
   ULONG          InstanceNumber;
   char           Alias[64];
   int            Status;
   int            LastError;
   char           Server[256];
   char           Interface[256];
   char           Username[256];
   char           Password[256];
   BOOL           Enable;
}
COSA_DML_DDNS_CLIENT;
typedef struct
_COSA_DML_DDNS_HOST
{
   ULONG          InstanceNumber;
   char           Alias[64];
   int            Status;
   char           Name[256];
   char           LastUpdate[256];
   BOOL           Enable;
}
COSA_DML_DDNS_HOST;

typedef struct
_COSA_DML_DDNS_SERVER
{
   ULONG          InstanceNumber;
   ULONG          ServerPort;
   int            CheckInterval;
   int            RetryInterval;
   int            MaxRetries;
   char           Alias[64];
   char           Name[64];
   char           ServiceName[256];
   char           ServerAddress[256];
   char           SupportedProtocols[64];
   char           Protocol[16];
   BOOL           Enable;
}
COSA_DML_DDNS_SERVER;
/***************************
*  FUNCTION DECLARATION ***
***************************/

//DynamicDNS.

/**
* @brief Get the Dynamic DNS enabled status.
*
* This function retrieves whether the Dynamic DNS functionality is enabled.
*
* @return The enabled status of Dynamic DNS.
* @retval TRUE(1) if Dynamic DNS is enabled.
* @retval FALSE(0) if Dynamic DNS is disabled.
*
*/
BOOL  CosaDmlDynamicDns_GetEnable();

/**
* @brief Get the list of supported Dynamic DNS services.
*
* This function retrieves a comma-separated list of supported Dynamic DNS service providers.
*
* @param[out] supportedServices - Pointer to a buffer where the list of supported services will be returned.
*                                 \n The buffer should be allocated by the caller with sufficient size.
*
* @return Status code indicating success or failure.
* @retval 0  If the operation is successful and the services list was retrieved.
* @retval -1 If the operation fails.
*
*/
ULONG CosaDmlDynamicDns_GetsupportedServices(char *supportedServices);

/**
* @brief Set the Dynamic DNS enabled status.
*
* This function enables or disables the Dynamic DNS functionality.
*
* @param[in] bValue - Boolean flag indicating whether to enable (TRUE) or disable (FALSE) Dynamic DNS.
*
* @return Status code indicating success or failure.
* @retval 0 if Dynamic DNS status set successfully.
* @retval -1 if failure in setting DynamicDns
*
*/
ULONG CosaDmlDynamicDns_SetEnable(BOOL bValue);

//DynamicDNS.Client.{i}

/**
* @brief Get the number of Dynamic DNS client entries.
*
* This function retrieves the count of Dynamic DNS client entries.
*
* @return The number of DDNS client entries.
* @retval Number of Dynamic DNS client entries on success.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ULONG CosaDmlDynamicDns_Client_GetNumberOfEntries(void);

/**
* @brief Get a Dynamic DNS client entry by index.
*
* This function retrieves a Dynamic DNS client entry at the specified index.
*
* @param[in] index - Zero-based index of the client entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DDNS_CLIENT structure where the configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Client_GetEntryByIndex(ULONG index, COSA_DML_DDNS_CLIENT *pEntry);

/**
* @brief Set values for a Dynamic DNS client entry.
*
* This function updates the instance number and alias for a DDNS client entry.
*
* @param[in] index - Zero-based index of the client entry to update.
* @param[in] ins - New instance number to assign to the entry.
* @param[in] alias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Client_SetValues(ULONG index, ULONG ins, const char *alias);

/**
* @brief Add a new Dynamic DNS client entry.
*
* This function adds a new Dynamic DNS client configuration entry.
*
* @param[in] pEntry - Pointer to a COSA_DML_DDNS_CLIENT structure containing the client configuration
*                     to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Client_AddEntry(COSA_DML_DDNS_CLIENT *pEntry);

/**
* @brief Delete a Dynamic DNS client entry.
*
* This function removes a Dynamic DNS client configuration entry.
*
* @param[in] ins - Instance number of the client entry to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Client_DelEntry(ULONG ins);

/**
* @brief Get the configuration of a Dynamic DNS client.
*
* This function retrieves the configuration of a Dynamic DNS client entry.
*
* @param[in] ins - Instance number of the client entry to query.
* @param[out] pEntry - Pointer to a COSA_DML_DDNS_CLIENT structure where the configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Client_GetConf(ULONG ins, COSA_DML_DDNS_CLIENT *pEntry);

/**
* @brief Set the configuration of a Dynamic DNS client.
*
* This function updates the configuration of a Dynamic DNS client entry.
*
* @param[in] ins - Instance number of the client entry to update.
* @param[in] pEntry - Pointer to a COSA_DML_DDNS_CLIENT structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Client_SetConf(ULONG ins, COSA_DML_DDNS_CLIENT *pEntry);

/**
* @brief Get the Dynamic DNS client status.
*
* This function retrieves the current operational status of the Dynamic DNS client.
*
* @return The client status code.
*
*/
ULONG CosaDmlDynamicDns_GetClientStatus();

/**
* @brief Get the Dynamic DNS client last error.
*
* This function retrieves the last error code encountered by the Dynamic DNS client.
*
* @return The last error code.
*
*/
ULONG CosaDmlDynamicDns_GetClientLastError();

//DynamicDNS.Client.{i}.Host.{i}

/**
* @brief Get the number of Dynamic DNS host entries.
*
* This function retrieves the count of Dynamic DNS host entries.
*
* @return The number of DDNS host entries.
*
*/
ULONG CosaDmlDynamicDns_Host_GetNumberOfEntries(void);

/**
* @brief Get a Dynamic DNS host entry by index.
*
* This function retrieves a Dynamic DNS host entry at the specified index.
*
* @param[in] index - Zero-based index of the host entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DDNS_HOST structure where the configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Host_GetEntryByIndex(ULONG index, COSA_DML_DDNS_HOST *pEntry);

/**
* @brief Set values for a Dynamic DNS host entry.
*
* This function updates the instance number and alias for a DDNS host entry.
*
* @param[in] index - Zero-based index of the host entry to update.
* @param[in] ins - New instance number to assign to the entry.
* @param[in] alias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Host_SetValues(ULONG index, ULONG ins, const char *alias);

/**
* @brief Add a new Dynamic DNS host entry.
*
* This function adds a new Dynamic DNS host configuration entry.
*
* @param[in] pEntry - Pointer to a COSA_DML_DDNS_HOST structure containing the host configuration
*                     to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Host_AddEntry(COSA_DML_DDNS_HOST *pEntry);

/**
* @brief Delete a Dynamic DNS host entry.
*
* This function removes a Dynamic DNS host configuration entry.
*
* @param[in] ins - Instance number of the host entry to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Host_DelEntry(ULONG ins);

/**
* @brief Get the configuration of a Dynamic DNS host.
*
* This function retrieves the configuration of a Dynamic DNS host entry.
*
* @param[in] ins - Instance number of the host entry to query.
* @param[out] pEntry - Pointer to a COSA_DML_DDNS_HOST structure where the configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Host_GetConf(ULONG ins, COSA_DML_DDNS_HOST *pEntry);

/**
* @brief Set the configuration of a Dynamic DNS host.
*
* This function updates the configuration of a Dynamic DNS host entry.
*
* @param[in] ins - Instance number of the host entry to update.
* @param[in] pEntry - Pointer to a COSA_DML_DDNS_HOST structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Host_SetConf(ULONG ins, COSA_DML_DDNS_HOST *pEntry);

/**
* @brief Get the last successful update timestamp for a Dynamic DNS host.
*
* This function retrieves the timestamp of the last successful update of the hostname to the Dynamic DNS server.
*
* @param[out] lastUpdate - Pointer to a buffer where the last update timestamp will be returned.
*
* @return The number of characters written to the buffer.
* @retval 0 The timestamp was successfully retrieved and written to the lastUpdate buffer.
* @retval -1 An error occurred during retrieval.
*
*/
ULONG CosaDmlDynamicDns_GetHostLastUpdate(char*  lastUpdate);

/**
* @brief Initialize TR-181 Dynamic DNS host configuration.
*
* This function initializes the TR-181 data model for Dynamic DNS host entries.
*
* @return None.
*
*/
void CosaInitializeTr181DdnsHost();

//DynamicDNS.Server.{i}.

/**
* @brief Get the number of Dynamic DNS server entries.
*
* This function retrieves the count of Dynamic DNS server entries.
*
* @return The number of DDNS server entries.
*
*/
ULONG CosaDmlDynamicDns_Server_GetNumberOfEntries(void);

/**
* @brief Get a Dynamic DNS server entry by index.
*
* This function retrieves a Dynamic DNS server entry at the specified index.
*
* @param[in] index - Zero-based index of the server entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DDNS_SERVER structure where the configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Server_GetEntryByIndex(ULONG index, COSA_DML_DDNS_SERVER *pEntry);

/**
* @brief Set values for a Dynamic DNS server entry.
*
* This function updates the instance number and alias for a DDNS server entry.
*
* @param[in] index - Zero-based index of the server entry to update.
* @param[in] ins - New instance number to assign to the entry.
* @param[in] alias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Server_SetValues(ULONG index, ULONG ins, const char *alias);

/**
* @brief Add a new Dynamic DNS server entry.
*
* This function adds a new Dynamic DNS server configuration entry.
*
* @param[in] pEntry - Pointer to a COSA_DML_DDNS_SERVER structure containing the server configuration
*                     to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Server_AddEntry(COSA_DML_DDNS_SERVER *pEntry);

/**
* @brief Delete a Dynamic DNS server entry.
*
* This function removes a Dynamic DNS server configuration entry.
*
* @param[in] ins - Instance number of the server entry to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Server_DelEntry(ULONG ins);

/**
* @brief Get the configuration of a Dynamic DNS server.
*
* This function retrieves the configuration of a Dynamic DNS server entry.
*
* @param[in] ins - Instance number of the server entry to query.
* @param[out] pEntry - Pointer to a COSA_DML_DDNS_SERVER structure where the configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Server_GetConf(ULONG ins, COSA_DML_DDNS_SERVER *pEntry);

/**
* @brief Set the configuration of a Dynamic DNS server.
*
* This function updates the configuration of a Dynamic DNS server entry.
*
* @param[in] ins - Instance number of the server entry to update.
* @param[in] pEntry - Pointer to a COSA_DML_DDNS_SERVER structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS CosaDmlDynamicDns_Server_SetConf(ULONG ins, COSA_DML_DDNS_SERVER *pEntry);

/**
* @brief Initialize TR-181 Dynamic DNS service provider list.
*
* This function initializes the TR-181 data model for Dynamic DNS service provider entries.
*
* @return None.
*
*/
void CosaInitializeTr181DdnsServiceProviderList();
#endif  //_COSA_DYNAMICDNS_APIS_H


#endif
