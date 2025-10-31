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

    module: cosa_firewall_dml.h

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


#ifndef  _COSA_FIREWALL_DML_H
#define  _COSA_FIREWALL_DML_H

/***********************************************************************

 APIs for Object:

    Firewall.

    *  Firewall_GetParamBoolValue
    *  Firewall_GetParamIntValue
    *  Firewall_GetParamUlongValue
    *  Firewall_GetParamStringValue
    *  Firewall_SetParamBoolValue
    *  Firewall_SetParamIntValue
    *  Firewall_SetParamUlongValue
    *  Firewall_SetParamStringValue
    *  Firewall_Validate
    *  Firewall_Commit
    *  Firewall_Rollback

***********************************************************************/
/**
 * @brief Get boolean parameter value from Firewall object.
 *
 * Retrieves boolean configuration parameters for the Firewall object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Firewall_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from Firewall object.
 *
 * Retrieves integer configuration parameters for the Firewall object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Firewall_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from Firewall object.
 *
 * Retrieves unsigned long configuration parameters for the Firewall object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Firewall_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from Firewall object.
 *
 * Retrieves string configuration parameters for the Firewall object, such as Version and LastChange.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 Buffer size too small.
 * @retval -1 Not supported.
 */
ULONG
Firewall_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for Firewall object.
 *
 * Sets boolean configuration parameters for the Firewall object.
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
Firewall_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for Firewall object.
 *
 * Sets integer configuration parameters for the Firewall object.
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
Firewall_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for Firewall object.
 *
 * Sets unsigned long configuration parameters for the Firewall object.
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
Firewall_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for Firewall object.
 *
 * Sets string configuration parameters for the Firewall object.
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
Firewall_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Firewall object parameter values.
 *
 * Validates staged parameter changes for the Firewall object before committing.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer for the name of parameter if there's a validation (minimum 1 byte, maximum 128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name. Usually size of 1023 will be used.

 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Firewall_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Firewall object parameter changes.
 *
 * Applies validated parameter changes to the Firewall object configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
Firewall_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback Firewall object parameter changes.
 *
 * Reverts uncommitted parameter changes for the Firewall object.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
Firewall_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Get boolean parameter value from Firewall IPv4 configuration.
 *
 * Retrieves boolean configuration parameters for IPv4 firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
V4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set boolean parameter value for Firewall IPv4 configuration.
 *
 * Sets boolean configuration parameters for IPv4 firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bBool Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
V4_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bBool
    );

/**
 * @brief Validate Firewall IPv4 parameter values.
 *
 * Validates staged parameter changes for IPv4 firewall configuration before committing.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer for the name of parameter if there's a validation(minimum 1 byte, maximum 128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
V4_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Firewall IPv4 parameter changes.
 *
 * Applies validated parameter changes to the IPv4 firewall configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
V4_Commit
   (
       ANSC_HANDLE                  hInsContext
   );

/**
 * @brief Rollback Firewall IPv4 parameter changes.
 *
 * Reverts uncommitted parameter changes for the IPv4 firewall configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
V4_Rollback
   (
       ANSC_HANDLE                  hInsContext
   );

/**
 * @brief Get boolean parameter value from Firewall IPv6 configuration.
 *
 * Retrieves boolean configuration parameters for IPv6 firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
V6_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set boolean parameter value for Firewall IPv6 configuration.
 *
 * Sets boolean configuration parameters for IPv6 firewall settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bBool Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
V6_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bBool
    );

/**
 * @brief Validate Firewall IPv6 parameter values.
 *
 * Validates staged parameter changes for IPv6 firewall configuration before committing.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer for the name of parameter if there's a validation (minimum 1 byte, maximum 128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
V6_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Firewall IPv6 parameter changes.
 *
 * Applies validated parameter changes to the IPv6 firewall configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
V6_Commit
   (
       ANSC_HANDLE                  hInsContext
   );

/**
 * @brief Rollback Firewall IPv6 parameter changes.
 *
 * Reverts uncommitted parameter changes for the IPv6 firewall configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
V6_Rollback
   (
       ANSC_HANDLE                  hInsContext
   );


#endif
