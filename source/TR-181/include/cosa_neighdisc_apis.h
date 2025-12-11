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

    module: cosa_neighdisc_apis.h

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


#ifndef  _COSA_NEIGHDISC_APIS_H
#define  _COSA_NEIGHDISC_APIS_H

#include "cosa_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "plugin_main_apis.h"
#include "dml_tr181_custom_cfg.h"

typedef  enum
_COSA_DML_NEIGHDISC_IF_STATUS
{
    COSA_DML_NEIGHDISC_IF_STATUS_Disabled = 1,
    COSA_DML_NEIGHDISC_IF_STATUS_Enabled,
    COSA_DML_NEIGHDISC_IF_STATUS_ErrorMisconfigured,
    COSA_DML_NEIGHDISC_IF_STATUS_Error
}
COSA_DML_NEIGHDISC_IF_STATUS, *PCOSA_DML_NEIGHDISC_IF_STATUS;

#define COSA_DML_NEIGHDISC_IFNAME                   CFG_TR181_NeighborDiscovery_IfName

/*
 *  Structure definitions for NEIGHDISC InterfaceSetting
 */
typedef  struct
_COSA_DML_NEIGHDISC_IF_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    char                            Interface[COSA_DML_IF_NAME_LENGTH];
    ULONG                           RetransTimer;
    ULONG                           RtrSolicitationInterval;
    ULONG                           MaxRtrSolicitations;
    BOOLEAN                         bNUDEnable;
    BOOLEAN                         bRSEnable;
}
COSA_DML_NEIGHDISC_IF_CFG,  *PCOSA_DML_NEIGHDISC_IF_CFG;


typedef enum
_NEIGHBOR_STATUS{
    NEIGHBOR_STATUS_INCOMPLETE = 1,
    NEIGHBOR_STATUS_REACHABLE,
    NEIGHBOR_STATUS_STALE,
    NEIGHBOR_STATUS_DELAY,
    NEIGHBOR_STATUS_PROBE
}
NEIGHBOR_STATUS;

typedef  struct
_COSA_DML_NEIGHTABLE_INFO
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    char                            Address[64];
    char                            MACAddress[32];
    char                            Interface[16];
    NEIGHBOR_STATUS                 Status;
}
COSA_DML_NEIGHTABLE_INFO, *PCOSA_DML_NEIGHTABLE_INFO;

typedef  struct
_COSA_DML_NEIGHDISC_IF_INFO
{
    COSA_DML_NEIGHDISC_IF_STATUS           Status;
}
COSA_DML_NEIGHDISC_IF_INFO,  *PCOSA_DML_NEIGHDISC_IF_INFO;


typedef  struct
_COSA_DML_NEIGHDISC_IF_FULL
{
    COSA_DML_NEIGHDISC_IF_CFG             Cfg;
    COSA_DML_NEIGHDISC_IF_INFO            Info;
}
COSA_DML_NEIGHDISC_IF_FULL, *PCOSA_DML_NEIGHDISC_IF_FULL;

/**
 * @brief Initialize the Neighbor Discovery interface subsystem.
 *
 * This function initializes the Neighbor Discovery interface entry in the system.
 *
 * @param[in] hDml - Handle to the Data Model Library context.
 * @param[in,out] phContext - Pointer to handle for storing the initialized context.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if initialization completed successfully.
 */
ANSC_STATUS
CosaDmlNeighdiscInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
 * @brief Retrieve the global enable status of Neighbor Discovery.
 *
 * This function retrieves the current enable/disable state of the IPv6 Neighbor
 * Discovery Protocol from the system configuration.
 *
 * @param[out] pEnabled - Pointer to a boolean variable where the enable state will be stored.
 *                        \n TRUE if Neighbor Discovery is enabled, FALSE if disabled.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the enable state was successfully retrieved.
 * @retval ANSC_STATUS_FAILURE if syscfg_get fails to retrieve the configuration.
 */
ULONG
CosaDmlNeighdiscGetEnabled
    (
        BOOLEAN * pEnabled
    );

/**
 * @brief Set the global enable status of Neighbor Discovery.
 *
 * This function sets the enable/disable state of the IPv6 Neighbor Discovery Protocol.
 *
 * @param[in] bEnabled - Boolean value to set the Neighbor Discovery enable state.
 *                       \n TRUE to enable Neighbor Discovery, FALSE to disable.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the enable state was successfully set and persisted.
 * @retval ANSC_STATUS_FAILURE if syscfg_set_commit fails.
 */
ULONG
CosaDmlNeighdiscSetEnabled
    (
        BOOLEAN  bEnabled
    );

/*
 *  Neighdisc InterfaceSetting
 */

/**
 * @brief Retrieve the number of Neighbor Discovery interface entries.
 *
 * This function retrieves the total number of Neighbor Discovery interface entries
 * currently configured in the system.
 *
 * @param[in] hContext - Handle to the interface context.
 *
 * @return The number of Neighbor Discovery interface entries in the system.
 */
ULONG
CosaDmlNeighdiscIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
 * @brief Retrieve the complete information of a Neighbor Discovery interface entry by index.
 *
 * This function retrieves the full configuration and status information for a Neighbor
 * Discovery interface designated by the specified index.
 *
 * @param[in] hContext - Handle to the interface context.
 * @param[in] ulIndex - Zero-based index of the interface entry to retrieve.
 * @param[out] pEntry - Pointer to COSA_DML_NEIGHDISC_IF_FULL structure where the interface
 *                      information will be stored.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the interface entry was successfully retrieved.
 */
ANSC_STATUS
CosaDmlNeighdiscIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_NEIGHDISC_IF_FULL       pEntry
    );

