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

    module:	cosa_dml_api_dns.h

        For Data Model Library Implementation (DML),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structure and function
        prototypes for DML DNS API.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        12/15/2010    initial revision.

**********************************************************************/


#ifndef  _COSA_DML_API_DNS_
#define  _COSA_DML_API_DNS_

#include "cosa_dml_api_common.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

typedef  enum
_COSA_DML_DNS_STATUS
{
    COSA_DML_DNS_STATUS_Disabled        = 1,
    COSA_DML_DNS_STATUS_Enabled,
    COSA_DML_DNS_STATUS_Error
}
COSA_DML_DNS_STATUS, *PCOSA_DML_DNS_STATUS;


typedef  enum
_COSA_DML_DNS_ADDR_SRC
{
    COSA_DML_DNS_ADDR_SRC_DHCP          = 1,
    COSA_DML_DNS_ADDR_SRC_DHCPV4,
    COSA_DML_DNS_ADDR_SRC_DHCPV6,
    COSA_DML_DNS_ADDR_SRC_RouterAdvertisement,
    COSA_DML_DNS_ADDR_SRC_IPCP,
    COSA_DML_DNS_ADDR_SRC_Static
}
COSA_DML_DNS_ADDR_SRC, *PCOSA_DML_DNS_ADDR_SRC;


typedef  struct
_COSA_DML_DNS_CLIENT_SERVER
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_DNS_STATUS             Status;
    char                            DNSServer[64];
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH]; /* IP interface name */
    COSA_DML_DNS_ADDR_SRC           Type;
    ULONG                           Order;
}
COSA_DML_DNS_CLIENT_SERVER,  *PCOSA_DML_DNS_CLIENT_SERVER;


typedef  struct
_COSA_DML_DNS_RELAY_ENTRY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_DNS_STATUS             Status;
    ANSC_IPV4_ADDRESS               DNSServer;
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH]; /* IP interface name */
    COSA_DML_DNS_ADDR_SRC           Type;
}
COSA_DML_DNS_RELAY_ENTRY,  *PCOSA_DML_DNS_RELAY_ENTRY;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the DNS subsystem.
*
* This function initializes the DNS Data Model Library subsystem.
*
* @param[in] hDml - Handle to the DML object.
* @param[out] phContext - Pointer to a handle where the context will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/*
 *  DNS Client
 */

/**
* @brief Enable or disable the DNS client.
*
* This function enables or disables the DNS client functionality.
*
* @param[in] hContext - Handle to the context.
* @param[in] bEnabled - Boolean flag indicating whether to enable (TRUE) or disable (FALSE) the DNS client.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDnsEnableClient
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

/**
* @brief Get the DNS client status.
*
* This function retrieves the current operational status of the DNS client.
*
* @param[in] hContext - Handle to the context.
*
* @return The DNS client status.
* @retval COSA_DML_DNS_STATUS_Disabled if the DNS client is disabled.
* @retval COSA_DML_DNS_STATUS_Enabled if the DNS client is enabled.
* @retval COSA_DML_DNS_STATUS_Error if the DNS client is in an error state.
*
*/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetClientStatus
    (
        ANSC_HANDLE                 hContext
    );

/*
 *  DNS Client Server
 */

/**
* @brief Get the list of DNS client servers.
*
* This function retrieves the array of DNS servers configured for the DNS client.
* The memory for the returned array is allocated by the callee and must be freed by the caller.
*
* @param[in] hContext - Handle to the context.
* @param[out] pulCount - Pointer to a ULONG where the number of DNS server entries will be returned.
*
* @return Pointer to an array of COSA_DML_DNS_CLIENT_SERVER structures containing DNS server configurations.
* @retval Non-NULL Pointer to the allocated array of DNS server configurations.
* @retval NULL No DNS server entries were found or the operation failed.
*
*/
PCOSA_DML_DNS_CLIENT_SERVER
CosaDmlDnsClientGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

