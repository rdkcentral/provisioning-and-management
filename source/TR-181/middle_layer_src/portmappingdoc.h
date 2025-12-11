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
#ifndef __PORTMAPPINGDOC_H__
#define __PORTMAPPINGDOC_H__

#include <stdint.h>
#include <stdlib.h>
#include <msgpack.h>
typedef struct
{
    char      *internal_client;
    char      *enable;
    char  *external_port_end_range;
    char      *protocol;
    char      *description;
    char *external_port;
} portdoc_t;

typedef struct {
    portdoc_t *entries;
    size_t      entries_count;
    char * subdoc_name;
    uint32_t  version;
    uint16_t   transaction_id;
} portmappingdoc_t;

/**
 * @brief Converts a msgpack buffer into a portmappingdoc_t structure.
 *
 *  @param[in] buf the buffer to convert
 *  @param[in] len the length of the buffer in bytes
 *
 * @return Pointer to the converted portmappingdoc_t structure.
 * @retval Valid pointer to portmappingdoc_t on success.
 * @retval NULL if an error occurred.
 */
portmappingdoc_t* portmappingdoc_convert( const void *buf, size_t len );

/**
 * @brief Destroys an portmappingdoc_t object.
 *
 * @param[in] d the portmappingdoc to destroy
 *
 * @return None
 */
void portmappingdoc_destroy( portmappingdoc_t *d );

/**
 * @brief Returns a general reason why the conversion failed.
 *
 * @param[in] errnum the errno value to inspect
 *
 * @return Constant string describing the error.
 * @retval Error description string (do not alter or free).
 * @retval Unknown error otherwise
 */
const char* portmappingdoc_strerror( int errnum );

#endif
