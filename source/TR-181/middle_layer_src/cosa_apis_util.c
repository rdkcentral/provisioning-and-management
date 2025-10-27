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


/**********************************************************************

    module:	cosa_apis_util.h

        This is base file for all parameters H files.

    ---------------------------------------------------------------

    description:

        This file contains all utility functions for COSA DML API development.

    ---------------------------------------------------------------

    environment:

        COSA independent

    ---------------------------------------------------------------

    author:

        Roger Hu
        leichen2@cisco.com

    ---------------------------------------------------------------

    revision:

        01/30/2011    initial revision.
        06/15/2012    add IPv4 address utils
        06/15/2012    add vsystem()

**********************************************************************/



#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "safec_lib_common.h"
#include "cosa_drg_common.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/types.h>
#include "ansc_platform.h"

//$HL 4/30/2013
#include "ccsp_psm_helper.h"

#if defined (FEATURE_RDKB_WAN_MANAGER)
extern ANSC_HANDLE bus_handle;
#define ETH_AGENT_COMPONENT_NAME                "eRT.com.cisco.spvtg.ccsp.ethagent"
#define ETH_AGENT_DBUS_PATH                     "/com/cisco/spvtg/ccsp/ethagent"
#endif 

ANSC_STATUS
COSAGetParamValueByPathName
    (
        void*                      bus_handle,
        parameterValStruct_t       *val,
        ULONG                      *parameterValueLength
    );

ANSC_STATUS
CosaUtilStringToHex
    (
        char          *str,
        unsigned char *hex_str,
        int           hex_sz 
    )
{
    INT   i = 0, index = 0, val = 0;
    CHAR  byte[3]       = {'\0'};

    while(str[i] != '\0')
    {
        byte[0] = str[i];
        byte[1] = str[i+1];
        byte[2] = '\0';
        if(_ansc_sscanf(byte, "%x", &val) != 1)
            break;
	hex_str[index] = val;
        i += 2;
        index++;
        if (str[i] == ':' || str[i] == '-'  || str[i] == '_')
            i++;
    }
    if(index != hex_sz)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

uint32_t CosaUtilGetIfAddr (char *netdev)
{
    struct ifreq ifr;
    struct sockaddr_in *ipaddr;
    uint32_t value = 0;
    errno_t rc;
    int fd;

#if defined (FEATURE_SUPPORT_MAPT_NAT46)

    #define BUFLEN_8 8
    char maptEnable[BUFLEN_8] = {0};
    char  maptConfig[BUFLEN_8] = {0};
    int strcmp_ret = -1;

    if (0 == syscfg_get(NULL, "MAPT_Enable", maptEnable, sizeof(maptEnable)))
    {
        rc = strcmp_s(maptEnable, 4, "true", &strcmp_ret);
        ERR_CHK(rc);
        if (0 == strcmp_ret)
        {
            if (0 == commonSyseventGet("mapt_config_flag", maptConfig, sizeof(maptConfig)))
            {
                strcmp_ret = -1;
                rc = strcmp_s(maptConfig, 3, "set", &strcmp_ret);
                ERR_CHK(rc);
                if (0 == strcmp_ret)
                {
                    strcmp_ret = -1;
                    rc = strcmp_s(netdev, 8, "erouter0", &strcmp_ret);
                    ERR_CHK(rc);
                    if (0 == strcmp_ret)
                    {
                        return value;
                    }
                }
            }
        }
    }
#endif
    rc = strcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), netdev);
    if (rc != EOK)
    {
       ERR_CHK(rc);
    }

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
    {
        if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
        {
            /*
               The 32bit IP address is returned within the sin_addr field of
               a struct sockaddr_in structure. It is always correctly aligned
               and in network byte order (ie it is always big endian and NOT
               determined by the endianness of the target CPU).
            */
            ipaddr = (struct sockaddr_in *) &ifr.ifr_addr;
            value = (uint32_t) ipaddr->sin_addr.s_addr;
        }
        else
        {
           perror("CosaUtilGetIfAddr IOCTL failure.");
           CcspTraceWarning(("Cannot get ipv4 address of netdev:%s\n",netdev));
        }

        close(fd);
    }
    else
    {
        perror("CosaUtilGetIfAddr failed to open socket.");
    }
    return value;
}

ANSC_STATUS
CosaSListPushEntryByInsNum
    (
        PSLIST_HEADER               pListHead,
        PCOSA_CONTEXT_LINK_OBJECT   pCosaContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContextEntry = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    ULONG                           ulIndex           = 0;

    if ( pListHead->Depth == 0 )
    {
        AnscSListPushEntryAtBack(pListHead, &pCosaContext->Linkage);
    }
    else
    {
        pSLinkEntry = AnscSListGetFirstEntry(pListHead);

        for ( ulIndex = 0; ulIndex < pListHead->Depth; ulIndex++ )
        {
            pCosaContextEntry = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

            if ( pCosaContext->InstanceNumber < pCosaContextEntry->InstanceNumber )
            {
                AnscSListPushEntryByIndex(pListHead, &pCosaContext->Linkage, ulIndex);

                return ANSC_STATUS_SUCCESS;
            }
        }

        AnscSListPushEntryAtBack(pListHead, &pCosaContext->Linkage);
    }

    return ANSC_STATUS_SUCCESS;
}

PCOSA_CONTEXT_LINK_OBJECT
CosaSListGetEntryByInsNum
    (
        PSLIST_HEADER               pListHead,
        ULONG                       InstanceNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContextEntry = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    ULONG                           ulIndex           = 0;

    if ( pListHead->Depth == 0 )
    {
        return NULL;
    }
    else
    {
        pSLinkEntry = AnscSListGetFirstEntry(pListHead);

        for ( ulIndex = 0; ulIndex < pListHead->Depth; ulIndex++ )
        {
            pCosaContextEntry = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

            if ( pCosaContextEntry->InstanceNumber == InstanceNumber )
            {
                return pCosaContextEntry;
            }
        }
    }

    return NULL;
}

PUCHAR
CosaUtilGetLowerLayers
    (
        PUCHAR                      pTableName,
        PUCHAR                      pKeyword
    )
{

    ULONG                           ulNumOfEntries              = 0;
    ULONG                           i                           = 0;
    ULONG                           j                           = 0;
    ULONG                           ulEntryNameLen;
    CHAR                            ucEntryParamName[256]       = {0};
    CHAR                            ucEntryNameValue[256]       = {0};
    CHAR                            ucEntryFullPath[256]        = {0};
    CHAR                            ucLowerEntryPath[256]       = {0};
    CHAR                            ucLowerEntryName[256]       = {0};
    ULONG                           ulEntryInstanceNum          = 0;
    ULONG                           ulEntryPortNum              = 0;
#if defined (INTEL_PUMA7)
    ULONG                           ulEntryIntfNum              = 0;
#endif
    PUCHAR                          pMatchedLowerLayer          = NULL;
    PANSC_TOKEN_CHAIN               pTableListTokenChain        = (PANSC_TOKEN_CHAIN)NULL;
    PANSC_STRING_TOKEN              pTableStringToken           = (PANSC_STRING_TOKEN)NULL;
    errno_t                         rc                          = -1;

    if ( !pTableName || AnscSizeOfString((const char*)pTableName) == 0 ||
         !pKeyword   || AnscSizeOfString((const char*)pKeyword) == 0
       )
    {
        return NULL;
    }

    pTableListTokenChain = AnscTcAllocate((char*)pTableName, ",");

    if ( !pTableListTokenChain )
    {
        return NULL;
    }

    while ((pTableStringToken = AnscTcUnlinkToken(pTableListTokenChain)))
    {
            /* CID: 56300 Array compared against 0*/
            if (strcmp(pTableStringToken->Name, "Device.Ethernet.Interface.") == 0)
            {
                parameterValStruct_t varStruct;
                ulNumOfEntries = 0;
                rc = strcpy_s(ucEntryParamName, sizeof(ucEntryParamName), "Device.Ethernet.InterfaceNumberOfEntries");
                ERR_CHK(rc);
                varStruct.parameterName = ucEntryParamName;
                varStruct.parameterValue = ucEntryNameValue;

                ulEntryNameLen = sizeof(ucEntryNameValue);
                if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                {
                    AnscTraceFlow(("<HL>%s not found %s\n",__FUNCTION__,varStruct.parameterName ));
                    break;
                }
                _ansc_sscanf(ucEntryNameValue,"%lu",&ulNumOfEntries);
                i = 0;
                ulEntryInstanceNum =1;
                while (i < ulNumOfEntries)
                {
                    _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));
                    rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName),"Device.Ethernet.Interface.%lu.Name",ulEntryInstanceNum);
                    if(rc < EOK)
                    {
                      ERR_CHK(rc);
                      ulEntryInstanceNum++;
                      i++;
                      continue;
                    }

                    ulEntryNameLen = sizeof(ucEntryNameValue);
                     if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                    {
                        AnscTraceFlow(("<HL>%s Ethernet instance(%lu) not found\n", __FUNCTION__,
                            ulEntryInstanceNum));
                        ulEntryInstanceNum++;
                        continue;
                    }
                    if (strcmp(ucEntryNameValue, (char*)pKeyword) == 0)
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath),"Device.Ethernet.Interface.%lu",ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                           ERR_CHK(rc);
                           ulEntryInstanceNum++;
                           i++;
                           continue;
                        }
