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

#ifndef  _COSA_X_RDKCENTRAL_COM_VIDEOSERVICE_DML_H
#define  _COSA_X_RDKCENTRAL_COM_VIDEOSERVICE_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**
 * @brief Retrieves boolean parameter values for Video Service configuration.
 *
 * This function handles TR-181 Device.X_RDKCENTRAL-COM_VideoService boolean parameters for video service settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
VideoService_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for Video Service configuration.
 *
 * This function handles TR-181 Device.X_RDKCENTRAL-COM_VideoService boolean parameters for configuring video service settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set for the parameter.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
VideoService_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves boolean parameter values for WMM downstream mapping configuration.
 *
 * This function handles TR-181 boolean parameters for WMM (WiFi Multimedia) downstream QoS mapping settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve for WMM downstream mapping.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is found and retrieved successfully.
 * @retval FALSE If parameter is not supported or retrieval fails.
 */
BOOL
WMMdownstreamMap_GetParamBoolValue
	(
		 ANSC_HANDLE                 hInsContext,
		 char*                       ParamName,
		 BOOL*                       pBool
	);

/**
 * @brief Sets boolean parameter values for WMM downstream mapping configuration.
 *
 * This function handles TR-181 boolean parameters for configuring WMM (WiFi Multimedia) downstream QoS mapping settings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set for WMM downstream mapping.
 * @param[in] bValue Boolean value to set for the parameter.
 *
 * @return The status of the operation.
 * @retval TRUE If parameter is set successfully.
 * @retval FALSE If parameter is not supported or set operation fails.
 */
BOOL
WMMdownstreamMap_SetParamBoolValue
	(
		 ANSC_HANDLE                 hInsContext,
		 char*                       ParamName,
		 BOOL                        bValue
	);

/*BOOL
VideoService_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
VideoService_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
VideoService_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );*/

#endif
