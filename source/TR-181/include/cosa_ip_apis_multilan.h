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

    File: cosa_ip_apis_multilan.h

    For Data Model SBAPI Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        This file defines the APIs servicing TR-181
        data model IP Interface SBAPI integration.

    ---------------------------------------------------------------

    environment:

        Platform Independent, but with multi-LAN support

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/28/2013  initial revision.

**********************************************************************/

#ifndef _COSA_IP_APIS_MULTILAN_
#define _COSA_IP_APIS_MULTILAN_

#include "cosa_ip_apis.h"
#include "dmsb_tr181_psm_definitions.h"
#include "dml_tr181_custom_cfg.h"

/**************************************************************************
                        DATA STRUCTURE DEFINITIONS
**************************************************************************/

typedef struct
_DMSB_TR181_IP_IF
{
     SINGLE_LINK_ENTRY              Linkage;
     COSA_DML_IP_IF_CFG             Cfg;
     COSA_DML_IP_IF_INFO            Info;

     COSA_DML_IF_STATS              LastStats;
}
DMSB_TR181_IP_IF, *PDMSB_TR181_IP_IF;

#define  ACCESS_DMSB_TR181_IP_IF(p)                 ACCESS_CONTAINER(p, DMSB_TR181_IP_IF, Linkage)

typedef struct
_DMSB_TR181_IP_CONTEXT
{
    /* IP IF */
    BOOLEAN                         IpIfInit;
    SLIST_HEADER                    IpIfList;
}
DMSB_TR181_IP_CONTEXT, *PDMSB_TR181_IP_CONTEXT;

#define  DMSB_TR181_IP_CONTEXT_Init(p)                          \
            {                                                   \
                p->IpIfInit      = FALSE;                       \
                AnscSListInitializeHeader(&p->IpIfList);        \
            }

/**************************************************************************
                        ROUTINE PROTOTYPES
**************************************************************************/

/**
* @brief Initialize the IP multi-LAN subsystem.
*
* This function initializes the IP interface data model layer with multi-LAN support.
* It allocates and initializes the IP context structure for managing multiple LAN interfaces.
*
* @param[in] hDml - Handle to the DML object.
* @param[out] phContext - Pointer to a handle where the IP context will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_RESOURCES if memory allocation fails.
*
*/
ANSC_STATUS
CosaDmlIpMlanInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/*
 *  IP Interface
 */

/**
* @brief Get the number of IP interface entries.
*
* This function retrieves the count of IP interface entries in the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
*
* @return The number of IP interface entries.
*
*/
ULONG
CosaDmlIpIfMlanGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get an IP interface entry by index.
*
* This function retrieves an IP interface entry at the specified index from the
* multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIndex - Zero-based index of the IP interface entry to retrieve.
*                      \n Valid range: 0 to (number of entries - 1).
* @param[out] pEntry - Pointer to a COSA_DML_IP_IF_FULL structure where the IP interface
*                      information will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND otherwise
*/
ANSC_STATUS
CosaDmlIpIfMlanGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_IP_IF_FULL        pEntry
    );

/**
* @brief Set values for an IP interface entry.
*
* This function updates the instance number and alias for an IP interface entry
* in the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIndex - Zero-based index of the IP interface entry to update.
*                      \n Valid range: 0 to (number of entries - 1).
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
*                               \n Valid range: 1-4294967295.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND otherwise
*
*/
ANSC_STATUS
CosaDmlIpIfMlanSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new IP interface entry.
*
* This function adds a new IP interface entry to the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] pEntry - Pointer to a COSA_DML_IP_IF_FULL structure containing the IP interface
*                     configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_RESOURCES otherwise
*
*/
ANSC_STATUS
CosaDmlIpIfMlanAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_FULL        pEntry
    );

/**
* @brief Delete an IP interface entry.
*
* This function removes an IP interface entry from the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulInstanceNumber - Instance number of the IP interface entry to be deleted.
*                               \n Valid range: 1-4294967295.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND otherwise
*
*/
ANSC_STATUS
CosaDmlIpIfMlanDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of an IP interface.
*
* This function updates the configuration of an IP interface in the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] pCfg - Pointer to a COSA_DML_IP_IF_CFG structure containing the new configuration.
*                   \n The InstanceNumber field identifies which interface to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE or ANSC_STATUS_CANT_FIND otherwise
*
*/
ANSC_STATUS
CosaDmlIpIfMlanSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    );

