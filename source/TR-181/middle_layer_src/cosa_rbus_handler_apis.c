/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2018 RDK Management
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sysevent/sysevent.h>
#include "ccsp_trace.h"
#include "cosa_rbus_handler_apis.h"
#include "ccsp_psm_helper.h"
#include "safec_lib_common.h"
#include "cosa_dhcpv6_apis.h"
#include "syscfg/syscfg.h"
#include "secure_wrapper.h"

#if defined (RBUS_WAN_IP)
#include "cosa_deviceinfo_dml.h"
#endif /*RBUS_WAN_IP*/
#if defined (WIFI_MANAGE_SUPPORTED)
#include "cosa_managedwifi_webconfig_apis.h"
unsigned int gManageWiFiBridgeSubscribersCount = 0;
unsigned int gManageWiFiEnableSubscribersCount = 0;
unsigned int gManageWiFiInterfaceSubscribersCount = 0;
#endif /*WIFI_MANAGE_SUPPORTED*/
#if defined (RBUS_WAN_IP)
unsigned int gSubscribersCount_IPv4 = 0;
unsigned int gSubscribersCount_IPv6 = 0;
#endif /*RBUS_WAN_IP*/

#if defined (SPEED_BOOST_SUPPORTED)
#include "speedboost_rbus_handler.h"
#include "speedboost_webconfig_apis.h"
#include "speedboost_dml.h"
#include "speedboost_apis.h"
#endif /*SPEED_BOOST_SUPPORTED*/

rbusHandle_t handle;

#define NUM_OF_RBUS_PARAMS sizeof(devCtrlRbusDataElements)/sizeof(devCtrlRbusDataElements[0])

#if  defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED)
DeviceControl_Net_Mode deviceControl_Net_Mode;


//PSM
extern ANSC_HANDLE g_MessageBusHandle;
extern char g_Subsystem[32];
extern void* g_pDslhDmlAgent;

unsigned int gSubscribersCount = 0;

static int sysevent_fd 	  = -1;
static token_t sysevent_token = 0;
#endif

#if defined (USE_REMOTE_DEBUGGER)
char RRDIssueType[256];
char RRDWebCfgData[256];
bool RRD_BoolValue;
#endif
/***********************************************************************

  Data Elements declaration:

 ***********************************************************************/
rbusDataElement_t devCtrlRbusDataElements[] = {

#if  defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED)
	{DEVCTRL_NET_MODE_TR181, RBUS_ELEMENT_TYPE_EVENT, {getUlongHandler, setUlongHandler, NULL, NULL, eventDevctrlSubHandler, NULL}},
#endif
#if defined (AMENITIES_NETWORK_ENABLED)
    {"Device.LAN.Bridge.{i}", RBUS_ELEMENT_TYPE_TABLE, {NULL, NULL, addRowInTableHandler, removeRowInTableHander, NULL,NULL}},
    {"Device.LAN.Bridge.{i}.VapName", RBUS_ELEMENT_TYPE_EVENT, {getStringAmenityHandler, setStringAmenityHandler, NULL, NULL, NULL, NULL}},
    {"Device.LAN.Bridge.{i}.Name", RBUS_ELEMENT_TYPE_EVENT, {getStringAmenityHandler, setStringAmenityHandler, NULL, NULL, NULL, NULL}},
    {"Device.LAN.Bridge.{i}.WiFiInterfaces", RBUS_ELEMENT_TYPE_EVENT, {getStringAmenityHandler, setStringAmenityHandler, NULL, NULL, NULL, NULL}},
#elif defined (WIFI_MANAGE_SUPPORTED)
    {MANAGE_WIFI_LAN_BRIDGE, RBUS_ELEMENT_TYPE_EVENT, {getStringHandler, setStringHandler, NULL, NULL, eventManageWiFiBridgeSubHandler,NULL}},
    {MANAGE_WIFI_ENABLE, RBUS_ELEMENT_TYPE_EVENT, {getBoolHandler, NULL, NULL, NULL, eventManageWiFiEnableSubHander, NULL}},
    {MANAGE_WIFI_INTERFACES, RBUS_ELEMENT_TYPE_EVENT, {getStringHandler, setStringHandler, NULL, NULL, eventManageWiFiInterfaceSubHandler,NULL}},
#endif
#if defined (RBUS_WAN_IP)
    {PRIMARY_WAN_IP_ADDRESS, RBUS_ELEMENT_TYPE_EVENT, {getStringHandlerWANIP_RBUS, NULL, NULL, NULL, eventWANIPSubHandler, NULL}},
    {PRIMARY_WAN_IPv6_ADDRESS, RBUS_ELEMENT_TYPE_EVENT, {getStringHandlerWANIP_RBUS, NULL, NULL, NULL, eventWANIPSubHandler, NULL}},
#endif
#if defined (SPEED_BOOST_SUPPORTED)
    {ADVERTISEMENT_PVD_ENABLE, RBUS_ELEMENT_TYPE_PROPERTY, {Pvd_GetBoolHandler, Pvd_SetBoolHandler, NULL, NULL, Pvd_subBoolHandler, NULL}},
    {ADVERTISEMENT_PVD_H_FLAG, RBUS_ELEMENT_TYPE_PROPERTY, {Pvd_GetBoolHandler, Pvd_SetBoolHandler, NULL, NULL, Pvd_subBoolHandler, NULL}},
    {ADVERTISEMENT_PVD_DELAY, RBUS_ELEMENT_TYPE_PROPERTY, {Pvd_GetIntHandler, Pvd_SetIntHandler, NULL, NULL, Pvd_subIntHandler, NULL}},
    {ADVERTISEMENT_PVD_SEQ_NUMBER, RBUS_ELEMENT_TYPE_PROPERTY, {Pvd_GetIntHandler, Pvd_SetIntHandler, NULL, NULL, Pvd_subIntHandler, NULL}},
    {ADVERTISEMENT_PVD_FQDN, RBUS_ELEMENT_TYPE_PROPERTY, {Pvd_GetStringHandler, Pvd_SetStringHandler, NULL, NULL, Pvd_subStringHandler, NULL}},
    {SPEEDBOOST_BLOB_DATA, RBUS_ELEMENT_TYPE_EVENT, {getBlobHandler, setBlobHandler, NULL, NULL, NULL, NULL}},
    {SPEEDBOOST_NUMBER_OF_CONFIGURED_DEVICES, RBUS_ELEMENT_TYPE_EVENT, {clientsInfoGetHandler, NULL, NULL, NULL, clientIntSubHandler, NULL}},
    {SPEEDBOOST_NUMBER_OF_ELIGIBLE_DEVICES, RBUS_ELEMENT_TYPE_EVENT, {clientsInfoGetHandler, NULL, NULL, NULL, clientIntSubHandler, NULL}},
    {SPEEDBOOST_CURRENT_ACTIVE_DEVICE_LIST, RBUS_ELEMENT_TYPE_EVENT, {clientsInfoGetHandler, NULL, NULL, NULL, clientStringSubHandler, NULL}},
    {SPEED_BOOST_PORT_RANGE, RBUS_ELEMENT_TYPE_EVENT, {speed_GetStringHandler, speed_SetStringHandler, NULL, NULL, speed_subStringHandler, NULL}},
    {SPEED_BOOST_NORMAL_PORT_RANGE, RBUS_ELEMENT_TYPE_EVENT, {speed_GetStringHandler, speed_SetStringHandler, NULL, NULL, speed_subStringHandler, NULL}},
#endif /*SPEED_BOOST_SUPPORTED*/
#if defined (USE_REMOTE_DEBUGGER)  
    {RRD_SET_ISSUE_EVENT, RBUS_ELEMENT_TYPE_EVENT, {RRD_GetStringHandler, RRD_SetStringHandler, NULL, NULL, NULL, NULL}},
    {RRD_WEBCFG_ISSUE_EVENT, RBUS_ELEMENT_TYPE_EVENT, {RRDWebCfg_GetStringHandler, RRDWebCfg_SetStringHandler, NULL, NULL, NULL, NULL}},
    {RDM_DOWNLOAD_EVENT,RBUS_ELEMENT_TYPE_EVENT, {NULL, RRD_SetBoolHandler, NULL, NULL, NULL, NULL}},
#endif  
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#if  defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED)

/***********************************************************************

  Get Handler APIs for objects of type RBUS_Ulong:

 ***********************************************************************/
rbusError_t getUlongHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t *opts)
{	
    pthread_mutex_lock(&mutex);
    char const* name = rbusProperty_GetName(property);
    (void)handle;
    (void)opts;
    rbusValue_t value;
    rbusValue_Init(&value);

    if (strcmp(name, DEVCTRL_NET_MODE_TR181) == 0)
    {
#if defined (RDKB_EXTENDER_ENABLED)
    char buf[ 8 ] = { 0 };
    if( 0 == syscfg_get( NULL, "Device_Mode", buf, sizeof( buf ) ) )
    {
        deviceControl_Net_Mode.DevCtrlNetMode = atoi(buf);
        rbusValue_SetUInt32(value, deviceControl_Net_Mode.DevCtrlNetMode);
    }
    else
    {
        CcspTraceError(("syscfg_get failed to retrieve  device networking mode\n")); 
        deviceControl_Net_Mode.DevCtrlNetMode = 0;
        CcspTraceWarning(("Returning '%lu' as NetworkingMode\n", deviceControl_Net_Mode.DevCtrlNetMode));
        rbusValue_SetUInt32(value, deviceControl_Net_Mode.DevCtrlNetMode);
    }     
#else
        //setting value as router for non-xle devices	
        deviceControl_Net_Mode.DevCtrlNetMode = 0;
        CcspTraceWarning(("Getting Device Networking Mode value for non-xle devices, new value = '%lu'\n", deviceControl_Net_Mode.DevCtrlNetMode));
        rbusValue_SetUInt32(value, deviceControl_Net_Mode.DevCtrlNetMode);
#endif
    }
    else
    {	
        CcspTraceWarning(("Device Networking Mode rbus get handler invalid input\n"));
        pthread_mutex_unlock(&mutex);
        return RBUS_ERROR_INVALID_INPUT;
    }
	
    rbusProperty_SetValue(property, value);
    rbusValue_Release(value);
    pthread_mutex_unlock(&mutex);

    return RBUS_ERROR_SUCCESS;
}

