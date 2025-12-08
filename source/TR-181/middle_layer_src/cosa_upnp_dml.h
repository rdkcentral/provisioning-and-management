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

    module: cosa_upnp_dml.h

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


#ifndef  _COSA_UPNP_DML_H
#define  _COSA_UPNP_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_upnp_internal.h"

/***********************************************************************

 APIs for Object:

    UPnP.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    UPnP.

    *  Device_GetParamBoolValue
    *  Device_GetParamIntValue
    *  Device_GetParamUlongValue
    *  Device_GetParamStringValue
    *  Device_SetParamBoolValue
    *  Device_SetParamIntValue
    *  Device_SetParamUlongValue
    *  Device_SetParamStringValue
    *  Device_Validate
    *  Device_Commit
    *  Device_Rollback

***********************************************************************/
/**
 * @brief Get a boolean parameter value from UPnP.Device.
 *
 * Retrieves the value of a boolean parameter for Device.UPnP.Device.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
  */
BOOL
Device_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter value from UPnP.Device.
 *
 * Retrieves the value of an integer parameter for Device.UPnP.Device.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 * @param[in] ParamName Name of the integer parameter to retrieve.
 * @param[out] pInt Pointer to store the retrieved integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Device_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter value from UPnP.Device.
 *
 * Retrieves the value of an unsigned long parameter for Device.UPnP.Device.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 * @param[in] ParamName Name of the unsigned long parameter to retrieve.
 * @param[out] pUlong Pointer to store the retrieved unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Device_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter value from UPnP.Device.
 *
 * Retrieves the value of a string parameter for Device.UPnP.Device.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 * @param[in] ParamName Name of the string parameter.
 * @param[out] pValue Buffer to store the retrieved string value.
 * @param[in,out] pUlSize Size of the pValue buffer (input), length of retrieved string (output).
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
Device_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Set a boolean parameter value for UPnP.Device.
 *
 * Sets the value of a boolean parameter for Device.UPnP.Device..
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Device_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Set an integer parameter value for UPnP.Device.
 *
 * Sets the value of an integer parameter for Device.UPnP.Device.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 * @param[in] ParamName Name of the integer parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Device_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Set an unsigned long parameter value for UPnP.Device.
 *
 * Sets the value of an unsigned long parameter for Device.UPnP.Device.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 * @param[in] ParamName Name of the unsigned long parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Device_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Set a string parameter value for UPnP.Device.
 *
 * Sets the value of a string parameter for Device.UPnP.Device.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 * @param[in] ParamName Name of the string parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
Device_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validate pending parameter changes for UPnP.Device.
 *
 * Validates all pending parameter changes for Device.UPnP.Device object before committing.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 * @param[out] pReturnParamName Buffer to store the name of the parameter,if there's a validation(128 bytes).
 * @param[in,out] puLength Size of pReturnParamName buffer.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
Device_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commit pending parameter changes for UPnP.Device.
 *
 * Commits all validated pending parameter changes for Device.UPnP.Device to the backend.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
Device_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rollback pending parameter changes for UPnP.Device.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
Device_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    UPnP.Capabilities.

    *  Capabilities_GetParamBoolValue
    *  Capabilities_GetParamIntValue
    *  Capabilities_GetParamUlongValue
    *  Capabilities_GetParamStringValue

***********************************************************************/
/**
 * @brief Get a boolean parameter value from UPnP.Device.Capabilities.
 *
 * Retrieves the value of a boolean parameter for Device.UPnP.Device.Capabilities.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.Capabilities.
 * @param[in] ParamName Name of the boolean parameter.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Capabilities_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter value from UPnP.Device.Capabilities.
 *
 * Retrieves the value of an integer parameter for Device.UPnP.Device.Capabilities.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.Capabilities.
 * @param[in] ParamName Name of the integer parameter to retrieve.
 * @param[out] pInt Pointer to store the retrieved integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Capabilities_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter value from UPnP.Device.Capabilities.
 *
 * Retrieves the value of an unsigned long parameter for Device.UPnP.Device.Capabilities.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.Capabilities.
 * @param[in] ParamName Name of the unsigned long parameter to retrieve.
 * @param[out] pUlong Pointer to store the retrieved unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Capabilities_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter value from UPnP.Device.Capabilities.
 *
 * Retrieves the value of a string parameter for Device.UPnP.Device.Capabilities.
 *
 * @param[in] hInsContext Instance context handle for UPnP.Device.Capabilities.
 * @param[in] ParamName Name of the string parameter to retrieve.
 * @param[out] pValue Buffer to store the retrieved string value.
 * @param[in,out] pUlSize Size of the pValue buffer (input), length of retrieved string (output).
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
Capabilities_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
