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

    module: cosa_ip_dml.c

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

        01/17/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "ansc_string_util.h"
/*#include "cosa_diagnostic_apis.h"*/
#include "plugin_main_apis.h"
#include "cosa_ip_apis.h"
#include "cosa_ip_apis_multilan.h"
#include "cosa_ip_dml.h"
#include "cosa_ip_internal.h"
#include "safec_lib_common.h"
#include <syscfg/syscfg.h>

static ULONG last_tick;
#define REFRESH_INTERVAL 120
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))


#ifndef _COSA_SIM_
BOOL CosaIpifGetSetSupported(char * pParamName);
#endif

#include <net/if.h>

ANSC_STATUS COSAGetParamValueByPathName(void* bus_handle,parameterValStruct_t *val, ULONG *parameterValueLength);

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
/***********************************************************************

 APIs for Object:

    IP.

    *  IP_GetParamBoolValue
    *  IP_GetParamIntValue
    *  IP_GetParamUlongValue
    *  IP_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_GetParamBoolValue
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
IP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "IPv4Capable") == 0)
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    if (strcmp(ParamName, "IPv6Capable") == 0)
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    if (strcmp(ParamName, "IPv4Enable") == 0)
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    if (strcmp(ParamName, "IPv6Enable") == 0)
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "IPv4Status") == 0)
    {
        /* collect value */
        *puLong = COSA_DML_IPV4V6_STATUS_Enabled;
        return TRUE;
    }

    if (strcmp(ParamName, "IPv6Status") == 0)
    {
        /* collect value */
        *puLong = COSA_DML_IPV4V6_STATUS_Enabled;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IP_GetParamStringValue
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
IP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pUlSize);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "ULAPrefix") == 0)
    {
        /* collect value */
        *pValue = 0;
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_SetParamBoolValue
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
IP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(bValue);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "IPv4Enable") == 0)
    {
        /* not supported */
        return FALSE;
    }

    if (strcmp(ParamName, "IPv6Enable") == 0)
    {
        /* not supported */
        return FALSE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IP_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IP_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(uValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pString);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "ULAPrefix") == 0)
    {
        /* not supported */
        return FALSE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
IP_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IP_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IP_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IP_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.

    *  Interface2_GetEntryCount
    *  Interface2_GetEntry
    *  Interface2_AddEntry
    *  Interface2_DelEntry
    *  Interface2_GetParamBoolValue
    *  Interface2_GetParamIntValue
    *  Interface2_GetParamUlongValue
    *  Interface2_GetParamStringValue
    *  Interface2_SetParamBoolValue
    *  Interface2_SetParamIntValue
    *  Interface2_SetParamUlongValue
    *  Interface2_SetParamStringValue
    *  Interface2_Validate
    *  Interface2_Commit
    *  Interface2_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface2_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
Interface2_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IP              pMyObject = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;

    return AnscSListQueryDepth(&pMyObject->InterfaceList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Interface2_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
Interface2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PSLIST_HEADER                   pIPIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(pIPIFHead, nIndex);

    if ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pCosaContext->InstanceNumber;
    }
    
    return pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Interface2_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/
ANSC_HANDLE
Interface2_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PSLIST_HEADER                   pIPIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)NULL;
    errno_t                         rc            = -1;

#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported("addentry"))
    {
        return NULL;
    }
#endif
#endif
    pIPInterface = (PCOSA_DML_IP_IF_FULL2)AnscAllocateMemory(sizeof(COSA_DML_IP_IF_FULL2));

    if ( !pIPInterface )
    {
        return NULL;
    }
#if defined (MULTILAN_FEATURE)
    /*
     * Adding default Interface type
     */
    pIPInterface->Cfg.IfType   = COSA_DML_IP_IF_TYPE_Normal;
#endif

#if defined (INTEL_PUMA7)
    /*
     * Adding default MTU
     */
    pIPInterface->Cfg.MaxMTUSize = COSA_DML_IP_IF_DEFAULT_MTU;
#endif

    rc = sprintf_s(pIPInterface->Cfg.Alias, sizeof(pIPInterface->Cfg.Alias),"Interface%lu", pMyObject->ulNextInterfaceInsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pIPInterface);
      return NULL;
    }

    AnscSListInitializeHeader(&pIPInterface->IPV4List);
    
    pIPInterface->ulNextIPV4InsNum = 1;
    
    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pIPInterface);
            
            return NULL;
        }

        pCosaContext->InstanceNumber = pIPInterface->Cfg.InstanceNumber = pMyObject->ulNextInterfaceInsNum;

        pMyObject->ulNextInterfaceInsNum++;

        if ( 0 == pMyObject->ulNextInterfaceInsNum )
        {
            pMyObject->ulNextInterfaceInsNum = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pIPInterface;
        pCosaContext->hParentTable     = NULL;
        pCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderIPInterfaceHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pIPIFHead, pCosaContext);

        CosaIPRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_IPIF_NextInsNum,
            pMyObject->ulNextInterfaceInsNum,
            COSA_DML_RR_NAME_IPIF_Prefix,
            0,
            pIPInterface->Cfg.Alias,
            (ANSC_HANDLE)pCosaContext
        );
    }

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface2_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
Interface2_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PSLIST_HEADER                   pIPIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported("delentry"))
    {
        return ANSC_STATUS_FAILURE;
    }
#endif
#endif
    CosaDmlIpIfDelEntry(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber);

    AnscSListPopEntryByLink(pIPIFHead, &pCosaContext->Linkage);

    if ( pCosaContext->bNew )
    {
        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    
    AnscFreeMemory(pIPInterface);
    AnscFreeMemory(pCosaContext);

    return 0;
    }


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_GetParamBoolValue
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
Interface2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pIPInterface->Cfg.bEnabled;
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "Reset") == 0)
    {
        /* collect value */
        *pBool = FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "Loopback") == 0)
    {
        /* collect value */
        *pBool = pIPInterface->Cfg.Loopback;
        return TRUE;
    }

    if (strcmp(ParamName, "AutoIPEnable") == 0)
    {
        /* collect value */
        *pBool = pIPInterface->Cfg.AutoIPEnable;
        return TRUE;
    }

    if (strcmp(ParamName, "IPv4Enable") == 0)
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    if (strcmp(ParamName, "IPv6Enable") == 0)
    {
        /* collect value */
#if defined (MULTILAN_FEATURE)
        *pBool = pIPInterface->Cfg.bIPv6Enabled;
#else
	*pBool = TRUE;
#endif
        return TRUE;
    }

    if (strcmp(ParamName, "ULAEnable") == 0)
    {
        /* collect value */
        *pBool = FALSE;
        return TRUE;
    }
    else if (strcmp(ParamName, "X_CISCO_COM_UPnPIGDEnabled") == 0)
    {
        *pBool = pIPInterface->Cfg.UpnpIgdEnabled;
        return  TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Interface2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Interface2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_DML_IP_IF_INFO            p            = NULL;
    long uptime1                                 = 0;

#if defined (_INTEL_MAX_MTU_PROPOSED_FEATURE_)
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
#endif

    p = CosaDmlIpIfGetEntry2(NULL, pIPInterface->Cfg.InstanceNumber);

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* Get value every time */
        if (CosaUtilIoctlXXX((char *)pIPInterface->Info.Name, "status", NULL) & IFF_UP) 
        {
            *puLong = COSA_DML_IF_STATUS_Up;
        }
        else
        {
            *puLong = COSA_DML_IF_STATUS_LowerLayerDown;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "LastChange") == 0)
    {
        /* collect value */
        get_uptime(&uptime1);
        *puLong = AnscGetTimeIntervalInSeconds(pIPInterface->Info.LastChange, uptime1);
        return TRUE;
    }

    if (strcmp(ParamName, "MaxMTUSize") == 0)
    {
        /* collect value */
        *puLong = pIPInterface->Cfg.MaxMTUSize;

#if defined (_INTEL_MAX_MTU_PROPOSED_FEATURE_)
        CosaDmlIpIfGetCfg (pMyObject->hSbContext, (PCOSA_DML_IP_IF_CFG)&pIPInterface->Cfg);
#endif

        return TRUE;
    }

    if (strcmp(ParamName, "Type") == 0)
    {
        /* collect value */
        *puLong = pIPInterface->Cfg.IfType;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_TTL") == 0)
    {
        *puLong = pIPInterface->Cfg.WanTTL;
        return TRUE;
    }
    else if (strcmp(ParamName, "X_CISCO_COM_ARPCacheTimeout") == 0)
    {
        *puLong = pIPInterface->Cfg.ArpCacheTimeout;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_IPv6_IANA_T1") == 0)
    {
        if ( p ){
            *puLong = p->iana_t1;
            return TRUE;
        }
    }

    if (strcmp(ParamName, "X_CISCO_COM_IPv6_IANA_T2") == 0)
    {
        if ( p ){
            *puLong = p->iana_t2;
            return TRUE;
        }
    }

    if (strcmp(ParamName, "X_CISCO_COM_IPv6_IAPD_T1") == 0)
    {
        if ( p ){
            *puLong = p->iapd_t1;
            return TRUE;
        }
    }

    if (strcmp(ParamName, "X_CISCO_COM_IPv6_IAPD_T2") == 0)
    {
        if ( p ){
            *puLong = p->iapd_t2;
            return TRUE;
        }
    }

    if (strcmp(ParamName, "X_CISCO_COM_IPv6_IANA_IAID") == 0)
    {
        if ( p ){
            *puLong = p->iana_iaid;
            return TRUE;
        }
    }

    if (strcmp(ParamName, "X_CISCO_COM_IPv6_IAPD_IAID") == 0)
    {
        if ( p ){
            *puLong = p->iapd_iaid;         
            return TRUE;
        }
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface2_GetParamStringValue
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
Interface2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    errno_t                         rc           = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPInterface->Cfg.Alias);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Name") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPInterface->Info.Name);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        if(_ansc_strlen(pIPInterface->Cfg.LowerLayers) == 0)
        {
            CcspTraceDebug
                ((
                    "%s -- LowerLayers, IpIf %lu, LinkName is %s, EthLink %lu, IfType %d\n",
                    __FUNCTION__,
                    pIPInterface->Cfg.InstanceNumber,
                    pIPInterface->Cfg.LinkName,
                    pIPInterface->Cfg.LinkInstNum,
                    pIPInterface->Cfg.IfType
                ));

            if ( pIPInterface->Cfg.IfType != COSA_DML_IP_IF_TYPE_Normal )
            {
                pIPInterface->Cfg.LowerLayers[0] = '\0';
            }
            else if ( !pIPInterface->Cfg.LinkInstNum )
            {
                /* Link instance number/LinkName is invalid*/
                char* linkTypePath = CosaUtilGetLinkTypePath(pIPInterface->Cfg.LinkType);
                unsigned char *pLowerLayer;

                if (( pLowerLayer = CosaUtilGetLowerLayers((PUCHAR)linkTypePath, (PUCHAR)pIPInterface->Cfg.LinkName) ))
                {
                    rc = strcpy_s(pIPInterface->Cfg.LowerLayers, sizeof(pIPInterface->Cfg.LowerLayers),(char*)pLowerLayer);
                    if(rc != EOK)
                    {
                      ERR_CHK(rc);
                      AnscFreeMemory(pLowerLayer);
                      return -1;
                    }

                    AnscFreeMemory(pLowerLayer);
                }
                else
                {
                    pIPInterface->Cfg.LowerLayers[0] = '\0';
                }
            }
            else
            {
                char* linkTypePath = CosaUtilGetLinkTypePath(pIPInterface->Cfg.LinkType);

                rc = sprintf_s(pIPInterface->Cfg.LowerLayers, sizeof(pIPInterface->Cfg.LowerLayers),"%s%lu", linkTypePath, pIPInterface->Cfg.LinkInstNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  return -1;
                }
            }
        }

        CcspTraceDebug
            ((
                "%s -- LowerLayers is %s\n",
                __FUNCTION__,
                pIPInterface->Cfg.LowerLayers
            ));

        rc = strcpy_s(pValue, *pUlSize, pIPInterface->Cfg.LowerLayers);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "Router") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPInterface->Cfg.RouterName);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_SetParamBoolValue
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
Interface2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pIPInterface ->Cfg.bEnabled = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "Reset") == 0)
    {
        /* save update to backup */
        if ( TRUE == bValue )
        {
            CosaDmlIpIfReset(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber);
        }
        return TRUE;
    }

    if (strcmp(ParamName, "Loopback") == 0)
    {
        /* save update to backup */
        pIPInterface->Cfg.Loopback = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "AutoIPEnable") == 0)
    {
        /* save update to backup */
        pIPInterface->Cfg.AutoIPEnable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "IPv4Enable") == 0)
    {
        return FALSE;
    }

    if (strcmp(ParamName, "IPv6Enable") == 0)
    {
#if defined (MULTILAN_FEATURE)
        /* save update to backup */
        pIPInterface ->Cfg.bIPv6Enabled = bValue;
        return TRUE;
#else
        return FALSE;
#endif
    }

    if (strcmp(ParamName, "ULAEnable") == 0)
    {
        return FALSE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_UPnPIGDEnabled") == 0)
    {
        /* save update to backup */
        pIPInterface->Cfg.UpnpIgdEnabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Interface2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Interface2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "MaxMTUSize") == 0)
    {
        /* save update to backup */
        pIPInterface->Cfg.MaxMTUSize = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_TTL") == 0)
    {
        pIPInterface->Cfg.WanTTL = uValue;
        return TRUE;
    }
    else if (strcmp(ParamName, "X_CISCO_COM_ARPCacheTimeout") == 0)
    {
        pIPInterface->Cfg.ArpCacheTimeout = uValue;
        return  TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Interface2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    errno_t                         rc           = -1;
    ANSC_STATUS                     rc1          = ANSC_STATUS_SUCCESS;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pIPInterface->Cfg.Alias, sizeof(pIPInterface->Cfg.Alias), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        if ( pIPInterface->Cfg.IfType != COSA_DML_IP_IF_TYPE_Normal )
        {
            /* Loopback/Tunnel/Tunneled cannot have LowerLayers */
            return  FALSE;
        }
        else
        {
            ULONG                           ulIndex;
            CHAR                           ucEntryParamName[256]       = {0};
            CHAR                           ucEntryNameValue[256]       = {0};
#if defined (MULTILAN_FEATURE) || defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
            ULONG                           ulEntryNameLen = 256;
#else
      	    ULONG                           size;
            parameterValStruct_t            varStruct;
#endif
 
           AnscTraceWarning
                ((
                    "%s -- IpIf %lu, LinkName is %s, EthLink %lu, LowerLayers %s\n",
                    __FUNCTION__,
                    pIPInterface->Cfg.InstanceNumber,
                    pIPInterface->Info.Name,
                    pIPInterface->Cfg.LinkInstNum,
                    pString
                ));

            if ( _ansc_strlen(pString) == 0 )
            {
                pIPInterface->Cfg.LinkType    = COSA_DML_LINK_TYPE_LAST;
                pIPInterface->Cfg.LinkInstNum = 0;
                pIPInterface->Cfg.LinkName[0] = '\0';
            }
            else
            {
                pIPInterface->Cfg.LinkType = CosaUtilGetLinkTypeFromPath(pString);

                if ( pIPInterface->Cfg.LinkType == COSA_DML_LINK_TYPE_LAST )
                {
                   AnscTraceWarning(("%s : Parameter is not matching with any of the Link Type. Linktype:%d\n ",__FUNCTION__,pIPInterface->Cfg.LinkType));
                   return FALSE;
                }
                /* Normalize the LowerLayer string -- remove the '.' at the end */
                if ( pString[_ansc_strlen(pString) - 1] == '.' )
                {
                    pString[_ansc_strlen(pString) - 1] = '\0';   
                }

                /* Extract Instance Number */
                ulIndex = _ansc_strlen(pString) - 1;
#if defined (MULTILAN_FEATURE)
                while ( (ulIndex != 0) && (pString[ulIndex -1] != '.') )
#else
		while ( (ulIndex != 0) && (pString[ulIndex] != '.') )
#endif
                {
                    ulIndex--;
                }

                if ( ulIndex == 0 )
                {
                    pIPInterface->Cfg.LinkInstNum = 0;
                }
                else
                {
                    pIPInterface->Cfg.LinkInstNum = (ULONG)AnscString2Int(&pString[ulIndex]);
                }
                /* Retrieve LinkName */
                rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName),"%s.Name", pString);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  return FALSE;
                }
#if defined (MULTILAN_FEATURE) || defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
                ulEntryNameLen = sizeof(ucEntryNameValue);
                rc = CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen);
                if ( 0 == rc )
