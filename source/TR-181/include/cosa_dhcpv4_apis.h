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

    module: dml_api_dhcp.h

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


#ifndef  _DML_API_DHCP_
#define  _DML_API_DHCP_

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define  COSA_DML_DHCP_MAX_ENTRIES                  4
#define  COSA_DML_DHCP_MAX_RESERVED_ADDRESSES       8
#define  COSA_DML_DHCP_MAX_OPT_ENTRIES              8

#define  COSA_DML_DHCP_LEASES_FILE                  "/tmp/dnsmasq.leases"
#define  COSA_DML_DHCP_OPTIONS_FILE                 "/tmp/dnsmasq.options"

#define  COSA_DML_DHCPV4_CLIENT_IFNAME              CFG_TR181_DHCPv4_CLIENT_IfName

#define _DEBUG_DHCPV4
#ifdef _DEBUG_DHCPV4
    #define ULOGF ulogf
#else
    #define ULOGF
#endif

typedef  enum
_COSA_DML_DHCP_STATUS
{
    COSA_DML_DHCP_STATUS_Disabled               = 1,
    COSA_DML_DHCP_STATUS_Enabled,
    COSA_DML_DHCP_STATUS_Error_Misconfigured,
    COSA_DML_DHCP_STATUS_Error
}
COSA_DML_DHCP_STATUS, *PCOSA_DML_DHCP_STATUS;


typedef  enum
_COSA_DML_DHCPC_STATUS
{
    COSA_DML_DHCPC_STATUS_Init                  = 1,
    COSA_DML_DHCPC_STATUS_Selecting,
    COSA_DML_DHCPC_STATUS_Requesting,
    COSA_DML_DHCPC_STATUS_Rebinding,
    COSA_DML_DHCPC_STATUS_Bound,
    COSA_DML_DHCPC_STATUS_Renewing
}
COSA_DML_DHCPC_STATUS, *PCOSA_DML_DHCPC_STATUS;


typedef  enum
_COSA_DML_DHCPS_VID_MODE
{
    COSA_DML_DHCPS_VID_MODE_Exact               = 1,
    COSA_DML_DHCPS_VID_MODE_Prefix,
    COSA_DML_DHCPS_VID_MODE_Suffix,
    COSA_DML_DHCPS_VID_MODE_Substring
}
COSA_DML_DHCPS_VID_MODE, *PCOSA_DML_DHCPS_VID_MODE;


typedef  struct
_COSA_DML_DHCP_OPT
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    UCHAR                           Tag;
    UCHAR                           Value[255];
}
COSA_DML_DHCP_OPT,  *PCOSA_DML_DHCP_OPT;


/*
 *  DHCP Client
 */
typedef  struct
_COSA_DML_DHCPC_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH]; /* IP interface name */
    BOOLEAN                         PassthroughEnable;
    char                            PassthroughDHCPPool[64];            /* DHCP server pool alias */
    char                            X_CISCO_COM_BootFileName[256];
}
COSA_DML_DHCPC_CFG,  *PCOSA_DML_DHCPC_CFG;


typedef  struct
_COSA_DML_DHCPC_INFO
{
    COSA_DML_DHCP_STATUS            Status;
    COSA_DML_DHCPC_STATUS           DHCPStatus;
    ANSC_IPV4_ADDRESS               IPAddress;
    ANSC_IPV4_ADDRESS               SubnetMask;
    ULONG                           NumIPRouters;
    ANSC_IPV4_ADDRESS               IPRouters[COSA_DML_DHCP_MAX_ENTRIES];
    ULONG                           NumDnsServers;
    ANSC_IPV4_ADDRESS               DNSServers[COSA_DML_DHCP_MAX_ENTRIES];
    int                             LeaseTimeRemaining;
    ANSC_IPV4_ADDRESS               DHCPServer;
}
COSA_DML_DHCPC_INFO,  *PCOSA_DML_DHCPC_INFO;


typedef  struct
_COSA_DML_DHCPC_FULL
{
    COSA_DML_DHCPC_CFG              Cfg;
    COSA_DML_DHCPC_INFO             Info;
}
COSA_DML_DHCPC_FULL, *PCOSA_DML_DHCPC_FULL;


typedef  struct
_COSA_DML_DHCPC_REQ_OPT
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           Order;
    UCHAR                           Tag;
    UCHAR                           Value[255];
}
COSA_DML_DHCPC_REQ_OPT,  *PCOSA_DML_DHCPC_REQ_OPT;


