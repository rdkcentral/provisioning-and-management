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

    module: cosa_x_cisco_com_devicecontrol_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        07/15/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_X_CISCO_COM_DEVICECONTROL_APIS_H
#define  _COSA_X_CISCO_COM_DEVICECONTROL_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

#include <sys/sysinfo.h>


#define CONFIG_AVAHI_DAEMON_FILENAME    "/var/tmp/avahi/avahi-daemon.conf"

#define CMD_START_AVAHI_DAEMON          "avahi-daemon -D"
#define CMD_START_AVAHI_AUTOIPD         "avahi-autoipd -D --force brlan0"
#define CMD_STOP_AVAHI_DAEMON           "avahi-daemon -k"
#define CMD_STOP_AVAHI_AUTOIPD          "avahi-autoipd -k brlan0"

#define UTOPIA_AVAHI_ENABLED            "ccsp_zeroconf_enabled"

#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
#define WAN_DBUS_PATH                   "/com/cisco/spvtg/ccsp/wanmanager"
#define WAN_COMPONENT_NAME              "eRT.com.cisco.spvtg.ccsp.wanmanager"
#define WAN_DML_IF_COUNT                "Device.X_RDK_WanManager.InterfaceNumberOfEntries"
#define WAN_DML_VIF_COUNT               "Device.X_RDK_WanManager.Interface.%lu.VirtualInterfaceNumberOfEntries"
#define WAN_DML_VIF_IP_MODE             "Device.X_RDK_WanManager.Interface.%lu.VirtualInterface.%lu.IP.Mode"
#endif


/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/
enum
{
    PRIMARY_MODE=1,
    BYOI_MODE,
    NONE_MODE
};

typedef enum
_COSA_DML_WanAddrMode
{
    COSA_DML_WanAddrMode_DHCP       = 1,
    COSA_DML_WanAddrMode_Static,
    COSA_DML_WanAddrMode_DHALIP,
}
COSA_DML_WanAddrMode, *PCOSA_DML_WanAddrMode;


typedef enum
_COSA_DML_LanMode
{
    COSA_DML_LanMode_BridgeDHCP = 1,
    COSA_DML_LanMode_BridgeStatic = 2,
    COSA_DML_LanMode_Router = 3,
	COSA_DML_LanMode_FullBridgeStatic = 4
}
COSA_DML_LanMode, *PCOSA_DML_LanMode;

typedef enum
_COSA_DML_LanNetworksAllow
{
    COSA_DML_LanNetworksAllow_Default = 0,
    COSA_DML_LanNetworksAllow_AnyPrivateClass,
    COSA_DML_LanNetworksAllow_AnyClass,
}
COSA_DML_LanNetworksAllow, *PCOSA_DML_LanNetworksAllow;

typedef enum
_COSA_DML_LanNapt
{
//    COSA_DML_LanNapt_Disable = 0,
    COSA_DML_LanNapt_DHCP =1,
    COSA_DML_LanNapt_StaticIP,
}COSA_DML_LanNapt, *PCOSA_DML_LanNapt;

typedef struct
_COSA_DML_LAN_MANAGEMENT
{
    ULONG                       InstanceNumber;
    char                        Alias[COSA_DML_IF_NAME_LENGTH];

    COSA_DML_LanMode            LanMode;
    ANSC_IPV4_ADDRESS           LanNetwork;
    COSA_DML_LanNetworksAllow   LanNetworksAllow;
    ANSC_IPV4_ADDRESS           LanSubnetMask;
    ANSC_IPV4_ADDRESS           LanIPAddress;
    BOOLEAN                     LanDhcpServer;
    COSA_DML_LanNapt            LanNaptType;
    BOOLEAN                     LanNaptEnable;
    ULONG                       LanTos;
    BOOLEAN                     LanDhcp125;
    BOOLEAN                     LanHnap;
    BOOLEAN                     LanUpnp;
}
COSA_DML_LAN_MANAGEMENT, *PCOSA_DML_LAN_MANAGEMENT;

