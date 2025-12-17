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

#ifndef _COSA_RLOG_INTERNAL_H
#define _COSA_RLOG_INTERNAL_H

#include "cosa_x_cisco_com_rlog_apis.h"

typedef struct
_COSA_DATAMODEL_RLOG
{
    COSA_BASE_CONTENT
    COSA_DML_RLOG       RLog;
}
COSA_DATAMODEL_RLOG, *PCOSA_DATAMODEL_RLOG;

/**
 * @brief Create the Remote Logging backend object.
 *
 * @return Handle to the newly created Remote Logging backend object
 * @retval Non-NULL Handle to the newly created Remote Logging backend object.
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaRLogCreate(void);

/**
 * @brief Initialize the Remote Logging backend.
 *
 * Loads remote logging configuration and establishes service connectivity for log forwarding.
 *
 * @param[in] hThisObject Handle to the Remote Logging backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization, error code otherwise.
 * @retval ANSC_STATUS_FAILURE If initialization or configuration loading fails.
 */
ANSC_STATUS
CosaRLogInitialize(ANSC_HANDLE hThisObject);

/**
 * @brief Remove and cleanup the Remote Logging backend.
 *
 * Frees all dynamically allocated resources for the Remote Logging backend.
 *
 * @param[in] hThisObject Handle to the Remote Logging backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful cleanup
 * @retval ANSC_STATUS_FAILURE If service termination fails.
 */
ANSC_STATUS
CosaRLogRemove(ANSC_HANDLE hThisObject);

#endif
