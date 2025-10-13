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

    module: cosa_deviceinfo_api_custom.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for custom DeviceInfo Data
        Models

    -------------------------------------------------------------------

    environment:

        platform dependent

    -------------------------------------------------------------------

    author:

        Ding Hua

    -------------------------------------------------------------------

    revision:

        07/07/2014    initial revision.

**************************************************************************/

#include "cosa_deviceinfo_apis.h"
#include "cosa_deviceinfo_apis_custom.h"
#include "dml_tr181_custom_cfg.h" 

#include "ccsp_psm_helper.h"            // for PSM_Get_Record_Value2
#include "dmsb_tr181_psm_definitions.h" // for DMSB_TR181_PSM_DeviceInfo_Root/ProductClass
#include "platform_hal.h"
#include "secure_wrapper.h"
#include "safec_lib_common.h"

#include "cosa_x_comcast_com_gre_apis.h"
#include <utctx/utctx.h>
#include <utctx/utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <pthread.h>
#include <syscfg/syscfg.h>
#include "cosa_drg_common.h"

#define _ERROR_ "NOT SUPPORTED"

#define CAPTIVEPORTAL_ENABLE     "CaptivePortal_Enable"
#define WEB_CONF_ENABLE         "eRT.com.cisco.spvtg.ccsp.webpa.WebConfigRfcEnable"
#define WEB_CONF_FILE           "/nvram/hotspot_blob"

extern void* g_pDslhDmlAgent;

#ifndef NO_WIFI_FEATURE_SUPPORT
static int
PsmGet(const char *param, char *value, int size)
{
    char *val = NULL;

    if (PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, NULL, &val) != CCSP_SUCCESS)
        return -1;
    
    if(val) {
        snprintf(value, size, "%s", val);
        ((CCSP_MESSAGE_BUS_INFO *)g_MessageBusHandle)->freefunc(val);
    }
    else return -1;

    return 0;
}
#endif

#ifdef CONFIG_VENDOR_CUSTOMER_COMCAST

ANSC_STATUS
COSAGetParamValueByPathName
    (
        void*                      bus_handle,
        parameterValStruct_t       *val,
        ULONG                      *parameterValueLength
    );

extern ANSC_HANDLE bus_handle;

static ANSC_STATUS
Local_CosaDmlGetParamValueByPathName
    (
        const char *pathName,
        char *pValue,
        PULONG pulSize
    )
{

    ANSC_STATUS retval = ANSC_STATUS_FAILURE;
    parameterValStruct_t varStruct;
    char outdata[80] = {0};
    ULONG size;
    errno_t rc = -1;

    varStruct.parameterName = (char*)pathName;
    varStruct.parameterValue = outdata;

    size = sizeof(outdata) - 1;
    retval = COSAGetParamValueByPathName(bus_handle, &varStruct, &size);
    if ( retval != ANSC_STATUS_SUCCESS) 
    {
        return ANSC_STATUS_FAILURE;
    }
    else 
    {
        rc = STRCPY_S_NOCLOBBER(pValue, *pulSize, outdata);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
        }
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }
}

/*X_COMCAST-COM_CM_MAC*/
ANSC_STATUS
CosaDmlDiGetCMMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
#ifndef _ENABLE_EPON_SUPPORT_
#ifdef _WNXL11BWL_PRODUCT_REQ_
        s_get_interface_mac("eth0", pValue, 18);
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
#else
	return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_CableModem.MACAddress", pValue, pulSize);
#endif
#else
	return Local_CosaDmlGetParamValueByPathName("Device.DPoE.Mac_address",pValue, pulSize);
#endif      
}
/*X_COMCAST-COM_EWAN_CMMAC*/
ANSC_STATUS
CosaDmlDiGetEwanCMMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
     platform_hal_GetBaseMacAddress(pValue);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