#if defined (_COSA_QCA_ARM_)
typedef struct
_COSA_DML_MCPROXY
{
 BOOLEAN bEnabled;
 char Name[COSA_DML_IF_NAME_LENGTH];
 char Upstream[COSA_DML_IF_NAME_LENGTH];
 char Downstream[COSA_DML_IF_NAME_LENGTH];
 char bProtocol[COSA_DML_IF_NAME_LENGTH];
 char Ipv6_erouterIP[COSA_DML_IF_NAME_LENGTH];
 BOOLEAN ApplySetting;
}
COSA_DML_MCPROXY, *PCOSA_DML_MCPROXY;

typedef struct
_COSA_DML_APPLYSETTING
{
 BOOLEAN apply_bEnabled;
 char apply_Name[COSA_DML_IF_NAME_LENGTH];
 char apply_Upstream[COSA_DML_IF_NAME_LENGTH];
 char apply_Downstream[COSA_DML_IF_NAME_LENGTH];
 char apply_bProtocol[COSA_DML_IF_NAME_LENGTH];
 char apply_Ipv6_erouterIP[COSA_DML_IF_NAME_LENGTH];
 BOOLEAN apply_ApplySetting;
}
COSA_DML_APPLYSETTING, *PCOSA_DML_APPLYSETTING;
#endif
typedef struct
_COSA_NOTIFY_WIFI
{
    int flag;
    int ticket;
}
COSA_NOTIFY_WIFI, *PCOSA_NOTIFY_WIFI;

#define FACTORY_RESET_KEY "factory_reset"
#define FACTORY_RESET_WIFI_VALUE "w"
#define FACTORY_RESET_ROUTER_VALUE "y"

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the Device Control module.
*
* This function initializes the Device Control backend by reading ZeroConfig
* enabled status from Utopia storage and starting Avahi services if enabled.
*
* @param[in] hDml  - Opaque handle from DM adapter.
* @param[out] phContext  - Opaque handle passed back from backend.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Get the Multi-Homed HSD flag value.
*
* This function retrieves the Multi-Homed HSD (High-Speed Data) flag.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the HSD flag value will be returned.
* @param[in,out] pulSize  - Pointer to the size variable.
*                    \n On input, contains the buffer size.
*                    \n On output, updated with the actual string length.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcGetMultiHomedHSDFlag
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the Multi-Homed UI Page Control value.
*
* This function retrieves the Multi-Homed UI Page Control settings.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the UI Page Control value will be returned.
* @param[in,out] pulSize  - Pointer to the size variable.
*                    \n On input, contains the buffer size.
*                    \n On output, updated with the actual string length.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcGetMultiHomedUIPageControl
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the Multi-Homed mode.
*
* This function retrieves the Multi-Homed mode setting.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the mode value should be returned.
* @param[in,out] pulSize  - Pointer to the size variable.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcGetMultiHomedMode
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the Multi-Homed bridging status.
*
* This function retrieves the Multi-Homed bridging status.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the bridging status should be returned.
* @param[in,out] pulSize  - Pointer to the size variable.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcGetMultiHomedBridgingStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Set the Multi-Homed HSD flag.
*
* This function sets the Multi-Homed HSD flag.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulValue  - The value to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcSetMultiHomedHSDFlag
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue
    );

/**
* @brief Set the Multi-Homed UI Page Control value.
*
* This function sets the Multi-Homed UI Page Control settings.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulValue  - The value to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcSetMultiHomedUIPageControl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue
    );

/**
* @brief Set the Multi-Homed mode.
*
* This function sets the Multi-Homed mode.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulValue  - The mode value to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcSetMultiHomedMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue
    );

/**
* @brief Get the WAN address mode.
*
* This function retrieves the WAN addressing mode (DHCP, Static) from Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pMode  - Pointer to store the WAN address mode.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetWanAddressMode
    (
        ANSC_HANDLE                 hContext,
        COSA_DML_WanAddrMode        *pMode
    );

/**
* @brief Get the WAN static IP address.
*
* This function retrieves the WAN static IP address from Utopia storage
*
* @param[in] hContext  - Handle to the context.
* @param[out] ipAddr  - Pointer to store the WAN static IP address in ULONG format.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetWanStaticIPAddress
    (
        ANSC_HANDLE                 hContext,
        /* XXX: Why not ANSC_IPV4_ADDRESS? it will build fail */
        ULONG                       *ipAddr
    );

