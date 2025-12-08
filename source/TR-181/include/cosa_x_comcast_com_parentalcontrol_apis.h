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
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

#ifndef _COSA_X_COMCAST_COM_PARENTALCONTROL_APIS_H
#define _COSA_X_COMCAST_COM_PARENTALCONTROL_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

typedef enum
{
    IPADDR_IPV4 = 1,
    IPADDR_IPV6 = 2,
}
COSA_DML_IPADDR_TYPE;

typedef enum
{
    PROTO_TCP = 1,
    PROTO_UDP = 2,
    PROTO_BOTH = 3,
}
COSA_DML_PROTO_TYPE;

typedef enum
{
    BLOCK_METHOD_URL = 1,
    BLOCK_METHOD_KEYWORD = 2,
}
COSA_DML_BLOCK_METHOD;

typedef enum
{
    MD_TYPE_BLOCK = 1,
    MD_TYPE_ALLOW = 2,
}
COSA_DML_MD_TYPE;


/*
 * .ManagedSites.
 */
typedef struct
_COSA_DML_MANAGEDSITES
{
    BOOL            Enable;
}
COSA_DML_MANAGEDSITES;

/*
 * .ManagedSites.BlockedURL.{i}.
 */
typedef struct
_COSA_DML_BLOCKEDURL
{
    ULONG           InstanceNumber;
    char            Alias[256];

    COSA_DML_BLOCK_METHOD BlockMethod;
    char            Site[1025];
    BOOL            AlwaysBlock;
    char            StartTime[64];
    char            EndTime[64];
    BOOL            StartTimeFlg;
    BOOL            EndTimeFlg;
    char            BlockDays[64];
    char            MAC[32];
    char            DeviceName[128];
}
COSA_DML_BLOCKEDURL;

/*
 * .ManagedSites.TrustedUser.{i}.
 */
typedef struct
_COSA_DML_TRUSTEDUSER
{
    ULONG                   InstanceNumber;
    char                    Alias[256];

    char                    HostDescription[64];
    COSA_DML_IPADDR_TYPE    IPAddressType;
    char                    IPAddress[64];
    BOOL                    Trusted;
}
COSA_DML_TRUSTEDUSER;

/*
 * .ManagedServices.
 */
typedef struct
_COSA_DML_MANAGED_SERVS
{
    BOOL            Enable;
}
COSA_DML_MANAGED_SERVS;

/*
 * .ManagedServices.Service.{i}.
 */
typedef struct
_COSA_DML_MS_SERV
{
    ULONG           InstanceNumber;
    char            Alias[256];

    char            Description[64];
    COSA_DML_PROTO_TYPE Protocol;
    ULONG           StartPort;
    ULONG           EndPort;
    BOOL            AlwaysBlock;
    char            StartTime[64];
    char            EndTime[64];
    char            BlockDays[64];
}
COSA_DML_MS_SERV;

/*
 * .ManagedServices.TrustedUser.{i}.
 */
typedef struct
_COSA_DML_MS_TRUSTEDUSER
{
    ULONG           InstanceNumber;
    char            Alias[256];

    char                    HostDescription[64];
    COSA_DML_IPADDR_TYPE    IPAddressType;
    char                    IPAddress[64];
    BOOL                    Trusted;
}
COSA_DML_MS_TRUSTEDUSER;

/*
 * .ManagedDevices.Redirect
 */
typedef struct
_COSA_DML_MD_RED
{
    char                        HTTP_Server_IP[17];
    char                        HTTPS_Server_IP[17];
    char                        Default_Server_IP[17];
    char                        HTTP_Server_IPv6[64];
    char                        HTTPS_Server_IPv6[64];
    char                        Default_Server_IPv6[64];
}
COSA_DML_MD_RED;
/*
 * .ManagedDevices.
 */
typedef struct
_COSA_DML_MANAGED_DEVS
{
    BOOL            Enable;
    BOOL            AllowAll;
}
COSA_DML_MANAGED_DEVS;

/*
 * .ManagedDevices.Device.{i}.
 */
typedef struct
_COSA_DML_MD_DEV
{
    ULONG           InstanceNumber;
    char            Alias[256];

    COSA_DML_MD_TYPE Type;
    char            Description[64];
    char            MACAddress[64];
    BOOL            AlwaysBlock;
    char            StartTime[64];
    char            EndTime[64];
    char            BlockDays[64];
}
COSA_DML_MD_DEV;

