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

    module:	dml_api_dhcp.h

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

        Ding Hua

    ---------------------------------------------------------------

    revision:

        12/15/2010    initial revision.

**********************************************************************/


#ifndef  _DML_API_DHCPV6_
#define  _DML_API_DHCPV6_

#include "cosa_apis.h"
#include "cosa_dhcpv4_apis.h"
#include "cosa_mapt_apis.h"
#include "dml_tr181_custom_cfg.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define  COSA_DML_DHCP_MAX_ENTRIES                  4
#define  COSA_DML_DHCP_MAX_RESERVED_ADDRESSES       8
#define  COSA_DML_DHCP_MAX_OPT_ENTRIES              8

#define _DEBUG_DHCPV6
#ifdef _DEBUG_DHCPV6
    #define ULOGF ulogf
#else
    #define ULOGF
#endif

#define COSA_DML_DHCPV6_CLIENT_IFNAME                 CFG_TR181_DHCPv6_CLIENT_IfName
#define COSA_DML_DHCPV6_SERVER_IFNAME                 CFG_TR181_DHCPv6_SERVER_IfName

#define COSA_DML_DHCPV6C_PREF_SYSEVENT_NAME           "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_v6pref"
#define COSA_DML_DHCPV6C_PREF_IAID_SYSEVENT_NAME      "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_iaid"
#define COSA_DML_DHCPV6C_PREF_T1_SYSEVENT_NAME        "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_t1"
#define COSA_DML_DHCPV6C_PREF_T2_SYSEVENT_NAME        "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_t2"
#define COSA_DML_DHCPV6C_PREF_PRETM_SYSEVENT_NAME     "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_pretm"
#define COSA_DML_DHCPV6C_PREF_VLDTM_SYSEVENT_NAME     "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_vldtm"

#define COSA_DML_DHCPV6C_ADDR_SYSEVENT_NAME           "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_v6addr"
#define COSA_DML_DHCPV6C_ADDR_IAID_SYSEVENT_NAME      "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_iaid"
#define COSA_DML_DHCPV6C_ADDR_T1_SYSEVENT_NAME        "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_t1"
#define COSA_DML_DHCPV6C_ADDR_T2_SYSEVENT_NAME        "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_t2"
#define COSA_DML_DHCPV6C_ADDR_PRETM_SYSEVENT_NAME     "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_pretm"
#define COSA_DML_DHCPV6C_ADDR_VLDTM_SYSEVENT_NAME     "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_vldtm"

#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
#define COSA_DML_WANIface_PREF_SYSEVENT_NAME           "tr_%s_dhcpv6_client_v6pref"
#define COSA_DML_WANIface_PREF_IAID_SYSEVENT_NAME      "tr_%s_dhcpv6_client_pref_iaid"
#define COSA_DML_WANIface_PREF_T1_SYSEVENT_NAME        "tr_%s_dhcpv6_client_pref_t1"
#define COSA_DML_WANIface_PREF_T2_SYSEVENT_NAME        "tr_%s_dhcpv6_client_pref_t2"
#define COSA_DML_WANIface_PREF_PRETM_SYSEVENT_NAME     "tr_%s_dhcpv6_client_pref_pretm"
#define COSA_DML_WANIface_PREF_VLDTM_SYSEVENT_NAME     "tr_%s_dhcpv6_client_pref_vldtm"

#define COSA_DML_WANIface_ADDR_SYSEVENT_NAME           "tr_%s_dhcpv6_client_v6addr"
#define COSA_DML_WANIface_ADDR_IAID_SYSEVENT_NAME      "tr_%s_dhcpv6_client_addr_iaid"
#define COSA_DML_WANIface_ADDR_T1_SYSEVENT_NAME        "tr_%s_dhcpv6_client_addr_t1"
#define COSA_DML_WANIface_ADDR_T2_SYSEVENT_NAME        "tr_%s_dhcpv6_client_addr_t2"
#define COSA_DML_WANIface_ADDR_PRETM_SYSEVENT_NAME     "tr_%s_dhcpv6_client_addr_pretm"
#define COSA_DML_WANIface_ADDR_VLDTM_SYSEVENT_NAME     "tr_%s_dhcpv6_client_addr_vldtm"
#endif

