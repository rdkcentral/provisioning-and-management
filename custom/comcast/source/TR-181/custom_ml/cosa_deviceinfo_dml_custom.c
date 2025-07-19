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

    module: cosa_deviceinfo_apis_custom.h

    For Data Model Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        The prototypes of custom data model APIs are defined here

    ---------------------------------------------------------------

    environment:

        Maybe platform independent, but customer specific

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        05/08/2014  initial revision.

**************************************************************************/

#include "dml_tr181_custom_cfg.h" 
#include "cosa_deviceinfo_dml.h"
#include "cosa_deviceinfo_apis_custom.h"
#include <syscfg/syscfg.h>
#include "safec_lib_common.h"

#define WHITE	0
#define SOLID	0
#define BLINK	1

#if defined (FEATURE_RDKB_LED_MANAGER_CAPTIVE_PORTAL)
#include <sysevent/sysevent.h>
#define SYSEVENT_LED_STATE    "led_event"
#define IPV4_UP_EVENT         "rdkb_ipv4_up"
#define IPV4_DOWN_EVENT       "rdkb_ipv4_down"
#define LIMITED_OPERATIONAL   "rdkb_limited_operational"
#define WAN_LINK_UP          "rdkb_wan_link_up"
#endif



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamBoolValue_Custom
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_GetParamBoolValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

#ifdef CONFIG_INTERNET2P0
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_ConfigureWiFi") == 0)
    {
       *pBool = pMyObject->bWiFiConfigued;
	return TRUE;
    }
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_WiFiNeedsPersonalization") == 0)
    {
	    char buf[5];
        /*CID: 63071 Array compared against 0*/
        if(!syscfg_get( NULL, "redirection_flag", buf, sizeof(buf)))
    	{
    		if (strcmp(buf,"true") == 0)
    		        *pBool = TRUE;
    		    else
    		        *pBool = FALSE;
    	} else {
            return FALSE;
        }
	    return TRUE;
    }
	    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CaptivePortalEnable") == 0)
    {
 //      *pBool = pMyObject->bCaptivePortalEnable;
        if (CosaDmlGetCaptivePortalEnable(&pMyObject->bCaptivePortalEnable) != ANSC_STATUS_SUCCESS)
            return FALSE;
       *pBool = pMyObject->bCaptivePortalEnable;
	return TRUE;
    }
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUICapable") == 0)
    {

	    *pBool = pMyObject->bCloudCapable;
	     return TRUE;

    }
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUIEnable") == 0)
    {
       *pBool = pMyObject->bCloudEnable;
	char buf[5];
        /*CID: 63071 Array compared against 0*/
        if(!syscfg_get( NULL, "cloud_enable_flag", buf, sizeof(buf)))
    	{
            if (strcmp(buf,"1") == 0)
    		 pMyObject->bCloudEnable = TRUE;
            else
    		 pMyObject->bCloudEnable = FALSE;
    	}else
            return FALSE;

	*pBool = pMyObject->bCloudEnable;
	return TRUE;
    }
#endif

