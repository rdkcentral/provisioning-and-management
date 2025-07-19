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

#include "webui_doc.h"
#include "webui_webconfig_apis.h"
#include "webui_webconfig_dml.h"

#include "ccsp_base_api.h"
#include "messagebus_interface_helper.h"
#include "ccsp_trace.h"
#include <trower-base64/base64.h>

/***********************************************************************

 APIs for Object:

    Device.X_RDK_WebUI.Data

    *  WebUIconfig_GetParamStringValue
    *  WebUIconfig_SetParamStringValue

***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WebUIconfig_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
WebUIconfig_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
    if (strcmp(ParamName, "Data") == 0)
    {
        CcspTraceWarning(("WebUI config data Get Not supported\n"));
        return 0;
    }
    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WebUIconfig_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
WebUIconfig_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Data") == 0) {
        unsigned char * decodeMsg = NULL;
        int decodeMsgSize = 0;
        int size = 0;

        struct timespec start, end, *startPtr, *endPtr;
        startPtr = &start;
        endPtr = &end;

        msgpack_zone mempool;
        msgpack_object deserialized;
        msgpack_unpack_return unpack_ret;

        getCurrentTime(startPtr);
        decodeMsgSize = b64_get_decoded_buffer_size(strlen(pString));

        decodeMsg = (unsigned char *)malloc(sizeof(unsigned char) * decodeMsgSize);

        size = b64_decode((const uint8_t *)pString, strlen(pString), (uint8_t *)decodeMsg);

        getCurrentTime(endPtr);
        CcspTraceWarning(("Base64 decode Elapsed time : %ld ms\n", timeValDiff(startPtr, endPtr)));

        msgpack_zone_init(&mempool, 2048);
        unpack_ret = msgpack_unpack((const char *)decodeMsg, size, NULL, &mempool, &deserialized);

        switch (unpack_ret)
        {
            case MSGPACK_UNPACK_SUCCESS:
                CcspTraceWarning(("MSGPACK_UNPACK_SUCCESS :%d\n", unpack_ret));
                break;
            case MSGPACK_UNPACK_EXTRA_BYTES:
                CcspTraceWarning(("MSGPACK_UNPACK_EXTRA_BYTES :%d\n", unpack_ret));
                break;
            case MSGPACK_UNPACK_CONTINUE:
                CcspTraceWarning(("MSGPACK_UNPACK_CONTINUE :%d\n", unpack_ret));
                break;
            case MSGPACK_UNPACK_PARSE_ERROR:
                CcspTraceWarning(("MSGPACK_UNPACK_PARSE_ERROR :%d\n", unpack_ret));
                break;
            case MSGPACK_UNPACK_NOMEM_ERROR:
                CcspTraceWarning(("MSGPACK_UNPACK_NOMEM_ERROR :%d\n", unpack_ret));
                break;
            default:
                CcspTraceWarning(("Message Pack decode failed with error: %d\n", unpack_ret));
        }

        msgpack_zone_destroy(&mempool);
        //End of msgpack decoding

        if (unpack_ret == MSGPACK_UNPACK_SUCCESS)
        {
            webui_doc_t *doc;
            doc = webui_doc_convert((const char *)decodeMsg, size + 1);

            if (NULL != doc)
            {
                CcspTraceWarning(("doc->subdoc_name is %s\n", doc->subdoc_name));
                CcspTraceWarning(("doc->version is %lu\n", (unsigned long)doc->version));
                CcspTraceWarning(("doc->transaction_id is %d\n", doc->transaction_id));
                CcspTraceWarning(("WebUI Mode configuration received\n"));

                execData *execDataWebUI = NULL;
                execDataWebUI = (execData *)malloc(sizeof(execData));

                if (execDataWebUI != NULL)
                {
                    memset(execDataWebUI, 0, sizeof(*execDataWebUI));

                    execDataWebUI->txid = doc->transaction_id;
                    execDataWebUI->version = doc->version;

                    strncpy(execDataWebUI->subdoc_name, "webui", sizeof(execDataWebUI->subdoc_name) - 1);

                    execDataWebUI->user_data = (void *)doc;
                    execDataWebUI->calcTimeout = NULL;
                    execDataWebUI->executeBlobRequest = webui_webconfig_process_request;
                    execDataWebUI->rollbackFunc = webui_webconfig_rollback;
                    execDataWebUI->freeResources = webui_webconfig_free_resources;
                    PushBlobRequest(execDataWebUI);

                    CcspTraceWarning(("WebUI PushBlobRequest complete\n"));

                    if (decodeMsg != NULL) {
                        free(decodeMsg);
                    }
                    return TRUE;
                }
                else
                {
                    if (decodeMsg != NULL) {
                        free(decodeMsg);
                    }
                    CcspTraceWarning(("execDataWebUI memory allocation failed\n"));

                    return FALSE;
                }
            }
            if (decodeMsg != NULL) {
                free(decodeMsg);
            }
            return TRUE;
        }
        else
        {
            if (decodeMsg != NULL) {
                free(decodeMsg);
            }
            CcspTraceWarning(("Corrupted WebUI Config value\n"));
            return FALSE;
        }

        return TRUE;
    }
    return FALSE;
}