/**
* @brief Get the WAN static subnet mask.
*
* This function retrieves the WAN static subnet mask from Utopia storage
*
* @param[in] hContext  - Handle to the context.
* @param[out] ipAddr  - Pointer to store the WAN static subnet mask in ULONG format.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetWanStaticSubnetMask
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr
    );

/**
* @brief Get the WAN static gateway IP address.
*
* This function retrieves the WAN static default gateway IP address from Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[out] ipAddr  - Pointer to store the WAN static gateway IP address in ULONG format.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetWanStaticGatewayIP
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr
    );

/**
* @brief Get the WAN secondary IP address.
*
* This function retrieves the WAN secondary IP address.
*
* @param[in] hContext  - Handle to the context.
* @param[out] ipAddr  - Pointer to store the secondary IP address.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcGetWanSecondIPAddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr
    );

/**
* @brief Get the WAN secondary IP RIP advertised flag.
*
* This function retrieves whether the WAN secondary IP is RIP advertised.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the RIP advertised flag.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcGetWanSecondIPRipAdvertised
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Get the WAN backup default gateway.
*
* This function retrieves the WAN backup default gateway IP address.
*
* @param[in] hContext  - Handle to the context.
* @param[out] ipAddr  - Pointer to store the backup gateway IP address.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDcGetWanBackupDefaultGateway
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr
    );

/**
* @brief Get the WAN nameserver IP address.
*
* This function retrieves the WAN nameserver IP address from Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[out] ipAddr  - Pointer to store the nameserver IP address in ULONG format.
* @param[in] nameServerNo  - Nameserver number indicator.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetWanNameServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr,
        int                         nameServerNo
    );

/**
* @brief Get the WAN hostname.
*
* This function retrieves the WAN hostname by reading from /etc/hostname file.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pHostName  - Pointer to a buffer where the hostname will be returned.
*                    \n The buffer should be allocated with a size of at least 1024 bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if /etc/hostname file cannot be opened.
*
*/
ANSC_STATUS
CosaDmlDcGetWanHostName
    (
        ANSC_HANDLE                 hContext,
        char                        *pHostName
    );

/**
* @brief Get the WAN domain name.
*
* This function retrieves the current validated WAN domain name by parsing /etc/resolv.conf
* file and extracting the "search" domain value.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pDomainName  - Pointer to a buffer where the domain name will be returned.
*                    \n The buffer should be allocated with sufficient size.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if /etc/resolv.conf file cannot be opened.
*
*/
ANSC_STATUS
CosaDmlDcGetWanDomainName
    (
        ANSC_HANDLE                 hContext,
        char                        *pDomainName
    );