/*
 *  DHCP Server
 */
struct
_COSA_DML_DHCPS_CFG
{
    BOOLEAN                         bEnabled;
};
typedef struct _COSA_DML_DHCPS_CFG COSA_DML_DHCPS_CFG,  *PCOSA_DML_DHCPS_CFG;


struct
_COSA_DML_DHCPS_POOL_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           Order;
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH];         /* IP interface name */
    char                            VendorClassID[256];
    BOOLEAN                         VendorClassIDExclude;
    COSA_DML_DHCPS_VID_MODE         VendorClassIDMode;
    UCHAR                           ClientID[256];
    BOOLEAN                         ClientIDExclude;
    UCHAR                           UserClassID[256];
    BOOLEAN                         UserClassIDExclude;
    UCHAR                           Chaddr[6];
    UCHAR                           ChaddrMask[6];
    BOOLEAN                         ChaddrExclude;
    BOOLEAN                         DNSServersEnabled;
    ANSC_IPV4_ADDRESS               MinAddress;
    char                            MinAddressUpdateSource[16];
    ANSC_IPV4_ADDRESS               MaxAddress;
    char                            MaxAddressUpdateSource[16];
    ANSC_IPV4_ADDRESS               ReservedAddresses[COSA_DML_DHCP_MAX_RESERVED_ADDRESSES];
    ANSC_IPV4_ADDRESS               SubnetMask;
    ANSC_IPV4_ADDRESS               DNSServers[COSA_DML_DHCP_MAX_ENTRIES];
    char                            DomainName[64];
    ANSC_IPV4_ADDRESS               IPRouters[COSA_DML_DHCP_MAX_ENTRIES];
    int                             LeaseTime;
    int                             X_CISCO_COM_TimeOffset;
    BOOLEAN                         bAllowDelete;
};
typedef struct _COSA_DML_DHCPS_POOL_CFG COSA_DML_DHCPS_POOL_CFG,  *PCOSA_DML_DHCPS_POOL_CFG;


struct
_COSA_DML_DHCPS_POOL_INFO
{
    COSA_DML_DHCP_STATUS            Status;
    ULONG                           X_CISCO_COM_Connected_Device_Number;
};
typedef  struct _COSA_DML_DHCPS_POOL_INFO COSA_DML_DHCPS_POOL_INFO,  *PCOSA_DML_DHCPS_POOL_INFO;


struct
_COSA_DML_DHCPS_POOL_FULL
{
    COSA_DML_DHCPS_POOL_CFG    Cfg;
    COSA_DML_DHCPS_POOL_INFO   Info;
};
typedef  struct _COSA_DML_DHCPS_POOL_FULL COSA_DML_DHCPS_POOL_FULL, *PCOSA_DML_DHCPS_POOL_FULL;

struct
_COSA_DML_DHCPS_POOL_FULL_LINK_OBJ
{
    SINGLE_LINK_ENTRY           Linkage;
    COSA_DML_DHCPS_POOL_FULL    SPool;
    SLIST_HEADER                StaticAddressList;
    SLIST_HEADER                OptionList;
};
typedef  struct _COSA_DML_DHCPS_POOL_FULL_LINK_OBJ COSA_DML_DHCPS_POOL_FULL_LINK_OBJ, *PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ;

struct
_COSA_DML_DHCPS_X_CISCO_COM_SADDR
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    UCHAR                           Chaddr[6];
    ANSC_IPV4_ADDRESS               Yiaddr;
    char                            DeviceName[COSA_DML_ALIAS_NAME_LENGTH];
};
typedef  struct _COSA_DML_DHCPS_X_CISCO_COM_SADDR COSA_DML_DHCPS_X_CISCO_COM_SADDR,  *PCOSA_DML_DHCPS_X_CISCO_COM_SADDR;

// Need to define link object for dynamic table
struct
_COSA_DML_DHCPS_SADDR
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    UCHAR                           Chaddr[6];
    ANSC_IPV4_ADDRESS               Yiaddr;
    char                            DeviceName[COSA_DML_ALIAS_NAME_LENGTH];
    char                            Comment[256];
    BOOLEAN                         ActiveFlag;
};
typedef  struct _COSA_DML_DHCPS_SADDR COSA_DML_DHCPS_SADDR,  *PCOSA_DML_DHCPS_SADDR;

