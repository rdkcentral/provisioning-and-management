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

    module: cosa_ppp_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        07/12/2011    initial revision.

**************************************************************************/

#ifndef FEATURE_RDKB_XDSL_PPP_MANAGER

#ifndef  _COSA_PPP_DML_H
#define  _COSA_PPP_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_ppp_internal.h"

/***********************************************************************

 APIs for Object:

    PPP.

    *  PPP_GetParamBoolValue
    *  PPP_GetParamIntValue
    *  PPP_GetParamUlongValue
    *  PPP_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves a boolean parameter value from the Device.PPP object.
 *
 * @param[in] hInsContext Instance handle for the PPP object context.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[out] pBool Pointer to store the retrieved boolean value (TRUE/FALSE).
 *
 * @return status of operation
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found or error occurred.
 */
BOOL
PPP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter value from the Device.PPP object.
 *
 * @param[in] hInsContext Instance handle for the PPP object context.
 * @param[in] ParamName Name of the integer parameter.
 * @param[out] pInt Pointer to store the retrieved signed integer value.
 *
 * @return status of operation
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found or error occurred.
 */
BOOL
PPP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter value from the Device.PPP object.
 *
 * @param[in] hInsContext Instance handle for the PPP object context.
 * @param[in] ParamName Name of the ulong parameter.
 * @param[out] pUlong Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found or error occurred.
 */
BOOL
PPP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter value from the Device.PPP object.
 *
 * @param[in] hInsContext Instance handle for the PPP object context.
 * @param[in] ParamName Name of the string parameter.
 * @param[out] pValue Buffer to store the retrieved string value, size determined by pUlSize.
 * @param[in,out] pUlSize On input: size of pValue buffer; on output: actual string length. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter retrieved and stored in pValue.
 * @retval 1 Buffer insufficient, pUlSize contains required size.
 * @retval -1 if not supported.
 */
ULONG
PPP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    PPP.Interface.{i}.

    *  Interface_GetEntryCount
    *  Interface_GetEntry
    *  Interface_AddEntry
    *  Interface_DelEntry
    *  Interface_GetParamBoolValue
    *  Interface_GetParamIntValue
    *  Interface_GetParamUlongValue
    *  Interface_GetParamStringValue
    *  Interface_SetParamBoolValue
    *  Interface_SetParamIntValue
    *  Interface_SetParamUlongValue
    *  Interface_SetParamStringValue
    *  Interface_Validate
    *  Interface_Commit
    *  Interface_Rollback

***********************************************************************/
/**
 * @brief Gets the number of Device.PPP.Interface.{i} table entries.
 *
 * @param[in] hInsContext Instance handle.
 *
 * @return Number of Interface table entries.
 * @retval Count of configured PPP interfaces.
 */
ULONG
Interface3_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific Device.PPP.Interface.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the PPP context.
 * @param[in] nIndex Zero-based index of the Interface entry to retrieve.
 * @param[out] pInsNumber Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the Interface entry context.
 * @retval Non-NULL Handle to the Interface entry context.
 * @retval NULL Entry not found at specified index.
 */
ANSC_HANDLE
Interface3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new Device.PPP.Interface.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the PPP context.
 * @param[out] pInsNumber Pointer to store the allocated instance number for the new entry.
 *
 * @return Instance handle of the Interface entry
 * @retval Non-NULL Handle to the new Interface entry.
 * @retval NULL Failed to create entry .
 */
ANSC_HANDLE
Interface3_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a Device.PPP.Interface.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the PPP context.
 * @param[in] hInstance Instance handle of the Interface entry to delete.
 *
 * @return Status of operation
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval ANSC_STATUS_FAILURE Failed to delete entry.
 */
ULONG
Interface3_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter from a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found.
 */
BOOL
Interface3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter from a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName Name of the integer parameter.
 * @param[out] pInt Pointer to store the retrieved signed integer value.
 *
 * @return status of operation
 * @retval TRUE Parameter found.
 * @retval FALSE Parameter not found.
 */
BOOL
Interface3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter from a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName Name of the ulong parameter.
 * @param[out] pUlong Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE Parameter found.
 * @retval FALSE Parameter not found.
 */
BOOL
Interface3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter from a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName Name of the string parameter.
 * @param[out] pValue Buffer to store the retrieved string, size determined by pUlSize.
 * @param[in,out] pUlSize On input: buffer size; on output: actual string length.  Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small, pUlSize contains required size.
 * @retval -1 if not supported
 */