/**
* @brief Get the WAN static domain name configured by user.
*
* This function retrieves the WAN static domain name that was set by the user.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pStaticDomainName  - Pointer to a buffer where the static domain name will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetWanStaticDomainName
    (
        ANSC_HANDLE                 hContext,
        char                        *pStaticDomainName
    );

/**
* @brief Set the WAN address mode.
*
* This function sets the WAN addressing mode (DHCP or Static) in Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[in] mode  - The WAN address mode to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWanAddressMode
    (
        ANSC_HANDLE                 hContext,
        COSA_DML_WanAddrMode        mode
    );

/**
* @brief Set the WAN static IP address.
*
* This function sets the WAN static IP address in Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ipAddr  - The WAN static IP address to set in uint32_t format.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWanStaticIPAddress
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

/**
* @brief Set the WAN static subnet mask.
*
* This function sets the WAN static subnet mask in Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ipAddr  - The WAN static subnet mask to set in uint32_t format.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWanStaticSubnetMask
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

/**
* @brief Set the WAN static gateway IP address.
*
* This function sets the WAN static default gateway IP address in Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ipAddr  - The WAN static gateway IP address to set in uint32_t format.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWanStaticGatewayIP
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

/**
* @brief Set the WAN secondary IP address.
*
* This function sets the WAN secondary IP address.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ipAddr  - The secondary IP address to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWanSecondIPAddr
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

/**
* @brief Set the WAN secondary IP RIP advertised flag.
*
* This function sets whether the WAN secondary IP is RIP advertised.
*
* @param[in] hContext  - Handle to the context.
* @param[in] Flag  - The RIP advertised flag to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWanSecondIPRipAdvertised
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     Flag
    );

/**
* @brief Set the WAN backup default gateway.
*
* This function sets the WAN backup default gateway IP address.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ipAddr  - The backup gateway IP address to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWanBackupDefaultGateway
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

/**
* @brief Set the WAN nameserver IP address.
*
* This function sets the WAN nameserver IP address in Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ipAddr  - The nameserver IP address to set in uint32_t format.
* @param[in] nameServerNo  - Nameserver number indicator.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails, IP is in private range, or set_resolv_conf.sh execution fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWanNameServer
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr,
        int                         nameServerNo
    );

/**
* @brief Get the enable static nameserver flag.
*
* This function retrieves whether static nameservers are enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the enable flag.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetEnableStaticNameServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Set the enable static nameserver flag.
*
* This function sets whether static nameservers are enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The enable flag to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetEnableStaticNameServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Get the WAN release flag.
*
* This function retrieves the WAN release flag.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the release flag.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Set the WAN release flag to trigger DHCP release.
*
* This function sets the WAN release flag.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The release flag to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Get the WAN renew flag.
*
* This function retrieves the WAN renew flag.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the renew flag.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Set the WAN renew flag to trigger DHCP renew.
*
* This function sets the WAN renew flag.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The renew flag to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Get the reset to default enable flag.
*
* This function retrieves the reset to default enable flag.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the reset default enable flag.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetResetDefaultEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Get the SNMP enable status.
*
* This function retrieves the SNMP enable status via platform_hal_GetSNMPEnable.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the SNMP enable status will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetSNMPEnable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the SNMP enable status.
*
* This function sets the SNMP enable status via platform_hal_SetSNMPEnable.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the SNMP enable status string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetSNMPEnable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the device hostname.
*
* This function sets the device hostname in Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the hostname string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails or set_hostname.sh execution fails.
*
*/
ANSC_STATUS
CosaDmlDcSetHostName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the WAN domain name.
*
* This function sets the WAN domain name in Utopia storage.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the domain name string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails or set_wandomain.sh execution fails.
*
*/
ANSC_STATUS
CosaDmlDcSetDomainName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Get the reboot device trigger status.
*
* This function retrieves the reboot device trigger status.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the reboot status will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Get the factory reset trigger status.
*
* This function retrieves the factory reset trigger status.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the factory reset status will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Get the user changed flags.
*
* This function retrieves flags indicating which configuration values were changed by the user.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the user changed flags will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetUserChangedFlags
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Reinitialize the cable modem MAC address.
*
* This function reinitializes the cable modem MAC address.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetReInitCmMac ();

