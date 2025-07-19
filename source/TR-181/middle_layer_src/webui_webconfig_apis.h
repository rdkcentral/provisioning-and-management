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


pErr webui_webconfig_process_request(void *Data);
int webui_webconfig_rollback();
void webui_webconfig_free_resources(void *arg);
int webui_webconfig_handle_blob(webui_param_t *mode);

#endif // _WEBUI_WEBCONFIG_H