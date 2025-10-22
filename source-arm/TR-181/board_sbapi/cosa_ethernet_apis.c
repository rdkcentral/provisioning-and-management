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

    module: cosa_ethernet_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlEthInit
        *  CosaDmlEthPortGetNumberOfEntries
        *  CosaDmlEthPortGetEntry
        *  CosaDmlEthPortSetCfg
        *  CosaDmlEthPortGetCfg
        *  CosaDmlEthPortGetDinfo
        *  CosaDmlEthPortGetStats
        *  CosaDmlEthLinkGetNumberOfEntries
        *  CosaDmlEthLinkGetEntry
        *  CosaDmlEthLinkAddEntry
        *  CosaDmlEthLinkDelEntry
        *  CosaDmlEthLinkSetCfg
        *  CosaDmlEthLinkGetCfg
        *  CosaDmlEthLinkGetDinfo
        *  CosaDmlEthLinkGetStats
        *  CosaDmlEthVlanTerminationGetNumberOfEntries
        *  CosaDmlEthVlanTerminationGetEntry
        *  CosaDmlEthVlanTerminationAddEntry
        *  CosaDmlEthVlanTerminationDelEntry
        *  CosaDmlEthVlanTerminationSetCfg
        *  CosaDmlEthVlanTerminationGetCfg
        *  CosaDmlEthVlanTerminationGetDinfo
        *  CosaDmlEthVlanTerminationGetStats
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_ethernet_apis.h"
#include "cosa_ethernet_apis_multilan.h"
#include "secure_wrapper.h"
#include "safec_lib_common.h"

#ifdef _HUB4_PRODUCT_REQ_
#include "sysevent/sysevent.h"
#endif

#ifdef _COSA_SIM_

#elif defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)

#include "cosa_ethernet_apis_ext.h"
#include "cosa_drg_common.h"
#include "ansc_string_util.h"

#include "ccsp_psm_helper.h"
#include "dmsb_tr181_psm_definitions.h"

#include <utctx/utctx_api.h>
#include "linux/sockios.h"
#include <sys/ioctl.h>

#ifdef ARRIS_XB3_PLATFORM_CHANGES
#include "rdk_cm_api_arris.h"
#else
#include "linux/if.h"
#endif

#ifdef CORE_NET_LIB
#include <libnet.h>
#endif

static int saveID(char* ifName, char* pAlias, ULONG ulInstanceNumber);
static int loadID(char* ifName, char* pAlias, ULONG* ulInstanceNumber);
//static int saveLinkID(char* ifName, char* pAlias, ULONG ulInstanceNumber);
//static int loadLinkID(char* ifName, char* pAlias, ULONG* ulInstanceNumber);
COSA_DML_IF_STATUS getIfStatus(const PUCHAR name, struct ifreq *pIfr);
static int setIfStatus(struct ifreq *pIfr);
int _getMac(char* ifName, char* mac);
void rdkb_api_platform_hal_GetLanMacAddr(char* mac);

/**************************************************************************
                        DATA STRUCTURE DEFINITIONS
**************************************************************************/

/**************************************************************************
                        GLOBAL VARIABLES
**************************************************************************/
#include "ccsp_hal_ethsw.h" 

#if defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)

#include "syscfg/syscfg.h"

int puma6_getSwitchCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg);
int puma6_setSwitchCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg); 
int puma6_getSwitchDInfo(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_DINFO pDinfo);
int puma6_getSwitchStats(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_STATS pStats);


COSA_DML_ETH_PORT_SINFO      g_EthIntSInfo[] = 
    {
        /* Downstream (LAN) ports */
        {SWITCH_PORT_0_NAME,                FALSE,  {0,0,0,0,0,0}},
        {SWITCH_PORT_1_NAME,                FALSE,  {0,0,0,0,0,0}},
#if defined(ETH_3_PORTS) || defined(ETH_4_PORTS) || defined(ETH_5_PORTS) || defined(ETH_6_PORTS) || defined(ETH_8_PORTS)
        {SWITCH_PORT_2_NAME,                FALSE,  {0,0,0,0,0,0}},
#endif
#if defined(ETH_4_PORTS) || defined(ETH_5_PORTS) || defined(ETH_6_PORTS) || defined(ETH_8_PORTS)
        {SWITCH_PORT_3_NAME,                FALSE,  {0,0,0,0,0,0}},
#endif
#if defined(ETH_5_PORTS) || defined(ETH_6_PORTS) || defined(ETH_8_PORTS)
        {SWITCH_PORT_4_NAME,                FALSE,  {0,0,0,0,0,0}},
#endif
#if defined(ETH_6_PORTS) || defined(ETH_8_PORTS)
        {SWITCH_PORT_5_NAME,                FALSE,  {0,0,0,0,0,0}},
#endif
#if defined(ETH_8_PORTS)
        {SWITCH_PORT_6_NAME,                FALSE,  {0,0,0,0,0,0}},
        {SWITCH_PORT_7_NAME,                FALSE,  {0,0,0,0,0,0}},
#endif
#if defined(_HUB4_PRODUCT_REQ_)
        /* Upstream (WAN) ports */
        {DMSB_ETH_IF_NAME_DFT_WanRouting,   TRUE,   {0,0,0,0,0,0}}
#else        
        /* Upstream (WAN) ports */
        {DMSB_ETH_IF_NAME_DFT_WanRouting,   TRUE,   {0,0,0,0,0,0}},
#if defined(INTEL_PUMA7) && !defined(_ARRIS_XB6_PRODUCT_REQ_)
        {DMSB_ETH_IF_NAME_DFT_WanBridging,  TRUE,   {0,0,0,0,0,0}},
        {SWITCH_PORT_4_NAME,                FALSE,  {0,0,0,0,0,0}},
        {SWITCH_PORT_5_NAME,                FALSE,  {0,0,0,0,0,0}}
#else
        {DMSB_ETH_IF_NAME_DFT_WanBridging,  TRUE,   {0,0,0,0,0,0}}
#endif /* defined(INTEL_PUMA7) && !defined(_ARRIS_XB6_PRODUCT_REQ_) */
#endif /* _HUB4_PRODUCT_REQ_ */ 
    };

static const ULONG g_EthernetIntNum = sizeof(g_EthIntSInfo)/sizeof(g_EthIntSInfo[0]);

#if defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)
static int getIfCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg);
static int setIfCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg);
static int getIfStats(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_STATS pStats);
static int getIfDInfo(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_DINFO pDinfo);
#endif

static int getIfStats2(const PUCHAR pName, PCOSA_DML_ETH_STATS pStats);

static const EthIntControlFuncs ifFuncs = {
    getIfCfg,
    setIfCfg,
    getIfStats,
    getIfDInfo
};

static const EthIntControlFuncs swFuncs = {
    puma6_getSwitchCfg,
    puma6_setSwitchCfg,
    puma6_getSwitchStats,
    puma6_getSwitchDInfo
};