#if defined (CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION) || defined (INTEL_PUMA7)
#define COSA_DML_DHCPV6S_ADDR_SYSEVENT_NAME      "ipv6_"COSA_DML_DHCPV6_SERVER_IFNAME"-addr"
#else
#define COSA_DML_DHCPV6S_ADDR_SYSEVENT_NAME      "tr_"COSA_DML_DHCPV6_SERVER_IFNAME"_dhcpv6_server_v6addr"
#endif

/*
 *  DHCP Client
 */

#if defined (_HUB4_PRODUCT_REQ_) || defined (FEATURE_SUPPORT_MAPT_NAT46)
#define SYSEVENT_MAPT_CONFIG_FLAG "mapt_config_flag"
#define SYSEVENT_MAPT_RATIO "mapt_ratio"
#define SYSEVENT_MAP_RULE_IPADDRESS "map_rule_ip_address"
#define SYSEVENT_MAPT_PSID_OFFSET "mapt_psid_offset"
#define SYSEVENT_MAPT_PSID_VALUE "mapt_psid_value"
#define SYSEVENT_MAPT_PSID_LENGTH "mapt_psid_length"
#define SYSEVENT_MAP_RULE_IPV6_ADDRESS "map_rule_ipv6_address"
#define SYSEVENT_MAP_EA_LEN "map_ea_length"
#define SYSEVENT_MAP_TRANSPORT_MODE "map_transport_mode"
#define SYSEVENT_MAP_IS_FMR "map_is_fmr"
#define SYSEVENT_MAP_BR_IPV6_PREFIX        "map_br_ipv6_prefix"
#define SYSEVENT_MAPT_IPADDRESS "mapt_ip_address"
#endif

typedef  struct
_COSA_DML_DHCPCV6_SVR
{
    UCHAR                           SourceAddress[40];
    UCHAR                           DUID[131]; /* IP interface name */
    UCHAR                           InformationRefreshTime[32];
}
COSA_DML_DHCPCV6_SVR,  *PCOSA_DML_DHCPCV6_SVR;

typedef  struct
_COSA_DML_DHCPCV6_CFG
{
    ULONG                           InstanceNumber;
    UCHAR                           Alias[COSA_DML_ALIAS_NAME_LENGTH];
    LONG                            SuggestedT1;
    LONG                            SuggestedT2;
    UCHAR                           Interface[COSA_DML_ALIAS_NAME_LENGTH]; /* IP interface name */
    UCHAR                           RequestedOptions[512];
    BOOLEAN                         bEnabled;
    BOOLEAN                         RequestAddresses;
    BOOLEAN                         RequestPrefixes;
    BOOLEAN                         RapidCommit;
    BOOLEAN                         Renew;
}
COSA_DML_DHCPCV6_CFG,  *PCOSA_DML_DHCPCV6_CFG;

typedef struct
_COSA_DML_DHCPCV6_INFO
{
    COSA_DML_DHCP_STATUS            Status;
    UCHAR                           SupportedOptions[512];
    UCHAR                           DUID[131];
}
COSA_DML_DHCPCV6_INFO,  *PCOSA_DML_DHCPCV6_INFO;


typedef  struct
_COSA_DML_DHCPCV6_FULL
{
    COSA_DML_DHCPCV6_CFG              Cfg;
    COSA_DML_DHCPCV6_INFO             Info;
}
COSA_DML_DHCPCV6_FULL, *PCOSA_DML_DHCPCV6_FULL;

typedef  struct
_COSA_DML_DHCPCV6_SENT
{
    ULONG                           InstanceNumber;
    UCHAR                           Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           Tag;
    UCHAR                           Value[255];
}
COSA_DML_DHCPCV6_SENT,  *PCOSA_DML_DHCPCV6_SENT;

struct
_COSA_DML_DHCPCV6_RECV
{
    SINGLE_LINK_ENTRY               Link;
    ULONG                           Tag;
    UCHAR                           Server[255];
    UCHAR                           Value[1024];
};
typedef struct _COSA_DML_DHCPCV6_RECV COSA_DML_DHCPCV6_RECV,  *PCOSA_DML_DHCPCV6_RECV;

#define  ACCESS_DHCPV6_RECV_LINK_OBJECT(p)              \
         ACCESS_CONTAINER(p, COSA_DML_DHCPCV6_RECV, Link)

/*
 *  DHCP Server
 */
struct
_COSA_DML_DHCPSV6_CFG
{
    BOOLEAN                         bEnabled;
};
typedef struct _COSA_DML_DHCPSV6_CFG COSA_DML_DHCPSV6_CFG,  *PCOSA_DML_DHCPSV6_CFG;


