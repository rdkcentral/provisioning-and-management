#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include <rbus/rbus.h>
#include "rbuscore.h"

#include "pam_register_component.h"
#include "ccsp_trace.h"
#include "ccsp_base_api.h"
#include "ccsp_message_bus.h"
#include "ansc_platform.h"

#define WIFI_COMPONENT "eRT.com.cisco.spvtg.ccsp.wifi"
#define WAN_COMPONENT  "eRT.com.cisco.spvtg.ccsp.wanmanager"

#define MAX_RETRY 30
#define RETRY_DELAY 2



/* ----------------------------------------------------------- */
/* DEPENDENCIES */
static const char* WIFI_DEPS[] = {
    "eRT.com.cisco.spvtg.ccsp.psm",
    "eRT.com.cisco.spvtg.ccsp.pam",
};
static const int WIFI_DEPS_COUNT = sizeof(WIFI_DEPS) / sizeof(WIFI_DEPS[0]);

static const char* WAN_DEPS[] = {
    "eRT.com.cisco.spvtg.ccsp.psm",
    "eRT.com.cisco.spvtg.ccsp.pam",
    "eRT.com.cisco.spvtg.ccsp.ethagent",
};
static const int WAN_DEPS_COUNT = sizeof(WAN_DEPS) / sizeof(WAN_DEPS[0]);

/* ----------------------------------------------------------- */
/* RBUS COMPONENT CHECK */
static bool isComponentRegisteredInRbus(const char* name)
{
    int count = 0;
    char** components = NULL;

    rbusCoreError_t rc = rbus_discoverRegisteredComponents(&count, &components);
    if(rc != RBUSCORE_SUCCESS || components == NULL)
        return false;

    bool found = false;

    for(int i = 0; i < count; i++)
    {
        if(components[i] && strcmp(components[i], name) == 0)
            found = true;

        free(components[i]);
    }
    free(components);

    return found;
}

/* ----------------------------------------------------------- */
/* DEP CHECK */
static bool areAllDepsUp(const char** deps, int dep_count)
{
    for(int i = 0; i < dep_count; i++)
    {
        if(!isComponentRegisteredInRbus(deps[i]))
            return false;
    }
    return true;
}

/* ----------------------------------------------------------- */
/* EVENT GET HANDLER */
static rbusError_t eventGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;

    rbusValue_t value;
    rbusValue_Init(&value);
    rbusValue_SetBoolean(value, true);

    rbusProperty_SetValue(property, value);
    rbusValue_Release(value);

    return RBUS_ERROR_SUCCESS;
}

/* ----------------------------------------------------------- */
/* EVENT SUB HANDLER */
static rbusError_t eventSubHandler(
    rbusHandle_t handle,
    rbusEventSubAction_t action,
    const char* eventName,
    rbusFilter_t filter,
    int32_t interval,
    bool* autoPublish)
{
    (void)handle;
    (void)filter;
    (void)interval;

    CcspTraceInfo(("[PAM] subHandler called for %s action=%d\n", eventName, action));

    if(autoPublish)
        *autoPublish = false;

    return RBUS_ERROR_SUCCESS;
}

/* ----------------------------------------------------------- */
/* REGISTER EVENTS */
void registerPamEvents(rbusHandle_t handle)
{
    if(!handle)
    {
        CcspTraceInfo(("[PAM] Cannot register events, handle NULL\n"));
        return;
    }

    CcspTraceInfo(("[PAM] Registering RBUS events\n"));

    rbusDataElement_t dataElements[2] =
    {
        { "wifi_ready_to_go", RBUS_ELEMENT_TYPE_EVENT,
            { eventGetHandler, NULL, NULL, NULL, eventSubHandler, NULL } },

        { "wan_ready_to_go", RBUS_ELEMENT_TYPE_EVENT,
            { eventGetHandler, NULL, NULL, NULL, eventSubHandler, NULL } }
    };

    rbusError_t rc = rbus_regDataElements(handle, 2, dataElements);

    CcspTraceInfo(("[PAM] Event registration rc=%d\n", rc));
}

/* ----------------------------------------------------------- */
/* PUBLISH EVENT */
static void publishReadyEvent(rbusHandle_t handle, const char* eventName)
{
    rbusValue_t newVal, oldVal;
    rbusEvent_t event = {0};
    rbusObject_t data;

    rbusValue_Init(&newVal);
    rbusValue_Init(&oldVal);

    rbusValue_SetBoolean(newVal, true);
    rbusValue_SetBoolean(oldVal, false);

    rbusObject_Init(&data, NULL);
    rbusObject_SetValue(data, "value", newVal);
    rbusObject_SetValue(data, "oldValue", oldVal);

    event.name = eventName;
    event.data = data;
    event.type = RBUS_EVENT_VALUE_CHANGED;

    rbusError_t rc = rbusEvent_Publish(handle, &event);

    rbusValue_Release(newVal);
    rbusValue_Release(oldVal);
    rbusObject_Release(data);

    CcspTraceInfo(("[PAM] Publish %s rc=%d (%s)\n",
        eventName,
        rc,
        rc == RBUS_ERROR_NOSUBSCRIBERS ? "NO SUBSCRIBERS" : "DELIVERED"));
}

/* ----------------------------------------------------------- */
/* WIFI */
void pam_checkAndPublishWifiReady(rbusHandle_t handle)
{
    rbusHandle_t rbusHandle = handle;

    if(!rbusHandle)
    {
        CcspTraceError(("[PAM] Invalid RBUS handle\n"));
        return;
    }
    
    for(int retry = 0; retry < MAX_RETRY; retry++)
    {
        if(!isComponentRegisteredInRbus(WIFI_COMPONENT))
        {
            CcspTraceInfo(("[PAM] WiFi NOT ready\n"));
            sleep(RETRY_DELAY);
            continue;
        }

        if(!areAllDepsUp(WIFI_DEPS, WIFI_DEPS_COUNT))
        {
            sleep(RETRY_DELAY);
            continue;
        }

        publishReadyEvent(rbusHandle, "wifi_ready_to_go");
        return;
    }

    CcspTraceInfo(("[PAM] WiFi READY EVENT TIMEOUT\n"));
}

/* ----------------------------------------------------------- */
/* WAN */
void pam_checkAndPublishWanReady(rbusHandle_t handle)
{
    rbusHandle_t rbusHandle = handle;

    if(!rbusHandle)
    {
        CcspTraceError(("[PAM] Invalid RBUS handle\n"));
        return;
    }

  
    for(int retry = 0; retry < MAX_RETRY; retry++)
    {
        if(!isComponentRegisteredInRbus(WAN_COMPONENT))
        {
            CcspTraceInfo(("[PAM] WAN NOT ready\n"));
            sleep(RETRY_DELAY);
            continue;
        }

        if(!areAllDepsUp(WAN_DEPS, WAN_DEPS_COUNT))
        {
            sleep(RETRY_DELAY);
            continue;
        }

        publishReadyEvent(rbusHandle, "wan_ready_to_go");
        return;
    }

    CcspTraceInfo(("[PAM] WAN READY EVENT TIMEOUT\n"));
}