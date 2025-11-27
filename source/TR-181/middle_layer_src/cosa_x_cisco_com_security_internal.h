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

    module: cosa_x_cisco_com_security_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the internal apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#ifndef  _COSA_X_CISCO_SECURITY_INTERNAL_H
#define  _COSA_X_CISCO_SECURITY_INTERNAL_H

#include "cosa_x_cisco_com_security_apis.h"

#define  COSA_IREP_FOLDER_NAME_IA                        "InternetAccess"
#define  COSA_IREP_FOLDER_NAME_IAHA                      "InternetAccessHalfAdded"
#define  COSA_IREP_FOLDER_NAME_IA_Policy                 "InternetAccessPolicy"
#define  COSA_IREP_FOLDER_NAME_MAC                       "Mac"
#define  COSA_DML_RR_NAME_IA_Name                        "Name"
#define  COSA_DML_RR_NAME_IA_Upper_InsNum                "UpperInstanceNumber"
#define  COSA_DML_RR_NAME_IA_InsNum                      "InstanceNumber"
#define  COSA_DML_RR_NAME_IA_Alias                       "Alias"
#define  COSA_DML_RR_NAME_IA_NextInsNunmber              "NextAPInstanceNum"
#define  COSA_DML_RR_NAME_IA_URL_NextInsNunmber          "NextURLInstanceNum"
#define  COSA_DML_RR_NAME_IA_KEYWORD_NextInsNunmber      "NextKeywordInstanceNum"
#define  COSA_DML_RR_NAME_IA_APP_NextInsNunmber          "NextAppInstanceNum"
#define  COSA_DML_RR_NAME_IA_Prefix                      "InternetAccessPolicy"
#define  COSA_DML_RR_NAME_IA_URL_Prefix                  "BlockedURL"
#define  COSA_DML_RR_NAME_IA_KEYWORD_Prefix              "BlockedKeyword"
#define  COSA_DML_RR_NAME_IA_APP_Prefix                  "BlockedApp"

#define  COSA_DATAMODEL_SECURITY_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    /* start of FIREWALL object class content */                                            \
    COSA_DML_SECURITY_CFG            SecurityConfig;                                        \
    COSA_DML_FIREWALL_CFG2           FirewallConfig;                                        \
    SLIST_HEADER                     AccessPolicyList;                                      \
    SLIST_HEADER                     LogList;                                               \
    ANSC_HANDLE                      pLog;                                                  \
    ULONG                            ulLogEntryCount;                                       \
    ULONG                            ulNextAPInstanceNum;                                   \
    ULONG                            ulNextLogInstance;                                     \
    ANSC_HANDLE                      hIrepFolderCOSA;                                       \
    ANSC_HANDLE                      hIrepFolderIA;                                         \
    ANSC_HANDLE                      hIrepFolderIAHA;                                       \


typedef  struct
_COSA_DATAMODEL_SECURITY
{
	COSA_DATAMODEL_SECURITY_CLASS_CONTENT
}
COSA_DATAMODEL_SECURITY,  *PCOSA_DATAMODEL_SECURITY;


/**
 * @brief Create the Cisco Security backend object.
 *
 * @return Handle to the newly created Security backend object
 * @retval Non-NULL handle to the newly created Security backend object.
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaSecurityCreate
    (
        VOID
    );

/**
 * @brief Initialize the Cisco Security backend.
 *
 * @param[in] hThisObject Handle to the Security backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaSecurityInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Remove and cleanup the Cisco Security backend.
 *
 * @param[in] hThisObject Handle to the Security backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful cleanup.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaSecurityRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Load Cisco Internet Access policies from IREP registry.
 *
 * @param[in] hThisObject Handle to the Security backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful registry load.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaSecurityIARegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Add Cisco Internet Access policy entry to IREP registry.
 *
 * @param[in] hThisObject Handle to the Security backend object.
 * @param[in] pNextInsNumName Name of the registry parameter storing next instance number.
 * @param[in] ulNextInsNum Next available instance number for new entries.
 * @param[in] pPreffix Registry path prefix for the Internet Access policy entry.
 * @param[in] ulUpperInsNum Parent object instance number in data model hierarchy.
 * @param[in] pAlias User-defined alias name for the Internet Access policy entry (max 64 chars).
 * @param[in] hCosaContext Handle to the COSA context structure for the policy entry.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful registry add.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaSecurityIARegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pNextInsNumName,
        ULONG                       ulNextInsNum,
        char*                       pPreffix,
        ULONG                       ulUpperInsNum,
        char*                       pAlias,
        ANSC_HANDLE                 hCosaContext
    );

/*
ANSC_STATUS
CosaSecurityIARegSetInfo
    (
        ANSC_HANDLE                 hThisObject
    );
*/

/**
 * @brief Delete Cisco Internet Access policy entry from IREP registry.
 *
 * @param[in] hThisObject Handle to the Security backend object.
 * @param[in] hCosaContext Handle to the COSA context structure for the policy entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful registry deletion.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaSecurityIARegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );


#endif
