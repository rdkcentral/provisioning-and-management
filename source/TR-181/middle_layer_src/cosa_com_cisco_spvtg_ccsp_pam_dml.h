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

    module: cosa_com_cisco_spvtg_ccsp_pam_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Ding Hua

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_COM_CISCO_SPVTG_CCSP_PAM_DML_H
#define  _COSA_COM_CISCO_SPVTG_CCSP_PAM_DML_H

/***********************************************************************

 APIs for Object:

    com.cisco.spvtg.ccsp.pam.

    *  Pam_GetParamBoolValue
    *  Pam_GetParamIntValue
    *  Pam_GetParamUlongValue
    *  Pam_GetParamStringValue

***********************************************************************/

/**
 * @brief Get boolean parameter value from PAM object.
 *
 * @param[in] hInsContext - The instance handle to the PAM object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pBool      - Pointer to store the boolean value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Pam_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief Get integer parameter value from PAM object.
 *
 * @param[in] hInsContext - The instance handle to the PAM object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pInt       - Pointer to store the integer value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Pam_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief Get unsigned long parameter value from PAM object.
 *
 * @param[in] hInsContext - The instance handle to the PAM object.
 * @param[in] ParamName   - Pointer to the parameter name.
 * @param[out] pUlong     - Pointer to store the unsigned long value.
 *
 * @return The status of the operation.
 * @retval TRUE if the parameter is found and retrieved successfully.
 * @retval FALSE if the parameter is not found or operation fails.
 *
 */
BOOL
Pam_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief Get string parameter value from PAM object.
 *
 * @param[in] hInsContext  - The instance handle to the PAM object.
 * @param[in] ParamName    - Pointer to the parameter name.
 * @param[out] pValue      - Pointer to a buffer where the string value will be stored.
 * @param[in,out] pUlSize  - Pointer to the buffer size,usually size 1023 is used; updated with actual size on return. Usually size of 1023 will be used.
 *
 * @return The status of the operation.
 * @retval 0 if the operation is successful.
 * @retval 1 if the buffer is too small; pUlSize updated with required size.
 * @retval -1 if not supported.
 *
 */
ULONG
Pam_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
