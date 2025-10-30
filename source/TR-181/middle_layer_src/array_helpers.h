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
#ifndef __ARRAY_HELPERS_H__
#define __ARRAY_HELPERS_H__

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

typedef int (*process_fn_t)(void *, msgpack_object *);
typedef void (*destroy_fn_t)(void *);

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/

/**
 * @brief Decode msgpack buffer and convert to array structure.
 *
 * This helper function decodes the msgpack buffer, checks for sanity items(including an optional wrapper map),
 * and calls the process function. It also allocates the structure for the caller.
 *
 * @param[in] buf          - Pointer to the buffer to decode.
 * @param[in] len          - The length of the buffer in bytes.
 * @param[in] struct_size  - The size of the structure to allocate and pass to process.
 * @param wrapper          - The optional wrapper to look for & enforce
 * @param expect_type      - The type of object expected
 * @param[in] optional     - If the inner wrapper layer is optional.
 * @param[in] process      - The process function to call if successful.
 * @param[in] destroy      - The destroy function to call if there was an error.
 *
 * @return Pointer to the processed object on success, NULL on error.
 * @retval Non-NULL Pointer to the processed object after successful conversion.
 * @retval NULL on error
 *
 */
void* helper_convert_array( const void *buf, size_t len,
                      size_t struct_size, bool optional,
                      process_fn_t process,
                      destroy_fn_t destroy );

#endif
