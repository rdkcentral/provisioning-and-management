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

    module: cosa_hosts_dml.h

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


#ifndef  _COSA_HOSTS_DML_H
#define  _COSA_HOSTS_DML_H

#include "cosa_hosts_dml_custom.h"

/***********************************************************************

 APIs for Object:

    Hosts.

    *  Hosts_GetParamBoolValue
    *  Hosts_GetParamIntValue
    *  Hosts_GetParamUlongValue
    *  Hosts_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves a boolean parameter value from Device.Hosts.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise.
 */
BOOL
Hosts_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter value from Device.Hosts.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to receive the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise.
 */
BOOL
Hosts_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter value from Device.Hosts.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to receive the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise.
 */
BOOL
Hosts_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter value from Device.Hosts.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Buffer to receive the string value (typically 256 bytes).
 * @param[in,out] pUlSize Pointer to buffer size; receives actual string length. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if successful.
 * @retval 1 if buffer too small.
 * @retval -1 if not supported.
 */
ULONG
Hosts_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Hosts.Host.{i}.

    *  Host_GetEntryCount
    *  Host_GetEntry
    *  Host_IsUpdated
    *  Host_Synchronize
    *  Host_GetParamBoolValue
    *  Host_GetParamIntValue
    *  Host_GetParamUlongValue
    *  Host_GetParamStringValue
    *  Host_SetParamBoolValue
    *  Host_SetParamIntValue
    *  Host_SetParamUlongValue
    *  Host_SetParamStringValue
    *  Host_Validate
    *  Host_Commit
    *  Host_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of Host entries in the Device.Hosts.Host table.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of Host entries in the table.
 * @retval ULONG The number of entries.
 */
ULONG
Host_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a Host entry by index from the Device.Hosts.Host table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Pointer to receive the instance number of the entry.
 *
 * @return Handle to the Host entry, or NULL if not found.
 * @retval Non-NULL handle to the Host entry.
 * @retval NULL if not found.
 */
ANSC_HANDLE
Host_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the Host table has been updated.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval TRUE if Host table has been updated.
 * @retval FALSE otherwise.
 */
BOOL
Host_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the Host table with current device discovery data.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval error code otherwise.
 */
ULONG
Host_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a boolean parameter value from a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise.
 */
BOOL
Host_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter value from a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to receive the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise.
 */
BOOL
Host_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter value from a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to receive the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise.
 */
BOOL
Host_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter value from a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Buffer to receive the string value (typically 256 bytes).
 * @param[in,out] pUlSize Pointer to buffer size; receives actual string length. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if successful.
 * @retval 1 if buffer too small.
 * @retval -1 if not supported.
 */
ULONG
Host_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter value for a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise.
 */
BOOL
Host_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets an integer parameter value for a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise.
 */
BOOL
Host_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets an unsigned long parameter value for a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise.
 */
BOOL
Host_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets a string parameter value for a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise.
 */
BOOL
Host_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending changes to a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 * @param[out] pReturnParamName Buffer to receive the name of the parameter if there's a validation (typically 128 bytes).
 * @param[in,out] puLength Pointer to buffer size; receives actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Host_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending changes to a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval error code otherwise.
 */
ULONG
Host_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending changes to a Host entry.
 *
 * @param[in] hInsContext Instance context handle to the Host entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval error code otherwise.
 */
ULONG
Host_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
