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

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


/**************************************************************************

    module: cosa_bridging_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        03/14/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_BRIDGING_DML_H
#define  _COSA_BRIDGING_DML_H

/***********************************************************************

 APIs for Object:

    Bridging.

    *  Bridging_GetParamBoolValue
    *  Bridging_GetParamIntValue
    *  Bridging_GetParamUlongValue
    *  Bridging_GetParamStringValue

***********************************************************************/
/**
 * @brief Get boolean parameter value from Bridging object.
 *
 * This function retrieves a boolean parameter value from the Bridging object.
 *
 * @param[in] hInsContext - The instance handle.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridging_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from Bridging object.
 *
 * This function retrieves an integer parameter value from the Bridging object.
 *
 * @param[in] hInsContext - The instance handle.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridging_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from Bridging object.
 *
 * This function retrieves an unsigned long parameter value from the Bridging object.
 *
 * @param[in] hInsContext - The instance handle.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridging_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from Bridging object.
 *
 * This function retrieves a string parameter value from the Bridging object.
 *
 * @param[in] hInsContext  - The instance handle.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not supported.
 *
 */
ULONG
Bridging_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.

    *  Bridge_GetEntryCount
    *  Bridge_GetEntry
    *  Bridge_AddEntry
    *  Bridge_DelEntry
    *  Bridge_GetParamBoolValue
    *  Bridge_GetParamIntValue
    *  Bridge_GetParamUlongValue
    *  Bridge_GetParamStringValue
    *  Bridge_SetParamBoolValue
    *  Bridge_SetParamIntValue
    *  Bridge_SetParamUlongValue
    *  Bridge_SetParamStringValue
    *  Bridge_Validate
    *  Bridge_Commit
    *  Bridge_Rollback

***********************************************************************/
/**
 * @brief Get the count of Bridge table entries.
 *
 * This function retrieves the number of Bridge entries in the Bridging.Bridge table.
 *
 * @param[in] hInsContext - The instance handle.
 *
 * @return The number of Bridge entries.
 * @retval ULONG The count of Bridge table entries.
 *
 */
ULONG
Bridge_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a Bridge entry by index.
 *
 * This function retrieves a Bridge entry from the Bridging.Bridge table by the specified index.
 *
 * @param[in] hInsContext  - The instance handle.
 * @param[in] nIndex       - The zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  - Pointer to store the instance number of the entry.
 *
 * @return Handle to the Bridge entry.
 * @retval ANSC_HANDLE Handle to the Bridge entry if found.
 * @retval NULL if entry is not found.
 *
 */
ANSC_HANDLE
Bridge_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new Bridge entry.
 *
 * This function adds a new entry to the Bridging.Bridge table.
 *
 * @param[in] hInsContext  - The instance handle.
 * @param[out] pInsNumber  - Pointer to store the instance number of the new entry.
 *
 * @return Handle to the newly created Bridge entry.
 * @retval ANSC_HANDLE Handle to the new Bridge entry if successful.
 * @retval NULL if the operation fails.
 *
 */
ANSC_HANDLE
Bridge_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a Bridge entry.
 *
 * This function deletes an entry from the Bridging.Bridge table.
 *
 * @param[in] hInsContext - The instance handle.
 * @param[in] hInstance   - Handle to the Bridge instance to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ULONG
Bridge_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter value from Bridge entry.
 *
 * This function retrieves a boolean parameter value from a Bridge entry.
 *
 * @param[in] hInsContext - The instance handle to the Bridge entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridge_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from Bridge entry.
 *
 * This function retrieves an integer parameter value from a Bridge entry.
 *
 * @param[in] hInsContext - The instance handle to the Bridge entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridge_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from Bridge entry.
 *
 * This function retrieves an unsigned long parameter value from a Bridge entry.
 *
 * @param[in] hInsContext - The instance handle to the Bridge entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridge_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from Bridge entry.
 *
 * This function retrieves a string parameter value from a Bridge entry.
 *
 * @param[in] hInsContext  - The instance handle to the Bridge entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not supported
 *
 */
