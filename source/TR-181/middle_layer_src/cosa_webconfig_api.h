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

#ifndef  _COSA_WEBCONFIG_API_H
#define  _COSA_WEBCONFIG_API_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "ansc_status.h"
#include "portmappingdoc.h"
#include "dmz_wan_doc.h"
#include "webui_doc.h"

#include "cosa_nat_apis.h"
#include "msgpack.h"
#include "webconfig_framework.h"
#ifndef NO_WIFI_FEATURE_SUPPORT
#include "cosa_GRE_webconfig_apis.h"
#endif
#if defined (WIFI_MANAGE_SUPPORTED)
#include "cosa_managedwifi_webconfig_apis.h"
#endif /*WIFI_MANAGE_SUPPORTED*/
#if defined (FEATURE_RDKB_DHCP_MANAGER)
#define SUBDOC_COUNT 6
#else
#define SUBDOC_COUNT 8
#endif

#define PORTMAP_CACHE_SIZE 1024
#define DMZ_CACHE_SIZE 2

#define BLOCK_SIZE 32
#define VAL_BLOCK_SIZE 129 // for ipv6 address 128 + 1 size is needed

#define SinglePortForwardCount
#define ALIAS_PRE_SPF "spf_"
#define ALIAS_PRE_PFR "pfr_"
#define ALIAS_POS_PROTO "::protocol"
#define ALIAS_POS_NAME "::name"
#define ALIAS_POS_EXT_PORT "::external_port"
#define ALIAS_POS_IP "::to_ip"
#define ALIAS_POS_ENABLE "::enabled"
#define ALIAS_SPF "SinglePortForward_"

#define ALIAS_PFR "PortRangeForward_"
#define ALIAS_PFR_INT_RANGE "::internal_port_range_size"
#define ALIAS_PFR_PUBLIC_IP "::public_ip"
#define ALIAS_POS_EXT_PORT_RANGE "::external_port_range"

#define ALIAS_POS_IPV6 "::to_ipv6"
#define ALIAS_POS_INT_PORT "::internal_port"
#define ALIAS_POS_PREV_STATE "::prev_rule_enabled_state"

#define HOTSPOT_BLOB_FILE "/nvram/hotspot_blob"

#ifdef WEBCFG_TEST_SIM

#define NACK_SIMULATE_FILE "/tmp/sim_nack"
#define TIMEOUT_SIMULATE_FILE "/tmp/sim_timeout"

#endif

typedef struct {
    char cmd[BLOCK_SIZE];
    char val[VAL_BLOCK_SIZE];
} t_cache;

extern t_cache pf_cache[PORTMAP_CACHE_SIZE];
extern t_cache pf_cache_bkup[PORTMAP_CACHE_SIZE];

extern t_cache dmz_cache[DMZ_CACHE_SIZE];
extern t_cache dmz_cache_bkup[DMZ_CACHE_SIZE];

/**
 * @brief Retrieves the version number of a webconfig subdocument blob.
 *
 * This function queries the current version of the specified webconfig subdocument.
 *
 * @param[in] subdoc Name of the subdocument to query.
 *
 * @return Version number of the subdocument blob
 * @retval current version number of the subdocument blob.
 * @retval 0 if not found.
 */
uint32_t getBlobVersion(char* subdoc);

/**
 * @brief Sets the version number of a webconfig subdocument blob.
 *
 * This function updates the version number for the specified webconfig subdocument.
 *
 * @param[in] subdoc Name of the subdocument to update.
 * @param[in] version New version number to set.
 *
 * @return status of operation
 * @retval 0 on success.
 * @retval non-zero error code on failure.
 */

int setBlobVersion(char* subdoc,uint32_t version);
/**
 * @brief Initializes the webconfig framework for processing configuration blobs.
 *
 * This function sets up the webconfig framework, registering subdocument handlers.
 *
 * @return void
 */
void webConfigFrameworkInit() ;

/**
 * @brief Clears the port forwarding cache.
 *
 * This function resets all entries in the port forwarding cache to their default state.
 *
 * @param[in,out] tmp_pf_cache Pointer to port forwarding cache to clear.
 *
 * @return void
 */
void clear_pf_cache(t_cache *tmp_pf_cache);
/**
 * @brief Prints the contents of the port forwarding cache for debugging.
 *
 * This function outputs the port forwarding cache entries to the log for troubleshooting.
 *
 * @param[in] tmp_pf_cache Pointer to port forwarding cache to print.
 *
 * @return void
 */
void print_pf_cache(t_cache *tmp_pf_cache);
/**
 * @brief Clears port forwarding entries from the persistent database.
 *
 * This function removes port forwarding cache entries from the PSM database.
 *
 * @param[in] tmp_pf_cache Pointer to port forwarding cache specifying entries to clear.
 *
 * @return status of operation
 * @retval 0 on success.
 * @retval non-zero error code on failure.
 */
