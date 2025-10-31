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

    module: cosa_x_cisco_com_devicecontrol_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        07/15/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_X_CISCO_COM_DEVICECONTROL_DML_H
#define  _COSA_X_CISCO_COM_DEVICECONTROL_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_devicecontrol_internal.h"

/***********************************************************************

 APIs for Object:

    DeviceInfo.

    *  X_CISCO_COM_DeviceControl_GetParamBoolValue
    *  X_CISCO_COM_DeviceControl_GetParamIntValue
    *  X_CISCO_COM_DeviceControl_GetParamUlongValue
    *  X_CISCO_COM_DeviceControl_GetParamStringValue
    *  X_CISCO_COM_DeviceControl_SetParamBoolValue
    *  X_CISCO_COM_DeviceControl_SetParamIntValue
    *  X_CISCO_COM_DeviceControl_SetParamUlongValue
    *  X_CISCO_COM_DeviceControl_SetParamStringValue
    *  X_CISCO_COM_DeviceControl_Validate
    *  X_CISCO_COM_DeviceControl_Commit
    *  X_CISCO_COM_DeviceControl_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values from Device.X_CISCO_COM_DeviceControl.
 *
 * This function gets boolean parameter values for Cisco device control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pBool Pointer to store boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
X_CISCO_COM_DeviceControl_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values from Device.X_CISCO_COM_DeviceControl.
 *
 * This function gets integer parameter values for Cisco device control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pInt Pointer to store integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
X_CISCO_COM_DeviceControl_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values from Device.X_CISCO_COM_DeviceControl.
 *
 * This function gets unsigned long parameter values for Cisco device control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pUlong Pointer to store unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
X_CISCO_COM_DeviceControl_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values from Device.X_CISCO_COM_DeviceControl.
 *
 * This function gets string parameter values for Cisco device control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pValue Buffer to store string value (buffer size: *pUlSize bytes).
 * @param[in,out] pUlSize Input: buffer size, Output: required/actual size in bytes. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 if not supported.
 */
ULONG
X_CISCO_COM_DeviceControl_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for Device.X_CISCO_COM_DeviceControl.
 *
 * This function sets boolean parameter values for Cisco device control actions.
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_DeviceControl_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Device.X_CISCO_COM_DeviceControl.
 *
 * This function sets integer parameter values for Cisco device control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_DeviceControl_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for Device.X_CISCO_COM_DeviceControl.
 *
 * This function sets unsigned long parameter values for Cisco device control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_DeviceControl_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for Device.X_CISCO_COM_DeviceControl.
 *
 * This function sets string parameter values for Cisco device control settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] strValue String value to set (buffer size: 1024 bytes).
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_DeviceControl_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates Device.X_CISCO_COM_DeviceControl parameter changes before commit.
 *
 * This function validates all pending parameter changes for Cisco device control configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to store parameter name, if there's a validation(buffer size: 128 bytes).
 * @param[out] puLength Pointer to store length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
X_CISCO_COM_DeviceControl_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.X_CISCO_COM_DeviceControl parameter changes to persistent storage.
 *
 * This function applies all validated parameter changes to the Cisco device control configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
X_CISCO_COM_DeviceControl_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.X_CISCO_COM_DeviceControl parameter changes.
 *
 * This function reverts all uncommitted parameter changes for Cisco device control configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
X_CISCO_COM_DeviceControl_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Gets the number of LAN management entries in Device.X_CISCO_COM_DeviceControl.LanManagementEntry table.
 *
 * This function returns the count of configured LAN management entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Number of LAN management entries.
 */
ULONG
LanMngm_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Gets a LAN management entry from Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i} table by index.
 *
 * This function retrieves a LAN management entry handle at the specified index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry.
 * @param[out] pInsNumber Pointer to store instance number of the entry.
 *
 * @return Handle to the entry context, or NULL if not found.
 * @retval non-NULL Handle to the entry context.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
LanMngm_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new LAN management entry to Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i} table.
 *
 * This function creates a new LAN management entry in the table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Pointer to store instance number of newly created entry.
 *
 * @return Handle to the new entry context, or NULL if addition failed.
 * @retval non-NULL Handle to the new entry context.
 * @retval NULL Addition failed.
 */
ANSC_HANDLE
LanMngm_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a LAN management entry from Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i} table.
 *
 * This function removes a LAN management entry from the table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS Entry deleted successfully.
 * @retval error code otherwise.
 */
ULONG
LanMngm_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values from Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i}.
 *
 * This function gets boolean parameter values for a LAN management entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pBool Pointer to store boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
