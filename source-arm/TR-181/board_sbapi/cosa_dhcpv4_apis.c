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

    module: cosa_dhcpv4_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.
        03/15/2011    Added for backend Dhcp4 clients.
        04/08/2011    Added Dhcp4 clients Sent Options and Req Options.
 

**************************************************************************/
#define _XOPEN_SOURCE 700
#include <string.h>
#include <strings.h>
#include "secure_wrapper.h"
#include "cosa_apis.h"
#include "cosa_dhcpv4_apis.h"
#include "cosa_dhcpv4_internal.h"
#include "plugin_main_apis.h"
#include "dml_tr181_custom_cfg.h"
#include "secure_wrapper.h"
#include "utapi/utapi.h"
#include "utapi/utapi_util.h"
#include "safec_lib_common.h"

#if ( defined _COSA_SIM_ )

#elif defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)

#undef _COSA_SIM_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>

#include <utctx/utctx_api.h>
#include <ulog/ulog.h>
#include <syscfg/syscfg.h>
#include <utapi/utapi_tr_dhcp.h>
#include "dhcpv4c_api.h"
#include <syslog.h>
//#include "cosa_moca_apis.h"
#include <ccsp_syslog.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

#ifdef CONFIG_SYSTEM_MOCA
#include "moca_hal.h"
#endif

#include "ccsp_psm_helper.h"
#include "sysevent/sysevent.h"
#include "dmsb_tr181_psm_definitions.h"

#include "lm_api.h"
#include <cjson/cJSON.h>

#undef COSA_DML_DHCP_LEASES_FILE
#undef COSA_DML_DHCP_OPTIONS_FILE

#define  COSA_DML_DHCP_LEASES_FILE                  "/nvram/dnsmasq.leases"
#define  COSA_DML_DHCP_OPTIONS_FILE                 "/etc/dnsmasq.options"
#define  LAN_L3_IFNAME      "brlan0"
#define  WIFI_CLIENTS_MAC_FILE                      "/var/tmp/wifi_clients_mac"
#define  LAN_NOT_RESTART_FLAG                        "/var/tmp/lan_not_restart"

#define COSA_DHCP4_SYSCFG_NAMESPACE NULL

// defind PSM paramaters
#define PSM_ENABLE_STRING_TRUE  "TRUE"
#define PSM_ENABLE_STRING_FALSE "FALSE"        

#define BOOTSTRAP_INFO_FILE             "/opt/secure/bootstrap.json"

#ifndef FEATURE_RDKB_WAN_MANAGER
COSA_DML_DHCPC_FULL     CH_g_dhcpv4_client[COSA_DML_DHCP_MAX_ENTRIES]; 
COSA_DML_DHCP_OPT       g_dhcpv4_client_sent[COSA_DML_DHCP_MAX_ENTRIES][COSA_DML_DHCP_MAX_OPT_ENTRIES];
COSA_DML_DHCPC_REQ_OPT  g_dhcpv4_client_req[COSA_DML_DHCP_MAX_ENTRIES][COSA_DML_DHCP_MAX_OPT_ENTRIES]; 

ULONG          g_Dhcp4ClientNum = 0;
ULONG          g_Dhcp4ClientSentOptNum[COSA_DML_DHCP_MAX_ENTRIES] = {0,0,0,0};
ULONG          g_Dhcp4ClientReqOptNum[COSA_DML_DHCP_MAX_ENTRIES]  = {0,0,0,0};
#endif

/*for server.pool.client*/
PCOSA_DML_DHCPSV4_CLIENT g_dhcpv4_server_client = NULL; 
ULONG                    g_dhcpv4_server_client_count = 0;
PCOSA_DML_DHCPSV4_CLIENTCONTENT g_dhcpv4_server_client_content = NULL;

COSA_DML_DHCPSV4_OPTION g_dhcpv4_server_pool_option[] =
{
    {
        1,
        "Option1",
        6,
        "Value2",
        TRUE
    }
};

// Define second DHCP Server Pool data structure here 
// this structure will be replaced when implementing full support
// for add server pool and delete server pool
// default value is set in init function
//COSA_DML_DHCPS_POOL_FULL g_dhcpv4_server_pool2;
SLIST_HEADER g_dhcpv4_server_pool_list;

// for PSM access
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
// PSM access MACRO
#define _PSM_WRITE_PARAM(_PARAM_NAME) { \
        errno_t rc = -1; \
        rc = sprintf_s(param_name, sizeof(param_name), _PARAM_NAME, instancenum); \
        if(rc < EOK) { \
            ERR_CHK(rc); \
        } \
        retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmSet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));\
        } \
        else \
        { \
            /*AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value)); */\
        } \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }

#define _PSM_READ_PARAM(_PARAM_NAME) { \
        errno_t rc = -1; \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        rc = sprintf_s(param_name, sizeof(param_name), _PARAM_NAME, instancenum); \
        if(rc < EOK) { \
            ERR_CHK(rc); \
        } \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d reading %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value));\
        } \
        else { \
            /*AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = \n%s\n", __FUNCTION__,param_name, param_value)); */\
        } \
    }

#define _PSM_WRITE_TBL_PARAM(_PARAM_NAME) { \
        errno_t rc = -1; \
        rc = sprintf_s(param_name, sizeof(param_name), _PARAM_NAME, tblInstancenum, instancenum); \
        if(rc < EOK) { \
            ERR_CHK(rc); \
        } \
        retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmSet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));\
            /*printf("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value);*/\
        } \
        else \
        { \
            /*AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value));*/ \
            /*printf("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value);*/ \
        } \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }

#define _PSM_READ_TBL_PARAM(_PARAM_NAME) { \
        errno_t rc = -1; \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        rc = sprintf_s(param_name, sizeof(param_name), _PARAM_NAME, tblInstancenum, instancenum); \
        if(rc < EOK) { \
            ERR_CHK(rc); \
        } \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d reading %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value));\
            /*printf("%s Error %d reading %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value);*/\
        } \
        else { \
            /*AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = \n%s\n", __FUNCTION__,param_name, param_value)); */\
            /*printf("%s: retPsmGet == CCSP_SUCCESS reading %s = \n%s\n", __FUNCTION__,param_name, param_value);*/ \
        } \
    }

extern void mac_string_to_array(char *pStr, unsigned char array[6]);

int sbapi_get_dhcpv4_active_number(int index, ULONG minAddress, ULONG maxAddress);

ANSC_STATUS CosaDhcpInitJournal(PCOSA_DML_DHCPS_POOL_CFG  pPoolCfg);

ANSC_STATUS COSAGetParamValueByPathName(void* bus_handle, parameterValStruct_t *val, ULONG *parameterValueLength);

int Utopia_get_lan_host_comments(UtopiaContext *ctx, unsigned char *pMac, unsigned char *pComments);

void _get_shell_output (FILE *fp, char *buf, size_t len);
int _get_shell_output2 (FILE *fp, char *needle);

int find_arp_entry(char *ipaddr, char *ifname, unsigned char *pMac)
{
	struct arpreq arpreq;
	struct sockaddr_in *sin;
	struct in_addr ina;
    unsigned char zeroMac[6]={0,0,0,0,0,0};
	int rv, s;
	errno_t rc = -1;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0)
		return(-1);
	memset(&arpreq, 0, sizeof(struct arpreq));
	sin = (struct sockaddr_in *) &arpreq.arp_pa;
	memset(sin, 0, sizeof(struct sockaddr_in));
	sin->sin_family = AF_INET;
	ina.s_addr = inet_addr(ipaddr);
	memcpy(&sin->sin_addr, (char *)&ina, sizeof(struct in_addr));
	rc = strcpy_s(arpreq.arp_dev, sizeof(arpreq.arp_dev), ifname);
	ERR_CHK(rc);
	rv = ioctl(s, SIOCGARP, &arpreq);
	close(s);
	if((rv < 0)||memcmp(zeroMac,arpreq.arp_ha.sa_data,6)==0)
		return(-1);

	memcpy(pMac,arpreq.arp_ha.sa_data,6);
	return(0);
}
#ifdef CONFIG_SYSTEM_MOCA
int usg_cpe_from_moca(char *pMac)
{

    AnscTraceFlow(("Inside %s\n", __FUNCTION__));
    int n=0,i;
    unsigned char macArray[6];
    moca_cpe_t cpes[kMoca_MaxCpeList];

    mac_string_to_array(pMac,macArray);
    
    AnscTraceFlow(("Calling moca_GetMocaCPEs, %s\n", __FUNCTION__));
    moca_GetMocaCPEs(0, cpes, &n);
    AnscTraceFlow(("Returned from moca_GetMocaCPEs, %s\n", __FUNCTION__));
    printf("%s", "");
    for(i=0;i<n;i++){
        printf("MAC[%d]-> %02x:%02x:%02x:%02x:%02x:%02x\n", i, cpes[i].mac_addr[0],cpes[i].mac_addr[1],cpes[i].mac_addr[2],
                cpes[i].mac_addr[3],cpes[i].mac_addr[4],cpes[i].mac_addr[5]);
        if(!memcmp(macArray,cpes[i].mac_addr,6))
            return(1);
    }
    return(0);
}
#endif
/*Only search SSID 1 & 2, and the SSID number is the same as the AccessPoint number. pMac's format is xx:xx:xx:xx:xx:xx*/
#if 0 
int usg_get_cpe_associated_ssid(char *pMac, char ssid[64], int *pAp)
{
	int i, j, amount,size;
	char dm[128], outdata[32];
    unsigned char mac1[6],mac2[6];
    parameterValStruct_t varStruct;

    if(bus_handle==NULL){
        printf("Warning: bus_hanle is NULL !!!\n ");
        return(-1);
    }

    varStruct.parameterName = dm;
    varStruct.parameterValue = outdata;
    mac_string_to_array(pMac,mac1);
	for(i=1;i<=2;i++){
		/*get the amount of this Access Point*/
		snprintf(dm,sizeof(dm), "Device.WiFi.AccessPoint.%d.AssociatedDeviceNumberOfEntries", i);
        size = sizeof(outdata);
        if(COSAGetParamValueByPathName(bus_handle,&varStruct,&size)){
            printf("Failed to get AP's STA number\n");
            continue;
        }
		amount = atoi(outdata);
		/*check the wireless stations in this access point*/
		for(j=1;j<=amount;j++){
			snprintf(dm,sizeof(dm), "Device.WiFi.AccessPoint.%d.AssociatedDevice.%d.MACAddress", i, j);
            size = sizeof(outdata);
			if(!COSAGetParamValueByPathName(bus_handle, &varStruct,&size)){
                mac_string_to_array(outdata,mac2);
				if(!memcmp(mac1,mac2,6)){/*Found this client*/
					snprintf(dm,sizeof(dm), "Device.WiFi.SSID.%d.SSID", i);
                    size = sizeof(outdata);
                    if(COSAGetParamValueByPathName(bus_handle, &varStruct,&size))
						return(-1);
                    snprintf(ssid, 64, "%s", outdata);
                    *pAp = i;
					return(0);
				}
			}
		}
	}
	return(-1);
}
#else
typedef struct wifi_client_s{
    SINGLE_LINK_ENTRY Linkage;
    char mac[18];
    char ssid[64];
    ULONG ap;
}wifi_client_t;

#define WIFI_CLIENT_MAXIMUM 254
#define ACCESS_WIFI_CLIENT(p) \
        ACCESS_CONTAINER(p, wifi_client_t, Linkage)

/*
static SLIST_HEADER g_wifiClientTable[WIFI_CLIENT_MAXIMUM];
static pthread_mutex_t g_wifiClientTable_mutex = PTHREAD_MUTEX_INITIALIZER;


static void init_wifi_client(void)
{
    ULONG i;

    pthread_mutex_lock(&g_wifiClientTable_mutex);
    for (i = 0; i < WIFI_CLIENT_MAXIMUM; i++){
        AnscSListInitializeHeader(&g_wifiClientTable[i]);
    }
    pthread_mutex_unlock(&g_wifiClientTable_mutex);

}

static void string_tolower(char *pMac)
{
    ULONG i;

    for (i = 0; pMac[i] != '\0' ; i++){
        if (pMac[i] >= 'A' && pMac[i] <= 'Z')
            pMac[i] += 'a' - 'A';
    }
}*/

/*
static void insert_wifi_client(const char *pMac, const char *pSsid, const ULONG ap)
{
    ULONG hashIndex; 
    string_tolower((char*)pMac);    

    hashIndex = AnscHashString(pMac, strlen(pMac), WIFI_CLIENT_MAXIMUM);

    wifi_client_t *pNewClient = AnscAllocateMemory(sizeof(wifi_client_t));
    if (!pNewClient){
        printf("!!!Error failed to allocate memory for new wifi client\n");
        return;
    }

    AnscCopyString(pNewClient->mac, (char*)pMac);
    AnscCopyString(pNewClient->ssid, (char*)pSsid);
    pNewClient->ap = ap;

    pthread_mutex_lock(&g_wifiClientTable_mutex);
    AnscSListPushEntry(&g_wifiClientTable[hashIndex], &pNewClient->Linkage);
    pthread_mutex_unlock(&g_wifiClientTable_mutex);

}*/

/*
static wifi_client_t* lookup_wifi_client(const char *pMac)
{
    //CID:55477 Wrong sizeof argument
    ULONG hashIndex = AnscHashString(pMac, strlen(pMac), WIFI_CLIENT_MAXIMUM);
    PSINGLE_LINK_ENTRY pSLinkEntry = NULL;
    wifi_client_t *pClient = NULL;

    pthread_mutex_lock(&g_wifiClientTable_mutex);
    pSLinkEntry = AnscSListGetFirstEntry(&g_wifiClientTable[hashIndex]);
    while (pSLinkEntry){
        pClient = ACCESS_WIFI_CLIENT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if (pClient && AnscEqualString(pClient->mac, pMac, FALSE)){
            pthread_mutex_unlock(&g_wifiClientTable_mutex);
            return pClient;
        }
    }

    pthread_mutex_unlock(&g_wifiClientTable_mutex);

    return NULL;
}

static void flush_wifi_client(void)
{
    wifi_client_t *pClient = NULL;
    ULONG i;
    PSINGLE_LINK_ENTRY pSLinkEntry = NULL;

    pthread_mutex_lock(&g_wifiClientTable_mutex);
    for (i = 0; i < WIFI_CLIENT_MAXIMUM; i++){
        pSLinkEntry = AnscSListGetFirstEntry(&g_wifiClientTable[i]);
        while (pSLinkEntry){
            pClient = ACCESS_WIFI_CLIENT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
            if (pClient){
                AnscFreeMemory(pClient);
                pClient = NULL;
            }
        }
        AnscSListInitializeHeader(&g_wifiClientTable[i]);
    }
    pthread_mutex_unlock(&g_wifiClientTable_mutex);

}
*/
/*
static void print_wifi_client(void)
{
    wifi_client_t *pClient = NULL;
    ULONG i;
    PSINGLE_LINK_ENTRY pSLinkEntry = NULL;

    pthread_mutex_lock(&g_wifiClientTable_mutex);
    for (i = 0; i < WIFI_CLIENT_MAXIMUM; i++){
        pSLinkEntry = AnscSListGetFirstEntry(&g_wifiClientTable[i]);
        while(pSLinkEntry){
            pClient = ACCESS_WIFI_CLIENT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
            if (pClient){
                printf("AP %d SSID %s CLIENT %s\n", pClient->ap, pClient->ssid, pClient->mac);
            }
        }
    }
    pthread_mutex_unlock(&g_wifiClientTable_mutex);

}
*/

#if 0
int usg_get_cpe_associated_ssid(void *arg)
{
    UNREFERENCED_PARAMETER(arg);
    int i, j, amount,size;
    char dm[128], outdata[33];    // outdata also holds SSID
    char ssid[64]={0},mac[18];
    parameterValStruct_t varStruct;
    struct timespec tm;
    int fd = -1;
    const char str_ld[] = "\r\n";
    
    pthread_detach(pthread_self());
   
    printf("------%s START working------\n\n", __func__);
 
    tm.tv_sec = 10;
    tm.tv_nsec = 0;

    init_wifi_client();


    while(1){
reopen:
        /* CID: 135361 Insecure file permissions - missing mode arg with O_CREAT*/
        fd = open(WIFI_CLIENTS_MAC_FILE, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IWUSR);
        if (-1 == fd){
            sleep(1);
            goto reopen;
        }
    
        if(bus_handle){
            varStruct.parameterName = dm;
            varStruct.parameterValue = outdata;
	
            for(i=1;i<=2;i++){
		        /*get the amount of this Access Point*/
		        snprintf(dm,sizeof(dm), "Device.WiFi.AccessPoint.%d.AssociatedDeviceNumberOfEntries", i);
                size = sizeof(outdata);
                if(COSAGetParamValueByPathName(bus_handle,&varStruct,&size)){
                    printf("Failed to get AP's STA number\n");
                    continue;
                }
		
                amount = atoi(outdata);
		
                /*check the wireless stations in this access point*/
		        for(j=1;j<=amount;j++){
			        snprintf(dm,sizeof(dm), "Device.WiFi.AccessPoint.%d.AssociatedDevice.%d.MACAddress", i, j);
                    size = sizeof(outdata);
			        if(!COSAGetParamValueByPathName(bus_handle, &varStruct,&size)){
				        snprintf(mac, 18, "%s", outdata);
                        flock(fd, LOCK_EX);
                        write(fd, mac, 18);
                        write(fd, str_ld, 2);
                        flock(fd, LOCK_UN);
                       	
                        snprintf(dm,sizeof(dm), "Device.WiFi.SSID.%d.SSID", i);
                        size = sizeof(outdata);
                        if(!COSAGetParamValueByPathName(bus_handle, &varStruct,&size)){
                            snprintf(ssid, 64, "%s", outdata);
				        }
                     
                        insert_wifi_client(mac, ssid, i);
			        }
		        }
	        }
        }
        nanosleep(&tm, NULL);
        //print_wifi_client();
        flush_wifi_client();

        close(fd);
        /* CID: 69027 Unused value - removing the statement fd = -1*/
    }
}
#endif
#endif

