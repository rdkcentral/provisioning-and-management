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


/**********************************************************************

    module:	dml_api_bridging.h

        For Data Model Library Implementation (DML),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structure and function
        prototypes for DML DHCP API.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Li Shi

    ---------------------------------------------------------------

    revision:

        03/15/2011    initial revision.

**********************************************************************/


#ifndef  _DML_API_BRIDGING_
#define  _DML_API_BRIDGING_

#include "ansc_platform.h"
#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define SYSCFG_HOMESECURITY_ETH4_FLAG			"HomeSecurityEthernet4Flag"

typedef  enum
_COSA_DML_BRG_STATUS
{
    COSA_DML_BRG_STATUS_Disabled        = 1,
    COSA_DML_BRG_STATUS_Enabled,
    COSA_DML_BRG_STATUS_Error
}
COSA_DML_BRG_STATUS, *PCOSA_DML_BRG_STATUS;

typedef  enum
_COSA_DML_BRG_STD
{
    COSA_DML_BRG_STD_8021D_2004        = 1,
    COSA_DML_BRG_STD_8021Q_2005
}
COSA_DML_BRG_STD, *PCOSA_DML_BRG_STD;

//$HL 4/26/2013
typedef  enum
_COSA_DML_BPORT_MODE
{
    COSA_DML_BPORT_UNKNOWN  = 0,
    COSA_DML_BPORT_PASSTHRU  = 1,
    COSA_DML_BPORT_TAGGING
}
COSA_DML_BPORT_MODE, *PCOSA_DML_BPORT_MODE;

//$HL 07/15/2013
typedef  enum
_COSA_DML_BRG_LINK_TYPE
{
    COSA_DML_BRG_LINK_TYPE_NONE, //$HL 4/23/2013
    COSA_DML_BRG_LINK_TYPE_Eth = 1,
    COSA_DML_BRG_LINK_TYPE_EthVlan,
    COSA_DML_BRG_LINK_TYPE_Usb,
    COSA_DML_BRG_LINK_TYPE_Hpna,
    COSA_DML_BRG_LINK_TYPE_Moca,
    COSA_DML_BRG_LINK_TYPE_HomePlug,
    COSA_DML_BRG_LINK_TYPE_Upa,
    COSA_DML_BRG_LINK_TYPE_WiFiSsid,
    COSA_DML_BRG_LINK_TYPE_Bridge,
    COSA_DML_BRG_LINK_TYPE_Gre
}
COSA_DML_BRG_LINK_TYPE, *PCOSA_DML_BRG_LINK_TYPE;

typedef  enum
_COSA_DML_BRG_PORT_STATE
{
    COSA_DML_BRG_PORT_STATE_Disabled    = 1,
    COSA_DML_BRG_PORT_STATE_Blocking,
    COSA_DML_BRG_PORT_STATE_Listening,
    COSA_DML_BRG_PORT_STATE_Learning,
    COSA_DML_BRG_PORT_STATE_Forwarding,
    COSA_DML_BRG_PORT_STATE_Broken
}
COSA_DML_BRG_PORT_STATE, *PCOSA_DML_BRG_PORT_STATE;

typedef  enum
_COSA_DML_BRG_PORT_AFT
{
    COSA_DML_BRG_PORT_AFT_AdmitAll      = 1,
    COSA_DML_BRG_PORT_AFT_AdmitOnlyVLANTagged,
    COSA_DML_BRG_PORT_AFT_AdmitOnlyPrioUntagged
}
COSA_DML_BRG_PORT_AFT, *PCOSA_DML_BRG_PORT_AFT;


typedef  struct
_COSA_DML_BRG_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];
#if defined (MULTILAN_FEATURE)
    char                            name[COSA_DML_IF_NAME_LENGTH];
#endif
    BOOLEAN                         bEnabled;
    BOOLEAN                         bAllowDelete;
    COSA_DML_BRG_STD                Std;
}
COSA_DML_BRG_CFG, *PCOSA_DML_BRG_CFG;