static const int g_PortIDs[]={
#if defined(ETH_5_PORTS)
    CCSP_HAL_ETHSW_EthPort1,
    CCSP_HAL_ETHSW_EthPort2,
    CCSP_HAL_ETHSW_EthPort3,
    CCSP_HAL_ETHSW_EthPort4,
    CCSP_HAL_ETHSW_EthPort5
#elif defined(ETH_6_PORTS)
    CCSP_HAL_ETHSW_EthPort1,
    CCSP_HAL_ETHSW_EthPort2,
    CCSP_HAL_ETHSW_EthPort3,
    CCSP_HAL_ETHSW_EthPort4,
    CCSP_HAL_ETHSW_EthPort5,
    CCSP_HAL_ETHSW_EthPort6
#elif defined(ETH_8_PORTS) || defined(INTEL_PUMA7)
    CCSP_HAL_ETHSW_EthPort1,
    CCSP_HAL_ETHSW_EthPort2,
    CCSP_HAL_ETHSW_EthPort3,
    CCSP_HAL_ETHSW_EthPort4,
    CCSP_HAL_ETHSW_EthPort5,
    CCSP_HAL_ETHSW_EthPort6,
    CCSP_HAL_ETHSW_EthPort7,
    CCSP_HAL_ETHSW_EthPort8
#else
    CCSP_HAL_ETHSW_EthPort1,
    CCSP_HAL_ETHSW_EthPort2,
    CCSP_HAL_ETHSW_EthPort3,
    CCSP_HAL_ETHSW_EthPort4
#endif
};

static CosaEthInterfaceInfo g_EthEntries[] = 
    {
#if defined(ETH_2_PORTS)
        {g_EthIntSInfo + 0, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 0, {0}},
        {g_EthIntSInfo + 1, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 1, {0}},
        {g_EthIntSInfo + 2, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}},
        {g_EthIntSInfo + 3, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}}
#elif defined(ETH_3_PORTS)
        {g_EthIntSInfo + 0, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 0, {0}},
        {g_EthIntSInfo + 1, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 1, {0}},
        {g_EthIntSInfo + 2, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 2, {0}},
        {g_EthIntSInfo + 3, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}},
        {g_EthIntSInfo + 4, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}}
#elif defined(ETH_5_PORTS)
        {g_EthIntSInfo + 0, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 0, {0}},
        {g_EthIntSInfo + 1, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 1, {0}},
        {g_EthIntSInfo + 2, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 2, {0}},
        {g_EthIntSInfo + 3, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 3, {0}},
        {g_EthIntSInfo + 4, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 4, {0}},
        {g_EthIntSInfo + 5, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}},
        {g_EthIntSInfo + 6, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}}
#elif defined(ETH_6_PORTS)
        {g_EthIntSInfo + 0, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 0, {0}},
        {g_EthIntSInfo + 1, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 1, {0}},
        {g_EthIntSInfo + 2, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 2, {0}},
        {g_EthIntSInfo + 3, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 3, {0}},
        {g_EthIntSInfo + 4, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 4, {0}},
        {g_EthIntSInfo + 5, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 5, {0}},
        {g_EthIntSInfo + 6, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}},
        {g_EthIntSInfo + 7, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}}
#elif defined(ETH_8_PORTS)
        {g_EthIntSInfo + 0, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 0, {0}},
        {g_EthIntSInfo + 1, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 1, {0}},
        {g_EthIntSInfo + 2, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 2, {0}},
        {g_EthIntSInfo + 3, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 3, {0}},
        {g_EthIntSInfo + 4, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 4, {0}},
        {g_EthIntSInfo + 5, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 5, {0}},
        {g_EthIntSInfo + 6, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 6, {0}},
        {g_EthIntSInfo + 7, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 7, {0}},
        {g_EthIntSInfo + 8, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}},
        {g_EthIntSInfo + 9, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}}
#else
        {g_EthIntSInfo + 0, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 0, {0}},
        {g_EthIntSInfo + 1, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 1, {0}},
        {g_EthIntSInfo + 2, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 2, {0}},
        {g_EthIntSInfo + 3, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 3, {0}},
        {g_EthIntSInfo + 4, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}},
#if defined(INTEL_PUMA7) && !defined(_ARRIS_XB6_PRODUCT_REQ_)
        {g_EthIntSInfo + 5, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}},
        {g_EthIntSInfo + 6, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 4, {0}},
        {g_EthIntSInfo + 7, {'\0'}, 0, 0, &swFuncs, g_PortIDs + 5, {0}}
#else
        {g_EthIntSInfo + 5, {'\0'}, 0, 0, &ifFuncs, NULL,          {0}}
#endif
#endif
    };

#endif


/**********************************************************************
                            Routine Trunks
**********************************************************************/

ANSC_STATUS
CosaDmlEthInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    ANSC_STATUS                     returnStatus;

#if defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)
    CHAR strMac[128]        = {0};
    ULONG i                 = 0;
    ULONG wanIndex          = 0;

    UNREFERENCED_PARAMETER(phContext);

#if !defined(_HUB4_PRODUCT_REQ_)
    ULONG lbrIndex          = 0;
#endif

#ifdef FEATURE_RDKB_XDSL_PPP_MANAGER
    char wanPhyName[32] = {0};
    char out_value[32] = {0};
#endif

    /*
     *  Manufacturer programmed MAC address for LAN interface should be read out here. -- DH  
     *
     *  It doesn't make sense to even have a MAC address in Ethernet Interface DM object,
     *  so we are not going to fill the MAC address for Upstream interfaces.
     */
    _getMac("brlan0", strMac);

    /*  Iterate through Ethernet ports, assign LAN mac to downstream ports
        Keep track of the index of upstream ports to assign their MAC addresses
     */
    for (i=0; i < g_EthernetIntNum; ++i) {
        if (!g_EthIntSInfo[i].bUpstream) {
            if ( AnscSizeOfString(strMac) != 0 )
                AnscCopyMemory(g_EthIntSInfo[i].MacAddress, strMac, 6);
        }
        else {
            if (strcmp(g_EthIntSInfo[i].Name, DMSB_ETH_IF_NAME_DFT_WanRouting) == 0)
                wanIndex = i;
#if !defined(_HUB4_PRODUCT_REQ_)
            if (strcmp(g_EthIntSInfo[i].Name, DMSB_ETH_IF_NAME_DFT_WanBridging) == 0)
                lbrIndex = i;
#endif
        }
    }
#ifdef FEATURE_RDKB_XDSL_PPP_MANAGER
    if (syscfg_get(NULL, "wan_physical_ifname", out_value, sizeof(out_value)) == 0)
    {
       strncpy(wanPhyName, out_value, sizeof(wanPhyName));
       CcspTraceInfo(("%s %d - WanPhyName=%s \n", __FUNCTION__,__LINE__, wanPhyName));
    }
    else
    {
       strncpy(wanPhyName, "erouter0", sizeof(wanPhyName));
       CcspTraceInfo(("%s %d - WanPhyName=%s \n", __FUNCTION__,__LINE__, wanPhyName));
    }
    if ( (-1 != _getMac(wanPhyName, strMac)))
#else
    if ( (-1 != _getMac("erouter0", strMac)))
#endif	    
                AnscCopyMemory(g_EthIntSInfo[wanIndex].MacAddress, strMac, 6);

#if !defined(_HUB4_PRODUCT_REQ_)
    if ( (-1 != _getMac("lbr0", strMac)) )
                AnscCopyMemory(g_EthIntSInfo[lbrIndex].MacAddress, strMac, 6);
#endif

#endif 

    returnStatus = CosaDmlEthMlanInit(hDml, phContext);

    if ( returnStatus  != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }

    for (i=0; i < g_EthernetIntNum; ++i) {
        loadID(g_EthEntries[i].sInfo->Name, g_EthEntries[i].Alias, &g_EthEntries[i].instanceNumber);
        g_EthEntries[i].LastChange = AnscGetTickInSeconds();
    }

    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlEthPortGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{  
    UNREFERENCED_PARAMETER(hContext);
    return g_EthernetIntNum;
}

