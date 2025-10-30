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

    module: cosa_dns_dml.h

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


#ifndef  _COSA_DNS_DML_H
#define  _COSA_DNS_DML_H

/***********************************************************************

 APIs for Object:

    DNS.


***********************************************************************/

/***********************************************************************

 APIs for Object:

    DNS.Client.

    *  Client1_GetParamBoolValue
    *  Client1_GetParamIntValue
    *  Client1_GetParamUlongValue
    *  Client1_GetParamStringValue
    *  Client1_SetParamBoolValue
    *  Client1_SetParamIntValue
    *  Client1_SetParamUlongValue
    *  Client1_SetParamStringValue
    *  Client1_Validate
    *  Client1_Commit
    *  Client1_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values from Device.DNS.Client.
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
Client1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values from Device.DNS.Client.
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
Client1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values from Device.DNS.Client.
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
Client1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values from Device.DNS.Client.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pValue Buffer to store string value (buffer size: *pUlSize bytes).
 * @param[in,out] pUlSize Input: buffer size, Output: required/actual size in bytes. Usually the size of 1023 is used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Not supported
 */
ULONG
Client1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for Device.DNS.Client..
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
Client1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Device.DNS.Client.
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
Client1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for Device.DNS.Client.
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
Client1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for Device.DNS.Client.
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
Client1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates Device.DNS.Client parameter changes before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to store parameter name if there's a validation(buffer size: 128 bytes).
 * @param[out] puLength Pointer to store length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
Client1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.DNS.Client parameter changes to persistent storage.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
Client1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.DNS.Client parameter changes whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
Client1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DNS.Client.Server.{i}.

    *  Server1_GetEntryCount
    *  Server1_GetEntry
    *  Server1_AddEntry
    *  Server1_DelEntry
    *  Server1_IsUpdated
    *  Server1_Synchronize
    *  Server1_GetParamBoolValue
    *  Server1_GetParamIntValue
    *  Server1_GetParamUlongValue
    *  Server1_GetParamStringValue
    *  Server1_SetParamBoolValue
    *  Server1_SetParamIntValue
    *  Server1_SetParamUlongValue
    *  Server1_SetParamStringValue
    *  Server1_Validate
    *  Server1_Commit
    *  Server1_Rollback

***********************************************************************/
/**
 * @brief Gets the number of DNS server entries in Device.DNS.Client.Server table.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Number of DNS server entries.
 */
ULONG
Server1_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Gets a DNS server entry from Device.DNS.Client.Server.{i} table by index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry.
 * @param[out] pInsNumber Pointer to store instance number of the entry.
 *
 * @return Handle to the entry context
 * @retval non-NULL Handle to the entry context.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
Server1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new DNS server entry to Device.DNS.Client.Server.{i} table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Pointer to store instance number of newly created entry.
 *
 * @return Handle to the new entry context
 * @retval non-NULL Handle to the new entry context.
 * @retval NULL Addition failed.
 */
ANSC_HANDLE
Server1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a DNS server entry from Device.DNS.Client.Server.{i} table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 */
ULONG
Server1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Checks if Device.DNS.Client.Server table has been updated.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The update status.
 * @retval TRUE if the entry table needs to be refreshed.
 * @retval FALSE if the entry table is still current.
 */
BOOL
Server1_IsUpdated
    (
        ANSC_HANDLE                  hInsContext
    );

/**
 * @brief Synchronizes Device.DNS.Client.Server table with backend data.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if successful.
 * @retval error code otherwise.
 */
ULONG
Server1_Synchronize
    (
        ANSC_HANDLE                  hInsContext
    );

/**
 * @brief Retrieves boolean parameter values from Device.DNS.Client.Server.{i}.
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
Server1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values from Device.DNS.Client.Server.{i}.
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
Server1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values from Device.DNS.Client.Server.{i}.
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
Server1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values from Device.DNS.Client.Server.{i}.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pValue Buffer to store string value (buffer size: *pUlSize bytes).
 * @param[in,out] pUlSize Input: buffer size, Output: required/actual size in bytes. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Not supported.
 */
ULONG
Server1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for Device.DNS.Client.Server.{i}.
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
Server1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Device.DNS.Client.Server.{i}.
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
Server1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for Device.DNS.Client.Server.{i}.
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
Server1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for Device.DNS.Client.Server.{i}.
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
Server1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates Device.DNS.Client.Server.{i} parameter changes before commit.
 *
 * This function validates all pending parameter changes for a DNS server entry.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to store parameter name if there's a validation(buffer size: 128 bytes).
 * @param[out] puLength Pointer to store length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if failed.
 */
