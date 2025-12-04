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

    module: cosa_ipv6rd_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        leichen2@cisco.com

    -------------------------------------------------------------------

    revision:

        05/08/2011    initial revision.
        02/09/2011    Define APIs.

**************************************************************************/

#ifndef _COSA_IPV6RD_INTERNEL_H_
#define _COSA_IPV6RD_INTERNEL_H_

// max interface instances (is it enough?)
#define MAX_6RDIF_INS              64

#define IPV6RD_DEF_STRLEN          512

typedef struct _COSA_DML_IPV6RD_IF
{
    /* configrable params */
    ULONG               InstanceNumber;     /* start counting from 1 */
    BOOL                Enable;
    char                Alias[64 + 1];
    char                BorderRelayIPv4Addr[IPV6RD_DEF_STRLEN + 1];
    BOOL                AllTrafficToBorderRelay;
    char                SPIPv6Prefix[IPV6RD_DEF_STRLEN + 1];
    ULONG               IPv4MaskLength;
    char                AddressSource[256 + 1];

    /* readonly (runtime stauts) params */
    char                Status[IPV6RD_DEF_STRLEN + 1];
    char                TunnelInterface[256 + 1];
    char                TunneledInterface[256 + 1];
} COSA_DML_IPV6RD_IF, *PCOSA_DML_IPV6RD_IF;

/**
* @brief Initialize the IPv6 Rapid Deployment (6RD) subsystem.
*
* This function initializes the 6RD data model library and loads configuration from persistent storage (syscfg).
*
* @param[in] hDml Handle to the DML object.
* @param[out] phContext Pointer to receive the context handle.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_IPv6rdInit(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext
        );

/**
* @brief Finalize the IPv6 Rapid Deployment (6RD) subsystem.
*
* This function performs cleanup operations for the 6RD data model library,
* releasing resources and saving configuration if necessary.
*
* @param[in] hDml Handle to the DML object.
* @param[in,out] phContext Pointer to the context handle.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_IPv6rdFinalize(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext
        );

/**
* @brief Get the global enable state of IPv6 Rapid Deployment.
*
* This function retrieves the global enable/disable state of the 6RD subsystem.
*
* @param[in] hDml Handle to the DML object.
* @param[in,out] phContext Pointer to the context handle.
* @param[out] bEnable Pointer to receive the enable state.
                      TRUE for enabled state, FALSE for disabled state.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_IPv6rdGetEnable(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext,
        BOOL            *bEnable
        );

/**
* @brief Set the global enable state of IPv6 Rapid Deployment.
*
* This function enables or disables the 6RD subsystem globally.
*
* @param[in] hDml Handle to the DML object.
* @param[in,out] phContext Pointer to the context handle.
* @param[in] bEnable Enable state to set.
*                    TRUE for enabled state, FALSE for disabled state.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_IPv6rdSetEnable(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext,
        BOOL            bEnable
        );

/**
* @brief Get the number of 6RD interface entries.
*
* This function retrieves the total number of configured 6RD interface entries and their instance numbers.
*
* @param[in] hContext DML context handle.
* @param[in,out] NumEntries Pointer to the number of entries.
*                           \n As input: size of InsNumArray buffer.
*                           \n As output: actual number of 6RD interface entries.
* @param[out] InsNumArray Array to receive the instance numbers.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_IPv6rdGetNumberOfEntries(
        ANSC_HANDLE     hContext,
        ULONG           *NumEntries, /* input-output */
        ULONG           InsNumArray[]
        );

/**
* @brief Get a 6RD interface entry by instance number.
*
* This function retrieves the configuration and runtime status of a specific
* 6RD interface identified by its instance number.
*
* @param[in] hContext DML context handle.
* @param[in] ulInstanceNumber Instance number of the 6RD interface to retrieve.
* @param[out] pEntry Pointer to receive the 6RD interface configuration and status.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_IPv6rdGetEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber,
        PCOSA_DML_IPV6RD_IF     pEntry
        );

/**
* @brief Set (update) a 6RD interface entry configuration.
*
* This function applies new configuration parameters to an existing 6RD interface.
*
* @param[in] hContext DML context handle.
* @param[in] ulInstanceNumber Instance number of the 6RD interface to update.
* @param[in] pEntry Pointer to the new 6RD interface configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_IPv6rdSetEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber,
        PCOSA_DML_IPV6RD_IF     pEntry
        );


/**
* @brief Add a new 6RD interface entry.
*
* This function creates a new 6RD interface configuration entry.
*
* @param[in] hContext DML context handle.
* @param[in,out] pEntry Pointer to the 6RD interface configuration to add.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_IPv6rdAddEntry(
        ANSC_HANDLE             hContext,
        PCOSA_DML_IPV6RD_IF     pEntry
        );

/**
* @brief Delete a 6RD interface entry.
*
* This function removes a 6RD interface configuration.
*
* @param[in] hContext DML context handle.
* @param[in] ulInstanceNumber Instance number of the 6RD interface to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_IPv6rdDelEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber
        );

/**
* @brief Convert interface name to IPv4 address.
*
* This function retrieves the IPv4 address assigned to a network interface
* specified by its interface name.
*
* @param[in] ifname Network interface name.
* @param[out] addr Buffer to receive the IPv4 address string.
* @param[in] size Size of the addr buffer in bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*/
ANSC_STATUS
CosaDml_Ifname2Addr(
		const char *ifname,
		char *addr,
		unsigned int size
		);

#endif /* _COSA_IPV6RD_INTERNEL_H_ */
