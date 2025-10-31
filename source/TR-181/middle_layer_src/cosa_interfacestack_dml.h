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

    module: cosa_interfacestack_dml.h

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


#ifndef  _COSA_INTERFACESTACK_DML_H
#define  _COSA_INTERFACESTACK_DML_H

/***********************************************************************

 APIs for Object:

    InterfaceStack.{i}.

    *  InterfaceStack_GetEntryCount
    *  InterfaceStack_GetEntry
    *  InterfaceStack_IsUpdated
    *  InterfaceStack_Synchronize
    *  InterfaceStack_GetParamBoolValue
    *  InterfaceStack_GetParamIntValue
    *  InterfaceStack_GetParamUlongValue
    *  InterfaceStack_GetParamStringValue

***********************************************************************/
/**
 * @brief Get the number of InterfaceStack entries.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return Number of InterfaceStack entries in the table.
 * @retval ULONG The number of entries.
 */
ULONG
InterfaceStack_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Get an InterfaceStack entry by index.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the InterfaceStack entry to retrieve.
 * @param[out] pInsNumber Pointer to store the instance number of the retrieved entry.
 *
 * @return Handle to the InterfaceStack entry context
 * @retval ANSC_HANDLE Handle to the InterfaceStack entry context.
 * @retval NULL if index is out of range.
 */
ANSC_HANDLE
InterfaceStack_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Check if the InterfaceStack table has been updated.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval TRUE if the table has been updated.
 * @retval FALSE otherwise.
 */
BOOL
InterfaceStack_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronize the InterfaceStack table with backend.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval error code otherwise.
 */
ULONG
InterfaceStack_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Get a boolean parameter value from InterfaceStack entry.
 *
 * @param[in] hInsContext Instance context handle for the InterfaceStack entry.
 * @param[in] ParamName Name of the boolean parameter to retrieve.
 * @param[out] pBool Pointer to store the retrieved boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved.
 * @retval FALSE otherwise.
 */
BOOL
InterfaceStack_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get an integer parameter value from InterfaceStack entry.
 *
 * @param[in] hInsContext Instance context handle for the InterfaceStack entry.
 * @param[in] ParamName Name of the integer parameter to retrieve.
 * @param[out] pInt Pointer to store the retrieved integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved.
 * @retval FALSE otherwise.
 */
BOOL
InterfaceStack_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get an unsigned long parameter value from InterfaceStack entry.
 *
 * @param[in] hInsContext Instance context handle for the InterfaceStack entry.
 * @param[in] ParamName Name of the unsigned long parameter to retrieve.
 * @param[out] pUlong Pointer to store the retrieved unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if parameter found and retrieved.
 * @retval FALSE otherwise.
 */
BOOL
InterfaceStack_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get a string parameter value from InterfaceStack entry.
 *
 * @param[in] hInsContext Instance context handle for the InterfaceStack entry.
 * @param[in] ParamName Name of the string parameter.
 * @param[out] pValue Buffer to store the retrieved string value.
 * @param[in,out] pUlSize Size of the pValue buffer (input), length of retrieved string (output). Usually size of 1023 is used.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval 1 if buffer too small.
 * @retval -1 if not supported.
 */
ULONG
InterfaceStack_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
