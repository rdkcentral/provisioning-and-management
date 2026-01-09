/*
 * If not stated otherwise in this file or this component's Licenses.txt file
 * the following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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

#ifndef  RBUS_HANDLER_APIS_H
#define  RBUS_HANDLER_APIS_H



#include <stdbool.h>
#include <rbus/rbus.h>
#include <pthread.h>

#if defined (USE_REMOTE_DEBUGGER)
#include "rrdInterface.h"
#endif

//#define RBUS_COMPONENT_NAME	"RbusPandMSsp"
#define RBUS_COMPONENT_NAME	"CcspPandMSsp"

#define DEVCTRL_NET_MODE_TR181	"Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode"
#if defined (RBUS_WAN_IP)
#define PRIMARY_WAN_IP_ADDRESS "Device.DeviceInfo.X_COMCAST-COM_WAN_IP"
#define PRIMARY_WAN_IPv6_ADDRESS "Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6"
#endif /*RBUS_WAN_IP*/


#define  ARRAY_SZ(x) (sizeof(x) / sizeof((x)[0]))
#if defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED)
typedef struct
_DeviceControl_Net_Mode_
{
    ULONG DevCtrlNetMode;

} DeviceControl_Net_Mode;

/**
 * @brief RBUS get handler for unsigned long parameter values.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t getUlongHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);

/**
 * @brief RBUS set handler for unsigned long parameter values.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] prop RBUS property to set.
 * @param[in] opts RBUS set handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t setUlongHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts);

/**
 * @brief RBUS event subscription handler for device control events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter.
 * @param[in] interval Polling interval for the event.
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t eventDevctrlSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish);

/**
 * @brief Initializes network mode for device control.
 *
 * @return status of operation
 * @retval true on success
 * @retval false on failure.
 */
bool initNetMode();

/**
 * @brief Sends an RBUS update event with new and old values.
 *
 * @param[in] event_name Name of the event to publish.
 * @param[in] eventNewData Pointer to new value data.
 * @param[in] eventOldData Pointer to old value data.
 * @param[in] rbus_type RBUS value type.
 *
 * @return  status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t sendUpdateEvent(char* event_name , void* eventNewData, void* eventOldData, rbusValueType_t rbus_type);

/**
 * @brief Publishes device control network mode change event via RBUS.
 *
 * @param[in] new_val New network mode value.
 * @param[in] old_val Previous network mode value.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t publishDevCtrlNetMode(uint32_t new_val, uint32_t old_val);

/**
 * @brief publish DevCtrlNetMode during PandM initialization
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */

int publishInitialDevCtrlVal();

/**
 * @brief Publishes DevCtrlNetMode during PandM initialization
 *
 * @return status of operation
 * @retval 0 on success
 * @retval -1 on failure.
 */

bool PAM_Rbus_SyseventInit();

/**
 * @brief Initializes RBUS sysevent integration for PAM component.
 *
 * @return status of operation
 * @retval true on success
 * @retval false on failure.
 */

