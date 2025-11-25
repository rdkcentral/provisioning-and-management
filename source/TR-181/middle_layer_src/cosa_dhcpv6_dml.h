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

    module: cosa_apis_dhcpv6.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        09/07/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_APIS_DHCPV6_H
#define  _COSA_APIS_DHCPV6_H

/***********************************************************************

 APIs for Object:

    DHCPv6.

    *  DHCPv6_GetParamBoolValue
    *  DHCPv6_GetParamIntValue
    *  DHCPv6_GetParamUlongValue
    *  DHCPv6_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for the Device.DHCPv6 object.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 object.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
DHCPv6_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for the Device.DHCPv6 object.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 object.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
DHCPv6_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for the Device.DHCPv6 object.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 object.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
DHCPv6_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for the Device.DHCPv6 object.
 *
 * @param[in]     hInsContext Instance handle for the DHCPv6 object.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value.
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
DHCPv6_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#ifndef FEATURE_RDKB_WAN_MANAGER
/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.

    *  Client3_GetEntryCount
    *  Client3_GetEntry
    *  Client3_AddEntry
    *  Client3_DelEntry
    *  Client3_GetParamBoolValue
    *  Client3_GetParamIntValue
    *  Client3_GetParamUlongValue
    *  Client3_GetParamStringValue
    *  Client3_SetParamBoolValue
    *  Client3_SetParamIntValue
    *  Client3_SetParamUlongValue
    *  Client3_SetParamStringValue
    *  Client3_Validate
    *  Client3_Commit
    *  Client3_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of DHCPv6 client entries in the Device.DHCPv6.Client.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 context.
 *
 * @return The count of DHCPv6 client entries.
 */
ULONG
Client3_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific DHCPv6 client entry from the Device.DHCPv6.Client.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to specific DHCPv6 client entry
 * @retval Handle to the DHCPv6 client entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
Client3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new DHCPv6 client entry to the Device.DHCPv6.Client.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 context.
 * @param[out] pInsNumber  Pointer to store the instance number of the newly created entry.
 *
 * @return Handle to the newly created DHCPv6 client entry
 * @retval Handle to the newly created DHCPv6 client entry
 * @retval NULL on failure.
 */
ANSC_HANDLE
Client3_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a DHCPv6 client entry from the Device.DHCPv6.Client.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 context.
 * @param[in] hInstance   Handle to the specific DHCPv6 client entry to be deleted.
 *
 * @return The status of the operation
 * @retval ANSC_STATUS_SUCCESS on successful deletion
 * @retval error status.
 */
ULONG
Client3_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 client entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 client entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv6 client entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 client entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 client entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 client entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 client entry.
 *
 * @param[in]     hInsContext Instance handle for the DHCPv6 client entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
Client3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for a DHCPv6 client entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client3_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for a DHCPv6 client entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client3_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for a DHCPv6 client entry.
 *
 * @param[in] hInsContext  Instance handle for the DHCPv6 client entry.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client3_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for a DHCPv6 client entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client3_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for a DHCPv6 client entry before committing.
 *
 * @param[in]     hInsContext       Instance handle for the DHCPv6 client entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return The status of validation
 * @retval TRUE if there's no validation
 * @retval FALSE if there's validation.
 */
BOOL
Client3_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for a DHCPv6 client entry to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful commit
 * @retval error status.
 */
ULONG
Client3_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for a DHCPv6 client entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful rollback
 * @retval error status.
 */
ULONG
Client3_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.Server.{i}.

    *  Server2_GetEntryCount
    *  Server2_GetEntry
    *  Server2_IsUpdated
    *  Server2_Synchronize
    *  Server2_GetParamBoolValue
    *  Server2_GetParamIntValue
    *  Server2_GetParamUlongValue
    *  Server2_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of DHCPv6 server entries in the Device.DHCPv6.Client.{i}.Server.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client context.
 *
 * @return The count of DHCPv6 server entries.
 */
