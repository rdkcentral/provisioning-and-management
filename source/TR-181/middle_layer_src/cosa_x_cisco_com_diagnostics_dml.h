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

    module: cosa_x_cisco_com_mld_dml.h

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


#ifndef  _COSA_MLD_DML_H
#define  _COSA_MLD_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_diagnostics_internal.h"



/***********************************************************************

 APIs for Object:

    X_CISCO_COM_MLD.Group.{i}.

    *  Entry_GetEntryCount
    *  Entry_GetEntry
    *  Entry_IsUpdated
    *  Entry_Synchronize
    *  Entry_GetParamBoolValue
    *  Entry_GetParamIntValue
    *  Entry_GetParamUlongValue
    *  Entry_GetParamStringValue

***********************************************************************/
/**
* @brief Get the number of diagnostic entries.
*
* @param[in] hInsContext - The instance handle to the diagnostics object.
*
* @return The number of diagnostic entries.
*
*/
ULONG
Entry_GetEntryCount
    (
        ANSC_HANDLE
    );
/**
* @brief Get a specific diagnostic entry by index.
*
* @param[in] hInsContext  - The instance handle to the diagnostics object.
* @param[in] nIndex       - The zero-based index of the entry to retrieve.
* @param[out] pInsNumber  - Pointer to a variable to receive the instance number of the entry.
*
* @return Handle to the diagnostic entry
* @return Non-NULL Handle to the diagnostic entry.
* @retval NULL if the entry is not found
*/
ANSC_HANDLE
Entry_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );
/**
* @brief Check if the diagnostic entry table needs to be updated.
*
* @param[in] hInsContext - The instance handle to the diagnostics object.
*
* @return The update status.
* @retval TRUE if the entry table needs to be refreshed.
* @retval FALSE if the entry table is still current.
*
*/
BOOL
Entry_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );
/**
* @brief Synchronize the diagnostic entry table with the current system state.
*
* @param[in] hInsContext - The instance handle to the diagnostics object.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
*
*/
ULONG
Entry_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );
/**
* @brief Get boolean parameter value from diagnostic entry.
*
* @param[in] hInsContext - The instance handle to the diagnostic entry object.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pBool      - Pointer to store the boolean value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
Entry_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
/**
* @brief Get integer parameter value from diagnostic entry.
*
* @param[in] hInsContext - The instance handle to the diagnostic entry object.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pInt       - Pointer to store the integer value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
Entry_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );
/**
* @brief Get unsigned long parameter value from diagnostic entry.
*
* @param[in] hInsContext - The instance handle to the diagnostic entry object.
* @param[in] ParamName   - Pointer to the parameter name.
* @param[out] pUlong     - Pointer to store the unsigned long value.
*
* @return The status of the operation.
* @retval TRUE if the parameter is found and retrieved successfully.
* @retval FALSE if the parameter is not found or operation fails.
*
*/
BOOL
Entry_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );
/**
* @brief Get string parameter value from diagnostic entry.
*
* @param[in] hInsContext  - The instance handle to the diagnostic entry object.
* @param[in] ParamName    - Pointer to the parameter name.
* @param[out] pValue      - Pointer to a buffer where the string value will be stored.
* @param[in,out] pUlSize  - Pointer to the buffer size; updated with actual size on return. Usually size of 4095 will be used.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval 1 if the buffer is too small; pUlSize updated with required size.
* @retval -1 if not supported.
*
*/
ULONG
Entry_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
