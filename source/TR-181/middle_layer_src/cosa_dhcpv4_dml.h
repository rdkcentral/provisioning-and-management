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

    module: cosa_dhcpv4_dml.h

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


#ifndef  _COSA_DHCPV4_DML_H
#define  _COSA_DHCPV4_DML_H

#include "cosa_apis.h"

/***********************************************************************

 APIs for Object:

    DHCPv4.

    *  DHCPv4_GetParamBoolValue
    *  DHCPv4_GetParamIntValue
    *  DHCPv4_GetParamUlongValue
    *  DHCPv4_GetParamStringValue

***********************************************************************/
/**
 * @brief Get a boolean parameter from the DHCPv4 object
 *
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
DHCPv4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter from the DHCPv4 object
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pInt        Pointer to store the integer value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
DHCPv4_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter from the DHCPv4 object
 *
 * Retrieves an unsigned long parameter from the Device.DHCPv4 TR-181 data model object.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pUlong      Pointer to store the unsigned long value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
*/
BOOL
DHCPv4_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter from the DHCPv4 object
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
DHCPv4_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#ifndef FEATURE_RDKB_WAN_MANAGER
/***********************************************************************

 APIs for Object:

    DHCPv4.Client.{i}.

    *  Client_GetEntryCount
    *  Client_GetEntry
    *  Client_AddEntry
    *  Client_DelEntry
    *  Client_GetParamBoolValue
    *  Client_GetParamIntValue
    *  Client_GetParamUlongValue
    *  Client_GetParamStringValue
    *  Client_SetParamBoolValue
    *  Client_SetParamIntValue
    *  Client_SetParamUlongValue
    *  Client_SetParamStringValue
    *  Client_Validate
    *  Client_Commit
    *  Client_Rollback

***********************************************************************/
/**
 * @brief Get the count of DHCPv4 client table entries
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The total number of DHCPv4 client entries
 */
ULONG
Client_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a DHCPv4 client entry by index
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] nIndex       Zero-based index of the entry to retrieve
 * @param[out] pInsNumber  Instance number of the retrieved entry
 *
 * @return Handle to the DHCPv4 client entry.
 * @retval Handle to the client entry context
 * @retval NULL if not found
 */
ANSC_HANDLE
Client_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[out] pInsNumber  Instance number assigned to the new entry
 *
 * @return Handle to the DHCPv4 client entry.
 * @retval Handle to the newly added client entry
 * @retval NULL on failure
 */
ANSC_HANDLE
Client_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] hInstance    Handle to the client entry to delete
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
Client_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get a boolean parameter from a DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter from a DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pInt        Pointer to store the integer value
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter from a DHCPv4 client entry
 *
 * Retrieves an unsigned long parameter (such as Status) from a
 * Device.DHCPv4.Client table entry.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pUlong      Pointer to store the unsigned long value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Client_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter from a DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
Client_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set a boolean parameter for a DHCPv4 client entry
 *
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       Boolean value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set an integer parameter for a DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] value        Integer value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set an unsigned long parameter for a DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] uValuepUlong Unsigned long value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set a string parameter for a DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] strValue     String value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate DHCPv4 client entry parameters before commit
 *
 * @param[in] hInsContext       Instance context handle
 * @param[out] pReturnParamName Buffer(128 bytes) to store the name of parameter if there's a validation.
 * @param[in,out] puLength      Size of the pReturnParamName buffer
 *
 * @return The status of validation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation
 */
BOOL
Client_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit changes to a DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
Client_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback changes to a DHCPv4 client entry
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
Client_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Client.{i}.SentOption.{i}.

    *  SentOption_GetEntryCount
    *  SentOption_GetEntry
    *  SentOption_AddEntry
    *  SentOption_DelEntry
    *  SentOption_GetParamBoolValue
    *  SentOption_GetParamIntValue
    *  SentOption_GetParamUlongValue
    *  SentOption_GetParamStringValue
    *  SentOption_SetParamBoolValue
    *  SentOption_SetParamIntValue
    *  SentOption_SetParamUlongValue
    *  SentOption_SetParamStringValue
    *  SentOption_Validate
    *  SentOption_Commit
    *  SentOption_Rollback