/**
* @brief Get the device configuration status.
*
* This function retrieves the device configuration status.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the device configuration status will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetDeviceConfigStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Get the device config ignore status.
*
* This function retrieves the device config ignore status.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the config ignore status will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetDeviceConfigIgnore
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the device config ignore status.
*
* This function sets the device config ignore status.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the config ignore status string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetDeviceConfigIgnore
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the reboot device trigger.
*
* This function triggers a device reboot based on the provided value.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the reboot trigger value string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Get the factory reset trigger status (duplicate).
*
* This function retrieves the factory reset trigger status.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the factory reset status will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the user changed flags.
*
* This function sets flags indicating which configuration values were changed by the user.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the user changed flags string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetUserChangedFlags
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the reset to default enable flag.
*
* This function sets the reset to default enable flag.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The reset default enable flag to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetResetDefaultEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Get the device mode.
*
* This function retrieves the device mode from Utopia storage. The mode is 0-based in storage but
* converted to 1-based for the data model.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to store the device mode.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetDeviceMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

/**
* @brief Get the Telnet enable status.
*
* This function retrieves whether Telnet service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the Telnet enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetTelnetEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Get the SSH enable status.
*
* This function retrieves whether SSH service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the SSH enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetSSHEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Get the HNAP enable status.
*
* This function retrieves whether HNAP (Home Network Administration Protocol) service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the HNAP enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetHNAPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Set the Telnet enable status.
*
* This function sets whether Telnet service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The Telnet enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetTelnetEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Set the SSH enable status.
*
* This function sets whether SSH service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The SSH enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetSSHEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Set the HNAP enable status.
*
* This function sets whether HNAP (Home Network Administration Protocol) service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The HNAP enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetHNAPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );


/**
* @brief Get the MSO remote management enable status.
*
* This function retrieves whether MSO remote management is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the MSO remote management enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetMsoRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Set the MSO remote management enable status.
*
* This function sets whether MSO remote management is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The MSO remote management enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetMsoRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Get the customer admin remote management enable status.
*
* This function retrieves whether customer admin remote management is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the customer admin remote management enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetCusadminRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Set the customer admin remote management enable status.
*
* This function sets whether customer admin remote management is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The customer admin remote management enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetCusadminRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Get the HS Ethernet port enable status.
*
* This function retrieves whether the HS Ethernet port is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the HS Ethernet port enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetHSEthernetPortEnable
    (
        ANSC_HANDLE                hContext,
        BOOLEAN                    *pFlag
    );

/**
* @brief Set the HS Ethernet port enable status.
*
* This function sets whether the HS Ethernet port is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The HS Ethernet port enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetHSEthernetPortEnable
    (
        ANSC_HANDLE                hContext,
        BOOLEAN                    pFlag
    );

/**
* @brief Get the guest password.
*
* This function retrieves the guest network password.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the guest password will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetGuestPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the guest password.
*
* This function sets the guest network password.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the guest password string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetGuestPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Get the number of guest users.
*
* This function retrieves the number of guest users currently configured.
*
* @return The status of the operation or number of guest users.
* @retval The number of guest users if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ULONG
CosaDmlDcGetNoOfGuests
    (
    );

/**
* @brief Set the number of guest users.
*
* This function sets the number of guest users.
*
* @param[in] uVal  - The number of guest users to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetNoOfGuests
    (
        ULONG                      uVal
    );

/**
* @brief Get the number of LAN Management entries.
*
* This function retrieves the number of configured LAN management entries.
*
* @return The number of LAN management entries.
*
*/
ULONG
CosaDmlLanMngm_GetNumberOfEntries
    (
        void
    );

/**
* @brief Get a LAN Management entry by index.
*
* This function retrieves a LAN management entry at the specified index.
*
* @param[in] index  - The index of the entry to retrieve.
* @param[out] pLanMngm  - Pointer to a COSA_DML_LAN_MANAGEMENT structure to store the entry.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the index is invalid or retrieval fails.
*
*/
ANSC_STATUS
CosaDmlLanMngm_GetEntryByIndex
    (
        ULONG index,
        PCOSA_DML_LAN_MANAGEMENT pLanMngm
    );

/**
* @brief Set values for a LAN Management entry.
*
* This function sets the instance number and alias for a LAN management entry at the specified index.
*
* @param[in] index  - The index of the entry to update.
* @param[in] ins  - The instance number to set.
* @param[in] alias  - Pointer to the alias string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the update fails.
*
*/
ANSC_STATUS
CosaDmlLanMngm_SetValues
    (
        ULONG index,
        ULONG ins,
        const char *alias
    );

/**
* @brief Add a new LAN Management entry.
*
* This function adds a new LAN management entry to the configuration.
*
* @param[in] pLanMngm  - Pointer to a COSA_DML_LAN_MANAGEMENT structure containing the entry to add.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlLanMngm_AddEntry
    (
        PCOSA_DML_LAN_MANAGEMENT pLanMngm
    );

/**
* @brief Delete a LAN Management entry.
*
* This function deletes a LAN management entry with the specified instance number.
*
* @param[in] ins  - The instance number of the entry to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the deletion fails.
*
*/
ANSC_STATUS
CosaDmlLanMngm_DelEntry
    (
        ULONG ins
    );