/*************************Set hanlder for local data*****************************************/
rbusError_t setUlongHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts)
{
#if defined (RDKB_EXTENDER_ENABLED)
    pthread_mutex_lock(&mutex);
    (void)handle;
    (void)opts;
    char const* name = rbusProperty_GetName(prop);
    rbusValue_t value = rbusProperty_GetValue(prop);
    rbusValueType_t type = rbusValue_GetType(value);
    rbusError_t ret = RBUS_ERROR_SUCCESS;
    char strValue[3] = {0};
    errno_t  rc = -1;
    uint32_t rVal = 0;
    char buf[8] = {0};
   
    //Here deviceControl_Net_Mode is global

    if(strcmp(name, DEVCTRL_NET_MODE_TR181) == 0)
    {
        if (type != RBUS_UINT32)
        {
            CcspTraceWarning(("Device Networking Mode input value is of invalid type\n"));
            pthread_mutex_unlock(&mutex);
            return RBUS_ERROR_INVALID_INPUT;
        }

        //Getting value from rbus set
        rVal = rbusValue_GetUInt32(value);
        if (rVal > 1) {
            CcspTraceError(("Invalid set value for the parameter '%s'\n", DEVCTRL_NET_MODE_TR181));
            return RBUS_ERROR_INVALID_INPUT;
        }

        /* Updating the Device Networking Mode in PSM database over sysevent */
        rc = sprintf_s(strValue, sizeof(strValue),"%lu", rVal);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        if (0 > sysevent_fd)
        {
            CcspTraceError(("Failed to execute sysevent_set. sysevent_fd have no value:'%d'\n", sysevent_fd));
            return RBUS_ERROR_BUS_ERROR;
        }
		
        // Fetch old value    
        uint32_t oldDevCtrlNetMode = deviceControl_Net_Mode.DevCtrlNetMode;
        //update DevCtrlNetMode with new value and publish
        if (oldDevCtrlNetMode != rVal) 	
        {
            snprintf(buf,sizeof(buf),"%d",rVal);
        
            //Setting Device Mode
            if (syscfg_set(NULL, "Device_Mode", buf) != 0)
            {
                CcspTraceInfo(("\n Device_Mode set syscfg failed\n"));       
            }
            else
            {
                if (syscfg_commit() != 0)
                {
                    CcspTraceInfo(("\nDevice_Mode syscfg_commit failed\n"));
                }
                else
                {
                    if(sysevent_set(sysevent_fd, sysevent_token, "DeviceMode", strValue, 0) != 0)
                    {
                        CcspTraceError(("Failed to execute sysevent_set from %s:%d\n", __FUNCTION__, __LINE__));
                        return RBUS_ERROR_BUS_ERROR;
                    }
                    CcspTraceInfo(("sysevent_set execution success.\n"));
                    deviceControl_Net_Mode.DevCtrlNetMode = rVal;
                    ret = publishDevCtrlNetMode(rVal, oldDevCtrlNetMode);
                    if (ret != RBUS_ERROR_SUCCESS)
                    {
                        CcspTraceError(("%s-%d: Failed to update and publish device mode value\n", __FUNCTION__, __LINE__));
                        return ret;
                    }
                    configureIpv6Route(rVal);
                }
            }
        }
    }    
    pthread_mutex_unlock(&mutex);
    return RBUS_ERROR_SUCCESS;
#else	
    (void)handle;
    (void)opts;
    (void)prop;
    CcspTraceError(("Set handler not supported for this device.\n"));
    return RBUS_ERROR_BUS_ERROR;
#endif
}

/***********************************************************************

  Event subscribe handler API for objects:

 ***********************************************************************/
rbusError_t eventDevctrlSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish)
{
	(void)handle;
	(void)filter;
	(void)interval;

	*autoPublish = false;
  
  	CcspTraceWarning(("Event devctrl sub handler called.\n"));

	if (strcmp(eventName, DEVCTRL_NET_MODE_TR181) == 0)
	{
		if (action == RBUS_EVENT_ACTION_SUBSCRIBE)
		{
			gSubscribersCount += 1;
		}
		else
		{
			if (gSubscribersCount > 0)
			{
				gSubscribersCount -= 1;
			}
		}
		CcspTraceWarning(("Subscribers count changed, new value=%d\n", gSubscribersCount));
	}
	else
	{
		CcspTraceWarning(("provider: eventSubHandler unexpected eventName %s\n", eventName));
	}
	return RBUS_ERROR_SUCCESS;
}


/*******************************************************************************

  initNetMode(): Initialize deviceControl_Net_Mode struct with default values

 ********************************************************************************/
bool initNetMode()
{
		char strValue[3];
        errno_t  rc = -1;
        ULONG uValue = 1;

        /* Updating the Device Networking Mode in PSM database over sysevent */
        rc = sprintf_s(strValue, sizeof(strValue),"%lu",uValue);
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }
        
		//sysevent set for default value
		if (0 > sysevent_fd)
		{
			CcspTraceError(("Failed to execute sysevent_set. sysevent_fd have no value:'%d'\n", sysevent_fd));
			return false;
		}
		if(sysevent_set(sysevent_fd, sysevent_token, "DeviceMode", strValue, 0) != 0)
		{
			CcspTraceError(("Failed to execute sysevent_set from %s:%d\n", __FUNCTION__, __LINE__));
			return false;
		}
		CcspTraceInfo(("sysevent_set execution success.\n"));
		CcspTraceWarning(("Initialized Device Networking Mode with default values.\n"));
		return true;
}