***********************************************************************/
/**
 * @brief Get the count of DHCPv4 client sent option entries
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The total number of sent option entries
 */
ULONG
SentOption_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a DHCPv4 client sent option entry by index
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] nIndex       Zero-based index of the entry
 * @param[out] pInsNumber  Instance number of the retrieved entry
 *
 * @return The handle to identify the sent option entry
 * @retval Handle to the sent option entry
 * @retval NULL if not found
 */
ANSC_HANDLE
SentOption_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new DHCPv4 client sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[out] pInsNumber  Instance number assigned to the new entry
 *
 * @return The handle to identify the newly created sent option entry
 * @retval Handle to the newly created sent option entry
 * @retval NULL on failure
 */
ANSC_HANDLE
SentOption_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a DHCPv4 client sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] hInstance    Handle to the sent option entry to delete
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
SentOption_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get a boolean parameter from a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
SentOption_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter from a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pInt        Pointer to store the integer value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
SentOption_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter from a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pUlong      Pointer to store the unsigned long value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
SentOption_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter from a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value.
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
SentOption_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set a boolean parameter for a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       Boolean value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
SentOption_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set an integer parameter for a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] value        Integer value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
SentOption_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set an unsigned long parameter for a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] uValuepUlong Unsigned long value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
SentOption_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set a string parameter for a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] strValue     String value to set (maximum 256 bytes)
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
SentOption_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate sent option entry parameters before commit
 *
 * Validates all parameters set for a Device.DHCPv4.Client.{i}.SentOption entry.
 *
 * @param[in] hInsContext       Instance context handle
 * @param[out] pReturnParamName Buffer to store the name of parameter (buffer size 128 bytes) if there's a validation.
 * @param[in,out] puLength      Size of the pReturnParamName buffer
 *
 * @return The status of validation
 * @retval TRUE if if there's no validation
 * @retval FALSE if there's validation
 */
BOOL
SentOption_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit changes to a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
SentOption_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback changes to a sent option entry
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
SentOption_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Client.{i}.ReqOption.{i}.

    *  ReqOption_GetEntryCount
    *  ReqOption_GetEntry
    *  ReqOption_AddEntry
    *  ReqOption_DelEntry
    *  ReqOption_GetParamBoolValue
    *  ReqOption_GetParamIntValue
    *  ReqOption_GetParamUlongValue
    *  ReqOption_GetParamStringValue
    *  ReqOption_SetParamBoolValue
    *  ReqOption_SetParamIntValue
    *  ReqOption_SetParamUlongValue
    *  ReqOption_SetParamStringValue
    *  ReqOption_Validate
    *  ReqOption_Commit
    *  ReqOption_Rollback

***********************************************************************/
/**
 * @brief Get the count of DHCPv4 client requested option entries
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The total number of requested option entries
 */
ULONG
ReqOption_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a DHCPv4 client requested option entry by index
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] nIndex       Zero-based index of the entry
 * @param[out] pInsNumber  Instance number of the retrieved entry
 *
 * @return Handle to the option entry
 * @retval Handle to the client requested option entry
 * @retval NULL if not found
 */
ANSC_HANDLE
ReqOption_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new DHCPv4 client requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[out] pInsNumber  Instance number assigned to the new entry
 *
 * @return Handle to the newly added option entry
 * @retval Handle to the new client requested option entry
 * @retval NULL on failure
 */
