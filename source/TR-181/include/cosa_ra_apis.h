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

    module: cosa_ra_apis.h

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


#ifndef  _COSA_RA_APIS_H
#define  _COSA_RA_APIS_H

#include "cosa_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "plugin_main_apis.h"

typedef  enum
_COSA_DML_RA_IF_STATUS
{
    COSA_DML_RA_IF_STATUS_Disabled = 1,
    COSA_DML_RA_IF_STATUS_Enabled,
    COSA_DML_RA_IF_STATUS_ErrorMisconfigured,
    COSA_DML_RA_IF_STATUS_Error
}
COSA_DML_RA_IF_STATUS, *PCOSA_DML_RA_IF_STATUS;

typedef  enum
_COSA_DML_RA_PREFER_ROUTER_TYPE
{
    COSA_DML_RA_PREFER_ROUTER_High = 1,
    COSA_DML_RA_PREFER_ROUTER_Medium,
    COSA_DML_RA_PREFER_ROUTER_Low
}
COSA_DML_RA_PREFER_ROUTER_TYPE, *PCOSA_DML_RA_PREFER_ROUTER_TYPE;


/*
 *  Structure definitions for RA InterfaceSetting
 */
typedef  struct
_COSA_DML_RA_IF_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    char                            Interface[COSA_DML_IF_NAME_LENGTH];
    char                            ManualPrefixes[COSA_DML_IF_NAME_LENGTH];

    ULONG                           MaxRtrAdvInterval;
    ULONG                           MinRtrAdvInterval;
    ULONG                           AdvDefaultLifetime;
    BOOLEAN                         bAdvManagedFlag;
    BOOLEAN                         bAdvOtherConfigFlag;
    BOOLEAN                         bAdvMobileAgentFlag;
    COSA_DML_RA_PREFER_ROUTER_TYPE  AdvPreferredRouterFlag;
    BOOLEAN                         bAdvNDProxyFlag;
    ULONG                           AdvLinkMTU;
    ULONG                           AdvReachableTime;
    ULONG                           AdvRetransTimer;
    ULONG                           AdvCurHopLimit;
}
COSA_DML_RA_IF_CFG,  *PCOSA_DML_RA_IF_CFG;


typedef  struct
_COSA_DML_RA_IF_INFO
{
    COSA_DML_RA_IF_STATUS           Status;
    char                            Prefixes[COSA_DML_IF_NAME_LENGTH];
}
COSA_DML_RA_IF_INFO,  *PCOSA_DML_RA_IF_INFO;


typedef  struct
_COSA_DML_RA_IF_FULL
{
    COSA_DML_RA_IF_CFG             Cfg;
    COSA_DML_RA_IF_INFO            Info;
}
COSA_DML_RA_IF_FULL, *PCOSA_DML_RA_IF_FULL;

typedef  struct
_COSA_DML_RA_IF_FULL2
{
    COSA_DML_RA_IF_CFG            Cfg;
    COSA_DML_RA_IF_INFO           Info;

    SLIST_HEADER                  OptionList;
    ULONG                         ulNextOptionInsNum;
}
COSA_DML_RA_IF_FULL2, *PCOSA_DML_RA_IF_FULL2;

#define RA_OPTION_VALUE_SIZE   1024
typedef struct
_COSA_DML_RA_OPTION
{
    SINGLE_LINK_ENTRY               Link;
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           Tag;
    char                            Value[RA_OPTION_VALUE_SIZE];
}
COSA_DML_RA_OPTION, * PCOSA_DML_RA_OPTION;

#define  ACCESS_RA_OPTION_LINK_OBJECT(p)              \
         ACCESS_CONTAINER(p, COSA_DML_RA_OPTION, Link)

/**
 * @brief Initialize the Router Advertisement (RA) subsystem.
 *
 * This function initializes the IPv6 Router Advertisement subsystem during component
 * startup.
 *
 * @param[in] hDml - Handle to the Data Model Library context.
 * @param[in,out] phContext - Pointer to handle for storing the initialized context.
 *                            \n The context handle will be populated by this function.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if initialization completed successfully.
 */
ANSC_STATUS
CosaDmlRAInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
 * @brief Retrieve the global enable status of Router Advertisement.
 *
 * This function retrieves the current enable/disable state of the IPv6 Router
 * Advertisement Protocol.
 *
 * @param[out] pEnabled - Pointer to a boolean variable where the enable state will be stored.
 *                        \n TRUE if Router Advertisement is enabled, FALSE if disabled.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the enable state was successfully retrieved.
 * @retval ANSC_STATUS_FAILURE otherwise.
 *
 */
