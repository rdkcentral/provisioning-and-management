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

#include "cosa_routing_dml_mock.h"
#include <stdio.h>

//#define ANSC_STATUS unsigned long
/*
extern "C"
{
    #include "cosa_apis.h"
    #include "ansc_status.h"
    #include "ansc_debug_wrapper_base.h"
    #include "user_base.h"
    #include "ansc_common_structures.h"
}
*/
ANSC_STATUS
CosaSListPushEntryByInsNum
    (
        PSLIST_HEADER               pListHead,
        PCOSA_CONTEXT_LINK_OBJECT   pCosaContext
    )
{
    (void)pListHead;
    (void)pCosaContext;
    return ANSC_STATUS_SUCCESS;
}

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
    )
{


        (void)hThisObject;
        (void)pNextInsNumName;
        (void)ulNextInsNum;
        (void)pPreffix;
        (void)ulUpperInsNum;
        (void)pAlias;
        (void)hCosaContext;
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaRoutingRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{

    (void)hThisObject;
    (void)hCosaContext;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS is_usg_in_bridge_mode(BOOL *pBridgeMode)
{
    *pBridgeMode = FALSE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlStaticRouteDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlStaticRouteAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlStaticRouteGetEntryByName
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRoutingDelV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingAddV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingSetV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingRouterGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    )
{

    (void)(hContext);
    (void)(pCfg);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingGetV4Entry2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingDelV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    )
{

    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlRoutingGetNumberOfV6Entries
    (
        ANSC_HANDLE                 hContext
    )
{
    (void)(hContext);
    return 0;
}

ANSC_STATUS
CosaDmlRoutingGetV6Entry
    (
        ANSC_HANDLE                     hContext,
        ULONG                           ulIndex,
        PCOSA_DML_ROUTING_V6_ENTRY      pEntry
    )
{
    (void)(hContext);
    (void)ulIndex;
    (void)pEntry;

    return ANSC_STATUS_SUCCESS;
}

PUCHAR
CosaUtilGetFullPathNameByKeyword
    (
        PUCHAR                      pTableName,
        PUCHAR                      pParameterName,
        PUCHAR                      pKeyword
    )
{
    (void)pTableName;
    (void)pParameterName;
    (void)pKeyword;
    return NULL;
}

ANSC_STATUS
isValidInput
    (
        char                       *inputparam,
        char                       *wrapped_inputparam,
        int                         lengthof_inputparam,
        int                         sizeof_wrapped_inputparam
    )
{
    (void)inputparam;
    (void)wrapped_inputparam;
    (void)lengthof_inputparam;
    (void)sizeof_wrapped_inputparam;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingAddV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry
    )
{
    (void)hContext;
    (void)pEntry;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingSetV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    )
{
    (void)hContext;
    (void)pEntry;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingGetV6Entry2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRipSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_CFG           pCfg
    )
{
    (void)(hContext);
    (void)(pCfg);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRipIfAddCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRipIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRipIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_RIP_IF_CFG        pEntry
    )
{
    (void)(hContext);
    (void)(ulIndex);
    (void)(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRouteInfoSetEnabled
    (
        BOOLEAN value
    )
{
    (void)value;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRouteInfoGetEnabled
    (
        BOOLEAN * pBool
    )
{
    (void)pBool;
    return ANSC_STATUS_SUCCESS;
}

PCOSA_DML_ROUTEINFO_IF_INFO
CosaDmlRoutingGetRouteInfoIf
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    (void)hContext;
    (void)pulCount;
    return NULL;
}

ANSC_STATUS
CosaDmlStaticRouteSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_STATICROUTE_CFG   pEntry
    )
{
    (void)(hContext);
    (void)(pEntry);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRoutingRouterSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    )
{

    (void)hContext;
    (void)pCfg;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRoutingSetV6EntryValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    (void)hContext;
    (void)ulIndex;
    (void)ulInstanceNumber;
    (void)pAlias;
    return ANSC_STATUS_SUCCESS;
}