#ifdef CONFIG_CISCO_HOTSPOT
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "X_COMCAST-COM_xfinitywifiCapableCPE") == 0)
    {
        if (CosaDmlDiGetXfinityWiFiCapable(pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (strcmp(ParamName, "X_COMCAST_COM_xfinitywifiEnable") == 0)
    {
//        if (CosaDmlDiGetXfinityWiFiEnable(pBool) != ANSC_STATUS_SUCCESS)
//            return FALSE;
			// printf("%s : bxfinitywifiEnable value is : %d\n",__FUNCTION__,pMyObject->bxfinitywifiEnable);
			*pBool = pMyObject->bxfinitywifiEnable;
        return TRUE;
    }
#endif

    if (strcmp(ParamName, "X_COMCAST-COM_rdkbPlatformCapable") == 0)
    {
       *pBool = TRUE;
	    return TRUE;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_IsCloudReachable") == 0)
    {
	CcspTraceWarning(("IsCloudReachable: %s \n",pMyObject->CloudPersonalizationURL));

	*pBool = FALSE;
	/* Get the CloudURL reachable status */
	if ( ANSC_STATUS_SUCCESS != CosaDmlGetCloudUIReachableStatus( pMyObject->CloudPersonalizationURL,
			  pBool ) )
	{
            *pBool = FALSE;
            return FALSE;
	}

	return TRUE;
    }
#if !defined (NO_MOCA_FEATURE_SUPPORT)
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_EnableMoCAforXi5") == 0)
    {
       *pBool = pMyObject->bEnableMoCAforXi5;
           return TRUE;
    }
#endif
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DeviceInfo_GetParamStringValue_Custom
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
DeviceInfo_GetParamStringValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#ifndef MODEM_ONLY_SUPPORT
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
#endif
    char isEthEnabled[64]={'\0'};
    int EthWANEnable = 0;
#ifdef CONFIG_INTERNET2P0
    errno_t rc = -1;
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUIWebURL") == 0)
    { 
        syscfg_get(NULL, "redirection_url", pMyObject->WebURL, sizeof(pMyObject->WebURL));
        rc = strcpy_s(pValue, *pulSize, pMyObject->WebURL);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    } 
#endif
        	if( 0 == syscfg_get( NULL, "eth_wan_enabled", isEthEnabled, sizeof(isEthEnabled)) && (isEthEnabled[0] != '\0' && strcmp(isEthEnabled, "true") == 0))
        	{
			EthWANEnable = 1;
		}
			
#ifdef CONFIG_VENDOR_CUSTOMER_COMCAST
	if (strcmp(ParamName, "X_COMCAST-COM_CM_MAC") == 0)
	{
	        if( EthWANEnable )
        	{
#if defined (_HUB4_PRODUCT_REQ_)
		   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);
#else
                   CosaDmlDiGetEwanCMMacAddress(NULL, pValue,pulSize);
#endif /* _HUB4_PRODUCT_REQ_ */
        	}
		else
		{
		   CosaDmlDiGetCMMacAddress(NULL, pValue,pulSize);
		}
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_WAN_MAC") == 0)
	{
	   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_AP_MAC") == 0)
	{
	   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_MTA_MAC") == 0)
	{
#if !defined(_PLATFORM_RASPBERRYPI_) && !defined(_PLATFORM_TURRIS_) && !defined(_PLATFORM_BANANAPI_R4_)
  	   CosaDmlDiGetMTAMacAddress(NULL, pValue,pulSize);
#endif
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_CM_IP") == 0)
	{
        	if( EthWANEnable )
        	{
			CosaDmlDiGetRouterIPv6Address(NULL, pValue,pulSize);
				
			if(strlen(pValue) == 0)
				CosaDmlDiGetRouterIPAddress(NULL, pValue,pulSize);

            		
        	}
		else
		{
	   		CosaDmlDiGetCMIPAddress(NULL, pValue,pulSize);
		}
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_WAN_IP") == 0)
	{
	   CosaDmlDiGetRouterIPAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_WAN_IPv6") == 0)
	{
	   CosaDmlDiGetRouterIPv6Address(NULL, pValue,pulSize);
           #if defined(_COSA_FOR_BCI_)
           if (syscfg_set_commit(NULL, "wanIPv6Address",pValue) != 0) {
               AnscTraceWarning(("syscfg_set failed\n"));
           }
           #endif
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_MTA_IP") == 0)
	{
#if !defined(_PLATFORM_RASPBERRYPI_) && !defined(_PLATFORM_TURRIS_) && !defined(_PLATFORM_BANANAPI_R4_)
   	   CosaDmlDiGetMTAIPAddress(NULL, pValue,pulSize);
#endif
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_MTA_IPV6") == 0)
	{
#if !defined(_PLATFORM_RASPBERRYPI_) && !defined(_PLATFORM_BANANAPI_R4_)
   	   CosaDmlDiGetMTAIPV6Address(NULL, pValue,pulSize);
#endif
	   return 0;
	}
#endif

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamBoolValue_Custom
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
DeviceInfo_SetParamBoolValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

#if !defined (CONFIG_INTERNET2P0) && defined (NO_MOCA_FEATURE_SUPPORT) && !defined (CONFIG_CISCO_HOTSPOT)
UNREFERENCED_PARAMETER(ParamName);
UNREFERENCED_PARAMETER(bValue);
#else
PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
#endif

#ifdef CONFIG_INTERNET2P0

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_ConfigureWiFi") == 0)
    {
#if defined (FEATURE_RDKB_LED_MANAGER_CAPTIVE_PORTAL)
	int sysevent_led_fd = -1;
	token_t sysevent_led_token;
	char cmd[126] = {0};
	char buf[126] = {0};
	char ledStatus[126] = {0};
	FILE *fp = NULL;
	sysevent_led_fd = sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "OperationalStateHandler", &sysevent_led_token);
#endif
	char val[8];
	if(!syscfg_get( NULL, "WiFiPersonalizationSupport" , val, sizeof(val)))
	{
		if (strcmp(val, "false") == 0)
		{
			CcspTraceInfo(("WiFiPersonalization is Disabled. Setting ConfigureWiFi to FALSE\n"));
			bValue = FALSE;
		}
	}
	if ( bValue == TRUE )
	{

		CcspTraceWarning(("CaptivePortal:Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n"));
		CcspTraceWarning(("RDKB_GW_MODE:Setting RDKB GW to CaptivePortal  ...\n"));
		printf("Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n");
		pMyObject->bWiFiConfigued = bValue;

		char buf[8];
                    /* CID: 92407 Array compared against 0*/
		if(!syscfg_get( NULL, "CaptivePortal_Enable" , buf, sizeof(buf))) {
		    if (strcmp(buf,"true") == 0)
		    {
#if defined(INTEL_PUMA7) || defined(_XB6_PRODUCT_REQ_)
			if ( ANSC_STATUS_SUCCESS == CosaDmlSetLED(WHITE, BLINK, 1) )
				CcspTraceInfo(("Front LED Transition: WHITE LED will blink, Reason: CaptivePortal_MODE\n"));
#endif
#if defined (FEATURE_RDKB_LED_MANAGER_CAPTIVE_PORTAL)
		snprintf(cmd, sizeof(cmd), "sysevent get wan-status");
                if (((fp = popen(cmd,"r")) != NULL) && (fgets(buf, sizeof(buf), fp)))
		{
			if (strncmp(buf, "started", strlen("started")) == 0)
			{
				if(sysevent_led_fd != -1)
				{
					snprintf(cmd, sizeof(cmd), "sysevent get led_event");
					if (((fp = popen(cmd,"r")) != NULL) && (fgets(ledStatus, sizeof(ledStatus), fp))) 
					{
						CcspTraceInfo (("[%s][%d] ledStatus is %s\n", __FUNCTION__,__LINE__, ledStatus));
						if(strncmp(ledStatus, "rdkb_wan_link_up", strlen("rdkb_wan_link_up")) == 0)
						{
							sysevent_set(sysevent_led_fd, sysevent_led_token, SYSEVENT_LED_STATE,WAN_LINK_UP, 0);
							CcspTraceInfo (("[%s][%d] Sent WAN_LINK_UP Event to RdkLedManager; reason wan registration\n", __FUNCTION__,__LINE__));
						}
						else
						{
							sysevent_set(sysevent_led_fd, sysevent_led_token, SYSEVENT_LED_STATE, LIMITED_OPERATIONAL, 0);
							CcspTraceInfo (("[%s][%d] Successfully sent LIMITED_OPERATIONAL event to RdkledManager\n", __FUNCTION__,__LINE__));
						}

					} 
				}
			}
			else
			{
				if(sysevent_led_fd != -1)
				{
					snprintf(cmd, sizeof(cmd), "sysevent get led_event");
					if (((fp = popen(cmd,"r")) != NULL) && (fgets(ledStatus, sizeof(ledStatus), fp)))
					{
						CcspTraceInfo (("[%s][%d] ledStatus is %s\n", __FUNCTION__,__LINE__, ledStatus));
						if(strncmp(ledStatus, "rdkb_ipv4_down", strlen("rdkb_ipv4_down")) == 0)
						{
							sysevent_set(sysevent_led_fd, sysevent_led_token, SYSEVENT_LED_STATE, IPV4_DOWN_EVENT, 0);
								CcspTraceInfo (("[%s][%d] Successfully sent IPV4_DOWN_EVENT to RdkledManager\n", __FUNCTION__,__LINE__));
                                                }
					}
				}
                          }
                    }
                    if(fp)
                           pclose(fp);
#endif
			    //if CaptivePortal_Enable is true, Then only we need to run redirect_url.sh
			    printf("%s calling redirect_url.sh script to start redirection\n",__FUNCTION__);
			    system("source /etc/redirect_url.sh &");
		    }
#if defined(INTEL_PUMA7) || defined(_XB6_PRODUCT_REQ_) 
		    else
		    {
			if ( ANSC_STATUS_SUCCESS == CosaDmlSetLED(WHITE, SOLID, 0) )
				CcspTraceInfo(("Front LED Transition: WHITE LED will be SOLID, Reason: ConfigureWiFi is TRUE, but CaptivePortal is disabled\n"));
#if defined (FEATURE_RDKB_LED_MANAGER_CAPTIVE_PORTAL)
		snprintf(cmd, sizeof(cmd), "sysevent get wan-status");
		if (((fp = popen(cmd,"r")) != NULL) && (fgets(buf, sizeof(buf), fp)))
		{
                CcspTraceInfo (("[%s][%d] buf value is %s\n", __FUNCTION__,__LINE__, buf));
		if (strncmp(buf, "started", strlen("started")) == 0)
		{
			if(sysevent_led_fd != -1)
			{
		            sysevent_set(sysevent_led_fd, sysevent_led_token, SYSEVENT_LED_STATE, IPV4_UP_EVENT, 0);
			    CcspTraceInfo (("[%s][%d] Successfully sent IPV4_UP_EVENT to RdkledManager\n", __FUNCTION__,__LINE__));
			}
		    }
		    else
		    {
			if(sysevent_led_fd != -1)
                        {
                                sysevent_set(sysevent_led_fd, sysevent_led_token, SYSEVENT_LED_STATE, IPV4_DOWN_EVENT, 0);
                                CcspTraceInfo (("[%s][%d] Successfully sent IPV4_DOWN_EVENT to RdkledManager\n", __FUNCTION__,__LINE__));
                        }
		
		}
		}
		if(fp)
			pclose(fp);
#endif
		    }
#endif
                } else {
                   return FALSE;
                }

        return TRUE;
	 }
	
         else if  ( bValue == FALSE )
	 {
#if defined(INTEL_PUMA7) || defined(_XB6_PRODUCT_REQ_) || defined(_CBR2_PRODUCT_REQ_)
	    FILE *responsefd=NULL;
	    char *networkResponse = "/var/tmp/networkresponse.txt";
	    int iresCode = 0;
	    char responseCode[10];

            if((responsefd = fopen(networkResponse, "r")) != NULL)
            {
                if(fgets(responseCode, sizeof(responseCode), responsefd) != NULL)
                {
                    iresCode = atoi(responseCode);
                }

                fclose(responsefd);
                responsefd = NULL;
            }
	if(iresCode == 204)
	{	
        	if ( ANSC_STATUS_SUCCESS == CosaDmlSetLED(WHITE, SOLID, 0) )
            		CcspTraceInfo(("Front LED Transition: WHITE LED will be SOLID, Reason: Gateway_MODE\n"));
#if defined (FEATURE_RDKB_LED_MANAGER_CAPTIVE_PORTAL)
	snprintf(cmd, sizeof(cmd), "sysevent get wan-status");
	if (((fp = popen(cmd,"r")) != NULL) && (fgets(buf, sizeof(buf), fp)))
	{
		CcspTraceInfo (("[%s][%d] buf value is %s\n", __FUNCTION__,__LINE__, buf));
		if (strncmp(buf, "started", strlen("started")) == 0)
		{
			if(sysevent_led_fd != -1)
			{
				sysevent_set(sysevent_led_fd, sysevent_led_token, SYSEVENT_LED_STATE, IPV4_UP_EVENT, 0);
				CcspTraceInfo (("[%s][%d] Successfully sent IPV4_UP_EVENT to RdkledManager\n", __FUNCTION__,__LINE__));
			}
		}
		else
		{
			if(sysevent_led_fd != -1)
			{
				
				sysevent_set(sysevent_led_fd, sysevent_led_token, SYSEVENT_LED_STATE, IPV4_DOWN_EVENT, 0);
				CcspTraceInfo (("[%s][%d] Successfully sent IPV4_DOWN_EVENT to RdkledManager\n", __FUNCTION__,__LINE__));
			}
		}
	}
	if(fp)
		pclose(fp);
#endif
	}
#endif

		CcspTraceWarning(("CaptivePortal:Wi-Fi SSID and Passphrase are configured,setting ConfigureWiFi to false ...\n"));
		CcspTraceWarning(("RDKB_GW_MODE:Setting RDKB GW to Online  ...\n"));
		printf("Wi-Fi SSID and Passphrase are configured,setting ConfigureWiFi to false ...\n");

		pMyObject->bWiFiConfigued = bValue;
		printf("%s calling revert_redirect.sh script to remove the redirection changes\n",__FUNCTION__);
		system("source /etc/revert_redirect.sh &");

	    return TRUE;
	 }	
#if defined (FEATURE_RDKB_LED_MANAGER_CAPTIVE_PORTAL)
	 if (sysevent_led_fd != -1)
		 sysevent_close(sysevent_led_fd, sysevent_led_token);
#endif
	return FALSE;
    } 

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CaptivePortalEnable") == 0)
    {
	if( pMyObject->bCaptivePortalEnable == bValue )
	{
		return TRUE;	
	}
#ifdef SKY_FEATURE_SELFHEAL
        if (!bValue)
        {
            CcspTraceWarning(("%s : CAPTIVEPORTAL should be in enable mode for SKY_FEATURE_SELFHEAL\n", __FUNCTION__));
            return FALSE;
        }
#endif
        if (CosaDmlSetCaptivePortalEnable(bValue) != ANSC_STATUS_SUCCESS)
            return FALSE;
     	pMyObject->bCaptivePortalEnable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUICapable") == 0)
    {
       // We should not allow SET of Capable flag.
#if 0 
	  if( bValue == TRUE) {

             if (syscfg_set_commit(NULL, "cloud_capable_flag", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {
		    pMyObject->bCloudCapable = bValue;
             }

         } else {

             if (syscfg_set_commit(NULL, "cloud_capable_flag", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {
		  pMyObject->bCloudCapable = bValue;
             }
         }
#endif
	return TRUE;


    }
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUIEnable") == 0)
    {
        

	if ( pMyObject->bCloudCapable == TRUE )
	{

          if( bValue == TRUE) {

             if (syscfg_set_commit(NULL, "cloud_enable_flag", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {
			pMyObject->bCloudEnable = bValue;
			CcspTraceWarning(("CaptivePortal:Enabling CloudUIEnable to start redirection to Cloud URL ...\n"));
             }

         } else {

             if (syscfg_set_commit(NULL, "cloud_enable_flag", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {
			pMyObject->bCloudEnable = bValue;
			CcspTraceWarning(("CaptivePortal:Disabling CloudUIEnable to stop redirection to Cloud URL ...\n"));
             }
         }
	}
	else
	{
		printf("First enable cloud capable to modify this parameter\n");
		return FALSE;
	}
	return TRUE;
    }
#endif

#ifdef CONFIG_CISCO_HOTSPOT
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "X_COMCAST_COM_xfinitywifiEnable") == 0)
    {
        if (CosaDmlDiSetXfinityWiFiEnable(bValue) != ANSC_STATUS_SUCCESS)
            return FALSE;
        pMyObject->bxfinitywifiEnable = bValue;
		printf("%s : bxfinitywifiEnable value is : %d\n",__FUNCTION__,pMyObject->bxfinitywifiEnable);
        return TRUE;
    }
#endif
#if !defined (NO_MOCA_FEATURE_SUPPORT)
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_EnableMoCAforXi5") == 0)
    {
               /* No need to process same value received case */
               if( pMyObject->bEnableMoCAforXi5 == bValue )
               {
                       return TRUE;
               }
               CosaDmlDiSetEnableMoCAforXi5Flag( pMyObject, &bValue, &pMyObject->bEnableMoCAforXi5 );
       return TRUE;
    }
#endif
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
