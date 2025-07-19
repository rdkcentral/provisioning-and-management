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

    module: cosa_interfacestack_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaIFStackCreate
        *  CosaIFStackInitialize
        *  CosaIFStackRemove
        *  
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
#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_interfacestack_internal.h"
#include "safec_lib_common.h"

#define TABLE_SIZE 256

static int CosaIFStackGetParamValueString(char *pParamName, char *pParamValue, ULONG *pValueSize);
static ULONG CosaIFStackGetParamValueULong(char *pParamName);
ANSC_STATUS COSAGetParamValueByPathName(void* bus_handle, parameterValStruct_t *val, ULONG *parameterValueLength);

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaIFStackCreate
            (
            );

    description:

        This function constructs cosa interfacestack object and return handle.

    argument:  

    return:     newly created interfacestack object.

**********************************************************************/

ANSC_HANDLE
CosaIFStackCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_IFSTACK         pMyObject    = (PCOSA_DATAMODEL_IFSTACK)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_IFSTACK)AnscAllocateMemory(sizeof(COSA_DATAMODEL_IFSTACK));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_INTERFACESTACK_OID;
    pMyObject->Create            = CosaIFStackCreate;
    pMyObject->Initialize        = CosaIFStackInitialize;
    pMyObject->Remove            = CosaIFStackRemove;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaIFStackInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate cosa interfaceStack object and return status.

    argument: ANSC_HANDLE                 hThisObject
                  This handle is actually the pointer of this object itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaIFStackInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IFSTACK         pMyObject    = (PCOSA_DATAMODEL_IFSTACK)hThisObject;
    
    /* Initiation all functions */
    pMyObject->ulEntryCount = 0;
    
    AnscSListInitializeHeader(&pMyObject->InterfaceStackList)

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaIFStackRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function remove interfaceStack object and return status.

    argument:   ANSC_HANDLE                 hThisObject
                    This handle is actually the pointer of this object itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaIFStackRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IFSTACK         pMyObject       = (PCOSA_DATAMODEL_IFSTACK)hThisObject;
    PSLIST_HEADER                   pIFStackHead    = (PSLIST_HEADER)&pMyObject->InterfaceStackList;
    PCOSA_DML_IFSTACK_ENTRY         pInterfaceStack = (PCOSA_DML_IFSTACK_ENTRY)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    /* InterfaceStack List */
    pLink = AnscSListGetFirstEntry(pIFStackHead);

    while ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink        = AnscSListGetNextEntry(pLink);

        pInterfaceStack = (PCOSA_DML_IFSTACK_ENTRY)pCosaContext->hContext;

        if ( pInterfaceStack )
        {
            AnscSListPopEntryByLink(pIFStackHead, &pCosaContext->Linkage);

            AnscFreeMemory(pInterfaceStack);
            AnscFreeMemory(pCosaContext);
        }
    }

    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaIFStackAddEntry
            (
            
                ANSC_HANDLE                 hThisObject,
                PCOSA_DML_IFSTACK_ENTRY     pEntry
            );


    description:

        This function add  cosa interfaceStack object and return status.

    argument:   ANSC_HANDLE                 hThisObject
                    This handle is actually the pointer of this object itself.

                    PCOSA_DML_IFSTACK_ENTRY     pEntry

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaIFStackAddEntry
    (
    
        ANSC_HANDLE                 hThisObject,
        PCOSA_DML_IFSTACK_ENTRY     pEntry
    )
{
    PCOSA_DATAMODEL_IFSTACK         pMyObject       = (PCOSA_DATAMODEL_IFSTACK)hThisObject;
    PCOSA_DML_IFSTACK_ENTRY         pInterfaceStack = (PCOSA_DML_IFSTACK_ENTRY)NULL;
    PSLIST_HEADER                   pIFStackHead    = (PSLIST_HEADER)&pMyObject->InterfaceStackList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pInterfaceStack = (PCOSA_DML_IFSTACK_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_IFSTACK_ENTRY));

    if ( !pInterfaceStack )
    {
        return ANSC_STATUS_RESOURCES;
    }
                
    pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    
    if ( !pCosaContext )
    {
        AnscFreeMemory(pInterfaceStack);
        
        return ANSC_STATUS_RESOURCES;
    }

    AnscCopyMemory(pInterfaceStack, pEntry, sizeof(COSA_DML_IFSTACK_ENTRY));

    pMyObject->ulEntryCount++;
    pCosaContext->InstanceNumber = pMyObject->ulEntryCount;
   
    if ( 0 == pMyObject->ulEntryCount )
    {
        pMyObject->ulEntryCount = 1;
    }

    pCosaContext->hContext      = (ANSC_HANDLE)pInterfaceStack;
    pCosaContext->hParentTable  = NULL;

    CosaSListPushEntryByInsNum(pIFStackHead, pCosaContext);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaIFStackEmptyTable
            (
            
                ANSC_HANDLE                 hThisObject
            );


    description:

        This function delete all cosa interfaceStack entry and return status.

    argument:   ANSC_HANDLE                 hThisObject
                    This handle is actually the pointer of this object itself.

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaIFStackEmptyTable
    (
    
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_IFSTACK         pMyObject    = (PCOSA_DATAMODEL_IFSTACK)hThisObject;
    PSLIST_HEADER                   pIFStackHead = (PSLIST_HEADER)&pMyObject->InterfaceStackList;
    PCOSA_DML_IFSTACK_ENTRY         pInterfaceStack = (PCOSA_DML_IFSTACK_ENTRY)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    
    /* InterfaceStack List */
    pLink = AnscSListGetFirstEntry(pIFStackHead);
    
    while ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink        = AnscSListGetNextEntry(pLink);
    
        pInterfaceStack = (PCOSA_DML_IFSTACK_ENTRY)pCosaContext->hContext;
    
        if ( pInterfaceStack )
        {
            AnscSListPopEntryByLink(pIFStackHead, &pCosaContext->Linkage);
    
            AnscFreeMemory(pInterfaceStack);
            AnscFreeMemory(pCosaContext);
        }
    }

    /*  Initiation again */
    pMyObject->ulEntryCount = 0;
    
    AnscSListInitializeHeader(&pMyObject->InterfaceStackList)

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

      caller:     self

      prototype:

        int
        CosaIFStackGetParamValueString
            (
                char *pParamName
                char *pParamValue
                ULONG *pValueSize
            );


    description:

        This function retrieves the requested data model parameter
        with type string from internal database or via dbus for
        external parameters.

    argument:   char *pParamName    DataModel Parameter Name
                char *pParamValue   Output Value
                ULONG *pValueSize   Size of pParamValue buffer

    return:     operation status, 0 for success

