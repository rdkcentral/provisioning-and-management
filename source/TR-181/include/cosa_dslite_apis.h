/**********************************************************************
 * Copyright 2017-2019 ARRIS Enterprises, LLC.
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
**********************************************************************/

#ifndef  _COSA_DSLITE_APIS_H
#define  _COSA_DSLITE_APIS_H

#include "../middle_layer_src/cosa_apis.h"

struct _COSA_DML_DSLITE
{
    unsigned long  InstanceNumber;
    int            active;
    int            status;
    char           alias[64+1];
    int            mode;
    int            addr_type;
    char           addr_inuse[256+1];
    char           addr_fqdn[256+1];
    char           addr_ipv6[256+1];
    int            origin;
    char           tunnel_interface[256+1];
    char           tunneled_interface[256+1];
    int            mss_clamping_enable;
    unsigned long  tcpmss;
    int            ipv6_frag_enable;
    char           tunnel_v4addr[64+1];
}_struct_pack_;

typedef struct _COSA_DML_DSLITE COSA_DML_DSLITE,  *PCOSA_DML_DSLITE;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the DS-Lite subsystem.
*
* This function initializes the DS-Lite Data Model Library subsystem and prepares it for operation.
*
* @param[in] hDml - Handle to the DML object.
* @param[out] phContext - Pointer to a handle where the context will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDsliteInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Get the DS-Lite enabled status.
*
* This function retrieves whether the DS-Lite functionality is enabled.
*
* @param[in] hContext - Handle to the context.
* @param[out] bEnabled - Pointer to a BOOLEAN where the enabled status will be returned.
*                        \n TRUE if DS-Lite is enabled, FALSE otherwise.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlGetDsliteEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *bEnabled
    );

/**
* @brief Set the DS-Lite enabled status.
*
* This function enables or disables the DS-Lite functionality.
*
* @param[in] hContext - Handle to the context.
* @param[in] bEnabled - Boolean flag indicating whether to enable (TRUE) or disable (FALSE) DS-Lite.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlSetDsliteEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

/**
* @brief Add a new DS-Lite entry.
*
* This function adds a new DS-Lite tunnel configuration entry.
*
* @param[in] hContext - Handle to the context.
* @param[in] pEntry - Pointer to a COSA_DML_DSLITE structure containing the DS-Lite configuration
*                     to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the addition fails.
*
*/
ANSC_STATUS
CosaDmlDsliteAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DSLITE            pEntry
    );

/**
* @brief Delete a DS-Lite entry.
*
* This function removes a DS-Lite tunnel configuration entry.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulInstanceNumber - Instance number of the DS-Lite entry to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or deletion fails.
*
*/
ANSC_STATUS
CosaDmlDsliteDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set the configuration of a DS-Lite entry.
*
* This function updates the configuration of a DS-Lite tunnel entry.
*
* @param[in] hContext - Handle to the context.
* @param[in] pEntry - Pointer to a COSA_DML_DSLITE structure containing the new configuration.
*                     \n The InstanceNumber field identifies which entry to update.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDsliteSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DSLITE            pEntry
    );

/**
* @brief Get the configuration of a DS-Lite entry.
*
* This function retrieves the configuration of a DS-Lite tunnel entry.
*
* @param[in] hContext - Handle to the context.
* @param[in,out] pEntry - Pointer to a COSA_DML_DSLITE structure.
*                         \n [in] The InstanceNumber field identifies which entry to query.
*                         \n [out] The structure will be filled with the DS-Lite configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDsliteGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DSLITE            pEntry
    );

/**
* @brief Get the number of DS-Lite entries.
*
* This function retrieves the count of DS-Lite tunnel entries.
*
* @param[in] hContext - Handle to the context.
* @param[out] pNum - Pointer to a ULONG where the number of DS-Lite entries will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the operation fails.
*
*/
ANSC_STATUS
CosaDmlDsliteGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pNum
    );

/**
* @brief Get a DS-Lite entry by index.
*
* This function retrieves a DS-Lite entry at the specified index.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the DS-Lite entry to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_DSLITE structure where the configuration will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found.
*
*/
ANSC_STATUS
CosaDmlDsliteGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DSLITE            pEntry
    );

/**
* @brief Set the instance number for a DS-Lite entry.
*
* This function updates the instance number for a DS-Lite entry at the specified index.
*
* @param[in] hContext - Handle to the context.
* @param[in] ulIndex - Zero-based index of the DS-Lite entry to update.
* @param[in] ulInstanceNumber - New instance number to assign to the entry.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the entry is not found or update fails.
*
*/
ANSC_STATUS
CosaDmlDsliteSetInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set a sysevent key-value pair.
*
* This function sets a key-value pair in the sysevent system using the common sysevent connection.
*
* @param[in] key - Pointer to a null-terminated string containing the sysevent key name.
* @param[in] value - Pointer to a null-terminated string containing the value to set for the key.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval Non-zero error code if the operation fails.
*
*/
int commonSyseventSet(char* key, char* value);

/**
* @brief Get a sysevent value by key.
*
* This function retrieves the value associated with a key from the sysevent system
* using the common sysevent connection.
*
* @param[in] key - Pointer to a null-terminated string containing the sysevent key name.
* @param[out] value - Pointer to a buffer where the retrieved value will be returned.
* @param[in] valLen - Maximum length of the value buffer.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval Non-zero error code if the operation fails.
*
*/
int commonSyseventGet(char* key, char* value, int valLen);

/**
* @brief Close the common sysevent connection.
*
* This function closes the common sysevent connection and releases associated resources.
*
* @return The status of the operation.
* @retval 0 if the operation is successful or if the connection was not open.
* @retval Non-zero error code if the operation fails.
*
*/
int commonSyseventClose();

#endif
