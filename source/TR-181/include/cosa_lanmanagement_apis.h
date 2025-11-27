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

/**************************************************************************

    module: cosa_lanmanagement_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlLanManagementInit
        *  CosaDmlLanManagementSetCfg
        *  CosaDmlLanManagementGetCfg
        *  CosaDmlLanMngm_SetLanIpv6Enable
        *  CosaDmlLanMngm_SetLanIpv6UlaEnable
        *  CosaDmlLanMngm_SetLanIpv6Ula

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:


    -------------------------------------------------------------------

    revision:

        05/9/2019    initial revision.

**************************************************************************/

#ifndef  _COSA_LANMANAGEMENT_API_H
#define  _COSA_LANMANAGEMENT_API_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_common_util.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

typedef  struct
_COSA_DML_LANMANAGEMENT_CFG
{
    BOOLEAN                         LanIpv6Enable;
	BOOLEAN                         LanIpv6UlaEnable;
    char                            LanIpv6Ula[65]; /* CID 163210 fix */
    char                            LanIpv6UlaPrefix[64];
}
COSA_DML_LANMANAGEMENT_CFG,  *PCOSA_DML_LANMANAGEMENT_CFG;

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the LAN Management subsystem.
*
* This function initializes the LAN Management data model library and loads IPv6 configuration parameters from persistent storage.
*
* @param[in] hDml Handle to the DML object.
* @param[out] phContext Pointer to receive the context handle.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDmlLanManagementInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Set LAN Management configuration.
*
* This function applies new LAN Management configuration parameters.
*
* @param[in] hContext DML context handle.
* @param[in] pTimeCfg Pointer to the LAN Management configuration structure to apply.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDmlLanManagementSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_LANMANAGEMENT_CFG          pTimeCfg
    );

/**
* @brief Get LAN Management configuration.
*
* This function retrieves the current LAN Management configuration.
*
* @param[in] hContext DML context handle.
* @param[out] pTimeCfg Pointer to receive the current LAN Management configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDmlLanManagementGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_LANMANAGEMENT_CFG          pTimeCfg
    );

#if defined(_HUB4_PRODUCT_REQ_) || defined(_RDKB_GLOBAL_PRODUCT_REQ_)
/**
* @brief Set LAN IPv6 ULA (Unique Local Address).
*
* This function configures the IPv6 Unique Local Address for the LAN interface.
*
* @param[in] ula_prefix IPv6 ULA prefix string.
* @param[in] ula Complete IPv6 ULA address string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS CosaDmlLanMngm_SetLanIpv6Ula(char *ula_prefix, char *ula);
#endif

/**
* @brief Get the device serial number.
*
* This function retrieves the device serial number from the platform HAL.
* The serial number is typically used for device identification and tracking.
*
* @param[out] pValue Pointer to a buffer where the serial number string will be returned.
*
* @return The status of the operation.
* @retval RETURN_OK if the operation is successful.
* @retval RETURN_ERR if any error is detected during the operation.
*/
INT platform_hal_GetSerialNumber(CHAR* pValue);
#endif
