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

    module: cosa_ip_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/18/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_IP_DML_H
#define  _COSA_IP_DML_H

/***********************************************************************

 APIs for Object:

    IP.

    *  IP_GetParamBoolValue
    *  IP_GetParamIntValue
    *  IP_GetParamUlongValue
    *  IP_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for IP base object.
 *
 * @param[in]  hInsContext Instance handle for the IP context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for IP base object.
 *
 * @param[in]  hInsContext Instance handle for the IP context.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for IP base object.
 *
 * @param[in]  hInsContext Instance handle for the IP context.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for IP base object.
 *
 * @param[in]     hInsContext Instance handle for the IP context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
IP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
/**
 * @brief Sets boolean parameter values for IP base object.
 *
 * @param[in] hInsContext Instance handle for the IP context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
IP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for IP base object.
 *
 * @param[in] hInsContext Instance handle for the IP context.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
IP_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for IP base object.
 *
 * @param[in] hInsContext   Instance handle for the IP context.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
IP_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for IP base object.
 *
 * @param[in] hInsContext Instance handle for the IP context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
IP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
* @brief Validates pending parameter changes for IP base object.
*
* @param[in] hInsContext       - Instance handle for the IP context.
* @param[out] pReturnParamName - Pointer to a buffer(128 bytes) to store the parameter name if there's a validation.
* @param[in,out] puLength      - Pointer to the buffer size; updated with actual size on return.
*
* @return The status of the validation.
* @retval TRUE if there's no validation.
* @retval FALSE if there's validation.
*
*/
BOOL
IP_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
* @brief Commits pending parameter changes for IP base object.
*
* @param[in] hInsContext - Instance handle for the IP context.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval nonzero if any error is detected during the operation.
*
*/
ULONG
IP_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
* @brief Rolls back pending parameter changes for IP base object.
*
* @param[in] hInsContext - Instance handle for the IP context.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
*
*/
ULONG
IP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.

    *  Interface2_GetEntryCount
    *  Interface2_GetEntry
    *  Interface2_AddEntry
    *  Interface2_DelEntry
    *  Interface2_GetParamBoolValue
    *  Interface2_GetParamIntValue
    *  Interface2_GetParamUlongValue
    *  Interface2_GetParamStringValue
    *  Interface2_SetParamBoolValue
    *  Interface2_SetParamIntValue
    *  Interface2_SetParamUlongValue
    *  Interface2_SetParamStringValue
    *  Interface2_Validate
    *  Interface2_Commit
    *  Interface2_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of IP Interface entries.
 *
 * @param[in] hInsContext Instance handle for the IP context.
 *
 * @return The count of IP Interface entries.
 */
ULONG
Interface2_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific IP Interface entry by index.
 *
 * @param[in]  hInsContext  Instance handle for the IP context.
 * @param[in]  nIndex       Index of the Interface entry to retrieve (0-based).
 * @param[out] pInsNumber   Pointer to store the instance number of the entry.
 *
 * @return Handle to the Interface entry
 * @retval Handle to the specific Interface entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
Interface2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new IP Interface entry.
 *
 * @param[in]  hInsContext  Instance handle for the IP context.
 * @param[out] pInsNumber   Pointer to store the instance number assigned to the new entry.
 *
 * @return Handle to the newly created Interface entry
 * @retval Handle to the newly created IP Interface entry
 * @retval NULL if creation failed.
 */
ANSC_HANDLE
Interface2_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes an IP Interface entry.
 *
 * @param[in] hInsContext Instance handle for the IP context.
 * @param[in] hInstance   Handle to the Interface entry to delete.
 *
 * @return Status of the deletion operation.
 * @retval 0       Deletion succeeded.
 * @retval nonzero Deletion failed.
 */
ULONG
Interface2_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for IP Interface.
 *
 * @param[in]  hInsContext Instance handle for the Interface entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return Status of the operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Interface2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for IP Interface.
 *
 * @param[in]  hInsContext Instance handle for the Interface entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return Status of the operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Interface2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for IP Interface.
 *
 * @param[in]  hInsContext Instance handle for the Interface entry.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return Status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Interface2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for IP Interface.
 *
 * @param[in]     hInsContext Instance handle for the Interface entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string. Usually size of 1023 will be used.
 *
 * @return Status of the operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
