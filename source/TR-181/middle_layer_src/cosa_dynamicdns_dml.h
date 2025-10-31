/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2021 RDK Management
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

#if defined(DDNS_BROADBANDFORUM)


#ifndef  _COSA_DYNAMICDNS_DML_H
#define  _COSA_DYNAMICDNS_DML_H

#include  "cosa_dynamicdns_apis.h"

/***********************************************************************

 APIs for Object:

    DynamicDNS.

    *  DynamicDNS_GetParamBoolValue
    *  DynamicDNS_GetParamStringValue
    *  DynamicDNS_SetParamBoolValue

***********************************************************************/

/**
 * @brief Retrieves a boolean parameter value from the Device.DynamicDNS object.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS object context.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value stored in pBool.
 * @retval FALSE Parameter not found or error occurred.
 */
BOOL
DynamicDNS_GetParamBoolValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    BOOL*                       pBool
);

/**
 * @brief Retrieves a string parameter value from the Device.DynamicDNS object.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS object context.
 * @param[in] ParamName Name of the string parameter.
 * @param[out] pValue Buffer to store the retrieved string value, size determined by puLong.
 * @param[in,out] puLong On input: size of pValue buffer; on output: actual string length. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved and stored in pValue.
 * @retval 1 Buffer insufficient, puLong contains required size.
 * @retval -1 Not supported.
 */
ULONG
DynamicDNS_GetParamStringValue
(
    ANSC_HANDLE                     hInsContext,
    char*                           ParamName,
    char*                           pValue,
    ULONG*                          puLong
);

/**
 * @brief Sets a boolean parameter value in the Device.DynamicDNS object.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS object context.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
DynamicDNS_SetParamBoolValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    BOOL                        bValue
);
/***********************************************************************

 APIs for Object:

    Device.DynamicDNS.Client.{i}


    *  DDNSClient_GetEntryCount
    *  DDNSClient_GetEntry
    *  DDNSClient_AddEntry
    *  DDNSClient_DelEntry
    *  DDNSClient_GetParamBoolValue
    *  DDNSClient_GetParamUlongValue
    *  DDNSClient_GetParamStringValue
    *  DDNSClient_SetParamBoolValue
    *  DDNSClient_SetParamStringValue
    *  DDNSClient_Validate
    *  DDNSClient_Commit
    *  DDNSClient_Rollback

***********************************************************************/

/**
 * @brief Gets the number of Device.DynamicDNS.Client.{i} table entries.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS context.
 *
 * @return Number of Client table entries.
 * @retval Count of configured DDNS clients.
 */
ULONG
DDNSClient_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a specific Device.DynamicDNS.Client.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS context.
 * @param[in] nIndex Zero-based index of the Client entry to retrieve.
 * @param[out] pInsNumber Pointer to store the instance number of the retrieved entry.
 *
 * @return Instance handle of the Client entry
 * @retval Non-NULL Handle to the Client entry context.
 * @retval NULL Entry not found at specified index.
 */
ANSC_HANDLE
DDNSClient_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new Device.DynamicDNS.Client.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS context.
 * @param[out] pInsNumber Pointer to store the allocated instance number for the new entry.
 *
 * @return Instance handle of the newly created Client entry
 * @retval Non-NULL Handle to the new Client entry.
 * @retval NULL Failed to create entry.
 */
ANSC_HANDLE
DDNSClient_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a Device.DynamicDNS.Client.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS context.
 * @param[in] hInstance Instance handle of the Client entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval ANSC_STATUS_FAILURE Failed to delete entry.
 */
ULONG
DDNSClient_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter from a Device.DynamicDNS.Client.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value stored in pBool.
 * @retval FALSE Parameter not found.
 */
BOOL
DDNSClient_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an unsigned long parameter from a Device.DynamicDNS.Client.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry.
 * @param[in] ParamName Name of the ulong parameter.
 * @param[out] puLong Pointer to store the retrieved unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value stored in puLong.
 * @retval FALSE Parameter not found.
 */
