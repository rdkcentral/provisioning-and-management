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

#ifndef  _COSA_X_COMCAST_COM_PARENTCONTROL_DML_H
#define  _COSA_X_COMCAST_COM_PARENTCONTROL_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_comcast_com_parentalcontrol_internal.h"

/**
 * @brief Retrieves a boolean parameter value from Device.X_COMCAST_COM_ParentalControl.
 *
 * This function retrieves boolean ParentalControl configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
ParentalControl_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets a boolean parameter value for Device.X_COMCAST_COM_ParentalControl.
 *
 * This function sets boolean ParentalControl configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
ParentalControl_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves a boolean parameter value from Device.X_COMCAST_COM_ParentalControl.ManagedSites.
 *
 * This function retrieves boolean ManagedSites configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
MngSites_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets a boolean parameter value for Device.X_COMCAST_COM_ParentalControl.ManagedSites.
 *
 * This function sets boolean ManagedSites configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MngSites_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Validates Device.X_COMCAST_COM_ParentalControl.ManagedSites pending configuration changes.
 *
 * This function validates ManagedSites configuration parameters before committing.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pReturnParamName Buffer to receive parameter, if there's validation.
 * @param[in,out] puLength Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation
 */
BOOL
MngSites_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.X_COMCAST_COM_ParentalControl.ManagedSites configuration changes.
 *
 * This function applies validated ManagedSites configuration changes.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
MngSites_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.X_COMCAST_COM_ParentalControl.ManagedSites configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
MngSites_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a boolean parameter value from Device.X_COMCAST_COM_ParentalControl.ManagedServices.
 *
 * This function retrieves boolean ManagedServices configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
MngServs_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets a boolean parameter value for Device.X_COMCAST_COM_ParentalControl.ManagedServices.
 *
 * This function sets boolean ManagedServices configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MngServs_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Validates Device.X_COMCAST_COM_ParentalControl.ManagedServices pending configuration changes.
 *
 * This function validates ManagedServices configuration parameters before committing.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pReturnParamName Buffer to receive parameter name if there's validation.
 * @param[in,out] puLength Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
MngServs_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.X_COMCAST_COM_ParentalControl.ManagedServices configuration changes.
 *
 * This function applies validated ManagedServices configuration changes.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
MngServs_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.X_COMCAST_COM_ParentalControl.ManagedServices configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
MngServs_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a boolean parameter value from Device.X_COMCAST_COM_ParentalControl.ManagedDevices.
 *
 * This function retrieves boolean ManagedDevices configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
MngDevs_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets a boolean parameter value for Device.X_COMCAST_COM_ParentalControl.ManagedDevices.
 *
 * This function sets boolean ManagedDevices configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MngDevs_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Validates Device.X_COMCAST_COM_ParentalControl.ManagedDevices pending configuration changes.
 *
 * This function validates ManagedDevices configuration parameters before committing.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pReturnParamName Buffer to receive parameter name, if there's validation.
 * @param[in,out] puLength Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
MngDevs_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.X_COMCAST_COM_ParentalControl.ManagedDevices configuration changes.
 *
 * This function applies validated ManagedDevices configuration changes.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
MngDevs_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.X_COMCAST_COM_ParentalControl.ManagedDevices configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
MngDevs_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves the number of entries in Device.X_COMCAST_COM_ParentalControl.ManagedSites.BlockedURL table.
 *
 * This function returns the count of blocked URL entries in the parental control
 * website blocking list.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return Number of BlockedURL entries
 */
ULONG
PcBlkURL_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a BlockedURL entry by index from Device.X_COMCAST_COM_ParentalControl.ManagedSites.BlockedURL.{i}.
 *
 * This function retrieves a blocked URL entry by index and returns its instance handle
 * and instance number.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] nIndex Zero-based index of entry to retrieve
 * @param[out] pInsNumber Pointer to receive instance number
 *
 * @return Instance handle of entry if successful
 * @return Non-NULL Instance handle of entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
PcBlkURL_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new BlockedURL entry to Device.X_COMCAST_COM_ParentalControl.ManagedSites.BlockedURL table.
 *
 * This function creates a new blocked URL entry in the parental control website
 * blocking list.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pInsNumber Pointer to receive assigned instance number
 *
 * @return Instance handle of new entry if successful
 * @retval Non-NULL Instance handle of new entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
PcBlkURL_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a BlockedURL entry from Device.X_COMCAST_COM_ParentalControl.ManagedSites.BlockedURL table.
 *
 * This function removes a blocked URL entry from the parental control website blocking list.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] hInstance Instance handle of entry to delete
 *
 * @return The status of the operation.
 * @retval 0 if deletion successful.
 * @retval error code otherwise.
 */