/*******************************************************************************

  publishDevCtrlNetMode(): publish DevCtrlNetMode event after event value gets updated

 ********************************************************************************/
 
rbusError_t publishDevCtrlNetMode(uint32_t new_val, uint32_t old_val)
{
	rbusError_t ret = RBUS_ERROR_SUCCESS;
	CcspTraceInfo(("Publishing Device Networking Mode with updated value=%d\n", new_val));
	if (gSubscribersCount > 0)
	{
		ret = sendUpdateEvent(DEVCTRL_NET_MODE_TR181, (void*)&new_val, (void*)&old_val, RBUS_INT32);
		if(ret == RBUS_ERROR_SUCCESS) {
			CcspTraceInfo(("Published Device Networking Mode with updated value.\n"));
		}
	}
	return ret;
}

bool PAM_Rbus_SyseventInit()
{		
	if (0 > sysevent_fd)
	{
		if ((sysevent_fd = sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "PAM_Rbus", &sysevent_token)) < 0)
		{
			CcspTraceError(("Failed to open sysevent.\n"));
			return false;
		}
		CcspTraceInfo(("sysevent_open success.\n"));
		return true;
	}
	CcspTraceError(("Failed to open sysevent. sysevent_fd already have a value '%d'\n", sysevent_fd));
	return false;
}
#endif

/*******************************************************************************

  sendUpdateEvent(): publish event after event value gets updated

 ********************************************************************************/
rbusError_t sendUpdateEvent(char* event_name , void* eventNewData, void* eventOldData, rbusValueType_t rbus_type)
{
    rbusEvent_t event={0};
    rbusObject_t data;
    rbusValue_t value, oldVal, byVal;
    rbusError_t ret = RBUS_ERROR_BUS_ERROR;

    CcspTraceInfo(("sendUpdateEvent event_name:%s, eventNewData:%s, eventOldData:%s, rbus_type: %d\n", event_name, (char*)eventNewData, (char*)eventOldData, rbus_type));

    if(event_name == NULL || eventNewData == NULL){
        CcspTraceError(("%s %d - Failed publishing\n", __FUNCTION__, __LINE__));
        return ret;
    }

    rbusValue_Init(&value);
    rbusValue_Init(&oldVal);

    switch(rbus_type){
        case RBUS_INT32:
            //initialize and set new value for the event
            rbusValue_SetUInt32(value, *((int*)eventNewData));
            //initialize and set previous value for the event
            rbusValue_SetUInt32(oldVal, *((int*)eventOldData));
            break;
        case RBUS_STRING:
            //initialize and set new value for the event
            rbusValue_SetString(value, (char*)eventNewData);
            //initialize and set previous value for the event
            rbusValue_SetString(oldVal, (char*)eventOldData);
            break;
        case RBUS_BOOLEAN:
            rbusValue_SetBoolean(oldVal, *((bool*)eventOldData));
            rbusValue_SetBoolean(value, *((bool*)eventNewData));
            break;
        default:
            //initialize and set new value for the event
            rbusValue_Release(value);
            rbusValue_Release(oldVal);
            return RBUS_ERROR_BUS_ERROR;
    }

    //initialize and set responsible component name for value change
    rbusValue_Init(&byVal);
    rbusValue_SetString(byVal, RBUS_COMPONENT_NAME);
    //initialize and set rbusObject with desired values
    rbusObject_Init(&data, NULL);
    rbusObject_SetValue(data, "value", value);
    rbusObject_SetValue(data, "oldValue", oldVal);
    rbusObject_SetValue(data, "by", byVal);
    //set data to be transferred
    event.name = event_name;
    event.data = data;
    event.type = RBUS_EVENT_VALUE_CHANGED;
    //publish the event

    if (NULL == handle)
    {
        CcspTraceError(("%s: Rbus handler is NULL\n", __FUNCTION__));
        return ret;
    }
    if (NULL == event.name || NULL == event.data)
    {
        CcspTraceError(("%s: Rbus event is NULL\n", __FUNCTION__));
        return ret;
    }

    ret = rbusEvent_Publish(handle, &event);

    if(ret != RBUS_ERROR_SUCCESS) {
            if (ret == RBUS_ERROR_NOSUBSCRIBERS)
            {
                CcspTraceError(("%s: No subscribers found for event: %s\n", __FUNCTION__, event_name));
            }
            else
            {
                CcspTraceError(("rbusEvent_Publish: Unable to Publish event data %s  rbus error code : %s\n",event_name, rbusError_ToString(ret)));
            }
    }
    else{
        CcspTraceWarning(("rbusEvent_Publish for %s success: %d\n", event_name, ret));
    }

    //release all initialized rbusValue objects
    rbusValue_Release(value);
    rbusValue_Release(oldVal);
    rbusValue_Release(byVal);
    rbusObject_Release(data);
    return ret;
}

#if defined (RBUS_WAN_IP)

void free_publish_wanip_struct(publish_wanip_t *param)
{
    if(param != NULL)
    {
        if(param->event_name != NULL)
        {
            free(param->event_name);
            param->event_name = NULL;
        }
        if(param->new_val != NULL)
        {
            free(param->new_val);
            param->new_val = NULL;
        }
        if(param->old_val != NULL)
        {
            free(param->old_val);
            param->old_val = NULL;
        }
        free(param);
        param = NULL;
    }
}

#if defined (USE_REMOTE_DEBUGGER)
rbusError_t RRDWebCfg_GetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts)
{
    (void) handle;
    (void) opts;
    char const* propertyName;
    CcspTraceInfo(("Enter %s \n", __FUNCTION__));

    propertyName = rbusProperty_GetName(property);
    if(propertyName)
    {
        CcspTraceInfo(("[%s]: Called for %s\n", __FUNCTION__, propertyName));
    }
    else
    {
        CcspTraceError(("[%s]: Invalid property name \n", __FUNCTION__));
        return RBUS_ERROR_INVALID_INPUT;
    }
    if(strcmp(propertyName, RRD_WEBCFG_ISSUE_EVENT) == 0)
    {

            rbusValue_t value;
            rbusValue_Init(&value);
            rbusValue_SetString(value, RRDWebCfgData);
            rbusProperty_SetValue(property, value);
            rbusValue_Release(value);
    }
    else
    {
        CcspTraceError(("[%s]: Incorrect property name %s\n", __FUNCTION__, propertyName));
        return RBUS_ERROR_INVALID_INPUT;
    }

    CcspTraceInfo(("Exit %s \n", __FUNCTION__));

    return RBUS_ERROR_SUCCESS;
}

