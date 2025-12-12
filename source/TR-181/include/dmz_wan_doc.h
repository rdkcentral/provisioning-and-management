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
#ifndef __WANPARAM_H__
#define __WANPARAM_H__
#include <stdint.h>
#include <stdlib.h>
#include <msgpack.h>
typedef struct
{
    bool     enable;
    char *   internal_ip;
} dmz_wanparam_t;

typedef struct {
    dmz_wanparam_t  *param;
    char *       subdoc_name;
    uint32_t     version;
    uint16_t     transaction_id;
} dmz_wandoc_t;
/**
 * @brief Convert a msgpack buffer into a dmz_wandoc_t structure.
 *
 * This function decodes the msgpack buffer, validates the structure, and converts it
 * into a dmz_wandoc_t object.
 *
 * @param[in] buf  - Pointer to the msgpack buffer to convert.
 *                 \n The buffer must contain valid msgpack-encoded DMZ WAN configuration data.
 * @param[in] len  - The length of the buffer in bytes.
 *
 * @return Pointer to a newly allocated dmz_wandoc_t structure
 * @retval NOT NULL Pointer to the populated dmz_wandoc_t structure.
 * @retval NULL if memory allocation fails, buffer is invalid, or msgpack decoding fails.
 *
 */
dmz_wandoc_t* dmz_wandoc_convert( const void *buf, size_t len );

/**
 * @brief Destroy a dmz_wandoc_t object and free all allocated memory.
 *
 * This function releases all memory allocated for the dmz_wandoc_t structure.
 *
 * @param[in] d  - Pointer to the dmz_wandoc_t structure to destroy.
 *
 * @return None.
 *
 */
void dmz_wandoc_destroy( dmz_wandoc_t *d );

/**
 * @brief Get a descriptive error message for a given error number.
 *
 * This function returns a human-readable string describing the error condition
 * indicated by the error number.
 *
 * @param[in] errnum  - The error number to inspect.
 *                    \n Valid values include: OK (0), OUT_OF_MEMORY, INVALID_FIRST_ELEMENT,
 *                    \n INVALID_VERSION, INVALID_OBJECT.
 *
 * @return Pointer to a constant string describing the error.
 * @retval "No errors." if errnum is OK (0).
 * @retval "Out of memory." if errnum is OUT_OF_MEMORY.
 * @retval "Invalid first element." if errnum is INVALID_FIRST_ELEMENT.
 * @retval "Invalid 'version' value." if errnum is INVALID_VERSION.
 * @retval "Invalid 'value' array." if errnum is INVALID_OBJECT.
 * @retval "Unknown error." if errnum does not match any known error code.
 *
 */
const char* wandoc_strerror( int errnum );
#endif

