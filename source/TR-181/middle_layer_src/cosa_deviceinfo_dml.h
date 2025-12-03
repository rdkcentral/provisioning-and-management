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

    module: cosa_deviceinfo_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_DEVICEINFO_DML_H
#define  _COSA_DEVICEINFO_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_deviceinfo_internal.h"
#include "cosa_deviceinfo_apis.h"
#include "cosa_deviceinfo_dml_custom.h"
#define  PARTNER_ID_LEN  64
#define PARAM_TCP_IPV4_ENABLE "Device.QOS.X_RDK_LatencyMeasure_IPv4Enable"
#define COMP_TCP_IPV4_ENABLE "com.cisco.spvtg.ccsp.tdm"
#define DBUSPATH_TCP_IPV4_ENABLE "/com/cisco/spvtg/ccsp/tdm"

#define PARAM_TCP_IPV6_ENABLE "Device.QOS.X_RDK_LatencyMeasure_IPv6Enable"
#define COMP_TCP_IPV6_ENABLE "com.cisco.spvtg.ccsp.tdm"
#define DBUSPATH_TCP_IPV6_ENABLE "/com/cisco/spvtg/ccsp/tdm"

#ifdef FEATURE_SUPPORT_ONBOARD_LOGGING

#define LOGGING_MODULE           "PAM"
#define OnboardLog(...)          rdk_log_onboard(LOGGING_MODULE, __VA_ARGS__)
#else
#define OnboardLog(...)
#endif
/***********************************************************************

 APIs for Object:

    DeviceInfo.

    *  DeviceInfo_GetParamBoolValue
    *  DeviceInfo_GetParamIntValue
    *  DeviceInfo_GetParamUlongValue
    *  DeviceInfo_GetParamStringValue
    *  DeviceInfo_SetParamBoolValue
    *  DeviceInfo_SetParamIntValue
    *  DeviceInfo_SetParamUlongValue
    *  DeviceInfo_SetParamStringValue
    *  DeviceInfo_Validate
    *  DeviceInfo_Commit
    *  DeviceInfo_Rollback

***********************************************************************/
/**
 * @brief Get boolean parameter value from DeviceInfo object.
 *
 * @param[in] hInsContext - The instance handle to the DeviceInfo object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DeviceInfo_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from DeviceInfo object.
 *
 * @param[in] hInsContext - The instance handle to the DeviceInfo object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DeviceInfo_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from DeviceInfo object.
 *
 * @param[in] hInsContext - The instance handle to the DeviceInfo object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DeviceInfo_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from DeviceInfo object.
 *
 * @param[in] hInsContext  - The instance handle to the DeviceInfo object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if not supported
 *
 */
ULONG
DeviceInfo_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for DeviceInfo object.
 *
 * @param[in] hInsContext - The instance handle to the DeviceInfo object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DeviceInfo_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for DeviceInfo object.
 *
 * @param[in] hInsContext - The instance handle to the DeviceInfo object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] value       - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DeviceInfo_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for DeviceInfo object.
 *
 * @param[in] hInsContext  - The instance handle to the DeviceInfo object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[in] uValuepUlong - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DeviceInfo_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for DeviceInfo object.
 *
 * @param[in] hInsContext - The instance handle to the DeviceInfo object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] strValue    - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DeviceInfo_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate DeviceInfo object parameter values.
 *
 * @param[in] hInsContext        - The instance handle to the DeviceInfo object.
 * @param[out] pReturnParamName  - Pointer to a buffer(128 bytes) to store the parameter name if there is a validation.
 * @param[in,out] puLength       - Pointer to the buffer length.
 *
 * @return The validation result.
 * @retval TRUE if there is no validation.
 * @retval FALSE if there's validation .
 *
 */
BOOL
DeviceInfo_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit DeviceInfo object parameter changes.
 *
 * @param[in] hInsContext - The instance handle to the DeviceInfo object.
 *
 * @return The status of the operation.
 *
 */
ULONG
DeviceInfo_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback DeviceInfo object parameter changes whenever there's a validation found.
 *
 * @param[in] hInsContext - The instance handle to the DeviceInfo object.
 *
 * @return The status of the operation.
 *
 */
ULONG
DeviceInfo_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );



/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_WIFI_TELEMETRY.{i}.

    *  WiFi_Telemetry_SetParamIntValue
    *  WiFi_Telemetry_SetParamStringValue
    *  WiFi_Telemetry_GetParamIntValue
    *  WiFi_Telemetry_GetParamStringValue
***********************************************************************/
/**
 * @brief Set integer parameter value for WiFi_Telemetry object.
 *
 * @param[in] hInsContext - The instance handle to the WiFi_Telemetry object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] iValue      - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
WiFi_Telemetry_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    );

/**
 * @brief Set string parameter value for WiFi_Telemetry object.
 *
 * @param[in] hInsContext - The instance handle to the WiFi_Telemetry object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] pString     - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
WiFi_Telemetry_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Get integer parameter value from WiFi_Telemetry object.
 *
 * @param[in] hInsContext - The instance handle to the WiFi_Telemetry object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
WiFi_Telemetry_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get string parameter value from WiFi_Telemetry object.
 *
 * @param[in] hInsContext  - The instance handle to the WiFi_Telemetry object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if not supported.
 *
 */
ULONG
WiFi_Telemetry_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );


/**
 * @brief Get boolean parameter value from EasyConnect object.
 *
 * @param[in] hInsContext - The instance handle to the EasyConnect object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
EasyConnect_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set boolean parameter value for EasyConnect object.
 *
 * @param[in] hInsContext - The instance handle to the EasyConnect object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
EasyConnect_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Get boolean parameter value from DFS object.
 *
 * @param[in] hInsContext - The instance handle to the DFS object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DFS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set boolean parameter value for DFS object.
 *
 * @param[in] hInsContext - The instance handle to the DFS object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DFS_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/**
 * @brief Get boolean parameter value from DFSatBootUp object.
 *
 * @param[in] hInsContext - The instance handle to the DFSatBootUp object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DFSatBootUp_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set boolean parameter value for DFSatBootUp object.
 *
 * @param[in] hInsContext - The instance handle to the DFSatBootUp object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
DFSatBootUp_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.

    *  Generic_GetParamUlongValue
    *  Generic_SetParamUlongValue
    *  Generic_GetParamBoolValue
    *  Generic_GetParamStringValue
    *  Generic_GetParamIntValue
    *  Generic_SetParamBoolValue
    *  Generic_SetParamStringValue
    *  Generic_SetParamIntValue
***********************************************************************/
/**
 * @brief Get unsigned long parameter value from Generic RFC object.
 *
 * @param[in] hInsContext - The instance handle to the Generic RFC object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pValue     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Generic_GetParamUlongValue
    (
        ANSC_HANDLE             hInsContext,
        char*                   ParamName,
        ULONG*                  pValue
    );

/**
 * @brief Get boolean parameter value from Generic RFC object.
 *
 * @param[in] hInsContext - The instance handle to the Generic RFC object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Generic_GetParamBoolValue
        (
                ANSC_HANDLE                             hInsContext,
                char*                                           ParamName,
                BOOL*                                           pBool
        );

/**
 * @brief Get string parameter value from Generic RFC object.
 *
 * @param[in] hInsContext  - The instance handle to the Generic RFC object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not Supported
 *
 */