struct
_COSA_DML_DHCPS_SADDR_LINK_OBJ
{
    SINGLE_LINK_ENTRY           Linkage;
    COSA_DML_DHCPS_SADDR        SPoolSADDR;
};
typedef  struct _COSA_DML_DHCPS_SADDR_LINK_OBJ COSA_DML_DHCPS_SADDR_LINK_OBJ,  *PCOSA_DML_DHCPS_SADDR_LINK_OBJ;

// Need to define link object for dynamic table
// Value is changed to 512 bytes to hold 256 bytes of hexdecimal string
struct
_COSA_DML_DHCPSV4_OPTION
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    ULONG                           Tag;
    UCHAR                           Value[512];
    BOOLEAN                         bEnabled;
};
typedef  struct _COSA_DML_DHCPSV4_OPTION COSA_DML_DHCPSV4_OPTION,  *PCOSA_DML_DHCPSV4_OPTION;

struct
_COSA_DML_DHCPSV4_OPTION_LINK_OBJ
{
    SINGLE_LINK_ENTRY           Linkage;
    COSA_DML_DHCPSV4_OPTION     SPoolOption;
};
typedef  struct _COSA_DML_DHCPSV4_OPTION_LINK_OBJ COSA_DML_DHCPSV4_OPTION_LINK_OBJ,  *PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ;

struct
_COSA_DML_DHCPSV4_CLIENT
{
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    UCHAR                           Chaddr[18];
    BOOLEAN                         Active;
    UCHAR                           X_CISCO_COM_HostName[64];
    UCHAR                           ClassId[256];
    UCHAR                           X_CISCO_COM_Interface[256];
    UCHAR                           X_CISCO_COM_Comment[256];
};
typedef  struct _COSA_DML_DHCPSV4_CLIENT COSA_DML_DHCPSV4_CLIENT,  *PCOSA_DML_DHCPSV4_CLIENT;

struct
_COSA_DML_DHCPSV4_CLIENT_IPADDRESS
{
    ULONG                           IPAddress;
    UCHAR                           LeaseTimeRemaining[32];
    UCHAR                           X_CISCO_COM_LeaseTimeCreation[32];
    UCHAR                           X_CISCO_COM_LeaseTimeDuration[32];
};
typedef  struct _COSA_DML_DHCPSV4_CLIENT_IPADDRESS COSA_DML_DHCPSV4_CLIENT_IPADDRESS,  *PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS;

struct
_COSA_DML_DHCPSV4_CLIENT_OPTION
{
    ULONG                           Tag;
    UCHAR                           Value[255];
};
typedef  struct _COSA_DML_DHCPSV4_CLIENT_OPTION COSA_DML_DHCPSV4_CLIENT_OPTION,  *PCOSA_DML_DHCPSV4_CLIENT_OPTION;

struct
_COSA_DML_DHCPSV4_CLIENTCONTENT
{
    PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS       pIPAddress;
    PCOSA_DML_DHCPSV4_CLIENT_OPTION          pOption;
    ULONG                                    NumberofIPAddress;
    ULONG                                    NumberofOption;
};
typedef  struct _COSA_DML_DHCPSV4_CLIENTCONTENT COSA_DML_DHCPSV4_CLIENTCONTENT,  *PCOSA_DML_DHCPSV4_CLIENTCONTENT;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the DHCPv4 subsystem.
*
* This function initializes the DHCPv4 client and server data structures.
*
* @param[in] hDml - Handle to the DML context.
* @param[in,out] phContext - Pointer to handle for returning context information.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDhcpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/*
 *  DHCP Client
 */

/**
* @brief Get the total number of DHCPv4 client entries.
*
* This function retrieves the count of configured DHCPv4 client instances in the system.
*
* @param[in] hContext - Handle to the context.
*
* @return The number of DHCPv4 client entries.
*
*/
ULONG
CosaDmlDhcpcGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get a DHCPv4 client entry by index.
*
* This function retrieves the complete configuration and information for a DHCPv4 client
* entry at the specified index position in the client list. The usual process is the caller
* gets the total number of entries, then iterate through those by calling this API.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the DHCPv4 client entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPC_FULL structure where the client
*                      configuration and information will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPC_FULL        pEntry
    );