ANSC_HANDLE
ReqOption_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a DHCPv4 client requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] hInstance    Handle to the requested option entry to delete
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
ReqOption_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get a boolean parameter from a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ReqOption_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter from a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pInt        Pointer to store the integer value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ReqOption_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter from a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pUlong      Pointer to store the unsigned long value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ReqOption_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter from a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value (minimum 256 bytes)
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
ReqOption_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set a boolean parameter for a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       Boolean value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
ReqOption_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set an integer parameter for a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] value        Integer value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
ReqOption_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set an unsigned long parameter for a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] uValuepUlong Unsigned long value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
ReqOption_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set a string parameter for a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] strValue     String value to set (maximum 256 bytes)
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
ReqOption_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate requested option entry parameters before commit
 *
 * @param[in] hInsContext       Instance context handle
 * @param[out] pReturnParamName Buffer to store the name of parameter there's a validation. (buffer size 128 bytes)
 * @param[in,out] puLength      Size of the pReturnParamName buffer
 *
 * @return The status of validation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation
 */
BOOL
ReqOption_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit changes to a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
ReqOption_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback changes to a requested option entry
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
ReqOption_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );
#endif
/***********************************************************************

 APIs for Object:

    DHCPv4.Server.

    *  Server_GetParamBoolValue
    *  Server_GetParamIntValue
    *  Server_GetParamUlongValue
    *  Server_GetParamStringValue
    *  Server_SetParamBoolValue
    *  Server_SetParamIntValue
    *  Server_SetParamUlongValue
    *  Server_SetParamStringValue
    *  Server_Validate
    *  Server_Commit
    *  Server_Rollback

***********************************************************************/
/**
 * @brief Get a boolean parameter from DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Server_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter from DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pInt        Pointer to store the integer value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Server_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter from DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pUlong      Pointer to store the unsigned long value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Server_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter from DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value (minimum 256 bytes)
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
Server_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set a boolean parameter for DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       Boolean value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
Server_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set an integer parameter for DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] value        Integer value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
Server_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set an unsigned long parameter for DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] uValuepUlong Unsigned long value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
Server_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set a string parameter for DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] strValue     String value to set (maximum 256 bytes)
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
Server_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate DHCPv4 server parameters before commit
 *
 * Validates all parameters set for Device.DHCPv4.Server.
 *
 * @param[in] hInsContext       Instance context handle
 * @param[out] pReturnParamName Buffer to store the name of parameter if there's a validation(buffer size 256 bytes)
 * @param[in,out] puLength      Size of the pReturnParamName buffer
 *
 * @return The status of validation
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation
 */
BOOL
Server_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit changes to DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
Server_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback changes to DHCPv4 server
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
Server_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    DHCPv4.Server.X_COM_CISCO_StaticAddress.{i}.

    *  X_COM_CISCO_StaticAddress_GetEntryCount
    *  X_COM_CISCO_StaticAddress_GetEntry
    *  X_COM_CISCO_StaticAddress_AddEntry
    *  X_COM_CISCO_StaticAddress_DelEntry
    *  X_COM_CISCO_StaticAddress_GetParamBoolValue
    *  X_COM_CISCO_StaticAddress_GetParamIntValue
    *  X_COM_CISCO_StaticAddress_GetParamUlongValue
    *  X_COM_CISCO_StaticAddress_GetParamStringValue
    *  X_COM_CISCO_StaticAddress_SetParamBoolValue
    *  X_COM_CISCO_StaticAddress_SetParamIntValue
    *  X_COM_CISCO_StaticAddress_SetParamUlongValue
    *  X_COM_CISCO_StaticAddress_SetParamStringValue
    *  X_COM_CISCO_StaticAddress_Validate
    *  X_COM_CISCO_StaticAddress_Commit
    *  X_COM_CISCO_StaticAddress_Rollback

***********************************************************************/
/**
 * @brief Get the count of Cisco static address entries
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The total number of static address entries
 */
ULONG
X_COM_CISCO_StaticAddress_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a Cisco static address entry by index
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] nIndex       Zero-based index of the entry
 * @param[out] pInsNumber  Instance number of the retrieved entry
 *
 * @return Handle to the cisco static address entry
 * @retval Handle to the static address entry
 * @retval NULL if not found
 */