/**
* @brief Get the configuration of an IP interface.
*
* This function retrieves the configuration of an IP interface from the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in,out] pCfg - Pointer to a COSA_DML_IP_IF_CFG structure.
*                       \n [in] The InstanceNumber field identifies which interface to query.
*                       \n [out] The structure will be filled with the interface configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND otherwise
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    );

/**
* @brief Get the runtime information of an IP interface.
*
* This function retrieves the dynamic runtime information of an IP interface
* from the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulInstanceNumber - Instance number of the IP interface to query.
*                               \n Valid range: 1-4294967295.
* @param[out] pInfo - Pointer to a COSA_DML_IP_IF_INFO structure where the runtime
*                     information will be returned.
*                     \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND otherwise
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IP_IF_INFO        pInfo
    );

/**
* @brief Reset an IP interface.
*
* This function resets an IP interface in the multi-LAN system, clearing its statistics
* and reinitializing its state.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulInstanceNumber - Instance number of the IP interface to reset.
*                               \n Valid range: 1-4294967295.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND otherwise
*
*/
ANSC_STATUS
CosaDmlIpIfMlanReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/*
 *  IP Interface IPv4Address
 */

/**
* @brief Get the number of IPv4 addresses for an IP interface.
*
* This function retrieves the count of IPv4 addresses configured on the specified
* IP interface in the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
*
* @return The number of IPv4 addresses on the interface.
*
*/
ULONG
CosaDmlIpIfMlanGetNumberOfV4Addrs
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber
    );

/**
* @brief Get the IPv4 address of an IP interface.
*
* This function retrieves the IPv4 address configured on the specified IP interface.
*
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[out] pEntry - Pointer to a COSA_DML_IP_V4ADDR structure where the IPv4 address
*                      information will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetIPv4Addr
    (
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

/**
* @brief Get the subnet mask of an IP interface.
*
* This function retrieves the subnet mask configured on the specified IP interface.
*
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[out] pEntry - Pointer to a COSA_DML_IP_V4ADDR structure where the subnet mask
*                      information will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetSubnetMask
    (
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

/**
* @brief Get an IPv4 address entry by index.
*
* This function retrieves an IPv4 address entry at the specified index for the given
* IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] ulIndex - Zero-based index of the IPv4 address entry to retrieve.
*                      \n Valid range: 0 to (number of IPv4 addresses - 1).
* @param[out] pEntry - Pointer to a COSA_DML_IP_V4ADDR structure where the IPv4 address
*                      information will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if index is zero
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

/**
* @brief Set values for an IPv4 address entry.
*
* This function updates the instance number and alias for an IPv4 address entry.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] ulIndex - Zero-based index of the IPv4 address entry to update.
*                      \n Valid range: 0 to (number of IPv4 addresses - 1).
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
*                               \n Valid range: 1-4294967295.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanSetV4AddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new IPv4 address entry.
*
* This function adds a new IPv4 address to the specified IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_IP_V4ADDR structure containing the IPv4 address
*                     configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported
*/
ANSC_STATUS
CosaDmlIpIfMlanAddV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

/**
* @brief Delete an IPv4 address entry.
*
* This function removes an IPv4 address from the specified IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_IP_V4ADDR structure identifying the IPv4 address
*                     to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported
*
*/
ANSC_STATUS
CosaDmlIpIfMlanDelV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

/**
* @brief Set the configuration of an IPv4 address.
*
* This function updates the configuration of an IPv4 address on the specified IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_IP_V4ADDR structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanSetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

/**
* @brief Get an IPv4 address entry by instance number.
*
* This function retrieves an IPv4 address entry identified by its instance number.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in,out] pEntry - Pointer to a COSA_DML_IP_V4ADDR structure.
*                         \n [in] The InstanceNumber field identifies which IPv4 address to query.
*                         \n [out] The structure will be filled with the IPv4 address information.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetV4Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry          /* Identified by InstanceNumber */
    );