rbusError_t RRDWebCfg_SetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts)
{
    (void) handle;
    (void) opts;
    char const* propertyName;
    rbusError_t ret = RBUS_ERROR_SUCCESS;

    CcspTraceInfo(("Enter %s \n", __FUNCTION__));

    propertyName = rbusProperty_GetName(property);
    if(propertyName)
    {
        CcspTraceInfo(("[%s]: Called for %s\n", __FUNCTION__, propertyName));
    }
    else
    {
        CcspTraceError(("[%s]: Invalid property name\n", __FUNCTION__));
        return RBUS_ERROR_INVALID_INPUT;
    }
    if(strcmp(propertyName, RRD_WEBCFG_ISSUE_EVENT) == 0)
    {
        char prev_val[256] = {0};
        rbusValue_t value = rbusProperty_GetValue(property);
        if(rbusValue_GetType(value) != RBUS_STRING)
        {
            CcspTraceError(("[%s]: Invalid gettype name\n", __FUNCTION__));
            return RBUS_ERROR_INVALID_INPUT;
        }
        strncpy(prev_val, RRDWebCfgData, sizeof(prev_val)-1);
        CcspTraceInfo(("[%s]: RemotedebuggerWebCfgData Previous String Value is %s\n", __FUNCTION__,RRDWebCfgData));
        const char* webcfgstr = rbusValue_GetString(value,NULL);
        if(strlen(webcfgstr) > 255)
        {
            CcspTraceError(("[%s]: Invalid string length\n", __FUNCTION__));
            return RBUS_ERROR_INVALID_INPUT;
        }

        strncpy(RRDWebCfgData, webcfgstr, sizeof(RRDWebCfgData)-1);
        CcspTraceInfo(("[%s]: RemotedebuggerWebCfgData String Value is %s\n", __FUNCTION__,RRDWebCfgData));
        if(syscfg_set_commit(NULL, "RemotedebuggerWebCfgData", RRDWebCfgData) != 0)
        {
            CcspTraceError(("[%s]: RemotedebuggerWebCfgData as %s syscfg_set Failed!!!\n",__FUNCTION__,RRDWebCfgData));
            return RBUS_ERROR_BUS_ERROR;
        }

        CcspTraceInfo(("[%s]: Publishing RBUS Event for RemotedebuggerWebCfgData as %s \n", __FUNCTION__,RRDWebCfgData));
        ret = sendUpdateEvent(RRD_WEBCFG_ISSUE_EVENT, RRDWebCfgData, prev_val, RBUS_STRING);
        if(ret == RBUS_ERROR_SUCCESS)
        {
            CcspTraceWarning(("%s: RBUS Publish event failed for %s with return : %s !!! \n ", __FUNCTION__, propertyName, rbusError_ToString(ret)));
        }
        else
        {
            CcspTraceInfo(("%s: RBUS Publish event success for %s !!! \n ", __FUNCTION__, propertyName));
        }
    }

    CcspTraceInfo(("Exit %s \n", __FUNCTION__));

    return RBUS_ERROR_SUCCESS;
}

rbusError_t RRD_GetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts)
{
    (void) handle;
    (void) opts;
    char const* propertyName;
    CcspTraceInfo(("Enter %s \n", __FUNCTION__));

    propertyName = rbusProperty_GetName(property);
    if(propertyName)
    {
        CcspTraceInfo(("Called %s for [%s]\n", __FUNCTION__, propertyName));
    }
    else
    {
        CcspTraceError(("[%s]: Invalid property name\n", __FUNCTION__));
        return RBUS_ERROR_INVALID_INPUT;
    }
    if(strcmp(propertyName, RRD_SET_ISSUE_EVENT) == 0)
    {

            rbusValue_t value;
            rbusValue_Init(&value);
            rbusValue_SetString(value, RRDIssueType);
            rbusProperty_SetValue(property, value);
            rbusValue_Release(value);
    }
    else
    {
        CcspTraceError(("[%s]: Incorrect property name %s\n", __FUNCTION__, propertyName));
        return RBUS_ERROR_INVALID_INPUT;
    }

    CcspTraceInfo(("Exit %s \n", __FUNCTION__));

    return RBUS_ERROR_SUCCESS;
}

rbusError_t RRD_SetStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts)
{
    (void) handle;
    (void) opts;
    char const* propertyName;
    rbusError_t ret = RBUS_ERROR_SUCCESS;

    CcspTraceInfo(("Enter %s \n", __FUNCTION__));

    propertyName = rbusProperty_GetName(property);
    if(propertyName)
    {
        CcspTraceInfo(("Called %s for [%s]\n", __FUNCTION__, propertyName));
    }
    else
    {
        CcspTraceError(("[%s]: Invalid property name\n", __FUNCTION__));
        return RBUS_ERROR_INVALID_INPUT;
    }
    if(strcmp(propertyName, RRD_SET_ISSUE_EVENT) == 0)
    {
        char prev_val[256] = {0};
        rbusValue_t value = rbusProperty_GetValue(property);
        if(rbusValue_GetType(value) != RBUS_STRING)
        {
            CcspTraceError(("[%s]: Invalid gettype name\n", __FUNCTION__));
            return RBUS_ERROR_INVALID_INPUT;
        }
        strncpy(prev_val, RRDIssueType, sizeof(prev_val)-1);
        CcspTraceInfo(("[%s]: RemotedebuggerIssueType Previous String Value is %s\n", __FUNCTION__,RRDIssueType));
        const char* str = rbusValue_GetString(value,NULL);
        if(strlen(str) > 255)
        {
            CcspTraceError(("[%s]: Invalid string length\n", __FUNCTION__));
            return RBUS_ERROR_INVALID_INPUT;
        }

        strncpy(RRDIssueType, str, sizeof(RRDIssueType)-1);
        CcspTraceInfo(("[%s]: RemotedebuggerIssueType String Value is %s\n", __FUNCTION__,RRDIssueType));
        if(syscfg_set_commit(NULL, "RemoteDebuggerIssueType", RRDIssueType) != 0)
        {
            CcspTraceError(("[%s]: RemotedebuggerIssueType as %s syscfg_set Failed!!!\n",__FUNCTION__,RRDIssueType));
            return RBUS_ERROR_BUS_ERROR;
        }

        CcspTraceInfo(("[%s]: Publishing RBUS Event for RemotedebuggerIssueType as %s \n", __FUNCTION__,RRDIssueType));
        ret = sendUpdateEvent(RRD_SET_ISSUE_EVENT, RRDIssueType, prev_val, RBUS_STRING);
        if(ret == RBUS_ERROR_SUCCESS)
        {
           CcspTraceWarning(("%s: RBUS Publish event failed for %s with return : %s !!! \n ", __FUNCTION__, propertyName, rbusError_ToString(ret)));
        }
        else
        {
            CcspTraceInfo(("%s: RBUS Publish event success for %s !!! \n ", __FUNCTION__, propertyName));
        }
    }

    CcspTraceInfo(("Exit %s \n", __FUNCTION__));

    return RBUS_ERROR_SUCCESS;
}

rbusError_t RRD_SetBoolHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts)
{
    (void) handle;
    (void) opts;
    char const* propertyName;
    rbusError_t ret = RBUS_ERROR_SUCCESS;

    CcspTraceInfo(("Enter %s \n", __FUNCTION__));

    propertyName = rbusProperty_GetName(property);
    if(propertyName)
    {
        CcspTraceInfo(("Called %s for [%s]\n", __FUNCTION__, propertyName));
    }
    else
    {
        CcspTraceError(("[%s]: Invalid property name\n", __FUNCTION__));
        return RBUS_ERROR_INVALID_INPUT;
    }

    if(strcmp(propertyName, RDM_DOWNLOAD_EVENT) == 0)
    {
        rbusValue_t value = rbusProperty_GetValue(property);
        if(rbusValue_GetType(value) != RBUS_BOOLEAN)
        {
            CcspTraceError(("[%s]: Invalid gettype name\n", __FUNCTION__));
            return RBUS_ERROR_INVALID_INPUT;
        }

        bool prev_val = RRD_BoolValue;
        RRD_BoolValue = rbusValue_GetBoolean(value);

        CcspTraceInfo(("[%s]: Previous Boolean Value is %d\n", __FUNCTION__, prev_val));
        CcspTraceInfo(("[%s]: New Boolean Value is %d\n", __FUNCTION__, RRD_BoolValue));

        if(syscfg_set_commit(NULL, "RemoteDebuggerBoolValue", RRD_BoolValue ? "true" : "false") != 0)
        {
            CcspTraceError(("[%s]: Boolean Value as %d syscfg_set Failed!!!\n", __FUNCTION__, RRD_BoolValue));
            return RBUS_ERROR_BUS_ERROR;
        }

        CcspTraceInfo(("[%s]: Publishing RBUS Event for Boolean Value as %d \n", __FUNCTION__, RRD_BoolValue));
        ret = sendUpdateEvent( RDM_DOWNLOAD_EVENT, &RRD_BoolValue, &prev_val, RBUS_BOOLEAN);
        if(ret != RBUS_ERROR_SUCCESS)
        {
           CcspTraceWarning(("%s: RBUS Publish event failed for %s with return : %s !!! \n ", __FUNCTION__, propertyName, rbusError_ToString(ret)));
        }
        else
        {
            CcspTraceInfo(("%s: RBUS Publish event success for %s !!! \n ", __FUNCTION__, propertyName));
        }
    }

    CcspTraceInfo(("Exit %s \n", __FUNCTION__));

    return RBUS_ERROR_SUCCESS;
}

