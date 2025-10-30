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

    module: cosa_ethernet_dml.h

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


#ifndef  _COSA_ETHERNET_DML_H
#define  _COSA_ETHERNET_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_ethernet_internal.h"

/***********************************************************************

 APIs for Object:

    Ethernet.

    *  Ethernet_GetParamBoolValue
    *  Ethernet_GetParamIntValue
    *  Ethernet_GetParamUlongValue
    *  Ethernet_GetParamStringValue

***********************************************************************/
/**
 * @brief Get boolean parameter value from Ethernet object.
 *
 * Retrieves boolean configuration parameters for the Ethernet object.
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
Ethernet_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from Ethernet object.
 *
 * Retrieves integer configuration parameters for the Ethernet object.
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
Ethernet_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from Ethernet object.
 *
 * Retrieves unsigned long configuration parameters for the Ethernet object.
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
Ethernet_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from Ethernet object.
 *
 * Retrieves string configuration parameters for the Ethernet object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 1023 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 is used,
 *
 * @return The status of the operation.
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 Buffer size too small.
 * @retval -1 Not supported.
 */
ULONG
Ethernet_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#ifndef FEATURE_RDKB_WAN_MANAGER
/***********************************************************************

 APIs for Object:

    Ethernet.Interface.{i}.

    *  Interface_GetEntryCount
    *  Interface_GetEntry
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
 * @brief Get the count of Ethernet Interface table entries.
 *
 * Retrieves the total number of Ethernet interface entries.
 *
 * @param[in] hInsContext Instance context handle (unused in function signature but implied).
 *
 * @return The number of Ethernet Interface entries.
 */
ULONG
Interface_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific Ethernet Interface table entry.
 *
 * Retrieves a specific Ethernet interface entry by index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Instance number of the entry.
 *
 * @return Handle to the entry context, or NULL if not found.
 * @retval non-NULL Handle to the entry context.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
Interface_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Get boolean parameter from an Ethernet Interface entry.
 *
 * Retrieves boolean parameters from an Ethernet interface entry.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Interface_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from an Ethernet Interface entry.
 *
 * Retrieves integer parameters from an Ethernet interface entry.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Interface_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from an Ethernet Interface entry.
 *
 * Retrieves unsigned long parameters from an Ethernet interface entry.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Interface_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from an Ethernet Interface entry.
 *
 * Retrieves string parameters from an Ethernet interface entry.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 1023 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 Buffer size too small.
 * @retval -1 Not supported.
 */
ULONG
Interface_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter for an Ethernet Interface entry.
 *
 * Sets boolean parameters for an Ethernet interface entry.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Interface_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter for an Ethernet Interface entry.
 *
 * Sets integer parameters for an Ethernet interface entry.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Interface_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter for an Ethernet Interface entry.
 *
 * Sets unsigned long parameters for an Ethernet interface entry, such as MaxBitRate.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Interface_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter for an Ethernet Interface entry.
 *
 * Sets string parameters for an Ethernet interface entry, such as Alias, LowerLayers.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 1023 bytes).
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Interface_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Ethernet Interface entry parameter values.
 *
 * Validates staged parameter changes for an Ethernet interface entry before committing.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 * @param[out] pReturnParamName Buffer for the name of parameter if there's a validation.(minimum 1 byte, maximum 128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
Interface_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Ethernet Interface entry parameter changes.
 *
 * Applies validated parameter changes to the Ethernet interface entry configuration.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
Interface_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback Ethernet Interface entry parameter changes.
 *
 * Reverts uncommitted parameter changes for an Ethernet interface entry.
 *
 * @param[in] hInsContext Instance context handle for the Interface entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
Interface_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Ethernet.Interface.{i}.Stats.

    *  Stats_GetParamBoolValue
    *  Stats_GetParamIntValue
    *  Stats_GetParamUlongValue
    *  Stats_GetParamStringValue

***********************************************************************/
/**
 * @brief Get boolean parameter from Ethernet Interface Stats object.
 *
 * Retrieves boolean statistics parameters for the Ethernet interface.
 *
 * @param[in] hInsContext Instance context handle for the Interface Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Stats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from Ethernet Interface Stats object.
 *
 * Retrieves integer statistics parameters for the Ethernet interface.
 *
 * @param[in] hInsContext Instance context handle for the Interface Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Stats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from Ethernet Interface Stats object.
 *
 * Retrieves unsigned long statistics parameters.
 *
 * @param[in] hInsContext Instance context handle for the Interface Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Stats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from Ethernet Interface Stats object.
 *
 * Retrieves string statistics parameters for the Ethernet interface.
 *
 * @param[in] hInsContext Instance context handle for the Interface Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 1023 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 Buffer size too small.
 * @retval -1 Not supported.
 */
