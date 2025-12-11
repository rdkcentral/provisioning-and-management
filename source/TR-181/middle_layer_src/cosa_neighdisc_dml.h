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

    module: cosa_apis_neighbordiscovery.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        10/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_APIS_NEIGHBORDISCOVERY_H
#define  _COSA_APIS_NEIGHBORDISCOVERY_H

/***********************************************************************

 APIs for Object:

    NeighborDiscovery.

    *  NeighborDiscovery_GetParamBoolValue
    *  NeighborDiscovery_GetParamIntValue
    *  NeighborDiscovery_GetParamUlongValue
    *  NeighborDiscovery_GetParamStringValue
    *  NeighborDiscovery_SetParamBoolValue
    *  NeighborDiscovery_SetParamIntValue
    *  NeighborDiscovery_SetParamUlongValue
    *  NeighborDiscovery_SetParamStringValue
    *  NeighborDiscovery_Validate
    *  NeighborDiscovery_Commit
    *  NeighborDiscovery_Rollback

***********************************************************************/
/**
 * @brief Retrieves a boolean parameter value from the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 * @param[in] ParamName Name of the boolean parameter
 * @param[out] pBool Pointer to store the retrieved boolean value
 *
 * @return status of operation
 * @retval TRUE Parameter found and value stored in pBool.
 * @retval FALSE Parameter not found or error occurred.
 */
BOOL
NeighborDiscovery_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter value from the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 * @param[in] ParamName Name of the integer parameter.
 * @param[out] pInt Pointer to store the retrieved signed integer value.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value stored in pInt.
 * @retval FALSE Parameter not found or error occurred.
 */
BOOL
NeighborDiscovery_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter value from the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 * @param[in] ParamName Name of the ulong parameter .
 * @param[out] pUlong Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found or error occurred.
 */
BOOL
NeighborDiscovery_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter value from the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
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
NeighborDiscovery_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter value in the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 * @param[in] ParamName Name of the boolean parameter
 * @param[in] bValue Boolean value to set .
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
NeighborDiscovery_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets an integer parameter value in the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 * @param[in] ParamName Name of the integer parameter.
 * @param[in] value Signed integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
NeighborDiscovery_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets an unsigned long parameter value in the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 * @param[in] ParamName Name of the ulong parameter.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
NeighborDiscovery_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets a string parameter value in the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 * @param[in] ParamName Name of the string parameter.
 * @param[in] strValue Null-terminated string value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