#endif

void *waitForTelemetryReady(void *arg) 
{
    int ret = 0;
    FILE *file;
    char *path = "/tmp/.t2ReadyToReceiveEvents";
    bool t2ReadyToReceiveEvents = false;
    pthread_detach(pthread_self());

    publish_wanip_t *wanip_arguments = (publish_wanip_t*)arg;
    if (wanip_arguments == NULL) {
        CcspTraceError(("%s publiship_args struct is NULL\n", __FUNCTION__ ));
        return NULL;
    }

    char *event_name = wanip_arguments->event_name;
    char *new_val = wanip_arguments->new_val;
    char *old_val = wanip_arguments->old_val;

    /* Check if file exists. Wait for max 2 mins for Telemetery to create the file
     * and send the events.
     */
    for(int count=0; count<24; count++){
        file = fopen(path, "rb");
        if(file == NULL)
        {
            if(count%4==0){
                CcspTraceWarning(("%s Telemetry is not ready to receive events. Waiting for /tmp/.t2ReadyToReceiveEvents file: %d\n", __FUNCTION__, __LINE__));
            }
            sleep(5);
        }
        else
        {
            t2ReadyToReceiveEvents=true;
            fclose(file);
            break;
        }
    }

    if(!t2ReadyToReceiveEvents)
    {
        CcspTraceError(("/tmp/.t2ReadyToReceiveEvents file is not created even after 2 minutes. Exiting\n"));
        goto EXIT;
    }
    else
    {
        CcspTraceInfo(("%s /tmp/.t2ReadyToReceiveEvents file exists. Telemetry is ready to receive eventss: %d\n", __FUNCTION__, __LINE__));
    }

    bool IPv6Flag = false;
    if(strcmp(event_name,PRIMARY_WAN_IPv6_ADDRESS)==0){
        IPv6Flag=true;
    }

    CcspTraceInfo(("Publishing Primary WAN %s address with updated value=%s\n", IPv6Flag?"IPv6":"IPv4", new_val));
    ret = sendUpdateEvent(event_name, (void*)new_val, (void*)old_val, RBUS_STRING);
    if(ret == RBUS_ERROR_SUCCESS) 
    {
        CcspTraceInfo(("Published Primary WAN %s address with updated value.\n", IPv6Flag?"IPv6":"IPv4"));
    }
    else 
    {
        CcspTraceError(("Failed to publish Primary WAN %s address with updated value.\n", IPv6Flag?"IPv6":"IPv4"));
    }

    EXIT:
        free_publish_wanip_struct(wanip_arguments);
        return NULL;
}

rbusError_t publishWanIpAddr(char* event_name, char* new_val, char* old_val)
{
    if((event_name == NULL) || (new_val == NULL) || (old_val == NULL))
    {
        CcspTraceError(("%s arguments are NULL\n", __FUNCTION__ ));
        return RBUS_ERROR_BUS_ERROR;
    }

    pthread_t threadId;
    publish_wanip_t *publish_wanip_args = NULL;
    publish_wanip_args = (publish_wanip_t *)malloc(sizeof(publish_wanip_t));

    if (publish_wanip_args == NULL) {
        CcspTraceError(("%s publiship_args NULL.\n", __FUNCTION__ ));
        return RBUS_ERROR_BUS_ERROR;
    }

    memset(publish_wanip_args, 0, sizeof(publish_wanip_t));
    publish_wanip_args->event_name = strdup(event_name);
    publish_wanip_args->new_val = strdup(new_val);
    publish_wanip_args->old_val = strdup(old_val);

    CcspTraceDebug(("%s pthread_create with arguments for publishing the wan_ip: %s,%s,%s, LINE: %d\n", __FUNCTION__, publish_wanip_args->event_name, publish_wanip_args->new_val, publish_wanip_args->old_val ,__LINE__)); 
    if (pthread_create(&threadId, NULL, waitForTelemetryReady, (void *) publish_wanip_args) != 0) 
    {
        CcspTraceError(("%s: Error creating thread waitForTelemetryReady\n", __FUNCTION__));
        free_publish_wanip_struct(publish_wanip_args);
        return RBUS_ERROR_BUS_ERROR;
    } 
    else 
    {
        CcspTraceInfo(("%s: Created thread waitForTelemetryReady\n", __FUNCTION__));
    }

    return RBUS_ERROR_SUCCESS;
}

#endif

#if defined (WIFI_MANAGE_SUPPORTED)
rbusError_t getStringHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t *opts)
{
    pthread_mutex_lock(&mutex);
    (void)handle;
    (void)opts;
    char const* name = rbusProperty_GetName(property);
    rbusValue_t value;
    rbusValue_Init(&value);
    ManageWiFiInfo_t sManageWifiDetails = {0};
    char aParamVal[BUFF_LEN_64] = {0};
    
    getManageWiFiDetails(&sManageWifiDetails);
    if (0 == strcmp(name,MANAGE_WIFI_LAN_BRIDGE))
    {
        snprintf(aParamVal, BUFF_LEN_64-1, "%s%s",sManageWifiDetails.aKey,sManageWifiDetails.aBridgeName);
    }
    else if (0 == strcmp(name, MANAGE_WIFI_INTERFACES))
    {
        snprintf(aParamVal, BUFF_LEN_64-1, "%s%s",sManageWifiDetails.aKey,sManageWifiDetails.aWiFiInterfaces);
    }
    else
    {
        CcspTraceWarning(("Device Managed WiFi Bridge rbus get handler invalid input\n"));
        pthread_mutex_unlock(&mutex);
        return RBUS_ERROR_INVALID_INPUT;
    }
    rbusValue_SetString(value,aParamVal);

    rbusProperty_SetValue(property, value);
    rbusValue_Release(value);
    pthread_mutex_unlock(&mutex);

    return RBUS_ERROR_SUCCESS;
}
rbusError_t setStringHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;
    char const* name = rbusProperty_GetName(prop);
    rbusValue_t value = rbusProperty_GetValue(prop);
    rbusValueType_t type = rbusValue_GetType(value);
    const char *strVal = NULL;
    char aString[BUFF_LEN_128] = {'\0'};
    char *pKeyVal = aString;
    ManageWiFiInfo_t sManageWifiDetails = {0};
    int len = 0;

    if (type != RBUS_STRING)
    {
        CcspTraceWarning(("input value is of invalid type\n"));
        return RBUS_ERROR_INVALID_INPUT;
    }

    strVal = rbusValue_GetString(value, &len);
    if (NULL == strVal)
    {
        CcspTraceError(("Invalid set value for the parameter '%s'\n", MANAGE_WIFI_LAN_BRIDGE));
        return RBUS_ERROR_INVALID_INPUT;
    }

    CcspTraceInfo(("%s:%d, StrVal:%s, len:%d\n",__FUNCTION__,__LINE__,strVal, len));
    snprintf(aString, BUFF_LEN_128,strVal);
    CcspTraceInfo(("%s:%d, aString:%s\n",__FUNCTION__,__LINE__,aString));
    pKeyVal = strtok(pKeyVal, ":");
    if (NULL != pKeyVal)
    {
        CcspTraceInfo(("%s:%d, pKeyVal:%s\n",__FUNCTION__,__LINE__,pKeyVal));

        /* CID 346807 : Calling risky function fix */
        strncpy(sManageWifiDetails.aKey, pKeyVal, sizeof(sManageWifiDetails.aKey) - 1);
        
        CcspTraceInfo(("%s:%d, sManageWifiDetails.aKey:%s\n",__FUNCTION__,__LINE__,sManageWifiDetails.aKey));
    }
    pKeyVal = strtok(NULL, ":");

    if (NULL == pKeyVal)
    {
        CcspTraceWarning(("%s:%d, Value is NULL in the Key:Value pair\n",__FUNCTION__,__LINE__));
        return RBUS_ERROR_INVALID_INPUT;
    }

    if (0 == strcmp(name,MANAGE_WIFI_LAN_BRIDGE))
    {
	sManageWifiDetails.eUpdateType = BRIDGE_NAME;
        CcspTraceInfo(("%s:%d, pKeyVal:%s\n",__FUNCTION__,__LINE__,pKeyVal));
        strcpy(sManageWifiDetails.aBridgeName,pKeyVal);
        CcspTraceInfo(("%s:%d, sManageWifiDetails.aBridgeName:%s\n",__FUNCTION__,__LINE__,sManageWifiDetails.aBridgeName));
        setManageWiFiDetails (&sManageWifiDetails);
    }
    else if (0 == strcmp(name, MANAGE_WIFI_INTERFACES))
    {
	sManageWifiDetails.eUpdateType = WIFI_INTERFACES;
        CcspTraceInfo(("%s:%d, pKeyVal:%s\n",__FUNCTION__,__LINE__,pKeyVal));
        strcpy(sManageWifiDetails.aWiFiInterfaces,pKeyVal);
        CcspTraceInfo(("%s:%d, sManageWifiDetails.aWiFiInterfaces:%s\n",__FUNCTION__,__LINE__,sManageWifiDetails.aWiFiInterfaces));
        setManageWiFiDetails (&sManageWifiDetails);
    }
    else
    {
        CcspTraceWarning(("Device Managed WiFi rbus set handler invalid input\n"));
        return RBUS_ERROR_INVALID_INPUT;
    }
    return RBUS_ERROR_SUCCESS;
}

