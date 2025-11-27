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
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#ifndef __HOTSPOTDOC_H__
#define __HOTSPOTDOC_H__

#include <stdint.h>
#include <stdlib.h>
#include <msgpack.h>

#include "libHotspotApi.h"

/**
* @brief Converts a msgpack buffer into a hotspotparam_t structure.
*
* This function converts a msgpack buffer into an hotspotparam_t structure if possible.
*
* @param[in] buf - Pointer to the buffer to convert.
* @param[in] len - The length of the buffer in bytes.
*
* @return The status of the operation.
* @retval Pointer to hotspotparam_t structure if the operation is successful.
* @retval NULL if any error is detected during the operation.
*
*/
hotspotparam_t* hotspotdoc_convert( const void *buf, size_t len );

/**
* @brief Returns a general reason why the conversion failed.
*
* This function returns a constant string describing the error.
*
* @param[in] errnum - The errno value to inspect.
*
* @return The status of the operation.
* @retval Constant string describing the error if the operation is successful.
* @retval unknown error otherwise
*
*/
const char* hotspotdoc_strerror( int errnum );

/**
* @brief Destroys a hotspotparam_t object.
*
* This function frees all memory associated with the hotspotparam_t object and destroys it.
*
* @param[in] hd - Pointer to the hotspotparam_t object to destroy.
*
* @return None
*
*/
void hotspotdoc_destroy( hotspotparam_t *hd );

/**
* @brief Converts a msgpack buffer into a tunneldoc_t structure.
*
* This function converts a msgpack buffer into a tunneldoc_t structure if possible.
*
* @param[in] buf - Pointer to the buffer to convert.
* @param[in] len - The length of the buffer in bytes.
*
* @return The status of the operation.
* @retval Pointer to tunneldoc_t structure if the operation is successful.
* @retval NULL if any error is detected during the operation.
*
*/
tunneldoc_t* tunneldoc_convert(const void *buf, size_t len);

/**
* @brief Destroys a tunneldoc_t object.
*
* This function frees all memory associated with the tunneldoc_t object and destroys it.
*
* @param[in] td - Pointer to the tunneldoc_t object to destroy.
*
* @return None.
*
*/
void tunneldoc_destroy( tunneldoc_t *td );

/**
* @brief Converts a msgpack buffer into a wifi_doc_t structure.
*
* This function converts a msgpack buffer into a wifi_doc_t structure if possible.
*
* @param[in] buf - Pointer to the buffer to convert.
* @param[in] len - The length of the buffer in bytes.
*
* @return The status of the operation.
* @retval Pointer to wifi_doc_t structure if the operation is successful.
* @retval NULL if any error is detected during the operation.
*
*/
wifi_doc_t* wifi_doc_convert(const void *buf, size_t len);

/**
* @brief Destroys a wifi_doc_t object.
*
* This function frees all memory associated with the wifi_doc_t object and destroys it.
*
* @param[in] wd - Pointer to the wifi_doc_t object to destroy.
*
* @return None.
*
*/
void wifi_doc_destroy( wifi_doc_t *wd );

#endif