ULONG
Generic_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Get integer parameter value from Generic RFC object.
 *
 * @param[in] hInsContext - The instance handle to the Generic RFC object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Generic_GetParamIntValue
        (
                ANSC_HANDLE                             hInsContext,
                char*                                           ParamName,
                int*                                            pInt
        );

/**
 * @brief Set unsigned long parameter value for Generic RFC object.
 *
 * @param[in] hInsContext - The instance handle to the Generic RFC object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] ulValue     - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Generic_SetParamUlongValue
    (
        ANSC_HANDLE             hInsContext,
        char*                   ParamName,
        ULONG                   ulValue
    );

/**
 * @brief Set boolean parameter value for Generic RFC object.
 *
 * @param[in] hInsContext - The instance handle to the Generic RFC object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Generic_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set string parameter value for Generic RFC object.
 *
 * @param[in] hInsContext - The instance handle to the Generic RFC object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] strValue    - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Generic_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );


/**
 * @brief Set integer parameter value for Generic RFC object.
 *
 * @param[in] hInsContext - The instance handle to the Generic RFC object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] value       - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Generic_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );


typedef BOOL (*GETBOOL_FUNC_PTR)(ANSC_HANDLE, char*, BOOL*);
/**
* @brief Check if the provided boolean value is the same as the current parameter value.
*
* @param[in] hInsContext  - The instance handle to the object.
* @param[in] ParamName    - Pointer to the parameter name.
* @param[in] bValue       - The boolean value to compare.
* @param[in] getBoolFunc  - Function pointer to retrieve the current boolean parameter value.
*
* @return The status of the comparison.
* @retval TRUE if the provided boolean value is the same as the current parameter value.
* @retval FALSE if the values are different or operation fails.
*
*/
BOOL IsBoolSame(ANSC_HANDLE hInsContext,char* ParamName, BOOL bValue, GETBOOL_FUNC_PTR getBoolFunc);
typedef ULONG (*GETSTRING_FUNC_PTR)(ANSC_HANDLE, char*, char*, ULONG*);
/**
* @brief Check if the provided string value is the same as the current parameter value.
*
* @param[in] hInsContext    - The instance handle to the object.
* @param[in] ParamName      - Pointer to the parameter name.
* @param[in] pValue         - Pointer to the string value to compare.
* @param[in] getStringFunc  - Function pointer to retrieve the current string parameter value.
*
* @return The status of the comparison.
* @retval TRUE if the provided string value is the same as the current parameter value.
* @retval FALSE if the values are different or operation fails.
*
*/
BOOL IsStringSame(ANSC_HANDLE hInsContext,char* ParamName, char* pValue, GETSTRING_FUNC_PTR getStringFunc);
typedef BOOL (*GETULONG_FUNC_PTR)(ANSC_HANDLE, char*, ULONG*);
/**
* @brief Check if the provided unsigned long value is the same as the current parameter value.
*
* @param[in] hInsContext   - The instance handle to the object.
* @param[in] ParamName     - Pointer to the parameter name.
* @param[in] ulValue       - The unsigned long value to compare.
* @param[in] getUlongFunc  - Function pointer to retrieve the current unsigned long parameter value.
*
* @return The status of the comparison.
* @retval TRUE if the provided unsigned long value is the same as the current parameter value.
* @retval FALSE if the values are different or operation fails.
*
*/
BOOL IsUlongSame(ANSC_HANDLE hInsContext,char* ParamName, ULONG ulValue, GETULONG_FUNC_PTR getUlongFunc);
typedef BOOL (*GETINT_FUNC_PTR)(ANSC_HANDLE, char*, int*);
/**
* @brief Check if the provided integer value is the same as the current parameter value.
*
* @param[in] hInsContext - The instance handle to the object.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[in] value       - The integer value to compare.
* @param[in] getIntFunc  - Function pointer to retrieve the current integer parameter value.
*
* @return The status of the comparison.
* @retval TRUE if the provided integer value is the same as the current parameter value.
* @retval FALSE if the values are different or operation fails.
*
*/
BOOL IsIntSame(ANSC_HANDLE hInsContext,char* ParamName, int value, GETINT_FUNC_PTR getIntFunc);



/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Control

    *  Control_GetParamUlongValue
    *  Control_SetParamUlongValue
***********************************************************************/
/**
 * @brief Get unsigned long parameter value from Control object.
 *
 * @param[in] hInsContext - The instance handle to the Control object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pValue     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Control_GetParamUlongValue
    (
        ANSC_HANDLE 		hInsContext,
        char*			ParamName,
        ULONG*			pValue
    );

/**
 * @brief Set integer parameter value for Control object.
 *
 * @param[in] hInsContext - The instance handle to the Control object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] ulValue     - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Control_SetParamIntValue
    (
        ANSC_HANDLE 		hInsContext,
        char*			ParamName,
        ULONG 			ulValue
    );

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Control.XconfSelector
    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Control.XconfUrl
    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Control.XconfRecoveryUrl

    *  Control_GetParamStringValue
    *  Control_SetParamStringValue
***********************************************************************/
/**
 * @brief Get string parameter value from Control object.
 *
 * @param[in] hInsContext  - The instance handle to the Control object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if the parameter is not found or operation fails.
 *
 */
ULONG
Control_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set string parameter value for Control object.
 *
 * @param[in] hInsContext - The instance handle to the Control object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] pString     - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Control_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

 /***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Identity.DeviceType

    * Identity_GetParamStringValue
    * Identity_SetParamStringValue
***********************************************************************/
/**
 * @brief Get string parameter value from Identity object.
 *
 * @param[in] hInsContext  - The instance handle to the Identity object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not Supported
 *
 */
ULONG
Identity_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set string parameter value for Identity object.
 *
 * @param[in] hInsContext - The instance handle to the Identity object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] pString     - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Identity_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.STAGE.Enable

    * STAGE_GetParamBoolValue
    * STAGE_SetParamBoolValue
***********************************************************************/
/**
 * @brief Get boolean parameter value from STAGE object.
 *
 * @param[in] hInsContext - The instance handle to the STAGE object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */

BOOL
STAGE_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
/**
 * @brief Set boolean parameter value for STAGE object.
 *
 * @param[in] hInsContext - The instance handle to the STAGE object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */

BOOL
STAGE_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Control.DisableNativeHostapd

    *  Control_GetParamBoolValue
    *  Control_SetParamBoolValue
***********************************************************************/
/**
 * @brief Get boolean parameter value from Control object.
 *
 * @param[in] hInsContext - The instance handle to the Control object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Control_GetParamBoolValue
        (
                ANSC_HANDLE         hInsContext,
                char*               ParamName,
                BOOL*               pBool
        );

/**
 * @brief Set boolean parameter value for Control object.
 *
 * @param[in] hInsContext - The instance handle to the Control object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Control_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.UniqueTelemetryId.

    *  UniqueTelemetryId_GetParamBoolValue
    *  UniqueTelemetryId_GetParamStringValue
    *  UniqueTelemetryId_GetParamIntValue
    *  UniqueTelemetryId_SetParamBoolValue
    *  UniqueTelemetryId_SetParamStringValue
    *  UniqueTelemetryId_SetParamIntValue
***********************************************************************/
/**
 * @brief Get boolean parameter value from UniqueTelemetryId object.
 *
 * @param[in] hInsContext - The instance handle to the UniqueTelemetryId object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
UniqueTelemetryId_GetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL*						pBool
	);

/**
 * @brief Get string parameter value from UniqueTelemetryId object.
 *
 * @param[in] hInsContext  - The instance handle to the UniqueTelemetryId object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not supported
 *
 */
