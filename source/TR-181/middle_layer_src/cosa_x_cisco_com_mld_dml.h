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
#include "cosa_x_cisco_com_mld_internal.h"

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_IGMP.

    *  X_CISCO_COM_MLD_GetParamBoolValue
    *  X_CISCO_COM_MLD_GetParamIntValue
    *  X_CISCO_COM_MLD_GetParamUlongValue
    *  X_CISCO_COM_MLD_GetParamStringValue
    *  X_CISCO_COM_MLD_SetParamBoolValue
    *  X_CISCO_COM_MLD_SetParamIntValue
    *  X_CISCO_COM_MLD_SetParamUlongValue
    *  X_CISCO_COM_MLD_SetParamStringValue
    *  X_CISCO_COM_MLD_Validate
    *  X_CISCO_COM_MLD_Commit
    *  X_CISCO_COM_MLD_Rollback

***********************************************************************/
/**
 * @brief Retrieves a boolean parameter value from Device.X_CISCO_COM_MLD.
 *
 * This function returns boolean parameters.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
X_CISCO_COM_MLD_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter value from Device.X_CISCO_COM_MLD.
 *
 * This function returns integer parameters from the MLD object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to receive the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
X_CISCO_COM_MLD_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter value from Device.X_CISCO_COM_MLD.
 *
 * This function returns ULONG parameters from the MLD object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to receive the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
X_CISCO_COM_MLD_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter value from Device.X_CISCO_COM_MLD.
 *
 * This function returns string parameters from the MLD object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Buffer to receive the string value (typically 256 bytes).
 * @param[in,out] pUlSize Pointer to buffer size; receives actual string length. Usually size of 4095 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 not supported.
 */
ULONG
X_CISCO_COM_MLD_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief Sets a boolean parameter value for Device.X_CISCO_COM_MLD.
 *
 * This function sets boolean parameters from the MLD object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] bValue Boolean value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_MLD_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Sets an integer parameter value for Device.X_CISCO_COM_MLD.
 *
 * This function sets integer parameters in the MLD snooping object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] value Integer value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_MLD_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief Sets an unsigned long parameter value for Device.X_CISCO_COM_MLD.
 *
 * This function sets ULONG parameters in the MLD object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] uValuepUlong Unsigned long value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_MLD_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief Sets a string parameter value for Device.X_CISCO_COM_MLD.
 *
 * This function sets string parameters in the MLD object.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to set.
 * @param[in] strValue String value to set.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
X_CISCO_COM_MLD_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief Validates pending changes to Device.X_CISCO_COM_MLD.
 *
 * This function validates cached parameter changes before committing.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[out] pReturnParamName Buffer to receive the name of the parameter, if there's a validation (typically 128 bytes).
 * @param[in,out] puLength Pointer to buffer size; receives actual length of parameter name.
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
X_CISCO_COM_MLD_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief Commits pending changes to Device.X_CISCO_COM_MLD.
 *
 * This function applies all cached changes to the MLD configuration.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
X_CISCO_COM_MLD_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Rolls back pending changes to Device.X_CISCO_COM_MLD.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
X_CISCO_COM_MLD_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_MLD.Group.{i}.

    *  Group1_GetEntryCount
    *  Group1_GetEntry
    *  Group1_IsUpdated
    *  Group1_Synchronize
    *  Group1_GetParamBoolValue
    *  Group1_GetParamIntValue
    *  Group1_GetParamUlongValue
    *  Group1_GetParamStringValue

***********************************************************************/
/**
 * @brief Retrieves the number of Group entries in the Device.X_CISCO_COM_MLD.Group table.
 *
 * This function returns the count of IPv6 multicast groups discovered by MLD.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The number of Group entries in the table.
 */
ULONG
Group1_GetEntryCount
    (
        ANSC_HANDLE
    );

/**
 * @brief Retrieves a Group entry by index from the Device.X_CISCO_COM_MLD.Group table.
 *
 * This function returns the specified MLD multicast group entry for
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] nIndex Zero-based index of the entry to retrieve.
 * @param[out] pInsNumber Pointer to receive the instance number of the entry.
 *
 * @return Handle to the Group entry
 * @retval non-NULL handle to Group entry.
 * @retval NULL if not found.
 */
ANSC_HANDLE
Group1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief Checks if the Group table has been updated.
 *
 * This function checks whether MLD multicast group discoveries have changed since the last synchronization.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The update status.
 * @retval TRUE if Group table has been updated.
 * @retval FALSE if Group entry table is still current.
 */
BOOL
Group1_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Synchronizes the Group table with current MLD snooping data.
 *
 * This function forces a refresh of the Group table from MLD snooping backend,
 * updating multicast group membership information.
 *
 * @param[in] hInsContext Instance context handle.
 *
 * @return The status of the operation.
 * @retval 0 if successful.
 * @retval error code otherwise.
 */
ULONG
Group1_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief Retrieves a boolean parameter value from a Group entry.
 *
 * This function returns boolean parameters from Device.X_CISCO_COM_MLD.Group.{i}.
 *
 * @param[in] hInsContext Instance context handle to the Group entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pBool Pointer to receive the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Group1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Retrieves an integer parameter value from a Group entry.
 *
 * This function returns integer parameters from Device.X_CISCO_COM_MLD.Group.{i}.
 *
 * @param[in] hInsContext Instance context handle to the Group entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pInt Pointer to receive the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Group1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Retrieves an unsigned long parameter value from a Group entry.
 *
 * This function returns ULONG parameters from Device.X_CISCO_COM_MLD.Group.{i}.
 *
 * @param[in] hInsContext Instance context handle to the Group entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pUlong Pointer to receive the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
Group1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Retrieves a string parameter value from a Group entry.
 *
 * This function returns string parameters from Device.X_CISCO_COM_MLD.Group.{i}.
 *
 * @param[in] hInsContext Instance context handle to the Group entry.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Buffer to receive the string value (typically 256 bytes).
 * @param[in,out] pUlSize Pointer to buffer size; receives actual string length. Usually size of 4095 will be used.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 not supported..
 */
ULONG
Group1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