ANSC_STATUS
CosaDmlEthPortGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ETH_PORT_FULL     pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (pEntry)
    {
        _ansc_memset(pEntry, 0, sizeof(COSA_DML_ETH_PORT_FULL));
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

#if defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)
    if (ulIndex < g_EthernetIntNum)
    {
        g_EthEntries[ulIndex].control->getCfg(g_EthEntries + ulIndex, &pEntry->Cfg);
        AnscCopyMemory(&pEntry->StaticInfo, &g_EthIntSInfo[ulIndex], sizeof(COSA_DML_ETH_PORT_SINFO));
        g_EthEntries[ulIndex].control->getDInfo(g_EthEntries + ulIndex, &pEntry->DynamicInfo);
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
#endif
}

ANSC_STATUS
CosaDmlEthPortSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    g_EthEntries[ulIndex].instanceNumber=ulInstanceNumber;
    errno_t rc = -1;
    rc = strcpy_s(g_EthEntries[ulIndex].Alias,sizeof(g_EthEntries[ulIndex].Alias), pAlias);
    ERR_CHK(rc);
    saveID(g_EthIntSInfo[ulIndex].Name, pAlias, ulInstanceNumber);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthPortSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_PORT_CFG      pCfg
    )
{
    COSA_DML_ETH_PORT_CFG origCfg;
    PCosaEthInterfaceInfo pEthIf = (PCosaEthInterfaceInfo  )NULL;
    errno_t rc = -1;
    UNREFERENCED_PARAMETER(hContext);

    /*RDKB-6838, CID-32984, null check before use*/
    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }

    pEthIf = getIF(pCfg->InstanceNumber);

    if ( !pEthIf )
    {
        return ANSC_STATUS_FAILURE;
    }


    pEthIf->control->getCfg(pEthIf, &origCfg);

    pEthIf->control->setCfg(pEthIf, pCfg);

    if ( origCfg.bEnabled != pCfg->bEnabled )
    {
        //pEthIf->control->getStats(pEthIf, &pEthIf->LastStats);

        pEthIf->LastChange = AnscGetTickInSeconds();
    }
    
    if (strcmp(pCfg->Alias, pEthIf->Alias) != 0)
    {
        rc = strcpy_s(pEthIf->Alias,sizeof(pEthIf->Alias), pCfg->Alias);
        ERR_CHK(rc);
        saveID(pEthIf->sInfo->Name, pCfg->Alias, pCfg->InstanceNumber);
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthPortGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_PORT_CFG      pCfg
    )
{
    PCosaEthInterfaceInfo pEthIf = (PCosaEthInterfaceInfo  )NULL;
    errno_t rc = -1;
    UNREFERENCED_PARAMETER(hContext);

    /*RDKB-6838, CID-33167, null check before use*/
    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }

    pEthIf = getIF(pCfg->InstanceNumber);

    if ( !pEthIf )
    {
        return ANSC_STATUS_FAILURE;
    }


    pEthIf->control->getCfg(pEthIf, pCfg);
    
    rc = strcpy_s(pCfg->Alias,sizeof(pCfg->Alias), pEthIf->Alias);
    ERR_CHK(rc);

    pCfg->InstanceNumber = pEthIf->instanceNumber;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthPortGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_PORT_DINFO    pInfo
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (!pInfo)
    {
        return ANSC_STATUS_FAILURE;
    }

    PCosaEthInterfaceInfo pEthIf = getIF(ulInstanceNumber);

    if ( !pEthIf )
    {
        return ANSC_STATUS_FAILURE;
    }

    pEthIf->control->getDInfo(pEthIf, pInfo);

    pInfo->LastChange = pEthIf->LastChange;
    
    return ANSC_STATUS_SUCCESS;
}

static int ethGetClientsCount
    (
        ULONG PortId,
        LONG num_eth_device,
        eth_device_t *eth_device
        )
{
    int idx;
    int count_client = 0;

    if (!eth_device)
    {
        CcspTraceWarning(("ethGetClientsCount Invalid input Param\n"));
        return 0;
    }

    for (idx = 0; idx < num_eth_device; idx++)
    {
        if (PortId == (ULONG)eth_device[idx].eth_port)
        {
            count_client++;
        }
    }

    return count_client;
}

static void ethGetClientMacDetails
    (
	LONG PortId,
        LONG client_num,
        LONG num_eth_device,
        eth_device_t *eth_device,
        char *mac
    )
{
	int idx;
	int isClient = 0;

	if (!eth_device || !mac)
	{
		CcspTraceWarning(("ethGetClientMacDetails Invalid input Param\n"));
		return;
	}

	for (idx = 0; idx < num_eth_device; idx++)
	{
		if (PortId == eth_device[idx].eth_port)
		{
			isClient++;
			if (isClient == client_num) {
				sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
					eth_device[idx].eth_devMacAddress[0],
					eth_device[idx].eth_devMacAddress[1],
					eth_device[idx].eth_devMacAddress[2],
					eth_device[idx].eth_devMacAddress[3],
					eth_device[idx].eth_devMacAddress[4],
					eth_device[idx].eth_devMacAddress[5]);
				return;
			}
		}
	}
}