Interface2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Interface2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Interface2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for IP Interface.
 *
 * @param[in] hInsContext   Instance handle for the Interface entry.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Interface2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Interface2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending parameter changes for IP Interface.
 *
 * @param[in]     hInsContext       Instance handle for the Interface entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of returned parameter name.
 *
 * @return Status of operation
 * @retval TRUE if there's no validation
 * @retval FALSE if there's validation.
 */
BOOL
Interface2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending parameter changes for IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @return Status of the operation.
 * @retval 0       Commit succeeded.
 * @retval nonzero Commit failed, changes rolled back.
 */
ULONG
Interface2_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending parameter changes for IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @return Status of the operation.
 * @retval 0 Rollback succeeded.
 */
ULONG
Interface2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv4Address.{i}.

    *  IPv4Address_GetEntryCount
    *  IPv4Address_GetEntry
    *  IPv4Address_AddEntry
    *  IPv4Address_DelEntry
    *  IPv4Address_GetParamBoolValue
    *  IPv4Address_GetParamIntValue
    *  IPv4Address_GetParamUlongValue
    *  IPv4Address_GetParamStringValue
    *  IPv4Address_SetParamBoolValue
    *  IPv4Address_SetParamIntValue
    *  IPv4Address_SetParamUlongValue
    *  IPv4Address_SetParamStringValue
    *  IPv4Address_Validate
    *  IPv4Address_Commit
    *  IPv4Address_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of IPv4 Address entries for an IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @return The count of IPv4 Address entries.
 */
ULONG
IPv4Address_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific IPv4 Address entry by index.
 *
 * @param[in]  hInsContext  Instance handle for the Interface entry.
 * @param[in]  nIndex       Index of the IPv4Address entry to retrieve (0-based).
 * @param[out] pInsNumber   Pointer to store the instance number of the entry.
 *
 * @return Handle to the IPv4Address entry
 * @retval Handle to the specific IPv4Address entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
IPv4Address_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new IPv4 Address entry to an IP Interface.
 *
 * @param[in]  hInsContext  Instance handle for the Interface entry.
 * @param[out] pInsNumber   Pointer to store the instance number assigned to the new entry.
 *
 * @return Handle to the newly created IPv4Address entry
 * @retval Handle to the newly created IPv4Address entry
 * @retval NULL if creation failed.
 */
ANSC_HANDLE
IPv4Address_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes an IPv4 Address entry from an IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] hInstance   Handle to the IPv4Address entry to delete.
 *
 * @return Status of the operation.
 * @retval 0       Deletion succeeded.
 * @retval nonzero Deletion failed.
 */
ULONG
IPv4Address_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for IPv4 Address.
 *
 * @param[in]  hInsContext Instance handle for the IPv4Address entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for IPv4 Address.
 *
 * @param[in]  hInsContext Instance handle for the IPv4Address entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for IPv4 Address.
 *
 * @param[in]  hInsContext Instance handle for the IPv4Address entry.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for IPv4 Address.
 *
 * @param[in]     hInsContext Instance handle for the IPv4Address entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 64 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
IPv4Address_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for IPv4 Address.
 *
 * @param[in] hInsContext Instance handle for the IPv4Address entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for IPv4 Address.
 *
 * @param[in] hInsContext Instance handle for the IPv4Address entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for IPv4 Address.
 *
 * @param[in] hInsContext   Instance handle for the IPv4Address entry.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for IPv4 Address.
 *
 * @param[in] hInsContext Instance handle for the IPv4Address entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 64 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending parameter changes for IPv4 Address.
 *
 * @param[in]     hInsContext       Instance handle for the IPv4Address entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the parameter name if there's no validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of returned parameter name.
 *
 * @return Status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
IPv4Address_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending parameter changes for IPv4 Address.
 *
 * @param[in] hInsContext Instance handle for the IPv4Address entry.
 *
 * @return Status of the operation.
 * @retval 0       Commit succeeded.
 * @retval nonzero Commit failed, changes rolled back.
 */
ULONG
IPv4Address_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending parameter changes for IPv4 Address.
 *
 * @param[in] hInsContext Instance handle for the IPv4Address entry.
 *
 * @return Status of the operation.
 * @retval 0 Rollback succeeded.
 */