/*
 *  IP Interface IPv6Address
 */

/**
* @brief Add a new IPv6 address entry.
*
* This function adds a new IPv6 address to the specified IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_IP_V6ADDR structure containing the IPv6 address
*                     configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported
*/
ANSC_STATUS
CosaDmlIpIfMlanAddV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

/**
* @brief Delete an IPv6 address entry.
*
* This function removes an IPv6 address from the specified IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_IP_V6ADDR structure identifying the IPv6 address
*                     to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE otherwise.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanDelV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

/**
* @brief Set the configuration of an IPv6 address.
*
* This function updates the configuration of an IPv6 address on the specified IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_IP_V6ADDR structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported
*
*/
ANSC_STATUS
CosaDmlIpIfMlanSetV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

/**
* @brief Get an IPv6 address entry by instance number.
*
* This function retrieves an IPv6 address entry identified by its instance number.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in,out] pEntry - Pointer to a COSA_DML_IP_V6ADDR structure.
*                         \n [in] The InstanceNumber field identifies which IPv6 address to query.
*                         \n [out] The structure will be filled with the IPv6 address information.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetV6Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry          /* Identified by InstanceNumber */
    );

/*
 *  IP Interface IPv6Prefix
 */

/**
* @brief Get the number of IPv6 prefixes for an IP interface.
*
* This function retrieves the count of IPv6 prefixes configured on the specified
* IP interface in the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
*
* @return The number of IPv6 prefixes on the interface.
*
*/
ULONG
CosaDmlIpIfMlanGetNumberOfV6Prefixs
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber
    );

/**
* @brief Get an IPv6 prefix entry by index.
*
* This function retrieves an IPv6 prefix entry at the specified index for the given
* IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] ulIndex - Zero-based index of the IPv6 prefix entry to retrieve.
*                      \n Valid range: 0 to (number of IPv6 prefixes - 1).
* @param[out] pEntry - Pointer to a COSA_DML_IP_V6PREFIX structure where the IPv6 prefix
*                      information will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

/**
* @brief Set values for an IPv6 prefix entry.
*
* This function updates the instance number and alias for an IPv6 prefix entry.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] ulIndex - Zero-based index of the IPv6 prefix entry to update.
*                      \n Valid range: 0 to (number of IPv6 prefixes - 1).
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
*                               \n Valid range: 1-4294967295.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanSetV6PrefixValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new IPv6 prefix entry.
*
* This function adds a new IPv6 prefix to the specified IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_IP_V6PREFIX structure containing the IPv6 prefix
*                     configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported
*/
ANSC_STATUS
CosaDmlIpIfMlanAddV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

/**
* @brief Delete an IPv6 prefix entry.
*
* This function removes an IPv6 prefix from the specified IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_IP_V6PREFIX structure identifying the IPv6 prefix
*                     to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanDelV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

/**
* @brief Set the configuration of an IPv6 prefix.
*
* This function updates the configuration of an IPv6 prefix on the specified IP interface.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in] pEntry - Pointer to a COSA_DML_IP_V6PREFIX structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanSetV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

/**
* @brief Get an IPv6 prefix entry by instance number.
*
* This function retrieves an IPv6 prefix entry identified by its instance number.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[in,out] pEntry - Pointer to a COSA_DML_IP_V6PREFIX structure.
*                         \n [in] The InstanceNumber field identifies which IPv6 prefix to query.
*                         \n [out] The structure will be filled with the IPv6 prefix information.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_UNAPPLICABLE if not supported.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetV6Prefix2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry          /* Identified by InstanceNumber */
    );

/*
 *  IP Interface statistics
 */

/**
* @brief Get the statistics of an IP interface.
*
* This function retrieves the traffic statistics for the specified IP interface
* in the multi-LAN system.
*
* @param[in] hContext - Handle to the IP context.
* @param[in] ulIpIfInstanceNumber - Instance number of the IP interface.
*                                   \n Valid range: 1-4294967295.
* @param[out] pStats - Pointer to a COSA_DML_IP_STATS structure where the statistics
*                      will be returned.
*                      \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if not found.
*
*/
ANSC_STATUS
CosaDmlIpIfMlanGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_STATS          pStats
    );


#endif
