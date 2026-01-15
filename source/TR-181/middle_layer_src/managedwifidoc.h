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
#ifndef __MANAGEDWIFIDOC_H__
#define __MANAGEDWIFIDOC_H__

#include <stdint.h>
#include <stdlib.h>
#include <msgpack.h>


typedef struct
{
    bool mwenable;
    char *   alias;
    bool     dhcp_server_enable;
    char *   lan_ip_address;
    char *   lan_subnet_mask;
    char *   dhcp_start_ip_address;
    char *   dhcp_end_ip_address;
    char*    leasetime;
    bool     ipv6enable;
    char *   firewall;
} lanconfig_t;

#if defined (AMENITIES_NETWORK_ENABLED)
typedef struct
{
    bool *pIsAmenityEnabled;
    char ** ppVapNames;
    int * pVlanIds;
    int iVapCount;
} lanconfigTunnelInfo_t;
#endif

typedef struct
{
    lanconfig_t * entries;
    size_t    entries_count;
} lanconfigdoc_t;

typedef struct
{
    char *name;
    char *value;
    uint32_t   value_size;
    uint16_t type;
} mwparam_t;

typedef struct {
    mwparam_t   *entries;
    size_t      entries_count;
    char *      subdoc_name;
    uint32_t    version;
    uint16_t    transaction_id;
} managedwifidoc_t;

/**
 * @brief Converts a msgpack buffer into a managedwifidoc_t structure.
 *
 *  @param[in] buf the buffer to convert
 *  @param[in] len the length of the buffer in bytes
 *
 *  @return Pointer to the converted managedwifidoc_t structure.
 *  @retval Valid pointer to managedwifidoc_t on success
 *  @retval NULL on error.
 *
 */
managedwifidoc_t* managedwifidoc_convert( const void *buf, size_t len );
/**
 *  @brief Returns a general reason why the conversion failed.
 *
 *  @param[in] errnum the errno value to inspect
 *
 *  @return Constant string describing the error
 *  @retval Error description string (do not alter or free).
 *  @retval Unknown error otherwise
 */
const char* managedwifidoc_strerror( int errnum );

#if defined (AMENITIES_NETWORK_ENABLED)
/**
* @brief Converts a msgpack buffer into a lanconfigTunnelInfo_t structure.
*
*  @param[in] buf the buffer to convert
*  @param[in] len the length of the buffer in bytes
*
* @return Pointer to the converted lanconfigTunnelInfo_t structure.
* @retval Valid pointer to lanconfigTunnelInfo_t on success.
* @retval NULL on error.
*/
lanconfigTunnelInfo_t* lanconfigdoc_convert( const void *buf, size_t len);
/**
 * @brief Destroys a lanconfigTunnelInfo_t object and frees allocated memory.
 *
 * @param[in] lwd Pointer to the lanconfigTunnelInfo_t structure to destroy.
 *
 * @return None.
 */
void tunnelLanConfigDocdestroy( lanconfigTunnelInfo_t *lwd );
#else
/**
 * @brief Destroys a lanconfigdoc_t object and frees allocated memory.
 *
 * @param[in] lwd Pointer to the lanconfigdoc_t structure to destroy.
 *
 * @return None.
 */
void lanConfigDoc_destroy( lanconfigdoc_t *lwd );

/**
 * @brief Converts a msgpack buffer into a lanconfigdoc_t structure.
 *
 * @param[in] buf the buffer to convert
 * @param[in] len the length of the buffer in bytes
 *
 * @return Pointer to the converted lanconfigdoc_t structure.
 * @retval Valid pointer to lanconfigdoc_t on success.
 * @retval NULL on error.
 */
lanconfigdoc_t* lanconfigdoc_convert( const void *buf, size_t len );
#endif
/**
 * @brief Destroys a managedwifidoc_t object and frees allocated memory.
 *
 * @param[in] mwd Pointer to the managedwifidoc_t structure to destroy.
 *
 * @return None.
 */
void connectedbuilding_destroy(managedwifidoc_t *mwd);

#endif
