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

    module: cosa_ethernet_internal.h

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

#ifndef  _COSA_X_RDK_FEATURES_INTERNAL_H
#define  _COSA_X_RDK_FEATURES_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include <syscfg/syscfg.h>
#include <sysevent/sysevent.h>
#include "safec_lib_common.h"
#include <unistd.h>

#define  COSA_DATAMODEL_FEATURES_CLASS_CONTENT                              \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \


typedef  struct
_COSA_DATAMODEL_FEATURES_CLASS_CONTENT
{
    COSA_DATAMODEL_FEATURES_CLASS_CONTENT
}
COSA_DATAMODEL_FEATURES, *PCOSA_DATAMODEL_FEATURES;

/*
    Standard function declaration
*/

/**
* @brief Create RDK Features data model object.
*
* @return Handle to the Features object.
* @retval Handle to newly created Features object.
* @retval NULL if memory allocation fails.
*/
ANSC_HANDLE
CosaFeaturesCreate

    (
        VOID
    );

/**
 * @brief Initialize the RDK Features backend.
 *
 * @param[in] hThisObject Handle to the Features backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaFeaturesInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Remove and cleanup the RDK Features backend.
 *
 * @param[in] hThisObject Handle to the Features backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful cleanup.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaFeaturesRemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