#else
                varStruct.parameterName  = ucEntryParamName;
                varStruct.parameterValue = ucEntryNameValue;
                AnscTraceInfo(("%s:calling COSAGetParamValueByPathName with varStruct.parameterName = %s varStruct.parameterValue=%s\n",__FUNCTION__,varStruct.parameterName,varStruct.parameterValue));
                rc1 = COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct, &size);
                if ( ANSC_STATUS_SUCCESS == rc1 || rc1 == ANSC_STATUS_DISCARD )
#endif
                {
                    if(rc1 == ANSC_STATUS_DISCARD)
                    {
#if !defined (MULTILAN_FEATURE) && !defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
                       rc = CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &size);
#endif
                    }
                    if (rc == 0)
                    {
                       rc = STRCPY_S_NOCLOBBER(pIPInterface->Cfg.LinkName, sizeof(pIPInterface->Cfg.LinkName), ucEntryNameValue);
                       if(rc != EOK)
                       {
                          ERR_CHK(rc);
                          return FALSE;
                       }
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
                       /* LowerLayers Updated from WanManger. Allow only 1st interface.*/
                       if(pIPInterface->Cfg.InstanceNumber == 1)
                       {
                           rc = STRCPY_S_NOCLOBBER(pIPInterface->Cfg.LowerLayers, sizeof(pIPInterface->Cfg.LowerLayers), pString);
                           if(rc != EOK)
                           {
                               ERR_CHK(rc);
                               return FALSE;
                           }
                           CcspTraceInfo(("%s %d Updating LowerLayers : %s ,InstanceNumber : %lu\n", __FUNCTION__, __LINE__, pIPInterface->Cfg.LowerLayers, pIPInterface->Cfg.InstanceNumber));
                           /* save update to backup */
                           CosaUpdateIfname((pIPInterface->Cfg.InstanceNumber - 1),ucEntryNameValue);

                           rc = strcpy_s(pIPInterface->Info.Name, sizeof(pIPInterface->Info.Name), ucEntryNameValue);
                           if(rc != EOK)
                           {
                               ERR_CHK(rc);
                               return FALSE;
                           }
                           char out[256]= {0};
                           rc = sprintf_s(out, sizeof(out), "Interface_%s", pIPInterface->Info.Name);
                           if(rc < EOK)
                           {
                               ERR_CHK(rc);
                           }
                           rc = STRCPY_S_NOCLOBBER(pIPInterface->Cfg.Alias, sizeof(pIPInterface->Cfg.Alias), out);
                           if(rc != EOK)
                           {
                               ERR_CHK(rc);
                               return FALSE;
                           }
                       }
#else
                       rc = STRCPY_S_NOCLOBBER(pIPInterface->Cfg.LowerLayers, sizeof(pIPInterface->Cfg.LowerLayers), pString);
                       if(rc != EOK)
                       {
                           ERR_CHK(rc);
                           return FALSE;
                       }
#endif
                    }
                }
                else
                {
                    AnscTraceInfo(("CosaGetParamValueString/COSAGetParamValueByPathName returned failure and assigning LinkName variable to NULLL\n"));
                    pIPInterface->Cfg.LinkName[0] = '\0';
                }
            }
            return  TRUE;
        }
    }

    if (strcmp(ParamName, "Router") == 0)
    {
        /* save update to backup */
//        AnscCopyString(pIPInterface->Cfg.Router, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
Interface2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface2 = (PCOSA_DML_IP_IF_FULL2)NULL;
    PSLIST_HEADER                   pIPIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;
    errno_t                         rc            = -1;

    pLink = AnscSListGetFirstEntry(pIPIFHead);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPInterface2 = (PCOSA_DML_IP_IF_FULL2)pCosaContext2->hContext;

        if ( pIPInterface2 && 
            ((ULONG)pIPInterface2 != (ULONG)pIPInterface) &&
             (strcmp(pIPInterface2->Cfg.Alias, pIPInterface->Cfg.Alias) == 0))

        {
            rc = strcpy_s(pReturnParamName, *puLength, "Alias");
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }

            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("Interface2_Validate() failed.\n"));
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface2_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
Interface2_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlIpIfAddEntry(pMyObject->hSbContext, (PCOSA_DML_IP_IF_FULL)pIPInterface);

        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlIpIfSetCfg(pMyObject->hSbContext, (PCOSA_DML_IP_IF_CFG)&pIPInterface->Cfg);
    }
    
    return 0;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface2_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
Interface2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2            pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    CcspTraceWarning(("Interface2_Rollback()----begin to rollback......\n"));

    CosaDmlIpIfGetCfg (pMyObject->hSbContext, (PCOSA_DML_IP_IF_CFG)&pIPInterface->Cfg);
    CosaDmlIpIfGetInfo(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, (PCOSA_DML_IP_IF_INFO)&pIPInterface->Info);

    return 0; 
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv4Address.{i}.

    *  IPv4Address_GetEntryCount
    *  IPv4Address_GetEntry
    *  IPv4Address_AddEntry
    *  IPv4Address_DelEntry
    *  IPv4Address_GetParamBoolValue
    *  IPv4Address_GetParamIntValue
    *  IPv4Address_GetParamUlongValue
    *  IPv4Address_GetParamStringValue
    *  IPv4Address_SetParamBoolValue
    *  IPv4Address_SetParamIntValue
    *  IPv4Address_SetParamUlongValue
    *  IPv4Address_SetParamStringValue
    *  IPv4Address_Validate
    *  IPv4Address_Commit
    *  IPv4Address_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
IPv4Address_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    return AnscSListQueryDepth(&pIPInterface->IPV4List);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv4Address_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
IPv4Address_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PSINGLE_LINK_ENTRY              pLink            = (PSINGLE_LINK_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pLink = AnscSListGetEntryByIndex((PSLIST_HEADER)&pIPInterface->IPV4List, nIndex);

    if ( pLink )
    {
        pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pSubCosaContext->InstanceNumber;
    }
    
    return pSubCosaContext;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv4Address_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/
ANSC_HANDLE
IPv4Address_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_IP              pMyObject        = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr        = (PCOSA_DML_IP_V4ADDR)NULL;
    errno_t                         rc               = -1;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported("ipv4addr_addentry"))
    {
        return NULL;
    }
#endif
#endif
    pIPv4Addr = (PCOSA_DML_IP_V4ADDR)AnscAllocateMemory(sizeof(COSA_DML_IP_V4ADDR));

    if ( !pIPv4Addr )
    {
        return NULL;
    }

    rc = sprintf_s(pIPv4Addr->Alias, sizeof(pIPv4Addr->Alias),"IPv4Address%lu", pIPInterface->ulNextIPV4InsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pIPv4Addr);
      return NULL;
    }

    /* Update the middle layer data */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pIPv4Addr);
            
            return NULL;
        }

        pSubCosaContext->InstanceNumber = pIPInterface->ulNextIPV4InsNum;

        pIPInterface->ulNextIPV4InsNum++;

        if ( 0 == pIPInterface->ulNextIPV4InsNum )
        {
            pIPInterface->ulNextIPV4InsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pIPv4Addr;
        pSubCosaContext->hParentTable     = pIPInterface;
        pSubCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderIPInterfaceHA;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pIPInterface->IPV4List, pSubCosaContext);

        CosaIPRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_IPIF_IPv4_NextInsNum,
            pIPInterface->ulNextIPV4InsNum,
            COSA_DML_RR_NAME_IPIF_IPV4_Prefix,
            pIPInterface->Cfg.InstanceNumber,
            pIPv4Addr->Alias,
            (ANSC_HANDLE)pSubCosaContext
        );
    }

    *pInsNumber = pSubCosaContext->InstanceNumber;

    return pSubCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv4Address_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_DATAMODEL_IP              pMyObject       = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface    = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr       = (PCOSA_DML_IP_V4ADDR)pSubCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported("ipv4addr_delentry"))
    {
        return ANSC_STATUS_FAILURE;
    }
