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

    module: cosa_nat_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlNatGetPortMappingNumber
        *  CosaDmlNatIsUpdatedPortMappings
        *  CosaDmlNatSynchronizePortMappings
        *  CosaDmlNatGetPortMappings
        *  CosaDmlNatAddPortMapping
        *  CosaDmlNatDelPortMapping
        *  CosaDmlNatSetPortMapping
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "cosa_apis.h"
#include "cosa_nat_apis.h"
#include "cosa_nat_internal.h"
#include "plugin_main_apis.h"
#include "dml_tr181_custom_cfg.h"
#include "safec_lib_common.h"
#include "secure_wrapper.h"

#ifdef _BWG_PRODUCT_REQ_
//CGWTDETS-8800 : Usable Statics will no longer support 1-1 NAT :: START
#define max_size 3   //Max size of Exclusion list
//CGWTDETS-8800 : Usable Statics will no longer support 1-1 NAT :: END
#endif


#if defined (MULTILAN_FEATURE)
#define MAX_QUERY 1024
#define IP_INTERFACE_COUNT_DML "Device.IP.InterfaceNumberOfEntries"
#define DHCPV4_SERVER_POOL_COUNT_DML "Device.DHCPv4.Server.PoolNumberOfEntries"
#define IP_INTERFACE_DML "Device.IP.Interface."
#define DHCPV4_SERVER_POOL_DML "Device.DHCPv4.Server.Pool."
#endif

PFN_COSA_DML_NAT_GEN   g_nat_pportmapping_callback = NULL;

extern void* g_pDslhDmlAgent;

#ifdef _BWG_PRODUCT_REQ_
int updateExclusionList(char* ip, int index, int action);
void handleExclusion(int count);
#endif

#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
#define BUFLEN_8 8
#define BUFLEN_32 32
#define BUFLEN_256 256
#define SET "set"
#define RET_ERR -1

#define SYSEVENT_MAPT_CONFIG_FLAG "mapt_config_flag"
#define SYSEVENT_MAPT_IP_ADDRESS "mapt_ip_address"

#ifdef FEATURE_MAPT_DEBUG
void logPrintIvictl(char* filename, int line, char *fmt,...);
#define LOG_PRINT_IVICTL(...) logPrintIvictl(__FILE__, __LINE__, __VA_ARGS__ )
#endif

static int delete_mapt_pf_single_rule(int internal_port, int external_port, int protocol, char* dest_ip);
static int delete_mapt_pf_range_rule(int start_port, int end_port, int protocol, char* dest_ip);

#endif
#endif
#endif

#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
#ifdef FEATURE_MAPT_DEBUG
void logPrintIvictl(char* filename, int line, char *fmt,...)
{
    static FILE *fpMaptLogFile;
    static char strMaptLogFileName[32] = "/tmp/ivirule_del.txt";
    va_list         list;
    char            *p, *r;
    time_t ctime;
    int     e;
    struct tm *info;

    fpMaptLogFile = fopen(strMaptLogFileName,"a");

    time(&ctime); /* Get current time */
    info = localtime(&ctime);

    fprintf(fpMaptLogFile,"[%02d:%02d:%02d][line:%d] ",
        info->tm_hour,info->tm_min,info->tm_sec,line);

    va_start( list, fmt );

    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )
        {
            fputc( *p,fpMaptLogFile );
        }
        else
        {
            switch ( *++p )
            {

            case 's':
            {
                r = va_arg( list, char * );

                fprintf(fpMaptLogFile,"%s", r);
                continue;
            }

            case 'd':
            {
                e = va_arg( list, int );

                fprintf(fpMaptLogFile,"%d", e);
                continue;
            }

            default:
                fputc( *p, fpMaptLogFile );
            }
        }
    }
    va_end( list );
    fputc( '\n', fpMaptLogFile );
    fclose(fpMaptLogFile);
}
#endif
#endif
#endif
#endif


#if ( defined _COSA_SIM_ )

COSA_DML_NAT_PMAPPING   g_nat_portmapping[] =
    {
        {
            0,
            "",
            TRUE,
            COSA_DML_NAT_STATUS_Enabled,
            "wan0",
            TRUE,
            232332,
            {
                "\x40\x40\x40\x01"
            },
            80,
            80,
            22222,
            2,    /* UDP */
            {
                "\xc0\xa8\x01\x01"
            },
            "this is for vod"
        },
        {
            0,
            "",
            TRUE,
            COSA_DML_NAT_STATUS_Enabled,
            "wan0",
            TRUE,
            21222,
            {
                "\x40\x40\x40\x02"
            },
            21,
            21,
            22222,
            1,   /* TCP */
            {
                "\xc0\xa8\x01\x02"
            },
            "this is for ftp"
        }
    };

COSA_DML_NAT_DMZ    g_Dmz =
    {
        TRUE,
        "10.10.10.10",
        "10.10.10.11",
        "192.168.1.66"
    };

ANSC_STATUS
CosaDmlNatGetLanIP
    (
        ANSC_HANDLE                 hContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        BOOL
        CosaDmlNatInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext,
                PFN_COSA_DML_NAT_GEN        pValueGenFn
            );

        Description:
            This is the initialization routine for NAT backend.

        Arguments:
            hDml               Opaque handle from DM adapter. Backend saves this handle for calling pValueGenFn.
             phContext       Opaque handle passed back from backend, needed by CosaDmlNatXyz() routines.
            pValueGenFn    Function pointer to instance number/alias generation callback.

        Return:
            Status of operation.

**********************************************************************/
ANSC_STATUS
CosaDmlNatInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext,
        PFN_COSA_DML_NAT_GEN        pValueGenFn
    )
{
    g_nat_pportmapping_callback = pValueGenFn;

    return ANSC_STATUS_SUCCESS;

}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGet
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_DMZ           pDmlDmz
            )

        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/
ANSC_STATUS
CosaDmlNatGet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    )
{
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSet
            (
                ANSC_HANDLE                 hContext,

            )
        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/
ANSC_STATUS
CosaDmlNatSet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGetDmz
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_DMZ           pDmlDmz
            )

        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/

ANSC_STATUS
CosaDmlNatGetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmlDmz
    )
{
    AnscCopyMemory(pDmlDmz, &g_Dmz, sizeof(COSA_DML_NAT_DMZ));

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSetDmz
            (
                ANSC_HANDLE                 hContext,

            )
        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/

ANSC_STATUS
CosaDmlNatSetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmz
    )
{
    AnscCopyMemory(&g_Dmz, pDmz, sizeof(COSA_DML_NAT_DMZ));

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_NAT_PMAPPING
        CosaDmlNatGetPortMappings
            (
                ANSC_HANDLE                 hContext,
                PULONG                          pulCount
            )
        Description:
            This routine is to retrieve the complete list of NAT port mappings, which is a dynamic table.

        Arguments:
            pulCount  is to receive the actual number of entries.

        Return:
            The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_NAT_PMAPPING
CosaDmlNatGetPortMappings
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    PCOSA_DML_NAT_PMAPPING pNatPMapping = NULL;
    ULONG                         index = 0;

    for(index =0; index < sizeof(g_nat_portmapping)/sizeof(COSA_DML_NAT_PMAPPING); index++)
    {
        if ( g_nat_portmapping[index].InstanceNumber  ==  0 )
        {
            g_nat_pportmapping_callback( NULL, &g_nat_portmapping[index] );

        }
    }

    pNatPMapping = (PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory( sizeof(g_nat_portmapping) );

    if ( pNatPMapping )
    {
         AnscCopyMemory(pNatPMapping, g_nat_portmapping, sizeof(g_nat_portmapping) );

        *pulCount = 2;
    }
    else
    {
        *pulCount = 0;
    }

    return pNatPMapping;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatAddPortMapping
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_NAT_PMAPPING      pEntry
            )

    Description:
        The API adds one port mapping entry into NAT.

    Arguments:
        pEntry      Caller does not need to fill in Status or Alias fields. Upon return, callee fills in the generated Alias and associated Status.

    Return:
        he pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatAddPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry
    )
{

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatDelPortMapping
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    Description:
        The API removes the designated port mapping entry from NAT.
    Arguments:
        pAlias      The entry is identified through Alias.
    Return:
        The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatDelPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGetPortMappings
            (
                ANSC_HANDLE                 hThisObject
            );

    Description:
        The API re-configures the designated port mapping entry.
    Arguments:
        pAlias      The entry is identified through Alias.
        pEntry      The new configuration is passed through this argument, even Alias field can be changed.

    Return:
    The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatSetPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry          /* Identified by InstanceNumber */
    )
{
    ULONG                          index = 0;

    for(index =0; index < sizeof(g_nat_portmapping)/sizeof(COSA_DML_NAT_PMAPPING); index++)
    {
        if ( g_nat_portmapping[index].InstanceNumber  ==  pEntry->InstanceNumber )
        {
            AnscCopyMemory(&g_nat_portmapping[index], pEntry, sizeof(COSA_DML_NAT_PMAPPING));

            break;
        }
    }

    return ANSC_STATUS_SUCCESS;

}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_NAT_PTRIGGER
        CosaDmlNatGetPortTriggers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount,
                BOOLEAN                     bCommit
            )
        Description:
            This routine is to retrieve the complete list of NAT port triggers, which is a table.

        Arguments:
            pulCount  is to receive the actual number of entries.

        Return:
            The pointer to the array of NAT port triggers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/

PCOSA_DML_NAT_PTRIGGER
CosaDmlNatGetPortTriggers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        BOOLEAN                     bCommit
    )
{
    *pulCount = 0;
    return NULL;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatAddPortTrigger
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            )

    Description:
        The API adds one port trigger entry into NAT.

    Arguments:
        pEntry      Caller does not need to fill in Status or Alias fields. Upon return, callee fills in the generated Alias and associated Status.

    Return:
        Status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlNatAddPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatDelPortTrigger
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            );

    Description:
        The API removes the designated port trigger entry from NAT.
    Arguments:
        pAlias      The entry is identified through Alias.
    Return:
        Status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlNatDelPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSetPortTrigger
            (
                ANSC_HANDLE                 hThisObject,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            );

    Description:
        The API re-configures the designated port mapping entry.
    Arguments:
        pAlias      The entry is identified through Alias.
        pEntry      The new configuration is passed through this argument, even Alias field can be changed.

    Return:
        Status of the operation

**********************************************************************/

ANSC_STATUS
CosaDmlNatSetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry          /* Identified by InstanceNumber */
    )
{
    return ANSC_STATUS_SUCCESS;
}



static BOOL g_NatPTTriggerEnable = TRUE;

ANSC_STATUS
CosaDmlNatGetPortTriggerEnable(BOOL *pBool)
{
    *pBool = g_NatPTTriggerEnable;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlNatSetPortTriggerEnable(BOOL vBool)
{
    g_NatPTTriggerEnable = vBool;
    return ANSC_STATUS_SUCCESS;
}

#elif defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)

#include <utctx/utctx.h>
#include <utctx/utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <ccsp_syslog.h>


#define U_2_SB_PF_PROTOCOL(x)  ((x) == TCP ? 1 : ((x) == UDP ? 2 : 3) ) 
#define SB_2_U_PF_PPOTOCOL(x)  ((x) == 1 ? TCP :((x) == 2 ? UDP : BOTH_TCP_UDP) )

ULONG *g_NatPortFwdSingleInstanceNum = NULL;
ULONG *g_NatPortFwdRangeInstanceNum = NULL;
ULONG *g_NatPortFwdDynInstanceNum = NULL;
ULONG  g_NatPortFwdDynInstanceNumCount = 0;
static BOOL g_NatPTTriggerEnable;
static BOOL g_NatPFEnable;
static BOOL g_NatHSPFEnable;
static BOOL g_NatOne2OneEnable;

ANSC_IPV4_ADDRESS  g_NatLanIP = {{0}};

#if defined _DT_WAN_Manager_Enable_
static ULONG
saveID
    (
        UtopiaContext* utctx,
        int external_port,
        int proto,
        char* pAlias,
        ULONG ulInstanceNumber
    )
{
    char keyStr[32];
    char idStr[128];
    errno_t safec_rc = -1;
	
    if ( proto < 0 )
    {
        proto = 0;
    }

    safec_rc = sprintf_s(keyStr, sizeof(keyStr), "%d,%d", external_port, proto);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = sprintf_s(idStr, sizeof(idStr), "%s,%u", pAlias,ulInstanceNumber);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    Utopia_RawSet(utctx,COSA_NAT_ID_SYSCFG_NAMESPACE,keyStr,idStr);

    return 0;
}

static ULONG
loadID
    (
        UtopiaContext *utctx,
        int external_port,
        int proto,
        char* pAlias,
        ULONG* ulInstanceNumber
    )
{
    char keyStr[32];
    char idStr[128];
    char* instNumString;
    int rv;
    errno_t safec_rc = -1;

    if ( proto < 0 )
    {
        proto = 0;
    }

    safec_rc = sprintf_s(keyStr, sizeof(keyStr), "%d,%d", external_port, proto);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }

    idStr[0] = 0;
    rv =Utopia_RawGet(utctx, COSA_NAT_ID_SYSCFG_NAMESPACE, keyStr, idStr, sizeof(idStr));
    if (rv == -1 || idStr[0] == '\0') {
        return -1;
    }
    instNumString=idStr + AnscSizeOfToken(idStr, ",", sizeof(idStr))+1;
    *(instNumString-1)='\0';

    AnscCopyString(pAlias, idStr);
    *ulInstanceNumber = AnscGetStringUlong(instNumString);

    return 0;
}
#endif

/*
static ULONG
unsetID
    (
        UtopiaContext *utctx,
        int external_port,
        int proto
    )
{
    char keyStr[32];
    errno_t safec_rc = -1;

    if ( proto < 0 )
    {
        proto = 0;
    }

    safec_rc = sprintf_s(keyStr, sizeof(keyStr), "%d,%d", external_port, proto);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
	
    Utopia_RawSet(utctx, COSA_NAT_ID_SYSCFG_NAMESPACE, keyStr, "");

    return 0;
}
*/

/* InstanceNum save/load for PortTrigger entries */
/*
static ULONG
saveIDPt
    (
        UtopiaContext* utctx,
        char* pAlias,
        ULONG ulInstanceNumber
    )
{
    char idStr[32];

    errno_t safec_rc = sprintf_s(idStr,  sizeof(idStr), "%u", ulInstanceNumber);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
	
    Utopia_RawSet(utctx,COSA_NAT_ID_SYSCFG_NAMESPACE,pAlias,idStr);

    return 0;
}

static ULONG
loadIDPt
    (
        UtopiaContext *utctx,
        char* pAlias,
        ULONG* pulInstanceNumber
    )
{
    char idStr[128];
    int rv;

    idStr[0] = 0;
    rv =Utopia_RawGet(utctx, COSA_NAT_ID_SYSCFG_NAMESPACE, pAlias, idStr, sizeof(idStr));

    if (rv == -1 || idStr[0] == '\0')
    {
        return -1;
    }

    *pulInstanceNumber = AnscGetStringUlong(idStr);

    return 0;
}

static ULONG
unsetIDPt
    (
        UtopiaContext *utctx,
        char* pAlias
    )
{

    Utopia_RawSet(utctx, COSA_NAT_ID_SYSCFG_NAMESPACE, pAlias, "");

    return 0;
}
*/
#define trace(x, argv...)

#define UPDATE_PF_SINGLE_RULE_ENABLE(COUNT, DATA, PF_ENABLE,HS_ENABLE) {  \
    int i;  \
    for(i = 0; i < COUNT; i++){  \
        if(DATA[i].internal_port == 0)   \
    	{ \
            if( 1 == PF_ENABLE )\
        	{ \
				DATA[i].enabled = DATA[i].prevRuleEnabledState; \
        	} \
			else \
			{ \
				DATA[i].enabled = (PF_ENABLE == -1 ? (DATA[i].enabled) : PF_ENABLE); \
			} \
    	} \
        else    \
    	{ \
            if( 1 == HS_ENABLE )\
        	{ \
				DATA[i].enabled = DATA[i].prevRuleEnabledState; \
        	} \
			else \
			{ \
				DATA[i].enabled = (HS_ENABLE == -1 ? (DATA[i].enabled) : HS_ENABLE); \
			} \
    	} \
    }   \
}

#define UPDATE_PF_RANGE_RULE_ENABLE(COUNT, DATA, PF_ENABLE,HS_ENABLE,NAT_ENABLE) {  \
    int i;  \
    for(i = 0; i < COUNT; i++){  \
        if(strcmp("0.0.0.0", DATA[i].public_ip) != 0) \
        { \
            if( 1 == NAT_ENABLE )\
        	{ \
				DATA[i].enabled = DATA[i].prevRuleEnabledState; \
        	} \
			else \
			{ \
				DATA[i].enabled = (NAT_ENABLE == -1 ? (DATA[i].enabled) : NAT_ENABLE); \
			} \
            printf("Setting the rule as %d \n", NAT_ENABLE); \
        }else{ \
            if(DATA[i].internal_port == 0)   \
            { \
				if( 1 == PF_ENABLE )\
				{ \
					DATA[i].enabled = DATA[i].prevRuleEnabledState; \
				} \
				else \
				{ \
					DATA[i].enabled = (PF_ENABLE == -1 ? (DATA[i].enabled) : PF_ENABLE); \
				} \
            } \
            else \
            { \
				if( 1 == HS_ENABLE )\
				{ \
					DATA[i].enabled = DATA[i].prevRuleEnabledState; \
				} \
				else \
				{ \
					DATA[i].enabled = (HS_ENABLE == -1 ? (DATA[i].enabled) : HS_ENABLE); \
				} \
            } \
        } \
    }   \
}

