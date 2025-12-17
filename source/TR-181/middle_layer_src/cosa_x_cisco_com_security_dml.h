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

    module: cosa_x_cisco_com_security_dml.h

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


#ifndef  _COSA_X_CISCO_COM_SECURITY_DML_H
#define  _COSA_X_CISCO_COM_SECURITY_DML_H

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.Firewall.

    *  X_CISCO_COM_Security_GetParamBoolValue
    *  X_CISCO_COM_Security_GetParamIntValue
    *  X_CISCO_COM_Security_GetParamUlongValue
    *  X_CISCO_COM_Security_GetParamStringValue
    *  X_CISCO_COM_Security_SetParamBoolValue
    *  X_CISCO_COM_Security_SetParamIntValue
    *  X_CISCO_COM_Security_SetParamUlongValue
    *  X_CISCO_COM_Security_SetParamStringValue
    *  X_CISCO_COM_Security_Validate
    *  X_CISCO_COM_Security_Commit
    *  X_CISCO_COM_Security_Rollback

***********************************************************************/
/**
 * @brief Get X_CISCO_COM_Security boolean parameter values.
 *
 * Retrieves boolean configuration values for security settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to a boolean where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
X_CISCO_COM_Security_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get X_CISCO_COM_Security integer parameter values.
 *
 * Retrieves integer configuration values for security settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to an integer where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
X_CISCO_COM_Security_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get X_CISCO_COM_Security unsigned long parameter values.
 *
 * Retrieves unsigned long configuration values for security settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to an unsigned long where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
X_CISCO_COM_Security_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get X_CISCO_COM_Security string parameter values.
 *
 * Retrieves string configuration values for security settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
X_CISCO_COM_Security_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set X_CISCO_COM_Security boolean parameter values.
 *
 * Updates boolean configuration values for security settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
X_CISCO_COM_Security_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set X_CISCO_COM_Security integer parameter values.
 *
 * Updates integer configuration values for security settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
X_CISCO_COM_Security_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set X_CISCO_COM_Security unsigned long parameter values.
 *
 * Updates unsigned long configuration values for security settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
X_CISCO_COM_Security_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set X_CISCO_COM_Security string parameter values.
 *
 * Updates string configuration values for security settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
X_CISCO_COM_Security_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate X_CISCO_COM_Security parameter changes.
 *
 * Validates all staged parameter changes for consistency and correctness before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Pointer to buffer (128 bytes) for parameter name if there's validation.
 * @param[in,out] puLength Pointer to buffer length for parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
X_CISCO_COM_Security_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit X_CISCO_COM_Security parameter changes.
 *
 * Applies all validated parameter changes to the active configuration and persists settings.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if commit fails.
 */
ULONG
X_CISCO_COM_Security_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback X_CISCO_COM_Security parameter changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if rollback fails.
 */
ULONG
X_CISCO_COM_Security_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.Firewall.

    *  Firewall1_GetParamBoolValue
    *  Firewall1_GetParamIntValue
    *  Firewall1_GetParamUlongValue
    *  Firewall1_GetParamStringValue
    *  Firewall1_SetParamBoolValue
    *  Firewall1_SetParamIntValue
    *  Firewall1_SetParamUlongValue
    *  Firewall1_SetParamStringValue
    *  Firewall1_Validate
    *  Firewall1_Commit
    *  Firewall1_Rollback

