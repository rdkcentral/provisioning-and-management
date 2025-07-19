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

    module: cosa_ipv6rd_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        leichen2@cisco.com

    -------------------------------------------------------------------

    revision:

        08/04/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "cosa_ipv6rd_dml.h"
#include "cosa_ipv6rd_internal.h"
#include "cosa_ipv6rd_apis.h"
#include "dml_tr181_custom_cfg.h"
#include "cosa_deviceinfo_apis.h"
#include "safec_lib_common.h"

extern void* g_pDslhDmlAgent;

/***********************************************************************
 IMPORTANT NOTE:

 According to TR69 spec:
 On successful receipt of a SetParameterValues RPC, the CPE MUST apply 
 the changes to all of the specified Parameters atomically. That is, either 
 all of the value changes are applied together, or none of the changes are 
 applied at all. In the latter case, the CPE MUST return a fault response 
 indicating the reason for the failure to apply the changes. 
 
 The CPE MUST NOT apply any of the specified changes without applying all 
 of them.

 In order to set parameter values correctly, the back-end is required to
 hold the updated values until "Validate" and "Commit" are called. Only after
 all the "Validate" passed in different objects, the "Commit" will be called.
 Otherwise, "Rollback" will be called instead.

 The sequence in COSA Data Model will be:

 SetParamBoolValue/SetParamIntValue/SetParamUlongValue/SetParamStringValue
 -- Backup the updated values;

 if( Validate_XXX())
 {
     Commit_XXX();    -- Commit the update all together in the same object
 }
 else
 {
     Rollback_XXX();  -- Remove the update at backup;
 }
 
***********************************************************************/

BOOL
IPv6rd_GetParamBoolValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        BOOL *pBool)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IPV6RD pEntry = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;

    if (!pEntry)
    {
        return FALSE;
    }

    if (strcmp(ParamName, "Enable") == 0)
    {
        CosaDml_IPv6rdGetEnable(NULL, NULL, &pEntry->Enable);
        *pBool = pEntry->Enable;
        return TRUE;
    }

    return FALSE;
}

BOOL
IPv6rd_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IPV6RD pEntry = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;

    if (!pEntry)
    {
        return FALSE;
    }

    if (strcmp(ParamName, "InterfaceSettingNumberOfEntries") == 0)
    {
        *pUlong = pEntry->NumOfInterface;
        return TRUE;
    }

    return FALSE;
}

BOOL
IPv6rd_SetParamBoolValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        BOOL        bValue
        )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IPV6RD pEntry = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;

    if (!pEntry)
        return FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pEntry->Enable = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
IPv6rd_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(ulValue);
    PCOSA_DATAMODEL_IPV6RD pEntry = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;

    if (!pEntry)
        return FALSE;

    return FALSE;
}

BOOL
IPv6rd_Validate(
        ANSC_HANDLE hInsContext,
        char        *pReturnParamName,
        ULONG       *puLength
        )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    /* TODO: */
    return TRUE;
}

ULONG
IPv6rd_Commit(
        ANSC_HANDLE hInsContext
        )
{
    UNREFERENCED_PARAMETER(hInsContext);

    PCOSA_DATAMODEL_IPV6RD pEntry = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;

    if (CosaDml_IPv6rdSetEnable(NULL, NULL, pEntry->Enable) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("IPv6rd_Commit: CosaDml_IPv6rdSetEnable error\n"));
        /* return -1; */
    }

    return 0;
}

ULONG
IPv6rd_Rollback(
        ANSC_HANDLE hInsContext
        )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IPV6RD pEntry = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;

    if (CosaDml_IPv6rdGetEnable(NULL, NULL, &pEntry->Enable) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("IPv6rd_Rollback: CosaDml_IPv6rdGetEnable error\n"));
        /* return -1; */
    }

    return 0;
}

ULONG
IPv6rdIF_GetEntryCount(
        ANSC_HANDLE hInsContext)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IPV6RD  pMyObject = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;

    if (!pMyObject)
        return -1;

    return AnscSListQueryDepth(&pMyObject->IfList);
}

