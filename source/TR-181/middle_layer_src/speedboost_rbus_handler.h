/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
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
 *
**********************************************************************/

#include "cosa_rbus_handler_apis.h"

#define UNUSED_PARAMETER(x) (void)(x)

#define ADVERTISEMENT_PVD_ENABLE "Device.RouterAdvertisement.X_RDK_PvD.Enable"
#define ADVERTISEMENT_PVD_H_FLAG "Device.RouterAdvertisement.X_RDK_PvD.H-Flag"
#define ADVERTISEMENT_PVD_DELAY "Device.RouterAdvertisement.X_RDK_PvD.Delay"
#define ADVERTISEMENT_PVD_SEQ_NUMBER "Device.RouterAdvertisement.X_RDK_PvD.SeqNumber"
#define ADVERTISEMENT_PVD_FQDN "Device.RouterAdvertisement.X_RDK_PvD.FQDN"
#define SPEED_BOOST_PORT_RANGE "Device.X_RDK_Speedboost.PortRanges"
#define SPEED_BOOST_NORMAL_PORT_RANGE "Device.X_RDK_Speedboost.NormalPortRange"

// speedboost.h
extern char current_speedboost_value[64];
extern char current_normal_speedboost_value[64];

/**
 * @brief RBUS get handler for PvD (Provisioning Domain) boolean parameters.
 *
 * This function handles RBUS GET requests for Router Advertisement PvD boolean parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t Pvd_GetBoolHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
/**
 * @brief RBUS set handler for PvD boolean parameters.
 *
 * This function handles RBUS SET requests for Router Advertisement PvD boolean parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] prop RBUS property to set.
 * @param[in] opts RBUS set handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t Pvd_SetBoolHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts);
/**
 * @brief RBUS event subscription handler for PvD boolean parameter changes.
 *
 * This function handles subscribe/unsubscribe requests for PvD boolean parameter change events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter.
 * @param[in] interval Polling interval for the event.
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t Pvd_subBoolHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish);
/**
 * @brief RBUS get handler for PvD integer parameters.
 *
 * This function handles RBUS GET requests for Router Advertisement PvD integer parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t Pvd_GetIntHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
/**
 * @brief RBUS set handler for PvD integer parameters.
 *
 * This function handles RBUS SET requests for Router Advertisement PvD integer parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] prop RBUS property to set.
 * @param[in] opts RBUS set handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t Pvd_SetIntHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts);
/**
 * @brief RBUS event subscription handler for PvD integer parameter changes.
 *
 * This function handles subscribe/unsubscribe requests for PvD integer parameter change events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter.
 * @param[in] interval Polling interval for the event.
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t Pvd_subIntHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish);
/**
 * @brief RBUS get handler for PvD string parameters.
 *
 * This function handles RBUS GET requests for Router Advertisement PvD string parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t Pvd_GetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
/**
 * @brief RBUS set handler for PvD string parameters.
 *
 * This function handles RBUS SET requests for Router Advertisement PvD string parameters.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property to set.
 * @param[in] opts RBUS set handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t Pvd_SetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts);
/**
 * @brief RBUS event subscription handler for PvD string parameter changes.
 *
 * This function handles subscribe/unsubscribe requests for PvD string parameter change events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter.
 * @param[in] interval Polling interval for the event.
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t Pvd_subStringHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish);
/**
 * @brief RBUS set handler for SpeedBoost port range string parameters.
 *
 * This function handles RBUS SET requests for SpeedBoost port range configuration strings.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property to set.
 * @param[in] opts RBUS set handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t speed_SetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts);
/**
 * @brief RBUS get handler for SpeedBoost port range string parameters.
 *
 * This function handles RBUS GET requests for SpeedBoost port range configuration strings.
 *
 * @param[in] handle RBUS handle for the request.
 * @param[in] property RBUS property being requested.
 * @param[in] opts RBUS get handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t speed_GetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
/**
 * @brief RBUS event subscription handler for SpeedBoost port range changes.
 *
 * This function handles subscribe/unsubscribe requests for SpeedBoost port range change events.
 *
 * @param[in] handle RBUS handle for the subscription.
 * @param[in] action Subscribe or unsubscribe action.
 * @param[in] eventName Name of the event being subscribed/unsubscribed.
 * @param[in] filter Event filter.
 * @param[in] interval Polling interval for the event.
 * @param[out] autoPublish Whether to automatically publish events.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t speed_subStringHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish);

/**
 * @brief Publishes RBUS event when PvD value changes.
 *
 * This function sends an RBUS event notification when a PvD parameter value changes.
 *
 * @param[in] dm_event Name of the data model event to publish.
 * @param[in] prev_dm_value Pointer to previous data model value.
 * @param[in] dm_value Pointer to new data model value.
 * @param[in] rbus_type RBUS value type.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval non-zero error code on failure.
 */