#if defined(_PLATFORM_RASPBERRYPI_)
                        // This logic is needed for RPI target due to hardware limitations(i,e Ethernet bridge ports are not active at all time)
                        _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));
                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName),"Device.Ethernet.Interface.%lu.Enable",ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                           ERR_CHK(rc);
                           ulEntryInstanceNum++;
                           i++;
                           continue;
                        }
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                        {
                            AnscTraceFlow(("<HL>%s Ethernet instance(%lu) not found\n", __FUNCTION__,
                            ulEntryInstanceNum));
                            ulEntryInstanceNum++;
                            continue;
                        }
                        if (strcmp(ucEntryNameValue, "false") == 0)
			     break;
#endif
                        pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucEntryFullPath);
                        break;
                    }
                    ulEntryInstanceNum++;
                    i++;
                }
            }
            else if (strcmp(pTableStringToken->Name, "Device.IP.Interface.") == 0)
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.IP.InterfaceNumberOfEntries");
                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.IP.Interface.", i);

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath), "Device.IP.Interface.%lu",  ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          continue;
                        }

                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%s.Name", ucEntryFullPath);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          continue;
                        }

                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             (strcmp(ucEntryNameValue, (char*)pKeyword) == 0) )
                        {
                            pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
            else if (strcmp(pTableStringToken->Name, "Device.USB.Interface.") == 0)
            {
            }
            else if (strcmp(pTableStringToken->Name, "Device.HPNA.Interface.") == 0)
            {
            }
            else if (strcmp(pTableStringToken->Name, "Device.DSL.Interface.") == 0)
            {
            }
            else if (strcmp(pTableStringToken->Name, "Device.WiFi.Radio.") == 0)
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.WiFi.RadioNumberOfEntries");

                for (i = 0; i < ulNumOfEntries; i++)
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.WiFi.Radio.", i);
                    
                    if (ulEntryInstanceNum)
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath), "Device.WiFi.Radio.%lu.", ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          continue;
                        }
                        
                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%sName", ucEntryFullPath);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          continue;
                        }
                        
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if (( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                            AnscEqualString(ucEntryNameValue, (char*)pKeyword, TRUE) )
                        {
                            pMatchedLowerLayer = (PUCHAR)AnscCloneString(ucEntryFullPath);
                            
                            break;
                        }
                    }
                }
            }
            else if (strcmp(pTableStringToken->Name, "Device.HomePlug.Interface.") == 0)
            {
            }
#if !defined (NO_MOCA_FEATURE_SUPPORT)
            else if (strcmp(pTableStringToken->Name, "Device.MoCA.Interface.") == 0)
            {

                parameterValStruct_t varStruct;
                  ulNumOfEntries = 0;
                rc = strcpy_s(ucEntryParamName, sizeof(ucEntryParamName), "Device.MoCA.InterfaceNumberOfEntries");
                ERR_CHK(rc);
                varStruct.parameterName = ucEntryParamName;
                varStruct.parameterValue = ucEntryNameValue;

                ulEntryNameLen = sizeof(ucEntryNameValue);
                if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                {
                    AnscTraceFlow(("<HL>%s not found %s\n",__FUNCTION__,varStruct.parameterName ));
                    break;
                }

                //AnscTraceFlow(("<HL>%s ucEntryNameValue=%s\n", __FUNCTION__,ucEntryNameValue));
                _ansc_sscanf((const char*)ucEntryNameValue,"%lu",&ulNumOfEntries);
                //AnscTraceFlow(("<HL>%s Wifi # of entries=%d\n", __FUNCTION__,ulNumOfEntries));

                i = 0;
                ulEntryInstanceNum =1;
                while (i < ulNumOfEntries)
                {
                    _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));
                    rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "Device.MoCA.Interface.%lu.Name",ulEntryInstanceNum);
                    if(rc < EOK)
                    {
                      ERR_CHK(rc);
                      ulEntryInstanceNum++;
                      i++;
                      continue;
                    }
                        
                    ulEntryNameLen = sizeof(ucEntryNameValue);
                    if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                    {
                        AnscTraceFlow(("<HL>%s WiFi instance(%lu) not found\n", __FUNCTION__,
                            ulEntryInstanceNum));
                        ulEntryInstanceNum++;
                        continue;
                    }  
                    //AnscTraceFlow(("<HL>%s WiFi instance(%d) has name =%s inputName=%s\n",__FUNCTION__,ulEntryInstanceNum,ucEntryNameValue,pKeyword));
                    if (strcmp(ucEntryNameValue, (char*)pKeyword) == 0)
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath),"Device.MoCA.Interface.%lu",ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          ulEntryInstanceNum++;
                          i++;
                          continue;
                        }
                        pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucEntryFullPath);

                        break;
                    }
                    ulEntryInstanceNum++;
                    i++;
                }

            }
#endif
            else if (strcmp(pTableStringToken->Name, "Device.X_CISCO_COM_GRE.Interface.") == 0)
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.X_CISCO_COM_GRE.InterfaceNumberOfEntries");

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.X_CISCO_COM_GRE.Interface.", i);

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath), "Device.X_CISCO_COM_GRE.Interface.%lu", ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          continue;
                        }

                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%s.Name", ucEntryFullPath);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          continue;
                        }
               
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             (strcmp(ucEntryNameValue, (char*)pKeyword) == 0) )
                        {
                            pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
#if defined (INTEL_PUMA7)
            else if (strcmp(pTableStringToken->Name, "Device.GRE.Tunnel.") == 0)
            {
                ulNumOfEntries =  CosaGetParamValueUlong("Device.GRE.TunnelNumberOfEntries");
                CcspTraceInfo(("----------CosaUtilGetLowerLayers, tunnelnum:%lu\n", ulNumOfEntries));
                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.GRE.Tunnel.", i);
                    CcspTraceInfo(("----------CosaUtilGetLowerLayers, instance num:%lu\n", ulEntryInstanceNum));
                    if ( ulEntryInstanceNum )
                    {
                        _ansc_sprintf(ucEntryFullPath, "%s%lu", "Device.GRE.Tunnel.", ulEntryInstanceNum);
                        rc = sprintf_s(ucLowerEntryPath, sizeof(ucLowerEntryPath), "%s%s", ucEntryFullPath, ".InterfaceNumberOfEntries");
                        if(rc < EOK)  ERR_CHK(rc);

                        ulEntryIntfNum = CosaGetParamValueUlong(ucLowerEntryPath);
                        CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,intf num:%lu\n",ucLowerEntryPath, ulEntryIntfNum));
                        for ( j = 1; j<= ulEntryIntfNum; j++) {
                            rc = sprintf_s(ucLowerEntryName, sizeof(ucLowerEntryName), "%s%s%lu", ucEntryFullPath, ".Interface.", j);
                            if(rc < EOK)  ERR_CHK(rc);
                            rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%s%s%lu%s", ucEntryFullPath, ".Interface.", j, ".Name");
                            if(rc < EOK)  ERR_CHK(rc);
                            CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,Param2:%s\n", ucLowerEntryName, ucEntryParamName));
                            ulEntryNameLen = sizeof(ucEntryNameValue);
                            if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                                 AnscEqualString(ucEntryNameValue, (char *)pKeyword , TRUE ) )
                            {
                                pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucLowerEntryName);
                                CcspTraceInfo(("----------CosaUtilGetLowerLayers, J:%lu, LowerLayer:%s\n", j, pMatchedLowerLayer));
                                break;
                            }
                        }
                    }
                }
            }
#endif
            else if (strcmp(pTableStringToken->Name, "Device.UPA.Interface.") == 0)
            {
            }
            else if (strcmp(pTableStringToken->Name, "Device.ATM.Link.") == 0)
            {
            }
            else if (strcmp(pTableStringToken->Name, "Device.PTM.Link.") == 0)
            {
            }
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
            else if (strcmp(pTableStringToken->Name, "Device.X_RDK_Ethernet.Link.") == 0)
            {
                PAM_GetInterfaceInstanceFromVlanmanager((char*)pKeyword, &pMatchedLowerLayer);
            }