ANSC_HANDLE
X_COM_CISCO_StaticAddress_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[out] pInsNumber  Instance number assigned to the new entry
 *
 * @return Handle to the newly created static address entry
 * @retval Handle to the newly created address entry
 * @retval NULL on failure
 */
ANSC_HANDLE
X_COM_CISCO_StaticAddress_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Delete a Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] hInstance    Handle to the static address entry to delete
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
X_COM_CISCO_StaticAddress_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Get a boolean parameter from Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
X_COM_CISCO_StaticAddress_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter from Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pInt        Pointer to store the integer value
 *
 * @return The status of operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
X_COM_CISCO_StaticAddress_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter from Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pUlong      Pointer to store the unsigned long value
 *
 * @return The status of operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
X_COM_CISCO_StaticAddress_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter from Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value (minimum 256 bytes)
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
X_COM_CISCO_StaticAddress_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set a boolean parameter for Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       Boolean value to set
 *
 * @return The status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
X_COM_CISCO_StaticAddress_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set an integer parameter for Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] value        Integer value to set
 *
 * @return The status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
X_COM_CISCO_StaticAddress_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set an unsigned long parameter for Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] uValuepUlong Unsigned long value to set
 *
 * @return The status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
X_COM_CISCO_StaticAddress_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set a string parameter for Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] strValue     String value to set (maximum 256 bytes)
 *
 * @return The status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
X_COM_CISCO_StaticAddress_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate Cisco static address entry parameters before commit
 *
 * @param[in] hInsContext       Instance context handle
 * @param[out] pReturnParamName Buffer(128 bytes) to store the name of parameter if there's a validation.
 * @param[in,out] puLength      Size of the pReturnParamName buffer
 *
 * @return The status of validation
 * @retval TRUE if there's no validation
 * @retval FALSE if there's validation
 */
BOOL
X_COM_CISCO_StaticAddress_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit changes to Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
X_COM_CISCO_StaticAddress_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback changes to Cisco static address entry
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of operation
 * @retval 0 on success
 * @retval error code otherwise
 */
ULONG
X_COM_CISCO_StaticAddress_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.

    *  Pool_GetEntryCount
    *  Pool_GetEntry
    *  Pool_AddEntry
    *  Pool_DelEntry
    *  Pool_GetParamBoolValue
    *  Pool_GetParamIntValue
    *  Pool_GetParamUlongValue
    *  Pool_GetParamStringValue
    *  Pool_SetParamBoolValue
    *  Pool_SetParamIntValue
    *  Pool_SetParamUlongValue
    *  Pool_SetParamStringValue
    *  Pool_Validate
    *  Pool_Commit
    *  Pool_Rollback

***********************************************************************/
/**
 * @brief Get the count of DHCPv4 server pool entries
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The total number of pool entries
 */
ULONG
Pool_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a DHCPv4 server pool entry by index
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] nIndex       Zero-based index of the entry
 * @param[out] pInsNumber  Instance number of the retrieved entry
 *
 * @return Handle to the DHCPV4 server pool entry
 * @retval Handle to the server pool entry
 * @retval NULL if not found
 */
ANSC_HANDLE
Pool_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Add a new DHCPv4 server pool entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[out] pInsNumber  Instance number assigned to the new entry
 *
 * @return Handle to the newly created pool entry
 * @retval Handle to the newly created server pool entry
 * @retval NULL on failure
 */
ANSC_HANDLE
Pool_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a DHCPv4 server pool entry from the Device.DHCPv4.Server.Pool.{i} table.
 *
 * @param[in] hInsContext Instance handle for the pool table context.
 * @param[in] hInstance   Handle to the specific pool entry to be deleted.
 *
 * @return The status of operation.
 * @retval ANSC_STATUS_SUCCESS on successful deletion
 * @retval error status.
 */
