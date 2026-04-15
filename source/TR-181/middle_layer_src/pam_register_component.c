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
    "eRT.com.cisco.spvtg.ccsp.ledmanager",
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
        return false;
    }

    bool found = false;

    for(int i = 0; i < count; i++)
    {
        if(components[i] && strcmp(components[i], name) == 0)
        {
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
    for(int i = 0; i < dep_count; i++)
    {
        if(!isComponentRegisteredInRbus(deps[i]))
        {
            CcspTraceInfo(("[PAM] Dependency %s is NOT up\n", deps[i]));
            return false;
        }

        CcspTraceInfo(("[PAM] Dependency %s is UP\n", deps[i]));
    }

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

    return (rbusHandle_t)bus_info->rbus_handle;
}

/* -----------------------------------------------------------
 *  EVENT GET HANDLER (REQUIRED)
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
 *  REGISTER EVENTS (CR STYLE)
 * ----------------------------------------------------------- */
static void registerPamEvents(rbusHandle_t handle)
{
    if(!handle)
    {
        CcspTraceInfo(("[PAM] Invalid RBUS handle for registration\n"));
        return;
    }

    rbusDataElement_t dataElements[2] =
    {
        { "wifi_ready_to_go", RBUS_ELEMENT_TYPE_EVENT, { eventGetHandler, NULL, NULL, NULL, NULL, NULL } },
        { "wan_ready_to_go",  RBUS_ELEMENT_TYPE_EVENT, { eventGetHandler, NULL, NULL, NULL, NULL, NULL } }
    };

    rbusError_t rc = rbus_regDataElements(handle, 2, dataElements);

    if(rc != RBUS_ERROR_SUCCESS)
    {
        CcspTraceInfo(("[PAM] Event registration failed rc=%d\n", rc));
    }
    else
    {
        CcspTraceInfo(("[PAM] Events registered successfully\n"));
    }
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

    if(rc != RBUS_ERROR_SUCCESS)
    {
        CcspTraceInfo(("[PAM] Publish FAILED %s rc=%d\n", eventName, rc));
    }
    else
    {
        CcspTraceInfo(("[PAM] Publish SUCCESS %s\n", eventName));
    }
}

/* -----------------------------------------------------------
 *  WIFI READY
 * ----------------------------------------------------------- */
void pam_checkAndPublishWifiReady(rbusHandle_t handle)
{
    (void)handle;

    static bool eventsRegistered = false;

    rbusHandle_t rbusHandle = getRbusHandle();

    if(!eventsRegistered)
    {
        registerPamEvents(rbusHandle);
        eventsRegistered = true;
    }

    for(int retry = 0; retry < 10; retry++)
    {
        if(areAllDepsUp(WIFI_DEPS, WIFI_DEPS_COUNT))
        {
            publishReadyEvent(rbusHandle, "wifi_ready_to_go");
            return;
        }

        sleep(2);
    }
}

/* -----------------------------------------------------------
 *  WAN READY
 * ----------------------------------------------------------- */
void pam_checkAndPublishWanReady(rbusHandle_t handle)
{
    (void)handle;

    static bool eventsRegistered = false;

    rbusHandle_t rbusHandle = getRbusHandle();

    if(!eventsRegistered)
    {
        registerPamEvents(rbusHandle);
        eventsRegistered = true;
    }


    for(int retry = 0; retry < 10; retry++)
    {
        if(areAllDepsUp(WAN_DEPS, WAN_DEPS_COUNT))
        {
            publishReadyEvent(rbusHandle, "wan_ready_to_go");
            return;
        }

        sleep(2);
    }
}