/**
* @brief Set the instance number and alias for a DHCPv4 client entry.
*
* This function updates the instance number and alias for a DHCPv4 client entry at
* the specified index to the syscfg DB.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the client entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the client.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new DHCPv4 client entry.
*
* This function creates a new DHCPv4 client instance with the specified configuration.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPC_FULL structure containing the client
*                         configuration to be added. Caller fills in pEntry->Cfg, except Alias field.
*                         \n Upon return, callee fills pEntry->Cfg.Alias field and as many as possible fields in pEntry->Info.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if memory allocation fails or client addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_FULL        pEntry
    );

/**
* @brief Delete a DHCPv4 client entry.
*
* This function removes a DHCPv4 client instance identified by its instance number from the system.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the client to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a DHCPv4 client.
*
* This function updates the configuration parameters of an existing DHCPv4 client identified by its instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] pCfg - Pointer to a COSA_DML_DHCPC_CFG structure containing the new configuration, even Alias field can be changed.
*                   \n The InstanceNumber field identifies which client to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client is not found or configuration update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_CFG         pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration of a DHCPv4 client.
*
* This function retrieves the current configuration parameters of a DHCPv4 client identified by its instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pCfg - Pointer to a COSA_DML_DHCPC_CFG structure.
*                       \n [in] The InstanceNumber field identifies which client to query.
*                       \n [out] The structure will be filled with the client configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_CFG         pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get runtime information of a DHCPv4 client.
*
* This function retrieves the runtime status information of a DHCPv4 client identified
* by its instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the client to query.
* @param[out] pInfo - Pointer to a COSA_DML_DHCPC_INFO structure where the runtime
*                     information will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPC_INFO        pInfo
    );

/**
* @brief Renew the DHCP lease for a DHCPv4 client.
*
* This function triggers a DHCP lease renewal for the specified DHCPv4 client instance.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the client to renew.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the client is not found or renew operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcRenew
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/*
 *  DHCP Client Send/Req Option
 *
 *  The options are managed on top of a DHCP client,
 *  which is identified through pClientAlias
 */

/**
* @brief Get the number of sent options for a DHCPv4 client.
*
* This function retrieves the count of DHCP options configured to be sent by a specific DHCPv4 client instance.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
*
* @return The number of sent option entries for the specified client.
*
*/
ULONG
CosaDmlDhcpcGetNumberOfSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    );

/**
* @brief Get a sent option entry by instance number.
*
* This function retrieves a DHCP sent option entry for a specific DHCPv4 client
* identified by the option's instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCP_OPT structure.
*                         \n [in] The InstanceNumber field identifies which option to query.
*                         \n [out] The structure will be filled with the option configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcGetSentOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    );

/**
* @brief Get a sent option entry by index.
*
* This function retrieves a DHCP sent option entry for a specific DHCPv4 client
* at the specified index position.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in] ulIndex - Zero-based index of the sent option entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCP_OPT structure where the option
*                      configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcGetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCP_OPT          pEntry
    );

/**
* @brief Set the instance number and alias for a sent option entry.
*
* This function updates the instance number and alias for a DHCP sent option entry
* associated with a specific DHCPv4 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in] ulIndex - Zero-based index of the sent option entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the option.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcSetSentOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new sent option entry to a DHCPv4 client.
*
* This function adds a new DHCP option to be sent by the specified DHCPv4 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCP_OPT structure containing the option
*                         configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcAddSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    );

/**
* @brief Delete a sent option entry from a DHCPv4 client.
*
* This function removes a DHCP sent option from the specified DHCPv4 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in] ulInstanceNumber - Instance number of the sent option to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcDelSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a sent option.
*
* This function updates the configuration of a DHCP sent option for the specified DHCPv4 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in] pEntry - Pointer to a COSA_DML_DHCP_OPT structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcSetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry        /* Identified by InstanceNumber */
    );

/**
* @brief Get the number of requested options for a DHCPv4 client.
*
* This function retrieves the count of DHCP options configured to be requested by a specific
* DHCPv4 client instance.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
*
* @return The number of requested option entries for the specified client.
*
*/
ULONG
CosaDmlDhcpcGetNumberOfReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    );

