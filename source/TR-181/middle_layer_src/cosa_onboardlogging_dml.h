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
#ifndef  _COSA_ONBOARDLOGGING_DML_H
#define  _COSA_ONBOARDLOGGING_DML_H

#include "slap_definitions.h"
#include "plugin_main_apis.h"

/***********************************************************************

 APIs for Object:

    OnboardingLogging.

    *  OnboardingLogging_GetParamBoolValue
    *  OnboardingLogging_SetParamBoolValue

***********************************************************************/
/**
 * @brief Enables onboard logging functionality.
 *
 * @param[in] hThisObject Handle to the OnboardLogging object instance.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS if onboard logging was enabled successfully,
 * @retval ANSC_STATUS_FAILURE otherwise.
 */
ANSC_STATUS
EnableOnboardLogging
    (
        ANSC_HANDLE hThisObject
    );
/**
 * @brief Disables onboard logging functionality.
 *
 * @param[in] hThisObject Handle to the OnboardLogging object instance.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS if onboard logging was disabled successfully,
 * @retval ANSC_STATUS_FAILURE otherwise.
 */
ANSC_STATUS
DisableOnboardLogging
    (
        ANSC_HANDLE hThisObject
    );
/**
 * @brief Retrieves boolean parameter values for OnboardingLogging configuration.
 *
 * @param[in] hInsContext Instance context handle .
 * @param[in] ParamName Name of the parameter to retrieve
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter was retrieved successfully
 * @retval FALSE if parameter not found.
 */
BOOL
OnboardingLogging_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for OnboardingLogging configuration.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Name of the parameter to set
 * @param[in] bValue Boolean value to set for the parameter.
 *
 * @return status of operation
 * @retval TRUE if parameter was set successfully.
 * @retval FALSE if parameter not found or setting failed.
 */
BOOL
OnboardingLogging_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

#endif //_COSA_ONBOARDLOGGING_DML_H

