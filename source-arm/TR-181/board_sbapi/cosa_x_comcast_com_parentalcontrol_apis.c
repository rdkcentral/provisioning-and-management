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
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include "cosa_x_comcast_com_parentalcontrol_apis.h"
#include "syscfg/syscfg.h"
#include <utctx/utctx.h>
#include <utapi/utapi.h>
#include <utapi/utapi_util.h>
#include <utapi/utapi_parental_control.h>
#include "cosa_drg_common.h"
#include "autoconf.h"
#include "safec_lib_common.h"
#include "secure_wrapper.h"

#if defined(_COSA_SIM_)

ANSC_STATUS
CosaDmlParentalControlInit(ANSC_HANDLE hDml, PANSC_HANDLE phContext)
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedSites
 */
static COSA_DML_MANAGEDSITES g_ManagedSites = {
    .Enable             = FALSE,
};

ANSC_STATUS
CosaDmlMngSites_GetConf(COSA_DML_MANAGEDSITES *conf)
{
    memcpy(conf, &g_ManagedSites, sizeof(COSA_DML_MANAGEDSITES));
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMngSites_SetConf(COSA_DML_MANAGEDSITES *conf)
{
    memcpy(&g_ManagedSites, conf, sizeof(COSA_DML_MANAGEDSITES));
    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices
 */
static COSA_DML_MANAGED_SERVS g_ManagedServs = {
    .Enable             = FALSE,
};

ANSC_STATUS
CosaDmlMngServs_GetConf(COSA_DML_MANAGED_SERVS *conf)
{
    memcpy(conf, &g_ManagedServs, sizeof(COSA_DML_MANAGED_SERVS));
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMngServs_SetConf(COSA_DML_MANAGED_SERVS *conf)
{
    memcpy(&g_ManagedServs, conf, sizeof(COSA_DML_MANAGED_SERVS));
    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedDevices
 */
static COSA_DML_MANAGED_DEVS g_ManagedDevs = {
    .Enable             = FALSE,
    .AllowAll           = FALSE,
};

ANSC_STATUS
CosaDmlMngDevs_GetConf(COSA_DML_MANAGED_DEVS *conf)
{
    memcpy(conf, &g_ManagedDevs, sizeof(COSA_DML_MANAGED_DEVS));
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMngDevs_SetConf(COSA_DML_MANAGED_DEVS *conf)
{
    memcpy(&g_ManagedDevs, conf, sizeof(COSA_DML_MANAGED_DEVS));
    return ANSC_STATUS_SUCCESS;
}

/*
 * Blocked URL
 */

#define  MAX_URL_ENTRY      4

static int g_NrBlkURL   = MAX_URL_ENTRY / 2;

static COSA_DML_BLOCKEDURL g_BlockedURLs[MAX_URL_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-BlockedSite-1",
        .BlockMethod    = BLOCK_METHOD_URL,
        .Site           = "http://baidu.com",
        .AlwaysBlock    = FALSE,
        .StartTime      = "19:00",
        .EndTime        = "22:00",
        .BlockDays      = "Sat,Sun",
    },
    {
        .InstanceNumber = 2,
        .Alias          = "cpe-BlockedSite-2",
        .BlockMethod    = BLOCK_METHOD_KEYWORD,
        .Site           = "keyword",
        .AlwaysBlock    = FALSE,
        .StartTime      = "10:00",
        .EndTime        = "12:00",
        .BlockDays      = "Mon,Wed,Fri",
    },
};

static int
BlkURL_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrBlkURL; i++)
        if (g_BlockedURLs[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlBlkURL_GetNumberOfEntries(void)
{
    return g_NrBlkURL;
}

ANSC_STATUS
CosaDmlBlkURL_GetEntryByIndex(ULONG index, COSA_DML_BLOCKEDURL *pEntry)
{
    if (index >= g_NrBlkURL)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_BlockedURLs[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrBlkURL)
        return ANSC_STATUS_FAILURE;

    g_BlockedURLs[index].InstanceNumber = ins;
    snprintf(g_BlockedURLs[index].Alias, sizeof(g_BlockedURLs[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_AddEntry(COSA_DML_BLOCKEDURL *pEntry)
{
    if (g_NrBlkURL >= MAX_URL_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_BlockedURLs[g_NrBlkURL++] = *pEntry;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_DelEntry(ULONG ins)
{
    int i;

    if ((i = BlkURL_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    memmove(&g_BlockedURLs[i], &g_BlockedURLs[i+1], 
            (g_NrBlkURL - i - 1) * sizeof(COSA_DML_BLOCKEDURL));
    g_NrBlkURL--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_GetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry)
{
    int i;

    if ((i = BlkURL_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_BlockedURLs[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlBlkURL_SetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry)
{
    int i;

    if ((i = BlkURL_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    g_BlockedURLs[i] = *pEntry;
    g_BlockedURLs[i].InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}

/*
 * TrustedUser
 */

#define  MAX_TRUSTEDUSER_ENTRY      4

static int g_NrTrustedUser   = MAX_TRUSTEDUSER_ENTRY / 2;

static COSA_DML_TRUSTEDUSER g_TrustedUsers[MAX_TRUSTEDUSER_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-TrustedUser-1",
        .HostDescription = "Host-1",
        .IPAddressType  = IPADDR_IPV4,
        .IPAddress      = "192.168.0.1",
    },
    {
        .InstanceNumber = 2,
        .Alias          = "cpe-TrustedUser-2",
        .HostDescription = "Host-2",
        .IPAddressType  = IPADDR_IPV6,
        .IPAddress      = "2012:CAFE::1",
    },
};

static int
TrustedUser_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrTrustedUser; i++)
        if (g_TrustedUsers[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlTrustedUser_GetNumberOfEntries(void)
{
    return g_NrTrustedUser;
}

ANSC_STATUS
CosaDmlTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_TRUSTEDUSER *pEntry)
{
    if (index >= g_NrTrustedUser)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_TrustedUsers[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrTrustedUser)
        return ANSC_STATUS_FAILURE;

    g_TrustedUsers[index].InstanceNumber = ins;
    snprintf(g_TrustedUsers[index].Alias, sizeof(g_TrustedUsers[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_AddEntry(COSA_DML_TRUSTEDUSER *pEntry)
{
    if (g_NrTrustedUser >= MAX_TRUSTEDUSER_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_TrustedUsers[g_NrTrustedUser++] = *pEntry;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_DelEntry(ULONG ins)
{
    int i;

    if ((i = TrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    memmove(&g_TrustedUsers[i], &g_TrustedUsers[i+1], 
            (g_NrTrustedUser - i - 1) * sizeof(COSA_DML_TRUSTEDUSER));
    g_NrTrustedUser--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_GetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry)
{
    int i;

    if ((i = TrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_TrustedUsers[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlTrustedUser_SetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry)
{
    int i;

    if ((i = TrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    g_TrustedUsers[i] = *pEntry;
    g_TrustedUsers[i].InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices.Service
 */

#define  MAX_MSSERV_ENTRY      4

static int g_NrMSServs   = MAX_MSSERV_ENTRY / 2;

static COSA_DML_MS_SERV g_MSServs[MAX_MSSERV_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-MSService-1",
        .Description    = "Service-1",
        .Protocol       = PROTO_TCP,
        .StartPort      = 100,
        .EndPort        = 1024,
        .AlwaysBlock   = FALSE,
        .StartTime      = "19:00",
        .EndTime        = "22:00",
        .BlockDays      = "Sat,Sun",
    },
    {
        .InstanceNumber = 2,
        .Alias          = "cpe-MSService-2",
        .Description    = "Service-2",
        .Protocol       = PROTO_BOTH,
        .StartPort      = 200,
        .EndPort        = 2024,
        .AlwaysBlock   = FALSE,
        .StartTime      = "20:00",
        .EndTime        = "24:00",
        .BlockDays      = "Mon,Tue",
    },
};

static int
MSServ_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrMSServs; i++)
        if (g_MSServs[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlMSServ_GetNumberOfEntries(void)
{
    return g_NrMSServs;
}

ANSC_STATUS
CosaDmlMSServ_GetEntryByIndex(ULONG index, COSA_DML_MS_SERV *pEntry)
{
    if (index >= g_NrMSServs)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MSServs[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrMSServs)
        return ANSC_STATUS_FAILURE;

    g_MSServs[index].InstanceNumber = ins;
    snprintf(g_MSServs[index].Alias, sizeof(g_MSServs[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_AddEntry(COSA_DML_MS_SERV *pEntry)
{
    if (g_NrMSServs >= MAX_MSSERV_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_MSServs[g_NrMSServs++] = *pEntry;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_DelEntry(ULONG ins)
{
    int i;

    if ((i = MSServ_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    memmove(&g_MSServs[i], &g_MSServs[i+1], 
            (g_NrMSServs - i - 1) * sizeof(COSA_DML_MS_SERV));
    g_NrMSServs--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_GetConf(ULONG ins, COSA_DML_MS_SERV *pEntry)
{
    int i;

    if ((i = MSServ_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MSServs[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMSServ_SetConf(ULONG ins, COSA_DML_MS_SERV *pEntry)
{
    int i;

    if ((i = MSServ_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    g_MSServs[i] = *pEntry;
    g_MSServs[i].InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedServices.TrustedUser
 */

#define  MAX_MSTRUSTEDUSER_ENTRY      4

static int g_NrMSTrustedUsers   = MAX_MSTRUSTEDUSER_ENTRY / 2;

static COSA_DML_MS_TRUSTEDUSER g_MSTrustedUsers[MAX_MSTRUSTEDUSER_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-MSTrustedUser-1",
        .HostDescription = "Dummy",
        .IPAddressType  = IPADDR_IPV6,
        .IPAddress      = "2012:cafe::3",
        .Trusted        = FALSE,
    },
    {
        .InstanceNumber = 2,
        .Alias          = "cpe-MSTrustedUser-2",
        .HostDescription = "Dummy",
        .IPAddressType  = IPADDR_IPV4,
        .IPAddress      = "192.168.0.4",
        .Trusted        = TRUE,
    },
};

static int
MSTrustedUser_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrMSTrustedUsers; i++)
        if (g_MSTrustedUsers[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlMSTrustedUser_GetNumberOfEntries(void)
{
    return g_NrMSTrustedUsers;
}

ANSC_STATUS
CosaDmlMSTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    if (index >= g_NrMSTrustedUsers)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MSTrustedUsers[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrMSTrustedUsers)
        return ANSC_STATUS_FAILURE;

    g_MSTrustedUsers[index].InstanceNumber = ins;
    snprintf(g_MSTrustedUsers[index].Alias, sizeof(g_MSTrustedUsers[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_AddEntry(COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    if (g_NrMSTrustedUsers >= MAX_MSTRUSTEDUSER_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_MSTrustedUsers[g_NrMSTrustedUsers++] = *pEntry;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_DelEntry(ULONG ins)
{
    int i;

    if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    memmove(&g_MSTrustedUsers[i], &g_MSTrustedUsers[i+1], 
            (g_NrMSTrustedUsers - i - 1) * sizeof(COSA_DML_MS_TRUSTEDUSER));
    g_NrMSTrustedUsers--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_GetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    int i;

    if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MSTrustedUsers[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMSTrustedUser_SetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    int i;

    if ((i = MSTrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    g_MSTrustedUsers[i] = *pEntry;
    g_MSTrustedUsers[i].InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedDevices.Device
 */

#define  MAX_MDDEV_ENTRY      4

static int g_NrMDDevs   = MAX_MDDEV_ENTRY / 2;

static COSA_DML_MD_DEV g_MDDevs[MAX_MDDEV_ENTRY] = {
    {
        .InstanceNumber = 1,
        .Alias          = "cpe-MDDevice-1",
        .Description    = "dummy-1",
        .MACAddress     = "00:1A:2B:AA:BB:CC",
        .AlwaysBlock   = FALSE,
        .StartTime      = "19:00",
        .EndTime        = "22:00",
        .BlockDays      = "Sat,Sun",
    },
    {
        .InstanceNumber = 2,
        .Alias          = "cpe-MDDevice-2",
        .Description    = "dummy-2",
        .MACAddress     = "00:1A:2B:11:22:33",
        .AlwaysBlock   = TRUE,
        .StartTime      = "20:00",
        .EndTime        = "24:00",
        .BlockDays      = "Mon,Tue",
    },
};

static int
MDDev_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_NrMDDevs; i++)
        if (g_MDDevs[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlMDDev_GetNumberOfEntries(void)
{
    return g_NrMDDevs;
}

ANSC_STATUS
CosaDmlMDDev_GetEntryByIndex(ULONG index, COSA_DML_MD_DEV *pEntry)
{
    if (index >= g_NrMDDevs)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MDDevs[index];
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_NrMDDevs)
        return ANSC_STATUS_FAILURE;

    g_MDDevs[index].InstanceNumber = ins;
    snprintf(g_MDDevs[index].Alias, sizeof(g_MDDevs[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_AddEntry(COSA_DML_MD_DEV *pEntry)
{
    if (g_NrMDDevs >= MAX_MDDEV_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_MDDevs[g_NrMDDevs++] = *pEntry;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_DelEntry(ULONG ins)
{
    int i;

    if ((i = MDDev_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    memmove(&g_MDDevs[i], &g_MDDevs[i+1], 
            (g_NrMDDevs - i - 1) * sizeof(COSA_DML_MD_DEV));
    g_NrMDDevs--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_GetConf(ULONG ins, COSA_DML_MD_DEV *pEntry)
{
    int i;

    if ((i = MDDev_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pEntry = g_MDDevs[i];
    pEntry->InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMDDev_SetConf(ULONG ins, COSA_DML_MD_DEV *pEntry)
{
    int i;

    if ((i = MDDev_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    g_MDDevs[i] = *pEntry;
    g_MDDevs[i].InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}

#elif defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)

#define _CHK_TIME(h, m) ((h) >= 0 && (h) < 24 && (m) >= 0 && (m) < 60)
BOOL
CosaDmlMngSites_Chktime(COSA_DML_BLOCKEDURL *pBlkURL)
{
    int start_time[2];
    int end_time[2];
    int ret;
    if(pBlkURL->AlwaysBlock == FALSE && pBlkURL->StartTimeFlg == TRUE && pBlkURL->EndTimeFlg == TRUE){
        ret = sscanf(pBlkURL->StartTime, "%02d:%02d", &start_time[0], &start_time[1]);
        if(ret != 2 || ! _CHK_TIME(start_time[0], start_time[1]) ){
            pBlkURL->StartTimeFlg = FALSE;
        }
        ret = sscanf(pBlkURL->EndTime, "%02d:%02d", &end_time[0], &end_time[1]);
        if(ret != 2 ||  !_CHK_TIME(end_time[0], end_time[1]) ){
            pBlkURL->EndTimeFlg = FALSE;
        }
        if(pBlkURL->StartTimeFlg != TRUE || pBlkURL->EndTimeFlg != TRUE){
            goto WRONG_FORMAT;
        }
        start_time[0] = (start_time[0] << 16) | start_time[1];
        end_time[0] = (end_time[0] << 16) | end_time[1];
        if(start_time[0] < end_time[0]){
            return TRUE;
        }else{
            CcspTraceWarning(("%s failed :start time latter than end time\n", __FUNCTION__));
            return FALSE;
        }
WRONG_FORMAT:
        CcspTraceWarning(("%s failed :wrong time format\n", __FUNCTION__));
        return FALSE;
   }
   return TRUE;
}


ANSC_STATUS
CosaDmlParentalControlInit(ANSC_HANDLE hDml, PANSC_HANDLE phContext)
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    return ANSC_STATUS_SUCCESS;
}

/*
 * ManagedSites
 */
ANSC_STATUS
CosaDmlMngSites_GetConf(COSA_DML_MANAGEDSITES *conf)
{
    UtopiaContext ctx;
    int enabled = -1;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    Utopia_GetBlkURLCfg(&ctx, &enabled);
    conf->Enable = enabled == 1 ? TRUE : FALSE;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMngSites_SetConf(COSA_DML_MANAGEDSITES *conf)
{
    int rc = -1;
    UtopiaContext ctx;
   
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_SetBlkURLCfg(&ctx, conf->Enable);

    Utopia_Free(&ctx,!rc);
    
    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else 
    {
       commonSyseventSet("pp_flush", "1");
       commonSyseventSet("firewall-restart", "");
       return ANSC_STATUS_SUCCESS;
    }
}

/*
 * ManagedServices
 */
ANSC_STATUS
CosaDmlMngServs_GetConf(COSA_DML_MANAGED_SERVS *conf)
{
    UtopiaContext ctx;
    int enabled = -1;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    Utopia_GetMngServsCfg(&ctx, &enabled);
    conf->Enable = enabled == 1 ? TRUE : FALSE;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMngServs_SetConf(COSA_DML_MANAGED_SERVS *conf)
{
    int rc = -1;
    UtopiaContext ctx;
   
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_SetMngServsCfg(&ctx, conf->Enable);
    
    Utopia_Free(&ctx,!rc);
    
    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
    {
       commonSyseventSet("firewall-restart", "");
       return ANSC_STATUS_SUCCESS;
    }
}

/*
 * ManagedDevices
 */
ANSC_STATUS
CosaDmlMDRed_GetConf(COSA_DML_MD_RED *pEntry)
{
    char ipv4[17];
    char ipv6[64];
    int rc;

	/* HTTP_Server_IP */
    memset(ipv4, 0, sizeof(ipv4));
    rc = syscfg_get( NULL, "HTTP_Server_IP", ipv4, sizeof(ipv4));
    if( rc == 0 && ipv4[0] != '\0' )
    {
       AnscCopyString(pEntry->HTTP_Server_IP, ipv4);
    }
	else
	{
		/*Setting default value as it may break iptable rule*/
		memset(ipv4, 0, sizeof(ipv4));
		snprintf(ipv4,sizeof(ipv4),"127.0.0.1");
		if (syscfg_set_commit(NULL, "HTTP_Server_IP", ipv4) != 0)
   		{
			CcspTraceWarning(("syscfg_set failed\n"));
   		}
		AnscCopyString(pEntry->HTTP_Server_IP, ipv4);
	}

	/* HTTPS_Server_IP */
    memset(ipv4, 0, sizeof(ipv4));
    rc = syscfg_get( NULL, "HTTPS_Server_IP", ipv4, sizeof(ipv4));
    if( rc == 0 && ipv4[0] != '\0' )
    {
       AnscCopyString(pEntry->HTTPS_Server_IP, ipv4);
    }
	else
	{
		/*Setting default value as it may break iptable rule*/
		memset(ipv4, 0, sizeof(ipv4));
		snprintf(ipv4,sizeof(ipv4),"127.0.0.1");
		if (syscfg_set_commit(NULL, "HTTPS_Server_IP", ipv4) != 0)
   		{
			CcspTraceWarning(("syscfg_set failed\n"));
   		}
		AnscCopyString(pEntry->HTTPS_Server_IP, ipv4);
	
	}

	/* Default_Server_IP */
    memset(ipv4, 0, sizeof(ipv4));
    rc = syscfg_get( NULL, "Default_Server_IP", ipv4, sizeof(ipv4));
    if( rc == 0 && ipv4[0] != '\0' )
    {
       AnscCopyString(pEntry->Default_Server_IP, ipv4);
    }
	else
	{
		/*Setting default value as it may break iptable rule*/
		memset(ipv4, 0, sizeof(ipv4));
		snprintf(ipv4,sizeof(ipv4),"127.0.0.1");
		if (syscfg_set_commit(NULL, "Default_Server_IP", ipv4) != 0)
   		{
			CcspTraceWarning(("syscfg_set failed\n"));
   		}
		AnscCopyString(pEntry->Default_Server_IP, ipv4);
	
	}

	/* HTTP_Server_IPv6 */
    memset(ipv6, 0, sizeof(ipv6));
    rc = syscfg_get( NULL, "HTTP_Server_IPv6", ipv6, sizeof(ipv6));
    if( rc == 0 && ipv6[0] != '\0' )
    {
       AnscCopyString(pEntry->HTTP_Server_IPv6, ipv6);
    }
	else
	{
		/*Setting default value as it may break iptable rule*/
		memset(ipv6, 0, sizeof(ipv6));
		snprintf(ipv6,sizeof(ipv6),"::1");
		if (syscfg_set_commit(NULL, "HTTP_Server_IPv6", ipv6) != 0)
   		{
			CcspTraceWarning(("syscfg_set failed\n"));
   		}
		AnscCopyString(pEntry->HTTP_Server_IPv6, ipv6);
	
	}

	/* HTTPS_Server_IPv6 */
    memset(ipv6, 0, sizeof(ipv6));
    rc = syscfg_get( NULL, "HTTPS_Server_IPv6", ipv6, sizeof(ipv6));
    if( rc == 0 && ipv6[0] != '\0' )
    {
       AnscCopyString(pEntry->HTTPS_Server_IPv6, ipv6);
    }
	else
	{
		/*Setting default value as it may break iptable rule*/
		memset(ipv6, 0, sizeof(ipv6));
		snprintf(ipv6,sizeof(ipv6),"::1");
		if (syscfg_set_commit(NULL, "HTTPS_Server_IPv6", ipv6) != 0)
   		{
			CcspTraceWarning(("syscfg_set failed\n"));
   		}
		AnscCopyString(pEntry->HTTPS_Server_IPv6, ipv6);
	
	}

	/* Default_Server_IPv6 */
    memset(ipv6, 0, sizeof(ipv6));
    rc = syscfg_get( NULL, "Default_Server_IPv6", ipv6, sizeof(ipv6));
    if( rc == 0 && ipv6[0] != '\0' )
    {
       AnscCopyString(pEntry->Default_Server_IPv6, ipv6);
    }
	else
	{
		/*Setting default value as it may break iptable rule*/
		memset(ipv6, 0, sizeof(ipv6));
		snprintf(ipv6,sizeof(ipv6),"::1");
		if (syscfg_set_commit(NULL, "Default_Server_IPv6", ipv6) != 0)
   		{
			CcspTraceWarning(("syscfg_set failed\n"));
   		}
		AnscCopyString(pEntry->Default_Server_IPv6, ipv6);
	
	}

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDRed_SetConf(COSA_DML_MD_RED *pEntry)
{
   if ((syscfg_set(NULL, "HTTP_Server_IP", pEntry->HTTP_Server_IP) != 0) ||
       (syscfg_set(NULL, "HTTPS_Server_IP", pEntry->HTTPS_Server_IP) != 0) ||
       (syscfg_set(NULL, "Default_Server_IP", pEntry->Default_Server_IP) != 0) ||
       (syscfg_set(NULL, "HTTP_Server_IPv6", pEntry->HTTP_Server_IPv6) != 0) ||
       (syscfg_set(NULL, "HTTPS_Server_IPv6", pEntry->HTTPS_Server_IPv6) != 0) ||
       (syscfg_set(NULL, "Default_Server_IPv6", pEntry->Default_Server_IPv6) != 0))
   {
       CcspTraceWarning(("syscfg_set failed\n"));
   }
   if (syscfg_commit() != 0) 
   {
       CcspTraceWarning(("syscfg_commit failed\n"));
   }

   return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS isValidIP( int type, char *ipAddress, char *wrapped_inputparam, int sizeof_wrapped_inputparam)
{
	struct sockaddr_in sa;
	struct sockaddr_in6 sa6;
	int result= -1;
	errno_t rc =  -1;

	if(type == 4)
		result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
	else
		result = inet_pton(AF_INET6, ipAddress, &(sa6.sin6_addr));

	if (1!=result)
		return ANSC_STATUS_FAILURE;
	if(strstr(ipAddress,"'"))
		 return ANSC_STATUS_FAILURE;

	rc = sprintf_s(wrapped_inputparam, sizeof_wrapped_inputparam ,"'%s'",ipAddress);
	if(rc < EOK)
	{
		ERR_CHK(rc);
		return ANSC_STATUS_FAILURE;
	}

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMngDevs_GetConf(COSA_DML_MANAGED_DEVS *conf)
{
    UtopiaContext ctx;
    mng_devs_t mng_devs;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetMngDevsCfg(&ctx, &mng_devs);
    
    conf->Enable = mng_devs.enable == 1 ? TRUE : FALSE;
    conf->AllowAll = mng_devs.allow_all == 1 ? TRUE : FALSE;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMngDevs_SetConf(COSA_DML_MANAGED_DEVS *conf)
{
    int rc = -1;
    UtopiaContext ctx;
    mng_devs_t mng_devs;
   
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    mng_devs.enable = conf->Enable ? 1 : 0;
    mng_devs.allow_all = conf->AllowAll ? 1 : 0;
    
    rc = Utopia_SetMngDevsCfg(&ctx, &mng_devs);

    Utopia_Free(&ctx,!rc);
    
    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
    {
       CcspTraceWarning(("ParentalControlManagedDevices:%s\n",(mng_devs.enable == 1 ) ? "Enabled" : "Disabled" ));
       commonSyseventSet("firewall-restart", "");
       return ANSC_STATUS_SUCCESS;
    }
}

/*
 * Blocked URL
 */
static int g_NrBlkURL =  0;

static int
BlkURL_InsGetIndex(ULONG ins)
{
    int i, ret = -1;
    int ins_num;
    UtopiaContext ctx;
    
    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    for (i = 0; i < g_NrBlkURL; i++)
    {
        Utopia_GetBlkURLInsNumByIndex(&ctx, i, &ins_num);
        if ((ULONG)ins_num == ins) {
            ret = i;
            break;
        }
    }

    Utopia_Free(&ctx, 0);

    return ret;
}

ULONG
CosaDmlBlkURL_GetNumberOfEntries(void)
{
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetNumberOfBlkURL(&ctx, &g_NrBlkURL);

    Utopia_Free(&ctx, 0);

    return g_NrBlkURL;
}

#ifdef UTC_ENABLE
int dIndx[250];
int blkIns[250];
int dCnt = 0;
void ResetInsBuf()
{
	memset(dIndx,0, sizeof(dIndx));
	memset(blkIns,0, sizeof(blkIns));
	dCnt = 0;
}
int SetDelIndex(int ind,int ins)
{
	dIndx[dCnt] = ind;
	blkIns[dCnt] = ins;
	dCnt++;
	AnscTraceWarning(("<<< %s -- set Del index %d ...\n", __FUNCTION__,ind));
    return 0;
}

int ChkDelIndex(int ind)
{
	int cnt = 0;
	if(ind == 0)
	{
		return 0;
	}
	for(cnt =0; cnt < 250; cnt++)
	{
	if(dIndx[cnt] == ind)
	{
		AnscTraceWarning(("<<< %s -- chk Del index %d ...\n", __FUNCTION__,ind));
		return 1;
	}
		
	}
	return 0;
}
int isAlwaysBlock(char *days, char *sTime, char *eTime)
{
	if(!strcmp(days,"Mon,Tue,Wed,Thu,Fri,Sat,Sun"))
	{
		AnscTraceWarning(("<<< %s -- All day are set ...\n", __FUNCTION__));
		if((!strcmp(sTime,"00:00")) && (!strcmp(eTime,"23:59")))
		{
			AnscTraceWarning(("<<< %s -- Always block ...\n", __FUNCTION__));
			return 1;
		}
	}
	return 0;
}
int delBlkUrl()
{
	int i = 0;
	int rc = -1;
	UtopiaContext ctx;
        /* CID: 59086 Unchecked return value*/
	if(!Utopia_Init(&ctx))
           return rc;
	for(i = 0;i<dCnt;i++)
	{
		
    	AnscTraceWarning(("<<< %s delete ins_num  %d ...\n", __FUNCTION__,blkIns[i]));
        rc = Utopia_DelBlkURL(&ctx, blkIns[i]);
	}
	Utopia_GetNumberOfBlkURL(&ctx, &g_NrBlkURL);
	Utopia_Free(&ctx, !rc);
    return 0;
	
}
ANSC_STATUS CosaDmlBlkURL_RollbackUTCtoLocal()
{
    UtopiaContext ctx;
    blkurl_t blkurl,tmp;
    ULONG index =0;
	ULONG TIndex =0;
    int rc = -1;
    char start_time[25];
    char end_time[25];
    char eTime_block_days[64];
	int sRet = 0,eRet = 0;
    ULONG nCount = 0;
    errno_t safec_rc = -1;
    AnscTraceWarning(("<<< %s -- ...\n", __FUNCTION__));
	nCount = g_NrBlkURL;
    ResetInsBuf();
	AnscTraceWarning(("<<< %s --  g_NrBlkURL %d ...\n", __FUNCTION__,g_NrBlkURL));

    for(index = 0;index < nCount; index++)
    {

       /*CID: 55473 Unchecked return value*/
	 if(!Utopia_Init(&ctx))
            return ANSC_STATUS_FAILURE;
     AnscTraceWarning(("<<< %s -- Call GetBlkURLByIndex %lu ...\n", __FUNCTION__,index));
	 if(ChkDelIndex(index))
	 {
		 Utopia_Free(&ctx, !rc);
		 continue;
	 }
     Utopia_GetBlkURLByIndex(&ctx, index, &blkurl);
	 if(blkurl.always_block)
	 {
		 AnscTraceWarning(("<<< %s --alwaysblock  %s ...\n", __FUNCTION__,blkurl.site));
		 Utopia_Free(&ctx, !rc);
		 continue;
	 }
	 for(TIndex = index+1;TIndex < nCount; TIndex++)
	 {
		 if(ChkDelIndex(TIndex))
		  continue;
	 
	 AnscTraceWarning(("<<< %s --  TIndex %lu ...\n", __FUNCTION__,TIndex));
	 
		 Utopia_GetBlkURLByIndex(&ctx, TIndex, &tmp);
		 if(!strcmp(tmp.site,blkurl.site))
		 {
			 if(tmp.always_block == blkurl.always_block)
			 {
				AnscTraceWarning(("%s -- tmp.block_days = %s...\n", __FUNCTION__, tmp.block_days));

				 if(strcmp(tmp.block_days,blkurl.block_days) || (!strcmp(blkurl.block_days,"Mon,Tue,Wed,Thu,Fri,Sat,Sun")))
				 {
					 if(strstr(tmp.start_time,"00:00"))
					 {
						 if(strstr(blkurl.end_time,"23:59"))
						 {
							 AnscTraceWarning(("<<< %s -- 1 del rule %s ...\n", __FUNCTION__,blkurl.site));
							 safec_rc = strcpy_s(blkurl.end_time,sizeof(blkurl.end_time),tmp.end_time);
							 if(safec_rc != EOK)
							 {
								 ERR_CHK(safec_rc);
							 }
							 SetDelIndex(TIndex,tmp.ins_num);
							 Utopia_GetNumberOfBlkURL(&ctx, &g_NrBlkURL);
						 }
					 }
					 else if(strstr(tmp.end_time,"23:59"))
					 {
						 if(strstr(blkurl.start_time,"00:00"))
						 {
							 AnscTraceWarning(("<<< %s -- 2 del rule %s ...\n", __FUNCTION__,blkurl.site));
							 safec_rc = strcpy_s(blkurl.start_time,sizeof(blkurl.start_time),tmp.start_time);
							 if(safec_rc != EOK)
							 {
								 ERR_CHK(safec_rc);
							 }
							 SetDelIndex(TIndex,tmp.ins_num);
							 Utopia_GetNumberOfBlkURL(&ctx, &g_NrBlkURL);
						 }
						 
					 }		 
				 }
				 else
				 {
					 AnscTraceWarning(("%s -- continue tmp.block_days = %s...\n", __FUNCTION__, tmp.block_days));
					 continue;
				 }
			 }
		 }
		 
	 }
     memset(start_time,0,25);
     memset(end_time,0,25);
     if(ChkDelIndex(TIndex) == 0)
	 {
		 AnscTraceWarning(("%s -- blkurl.alias = %s...\n", __FUNCTION__, blkurl.alias));
		 AnscTraceWarning(("%s -- blkurl.site = %s...\n", __FUNCTION__, blkurl.site));
		 AnscTraceWarning(("%s -- blkurl.start_time = %s...\n", __FUNCTION__, blkurl.start_time));
		 AnscTraceWarning(("%s -- blkurl.end_time = %s...\n", __FUNCTION__, blkurl.end_time));

		 safec_rc = strcpy_s(start_time, sizeof(start_time), blkurl.start_time);
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 safec_rc = strcpy_s(end_time, sizeof(end_time), blkurl.end_time);
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 sRet = ConvUTCToLocal(start_time, blkurl.start_time);
		 eRet = ConvUTCToLocal(end_time, blkurl.end_time);
		 memset(eTime_block_days,0,64);
		 split_BlockDays(sRet,eRet,blkurl.block_days,eTime_block_days);
		 AnscTraceWarning(("%s -- Converted blkurl.start_time = %s...\n", __FUNCTION__, blkurl.start_time));
		 AnscTraceWarning(("%s -- Converted blkurl.end_time = %s...\n", __FUNCTION__, blkurl.end_time));
		 AnscTraceWarning(("%s -- Converted blkurl.block_days = %s...\n", __FUNCTION__, blkurl.block_days));

		 rc = Utopia_SetBlkURLByIndex(&ctx, index, &blkurl);
	 }
	 else
	 {
		AnscTraceWarning(("%s --else nCount = %lu index = %lu...\n", __FUNCTION__, nCount,index));
	 }
     Utopia_Free(&ctx, !rc);
     if (rc != 0)
	 {
	    AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
        return ANSC_STATUS_FAILURE;
	 }

   }
   delBlkUrl();
 return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlBlkURL_Migration()
{
    UtopiaContext ctx;
    blkurl_t blkurl;
    int index =0;
    int rc = -1;
    char start_time[25];
    char end_time[25];
    char rb_end_time[25];
	int sRet = 0,eRet = 0;
	int                             InsNum                 = 0;
	char eTime_block_days[64];
	errno_t safec_rc = -1;
    AnscTraceWarning(("<<< %s -- ...\n", __FUNCTION__));
for(index = 0;index < g_NrBlkURL; index++)
    {
     AnscTraceWarning(("<<< %s -- index %d ...\n", __FUNCTION__,index));

    if (index >= g_NrBlkURL || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

     Utopia_GetBlkURLByIndex(&ctx, index, &blkurl);
     memset(start_time,0,25);
     memset(end_time,0,25);
    
     AnscTraceWarning(("%s -- blkurl.alias = %s...\n", __FUNCTION__, blkurl.alias));
     AnscTraceWarning(("%s -- blkurl.site = %s...\n", __FUNCTION__, blkurl.site));
     AnscTraceWarning(("%s -- blkurl.start_time = %s...\n", __FUNCTION__, blkurl.start_time));
     AnscTraceWarning(("%s -- blkurl.end_time = %s...\n", __FUNCTION__, blkurl.end_time));

     if(isAlwaysBlock(blkurl.block_days,blkurl.start_time,blkurl.end_time))
	 {
		 blkurl.always_block = TRUE;
		rc = Utopia_SetBlkURLByIndex(&ctx, index, &blkurl);
        if (rc != 0)
	    {
	       AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
           Utopia_Free(&ctx, 0);
           return ANSC_STATUS_FAILURE;
	    }
		Utopia_Free(&ctx, !rc);
		continue;
	 }
	 if(blkurl.always_block == TRUE)
	 {
		 AnscTraceWarning(("<<< %s --alwaysblock  %s ...\n", __FUNCTION__,blkurl.site));
		 Utopia_Free(&ctx, !rc);
		 continue;
	 }
     safec_rc = strcpy_s(start_time, sizeof(start_time), blkurl.start_time);
     if(safec_rc != EOK)
     {
        ERR_CHK(safec_rc);
     }
     safec_rc = strcpy_s(end_time, sizeof(end_time), blkurl.end_time);
     if(safec_rc != EOK)
     {
        ERR_CHK(safec_rc);
     }
     sRet = ConvLocalToUTC(start_time, blkurl.start_time);
     eRet = ConvLocalToUTC(end_time, blkurl.end_time);
     AnscTraceWarning(("%s -- Converted blkurl.start_time = %s sRet = %d ...\n", __FUNCTION__, blkurl.start_time,sRet));
     AnscTraceWarning(("%s -- Converted blkurl.end_time = %s eRet = %d ...\n", __FUNCTION__, blkurl.end_time,eRet));
	 if(!strcmp(blkurl.end_time,"00:00"))
	 {
		 AnscTraceWarning(("%s -- inside if blkurl.end_time = %s eRet = %d ...\n", __FUNCTION__, blkurl.end_time,eRet));
		 safec_rc = strcpy_s(blkurl.end_time, sizeof(blkurl.end_time),"23:59");
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		rc = Utopia_SetBlkURLByIndex(&ctx, index, &blkurl);
        if (rc != 0)
	    {
	       AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
           Utopia_Free(&ctx, 0);
           return ANSC_STATUS_FAILURE;
	    }
		Utopia_Free(&ctx, !rc);
		continue;
	 }
     memset(eTime_block_days,0,64);
	 if(split_BlockDays(sRet,eRet,blkurl.block_days,eTime_block_days))
	 {
		 AnscTraceWarning(("%s -- Day change required, split the rule  ...\n", __FUNCTION__));
		 AnscTraceWarning(("%s -- Converted block_days for first rule = %s  ...\n", __FUNCTION__, blkurl.block_days));
		 AnscTraceWarning(("%s -- Converted block_days for Second rule = %s ...\n", __FUNCTION__,eTime_block_days));
		 // split rule timings
		 safec_rc = strcpy_s(rb_end_time,sizeof(rb_end_time), blkurl.end_time);
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 safec_rc = strcpy_s(blkurl.end_time,sizeof(blkurl.end_time),"23:59");
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 AnscTraceWarning(("%d -- Converted blkurl.end_time = %s ...\n", __LINE__,blkurl.end_time));
		 rc = Utopia_SetBlkURLByIndex(&ctx, index, &blkurl);
		 if (rc != 0)
			{
			    AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
                Utopia_Free(&ctx, 0);
				return ANSC_STATUS_FAILURE;
			}
		    safec_rc = strcpy_s(blkurl.end_time,sizeof(blkurl.end_time),rb_end_time);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		 	AnscTraceWarning(("%d -- Converted blkurl.end_time = %s ...\n", __LINE__,blkurl.end_time));
		    safec_rc = strcpy_s(blkurl.start_time,sizeof(blkurl.start_time),"00:00");
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		    AnscTraceWarning(("%d -- Converted blkurl.start_time = %s ...\n", __LINE__,blkurl.start_time));
		    safec_rc = strcpy_s(blkurl.block_days,sizeof(blkurl.block_days),eTime_block_days);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		    AnscTraceWarning(("%d -- Converted eTime_block_days = %s ...\n", __LINE__,eTime_block_days));
		    AnscTraceWarning(("%d -- Converted blkurl.block_days = %s ...\n", __LINE__,blkurl.block_days));

		blkurl.ins_num = InsNum;
		rc = Utopia_AddBlkURL(&ctx,&blkurl);
			if (rc != 0)
			{
			   AnscTraceWarning(("%s -- Utopia_AddBlkURL failed...\n",__FUNCTION__));
               Utopia_Free(&ctx, 0);
			   return ANSC_STATUS_FAILURE;
			}
	 }else
	 {
        rc = Utopia_SetBlkURLByIndex(&ctx, index, &blkurl);
        if (rc != 0)
	    {
	       AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
           Utopia_Free(&ctx, 0);
           return ANSC_STATUS_FAILURE;
	    }
	 }
    AnscTraceWarning(("%s -- Converted Success ...\n", __FUNCTION__));
    Utopia_Free(&ctx, !rc);
    }
 if(g_NrBlkURL)
	{
           /* CID: 63902 Unchecked return value*/
	   if(!Utopia_Init(&ctx))
              return ANSC_STATUS_FAILURE;
       rc = Utopia_GetNumberOfBlkURL(&ctx, &g_NrBlkURL);
	   AnscTraceWarning(("%s -- Converted g_NrBlkURL = %d ...\n", __FUNCTION__,g_NrBlkURL));
       Utopia_Free(&ctx, !rc);
       if (rc != 0)
           return ANSC_STATUS_FAILURE;
	}
 return ANSC_STATUS_SUCCESS;
}
#endif
ANSC_STATUS
CosaDmlBlkURL_GetEntryByIndex(ULONG index, COSA_DML_BLOCKEDURL *pEntry)
{
    UtopiaContext ctx;
    blkurl_t blkurl;

    if (index >= (ULONG)g_NrBlkURL || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetBlkURLByIndex(&ctx, index, &blkurl);

    pEntry->InstanceNumber = blkurl.ins_num;
    pEntry->AlwaysBlock = blkurl.always_block;

    if(!_ansc_strcmp(blkurl.block_method, "URL"))
        pEntry->BlockMethod = BLOCK_METHOD_URL;
    else if(!_ansc_strcmp(blkurl.block_method, "KEYWD"))
        pEntry->BlockMethod = BLOCK_METHOD_KEYWORD;
    else
        pEntry->BlockMethod = 0;

    _ansc_strncpy(pEntry->Alias, blkurl.alias, sizeof(pEntry->Alias));
    _ansc_strncpy(pEntry->Site, blkurl.site, sizeof(pEntry->Site));

    _ansc_strncpy(pEntry->StartTime, blkurl.start_time, sizeof(pEntry->StartTime));
    _ansc_strncpy(pEntry->EndTime, blkurl.end_time, sizeof(pEntry->EndTime));
    _ansc_strncpy(pEntry->BlockDays, blkurl.block_days, sizeof(pEntry->BlockDays));
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[URL]:%lu,%s\n", __FUNCTION__, __LINE__, pEntry->InstanceNumber,pEntry->Site));
    /* CID: 71902 Array compared against 0*/
        pEntry->StartTimeFlg = TRUE;
        pEntry->EndTimeFlg = TRUE;

#if defined(CONFIG_CISCO_FEATURE_CISCOCONNECT) 
    _ansc_strncpy(pEntry->MAC, blkurl.mac, sizeof(pEntry->MAC));
    _ansc_strncpy(pEntry->DeviceName, blkurl.device_name, sizeof(pEntry->DeviceName));
#endif
#if defined(CONFIG_CISCO_FEATURE_CISCOCONNECT) || defined(CONFIG_CISCO_PARCON_WALLED_GARDEN)  
    char str[256];

    FILE *fp = v_secure_popen("r", "ipset test %lu 0.0.0.0 2>&1", blkurl.ins_num);
    fgets(str, sizeof(str), fp);
    v_secure_pclose(fp);
  
    //create ip set if not exists
    if(strstr(str, "does not exist") != NULL) {
        v_secure_system("ipset create %lu hash:ip", blkurl.ins_num);
    }

    fp = v_secure_popen("r", "ipset test %lu_v6 01::02 2>&1", blkurl.ins_num);
    fgets(str, sizeof(str), fp);
    v_secure_pclose(fp);
  
    //create ip set if not exists
    if(strstr(str, "does not exist") != NULL) {
        v_secure_system("ipset create %lu_v6 hash:ip family inet6 ", blkurl.ins_num);
    }

#endif

    Utopia_Free(&ctx, 0);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_SetValues(ULONG index, ULONG ins, const char *alias)
{
    int rc = -1;
    UtopiaContext ctx;
    
    if (index >= (ULONG)g_NrBlkURL || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_SetBlkURLInsAndAliasByIndex(&ctx, index, ins, alias);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlBlkURL_AddEntry(COSA_DML_BLOCKEDURL *pEntry)
{
    int rc = -1;
    UtopiaContext ctx;
    blkurl_t blkurl;
    errno_t safec_rc = -1;
    
    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    blkurl.ins_num = pEntry->InstanceNumber;
    blkurl.always_block = pEntry->AlwaysBlock;
    
    if(pEntry->BlockMethod == BLOCK_METHOD_URL)
    {
        safec_rc = strcpy_s(blkurl.block_method, sizeof(blkurl.block_method), "URL");
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }
    }
    else if(pEntry->BlockMethod == BLOCK_METHOD_KEYWORD)
    {
        safec_rc = strcpy_s(blkurl.block_method, sizeof(blkurl.block_method), "KEYWD");
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }
    }
    else
    {
        safec_rc = strcpy_s(blkurl.block_method, sizeof(blkurl.block_method), "");
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }
    }

    /* CID 185693 fix */
    safec_rc = strcpy_s(blkurl.alias, (sizeof(blkurl.alias)), pEntry->Alias);
    if(safec_rc != EOK)
    {
        ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(blkurl.site, sizeof(blkurl.site), pEntry->Site);
    if(safec_rc != EOK)
    {
        ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(blkurl.start_time, sizeof(blkurl.start_time), pEntry->StartTime);
    if(safec_rc != EOK)
    {
        ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(blkurl.end_time, sizeof(blkurl.end_time), pEntry->EndTime);
    if(safec_rc != EOK)
    {
        ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(blkurl.block_days, sizeof(blkurl.block_days), pEntry->BlockDays);
    if(safec_rc != EOK)
    {
        ERR_CHK(safec_rc);
    }
#ifdef CONFIG_CISCO_FEATURE_CISCOCONNECT
    safec_rc = strcpy_s(blkurl.mac, sizeof(blkurl.mac), pEntry->MAC);
    if(safec_rc != EOK)
    {
        ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(blkurl.device_name, sizeof(blkurl.device_name), pEntry->DeviceName);
    if(safec_rc != EOK)
    {
        ERR_CHK(safec_rc);
    }
#endif
    rc = Utopia_AddBlkURL(&ctx, &blkurl);
    Utopia_GetNumberOfBlkURL(&ctx, &g_NrBlkURL);
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[URL]:%lu,%s\n", __FUNCTION__, __LINE__, pEntry->InstanceNumber,pEntry->Site));

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
#if defined(CONFIG_CISCO_FEATURE_CISCOCONNECT) || defined(CONFIG_CISCO_PARCON_WALLED_GARDEN)  
        v_secure_system("ipset create %lu hash:ip", blkurl.ins_num);
        v_secure_system("ipset create %lu_v6 hash:ip family inet6 ", blkurl.ins_num);
#endif

        commonSyseventSet("pp_flush", "1");
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

#define URL2IP_PATH "/var/.pc_url2ip_%lu"
#define URL2IP6_PATH "/var/.pc_url2ipv6_%lu"

ANSC_STATUS
CosaDmlBlkURL_DelEntry(ULONG ins)
{
    int rc = -1;
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_DelBlkURL(&ctx, ins);
    Utopia_GetNumberOfBlkURL(&ctx, &g_NrBlkURL);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
#if defined(CONFIG_CISCO_FEATURE_CISCOCONNECT) || defined(CONFIG_CISCO_PARCON_WALLED_GARDEN)  
        v_secure_system("ipset destroy %lu", ins);
        v_secure_system("ipset destroy %lu_v6", ins);
#else
        char url2ipFilePath[256];
        snprintf(url2ipFilePath, sizeof(url2ipFilePath), URL2IP_PATH, ins);
        /* CID: 104500 fix*/
        if (remove(url2ipFilePath) != 0)
        {
            CcspTraceWarning(("%s: Error deleting the file\n", __FUNCTION__));
        }
        snprintf(url2ipFilePath, sizeof(url2ipFilePath), URL2IP6_PATH, ins);
        if (remove(url2ipFilePath) != 0)
        {
            CcspTraceWarning(("%s: Error deleting the file\n", __FUNCTION__));
        }
#endif

        commonSyseventSet("pp_flush", "1");
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlBlkURL_GetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry)
{
    int index;

    if ((index = BlkURL_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    return CosaDmlBlkURL_GetEntryByIndex(index, pEntry);
}


ANSC_STATUS
CosaDmlBlkURL_SetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry)
{
    int index;
    UtopiaContext ctx;
    blkurl_t blkurl;
    int rc = -1;
    
    if ((index = BlkURL_InsGetIndex(ins)) == -1 || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    index = BlkURL_InsGetIndex(ins);

    blkurl.ins_num = pEntry->InstanceNumber;
    blkurl.always_block = pEntry->AlwaysBlock;

    if(pEntry->BlockMethod == BLOCK_METHOD_URL)
        _ansc_strncpy(blkurl.block_method, "URL", sizeof(blkurl.block_method));
    else if(pEntry->BlockMethod == BLOCK_METHOD_KEYWORD)
        _ansc_strncpy(blkurl.block_method, "KEYWD", sizeof(blkurl.block_method));
    else
        _ansc_strncpy(blkurl.block_method, "", sizeof(blkurl.block_method));

    _ansc_strncpy(blkurl.alias, pEntry->Alias, sizeof(blkurl.alias));
    _ansc_strncpy(blkurl.site, pEntry->Site, sizeof(blkurl.site));

    _ansc_strncpy(blkurl.start_time, pEntry->StartTime, sizeof(blkurl.start_time));
    _ansc_strncpy(blkurl.end_time, pEntry->EndTime, sizeof(blkurl.end_time));
    _ansc_strncpy(blkurl.block_days, pEntry->BlockDays, sizeof(blkurl.block_days));
#ifdef CONFIG_CISCO_FEATURE_CISCOCONNECT
    _ansc_strncpy(blkurl.mac, pEntry->MAC, sizeof(blkurl.mac));
    _ansc_strncpy(blkurl.device_name, pEntry->DeviceName, sizeof(blkurl.device_name));
#endif

    rc = Utopia_SetBlkURLByIndex(&ctx, index, &blkurl);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
#if defined(CONFIG_CISCO_FEATURE_CISCOCONNECT) || defined(CONFIG_CISCO_PARCON_WALLED_GARDEN)  
        v_secure_system("ipset flush %lu", blkurl.ins_num);
        v_secure_system("ipset flush %lu_v6", blkurl.ins_num);
#else
        char url2ipFilePath[256];
        snprintf(url2ipFilePath, sizeof(url2ipFilePath), URL2IP_PATH, ins);
        remove(url2ipFilePath);
        snprintf(url2ipFilePath, sizeof(url2ipFilePath), URL2IP6_PATH, ins);
        remove(url2ipFilePath);
#endif

        commonSyseventSet("pp_flush", "1");
        commonSyseventSet("firewall-restart", "");

        return ANSC_STATUS_SUCCESS; 
    }
}

/*
 * TrustedUser
 */
static int g_NrTrustedUser   = 0;

static int
TrustedUser_InsGetIndex(ULONG ins)
{
    int i, ret = -1;
    int ins_num;
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    for (i = 0; i < g_NrTrustedUser; i++)
    {
        Utopia_GetTrustedUserInsNumByIndex(&ctx, i, &ins_num);
        if ((ULONG)ins_num == ins) {
            ret = i;
            break;
        }
    }

    Utopia_Free(&ctx, 0);

    return ret;
}

ULONG
CosaDmlTrustedUser_GetNumberOfEntries(void)
{
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetNumberOfTrustedUser(&ctx, &g_NrTrustedUser);

    Utopia_Free(&ctx, 0);

    return g_NrTrustedUser;
}

ANSC_STATUS
CosaDmlTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_TRUSTEDUSER *pEntry)
{
    UtopiaContext ctx;
    trusted_user_t trusted_user;

    if (index >= (ULONG)g_NrTrustedUser || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetTrustedUserByIndex(&ctx, index, &trusted_user);

    pEntry->InstanceNumber = trusted_user.ins_num;
    pEntry->Trusted = trusted_user.trusted;
    pEntry->IPAddressType = trusted_user.ipaddrtype == 6 ? IPADDR_IPV6 : IPADDR_IPV4;
    _ansc_strncpy(pEntry->Alias, trusted_user.alias, sizeof(pEntry->Alias));
    _ansc_strncpy(pEntry->HostDescription, trusted_user.host_descp, sizeof(pEntry->HostDescription));
    _ansc_strncpy(pEntry->IPAddress, trusted_user.ipaddr, sizeof(pEntry->IPAddress));

    AnscTraceWarning(("%s-%d RDKB_PCONTROL[TUSER]:%lu,%s\n", __FUNCTION__, __LINE__, pEntry->InstanceNumber, pEntry->IPAddress));
    Utopia_Free(&ctx, 0);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias)
{
    int rc = -1;
    UtopiaContext ctx;
    
    if (index >= (ULONG)g_NrTrustedUser || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_SetTrustedUserInsAndAliasByIndex(&ctx, index, ins, alias);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTrustedUser_AddEntry(COSA_DML_TRUSTEDUSER *pEntry)
{
    int rc = -1;
    UtopiaContext ctx;
    trusted_user_t trusted_user;
    
    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    trusted_user.ins_num = pEntry->InstanceNumber;
    
    trusted_user.trusted = pEntry->Trusted;
    trusted_user.ipaddrtype = pEntry->IPAddressType == IPADDR_IPV4 ? 4 : 6;
    _ansc_strncpy(trusted_user.alias, pEntry->Alias, sizeof(trusted_user.alias));
    /* CID 185687 fix  */
    _ansc_strncpy(trusted_user.host_descp, pEntry->HostDescription, (sizeof(trusted_user.host_descp)-1));
    _ansc_strncpy(trusted_user.ipaddr, pEntry->IPAddress, sizeof(trusted_user.ipaddr));

    rc = Utopia_AddTrustedUser(&ctx, &trusted_user);
    Utopia_GetNumberOfTrustedUser(&ctx, &g_NrTrustedUser);
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[TUSER]:%lu,%s\n", __FUNCTION__, __LINE__, pEntry->InstanceNumber,pEntry->IPAddress));

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("pp_flush", "1");
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlTrustedUser_DelEntry(ULONG ins)
{
    int rc = -1;
    UtopiaContext ctx;
    
    if (TrustedUser_InsGetIndex(ins) == -1 || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_DelTrustedUser(&ctx, ins);
    Utopia_GetNumberOfTrustedUser(&ctx, &g_NrTrustedUser);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("pp_flush", "1");
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlTrustedUser_GetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry)
{
    int index;

    if ((index = TrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    return CosaDmlTrustedUser_GetEntryByIndex(index, pEntry);
}


ANSC_STATUS
CosaDmlTrustedUser_SetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry)
{
    int index;
    UtopiaContext ctx;
    trusted_user_t trusted_user;
    int rc = -1;

    if ((index = TrustedUser_InsGetIndex(ins)) == -1 || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    index = TrustedUser_InsGetIndex(ins);

    trusted_user.ins_num = pEntry->InstanceNumber;
    trusted_user.ipaddrtype = pEntry->IPAddressType == IPADDR_IPV4 ? 4 : 6;
    _ansc_strncpy(trusted_user.alias, pEntry->Alias, sizeof(trusted_user.alias));
    _ansc_strncpy(trusted_user.host_descp, pEntry->HostDescription, sizeof(trusted_user.host_descp));
    _ansc_strncpy(trusted_user.ipaddr, pEntry->IPAddress, sizeof(trusted_user.ipaddr));
    trusted_user.trusted = pEntry->Trusted;

    rc = Utopia_SetTrustedUserByIndex(&ctx, index, &trusted_user);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("pp_flush", "1");
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS; 
    }
}

/*
 * ManagedServices.Service
 */
static int g_NrMSServs   = 0;

static int
MSServ_InsGetIndex(ULONG ins)
{
    int i, ret = -1;
    int ins_num;
    UtopiaContext ctx;

    CosaDmlMSServ_GetNumberOfEntries();

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    for (i = 0; i < g_NrMSServs; i++)
    {
        Utopia_GetMSServInsNumByIndex(&ctx, i, &ins_num);
        if ((ULONG)ins_num == ins) {
            ret = i;
            break;
        }
    }

    Utopia_Free(&ctx, 0);

    return ret;
}

ULONG
CosaDmlMSServ_GetNumberOfEntries(void)
{
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetNumberOfMSServ(&ctx, &g_NrMSServs);

    Utopia_Free(&ctx, 0);

    return g_NrMSServs;
}
#ifdef UTC_ENABLE
int delMSServ()
{
	int i = 0;
	int rc = -1;
	UtopiaContext ctx;
        /*CID: 72810 Unchecked return value*/
	if(!Utopia_Init(&ctx))
             return rc;
	for(i = 0;i<dCnt;i++)
	{
		
    	AnscTraceWarning(("<<< %s delete ins_num  %d ...\n", __FUNCTION__,blkIns[i]));
        rc = Utopia_DelMSServ(&ctx, blkIns[i]);
	}
	Utopia_GetNumberOfMSServ(&ctx, &g_NrMSServs);
	Utopia_Free(&ctx, !rc);
    return 0;
	
}

ANSC_STATUS CosaDmlMSServ_RollbackUTCtoLocal()
{
    UtopiaContext ctx;
    ms_serv_t ms_serv,tmp;
    int index =0;
	int TIndex =0;
    int rc = -1;
    char start_time[25];
    char end_time[25];
	char eTime_block_days[64];
	int sRet = 0,eRet = 0;
    errno_t safec_rc = -1;
    ResetInsBuf();
AnscTraceWarning(("<<< %s -- ...\n", __FUNCTION__));
for(index = 0;index < g_NrMSServs; index++)
    {
     /*CID: 52932 Unchecked return value*/
     if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

     AnscTraceWarning(("<<< %s -- Call Utopia_GetMSServByIndex %d ...\n", __FUNCTION__,index));
	 if(ChkDelIndex(index))
	 {
		 Utopia_Free(&ctx, !rc);
		 continue;
	 }
     Utopia_GetMSServByIndex(&ctx, index, &ms_serv);
	 if(ms_serv.always_block)
	 {
		 Utopia_Free(&ctx, !rc);
		 continue;
	 }
     
	 for(TIndex = index+1;TIndex <g_NrMSServs; TIndex++)
	 {
		 if(ChkDelIndex(TIndex))
		  continue;
		 Utopia_GetMSServByIndex(&ctx, TIndex, &tmp);
		 if(!strcmp(tmp.descp,ms_serv.descp))
		 {
			 if(!strcmp(tmp.protocol,ms_serv.protocol))
			 {
				 if(tmp.always_block == ms_serv.always_block)
				 {
					if(strcmp(tmp.block_days,ms_serv.block_days) || (!strcmp(ms_serv.block_days,"Mon,Tue,Wed,Thu,Fri,Sat,Sun")))
					 {
						 if(strstr(tmp.start_time,"00:00"))
						 {
							 if(strstr(ms_serv.end_time,"23:59"))
							 {
								 AnscTraceWarning(("<<< %s -- copy end time and del rule %d ...\n", __FUNCTION__,TIndex));
								 safec_rc = strcpy_s(ms_serv.end_time,sizeof(ms_serv.end_time),tmp.end_time);
								 if(safec_rc != EOK)
								 {
									ERR_CHK(safec_rc);
								 }
								 SetDelIndex(TIndex,tmp.ins_num);
								 Utopia_GetNumberOfMSServ(&ctx, &g_NrMSServs);
							 }
						 }
						 else if(strstr(tmp.end_time,"23:59"))
						 {
							 if(strstr(ms_serv.start_time,"00:00"))
							 {
								 AnscTraceWarning(("<<< %s -- copy start time and del rule %d ...\n", __FUNCTION__,TIndex));
								 safec_rc = strcpy_s(ms_serv.start_time,sizeof(ms_serv.start_time),tmp.start_time);
								 if(safec_rc != EOK)
								 {
									 ERR_CHK(safec_rc);
								 }
								 SetDelIndex(TIndex,tmp.ins_num);
								 Utopia_GetNumberOfMSServ(&ctx, &g_NrMSServs);
							 }
							 
						 }		 
					 }
					 else
					 {
						 continue;
					 }
				 }
		     }
		 }
		 
	 }
     memset(start_time,0,25);
     memset(end_time,0,25);
     if(ChkDelIndex(TIndex) == 0)
	 {
		 AnscTraceWarning(("%s -- ms_serv.alias = %s...\n", __FUNCTION__, ms_serv.alias));
		 AnscTraceWarning(("%s -- ms_serv.start_time = %s...\n", __FUNCTION__, ms_serv.start_time));
		 AnscTraceWarning(("%s -- ms_serv.end_time = %s...\n", __FUNCTION__, ms_serv.end_time));

		 safec_rc = strcpy_s(start_time, sizeof(start_time), ms_serv.start_time);
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 safec_rc = strcpy_s(end_time, sizeof(end_time), ms_serv.end_time);
		 if(safec_rc != EOK)
		 {
			ERR_CHK(safec_rc);
		 }
		 sRet = ConvUTCToLocal(start_time, ms_serv.start_time);
		 eRet = ConvUTCToLocal(end_time, ms_serv.end_time);
		 memset(eTime_block_days,0,64);
		 split_BlockDays(sRet,eRet,ms_serv.block_days,eTime_block_days);
		 AnscTraceWarning(("%s -- Converted ms_serv.start_time = %s...\n", __FUNCTION__, ms_serv.start_time));
		 AnscTraceWarning(("%s -- Converted ms_serv.end_time = %s...\n", __FUNCTION__, ms_serv.end_time));
		 AnscTraceWarning(("%s -- Converted ms_serv.block_days = %s...\n", __FUNCTION__, ms_serv.block_days));

		 rc = Utopia_SetMSServByIndex(&ctx, index, &ms_serv);
	 }
     Utopia_Free(&ctx, !rc);
     if (rc != 0)
	 {
	    AnscTraceWarning(("%s -- Utopia_SetMSServByIndex failed...\n",__FUNCTION__));
        return ANSC_STATUS_FAILURE;
	 }

   }
 delMSServ();
 return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS CosaDmlMSServ_Migration()
{
    UtopiaContext ctx;
    ms_serv_t ms_serv;
    int index =0;
    int rc = -1;
    char start_time[25];
    char end_time[25];
	char rb_end_time[25];
	int sRet = 0,eRet = 0;
	int InsNum                 = 0;
	char eTime_block_days[64];
    errno_t safec_rc = -1;
    AnscTraceWarning(("<<< %s -- ...\n", __FUNCTION__));
for(index = 0;index < g_NrMSServs; index++)
    {
     AnscTraceWarning(("<<< %s -- index %d ...\n", __FUNCTION__,index));

    if (index >= g_NrMSServs || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

     Utopia_GetMSServByIndex(&ctx, index, &ms_serv);
     memset(start_time,0,25);
     memset(end_time,0,25);
    
     AnscTraceWarning(("%s -- ms_serv.alias = %s...\n", __FUNCTION__, ms_serv.alias));
     AnscTraceWarning(("%s -- ms_serv.start_time = %s...\n", __FUNCTION__, ms_serv.start_time));
     AnscTraceWarning(("%s -- ms_serv.end_time = %s...\n", __FUNCTION__, ms_serv.end_time));

     if(isAlwaysBlock(ms_serv.block_days,ms_serv.start_time,ms_serv.end_time))
	 {
		 ms_serv.always_block = TRUE;
		rc = Utopia_SetMSServByIndex(&ctx, index, &ms_serv);
        if (rc != 0)
	    {
	       AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
           Utopia_Free(&ctx, 0);
           return ANSC_STATUS_FAILURE;
	    }
		Utopia_Free(&ctx, !rc);
		continue;
	 }
	 if(ms_serv.always_block == TRUE)
	 {
		 Utopia_Free(&ctx, !rc);
		 continue;
	 }
     safec_rc = strcpy_s(start_time, sizeof(start_time), ms_serv.start_time);
     if(safec_rc != EOK)
     {
        ERR_CHK(safec_rc);
     }
	 safec_rc = strcpy_s(end_time, sizeof(end_time), ms_serv.end_time);
     if(safec_rc != EOK)
     {
        ERR_CHK(safec_rc);
     }
     sRet = ConvLocalToUTC(start_time, ms_serv.start_time);
     eRet = ConvLocalToUTC(end_time, ms_serv.end_time);
     AnscTraceWarning(("%s -- Converted ms_serv.start_time = %s...\n", __FUNCTION__, ms_serv.start_time));
     AnscTraceWarning(("%s -- Converted ms_serv.end_time = %s...\n", __FUNCTION__, ms_serv.end_time));
	 if(!strcmp(ms_serv.end_time,"00:00"))
	 {
		 AnscTraceWarning(("%s -- inside if ms_serv.end_time = %s eRet = %d ...\n", __FUNCTION__, ms_serv.end_time,eRet));
		 safec_rc = strcpy_s(ms_serv.end_time, sizeof(ms_serv.end_time),"23:59");
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		rc = Utopia_SetMSServByIndex(&ctx, index, &ms_serv);
        if (rc != 0)
	    {
	       AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
           Utopia_Free(&ctx, 0);
           return ANSC_STATUS_FAILURE;
	    }
		Utopia_Free(&ctx, !rc);
		continue;
	 }
     
     memset(eTime_block_days,0,64);
	 if(split_BlockDays(sRet,eRet,ms_serv.block_days,eTime_block_days))
	 {
		 AnscTraceWarning(("%s -- Day change required, split the rule  ...\n", __FUNCTION__));
		 AnscTraceWarning(("%s -- Converted block_days for first rule = %s  ...\n", __FUNCTION__, ms_serv.block_days));
		 AnscTraceWarning(("%s -- Converted block_days for Second rule = %s ...\n", __FUNCTION__,eTime_block_days));
		 // split rule timings
		 safec_rc = strcpy_s(rb_end_time, sizeof(rb_end_time), ms_serv.end_time);
		 if(safec_rc != EOK)
		 {
			ERR_CHK(safec_rc);
		 }
		 safec_rc = strcpy_s(ms_serv.end_time, sizeof(ms_serv.end_time), "23:59");
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 AnscTraceWarning(("%d -- Converted ms_serv.end_time = %s ...\n", __LINE__,ms_serv.end_time));
		 rc = Utopia_SetMSServByIndex(&ctx, index, &ms_serv);
		 if (rc != 0)
			{
			    AnscTraceWarning(("%s -- Utopia_SetMSServByIndex failed...\n",__FUNCTION__));
                Utopia_Free(&ctx, 0);
				return ANSC_STATUS_FAILURE;
			}
			safec_rc = strcpy_s(ms_serv.end_time, sizeof(ms_serv.end_time),rb_end_time);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		 	AnscTraceWarning(("%d -- Converted ms_serv.end_time = %s ...\n", __LINE__,ms_serv.end_time));
			safec_rc = strcpy_s(ms_serv.start_time, sizeof(ms_serv.start_time),"00:00");
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		    AnscTraceWarning(("%d -- Converted ms_serv.start_time = %s ...\n", __LINE__,ms_serv.start_time));
			safec_rc = strcpy_s(ms_serv.block_days, sizeof(ms_serv.block_days), eTime_block_days);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		    AnscTraceWarning(("%d -- Converted eTime_block_days = %s ...\n", __LINE__,eTime_block_days));
		    AnscTraceWarning(("%d -- Converted ms_serv.block_days = %s ...\n", __LINE__,ms_serv.block_days));

		ms_serv.ins_num = InsNum;
		rc = Utopia_AddMSServ(&ctx,&ms_serv);
			if (rc != 0)
			{
			   AnscTraceWarning(("%s -- Utopia_AddMSServ failed...\n",__FUNCTION__));

               Utopia_Free(&ctx, 0);
			   return ANSC_STATUS_FAILURE;
			}
	 }else
	 {
        rc = Utopia_SetMSServByIndex(&ctx, index, &ms_serv);
        if (rc != 0)
	    {
	       AnscTraceWarning(("%s -- Utopia_SetMSServByIndex failed...\n",__FUNCTION__));
           Utopia_Free(&ctx, 0);
           return ANSC_STATUS_FAILURE;
	    }
	 }
    AnscTraceWarning(("%s -- Converted Success ...\n", __FUNCTION__));
    Utopia_Free(&ctx, !rc);
    }
 if(g_NrMSServs)
	{
           /*CID: 72126 Unchecked return value*/
	   if(!Utopia_Init(&ctx))
               return ANSC_STATUS_FAILURE;
       rc = Utopia_GetNumberOfBlkURL(&ctx, &g_NrMSServs);
	   AnscTraceWarning(("%s -- Converted g_NrMSServs = %d ...\n", __FUNCTION__,g_NrMSServs));
       Utopia_Free(&ctx, !rc);
       if (rc != 0)
           return ANSC_STATUS_FAILURE;
	}
 return ANSC_STATUS_SUCCESS;
}
#endif
ANSC_STATUS
CosaDmlMSServ_GetEntryByIndex(ULONG index, COSA_DML_MS_SERV *pEntry)
{
    UtopiaContext ctx;
    ms_serv_t ms_serv;

    if (index >= (ULONG)g_NrMSServs || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetMSServByIndex(&ctx, index, &ms_serv);

    pEntry->InstanceNumber = ms_serv.ins_num;
    pEntry->AlwaysBlock = ms_serv.always_block;
    pEntry->StartPort = ms_serv.start_port;
    pEntry->EndPort = ms_serv.end_port;
    
    _ansc_strncpy(pEntry->Alias, ms_serv.alias, sizeof(pEntry->Alias));
    _ansc_strncpy(pEntry->Description, ms_serv.descp, sizeof(pEntry->Description));

    _ansc_strncpy(pEntry->StartTime, ms_serv.start_time, sizeof(pEntry->StartTime));
    _ansc_strncpy(pEntry->EndTime, ms_serv.end_time, sizeof(pEntry->EndTime));
    
    _ansc_strncpy(pEntry->BlockDays, ms_serv.block_days, sizeof(pEntry->BlockDays));
    
    if(!_ansc_strcmp(ms_serv.protocol, "TCP"))
        pEntry->Protocol = PROTO_TCP;
    else if(!_ansc_strcmp(ms_serv.protocol, "UDP"))
        pEntry->Protocol = PROTO_UDP;
    else
        pEntry->Protocol = PROTO_BOTH;

    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MSSERV]:%lu,%s\n", __FUNCTION__, __LINE__,pEntry->InstanceNumber,pEntry->Description));
    Utopia_Free(&ctx, 0);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_SetValues(ULONG index, ULONG ins, const char *alias)
{
    int rc = -1;
    UtopiaContext ctx;
    
    if (index >= (ULONG)g_NrMSServs || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_SetMSServInsAndAliasByIndex(&ctx, index, ins, alias);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSServ_AddEntry(COSA_DML_MS_SERV *pEntry)
{
    int rc = -1;
    UtopiaContext ctx;
  /* CID 185687 Fix - Buffer not null terminated */
    ms_serv_t ms_serv = {0};
    errno_t safec_rc = -1;
    
    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    ms_serv.ins_num  = pEntry->InstanceNumber;
    ms_serv.always_block = pEntry->AlwaysBlock;
    ms_serv.start_port = pEntry->StartPort;
    ms_serv.end_port = pEntry->EndPort;
    
    /* CID 185687 Fix - Buffer not null terminated */
    strncpy(ms_serv.alias, pEntry->Alias, sizeof(ms_serv.alias)-1);
    strncpy(ms_serv.descp, pEntry->Description, sizeof(ms_serv.descp)-1);

    strncpy(ms_serv.start_time, pEntry->StartTime, sizeof(ms_serv.start_time)-1);
    strncpy(ms_serv.end_time, pEntry->EndTime, sizeof(ms_serv.end_time)-1);
    /* CID 185686 fix */
    strncpy(ms_serv.block_days, pEntry->BlockDays, (sizeof(ms_serv.block_days)-1));

    switch(pEntry->Protocol)
    {
        case PROTO_TCP:
            safec_rc = strcpy_s(ms_serv.protocol, sizeof(ms_serv.protocol), "TCP");
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            break;
        case PROTO_UDP:
            safec_rc = strcpy_s(ms_serv.protocol, sizeof(ms_serv.protocol), "UDP");
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            break;
        case PROTO_BOTH:
            safec_rc = strcpy_s(ms_serv.protocol, sizeof(ms_serv.protocol), "BOTH");
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            break;
    }

    rc = Utopia_AddMSServ(&ctx, &ms_serv);
    Utopia_GetNumberOfMSServ(&ctx, &g_NrMSServs);
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MSSERV]:%lu,%s\n", __FUNCTION__, __LINE__, pEntry->InstanceNumber,pEntry->Description));

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlMSServ_DelEntry(ULONG ins)
{
    int rc = -1;
    UtopiaContext ctx;
    
    if (MSServ_InsGetIndex(ins) == -1 || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_DelMSServ(&ctx, ins);
    Utopia_GetNumberOfMSServ(&ctx, &g_NrMSServs);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlMSServ_GetConf(ULONG ins, COSA_DML_MS_SERV *pEntry)
{
    int index;

    if ((index = MSServ_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    return CosaDmlMSServ_GetEntryByIndex(index, pEntry);
}


ANSC_STATUS
CosaDmlMSServ_SetConf(ULONG ins, COSA_DML_MS_SERV *pEntry)
{
    int index;
    UtopiaContext ctx;
    ms_serv_t ms_serv;
    int rc =  -1;
    errno_t safec_rc = -1;

    if ((index = MSServ_InsGetIndex(ins)) == -1 || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    index = MSServ_InsGetIndex(ins);

    ms_serv.ins_num  = pEntry->InstanceNumber;
    ms_serv.always_block = pEntry->AlwaysBlock;
    ms_serv.start_port = pEntry->StartPort;
    ms_serv.end_port = pEntry->EndPort;
    
    _ansc_strncpy(ms_serv.alias, pEntry->Alias, sizeof(ms_serv.alias));
    _ansc_strncpy(ms_serv.descp, pEntry->Description, sizeof(ms_serv.descp));

    _ansc_strncpy(ms_serv.start_time, pEntry->StartTime, sizeof(ms_serv.start_time));
    _ansc_strncpy(ms_serv.end_time, pEntry->EndTime, sizeof(ms_serv.end_time));
    _ansc_strncpy(ms_serv.block_days, pEntry->BlockDays, sizeof(ms_serv.block_days));

    switch(pEntry->Protocol)
    {
        case PROTO_TCP:
            safec_rc = strcpy_s(ms_serv.protocol, sizeof(ms_serv.protocol), "TCP");
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            break;
        case PROTO_UDP:
            safec_rc = strcpy_s(ms_serv.protocol, sizeof(ms_serv.protocol), "UDP");
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            break;
        case PROTO_BOTH:
            safec_rc = strcpy_s(ms_serv.protocol, sizeof(ms_serv.protocol), "BOTH");
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            break;
    }
    
    rc = Utopia_SetMSServByIndex(&ctx, index, &ms_serv);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS; 
    }
}

/*
 * ManagedServices.TrustedUser
 */
static int g_NrMSTrustedUsers   = 0;

static int
MSTrustedUser_InsGetIndex(ULONG ins)
{
    int i, ins_num, ret = -1;
    UtopiaContext ctx;

    CosaDmlMSTrustedUser_GetNumberOfEntries();

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    for (i = 0; i < g_NrMSTrustedUsers; i++)
    {
        Utopia_GetMSTrustedUserInsNumByIndex(&ctx, i, &ins_num);
        if ((ULONG)ins_num == ins) {
            ret = i;
            break;
        }
    }

    Utopia_Free(&ctx, 0);

    return ret;
}

ULONG
CosaDmlMSTrustedUser_GetNumberOfEntries(void)
{
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetNumberOfMSTrustedUser(&ctx, &g_NrMSTrustedUsers);

    Utopia_Free(&ctx, 0);

    return g_NrMSTrustedUsers;
}

ANSC_STATUS
CosaDmlMSTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    UtopiaContext ctx;
    ms_trusteduser_t ms_trusteduser;

    if (index >= (ULONG)g_NrMSTrustedUsers || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetMSTrustedUserByIndex(&ctx, index, &ms_trusteduser);

    pEntry->InstanceNumber = ms_trusteduser.ins_num;
    pEntry->Trusted = ms_trusteduser.trusted;
    pEntry->IPAddressType = ms_trusteduser.ipaddrtype == 4 ? IPADDR_IPV4 : IPADDR_IPV6;
    _ansc_strncpy(pEntry->Alias, ms_trusteduser.alias, sizeof(pEntry->Alias));
    _ansc_strncpy(pEntry->HostDescription, ms_trusteduser.host_descp, sizeof(pEntry->HostDescription));
    _ansc_strncpy(pEntry->IPAddress, ms_trusteduser.ipaddr, sizeof(pEntry->IPAddress));
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MSTUSER]:%lu,%s\n", __FUNCTION__, __LINE__,pEntry->InstanceNumber,pEntry->IPAddress));

    Utopia_Free(&ctx, 0);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias)
{
    int rc = -1;
    UtopiaContext ctx;
    
    if (index >= (ULONG)g_NrMSTrustedUsers || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_SetMSTrustedUserInsAndAliasByIndex(&ctx, index, ins, alias);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMSTrustedUser_AddEntry(COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    int rc = -1;
    UtopiaContext ctx;
    ms_trusteduser_t ms_trusteduser = {0};
    
    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    ms_trusteduser.ins_num = pEntry->InstanceNumber;
    ms_trusteduser.trusted = pEntry->Trusted;
    ms_trusteduser.ipaddrtype = pEntry->IPAddressType == IPADDR_IPV4 ? 4 : 6;
    /* CID 185691 fix  */
    _ansc_strncpy(ms_trusteduser.alias, pEntry->Alias, (sizeof(ms_trusteduser.alias)-1));
    _ansc_strncpy(ms_trusteduser.host_descp, pEntry->HostDescription, sizeof(ms_trusteduser.host_descp)-1);
    _ansc_strncpy(ms_trusteduser.ipaddr, pEntry->IPAddress, sizeof(ms_trusteduser.ipaddr)-1);

    rc = Utopia_AddMSTrustedUser(&ctx, &ms_trusteduser);
    Utopia_GetNumberOfMSTrustedUser(&ctx, &g_NrMSTrustedUsers);
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MSTUSER]:%lu,%s\n", __FUNCTION__, __LINE__, pEntry->InstanceNumber,pEntry->IPAddress));

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlMSTrustedUser_DelEntry(ULONG ins)
{
    int rc = -1;
    UtopiaContext ctx;
    
    if (MSTrustedUser_InsGetIndex(ins) == -1 || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_DelMSTrustedUser(&ctx, ins);
    Utopia_GetNumberOfMSTrustedUser(&ctx, &g_NrMSTrustedUsers);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlMSTrustedUser_GetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    int index;

    if ((index = MSTrustedUser_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    return CosaDmlMSTrustedUser_GetEntryByIndex(index, pEntry);
}


ANSC_STATUS
CosaDmlMSTrustedUser_SetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry)
{
    int index;
    UtopiaContext ctx;
    ms_trusteduser_t ms_trusteduser;
    int rc = -1;

    if ((index = MSTrustedUser_InsGetIndex(ins)) == -1 || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    index = MSTrustedUser_InsGetIndex(ins);

    ms_trusteduser.ins_num = pEntry->InstanceNumber;
    ms_trusteduser.trusted = pEntry->Trusted;
    ms_trusteduser.ipaddrtype = pEntry->IPAddressType == IPADDR_IPV4 ? 4 : 6;
    _ansc_strncpy(ms_trusteduser.alias, pEntry->Alias, sizeof(ms_trusteduser.alias));
    _ansc_strncpy(ms_trusteduser.host_descp, pEntry->HostDescription, sizeof(ms_trusteduser.host_descp));
    _ansc_strncpy(ms_trusteduser.ipaddr, pEntry->IPAddress, sizeof(ms_trusteduser.ipaddr));

    rc = Utopia_SetMSTrustedUserByIndex(&ctx, index, &ms_trusteduser);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

/*
 * ManagedDevices.Device
 */
static int g_NrMDDevs   = 0;

static int
MDDev_InsGetIndex(ULONG ins)
{
    int i, ins_num, ret = -1;
    UtopiaContext ctx;

    CosaDmlMDDev_GetNumberOfEntries();

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    for (i = 0; i < g_NrMDDevs; i++)
    {
        Utopia_GetMDDevInsNumByIndex(&ctx, i, &ins_num);
        if((ULONG)ins_num == ins) {
            ret = i;
            break;
        }
    }

    Utopia_Free(&ctx, 0);

    return ret;
}

ULONG
CosaDmlMDDev_GetNumberOfEntries(void)
{
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetNumberOfMDDev(&ctx, &g_NrMDDevs);

    Utopia_Free(&ctx, 0);

    return g_NrMDDevs;
}
#ifdef UTC_ENABLE
int delMDDev()
{
	int i = 0;
	int rc = -1;
	UtopiaContext ctx;
        /*CID: 66110 Unchecked return value*/
	if(!Utopia_Init(&ctx))
           return rc;
	for(i = 0;i<dCnt;i++)
	{
		
    	AnscTraceWarning(("<<< %s delete ins_num  %d ...\n", __FUNCTION__,blkIns[i]));
        rc = Utopia_DelMDDev(&ctx, blkIns[i]);
	}
	Utopia_GetNumberOfMDDev(&ctx, &g_NrMDDevs);
	Utopia_Free(&ctx, !rc);
    return 0;
	
}
ANSC_STATUS CosaDmlMDDev_RollbackUTCtoLocal()
{
    UtopiaContext ctx;
    md_dev_t md_dev,tmp;
    int index =0;
	int TIndex =0;
    int rc = -1;
    char start_time[25];
    char end_time[25];
	char eTime_block_days[64];
	int sRet = 0,eRet = 0;
    errno_t safec_rc = -1;
    ResetInsBuf();
	AnscTraceWarning(("<<< %s --  g_NrBlkURL %d ...\n", __FUNCTION__,g_NrBlkURL));
    AnscTraceWarning(("<<< %s -- ...\n", __FUNCTION__));
for(index = 0;index < g_NrMDDevs; index++)
    {

        /*CID: 68927 Unchecked return value*/
	if(!Utopia_Init(&ctx))
           return ANSC_STATUS_FAILURE;
     AnscTraceWarning(("<<< %s -- Call Utopia_GetMDDevByIndex %d ...\n", __FUNCTION__,index));
	 if(ChkDelIndex(index))
	 {
		 Utopia_Free(&ctx, !rc);
		 continue;
	 }
     Utopia_GetMDDevByIndex(&ctx, index, &md_dev);
	 if(md_dev.always)
	 {
		 Utopia_Free(&ctx, !rc);
		 continue;
	 }
	 for(TIndex = index+1;TIndex <g_NrMDDevs; TIndex++)
	 {
		 if(ChkDelIndex(TIndex))
		  continue;
	  
		 Utopia_GetMDDevByIndex(&ctx, TIndex, &tmp);
		 if(!strcmp(tmp.macaddr,md_dev.macaddr))
		 {
			 if(!strcmp(tmp.descp,md_dev.descp))
			 {
				 if(tmp.always == md_dev.always)
				 {
					if(strcmp(tmp.block_days,md_dev.block_days) || (!strcmp(md_dev.block_days,"Mon,Tue,Wed,Thu,Fri,Sat,Sun")))
					 {
						 if(strstr(tmp.start_time,"00:00"))
						 {
							 if(strstr(md_dev.end_time,"23:59"))
							 {
								 AnscTraceWarning(("<<< %s -- copy end time and del rule %d ...\n", __FUNCTION__,TIndex));
								 safec_rc = strcpy_s(md_dev.end_time,sizeof(md_dev.end_time),tmp.end_time);
								 if(safec_rc != EOK)
								 {
									 ERR_CHK(safec_rc);
								 }
							     SetDelIndex(TIndex,tmp.ins_num);
								 Utopia_GetNumberOfMDDev(&ctx, &g_NrMDDevs);
							 }
						 }
						 else if(strstr(tmp.end_time,"23:59"))
						 {
							 if(strstr(md_dev.start_time,"00:00"))
							 {
								 AnscTraceWarning(("<<< %s -- copy start time and del rule %d ...\n", __FUNCTION__,TIndex));
								 safec_rc = strcpy_s(md_dev.start_time,sizeof(md_dev.start_time),tmp.start_time);
								 if(safec_rc != EOK)
								 {
									 ERR_CHK(safec_rc);
								 }
								 SetDelIndex(TIndex,tmp.ins_num);
								 Utopia_GetNumberOfMDDev(&ctx, &g_NrMDDevs);
							 }
							 
						 }		 
					 }
					 else
					 {
						 continue;
					 }
				 }
			 }
		 }
		 
	 }
     memset(start_time,0,25);
     memset(end_time,0,25);
     if(ChkDelIndex(TIndex) == 0)
	 {
		 AnscTraceWarning(("%s -- md_dev.alias = %s...\n", __FUNCTION__, md_dev.alias));
		 AnscTraceWarning(("%s -- md_dev.start_time = %s...\n", __FUNCTION__, md_dev.start_time));
		 AnscTraceWarning(("%s -- md_dev.end_time = %s...\n", __FUNCTION__, md_dev.end_time));

		 safec_rc = strcpy_s(start_time, sizeof(start_time), md_dev.start_time);
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 safec_rc = strcpy_s(end_time, sizeof(end_time), md_dev.end_time);
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 sRet = ConvUTCToLocal(start_time, md_dev.start_time);
		 eRet = ConvUTCToLocal(end_time, md_dev.end_time);
		 memset(eTime_block_days,0,64);
		 split_BlockDays(sRet,eRet,md_dev.block_days,eTime_block_days);
		 AnscTraceWarning(("%s -- Converted md_dev.start_time = %s...\n", __FUNCTION__, md_dev.start_time));
		 AnscTraceWarning(("%s -- Converted md_dev.end_time = %s...\n", __FUNCTION__, md_dev.end_time));
		 

		 rc = Utopia_SetMDDevByIndex(&ctx, index, &md_dev);
	 }
     Utopia_Free(&ctx, !rc);
    if (rc != 0)
	{
	AnscTraceWarning(("%s -- Utopia_SetMDDevByIndex failed...\n",__FUNCTION__));
        return ANSC_STATUS_FAILURE;
	}

	}
 delMDDev();
 return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS CosaDmlMDDev_Migration()
{
    UtopiaContext ctx;
    md_dev_t md_dev;
    int index =0;
    int rc = -1;
    char start_time[25];
    char end_time[25];
	char rb_end_time[25];
	int sRet = 0,eRet = 0;
	int                             InsNum                 = 0;
	char eTime_block_days[64];
    errno_t safec_rc = -1;
AnscTraceWarning(("<<< %s -- ...\n", __FUNCTION__));
for(index = 0;index < g_NrMDDevs; index++)
{
    if (index >= g_NrMDDevs || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

     Utopia_GetMDDevByIndex(&ctx, index, &md_dev);
     memset(start_time,0,25);
     memset(end_time,0,25);
    
     AnscTraceWarning(("%s -- md_dev.alias = %s...\n", __FUNCTION__, md_dev.alias));
     AnscTraceWarning(("%s -- md_dev.start_time = %s...\n", __FUNCTION__, md_dev.start_time));
     AnscTraceWarning(("%s -- md_dev.end_time = %s...\n", __FUNCTION__, md_dev.end_time));

	 if(isAlwaysBlock(md_dev.block_days,md_dev.start_time,md_dev.end_time))
	 {
		 md_dev.always = TRUE;
		rc = Utopia_SetMDDevByIndex(&ctx, index, &md_dev);
        if (rc != 0)
	    {
	       AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
           Utopia_Free(&ctx, 0);
           return ANSC_STATUS_FAILURE;
	    }
		Utopia_Free(&ctx, !rc);
		continue;
	 }
	 if(md_dev.always == TRUE)
	 {
		
		 Utopia_Free(&ctx, !rc);
		 continue;
	 }
	 
     safec_rc = strcpy_s(start_time, sizeof(start_time),md_dev.start_time);
     if(safec_rc != EOK)
     {
         ERR_CHK(safec_rc);
     }
     safec_rc = strcpy_s(end_time, sizeof(end_time), md_dev.end_time);
     if(safec_rc != EOK)
     {
         ERR_CHK(safec_rc);
     }
     sRet = ConvLocalToUTC(start_time, md_dev.start_time);
     eRet = ConvLocalToUTC(end_time, md_dev.end_time);
     AnscTraceWarning(("%s -- Converted md_dev.start_time = %s...\n", __FUNCTION__, md_dev.start_time));
     AnscTraceWarning(("%s -- Converted md_dev.end_time = %s...\n", __FUNCTION__, md_dev.end_time));
     
	 if(!strcmp(md_dev.end_time,"00:00"))
	 {
		 AnscTraceWarning(("%s -- inside if md_dev.end_time = %s eRet = %d ...\n", __FUNCTION__, md_dev.end_time,eRet));
		 safec_rc = strcpy_s(md_dev.end_time,sizeof(md_dev.end_time),"23:59");
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		rc = Utopia_SetMDDevByIndex(&ctx, index, &md_dev);
        if (rc != 0)
	    {
	       AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
           Utopia_Free(&ctx, 0);
           return ANSC_STATUS_FAILURE;
	    }
		Utopia_Free(&ctx, !rc);
		continue;
	 }
	 
     memset(eTime_block_days,0,64);
	 if(split_BlockDays(sRet,eRet,md_dev.block_days,eTime_block_days))
	 {
		 AnscTraceWarning(("%s -- Day change required, split the rule  ...\n", __FUNCTION__));
		 AnscTraceWarning(("%s -- Converted block_days for first rule = %s  ...\n", __FUNCTION__, md_dev.block_days));
		 AnscTraceWarning(("%s -- Converted block_days for Second rule = %s ...\n", __FUNCTION__,eTime_block_days));
		 // split rule timings
		 safec_rc = strcpy_s(rb_end_time,sizeof(rb_end_time),md_dev.end_time);
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 safec_rc = strcpy_s(md_dev.end_time,sizeof(md_dev.end_time),"23:59");
		 if(safec_rc != EOK)
		 {
			 ERR_CHK(safec_rc);
		 }
		 AnscTraceWarning(("%d -- Converted md_dev.end_time = %s ...\n", __LINE__,md_dev.end_time));
		 rc = Utopia_SetMDDevByIndex(&ctx, index, &md_dev);
		 if (rc != 0)
			{
			    AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
                Utopia_Free(&ctx, 0);
				return ANSC_STATUS_FAILURE;
			}
		    safec_rc = strcpy_s(md_dev.end_time,sizeof(md_dev.end_time),rb_end_time);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		 	AnscTraceWarning(("%d -- Converted md_dev.end_time = %s ...\n", __LINE__,md_dev.end_time));
		    safec_rc = strcpy_s(md_dev.start_time,sizeof(md_dev.start_time),"00:00");
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		    AnscTraceWarning(("%d -- Converted md_dev.start_time = %s ...\n", __LINE__,md_dev.start_time));
		    safec_rc = strcpy_s(md_dev.block_days,sizeof(md_dev.block_days),eTime_block_days);
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		    AnscTraceWarning(("%d -- Converted eTime_block_days = %s ...\n", __LINE__,eTime_block_days));
		    AnscTraceWarning(("%d -- Converted md_dev.block_days = %s ...\n", __LINE__,md_dev.block_days));

		md_dev.ins_num = InsNum;
		rc = Utopia_AddMDDev(&ctx,&md_dev);
			if (rc != 0)
			{
			   AnscTraceWarning(("%s -- Utopia_AddMDDev failed...\n",__FUNCTION__));
               Utopia_Free(&ctx, 0);
			   return ANSC_STATUS_FAILURE;
			}
	 }else
	 {
        rc = Utopia_SetMDDevByIndex(&ctx, index, &md_dev);
        if (rc != 0)
	    {
	       AnscTraceWarning(("%s -- SetBlkURLByIndex failed...\n",__FUNCTION__));
           Utopia_Free(&ctx, 0);
           return ANSC_STATUS_FAILURE;
	    }
	 }
    AnscTraceWarning(("%s -- Converted Success ...\n", __FUNCTION__));
    Utopia_Free(&ctx, !rc);
    }
 if(g_NrMDDevs)
	{
           /* CID: 55877 Unchecked return value*/
	   if(!Utopia_Init(&ctx))
              return ANSC_STATUS_FAILURE;
       rc = Utopia_GetNumberOfMDDev(&ctx, &g_NrMDDevs);
	   AnscTraceWarning(("%s -- Converted g_NrMDDevs = %d ...\n", __FUNCTION__,g_NrMDDevs));
       Utopia_Free(&ctx, !rc);
       if (rc != 0)
           return ANSC_STATUS_FAILURE;
	}
 return ANSC_STATUS_SUCCESS;
}
#endif
ANSC_STATUS
CosaDmlMDDev_GetEntryByIndex(ULONG index, COSA_DML_MD_DEV *pEntry)
{
    UtopiaContext ctx;
    md_dev_t md_dev;
    int i,j,size;
    char mac_addr[64];

    if (index >= (ULONG)g_NrMDDevs || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetMDDevByIndex(&ctx, index, &md_dev);

    pEntry->InstanceNumber = md_dev.ins_num;
    pEntry->Type = md_dev.is_block ? MD_TYPE_BLOCK : MD_TYPE_ALLOW;
    pEntry->AlwaysBlock = md_dev.always; 
    _ansc_strncpy(pEntry->Alias, md_dev.alias, sizeof(pEntry->Alias));
    _ansc_strncpy(pEntry->Description, md_dev.descp, sizeof(pEntry->Description));

    _ansc_strncpy(pEntry->StartTime, md_dev.start_time, sizeof(pEntry->StartTime));
    _ansc_strncpy(pEntry->EndTime, md_dev.end_time, sizeof(pEntry->EndTime));
    
    _ansc_strncpy(pEntry->BlockDays, md_dev.block_days, sizeof(pEntry->BlockDays));
    _ansc_strncpy(pEntry->MACAddress, md_dev.macaddr, sizeof(pEntry->MACAddress));
    j = 0;
    size = strlen(pEntry->MACAddress);
    for (i = 0; i < size; i++) {
        if (pEntry->MACAddress[i] != ':'){
            char ch1 = pEntry->MACAddress[i];
            mac_addr[j] = ch1;
            j++;
        }
    }
    mac_addr[j] = '\0';
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MDDEV]:%lu,%s\n", __FUNCTION__, __LINE__,pEntry->InstanceNumber,mac_addr));

    Utopia_Free(&ctx, 0);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_SetValues(ULONG index, ULONG ins, const char *alias)
{
    int rc = -1;
    UtopiaContext ctx;
    
    if (index >= (ULONG)g_NrMDDevs || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_SetMDDevInsAndAliasByIndex(&ctx, index, ins, alias);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMDDev_AddEntry(COSA_DML_MD_DEV *pEntry)
{
    int rc = -1,i,j,size;
    char mac_addr[64];
    UtopiaContext ctx;
    md_dev_t md_dev;
    
    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    md_dev.ins_num  = pEntry->InstanceNumber;
    md_dev.is_block = pEntry->Type == MD_TYPE_BLOCK ? 1 : 0;
    md_dev.always = pEntry->AlwaysBlock;
    _ansc_strncpy(md_dev.alias, pEntry->Alias, sizeof(md_dev.alias));
    _ansc_strncpy(md_dev.descp, pEntry->Description, sizeof(md_dev.descp));

    _ansc_strncpy(md_dev.start_time, pEntry->StartTime, sizeof(md_dev.start_time));
    _ansc_strncpy(md_dev.end_time, pEntry->EndTime, sizeof(md_dev.end_time));
    _ansc_strncpy(md_dev.block_days, pEntry->BlockDays, sizeof(md_dev.block_days));
    /* CID 185690 fix  */
    _ansc_strncpy(md_dev.macaddr, pEntry->MACAddress, (sizeof(md_dev.macaddr)-1));

    rc = Utopia_AddMDDev(&ctx, &md_dev);
    Utopia_GetNumberOfMDDev(&ctx, &g_NrMDDevs);
    j = 0;
    size = strlen(pEntry->MACAddress);
    for (i = 0; i < size; i++) {
        if (pEntry->MACAddress[i] != ':'){
            char ch1 = pEntry->MACAddress[i];
            mac_addr[j] = ch1;
            j++;
        }
    }
    mac_addr[j] = '\0';
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MDDEV]:%lu,%s\n", __FUNCTION__, __LINE__, pEntry->InstanceNumber,mac_addr));

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlMDDev_DelEntry(ULONG ins)
{
    int rc = -1;
    UtopiaContext ctx;
    
    if (MDDev_InsGetIndex(ins) == -1 || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_DelMDDev(&ctx,ins);
    Utopia_GetNumberOfMDDev(&ctx, &g_NrMDDevs);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlMDDev_GetConf(ULONG ins, COSA_DML_MD_DEV *pEntry)
{
    int index;

    if ((index = MDDev_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    return CosaDmlMDDev_GetEntryByIndex(index, pEntry);
}

ANSC_STATUS
CosaDmlMDDev_SetConf(ULONG ins, COSA_DML_MD_DEV *pEntry)
{
    int index;
    UtopiaContext ctx;
    md_dev_t md_dev;
    int rc =  -1;

    if ((index = MDDev_InsGetIndex(ins)) == -1 || !Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    index = MDDev_InsGetIndex(ins);

    md_dev.ins_num  = pEntry->InstanceNumber;
    md_dev.is_block = pEntry->Type == MD_TYPE_BLOCK ? 1 : 0;
    md_dev.always = pEntry->AlwaysBlock;
    _ansc_strncpy(md_dev.alias, pEntry->Alias, sizeof(md_dev.alias));
    _ansc_strncpy(md_dev.descp, pEntry->Description, sizeof(md_dev.descp));

    _ansc_strncpy(md_dev.start_time, pEntry->StartTime, sizeof(md_dev.start_time));
    _ansc_strncpy(md_dev.end_time, pEntry->EndTime, sizeof(md_dev.end_time));
    _ansc_strncpy(md_dev.block_days, pEntry->BlockDays, sizeof(md_dev.block_days));
    _ansc_strncpy(md_dev.macaddr, pEntry->MACAddress, sizeof(md_dev.macaddr));
    
    rc = Utopia_SetMDDevByIndex(&ctx, index, &md_dev);

    Utopia_Free(&ctx, !rc);
    
    if (rc != 0)
        return ANSC_STATUS_FAILURE;
    else
    {
        commonSyseventSet("firewall-restart", "");
        return ANSC_STATUS_SUCCESS;
    }
}

#endif /* _COSA_SIM_ */
