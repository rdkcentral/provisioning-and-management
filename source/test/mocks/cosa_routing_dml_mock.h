/*****************************************************************************
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2024 RDK Management
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
*******************************************************************************/

#ifndef COSA_ROUTING_DML_MOCK_H
#define COSA_ROUTING_DML_MOCK_H

//#include "cosa_routing_apis.h"
#ifdef __cplusplus
extern "C" {
#endif

#define COSA_DML_IPV6_ADDR_LENGTH 128
//#include "cosa_routing_apis.h"
#include "cosa_apis.h"
#include "ansc_status.h"
#include "ansc_debug_wrapper_base.h"
#include "user_base.h"
#include "ansc_common_structures.h"

typedef  enum
_COSA_DML_RIP_MODE
{
    COSA_DML_RIP_MODE_Send              = 1,
    COSA_DML_RIP_MODE_Receive,
    COSA_DML_RIP_MODE_Both
}
COSA_DML_RIP_MODE, *PCOSA_DML_RIP_MODE;

typedef  struct
_COSA_DML_RIP_IF_CFG
{
    /*readonly*/
    ULONG                           InstanceNumber;
    COSA_DML_STATUS                 Status;

    /*writable */
    CHAR                            Alias[COSA_DML_IF_NAME_LENGTH];

    CHAR                            Interface[COSA_DML_IF_NAME_LENGTH]; /* IP interface name */
    BOOLEAN                         Enable;

    BOOLEAN                         AcceptRA;
    BOOLEAN                         SendRA;
    USHORT                          X_CISCO_COM_SendVersion;
    USHORT                          X_CISCO_COM_ReceiveVersion;

    ULONG                           X_CISCO_COM_Neighbor;

    ULONG                           X_CISCO_COM_AuthenticationType;
    ULONG                           X_CISCO_COM_Md5KeyID;
    CHAR                            X_CISCO_COM_Md5KeyValue[128];
    CHAR                            X_CISCO_COM_SimplePassword[128];
}
COSA_DML_RIP_IF_CFG, *PCOSA_DML_RIP_IF_CFG;

typedef  enum
_COSA_DML_ROUTEINFO_PRF_TYPE
{
    COSA_DML_ROUTEINFO_PRF_High     = 1,
    COSA_DML_ROUTEINFO_PRF_Medium,
    COSA_DML_ROUTEINFO_PRF_Low
}
COSA_DML_ROUTEINFO_PRF_TYPE, *PCOSA_DML_ROUTEINFO_PRF_TYPE;

typedef  enum
_COSA_DML_ROUTEINFO_STATUS_TYPE
{
    COSA_DML_ROUTEINFO_STATUS_ForwardingEntryCreated = 1,
    COSA_DML_ROUTEINFO_STATUS_NoForwardingEntry ,
    COSA_DML_ROUTEINFO_STATUS_Error ,
}
COSA_DML_ROUTEINFO_STATUS_TYPE, *PCOSA_DML_ROUTEINFO_STATUS_TYPE;


typedef  struct
_COSA_DML_ROUTEINFO_IF_INFO
{
    char                            Interface[256]; /* IP interface name */
    char                            SourceRouter[64];
    char                            Prefix[64];
    char                            RouteLifetime[32];
    COSA_DML_ROUTEINFO_PRF_TYPE     PreferredRouteFlag;
    COSA_DML_ROUTEINFO_STATUS_TYPE            Status;
}
COSA_DML_ROUTEINFO_IF_INFO,  *PCOSA_DML_ROUTEINFO_IF_INFO;

typedef  struct
_COSA_DML_RIP_CFG
{
    BOOLEAN                         Enable;
    COSA_DML_RIP_MODE               Mode;
    ULONG                           X_CISCO_COM_UpdateInterval;
    ULONG                           X_CISCO_COM_DefaultMetric;
}
COSA_DML_RIP_CFG, *PCOSA_DML_RIP_CFG;

typedef  enum
_COSA_DML_ROUTING_STATUS
{
    COSA_DML_ROUTING_STATUS_Enabled     = 1,
    COSA_DML_ROUTING_STATUS_Disabled,
    COSA_DML_ROUTING_STATUS_Error
}
COSA_DML_ROUTING_STATUS, *PCOSA_DML_ROUTING_STATUS;

typedef  enum
_COSA_DML_ROUTING_V6_ORIGIN
{
    COSA_DML_ROUTING_IPV6_ORIGIN_DHCPv6     = 1,
    COSA_DML_ROUTING_IPV6_ORIGIN_OSPF,
    COSA_DML_ROUTING_IPV6_ORIGIN_RA,
    COSA_DML_ROUTING_IPV6_ORIGIN_RIPng,
    COSA_DML_ROUTING_IPV6_ORIGIN_Static
}
COSA_DML_ROUTING_V6_ORIGIN, *PCOSA_DML_ROUTING_V6_ORIGIN;

typedef  struct
_COSA_DML_ROUTING_V6_ENTRY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         Enable;
    COSA_DML_ROUTING_STATUS         Status;                 /* Read-only */

    char                            DestIPPrefix[64];
    int                             ForwardingPolicy;       /* Not supported for now */
    char                            NextHop[COSA_DML_IPV6_ADDR_LENGTH];
    char                            Interface[COSA_DML_IF_NAME_LENGTH]; /* IP interface name */
    COSA_DML_ROUTING_V6_ORIGIN      Origin;                 /* Read-only */
    int                             ForwardingMetric;
    char                            ExpirationTime[64];         /* Read-only */
}
COSA_DML_ROUTING_V6_ENTRY,  *PCOSA_DML_ROUTING_V6_ENTRY;

	
typedef  enum
_COSA_DML_ROUTING_V4_ORIGIN
{
    COSA_DML_ROUTING_IPV4_ORIGIN_DHCPv4     = 1,
    COSA_DML_ROUTING_IPV4_ORIGIN_OSPF,
    COSA_DML_ROUTING_IPV4_ORIGIN_IPCP,
    COSA_DML_ROUTING_IPV4_ORIGIN_RIP,
    COSA_DML_ROUTING_IPV4_ORIGIN_Static
}
COSA_DML_ROUTING_V4_ORIGIN, *PCOSA_DML_ROUTING_V4_ORIGIN;