ANSC_HANDLE
IPv6rdIF_GetEntry(
        ANSC_HANDLE hInsContext,
        ULONG       nIndex,
        ULONG       *pInsNumber)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IPV6RD  pMyObject = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;
    PCOSA_CONTEXT_LINK_OBJECT   pLinkObject = NULL;
    PSINGLE_LINK_ENTRY      pSLinkEntry = NULL;
    PCOSA_DML_IPV6RD_IF     pEntry = NULL;

    if (!pMyObject)
        return NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(&pMyObject->IfList, nIndex);
	if (!pSLinkEntry)
		return NULL;

    pLinkObject = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
    if (!pLinkObject)
        return NULL;

    *pInsNumber = pLinkObject->InstanceNumber;

    pEntry = pLinkObject->hContext;
    if (!pEntry)
        return NULL;

    if (CosaDml_IPv6rdGetEntry(NULL, pEntry->InstanceNumber, pEntry) != ANSC_STATUS_SUCCESS)
    {
        /*
         * It may happen object is add, but parameters are not set, 
         * so node may save some where and load when Init, 
         * but for backend (PSM) this instance is not exist.
         */
        CcspTraceError(("IPv6rdIF_GetEntry: !!!!!!!! CosaDml_IPv6rdGetEntry error, "
                    "but also return pLinkObject for edit later (it's alloced)\n"));
        /* memset(pEntry, 0, sizeof(COSA_DML_IPV6RD_IF)); */
        /* don't return NULL */
    }

    return pLinkObject;
}

ANSC_HANDLE
IPv6rdIF_AddEntry(
        ANSC_HANDLE hInsContext,
        ULONG       *pInsNumber)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IPV6RD      pMyObject   = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;
    PSLIST_HEADER               pListHead   = (PSLIST_HEADER)&pMyObject->IfList;
    PCOSA_DML_IPV6RD_IF         pEntry      = NULL;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext = NULL;
    errno_t                     rc           = -1;

    pEntry = (PCOSA_DML_IPV6RD_IF)AnscAllocateMemory(sizeof(COSA_DML_IPV6RD_IF));
    if (!pEntry)
    {
        return NULL;
    }

    rc = sprintf_s(pEntry->Alias, sizeof(pEntry->Alias),"tun6rd%lu", pMyObject->ulIfNextInstance);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pEntry);
      return NULL;
    }
#if !CFG_TR181_NO_CosaDml_Ifname2Addr
	CosaDml_Ifname2Addr(CFG_TR181_6rd_IfName, pEntry->AddressSource, sizeof(pEntry->AddressSource));
#endif
    rc = strcpy_s(pEntry->Status, sizeof(pEntry->Status),"Disabled");
    if(rc != EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pEntry);
      return NULL;
    }

    pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pCosaContext)
    {
        AnscFreeMemory(pEntry);
        return NULL;
    }

    pCosaContext->InstanceNumber = pEntry->InstanceNumber = pMyObject->ulIfNextInstance;
    pMyObject->ulIfNextInstance++;

    if (pMyObject->ulIfNextInstance == 0)
    {
        pMyObject->ulIfNextInstance = 1;
    }

    pCosaContext->hContext      = (ANSC_HANDLE)pEntry;
    pCosaContext->hParentTable  = NULL;
    pCosaContext->bNew          = TRUE;

    CosaSListPushEntryByInsNum(pListHead, pCosaContext);

    CosaIPv6rdIfRegAddInfo((ANSC_HANDLE)pMyObject, pCosaContext);

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}

ULONG
IPv6rdIF_DelEntry(
        ANSC_HANDLE hInsContext,
        ULONG       hInstance)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IPV6RD      pMyObject = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext2 = NULL;
    PCOSA_DML_IPV6RD_IF         pEntry = (PCOSA_DML_IPV6RD_IF)pCosaContext->hContext;
    PCOSA_DML_IPV6RD_IF         pNewEntry = NULL;
    PSLIST_HEADER               pListHead = (PSLIST_HEADER)&pMyObject->IfList;
    PSINGLE_LINK_ENTRY          pSLinkEntry = NULL;

    if (CosaDml_IPv6rdDelEntry(NULL, pEntry->InstanceNumber) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("IPv6rdIF_DelEntry: CosaDml_IPv6rdDelEntry error\n"));
        /* may fail if object is add be PSM config is not set.
         * so do not return here */
        /* return ANSC_STATUS_FAILURE; */
    }

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->IfList);
    while (pSLinkEntry)
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        pNewEntry = (PCOSA_DML_IPV6RD_IF)pCosaContext2->hContext;
        if (pNewEntry && AnscEqualString(pNewEntry->Alias, pEntry->Alias, FALSE))
        {
            AnscSListPopEntryByLink(pListHead, &pCosaContext2->Linkage);

			if (pCosaContext->bNew)
	            CosaIPv6rdIfRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCosaContext2);

            AnscFreeMemory(pNewEntry);
            AnscFreeMemory(pCosaContext2);
            break;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

