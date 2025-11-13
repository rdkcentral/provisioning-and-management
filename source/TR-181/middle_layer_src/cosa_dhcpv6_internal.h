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

    module: cosa_dhcpv4_internal.h

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


#ifndef  _COSA_DHCPV6_INTERNAL_H
#define  _COSA_DHCPV6_INTERNAL_H

#include "cosa_apis.h"
#include "cosa_dhcpv6_dml.h"

/*
  *   This is cosa middle layer definition
  *
  */
#define  COSA_IREP_FOLDER_NAME_DHCPV6                      "Dhcpv6"
#ifndef FEATURE_RDKB_WAN_MANAGER
#define  COSA_IREP_FOLDER_NAME_DHCPV6_CLIENT               "Client"
#define  COSA_IREP_FOLDER_NAME_DHCPV6_SENTOPTION           "SentOption"
#endif
#define  COSA_IREP_FOLDER_NAME_DHCPV6_POOL                 "Pool"
#define  COSA_IREP_FOLDER_NAME_DHCPV6_OPTION               "Option"
#define  COSA_DML_RR_NAME_Dhcpv6Alias                      "Alias"
#define  COSA_DML_RR_NAME_Dhcpv6bNew                       "bNew"
#define  COSA_DML_RR_NAME_Dhcpv6NextInsNunmber             "NextInstanceNumber"

#define  COSA_DML_DHCPV6_ALIAS                             64

#define  COSA_NAT_ROLLBACK_TEST                            0       /* This is just for test purpose */

#define   COSA_DML_DHCPV6_ACCESS_INTERVAL_CLIENTSERVER   60 /* seconds*/
#define   COSA_DML_DHCPV6_ACCESS_INTERVAL_CLIENTRECV     60 /* seconds*/
#define   COSA_DML_DHCPV6_ACCESS_INTERVAL_POOLCLIENT     10 /* seconds*/


#ifndef FEATURE_RDKB_WAN_MANAGER
/*
*  This struct is only for dhcpc because it have two sub tables.
*  For the two table, they just use common link struct because they havenot sub tables.
*/
#define  COSA_CONTEXT_DHCPCV6_LINK_CLASS_CONTENT                                              \
        COSA_CONTEXT_LINK_CLASS_CONTENT                                                     \
        SLIST_HEADER                      SentOptionList;                                    \
        ULONG                             maxInstanceOfSent;                                \
        PCOSA_DML_DHCPCV6_SVR             pServerEntry;                                       \
        ULONG                             NumberOfServer;                                     \
        ULONG                             PreviousVisitTimeOfServer;                                      \
        PCOSA_DML_DHCPCV6_RECV            pRecvEntry;                                    \
        ULONG                             NumberOfRecv;                                     \
        ULONG                             PreviousVisitTimeOfRecv;                                      \
        CHAR                              AliasOfSent[COSA_DML_DHCPV6_ALIAS];                \

typedef  struct
_COSA_CONTEXT_DHCPCV6_LINK_OBJECT
{
    COSA_CONTEXT_DHCPCV6_LINK_CLASS_CONTENT
}
COSA_CONTEXT_DHCPCV6_LINK_OBJECT, *PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT;

#define  ACCESS_COSA_CONTEXT_DHCPCV6_LINK_OBJECT(p)                                \
         ACCESS_CONTAINER(p, COSA_CONTEXT_DHCPCV6_LINK_OBJECT, Linkage)            \

#endif
/*
*  This struct is only for dhcps.pool because it have one sub tables.
*  For the two table, they just use common link struct because they havenot sub tables.
*/
#define  COSA_CONTEXT_POOL_LINK_CLASS_CONTENT                                              \
        COSA_CONTEXT_LINK_CLASS_CONTENT                                                    \
        PCOSA_DML_DHCPSV6_CLIENT          pClientList;                               \
        PCOSA_DML_DHCPSV6_CLIENTCONTENT   pClientContentList;                        \
        ULONG                             NumberOfClient;                                     \
        ULONG                             PreviousVisitTimeOfClient;                                      \
        SLIST_HEADER                      OptionList;                               \
        ULONG                             maxInstanceOfOption;                      \
        CHAR                              AliasOfOption[COSA_DML_DHCPV6_ALIAS];           \

typedef  struct
_COSA_CONTEXT_POOLV6_LINK_OBJECT
{
    COSA_CONTEXT_POOL_LINK_CLASS_CONTENT
}
COSA_CONTEXT_POOLV6_LINK_OBJECT, *PCOSA_CONTEXT_POOLV6_LINK_OBJECT;

#define  ACCESS_COSA_CONTEXT_POOLV6_LINK_OBJECT(p)              \
         ACCESS_CONTAINER(p, COSA_CONTEXT_POOLV6_LINK_OBJECT, Linkage)