***********************************************************************/
/**
 * @brief Get Firewall boolean parameter values.
 *
 * Retrieves boolean configuration values for firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to a boolean where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
Firewall1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get Firewall integer parameter values.
 *
 * Retrieves integer configuration values for firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to an integer where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
Firewall1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get Firewall unsigned long parameter values.
 *
 * Retrieves unsigned long configuration values for firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to an unsigned long where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
Firewall1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get Firewall string parameter values.
 *
 * Retrieves string configuration values for firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
Firewall1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set Firewall boolean parameter values.
 *
 * Updates boolean configuration values for firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
Firewall1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set Firewall integer parameter values.
 *
 * Updates integer configuration values for firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
Firewall1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set Firewall unsigned long parameter values.
 *
 * Updates unsigned long configuration values for firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
Firewall1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set Firewall string parameter values.
 *
 * Updates string configuration values for firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
Firewall1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Firewall parameter changes.
 *
 * Validates all staged firewall parameter changes for consistency and correctness before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Pointer to buffer (128 bytes) for parameter name if there's  validation.
 * @param[in,out] puLength Pointer to buffer length for parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Firewall1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Firewall parameter changes.
 *
 * Applies all validated firewall parameter changes to the active configuration and persists settings.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if commit fails.
 */
ULONG
Firewall1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback Firewall parameter changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if rollback fails.
 */
ULONG
Firewall1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.

    *  InternetAccess_GetParamBoolValue
    *  InternetAccess_GetParamIntValue
    *  InternetAccess_GetParamUlongValue
    *  InternetAccess_GetParamStringValue

***********************************************************************/
/**
 * @brief Get InternetAccess boolean parameter values.
 *
 * Retrieves boolean configuration values for internet access control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to a boolean where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
InternetAccess_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get InternetAccess integer parameter values.
 *
 * Retrieves integer configuration values for internet access control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to an integer where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
InternetAccess_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get InternetAccess unsigned long parameter values.
 *
 * Retrieves unsigned long configuration values for internet access control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to an unsigned long where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
InternetAccess_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get InternetAccess string parameter values.
 *
 * Retrieves string configuration values for internet access control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
InternetAccess_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#if !defined (RESOURCE_OPTIMIZATION)

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.

    *  AccessPolicy_GetEntryCount
    *  AccessPolicy_GetEntry
    *  AccessPolicy_AddEntry
    *  AccessPolicy_DelEntry
    *  AccessPolicy_GetParamBoolValue
    *  AccessPolicy_GetParamIntValue
    *  AccessPolicy_GetParamUlongValue
    *  AccessPolicy_GetParamStringValue
    *  AccessPolicy_SetParamBoolValue
    *  AccessPolicy_SetParamIntValue
    *  AccessPolicy_SetParamUlongValue
    *  AccessPolicy_SetParamStringValue
    *  AccessPolicy_Validate
    *  AccessPolicy_Commit
    *  AccessPolicy_Rollback

***********************************************************************/
/**
 * @brief Get the count of AccessPolicy entries.
 *
 * Retrieves the total number of internet access policy entries configured.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of AccessPolicy entries.
 */
ULONG
AccessPolicy_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific AccessPolicy entry by index.
 *
 * Retrieves an access policy entry from the list by its zero-based index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Pointer to return the instance number of the entry.
 *
 * @return Handle to the AccessPolicy entry
 * @retval Non-NULL Handle to the AccessPolicy entry.
 * @retval NULL If entry does not exist at the specified index.
 */
ANSC_HANDLE
AccessPolicy_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new AccessPolicy entry.
 *
 * Creates and adds a new internet access policy entry to the configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Pointer to return the instance number of the new entry.
 *
 * @return Handle to the newly created AccessPolicy entry.
 * @retval Non-NULL Handle to the newly created AccessPolicy entry.
 * @retval NULL If memory allocation fails or entry creation fails.
 */
ANSC_HANDLE
AccessPolicy_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete an AccessPolicy entry.
 *
 * Removes an internet access policy entry from the configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if deletion fails.
 */
ULONG
AccessPolicy_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get AccessPolicy boolean parameter values.
 *
 * Retrieves boolean configuration values for access policy settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to a boolean where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
AccessPolicy_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get AccessPolicy integer parameter values.
 *
 * Retrieves integer configuration values for access policy settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to an integer where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
