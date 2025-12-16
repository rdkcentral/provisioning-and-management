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

    module: cosa_ipv6rd_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        leichen2@cisco.com

    -------------------------------------------------------------------

    revision:

        05/08/2011    initial revision.

**************************************************************************/

#ifndef _COSA_IPV6RD_DML_H_
#define _COSA_IPV6RD_DML_H_

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**
 * @brief Get boolean parameter value from IPv6rd object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Buffer to receive the boolean value.
 *
 * @return status of operation
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
IPv6rd_GetParamBoolValue(
        ANSC_HANDLE hInsContext,
        char *ParamName,
        BOOL *pBool);

/**
 * @brief Get unsigned long parameter value from IPv6rd object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Buffer to receive the unsigned long value.
 *
 * @return status of operation
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
IPv6rd_GetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char *ParamName,
        ULONG *pUlong);

/**
 * @brief Set boolean parameter value for IPv6rd object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv6rd_SetParamBoolValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        BOOL        bValue);

/**
 * @brief Set unsigned long parameter value for IPv6rd object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] ulValue Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv6rd_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue);

/**
 * @brief Validate IPv6rd object parameter values.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer for the name of parameter (minimum 1 byte, maximum 128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation..
 * @retval FALSE if there's validation
 */
BOOL
IPv6rd_Validate(
        ANSC_HANDLE hInsContext,
        char        *pReturnParamName,
        ULONG       *puLength);

/**
 * @brief Commit IPv6rd object parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
IPv6rd_Commit(
        ANSC_HANDLE hInsContext);

/**
 * @brief Rollback IPv6rd object parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation.
 * @retval 0 Rollback successful.
 */
ULONG
IPv6rd_Rollback(
        ANSC_HANDLE hInsContext);

/**
 * @brief Get the count of IPv6rdIF table entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of entries in the IPv6rdIF table.
 */
ULONG
IPv6rdIF_GetEntryCount(
        ANSC_HANDLE hInsContext);

/**
 * @brief Get a specific entry from the IPv6rdIF table.
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
IPv6rdIF_GetEntry(
        ANSC_HANDLE hInsContext,
        ULONG       nIndex,
        ULONG       *pInsNumber);

/**
 * @brief Add a new entry to the IPv6rdIF table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Instance number assigned to the new entry.
 *
 * @return Handle to the new entry context.
 * @retval non-NULL Handle to the new entry context.
 * @retval NULL Addition failed.
 */
ANSC_HANDLE
IPv6rdIF_AddEntry(
        ANSC_HANDLE hInsContext,
        ULONG       *pInsNumber);

/**
 * @brief Delete an entry from the IPv6rdIF table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return Status of operation
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval ANSC_STATUS_FAILURE Deletion failed.
 */
ULONG
IPv6rdIF_DelEntry(
        ANSC_HANDLE hInsContext,
        ULONG       hInstance);

/**
 * @brief Get boolean parameter value from IPv6rdIF entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Buffer to receive the boolean value.
 *
 * @return status of operation
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
IPv6rdIF_GetParamBoolValue(
        ANSC_HANDLE hInsContext,
        char *ParamName,
        BOOL *pBool);

/**
 * @brief Get string parameter value from IPv6rdIF entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to receive the string value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pSize On input: size of pValue buffer. On output: actual length of string.  Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
IPv6rdIF_GetParamStringValue(
        ANSC_HANDLE     hInsContext,
        char            *ParamName,
        char            *pValue,
        ULONG           *pSize);

/**
 * @brief Get unsigned long parameter value from IPv6rdIF entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Buffer to receive the unsigned long value.
 *
 * @return status of operation
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
IPv6rdIF_GetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char *ParamName,
        ULONG *pUlong);

/**
 * @brief Set boolean parameter value for IPv6rdIF entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv6rdIF_SetParamBoolValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        BOOL        bValue);

/**
 * @brief Set string parameter value for IPv6rdIF entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv6rdIF_SetParamStringValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        char        *pString);

/**
 * @brief Set unsigned long parameter value for IPv6rdIF entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv6rdIF_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       bValue);

/**
 * @brief Validate IPv6rdIF entry parameter values.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer for the name of valid parameter (minimum 1 byte, maximum 128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
IPv6rdIF_Validate(
        ANSC_HANDLE hInsContext,
        char        *pReturnParamName,
        ULONG       *puLength);

/**
 * @brief Commit IPv6rdIF entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
IPv6rdIF_Commit(
        ANSC_HANDLE hInsContext);

/**
 * @brief Rollback IPv6rdIF entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Rollback successful.
 * @retval non-zero Rollback failed.
 */
ULONG
IPv6rdIF_Rollback(
        ANSC_HANDLE hInsContext);

#endif /* _COSA_IPV6RD_DML_H_ */