ULONG
PcBlkURL_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter value from BlockedURL entry.
 *
 * This function retrieves boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
PcBlkURL_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves a string parameter value from BlockedURL entry.
 *
 * This function retrieves string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pValue Buffer to receive parameter value string
 * @param[in,out] pUlSize Pointer to buffer size (in: buffer size, out: actual string length)
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
PcBlkURL_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Retrieves an unsigned long parameter value from BlockedURL entry.
 *
 * This function retrieves unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pUlong Pointer to unsigned long value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
PcBlkURL_GetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char *ParamName,
        ULONG *pUlong);

/**
 * @brief Sets a boolean parameter value for BlockedURL entry.
 *
 * This function sets boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PcBlkURL_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets a string parameter value for BlockedURL entry.
 *
 * This function sets string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] strValue String value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PcBlkURL_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Sets an unsigned long parameter value for BlockedURL entry.
 *
 * This function sets unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] ulValue Unsigned long value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PcBlkURL_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue);

/**
 * @brief Validates BlockedURL entry pending configuration changes.
 *
 * This function validates BlockedURL entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pReturnParamName Buffer(128 bytes) to receive parameter name if there's validation.
 * @param[in,out] puLength Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
PcBlkURL_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits BlockedURL entry configuration changes.
 *
 * This function applies validated BlockedURL entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
PcBlkURL_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back BlockedURL entry configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
PcBlkURL_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves the number of entries in Device.X_COMCAST_COM_ParentalControl.ManagedSites.TrustedUser table.
 *
 * This function returns the count of trusted user entries who are exempt from
 * parental control website blocking.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return Number of TrustedUser entries
 */
ULONG
PcTrustedUser_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a TrustedUser entry by index from Device.X_COMCAST_COM_ParentalControl.ManagedSites.TrustedUser.{i}.
 *
 * This function retrieves a trusted user entry by index and returns its instance
 * handle and instance number.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] nIndex Zero-based index of entry to retrieve
 * @param[out] pInsNumber Pointer to receive instance number
 *
 * @return Instance handle of entry if successful, NULL otherwise
 * @return Non-NULL Instance handle of entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
PcTrustedUser_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new TrustedUser entry to Device.X_COMCAST_COM_ParentalControl.ManagedSites.TrustedUser table.
 *
 * This function creates a new trusted user entry who will be exempt from
 * parental control website blocking.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pInsNumber Pointer to receive assigned instance number
 *
 * @return Instance handle of new entry if successful
 * @retval Non-NULL Instance handle of new entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
PcTrustedUser_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a TrustedUser entry from Device.X_COMCAST_COM_ParentalControl.ManagedSites.TrustedUser table.
 *
 * This function removes a trusted user entry, subjecting the user to parental
 * control website blocking.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] hInstance Instance handle of entry to delete
 *
 * @return The status of the operation.
 * @retval 0 if deletion successful.
 * @retval error code otherwise.
 */
ULONG
PcTrustedUser_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter value from TrustedUser entry.
 *
 * This function retrieves boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
PcTrustedUser_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves a string parameter value from TrustedUser entry.
 *
 * This function retrieves string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pValue Buffer to receive parameter value string
 * @param[in,out] pUlSize Pointer to buffer size (in: buffer size, out: actual string length)
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
PcTrustedUser_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter value for TrustedUser entry.
 *
 * This function sets boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PcTrustedUser_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets a string parameter value for TrustedUser entry.
 *
 * This function sets string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] strValue String value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PcTrustedUser_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Retrieves an unsigned long parameter value from TrustedUser entry.
 *
 * This function retrieves unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pUlong Pointer to unsigned long value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
PcTrustedUser_GetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char *ParamName,
        ULONG *pUlong);

/**
 * @brief Sets an unsigned long parameter value for TrustedUser entry.
 *
 * This function sets unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] ulValue Unsigned long value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
PcTrustedUser_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        );
/**
 * @brief Validates TrustedUser entry pending configuration changes.
 *
 * This function validates TrustedUser entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pReturnParamName Buffer to receive parameter name, if there's validation.
 * @param[in,out] puLength Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
PcTrustedUser_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits TrustedUser entry configuration changes.
 *
 * This function applies validated TrustedUser entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
PcTrustedUser_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back TrustedUser entry configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
PcTrustedUser_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves the number of entries in Device.X_COMCAST_COM_ParentalControl.ManagedServices.Service table.
 *
 * This function returns the count of managed service entries for service/application blocking.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return Number of Service entries
 */