ULONG
Pool_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv4 server pool entry.
 *
 * @param[in]  hInsContext Instance handle for the pool entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv4 server pool entry.
 *
 * @param[in]  hInsContext Instance handle for the pool entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv4 server pool entry.
 *
 * @param[in]  hInsContext Instance handle for the pool entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv4 server pool entry.
 *
 * @param[in]     hInsContext Instance handle for the pool entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation.
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
Pool_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for a DHCPv4 server pool entry.
 *
 * @param[in] hInsContext Instance handle for the pool entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for a DHCPv4 server pool entry.
 *
 * @param[in] hInsContext Instance handle for the pool entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for a DHCPv4 server pool entry.
 *
 * @param[in] hInsContext  Instance handle for the pool entry.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for a DHCPv4 server pool entry.
 *
 * @param[in] hInsContext Instance handle for the pool entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Pool_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for a DHCPv4 server pool entry before committing.
 *
 * @param[in]     hInsContext       Instance handle for the pool entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return The status of validation
 * @retval TRUE if if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Pool_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for a DHCPv4 server pool entry to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the pool entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful commit
 * @retval error status.
 */
ULONG
Pool_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for a DHCPv4 server pool entry.
 *
 * @param[in] hInsContext Instance handle for the pool entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful rollback
 * @retval error status.
 */
ULONG
Pool_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.StaticAddress.{i}.

    *  StaticAddress_GetEntryCount
    *  StaticAddress_GetEntry
    *  StaticAddress_AddEntry
    *  StaticAddress_IsUpdated
    *  StaticAddress_Synchronize
    *  StaticAddress_DelEntry
    *  StaticAddress_GetParamBoolValue
    *  StaticAddress_GetParamIntValue
    *  StaticAddress_GetParamUlongValue
    *  StaticAddress_GetParamStringValue
    *  StaticAddress_SetParamBoolValue
    *  StaticAddress_SetParamIntValue
    *  StaticAddress_SetParamUlongValue
    *  StaticAddress_SetParamStringValue
    *  StaticAddress_Validate
    *  StaticAddress_Commit
    *  StaticAddress_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of static address entries in the Device.DHCPv4.Server.Pool.{i}.StaticAddress.{i} table.
 *
 * @param[in] hInsContext Instance handle for the pool context.
 *
 * @return The count of static address entries.
 */
ULONG
StaticAddress_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific static address entry from the Device.DHCPv4.Server.Pool.{i}.StaticAddress.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the pool context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the static address entry
 * @retval Handle to the specific static address entry.
 * @retval NULL if not found.
 */
ANSC_HANDLE
StaticAddress_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the static address table has been updated.
 *
 * @param[in] hInsContext Instance handle for the pool context.
 *
 * @return The status of update check.
 * @retval TRUE if the table has been updated
 * @retval FALSE otherwise.
 */
BOOL
StaticAddress_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the static address table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the pool context.
 *
 * @return The status of synchronization.
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error status.
 */
ULONG
StaticAddress_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );


/**
 * @brief Adds a new static address entry to the Device.DHCPv4.Server.Pool.{i}.StaticAddress.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the pool context.
 * @param[out] pInsNumber  Pointer to store the instance number of the newly created entry.
 *
 * @return Handle to the newly created static address entry
 * @retval Handle to the newly added static address entry.
 * @retval NULL on failure.
 */
ANSC_HANDLE
StaticAddress_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a static address entry from the Device.DHCPv4.Server.Pool.{i}.StaticAddress.{i} table.
 *
 * @param[in] hInsContext Instance handle for the pool context.
 * @param[in] hInstance   Handle to the specific static address entry to be deleted.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful deletion
 * @retval error status.
 */
ULONG
StaticAddress_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for a static address entry.
 *
 * @param[in]  hInsContext Instance handle for the static address entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
StaticAddress_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a static address entry.
 *
 * @param[in]  hInsContext Instance handle for the static address entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
StaticAddress_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a static address entry.
 *
 * @param[in]  hInsContext Instance handle for the static address entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
StaticAddress_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a static address entry.
 *
 * @param[in]     hInsContext Instance handle for the static address entry.
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
StaticAddress_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for a static address entry.
 *
 * @param[in] hInsContext Instance handle for the static address entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
StaticAddress_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for a static address entry.
 *
 * @param[in] hInsContext Instance handle for the static address entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
StaticAddress_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for a static address entry.
 *
 * @param[in] hInsContext  Instance handle for the static address entry.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
StaticAddress_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for a static address entry.
 *
 * @param[in] hInsContext Instance handle for the static address entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
StaticAddress_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for a static address entry before committing.
 *
 * @param[in]     hInsContext       Instance handle for the static address entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return The status of validation
 * @retval TRUE if there is no validation
 * @retval FALSE if there is validation.
 */
BOOL
StaticAddress_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for a static address entry to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the static address entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful commit
 * @retval error status on failure
 */
ULONG
StaticAddress_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for a static address entry.
 *
 * @param[in] hInsContext Instance handle for the static address entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful rollback
 * @retval error status on failure
 */
ULONG
StaticAddress_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Option.{i}.

    *  Option1_GetEntryCount
    *  Option1_GetEntry
    *  Option1_AddEntry
    *  Option1_DelEntry
    *  Option1_GetParamBoolValue
    *  Option1_GetParamIntValue
    *  Option1_GetParamUlongValue
    *  Option1_GetParamStringValue
    *  Option1_SetParamBoolValue
    *  Option1_SetParamIntValue
    *  Option1_SetParamUlongValue
    *  Option1_SetParamStringValue
    *  Option1_Validate
    *  Option1_Commit
    *  Option1_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of option entries in the Device.DHCPv4.Server.Pool.{i}.Option.{i} table.
 *
 * @param[in] hInsContext Instance handle for the pool context.
 *
 * @return The count of option entries.
 */
ULONG
Option1_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves the entry specified by the index.
 *
 * @param[in]  hInsContext Instance handle for the pool context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to specific option entry
 * @retval Handle to the option entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
Option1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new DHCPv4 option entry to the Device.DHCPv4.Server.Pool.{i}.Option.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the pool context.
 * @param[out] pInsNumber  Pointer to store the instance number of the newly created entry.
 *
 * @return Handle to the newly created option entry
 * @retval Handle to the newly created DHCPV4 option entry
 * @retval NULL on failure.
 */
ANSC_HANDLE
Option1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a DHCPv4 option entry from the Device.DHCPv4.Server.Pool.{i}.Option.{i} table.
 *
 * @param[in] hInsContext Instance handle for the pool context.
 * @param[in] hInstance   Handle to the specific option entry to be deleted.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful deletion
 * @retval error status.
 */
ULONG
Option1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv4 option entry.
 *
 * @param[in]  hInsContext Instance handle for the option entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv4 option entry.
 *
 * @param[in]  hInsContext Instance handle for the option entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv4 option entry.
 *
 * @param[in]  hInsContext Instance handle for the option entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv4 option entry.
 *
 * @param[in]     hInsContext Instance handle for the option entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve.
 * @param[out]    pValue      Buffer to store the retrieved string value
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation.
 * @retval 0 on success.
 * @retval 1 if buffer size is insufficient.
 * @retval -1 if parameter not supported.
 */
ULONG
Option1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for a DHCPv4 option entry.
 *
 * @param[in] hInsContext Instance handle for the option entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and set successfully.
 * @retval FALSE otherwise.
 */
BOOL
Option1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for a DHCPv4 option entry.
 *
 * @param[in] hInsContext Instance handle for the option entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and set successfully.
 * @retval FALSE otherwise.
 */
BOOL
Option1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for a DHCPv4 option entry.
 *
 * @param[in] hInsContext  Instance handle for the option entry.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and set successfully.
 * @retval FALSE otherwise.
 */
BOOL
Option1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for a DHCPv4 option entry.
 *
 * @param[in] hInsContext Instance handle for the option entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and set successfully.
 * @retval FALSE otherwise.
 */
