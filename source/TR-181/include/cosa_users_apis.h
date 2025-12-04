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

    module: cosa_users_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_USERS_APIS_H
#define  _COSA_USERS_APIS_H

#include "cosa_apis.h"
#include "cosa_users_dml.h"
#include "cosa_common_util.h"

#define STR_SZ 64
#define PWD_SZ 128

typedef  enum
_COSA_DML_USER_PERMISSION
{
    COSA_DML_USER_PERMISSION_Admin                  = 1,
    COSA_DML_USER_PERMISSION_HomeUser,
    COSA_DML_USER_PERMISSION_Restricted,
    COSA_DML_USER_PERMISSION_Denied
}
COSA_DML_USER_PERMISSION, *PCOSA_DML_USER_PERMISSION;

struct _COSA_DML_USER
{
    ULONG                           InstanceNumber;

    BOOLEAN                         bEnabled;

    BOOLEAN                         RemoteAccessCapable;
    char                            Username[STR_SZ];
    char                            Password[PWD_SZ];
    char                            Language[16];
    char                            NumOfFailedAttempts;
    char                            X_RDKCENTRAL_COM_ComparePassword[32];
    char                            HashedPassword[128];
    int			       				RemainingAttempts;
    int			       				LoginCounts;
    int								LockOutRemainingTime;
    #if defined(_COSA_FOR_BCI_)
    int                             NumOfRestoreFailedAttempt;
    #endif
    /*
     * *  Extensions
     * */
    COSA_DML_USER_PERMISSION        AccessPermission;
}_struct_pack_;

typedef struct _COSA_DML_USER COSA_DML_USER,  *PCOSA_DML_USER;

/*
    Function declaration
*/


/**
* @brief Initialize the Users module.
*
* This function initializes the Users module for the device.
*
* @param[in] hDml  - Handle to the DML object.
* @param[in,out] phContext  - Pointer to the context handle.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlUserInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Get the number of user entries.
*
* This function retrieves the count of configured user entries from system configuration.
*
* @param[in] hContext  - Handle to the context.
*
* @return The status of the operation.
* @retval The number of user entries found.
* @retval 0 if no entries are configured.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails.
*
*/
ULONG
CosaDmlUserGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get a user entry by index.
*
* This function retrieves a specific user entry by its index from system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulIndex  - The index of the user entry to retrieve.
* @param[out] pEntry  - Pointer to the user entry structure where the retrieved data will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails or entry retrieval fails.
*
*/
ANSC_STATUS
CosaDmlUserGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_USER              pEntry
    );

/**
* @brief Set the instance number for a user entry.
*
* This function updates the instance number for a user entry at a specific index in system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulIndex  - The index of the user entry to update.
* @param[in] ulInstanceNumber  - The new instance number to set for the entry.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails or update operation fails.
*
*/
ANSC_STATUS
CosaDmlUserSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Add a new user entry.
*
* This function adds a new user entry to the system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pEntry  - Pointer to the user entry structure containing the new user data.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails or add operation fails.
*
*/
ANSC_STATUS
CosaDmlUserAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry
    );

/**
* @brief Delete a user entry.
*
* This function deletes a user entry from system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] ulInstanceNumber  - The instance number of the user entry to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails or delete operation fails.
*
*/
ANSC_STATUS
CosaDmlUserDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Set/Update a user configuration entry.
*
* This function updates an existing user entry in system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pEntry  - Pointer to the user entry structure containing the updated user data.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails or update operation fails.
*
*/
ANSC_STATUS
CosaDmlUserSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    );

/**
* @brief Get the configuration for a user entry.
*
* This function retrieves the configuration for a specific user entry by matching the
* instance number from system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in,out] pEntry  - Pointer to the user entry structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization fails or retrieval operation fails.
*
*/
ANSC_STATUS
CosaDmlUserGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    );

/**
* @brief Validate a user password.
*
* This function validates a user password by comparing the hashed version of the provided
* password with the stored hashed password.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pString  - Pointer to the password string to validate.
* @param[in] pEntry  - Pointer to the user entry structure containing user information.
*                    \n The Username and HashedPassword fields are used for validation.
* @param[out] hashpassword  - Pointer to a buffer where the validation result will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if password hashing or extraction fails.
*
*/
ANSC_STATUS
user_validatepwd
        (
            ANSC_HANDLE                 hContext,
            PCHAR                       pString,
            PCOSA_DML_USER              pEntry,
            char*                       hashpassword
        );

/**
* @brief Hash a user password.
*
* This function generates a hashed password.
*
* @param[in] pString  - Pointer to the password string to hash.
* @param[out] hashedpassword  - Pointer to a buffer where the hashed password (hex string) will be stored.
*                    \n The buffer should be allocated with size specified by hashedpassword_size parameter.
* @param[in] hashedpassword_size  - The size of the hashedpassword buffer in bytes.Recommended size is 128 bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE otherwise.
*/
ANSC_STATUS
hash_userPassword
        (
                PCHAR              pString,
                char*              hashedpassword,     /* Identified by InstanceNumber */
                int                hashedpassword_size
        );

/**
* @brief Hash and save a user password to system configuration.
*
* This function hashes a password using hash_userPassword and saves it to system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pString  - Pointer to the password string to hash and save.
* @param[in,out] pEntry  - Pointer to the user entry structure.
*                    \n The Username field identifies which user's password to save.
*                    \n The HashedPassword field is updated with the hashed password.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if hashing fails, syscfg_set fails, or username is not supported.
*
*/
ANSC_STATUS
user_hashandsavepwd
        (
            ANSC_HANDLE                 hContext,
            PCHAR                       pString,
            PCOSA_DML_USER              pEntry

        );

/**
* @brief Reset a user's password to the default value.
*
* This function resets a user's password to the default value based on the username.
*
* @param[in] bValue  - Boolean value indicating whether to reset the password.
*                    \n Must be TRUE for password reset to proceed.
* @param[in,out] pEntry  - Pointer to the user entry structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if the username is not supported, bValue is FALSE, rdkconfig_get fails, or memset_s fails.
*
*/
ANSC_STATUS
CosaDmlUserResetPassword
      (
          BOOL                        bValue,
          PCOSA_DML_USER              pEntry
      );

#endif