ULONG
Server2_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific DHCPv6 server entry from the Device.DHCPv6.Client.{i}.Server.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 client context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the DHCPv6 server entry
 * @retval Handle to the specific DHCPv6 server entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
Server2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the DHCPv6 server table has been updated.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client context.
 *
 * @return The status of the operation
 * @retval TRUE if the table has been updated
 * @retval FALSE otherwise.
 */
BOOL
Server2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the DHCPv6 server table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client context.
 *
 * @return The status of the operation
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error status.
 */
ULONG
Server2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 server entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 server entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Server2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv6 server entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 server entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Server2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 server entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 server entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Server2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 server entry.
 *
 * @param[in]     hInsContext Instance handle for the DHCPv6 server entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
Server2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.SentOption.{i}.

    *  SentOption1_GetEntryCount
    *  SentOption1_GetEntry
    *  SentOption1_AddEntry
    *  SentOption1_DelEntry
    *  SentOption1_GetParamBoolValue
    *  SentOption1_GetParamIntValue
    *  SentOption1_GetParamUlongValue
    *  SentOption1_GetParamStringValue
    *  SentOption1_SetParamBoolValue
    *  SentOption1_SetParamIntValue
    *  SentOption1_SetParamUlongValue
    *  SentOption1_SetParamStringValue
    *  SentOption1_Validate
    *  SentOption1_Commit
    *  SentOption1_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of sent option entries in the Device.DHCPv6.Client.{i}.SentOption.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client context.
 *
 * @return The count of sent option entries.
 */
ULONG
SentOption1_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific sent option entry from the Device.DHCPv6.Client.{i}.SentOption.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 client context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the sent option entry
 * @retval Handle to the specific sent option entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
SentOption1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new sent option entry to the Device.DHCPv6.Client.{i}.SentOption.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 client context.
 * @param[out] pInsNumber  Pointer to store the instance number of the newly created entry.
 *
 * @return Handle to the newly created sent option entry
 * @retval Handle to the newly created sent option entry
 * @retval NULL on failure.
 */
ANSC_HANDLE
SentOption1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a sent option entry from the Device.DHCPv6.Client.{i}.SentOption.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client context.
 * @param[in] hInstance   Handle to the specific sent option entry to be deleted.
 *
 * @return The status of the operation
 * @retval ANSC_STATUS_SUCCESS on successful deletion
 * @retval error status.
 */
ULONG
SentOption1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 sent option entry.
 *
 * @param[in]  hInsContext Instance handle for the sent option entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
SentOption1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv6 sent option entry.
 *
 * @param[in]  hInsContext Instance handle for the sent option entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
SentOption1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 sent option entry.
 *
 * @param[in]  hInsContext Instance handle for the sent option entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
SentOption1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 sent option entry.
 *
 * @param[in]     hInsContext Instance handle for the sent option entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
SentOption1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for a DHCPv6 sent option entry.
 *
 * @param[in] hInsContext Instance handle for the sent option entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
SentOption1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for a DHCPv6 sent option entry.
 *
 * @param[in] hInsContext Instance handle for the sent option entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
SentOption1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for a DHCPv6 sent option entry.
 *
 * @param[in] hInsContext  Instance handle for the sent option entry.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
SentOption1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for a DHCPv6 sent option entry.
 *
 * @param[in] hInsContext Instance handle for the sent option entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
SentOption1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for a DHCPv6 sent option entry before committing.
 *
 * @param[in]     hInsContext       Instance handle for the sent option entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return The status of validation
 * @retval TRUE if there's no validation
 * @retval FALSE if there's validation.
 */
BOOL
SentOption1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for a DHCPv6 sent option entry to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the sent option entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful commit
 * @retval error status.
 */
ULONG
SentOption1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for a DHCPv6 sent option entry.
 *
 * @param[in] hInsContext Instance handle for the sent option entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful rollback
 * @retval error status.
 */