ULONG
UniqueTelemetryId_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Get integer parameter value from UniqueTelemetryId object.
 *
 * @param[in] hInsContext - The instance handle to the UniqueTelemetryId object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
UniqueTelemetryId_GetParamIntValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		int*						pInt
	);


/**
 * @brief Set boolean parameter value for UniqueTelemetryId object.
 *
 * @param[in] hInsContext - The instance handle to the UniqueTelemetryId object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
UniqueTelemetryId_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set string parameter value for UniqueTelemetryId object.
 *
 * @param[in] hInsContext - The instance handle to the UniqueTelemetryId object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] strValue    - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
UniqueTelemetryId_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );


/**
 * @brief Set integer parameter value for UniqueTelemetryId object.
 *
 * @param[in] hInsContext - The instance handle to the UniqueTelemetryId object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] value       - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
UniqueTelemetryId_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );


/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.ManageableNotification.

    *  ManageableNotification_GetParamBoolValue
    *  ManageableNotification_SetParamBoolValue

***********************************************************************/

/**
 * @brief Get boolean parameter value from ManageableNotification object.
 *
 * @param[in] hInsContext - The instance handle to the ManageableNotification object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
ManageableNotification_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set boolean parameter value for ManageableNotification object.
 *
 * @param[in] hInsContext - The instance handle to the ManageableNotification object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
ManageableNotification_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WebUI.Enable

    *  WebUI_GetParamUlongValue
    *  WebUI_SetParamUlongValue

***********************************************************************/

/**
 * @brief Get unsigned long parameter value from WebUI object.
 *
 * @param[in] hInsContext - The instance handle to the WebUI object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] puLong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
WebUI_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
);

/**
 * @brief Set unsigned long parameter value for WebUI object.
 *
 * @param[in] hInsContext - The instance handle to the WebUI object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] uValue      - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
WebUI_SetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG                       uValue
);

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SWDLDirect.Enable

    *  SWDLDirect_GetParamBoolValue
    *  SWDLDirect_SetParamBoolValue

***********************************************************************/

/**
 * @brief Get boolean parameter value from SWDLDirect object.
 *
 * @param[in] hInsContext - The instance handle to the SWDLDirect object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
SWDLDirect_GetParamBoolValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    BOOL*                       pBool
);

/**
 * @brief Set boolean parameter value for SWDLDirect object.
 *
 * @param[in] hInsContext - The instance handle to the SWDLDirect object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
SWDLDirect_SetParamBoolValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    BOOL                        bValue
);

/***********************************************************************

 APIs for Object:

	Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.CognitiveMotionDetection.Enable

    *  CognitiveMotionDetection_GetParamBoolValue
    *  CognitiveMotionDetection_SetParamBoolValue
***********************************************************************/
#ifdef FEATURE_COGNITIVE_WIFIMOTION
/**
 * @brief Get boolean parameter value from CognitiveMotionDetection object.
 *
 * @param[in] hInsContext - The instance handle to the CognitiveMotionDetection object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
CognitiveMotionDetection_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set boolean parameter value for CognitiveMotionDetection object.
 *
 * @param[in] hInsContext - The instance handle to the CognitiveMotionDetection object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
CognitiveMotionDetection_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
#endif
/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Snmpv3DHKickstart.KickstartTable{i}.

    *  Snmpv3DHKickstart_GetParamBoolValue
    *  Snmpv3DHKickstart_SetParamBoolValue
    *  Snmpv3DHKickstart_GetParamUlongValue
    *  Snmpv3DHKickstart_SetParamUlongValue

***********************************************************************/
/**
 * @brief Get boolean parameter value from Snmpv3DHKickstart object.
 *
 * @param[in] hInsContext - The instance handle to the Snmpv3DHKickstart object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Snmpv3DHKickstart_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set boolean parameter value for Snmpv3DHKickstart object.
 *
 * @param[in] hInsContext - The instance handle to the Snmpv3DHKickstart object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Snmpv3DHKickstart_SetParamBoolValue

(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 );

/**
 * @brief Get unsigned long parameter value from Snmpv3DHKickstart object.
 *
 * @param[in] hInsContext - The instance handle to the Snmpv3DHKickstart object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] puLong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Snmpv3DHKickstart_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief Set unsigned long parameter value for Snmpv3DHKickstart object.
 *
 * @param[in] hInsContext - The instance handle to the Snmpv3DHKickstart object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] uValue      - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Snmpv3DHKickstart_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Snmpv3DHKickstart.KickstartTable{i}.

    *  KickstartTable_GetEntryCount
    *  KickstartTable_GetEntry
    *  KickstartTable_GetParamStringValue
    *  KickstartTable_SetParamStringValue

***********************************************************************/

/**
 * @brief Get the number of KickstartTable entries.
 *
 * @param[in] hInsContext - The instance handle.
 *
 * @return The number of KickstartTable entries.
 *
 */
ULONG
KickstartTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Get a KickstartTable entry by index.
 *
 * @param[in] hInsContext  - The instance handle to the parent object.
 * @param[in] nIndex       - The index of the KickstartTable entry.
 * @param[out] pInsNumber  - Pointer to store the instance number of the KickstartTable entry.
 *
 * @return The handle to the KickstartTable entry.
 * @retval Non-NULL handle if the entry is found.
 * @retval NULL if the entry is not found.
 *
 */
ANSC_HANDLE
KickstartTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Get string parameter value from KickstartTable entry.
 *
 * @param[in] hInsContext  - The instance handle to the KickstartTable entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not supported
 *
 */
LONG
KickstartTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set string parameter value for KickstartTable entry.
 *
 * @param[in] hInsContext - The instance handle to the KickstartTable entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] pString     - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
KickstartTable_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

#if !defined (RESOURCE_OPTIMIZATION)

/***********************************************************************

 APIs for Object:

    DeviceInfo.VendorConfigFile.{i}.

    *  VendorConfigFile_GetEntryCount
    *  VendorConfigFile_GetEntry
    *  VendorConfigFile_GetParamBoolValue
    *  VendorConfigFile_GetParamIntValue
    *  VendorConfigFile_GetParamUlongValue
    *  VendorConfigFile_GetParamStringValue
    *  VendorConfigFile_SetParamBoolValue
    *  VendorConfigFile_SetParamIntValue
    *  VendorConfigFile_SetParamUlongValue
    *  VendorConfigFile_SetParamStringValue
    *  VendorConfigFile_Validate
    *  VendorConfigFile_Commit
    *  VendorConfigFile_Rollback

***********************************************************************/
/**
 * @brief Get the number of VendorConfigFile entries.
 *
 * @param[in] hInsContext - The instance handle.
 *
 * @return The number of VendorConfigFile entries.
 *
 */
ULONG
VendorConfigFile_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a VendorConfigFile entry by index.
 *
 * @param[in] hInsContext  - The instance handle to the parent object.
 * @param[in] nIndex       - The index of the VendorConfigFile entry.
 * @param[out] pInsNumber  - Pointer to store the instance number of the VendorConfigFile entry.
 *
 * @return The handle to the VendorConfigFile entry.
 * @retval Non-NULL handle if the entry is found.
 * @retval NULL if the entry is not found.
 *
 */
