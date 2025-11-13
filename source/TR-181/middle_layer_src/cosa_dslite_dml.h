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

#ifndef  _COSA_APIS_DSLITE_H
#define  _COSA_APIS_DSLITE_H

#include "cosa_apis.h"

/***********************************************************************

 APIs for Object:

    DSLite.

    *  DSLite_GetParamBoolValue
    *  DSLite_SetParamBoolValue

    *  InterfaceSetting4_GetEntryCount
    *  InterfaceSetting4_GetEntry
    *  InterfaceSetting4_AddEntry
    *  InterfaceSetting4_DelEntry

    *  InterfaceSetting4_GetParamBoolValue
    *  InterfaceSetting4_GetParamStringValue
    *  InterfaceSetting4_GetParamUlongValue

    *  InterfaceSetting4_SetParamBoolValue
    *  InterfaceSetting4_SetParamStringValue
    *  InterfaceSetting4_SetParamUlongValue

    *  InterfaceSetting4_Validate
    *  InterfaceSetting4_Commit
    *  InterfaceSetting4_Rollback

***********************************************************************/
/**
 * @brief Retrieves a boolean parameter value from Device.DSLite.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
DSLite_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets a boolean parameter value for Device.DSLite.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
DSLite_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves the number of InterfaceSetting entries in the Device.DSLite.InterfaceSetting table.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of InterfaceSetting entries in the table.
 */
ULONG
InterfaceSetting4_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves an InterfaceSetting entry by index from the Device.DSLite.InterfaceSetting table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Pointer to receive the instance number of the entry.
 *
 * @return Handle to the InterfaceSetting entry
 * @retval non-NULL Handle to the entry context.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
InterfaceSetting4_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new InterfaceSetting entry to the Device.DSLite.InterfaceSetting table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Pointer to receive the assigned instance number.
 *
 * @return Handle to the newly created InterfaceSetting entry
 * @retval non-NULL Handle to the new entry context.
 * @retval NULL Addition failed.
 */
ANSC_HANDLE
InterfaceSetting4_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes an InterfaceSetting entry from the Device.DSLite.InterfaceSetting table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the InterfaceSetting entry to delete.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 */
ULONG
InterfaceSetting4_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter value from an InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle to the InterfaceSetting entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
InterfaceSetting4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves a string parameter value from an InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle to the InterfaceSetting entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Buffer to receive the string value (typically 256 bytes).
 * @param[in,out] pUlSize Pointer to buffer size; receives actual string length. Usually the size of 1023 is used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Not supported.
 */
ULONG
InterfaceSetting4_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Retrieves an unsigned long parameter value from an InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle to the InterfaceSetting entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to receive the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
InterfaceSetting4_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Sets a boolean parameter value for an InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle to the InterfaceSetting entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
InterfaceSetting4_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets a string parameter value for an InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle to the InterfaceSetting entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
*/
BOOL
InterfaceSetting4_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Sets an unsigned long parameter value for an InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle to the InterfaceSetting entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
InterfaceSetting4_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Validates pending changes to an InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle to the InterfaceSetting entry.
 * @param[out] pReturnParamName Buffer to receive the name of the parameter if there's a validation.
 * @param[in,out] puLength Pointer to buffer size; receives actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation
 */
BOOL
InterfaceSetting4_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending changes to an InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle to the InterfaceSetting entry.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
InterfaceSetting4_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending changes to an InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle to the InterfaceSetting entry.
 *
 * @return The status of the operation.
 * @retval 0 on success
 * @retval error code otherwise.
 */
ULONG
InterfaceSetting4_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