#endif
            else if (strcmp(pTableStringToken->Name, "Device.Ethernet.Link.") == 0)
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.Ethernet.LinkNumberOfEntries");

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Ethernet.Link.", i);

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath), "Device.Ethernet.Link.%lu", ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          continue;
                        }

                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%s.Name", ucEntryFullPath);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          continue;
                        }
               
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             (strcmp(ucEntryNameValue, (char*)pKeyword) == 0) )
                        {
                            pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
            else if (strcmp(pTableStringToken->Name, "Device.Ethernet.VLANTermination.") == 0)
            {
            }
            else if (strcmp(pTableStringToken->Name, "Device.WiFi.SSID.") == 0)
            {
                parameterValStruct_t varStruct;
                ulNumOfEntries = 0;
                rc = strcpy_s(ucEntryParamName, sizeof(ucEntryParamName), "Device.WiFi.SSIDNumberOfEntries");
                ERR_CHK(rc);
                varStruct.parameterName = ucEntryParamName;
                varStruct.parameterValue = ucEntryNameValue;

                ulEntryNameLen = sizeof(ucEntryNameValue);
                if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                {
                    AnscTraceFlow(("<HL>%s not found %s\n",__FUNCTION__,varStruct.parameterName ));
                    break;
                }
                //AnscTraceFlow(("<HL>%s ucEntryNameValue=%s\n", __FUNCTION__,ucEntryNameValue));
                _ansc_sscanf(ucEntryNameValue,"%lu",&ulNumOfEntries);
                //AnscTraceFlow(("<HL>%s Wifi # of entries=%d\n", __FUNCTION__,ulNumOfEntries));
                i = 0;
                ulEntryInstanceNum =1;
                while (i < ulNumOfEntries)
                {
                    _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));
                    rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName),"Device.WiFi.SSID.%lu.Name",ulEntryInstanceNum);
                    if(rc < EOK)
                    {
                      ERR_CHK(rc);
                      ulEntryInstanceNum++;
                      i++;
                      continue;
                    }
                        
                    ulEntryNameLen = sizeof(ucEntryNameValue);
                    if (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen))
                    {
                        AnscTraceFlow(("<HL>%s WiFi instance(%lu) not found\n", __FUNCTION__,
                            ulEntryInstanceNum));
                        ulEntryInstanceNum++;
                        continue;
                    }  
                    //AnscTraceFlow(("<HL>%s WiFi instance(%d) has name =%s inputName=%s\n",__FUNCTION__,ulEntryInstanceNum,ucEntryNameValue,pKeyword));
                    if (strcmp(ucEntryNameValue, (char*)pKeyword) == 0)
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath),"Device.WiFi.SSID.%lu",ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                           ERR_CHK(rc);
                           ulEntryInstanceNum++;
                           i++;
                           continue;
                        }
                        pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucEntryFullPath);

                        break;
                    }
                    ulEntryInstanceNum++;
                    i++;
                }
            }
            else if (strcmp(pTableStringToken->Name, "Device.Bridging.Bridge.") == 0)
            {
                ulNumOfEntries =  CosaGetParamValueUlong("Device.Bridging.BridgeNumberOfEntries");
                CcspTraceInfo(("----------CosaUtilGetLowerLayers, bridgenum:%lu\n", ulNumOfEntries));
                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Bridging.Bridge.", i);
                    CcspTraceInfo(("----------CosaUtilGetLowerLayers, instance num:%lu\n", ulEntryInstanceNum));

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath), "Device.Bridging.Bridge.%lu", ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                           ERR_CHK(rc);
                           continue;
                        }
                        rc = sprintf_s(ucLowerEntryPath, sizeof(ucLowerEntryPath), "%s.PortNumberOfEntries", ucEntryFullPath); 
                        if(rc < EOK)
                        {
                           ERR_CHK(rc);
                           continue;
                        }
                        ulEntryPortNum = CosaGetParamValueUlong(ucLowerEntryPath);  
                        CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,port num:%lu\n",ucLowerEntryPath, ulEntryPortNum));

                        for ( j = 1; j<= ulEntryPortNum; j++) {
                            rc = sprintf_s(ucLowerEntryName, sizeof(ucLowerEntryName), "%s.Port.%lu", ucEntryFullPath, j);
                            if(rc < EOK)
                            {
                              ERR_CHK(rc);
                              continue;
                            }
                            rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%s.Port.%lu.Name", ucEntryFullPath, j);
                            if(rc < EOK)
                            {
                              ERR_CHK(rc);
                              continue;
                            }
                            CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,Param2:%s\n", ucLowerEntryName, ucEntryParamName));
                        
                            ulEntryNameLen = sizeof(ucEntryNameValue);
                            if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                                 AnscEqualString(ucEntryNameValue, (char*)pKeyword , TRUE ) )
                            {
                                pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucLowerEntryName);
                                CcspTraceInfo(("----------CosaUtilGetLowerLayers, J:%lu, LowerLayer:%s\n", j, pMatchedLowerLayer));
                                break;
                            }
                        }
                    }
                }
            }
            else if (strcmp(pTableStringToken->Name, "Device.PPP.Interface.") == 0)
            {
            }
            else if (strcmp(pTableStringToken->Name, "Device.DSL.Channel.") == 0)
            {
            }
            
            if ( pMatchedLowerLayer )
            {
		AnscFreeMemory(pTableStringToken);
                break;
            }

        AnscFreeMemory(pTableStringToken);
    }
    if ( pTableListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pTableListTokenChain);
    }

    /* Too many trace. Disable this  -- Yan */
    /*
    CcspTraceWarning
        ((
            "CosaUtilGetLowerLayers: %s matched LowerLayer(%s) with keyword %s in the table %s\n",
            pMatchedLowerLayer ? "Found a":"Not find any",
            pMatchedLowerLayer ? pMatchedLowerLayer : "",
            pKeyword,
            pTableName
        ));
    */

    return pMatchedLowerLayer;
}

/*
 *
 */
/*mapping for LinkType conversion*/

/* please don't alter LinkType, it can be used as index starting with 1*/
LINKTYPE_MAP_T g_linktype_map[COSA_DML_LINK_TYPE_TOTAL] = {
    {   "Device.Ethernet.Interface.", 
        "Ethernet", 
        COSA_DML_LINK_TYPE_Eth 
    },
    {   "Device.Ethernet.Link.", 
        "EthLink",
        COSA_DML_LINK_TYPE_EthLink 
    },
    {   "Device.Ethernet.VLANTermination.", 
        "Ethernet.VLANTermination", 
        COSA_DML_LINK_TYPE_EthVlan
    },
    {   "Device.USB.Interface.", 
        "USB", 
        COSA_DML_LINK_TYPE_Usb
    },
    {   "Device.HPNA.Interface.", 
        "HPNA", 
        COSA_DML_LINK_TYPE_Hpna
    },
#if !defined (NO_MOCA_FEATURE_SUPPORT)
    {   "Device.MoCA.Interface.", 
        "MoCA", 
        COSA_DML_LINK_TYPE_Moca
    },
#endif
    {   "Device.HomePlug.Interface.", 
        "HomePlug", 
        COSA_DML_LINK_TYPE_HomePlug
    },
    {   "Device.UPA.Interface.", 
        "UPA", 
        COSA_DML_LINK_TYPE_Upa
    },
    {   "Device.WiFi.SSID.", 
        "WiFi", 
        COSA_DML_LINK_TYPE_WiFiSsid
    },
    {   "Device.Bridging.Bridge.", 
        "Bridge", 
        COSA_DML_LINK_TYPE_Bridge
    },
    {   "Device.PPP.Interface.", 
        "PPP", 
        COSA_DML_LINK_TYPE_PPP
    },
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE) || defined(_RDKB_GLOBAL_PRODUCT_REQ_)
#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
    {   "Device.X_RDK_Ethernet.VLANTermination.",
        "VlanLink",
        COSA_DML_LINK_TYPE_VlanLink
    },
#endif
    {   "Device.X_RDK_Ethernet.Link.",
        "VlanLink",
        COSA_DML_LINK_TYPE_VlanLink
    },
#endif
};


COSA_DML_LINK_TYPE CosaUtilGetLinkTypeFromStr(char* pLinkTypeStr)
{
    int index=0;
    if(NULL == pLinkTypeStr)
        return COSA_DML_LINK_TYPE_LAST;

    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        if(NULL != g_linktype_map[index].LinkTypeStr && strstr(pLinkTypeStr, g_linktype_map[index].LinkTypeStr) != NULL)
        {
            return g_linktype_map[index].LinkType;
        }
    }

    return COSA_DML_LINK_TYPE_LAST;
}

char* CosaUtilGetStrFromLinkTypePath(char* pLinkTypePath){
    int index=0;

    if(NULL == pLinkTypePath)
        return NULL;

    AnscTraceFlow(("%s: %s\n", __FUNCTION__, pLinkTypePath));
    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        //if(!strncmp(g_linktype_map[index].LinkTypePath, pLinkTypePath, 
        //        sizeof(g_linktype_map[index].LinkTypePath)))
        if(NULL != g_linktype_map[index].LinkTypePath && strstr(pLinkTypePath, g_linktype_map[index].LinkTypePath) != NULL)
        {
            AnscTraceFlow(("%s: return index %d\n", __FUNCTION__, index));
            return g_linktype_map[index].LinkTypeStr;
        }
    }

    return NULL;
}

char* CosaUtilGetLinkTypeStr(COSA_DML_LINK_TYPE LinkType)
{
    int index=0;
    AnscTraceFlow(("%s: %d\n", __FUNCTION__, LinkType));
    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        if(g_linktype_map[index].LinkType == LinkType)
        {
            AnscTraceFlow(("%s: return index %d\n", __FUNCTION__, index));
            return g_linktype_map[index].LinkTypeStr;
        }
    }
    return NULL;
}

char* CosaUtilGetLinkTypePath(COSA_DML_LINK_TYPE LinkType)
{
    int index=0;
    AnscTraceFlow(("%s: %d\n", __FUNCTION__, LinkType));
    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        if(g_linktype_map[index].LinkType == LinkType)
        {
            AnscTraceFlow(("%s: return index %d\n", __FUNCTION__, index));
            return g_linktype_map[index].LinkTypePath;
        }
    }
    return NULL;
}