ULONG
SentOption1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.ReceivedOption.{i}.

    *  ReceivedOption_GetEntryCount
    *  ReceivedOption_GetEntry
    *  ReceivedOption_IsUpdated
    *  ReceivedOption_Synchronize
    *  ReceivedOption_GetParamBoolValue
    *  ReceivedOption_GetParamIntValue
    *  ReceivedOption_GetParamUlongValue
    *  ReceivedOption_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of received option entries in the Device.DHCPv6.Client.{i}.ReceivedOption.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client context.
 *
 * @return The count of received option entries.
 */
ULONG
ReceivedOption_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific received option entry from the Device.DHCPv6.Client.{i}.ReceivedOption.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 client context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the received option entry
 * @retval Handle to the specific received option entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
ReceivedOption_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the received option table has been updated.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client context.
 *
 * @return The status of the operation
 * @retval TRUE if the table has been updated
 * @retval FALSE otherwise.
 */
BOOL
ReceivedOption_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the received option table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 client context.
 *
 * @return The status of the operation
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error status.
 */
ULONG
ReceivedOption_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 received option entry.
 *
 * @param[in]  hInsContext Instance handle for the received option entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ReceivedOption_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv6 received option entry.
 *
 * @param[in]  hInsContext Instance handle for the received option entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ReceivedOption_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 received option entry.
 *
 * @param[in]  hInsContext Instance handle for the received option entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ReceivedOption_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 received option entry.
 *
 * @param[in]     hInsContext Instance handle for the received option entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
ReceivedOption_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#endif
/***********************************************************************

 APIs for Object:

    DHCPv6.Server.

    *  Server3_GetParamBoolValue
    *  Server3_GetParamIntValue
    *  Server3_GetParamUlongValue
    *  Server3_GetParamStringValue
    *  Server3_SetParamBoolValue
    *  Server3_SetParamIntValue
    *  Server3_SetParamUlongValue
    *  Server3_SetParamStringValue
    *  Server3_Validate
    *  Server3_Commit
    *  Server3_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for the Device.DHCPv6.Server object.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 Server object.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Server3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for the Device.DHCPv6.Server object.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 Server object.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Server3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for the Device.DHCPv6.Server object.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 Server object.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Server3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for the Device.DHCPv6.Server object.
 *
 * @param[in]     hInsContext Instance handle for the DHCPv6 Server object.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
Server3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for the Device.DHCPv6.Server object.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 Server object.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Server3_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for the Device.DHCPv6.Server object.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 Server object.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Server3_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for the Device.DHCPv6.Server object.
 *
 * @param[in] hInsContext  Instance handle for the DHCPv6 Server object.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
Server3_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for the Device.DHCPv6.Server object.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 Server object.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Server3_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for the Device.DHCPv6.Server object before committing.
 *
 * @param[in]     hInsContext       Instance handle for the DHCPv6 Server object.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return The status of validation
 * @retval TRUE if there's no validation
 * @retval FALSE if validation fails.
 */
BOOL
Server3_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for the Device.DHCPv6.Server object to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 Server object.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful commit
 * @retval error status.
 */
ULONG
Server3_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for the Device.DHCPv6.Server object.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 Server object.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful rollback
 * @retval error status.
 */
ULONG
Server3_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.

    *  Pool1_GetEntryCount
    *  Pool1_GetEntry
    *  Pool1_AddEntry
    *  Pool1_DelEntry
    *  Pool1_GetParamBoolValue
    *  Pool1_GetParamUlongValue
    *  Pool1_GetParamStringValue
    *  Pool1_SetParamBoolValue
    *  Pool1_SetParamUlongValue
    *  Pool1_SetParamStringValue
    *  Pool1_Validate
    *  Pool1_Commit
    *  Pool1_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of DHCPv6 server pool entries in the Device.DHCPv6.Server.Pool.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 Server context.
 *
 * @return The count of DHCPv6 server pool entries.
 */