#define PRINT_PF_SING_EACH_RULE(COUNT, DATA) {  \
    int i;  \
    for(i = 0; i < COUNT; i++){  \
           trace("@@@@@@@@@@@@@ i %d enabled %d \n",i, DATA[i].enabled); \
           trace("@@@@@@@@@@@@@ i %d name %s \n",i, DATA[i].name); \
           trace("@@@@@@@@@@@@@ i %d ruleid %d \n",i, DATA[i].rule_id); \
           trace("@@@@@@@@@@@@@ i %d proto %d \n",i, DATA[i].protocol); \
           trace("@@@@@@@@@@@@@ i %d external_port %d \n",i, DATA[i].external_port); \
           trace("@@@@@@@@@@@@@ i %d inter port %d \n",i, DATA[i].internal_port); \
           trace("@@@@@@@@@@@@@ i %d ip %x \n",i, DATA[i].dest_ip); \
    }   \
}

#define PRINT_PF_EACH_RULE(COUNT, DATA) {  \
    int i;  \
    for(i = 0; i < COUNT; i++){  \
           trace("@@@@@@@@@@@@@ i %d enabled %d \n",i, DATA[i].enabled); \
           trace("@@@@@@@@@@@@@ i %d name %s \n",i, DATA[i].name); \
           trace("@@@@@@@@@@@@@ i %d ruleid %d \n",i, DATA[i].rule_id); \
           trace("@@@@@@@@@@@@@ i %d proto %d \n",i, DATA[i].protocol); \
           trace("@@@@@@@@@@@@@ i %d external_port %d \n",i, DATA[i].start_port); \
           trace("@@@@@@@@@@@@@ i %d end port %d \n",i, DATA[i].end_port); \
           trace("@@@@@@@@@@@@@ i %d inter port %d \n",i, DATA[i].internal_port); \
           trace("@@@@@@@@@@@@@ i %d internal_port_range_size %d \n",i, DATA[i].internal_port_range_size); \
           trace("@@@@@@@@@@@@@ i %d ip %x \n",i, DATA[i].dest_ip); \
    }   \
}

ANSC_STATUS _Update_NAT_PF_HS_Enable(UtopiaContext *pCtx, int pfEnable, int HSEnable, int natEnable)
{
    int rc;
    portFwdSingle_t         *singleInfo = NULL;
    portFwdRange_t           *rangeInfo = NULL;
    int            PortFwdSingleCount = 0;
    int             PortFwdRangeCount = 0;

    rc = Utopia_GetPortForwardingRange(pCtx, &PortFwdRangeCount, &rangeInfo);
    if ( rc != SUCCESS )
    {
        CcspTraceWarning(("Utopia_GetPortForwardingRange failed rc %d in %s\n", rc, __FUNCTION__));
        PortFwdRangeCount = 0;
        rangeInfo = NULL;
    }

    rc = Utopia_GetPortForwarding(pCtx, &PortFwdSingleCount, &singleInfo);
    if ( rc != SUCCESS )
    {
        CcspTraceWarning(("Utopia_GetPortForwarding failed rc %d in %s\n", rc, __FUNCTION__));
        PortFwdSingleCount = 0;
        singleInfo = NULL;
    }

    PRINT_PF_SING_EACH_RULE(PortFwdSingleCount, singleInfo);
    PRINT_PF_EACH_RULE(PortFwdRangeCount, rangeInfo);
    /* change rule status */
    UPDATE_PF_RANGE_RULE_ENABLE(PortFwdRangeCount, rangeInfo, pfEnable,HSEnable, natEnable);
    UPDATE_PF_SINGLE_RULE_ENABLE(PortFwdSingleCount, singleInfo, pfEnable,HSEnable);

    PRINT_PF_EACH_RULE(PortFwdRangeCount, rangeInfo);
    PRINT_PF_SING_EACH_RULE(PortFwdSingleCount, singleInfo);

    if(PortFwdSingleCount != 0)
    {
        rc = Utopia_SetPortForwarding(pCtx, PortFwdSingleCount, singleInfo);
        if(rc != SUCCESS)
        {
            CcspTraceWarning(("Utopia_SetPortForwarding failed rc %d in %s\n", rc, __FUNCTION__));
        }
        free(singleInfo);
    }
   
    if(PortFwdRangeCount != 0)
    {
        Utopia_SetPortForwardingRange(pCtx, PortFwdRangeCount, rangeInfo);
        if(rc != SUCCESS)
        {
            CcspTraceWarning(("Utopia_SetPortForwardingRange failed rc %d in %s\n", rc, __FUNCTION__));
        }
        free(rangeInfo);
    }

    if( pfEnable != -1 )
    {
        rc = Utopia_RawSet(pCtx, COSA_NAT_SYSCFG_NAMESPACE, PORT_FORWARD_ENABLED_KEY, ( pfEnable == 1) ? "1" : "0");
        if (rc != 1 )
        {
            CcspTraceWarning(("Utopia_RawSet failed rc %d in %s\n", rc, __FUNCTION__));
        }
        else
        {
            g_NatPFEnable = pfEnable;
        }
    }

    if( HSEnable != -1 )
    {
        rc = Utopia_RawSet(pCtx, COSA_NAT_SYSCFG_NAMESPACE, HS_PORT_FORWARD_ENABLED_KEY, ( HSEnable == 1) ? "1" : "0");
        if ( rc != 1 )
        {
            CcspTraceWarning(("Utopia_RawSet failed rc %d in %s\n", rc, __FUNCTION__));
        }
        else
        {
            g_NatHSPFEnable = HSEnable;
        }
    }
    if( natEnable != -1 )
    {
        rc = Utopia_RawSet(pCtx, COSA_NAT_SYSCFG_NAMESPACE, NAT_FORWARD_ENABLED_KEY, ( natEnable == 1) ? "1" : "0");
        if ( rc != 1 )
        {
            CcspTraceWarning(("Utopia_RawSet failed rc %d in %s\n", rc, __FUNCTION__));
        }
        else
        {
            g_NatOne2OneEnable = natEnable;
        }
    }
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGet
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_DMZ           pDmlDmz
            )

        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/
ANSC_STATUS
CosaDmlNatGet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    )
{
    UNREFERENCED_PARAMETER(hContext);
    COSA_DML_NAT_CUSTOM_SET_ENABLEHSPORTMAPPING(pDmlNat, g_NatHSPFEnable);
	COSA_DML_NAT_CUSTOM_SET_ENABLEPORTMAPPING(pDmlNat, g_NatPFEnable);
    COSA_DML_NAT_CUSTOM_SET_ENABLENATMAPPING(pDmlNat, g_NatOne2OneEnable);
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSet
            (
                ANSC_HANDLE                 hContext,

            )
        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/
ANSC_STATUS
CosaDmlNatSet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext Ctx;
    int pf = -1, hs = -1, nat = -1;
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceError(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    
    if(COSA_DML_NAT_CUSTOM_GET_ENABLEPORTMAPPING(pDmlNat) != g_NatPFEnable)
    {
        pf = COSA_DML_NAT_CUSTOM_GET_ENABLEPORTMAPPING(pDmlNat);
    }
    
    if(COSA_DML_NAT_CUSTOM_GET_ENABLEHSPORTMAPPING(pDmlNat) != g_NatHSPFEnable)
    {
        hs = COSA_DML_NAT_CUSTOM_GET_ENABLEHSPORTMAPPING(pDmlNat);
    }
    if(COSA_DML_NAT_CUSTOM_GET_ENABLENATMAPPING(pDmlNat) != g_NatOne2OneEnable)
    {
        nat = COSA_DML_NAT_CUSTOM_GET_ENABLENATMAPPING(pDmlNat);
    }
    if(pf != -1 || hs != -1 || nat != -1)
    {
        _Update_NAT_PF_HS_Enable(&Ctx, pf, hs, nat);
        Utopia_Free(&Ctx, 1);
    }
    else
    {
        CcspTraceInfo(("%s nothing to set\n", __FUNCTION__));
        Utopia_Free(&Ctx, 0);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlNatGetLanIP
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext          Ctx;
    lanSetting_t           lan;
    ULONG                  rc, i;

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));

        return ANSC_STATUS_FAILURE;
    }

    rc = Utopia_GetLanSettings(&Ctx, &lan);

    if (SUCCESS == rc)
    {
        char* pCh    = NULL;
        char* pStart = lan.ipaddr;

        for ( i = 0; i < IPV4_ADDRESS_SIZE; i++ )
        {
            pCh = _ansc_strchr(pStart, '.');

            if ( pCh )
            {
                *pCh = 0;
                g_NatLanIP.Dot[i] = _ansc_atoi(pStart);
                pStart = pCh + 1;
            }
            else
            {
                g_NatLanIP.Dot[i] = _ansc_atoi(pStart);
            }
        }
    }

    Utopia_Free(&Ctx, 0);

    return (rc == SUCCESS) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE;
}


#if defined(FEATURE_MAPT) || defined(FEATURE_SUPPORT_MAPT_NAT46)
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGetActiveIPv4TcpInternalPorts
            (
                int*                       nports
            )
        Description:
            This routine is to retrieve the info of Number of ActiveIPv4TcpInternalPorts.

        Arguments:
            nports is the pointer to return the port info.

        Return:
            Success/Failure.

**********************************************************************/

ANSC_STATUS
CosaDmlNatGetActiveIPv4TcpInternalPorts
    (
        int*                       nports
    )
{
    FILE *fp = NULL;
    char buffer[20] = {0};

    if ((fp = v_secure_popen("r","conntrack -L -p TCP | wc -l")))
    {
        if (fgets(buffer, sizeof(buffer), fp))
        {
            buffer[strlen(buffer) - 1] = '\0';
        }
        v_secure_pclose(fp);
    }

    *nports = _ansc_atoi(buffer);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGetActiveIPv4UdpInternalPorts
            (
               int*                       nports
            )
        Description:
            This routine is to retrieve the info of Number of ActiveIPv4UdpInternalPorts.

        Arguments:
            nports is the pointer to return the port info.

        Return:
            Success/Failure.

**********************************************************************/

ANSC_STATUS
CosaDmlNatGetActiveIPv4UdpInternalPorts
    (
        int*                       nports
    )
{
    FILE *fp = NULL;
    char buffer[20] = {0};

    if ((fp = v_secure_popen("r","conntrack -L -p UDP | wc -l")))
    {
        if (fgets(buffer, sizeof(buffer), fp))
        {
            buffer[strlen(buffer) - 1] = '\0';
        }
        v_secure_pclose(fp);
    }

    *nports = _ansc_atoi(buffer);

    return ANSC_STATUS_SUCCESS;
}
#endif

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGetDmz
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_DMZ           pDmlDmz
            )
        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/

ANSC_STATUS
CosaDmlNatGetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmlDmz
    )
{
    UNREFERENCED_PARAMETER(hContext);
    dmz_t*                          pDmz            = (dmz_t*           )NULL;
    ULONG                           rc              = 0;
    /*ANSC_IPV4_ADDRESS               InternalIP      = {0};
      char*                           pInternalIPStr  = NULL;*/
    UtopiaContext                   Ctx;
    errno_t                         safec_rc        = -1;
    if ( !pDmlDmz )
    {
        CcspTraceWarning(("CosaDmlNatGetDmz pDmlDmz is NULL!\n"));

        return ANSC_STATUS_FAILURE;
    }

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));

        return ANSC_STATUS_FAILURE;
    }

    pDmz    = (dmz_t*)AnscAllocateMemory(sizeof(dmz_t));

    if ( !pDmz)
    {
        CcspTraceError(("CosaDmlNatGetDmz no memory!\n"));

        Utopia_Free(&Ctx, 0);
        return ANSC_STATUS_FAILURE;
    }

    CcspTraceInfo(("!!! Ready to Utopia_GetDMZSettings !!!\n"));

    rc = Utopia_GetDMZSettings(&Ctx, pDmz);

    CcspTraceInfo(("!!! After Utopia_GetDMZSettings !!!\n"));

    if ( rc != SUCCESS)
    {
        CcspTraceWarning(("Utopia_GetDMZSettings failed rc %lu in %s\n", rc, __FUNCTION__));
        AnscFreeMemory(pDmz);
        Utopia_Free(&Ctx, 0);
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pDmlDmz->bEnabled = pDmz->enabled;
        /*
        if ( _ansc_strlen(pDmz->source_ip_start) == 0 )
        {
             AnscCopyString(pDmlDmz->RemoteIPStart, "0.0.0.0");
        }
        else
        {
             AnscCopyString(pDmlDmz->RemoteIPStart, pDmz->source_ip_start);
        }
        if ( _ansc_strlen(pDmz->source_ip_end) == 0 )
        {
             AnscCopyString(pDmlDmz->RemoteIPEnd, "0.0.0.0");
        }
        else
        {
             AnscCopyString(pDmlDmz->RemoteIPEnd,   pDmz->source_ip_end  );
        }
        AnscCopyString(pDmlDmz->InternalMAC,   pDmz->dest_mac       );

        CcspTraceInfo(("!!!!!! NAT Lan IP: %u.%u.%u.%u pDmz->dest_ip: %d!!!!!!\n", g_NatLanIP.Dot[0], g_NatLanIP.Dot[1], g_NatLanIP.Dot[2], g_NatLanIP.Dot[3], pDmz->dest_ip));

        if ( pDmz->dest_ip != 0 )
        {
            InternalIP.Dot[0] = g_NatLanIP.Dot[0];
            InternalIP.Dot[1] = g_NatLanIP.Dot[1];
            InternalIP.Dot[2] = g_NatLanIP.Dot[2];
            InternalIP.Dot[3] = (UCHAR)pDmz->dest_ip;
        }
        else
        {
            InternalIP.Dot[0] = 0;
            InternalIP.Dot[1] = 0;
            InternalIP.Dot[2] = 0;
            InternalIP.Dot[3] = 0;
        }

        pInternalIPStr = SlapVcoIp4AddrToString(NULL, InternalIP.Value);
        */
        if (strlen(pDmz->dest_ip)){
            safec_rc = strcpy_s(pDmlDmz->InternalIP,sizeof(pDmlDmz->InternalIP), pDmz->dest_ip);  /* Todo Get Lan !!!!!!  */
            ERR_CHK(safec_rc);
        }else{
            safec_rc = strcpy_s(pDmlDmz->InternalIP,sizeof(pDmlDmz->InternalIP), "0.0.0.0");
            ERR_CHK(safec_rc);
        }

        if (strlen(pDmz->dest_ipv6)){
            safec_rc = strcpy_s(pDmlDmz->IPv6Host,sizeof(pDmlDmz->IPv6Host), pDmz->dest_ipv6);
            ERR_CHK(safec_rc);
        }else{
            //CISCOXB3-5927 : ip6 table is not getting restored
            memset(pDmlDmz->IPv6Host ,0 ,sizeof(pDmlDmz->IPv6Host));
        }

        //AnscFreeMemory(pInternalIPStr);
        AnscFreeMemory(pDmz);
        Utopia_Free(&Ctx, 0);
        return ANSC_STATUS_SUCCESS;
    }
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSetDmz
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_DMZ           pDmlDmz

            )
        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/

ANSC_STATUS
CosaDmlNatSetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmlDmz
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    dmz_t*                          pDmz            = (dmz_t*           )NULL;
    ULONG                           rc              = 0;
    errno_t                         safec_rc        = -1;
    UtopiaContext                   Ctx;

    if ( !pDmlDmz )
    {
        CcspTraceWarning(("CosaDmlNatSetDmz pDmlDmz is NULL!\n"));

        return ANSC_STATUS_FAILURE;
    }

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));

        return ANSC_STATUS_FAILURE;
    }

    pDmz    = (dmz_t*)AnscAllocateMemory(sizeof(dmz_t));

    if ( !pDmz )
    {
        CcspTraceError(("CosaDmlNatGetDmz no memory!\n"));
        Utopia_Free(&Ctx, 0);
        return ANSC_STATUS_RESOURCES;
    }

    pDmz->enabled = pDmlDmz->bEnabled;
    /*
    ANSC_IPV4_ADDRESS               InternalIP      = {0};
    AnscCopyString(pDmz->source_ip_start, pDmlDmz->RemoteIPStart);
    AnscCopyString(pDmz->source_ip_end,   pDmlDmz->RemoteIPEnd  );
    AnscCopyString(pDmz->dest_mac,        pDmlDmz->InternalMAC  );
    InternalIP.Value = SlapVcoStringToIp4Addr(NULL, pDmlDmz->InternalIP);
    pDmz->dest_ip = InternalIP.Dot[3];
    */
    safec_rc = strcpy_s(pDmz->dest_ip,sizeof(pDmz->dest_ip), pDmlDmz->InternalIP);
    ERR_CHK(safec_rc);
    safec_rc = strcpy_s(pDmz->dest_ipv6,sizeof(pDmz->dest_ipv6), pDmlDmz->IPv6Host);
    ERR_CHK(safec_rc);

    rc = Utopia_SetDMZSettings(&Ctx, pDmz); //CID : 71892 Big parameter passed by value

    if ( rc != SUCCESS)
    {
        CcspTraceWarning(("Utopia_SetDMZSettings failed rc %lu in %s\n", rc, __FUNCTION__));

        returnStatus = ANSC_STATUS_FAILURE;
    }
    else
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    AnscFreeMemory(pDmz);
    Utopia_Free(&Ctx, 1);

    return returnStatus;
}
/*
static int s_UpdateDynPortMapping (int index, portMapDyn_t *pmap)
{
    token_t  se_token;
    int      se_fd = s_sysevent_connect(&se_token);
    if (0 > se_fd) {
        return ERR_SYSEVENT_CONN;
    }

    char value[1024], param[128], unique[128];
    time(&pmap->last_updated);
    snprintf(value, sizeof(value), "%s,%s,%d,%s,%d,%s,%d,%ld,%s",
                               (TRUE == pmap->enabled) ? "enabled" : "disabled",
                               (strlen(pmap->external_host) == 0) ? "none" : pmap->external_host,
                               pmap->external_port,
                               pmap->internal_host,
                               pmap->internal_port,
                               (TCP == pmap->protocol) ? "tcp" : "udp",
                               pmap->lease,
                               (long)(pmap->last_updated),
                               pmap->name);

    snprintf(param, sizeof(param), "portmap_dyn_%d", index);
    sysevent_get(se_fd, se_token, param, unique, sizeof(unique));
    if (0 == *unique || 0 == strcasecmp(unique, "none")) {
        // add new unique entry
        sysevent_set_unique(se_fd, se_token, "portmap_dyn_pool", value, unique, sizeof(unique));
        sysevent_set(se_fd, se_token, param, unique, 0);
        ulogf(ULOG_CONFIG, UL_UTAPI, "%s: add entry (index %d): create new unique entry %s", __FUNCTION__, index, unique);
    } else {
        // overwrite existing unique entry
        ulogf(ULOG_CONFIG, UL_UTAPI, "%s: add entry (index %d): overwrite unique entry %s", __FUNCTION__, index, unique);
        sysevent_set(se_fd, se_token, unique, value, 0);
    }

    ulog(ULOG_CONFIG, UL_UTAPI, "restarting firewall...");

    int rc = sysevent_set(se_fd, se_token, "firewall-restart", NULL, 0);

    ulogf(ULOG_CONFIG, UL_UTAPI, "firewall restart mesg sent, (rc %d)", rc);

    sleep(1);

    ulog(ULOG_CONFIG, UL_UTAPI, "firewall hold off done");

    return UT_SUCCESS;
}
*/