/**
* @brief Initialize the Parental Control subsystem.
*
* This function initialize the Parental Control subsystem
*
* @param[in] hDml  - Handle to the DML object.
* @param[in,out] phContext  - Pointer to a context handle for storing initialization state.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if initialization is successful.
* @retval ANSC_STATUS_FAILURE if initialization fails.
*
*/
ANSC_STATUS
CosaDmlParentalControlInit(ANSC_HANDLE hDml, PANSC_HANDLE phContext);

/**
* @brief Get the Managed Sites configuration.
*
* This function retrieves the enable/disable status from Utopia storage.
*
* @param[out] conf  - Pointer to a COSA_DML_MANAGEDSITES structure where the configuration will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMngSites_GetConf(COSA_DML_MANAGEDSITES *conf);

/**
* @brief Set the Managed Sites configuration.
*
* This function stores the configuration in Utopia storage.
*
* @param[in] conf  - Pointer to a COSA_DML_MANAGEDSITES structure containing the configuration to be set.
*                  \n The Enable field determines whether to enable or disable the Managed Sites feature.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is set successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMngSites_SetConf(COSA_DML_MANAGEDSITES *conf);

/**
* @brief Get the Managed Services configuration.
*
* This function retrieves the enable/disable status from Utopia storage.
*
* @param[out] conf  - Pointer to a COSA_DML_MANAGED_SERVS structure where the configuration will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMngServs_GetConf(COSA_DML_MANAGED_SERVS *conf);

/**
* @brief Set the Managed Services configuration.
*
* This function stores the configuration in Utopia storage.
*
* @param[in] conf  - Pointer to a COSA_DML_MANAGED_SERVS structure containing the configuration to be set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is set successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMngServs_SetConf(COSA_DML_MANAGED_SERVS *conf);

/**
* @brief Get the Managed Devices configuration.
*
* This function retrieves the configuration from Utopia storage.
*
* @param[out] conf  - Pointer to a COSA_DML_MANAGED_DEVS structure where the configuration will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMngDevs_GetConf(COSA_DML_MANAGED_DEVS *conf);

/**
* @brief Set the Managed Devices configuration.
*
* This function stores the configuration in Utopia storage.
*
* @param[in] conf  - Pointer to a COSA_DML_MANAGED_DEVS structure containing the configuration to be set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is set successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMngDevs_SetConf(COSA_DML_MANAGED_DEVS *conf);

/**
* @brief Get the number of Blocked URL entries.
*
* This function retrieves the count from Utopia storage.
*
* @return The status of the operation or number of Blocked URL entries.
* @retval The number of Blocked URL entries currently configured.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ULONG
CosaDmlBlkURL_GetNumberOfEntries(void);

/**
* @brief Get a Blocked URL entry by index.
*
* This function retrieves the entry from Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_BLOCKEDURL structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlBlkURL_GetEntryByIndex(ULONG index, COSA_DML_BLOCKEDURL *pEntry);

/**
* @brief Set the instance number and alias for a Blocked URL entry at a specific index.
*
* This function updates the instance number and alias for a Blocked URL entry in Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to update.
* @param[in] ins  - The instance number to assign to the entry.
* @param[in] alias  - Pointer to a null-terminated string containing the alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the values are set successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlBlkURL_SetValues(ULONG index, ULONG ins, const char *alias);

/**
* @brief Add a new Blocked URL entry.
*
* This function stores the entry in Utopia storage.
*
* @param[in] pEntry  - Pointer to a COSA_DML_BLOCKEDURL structure containing the entry to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is added successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlBlkURL_AddEntry(COSA_DML_BLOCKEDURL *pEntry);

/**
* @brief Delete a Blocked URL entry by instance number.
*
* This function removes the entry from Utopia storage.
*
* @param[in] ins  - The instance number of the entry to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is deleted successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlBlkURL_DelEntry(ULONG ins);

/**
* @brief Get a Blocked URL entry by instance number.
*
* This function locates or retrieve the entry by instance number.
*
* @param[in] ins  - The instance number of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_BLOCKEDURL structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlBlkURL_GetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry);

/**
* @brief Set the configuration for a Blocked URL entry by instance number.
*
* This function updates the configuration for a Blocked URL entry in Utopia storage.
*
* @param[in] ins  - The instance number of the entry to update.
* @param[in] pEntry  - Pointer to a COSA_DML_BLOCKEDURL structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is set successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlBlkURL_SetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry);

/**
* @brief Get the number of Trusted User entries for Managed Sites.
*
* This function retrieves the count from Utopia storage.
*
* @return The status of the operation or number of Trusted User entries.
* @retval The number of Trusted User entries currently configured.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ULONG
CosaDmlTrustedUser_GetNumberOfEntries(void);

/**
* @brief Get a Trusted User entry by index.
*
* This function retrieves the entry from Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_TRUSTEDUSER structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_TRUSTEDUSER *pEntry);

/**
* @brief Set the instance number and alias for a Trusted User entry at a specific index.
*
* This function updates instance number and alias for a Trusted User entry in Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to update.
* @param[in] ins  - The instance number to assign to the entry.
* @param[in] alias  - Pointer to a null-terminated string containing the alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the values are set successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias);

/**
* @brief Add a new Trusted User entry for Managed Sites.
*
* This function stores the new entry in Utopia storage.
*
* @param[in] pEntry  - Pointer to a COSA_DML_TRUSTEDUSER structure containing the entry to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is added successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlTrustedUser_AddEntry(COSA_DML_TRUSTEDUSER *pEntry);

/**
* @brief Delete a Trusted User entry by instance number.
*
* This function removes the entry from Utopia storage.
*
* @param[in] ins  - The instance number of the entry to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is deleted successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlTrustedUser_DelEntry(ULONG ins);

/**
* @brief Get a Trusted User entry by instance number.
*
* This function locates or retrieve the entry by instance number.
*
* @param[in] ins  - The instance number of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_TRUSTEDUSER structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlTrustedUser_GetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry);

/**
* @brief Set the configuration for a Trusted User entry by instance number.
*
* This function updates the configuration for a Trusted User entry in Utopia storage.
*
* @param[in] ins  - The instance number of the entry to update.
* @param[in] pEntry  - Pointer to a COSA_DML_TRUSTEDUSER structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is set successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlTrustedUser_SetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry);

/**
* @brief Get the number of Managed Services Service entries.
*
* This function retrieves the count from Utopia storage.
*
* @return The status of the operation or number of Managed Services Service entries.
* @retval The number of Managed Services Service entries currently configured.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*/
ULONG
CosaDmlMSServ_GetNumberOfEntries(void);