int usg_get_cpe_associate_interface(char *pMac, char ifname[64])
{

    unsigned char macArray[6];
    LM_cmd_common_result_t result;
    errno_t rc = -1;

    memset(&result, 0, sizeof(result));
    mac_string_to_array(pMac, macArray);

    if(lm_get_host_by_mac((char*)macArray, &result) == -1){ // failed to get wifi interface from Lm_Hosts
#ifdef CONFIG_SYSTEM_MOCA
        if(usg_cpe_from_moca(pMac)){
            rc = strcpy_s(ifname, 64, "Device.MoCA.Interface.1");
            ERR_CHK(rc);
        }
        else
#endif
        {
            rc = strcpy_s(ifname, 64, "Device.Ethernet.Interface.x");
            ERR_CHK(rc);
        }
    }else{
        if(result.result == 0)  {
            rc = strcpy_s(ifname, 64, result.data.host.l1IfName);
            ERR_CHK(rc);
        } else {
            rc = strcpy_s(ifname, 64, "Device.Ethernet.Interface.x"); // failed to get result, default value
            ERR_CHK(rc);
        }
    }
    return(0);
}

#if 0
static ANSC_STATUS CosaDmlDhcpcScan()
{
    CHAR            tmpBuff[64]    = {0};
    CHAR            tmp[3][64];
    CHAR            ip[32];   
    CHAR            subnet[32];
    CHAR            str_val[64] = {0};
    CHAR            str_key[64] = {0};
    CHAR            dns[3][32]  = {"","",""};
    CHAR            line[128];
    char            *tok;
    int             nmu_dns_server =  0;
    FILE            *fp = NULL;
    char            *st = NULL;
    PCOSA_DML_DHCPC_FULL  pEntry = NULL;
    
    ULOGF(ULOG_SYSTEM, UL_DHCP, "%s:\n", __FUNCTION__);
    ULONG ulIndex = 0;
    
    g_Dhcp4ClientNum = 0;
       
    for (ulIndex = 0; ulIndex < 2; ulIndex++)
    {
        if (ulIndex == 0)
        {
            fp = fopen("/tmp/udhcp.log", "r");
        }       
        else if (ulIndex == 1)
        {

            /*RDKB-6750, CID-33082, free resources before exit*/
            if(fp)
            {
                fclose(fp);
                fp = NULL;
            }

                return ANSC_STATUS_SUCCESS;

        }
        pEntry = &CH_g_dhcpv4_client[ulIndex];
        
        if (!fp) 
        {
            ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: failed to open udhcpc log file\n", __FUNCTION__); 
            return ANSC_STATUS_FAILURE;
        }   
        
        while ( fgets(line, sizeof(line), fp) )
        {         
            memset(str_key, 0, sizeof(str_key));
            memset(str_val, 0, sizeof(str_val));

            st = NULL;
            tok = strtok_r( line, ":", &st);
            if(tok) strncpy(str_key, tok, sizeof(str_key)-1 );

            tok = strtok_r(NULL, ":", &st);
            while( tok && (tok[0] == ' ') ) tok++; /*RDKB-6750, CID-33384, CID-32954; null check before use*/
            if(tok) strncpy(str_val, tok, sizeof(str_val)-1 );

            if ( str_val[ _ansc_strlen(str_val) - 1 ] == '\n' )
            {
                str_val[ _ansc_strlen(str_val) - 1 ] = '\0';
            }
           
            if( !strcmp(str_key, "interface     ") )
            {
                AnscCopyString( pEntry->Cfg.Interface, str_val);
            }
            else if( !strcmp(str_key, "ip address    ") )
            {
                ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: DHCP Client IP %s\n", __FUNCTION__, str_val);
                sscanf(str_val, "%s", ip);
                AnscWriteUlong(&pEntry->Info.IPAddress.Value, _ansc_inet_addr(ip));
            }
            else if( !strcmp(str_key, "subnet mask   ") )
            {
                sscanf(str_val, "%s", subnet );
                AnscWriteUlong(&pEntry->Info.SubnetMask.Value, _ansc_inet_addr(subnet));
                ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: subnet %s\n", __FUNCTION__, subnet);
            }
            else if( !strcmp(str_key, "lease time    ") )
            {
                pEntry->Info.LeaseTimeRemaining = atoi(str_val);
            }
            else if( !strcmp(str_key, "router        ") )
            {
                ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: DHCP router IP %s\n", __FUNCTION__, str_val);
                sscanf(str_val, "%s", ip ); 
                AnscWriteUlong(&pEntry->Info.IPRouters[0].Value, _ansc_inet_addr(ip) ); 
            }
            else if( !strcmp(str_key, "server id     ") )
            {
                ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: DHCP Sever IP %s\n", __FUNCTION__, str_val);
                sscanf(str_val, "%s", ip ); 
                AnscWriteUlong(&pEntry->Info.DHCPServer.Value, _ansc_inet_addr(ip));
            }
            else if( !strcmp(str_key, "dns server    ") )
            {
                nmu_dns_server = 0;               
                char *tok;
                char *sub_st = NULL;
                tok = strtok_r( str_val, " ", &sub_st);
                if(tok) strncpy(dns[0], tok, sizeof(dns[0])-1 ); 
                ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: DNS 0 %s\n", __FUNCTION__, dns[0]);
                /*CID: 68331 Array compared against 0*/
                ++nmu_dns_server;
                AnscWriteUlong(&pEntry->Info.DNSServers[0].Value, _ansc_inet_addr(dns[0]));
               
                while( tok != NULL)
                {
                    tok = strtok_r(NULL, " ", &sub_st);
                    ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: DNS %d %s\n", __FUNCTION__, nmu_dns_server, tok);
                    if(tok) strncpy(dns[nmu_dns_server], tok, sizeof(dns[nmu_dns_server])-1 );
                    if (strlen(dns[nmu_dns_server]) > 1 )
                    {    
                        AnscWriteUlong(&pEntry->Info.DNSServers[nmu_dns_server].Value, _ansc_inet_addr(dns[nmu_dns_server]));
                        ++nmu_dns_server;
                        if( nmu_dns_server > 2) /*RDKB-6750, CID-33057, Fixing Out-of-bounds read of dns[3][] */
                            nmu_dns_server = 2;
                    }
                }
            }
        }
                
        if ( !_ansc_strncmp(pEntry->Cfg.Interface, COSA_DML_DHCPV4_CLIENT_IFNAME, _ansc_strlen(COSA_DML_DHCPV4_CLIENT_IFNAME)) )
        {
            memset(tmpBuff, 0, sizeof(tmpBuff));
            syscfg_get(NULL, "tr_dhcp4_instance_wan", tmpBuff, sizeof(tmpBuff));
            CH_g_dhcpv4_client[ulIndex].Cfg.InstanceNumber  = atoi(tmpBuff);
            
            memset(tmpBuff, 0, sizeof(tmpBuff));
            syscfg_get(NULL, "tr_dhcp4_alias_wan", tmpBuff, sizeof(tmpBuff));
            AnscCopyString(CH_g_dhcpv4_client[ulIndex].Cfg.Alias, tmpBuff);
            ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: interface %s, instanceNmuber %d, Alias %s\n", 
                    __FUNCTION__, CH_g_dhcpv4_client[ulIndex].Cfg.Interface, CH_g_dhcpv4_client[ulIndex].Cfg.InstanceNumber, CH_g_dhcpv4_client[ulIndex].Cfg.Alias);
        }
        else if ( !_ansc_strncmp(pEntry->Cfg.Interface, "lan0", _ansc_strlen("lan0")) )
        {
            memset(tmpBuff, 0, sizeof(tmpBuff));
            syscfg_get(NULL, "tr_dhcp4_instance_lan", tmpBuff, sizeof(tmpBuff));
            CH_g_dhcpv4_client[ulIndex].Cfg.InstanceNumber  = atoi(tmpBuff);
            
            memset(tmpBuff, 0, sizeof(tmpBuff));
            syscfg_get(NULL, "tr_dhcp4_alias_lan", tmpBuff, sizeof(tmpBuff));
            AnscCopyString(CH_g_dhcpv4_client[ulIndex].Cfg.Alias, tmpBuff);
            ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: interface %s, instanceNmuber %d, Alias %s\n", __FUNCTION__, 
                    CH_g_dhcpv4_client[ulIndex].Cfg.Interface, CH_g_dhcpv4_client[ulIndex].Cfg.InstanceNumber, CH_g_dhcpv4_client[ulIndex].Cfg.Alias);
        }
            
        pEntry->Cfg.PassthroughEnable   = FALSE;
                    
        //AnscCopyString( pEntry->Cfg.PassthroughDHCPPool, "");  
        pEntry->Cfg.bEnabled                     = TRUE;
        //pEntry->Cfg.bRenew                       = FALSE;

        pEntry->Info.Status                      = COSA_DML_DHCP_STATUS_Enabled;
        pEntry->Info.DHCPStatus                  = COSA_DML_DHCPC_STATUS_Bound;
    
        pEntry->Info.NumIPRouters                = 1;
        pEntry->Info.NumDnsServers               = nmu_dns_server;
        g_Dhcp4ClientNum++;
        ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: ulIndex %d, Interface %s, g_Dhcp4ClientNum %d\n", __FUNCTION__, ulIndex, CH_g_dhcpv4_client[ulIndex].Cfg.Interface, g_Dhcp4ClientNum);
     
     /*CID: 59145 Logically dead code*/   
     fclose(fp);
    }
    return ANSC_STATUS_SUCCESS;
}
#endif
/*if the address is not in the following range, we think it is a public address
 10.0.0.0    ~ 10.255.255.255
 172.16.0.0  ~ 172.31.255.255
 192.168.0.0 ~ 192.168.255.255.
 */
static int is_a_public_addr(unsigned int addr)
{
    if((addr >= 0x0A000000)&&(addr <= 0x0AFFFFFF)){
        return(0);
    }else if((addr >= 0xAC100000)&&(addr <= 0xAC1FFFFF)){
        return(0);
    }else if((addr >= 0xC0A80000)&&(addr <= 0xC0A8FFFF)){
        return(0);
    }else
        return(1);
}

// DHCPv4 Server Pool PSM handling code
static void deleteDHCPv4ServerPoolOptionPSM(ULONG poolInstanceNumber, ULONG instanceNumber)
{
    int retPsmSet = CCSP_SUCCESS;
    char param_path[256] = {0};    
    errno_t rc = -1;
    
    // borrowing PSM_DHCPV4_SERVER_POOL_i here for Option_i
    rc = sprintf_s(param_path, sizeof(param_path), PSM_DHCPV4_SERVER_POOL_OPTION PSM_DHCPV4_SERVER_POOL_i, poolInstanceNumber, instanceNumber);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    AnscTraceFlow(("%s: deleting %s\n", __FUNCTION__, param_path));
    //printf("%s: deleting %s\n", __FUNCTION__,  param_path);
    retPsmSet = PSM_Del_Record(bus_handle, g_Subsystem, param_path);

    if ( retPsmSet != CCSP_SUCCESS )
    {
        AnscTraceFlow(("%s -- failed to delete PSM records, error code %d", __FUNCTION__, retPsmSet));
        //printf("%s -- failed to delete PSM records, error code %d", __FUNCTION__, retPsmSet);
    }
    //else{
        //printf("%s -- delete PSM records, return successful %d", __FUNCTION__, retPsmSet);
    //}
    
}

static void deleteDHCPv4ServerPoolPSM(ULONG instanceNumber)
{
    int retPsmSet = CCSP_SUCCESS;
    char param_path[256] = {0};
    errno_t rc = -1;
    
    rc = sprintf_s(param_path, sizeof(param_path), PSM_DHCPV4_SERVER_POOL PSM_DHCPV4_SERVER_POOL_i, instanceNumber);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    AnscTraceFlow(("%s: deleting %s\n", __FUNCTION__, param_path));
    retPsmSet = PSM_Del_Record(bus_handle, g_Subsystem, param_path);

    if ( retPsmSet != CCSP_SUCCESS )
    {
        AnscTraceFlow(("%s -- failed to delete PSM records, error code %d", __FUNCTION__, retPsmSet));
    }
    
}
 
static BOOLEAN writeDHCPv4ServerPoolOptionToPSM(ULONG tblInstancenum, PCOSA_DML_DHCPSV4_OPTION pNewOption, PCOSA_DML_DHCPSV4_OPTION pOldOption)
{
    // setting cfg parameters for the second pool, write to PSM
    int retPsmSet = CCSP_SUCCESS;
    char param_name[256] = {0};
    char param_value[256] = {0};
    ULONG instancenum = pNewOption->InstanceNumber;
    BOOLEAN dhcpServerRestart = FALSE;
    errno_t rc = -1;

    memset(param_value, 0, sizeof(param_value));
    memset(param_name, 0, sizeof(param_name));

    //printf("%s\n", __FUNCTION__);
    if (pNewOption->bEnabled != pOldOption->bEnabled)
    {
        //printf("%s: write bEnabled\n", __FUNCTION__);
        rc = strcpy_s(param_value, sizeof(param_value), ((pNewOption->bEnabled) ? "TRUE" : "FALSE"));
        ERR_CHK(rc);
        _PSM_WRITE_TBL_PARAM(PSM_DHCPV4_SERVER_POOL_OPTION_ENABLE);
        dhcpServerRestart = TRUE;
        pOldOption->bEnabled = pNewOption->bEnabled;
    }
    
    if (strcmp(pNewOption->Alias, pOldOption->Alias) != 0)
    {
        //printf("%s: write Alias\n", __FUNCTION__);
        rc = strcpy_s(param_value, sizeof(param_value), pNewOption->Alias);
        ERR_CHK(rc);
        _PSM_WRITE_TBL_PARAM(PSM_DHCPV4_SERVER_POOL_OPTION_ALIAS);
        rc = STRCPY_S_NOCLOBBER(pOldOption->Alias, sizeof(pOldOption->Alias), pNewOption->Alias);
        ERR_CHK(rc);
    }

    if (pNewOption->Tag != pOldOption->Tag)
    {
        //printf("%s: write Tag %d\n", __FUNCTION__, pNewOption->Tag);
        rc = sprintf_s(param_value, sizeof(param_value), "%lu", pNewOption->Tag );
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        _PSM_WRITE_TBL_PARAM(PSM_DHCPV4_SERVER_POOL_OPTION_TAG);
        pOldOption->Tag = pNewOption->Tag;
        dhcpServerRestart = TRUE;
    }

    // hexdecimal value
    if (strcmp((char*)pNewOption->Value, (char*)pOldOption->Value) != 0)
    {
        //printf("%s: write Value %s\n", __FUNCTION__, pNewOption->Value);
        rc = sprintf_s(param_name, sizeof(param_name), PSM_DHCPV4_SERVER_POOL_OPTION_VALUE, tblInstancenum, instancenum); 
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        rc = sprintf_s(param_value, sizeof(param_value), "%s", pNewOption->Value);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        retPsmSet = PSM_Set_Record_Value2(bus_handle, g_Subsystem, param_name, ccsp_byte, param_value); 
        if (retPsmSet != CCSP_SUCCESS) { 
            AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));
            //printf("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value);
        } 
        else 
        { 
            AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value)); 
            //printf("%s: retPsmGet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name, param_value);
        } 
        dhcpServerRestart = TRUE;
    }

    return dhcpServerRestart;
}

