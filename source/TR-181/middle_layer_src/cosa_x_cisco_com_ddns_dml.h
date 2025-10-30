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

    module: cosa_x_cisco_com_ddns_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/

#if !defined(DDNS_BROADBANDFORUM)
#ifndef  _COSA_X_CISCO_COM_DDNS_DML_H
#define  _COSA_X_CISCO_COM_DDNS_DML_H

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_DDNS.

    *  X_CISCO_COM_DDNS_GetParamBoolValue
    *  X_CISCO_COM_DDNS_GetParamIntValue
    *  X_CISCO_COM_DDNS_GetParamUlongValue
    *  X_CISCO_COM_DDNS_GetParamStringValue
    *  X_CISCO_COM_DDNS_SetParamBoolValue
    *  X_CISCO_COM_DDNS_SetParamIntValue
    *  X_CISCO_COM_DDNS_SetParamUlongValue
    *  X_CISCO_COM_DDNS_SetParamStringValue
    *  X_CISCO_COM_DDNS_Validate
    *  X_CISCO_COM_DDNS_Commit
    *  X_CISCO_COM_DDNS_Rollback

***********************************************************************/
/**
 * @brief Get boolean parameter value from X_CISCO_COM_DDNS object.
 *
 * Retrieves a boolean parameter value from the Cisco Dynamic DNS object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Buffer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
X_CISCO_COM_DDNS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from X_CISCO_COM_DDNS object.
 *
 * Retrieves an integer parameter value from the Cisco Dynamic DNS object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Buffer to receive the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
X_CISCO_COM_DDNS_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from X_CISCO_COM_DDNS object.
 *
 * Retrieves an unsigned long parameter value from the Cisco Dynamic DNS object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Buffer to receive the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
X_CISCO_COM_DDNS_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from X_CISCO_COM_DDNS object.
 *
 * Retrieves a string parameter value from the Cisco Dynamic DNS object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to receive the string value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of string. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 if not supported.
 */
ULONG
X_CISCO_COM_DDNS_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for X_CISCO_COM_DDNS object.
 *
 * Sets a boolean parameter value for the Cisco Dynamic DNS object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_DDNS_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for X_CISCO_COM_DDNS object.
 *
 * Sets an integer parameter value for the Cisco Dynamic DNS object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_DDNS_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for X_CISCO_COM_DDNS object.
 *
 * Sets an unsigned long parameter value for the Cisco Dynamic DNS object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_DDNS_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for X_CISCO_COM_DDNS object.
 *
 * Sets a string parameter value for the Cisco Dynamic DNS object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_DDNS_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate X_CISCO_COM_DDNS object parameter values.
 *
 * Validates staged parameter changes for the Cisco Dynamic DNS object before committing.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer for the name of parameter if there's a validation (128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
X_CISCO_COM_DDNS_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit X_CISCO_COM_DDNS object parameter changes.
 *
 * Applies validated parameter changes to the Cisco Dynamic DNS object configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
X_CISCO_COM_DDNS_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback X_CISCO_COM_DDNS object parameter changes.
 *
 * Reverts uncommitted parameter changes for the Cisco Dynamic DNS object.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
X_CISCO_COM_DDNS_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_DDNS.Service.{i}.

    *  Service_GetEntryCount
    *  Service_GetEntry
    *  Service_AddEntry
    *  Service_DelEntry
    *  Service_GetParamBoolValue
    *  Service_GetParamIntValue
    *  Service_GetParamUlongValue
    *  Service_GetParamStringValue
    *  Service_SetParamBoolValue
    *  Service_SetParamIntValue
    *  Service_SetParamUlongValue
    *  Service_SetParamStringValue
    *  Service_Validate
    *  Service_Commit
    *  Service_Rollback

***********************************************************************/
/**
 * @brief Get the count of DDNS Service table entries.
 *
 * Retrieves the total number of Dynamic DNS service entries in the Service table.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of entries in the Service table.
 */
ULONG
Service_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific entry from the DDNS Service table.
 *
 * Retrieves a specific Dynamic DNS service entry by index from the Service table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Instance number of the entry.
 *
 * @return Handle to the entry context
 * @retval non-NULL Handle to the entry context.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
Service_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new entry to the DDNS Service table.
 *
 * Creates and adds a new Dynamic DNS service entry to the Service table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Instance number assigned to the new entry.
 *
 * @return Handle to the new entry context
 * @retval non-NULL Handle to the new entry context.
 * @retval NULL Addition failed.
 */
ANSC_HANDLE
Service_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete an entry from the DDNS Service table.
 *
 * Removes a specific Dynamic DNS service entry from the Service table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval error code otherwise.
 */
ULONG
Service_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter value from DDNS Service entry.
 *
 * Retrieves a boolean parameter value from a specific Dynamic DNS service entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Buffer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Service_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from DDNS Service entry.
 *
 * Retrieves an integer parameter value from a specific Dynamic DNS service entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Buffer to receive the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Service_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from DDNS Service entry.
 *
 * Retrieves an unsigned long parameter value from a specific Dynamic DNS service entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Buffer to receive the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Service_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from DDNS Service entry.
 *
 * Retrieves a string parameter value from a specific Dynamic DNS service entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to receive the string value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of string. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 if not supported.
 */
ULONG
Service_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for DDNS Service entry.
 *
 * Sets a boolean parameter value for a specific Dynamic DNS service entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Service_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for DDNS Service entry.
 *
 * Sets an integer parameter value for a specific Dynamic DNS service entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Service_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for DDNS Service entry.
 *
 * Sets an unsigned long parameter value for a specific Dynamic DNS service entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Service_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for DDNS Service entry.
 *
 * Sets a string parameter value for a specific Dynamic DNS service entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Service_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate DDNS Service entry parameter values.
 *
 * Validates staged parameter changes for a Dynamic DNS service entry before committing.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer for the name of parameter if there's a validation(128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Service_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit DDNS Service entry parameter changes.
 *
 * Applies validated parameter changes to the Dynamic DNS service entry configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
Service_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback DDNS Service entry parameter changes.
 *
 * Reverts uncommitted parameter changes for a Dynamic DNS service entry.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Rollback successful.
 * @retval Non-zero error code otherwise.
 */
ULONG
Service_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
#endif
