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
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __MANAGEDWIFI_WEBCONFIG_PARAM_H__
#define __MANAGEDWIFI_WEBCONFIG_PARAM_H__
#include "ccsp_base_api.h"
#include "messagebus_interface_helper.h"
#include "msgpack.h"
#include "webcfgparam.h"
#include "managedwifi_helpers.h"
#include "managedwifidoc.h"
#include "ccsp_trace.h"
#include <trower-base64/base64.h>
#include "cosa_webconfig_api.h"
#include "cosa_rbus_handler_apis.h"
#include <syscfg/syscfg.h>

/**
* @brief Unpack and process Managed WiFi webconfig data.
*
* This function decodes and processes the Managed WiFi configuration data received
* via webconfig. It unpacks the msgpack encoded data and applies the configuration.
*
* @param[in] pString Pointer to the encoded configuration data string.
*
* @return The status of the operation.
* @retval TRUE if the operation is successful.
* @retval FALSE if the operation fails.
*/
BOOL unpackAndProcessManagedWifiData(char* pString);

/**
* @brief Free memory allocated for Managed WiFi configuration.
*
* This function releases memory resources allocated for Managed WiFi configuration structures.
*
* @param[in] arg Pointer to the memory to be freed.
*
* @return None.
*/
void freeMem_managedwifi(void* arg);

/**
* @brief Process Managed WiFi webconfig configuration.
*
* This function processes the Managed WiFi configuration data and applies it to the system.
*
* @param[in] Data Pointer to the webconfig data structure.
*
* @return Pointer to error structure (pErr) containing operation result details.
* @retval NULL on success, indicating no error occurred during processing.
* @retval Non-NULL pErr structure on failure.
*/
pErr processManagedWebconfigConfig(void *Data);

/**
* @brief Calculate timeout for Managed WiFi operations.
*
* This function calculates the appropriate timeout value based on the number of entries.
*
* @param[in] numOfEntries Number of configuration entries to process.
*
* @return Calculated timeout value in seconds.
*/
size_t calculateTimeout(size_t numOfEntries);

/**
* @brief Create Managed WiFi bridge.
*
* This function creates and configures the Managed WiFi L3 network bridge with the
* specified LAN configuration parameters.
*
* @param[in] pLanConfigParams Pointer to the LAN configuration parameters structure.
*
* @return Pointer to error structure (pErr) containing operation result details.
* @retval NULL on success, indicating no error occurred during processing.
* @retval Non-NULL pErr structure on failure.
*/
pErr create_manage_wifi_bridge(lanconfig_t * pLanConfigParams);

/**
* @brief Initialize Managed WiFi backup structure.
*
* This function initializes the backup configuration structure used for rollback
* operations in case of configuration failures.
*
* @return None.
*/
void initManageWiFiBacupStruct(void);

#define BR106_PSM_INDEX "6"
#define MAX_MANAGE_WIFI_BRIDGE_THREAD_EXEC_TIMEOUT 30
#define BUFF_LEN_128 128
#define BUFF_LEN_64 64
#define BUFF_LEN_32 32
#define BUFF_LEN_8  8

#define MANAGE_WIFI_BRIDGE_INDEX "dmsb.MultiLAN.ManageWiFi_l3net"
#define MANAGE_WIFI_LAN_BRIDGE "Device.LAN.Bridge.1.Name"
#define MANAGE_WIFI_ENABLE "Device.X_RDK_LANConfig.ManagedWiFiEnable"
#define MANAGE_WIFI_INTERFACES "Device.LAN.Bridge.1.WiFiInterfaces"

#if defined (AMENITIES_NETWORK_ENABLED)
#define NUM_OF_SUPPORTED_ELEMENTS 3
#define LAN_BRIDGES_TABLE "Device.LAN.Bridge."
#define VAP_NAME_2G "managed_guest_2g"
#define VAP_NAME_5G "managed_guest_5g"
#define VAP_NAME_6G "managed_guest_6g"

#define WIFI_IF_STR "WiFiInterfaces"
#define BRIDGE_NAME_STR "Name"
#define VAP_NAME_STR "VapName"

