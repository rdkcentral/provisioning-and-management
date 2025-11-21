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

    module: cosa_ethernet_apis.h

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


#ifndef  _COSA_ETHERNET_API_H
#define  _COSA_ETHERNET_API_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define MAXINSTANCE                 128
#define ETH_INTERFACE_MAX_ASSOC_DEVICES   256
#define MAC_SZ  6 /* Mac address in Hex format */
#define MACADDR_SZ 18 /* Expanded MAC address in 00:02:... format */

typedef  enum
_COSA_DML_ETH_DUPLEX_MODE
{
    COSA_DML_ETH_DUPLEX_Half        = 1,
    COSA_DML_ETH_DUPLEX_Full,
    COSA_DML_ETH_DUPLEX_Auto
}
COSA_DML_ETH_DUPLEX_MODE, *PCOSA_DML_ETH_DUPLEX_MODE;


typedef  struct
_COSA_DML_ETH_STATS
{
    ULONG                           BytesSent;
    ULONG                           BytesReceived;
    ULONG                           PacketsSent;
    ULONG                           PacketsReceived;
    ULONG                           ErrorsSent;
    ULONG                           ErrorsReceived;
    ULONG                           UnicastPacketsSent;
    ULONG                           UnicastPacketsReceived;
    ULONG                           DiscardPacketsSent;
    ULONG                           DiscardPacketsReceived;
    ULONG                           MulticastPacketsSent;
    ULONG                           MulticastPacketsReceived;
    ULONG                           BroadcastPacketsSent;
    ULONG                           BroadcastPacketsReceived;
    ULONG                           UnknownProtoPacketsReceived;
}
COSA_DML_ETH_STATS, *PCOSA_DML_ETH_STATS;


typedef  struct
_COSA_DML_ETH_PORT_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    LONG                            MaxBitRate;
    COSA_DML_ETH_DUPLEX_MODE        DuplexMode;
}
COSA_DML_ETH_PORT_CFG,  *PCOSA_DML_ETH_PORT_CFG;


/*
 *  Static portion of Ethernet port info
 */
typedef  struct
_COSA_DML_ETH_PORT_SINFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];
    BOOLEAN                         bUpstream;
    UCHAR                           MacAddress[6];
}
COSA_DML_ETH_PORT_SINFO,  *PCOSA_DML_ETH_PORT_SINFO;


/*
 *  Dynamic portion of Ethernet port info
 */
typedef  struct
_COSA_DML_ASSOCDEV_INFO
{
    CHAR                            MacAddress[MACADDR_SZ];
}
COSA_DML_ASSOCDEV_INFO, *PCOSA_DML_ASSOCDEV_INFO;

typedef  struct
_COSA_DML_ETH_PORT_DINFO
{
    COSA_DML_IF_STATUS              Status;
    ULONG                           CurrentBitRate;
    ULONG                           LastChange;
    ULONG                           AssocDevicesCount;
    UCHAR                           AssocDevices[(MAC_SZ*ETH_INTERFACE_MAX_ASSOC_DEVICES)];
}
COSA_DML_ETH_PORT_DINFO,  *PCOSA_DML_ETH_PORT_DINFO;


typedef  struct
_COSA_DML_ETH_PORT_FULL
{
    COSA_DML_ETH_PORT_CFG           Cfg;
    COSA_DML_ETH_PORT_SINFO         StaticInfo;
    COSA_DML_ETH_PORT_DINFO         DynamicInfo;
    COSA_DML_ASSOCDEV_INFO     	    AssocClient[ETH_INTERFACE_MAX_ASSOC_DEVICES];
}
COSA_DML_ETH_PORT_FULL, *PCOSA_DML_ETH_PORT_FULL;


typedef  struct
_COSA_DML_ETH_LINK_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    BOOLEAN                         bPriorityTagging;
    COSA_DML_LINK_TYPE              LinkType;           /* LinkType and LinkName/LinkInstNum constitutes LowerLayers */
    char                            LinkName[COSA_DML_IF_NAME_LENGTH];  /* This field is filled anyway */
    ULONG                           LinkInstNum;        /* Option 2, multi-LAN SB PSM design */
    char                            LowerLayers[COSA_DML_LOWERLAYER_NAME_LENGTH];   /* Only used in middle layer */
}
COSA_DML_ETH_LINK_CFG,  *PCOSA_DML_ETH_LINK_CFG;


