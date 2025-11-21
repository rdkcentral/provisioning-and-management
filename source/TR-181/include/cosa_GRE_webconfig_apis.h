/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __HOTSPOT_WEBCONFIG_PARAM_H__
#define __HOTSPOT_WEBCONFIG_PARAM_H__

#include "ccsp_base_api.h"
#include "messagebus_interface_helper.h"
#include "msgpack.h"
#include "webcfgparam.h"
#include "hotspot_helpers.h"
#include "hotspotdoc.h"
#include "ccsp_trace.h"
#include "cosa_webconfig_api.h"
#include "libHotspotApi.h"
#include <syscfg/syscfg.h>

/**
* @brief Unpack and process hotspot webconfig data.
*
* This function decodes the input base64 string, unpacks the msgpack data,
* extracts GRE tunnel and WiFi VAP configurations, validates their consistency,
* and triggers a multi-component execution request to apply the hotspot settings.
*
* @param[in] pString - Pointer to a null-terminated string containing the base64-encoded
*                      hotspot configuration data in msgpack format.
*
* @return The status of the operation.
* @retval TRUE if the data is successfully unpacked, validated, and queued for processing.
* @retval FALSE if decoding fails, msgpack unpacking fails, validation fails, or if the data is corrupted.
*
*/
BOOL unpackAndProcessHotspotData(char* pString);

/**
* @brief Initialize the hotspot webconfig subsystem.
*
* This function initializes the hotspot webconfig functionality by checking if the
* WebConfig RFC is enabled and if a hotspot blob file exists.
*
* @return void
*
*/
void wbInitializeHotspot();

/**
* @brief Free memory allocated for hotspot webconfig execution.
*
* This function frees all memory resources allocated during hotspot webconfig processing,
* including execution data structures, policy sequence details, tunnel document data,
* WiFi encoded data, and multi-component execution information.
*
* @param[in] arg - Pointer to the execData structure containing hotspot execution data
*                  and associated resources to be freed.
*
* @return void
*
*/
void freeMem_hotspot(void* arg);

#endif