ULONG
Stats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#endif // #ifndef FEATURE_RDKB_WAN_MANAGER
/***********************************************************************

 APIs for Object:

    Ethernet.Link.{i}.

    *  Link_GetEntryCount
    *  Link_GetEntry
    *  Link_AddEntry
    *  Link_DelEntry
    *  Link_GetParamBoolValue
    *  Link_GetParamIntValue
    *  Link_GetParamUlongValue
    *  Link_GetParamStringValue
    *  Link_SetParamBoolValue
    *  Link_SetParamIntValue
    *  Link_SetParamUlongValue
    *  Link_SetParamStringValue
    *  Link_Validate
    *  Link_Commit
    *  Link_Rollback

***********************************************************************/
/**
 * @brief Get the count of Ethernet Link table entries.
 *
 * Retrieves the total number of Ethernet link entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of Ethernet Link entries.
 */
ULONG
Link_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific Ethernet Link table entry.
 *
 * Retrieves a specific Ethernet link entry by index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Instance number of the entry.
 *
 * @return Handle to the entry context, or NULL if not found.
 * @retval non-NULL Handle to the entry context.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
Link_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new Ethernet Link table entry.
 *
 * Creates and adds a new Ethernet link entry to the Link table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Instance number assigned to the new entry.
 *
 * @return Handle to the new entry context, or NULL if addition failed.
 * @retval non-NULL Handle to the new entry context.
 * @retval NULL Addition failed.
 */
ANSC_HANDLE
Link_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete an Ethernet Link table entry.
 *
 * Removes a specific Ethernet link entry from the Link table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval ANSC_STATUS_FAILURE Deletion failed.
 */
ULONG
Link_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter from an Ethernet Link entry.
 *
 * Retrieves boolean parameters from an Ethernet link entry.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Link_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from an Ethernet Link entry.
 *
 * Retrieves integer parameters from an Ethernet link entry.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Link_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from an Ethernet Link entry.
 *
 * Retrieves unsigned long parameters from an Ethernet link entry.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Link_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from an Ethernet Link entry.
 *
 * Retrieves string parameters from an Ethernet link entry.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 1023 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 Buffer size too small.
 * @retval -1 Not supported.
 */
ULONG
Link_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter for an Ethernet Link entry.
 *
 * Sets boolean parameters for an Ethernet link entry.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Link_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter for an Ethernet Link entry.
 *
 * Sets integer parameters for an Ethernet link entry.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Link_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter for an Ethernet Link entry.
 *
 * Sets unsigned long parameters for an Ethernet link entry.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Link_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter for an Ethernet Link entry.
 *
 * Sets string parameters for an Ethernet link entry.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 1023 bytes).
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Link_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Ethernet Link entry parameter values.
 *
 * Validates staged parameter changes for an Ethernet link entry before committing.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 * @param[out] pReturnParamName Buffer for the name of parameter if there's a validation(minimum 1 byte, maximum 128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
Link_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit Ethernet Link entry parameter changes.
 *
 * Applies validated parameter changes to the Ethernet link entry configuration.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
Link_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback Ethernet Link entry parameter changes.
 *
 * Reverts uncommitted parameter changes for an Ethernet link entry.
 *
 * @param[in] hInsContext Instance context handle for the Link entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
Link_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Ethernet.Link.{i}.Stats.

    *  Stats1_GetParamBoolValue
    *  Stats1_GetParamIntValue
    *  Stats1_GetParamUlongValue
    *  Stats1_GetParamStringValue

***********************************************************************/
/**
 * @brief Get boolean parameter from Ethernet Link Stats object.
 *
 * Retrieves boolean statistics parameters for the Ethernet link.
 *
 * @param[in] hInsContext Instance context handle for the Link Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Stats1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from Ethernet Link Stats object.
 *
 * Retrieves integer statistics parameters for the Ethernet link.
 *
 * @param[in] hInsContext Instance context handle for the Link Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Stats1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from Ethernet Link Stats object.
 *
 * Retrieves unsigned long statistics parameters for the Ethernet link.
 *
 * @param[in] hInsContext Instance context handle for the Link Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
Stats1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from Ethernet Link Stats object.
 *
 * Retrieves string statistics parameters for the Ethernet link.
 *
 * @param[in] hInsContext Instance context handle for the Link Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 1023 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 Buffer size too small.
 * @retval -1 Not supported.
 */
ULONG
Stats1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Ethernet.VLANTermination.{i}.

    *  VLANTermination_GetEntryCount
    *  VLANTermination_GetEntry
    *  VLANTermination_AddEntry
    *  VLANTermination_DelEntry
    *  VLANTermination_GetParamBoolValue
    *  VLANTermination_GetParamIntValue
    *  VLANTermination_GetParamUlongValue
    *  VLANTermination_GetParamStringValue
    *  VLANTermination_SetParamBoolValue
    *  VLANTermination_SetParamIntValue
    *  VLANTermination_SetParamUlongValue
    *  VLANTermination_SetParamStringValue
    *  VLANTermination_Validate
    *  VLANTermination_Commit
    *  VLANTermination_Rollback

***********************************************************************/
/**
 * @brief Get the count of VLANTermination table entries.
 *
 * Retrieves the total number of VLAN termination entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of VLANTermination entries.
 */
