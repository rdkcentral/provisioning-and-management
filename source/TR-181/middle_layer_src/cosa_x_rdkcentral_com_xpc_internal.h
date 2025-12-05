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

#ifndef  _COSA_X_RDKCENTRAL_COM_XPC_INTERNAL_H
#define  _COSA_X_RDKCENTRAL_COM_XPC_INTERNAL_H

#include "ansc_platform.h"
#include "ansc_string_util.h"

typedef  struct
_COSA_DATAMODEL_XPC
{
    BOOL            bEnable;
}
COSA_DATAMODEL_XPC,  *PCOSA_DATAMODEL_XPC;

/*
    Standard function declaration
*/
/**
* @brief Enable XPC service.
*
* @param[in] hThisObject  - Handle to XPC object.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if XPC is enabled successfully.
* @retval ANSC_STATUS_FAILURE if platform HAL operation fails.
*
*/
ANSC_STATUS CosaXpcEnable(ANSC_HANDLE hThisObject);

/**
* @brief Disable XPC service.
*
* @param[in] hThisObject  - Handle to XPC object.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if XPC is disabled successfully.
* @retval ANSC_STATUS_FAILURE if platform HAL operation fails.
*
*/
ANSC_STATUS CosaXpcDisable(ANSC_HANDLE hThisObject);

/**
 * @brief Create the RDKCentral XPC backend object.
 *
 * @return Handle to the newly created XPC backend object.
 * @retval Non-NULL Handle to the newly created XPC backend object.
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaXpcCreate
    (
        VOID
    );

/**
 * @brief Initialize the RDKCentral XPC backend.
 *
 * @param[in] hThisObject Handle to the XPC backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaXpcInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Remove and cleanup the RDKCentral XPC backend.
 *
 * @param[in] hThisObject Handle to the XPC backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful cleanup.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaXpcRemove
    (
        ANSC_HANDLE                 hThisObject
    );
#endif //_COSA_X_RDKCENTRAL_COM_XPC_INTERNAL_H
