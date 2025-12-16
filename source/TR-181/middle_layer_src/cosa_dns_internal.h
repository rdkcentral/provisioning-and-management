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

    module: cosa_dns_internal.h

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


#ifndef  _COSA_DNS_INTERNAL_H
#define  _COSA_DNS_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "cosa_dml_api_dns.h"

#define  COSA_IREP_FOLDER_NAME_SERVER              "DNSServer"
#define  COSA_IREP_FOLDER_NAME_SERVER_HA           "DNSServerHalfAdded"

#define  COSA_DML_RR_NAME_SERVER_Prefix            "DNSServer"
#define  COSA_DML_RR_NAME_FORWARD_Prefix           "Forwarding"

#define  COSA_DML_RR_NAME_SERVER_Upper_InsNum      "UpperInsNum"
#define  COSA_DML_RR_NAME_SERVER_InsNum            "InstanceNum"
#define  COSA_DML_RR_NAME_SERVER_Alias             "Alias"

#define  COSA_DML_RR_NAME_SERVER_NextInsNum        "NextInsNum"
#define  COSA_DML_RR_NAME_FORWARD_NextInsNum       "NextForwardInsNum"

#define  COSA_DML_DNS_ACCESS_INTERVAL              5 /* seconds*/

/***********************************
    Actual definition declaration
************************************/

/*
    Object definition
*/

#define  COSA_DATAMODEL_DNS_CLASS_CONTENT                               \
    /* duplication of the base object class content */                           \
    COSA_BASE_CONTENT                                                    \
    /* start of NAT object class content */                                   \
    SLIST_HEADER                    ServerList;                              \
    ULONG                           ulNextServerInsNum;                      \
    ULONG                           PreVisitSrvTime;                         \
    ANSC_HANDLE                     hIrepFolderCOSA;                         \
    ANSC_HANDLE                     hIrepFolderServer;                       \
    ANSC_HANDLE                     hIrepFolderServerHA;                     \
    SLIST_HEADER                    ForwardList;                             \
    ULONG                           ulNextForwardInsNum;                      \
    ULONG                           PreVisitForwardTime;                     \

typedef  struct
_COSA_DATAMODEL_DNS
{
    COSA_DATAMODEL_DNS_CLASS_CONTENT
}
COSA_DATAMODEL_DNS,  *PCOSA_DATAMODEL_DNS;

/**********************************
    Standard function declaration
***********************************/
/**
 * @brief Create the DNS backend object.
 *
 * @return Handle to the DNS backend object
 * @retval Handle to the newly created DNS backend object
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaDNSCreate
    (
        VOID
    );

/**
 * @brief Initializes the DNS backend object.
 *
 * @param[in] hThisObject Handle to the DNS backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval Error code otherwise
 */
ANSC_STATUS
CosaDNSInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the DNS backend object.
 *
 * @param[in] hThisObject Handle to the DNS backend object to be removed.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful removal.
 * @retval Error code otherwise
 */
ANSC_STATUS
CosaDNSRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Loads DNS configuration from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the DNS backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful backend fetch.
 * @retval Error code otherwise
 */
ANSC_STATUS
CosaDNSRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Adds a DNS entry to persistent registry storage.
 *
 * @param[in] hThisObject Handle to the DNS backend object.
 * @param[in] pNextInsNumName Name of the next instance number parameter.
 * @param[in] ulNextInsNum Next instance number to use for new entries.
 * @param[in] pPreffix Registry key prefix for the entry type.
 * @param[in] ulUpperInsNum Upper-level instance number for hierarchical objects.
 * @param[in] pAlias User-assigned alias name for the entry.
 * @param[in] hCosaContext Handle to the COSA context object for the entry.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful entry addition.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDNSRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pNextInsNumName,
        ULONG                       ulNextInsNum,
        char*                       pPreffix,
        ULONG                       ulUpperInsNum,
        char*                       pAlias,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Deletes a DNS entry from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the DNS backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful entry deletion.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDNSRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );


#endif