/*
*  This struct is for dhcp.
*/
#define  COSA_DATAMODEL_DHCPV6_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                              \
    COSA_BASE_CONTENT                                                                     \
    /* start of NAT object class content */                                                      \
    SLIST_HEADER                    ClientList;    /* This is for entry added */               \
    SLIST_HEADER                    PoolList;    /* This is for entry added */                 \
    ULONG                           maxInstanceOfClient;                                  \
    ULONG                           maxInstanceOfPool;                                    \
    ANSC_HANDLE                     hIrepFolderDhcpv6;                                    \
    CHAR                            AliasOfClient[COSA_DML_DHCPV6_ALIAS];                 \
    CHAR                            AliasOfPool[COSA_DML_DHCPV6_ALIAS];                   \

typedef  struct
_COSA_DATAMODEL_DHCPV6
{
    COSA_DATAMODEL_DHCPV6_CLASS_CONTENT
}
COSA_DATAMODEL_DHCPV6,  *PCOSA_DATAMODEL_DHCPV6;

#ifndef FEATURE_RDKB_WAN_MANAGER

#define   DHCPV6_CLIENT_ENTRY_MATCH(src,dst)                       \
    (strcmp((src)->Alias, (dst)->Alias) == 0)

#define   DHCPV6_CLIENT_ENTRY_MATCH2(src,dst)                      \
    (strcmp((src), (dst)) == 0)

#define   DHCPV6_SENDOPTION_ENTRY_MATCH(src,dst)                   \
    (strcmp((src)->Alias, (dst)->Alias) == 0)

#define   DHCPV6_SENDOPTION_ENTRY_MATCH2(src,dst)                  \
    (strcmp((src), (dst)) == 0)

#define   DHCPV6_REQOPTION_ENTRY_MATCH(src,dst)                    \
    (strcmp((src)->Alias, (dst)->Alias) == 0)

#define   DHCPV6_REQOPTION_ENTRY_MATCH2(src,dst)                   \
    (strcmp((src), (dst)) == 0)

#define   DHCPV6_POOL_ENTRY_MATCH(src,dst)                         \
    (strcmp((src)->Alias, (dst)->Alias) == 0)

#endif

#define   DHCPV6_POOL_ENTRY_MATCH2(src,dst)                        \
    (strcmp((src), (dst)) == 0)

#define   DHCPV6_POOLOPTION_ENTRY_MATCH(src,dst)                \
    (strcmp((src)->Alias, (dst)->Alias) == 0)

#define   DHCPV6_POOLOPTION_ENTRY_MATCH2(src,dst)               \
    (strcmp((src), (dst)) == 0)

#ifndef FEATURE_RDKB_WAN_MANAGER

#define   DHCPV6_CLIENT_INITIATION_CONTEXT(pDhcpc)                 \
    COSA_CONTEXT_LINK_INITIATION_CONTENT(((PCOSA_CONTEXT_LINK_OBJECT)(pDhcpc)))  \
    AnscSListInitializeHeader(&(pDhcpc)->SentOptionList);          \
    (pDhcpc)->maxInstanceOfSent               = 0;                 \
    (pDhcpc)->pServerEntry                    = NULL;                 \
    (pDhcpc)->NumberOfServer                  = 0;                 \
    (pDhcpc)->PreviousVisitTimeOfServer       = 0;                 \
    (pDhcpc)->pRecvEntry                      = NULL;                 \
    (pDhcpc)->NumberOfRecv                    = 0;                 \
    (pDhcpc)->PreviousVisitTimeOfRecv         = 0;                 \
    AnscZeroMemory((pDhcpc)->AliasOfSent, sizeof((pDhcpc)->AliasOfSent) ); \

#endif

#define   DHCPV6_POOL_INITIATION_CONTEXT(pPool)                            \
    COSA_CONTEXT_LINK_INITIATION_CONTENT(((PCOSA_CONTEXT_LINK_OBJECT)(pPool)))   \
    AnscSListInitializeHeader(&(pPool)->OptionList);        \
    (pPool)->maxInstanceOfOption       = 0;                 \
    (pPool)->pClientList               = NULL;                 \
    (pPool)->pClientContentList        = NULL;                 \
    (pPool)->NumberOfClient            = 0;                 \
    (pPool)->PreviousVisitTimeOfClient = 0;                 \
    AnscZeroMemory((pPool)->AliasOfOption, sizeof((pPool)->AliasOfOption) ); \

#ifndef FEATURE_RDKB_WAN_MANAGER

