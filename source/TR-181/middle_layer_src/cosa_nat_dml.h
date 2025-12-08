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

    module: cosa_nat_dml.h

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


#ifndef  _COSA_NAT_DML_H
#define  _COSA_NAT_DML_H



#include "webconfig_framework.h"
#include "cosa_webconfig_api.h"
#include "cosa_nat_webconfig_apis.h"
#include "cosa_nat_apis.h"

/***********************************************************************

 APIs for Object:

    NAT.

    *  NAT_GetParamBoolValue
    *  NAT_GetParamIntValue
    *  NAT_GetParamUlongValue
    *  NAT_GetParamStringValue

***********************************************************************/
/**
 * @brief Get boolean parameter value from NAT object.
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
NAT_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from NAT object.
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
NAT_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from NAT object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter foud and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
NAT_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from NAT object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 1024 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 if short of buffer size
 * @retval -1 if not supported.
 */
ULONG
NAT_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for NAT object.
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
NAT_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    NAT.X_RDK_PortMapping.

    *  X_RDK_PortMapping_GetParamStringValue
    *  X_RDK_PortMapping_SetParamStringValue

***********************************************************************/
/**
 * @brief Get string parameter value from X_RDK_PortMapping object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 4096 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value.
 *
 * @return status of operation
 * @retval 0  Parameter found and value retrieved successfully.
 * @retval -1  unsupported
 */
