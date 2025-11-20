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

    File: cosa_x_cisco_com_multilan_dml.h

    For Data Model SBAPI Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        This file defines the DM Middle Layer functions servicing
        data model X_CISCO_COM_MultiLAN data model.

    ---------------------------------------------------------------

    environment:

        Platform Independent, but with multi-LAN support

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/28/2013  initial revision.

**********************************************************************/

#ifndef _COSA_X_CISCO_COM_MULTILAN_DML_
#define _COSA_X_CISCO_COM_MULTILAN_DML_

/**************************************************************************
                        DATA STRUCTURE DEFINITIONS
**************************************************************************/

/**************************************************************************
                        ROUTINE PROTOTYPES
**************************************************************************/

/**
 * @brief Get X_CISCO_COM_MultiLAN string parameter values.
 *
 * Retrieves string configuration values for MultiLAN interface mappings.
 *
 * @param[in] hInsContext Instance context handle.
 * @param[in] ParamName Name of the parameter to retrieve.
 * @param[out] pValue Pointer to a buffer where the parameter value will be returned.
 * @param[in,out] pUlSize Pointer to the buffer size. Usually size of 1023 will be used.
 *                        \n On input, specifies the allocated buffer size.
 *                        \n On output, contains the required size if buffer is insufficient.
 *
 * @return The status of the operation.
 * @retval 0 If the operation is successful.
 * @retval 1 If short of buffer size (*pUlSize = required size).
 * @retval -1 If parameter is not supported.
 */
ULONG
X_CISCO_COM_MultiLAN_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif

