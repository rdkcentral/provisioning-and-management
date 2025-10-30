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

    module:	cosa_apis_util.h

        This is base file for all parameters H files.

    ---------------------------------------------------------------

    description:

        This file contains all utility functions for COSA DML API development.

    ---------------------------------------------------------------

    environment:

        COSA independent

    ---------------------------------------------------------------

    author:

        Roger Hu
        leichen2@cisco.com

    ---------------------------------------------------------------

    revision:

        01/30/2011    initial revision.
        06/15/2012    add IPv4 address utils

**********************************************************************/


#ifndef  _COSA_APIS_UTIL_H
#define  _COSA_APIS_UTIL_H

#include "cosa_dml_api_common.h"

typedef struct StaticRoute
{
    char         name[64];
    char         dest_lan_ip[16];
    char         netmask[16];
    char         gateway[16];
    int          metric;
    char         dest_intf[10];
    char         origin[16];
}StaticRoute;

#ifndef _BUILD_ANDROID
enum
{
    IPV6_ADDR_SCOPE_UNKNOWN,
    IPV6_ADDR_SCOPE_GLOBAL,
    IPV6_ADDR_SCOPE_LINKLOCAL,
    IPV6_ADDR_SCOPE_SITELOCAL,
    IPV6_ADDR_SCOPE_COMPATv4,
    IPV6_ADDR_SCOPE_LOOPBACK
};
#endif

typedef struct
{
    int    scope;
    char   v6addr[64];
    char   v6pre[64];
}ipv6_addr_info_t;

typedef  enum
_COSA_DML_LINK_TYPE
{
    COSA_DML_LINK_TYPE_Eth       = 1,
    COSA_DML_LINK_TYPE_EthLink,
    COSA_DML_LINK_TYPE_EthVlan,
    COSA_DML_LINK_TYPE_Usb,
    COSA_DML_LINK_TYPE_Hpna,
    COSA_DML_LINK_TYPE_Moca,
    COSA_DML_LINK_TYPE_HomePlug,
    COSA_DML_LINK_TYPE_Upa,
    COSA_DML_LINK_TYPE_WiFiSsid,
    COSA_DML_LINK_TYPE_Bridge,
    COSA_DML_LINK_TYPE_PPP,
    COSA_DML_LINK_TYPE_DOCSIS,
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE) || defined(_RDKB_GLOBAL_PRODUCT_REQ_)
    COSA_DML_LINK_TYPE_VlanLink,
#endif
    COSA_DML_LINK_TYPE_LAST
}
COSA_DML_LINK_TYPE, *PCOSA_DML_LINK_TYPE;

#define COSA_DML_LINK_TYPE_TOTAL                    COSA_DML_LINK_TYPE_LAST

typedef struct
LINKTYPE_MAP
{
    char*                           LinkTypePath;
    char*                           LinkTypeStr;
    COSA_DML_LINK_TYPE              LinkType;
}
LINKTYPE_MAP_T;

/**
 * @brief Get the IPv4 address of a network device.
 *
 * This function retrieves the IPv4 address assigned to the specified network device.
 *
 * @param[in] netdev - Pointer to the network device name.
 *
 * @return The IPv4 address in host byte order.
 * @retval uint32_t The IPv4 address on success.
 * @retval 0 if the operation fails.
 *
 */
uint32_t
CosaUtilGetIfAddr
    (
        char*       netdev
    );

/**
 * @brief Get the lower layers path from a table by keyword.
 *
 * This function retrieves the lower layers path from the specified table that matches the keyword.
 *
 * @param[in] pTableName - Pointer to the table name.
 * @param[in] pKeyword   - Pointer to the keyword to search for.
 *
 * @return Pointer to the lower layers path string.
 * @retval Pointer to the lower layers path on success.
 * @retval NULL if not found or operation fails.
 *
 */
PUCHAR
CosaUtilGetLowerLayers
    (
        PUCHAR                      pTableName,
        PUCHAR                      pKeyword
    );