ULONG
IPv4Address_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv6Address.{i}.

    *  IPv6Address_GetEntryCount
    *  IPv6Address_GetEntry
    *  IPv6Address_AddEntry
    *  IPv6Address_DelEntry
    *  IPv6Address_GetParamBoolValue
    *  IPv6Address_GetParamIntValue
    *  IPv6Address_GetParamUlongValue
    *  IPv6Address_GetParamStringValue
    *  IPv6Address_SetParamBoolValue
    *  IPv6Address_SetParamIntValue
    *  IPv6Address_SetParamUlongValue
    *  IPv6Address_SetParamStringValue
    *  IPv6Address_Validate
    *  IPv6Address_Commit
    *  IPv6Address_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of IPv6 Address entries for an IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @return The count of IPv6 Address entries.
 */
ULONG
IPv6Address_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific IPv6 Address entry by index.
 *
 * @param[in]  hInsContext  Instance handle for the Interface entry.
 * @param[in]  nIndex       Index of the IPv6Address entry to retrieve (0-based).
 * @param[out] pInsNumber   Pointer to store the instance number of the entry.
 *
 * @return Handle to the IPv6Address entry.
 * @retval Handle to the IPv6Address entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
IPv6Address_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new IPv6 Address entry to an IP Interface.
 *
 * @param[in]  hInsContext  Instance handle for the Interface entry.
 * @param[out] pInsNumber   Pointer to store the instance number assigned to the new entry.
 *
 * @return Handle to the IPv6Address entry
 * @retval Handle to the newly created IPv6Address entry
 * @retval NULL if creation failed.
 */
ANSC_HANDLE
IPv6Address_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes an IPv6 Address entry from an IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] hInstance   Handle to the IPv6Address entry to delete.
 *
 * @return Status of the operation.
 * @retval 0       Deletion succeeded.
 * @retval nonzero Deletion failed.
 */
ULONG
IPv6Address_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for IPv6 Address.
 *
 * @param[in]  hInsContext Instance handle for the IPv6Address entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for IPv6 Address.
 *
 * @param[in]  hInsContext Instance handle for the IPv6Address entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for IPv6 Address.
 *
 * @param[in]  hInsContext Instance handle for the IPv6Address entry.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for IPv6 Address.
 *
 * @param[in]     hInsContext Instance handle for the IPv6Address entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 128 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string.Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
IPv6Address_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for IPv6 Address.
 *
 * @param[in] hInsContext Instance handle for the IPv6Address entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for IPv6 Address.
 *
 * @param[in] hInsContext Instance handle for the IPv6Address entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for IPv6 Address.
 *
 * @param[in] hInsContext   Instance handle for the IPv6Address entry.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for IPv6 Address.
 *
 * @param[in] hInsContext Instance handle for the IPv6Address entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 128 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Address_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
* @brief Validates pending parameter changes for IPv6 Address.
*
* @param[in] hInsContext       - Instance handle for the IPv6Address entry.
* @param[out] pReturnParamName - Pointer to a buffer(128 bytes) to store the parameter name if there's a validation.
* @param[in,out] puLength      - Pointer to the buffer size; updated with actual size on return.
*
* @return The status of the validation.
* @retval TRUE if there's no validation.
* @retval FALSE if there's validation.
*
*/
BOOL
IPv6Address_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
* @brief Commits pending parameter changes for IPv6 Address.
*
* @param[in] hInsContext - Instance handle for the IPv6Address entry.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval nonzero if any error is detected during the operation.
*
*/
ULONG
IPv6Address_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
* @brief Roll back the update whenever there's a validation found.
*
* @param[in] hInsContext - Instance handle for the IPv6Address entry.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
*
*/
ULONG
IPv6Address_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
* @brief Check if the IPv6 Address table needs to be updated.
*
* @param[in] hInsContext - Instance handle for the Interface entry.
*
* @return The update status.
* @retval TRUE if the IPv6 Address table needs to be refreshed.
* @retval FALSE if the IPv6 Address table is still current.
*
*/
BOOL
IPv6Address_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes IPv6 Address entries with the system.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS Synchronization succeeded.
 * @retval ANSC_STATUS_FAILURE Synchronization failed.
 */