COSA_DML_LINK_TYPE CosaUtilGetLinkTypeFromPath(char*pLinkTypePath)
{
    int index=0;
    AnscTraceFlow(("%s: %s\n", __FUNCTION__, pLinkTypePath));
    for(index=0; index<COSA_DML_LINK_TYPE_TOTAL; index++)
    {
        //if(!strncmp(g_linktype_map[index].LinkTypePath, pLinkTypePath, 
        //        sizeof(g_linktype_map[index].LinkTypePath)))
        if(NULL != g_linktype_map[index].LinkTypePath && strstr(pLinkTypePath, g_linktype_map[index].LinkTypePath) != NULL)
        {
            AnscTraceFlow(("%s: return index %d\n", __FUNCTION__, index));
            return g_linktype_map[index].LinkType;
        }
    }
    return COSA_DML_LINK_TYPE_LAST;
}


ANSC_STATUS
CosaUtilConstructLowerLayers
    (
        COSA_DML_LINK_TYPE          LinkType,
        ULONG                       InstNumber,
        char*                       pLowerLayersBuf,
        PULONG                      pBufLen        
    )
{
    char*                           linkTypePath;
    errno_t                         rc  = -1;

    linkTypePath = CosaUtilGetLinkTypePath(LinkType);

    /* TBD - Need to check the pBufLen */

    if ( LinkType == COSA_DML_LINK_TYPE_Bridge )
    {
        /* Special processing for Bridge type LowerLayers */
        rc = sprintf_s(pLowerLayersBuf, *pBufLen,"%s%lu.Port.1", linkTypePath, InstNumber);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return  ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        rc = sprintf_s(pLowerLayersBuf, *pBufLen ,"%s%lu", linkTypePath, InstNumber);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return  ANSC_STATUS_FAILURE;
        }
    }

#if !defined(_64BIT_ARCH_SUPPORT_)
    AnscTraceFlow(("%s, size %d, buf len %lu\n", pLowerLayersBuf, _ansc_strlen(pLowerLayersBuf), *pBufLen));
#else
    AnscTraceFlow(("%s, size %zu, buf len %lu\n", pLowerLayersBuf, _ansc_strlen(pLowerLayersBuf), *pBufLen));
#endif
    return  ANSC_STATUS_SUCCESS;
}

#if defined (FEATURE_RDKB_WAN_MANAGER)
ANSC_STATUS EthAgent_getParams(char *pComponent, char *pBus, char *pParamName, char *pReturnVal)
{
    parameterValStruct_t **retVal = NULL;
    char *ParamName[1];
    int ret = 0, nval;

    //Assign address for get parameter name
    ParamName[0] = pParamName;

    ret = CcspBaseIf_getParameterValues(
        bus_handle,
        pComponent,
        pBus,
        ParamName,
        1,
        &nval,
        &retVal);

    //Copy the value
    if (CCSP_SUCCESS == ret)
    {
        /*CID-187380 NULL Pointer Dereference*/
        if (retVal != NULL)
        {
            if (NULL != retVal[0]->parameterValue)
            {
                CcspTraceWarning(("%s parameterValue[%s]\n", __FUNCTION__, retVal[0]->parameterValue));
                memcpy(pReturnVal, retVal[0]->parameterValue, strlen(retVal[0]->parameterValue) + 1);
            }
 
            free_parameterValStruct_t(bus_handle, nval, retVal);
        }

        return ANSC_STATUS_SUCCESS;
    }
    if (retVal)
    {
        free_parameterValStruct_t(bus_handle, nval, retVal);
    }

    return ANSC_STATUS_FAILURE;
}
#endif

/*
 *  Retrieve the parameter Name of the LowerLayer
 */
ANSC_STATUS
CosaUtilGetLowerLayerName
    (
        COSA_DML_LINK_TYPE          LinkType,
        ULONG                       InstNumber,
        char*                       pParamValueBuf,
        PULONG                      pBufLen        
    )
{
    ANSC_STATUS                     returnStatus;
    char                            pParamPath[256] = {0};
    ULONG                           ParamPathLen    = sizeof(pParamPath);

    UNREFERENCED_PARAMETER(pBufLen);
    returnStatus = CosaUtilConstructLowerLayers(LinkType, InstNumber, pParamPath, &ParamPathLen);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        /*CID: 66733 fix*/
        strncat(pParamPath, ".Name", sizeof(pParamPath) - strlen(pParamPath) - 1);
    }

#if defined (FEATURE_RDKB_WAN_MANAGER)
    // Use Cosa API for queries related to Bridge as ethagent does not support "Bridge"
    if(strstr(pParamPath, "Device.Bridging.Bridge."))
    {
        returnStatus = CosaGetParamValueString(pParamPath, pParamValueBuf, pBufLen);
    }
    else
    {
        returnStatus = EthAgent_getParams( ETH_AGENT_COMPONENT_NAME, ETH_AGENT_DBUS_PATH, pParamPath, pParamValueBuf);
    }
#else
    returnStatus = CosaGetParamValueString(pParamPath, pParamValueBuf, pBufLen);
#endif
    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        AnscTraceFlow(("CosaUtilGetLowerLayerName -- value %s\n", pParamValueBuf));

        return  returnStatus;
    }
    else
    {
        AnscTraceWarning(("CosaUtilGetLowerLayerName -- failure %lu, to retrieve %s\n", returnStatus, pParamPath));

        return  returnStatus;
    }
}


/*
 * Find the management port in Bridge and return name from that port
 * It is the name of the bridge.
 */
PUCHAR
CosaUtilFindBridgeName(char* pBridgePath)
{
    ULONG                           j                           = 0;
    ULONG                           ulEntryNameLen;
    CHAR                            ucEntryParamName[256]       = {0};
    CHAR                            ucEntryNameValue[256]       = {0};
   
    CHAR                            ucLowerEntryPath[256]       = {0};
    CHAR                            ucLowerEntryName[256]       = {0};
    ULONG                           ulEntryPortNum              = 0;
    PUCHAR                          pMatchedBridgeName          = NULL;
    BOOL                            bMgrPort                    = FALSE;
    errno_t                         rc                          = -1;

    rc = sprintf_s(ucLowerEntryPath, sizeof(ucLowerEntryPath), "%s%s", pBridgePath, ".PortNumberOfEntries");
    if(rc < EOK)
    {
       ERR_CHK(rc);
       return NULL;
    }
                        
    ulEntryPortNum = CosaGetParamValueUlong(ucLowerEntryPath);  
    AnscTraceFlow(("%s: Param:%s,port num:%lu\n", __FUNCTION__, ucLowerEntryPath, ulEntryPortNum));

    for ( j = 1; j<= ulEntryPortNum; j++) {
        rc = sprintf_s(ucLowerEntryName, sizeof(ucLowerEntryName) ,"%s.Port.%lu", pBridgePath, j);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          continue;
        }

        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName) , "%s.Port.%lu.ManagementPort", pBridgePath, j);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          continue;
        }
        AnscTraceFlow(("%s: Param:%s,Param2:%s\n",__FUNCTION__, ucLowerEntryName, ucEntryParamName)); 
                        
        bMgrPort = CosaGetParamValueBool(ucEntryParamName);
        if(bMgrPort)
        {
            rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%s.Port.%lu.Name", pBridgePath, j);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              continue;
            }
                
            ulEntryNameLen = sizeof(ucEntryNameValue);
            if (0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) 
            {
                // not include port instance.
                pMatchedBridgeName =  (PUCHAR)AnscCloneString(ucEntryNameValue);
                AnscTraceFlow(("%s: J:%lu, Name:%s\n", __FUNCTION__, j, pMatchedBridgeName));
                break;
            }
        }
    }

    return pMatchedBridgeName;
}

/*
 * Return Device.Bridging.Bridge.x
 * not include port instance number.
 */