static BOOLEAN writeDHCPv4ServerPoolCFGToPSM(PCOSA_DML_DHCPS_POOL_CFG pNewCfg, PCOSA_DML_DHCPS_POOL_CFG pOldCfg)
{
    // setting cfg parameters for the second pool, write to PSM
    int retPsmSet = CCSP_SUCCESS;
    char param_name[256] = {0};
    char param_value[256] = {0};
    ULONG instancenum = pNewCfg->InstanceNumber;
    BOOLEAN dhcpServerRestart = FALSE;
    errno_t rc = -1;

    if (pNewCfg->bEnabled != pOldCfg->bEnabled)
    {
        rc = strcpy_s(param_value, sizeof(param_value), ((pNewCfg->bEnabled) ? "TRUE" : "FALSE"));
        ERR_CHK(rc);
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_ENABLE);
        dhcpServerRestart = TRUE;
        pOldCfg->bEnabled = pNewCfg->bEnabled;
    }
    
    if (strcmp(pNewCfg->Alias, pOldCfg->Alias) != 0)
    {
        rc = strcpy_s(param_value, sizeof(param_value), pNewCfg->Alias);
        ERR_CHK(rc);
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_ALIAS);
        rc = STRCPY_S_NOCLOBBER(pOldCfg->Alias, sizeof(pOldCfg->Alias), pNewCfg->Alias);
        ERR_CHK(rc);
    }

    if (strcmp(pNewCfg->Interface, pOldCfg->Interface) != 0)
    {
        char* instStr;
        int strLength = strlen("Device.IP.Interface.");
        instStr = &(pNewCfg->Interface[strLength]);
        AnscTraceFlow(("%s: instStr %s\n", __FUNCTION__, instStr));
        rc = strcpy_s(param_value, sizeof(param_value), instStr);
        ERR_CHK(rc);
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_INTERFACE);
        rc = STRCPY_S_NOCLOBBER(pOldCfg->Interface, sizeof(pOldCfg->Interface), pNewCfg->Interface);
        ERR_CHK(rc);
        dhcpServerRestart = TRUE;
    }

    if (pNewCfg->MinAddress.Value != pOldCfg->MinAddress.Value)
    {
        rc = sprintf_s(param_value, sizeof(param_value), "%d.%d.%d.%d",
            pNewCfg->MinAddress.Dot[0], pNewCfg->MinAddress.Dot[1], pNewCfg->MinAddress.Dot[2], pNewCfg->MinAddress.Dot[3]);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_MINADDRESS);
        pOldCfg->MinAddress.Value = pNewCfg->MinAddress.Value;
        dhcpServerRestart = TRUE;
    }

    if (pNewCfg->MaxAddress.Value != pOldCfg->MaxAddress.Value)
    {
        rc = sprintf_s(param_value, sizeof(param_value), "%d.%d.%d.%d",
            pNewCfg->MaxAddress.Dot[0], pNewCfg->MaxAddress.Dot[1],pNewCfg->MaxAddress.Dot[2],pNewCfg->MaxAddress.Dot[3]);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_MAXADDRESS);
        pOldCfg->MaxAddress.Value = pNewCfg->MaxAddress.Value;
        dhcpServerRestart = TRUE;
    }

    if (pNewCfg->SubnetMask.Value != pOldCfg->SubnetMask.Value)
    {
        rc = sprintf_s(param_value, sizeof(param_value), "%d.%d.%d.%d",
            pNewCfg->SubnetMask.Dot[0], pNewCfg->SubnetMask.Dot[1],pNewCfg->SubnetMask.Dot[2],pNewCfg->SubnetMask.Dot[3]);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_SUBNETMASK);
        pOldCfg->SubnetMask.Value = pNewCfg->SubnetMask.Value;
        dhcpServerRestart = TRUE;
    }

    /*
    printf("%s:%x, %x, %x, %x\n", __FUNCTION__,
        pNewCfg->DNSServers[0].Value, pNewCfg->DNSServers[1].Value,
        pNewCfg->DNSServers[2].Value, pNewCfg->DNSServers[3].Value);
    printf("%s:%x, %x, %x, %x\n", __FUNCTION__,
        pOldCfg->DNSServers[0].Value,  pOldCfg->DNSServers[1].Value,
        pOldCfg->DNSServers[2].Value, pOldCfg->DNSServers[3].Value);
    */
    if (pNewCfg->DNSServers[0].Value != pOldCfg->DNSServers[0].Value ||
        pNewCfg->DNSServers[1].Value != pOldCfg->DNSServers[1].Value ||
        pNewCfg->DNSServers[2].Value != pOldCfg->DNSServers[2].Value ||
        pNewCfg->DNSServers[3].Value != pOldCfg->DNSServers[3].Value )
    {
        ULONG bufSize = sizeof(param_value);
            
        if(CosaDmlGetIpaddrString(param_value, &bufSize, &(pNewCfg->DNSServers[0].Value), COSA_DML_DHCP_MAX_ENTRIES ))
        {
            _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_DNSSERVERS);
        }
        pOldCfg->DNSServers[0].Value = pNewCfg->DNSServers[0].Value;
        pOldCfg->DNSServers[1].Value = pNewCfg->DNSServers[1].Value;
        pOldCfg->DNSServers[2].Value = pNewCfg->DNSServers[2].Value;
        pOldCfg->DNSServers[3].Value = pNewCfg->DNSServers[3].Value;
        dhcpServerRestart = TRUE;
    }

    /*
    printf("%s:%x, %x, %x, %x\n", __FUNCTION__,
        pNewCfg->IPRouters[0].Value, pNewCfg->IPRouters[1].Value,
        pNewCfg->IPRouters[2].Value, pNewCfg->IPRouters[3].Value );
    printf("%s:%x, %x, %x, %x\n", __FUNCTION__,
        pOldCfg->IPRouters[0].Value, pOldCfg->IPRouters[1].Value,
        pOldCfg->IPRouters[2].Value, pOldCfg->IPRouters[3].Value);
        */
    if (pNewCfg->IPRouters[0].Value != pOldCfg->IPRouters[0].Value ||
        pNewCfg->IPRouters[1].Value != pOldCfg->IPRouters[1].Value ||
        pNewCfg->IPRouters[2].Value != pOldCfg->IPRouters[2].Value ||
        pNewCfg->IPRouters[3].Value != pOldCfg->IPRouters[3].Value )
    {
        ULONG bufSize = sizeof(param_value);
            
        if ( CosaDmlGetIpaddrString(param_value, &bufSize, &(pNewCfg->IPRouters[0].Value), COSA_DML_DHCP_MAX_ENTRIES ) )
        {
            _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_IPROUTERS);
        }
        pOldCfg->IPRouters[0].Value = pNewCfg->IPRouters[0].Value;
        pOldCfg->IPRouters[1].Value = pNewCfg->IPRouters[1].Value;
        pOldCfg->IPRouters[2].Value = pNewCfg->IPRouters[2].Value;
        pOldCfg->IPRouters[3].Value = pNewCfg->IPRouters[3].Value;
        dhcpServerRestart = TRUE;
    }
        
    if (strcmp(pNewCfg->DomainName, pOldCfg->DomainName) != 0)
    {
        rc = strcpy_s(param_value, sizeof(param_value), pNewCfg->DomainName);
        ERR_CHK(rc);
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_DOMAINNAME);
        rc = STRCPY_S_NOCLOBBER(pOldCfg->DomainName, sizeof(pOldCfg->DomainName), pNewCfg->DomainName);
        ERR_CHK(rc);
        dhcpServerRestart = TRUE;
    }

    if (pNewCfg->LeaseTime != pOldCfg->LeaseTime)
    {
        rc = sprintf_s(param_value, sizeof(param_value), "%d", pNewCfg->LeaseTime );
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_LEASETIME);
        pOldCfg->LeaseTime = pNewCfg->LeaseTime;
        dhcpServerRestart = TRUE;
    }

    if (pNewCfg->Order != pOldCfg->Order)
    {
        rc = sprintf_s(param_value, sizeof(param_value), "%lu", pNewCfg->Order );
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_ORDER);
        pOldCfg->Order = pNewCfg->Order;
        dhcpServerRestart = TRUE;
    }

    if (pNewCfg->X_CISCO_COM_TimeOffset != pOldCfg->X_CISCO_COM_TimeOffset)
    {
        rc = sprintf_s(param_value, sizeof(param_value), "%d", pNewCfg->X_CISCO_COM_TimeOffset );
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_LEASETIME);
        pOldCfg->X_CISCO_COM_TimeOffset = pNewCfg->X_CISCO_COM_TimeOffset;
        dhcpServerRestart = TRUE;
    }

    return dhcpServerRestart;
}

static void getDHCPv4ServerPoolParametersFromPSM(ULONG instancenum, PCOSA_DML_DHCPS_POOL_FULL pPool)
{
    int retPsmGet = CCSP_SUCCESS;
    char *param_value= NULL;
    char param_name[256]= {0};
    PCOSA_DML_DHCPS_POOL_CFG pPoolCfg = &(pPool->Cfg); 
    errno_t rc = -1;

    pPoolCfg->InstanceNumber = instancenum; 

    // getting pool parameters from PSM
    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_ALLOWDELETE);
    if (retPsmGet == CCSP_SUCCESS)
    {
        if(strcmp(param_value, PSM_ENABLE_STRING_TRUE) == 0)
        {   
            pPoolCfg->bAllowDelete = TRUE;
        }
        else
        {
            pPoolCfg->bAllowDelete = FALSE;
        }
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }
    else
    {
        // if there is no record in PSM,initial value for allowdelete will be true.
        pPoolCfg->bAllowDelete = TRUE;
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_ENABLE);
    if (retPsmGet == CCSP_SUCCESS)
    {
        if(strcmp(param_value, PSM_ENABLE_STRING_TRUE) == 0)
        {   
            pPoolCfg->bEnabled = TRUE;
        }
        else
        {
            pPoolCfg->bEnabled = FALSE;
        }
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }
    else
    {
        // if there is no record in PSM,initial value for enabled will be false.
        pPoolCfg->bEnabled = FALSE;
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_ALIAS);
    if (retPsmGet == CCSP_SUCCESS)
    {
        rc = STRCPY_S_NOCLOBBER(pPoolCfg->Alias, sizeof(pPoolCfg->Alias), param_value);
        ERR_CHK(rc);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_INTERFACE);
    if (retPsmGet == CCSP_SUCCESS)
    {
        //AnscCopyString(pPoolCfg->Interface, param_value); 
        rc = sprintf_s(pPoolCfg->Interface, sizeof(pPoolCfg->Interface), "Device.IP.Interface.%s", param_value);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_MINADDRESS);
    if (retPsmGet == CCSP_SUCCESS)
    {
        pPoolCfg->MinAddress.Value = inet_addr(param_value); 
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_MAXADDRESS);
    if (retPsmGet == CCSP_SUCCESS)
    {
        pPoolCfg->MaxAddress.Value = inet_addr(param_value); 
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);        
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_SUBNETMASK);
    if (retPsmGet == CCSP_SUCCESS)
    {
        pPoolCfg->SubnetMask.Value = inet_addr(param_value); 
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);   
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_DOMAINNAME);
    if (retPsmGet == CCSP_SUCCESS)
    {
        rc = STRCPY_S_NOCLOBBER(pPoolCfg->DomainName, sizeof(pPoolCfg->DomainName), param_value);
        ERR_CHK(rc);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_DNSSERVERS);
    if (retPsmGet == CCSP_SUCCESS)
    {
        /*CID: 128145 Out-of-bounds access*/
        CosaDmlSetIpaddr((PULONG)&pPoolCfg->DNSServers[0], param_value, COSA_DML_DHCP_MAX_ENTRIES);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);   
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_IPROUTERS);
    if (retPsmGet == CCSP_SUCCESS)
    {
        /*CID: 128144 Out-of-bounds access*/
        CosaDmlSetIpaddr((PULONG)&pPoolCfg->IPRouters[0], param_value, COSA_DML_DHCP_MAX_ENTRIES);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);   
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_LEASETIME);
    if (retPsmGet == CCSP_SUCCESS)
    {
        _ansc_sscanf(param_value, "%d", &(pPoolCfg->LeaseTime));
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);   
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_ORDER);
    if (retPsmGet == CCSP_SUCCESS)
    {
        _ansc_sscanf(param_value, "%lu", &(pPoolCfg->Order));
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);   
    }

    _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_TIMEOFFSET);
    if (retPsmGet == CCSP_SUCCESS)
    {
        _ansc_sscanf(param_value, "%d", &(pPoolCfg->X_CISCO_COM_TimeOffset));
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);   
    }

    // Get the UpdateSource info from /opt/secure/bootstrap.json
    CosaDhcpInitJournal(pPoolCfg);
    
    return;
}

static void getDHCPv4ServerPoolOptionFromPSM(ULONG tblInstancenum, ULONG instancenum, PCOSA_DML_DHCPSV4_OPTION pPoolOption)
{
    int retPsmGet = CCSP_SUCCESS;
    char *param_value= NULL;
    char param_name[256]= {0};
    errno_t                         rc              = -1;
    

    //printf("%s: for pool %d, instance %d\n", __FUNCTION__, tblInstancenum, instancenum);
    pPoolOption->InstanceNumber = instancenum; 

    _PSM_READ_TBL_PARAM(PSM_DHCPV4_SERVER_POOL_OPTION_ENABLE);
    if (retPsmGet == CCSP_SUCCESS)
    {
        if(strcmp(param_value, PSM_ENABLE_STRING_TRUE) == 0)
        {   
            pPoolOption->bEnabled = TRUE;
        }
        else
        {
            pPoolOption->bEnabled = FALSE;
        }
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }
    else
    {
        // if there is no record in PSM,initial value for enabled will be false.
        pPoolOption->bEnabled = FALSE;
    }

    _PSM_READ_TBL_PARAM(PSM_DHCPV4_SERVER_POOL_OPTION_ALIAS);
    if (retPsmGet == CCSP_SUCCESS)
    {
        rc = STRCPY_S_NOCLOBBER(pPoolOption->Alias, sizeof(pPoolOption->Alias), param_value);
        ERR_CHK(rc);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }

    _PSM_READ_TBL_PARAM(PSM_DHCPV4_SERVER_POOL_OPTION_TAG);
    if (retPsmGet == CCSP_SUCCESS)
    {
        _ansc_sscanf(param_value, "%lu", &(pPoolOption->Tag));
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);   
    }
    else
    {
        pPoolOption->Tag = 0;
    }

    // read 256 bytes value
    _PSM_READ_TBL_PARAM(PSM_DHCPV4_SERVER_POOL_OPTION_VALUE);
    if (retPsmGet == CCSP_SUCCESS)
    {
        rc = STRCPY_S_NOCLOBBER((char*)pPoolOption->Value, sizeof(pPoolOption->Value), param_value);
        ERR_CHK(rc);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }
    else
    {
       pPoolOption->Value[0] = '\0';
    }

    return;
}

static void readDHCPv4ServerPoolFromPSM()
{
    // get pool count and loop through pools in PSM
        // second pool indexed 1
        // getting pool parameters from PSM for second pool
    unsigned int poolCnt = 0;
    unsigned int* poolList = NULL;
    int retPsmGet = CCSP_SUCCESS;
    PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = NULL;
    PCOSA_DML_DHCPS_POOL_FULL   pPool = NULL;
    unsigned int saddrCnt = 0;
    unsigned int* saddrList = NULL;
    int retPsmGet1 = CCSP_SUCCESS;
    unsigned int optionCnt = 0;
    unsigned int* optionList = NULL;
    PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ pOptionLinkObj = NULL;
    PCOSA_DML_DHCPSV4_OPTION          pOption = NULL;
    char param_name[256]= {0};
    unsigned int i=0, j=0;
    errno_t rc = -1;

    AnscSListInitializeHeader(&g_dhcpv4_server_pool_list);
    retPsmGet = PsmGetNextLevelInstances(bus_handle, g_Subsystem, PSM_DHCPV4_SERVER_POOL, &poolCnt, &poolList);
    if ( retPsmGet == CCSP_SUCCESS && poolList != NULL ) 
    {
        AnscTraceFlow(("%s: poolCnt = %d\n", __FUNCTION__, poolCnt));
        for (i = 0; i< poolCnt; i++)
        {
            AnscTraceFlow(("%s: pool instance %d\n", __FUNCTION__, poolList[i]));
        
            if(poolList[i] == 1)
            {
                // at this time, pool instance 1 is saved in utopia, so it should not be here
                // We are ignoring it if we see instance 1.
                // When we save pool 1 in PSM, we also need to change utopia script to reflect it.
                AnscTraceFlow(("%s: pool instance %d is ignored at this time.\n", __FUNCTION__, poolList[i]));
                continue;
            }

            pPoolLinkObj = (PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ)AnscAllocateMemory(sizeof (COSA_DML_DHCPS_POOL_FULL_LINK_OBJ));
            if(pPoolLinkObj == NULL)
            {
                AnscTraceFlow(("%s: out of memory!!!\n", __FUNCTION__));
                continue;
            }

            AnscSListInitializeHeader(&(pPoolLinkObj->StaticAddressList)); 
            AnscSListInitializeHeader(&(pPoolLinkObj->OptionList));

            pPool = &(pPoolLinkObj->SPool);
            DHCPV4_POOL_SET_DEFAULTVALUE(pPool);

            getDHCPv4ServerPoolParametersFromPSM(poolList[i], pPool);
            

            // find optionList and SAddrList
            rc = sprintf_s(param_name, sizeof(param_name), PSM_DHCPV4_SERVER_POOL_STATICADDRESS, poolList[i]);
            if(rc < EOK)
            {
                ERR_CHK(rc);
                continue;
            }

            retPsmGet1 = PsmGetNextLevelInstances(bus_handle, g_Subsystem, param_name, &saddrCnt, &saddrList);
            if ( retPsmGet1 == CCSP_SUCCESS && saddrList != NULL )
            {
                AnscTraceFlow(("%s: found %d DHCPv4 Server Pool SADDR entry %s\n", __FUNCTION__, saddrCnt, param_name));
                AnscTraceFlow(("%s: not supported for now\n", __FUNCTION__));
                ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(saddrList);
            }
            else
            {
                AnscTraceFlow(("%s: Can't find DHCPv4 Server Pool SADDR entry %s\n", __FUNCTION__, param_name));
            }

            rc = sprintf_s(param_name, sizeof(param_name), PSM_DHCPV4_SERVER_POOL_OPTION, (ULONG)poolList[i]);
            if(rc < EOK)
            {
                ERR_CHK(rc);
                continue;
            }
            retPsmGet1 = PsmGetNextLevelInstances(bus_handle, g_Subsystem, param_name, &optionCnt, &optionList);
            if ( retPsmGet1 == CCSP_SUCCESS && optionList != NULL )
            {
                AnscTraceFlow(("%s: found %u DHCPv4 Server Pool OPTION entry %s\n", __FUNCTION__, optionCnt, param_name));
                //printf("%s: found %d DHCPv4 Server Pool OPTION entry %s\n", __FUNCTION__, optionCnt, param_name);

                for(j=0; j< optionCnt; j++)
                {
                    // get optionList from PSM
                    /* CID:73504 Wrong sizeof argument - modified the struct ptr to struct in sizeof*/
                    pOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)AnscAllocateMemory(sizeof (COSA_DML_DHCPSV4_OPTION_LINK_OBJ));
                    if(pOptionLinkObj == NULL)
                    {
                        AnscTraceFlow(("%s: out of memory!!!\n", __FUNCTION__));
                        continue;
                    }
                    pOption = &(pOptionLinkObj->SPoolOption);

                    //AnscZeroMemory(pOption, sizeof(COSA_DML_DHCPSV4_OPTION));
                    //DHCPV4_POOLOPTION_SET_DEFAULTVALUE(pOption);

                    getDHCPv4ServerPoolOptionFromPSM(poolList[i], optionList[j], pOption);

                    AnscSListPushEntryAtBack(&(pPoolLinkObj->OptionList), &pOptionLinkObj->Linkage); 
                }
                ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(optionList);
            }
            else
            {
                AnscTraceFlow(("%s: Can't find DHCPv4 Server Pool OPTION entry %s\n", __FUNCTION__, param_name));
                //printf("%s: Can't find DHCPv4 Server Pool OPTION entry %s\n", __FUNCTION__, param_name);
            }
            // push pool to the end of list
            AnscSListPushEntryAtBack(&g_dhcpv4_server_pool_list, &pPoolLinkObj->Linkage);   

        }// poolCnt


        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(poolList);
    } // pool list
    else
    {
        AnscTraceFlow(("%s: Can't find DHCPv4 Server Pool %s\n", __FUNCTION__, PSM_DHCPV4_SERVER_POOL));
    }
    return;
}


