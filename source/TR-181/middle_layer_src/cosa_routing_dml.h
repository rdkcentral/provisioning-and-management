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

    module: cosa_routing_dml.h

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


#ifndef  _COSA_ROUTING_DML_H
#define  _COSA_ROUTING_DML_H

/***********************************************************************

 APIs for Object:

    Routing.

    *  Routing_GetParamBoolValue
    *  Routing_GetParamIntValue
    *  Routing_GetParamUlongValue
    *  Routing_GetParamStringValue

***********************************************************************/
/**
 * @brief Get boolean parameter value from Routing object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Routing_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from Routing object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Routing_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from Routing object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Routing_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from Routing object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 if short of buffer size.
 * @retval -1 if not supported
 */
ULONG
Routing_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Routing.Router.{i}.

    *  Router_GetEntryCount
    *  Router_GetEntry
    *  Router_AddEntry
    *  Router_DelEntry
    *  Router_GetParamBoolValue
    *  Router_GetParamIntValue
    *  Router_GetParamUlongValue
    *  Router_GetParamStringValue
    *  Router_SetParamBoolValue
    *  Router_SetParamIntValue
    *  Router_SetParamUlongValue
    *  Router_SetParamStringValue
    *  Router_Validate
    *  Router_Commit
    *  Router_Rollback

***********************************************************************/
/**
 * @brief Get the number of Router table entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of Router entries.
 */
ULONG
Router_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific Router table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry.
 * @param[out] pInsNumber Instance number of the entry.
 *
 * @return Handle to the table entry
 * @retval non-NULL Valid entry handle.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
Router_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new Router table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Assigned instance number for the new entry.
 *
 * @return Handle to the new entry
 * @retval non-NULL Valid entry handle.
 * @retval NULL Entry creation failed.
 */
ANSC_HANDLE
Router_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a Router table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return Status of operation
 * @retval 0 Deletion successful.
 * @retval non-zero Deletion failed.
 */
ULONG
Router_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter from a Router table entry.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return Status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Router_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from a Router table entry.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Router_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from a Router table entry.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return Status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Router_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from a Router table entry.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return Status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 if short of buffer size
 * @retval -1 if not supported
 *
 */
ULONG
Router_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter for a Router table entry.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Router_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter for a Router table entry.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Router_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter for a Router table entry.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Router_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter for a Router table entry.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Router_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Router table entry parameter values.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 * @param[out] pReturnParamName Buffer(128 bytes) for the name of parameter if there's a validation.
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Router_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Router table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
Router_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback Router table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the Router entry.
 *
 * @return Status of operation
 * @retval 0 Rollback successful.
 */
ULONG
Router_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.Router.{i}.IPv4Forwarding.{i}.

    *  IPv4Forwarding_GetEntryCount
    *  IPv4Forwarding_GetEntry
    *  IPv4Forwarding_AddEntry
    *  IPv4Forwarding_DelEntry
    *  IPv4Forwarding_GetParamBoolValue
    *  IPv4Forwarding_GetParamIntValue
    *  IPv4Forwarding_GetParamUlongValue
    *  IPv4Forwarding_GetParamStringValue
    *  IPv4Forwarding_SetParamBoolValue
    *  IPv4Forwarding_SetParamIntValue
    *  IPv4Forwarding_SetParamUlongValue
    *  IPv4Forwarding_SetParamStringValue
    *  IPv4Forwarding_Validate
    *  IPv4Forwarding_Commit
    *  IPv4Forwarding_Rollback

***********************************************************************/
/**
 * @brief Get the number of IPv4Forwarding table entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of IPv4Forwarding entries.
 */
ULONG
IPv4Forwarding_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry.
 * @param[out] pInsNumber Instance number of the entry.
 *
 * @return Handle to the table entry
 * @retval non-NULL Valid entry handle.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
IPv4Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Assigned instance number for the new entry.
 *
 * @return Handle to the new entry
 * @retval non-NULL Valid entry handle.
 * @retval NULL Entry creation failed.
 */