BOOL
IPv6rdIF_GetParamBoolValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        BOOL *pBool)
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IPV6RD_IF             pEntry = (PCOSA_DML_IPV6RD_IF)NULL;

    if (!pLinkObject)
        return FALSE;

    pEntry = (PCOSA_DML_IPV6RD_IF)pLinkObject->hContext; /*RDKB-6740, CID-33461, null must checked before use*/
    if (!pEntry)
        return FALSE;

    CosaDml_IPv6rdGetEntry(NULL, pEntry->InstanceNumber, pEntry);

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pEntry->Enable;
        return TRUE;
    }
    else if (strcmp(ParamName, "AllTrafficToBorderRelay") == 0)
    {
        *pBool = pEntry->AllTrafficToBorderRelay;
        return TRUE;
    }

    return FALSE;
}

ULONG
IPv6rdIF_GetParamStringValue(
        ANSC_HANDLE     hInsContext,
        char            *ParamName,
        char            *pValue,
        ULONG           *pSize)
{
    UNREFERENCED_PARAMETER(pSize);
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IPV6RD_IF             pEntry = (PCOSA_DML_IPV6RD_IF)NULL;
    PUCHAR path = NULL;
    char tmp[128];
    errno_t  rc = -1;

    if (!pLinkObject)
        return FALSE;

    pEntry = (PCOSA_DML_IPV6RD_IF)pLinkObject->hContext; /*RDKB-6740, CID-33563, null must checked before use*/
    if (!pEntry)
        return FALSE;

    CosaDml_IPv6rdGetEntry(NULL, pEntry->InstanceNumber, pEntry);
    if (strcmp(ParamName, "Status") == 0)
    {
      rc = strcpy_s(pValue,*pSize, pEntry->Status);
      if(rc != EOK)
      {
         ERR_CHK(rc);
         return -1;
      }
    }
    else if (strcmp(ParamName, "Alias") == 0)
    {
       rc = strcpy_s(pValue,*pSize, pEntry->Alias);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return -1;
       }
    }
    else if (strcmp(ParamName, "BorderRelayIPv4Addresses") == 0)
    {
       rc = strcpy_s(pValue,*pSize, pEntry->BorderRelayIPv4Addr);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return -1;
       }
    }
    else if (strcmp(ParamName, "SPIPv6Prefix") == 0)
    {
       rc = strcpy_s(pValue,*pSize, pEntry->SPIPv6Prefix);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return -1;
       }
    }
    else if (strcmp(ParamName, "AddressSource") == 0)
    {
        if (_ansc_strlen(pEntry->AddressSource) == 0)
        {
            pValue[0] = '\0';
            return 0;
        }

        path = CosaUtilGetFullPathNameByKeyword((PUCHAR)"Device.IP.Interface.", (PUCHAR)"Name", (PUCHAR)CFG_TR181_6rd_IfName);
        if ((path != NULL) && (_ansc_strlen((char*)path) > 0))
        {
            if (path[_ansc_strlen((char*)path) - 1] == '.')
                path[_ansc_strlen((char*)path) - 1] = '\0';
            rc = sprintf_s(tmp, sizeof(tmp),"%s.IPv4Address.", path);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(path);
              return -1;
            }
            AnscFreeMemory(path); /*RDKB-6740, CID-33386, free unused resource before exit*/
            path = NULL;
            path = CosaUtilGetFullPathNameByKeyword((PUCHAR)tmp, (PUCHAR)"IPAddress", (PUCHAR)pEntry->AddressSource);
        }

        if (path == NULL)
        {
           pValue[0] = '\0';
        }
        else
        {
           rc = strcpy_s(pValue,*pSize, (char*)path);
           if(rc != EOK)
           {
             ERR_CHK(rc);
	     // CID 178705 : Resource leak (RESOURCE_LEAK)
	     if(path)
             {
                 AnscFreeMemory(path);
                 path = NULL;
             }
             return -1;
           }
        }
        /* AnscCopyString(pValue, pEntry->AddressSource); */
    }
    else if (strcmp(ParamName, "TunnelInterface") == 0)
    {
        path = CosaUtilGetFullPathNameByKeyword((PUCHAR)"Device.IP.Interface.", 
                (PUCHAR)"Name", (PUCHAR)pEntry->TunnelInterface);
        if (path == NULL)
        {
           pValue[0] = '\0';
        }
        else
        {
           rc = strcpy_s(pValue,*pSize, (char*)path);
           if(rc != EOK)
           {
              ERR_CHK(rc);
              return -1;
           }
        }
        /* AnscCopyString(pValue, pEntry->TunnelInterface); */
    }
    else if (strcmp(ParamName, "TunneledInterface") == 0)
    {
        path = CosaUtilGetFullPathNameByKeyword((PUCHAR)"Device.IP.Interface.", 
                (PUCHAR)"Name", (PUCHAR)pEntry->TunneledInterface);
        if (path == NULL)
        {
           pValue[0] = '\0';
        }
        else
        {
           rc = strcpy_s(pValue,*pSize, (char*)path);
           if(rc != EOK)
           {
              ERR_CHK(rc);
              return -1;
           }
        }
        /* AnscCopyString(pValue, pEntry->TunneledInterface); */
    }
    else
    {
        return -1;
    }

     /*RDKB-6740, CID-33386, free unusd resource before return*/
    if(path)
    {
        AnscFreeMemory(path); /*RDKB-6740, CID-33386, free unused resource before exit*/
        path = NULL;
    }

    return 0;
}