struct
_COSA_DML_DHCPSV6_POOL_CFG
{
    ULONG                           InstanceNumber;
    UCHAR                           Alias[COSA_DML_ALIAS_NAME_LENGTH];

    ULONG                           Order;
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH];         /* IP interface name */
    char                            VendorClassID[256];
    UCHAR                           UserClassID[256];
    UCHAR                           SourceAddress[40];
    UCHAR                           SourceAddressMask[40];
    UCHAR                           IANAManualPrefixes[320];
    UCHAR                           IAPDManualPrefixes[320];
    UCHAR                           PrefixRangeBegin[64];
    UCHAR                           PrefixRangeEnd[64];
    UCHAR                           StartAddress[64];
    signed long                     LeaseTime;
    ULONG                           IAPDAddLength;
    ULONG                           IANAAmount;
    UCHAR                           DUID[131];
    BOOLEAN                         IAPDEnable;
    BOOLEAN                         SourceAddressExclude;
    BOOLEAN                         IANAEnable;
    BOOLEAN                         UserClassIDExclude;
    BOOLEAN                         VendorClassIDExclude;
    BOOLEAN                         DUIDExclude;
    BOOLEAN                         bEnabled;
    BOOLEAN                         UnicastEnable;
    BOOLEAN                         RapidEnable;
    BOOLEAN                         EUI64Enable;
    BOOLEAN                         X_RDKCENTRAL_COM_DNSServersEnabled;
    UCHAR                           X_RDKCENTRAL_COM_DNSServers[256];
};
typedef struct _COSA_DML_DHCPSV6_POOL_CFG COSA_DML_DHCPSV6_POOL_CFG,  *PCOSA_DML_DHCPSV6_POOL_CFG;


struct
_COSA_DML_DHCPSV6_POOL_INFO
{
    COSA_DML_DHCP_STATUS            Status;
    UCHAR                           IANAPrefixes[320];
    UCHAR                           IAPDPrefixes[320];
};
typedef  struct _COSA_DML_DHCPSV6_POOL_INFO COSA_DML_DHCPSV6_POOL_INFO,  *PCOSA_DML_DHCPSV6_POOL_INFO;


struct
_COSA_DML_DHCPSV6_POOL_FULL
{
    COSA_DML_DHCPSV6_POOL_CFG    Cfg;
    COSA_DML_DHCPSV6_POOL_INFO   Info;
};
typedef  struct _COSA_DML_DHCPSV6_POOL_FULL COSA_DML_DHCPSV6_POOL_FULL, *PCOSA_DML_DHCPSV6_POOL_FULL;


struct
_COSA_DML_DHCPSV6_CLIENT
{
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    UCHAR                           SourceAddress[40];
    BOOLEAN                         Active;
};
typedef  struct _COSA_DML_DHCPSV6_CLIENT COSA_DML_DHCPSV6_CLIENT,  *PCOSA_DML_DHCPSV6_CLIENT;

struct
_COSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS
{
    UCHAR                           IPAddress[40];
    UCHAR                           PreferredLifetime[32];
    UCHAR                           ValidLifetime[32];
};
typedef  struct _COSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS COSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS,  *PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS;

struct
_COSA_DML_DHCPSV6_CLIENT_IPV6PREFIX
{
    UCHAR                           Prefix[40];
    UCHAR                           PreferredLifetime[32];
    UCHAR                           ValidLifetime[32];
};
typedef  struct _COSA_DML_DHCPSV6_CLIENT_IPV6PREFIX COSA_DML_DHCPSV6_CLIENT_IPV6PREFIX,  *PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX;

struct
_COSA_DML_DHCPSV6_CLIENT_OPTION
{
    ULONG                           Tag;
    UCHAR                           Value[255];
};
typedef  struct _COSA_DML_DHCPSV6_CLIENT_OPTION COSA_DML_DHCPSV6_CLIENT_OPTION,  *PCOSA_DML_DHCPSV6_CLIENT_OPTION;

struct
_COSA_DML_DHCPSV6_CLIENTCONTENT
{
    PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS     pIPv6Address;
    PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX      pIPv6Prefix;
    PCOSA_DML_DHCPSV6_CLIENT_OPTION          pOption;
    ULONG                                    NumberofIPv6Address;
    ULONG                                    NumberofIPv6Prefix;
    ULONG                                    NumberofOption;
};
typedef  struct _COSA_DML_DHCPSV6_CLIENTCONTENT COSA_DML_DHCPSV6_CLIENTCONTENT,  *PCOSA_DML_DHCPSV6_CLIENTCONTENT;

