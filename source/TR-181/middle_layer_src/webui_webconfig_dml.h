/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
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

#ifndef  _COSA_WEBUI_DML_H
#define  _COSA_WEBUI_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/***********************************************************************

 APIs for Object:

    Device.X_RDK_WebUI.Data

    *  WebUIconfig_GetParamStringValue
    *  WebUIconfig_SetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves a string parameter value from Device.X_RDK_WebUI.Data.
 *
 * This function returns string parameters of WebUI WebConfig service.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Buffer to receive the string value.
 * @param[in,out] pUlSize Pointer to buffer size; receives actual string length. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
WebUIconfig_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a string parameter value for Device.X_RDK_WebUI.Data.
 *
 * This function sets string parameters of WebUI Configuration.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter set successfully.
 * @retval FALSE otherwise.
 */
BOOL
WebUIconfig_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

#endif