BOOL
Option1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for a DHCPv4 option entry before committing.
 *
 * @param[in]     hInsContext       Instance handle for the option entry.
 * @param[out]    pReturnParamName  Buffer (128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return The status of validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Option1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for a DHCPv4 option entry to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the option entry.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful commit.
 * @retval error status.
 */
ULONG
Option1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for a DHCPv4 option entry.
 *
 * @param[in] hInsContext Instance handle for the option entry.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful rollback.
 * @retval error status.
 */
ULONG
Option1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Client.{i}.

    *  Client2_GetEntryCount
    *  Client2_GetEntry
    *  Client2_IsUpdated
    *  Client2_Synchronize
    *  Client2_GetParamBoolValue
    *  Client2_GetParamIntValue
    *  Client2_GetParamUlongValue
    *  Client2_GetParamStringValue
    *  Client2_SetParamBoolValue
    *  Client2_SetParamIntValue
    *  Client2_SetParamUlongValue
    *  Client2_SetParamStringValue
    *  Client2_Validate
    *  Client2_Commit
    *  Client2_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of client entries in the Device.DHCPv4.Server.Pool.{i}.Client.{i} table.
 *
 * @param[in] hInsContext Instance handle for the pool context.
 *
 * @return The count of client entries in the pool.
 */
ULONG
Client2_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific client entry from the Device.DHCPv4.Server.Pool.{i}.Client.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the pool context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the client entry.
 * @retval Handle to the specific client entry.
 * @retval NULL if not found.
 */
ANSC_HANDLE
Client2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the client table has been updated.
 *
 * @param[in] hInsContext Instance handle for the pool context.
 *
 * @return The status of update check.
 * @retval TRUE if the table has been updated.
 * @retval FALSE otherwise.
 */
BOOL
Client2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the client table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the pool context.
 *
 * @return The status of synchronization.
 * @retval ANSC_STATUS_SUCCESS on successful synchronization.
 * @retval error status.
 */
ULONG
Client2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for a DHCPv4 server pool client entry.
 *
 * @param[in]  hInsContext Instance handle for the client entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a DHCPv4 server pool client entry.
 *
 * @param[in]  hInsContext Instance handle for the client entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a DHCPv4 server pool client entry.
 *
 * @param[in]  hInsContext Instance handle for the client entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Client2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a DHCPv4 server pool client entry.
 *
 * @param[in]     hInsContext Instance handle for the client entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation.
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
Client2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for a DHCPv4 server pool client entry.
 *
 * @param[in] hInsContext Instance handle for the client entry.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets integer parameter values for a DHCPv4 server pool client entry.
 *
 * @param[in] hInsContext Instance handle for the client entry.
 * @param[in] ParamName   Name of the integer parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] value       Integer value to set.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets unsigned long parameter values for a DHCPv4 server pool client entry.
 *
 * @param[in] hInsContext  Instance handle for the client entry.
 * @param[in] ParamName    Name of the ulong parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets string parameter values for a DHCPv4 server pool client entry.
 *
 * @param[in] hInsContext Instance handle for the client entry.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] strValue    String value to set (minimum 0 bytes, maximum 256 bytes).
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Client2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates parameter changes for a DHCPv4 server pool client entry before committing.
 *
 * @param[in]     hInsContext       Instance handle for the client entry.
 * @param[out]    pReturnParamName  Buffer(128 bytes) to store the name of the parameter if there's a validation.
 * @param[in,out] puLength          Input: Size of pReturnParamName buffer; Output: Length of parameter name.
 *
 * @return The status of validation
 * @retval TRUE if there's no validation
 * @retval FALSE if there's validation.
 */
BOOL
Client2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits validated parameter changes for a DHCPv4 server pool client entry to persistent storage.
 *
 * @param[in] hInsContext Instance handle for the client entry.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful commit
 * @retval error status.

 */
ULONG
Client2_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back uncommitted parameter changes for a DHCPv4 server pool client entry.
 *
 * @param[in] hInsContext Instance handle for the client entry.
 *
 * @return  The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful rollback
 * @retval error status.
 */