typedef  struct
_COSA_DML_BRG_INFO
{
    COSA_DML_BRG_STATUS             Status;
}
COSA_DML_BRG_INFO, *PCOSA_DML_BRG_INFO;


typedef  struct
_COSA_DML_BRG_FULL
{
    COSA_DML_BRG_CFG                Cfg;
    COSA_DML_BRG_INFO               Info;
}
COSA_DML_BRG_FULL, *PCOSA_DML_BRG_FULL;

typedef  struct
_COSA_DML_BRG_PORT_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_BRG_STD                Std;
    COSA_DML_BRG_LINK_TYPE          LinkType;
    char                            LinkName[COSA_DML_IF_NAME_LENGTH];  /* LinkName, including bridge name */
    BOOLEAN                         bManagementPort;
    UCHAR                           DftUserPriority;                    /* not supported for now */
    UCHAR                           PriorityRegeneration[8];            /* not supported for now */
    USHORT                          PVID;                               /* not supported for now, default 0 */
    COSA_DML_BRG_PORT_AFT           AcceptableFrameTypes;
    BOOLEAN                         bIngressFiltering;                  /* not supported for now */
    BOOLEAN                         bPriorityTagging;                   /* not supported for now */
    COSA_DML_BPORT_MODE             mode;                               //$HL 4/16/2013 need DM to support
    BOOLEAN                         bUpstream;
    BOOLEAN                         bAllowDelete;
}
COSA_DML_BRG_PORT_CFG, *PCOSA_DML_BRG_PORT_CFG;


typedef  struct
_COSA_DML_BRG_PORT_INFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];      /* Read-only */
    COSA_DML_IF_STATUS              Status;
    ULONG                           LastChange;
    COSA_DML_BRG_PORT_STATE         PortState;
}
COSA_DML_BRG_PORT_INFO, *PCOSA_DML_BRG_PORT_INFO;


typedef  struct
_COSA_DML_BRG_PORT_FULL
{
    COSA_DML_BRG_PORT_CFG           Cfg;
    COSA_DML_BRG_PORT_INFO          Info;
}
COSA_DML_BRG_PORT_FULL, *PCOSA_DML_BRG_PORT_FULL;

typedef  struct
_COSA_PRI_BRG_FULL
{
    COSA_DML_BRG_CFG                Cfg;
    COSA_DML_BRG_INFO               Info;
    USHORT                          ulNumOfPort;
    COSA_DML_BRG_PORT_FULL          PortList[10];
}
COSA_PRI_BRG_FULL, *PCOSA_PRI_BRG_FULL;

typedef  struct
_COSA_DML_BRG_FULL_ALL
{
    COSA_DML_BRG_CFG                Cfg;
    COSA_DML_BRG_INFO               Info;

    SLIST_HEADER                    PortList;
    ULONG                           ulNextPortInsNum;
    SLIST_HEADER                    VLANList;
    ULONG                           ulNextVLANInsNum;
    SLIST_HEADER                    VLANPortList;
    ULONG                           ulNextVLANPortInsNum;
}
COSA_DML_BRG_FULL_ALL, *PCOSA_DML_BRG_FULL_ALL;


typedef  struct
_COSA_DML_BRG_VLAN_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    int                             VLANID;                             /* 1-4094 */
}
COSA_DML_BRG_VLAN_CFG, *PCOSA_DML_BRG_VLAN_CFG;

typedef  struct
_COSA_DML_BRG_VLAN_INFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];      /* Read-only */
}
COSA_DML_BRG_VLAN_INFO, *PCOSA_DML_BRG_VLAN_INFO;

typedef  struct
_COSA_DML_BRG_VLAN_FULL
{
    COSA_DML_BRG_VLAN_CFG           Cfg;
    COSA_DML_BRG_VLAN_INFO          Info;
    COSA_DML_BRG_VLAN_CFG           Cfg1;
}
COSA_DML_BRG_VLAN_FULL, *PCOSA_DML_BRG_VLAN_FULL;