PUCHAR
CosaUtilFindBridgePath(char* pBridgeName)
{
    ULONG                           ulNumOfEntries              = 0;
    ULONG                           i                           = 0;
    ULONG                           j                           = 0;
    ULONG                           ulEntryNameLen;
    CHAR                            ucEntryParamName[256]       = {0};
    CHAR                            ucEntryNameValue[256]       = {0};
    CHAR                            ucEntryFullPath[256]        = {0};
    CHAR                            ucLowerEntryPath[256]       = {0};
    CHAR                            ucLowerEntryName[256]       = {0};
    ULONG                           ulEntryInstanceNum          = 0;
    ULONG                           ulEntryPortNum              = 0;
    PUCHAR                          pMatchedLowerLayer         = NULL;
    errno_t                         rc                         = -1;
    

    ulNumOfEntries =  CosaGetParamValueUlong("Device.Bridging.BridgeNumberOfEntries");
    CcspTraceInfo(("----------CosaUtilGetLowerLayers, bridgenum:%lu\n", ulNumOfEntries));
    AnscTraceFlow(("%s: bridgenum:%lu\n", __FUNCTION__, ulNumOfEntries));
    for ( i = 0 ; i < ulNumOfEntries; i++ )
    {
        ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Bridging.Bridge.", i);
        CcspTraceInfo(("----------CosaUtilGetLowerLayers, instance num:%lu\n", ulEntryInstanceNum));
        AnscTraceFlow(("%s: instance num:%lu\n", __FUNCTION__, ulEntryInstanceNum));
        if ( ulEntryInstanceNum )
        {
            rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath), "Device.Bridging.Bridge.%lu", ulEntryInstanceNum);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              continue;
            }
            rc = sprintf_s(ucLowerEntryPath, sizeof(ucLowerEntryPath), "%s.PortNumberOfEntries", ucEntryFullPath);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              continue;
            }
                        
            ulEntryPortNum = CosaGetParamValueUlong(ucLowerEntryPath);  
            CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,port num:%lu\n",ucLowerEntryPath, ulEntryPortNum));

            for ( j = 1; j<= ulEntryPortNum; j++) {
                rc = sprintf_s(ucLowerEntryName, sizeof(ucLowerEntryName), "%s.Port.%lu", ucEntryFullPath, j);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  continue;
                }

                rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%s.Port.%lu.Name", ucEntryFullPath, j);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  continue;
                }

                CcspTraceInfo(("----------CosaUtilGetLowerLayers, Param:%s,Param2:%s\n", ucLowerEntryName, ucEntryParamName));
                        
                ulEntryNameLen = sizeof(ucEntryNameValue);
                if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                        AnscEqualString(ucEntryNameValue, (char*)pBridgeName , TRUE ) )
                {
                    // not include port instance.
                    pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucEntryFullPath);
                    CcspTraceInfo(("----------CosaUtilGetLowerLayers, J:%lu, LowerLayer:%s\n", j, pMatchedLowerLayer));
                    break;
                }
            }
        }
    }

    return pMatchedLowerLayer;
}
/*
    CosaUtilGetFullPathNameByKeyword
    
   Description:
        This funcation serves for searching other pathname  except lowerlayer.
        
    PUCHAR                      pTableName
        This is the Table names divided by ",". For example 
        "Device.Ethernet.Interface., Device.Dhcpv4." 
        
    PUCHAR                      pParameterName
        This is the parameter name which hold the keyword. eg: "name"
        
    PUCHAR                      pKeyword
        This is keyword. eg: "wan0".

    return value
        return result string which need be free by the caller.
*/
PUCHAR
CosaUtilGetFullPathNameByKeyword
    (
        PUCHAR                      pTableName,
        PUCHAR                      pParameterName,
        PUCHAR                      pKeyword
    )
{

    ULONG                           ulNumOfEntries              = 0;
    ULONG                           i                           = 0;
    ULONG                           ulEntryNameLen;
    CHAR                            ucEntryParamName[256]       = {0};
    CHAR                            ucEntryNameValue[256]       = {0};
    CHAR                            ucTmp[128]                  = {0};
    CHAR                            ucTmp2[128]                 = {0};
    CHAR                            ucEntryFullPath[256]        = {0};
    PUCHAR                          pMatchedLowerLayer          = NULL;
    ULONG                           ulEntryInstanceNum          = 0;  
    PANSC_TOKEN_CHAIN               pTableListTokenChain        = (PANSC_TOKEN_CHAIN)NULL;
    PANSC_STRING_TOKEN              pTableStringToken           = (PANSC_STRING_TOKEN)NULL;
    PCHAR                           pString                     = NULL;
    PCHAR                           pString2                    = NULL;
    errno_t                         rc                          = -1;

    if ( !pTableName || AnscSizeOfString((const char*)pTableName) == 0 ||
         !pKeyword   || AnscSizeOfString((const char*)pKeyword) == 0   ||
         !pParameterName   || AnscSizeOfString((const char*)pParameterName) == 0
       )
    {
        return NULL;
    }

    pTableListTokenChain = AnscTcAllocate((char*)pTableName, ",");

    if ( !pTableListTokenChain )
    {
        return NULL;
    }

    while ((pTableStringToken = AnscTcUnlinkToken(pTableListTokenChain)))
    {
         /*CID: 62085 Array compared against 0*/
            /* Get the string XXXNumberOfEntries */
            pString2 = &pTableStringToken->Name[0];
            pString  = pString2;
            for (i = 0;pTableStringToken->Name[i]; i++)
            {
                if ( pTableStringToken->Name[i] == '.' )
                {
                    pString2 = pString;
                    pString  = &pTableStringToken->Name[i+1];
                }
            }

            pString--;
            pString[0] = '\0';
            rc = sprintf_s(ucTmp2, sizeof(ucTmp2), "%sNumberOfEntries", pString2);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pTableStringToken);
              continue;
            }
            pString[0] = '.';

            /* Enumerate the entry in this table */
            if ( TRUE )
            {
                pString2--;
                pString2[0]='\0';
                rc = sprintf_s(ucTmp, sizeof(ucTmp), "%s.%s", pTableStringToken->Name, ucTmp2);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  AnscFreeMemory(pTableStringToken);
                  continue;
                }

                pString2[0]='.';
                ulNumOfEntries =       CosaGetParamValueUlong(ucTmp);

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex(pTableStringToken->Name, i);

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath), "%s%lu.", pTableStringToken->Name, ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          AnscFreeMemory(pTableStringToken);
                          continue;
                        }

                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName), "%s%s", ucEntryFullPath, pParameterName);
                        if(rc < EOK)
                        {
                          ERR_CHK(rc);
                          AnscFreeMemory(pTableStringToken);
                          continue;
                        }
               
                        ulEntryNameLen = sizeof(ucEntryNameValue);
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             (strcmp(ucEntryNameValue, (char*)pKeyword) == 0) )
                        {
                            pMatchedLowerLayer =  (PUCHAR)AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }

            if ( pMatchedLowerLayer )
            {
                break;
            }

        if(NULL != pTableStringToken)
        {
            AnscFreeMemory(pTableStringToken);
            pTableStringToken = NULL;  /* CID 348164 fix - Double free prevention */
        }
    }

    // Free remaining tokens in the token chain
    while ((pTableStringToken = AnscTcUnlinkToken(pTableListTokenChain)) != NULL)
    {
        AnscFreeMemory(pTableStringToken);
        pTableStringToken = NULL;  /* CID 348164 fix - Double free prevention */
    }

    if ( pTableListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pTableListTokenChain);
    }

/*
    CcspTraceWarning
        ((
            "CosaUtilGetFullPathNameByKeyword: %s matched parameters(%s) with keyword %s in the table %s(%s)\n",
            pMatchedLowerLayer ? "Found a":"Not find any",
            pMatchedLowerLayer ? pMatchedLowerLayer : "",
            pKeyword,
            pTableName,
            pParameterName
        ));
*/

    return pMatchedLowerLayer;
}


ULONG
CosaUtilChannelValidate
    (
        UINT                       uiRadio,
        ULONG                      Channel
    )
{
    unsigned long channelList_5G [] = {36, 40, 44, 48, 52, 56, 60, 64, 149, 153, 157, 161, 165};
    int i;
    switch(uiRadio)
    {
        case 1:
             if((Channel < 1) || (Channel > 11))
                return 0;
             return 1;
        case 2:
             for(i=0; i<13; i++)
             {
                if(Channel == channelList_5G[i])
                  return 1;
             }
             return 0;
             break;
        default:
             break;
     }
     return 0;
}

ULONG
CosaUtilChannelValidate2
    (
        UINT                       uiRadio,
        ULONG                      Channel,
        char                       *channelList
    )
{
    // This should be updated to use the possible channels list  Device.WiFi.Radio.1.PossibleChannels instead of a static list.
    unsigned long channelList_5G [] = {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165};
    int i;

    // Channel maybe 0 if radio is disabled or auto channel was set
    if (Channel == 0) {
        return 1;
    }

    // If channelList is provided use it.
    if (channelList != NULL) {
        char chan[4];
        errno_t rc = -1;
        rc = sprintf_s(chan, sizeof(chan),"%lu",Channel);
        if(rc < EOK)
        {
           ERR_CHK(rc);
           return 0;
        }
        if (strstr(channelList,chan) != NULL) {
            return 1;
        } 
    }

    switch(uiRadio)
    {
        case 1:
             if(Channel > 11)
                return 0;
             return 1;
        case 2:
             for(i=0; i<24; i++)
             {
                if(Channel == channelList_5G[i])
                  return 1;
             }
             return 0;
             break;
        default:
             break;
     }
     return 0;
}

