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

#ifndef  _COSA_NAT_WEBCONFIG_API_H
#define  _COSA_NAT_WEBCONFIG_API_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "ansc_status.h"
#include "dmz_wan_doc.h"
#include "webconfig_framework.h"



/**
 * @brief Initialize the DMZ cache structure during system boot.
 *
 * This function initializes the DMZ cache array by setting up command-value pairs for DMZ configuration parameters.
 *
 * @param[in,out] tmp_dmz_cache - Pointer to the DMZ cache array to be initialized.
 *
 * @return None.
 */
void init_dmz_cache(t_cache *tmp_dmz_cache);

/**
 * @brief Clear all entries in the DMZ cache buffer.
 *
 * This function clears all DMZ cache entries by resetting both command and value
 * fields to zero for all cache elements.
 *
 * @param[in,out] tmp_dmz_cache - Pointer to the DMZ cache array to be cleared.
 *
 * @return None.
 */
void clear_dmz_cache(t_cache *tmp_dmz_cache);

/**
 * @brief Print the contents of the DMZ cache for debugging purposes.
 *
 * This function prints all DMZ cache entries to the system trace log using CcspTraceWarning.
 *
 * @param[in] tmp_dmz_cache - Pointer to the DMZ cache array whose contents will be printed.
 *
 * @return None.
 */
void print_dmz_cache(t_cache *tmp_dmz_cache);

/**
 * @brief Clear DMZ entries from the system configuration database.
 *
 * This function removes all DMZ configuration entries from the syscfg database
 * by unsetting each cache entry's command parameter and committing the changes.
 *
 * @param[in,out] tmp_dmz_cache - Pointer to the DMZ cache array whose entries will be
 *                                 removed from the database.
 *
 * @return The status of the operation.
 * @retval 0 if all DMZ entries were successfully removed from the database.
 * @retval SYSCFG_FAILURE if syscfg_unset or syscfg_commit operations fail.
 */
int clear_dmz_cache_DB(t_cache *tmp_dmz_cache);

/**
 * @brief Apply DMZ configuration from cache to the DML (Data Model Layer).
 *
 * This function transfers DMZ configuration from the cache structure to the COSA
 * Data Model Layer structures.
 *
 * @param[in] tmp_dmz_cache - Pointer to the DMZ cache array containing configuration
 *                             to be applied to the DML layer.
 *
 * @return None.
 */
void apply_dmz_ToDML(t_cache *tmp_dmz_cache);

/**
 * @brief Create a backup copy of the DMZ cache for rollback purposes.
 *
 * This function creates a backup of the current DMZ cache configuration by copying
 * all command and value fields from the source cache to the backup cache.
 *
 * @param[in] tmp_dmz_cache - Pointer to the source DMZ cache array to be backed up.
 * @param[out] tmp_dmz_cache_bkup - Pointer to the backup DMZ cache array where data will be copied.
 * @param[in] cache_size - Number of cache entries to back up.
 *
 * @return None.
 */
void backup_dmz_cache(t_cache *tmp_dmz_cache,t_cache *tmp_dmz_cache_bkup,int cache_size);

/**
 * @brief Apply DMZ cache configuration to the system configuration database.
 *
 * This function persists the DMZ configuration from the cache to the syscfg database.
 *
 * @param[in] tmp_dmz_cache - Pointer to the DMZ cache array containing configuration
 *                             to be persisted to the database.
 * @param[in] cache_size - Number of cache entries to apply.
 *
 * @return The status of the operation.
 * @retval 0 if all DMZ cache entries were successfully applied to the database.
 * @retval SYSCFG_FAILURE if syscfg_set or syscfg_commit operations fail.
 */
int apply_dmz_cache_ToDB(t_cache *tmp_dmz_cache, int cache_size);

/**
 * @brief Process DMZ webconfig blob requests from the webconfig framework.
 *
 * This callback function is invoked by the webconfig framework to execute DMZ configuration blob requests.
 *
 * @param[in] Data - Pointer to dmz_wandoc_t structure containing the DMZ configuration
 *                   parameters from the webconfig blob.
 *
 * @return Pointer to error structure (pErr) containing operation result details.
 * @retval pErr->ErrorCode = BLOB_EXEC_SUCCESS if the operation is successful.
 * @retval pErr->ErrorCode = SYSCFG_FAILURE if system configuration operation fails.
 * @retval pErr->ErrorCode = INVALID_DMZ_WAN_IP if the internal IP address validation fails.
 */
pErr Process_DMZ_WebConfigRequest(void *Data);

/**
 * @brief Read DMZ blob entries and populate the DMZ cache structure.
 *
 * This function extracts DMZ configuration parameters from the dmz_wandoc_t structure.
 *
 * @param[in] rpm - Pointer to dmz_wandoc_t structure containing DMZ parameters.
 *                  \n rpm->param[0].enable: DMZ enable state (TRUE/FALSE).
 *                  \n rpm->param[0].internal_ip: Internal IP address for DMZ host (when enabled).
 *
 * @return The status of the operation.
 * @retval 0 if DMZ configuration was successfully populated into the cache.
 * @retval INVALID_DMZ_WAN_IP if the internal IP is invalid, missing, loopback, multicast,
 *         broadcast, or outside the local LAN network.
 */
int set_dmz_conf(dmz_wandoc_t *rpm);

/**
 * @brief Rollback DMZ configuration to the previous backed-up state.
 *
 * This callback function is invoked by the webconfig framework when DMZ blob execution fails.
 *
 * @return The status of the rollback operation.
 * @retval 0 if the rollback was successful and backup configuration was restored.
 * @retval SYSCFG_FAILURE if applying the backup cache to the database failed.
 */
int rollback_dmz() ;

/**
 * @brief Free resources allocated for DMZ webconfig blob processing.
 *
 * This callback function is invoked by the webconfig framework to free all resources
 * allocated during DMZ blob execution.
 *
 * @param[in] arg - Pointer to execData structure containing blob execution context.
 *
 * @return None.
 */
void freeResources_dmz(void *arg);
#endif

