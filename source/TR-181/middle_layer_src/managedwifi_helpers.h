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
#ifndef __MANAGEDWIFI_HELPERS_H__
#define __MANAGEDWIFI_HELPERS_H__
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <trower-base64/base64.h>
#include <msgpack.h>

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
    MW_HELPERS_OK = 0,
    MW_HELPERS_OUT_OF_MEMORY,
    MW_HELPERS_INVALID_FIRST_ELEMENT,
    MW_HELPERS_MISSING_WRAPPER
};
typedef int (*process_fn_t)(void *, int, ...);
typedef void (*destroy_fn_t)(void *);

//for wifidoc append
typedef struct mg_wifi_appenddoc_struct{
    char * subdoc_name;
    uint32_t  version;
    uint16_t   transaction_id;
    size_t *count;
}mg_wifi_appenddoc_t;

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
/**
 * @brief Helper function to decode msgpack buffer for managed WiFi webconfig.
 *
 * This function decodes a msgpack buffer, validates structure including optional wrapper map,
 * allocates the structure and calls the process function for managed WiFi configurations.
 *
 * @param[in] buf Buffer to decode.
 * @param[in] len Length of buffer in bytes.
 * @param[in] struct_size Size of structure to allocate and pass to process.
 * @param[in] wrapper Optional wrapper name to look for and enforce.
 * @param[in] expect_type Type of msgpack object expected.
 * @param[in] optional Whether the inner wrapper layer is optional.
 * @param[in] process Process function to call if successful.
 * @param[in] destroy Destroy function to call if there was an error.
 *
 * @returns the object after process has done it's magic to it on success
 * @retval Pointer to processed object on success.
 * @retval NULL on error.
 */
void* comp_mw_helper_convert( const void *buf, size_t len,
                      size_t struct_size, const char *wrapper,
                      msgpack_object_type expect_type, bool optional,
                      process_fn_t process,
                      destroy_fn_t destroy );

/**
 * @brief Packs managed WiFi appenddoc structure into msgpack format.
 *
 * This function serializes a managed WiFi appenddoc structure into msgpack binary format for webconfig transmission.
 *
 * @param[in] appenddocData Pointer to mg_wifi_appenddoc_t structure to pack.
 * @param[out] data Pointer to output buffer pointer.
 *
 * @return Size of packed msgpack data in bytes.
 * @retval Size in bytes if successful.
 * @retval 0 on failure.
 */
size_t mg_wifi_pack_appenddoc(const mg_wifi_appenddoc_t *appenddocData,void **data);

/**
 * @brief Appends managed WiFi document with metadata to webconfig.
 *
 * This function creates a complete managed WiFi subdocument with version and transaction tracking for webconfig processing.
 *
 * @param[in] subdoc_name Subdocument name.
 * @param[in] version Subdocument version number.
 * @param[in] trans_id Transaction ID.
 * @param[in] blob_data WiFi configuration blob data.
 * @param[in] blob_size Size of blob data in bytes.
 *
 * @return Pointer to allocated complete document string.
 * @retval Pointer to document string if successful (caller must free).
 * @retval NULL on failure.
 */
char * mg_append_wifi_doc(char * subdoc_name, uint32_t version, uint16_t trans_id, char * blob_data, size_t blob_size);

/**
 * @brief Appends encoded managed WiFi data and metadata to buffer.
 *
 * This function combines encoded managed WiFi configuration data with metadata, allocating output buffer.
 *
 * @param[out] appendData Pointer to output buffer pointer.
 * @param[in] encodedBuffer Encoded WiFi data buffer.
 * @param[in] encodedSize Size of encoded data in bytes.
 * @param[in] metadataPack Packed metadata buffer.
 * @param[in] metadataSize Size of metadata in bytes.
 *
 * @return Appended total buffer size.
 * @retval Total buffer size in bytes if successful.
 * @retval 0 or less than 1 on failure.
 */
size_t mg_appendWifiEncodedData( void **appendData, void *encodedBuffer, size_t encodedSize, void *metadataPack, size_t metadataSize );
/**
* @brief Encode managed WiFi blob data to base64 format.
*
* This function encodes managed WiFi configuration blob data to base64 string format for transmission.
*
* @param[in] blob_data - Pointer to the WiFi configuration blob data to be encoded.
* @param[in] blob_size - Size of the blob data in bytes.
*
* @return Pointer to base64 encoded string.
* @retval Pointer to a null-terminated base64 encoded string on success.
* @retval NULL on memory allocation failure.
*/
char * base64wifiblobencoder(char * blob_data, size_t blob_size );
#endif