ANSC_STATUS
CosaDmlDhcpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    readDHCPv4ServerPoolFromPSM();

    //PCOSA_DML_DHCPS_POOL_FULL pServerPool = &g_dhcpv4_server_pool2;
    //DHCPV4_POOL_SET_DEFAULTVALUE(pServerPool);

    /*
     * The thread has two purposes, 
     *  1. Provide wifi connected clients mac list
     *  2. Implement X_CISCO_COM_Interface to decide if clients coming from WiFi-2.4 or WiFi-5
     * Purpose 1 has no use since enhancements of Lm_hosts 
     * Purpose 2 can be achieved through Lm_hosts API 
     *  
     * Based on above, the polling thread will be removed. 
     */
    //pthread_t pid;
    //pthread_create(&pid, NULL, usg_get_cpe_associated_ssid, NULL);
    return ANSC_STATUS_SUCCESS;
}

#ifndef FEATURE_RDKB_WAN_MANAGER
/*
    Description:
        The API retrieves the number of DHCP clients in the system.
 */
ULONG
CosaDmlDhcpcGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return 1;
}

/*
    Description:
        The API retrieves the complete info of the DHCP clients designated by index. The usual process is the caller gets the total number of entries, then iterate through those by calling this API.
    Arguments:
        ulIndex        Indicates the index number of the entry.
        pEntry        To receive the complete info of the entry.
*/
ANSC_STATUS
CosaDmlDhcpcGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPC_FULL        pEntry
    )
{
	UNREFERENCED_PARAMETER(hContext);
        if(ulIndex >0)
		return(ANSC_STATUS_FAILURE);
	pEntry->Cfg.InstanceNumber = 1;
	CosaDmlDhcpcGetCfg(hContext, &pEntry->Cfg);
	CosaDmlDhcpcGetInfo(hContext, ulIndex,&pEntry->Info);
	return ANSC_STATUS_SUCCESS;  
}

/*
    Function Name: CosaDmlDhcpcSetValues
    Description:
        The API set the the DHCP clients' instance number and alias to the syscfg DB.
    Arguments:
        ulIndex        Indicates the index number of the entry.
        ulInstanceNumber Client's Instance number 
        pAlias          pointer to the Client's Alias
*/
ANSC_STATUS
CosaDmlDhcpcSetValues
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
    return ANSC_STATUS_FAILURE;
}

/*
    Description:
        The API adds DHCP client. 
    Arguments:
        pEntry        Caller fills in pEntry->Cfg, except Alias field. Upon return, callee fills pEntry->Cfg.Alias field and as many as possible fields in pEntry->Info.
*/
ANSC_STATUS
CosaDmlDhcpcAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_FULL        pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}

/*
    Description:
        The API removes the designated DHCP client entry. 
    Arguments:
        pAlias        The entry is identified through Alias.
*/
ANSC_STATUS
CosaDmlDhcpcDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    return ANSC_STATUS_FAILURE;
}

/*
Description:
    The API re-configures the designated DHCP client entry. 
Arguments:
    pAlias        The entry is identified through Alias.
    pEntry        The new configuration is passed through this argument, even Alias field can be changed.
*/

ANSC_STATUS
CosaDmlDhcpcSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_CFG         pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCfg);
    /*don't allow to change dhcp configuration*/ 

    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpcGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_CFG         pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
	char ifname[32];
	errno_t rc = -1;
  
    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }
    
	rc = strcpy_s(pCfg->Alias, sizeof(pCfg->Alias), "eRouter");
	ERR_CHK(rc);
	pCfg->bEnabled = TRUE;
	pCfg->InstanceNumber = 1;
	if(dhcpv4c_get_ert_ifname(ifname))
		pCfg->Interface[0] = 0;
	else
	{
		rc = strcpy_s(pCfg->Interface, sizeof(pCfg->Interface), ifname);
		ERR_CHK(rc);
	}
	pCfg->PassthroughEnable = TRUE;
	pCfg->PassthroughDHCPPool[0] = 0;
      
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpcGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPC_INFO        pInfo
    )
{
    UNREFERENCED_PARAMETER(hContext);
	ULONG i;
	dhcpv4c_ip_list_t ad;
    
    if ( (!pInfo) || (ulInstanceNumber != 1) ){
        return ANSC_STATUS_FAILURE;
    }

	pInfo->Status = COSA_DML_DHCP_STATUS_Enabled;
	dhcpv4c_get_ert_fsm_state((int*)&pInfo->DHCPStatus);
	dhcpv4c_get_ert_ip_addr((unsigned int*)&pInfo->IPAddress.Value);
	dhcpv4c_get_ert_mask((unsigned int*)&pInfo->SubnetMask.Value);
	pInfo->NumIPRouters = 1;
	dhcpv4c_get_ert_gw((unsigned int*)&pInfo->IPRouters[0].Value);
	ad.number = 0;
	dhcpv4c_get_ert_dns_svrs(&ad);
	pInfo->NumDnsServers = ad.number;
    if (pInfo->NumDnsServers > COSA_DML_DHCP_MAX_ENTRIES)
    {
        CcspTraceError(("!!! Max DHCP Entry Overflow: %d",ad.number));
	    pInfo->NumDnsServers = COSA_DML_DHCP_MAX_ENTRIES; // Fail safe
    }
	for(i=0; i< pInfo->NumDnsServers;i++)
		pInfo->DNSServers[i].Value = ad.addrs[i];
	dhcpv4c_get_ert_remain_lease_time((unsigned int*)&pInfo->LeaseTimeRemaining);
	dhcpv4c_get_ert_dhcp_svr((unsigned int*)&pInfo->DHCPServer);
   
    return ANSC_STATUS_SUCCESS;
}

/*
    Description:
        The API initiates a DHCP client renewal. 
    Arguments:
        pAlias        The entry is identified through Alias.
*/
ANSC_STATUS
CosaDmlDhcpcRenew
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{  
	UNREFERENCED_PARAMETER(hContext);
        if(ulInstanceNumber != 1)
		return(ANSC_STATUS_FAILURE);
#ifndef _HUB4_PRODUCT_REQ_
	v_secure_system("sysevent set dhcp_client-renew");
#endif
	return(ANSC_STATUS_SUCCESS);
}

/*
 *  DHCP Client Send/Req Option
 *
 *  The options are managed on top of a DHCP client,
 *  which is identified through pClientAlias
 */

/*
static BOOL CosaDmlDhcpcWriteOptions(ULONG ulClientInstanceNumber)
{
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    return FALSE;
}*/


ULONG
CosaDmlDhcpcGetNumberOfSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    return 0;
}

ANSC_STATUS
CosaDmlDhcpcGetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCP_OPT          pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpcGetSentOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    )
{   
    UNREFERENCED_PARAMETER(hContext);
    ULONG index   = 0;
    ULONG i     = 0;

    for(i = 0; i < g_Dhcp4ClientNum; i++)
    {
        if ( CH_g_dhcpv4_client[i].Cfg.InstanceNumber ==  ulClientInstanceNumber)
        {
            for( index = 0;  index < g_Dhcp4ClientSentOptNum[i]; index++)
            {
                if ( pEntry->InstanceNumber == g_dhcpv4_client_sent[i][index].InstanceNumber )
                {
                    AnscCopyMemory( pEntry, &g_dhcpv4_client_sent[i][index], sizeof(COSA_DML_DHCP_OPT));
                    return ANSC_STATUS_SUCCESS;
                }
            } 
        }
    }

    return ANSC_STATUS_FAILURE;
}
    

ANSC_STATUS
CosaDmlDhcpcSetSentOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext   ctx;
    CHAR            inst_str[64];    
    ULONG           i = 0;
    errno_t         rc = -1;
    
    ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: ulClientInstanceNumber %d,ulIndex %d, ulInstanceNumber %d\n", 
          __FUNCTION__, ulClientInstanceNumber, ulIndex, ulInstanceNumber);
          
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }
    
    for (i = 0; i < g_Dhcp4ClientNum; i++)
    {
        if ( CH_g_dhcpv4_client[i].Cfg.InstanceNumber ==  ulClientInstanceNumber)
        {
            if ( ulIndex  > g_Dhcp4ClientSentOptNum[i] )
            {
                ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: failed line %d\n", __FUNCTION__, __LINE__);
                Utopia_Free(&ctx,0);
                return ANSC_STATUS_FAILURE;
            }
            g_dhcpv4_client_sent[i][ulIndex].InstanceNumber  = ulInstanceNumber;
            rc = STRCPY_S_NOCLOBBER(g_dhcpv4_client_sent[i][ulIndex].Alias, sizeof(g_dhcpv4_client_sent[i][ulIndex].Alias), pAlias);
            ERR_CHK(rc);

            if( !_ansc_strncmp(CH_g_dhcpv4_client[i].Cfg.Interface, COSA_DML_DHCPV4_CLIENT_IFNAME, _ansc_strlen(COSA_DML_DHCPV4_CLIENT_IFNAME)))
            {                
                rc = sprintf_s(inst_str, sizeof(inst_str), "%lu", ulInstanceNumber);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                Utopia_RawSet(&ctx, COSA_DHCP4_SYSCFG_NAMESPACE, "tr_dhcp4_sent_instance_wan", inst_str);
                Utopia_RawSet(&ctx, COSA_DHCP4_SYSCFG_NAMESPACE, "tr_dhcp4_sent_alias_wan", pAlias);
            }
            else
            {
                ULOGF(ULOG_SYSTEM, UL_DHCP, "%s: not " COSA_DML_DHCPV4_CLIENT_IFNAME " nor lan0, New Interface %s\n", __FUNCTION__, CH_g_dhcpv4_client[i].Cfg.Interface);
                rc = sprintf_s(inst_str, sizeof(inst_str), "%lu", ulInstanceNumber);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                Utopia_RawSet(&ctx, COSA_DHCP4_SYSCFG_NAMESPACE, "tr_dhcp4_sent_instance_unknown", inst_str);
                Utopia_RawSet(&ctx, COSA_DHCP4_SYSCFG_NAMESPACE, "tr_dhcp4_sent_alias_unknown", pAlias);
                //return ANSC_STATUS_FAILURE;
            }
            Utopia_Free(&ctx, 1);
            return ANSC_STATUS_SUCCESS;
        }
    }
    Utopia_Free(&ctx, 0);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpcAddSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}



ANSC_STATUS
CosaDmlDhcpcDelSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpcSetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}

/*
 *  DHCP Client Send/Req Option
 *
 *  The options are managed on top of a DHCP client,
 *  which is identified through pClientAlias
 */
ULONG
CosaDmlDhcpcGetNumberOfReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    )
{ 
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    return 0;
}

ANSC_STATUS
CosaDmlDhcpcGetReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    )
{   
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;    
}

ANSC_STATUS
CosaDmlDhcpcGetReqOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    )
{   
   UNREFERENCED_PARAMETER(hContext);
   UNREFERENCED_PARAMETER(ulClientInstanceNumber);
   UNREFERENCED_PARAMETER(pEntry);
   return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpcSetReqOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{ 
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    UNREFERENCED_PARAMETER(pAlias);
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpcAddReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpcDelReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpcSetReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulClientInstanceNumber);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}
#endif
/*
 *  DHCP Server
 */
ANSC_STATUS
CosaDmlDhcpsEnable
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bEnable
    )
{ 
    UNREFERENCED_PARAMETER(hContext);
    int rc = -1;
    UtopiaContext ctx;

    // Try write to PSM first
    int retPsmSet = CCSP_SUCCESS;
    char param_name[256] = {0};
    char param_value[256] = {0};
    errno_t safec_rc = -1;

    safec_rc = strcpy_s(param_value, sizeof(param_value), (bEnable ? PSM_ENABLE_STRING_TRUE : PSM_ENABLE_STRING_FALSE));
    if (safec_rc != EOK) {
        ERR_CHK(safec_rc);
        return ANSC_STATUS_FAILURE;
    }

    safec_rc = strcpy_s(param_name, sizeof(param_name), PSM_DHCPV4_SERVER_ENABLE);
    if(safec_rc != EOK)
    {
        ERR_CHK(safec_rc);
        return ANSC_STATUS_FAILURE;
    }
    retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
    if (retPsmSet != CCSP_SUCCESS) {
        AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));
    } 
    else 
    { 
        AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = %s \n", __FUNCTION__,param_name,param_value)); 
    }
      
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_SetDhcpServerEnable(&ctx, bEnable);

    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);
    
    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}

/*
    Description:
        The API retrieves the current state of DHCP server: Enabled or Disabled. 
*/
BOOLEAN
CosaDmlDhcpsGetState
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    int rc = -1;
    UtopiaContext ctx;
    BOOLEAN bEnabled = FALSE;
    
    // Read from PSM first
    int retPsmGet = CCSP_SUCCESS;
    char param_name[256] = {0};
    char* param_value = NULL;
    errno_t safec_rc = -1;

    safec_rc = strcpy_s(param_name, sizeof(param_name), PSM_DHCPV4_SERVER_ENABLE);
    if(safec_rc != EOK)
    {
        ERR_CHK(safec_rc);
        return FALSE;
    }
    retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value);
    if (retPsmGet != CCSP_SUCCESS) {
        AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value));
    } 
    else 
    { 
       // AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = %s \n", __FUNCTION__,param_name,param_value)); 
        /* reading from PSM, but take the one from Utopia.
        if(strcmp(param_value, PSM_ENABLE_STRING_TRUE))
        {
            bEnabled = TRUE;
        }
        else
        {
            bEnabled = FALSE;
        }
        */
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }
    
    if(!Utopia_Init(&ctx))
        return FALSE; /* Default Value */

    rc = Utopia_GetDhcpServerEnable(&ctx, &bEnabled);

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    if (rc != 0)
       return FALSE; /* Default value */ 
    else
       return bEnabled;
}

/*
 *  DHCP Server Pool
 */
ULONG
CosaDmlDhcpsGetNumberOfPools
    (
        ANSC_HANDLE                 hContext
    )
{
    // utopia returns the first pool, and other pools are from pool list
    // we will move first pool from utopia to psm
    UNREFERENCED_PARAMETER(hContext);
    return Utopia_GetNumberOfDhcpV4ServerPools() + AnscSListQueryDepth(&g_dhcpv4_server_pool_list); 
}