typedef  struct
_COSA_DML_ROUTER_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
}
COSA_DML_ROUTER_CFG,  *PCOSA_DML_ROUTER_CFG;

typedef  struct
_COSA_DML_STATICROUTE_CFG
{
    BOOLEAN   RIPAdvertise;
    ULONG     DestIPAddress;
    ULONG     DestSubnetMask;
    ULONG     GatewayIPAddress;
    char      Name[64];
}
COSA_DML_STATICROUTE_CFG,  *PCOSA_DML_STATICROUTE_CFG;

typedef  struct
_COSA_DML_ROUTING_V4_ENTRY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         Enable;
    COSA_DML_ROUTING_STATUS         Status;             /* Read-only */
    BOOLEAN                         StaticRoute;        /* Read-only */
    ANSC_IPV4_ADDRESS               DestIPAddress;
    ANSC_IPV4_ADDRESS               DestSubnetMask;
    int                             ForwardingPolicy;   /* Not supported for now */
    ANSC_IPV4_ADDRESS               GatewayIPAddress;
    char                            Interface[64];      /* IP interface name */
    int                             ForwardingMetric;
    COSA_DML_ROUTING_V4_ORIGIN      Origin;
    BOOLEAN                         X_CISCO_COM_RIPAdvertise;
}
COSA_DML_ROUTING_V4_ENTRY,  *PCOSA_DML_ROUTING_V4_ENTRY;

typedef  struct
_COSA_DML_ROUTEINFO_CFG
{
    BOOLEAN                         bEnabled;
}
COSA_DML_ROUTEINFO_CFG, *PCOSA_DML_ROUTEINFO_CFG;

typedef  struct
_COSA_DML_ROUTEINFO_FULL
{
    COSA_DML_ROUTEINFO_CFG                Cfg;
    ULONG                                 IfNum;
    COSA_DML_ROUTEINFO_IF_INFO            * pInfo;
}
COSA_DML_ROUTEINFO_FULL, *PCOSA_DML_ROUTEINFO_FULL;