/*
 *  Static portion of Ethernet Link info
 */
typedef  struct
_COSA_DML_ETH_LINK_SINFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];
    UCHAR                           MacAddress[6];
}
COSA_DML_ETH_LINK_SINFO,  *PCOSA_DML_ETH_LINK_SINFO;


/*
 *  Dynamic portion of Ethernet Link info
 */
typedef  struct
_COSA_DML_ETH_LINK_DINFO
{
    COSA_DML_IF_STATUS              Status;
    ULONG                           LastChange;
}
COSA_DML_ETH_LINK_DINFO,  *PCOSA_DML_ETH_LINK_DINFO;


typedef  struct
_COSA_DML_ETH_LINK_FULL
{
    COSA_DML_ETH_LINK_CFG           Cfg;
    COSA_DML_ETH_LINK_SINFO         StaticInfo;
    COSA_DML_ETH_LINK_DINFO         DynamicInfo;
}
COSA_DML_ETH_LINK_FULL, *PCOSA_DML_ETH_LINK_FULL;


/*
 *  Configuration portion of Ethernet VLAN Termination info
 */
typedef  struct
_COSA_DML_ETH_VLAN_TERMINATION_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    char                            LowerLayers[COSA_DML_IF_NAME_LENGTH];
    char                            EthLinkName[COSA_DML_IF_NAME_LENGTH];   /* associated Ethernet Link */
    ULONG                           VLANID;
}
COSA_DML_ETH_VLAN_TERMINATION_CFG,  *PCOSA_DML_ETH_VLAN_TERMINATION_CFG;


/*
 *  Static portion of Ethernet VLAN Termination info
 */
typedef  struct
_COSA_DML_ETH_VLAN_TERMINATION_SINFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];
}
COSA_DML_ETH_VLAN_TERMINATION_SINFO,  *PCOSA_DML_ETH_VLAN_TERMINATION_SINFO;


/*
 *  Dynamic portion of Ethernet VLAN Termination info
 */
typedef  struct
_COSA_DML_ETH_VLAN_TERMINATION_DINFO
{
    COSA_DML_IF_STATUS              Status;
    ULONG                           LastChange;
}
COSA_DML_ETH_VLAN_TERMINATION_DINFO,  *PCOSA_DML_ETH_VLAN_TERMINATION_DINFO;


typedef  struct
_COSA_DML_ETH_VLAN_TERMINATION_FULL
{
    COSA_DML_ETH_VLAN_TERMINATION_CFG   Cfg;
    COSA_DML_ETH_VLAN_TERMINATION_SINFO StaticInfo;
    COSA_DML_ETH_VLAN_TERMINATION_DINFO DynamicInfo;
    COSA_DML_ETH_STATS                  LastStats;
}
COSA_DML_ETH_VLAN_TERMINATION_FULL, *PCOSA_DML_ETH_VLAN_TERMINATION_FULL;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the Ethernet subsystem.
*
* This function initializes the Ethernet Data Model Library subsystem and prepares it for operation.
*
* @param[in] hDml - Handle to the DML object.
* @param[out] phContext - Pointer to a handle where the Ethernet context will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval Other ANSC_STATUS codes otherwise
*
*/
ANSC_STATUS
CosaDmlEthInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/*
 *  Ethernet Port
 */

/**
* @brief Get the number of Ethernet port entries.
*
* This function retrieves the count of Ethernet port entries.
*
* @param[in] hContext - Handle to the Ethernet context.
*
* @return The number of Ethernet port entries.
*
*/
ULONG
CosaDmlEthPortGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get an Ethernet port entry by index.
*
* This function retrieves an Ethernet port entry at the specified index.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulIndex - Zero-based index of the port entry to retrieve.
*                      \n Valid range: 0 to (number of entries - 1).
* @param[out] pEntry - Pointer to a COSA_DML_ETH_PORT_FULL structure where the port information
*                      will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlEthPortGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ETH_PORT_FULL     pEntry
    );