ULONG
CosaDmlRAGetEnabled
    (
        BOOLEAN * pEnabled
    );

/**
 * @brief Set the global enable status of Router Advertisement.
 *
 * This function sets the enable/disable state of the IPv6 Router Advertisement
 * Protocol.
 *
 * @param[in] bEnabled - Boolean value to set the Router Advertisement enable state.
 *                       \n TRUE to enable Router Advertisement, FALSE to disable.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if set successfully
 * @retval ANSC_STATUS_FAILURE otherwise.
 *
 */
ULONG
CosaDmlRASetEnabled
    (
        BOOLEAN  bEnabled
    );

/*
 *  Ra InterfaceSetting
 */

/**
 * @brief Retrieve the number of Router Advertisement interface entries.
 *
 * This function retrieves the total number of Router Advertisement interface entries
 * currently configured in the system.
 *
 * @param[in] hContext - Handle to the RA interface context.
 *
 * @return The number of Router Advertisement interface entries in the system.
 *
 */
ULONG
CosaDmlRaIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
 * @brief Retrieve the complete information of a Router Advertisement interface entry by index.
 *
 * This function retrieves the full configuration and status information for the Router
 * Advertisement interface designated by the specified index.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulIndex - Zero-based index of the interface entry to retrieve.
 *                      \n Must be 0 as only one interface is supported.
 * @param[out] pEntry - Pointer to COSA_DML_RA_IF_FULL structure where the interface
 *                      information will be stored.
 *                      \n Contains both Cfg (configuration) and Info (status) members.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the interface entry was successfully retrieved.
 * @retval ANSC_STATUS_FAILURE otherwise

 */
ANSC_STATUS
CosaDmlRaIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_RA_IF_FULL       pEntry
    );

/**
 * @brief Set the backend values for a Router Advertisement interface entry.
 *
 * This function sets the backend values (instance number and alias) for the RA interface
 * entry designated by the specified index.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulIndex - Zero-based index of the interface entry to update.
 *                      \n Must be 0 as only one interface is supported.
 * @param[in] ulInstanceNumber - Instance number to assign to the interface entry.
 *                               \n Must be a positive integer.
 * @param[in] pAlias - Alias string to assign to the interface entry.
 *                     \n Must be a null-terminated string (max length defined by DM).
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the backend values were successfully set.
 *
 */
ANSC_STATUS
CosaDmlRaIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
 * @brief Add a new Router Advertisement interface entry.
 *
 * This function attempts to add a new Router Advertisement interface to the system.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] pEntry - Pointer to COSA_DML_RA_IF_FULL structure containing the interface
 *                     configuration to add.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_FULL        pEntry
    );

/**
 * @brief Delete a Router Advertisement interface entry by instance number.
 *
 * This function attempts to delete a Router Advertisement interface from the system.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulInstanceNumber - Instance number of the interface entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
 * @brief Set the configuration for a Router Advertisement interface.
 *
 * This function applies the specified configuration to the RA interface identified by
 * the instance number.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] pCfg - Pointer to COSA_DML_RA_IF_CFG structure containing the interface
 *                   configuration to apply.
 *                   \n Must have valid InstanceNumber to identify the target interface.
 *                   \n Writable fields: bManagedFlag, bOtherFlag, AdvLinkMTU.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the configuration was successfully applied.
 * @retval ANSC_STATUS_FAILURE if Utopia operations fail or Zebra restart trigger fails.
 *
 */
ANSC_STATUS
CosaDmlRaIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG         pCfg        /* Identified by InstanceNumber */
    );

/**
 * @brief Retrieve the configuration for a Router Advertisement interface.
 *
 * This function retrieves the current configuration of the RA interface identified by
 * the instance number.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in,out] pCfg - Pointer to COSA_DML_RA_IF_CFG structure.
 *                       \n [in] InstanceNumber must be set to identify the target interface.
 *                       \n [out] Structure populated with current RA configuration.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the configuration was successfully retrieved.
 * @retval ANSC_STATUS_FAILURE if loading the RA interface configuration fails.
 *
 */
ANSC_STATUS
CosaDmlRaIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG         pCfg        /* Identified by InstanceNumber */
    );