ULONG
IPv6Address_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv6Prefix.{i}.

    *  IPv6Prefix_GetEntryCount
    *  IPv6Prefix_GetEntry
    *  IPv6Prefix_AddEntry
    *  IPv6Prefix_DelEntry
    *  IPv6Prefix_GetParamBoolValue
    *  IPv6Prefix_GetParamIntValue
    *  IPv6Prefix_GetParamUlongValue
    *  IPv6Prefix_GetParamStringValue
    *  IPv6Prefix_SetParamBoolValue
    *  IPv6Prefix_SetParamIntValue
    *  IPv6Prefix_SetParamUlongValue
    *  IPv6Prefix_SetParamStringValue
    *  IPv6Prefix_Validate
    *  IPv6Prefix_Commit
    *  IPv6Prefix_Rollback

***********************************************************************/
/**
 * @brief Checks if IPv6 Prefix entries have been updated.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @return status of update.
 * @retval TRUE if entries have been updated
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes IPv6 Prefix entries with the system.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @retval 0 Synchronization succeeded.
 */
ULONG
IPv6Prefix_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves the number of IPv6 Prefix entries for an IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 *
 * @return The count of IPv6 Prefix entries.
 */
ULONG
IPv6Prefix_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific IPv6 Prefix entry by index.
 *
 * @param[in]  hInsContext  Instance handle for the Interface entry.
 * @param[in]  nIndex       Index of the IPv6Prefix entry to retrieve (0-based).
 * @param[out] pInsNumber   Pointer to store the instance number of the entry.
 *
 * @return Handle to the IPv6Prefix entry
 * @retval Handle to the specific IPv6Prefix entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
IPv6Prefix_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new IPv6 Prefix entry to an IP Interface.
 *
 * @param[in]  hInsContext  Instance handle for the Interface entry.
 * @param[out] pInsNumber   Pointer to store the instance number assigned to the new entry.
 *
 * @return Handle to the IPv6Prefix entry
 * @retval Handle to the newly created IPv6Prefix entry
 * @retval NULL if creation failed.
 */
ANSC_HANDLE
IPv6Prefix_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes an IPv6 Prefix entry from an IP Interface.
 *
 * @param[in] hInsContext Instance handle for the Interface entry.
 * @param[in] hInstance   Handle to the IPv6Prefix entry to delete.
 *
 * @retval 0       Deletion succeeded.
 * @retval nonzero Deletion failed.
 */
ULONG
IPv6Prefix_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for IPv6 Prefix.
 *
 * @param[in]  hInsContext Instance handle for the IPv6Prefix entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for IPv6 Prefix.
 *
 * @param[in]  hInsContext Instance handle for the IPv6Prefix entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for IPv6 Prefix.
 *
 * @param[in]  hInsContext Instance handle for the IPv6Prefix entry.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for IPv6 Prefix.
 *
 * @param[in]     hInsContext Instance handle for the IPv6Prefix entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 128 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string.Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
IPv6Prefix_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for IPv6 Prefix.
 *
 * @param[in] hInsContext Instance handle for the IPv6Prefix entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for IPv6 Prefix.
 *
 * @param[in] hInsContext Instance handle for the IPv6Prefix entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for IPv6 Prefix.
 *
 * @param[in] hInsContext   Instance handle for the IPv6Prefix entry.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for IPv6 Prefix.
 *
 * @param[in] hInsContext Instance handle for the IPv6Prefix entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 128 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending parameter changes for IPv6 Prefix.
 *
 * @param[in]     hInsContext       Instance handle for the IPv6Prefix entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of returned parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation
 * @retval FALSE otherwise.
 */
BOOL
IPv6Prefix_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending parameter changes for IPv6 Prefix.
 *
 * @param[in] hInsContext Instance handle for the IPv6Prefix entry.
 *
 * @return Status of the operation.
 * @retval 0       Commit succeeded.
 * @retval nonzero Commit failed, changes rolled back.
 */
ULONG
IPv6Prefix_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending parameter changes for IPv6 Prefix.
 *
 * @param[in] hInsContext Instance handle for the IPv6Prefix entry.
 *
 * @return Status of the operation.
 * @retval 0 Rollback succeeded.
 */
ULONG
IPv6Prefix_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.Stats.

    *  Stats5_GetParamBoolValue
    *  Stats5_GetParamIntValue
    *  Stats5_GetParamUlongValue
    *  Stats5_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for IP Interface Statistics.
 *
 * @param[in]  hInsContext Instance handle for the Interface entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Stats5_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for IP Interface Statistics.
 *
 * @param[in]  hInsContext Instance handle for the Interface entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Stats5_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for IP Interface Statistics.
 *
 * @param[in]  hInsContext Instance handle for the Interface entry.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Stats5_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for IP Interface Statistics.
 *
 * @param[in]     hInsContext Instance handle for the Interface entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string.Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