/**
* @brief Get the configuration of a LAN Management entry.
*
* This function retrieves the configuration for a LAN management entry with the specified instance number.
*
* @param[in] ins  - The instance number of the entry to retrieve.
* @param[out] pLanMngm  - Pointer to a COSA_DML_LAN_MANAGEMENT structure to store the configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the retrieval fails.
*
*/
ANSC_STATUS
CosaDmlLanMngm_GetConf
    (
        ULONG ins,
        PCOSA_DML_LAN_MANAGEMENT pLanMngm
    );

/**
* @brief Set the configuration of a LAN Management entry.
*
* This function sets the configuration for a LAN management entry with the specified instance number.
*
* @param[in] ins  - The instance number of the entry to update.
* @param[in] pLanMngm  - Pointer to a COSA_DML_LAN_MANAGEMENT structure containing the configuration to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the update fails.
*
*/
ANSC_STATUS
CosaDmlLanMngm_SetConf
    (
        ULONG ins,
        PCOSA_DML_LAN_MANAGEMENT pLanMngm
    );

/**
* @brief Reset the bridge interface (br0) with new IP configuration.
*
* This function resets the br0 bridge interface with the specified IP address and subnet mask.
*
* @param[in] ip  - Pointer to the IP address string.
* @param[in] sub  - Pointer to the subnet mask string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the reset fails.
*
*/
ANSC_STATUS CosaDmlDcResetBr0(char *ip, char *sub);


/**
* @brief Start ZeroConfig/Avahi services.
*
* This function starts the ZeroConfig automatic network configuration services.
*
* @return None.
*/
void _CosaDmlDcStartZeroConfig();

/**
* @brief Stop ZeroConfig/Avahi services.
*
* This function stops the ZeroConfig automatic network configuration services.
*
* @return None.
*
*/
void _CosaDmlDcStopZeroConfig();

/**
* @brief Get the erouter enabled status.
*
* This function retrieves whether the erouter is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the erouter enabled status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetErouterEnabled
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Set the erouter enabled status.
*
* This function sets whether the erouter is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] bFlag  - The erouter enabled status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetErouterEnabled
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    );

/**
* @brief Reboot WiFi in a separate thread.
*
* This function creates a new thread to reboot the WiFi subsystem.
*
* @param[in] hContext  - Handle to the context.
*
* @return Pointer to thread data.
* @retval Pointer to thread data if thread creation is successful.
* @retval NULL if thread creation fails.
*
*/
void* CosaDmlDcRebootWifi(ANSC_HANDLE   hContext);

/**
* @brief Restart the router in a separate thread.
*
* This function executes a router restart operation in a separate thread.
*
* @param[in] arg  - Argument passed to the thread.
*
* @return Pointer to thread result.
* @retval Pointer to thread data if thread creation is successful.
* @retval NULL if thread creation fails.
*
*/
void* CosaDmlDcRestartRouter(void* arg);

/**
* @brief Save WiFi health status into NVRAM.
*
* This function persists the WiFi health status information to non-volatile RAM.
*
* @return None.
*
*/
void CosaDmlDcSaveWiFiHealthStatusintoNVRAM( void  );

/**
* @brief Check and get device properties entry.
*
* This function checks and retrieves a device properties entry from configuration.
*
* @param[out] pOutput  - Pointer to buffer where the property value will be stored.
* @param[in] size  - Size of the output buffer.
* @param[in] sDevicePropContent  - Pointer to the device property content string to search for.
*
* @return The status of the operation.
* @retval 0 for success.
* @retval non-zero for failure.
*
*/
int CheckAndGetDevicePropertiesEntry( char *pOutput, int size, char *sDevicePropContent );

#ifndef PON_GATEWAY
/**
* @brief Reinitialize the cable modem MAC address.
*
* This function reinitializes the cable modem MAC address using the cm_hal interface.
*
* @return The status code from the HAL operation.
*
*/
INT cm_hal_ReinitMac();
#endif