NeighborDiscovery_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates staged changes to the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 * @param[out] pReturnParamName Buffer (128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength On input: buffer size; on output: actual parameter name length if validation fails.
 *
 * @return status of operation
 * @retval TRUE  if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
NeighborDiscovery_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits staged changes to the Device.NeighborDiscovery object.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 *
 * @return Status of operation
 * @retval ANSC_STATUS_SUCCESS Changes committed and NDP updated.
 * @retval ANSC_STATUS_FAILURE Commit failed.
 */
ULONG
NeighborDiscovery_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted changes to the Device.NeighborDiscovery object whenever there's a validation found.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery object context.
 *
 * @return Status of operation.
 * @retval ANSC_STATUS_SUCCESS Changes rolled back successfully.
 * @retval ANSC_STATUS_FAILURE Rollback operation failed.
 */
ULONG
NeighborDiscovery_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    NeighborDiscovery.InterfaceSetting.{i}.

    *  InterfaceSetting2_GetEntryCount
    *  InterfaceSetting2_GetEntry
    *  InterfaceSetting2_AddEntry
    *  InterfaceSetting2_DelEntry
    *  InterfaceSetting2_GetParamBoolValue
    *  InterfaceSetting2_GetParamIntValue
    *  InterfaceSetting2_GetParamUlongValue
    *  InterfaceSetting2_GetParamStringValue
    *  InterfaceSetting2_SetParamBoolValue
    *  InterfaceSetting2_SetParamIntValue
    *  InterfaceSetting2_SetParamUlongValue
    *  InterfaceSetting2_SetParamStringValue
    *  InterfaceSetting2_Validate
    *  InterfaceSetting2_Commit
    *  InterfaceSetting2_Rollback

***********************************************************************/
/**
 * @brief Gets the number of Device.NeighborDiscovery.InterfaceSetting.{i} table entries.
 *
 * @param[in] hInsContext Instance handle
 *
 * @return Number of InterfaceSetting table entries.
 * @retval Count of configured interface settings.
 */
ULONG
InterfaceSetting2_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific Device.NeighborDiscovery.InterfaceSetting.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery context.
 * @param[in] nIndex Zero-based index of the InterfaceSetting entry to retrieve.
 * @param[out] pInsNumber Pointer to store the instance number of the retrieved entry.
 *
 * @return status of operation
 * @retval Non-NULL Handle to the InterfaceSetting entry context.
 * @retval NULL Entry not found at specified index.
 */
ANSC_HANDLE
InterfaceSetting2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new Device.NeighborDiscovery.InterfaceSetting.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery context.
 * @param[out] pInsNumber Pointer to store the allocated instance number for the new entry.
 *
 * @return Instance handle of the InterfaceSetting entry.
 * @retval Non-NULL Handle to the new InterfaceSetting entry.
 * @retval NULL Failed to create entry.
 */
ANSC_HANDLE
InterfaceSetting2_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a Device.NeighborDiscovery.InterfaceSetting.{i} table entry.
 *
 * @param[in] hInsContext Instance handle for the NeighborDiscovery context.
 * @param[in] hInstance Instance handle of the InterfaceSetting entry to delete.
 *
 * @return Status of operation
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval ANSC_STATUS_FAILURE Failed to delete entry.
 */
ULONG
InterfaceSetting2_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter from a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 * @param[in] ParamName Name of the boolean parameter
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found.
 */
BOOL
InterfaceSetting2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter from a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 * @param[in] ParamName Name of the integer parameter.
 * @param[out] pInt Pointer to store the retrieved signed integer value.
 *
 * @return status of operation.
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found.
 */
BOOL
InterfaceSetting2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter from a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 * @param[in] ParamName Name of the ulong parameter.
 * @param[out] pUlong Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE Parameter found
 * @retval FALSE Parameter not found.
 */
BOOL
InterfaceSetting2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter from a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 * @param[in] ParamName Name of the string parameter
 * @param[out] pValue Buffer to store the retrieved string, size determined by pUlSize.
 * @param[in,out] pUlSize On input: buffer size; on output: actual string length. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small, pUlSize contains required size.
 * @retval  -1 if not supported.
 */
ULONG
InterfaceSetting2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter in a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 * @param[in] ParamName Name of the boolean parameter
 * @param[in] bValue Boolean value to set .
 *
 * @return status of operation
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
InterfaceSetting2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets an integer parameter in a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 * @param[in] ParamName Name of the integer parameter.
 * @param[in] value Signed integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
InterfaceSetting2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets an unsigned long parameter in a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 * @param[in] ParamName Name of the ulong parameter.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
InterfaceSetting2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets a string parameter in a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 * @param[in] ParamName Name of the string parameter
 * @param[in] strValue Null-terminated string value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter sets successfully.
 * @retval FALSE Parameter not recognized or invalid value.
 */
BOOL
InterfaceSetting2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates staged changes to a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 * @param[out] pReturnParamName Buffer(128 bytes) to store the name of the parameter if there's no validation, size determined by puLength.
 * @param[in,out] puLength On input: buffer size; on output: actual parameter name length if validation fails.
 *
 * @return status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
InterfaceSetting2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits staged changes to a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 *
 * @return Status of operation
 * @retval ANSC_STATUS_SUCCESS Changes committed and NDP configured on interface.
 * @retval ANSC_STATUS_FAILURE Commit failed
 */
ULONG
InterfaceSetting2_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted changes to a Device.NeighborDiscovery.InterfaceSetting.{i} entry.
 *
 * @param[in] hInsContext Instance handle for the InterfaceSetting entry.
 *
 * @return Status of operation.
 * @retval ANSC_STATUS_SUCCESS Changes rolled back successfully.
 * @retval ANSC_STATUS_FAILURE Rollback operation failed.
 */
ULONG
InterfaceSetting2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