ANSC_HANDLE
IPv4Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete an IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return Status of operation
 * @retval 0 Deletion successful.
 * @retval non-zero Deletion failed.
 */
ULONG
IPv4Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter from an IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
IPv4Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from an IPv4Forwarding table entry.

 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
IPv4Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from an IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
IPv4Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from an IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 if short of buffer size
 * @retval -1 if not supported
 */
ULONG
IPv4Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter for an IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv4Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter for an IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv4Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter for an IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv4Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter for an IPv4Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv4Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate IPv4Forwarding table entry parameter values.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 * @param[out] pReturnParamName Buffer(128 bytes) for the name of parameter if there's a validation.
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
IPv4Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit IPv4Forwarding table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
IPv4Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback IPv4Forwarding table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the IPv4Forwarding entry.
 *
 * @return Status of operation
 * @retval 0 Rollback successful.
 */
ULONG
IPv4Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.Router.{i}.IPv6Forwarding.{i}.

    *  IPv6Forwarding_GetEntryCount
    *  IPv6Forwarding_GetEntry
    *  IPv6Forwarding_AddEntry
    *  IPv6Forwarding_DelEntry
    *  IPv6Forwarding_GetParamBoolValue
    *  IPv6Forwarding_GetParamIntValue
    *  IPv6Forwarding_GetParamUlongValue
    *  IPv6Forwarding_GetParamStringValue
    *  IPv6Forwarding_SetParamBoolValue
    *  IPv6Forwarding_SetParamIntValue
    *  IPv6Forwarding_SetParamUlongValue
    *  IPv6Forwarding_SetParamStringValue
    *  IPv6Forwarding_Validate
    *  IPv6Forwarding_Commit
    *  IPv6Forwarding_Rollback

***********************************************************************/
/**
 * @brief Get the number of IPv6Forwarding table entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of IPv6Forwarding entries.
 */
ULONG
IPv6Forwarding_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry.
 * @param[out] pInsNumber Instance number of the entry.
 *
 * @return Handle to the table entry
 * @retval non-NULL Valid entry handle.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
IPv6Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Assigned instance number for the new entry.
 *
 * @return Handle to the new entry
 * @retval non-NULL Valid entry handle.
 * @retval NULL Entry creation failed.
 */
ANSC_HANDLE
IPv6Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete an IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return Status of operation.
 * @retval 0 Deletion successful.
 * @retval non-zero Deletion failed.
 */
ULONG
IPv6Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter from an IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
IPv6Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from an IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
IPv6Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from an IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
IPv6Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from an IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used
 *
 * @return status of operation.
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 if short of buffer size
 * @retval  -1 if not supported
 */
ULONG
IPv6Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter for an IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv6Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter for an IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv6Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter for an IPv6Forwarding table entry..
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv6Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter for an IPv6Forwarding table entry.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
IPv6Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate IPv6Forwarding table entry parameter values.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 * @param[out] pReturnParamName Buffer(128 bytes) for the name of parameter if there's a validation.
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there is no validation.
 * @retval FALSE if there's validation.
 */
BOOL
IPv6Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit IPv6Forwarding table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
IPv6Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback IPv6Forwarding table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the IPv6Forwarding entry.
 *
 * @return Status of operation
 * @retval 0 Rollback successful.
 */
ULONG
IPv6Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.RIP.

    *  RIP_GetParamBoolValue
    *  RIP_GetParamIntValue
    *  RIP_GetParamUlongValue
    *  RIP_GetParamStringValue
    *  RIP_SetParamBoolValue
    *  RIP_SetParamIntValue
    *  RIP_SetParamUlongValue
    *  RIP_SetParamStringValue
    *  RIP_Validate
    *  RIP_Commit
    *  RIP_Rollback

***********************************************************************/
/**
 * @brief Get boolean parameter value from RIP object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
RIP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from RIP object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
RIP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from RIP object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
RIP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from RIP object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return  status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 if short of buffer size
 * @retval -1 if not supported
 */