ULONG
Bridge_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for Bridge entry.
 *
 * This function sets a boolean parameter value for a Bridge entry.
 *
 * @param[in] hInsContext - The instance handle to the Bridge entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridge_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for Bridge entry.
 *
 * This function sets an integer parameter value for a Bridge entry.
 *
 * @param[in] hInsContext - The instance handle to the Bridge entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] value       - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridge_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for Bridge entry.
 *
 * This function sets an unsigned long parameter value for a Bridge entry.
 *
 * @param[in] hInsContext  - The instance handle to the Bridge entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[in] uValuepUlong - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridge_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for Bridge entry.
 *
 * This function sets a string parameter value for a Bridge entry.
 *
 * @param[in] hInsContext - The instance handle to the Bridge entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] strValue    - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Bridge_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Bridge entry parameter values.
 *
 * This function validates the parameter values of a Bridge entry before committing.
 *
 * @param[in] hInsContext        - The instance handle to the Bridge entry.
 * @param[out] pReturnParamName  - Pointer to a buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength       - Pointer to the buffer length.
 *
 * @return The validation result.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 *
 */
BOOL
Bridge_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Bridge entry parameter changes.
 *
 * This function commits the parameter changes made to a Bridge entry.
 *
 * @param[in] hInsContext - The instance handle to the Bridge entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
Bridge_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback Bridge entry parameter changes.
 *
 * This function rolls back uncommitted parameter changes made to a Bridge entry.
 *
 * @param[in] hInsContext - The instance handle to the Bridge entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
Bridge_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.Port.{i}.

    *  Port_GetEntryCount
    *  Port_GetEntry
    *  Port_AddEntry
    *  Port_DelEntry
    *  Port_GetParamBoolValue
    *  Port_GetParamIntValue
    *  Port_GetParamUlongValue
    *  Port_GetParamStringValue
    *  Port_SetParamBoolValue
    *  Port_SetParamIntValue
    *  Port_SetParamUlongValue
    *  Port_SetParamStringValue
    *  Port_Validate
    *  Port_Commit
    *  Port_Rollback

***********************************************************************/
/**
 * @brief Get the count of Port table entries.
 *
 * This function retrieves the number of Port entries in the Bridging.Bridge.{i}.Port table.
 *
 * @param[in] ANSC_HANDLE - The instance handle (unused parameter).
 *
 * @return The number of Port entries.
 * @retval ULONG The count of Port table entries.
 *
 */
ULONG
Port_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a Port entry by index.
 *
 * This function retrieves a Port entry from the Bridging.Bridge.{i}.Port table by the specified index.
 *
 * @param[in] hInsContext  - The instance handle to the parent Bridge.
 * @param[in] nIndex       - The zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  - Pointer to store the instance number of the entry.
 *
 * @return Handle to the Port entry.
 * @retval ANSC_HANDLE Handle to the Port entry if found.
 * @retval NULL if entry is not found.
 *
 */
ANSC_HANDLE
Port_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new Port entry.
 *
 * This function adds a new entry to the Bridging.Bridge.{i}.Port table.
 *
 * @param[in] hInsContext  - The instance handle to the parent Bridge.
 * @param[out] pInsNumber  - Pointer to store the instance number of the new entry.
 *
 * @return Handle to the newly created Port entry.
 * @retval ANSC_HANDLE Handle to the new Port entry if successful.
 * @retval NULL if the operation fails.
 *
 */
ANSC_HANDLE
Port_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a Port entry.
 *
 * This function deletes an entry from the Bridging.Bridge.{i}.Port table.
 *
 * @param[in] hInsContext - The instance handle to the parent Bridge.
 * @param[in] hInstance   - Handle to the Port instance to delete.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval -1 if the operation fails.
 *
 */
ULONG
Port_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter value from Port entry.
 *
 * This function retrieves a boolean parameter value from a Port entry.
 *
 * @param[in] hInsContext - The instance handle to the Port entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Port_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from Port entry.
 *
 * This function retrieves an integer parameter value from a Port entry.
 *
 * @param[in] hInsContext - The instance handle to the Port entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Port_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from Port entry.
 *
 * This function retrieves an unsigned long parameter value from a Port entry.
 *
 * @param[in] hInsContext - The instance handle to the Port entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Port_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from Port entry.
 *
 * This function retrieves a string parameter value from a Port entry.
 *
 * @param[in] hInsContext  - The instance handle to the Port entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if the parameter is not found or operation fails.
 *
 */