/**
* @brief Set values for an Ethernet port entry.
*
* This function updates the instance number and alias for an Ethernet port entry.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulIndex - Zero-based index of the port entry to update.
*                      \n Valid range: 0 to (number of entries - 1).
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
*                               \n Valid range: 1-4294967295.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*                     \n Maximum length: COSA_DML_IF_NAME_LENGTH.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval Other ANSC_STATUS codes otherwise
*
*/
ANSC_STATUS
CosaDmlEthPortSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Set the configuration of an Ethernet port.
*
* This function updates the configuration of an Ethernet port.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] pCfg - Pointer to a COSA_DML_ETH_PORT_CFG structure containing the new configuration.
*                   \n The InstanceNumber field identifies which port to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the port is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlEthPortSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration of an Ethernet port.
*
* This function retrieves the configuration of an Ethernet port.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in,out] pCfg - Pointer to a COSA_DML_ETH_PORT_CFG structure.
*                       \n [in] The InstanceNumber field identifies which port to query.
*                       \n [out] The structure will be filled with the port configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the port is not found.
*
*/
ANSC_STATUS
CosaDmlEthPortGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the dynamic information of an Ethernet port.
*
* This function retrieves the dynamic runtime information of an Ethernet port.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulInstanceNumber - Instance number of the port to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pInfo - Pointer to a COSA_DML_ETH_PORT_DINFO structure where the dynamic information
*                     will be returned.
*                     \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the port is not found.
*
*/
ANSC_STATUS
CosaDmlEthPortGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_PORT_DINFO    pInfo
    );

/**
* @brief Get the MAC addresses of clients connected to an Ethernet port.
*
* This function retrieves the MAC addresses of devices associated with an Ethernet port.
*
* @param[in,out] pEthernetPortFull - Pointer to a COSA_DML_ETH_PORT_FULL structure.
*                                    \n [in] Contains the port information.
*                                    \n [out] The AssocClient array will be filled with client MAC addresses.
* @param[in] ulInstanceNumber - Instance number of the port to query.
*                               \n Valid range: 1-4294967295.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlEthPortGetClientMac
    (
        PCOSA_DML_ETH_PORT_FULL pEthernetPortFull,
        ULONG                   ulInstanceNumber
    );

/**
* @brief Get the statistics of an Ethernet port.
*
* This function retrieves the traffic statistics for an Ethernet port.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulInstanceNumber - Instance number of the port to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pStats - Pointer to a COSA_DML_ETH_STATS structure where the statistics will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the port is not found.
*
*/
ANSC_STATUS
CosaDmlEthPortGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    );

/*
 *  Ethernet Link
 */

/**
* @brief Get the number of Ethernet link entries.
*
* This function retrieves the count of Ethernet link entries.
*
* @param[in] hContext - Handle to the Ethernet context.
*
* @return The number of Ethernet link entries.
*
*/
ULONG
CosaDmlEthLinkGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get an Ethernet link entry by index.
*
* This function retrieves an Ethernet link entry at the specified index.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulIndex - Zero-based index of the link entry to retrieve.
*                      \n Valid range: 0 to (number of entries - 1).
* @param[out] pEntry - Pointer to a COSA_DML_ETH_LINK_FULL structure where the link information
*                      will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND otherwise
*
*/
ANSC_STATUS
CosaDmlEthLinkGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    );

/**
* @brief Set values for an Ethernet link entry.
*
* This function updates the instance number and alias for an Ethernet link entry.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulIndex - Zero-based index of the link entry to update.
*                      \n Valid range: 0 to (number of entries - 1).
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
*                               \n Valid range: 1-4294967295.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*                     \n Maximum length: COSA_DML_IF_NAME_LENGTH.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND otherwise
*
*/
ANSC_STATUS
CosaDmlEthLinkSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new Ethernet link entry.
*
* This function adds a new Ethernet link entry.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] pEntry - Pointer to a COSA_DML_ETH_LINK_FULL structure containing the link
*                     configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlEthLinkAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    );

