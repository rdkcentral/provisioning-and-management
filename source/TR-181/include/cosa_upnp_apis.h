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

    module: cosa_upnp_apis.h

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


#ifndef  _COSA_UPNP_API_H
#define  _COSA_UPNP_API_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the UPnP module.
*
* This function initializes the UPnP (Universal Plug and Play) module for the device.
*
* @param[in] hDml  - Handle to the DML object.
* @param[in,out] phContext  - Pointer to the context handle.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlUpnpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Enable or disable the UPnP device functionality.
*
* This function enables or disables the UPnP device functionality.
*
* @param[in] hContext  - Handle to the context.
* @param[in] bEnabled  - Boolean flag to enable (TRUE) or disable (FALSE) UPnP device functionality.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia context initialization fails.
*
*/
ANSC_STATUS
CosaDmlUpnpDevEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

/**
* @brief Get the current state of the UPnP device functionality.
*
* This function retrieves the current enable/disable state of the UPnP device functionality
* from the system configuration.
*
* @param[in] hContext  - Handle to the context.
*
* @return The current state of UPnP device functionality.
* @retval TRUE if UPnP device functionality is enabled.
* @retval FALSE if UPnP device functionality is disabled.
*
*/
BOOLEAN
CosaDmlUpnpDevGetState
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Enable or disable the UPnP Media Server functionality.
*
* This function enables or disables the UPnP Media Server functionality on the device.
*
* @param[in] hContext  - Handle to the context.
* @param[in] bEnabled  - Boolean flag to enable (TRUE) or disable (FALSE) UPnP Media Server functionality.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlUpnpDevEnableMediaServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

/**
* @brief Get the current state of the UPnP Media Server functionality.
*
* This function retrieves the current enable/disable state of the UPnP Media Server functionality.
*
* @param[in] hContext  - Handle to the context.
*
* @return The current state of UPnP Media Server functionality.
* @retval TRUE if UPnP Media Server functionality is enabled.
* @retval FALSE if UPnP Media Server functionality is disabled.
*
*/
BOOLEAN
CosaDmlUpnpDevGetMediaServerState
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Enable or disable the UPnP IGD (Internet Gateway Device) functionality.
*
* This function enables or disables the UPnP IGD functionality on the device.
*
* @param[in] hContext  - Handle to the context.
* @param[in] bEnabled  - Boolean flag to enable (TRUE) or disable (FALSE) UPnP IGD functionality.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia context initialization fails.
*
*/
ANSC_STATUS
CosaDmlUpnpDevEnableIgd
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

/**
* @brief Get the current state of the UPnP IGD (Internet Gateway Device) functionality.
*
* This function retrieves the current enable/disable state of the UPnP IGD functionality
* from the system configuration.
*
* @param[in] hContext  - Handle to the context.
*
* @return The current state of UPnP IGD functionality.
* @retval TRUE if UPnP IGD functionality is enabled.
* @retval FALSE if UPnP IGD functionality is disabled.
*
*/
BOOLEAN
CosaDmlUpnpDevGetIgdState
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get the UPnP device architecture version.
*
* This function retrieves the UPnP device architecture version information.
*
* @param[in] hContext  - Handle to the context. This parameter is currently unused.
* @param[out] pMajorVer  - Pointer to store the major version number.
* @param[out] pMinorVer  - Pointer to store the minor version number.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlUpnpDevGetArchVer
    (
        ANSC_HANDLE                 hContext,
        ULONG*                      pMajorVer,
        ULONG*                      pMinorVer
    );

/**
* @brief Enable or disable UPnP discovery functionality.
*
* This function enables or disables the UPnP discovery functionality on the device.
*
* @param[in] hContext  - Handle to the context.
* @param[in] bEnabled  - Boolean flag to enable (TRUE) or disable (FALSE) UPnP discovery functionality.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlUpnpSetDiscoveryEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

/**
* @brief Get the current state of UPnP discovery functionality.
*
* This function retrieves the current enable/disable state of the UPnP discovery functionality.
*
* @param[in] hContext  - Handle to the context.
*
* @return The current state of UPnP discovery functionality.
* @retval TRUE if UPnP discovery functionality is enabled.
* @retval FALSE if UPnP discovery functionality is disabled.
*
*/
BOOLEAN
CosaDmlUpnpGetDiscoveryEnable
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Set the UPnP discovery polling interval.
*
* This function sets the polling interval for UPnP discovery functionality.
*
* @param[in] hContext  - Handle to the context.
* @param[in] PollingInterval  - The polling interval value in seconds.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlUpnpSetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext,
        ULONG                       PollingInterval
    );

/**
* @brief Get the UPnP discovery polling interval.
*
* This function retrieves the current polling interval for UPnP discovery functionality.
*
* @param[in] hContext  - Handle to the context.
*
* @return The polling interval value in seconds.
*
*/
ULONG
CosaDmlUpnpGetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get the UPnP advertisement period.
*
* This function retrieves the UPnP advertisement period value from the system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[out] val  - Pointer to store the advertisement period value in seconds.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia context initialization fails.
*
*/
ANSC_STATUS
CosaDmlUpnpDevGetAdvPeriod
    (
        ANSC_HANDLE                 hContext,
        PULONG                     *val
    );

/**
* @brief Get the UPnP advertisement Time To Live (TTL) value.
*
* This function retrieves the UPnP advertisement TTL value from the system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[out] val  - Pointer to store the TTL value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia context initialization fails.
*
*/
ANSC_STATUS
CosaDmlUpnpDevGetTTL
    (
        ANSC_HANDLE                 hContext,
        PULONG                     *val
    );

/**
* @brief Set the UPnP advertisement Time To Live (TTL) value.
*
* This function sets the UPnP advertisement TTL value in the system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] val  - The TTL value to be set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia context initialization fails.
*
*/
ANSC_STATUS
CosaDmlUpnpDevSetTTL
    (
        ANSC_HANDLE                 hContext,
        ULONG                       val
    );

/**
* @brief Set the UPnP advertisement period.
*
* This function sets the UPnP advertisement period value in the system configuration.
*
* @param[in] hContext  - Handle to the context.
* @param[in] val  - The advertisement period value in seconds to be set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia context initialization fails.
*
*/
ANSC_STATUS
CosaDmlUpnpDevSetAdvPeriod
    (
        ANSC_HANDLE                 hContext,
        ULONG                       val
    );
#endif