ULONG
Pool1_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific DHCPv6 server pool entry from the Device.DHCPv6.Server.Pool.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 Server context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the DHCPv6 server pool entry
 * @retval Handle to the specific DHCPv6 server pool entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
Pool1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new DHCPv6 server pool entry to the Device.DHCPv6.Server.Pool.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 Server context.
 * @param[out] pInsNumber  Pointer to store the instance number of the newly created entry.
 *
 * @return Handle to the newly created DHCPv6 server pool entry
 * @retval Handle to the newly created DHCPv6 server pool entry
 * @retval NULL on failure.
 */
ANSC_HANDLE
Pool1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a DHCPv6 server pool entry from the Device.DHCPv6.Server.Pool.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 Server context.
 * @param[in] hInstance   Handle to the specific DHCPv6 server pool entry to be deleted.
 *
 * @return The status of the operation
 * @retval ANSC_STATUS_SUCCESS on successful deletion
 * @retval error status.
 */
ULONG
Pool1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 server pool entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 server pool entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 server pool entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 server pool entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 server pool entry.
 *
 * @param[in]     hInsContext Instance handle for the DHCPv6 server pool entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
Pool1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for a DHCPv6 server pool entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 server pool entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets unsigned long parameter values for a DHCPv6 server pool entry.
 *
 * @param[in] hInsContext  Instance handle for the DHCPv6 server pool entry.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for a DHCPv6 server pool entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 server pool entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for a DHCPv6 server pool entry before committing.
 *
 * @param[in]     hInsContext       Instance handle for the DHCPv6 server pool entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return The status of validation
 * @retval TRUE if there's no validation
 * @retval FALSE if validation fails
 */
BOOL
Pool1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for a DHCPv6 server pool entry to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 server pool entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful commit
 * @retval error status.
 */
ULONG
Pool1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for a DHCPv6 server pool entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 server pool entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful rollback
 * @retval error status.
 */
ULONG
Pool1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.

    *  Client4_GetEntryCount
    *  Client4_GetEntry
    *  Client4_IsUpdated
    *  Client4_Synchronize
    *  Client4_GetParamBoolValue
    *  Client4_GetParamIntValue
    *  Client4_GetParamUlongValue
    *  Client4_GetParamStringValue
    *  Client4_SetParamBoolValue
    *  Client4_SetParamIntValue
    *  Client4_SetParamUlongValue
    *  Client4_SetParamStringValue
    *  Client4_Validate
    *  Client4_Commit
    *  Client4_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of DHCPv6 pool client entries in the Device.DHCPv6.Server.Pool.{i}.Client.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool context.
 *
 * @return The count of DHCPv6 pool client entries.
 */
ULONG
Client4_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific DHCPv6 pool client entry from the Device.DHCPv6.Server.Pool.{i}.Client.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the DHCPv6 pool client entry
 * @retval Handle to the specific DHCPv6 pool client entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
Client4_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the DHCPv6 pool client table has been updated.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool context.
 *
 * @return The status of the operation
 * @retval TRUE if the table has been updated
 * @retval FALSE otherwise.
 */
BOOL
Client4_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the DHCPv6 pool client table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool context.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error status.
 */
ULONG
Client4_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 pool client entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool client entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv6 pool client entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool client entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client4_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 pool client entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool client entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client4_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 pool client entry.
 *
 * @param[in]     hInsContext Instance handle for the DHCPv6 pool client entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
Client4_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for a DHCPv6 pool client entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client4_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for a DHCPv6 pool client entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client4_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for a DHCPv6 pool client entry.
 *
 * @param[in] hInsContext  Instance handle for the DHCPv6 pool client entry.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client4_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for a DHCPv6 pool client entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client4_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for a DHCPv6 pool client entry before committing.
 *
 * @param[in]     hInsContext       Instance handle for the DHCPv6 pool client entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return The status of validation
 * @retval TRUE if there's no validation
 * @retval FALSE if validation fails.
 */