/*X_COMCAST-COM_WAN_MAC*/
ANSC_STATUS
CosaDmlDiGetRouterMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
#ifdef FEATURE_RDKB_XDSL_PPP_MANAGER
    char wanPhyName[32] = {0};
    char out_value[32] = {0};

    if (syscfg_get(NULL, "wan_physical_ifname", out_value, sizeof(out_value)) == 0)
    {
       strncpy(wanPhyName, out_value, sizeof(wanPhyName));
    }
    else
    {
       strncpy(wanPhyName, "erouter0", sizeof(wanPhyName));
       CcspTraceInfo(("%s %d - WanPhyName=%s \n", __FUNCTION__,__LINE__, wanPhyName));
    }
    s_get_interface_mac(wanPhyName, pValue, 18);

    //"wan_physical_ifname" is "erouter0" for all platforms irrespective of connection types(vdsl/adsl/wanoe)
    // for the ppp enabled platforms, if erouter0 has no mac, do an additional check on atm0
    if(strncmp(pValue, "00:00:00:00:00:00", strlen("00:00:00:00:00:00")) == 0)
    {
        // this will call ioctl to get mac if interface is present
        s_get_interface_mac("atm0", pValue, 18);
        CcspTraceInfo(("%s %d - WAN mac on atm0=%s \n", __FUNCTION__,__LINE__, pValue));

    }
#else	
#ifdef _WNXL11BWL_PRODUCT_REQ_
        s_get_interface_mac("eth0", pValue, 18);
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
#else
    s_get_interface_mac("erouter0", pValue, 18);
#endif /* _WNXL11BWL_PRODUCT_REQ_ */
#endif    
    *pulSize = AnscSizeOfString(pValue);

    return ANSC_STATUS_SUCCESS;
}

#if !defined (NO_MTA_FEATURE_SUPPORT)
/*X_COMCAST-COM_MTA_MAC*/
ANSC_STATUS
CosaDmlDiGetMTAMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if !defined (_HUB4_PRODUCT_REQ_)
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_MTA.MACAddress", pValue, pulSize);
#else
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pulSize);
    return ANSC_STATUS_FAILURE;
#endif
}
#endif

/*X_COMCAST-COM_WAN_IP*/
ANSC_STATUS
CosaDmlDiGetRouterIPAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined(_WNXL11BWL_PRODUCT_REQ_) || defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
    char wan_interface[32] = {0};
    commonSyseventGet("current_wan_ifname", wan_interface, sizeof(wan_interface));
#else
    char *wan_interface = "erouter0";
#endif
    uint32_t ip = CosaUtilGetIfAddr(wan_interface);
    unsigned char *a = (unsigned char *) &ip;

    /*
       The value returned by CosaUtilGetIfAddr() is in network byte order
       (ie it's always big endian). Processing as bytes allows this code to
       be agnostic of target endianness.
    */
    *pulSize = sprintf (pValue, "%d.%d.%d.%d", a[0], a[1], a[2], a[3]);

    return ANSC_STATUS_SUCCESS;
}

/*X_COMCAST-COM_WAN_IPv6*/
ANSC_STATUS
CosaDmlDiGetRouterIPv6Address
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pulSize);
	ipv6_addr_info_t * p_v6addr = NULL;
    int  v6addr_num = 0, i, l_iIpV6AddrLen;

#if defined(_RDKB_GLOBAL_PRODUCT_REQ_)
    char            acLANIPv6GUASupport[8] = {0};
    unsigned char   IsLANIPv6GUASupported  = FALSE;

    commonSyseventGet("LANIPv6GUASupport", acLANIPv6GUASupport, sizeof(acLANIPv6GUASupport));
    if( 0 == strncmp( acLANIPv6GUASupport, "true", strlen( acLANIPv6GUASupport ) ) )
    {   
        IsLANIPv6GUASupported = TRUE;
    }

    if( TRUE == IsLANIPv6GUASupported )
    {
        CosaUtilGetIpv6AddrInfo("brlan0", &p_v6addr, &v6addr_num);
    }
    else
    {
        CosaUtilGetIpv6AddrInfo("erouter0", &p_v6addr, &v6addr_num);
    }
