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

        The prototypes of custom data model SBAPI are defined here

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


#ifndef  _COSA_DEVICEINFO_APIS_CUSTOM_H
#define  _COSA_DEVICEINFO_APIS_CUSTOM_H


/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define  COSA_DEVICEINFO_UseSnAsProvisioningCode    1

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

#ifdef CONFIG_VENDOR_CUSTOMER_COMCAST

/*X_COMCAST-COM_CM_MAC*/
ANSC_STATUS
CosaDmlDiGetCMMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/*X_COMCAST-COM_EWAN_CMMAC*/
ANSC_STATUS
CosaDmlDiGetEwanCMMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );


/*X_COMCAST-COM_WAN_IP*/
ANSC_STATUS
CosaDmlDiGetRouterMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/*X_COMCAST-COM_MTA_MAC*/
ANSC_STATUS
CosaDmlDiGetMTAMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/*X_COMCAST-COM_CM_IP*/
ANSC_STATUS
CosaDmlDiGetCMIPAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/*X_COMCAST-COM_WAN_IP*/
ANSC_STATUS
CosaDmlDiGetRouterIPAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/*X_COMCAST-COM_MTA_IP*/
ANSC_STATUS
CosaDmlDiGetMTAIPAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/*X_COMCAST-COM_MTA_IPV6*/
ANSC_STATUS
CosaDmlDiGetMTAIPV6Address
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

#endif


#ifdef CONFIG_CISCO_HOTSPOT
ANSC_STATUS
CosaDmlDiGetXfinityWiFiCapable
    (
        BOOL *pValue
    );

ANSC_STATUS
CosaDmlDiGetXfinityWiFiEnable
    (
        BOOL *pValue
    );

ANSC_STATUS
CosaDmlDiSetXfinityWiFiEnable
    (
        BOOL value
    );

ANSC_STATUS
	CosaDmlDiGetCloudCapable
	(
		BOOL *pValue
	);

#endif

ANSC_STATUS
CosaDmlGetCloudUIReachableStatus
    (
		CHAR *pCloudPersonalizationURL,
        BOOL *pValue
    );

ANSC_STATUS
CosaDmlGetCaptivePortalEnable
    (
        BOOL *pValue
    );

ANSC_STATUS
CosaDmlDiGetRouterIPv6Address
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlSetCaptivePortalEnable
    (
        BOOL value
    );

#if defined(INTEL_PUMA7) || defined(_COSA_BCM_ARM_) || defined(_COSA_QCA_ARM_)
ANSC_STATUS
CosaDmlSetLED
		(
			int color,
			int state,
			int interval
		);
#endif

#endif