/**
* @brief Get a requested option entry by instance number.
*
* This function retrieves a DHCP requested option entry for a specific DHCPv4 client
* identified by the option's instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPC_REQ_OPT structure.
*                         \n [in] The InstanceNumber field identifies which option to query.
*                         \n [out] The structure will be filled with the option configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcGetReqOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    );

/**
* @brief Get a requested option entry by index.
*
* This function retrieves a DHCP requested option entry for a specific DHCPv4 client
* at the specified index position.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in] ulIndex - Zero-based index of the requested option entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPC_REQ_OPT structure where the option
*                      configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcGetReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    );

/**
* @brief Set the instance number and alias for a requested option entry.
*
* This function updates the instance number and alias for a DHCP requested option entry
* associated with a specific DHCPv4 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in] ulIndex - Zero-based index of the requested option entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the option.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcSetReqOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new requested option entry to a DHCPv4 client.
*
* This function adds a new DHCP option to be requested by the specified DHCPv4 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPC_REQ_OPT structure containing the option
*                         configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcAddReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    );

/**
* @brief Delete a requested option entry from a DHCPv4 client.
*
* This function removes a DHCP requested option from the specified DHCPv4 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in] ulInstanceNumber - Instance number of the requested option to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcDelReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a requested option.
*
* This function updates the configuration of a DHCP requested option for the specified DHCPv4 client.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the DHCPv4 client.
* @param[in] pEntry - Pointer to a COSA_DML_DHCPC_REQ_OPT structure containing the new configuration.
*                     \n The InstanceNumber field identifies which option to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the option is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpcSetReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry        /* Identified by InstanceNumber */
    );

/*
 *  DHCP Server
 */

/**
* @brief Enable or disable the DHCPv4 server.
*
* This function enables or disables the DHCPv4 server globally.
*
* @param[in] hContext - Handle to the context.
* @param[in] bEnable - Boolean flag to enable or disable the server.
*                      \n TRUE: Enable the DHCPv4 server.
*                      \n FALSE: Disable the DHCPv4 server.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsEnable
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bEnable
    );

/**
* @brief Get the current state of the DHCPv4 server.
*
* This function retrieves the current enabled/disabled state of the DHCPv4 server.
*
* @param[in] hContext - Handle to the context.
*
* @return Current state of the DHCPv4 server.
* @retval TRUE if the DHCPv4 server is enabled.
* @retval FALSE if the DHCPv4 server is disabled.
*
*/
BOOLEAN
CosaDmlDhcpsGetState
    (
        ANSC_HANDLE                 hContext
    );


/*
 *  DHCP Server X_COM_CISCO_StaticAddress
 *
 *  The static addresses are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */

/**
* @brief Get the number of Cisco static address entries.
*
* This function retrieves the count of Cisco-specific static address entries configured
* across all DHCP server pools.
*
* @param[in] hContext - Handle to the context.
*
* @return The number of Cisco static address entries.
*
*/
ULONG
CosaDmlDhcpsGetNumberOfX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get a Cisco static address entry by index.
*
* This function retrieves a Cisco-specific static address entry at the specified index position.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the static address entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure where the static address
*                      configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

/**
* @brief Get a Cisco static address entry by instance number.
*
* This function retrieves a Cisco-specific static address entry identified by instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the static address configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetX_COM_CISCO_SaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

/**
* @brief Get a Cisco static address entry by instance number (client variant).
*
* This function retrieves a Cisco-specific static address entry identified by instance number
* from the client context.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the static address configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcGetX_COM_CISCO_SaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

/**
* @brief Set values for a Cisco static address entry.
*
* This function updates the instance number and alias for a Cisco-specific static address entry.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the static address entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsSetX_COM_CISCO_SaddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new Cisco static address entry.
*
* This function adds a new Cisco-specific static address entry to the DHCP server configuration.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure containing the static address
*                         configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsAddX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

/**
* @brief Delete a Cisco static address entry.
*
* This function removes a Cisco-specific static address entry identified by instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the static address to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsDelX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a Cisco static address entry.
*
* This function updates the configuration of a Cisco-specific static address entry.
*
* @param[in] hContext - Handle to the context.
* @param[in] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure containing the new configuration.
*                     \n The InstanceNumber field identifies which entry to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsSetX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry        /* Identified by InstanceNumber */
    );

/*
 *  DHCP Server Pool
 */

/**
* @brief Get the number of DHCP server pools.
*
* This function retrieves the total count of configured DHCP server pools.
*
* @param[in] hContext - Handle to the context.
*
* @return The number of DHCP server pool entries.
*
*/
ULONG
CosaDmlDhcpsGetNumberOfPools
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get a DHCP server pool entry by index.
*
* This function retrieves the full configuration and information for a DHCP server pool
* at the specified index position.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the pool entry to retrieve.
*                      \n Valid range: 0 to (number of pools - 1).
* @param[out] pEntry - Pointer to a COSA_DML_DHCPS_POOL_FULL structure where the pool
*                      configuration and information will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_POOL_FULL   pEntry
    );

