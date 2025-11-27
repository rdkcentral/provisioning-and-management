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

    module: cosa_nat_dml_custom.h

    For Data Model Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        The prototypes of custom data model SBAPI are defined here

    ---------------------------------------------------------------

    environment:

        Maybe platform independent, but customer specific

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        05/08/2014  initial revision.

**************************************************************************/


#ifndef  _COSA_NAT_DML_CUSTOM_H
#define  _COSA_NAT_DML_CUSTOM_H

/**********************************************************************
         PROTOTYPES of CUSTOM DATA MODEL PROCESSING FUNCTIONS
**********************************************************************/

/**
 * @brief Retrieve custom boolean parameter values from the NAT configuration.
 *
 * This function retrieves custom boolean parameter values specific to NAT configuration,
 * including port mapping and NAT mapping enable states.
 *
 * @param[in] hInsContext - The instance handle context for the NAT configuration.
 * @param[in] ParamName - The name of the custom boolean parameter to retrieve.
 * @param[out] pBool - Pointer to a boolean variable where the retrieved value will be stored.
 *                     \n Returns TRUE if the feature is enabled, FALSE otherwise.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter was successfully retrieved.
 * @retval FALSE if the parameter name is not recognized or retrieval failed.
 */
BOOL
NAT_GetParamBoolValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Set custom boolean parameter values in the NAT configuration.
 *
 * This function sets custom boolean parameter values specific to NAT configuration,
 * including port mapping and NAT mapping enable states.
 *
 * @param[in] hInsContext - The instance handle context for the NAT configuration.
 * @param[in] ParamName - The name of the custom boolean parameter to set.
 * @param[in] bValue - The boolean value to set for the parameter.
 *                     \n TRUE to enable the feature, FALSE to disable.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter was successfully set.
 * @retval FALSE if the parameter name is not recognized, device is in bridge mode, or setting failed.
 */
BOOL
NAT_SetParamBoolValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief Retrieve custom unsigned long parameter values from a NAT port mapping entry.
 *
 * This function retrieves custom unsigned long parameter values specific to a NAT port mapping
 * configuration.
 *
 * @param[in] hInsContext - The instance handle context for the port mapping entry.
 * @param[in] ParamName - The name of the custom unsigned long parameter to retrieve.
 * @param[out] puLong - Pointer to an unsigned long variable where the retrieved value will be stored.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter was successfully retrieved.
 * @retval FALSE if the parameter name is not recognized.
 */
BOOL
PortMapping_GetParamUlongValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief Set custom unsigned long parameter values in a NAT port mapping entry.
 *
 * This function sets custom unsigned long parameter values specific to a NAT port mapping
 * configuration.
 *
 * @param[in] hInsContext - The instance handle context for the port mapping entry.
 * @param[in] ParamName - The name of the custom unsigned long parameter to set.
 * @param[in] uValue - The unsigned long value to set for the parameter.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter was successfully set.
 * @retval FALSE if the parameter name is not recognized.
 */
BOOL
PortMapping_SetParamUlongValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

#endif