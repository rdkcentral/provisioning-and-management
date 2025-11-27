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

#ifndef  _WEBUI_WEBCONFIG_H
#define  _WEBUI_WEBCONFIG_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "ansc_status.h"
#include "ansc_platform.h"

#include "webconfig_framework.h"
#include "webui_doc.h"

/**
 * @brief Processes a WebUI webconfig request.
 *
 * This function handles incoming WebUI configuration from webconfig framework.
 *
 * @param[in] Data Pointer to webconfig request data.
 *
 * @return Pointer to error structure indicating the result of the WebUI blob execution
 * @retval Non-NULL Pointer to error structure if Memory allocation success.
 * @retval NULL if memory allocation fails
 */
pErr webui_webconfig_process_request(void *Data);
/**
 * @brief Rolls back WebUI webconfig to previous state.
 *
 * This function restores WebUI configuration from backup when a configuration update fails.
 *
 * @return The status of the operation.
 * @retval 0 if successful.
 * @retval Non-zero error code if any error is detected.
 */
int webui_webconfig_rollback();
/**
 * @brief Frees resources allocated for WebUI webconfig processing.
 *
 * This function releases memory and resources associated with WebUI webconfig request.
 *
 * @param[in] arg Pointer to resource data to free.
 *
 * @return void
 */
void webui_webconfig_free_resources(void *arg);
/**
 * @brief Handles WebUI configuration blob data.
 *
 * This function processes the WebUI parameter blob and applies configuration changes.
 *
 * @param[in] mode Pointer to webui_param_t structure containing WebUI configuration data.
 *
 * @return The status of the operation.
 * @retval 0 if successful.
 * @retval Non-zero error code if error detected.
 */
int webui_webconfig_handle_blob(webui_param_t *mode);

#endif // _WEBUI_WEBCONFIG_H