/**
* @brief Set values for a DHCP server pool entry.
*
* This function updates the instance number and alias for a DHCP server pool.
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
CosaDmlDhcpsSetPoolValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new DHCP server pool.
*
* This function creates a new DHCP server pool with the specified configuration.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPS_POOL_FULL structure containing the
*                         pool configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsAddPool
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_FULL   pEntry
    );

/**
* @brief Delete a DHCP server pool.
*
* This function removes a DHCP server pool identified by instance number.
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
CosaDmlDhcpsDelPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a DHCP server pool.
*
* This function updates the configuration parameters of a DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] pCfg - Pointer to a COSA_DML_DHCPS_POOL_CFG structure containing the new configuration.
*                   \n The InstanceNumber field identifies which pool to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsSetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_CFG    pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration of a DHCP server pool.
*
* This function retrieves the current configuration parameters of a DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pCfg - Pointer to a COSA_DML_DHCPS_POOL_CFG structure.
*                       \n [in] The InstanceNumber field identifies which pool to query.
*                       \n [out] The structure will be filled with the pool configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_CFG         pCfg        /* Identified by InstanceNumber */
    );

/**
* @brief Get runtime information of a DHCP server pool.
*
* This function retrieves runtime status and statistics for a DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the pool to query.
* @param[out] pInfo - Pointer to a COSA_DML_DHCPS_POOL_INFO structure where the pool
*                     runtime information will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the pool is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetPoolInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPS_POOL_INFO   pInfo
    );

/**
* @brief Set the IPv4 enable/disable status of the DHCP server.
*
* This function enables or disables IPv4 DHCP server functionality.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsSetIpv4Status
    (
        void
    );

/*
 *  DHCP Server Pool Static Address
 *
 *  The static addresses are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */

/**
* @brief Get the number of static addresses in a DHCP server pool.
*
* This function retrieves the count of static address reservations configured for
* a specific DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
*
* @return The number of static address entries in the pool.
*
*/
ULONG
CosaDmlDhcpsGetNumberOfSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    );

/**
* @brief Get a static address entry by index.
*
* This function retrieves a static address reservation entry at the specified index
* within a DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in] ulIndex - Zero-based index of the static address entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure where the static address
*                      configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

/**
* @brief Get a static address entry by instance number (server variant).
*
* This function retrieves a static address reservation entry identified by instance number
* from the server context.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the static address entry.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the static address configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetSaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

/**
* @brief Get a static address entry by instance number (client variant).
*
* This function retrieves a static address reservation entry identified by instance number
* from the client context.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulClientInstanceNumber - Instance number of the static address entry.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the static address configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpcGetSaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

/**
* @brief Set values for a static address entry.
*
* This function updates the instance number and alias for a static address reservation.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in] ulIndex - Zero-based index of the static address entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
* @param[in] pAlias - Pointer to a null-terminated string containing the new alias name.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsSetSaddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new static address entry to a pool.
*
* This function adds a new static address reservation to a DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure containing the
*                         static address configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsAddSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

/**
* @brief Delete a static address entry from a pool.
*
* This function removes a static address reservation from a DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in] ulInstanceNumber - Instance number of the static address to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsDelSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a static address entry.
*
* This function updates the configuration of a static address reservation in a pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in] pEntry - Pointer to a COSA_DML_DHCPS_SADDR structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsSetSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry        /* Identified by InstanceNumber */
    );


/*
 *  DHCP Server Pool Option
 *
 *  The options are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */

/**
* @brief Get the number of DHCP options in a server pool.
*
* This function retrieves the count of DHCP options configured for a specific DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
*
* @return The number of DHCP option entries in the pool.
*
*/
ULONG
CosaDmlDhcpsGetNumberOfOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    );