BOOL
Server1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.DNS.Client.Server.{i} parameter changes to persistent storage.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
Server1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.DNS.Client.Server.{i} parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
Server1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    DNS.Relay.

    *  Relay_GetParamBoolValue
    *  Relay_GetParamIntValue
    *  Relay_GetParamUlongValue
    *  Relay_GetParamStringValue
    *  Relay_SetParamBoolValue
    *  Relay_SetParamIntValue
    *  Relay_SetParamUlongValue
    *  Relay_SetParamStringValue
    *  Relay_Validate
    *  Relay_Commit
    *  Relay_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values from Device.DNS.Relay.
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
Relay_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values from Device.DNS.Relay.
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
Relay_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values from Device.DNS.Relay.
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
Relay_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values from Device.DNS.Relay.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pValue Buffer to store string value (buffer size: *pUlSize bytes).
 * @param[in,out] pUlSize Input: buffer size, Output: required/actual size in bytes. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Not supported.
 */
ULONG
Relay_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for Device.DNS.Relay.
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
Relay_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Device.DNS.Relay.
 *
 * This function sets integer parameter values for DNS relay configuration.
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
Relay_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for Device.DNS.Relay.
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
Relay_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for Device.DNS.Relay.
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
Relay_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates Device.DNS.Relay parameter changes before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to store parameter name if there's a validation(buffer size: 128 bytes).
 * @param[out] puLength Pointer to store length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
Relay_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.DNS.Relay parameter changes to persistent storage.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
Relay_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.DNS.Relay parameter changes.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Rollback successful.
 * @retval Non-zero error code otherwise.
 */
ULONG
Relay_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DNS.Relay.Forwarding.{i}.

    *  Forwarding_GetEntryCount
    *  Forwarding_GetEntry
    *  Forwarding_AddEntry
    *  Forwarding_DelEntry
    *  Forwarding_IsUpdated
    *  Forwarding_Synchrize
    *  Forwarding_GetParamBoolValue
    *  Forwarding_GetParamIntValue
    *  Forwarding_GetParamUlongValue
    *  Forwarding_GetParamStringValue
    *  Forwarding_SetParamBoolValue
    *  Forwarding_SetParamIntValue
    *  Forwarding_SetParamUlongValue
    *  Forwarding_SetParamStringValue
    *  Forwarding_Validate
    *  Forwarding_Commit
    *  Forwarding_Rollback

***********************************************************************/
/**
 * @brief Gets the number of DNS forwarding entries in Device.DNS.Relay.Forwarding table.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Number of DNS forwarding entries.
 */
ULONG
Forwarding_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Gets a DNS forwarding entry from Device.DNS.Relay.Forwarding.{i} table by index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry.
 * @param[out] pInsNumber Pointer to store instance number of the entry.
 *
 * @return Handle to the entry context
 * @retval non-NULL Handle to the entry context.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new DNS forwarding entry to Device.DNS.Relay.Forwarding.{i} table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Pointer to store instance number of newly created entry.
 *
 * @return Handle to the new entry context.
 * @retval non-NULL Handle to the new entry context.
 * @retval NULL Addition failed.
 */
ANSC_HANDLE
Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a DNS forwarding entry from Device.DNS.Relay.Forwarding.{i} table.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the entry to delete.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 */
ULONG
Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Checks if Device.DNS.Relay.Forwarding table has been updated.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The update status.
 * @retval TRUE if the entry table needs to be refreshed.
 * @retval FALSE if the entry table is still current.
 */
BOOL
Forwarding_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes Device.DNS.Relay.Forwarding table with backend data.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if successful.
 * @retval error code otherwise.
 */
ULONG
Forwarding_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values from Device.DNS.Relay.Forwarding.{i}.
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
Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values from Device.DNS.Relay.Forwarding.{i}.
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
Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values from Device.DNS.Relay.Forwarding.{i}.
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
Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values from Device.DNS.Relay.Forwarding.{i}.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pValue Buffer to store string value (buffer size: *pUlSize bytes).
 * @param[in,out] pUlSize Input: buffer size, Output: required/actual size in bytes. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Not supported.
 */
ULONG
Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for Device.DNS.Relay.Forwarding.{i}.
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
Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Device.DNS.Relay.Forwarding.{i}.
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
Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for Device.DNS.Relay.Forwarding.{i}.
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
Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for Device.DNS.Relay.Forwarding.{i}.
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
Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates Device.DNS.Relay.Forwarding.{i} parameter changes before commit.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to store parameter name if there's a validation(buffer size: 128 bytes).
 * @param[out] puLength Pointer to store length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.DNS.Relay.Forwarding.{i} parameter changes to persistent storage.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.DNS.Relay.Forwarding.{i} parameter changes whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Rollback successful.
 * @retval Non-zero error code otherwise.
 */
