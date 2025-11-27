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

#ifndef __WEBUI_PARAM_H__
#define __WEBUI_PARAM_H__
#include <stdint.h>
#include <stdlib.h>
#include <msgpack.h>

#define WEBUI_WEBCONFIG_SUBDOC_NAME "webui"

enum WEBUI_MODE {
    WEBUI_MODE_DISABLE,
    WEBUI_MODE_ENABLE,
#if !defined (_HUB4_PRODUCT_REQ_) && !defined(_SR300_PRODUCT_REQ_)
    WEBUI_MODE_MSOONLY
#endif
};

typedef struct
{
    char* mode;
} webui_param_t;

typedef struct {
    webui_param_t  *param;
    char*          subdoc_name;
    uint32_t       version;
    uint16_t       transaction_id;
} webui_doc_t;
/**
 * @brief Converts a msgpack buffer into a webui_doc_t structure.
 *
 *  @param[in] buf the buffer to convert
 *  @param[in] len the length of the buffer in bytes
 *
 * @return Pointer to the converted webui_doc_t structure.
 * @retval Pointer to webui_doc_t on success.
 * @retval NULL if an error occurred.
 */
webui_doc_t* webui_doc_convert( const void *buf, size_t len );
/**
 * @brief Destroys a webui_doc_t object.
 *
 *  @param[in] d the webui_doc_t object to destroy.
 *
 * @return None.
 */
void webui_doc_destroy( webui_doc_t *d );
/**
 * @brief Returns a general reason why the conversion failed.
 *
 *  @param[in] errnum the errno value to inspect.
 *
 * @return Constant string describing the error.
 * @retval Error description string (do not alter or free)
 * @retval unknown error
 */
const char* webui_doc_strerror( int errnum );
#endif