struct
_COSA_DML_DHCPSV6_POOL_OPTION
{
    ULONG                           InstanceNumber;
    UCHAR                           Alias[COSA_DML_ALIAS_NAME_LENGTH];

    ULONG                           Tag;
    UCHAR                           PassthroughClient[COSA_DML_ALIAS_NAME_LENGTH];
    UCHAR                           Value[255];
    BOOLEAN                         bEnabled;
};
typedef struct _COSA_DML_DHCPSV6_POOL_OPTION COSA_DML_DHCPSV6_POOL_OPTION,  *PCOSA_DML_DHCPSV6_POOL_OPTION;

struct DHCP_TAG
{
    int tag;
    char * cmdstring;
};

/**
* @brief Check if a DHCP tag is permitted.
*
* This function validates whether the specified DHCP option tag is permitted for configuration.
*
* @param[in] tag - The DHCP option tag number to check.
*
* @return The validation status.
* @retval TRUE if the tag is permitted.
* @retval FALSE if the tag is not permitted.
*
*/
BOOL tagPermitted(int tag);

/**
* @brief Convert datetime string to seconds.
*
* This function converts a datetime string representation to seconds.
*
* @param[in] p_dt - Pointer to a null-terminated string containing the datetime value.
*
* @return The time value in seconds.
* @retval The time value in seconds on success.
* @retval 0 if the input string is NULL or the parsing fails.
*
*/
int _datetime_to_secs(char * p_dt);

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the DHCPv6 subsystem.
*
* This function initializes the DHCPv6 subsystem, setting up necessary data structures
* and preparing the system for DHCPv6 client and server operations.
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
CosaDmlDhcpv6Init
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Get the total number of DHCPv6 client entries.
*
* This function retrieves the count of configured DHCPv6 client instances.
*
* @param[in] hContext - Handle to the context.
*
* @return The number of DHCPv6 client entries.
*
*/
ULONG
CosaDmlDhcpv6cGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get a DHCPv6 client entry by index.
*
* This function retrieves the full configuration and information for a DHCPv6 client at the specified index position.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the client entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPCV6_FULL structure where the client
*                      configuration and information will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPCV6_FULL      pEntry
    );

/**
* @brief Set the instance number and alias for a DHCPv6 client entry.
*
* This function updates the instance number and alias for a DHCPv6 client at the specified index.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the client entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the client.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new DHCPv6 client entry.
*
* This function creates a new DHCPv6 client entry with the specified configuration.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPCV6_FULL structure containing the
*                         client configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPCV6_FULL      pEntry
    );

/**
* @brief Delete a DHCPv6 client entry.
*
* This function removes a DHCPv6 client entry identified by instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the client to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a DHCPv6 client.
*
* This function updates the configuration parameters of a DHCPv6 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] pCfg - Pointer to a COSA_DML_DHCPCV6_CFG structure containing the new configuration, even Alias field can be changed.
*                   \n The InstanceNumber field identifies which client to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPCV6_CFG       pCfg
    );

/**
* @brief Get the configuration of a DHCPv6 client.
*
* This function retrieves the current configuration parameters of a DHCPv6 client.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pCfg - Pointer to a COSA_DML_DHCPCV6_CFG structure.
*                       \n [in] The InstanceNumber field identifies which client to query.
*                       \n [out] The structure will be filled with the client configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPCV6_CFG       pCfg
    );

/**
* @brief Get runtime information of a DHCPv6 client.
*
* This function retrieves runtime status and information for a DHCPv6 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the client to query.
* @param[out] pInfo - Pointer to a COSA_DML_DHCPCV6_INFO structure where the client
*                     runtime information will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPCV6_INFO      pInfo
    );

/**
* @brief Get DHCPv6 server configuration for a client.
*
* This function retrieves the list of DHCPv6 servers contacted by a specific client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv6 client.
* @param[out] ppCfg - Pointer to a COSA_DML_DHCPCV6_SVR pointer where the server list
*                     will be returned.
* @param[out] pSize - Pointer to a ULONG where the number of server entries will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cGetServerCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SVR      *ppCfg,
        PULONG                      pSize
    );

/**
* @brief Renew the DHCPv6 lease for a client.
*
* This function forces a DHCPv6 lease renewal for the specified client instance.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the DHCPv6 client.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client is not found or renewal fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cRenew
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Get the number of sent options for a DHCPv6 client.
*
* This function retrieves the count of DHCPv6 options configured to be sent by a specific client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv6 client.
*
* @return The number of sent option entries for the client.
*
*/
ULONG
CosaDmlDhcpv6cGetNumberOfSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    );

