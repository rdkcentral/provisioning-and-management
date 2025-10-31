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

    module: cosa_x_cisco_com_ddns_internal.h

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
#if !defined(DDNS_BROADBANDFORUM)
#ifndef  _COSA_X_CISCO_COM_DDNS_INTERNAL_H
#define  _COSA_X_CISCO_COM_DDNS_INTERNAL_H

#include "cosa_x_cisco_com_ddns_apis.h"

#define  COSA_IREP_FOLDER_NAME_DDNS                      "DDNS"
#define  COSA_DML_RR_NAME_DdnsServiceAlias               "Alias"
#define  COSA_DML_RR_NAME_DdnsServiceInsNum              "InstanceNumber"
#define  COSA_DML_RR_NAME_DdnsServiceNextInsNumber       "NextInstanceNumber"
#define  COSA_DML_RR_NAME_DdnsServiceNewlyAdded          "NewlyAdded"

#define  COSA_DATAMODEL_DDNS_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    /* start of DDNS object class content */                                                \
    ULONG                           ulNextInstance;                                         \
    SLIST_HEADER                    ContextHead;                                            \
    COSA_DML_DDNS_CFG               DdnsConfig;                                             \
    ANSC_HANDLE                     hIrepFolderCOSA;                                        \
    ANSC_HANDLE                     hIrepFolderDdns;                                        \


typedef  struct
_COSA_DATAMODEL_DDNS
{
	COSA_DATAMODEL_DDNS_CLASS_CONTENT
}
COSA_DATAMODEL_DDNS,  *PCOSA_DATAMODEL_DDNS;


/*
 *  Standard function declaration
 */

/**
 * @brief Creates a new Cisco DDNS backend object.
 *
 * @return Handle to the Cisco DDNS backend object,
 * @retval Handle to the newly created CosaDdns backend object.
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaDdnsCreate
    (
        VOID
    );

/**
 * @brief Initializes the Cisco DDNS backend object.
 *
 * @param[in] hThisObject Handle to the Cisco DDNS backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDdnsInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the Cisco DDNS backend object.
 *
 * @param[in] hThisObject Handle to the Cisco DDNS backend object to be removed.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful removal.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDdnsRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Loads Cisco DDNS configuration from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the Cisco DDNS backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful configuration load.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDdnsRegGetDdnsInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Adds a Cisco DDNS service entry to persistent registry storage.
 *
 * @param[in] hThisObject Handle to the Cisco DDNS backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the DDNS service entry.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful entry addition.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDdnsRegAddDdnsInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Deletes a Cisco DDNS service entry from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the Cisco DDNS backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the DDNS service entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful entry deletion.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDdnsRegDelDdnsInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

#endif
#endif