LanMngm_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values from Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i}.
 *
 * This function gets integer parameter values for a LAN management entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pInt Pointer to store integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
LanMngm_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values from Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i}.
 *
 * This function gets unsigned long parameter values for a LAN management entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pUlong Pointer to store unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
LanMngm_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values from Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i}.
 *
 * This function gets string parameter values for a LAN management entry (LanNetwork, LanNetworksAllow, LanIPAddress, LanSubnetMask, LanDhcpStartIp, LanDhcpEndIp).
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pValue Buffer to store string value (buffer size: *pUlSize bytes).
 * @param[in,out] pUlSize Input: buffer size, Output: required/actual size in bytes. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 if not supported.
 */
ULONG
LanMngm_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i}.
 *
 * This function sets boolean parameter values for a LAN management entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
LanMngm_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i}.
 *
 * This function sets integer parameter values for a LAN management entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
LanMngm_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i}.
 *
 * This function sets unsigned long parameter values for a LAN management entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
LanMngm_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i}.
 *
 * This function sets string parameter values for a LAN management entry (LanNetwork, LanNetworksAllow, LanIPAddress, LanSubnetMask, LanDhcpStartIp, LanDhcpEndIp).
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] strValue String value to set (buffer size: 256 bytes).
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
LanMngm_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i} parameter changes before commit.
 *
 * This function validates all pending parameter changes for a LAN management entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to store parameter name if there's a validation(buffer size: 128 bytes).
 * @param[out] puLength Pointer to store length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
LanMngm_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i} parameter changes to persistent storage.
 *
 * This function applies all validated parameter changes to the LAN management entry configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
LanMngm_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i} parameter changes.
 *
 * This function reverts all uncommitted parameter changes for a LAN management entry.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
LanMngm_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#if !defined (RESOURCE_OPTIMIZATION)

/**
 * @brief Retrieves boolean parameter values from Device.X_CISCO_COM_DeviceControl.WebAccessLevel.
 *
 * This function gets boolean parameter values for web access level configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pBool Pointer to store boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
WebAccessLevel_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pBool
    );

/**
 * @brief Retrieves integer parameter values from Device.X_CISCO_COM_DeviceControl.WebAccessLevel.
 *
 * This function gets integer parameter values for web access level configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pInt Pointer to store integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
WebAccessLevel_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values from Device.X_CISCO_COM_DeviceControl.WebAccessLevel.
 *
 * This function gets unsigned long parameter values for web access level configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] puLong Pointer to store unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
WebAccessLevel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief Retrieves string parameter values from Device.X_CISCO_COM_DeviceControl.WebAccessLevel.
 *
 * This function gets string parameter values for web access level configuration (HomeUser, AccessLevel).
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pValue Buffer to store string value (buffer size: *pulSize bytes).
 * @param[in,out] pulSize Input: buffer size, Output: required/actual size in bytes. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
WebAccessLevel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
 * @brief Sets boolean parameter values for Device.X_CISCO_COM_DeviceControl.WebAccessLevel.
 *
 * This function sets boolean parameter values for web access level configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
WebAccessLevel_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Device.X_CISCO_COM_DeviceControl.WebAccessLevel.
 *
 * This function sets integer parameter values for web access level configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] iValue Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
WebAccessLevel_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    );

/**
 * @brief Sets unsigned long parameter values for Device.X_CISCO_COM_DeviceControl.WebAccessLevel.
 *
 * This function sets unsigned long parameter values for web access level configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] uValue Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
WebAccessLevel_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

/**
 * @brief Sets string parameter values for Device.X_CISCO_COM_DeviceControl.WebAccessLevel.
 *
 * This function sets string parameter values for web access level configuration (HomeUser, AccessLevel).
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to set (buffer size: 256 bytes).
 * @param[in] pString String value to set (buffer size: 256 bytes).
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
WebAccessLevel_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Validates Device.X_CISCO_COM_DeviceControl.WebAccessLevel parameter changes before commit.
 *
 * This function validates all pending parameter changes for web access level configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to store parameter name if there's a validation (buffer size: 128 bytes).
 * @param[out] puLength Pointer to store length of failed parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
WebAccessLevel_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.X_CISCO_COM_DeviceControl.WebAccessLevel parameter changes to persistent storage.
 *
 * This function applies all validated parameter changes to the web access level configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
WebAccessLevel_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.X_CISCO_COM_DeviceControl.WebAccessLevel parameter changes.
 *
 * This function reverts all uncommitted parameter changes for web access level configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
WebAccessLevel_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif

#endif
