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

    module: cosa_x_cisco_com_mld_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_MLD_APIS_H
#define  _COSA_MLD_APIS_H

#include "cosa_apis.h"


/***********************************
    Actual definition declaration
************************************/

/*
    Nat Part
*/
typedef  enum
_COSA_DML_MLD_STATUS
{
    COSA_DML_MLD_STATUS_Disabled                = 1,
    COSA_DML_MLD_STATUS_Enabled,
    COSA_DML_MLD_STATUS_Error
}
COSA_DML_MLD_STATUS, *PCOSA_DML_MLD_STATUS;

typedef  enum
_COSA_DML_MLD_MULTICAST_MODE
{
    COSA_DML_MLD_MULTICAST_MODE_ASM                = 3, /* According to MLD Proxy */
    COSA_DML_MLD_MULTICAST_MODE_SSM,
    COSA_DML_MLD_MULTICAST_MODE_Both
}
COSA_DML_MLD_MULTICAST_MODE, *PCOSA_DML_MLD_MULTICAST_MODE;

typedef  enum
_COSA_DML_MLD_FORWARD_SCOPE
{
    COSA_DML_MLD_FORWARD_SCOPE_Link                 = 10, /* According to MLD Proxy */
    COSA_DML_MLD_FORWARD_SCOPE_Site,
    COSA_DML_MLD_FORWARD_SCOPE_Global
}
COSA_DML_MLD_FORWARD_SCOPE, *PCOSA_DML_MLD_FORWARD_SCOPE;

typedef  struct
_COSA_DML_MLD_CFG
{
    BOOLEAN                         bEnabled;
    BOOLEAN                         bSnoopingEnabled;
}
COSA_DML_MLD_CFG,  *PCOSA_DML_MLD_CFG;

typedef  struct
_COSA_DML_MLD_INFO
{
    COSA_DML_MLD_STATUS             Status;
}
COSA_DML_MLD_INFO,  *PCOSA_DML_MLD_INFO;


typedef  struct
_COSA_DML_MLD
{
    COSA_DML_MLD_CFG                Cfg;
    COSA_DML_MLD_INFO               Info;
}
COSA_DML_MLD,  *PCOSA_DML_MLD;

typedef  struct
_COSA_DML_MLD_GROUP
{
    char                            GroupAddress[256];
    char                            Interfaces[512];
}
COSA_DML_MLD_GROUP,  *PCOSA_DML_MLD_GROUP;

/*************************************
    The actual function declaration
**************************************/

/**
* @brief Initialize the MLD (Multicast Listener Discovery) subsystem.
*
* This function initializes the MLD module.
*
* @param[in] hDml  - Handle to the DML.
* @param[out] phContext  - Pointer to handle for storing context.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlMldInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Get the MLD configuration.
*
* This function retrieves the MLD configuration including enabled and snooping status.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pCfg  - Pointer to a COSA_DML_MLD_CFG structure where configuration will be returned.
*                    \n Contains bEnabled (TRUE if ecmh daemon is running) and bSnoopingEnabled
*                    (TRUE if multicast snooping is enabled in bridge).
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if pCfg is NULL.
*
*/
ANSC_STATUS
CosaDmlMldGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    );

/**
* @brief Set the MLD configuration.
*
* This function sets the MLD configuration including enabled and snooping status.
*
* @param[in] hContext  - Handle to the context.
* @param[in] pCfg  - Pointer to a COSA_DML_MLD_CFG structure containing the configuration to set.
*                    \n bSnoopingEnabled controls multicast snooping in bridge.
*                    \n bEnabled controls ecmh daemon execution.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if pCfg is NULL.
*
*/
ANSC_STATUS
CosaDmlMldSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    );

/**
* @brief Get the MLD runtime information.
*
* This function retrieves the current MLD operational status by checking if the ecmh daemon is running.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pCfg  - Pointer to a COSA_DML_MLD_INFO structure where status will be returned.
*                    \n Status will be COSA_DML_MLD_STATUS_Enabled if ecmh is running,
*                    or COSA_DML_MLD_STATUS_Disabled if not running.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if pCfg is NULL.
*
*/
ANSC_STATUS
CosaDmlMldGetInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_INFO          pCfg
    );

/**
* @brief Get MLD multicast group information.
*
* This function retrieves the list of active MLD multicast groups by sending SIGUSR1 to the ecmh
* daemon.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pulCount  - Pointer to store the number of MLD group entries returned.
* @param[out] pMldGroupArray  - Pointer to an array of COSA_DML_MLD_GROUP structures.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if pulCount or pMldGroupArray is NULL, or ecmh daemon is not running, or dump file cannot be read.
*
*/
ANSC_STATUS
CosaDmlMldGetGroup
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_MLD_GROUP         pMldGroupArray
    );

#endif