/**
* @brief Get a DHCP option entry by index.
*
* This function retrieves a DHCP option entry at the specified index within a server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in] ulIndex - Zero-based index of the option entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DHCPSV4_OPTION structure where the option
*                      configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    );

/**
* @brief Get a DHCP option entry by instance number.
*
* This function retrieves a DHCP option entry identified by instance number.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPSV4_OPTION structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the option configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    );

/**
* @brief Set values for a DHCP option entry.
*
* This function updates the instance number and alias for a DHCP option.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
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
CosaDmlDhcpsSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a new DHCP option to a pool.
*
* This function adds a new DHCP option entry to a DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in,out] pEntry - Pointer to a COSA_DML_DHCPSV4_OPTION structure containing the
*                         option configuration to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    );

/**
* @brief Delete a DHCP option from a pool.
*
* This function removes a DHCP option entry from a DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in] ulInstanceNumber - Instance number of the option to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a DHCP option.
*
* This function updates the configuration of a DHCP option in a server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[in] pEntry - Pointer to a COSA_DML_DHCPSV4_OPTION structure containing the new configuration.
*                     \n The InstanceNumber field identifies which entry to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry        /* Identified by InstanceNumber */
    );

/*
    This is for dhcpv4.server.pool.{i}.client.
*/

/**
* @brief Get client information from a DHCP server pool.
*
* This function retrieves the list of clients currently associated with a DHCP server pool.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulPoolInstanceNumber - Instance number of the DHCP server pool.
* @param[out] ppEntry - Pointer to a COSA_DML_DHCPSV4_CLIENT pointer where the client list
*                       will be returned.
* @param[out] ppClientContent - Pointer to a COSA_DML_DHCPSV4_CLIENTCONTENT pointer where the
*                               client content information will be returned.
* @param[out] pSize - Pointer to a ULONG where the number of client entries will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetClient
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_CLIENT   *ppEntry,
        PCOSA_DML_DHCPSV4_CLIENTCONTENT *ppClientContent,
        PULONG                      pSize
    );

/**
* @brief Ping a DHCP client to check connectivity.
*
* This function sends an ICMP ping to a DHCP client to verify network connectivity.
*
* @param[in] pDhcpsClient - Pointer to a COSA_DML_DHCPSV4_CLIENT_IPADDRESS structure containing
*                           the client's IP address information.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the ping is successful.
* @retval ANSC_STATUS_FAILURE if the ping fails or client is unreachable.
*
*/
ANSC_STATUS
CosaDmlDhcpsPing
    (
        PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS    pDhcpsClient
    );

/**
* @brief Get the lease time duration for a DHCP client.
*
* This function retrieves the remaining lease time duration for a DHCP client.
*
* @param[in,out] pDhcpsClient - Pointer to a COSA_DML_DHCPSV4_CLIENT_IPADDRESS structure.
*                               \n [in] Contains the client's IP address information.
*                               \n [out] The structure will be updated with lease time information.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails or client is not found.
*
*/
ANSC_STATUS
CosaDmlDhcpsGetLeaseTimeDuration
    (
        PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS    pDhcpsClient
    );

/**
* @brief Update a JSON parameter with legacy method.
*
* This function updates a JSON configuration parameter using the legacy update method
* without source tracking or timestamp information.
*
* @param[in] pKey - Pointer to a null-terminated string containing the parameter key name.
* @param[in] PartnerId - Pointer to a null-terminated string containing the partner identifier.
* @param[in] pValue - Pointer to a null-terminated string containing the new parameter value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
extern ANSC_STATUS UpdateJsonParamLegacy
	(
		char*                       pKey,
		char*			PartnerId,
		char*			pValue
    );

/**
* @brief Update a JSON parameter with full tracking.
*
* This function updates a JSON configuration parameter with source tracking and timestamp information.
*
* @param[in] pKey - Pointer to a null-terminated string containing the parameter key name.
* @param[in] PartnerId - Pointer to a null-terminated string containing the partner identifier.
* @param[in] pValue - Pointer to a null-terminated string containing the new parameter value.
* @param[in] pSource - Pointer to a null-terminated string containing the source of the update.
* @param[in] pCurrentTime - Pointer to a null-terminated string containing the timestamp of the update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
extern ANSC_STATUS UpdateJsonParam
	(
		char*           pKey,
		char*			PartnerId,
		char*			pValue,
		char*                   pSource,
		char*			pCurrentTime
    );

/**
* @brief Fill the current partner ID.
*
* This function retrieves and fills the current partner identifier into the provided buffer.
*
* @param[out] pValue - Pointer to a buffer where the partner ID will be written.
* @param[in,out] pulSize - Pointer to a ULONG containing the buffer size.
*                          \n [in] Size of the buffer in bytes.
*                          \n [out] Actual size of the partner ID string written.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
extern ANSC_STATUS fillCurrentPartnerId
        (
                char*                       pValue,
        PULONG                      pulSize
    );
#endif

