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

/**************************************************************************

    module: cosa_userinterface_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_USERINTERFACE_API_H
#define  _COSA_USERINTERFACE_API_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

#define HTTP_INTERNAL_PORT            80
#define HTTPS_INTERNAL_PORT           443
#define SSH_INTERNAL_PORT             22

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

typedef  enum
_COSA_DML_RA_PROTOCOL
{
    COSA_DML_RA_PROTOCOL_Http           = 1,
    COSA_DML_RA_PROTOCOL_Https,
    COSA_DML_RA_PROTOCOL_Ssh,
    COSA_DML_RA_PROTOCOL_Telnet,
}
COSA_DML_RA_PROTOCOL, *PCOSA_DML_RA_PROTOCOL;


struct
_COSA_DML_RA_CFG
{
    /*
     *  Extensions -- X_CISCO_COM_RemoteAccess
     */
    BOOLEAN                         bEnabled;
    char                            SupportedProtocols[64];
    BOOLEAN                         bFromAnyIp;
    ANSC_IPV4_ADDRESS               StartIp;
    ANSC_IPV4_ADDRESS               EndIp;
    BOOLEAN                         HttpEnable;
    ULONG                           HttpPort;
    BOOLEAN                         HttpsEnable;
    ULONG                           HttpsPort;
    BOOLEAN                         SSHEnable;
    ULONG                           SSHPort;
    BOOLEAN                         TelnetEnable;
    ULONG                           TelnetPort;
    BOOLEAN                         UpgradePermission;
    char                            StartIpV6[64];
    char                            EndIpV6[64];
}_struct_pack_;

typedef struct _COSA_DML_RA_CFG  COSA_DML_RA_CFG,  *PCOSA_DML_RA_CFG;

typedef struct
_COSA_DML_USERINTERFACE_CFG
{
    BOOLEAN                         bPasswordLockoutEnable;
    BOOLEAN                         bHTTPSecurityHeaderEnable;
    ULONG                           PasswordLockoutAttempts;
    ULONG                           PasswordLockoutTime;
}
COSA_DML_USERINTERFACE_CFG,  *PCOSA_DML_USERINTERFACE_CFG;

typedef  struct
_COSA_DML_UI_IPRANGE_ENTRY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    char                            StartIP[16];
    char                            EndIP[16];
    char                            Desp[64];
}
COSA_DML_UI_IPRANGE_ENTRY,  *PCOSA_DML_UI_IPRANGE_ENTRY;

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the Remote Access module.
*
* This function initializes the Remote Access (RA) module for the device.
*
* @param[in] hDml  - Handle to the DML object.
* @param[in,out] phContext  - Pointer to the context handle.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlRaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Set the Remote Access configuration parameters.
*
* This function sets the Remote Access configuration including enable status, IP range restrictions,
* protocol access settings (HTTP, HTTPS, SSH, Telnet), port numbers, and firmware upgrade permissions.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pCfg  - Pointer to the Remote Access configuration structure containing all settings.
*                    \n This includes enable flag, supported protocols, IP range (IPv4/IPv6),
*                    \n HTTP/HTTPS/SSH/Telnet enable flags and ports, and upgrade permission.
*
* @return The status of the operation.
* @retval ANSC_STATUS_FAILURE if the operation is successful or if Utopia initialization fails or firewall restart fails.
*
*/
ANSC_STATUS
CosaDmlRaSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    );

/**
* @brief Get the Remote Access configuration parameters.
*
* This function retrieves the Remote Access configuration from system configuration.
*
* @param[in] hContext  - Handle to the context. This parameter is currently unused.
* @param[out] pCfg  - Pointer to the Remote Access configuration structure where settings will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails or any configuration retrieval fails.
*
*/
ANSC_STATUS
CosaDmlRaGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    );

/**
* @brief Set the User Interface configuration parameters.
*
* This function sets the User Interface configuration including password lockout settings and
* HTTP security header enable flag.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pCfg  - Pointer to the User Interface configuration structure containing settings.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if operation fails.
*
*/
ANSC_STATUS
CosaDmlUserInterfaceSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USERINTERFACE_CFG            pCfg
    );

/**
* @brief Get the User Interface configuration parameters.
*
* This function retrieves the User Interface configuration from system configuration.
*
* @param[in] hContext  - Handle to the context
* @param[out] pCfg  - Pointer to the User Interface configuration structure where settings will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlUserInterfaceGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USERINTERFACE_CFG            pCfg
    );

/**
* @brief Get the list of supported Remote Access protocols.
*
* This function retrieves the list of supported Remote Access protocols.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pCount  - Pointer to store the count of supported protocols.
* @param[out] pEntries  - Pointer to an array of COSA_DML_RA_PROTOCOL entries where the supported protocols will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if operation fails.
*
*/
ANSC_STATUS
CosaDmlRaGetSupportedProtocols
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pCount,
        PCOSA_DML_RA_PROTOCOL       pEntries
    );

/**
* @brief Get the number of User Interface IP Range entries.
*
* This function retrieves the count of configured IP range entries for user interface access
* management from system configuration.
*
* @param[in] hContext  - Handle to the context.
*
* @return The number of IP range entries.
* @retval The number of User Interface IP Range entries
* @retval 0 if Utopia initialization fails or no entries are configured.
*
*/
ULONG
CosaDmlUIIPRangeGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get a User Interface IP Range entry by index.
*
* This function retrieves a specific IP range entry by its index from system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulIndex  - The index of the IP range entry to retrieve (0-based).
* @param[out] pEntry  - Pointer to the IP range entry structure where the retrieved data will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlUIIPRangeGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );

/**
* @brief Set the instance number and alias for a User Interface IP Range entry.
*
* This function updates the instance number and alias for an IP range entry at a specific index
* in system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulIndex  - The index of the IP range entry to update (0-based).
* @param[in] ulInstanceNumber  - The new instance number to set for the entry.
* @param[in] pAlias  - Pointer to the new alias string to set for the entry.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlUIIPRangeSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new User Interface IP Range entry.
*
* This function adds a new IP range entry to the system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pEntry  - Pointer to the IP range entry structure containing the new entry data.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlUIIPRangeAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );

/**
* @brief Delete a User Interface IP Range entry.
*
* This function deletes an IP range entry from system configuration by matching the instance number.
*
* @param[in] hContext  - Handle to the context. This parameter is currently unused.
* @param[in] pEntry  - Pointer to the IP range entry structure containing the InstanceNumber of the entry to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlUIIPRangeDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );

/**
* @brief Set/Update a User Interface IP Range entry.
*
* This function updates an existing IP range entry in system configuration by matching the
* instance number.
*
* @param[in] hContext  - Handle to the context. This parameter is currently unused.
* @param[in] pEntry  - Pointer to the IP range entry structure containing the updated entry data.
*                    \n The InstanceNumber field is used to locate the entry to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlUIIPRangeSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );

/**
* @brief Get the configuration for a User Interface IP Range entry.
*
* This function is used to retrieve the configuration for a specific IP range entry.
*
* @param[in] hContext  - Handle to the context.
* @param[in,out] pEntry  - Pointer to the IP range entry structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if operation is successful
*
*/
ANSC_STATUS
CosaDmlUIIPRangeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );
#endif