ANSC_HANDLE
VendorConfigFile_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Get boolean parameter value from VendorConfigFile entry.
 *
 * @param[in] hInsContext - The instance handle to the VendorConfigFile entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VendorConfigFile_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from VendorConfigFile entry.
 *
 * @param[in] hInsContext - The instance handle to the VendorConfigFile entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VendorConfigFile_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from VendorConfigFile entry.
 *
 * @param[in] hInsContext - The instance handle to the VendorConfigFile entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VendorConfigFile_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from VendorConfigFile entry.
 *
 * @param[in] hInsContext  - The instance handle to the VendorConfigFile entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if Not suppported
 *
 */
ULONG
VendorConfigFile_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set boolean parameter value for VendorConfigFile entry.
 *
 * @param[in] hInsContext - The instance handle to the VendorConfigFile entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] bValue      - The boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VendorConfigFile_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set integer parameter value for VendorConfigFile entry.
 *
 * @param[in] hInsContext - The instance handle to the VendorConfigFile entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] value       - The integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VendorConfigFile_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set unsigned long parameter value for VendorConfigFile entry.
 *
 * @param[in] hInsContext  - The instance handle to the VendorConfigFile entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[in] uValuepUlong - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VendorConfigFile_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set string parameter value for VendorConfigFile entry.
 *
 * @param[in] hInsContext - The instance handle to the VendorConfigFile entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] strValue    - Pointer to the string value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
VendorConfigFile_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate VendorConfigFile entry parameter values.
 *
 * @param[in] hInsContext        - The instance handle to the VendorConfigFile entry.
 * @param[out] pReturnParamName  - Pointer to a buffer (128 bytes) to store the parameter name if there's a validation.
 * @param[in,out] puLength       - Pointer to the buffer length.
 *
 * @return The validation result.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 *
 */
BOOL
VendorConfigFile_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit VendorConfigFile entry parameter changes.
 *
 * @param[in] hInsContext - The instance handle to the VendorConfigFile entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
VendorConfigFile_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback VendorConfigFile entry parameter changes.
 *
 * @param[in] hInsContext - The instance handle to the VendorConfigFile entry.
 *
 * @return The status of the operation.
 *
 */
ULONG
VendorConfigFile_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif

/***********************************************************************

 APIs for Object:

    DeviceInfo.SupportedDataModel.{i}.

    *  SupportedDataModel_GetEntryCount
    *  SupportedDataModel_GetEntry
    *  SupportedDataModel_GetParamBoolValue
    *  SupportedDataModel_GetParamIntValue
    *  SupportedDataModel_GetParamUlongValue
    *  SupportedDataModel_GetParamStringValue

***********************************************************************/
/**
 * @brief Get the number of SupportedDataModel entries.
 *
 * @param[in] hInsContext - The instance handle.
 *
 * @return The number of SupportedDataModel entries.
 *
 */
ULONG
SupportedDataModel_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a SupportedDataModel entry by index.
 *
 * @param[in] hInsContext  - The instance handle to the parent object.
 * @param[in] nIndex       - The index of the SupportedDataModel entry.
 * @param[out] pInsNumber  - Pointer to store the instance number of the SupportedDataModel entry.
 *
 * @return The handle to the SupportedDataModel entry.
 * @retval Non-NULL handle if the entry is found.
 * @retval NULL if the entry is not found.
 *
 */
ANSC_HANDLE
SupportedDataModel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Get boolean parameter value from SupportedDataModel entry.
 *
 * @param[in] hInsContext - The instance handle to the SupportedDataModel entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
SupportedDataModel_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from SupportedDataModel entry.
 *
 * @param[in] hInsContext - The instance handle to the SupportedDataModel entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
SupportedDataModel_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from SupportedDataModel entry.
 *
 * @param[in] hInsContext - The instance handle to the SupportedDataModel entry.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
SupportedDataModel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from SupportedDataModel entry.
 *
 * @param[in] hInsContext  - The instance handle to the SupportedDataModel entry.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if not supported.
 *
 */
ULONG
SupportedDataModel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.MemoryStatus.

    *  MemoryStatus_GetParamBoolValue
    *  MemoryStatus_GetParamIntValue
    *  MemoryStatus_GetParamUlongValue
    *  MemoryStatus_GetParamStringValue
    *  MemoryStatus_SetParamUlongValue

***********************************************************************/
/**
 * @brief Get boolean parameter value from MemoryStatus object.
 *
 * @param[in] hInsContext - The instance handle to the MemoryStatus object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
MemoryStatus_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from MemoryStatus object.
 *
 * @param[in] hInsContext - The instance handle to the MemoryStatus object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
MemoryStatus_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from MemoryStatus object.
 *
 * @param[in] hInsContext - The instance handle to the MemoryStatus object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
MemoryStatus_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from MemoryStatus object.
 *
 * @param[in] hInsContext  - The instance handle to the MemoryStatus object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if if not supported.
 *
 */
ULONG
MemoryStatus_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set unsigned long parameter value for MemoryStatus object.
 *
 * @param[in] hInsContext - The instance handle to the MemoryStatus object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[in] uValue      - The unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is set successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
MemoryStatus_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.ProcessStatus.

    *  ProcessStatus_GetParamBoolValue
    *  ProcessStatus_GetParamIntValue
    *  ProcessStatus_GetParamUlongValue
    *  ProcessStatus_GetParamStringValue

***********************************************************************/

/**
 * @brief Get boolean parameter value from ProcessStatus object.
 *
 * @param[in] hInsContext - The instance handle to the ProcessStatus object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
ProcessStatus_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from ProcessStatus object.
 *
 * @param[in] hInsContext - The instance handle to the ProcessStatus object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
ProcessStatus_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from ProcessStatus object.
 *
 * @param[in] hInsContext - The instance handle to the ProcessStatus object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
ProcessStatus_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from ProcessStatus object.
 *
 * @param[in] hInsContext  - The instance handle to the ProcessStatus object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if not supported.
 *
 */
ULONG
ProcessStatus_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.ProcessStatus.Process.{i}.

    *  Process_GetEntryCount
    *  Process_GetEntry
    *  Process_IsUpdated
    *  Process_Synchronize
    *  Process_GetParamBoolValue
    *  Process_GetParamIntValue
    *  Process_GetParamUlongValue
    *  Process_GetParamStringValue

***********************************************************************/

#if !defined (RESOURCE_OPTIMIZATION)

/**
 * @brief Get the count of process table entries
 *
 * Retrieves the total number of process entries currently tracked in the Device.DeviceInfo.ProcessStatus.Process table.
 * This function is only available when RESOURCE_OPTIMIZATION is not defined.
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The total number of process entries
 */
ULONG
Process_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get a specific process table entry by index
 *
 * Retrieves a process entry from the Device.DeviceInfo.ProcessStatus.Process table at the specified index position.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] nIndex       Zero-based index of the entry to retrieve
 * @param[out] pInsNumber  Instance number of the retrieved entry
 *
 * @return Handle to the process entry context
 * @retval Non-NULL handle if found
 * @retval NULL if not found
 */
ANSC_HANDLE
Process_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Check if the process table has been updated
 *
 * Determines whether the process table data needs to be resynchronized.
 * Returns TRUE if the cached data is stale and needs refreshing.
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return Validation result
 * @retval TRUE if the process table needs update
 * @retval FALSE otherwise
 */