BOOL
DDNSClient_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief Retrieves a string parameter from a Device.DynamicDNS.Client.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry.
 * @param[in] ParamName Name of the string parameter.
 * @param[out] pValue Buffer to store the retrieved string, size determined by pUlSize.
 * @param[in,out] pUlSize On input: buffer size; on output: actual string length.Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small, pUlSize contains required size.
 * @retval -1 Not supported.
 */
ULONG
DDNSClient_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter in a Device.DynamicDNS.Client.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
DDNSClient_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets a string parameter in a Device.DynamicDNS.Client.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry.
 * @param[in] ParamName Name of the string parameter.
 * @param[in] strValue Null-terminated string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
DDNSClient_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates staged changes to a Device.DynamicDNS.Client.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry.
 * @param[out] pReturnParamName Buffer(128 bytes) to store the name of the parameter if there's a validation, size determined by puLength.
 * @param[in,out] puLength On input: buffer size; on output: actual parameter name length.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
DDNSClient_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits staged changes to a Device.DynamicDNS.Client.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
DDNSClient_Commit
    (
        ANSC_HANDLE                 hInsContext
    );
/**
 * @brief Rolls back uncommitted changes to a Device.DynamicDNS.Client.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
DDNSClient_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Device.DynamicDNS.Client.{i}.Hostname.{i}

    *  DDNSHostname_GetEntryCount
    *  DDNSHostname_GetEntry
    *  DDNSHostname_AddEntry
    *  DDNSHostname_DelEntry
    *  DDNSHostname_GetParamBoolValue
    *  DDNSHostname_GetParamUlongValue
    *  DDNSHostname_GetParamStringValue
    *  DDNSHostname_SetParamBoolValue
    *  DDNSHostname_SetParamStringValue
    *  DDNSHostname_Validate
    *  DDNSHostname_Commit
    *  DDNSHostname_Rollback

***********************************************************************/

/**
 * @brief Gets the number of Device.DynamicDNS.Client.{i}.Hostname.{i} table entries.
 *
 * @param[in] hInsContext Instance handle for the Client entry context.
 *
 * @return Number of Hostname table entries for this client.
 * @retval >=0 Count of configured hostnames.
 */
ULONG
DDNSHostname_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a specific Device.DynamicDNS.Client.{i}.Hostname.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry context.
 * @param[in] nIndex Zero-based index of the Hostname entry to retrieve.
 * @param[out] pInsNumber Pointer to store the instance number of the retrieved entry.
 *
 * @return Instance handle of the Hostname entry, or NULL if not found.
 * @retval Non-NULL Handle to the Hostname entry context.
 * @retval NULL Entry not found at specified index.
 */
ANSC_HANDLE
DDNSHostname_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new Device.DynamicDNS.Client.{i}.Hostname.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry context.
 * @param[out] pInsNumber Pointer to store the allocated instance number for the new entry.
 *
 * @return Instance handle of the newly created Hostname entry.
 * @retval Non-NULL Handle to the new Hostname entry.
 * @retval NULL Failed to create entry.
 */
ANSC_HANDLE
DDNSHostname_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a Device.DynamicDNS.Client.{i}.Hostname.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the Client entry context.
 * @param[in] hInstance Instance handle of the Hostname entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval ANSC_STATUS_FAILURE Failed to delete entry.
 */
ULONG
DDNSHostname_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter from a Device.DynamicDNS.Client.{i}.Hostname.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Hostname entry.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found.
 */
BOOL
DDNSHostname_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an unsigned long parameter from a Device.DynamicDNS.Client.{i}.Hostname.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Hostname entry.
 * @param[in] ParamName Name of the ulong parameter.
 * @param[out] puLong Pointer to store the retrieved unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found.
 */
BOOL
DDNSHostname_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief Retrieves a string parameter from a Device.DynamicDNS.Client.{i}.Hostname.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Hostname entry.
 * @param[in] ParamName Name of the string parameter.
 * @param[out] pValue Buffer to store the retrieved string, size determined by pUlSize.
 * @param[in,out] pUlSize On input: buffer size; on output: actual string length. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small, pUlSize contains required size.
 * @retval -1 Not supported.
 */
