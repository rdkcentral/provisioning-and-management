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
#ifndef __LAN_WEBCONFIG_PARAM_H__
#define __LAN_WEBCONFIG_PARAM_H__
#include <stdint.h>
#include <stdlib.h>
#include <msgpack.h>
typedef struct
{
    bool     dhcp_server_enable;
    char *   lan_ip_address;
    char *   lan_subnet_mask;
    char *   dhcp_start_ip_address;
    char *   dhcp_end_ip_address;
    uint32_t leasetime;
} lanparam_t;

typedef struct {
    lanparam_t  *param;
    size_t entries_count;
    char *       subdoc_name;
    uint32_t     version;
    uint16_t     transaction_id;
} landoc_t;

/**
* @brief Convert a msgpack buffer into a landoc_t structure.
*
* @param[in] buf  - Pointer to the msgpack buffer to convert.
* @param[in] len  - Length of the buffer in bytes.
*
* @return Pointer to the landoc_t structure.
* @retval Pointer to a newly allocated landoc_t structure on success.
* @retval NULL on error.
*
*/
landoc_t* landoc_convert( const void *buf, size_t len );

/**
* @brief Destroy a landoc_t object and free all allocated memory.
*
* @param[in] d  - Pointer to the landoc_t structure to destroy.
*
* @return None.
*
*/
void landoc_destroy( landoc_t *d );

/**
* @brief Get an error message string for a given error number.
*
* @param[in] errnum  - The error number to inspect.
*
* @return Pointer to a constant string describing the error.
* @retval "No errors." if errnum is OK.
* @retval "Out of memory." if errnum is OUT_OF_MEMORY.
* @retval "Invalid first element." if errnum is INVALID_FIRST_ELEMENT.
* @retval "Invalid 'version' value." if errnum is INVALID_VERSION.
* @retval "Invalid 'value' array." if errnum is INVALID_OBJECT.
* @retval "Unknown error." if errnum does not match any known error code.
*
*/
const char* landoc_strerror( int errnum );
#endif