AccessPolicy_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get AccessPolicy unsigned long parameter values.
 *
 * Retrieves unsigned long configuration values for access policy settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to an unsigned long where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
AccessPolicy_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get AccessPolicy string parameter values.
 *
 * Retrieves string configuration values for access policy settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
AccessPolicy_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set AccessPolicy boolean parameter values.
 *
 * Updates boolean configuration values for access policy settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
AccessPolicy_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set AccessPolicy integer parameter values.
 *
 * Updates integer configuration values for access policy settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
AccessPolicy_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set AccessPolicy unsigned long parameter values.
 *
 * Updates unsigned long configuration values for access policy settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
AccessPolicy_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set AccessPolicy string parameter values.
 *
 * Updates string configuration values for access policy settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
AccessPolicy_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate AccessPolicy parameter changes.
 *
 * Validates all staged access policy parameter changes for consistency and correctness before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Pointer to buffer(128 bytes) for parameter name, if there's validation.
 * @param[in,out] puLength Pointer to buffer length for parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
AccessPolicy_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit AccessPolicy parameter changes.
 *
 * Applies all validated access policy parameter changes to the active configuration and persists settings.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS The operation was successful.
 * @retval Error code if commit fails.
 */
ULONG
AccessPolicy_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback AccessPolicy parameter changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if rollback fails.
 */
ULONG
AccessPolicy_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.Schedule.

    *  Schedule_GetParamBoolValue
    *  Schedule_GetParamIntValue
    *  Schedule_GetParamUlongValue
    *  Schedule_GetParamStringValue
    *  Schedule_SetParamBoolValue
    *  Schedule_SetParamIntValue
    *  Schedule_SetParamUlongValue
    *  Schedule_SetParamStringValue
    *  Schedule_Validate
    *  Schedule_Commit
    *  Schedule_Rollback

***********************************************************************/
/**
 * @brief Get Schedule boolean parameter values.
 *
 * Retrieves boolean configuration values for access policy schedule settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to a boolean where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
Schedule_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get Schedule integer parameter values.
 *
 * Retrieves integer configuration values for schedule settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to an integer where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
Schedule_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get Schedule unsigned long parameter values.
 *
 * Retrieves unsigned long configuration values for schedule settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to an unsigned long where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
Schedule_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get Schedule string parameter values.
 *
 * Retrieves string configuration values for schedule settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
Schedule_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set Schedule boolean parameter values.
 *
 * Updates boolean configuration values for schedule settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
Schedule_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set Schedule integer parameter values.
 *
 * Updates integer configuration values for schedule settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
Schedule_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set Schedule unsigned long parameter values.
 *
 * Updates unsigned long configuration values for schedule settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
Schedule_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set Schedule string parameter values.
 *
 * Updates string configuration values for schedule settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
Schedule_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Schedule parameter changes.
 *
 * Validates all staged schedule parameter changes for consistency and correctness before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Pointer to buffer(128 bytes) for parameter name, if there's validation.
 * @param[in,out] puLength Pointer to buffer length for parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Schedule_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Schedule parameter changes.
 *
 * Applies all validated schedule parameter changes to the active configuration and persists settings.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if commit fails.
 */
ULONG
Schedule_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback Schedule parameter changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if rollback fails.
 */
ULONG
Schedule_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.BlockedURL.{i}.

    *  BlockedURL_GetEntryCount
    *  BlockedURL_GetEntry
    *  BlockedURL_GetParamBoolValue
    *  BlockedURL_GetParamIntValue
    *  BlockedURL_GetParamUlongValue
    *  BlockedURL_GetParamStringValue
    *  BlockedURL_SetParamBoolValue
    *  BlockedURL_SetParamIntValue
    *  BlockedURL_SetParamUlongValue
    *  BlockedURL_SetParamStringValue
    *  BlockedURL_Validate
    *  BlockedURL_Commit
    *  BlockedURL_Rollback

