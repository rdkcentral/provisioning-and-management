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

    module: cosa_users_dml.h

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


#ifndef  _COSA_USERS_DML_H
#define  _COSA_USERS_DML_H

#include "mso_mgmt_hal.h"

/***********************************************************************

 APIs for Object:

    Users.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    Users.User.{i}.

    *  User_GetEntryCount
    *  User_GetEntry
    *  User_AddEntry
    *  User_DelEntry
    *  User_GetParamBoolValue
    *  User_GetParamIntValue
    *  User_GetParamUlongValue
    *  User_GetParamStringValue
    *  User_SetParamBoolValue
    *  User_SetParamIntValue
    *  User_SetParamUlongValue
    *  User_SetParamStringValue
    *  User_Validate
    *  User_Commit
    *  User_Rollback

***********************************************************************/
/**
 * @brief Retrieves the number of User entries in the Device.Users.User table.
 *
 * This function returns the count of user accounts configured on the device.
 *
 * @param[in] hInsContext Instance context handle (unused for singleton table).
 *
 * @return The number of User entries in the table.
 */
ULONG
User_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a User entry by index from the Device.Users.User table.
 *
 * This function returns the specified user account entry from the User table
 * for Device.Users.User.{i}.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Pointer to receive the instance number of the entry.
 *
 * @return Handle to the User entry
 * @retval Handle to the User entry by index.
 * @retval NULL if not found.
 */
ANSC_HANDLE
User_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Adds a new User entry to the Device.Users.User table.
 *
 * This function creates a new user account in the User table. The entry is
 * not persisted until User_Commit is called.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pInsNumber Pointer to receive the assigned instance number.
 *
 * @return Handle to the User entry
 * @return Handle to the User entry.
 * @retval NULL if not found.
 */
ANSC_HANDLE
User_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

/**
 * @brief Deletes a User entry from the Device.Users.User table.
 *
 * This function marks a user account for deletion. The deletion is not
 * finalized until User_Commit is called.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] hInstance Handle to the User entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful removal.
 * @retval Error code otherwise
 */
ULONG
User_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

/**
 * @brief Retrieves a boolean parameter value from a User entry.
 *
 * This function returns boolean parameters from Device.Users.User.{i}.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise
 */
BOOL
User_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter value from a User entry.
 *
 * This function returns integer parameters from Device.Users.User.{i}.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to receive the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise
 */
BOOL
User_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter value from a User entry.
 *
 * This function returns ULONG parameters from Device.Users.User.{i}.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to receive the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved successfully.
 * @retval FALSE otherwise
 */
BOOL
User_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter value from a User entry.
 *
 * This function returns string parameters from Device.Users.User.{i}.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Buffer to receive the string value (typically 256 bytes).
 * @param[in,out] pUlSize Pointer to buffer size; receives actual string length. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if successful.
 * @retval 1 if buffer too small.
 * @retval -1 if not supported.
 */
ULONG
User_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter value for a User entry.
 *
 * This function sets boolean parameters in Device.Users.User.{i}.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise
 */
BOOL
User_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets an integer parameter value for a User entry.
 *
 * This function sets integer parameters in Device.Users.User.{i}.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise
 */
BOOL
User_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets an unsigned long parameter value for a User entry.
 *
 * This function sets ULONG parameters in Device.Users.User.{i}.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise
 */
BOOL
User_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets a string parameter value for a User entry.
 *
 * This function sets string parameters in Device.Users.User.{i}.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise
 */
BOOL
User_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending changes to a User entry.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 * @param[out] pReturnParamName Buffer to receive the name of the parameter if there's a validation.(typically 128 bytes).
 * @param[in,out] puLength Pointer to buffer size; receives actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
User_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending changes to a User entry.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
User_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending changes to a User entry.
 *
 * This function is called to roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle to the User entry.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
User_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