rbusError_t getBoolHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t *opts)
{
    pthread_mutex_lock(&mutex);
    char const* name = rbusProperty_GetName(property);
    (void)handle;
    (void)opts;
    rbusValue_t value;
    rbusValue_Init(&value);
    BOOL bEnable = false;

    if (0 == strcmp(name,MANAGE_WIFI_ENABLE))
    {
        getManageWiFiEnable(&bEnable);
        CcspTraceInfo(("%s:%d, bEnable:%d\n",__FUNCTION__,__LINE__,bEnable));
        rbusValue_SetBoolean(value,bEnable);
    }
    else
    {
        CcspTraceWarning(("Device Managed WiFi rbus Enable get handler invalid input\n"));
        pthread_mutex_unlock(&mutex);
        return RBUS_ERROR_INVALID_INPUT;
    }

    rbusProperty_SetValue(property, value);
    rbusValue_Release(value);
    pthread_mutex_unlock(&mutex);

    return RBUS_ERROR_SUCCESS;
}
rbusError_t eventManageWiFiBridgeSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish)
{
    (void)handle;
    (void)filter;
    (void)interval;

    *autoPublish = false;

    CcspTraceWarning(("Event WiFiManage Bridge Name sub handler called.\n"));

    if (strcmp(eventName, MANAGE_WIFI_LAN_BRIDGE) == 0)
    {
        if (action == RBUS_EVENT_ACTION_SUBSCRIBE)
        {
            gManageWiFiBridgeSubscribersCount += 1;
        }
        else
        {
            if (gManageWiFiBridgeSubscribersCount > 0)
            {
                gManageWiFiBridgeSubscribersCount -= 1;
            }
        }
        CcspTraceWarning(("WiFi Manage Bridge Name Subscribers count changed, new value=%d\n", gManageWiFiBridgeSubscribersCount));
    }
    else
    {
        CcspTraceWarning(("provider: eventSubHandler unexpected eventName %s\n", eventName));
    }
    return RBUS_ERROR_SUCCESS;
}
rbusError_t eventManageWiFiEnableSubHander(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish)
{
    (void)handle;
    (void)filter;
    (void)interval;

    *autoPublish = false;

    CcspTraceWarning(("Event Manage WiFi Enable sub handler called.\n"));

    if (strcmp(eventName, MANAGE_WIFI_ENABLE) == 0)
    {
        if (action == RBUS_EVENT_ACTION_SUBSCRIBE)
        {
            gManageWiFiEnableSubscribersCount+= 1;
        }
        else
        {
            if (gManageWiFiEnableSubscribersCount> 0)
            {
                gManageWiFiEnableSubscribersCount -= 1;
            }
        }
        CcspTraceWarning(("Manage WiFi Enable Subscribers count changed, new value=%d\n", gManageWiFiEnableSubscribersCount));
    }
    else
    {
        CcspTraceWarning(("provider: eventSubHandler unexpected eventName %s\n", eventName));
    }
    return RBUS_ERROR_SUCCESS;
}

rbusError_t eventManageWiFiInterfaceSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish)
{
    (void)handle;
    (void)filter;
    (void)interval;

    *autoPublish = false;

    CcspTraceWarning(("Event Manage WiFi Interface sub handler called.\n"));

    if (strcmp(eventName, MANAGE_WIFI_INTERFACES) == 0)
    {
        if (action == RBUS_EVENT_ACTION_SUBSCRIBE)
        {
            gManageWiFiInterfaceSubscribersCount += 1;
        }
        else
        {
            if (gManageWiFiInterfaceSubscribersCount > 0)
            {
                gManageWiFiInterfaceSubscribersCount -= 1;
            }
        }
        CcspTraceWarning(("Manage WiFi Interface Subscribers count changed, new value=%d\n", gManageWiFiInterfaceSubscribersCount));
    }
    else
    {
        CcspTraceWarning(("provider: eventSubHandler unexpected eventName %s\n", eventName));
    }
    return RBUS_ERROR_SUCCESS;
}
#endif /*WIFI_MANAGE_SUPPORTED*/

#if defined (AMENITIES_NETWORK_ENABLED)
rbusError_t getStringAmenityHandler(rbusHandle_t rbusHandle, rbusProperty_t rbusProp, rbusGetHandlerOptions_t* rbusOpts)
{
    (void)rbusHandle;
    (void)rbusOpts;
    CcspTraceInfo(("%s:%d, Amenity Network get handler called.\n", __FUNCTION__, __LINE__));
    char const* pName = rbusProperty_GetName(rbusProp);
    char cParamVal[BUFF_LEN_64] = {0};
    int iBridgeIndex = 0;
    char cElementName[BUFF_LEN_32] = {0};

    if (0 != strncmp (pName, LAN_BRIDGES_TABLE,strlen(LAN_BRIDGES_TABLE)))
    {
        CcspTraceError(("%s:%d, Invalid set value for the parameter '%s'\n", __FUNCTION__, __LINE__, pName));
        return RBUS_ERROR_INVALID_INPUT;
    }
    sscanf (pName, "Device.LAN.Bridge.%d.%31s", &iBridgeIndex, cElementName);
    CcspTraceInfo(("%s:%d, Bridge index is %d\n", __FUNCTION__, __LINE__, iBridgeIndex));
    CcspTraceInfo(("%s:%d, Element name is %s\n", __FUNCTION__, __LINE__, cElementName));
    amenityBridgeInfo_t *pAmenityBridgeInfo = NULL;
    int iVapCount = 0;
    if (0 != getAmenityRbusData(&pAmenityBridgeInfo, &iVapCount))
    {
        CcspTraceError(("%s:%d, Failed to get Amenity Rbus data\n", __FUNCTION__, __LINE__));
        return RBUS_ERROR_INVALID_INPUT;
    }
    if (NULL == pAmenityBridgeInfo)
    {
        CcspTraceError(("%s:%d, Failed to get Amenity Rbus data:%p\n", __FUNCTION__, __LINE__, pAmenityBridgeInfo));
    }
    if (iBridgeIndex < 1 || iBridgeIndex > iVapCount)
    {
        CcspTraceError(("%s:%d, Invalid bridge index %d\n", __FUNCTION__, __LINE__, iBridgeIndex));
    }
    if ((NULL != pAmenityBridgeInfo) && (iBridgeIndex >= 1 || iBridgeIndex <= iVapCount))
    {
        if (!strncmp(cElementName, VAP_NAME_STR, strlen(VAP_NAME_STR)))
        {
            snprintf (cParamVal, sizeof(cParamVal), "%s", pAmenityBridgeInfo[iBridgeIndex-1].cVapName);
            CcspTraceInfo(("%s:%d, Amenity Network %d:%s get handler called.\n", __FUNCTION__, __LINE__, iBridgeIndex, pAmenityBridgeInfo[iBridgeIndex-1].cVapName));
        }
        else if (!strncmp(cElementName, BRIDGE_NAME_STR, strlen(BRIDGE_NAME_STR)))
        {
            snprintf (cParamVal, sizeof(cParamVal), "%s", pAmenityBridgeInfo[iBridgeIndex-1].cBridgeName);
            CcspTraceInfo(("%s:%d, Amenity Network %d:%s get handler called.\n", __FUNCTION__, __LINE__, iBridgeIndex, pAmenityBridgeInfo[iBridgeIndex-1].cBridgeName));
        }
        else if (!strncmp(cElementName, WIFI_IF_STR, strlen(WIFI_IF_STR)))
        {
            snprintf (cParamVal, sizeof(cParamVal), "%s", pAmenityBridgeInfo[iBridgeIndex-1].cWiFiInterface);
            CcspTraceInfo(("%s:%d, Amenity Network %d:%s get handler called.\n", __FUNCTION__, __LINE__, iBridgeIndex, pAmenityBridgeInfo[iBridgeIndex-1].cWiFiInterface));
        }
    }
    CcspTraceInfo(("%s:%d, cParamVal:%s\n",__FUNCTION__,__LINE__,cParamVal));
    rbusValue_t pValue = NULL;
    rbusValue_Init(&pValue);
    rbusValue_SetString(pValue, cParamVal);
    rbusProperty_SetValue(rbusProp, pValue);
    rbusValue_Release(pValue);

    return RBUS_ERROR_SUCCESS;
}