/**
* @brief Get a sent option entry by index.
*
* This function retrieves a DHCPv6 sent option entry at the specified index for a client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv6 client.
* @param[in] ulIndex - Zero-based index of the sent option entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPCV6_SENT structure where the option
*                      configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cGetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    );

/**
* @brief Get a sent option entry by instance number.
*
* This function retrieves a DHCPv6 sent option entry identified by instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv6 client.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPCV6_SENT structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the option configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cGetSentOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    );

/**
* @brief Set the instance number and alias for a sent option entry.
*
* This function updates the instance number and alias for a DHCPv6 sent option.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv6 client.
* @param[in] ulIndex - Zero-based index of the sent option entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cSetSentOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new sent option entry to a DHCPv6 client.
*
* This function adds a new DHCPv6 option to be sent by a client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv6 client.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPCV6_SENT structure containing the
*                         option configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cAddSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    );

/**
* @brief Delete a sent option entry from a DHCPv6 client.
*
* This function removes a DHCPv6 sent option from a client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv6 client.
* @param[in] ulInstanceNumber - Instance number of the sent option to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cDelSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a sent option.
*
* This function updates the configuration of a DHCPv6 sent option.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv6 client.
* @param[in] pEntry - Pointer to a COSA_DML_DHCPCV6_SENT structure containing the new configuration.
*                     \n The InstanceNumber field identifies which entry to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cSetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    );

/**
* @brief Get received DHCPv6 options from server.
*
* This function retrieves the list of DHCPv6 options received by a client from the server.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv6 client.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPCV6_RECV pointer where the received options list
*                      will be returned.
* @param[out] pSize - Pointer to a ULONG where the number of received option entries will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6cGetReceivedOptionCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_RECV     *pEntry,
        PULONG                      pSize
    );

/**
* @brief Enable or disable the DHCPv6 server.
*
* This function enables or disables the DHCPv6 server functionality.
*
* @param[in] hContext - Handle to the context.
* @param[in] bEnable - Boolean flag to enable or disable the server.
*                      \n TRUE to enable, FALSE to disable.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sEnable
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bEnable
    );

/**
* @brief Format DNS option for DHCPv6 server.
*
* This function formats the DNS option string for DHCPv6 server configuration.
*
* @param[in,out] option - Pointer to a null-terminated string containing the DNS option to format.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval -1 if the input option pointer is NULL.
*
*/
int CosaDmlDhcpv6s_format_DNSoption( char *option );

/**
* @brief Get the current state of the DHCPv6 server.
*
* This function retrieves the current operational state of the DHCPv6 server.
*
* @param[in] hContext - Handle to the context.
*
* @return The state of the DHCPv6 server.
* @retval TRUE if the server is enabled.
* @retval FALSE if the server is disabled.
*
*/
BOOLEAN
CosaDmlDhcpv6sGetState
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get the number of DHCPv6 server pools.
*
* This function retrieves the total count of configured DHCPv6 server pools.
*
* @param[in] hContext - Handle to the context.
*
* @return The number of DHCPv6 server pool entries.
*
*/
ULONG
CosaDmlDhcpv6sGetNumberOfPools
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get a DHCPv6 server pool entry by index.
*
* This function retrieves the full configuration and information for a DHCPv6 server pool
* at the specified index position.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the pool entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPSV6_POOL_FULL structure where the pool
*                      configuration and information will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV6_POOL_FULL   pEntry
    );