#endif
#endif
    CosaDmlIpIfDelV4Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv4Addr);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pIPInterface->IPV4List, &pSubCosaContext->Linkage);

    if ( pSubCosaContext->bNew )
    {
        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pSubCosaContext);
    }

    AnscFreeMemory(pIPv4Addr);
    AnscFreeMemory(pSubCosaContext);

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_GetParamBoolValue
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
IPv4Address_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pIPv4Addr->bEnabled;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv4Address_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv4Address_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "IPAddress") == 0)
    {
#if !defined(_COSA_BCM_MIPS_) && !defined(_ENABLE_DSL_SUPPORT_)
        if ((pIPv4Addr->AddressingType == COSA_DML_IP_ADDR_TYPE_Static) && (pIPInterface->Cfg.InstanceNumber >= (ULONG)CosaGetUsgIfNum()))
        {
	    CosaDmlIpIfMlanGetIPv4Addr(pIPInterface->Cfg.InstanceNumber, pIPv4Addr);
            *puLong = pIPv4Addr->IPAddress.Value;
        }
        else
#endif /* !_COSA_BCM_MIPS_ */
        {
            /* TBC -- this should be from a SBAPI call */
            *puLong = (ULONG) CosaUtilGetIfAddr(pIPInterface->Cfg.LinkName);
        }
         
         return TRUE;
    }

    if (strcmp(ParamName, "SubnetMask") == 0)
    {
        //BOOL                                      bridgeMode;
        //if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
        //   (TRUE == bridgeMode))
        //{
        //    *puLong = 0; 
        //}
#if !defined(_COSA_BCM_MIPS_) && !defined(_ENABLE_DSL_SUPPORT_)
        if ((pIPv4Addr->AddressingType == COSA_DML_IP_ADDR_TYPE_Static) && (pIPInterface->Cfg.InstanceNumber >= (ULONG)CosaGetUsgIfNum()))
        {
	    CosaDmlIpIfMlanGetSubnetMask(pIPInterface->Cfg.InstanceNumber, pIPv4Addr);
            *puLong = pIPv4Addr->SubnetMask.Value;
        }
        else
#endif /* !_COSA_BCM_MIPS_ */
        {
            /*
             *  TBC --  Why on earth is platform specific code called in middle layer!
             */
            *puLong = CosaUtilIoctlXXX(pIPInterface->Info.Name, "netmask", NULL);
        }
        return TRUE;

    }

    if (strcmp(ParamName, "AddressingType") == 0)
    {
        /*
         * The approach taken here is similar to the one taken by the previous 2 parameters.
         * As this level of the datamodel involves both LAN and WAN interfaces and they use
         * different variables in syscfg for persistent storage,below function is being used
         * to take necessary decision-making to fetch the correct value from the appropriate place.
         */
        *puLong = _get_addressing_type(pIPInterface->Cfg.InstanceNumber);
        return TRUE;
    }

    if (strcmp(ParamName, "Status") == 0)
    {
        *puLong = pIPv4Addr->Status;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_GetParamStringValue
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
IPv4Address_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    errno_t                         rc           = -1;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv4Addr->Alias);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }

        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_SetParamBoolValue
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
IPv4Address_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif  
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pIPv4Addr->bEnabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv4Address_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv4Address_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        /* save update to backup */
        if (pIPv4Addr->AddressingType != COSA_DML_IP_ADDR_TYPE_Static)
        {
            return FALSE;
        }
        else 
        {
            pIPv4Addr->IPAddress.Value = uValue;
            return TRUE;
        }
    }

    if (strcmp(ParamName, "SubnetMask") == 0)
    {
        /* save update to backup */
        if (pIPv4Addr->AddressingType != COSA_DML_IP_ADDR_TYPE_Static)
        {
            return FALSE;
        }
        else
        {
            pIPv4Addr->SubnetMask.Value = uValue;
            return TRUE;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv4Address_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    errno_t                         rc           = -1;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pIPv4Addr->Alias, sizeof(pIPv4Addr->Alias), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
IPv4Address_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr     = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr2    = (PCOSA_DML_IP_V4ADDR)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;
    errno_t                         rc            = -1;

    pLink = AnscSListGetFirstEntry(&pIPInterface->IPV4List);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv4Addr2 = (PCOSA_DML_IP_V4ADDR)pCosaContext2->hContext;

        if ( pIPv4Addr2 &&
            ((ULONG)pIPv4Addr2 != (ULONG)pIPv4Addr) &&
             (strcmp(pIPv4Addr2->Alias, pIPv4Addr->Alias) == 0))

        {
            rc = strcpy_s(pReturnParamName, *puLength, "Alias");
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("IPv4Address_Validate() failed.\n"));            
            return FALSE;
        }
    }

    return TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv4Address_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlIpIfAddV4Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv4Addr);

        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlIpIfSetV4Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv4Addr);
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv4Address_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    CcspTraceWarning(("IPv4Address_Rollback()----begin to rollback......\n"));

    CosaDmlIpIfGetV4Addr2(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv4Addr);

    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv6Address.{i}.

    *  IPv6Address_GetEntryCount
    *  IPv6Address_GetEntry
    *  IPv6Address_AddEntry
    *  IPv6Address_DelEntry
    *  IPv6Address_GetParamBoolValue
    *  IPv6Address_GetParamIntValue
    *  IPv6Address_GetParamUlongValue
    *  IPv6Address_GetParamStringValue
    *  IPv6Address_SetParamBoolValue
    *  IPv6Address_SetParamIntValue
    *  IPv6Address_SetParamUlongValue
    *  IPv6Address_SetParamStringValue
    *  IPv6Address_Validate
    *  IPv6Address_Commit
    *  IPv6Address_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
IPv6Address_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{    
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    return AnscSListQueryDepth(&pIPInterface->IPV6List);

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Address_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
IPv6Address_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PSINGLE_LINK_ENTRY              pLink            = (PSINGLE_LINK_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pLink = AnscSListGetEntryByIndex((PSLIST_HEADER)&pIPInterface->IPV6List, nIndex);

    if ( pLink )
    {
        pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pSubCosaContext->InstanceNumber;
    }
    
    return pSubCosaContext;
}

#define IPV6_TABLE_TIMEOUT 30

BOOL
IPv6Address_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    BOOL                            bIsUpdated   = TRUE;

    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pIPInterface->V6AddrPreviousVisitTime ) < IPV6_TABLE_TIMEOUT )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pIPInterface->V6AddrPreviousVisitTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    return bIsUpdated;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv6Address_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                           returnStatus      = ANSC_STATUS_FAILURE;
    PCOSA_CONTEXT_LINK_OBJECT             pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2                 pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT             pCxtLink     = NULL;
    PSINGLE_LINK_ENTRY                    pLink       = NULL;
    PSINGLE_LINK_ENTRY                    pLink2      = NULL;
    PCOSA_DML_IP_V6ADDR                   pIPv6Addr         = (PCOSA_DML_IP_V6ADDR)NULL;
    PCOSA_DML_IP_V6ADDR                   pIPv6Addr2        = (PCOSA_DML_IP_V6ADDR)NULL;
    ULONG                                 entryCount        = 0;
    ULONG                                 i                 = 0;
    PULONG                                pOldInBE          = NULL; /*Old entry in back end*/

    pIPv6Addr         = CosaDmlIPGetIPv6Addresses(pIPInterface,&entryCount);
    if ( !pIPv6Addr )
    {
        /* Get Error, we don't del link because next time, it may be successful */
        return ANSC_STATUS_FAILURE;
    }

    pOldInBE    =   (PULONG)AnscAllocateMemory( entryCount * sizeof(ULONG) );
    if ( !pOldInBE )
    {
        goto EXIT3;
    }
    
    /* Search the whole link and mark bFound of exist entry to FALSE */
    pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6List);
    while( pLink )
    {

        pCxtLink = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv6Addr2 = (PCOSA_DML_IP_V6ADDR)pCxtLink->hContext;

        pIPv6Addr2->bFound = FALSE;
    }

    /* go over all new entries, find them in current link table and mark them */
    for ( i = 0; i < entryCount; i++)
    {
        pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6List);
        while( pLink )
        {
            pCxtLink    = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
            pLink       = AnscSListGetNextEntry(pLink);

            pIPv6Addr2     = (PCOSA_DML_IP_V6ADDR)pCxtLink->hContext;
            if ( pIPv6Addr2->InstanceNumber == pIPv6Addr[i].InstanceNumber )
            {
                pOldInBE[i]         =  1;
                /* If found, update the content also, watch the sequence of below 2 statements, we should set bFound later */
                *pIPv6Addr2    = pIPv6Addr[i];
                pIPv6Addr2->bFound  = TRUE;

                pCxtLink->bNew    = FALSE;
                
                break;
            }
        }
    }

    /* We need del unreferred entry if it's not delay_added entry */
    pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6List);
    while( pLink )
    {
        pCxtLink = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink2      = pLink;
        pLink       = AnscSListGetNextEntry(pLink);
        pIPv6Addr2     = (PCOSA_DML_IP_V6ADDR)pCxtLink->hContext;

        if ( pIPv6Addr2->bFound  == FALSE && pCxtLink->bNew  == FALSE )
        {
            AnscSListPopEntryByLink(&pIPInterface->IPV6List, pLink2);

            AnscFreeMemory( pCxtLink->hContext );
            AnscFreeMemory( pCxtLink );
        }
    }

    /* We add new entry into our link table */
    for ( i = 0; i < entryCount; i++ )
    {
        if ( pOldInBE[i] == 0 )
        {
            /* Add new one */
            pCxtLink             = (COSA_CONTEXT_LINK_OBJECT *)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCxtLink )
            {
                goto EXIT2;
            }
            
            pIPv6Addr2        = (COSA_DML_IP_V6ADDR *)AnscAllocateMemory(sizeof(COSA_DML_IP_V6ADDR));
            if ( !pIPv6Addr2 )
            {
                goto EXIT1;
            }
            /* copy new content which should include InstanceNumber and Alias */
            *pIPv6Addr2       = pIPv6Addr[i];

            pCxtLink->hContext   = (ANSC_HANDLE)pIPv6Addr2;
            pCxtLink->InstanceNumber = pIPv6Addr2->InstanceNumber;
            pCxtLink->bNew       = FALSE;
            pCxtLink->hParentTable = pIPInterface;
            /*todo: add parent table*/

            CosaSListPushEntryByInsNum(&pIPInterface->IPV6List, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
        }
    }

    {
        /*ULONG ret = 0;
        
        ret = AnscSListQueryDepth(&pIPInterface->IPV6List);*/
    }
        
    returnStatus =  ANSC_STATUS_SUCCESS;

    goto EXIT2;
    
EXIT1:
    AnscFreeMemory(pCxtLink);
    
EXIT2:
    AnscFreeMemory(pOldInBE);
    
EXIT3:
    AnscFreeMemory(pIPv6Addr);
    return returnStatus;



}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Address_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/
ANSC_HANDLE
IPv6Address_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_IP              pMyObject        = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr        = (PCOSA_DML_IP_V6ADDR)NULL;
    errno_t                         rc               = -1;

    pIPv6Addr = (PCOSA_DML_IP_V6ADDR)AnscAllocateMemory(sizeof(COSA_DML_IP_V6ADDR));
    /*RDKB-, CID-33520, perform null check before use */
    if ( !pIPv6Addr )
    {
        return NULL;
    }

    pIPv6Addr->Origin = COSA_DML_IP6_ORIGIN_Static;
    rc = strcpy_s(pIPv6Addr->PreferredLifetime, sizeof(pIPv6Addr->PreferredLifetime), "9999-12-31T23:59:59Z");
    if(rc != EOK)
    {
        ERR_CHK(rc);
        AnscFreeMemory(pIPv6Addr);
        return NULL;
    }
    rc = strcpy_s(pIPv6Addr->ValidLifetime, sizeof(pIPv6Addr->ValidLifetime), "9999-12-31T23:59:59Z");
    if(rc != EOK)
    {
        ERR_CHK(rc);
        AnscFreeMemory(pIPv6Addr);
        return NULL;
    }


    rc = sprintf_s(pIPv6Addr->Alias, sizeof(pIPv6Addr->Alias),"IPv6Address%lu", pIPInterface->ulNextIPV6InsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pIPv6Addr);
      return NULL;
    }

    /* Update the middle layer data */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pIPv6Addr);
            
            return NULL;
        }

        pSubCosaContext->InstanceNumber = pIPv6Addr->InstanceNumber = pIPInterface->ulNextIPV6InsNum;

        pIPInterface->ulNextIPV6InsNum++;

        if ( 0 == pIPInterface->ulNextIPV6InsNum )
        {
            pIPInterface->ulNextIPV6InsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pIPv6Addr;
        pSubCosaContext->hParentTable     = pIPInterface;
        pSubCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderIPInterfaceHA;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pIPInterface->IPV6List, pSubCosaContext);

        CosaIPRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_IPIF_IPv6_NextInsNum,
            pIPInterface->ulNextIPV6InsNum,
            COSA_DML_RR_NAME_IPIF_IPV6_Prefix,
            pIPInterface->Cfg.InstanceNumber,
            pIPv6Addr->Alias,
            (ANSC_HANDLE)pSubCosaContext
        );
    }

    *pInsNumber = pSubCosaContext->InstanceNumber;

    return pSubCosaContext;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv6Address_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_DATAMODEL_IP              pMyObject       = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface    = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr       = (PCOSA_DML_IP_V6ADDR)pSubCosaContext->hContext;

    if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
        return ANSC_STATUS_FAILURE;

    CosaDmlIpIfDelV6Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Addr);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pIPInterface->IPV6List, &pSubCosaContext->Linkage);

    if ( pSubCosaContext->bNew )
    {
        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pSubCosaContext);
    }

    AnscFreeMemory(pIPv6Addr);
    AnscFreeMemory(pSubCosaContext);

    return 0;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_GetParamBoolValue
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
IPv6Address_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pIPv6Addr->bEnabled;
        return TRUE;
    }

    if (strcmp(ParamName, "Anycast") == 0)
    {
        /* collect value */
        *pBool = pIPv6Addr->bAnycast;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Address_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /*PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;*/

    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Address_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong = pIPv6Addr->Status;
        return TRUE;
    }

    if (strcmp(ParamName, "IPAddressStatus") == 0)
    {
        /* collect value */
#ifndef _COSA_SIM_
        *puLong = CosaDmlIPv6addrGetV6Status(pIPv6Addr, pIPInterface);
        return TRUE;
#else
       /* CID:54363 Structurally dead code*/

		*puLong = pIPv6Addr->V6Status;
        return TRUE;
#endif
    }

    if (strcmp(ParamName, "Origin") == 0)
    {
        /* collect value */
        *puLong = pIPv6Addr->Origin;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_PreferredLifetime") == 0)
    {
        /* collect value */
        *puLong  = pIPv6Addr->iana_pretm;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_ValidLifetime") == 0)
    {
        /* collect value */
        *puLong  = pIPv6Addr->iana_vldtm;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_GetParamStringValue
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
IPv6Address_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;
    errno_t                         rc           = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Addr->Alias);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "IPAddress") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Addr->IP6Address);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Prefix") == 0)
    {
        /* collect value */
#ifndef _COSA_SIM_

        if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static){        
            CosaDmlGetPrefixPathName(pIPInterface->Info.Name, pIPInterface->Cfg.InstanceNumber, pIPv6Addr, pValue);
        }else {
            rc = strcpy_s(pValue, *pUlSize, pIPv6Addr->Prefix);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
        }
        return 0;
#else
    /*CID: 69097 Structurally dead code*/

        rc = strcpy_s(pValue, *pUlSize, pIPv6Addr->Prefix);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
#endif
    }

    if (strcmp(ParamName, "PreferredLifetime") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Addr->PreferredLifetime);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "ValidLifetime") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Addr->ValidLifetime);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_SetParamBoolValue
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
IPv6Address_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        /*not supported*/
        if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
            return FALSE;
