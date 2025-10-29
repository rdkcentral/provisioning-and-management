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

    module: cosa_hosts_internal.h

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


#ifndef  _COSA_HOSTS_INTERNAL_H
#define  _COSA_HOSTS_INTERNAL_H

#include "cosa_apis.h"


#define COSA_DML_USERS_USER_ACCESS_INTERVAL     10

/*
    Object definition
*/
#define  COSA_DATAMODEL_HOSTS_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                                \
    COSA_BASE_CONTENT                                                                       \
    /* start of NAT object class content */                                                         \
    PCOSA_DML_HOST_ENTRY            pHostEntry;                                             \
    ULONG                           HostEntryCount;                                         \
    ULONG                           PreviousVisitTime;                                      \

typedef  struct
_COSA_DATAMODEL_HOSTS
{
    COSA_DATAMODEL_HOSTS_CLASS_CONTENT
}
COSA_DATAMODEL_HOSTS,  *PCOSA_DATAMODEL_HOSTS;

/**********************************
    Standard function declaration
***********************************/
/**
 * @brief Create the Hosts backend object.
 *
 * @return Handle to the newly created Hosts backend object, or NULL on failure.
 * @retval Non-NULL handle to the new object.
 * @retval NULL on failure.
 *
 */
ANSC_HANDLE
CosaHostsCreate
    (
        VOID
    );

/**
 * @brief Initializes the Hosts backend object.
 *
 * @param[in] hThisObject Handle to the Hosts backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval error code otherwise.
 *
 */
ANSC_STATUS
CosaHostsInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the Hosts backend object.
 *
 * @param[in] hThisObject Handle to the Hosts backend object to be removed.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful removal.
 * @retval error code otherwise.
 *
 */
ANSC_STATUS
CosaHostsRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Allocates memory for LAN manager data structures.
 *
 * @param[in] size Size in bytes to allocate.
 *
 * @return Pointer to the allocated memory block, or NULL on allocation failure.
 * @retval void* Pointer to the allocated memory block.
 * @retval NULL on allocation failure.
 *
 */
void * LanManager_Allocate
(
    size_t size
);
/**
 * @brief Frees memory allocated by LanManager.
 *
 * Deallocates a memory block previously allocated by LanManager_Allocate.
 *
 * @param[in] p Pointer to the memory block to free.
 *
 * @return None.
 */
void LanManager_Free
(
    void *p
);
/**
 * @brief Clones a string by allocating new memory and copying the content.
 *
 * @param[in] src Source string to clone (minimum 0 bytes, maximum SIZE_MAX bytes).
 *
 * @return Pointer to the newly allocated cloned string, or NULL on failure.
 * @retval char* Pointer to the cloned string.
 * @retval NULL If allocation fails or src is NULL.
 */
char * LanManager_CloneString
(
    const char * src
);

#endif