ULONG
Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DNS.Diagnostics.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    DNS.Diagnostics.NSLookupDiagnostics.

    *  NSLookupDiagnostics_GetParamBoolValue
    *  NSLookupDiagnostics_GetParamIntValue
    *  NSLookupDiagnostics_GetParamUlongValue
    *  NSLookupDiagnostics_GetParamStringValue
    *  NSLookupDiagnostics_SetParamBoolValue
    *  NSLookupDiagnostics_SetParamIntValue
    *  NSLookupDiagnostics_SetParamUlongValue
    *  NSLookupDiagnostics_SetParamStringValue
    *  NSLookupDiagnostics_Validate
    *  NSLookupDiagnostics_Commit
    *  NSLookupDiagnostics_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values from Device.DNS.Diagnostics.NSLookupDiagnostics.
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
NSLookupDiagnostics_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values from Device.DNS.Diagnostics.NSLookupDiagnostics.
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
NSLookupDiagnostics_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values from Device.DNS.Diagnostics.NSLookupDiagnostics.
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
NSLookupDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values from Device.DNS.Diagnostics.NSLookupDiagnostics.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pValue Buffer to store string value (buffer size: *pUlSize bytes).
 * @param[in,out] pUlSize Input: buffer size, Output: required/actual size in bytes. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Not supported.
 */
ULONG
NSLookupDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for Device.DNS.Diagnostics.NSLookupDiagnostics.
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
NSLookupDiagnostics_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Device.DNS.Diagnostics.NSLookupDiagnostics.
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
NSLookupDiagnostics_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for Device.DNS.Diagnostics.NSLookupDiagnostics.
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
NSLookupDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for Device.DNS.Diagnostics.NSLookupDiagnostics.
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
NSLookupDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates Device.DNS.Diagnostics.NSLookupDiagnostics parameter changes before commit.
 *
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to store parameter name if there's a validation(buffer size: 128 bytes).
 * @param[out] puLength Pointer to store length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's a validation.
 */
BOOL
NSLookupDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits Device.DNS.Diagnostics.NSLookupDiagnostics parameter changes and starts test.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed, rollback should be called.
 */
ULONG
NSLookupDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back Device.DNS.Diagnostics.NSLookupDiagnostics parameter changes whenever there is a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Rollback successful.
 * @retval Non-zero error code otherwise.
 */
ULONG
NSLookupDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DNS.Diagnostics.NSLookupDiagnostics.Result.{i}.

    *  Result_GetEntryCount
    *  Result_GetEntry
    *  Result_IsUpdated
    *  Result_Synchronize
    *  Result_GetParamBoolValue
    *  Result_GetParamIntValue
    *  Result_GetParamUlongValue
    *  Result_GetParamStringValue

***********************************************************************/
/**
 * @brief Gets the number of NSLookup result entries in Device.DNS.Diagnostics.NSLookupDiagnostics.Result table.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Number of NSLookup result entries.
 */
ULONG
Result_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Gets an NSLookup result entry from Device.DNS.Diagnostics.NSLookupDiagnostics.Result.{i} table by index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry.
 * @param[out] pInsNumber Pointer to store instance number of the entry.
 *
 * @return Handle to the entry context
 * @retval non-NULL Handle to the entry context.
 * @retval NULL Entry not found.
 */
ANSC_HANDLE
Result_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if Device.DNS.Diagnostics.NSLookupDiagnostics.Result table has been updated.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The update status.
 * @retval TRUE if the entry table needs to be refreshed.
 * @retval FALSE if the entry table is still current.
 */
BOOL
Result_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes Device.DNS.Diagnostics.NSLookupDiagnostics.Result table with backend data.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if successful.
 * @retval error code otherwise.
 */
ULONG
Result_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values from Device.DNS.Diagnostics.NSLookupDiagnostics.Result.{i}.
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
Result_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values from Device.DNS.Diagnostics.NSLookupDiagnostics.Result.{i}.
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
Result_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values from Device.DNS.Diagnostics.NSLookupDiagnostics.Result.{i}.
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
Result_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values from Device.DNS.Diagnostics.NSLookupDiagnostics.Result.{i}.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Parameter name to retrieve (buffer size: 256 bytes).
 * @param[out] pValue Buffer to store string value (buffer size: *pUlSize bytes).
 * @param[in,out] pUlSize Input: buffer size, Output: required/actual size in bytes. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Not supported.
 */
ULONG
Result_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