rbusError_t setStringAmenityHandler(rbusHandle_t rbusHandle, rbusProperty_t rbusProp, rbusSetHandlerOptions_t* rbusOpts)
{
    (void)rbusHandle;
    (void)rbusOpts;

    CcspTraceInfo(("%s:%d, Amenity Network set handler called.\n", __FUNCTION__, __LINE__));

    char const* pName = rbusProperty_GetName(rbusProp);
    rbusValue_t pValue = rbusProperty_GetValue(rbusProp);
    rbusValueType_t rbusType = rbusValue_GetType(pValue);

    if (RBUS_STRING != rbusType)
    {
        CcspTraceError(("%s:%d, Invalid type for property %s\n", __FUNCTION__, __LINE__, pName));
        return RBUS_ERROR_INVALID_INPUT;
    }
    int iStrLen = 0;
    int iBridgeIndex = 0;
    char cElementName[BUFF_LEN_32] = {0};
    const char *pStrVal = rbusValue_GetString(pValue, &iStrLen);
    if (NULL == pStrVal)
    {
        CcspTraceError(("%s:%d, Invalid set value for the parameter '%s'\n", __FUNCTION__, __LINE__, pName));
        return RBUS_ERROR_INVALID_INPUT;
    }
    CcspTraceInfo(("%s:%d, StrVal:%s, len:%d\n",__FUNCTION__,__LINE__,pStrVal, iStrLen));

    if (0 != strncmp (pName, LAN_BRIDGES_TABLE,strlen(LAN_BRIDGES_TABLE)))
    {
        CcspTraceError(("%s:%d, Invalid set value for the parameter '%s'\n", __FUNCTION__, __LINE__, pName));
        return RBUS_ERROR_INVALID_INPUT;
    }
    sscanf (pName, "Device.LAN.Bridge.%d.%31s", &iBridgeIndex, cElementName);
    CcspTraceInfo(("%s:%d, Bridge index is %d\n", __FUNCTION__, __LINE__, iBridgeIndex));
    CcspTraceInfo(("%s:%d, Element name is %s\n", __FUNCTION__, __LINE__, cElementName));

    amenityBridgeInfo_t *pAmenityBridgeInfo = NULL;
    int iVapCount = 0;
    if (0 != getAmenityRbusData(&pAmenityBridgeInfo, &iVapCount))
    {
        CcspTraceError(("%s:%d, Failed to get Amenity Rbus data\n", __FUNCTION__, __LINE__));
        return RBUS_ERROR_INVALID_INPUT;
    }
    if (NULL == pAmenityBridgeInfo)
    {
        CcspTraceError(("%s:%d, Failed to get Amenity Rbus data\n", __FUNCTION__, __LINE__));
        return RBUS_ERROR_INVALID_INPUT;
    }
    if (iBridgeIndex < 1 || iBridgeIndex > iVapCount)
    {
        CcspTraceError(("%s:%d, Invalid bridge index %d\n", __FUNCTION__, __LINE__, iBridgeIndex));
        return RBUS_ERROR_INVALID_INPUT;
    }

    if (!strncmp(cElementName, VAP_NAME_STR, strlen(VAP_NAME_STR)))
    {
        snprintf (pAmenityBridgeInfo[iBridgeIndex -1].cVapName, sizeof(pAmenityBridgeInfo[iBridgeIndex-1].cVapName), "%s", pStrVal);
        CcspTraceInfo(("%s:%d, Amenity Network %d:%s set handler called.\n", __FUNCTION__, __LINE__, iBridgeIndex, pAmenityBridgeInfo[iBridgeIndex-1].cVapName));
    }
    else if (!strncmp(cElementName, BRIDGE_NAME_STR, strlen(BRIDGE_NAME_STR)))
    {
        snprintf (pAmenityBridgeInfo[iBridgeIndex-1].cBridgeName, sizeof(pAmenityBridgeInfo[iBridgeIndex-1].cBridgeName), "%s", pStrVal);
        CcspTraceInfo(("%s:%d, Amenity Network %d:%s set handler called.\n", __FUNCTION__, __LINE__, iBridgeIndex, pAmenityBridgeInfo[iBridgeIndex-1].cBridgeName));
    }
    else if (!strncmp(cElementName, WIFI_IF_STR, strlen(WIFI_IF_STR)))
    {
        snprintf (pAmenityBridgeInfo[iBridgeIndex-1].cWiFiInterface, sizeof(pAmenityBridgeInfo[iBridgeIndex-1].cWiFiInterface), "%s", pStrVal);
        CcspTraceInfo(("%s:%d, Amenity Network %d:%s set handler called.\n", __FUNCTION__, __LINE__, iBridgeIndex, pAmenityBridgeInfo[iBridgeIndex-1].cWiFiInterface));
    }

    return RBUS_ERROR_SUCCESS;
}

rbusError_t addRowInTableHandler (rbusHandle_t rbusHandle, char const *pTableName, char const * pAliasName, uint32_t *pInstanceNum)
{
    (void)rbusHandle;
    static uint32_t ui32InstanceNum = 1;
    *pInstanceNum = ui32InstanceNum++;

    CcspTraceInfo(("%s:%d:, TableName:%s, AliasName:%s, InstanceNum:%d\n", __FUNCTION__, __LINE__, pTableName, pAliasName, ui32InstanceNum));

    return RBUS_ERROR_SUCCESS;
}

rbusError_t removeRowInTableHander (rbusHandle_t rbusHandle, char const *pRowName)
{
    (void)rbusHandle;
    CcspTraceInfo(("%s:%d:, RowName:%s\n", __FUNCTION__, __LINE__, pRowName));

    return RBUS_ERROR_SUCCESS;
}

#endif

#if defined (RBUS_WAN_IP)