ULONG
X_RDK_PortMapping_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set string parameter value for X_RDK_PortMapping object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString String value to set (minimum 0 bytes, maximum 4096 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_RDK_PortMapping_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/***********************************************************************

 APIs for Object:

    NAT.X_CISCO_COM_DMZ.

    *  X_CISCO_COM_DMZ_GetParamBoolValue
    *  X_CISCO_COM_DMZ_GetParamStringValue
    *  X_CISCO_COM_DMZ_SetParamBoolValue
    *  X_CISCO_COM_DMZ_SetParamStringValue
    *  X_CISCO_COM_DMZ_Validate
    *  X_CISCO_COM_DMZ_Commit
    *  X_CISCO_COM_DMZ_Rollback

***********************************************************************/
/**
 * @brief Get boolean parameter value from X_CISCO_COM_DMZ object.
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
X_CISCO_COM_DMZ_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get string parameter value from X_CISCO_COM_DMZ object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 128 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 if short of buffer size
 * @retval -1 if not supported.
 */
ULONG
X_CISCO_COM_DMZ_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for X_CISCO_COM_DMZ object.
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
X_CISCO_COM_DMZ_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set string parameter value for X_CISCO_COM_DMZ object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString String value to set (minimum 0 bytes, maximum 128 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_DMZ_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Validate X_CISCO_COM_DMZ object parameter values.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer(128 bytes) for the name of parameter if there's a validation.
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
X_CISCO_COM_DMZ_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit X_CISCO_COM_DMZ object parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
X_CISCO_COM_DMZ_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback X_CISCO_COM_DMZ object parameter changes whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation.
 *
 * @retval 0 Rollback successful.
 */
ULONG
X_CISCO_COM_DMZ_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    NAT.PortMapping.{i}.

    *  PortMapping_GetEntryCount
    *  PortMapping_GetEntry
    *  PortMapping_IsUpdated
    *  PortMapping_Synchronize
    *  PortMapping_GetParamBoolValue
    *  PortMapping_GetParamIntValue
    *  PortMapping_GetParamUlongValue
    *  PortMapping_GetParamStringValue
    *  PortMapping_SetParamBoolValue
    *  PortMapping_SetParamIntValue
    *  PortMapping_SetParamUlongValue
    *  PortMapping_SetParamStringValue
    *  PortMapping_Validate
    *  PortMapping_Commit
    *  PortMapping_Rollback

***********************************************************************/
/**
 * @brief Get the number of PortMapping table entries.
 *
 * @param[in] hInsContext Instance context handle (unused in function signature but implied).
 *
 * @return The number of PortMapping entries.
 */
ULONG
PortMapping_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific PortMapping table entry.
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
PortMapping_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Check if PortMapping table has been updated.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return status of operation
 * @retval TRUE Table is updated.
 * @retval FALSE Table is current.
 */
BOOL
PortMapping_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronize PortMapping table.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Status of operation.
 * @retval 0 Synchronization successful.
 * @retval non-zero Synchronization failed.
 */
ULONG
PortMapping_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Add a new PortMapping table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Assigned instance number for the new entry.
 *
 * @return Handle to the new entry
 * @retval non-NULL Valid entry handle.
 * @retval NULL Entry creation failed.
 */
ANSC_HANDLE
PortMapping_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a PortMapping table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return Status of operation
 * @retval 0 Deletion successful.
 * @retval non-zero Deletion failed.
 */
ULONG
PortMapping_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter from a PortMapping table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
PortMapping_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from a PortMapping table entry.

 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
PortMapping_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from a PortMapping table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
PortMapping_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from a PortMapping table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval  1 if short of buffer size
 * @retval -1 if not supported.
 */
ULONG
PortMapping_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter for a PortMapping table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PortMapping_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter for a PortMapping table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PortMapping_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter for a PortMapping table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PortMapping_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter for a PortMapping table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PortMapping_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate PortMapping table entry parameter values.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 * @param[out] pReturnParamName Buffer(128 bytes) for the name of parameter if there's a validation.
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
PortMapping_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit PortMapping table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
PortMapping_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback PortMapping table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the PortMapping entry.
 *
 * @return Status of operation
 * @retval 0 Rollback successful.
 */
ULONG
PortMapping_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    NAT.X_CISCO_COM_PortTriggers.Trigger.{i}.

    *  PortTrigger_GetEntryCount
    *  PortTrigger_GetEntry
    *  PortTrigger_AddEntry
    *  PortTrigger_DelEntry
    *  PortTrigger_GetParamBoolValue
    *  PortTrigger_GetParamUlongValue
    *  PortTrigger_GetParamStringValue
    *  PortTrigger_SetParamBoolValue
    *  PortTrigger_SetParamUlongValue
    *  PortTrigger_SetParamStringValue
    *  PortTrigger_Validate
    *  PortTrigger_Commit
    *  PortTrigger_Rollback

***********************************************************************/
/**
 * @brief Get the number of PortTrigger table entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of PortTrigger entries.
 */
ULONG
PortTrigger_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Get a specific PortTrigger table entry by the index.
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
PortTrigger_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new PortTrigger table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Assigned instance number for the new entry.
 *
 * @return Handle to the new entry
 * @retval non-NULL Valid entry handle.
 * @retval NULL Entry creation failed.
 */
ANSC_HANDLE
PortTrigger_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a PortTrigger table entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return Status of operation
 * @retval 0 Deletion successful.
 * @retval non-zero Deletion failed.
 */
ULONG
PortTrigger_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter from a PortTrigger table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortTrigger entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
PortTrigger_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get unsigned long parameter from a PortTrigger table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortTrigger entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] puLong Pointer to unsigned long value to be returned.
 *
 * @return status of operation
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
PortTrigger_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief Get string parameter from a PortTrigger table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortTrigger entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 256 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 if short of buffer size
 * @retval -1 if not supported.
 */
ULONG
PortTrigger_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter for a PortTrigger table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortTrigger entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PortTrigger_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set unsigned long parameter for a PortTrigger table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortTrigger entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValue Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PortTrigger_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

/**
 * @brief Set string parameter for a PortTrigger table entry.
 *
 * @param[in] hInsContext Instance context handle for the PortTrigger entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PortTrigger_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Validate PortTrigger table entry parameter values.
 *
 * @param[in] hInsContext Instance context handle for the PortTrigger entry.
 * @param[out] pReturnParamName Buffer (128 bytes) for the name of parameter if there's a validation.
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
PortTrigger_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit PortTrigger table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the PortTrigger entry.
 *
 * @return Status of operation
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
PortTrigger_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback PortTrigger table entry parameter changes.
 *
 * @param[in] hInsContext Instance context handle for the PortTrigger entry.

 * @return Status of operation
 * @retval 0 Rollback successful.
 */
ULONG
PortTrigger_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Get boolean parameter value from NatPortTrigger object.
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
NatPortTrigger_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set boolean parameter value for NatPortTrigger object.
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
NatPortTrigger_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );


#endif