***********************************************************************/
/**
 * @brief Get the count of BlockedURL entries.
 *
 * Retrieves the total number of blocked URL entries configured for an access policy.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of BlockedURL entries.
 */
ULONG
BlockedURL_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific BlockedURL entry by index.
 *
 * Retrieves a blocked URL entry from the list by its zero-based index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Pointer to return the instance number of the entry.
 *
 * @return Handle to the BlockedURL entry.
 * @retval Non-NULL Handle to the BlockedURL entry.
 * @retval NULL If entry does not exist at the specified index.
 */
ANSC_HANDLE
BlockedURL_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new BlockedURL entry.
 *
 * Creates and adds a new blocked URL entry to the access policy configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Pointer to return the instance number of the new entry.
 *
 * @return Handle to the newly created BlockedURL entry.
 * @retval Non-NULL Handle to the newly created BlockedURL entry.
 * @retval NULL If memory allocation fails or entry creation fails.
 */
ANSC_HANDLE
BlockedURL_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a BlockedURL entry.
 *
 * Removes a blocked URL entry from the access policy configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if deletion fails.
 */
ULONG
BlockedURL_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get BlockedURL boolean parameter values.
 *
 * Retrieves boolean configuration values for blocked URL settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to a boolean where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
BlockedURL_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get BlockedURL integer parameter values.
 *
 * Retrieves integer configuration values for blocked URL settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to an integer where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
BlockedURL_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get BlockedURL unsigned long parameter values.
 *
 * Retrieves unsigned long configuration values for blocked URL settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to an unsigned long where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
BlockedURL_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get BlockedURL string parameter values.
 *
 * Retrieves string configuration values for blocked URL settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
BlockedURL_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set BlockedURL boolean parameter values.
 *
 * Updates boolean configuration values for blocked URL settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedURL_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set BlockedURL integer parameter values.
 *
 * Updates integer configuration values for blocked URL settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedURL_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set BlockedURL unsigned long parameter values.
 *
 * Updates unsigned long configuration values for blocked URL settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedURL_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set BlockedURL string parameter values.
 *
 * Updates string configuration values for blocked URL settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedURL_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate BlockedURL parameter changes.
 *
 * Validates all staged blocked URL parameter changes for consistency and correctness before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Pointer to buffer for parameter name, if there's validation(128 bytes).
 * @param[in,out] puLength Pointer to buffer length for parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
BlockedURL_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit BlockedURL parameter changes.
 *
 * Applies all validated blocked URL parameter changes to the active configuration and persists settings.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS The operation was successful.
 * @retval Error code if commit fails.
 */
ULONG
BlockedURL_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback BlockedURL parameter changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if rollback fails.
 */
ULONG
BlockedURL_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.BlockedKeyword.{i}.

    *  BlockedKeyword_GetEntryCount
    *  BlockedKeyword_GetEntry
    *  BlockedKeyword_GetParamBoolValue
    *  BlockedKeyword_GetParamIntValue
    *  BlockedKeyword_GetParamUlongValue
    *  BlockedKeyword_GetParamStringValue
    *  BlockedKeyword_SetParamBoolValue
    *  BlockedKeyword_SetParamIntValue
    *  BlockedKeyword_SetParamUlongValue
    *  BlockedKeyword_SetParamStringValue
    *  BlockedKeyword_Validate
    *  BlockedKeyword_Commit
    *  BlockedKeyword_Rollback

***********************************************************************/
/**
 * @brief Get the count of BlockedKeyword entries.
 *
 * Retrieves the total number of blocked keyword entries configured for an access policy.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of BlockedKeyword entries.
 */
ULONG
BlockedKeyword_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific BlockedKeyword entry by index.
 *
 * Retrieves a blocked keyword entry from the list by its zero-based index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Pointer to return the instance number of the entry.
 *
 * @return Handle to the BlockedKeyword entry.
 * @retval Non-NULL Handle to the BlockedKeyword entry.
 * @retval NULL If entry does not exist at the specified index.
 */
