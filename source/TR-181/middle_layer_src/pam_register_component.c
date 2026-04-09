#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <rbus/rbus.h>
#include "rbuscore.h"
#include "pam_register_component.h"

/* -----------------------------------------------------------
 *  HARD-CODED DEPENDENCIES (REPLACING deviceprofiles.xml)
 * ----------------------------------------------------------- */

/* WiFi component dependencies */
static const char* WIFI_DEPS[] = {
    "com.cisco.spvtg.ccsp.psm",
    "com.cisco.spvtg.ccsp.pam",
};
static const int WIFI_DEPS_COUNT = sizeof(WIFI_DEPS) / sizeof(WIFI_DEPS[0]);

/* WAN Manager component dependencies */
static const char* WAN_DEPS[] = {
    "com.cisco.spvtg.ccsp.psm",
    "com.cisco.spvtg.ccsp.pam",
    "com.cisco.spvtg.ccsp.ledmanager",
};
static const int WAN_DEPS_COUNT = sizeof(WAN_DEPS) / sizeof(WAN_DEPS[0]);


/* -----------------------------------------------------------
 *  CHECK IF A COMPONENT IS PRESENT IN RBUS
 * ----------------------------------------------------------- */
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
        {
            found = true;
        }
        free(components[i]);
    }
    free(components);

    return found;
}


/* -----------------------------------------------------------
 *  CHECK IF ALL DEPENDENCIES ARE UP
 * ----------------------------------------------------------- */
static bool areAllDepsUp(const char** deps, int dep_count)
{
    for(int i = 0; i < dep_count; i++)
    {
        if(!isComponentRegisteredInRbus(deps[i]))
        {
            printf("[PAM] Dependency %s is NOT up\n", deps[i]);
            return false;
        }
        printf("[PAM] Dependency %s is UP\n", deps[i]);
    }
    return true;
}


/* -----------------------------------------------------------
 *  PUBLISH READY EVENT (EXACT CODE FROM YOUR MENTOR)
 * ----------------------------------------------------------- */
static void publishReadyEvent(rbusHandle_t g_hRbus, const char* eventName)
{
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

    printf("[PAM] Publishing ready event %s\n", eventName);

    rc = rbusEvent_Publish(g_hRbus, &event);

    rbusValue_Release(newVal);
    rbusValue_Release(oldVal);
    rbusObject_Release(data);

    if(rc != RBUS_ERROR_SUCCESS)
    {
        printf("[PAM] rbusEvent_Publish %s FAILED rc=%d\n", eventName, rc);
    }
}


/* -----------------------------------------------------------
 *  PUBLIC API: CHECK AND PUBLISH WIFI READY
 * ----------------------------------------------------------- */
void pam_checkAndPublishWifiReady(rbusHandle_t handle)
{
    if(areAllDepsUp(WIFI_DEPS, WIFI_DEPS_COUNT))
    {
        publishReadyEvent(handle, "wifi_ready_to_go");
    }
}


/* -----------------------------------------------------------
 *  PUBLIC API: CHECK AND PUBLISH WAN READY
 * ----------------------------------------------------------- */
void pam_checkAndPublishWanReady(rbusHandle_t handle)
{
    if(areAllDepsUp(WAN_DEPS, WAN_DEPS_COUNT))
    {
        publishReadyEvent(handle, "wan_ready_to_go");
    }
}