ULONG CosaUtilIoctlXXX(char * if_name, char * method, void * input)
{
    ULONG ret = 0;
    struct ifreq ifr;
	int sock;

    if (!if_name || !method)
    {
        return 0;
    }
    
	sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0 ) 
    {
        return 0;
    }
    //CID 162999 : Buffer not null terminated 
    strncpy(ifr.ifr_name, if_name, IF_NAMESIZE-1);

    if (!strcmp(method, "mtu"))
    {
        if (ioctl(sock, SIOCGIFMTU, &ifr) == 0)
        {
            ret = ifr.ifr_mtu;
            goto _EXIT;
        }
        else 
        {
            goto _EXIT;
        }
    }
    if (!strcmp(method, "setmtu"))
    {
        ULONG mtu = *(ULONG *)input;
        ifr.ifr_mtu = mtu;

        ret = ioctl(sock, SIOCSIFMTU, &ifr);
        goto _EXIT;
    }
    else if (!strcmp(method, "status"))
    {
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
        {
            ret = ifr.ifr_flags;
            goto _EXIT;

        }
        else 
        {
            goto _EXIT;
        }        
    }
    else if (!strcmp(method, "netmask"))
    {
        if (ioctl(sock, SIOCGIFNETMASK, &ifr) == 0)
        {
            memcpy(&ret, ifr.ifr_netmask.sa_data + 2,4);
            goto _EXIT;

        }
        else 
        {
            goto _EXIT;
        }        
    }
    else if (!strcmp(method, "set_netmask"))
    {
        ULONG mask = *(ULONG *)input;

        /*first get netmask then modify it*/
        if (ioctl(sock, SIOCGIFNETMASK, &ifr) == 0)
        {
            memcpy(ifr.ifr_netmask.sa_data + 2, &mask, sizeof(mask));
            ret = ioctl(sock, SIOCSIFNETMASK, &ifr);
            
            goto _EXIT;
        }
        else 
        {
            ret = -1;
            goto _EXIT;
        }        
    }
_EXIT:
    close(sock);
    return ret;
}

#if defined( _HUB4_PRODUCT_REQ_ ) || defined( _SR300_PRODUCT_REQ_ ) || defined(_RDKB_GLOBAL_PRODUCT_REQ_)

#define NET_EXT_STATS_FILE "/proc/net/dev_extstats"
#define NET_STATS_FILE "/proc/net/dev"
int CosaUtilGetIfStats(char * ifname, PCOSA_DML_IF_STATS pStats)
{
    FILE * fp = NULL;
    char buf[1024] = {0} ;
    char *tok, *delim = ": \t\r\n", *sp = NULL, *ptr = NULL;
    int idx = 0, ret = 0;;

    if(ifname == NULL  || pStats == NULL )
        return -1;

    // check if dev_extstats exists in platform
    fp = fopen(NET_EXT_STATS_FILE, "r");

    if(fp == NULL)
    {
        fp = fopen(NET_STATS_FILE, "r");
        // both files are not present . Return error
        if(fp == NULL)
            return -1;
    }

    // skip first row
    if (fgets(buf, sizeof(buf), fp) == NULL)
    {
        fclose(fp);
        return -1;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        if(ifname[0] == '\0')
            break;

        if ((strstr(buf, (char *)ifname) == NULL))
            continue;
        // found an interface entry in /proc/net/dev_extstats or /proc/net/dev
        // fetch all statistics by iterating each column using delim

        for (idx = 1, ptr = buf; (tok = strtok_r(ptr, delim, &sp)) != NULL; idx++)
        {
            ptr = NULL;
            switch (idx)
            {
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
                case 9:
                    pStats->MulticastPacketsReceived = (ULONG)atol(tok);
                    break;
                case 18:
                    pStats->MulticastPacketsSent = (ULONG)atol(tok);
                    break;
                case 21:
                    // required for cloud integration
                    pStats->UnicastPacketsReceived = (ULONG)atol(tok);
                    break;
                case 22:
                    // required for cloud integration
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
                default:
                    break;
            }
        } // for
        // fetched all stats values for the interface
        ret = TRUE;
        break;
    }// while
    fclose(fp);

    return ret;
}

#else // other platforms

/*
   Note: this function duplicates getIfStats2(), except that in this
   case the results are returned in a COSA_DML_IF_STATS struct and in
   getIfStats2() results are returned in a COSA_DML_ETH_STATS struct.
   The return values are encoded differently too.
*/
int CosaUtilGetIfStats (char *ifname, PCOSA_DML_IF_STATS pStats)
{
    FILE *fp;
    char buf[512];
    char *device = ifname;
    int device_len;
    int result = -1;

    /*
       The data types in COSA_DML_IF_STATS are currently ULONG, which is
       not enough to hold 64bit byte and packet counter values. As a
       workaround, use local variables to hold those values during parsing.
       Note that fixing this isn't a simple change as the data types in
       COSA_DML_IF_STATS reflect those used in the data model (ie we need
       to change the data model in order to return 64bit byte and packet
       counters...).
    */
    unsigned long long rx_bytes;
    unsigned long long rx_packets;
    unsigned long long tx_bytes;
    unsigned long long tx_packets;

    memset (pStats, 0, sizeof(COSA_DML_IF_STATS));

    if ((fp = fopen ("/proc/net/dev", "r")) == NULL)
        return -1;
    if (fgets (buf, sizeof(buf), fp) == NULL)
        goto done;
    if (fgets (buf, sizeof(buf), fp) == NULL)
        goto done;

    device_len = strlen (device);

    while (fgets (buf, sizeof(buf), fp) != NULL) {
        char *p = buf;
        while ((*p == ' ') || (*p == '\t'))
            p++;
        if (strncmp (p, device, device_len) != 0)
            continue;
        p += device_len;
        if (*p++ != ':')
            continue;

        if (sscanf (p, "%llu%llu%lu%lu%*u%*u%*u%lu%llu%llu%lu%lu",
                       &rx_bytes, &rx_packets, &pStats->ErrorsReceived, &pStats->DiscardPacketsReceived, &pStats->MulticastPacketsReceived,
                       &tx_bytes, &tx_packets, &pStats->ErrorsSent, &pStats->DiscardPacketsSent) == 9)
        {
            pStats->BytesSent       = (ULONG) tx_bytes;      /* Truncate !! */
            pStats->BytesReceived   = (ULONG) rx_bytes;      /* Truncate !! */
            pStats->PacketsSent     = (ULONG) tx_packets;    /* Truncate !! */
            pStats->PacketsReceived = (ULONG) rx_packets;    /* Truncate !! */
            result = 0;
        }

        break;
    }

done:
    fclose (fp);

    return (result == 0) ? TRUE : 0;	/* map result to return value (TRUE for success, 0 for failure) */
}
#endif

ULONG NetmaskToNumber(char *netmask)
{
    char * pch;
    ULONG val;
    ULONG i;
    ULONG count = 0;
    char *st = NULL;

    pch = strtok_r(netmask, ".", &st);
    while (pch != NULL)
    {
        val = atoi(pch);
        for (i=0;i<8;i++)
        {
            if (val&0x01)
            {
                count++;
            }
            val = val >> 1;
        }
        pch = strtok_r(NULL,".", &st);
    }
    return count;
}

ANSC_STATUS
CosaUtilGetStaticRouteTable
    (
        UINT                        *count,
        StaticRoute                 **out_sroute
    )
{
    UNREFERENCED_PARAMETER(count);
    UNREFERENCED_PARAMETER(out_sroute);
    return ANSC_STATUS_SUCCESS;

}

#define IPV6_ADDR_LOOPBACK      0x0010U
#define IPV6_ADDR_LINKLOCAL     0x0020U
#define IPV6_ADDR_SITELOCAL     0x0040U
#define IPV6_ADDR_COMPATv4      0x0080U
#define IPV6_ADDR_SCOPE_MASK    0x00f0U

#if  !defined(_COSA_SIM_)

/* function: is_usg_in_bridge_mode
   description: judge if USG is in bridge mode or not
   input: none
   output: TRUE in bridge mode, otherwise FALSE
   return: ANSC_STATUS_SUCCESS/ANSC_STATUS_FAILURE
 */
ANSC_STATUS is_usg_in_bridge_mode(BOOL *pBridgeMode)
{
    char retVal[128] = {'\0'};
    ULONG retLen;
        retLen = sizeof( retVal );
    if (pBridgeMode == NULL)
        return ANSC_STATUS_FAILURE;

    if (0 == CosaGetParamValueString(
                "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.1.LanMode",
                retVal,
                &retLen)){
        if (strcmp(retVal, "router") == 0)
            *pBridgeMode = FALSE;
        else
            *pBridgeMode = TRUE;
        return ANSC_STATUS_SUCCESS;
    }else
        return ANSC_STATUS_FAILURE;

}

/*caller must free(*pp_info)*/
#define _PROCNET_IFINET6  "/proc/net/if_inet6"
#define MAX_INET6_PROC_CHARS 200

typedef struct v6sample {
           unsigned int bitsToMask;
           char intrName[20];
           unsigned char ipv6_addr[40];
           char address6[40];
           unsigned int devIndex;
           unsigned int flags;
           unsigned int scopeofipv6;
           char prefix_v6[40];
}ifv6Details;

int getIpv6Scope(int scope_v6)
{
    int scopeToReturn = scope_v6 & IPV6_ADDR_SCOPE_MASK;

            if(scopeToReturn == 0)
                return IPV6_ADDR_SCOPE_GLOBAL;                          
            else if( scopeToReturn == IPV6_ADDR_LINKLOCAL)
                return IPV6_ADDR_SCOPE_LINKLOCAL;
            else if( scopeToReturn == IPV6_ADDR_SITELOCAL)
                return IPV6_ADDR_SCOPE_SITELOCAL;
            else if( scopeToReturn == IPV6_ADDR_COMPATv4)
                return IPV6_ADDR_SCOPE_COMPATv4;
            else if( scopeToReturn == IPV6_ADDR_LOOPBACK)
                return IPV6_ADDR_SCOPE_LOOPBACK;
            else
                return IPV6_ADDR_SCOPE_UNKNOWN;
}

