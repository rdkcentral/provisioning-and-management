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

/**************************************************************************

    module: cosa_lanmanagement_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:


    -------------------------------------------------------------------

    revision:

        05/09/2019    initial revision.

**************************************************************************/

#ifndef  _COSA_LANMANAGEMENT_DML_H
#define  _COSA_LANMANAGEMENT_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_lanmanagement_internal.h"
#include "dmsb_tr181_psm_definitions.h"

/***********************************************************************

 APIs for Object:

    DeviceInfo.

    *  LanManagement_GetParamBoolValue
    *  LanManagement_GetParamStringValue
    *  LanManagement_SetParamBoolValue
    *  LanManagement_SetParamStringValue
    *  LanManagement_Validate
    *  LanManagement_Commit
    *  LanManagement_Rollback

***********************************************************************/

/**
 * @brief Retrieves a boolean parameter value from Device.X_CISCO_COM_MultiLAN.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise
 */
BOOL
LanManagement_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves a string parameter value from Device.X_CISCO_COM_MultiLAN.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Buffer to receive the string value (typically 256 bytes).
 * @param[in,out] pUlSize Pointer to buffer size; receives actual string length. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if successful.
 * @retval 1 if buffer too small.
 * @retval -1 if not supported
 */
ULONG
LanManagement_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter value for Device.X_CISCO_COM_MultiLAN.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise.
 */
BOOL
LanManagement_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets a string parameter value for Device.X_CISCO_COM_MultiLAN.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise
 */
BOOL
LanManagement_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending changes to Device.X_CISCO_COM_MultiLAN.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to receive the name of the parameter, if there's a validation(typically 128 bytes).
 * @param[in,out] puLength Pointer to buffer size; receives actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
LanManagement_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending changes to Device.X_CISCO_COM_MultiLAN.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Error code otherwise.
 */
ULONG
LanManagement_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending changes to Device.X_CISCO_COM_MultiLAN.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Error code otherwise.
 */
ULONG
LanManagement_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