int clear_pf_cache_DB(t_cache *tmp_pf_cache);
/**
 * @brief Applies port forwarding cache entries to the persistent database.
 *
 * This function writes port forwarding configuration from cache to PSM database and activates rules.
 *
 * @param[in] tmp_pf_cache Pointer to port forwarding cache to apply.
 * @param[in] cache_size Number of entries in the cache.
 * @param[in] pmapEnable Flag indicating whether port mapping is enabled.
 *
 * @return status of operation
 * @retval 0 on success.
 * @retval non-zero error code on failure.
 */
int apply_pf_cache_ToDB(t_cache *tmp_pf_cache, int cache_size,int pmapEnable);
/**
 * @brief Sets port mapping configuration from webconfig document.
 *
 * This function processes a port mapping webconfig document and applies the configuration.
 *
 * @param[in] rpm Pointer to portmappingdoc_t structure containing port mapping configuration.
 *
 * @return status of operation
 * @retval 0 on success.
 * @retval non-zero error code on failure.
 */
int set_portmap_conf(portmappingdoc_t *rpm);
/**
 * @brief Creates a backup copy of the port forwarding cache.
 *
 * This function copies port forwarding cache entries to backup cache for rollback support.
 *
 * @param[in] tmp_pf_cache Source port forwarding cache to backup.
 * @param[out] tmp_pf_cache_bkup Destination backup cache.
 * @param[in] cache_size Number of entries to backup.
 *
 * @return void
 */
void backup_pf_cache(t_cache *tmp_pf_cache,t_cache *tmp_pf_cache_bkup,int cache_size);

/**
 * @brief Initializes the port forwarding cache from persistent storage.
 *
 * This function loads port forwarding configuration from PSM database into cache.
 *
 * @param[out] tmp_pf_cache Pointer to port forwarding cache to initialize.
 *
 * @return void
 */
void init_pf_cache(t_cache *tmp_pf_cache);

/**
 * @brief Processes a port forwarding webconfig request.
 *
 * This function handles incoming port forwarding configuration from webconfig framework,
 * validates the data, and applies the configuration.
 *
 * @param[in] Data Pointer to webconfig request data.
 *
 * @return status of operation
 * @retval NULL on success.
 * @retval error details on failure.
 */
pErr Process_PF_WebConfigRequest(void *Data);
/**
 * @brief Rolls back port forwarding configuration to previous state.
 *
 * This function restores port forwarding configuration from backup cache when
 * a configuration update fails.
 *
 * @return status of operation
 * @retval 0 on success.
 * @retval non-zero error code on failure.
 */
int rollback_PortForwarding() ;
/**
 * @brief Frees resources allocated for port forwarding webconfig processing.
 *
 * This function releases memory and resources associated with port forwarding webconfig request.
 *
 * @param[in] arg Pointer to resource data to free.
 *
 * @return void
 */
void freeResources_PortForwarding(void *arg);
/**
 * @brief Decodes a base64-encoded string into a binary buffer.
 *
 * This function decodes base64 encoded webconfig data and allocates a buffer for the result.
 *
 * @param[in] pString Base64-encoded input string.
 * @param[out] buffer Pointer to receive allocated decoded buffer (caller must free).
 * @param[out] size Pointer to receive size of decoded buffer in bytes.
 *
 * @return status of operation
 * @return 0 on success.
 * @retval non-zero error code on failure.
 */
int  get_base64_decodedbuffer(char *pString, char **buffer, int *size);
/**
 * @brief Gets the status of msgpack unpacking operation.
 *
 * This function attempts to unpack msgpack data and returns the unpacking status.
 *
 * @param[in] decodedbuf Buffer containing msgpack data to unpack.
 * @param[in] size Size of the buffer in bytes.
 *
 * @return msgpack_unpack_return status of unpacking operation
 * @retval MSGPACK_UNPACK_SUCCESS on msgpack_unpack_return success.
 * @retval other msgpack_unpack_return codes on failures (MSGPACK_UNPACK_EXTRA_BYTES,MSGPACK_UNPACK_CONTINUE, MSGPACK_UNPACK_PARSE_ERROR).
 */
msgpack_unpack_return get_msgpack_unpack_status(char *decodedbuf, int size);
/**
 * @brief Gets the current system time with high precision.
 *
 * This function retrieves the current time for performance measurement and timestamping.
 *
 * @param[out] timer Pointer to timespec structure to store current time.
 *
 * @return void
 */
void getCurrentTime(struct timespec *timer);
/**
 * @brief Calculates the time difference between two timespec values.
 *
 * This function computes the elapsed time in milliseconds between start and finish times.
 *
 * @param[in] starttime Pointer to start time.
 * @param[in] finishtime Pointer to finish time.
 *
 * @return Time difference in milliseconds.
 */
long timeValDiff(struct timespec *starttime, struct timespec *finishtime);

#endif
