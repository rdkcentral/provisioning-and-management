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

    module: cosa_userinterface_dml.h

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


#ifndef  _COSA_USERINTERFACE_DML_H
#define  _COSA_USERINTERFACE_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_userinterface_internal.h"

/***********************************************************************

 APIs for Object:

    UserInterface.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    UserInterface..

    *  UserInterface_GetParamBoolValue
    *  UserInterface_GetParamUlongValue
    *  UserInterface_SetParamBoolValue
    *  UserInterface_SetParamUlongValue
    *  RemoteAccess_Validate
    *  RemoteAccess_Commit
    *  RemoteAccess_Rollback

***********************************************************************/
/**
 * @brief Retrieves a boolean parameter value from Device.UserInterface.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
UserInterface_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
/**
 * @brief Sets a boolean parameter value for Device.UserInterface.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise.
 */
BOOL
UserInterface_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieves an unsigned long parameter value from Device.UserInterface.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] puLong Pointer to receive the unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
UserInterface_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief Sets an unsigned long parameter value for Device.UserInterface.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValue Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise.
 */
BOOL
UserInterface_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

/**
 * @brief Validates pending changes to Device.UserInterface.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer(128 bytes) to receive the name of the parameter if there is a validation .
 * @param[in,out] puLength Pointer to buffer size; receives actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there is no validation
 * @retval FALSE if there's validation.
 */
BOOL
UserInterface_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending changes to Device.UserInterface.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return status of operation
 * @retval 0 on success
 * @retval error code otherwise.
 */
ULONG
UserInterface_Commit
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    UserInterface.RemoteAccess.

    *  RemoteAccess_GetParamBoolValue
    *  RemoteAccess_GetParamIntValue
    *  RemoteAccess_GetParamUlongValue
    *  RemoteAccess_GetParamStringValue
    *  RemoteAccess_SetParamBoolValue
    *  RemoteAccess_SetParamIntValue
    *  RemoteAccess_SetParamUlongValue
    *  RemoteAccess_SetParamStringValue
    *  RemoteAccess_Validate
    *  RemoteAccess_Commit
    *  RemoteAccess_Rollback

***********************************************************************/
/**
 * @brief Retrieves a boolean parameter value from Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return status of operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
RemoteAccess_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter value from Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to receive the integer value.
 *
 * @return status of operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
RemoteAccess_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter value from Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to receive the unsigned long value.
 *
 * @return status of operation
 * @retval TRUE if parameter found and retrieved successfully
 * @retval FALSE otherwise.
 */
BOOL
RemoteAccess_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter value from Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Buffer to receive the string value (typically 256 bytes).
 * @param[in,out] pUlSize Pointer to buffer size; receives actual string length. Usually the size of 1023 is used.
 *
 * @return status of operation
 * @retval 0 if successful
 * @retval 1 if buffer too small
 * @retval -1 if not supported
 */
ULONG
RemoteAccess_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter value for Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE  otherwise
 */
BOOL
RemoteAccess_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets an integer parameter value for Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise.
 */
BOOL
RemoteAccess_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets an unsigned long parameter value for Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise
 */
BOOL
RemoteAccess_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets a string parameter value for Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return status of operation
 * @retval TRUE if parameter set successfully
 * @retval FALSE otherwise.
 */
BOOL
RemoteAccess_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending changes to Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer(128 bytes) to receive the name of the parameter if there is a validation.
 * @param[in,out] puLength Pointer to buffer size; receives actual length of parameter name.
 *
 * @return status of operation
 * @retval TRUE if there is no validation
 * @retval FALSE if there's validation.
 */
BOOL
RemoteAccess_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending changes to Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return status of operation
 * @retval 0 on success
 * @retval error code otherwise.
 */
ULONG
RemoteAccess_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending changes to Device.UserInterface.RemoteAccess.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return
 0 on success, error code otherwise.
 */
ULONG
RemoteAccess_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