ANSC_STATUS _AddPortMapping(        
    UtopiaContext   *pCtx,
    PCOSA_DML_NAT_PMAPPING      pEntry
)
{
    portFwdSingle_t         singleInfo;
    portFwdRange_t           rangeInfo;
    int                             rc;
    errno_t                 safec_rc = -1;

    if ( pEntry->ExternalPortEndRange == pEntry->ExternalPort && pEntry->PublicIP.Value == 0 )
    {
        safec_rc = sprintf_s(singleInfo.dest_ip, sizeof(singleInfo.dest_ip), "%d.%d.%d.%d", pEntry->InternalClient.Dot[0],\
              pEntry->InternalClient.Dot[1],\
              pEntry->InternalClient.Dot[2],\
              pEntry->InternalClient.Dot[3]);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
           return ANSC_STATUS_FAILURE;
        }

        singleInfo.enabled = pEntry->bEnabled;
		singleInfo.prevRuleEnabledState = pEntry->bEnabled;
        singleInfo.external_port = pEntry->ExternalPort;
        singleInfo.internal_port = pEntry->InternalPort;
        singleInfo.protocol = SB_2_U_PF_PPOTOCOL(pEntry->Protocol);
        singleInfo.rule_id = pEntry->InstanceNumber;
        //AnscCopyString(singleInfo.name, pEntry->Description);
        strncpy(singleInfo.name, pEntry->Description, sizeof(singleInfo.name));
        strncpy(singleInfo.dest_ipv6, pEntry->X_CISCO_COM_InternalClientV6, sizeof(singleInfo.dest_ipv6));
        
        rc = Utopia_AddPortForwarding(pCtx, &singleInfo);
        if ( rc != SUCCESS )
        {
            CcspTraceWarning(("Utopia_AddPortForwarding failed rc %d in %s\n", rc, __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        safec_rc = sprintf_s(rangeInfo.dest_ip, sizeof(rangeInfo.dest_ip), "%d.%d.%d.%d", pEntry->InternalClient.Dot[0],\
              pEntry->InternalClient.Dot[1],\
              pEntry->InternalClient.Dot[2],\
              pEntry->InternalClient.Dot[3]);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
           return ANSC_STATUS_FAILURE;
        }
        safec_rc = sprintf_s(rangeInfo.public_ip, sizeof(rangeInfo.public_ip), "%d.%d.%d.%d", pEntry->PublicIP.Dot[0],\
              pEntry->PublicIP.Dot[1],\
              pEntry->PublicIP.Dot[2],\
              pEntry->PublicIP.Dot[3]);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
           return ANSC_STATUS_FAILURE;
        }
        rangeInfo.enabled = pEntry->bEnabled;
		rangeInfo.prevRuleEnabledState = pEntry->bEnabled;		
        rangeInfo.end_port = pEntry->ExternalPortEndRange;
        rangeInfo.internal_port = pEntry->InternalPort;
        rangeInfo.protocol = SB_2_U_PF_PPOTOCOL(pEntry->Protocol);
        rangeInfo.start_port = pEntry->ExternalPort;
        rangeInfo.rule_id = pEntry->InstanceNumber;
        rangeInfo.internal_port_range_size = 0;
        //AnscCopyString(rangeInfo.name, pEntry->Description);
        strncpy(rangeInfo.name, pEntry->Description, sizeof(rangeInfo.name));
        /* CID 163037 fix */
	strncpy(rangeInfo.dest_ipv6, pEntry->X_CISCO_COM_InternalClientV6, (sizeof(rangeInfo.dest_ipv6)-1));

        rc = Utopia_AddPortForwardingRange(pCtx, &rangeInfo);
        if ( rc != SUCCESS )
        {
            CcspTraceWarning(("Utopia_SetPortForwardingRange failed rc %d in %s\n", rc, __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS _Update_TriggerEnable(UtopiaContext   *pCtx, boolean_t enabled){
    
    if(enabled == TRUE && g_NatPTTriggerEnable == FALSE){

        /* Active port trigger */
        if(Utopia_RawSet(pCtx, COSA_NAT_ID_SYSCFG_NAMESPACE, PORT_TRIGGER_ENABLED_KEY, "1"))
        {
            g_NatPTTriggerEnable = TRUE;
            return ANSC_STATUS_SUCCESS;
        }
        else
        {
            CcspTraceWarning(("Utopia_RawSet failed in %s\n",  __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        return ANSC_STATUS_SUCCESS;
    }
}

int _Check_PF_parameter(PCOSA_DML_NAT_PMAPPING pPortMapping)
{      
    if( pPortMapping->PublicIP.Value == 0 &&
        ((pPortMapping->ExternalPort == 0) || 
         (pPortMapping->ExternalPortEndRange < pPortMapping->ExternalPort) ||
         (pPortMapping->Protocol > 3 || pPortMapping->Protocol < 1) ||
          FALSE == CosaDmlNatChkPortRange(pPortMapping->InstanceNumber, pPortMapping->bEnabled, \
                pPortMapping->ExternalPort, pPortMapping->ExternalPortEndRange, pPortMapping->Protocol , 0)) 
        )
    {
        CcspTraceWarning(("Wrong Port Mapping parameter external Port %d ~ %d, protocol %d, InternalPort %d,InternalClient %x PublicIP %x\n", \
                   pPortMapping->ExternalPort, pPortMapping->ExternalPortEndRange, \
                   pPortMapping->Protocol,pPortMapping->InternalPort, pPortMapping->InternalClient.Value,\
                   pPortMapping->PublicIP.Value ));
        return FALSE;
    }
    
    if( pPortMapping->InternalClient.Value == 0 || 
        FALSE == CosaDmlNatChkPortMappingClient(pPortMapping->InternalClient.Value)){
        CcspTraceWarning(("Wrong InternalClient value %x\n",pPortMapping->InternalClient.Value ));
        return FALSE;
    }

#if defined (SPEED_BOOST_SUPPORTED)
    if( IsPortOverlapWithSpeedboostPortRange(pPortMapping->ExternalPort, pPortMapping->ExternalPortEndRange, pPortMapping->InternalPort , pPortMapping->InternalPort))
        return FALSE;
#endif

    return TRUE;
}


int _Check_PT_parameter(PCOSA_DML_NAT_PTRIGGER pPortTrigger)
{
    /* Check parameter setting */
    if( (pPortTrigger->TriggerProtocol > 3 || pPortTrigger->TriggerProtocol < 1) ||
        (pPortTrigger->TriggerPortStart == 0) || 
        (pPortTrigger->TriggerPortEnd == 0) ||
        (pPortTrigger->TriggerPortEnd < pPortTrigger->TriggerPortStart) ||
        (pPortTrigger->ForwardPortEnd == 0) ||
        (pPortTrigger->ForwardPortStart == 0) ||
        (pPortTrigger->ForwardPortEnd < pPortTrigger->ForwardPortStart))
    {
        trace("@@@@pPortTrigger->TriggerProtocol %d \n", pPortTrigger->TriggerProtocol);
        trace("@@@@pPortTrigger-> TriggerPortStart %d \n", pPortTrigger->TriggerPortStart );
        trace("@@@@pPortTrigger->TriggerPortEnd %d \n", pPortTrigger->TriggerPortEnd );
        trace("@@@@pPortTrigger->ForwardPortStart %d \n", pPortTrigger->ForwardPortStart);
        trace("@@@@pPortTrigger->ForwardPortEnd %d \n", pPortTrigger->ForwardPortEnd);
        CcspTraceWarning(("Port Trigger parameter error \n"));
        return FALSE;
    }

#if defined (SPEED_BOOST_SUPPORTED)
    if( IsPortOverlapWithSpeedboostPortRange(pPortTrigger->TriggerPortStart, pPortTrigger->TriggerPortEnd , pPortTrigger->ForwardPortStart, pPortTrigger->ForwardPortEnd))
        return FALSE;
#endif

    return TRUE;
}

#if defined (SPEED_BOOST_SUPPORTED)
/*
- *  Procedure     : IsPortOverlapWithSpeedboostPortRange
- *  Purpose       : check if External or internal ports are overlap with Speedboot Range port
- *  Parameters    :
- *    fp          : External and internal port ranges from PF or PT user defined
- *  Return        :
- *    TRUE        : PF/PT port ranges are overlapping with xm speedboost port ranges
- *    FALSE       : pvd not enabled or PF/PT ports are not overlapping with xm speedboost portranges
- *
- */
int IsPortOverlapWithSpeedboostPortRange(int ExternalPort, int ExternalPortEndRange, int InternalPort , int InternalPortend)
{
    char pvd_enabled[8]={0};
    char sb_port_startv4[16]={0};
    char sb_port_endv4[16]={0};
    char sb_port_startv6[16]={0};
    char sb_port_endv6[16]={0};
    memset(pvd_enabled, 0, sizeof(pvd_enabled));
    memset(sb_port_startv4, 0, sizeof(sb_port_startv4));
    memset(sb_port_endv4, 0, sizeof(sb_port_endv4));
    memset(sb_port_startv6, 0, sizeof(sb_port_startv6));
    memset(sb_port_endv6, 0, sizeof(sb_port_endv6));

    int rc = syscfg_get( NULL, "Advertisement_pvd_enable" , pvd_enabled , sizeof( pvd_enabled ) ) ;

    if (rc == 0 && (0 == strcmp("1", pvd_enabled) || 0 == strcasecmp("true", pvd_enabled)))
    {
       rc = syscfg_get( NULL, "SpeedBoost_Port_StartV4" , sb_port_startv4 , sizeof( sb_port_startv4 ) );
       rc |= syscfg_get( NULL, "SpeedBoost_Port_EndV4" , sb_port_endv4 , sizeof( sb_port_endv4 ) );
       rc |= syscfg_get( NULL, "SpeedBoost_Port_StartV6" , sb_port_startv6 , sizeof( sb_port_startv6 ) );
       rc |= syscfg_get( NULL, "SpeedBoost_Port_EndV6" , sb_port_endv6 , sizeof( sb_port_endv6 ) );

       if (rc == 0 && atoi(sb_port_startv4) > 0 && atoi(sb_port_endv4) > 0 && atoi(sb_port_startv6) > 0 && atoi(sb_port_endv6) > 0 )
       {
          if ((atoi(sb_port_endv4) >= ExternalPort && atoi(sb_port_startv4) <= ExternalPortEndRange) || \
              (atoi(sb_port_endv4) >= InternalPort && atoi(sb_port_startv4) <= InternalPortend) || \
              (atoi(sb_port_endv6) >= ExternalPort && atoi(sb_port_startv6) <= ExternalPortEndRange) || \
              (atoi(sb_port_endv6) >= InternalPort && atoi(sb_port_startv6) <= InternalPortend))
          {
             CcspTraceError((" External or Internal port is overlapping with speedboost port, exit from %s\n", __FUNCTION__));
             return TRUE;
          }
       }
    }
    return FALSE;
}
#endif

/* IP is type of ANSC_IPV4_ADDRESS, but ANSC_IPV4_ADDRESS is defind as union <anonymous> we cannot use it as parameter */  
static inline void _sent_syslog_pm_sb(char *opt, UCHAR protocol, USHORT external, USHORT external_end, USHORT internal, UCHAR ip[4], BOOLEAN active)
{
    char extPort[30], intPort[20];
    errno_t safec_rc = -1;
    if(external_end == 0)
    {
        safec_rc = sprintf_s(extPort, sizeof(extPort), "ExternelPort %d", external);
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
        }
    }
    else
    {
        safec_rc = sprintf_s(extPort, sizeof(extPort), "ExternelPort(s) %d~%d", external, external_end);
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
        }
    }
    if(internal != 0)
    {
        safec_rc = sprintf_s(intPort, sizeof(intPort), "InternelPort %d",internal); 
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
        }
    }
    else
        intPort[0]='\0';

    syslog_systemlog("PortMapping", LOG_NOTICE, "%s: %s %s%s IP %d.%d.%d.%d %s", opt,\
        (protocol==1)? "TCP": ((protocol == 2) ? "UDP" : "TCP/UDP"), \
        extPort, intPort, ip[0], ip[1],ip[2],ip[3],\
        (active==TRUE)?"Active":"Inactive");
}

static inline void _sent_syslog_pt_sb(char *opt, UCHAR protocol, USHORT trigger_start, USHORT trigger_end, USHORT forward_start, USHORT forward_end, BOOLEAN active)
{
    syslog_systemlog("PortTriggering", LOG_NOTICE, "%s: %s TriggerPort(s) %d~%d TargetPort(s) %d~%d %s", opt,\
        (protocol==1)? "TCP": ((protocol == 2) ? "UDP" : "TCP/UDP"), \
        trigger_start, trigger_end, forward_start, forward_end,\
        (active==TRUE)? "Active":"Inactive");
}

static inline void _sent_syslog_pm_u(char *opt, protocol_t protocol, int external, int external_end, int internal, char *ip, boolean_t active)
{
    char extPort[30], intPort[20];
    errno_t safec_rc = -1;
    if(external_end == 0)
    {
        safec_rc = sprintf_s(extPort, sizeof(extPort), "ExternelPort %d", external);
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
        }
    }
    else
    {
        safec_rc = sprintf_s(extPort, sizeof(extPort), "ExternelPort(s) %d~%d", external, external_end);
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
        }
    }

    if(internal != 0)
    {
        safec_rc = sprintf_s(intPort, sizeof(intPort), "InternelPort %d",internal); 
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
        }
    }
    else
        intPort[0]='\0';

    syslog_systemlog("PortMapping", LOG_NOTICE, "%s: %s %s%s IP %s %s", opt,\
        (protocol==TCP)? "TCP": ((protocol == UDP) ? "UDP" : "TCP/UDP"), \
        extPort, intPort, ip,\
        (active==TRUE)?" Active":" Inactive");

}
static inline void _sent_syslog_pt_u(char *opt, protocol_t protocol, int trigger_start, int trigger_end, int forward_start, int forward_end, boolean_t active)
{
    syslog_systemlog("PortTriggering", LOG_NOTICE, "%s: %s TriggerPort(s) %d~%d TargetPort(s) %d~%d %s", opt,\
        (protocol==1)? "TCP": ((protocol == 2) ? "UDP" : "TCP/UDP"), \
        trigger_start, trigger_end, forward_start, forward_end,\
        (active==TRUE)?" Active":" Inactive");
}

#ifdef CONFIG_CISCO_CCSP_PRODUCT_ARES
#define RULE_PORTMAPPING_TYPE_NORMAL 1
#define RULE_PORTMAPPING_TYPE_HS 2
#define RULE_PORTMAPPING_TYPE_PUBLICIP 3
#define PORT_MAPPING_NORMAL_MAX_RULE_NUM 9
static inline int _CHECK_PORTMAPPING_RULE_TYPE(PCOSA_DML_NAT_PMAPPING pEntry){
    if(pEntry->PublicIP.Value != 0)
        return RULE_PORTMAPPING_TYPE_PUBLICIP;
    else if(pEntry->InternalPort == 0)
        return RULE_PORTMAPPING_TYPE_NORMAL;
    else
        return RULE_PORTMAPPING_TYPE_HS; 
}

static inline int _CHECK_PORTMAPPING_RULE_TYPE_U_RANGE(portFwdRange_t *pEntry)
{
    if(pEntry->public_ip[0] != '\0' && 0 != strcmp(pEntry->public_ip, "0.0.0.0"))
        return RULE_PORTMAPPING_TYPE_PUBLICIP;
    else if(pEntry->internal_port == 0)
        return RULE_PORTMAPPING_TYPE_NORMAL;
    else
        return RULE_PORTMAPPING_TYPE_HS; 
}

static inline int _CHECK_PORTMAPPING_RULE_TYPE_UTOPAI_SINGLE(portFwdSingle_t *pEntry)
{
    if(pEntry->internal_port == 0)
        return RULE_PORTMAPPING_TYPE_NORMAL;
    else
        return RULE_PORTMAPPING_TYPE_HS; 
}

#define PORTMAPPING_ADD_COUNT(type) \
{ \
    switch(type) \
    { \
        case RULE_PORTMAPPING_TYPE_NORMAL: \
            portMappingNormalRuleNUM++; \
            break; \
        case RULE_PORTMAPPING_TYPE_HS: \
            portMappingHSRuleNUM++; \
            break; \
        case RULE_PORTMAPPING_TYPE_PUBLICIP: \
            portMappingPublicIPRuleNUM++; \
            break; \
    } \
}
#endif

BOOL CosaDmlNatChkPortMappingMaxRuleNum(PCOSA_DML_NAT_PMAPPING pEntry)
{
    UNREFERENCED_PARAMETER(pEntry);
/* Don't limit portmapping rule number now.*/
#if 0 
    UtopiaContext          Ctx;
    portFwdSingle_t        *singleInfo = NULL;
    portFwdRange_t         *rangeInfo = NULL;
    int portMappingNormalRuleNUM = 0;
    int portMappingHSRuleNUM = 0;
    int portMappingPublicIPRuleNUM = 0;
    int             PortFwdRangeCount = 0;
    int            PortFwdSingleCount = 0;
    int type;
    int i;
    ULONG rc;
    if (!Utopia_Init(&Ctx))
    {
        return FALSE;
    }
    rc = Utopia_GetPortForwardingRange(&Ctx, &PortFwdRangeCount, &rangeInfo);
    if ( rc == SUCCESS )
    {
        for(i = 0; i< PortFwdRangeCount; i++, rangeInfo++){
            if(rangeInfo->rule_id != pEntry->InstanceNumber){
                type = _CHECK_PORTMAPPING_RULE_TYPE_U_RANGE(rangeInfo);
                PORTMAPPING_ADD_COUNT(type);
            }
        }
    }

    rc = Utopia_GetPortForwarding(&Ctx, &PortFwdSingleCount, &singleInfo);
    if ( rc == SUCCESS )
    {
        for(i = 0; i< PortFwdSingleCount;i++, singleInfo++){
            if(singleInfo->rule_id != pEntry->InstanceNumber){
                type=_CHECK_PORTMAPPING_RULE_TYPE_UTOPAI_SINGLE(singleInfo);
                PORTMAPPING_ADD_COUNT(type);
            }
        }
    }
    Utopia_Free(&Ctx, 0);

    type = _CHECK_PORTMAPPING_RULE_TYPE(pEntry);
    PORTMAPPING_ADD_COUNT(type);
    if(portMappingNormalRuleNUM <= PORT_MAPPING_NORMAL_MAX_RULE_NUM)
    {
        return TRUE;
    }
    else
    {
        CcspTraceWarning(("Port Mapping over max rule number \n"));
        return FALSE;
    }
#else 
        return TRUE;
#endif
}


/**********************************************************************

    caller:     self

    prototype:

        BOOL
        CosaDmlNatInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext,
                PFN_COSA_DML_NAT_GEN        pValueGenFn
            );

        Description:
            This is the initialization routine for NAT backend.

        Arguments:
            hDml               Opaque handle from DM adapter. Backend saves this handle for calling pValueGenFn.
             phContext       Opaque handle passed back from backend, needed by CosaDmlNatXyz() routines.
            pValueGenFn    Function pointer to instance number/alias generation callback.

        Return:
            Status of operation.

**********************************************************************/
ANSC_STATUS
CosaDmlNatInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext,
        PFN_COSA_DML_NAT_GEN        pValueGenFn
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    UtopiaContext Ctx;
    char tmp[25];
    g_nat_pportmapping_callback = pValueGenFn;
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceError(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    if(!Utopia_RawGet(&Ctx, COSA_NAT_SYSCFG_NAMESPACE, PORT_TRIGGER_ENABLED_KEY, tmp, sizeof(tmp)))
    {
        CcspTraceWarning(("%s Error get port trigger enabled\n", __FUNCTION__));
        g_NatPTTriggerEnable = FALSE;
    }
    else
    {
        if(!strcmp(tmp, "1"))
            g_NatPTTriggerEnable = TRUE;
        else
            g_NatPTTriggerEnable = FALSE;
    }

    if(!Utopia_RawGet(&Ctx, COSA_NAT_SYSCFG_NAMESPACE, PORT_FORWARD_ENABLED_KEY, tmp, sizeof(tmp)))
    {
        CcspTraceWarning(("%s Error get port forward enabled\n", __FUNCTION__));
        g_NatPFEnable = FALSE;
    }
    else
    {
        if(!strcmp(tmp, "1"))
            g_NatPFEnable = TRUE;
        else
            g_NatPFEnable = FALSE;
    }

    if(!Utopia_RawGet(&Ctx, COSA_NAT_SYSCFG_NAMESPACE, HS_PORT_FORWARD_ENABLED_KEY, tmp, sizeof(tmp)))
    {
        CcspTraceWarning(("%s Error get HS port forward enabled\n", __FUNCTION__));
        g_NatHSPFEnable = FALSE;
    }
    else
    {
        if(!strcmp(tmp, "1"))
            g_NatHSPFEnable = TRUE;
        else
            g_NatHSPFEnable = FALSE;
    }
    if(!Utopia_RawGet(&Ctx, COSA_NAT_SYSCFG_NAMESPACE, NAT_FORWARD_ENABLED_KEY, tmp, sizeof(tmp)))
    {
        CcspTraceWarning(("%s Error get HS port forward enabled\n", __FUNCTION__));
        g_NatOne2OneEnable = FALSE;
    }
    else
    {
        if(!strcmp(tmp, "1"))
            g_NatOne2OneEnable = TRUE;
        else
            g_NatOne2OneEnable = FALSE;
    }

    Utopia_Free(&Ctx, 0);
    return ANSC_STATUS_SUCCESS;
}
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS_SUCCESS 
        CosaDmlNatGetPortMapping
            (
                ANSC_HANDLE                 hContext,
                PULONG                      InstanceNumber,
                PCOSA_DML_NAT_PMAPPING      pNatPMapping
            )
        Description:
            This routine is to retrieve the port mappings.

        Arguments:

        Return:

**********************************************************************/
ANSC_STATUS
CosaDmlNatGetPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                      InstanceNumber,
        PCOSA_DML_NAT_PMAPPING      pNatPMapping
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext          Ctx = {0};
    portFwdSingle_t        singleInfo;
    portFwdRange_t         rangeInfo;
    int                    PortFwdDynCount = 0;
    ULONG                  rc;
    errno_t                safec_rc = -1;
/*  portMapDyn_t           dynInfo ={0};
    ANSC_IPV4_ADDRESS      nat_lan;  
    COSA_DML_NAT_PMAPPING  tmp ={0};
    lanSetting_t           lan;
    ULONG                  ulIndex = 0;*/

    if (pNatPMapping == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
#if 0
    /* We have to get the value every time in case of the modification of Lan setting */
    rc = Utopia_GetLanSettings(&Ctx, &lan);
    if (SUCCESS == rc)
    {
        char* pCh    = NULL;
        char* pStart = lan.ipaddr;

        for ( i = 0; i < IPV4_ADDRESS_SIZE; i++ )
        {
            pCh = _ansc_strchr(pStart, '.');

            if ( pCh )
            {
                *pCh = 0;
                nat_lan.Dot[i] = _ansc_atoi(pStart);
                pStart = pCh + 1;
            }
            else
            {
                nat_lan.Dot[i] = _ansc_atoi(pStart);
            }
        }
    }
#endif

    AnscZeroMemory( &rangeInfo,sizeof(portFwdRange_t)); /*RDKB-6842, CID-33182, initializing */

    rangeInfo.rule_id = InstanceNumber; 
    rc = Utopia_GetPortForwardingRangeByRuleId(&Ctx, &rangeInfo);
    if ( rc == SUCCESS )
    {
        pNatPMapping->AllInterfaces = TRUE;
        pNatPMapping->bEnabled = rangeInfo.enabled;
        pNatPMapping->ExternalPort = rangeInfo.start_port;
        pNatPMapping->ExternalPortEndRange = rangeInfo.end_port;
        pNatPMapping->InternalPort = rangeInfo.internal_port;
        pNatPMapping->LeaseDuration = 0;
        pNatPMapping->Protocol = U_2_SB_PF_PROTOCOL(rangeInfo.protocol);
        pNatPMapping->RemoteHost.Value = 0;
        pNatPMapping->PublicIP.Value = inet_addr(rangeInfo.public_ip);
        pNatPMapping->Status = (rangeInfo.enabled ? COSA_DML_NAT_STATUS_Enabled : COSA_DML_NAT_STATUS_Disabled);
        pNatPMapping->InternalClient.Value = inet_addr(rangeInfo.dest_ip);
        pNatPMapping->InstanceNumber = rangeInfo.rule_id;
        safec_rc = strcpy_s(pNatPMapping->Description,sizeof(pNatPMapping->Description), rangeInfo.name);
        ERR_CHK(safec_rc);
        pNatPMapping->X_CISCO_COM_Origin = COSA_DML_NAT_PMAPPING_Origin_Static;
        Utopia_Free(&Ctx, 0);
        return ANSC_STATUS_SUCCESS;
    }

    singleInfo.rule_id = InstanceNumber;
    rc = Utopia_GetPortForwardingByRuleId(&Ctx, &singleInfo);
    if ( rc == SUCCESS )
    {
        pNatPMapping->AllInterfaces = TRUE;
        pNatPMapping->bEnabled = singleInfo.enabled;
        pNatPMapping->ExternalPort = singleInfo.external_port;
        pNatPMapping->ExternalPortEndRange = pNatPMapping->ExternalPort;
        pNatPMapping->InternalPort = singleInfo.internal_port;
        pNatPMapping->LeaseDuration = 0;
        pNatPMapping->Protocol = U_2_SB_PF_PROTOCOL(singleInfo.protocol);
        pNatPMapping->RemoteHost.Value = 0;
        pNatPMapping->PublicIP.Value = 0; 
        pNatPMapping->Status = (singleInfo.enabled ? COSA_DML_NAT_STATUS_Enabled : COSA_DML_NAT_STATUS_Disabled);
        pNatPMapping->InternalClient.Value = inet_addr(singleInfo.dest_ip);
        pNatPMapping->InstanceNumber = singleInfo.rule_id;
        safec_rc = strcpy_s(pNatPMapping->Description,sizeof(pNatPMapping->Description), singleInfo.name);
        ERR_CHK(safec_rc);
        pNatPMapping->X_CISCO_COM_Origin = COSA_DML_NAT_PMAPPING_Origin_Static;
        Utopia_Free(&Ctx, 0);
        return ANSC_STATUS_SUCCESS;
    }

    rc = Utopia_GetDynPortMappingCount(&PortFwdDynCount);
    if(rc != SUCCESS){
        PortFwdDynCount = 0;
        CcspTraceWarning(("Utopia_GetDynPortMappingCount failed rc %lu in %s\n", rc, __FUNCTION__));
    }
     else
     {
        CcspTraceWarning(("DynPortMappingCount is %d, we are not adding entry to TR-181/database since these are dynamic entries %s\n", PortFwdDynCount, __FUNCTION__));
     }

#if 0
    errno_t safec_rc = -1;
    if ( PortFwdDynCount != 0 )
    {
        for (i = 0; i < PortFwdDynCount; i++, ulIndex++)
        {
            rc = Utopia_GetDynPortMapping(i + 1, &dynInfo);
            if ( UT_SUCCESS != rc )
            {
                CcspTraceWarning(("Utopia_GetDynPortMapping failed rc %d in %s\n", rc, __FUNCTION__));
                Utopia_Free(&Ctx, 0);
                return ANSC_STATUS_FAILURE;
            }

            rc = loadID
            (
                &Ctx,
                dynInfo.external_port,
                dynInfo.protocol,
                tmp.Alias,
                &tmp.InstanceNumber
            );

            if(InstanceNumber == tmp.InstanceNumber){
                pNatPMapping->AllInterfaces = TRUE;
                pNatPMapping->bEnabled = dynInfo.enabled;
                pNatPMapping->ExternalPort = dynInfo.external_port;
                pNatPMapping->ExternalPortEndRange = pNatPMapping->ExternalPort;
                pNatPMapping->InternalPort = dynInfo.internal_port;
                pNatPMapping->LeaseDuration = dynInfo.lease;
                pNatPMapping->PublicIP.Value = 0; 
                pNatPMapping->Protocol = U_2_SB_PF_PROTOCOL(dynInfo.protocol);
                pNatPMapping->RemoteHost.Value =  inet_addr(dynInfo.external_host);
                pNatPMapping->Status = (dynInfo.enabled ? COSA_DML_NAT_STATUS_Enabled : COSA_DML_NAT_STATUS_Disabled);
                safec_rc = strcpy_s(pNatPMapping->Alias,sizeof(pNatPMapping->Alias), tmp.Alias);
                if(safec_rc != EOK)
                {
                   ERR_CHK(safec_rc);
                }
                if ( TRUE )
                {
                    char* pCh    = NULL;
                    char* pStart = dynInfo.internal_host;
                    ULONG j;

                    for ( j = 0; j < IPV4_ADDRESS_SIZE; j++ )
                    {
                        pCh = _ansc_strchr(pStart, '.');
                        if ( pCh )
                        {
                            *pCh = 0;
                            pNatPMapping->InternalClient.Dot[j] = _ansc_atoi(pStart);
                            pStart = pCh + 1;
                        }
                        else
                        {
                            pNatPMapping->InternalClient.Dot[j] = _ansc_atoi(pStart);
                        }
                    }
                }
                AnscCopyString(pNatPMapping->Description, dynInfo.name);
                pNatPMapping->X_CISCO_COM_Origin = COSA_DML_NAT_PMAPPING_Origin_Dynamic;
                Utopia_Free(&Ctx, 0);
                return ANSC_STATUS_SUCCESS;
            }
        }
    }
#endif
    Utopia_Free(&Ctx, 0);
    return ANSC_STATUS_FAILURE;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_NAT_PMAPPING
        CosaDmlNatGetPortMappings
            (
                ANSC_HANDLE                 hContext,
                PULONG                          pulCount
            )
        Description:
            This routine is to retrieve the complete list of NAT port mappings, which is a dynamic table.

        Arguments:
            pulCount  is to receive the actual number of entries.

        Return:
            The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_NAT_PMAPPING
CosaDmlNatGetPortMappings
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext          Ctx;
    PCOSA_DML_NAT_PMAPPING pNatPMapping = NULL;
    portFwdSingle_t         *singleInfo = NULL;
    portFwdRange_t           *rangeInfo = NULL;
//    portMapDyn_t                dynInfo;
#if defined _DT_WAN_Manager_Enable_
    portMapDyn_t                dynInfo;
#endif
//    lanSetting_t                    lan;
    ULONG                       ulIndex = 0;
    int            PortFwdSingleCount = 0;
    int             PortFwdRangeCount = 0;
    int               PortFwdDynCount = 0;
    ULONG                          rc;
    int                              i;
    int                      allCount = 0;
#ifdef _BWG_PRODUCT_REQ_
    int nat_count = 0;  //used to track total number of NAT entries
#endif
    errno_t                  safec_rc = -1;
//    ANSC_IPV4_ADDRESS             nat_lan;  
     if (!pulCount)
    {
        return NULL;
    }

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        *pulCount  =  0 ;
        return NULL;
    }
#if 0
    /* We have to get the value every time in case of the modification of Lan setting */
    rc = Utopia_GetLanSettings(&Ctx, &lan);
    if (SUCCESS == rc)
    {
        char* pCh    = NULL;
        char* pStart = lan.ipaddr;

        for ( i = 0; i < IPV4_ADDRESS_SIZE; i++ )
        {
            pCh = _ansc_strchr(pStart, '.');

            if ( pCh )
            {
                *pCh = 0;
                nat_lan.Dot[i] = _ansc_atoi(pStart);
                pStart = pCh + 1;
            }
            else
            {
                nat_lan.Dot[i] = _ansc_atoi(pStart);
            }
        }
    }
#endif
    rc = Utopia_GetPortForwardingRange(&Ctx, &PortFwdRangeCount, &rangeInfo);
    if ( rc != SUCCESS )
    {
        CcspTraceWarning(("Utopia_GetPortForwardingRange failed rc %lu in %s\n", rc, __FUNCTION__));
        PortFwdRangeCount = 0;
        rangeInfo = NULL;
    }

    rc = Utopia_GetPortForwarding(&Ctx, &PortFwdSingleCount, &singleInfo);
    if ( rc != SUCCESS )
    {
        CcspTraceWarning(("Utopia_GetPortForwarding failed rc %lu in %s\n", rc, __FUNCTION__));
        PortFwdSingleCount = 0;
        singleInfo = NULL;
    }


    rc = Utopia_GetDynPortMappingCount(&PortFwdDynCount);
    if(rc != SUCCESS){
        PortFwdDynCount = 0;
        CcspTraceWarning(("Utopia_GetDynPortMappingCount failed rc %lu in %s\n", rc, __FUNCTION__));
    }
     else
     {
        CcspTraceWarning(("DynPortMappingCount is %d, we are not adding entry to TR-181/database since these are dynamic entries %s\n", PortFwdDynCount, __FUNCTION__));
     }

#if defined _DT_WAN_Manager_Enable_
    allCount = PortFwdSingleCount + PortFwdRangeCount + PortFwdDynCount;
#else
    allCount = PortFwdSingleCount + PortFwdRangeCount ;
#endif
    if (allCount == 0)
    {
        Utopia_Free(&Ctx, 0);
        *pulCount = 0;
        /*RDKB,CID-33473, CID-32920; free unused resource before return*/
        if(rangeInfo) 
            free(rangeInfo);
        if(singleInfo) 
            free(singleInfo);
        return NULL;
    }

#if defined _DT_WAN_Manager_Enable_
    if ( g_NatPortFwdDynInstanceNum )
    {
        AnscFreeMemory(g_NatPortFwdDynInstanceNum);
        g_NatPortFwdDynInstanceNum = NULL;
    }
#endif
    pNatPMapping = AnscAllocateMemory(sizeof(COSA_DML_NAT_PMAPPING)*(allCount));
    if(pNatPMapping == NULL)
    {
        CcspTraceWarning((" AnscAllocateMemory failed rc %lu in %s\n", rc, __FUNCTION__));
        /*RDKB,CID-33473, CID-32920; free unused resource before return*/
        if(rangeInfo) 
            free(rangeInfo);
        if(singleInfo)
            free(singleInfo);
        *pulCount = 0;
        Utopia_Free(&Ctx, 0);
        return NULL;
    }
    ulIndex = 0;
    if ( PortFwdSingleCount != 0 )
    {
        for (i = 0; i < PortFwdSingleCount; i++, ulIndex++)
        {
            pNatPMapping[ulIndex].AllInterfaces = TRUE;
            pNatPMapping[ulIndex].bEnabled = singleInfo[i].enabled;
            pNatPMapping[ulIndex].ExternalPort = singleInfo[i].external_port;
            pNatPMapping[ulIndex].ExternalPortEndRange = pNatPMapping[ulIndex].ExternalPort;
            pNatPMapping[ulIndex].InternalPort = singleInfo[i].internal_port;
            pNatPMapping[ulIndex].LeaseDuration = 0;
            pNatPMapping[ulIndex].Protocol = U_2_SB_PF_PROTOCOL(singleInfo[i].protocol);
            pNatPMapping[ulIndex].RemoteHost.Value = 0;
            pNatPMapping[ulIndex].PublicIP.Value = 0; 
            pNatPMapping[ulIndex].Status = (singleInfo[i].enabled ? COSA_DML_NAT_STATUS_Enabled : COSA_DML_NAT_STATUS_Disabled);
            pNatPMapping[ulIndex].InternalClient.Value = inet_addr(singleInfo[i].dest_ip);
            pNatPMapping[ulIndex].InstanceNumber = singleInfo[i].rule_id;
            pNatPMapping[ulIndex].X_CISCO_COM_Origin = COSA_DML_NAT_PMAPPING_Origin_Static;
            safec_rc = strcpy_s(pNatPMapping[ulIndex].Description,sizeof(pNatPMapping[ulIndex].Description), singleInfo[i].name);
            ERR_CHK(safec_rc);
            safec_rc = strcpy_s(pNatPMapping[ulIndex].X_CISCO_COM_InternalClientV6,sizeof(pNatPMapping[ulIndex].X_CISCO_COM_InternalClientV6), singleInfo[i].dest_ipv6);
            ERR_CHK(safec_rc);
        }
        free(singleInfo);
        singleInfo = NULL;
    }

    if ( PortFwdRangeCount != 0 )
    {
        for (i = 0; i < PortFwdRangeCount; i++, ulIndex++)
        {
            pNatPMapping[ulIndex].AllInterfaces = TRUE;
            pNatPMapping[ulIndex].bEnabled = rangeInfo[i].enabled;
            pNatPMapping[ulIndex].ExternalPort = rangeInfo[i].start_port;
            pNatPMapping[ulIndex].ExternalPortEndRange = rangeInfo[i].end_port;
            pNatPMapping[ulIndex].InternalPort = rangeInfo[i].internal_port;
            pNatPMapping[ulIndex].LeaseDuration = 0;
            pNatPMapping[ulIndex].Protocol = U_2_SB_PF_PROTOCOL(rangeInfo[i].protocol);
            pNatPMapping[ulIndex].RemoteHost.Value = 0;
            pNatPMapping[ulIndex].PublicIP.Value = inet_addr(rangeInfo[i].public_ip);
            pNatPMapping[ulIndex].Status = (rangeInfo[i].enabled ? COSA_DML_NAT_STATUS_Enabled : COSA_DML_NAT_STATUS_Disabled);
            pNatPMapping[ulIndex].InternalClient.Value = inet_addr(rangeInfo[i].dest_ip);
            pNatPMapping[ulIndex].InstanceNumber = rangeInfo[i].rule_id;
            pNatPMapping[ulIndex].X_CISCO_COM_Origin = COSA_DML_NAT_PMAPPING_Origin_Static;
            safec_rc = strcpy_s(pNatPMapping[ulIndex].Description,sizeof(pNatPMapping[ulIndex].Description), rangeInfo[i].name);
            ERR_CHK(safec_rc);
            safec_rc = strcpy_s(pNatPMapping[ulIndex].X_CISCO_COM_InternalClientV6,sizeof(pNatPMapping[ulIndex].X_CISCO_COM_InternalClientV6), rangeInfo[i].dest_ipv6);
            ERR_CHK(safec_rc);
#ifdef _BWG_PRODUCT_REQ_
            //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: START
            if(strcmp("0.0.0.0",rangeInfo[i].public_ip))
            {
                updateExclusionList(rangeInfo[i].public_ip,nat_count+1,1);
                nat_count++;
            }
            //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: END
#endif
        }
#ifdef _BWG_PRODUCT_REQ_
        handleExclusion(nat_count);
#endif
        free(rangeInfo);
        rangeInfo = NULL;
    }

#if defined _DT_WAN_Manager_Enable_
    if ( PortFwdDynCount != 0 )
    {
        g_NatPortFwdDynInstanceNum = AnscAllocateMemory(sizeof(ULONG)*PortFwdDynCount);
        g_NatPortFwdDynInstanceNumCount = PortFwdDynCount;

        for (i = 0; i < PortFwdDynCount; i++, ulIndex++)
        {
            rc = Utopia_GetDynPortMapping(i + 1, &dynInfo);
            if ( UT_SUCCESS != rc )
            {
                CcspTraceWarning(("Utopia_GetDynPortMapping failed rc %d in %s\n", rc, __FUNCTION__));
                if(rangeInfo) /*RDKB-6842, CID-33473, free unused resource before exit*/
                    free(rangeInfo);
                if(singleInfo)
                    free(singleInfo);
                *pulCount = 0;
                Utopia_Free(&Ctx, 0);
                AnscFreeMemory(pNatPMapping);
                return NULL;
            }

            pNatPMapping[ulIndex].AllInterfaces = TRUE;
            pNatPMapping[ulIndex].bEnabled = dynInfo.enabled;
            pNatPMapping[ulIndex].ExternalPort = dynInfo.external_port;
            pNatPMapping[ulIndex].ExternalPortEndRange = pNatPMapping[ulIndex].ExternalPort;
            pNatPMapping[ulIndex].InternalPort = dynInfo.internal_port;
            pNatPMapping[ulIndex].LeaseDuration = dynInfo.lease;
            pNatPMapping[ulIndex].Protocol = U_2_SB_PF_PROTOCOL(dynInfo.protocol);
            pNatPMapping[ulIndex].RemoteHost.Value =  inet_addr(dynInfo.external_host);
            pNatPMapping[ulIndex].PublicIP.Value = 0; 
            pNatPMapping[ulIndex].Status = (dynInfo.enabled ? COSA_DML_NAT_STATUS_Enabled : COSA_DML_NAT_STATUS_Disabled);
            pNatPMapping[ulIndex].X_CISCO_COM_Origin = COSA_DML_NAT_PMAPPING_Origin_Dynamic;

            if ( TRUE )
            {
                char* pCh    = NULL;
                char* pStart = dynInfo.internal_host;
                ULONG j;

                for ( j = 0; j < IPV4_ADDRESS_SIZE; j++ )
                {
                    pCh = _ansc_strchr(pStart, '.');
                    if ( pCh )
                    {
                        *pCh = 0;
                        pNatPMapping[ulIndex].InternalClient.Dot[j] = _ansc_atoi(pStart);
                        pStart = pCh + 1;
                    }
                    else
                    {
                        pNatPMapping[ulIndex].InternalClient.Dot[j] = _ansc_atoi(pStart);
                    }
                }
            }

            AnscCopyString(pNatPMapping[ulIndex].Description, dynInfo.name);

            rc = loadID
            (
                &Ctx,
                dynInfo.external_port,
                dynInfo.protocol,
                pNatPMapping[ulIndex].Alias,
                &pNatPMapping[ulIndex].InstanceNumber
            );

            if ( -1 == rc )
            {
                g_nat_pportmapping_callback( NULL, &pNatPMapping[ulIndex] );

                saveID
                (
                    &Ctx,
                    dynInfo.external_port,
                    dynInfo.protocol,
                    pNatPMapping[ulIndex].Alias,
                    pNatPMapping[ulIndex].InstanceNumber
                );
            }
            g_NatPortFwdDynInstanceNum[i] = pNatPMapping[ulIndex].InstanceNumber;
        }
    }
#endif
    if(rangeInfo) /*RDKB-6842, CID-33473, free unused resource before exit*/
        free(rangeInfo);

    if(singleInfo)
        free(singleInfo);

    *pulCount = allCount;
    Utopia_Free(&Ctx, 1);
    return pNatPMapping;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatAddPortMapping
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_NAT_PMAPPING      pEntry
            )

    Description:
        The API adds one port mapping entry into NAT.

    Arguments:
        pEntry      Caller does not need to fill in Status or Alias fields. Upon return, callee fills in the generated Alias and associated Status.

    Return:
        he pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatAddPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext                   Ctx;
#ifdef _BWG_PRODUCT_REQ_
    char ip[256];
    int i=0,nat_count=0; //used to track the number of NAT entries
    int             ulPortFwdRangeCount = 0;
    portFwdRange_t           *rangeInfo = NULL;
    int                              rc = 0;
#endif
    if (!pEntry)
    {
        return ANSC_STATUS_FAILURE;
    }

#if defined (MULTILAN_FEATURE)
    if(pEntry->bEnabled && !_Check_PF_parameter(pEntry))
#else
    if( !_Check_PF_parameter(pEntry))
#endif
    {
        CcspTraceWarning(("Parameter Error in %s \n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    if(!CosaDmlNatChkEnableFlg(pEntry)){
        return ANSC_STATUS_FAILURE;
    }
   
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        syslog_systemlog("PortMapping", LOG_ERR, "%s Error initializing context\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }
#ifdef _BWG_PRODUCT_REQ_
    //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: START
    rc = Utopia_GetPortForwardingRange(&Ctx, &ulPortFwdRangeCount, &rangeInfo);
    if ( rc != SUCCESS )
    {
        CcspTraceWarning(("Utopia_GetPortForwardingRange failed rc %d in %s\n", rc, __FUNCTION__));
        ulPortFwdRangeCount = 0;
        rangeInfo = NULL;
    }

    for(i=0;i<ulPortFwdRangeCount;i++)
    {
        if(strcmp("0.0.0.0",rangeInfo[i].public_ip))
        {
            nat_count++;
        }
    }
    //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: END
#endif
    if( ANSC_STATUS_SUCCESS == _AddPortMapping(&Ctx, pEntry))
    {
        Utopia_Free(&Ctx, 1);

#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT        
#ifdef IVI_KERNEL_SUPPORT
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("protocol: %d, ExternalPort :%d, ExternalEndRange : %d, InternalPort : %d, bEnable :%d",
                    pEntry->Protocol, pEntry->ExternalPort, pEntry->ExternalPortEndRange, pEntry->InternalPort,
                    pEntry->bEnabled);
#endif        
#endif        
#endif        
#endif        
        _sent_syslog_pm_sb("ADD", pEntry->Protocol, pEntry->ExternalPort, pEntry->ExternalPortEndRange, pEntry->InternalPort, pEntry->InternalClient.Dot,pEntry->bEnabled);

#ifdef _BWG_PRODUCT_REQ_
        //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: START
        if(rc == SUCCESS)
        {
        sprintf(ip, "%d.%d.%d.%d", pEntry->PublicIP.Dot[0],\
              pEntry->PublicIP.Dot[1],\
              pEntry->PublicIP.Dot[2],\
              pEntry->PublicIP.Dot[3]);

        if(strcmp("0.0.0.0",ip))
        {
            updateExclusionList(ip,nat_count+1,1);
            handleExclusion(nat_count+1);
        }

        }
        if(ulPortFwdRangeCount > 0)
            free(rangeInfo);

        //CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: END
#endif
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        _sent_syslog_pm_sb("ADD FAILED", pEntry->Protocol, pEntry->ExternalPort, pEntry->ExternalPortEndRange, pEntry->InternalPort, pEntry->InternalClient.Dot,pEntry->bEnabled);
        Utopia_Free(&Ctx, 0);
        return ANSC_STATUS_FAILURE;
    }
}


#ifdef _BWG_PRODUCT_REQ_
//CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: START
ANSC_STATUS UpdateList(ANSC_HANDLE hContext)
{
    UNREFERENCED_PARAMETER(hContext);
    int count=0,i=0, j=0, nat_count=0; //used to track totalcount and nat count values
    ULONG rc = 0;
    UtopiaContext Ctx;
    portFwdRange_t *rangeInfo = NULL;
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        syslog_systemlog("PortMapping", LOG_NOTICE, "%s Error initializing context\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }
    rc = Utopia_GetPortForwardingRange(&Ctx, &count, &rangeInfo);
    if ( rc != SUCCESS )
    {
        CcspTraceWarning(("Utopia_GetPortForwardingRange failed rc %lu in %s\n", rc, __FUNCTION__));
        count = 0;
        rangeInfo = NULL;
    }
    if(rc == SUCCESS)
    {
    for(i=0;i<count;i++)
    {
         if(strcmp("0.0.0.0",rangeInfo[i].public_ip))
         {
             updateExclusionList(rangeInfo[i].public_ip,nat_count+1,1);
             nat_count++;
         }
    }
    }
    //CGWTDETS-8800 : Usable Statics will no longer support 1-1 NAT :: START
    if(nat_count < max_size )
    {
        for(j=nat_count;j<max_size;j++)
        {
            updateExclusionList("0.0.0.0",j+1,1);
        }
    }
    //CGWTDETS-8800 : Usable Statics will no longer support 1-1 NAT :: END
    if(count > 0)
        free(rangeInfo);

    handleExclusion(nat_count);

    Utopia_Free(&Ctx,1);
    return ANSC_STATUS_SUCCESS;
}
//CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: END
#endif


#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
static int delete_mapt_pf_single_rule(int internal_port, int external_port, int protocol, char* dest_ip)
{
    token_t sysevent_token;
    static int sysevent_fd = -1;
    char mapt_config_value[BUFLEN_8] = {0};
    char mapt_ip_address[BUFLEN_32] = {0}; /*For Future*/
    char cmdIvictlPf[BUFLEN_256] = {0};
    char ipAddress[BUFLEN_32] = {0};
    char mapt_protocol_tcp[BUFLEN_8]  = "100";
    char mapt_protocol_udp[BUFLEN_8]  = "010";
    char mapt_protocol_both[BUFLEN_8] = "110";
    //char mapt_protocol_icmp[BUFLEN_8] = "001";/*For Future*/

#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("Deleting Single Port Forward Rule --- Start");
    LOG_PRINT_IVICTL("internal_port:%d external_port:%d protocol:%d dest_ip:%s",internal_port, external_port, protocol, dest_ip);
#endif
    sysevent_fd = s_sysevent_connect(&sysevent_token);
    CcspTraceInfo(("internal_port:%d external_port:%d protocol:%d dest_ip:%s",internal_port, external_port, protocol, dest_ip));

    if (sysevent_fd < 0)
    {
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("ERROR: Sysevent FD is not available \n");
#endif        
        return ERR_SYSEVENT_CONN;
    }

    if (sysevent_get(sysevent_fd, sysevent_token, SYSEVENT_MAPT_CONFIG_FLAG, mapt_config_value, sizeof(mapt_config_value)) != 0)
    {
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("ERROR: Failed to get MAPT configuration value from sysevent \n");
#endif        
        return RET_ERR;
    }

    if (strncmp(mapt_config_value,SET, 3) == 0)
    {
        /*Get the MAP-T Address*/
        if (sysevent_get(sysevent_fd, sysevent_token, SYSEVENT_MAPT_IP_ADDRESS, mapt_ip_address, sizeof(mapt_ip_address)) != 0)
        {
#ifdef FEATURE_MAPT_DEBUG
            LOG_PRINT_IVICTL("ERROR: Failed to get MAPT IP Address from sysevent \n");
#endif        
            return RET_ERR;
        }
    }

#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("mapt_ip_address:%s",mapt_ip_address);
#endif        

    if (strlen(dest_ip) != 0)
    {    
        strncpy(ipAddress, dest_ip, sizeof(ipAddress));
    }
    else
    {
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("Invalid IP Address");
#endif        
        return RET_ERR;
    }
#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("ipAddress:%s",ipAddress);
#endif        

    if (protocol == 0 )
    {
        snprintf(cmdIvictlPf, sizeof(cmdIvictlPf),
                "ivictl -d -a %s -p %d -q %d -P %s ",
                ipAddress, external_port, external_port, mapt_protocol_tcp);
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("ivictl:tcp: %s",cmdIvictlPf);
#endif        

        if (system(cmdIvictlPf) != 0)
        {
#ifdef FEATURE_MAPT_DEBUG
            LOG_PRINT_IVICTL("Error Deleting the Rule");
#endif        
            return RET_ERR;
        }
    }
    else if (protocol == 1 )
    {
        snprintf(cmdIvictlPf, sizeof(cmdIvictlPf),
                "ivictl -d -a %s -p %d -q %d -P %s ",
                ipAddress, external_port, external_port, mapt_protocol_udp);
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("ivictl:udp: %s",cmdIvictlPf);
#endif        
        if (system(cmdIvictlPf) != 0)
        {
#ifdef FEATURE_MAPT_DEBUG
                LOG_PRINT_IVICTL("Error Deleting the Rule");
#endif        
                return RET_ERR;
        }
    }
    else if (protocol == 2)
    {
        snprintf(cmdIvictlPf, sizeof(cmdIvictlPf),
                "ivictl -d -a %s -p %d -q %d -P %s ",
                ipAddress, external_port, external_port, mapt_protocol_both);
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("ivictl:both: %s",cmdIvictlPf);
#endif        
        if (system(cmdIvictlPf) != 0)
        {
#ifdef FEATURE_MAPT_DEBUG
            LOG_PRINT_IVICTL("Error Deleting the Rule");
#endif
            return RET_ERR;
        }
    }
   
#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("Deleting Single Port Forward Rule --- End ");
#endif
    return ANSC_STATUS_SUCCESS;
}

static int delete_mapt_pf_range_rule(int start_port, int end_port, int protocol, char* dest_ip)
{
    token_t sysevent_token;
    static int sysevent_fd = -1;
    char mapt_config_value[BUFLEN_8] = {0};
    char mapt_ip_address[BUFLEN_32] = {0};/*For Future*/
    char ipAddress[BUFLEN_32] = {0};
    char cmdIvictlPf[BUFLEN_256] = {0};
    char mapt_protocol_tcp[BUFLEN_8]  = "100";
    char mapt_protocol_udp[BUFLEN_8]  = "010";
    char mapt_protocol_both[BUFLEN_8] = "110";
    //char mapt_protocol_icmp[BUFLEN_8] = "001";/*For Future*/
    int range = 0;
    int index ;

#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("Deleting Range Port Forward Rule --- Start");
    LOG_PRINT_IVICTL("start_port:%d end_port:%d protocol:%d dest_ip:%s",start_port, end_port, protocol, dest_ip);
#endif

    sysevent_fd = s_sysevent_connect(&sysevent_token);
    if (sysevent_fd < 0)
    {
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("ERROR: Sysevent FD is not available \n");
#endif
        return ERR_SYSEVENT_CONN;
    }

    if (sysevent_get(sysevent_fd, sysevent_token, SYSEVENT_MAPT_CONFIG_FLAG, mapt_config_value, sizeof(mapt_config_value)) != 0)
    {
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("ERROR: Failed to get MAPT configuration value from sysevent \n");
#endif
        return RET_ERR;
    }

    if (strncmp(mapt_config_value,SET, 3) == 0)
    {
        /*Get the MAP-T Address*/
        if (sysevent_get(sysevent_fd, sysevent_token, SYSEVENT_MAPT_IP_ADDRESS, mapt_ip_address, sizeof(mapt_ip_address)) != 0)
        {
#ifdef FEATURE_MAPT_DEBUG
            LOG_PRINT_IVICTL("ERROR: Failed to get MAPT IP Address from sysevent \n");
#endif
            return RET_ERR;
        }
    }
#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("mapt_ip_address:%s",mapt_ip_address);
#endif
    if (strlen(dest_ip) != 0)
    {    
        strncpy(ipAddress, dest_ip, sizeof(ipAddress));
    }
    else
    {
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("Invalid IP Address");
#endif
        return RET_ERR;
    }
#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("ipAddress:%s",ipAddress);
#endif

    range = end_port - start_port;

#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("range:%d",range);
#endif
    if (protocol == 0 )
    {
        for (index = 0; index <= range;  index++)
        {
            snprintf(cmdIvictlPf, sizeof(cmdIvictlPf),
                "ivictl -d -a %s -p %d -q %d -P %s ",
                ipAddress, start_port+index, start_port+index, mapt_protocol_tcp);

#ifdef FEATURE_MAPT_DEBUG
            LOG_PRINT_IVICTL("ivictl:tcp: %s",cmdIvictlPf);
#endif
            if (system(cmdIvictlPf) != 0)
            {
#ifdef FEATURE_MAPT_DEBUG
                LOG_PRINT_IVICTL("Error Deleting the Rule");
#endif
                return RET_ERR;
            }
            memset(cmdIvictlPf, 0, sizeof(cmdIvictlPf));
        }
    }
    else if (protocol == 1 )
    {
        for (index = 0; index <= range;  index++)
        {
            snprintf(cmdIvictlPf, sizeof(cmdIvictlPf),
                "ivictl -d -a %s -p %d -q %d -P %s ",
                ipAddress, start_port+index, start_port+index, mapt_protocol_udp);
#ifdef FEATURE_MAPT_DEBUG
            LOG_PRINT_IVICTL("ivictl:udp: %s",cmdIvictlPf);
#endif
            if (system(cmdIvictlPf) != 0)
            {
#ifdef FEATURE_MAPT_DEBUG
                LOG_PRINT_IVICTL("Error Deleting the Rule");
#endif
                return RET_ERR;
            }
            memset(cmdIvictlPf, 0, sizeof(cmdIvictlPf));
        }
    }
    else if (protocol == 2)
    {
        for (index = 0; index <= range;  index++)
        {
            snprintf(cmdIvictlPf, sizeof(cmdIvictlPf),
                    "ivictl -d -a %s -p %d -q %d -P %s ",
                    ipAddress, start_port+index, start_port+index, mapt_protocol_both);
#ifdef FEATURE_MAPT_DEBUG
            LOG_PRINT_IVICTL("ivictl:both: %s",cmdIvictlPf);
#endif
            if (system(cmdIvictlPf) != 0)
            {
#ifdef FEATURE_MAPT_DEBUG
                LOG_PRINT_IVICTL("Error Deleting the Rule");
#endif
                return RET_ERR;
            }
            memset(cmdIvictlPf, 0, sizeof(cmdIvictlPf));
        }
    }
#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("Deleting Range Port Forward Rule --- End  ");
#endif
    return ANSC_STATUS_SUCCESS;
}
#endif
#endif
#endif

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatDelPortMapping
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    Description:
        The API removes the designated port mapping entry from NAT.
    Arguments:
        pAlias      The entry is identified through Alias.
    Return:
        The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatDelPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext                   Ctx;
    portFwdSingle_t          singleInfo;
    portFwdRange_t            rangeInfo;

    ULONG                            rc = 0;
   
    if ( ulInstanceNumber < 1 )
    {
        return ANSC_STATUS_FAILURE;
    }
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        syslog_systemlog("PortMapping", LOG_NOTICE, "%s Error initializing context\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }
    singleInfo.rule_id = ulInstanceNumber;
    rc = Utopia_GetPortForwardingByRuleId(&Ctx, &singleInfo);
    if(SUCCESS == rc) 
    {
        rc = Utopia_DelPortForwardingByRuleId(&Ctx, ulInstanceNumber);
        if(SUCCESS == rc)
        {
            Utopia_Free(&Ctx, 1);
#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
            int status ;
            status = delete_mapt_pf_single_rule(singleInfo.internal_port, singleInfo.external_port, singleInfo.protocol, singleInfo.dest_ip);
            if (status == RET_ERR)
#ifdef FEATURE_MAPT_DEBUG
                LOG_PRINT_IVICTL("Error Deleting Port Forwarding Single Rule");
#endif
#endif
#endif
#endif
            _sent_syslog_pm_u("DELETE", singleInfo.protocol, singleInfo.external_port, 0, \
                    singleInfo.internal_port, singleInfo.dest_ip, singleInfo.enabled);
            return ANSC_STATUS_SUCCESS;
        }
        else
        {
            CcspTraceWarning(("%s Error DelPortForwardingByRuleId rc %lu\n", __FUNCTION__, rc));
            _sent_syslog_pm_u("DELETE FAILED", singleInfo.protocol, singleInfo.external_port, 0, \
                    singleInfo.internal_port, singleInfo.dest_ip, singleInfo.enabled);
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
        }
    }
    AnscZeroMemory(&rangeInfo, sizeof(portFwdRange_t)); /*RDKB-6842, CID-32930, initializing*/
    rangeInfo.rule_id = ulInstanceNumber;
    rc = Utopia_GetPortForwardingRangeByRuleId(&Ctx, &rangeInfo);
    if(SUCCESS == rc)
    {
        rc = Utopia_DelPortForwardingRangeByRuleId(&Ctx, ulInstanceNumber);
        if(SUCCESS == rc)
        {
            Utopia_Free(&Ctx, 1);
#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
            int status ;
            status = delete_mapt_pf_range_rule(rangeInfo.start_port, rangeInfo.end_port, rangeInfo.protocol, rangeInfo.dest_ip);
            if (status == RET_ERR)
#ifdef FEATURE_MAPT_DEBUG
                LOG_PRINT_IVICTL("Error Deleting Port Forwarding Range Rule");
#endif
#endif
#endif
#endif
            _sent_syslog_pm_u("DELETE", rangeInfo.protocol, rangeInfo.start_port, rangeInfo.end_port,\
                    rangeInfo.internal_port, rangeInfo.dest_ip, rangeInfo.enabled);
            return ANSC_STATUS_SUCCESS;
        }
        else
        {
            CcspTraceWarning(("%s Error DelPortForwardingRangeByRuleId rc %lu\n", __FUNCTION__, rc));
            _sent_syslog_pm_u("DELETE FAILED", rangeInfo.protocol, rangeInfo.start_port, rangeInfo.end_port,\
                    rangeInfo.internal_port, rangeInfo.dest_ip, rangeInfo.enabled);
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
        }
    }

#if 0
    ULONG                       ulIndex = 0;
    portMapDyn_t                dynInfo = {0};
    int                         PortFwdDynCount = 0;
    Utopia_GetDynPortMappingCount((int*)&PortFwdDynCount);
    PortFwdDynCount = (PortFwdDynCount < g_NatPortFwdDynInstanceNumCount ? PortFwdDynCount : g_NatPortFwdDynInstanceNumCount);

    for ( ulIndex = 0; ulIndex < PortFwdDynCount; ulIndex++ )
    {
        if ( g_NatPortFwdDynInstanceNum[ulIndex] == ulInstanceNumber )
        {
            break;
        }
    }
    if ( ulIndex != PortFwdDynCount )
    {
        rc = Utopia_GetDynPortMapping(ulIndex + 1, &dynInfo);
        if ( rc != UT_SUCCESS )
        {
            CcspTraceWarning(("Utopia_GetDynPortMapping failed rc %d in %s\n", rc, __FUNCTION__));
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
        }

        unsetID
        (
            &Ctx,
            dynInfo.external_port,
            dynInfo.protocol
        );

        rc = Utopia_DeleteDynPortMappingIndex(ulIndex + 1);
        if ( rc != UT_SUCCESS )
        {
            CcspTraceWarning(("Utopia_DeleteDynPortMappingIndex failed rc %d in %s\n", rc, __FUNCTION__));
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
        }

        g_NatPortFwdDynInstanceNum[ulIndex] = 0;
        Utopia_Free(&Ctx, 1);
        return ANSC_STATUS_SUCCESS;
    }
#endif
    CcspTraceInfo(("Nothing to delete in %s\n", __FUNCTION__));
    Utopia_Free(&Ctx, 0);
    return ANSC_STATUS_FAILURE;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSetPortMappings
            (
                ANSC_HANDLE                 hThisObject
            );

    Description:
        The API re-configures the designated port mapping entry.
    Arguments:
        pAlias      The entry is identified through Alias.
        pEntry      The new configuration is passed through this argument, even Alias field can be changed.

    Return:
    The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatSetPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry          /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext                   Ctx;
    portFwdSingle_t          singleInfo;
    portFwdRange_t           rangeInfo;
    ULONG                            rc = 0;

#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
    bool singleRule = FALSE;
#endif    
#endif    
#endif    
    if (!pEntry)
    {
        return ANSC_STATUS_FAILURE;
    }
    
    /* Check parameter */
#if defined (MULTILAN_FEATURE)
    if(pEntry->bEnabled && !_Check_PF_parameter(pEntry))
#else
    if( !_Check_PF_parameter(pEntry))
#endif
    {
        CcspTraceWarning(("Parameter Error in %s \n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    if(!CosaDmlNatChkEnableFlg(pEntry)){
        return ANSC_STATUS_FAILURE;
    }
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        syslog_systemlog("PortMapping", LOG_NOTICE, "%s Error initializing context\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    /* Check/delete range rule */
#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
#ifdef FEATURE_MAPT_DEBUG
    LOG_PRINT_IVICTL("New pEntry protocol: %d, ExternalPort :%d, ExternalEndRange : %d, InternalPort : %d,  bEnable :%d ",
                    pEntry->Protocol, pEntry->ExternalPort, pEntry->ExternalPortEndRange, pEntry->InternalPort,
                    pEntry->bEnabled); 
#endif    
#endif    
#endif    
#endif    

    rangeInfo.rule_id = pEntry->InstanceNumber;
    rc = Utopia_GetPortForwardingRangeByRuleId(&Ctx, &rangeInfo);
#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
    if (rc == SUCCESS)
    {    
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL(">>Range-old-params<< ~protocol: %d, ExternalPort :%d, ExternalEndRange : %d, InternalPort : %d,  bEnable :%d ",
                rangeInfo.protocol, rangeInfo.start_port, rangeInfo.end_port, rangeInfo.internal_port,
                rangeInfo.enabled);
        LOG_PRINT_IVICTL("Range old-ip Destip [%s]",rangeInfo.dest_ip);
        CcspTraceWarning((">>AT :%d>> Range >> New Port Mapping parameter external Port %d ~ %d, InternalPort %d, protocol %d, Enabled %d, DestIP [%02u.%02u.%02u.%02u]\n", \
                __LINE__,pEntry->ExternalPort, pEntry->ExternalPortEndRange, pEntry->InternalPort, \
                pEntry->Protocol, pEntry->bEnabled, \
                pEntry->InternalClient.Dot[0],pEntry->InternalClient.Dot[1],pEntry->InternalClient.Dot[2],pEntry->InternalClient.Dot[3] ));
#endif
        singleRule = FALSE;
    }
#endif
#endif
#endif    

    if(rc == SUCCESS)
    {
        rc = Utopia_DelPortForwardingRangeByRuleId(&Ctx, pEntry->InstanceNumber);
        if(rc != SUCCESS){ 
            CcspTraceWarning((" Utopia_DelPortForwardingRangeByRuleId failed rc %lu in %s\n", rc, __FUNCTION__));
            _sent_syslog_pm_sb("EDIT FAILED", pEntry->Protocol, pEntry->ExternalPort, pEntry->ExternalPortEndRange, pEntry->InternalPort, pEntry->InternalClient.Dot,pEntry->bEnabled);
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
        }
    }

    /* check/delete single rule */
    singleInfo.rule_id = pEntry->InstanceNumber;
    rc = Utopia_GetPortForwardingByRuleId(&Ctx, &singleInfo);
    
#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
    if (rc == SUCCESS)
    {
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL(">>Single>>>Old Port Mapping parameter external Port %d ~ %d, InternalPort %d, protocol %d, Enabled %d ", 
                singleInfo.external_port, singleInfo.internal_port, singleInfo.internal_port, 
                singleInfo.protocol,singleInfo.enabled);
        LOG_PRINT_IVICTL("Single old-ip Destip [%s]",singleInfo.dest_ip);
        LOG_PRINT_IVICTL("Dest_ip : %s", singleInfo.dest_ip);
#endif
        singleRule = TRUE;
    }
#endif
#endif
#endif

    if(rc == SUCCESS)    
    {
       rc = Utopia_DelPortForwardingByRuleId(&Ctx, pEntry->InstanceNumber);
       if(rc != SUCCESS){
            CcspTraceWarning((" Utopia_DelPortForwardingByRuleId failed rc %lu in %s\n", rc, __FUNCTION__));
            _sent_syslog_pm_sb("EDIT FAILED", pEntry->Protocol, pEntry->ExternalPort, pEntry->ExternalPortEndRange, pEntry->InternalPort, pEntry->InternalClient.Dot,pEntry->bEnabled);
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
       }
    }

    if( ANSC_STATUS_SUCCESS == _AddPortMapping(&Ctx, pEntry))
    {
        Utopia_Free(&Ctx, 1);
#ifdef _HUB4_PRODUCT_REQ_
#ifdef FEATURE_MAPT
#ifdef IVI_KERNEL_SUPPORT
#ifdef FEATURE_MAPT_DEBUG
        LOG_PRINT_IVICTL("!!!! ADDING THE RULES !!!!");
#endif
        /*Delete the previous rule*/
        if (singleRule == TRUE)
        {
            int status ;
            status = delete_mapt_pf_single_rule(singleInfo.internal_port, singleInfo.external_port, singleInfo.protocol, singleInfo.dest_ip);
            if (status == RET_ERR)
#ifdef FEATURE_MAPT_DEBUG
                LOG_PRINT_IVICTL("Error Deleting Port Forwarding Single Rule");
#endif
            CcspTraceWarning((" Error Deleting Port Forwarding Single Rule %s\n",  __FUNCTION__));
        }
        else
        {
            int status;
            status = delete_mapt_pf_range_rule(rangeInfo.start_port, rangeInfo.end_port, rangeInfo.protocol, rangeInfo.dest_ip);
            if (status == RET_ERR)
#ifdef FEATURE_MAPT_DEBUG
                LOG_PRINT_IVICTL("Error Deleting Port Forwarding Range Rule");
#endif
            CcspTraceWarning((" Error Deleting Port Forwarding Range Rule %s\n",  __FUNCTION__));
        }
#endif
#endif
#endif

        _sent_syslog_pm_sb("ADD", pEntry->Protocol, pEntry->ExternalPort, pEntry->ExternalPortEndRange, pEntry->InternalPort, pEntry->InternalClient.Dot,pEntry->bEnabled);
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        Utopia_Free(&Ctx, 1);
        _sent_syslog_pm_sb("ADD FAILED", pEntry->Protocol, pEntry->ExternalPort, pEntry->ExternalPortEndRange, pEntry->InternalPort, pEntry->InternalClient.Dot,pEntry->bEnabled);
        return ANSC_STATUS_FAILURE;
    }

    /* dynPortFwd should not be setting at DM */
#if 0
    int                 PortFwdDynCount = 0;
    ULONG                       ulIndex = 0;
    portMapDyn_t                dynInfo;
    errno_t              safec_rc = -1;
    /* Check if it is a dynPortFwd */
    Utopia_GetDynPortMappingCount(&PortFwdDynCount);
    PortFwdDynCount = (PortFwdDynCount < g_NatPortFwdDynInstanceNumCount ? PortFwdDynCount : g_NatPortFwdDynInstanceNumCount);

    for ( ulIndex = 0; ulIndex < PortFwdDynCount; ulIndex++ )
    {
        if ( g_NatPortFwdDynInstanceNum[ulIndex] == pEntry->InstanceNumber )
        {
            break;
        }
    }
    if ( ulIndex != PortFwdDynCount )
    {
        rc = Utopia_GetDynPortMapping(ulIndex + 1, &dynInfo);
        if ( rc != UT_SUCCESS )
        {
            CcspTraceWarning(("Utopia_GetDynPortMapping failed rc %d in %s\n", rc, __FUNCTION__));
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
        }

        unsetID
        (
            &Ctx,
            dynInfo.external_port,
            dynInfo.protocol
        );

        dynInfo.enabled = pEntry->bEnabled;
        dynInfo.external_port = pEntry->ExternalPort;
        dynInfo.internal_port = pEntry->InternalPort;
        dynInfo.lease = pEntry->LeaseDuration;
        dynInfo.protocol = SB_2_U_PF_PPOTOCOL(pEntry->Protocol);

        AnscCopyString(dynInfo.name, pEntry->Description);
        safec_rc = sprintf_s(dynInfo.internal_host, sizeof(dynInfo.internal_host),
                        "%u.%u.%u.%u",
                        pEntry->InternalClient.Dot[0],
                        pEntry->InternalClient.Dot[1],
                        pEntry->InternalClient.Dot[2],
                        pEntry->InternalClient.Dot[3]
                     );
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
        }

        rc = s_UpdateDynPortMapping(ulIndex + 1, &dynInfo);
        if ( rc != UT_SUCCESS )
        {
            CcspTraceWarning(("Utopia_UpdateDynPortMapping failed rc %d in %s\n", rc, __FUNCTION__));
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
        }
        saveID
        (
            &Ctx,
            pEntry->ExternalPort,
            pEntry->Protocol - 1,
            pEntry->Alias,
            pEntry->InstanceNumber
        );

        g_NatPortFwdDynInstanceNum[ulIndex] = pEntry->InstanceNumber;
        Utopia_Free(&Ctx, 1);
        return ANSC_STATUS_SUCCESS;
    }
#endif
    /*CID: 62430 Structurally dead code*/
}
#ifdef CONFIG_CISCO_HOME_SECURITY 
#define HNAP_DHCP_POOL_DM "Device.DHCPv4.Server.Pool.2."

BOOL _Check_HS_PF_client(ULONG client)
{
    ULONG ipaddr = 0xffffffff, netmask = 0xffffffff;
    char strval[64];
    ULONG size;
    size = sizeof(strval);
    if(0 != g_GetParamValueString(g_pDslhDmlAgent, HNAP_DHCP_POOL_DM "IPRouters", strval, &size))
        return FALSE;
    inet_pton(AF_INET, strval, &ipaddr);

    size = sizeof(strval);
    if(0 != g_GetParamValueString(g_pDslhDmlAgent, HNAP_DHCP_POOL_DM "SubnetMask", strval, &size))
        return FALSE;
    inet_pton(AF_INET, strval, &netmask);
    if((client != ipaddr) &&
        !IPv4Addr_IsBroadcast(client, ipaddr, netmask) &&
        !IPv4Addr_IsNetworkAddr(client, ipaddr, netmask) &&
        IPv4Addr_IsSameNetwork(client, ipaddr, netmask) )
        return TRUE; 
    else
        return FALSE;
}
#endif
#define IS_ONE2ONE_NAT(pEntry) (pEntry->PublicIP.Value != 0)
#define IS_PORTFWD(pEntry)      (pEntry->InternalPort == 0 && pEntry->PublicIP.Value == 0)
#define IS_HS_PORTFWD(pEntry)  (pEntry->InternalPort != 0 && pEntry->PublicIP.Value == 0) 
BOOL CosaDmlNatChkEnableFlg(PCOSA_DML_NAT_PMAPPING pPortMapping)
{
    if( IS_ONE2ONE_NAT(pPortMapping) && g_NatOne2OneEnable == 0 ){
        CcspTraceWarning(("Cannot Add nat rule when nat is disable"));
        syslog_systemlog("PortMapping", LOG_NOTICE, "Cannot Add nat rule when nat is disable");
        return FALSE;
    }
    /* Cannot set port forward rule when port forward is disable */
    if(IS_PORTFWD(pPortMapping) && g_NatPFEnable == 0)
    {
        CcspTraceWarning(("Cannot Add port forward rule when port forwarding is disable"));
        syslog_systemlog("PortMapping", LOG_NOTICE, "Cannot Add port forward rule when port forwarding is disable");
        return FALSE;
    }
    /* Cannot set HS port forward rule when HS port forward is disble */
    if(IS_HS_PORTFWD(pPortMapping) && g_NatHSPFEnable == 0)
    {
        CcspTraceWarning(("Cannot Add HS port forward rule when HS port forwarding is disable"));
        syslog_systemlog("PortMapping", LOG_NOTICE, "Cannot Add HS port forward rule when HS port forwarding is disable");
        return FALSE;
    }
    return TRUE; 
}
/*eg. 2042:cafe:0:5::/64 */
BOOL _get_prefix(char* in_prefix, ipv6Prefix_t * out)
{
	char *ptr, c = '/';
	int length = 0;
	ptr = strchr(in_prefix, c); /* should only contain one '/'*/
	if (ptr == 0)
	{
		return FALSE;
	} else {
		length = ptr - in_prefix;
		memcpy(out->prefix, in_prefix, length); /* copy 2042:cafe:0:5:: */
		out->prefix[length] = '\0';
		out->size = atoi(ptr +1); /* assigned 64 */
		return TRUE;
	}
}
BOOL _compare_last_byte(char *a, char *b, int nbits) {
	if (nbits == 0)
		return TRUE;
	if ((*a >> (8 - nbits)) != (*b >> (8 - nbits))) {
		return FALSE;
	} else {
		return TRUE;
	}
}


BOOL CosaDmlNatChkPortMappingIPV6Address(char* address)
{
	char ipv6_prefix[IPADDR_SZ] = {0};
	char ipv6_address[IPADDR_SZ] = {0};
    char wan_ip6[IPADDR_SZ] = {0};
    UtopiaContext Ctx;
	ipv6Info_t ipv6_info;
	ipv6Prefix_t prx_t;
	int nbytes = 0, nbits = 0;
	BOOL ret;
    int i;
    ipv6_addr_info_t * p_v6addr = NULL;
    int  v6addr_num = 0;
	
    if(*address == 'x'){
		return TRUE; /*ipv6 not set, default is 'x'*/
	}

    if(inet_pton(AF_INET6, address, ipv6_address)!=1){
		CcspTraceWarning(("%s Error ipv6 address context\n", __FUNCTION__));
        return FALSE;
	}	

    /* cannot set wan IPv6 address */
    CosaUtilGetIpv6AddrInfo("erouter0", &p_v6addr, &v6addr_num);
    for(i = 0; i < v6addr_num; i++ )
    {
        if(p_v6addr[i].scope == IPV6_ADDR_SCOPE_GLOBAL)
        {
           inet_pton(AF_INET6, p_v6addr[i].v6addr, wan_ip6);
           if(0 == memcmp(wan_ip6, ipv6_address, sizeof(wan_ip6)))
            {
                CcspTraceWarning(("%s Error ipv6 address same as wan ipv6 address\n", __FUNCTION__));
                free(p_v6addr);
                return FALSE;
            } 
        } 
    } 
    if(p_v6addr)
        free(p_v6addr);

    bzero(&prx_t, sizeof(prx_t));
    bzero(&ipv6_info, sizeof(ipv6_info));
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return FALSE;
    }
	Utopia_GetIPv6Settings(&Ctx, &ipv6_info);
	Utopia_Free(&Ctx, 0);
	/*ipv6_prefix should look like this 2042:cafe:0:5::/64 */
	ret = _get_prefix(ipv6_info.ipv6_prefix, &prx_t);
	if(!ret){
		return TRUE; /*invalid prefix or prefix not set, do not check */
	}
	if(inet_pton(AF_INET6, prx_t.prefix, ipv6_prefix)!=1){
		/*change prefix to binaries */
		CcspTraceWarning(("%s Error ipv6 prefix context\n", __FUNCTION__));
	}
	nbytes = prx_t.size/8;
    nbits = prx_t.size%8;
	if((memcmp(ipv6_prefix, ipv6_address, prx_t.size/8) == 0) && 
		_compare_last_byte(ipv6_prefix+nbytes, ipv6_address+nbytes, nbits)) 
	{
		return TRUE;
	}
	return FALSE;	
}

#if defined (MULTILAN_FEATURE)
/* Find an IPv4 LAN data object for which the given client IP is part of the same subnet */
static BOOL _Find_IPv4_LAN_DML(ULONG client, ULONG *dml_ip_instance, ULONG *dml_address_instance)
{
    char name[MAX_QUERY] = {0};
    char param_val[MAX_QUERY] = {0};
    char ip_instance[MAX_QUERY] = {0};
    char addr_instance[MAX_QUERY] = {0};
    char ip_addr[MAX_QUERY] = {0};
    char ip_netmask[MAX_QUERY] = {0};
    int ip_interface_count = 0;
    int ip_address_count = 0;
    int i = 0;
    int j = 0;
    ULONG ip_inst_num = 0;
    ULONG addr_inst_num = 0;
    ULONG val_len = 0;
    ULONG ipaddr = 0xffffffff, netmask = 0xffffffff;
    errno_t rc = -1;

    char str[MAX_QUERY];
    inet_ntop(AF_INET, &client, str, INET_ADDRSTRLEN);

    ip_interface_count = g_GetParamValueUlong(g_pDslhDmlAgent, IP_INTERFACE_COUNT_DML);

    for (i=0; i<ip_interface_count; i++)
    {
        /* Get the next IP interface instance */
        ip_inst_num = g_GetInstanceNumberByIndex(g_pDslhDmlAgent, IP_INTERFACE_DML, i); 
        if (ip_inst_num)
        {
            /* Get ethernet link object */
            snprintf(ip_instance, sizeof(ip_instance), "%s%lu", IP_INTERFACE_DML, ip_inst_num);
            rc = sprintf_s(name, sizeof(name), "%s.LowerLayers", ip_instance);
            if(rc < EOK) ERR_CHK(rc);
            val_len = sizeof(param_val);
            if ( (0 == g_GetParamValueString(g_pDslhDmlAgent, name, param_val, &val_len)) && _ansc_strstr(param_val, "Device.Ethernet.Link"))
            {
                /* See if lower layer is a bridge (all LANs are bridges currenctly) */
                rc = sprintf_s(name, sizeof(name), "%s.LowerLayers", param_val);
                if(rc < EOK) ERR_CHK(rc);
                val_len = sizeof(param_val);
                if ( (0 == g_GetParamValueString(g_pDslhDmlAgent, name, param_val, &val_len)) && _ansc_strstr(param_val, "Device.Bridging.Bridge"))
                {
                    /* Now iterate through each IPv4 address */
                    rc = sprintf_s(name, sizeof(name), "%s.IPv4AddressNumberOfEntries", ip_instance);
                    if(rc < EOK) ERR_CHK(rc);
                    ip_address_count = g_GetParamValueUlong(g_pDslhDmlAgent, name);
                    for (j=0; j<ip_address_count; j++)
                    {
                        rc = sprintf_s(name, sizeof(name), "%s.IPv4Address.", ip_instance);
                        if(rc < EOK) ERR_CHK(rc);
                        addr_inst_num = g_GetInstanceNumberByIndex(g_pDslhDmlAgent, name, j);
                        if (addr_inst_num)
                        {
                            rc = sprintf_s(addr_instance, sizeof(addr_instance), "%s.IPv4Address.%lu", ip_instance, addr_inst_num);
                            if(rc < EOK) ERR_CHK(rc);
                            /* Get IPv4 address and netmask */
                            rc = sprintf_s(name, sizeof(name), "%s.IPAddress", addr_instance);
                            if(rc < EOK) ERR_CHK(rc);
                            val_len = sizeof(ip_addr);
                            if (g_GetParamValueString(g_pDslhDmlAgent, name, ip_addr, &val_len)) 
                                continue; 
                            rc = sprintf_s(name, sizeof(name), "%s.SubnetMask", addr_instance);
                            if(rc < EOK) ERR_CHK(rc);
                            val_len = sizeof(ip_netmask);
                            if (g_GetParamValueString(g_pDslhDmlAgent, name, ip_netmask, &val_len)) 
                                continue; 
                 

                            if( inet_pton(AF_INET, ip_addr, &ipaddr) && inet_pton(AF_INET, ip_netmask, &netmask) )
                            {
                                /* Check if client address is part of this subnet, if so, return IP instance ID */
                                if (IPv4Addr_IsSameNetwork(client, ipaddr, netmask)&&
                                   !IPv4Addr_IsBroadcast(client, ipaddr, netmask) &&
                                   !IPv4Addr_IsNetworkAddr(client, ipaddr, netmask))
                                {
                                    if(dml_ip_instance)
                                        *dml_ip_instance = ip_inst_num;
                                    if(dml_address_instance)
                                        *dml_address_instance = addr_inst_num;
                                    return TRUE;
                                }
                                else
                                {
                                }
                            }           
                        } 
                    }
                } 
            }
        } 
    }
    return FALSE;
}

/* Validate client ip address */
static BOOL validateClientIPAddress(ULONG client_ip_address)
{
    char pool_instance[MAX_QUERY] = {0};
    char ip_instance[MAX_QUERY] = {0};
    char param_val[MAX_QUERY] = {0};
    char name[MAX_QUERY] = {0};
    ULONG val_len = 0;
    ULONG i = 0;
    ULONG dhcp_server_pool_count = 0;
    ULONG pool_inst_num = 0;
    ULONG dml_ip_inst_num = 0;
    ULONG dhcp_ip_address_start = 0xffffffff;
    ULONG dhcp_ip_address_end = 0xffffffff;
    errno_t rc = -1;

    /* Find the IP instance */
    if(_Find_IPv4_LAN_DML(client_ip_address, &dml_ip_inst_num, NULL) && dml_ip_inst_num)
    {
        snprintf(ip_instance, sizeof(ip_instance), "%s%lu", IP_INTERFACE_DML, dml_ip_inst_num);

        /* Check dhcp range */
        dhcp_server_pool_count = g_GetParamValueUlong(g_pDslhDmlAgent, DHCPV4_SERVER_POOL_COUNT_DML);
        for(i = 0; i < dhcp_server_pool_count; i++)
        {
            pool_inst_num = g_GetInstanceNumberByIndex(g_pDslhDmlAgent, DHCPV4_SERVER_POOL_DML, i);

            if(pool_inst_num)
            {
                 snprintf(pool_instance, sizeof(pool_instance), "%s%lu", DHCPV4_SERVER_POOL_DML, pool_inst_num);
                 rc = sprintf_s(name, sizeof(name), "%s.Interface", pool_instance);
                 if(rc < EOK) ERR_CHK(rc);
                 val_len = sizeof(param_val);
                if((0 == g_GetParamValueString(g_pDslhDmlAgent, name, param_val, &val_len)) && _ansc_strstr(param_val, ip_instance))
                {
                    rc = sprintf_s(name, sizeof(name), "%s.Enable", pool_instance);
                    if(rc < EOK) ERR_CHK(rc);
                    if(g_GetParamValueBool(g_pDslhDmlAgent, name))
                    {
                        rc = sprintf_s(name, sizeof(name), "%s.MinAddress", pool_instance);
                        if(rc < EOK) ERR_CHK(rc);
                        val_len = sizeof(param_val);
                        if(g_GetParamValueString(g_pDslhDmlAgent, name, param_val, &val_len))
                            continue;

                        inet_pton(AF_INET, param_val, &dhcp_ip_address_end);
                        dhcp_ip_address_start = htonl(dhcp_ip_address_end);

                        rc = sprintf_s(name, sizeof(name), "%s.MaxAddress", pool_instance);
                        if(rc < EOK) ERR_CHK(rc);
                        val_len = sizeof(param_val);
                        if(g_GetParamValueString(g_pDslhDmlAgent, name, param_val, &val_len))
                            continue;

                        inet_pton(AF_INET, param_val, &dhcp_ip_address_end);
                        dhcp_ip_address_end = htonl(dhcp_ip_address_end);

                        client_ip_address = htonl(client_ip_address);
                        if(client_ip_address >= dhcp_ip_address_start && client_ip_address <= dhcp_ip_address_end)
                        {
                            return TRUE;
                        }
                    }
                }
            }
        }
    }

    return FALSE;
}
#endif

BOOL CosaDmlNatChkPortMappingClient(ULONG client)
{
#if !defined (MULTILAN_FEATURE)
    UtopiaContext                   Ctx;
    lanSetting_t lan;
    ULONG ipaddr = 0xffffffff, netmask = 0xffffffff;

    dhcpServerInfo_t dhcp;
    ULONG dhcpIPAddressStart  =0xffffffff, dhcpIPAddressEnd = 0xffffffff;

    ULONG startIP, endIP, checkIP;
#endif

    BOOL ret;

    /* Ipv4 client IP will be set as 255.255.255.255 if only need IPv6 portforwarding */
    if(client == 0xffffffff){
        return TRUE;
    }

#if !defined (MULTILAN_FEATURE)
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return FALSE;
    }

    /*Convert dhcp start and end address from text to binary form*/
    Utopia_GetDHCPServerSettings (&Ctx, &dhcp);
    inet_pton(AF_INET, dhcp.DHCPIPAddressStart, &dhcpIPAddressStart);
    inet_pton(AF_INET, dhcp.DHCPIPAddressEnd, &dhcpIPAddressEnd);

    /*Convert unsigned integer hostlong from host byte order to network byte order*/
    startIP = htonl(dhcpIPAddressStart);
    endIP = htonl(dhcpIPAddressEnd);
    checkIP = htonl(client);

    Utopia_GetLanSettings(&Ctx, &lan);
    Utopia_Free(&Ctx, 0);
    inet_pton(AF_INET, lan.ipaddr, &ipaddr);
    inet_pton(AF_INET, lan.netmask, &netmask);

    if((client != ipaddr) &&
	(checkIP >= startIP && checkIP <= endIP) && //check that client(checkIP) is in DHCP range or not
        !IPv4Addr_IsBroadcast(client, ipaddr, netmask) &&
        !IPv4Addr_IsNetworkAddr(client, ipaddr, netmask) &&
        (IPv4Addr_IsSameNetwork(client, ipaddr, netmask) || IPv4Addr_IsSameNetwork(client, 0xac100c00, 0xffffff00)))
#else
	if(validateClientIPAddress(client))
#endif
        ret =  TRUE; 
#ifdef CONFIG_CISCO_HOME_SECURITY 
    else if(_Check_HS_PF_client(client) == TRUE)
        ret = TRUE;
#endif
    else
        ret = FALSE;
    
    return ret;
}
/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_NAT_PTRIGGER
        CosaDmlNatGetPortTrigger
            (
                ANSC_HANDLE                 hContext,
                PULONG                      instanceNum
            )
        Description:
            This routine is to retrieve the NAT port trigger.

        Arguments:
            InstanceNum.

        Return:
            The pointer to NAT port trigger, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/

ANSC_STATUS
CosaDmlNatGetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        ULONG                       InstanceNum,
        PCOSA_DML_NAT_PTRIGGER      pNatPTrigger
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext                   Ctx = {0};
    portRangeTrig_t                 porttrigger;
    int                             rc           = 0;

    if ( pNatPTrigger == NULL )
    {
        CcspTraceWarning(("CosaDmlNatGetPortTrigger pTrigger is NULL!\n"));
        return ANSC_STATUS_FAILURE;
    }

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    AnscZeroMemory(&porttrigger, sizeof(portRangeTrig_t)); /*RDKB-6842, CID-33457, initialize before use*/

    porttrigger.rule_id = InstanceNum;
    rc = Utopia_GetPortTriggerByRuleId(&Ctx, &porttrigger);
    if(rc != SUCCESS){
        Utopia_Free(&Ctx, 0);
        return ANSC_STATUS_FAILURE;
    }

    pNatPTrigger->bEnabled          = porttrigger.enabled;
    pNatPTrigger->TriggerProtocol   = U_2_SB_PF_PROTOCOL(porttrigger.trigger_proto);
    pNatPTrigger->TriggerPortStart  = porttrigger.trigger_start;
    pNatPTrigger->TriggerPortEnd    = porttrigger.trigger_end;
    pNatPTrigger->ForwardProtocol   = U_2_SB_PF_PROTOCOL(porttrigger.forward_proto);
    pNatPTrigger->ForwardPortStart  = porttrigger.fwd_range_start;
    pNatPTrigger->ForwardPortEnd    = porttrigger.fwd_range_end;
    pNatPTrigger->InstanceNumber    = porttrigger.rule_id;
//        AnscCopyString(pNatPTrigger->Alias, porttrigger.name);
//        AnscCopyString(pNatPTrigger->Description, porttrigger.name);
    strncpy(pNatPTrigger->Alias, porttrigger.name, sizeof(pNatPTrigger->Alias));
    strncpy(pNatPTrigger->Description, porttrigger.name,sizeof(pNatPTrigger->Description));

    Utopia_Free(&Ctx, 0);
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_NAT_PTRIGGER
        CosaDmlNatGetPortTriggers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount,
                BOOLEAN                     bCommit
            )
        Description:
            This routine is to retrieve the complete list of NAT port triggers, which is a table.

        Arguments:
            pulCount  is to receive the actual number of entries.

        Return:
            The pointer to the array of NAT port triggers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/

PCOSA_DML_NAT_PTRIGGER
CosaDmlNatGetPortTriggers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        BOOLEAN                     bCommit
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext                   Ctx;
    PCOSA_DML_NAT_PTRIGGER          pNatPTrigger = (PCOSA_DML_NAT_PTRIGGER)NULL;
    portRangeTrig_t *               porttrigger  = (portRangeTrig_t *     )NULL;
    ULONG                           ulIndex      = 0;
    int                             rc           = 0;
    int                             count = 0;
    if ( !pulCount )
    {
        CcspTraceWarning(("CosaDmlNatGetPortTriggers pulCount is NULL!\n"));
        return NULL;
    }

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));

        *pulCount  =  0 ;
        return NULL;
    }

    rc = Utopia_GetPortTrigger(&Ctx, &count, &porttrigger);
    if ( rc != SUCCESS )
    {
        CcspTraceWarning(("Utopia_GetPortTrigger failed rc %d in %s\n", rc, __FUNCTION__));
        *pulCount = 0;
        Utopia_Free(&Ctx, 0);
        return NULL;
    }

    *pulCount = count;
    if (*pulCount == 0)
    {
        if(porttrigger) /*RDKB-6842, CID-33375, free ununsed resources before exit*/
            free(porttrigger);
        Utopia_Free(&Ctx, 0);
        return NULL;
    }

    pNatPTrigger = AnscAllocateMemory(sizeof(COSA_DML_NAT_PTRIGGER) * (*pulCount));
    if ( !pNatPTrigger )
    {
        CcspTraceError(("CosaDmlNatGetPortTriggers not enough memory!\n"));
        if(porttrigger) /*RDKB-6842, CID-33375, free ununsed resources before exit*/
            free(porttrigger);
        *pulCount = 0;
        Utopia_Free(&Ctx, 0);
        return NULL;
    }

   
    for ( ulIndex = 0; ulIndex < *pulCount; ulIndex++ )
    {
        pNatPTrigger[ulIndex].bEnabled          = porttrigger[ulIndex].enabled;
        pNatPTrigger[ulIndex].TriggerProtocol   = U_2_SB_PF_PROTOCOL(porttrigger[ulIndex].trigger_proto);
        pNatPTrigger[ulIndex].TriggerPortStart  = porttrigger[ulIndex].trigger_start;
        pNatPTrigger[ulIndex].TriggerPortEnd    = porttrigger[ulIndex].trigger_end;
        pNatPTrigger[ulIndex].ForwardProtocol   = U_2_SB_PF_PROTOCOL(porttrigger[ulIndex].forward_proto);
        pNatPTrigger[ulIndex].ForwardPortStart  = porttrigger[ulIndex].fwd_range_start;
        pNatPTrigger[ulIndex].ForwardPortEnd    = porttrigger[ulIndex].fwd_range_end;
        pNatPTrigger[ulIndex].InstanceNumber    = porttrigger[ulIndex].rule_id;
//        AnscCopyString(pNatPTrigger[ulIndex].Alias, porttrigger[ulIndex].name);
//        AnscCopyString(pNatPTrigger[ulIndex].Description, porttrigger[ulIndex].name);
        /* CID 185685 fix */
	strncpy(pNatPTrigger[ulIndex].Alias, porttrigger[ulIndex].name, (sizeof(pNatPTrigger[ulIndex].Alias)-1));
        strncpy(pNatPTrigger[ulIndex].Description, porttrigger[ulIndex].name,(sizeof(pNatPTrigger[ulIndex].Description)-1));
    }


    Utopia_Free(&Ctx, bCommit);
    free(porttrigger);
    return pNatPTrigger;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatAddPortTrigger
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            )

    Description:
        The API adds one port trigger entry into NAT.

    Arguments:
        pEntry      Caller does not need to fill in Status or Alias fields. Upon return, callee fills in the generated Alias and associated Status.

    Return:
        Status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlNatAddPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_FAILURE;
    UtopiaContext                   Ctx;
    portRangeTrig_t                 porttriggerNew;
    ULONG                           rc                  = 0;

    if (!pEntry)
    {
        goto EXIT1;
    }

    if(g_NatPTTriggerEnable == 0)
    {
        CcspTraceWarning(("Cannot Add port trigger rule when port trigger disable\n"));
        goto EXIT1;
    }
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        goto EXIT1;
    }
    porttriggerNew.enabled           = pEntry->bEnabled;
	porttriggerNew.prevRuleEnabledState = pEntry->bEnabled;
    porttriggerNew.trigger_proto     = SB_2_U_PF_PPOTOCOL(pEntry->TriggerProtocol);
    /* Because SNMP cannot set forward proto, set forward proto = trigger proto */
    //    porttriggerNew.forward_proto     = SB_2_U_PF_PPOTOCOL(pEntry->ForwardProtocol);
    porttriggerNew.forward_proto    = porttriggerNew.trigger_proto; 
    porttriggerNew.trigger_start     = pEntry->TriggerPortStart;
    porttriggerNew.trigger_end       = pEntry->TriggerPortEnd;
    porttriggerNew.fwd_range_start   = pEntry->ForwardPortStart;
    porttriggerNew.fwd_range_end     = pEntry->ForwardPortEnd;
    porttriggerNew.rule_id           = pEntry->InstanceNumber;
    //AnscCopyString(porttriggerNew.name, pEntry->Description);
    strncpy(porttriggerNew.name, pEntry->Description, sizeof(porttriggerNew.name));
    rc = Utopia_AddPortTrigger(&Ctx, &porttriggerNew); 

    if ( rc != SUCCESS )
    {
        CcspTraceWarning(("CosaDmlNatAddPortTrigger failed rc %lu in %s\n", rc, __FUNCTION__));
        Utopia_Free(&Ctx, 0);
        _sent_syslog_pt_sb("ADD FAILED", pEntry->TriggerProtocol, pEntry->TriggerPortStart, pEntry->TriggerPortEnd, pEntry->ForwardPortStart, pEntry->ForwardPortEnd, pEntry->bEnabled);
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else
    {
//        returnStatus = _Update_TriggerEnable(&Ctx, porttriggerNew.enabled); 
        Utopia_Free(&Ctx, 1);
        _sent_syslog_pt_sb("ADD", pEntry->TriggerProtocol, pEntry->TriggerPortStart, pEntry->TriggerPortEnd, pEntry->ForwardPortStart, pEntry->ForwardPortEnd, pEntry->bEnabled);
        return ANSC_STATUS_SUCCESS;
    }
EXIT1:
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatDelPortTrigger
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            );

    Description:
        The API removes the designated port trigger entry from NAT.
    Arguments:
        pAlias      The entry is identified through Alias.
    Return:
        Status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlNatDelPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext                   Ctx;
    int                             rc                  = 0;

    if ( !pEntry )
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        syslog_systemlog("PortTriggering", LOG_NOTICE, "%s Error initializing context", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }
    
    rc = Utopia_DelPortTriggerByRuleId(&Ctx, pEntry->InstanceNumber);
    if ( rc == SUCCESS )
    {
        Utopia_Free(&Ctx, 1);
        _sent_syslog_pt_sb("DELETE", pEntry->TriggerProtocol, pEntry->TriggerPortStart, pEntry->TriggerPortEnd, pEntry->ForwardPortStart, pEntry->ForwardPortEnd, pEntry->bEnabled);
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        CcspTraceWarning(("Utopia_DelPortTriggerByRuleId failed rc %d in %s\n", rc, __FUNCTION__));
        Utopia_Free(&Ctx, 0);
        _sent_syslog_pt_sb("DELETE FAILED", pEntry->TriggerProtocol, pEntry->TriggerPortStart, pEntry->TriggerPortEnd, pEntry->ForwardPortStart, pEntry->ForwardPortEnd, pEntry->bEnabled);
        return ANSC_STATUS_FAILURE;
    }
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSetPortTrigger
            (
                ANSC_HANDLE                 hThisObject,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            );

    Description:
        The API re-configures the designated port trigger entry.
    Arguments:
        pAlias      The entry is identified through Alias.
        pEntry      The new configuration is passed through this argument, even Alias field can be changed.

    Return:
        Status of the operation