#define VAP_NAME_2G_INDEX "dmsb.MultiLAN.AmenityNetwork_2g_l3net"
#define VAP_NAME_5G_INDEX "dmsb.MultiLAN.AmenityNetwork_5g_l3net"
#define VAP_NAME_6G_INDEX "dmsb.MultiLAN.AmenityNetwork_6g_l3net"
#endif


typedef enum
{
    BRIDGE_NAME,
    WIFI_INTERFACES,
    OTHER_UPDATE
}updateType_t;
typedef struct
{
    char aKey[BUFF_LEN_32];
    char aBridgeName[BUFF_LEN_32];
    char aBridgeIndex[BUFF_LEN_8];
    char aWiFiInterfaces[BUFF_LEN_128];
    updateType_t eUpdateType;
}ManageWiFiInfo_t;

typedef struct
{
    bool bMwEnable;
    char aAlias [BUFF_LEN_128];
    bool bDhcpServerEnable;
    char aLanIpAddr [BUFF_LEN_32];
    char aLanSubnetMask [BUFF_LEN_32];
    char aDhcpStartIpAdd [BUFF_LEN_32];
    char aDhcpEndIpAdd [BUFF_LEN_32];
    char aLeaseTime [BUFF_LEN_32];
    bool bIpv6Enable;
    char aWiFiInterfaces[BUFF_LEN_128];
} backupLanconfig_t;

typedef enum
{
    RBUS,
    OTHER_EVENT
}EventType;

typedef enum
{
    PRIVATE,
    XHS,
    MANAGE_WIFI
}InterfaceType_t;

#if defined (AMENITIES_NETWORK_ENABLED)
typedef struct
{
    char cBridgeName[BUFF_LEN_32];
    char cBridgeIndex[BUFF_LEN_8];
    char cVapName[BUFF_LEN_32];
    char cWiFiInterface[BUFF_LEN_8];
    int iVlanId;
}amenityBridgeInfo_t;

typedef struct
{
    amenityBridgeInfo_t *pBridgeInfoStruct;
    int iBridgeCount;
    bool bIsAmenityEnabled;
}amenityBridgeDetails_t;

typedef struct
{
    int iBridgeIndex;
    const char *pVapName;
} VapMapping;

typedef struct {
    const char *vapName;
    uint16_t bitEnable;
    uint16_t bitDisable;
    uint16_t bitDown;
    uint16_t bitUp;
} BridgeBandInfo;

typedef struct
{
   amenityBridgeDetails_t ** ppAmenityDetails;
   uint16_t ui16Flag;
   bool completed;
}AmenityThread_t;

/**
* @brief Get amenity network RBUS data.
*
* This function retrieves amenity network bridge configuration data via RBUS.
*
* @param[out] ppBridgeRbusData Pointer to receive the amenity bridge information array.
* @param[out] pVapCount Pointer to receive the number of VAPs.
*
* @return The status of the operation.
* @retval 0 on success.
* @retval -1 on failure.
*/
int getAmenityRbusData(amenityBridgeInfo_t ** ppBridgeRbusData, int *pVapCount);

/**
* @brief Initialize amenity network bridges.
*
* This function initializes the amenity network bridges by reading configuration from PSM.
*
* @return None.
*/
void initializeAmenityBridges(void);

/**
* @brief Read tunnel details from PSM.
*
* This function reads GRE tunnel configuration details for amenity networks from PSM.
*
* @param[in] iBridgeCount Number of bridges to read tunnel details for.
*
* @return The status of the operation.
* @retval 0 on success,
* @retval -1 on failure.
*/
int readTunnelDetailsFromPsm(int iBridgeCount);

/**
* @brief Update amenity network PSM configuration.
*
* This function updates the PSM with amenity network VAP configuration including VAP index and VAP name.
*
* @param[in] cVapIndex VAP index string.
* @param[in] cVapName VAP name string.
* @param[in] iCount Bridge count.
* @param[out] pErrRetVal Pointer to error structure to receive operation result.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval 1 if the operation fails.
*/
BOOL updateAmenityPSM(char *cVapIndex, char *cVapName , int iCount , pErr pErrRetVal);