ULONG
Client2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Client.{i}.IPv4Address.{i}.

    *  IPv4Address2_GetEntryCount
    *  IPv4Address2_GetEntry
    *  IPv4Address2_IsUpdated
    *  IPv4Address2_Synchronize
    *  IPv4Address2_GetParamBoolValue
    *  IPv4Address2_GetParamIntValue
    *  IPv4Address2_GetParamUlongValue
    *  IPv4Address2_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of IPv4 address entries in the Device.DHCPv4.Server.Pool.{i}.Client.{i}.IPv4Address.{i} table.
 *
 * @param[in] hInsContext Instance handle for the client context.
 *
 * @return The count of IPv4 address entries for the client.
 */
ULONG
IPv4Address2_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific IPv4 address entry from the Device.DHCPv4.Server.Pool.{i}.Client.{i}.IPv4Address.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the client context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to specific IPv4 address entry
 * @retval Handle to the IPv4 address entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
IPv4Address2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the IPv4 address table has been updated.
 *
 * @param[in] hInsContext Instance handle for the client context.
 *
 * @return The status of update check.
 * @retval TRUE if the table has been updated
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the IPv4 address table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the client context.
 *
 * @return The status of synchronization.
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error status.
 */
ULONG
IPv4Address2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for a client IPv4 address entry.
 *
 * @param[in]  hInsContext Instance handle for the IPv4 address entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return  The status of operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a client IPv4 address entry.
 *
 * @param[in]  hInsContext Instance handle for the IPv4 address entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a client IPv4 address entry.
 *
 * @param[in]  hInsContext Instance handle for the IPv4 address entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation.
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv4Address2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a client IPv4 address entry.
 *
 * @param[in]     hInsContext Instance handle for the IPv4 address entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation.
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
IPv4Address2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Client.{i}.Option.{i}.

    *  Option2_GetEntryCount
    *  Option2_GetEntry
    *  Option2_IsUpdated
    *  Option2_Synchronize
    *  Option2_GetParamBoolValue
    *  Option2_GetParamIntValue
    *  Option2_GetParamUlongValue
    *  Option2_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of option entries in the Device.DHCPv4.Server.Pool.{i}.Client.{i}.Option.{i} table.
 *
 * @param[in] hInsContext Instance handle for the client context.
 *
 * @return The count of option entries for the client.
 */
ULONG
Option2_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a specific option entry from the Device.DHCPv4.Server.Pool.{i}.Client.{i}.Option.{i} table.
 *
 * @param[in]  hInsContext Instance handle for the client context.
 * @param[in]  nIndex      Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber  Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to specific client option entry
 * @retval Handle to the client option entry
 * @retval NULL if not found.
 */
ANSC_HANDLE
Option2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the client option table has been updated.
 *
 * @param[in] hInsContext Instance handle for the client context.
 *
 * @return The status of update check.
 * @retval TRUE if the table has been updated
 * @retval FALSE otherwise.
 */
BOOL
Option2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the client option table with the backend data store.
 *
 * @param[in] hInsContext Instance handle for the client context.
 *
 * @return The status of operation
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error status.
 */
ULONG
Option2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves boolean parameter values for a client option entry.
 *
 * @param[in]  hInsContext Instance handle for the client option entry.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves integer parameter values for a client option entry.
 *
 * @param[in]  hInsContext Instance handle for the client option entry.
 * @param[in]  ParamName   Name of the integer parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pInt        Pointer to store the retrieved integer value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves unsigned long parameter values for a client option entry.
 *
 * @param[in]  hInsContext Instance handle for the client option entry.
 * @param[in]  ParamName   Name of the ulong parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pUlong      Pointer to store the retrieved unsigned long value.
 *
 * @return The status of operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Option2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves string parameter values for a client option entry.
 *
 * @param[in]     hInsContext Instance handle for the client option entry.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 256 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of operation.
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported
 */
ULONG
Option2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