ULONG
VLANTermination_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific VLANTermination table entry.
 *
 * Retrieves a specific VLAN termination entry by index.
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
VLANTermination_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new VLANTermination table entry.
 *
 * Creates and adds a new VLAN termination entry to the VLANTermination table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Instance number assigned to the new entry.
 *
 * @return Handle to the new entry context
 * @retval non-NULL Handle to the new entry context.
 * @retval NULL Addition failed.
 */
ANSC_HANDLE
VLANTermination_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a VLANTermination table entry.
 *
 * Removes a specific VLAN termination entry from the VLANTermination table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval ANSC_STATUS_FAILURE Deletion failed.
 */
ULONG
VLANTermination_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get boolean parameter from a VLANTermination entry.
 *
 * Retrieves boolean parameters from a VLAN termination entry, such as Enable.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
VLANTermination_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from a VLANTermination entry.
 *
 * Retrieves integer parameters from a VLAN termination entry.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
VLANTermination_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from a VLANTermination entry.
 *
 * Retrieves unsigned long parameters from a VLAN termination entry.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
VLANTermination_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from a VLANTermination entry.
 *
 * Retrieves string parameters from a VLAN termination entry.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pValue Buffer to store the parameter value (minimum 0 bytes, maximum 1023 bytes).
 * @param[in,out] pUlSize On input: size of pValue buffer. On output: actual length of parameter value. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter found and value retrieved successfully.
 * @retval 1 Buffer size too small.
 * @retval -1 Not supported.
 */
ULONG
VLANTermination_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter for a VLANTermination entry.
 *
 * Sets boolean parameters for a VLAN termination entry, such as Enable.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
VLANTermination_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter for a VLANTermination entry.
 *
 * Sets integer parameters for a VLAN termination entry.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
VLANTermination_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter for a VLANTermination entry.
 *
 * Sets unsigned long parameters for a VLAN termination entry.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
VLANTermination_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter for a VLANTermination entry.
 *
 * Sets string parameters for a VLAN termination entry.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue String value to set (minimum 0 bytes, maximum 1023 bytes).
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
VLANTermination_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate VLANTermination entry parameter values.
 *
 * Validates staged parameter changes for a VLAN termination entry before committing.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 * @param[out] pReturnParamName Buffer for the name of parameter if there's a validation(minimum 1 byte, maximum 128 bytes).
 * @param[in,out] puLength On input: size of pReturnParamName buffer. On output: actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
VLANTermination_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit VLANTermination entry parameter changes.
 *
 * Applies validated parameter changes to the VLAN termination entry configuration.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
VLANTermination_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback VLANTermination entry parameter changes.
 *
 * Reverts uncommitted parameter changes for a VLAN termination entry.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
VLANTermination_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Ethernet.VLANTermination.{i}.Stats.

    *  VLANTermination_Stats_GetParamBoolValue
    *  VLANTermination_Stats_GetParamIntValue
    *  VLANTermination_Stats_GetParamUlongValue
    *  VLANTermination_Stats_GetParamStringValue

***********************************************************************/
/**
 * @brief Get boolean parameter from VLANTermination Stats object.
 *
 * Retrieves boolean statistics parameters for the VLANTermination interface.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool Pointer to boolean value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
VLANTermination_Stats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter from VLANTermination Stats object.
 *
 * Retrieves integer statistics parameters for the VLANTermination interface.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt Pointer to integer value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
VLANTermination_Stats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter from VLANTermination Stats object.
 *
 * Retrieves unsigned long statistics parameters for the VLANTermination interface.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination Stats entry.
 * @param[in] ParamName Name of the parameter (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong Pointer to unsigned long value to be returned.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter found and value retrieved successfully.
 * @retval FALSE Parameter not found or retrieval failed.
 */
BOOL
VLANTermination_Stats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter from VLANTermination Stats object.
 *
 * Retrieves string statistics parameters for the VLANTermination interface.
 *
 * @param[in] hInsContext Instance context handle for the VLANTermination Stats entry.
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
VLANTermination_Stats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Get MAC address for a network interface.
 *
 * Retrieves the MAC address for the specified network interface.
 *
 * @param[in] ifName Name of the network interface (minimum 1 byte, maximum 64 bytes).
 * @param[out] mac Buffer to store the MAC address string (minimum 18 bytes recommended).
 *
 * @return Status of the operation.
 * @retval 0 MAC address retrieved successfully.
 * @retval -1 Failed to retrieve MAC address.
 */
int
_getMac
    (
        char*                      ifName,
        char*                      mac
    );

/**
 * @brief Update static MAC address for Ethernet link.
 *
 * Updates the static MAC address configuration for an Ethernet link entry.
 *
 * @param[in] hContext Context handle.
 * @param[in] pCfg Pointer to Ethernet link configuration structure.
 * @param[in,out] pEntry Pointer to Ethernet link full entry structure to be updated.
 *
 * @return Status of the operation.
 * @retval ANSC_STATUS_SUCCESS Static MAC address updated successfully.
 * @retval ANSC_STATUS_FAILURE Update failed.
 */
ANSC_STATUS
CosaDmlEthLinkUpdateStaticMac
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    );

#endif