/**
* @brief Set values for a DHCPv6 server pool entry.
*
* This function updates the instance number and alias for a DHCPv6 server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the pool entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the pool.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sSetPoolValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new DHCPv6 server pool.
*
* This function creates a new DHCPv6 server pool with the specified configuration.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPSV6_POOL_FULL structure containing the
*                         pool configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sAddPool
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPSV6_POOL_FULL   pEntry
    );

/**
* @brief Delete a DHCPv6 server pool.
*
* This function removes a DHCPv6 server pool identified by instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the pool to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sDelPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a DHCPv6 server pool.
*
* This function updates the configuration parameters of a DHCPv6 server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] pCfg - Pointer to a COSA_DML_DHCPSV6_POOL_CFG structure containing the new configuration.
*                   \n The InstanceNumber field identifies which pool to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sSetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPSV6_POOL_CFG    pCfg
    );

/**
* @brief Get the configuration of a DHCPv6 server pool.
*
* This function retrieves the current configuration parameters of a DHCPv6 server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pCfg - Pointer to a COSA_DML_DHCPSV6_POOL_CFG structure.
*                       \n [in] The InstanceNumber field identifies which pool to query.
*                       \n [out] The structure will be filled with the pool configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPSV6_POOL_CFG    pCfg
    );

/**
* @brief Get runtime information of a DHCPv6 server pool.
*
* This function retrieves runtime status and statistics for a DHCPv6 server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the pool to query.
* @param[out] pInfo - Pointer to a COSA_DML_DHCPSV6_POOL_INFO structure where the pool
*                     runtime information will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetPoolInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_INFO pInfo
    );

/**
* @brief Get client information from a DHCPv6 server pool.
*
* This function retrieves the list of clients currently associated with a DHCPv6 server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[out] ppEntry - Pointer to a COSA_DML_DHCPSV6_CLIENT pointer where the client list
*                       will be returned.
* @param[out] pSize - Pointer to a ULONG where the number of client entries will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetClient
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_CLIENT   *ppEntry,
        PULONG                      pSize
    );

/**
* @brief Get IPv6 addresses for a client in a server pool.
*
* This function retrieves the list of IPv6 addresses assigned to a specific client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[in] ulClientIndex - Zero-based index of the client.
* @param[out] ppEntry - Pointer to a COSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS pointer where the address list
*                       will be returned.
* @param[out] pSize - Pointer to a ULONG where the number of address entries will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetIPv6Address
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulClientIndex,
        PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS   *ppEntry,
        PULONG                      pSize
    );

/**
* @brief Get IPv6 prefixes for a client in a server pool.
*
* This function retrieves the list of IPv6 prefixes delegated to a specific client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[in] ulClientIndex - Zero-based index of the client.
* @param[out] ppEntry - Pointer to a COSA_DML_DHCPSV6_CLIENT_IPV6PREFIX pointer where the prefix list
*                       will be returned.
* @param[out] pSize - Pointer to a ULONG where the number of prefix entries will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetIPv6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulClientIndex,
        PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX   *ppEntry,
        PULONG                      pSize
    );

/**
* @brief Get DHCPv6 options for a client in a server pool.
*
* This function retrieves the list of DHCPv6 options assigned to a specific client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[in] ulClientIndex - Zero-based index of the client.
* @param[out] ppEntry - Pointer to a COSA_DML_DHCPSV6_CLIENT_OPTION pointer where the option list
*                       will be returned.
* @param[out] pSize - Pointer to a ULONG where the number of option entries will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetIPv6Option
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulClientIndex,
        PCOSA_DML_DHCPSV6_CLIENT_OPTION   *ppEntry,
        PULONG                      pSize
    );

/**
* @brief Get the number of DHCPv6 options in a server pool.
*
* This function retrieves the count of DHCPv6 options configured for a specific server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
*
* @return The number of DHCPv6 option entries in the pool.
*
*/
ULONG
CosaDmlDhcpv6sGetNumberOfOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    );

/**
* @brief Get a DHCPv6 option entry by index.
*
* This function retrieves a DHCPv6 option entry at the specified index within a server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[in] ulIndex - Zero-based index of the option entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPSV6_POOL_OPTION structure where the option
*                      configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV6_POOL_OPTION    pEntry
    );

/**
* @brief Get a DHCPv6 option entry by instance number.
*
* This function retrieves a DHCPv6 option entry identified by instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPSV6_POOL_OPTION structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the option configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION  pEntry
    );

/**
* @brief Set values for a DHCPv6 option entry.
*
* This function updates the instance number and alias for a DHCPv6 option.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[in] ulIndex - Zero-based index of the option entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new DHCPv6 option to a pool.
*
* This function adds a new DHCPv6 option entry to a server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPSV6_POOL_OPTION structure containing the
*                         option configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION          pEntry
    );

/**
* @brief Delete a DHCPv6 option from a pool.
*
* This function removes a DHCPv6 option entry from a server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[in] ulInstanceNumber - Instance number of the option to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a DHCPv6 option.
*
* This function updates the configuration of a DHCPv6 option in a server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCPv6 server pool.
* @param[in] pEntry - Pointer to a COSA_DML_DHCPSV6_POOL_OPTION structure containing the new configuration.
*                     \n The InstanceNumber field identifies which entry to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION          pEntry
    );

/* TBC  -- the functions below should be reviewed, on necessity and name convention */