Stats5_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.ActivePort.{i}.

    *  ActivePort_GetEntryCount
    *  ActivePort_GetEntry
    *  ActivePort_IsUpdated
    *  ActivePort_Synchronize
    *  ActivePort_GetParamBoolValue
    *  ActivePort_GetParamIntValue
    *  ActivePort_GetParamUlongValue
    *  ActivePort_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of Active Port entries.
 *
 * @param[in] hInsContext Instance handle for the IP context.
 *
 * @return The count of Active Port entries.
 */
ULONG
ActivePort_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific Active Port entry by index.
 *
 * @param[in]  hInsContext  Instance handle for the IP context.
 * @param[in]  nIndex       Index of the ActivePort entry to retrieve (0-based).
 * @param[out] pInsNumber   Pointer to store the instance number of the entry.
 *
 * @return Handle to the ActivePort entry
 * @retval Handle to the specific ActivePort entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
ActivePort_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if Active Port entries have been updated.
 *
 * @param[in] hInsContext Instance handle for the IP context.
 *
 * @return status of update.
 * @retval TRUE if entries have been updated
 * @retval FALSE otherwise.
 */
BOOL
ActivePort_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes Active Port entries with the system.
 *
 * @param[in] hInsContext Instance handle for the IP context.
 *
 * @retval 0 Synchronization succeeded.
 */
ULONG
ActivePort_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for Active Port.
 *
 * @param[in]  hInsContext Instance handle for the ActivePort entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ActivePort_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for Active Port.
 *
 * @param[in]  hInsContext Instance handle for the ActivePort entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ActivePort_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for Active Port.
 *
 * @param[in]  hInsContext Instance handle for the ActivePort entry.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ActivePort_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for Active Port.
 *
 * @param[in]     hInsContext Instance handle for the ActivePort entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
ActivePort_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    IP.Diagnostics.X_CISCO_COM_ARP.

    *  X_CISCO_COM_ARP_GetParamBoolValue
    *  X_CISCO_COM_ARP_GetParamIntValue
    *  X_CISCO_COM_ARP_GetParamUlongValue
    *  X_CISCO_COM_ARP_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for ARP diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the ARP diagnostics context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
X_CISCO_COM_ARP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for ARP diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the ARP diagnostics context.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
X_CISCO_COM_ARP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for ARP diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the ARP diagnostics context.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
X_CISCO_COM_ARP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for ARP diagnostics.
 *
 * @param[in]     hInsContext Instance handle for the ARP diagnostics context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
X_CISCO_COM_ARP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.X_CISCO_COM_ARP.Table.{i}.

    *  ARPTable_GetEntryCount
    *  ARPTable_GetEntry
    *  ARPTable_IsUpdated
    *  ARPTable_Synchronize
    *  ARPTable_GetParamBoolValue
    *  ARPTable_GetParamIntValue
    *  ARPTable_GetParamUlongValue
    *  ARPTable_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of ARP Table entries.
 *
 * @param[in] hInsContext Instance handle for the ARP diagnostics context.
 *
 * @return The count of ARP Table entries.
 */
ULONG
ARPTable_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific ARP Table entry by index.
 *
 * @param[in]  hInsContext  Instance handle for the ARP diagnostics context.
 * @param[in]  nIndex       Index of the ARPTable entry to retrieve (0-based).
 * @param[out] pInsNumber   Pointer to store the instance number of the entry.
 *
 * @return Handle to the ARPTable entry
 * @retval Handle to the specific ARPTable entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
ARPTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if ARP Table entries have been updated.
 *
 * @param[in] hInsContext Instance handle for the ARP diagnostics context.
 *
 * @return status of update.
 * @retval TRUE if entries have been updated
 * @retval FALSE otherwise.
 */
BOOL
ARPTable_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes ARP Table entries with the system.
 *
 * @param[in] hInsContext Instance handle for the ARP diagnostics context.
 *
 * @retval 0 Synchronization succeeded.
 */
ULONG
ARPTable_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for ARP Table entry.
 *
 * @param[in]  hInsContext Instance handle for the ARPTable entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ARPTable_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for ARP Table entry.
 *
 * @param[in]  hInsContext Instance handle for the ARPTable entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ARPTable_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for ARP Table entry.
 *
 * @param[in]  hInsContext Instance handle for the ARPTable entry.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ARPTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for ARP Table entry.
 *
 * @param[in]     hInsContext Instance handle for the ARPTable entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
ARPTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.IPPing.

    *  IPPing_GetParamBoolValue
    *  IPPing_GetParamIntValue
    *  IPPing_GetParamUlongValue
    *  IPPing_GetParamStringValue
    *  IPPing_SetParamBoolValue
    *  IPPing_SetParamIntValue
    *  IPPing_SetParamUlongValue
    *  IPPing_SetParamStringValue
    *  IPPing_Validate
    *  IPPing_Commit
    *  IPPing_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for IP Ping diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the IPPing context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPPing_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for IP Ping diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the IPPing context.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPPing_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for IP Ping diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the IPPing context.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPPing_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for IP Ping diagnostics.
 *
 * @param[in]     hInsContext Instance handle for the IPPing context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
IPPing_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for IP Ping diagnostics.
 *
 * @param[in] hInsContext Instance handle for the IPPing context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPPing_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for IP Ping diagnostics.
 *
 * @param[in] hInsContext Instance handle for the IPPing context.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPPing_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for IP Ping diagnostics.
 *
 * @param[in] hInsContext   Instance handle for the IPPing context.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPPing_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for IP Ping diagnostics.
 *
 * @param[in] hInsContext Instance handle for the IPPing context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPPing_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending parameter changes for IP Ping diagnostics.
 *
 * @param[in]     hInsContext       Instance handle for the IPPing context.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of returned parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
IPPing_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending parameter changes for IP Ping diagnostics.
 *
 * @param[in] hInsContext Instance handle for the IPPing context.
 *
 * @retval 0       Commit succeeded.
 * @retval nonzero Commit failed, changes rolled back.
 */
ULONG
IPPing_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance handle for the IPPing context.
 *
 * @retval 0 Rollback succeeded.
 */
ULONG
IPPing_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.TraceRoute.

    *  TraceRoute_GetParamBoolValue
    *  TraceRoute_GetParamIntValue
    *  TraceRoute_GetParamUlongValue
    *  TraceRoute_GetParamStringValue
    *  TraceRoute_SetParamBoolValue
    *  TraceRoute_SetParamIntValue
    *  TraceRoute_SetParamUlongValue
    *  TraceRoute_SetParamStringValue
    *  TraceRoute_Validate
    *  TraceRoute_Commit
    *  TraceRoute_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for TraceRoute diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the TraceRoute context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
TraceRoute_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for TraceRoute diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the TraceRoute context.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
TraceRoute_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for TraceRoute diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the TraceRoute context.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
TraceRoute_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for TraceRoute diagnostics.
 *
 * @param[in]     hInsContext Instance handle for the TraceRoute context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
TraceRoute_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for TraceRoute diagnostics.
 *
 * @param[in] hInsContext Instance handle for the TraceRoute context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
TraceRoute_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for TraceRoute diagnostics.
 *
 * @param[in] hInsContext Instance handle for the TraceRoute context.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
TraceRoute_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for TraceRoute diagnostics.
 *
 * @param[in] hInsContext   Instance handle for the TraceRoute context.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
TraceRoute_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for TraceRoute diagnostics.
 *
 * @param[in] hInsContext Instance handle for the TraceRoute context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise
 */
BOOL
TraceRoute_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending parameter changes for TraceRoute diagnostics.
 *
 * @param[in]     hInsContext       Instance handle for the TraceRoute context.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of returned parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation
 * @retval FALSE if there's validation.
 */
BOOL
TraceRoute_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending parameter changes for TraceRoute diagnostics.
 *
 * @param[in] hInsContext Instance handle for the TraceRoute context.
 *
 * @retval 0       Commit succeeded.
 * @retval nonzero Commit failed, changes rolled back.
 */
ULONG
TraceRoute_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending parameter changes whenever there's a validation found.
 *
 * @param[in] hInsContext Instance handle for the TraceRoute context.
 *
 * @retval 0 Rollback succeeded.
 */
