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

#ifndef  _COSA_ADVSEC_UTILS_H
#define  _COSA_ADVSEC_UTILS_H

#include "ansc_platform.h"
#include "ansc_string_util.h"

/**
 * @brief Get an unsigned long value from system configuration.
 *
 * @param[in] setting - Pointer to the system configuration setting name.
 * @param[out] value  - Pointer to store the retrieved unsigned long value.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if fails.
 *
 */
ANSC_STATUS CosaGetSysCfgUlong(char* setting, ULONG *value);

/**
 * @brief Set an unsigned long value in system configuration.
 *
 * @param[in] setting - Pointer to the system configuration setting name.
 * @param[in] value   - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if fails.
 *
 */
ANSC_STATUS CosaSetSysCfgUlong(char* setting, ULONG value);

/**
 * @brief Get a string value from system configuration.
 *
 * @param[in] setting  - Pointer to the system configuration setting name.
 * @param[out] value   - Pointer to a buffer where the string value will be stored.
 *                       \n The buffer should be allocated with sufficient size (at least 1024 bytes).
 * @param[out] pulSize - Pointer to store the size of the retrieved string.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if fails.
 *
 */
ANSC_STATUS CosaGetSysCfgString(char* setting, char *value, PULONG pulSize);

/**
 * @brief Set a string value in system configuration.
 *
 * @param[in] setting - Pointer to the system configuration setting name.
 * @param[in] pValue  - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if fails.
 *
 */
ANSC_STATUS CosaSetSysCfgString(char* setting, char *pValue);

#endif //_COSA_ADVSEC_UTILS_H