/**
 * @brief Get the full path name of a parameter by keyword.
 *
 * This function retrieves the full path name of a parameter from the specified table that matches the keyword.
 *
 * @param[in] pTableName     - Pointer to the table name.
 * @param[in] pParameterName - Pointer to the parameter name.
 * @param[in] pKeyword       - Pointer to the keyword to search for.
 *
 * @return Pointer to the full path name string.
 * @retval PUCHAR Pointer to the full path name on success.
 * @retval NULL if not found or operation fails.
 *
 */
PUCHAR
CosaUtilGetFullPathNameByKeyword
    (
        PUCHAR                      pTableName,
        PUCHAR                      pParameterName,
        PUCHAR                      pKeyword
    );

/**
 * @brief Validate a channel for a specific radio.
 *
 * This function validates whether the specified channel is valid for the given radio.
 *
 * @param[in] uiRadio - The radio index.
 * @param[in] Channel - The channel number to validate.
 *
 * @return The status of the validation.
 * @retval ULONG Non-zero if the channel is valid.
 * @retval 0 if the channel is invalid.
 *
 */
ULONG
CosaUtilChannelValidate
    (
        UINT                        uiRadio,
        ULONG                       Channel
    );

/**
 * @brief Validate a channel against a channel list for a specific radio.
 *
 * This function validates whether the specified channel is valid for the given radio
 * by checking against the provided channel list.
 *
 * @param[in] uiRadio     - The radio index.
 * @param[in] Channel     - The channel number to validate.
 * @param[in] channelList - Pointer to the channel list string.
 *
 * @return The status of the validation.
 * @retval ULONG Non-zero if the channel is valid.
 * @retval 0 if the channel is invalid.
 *
 */
ULONG
CosaUtilChannelValidate2
    (
        UINT                       uiRadio,
        ULONG                      Channel,
        char                       *channelList
    );

/**
 * @brief Get interface statistics for a network interface.
 *
 * This function retrieves statistics for the specified network interface.
 *
 * @param[in] ifname  - Pointer to the interface name.
 * @param[out] pStats - Pointer to a structure where the statistics will be stored.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval -1 if the operation fails.
 *
 */
int CosaUtilGetIfStats(char * ifname, PCOSA_DML_IF_STATS  pStats);

/**
 * @brief Execute ioctl operation on a network interface.
 *
 * This function performs an ioctl operation on the specified network interface.
 *
 * @param[in] if_name - Pointer to the interface name.
 * @param[in] method  - Pointer to the ioctl method name.
 * @param[in] input - Pointer to the input/output data for the ioctl operation.
 *
 * @return The status of the operation.
 * @retval ULONG Status code of the ioctl operation.
 *
 */
ULONG CosaUtilIoctlXXX(char * if_name, char * method, void * input);

/**
 * @brief Convert a netmask string to a number.
 *
 * This function converts a netmask in dotted decimal notation to a numerical value.
 *
 * @param[in] netmask - Pointer to the netmask string in dotted decimal notation.
 *
 * @return The numerical value of the netmask.
 * @retval ULONG The netmask value.
 *
 */
ULONG NetmaskToNumber(char *netmask);

/**
 * @brief Get the static route table.
 *
 * This function retrieves the static route table from the system.
 *
 * @param[out] count     - Pointer to store the number of routes retrieved.
 * @param[out] out_route - Pointer to store the array of static routes.
 *                         \n The memory will be allocated and must be freed by caller.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ANSC_STATUS
CosaUtilGetStaticRouteTable
    (
        UINT                        *count,
        StaticRoute                 **out_route
    );

/**
 * @brief Get IPv6 address information for an interface.
 *
 * This function retrieves IPv6 address information for the specified network interface.
 *
 * @param[in] ifname   - Pointer to the interface name.
 * @param[out] pp_info - Pointer to store the array of IPv6 address information structures.
 *                       \n The memory will be allocated and must be freed by caller.
 * @param[out] num     - Pointer to store the number of IPv6 addresses retrieved.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval -1 if the operation fails.
 *
 */
int CosaUtilGetIpv6AddrInfo (char * ifname, ipv6_addr_info_t ** pp_info, int * num);

/**
 * @brief Safely copy a string with size boundary checking.
 *
 * This function copies a string from source to destination with size boundary checking to prevent buffer overflow.
 *
 * @param[out] dst      - Pointer to the destination buffer.
 * @param[in] src       - Pointer to the source string.
 * @param[in] dst_size  - The size of the destination buffer.
 *
 * @return Pointer to the destination buffer.
 * @retval char* Pointer to the destination buffer on success.
 *
 */