// init function for server pool
ANSC_STATUS
CosaDmlDhcpsGetPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_POOL_FULL   pEntry
    )
{

    UNREFERENCED_PARAMETER(hContext);
    AnscTraceFlow(("%s: ulIndex = %lu\n", __FUNCTION__, ulIndex));

    if(ulIndex == 0){
        // ulIndex start from 0 for the 1st pool
        int rc = -1;
        UtopiaContext ctx;

        if(!Utopia_Init(&ctx))
            return ANSC_STATUS_FAILURE;

        rc = Utopia_GetDhcpV4ServerPoolEntry(&ctx, ulIndex, pEntry);

        /* Free Utopia Context */
        Utopia_Free(&ctx,0);

        if (rc != 0)
            return ANSC_STATUS_FAILURE;
        else
            return ANSC_STATUS_SUCCESS;
    }
    else 
    {
        // second pool indexed 1
        // getting pool parameters from PSM for second pool
        // Get pool from list
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj=NULL;
        PCOSA_DML_DHCPS_POOL_FULL pPool=NULL;
        PCOSA_DML_DHCPS_POOL_CFG pPoolCfg = NULL;
        PCOSA_DML_DHCPS_POOL_INFO pPoolInfo = NULL;

        AnscTraceFlow(("%s: getting DHCPv4 Server pool index %lu\n", __FUNCTION__, ulIndex));
        pPoolLinkObj = (PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ)AnscSListGetEntryByIndex(&g_dhcpv4_server_pool_list,ulIndex-1);
        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool index %lu\n", __FUNCTION__, ulIndex));
            return ANSC_STATUS_CANT_FIND;
        }
        pPool = &(pPoolLinkObj->SPool);

        // need to copy Cfg and Info
        pPoolCfg = &(pPool->Cfg);
        pPoolInfo = &(pPool->Info);
        AnscTraceFlow(("%s:found index %lu, instancenum %lu\n", __FUNCTION__, ulIndex, pPoolCfg->InstanceNumber));
        AnscCopyMemory(&(pEntry->Cfg), pPoolCfg, sizeof(COSA_DML_DHCPS_POOL_CFG));
        AnscCopyMemory(&(pEntry->Info), pPoolInfo, sizeof(COSA_DML_DHCPS_POOL_INFO));

    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsSetPoolValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{

    UNREFERENCED_PARAMETER(hContext);
    AnscTraceFlow(("%s: ulIndex = %lu, ulInstanceNumber %lu\n", __FUNCTION__, ulIndex, ulInstanceNumber));

    if(ulIndex == 0){
        int rc = -1;
        UtopiaContext ctx;

        if(ulInstanceNumber != 1){/*Note: should be changed if there are multiple pool*/
            syslog(LOG_ERR, "SBAPI->CosaDmlDhcpsSetPoolValues:Instance number of DHCP Pool is not 1");
            return(ANSC_STATUS_FAILURE);
        }

        if(!Utopia_Init(&ctx))
            return ANSC_STATUS_FAILURE;

        rc = Utopia_SetDhcpV4ServerPoolValues(&ctx, ulIndex, ulInstanceNumber, pAlias);

        /* Free Utopia Context */
        Utopia_Free(&ctx,!rc);

        if (rc != 0)
            return ANSC_STATUS_FAILURE;
        else
            return ANSC_STATUS_SUCCESS;
    }
    else
    {
        // setting cfg parameters for the second pool, write to PSM
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj=NULL;
        PCOSA_DML_DHCPS_POOL_FULL pPool=NULL;
        PCOSA_DML_DHCPS_POOL_CFG pPoolCfg = NULL;
        AnscTraceFlow(("%s: getting DHCPv4 Server pool index %lu\n", __FUNCTION__, ulIndex));
        pPoolLinkObj = (PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ)AnscSListGetEntryByIndex(&g_dhcpv4_server_pool_list,ulIndex-1);
        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool index %lu\n", __FUNCTION__, ulIndex));
            return ANSC_STATUS_CANT_FIND;
        }

        pPool = &(pPoolLinkObj->SPool);

        int retPsmSet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char param_value[256] = {0};
        ULONG instancenum = ulInstanceNumber;
        errno_t safec_rc = -1;
        pPoolCfg = &(pPool->Cfg);
        memset(param_value, 0, sizeof(param_value));
        memset(param_name, 0, sizeof(param_name));
    
        if (strcmp(pAlias, pPoolCfg->Alias) != 0)
        {
            safec_rc = strcpy_s(param_value, sizeof(param_value), pAlias);
            if(safec_rc != EOK)
            {
                ERR_CHK(safec_rc);
                return ANSC_STATUS_FAILURE;
            }
            _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_ALIAS);
            safec_rc = STRCPY_S_NOCLOBBER(pPoolCfg->Alias, sizeof(pPoolCfg->Alias), pAlias);
            if ( safec_rc != EOK )
            {
                ERR_CHK(safec_rc);
                return ANSC_STATUS_FAILURE;
            }
        }

        return ANSC_STATUS_SUCCESS;
    }
}

static PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ find_dhcpv4_pool_by_instancenum(ULONG instancenum)
{
    PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ curPoolLinkObj = (PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ)AnscSListGetFirstEntry(&g_dhcpv4_server_pool_list);
    while(curPoolLinkObj != NULL)
    {
        if(curPoolLinkObj->SPool.Cfg.InstanceNumber == instancenum)
        {
            AnscTraceFlow(("%s: found DHCPv4 Server Pool for instance number %lu\n", __FUNCTION__, instancenum));
            return curPoolLinkObj;
        }

        curPoolLinkObj = (PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ)curPoolLinkObj->Linkage.Next;
    }

    AnscTraceFlow(("%s:DHCPv4 Server Pool not found for instance number %lu\n", __FUNCTION__, instancenum));
    return NULL;

}

ANSC_STATUS
CosaDmlDhcpsAddPool
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_FULL   pEntry
    )
{
    
    UNREFERENCED_PARAMETER(hContext);
    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = NULL;
        PCOSA_DML_DHCPS_POOL_FULL pPool = NULL;
        BOOLEAN dhcpServerRestart = FALSE;

        syslog(LOG_INFO, "SBAPI->CosaDmlDhcpsAddPool:Instance %lu",pEntry->Cfg.InstanceNumber);
        pPoolLinkObj = (PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ)AnscAllocateMemory(sizeof(COSA_DML_DHCPS_POOL_FULL_LINK_OBJ));
        if (!pPoolLinkObj)
        {
            AnscTraceFlow(("%s: Out of Memory!\n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;        
        }
        AnscSListInitializeHeader(&(pPoolLinkObj->StaticAddressList)); 
        AnscSListInitializeHeader(&(pPoolLinkObj->OptionList));

        pPool = &(pPoolLinkObj->SPool);
        DHCPV4_POOL_SET_DEFAULTVALUE(pPool);

        pPool->Cfg.InstanceNumber = pEntry->Cfg.InstanceNumber;
        AnscTraceFlow(("%s: AnscSListPushEntryAtBack instancenum = %lu\n", __FUNCTION__, pPool->Cfg.InstanceNumber));
        AnscSListPushEntryAtBack(&g_dhcpv4_server_pool_list, &pPoolLinkObj->Linkage);
        
        // Write CFG values to PSM
        // since this is new table, we don't check address range for now.

        dhcpServerRestart = writeDHCPv4ServerPoolCFGToPSM(&(pEntry->Cfg), &(pPool->Cfg));

        if(dhcpServerRestart)
        {
            AnscTraceFlow(("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__));
            printf("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__);
            v_secure_system("sysevent set dhcp_server-resync");
            v_secure_system("sysevent set dhcp_server-restart");
        }
        
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsDelPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    syslog(LOG_ERR, "SBAPI->CosaDmlDhcpsDelPool:Instance %lu",ulInstanceNumber);
    PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(ulInstanceNumber);
    if(pPoolLinkObj == NULL)
    {
        AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, ulInstanceNumber));
        return ANSC_STATUS_CANT_FIND;
    }

    if(AnscSListQueryDepth( &pPoolLinkObj->StaticAddressList ) != 0)
    {
        AnscTraceFlow(("%s:DHCPv4 server pool %lu is not empty, %d static addresses\n", __FUNCTION__, ulInstanceNumber, AnscSListQueryDepth( &pPoolLinkObj->StaticAddressList )));
        return ANSC_STATUS_CANT_FIND;
    }

    if(AnscSListQueryDepth( &pPoolLinkObj->OptionList ) != 0)
    {
        AnscTraceFlow(("%s:DHCPv4 server pool %lu is not empty, %d options\n", __FUNCTION__, ulInstanceNumber, AnscSListQueryDepth( &pPoolLinkObj->OptionList )));
        return ANSC_STATUS_CANT_FIND;
    }

    AnscTraceFlow(("%s:pop link instancenum = %lu\n", __FUNCTION__, pPoolLinkObj->SPool.Cfg.InstanceNumber));
    AnscSListPopEntryByLink(&g_dhcpv4_server_pool_list, &pPoolLinkObj->Linkage);

    deleteDHCPv4ServerPoolPSM(ulInstanceNumber);
    AnscFreeMemory(pPoolLinkObj);

    AnscTraceFlow(("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__));
    v_secure_system("sysevent set dhcp_server-resync");
    v_secure_system("sysevent set dhcp_server-restart");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsSetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_CFG    pCfg
    )
{

    UNREFERENCED_PARAMETER(hContext);
    AnscTraceFlow(("%s: pCfg->InstanceNumber =%lu\n", __FUNCTION__, pCfg->InstanceNumber));

    if(pCfg->InstanceNumber == 1){
        int rc = -1;
        UtopiaContext ctx;
        COSA_DML_DHCPS_POOL_CFG tempCfg;
        char startIp[20],endIp[20];

        if(pCfg->InstanceNumber != 1){
            syslog(LOG_ERR, "SBAPI->CosaDmlDhcpsSetPoolCfg:Instance number of DHCP Pool is not 1");
            return ANSC_STATUS_FAILURE;
        }
        if(is_a_public_addr(ntohl(pCfg->MinAddress.Value)) || 
	    is_a_public_addr(ntohl(pCfg->MaxAddress.Value)))
            return ANSC_STATUS_FAILURE;

        if(!Utopia_Init(&ctx))
            return ANSC_STATUS_FAILURE;

        tempCfg.InstanceNumber = pCfg->InstanceNumber ;
        Utopia_GetDhcpV4ServerPoolCfg(&ctx, &tempCfg);
        rc = Utopia_SetDhcpV4ServerPoolCfg(&ctx, pCfg);

        /* Free Utopia Context */
        Utopia_Free(&ctx,!rc);

        if (rc != 0)
            return ANSC_STATUS_FAILURE;
        else{
            if((tempCfg.MinAddress.Value!=pCfg->MinAddress.Value)||(tempCfg.MaxAddress.Value!=pCfg->MaxAddress.Value)
                ||(tempCfg.LeaseTime!=pCfg->LeaseTime)){
                snprintf(startIp,sizeof(startIp),"%d.%d.%d.%d",pCfg->MinAddress.Dot[0],pCfg->MinAddress.Dot[1],pCfg->MinAddress.Dot[2],pCfg->MinAddress.Dot[3]);
                snprintf(endIp,sizeof(endIp),"%d.%d.%d.%d",pCfg->MaxAddress.Dot[0],pCfg->MaxAddress.Dot[1],pCfg->MaxAddress.Dot[2],pCfg->MaxAddress.Dot[3]);
                syslog_systemlog("LAN DHCPv4 Server", LOG_NOTICE, "Configuration change to: Start->%s End->%s Lease time->%d", startIp,endIp,pCfg->LeaseTime);
            }
            return ANSC_STATUS_SUCCESS;
        }
    }
    else
    {
        // setting cfg parameters for the second pool, write to PSM
        PCOSA_DML_DHCPS_POOL_CFG pPoolCfg = NULL;
        BOOLEAN dhcpServerRestart = FALSE;
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(pCfg->InstanceNumber);

        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, pCfg->InstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }

        pPoolCfg = &(pPoolLinkObj->SPool.Cfg);

        // check address only for pool 2
        if( (pCfg->InstanceNumber == 2) &&
            ( is_a_public_addr(ntohl(pCfg->MinAddress.Value)) || 
              is_a_public_addr(ntohl(pCfg->MaxAddress.Value)) ) )
        {
            AnscTraceFlow(("%s: MinAddress %d.%d.%d.%d or MacAddress %d.%d.%d.%d range checking error\n", __FUNCTION__,
                pCfg->MinAddress.Dot[0],pCfg->MinAddress.Dot[1],pCfg->MinAddress.Dot[2],pCfg->MinAddress.Dot[3],
                pCfg->MaxAddress.Dot[0],pCfg->MaxAddress.Dot[1],pCfg->MaxAddress.Dot[2],pCfg->MaxAddress.Dot[3]));
            return ANSC_STATUS_FAILURE;
        }

        dhcpServerRestart = writeDHCPv4ServerPoolCFGToPSM(pCfg, pPoolCfg);

        if(dhcpServerRestart)
        {
            // resync for the second pool
            AnscTraceFlow(("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__));
            printf("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__);
            v_secure_system("sysevent set dhcp_server-resync");
            v_secure_system("sysevent set dhcp_server-restart");
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDhcpsSetIpv4Status()
{
//update ipv4_4-status value here based on lan_status-dhcp event
        token_t        se_token;
        char           dhcp_ipv4Status[64]={'\0'};
        int            se_fd = -1;
        char           evtValue[10]={'\0'};

        se_fd = s_sysevent_connect(&se_token);
        if (0 > se_fd) {

            AnscTraceFlow(("%s: dhcpipv4_status = syseventerror\n", __FUNCTION__));

            AnscCopyString(dhcp_ipv4Status, "syseventError");
            return ANSC_STATUS_FAILURE;
        }
        else
        {
            if( 0 == sysevent_get(se_fd, se_token, "lan_status-dhcp", evtValue, sizeof(evtValue)) )
            {
                if ('\0' == evtValue[0])
                {

                    AnscTraceFlow(("%s: ipv4_4-status needs to be redo to start Authoritative dns\n",__FUNCTION__));
                    /* Get DHCP Server Status */
                    sysevent_set(se_fd, se_token, "ipv4_4-status", "down", 0);
                    sleep(3);
                    sysevent_set(se_fd, se_token, "ipv4_4-status", "up", 0);

                }
                else
                {
                    AnscTraceFlow(("%s: ip4_4-status=%s\n",__FUNCTION__,evtValue));
                }
            }
        }
  	sysevent_close(se_fd,se_token);
        return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDhcpsGetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_CFG    pCfg
    )
{
   
    UNREFERENCED_PARAMETER(hContext);
    AnscTraceFlow(("%s: pCfg->InstanceNumber =%lu\n", __FUNCTION__, pCfg->InstanceNumber));
 
    if(pCfg->InstanceNumber == 1){
        int rc = -1;
        UtopiaContext ctx;

        if(!Utopia_Init(&ctx))
            return ANSC_STATUS_FAILURE;

        rc = Utopia_GetDhcpV4ServerPoolCfg(&ctx, pCfg);
        
        /* Free Utopia Context */       
        Utopia_Free(&ctx,0);

        if (rc != 0)
            return ANSC_STATUS_FAILURE;
        else
            return ANSC_STATUS_SUCCESS;
    }
    else
    {
        // get cfg information from the second pool
        PCOSA_DML_DHCPS_POOL_CFG pPoolCfg = NULL;
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(pCfg->InstanceNumber);

        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, pCfg->InstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }

        pPoolCfg = &(pPoolLinkObj->SPool.Cfg);
        AnscCopyMemory(pCfg, pPoolCfg, sizeof(COSA_DML_DHCPS_POOL_CFG));
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsGetPoolInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPS_POOL_INFO   pInfo
    )
{

    UNREFERENCED_PARAMETER(hContext);
    AnscTraceFlow(("%s: ulInstanceNumber =%lu\n", __FUNCTION__, ulInstanceNumber));
    errno_t                         rc              = -1;
   
    if(ulInstanceNumber == 1) {
        int rc = -1;
        UtopiaContext ctx;
        PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hContext;
        PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;

        if(!Utopia_Init(&ctx))
            return ANSC_STATUS_FAILURE;

        rc = Utopia_GetDhcpV4ServerPoolInfo(&ctx, ulInstanceNumber, pInfo);

        /* Free Utopia Context */
        Utopia_Free(&ctx,0);
        pInfo->X_CISCO_COM_Connected_Device_Number = sbapi_get_dhcpv4_active_number(
                                                        ulInstanceNumber, 
                                                        pPool->Cfg.MinAddress.Value,
                                                        pPool->Cfg.MaxAddress.Value);

        if (rc != 0)
            return ANSC_STATUS_FAILURE;
        else
            return ANSC_STATUS_SUCCESS;
    }
    else
    {
        // return poolInfo for the second pool, no value for X_CISCO_COM_Connected_Device_Number
        PCOSA_DML_DHCPS_POOL_INFO pPoolInfo = NULL;
        PCOSA_DML_DHCPS_POOL_CFG pPoolCfg = NULL;
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(ulInstanceNumber);

        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, ulInstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }

        pPoolCfg = &(pPoolLinkObj->SPool.Cfg);
        pPoolInfo = &(pPoolLinkObj->SPool.Info);

        //update status value here
        token_t        se_token;
        char           dhcp_status[64];
        int            se_fd = -1;

        se_fd = s_sysevent_connect(&se_token);
        if (0 > se_fd) {

            AnscTraceFlow(("%s: dhcp_status = syseventerror\n", __FUNCTION__));
              
            rc = strcpy_s(dhcp_status, sizeof(dhcp_status), "syseventError");
            ERR_CHK(rc);
        }
        else
        {
            /* Get DHCP Server Status */             
            sysevent_get(se_fd, se_token, "dhcp_server-status", dhcp_status, sizeof(dhcp_status));

            AnscTraceFlow(("%s: dhcp_status = %s\n", __FUNCTION__, dhcp_status));
             
        }

        if (0 == strcmp(dhcp_status, "started")) {
            pPoolInfo->Status = COSA_DML_DHCP_STATUS_Enabled;
        }else if (0 == strcmp(dhcp_status, "error")) {
            if(pPoolCfg->bEnabled)
                pPoolInfo->Status = COSA_DML_DHCP_STATUS_Error_Misconfigured; /*It is enabled but still is stopped */
            else
                pPoolInfo->Status = COSA_DML_DHCP_STATUS_Disabled; /* It is disabled */
        } else {
            pPoolInfo->Status = COSA_DML_DHCP_STATUS_Error;
        }

        pPoolInfo->X_CISCO_COM_Connected_Device_Number = sbapi_get_dhcpv4_active_number(
                                                            ulInstanceNumber, 
                                                            pPoolCfg->MinAddress.Value,
                                                            pPoolCfg->MaxAddress.Value);

        AnscCopyMemory(pInfo, pPoolInfo, sizeof(COSA_DML_DHCPS_POOL_INFO));
    }
    return ANSC_STATUS_SUCCESS;
}

int dhcp_saddr_invalid_addr(PCOSA_DML_DHCPS_SADDR pEntry)
{
	unsigned char zeroMac[6] = {0,0,0,0,0,0};
	if(pEntry->Yiaddr.Value==0){
                return 1;
        }
        if((pEntry->Chaddr[0] & 1 )||(!memcmp(pEntry->Chaddr,zeroMac, 6))){
                return 1;
        }
	return(0);
}

/*
 *  DHCP Server Pool Static Address
 *
 *  The static addresses are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */
ULONG
CosaDmlDhcpsGetNumberOfSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    int count = 0;
    UtopiaContext ctx;

    if(ulPoolInstanceNumber != 1)
    {
        // not supporting Saddr for other pool table yet.
        return 0;
    }

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    count = Utopia_GetDhcpV4SPool_NumOfStaticAddress(&ctx, ulPoolInstanceNumber);

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    return count;
}

/*
 * Getting saddr by index
 */
ANSC_STATUS
CosaDmlDhcpsGetSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{  
    UNREFERENCED_PARAMETER(hContext);
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_GetDhcpV4SPool_SAddress(&ctx, ulPoolInstanceNumber, ulIndex, pEntry);

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsGetSaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{   
    UNREFERENCED_PARAMETER(hContext);
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_GetDhcpV4SPool_SAddressByInsNum(&ctx, ulClientInstanceNumber, pEntry);
    
    /* Free Utopia Context */       
    Utopia_Free(&ctx,0);            

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}


ANSC_STATUS
CosaDmlDhcpsSetSaddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    int rc = -1;
    UtopiaContext ctx;

    if(ulPoolInstanceNumber != 1){
        syslog(LOG_ERR, "SBAPI->CosaDmlDhcpsSetSaddrValues:Instance number of DHCP Pool is not 1");
        return(ANSC_STATUS_FAILURE);
    }

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_SetDhcpV4SPool_SAddress_Values(&ctx, ulPoolInstanceNumber, ulIndex, ulInstanceNumber, pAlias);

    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDhcpsAddSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    int rc = -1;
    UtopiaContext ctx;

    if(ulPoolInstanceNumber != 1){
        syslog(LOG_ERR, "SBAPI->CosaDmlDhcpsAddSaddr:Instance number of DHCP Pool is not 1");
        return(ANSC_STATUS_FAILURE);
    }
    if(dhcp_saddr_invalid_addr(pEntry))
	return(ANSC_STATUS_FAILURE);
    if(is_a_public_addr(ntohl(pEntry->Yiaddr.Value)))
        return(ANSC_STATUS_FAILURE);

    unlink(LAN_NOT_RESTART_FLAG);

    v_secure_system("ip nei | grep %02x:%02x:%02x:%02x:%02x:%02x > /dev/null || touch " LAN_NOT_RESTART_FLAG, pEntry->Chaddr[0], pEntry->Chaddr[1], pEntry->Chaddr[2], pEntry->Chaddr[3], pEntry->Chaddr[4], pEntry->Chaddr[5]);

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_AddDhcpV4SPool_SAddress(&ctx, ulPoolInstanceNumber, pEntry);
        
    /* Free Utopia Context */       
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDhcpsDelSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_DelDhcp4SPool_SAddress(&ctx, ulPoolInstanceNumber, ulInstanceNumber);
        
    /* Free Utopia Context */       
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDhcpsSetSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    int rc = -1;
    UtopiaContext ctx;

    if(ulPoolInstanceNumber != 1){
        syslog(LOG_ERR, "SBAPI->CosaDmlDhcpsSetSaddr:Instance number of DHCP Pool is not 1");
        return(ANSC_STATUS_FAILURE);
    }
    if(dhcp_saddr_invalid_addr(pEntry))
	return(ANSC_STATUS_FAILURE);
    if(is_a_public_addr(ntohl(pEntry->Yiaddr.Value)))
        return(ANSC_STATUS_FAILURE);

    unlink(LAN_NOT_RESTART_FLAG);

    v_secure_system("ip nei | grep %02x:%02x:%02x:%02x:%02x:%02x > /dev/null || touch " LAN_NOT_RESTART_FLAG, pEntry->Chaddr[0], pEntry->Chaddr[1], pEntry->Chaddr[2], pEntry->Chaddr[3], pEntry->Chaddr[4], pEntry->Chaddr[5]);

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_SetDhcpV4SPool_SAddress(&ctx, ulPoolInstanceNumber, pEntry);
        
    /* Free Utopia Context */       
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}
    
/*
 *  DHCP Server Pool Option
 *
 *  The options are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */

ULONG
CosaDmlDhcpsGetNumberOfOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    )
{
    //printf("%s: ulPoolInstanceNumber %d\n", __FUNCTION__, ulPoolInstanceNumber);
    UNREFERENCED_PARAMETER(hContext);
    if(ulPoolInstanceNumber == 1)
    {
        // first pool.
        return sizeof(g_dhcpv4_server_pool_option) / sizeof(COSA_DML_DHCPSV4_OPTION);
    }
    else{
        // other pool
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(ulPoolInstanceNumber);
        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, ulPoolInstanceNumber));
            //printf("%s: can't find DHCPv4 server pool instance %d\n", __FUNCTION__, ulPoolInstanceNumber);
            return ANSC_STATUS_CANT_FIND;
        }
        else
        {
            return AnscSListQueryDepth(&(pPoolLinkObj->OptionList));
        }
    }
    return ANSC_STATUS_SUCCESS;    
}
    
ANSC_STATUS
CosaDmlDhcpsGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    )
{
    //printf("%s: ulPoolInstanceNumber %d, ulIndex %d\n", __FUNCTION__, ulPoolInstanceNumber, ulIndex);
    UNREFERENCED_PARAMETER(hContext);
    if(ulPoolInstanceNumber == 1)
    {
        if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_option)/sizeof(COSA_DML_DHCPSV4_OPTION) )
            return ANSC_STATUS_FAILURE;
    
        AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_option[ulIndex], sizeof(COSA_DML_DHCPSV4_OPTION));
        
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ pPoolOptionLinkObj = NULL;
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(ulPoolInstanceNumber);
        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, ulPoolInstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }
        
        pPoolOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)AnscSListGetEntryByIndex(&(pPoolLinkObj->OptionList),ulIndex);
        if(pPoolOptionLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool option index %lu\n", __FUNCTION__, ulIndex));
            //printf("%s: can't find DHCPv4 server pool option index %d\n", __FUNCTION__, ulIndex);
            return ANSC_STATUS_CANT_FIND;
        }

        AnscCopyMemory(pEntry, &(pPoolOptionLinkObj->SPoolOption), sizeof(COSA_DML_DHCPSV4_OPTION));
        
    }

    return ANSC_STATUS_SUCCESS;
}
 
