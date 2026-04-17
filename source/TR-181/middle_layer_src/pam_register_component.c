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

/* -----------------------------------------------------------
 *  GLOBAL FLAG (AVOID DUPLICATE REGISTRATION)
 * ----------------------------------------------------------- */
static bool g_eventsRegistered = false;

/* -----------------------------------------------------------
 *  HARD-CODED DEPENDENCIES
 * ----------------------------------------------------------- */
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

/* -----------------------------------------------------------
 *  RBUS CHECK
 * ----------------------------------------------------------- */
static bool isComponentRegisteredInRbus(const char* name)
{
    CcspTraceInfo(("[PAM] Checking RBUS registration for %s\n", name));

    int count = 0;
    char** components = NULL;

    rbusCoreError_t rc = rbus_discoverRegisteredComponents(&count, &components);
    if(rc != RBUSCORE_SUCCESS || components == NULL)
    {
        CcspTraceInfo(("[PAM] RBUS discovery failed\n"));
        return false;
    }

    bool found = false;

    for(int i = 0; i < count; i++)
    {
        if(components[i] && strcmp(components[i], name) == 0)
        {
            CcspTraceInfo(("[PAM] Found component %s\n", name));
            found = true;
        }
        free(components[i]);
    }
    free(components);

    return found;
}

/* ----------------------------------------------------------- */
static bool areAllDepsUp(const char** deps, int dep_count)
{
    CcspTraceInfo(("[PAM] Checking all dependencies...\n"));

    for(int i = 0; i < dep_count; i++)
    {
        if(!isComponentRegisteredInRbus(deps[i]))
        {
            CcspTraceInfo(("[PAM] Dependency %s is NOT up\n", deps[i]));
            return false;
        }

        CcspTraceInfo(("[PAM] Dependency %s is UP\n", deps[i]));
    }

    CcspTraceInfo(("[PAM] All dependencies are UP\n"));
    return true;
}

/* -----------------------------------------------------------
 *  GET RBUS HANDLE FROM CCSP
 * ----------------------------------------------------------- */
extern ANSC_HANDLE bus_handle;

static rbusHandle_t getRbusHandle()
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    if(!bus_info)
    {
        CcspTraceInfo(("[PAM] bus_info NULL\n"));
        return NULL;
    }

    CcspTraceInfo(("[PAM] Got RBUS handle: %p\n", bus_info->rbus_handle));
    return (rbusHandle_t)bus_info->rbus_handle;
}

/* -----------------------------------------------------------
 *  EVENT GET HANDLER
 * ----------------------------------------------------------- */
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

/* -----------------------------------------------------------
 *  EVENT SUB HANDLER 
 * ----------------------------------------------------------- */
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
/* -----------------------------------------------------------
 *  REGISTER EVENTS
 * ----------------------------------------------------------- */
static void registerPamEvents(rbusHandle_t handle)
{
    CcspTraceInfo(("[PAM] >>> Enter registerPamEvents\n"));

    if(!handle)
    {
        CcspTraceInfo(("[PAM] RBUS handle NULL, cannot register\n"));
        return;
    }

    CcspTraceInfo(("[PAM] Registering events on handle %p\n", handle));

    rbusDataElement_t dataElements[2] =
    {
        { "wifi_ready_to_go", RBUS_ELEMENT_TYPE_EVENT,
            { eventGetHandler, NULL, NULL, NULL, eventSubHandler, NULL } },

        { "wan_ready_to_go", RBUS_ELEMENT_TYPE_EVENT,
            { eventGetHandler, NULL, NULL, NULL, eventSubHandler, NULL } }
    };

    rbusError_t rc = rbus_regDataElements(handle, 2, dataElements);

    if(rc != RBUS_ERROR_SUCCESS)
    {
        CcspTraceInfo(("[PAM] Event registration FAILED rc=%d\n", rc));
    }
    else
    {
        CcspTraceInfo(("[PAM] Event registration SUCCESS\n"));
    }

    CcspTraceInfo(("[PAM] <<< Exit registerPamEvents\n"));
}

/* -----------------------------------------------------------
 *  PUBLISH EVENT
 * ----------------------------------------------------------- */
static void publishReadyEvent(rbusHandle_t g_hRbus, const char* eventName)
{
    if(!g_hRbus)
    {
        CcspTraceInfo(("[PAM] Invalid handle, cannot publish %s\n", eventName));
        return;
    }

    CcspTraceInfo(("[PAM] Publishing using handle %p\n", g_hRbus));

    rbusError_t rc;

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

    CcspTraceInfo(("[PAM] Publishing event %s\n", eventName));

    rc = rbusEvent_Publish(g_hRbus, &event);

    rbusValue_Release(newVal);
    rbusValue_Release(oldVal);
    rbusObject_Release(data);

    if(rc == RBUS_ERROR_SUCCESS || rc == RBUS_ERROR_NOSUBSCRIBERS)
    {
        CcspTraceInfo(("[PAM] Publish SUCCESS %s (rc=%d, subscribers=%s)\n",eventName, rc,rc == RBUS_ERROR_NOSUBSCRIBERS ? "none" : "yes"));
    }
    else
    {
        CcspTraceInfo(("[PAM] Publish FAILED %s rc=%d\n", eventName, rc));
    }
}

/* -----------------------------------------------------------
 *  WIFI READY
 * ----------------------------------------------------------- */
void pam_checkAndPublishWifiReady(rbusHandle_t handle)
{
    (void)handle;

    rbusHandle_t rbusHandle = getRbusHandle();

    CcspTraceInfo(("[PAM] g_eventsRegistered = %d\n", g_eventsRegistered));

    if(!g_eventsRegistered)
    {
        CcspTraceInfo(("[PAM] First-time registration\n"));
        registerPamEvents(rbusHandle);
        g_eventsRegistered = true;
    }

    for(int retry = 0; retry < 10; retry++)
    {
        /* 🔍 Check WiFi component */
        if(!isComponentRegisteredInRbus(WIFI_COMPONENT))
        {
            CcspTraceInfo(("[PAM] WiFi component NOT ready\n"));
            sleep(2);
            continue;
        }
        CcspTraceInfo(("[PAM] WiFi component is UP\n"));

        /* 🔍 Check dependencies */
        if(areAllDepsUp(WIFI_DEPS, WIFI_DEPS_COUNT))
        {
            publishReadyEvent(rbusHandle, "wifi_ready_to_go");
            return;
        }

        CcspTraceInfo(("[PAM] WiFi retry %d/10\n", retry+1));
        sleep(2);
    }
}
/* -----------------------------------------------------------
 *  WAN READY
 * ----------------------------------------------------------- */
void pam_checkAndPublishWanReady(rbusHandle_t handle)
{
    (void)handle;

    rbusHandle_t rbusHandle = getRbusHandle();

    if(!g_eventsRegistered)
    {
        registerPamEvents(rbusHandle);
        g_eventsRegistered = true;
    }

    for(int retry = 0; retry < 10; retry++)
    {
        /* 🔍 Check WAN component */
        if(!isComponentRegisteredInRbus(WAN_COMPONENT))
        {
            CcspTraceInfo(("[PAM] WAN component NOT ready\n"));
            sleep(2);
            continue;
        }
        CcspTraceInfo(("[PAM] WAN component is UP\n"));

        /* 🔍 Check dependencies */
        if(areAllDepsUp(WAN_DEPS, WAN_DEPS_COUNT))
        {
            publishReadyEvent(rbusHandle, "wan_ready_to_go");
            return;
        }

        CcspTraceInfo(("[PAM] WAN retry %d/10\n", retry+1));
        sleep(2);
    }
}