int Publish_PvdValueOnChange(char *dm_event, void *prev_dm_value, void *dm_value, rbusValueType_t rbus_type);
/**
 * @brief RBUS set handler for SpeedBoost blob data.
 *
 * This function handles RBUS SET requests for SpeedBoost configuration blob data.
 *
 * @param[in] rbusHandle RBUS handle for the request.
 * @param[in] rbusProperty RBUS property to set.
 * @param[in] pRbusHandleOpts RBUS set handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t setBlobHandler(rbusHandle_t rbusHandle, rbusProperty_t rbusProperty, rbusSetHandlerOptions_t* pRbusHandleOpts);

/**
 * @brief RBUS get handler for SpeedBoost blob data.
 *
 * This function handles RBUS GET requests for SpeedBoost configuration blob data.
 *
 * @param[in] rbusHandle RBUS handle for the request.
 * @param[in] rbusProperty RBUS property being requested.
 * @param[in] pRbusHandleOpts RBUS get handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t getBlobHandler(rbusHandle_t rbusHandle, rbusProperty_t rbusProperty, rbusGetHandlerOptions_t* pRbusHandleOpts);

/**
 * @brief RBUS event subscription handler for SpeedBoost client integer count changes.
 *
 * This function handles subscribe/unsubscribe requests for SpeedBoost client count change events.
 *
 * @param[in] rbusHandle RBUS handle for the subscription.
 * @param[in] rbusAction Subscribe or unsubscribe action.
 * @param[in] pEventName Name of the event being subscribed/unsubscribed.
 * @param[in] rbusFilter Event filter.
 * @param[in] i32Interval Polling interval for the event.
 * @param[out] pAutoPublish Whether to automatically publish events.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t clientIntSubHandler(rbusHandle_t rbusHandle, rbusEventSubAction_t rbusAction, const char *pEventName, rbusFilter_t rbusFilter, int32_t i32Interval, bool *pAutoPublish);

/**
 * @brief RBUS get handler for SpeedBoost client information.
 *
 * This function handles RBUS GET requests for SpeedBoost client count and status information.
 *
 * @param[in] rbusHandle RBUS handle for the request.
 * @param[in] rbusProperty RBUS property being requested.
 * @param[in] pRbusHandleOpts RBUS get handler options.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t clientsInfoGetHandler(rbusHandle_t rbusHandle, rbusProperty_t rbusProperty, rbusGetHandlerOptions_t* pRbusHandleOpts);

/**
 * @brief RBUS event subscription handler for SpeedBoost client string list changes.
 *
 * This function handles subscribe/unsubscribe requests for SpeedBoost active client list change events.
 *
 * @param[in] rbusHandle RBUS handle for the subscription.
 * @param[in] rbusAction Subscribe or unsubscribe action.
 * @param[in] pEventName Name of the event being subscribed/unsubscribed.
 * @param[in] rbusFilter Event filter.
 * @param[in] i32Interval Polling interval for the event.
 * @param[out] pAutoPublish Whether to automatically publish events.
 *
 * @return The status of the operation.
 * @retval RBUS_ERROR_SUCCESS on success.
 * @retval RBUS error code on failure.
 */
rbusError_t clientStringSubHandler(rbusHandle_t rbusHandle, rbusEventSubAction_t rbusAction, const char *pEventName, rbusFilter_t rbusFilter, int32_t i32Interval, bool *pAutoPublish);

/**
 * @brief Publishes RBUS event if there are active subscribers.
 *
 * This function checks for active subscribers and publishes the SpeedBoost event if any exist.
 *
 * @param[in] pEventName Name of the event to publish.
 * @param[in] pPrevVal Pointer to previous value.
 * @param[in] pCurrVal Pointer to current value.
 *
 * @return void
 */
void publishEventChangeIfSubscribed(const char *pEventName, void *pPrevVal, void *pCurrVal);