ANSC_STATUS
CosaDmlEthPortGetClientMac
    (
	PCOSA_DML_ETH_PORT_FULL pEthernetPortFull,
        ULONG			ulInstanceNumber
    )
{
	int ret = ANSC_STATUS_FAILURE;

	ULONG total_eth_device = 0;
	eth_device_t *output_struct = NULL;

	ret = CcspHalExtSw_getAssociatedDevice(&total_eth_device, &output_struct);
	if (ANSC_STATUS_SUCCESS != ret)
	{
		CcspTraceError(("%s CcspHalExtSw_getAssociatedDevice failed\n", __func__));
		return ret;
	}

        if ( total_eth_device )
        {    
           ULONG i = 1; 
           ULONG ulNumClients = 0; 

           //Get the no of clients associated with port
           ulNumClients = ethGetClientsCount(ulInstanceNumber, total_eth_device, output_struct);
           pEthernetPortFull->DynamicInfo.AssocDevicesCount = 0; 

           if( ulNumClients  > 0 )
           {    
               pEthernetPortFull->DynamicInfo.AssocDevicesCount = ulNumClients;

               //Get Mac for associated clients
               for ( i = 1; i <= ulNumClients; i++ )
               {    
                    ethGetClientMacDetails(
                                   ulInstanceNumber,
                                   i,   
                                   total_eth_device,
                                   output_struct,
                                   (char*)pEthernetPortFull->AssocClient[i - 1].MacAddress);
               }    
           }    

           //Release the allocated memory by HAL
           if( NULL != output_struct )
           {    
              free(output_struct);
              output_struct = NULL;
           }    
        } 

   return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthPortGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (!pStats)
    {
        return ANSC_STATUS_FAILURE;
    }

    _ansc_memset(pStats, 0, sizeof(COSA_DML_ETH_STATS));

    PCosaEthInterfaceInfo pEthIf = getIF(ulInstanceNumber);

    if ( !pEthIf )
    {
        return ANSC_STATUS_FAILURE;
    }

    pEthIf->control->getStats(pEthIf, pStats);

    pStats->BroadcastPacketsReceived    -= pEthIf->LastStats.BroadcastPacketsReceived;
    pStats->BroadcastPacketsSent        -= pEthIf->LastStats.BroadcastPacketsSent;
    pStats->BytesReceived               -= pEthIf->LastStats.BytesReceived;
    pStats->BytesSent                   -= pEthIf->LastStats.BytesSent;
    pStats->DiscardPacketsReceived      -= pEthIf->LastStats.DiscardPacketsReceived;
    pStats->DiscardPacketsSent          -= pEthIf->LastStats.DiscardPacketsSent;
    pStats->ErrorsReceived              -= pEthIf->LastStats.ErrorsReceived;
    pStats->ErrorsSent                  -= pEthIf->LastStats.ErrorsSent;
    pStats->MulticastPacketsReceived    -= pEthIf->LastStats.MulticastPacketsReceived;
    pStats->MulticastPacketsSent        -= pEthIf->LastStats.MulticastPacketsSent;
    pStats->PacketsReceived             -= pEthIf->LastStats.PacketsReceived;
    pStats->PacketsSent                 -= pEthIf->LastStats.PacketsSent;
    pStats->UnicastPacketsReceived      -= pEthIf->LastStats.UnicastPacketsReceived;
    pStats->UnicastPacketsSent          -= pEthIf->LastStats.UnicastPacketsSent;
    pStats->UnknownProtoPacketsReceived -= pEthIf->LastStats.UnknownProtoPacketsReceived;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************
       Ethernet Link  -- moved to cosa_ethernet_apis_multilan.c

**********************************************************************/

/**********************************************************************
                            Ethernet VLAN Termination
**********************************************************************/

COSA_DML_ETH_VLAN_TERMINATION_FULL  g_EthernetVlanTermination[MAXINSTANCE];

ULONG                               g_EthernetVlanTerminationNum = 0;

ULONG
CosaDmlEthVlanTerminationGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return g_EthernetVlanTerminationNum;
}

ANSC_STATUS
CosaDmlEthVlanTerminationGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = -1;
    if ( !pEntry )
        return ANSC_STATUS_FAILURE;

    if ( ulIndex < g_EthernetVlanTerminationNum )
    {
        AnscCopyMemory(pEntry, &g_EthernetVlanTermination[ulIndex], sizeof(COSA_DML_ETH_VLAN_TERMINATION_FULL));

        char ifName[256];
        rc = sprintf_s(ifName, sizeof(ifName), "%s.%lu", pEntry->Cfg.EthLinkName, pEntry->Cfg.VLANID);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
        }
        pEntry->DynamicInfo.Status = getIfStatus((PUCHAR)ifName, NULL);
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthVlanTerminationSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    UNREFERENCED_PARAMETER(pAlias);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthVlanTerminationAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = -1;
    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    if ( g_EthernetVlanTerminationNum < MAXINSTANCE )
    {
        AnscCopyMemory(&g_EthernetVlanTermination[g_EthernetVlanTerminationNum], pEntry, sizeof(COSA_DML_ETH_VLAN_TERMINATION_FULL));
        
        g_EthernetVlanTerminationNum++;

        if (pEntry->Cfg.EthLinkName[0] && pEntry->Cfg.VLANID)
        {
            char cmd[256];
#ifdef CORE_NET_LIB
            vlan_create(pEntry->Cfg.EthLinkName, pEntry->Cfg.VLANID);
#else
            v_secure_system("vconfig add %s %lu", pEntry->Cfg.EthLinkName, pEntry->Cfg.VLANID);
#endif /* CORE_NET_LIB */

            if (pEntry->Cfg.bEnabled)
            {
                rc = sprintf_s(cmd, sizeof(cmd), "ip link set %s.%lu up", pEntry->Cfg.EthLinkName, pEntry->Cfg.VLANID);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
            }
            else
            {
                pEntry->DynamicInfo.Status = COSA_DML_IF_STATUS_Down;
            }

            pEntry->DynamicInfo.LastChange = AnscGetTickInSeconds();
        }
        else
        {
            pEntry->DynamicInfo.Status = COSA_DML_IF_STATUS_NotPresent;
        }
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthVlanTerminationDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;
    
    UNREFERENCED_PARAMETER(hContext);

    for ( i = 0; i < g_EthernetVlanTerminationNum; i++ )
    {
        if ( g_EthernetVlanTermination[i].Cfg.InstanceNumber == ulInstanceNumber )
        {
            if (g_EthernetVlanTermination[i].Cfg.EthLinkName[0] && g_EthernetVlanTermination[i].Cfg.VLANID)
            {
#ifdef CORE_NET_LIB
                char buf[IP_ARG_SIZE];
                snprintf(buf, sizeof(buf), "%s.%lu",g_EthernetVlanTermination[i].Cfg.EthLinkName, g_EthernetVlanTermination[i].Cfg.VLANID);
                vlan_delete(buf);
#else
                v_secure_system("vconfig rem %s.%lu", g_EthernetVlanTermination[i].Cfg.EthLinkName, g_EthernetVlanTermination[i].Cfg.VLANID);
#endif /* CORE_NET_LIB */
            }

            for ( j = i; j < g_EthernetVlanTerminationNum; j++ )
            {
                AnscCopyMemory(&g_EthernetVlanTermination[j], &g_EthernetVlanTermination[j+1], sizeof(COSA_DML_ETH_VLAN_TERMINATION_FULL));
            }

            g_EthernetVlanTerminationNum--;

            return ANSC_STATUS_SUCCESS;
        }
    }
    
    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationValidateCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = -1;
    if ( !pCfg )
    {
        return FALSE;
    }

    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry  = getVlanTermination(pCfg->InstanceNumber);

    if (!pEntry || strcmp(pEntry->Cfg.LowerLayers, pCfg->LowerLayers))
    {
        if (pCfg->LowerLayers[0])
        {
            // LowerLayers or VLANID updated, need to come up a new device name
            ULONG                           ulEntryNameLen              = 256;
            CHAR                            ucEntryParamName[256]       = {0};
            CHAR                            ucEntryNameValue[256]       = {0};

            rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%sName", pCfg->LowerLayers);
            if(rc < EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }

            if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                 ( AnscSizeOfString((const char*)ucEntryNameValue) != 0                                        ) )
            {
                rc = strcpy_s(pCfg->EthLinkName,sizeof(pCfg->EthLinkName), ucEntryNameValue);
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
            }
            else
            {
                pCfg->EthLinkName[0] = '\0';
            }
        }
        else
            pCfg->EthLinkName[0] = '\0';
    }

    if (pCfg->bEnabled)
    {
        if (!pCfg->VLANID && pEntry && pCfg->VLANID != pEntry->Cfg.VLANID)
        {
            rc = strcpy_s(pReturnParamName,*puLength, "VLANID");
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
            *puLength = AnscSizeOfString("VLANID");
            return FALSE;
        }
        if (!pCfg->EthLinkName[0])
        {
            if (pEntry && strcmp(pCfg->LowerLayers, pEntry->Cfg.LowerLayers))
            {
                rc = strcpy_s(pReturnParamName,*puLength, "LowerLayers");
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                *puLength = AnscSizeOfString("LowerLayers");
            }
            else
            {
                rc = strcpy_s(pReturnParamName,*puLength, "Enable");
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                *puLength = AnscSizeOfString("Enable");
            }
            return FALSE;
        }
    }

    return TRUE;
}