BOOL
Client4_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for a DHCPv6 pool client entry to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client entry.
 *
 * @return The status of commit operation
 * @retval ANSC_STATUS_SUCCESS on successful commit
 * @retval error status.
 */
ULONG
Client4_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for a DHCPv6 pool client entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client entry.
 *
 * @return The status of rollback operation
 * @retval ANSC_STATUS_SUCCESS on successful rollback
 * @retval error status.
 */
ULONG
Client4_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Address.{i}.

    *  IPv6Address2_GetEntryCount
    *  IPv6Address2_GetEntry
    *  IPv6Address2_IsUpdated
    *  IPv6Address2_Synchronize
    *  IPv6Address2_GetParamBoolValue
    *  IPv6Address2_GetParamIntValue
    *  IPv6Address2_GetParamUlongValue
    *  IPv6Address2_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of IPv6 address entries in the Device.DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Address.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client context.
 *
 * @return The count of IPv6 address entries for the client.
 */
ULONG
IPv6Address2_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific IPv6 address entry from the Device.DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Address.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool client context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the IPv6 address entry
 * @retval Handle to the specific IPv6 address entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
IPv6Address2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the IPv6 address table has been updated.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client context.
 *
 * @return The status of the operation
 * @retval TRUE if the table has been updated
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the IPv6 address table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client context.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error status.
 */
ULONG
IPv6Address2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 pool client IPv6 address entry.
 *
 * @param[in]  hInsContext Instance handle for the IPv6 address entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv6 pool client IPv6 address entry.
 *
 * @param[in]  hInsContext Instance handle for the IPv6 address entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 pool client IPv6 address entry.
 *
 * @param[in]  hInsContext Instance handle for the IPv6 address entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 pool client IPv6 address entry.
 *
 * @param[in]     hInsContext Instance handle for the IPv6 address entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
IPv6Address2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Prefix.{i}.

    *  IPv6Prefix1_GetEntryCount
    *  IPv6Prefix1_GetEntry
    *  IPv6Prefix1_IsUpdated
    *  IPv6Prefix1_Synchronize
    *  IPv6Prefix1_GetParamBoolValue
    *  IPv6Prefix1_GetParamIntValue
    *  IPv6Prefix1_GetParamUlongValue
    *  IPv6Prefix1_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of IPv6 prefix entries in the Device.DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Prefix.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client context.
 *
 * @return The count of IPv6 prefix entries for the client.
 */
ULONG
IPv6Prefix1_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific IPv6 prefix entry from the Device.DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Prefix.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool client context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the IPv6 prefix entry
 * @retval Handle to the specific IPv6 prefix entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
IPv6Prefix1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the IPv6 prefix table has been updated.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client context.
 *
 * @return The status of the operation
 * @retval TRUE if the table has been updated
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix1_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the IPv6 prefix table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client context.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error status.
 */
ULONG
IPv6Prefix1_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 pool client IPv6 prefix entry.
 *
 * @param[in]  hInsContext Instance handle for the IPv6 prefix entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv6 pool client IPv6 prefix entry.
 *
 * @param[in]  hInsContext Instance handle for the IPv6 prefix entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 pool client IPv6 prefix entry.
 *
 * @param[in]  hInsContext Instance handle for the IPv6 prefix entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 pool client IPv6 prefix entry.
 *
 * @param[in]     hInsContext Instance handle for the IPv6 prefix entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
IPv6Prefix1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.Option.{i}.

    *  Option3_GetEntryCount
    *  Option3_GetEntry
    *  Option3_IsUpdated
    *  Option3_Synchronize
    *  Option3_GetParamBoolValue
    *  Option3_GetParamIntValue
    *  Option3_GetParamUlongValue
    *  Option3_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of DHCPv6 option entries in the Device.DHCPv6.Server.Pool.{i}.Client.{i}.Option.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client context.
 *
 * @return The count of DHCPv6 option entries for the client.
 */