ULONG
MSServ_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a Service entry by index from Device.X_COMCAST_COM_ParentalControl.ManagedServices.Service.{i}.
 *
 * This function retrieves a managed service entry by index and returns its instance
 * handle and instance number.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] nIndex Zero-based index of entry to retrieve
 * @param[out] pInsNumber Pointer to receive instance number
 *
 * @return Instance handle of entry if successful, NULL otherwise
 * @return Non-NULL Instance handle of entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
MSServ_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new Service entry to Device.X_COMCAST_COM_ParentalControl.ManagedServices.Service table.
 *
 * This function creates a new managed service entry for service/application blocking.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pInsNumber Pointer to receive assigned instance number
 *
 * @return Instance handle of new entry if successful, NULL otherwise
 * @retval Non-NULL Instance handle of new entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
MSServ_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a Service entry from Device.X_COMCAST_COM_ParentalControl.ManagedServices.Service table.
 *
 * This function removes a managed service entry from the service/application blocking list.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] hInstance Instance handle of entry to delete
 *
 * @return The status of the operation.
 * @retval 0 if deletion successful.
 * @retval error code otherwise.
 */
ULONG
MSServ_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter value from Service entry.
 *
 * This function retrieves boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
MSServ_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves a string parameter value from Service entry.
 *
 * This function retrieves string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pValue Buffer to receive parameter value string
 * @param[in,out] pUlSize Pointer to buffer size (in: buffer size, out: actual string length)
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
MSServ_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Retrieves an unsigned long parameter value from Service entry.
 *
 * This function retrieves unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pUlong Pointer to unsigned long value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
MSServ_GetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char *ParamName,
        ULONG *pUlong);

/**
 * @brief Sets a boolean parameter value for Service entry.
 *
 * This function sets boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MSServ_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets a string parameter value for Service entry.
 *
 * This function sets string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] strValue String value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MSServ_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Sets an unsigned long parameter value for Service entry.
 *
 * This function sets unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] ulValue Unsigned long value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MSServ_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        );

/**
 * @brief Validates Service entry pending configuration changes.
 *
 * This function validates Service entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pReturnParamName Buffer to receive parameter name, if there's validation.
 * @param[in,out] puLength Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
MSServ_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Service entry configuration changes.
 *
 * This function applies validated Service entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
MSServ_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Service entry configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
MSServ_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves the number of entries in Device.X_COMCAST_COM_ParentalControl.ManagedServices.TrustedUser table.
 *
 * This function returns the count of trusted user entries who are exempt from
 * parental control service/application blocking.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return Number of TrustedUser entries
 */
ULONG
MSTrustedUser_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a TrustedUser entry by index from Device.X_COMCAST_COM_ParentalControl.ManagedServices.TrustedUser.{i}.
 *
 * This function retrieves a trusted user entry.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] nIndex Zero-based index of entry to retrieve
 * @param[out] pInsNumber Pointer to receive instance number
 *
 * @return Instance handle of entry if successful
 * @return Non-NULL Instance handle of entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
MSTrustedUser_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new TrustedUser entry to Device.X_COMCAST_COM_ParentalControl.ManagedServices.TrustedUser table.
 *
 * This function creates a new trusted user entry.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pInsNumber Pointer to receive assigned instance number
 *
 * @return Instance handle of new entry if successful
 * @retval Non-NULL Instance handle of new entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
MSTrustedUser_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a TrustedUser entry from Device.X_COMCAST_COM_ParentalControl.ManagedServices.TrustedUser table.
 *
 * This function removes a trusted user entry.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] hInstance Instance handle of entry to delete
 *
 * @return The status of the operation.
 * @retval 0 if deletion successful.
 * @retval error code otherwise.
 */
ULONG
MSTrustedUser_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter value from ManagedServices TrustedUser entry.
 *
 * This function retrieves boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
MSTrustedUser_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves a string parameter value from ManagedServices TrustedUser entry.
 *
 * This function retrieves string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pValue Buffer to receive parameter value string
 * @param[in,out] pUlSize Pointer to buffer size (in: buffer size, out: actual string length)
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
MSTrustedUser_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Retrieves an unsigned long parameter value from ManagedServices TrustedUser entry.
 *
 * This function retrieves unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pUlong Pointer to unsigned long value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
MSTrustedUser_GetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char *ParamName,
        ULONG *pUlong);