#define DHCPV6_CLIENT_SET_DEFAULTVALUE(pDhcpc)                                         \
    (pDhcpc)->Cfg.bEnabled                    = FALSE;                                 \
    AnscZeroMemory((pDhcpc)->Cfg.Interface, sizeof((pDhcpc)->Cfg.Interface));          \
    (pDhcpc)->Info.Status                     = COSA_DML_DHCP_STATUS_Disabled;         \

#define DHCPV6_SENTOPTION_SET_DEFAULTVALUE(pSentOption)                                \
    (pSentOption)->bEnabled                   = FALSE;                                 \
    (pSentOption)->Tag                        = 0;                                    \
    AnscZeroMemory( (pSentOption)->Value, sizeof( (pSentOption)->Value ) );           \

#endif

#define DHCPV6_POOL_SET_DEFAULTVALUE(pPool)                                            \
    (pPool)->Cfg.bEnabled                     = FALSE;                                 \
    (pPool)->Cfg.Order                        = 1;                                     \
    (pPool)->Cfg.DUIDExclude                  = FALSE;                                 \
    (pPool)->Cfg.VendorClassIDExclude         = FALSE;                                 \
    (pPool)->Cfg.UserClassIDExclude           = FALSE;                                 \
    (pPool)->Cfg.SourceAddressExclude         = FALSE;                                 \

#define DHCPV6_POOLOPTION_SET_DEFAULTVALUE(pPool)                                  \
    (pPool)->bEnabled                        = FALSE;                                  \
    AnscZeroMemory( (pPool)->Value, sizeof( (pPool)->Value ) );                \


/*
    Function declaration
*/

/**
 * @brief Create the DHCPv6 backend object.
 *
 * @return Handle to the newly created DHCPv6 backend object
 * @retval non-NULL Handle to the new create context
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaDhcpv6Create
    (
        VOID
    );

/**
 * @brief Initializes the DHCPv6 backend object.
 *
 * @param[in] hThisObject Handle to the DHCPv6 backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDhcpv6Initialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the DHCPv6 backend object.
 *
 * @param[in] hThisObject Handle to the DHCPv6 backend object to be removed.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful removal.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDhcpv6Remove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Loads DHCPv6 configuration from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the DHCPv6 backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful configuration load
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDhcpv6RegGetDhcpv6Info
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Saves DHCPv6 configuration to persistent registry storage.
 *
 * @param[in] hThisObject Handle to the DHCPv6 backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful configuration save.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDhcpv6RegSetDhcpv6Info
    (
        ANSC_HANDLE                 hThisObject
    );
#ifndef FEATURE_RDKB_WAN_MANAGER
/**
 * @brief Checks if a DHCPv6 client has delay-added child entries.
 *
 * @param[in] hContext Pointer to the DHCPv6 client context link object.
 *
 * @return The status of the operation.
 * @retval TRUE if the client has delay-added child entries.
 * @retval FALSE otherwise.
 */
BOOL
CosaDhcpv6ClientHasDelayAddedChild
    (
        PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT     hContext
    );
#endif
/**
 * @brief Checks if a DHCPv6 pool has delay-added child entries.
 *
 * @param[in] hContext Pointer to the DHCPv6 pool context link object.
 *
 * @return The status of the operation.
 * @retval TRUE if the pool has delay-added child entries,
 * @retval FALSE otherwise.
 */
BOOL
CosaDhcpv6PoolHasDelayAddedChild
    (
        PCOSA_CONTEXT_POOLV6_LINK_OBJECT      hContext
    );

/**
 * @brief Retrieves DHCPv6 client content by client context.
 *
 * @param[in] hClientContext Handle to the DHCPv6 client context.
 *
 * @return Handle to the DHCPv6 client content structure
 * @retval non-NULL Handle to the client content
 * @retval NULL if not found.
 */
ANSC_HANDLE
CosaDhcpv6GetClientContentbyClient
    (
        ANSC_HANDLE                 hClientContext
    );

/**
 * @brief Fetches DHCPv6 information from backend HAL APIs.
 *
 * @param[in] hThisObject Handle to the DHCPv6 backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful backend fetch.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDhcpv6BackendGetDhcpv6Info
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Converts an IP address string to an array of ULONG values.
 *
 * @param[out] pIPAddr Pointer to an array of ULONG values to store the converted IP address.
 * @param[in] pString Pointer to the null-terminated IP address string to be converted.
 * @param[in] MaxNumber Maximum number of ULONG elements in the pIPAddr array (4 for IPv4, 16 for IPv6 bytes).
 *
 * @return The status of the operation.
 * @retval TRUE on successful conversion.
 * @retval FALSE on invalid format or buffer overflow.
 */
BOOL
CosaDmlSetIpaddr
    (
        PULONG pIPAddr,
        PCHAR pString,
        ULONG MaxNumber
    );

#endif