ANSC_STATUS
CosaDmlEthVlanTerminationSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }

    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry  = getVlanTermination(pCfg->InstanceNumber);

    if (pEntry)
    {
        char ifName[256];
        errno_t rc = -1;
#ifdef CORE_NET_LIB
        char buf[IP_ARG_SIZE];
#endif /* CORE_NET_LIB */
        if (strcmp(pEntry->Cfg.EthLinkName, pCfg->EthLinkName) || pEntry->Cfg.VLANID != pCfg->VLANID)
        {
            if (pEntry->Cfg.EthLinkName[0] && pEntry->Cfg.VLANID)
            {
#ifdef CORE_NET_LIB
                snprintf(buf, sizeof(buf), "%s.%lu", pEntry->Cfg.EthLinkName, pEntry->Cfg.VLANID);
                vlan_delete(buf);
#else
                v_secure_system("vconfig rem %s.%lu", pEntry->Cfg.EthLinkName, pEntry->Cfg.VLANID);
#endif /* CORE_NET_LIB */
            }
            if (pCfg->EthLinkName[0] && pCfg->VLANID)
            {
#ifdef CORE_NET_LIB
                vlan_create(pCfg->EthLinkName, pCfg->VLANID);
#else
                v_secure_system("vconfig add %s %lu", pCfg->EthLinkName, pCfg->VLANID);
#endif /* CORE_NET_LIB */
                if (pCfg->bEnabled)
                {
#ifdef CORE_NET_LIB
                    snprintf(buf, sizeof(buf), "%s.%lu", pCfg->EthLinkName, pCfg->VLANID);
                    interface_up(buf);
#else
                    v_secure_system("ip link set %s.%lu up", pCfg->EthLinkName, pCfg->VLANID);
#endif /* CORE_NET_LIB */
                }
            }
            pEntry->DynamicInfo.LastChange = AnscGetTickInSeconds();
            _ansc_memset(&pEntry->LastStats, 0, sizeof(COSA_DML_ETH_STATS));
        }
        else if (!pEntry->Cfg.bEnabled && pCfg->bEnabled)
        {
#ifdef CORE_NET_LIB
            snprintf(buf, sizeof(buf), "%s.%lu", pCfg->EthLinkName, pCfg->VLANID);
            interface_up(buf);
#else
            v_secure_system("ip link set %s.%lu up", pCfg->EthLinkName, pCfg->VLANID);
#endif /* CORE_NET_LIB */

            pEntry->DynamicInfo.LastChange = AnscGetTickInSeconds();

            rc = sprintf_s(ifName, sizeof(ifName), "%s.%lu", pCfg->EthLinkName, pCfg->VLANID);
            if(rc < EOK)
            {
                ERR_CHK(rc);
                return ANSC_STATUS_FAILURE;
            }
            getIfStats2((PUCHAR)ifName, &pEntry->LastStats);
        }
        else if (pEntry->Cfg.bEnabled && !pCfg->bEnabled)
        {
#ifdef CORE_NET_LIB

            snprintf(buf, sizeof(buf), "%s.%lu", pCfg->EthLinkName, pCfg->VLANID);
            interface_down(buf);
#else
            v_secure_system("ip link set %s.%lu down", pCfg->EthLinkName, pCfg->VLANID);
#endif /* CORE_NET_LIB */

            pEntry->DynamicInfo.LastChange = AnscGetTickInSeconds();
        }

        AnscCopyMemory(&pEntry->Cfg, pCfg, sizeof(COSA_DML_ETH_VLAN_TERMINATION_CFG));

        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }

    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry  = getVlanTermination(pCfg->InstanceNumber);

    if (pEntry)
    {
        AnscCopyMemory(pCfg, &pEntry->Cfg, sizeof(COSA_DML_ETH_VLAN_TERMINATION_CFG));

        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_VLAN_TERMINATION_DINFO pInfo
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if ( !pInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry  = getVlanTermination(ulInstanceNumber);

    if (pEntry)
    {
        if (!(pEntry->Cfg.EthLinkName[0] && pEntry->Cfg.VLANID))
        {
           pEntry->DynamicInfo.Status = COSA_DML_IF_STATUS_NotPresent;
        }
        else if (!pEntry->Cfg.bEnabled)
        {
           pEntry->DynamicInfo.Status = COSA_DML_IF_STATUS_Down;
        }
        else
        {
           char ifName[256];
           errno_t rc = -1;
           rc = sprintf_s(ifName, sizeof(ifName), "%s.%lu", pEntry->Cfg.EthLinkName, pEntry->Cfg.VLANID);
           if(rc < EOK)
           {
               ERR_CHK(rc);
               return ANSC_STATUS_FAILURE;
           }
           pEntry->DynamicInfo.Status = getIfStatus((PUCHAR)ifName, NULL);
        }

        AnscCopyMemory(pInfo, &pEntry->DynamicInfo , sizeof(COSA_DML_ETH_VLAN_TERMINATION_DINFO));

        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = -1;
    if (!pStats)
        return ANSC_STATUS_FAILURE;

    _ansc_memset(pStats, 0, sizeof(COSA_DML_ETH_STATS));

    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry  = getVlanTermination(ulInstanceNumber);

    if (!pEntry)
    {
        return ANSC_STATUS_FAILURE;
    }

    if (pEntry->Cfg.EthLinkName[0] && pEntry->Cfg.VLANID && pEntry->Cfg.bEnabled)
    {
       char ifName[256];
       rc = sprintf_s(ifName, sizeof(ifName), "%s.%lu", pEntry->Cfg.EthLinkName, pEntry->Cfg.VLANID);
       if(rc < EOK)
       {
           ERR_CHK(rc);
           return ANSC_STATUS_FAILURE;
       }
       pEntry->DynamicInfo.Status = getIfStatus((PUCHAR)ifName, NULL);

       if (pEntry->DynamicInfo.Status == COSA_DML_IF_STATUS_Up)
       {
           getIfStats2((PUCHAR)ifName, pStats);
    
           pStats->BroadcastPacketsReceived    -= pEntry->LastStats.BroadcastPacketsReceived;
           pStats->BroadcastPacketsSent        -= pEntry->LastStats.BroadcastPacketsSent;
           pStats->BytesReceived               -= pEntry->LastStats.BytesReceived;
           pStats->BytesSent                   -= pEntry->LastStats.BytesSent;
           pStats->DiscardPacketsReceived      -= pEntry->LastStats.DiscardPacketsReceived;
           pStats->DiscardPacketsSent          -= pEntry->LastStats.DiscardPacketsSent;
           pStats->ErrorsReceived              -= pEntry->LastStats.ErrorsReceived;
           pStats->ErrorsSent                  -= pEntry->LastStats.ErrorsSent;
           pStats->MulticastPacketsReceived    -= pEntry->LastStats.MulticastPacketsReceived;
           pStats->MulticastPacketsSent        -= pEntry->LastStats.MulticastPacketsSent;
           pStats->PacketsReceived             -= pEntry->LastStats.PacketsReceived;
           pStats->PacketsSent                 -= pEntry->LastStats.PacketsSent;
           pStats->UnicastPacketsReceived      -= pEntry->LastStats.UnicastPacketsReceived;
           pStats->UnicastPacketsSent          -= pEntry->LastStats.UnicastPacketsSent;
           pStats->UnknownProtoPacketsReceived -= pEntry->LastStats.UnknownProtoPacketsReceived;
       }
    }

    return ANSC_STATUS_SUCCESS;
}

#if defined(_HUB4_PRODUCT_REQ_) || defined(_RDKB_GLOBAL_PRODUCT_REQ_)
ANSC_STATUS
CosaDmlEthLinkGetWanUpDownTime(char* pcWanUpDownTime, int nTimeLength)
{
    int ret 			  = 0;
    static int sysevent_fd 	  = -1;
    static token_t sysevent_token = 0;
    if (0 > sysevent_fd)
    {
        if ((sysevent_fd = sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "wan_up_down_time", &sysevent_token)) < 0)
        {
            fprintf(stderr, "%s: fail to open sysevent\n", __FUNCTION__);
            return -1;
        }
    }
    /*Get the time in ticks at which the Last Wan Up/Down happened*/
    if(sysevent_get(sysevent_fd, sysevent_token, "last_wan_up_down_time", pcWanUpDownTime, nTimeLength) != 0)
    {
	ret = -1;
    }
return ret;
}
#endif

/**********************************************************************
                        HELPER ROUTINES
**********************************************************************/


#if defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)
int puma6_getSwitchCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg)
{
    CCSP_HAL_ETHSW_PORT         port        = *((PCCSP_HAL_ETHSW_PORT)eth->hwid);
    INT                         status;
    CCSP_HAL_ETHSW_ADMIN_STATUS AdminStatus;
    CCSP_HAL_ETHSW_LINK_RATE    LinkRate;
    CCSP_HAL_ETHSW_DUPLEX_MODE  DuplexMode;

    /* By default, port is enabled */
    pcfg->bEnabled = TRUE;

    status = CcspHalEthSwGetPortAdminStatus(port, &AdminStatus);

    if ( status == RETURN_OK )
    {
        switch ( AdminStatus )
        {
            case CCSP_HAL_ETHSW_AdminUp:
            {
                pcfg->bEnabled = TRUE;
                break;
            }
            case CCSP_HAL_ETHSW_AdminDown:
            {
                pcfg->bEnabled = FALSE;
                break;
            }
            default:
            {
                pcfg->bEnabled = TRUE;
                break;
            }
        }        
    }

    status = CcspHalEthSwGetPortCfg(port, &LinkRate, &DuplexMode);

    if ( status == RETURN_OK )
    {
        switch ( LinkRate )
        {
            case CCSP_HAL_ETHSW_LINK_10Mbps:
            {
                pcfg->MaxBitRate = 10;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_100Mbps:
            {
                pcfg->MaxBitRate = 100;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_1Gbps:
            {
                pcfg->MaxBitRate = 1000;
                break;
            }
#ifdef _2_5G_ETHERNET_SUPPORT_
            case CCSP_HAL_ETHSW_LINK_2_5Gbps:
            {
                pcfg->MaxBitRate = 2500;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_5Gbps:
            {
                pcfg->MaxBitRate = 5000;
                break;
            }
#endif // _2_5G_ETHERNET_SUPPORT_
            case CCSP_HAL_ETHSW_LINK_10Gbps:
            {
                pcfg->MaxBitRate = 10000;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_Auto:
            {
                pcfg->MaxBitRate = 0;
                break;
            }
            default:
            {
                pcfg->MaxBitRate = 0;
                break;
            }
        }

        switch ( DuplexMode )
        {
            case CCSP_HAL_ETHSW_DUPLEX_Auto:
            {
                pcfg->DuplexMode = COSA_DML_ETH_DUPLEX_Auto; 
                break;
            }
            case CCSP_HAL_ETHSW_DUPLEX_Half:
            {
                pcfg->DuplexMode = COSA_DML_ETH_DUPLEX_Half; 
                break;
            }
            case CCSP_HAL_ETHSW_DUPLEX_Full:
            {
                pcfg->DuplexMode = COSA_DML_ETH_DUPLEX_Full; 
                break;
            }
            default:
            {
                pcfg->DuplexMode = COSA_DML_ETH_DUPLEX_Auto; 
                break;
            }
        }
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS; 
     
}
int puma6_setSwitchCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg) {
    CCSP_HAL_ETHSW_PORT         port        = *((PCCSP_HAL_ETHSW_PORT)eth->hwid);
	CCSP_HAL_ETHSW_ADMIN_STATUS AdminStatus;
	if(pcfg->bEnabled == TRUE)
	{
		AdminStatus = CCSP_HAL_ETHSW_AdminUp;
	}
	else
	{	
		AdminStatus = CCSP_HAL_ETHSW_AdminDown;
	}
	CcspHalEthSwSetPortAdminStatus(port,AdminStatus);
    return ANSC_STATUS_SUCCESS; 
}
int puma6_getSwitchDInfo(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_DINFO pDinfo){
    CCSP_HAL_ETHSW_PORT         port        = *((PCCSP_HAL_ETHSW_PORT)eth->hwid);
    INT                         status;
    CCSP_HAL_ETHSW_LINK_RATE    LinkRate;
    CCSP_HAL_ETHSW_DUPLEX_MODE  DuplexMode;
    CCSP_HAL_ETHSW_LINK_STATUS  LinkStatus;

    pDinfo->Status         = COSA_DML_IF_STATUS_Down;
    pDinfo->CurrentBitRate = 0;

    status = CcspHalEthSwGetPortStatus(port, &LinkRate, &DuplexMode, &LinkStatus);

    if ( status == RETURN_OK )
    {
        switch ( LinkStatus )
        {
            case CCSP_HAL_ETHSW_LINK_Up:
            {
                pDinfo->Status = COSA_DML_IF_STATUS_Up;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_Down:
            {
                pDinfo->Status = COSA_DML_IF_STATUS_Down;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_Disconnected:
            {
                pDinfo->Status = COSA_DML_IF_STATUS_Down;
                break;
            }
            default:
            {
                pDinfo->Status = COSA_DML_IF_STATUS_Down;
                break;
            }
        }

        switch ( LinkRate )
        {
            case CCSP_HAL_ETHSW_LINK_10Mbps:
            {
                pDinfo->CurrentBitRate = 10;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_100Mbps:
            {
                pDinfo->CurrentBitRate = 100;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_1Gbps:
            {
                pDinfo->CurrentBitRate = 1000;
                break;
            }
#ifdef _2_5G_ETHERNET_SUPPORT_
            case CCSP_HAL_ETHSW_LINK_2_5Gbps:
            {
                pDinfo->CurrentBitRate = 2500;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_5Gbps:
            {
                pDinfo->CurrentBitRate = 5000;
                break;
            }
#endif // _2_5G_ETHERNET_SUPPORT_
            case CCSP_HAL_ETHSW_LINK_10Gbps:
            {
                pDinfo->CurrentBitRate = 10000;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_Auto:
            {
                pDinfo->CurrentBitRate = 0;
                break;
            }
            default:
            {
                pDinfo->CurrentBitRate = 0;
                break;
            }
        }
    }
    else
    {
        return ANSC_STATUS_FAILURE; 
    }

    return ANSC_STATUS_SUCCESS; 
}

int puma6_getSwitchStats(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_STATS pStats){
#if defined(ETH_STATS_ENABLED)
    CCSP_HAL_ETHSW_PORT   port  = *((PCCSP_HAL_ETHSW_PORT)eth->hwid);
    CcspHalEthSwGetEthPortStats(port, (PCCSP_HAL_ETH_STATS)pStats);
#else
    UNREFERENCED_PARAMETER(eth);
    UNREFERENCED_PARAMETER(pStats);
#endif
    return ANSC_STATUS_SUCCESS;
}

static int getIfCfg(PCosaEthInterfaceInfo pEthIf, PCOSA_DML_ETH_PORT_CFG pCfg)
{    
    if ( getIfStatus( (PUCHAR)pEthIf->sInfo->Name, NULL ) == COSA_DML_IF_STATUS_Up )
    {
        pCfg->bEnabled = TRUE;
    }
    else
    {
        pCfg->bEnabled = FALSE;
    }

    pCfg->DuplexMode = COSA_DML_ETH_DUPLEX_Auto;
    pCfg->MaxBitRate = 1000;

    return 0;
}

static int setIfCfg(PCosaEthInterfaceInfo pEthIf, PCOSA_DML_ETH_PORT_CFG pCfg)
{
    struct ifreq ifr;
	COSA_DML_IF_STATUS enifStatus = COSA_DML_IF_STATUS_Unknown;

	enifStatus = getIfStatus((PUCHAR)pEthIf->sInfo->Name, &ifr);

    if ( ( enifStatus == COSA_DML_IF_STATUS_Unknown ) ||
         ( enifStatus == COSA_DML_IF_STATUS_NotPresent ) ||
         ( enifStatus == COSA_DML_IF_STATUS_Error ) )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    if ( pCfg->bEnabled && !(ifr.ifr_flags & IFF_UP) )
    {
        ifr.ifr_flags |= IFF_UP;

        if ( setIfStatus(&ifr) )
        {
            return -1;
        }  

        /*
         *  Do not trigger a respective wan-restart or multinet events for now
         *      pEthIf->sInfo->bUpstream == TRUE -> sysevent set wan-restart
         */
    }
    else if ( !(pCfg->bEnabled) && (ifr.ifr_flags & IFF_UP) )
    {
        ifr.ifr_flags &= ~IFF_UP;

        if ( setIfStatus(&ifr) )
        {
            return -1;
        }
    }

    return 0;
}

static int getIfStats(PCosaEthInterfaceInfo pEthIf, PCOSA_DML_ETH_STATS pStats)
{
    return getIfStats2((PUCHAR)pEthIf->sInfo->Name, pStats);
}

static int getIfDInfo(PCosaEthInterfaceInfo pEthIf, PCOSA_DML_ETH_PORT_DINFO pInfo)
{
    pInfo->Status = getIfStatus((PUCHAR)pEthIf->sInfo->Name, NULL);
    
    return 0;
}
#endif

static int getIfStats2(const PUCHAR pName, PCOSA_DML_ETH_STATS pStats)
{
#if 1
    FILE *fp;
    char buf[512];
    char *tok, *delim = ": \t\r\n", *sp, *ptr;
    int idx;

#if defined(_RDKB_GLOBAL_PRODUCT_REQ_)
#define NET_EXT_STATS_FILE "/proc/net/dev_extstats"
#define NET_STATS_FILE "/proc/net/dev" 

    // check if dev_extstats exists in platform
    fp = fopen(NET_EXT_STATS_FILE, "r");

    if(fp == NULL)
    {
        fp = fopen(NET_STATS_FILE, "r");
        // both files are not present . Return error
        if(fp == NULL)
            return -1;
    }
#else
#ifdef _HUB4_PRODUCT_REQ_
    if ((fp = fopen("/proc/net/dev_extstats", "rb")) == NULL)
#else
    if ((fp = fopen("/proc/net/dev", "rb")) == NULL)
#endif /* _HUB4_PRODUCT_REQ_ */
        return -1;
#endif /* _RDKB_GLOBAL_PRODUCT_REQ_ */

    /* skip head line */
    if (fgets(buf, sizeof(buf), fp) == NULL) {
        fclose(fp);
        return -1;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (strstr(buf, (char *)pName) == NULL)
            continue;

        for (idx = 1, ptr = buf;
                (tok = strtok_r(ptr, delim, &sp)) != NULL; idx++) {
            ptr = NULL;
            switch (idx) {
            case 2:
                pStats->BytesReceived = (ULONG)atol(tok);
                break;
            case 3:
                pStats->PacketsReceived = (ULONG)atol(tok);
                break;
            case 4:
                pStats->ErrorsReceived = (ULONG)atol(tok);
                break;
            case 5:
                pStats->DiscardPacketsReceived = (ULONG)atol(tok);
                break;
            case 10:
                pStats->BytesSent = (ULONG)atol(tok);
                break;
            case 11:
                pStats->PacketsSent = (ULONG)atol(tok);
                break;
            case 12:
                pStats->ErrorsSent = (ULONG)atol(tok);
                break;
            case 13:
                pStats->DiscardPacketsSent = (ULONG)atol(tok);
                break;
#if defined(_HUB4_PRODUCT_REQ_) || defined(_RDKB_GLOBAL_PRODUCT_REQ_)
            case 9:
                pStats->MulticastPacketsReceived = (ULONG)atol(tok);
                break;
            case 18:
                pStats->MulticastPacketsSent = (ULONG)atol(tok);
                break;
            case 21:
                pStats->UnicastPacketsReceived = (ULONG)atol(tok);
                break;
            case 22:
                pStats->UnicastPacketsSent = (ULONG)atol(tok);
                break;
            case 23:
                pStats->BroadcastPacketsReceived = (ULONG)atol(tok);
                break;
            case 24:
                pStats->BroadcastPacketsSent = (ULONG)atol(tok);
                break;
            case 25:
                pStats->UnknownProtoPacketsReceived = (ULONG)atol(tok);
                break;
#endif /* _HUB4_PRODUCT_REQ_ || _RDKB_GLOBAL_PRODUCT_REQ_ */
            default:
                break;
            }
        }
    }

    fclose(fp);
    return 0;

#else
    UCHAR strCmd[512] = {0};
    UCHAR strBuf[128] = {0};
    FILE  *pFile      = NULL;
    errno_t rc        = -1;
    
    rc = sprintf_s
        (
            strCmd,
            sizeof(strCmd),
            "cat /proc/net/dev | grep %s | tr : \" \" | awk '{print $2}'",
            pName
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return -1;
    }
    pFile = popen(strCmd, "r");
    fread(strBuf, sizeof(char), sizeof(strBuf), pFile);
    pStats->BytesReceived = _ansc_atoi(strBuf);
    pclose(pFile);

    rc = sprintf_s
        (
            strCmd,
            sizeof(strCmd),
            "cat /proc/net/dev | grep %s | tr : \" \" | awk '{print $3}'",
            pName
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return -1;
    }
    pFile = popen(strCmd, "r");
    fread(strBuf, sizeof(char), sizeof(strBuf), pFile);
    pStats->PacketsReceived= _ansc_atoi(strBuf);
    pclose(pFile);

    rc = sprintf_s
        (
            strCmd,
            sizeof(strCmd),
            "cat /proc/net/dev | grep %s | tr : \" \" | awk '{print $4}'",
            pName
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return -1;
    }
    pFile = popen(strCmd, "r");
    fread(strBuf, sizeof(char), sizeof(strBuf), pFile);
    pStats->ErrorsReceived= _ansc_atoi(strBuf);
    pclose(pFile);

    rc = sprintf_s
        (
            strCmd,
            sizeof(strCmd),
            "cat /proc/net/dev | grep %s | tr : \" \" | awk '{print $5}'",
            pName
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return -1;
    }
    pFile = popen(strCmd, "r");
    fread(strBuf, sizeof(char), sizeof(strBuf), pFile);
    pStats->DiscardPacketsReceived = _ansc_atoi(strBuf);
    pclose(pFile);

    rc = sprintf_s
        (
            strCmd,
            sizeof(strCmd),
            "cat /proc/net/dev | grep %s | tr : \" \" | awk '{print $10}'",
            pName
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return -1;
    }
    pFile = popen(strCmd, "r");
    fread(strBuf, sizeof(char), sizeof(strBuf), pFile);
    pStats->BytesSent= _ansc_atoi(strBuf);
    pclose(pFile);

    rc = sprintf_s
        (
            strCmd,
            sizeof(strCmd),
            "cat /proc/net/dev | grep %s | tr : \" \" | awk '{print $11}'",
            pName
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return -1;
    }
    pFile = popen(strCmd, "r");
    fread(strBuf, sizeof(char), sizeof(strBuf), pFile);
    pStats->PacketsSent = _ansc_atoi(strBuf);
    pclose(pFile);

    rc = sprintf_s
        (
            strCmd,
            sizeof(strCmd),
            "cat /proc/net/dev | grep %s | tr : \" \" | awk '{print $12}'",
            pName
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return -1;
    }
    pFile = popen(strCmd, "r");
    fread(strBuf, sizeof(char), sizeof(strBuf), pFile);
    pStats->ErrorsSent= _ansc_atoi(strBuf);
    pclose(pFile);

    rc = sprintf_s
        (
            strCmd,
            sizeof(strCmd),
            "cat /proc/net/dev | grep %s | tr : \" \" | awk '{print $13}'",
            pName
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return -1;
    }
    pFile = popen(strCmd, "r");
    fread(strBuf, sizeof(char), sizeof(strBuf), pFile);
    pStats->DiscardPacketsSent= _ansc_atoi(strBuf);
    pclose(pFile);

    return 0;
#endif
}

static int setIfStatus(struct ifreq *pIfr)
{
    int skfd;
    
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    /* CID: 73861 Argument cannot be negative*/
    if(skfd == -1)
       return -1;

    if (ioctl(skfd, SIOCSIFFLAGS, pIfr) < 0) {
        CcspTraceWarning(("cosa_ethernet_apis.c - setIfStatus: Set interface %s error...\n", pIfr->ifr_name));
        close(skfd);
        return -1;
    }
    close(skfd);

    return 0;
}

PCosaEthInterfaceInfo getIF(const ULONG instanceNumber) {
    ULONG i;
    for (i = 0; i < g_EthernetIntNum; ++i) {
        if (g_EthEntries[i].instanceNumber == instanceNumber) {
            break;
        }
    }

    if (i == g_EthernetIntNum) {
        return NULL;
    }
    return g_EthEntries + i;
}

PCOSA_DML_ETH_VLAN_TERMINATION_FULL getVlanTermination(const ULONG ulInstanceNumber) {
    ULONG i;
    for ( i = 0; i < g_EthernetVlanTerminationNum; i++ )
    {
        if ( ulInstanceNumber == g_EthernetVlanTermination[i].Cfg.InstanceNumber )
        {
            break;
        }
    }

    if (i == g_EthernetVlanTerminationNum ) {
        return NULL;
    }

    return g_EthernetVlanTermination + i;
}
static int saveID(char* ifName, char* pAlias, ULONG ulInstanceNumber) {
#ifdef FEATURE_RDKB_WAN_MANAGER
    CcspTraceInfo(("Ignoring saveID for ifName: %s, pAlias: %s, ulInstanceNumber: %lu\n", ifName, pAlias, ulInstanceNumber));
    return -1;
#else
    UtopiaContext utctx;
    char idStr[COSA_DML_IF_NAME_LENGTH+10] = {0};
    errno_t rc = -1;
    /* CID: 58910 Unchecked return value*/
    if(!Utopia_Init(&utctx))
       return -1;

    rc = sprintf_s(idStr, sizeof(idStr), "%s,%lu", pAlias,ulInstanceNumber);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    Utopia_RawSet(&utctx,COSA_ETH_INT_ID_SYSCFG_NAMESPACE,ifName,idStr);

    Utopia_Free(&utctx,TRUE);

    return 0;
#endif
}

/*static int saveLinkID(char* ifName, char* pAlias, ULONG ulInstanceNumber) {
    UtopiaContext utctx;
    char idStr[COSA_DML_IF_NAME_LENGTH+10] = {0};
    //CID: 65042 Unchecked return value
    if(!Utopia_Init(&utctx))
        return -1;

    sprintf(idStr,"%s,%u", pAlias,ulInstanceNumber);
    Utopia_RawSet(&utctx,COSA_ETH_LINK_ID_SYSCFG_NAMESPACE,ifName,idStr);

    Utopia_Free(&utctx,TRUE);

    return 0;
}*/

static int loadID(char* ifName, char* pAlias, ULONG* ulInstanceNumber) {
    UtopiaContext utctx;
    char idStr[COSA_DML_IF_NAME_LENGTH+10] = {0};
    char* instNumString;
    int rv;
    errno_t rc = -1;
    /*CID: 70909 Unchecked return value*/
    if(!Utopia_Init(&utctx))
        return -1;

    rv =Utopia_RawGet(&utctx, COSA_ETH_INT_ID_SYSCFG_NAMESPACE, ifName, idStr, sizeof(idStr));
    if (rv == -1 || idStr[0] == '\0') {
        Utopia_Free(&utctx, 0);
        return -1;
    }
    instNumString=idStr + AnscSizeOfToken(idStr, ",", sizeof(idStr))+1;
    *(instNumString-1)='\0';

    rc = strcpy_s(pAlias, COSA_DML_IF_NAME_LENGTH, idStr);   // Here pAlias having the size is COSA_DML_IF_NAME_LENGTH(512 bytes) from calling funtion
    if(rc != EOK)
    {
        ERR_CHK(rc);
        return -1;
    }

    *ulInstanceNumber = AnscGetStringUlong(instNumString);
    Utopia_Free(&utctx, 0);

    return 0;
}

/*static int loadLinkID(char* ifName, char* pAlias, ULONG* ulInstanceNumber) {
    UtopiaContext utctx;
    char idStr[COSA_DML_IF_NAME_LENGTH+10] = {0};
    char* instNumString;
    int rv;
    //CID: 61200 Unchecked return value
    if(!Utopia_Init(&utctx))
       return -1;

    rv =Utopia_RawGet(&utctx, COSA_ETH_LINK_ID_SYSCFG_NAMESPACE, ifName, idStr, sizeof(idStr));
    if (rv == -1 || idStr[0] == '\0') {
        Utopia_Free(&utctx, 0);
        return -1;
    }
    instNumString=idStr + AnscSizeOfToken(idStr, ",", sizeof(idStr))+1;
    *(instNumString-1)='\0';

    AnscCopyString(pAlias, idStr);
    *ulInstanceNumber = AnscGetStringUlong(instNumString);
    Utopia_Free(&utctx, 0);

    return 0;
}*/

#endif

