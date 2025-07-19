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


ANSC_STATUS
CosaDmlUserInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ULONG
CosaDmlUserGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlUserGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_USER              pEntry
    );

ANSC_STATUS
CosaDmlUserSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlUserAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry
    );

ANSC_STATUS
CosaDmlUserDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlUserSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlUserGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    );
ANSC_STATUS
user_validatepwd
        (
            ANSC_HANDLE                 hContext,
            PCHAR                       pString,
            PCOSA_DML_USER              pEntry,
            char*                       hashpassword
        );

ANSC_STATUS
hash_userPassword
        (
                PCHAR              pString,
                char*              hashedpassword,     /* Identified by InstanceNumber */
                int                hashedpassword_size
        );

ANSC_STATUS
user_hashandsavepwd
        (
            ANSC_HANDLE                 hContext,
            PCHAR                       pString,
            PCOSA_DML_USER              pEntry

        );

ANSC_STATUS
CosaDmlUserResetPassword
      (
          BOOL                        bValue,
          PCOSA_DML_USER              pEntry
      );

#endif