int parseProcfileParams(char* lineToParse,ifv6Details *detailsToParse,char* interface)
{

    struct sockaddr_in6 sAddr6;
    char splitv6[8][5];
    errno_t  rc  = -1;
    CcspTraceDebug(("%s, Parse the line read from file\n",__FUNCTION__));

    if (lineToParse == NULL)
           return 0;

    if(sscanf(lineToParse, "%39s %x %x %x %x %19s", detailsToParse->ipv6_addr,&detailsToParse->devIndex,
              &detailsToParse->bitsToMask,&detailsToParse->scopeofipv6,&detailsToParse->flags,detailsToParse->intrName) == 6)
    {

       CcspTraceDebug(("%s, Check if interface matches\n",__FUNCTION__));
       if (!strcmp(interface, detailsToParse->intrName))
       {
           CcspTraceDebug(("%s,Interface matched\n",__FUNCTION__));
           //Convert the raw interface ip to IPv6 format
           int position,placeholder=0;
           for (position=0; detailsToParse->ipv6_addr[position] != '\0'; position++)
           {
               detailsToParse->address6[placeholder] = detailsToParse->ipv6_addr[position];
               placeholder++;
               // Positions at which ":" should be put.
               if((position==3)||(position==7)||(position==11)||(position==15)||
                   (position==19)||(position==23)||(position==27))
               {
                   detailsToParse->address6[placeholder] = ':';
                   placeholder++;
               }
           }
           detailsToParse->address6[placeholder] = '\0';
           CcspTraceDebug(("%s,Interface IPv6 address calculation\n",__FUNCTION__));
           CcspTraceDebug(("IPv6 address is %s \n",detailsToParse->address6));
           inet_pton(AF_INET6, detailsToParse->address6,(struct sockaddr *) &sAddr6.sin6_addr);
           sAddr6.sin6_family = AF_INET6;
           inet_ntop(AF_INET6, (struct sockaddr *) &sAddr6.sin6_addr, detailsToParse->address6, sizeof(detailsToParse->address6));
           CcspTraceDebug(("%s,Interface IPv6 address is: %s\n",__FUNCTION__,detailsToParse->address6));

           if(sscanf(lineToParse, "%4s%4s%4s%4s%4s%4s%4s%4s", splitv6[0], splitv6[1], splitv6[2],
                                                              splitv6[3], splitv6[4],splitv6[5], splitv6[6], splitv6[7])==8)
           {
               memset(detailsToParse->prefix_v6,0,sizeof(detailsToParse->prefix_v6));
               UINT iCount =0;
               for (iCount=0; (iCount< ( detailsToParse->bitsToMask%16 ? (detailsToParse->bitsToMask/16+1):detailsToParse->bitsToMask/16)) && iCount<8; iCount++)
               {
                  rc = sprintf_s(detailsToParse->prefix_v6+strlen(detailsToParse->prefix_v6),sizeof(detailsToParse->prefix_v6), "%s:",splitv6[iCount]);
                  if(rc < EOK)
                  {
                    ERR_CHK(rc);
                    return 0;
                  }
               }
               CcspTraceDebug(("%s,Interface IPv6 prefix calculation done\n",__FUNCTION__));
            }
            return 1;
      }
      else
      {
         CcspTraceDebug(("%s,Interface not found\n",__FUNCTION__));
         return 0;
      }
    }
    else
    {
      CcspTraceError(("%s,Interface line read failed\n",__FUNCTION__));
      return 0;
    }
}
int CosaUtilGetIpv6AddrInfo (char * ifname, ipv6_addr_info_t ** pp_info, int * p_num)
{
    FILE * fp = NULL;
    ipv6_addr_info_t * p_ai = NULL;
    char procLine[MAX_INET6_PROC_CHARS];
    ifv6Details v6Details;
    int parsingResult;
    errno_t rc = -1;
    memset(v6Details.prefix_v6, 0, sizeof(v6Details.prefix_v6));
  
    if (!ifname || !pp_info || !p_num)
        return -1;

    *p_num = 0;

    fp = fopen(_PROCNET_IFINET6, "r");
    if (!fp)
        return -1;
    
    while(fgets(procLine, MAX_INET6_PROC_CHARS, fp))
    {

        parsingResult=parseProcfileParams(procLine, &v6Details,ifname);
        if (parsingResult == 1)
        { 
            (*p_num)++;
            *pp_info = realloc(*pp_info,  *p_num * sizeof(ipv6_addr_info_t));
            if (!*pp_info) 
            {
                fclose(fp);
                return -1;
            }

            p_ai = &(*pp_info)[*p_num-1];
            strncpy(p_ai->v6addr, v6Details.address6, sizeof(p_ai->v6addr));

            // Get the scope of IPv6
            p_ai->scope = getIpv6Scope(v6Details.scopeofipv6);
            CcspTraceInfo(("%s,Interface scope is : %d\n",__FUNCTION__,v6Details.scopeofipv6));           
 
            memset(p_ai->v6pre, 0, sizeof(p_ai->v6pre));
            /*CID: 64940 - Array Compared against null - fixed*/
            if(v6Details.prefix_v6[0] != '\0')
            {
                 rc = strcpy_s(p_ai->v6pre, sizeof(p_ai->v6pre),v6Details.prefix_v6);
                 if(rc != EOK)
                 {
                    ERR_CHK(rc);
                    fclose(fp);
                    return -1;
                 }

            }
            else
            {
                CcspTraceInfo(("%s,Interface ipv6 prefix is NULL\n",__FUNCTION__));
            }

            rc = sprintf_s(p_ai->v6pre+strlen(p_ai->v6pre), sizeof(p_ai->v6pre)-strlen(p_ai->v6pre),":/%d", v6Details.bitsToMask);
            if(rc < EOK)
            {
               ERR_CHK(rc);
               fclose(fp);
               return -1;
            }
            
        }
    }

    fclose(fp);
    return 0;
}

#else
ANSC_STATUS is_usg_in_bridge_mode(BOOL *pBridgeMode)
{
    *pBridgeMode = FALSE;
    return ANSC_STATUS_SUCCESS;
}

int CosaUtilGetIpv6AddrInfo (char * ifname, ipv6_addr_info_t ** pp_info, int * p_num)
{
    *p_num = 0;
    *pp_info = NULL;
    return 0;
}
#endif

char *safe_strcpy (char *dst, char *src, size_t dst_size)
{
    size_t len;

    if (dst_size == 0)
        return dst;

    len = strlen (src);

    if (len >= dst_size)
    {
        dst[dst_size - 1] = 0;
        return memcpy (dst, src, dst_size - 1);
    }

    return memcpy (dst, src, len + 1);
}

int  __v6addr_mismatch(char * addr1, char * addr2, int pref_len)
{
    int i = 0;
    int num = 0;
    int mask = 0;
    char addr1_buf[128] = {0};
    char addr2_buf[128] = {0};
    struct in6_addr in6_addr1;
    struct in6_addr in6_addr2;
    errno_t     rc  = -1;

    if (!addr1 || !addr2)
        return -1;

    rc = strcpy_s(addr1_buf, sizeof(addr1_buf), addr1);
    if(rc != EOK)
    {
       ERR_CHK(rc);
       return -1;
    }

    rc = strcpy_s(addr2_buf, sizeof(addr2_buf), addr2);
    if(rc != EOK)
    {
       ERR_CHK(rc);
       return -1;
    }

    if ( inet_pton(AF_INET6, addr1_buf, &in6_addr1) != 1)
        return -8;
    if ( inet_pton(AF_INET6, addr2_buf, &in6_addr2) != 1)
        return -9;

    num = (pref_len%8)? (pref_len/8+1) : pref_len/8;
    if (pref_len%8)
    {
        for (i=0; i<num-1; i++)
            if (in6_addr1.s6_addr[i] != in6_addr2.s6_addr[i])
                return -3;

        for (i=0; i<pref_len%8; i++)
            mask += 1<<(7-i);
        
        if ( (in6_addr1.s6_addr[num-1] &  mask) == (in6_addr2.s6_addr[num-1] & mask))
            return 0;
        else
            return -4;
    }
    else 
    {
        for (i=0; i<num; i++)
            if (in6_addr1.s6_addr[i] != in6_addr2.s6_addr[i])
                return -5;
    }

    return 0;
}

int  __v6addr_mismatches_v6pre(char * v6addr,char * v6pre)
{
    int pref_len = 0;
    char addr_buf[128] = {0};
    char pref_buf[128] = {0};
    char * p = NULL;
    errno_t rc = -1;

    if (!v6addr || !v6pre)
        return -1;
    
    rc = strcpy_s(addr_buf, sizeof(addr_buf), v6addr);
    if(rc != EOK)
    {
       ERR_CHK(rc);
       return -1;
    }

    rc = strcpy_s(pref_buf, sizeof(pref_buf), v6pre);
    if(rc != EOK)
    {
       ERR_CHK(rc);
       return -1;
    }

    if (!(p = strchr(pref_buf, '/')))
        return -1;

    if (sscanf(p, "/%d", &pref_len) != 1)
        return -2;
    *p = 0;

    return __v6addr_mismatch(addr_buf, pref_buf, pref_len);
}