ANSC_HANDLE
BlockedKeyword_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new BlockedKeyword entry.
 *
 * Creates and adds a new blocked keyword entry to the access policy configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Pointer to return the instance number of the new entry.
 *
 * @return Handle to the newly created BlockedKeyword entry.
 * @retval Non-NULL Handle to the newly created BlockedKeyword entry.
 * @retval NULL If memory allocation fails or entry creation fails.
 */
ANSC_HANDLE
BlockedKeyword_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a BlockedKeyword entry.
 *
 * Removes a blocked keyword entry from the access policy configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if deletion fails.
 */
ULONG
BlockedKeyword_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get BlockedKeyword boolean parameter values.
 *
 * Retrieves boolean configuration values for blocked keyword settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to a boolean where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
BlockedKeyword_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get BlockedKeyword integer parameter values.
 *
 * Retrieves integer configuration values for blocked keyword settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to an integer where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
BlockedKeyword_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get BlockedKeyword unsigned long parameter values.
 *
 * Retrieves unsigned long configuration values for blocked keyword settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to an unsigned long where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
BlockedKeyword_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get BlockedKeyword string parameter values.
 *
 * Retrieves string configuration values for blocked keyword settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
BlockedKeyword_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set BlockedKeyword boolean parameter values.
 *
 * Updates boolean configuration values for blocked keyword settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedKeyword_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set BlockedKeyword integer parameter values.
 *
 * Updates integer configuration values for blocked keyword settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedKeyword_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set BlockedKeyword unsigned long parameter values.
 *
 * Updates unsigned long configuration values for blocked keyword settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedKeyword_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set BlockedKeyword string parameter values.
 *
 * Updates string configuration values for blocked keyword settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedKeyword_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate BlockedKeyword parameter changes.
 *
 * Validates all staged blocked keyword parameter changes for consistency and correctness before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Pointer to buffer for parameter name, if there's validation.
 * @param[in,out] puLength Pointer to buffer length for parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
BlockedKeyword_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit BlockedKeyword parameter changes.
 *
 * Applies all validated blocked keyword parameter changes to the active configuration and persists settings.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS The operation was successful.
 * @retval Error code if commit fails.
 */
ULONG
BlockedKeyword_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback BlockedKeyword parameter changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if rollback fails.
 */
ULONG
BlockedKeyword_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.BlockedApplication.{i}.

    *  BlockedApplication_GetEntryCount
    *  BlockedApplication_GetEntry
    *  BlockedApplication_GetParamBoolValue
    *  BlockedApplication_GetParamIntValue
    *  BlockedApplication_GetParamUlongValue
    *  BlockedApplication_GetParamStringValue
    *  BlockedApplication_SetParamBoolValue
    *  BlockedApplication_SetParamIntValue
    *  BlockedApplication_SetParamUlongValue
    *  BlockedApplication_SetParamStringValue
    *  BlockedApplication_Validate
    *  BlockedApplication_Commit
    *  BlockedApplication_Rollback

***********************************************************************/
/**
 * @brief Get the count of BlockedApplication entries.
 *
 * Retrieves the total number of blocked application entries configured for an access policy.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of BlockedApplication entries.
 */
ULONG
BlockedApplication_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific BlockedApplication entry by index.
 *
 * Retrieves a blocked application entry from the list by its zero-based index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Pointer to return the instance number of the entry.
 *
 * @return Handle to the BlockedApplication entry.
 * @retval Non-NULL Handle to the BlockedApplication entry.
 * @retval NULL If entry does not exist at the specified index.
 */
ANSC_HANDLE
BlockedApplication_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new BlockedApplication entry.
 *
 * Creates and adds a new blocked application entry to the access policy configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Pointer to return the instance number of the new entry.
 *
 * @return Handle to the newly created BlockedApplication entry
 * @retval Non-NULL Handle to the newly created BlockedApplication entry.
 * @retval NULL If memory allocation fails or entry creation fails.
 */