ULONG
TraceRoute_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.TraceRoute.RouteHops.{i}.

    *  RouteHops_GetEntryCount
    *  RouteHops_GetEntry
    *  RouteHops_IsUpdated
    *  RouteHops_Synchronize
    *  RouteHops_GetParamBoolValue
    *  RouteHops_GetParamIntValue
    *  RouteHops_GetParamUlongValue
    *  RouteHops_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of RouteHops entries for TraceRoute diagnostics.
 *
 * @param[in] hInsContext Instance handle for the TraceRoute context.
 *
 * @return The count of RouteHops entries.
 */
ULONG
RouteHops_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific RouteHops entry by index.
 *
 * @param[in]  hInsContext  Instance handle for the TraceRoute context.
 * @param[in]  nIndex       Index of the RouteHops entry to retrieve (0-based).
 * @param[out] pInsNumber   Pointer to store the instance number of the entry.
 *
 * @return Handle to the RouteHops entry
 * @retval Handle to the specific RouteHops entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
RouteHops_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if RouteHops entries have been updated.
 *
 * @param[in] hInsContext Instance handle for the TraceRoute context.
 *
 * @return status of update.
 * @retval TRUE if entries have been updated
 * @retval FALSE otherwise.
 */
BOOL
RouteHops_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes RouteHops entries with the system.
 *
 * @param[in] hInsContext Instance handle for the TraceRoute context.
 *
 * @retval 0 Synchronization succeeded.
 */
ULONG
RouteHops_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for RouteHops entry.
 *
 * @param[in]  hInsContext Instance handle for the RouteHops entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
RouteHops_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for RouteHops entry.
 *
 * @param[in]  hInsContext Instance handle for the RouteHops entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
RouteHops_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for RouteHops entry.
 *
 * @param[in]  hInsContext Instance handle for the RouteHops entry.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
RouteHops_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for RouteHops entry.
 *
 * @param[in]     hInsContext Instance handle for the RouteHops entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
RouteHops_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.DownloadDiagnostics.

    *  DownloadDiagnostics_GetParamBoolValue
    *  DownloadDiagnostics_GetParamIntValue
    *  DownloadDiagnostics_GetParamUlongValue
    *  DownloadDiagnostics_GetParamStringValue
    *  DownloadDiagnostics_SetParamBoolValue
    *  DownloadDiagnostics_SetParamIntValue
    *  DownloadDiagnostics_SetParamUlongValue
    *  DownloadDiagnostics_SetParamStringValue
    *  DownloadDiagnostics_Validate
    *  DownloadDiagnostics_Commit
    *  DownloadDiagnostics_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for Download Diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the DownloadDiagnostics context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
DownloadDiagnostics_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for Download Diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the DownloadDiagnostics context.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
DownloadDiagnostics_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for Download Diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the DownloadDiagnostics context.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
DownloadDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for Download Diagnostics.
 *
 * @param[in]     hInsContext Instance handle for the DownloadDiagnostics context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 512 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string. Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
DownloadDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for Download Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the DownloadDiagnostics context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
DownloadDiagnostics_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Download Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the DownloadDiagnostics context.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
DownloadDiagnostics_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for Download Diagnostics.
 *
 * @param[in] hInsContext   Instance handle for the DownloadDiagnostics context.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
DownloadDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for Download Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the DownloadDiagnostics context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 512 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
DownloadDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending parameter changes for Download Diagnostics.
 *
 * @param[in]     hInsContext       Instance handle for the DownloadDiagnostics context.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of returned parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation
 * @retval FALSE if there's validation.
 */
BOOL
DownloadDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending parameter changes for Download Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the DownloadDiagnostics context.
 *
 * @retval 0       Commit succeeded.
 * @retval nonzero Commit failed, changes rolled back.
 */
ULONG
DownloadDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending parameter changes whenever there's a validation found for Download Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the DownloadDiagnostics context.
 *
 * @retval 0 Rollback succeeded.
 */
ULONG
DownloadDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.UploadDiagnostics.

    *  UploadDiagnostics_GetParamBoolValue
    *  UploadDiagnostics_GetParamIntValue
    *  UploadDiagnostics_GetParamUlongValue
    *  UploadDiagnostics_GetParamStringValue
    *  UploadDiagnostics_SetParamBoolValue
    *  UploadDiagnostics_SetParamIntValue
    *  UploadDiagnostics_SetParamUlongValue
    *  UploadDiagnostics_SetParamStringValue
    *  UploadDiagnostics_Validate
    *  UploadDiagnostics_Commit
    *  UploadDiagnostics_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for Upload Diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the UploadDiagnostics context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