/**
* @brief Remove DHCPv6 configuration.
*
* This function performs cleanup operations to remove DHCPv6 configuration and resources.
*
* @param[in] hContext - Handle to the context.
*
* @return None.
*
*/
void
CosaDmlDhcpv6Remove
    (
        ANSC_HANDLE hContext
    );

/**
* @brief Start the DHCPv6 client.
*
* This function initializes and starts the DHCPv6 client to begin address/prefix acquisition.
*
* @return The status of the operation.
* @retval 0 if successful.
* @retval Non-zero error code if the operation fails.
*
*/
int
CosaDmlStartDHCP6Client
    (
    void
    );

/**
* @brief Get IAPD prefixes from the DHCPv6 server.
*
* This function retrieves the Identity Association for Prefix Delegation (IAPD) prefixes
* received from the DHCPv6 server.
*
* @param[in] pCfg - Pointer to the DHCPv6 server pool configuration structure.
* @param[out] pValue - Pointer to a buffer where the formatted prefix value will be returned.
* @param[in,out] pSize - Pointer to a ULONG.
*                        \n [in] Maximum size of the pValue buffer.
*                        \n [out] Actual size of the data written to pValue.
*
* @return The status of the operation.
* @retval 0 if successful.
* @retval Non-zero error code if the operation fails.
*
*/
int
CosaDmlDhcpv6sGetIAPDPrefixes
    (
        PCOSA_DML_DHCPSV6_POOL_CFG  pCfg,
        char*                       pValue,
        ULONG*                      pSize
     );

#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
/**
* @brief Get IAPD prefixes (alternative implementation).
*
* This function retrieves the Identity Association for Prefix Delegation (IAPD) prefixes.
*
* @param[in] pCfg - Pointer to the DHCPv6 server pool configuration structure.
* @param[out] pValue - Pointer to a buffer where the formatted prefix value will be returned.
* @param[in,out] pSize - Pointer to a ULONG.
*                        \n [in] Maximum size of the pValue buffer.
*                        \n [out] Actual size of the data written to pValue.
*
* @return The status of the operation.
* @retval 0 if successful.
* @retval Non-zero error code if the operation fails.
*
*/
int
CosaDmlDhcpv6sGetIAPDPrefixes2
    (
        PCOSA_DML_DHCPSV6_POOL_CFG  pCfg,
        char*                       pValue,
        ULONG*                      pSize
     );
#endif

/**
* @brief Get the DHCPv6 server type.
*
* This function retrieves the current DHCPv6 server type configuration.
*
* @param[in] hContext - Handle to the context.
*
* @return The server type as a ULONG value.
*
*/
ULONG
CosaDmlDhcpv6sGetType
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Ping a DHCPv6 client.
*
* This function performs an ICMPv6 ping to test reachability of a DHCPv6 client.
*
* @param[in] pDhcpsClient - Pointer to a COSA_DML_DHCPSV6_CLIENT structure containing
*                           the client information.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the ping is successful.
* @retval ANSC_STATUS_FAILURE if the ping fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sPing
    (
        PCOSA_DML_DHCPSV6_CLIENT    pDhcpsClient
    );

/**
* @brief Get the start address of the DHCPv6 pool.
*
* This function retrieves the starting IPv6 address of the DHCPv6 server pool.
*
* @param[in] hContext - Handle to the context. Reserved for future use.
* @param[out] addr - Pointer to a buffer where the start address will be returned.
* @param[in] len - Maximum length of the addr buffer.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sGetStartAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       addr,
        int                         len
    );

/**
* @brief Set the DHCPv6 server type.
*
* This function configures the DHCPv6 server type.
*
* @param[in] hContext - Handle to the context.
* @param[in] type - The server type value to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpv6sSetType
    (
        ANSC_HANDLE                 hContext,
        ULONG                        type
    );

/**
* @brief Get the enabled status of DHCPv6 client.
*
* This function retrieves whether the DHCPv6 client is enabled.
*
* @param[in] hContext - Handle to the context.
*
* @return The enabled status of the DHCPv6 client.
* @retval TRUE if the DHCPv6 client is enabled.
* @retval FALSE if the DHCPv6 client is disabled.
*
*/
BOOL
CosaDmlDhcpv6cGetEnabled
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Append an interface to the DHCPv6 configuration.
*
* This function adds a network interface to the DHCPv6 client/server configuration.
*
* @param[in] Inf_name - Pointer to a null-terminated string containing the interface name.
*
* @return The status of the operation.
* @retval 0 if successful.
* @retval Non-zero error code if the operation fails.
*
*/
INT
append_interface
	(
		char* 						Inf_name
	);