BOOL
IPv6rdIF_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong)
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IPV6RD_IF             pEntry = (PCOSA_DML_IPV6RD_IF)NULL;

    if (!pLinkObject)
        return FALSE;

    pEntry = (PCOSA_DML_IPV6RD_IF)pLinkObject->hContext; /*RDKB-6740, CID-33484, null must checked before use*/
    if (!pEntry)
        return FALSE;

    CosaDml_IPv6rdGetEntry(NULL, pEntry->InstanceNumber, pEntry);

    if (strcmp(ParamName, "IPv4MaskLength") == 0)
    {
        *pUlong = pEntry->IPv4MaskLength;
        return TRUE;
    }

    return FALSE;
}

BOOL
IPv6rdIF_SetParamBoolValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        BOOL        bValue
        )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IPV6RD_IF             pEntry = (PCOSA_DML_IPV6RD_IF)NULL;

    if (!pLinkObject)
        return FALSE;

    pEntry = (PCOSA_DML_IPV6RD_IF)pLinkObject->hContext;  /*RDKB-6740, CID-33063, null must checked before use*/
    if (!pEntry)
        return FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pEntry->Enable = bValue;
        return TRUE;
    }
    else if (strcmp(ParamName, "AllTrafficToBorderRelay") == 0)
    {
        pEntry->AllTrafficToBorderRelay = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
IPv6rdIF_SetParamStringValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        char        *pString
        )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IPV6RD_IF             pEntry = (PCOSA_DML_IPV6RD_IF)NULL;
    char v4addr[16];
    ULONG addlen;
    char tmp[128];
    ANSC_STATUS ret=ANSC_STATUS_FAILURE;
    errno_t     rc = -1;

    if (!pLinkObject)
        return FALSE;

    pEntry = (PCOSA_DML_IPV6RD_IF)pLinkObject->hContext; /*RDKB-6740, CID-33172, null must checked before use*/
    if (!pEntry)
        return FALSE;
    if (strcmp(ParamName, "Alias") == 0)
    {
		char wrapped_inputparam[256]={0};
		ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;
        CcspTraceWarning(("after isValidInput\n"));
        rc = strcpy_s(pEntry->Alias,sizeof(pEntry->Alias) ,wrapped_inputparam);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
		
        return TRUE;
    }
    else if (strcmp(ParamName, "BorderRelayIPv4Addresses") == 0)
    {
	char wrapped_inputparam[256]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;
        
	rc = strcpy_s(pEntry->BorderRelayIPv4Addr,sizeof(pEntry->BorderRelayIPv4Addr), wrapped_inputparam);
	if(rc != EOK)
	{
	    ERR_CHK(rc);
	    return FALSE;
	}
	return TRUE;
    }
    else if (strcmp(ParamName, "SPIPv6Prefix") == 0)
    {
	char wrapped_inputparam[256]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;
        
	rc = strcpy_s(pEntry->SPIPv6Prefix,sizeof(pEntry->SPIPv6Prefix), wrapped_inputparam);
	if(rc != EOK)
	{
	    ERR_CHK(rc);
	    return FALSE;
	}

	return TRUE;
    }
    else if (strcmp(ParamName, "AddressSource") == 0)
    {
	char wrapped_inputparam[256]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;
        
        if (( '\0' == wrapped_inputparam[ 0 ] ) || \
			( _ansc_strlen(wrapped_inputparam) == 0 )
			)
        {
            pEntry->AddressSource[0] = '\0';
            return TRUE;
        }

        addlen = sizeof(v4addr);
        rc = sprintf_s(tmp, sizeof(tmp),"%sIPAddress", wrapped_inputparam);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        if (g_GetParamValueString(g_pDslhDmlAgent, tmp, v4addr, &addlen) != 0)
        {
            CcspTraceWarning(("IPv6rdIF_SetParamStringValue: fail to get %s\n", tmp));
            return FALSE;
        }

        rc = strcpy_s(pEntry->AddressSource,sizeof(pEntry->AddressSource) ,v4addr);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        /* AnscCopyString(pEntry->AddressSource, pString); */
        return TRUE;
    }

    return FALSE;
}