BOOL
Process_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronize the process table with current system state
 *
 * Refreshes the process table cache by reading current process information
 * from the operating system.
 *
 * @param[in] hInsContext  Instance context handle
 *
 * @return The status of the operation
 * @retval 0 on success,
 * @retval error code otherwise
 */
ULONG
Process_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Get a boolean parameter value from a process entry
 *
 * Retrieves a boolean parameter from a Device.DeviceInfo.ProcessStatus.Process
 * table entry.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Process_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter value from a process entry
 *
 * Retrieves an integer parameter from a Device.DeviceInfo.ProcessStatus.Process
 * table entry.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pInt        Pointer to store the integer value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Process_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter value from a process entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pUlong      Pointer to store the unsigned long value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Process_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter value from a process entry
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value.
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
Process_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif

/***********************************************************************

 APIs for Object:

    DeviceInfo.NetworkProperties.

    *  NetworkProperties_GetParamBoolValue
    *  NetworkProperties_GetParamIntValue
    *  NetworkProperties_GetParamUlongValue
    *  NetworkProperties_GetParamStringValue

***********************************************************************/
/**
 * @brief Get a boolean parameter from DeviceInfo.NetworkProperties
 *
 * Retrieves a boolean network properties parameter from the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
NetworkProperties_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter from DeviceInfo.NetworkProperties
 *
 * Retrieves an integer network properties parameter from the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pInt        Pointer to store the integer value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
NetworkProperties_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter from DeviceInfo.NetworkProperties
 *
 * Retrieves an unsigned long network properties parameter from the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pUlong      Pointer to store the unsigned long value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
NetworkProperties_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter from DeviceInfo.NetworkProperties
 *
 * Retrieves a string network properties parameter from the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
NetworkProperties_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#if defined (USE_REMOTE_DEBUGGER)
/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.RDKRemoteDebugger.Enable

    *  RDKRemoteDebugger_GetParamBoolValue
    *  RDKRemoteDebugger_SetParamBoolValue
***********************************************************************/
/**
 * @brief Get RDK Remote Debugger enable status
 *
 * Retrieves the enable status of the RDK Remote Debugger feature. This feature
 * allows remote debugging access to the device when enabled.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the enable status
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
RDKRemoteDebugger_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set RDK Remote Debugger enable status
 *
 * Enables or disables the RDK Remote Debugger feature on the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       TRUE to enable, FALSE to disable
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
RDKRemoteDebugger_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RDKDownloadManager.InstallPackage

    *  RDKDownloadManager_SetParamStringValue

***********************************************************************/
/**
 * @brief Set RDK Download Manager install package parameter
 *
 * Sets the package identifier to trigger installation via RDK Download Manager.
 * The manager will download and install the specified package.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] pString      Package identifier or URL to install
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
RDKDownloadManager_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

#endif

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.xMemInsight.Enable

    *  xMemInsight_SetParamBoolValue
    *  xMemInsight_GetParamBoolValue
***********************************************************************/

BOOL xMemInsight_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL xMemInsight_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);


/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.xMemInsight.Args

    *  xMemInsight_SetParamStringValue
    *  xMemInsight_GetParamStringValue
***********************************************************************/

ULONG xMemInsight_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL xMemInsight_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AccountInfo.AccountID

    *  AccountInfo_GetParamStringValue
    *  AccountInfo_SetParamStringValue
***********************************************************************/
/**
 * @brief Set account information parameter
 *
 * Sets account-related information such as AccountID for the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] pString      Account identifier string
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
AccountInfo_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Get account information parameter
 *
 * Retrieves account-related information such as AccountID from the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the account identifier
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
AccountInfo_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.RPC.

    *  RPC_GetParamUlongValue
    *  RPC_SetParamUlongValue
    *  RPC_GetParamStringValue
    *  RPC_SetParamStringValue

***********************************************************************/
/**
 * @brief Get an unsigned long RPC parameter value
 *
 * Retrieves an unsigned long parameter from the xOpsDeviceMgmt RPC configuration,
 * such as BindPort or other numeric RPC settings.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] puLong      Pointer to store the unsigned long value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
RPC_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief Set an unsigned long RPC parameter value
 *
 * Sets an unsigned long parameter in the xOpsDeviceMgmt RPC configuration.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] uValue       Unsigned long value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
RPC_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

/**
 * @brief Get a string RPC parameter value
 *
 * Retrieves a string parameter from the xOpsDeviceMgmt RPC configuration,
 * such as server address or other string-based RPC settings.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value
 * @param[in,out] pulSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
RPC_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
 * @brief Set a string RPC parameter value
 *
 * Sets a string parameter in the xOpsDeviceMgmt RPC configuration.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] pString      String value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
RPC_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_SwitchToDibbler.

    *  SwitchToDibbler_GetParamBoolValue
    *  SwitchToDibbler_SetParamBoolValue

***********************************************************************/
/**
 * @brief Get switch to Dibbler DHCPv6 client status
 *
 * Retrieves the configuration status for switching to Dibbler DHCPv6 client
 * instead of the default DHCP client.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the switch status
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
SwitchToDibbler_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set switch to Dibbler DHCPv6 client status
 *
 * Enables or disables switching to Dibbler DHCPv6 client on the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       TRUE to enable Dibbler, FALSE to use default client
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
SwitchToDibbler_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.CDLDM.CDLModuleUrl

    *  CDLDM_GetParamStringValue
    *  CDLDM_SetParamStringValue
***********************************************************************/
/**
 * @brief Set Code Download Manager module URL
 *
 * Sets the URL for the CDLDM (Code Download Manager) module download location.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] pString      URL string for the CDL module
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
CDLDM_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Get Code Download Manager module URL
 *
 * Retrieves the URL for the CDLDM (Code Download Manager) module download location.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the URL string
 * @param[in,out] pUlSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
CDLDM_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_SwitchToUDHCPC.

    *  SwitchToUDHCPC_GetParamBoolValue
    *  SwitchToUDHCPC_SetParamBoolValue

***********************************************************************/
/**
 * @brief Get switch to udhcpc DHCP client status
 *
 * Retrieves the configuration status for switching to udhcpc DHCP client
 * instead of the default DHCP client.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the switch status
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
SwitchToUDHCPC_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set switch to udhcpc DHCP client status
 *
 * Enables or disables switching to udhcpc DHCP client on the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       TRUE to enable udhcpc, FALSE to use default client
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
SwitchToUDHCPC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.ReverseSSH.

    *  ReverseSSH_GetParamStringValue
    *  ReverseSSH_SetParamStringValue

***********************************************************************/

/**
 * @brief Get Reverse SSH parameter value
 *
 * Retrieves a Reverse SSH configuration parameter such as trigger or connection
 * string for establishing reverse SSH tunnels for device management.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value.
 * @param[in,out] pulSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
ReverseSSH_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
 * @brief Set Reverse SSH parameter value
 *
 * Sets a Reverse SSH configuration parameter to enable remote management
 * via reverse SSH tunnel.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] pString      String value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
    BOOL
        ReverseSSH_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );


/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_Syndication.

    *  Syndication_GetParamStringValue
    *  Syndication_SetParamStringValue
    *  Syndication_GetParamBoolValue
    *  Syndication_SetParamBoolValue

***********************************************************************/