/**
* @brief Bring down Managed WiFi bridge.
*
* This function disables and removes the Managed WiFi bridge configuration.
*
* @param[out] pErrRetVal Pointer to error structure to receive operation result.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval 1 if the operation fails.
*/
BOOL bringDownManagedWifiBridge(pErr pErrRetVal);
#endif

typedef struct
{
    InterfaceType_t eInterfaceType;
    char aIpAddr[BUFF_LEN_32];
    char aStartIpAddr[BUFF_LEN_32];
    char aEndIpAddr[BUFF_LEN_32];
}LanDetails_t;

typedef struct
{
   bool bMwEnable;
   char cFlag;
   bool completed;
}threadStruct_t;

/**
* @brief Get bridge details from PSM.
*
* This function retrieves bridge configuration details from PSM including bridge name.
*
* @return None.
*/
void getBridgeDetailsFromPsm(void);

/**
* @brief Get Managed WiFi details.
*
* This function retrieves the current Managed WiFi configuration details from PSM storage.
*
* @param[out] pManageWifiInfo Pointer to structure to receive Managed WiFi information.
*
* @return None.
*/
void getManageWiFiDetails(ManageWiFiInfo_t * pManageWifiInfo);

/**
* @brief Set Managed WiFi details.
*
* This function updates the Managed WiFi configuration details in PSM storage.
*
* @param[in] pManageWifiInfo Pointer to structure containing Managed WiFi information to set.
*
* @return None.
*/
void setManageWiFiDetails(ManageWiFiInfo_t * pManageWifiInfo);

/**
* @brief Notify via RBUS.
*
* This function sends a notification via RBUS for TR-181 parameter changes.
*
* @param[in] pTr181Param TR-181 parameter name.
* @param[in] pTr181ParamVal TR-181 parameter value.
*
* @return RBUS error code
* @retval RBUS_ERROR_SUCCESS on success
* @retval other RBUS error code otherwise.
*/
rbusError_t notifyViaRbus(char * pTr181Param, char *pTr181ParamVal);

/**
* @brief Process Managed WiFi configuration data.
*
* This function processes Managed WiFi configuration updates, applying changes
* to the system and handling rollback if necessary.
*
* @param[in] pLanConfig Pointer to LAN configuration structure.
* @param[in] cFlag Operation flags.
* @param[out] pErrRetVal Pointer to error structure to receive operation result.
*
* @return None.
*/
void processManageWifiData(backupLanconfig_t * pLanConfig, char cFlag, pErr pErrRetVal);

/**
* @brief Rollback LAN configuration.
*
* This function rolls back LAN configuration to the previous backup state
* when Lan blob execution fails.
*
* @return The status of the operation.
* @retval 0 on success.
* @retval Non-zero on failure.
*/
int rollbackLanconfig(void);

/**
* @brief Managed WiFi bridge creation thread.
*
* This is the thread function that handles Managed WiFi bridge creation and
* configuration in a separate thread to avoid blocking.
*
* @param[in,out] vArg Pointer to thread arguments.
*
* @return None
*/
void *ManageWiFiBridgeCreationThread(void * vArg);
#if defined (AMENITIES_NETWORK_ENABLED)
/**
* @brief Rollback tunnel LAN configuration.
*
* This function rolls back the GRE tunnel and LAN configuration for amenity
* networks to the previous backup state in case of configuration failures.
*
* @return The status of the operation.
* @retval 0 on success.
* @retval Non-zero on failure.
*/
int rollbackTunnelLanconfig (void);

/**
* @brief Clear LAN tunnel PSM configuration.
*
* This function clears the PSM configuration for a specific LAN tunnel bridge.
*
* @param[in] pBridgeIndex Bridge index string to clear PSM configuration.
*
* @return None.
*/
void lanTunnelPsmClear(char *pBridgeIndex);

/**
* @brief Amenity multinet notification handler thread.
*
* This is the thread function that handles amenity network multinet notifications
* and processes RBUS events for amenity bridge state changes.
*
* @param[in,out] vArg Pointer to thread arguments.
*
* @return None
*/
void *amenityMultinetNotifyHandler(void * vArg);