ULONG
Interface3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter in a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName Name of the boolean parameter
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
Interface3_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets an integer parameter in a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName Name of the integer parameter.
 * @param[in] value Signed integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
Interface3_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets an unsigned long parameter in a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName Name of the ulong parameter.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
Interface3_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets a string parameter in a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName Name of the string parameter
 * @param[in] strValue Null-terminated string value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
Interface3_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates staged changes to a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[out] pReturnParamName Buffer to store the name of the parameter if there's a validation, size determined by puLength.
 * @param[in,out] puLength On input: buffer size; on output: actual parameter name length if validation fails.
 *
 * @return status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Interface3_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits staged changes to a Device.PPP.Interface.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @return Status of operation
 * @retval ANSC_STATUS_SUCCESS Changes committed and PPP interface configured.
 * @retval ANSC_STATUS_FAILURE Commit failed .
 */
ULONG
Interface3_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted changes to a Device.PPP.Interface.{i} entry whenever there's a validation found.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @return Status of operation
 * @retval ANSC_STATUS_SUCCESS Changes rolled back successfully.
 * @retval ANSC_STATUS_FAILURE Rollback operation failed.
 */
ULONG
Interface3_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    PPP.Interface.{i}.PPPoE.

    *  PPPoE_GetParamBoolValue
    *  PPPoE_GetParamIntValue
    *  PPPoE_GetParamUlongValue
    *  PPPoE_GetParamStringValue
    *  PPPoE_SetParamBoolValue
    *  PPPoE_SetParamIntValue
    *  PPPoE_SetParamUlongValue
    *  PPPoE_SetParamStringValue
    *  PPPoE_Validate
    *  PPPoE_Commit
    *  PPPoE_Rollback

***********************************************************************/
/**
* @brief Retrieves a boolean parameter value from the PPPoE object.
*
* @param[in] hInsContext - Instance handle for the PPPoE object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pBool      - Pointer to store the retrieved boolean value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
PPPoE_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
* @brief Retrieves an integer parameter value from the PPPoE object.
*
* @param[in] hInsContext - Instance handle for the PPPoE object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pInt       - Pointer to store the retrieved integer value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
PPPoE_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
* @brief Retrieves an unsigned long parameter value from the PPPoE object.
*
* @param[in] hInsContext - Instance handle for the PPPoE object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pUlong     - Pointer to store the retrieved unsigned long value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
PPPoE_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
* @brief Retrieves a string parameter value from the PPPoE object.
*
* @param[in] hInsContext  - Instance handle for the PPPoE object context.
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
PPPoE_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
* @brief Sets a boolean parameter value for the PPPoE object.
*
* @param[in] hInsContext - Instance handle for the PPPoE object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[in] bValue      - The boolean value to set.
*
* @return The status of the operation.
* @retval TRUE if the parameter is set successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
PPPoE_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
* @brief Sets an integer parameter value for the PPPoE object.
*
* @param[in] hInsContext - Instance handle for the PPPoE object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[in] value       - The integer value to set.
*
* @return The status of the operation.
* @retval TRUE if the parameter is set successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
PPPoE_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
* @brief Sets an unsigned long parameter value for the PPPoE object.
*
* @param[in] hInsContext   - Instance handle for the PPPoE object context.
* @param[in] ParamName     - Pointer to the parameter name.
* @param[in] uValuepUlong  - The unsigned long value to set.
*
* @return The status of the operation.
* @retval TRUE if the parameter is set successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
PPPoE_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
* @brief Sets a string parameter value for the PPPoE object.
*
* @param[in] hInsContext - Instance handle for the PPPoE object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[in] strValue    - Pointer to the string value to set.
*
* @return The status of the operation.
* @retval TRUE if the parameter is set successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
PPPoE_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
* @brief Validates pending parameter changes for the PPPoE object.
*
* @param[in] hInsContext       - Instance handle for the PPPoE object context.
* @param[out] pReturnParamName - Pointer to a buffer(128 bytes) to store the parameter name if there's a validation.
* @param[in,out] puLength      - Pointer to the buffer size; updated with actual size on return.
*
* @return The validation result.
* @retval TRUE if there's no validation.
* @retval FALSE if there's validation.
*
*/
BOOL
PPPoE_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
* @brief Commits pending parameter changes for the PPPoE object.
*
* @param[in] hInsContext - Instance handle for the PPPoE object context.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ULONG
PPPoE_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
* @brief Rolls back pending parameter changes for the PPPoE object whenever there's a validation found.
*
* @param[in] hInsContext - Instance handle for the PPPoE object context.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ULONG
PPPoE_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    PPP.Interface.{i}.IPCP.

    *  IPCP_GetParamBoolValue
    *  IPCP_GetParamIntValue
    *  IPCP_GetParamUlongValue
    *  IPCP_GetParamStringValue
    *  IPCP_SetParamBoolValue
    *  IPCP_SetParamIntValue
    *  IPCP_SetParamUlongValue
    *  IPCP_SetParamStringValue
    *  IPCP_Validate
    *  IPCP_Commit
    *  IPCP_Rollback