#elif defined(_HUB4_PRODUCT_REQ_)
	CosaUtilGetIpv6AddrInfo("brlan0", &p_v6addr, &v6addr_num);
#elif defined(_WNXL11BWL_PRODUCT_REQ_)
        char wan_interface[32] = {0};
        commonSyseventGet("current_wan_ifname", wan_interface, sizeof(wan_interface));
        CosaUtilGetIpv6AddrInfo(wan_interface, &p_v6addr, &v6addr_num);
#else
        CosaUtilGetIpv6AddrInfo("erouter0", &p_v6addr, &v6addr_num);
#endif
    for(i = 0; i < v6addr_num; i++ )
    {
#if defined(_RDKB_GLOBAL_PRODUCT_REQ_)
        if ( p_v6addr[i].scope == IPV6_ADDR_SCOPE_GLOBAL ) 
        {
            if ( TRUE == IsLANIPv6GUASupported )
            {
                if ( (strncmp(p_v6addr[i].v6addr, "fd", 2) != 0) && (strncmp(p_v6addr[i].v6addr, "fc", 2) != 0) )
                {
                    l_iIpV6AddrLen = strlen(p_v6addr[i].v6addr);
                    strncpy(pValue, p_v6addr[i].v6addr, l_iIpV6AddrLen);
                    pValue[l_iIpV6AddrLen] = '\0';
                }
            }
            else
            {
                l_iIpV6AddrLen = strlen(p_v6addr[i].v6addr);
                strncpy(pValue, p_v6addr[i].v6addr, l_iIpV6AddrLen);
                pValue[l_iIpV6AddrLen] = '\0';
            }

        }
#else
#if defined(_HUB4_PRODUCT_REQ_)
        if((p_v6addr[i].scope == IPV6_ADDR_SCOPE_GLOBAL) && (strncmp(p_v6addr[i].v6addr, "fd", 2) != 0) && (strncmp(p_v6addr[i].v6addr, "fc", 2) != 0))
#else
        if(p_v6addr[i].scope == IPV6_ADDR_SCOPE_GLOBAL)
#endif
        {
			l_iIpV6AddrLen = strlen(p_v6addr[i].v6addr);
			strncpy(pValue, p_v6addr[i].v6addr, l_iIpV6AddrLen);
			pValue[l_iIpV6AddrLen] = '\0';
        }
#endif /** _RDKB_GLOBAL_PRODUCT_REQ_ */
    }
	if(p_v6addr)
        free(p_v6addr);

    return ANSC_STATUS_SUCCESS;
}

#if !defined (NO_MTA_FEATURE_SUPPORT)
/*X_COMCAST-COM_MTA_IP*/
ANSC_STATUS
CosaDmlDiGetMTAIPAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if !defined (_HUB4_PRODUCT_REQ_)
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_MTA.IPAddress", pValue, pulSize);
#else
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pulSize);
    return ANSC_STATUS_FAILURE;
#endif
}

/*X_COMCAST-COM_MTA_IPV6*/
ANSC_STATUS
CosaDmlDiGetMTAIPV6Address
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_MTA_V6.IPV6Address", pValue, pulSize);
}
#endif

/*X_COMCAST-COM_CM_IP*/
ANSC_STATUS
CosaDmlDiGetCMIPAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
#ifndef _ENABLE_EPON_SUPPORT_
    ANSC_STATUS retStatus;
    ULONG  pulSizeCopy = 0;
    pulSizeCopy = *pulSize;
    retStatus = Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_CableModem.IPv6Address", pValue, pulSize);
    if(!(*pulSize))
    {
        *pulSize = pulSizeCopy;
        return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_CableModem.IPAddress", pValue, pulSize);
    }
    return retStatus;
#else
    if ( pValue )
    {
        errno_t rc = -1;
        rc = strcpy_s(pValue, *pulSize, "0.0.0.0");
        ERR_CHK(rc);
    }
    UNREFERENCED_PARAMETER(pulSize);
    return ANSC_STATUS_SUCCESS;