ULONG
Port_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for Port entry.
 *
 * This function sets a boolean parameter value for a Port entry.
 *
 * @param[in] hInsContext - The instance handle to the Port entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Port_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for Port entry.
 *
 * This function sets an integer parameter value for a Port entry.
 *
 * @param[in] hInsContext - The instance handle to the Port entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] value       - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Port_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for Port entry.
 *
 * This function sets an unsigned long parameter value for a Port entry.
 *
 * @param[in] hInsContext  - The instance handle to the Port entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[in] uValuepUlong - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Port_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for Port entry.
 *
 * This function sets a string parameter value for a Port entry.
 *
 * @param[in] hInsContext - The instance handle to the Port entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] strValue    - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Port_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Port entry parameter values.
 *
 * This function validates the parameter values of a Port entry before committing.
 *
 * @param[in] hInsContext        - The instance handle to the Port entry.
 * @param[out] pReturnParamName  - Pointer to a buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength       - Pointer to the buffer length.
 *
 * @return The validation result.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation .
 *
 */
BOOL
Port_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Port entry parameter changes.
 *
 * This function commits the parameter changes made to a Port entry.
 *
 * @param[in] hInsContext - The instance handle to the Port entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
Port_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback Port entry parameter changes.
 *
 * This function rolls back uncommitted parameter changes made to a Port entry.
 *
 * @param[in] hInsContext - The instance handle to the Port entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
Port_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.Port.{i}.Stats.

    *  PortStats_GetParamBoolValue
    *  PortStats_GetParamIntValue
    *  PortStats_GetParamUlongValue
    *  PortStats_GetParamStringValue

***********************************************************************/
/**
 * @brief Get boolean parameter value from Port statistics.
 *
 * @param[in] hInsContext - The instance handle to the Port Stats.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
PortStats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from Port statistics.
 *
 * @param[in] hInsContext - The instance handle to the Port Stats.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
PortStats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from Port statistics.
 *
 * @param[in] hInsContext - The instance handle to the Port Stats.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
PortStats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from Port statistics.
 *
 * @param[in] hInsContext  - The instance handle to the Port Stats.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not supported
 *
 */
ULONG
PortStats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.VLAN.{i}.

    *  VLAN_GetEntryCount
    *  VLAN_GetEntry
    *  VLAN_AddEntry
    *  VLAN_DelEntry
    *  VLAN_GetParamBoolValue
    *  VLAN_GetParamIntValue
    *  VLAN_GetParamUlongValue
    *  VLAN_GetParamStringValue
    *  VLAN_SetParamBoolValue
    *  VLAN_SetParamIntValue
    *  VLAN_SetParamUlongValue
    *  VLAN_SetParamStringValue
    *  VLAN_Validate
    *  VLAN_Commit
    *  VLAN_Rollback

***********************************************************************/
/**
 * @brief Get the count of VLAN table entries.
 *
 * @param[in] ANSC_HANDLE - The instance handle (unused parameter).
 *
 * @return The number of VLAN entries.
 * @retval ULONG The count of VLAN table entries.
 *
 */
ULONG
VLAN_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a VLAN entry by index.
 *
 * @param[in] hInsContext  - The instance handle to the parent Bridge.
 * @param[in] nIndex       - The zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  - Pointer to store the instance number of the entry.
 *
 * @return Handle to the VLAN entry.
 * @retval ANSC_HANDLE Handle to the VLAN entry if found.
 * @retval NULL if entry is not found.
 *
 */
ANSC_HANDLE
VLAN_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new VLAN entry.
 *
 * @param[in] hInsContext  - The instance handle to the parent Bridge.
 * @param[out] pInsNumber  - Pointer to store the instance number of the new entry.
 *
 * @return Handle to the newly created VLAN entry.
 * @retval ANSC_HANDLE Handle to the new VLAN entry if successful.
 * @retval NULL if the operation fails.
 *
 */
ANSC_HANDLE
VLAN_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a VLAN entry.
 *
 * @param[in] hInsContext - The instance handle to the parent Bridge.
 * @param[in] hInstance   - Handle to the VLAN instance to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ULONG