char *safe_strcpy (char *dst, char *src, size_t dst_size);

/**
 * @brief Check if two IPv6 addresses mismatch based on prefix length.
 *
 * This function checks if two IPv6 addresses mismatch based on the specified prefix length.
 *
 * @param[in] addr1    - Pointer to the first IPv6 address string.
 * @param[in] addr2    - Pointer to the second IPv6 address string.
 * @param[in] pref_len - The prefix length to compare.
 *
 * @return The mismatch status.
 * @retval 0 if addresses match.
 * @retval Non-zero if addresses mismatch.
 *
 */
int  __v6addr_mismatch(char * addr1, char * addr2, int pref_len);

/**
 * @brief Check if an IPv6 address mismatches an IPv6 prefix.
 *
 * This function checks if an IPv6 address mismatches the specified IPv6 prefix.
 *
 * @param[in] v6addr - Pointer to the IPv6 address string.
 * @param[in] v6pre  - Pointer to the IPv6 prefix string.
 *
 * @return The mismatch status.
 * @retval 0 if address matches the prefix.
 * @retval Non-zero if address mismatches the prefix.
 *
 */
int  __v6addr_mismatches_v6pre(char * v6addr,char * v6pre);

/**
 * @brief Check if two IPv6 prefixes mismatch.
 *
 * This function checks if two IPv6 prefixes mismatch.
 *
 * @param[in] v6pref1 - Pointer to the first IPv6 prefix string.
 * @param[in] v6pref2 - Pointer to the second IPv6 prefix string.
 *
 * @return The mismatch status.
 * @retval 0 if prefixes match.
 * @retval Non-zero if prefixes mismatch.
 *
 */
int  __v6pref_mismatches(char * v6pref1,char * v6pref2);

/**
 * @brief Check if two IPv6 addresses are equal.
 *
 * This function checks if two IPv6 addresses are equal.
 *
 * @param[in] p_addr1 - Pointer to the first IPv6 address string.
 * @param[in] p_addr2 - Pointer to the second IPv6 address string.
 *
 * @return The equality status.
 * @retval 1 if addresses are equal.
 * @retval 0 if addresses are not equal.
 *
 */
int CosaDmlV6AddrIsEqual(char * p_addr1, char * p_addr2);

/**
 * @brief Check if two IPv6 prefixes are equal.
 *
 * This function checks if two IPv6 prefixes are equal.
 *
 * @param[in] p_pref1 - Pointer to the first IPv6 prefix string.
 * @param[in] p_pref2 - Pointer to the second IPv6 prefix string.
 *
 * @return The equality status.
 * @retval 1 if prefixes are equal.
 * @retval 0 if prefixes are not equal.
 *
 */
int CosaDmlV6PrefIsEqual(char * p_pref1, char * p_pref2);

/**
 * @brief Write a value to a sysctl file.
 *
 * This function writes an integer value to the specified sysctl file.
 *
 * @param[in] filename - Pointer to the sysctl file path.
 * @param[in] value    - The integer value to write.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval -1 if the operation fails.
 *
 */
int _write_sysctl_file(const char *filename, int value);

/*utility functions for linktype and lowerlayer*/
/**
 * @brief Get the link type path string from link type enumeration.
 *
 * This function retrieves the link type path string corresponding to the given link type enumeration.
 *
 * @param[in] LinkType - The link type enumeration value.
 *
 * @return Pointer to the link type path string.
 * @retval char* Pointer to the link type path string on success.
 * @retval NULL if link type is not found.
 *
 */
char*               CosaUtilGetLinkTypePath(COSA_DML_LINK_TYPE LinkType);

/**
 * @brief Get the link type string from link type path.
 *
 * This function retrieves the link type string from the given link type path.
 *
 * @param[in] pLinkTypePath - Pointer to the link type path string.
 *
 * @return Pointer to the link type string.
 * @retval char* Pointer to the link type string on success.
 * @retval NULL if link type path is not found.
 *
 */
char*               CosaUtilGetStrFromLinkTypePath(char* pLinkTypePath);

