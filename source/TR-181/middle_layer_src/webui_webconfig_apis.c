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
*/

#include "cosa_deviceinfo_apis.h"
#include "webui_doc.h"
#include "webui_webconfig_apis.h"
#include "webconfig_framework.h"
#include <syscfg/syscfg.h>
#include "safec_lib_common.h"

enum WEBUI_MODE getModeFromParam(char *mode)
{
    if (strcmp(mode, "Enable") == 0) {
        return WEBUI_MODE_ENABLE;
    }
    else if (strcmp(mode, "Disable") == 0) {
        return WEBUI_MODE_DISABLE;
    }
#if !defined (_HUB4_PRODUCT_REQ_) && !defined(_SR300_PRODUCT_REQ_)
    else if (strcmp(mode, "MSOonly") == 0) {
        return WEBUI_MODE_MSOONLY;
    }
#endif
    else {
        return -1;
    }
}

int webui_webconfig_handle_blob(webui_param_t *param)
{
    int mode;
    CcspTraceInfo(("Entering webui_webconfig_handle_blob\n"));

    if (param && param->mode)
    {
        mode = getModeFromParam(param->mode);
    }
    else
    {
        CcspTraceWarning(("WebUI Mode is Empty\n"));
        return ANSC_STATUS_FAILURE;
    }

    if (mode == -1) {
#if !defined (_HUB4_PRODUCT_REQ_) && !defined(_SR300_PRODUCT_REQ_)
        CcspTraceInfo(("Invalid WebUI Mode: %s. Mode should be Enable/Disable/MSOonly\n", param->mode));
#else
        CcspTraceInfo(("Invalid WebUI Mode: %s. Mode should be Enable/Disable\n", param->mode));
#endif
        return ANSC_STATUS_FAILURE;
    }

    if (syscfg_set_u_commit(NULL, "WebUIEnable", mode) != 0) {
        CcspTraceWarning(("syscfg_set failed to set WebUIEnable \n"));
        return SYSCFG_FAILURE;
    }
#if !defined (_HUB4_PRODUCT_REQ_) && !defined(_SR300_PRODUCT_REQ_)
    if (mode == WEBUI_MODE_DISABLE || mode == WEBUI_MODE_MSOONLY) {
#else
    if (mode == WEBUI_MODE_DISABLE) {
#endif
        CosaDmlDiSet_DisableRemoteManagement();
    }

    CcspTraceInfo(("WebUI Mode is Set to %s(%d)\n", param->mode, mode));

    return BLOB_EXEC_SUCCESS;
}

/* CallBack API to execute WEBUI Blob request */
pErr webui_webconfig_process_request(void *Data)
{

        pErr execRetVal = NULL;
        errno_t rc = -1;
        int ind = -1;

        execRetVal = (pErr) AnscAllocateMemory (sizeof(Err));
        if (execRetVal == NULL )
        {
            CcspTraceError(("%s : AnscAllocateMemory failed\n",__FUNCTION__));
            return execRetVal;
        }

        rc = memset_s(execRetVal, sizeof(Err), 0, sizeof(Err));
        ERR_CHK(rc);

        execRetVal->ErrorCode = BLOB_EXEC_SUCCESS;

        webui_doc_t *webui = (webui_doc_t *) Data ;
        if( webui != NULL && webui->subdoc_name != NULL && webui->param != NULL )
        {
            CcspTraceInfo(("%s: webui->subdoc_name is %s\n", __FUNCTION__, webui->subdoc_name));
            CcspTraceInfo(("%s: webui->version is %lu\n", __FUNCTION__, (long)webui->version));
            CcspTraceInfo(("%s: webui->transaction_id %lu\n",__FUNCTION__, (long) webui->transaction_id));
            CcspTraceInfo(("%s: webui->param->mode: %s\n", __FUNCTION__, webui->param->mode));

            rc = strcmp_s(WEBUI_WEBCONFIG_SUBDOC_NAME, strlen(WEBUI_WEBCONFIG_SUBDOC_NAME), webui->subdoc_name, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (ind == 0)) {
                int ret = webui_webconfig_handle_blob(webui->param);

                CcspTraceInfo(("%s: Return value = %d\n",__FUNCTION__, ret));
                if ( ret != BLOB_EXEC_SUCCESS )
                {
                    /* CID 559480: Overflowed constant fix - return proper error code */
                    execRetVal->ErrorCode = ANSC_STATUS_FAILURE;
                    if (ret == -1)
                    {
                       strncpy(execRetVal->ErrorMsg, "Invalid WebUI Mode", sizeof(execRetVal->ErrorMsg)-1);
                    }
                    else if (ret == SYSCFG_FAILURE)
                    {
                       strncpy(execRetVal->ErrorMsg, "syscfg_set failed for WebUI", sizeof(execRetVal->ErrorMsg)-1);
                    }
                }
            }
            else
            {
                CcspTraceWarning(("%s: Received an invalid subdoc: %s\n",__FUNCTION__, webui->subdoc_name));
                execRetVal->ErrorCode = SUBDOC_NOT_SUPPORTED;
            }
        }
        else
        {
            CcspTraceWarning(("%s: Received null subdoc blob\n",__FUNCTION__));
            execRetVal->ErrorCode = NULL_BLOB_EXEC_POINTER;
        }

        return execRetVal;
}

/* Callback function to rollback when webui blob execution fails */
int webui_webconfig_rollback()
{
    // return 0 to notify framework when rollback is success
    CcspTraceInfo((" Entering %s \n",__FUNCTION__));

    int ret = 0;

    CcspTraceWarning(("%s: Something went wrong while processing webconfig request\n",__FUNCTION__));

    return ret ;
}

/* Callback function to free webconfig resources */
void webui_webconfig_free_resources(void *arg)
{

    CcspTraceInfo((" Entering %s \n",__FUNCTION__));
    execData *blob_exec_data  = (execData*) arg;

    /* CID 158652: Dereference before null check */
    if ( blob_exec_data != NULL )
    {
        webui_doc_t *doc = (webui_doc_t *) blob_exec_data->user_data ;

        if ( doc != NULL )
        {
            webui_doc_destroy(doc);
        }

        AnscFreeMemory (blob_exec_data);
        blob_exec_data = NULL ;
    }
}