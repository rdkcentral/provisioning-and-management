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

    module: cosa_x_cisco_com_ddns_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

/*
 * This file defines data types and methods as the middle layer between COSA DM library
 * and backend implementation.
 */
#if !defined(DDNS_BROADBANDFORUM)
#ifndef  _COSA_X_CISCO_COM_DDNS_APIS_H
#define  _COSA_X_CISCO_COM_DDNS_APIS_H

#include "ansc_platform.h"
#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

#define COSA_DML_SERVICE_NAME_LENGTH    512
#define DDNS_NAMESPACE "CosaDDNS"

typedef  enum
_COSA_DML_DDNS_STATE
{
    COSA_DML_DDNS_STATE_Idle            = 1,
    COSA_DML_DDNS_STATE_InProgress,
    COSA_DML_DDNS_STATE_Succeeded,
    COSA_DML_DDNS_STATE_ErrorAuthentication,
    COSA_DML_DDNS_STATE_InvalidDomainName,
    COSA_DML_DDNS_STATE_Abuse,
    COSA_DML_DDNS_STATE_NoChange
}
COSA_DML_DDNS_STATE, *PCOSA_DML_DDNS_STATE;

typedef  struct
_COSA_DML_DDNS_CFG
{
    BOOLEAN                         bEnabled;
}
COSA_DML_DDNS_CFG,  *PCOSA_DML_DDNS_CFG;

/*
 *  DDNS Service
 */
typedef  struct
_COSA_DML_DDNS_SERVICE
{
    BOOLEAN                         bEnabled;
    ULONG                           InstanceNumber;
    char                            Alias[65];
    char                            ServiceName[32];
    char                            Username[65];
    char                            Password[65];
    char                            Domain[65];
    char                            AssociatedConnection[257];   /* IP interface name */
    COSA_DML_DDNS_STATE             ConnectionState;
    char                            Mail_exch[129];
    BOOLEAN                         Backup_mx;
    BOOLEAN                         Wildcard;
}
COSA_DML_DDNS_SERVICE,  *PCOSA_DML_DDNS_SERVICE;

/*
 *  The actual function declaration
 */

/**
* @brief Initialize the DDNS module.
*
* This function initializes the DDNS backend by loading DDNS service configurations from system storage.
*
* @param[in] hDml  - Opaque handle from DM adapter.
* @param[out] phContext  - Opaque handle passed back from backend.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Set the global DDNS configuration.
*
* This function sets the global DDNS enabled/disabled state and persists it to system storage.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pCfg  - Pointer to the DDNS configuration structure.
*                    \n The bEnabled field controls the global DDNS enable/disable state.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsSetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_CFG          pCfg
    );

/**
* @brief Get the global DDNS configuration.
*
* This function retrieves the global DDNS enabled/disabled state from the global
* variable and returns it in the configuration structure.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pCfg  - Pointer to the DDNS configuration structure.
*                    \n The bEnabled field is populated with the current DDNS enable/disable state.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_CFG          pCfg
    );

/**
* @brief Get the number of DDNS services.
*
* This function returns the total number of DDNS services currently configured in the
* system.
*
* @param[in] hContext  - Handle to the context.
*
* @return The number of DDNS services.
*
*/
ULONG
CosaDmlDdnsGetNumberOfServices
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get a DDNS service entry by index.
*
* This function retrieves a DDNS service entry from the global g_DdnsService array at the specified index.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulIndex  - Index of the service entry to retrieve.
* @param[out] pService  - Pointer to the service structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsGetService
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DDNS_SERVICE      pService
    );

/**
* @brief Set instance number and alias for a DDNS service entry.
*
* This function updates the instance number and alias for a DDNS service entry
* at the specified index in the global array.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulIndex  - Index of the service entry to update.
* @param[in] ulInstanceNumber  - Instance number to set for the service entry.
* @param[in] pAlias  - Pointer to the alias string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsServiceSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Get a DDNS service entry by instance number.
*
* This function retrieves a DDNS service entry from the global array
* by matching the specified instance number.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulInstanceNumber  - Instance number of the service entry to retrieve.
* @param[out] pService  - Pointer to the service structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsGetServiceByInstNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DDNS_SERVICE      pService
    );

/**
* @brief Add a new DDNS service entry.
*
* This function adds a new DDNS service entry to the system.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pService  - Pointer to the service structure containing the configuration to add.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsAddService
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_SERVICE      pService
    );

/**
* @brief Delete a DDNS service entry.
*
* This function deletes a DDNS service entry from the system by instance number.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulInstanceNumber  - Instance number of the service entry to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsDelService
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set/update a DDNS service entry configuration.
*
* This function updates the configuration for a DDNS service entry.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pService  - Pointer to the service structure containing the configuration to set.
*                    \n The ServiceName field identifies which service to update.
*                    \n The bEnabled, Username, Password, and Domain fields are updated and persisted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsSetService
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_SERVICE      pService
    );

/**
* @brief Get DDNS service information and status.
*
* This function retrieves detailed information for a specific DDNS service entry by
* instance number.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulInstanceNumber  - Instance number of the service entry to retrieve.
* @param[out] pInfo  - Pointer to the service structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDdnsGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DDNS_SERVICE      pInfo
);

#endif
#endif