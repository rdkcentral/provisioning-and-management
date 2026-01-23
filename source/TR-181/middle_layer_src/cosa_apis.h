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


/**********************************************************************

    module:	cosa_apis.h

        This is base file for all parameters H files.

    ---------------------------------------------------------------

    description:

        All cosa_xxx_apis.h will include this file and realize
		necessary functions in the struct of this file.

    ---------------------------------------------------------------

    environment:

        COSA independent

    ---------------------------------------------------------------

    author:

        Yan Li

    ---------------------------------------------------------------

    revision:

        01/12/2011    initial revision.

**********************************************************************/


#ifndef  _COSA_APIS_H
#define  _COSA_APIS_H

#include "ansc_platform.h"
#include "ansc_string_util.h"

#include "cosa_dml_api_common.h"
#include "cosa_apis_util.h"
#include "cosa_apis_busutil.h"

typedef  ANSC_HANDLE
(*PFN_COSADM_CREATE)
    (
        VOID
    );

typedef  ANSC_STATUS
(*PFN_COSADM_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_COSADM_INITIALIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * the main struct in cosa_xxx_apis.h need includes this struct and realize all functions.
 */
#define  COSA_BASE_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           Oid;                                                    \
    ANSC_HANDLE                     hSbContext;                                             \
                                                                                            \
    PFN_COSADM_CREATE               Create;                                                 \
    PFN_COSADM_REMOVE               Remove;                                                 \
    PFN_COSADM_INITIALIZE           Initialize;                                             \

typedef  struct
_COSA_BASE_OBJECT
{
    COSA_BASE_CONTENT
}
COSA_BASE_OBJECT,  *PCOSA_BASE_OBJECT;

/*
*  This struct is for creating entry context link in writable table when call GetEntry()
*/
#define  COSA_CONTEXT_LINK_CLASS_CONTENT                                                    \
         SINGLE_LINK_ENTRY                Linkage;                                          \
         ANSC_HANDLE                      hContext;                                         \
         ANSC_HANDLE                      hParentTable;  /* Back pointer */                 \
         ULONG                            InstanceNumber;                                   \
         BOOL                             bNew;                                             \
         ANSC_HANDLE                      hPoamIrepUpperFo;                                 \
         ANSC_HANDLE                      hPoamIrepFo;                                      \

typedef  struct
_COSA_CONTEXT_LINK_OBJECT
{
    COSA_CONTEXT_LINK_CLASS_CONTENT
}
COSA_CONTEXT_LINK_OBJECT,  *PCOSA_CONTEXT_LINK_OBJECT;

#define  ACCESS_COSA_CONTEXT_LINK_OBJECT(p)              \
         ACCESS_CONTAINER(p, COSA_CONTEXT_LINK_OBJECT, Linkage)

#define COSA_CONTEXT_LINK_INITIATION_CONTENT(cxt)                                      \
    (cxt)->hContext            = (ANSC_HANDLE)NULL;                                    \
    (cxt)->hParentTable        = (ANSC_HANDLE)NULL;                                    \
    (cxt)->InstanceNumber      = 0;                                                    \
    (cxt)->bNew                = FALSE;                                                \
    (cxt)->hPoamIrepUpperFo    = (ANSC_HANDLE)NULL;                                    \
    (cxt)->hPoamIrepFo         = (ANSC_HANDLE)NULL;                                    \

#define  COSA_DML_ALIAS_NAME_LENGTH                 64

/**
 * @brief Push a context link entry into a sorted list by instance number.
 *
 * This function inserts a COSA context link object into a sorted list based on the instance number.
 *
 * @param[in,out] pListHead    - Pointer to the list header.
 * @param[in] pCosaContext     - Pointer to the COSA context link object to insert.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ANSC_STATUS
CosaSListPushEntryByInsNum
    (
        PSLIST_HEADER               pListHead,
        PCOSA_CONTEXT_LINK_OBJECT   pCosaContext
    );

/**
 * @brief Get a context link entry from a list by instance number.
 *
 * This function retrieves a COSA context link object from a list based on the instance number.
 *
 * @param[in] pListHead      - Pointer to the list header.
 * @param[in] InstanceNumber - The instance number to search for.
 *
 * @return Pointer to the COSA context link object.
 * @retval PCOSA_CONTEXT_LINK_OBJECT Pointer to the context link object if found.
 * @retval NULL if not found.
 *
 */
PCOSA_CONTEXT_LINK_OBJECT
CosaSListGetEntryByInsNum
    (
        PSLIST_HEADER               pListHead,
        ULONG                       InstanceNumber
    );

#endif