ULONG
DDNSHostname_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter in a Device.DynamicDNS.Client.{i}.Hostname.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Hostname entry.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
DDNSHostname_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets a string parameter in a Device.DynamicDNS.Client.{i}.Hostname.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Hostname entry.
 * @param[in] ParamName Name of the string parameter.
 * @param[in] strValue Null-terminated string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
DDNSHostname_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates staged changes to a Device.DynamicDNS.Client.{i}.Hostname.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Hostname entry.
 * @param[out] pReturnParamName Buffer(128 bytes) to store the name of the parameter if there's a validation, size determined by puLength.
 * @param[in,out] puLength On input: buffer size; on output: actual parameter name length.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
DDNSHostname_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits staged changes to a Device.DynamicDNS.Client.{i}.Hostname.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Hostname entry.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
DDNSHostname_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted changes to a Device.DynamicDNS.Client.{i}.Hostname.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Hostname entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
DDNSHostname_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Device.DynamicDNS.Server.{i}

    *  DDNSServer_GetEntryCount
    *  DDNSServer_GetEntry
    *  DDNSServer_AddEntry
    *  DDNSServer_DelEntry
    *  DDNSServer_GetParamBoolValue
    *  DDNSServer_GetParamUlongValue
    *  DDNSServer_GetParamStringValue
    *  DDNSServer_SetParamBoolValue
    *  DDNSServer_SetParamStringValue
    *  DDNSServer_Validate
    *  DDNSServer_Commit
    *  DDNSServer_Rollback

***********************************************************************/
/**
 * @brief Gets the number of Device.DynamicDNS.Server.{i} table entries.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS context.
 *
 * @return Number of Server table entries.
 * @retval >=0 Count of configured DDNS servers.
 */
ULONG
DDNSServer_GetEntryCount
    (
         ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a specific Device.DynamicDNS.Server.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS context.
 * @param[in] nIndex Zero-based index of the Server entry to retrieve.
 * @param[out] pInsNumber Pointer to store the instance number of the retrieved entry.
 *
 * @return Instance handle of the Server entry.
 * @retval Non-NULL Handle to the Server entry context.
 * @retval NULL Entry not found at specified index.
 */
ANSC_HANDLE
DDNSServer_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new Device.DynamicDNS.Server.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS context.
 * @param[out] pInsNumber Pointer to store the allocated instance number for the new entry.
 *
 * @return Instance handle of the newly created Server entry.
 * @retval Non-NULL Handle to the new Server entry.
 * @retval NULL Failed to create entry.
 */
ANSC_HANDLE
DDNSServer_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a Device.DynamicDNS.Server.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the DynamicDNS context.
 * @param[in] hInstance Instance handle of the Server entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval ANSC_STATUS_FAILURE Failed to delete entry.
 */
ULONG
DDNSServer_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter from a Device.DynamicDNS.Server.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Server entry.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value stored in pBool.
 * @retval FALSE Parameter not found.
 */
BOOL
DDNSServer_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves a string parameter from a Device.DynamicDNS.Server.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Server entry.
 * @param[in] ParamName Name of the string parameter.
 * @param[out] pValue Buffer to store the retrieved string, size determined by pUlSize.
 * @param[in,out] pUlSize On input: buffer size; on output: actual string length. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small, pUlSize contains required size.
 * @retval -1 Not supported.
 */
ULONG
DDNSServer_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter in a Device.DynamicDNS.Server.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Server entry.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[in] pBool Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
DDNSServer_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        pBool
    );

/**
 * @brief Sets a string parameter in a Device.DynamicDNS.Server.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Server entry.
 * @param[in] ParamName Name of the string parameter.
 * @param[in] strValue Null-terminated string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
DDNSServer_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates staged changes to a Device.DynamicDNS.Server.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Server entry.
 * @param[out] pReturnParamName Buffer(128 bytes) to store the name of the parameter if there's a validation, size determined by puLength.
 * @param[in,out] puLength On input: buffer size; on output: actual parameter name length.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
DDNSServer_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits staged changes to a Device.DynamicDNS.Server.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Server entry.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
DDNSServer_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted changes to a Device.DynamicDNS.Server.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Server entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
DDNSServer_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif  //_COSA_DYNAMICDNS_DML_H


#endif