#endif

		pIPv6Addr->bEnabled = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "Anycast") == 0)
    {
        /* save update to backup */
        if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
            return FALSE;

		pIPv6Addr->bAnycast = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Address_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Address_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;

    if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
        return FALSE;
        
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* save update to backup */
        pIPv6Addr->Status = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "IPAddressStatus") == 0)
    {
        /* save update to backup */
        pIPv6Addr->V6Status = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Address_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    errno_t     rc =  -1;
    int ind = -1;

    /* check the parameter name and set the corresponding value */
    rc = strcmp_s("Alias", strlen("Alias"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->Alias, sizeof(pIPv6Addr->Alias), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
        return FALSE;
    
    rc = strcmp_s("IPAddress", strlen("IPAddress"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->IP6Address, sizeof(pIPv6Addr->IP6Address), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    rc = strcmp_s("Prefix", strlen("Prefix"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->Prefix, sizeof(pIPv6Addr->Prefix), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    rc = strcmp_s("PreferredLifetime", strlen("PreferredLifetime"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->PreferredLifetime, sizeof(pIPv6Addr->PreferredLifetime), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    rc = strcmp_s("ValidLifetime", strlen("ValidLifetime"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->ValidLifetime, sizeof(pIPv6Addr->ValidLifetime), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
IPv6Address_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr     = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr2    = (PCOSA_DML_IP_V6ADDR)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;
    struct in6_addr                 in6;
    errno_t                         rc            = -1;

    pLink = AnscSListGetFirstEntry(&pIPInterface->IPV6List);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv6Addr2 = (PCOSA_DML_IP_V6ADDR)pCosaContext2->hContext;

        if ( pIPv6Addr2 &&
            ((ULONG)pIPv6Addr2 != (ULONG)pIPv6Addr ) &&
             (strcmp(pIPv6Addr2->Alias, pIPv6Addr->Alias) == 0))
        {
            rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "Alias");
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("IPv6Address_Validate() on Alias failed.\n"));            
            return FALSE;
        }

        if ( pIPv6Addr2 &&
            (pIPv6Addr2->bEnabled && pIPv6Addr->bEnabled) &&
             ((ULONG)pIPv6Addr2 != (ULONG)pIPv6Addr ) &&
             CosaDmlV6AddrIsEqual(pIPv6Addr2->IP6Address, pIPv6Addr->IP6Address))
        {
            rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Address");
            if(rc != EOK)
            {
              ERR_CHK(rc);
              return FALSE;
            }
            *puLength = AnscSizeOfString("Address");
    
            CcspTraceWarning(("IPv6Address_Validate() on Address failed.\n"));            
            return FALSE;
        }
    }

    /*validate for Prefix*/
    if (pIPv6Addr->Prefix[0] &&
        !CosaDmlIpv6AddrMatchesPrefix(pIPv6Addr->Prefix, pIPv6Addr->IP6Address, pIPInterface->Cfg.InstanceNumber))
    {
        rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "Prefix");
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        *puLength = AnscSizeOfString("Prefix");

        CcspTraceWarning(("IPv6Address_Validate() on Prefix failed.\n"));            
        return FALSE;
    }

    /*validate 2 lifetimes*/
     /*CID: 58024 Array compared against 0*/
    if (pIPv6Addr->ValidLifetime[0] != '\0' &&
        CosaDmlDateTimeCompare(pIPv6Addr->PreferredLifetime, pIPv6Addr->ValidLifetime) > 0
        )
    {
        rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "PreferredLifetime/ValidLifetime");
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        *puLength = AnscSizeOfString("PreferredLifetime/ValidLifetime");

        CcspTraceWarning(("IPv6Address_Validate() on Lifetime failed.\n"));            
        return FALSE;
    }


    if (inet_pton(AF_INET6, pIPv6Addr->IP6Address, &in6) != 1)
    {
        rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Address");
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        *puLength = AnscSizeOfString("Address");
    
        CcspTraceWarning(("IPv6Address_Validate() on Address failed.\n"));
        return FALSE;
    }


    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv6Address_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlIpIfAddV6Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Addr);

        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlIpIfSetV6Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Addr);
    }
    
    return 0;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv6Address_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    CcspTraceWarning(("IPv6Address_Rollback()----begin to rollback......\n"));

    CosaDmlIpIfGetV6Addr2(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Addr);

    return ANSC_STATUS_SUCCESS;
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv6Prefix.{i}.

    *  IPv6Prefix_GetEntryCount
    *  IPv6Prefix_GetEntry
    *  IPv6Prefix_AddEntry
    *  IPv6Prefix_DelEntry
    *  IPv6Prefix_GetParamBoolValue
    *  IPv6Prefix_GetParamIntValue
    *  IPv6Prefix_GetParamUlongValue
    *  IPv6Prefix_GetParamStringValue
    *  IPv6Prefix_SetParamBoolValue
    *  IPv6Prefix_SetParamIntValue
    *  IPv6Prefix_SetParamUlongValue
    *  IPv6Prefix_SetParamStringValue
    *  IPv6Prefix_Validate
    *  IPv6Prefix_Commit
    *  IPv6Prefix_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
IPv6Prefix_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    return AnscSListQueryDepth(&pIPInterface->IPV6PrefixList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Prefix_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
IPv6Prefix_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PSINGLE_LINK_ENTRY              pLink            = (PSINGLE_LINK_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pLink = AnscSListGetEntryByIndex((PSLIST_HEADER)&pIPInterface->IPV6PrefixList, nIndex);

    if ( pLink )
    {
        pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pSubCosaContext->InstanceNumber;
    }
    
    return pSubCosaContext;
}

BOOL
IPv6Prefix_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    BOOL                            bIsUpdated       = TRUE;

    if (!AnscSListQueryDepth(&pIPInterface->IPV6PrefixList)) {
        return TRUE;
    }
    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pIPInterface->V6PrefPreviousVisitTime ) < IPV6_TABLE_TIMEOUT )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pIPInterface->V6PrefPreviousVisitTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

#ifdef _HUB4_PRODUCT_REQ_
    /* To update the ipv6_prefix value for dibbler configuration, the DML should be syncronzed in time. So time delay check removed */
    bIsUpdated  = TRUE;
#endif
    return bIsUpdated;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv6Prefix_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                           returnStatus      = ANSC_STATUS_FAILURE;
    PCOSA_CONTEXT_LINK_OBJECT             pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2                 pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT             pCxtLink     = NULL;
    PSINGLE_LINK_ENTRY                    pLink       = NULL;
    PSINGLE_LINK_ENTRY                    pLink2      = NULL;
    PCOSA_DML_IP_V6PREFIX                 pIPv6Pre         = (PCOSA_DML_IP_V6PREFIX)NULL;
    PCOSA_DML_IP_V6PREFIX                 pIPv6Pre2        = (PCOSA_DML_IP_V6PREFIX)NULL;
    ULONG                                 entryCount        = 0;
    ULONG                                 i                 = 0;
    PULONG                                pOldInBE       = 0; /*Old entry in back end*/

    pIPv6Pre         = CosaDmlIPGetIPv6Prefixes(pIPInterface,&entryCount);
    if ( !pIPv6Pre )
        return ANSC_STATUS_FAILURE;

    pOldInBE    =   (PULONG)AnscAllocateMemory( entryCount * sizeof(ULONG) );
    if ( !pOldInBE )
    {
        goto EXIT3;
    }
    
    /* Search the whole link and mark bFound of exist entry to FALSE */
    pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6PrefixList);
    while( pLink )
    {

        pCxtLink = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv6Pre2 = (PCOSA_DML_IP_V6PREFIX)pCxtLink->hContext;

        pIPv6Pre2->bFound = FALSE;
    }

    /* go over all old entries, find them in current link table and mark them */
    for ( i = 0; i < entryCount; i++)
    {
        pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6PrefixList);
        while( pLink )
        {
            pCxtLink    = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
            pLink       = AnscSListGetNextEntry(pLink);

            pIPv6Pre2     = (PCOSA_DML_IP_V6PREFIX)pCxtLink->hContext;
            if ( pIPv6Pre2->InstanceNumber == pIPv6Pre[i].InstanceNumber )
            {
                pOldInBE[i]         =  1;

                /* If found, update the content also, watch the sequence of below 2 statements, we should set bFound later  */
                *pIPv6Pre2    = pIPv6Pre[i];
                pIPv6Pre2->bFound  = TRUE;

                pCxtLink->bNew    = FALSE;
                
                break;
            }
        }
    }

    /* We need del unreferred entry if it's not delay_added entry */
    pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6PrefixList);
    while( pLink )
    {
        pCxtLink = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink2      = pLink;
        pLink       = AnscSListGetNextEntry(pLink);
        pIPv6Pre2     = (PCOSA_DML_IP_V6PREFIX)pCxtLink->hContext;

        if ( pIPv6Pre2->bFound  == FALSE && pCxtLink->bNew  == FALSE )
        {
            AnscSListPopEntryByLink(&pIPInterface->IPV6PrefixList, pLink2);

            AnscFreeMemory( pCxtLink->hContext );
            AnscFreeMemory( pCxtLink );
        }
    }

    /* We add new entry into our link table */
    for ( i = 0; i < entryCount; i++ )
    {
        if ( pOldInBE[i] == 0 )
        {
            /* Add new one */
            pCxtLink             = (COSA_CONTEXT_LINK_OBJECT *)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCxtLink )
            {
                goto EXIT2;
            }
            
            pIPv6Pre2        = (COSA_DML_IP_V6PREFIX *)AnscAllocateMemory(sizeof(COSA_DML_IP_V6PREFIX));
            if ( !pIPv6Pre2 )
            {
                goto EXIT1;
            }
            /* copy new content which should include InstanceNumber and Alias */
            *pIPv6Pre2       = pIPv6Pre[i];

            pCxtLink->hContext   = (ANSC_HANDLE)pIPv6Pre2;
            pCxtLink->hParentTable   = (ANSC_HANDLE)pIPInterface;
            pCxtLink->InstanceNumber = pIPv6Pre2->InstanceNumber;
            pCxtLink->bNew       = FALSE;

            CosaSListPushEntryByInsNum(&pIPInterface->IPV6PrefixList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
        }
    }
        
    returnStatus =  ANSC_STATUS_SUCCESS;

    goto EXIT2;
    
EXIT1:
    AnscFreeMemory(pCxtLink);
    
EXIT2:
    AnscFreeMemory(pOldInBE);
    
EXIT3:
    AnscFreeMemory(pIPv6Pre);
    return returnStatus;



}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Prefix_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/
ANSC_HANDLE
IPv6Prefix_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_IP              pMyObject        = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre         = (PCOSA_DML_IP_V6PREFIX)NULL;
    errno_t                         rc               = -1;

    pIPv6Pre = (PCOSA_DML_IP_V6PREFIX)AnscAllocateMemory(sizeof(COSA_DML_IP_V6PREFIX));

    if ( !pIPv6Pre )
    {
        return NULL;
    }

    /*we only support adding static prefix*/
    pIPv6Pre->Origin = COSA_DML_IP6PREFIX_ORIGIN_Static;
    pIPv6Pre->StaticType = COSA_DML_IP6PREFIX_STATICTYPE_Static;
    pIPv6Pre->bEnabled = COSA_DML_PREFIXENTRY_STATUS_Enabled;
    rc = strcpy_s(pIPv6Pre->PreferredLifetime, sizeof(pIPv6Pre->PreferredLifetime), "0001-01-01T00:00:00Z");
    ERR_CHK(rc);
    rc = strcpy_s(pIPv6Pre->ValidLifetime, sizeof(pIPv6Pre->ValidLifetime), "0001-01-01T00:00:00Z");
    ERR_CHK(rc);

    rc = sprintf_s(pIPv6Pre->Alias, sizeof(pIPv6Pre->Alias),"IPv6Prefix%lu", pIPInterface->ulNextIPV6PreInsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pIPv6Pre);
      return NULL;
    }

    /* Update the middle layer data */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pIPv6Pre);
            
            return NULL;
        }

        pSubCosaContext->InstanceNumber = pIPv6Pre->InstanceNumber =  pIPInterface->ulNextIPV6PreInsNum;

        pIPInterface->ulNextIPV6PreInsNum++;

        if ( 0 == pIPInterface->ulNextIPV6PreInsNum )
        {
            pIPInterface->ulNextIPV6PreInsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pIPv6Pre;
        pSubCosaContext->hParentTable     = pIPInterface;
        pSubCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderIPInterfaceHA;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pIPInterface->IPV6PrefixList, pSubCosaContext);

        CosaIPRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_IPIF_IPv6Pre_NextInsNum,
            pIPInterface->ulNextIPV6PreInsNum,
            COSA_DML_RR_NAME_IPIF_IPV6Pre_Prefix,
            pIPInterface->Cfg.InstanceNumber,
            pIPv6Pre->Alias,
            (ANSC_HANDLE)pSubCosaContext
        );
    }

    *pInsNumber = pSubCosaContext->InstanceNumber;

    return pSubCosaContext;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv6Prefix_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_DATAMODEL_IP              pMyObject       = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface    = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre        = (PCOSA_DML_IP_V6PREFIX)pSubCosaContext->hContext;

    if (pIPv6Pre->Origin != COSA_DML_IP6PREFIX_ORIGIN_Static)
        return ANSC_STATUS_FAILURE;

    CosaDmlIpIfDelV6Prefix(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Pre);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pIPInterface->IPV6PrefixList, &pSubCosaContext->Linkage);

    if ( pSubCosaContext->bNew )
    {
        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pSubCosaContext);
    }

    AnscFreeMemory(pIPv6Pre);
    AnscFreeMemory(pSubCosaContext);

    return 0; /* succeeded */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_GetParamBoolValue
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
IPv6Prefix_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pIPv6Pre->bEnabled;
        return TRUE;
    }

    if (strcmp(ParamName, "OnLink") == 0)
    {
        /* collect value */
        *pBool = pIPv6Pre->bOnlink;
        return TRUE;
    }

    if (strcmp(ParamName, "Autonomous") == 0)
    {
        /* collect value */
        *pBool = pIPv6Pre->bAutonomous;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Prefix_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Prefix_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong = pIPv6Pre->Status;
        return TRUE;
    }

    if (strcmp(ParamName, "PrefixStatus") == 0)
    {
        /* collect value */
        *puLong = pIPv6Pre->PrefixStatus;
        return TRUE;
    }

    if (strcmp(ParamName, "Origin") == 0)
    {
        /* collect value */
        *puLong = pIPv6Pre->Origin;
        return TRUE;
    }

    if (strcmp(ParamName, "StaticType") == 0)
    {
        /* collect value */
        *puLong = pIPv6Pre->StaticType;
        return TRUE;
    }


    if (strcmp(ParamName, "X_CISCO_COM_PreferredLifetime") == 0)
    {
        /* collect value */
        *puLong  = pIPv6Pre->iapd_pretm;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_ValidLifetime") == 0)
    {
        /* collect value */
        *puLong  = pIPv6Pre->iapd_vldtm;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_GetParamStringValue
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
IPv6Prefix_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;
    errno_t                         rc           = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Pre->Alias);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Prefix") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Pre->Prefix);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "ParentPrefix") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Pre->ParentPrefix);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "ChildPrefixBits") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Pre->ChildPrefixBits);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "PreferredLifetime") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Pre->PreferredLifetime);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "ValidLifetime") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pIPv6Pre->ValidLifetime);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_SetParamBoolValue
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
IPv6Prefix_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        /*not supported, even for static Prefix, we set Enable always true when adding one entry*/
        return FALSE;
    
#endif

		pIPv6Pre->bEnabled = bValue;
        return TRUE;
    }

    if (pIPv6Pre->Origin != COSA_DML_IP6PREFIX_ORIGIN_Static)
        return FALSE;

    if (strcmp(ParamName, "OnLink") == 0)
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        /*not supported, even for static Prefix, */
        return FALSE;
    