UploadDiagnostics_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for Upload Diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the UploadDiagnostics context.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
UploadDiagnostics_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for Upload Diagnostics.
 *
 * @param[in]  hInsContext Instance handle for the UploadDiagnostics context.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
UploadDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for Upload Diagnostics.
 *
 * @param[in]     hInsContext Instance handle for the UploadDiagnostics context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 512 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
UploadDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for Upload Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the UploadDiagnostics context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
UploadDiagnostics_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for Upload Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the UploadDiagnostics context.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
UploadDiagnostics_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for Upload Diagnostics.
 *
 * @param[in] hInsContext   Instance handle for the UploadDiagnostics context.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
UploadDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for Upload Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the UploadDiagnostics context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 512 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
UploadDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending parameter changes for Upload Diagnostics.
 *
 * @param[in]     hInsContext       Instance handle for the UploadDiagnostics context.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the parameter name if there's a validation .
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of returned parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation
 * @retval FALSE if there's validation.
 */
BOOL
UploadDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending parameter changes for Upload Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the UploadDiagnostics context.
 *
 * @retval 0       Commit succeeded.
 * @retval nonzero Commit failed, changes rolled back.
 */
ULONG
UploadDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending parameter changes whenever there's a validation found for Upload Diagnostics.
 *
 * @param[in] hInsContext Instance handle for the UploadDiagnostics context.
 *
 * @retval 0 Rollback succeeded.
 */
ULONG
UploadDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.UDPEchoConfig.

    *  UDPEchoConfig_GetParamBoolValue
    *  UDPEchoConfig_GetParamIntValue
    *  UDPEchoConfig_GetParamUlongValue
    *  UDPEchoConfig_GetParamStringValue
    *  UDPEchoConfig_SetParamBoolValue
    *  UDPEchoConfig_SetParamIntValue
    *  UDPEchoConfig_SetParamUlongValue
    *  UDPEchoConfig_SetParamStringValue
    *  UDPEchoConfig_Validate
    *  UDPEchoConfig_Commit
    *  UDPEchoConfig_Rollback

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for UDP Echo Config.
 *
 * @param[in]  hInsContext Instance handle for the UDPEchoConfig context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
UDPEchoConfig_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for UDP Echo Config.
 *
 * @param[in]  hInsContext Instance handle for the UDPEchoConfig context.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
UDPEchoConfig_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for UDP Echo Config.
 *
 * @param[in]  hInsContext Instance handle for the UDPEchoConfig context.
 * @param[in]  ParamName   Name of the unsigned long parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
UDPEchoConfig_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for UDP Echo Config.
 *
 * @param[in]     hInsContext Instance handle for the UDPEchoConfig context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of returned string.  Usually size of 1023 will be used.
 *
 * @return status of operation
 * @retval 0  Success.
 * @retval 1  Insufficient buffer size.
 * @retval -1 Parameter not supported or other error.
 */
ULONG
UDPEchoConfig_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for UDP Echo Config.
 *
 * @param[in] hInsContext Instance handle for the UDPEchoConfig context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
UDPEchoConfig_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for UDP Echo Config.
 *
 * @param[in] hInsContext Instance handle for the UDPEchoConfig context.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
UDPEchoConfig_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for UDP Echo Config.
 *
 * @param[in] hInsContext   Instance handle for the UDPEchoConfig context.
 * @param[in] ParamName     Name of the unsigned long parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong  Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
UDPEchoConfig_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for UDP Echo Config.
 *
 * @param[in] hInsContext Instance handle for the UDPEchoConfig context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
UDPEchoConfig_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending parameter changes for UDP Echo Config.
 *
 * @param[in]     hInsContext       Instance handle for the UDPEchoConfig context.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of returned parameter name.
 *
 * @return status of operation
 * @retval TRUE if there's no validation
 * @retval FALSE f there's validation.
 */
BOOL
UDPEchoConfig_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending parameter changes for UDP Echo Config.
 *
 * @param[in] hInsContext Instance handle for the UDPEchoConfig context.
 *
 * @retval 0       Commit succeeded.
 * @retval nonzero Commit failed, changes rolled back.
 */
ULONG
UDPEchoConfig_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending parameter changes whenever there's a validation found for UDP Echo Config.
 *
 * @param[in] hInsContext Instance handle for the UDPEchoConfig context.
 *
 * @retval 0 Rollback succeeded.
 */
ULONG
UDPEchoConfig_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
