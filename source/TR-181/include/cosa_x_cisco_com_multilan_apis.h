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

    File: cosa_x_cisco_com_multilan_apis.h

    For Data Model SBAPI Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        This file defines the SBAPI functions servicing
        data model X_CISCO_COM_MultiLAN integration.

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

#ifndef _COSA_X_CISCO_COM_MULTILAN_APIS_
#define _COSA_X_CISCO_COM_MULTILAN_APIS_

/**************************************************************************
                        DATA STRUCTURE DEFINITIONS
**************************************************************************/

/**************************************************************************
                        ROUTINE PROTOTYPES
**************************************************************************/

/**
* @brief Initialize the Multi-LAN subsystem.
*
* This function initializes the Multi-LAN module by allocating memory for the
* COSA_DATAMODEL_MULTILAN context structure and initializing it via COSA_DATAMODEL_MULTILAN_Init().
*
* @param[in] hDml  - Handle to the DML. This parameter is currently unused.
* @param[out] phContext  - Pointer to store the allocated COSA_DATAMODEL_MULTILAN context handle.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if initialization and memory allocation are successful.
* @retval ANSC_STATUS_RESOURCES if memory allocation fails.
*
*/
ANSC_STATUS
CosaDmlMlanInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Unload and clean up the Multi-LAN subsystem.
*
* This function cleans up the Multi-LAN module by calling COSA_DATAMODEL_MULTILAN_Clean()
* on the context structure. Memory cleanup is performed for allocated resources.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context to be cleaned up.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if cleanup is successful.
*
*/
ANSC_STATUS
CosaDmlMlanUnload
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get the Primary LAN IP interface path.
*
* This function retrieves the TR-181 path for the Primary LAN IP interface from PSM .
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the IP interface path will be returned.
*                    \n Format: "Device.IP.Interface.{i}" where {i} is the instance number.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetPrimaryLanIpIf
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Home Security IP interface path.
*
* This function retrieves the TR-181 path for the Home Security LAN IP interface from PSM.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the IP interface path will be returned.
*                    \n Format: "Device.IP.Interface.{i}" where {i} is the instance number.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetHomeSecurityIpIf
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Primary LAN Bridge path.
*
* This function retrieves the TR-181 path for the Primary LAN Bridge from PSM.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the bridge path will be returned.
*                    \n Format: "Device.Bridging.Bridge.{i}" where {i} is the instance number.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetPrimaryLanBridge
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Primary LAN Bridge Hot Spot ports.
*
* This function retrieves the list of Hot Spot ports for the Primary LAN Bridge from PSM.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the Hot Spot ports string will be returned.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetPrimaryLanBridgeHsPorts
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Primary LAN DHCPv4 server pool path.
*
* This function retrieves the TR-181 path for the Primary LAN DHCPv4 server pool from PSM.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the DHCPv4 server pool path will be returned.
*                    \n Format: "Device.DHCPv4.Server.Pool.{i}" where {i} is the instance number.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetPrimaryLanDhcpv4ServerPool
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Home Security Bridge path.
*
* This function retrieves the TR-181 path for the Home Security Bridge from PSM.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the bridge path will be returned.
*                    \n Format: "Device.Bridging.Bridge.{i}" where {i} is the instance number.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetHomeSecurityBridge
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Home Security Bridge Hot Spot ports.
*
* This function retrieves the list of Hot Spot ports for the Home Security Bridge from PSM.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the Hot Spot ports string will be returned.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetHomeSecurityBridgePorts
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Home Security DHCPv4 server pool path.
*
* This function retrieves the TR-181 path for the Home Security DHCPv4 server pool from PSM.
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the DHCPv4 server pool path will be returned.
*                    \n Format: "Device.DHCPv4.Server.Pool.{i}" where {i} is the instance number.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetHomeSecurityDhcpv4ServerPool
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Home Security WiFi Radio path.
*
* This function retrieves the TR-181 path for the Home Security WiFi Radio from PSM.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the WiFi Radio path will be returned.
*                    \n Format: "Device.WiFi.Radio.{i}" where {i} is the instance number.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetHomeSecurityWiFiRadio
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Home Security WiFi SSID path.
*
* This function retrieves the TR-181 path for the Home Security WiFi SSID from PSM.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the WiFi SSID path will be returned.
*                    \n Format: "Device.WiFi.SSID.{i}" where {i} is the instance number.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetHomeSecurityWiFiSsid
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

/**
* @brief Get the Home Security WiFi Access Point path.
*
* This function retrieves the TR-181 path for the Home Security WiFi Access Point from PSM.
*
* @param[in] hContext  - Handle to the COSA_DATAMODEL_MULTILAN context.
* @param[out] pValue  - Pointer to buffer where the WiFi Access Point path will be returned.
*                    \n Format: "Device.WiFi.AccessPoint.{i}" where {i} is the instance number.
* @param[in,out] pValueSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies buffer size. On output, contains required size if buffer is insufficient.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_MORE_DATA if buffer size is insufficient (required size returned in pValueSize).
* @retval ANSC_STATUS_RESOURCES if PSM value retrieval fails.
*
*/
ANSC_STATUS
CosaDmlMlanGetHomeSecurityWiFiAp
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

#endif
