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

    module: cosa_time_dml.h

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


#ifndef  _COSA_TIME_DML_H
#define  _COSA_TIME_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_time_internal.h"

/***********************************************************************

 APIs for Object:

    Time.

    *  Time_GetParamBoolValue
    *  Time_GetParamIntValue
    *  Time_GetParamUlongValue
    *  Time_GetParamStringValue
    *  Time_SetParamBoolValue
    *  Time_SetParamIntValue
    *  Time_SetParamUlongValue
    *  Time_SetParamStringValue
    *  Time_Validate
    *  Time_Commit
    *  Time_Rollback

***********************************************************************/
/**
 * @brief Get a boolean parameter value from Device.Time.
 *
 * Retrieves the value of a boolean parameter for Device.Time..
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Time_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter value from Device.Time.
 *
 * Retrieves the value of an integer parameter for Device.Time.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 * @param[in] ParamName Name of the integer parameter to retrieve.
 * @param[out] pInt Pointer to store the retrieved integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Time_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter value from Device.Time.
 *
 * Retrieves the value of an unsigned long parameter for Device.Time.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 * @param[in] ParamName Name of the unsigned long parameter to retrieve.
 * @param[out] pUlong Pointer to store the retrieved unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Time_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter value from Device.Time.
 *
 * Retrieves the value of a string parameter for Device.Time.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 * @param[in] ParamName Name of the string parameter.
 * @param[out] pValue Buffer to store the retrieved string value.
 * @param[in,out] pUlSize Size of the pValue buffer (input), length of retrieved string (output). Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 if not supported.
 */
ULONG
Time_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set a boolean parameter value for Device.Time.
 *
 * Sets the value of a boolean parameter for Device.Time.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Time_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set an integer parameter value for Device.Time.
 *
 * Sets the value of an integer parameter for Device.Time.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 * @param[in] ParamName Name of the integer parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Time_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set an unsigned long parameter value for Device.Time.
 *
 * Sets the value of an unsigned long parameter for Device.Time.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 * @param[in] ParamName Name of the unsigned long parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Time_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set a string parameter value for Device.Time.
 *
 * Sets the value of a string parameter for Device.Time.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 * @param[in] ParamName Name of the string parameter.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Time_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate pending parameter changes for Device.Time.
 *
 * Validates all pending parameter changes for Device.Time object before committing.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 * @param[out] pReturnParamName Buffer to store the name of the parameter, if there's a validation(128 bytes).
 * @param[in,out] puLength Size of pReturnParamName buffer.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Time_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit pending parameter changes for Device.Time.
 *
 * Commits all validated pending parameter changes for Device.Time to the backend.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
Time_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback pending parameter changes for Device.Time.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle for Device.Time.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
Time_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
