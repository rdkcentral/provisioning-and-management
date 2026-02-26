/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2026 RDK Management
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

#include <pthread.h>
#include <rbus/rbus.h>
#include "cosa_drg_common.h"
#include "ccsp_trace.h"
#include "telemetry_busmessage_sender.h"

#define WIFI_IGNITE_LINK_QUALITY_STATUS "Device.WiFi.Ignite.1.LinkQualityStatus"
#define LINK_QUALITY_VALUE_SERVICABLE "Serviceable"
#define LINK_QUALITY_VALUE_NON_SERVICEABLE "Non-serviceable"

/**
 * @brief RBUS event handler for handling WiFi Ignite Link Quality Status events
 *
 * This function is called when an RBUS event is received. It checks if the event is related to WiFi Ignite Link Quality Status and takes appropriate actions based on the value of the link quality status.
 *
 * @param rbus The RBUS handle (unused in this handler)
 * @param pRbusEvent Pointer to the RBUS event data
 * @param pRbusSubscription Pointer to the RBUS subscription data (unused in this handler)
 * @return RBUS_ERROR_SUCCESS on successful handling of the event, or an appropriate error code on failure
*/
rbusError_t rbusEventHandler(rbusHandle_t rbus, rbusEvent_t const* pRbusEvent, rbusEventSubscription_t* pRbusSubscription)
{
    (void)rbus; // Unused parameter
    (void)pRbusSubscription; // Unused parameter
    if (NULL == pRbusEvent || NULL == pRbusEvent->name )
    {
        CcspTraceError(("%s: Invalid RBUS event data\n", __FUNCTION__));
        return RBUS_ERROR_INVALID_INPUT;
    }
    CcspTraceInfo(("%s: Received RBUS event %s\n", __FUNCTION__, pRbusEvent->name));
    if (NULL == pRbusEvent->data)
    {
        CcspTraceWarning(("%s: RBUS event %s has no data\n", __FUNCTION__, pRbusEvent->name));
        return RBUS_ERROR_SUCCESS;
    }
    if (0 == strcmp(pRbusEvent->name, WIFI_IGNITE_LINK_QUALITY_STATUS))
    {
        CcspTraceInfo(("%s: Handling event %s\n", __FUNCTION__, WIFI_IGNITE_LINK_QUALITY_STATUS));
        rbusValue_t linkQualityValue = rbusObject_GetValue(pRbusEvent->data, NULL);
        if (NULL != linkQualityValue)
        {
            rbusValueType_t valueType = rbusValue_GetType(linkQualityValue);
            if (valueType == RBUS_STRING)
            {
                const char *pStrVal = rbusValue_GetString(linkQualityValue, NULL);
                CcspTraceInfo(("%s: Link Quality Status: %s\n", __FUNCTION__, pStrVal ? pStrVal : "NULL"));
                if (NULL != pStrVal)
                {
                    if (0 == strcasecmp(pStrVal, LINK_QUALITY_VALUE_SERVICABLE))
                    {
                        CcspTraceInfo(("%s: Link quality is Serviceable\n", __FUNCTION__));
                        t2_event_d("LinkQualityServiceble_Lan2WanAllowed", 1);
                        if (0 != commonSyseventSet("lan_wan_forwarding_enabled", "1"))
                        {
                            CcspTraceError(("%s: Failed to set sysevent lan_wan_forwarding_enabled to 1\n", __FUNCTION__));
                        }
                        if (0 != commonSyseventSet("firewall-restart", ""))
                        {
                            CcspTraceError(("%s: Failed to set sysevent firewall-restart\n", __FUNCTION__));
                        }
                    }
                    else if (0 == strcasecmp(pStrVal, LINK_QUALITY_VALUE_NON_SERVICEABLE))
                    {
                        CcspTraceInfo(("%s: Link quality is Non-serviceable\n", __FUNCTION__));
                        t2_event_d("LinkQualityNonServiceble_Lan2WanBlocked", 1);
                        if (0 != commonSyseventSet("lan_wan_forwarding_enabled", "0"))
                        {
                            CcspTraceError(("%s: Failed to set sysevent lan_wan_forwarding_enabled to 0\n", __FUNCTION__));
                        }
                        if (0 != commonSyseventSet("firewall-restart", ""))
                        {
                            CcspTraceError(("%s: Failed to set sysevent firewall-restart\n", __FUNCTION__));
                        }
                    }
                    else
                    {
                        //any other/unset/unknown → default allow
                        CcspTraceWarning(("%s: Unrecognized link quality value %s\n", __FUNCTION__, pStrVal));
                    }
                }
                else
                {
                    CcspTraceWarning(("%s: Link quality value is NULL\n", __FUNCTION__));
                }
            }
            else
            {
                CcspTraceWarning(("%s: Unexpected RBUS value type %d for event %s\n", __FUNCTION__, valueType, WIFI_IGNITE_LINK_QUALITY_STATUS));
            }
        }
        else
        {
            CcspTraceWarning(("%s: No value found in event data for event %s\n", __FUNCTION__, WIFI_IGNITE_LINK_QUALITY_STATUS));
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unrecognized event %s\n", __FUNCTION__, pRbusEvent->name));
    }
    return RBUS_ERROR_SUCCESS;
}

/**
 * @brief Thread function for subscribing to RBUS events
 *
 * This function runs in a separate thread and attempts to subscribe to the WiFi Ignite Link Quality Status event. If the subscription fails, it retries every 5 seconds until it succeeds.
 *
 * @param pArg Pointer to the RBUS handle passed as an argument
 * @return NULL on thread exit
*/
void *rbusSubscriptionThread(void *pArg)
{
    if (NULL == pArg)
    {
        CcspTraceError(("%s: Invalid argument for RBUS subscription thread\n", __FUNCTION__));
        return NULL;
    }
    rbusHandle_t rbusHandle = *(rbusHandle_t *)pArg;
    while (1)
    {
        rbusError_t rbusRet = rbusEvent_Subscribe(rbusHandle, WIFI_IGNITE_LINK_QUALITY_STATUS,(void*)&rbusEventHandler, NULL,0);
        if (rbusRet != RBUS_ERROR_SUCCESS && rbusRet != RBUS_ERROR_SUBSCRIPTION_ALREADY_EXIST)
        {
            CcspTraceError(("%s: rbus_event_subscribe failed for event %s with error code %d\n", __FUNCTION__, WIFI_IGNITE_LINK_QUALITY_STATUS, rbusRet));
            CcspTraceError(("%s: Retrying event subscription after 5 seconds...\n", __FUNCTION__));
            sleep(5);
        }
        else
        {
            CcspTraceInfo(("%s: rbus_event_subscribe successful for event %s\n", __FUNCTION__, WIFI_IGNITE_LINK_QUALITY_STATUS));
            break;
        }
    }
    pthread_exit(NULL);
}

/**
 * @brief Function to subscribe for RBUS events
 *
 * This function creates a separate thread to handle the subscription to RBUS events.
 *
 * @param pRbusHandle Pointer to the RBUS handle to be used for subscribing to events
 */
void subscribeForRbusEvents(rbusHandle_t * pRbusHandle)
{
    if (NULL == pRbusHandle || NULL == *pRbusHandle)
    {
        CcspTraceError(("%s: Invalid RBUS handle\n", __FUNCTION__));
        return;
    }
    pthread_t rbusSubscriptionThreadId;
    if (pthread_create(&rbusSubscriptionThreadId, NULL, rbusSubscriptionThread, pRbusHandle) != 0)
    {
        CcspTraceError(("%s: Failed to create RBUS event subscription thread\n", __FUNCTION__));
        return;
    }
    pthread_detach(rbusSubscriptionThreadId);
}