VLAN_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter value from VLAN entry.
 *
 * @param[in] hInsContext - The instance handle to the VLAN entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLAN_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from VLAN entry.
 *
 * @param[in] hInsContext - The instance handle to the VLAN entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLAN_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from VLAN entry.
 *
 * @param[in] hInsContext - The instance handle to the VLAN entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLAN_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from VLAN entry.
 *
 * @param[in] hInsContext  - The instance handle to the VLAN entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not supported
 *
 */
ULONG
VLAN_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for VLAN entry.
 *
 * @param[in] hInsContext - The instance handle to the VLAN entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLAN_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for VLAN entry.
 *
 * @param[in] hInsContext - The instance handle to the VLAN entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] value       - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLAN_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for VLAN entry.
 *
 * @param[in] hInsContext  - The instance handle to the VLAN entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[in] uValuepUlong - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLAN_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for VLAN entry.
 *
 * @param[in] hInsContext - The instance handle to the VLAN entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] strValue    - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLAN_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate VLAN entry parameter values.
 *
 * @param[in] hInsContext        - The instance handle to the VLAN entry.
 * @param[out] pReturnParamName  - Pointer to a buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength       - Pointer to the buffer length.
 *
 * @return The validation result.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 *
 */
BOOL
VLAN_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit VLAN entry parameter changes.
 *
 * @param[in] hInsContext - The instance handle to the VLAN entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
VLAN_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback VLAN entry parameter changes.
 *
 * @param[in] hInsContext - The instance handle to the VLAN entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
VLAN_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.VLANPort.{i}.

    *  VLANPort_GetEntryCount
    *  VLANPort_GetEntry
    *  VLANPort_AddEntry
    *  VLANPort_DelEntry
    *  VLANPort_GetParamBoolValue
    *  VLANPort_GetParamIntValue
    *  VLANPort_GetParamUlongValue
    *  VLANPort_GetParamStringValue
    *  VLANPort_SetParamBoolValue
    *  VLANPort_SetParamIntValue
    *  VLANPort_SetParamUlongValue
    *  VLANPort_SetParamStringValue
    *  VLANPort_Validate
    *  VLANPort_Commit
    *  VLANPort_Rollback

***********************************************************************/
/**
 * @brief Get the count of VLANPort table entries.
 *
 * @param[in] ANSC_HANDLE - The instance handle (unused parameter).
 *
 * @return The number of VLANPort entries.
 * @retval ULONG The count of VLANPort table entries.
 *
 */
ULONG
VLANPort_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a VLANPort entry by index.
 *
 * @param[in] hInsContext  - The instance handle to the parent Bridge.
 * @param[in] nIndex       - The zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  - Pointer to store the instance number of the entry.
 *
 * @return Handle to the VLANPort entry.
 * @retval ANSC_HANDLE Handle to the VLANPort entry if found.
 * @retval NULL if entry is not found.
 *
 */
ANSC_HANDLE
VLANPort_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new VLANPort entry.
 *
 * @param[in] hInsContext  - The instance handle to the parent Bridge.
 * @param[out] pInsNumber  - Pointer to store the instance number of the new entry.
 *
 * @return Handle to the newly created VLANPort entry.
 * @retval ANSC_HANDLE Handle to the new VLANPort entry if successful.
 * @retval NULL if the operation fails.
 *
 */
ANSC_HANDLE
VLANPort_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a VLANPort entry.
 *
 * @param[in] hInsContext - The instance handle to the parent Bridge.
 * @param[in] hInstance   - Handle to the VLANPort instance to delete.
 *
 * @return The status of the operation.
 *
 */
ULONG
VLANPort_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter value from VLANPort entry.
 *
 * @param[in] hInsContext - The instance handle to the VLANPort entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLANPort_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from VLANPort entry.
 *
 * @param[in] hInsContext - The instance handle to the VLANPort entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLANPort_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from VLANPort entry.
 *
 * @param[in] hInsContext - The instance handle to the VLANPort entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLANPort_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from VLANPort entry.
 *
 * @param[in] hInsContext  - The instance handle to the VLANPort entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not supported
 *
 */