/**
* @brief Delete an Ethernet link entry.
*
* This function removes an Ethernet link entry.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulInstanceNumber - Instance number of the link entry to be deleted.
*                               \n Valid range: 1-4294967295.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlEthLinkDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of an Ethernet link.
*
* This function updates the configuration of an Ethernet link.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] pCfg - Pointer to a COSA_DML_ETH_LINK_CFG structure containing the new configuration.
*                   \n The InstanceNumber field identifies which link to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE  or ANSC_STATUS_CANT_FIND if the link is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlEthLinkSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration of an Ethernet link.
*
* This function retrieves the configuration of an Ethernet link.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in,out] pCfg - Pointer to a COSA_DML_ETH_LINK_CFG structure.
*                       \n [in] The InstanceNumber field identifies which link to query.
*                       \n [out] The structure will be filled with the link configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND if the link is not found.
*
*/
ANSC_STATUS
CosaDmlEthLinkGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the dynamic information of an Ethernet link.
*
* This function retrieves the dynamic runtime information of an Ethernet link.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulInstanceNumber - Instance number of the link to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pInfo - Pointer to a COSA_DML_ETH_LINK_DINFO structure where the dynamic information
*                     will be returned.
*                     \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND if the link is not found.
*
*/
ANSC_STATUS
CosaDmlEthLinkGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_LINK_DINFO    pInfo
    );

/**
* @brief Get the statistics of an Ethernet link.
*
* This function retrieves the traffic statistics for an Ethernet link.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulInstanceNumber - Instance number of the link to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pStats - Pointer to a COSA_DML_ETH_STATS structure where the statistics will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND if the link is not found.
*
*/
ANSC_STATUS
CosaDmlEthLinkGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    );

/**
* @brief Update the static lower layer name for an Ethernet link.
*
* This function updates the lower layer name reference for an Ethernet link.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulInstanceNumber - Instance number of the link to update.
*                               \n Valid range: 1-4294967295.
* @param[in,out] pEntry - Pointer to a COSA_DML_ETH_LINK_FULL structure.
*                         \n [in] Contains the link information.
*                         \n [out] The LinkName will be updated with the lower layer name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND if the link is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlEthLinkUpdateStaticLowerLayerName
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    );

/*
 *  Ethernet VLAN Termination
 */

/**
* @brief Get the number of Ethernet VLAN Termination entries.
*
* This function retrieves the count of Ethernet VLAN Termination entries.
*
* @param[in] hContext - Handle to the Ethernet context.
*
* @return The number of Ethernet VLAN Termination entries.
*
*/
ULONG
CosaDmlEthVlanTerminationGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get an Ethernet VLAN Termination entry by index.
*
* This function retrieves an Ethernet VLAN Termination entry at the specified index.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulIndex - Zero-based index of the termination entry to retrieve.
*                      \n Valid range: 0 to (number of entries - 1).
* @param[out] pEntry - Pointer to a COSA_DML_ETH_VLAN_TERMINATION_FULL structure where the
*                      termination information will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlEthVlanTerminationGetEntry
    (
        ANSC_HANDLE                         hContext,
        ULONG                               ulIndex,
        PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry
    );