BOOL
IPv6rdIF_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IPV6RD_IF             pEntry = (PCOSA_DML_IPV6RD_IF)NULL;

    if (!pLinkObject)
        return FALSE;

    pEntry = (PCOSA_DML_IPV6RD_IF)pLinkObject->hContext;  /*RDKB-6740, CID-33243, null must checked before use*/
    if (!pEntry)
        return FALSE;

    if (strcmp(ParamName, "IPv4MaskLength") == 0)
    {
        pEntry->IPv4MaskLength = ulValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
IPv6rdIF_Validate(
        ANSC_HANDLE hInsContext,
        char        *pReturnParamName,
        ULONG       *puLength
        )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    /* TODO: */
    return TRUE;
}

ULONG
IPv6rdIF_Commit(
        ANSC_HANDLE hInsContext
        )
{
    PCOSA_DATAMODEL_IPV6RD          pMyObject = (PCOSA_DATAMODEL_IPV6RD)g_pCosaBEManager->hIPv6rd;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IPV6RD_IF             pEntry = (PCOSA_DML_IPV6RD_IF)pLinkObject->hContext;

    if (pLinkObject->bNew)
    {
        pLinkObject->bNew = FALSE;
        if (CosaDml_IPv6rdAddEntry(NULL, pEntry) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("IPv6rdIF_Commit: CosaDml_IPv6rdAddEntry error\n"));
            /* return -1; */
        }
        CosaIPv6rdIfRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pLinkObject);
    }
    else
    {
        if (CosaDml_IPv6rdSetEntry(NULL, pEntry->InstanceNumber, pEntry) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("IPv6rdIF_Commit: CosaDml_IPv6rdSetEntry error\n"));
            /* return -1; */
        }
    }

    return 0;
}

ULONG
IPv6rdIF_Rollback(
        ANSC_HANDLE hInsContext
        )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IPV6RD_IF             pEntry = (PCOSA_DML_IPV6RD_IF)pLinkObject->hContext;

    if (CosaDml_IPv6rdGetEntry(NULL, pEntry->InstanceNumber, pEntry) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("IPv6rdIF_Rollback: CosaDml_IPv6rdGetEntry error\n"));
        /* return -1; */
    }

    return 0;
}