typedef  struct
_COSA_DML_BRG_VLANPORT_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           VLANInsNum;
    ULONG                           PortInsNum;
    BOOLEAN                         bUntagged;
}
COSA_DML_BRG_VLANPORT_CFG, *PCOSA_DML_BRG_VLANPORT_CFG;

typedef  struct
_COSA_DML_BRG_VLANPORT_FULL
{
    COSA_DML_BRG_VLANPORT_CFG           Cfg;
    COSA_DML_BRG_VLANPORT_CFG           Cfg1;
}
COSA_DML_BRG_VLANPORT_FULL, *PCOSA_DML_BRG_VLANPORT_FULL;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the bridging subsystem.
*
* This function initializes the bridging data structures and loads bridge, port, and VLAN
* configurations from PSM (Persistent Storage Manager).
*
* @param[in] hDml - Handle to the DML (Data Model Library) context. Reserved for future use.
* @param[in,out] phContext - Pointer to handle for returning context information. Reserved for future use.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlBrgInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
);

/**
* @brief Get the total number of bridge entries.
*
* This function retrieves the count of configured bridge instances in the system.
* If the bridging subsystem has not been initialized, it will call CosaDmlBrgInit first.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
*
* @return The number of bridge entries.
*
*/
ULONG
CosaDmlBrgGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );


//$HL 7/8/2013
/**
* @brief Get the VLAN ID of a specific bridge.
*
* This function retrieves the VLAN ID associated with a bridge instance identified
* by its instance number.
*
* @param[in] ulInstanceNumber - The instance number of the bridge.
*                                \n Valid range: 1-4294967295.
*
* @return The VLAN ID of the bridge.
* @retval VLAN ID of a specific bridge.
* @retval ANSC_STATUS_CANT_FIND if the bridge is not found.
*
*/
int CosaDmlBrgGetVLANID
    (
        ULONG                       ulInstanceNumber
    );

/**
* @brief Get the name of a specific bridge.
*
* This function retrieves the name of a bridge instance identified by its instance number.
*
* @param[in] ulInstanceNumber - The instance number of the bridge.
*                                \n Valid range: 1-4294967295.
*
* @return Pointer to the bridge name string.
* @retval '\0' if the bridge is not found.
*
*/
char * CosaDmlBrgGetName
    (
        ULONG                       ulInstanceNumber
    );

/**
* @brief Get a bridge entry by index.
*
* This function retrieves the complete configuration and information for a bridge
* entry at the specified index position in the bridge list.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulIndex - Zero-based index of the bridge entry to retrieve.
*                      \n Valid range: 0 to (number of bridges - 1).
* @param[out] pEntry - Pointer to a COSA_DML_BRG_FULL structure where the bridge
*                      configuration and information will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge entry is not found.
*
*/
ANSC_STATUS
CosaDmlBrgGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_FULL          pEntry
    );

/**
* @brief Set the instance number and alias for a bridge entry.
*
* This function updates the instance number and alias (and optionally name when MULTILAN_FEATURE
* is defined) for a bridge entry at the specified index. The changes are persisted to PSM.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulIndex - Zero-based index of the bridge entry to update.
*                      \n Valid range: 0 to (number of bridges - 1).
* @param[in] ulInstanceNumber - New instance number to assign to the bridge.
*                               \n Valid range: 1-4294967295.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
* @param[in] pName - Pointer to a null-terminated string containing the new bridge name.
*                    \n This parameter is only available when MULTILAN_FEATURE is defined.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge entry is not found.
*
*/
ANSC_STATUS
CosaDmlBrgSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
#if defined (MULTILAN_FEATURE)
        char*                       pAlias,
        char*                       pName
#else
        char*                       pAlias
#endif
    );

/**
* @brief Add a new bridge entry.
*
* This function creates a new bridge instance with the specified configuration.
* It allocates a new VLAN ID, PSM instance number, and bridge device name, then
* persists the configuration to PSM and triggers network synchronization.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in,out] pEntry - Pointer to a COSA_DML_BRG_CFG structure containing the bridge
*                         configuration to be added.
*                         \n The InstanceNumber field will be used to identify the bridge.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if memory allocation fails, VLAN ID allocation fails, PSM instance
*         number allocation fails, or bridge name allocation fails.
*
*/
ANSC_STATUS
CosaDmlBrgAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pEntry
    );