int  __v6pref_mismatches(char * v6pref1,char * v6pref2)
{
    int pref1_len = 0;
    int pref2_len = 0;
    char pref1_buf[128] = {0};
    char pref2_buf[128] = {0};
    char * p = NULL;
    errno_t rc = -1;

    if (!v6pref1 || !v6pref2)
        return -1;
    
    rc = strcpy_s(pref1_buf, sizeof(pref1_buf), v6pref1);
    if(rc != EOK)
    {
       ERR_CHK(rc);
       return -1;
    }

    rc = strcpy_s(pref2_buf, sizeof(pref2_buf), v6pref2);
    if(rc != EOK)
    {
       ERR_CHK(rc);
       return -1;
    }

    if (!(p = strchr(pref1_buf, '/')))
        return -1;

    if (sscanf(p, "/%d", &pref1_len) != 1)
        return -2;
    *p = 0;

    if (!(p = strchr(pref2_buf, '/')))
        return -1;

    if (sscanf(p, "/%d", &pref2_len) != 1)
        return -2;
    *p = 0;

    if (pref1_len != pref2_len)
        return -7;

    return __v6addr_mismatch(pref1_buf, pref2_buf, pref1_len);
}

int CosaDmlV6AddrIsEqual(char * p_addr1, char * p_addr2)
{
    if (!p_addr1 || !p_addr2)
        return 0;

    return !__v6addr_mismatch(p_addr1, p_addr2, 128);
}

int CosaDmlV6PrefIsEqual(char * p_pref1, char * p_pref2)
{
    if (!p_pref1 || !p_pref2)
        return 0;

    return !__v6pref_mismatches(p_pref1, p_pref2);
}

int _write_sysctl_file(const char *filename, int value)
{
    char buf[12];
    size_t len;
    int fd;

    if ((fd = open(filename, O_WRONLY)) < 0) {
        perror("Failed to open file");
        return -1;
    }

    len = sprintf(buf, "%d", value);
    if (write(fd, buf, len) != (ssize_t) len) {
        perror("Failed to write to file");
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}

/*
 * IPV4 Address check functions
 */
/* addr is in network order */
int IPv4Addr_IsSameNetwork(uint32_t addr1, uint32_t addr2, uint32_t mask)
{
    return (addr1 & mask) == (addr2 & mask);
}

/* addr is in network order */
int IPv4Addr_IsLoopback(uint32_t addr)
{
    return (addr & htonl(0xff000000)) == htonl(0x7f000000);
}

/* addr is in network order */
int IPv4Addr_IsMulticast(uint32_t addr)
{
    return (addr & htonl(0xf0000000)) == htonl(0xe0000000);
}

/* addr is in network order */
int IPv4Addr_IsBroadcast(uint32_t addr, uint32_t net, uint32_t mask)
{
    /* all ones or all zeros (old) */
    if (addr == 0xffffffff)
        return 1;

    /* on the same sub network and host bits are all ones */
    if (IPv4Addr_IsSameNetwork(addr, net, mask)
            && (addr & ~mask) == (0xffffffff & ~mask))
        return 1;

    return 0;
}

/* addr is in network order */
int IPv4Addr_IsNetworkAddr(uint32_t addr, uint32_t net, uint32_t mask)
{
    if (IPv4Addr_IsSameNetwork(addr, net, mask)
            && (addr & ~mask) == 0)
        return 1;

    return 0;
}

/* addr is in network order */
int IPv4Addr_IsNetmaskValid(uint32_t netmask)
{
    uint32_t mask;
    uint32_t hostorder = ntohl(netmask);

    /* first zero */
    for (mask = 1UL << 31 ; mask != 0; mask >>= 1)
        if ((hostorder & mask) == 0)
            break;

    /* there is no one ? */
    for (; mask != 0; mask >>= 1)
        if ((hostorder & mask) != 0)
            return 0;

    return 1;
}

int IPv4Addr_IsClassA(uint32_t addr)
{
    return IPV4_CLASSA(ntohl(addr));
}

int IPv4Addr_IsClassB(uint32_t addr)
{
    return IPV4_CLASSB(ntohl(addr));
}

int IPv4Addr_IsClassC(uint32_t addr)
{
    return IPV4_CLASSC(ntohl(addr));
}

int IPv4Addr_IsClassD(uint32_t addr)
{
    return IPV4_CLASSD(ntohl(addr));
}

int vsystem(const char *fmt, ...)
{
    char cmd[512];
    va_list ap;
    ULONG n;

    va_start(ap, fmt);
    n = vsnprintf(cmd, sizeof(cmd), fmt, ap);
    va_end(ap);

    if (n >= sizeof(cmd))
        return -1;

    fprintf(stderr, "%s: %s\n", __FUNCTION__, cmd);
    return system(cmd);
}

int get_if_hwaddr(const char *ifname, char *mac, size_t size)
{
    int sockfd;
    struct ifreq ifr;
    unsigned char *ptr;

    if (!ifname || !mac || size < sizeof("00:00:00:00:00:00"))
        return -1;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", ifname);

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(sockfd);
        return -1;
    }

    ptr = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    snprintf(mac, size, "%02x:%02x:%02x:%02x:%02x:%02x",
            ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

    close(sockfd);
    return 0;
}

#ifdef IPV4ADDR_TEST

#define NEXTARG    argc--, argv++

enum {
    CMD_ISLOOP,
    CMD_ISMULT,
    CMD_COMP,
    CMD_ISBORAD,
    CMD_ISNET,
};

struct arguments {
    int             cmd;
    struct in_addr  addr;
    struct in_addr  net;
    struct in_addr  mask;
};

static uint32_t InaddrGetU32(struct in_addr *addr)
{
    return (uint32_t)addr->s_addr;
}

static void Usage(void)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "    ./v4addr l ADDR\n");
    fprintf(stderr, "    ./v4addr m ADDR\n");
    fprintf(stderr, "    ./v4addr c ADDR1 ADDR2 MASK\n");
    fprintf(stderr, "    ./v4addr b ADDR NET MASK\n");
    fprintf(stderr, "    ./v4addr n ADDR NET MASK\n");
}

static int ParseArgs(int argc, char *argv[], struct arguments *args)
{
    if (argc < 3) {
        Usage();
        exit(1);
    }

    bzero(args, sizeof(*args));

    NEXTARG;
    if (strcmp(*argv, "l") == 0) {
        args->cmd = CMD_ISLOOP;

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->addr) <= 0) {
            fprintf(stderr, "invalid addr\n");
            exit(1);
        }
    } else if (strcmp(*argv, "m") == 0) {
        args->cmd = CMD_ISMULT;

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->addr) <= 0) {
            fprintf(stderr, "invalid addr\n");
            exit(1);
        }
    } else if (strcmp(*argv, "c") == 0
            || strcmp(*argv, "b") == 0
            || strcmp(*argv, "n") == 0) {

        if (strcmp(*argv, "c") == 0)
            args->cmd = CMD_COMP;
        else if (strcmp(*argv, "b") == 0)
            args->cmd = CMD_ISBORAD;
        else if (strcmp(*argv, "n") == 0)
            args->cmd = CMD_ISNET;

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->addr) <= 0) {
            fprintf(stderr, "invalid addr\n");
            exit(1);
        }

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->net) <= 0) {
            fprintf(stderr, "invalid addr2/net\n");
            exit(1);
        }

        NEXTARG;
        if (inet_pton(AF_INET, *argv, &args->mask) <= 0
                || !IPv4Addr_IsNetmaskValid(InaddrGetU32(&args->mask))) {
            fprintf(stderr, "invalid mask\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "unknow command `%s'\n", *argv);
        exit(1);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    struct arguments args;

    if (ParseArgs(argc, argv, &args) != 0)
        exit(1);

    switch (args.cmd) {
    case CMD_ISLOOP:
        if (IPv4Addr_IsLoopback(InaddrGetU32(&args.addr)))
            AnscTraceFlow(("Is loopback\n"));
        else
            AnscTraceFlow(("Not loopback\n"));
        break;

    case CMD_ISMULT:
        if (IPv4Addr_IsMulticast(InaddrGetU32(&args.addr)))
            AnscTraceFlow(("Is multicast\n"));
        else
            printf("Not multicast\n");
        break;

    case CMD_COMP:
        if (IPv4Addr_IsSameNetwork(InaddrGetU32(&args.addr), 
                    InaddrGetU32(&args.net), InaddrGetU32(&args.mask)))
            printf("Is in same network\n");
        else
            printf("Not in same network\n");
        break;

    case CMD_ISBORAD:
        if (IPv4Addr_IsBroadcast(InaddrGetU32(&args.addr), 
                    InaddrGetU32(&args.net), InaddrGetU32(&args.mask)))
            printf("Is broadcast\n");
        else
            printf("Not broadcast\n");
        break;

    case CMD_ISNET:
        if (IPv4Addr_IsNetworkAddr(InaddrGetU32(&args.addr), 
                    InaddrGetU32(&args.net), InaddrGetU32(&args.mask)))
            printf("Is network address\n");
        else
            printf("Not network address\n");
        break;

    default:
        Usage();
        exit(1);
    }

    return 0;
}

#endif