/**
 * @brief Sets a boolean parameter value for ManagedServices TrustedUser entry.
 *
 * This function sets boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MSTrustedUser_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets a string parameter value for ManagedServices TrustedUser entry.
 *
 * This function sets string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] strValue String value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MSTrustedUser_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Sets an unsigned long parameter value for ManagedServices TrustedUser entry.
 *
 * This function sets unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] ulValue Unsigned long value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MSTrustedUser_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        );

/**
 * @brief Validates ManagedServices TrustedUser entry pending configuration changes.
 *
 * This function validates TrustedUser entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pReturnParamName Buffer to receive parameter name, if there's validation.
 * @param[in,out] puLength Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
MSTrustedUser_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits ManagedServices TrustedUser entry configuration changes.
 *
 * This function applies validated TrustedUser entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
MSTrustedUser_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back ManagedServices TrustedUser entry configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
MSTrustedUser_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves the number of entries in Device.X_COMCAST_COM_ParentalControl.ManagedDevices.Device table.
 *
 * This function returns the count of managed device entries for device-based parental control.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return Number of Device entries
 */
ULONG
MDDev_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a Device entry by index from Device.X_COMCAST_COM_ParentalControl.ManagedDevices.Device.{i}.
 *
 * This function retrieves a managed device entry by index and returns its instance
 * handle and instance number.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] nIndex Zero-based index of entry to retrieve
 * @param[out] pInsNumber Pointer to receive instance number
 *
 * @return Instance handle of entry if successful, NULL otherwise
 * @return Non-NULL Instance handle of entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
MDDev_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new Device entry to Device.X_COMCAST_COM_ParentalControl.ManagedDevices.Device table.
 *
 * This function creates a new managed device entry for device-based parental control and scheduling.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pInsNumber Pointer to receive assigned instance number
 *
 * @return Instance handle of new entry if successful
 * @retval Non-NULL Instance handle of new entry if successful.
 * @retval NULL otherwise.
 */
ANSC_HANDLE
MDDev_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a Device entry from Device.X_COMCAST_COM_ParentalControl.ManagedDevices.Device table.
 *
 * This function removes a managed device entry from device-based parental control.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] hInstance Instance handle of entry to delete
 *
 * @return The status of the operation.
 * @retval 0 if deletion successful.
 * @retval error code otherwise.
 */
ULONG
MDDev_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter value from ManagedDevices Device entry.
 *
 * This function retrieves boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
MDDev_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves a string parameter value from ManagedDevices Device entry.
 *
 * This function retrieves string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pValue Buffer to receive parameter value string
 * @param[in,out] pUlSize Pointer to buffer size (in: buffer size, out: actual string length)
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
MDDev_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Retrieves an unsigned long parameter value from ManagedDevices Device entry.
 *
 * This function retrieves unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pUlong Pointer to unsigned long value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
MDDev_GetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char *ParamName,
        ULONG *pUlong);

/**
 * @brief Sets a boolean parameter value for ManagedDevices Device entry.
 *
 * This function sets boolean parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MDDev_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets a string parameter value for ManagedDevices Device entry.
 *
 * This function sets string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] strValue String value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MDDev_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Sets an unsigned long parameter value for ManagedDevices Device entry.
 *
 * This function sets unsigned long parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] ulValue Unsigned long value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MDDev_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        );

/**
 * @brief Validates ManagedDevices Device entry pending configuration changes.
 *
 * This function validates Device entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pReturnParamName Buffer to receive parameter name, if there's validation.
 * @param[in,out] puLength Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
MDDev_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits ManagedDevices Device entry configuration changes.
 *
 * This function applies validated Device entry configuration.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
MDDev_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back ManagedDevices Device entry configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
MDDev_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a string parameter value from Device.X_COMCAST_COM_ParentalControl.ManagedDevices.Redirect.
 *
 * This function retrieves string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pValue Buffer to receive parameter value string
 * @param[in,out] pUlSize Pointer to buffer size (in: buffer size, out: actual string length)
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
MDRed_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );


/**
 * @brief Sets a string parameter value for Device.X_COMCAST_COM_ParentalControl.ManagedDevices.Redirect.
 *
 * This function sets string parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] strValue String value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
MDRed_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates ManagedDevices Redirect pending configuration changes.
 *
 * This function validates Redirect configuration.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pReturnParamName Buffer to receive parameter name, if there's validation.
 * @param[in,out] puLength Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
MDRed_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits ManagedDevices Redirect configuration changes.
 *
 * This function applies validated Redirect configuration.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
MDRed_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back ManagedDevices Redirect configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
MDRed_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


#endif