/**
* @brief Delete a bridge entry.
*
* This function removes a bridge instance identified by its instance number from the system.
* It removes all associated ports and VLANs, deletes the configuration from PSM, and
* triggers network synchronization.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulInstanceNumber - Instance number of the bridge to be deleted.
*                               \n Valid range: 1-4294967295.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge is not found.
* @retval ANSC_STATUS_NOT_SUPPORTED if deletion is not allowed.
*
*/
ANSC_STATUS
CosaDmlBrgDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a bridge.
*
* This function updates the configuration parameters of an existing bridge identified
* by its instance number. Changes to the enabled state trigger network member synchronization.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] pCfg - Pointer to a COSA_DML_BRG_CFG structure containing the new configuration.
*                   \n The InstanceNumber field identifies which bridge to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge is not found.
*
*/
ANSC_STATUS
CosaDmlBrgSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration of a bridge.
*
* This function retrieves the current configuration parameters of a bridge
* identified by its instance number.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in,out] pCfg - Pointer to a COSA_DML_BRG_CFG structure.
*                       \n [in] The InstanceNumber field identifies which bridge to query.
*                       \n [out] The structure will be filled with the bridge configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge is not found.
*
*/
ANSC_STATUS
CosaDmlBrgGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get runtime information of a bridge.
*
* This function retrieves the runtime status information of a bridge identified
* by its instance number, including the current operational status.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulInstanceNumber - Instance number of the bridge to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pInfo - Pointer to a COSA_DML_BRG_INFO structure where the runtime
*                     information will be returned.
*                     \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge is not found.
*
*/
ANSC_STATUS
CosaDmlBrgGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_BRG_INFO          pInfo
);

/*
 *  Bridge Ports
 */

/**
* @brief Get the total number of port entries for a bridge.
*
* This function retrieves the count of port instances associated with a specific bridge.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge.
*                                  \n Valid range: 1-4294967295.
*
* @return The number of port entries for the specified bridge.
* @retval 0 if the bridge is not found.
*
*/
ULONG
CosaDmlBrgPortGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber
    );

/**
* @brief Get a bridge port entry by index.
*
* This function retrieves the complete configuration and information for a bridge port
* entry at the specified index position within a bridge.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the port.
*                                  \n Valid range: 1-4294967295.
* @param[in] ulIndex - Zero-based index of the port entry to retrieve.
*                      \n Valid range: 0 to (number of ports - 1).
* @param[out] pEntry - Pointer to a COSA_DML_BRG_PORT_FULL structure where the port
*                      configuration and information will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or port is not found.
*
*/
ANSC_STATUS
CosaDmlBrgPortGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_PORT_FULL     pEntry
    );

/**
* @brief Set the instance number and alias for a bridge port entry.
*
* This function updates the instance number and alias for a bridge port entry at
* the specified index within a bridge. The changes are persisted to PSM.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the port.
*                                  \n Valid range: 1-4294967295.
* @param[in] ulIndex - Zero-based index of the port entry to update.
*                      \n Valid range: 0 to (number of ports - 1).
* @param[in] ulInstanceNumber - New instance number to assign to the port.
*                               \n Valid range: 1-4294967295.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*                     \n Maximum length: COSA_DML_IF_NAME_LENGTH (64 bytes).
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or port is not found.
*
*/
ANSC_STATUS
CosaDmlBrgPortSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new bridge port entry.
*
* This function adds a new port to a specified bridge with the given configuration.
* The operation is delegated to the bridge's control interface for execution.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge to add the port to.
*                                  \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_BRG_PORT_CFG structure containing the port
*                     configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge is not found or port addition fails.
*
*/
ANSC_STATUS
CosaDmlBrgPortAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pEntry
    );