ULONG
RIP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for RIP object.
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
RIP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for RIP object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RIP_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for RIP object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RIP_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for RIP object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RIP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate RIP object parameter values
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer(128 bytes) for the name of parameter if there is a validation.
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there is no validation.
 * @retval FALSE if there's validation.
 */
BOOL
RIP_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit RIP object parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
RIP_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback RIP object parameter changes whenever there is a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation.
 * @retval 0 Rollback successful.
 */
ULONG
RIP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.RIP.InterfaceSetting.{i}.

    *  InterfaceSetting_GetEntryCount
    *  InterfaceSetting_GetEntry
    *  InterfaceSetting_AddEntry
    *  InterfaceSetting_DelEntry
    *  InterfaceSetting_GetParamBoolValue
    *  InterfaceSetting_GetParamIntValue
    *  InterfaceSetting_GetParamUlongValue
    *  InterfaceSetting_GetParamStringValue
    *  InterfaceSetting_SetParamBoolValue
    *  InterfaceSetting_SetParamIntValue
    *  InterfaceSetting_SetParamUlongValue
    *  InterfaceSetting_SetParamStringValue
    *  InterfaceSetting_Validate
    *  InterfaceSetting_Commit
    *  InterfaceSetting_Rollback

***********************************************************************/
/**
 * @brief Get the count of InterfaceSetting table entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of entries in the InterfaceSetting table.
 */
ULONG
InterfaceSetting_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific entry from the InterfaceSetting table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Instance number of the entry.
 *
 * @return Handle to the entry context.
 * @retval non-NULL Handle to the entry context.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
InterfaceSetting_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new entry to the InterfaceSetting table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Instance number assigned to the new entry.
 *
 * @return Handle to the new entry context.
 * @retval non-NULL Handle to the new entry context.
 * @retval NULL Addition failed.
 */
ANSC_HANDLE
InterfaceSetting_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete an entry from the InterfaceSetting table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return Status of operation
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval ANSC_STATUS_FAILURE Deletion failed.
 */
ULONG
InterfaceSetting_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter value from InterfaceSetting entry.
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
InterfaceSetting_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Buffer to receive the integer value.
 *
 * @return Tstatus of operation
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
InterfaceSetting_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Buffer to receive the unsigned long value.
 *
 * @return status of operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
InterfaceSetting_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to receive the string value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of string. Usually size of 1023 is used.
 *
 * @return status of operation
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 if not supported.
 */
ULONG
InterfaceSetting_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for InterfaceSetting entry.
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
InterfaceSetting_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
InterfaceSetting_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
InterfaceSetting_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for InterfaceSetting entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
InterfaceSetting_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate InterfaceSetting entry parameter values.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer for the name of parameter if there's a validation(minimum 1 byte, maximum 128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE All parameters are valid.
 * @retval FALSE if there's validation
 */
BOOL
InterfaceSetting_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit InterfaceSetting entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
InterfaceSetting_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback InterfaceSetting entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Rollback successful.
 */
ULONG
InterfaceSetting_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#if !defined (RESOURCE_OPTIMIZATION)
/***********************************************************************

 APIs for Object:

    Routing.RouteInformation.

    *  RouteInformation_GetParamBoolValue
    *  RouteInformation_GetParamIntValue
    *  RouteInformation_GetParamUlongValue
    *  RouteInformation_GetParamStringValue
    *  RouteInformation_SetParamBoolValue
    *  RouteInformation_SetParamIntValue
    *  RouteInformation_SetParamUlongValue
    *  RouteInformation_SetParamStringValue
    *  RouteInformation_Validate
    *  RouteInformation_Commit
    *  RouteInformation_Rollback

***********************************************************************/
/**
 * @brief Get boolean parameter value from RouteInformation object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
RouteInformation_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from RouteInformation object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
RouteInformation_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from RouteInformation object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
RouteInformation_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from RouteInformation object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value.Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval if short of buffer size
 * @retval if not supported
 */
