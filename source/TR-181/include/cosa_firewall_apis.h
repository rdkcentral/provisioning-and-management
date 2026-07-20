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

    module: cosa_firewall_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

/*
 * This file defines data types and methods as the middle layer between COSA DM library
 * and backend implementation.
 */

#ifndef  _COSA_FIREWALL_APIS_H
#define  _COSA_FIREWALL_APIS_H

#include "ansc_platform.h"
#include "cosa_apis.h"
#include "plugin_main_apis.h"


typedef  enum
_COSA_DML_FIREWALL_LEVEL
{
    COSA_DML_FIREWALL_LEVEL_High    = 1,
    COSA_DML_FIREWALL_LEVEL_Medium,         /* Not applicable for phase 4 */
    COSA_DML_FIREWALL_LEVEL_Low,
    COSA_DML_FIREWALL_LEVEL_Custom,
    COSA_DML_FIREWALL_LEVEL_None
}
COSA_DML_FIREWALL_LEVEL, *PCOSA_DML_FIREWALL_LEVEL;


typedef  struct
_COSA_DML_FIREWALL_CFG
{
    COSA_DML_FIREWALL_LEVEL         FirewallLevel;
    char                            Version[8];
    char                            LastChange[32];
}
COSA_DML_FIREWALL_CFG,  *PCOSA_DML_FIREWALL_CFG;


/*
 *  The actual function declaration
 */

/**
* @brief Initialize the firewall subsystem.
*
* This function initializes the firewall data model layer and prepares the firewall
* subsystem for operation.
*
* @param[in] hDml - Handle to the DML (Data Model Library) object.
* @param[out] phContext - Pointer to a handle where the firewall context will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlFirewallInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Set the firewall configuration.
*
* This function updates the firewall configuration including the security level.
*
* @param[in] hContext - Handle to the firewall context.
* @param[in] pCfg - Pointer to a COSA_DML_FIREWALL_CFG structure containing the new configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlFirewallSetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG      pCfg
    );

/**
* @brief Get the firewall configuration.
*
* This function retrieves the current firewall configuration including the security level,
* version, and last change timestamp.
*
* @param[in] hContext - Handle to the firewall context.
* @param[out] pCfg - Pointer to a COSA_DML_FIREWALL_CFG structure where the configuration
*                    will be returned.
*                    \n The buffer must be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlFirewallGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG      pCfg
    );

/**
* @brief Get the IPv4 block fragmented IP packets setting.
*
* This function retrieves the current setting for blocking fragmented IPv4 packets
* from the system configuration.
*
* @param[out] pBool - Pointer to a BOOL where the setting will be returned.
*                     \n TRUE if fragmented IPv4 packets are blocked.
*                     \n FALSE if fragmented IPv4 packets are allowed.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV4GetBlockFragIPPkts
    (
        BOOL *pBool
    );

/**
* @brief Get the IPv4 port scan protection setting.
*
* This function retrieves the current setting for IPv4 port scan protection
* from the system configuration.
*
* @param[out] pBool - Pointer to a BOOL where the setting will be returned.
*                     \n TRUE if IPv4 port scan protection is enabled.
*                     \n FALSE if IPv4 port scan protection is disabled.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV4GetPortScanProtect
    (
        BOOL *pBool
    );

/**
* @brief Get the IPv4 IP flood detection setting.
*
* This function retrieves the current setting for IPv4 IP flood detection
* from the system configuration.
*
* @param[out] pBool - Pointer to a BOOL where the setting will be returned.
*                     \n TRUE if IPv4 IP flood detection is enabled.
*                     \n FALSE if IPv4 IP flood detection is disabled.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV4GetIPFloodDetect
    (
        BOOL *pBool
    );

/**
* @brief Set the IPv4 block fragmented IP packets setting.
*
* This function updates the setting for blocking fragmented IPv4 packets
* in the system configuration.
*
* @param[in] bValue - BOOL value to set.
*                     \n TRUE to block fragmented IPv4 packets.
*                     \n FALSE to allow fragmented IPv4 packets.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV4SetBlockFragIPPkts
    (
        BOOL bValue
    );

/**
* @brief Set the IPv4 port scan protection setting.
*
* This function updates the setting for IPv4 port scan protection
* in the system configuration.
*
* @param[in] bValue - BOOL value to set.
*                     \n TRUE to enable IPv4 port scan protection.
*                     \n FALSE to disable IPv4 port scan protection.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV4SetPortScanProtect
    (
        BOOL bValue
    );

/**
* @brief Set the IPv4 IP flood detection setting.
*
* This function updates the setting for IPv4 IP flood detection
* in the system configuration.
*
* @param[in] bValue - BOOL value to set.
*                     \n TRUE to enable IPv4 IP flood detection.
*                     \n FALSE to disable IPv4 IP flood detection.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV4SetIPFloodDetect
    (
        BOOL bValue
    );

/**
* @brief Get the IPv6 block fragmented IP packets setting.
*
* This function retrieves the current setting for blocking fragmented IPv6 packets
* from the system configuration.
*
* @param[out] pBool - Pointer to a BOOL where the setting will be returned.
*                     \n TRUE if fragmented IPv6 packets are blocked.
*                     \n FALSE if fragmented IPv6 packets are allowed.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV6GetBlockFragIPPkts
    (
        BOOL *pBool
    );

/**
* @brief Get the IPv6 port scan protection setting.
*
* This function retrieves the current setting for IPv6 port scan protection
* from the system configuration.
*
* @param[out] pBool - Pointer to a BOOL where the setting will be returned.
*                     \n TRUE if IPv6 port scan protection is enabled.
*                     \n FALSE if IPv6 port scan protection is disabled.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV6GetPortScanProtect
    (
        BOOL *pBool
    );

/**
* @brief Get the IPv6 IP flood detection setting.
*
* This function retrieves the current setting for IPv6 IP flood detection
* from the system configuration.
*
* @param[out] pBool - Pointer to a BOOL where the setting will be returned.
*                     \n TRUE if IPv6 IP flood detection is enabled.
*                     \n FALSE if IPv6 IP flood detection is disabled.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV6GetIPFloodDetect
    (
        BOOL *pBool
    );

/**
* @brief Set the IPv6 block fragmented IP packets setting.
*
* This function updates the setting for blocking fragmented IPv6 packets
* in the system configuration.
*
* @param[in] bValue - BOOL value to set.
*                     \n TRUE to block fragmented IPv6 packets.
*                     \n FALSE to allow fragmented IPv6 packets.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV6SetBlockFragIPPkts
    (
        BOOL bValue
    );

/**
* @brief Set the IPv6 port scan protection setting.
*
* This function updates the setting for IPv6 port scan protection
* in the system configuration.
*
* @param[in] bValue - BOOL value to set.
*                     \n TRUE to enable IPv6 port scan protection.
*                     \n FALSE to disable IPv6 port scan protection.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV6SetPortScanProtect
    (
        BOOL bValue
    );

/**
* @brief Set the IPv6 IP flood detection setting.
*
* This function updates the setting for IPv6 IP flood detection
* in the system configuration.
*
* @param[in] bValue - BOOL value to set.
*                     \n TRUE to enable IPv6 IP flood detection.
*                     \n FALSE to disable IPv6 IP flood detection.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGatewayV6SetIPFloodDetect
    (
        BOOL bValue
    );
#endif
