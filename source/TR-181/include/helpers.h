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
#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/* none */
#define match(p, s) strncmp((p)->key.via.str.ptr, s, (p)->key.via.str.size)
#define member_size(type, member) sizeof(((type *)0)->member)

/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
enum {
    HELPERS_OK = 0,
    HELPERS_OUT_OF_MEMORY,
    HELPERS_INVALID_FIRST_ELEMENT,
    HELPERS_MISSING_WRAPPER
};

typedef int (*process_fn_t)(void *, int, ...);
typedef void (*destroy_fn_t)(void *);

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/

/**
* @brief Helper function that decodes msgpack buffer, validates, and processes it.
*
* This function decodes the msgpack buffer, checks for a few sanity items
* (including an optional wrapper map) before calling the process function passed in.
* This also allocates the structure for the caller.
*
* @param[in] buf  - Pointer to the buffer containing msgpack data to decode.
* @param[in] len  - Length of the buffer in bytes.
* @param[in] struct_size  - Size of the structure to allocate and pass to process function.
* @param[in] wrapper  - Optional wrapper string to look for and enforce (can be NULL).
* @param[in] expect_type  - Type of msgpack object expected.
* @param[in] optional  - If true, the inner wrapper layer is optional.
* @param[in] process  - Process function to call if successful to populate the allocated structure.
* @param[in] destroy  - Destroy function to call to clean up if there was an error.
*
* @return Pointer to the processed object
* @retval Pointer to the object after process function has populated it on success,
* @retval NULL on error (memory allocation failure, invalid format, missing wrapper).
*
*/
void* helper_convert( const void *buf, size_t len,
                      size_t struct_size, const char *wrapper,
                      msgpack_object_type expect_type, bool optional,
                      process_fn_t process,
                      destroy_fn_t destroy );

/**
 * @brief Simple helper function that decodes msgpack array, validates, and processes it.
 *
 * This function decodes the msgpack buffer as an array, checks for a few sanity items
 * (including an optional wrapper map) before calling the process function passed in.
 * This also allocates the structure for the caller.
 *
 * @param[in] buf Buffer containing msgpack data to decode.
 * @param[in] len Length of the buffer in bytes.
 * @param[in] struct_size Size of the structure to allocate and pass to process function.
 * @param[in] wrapper Optional wrapper string to look for and enforce (can be NULL).
 * @param[in] expect_type Type of msgpack object expected in the array.
 * @param[in] optional If true, the inner wrapper layer is optional.
 * @param[in] process Process function to call if successful to populate the allocated structure.
 * @param[in] destroy Destroy function to call to clean up if there was an error.
 *
 * @return Pointer to the processed object.
 * @retval Pointer to the object after process function has populated it on success.
 * @retval NULL on error (memory allocation failure, invalid format, missing wrapper).
 */
void* helper_convert_array( const void *buf, size_t len,
                      size_t struct_size, const char *wrapper,
                      msgpack_object_type expect_type, bool optional,
                      process_fn_t process,
                      destroy_fn_t destroy );

#endif