/**
 * @brief Get a boolean syndication parameter value
 *
 * Retrieves a boolean parameter from the syndication partner configuration,
 * such as feature enable flags or status indicators.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
Syndication_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set a boolean syndication parameter value
 *
 * Sets a boolean parameter in the syndication partner configuration.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       Boolean value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
Syndication_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set a string syndication parameter value
 *
 * Sets a string parameter in the syndication partner configuration, such as
 * partner ID, portal URL, or other configuration strings.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] pString      String value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
Syndication_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Get a string syndication parameter value
 *
 * Retrieves a string parameter from the syndication partner configuration.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pValue      Buffer to store the string value.
 * @param[in,out] pulSize  Size of the buffer (input), length of the string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 if Not supported
 */
ULONG
Syndication_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
 * @brief Get WAN-side SSH access enable status
 *
 * Retrieves the enable status for SSH access from the WAN side of the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the enable status
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
WANsideSSH_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set WAN-side SSH access enable status
 *
 * Enables or disables SSH access from the WAN side of the device.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       TRUE to enable WAN-side SSH, FALSE to disable
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
WANsideSSH_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Get RDKB control parameter value
 *
 * Retrieves a boolean control parameter for RDK-B management and configuration.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to retrieve
 * @param[out] pBool       Pointer to store the boolean value
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
RDKB_Control_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set RDKB control parameter value
 *
 * Sets a boolean control parameter for RDK-B management and configuration.
 *
 * @param[in] hInsContext  Instance context handle
 * @param[in] ParamName    Name of the parameter to set
 * @param[in] bValue       Boolean value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
RDKB_Control_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves boolean parameter values for RDKB UI Branding configuration.
 *
 * @param[in]  hInsContext Instance handle for the RDKB UI Branding context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve.
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise
 */
BOOL
RDKB_UIBranding_GetParamBoolValue

	(
		ANSC_HANDLE 				hInsContext,
		char*					ParamName,
		BOOL*                       		pBool
	);

/**
 * @brief Retrieves string parameter values for RDKB UI Branding configuration.
 *
 * @param[in]     hInsContext Instance handle for the RDKB UIBranding context.
 * @param[in]     ParamName   Name of the string parameter to retrieve .
 * @param[out]    pValue      Buffer to store the retrieved string value. Usually size of 1023 will be used.
 * @param[in,out] pulSize     Input: Size of pValue buffer; Output: Length of retrieved string.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer too small
 * @retval -1 on error
 */
ULONG
RDKB_UIBranding_GetParamStringValue

	(
        ANSC_HANDLE 				        hInsContext,
        char*						ParamName,
        char*						pValue,
        ULONG*						pulSize
	);

/**
 * @brief Sets string parameter values for RDKB UI Branding configuration.
 *
 * @param[in] hInsContext Instance handle for the RDKB_UIBranding context.
 * @param[in] ParamName   Name of the string parameter to set.
 * @param[in] pString     String value to set
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully,
 * @retval FALSE otherwise.
 */
BOOL
RDKB_UIBranding_SetParamStringValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       		ParamName,
        char*                       		pString
    );

/**
 * @brief Retrieves string parameter values for UI branding footer configuration.
 *
 * @param[in]     hInsContext Instance handle for the Footer context.
 * @param[in]     ParamName   Name of the string parameter to retrieve .
 * @param[out]    pValue      Buffer to store the retrieved string value.
 * @param[in,out] pulSize     Input: Size of pValue buffer; Output: Length of retrieved string.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 on error
 */
ULONG
Footer_GetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	);

/**
 * @brief Sets string parameter values for UI branding footer configuration.
 *
 * @param[in] hInsContext Instance handle for the Footer context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Footer_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Retrieves string parameter values for UI connection status configuration.
 *
 * @param[in]     hInsContext Instance handle for the Connection context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pulSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
Connection_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	);

/**
 * @brief Sets string parameter values for UI connection status configuration.
 *
 * @param[in] hInsContext Instance handle for the Connection context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Connection_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );


/**
 * @brief Retrieves string parameter values for network diagnostic tools configuration.
 *
 * @param[in]     hInsContext Instance handle for the NetworkDiagnosticTools context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pulSize     Input: Size of pValue buffer; Output: Length of retrieved string.  Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
ULONG
NetworkDiagnosticTools_GetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	);

/**
 * @brief Sets string parameter values for network diagnostic tools configuration.
 *
 * @param[in] hInsContext Instance handle for the NetworkDiagnosticTools context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
NetworkDiagnosticTools_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Retrieves boolean parameter values for WiFi personalization settings.
 *
 * @param[in]  hInsContext Instance handle for the WiFiPersonalization context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
WiFiPersonalization_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves string parameter values for WiFi personalization settings.
 *
 * @param[in]     hInsContext Instance handle for the WiFiPersonalization context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pulSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
WiFiPersonalization_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	);

/**
 * @brief Sets boolean parameter values for WiFi personalization settings.
 *
 * @param[in] hInsContext Instance handle for the WiFiPersonalization context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
WiFiPersonalization_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/**
 * @brief Sets string parameter values for WiFi personalization settings.
 *
 * @param[in] hInsContext Instance handle for the WiFiPersonalization context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
WiFiPersonalization_SetParamStringValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Retrieves string parameter values for Local UI configuration.
 *
 * @param[in]     hInsContext Instance handle for the LocalUI context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pulSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
LocalUI_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	);
/**
 * @brief Sets string parameter values for Local UI configuration.
 *
 * @param[in] hInsContext Instance handle for the LocalUI context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
LocalUI_SetParamStringValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Retrieves boolean parameter values for IPv6 subprefix configuration.
 *
 * @param[in]  hInsContext Instance handle for the IPv6subPrefix context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6subPrefix_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
/**
 * @brief Sets boolean parameter values for IPv6 subprefix configuration.
 *
 * @param[in] hInsContext Instance handle for the IPv6subPrefix context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6subPrefix_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/**
 * @brief Retrieves boolean parameter values for IPv6 on LnF (Lost and Found) interface.
 *
 * @param[in]  hInsContext Instance handle for the IPv6onLnF context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6onLnF_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
/**
 * @brief Sets boolean parameter values for IPv6 on LnF (Lost and Found) interface.
 *
 * @param[in] hInsContext Instance handle for the IPv6onLnF context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6onLnF_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/**
 * @brief Retrieves boolean parameter values for IPv6 on XHS (Xfinity Home Security) interface.
 *
 * @param[in]  hInsContext Instance handle for the IPv6onXHS context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6onXHS_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
/**
 * @brief Sets boolean parameter values for IPv6 on XHS (Xfinity Home Security) interface.
 *
 * @param[in] hInsContext Instance handle for the IPv6onXHS context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6onXHS_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/**
 * @brief Retrieves boolean parameter values for IPv6 on POD interface.
 *
 * @param[in]  hInsContext Instance handle for the IPv6onPOD context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6onPOD_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
/**
 * @brief Sets boolean parameter values for IPv6 on POD interface.
 *
 * @param[in] hInsContext Instance handle for the IPv6onPOD context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6onPOD_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/**
 * @brief Retrieves boolean parameter values for IPv6 on MoCA interface.
 *
 * @param[in]  hInsContext Instance handle for the IPv6onMoCA context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6onMoCA_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
/**
 * @brief Sets boolean parameter values for IPv6 on MoCA interface.
 *
 * @param[in] hInsContext Instance handle for the IPv6onMoCA context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
IPv6onMoCA_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/**
 * @brief Retrieves boolean parameter values for EvoStream Direct Connect feature.
 *
 * @param[in]  hInsContext Instance handle for the EvoStream_DirectConnect context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
EvoStream_DirectConnect_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for EvoStream Direct Connect feature.
 *
 * @param[in] hInsContext Instance handle for the EvoStream_DirectConnect context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
EvoStream_DirectConnect_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/**
 * @brief Retrieves boolean parameter values for Local UI configuration.
 *
 * @param[in]  hInsContext Instance handle for the LocalUI context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
LocalUI_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
/**
 * @brief Sets boolean parameter values for Local UI configuration.
 *
 * @param[in] hInsContext Instance handle for the LocalUI context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
LocalUI_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/**
 * @brief Retrieves string parameter values for help tip configuration.
 *
 * @param[in]     hInsContext Instance handle for the HelpTip context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pulSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
HelpTip_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	);
/**
 * @brief Sets string parameter values for help tip configuration.
 *
 * @param[in] hInsContext Instance handle for the HelpTip context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
HelpTip_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.
    *  CloudUI_GetParamStringValue
    *  CloudUI_SetParamStringValue

***********************************************************************/