***********************************************************************/
/**
* @brief Retrieves a boolean parameter value from the IPCP object.
*
* @param[in] hInsContext - Instance handle for the IPCP object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pBool      - Pointer to store the retrieved boolean value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
IPCP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
* @brief Retrieves an integer parameter value from the IPCP object.
*
* @param[in] hInsContext - Instance handle for the IPCP object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pInt       - Pointer to store the retrieved integer value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
IPCP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
* @brief Retrieves an unsigned long parameter value from the IPCP object.
*
* @param[in] hInsContext - Instance handle for the IPCP object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pUlong     - Pointer to store the retrieved unsigned long value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
IPCP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
* @brief Retrieves a string parameter value from the IPCP object.
*
* @param[in] hInsContext  - Instance handle for the IPCP object context.
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
IPCP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
* @brief Sets a boolean parameter value for the IPCP object.
*
* @param[in] hInsContext - Instance handle for the IPCP object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[in] bValue      - The boolean value to set.
*
* @return The status of the operation.
* @retval TRUE if the parameter is set successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
IPCP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
* @brief Sets an integer parameter value for the IPCP object.
*
* @param[in] hInsContext - Instance handle for the IPCP object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[in] value       - The integer value to set.
*
* @return The status of the operation.
* @retval TRUE if the parameter is set successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
IPCP_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
* @brief Sets an unsigned long parameter value for the IPCP object.
*
* @param[in] hInsContext   - Instance handle for the IPCP object context.
* @param[in] ParamName     - Pointer to the parameter name.
* @param[in] uValuepUlong  - The unsigned long value to set.
*
* @return The status of the operation.
* @retval TRUE if the parameter is set successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
IPCP_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
* @brief Sets a string parameter value for the IPCP object.
*
* @param[in] hInsContext - Instance handle for the IPCP object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[in] strValue    - Pointer to the string value to set.
*
* @return The status of the operation.
* @retval TRUE if the parameter is set successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
IPCP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
* @brief Validates pending parameter changes for the IPCP object.
*
* @param[in] hInsContext       - Instance handle for the IPCP object context.
* @param[out] pReturnParamName - Pointer to a buffer(128 bytes) to store the parameter name if there's a validation.
* @param[in,out] puLength      - Pointer to the buffer size; updated with actual size on return.
*
* @return The validation result.
* @retval TRUE if there's no validation.
* @retval FALSE if there's validation .
*
*/
BOOL
IPCP_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
* @brief Commits pending parameter changes for the IPCP object.
*
* @param[in] hInsContext - Instance handle for the IPCP object context.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ULONG
IPCP_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
* @brief Rolls back pending parameter changes for the IPCP object whenever there's a validation found.
*
* @param[in] hInsContext - Instance handle for the IPCP object context.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ULONG
IPCP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    PPP.Interface.{i}.Stats.

    *  Stats_GetParamBoolValue
    *  Stats_GetParamIntValue
    *  Stats_GetParamUlongValue
    *  Stats_GetParamStringValue

***********************************************************************/
/**
* @brief Retrieves a boolean parameter value from the Stats object.
*
* @param[in] hInsContext - Instance handle for the Stats object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pBool      - Pointer to store the retrieved boolean value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
Stats6_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
* @brief Retrieves an integer parameter value from the Stats object.
*
* @param[in] hInsContext - Instance handle for the Stats object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pInt       - Pointer to store the retrieved integer value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
Stats6_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
* @brief Retrieves an unsigned long parameter value from the Stats object.
*
* @param[in] hInsContext - Instance handle for the Stats object context.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pUlong     - Pointer to store the retrieved unsigned long value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
Stats6_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
* @brief Retrieves a string parameter value from the Stats object.
*
* @param[in] hInsContext  - Instance handle for the Stats object context.
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
Stats6_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
#endif