#else
		pIPv6Pre->bOnlink = bValue;
        return TRUE;
#endif
    }

    if (strcmp(ParamName, "Autonomous") == 0)
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        /*not supported, even for static Prefix, */
        return FALSE;
    
#else
		pIPv6Pre->bAutonomous = bValue;
        return TRUE;
#endif
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Prefix_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Prefix_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    if (pIPv6Pre->Origin != COSA_DML_IP6PREFIX_ORIGIN_Static)
        return FALSE;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* save update to backup */
        pIPv6Pre->Status = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "PrefixStatus") == 0)
    {
        /* save update to backup */
        pIPv6Pre->PrefixStatus = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "StaticType") == 0)
    {
#ifndef _COSA_SIM_
        /*not supported*/
        return FALSE;

#else
      /* CID: 73388 Structurally dead code*/
        /* save update to backup */
        pIPv6Pre->StaticType = uValue;
        return TRUE;
#endif
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPv6Prefix_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;
    errno_t                         rc           = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pIPv6Pre->Alias, sizeof(pIPv6Pre->Alias), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    if (pIPv6Pre->Origin != COSA_DML_IP6PREFIX_ORIGIN_Static)
        return FALSE;

    if (strcmp(ParamName, "Prefix") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pIPv6Pre->Prefix, sizeof(pIPv6Pre->Prefix), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "ParentPrefix") == 0)
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        return FALSE;
#else
       /*CID: 61160 Structurally dead code*/
        rc = STRCPY_S_NOCLOBBER(pIPv6Pre->ParentPrefix, sizeof(pIPv6Pre->ParentPrefix), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
#endif
    }

    if (strcmp(ParamName, "ChildPrefixBits") == 0)
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        return FALSE;
#else
       /*CID: 61160 Structurally dead code*/
        rc = STRCPY_S_NOCLOBBER(pIPv6Pre->ChildPrefixBits, sizeof(pIPv6Pre->ChildPrefixBits), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
#endif
    }

    if (strcmp(ParamName, "PreferredLifetime") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pIPv6Pre->PreferredLifetime, sizeof(pIPv6Pre->PreferredLifetime), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "ValidLifetime") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pIPv6Pre->ValidLifetime, sizeof(pIPv6Pre->ValidLifetime), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
IPv6Prefix_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre      = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre2     = (PCOSA_DML_IP_V6PREFIX)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;
    struct in6_addr                 in6;
    char *                          p             =  NULL;
    errno_t                         rc            = -1;

    pLink = AnscSListGetFirstEntry(&pIPInterface->IPV6PrefixList);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv6Pre2 = (PCOSA_DML_IP_V6PREFIX)pCosaContext2->hContext;

        if ( pIPv6Pre2 &&
            ((ULONG)pIPv6Pre2 != (ULONG)pIPv6Pre ) &&
             (strcmp(pIPv6Pre2->Alias, pIPv6Pre->Alias) == 0))

        {
            rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "Alias");
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("IPv6Prefix_Validate() on Alias failed.\n"));            
            return FALSE;
        }

        if ( pIPv6Pre2 &&
            (pIPv6Pre2->bEnabled && pIPv6Pre->bEnabled) &&
             ((ULONG)pIPv6Pre2 != (ULONG)pIPv6Pre ) &&
             CosaDmlV6PrefIsEqual(pIPv6Pre2->Prefix, pIPv6Pre->Prefix))
        {
             rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Prefix");
             if(rc != EOK)
             {
               ERR_CHK(rc);
               return FALSE;
             }
            *puLength = AnscSizeOfString("Prefix");
    
            CcspTraceWarning(("IPv6Prefix_Validate() on Prefix failed.\n"));            
            return FALSE;
        }
    }

    if (!(p= strchr(pIPv6Pre->Prefix, '/')))
    {
        rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Prefix");
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        *puLength = AnscSizeOfString("Prefix");
    
        CcspTraceWarning(("IPv6Prefix_Validate() on Prefix failed.\n"));
        return FALSE;
    }
    else 
    {
        char * dup = AnscCloneString(pIPv6Pre->Prefix);
        
        p = strchr(dup, '/');
        if (p)
        {
            *p = 0;
        
            if (inet_pton(AF_INET6, dup, &in6) != 1)
            {
                rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Prefix");
                if(rc != EOK)
                {
                  ERR_CHK(rc);
		  // CID 175698 : Resource leak (RESOURCE_LEAK)
		  AnscFreeMemory(dup);
                  AnscFreeMemory(p);
                  dup = NULL;
                  p = NULL;
                  return FALSE;
                }
                *puLength = AnscSizeOfString("Prefix");

                CcspTraceWarning(("IPv6Prefix_Validate() on Prefix failed.\n"));
                AnscFreeMemory(dup);/*RDKB-, CID-33094, free unused resources before exit*/
                return FALSE;
            }
        }
        AnscFreeMemory(dup);
        
    }



    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv6Prefix_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlIpIfAddV6Prefix(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Pre);

        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlIpIfSetV6Prefix(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Pre);
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPv6Prefix_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    CcspTraceWarning(("IPv6Prefix_Rollback()----begin to rollback......\n"));

    CosaDmlIpIfGetV6Prefix2(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Pre);

    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.Stats.

    *  Stats5_GetParamBoolValue
    *  Stats5_GetParamIntValue
    *  Stats5_GetParamUlongValue
    *  Stats5_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats5_GetParamBoolValue
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
Stats5_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats5_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Stats5_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats5_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Stats5_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DML_IP_STATS              pIPStats      = (PCOSA_DML_IP_STATS)AnscAllocateMemory(sizeof(COSA_DML_IP_STATS));
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIfFull = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    if( !pIPStats )
    {
        return FALSE;
    }
   
    returnStatus = CosaDmlIpIfGetStats(pMyObject->hSbContext, pIfFull->Cfg.InstanceNumber, pIPStats);

    if( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto FAILED;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "BytesSent") == 0)
    {
        /* collect value */
        *puLong = pIPStats->BytesSent;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        /* collect value */
        *puLong = pIPStats->BytesReceived;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "PacketsSent") == 0)
    {
        /* collect value */
        *puLong = pIPStats->PacketsSent;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        /* collect value */
        *puLong = pIPStats->PacketsReceived;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "ErrorsSent") == 0)
    {
        /* collect value */
        *puLong = pIPStats->ErrorsSent;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "ErrorsReceived") == 0)
    {
        /* collect value */
        *puLong = pIPStats->ErrorsReceived;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "UnicastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = pIPStats->UnicastPacketsSent;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "UnicastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = pIPStats->UnicastPacketsReceived;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "DiscardPacketsSent") == 0)
    {
        /* collect value */
        *puLong = pIPStats->DiscardPacketsSent;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "DiscardPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = pIPStats->DiscardPacketsReceived;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "MulticastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = pIPStats->MulticastPacketsSent;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "MulticastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = pIPStats->MulticastPacketsReceived;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "BroadcastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = pIPStats->BroadcastPacketsSent;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "BroadcastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = pIPStats->BroadcastPacketsReceived;
        goto SUCCESS;
    }

    if (strcmp(ParamName, "UnknownProtoPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = pIPStats->UnknownProtoPacketsReceived;
        goto SUCCESS;
    }

SUCCESS:
    AnscFreeMemory(pIPStats);
    return TRUE;

FAILED:
    AnscFreeMemory(pIPStats);
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Stats5_GetParamStringValue
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
Stats5_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    IP.ActivePort.{i}.

    *  ActivePort_GetEntryCount
    *  ActivePort_GetEntry
    *  ActivePort_IsUpdated
    *  ActivePort_Synchronize
    *  ActivePort_GetParamBoolValue
    *  ActivePort_GetParamIntValue
    *  ActivePort_GetParamUlongValue
    *  ActivePort_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ActivePort_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
ActivePort_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
    ULONG                           entryCount    = 0;
    
    entryCount = pMyObject->activePortCount;
    
    return entryCount;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        ActivePort_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
ActivePort_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
    ULONG                           entryCount    = 0;
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort   = (PCOSA_DML_IP_ACTIVE_PORT)pMyObject->pActivePort;
    
    entryCount   = pMyObject->activePortCount;
    *pInsNumber  = nIndex + 1; 

    if( nIndex > (entryCount - 1))
    {
        return NULL;
    }
    
    return (ANSC_HANDLE)&pActivePort[nIndex];
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ActivePort_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
ActivePort_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (!last_tick) 
    {
        last_tick = AnscGetTickInSeconds();
        return TRUE;
    }
    
    if (last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - REFRESH_INTERVAL))
    {
        return FALSE;
    }
    else 
    {
        last_tick = AnscGetTickInSeconds();
        return TRUE;
    }
    
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ActivePort_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
ActivePort_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
#if 0
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort   = (PCOSA_DML_IP_ACTIVE_PORT)NULL;
    ULONG                           activePortCount 
                                                  = 0;
#endif
    
    /*pActivePort                = CosaDmlIpGetActivePorts(pMyObject->hSbContext,&activePortCount);*/
    pMyObject->activePortCount = 0; /*activePortCount;*/

    if( pMyObject->pActivePort )
    {
        AnscFreeMemory(pMyObject->pActivePort);
        pMyObject->pActivePort = NULL;
    }
    
    return returnStatus;
     /* CID: 68242 Structurally dead code*/
#if 0
    pMyObject->pActivePort     = (PCOSA_DML_IP_ACTIVE_PORT)
                                 AnscAllocateMemory(sizeof(COSA_DML_IP_ACTIVE_PORT)*activePortCount);
    
    if( !pMyObject->pActivePort )
    {
        AnscFreeMemory(pActivePort);
        return ANSC_STATUS_RESOURCES;
    }
    
    AnscCopyMemory(pMyObject->pActivePort,pActivePort,sizeof(COSA_DML_IP_ACTIVE_PORT)*activePortCount);

    AnscFreeMemory(pActivePort);
    return returnStatus;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ActivePort_GetParamBoolValue
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
ActivePort_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ActivePort_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
ActivePort_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ActivePort_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
ActivePort_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort   = (PCOSA_DML_IP_ACTIVE_PORT)hInsContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "LocalPort") == 0)
    {
        /* collect value */
        *puLong = pActivePort->LocalPort;
        return TRUE;
    }

    if (strcmp(ParamName, "RemotePort") == 0)
    {
        /* collect value */
        *puLong = pActivePort->RemotePort;
        return TRUE;
    }

    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong = pActivePort->Status;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ActivePort_GetParamStringValue
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
ActivePort_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort   = (PCOSA_DML_IP_ACTIVE_PORT)hInsContext;
    errno_t                         rc            = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "LocalIPAddress") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pActivePort->LocalIPAddress);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "RemoteIPAddress") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pActivePort->RemoteIPAddress);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

