/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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

#ifndef  _COSA_ONBOARDLOGGING_INTERNAL_H
#define  _COSA_ONBOARDLOGGING_INTERNAL_H

#include "ansc_platform.h"
#include "ansc_string_util.h"

typedef  struct
_COSA_DATAMODEL_ONBOARDLOGGING
{
    BOOL                        bEnable;
}
COSA_DATAMODEL_ONBOARDLOGGING,  *PCOSA_DATAMODEL_ONBOARDLOGGING;

/*
    Standard function declaration
*/
/**
 * @brief Creates a new OnboardLogging backend object.
 *
 * @return Handle to the OnboardLogging backend object
 * @retval Handle to the newly created OnboardLogging backend object
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaOnboardLoggingCreate
    (
        VOID
    );

/**
 * @brief Initializes the OnboardLogging backend object.
 *
 * @param[in] hThisObject Handle to the OnboardLogging backend object.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful initialization
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaOnboardLoggingInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the OnboardLogging backend object.
 *
 * @param[in] hThisObject Handle to the OnboardLogging backend object to be removed.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful removal
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaOnboardLoggingRemove
    (
        ANSC_HANDLE                 hThisObject
    );
#endif