#endif
}

#endif


#ifdef CONFIG_CISCO_HOTSPOT

ANSC_STATUS
CosaDmlDiGetXfinityWiFiCapable
    (
        BOOL *pValue
    )
{
    *pValue = TRUE;
    return ANSC_STATUS_SUCCESS;
}

#define HOTSPOT_PSM_EANBLE     "dmsb.hotspot.enable"

static int
PsmSet(const char *param, const char *value)
{
    if (PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, ccsp_string, (char *)value) != CCSP_SUCCESS)
        return -1;
    return 0;
}

static int 
DmSetBool(const char *param, BOOL value)
{
    parameterValStruct_t val[1];
    char crname[256], *fault = NULL;
    int err;

    val[0].parameterName  = (char*)param;
    val[0].parameterValue = (value ? "true" : "false");
    val[0].type           = ccsp_boolean;

    snprintf(crname, sizeof(crname), "%s%s", g_GetSubsystemPrefix(g_pDslhDmlAgent), CCSP_DBUS_INTERFACE_CR);

    if ((err = CcspBaseIf_SetRemoteParameterValue(g_MessageBusHandle, 
                crname, param, g_GetSubsystemPrefix(g_pDslhDmlAgent), 0, 0xFFFF, val, 1, 1, &fault)) != CCSP_SUCCESS)

    if (fault)
        AnscFreeMemory(fault);

    return (err == CCSP_SUCCESS) ? 0 : -1;
}

ANSC_STATUS
CosaDmlDiGetXfinityWiFiEnable
    (
        BOOL *pValue
    )
{
    char val[64];

    if (PsmGet(HOTSPOT_PSM_EANBLE, val, sizeof(val)) != 0)
        return ANSC_STATUS_FAILURE;

    *pValue = (atoi(val) == 1) ? TRUE : FALSE;
    return ANSC_STATUS_SUCCESS;
}