**********************************************************************/

static int CosaIFStackGetParamValueString(char *pParamName, char *pParamValue, ULONG *pValueSize)
{
    int                   returnValue = 0;
    parameterValStruct_t  varStruct;

    /* Check if object is managed by an external component... if so we must use
       COSAGetParamValueByPathName API to read its values */
    if ( _ansc_strstr(pParamName, ".WiFi.") != NULL )
    {
        varStruct.parameterName = pParamName;
        varStruct.parameterValue = pParamValue;
        returnValue = COSAGetParamValueByPathName(g_MessageBusHandle, &varStruct, pValueSize);
    }
    else
    {
        returnValue = CosaGetParamValueString(pParamName, pParamValue, pValueSize);
    }
    return returnValue;
}

/**********************************************************************

    caller:     self

    prototype:

        int
        CosaIFStackGetParamValueULong
            (
                char *pParamName
            );


    description:

        This function retrieves the requested data model parameter
        with type ULONG/int from internal database or via dbus for
        external parameters.

    argument:   char *pParamName    DataModel Parameter Name

    return:     Parameter value (ULONG)

**********************************************************************/

static ULONG CosaIFStackGetParamValueULong(char *pParamName)
{
    int                   returnValue = 0;
    char                  paramValue[32];
    ULONG                 paramSize;
    ULONG                 paramValueULong;
    parameterValStruct_t  varStruct;

    /* Check if object is managed by an external component... if so we must use
       COSAGetParamValueByPathName API to read its values */
    if ( _ansc_strstr(pParamName, ".WiFi.") != NULL )
    {
        varStruct.parameterName = pParamName;
        varStruct.parameterValue = paramValue;
        paramSize = sizeof(paramValue);
        returnValue = COSAGetParamValueByPathName(g_MessageBusHandle, &varStruct, &paramSize);
        paramValueULong = returnValue == 0 ? atoi(paramValue) : 0;
    }
    else
    {
        paramValueULong =  CosaGetParamValueUlong(pParamName);
    }
    return paramValueULong;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaIFStackCreateAll
            (
                ANSC_HANDLE                 hThisObject
            );


    description:

        This function generate all cosa interfaceStack entry and return status.

    argument:   ANSC_HANDLE                 hThisObject
                    This handle is actually the pointer of this object itself.

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaIFStackCreateAll
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IFSTACK         pMyObject       = (PCOSA_DATAMODEL_IFSTACK)hThisObject;
    PANSC_TOKEN_CHAIN               pListTokenChain = (PANSC_TOKEN_CHAIN)NULL;
    PANSC_STRING_TOKEN              pStringToken    = (PANSC_STRING_TOKEN)NULL;
    COSA_DML_IFSTACK_ENTRY          sInterfaceStack;
    PCHAR                           pEnd                  = NULL;    
    BOOL                            bMultiLayer           = FALSE;
    ULONG                           i                     = 0;
    ULONG                           j                     = 0;
    ULONG                           k                     = 0;
    int                             returnValue           = 0;
    ULONG                           ulLength              = 0;
    ULONG                           ulEntryInstanceNum    = 0;
    ULONG                           ulSubEntryInsNum      = 0;
    ULONG                           ulNumOfTable          = 0;
    ULONG                           ulNumOfParent         = 0;
    ULONG                           ulNumOfEntry          = 0;
    ULONG                           ulEntryNameLen        = 256;
    CHAR                            ucEntryParamName[256] = {0};
    CHAR                            ucEntryNameValue[1024] = {0};
    UCHAR                           ucEntryFullPath[256]  = {0};
    CHAR                            ucTableParam[256]     = {0};
    CHAR                            ucTable1[]            = "Device.Ethernet.Link.";
    CHAR                            ucTable2[]            = "Device.WiFi.SSID.";
    CHAR                            ucTable3[]            = "Device.IP.Interface.";
    CHAR                            ucTable4[]            = "Device.Ethernet.VLANTermination.";
    CHAR                            ucTable5[]            = "Device.PPP.Interface.";
    CHAR                            ucTable6[]            = "Device.DSL.Channel.";
    CHAR                            ucTable7[]            = "Device.DSL.BondingGroup.";
    CHAR                            ucTable8[]            = "Device.ATM.Link.";
    CHAR                            ucTable9[]            = "Device.PTM.Link.";
    CHAR                            ucTable10[256]        = "Device.Bridging.Bridge.{i}.Port.";  //Fix the ABW error in the code below
    PCHAR                           pAllTable[]           = {ucTable1, ucTable2, 
                                                             ucTable3, ucTable4,
                                                             ucTable5, ucTable6,
                                                             ucTable7, ucTable8,
                                                             ucTable9, ucTable10};
    errno_t                         rc                    = -1;
    
    /* Empty interfaceStack table first */
    CosaIFStackEmptyTable(pMyObject);

    /* Begin to generate table by accessing all related objects with LowerLayers */
    ulNumOfTable = sizeof(pAllTable)/sizeof(PUCHAR);

    for ( i = 0 ; i < ulNumOfTable; i++ )
    {
        if (( pEnd  = _ansc_strstr((const char*)pAllTable[i], ".{i}.") ))
        {
            /* This is a multi-layer object like Device.Bridging.Bridge.{i}.Port.{i}. */
            bMultiLayer = TRUE;
        }

        AnscZeroMemory(ucTableParam, 256);  

        if ( bMultiLayer )
        {
            /* To get Device.Bridging.Bridge */
            ulLength = (ULONG)(pEnd - pAllTable[i]);

            AnscCopyMemory(ucTableParam, pAllTable[i], ulLength);

            /* To get like .Port */
            pEnd = pEnd + 4;

            AnscZeroMemory(ucEntryFullPath, 256); 
            /*CID: 71557 Dereference after null check*/
            if(pEnd) 
               AnscCopyMemory(ucEntryFullPath, pEnd,  AnscSizeOfString((const char*)pEnd)-1);

            /* To get like Device.Bridging.BridgeNumberOfEntries */
            strncat(ucTableParam, "NumberOfEntries",sizeof(ucTableParam)-strlen(ucTableParam)-1);  
 
            ulNumOfParent =  CosaIFStackGetParamValueULong(ucTableParam);  

            /* To get like Device.Bridging.Bridge. */
            rc = strcpy_s(ucTableParam + ulLength,sizeof(ucTableParam)-ulLength,".");
            if(rc != EOK)
            {
              ERR_CHK(rc);
              continue;
            }
        }
        else
        {
            /* To get like  Device.Ethernet.Link. */
            AnscCopyMemory(ucTableParam, pAllTable[i], AnscSizeOfString((const char*)pAllTable[i])-1);

            /* To go following cycle for once */
            ulNumOfParent = 1;
        }

        for ( j = 0 ; j < ulNumOfParent; j++ )
        {   
            if ( bMultiLayer )
            {
                ulEntryInstanceNum = CosaGetInstanceNumberByIndex(ucTableParam, j);
 
                if ( 0 == ulEntryInstanceNum )
                {
                    CcspTraceWarning(("\n Invalid instancenumber for '%s'\n", ucTableParam)); 
                    continue;
                } 
                
                /* To get like Device.Bridging.Bridge.1.Port */
                rc = sprintf_s(pAllTable[i],
                              TABLE_SIZE,
                              "%s%lu%s",
                              ucTableParam,
                              ulEntryInstanceNum,
                              ucEntryFullPath);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  continue;
                }

                ulLength = AnscSizeOfString((const char*)pAllTable[i]);

                /* To get like Device.Bridging.Bridge.1.PortNumberOfEntries */
                _ansc_strcat(pAllTable[i], "NumberOfEntries");  
                
                ulNumOfEntry = CosaIFStackGetParamValueULong(pAllTable[i]);  

                /* To get like Device.Bridging.Bridge.1.Port. */
                rc = strcpy_s(pAllTable[i] + ulLength, TABLE_SIZE - ulLength,".");
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  continue;
                }
            }
            else
            {
                 /* To get like Device.Ethernet.LinkNumberOfEntries */
                strncat(ucTableParam, "NumberOfEntries", sizeof(ucTableParam)-strlen(ucTableParam)-1);        
                 
                ulNumOfEntry =  CosaIFStackGetParamValueULong(ucTableParam);  
            }
                       
            for ( k = 0 ; k < ulNumOfEntry; k++ )
            {
                if ( _ansc_strstr((const char*)pAllTable[i], ".WiFi.") != NULL )
                {
                    /* We can't get WiFi index-to-instance since it's managed by an
                       external component/process, so we derive it manually. */
                    ulSubEntryInsNum = k + 1;
                }
                else
                {
                    ulSubEntryInsNum = CosaGetInstanceNumberByIndex(pAllTable[i], k);
                }

                if ( 0 == ulSubEntryInsNum )
                {
                    CcspTraceWarning(("\n Invalid instancenumber for '%s'\n", pAllTable[i])); 
                    continue;
                }

                /* 1. To get value of table like Device.Ethernet.Link.{i}  */
                AnscZeroMemory(sInterfaceStack.HigherLayer, COSA_IFSTACK_NAME_LENGTH);
                AnscZeroMemory(sInterfaceStack.HigherAlias, COSA_IFSTACK_ALIAS_LENGTH);
                
                rc = sprintf_s(sInterfaceStack.HigherLayer,
                              sizeof(sInterfaceStack.HigherLayer),
                              "%s%lu",
                              pAllTable[i],
                              ulSubEntryInsNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  continue;
                }

                /* 2. To get value of table like Device.Ethernet.Link.{i}.Alias */
                ulEntryNameLen = 64;            
                    
                rc = sprintf_s(ucEntryParamName,
                              sizeof(ucEntryParamName),
                              "%s.Alias",
                              sInterfaceStack.HigherLayer);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  continue;
                }

                ulEntryNameLen = sizeof(ucEntryNameValue);
                returnValue = CosaIFStackGetParamValueString(ucEntryParamName,
                                                    ucEntryNameValue,
                                                    &ulEntryNameLen);
                
                if ( ( 0 == returnValue) && 
                     ( 0 != AnscSizeOfString((const char*)ucEntryNameValue)) )
                {
                    rc = strcpy_s(sInterfaceStack.HigherAlias,sizeof(sInterfaceStack.HigherAlias), ucEntryNameValue);
                    ERR_CHK(rc);
                } 
                else
                {
                    CcspTraceWarning(("\n No Alias for '%s'\n", sInterfaceStack.HigherLayer)); 
                }
                
                /* 3. To get value of table like Device.Ethernet.Link.{i}.LowerLayers */
                ulEntryNameLen = 1024;            
                    
                rc = sprintf_s(ucEntryParamName,
                              sizeof(ucEntryParamName),
                              "%s.LowerLayers",
                              sInterfaceStack.HigherLayer);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  continue;
                }

                ulEntryNameLen = sizeof(ucEntryNameValue);
                returnValue = CosaIFStackGetParamValueString(ucEntryParamName,
                                                    ucEntryNameValue,
                                                    &ulEntryNameLen);
                
                if ( ( 0 != returnValue) || 
                     ( 0 == AnscSizeOfString((const char*)ucEntryNameValue)) )
                {
                    CcspTraceWarning(("\n No LowerLayers for '%s'\n", sInterfaceStack.HigherLayer)); 
                    continue;
                }

                /* 4. To get value of single LowerLayer and cooresponding Alias */
                if ( TRUE )
                {
                    pListTokenChain = (PANSC_TOKEN_CHAIN)NULL;
                    pStringToken    = (PANSC_STRING_TOKEN)NULL;

                    pListTokenChain = AnscTcAllocate(ucEntryNameValue, ",");
                    
                    if ( !pListTokenChain )
                    {
                        CcspTraceWarning(("\n Failed to get single from comma-separated lowerlayers '%s'\n",
                                          ucEntryNameValue)); 
                        continue;
                    }  
                        
                    while (( pStringToken = AnscTcUnlinkToken(pListTokenChain) ))
                    {
                        /*CID: 67432 - Array Compared against null - fixed*/
                        if ( strlen(pStringToken->Name) != 0 )
                        {
                            /* 4.1 To get one LowerLayer */
                            AnscZeroMemory(sInterfaceStack.LowerLayer, COSA_IFSTACK_NAME_LENGTH);
                            AnscZeroMemory(sInterfaceStack.LowerAlias, COSA_IFSTACK_ALIAS_LENGTH);

                            /* Special handle for here exists the dot "." at the end of lowerlayer.
                                                  Actually lowerlayer should not like this. Just waiting for all related
                                                  objects finish the implementation of lowerlayers, removed this handle 
                                             */
                            ulLength = AnscSizeOfString(pStringToken->Name);
                            ulLength--;
                            
                            if ( _ansc_strcmp(pStringToken->Name + ulLength, ".") == 0 )
                            {
                                // CID 277494 : Destination buffer too small
                                if(ulLength < sizeof(sInterfaceStack.LowerLayer))
                                    AnscCopyMemory(sInterfaceStack.LowerLayer, pStringToken->Name, ulLength);
                            } 
                            else
                            {
                                rc = strcpy_s(sInterfaceStack.LowerLayer,sizeof(sInterfaceStack.LowerLayer), pStringToken->Name);
                                ERR_CHK(rc);
                            }
                            /* 4.2 To get one LowerLayer.Alias */
                            ulEntryNameLen = 64;
                            
                            rc = sprintf_s(ucEntryParamName,
                                          sizeof(ucEntryParamName),
                                          "%s.Alias",
                                          sInterfaceStack.LowerLayer);
                            if(rc < EOK)
                            {
                               ERR_CHK(rc);
                               continue;
                            }               
                            ulEntryNameLen = sizeof(ucEntryNameValue);
                            returnValue = CosaIFStackGetParamValueString(ucEntryParamName,
                                                                ucEntryNameValue,
                                                                &ulEntryNameLen);
                            
                            if ( ( 0 == returnValue) && 
                                 ( 0 != AnscSizeOfString((const char*)ucEntryNameValue)) )
                            {
                               rc = strcpy_s(sInterfaceStack.LowerAlias,sizeof(sInterfaceStack.LowerAlias), ucEntryNameValue);
                               ERR_CHK(rc);
                            } 
                            else
                            {
                                CcspTraceWarning(("\n No Alias for '%s'\n", sInterfaceStack.LowerLayer)); 
                            }
                            
                            /* 5. Add this entry into interfacestack table */
                            CosaIFStackAddEntry(pMyObject, &sInterfaceStack);
                        }
                        
                         AnscFreeMemory(pStringToken);
                    }

                    if ( pListTokenChain )
                    {
                        AnscTcFree((ANSC_HANDLE)pListTokenChain);
                    }
                }
            }
        }
    }

    return returnStatus;
}