/**
 * @brief Get the link type string from link type enumeration.
 *
 * This function retrieves the link type string corresponding to the given link type enumeration.
 *
 * @param[in] LinkType - The link type enumeration value.
 *
 * @return Pointer to the link type string.
 * @retval char* Pointer to the link type string on success.
 * @retval NULL if link type is not found.
 *
 */
char*               CosaUtilGetLinkTypeStr(COSA_DML_LINK_TYPE LinkType);

/**
 * @brief Get the link type enumeration from link type string.
 *
 * This function retrieves the link type enumeration corresponding to the given link type string.
 *
 * @param[in] pLinkTypeStr - Pointer to the link type string.
 *
 * @return The link type enumeration value.
 * @retval COSA_DML_LINK_TYPE The link type enumeration on success.
 * @retval COSA_DML_LINK_TYPE_LAST if link type string is not found.
 *
 */
COSA_DML_LINK_TYPE  CosaUtilGetLinkTypeFromStr(char* pLinkTypeStr);

/**
 * @brief Get the link type enumeration from link type path.
 *
 * This function retrieves the link type enumeration corresponding to the given link type path.
 *
 * @param[in] pLinkTypePath - Pointer to the link type path string.
 *
 * @return The link type enumeration value.
 * @retval COSA_DML_LINK_TYPE The link type enumeration on success.
 * @retval COSA_DML_LINK_TYPE_LAST if link type path is not found.
 *
 */
COSA_DML_LINK_TYPE  CosaUtilGetLinkTypeFromPath(char*pLinkTypePath);

/**
 * @brief Construct the full path of lower layers based on link type and instance number.
 *
 * This function constructs the full path of lower layers based on the specified link type and instance number.
 *
 * @param[in] LinkType         - The link type enumeration value.
 * @param[in] InstNumber       - The instance number.
 * @param[out] pLowerLayersBuf - Pointer to a buffer where the lower layers path will be stored.
 *                               \n The buffer should be allocated with sufficient size.
 * @param[in,out] pBufLen      - Pointer to the buffer length; updated with actual length on return.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ANSC_STATUS
CosaUtilConstructLowerLayers
    (
        COSA_DML_LINK_TYPE          LinkType,
        ULONG                       InstNumber,
        char*                       pLowerLayersBuf,
        PULONG                      pBufLen
    );

/**
 * @brief Retrieve the parameter name of the lower layer.
 *
 * This function retrieves the parameter name of the lower layer based on the specified link type and instance number.
 *
 * @param[in] LinkType        - The link type enumeration value.
 * @param[in] InstNumber      - The instance number.
 * @param[out] pParamValueBuf - Pointer to a buffer where the parameter name will be stored.
 *                              \n The buffer should be allocated with sufficient size.
 * @param[in,out] pBufLen     - Pointer to the buffer length; updated with actual length on return.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ANSC_STATUS
CosaUtilGetLowerLayerName
    (
        COSA_DML_LINK_TYPE          LinkType,
        ULONG                       InstNumber,
        char*                       pParamValueBuf,
        PULONG                      pBufLen
    );

// utility functions for getting Bridge information
/**
 * @brief Find the bridge name from bridge path.
 *
 * This function retrieves the bridge name corresponding to the given bridge path.
 *
 * @param[in] pBridgePath - Pointer to the bridge path string.
 *
 * @return Pointer to the bridge name string.
 * @retval PUCHAR Pointer to the bridge name on success.
 * @retval NULL if bridge path is not found.
 *
 */
PUCHAR CosaUtilFindBridgeName(char* pBridgePath);

/**
 * @brief Find the bridge path from bridge name.
 *
 * This function retrieves the bridge path corresponding to the given bridge name.
 *
 * @param[in] pBridgeName - Pointer to the bridge name string.
 *
 * @return Pointer to the bridge path string.
 * @retval PUCHAR Pointer to the bridge path on success.
 * @retval NULL if bridge name is not found.
 *
 */
PUCHAR CosaUtilFindBridgePath(char* pBridgeName);