/**
* @brief Get a Managed Services Service entry by index.
*
* This function retrieves the Managed Services Service entry from Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_MS_SERV structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSServ_GetEntryByIndex(ULONG index, COSA_DML_MS_SERV *pEntry);

/**
* @brief Set the instance number and alias for a Managed Services Service entry at a specific index.
*
* This function updates instance number and alias for a Managed Services Service entry in Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to update.
* @param[in] ins  - The instance number to assign to the entry.
* @param[in] alias  - Pointer to a null-terminated string containing the alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the values are set successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSServ_SetValues(ULONG index, ULONG ins, const char *alias);

/**
* @brief Add a new Managed Services Service entry.
*
* This function stores the new entry in Utopia storage.
*
* @param[in] pEntry  - Pointer to a COSA_DML_MS_SERV structure containing the entry to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is added successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSServ_AddEntry(COSA_DML_MS_SERV *pEntry);

/**
* @brief Delete a Managed Services Service entry by instance number.
*
* This function removes the entry from Utopia storage.
*
* @param[in] ins  - The instance number of the entry to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is deleted successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSServ_DelEntry(ULONG ins);

/**
* @brief Get a Managed Services Service entry by instance number.
*
* This function locates the entry by instance number.
*
* @param[in] ins  - The instance number of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_MS_SERV structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSServ_GetConf(ULONG ins, COSA_DML_MS_SERV *pEntry);

/**
* @brief Set the configuration for a Managed Services Service entry by instance number.
*
* This function updates configuration in Utopia storage.
*
* @param[in] ins  - The instance number of the entry to update.
* @param[in] pEntry  - Pointer to a COSA_DML_MS_SERV structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is set successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSServ_SetConf(ULONG ins, COSA_DML_MS_SERV *pEntry);

/**
* @brief Get the number of Managed Services Trusted User entries.
*
* This function retrieves the count from Utopia storage using Utopia_GetNumberOfMSTrustedUser().
*
* @return The status of the operation or number of Managed Services Trusted User.
* @retval The number of Managed Services Trusted User entries currently configured.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ULONG
CosaDmlMSTrustedUser_GetNumberOfEntries(void);

/**
* @brief Get a Managed Services Trusted User entry by index.
*
* This function retrieves the entry from Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_MS_TRUSTEDUSER structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_MS_TRUSTEDUSER *pEntry);

/**
* @brief Set the instance number and alias for a Managed Services Trusted User entry at a specific index.
*
* This function updates the instance number and alias for a Managed Services Trusted User entry in Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to update.
* @param[in] ins  - The instance number to assign to the entry.
* @param[in] alias  - Pointer to a null-terminated string containing the alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the values are set successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias);

/**
* @brief Add a new Managed Services Trusted User entry.
*
* This function stores the new entry in Utopia storage.
*
* @param[in] pEntry  - Pointer to a COSA_DML_MS_TRUSTEDUSER structure containing the entry to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is added successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSTrustedUser_AddEntry(COSA_DML_MS_TRUSTEDUSER *pEntry);

/**
* @brief Delete a Managed Services Trusted User entry by instance number.
*
* This function removes the entry from Utopia storage.
*
* @param[in] ins  - The instance number of the entry to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is deleted successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSTrustedUser_DelEntry(ULONG ins);

/**
* @brief Get a Managed Services Trusted User entry by instance number.
*
* This function locates or retrieve the entry by instance number.
*
* @param[in] ins  - The instance number of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_MS_TRUSTEDUSER structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSTrustedUser_GetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry);

/**
* @brief Set the configuration for a Managed Services Trusted User entry by instance number.
*
* This function updates the configuration for a Managed Services Trusted User entry in Utopia storage.
*
* @param[in] ins  - The instance number of the entry to update.
* @param[in] pEntry  - Pointer to a COSA_DML_MS_TRUSTEDUSER structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is set successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMSTrustedUser_SetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry);

/**
* @brief Get the number of Managed Devices Device entries.
*
* This function retrieves the count from Utopia storage.
*
* @return The status of the operation or number of Managed Devices Device entries.
* @retval The number of Managed Devices Device entries currently configured.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*/
ULONG
CosaDmlMDDev_GetNumberOfEntries(void);