#if 0
/***********************************************************************

 APIs for Object:

    IP.Diagnostics.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    IP.Diagnostics.X_CISCO_COM_ARP.

    *  X_CISCO_COM_ARP_GetParamBoolValue
    *  X_CISCO_COM_ARP_GetParamIntValue
    *  X_CISCO_COM_ARP_GetParamUlongValue
    *  X_CISCO_COM_ARP_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ARP_GetParamBoolValue
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
X_CISCO_COM_ARP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ARP_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_CISCO_COM_ARP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ARP_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_CISCO_COM_ARP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_ARP_GetParamStringValue
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
X_CISCO_COM_ARP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.X_CISCO_COM_ARP.Table.{i}.

    *  ARPTable_GetEntryCount
    *  ARPTable_GetEntry
    *  ARPTable_GetParamBoolValue
    *  ARPTable_GetParamIntValue
    *  ARPTable_GetParamUlongValue
    *  ARPTable_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ARPTable_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
ARPTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)pMyObject->pArpTable;
    ULONG                           entryCount          = pMyObject->ArpEntryCount;

    return entryCount;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        ARPTable_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
ARPTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)pMyObject->pArpTable;
    ULONG                           entryCount          = pMyObject->ArpEntryCount;

    *pInsNumber  = nIndex + 1; 
    
    return (ANSC_HANDLE)&pArpTable[nIndex]; /* return the handle */
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ARPTable_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
ARPTable_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)pMyObject->pArpTable;
    ULONG                           entryCount          = pMyObject->ArpEntryCount;
    BOOL                            bIsUpdated   = TRUE;

    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pMyObject->PreviousVisitTime ) < COSA_DML_DIAG_ARP_TABLE_ACCESS_INTERVAL )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pMyObject->PreviousVisitTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    return bIsUpdated;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ARPTable_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