/**
* @brief Delete a bridge port entry.
*
* This function removes a port from a specified bridge identified by the port's
* instance number. The operation is delegated to the bridge's control interface.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the port.
*                                  \n Valid range: 1-4294967295.
* @param[in] ulInstanceNumber - Instance number of the port to be deleted.
*                               \n Valid range: 1-4294967295.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or port is not found.
*
*/
ANSC_STATUS
CosaDmlBrgPortDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a bridge port.
*
* This function updates the configuration parameters of an existing bridge port
* identified by its instance number. Changes to link name, link type, enabled state,
* or mode trigger structure update confirmation. The changes are persisted to PSM.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the port.
*                                  \n Valid range: 1-4294967295.
* @param[in] pCfg - Pointer to a COSA_DML_BRG_PORT_CFG structure containing the new configuration.
*                   \n The InstanceNumber field identifies which port to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or port is not found.
*
*/
ANSC_STATUS
CosaDmlBrgPortSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration of a bridge port.
*
* This function retrieves the current configuration parameters of a bridge port
* identified by its instance number.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the port.
*                                  \n Valid range: 1-4294967295.
* @param[in,out] pCfg - Pointer to a COSA_DML_BRG_PORT_CFG structure.
*                       \n [in] The InstanceNumber field identifies which port to query.
*                       \n [out] The structure will be filled with the port configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or port is not found.
*
*/
ANSC_STATUS
CosaDmlBrgPortGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get runtime information of a bridge port.
*
* This function retrieves the runtime status information of a bridge port identified
* by its instance number, including name, last change time, port state, and status.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the port.
*                                  \n Valid range: 1-4294967295.
* @param[in] ulInstanceNumber - Instance number of the port to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pInfo - Pointer to a COSA_DML_BRG_PORT_INFO structure where the runtime
*                     information will be returned.
*                     \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or port is not found.
*
*/
ANSC_STATUS
CosaDmlBrgPortGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_BRG_PORT_INFO     pInfo
    );

/**
* @brief Get statistics of a bridge port.
*
* This function retrieves traffic statistics for a bridge port identified by its
* instance number. The operation is delegated to the port's control interface.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the port.
*                                  \n Valid range: 1-4294967295.
* @param[in] ulInstanceNumber - Instance number of the port to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pStats - Pointer to a COSA_DML_IF_STATS structure where the statistics
*                      will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or port is not found.
*
*/
ANSC_STATUS
CosaDmlBrgPortGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IF_STATS          pStats
    );

/**
* @brief Add a new VLAN entry to a bridge.
*
* This function adds a new VLAN to a specified bridge with the given configuration.
* The operation is delegated to the bridge's control interface for execution.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge to add the VLAN to.
*                                  \n Valid range: 1-4294967295.
* @param[in] pVLAN - Pointer to a COSA_DML_BRG_VLAN_FULL structure containing the VLAN
*                    configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge is not found or VLAN addition fails.
*
*/
ANSC_STATUS
CosaDmlBrgVlanAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_VLAN_FULL     pVLAN
    );

/* CID: 135308 Unrecoverable parse warning*/
/**
* @brief Delete a VLAN entry from a bridge.
*
* This function removes a VLAN from a specified bridge identified by the VLAN's
* instance number. The operation is delegated to the bridge's control interface.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the VLAN.
*                                  \n Valid range: 1-4294967295.
* @param[in] ulVLANInsNum - Instance number of the VLAN to be deleted.
*                           \n Valid range: 1-4294967295.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or VLAN is not found.
*
*/
ANSC_STATUS
CosaDmlBrgVlanDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulVLANInsNum
    );

/**
* @brief Get the total number of VLAN entries for a bridge.
*
* This function retrieves the count of VLAN instances associated with a specific bridge.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge.
*                                  \n Valid range: 1-4294967295.
*
* @return The total number of VLAN entries for a bridge.
* @retval The number of VLAN entries for the specified bridge.
* @retval 0 if the bridge is not found.
*
*/
ULONG
CosaDmlBrgVlanGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber
    );

