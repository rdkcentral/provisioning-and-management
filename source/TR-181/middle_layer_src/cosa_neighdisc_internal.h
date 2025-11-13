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

    module: cosa_neighdisc_internal.h

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


#ifndef  _COSA_NEIGHDISC_INTERNAL_H
#define  _COSA_NEIGHDISC_INTERNAL_H

#include "cosa_apis.h"

#define  COSA_IREP_FOLDER_NAME_NeighdiscIF                 "NeighdiscInterface"
#define  COSA_IREP_FOLDER_NAME_NeighdiscIF_HA              "NeighdiscInterfaceHalfAdded"

#define  COSA_DML_RR_NAME_NeighdiscIF_Prefix               "NeighdiscInterface"

#define  COSA_DML_RR_NAME_NeighdiscIF_Upper_InsNum         "UpperInstanceNumber"
#define  COSA_DML_RR_NAME_NeighdiscIF_InsNum               "InstanceNumber"
#define  COSA_DML_RR_NAME_NeighdiscIF_Alias                "Alias"

#define  COSA_DML_RR_NAME_NeighdiscIF_NextInsNum           "NextInstanceNumber"
#define  COSA_DML_RR_NAME_NeighdiscIF_OPTION_NextInsNum    "NextOptionInstanceNum"



/***********************************
    Actual definition declaration
************************************/

/*
    Object definition
*/

#define  COSA_DATAMODEL_NEIGHDISC_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    /* start of NAT object class content */                                                 \
    BOOL                            bEnabled;                                               \
    SLIST_HEADER                    InterfaceList;                                          \
    ULONG                           ulInterfaceCount;                                     \
    ULONG                           ulNextInterfaceInsNum;                                \
    ANSC_HANDLE                     hIrepFolderCOSA;                                       \
    ANSC_HANDLE                     hIrepFolderNeighdiscInterface;                                 \
    ANSC_HANDLE                     hIrepFolderNeighdiscInterfaceHA;                                 \
    ULONG                           ulNbTblNumber;                                           \
    PCOSA_DML_NEIGHTABLE_INFO       pNbTbl;                                                  \

typedef  struct
_COSA_DATAMODEL_NEIGHDISC
{
    COSA_DATAMODEL_NEIGHDISC_CLASS_CONTENT
}
COSA_DATAMODEL_NEIGHDISC,  *PCOSA_DATAMODEL_NEIGHDISC;

/**********************************
    Standard function declaration
***********************************/
/**
 * @brief Creates a new Neighbor Discovery backend object.
 *
 * @return Handle to the Neighbor Discovery backend object
 * @retval Handle to the newly created Neighbor Discovery backend object
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaNeighdiscCreate
    (
        VOID
    );

/**
 * @brief Initializes the Neighbor Discovery backend object.
 *
 * @param[in] hThisObject Handle to the Neighbor Discovery backend object.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful initialization
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNeighdiscInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the Neighbor Discovery backend object.
 *
 * @param[in] hThisObject Handle to the Neighbor Discovery backend object to be removed.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful removal
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNeighdiscRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Loads Neighbor Discovery configuration from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the Neighbor Discovery backend object.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful configuration load
 * @retval error code otherwise
 */
ANSC_STATUS
CosaNeighdiscRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Adds a Neighbor Discovery entry to persistent registry storage.
 *
 * @param[in] hThisObject Handle to the Neighbor Discovery backend object.
 * @param[in] pNextInsNumName Name of the next instance number parameter.
 * @param[in] ulNextInsNum Next instance number to use for new entries.
 * @param[in] pPreffix Registry key prefix for the entry type.
 * @param[in] ulUpperInsNum Upper-level instance number for hierarchical objects.
 * @param[in] pAlias User-assigned alias name for the entry.
 * @param[in] hCosaContext Handle to the COSA context object for the entry.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful entry addition
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNeighdiscRegAddInfo
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
 * @brief Deletes a Neighbor Discovery entry from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the Neighbor Discovery backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the entry to delete.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful entry deletion
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNeighdiscRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );





#endif