char const* GetParamName(char const* path)
{
    char const* p = path + strlen(path);
    while(p > path && *(p-1) != '.')
        p--;
    return p;
}
rbusError_t getStringHandlerWANIP_RBUS(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t *opts)
{
    (void)handle;
    (void)opts;

    int32_t rc;
    char const* name = rbusProperty_GetName(property);
    char* param = strdup(GetParamName(name));
    char val[256];
    ULONG ulen = 256;

    rbusValue_t value;
    rbusValue_Init(&value);

    rc = DeviceInfo_GetParamStringValue(NULL, param, val, &ulen);
    free(param);
    if(rc != 0)
    {
        CcspTraceError(("[%s]: getStringHandlerWANIP_RBUS failed\n", __FUNCTION__));
        return RBUS_ERROR_BUS_ERROR;
    }
    else{
        CcspTraceInfo(("[%s]: getStringHandlerWANIP_RBUS success\n", __FUNCTION__));
    }

    rbusValue_SetString(value, val);
    rbusProperty_SetValue(property, value);
    rbusValue_Release(value);

    return RBUS_ERROR_SUCCESS;
}
rbusError_t eventWANIPSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish)
{
    (void)handle;
    (void)filter;
    (void)interval;
    
    *autoPublish = false;
    
    CcspTraceInfo(("%s called for event '%s'\n", __FUNCTION__, eventName));

    if (strcmp(eventName, PRIMARY_WAN_IP_ADDRESS) == 0)
    {
        if (action == RBUS_EVENT_ACTION_SUBSCRIBE)
        {
            gSubscribersCount_IPv4 += 1;
        }
        else
        {
            if (gSubscribersCount_IPv4 > 0)
            {
                gSubscribersCount_IPv4 -= 1;
            }
        }
        CcspTraceWarning(("WAN IP Subscribers count changed, new value=%d\n", gSubscribersCount_IPv4));

    }
    else if (strcmp(eventName, PRIMARY_WAN_IPv6_ADDRESS) == 0)
    {
        if (action == RBUS_EVENT_ACTION_SUBSCRIBE)
        {
            gSubscribersCount_IPv6 += 1;
        }
        else
        {
            if (gSubscribersCount_IPv6 > 0)
            {
                gSubscribersCount_IPv6 -= 1;
            }
        }
        CcspTraceWarning(("WAN IPv6 Subscribers count changed, new value=%d\n", gSubscribersCount_IPv6));

    }
    else
    {
        CcspTraceWarning(("provider: eventSubHandler unexpected eventName %s\n", eventName));
    }
    return RBUS_ERROR_SUCCESS;
}

#endif /*RBUS_WAN_IP*/

#if defined(RBUS_BUILD_FLAG_ENABLE) && !defined(_HUB4_PRODUCT_REQ_) && !defined(RDKB_EXTENDER_ENABLED)
static void Cosa_Rbus_Handler_WanStatus_EventHandler(rbusHandle_t handle, rbusEvent_t const* event, rbusEventSubscription_t* subscription)
{
    (void)handle;
    (void)subscription;

    const char* eventName = event->name;

    if((eventName == NULL))
    {
        CcspTraceError(("%s %d : FAILED , value is NULL\n",__FUNCTION__, __LINE__));
        return;
    }

    CcspTraceInfo(("%s %d: Received %s\n", __FUNCTION__, __LINE__, eventName));

    // CurrentStatus Event
    if( 0 == strncmp(eventName, WANMGR_CURRENT_STATUS_TR181, strlen(WANMGR_CURRENT_STATUS_TR181)) )
    {
        rbusValue_t value;
        value = rbusObject_GetValue(event->data, NULL);

        char acStatus[16] = {0};
        strncpy(acStatus, rbusValue_GetString(value, NULL), sizeof(acStatus) - 1);
        acStatus[sizeof(acStatus) - 1] = '\0';
        CcspTraceInfo(("%s: Event:%s Status:%s\n", __FUNCTION__, eventName, acStatus));

        // Trigger Network Response script
        if( 0 == strncmp(acStatus, "Up", 2) )
        {
            v_secure_system("sh /etc/network_response.sh &");
        }
#if defined (_XB6_PRODUCT_REQ_)
        else
        {
            v_secure_system("sh /etc/network_response.sh OnlyForNoRf &");
        }
#endif /** _XB6_PRODUCT_REQ_ */
    }
}

/** Cosa_Rbus_Handler_SubscribeWanStatusEvent() */
void Cosa_Rbus_Handler_SubscribeWanStatusEvent( void )
{
    rbusError_t rc = RBUS_ERROR_SUCCESS;

    rc = rbusEvent_Subscribe(handle, WANMGR_CURRENT_STATUS_TR181, Cosa_Rbus_Handler_WanStatus_EventHandler, NULL, 60);
    if(rc != RBUS_ERROR_SUCCESS)
    {
        CcspTraceError(("%s %d - Failed to Subscribe %s, Error=%s \n", __FUNCTION__, __LINE__, WANMGR_CURRENT_STATUS_TR181, rbusError_ToString(rc)));
        return;
    }
}
#endif /**  RBUS_BUILD_FLAG_ENABLE && !_HUB4_PRODUCT_REQ_ && !RDKB_EXTENDER_ENABLED */

#if  defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED) ||  defined(RBUS_BUILD_FLAG_ENABLE) ||  defined(_HUB4_PRODUCT_REQ_) || defined (_PLATFORM_RASPBERRYPI_) || defined (WIFI_MANAGE_SUPPORTED) || defined (RBUS_WAN_IP)
/***********************************************************************

  devCtrlRbusInit(): Initialize Rbus and data elements

 ***********************************************************************/
rbusError_t devCtrlRbusInit()
{
	int rc = RBUS_ERROR_SUCCESS;

	if(RBUS_ENABLED != rbus_checkStatus())
    {
		CcspTraceWarning(("%s: RBUS not available. Events is not supported\n", __FUNCTION__));
		return RBUS_ERROR_BUS_ERROR;
    }

	rc = rbus_open(&handle, RBUS_COMPONENT_NAME);
	if (rc != RBUS_ERROR_SUCCESS)
	{
		CcspTraceWarning(("DevCtrl rbus initialization failed\n"));
		rc = RBUS_ERROR_NOT_INITIALIZED;
		return rc;
	}
#if  defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED) || defined (WIFI_MANAGE_SUPPORTED) || defined (RBUS_WAN_IP)
	// Register data elements
	rc = rbus_regDataElements(handle, NUM_OF_RBUS_PARAMS, devCtrlRbusDataElements);
#endif
	if (rc != RBUS_ERROR_SUCCESS)
	{
		CcspTraceWarning(("rbus register data elements failed\n"));
		rc = rbus_close(handle);
		return rc;
	}
    #if defined (AMENITIES_NETWORK_ENABLED)
    CcspTraceInfo(("%s: %d, Adding row to the rbus table\n", __FUNCTION__, __LINE__));
    for (int iCount = 1; iCount <= NUM_OF_SUPPORTED_ELEMENTS; iCount++)
    {
        CcspTraceInfo(("%s: %d, Adding row %d to the rbus table\n", __FUNCTION__, __LINE__, iCount));
        rc = rbusTable_addRow(handle, LAN_BRIDGES_TABLE, NULL, NULL);
        if (rc != RBUS_ERROR_SUCCESS)
        {
            CcspTraceError(("%s: %d, Failed to add row %d to the rbus table\n", __FUNCTION__, __LINE__, iCount));
        }
    }
    #endif
#if  defined  (WAN_FAILOVER_SUPPORTED) || defined(RDKB_EXTENDER_ENABLED)	
	//initialize sysevent
	PAM_Rbus_SyseventInit();
#endif

#if defined(RBUS_BUILD_FLAG_ENABLE) && !defined(_HUB4_PRODUCT_REQ_) && !defined(RDKB_EXTENDER_ENABLED)
        //Subscribe WAN Status Event
        Cosa_Rbus_Handler_SubscribeWanStatusEvent( );
#endif /**  RBUS_BUILD_FLAG_ENABLE && !_HUB4_PRODUCT_REQ_ && !RDKB_EXTENDER_ENABLED */

	return rc;
}
#endif