ULONG
VLANPort_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for VLANPort entry.
 *
 * @param[in] hInsContext - The instance handle to the VLANPort entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLANPort_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for VLANPort entry.
 *
 * @param[in] hInsContext - The instance handle to the VLANPort entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] value       - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLANPort_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for VLANPort entry.
 *
 * @param[in] hInsContext  - The instance handle to the VLANPort entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[in] uValuepUlong - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLANPort_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for VLANPort entry.
 *
 * @param[in] hInsContext - The instance handle to the VLANPort entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] strValue    - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VLANPort_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate VLANPort entry parameter values.
 *
 * @param[in] hInsContext        - The instance handle to the VLANPort entry.
 * @param[out] pReturnParamName  - Pointer to a buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength       - Pointer to the buffer length.
 *
 * @return The validation result.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 *
 */
BOOL
VLANPort_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit VLANPort entry parameter changes.
 *
 * @param[in] hInsContext - The instance handle to the VLANPort entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
VLANPort_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback VLANPort entry parameter changes.
 *
 * @param[in] hInsContext - The instance handle to the VLANPort entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
VLANPort_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#if !defined (RESOURCE_OPTIMIZATION)

/***********************************************************************

 APIs for Object:

    Bridging.Filter.{i}.

    *  Filter_GetEntryCount
    *  Filter_GetEntry
    *  Filter_AddEntry
    *  Filter_DelEntry
    *  Filter_GetParamBoolValue
    *  Filter_GetParamIntValue
    *  Filter_GetParamUlongValue
    *  Filter_GetParamStringValue
    *  Filter_SetParamBoolValue
    *  Filter_SetParamIntValue
    *  Filter_SetParamUlongValue
    *  Filter_SetParamStringValue
    *  Filter_Validate
    *  Filter_Commit
    *  Filter_Rollback

***********************************************************************/
/**
 * @brief Get the number of Filter entries.
 *
 * @param[in] hInsContext - The instance handle.
 *
 * @return The number of Filter entries.
 *
 */
ULONG
Filter_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a Filter entry by index.
 *
 * @param[in] hInsContext  - The instance handle to the parent object.
 * @param[in] nIndex       - The index of the Filter entry.
 * @param[out] pInsNumber  - Pointer to store the instance number of the Filter entry.
 *
 * @return The handle to the Filter entry.
 * @retval Non-NULL handle if the entry is found.
 * @retval NULL if the entry is not found.
 *
 */
ANSC_HANDLE
Filter_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new Filter entry.
 *
 * @param[in] hInsContext  - The instance handle to the parent object.
 * @param[out] pInsNumber  - Pointer to store the instance number of the newly created Filter entry.
 *
 * @return The handle to the newly created Filter entry.
 * @retval Non-NULL handle if the entry is created successfully.
 * @retval NULL if the entry creation fails.
 *
 */
ANSC_HANDLE
Filter_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a Filter entry.
 *
 * @param[in] hInsContext - The instance handle to the parent object.
 * @param[in] hInstance   - The instance handle to the Filter entry to be deleted.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the entry is deleted successfully.
 * @retval ANSC_STATUS_FAILURE if the deletion fails.
 *
 */
ULONG
Filter_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter value from Filter entry.
 *
 * @param[in] hInsContext - The instance handle to the Filter entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Filter_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from Filter entry.
 *
 * @param[in] hInsContext - The instance handle to the Filter entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Filter_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from Filter entry.
 *
 * @param[in] hInsContext - The instance handle to the Filter entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Filter_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from Filter entry.
 *
 * @param[in] hInsContext  - The instance handle to the Filter entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if not supported.
 *
 */
ULONG
Filter_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for Filter entry.
 *
 * @param[in] hInsContext - The instance handle to the Filter entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Filter_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for Filter entry.
 *
 * @param[in] hInsContext - The instance handle to the Filter entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] value       - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Filter_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for Filter entry.
 *
 * @param[in] hInsContext  - The instance handle to the Filter entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[in] uValuepUlong - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Filter_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for Filter entry.
 *
 * @param[in] hInsContext - The instance handle to the Filter entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] strValue    - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Filter_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Filter entry parameter values.
 *
 * @param[in] hInsContext        - The instance handle to the Filter entry.
 * @param[out] pReturnParamName  - Pointer to a buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength       - Pointer to the buffer length.
 *
 * @return The validation result.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 *
 */
BOOL
Filter_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Filter entry parameter changes.
 *
 * @param[in] hInsContext - The instance handle to the Filter entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
Filter_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback Filter entry parameter changes.
 *
 * @param[in] hInsContext - The instance handle to the Filter entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
Filter_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif

#endif