/**
* @brief Update firewall syscfg for amenity networks.
*
* This function updates the firewall syscfg configuration based on the number
* of amenity bridges and their enable state.
*
* @param[in] iBridgeCount Number of amenity bridges.
* @param[in] bIsAmenityEnabled Amenity network enable flag.
*
* @return The status of the operation.
* @retval 0 on success.
* @retval Non-zero on failure.
*/
int updateFirewallSyscfg(int iBridgeCount, bool bIsAmenityEnabled);
#endif

/**
* @brief Remove substring from a string.
*
* This function removes a specified substring from the main string, with optional
* extra space removal around the substring.
*
* @param[in,out] pMainString Main string to remove substring from.
* @param[in] pSubstring Substring to remove.
* @param[in] bExtraSpaceRemoval Flag to remove extra spaces (TRUE) or not (FALSE).
*
* @return The status of the operation.
* @retval 0 on success.
* @retval -1 on failure.
*/
int removeSubstring(char * pMainString, char * pSubstring, bool bExtraSpaceRemoval);

/**
* @brief Update backup configuration.
*
* This function updates the backup configuration structure with the current
* Managed WiFi configuration for rollback purposes.
*
* @return None
*/
void updateBackupConfig(void);

/**
* @brief Get Managed WiFi enable state.
*
* This function retrieves the current enable/disable state of Managed WiFi.
*
* @param[out] pWiFiEnable Pointer to receive the enable state (TRUE=enabled, FALSE=disabled).
*
* @return None.
*/
void getManageWiFiEnable(BOOL * pWiFiEnable);

/**
* @brief Publish event notification.
*
* This function publishes configuration change events via the specified
* notification mechanism (RBUS or other).
*
* @param[in] pTr181Param TR-181 parameter name.
* @param[in] pTr181ParamVal TR-181 parameter value.
* @param[in] eEventType Event notification type (RBUS or OTHER_EVENT).
*
* @return None.
*/
void publishEvent(char * pTr181Param, char *pTr181ParamVal, EventType eEventType);

/**
* @brief Confirm Managed WiFi VAP configuration.
*
* This function verifies that the Managed WiFi VAP configuration is correctly
* applied and operational.
*
* @return The status of the operation.
* @retval 0 on success.
* @retval -1 on failure.
*/
int confirmManageWifiVap(void);

/**
* @brief Restore previous PSM values.
*
* This function restores previous PSM values from the backup configuration
* during rollback operations.
*
* @return None.
*/
void restorePreviousPsmValue(void);

/**
* @brief Validate IP address range.
*
* This function validates that an IP address falls within the specified
* start and end address range.
*
* @param[in] pIpAddr IP address to validate.
* @param[in] pStartAddr Start of valid IP address range.
* @param[in] pEndAddr End of valid IP address range.
* @param[out] pErrRetVal Pointer to error structure to receive validation result.
*
* @return The status of the validation.
* @retval 0 if all parameters are valid.
* @retval -1 if invalid.
*/
int validateIpRange(char *pIpAddr, char *pStartAddr, char *pEndAddr, pErr pErrRetVal);

/**
* @brief Validate DHCP lease time.
*
* This function validates that the DHCP lease time is within acceptable limits.
* Lease time is less then 120 seconds is invalid.
*
* @param[in] pLeaseTime Lease time string to validate.
* @param[out] pErrRetVal Pointer to error structure to receive validation result.
*
* @return The status of the validation.
* @retval 0 if all parameters are valid.
* @retval -1 if invalid.
*/
int validateLeaseTime(char *pLeaseTime, pErr pErrRetVal);

/**
* @brief Extract lease time from string.
*
* This function extracts the lease time value from a string bounded by begin and end markers.
*
* @param[in] pBegin Pointer to beginning of lease time string.
* @param[in] pEnd Pointer to end of lease time string.
* @param[out] pLeaseTime Pointer to receive the extracted lease time value.
*
* @return None.
*/
void extractLeaseTime(char *pBegin, char *pEnd, int * pLeaseTime);

/**
* @brief Get Managed WiFi address range details.
*
* This function retrieves the IP address range details for Managed WiFi
* interface including IP address and DHCP pool ranges.
*
* @param[out] pManageWiFiAddrDetails Pointer to structure to receive address details.
*
* @return None.
*/
void getManageWiFiAddrRange(LanDetails_t * pManageWiFiAddrDetails);
#endif