#define BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE                                \
    {                                                                           \
        BOOL bridgeMode;                                                        \
        if ((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&      \
            (TRUE == bridgeMode))                                               \
            return FALSE;                                                       \
    }                                                                           \

/*
 * IPV4 Address check functions
 */
/* __NOTE__ all addresses here are in the host byte order */
#define IPV4_CLASSA(a)        ((((uint32_t)(a)) & 0x80000000UL) == 0)
#define IPV4_CLASSA_NET       0xff000000
#define IPV4_CLASSA_NSHIFT    24
#define IPV4_CLASSA_HOST      (0xffffffff & ~IP_CLASSA_NET)
#define IPV4_CLASSA_MAX       128

#define IPV4_CLASSB(a)        ((((uint32_t)(a)) & 0xc0000000UL) == 0x80000000UL)
#define IPV4_CLASSB_NET       0xffff0000
#define IPV4_CLASSB_NSHIFT    16
#define IPV4_CLASSB_HOST      (0xffffffff & ~IP_CLASSB_NET)
#define IPV4_CLASSB_MAX       65536

#define IPV4_CLASSC(a)        ((((uint32_t)(a)) & 0xe0000000UL) == 0xc0000000UL)
#define IPV4_CLASSC_NET       0xffffff00
#define IPV4_CLASSC_NSHIFT    8
#define IPV4_CLASSC_HOST      (0xffffffff & ~IP_CLASSC_NET)

#define IPV4_CLASSD(a)        (((uint32_t)(a) & 0xf0000000UL) == 0xe0000000UL)
#define IPV4_CLASSD_NET       0xf0000000
#define IPV4_CLASSD_NSHIFT    28
#define IPV4_CLASSD_HOST      0x0fffffff
#define IPV4_MULTICAST(a)     IP_CLASSD(a)

#define IPV4_EXPERIMENTAL(a)  (((uint32_t)(a) & 0xf0000000UL) == 0xf0000000UL)
#define IPV4_BADCLASS(a)      (((uint32_t)(a) & 0xf0000000UL) == 0xf0000000UL)

/* __NOTE__ all addresses here are in the network byte order */
/**
 * @brief Check if two IPv4 addresses are in the same network.
 *
 * This function checks if two IPv4 addresses belong to the same network based on the netmask.
 * Note: All addresses should be in network byte order.
 *
 * @param[in] addr1 - The first IPv4 address in network byte order.
 * @param[in] addr2 - The second IPv4 address in network byte order.
 * @param[in] mask  - The netmask in network byte order.
 *
 * @return The comparison result.
 * @retval 1 if addresses are in the same network.
 * @retval 0 if addresses are not in the same network.
 *
 */
int IPv4Addr_IsSameNetwork(uint32_t addr1, uint32_t addr2, uint32_t mask);

/**
 * @brief Check if an IPv4 address is a loopback address.
 *
 * This function checks if the given IPv4 address is a loopback address (127.0.0.0/8).
 * Note: Address should be in network byte order.
 *
 * @param[in] addr - The IPv4 address in network byte order.
 *
 * @return The check result.
 * @retval 1 if address is a loopback address.
 * @retval 0 if address is not a loopback address.
 *
 */
int IPv4Addr_IsLoopback(uint32_t addr);

/**
 * @brief Check if an IPv4 address is a multicast address.
 *
 * This function checks if the given IPv4 address is a multicast address (Class D: 224.0.0.0 - 239.255.255.255).
 * Note: Address should be in network byte order.
 *
 * @param[in] addr - The IPv4 address in network byte order.
 *
 * @return The check result.
 * @retval 1 if address is a multicast address.
 * @retval 0 if address is not a multicast address.
 *
 */
int IPv4Addr_IsMulticast(uint32_t addr);

/**
 * @brief Check if an IPv4 address is a broadcast address.
 *
 * This function checks if the given IPv4 address is a broadcast address for the specified network.
 * Note: All addresses should be in network byte order.
 *
 * @param[in] addr - The IPv4 address in network byte order.
 * @param[in] net  - The network address in network byte order.
 * @param[in] mask - The netmask in network byte order.
 *
 * @return The check result.
 * @retval 1 if address is a broadcast address.
 * @retval 0 if address is not a broadcast address.
 *
 */
int IPv4Addr_IsBroadcast(uint32_t addr, uint32_t net, uint32_t mask);

/**
 * @brief Check if an IPv4 address is a network address.
 *
 * This function checks if the given IPv4 address is the network address for the specified network.
 * Note: All addresses should be in network byte order.
 *
 * @param[in] addr - The IPv4 address in network byte order.
 * @param[in] net  - The network address in network byte order.
 * @param[in] mask - The netmask in network byte order.
 *
 * @return The check result.
 * @retval 1 if address is a network address.
 * @retval 0 if address is not a network address.
 *
 */
int IPv4Addr_IsNetworkAddr(uint32_t addr, uint32_t net, uint32_t mask);

/**
 * @brief Check if a netmask is valid.
 *
 * This function checks if the given netmask is valid (contiguous 1s followed by contiguous 0s).
 * Note: Netmask should be in network byte order.
 *
 * @param[in] netmask - The netmask in network byte order.
 *
 * @return The validation result.
 * @retval 1 if netmask is valid.
 * @retval 0 if netmask is not valid.
 *
 */
int IPv4Addr_IsNetmaskValid(uint32_t netmask);

/**
 * @brief Check if an IPv4 address is a Class A address.
 *
 * This function checks if the given IPv4 address is a Class A address (0.0.0.0 - 127.255.255.255).
 * Note: Address should be in network byte order.
 *
 * @param[in] addr - The IPv4 address in network byte order.
 *
 * @return The check result.
 * @retval 1 if address is a Class A address.
 * @retval 0 if address is not a Class A address.
 *
 */
int IPv4Addr_IsClassA(uint32_t addr);

/**
 * @brief Check if an IPv4 address is a Class B address.
 *
 * This function checks if the given IPv4 address is a Class B address (128.0.0.0 - 191.255.255.255).
 * Note: Address should be in network byte order.
 *
 * @param[in] addr - The IPv4 address in network byte order.
 *
 * @return The check result.
 * @retval 1 if address is a Class B address.
 * @retval 0 if address is not a Class B address.
 *
 */
int IPv4Addr_IsClassB(uint32_t addr);

/**
 * @brief Check if an IPv4 address is a Class C address.
 *
 * This function checks if the given IPv4 address is a Class C address (192.0.0.0 - 223.255.255.255).
 * Note: Address should be in network byte order.
 *
 * @param[in] addr - The IPv4 address in network byte order.
 *
 * @return The check result.
 * @retval 1 if address is a Class C address.
 * @retval 0 if address is not a Class C address.
 *
 */
int IPv4Addr_IsClassC(uint32_t addr);

/**
 * @brief Check if an IPv4 address is a Class D address.
 *
 * This function checks if the given IPv4 address is a Class D (multicast) address (224.0.0.0 - 239.255.255.255).
 * Note: Address should be in network byte order.
 *
 * @param[in] addr - The IPv4 address in network byte order.
 *
 * @return The check result.
 * @retval 1 if address is a Class D address.
 * @retval 0 if address is not a Class D address.
 *
 */
int IPv4Addr_IsClassD(uint32_t addr);

/**
 * @brief Execute a formatted system command.
 *
 * This function executes a system command with variable arguments.
 *
 * @param[in] fmt - Pointer to the format string for the command.
 * @param[in] ... - Variable arguments to be formatted into the command string.
 *
 * @return The status of the system command execution.
 * @retval int The return value from the system() call.
 *
 */
int vsystem(const char *fmt, ...);

/**
 * @brief Get the hardware (MAC) address of a network interface.
 *
 * This function retrieves the hardware (MAC) address for the specified network interface.
 *
 * @param[in] ifname  - Pointer to the interface name.
 * @param[out] mac    - Pointer to a buffer where the MAC address will be stored.
 *                      \n The buffer should be allocated with sufficient size.
 * @param[in] size    - The size of the MAC address buffer.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval -1 if the operation fails.
 *
 */
int get_if_hwaddr(const char *ifname, char *mac, size_t size);

/**
 * @brief Check if the device is in bridge mode.
 *
 * This function checks if the USG device is in bridge mode.
 *
 * @param[out] pBridgeMode - Pointer to store the bridge mode status.
 *                           \n TRUE if device is in bridge mode, FALSE otherwise.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ANSC_STATUS is_usg_in_bridge_mode(BOOL *pBridgeMode);

#endif