ANSC_STATUS
CosaDmlDhcpsGetOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    )
{

    //printf("%s: ulPoolInstanceNumber %d\n", __FUNCTION__, ulPoolInstanceNumber);
    UNREFERENCED_PARAMETER(hContext);
    if(ulPoolInstanceNumber == 1)
    {
        ULONG                           index = 0;
    
        for(index = 0; index < sizeof(g_dhcpv4_server_pool_option[index])/sizeof(COSA_DML_DHCPSV4_OPTION); index++)
        {
            if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_option[index].InstanceNumber )
            {
                AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_option[index], sizeof(COSA_DML_DHCPSV4_OPTION));
                return ANSC_STATUS_SUCCESS;
            }
        }
        
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(ulPoolInstanceNumber);
        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, ulPoolInstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }

        PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ curPoolOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)AnscSListGetFirstEntry(&(pPoolLinkObj->OptionList));
        while(curPoolOptionLinkObj != NULL)
        {
            if(curPoolOptionLinkObj->SPoolOption.InstanceNumber == pEntry->InstanceNumber)
            {
                AnscTraceFlow(("%s: found DHCPv4 Server Pool Option for instance number %lu\n", __FUNCTION__, pEntry->InstanceNumber));
                //printf("%s: found DHCPv4 Server Pool Option for instance number %d\n", __FUNCTION__, pEntry->InstanceNumber);
                break;
            }

            curPoolOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)curPoolOptionLinkObj->Linkage.Next;
        }

        if(curPoolOptionLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool option instance %lu\n", __FUNCTION__, pEntry->InstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }
        
        AnscCopyMemory(pEntry, &(curPoolOptionLinkObj->SPoolOption), sizeof(COSA_DML_DHCPSV4_OPTION));
    }

    return ANSC_STATUS_SUCCESS;
}
    
    
ANSC_STATUS
CosaDmlDhcpsSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    errno_t                         rc              = -1;

    //printf("%s: ulPoolInstanceNumber %d, ulIndex %d, ulInstanceNumber %d\n", __FUNCTION__, ulPoolInstanceNumber, ulIndex, ulInstanceNumber);
    UNREFERENCED_PARAMETER(hContext);
    if(ulPoolInstanceNumber == 1)
    {
        if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_option)/sizeof(COSA_DML_DHCPSV4_OPTION) )
            return ANSC_STATUS_FAILURE;
    
        g_dhcpv4_server_pool_option[ulIndex].InstanceNumber  = ulInstanceNumber;
        rc = STRCPY_S_NOCLOBBER(g_dhcpv4_server_pool_option[ulIndex].Alias, sizeof(g_dhcpv4_server_pool_option[ulIndex].Alias), pAlias);
        ERR_CHK(rc);
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ curPoolOptionLinkObj = NULL;
        PCOSA_DML_DHCPSV4_OPTION pPoolOption = NULL;
        //BOOLEAN dhcpServerRestart = FALSE;
        PCOSA_DML_DHCPSV4_OPTION  pNewEntry = NULL;
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(ulPoolInstanceNumber);

        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, ulPoolInstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }
        
        curPoolOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)AnscSListGetFirstEntry(&(pPoolLinkObj->OptionList));
        while(curPoolOptionLinkObj != NULL)
        {
            if(curPoolOptionLinkObj->SPoolOption.InstanceNumber == ulInstanceNumber)
            {
                AnscTraceFlow(("%s: found DHCPv4 Server Pool Option for instance number %lu\n", __FUNCTION__, ulInstanceNumber));
                //printf("%s: found DHCPv4 Server Pool Option for instance number %d\n", __FUNCTION__, ulInstanceNumber);
                break;
            }

            curPoolOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)curPoolOptionLinkObj->Linkage.Next;
        }

        if(curPoolOptionLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool option instance %lu\n", __FUNCTION__, ulInstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }

        // set option value Here YG
        pPoolOption = &(curPoolOptionLinkObj->SPoolOption);

        pNewEntry = (PCOSA_DML_DHCPSV4_OPTION)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV4_OPTION) );
        if ( !pNewEntry )
        {
            AnscTraceFlow(("%s: Out of memory!\n", __FUNCTION__));
            /* CID: 57154 Missing return statement*/
            return ANSC_STATUS_FAILURE;
        }
        AnscCopyMemory(pNewEntry, pPoolOption, sizeof(COSA_DML_DHCPSV4_OPTION));
        rc = STRCPY_S_NOCLOBBER(pNewEntry->Alias, sizeof(pNewEntry->Alias), pAlias);
        ERR_CHK(rc);
        // Write Option to PSM and update to new option value
        writeDHCPv4ServerPoolOptionToPSM(ulPoolInstanceNumber, pNewEntry, pPoolOption);

        /*RDKB-6750, CID-33558, free unused resources before exit*/
        AnscFreeMemory(pNewEntry);
        pNewEntry = NULL;
/* Don't need to restart DHCP server for alias change
        if(dhcpServerRestart)
        {
            AnscTraceFlow(("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__));
            printf("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__);
            system("sysevent set dhcp_server-resync");
            system("sysevent set dhcp_server-restart");
        }
*/
    }

    return ANSC_STATUS_SUCCESS;
}
    
ANSC_STATUS
CosaDmlDhcpsAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION  pEntry
    )
{
    //printf("%s: ulPoolInstanceNumber %d\n", __FUNCTION__, ulPoolInstanceNumber);
    UNREFERENCED_PARAMETER(hContext);
    if(ulPoolInstanceNumber == 1)
    {
        // not supported for pool 1 as before
        return ANSC_STATUS_SUCCESS;
    }

    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {

        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(ulPoolInstanceNumber);
        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, ulPoolInstanceNumber));
            //printf("%s: can't find DHCPv4 server pool instance %d\n", __FUNCTION__, ulPoolInstanceNumber);
            return ANSC_STATUS_CANT_FIND;
        }
        else
        {
            PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ pOptionLinkObj = NULL;
            PCOSA_DML_DHCPSV4_OPTION pPoolOption = NULL;
            BOOLEAN dhcpServerRestart = FALSE;

                
            pOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV4_OPTION_LINK_OBJ));
            if (!pOptionLinkObj)
            {
                AnscTraceFlow(("%s: Out of Memory!\n", __FUNCTION__));
                return ANSC_STATUS_FAILURE;        
            }

            pPoolOption = &(pOptionLinkObj->SPoolOption);
            DHCPV4_POOLOPTION_SET_DEFAULTVALUE(pPoolOption);

            pPoolOption->InstanceNumber = pEntry->InstanceNumber;
            AnscTraceFlow(("%s: AnscSListPushEntryAtBack instancenum = %lu\n", __FUNCTION__, pPoolOption->InstanceNumber));
            AnscSListPushEntryAtBack(&(pPoolLinkObj->OptionList), &pOptionLinkObj->Linkage);

            // Write Option to PSM and update to new option value
            dhcpServerRestart = writeDHCPv4ServerPoolOptionToPSM(ulPoolInstanceNumber, pEntry, pPoolOption);

            if(dhcpServerRestart)
            {
                AnscTraceFlow(("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__));
                printf("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__);
                v_secure_system("sysevent set dhcp_server-resync");
                v_secure_system("sysevent set dhcp_server-restart");
            }
        }
        
    }

    return ANSC_STATUS_SUCCESS;
    
}
    
ANSC_STATUS
CosaDmlDhcpsDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    //printf("SBAPI->CosaDmlDhcpsDelPool:Pool %d, Instance %d\n",ulPoolInstanceNumber, ulInstanceNumber);
     UNREFERENCED_PARAMETER(hContext);
     if(ulPoolInstanceNumber == 1)
    {
        // not supported for pool 1 as before
        return ANSC_STATUS_SUCCESS;
    }   

    AnscTraceFlow(("SBAPI->CosaDmlDhcpsDelPool:Pool %lu, Instance %lu",ulPoolInstanceNumber, ulInstanceNumber));
    PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(ulPoolInstanceNumber);
    if(pPoolLinkObj == NULL)
    {
        AnscTraceFlow(("%s: can't find DHCPv4 server pool %lu\n", __FUNCTION__, ulPoolInstanceNumber));
        return ANSC_STATUS_CANT_FIND;
    }
    // find option
    PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ curPoolOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)AnscSListGetFirstEntry(&(pPoolLinkObj->OptionList));
    while(curPoolOptionLinkObj != NULL)
    {
        if(curPoolOptionLinkObj->SPoolOption.InstanceNumber == ulInstanceNumber)
        {
            AnscTraceFlow(("%s: found DHCPv4 Server Pool Option for instance number %lu\n", __FUNCTION__, ulInstanceNumber));
            break;
        }

        curPoolOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)curPoolOptionLinkObj->Linkage.Next;
    }

    if(curPoolOptionLinkObj == NULL)
    {
        AnscTraceFlow(("%s: can't find DHCPv4 server pool option instance %lu\n", __FUNCTION__, ulInstanceNumber));
        return ANSC_STATUS_CANT_FIND;
    }
    
    AnscTraceFlow(("%s:pop link instancenum = %lu\n", __FUNCTION__, curPoolOptionLinkObj->SPoolOption.InstanceNumber));
    //printf("%s:pop link instancenum = %d\n", __FUNCTION__, curPoolOptionLinkObj->SPoolOption.InstanceNumber);
    AnscSListPopEntryByLink(&(pPoolLinkObj->OptionList), &curPoolOptionLinkObj->Linkage);

    deleteDHCPv4ServerPoolOptionPSM(ulPoolInstanceNumber, ulInstanceNumber);
    AnscFreeMemory(curPoolOptionLinkObj);

    AnscTraceFlow(("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__));
    //printf("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__);
    v_secure_system("sysevent set dhcp_server-resync");
    v_secure_system("sysevent set dhcp_server-restart");
    return ANSC_STATUS_SUCCESS;
    
}
    
ANSC_STATUS
CosaDmlDhcpsSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG                          index = 0;
    //printf("%s:Pool %d, Instance %d",__FUNCTION__, ulPoolInstanceNumber);
    if(ulPoolInstanceNumber == 1)
    {

        for(index = 0; index <sizeof(g_dhcpv4_server_pool_option)/sizeof(COSA_DML_DHCPSV4_OPTION); index++)
        {
            if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_option[index].InstanceNumber )
            {
                AnscCopyMemory( &g_dhcpv4_server_pool_option[index], pEntry, sizeof(COSA_DML_DHCPSV4_OPTION) );
                return ANSC_STATUS_SUCCESS;
            }
        }
    
        return ANSC_STATUS_SUCCESS;    
    }
    else
    {
        PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ pPoolLinkObj = find_dhcpv4_pool_by_instancenum(ulPoolInstanceNumber);
        if(pPoolLinkObj == NULL)
        {
            AnscTraceFlow(("%s: can't find DHCPv4 server pool instance %lu\n", __FUNCTION__, ulPoolInstanceNumber));
            return ANSC_STATUS_CANT_FIND;
        }
        else
        {

            // find option
            PCOSA_DML_DHCPSV4_OPTION pPoolOption = NULL;
            BOOLEAN dhcpServerRestart = FALSE;
            PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ curPoolOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)AnscSListGetFirstEntry(&(pPoolLinkObj->OptionList));
            
            while(curPoolOptionLinkObj != NULL)
            {
                if(curPoolOptionLinkObj->SPoolOption.InstanceNumber == pEntry->InstanceNumber)
                {
                    AnscTraceFlow(("%s: found DHCPv4 Server Pool Option for instance number %lu\n", __FUNCTION__, pEntry->InstanceNumber));
                    //printf("%s: found DHCPv4 Server Pool Option for instance number %d\n", __FUNCTION__, pEntry->InstanceNumber);
                    break;
                }

                curPoolOptionLinkObj = (PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ)curPoolOptionLinkObj->Linkage.Next;
            }

            if(curPoolOptionLinkObj == NULL)
            {
                AnscTraceFlow(("%s: can't find DHCPv4 server pool option instance %lu\n", __FUNCTION__, pEntry->InstanceNumber));
                return ANSC_STATUS_CANT_FIND;
            }
    
            pPoolOption = &(curPoolOptionLinkObj->SPoolOption);

            // Write Option to PSM and update to new option value
            dhcpServerRestart = writeDHCPv4ServerPoolOptionToPSM(ulPoolInstanceNumber, pEntry, pPoolOption);

            if(dhcpServerRestart)
            {
                AnscTraceFlow(("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__));
                printf("%s: notify sysevent dhcp_server-resync and dhcp_server-restart\n", __FUNCTION__);
                v_secure_system("sysevent set dhcp_server-resync");
                v_secure_system("sysevent set dhcp_server-restart");
            }

        }
    }
    return ANSC_STATUS_SUCCESS;
}
 