ANSC_HANDLE
BlockedApplication_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a BlockedApplication entry.
 *
 * Removes a blocked application entry from the access policy configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if deletion fails.
 */
ULONG
BlockedApplication_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get BlockedApplication boolean parameter values.
 *
 * Retrieves boolean configuration values for blocked application settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to a boolean where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
BlockedApplication_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get BlockedApplication integer parameter values.
 *
 * Retrieves integer configuration values for blocked application settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to an integer where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
BlockedApplication_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get BlockedApplication unsigned long parameter values.
 *
 * Retrieves unsigned long configuration values for blocked application settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to an unsigned long where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
BlockedApplication_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get BlockedApplication string parameter values.
 *
 * Retrieves string configuration values for blocked application settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
BlockedApplication_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set BlockedApplication boolean parameter values.
 *
 * Updates boolean configuration values for blocked application settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedApplication_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set BlockedApplication integer parameter values.
 *
 * Updates integer configuration values for blocked application settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedApplication_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set BlockedApplication unsigned long parameter values.
 *
 * Updates unsigned long configuration values for blocked application settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedApplication_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set BlockedApplication string parameter values.
 *
 * Updates string configuration values for blocked application settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
BlockedApplication_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate BlockedApplication parameter changes.
 *
 * Validates all staged blocked application parameter changes for consistency and correctness before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Pointer to buffer(128 bytes) for parameter name, if there's validation.
 * @param[in,out] puLength Pointer to buffer length for parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
BlockedApplication_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit BlockedApplication parameter changes.
 *
 * Applies all validated blocked application parameter changes to the active configuration and persists settings.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS The operation was successful.
 * @retval Error code if commit fails.
 */
ULONG
BlockedApplication_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback BlockedApplication parameter changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if rollback fails.
 */
ULONG
BlockedApplication_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.LogEntry.{i}.

    *  LogEntry_GetEntryCount
    *  LogEntry_GetEntry
    *  LogEntry_IsUpdated
    *  LogEntry_Synchronize
    *  LogEntry_GetParamBoolValue
    *  LogEntry_GetParamIntValue
    *  LogEntry_GetParamUlongValue
    *  LogEntry_GetParamStringValue

***********************************************************************/
/**
 * @brief Get the count of LogEntry entries.
 *
 * Retrieves the total number of internet access log entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of LogEntry entries.
 */
ULONG
LogEntry_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific LogEntry entry by index.
 *
 * Retrieves an internet access log entry from the list by its zero-based index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Pointer to return the instance number of the entry.
 *
 * @return Handle to the LogEntry entry.
 * @retval Non-NULL Handle to the LogEntry entry.
 * @retval NULL If entry does not exist at the specified index.
 */
ANSC_HANDLE
LogEntry_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Check if LogEntry table has been updated.
 *
 * Determines whether the internet access log entries have been modified and need synchronization.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The update status.
 * @retval TRUE If log entries have been updated since last sync.
 * @retval FALSE If no updates have occurred.
 */
BOOL
LogEntry_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronize LogEntry table.
 *
 * Refreshes the internet access log entries from the underlying data source.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval Non-zero error code if synchronization fails.
 */
ULONG
LogEntry_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Get LogEntry boolean parameter values.
 *
 * Retrieves boolean configuration values for log entry attributes.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to a boolean where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
LogEntry_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get LogEntry integer parameter values.
 *
 * Retrieves integer configuration values for log entry attributes.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to an integer where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
LogEntry_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get LogEntry unsigned long parameter values.
 *
 * Retrieves unsigned long configuration values for log entry attributes.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to an unsigned long where the parameter value will be returned.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
LogEntry_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get LogEntry string parameter values.
 *
 * Retrieves string configuration values for log entry attributes.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
LogEntry_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