/**
 * @brief Retrieves string parameter values for Cloud UI configuration.
 *
 * @param[in]     hInsContext Instance handle for the CloudUI context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pulSize     Input: Size of pValue buffer; Output: Length of retrieved string.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
CloudUI_GetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    );
/**
 * @brief Sets string parameter values for Cloud UI configuration.
 *
 * @param[in] hInsContext Instance handle for the CloudUI context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
CloudUI_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl
    *  SyndicationFlowControl_GetParamBoolValue
    *  SyndicationFlowControl_SetParamBoolValue
    *  SyndicationFlowControl_GetParamStringValue
    *  SyndicationFlowControl_SetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for Syndication Flow Control feature.
 *
 * @param[in]  hInsContext Instance handle for the SyndicationFlowControl context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
SyndicationFlowControl_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves string parameter values for Syndication Flow Control feature.
 *
 * @param[in]     hInsContext Instance handle for the SyndicationFlowControl context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
SyndicationFlowControl_GetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets boolean parameter values for RFC Feature configuration.
 *
 * @param[in] hInsContext Instance handle for the Feature context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Feature_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets boolean parameter values for Syndication Flow Control feature.
 *
 * @param[in] hInsContext Instance handle for the SyndicationFlowControl context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
SyndicationFlowControl_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Telemetry.MessageBusSource.Enable

    *  MessageBusSource_GetParamBoolValue
    *  MessageBusSource_SetParamBoolValue
***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for MessageBusSource enable configuration.
 *
 * @param[in]  hInsContext Instance handle for the MessageBusSource context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
MessageBusSource_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for MessageBusSource enable configuration.
 *
 * @param[in] hInsContext Instance handle for the MessageBusSource context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
MessageBusSource_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************
 *
 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.FWUpdate.AutoExcluded

    *  AutoExcluded_GetParamBoolValue
    *  AutoExcluded_SetParamBoolValue
    *  AutoExcluded_GetParamStringValue
    *  AutoExcluded_SetParamStringValue
***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for FWUpdate AutoExcluded feature.
 *
 * @param[in]  hInsContext Instance handle for the AutoExcluded context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
AutoExcluded_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for FWUpdate AutoExcluded feature.
 *
 * @param[in] hInsContext Instance handle for the AutoExcluded context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
AutoExcluded_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves string parameter values for FWUpdate AutoExcluded feature.
 *
 * @param[in]     hInsContext Instance handle for the AutoExcluded context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
AutoExcluded_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
);

/**
 * @brief Sets string parameter values for FWUpdate AutoExcluded feature.
 *
 * @param[in] hInsContext Instance handle for the AutoExcluded context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
AutoExcluded_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
);

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.OAUTH.
    *  OAUTH_GetParamStringValue
    *  OAUTH_SetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves string parameter values for OAUTH configuration.
 *
 * @param[in]     hInsContext Instance handle for the OAUTH context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
OAUTH_GetParamStringValue

    (
        ANSC_HANDLE        hInsContext,
        char*              ParamName,
        char*              pValue,
        ULONG*             pUlSize
    );

/**
 * @brief Sets string parameter values for OAUTH configuration.
 *
 * @param[in] hInsContext Instance handle for the OAUTH context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
OAUTH_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_EthernetWAN.

    *  EthernetWAN_GetParamStringValue

***********************************************************************/

/**
 * @brief Retrieves string parameter values for Ethernet WAN configuration.
 *
 * @param[in]     hInsContext Instance handle for the EthernetWAN context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
EthernetWAN_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );


/**
 * @brief Retrieves boolean parameter values for Telemetry configuration.
 *
 * @param[in]  hInsContext Instance handle for the Telemetry context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
Telemetry_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for Telemetry configuration.
 *
 * @param[in] hInsContext Instance handle for the Telemetry context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Telemetry_SetParamBoolValue
    (
        ANSC_HANDLE hInsContext,
	char* ParamName,
	BOOL bValue
    );


/**
 * @brief Retrieves string parameter values for Telemetry configuration.
 *
 * @param[in]     hInsContext Instance handle for the Telemetry context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
Telemetry_GetParamStringValue
    (
        ANSC_HANDLE hInsContext,
	char* ParamName,
	char* pValue,
        ULONG* pUlSize
    );

/**
 * @brief Sets string parameter values for Telemetry configuration.
 *
 * @param[in] hInsContext Instance handle for the Telemetry context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
Telemetry_SetParamStringValue
   (
        ANSC_HANDLE hInsContext,
	char* ParamName,
	char* pString
   );


/**********************************************************************

APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.MACsecRequired.Enable

    *  MACsecRequired_GetParamBoolValue
    *  MACsecRequired_SetParamBoolValue


**********************************************************************/

/**
 * @brief Retrieves boolean parameter values for MACsec required feature enable status.
 *
 * @param[in]  hInsContext Instance handle for the MACsecRequired context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
MACsecRequired_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for MACsec required feature enable status.
 *
 * @param[in] hInsContext Instance handle for the MACsecRequired context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
MACsecRequired_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );


/**********************************************************************

APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_EthernetWAN.MACsec

    *  EthernetWAN_MACsec_GetParamStringValue

**********************************************************************/

/**
 * @brief Retrieves string parameter values for Ethernet WAN MACsec configuration.
 *
 * @param[in]     hInsContext Instance handle for the EthernetWAN_MACsec context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
EthernetWAN_MACsec_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );


/***********************************************************************

 APIs for Object:

	Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.MocaAccountIsolation.Enable

    *  MocaAccountIsolation_GetParamBoolValue
    *  MocaAccountIsolation_SetParamBoolValue
***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for MoCA Account Isolation feature enable status.
 *
 * @param[in]  hInsContext Instance handle for the MocaAccountIsolation context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
MocaAccountIsolation_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for MoCA Account Isolation feature enable status.
 *
 * @param[in] hInsContext Instance handle for the MocaAccountIsolation context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
MocaAccountIsolation_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );


/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.CaptivePortalForNoCableRF.Enable

    *  CaptivePortalForNoCableRF_GetParamBoolValue
    *  CaptivePortalForNoCableRF_SetParamBoolValue
***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for Captive Portal for No Cable RF feature enable status.
 *
 * @param[in]  hInsContext Instance handle for the CaptivePortalForNoCableRF context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
CaptivePortalForNoCableRF_GetParamBoolValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for Captive Portal for No Cable RF feature enable status.
 *
 * @param[in] hInsContext Instance handle for the CaptivePortalForNoCableRF context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
CaptivePortalForNoCableRF_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );


/**********************************************************************

APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.BlockLostandFoundInternet.Enable

    *  BlockLostandFoundInternet_GetParamBoolValue
    *  BlockLostandFoundInternet_SetParamBoolValue


**********************************************************************/