ARPTable_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DIAG            pMyObject         = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable         = (PCOSA_DML_DIAG_ARP_TABLE)pMyObject->pArpTable;
    ULONG                           entryCount        = pMyObject->ArpEntryCount;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable2        = NULL;

    pArpTable2         = CosaDmlDiagGetARPTable(NULL,&entryCount);
    if ( !pArpTable2 )
    {
        /* Get Error, we don't del link because next time, it may be successful */
        return ANSC_STATUS_FAILURE;
    }

    if ( pArpTable )
    {
        AnscFreeMemory(pArpTable);
    }

    pMyObject->pArpTable     = pArpTable2;
    pMyObject->ArpEntryCount = entryCount;

    returnStatus =  ANSC_STATUS_SUCCESS;

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ARPTable_GetParamBoolValue
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
ARPTable_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ARPTable_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
ARPTable_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ARPTable_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
ARPTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)hInsContext;

    /* check the parameter name and return the corresponding value */

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        /* collect value */
        *puLong    =  pArpTable->IPAddress.Value;
        
        return TRUE;
    }


    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ARPTable_GetParamStringValue
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
ARPTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "MACAddress") == 0)
    {
        /* collect value */
        if ( sizeof(pArpTable->MACAddress) <= *pUlSize)
        {
            _ansc_sprintf
                (
                    pValue, 
                    "%02x:%02x:%02x:%02x:%02x:%02x", 
                    pArpTable->MACAddress[0],
                    pArpTable->MACAddress[1],
                    pArpTable->MACAddress[2],
                    pArpTable->MACAddress[3],
                    pArpTable->MACAddress[4],
                    pArpTable->MACAddress[5]
                );
            
            return 0;
        }
        else
        {
            *pUlSize = sizeof(pArpTable->MACAddress);
            return 1;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.IPPing.

    *  IPPing_GetParamBoolValue
    *  IPPing_GetParamIntValue
    *  IPPing_GetParamUlongValue
    *  IPPing_GetParamStringValue
    *  IPPing_SetParamBoolValue
    *  IPPing_SetParamIntValue
    *  IPPing_SetParamUlongValue
    *  IPPing_SetParamStringValue
    *  IPPing_Validate
    *  IPPing_Commit
    *  IPPing_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPPing_GetParamBoolValue
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
IPPing_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPPing_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPPing_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPPing_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPPing_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "DiagnosticsState") == 0)
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->DiagnosticState + 1;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping DiagnosticsState parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "NumberOfRepetitions") == 0)
    {
        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->NumberOfRepetitions;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping NumberOfRepetitions parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "Timeout") == 0)
    {
        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->Timeout;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping Timeout parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "DataBlockSize") == 0)
    {
        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->DataBlockSize;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping DataBlockSize parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "DSCP") == 0)
    {
        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->DSCP;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping DSCP parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "SuccessCount") == 0)
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->SuccessCount;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping SuccessCount parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "FailureCount") == 0)
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->FailureCount;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping FailureCount parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "AverageResponseTime") == 0)
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->AverageResponseTime;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping AverageResponseTime parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "MinimumResponseTime") == 0)
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->MinimumResponseTime;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping MinimumResponseTime parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "MaximumResponseTime") == 0)
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->MaximumResponseTime;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping MaximumResponseTime parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPPing_GetParamStringValue
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
IPPing_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;

    if (strcmp(ParamName, "Interface") == 0)
    {
        if ( pDiagPingInfo && AnscSizeOfString(pDiagPingInfo->Interface) < *pUlSize )
        {
            AnscCopyString(pValue, pDiagPingInfo->Interface);
        }
        else
        {
            if ( AnscSizeOfString(pDiagPingInfo->Interface) >= *pUlSize )
            {
                *pUlSize = AnscSizeOfString(pDiagPingInfo->Interface) + 1;
            }

            CcspTraceWarning(("Failed to get Ping Interface parameter!\n"));

            return -1;
        }
        return 0;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Host") == 0)
    {
        if ( pDiagPingInfo && AnscSizeOfString(pDiagPingInfo->Host) < *pUlSize )
        {
            AnscCopyString(pValue, pDiagPingInfo->Host);
        }
        else
        {
            if ( AnscSizeOfString(pDiagPingInfo->Host) >= *pUlSize )
            {
                *pUlSize = AnscSizeOfString(pDiagPingInfo->Host) + 1;
            }

            CcspTraceWarning(("Failed to get Ping Host parameter!\n"));
            return -1;
        }

        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPPing_SetParamBoolValue
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
IPPing_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPPing_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPPing_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPPing_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPPing_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;
    PDSLH_PING_INFO                 pDiagInfo           = NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DiagnosticsState") == 0)
    {
        if ( (uValue - 1) != (ULONG)DSLH_DIAG_STATE_TYPE_Requested )
        {
            return FALSE;
        }

        pDiagPingInfo->DiagnosticState = uValue - 1;

        return TRUE;
    }

    if (strcmp(ParamName, "NumberOfRepetitions") == 0)
    {
        pDiagPingInfo->NumberOfRepetitions = uValue;

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "Timeout") == 0)
    {
        pDiagPingInfo->Timeout             = uValue;

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "DataBlockSize") == 0)
    {
        pDiagPingInfo->DataBlockSize       = uValue;

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "DSCP") == 0)
    {
        pDiagPingInfo->DSCP                = uValue;

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPPing_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
IPPing_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;
    PDSLH_PING_INFO                 pDiagInfo           = NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
        AnscCopyString(pDiagPingInfo->Interface, pString);

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "Host") == 0)
    {
        AnscCopyString(pDiagPingInfo->Host, pString);

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPPing_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
IPPing_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;

    if ( !pDiagPingInfo )
    {
        return FALSE;
    }

    if ( pDiagPingInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )
    {
        if ( AnscSizeOfString(pDiagPingInfo->Host) == 0 )
        {
            AnscCopyString(pReturnParamName, "Host");

            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagPingInfo->NumberOfRepetitions < DSLH_PING_MIN_NumberOfRepetitions )
        {
            AnscCopyString(pReturnParamName, "NumberOfRepetitions");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagPingInfo->Timeout < DSLH_PING_MIN_Timeout )
        {
            AnscCopyString(pReturnParamName, "Timeout");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagPingInfo->DataBlockSize < DSLH_PING_MIN_DataBlockSize || pDiagPingInfo->DataBlockSize > DSLH_PING_MAX_DataBlockSize )
        {
            AnscCopyString(pReturnParamName, "DataBlockSize");

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPPing_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPPing_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;
    PDSLH_PING_INFO                 pDiagInfo           = NULL;

    if ( !pDiagPingInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

    if ( pDiagPingInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )
    {
        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
            CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_Ping);
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Requested;
            AnscSleep(1000);
        }

        CosaDmlDiagScheduleDiagnostic(DSLH_DIAGNOSTIC_TYPE_Ping, (ANSC_HANDLE)pDiagPingInfo);

        pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
    }

    if ( pDiagPingInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Canceled )
    {
        CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_Ping);

        pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPPing_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
IPPing_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;
    PDSLH_PING_INFO                 pDiagInfo           = NULL;

    if ( !pDiagPingInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

    if ( pDiagInfo )
    {
        DslhInitPingInfo(pDiagPingInfo);
        pDiagPingInfo->StructSize                    = sizeof(DSLH_PING_INFO);
        AnscCopyString(pDiagPingInfo->Host           , pDiagInfo->Host     );
        AnscCopyString(pDiagPingInfo->Interface      , pDiagInfo->Interface);
        pDiagPingInfo->Timeout                       = pDiagInfo->Timeout;
        pDiagPingInfo->NumberOfRepetitions           = pDiagInfo->NumberOfRepetitions;
        pDiagPingInfo->DataBlockSize                 = pDiagInfo->DataBlockSize;
    }
    else
    {
        DslhInitPingInfo(pDiagPingInfo);
    }

    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.TraceRoute.

    *  TraceRoute_GetParamBoolValue
    *  TraceRoute_GetParamIntValue
    *  TraceRoute_GetParamUlongValue
    *  TraceRoute_GetParamStringValue
    *  TraceRoute_SetParamBoolValue
    *  TraceRoute_SetParamIntValue
    *  TraceRoute_SetParamUlongValue
    *  TraceRoute_SetParamStringValue
    *  TraceRoute_Validate
    *  TraceRoute_Commit
    *  TraceRoute_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        TraceRoute_GetParamBoolValue
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
TraceRoute_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        TraceRoute_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
TraceRoute_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        TraceRoute_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
TraceRoute_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "DiagnosticsState") == 0)
    {
        pDiagTracerouteInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->DiagnosticState + 1;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute DiagnosticsState parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "NumberOfTries") == 0)
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->NumberOfTries;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute NumberOfTries parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "Timeout") == 0)
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->Timeout;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute Timeout parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "DataBlockSize") == 0)
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->DataBlockSize;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute DataBlockSize parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "DSCP") == 0)
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->DSCP;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute DSCP parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "MaxHopCount") == 0)
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->MaxHopCount;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute MaxHopCount parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "ResponseTime") == 0)
    {
        pDiagTracerouteInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->ResponseTime;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute ResponseTime parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        TraceRoute_GetParamStringValue
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
TraceRoute_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
        if ( pDiagTracerouteInfo && AnscSizeOfString(pDiagTracerouteInfo->Interface) < *pUlSize )
        {
            AnscCopyString(pValue, pDiagTracerouteInfo->Interface);
        }
        else
        {
            if ( AnscSizeOfString(pDiagTracerouteInfo->Interface) >= *pUlSize )
            {
                *pUlSize = AnscSizeOfString(pDiagTracerouteInfo->Interface) + 1;
            }

            CcspTraceWarning(("Failed to get Traceroute Interface parameter!\n"));
            return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "Host") == 0)
    {
        if ( pDiagTracerouteInfo && AnscSizeOfString(pDiagTracerouteInfo->Host) < *pUlSize )
        {
            AnscCopyString(pValue, pDiagTracerouteInfo->Host);
        }
        else
        {
            if ( AnscSizeOfString(pDiagTracerouteInfo->Host) >= *pUlSize )
            {
                *pUlSize = AnscSizeOfString(pDiagTracerouteInfo->Host) + 1;
            }

            CcspTraceWarning(("Failed to get Traceroute Host parameter!\n"));
            return -1;
        }

        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        TraceRoute_SetParamBoolValue
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
TraceRoute_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        TraceRoute_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
TraceRoute_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        TraceRoute_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
TraceRoute_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DiagnosticsState") == 0)
    {
        if ( (uValue - 1) != (ULONG)DSLH_DIAG_STATE_TYPE_Requested )
        {
            return FALSE;
        }

        pDiagTracerouteInfo->DiagnosticState = uValue - 1;

        return TRUE;
    }

    if (strcmp(ParamName, "NumberOfTries") == 0)
    {
        pDiagTracerouteInfo->NumberOfTries = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "Timeout") == 0)
    {
        pDiagTracerouteInfo->Timeout = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "DataBlockSize") == 0)
    {
        pDiagTracerouteInfo->DataBlockSize = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "DSCP") == 0)
    {
        pDiagTracerouteInfo->DSCP = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "MaxHopCount") == 0)
    {
        pDiagTracerouteInfo->MaxHopCount = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        TraceRoute_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
TraceRoute_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
        AnscCopyString(pDiagTracerouteInfo->Interface, pString);

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "Host") == 0)
    {
        AnscCopyString(pDiagTracerouteInfo->Host, pString);

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        TraceRoute_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
TraceRoute_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;

    if ( !pDiagTracerouteInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( pDiagTracerouteInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )
    {
        if ( AnscSizeOfString(pDiagTracerouteInfo->Host) == 0 )
        {
            AnscCopyString(pReturnParamName, "Host");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagTracerouteInfo->NumberOfTries < DSLH_TRACEROUTE_MIN_NumberOfTries )
        {
            AnscCopyString(pReturnParamName, "NumberOfTries");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagTracerouteInfo->Timeout < DSLH_TRACEROUTE_MIN_Timeout )
        {
            AnscCopyString(pReturnParamName, "Timeout");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagTracerouteInfo->DataBlockSize < DSLH_TRACEROUTE_MIN_DataBlockSize || pDiagTracerouteInfo->DataBlockSize > DSLH_TRACEROUTE_MAX_DataBlockSize )
        {
            AnscCopyString(pReturnParamName, "DataBlockSize");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagTracerouteInfo->MaxHopCount < DSLH_TRACEROUTE_MIN_MaxHopCount || pDiagTracerouteInfo->MaxHopCount > DSLH_TRACEROUTE_MAX_HOPS_COUNT )
        {
            AnscCopyString(pReturnParamName, "MaxHopCount");

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        TraceRoute_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
TraceRoute_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    if ( !pDiagTracerouteInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagTracerouteInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )
    {
        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
            CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_Traceroute);
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Requested;
            AnscSleep(1000);
        }

        CosaDmlDiagScheduleDiagnostic(DSLH_DIAGNOSTIC_TYPE_Traceroute, (ANSC_HANDLE)pDiagTracerouteInfo);

        pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
    }

    if ( pDiagTracerouteInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Canceled )
    {
        CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        TraceRoute_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
TraceRoute_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    if ( !pDiagTracerouteInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagInfo )
    {
        DslhInitTracerouteInfo(pDiagTracerouteInfo);
        pDiagTracerouteInfo->StructSize                = sizeof(DSLH_TRACEROUTE_INFO);
        AnscCopyString(pDiagTracerouteInfo->Host     , pDiagInfo->Host     );
        AnscCopyString(pDiagTracerouteInfo->Interface, pDiagInfo->Interface);
        pDiagTracerouteInfo->Timeout                   = pDiagInfo->Timeout;
        pDiagTracerouteInfo->MaxHopCount               = pDiagInfo->MaxHopCount;
        pDiagTracerouteInfo->NumberOfTries             = pDiagInfo->NumberOfTries;
        pDiagTracerouteInfo->DataBlockSize             = pDiagInfo->DataBlockSize;
        pDiagTracerouteInfo->UpdatedAt                 = pDiagInfo->UpdatedAt;
    }
    else
    {
        DslhInitTracerouteInfo(pDiagTracerouteInfo);
    }

    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.TraceRoute.RouteHops.{i}.

    *  RouteHops_GetEntryCount
    *  RouteHops_GetEntry
    *  RouteHops_IsUpdated
    *  RouteHops_Synchronize
    *  RouteHops_GetParamBoolValue
    *  RouteHops_GetParamIntValue
    *  RouteHops_GetParamUlongValue
    *  RouteHops_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RouteHops_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
RouteHops_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = NULL;

    pDiagTracerouteInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagTracerouteInfo && pDiagTracerouteInfo->DiagnosticState != DSLH_DIAG_STATE_TYPE_None 
         && pDiagTracerouteInfo->DiagnosticState != DSLH_DIAG_STATE_TYPE_Requested )
    {
        return pDiagTracerouteInfo->RouteHopsNumberOfEntries;
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        RouteHops_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
RouteHops_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo= NULL;
    PDSLH_ROUTEHOPS_INFO            pRouteHop          = (PDSLH_ROUTEHOPS_INFO       )NULL;

    pDiagTracerouteInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagTracerouteInfo )
    {
        pRouteHop = (PDSLH_ROUTEHOPS_INFO)&pDiagTracerouteInfo->hDiagRouteHops[nIndex];
    }

    if ( !pRouteHop )
    {
        return  (ANSC_HANDLE)NULL;
    }

    *pInsNumber  = nIndex + 1; 
    return pRouteHop;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouteHops_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
RouteHops_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    if ( !pDiagTracerouteInfo )
    {
        return FALSE;
    }

    pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagInfo && pDiagTracerouteInfo->UpdatedAt != pDiagInfo->UpdatedAt )
    {
        return  TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RouteHops_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
RouteHops_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    if ( !pDiagTracerouteInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( !pDiagInfo )
    {
        CcspTraceWarning(("Failed to get Traceroute backend information!\n"));

        return  ANSC_STATUS_FAILURE;
    }

    pDiagTracerouteInfo->UpdatedAt = pDiagInfo->UpdatedAt;

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouteHops_GetParamBoolValue
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
RouteHops_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouteHops_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
RouteHops_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouteHops_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
RouteHops_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PDSLH_ROUTEHOPS_INFO            pRouteHop          = (PDSLH_ROUTEHOPS_INFO       )hInsContext;

    if ( !pRouteHop )
    {
        CcspTraceWarning(("Failed to get route hops parameters hInsContext!\n"));

        return FALSE;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "ErrorCode") == 0)
    {
        *puLong = pRouteHop->HopErrorCode;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RouteHops_GetParamStringValue
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
RouteHops_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PDSLH_ROUTEHOPS_INFO            pRouteHop          = (PDSLH_ROUTEHOPS_INFO       )hInsContext;

    if ( !pRouteHop )
    {
        CcspTraceWarning(("Failed to get route hops parameters hInsContext!\n"));

        return FALSE;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Host") == 0)
    {
        if ( AnscSizeOfString(pRouteHop->HopHost) < *pUlSize )
        {
            AnscCopyString(pValue, pRouteHop->HopHost);

            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pRouteHop->HopHost) + 1;

            return -1;
        }
    }

    if (strcmp(ParamName, "HostAddress") == 0)
    {
        if ( AnscSizeOfString(pRouteHop->HopHostAddress) < *pUlSize )
        {
            AnscCopyString(pValue, pRouteHop->HopHostAddress);

            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pRouteHop->HopHostAddress) + 1;

            return -1;
        }
    }

    if (strcmp(ParamName, "RTTimes") == 0)
    {
        if ( AnscSizeOfString(pRouteHop->HopRTTimes) < *pUlSize )
        {
            AnscCopyString(pValue, pRouteHop->HopRTTimes);

            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pRouteHop->HopRTTimes) + 1;

            return -1;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}
/***********************************************************************
   

 APIs for Object:

    IP.Diagnostics.DownloadDiagnostics.

    *  DownloadDiagnostics_GetParamBoolValue
    *  DownloadDiagnostics_GetParamIntValue
    *  DownloadDiagnostics_GetParamUlongValue
    *  DownloadDiagnostics_GetParamStringValue
    *  DownloadDiagnostics_SetParamBoolValue
    *  DownloadDiagnostics_SetParamIntValue
    *  DownloadDiagnostics_SetParamUlongValue
    *  DownloadDiagnostics_SetParamStringValue
    *  DownloadDiagnostics_Validate
    *  DownloadDiagnostics_Commit
    *  DownloadDiagnostics_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_GetParamBoolValue
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
DownloadDiagnostics_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DownloadDiagnostics_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DownloadDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;
    PDSLH_TR143_DOWNLOAD_DIAG_STATS pDownloadDiagStats = NULL;


    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "DiagnosticsState") == 0)
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );

        if ( pDownloadDiagStats )
        {
            *puLong = pDownloadDiagStats->DiagStates + 1;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get DiagnosticsState\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if (strcmp(ParamName, "DSCP") == 0)
    {
        if ( pDownloadInfo )
        {
            *puLong = pDownloadInfo->DSCP;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get DSCP \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "EthernetPriority") == 0)
    {
        if ( pDownloadInfo )
        {
            *puLong = pDownloadInfo->EthernetPriority;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get EthernetPriority \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "TestBytesReceived") == 0)
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );        

        if ( pDownloadDiagStats )
        {
            *puLong = pDownloadDiagStats->TestBytesReceived;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get TestBytesReceived\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if (strcmp(ParamName, "TotalBytesReceived") == 0)
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );


        if ( pDownloadDiagStats )
        {
            *puLong = pDownloadDiagStats->TotalBytesReceived;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get TotalBytesReceived\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DownloadDiagnostics_GetParamStringValue
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
DownloadDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;
    PDSLH_TR143_DOWNLOAD_DIAG_STATS pDownloadDiagStats = NULL;
    PANSC_UNIVERSAL_TIME            pTime              = NULL;
    char                            pBuf[128]          = { 0 };


    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
        if ( pDownloadInfo )
        {
            AnscCopyString(pValue, pDownloadInfo->Interface);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get Interface\n!"));
            return -1;
        }
        
        return 0;
    }
    
    if (strcmp(ParamName, "DownloadURL") == 0)
    {
        if ( pDownloadInfo )
        {
            AnscCopyString(pValue, pDownloadInfo->DownloadURL);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get DownloadURL \n!"));
            return -1;
        }
        
        return 0;
    }

    if (strcmp(ParamName, "ROMTime") == 0)
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
        
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->ROMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
            
            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get ROMTime\n!"));

            return -1;
        }
        
        return 0;
    }

    if (strcmp(ParamName, "BOMTime") == 0)
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
        
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->BOMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
            
            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get BOMTime\n!"));

            return -1;
        }
        
        return 0;
    }

    if (strcmp(ParamName, "EOMTime") == 0)
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
                                
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->EOMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get EOMTime\n!"));

            return -1;
        }
        
        return 0;
    }

    if (strcmp(ParamName, "TCPOpenRequestTime") == 0)
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
                                        
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->TCPOpenRequestTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get TCPOpenRequestTime\n!"));

            return -1;
        }
        
        return 0;
    }

    if (strcmp(ParamName, "TCPOpenResponseTime") == 0)
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->TCPOpenResponseTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get TCPOpenResponseTime\n!"));

            return -1;
        }
        
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_SetParamBoolValue
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
DownloadDiagnostics_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DownloadDiagnostics_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DownloadDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;
    PDSLH_TR143_DOWNLOAD_DIAG_STATS pDownloadDiagStats = NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DiagnosticsState") == 0)
    {
        uValue--;
        if ( uValue != (ULONG)DSLH_TR143_DIAGNOSTIC_Requested )
        {
            return FALSE;
        }
        
        pDownloadInfo->DiagnosticsState = DSLH_TR143_DIAGNOSTIC_Requested;
        return TRUE;        
    }

    if (strcmp(ParamName, "DSCP") == 0)
    {
        pDownloadInfo->DSCP= uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "EthernetPriority") == 0)
    {
        pDownloadInfo->EthernetPriority = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DownloadDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;


    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
     /* Because not work across library, so disable this for later solution.*/ 
#if 0
       if ( !g_ValidateInterface(
            pString, 
            DSLH_WAN_LAYER3_CONNECTION_INTERFACE | DSLH_NULL_STRING_INTERFACE
            ))
        {
            return FALSE;
        }
#endif

        AnscCopyString(pDownloadInfo->Interface, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "DownloadURL") == 0)
    {
        if ( !pString || !(*pString) )
        {
            return FALSE;   
        }
 
        AnscCopyString(pDownloadInfo->DownloadURL, pString);
        return TRUE;
    }

    /*CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
DownloadDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;

    if ( pDownloadInfo->DiagnosticsState == DSLH_TR143_DIAGNOSTIC_Requested 
      && !AnscSizeOfString(pDownloadInfo->DownloadURL) )
    {
        AnscCopyString(pReturnParamName, "DownloadURL");
        return FALSE;
    }

    return  TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DownloadDiagnostics_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
DownloadDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;


    if ( pDownloadInfo->DiagnosticsState != DSLH_TR143_DIAGNOSTIC_Requested )
    {
        pDownloadInfo->DiagnosticsState = DSLH_TR143_DIAGNOSTIC_None;
    }

    returnStatus = CosaDmlDiagScheduleDiagnostic
                    (
                        DSLH_DIAGNOSTIC_TYPE_Download, 
                        (ANSC_HANDLE)pDownloadInfo
                    );
 
    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DownloadDiagnostics_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
DownloadDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadPreInfo   = NULL; 

    if ( !pDownloadInfo )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    DslhInitDownloadDiagInfo(pDownloadInfo);

    pDownloadPreInfo = (PDSLH_TR143_DOWNLOAD_DIAG_INFO)CosaDmlDiagGetConfigs
                        (
                            DSLH_DIAGNOSTIC_TYPE_Download
                        );

    if ( pDownloadPreInfo )
    {
        AnscCopyString(pDownloadInfo->Interface, pDownloadPreInfo->Interface);
        AnscCopyString(pDownloadInfo->DownloadURL, pDownloadPreInfo->DownloadURL);
        pDownloadInfo->DSCP             = pDownloadPreInfo->DSCP;
        pDownloadInfo->EthernetPriority = pDownloadPreInfo->EthernetPriority;
        pDownloadInfo->DiagnosticsState = pDownloadPreInfo->DiagnosticsState;
    }
    else
    {
       CcspTraceWarning(("Download Diagnostics---Failed to get previous configuration!\n")); 
    }

    return 0;
}


/***********************************************************************

 APIs for Object:

    IP.Diagnostics.UploadDiagnostics.

    *  UploadDiagnostics_GetParamBoolValue
    *  UploadDiagnostics_GetParamIntValue
    *  UploadDiagnostics_GetParamUlongValue
    *  UploadDiagnostics_GetParamStringValue
    *  UploadDiagnostics_SetParamBoolValue
    *  UploadDiagnostics_SetParamIntValue
    *  UploadDiagnostics_SetParamUlongValue
    *  UploadDiagnostics_SetParamStringValue
    *  UploadDiagnostics_Validate
    *  UploadDiagnostics_Commit
    *  UploadDiagnostics_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_GetParamBoolValue
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
UploadDiagnostics_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UploadDiagnostics_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UploadDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_STATS   pUploadDiagStats = NULL;

  
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "DiagnosticsState") == 0)
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );

        if ( pUploadDiagStats )
        {
            *puLong = pUploadDiagStats->DiagStates + 1;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get DiagnosticsState\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if (strcmp(ParamName, "DSCP") == 0)
    {
        if ( pUploadInfo )
        {
            *puLong = pUploadInfo->DSCP;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get DSCP \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "EthernetPriority") == 0)
    {
        if ( pUploadInfo )
        {
            *puLong = pUploadInfo->EthernetPriority;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get EthernetPriority \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "TestFileLength") == 0)
    {
        if ( pUploadInfo )
        {
            *puLong = pUploadInfo->TestFileLength;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get TestFileLength \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "TotalBytesSent") == 0)
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );

        if ( pUploadDiagStats )
        {
            *puLong = pUploadDiagStats->TotalBytesSent;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get TotalBytesSent\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UploadDiagnostics_GetParamStringValue
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
UploadDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_STATS   pUploadDiagStats = NULL;
    PANSC_UNIVERSAL_TIME            pTime            = NULL;
    char                            pBuf[128]        = { 0 };


    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
        if ( pUploadInfo )
        {
            AnscCopyString(pValue, pUploadInfo->Interface);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get Interface \n!"));
            return -1;
        }
        
        return 0;
    }

    if (strcmp(ParamName, "UploadURL") == 0)
    {
        if ( pUploadInfo )
        {
            AnscCopyString(pValue, pUploadInfo->UploadURL);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get UploadURL \n!"));
            return -1;
        }
        
        return 0;
    }

    if (strcmp(ParamName, "ROMTime") == 0)
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );
        
        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->ROMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
           
           AnscCopyString(pValue, pBuf);
        }
       else
       {
            CcspTraceWarning(("Upload Diagnostics---Failed to get ROMTime\n!"));

            return -1;
       }
       
       return 0;
    }
    
    if (strcmp(ParamName, "BOMTime") == 0)
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );

        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->BOMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get BOMTime\n!"));

            return -1;
        }

        return 0;
    }
    
    if (strcmp(ParamName, "EOMTime") == 0)
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );
                            
        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->EOMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get EOMTime\n!"));

            return -1;
        }

        return 0;
    }
    
    if (strcmp(ParamName, "TCPOpenRequestTime") == 0)
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );
                            
        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->TCPOpenRequestTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get TCPOpenRequestTime\n!"));

            return -1;
        }

        return 0;
    }
    
    if (strcmp(ParamName, "TCPOpenResponseTime") == 0)
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );
        
        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->TCPOpenResponseTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get TCPOpenResponseTime\n!"));

            return -1;
        }

        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_SetParamBoolValue
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
UploadDiagnostics_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UploadDiagnostics_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UploadDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_STATS   pUploadDiagStats = NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DiagnosticsState") == 0)
    {
        uValue--;
        if ( uValue != (ULONG)DSLH_TR143_DIAGNOSTIC_Requested )
        {
            return FALSE;
        }
        
        pUploadInfo->DiagnosticsState = DSLH_TR143_DIAGNOSTIC_Requested;
        return TRUE;        
    }

    if (strcmp(ParamName, "DSCP") == 0)
    {
        pUploadInfo->DSCP = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "EthernetPriority") == 0)
    {
        pUploadInfo->EthernetPriority = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "TestFileLength") == 0)
    {
        if ( uValue == 0 )
        {
            return FALSE;
        }
        
        pUploadInfo->TestFileLength = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UploadDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_STATS   pUploadDiagStats = NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
     /* Because not work across library, so disable this for later solution.*/ 
#if 0
        if ( !g_ValidateInterface(
            pString, 
            DSLH_WAN_LAYER3_CONNECTION_INTERFACE | DSLH_NULL_STRING_INTERFACE
            ))
        {
            return FALSE;
        }
#endif        

        AnscCopyString(pUploadInfo->Interface, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "UploadURL") == 0)
    {
        if ( !pString || !(*pString) )
        {
            return FALSE;   
        }
        
        AnscCopyString(pUploadInfo->UploadURL, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
UploadDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject    = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo  = pMyObject->hDiagUploadInfo;

    if ( pUploadInfo->DiagnosticsState == DSLH_TR143_DIAGNOSTIC_Requested 
       && !AnscSizeOfString(pUploadInfo->UploadURL) )
    {
        AnscCopyString(pReturnParamName, "UploadURL");
        return FALSE;
    }
    return  TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UploadDiagnostics_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
UploadDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DIAG            pMyObject    = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo  = pMyObject->hDiagUploadInfo;


    if ( pUploadInfo->DiagnosticsState != DSLH_TR143_DIAGNOSTIC_Requested )
    {
        pUploadInfo->DiagnosticsState = DSLH_TR143_DIAGNOSTIC_None;
    }

    returnStatus = CosaDmlDiagScheduleDiagnostic
                    (
                        DSLH_DIAGNOSTIC_TYPE_Upload, 
                        (ANSC_HANDLE)pUploadInfo
                    );

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UploadDiagnostics_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
UploadDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadPreInfo   = NULL; 

    if ( !pUploadInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    DslhInitUploadDiagInfo(pUploadInfo);

    pUploadPreInfo = (PDSLH_TR143_UPLOAD_DIAG_INFO)CosaDmlDiagGetConfigs
                        (
                            DSLH_DIAGNOSTIC_TYPE_Upload
                        );

    if ( pUploadPreInfo )
    {
        AnscCopyString(pUploadInfo->Interface, pUploadPreInfo->Interface);
        AnscCopyString(pUploadInfo->UploadURL, pUploadPreInfo->UploadURL);
        pUploadInfo->DSCP                 = pUploadPreInfo->DSCP;
        pUploadInfo->EthernetPriority     = pUploadPreInfo->EthernetPriority;
        pUploadInfo->TestFileLength       = pUploadPreInfo->TestFileLength;
        pUploadInfo->DiagnosticsState     = pUploadPreInfo->DiagnosticsState;
    }
    else
    {
        CcspTraceWarning(("Upload Diagnostics---Failed to get previous configuration!\n")); 
    }

    return 0;
}


/***********************************************************************

 APIs for Object:

    IP.Diagnostics.UDPEchoConfig.

    *  UDPEchoConfig_GetParamBoolValue
    *  UDPEchoConfig_GetParamIntValue
    *  UDPEchoConfig_GetParamUlongValue
    *  UDPEchoConfig_GetParamStringValue
    *  UDPEchoConfig_SetParamBoolValue
    *  UDPEchoConfig_SetParamIntValue
    *  UDPEchoConfig_SetParamUlongValue
    *  UDPEchoConfig_SetParamStringValue
    *  UDPEchoConfig_Validate
    *  UDPEchoConfig_Commit
    *  UDPEchoConfig_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_GetParamBoolValue
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
UDPEchoConfig_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        if ( pUdpEchoInfo )
        {
            *pBool = pUdpEchoInfo->Enable;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get Enable \n!"));
            *pBool = FALSE;
            return FALSE;
        }

        return TRUE;
    }
    
    if (strcmp(ParamName, "EchoPlusEnabled") == 0)
    {
        if ( pUdpEchoInfo )
        {
            *pBool = pUdpEchoInfo->EchoPlusEnabled;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get EchoPlusEnabled \n!"));
            *pBool = FALSE;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "EchoPlusSupported") == 0)
    {
        if ( pUdpEchoInfo )
        {
            *pBool = pUdpEchoInfo->EchoPlusSupported;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get EchoPlusSupported \n!"));
            *pBool = FALSE;
            return FALSE;
        }

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UDPEchoConfig_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UDPEchoConfig_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "SourceIPAddress") == 0)
    {
        if ( pUdpEchoInfo )
        {
            *puLong = pUdpEchoInfo->SourceIPAddress;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get SourceIPAddress \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "UDPPort") == 0)
    {
        if ( pUdpEchoInfo )
        {
            *puLong = pUdpEchoInfo->UDPPort;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get UDPPort \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                        (
                            DSLH_DIAGNOSTIC_TYPE_UdpEcho
                        );

        if ( pUdpEchoStats )
        {
            *puLong = pUdpEchoStats->PacketsReceived;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get PacketsReceived\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if (strcmp(ParamName, "PacketsResponded") == 0)
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                        (
                            DSLH_DIAGNOSTIC_TYPE_UdpEcho
                        );        

        if ( pUdpEchoStats )
        {
            *puLong = pUdpEchoStats->PacketsResponded;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get PacketsResponded\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                        (
                            DSLH_DIAGNOSTIC_TYPE_UdpEcho
                        );

        if ( pUdpEchoStats )
        {
            *puLong = pUdpEchoStats->BytesReceived;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get BytesReceived\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if (strcmp(ParamName, "BytesResponded") == 0)
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                        (
                            DSLH_DIAGNOSTIC_TYPE_UdpEcho
                        );

        if ( pUdpEchoStats )
        {
            *puLong = pUdpEchoStats->BytesResponded;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get BytesResponded\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UDPEchoConfig_GetParamStringValue
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
UDPEchoConfig_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;
    PANSC_UNIVERSAL_TIME            pTime         = NULL;
    char                            pBuf[128]     = { 0 };


    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
        if ( pUdpEchoInfo )
        {
            AnscCopyString(pValue, pUdpEchoInfo->Interface);
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get Interface\n!"));
            return -1;
        }
        
        return 0;
    }

    if (strcmp(ParamName, "TimeFirstPacketReceived") == 0)
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_UdpEcho
                            );
        
        if ( pUdpEchoStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUdpEchoStats->TimeFirstPacketReceived;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
            
            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get TimeFirstPacketReceived\n!"));

            return -1;
        }
        
        return 0;
    }

    if (strcmp(ParamName, "TimeLastPacketReceived") == 0)
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_UdpEcho
                            );
        
        if ( pUdpEchoStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUdpEchoStats->TimeLastPacketReceived;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
            
            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get TimeLastPacketReceived\n!"));

            return -1;
        }
        
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_SetParamBoolValue
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
UDPEchoConfig_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        pUdpEchoInfo->Enable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "EchoPlusEnabled") == 0)
    {
        pUdpEchoInfo->EchoPlusEnabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UDPEchoConfig_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UDPEchoConfig_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "SourceIPAddress") == 0)
    {
        if ( uValue == 0 )
        {
            return FALSE;
        }
            
        pUdpEchoInfo->SourceIPAddress = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "UDPPort") == 0)
    {
        if ( uValue == 0 )
        {
            return FALSE;
        } 

        pUdpEchoInfo->UDPPort = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
UDPEchoConfig_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;


    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
     /* Because not work across library, so disable this for later solution.*/ 
#if 0
       if ( !g_ValidateInterface(
            pString, 
            DSLH_WAN_LAYER3_CONNECTION_INTERFACE | DSLH_NULL_STRING_INTERFACE
            ))
        {
            return FALSE;
        }
#endif

        AnscCopyString(pUdpEchoInfo->Interface, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
UDPEchoConfig_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;


    if ( pUdpEchoInfo->EchoPlusEnabled && !pUdpEchoInfo->EchoPlusSupported )
    {
        AnscCopyString(pReturnParamName, "EchoPlusEnabled");
        return FALSE;
    }

    if ( pUdpEchoInfo->Enable && (pUdpEchoInfo->SourceIPAddress == 0) )
    {
        AnscCopyString(pReturnParamName, "SourceIPAddress");
        return FALSE;
    }
        
    if ( pUdpEchoInfo->Enable && (pUdpEchoInfo->UDPPort == 0) )
    {
        AnscCopyString(pReturnParamName, "UDPPort");
        return FALSE;
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UDPEchoConfig_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
UDPEchoConfig_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

   returnStatus = CosaDmlDiagScheduleDiagnostic
                (
                    DSLH_DIAGNOSTIC_TYPE_UdpEcho, 
                    (ANSC_HANDLE)pUdpEchoInfo
                );
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UDPEchoConfig_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
UDPEchoConfig_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject       = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo    = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoPreInfo = NULL; 

    if ( !pUdpEchoInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    DslhInitUDPEchoConfig(pUdpEchoInfo);

    pUdpEchoPreInfo = (PDSLH_TR143_UDP_ECHO_CONFIG)CosaDmlDiagGetConfigs
                    (
                        DSLH_DIAGNOSTIC_TYPE_UdpEcho
                    );

    if ( pUdpEchoPreInfo )
    {
        AnscCopyString(pUdpEchoInfo->Interface, pUdpEchoPreInfo->Interface);
        pUdpEchoInfo->Enable               = pUdpEchoPreInfo->Enable;
        pUdpEchoInfo->SourceIPAddress      = pUdpEchoPreInfo->SourceIPAddress;
        pUdpEchoInfo->UDPPort              = pUdpEchoPreInfo->UDPPort;
        pUdpEchoInfo->EchoPlusEnabled      = pUdpEchoPreInfo->EchoPlusEnabled;
        pUdpEchoInfo->EchoPlusSupported    = pUdpEchoPreInfo->EchoPlusSupported;
    }
    else
    {
        CcspTraceWarning(("UDP echo Diagnostics---Failed to get previous configuration!\n")); 
    }

    return 0;
}
#endif