void *XfinityWifiThread
    (
        void *arg
    )
{
    BOOL *pvalue = (BOOL*)arg;
    BOOL value = FALSE;
    
    if (!pvalue)
        return NULL;
    value = *pvalue;
    if (FALSE == value)
    {
        PsmSet(HOTSPOT_PSM_EANBLE, "0");
        //SSIDs 5 and 6 case
        if (DmSetBool("Device.WiFi.SSID.5.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.5 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.5 Disable OK\n", __FUNCTION__);
        }

        if (DmSetBool("Device.WiFi.SSID.6.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.6 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.6 Disable OK\n", __FUNCTION__);
        }
    	
        //SSIDs 9 and 10 case
        if (DmSetBool("Device.WiFi.SSID.9.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.9 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.9 Disable OK\n", __FUNCTION__);
        }
	
        if (DmSetBool("Device.WiFi.SSID.10.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.10 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.10 Disable OK\n", __FUNCTION__);
        }

        
        if (DmSetBool("Device.WiFi.SSID.16.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.16 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.16 Disable OK\n", __FUNCTION__);
        }
#if defined (_XB8_PRODUCT_REQ_) && defined(RDK_ONEWIFI)
        if (DmSetBool("Device.WiFi.SSID.19.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.19 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.19 Disable OK\n", __FUNCTION__);
        }

        if (DmSetBool("Device.WiFi.SSID.21.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.21 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.21 Disable OK\n", __FUNCTION__);
        }
#endif
#if defined (_PLATFORM_RASPBERRYPI_) && defined(RDK_ONEWIFI)
        if (DmSetBool("Device.WiFi.ApplyAccessPointSettings", true) != ANSC_STATUS_SUCCESS) {
            AnscTraceError(("%s: wifi apply settings failed\n", __FUNCTION__));
	}  else {  
            AnscTraceError(("%s: wifi apply settings success\n", __FUNCTION__));
	}    
#endif	
    }
    else
    {
         PsmSet(HOTSPOT_PSM_EANBLE, "1");
    }
        /*Removing webconfig files*/
        v_secure_system("rm /nvram/hotspot.json");

        v_secure_system("rm /nvram/hotspot_blob");
	
	if (g_SetParamValueBool("Device.X_COMCAST-COM_GRE.Tunnel.1.Enable", value) != ANSC_STATUS_SUCCESS) {
		fprintf(stderr, "%s: set X_COMCAST-COM_GRE.Tunnel.1.Enable error\n", __FUNCTION__);
        AnscTraceError(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Enable error\n", __FUNCTION__));
        AnscFreeMemory(pvalue);
		return NULL;
    } else {
        AnscTraceWarning(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Enable OK\n", __FUNCTION__));
    }
	if (g_SetParamValueBool("Device.X_COMCAST-COM_GRE.Tunnel.1.Interface.1.Enable", value) != ANSC_STATUS_SUCCESS) {
        AnscTraceError(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Interface.1.Enable error\n", __FUNCTION__));
    } else {
        AnscTraceWarning(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Interface.1.Enable OK\n", __FUNCTION__));
    }
/* RDKB-44262 leads to redundant gre-forceRestart
        if (g_SetParamValueBool("Device.X_COMCAST-COM_GRE.Tunnel.1.Interface.2.Enable", value) != ANSC_STATUS_SUCCESS) {
        AnscTraceError(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Interface.2.Enable error\n", __FUNCTION__));
    } else {
        AnscTraceWarning(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Interface.2.Enable OK\n", __FUNCTION__));
    }
*/
	//zqiu<<
    if (value) {
         //Update circuit ID here
        hotspot_update_circuit_ids(1,1); 
    }
    
    AnscFreeMemory(pvalue);
    pvalue = NULL;
    return NULL;
}

ANSC_STATUS
CosaDmlDiSetXfinityWiFiEnable
    (
        BOOL value
    )
{
    pthread_t thread_xfinity_wifi = 0;
    BOOL *pValue = NULL;

    FILE *fptr = NULL;
    char val[16] = {0};
    char rec[128] = {0};
 
    memset(rec, '\0', sizeof(rec));
    memset(val, '\0', sizeof(val));
    fptr = fopen(WEB_CONF_FILE, "r");
    snprintf(rec, sizeof(rec), "%s",  WEB_CONF_ENABLE);
    if((PsmGet(rec, val, sizeof(val)) != 0)){
        AnscTraceError(("%s Failed to get psm value for Webconfig \n", __FUNCTION__));
	// CID 175442 : Resource leak (RESOURCE_LEAK)
	if(fptr)
        {
            fclose(fptr);
        }
        return ANSC_STATUS_FAILURE;
    }
    if((strcmp(val, "true") == 0) && (NULL != fptr))
    {
        AnscTraceWarning(("%s: webconfig enabled...legacy hotspot not used... \n", __FUNCTION__));
        fclose(fptr);
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        AnscTraceWarning(("%s: webconfig disabled using legacy \n", __FUNCTION__));
        /*CID: 61742 Wrong sizeof argument*/
        pValue =  AnscAllocateMemory(sizeof(BOOL));
        if (pValue != NULL)
        {
           *pValue = value;
            pthread_create
            (
             &thread_xfinity_wifi,
             NULL,
             XfinityWifiThread,
             (void*)pValue
            );
            if(fptr != NULL)
             fclose(fptr);
            return ANSC_STATUS_SUCCESS;
        }
        else
        {
            if(fptr != NULL)
             fclose(fptr);
            return ANSC_STATUS_FAILURE;
        }
    }
}
#endif

ANSC_STATUS
CosaDmlGetCaptivePortalEnable
    (
        BOOL *pValue
    )
{
	char buf[8];
        /* CID: 58774 Array compared against 0*/
        if(!syscfg_get( NULL, CAPTIVEPORTAL_ENABLE , buf, sizeof(buf)))
	{
    		if (strcmp(buf,"true") == 0)
		{
			CcspTraceWarning(("CaptivePortal: Captive Portal switch is enabled...\n"));		
    		       *pValue = true;
		}
    		else
		{
			CcspTraceWarning(("CaptivePortal: Captive Portal switch is disabled...\n"));		
    		        *pValue = false;	
		}
           return ANSC_STATUS_SUCCESS;

    	} else {
           CcspTraceWarning(("CaptivePortal: syscfg get error\n"));
           return ANSC_STATUS_FAILURE;
        }
}

ANSC_STATUS
CosaDmlSetCaptivePortalEnable
    (
        BOOL value
    )
{
	char *value_string;

	if (value)
	{
		CcspTraceWarning(("CaptivePortal: Enabling Captive Portal switch ...\n"));		
		value_string = "true";
	}
	else
	{
		CcspTraceWarning(("CaptivePortal: Disabling Captive Portal switch ...\n"));		
		value_string = "false";
	}

	if (syscfg_set_commit(NULL, CAPTIVEPORTAL_ENABLE, value_string) != 0) {
		CcspTraceWarning(("syscfg_set failed to enable/disable captive portal\n"));
		return ANSC_STATUS_FAILURE;
	}

	v_secure_system("sh /etc/restart_services.sh %s", value_string);
    /*commonSyseventSet("dhcp-server-restart", "");
    commonSyseventSet("firewall-restart", "");
    commonSyseventSet("zebra-restart", ""); */

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
	CosaDmlDiGetCloudCapable
	(
     	BOOL *pValue
	)
{
	int rc;
    char buf[5];

	memset(buf, 0, sizeof(buf));
	rc = syscfg_get( NULL, "cloud_capable_flag", buf, sizeof(buf));
    if( rc == 0 )
    {
        if (strcmp(buf,"1") == 0)
            *pValue = TRUE;
        else
    		*pValue = FALSE;
    }
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlGetCloudUIReachableStatus
    (
		CHAR *pCloudPersonalizationURL,
        BOOL *pValue
    )
{
	ANSC_STATUS returnStatus   = ANSC_STATUS_SUCCESS;
	BOOL 		bProcessFuther = TRUE;
	/* Received Param Validation */
	if( NULL == pCloudPersonalizationURL )
	{
		*pValue        = FALSE;
		returnStatus   = ANSC_STATUS_FAILURE;
		bProcessFuther = FALSE;		
	}

	if( bProcessFuther )
	{
		FILE  *fp;
                char   retBuff[256]   = { 0 };
		int    curlResponse   = 0;
                
                fp = v_secure_popen( "r", "curl --connect-timeout 10 --interface erouter0 --write-out %%{http_code} --silent --output /dev/null '%s'", pCloudPersonalizationURL );

		if( NULL != fp )
		{
			fgets( retBuff, sizeof(retBuff), fp );
                        v_secure_pclose(fp);
			curlResponse = atoi(retBuff);

			CcspTraceInfo(("URL[ %s ] curlResponse[ %d ]\n",
									pCloudPersonalizationURL,
									curlResponse));
		
			if( ( curlResponse >= 200 ) && \
				( curlResponse < 400 ) 
			  )
			{
				*pValue = TRUE;
			}
			else
			{
				*pValue = FALSE;
			}
		}
		else
		{
			*pValue = FALSE;
		}
	}
	
    return returnStatus;
}

#if defined(INTEL_PUMA7) || defined(_XB6_PRODUCT_REQ_) || defined(_CBR2_PRODUCT_REQ_)
ANSC_STATUS
CosaDmlSetLED
    (
    	int color,
    	int state,
    	int interval
    )
{
    LEDMGMT_PARAMS ledMgmt;
    memset(&ledMgmt, 0, sizeof(LEDMGMT_PARAMS));

	ledMgmt.LedColor = color;
	ledMgmt.State	 = state;
	ledMgmt.Interval = interval;

	if(RETURN_ERR == platform_hal_setLed(&ledMgmt)) {
		CcspTraceWarning(("platform_hal_setLed failed\n"));
		return ANSC_STATUS_FAILURE;
	}

    return ANSC_STATUS_SUCCESS;	
}
#endif