/**
* @brief Get a Managed Devices Device entry by index.
*
* This function retrieves the entry from Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_MD_DEV structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMDDev_GetEntryByIndex(ULONG index, COSA_DML_MD_DEV *pEntry);

/**
* @brief Set the instance number and alias for a Managed Devices Device entry at a specific index.
*
* This function updates the instance number and alias in Utopia storage.
*
* @param[in] index  - The zero-based index of the entry to update.
* @param[in] ins  - The instance number to assign to the entry.
* @param[in] alias  - Pointer to a null-terminated string containing the alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the values are set successfully.
* @retval ANSC_STATUS_FAILURE if the index is out of range or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMDDev_SetValues(ULONG index, ULONG ins, const char *alias);

/**
* @brief Add a new Managed Devices Device entry.
*
* This function stores the new entry in Utopia storage.
*
* @param[in] pEntry  - Pointer to a COSA_DML_MD_DEV structure containing the entry to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is added successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMDDev_AddEntry(COSA_DML_MD_DEV *pEntry);

/**
* @brief Delete a Managed Devices Device entry by instance number.
*
* This function removes the entry from Utopia storage.
*
* @param[in] ins  - The instance number of the entry to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is deleted successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMDDev_DelEntry(ULONG ins);

/**
* @brief Get a Managed Devices Device entry by instance number.
*
* This function locates or retrieve the entry by instance number.
*
* @param[in] ins  - The instance number of the entry to retrieve.
* @param[out] pEntry  - Pointer to a COSA_DML_MD_DEV structure where the entry will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the entry is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMDDev_GetConf(ULONG ins, COSA_DML_MD_DEV *pEntry);

/**
* @brief Set the configuration for a Managed Devices Device entry by instance number.
*
* This function updates the configuration for a Managed Devices Device entry in Utopia storage.
*
* @param[in] ins  - The instance number of the entry to update.
* @param[in] pEntry  - Pointer to a COSA_DML_MD_DEV structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is set successfully.
* @retval ANSC_STATUS_FAILURE if the instance is not found or the operation fails.
*
*/
ANSC_STATUS
CosaDmlMDDev_SetConf(ULONG ins, COSA_DML_MD_DEV *pEntry);