#endif
#if defined (WIFI_MANAGE_SUPPORTED)
/**
 * @brief RBUS get handler for string parameter values.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t getStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t *opts);

/**
 * @brief RBUS set handler for string parameter values.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] prop RBUS property to set.
 * @param[in] opts RBUS set handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t setStringHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts);

/**
 * @brief RBUS event subscription handler for managed WiFi bridge events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter.
 * @param[in] interval Polling interval for the event.
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t eventManageWiFiBridgeSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish);

/**
 * @brief RBUS get handler for boolean parameter values.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return status of operation.
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t getBoolHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t *opts);

/**
 * @brief RBUS event subscription handler for managed WiFi enable events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter.
 * @param[in] interval Polling interval for the event.
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t eventManageWiFiEnableSubHander(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish);

/**
 * @brief RBUS event subscription handler for managed WiFi interface events.
 *
 * This function handles subscribe/unsubscribe requests for managed WiFi interface configuration events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter .
 * @param[in] interval Polling interval for the event .
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t eventManageWiFiInterfaceSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish);
#endif

#if defined (AMENITIES_NETWORK_ENABLED)
/**
 * @brief RBUS get handler for amenities network string parameters.
 *
 * @param[in] rbusHandle RBUS handle for the request.
 * @param[in] rbusProp RBUS property being requested.
 * @param[in] rbusOpts RBUS get handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t getStringAmenityHandler(rbusHandle_t rbusHandle, rbusProperty_t rbusProp, rbusGetHandlerOptions_t* rbusOpts);

/**
 * @brief RBUS set handler for amenities network string parameters.
 *
 * @param[in] rbusHandle RBUS handle for the request.
 * @param[in] rbusProp RBUS property to set.
 * @param[in] rbusOpts RBUS set handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t setStringAmenityHandler(rbusHandle_t rbusHandle, rbusProperty_t rbusProp, rbusSetHandlerOptions_t* rbusOpts);

/**
 * @brief RBUS handler to remove a row from a table.
 *
 * @param[in] rbusHandle RBUS handle for the request.
 * @param[in] pRowName Name of the row/instance to remove.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t removeRowInTableHander (rbusHandle_t rbusHandle, char const *pRowName);
/**
 * @brief RBUS handler to add a new row to a table.
 *
 * @param[in] rbusHandle RBUS handle for the request.
 * @param[in] pTableName Name of the table to add row to.
 * @param[in] pAliasName Alias name for the new instance.
 * @param[out] pInstanceNum Pointer to receive the new instance number.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t addRowInTableHandler (rbusHandle_t rbusHandle, char const *pTableName, char const * pAliasName, uint32_t *pInstanceNum);

#endif
#if defined (USE_REMOTE_DEBUGGER)
/**
 * @brief RBUS get handler for Remote Debugger string parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t RRD_GetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);

/**
 * @brief RBUS set handler for Remote Debugger string parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property to set.
 * @param[in] opts RBUS set handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t RRD_SetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts);

/**
 * @brief RBUS get handler for Remote Debugger webconfig string parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t RRDWebCfg_GetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);

/**
 * @brief RBUS set handler for Remote Debugger webconfig string parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property to set.
 * @param[in] opts RBUS set handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t RRDWebCfg_SetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts);

/**
 * @brief RBUS set handler for Remote Debugger boolean parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property to set.
 * @param[in] opts RBUS set handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t RRD_SetBoolHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts);
#endif

#if defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED) ||  defined(RBUS_BUILD_FLAG_ENABLE) || defined (_HUB4_PRODUCT_REQ_) || defined (_PLATFORM_RASPBERRYPI_) || defined (RBUS_WAN_IP)
/**
 * @brief Initializes RBUS handlers for device control parameters.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t devCtrlRbusInit();
#endif

#if defined (RBUS_WAN_IP)
typedef struct {
    char* event_name;
    char* new_val;
    char* old_val;
} publish_wanip_t;

/**
 * @brief Frees memory allocated for WAN IP publish structure.
 *
 * @param[in] param Pointer to publish_wanip_t structure to free.
 *
 * @return None
 */
void free_publish_wanip_struct(publish_wanip_t *param);

/**
 * @brief Publishes WAN IP address change event via RBUS.
 *
 * @param[in] event_name Name of the RBUS event to publish.
 * @param[in] new_val New WAN IP address value.
 * @param[in] old_val Previous WAN IP address value.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t publishWanIpAddr(char* event_name, char* new_val, char* old_val);

/**
 * @brief Extracts parameter name from a full TR-181 path.
 *
 * @param[in] path Full TR-181 parameter path .
 *
 * @return Pointer to parameter name string
 * @retval not NULL pointer to parameter name
 * @retval NULL if path is invalid.
 */
char const* GetParamName(char const* path);

/**
 * @brief RBUS get handler for WAN IP address string parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t getStringHandlerWANIP_RBUS(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t *opts);

/**
 * @brief RBUS event subscription handler for WAN IP address events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter.
 * @param[in] interval Polling interval for the event .
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t eventWANIPSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish);
/**
 * @brief RBUS event subscription handler for WAN IPv6 address events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter.
 * @param[in] interval Polling interval for the event.
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return status of operation
 * @retval RBUS_ERROR_SUCCESS on success
 * @retval RBUS error code on failure.
 */
rbusError_t eventWANIPv6SubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish);

#endif
#endif