ULONG
Option3_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific DHCPv6 option entry from the Device.DHCPv6.Server.Pool.{i}.Client.{i}.Option.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool client context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the DHCPv6 option entry
 * @retval Handle to the specific DHCPv6 option entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
Option3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the DHCPv6 option table has been updated.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client context.
 *
 * @return The status of the operation
 * @retval TRUE if the table has been updated
 * @retval FALSE otherwise.
 */
BOOL
Option3_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the DHCPv6 option table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool client context.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error status.
 */
ULONG
Option3_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 pool client option entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 option entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv6 pool client option entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 option entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 pool client option entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 option entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 pool client option entry.
 *
 * @param[in]     hInsContext Instance handle for the DHCPv6 option entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
Option3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Option.{i}.

    *  Option4_GetEntryCount
    *  Option4_GetEntry
    *  Option4_AddEntry
    *  Option4_DelEntry
    *  Option4_GetParamBoolValue
    *  Option4_GetParamIntValue
    *  Option4_GetParamUlongValue
    *  Option4_GetParamStringValue
    *  Option4_SetParamBoolValue
    *  Option4_SetParamIntValue
    *  Option4_SetParamUlongValue
    *  Option4_SetParamStringValue
    *  Option4_Validate
    *  Option4_Commit
    *  Option4_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of DHCPv6 option entries in the Device.DHCPv6.Server.Pool.{i}.Option.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 server pool context.
 *
 * @return The count of DHCPv6 option entries for the pool.
 */
ULONG
Option4_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific DHCPv6 option entry from the Device.DHCPv6.Server.Pool.{i}.Option.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 server pool context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the DHCPv6 option entry
 * @retval Handle to the specific DHCPv6 option entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
Option4_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new DHCPv6 option entry to the Device.DHCPv6.Server.Pool.{i}.Option.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 server pool context.
 * @param[out] pInsNumber  Pointer to store the instance number of the newly created entry.
 *
 * @return Handle to the newly created DHCPv6 option entry
 * @retval Handle to the new DHCPv6 option entry
 * @retval NULL on failure.
 */
ANSC_HANDLE
Option4_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a DHCPv6 option entry from the Device.DHCPv6.Server.Pool.{i}.Option.{i} table.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 server pool context.
 * @param[in] hInstance   Handle to the specific DHCPv6 option entry to be deleted.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful deletion
 * @retval error status.
 */
ULONG
Option4_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv6 pool option entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool option entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv6 pool option entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool option entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option4_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv6 pool option entry.
 *
 * @param[in]  hInsContext Instance handle for the DHCPv6 pool option entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option4_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv6 pool option entry.
 *
 * @param[in]     hInsContext Instance handle for the DHCPv6 pool option entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
Option4_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for a DHCPv6 pool option entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool option entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Option4_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for a DHCPv6 pool option entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool option entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Option4_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for a DHCPv6 pool option entry.
 *
 * @param[in] hInsContext  Instance handle for the DHCPv6 pool option entry.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Option4_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for a DHCPv6 pool option entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool option entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Option4_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for a DHCPv6 pool option entry before committing.
 *
 * @param[in]     hInsContext       Instance handle for the DHCPv6 pool option entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return status of validation
 * @retval TRUE if there's no validation
 * @retval FALSE if there's validation.
 */
BOOL
Option4_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for a DHCPv6 pool option entry to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool option entry.
 *
 * @return status of commit operation
 * @retval ANSC_STATUS_SUCCESS on successful commit
 * @retval error status.
 */
ULONG
Option4_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for a DHCPv6 pool option entry.
 *
 * @param[in] hInsContext Instance handle for the DHCPv6 pool option entry.
 *
 * @return status of rollback operation
 * @retval ANSC_STATUS_SUCCESS on successful rollback
 * @retval error status.
 */
ULONG
Option4_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