**********************************************************************/

ANSC_STATUS
CosaDmlNatSetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry          /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext                   Ctx;
    portRangeTrig_t                 porttrigger;
    int                             rc                  = 0;

    if ( !pEntry )
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

//    if( ! _Check_PT_parameter(pEntry)){
//        CcspTraceWarning(("Parameter Error in %s \n", __FUNCTION__));
//        return ANSC_STATUS_BAD_PARAMETER;
//    }

    if(g_NatPTTriggerEnable == 0)
    {
        CcspTraceWarning(("Cannot set port trigger rule when port trigger disabled\n"));
        syslog_systemlog("PortTriggering", LOG_NOTICE, "Cannot set port trigger rule when port trigger disabled");
        return ANSC_STATUS_FAILURE;
    }
    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        syslog_systemlog("PortTriggering", LOG_NOTICE, "%s Error initializing context", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    porttrigger.enabled           = pEntry->bEnabled;
	porttrigger.prevRuleEnabledState = pEntry->bEnabled;
    porttrigger.trigger_proto     = SB_2_U_PF_PPOTOCOL(pEntry->TriggerProtocol);
    porttrigger.forward_proto     =  porttrigger.trigger_proto;
    //    porttrigger.forward_proto     = SB_2_U_PF_PPOTOCOL(pEntry->ForwardProtocol);
    porttrigger.trigger_start     = pEntry->TriggerPortStart;
    porttrigger.trigger_end       = pEntry->TriggerPortEnd;
    porttrigger.fwd_range_start   = pEntry->ForwardPortStart;
    porttrigger.fwd_range_end     = pEntry->ForwardPortEnd;
    porttrigger.rule_id           = pEntry->InstanceNumber;
    //AnscCopyString(porttrigger.name, pEntry->Description);
    strncpy(porttrigger.name, pEntry->Description, sizeof(porttrigger.name));

    rc = Utopia_SetPortTriggerByRuleId(&Ctx, &porttrigger);
    if ( rc == SUCCESS )
    {
//        rc = _Update_TriggerEnable(&Ctx, porttrigger.enabled);
        Utopia_Free(&Ctx, 1);
        _sent_syslog_pt_sb("EDIT", pEntry->TriggerProtocol, pEntry->TriggerPortStart, pEntry->TriggerPortEnd, pEntry->ForwardPortStart, pEntry->ForwardPortEnd, pEntry->bEnabled);
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        CcspTraceWarning(("Utopia_SetPortTriggerByRuleId failed rc %d in %s\n", rc, __FUNCTION__));
        Utopia_Free(&Ctx, 0);
        _sent_syslog_pt_sb("EDIT FAILED", pEntry->TriggerProtocol, pEntry->TriggerPortStart, pEntry->TriggerPortEnd, pEntry->ForwardPortStart, pEntry->ForwardPortEnd, pEntry->bEnabled);
        return ANSC_STATUS_FAILURE;
    }
}