/**
 * @brief Retrieve runtime status information for a Router Advertisement interface.
 *
 * This function retrieves the current runtime status information of the RA interface
 * identified by the specified instance number.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulInstanceNumber - Instance number of the interface to query.
 *                               \n Must be a valid instance number.
 * @param[out] pInfo - Pointer to COSA_DML_RA_IF_INFO structure where the runtime
 *                     information will be stored.
 *                     \n Contains status fields like bEnabled and LastChange timestamp.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the runtime information was successfully retrieved.
 * @retval ANSC_STATUS_FAILURE if loading the RA interface information fails.
 *
 */
ANSC_STATUS
CosaDmlRaIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_RA_IF_INFO       pInfo
    );

/**
 * @brief Reset a Router Advertisement interface to its default configuration.
 *
 * This function attempts to reset the RA interface identified by the specified instance
 * number to its factory default settings.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulInstanceNumber - Instance number of the interface to reset.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
 * @brief Add a new Router Advertisement option to an RA interface.
 *
 * This function attempts to add a new RA option to the specified RA interface.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulRaIfInstanceNumber - Instance number of the RA interface to which the
 *                                   option should be added.
 * @param[in] pEntry - Pointer to COSA_DML_RA_OPTION structure containing the option
 *                     configuration to add.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    );

/**
 * @brief Delete a Router Advertisement option from an RA interface.
 *
 * This function attempts to delete a specific RA option from the specified RA interface.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulRaIfInstanceNumber - Instance number of the RA interface from which the
 *                                   option should be deleted.
 * @param[in] pEntry - Pointer to COSA_DML_RA_OPTION structure identifying the option
 *                     to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    );

/**
 * @brief Modify the configuration of a Router Advertisement option.
 *
 * This function attempts to update the configuration of an existing RA option on the
 * specified RA interface.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulRaIfInstanceNumber - Instance number of the RA interface containing the
 *                                   option to modify.
 * @param[in] pEntry - Pointer to COSA_DML_RA_OPTION structure containing the updated
 *                     option configuration.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    );

/**
 * @brief Retrieve configuration of a Router Advertisement option by instance number.
 *
 * This function retrieves the complete configuration of a specific RA option on the
 * specified RA interface.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulRaIfInstanceNumber - Instance number of the RA interface containing the option.
 * @param[in,out] pEntry - Pointer to COSA_DML_RA_OPTION structure.
 *                         \n [in] InstanceNumber field identifies the target option.
 *                         \n [out] Structure populated with option configuration.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfGetOption2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    );

/**
 * @brief Set the backend values for a Router Advertisement option entry.
 *
 * This function sets the backend values (instance number and alias) for an RA option
 * entry designated by the specified index on the given RA interface.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulRaIfInstanceNumber - Instance number of the RA interface containing the option.
 * @param[in] ulIndex - Zero-based index of the option entry to update.
 * @param[in] ulInstanceNumber - Instance number to assign to the option entry.
 * @param[in] pAlias - Alias string to assign to the option entry.
 *                     \n Must be a null-terminated string.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
 * @brief Retrieve a Router Advertisement option entry by index.
 *
 * This function retrieves the configuration of an RA option entry on the specified
 * RA interface by its zero-based index.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulRaIfInstanceNumber - Instance number of the RA interface containing the option.
 * @param[in] ulIndex - Zero-based index of the option entry to retrieve.
 * @param[out] pEntry - Pointer to COSA_DML_RA_OPTION structure where the option
 *                      information will be stored.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_RA_OPTION         pEntry
    );

/**
 * @brief Retrieve the count of Router Advertisement options for an RA interface.
 *
 * This function returns the total number of RA options configured on the specified
 * RA interface.
 *
 * @param[in] hContext - Handle to the RA interface context.
 * @param[in] ulRaIfInstanceNumber - Instance number of the RA interface to query.
 *
 * @return The number of RA options.
 * @retval 0 - No options are supported/configured.
 *
 */
ULONG
CosaDmlRaIfGetNumberOfOptions
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber
    );


/**
 * @brief Remove backend resources for Router Advertisement interfaces.
 *
 * This function performs cleanup of backend resources associated with RA interfaces.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_FAILURE - Operation not supported.
 *
 */
ANSC_STATUS
CosaDmlRaIfBERemove
    (
        void
    );


#endif