#define  COSA_DATAMODEL_ROUTING_CLASS_CONTENT                               \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of NAT object class content */                                 \
    SLIST_HEADER                    RouterList;                             \
    ULONG                           ulNextRouterInsNum;                     \
    ANSC_HANDLE                     hIrepFolderCOSA;                        \
    ANSC_HANDLE                     hIrepFolderRouter;                      \
    ANSC_HANDLE                     hIrepFolderRouterHA;                    \
    COSA_DML_ROUTEINFO_FULL         RouteInfo;                              \
    ULONG                           PreviousTime;                           \
    SLIST_HEADER                    StaticRoute;                            \
    ULONG                           NextInstanceNumber;                     \
    SLIST_HEADER                    RipIFList;                              \
    COSA_DML_RIP_CFG                RIPCfg;                                 \
    ULONG                           ulNextRipIFInsNum;                      \

typedef  struct
_COSA_DATAMODEL_ROUTING
{
    COSA_DATAMODEL_ROUTING_CLASS_CONTENT
}
COSA_DATAMODEL_ROUTING,  *PCOSA_DATAMODEL_ROUTING;


ANSC_STATUS
CosaRoutingRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaRoutingRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pNextInsNumName,
        ULONG                       ulNextInsNum,
        char*                       pPreffix,
        ULONG                       ulUpperInsNum,
        char*                       pAlias,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
is_usg_in_bridge_mode(BOOL *pBridgeMode);

ANSC_STATUS
CosaDmlStaticRouteDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    );

ANSC_STATUS
CosaDmlStaticRouteAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    );

ANSC_STATUS
CosaDmlStaticRouteGetEntryByName
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    );

ANSC_STATUS
CosaDmlRoutingDelV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingAddV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingSetV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingRouterGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    );

ANSC_STATUS
CosaDmlRoutingGetV4Entry2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingDelV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry
    );

ULONG
CosaDmlRoutingGetNumberOfV6Entries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlRoutingGetV6Entry
    (
        ANSC_HANDLE                     hContext,
        ULONG                           ulIndex,
        PCOSA_DML_ROUTING_V6_ENTRY      pEntry
    );

PUCHAR
CosaUtilGetFullPathNameByKeyword
    (
        PUCHAR                          pTableName,
        PUCHAR                          pParameterName,
        PUCHAR                          pKeyword
    );

ANSC_STATUS
isValidInput
    (
        char                           *inputparam,
        char                           *wrapped_inputparam,
        int                             lengthof_inputparam,
        int                             sizeof_wrapped_inputparam
    );

ANSC_STATUS
CosaDmlRoutingAddV6Entry
    (
        ANSC_HANDLE                     hContext,
        PCOSA_DML_ROUTING_V6_ENTRY      pEntry
    );

ANSC_STATUS
CosaDmlRoutingSetV6Entry
    (
        ANSC_HANDLE                     hContext,
        PCOSA_DML_ROUTING_V6_ENTRY      pEntry
    );

ANSC_STATUS
CosaDmlRoutingGetV6Entry2
    (
        ANSC_HANDLE                     hContext,
        PCOSA_DML_ROUTING_V6_ENTRY      pEntry
    );

ANSC_STATUS
CosaDmlRipSetCfg
    (
        ANSC_HANDLE                     hContext,
        PCOSA_DML_RIP_CFG               pCfg
    );

ANSC_STATUS
CosaDmlRipIfAddCfg
    (
        ANSC_HANDLE                     hContext,
        PCOSA_DML_RIP_IF_CFG            pEntry
    );

ANSC_STATUS
CosaDmlRipIfSetCfg
    (
        ANSC_HANDLE                     hContext,
        PCOSA_DML_RIP_IF_CFG            pEntry
    );

ANSC_STATUS
CosaDmlRipIfGetCfg
    (
        ANSC_HANDLE                     hContext,
        ULONG                           ulIndex,
        PCOSA_DML_RIP_IF_CFG            pEntry
    );

ANSC_STATUS
CosaDmlRouteInfoSetEnabled
    (
        BOOLEAN                         value
    );

ANSC_STATUS
CosaDmlRouteInfoGetEnabled
    (
        BOOLEAN                        *pBool
    );

PCOSA_DML_ROUTEINFO_IF_INFO
CosaDmlRoutingGetRouteInfoIf
    (
        ANSC_HANDLE                     hContext,
        PULONG                          pulCount
    ); 
ANSC_STATUS
CosaDmlStaticRouteSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    );

ANSC_STATUS
CosaDmlRoutingRouterSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    );
ANSC_STATUS
CosaDmlRoutingSetV6EntryValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );


#ifdef __cplusplus
}
#endif

#endif // COSA_ROUTING_DML_MOCK_H