void mac_string_to_array(char *pStr, unsigned char array[6])
{
    int tmp[6],n,i;

    memset(array,0,6);
    n = sscanf(pStr,"%02x:%02x:%02x:%02x:%02x:%02x",&tmp[0],&tmp[1],&tmp[2],&tmp[3],&tmp[4],&tmp[5]);
    if(n==6){
        for(i=0;i<n;i++)
            array[i] = (unsigned char)tmp[i];
    }
}

int sbapi_get_dhcpv4_active_number(int index, ULONG minAddress, ULONG maxAddress)
{
	UNREFERENCED_PARAMETER(index);
        struct stat fState;
	FILE *fp;
	int amount=0;
	char buffer[1024];
	unsigned long t1;
	char mac[20],ip[16];
    ULONG ipaddr = 0;

	if(stat(COSA_DML_DHCP_LEASES_FILE, &fState)){
		return(0);
	}
        v_secure_system("cp " COSA_DML_DHCP_LEASES_FILE " /var/dhcpClientList");

	snprintf(buffer,sizeof(buffer),"%s", "/var/dhcpClientList");
	fp=fopen(buffer,"r");
	if(fp == NULL){
		AnscTraceFlow(("failed to open temp lease file\n"));
		return(0);
	}
	while(fgets(buffer,sizeof(buffer),fp)){
		if(sscanf((const char*)buffer,"%lu %20s %16s",&t1,mac,ip)<3){
			continue;
		}
        // compare subnet for correct pool
        ipaddr = _ansc_ntohl((ULONG)inet_addr(ip));
        if((ipaddr < _ansc_ntohl(minAddress)) || (ipaddr > _ansc_ntohl(maxAddress)))
        {
            //not in pool range, go next
            AnscTraceFlow(("%s: IP=%x, is not in subnet of %x, and %x\n", __FUNCTION__, (unsigned int)ipaddr, (unsigned int)_ansc_ntohl(minAddress), (unsigned int)_ansc_ntohl(maxAddress)));
            continue;
        }
		if(!find_arp_entry(ip, LAN_L3_IFNAME,(unsigned char*)mac))
			amount++;
	}
	
	fclose(fp);
    v_secure_system("rm -f /var/dhcpClientList");
	return(amount);
}

#define COSA_DML_DHCP_LEASES_FILE_TMP "/tmp/dnsmasq.lease"
/*
COSA_DML_DHCP_LEASES_FILE

8129 00:25:2e:7d:05:b6 192.168.1.135 Cisco 01:00:25:2e:7d:05:b6

COSA_DML_DHCP_OPTIONS_FILE

client 192.168.1.145 5 
option 55 010F03062C2E2F1F2179F92B 
option 60 4D53465420352E30 
option 53 03 

*/
int _cosa_get_dhcps_client(ULONG instancenum, UCHAR *ifName, ULONG minAddress, ULONG maxAddress)
{
    UNREFERENCED_PARAMETER(instancenum);
    FILE * fp = NULL, *fpTmp = NULL;
    PCOSA_DML_DHCPSV4_CLIENT  pEntry  = NULL;
    PCOSA_DML_DHCPSV4_CLIENT  pEntry2 = NULL;
    PCOSA_DML_DHCPSV4_CLIENT  pNewEntry2 = NULL;
    PCOSA_DML_DHCPSV4_CLIENTCONTENT pContentEntry  = NULL;
    PCOSA_DML_DHCPSV4_CLIENTCONTENT pContentEntry2 = NULL;
    PCOSA_DML_DHCPSV4_CLIENTCONTENT pNewContentEntry2 = NULL;
    PCOSA_DML_DHCPSV4_CLIENT_OPTION pContentOptionEntry  = NULL;
    PCOSA_DML_DHCPSV4_CLIENT_OPTION *ppOption = NULL;
    ULONG size = 0, var32=0, sizeClientContent=0;
    ULONG i = 0;
    ULONG j = 0;
    ULONG index = 0;
    char   oneline[1024];
    char * pExpire = NULL,*pTemp;
    char * pMac = NULL;
    char * pIP = NULL;
    char *pHost = NULL, *pVClass = NULL, *pCt=NULL;
    char * pKey = NULL;
    char * pTmp1 = NULL;
    char * pTmp2 = NULL;
    time_t t1    = 0;
    struct tm t2 = {0};
    unsigned char macArray[6]={0};
    UtopiaContext utctx;
    ULONG currClientPool = 50;
    ULONG tempIPAddr=0;
    errno_t rc = -1;

    AnscTraceFlow(("Entered Inside %s\n", __FUNCTION__));
#if 0
    struct timeval tval;   
    gettimeofday(&tval, NULL);
    tm = *localtime(&tval.tv_sec);
    printf("%02d-%02d-%02d ENTERING %s %d\n",tm.tm_hour, tm.tm_min, tm.tm_sec, __func__, __LINE__);
#endif

    fp = fopen(COSA_DML_DHCP_LEASES_FILE, "r");
    if ( !fp )
    {
    	 AnscTraceFlow(("Opening COSA_DML_DHCP_LEASES_FILE failed %s\n", __FUNCTION__));
        /* the file doesn't exist and no host currently*/
        return -1;
    }

    AnscTraceFlow(("Opening COSA_DML_DHCP_LEASES_FILE in a read mode complete %s\n", __FUNCTION__));

    fpTmp = fopen(COSA_DML_DHCP_LEASES_FILE_TMP, "w");
    if (!fpTmp){
        /* failed to create temp leases file */

    	AnscTraceFlow(("Opening COSA_DML_DHCP_LEASES_FILE_TMP failed %s\n", __FUNCTION__));
        fclose(fp);
        return -1;
    }

    AnscTraceFlow(("Opening COSA_DML_DHCP_LEASES_FILE_TMP in a write mode complete, %s\n", __FUNCTION__));
    while(fgets(oneline, sizeof(oneline), fp)){
        fputs(oneline, fpTmp);
    }


    AnscTraceFlow(("Writing dnsmasq lease info to tmp file complete, %s\n", __FUNCTION__));
    fclose(fp);
    fclose(fpTmp);
    fp = NULL;
    fpTmp = NULL;

    fp = fopen(COSA_DML_DHCP_LEASES_FILE_TMP, "r");
    if (!fp){
        /* failed to open tmp lease file */
    	 AnscTraceFlow(("Opening COSA_DML_DHCP_LEASES_FILE_TMP in read mode failed %s\n", __FUNCTION__));
        return -1;
    }
    
    /* first allocate memory for 50 clients */
    pEntry2 = (PCOSA_DML_DHCPSV4_CLIENT)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV4_CLIENT)*currClientPool);
    if (!pEntry2){
        goto ErrRet;
    }
    pContentEntry2 = (PCOSA_DML_DHCPSV4_CLIENTCONTENT)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV4_CLIENTCONTENT)*currClientPool);
    if (!pContentEntry2){
        goto ErrRet;
    }

	while(fgets(&oneline[0], sizeof(oneline), fp) ){
	/*Note: newly added parameters must be set to NULL here*/
		pCt = NULL;
		pVClass = NULL;

		i = 0;
		while( oneline[i] == ' ' ) i++;
		if(oneline[i]==0){/*invalid line, ignore it*/
			continue;
		}
		pExpire = &oneline[i++];
		while( (oneline[i] != ' ') && (oneline[i] != '\0')) i++;
		if(oneline[i] ==0 ){/*invalid line, ignore it*/
			continue;
		}
		oneline[i++] = 0; /*end of expire time*/

		while( oneline[i] == ' ' ) i++;
		if(oneline[i]==0){/*invalid line, ignore it*/
			continue;
		}
		pMac = &oneline[i++];
		while( (oneline[i] != ' ') && (oneline[i] != '\0') ) i++;
		if(oneline[i]==0){/*invalid line, ignore it*/
			continue;
		}
		oneline[i++] = 0; /*end of MAC*/

		while( oneline[i] == ' ' ) i++;
		if(oneline[i]==0){/*invalid line, ignore it*/
			continue;
		}
		pIP = &oneline[i++];
		while( (oneline[i] != ' ') && (oneline[i] != '\0') ) i++;
		if(oneline[i]==0){/*invalid line, ignore it*/
			continue;
		}
		oneline[i++] = 0; /*end of IP Address*/

        // check if IP is in the same subnet
        tempIPAddr = _ansc_ntohl((ULONG)_ansc_inet_addr(pIP));
        if((tempIPAddr < _ansc_ntohl(minAddress)) || (tempIPAddr > _ansc_ntohl(maxAddress)))
        {
            AnscTraceFlow(("%s: IP=%x, is not in subnet between %x, and %x\n", __FUNCTION__, (unsigned int)tempIPAddr, (unsigned int)_ansc_ntohl(minAddress), (unsigned int)_ansc_ntohl(maxAddress)));
            continue;
        }

		while( oneline[i] == ' ' ) i++;
		if(oneline[i]==0){/*invalid line, ignore it*/
			continue;
		}
		pHost = &oneline[i++];
		while( (oneline[i] != ' ') && (oneline[i] != '\0') ) i++;
		if(oneline[i]==0){/*invalid line, ignore it*/
			continue;
		}
		oneline[i++] = 0; /*end of host name*/

		while( oneline[i] == ' ' ) i++;
		if(oneline[i]==0){/*invalid line, ignore it*/
			continue;
		}

		while( (oneline[i] != ' ') && (oneline[i] != '\0') ) i++;
		if(oneline[i]){/*There are newly added parameters*/
			oneline[i++] = 0; /*end of Client Id*/

			/*to get create time*/
			while( oneline[i] == ' ' ) i++;
			if(oneline[i]){/*valid character*/
				pCt = &oneline[i++];
				while( (oneline[i] != ' ') && (oneline[i] != '\0') && (oneline[i] != '\n') ) i++;
				oneline[i++] = 0; /*end of create time*/
			}
		
			/*to get vendor class id*/
			while( oneline[i] == ' ' ) i++;
			if(oneline[i]){/*valid character*/
				pVClass = &oneline[i++];
				/*remove the new line charchter*/
				pTemp = strchr(pVClass, '\n');
				if(pTemp)
				*pTemp = 0;
			}
		}

		/* for client */
		pEntry = &pEntry2[size];
		snprintf(pEntry->Alias, sizeof(pEntry->Alias), "Alias%lu", size);

		if(!find_arp_entry(pIP,(char*)ifName,macArray))
			pEntry->Active = TRUE;
		
		snprintf((char*)pEntry->Chaddr, sizeof(pEntry->Chaddr), "%s", pMac);

        #if !defined(_WNXL11BWL_PRODUCT_REQ_)
		if(pEntry->Active==TRUE)
		{
			usg_get_cpe_associate_interface(pMac, (char*)pEntry->X_CISCO_COM_Interface);
		}
        #endif
		snprintf((char*)pEntry->X_CISCO_COM_HostName, sizeof(pEntry->X_CISCO_COM_HostName), "%s", pHost);
		mac_string_to_array(pMac,macArray);
		if(Utopia_Init(&utctx)){
			Utopia_get_lan_host_comments(&utctx,macArray,pEntry->X_CISCO_COM_Comment);
			Utopia_Free(&utctx, 0);
		}
		// This check is to prevent P&M crash
		if(pVClass != NULL)
                {
			snprintf((char*)pEntry->ClassId, sizeof(pEntry->ClassId), "%s", pVClass);
		}

		/* for client content */
		pContentEntry  = &pContentEntry2[size];
		sizeClientContent = size + 1;
		pContentEntry->pIPAddress = (PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV4_CLIENT_IPADDRESS));
		if ( !pContentEntry->pIPAddress )
			goto ErrRet;
		pContentEntry->pIPAddress[0].IPAddress = (ULONG)_ansc_inet_addr(pIP);
        
		sscanf(pExpire,"%lu", &t1);
		if((unsigned long)t1 == 0){
			snprintf((char*)pContentEntry->pIPAddress[0].LeaseTimeRemaining, sizeof(pContentEntry->pIPAddress[0].LeaseTimeRemaining),"0001-01-01T00:00:00Z");
		}else{
			localtime_r(&t1, &t2);
			rc = sprintf_s((char*)pContentEntry->pIPAddress[0].LeaseTimeRemaining, sizeof(pContentEntry->pIPAddress[0].LeaseTimeRemaining), "%d-%02d-%02dT%02d:%02d:%02dZ",
                 t2.tm_year+1900, t2.tm_mon+1, t2.tm_mday, t2.tm_hour, t2.tm_min, t2.tm_sec);
			if(rc < EOK) ERR_CHK(rc);
		}
		if(pCt){
			sscanf(pCt,"%lu", &t1);
			if((unsigned long)t1 == 0){
				snprintf((char*)pContentEntry->pIPAddress[0].X_CISCO_COM_LeaseTimeCreation, sizeof(pContentEntry->pIPAddress[0].X_CISCO_COM_LeaseTimeCreation),"0001-01-01T00:00:00Z");
			}else{
				localtime_r(&t1, &t2);
				rc = sprintf_s((char*)pContentEntry->pIPAddress[0].X_CISCO_COM_LeaseTimeCreation, sizeof(pContentEntry->pIPAddress[0].X_CISCO_COM_LeaseTimeCreation), "%d-%02d-%02dT%02d:%02d:%02dZ",
                     t2.tm_year+1900, t2.tm_mon+1, t2.tm_mday, t2.tm_hour, t2.tm_min, t2.tm_sec);
				if(rc < EOK) ERR_CHK(rc);
			}
		}else{
			snprintf((char*)pContentEntry->pIPAddress[0].X_CISCO_COM_LeaseTimeCreation, sizeof(pContentEntry->pIPAddress[0].X_CISCO_COM_LeaseTimeCreation),"0001-01-01T00:00:00Z");
		}
	
		pContentEntry->NumberofIPAddress = 1;
		size += 1;
		sizeClientContent = size;
		/* reallocate memory for another 100 clients */
		if (size >= currClientPool){
			currClientPool += 50; 
			pNewEntry2 = (PCOSA_DML_DHCPSV4_CLIENT)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV4_CLIENT)*currClientPool);
			if (!pNewEntry2){
				goto ErrRet;
			}
            /*copy the old mem to the new allocated mem*/
            AnscCopyMemory(pNewEntry2, pEntry2, sizeof(COSA_DML_DHCPSV4_CLIENT)*size);
            AnscFreeMemory(pEntry2);
            pEntry2 = pNewEntry2;

			pNewContentEntry2 = (PCOSA_DML_DHCPSV4_CLIENTCONTENT)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV4_CLIENTCONTENT)*currClientPool);
			if (!pNewContentEntry2){
				goto ErrRet;
			}
            /*copy the old mem to the new allocated mem*/
            AnscCopyMemory(pNewContentEntry2, pContentEntry2, sizeof(COSA_DML_DHCPSV4_CLIENTCONTENT)*sizeClientContent);
            AnscFreeMemory(pContentEntry2);
            pContentEntry2=pNewContentEntry2;
		}
	}
	fclose(fp);
    fp = NULL;

	g_dhcpv4_server_client = pEntry2;
	g_dhcpv4_server_client_count = size;
	g_dhcpv4_server_client_content = pContentEntry2;
	if(size == 0)/*No DHCP Clients*/
		goto ErrRet;
#if 0 
	gettimeofday(&tval, NULL);
	tm = *localtime(&tval.tv_sec);
	printf("%02d-%02d-%02d EXITING %s %d\n", tm.tm_hour, tm.tm_min, tm.tm_sec, __func__, __LINE__);