/**
* @brief Set values for an Ethernet VLAN Termination entry.
*
* This function updates the instance number and alias for an Ethernet VLAN Termination entry.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulIndex - Zero-based index of the termination entry to update.
*                      \n Valid range: 0 to (number of entries - 1).
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
*                               \n Valid range: 1-4294967295.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*                     \n Maximum length: COSA_DML_IF_NAME_LENGTH.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlEthVlanTerminationSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new Ethernet VLAN Termination entry.
*
* This function adds a new Ethernet VLAN Termination entry.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] pEntry - Pointer to a COSA_DML_ETH_VLAN_TERMINATION_FULL structure containing
*                     the termination configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlEthVlanTerminationAddEntry
    (
        ANSC_HANDLE                         hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry
    );

/**
* @brief Delete an Ethernet VLAN Termination entry.
*
* This function removes an Ethernet VLAN Termination entry.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulInstanceNumber - Instance number of the termination entry to be deleted.
*                               \n Valid range: 1-4294967295.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlEthVlanTerminationDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Validate the configuration of an Ethernet VLAN Termination.
*
* This function validates the configuration parameters for an Ethernet VLAN Termination entry.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] pCfg - Pointer to a COSA_DML_ETH_VLAN_TERMINATION_CFG structure containing
*                   the configuration to validate.
* @param[out] pReturnParamName - Pointer to a buffer where the name of the invalid parameter
*                                will be returned (if validation fails).
*                                \n The buffer must be allocated by the caller.
* @param[in,out] puLength - Pointer to a ULONG.
*                           \n [in] Maximum size of the pReturnParamName buffer.
*                           \n [out] Actual length of the parameter name written.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the configuration is valid.
* @retval ANSC_STATUS_FAILURE if the configuration is invalid.
*
*/
ANSC_STATUS
CosaDmlEthVlanTerminationValidateCfg
    (
        ANSC_HANDLE                        hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg,
        char*                              pReturnParamName,
        ULONG*                             puLength
    );

/**
* @brief Set the configuration of an Ethernet VLAN Termination.
*
* This function updates the configuration of an Ethernet VLAN Termination.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] pCfg - Pointer to a COSA_DML_ETH_VLAN_TERMINATION_CFG structure containing
*                   the new configuration.
*                   \n The InstanceNumber field identifies which termination to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND if the termination is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlEthVlanTerminationSetCfg
    (
        ANSC_HANDLE                        hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration of an Ethernet VLAN Termination.
*
* This function retrieves the configuration of an Ethernet VLAN Termination.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in,out] pCfg - Pointer to a COSA_DML_ETH_VLAN_TERMINATION_CFG structure.
*                       \n [in] The InstanceNumber field identifies which termination to query.
*                       \n [out] The structure will be filled with the termination configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND if the termination is not found.
*
*/
ANSC_STATUS
CosaDmlEthVlanTerminationGetCfg
    (
        ANSC_HANDLE                        hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the dynamic information of an Ethernet VLAN Termination.
*
* This function retrieves the dynamic runtime information of an Ethernet VLAN Termination.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulInstanceNumber - Instance number of the termination to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pInfo - Pointer to a COSA_DML_ETH_VLAN_TERMINATION_DINFO structure where the
*                     dynamic information will be returned.
*                     \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND if the termination is not found.
*
*/
ANSC_STATUS
CosaDmlEthVlanTerminationGetDinfo
    (
        ANSC_HANDLE                          hContext,
        ULONG                                ulInstanceNumber,
        PCOSA_DML_ETH_VLAN_TERMINATION_DINFO pInfo
    );

/**
* @brief Get the statistics of an Ethernet VLAN Termination.
*
* This function retrieves the traffic statistics for an Ethernet VLAN Termination.
*
* @param[in] hContext - Handle to the Ethernet context.
* @param[in] ulInstanceNumber - Instance number of the termination to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pStats - Pointer to a COSA_DML_ETH_STATS structure where the statistics will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the termination is not found.
*
*/
ANSC_STATUS
CosaDmlEthVlanTerminationGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    );

#if defined(_HUB4_PRODUCT_REQ_) || defined(_RDKB_GLOBAL_PRODUCT_REQ_)
/**
* @brief Get the WAN link up/down time.
*
* This function retrieves the timestamp of the last WAN link up or down event.
*
* @param[out] pcWanUpDownTime - Pointer to a buffer where the timestamp will be returned.
*                               \n The buffer should be allocated by the caller.
* @param[in] nTimeLength - Maximum length of the pcWanUpDownTime buffer.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlEthLinkGetWanUpDownTime
    (
        char* pcWanUpDownTime,
	int nTimeLength
    );
#endif
#endif