/**
* @brief Set values for a DNS client server entry.
*
* This function updates the instance number and alias for a DNS client server entry.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the server entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDnsClientSetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new DNS client server entry.
*
* This function adds a new DNS server to the DNS client configuration.
* The caller does not need to fill in Status or Alias fields. Field Type has to be COSA_DML_DNS_ADDR_SRC_Static.
* The caller does not have to fill in field Interface. Upon return, callee fills in the generated Alias.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DNS_CLIENT_SERVER structure.
*                         \n [in] Contains the DNS server configuration to be added.
*                         \n [out] The generated Alias field will be filled in by the function.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDnsClientAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    );

/**
* @brief Delete a DNS client server entry.
*
* This function removes a DNS server from the DNS client configuration.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the DNS server entry to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDnsClientDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a DNS client server.
*
* This function updates the configuration of a DNS server in the DNS client.
*
* @param[in] hContext - Handle to the context.
* @param[in] pEntry - Pointer to a COSA_DML_DNS_CLIENT_SERVER structure containing the new configuration.
*                     \n The InstanceNumber field identifies which entry to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDnsClientSetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry      /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration of a DNS client server.
*
* This function retrieves the configuration of a DNS server from the DNS client.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DNS_CLIENT_SERVER structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the DNS server configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDnsClientGetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry      /* Identified by InstanceNumber */
    );

/*
 *  DNS Relay
 */

/**
* @brief Enable or disable the DNS relay.
*
* This function enables or disables the DNS relay (forwarding) functionality.
*
* @param[in] hContext - Handle to the context.
* @param[in] bEnabled - Boolean flag indicating whether to enable (TRUE) or disable (FALSE) the DNS relay.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDnsEnableRelay
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

/**
* @brief Get the DNS relay status.
*
* This function retrieves the current operational status of the DNS relay.
*
* @param[in] hContext - Handle to the context.
*
* @return The DNS relay status.
* @retval COSA_DML_DNS_STATUS_Disabled if the DNS relay is disabled.
* @retval COSA_DML_DNS_STATUS_Enabled if the DNS relay is enabled.
* @retval COSA_DML_DNS_STATUS_Error if the DNS relay is in an error state.
*
*/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetRelayStatus
    (
        ANSC_HANDLE                 hContext
    );

/*
 *  DNS Relay Forwarding (server)
 */

/**
* @brief Get the list of DNS relay forwarding servers.
*
* This function retrieves the array of DNS servers configured for DNS relay forwarding.
* The memory for the returned array is allocated by the callee and must be freed by the caller.
*
* @param[in] hContext - Handle to the context.
* @param[out] pulCount - Pointer to a ULONG where the number of DNS relay server entries will be returned.
*
* @return Pointer to an array of COSA_DML_DNS_RELAY_ENTRY structures containing DNS relay server configurations.
* @retval Non-NULL Pointer to the array of DNS relay server configurations.
* @retval NULL if no entries are found or if the operation fails.
*
*/
PCOSA_DML_DNS_RELAY_ENTRY
CosaDmlDnsRelayGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

/**
* @brief Set values for a DNS relay server entry.
*
* This function updates the instance number and alias for a DNS relay server entry.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulIndex - Zero-based index of the server entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDnsRelaySetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new DNS relay server entry.
*
* This function adds a new DNS server to the DNS relay forwarding configuration.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] pEntry - Pointer to a COSA_DML_DNS_RELAY_ENTRY structure containing the DNS relay server
*                     configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDnsRelayAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    );

/**
* @brief Delete a DNS relay server entry.
*
* This function removes a DNS server from the DNS relay forwarding configuration.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] ulInstanceNumber - Instance number of the DNS relay server entry to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDnsRelayDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a DNS relay server.
*
* This function updates the configuration of a DNS server in the DNS relay forwarding.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in] pEntry - Pointer to a COSA_DML_DNS_RELAY_ENTRY structure containing the new configuration.
*                     \n The InstanceNumber field identifies which entry to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDnsRelaySetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry      /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration of a DNS relay server.
*
* This function retrieves the configuration of a DNS server from the DNS relay forwarding.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[in,out] pEntry - Pointer to a COSA_DML_DNS_RELAY_ENTRY structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the DNS relay server configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDnsRelayGetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry      /* Identified by InstanceNumber */
    );


#endif