ULONG
RouteInformation_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for RouteInformation object.
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
RouteInformation_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for RouteInformation object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RouteInformation_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for RouteInformation object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RouteInformation_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for RouteInformation object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RouteInformation_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate RouteInformation object parameter values.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer(128 bytes) for the name of parameter if there is a validation.
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE All parameters are valid.
 * @retval FALSE if there's validation.
 */
BOOL
RouteInformation_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit RouteInformation object parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
RouteInformation_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback RouteInformation object parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Rollback successful.
 */
ULONG
RouteInformation_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.RouteInformation.InterfaceSetting.{i}.

    *  InterfaceSetting3_GetEntryCount
    *  InterfaceSetting3_GetEntry
    *  InterfaceSetting3_IsUpdated
    *  InterfaceSetting3_Synchronize
    *  InterfaceSetting3_GetParamBoolValue
    *  InterfaceSetting3_GetParamIntValue
    *  InterfaceSetting3_GetParamUlongValue
    *  InterfaceSetting3_GetParamStringValue

***********************************************************************/
/**
 * @brief Get the count of InterfaceSetting3 table entries.
 *
 * @param[in] hInsContext Instance context handle .
 *
 * @return The number of entries in the InterfaceSetting3 table.
 */
ULONG
InterfaceSetting3_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific entry from the InterfaceSetting3 table.
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
InterfaceSetting3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Check if InterfaceSetting3 table has been updated.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return status of operation
 * @retval TRUE Table has been updated .
 * @retval FALSE Table is up to date.
 */
BOOL
InterfaceSetting3_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronize InterfaceSetting3 table entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation.
 * @retval 0 Synchronization successful.
 * @retval non-zero Synchronization failed.
 */
ULONG
InterfaceSetting3_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Get boolean parameter value from InterfaceSetting3 entry.
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
InterfaceSetting3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from InterfaceSetting3 entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Buffer to receive the integer value.
 *
 * @return status of operation
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
InterfaceSetting3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from InterfaceSetting3 entry.
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
InterfaceSetting3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from InterfaceSetting3 entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to receive the string value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
InterfaceSetting3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#endif

/***********************************************************************

 APIs for Object:

    Routing.RouteInformation.

    *  RIPv2_GetParamBoolValue
    *  RIPv2_GetParamIntValue
    *  RIPv2_GetParamUlongValue
    *  RIPv2_GetParamStringValue
    *  RIPv2_SetParamBoolValue
    *  RIPv2_SetParamIntValue
    *  RIPv2_SetParamUlongValue
    *  RIPv2_SetParamStringValue
    *  RIPv2_Validate
    *  RIPv2_Commit
    *  RIPv2_Rollback

***********************************************************************/
/**
 * @brief Get boolean parameter value from RIPv2 object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
RIPv2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from RIPv2 object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
RIPv2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from RIPv2 object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
RIPv2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from RIPv2 object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 is used.
 *
 * @return status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 if Buffer size is insufficient
 * @retval -1 if not supported
 */
ULONG
RIPv2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for RIPv2 object.
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
RIPv2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for RIPv2 object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RIPv2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for RIPv2 object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RIPv2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for RIPv2 object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RIPv2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate RIPv2 object parameter values.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer(128 bytes) for the name of parameter if there is a validation.
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name. Usually the size of 1023 is used.
 *
 * @return status of operation
 * @retval TRUE if there is no validation
 * @retval FALSE if there's validation.
 */
BOOL
RIPv2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit RIPv2 object parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
RIPv2_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback RIPv2 object parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Rollback successful.
 */
ULONG
RIPv2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Check if USG is in bridge mode.
 *
 * @param[out] pBridgeMode Pointer to boolean value indicating bridge mode status.
 *
 * @return Status of the operation.
 * @retval ANSC_STATUS_SUCCESS Bridge mode status retrieved successfully.
 * @retval ANSC_STATUS_FAILURE Failed to retrieve bridge mode status.
 */
ANSC_STATUS
is_usg_in_bridge_mode
    (
        BOOL                        *pBridgeMode
    );

#endif
