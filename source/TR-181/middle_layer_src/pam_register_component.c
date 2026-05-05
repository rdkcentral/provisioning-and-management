#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>



#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlerror.h>

#include <rbus/rbus.h>
#include "rbuscore.h"
#include <errno.h>

#include "pam_register_component.h"
#include "ccsp_trace.h"

#define CCSP_ETHWAN_ENABLE "/nvram/ETHWAN_ENABLE"
#define CCSP_CR_ETHWAN_DEVICE_PROFILE_XML_FILE "/usr/ccsp/cr-ethwan-deviceprofile.xml"
#define CCSP_CR_DEVICE_PROFILE_XML_FILE "/usr/ccsp/cr-deviceprofile.xml"

#if defined(_SCER11BEL_PRODUCT_REQ_)
#define CCSP_USE_ETHWAN_PROFILE 1
#else
#define CCSP_USE_ETHWAN_PROFILE 0
#endif

#define MAX_COMPONENTS 20
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

static bool isSystemReady = false;
/* ----------------------------------------------------------- */
/* RBUS COMPONENT CHECK */
static bool isComponentRegisteredInRbus(const char* name)
{
    int count = 0;
    char** components = NULL;

    if(!name)
    {
        CcspTraceError(("[PAM] NULL component name passed to RBUS check\n"));
        return false;
    }

    char fullName[256] = {0};
    snprintf(fullName, sizeof(fullName), "eRT.%s", name);

    if(rbus_discoverRegisteredComponents(&count, &components) != RBUSCORE_SUCCESS)
    {
        CcspTraceError(("[PAM] rbus_discoverRegisteredComponents failed\n"));
        return false;
    }

    bool found = false;

    for(int i = 0; i < count; i++)
    {
        if(!components[i])
            continue;

        if(strcmp(components[i], fullName) == 0 ||
           strcmp(components[i], name) == 0)
        {
            found = true;
        }

        free(components[i]);
    }
    free(components);

    return found;
}
static bool isComponentInList(const char* name, char** list, int count)
{
    if(!name || !list)
        return false;

    char fullName[256] = {0};
    snprintf(fullName, sizeof(fullName), "eRT.%s", name);

    for(int i = 0; i < count; i++)
    {
        if(!list[i])
            continue;

        /* Match with eRT prefix */
        if(strcmp(list[i], fullName) == 0)
        {
            return true;
        }

        /* Match without prefix (CR case) */
        if(strcmp(list[i], name) == 0)
        {
            return true;
        }
    }
    return false;
}
static bool areAllComponentsReady()
{
    int count = 0;
    char** components = NULL;

    if(rbus_discoverRegisteredComponents(&count, &components) != RBUSCORE_SUCCESS)
    {
        CcspTraceError(("[PAM] Failed to fetch RBUS components\n"));
        return false;
    }

    for(int i = 0; i < g_componentCount; i++)
    {
        const char* comp = g_components[i].name;

        if(!isComponentInList(comp, components, count))
        {
            CcspTraceInfo(("[PAM] Component NOT ready: %s\n", comp));
            
            goto cleanup_and_return_false;
        }
        else
        {
            CcspTraceInfo(("[PAM] Component ready: %s\n", comp));
        }

        for(int j = 0; j < g_components[i].depCount; j++)
        {
            const char* dep = g_components[i].deps[j];

            if(!isComponentInList(dep, components, count))
            {
                CcspTraceInfo(("[PAM] Dependency NOT ready: %s (for %s)\n", dep, comp));
                
                goto cleanup_and_return_false;
            }
            else
            {
                CcspTraceInfo(("[PAM] Dependency ready: %s (for %s)\n", dep, comp));
            }
        }
    }

    CcspTraceInfo(("[PAM] All components + dependencies READY\n"));

    /* cleanup */
    for(int i = 0; i < count; i++)
        free(components[i]);
    free(components);

    return true;

cleanup_and_return_false:

    for(int i = 0; i < count; i++)
        free(components[i]);
    free(components);

    return false;
}
/* ----------------------------------------------------------- */
/* XML PARSE */
static void parseDeviceProfile()
{
    const char* fileName = NULL;

    if(CCSP_USE_ETHWAN_PROFILE || (access(CCSP_ETHWAN_ENABLE, F_OK) == 0))
    {
        fileName = CCSP_CR_ETHWAN_DEVICE_PROFILE_XML_FILE;
        CcspTraceInfo(("[PAM] Using ETHWAN device profile: %s\n", fileName));
    }
    else
    {
        fileName = CCSP_CR_DEVICE_PROFILE_XML_FILE;
        CcspTraceInfo(("[PAM] Using DEFAULT device profile: %s\n", fileName));
    }

    const char* fullPath = fileName;

    CcspTraceInfo(("[PAM] Parsing XML: %s\n", fullPath));
    CcspTraceInfo(("[PAM] Running as UID=%d GID=%d\n", getuid(), getgid()));

    if(access(fullPath, F_OK) != 0)
    {
        CcspTraceError(("[PAM] XML file NOT FOUND at path: %s\n", fullPath));
        return;
    }
    else
    {
        CcspTraceInfo(("[PAM] XML file FOUND\n"));
    }

    xmlInitParser();
    FILE* fp = fopen(fullPath, "rb");
    if(!fp)
    {
        CcspTraceError(("[PAM] fopen failed for %s errno=%d (%s)\n", fullPath, errno, strerror(errno)));
        return;
    }
    else
    {
        CcspTraceInfo(("[PAM] fopen SUCCESS\n"));
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if(fileSize <= 0)
    {
        CcspTraceError(("[PAM] File is empty or ftell failed: %s\n", fullPath));
        fclose(fp);
        return;
    }

    char* xmlBuf = (char*)malloc((size_t)fileSize);
    if(!xmlBuf)
    {
        CcspTraceError(("[PAM] malloc failed for XML buffer\n"));
        fclose(fp);
        return;
    }

    size_t bytesRead = fread(xmlBuf, 1, (size_t)fileSize, fp);
    fclose(fp);

    if((long)bytesRead != fileSize)
    {
        CcspTraceError(("[PAM] fread read %zu of %ld bytes from %s\n",
            bytesRead, fileSize, fullPath));
        free(xmlBuf);
        return;
    }

    xmlDocPtr doc = xmlReadMemory(xmlBuf, (int)fileSize, fullPath, NULL, 0);

    free(xmlBuf);

    if(!doc)
    {
        const xmlError* err = xmlGetLastError();
        if(err)
        {
            CcspTraceError(("[PAM] XML parse error: %s (line %d)\n",
                err->message, err->line));
        }
        else
        {
            CcspTraceError(("[PAM] Unknown XML parse error for %s\n", fullPath));
        }
        return;
    }
    CcspTraceInfo(("[PAM] XML parsed into memory successfully\n"));

    xmlNodePtr root = xmlDocGetRootElement(doc);

    if(!root)
    {
        CcspTraceError(("[PAM] XML root is NULL\n"));
        xmlFreeDoc(doc);
        return;
    }

    CcspTraceInfo(("[PAM] XML root node: %s\n", (char*)root->name));

    
    for(xmlNodePtr comps = root->children; comps; comps = comps->next)
    {
        if(comps->type != XML_ELEMENT_NODE || xmlStrcmp(comps->name, (xmlChar*)"components"))
            continue;

        CcspTraceInfo(("[PAM] Found <components> section\n"));

        for(xmlNodePtr comp = comps->children; comp; comp = comp->next)
        {
            if(comp->type != XML_ELEMENT_NODE || xmlStrcmp(comp->name, (xmlChar*)"component"))
                continue;

            if(g_componentCount >= MAX_COMPONENTS)
            {
                CcspTraceError(("[PAM] Max component limit reached\n"));
                break;
            }

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
                    CcspTraceInfo(("[PAM] Parsed name: %s\n", c->name));
                    xmlFree(val);
                }
                else if(!xmlStrcmp(field->name, (xmlChar*)"event"))
                {
                    xmlChar* val = xmlNodeGetContent(field);
                    c->eventName = strdup((char*)val);
                    CcspTraceInfo(("[PAM] Parsed event: %s\n", c->eventName));
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
                                c->deps[c->depCount] = strdup((char*)val);

                                CcspTraceInfo(("[PAM] Parsed dep[%d]: %s\n",
                                    c->depCount, c->deps[c->depCount]));

                                c->depCount++;
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
            else
            {
                CcspTraceError(("[PAM] Skipping component with NULL name\n"));
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
    if(!eventName)
    {
        CcspTraceError(("[PAM] Cannot publish NULL event\n"));
        return;
    }

    const int timeoutSec = 15;   
    const int intervalSec = 1;   

    time_t startTime = time(NULL);
    int attempt = 0;

    while(1)
    {
        attempt++;

        CcspTraceInfo(("[PAM] Publishing event: %s (attempt=%d)\n", eventName, attempt));

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

        CcspTraceInfo(("[PAM] Publish result for %s rc=%d\n", eventName, rc));

        rbusValue_Release(newVal);
        rbusValue_Release(oldVal);
        rbusObject_Release(data);

        if(rc == RBUS_ERROR_SUCCESS)
        {
            CcspTraceInfo(("[PAM] Event delivered successfully\n"));
            break;
        }

        if(rc == RBUS_ERROR_NOSUBSCRIBERS)
        {
            if(time(NULL) - startTime >= timeoutSec)
            {
                CcspTraceError(("[PAM] Timeout reached, no subscribers for %s\n", eventName));
                break;
            }

            CcspTraceInfo(("[PAM] No subscribers yet, retrying...\n"));
            sleep(intervalSec);
            continue;
        }

        
        CcspTraceError(("[PAM] Publish failed with rc=%d, stopping\n", rc));
        break;
    }
}

/* ----------------------------------------------------------- */
/* EVENT HANDLERS */
static rbusError_t systemReadyGetHandler(
    rbusHandle_t handle,
    rbusProperty_t property,
    rbusGetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;

    const char* name = rbusProperty_GetName(property);

    if(strcmp(name, "Device.CR_PAM.SystemReady") == 0)
    {
        rbusValue_t value;
        rbusValue_Init(&value);

        rbusValue_SetBoolean(value, isSystemReady);

        rbusProperty_SetValue(property, value);
        rbusValue_Release(value);

        CcspTraceInfo(("[PAM] GET Device.CR_PAM.SystemReady = %d\n", isSystemReady));

        return RBUS_ERROR_SUCCESS;
    }

    return RBUS_ERROR_ELEMENT_DOES_NOT_EXIST;
}
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

    CcspTraceInfo(("[PAM] Subscription action=%d for event=%s\n", action, eventName));

    if(autoPublish)
        *autoPublish = false;

    return RBUS_ERROR_SUCCESS;
}

/* ----------------------------------------------------------- */
/* REGISTER EVENTS */
void registerPamEvents(rbusHandle_t handle)
{
    CcspTraceInfo(("[PAM] registerPamEvents start\n"));

    rbusDataElement_t dataElements[3] =
    {
        { "wifi_ready_to_go", RBUS_ELEMENT_TYPE_EVENT,
            { eventGetHandler, NULL, NULL, NULL, eventSubHandler, NULL } },

        { "wan_ready_to_go", RBUS_ELEMENT_TYPE_EVENT,
            { eventGetHandler, NULL, NULL, NULL, eventSubHandler, NULL } },

        
        { "Device.CR_PAM.SystemReady", RBUS_ELEMENT_TYPE_PROPERTY,
            { systemReadyGetHandler, NULL, NULL, NULL, NULL, NULL } }
    };

    rbusError_t rc = rbus_regDataElements(handle, 3, dataElements);

    CcspTraceInfo(("[PAM] Event registration rc=%d\n", rc));
    CcspTraceInfo(("PAM: registerPamEvents() completed\n"));

    CcspTraceInfo(("[PAM] Starting dependency monitoring threads...\n"));
    pam_startDependencyMonitoring(handle);
}
/* ----------------------------------------------------------- */
/* SYSTEM READY + EVENT PUBLISH */
static void* monitorSystemReady(void* arg)
{
    rbusHandle_t handle = (rbusHandle_t)arg;

    CcspTraceInfo(("[PAM] SystemReady monitor thread started\n"));

    while(!isSystemReady)
    {
        if(areAllComponentsReady())
        {
            isSystemReady = true;

            CcspTraceInfo(("[PAM] ALL components ready → Device.CR_PAM.SystemReady = TRUE\n"));

            
            for(int i = 0; i < g_componentCount; i++)
            {
                PamComponent_t* comp = &g_components[i];

                if(comp->eventName)
                {
                    comp->handle = handle;
                    bool ready = true;

                    if(!isComponentRegisteredInRbus(comp->name))
                        ready = false;

                    for(int j = 0; j < comp->depCount && ready; j++)
                    {
                        if(!isComponentRegisteredInRbus(comp->deps[j]))
                            ready = false;
                    }

                    if(ready)
                    {
                        CcspTraceInfo(("[PAM] Publishing event after SystemReady: %s\n",
                            comp->eventName));

                        publishReadyEvent(handle, comp->eventName);
                    }
                    else
                    {
                        CcspTraceError(("[PAM] Unexpected: %s not ready even after SystemReady\n",
                            comp->name));
                    }
                }
            }

            break;
        }

        sleep(2);
    }

    CcspTraceInfo(("[PAM] SystemReady thread exiting\n"));
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

    CcspTraceInfo(("[PAM] Starting dependency monitoring (SINGLE THREAD MODE)...\n"));

    parseDeviceProfile();

  
    pthread_t sysTid;
    pthread_create(&sysTid, NULL, monitorSystemReady, handle);
    pthread_detach(sysTid);
}