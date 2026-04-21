#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <rbus/rbus.h>
#include "rbuscore.h"

#include "pam_register_component.h"
#include "ccsp_trace.h"

/* ----------------------------------------------------------- */
#define MAX_COMPONENTS 10
#define MAX_DEPS       10

typedef struct {
    char* name;
    char* deps[MAX_DEPS];
    int depCount;
    char* eventName;
    rbusHandle_t handle;
} PamComponent_t;

static PamComponent_t g_components[MAX_COMPONENTS];
static int g_componentCount = 0;

/* ----------------------------------------------------------- */
/* RBUS COMPONENT CHECK */
static bool isComponentRegisteredInRbus(const char* name)
{
    int count = 0;
    char** components = NULL;

    if(rbus_discoverRegisteredComponents(&count, &components) != RBUSCORE_SUCCESS)
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
/* XML PARSE */
static void parseDeviceProfile()
{
    const char* fileName = "/usr/ccsp/cr-deviceprofile.xml";  // correct path

    CcspTraceInfo(("[PAM] Parsing XML: %s\n", fileName));

    xmlDocPtr doc = xmlParseFile(fileName);
    if(!doc)
    {
        CcspTraceError(("[PAM] Failed to parse XML %s\n", fileName));
        return;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);

    for(xmlNodePtr comps = root->children; comps; comps = comps->next)
    {
        if(comps->type != XML_ELEMENT_NODE || xmlStrcmp(comps->name, (xmlChar*)"components"))
            continue;

        for(xmlNodePtr comp = comps->children; comp; comp = comp->next)
        {
            if(comp->type != XML_ELEMENT_NODE || xmlStrcmp(comp->name, (xmlChar*)"component"))
                continue;

            if(g_componentCount >= MAX_COMPONENTS)
                break;

            PamComponent_t* c = &g_components[g_componentCount];
            memset(c, 0, sizeof(PamComponent_t));

            for(xmlNodePtr field = comp->children; field; field = field->next)
            {
                if(field->type != XML_ELEMENT_NODE)
                    continue;

                if(!xmlStrcmp(field->name, (xmlChar*)"name"))
                {
                    xmlChar* val = xmlNodeGetContent(field);
                    c->name = strdup((char*)val);
                    xmlFree(val);
                }
                else if(!xmlStrcmp(field->name, (xmlChar*)"event"))
                {
                    xmlChar* val = xmlNodeGetContent(field);
                    c->eventName = strdup((char*)val);
                    xmlFree(val);
                }
                else if(!xmlStrcmp(field->name, (xmlChar*)"dependencies"))
                {
                    for(xmlNodePtr dep = field->children; dep; dep = dep->next)
                    {
                        if(dep->type == XML_ELEMENT_NODE &&
                           !xmlStrcmp(dep->name, (xmlChar*)"dependency"))
                        {
                            if(c->depCount < MAX_DEPS)
                            {
                                xmlChar* val = xmlNodeGetContent(dep);
                                c->deps[c->depCount++] = strdup((char*)val);
                                xmlFree(val);
                            }
                        }
                    }
                }
            }

            if(c->name)
            {
                CcspTraceInfo(("[PAM] Loaded comp=%s deps=%d event=%s\n",
                    c->name,
                    c->depCount,
                    c->eventName ? c->eventName : "NONE"));

                g_componentCount++;
            }
        }
    }

    CcspTraceInfo(("[PAM] Total components loaded: %d\n", g_componentCount));

    xmlFreeDoc(doc);
}

/* ----------------------------------------------------------- */
/* EVENT PUBLISH */
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

    CcspTraceInfo(("[PAM] Publish %s rc=%d\n", eventName, rc));
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
    CcspTraceInfo(("[PAM] registerPamEvents start\n"));

    /* old RBUS registration (required) */
    rbusDataElement_t dataElements[2] =
    {
        { "wifi_ready_to_go", RBUS_ELEMENT_TYPE_EVENT,
            { eventGetHandler, NULL, NULL, NULL, eventSubHandler, NULL } },

        { "wan_ready_to_go", RBUS_ELEMENT_TYPE_EVENT,
            { eventGetHandler, NULL, NULL, NULL, eventSubHandler, NULL } }
    };

    rbus_regDataElements(handle, 2, dataElements);

    CcspTraceInfo(("PAM: Starting dependency monitoring threads...\n"));
    pam_startDependencyMonitoring(handle);
}
/* ----------------------------------------------------------- */
/* THREAD */
static void* monitorComponentReady(void* arg)
{
    PamComponent_t* comp = (PamComponent_t*)arg;

    CcspTraceInfo(("[PAM] Thread started for %s\n", comp->name));

    while(1)
    {
        bool ready = true;

        /* STEP 1: check component itself */
        if(!isComponentRegisteredInRbus(comp->name))
        {
            CcspTraceInfo(("[PAM] %s NOT ready\n", comp->name));
            ready = false;
        }

        /* STEP 2: check dependencies */
        for(int i = 0; i < comp->depCount && ready; i++)
        {
            if(!isComponentRegisteredInRbus(comp->deps[i]))
            {
                CcspTraceInfo(("[PAM] %s waiting for dep: %s\n",
                    comp->name, comp->deps[i]));
                ready = false;
                break;
            }
        }

        if(ready)
        {
            CcspTraceInfo(("[PAM] %s + deps READY\n", comp->name));

            if(comp->eventName)
                publishReadyEvent(comp->handle, comp->eventName);

            break;
        }

        sleep(1);
    }

    CcspTraceInfo(("[PAM] Thread exiting for %s\n", comp->name));
    return NULL;
}

/* ----------------------------------------------------------- */
/* START MONITORING */
void pam_startDependencyMonitoring(rbusHandle_t handle)
{
    if(!handle)
    {
        CcspTraceError(("[PAM] Invalid RBUS handle\n"));
        return;
    }

    CcspTraceInfo(("[PAM] Starting dependency monitoring...\n"));

    parseDeviceProfile();

    for(int i = 0; i < g_componentCount; i++)
    {
        PamComponent_t* comp = &g_components[i];

        if(comp->eventName) // only event-based components
        {
            comp->handle = handle;

            pthread_t tid;
            pthread_create(&tid, NULL, monitorComponentReady, comp);
            pthread_detach(tid);

            CcspTraceInfo(("[PAM] Thread created for %s\n", comp->name));
        }
    }
}