/**
* @brief Remove an interface from the DHCPv6 configuration.
*
* This function removes a network interface from the DHCPv6 client/server configuration.
*
* @param[in] Inf_name - Pointer to a null-terminated string containing the interface name.
*
* @return The status of the operation.
* @retval 0 if successful.
* @retval Non-zero error code if the operation fails.
*
*/
INT
remove_interface
	(
		char* 						Inf_name
	);


#if defined (RDKB_EXTENDER_ENABLED) || defined (WAN_FAILOVER_SUPPORTED)
/**
* @brief Assign an IPv6 address to an interface.
*
* This function assigns an IPv6 address to a specified network interface.
*
* @param[in] ifname - Pointer to a null-terminated string containing the interface name.
* @param[in] ipv6Addr - Pointer to a null-terminated string containing the IPv6 address to assign.
*
* @return None.
*
*/
void AssignIpv6Addr(char* ifname , char* ipv6Addr);

/**
* @brief Delete an IPv6 address from an interface.
*
* This function removes an IPv6 address from a specified network interface.
*
* @param[in] ifname - Pointer to a null-terminated string containing the interface name.
* @param[in] ipv6Addr - Pointer to a null-terminated string containing the IPv6 address to delete.
*
* @return None.
*
*/
void DelIpv6Addr(char* ifname , char* ipv6Addr);

/**
* @brief Set an IPv6 route.
*
* This function adds an IPv6 route to the routing table with a specified metric.
*
* @param[in] ifname - Pointer to a null-terminated string containing the interface name.
* @param[in] route_addr - Pointer to a null-terminated string containing the route destination address.
* @param[in] metric_val - Metric value for the route.
*
* @return None.
*
*/
void SetV6Route(char* ifname , char* route_addr,int metric_val);

/**
* @brief Unset an IPv6 route.
*
* This function removes an IPv6 route from the routing table.
*
* @param[in] ifname - Pointer to a null-terminated string containing the interface name.
* @param[in] route_addr - Pointer to a null-terminated string containing the route destination address.
* @param[in] metric_val - Metric value for the route.
*
* @return None.
*
*/
void UnSetV6Route(char* ifname , char* route_addr,int metric_val);

/**
* @brief Set an IPv6 route in a specific routing table.
*
* This function adds an IPv6 route to a specific routing table with a specified metric.
*
* @param[in] ifname - Pointer to a null-terminated string containing the interface name.
* @param[in] route_addr - Pointer to a null-terminated string containing the route destination address.
* @param[in] metric_val - Metric value for the route.
* @param[in] table_num - Routing table number.
*
* @return None.
*
*/
void SetV6RouteTable(char* ifname , char* route_addr,int metric_val,int table_num);

/**
* @brief Unset an IPv6 route from a specific routing table.
*
* This function removes an IPv6 route from a specific routing table.
*
* @param[in] ifname - Pointer to a null-terminated string containing the interface name.
* @param[in] route_addr - Pointer to a null-terminated string containing the route destination address.
* @param[in] metric_val - Metric value for the route.
* @param[in] table_num - Routing table number.
*
* @return None.
*
*/
void UnSetV6RouteFromTable(char* ifname , char* route_addr,int metric_val, int table_num);

/**
* @brief Get the device operating mode.
*
* This function retrieves the current device operating mode.
*
* @return The device mode value.
* @retval 0 The default mode.
* @retval >0 The integer value representing a specific device mode.
*
*/
int Get_Device_Mode();

#endif

#if defined (RDKB_EXTENDER_ENABLED)
/**
* @brief Configure IPv6 routing based on device mode.
*
* This function configures IPv6 routing for the device based on its operating mode.
*
* @param[in] DeviceMode - The device operating mode value.
*
* @return None.
*
*/
void configureIpv6Route(uint32_t DeviceMode);
#endif


#endif