/**
 * @brief Set the instance number and alias for a Neighbor Discovery interface entry.
 *
 * This backend implementation function is used to update the instance number and alias
 * for a Neighbor Discovery interface entry.
 *
 * @param[in] hContext - Handle to the interface context.
 * @param[in] ulIndex - Zero-based index of the interface entry to update.
 * @param[in] ulInstanceNumber - The instance number to assign to the interface entry.
 * @param[in] pAlias - Pointer to a null-terminated string containing the alias name.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the values were successfully set.
 * @retval ANSC_STATUS_FAILURE if strcpy_s fails.
 */
ANSC_STATUS
CosaDmlNeighdiscIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
 * @brief Add a new Neighbor Discovery interface entry to the system.
 *
 * This function adds a new Neighbor Discovery interface entry to the system's interface table.
 *
 * @param[in] hContext - Handle to the interface context.
 * @param[in] pEntry - Pointer to COSA_DML_NEIGHDISC_IF_FULL structure containing the
 *                     complete interface configuration and information to be added.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the interface entry was successfully added, allocated by callee
 * @retval ANSC_STATUS_FAILURE if maximum number of interfaces is reached or if no entry is found.
 */
ANSC_STATUS
CosaDmlNeighdiscIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NEIGHDISC_IF_FULL       pEntry
    );

/**
 * @brief Delete a Neighbor Discovery interface entry from the system.
 *
 * This function removes a Neighbor Discovery interface entry from the system's interface
 * table identified by the specified instance number.
 *
 * @param[in] hContext - Handle to the interface context.
 * @param[in] ulInstanceNumber - The instance number of the interface entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the interface entry was successfully deleted.
 * @retval ANSC_STATUS_CANT_FIND if no entry with the specified instance number exists.
 */
ANSC_STATUS
CosaDmlNeighdiscIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
 * @brief Set the configuration for a Neighbor Discovery interface.
 *
 * This function updates the configuration parameters for a Neighbor Discovery interface
 * identified by the instance number in pCfg.
 *
 * @param[in] hContext - Handle to the interface context.
 * @param[in] pCfg - Pointer to COSA_DML_NEIGHDISC_IF_CFG structure containing the new
 *                   configuration parameters. RtrSolicitationInterval is stored in milliseconds but written to kernel in seconds.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the configuration was successfully set.
 * @retval ANSC_STATUS_CANT_FIND if no entry with the specified instance number exists.
 * @retval ANSC_STATUS_FAILURE if Configuration fails.
 */
ANSC_STATUS
CosaDmlNeighdiscIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NEIGHDISC_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

/**
 * @brief Retrieve the configuration for a Neighbor Discovery interface.
 *
 * This function retrieves the current configuration parameters for a Neighbor Discovery
 * interface identified by the instance number in pCfg.
 *
 * @param[in] hContext - Handle to the interface context.
 * @param[in,out] pCfg - Pointer to COSA_DML_NEIGHDISC_IF_CFG structure.
 *                       \n [in] pCfg->InstanceNumber identifies which interface to retrieve.
 *                       \n [out] Structure is populated with the interface configuration.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the configuration was successfully retrieved.
 * @retval ANSC_STATUS_CANT_FIND if no entry with the specified instance number exists.
 */
ANSC_STATUS
CosaDmlNeighdiscIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NEIGHDISC_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

/**
 * @brief Retrieve the runtime information for a Neighbor Discovery interface.
 *
 * This function retrieves the runtime status information for a Neighbor Discovery
 * interface identified by the specified instance number.
 *
 * @param[in] hContext - Handle to the interface context.
 * @param[in] ulInstanceNumber - The instance number of the interface entry.
 *                               \n Must match an existing interface's InstanceNumber.
 * @param[out] pInfo - Pointer to COSA_DML_NEIGHDISC_IF_INFO structure where the runtime
 *                     information will be stored.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the information was successfully retrieved.
 * @retval ANSC_STATUS_CANT_FIND if no entry with the specified instance number exists.
 */
ANSC_STATUS
CosaDmlNeighdiscIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_NEIGHDISC_IF_INFO       pInfo
    );

/**
 * @brief Reset a Neighbor Discovery interface to default settings.
 *
 * This function resets the specified Neighbor Discovery interface to its default configuration settings.
 *
 * @param[in] hContext - Handle to the interface context.
 * @param[in] ulInstanceNumber - The instance number of the interface entry to reset.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the interface was successfully reset.
 */
ANSC_STATUS
CosaDmlNeighdiscIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );



/**
 * @brief Remove backend resources for Neighbor Discovery interfaces.
 *
 * This function performs cleanup and removal of backend resources associated with Neighbor Discovery interfaces.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if backend resources were successfully removed or cleanup completed.
 */
ANSC_STATUS
CosaDmlNeighdiscIfBERemove
    (
        void
    );

/**
 * @brief Retrieve IPv6 neighbor table entries from the system.
 *
 * This function retrieves the IPv6 neighbor table entries by executing the "ip -6 neigh show"
 * command and parsing the results.
 *
 * @param[in] hContext - Handle to the neighbor table context.
 * @param[out] pulInstanceNumber - Pointer to ULONG where the total number of neighbor entries
 *                                 will be stored. Set to 0 if no entries are found or on error.
 * @param[out] ppNbTbl - Pointer to a pointer of COSA_DML_NEIGHTABLE_INFO array.
 *                       \n Set to NULL if no entries are found.
 *
 * @return None.
 */
void
CosaDmlNeighborTableGetEntry
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulInstanceNumber,
        PCOSA_DML_NEIGHTABLE_INFO*  ppNbTbl
    );

#endif
