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

    module: cosa_ip_internal.h

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


#ifndef  _COSA_IP_INTERNAL_H
#define  _COSA_IP_INTERNAL_H

#include "cosa_apis.h"

#define  COSA_IREP_FOLDER_NAME_IPIF                 "IPInterface"
#define  COSA_IREP_FOLDER_NAME_IPIF_HA              "IPInterfaceHalfAdded"

#define  COSA_DML_RR_NAME_IPIF_Prefix               "IPInterface"
#define  COSA_DML_RR_NAME_IPIF_IPV4_Prefix          "IPV4Address"
#define  COSA_DML_RR_NAME_IPIF_IPV6_Prefix          "IPV6Address"
#define  COSA_DML_RR_NAME_IPIF_IPV6Pre_Prefix       "IPV6Prefix"

#define  COSA_DML_RR_NAME_IPIF_Upper_InsNum         "UpperInstanceNumber"
#define  COSA_DML_RR_NAME_IPIF_InsNum               "InstanceNumber"
#define  COSA_DML_RR_NAME_IPIF_Alias                "Alias"

#define  COSA_DML_RR_NAME_IPIF_NextInsNum           "NextInstanceNumber"
#define  COSA_DML_RR_NAME_IPIF_IPv4_NextInsNum      "NextIPv4InstanceNum"
#define  COSA_DML_RR_NAME_IPIF_IPv6_NextInsNum      "NextIPv6InstanceNum"
#define  COSA_DML_RR_NAME_IPIF_IPv6Pre_NextInsNum   "NextIPv6PreInstanceNum"




/***********************************
    Actual definition declaration
************************************/

/*
    Object definition
*/

#define  COSA_DATAMODEL_IP_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    /* start of IP object class content */                                                 \
    ULONG                           activePortCount;                                        \
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort;                                            \
    SLIST_HEADER                    InterfaceList;                                          \
    ULONG                           ulInterfaceCount;                                     \
    ULONG                           ulNextInterfaceInsNum;                                \
    ANSC_HANDLE                     hIrepFolderCOSA;                                       \
    ANSC_HANDLE                     hIrepFolderIPInterface;                                 \
    ANSC_HANDLE                     hIrepFolderIPInterfaceHA;                                 \

typedef  struct
_COSA_DATAMODEL_IP
{
    COSA_DATAMODEL_IP_CLASS_CONTENT
}
COSA_DATAMODEL_IP,  *PCOSA_DATAMODEL_IP;

/**********************************
    Standard function declaration
***********************************/
/**
 * @brief Create the IP backend object.
 *
 * @return Handle to IP backend object
 * @retval Handle to the newly created IP backend object
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaIPCreate
    (
        VOID
    );

/**
 * @brief Initializes the IP backend object.
 *
 * @param[in] hThisObject Handle to the IP backend object.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful initialization
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaIPInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the IP backend object.
 *
 * @param[in] hThisObject Handle to the IP backend object to be removed.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful removal
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaIPRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Loads IP interface configuration from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the IP backend object.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful configuration load
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaIpRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Adds an IP interface entry to persistent registry storage.
 *
 * @param[in] hThisObject Handle to the IP backend object.
 * @param[in] pNextInsNumName Name of the next instance number parameter.
 * @param[in] ulNextInsNum Next instance number to use for new entries.
 * @param[in] pPreffix Registry key prefix for the entry type.
 * @param[in] ulUpperInsNum Upper-level instance number for hierarchical objects.
 * @param[in] pAlias User-assigned alias name for the entry.
 * @param[in] hCosaContext Handle to the COSA context object for the entry.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful entry addition
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaIPRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pNextInsNumName,
        ULONG                       ulNextInsNum,
        char*                       pPreffix,
        ULONG                       ulUpperInsNum,
        char*                       pAlias,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Deletes an IP interface entry from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the IP backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the entry to delete.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful entry deletion
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaIPRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Generates an instance alias for an IPv6 prefix entry.
 *
 * @param[in] p_ipif Pointer to the full IP interface structure containing the prefix.
 * @param[in,out] pEntry Pointer to the IPv6 prefix entry to generate an alias for.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful alias generation
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaIPv6PrefGenInstAlias
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

/**
 * @brief Generates an instance alias for an IPv6 address entry.
 *
 * @param[in] p_ipif Pointer to the full IP interface structure containing the address.
 * @param[in,out] pEntry Pointer to the IPv6 address entry to generate an alias for.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful alias generation
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaIPv6AddrGenInstAlias
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

#endif
