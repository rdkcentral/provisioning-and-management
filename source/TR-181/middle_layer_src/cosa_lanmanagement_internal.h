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

/**************************************************************************

    module: cosa_lanmanagement_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:


    -------------------------------------------------------------------

    revision:

        05/09/2019    initial revision.

**************************************************************************/

#ifndef  _COSA_LANMANAGEMENT_INTERNAL_H
#define  _COSA_LANMANAGEMENT_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_lanmanagement_apis.h"

#define  COSA_DATAMODEL_LANMANAGEMENT_CLASS_CONTENT               \
    /* duplication of the base object class content */            \
    COSA_BASE_CONTENT                                             \
    /* start of LANMANAGEMENT object class content */             \
    COSA_DML_LANMANAGEMENT_CFG   LanMngmCfg;                      \

typedef  struct
_COSA_DATAMODEL_LANMANAGEMENT_CLASS_CONTENT
{
    COSA_DATAMODEL_LANMANAGEMENT_CLASS_CONTENT
}
COSA_DATAMODEL_LANMANAGEMENT, *PCOSA_DATAMODEL_LANMANAGEMENT;

/*
    Standard function declaration
*/
/**
 * @brief Create the LAN Management backend object.
 *
 * @return Handle to the newly created LAN Management backend object.
 * @retval A non-NULL ANSC_HANDLE if successful.
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaLanManagementCreate
    (
        VOID
    );

/**
 * @brief Initializes the LAN Management backend object.
 *
 * @param[in] hThisObject Handle to the LAN Management backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval Error code otherwise
 */
ANSC_STATUS
CosaLanManagementInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the LAN Management backend object.
 *
 * @param[in] hThisObject Handle to the LAN Management backend object to be removed.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval Error code otherwise
 */
ANSC_STATUS
CosaLanManagementRemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
