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


/**********************************************************************

    module: dslh_dmagnt_exported.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the framework's exported functions
        by the Dslh DataModelAgent object;

        *   CosaGetParamValueUlong2
        *   CosaGetParamValueString
        *   CosaGetParamValueBool
        *   CosaGetInstanceNumberByIndex

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        01/06/2011    initial revision.
        01/11/2011    added SLAP related apis.
        03/21/2011    added api to retrieve instance number by index

**********************************************************************/
#ifndef  _COSA_APIS_BUSUTIL_H
#define  _COSA_APIS_BUSUTIL_H

/**
 * @brief Get an unsigned long parameter value from data model.
 *
 * @param[in] pParamName - Pointer to the full name of the parameter.
 *
 * @return The unsigned long value of the parameter.
 * @retval ULONG parameter value on success.
 * @retval 0 If the operation fails or parameter is not found.
 *
 */
ULONG
CosaGetParamValueUlong
    (
        char*                       pParamName
    );

/**
 * @brief Get a string parameter value from data model.
 *
 * @param[in] pParamName  - Pointer to the full name of the parameter.
 * @param[out] pBuffer    - Pointer to a buffer where the string value will be stored.
 *                          \n The buffer should be allocated with sufficient size (at least 1024 bytes).
 * @param[in,out] pulSize - Pointer to the buffer size; updated with actual size on return.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval -1 if the operation fails.
 * @retval 1 if new size is returned.
 *
 */
int
CosaGetParamValueString
    (
        char*                       pParamName,
        char*                       pBuffer,
        PULONG                      pulSize
    );

/**
 * @brief Get a boolean parameter value from data model.
 *
 * @param[in] pParamName - Pointer to the full name of the parameter.
 *
 * @return The boolean value of the parameter.
 * @retval TRUE if the parameter value is true.
 * @retval FALSE if the parameter value is false or operation fails.
 *
 */
BOOL
CosaGetParamValueBool
    (
        char*                       pParamName
    );

/**
 * @brief Get the instance number by index for a table object.
 *
 * @param[in] pObjName - Pointer to the full object name.
 * @param[in] ulIndex  - The index specified.
 *
 * @return The instance number at the specified index.
 * @retval ULONG The instance number on success.
 *
 */
ULONG
CosaGetInstanceNumberByIndex
    (
        char*                      pObjName,
        ULONG                      ulIndex
    );

#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
/**
 * @brief Get interface instance from VLAN manager by interface name.
 *
 * @param[in] pIfName              - Pointer to the interface name to match.
 * @param[out] pMatchedLowerLayer  - Pointer to store the matched lower layer instance path.
 *                                   \n The buffer will be allocated and must be freed by caller.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if operation fails.
 *
 */
ANSC_STATUS PAM_GetInterfaceInstanceFromVlanmanager(char *pIfName, PUCHAR *pMatchedLowerLayer);

/**
 * @brief Get parameter value from any component dynamically.
 *
 * This function discovers which component supports the given data model parameter
 * and retrieves the value from that component.
 *
 * @param[in] pQuery - Pointer to the parameter query string.
 * @param[out] pValue - Pointer to a buffer where the parameter value will be stored.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if operation fails.
 *
 */
ANSC_STATUS RdkBus_GetParamValueFromAnyComp( char * pQuery, char *pValue);
#endif
#endif