#endif

        AnscTraceFlow(("%s, Done with client parameters\n", __FUNCTION__));
    /* for option */
    
	fp = fopen(COSA_DML_DHCP_OPTIONS_FILE, "r");
	if ( !fp ){
		/* the file doesn't exist and no options currently*/
		return 0;
	}

    index = 0xFFFFFFFF;
    while(fgets(oneline, sizeof(oneline), fp) )
    {
        i = 0;
        while( (oneline[i] == ' ') && (oneline[i] != '\0') ) i++;
	if(oneline[i] == 0)/*invalid line*/
		continue;
        pKey = &oneline[i++];
        while( (oneline[i] != ' ') && (oneline[i] != '\0') ) i++;       
        oneline[i++] = 0; /*end of first parameter*/

        while( (oneline[i] == ' ') && (oneline[i] != '\0') ) i++;
	if(oneline[i] == 0)/*invalid line*/
		continue;
        pTmp1 = &oneline[i++];
        while( (oneline[i] != ' ') && (oneline[i] != '\0') ) i++;
	if(oneline[i] == 0)/*invalid line*/
		continue;
        oneline[i++] = 0;/*end of 2nd parameter*/

        while( (oneline[i] == ' ') && (oneline[i] != '\0') ) i++;
	if(oneline[i] == 0)/*invalid line*/
		continue;
        pTmp2 = &oneline[i++];
        while( (oneline[i] != ' ') && (oneline[i] != '\0') ) i++;       
        oneline[i++] = 0;

        if ( _ansc_strcmp("client", pKey) == 0 )
        {
            for( i = 0; i < g_dhcpv4_server_client_count; i++ )
            {
                for( j = 0; j < g_dhcpv4_server_client_content[i].NumberofIPAddress; j++ )
                {
                    if ( _ansc_strcmp( _ansc_inet_ntoa(*((struct in_addr*)&(g_dhcpv4_server_client_content[i].pIPAddress[j].IPAddress))), pTmp1 ) == 0 )
                        break;
                }
                if ( j < g_dhcpv4_server_client_content[i].NumberofIPAddress )
                    break;
            }

            if ( i < g_dhcpv4_server_client_count )
                index = i;
            else
                index = 0xFFFFFFFF;
        }
        else if ( _ansc_strcmp("option", pKey) == 0 )
        {
            if ( index == 0xFFFFFFFF )
                continue;

	    pContentOptionEntry = (PCOSA_DML_DHCPSV4_CLIENT_OPTION)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV4_CLIENT_OPTION)*(g_dhcpv4_server_client_content[index].NumberofOption+1));
	    if ( !pContentOptionEntry )
		continue; /*drop this option*/
            var32 = g_dhcpv4_server_client_content[index].NumberofOption++;
            ppOption = &(g_dhcpv4_server_client_content[index].pOption);
            
            if ( *ppOption )
            {
                AnscCopyMemory(pContentOptionEntry, *ppOption, sizeof(COSA_DML_DHCPSV4_CLIENT_OPTION)*var32 );
                AnscFreeMemory(*ppOption);
            }
            *ppOption = pContentOptionEntry;

            (*ppOption)[var32].Tag = atoi(pTmp1);
            memcpy( (*ppOption)[var32].Value, pTmp2, 254 );

        }            
    }
    AnscTraceFlow(("%s, Done with Option parameters\n", __FUNCTION__));
    if (fp)
        fclose(fp);
#if 0
    gettimeofday(&tval, NULL);
    tm = *localtime(&tval.tv_sec);
    printf("%02d-%02d-%02d EXITING %s %d\n", tm.tm_hour, tm.tm_min, tm.tm_sec, __func__, __LINE__);
#endif
    AnscTraceFlow(("Exiting from %s without error\n", __FUNCTION__));
    return 0;

ErrRet:
    if (fp)
        fclose(fp);
    if(pEntry2)
        AnscFreeMemory(pEntry2);
    for(i=0;i<sizeClientContent;i++){
        if(pContentEntry2[i].pIPAddress)
            AnscFreeMemory(pContentEntry2[i].pIPAddress);
	if(pContentEntry2[i].pOption)
	    AnscFreeMemory(pContentEntry2[i].pOption);
	}
    if(pContentEntry2) {
        AnscFreeMemory(pContentEntry2);
    }
    g_dhcpv4_server_client_count = 0;
    AnscTraceFlow(("Exiting from %s with error\n", __FUNCTION__));
    return(-1);
}

/*
 *   This is for 
 *     Pool.{i}.Client.{i}.
 *
 */
static unsigned long ulDhcpsPrevClientNumber = 0;

void CosaDmlDhcpsGetPrevClientNumber(ULONG ulPoolInstanceNumber, ULONG *pNumber)
{
    UNREFERENCED_PARAMETER(ulPoolInstanceNumber);
    *pNumber = ulDhcpsPrevClientNumber;
}

ANSC_STATUS
CosaDmlDhcpsGetClient
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_CLIENT   *ppClient,
        PCOSA_DML_DHCPSV4_CLIENTCONTENT *ppClientContent,
        PULONG                      pSize
    )
{        
    UNREFERENCED_PARAMETER(hContext);
    int                     ret = 0;
    COSA_DML_DHCPS_POOL_CFG poolCfg;
    CHAR                            ucEntryParamName[256] = {0};
    CHAR                            ucEntryNameValue[256] = {0};
    ULONG                           ulEntryNameLen=sizeof(ucEntryNameValue);

    poolCfg.InstanceNumber = ulPoolInstanceNumber;

    CosaDmlDhcpsGetPoolCfg(NULL, &poolCfg);
    //if(ulPoolInstanceNumber != 1)
    //{
        // not supporting other pool yet.
        //return ANSC_STATUS_FAILURE;
    //}
    if(poolCfg.InstanceNumber == 1) {
        snprintf(ucEntryNameValue, sizeof(ucEntryNameValue), "%s", poolCfg.Interface);
    } else {
        snprintf(ucEntryParamName, sizeof(ucEntryParamName), "%s.Name", poolCfg.Interface);
        /* CID 75074 fix*/
        int rc = 1;
        rc = CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen );
        if (rc == -1)
        {
            CcspTraceWarning(("%s CosaGetParamValueString Failed\n", __FUNCTION__ ));
        }
    }
    ret = _cosa_get_dhcps_client(ulPoolInstanceNumber, (unsigned char*)ucEntryNameValue, poolCfg.MinAddress.Value, poolCfg.MaxAddress.Value);

    if ( !ret )
    {
        // this is a strange way to get return value from _cosa_get_dhcps_client
        *ppClient = g_dhcpv4_server_client;
        g_dhcpv4_server_client = NULL;
        *ppClientContent = g_dhcpv4_server_client_content;
        g_dhcpv4_server_client_content = NULL;
        *pSize = g_dhcpv4_server_client_count;
        ulDhcpsPrevClientNumber = g_dhcpv4_server_client_count;
        g_dhcpv4_server_client_count = 0;
        return ANSC_STATUS_SUCCESS;
    }

    *pSize = 0;
    *ppClient = NULL;
    *ppClientContent = NULL;
    ulDhcpsPrevClientNumber = 0;

    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpsPing
    (
        PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS    pDhcpsClient
    )
{
    FILE *fp = NULL;

    /*ping -w 2 -c 1 fe80::225:2eff:fe7d:5b5 */
    fp = v_secure_popen("r","ping -W 1 -c 1 %s", _ansc_inet_ntoa(*((struct in_addr*)&(pDhcpsClient->IPAddress))) );
    if ( _get_shell_output2(fp, "0 packets received"))
    {
        /*1 packets transmitted, 0 packets received, 100% packet loss*/
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        /*1 packets transmitted, 1 packets received, 0% packet loss*/
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlDhcpsARPing
    (
        PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS    pDhcpsClient
    )
{
    FILE *fp = NULL;

    fp = v_secure_popen("r", "arping -I %s -c 2 -f -w 1 %s", LAN_L3_IFNAME, _ansc_inet_ntoa(*((struct in_addr*)&(pDhcpsClient->IPAddress))) );
    if ( _get_shell_output2(fp, "Received 0 reply"))
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlDhcpsGetLeaseTimeDuration
    (
        PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS pDhcpsClient
    )
{
    struct tm t = {0};
    time_t current_time;
    time_t create_time;
    time_t duration_time;
    int d_day, d_hour, d_min, d_sec;
    char time_buf[32] = {0};
    ULONG i;

    /*get current time*/
    current_time = time(NULL);

    /*get lease create time*/
    localtime_r(&current_time, &t);
    strncpy(time_buf, (char*)pDhcpsClient->X_CISCO_COM_LeaseTimeCreation, strlen((const char*)pDhcpsClient->X_CISCO_COM_LeaseTimeCreation));
    for (i = 0; i < strlen(time_buf); i++)
    {
        if('T' == time_buf[i])
            time_buf[i] = ' ';
        if('Z' == time_buf[i])
            time_buf[i] = '\0';
    }
    strptime(time_buf, "%Y-%m-%d %H:%M:%S", &t);
    create_time = mktime(&t);
    
    /*calc lease time duration (s)*/
    duration_time = current_time - create_time;
    d_day = duration_time / (24 * 3600);
    d_hour = duration_time % (24 * 3600) / 3600;
    d_min = duration_time % (24 * 3600) % 3600 / 60;
    d_sec = duration_time % (24 * 3600) % 3600 % 60;
    snprintf((char*)pDhcpsClient->X_CISCO_COM_LeaseTimeDuration, sizeof(pDhcpsClient->X_CISCO_COM_LeaseTimeDuration),
                "D:%02d H:%02d M:%02d S:%02d", d_day, d_hour, d_min, d_sec);


    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsGetX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{   
     UNREFERENCED_PARAMETER(hContext);
     UNREFERENCED_PARAMETER(ulIndex);
     UNREFERENCED_PARAMETER(pEntry);     
#if 0
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_stadicaddress)/sizeof(COSA_DML_DHCPS_SADDR) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_stadicaddress[ulIndex], sizeof(COSA_DML_DHCPS_SADDR));
    
#endif
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDhcpsGetX_COM_CISCO_SaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{
   UNREFERENCED_PARAMETER(hContext);
   UNREFERENCED_PARAMETER(pEntry);
#if 0
    ULONG                           index = 0;

    for(index = 0; index < sizeof(g_dhcpv4_server_pool_stadicaddress[index])/sizeof(COSA_DML_DHCPS_SADDR); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_stadicaddress[index].InstanceNumber )
        {
            AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_stadicaddress[index], sizeof(COSA_DML_DHCPS_SADDR));
            return ANSC_STATUS_SUCCESS;
        }
    }
#endif
    
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpsSetX_COM_CISCO_SaddrValues
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
#if 0
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_stadicaddress)/sizeof(COSA_DML_DHCPS_SADDR) )
        return ANSC_STATUS_SUCCESS;

    g_dhcpv4_server_pool_stadicaddress[ulIndex].InstanceNumber  = ulInstanceNumber;
    AnscCopyString( g_dhcpv4_server_pool_stadicaddress[ulIndex].Alias, pAlias );
#endif

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsAddX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{

    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsDelX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{

    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsSetX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{
   UNREFERENCED_PARAMETER(hContext);
   UNREFERENCED_PARAMETER(pEntry);
#if 0
    ULONG                           index = 0;

    for(index = 0; index < sizeof(g_dhcpv4_server_pool_stadicaddress[index])/sizeof(COSA_DML_DHCPS_SADDR); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_stadicaddress[index].InstanceNumber )
        {
            AnscCopyMemory(&g_dhcpv4_server_pool_stadicaddress[index], pEntry, sizeof(COSA_DML_DHCPS_SADDR));
            return ANSC_STATUS_SUCCESS;
        }
    }
#endif
    
    return ANSC_STATUS_SUCCESS;
}



#define PARTNER_ID_LEN 64
void FillParamUpdateSource(cJSON *partnerObj, char *key, char *paramUpdateSource)
{
    errno_t                         rc              = -1;
    cJSON *paramObj = cJSON_GetObjectItem( partnerObj, key);
    if ( paramObj != NULL )
    {
        char *valuestr = NULL;
        cJSON *paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
        if (paramObjVal)
            valuestr = paramObjVal->valuestring;
        if (valuestr != NULL)
        {
            rc = STRCPY_S_NOCLOBBER(paramUpdateSource, 16, valuestr);    // Here paramUpdateSource size 16 is getting from calling function
            ERR_CHK(rc);
            valuestr = NULL;
        }
        else
        {
            CcspTraceWarning(("%s - %s UpdateSource is NULL\n", __FUNCTION__, key ));
        }
    }
    else
    {
        CcspTraceWarning(("%s - %s Object is NULL\n", __FUNCTION__, key ));
    }
}

void FillPartnerIDJournal
    (
        cJSON *json ,
        char *partnerID ,
        PCOSA_DML_DHCPS_POOL_CFG  pPoolCfg
    )
{
                cJSON *partnerObj = cJSON_GetObjectItem( json, partnerID );
                if( partnerObj != NULL)
                {
                      FillParamUpdateSource(partnerObj, "Device.DHCPv4.Server.Pool.1.MinAddress", pPoolCfg->MinAddressUpdateSource);
                      FillParamUpdateSource(partnerObj, "Device.DHCPv4.Server.Pool.1.MaxAddress", pPoolCfg->MaxAddressUpdateSource);
                }
                else
                {
                      CcspTraceWarning(("%s - PARTNER ID OBJECT Value is NULL\n", __FUNCTION__ ));
                }
}

//Get the UpdateSource info from /opt/secure/bootstrap.json. This is needed to know for override precedence rules in set handlers
ANSC_STATUS
CosaDhcpInitJournal
    (
        PCOSA_DML_DHCPS_POOL_CFG  pPoolCfg
    )
{
        char *data = NULL;
        cJSON *json = NULL;
        FILE *fileRead = NULL;
        char PartnerID[PARTNER_ID_LEN] = {0};
        ULONG size = PARTNER_ID_LEN - 1;
        int len;
        errno_t rc = -1;
        if (!pPoolCfg)
        {
                CcspTraceWarning(("%s-%d : NULL param\n" , __FUNCTION__, __LINE__ ));
                return ANSC_STATUS_FAILURE;
        }

        if (access(BOOTSTRAP_INFO_FILE, F_OK) != 0)
        {
                return ANSC_STATUS_FAILURE;
        }

         fileRead = fopen( BOOTSTRAP_INFO_FILE, "r" );
         if( fileRead == NULL )
         {
                 CcspTraceWarning(("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ ));
                 return ANSC_STATUS_FAILURE;
         }

         fseek( fileRead, 0, SEEK_END );
         len = ftell( fileRead );
         /* CID: 62675 Argument cannot be negative*/
         if (len < 0) {
              CcspTraceWarning(("%s-%d : Error in file handle\n" , __FUNCTION__, __LINE__ ));
              fclose(fileRead);
              return ANSC_STATUS_FAILURE;
         }
         fseek( fileRead, 0, SEEK_SET );
         data = ( char* )malloc( sizeof(char) * (len + 1) );
         if (data != NULL)
         {
                memset( data, 0, ( sizeof(char) * (len + 1) ));
                /* CID 60789 fix*/
                size_t bytes_read = fread( data, 1, len, fileRead );
                if (bytes_read == 0)
                {
                        CcspTraceWarning(("%s-%d : Error in reading JSON file\n" , __FUNCTION__, __LINE__ ));
                }
         }
         else
         {
                 CcspTraceWarning(("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__));
                 fclose( fileRead );
                 return ANSC_STATUS_FAILURE;
         }

         fclose( fileRead );
         /*CID: 135443 String not null terminated*/
         data[len] = '\0';
         if ( data == NULL )
         {
                CcspTraceWarning(("%s-%d : fileRead failed \n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
         }
         else if ( strlen(data) != 0)
         {
                 json = cJSON_Parse( data );
                 if( !json )
                 {
                         CcspTraceWarning((  "%s : json file parser error : [%d]\n", __FUNCTION__,__LINE__));
                         free(data);
                         return ANSC_STATUS_FAILURE;
                 }
                 else
                 {
                         if(ANSC_STATUS_SUCCESS == fillCurrentPartnerId(PartnerID, &size))
                         {
                                if ( PartnerID[0] != '\0' )
                                {
                                        CcspTraceWarning(("%s : Partner = %s \n", __FUNCTION__, PartnerID));
                                        FillPartnerIDJournal(json, PartnerID, pPoolCfg);
                                }
                                else
                                {
                                        CcspTraceWarning(( "Reading Deafult PartnerID Values \n" ));
                                        rc = strcpy_s(PartnerID, sizeof(PartnerID), "comcast");
                                        if(rc != EOK)
                                        {
                                            ERR_CHK(rc);
                                            cJSON_Delete(json);
                                            free(data);
                                            return ANSC_STATUS_FAILURE;
                                        }
                                        FillPartnerIDJournal(json, PartnerID, pPoolCfg);
                                }
                        }
                        else{
                                CcspTraceWarning(("Failed to get Partner ID\n"));
                        }
                        cJSON_Delete(json);
                }
                free(data);
                data = NULL;
         }
         else
         {
                CcspTraceWarning(("BOOTSTRAP_INFO_FILE %s is empty\n", BOOTSTRAP_INFO_FILE));
                /*CID: 71605 Resource leak*/
                if(data)
                   free(data);
                return ANSC_STATUS_FAILURE;
         }
         return ANSC_STATUS_SUCCESS;
}

#endif