/**
* @brief Check if MoCA hardware is equipped.
*
* This function checks whether MoCA hardware is present in the device.
*
* @return The status of the operation.
* @retval TRUE if MoCA hardware is equipped.
* @retval FALSE otherwise.
*
*/
BOOL moca_HardwareEquipped(void);

/**
* @brief Get the HTTP server port.
*
* This function retrieves the port number used by the HTTP web server.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to store the HTTP port number.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetHTTPPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

/**
* @brief Get the HTTPS server port.
*
* This function retrieves the port number used by the HTTPS web server.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to store the HTTPS port number.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetHTTPSPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

/**
* @brief Get the reinitialize MAC threshold value.
*
* This function retrieves the threshold value for cable modem MAC reinitialization.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to store the reinit MAC threshold value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetReinitMacThreshold
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

/**
* @brief Get the IGMP proxy enable status.
*
* This function retrieves whether the IGMP proxy service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the IGMP proxy enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetIGMPProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Get the DNS proxy enable status.
*
* This function retrieves whether the DNS proxy service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the DNS proxy enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetDNSProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

/**
* @brief Get the ZeroConfig enable status.
*
* This function retrieves whether ZeroConfig automatic network configuration is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pFlag  - Pointer to store the ZeroConfig enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcGetEnableZeroConfig
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    );

/**
* @brief Get the HTTP server enable status.
*
* This function retrieves whether the HTTP web server is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to store the HTTP enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetHTTPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    );

/**
* @brief Get the HTTPS server enable status.
*
* This function retrieves whether the HTTPS web server is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to store the HTTPS enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetHTTPSEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    );

/**
* @brief Get the IGMP snooping enable status.
*
* This function retrieves whether IGMP snooping is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to store the IGMP snooping enable status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetIGMPSnoopingEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    );

/**
* @brief Get the WebUI timeout value.
*
* This function retrieves the WebUI inactivity timeout value in seconds.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to store the WebUI timeout value in seconds.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

/**
* @brief Get the power saving mode status.
*
* This function retrieves the current power saving mode status of the device.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to store the power saving mode status value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetPowerSavingModeStatus
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

/**
* @brief Get the MoCA hardware status.
*
* This function retrieves the MoCA hardware status by calling moca_HardwareEquipped().
*
* @param[in] hContext  - Handle to the context.
*
* @return The MoCA hardware status
* @retval 1 means moca hardware is Available.
* @retval 2 means moca hardware is NotAvailable.
*
*/
ULONG
CosaDmlGetMocaHardwareStatus
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get the parental control password.
*
* This function retrieves the parental control password.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the parental control password will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Get the default parental control password.
*
* This function retrieves the default parental control password from device configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the default parental control password will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetDefaultParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Get the parental control security question.
*
* This function retrieves the parental control security question.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the security question will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetParConQuestion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Get the parental control security answer.
*
* This function retrieves the parental control security answer.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pValue  - Pointer to a buffer where the security answer will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetParConAnswer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the IGMP proxy enable status.
*
* This function sets whether the IGMP proxy service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The IGMP proxy enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or script execution fails.
*
*/
ANSC_STATUS
CosaDmlDcSetIGMPProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Set the DNS proxy enable status.
*
* This function sets whether the DNS proxy service is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pFlag  - The DNS proxy enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcSetDNSProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

/**
* @brief Set the ZeroConfig enable status.
*
* This function sets whether ZeroConfig automatic network configuration is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] bFlag  - The ZeroConfig enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ANSC_STATUS
CosaDmlDcSetEnableZeroConfig
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    );

/**
* @brief Set the IGMP snooping enable status.
*
* This function sets whether IGMP snooping is enabled.
*
* @param[in] hContext  - Handle to the context.
* @param[in] value  - The IGMP snooping enable status to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetIGMPSnoopingEnable
    (
        ANSC_HANDLE                 hContext,
        ULONG                     value
    );

/**
* @brief Set the device mode.
*
* This function sets the device mode.
*
* @param[in] hContext  - Handle to the context.
* @param[in] value  - The device mode to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or WAN restart fails.
*
*/
ANSC_STATUS CosaDmlDcSetDeviceMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    );