/**
 * @brief Retrieves boolean parameter values for Block Lost and Found Internet feature enable status.
 *
 * @param[in]  hInsContext Instance handle for the BlockLostandFoundInternet context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
BlockLostandFoundInternet_GetParamBoolValue
  (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for Block Lost and Found Internet feature enable status.
 *
 * @param[in] hInsContext Instance handle for the BlockLostandFoundInternet context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
BlockLostandFoundInternet_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.CredDwnld.Enable
    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.CredDwnld.Use

    *  CredDwnld_GetParamBoolValue
    *  CredDwnld_SetParamBoolValue
    *  CredDwnld_GetParamStringValue
    *  CredDwnld_SetParamStringValue
***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for Credential Download feature.
 *
 * @param[in]  hInsContext Instance handle for the CredDwnld context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
CredDwnld_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for Credential Download feature.
 *
 * @param[in] hInsContext Instance handle for the CredDwnld context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
CredDwnld_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves string parameter values for Credential Download feature.
 *
 * @param[in]     hInsContext Instance handle for the CredDwnld context.
 * @param[in]     ParamName   Name of the string parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out]    pValue      Buffer to store the retrieved string value (minimum 1 byte, maximum 1024 bytes).
 * @param[in,out] pUlSize     Input: Size of pValue buffer; Output: Length of retrieved string. Usually size of 1023 will be used.
 *
 * @return The status of the operation
 * @retval 0 on success
 * @retval 1 if buffer size is insufficient
 * @retval -1 if parameter not supported.
 */
ULONG
CredDwnld_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets string parameter values for Credential Download feature.
 *
 * @param[in] hInsContext Instance handle for the CredDwnld context.
 * @param[in] ParamName   Name of the string parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] pString     String value to set (minimum 0 bytes, maximum 1024 bytes).
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
CredDwnld_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/**
 * @brief Retrieves boolean parameter values for Active Measurements RFC feature.
 *
 * @param[in]  hInsContext Instance handle for the ActiveMeasurements RFC context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ActiveMeasurements_RFC_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for Active Measurements RFC feature.
 *
 * @param[in] hInsContext Instance handle for the ActiveMeasurements RFC context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
ActiveMeasurements_RFC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves boolean parameter values for WPA3 Personal Transition RFC feature.
 *
 * @param[in]  hInsContext Instance handle for the WPA3 Personal Transition_RFC context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
WPA3_Personal_Transition_RFC_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for WPA3 Personal Transition RFC feature.
 *
 * @param[in] hInsContext Instance handle for the WPA3 Personal Transition RFC context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
WPA3_Personal_Transition_RFC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves boolean parameter values for Errors Received RFC feature.
 *
 * @param[in]  hInsContext Instance handle for the Errors Received RFC context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
ErrorsReceived_RFC_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for Errors Received RFC feature.
 *
 * @param[in] hInsContext Instance handle for the Errors Received RFC context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
ErrorsReceived_RFC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves boolean parameter values for Download CA Store RFC feature.
 *
 * @param[in]  hInsContext Instance handle for the DLCa Store RFC context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
DLCaStore_RFC_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for Download CA Store RFC feature.
 *
 * @param[in] hInsContext Instance handle for the DLCa Store RFC context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
DLCaStore_RFC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SHORTS.Enable

    *  SHORTS_GetParamBoolValue
    *  SHORTS_SetParamBoolValue

***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for SHORTS (Self-Healing On-demand Reboot To Service) feature.
 *
 * @param[in]  hInsContext Instance handle for the SHORTS context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
SHORTS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for SHORTS (Self-Healing On-demand Reboot To Service) feature.
 *
 * @param[in] hInsContext Instance handle for the SHORTS context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
SHORTS_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.CRL.Enable

    *  EnableOCSPStapling_GetParamBoolValue
    *  EnableOCSPStapling_SetParamBoolValue
***********************************************************************/
/**
 * @brief Retrieves boolean parameter values for OCSP (Online Certificate Status Protocol) Stapling feature.
 *
 * @param[in]  hInsContext Instance handle for the Enable OCSP Stapling context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
EnableOCSPStapling_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for OCSP (Online Certificate Status Protocol) Stapling feature.
 *
 * @param[in] hInsContext Instance handle for the Enable OCSP Stapling context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
EnableOCSPStapling_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

#if defined(FEATURE_MAPT) || defined(FEATURE_SUPPORT_MAPT_NAT46)
/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.MAP-T.Enable

    *  MAPT_DeviceInfo_GetParamBoolValue
    *  MAPT_DeviceInfo_SetParamBoolValue
***********************************************************************/


/**
 * @brief Retrieves boolean parameter values for MAP-T (Mapping of Address and Port with Translation) feature.
 *
 * @param[in]  hInsContext Instance handle for the MAPT DeviceInfo context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
MAPT_DeviceInfo_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Sets boolean parameter values for MAP-T (Mapping of Address and Port with Translation) feature.
 *
 * @param[in] hInsContext Instance handle for the MAPT DeviceInfo context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
MAPT_DeviceInfo_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

#endif

/**
 * @brief Retrieves boolean parameter values for Latency Measurement TCP Setup over IPv4.
 *
 * @param[in]  hInsContext Instance handle for the Latency Measure Tcp Setup IPv4 context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
LatencyMeasureTcpSetupIPv4_GetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL*						pBool
	);

/**
 * @brief Sets boolean parameter values for Latency Measurement TCP Setup over IPv4.
 *
 * @param[in] hInsContext Instance handle for the Latency Measure Tcp Setup IPv4 context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
LatencyMeasureTcpSetupIPv4_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves boolean parameter values for Latency Measurement TCP Setup over IPv6.
 *
 * @param[in]  hInsContext Instance handle for the Latency Measure Tcp Setup IPv6 context.
 * @param[in]  ParamName   Name of the boolean parameter to retrieve (minimum 1 byte, maximum 256 bytes).
 * @param[out] pBool       Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
LatencyMeasureTcpSetupIPv6_GetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL*						pBool
	);

/**
 * @brief Sets boolean parameter values for Latency Measurement TCP Setup over IPv6.
 *
 * @param[in] hInsContext Instance handle for the Latency Measure TCP Setup IPv6 context.
 * @param[in] ParamName   Name of the boolean parameter to set (minimum 1 byte, maximum 256 bytes).
 * @param[in] bValue      Boolean value to set.
 *
 * @return The status of the operation
 * @retval TRUE if parameter is supported and set successfully
 * @retval FALSE otherwise.
 */
BOOL
LatencyMeasureTcpSetupIPv6_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );
#endif