/**
* @brief Get the Managed Devices Redirect Server configuration.
*
* This function retrieves redirect server IPs from syscfg storage.
* If not configured, it sets default values: 127.0.0.1 for IPv4 and ::1 for IPv6.
*
* @param[out] pEntry  - Pointer to a COSA_DML_MD_RED structure where the configuration will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMDRed_GetConf(COSA_DML_MD_RED *pEntry);

/**
* @brief Set the Managed Devices Redirect Server configuration.
*
* This function stores the redirect server IPs in syscfg storage and commits the changes.
*
* @param[in] pEntry  - Pointer to a COSA_DML_MD_RED structure containing the configuration to be set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is set successfully.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlMDRed_SetConf(COSA_DML_MD_RED *pEntry);

/**
* @brief Validate an IP address and wrap it in single quotes for safe usage.
*
* This function validates the IP address and checks for malicious single quote characters.
*
* @param[in] type  - The IP address type: 4 for IPv4, 6 for IPv6.
* @param[in] ipAddress  - Pointer to a null-terminated string containing the IP address to validate.
* @param[out] wrapped_inputparam  - Pointer to a buffer where the wrapped IP address will be stored.
* @param[in] sizeof_wrapped_inputparam  - Size of the wrapped_inputparam buffer in bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the IP address is valid and wrapped successfully.
* @retval ANSC_STATUS_FAILURE if the IP address is invalid or contains single quotes.
*
*/
ANSC_STATUS isValidIP( int type, char *ipAddress, char *wrapped_inputparam, int sizeof_wrapped_inputparam);

/**
* @brief Convert local time to UTC time.
*
* This function is used to convert time schedules from local timezone to UTC.
*
* @param[in] LocalTime  - Pointer to a null-terminated string containing the local time in HH:MM format.
* @param[out] UtcTime  - Pointer to a buffer where the converted UTC time will be stored in HH:MM format.
*
* @return An integer status code indicating the result of the conversion.
*
*/
int ConvLocalToUTC(char* LocalTime, char* UtcTime);

/**
* @brief Convert UTC time to local time.
*
* This function is used to convert time schedules from UTC to local timezone.
*
* @param[in] UtcTime  - Pointer to a null-terminated string containing the UTC time in HH:MM format.
* @param[out] LocalTime  - Pointer to a buffer where the converted local time will be stored in HH:MM format.
*
* @return An integer status code indicating the result of the conversion.
*
*/
int ConvUTCToLocal( char* UtcTime, char* LocalTime);

/**
* @brief Split block days when time conversion crosses day boundaries.
*
* This function is used when time conversion causes schedule to span multiple days.
*
* @param[in] sRet  - Start time conversion result indicating if day boundary was crossed.
* @param[in] eRet  - End time conversion result indicating if day boundary was crossed.
* @param[in,out] sBDays  - Pointer to a string containing the start block days.
* @param[out] eBDays  - Pointer to a buffer where the adjusted block days for the split rule will be stored.
*
* @return An integer indicating whether splitting occurred.
* @retval non-zero if split required.
* @retval 0 otherwise.
*
*/
int split_BlockDays(int sRet, int eRet, char *sBDays, char *eBDays);

#ifdef UTC_ENABLE

/**
* @brief Migrate Blocked URL entries from local time to UTC time.
*
* This function converts all Blocked URL time schedules from local time to UTC.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the migration is completed successfully.
* @retval ANSC_STATUS_FAILURE if the migration fails.
*
*/
ANSC_STATUS CosaDmlBlkURL_Migration();

/**
* @brief Migrate Managed Services Service entries from local time to UTC time.
*
* This function converts all Managed Services Service time schedules from local time to UTC.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the migration is completed successfully.
* @retval ANSC_STATUS_FAILURE if the migration fails.
*
*/
ANSC_STATUS CosaDmlMSServ_Migration();

/**
* @brief Migrate Managed Devices Device entries from local time to UTC time.
*
* This function converts all Managed Devices Device time schedules from local time to UTC.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the migration is completed successfully.
* @retval ANSC_STATUS_FAILURE if the migration fails.
*
*/
ANSC_STATUS CosaDmlMDDev_Migration();

/**
* @brief Rollback Blocked URL entries from UTC time to local time.
*
* This function converts all Blocked URL time schedules from UTC back to local time.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the rollback is completed successfully.
* @retval ANSC_STATUS_FAILURE if the rollback fails.
*
*/
ANSC_STATUS CosaDmlBlkURL_RollbackUTCtoLocal();

/**
* @brief Rollback Managed Services Service entries from UTC time to local time.
*
* This function converts all Managed Services Service time schedules from UTC back to local time.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the rollback is completed successfully.
* @retval ANSC_STATUS_FAILURE if the rollback fails.
*
*/
ANSC_STATUS CosaDmlMSServ_RollbackUTCtoLocal();

/**
* @brief Rollback Managed Devices Device entries from UTC time to local time.
*
* This function converts all Managed Devices Device time schedules from UTC back to local time.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the rollback is completed successfully.
* @retval ANSC_STATUS_FAILURE if the rollback fails.
*
*/
ANSC_STATUS CosaDmlMDDev_RollbackUTCtoLocal();
#endif
#endif /* _COSA_X_COMCAST_COM_PARENTALCONTROL_H_ */