/**
* @brief Set the parental control password.
*
* This function sets the parental control password.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the parental control password string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the parental control security question.
*
* This function sets the parental control security question.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the security question string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetParConQuestion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the parental control security answer.
*
* This function sets the parental control security answer.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the security answer string to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetParConAnswer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the factory reset trigger.
*
* This function triggers a factory reset of the device to restore default settings.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pValue  - Pointer to the factory reset trigger value string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Configure the web server settings.
*
* This function configures HTTP and HTTPS server enable status and port numbers.
*
* @param[in] httpEn  - Enable status for HTTP server.
* @param[in] httpsEn  - Enable status for HTTPS server.
* @param[in] httpPort  - Port number for HTTP server.
* @param[in] httpsPort  - Port number for HTTPS server.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if port validation fails or configuration fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWebServer(BOOL httpEn, BOOL httpsEn, ULONG httpPort, ULONG httpsPort);


/**
* @brief Set the web access level for a user and interface.
*
* This function sets the web access level configuration for a specific user and interface.
*
* @param[in] hContext  - Handle to the context.
* @param[in] userIndex  - The user index.
* @param[in] ifIndex  - The interface index.
* @param[in] value  - The web access level value to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWebAccessLevel
    (
        ANSC_HANDLE                 hContext,
        int                         userIndex,
        int                         ifIndex,
        ULONG                       value
    );

/**
* @brief Get the web access level for a user and interface.
*
* This function retrieves the web access level configuration for a specific user and interface.
*
* @param[in] hContext  - Handle to the context.
* @param[in] userIndex  - The user index.
* @param[in] ifIndex  - The interface index.
* @param[out] pValue  - Pointer to store the web access level value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcGetWebAccessLevel
    (
        ANSC_HANDLE                 hContext,
        int                         userIndex,
        int                         ifIndex,
        ULONG                       *pValue
    );

/**
* @brief Check if a port is currently in use.
*
* This function checks whether a specific port number is currently in use by the system.
*
* @param[in] port  - The port number to check.
*
* @return The status of the operation.
* @retval TRUE if the port is in use.
* @retval FALSE otherwise.
*
*/
BOOL
IsPortInUse
(
    unsigned int port
);

/**
* @brief Check if port overlaps with port forwarding rules.
*
* This function checks whether a management port overlaps with configured port forwarding rules.
*
* @param[in] mgmtport  - The management port number to check.
*
* @return The status of the operation.
* @retval True if there is an overlap.
* @retval False otherwise.
*
*/
bool IsPortOverlapWithPFPorts(int mgmtport);

/**
* @brief Check if port overlaps with port triggering rules.
*
* This function checks whether a management port overlaps with configured port triggering rules.
*
* @param[in] mgmtport  - The management port number to check.
*
* @return The status of the operation.
* @retval True if there is an overlap.
* @retval False otherwise.
*
*/
bool IsPortOverlapWithPTPorts(int mgmtport);


/**
* @brief Set the reinitialize MAC threshold value.
*
* This function sets the threshold value for cable modem MAC reinitialization.
*
* @param[in] hContext  - Handle to the context.
* @param[in] value  - The reinit MAC threshold value to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetReinitMacThreshold
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    );

/**
* @brief Set the WebUI timeout value.
*
* This function sets the WebUI inactivity timeout value in seconds.
*
* @param[in] hContext  - Handle to the context.
* @param[in] value  - The WebUI timeout value in seconds to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDcSetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    );
#if defined (_COSA_QCA_ARM_)
ANSC_STATUS
Cosa_Set_McProxy
 (
ANSC_HANDLE                 hContext,
 PCOSA_DML_MCPROXY          pEntry
 );

ANSC_STATUS
CosaGetMcProxyValues
 (
 ANSC_HANDLE                 hContext,
 PCOSA_DML_MCPROXY          pEntry
 );
#endif
#endif