ANSC_STATUS
CosaDmlNatGetPortTriggerEnable(BOOL *pBool)
{
    *pBool = g_NatPTTriggerEnable;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlNatSetPortTriggerEnable(BOOL vBool)
{
    portRangeTrig_t                    *porttrigger = NULL;
    int                                 count;
    UtopiaContext                       Ctx;
    int                                 i;
    int                                 rc;

    if(vBool == g_NatPTTriggerEnable)
        return ANSC_STATUS_SUCCESS;

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    /* Gat all port trigger rules */
    rc = Utopia_GetPortTrigger(&Ctx, &count, &porttrigger);
    if ( rc != SUCCESS )
    {
        CcspTraceWarning(("Utopia_GetPortTrigger failed rc %d in %s\n", rc, __FUNCTION__));
        Utopia_Free(&Ctx, 0);
        return ANSC_STATUS_FAILURE;
    }

    /* change each rules enabled status */
    for(i = 0; i < count; i++)
    {
		/* Need to assign the backup value during port triggering enable case */
		if( TRUE == vBool )
		{
			porttrigger[i].enabled = porttrigger[i].prevRuleEnabledState;    
		}
		else
		{
			porttrigger[i].enabled = vBool;    
		}
    }

    /* set rules */
    if(count > 0 ){
        rc = Utopia_SetPortTrigger(&Ctx, count, porttrigger);
        free(porttrigger);
        if ( rc != SUCCESS )
        {
            CcspTraceWarning(("Utopia_SetPortTrigger failed rc %d in %s\n", rc, __FUNCTION__));
            Utopia_Free(&Ctx, 0);
            return ANSC_STATUS_FAILURE;
        }
    }

    /* store the PortTrigger enabled status */
    rc = Utopia_RawSet(&Ctx, COSA_NAT_SYSCFG_NAMESPACE, PORT_TRIGGER_ENABLED_KEY, (vBool == TRUE)? "1" : "0");
    if ( rc != 1 )
    {
        CcspTraceWarning(("Utopia_RawSet failed rc %d in %s\n", rc, __FUNCTION__));
        Utopia_Free(&Ctx, 1);
        return ANSC_STATUS_FAILURE; 
    }

    g_NatPTTriggerEnable = vBool;
    Utopia_Free(&Ctx, 1);
    return ANSC_STATUS_SUCCESS;
}

int CosaDmlNatChkPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger)
{
    UtopiaContext Ctx;
    int ret;
    if(enabled == FALSE)
        return TRUE;

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    ret = Utopia_CheckPortRange(&Ctx, (int)InstanceNumber, start, end, SB_2_U_PF_PPOTOCOL(protocol), is_trigger);
    Utopia_Free(&Ctx, 0);

    if(ret == FALSE)
        CcspTraceWarning(("%s Error port over range\n", __FUNCTION__));

    return ret; // return FALSE if port over range
}

int CosaDmlNatChkTriggerPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger)
{
    UtopiaContext Ctx;
    int ret;

    if(enabled == FALSE)
        return TRUE;

    if (!Utopia_Init(&Ctx))
    {
        CcspTraceWarning(("%s Error initializing context\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    ret = Utopia_CheckPortTriggerRange(&Ctx, (int)InstanceNumber, start, end, SB_2_U_PF_PPOTOCOL(protocol), is_trigger);
    Utopia_Free(&Ctx, 0);

    if(ret == FALSE)
        CcspTraceWarning(("%s Error port over range\n", __FUNCTION__));

    return ret; // return FALSE if port over range
}

int CosaDmlChkDesp(char *desp)
{
    if(desp == NULL ||
       desp[0] == '\0' || 
       desp[0] == ' ')
        return FALSE;
    return TRUE;
} 

void CosaDmlNatDelDynPortMappings (void)
{
    int index, count = 0;

    Utopia_GetDynPortMappingCount(&count);
    for (index = count; index > 0; index--)
    {
        Utopia_DeleteDynPortMappingIndex(index);
    }
}

#endif