/**
* @brief Set the instance number and alias for a VLAN entry.
*
* This function updates the instance number and alias for a VLAN entry at
* the specified index within a bridge. The changes are persisted to PSM.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the VLAN.
*                                  \n Valid range: 1-4294967295.
* @param[in] ulIndex - Zero-based index of the VLAN entry to update.
*                      \n Valid range: 0 to (number of VLANs - 1).
* @param[in] ulInstanceNumber - New instance number to assign to the VLAN.
*                               \n Valid range: 1-4294967295.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*                     \n Maximum length: COSA_DML_IF_NAME_LENGTH (64 bytes).
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or VLAN is not found.
*
*/
ANSC_STATUS
CosaDmlBrgVlanSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Get a VLAN entry by index.
*
* This function retrieves the complete configuration and information for a VLAN
* entry at the specified index position within a bridge.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the VLAN.
*                                  \n Valid range: 1-4294967295.
* @param[in] ulIndex - Zero-based index of the VLAN entry to retrieve.
*                      \n Valid range: 0 to (number of VLANs - 1).
* @param[out] pEntry - Pointer to a COSA_DML_BRG_VLAN_FULL structure where the VLAN
*                      configuration and information will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or VLAN is not found.
*
*/
ANSC_STATUS CosaDmlBrgVlanGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_VLAN_FULL     pEntry
    );

/**
* @brief Set the configuration of a VLAN.
*
* This function updates the configuration parameters of an existing VLAN identified
* by its instance number within a bridge. It updates the bridge's VLAN ID and persists
* the changes to PSM.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the VLAN.
*                                  \n Valid range: 1-4294967295.
* @param[in] pCfg - Pointer to a COSA_DML_BRG_VLAN_CFG structure containing the new configuration.
*                   \n The InstanceNumber field identifies which VLAN to update.
*                   \n The VLANID field specifies the new VLAN ID (range: 1-4094).
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or VLAN is not found.
*
*/
ANSC_STATUS
CosaDmlBrgVlanSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_VLAN_CFG      pCfg
    );

/**
* @brief Add a port to a VLAN.
*
* This function adds a bridge port to a specified VLAN, optionally configuring
* the port to send untagged frames. The operation is delegated to the port's
* control interface.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the port and VLAN.
*                                  \n Valid range: 1-4294967295.
* @param[in] VLANID - VLAN ID to which the port should be added.
*                     \n Valid range: 1-4094.
* @param[in] ulPortInstanceNumber - Instance number of the port to add to the VLAN.
*                                   \n Valid range: 1-4294967295.
* @param[in] bUntagged - Boolean flag indicating if egress packets should be untagged.
*                        \n TRUE: Packets will egress untagged.
*                        \n FALSE: Packets will egress tagged.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or port is not found.
*
*/
ANSC_STATUS
CosaDmlBrgVlanPortAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        int                         VLANID,
        ULONG                       ulPortInstanceNumber,
        BOOLEAN                     bUntagged
    );

/**
* @brief Remove a port from a VLAN.
*
* This function removes a bridge port from a specified VLAN. The operation is
* delegated to the port's control interface.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulBrgInstanceNumber - Instance number of the bridge containing the port and VLAN.
*                                  \n Valid range: 1-4294967295.
* @param[in] VLANID - VLAN ID from which the port should be removed.
*                     \n Valid range: 1-4094.
* @param[in] ulPortInstanceNumber - Instance number of the port to remove from the VLAN.
*                                   \n Valid range: 1-4294967295.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the bridge or port is not found.
*
*/
ANSC_STATUS
CosaDmlBrgVlanPortDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        int                         VLANID,
        ULONG                       ulPortInstanceNumber
    );

/**
* @brief Print Home Security VLAN PSM value for debugging.
*
* This function retrieves and prints the PSM value for the Home Security VLAN
* WiFi members configuration. Used for debugging and tracking VLAN corruption issues.
*
* @param[in] fun - Pointer to a null-terminated string containing the calling function name.
* @param[in] linenum - Line number from where this function is called.
*
* @return None.
*
*/
void CosaDmlPrintHSVlanPsmValue(char *fun,int linenum);

#endif

