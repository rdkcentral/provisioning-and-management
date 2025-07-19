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

    File: cosa_ethernet_apis_multilan.c

    For Data Model SBAPI Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        The APIs servicing TR-181 data model Ethernet Interface and
        Link SBAPI integration are implemented in this file.

    ---------------------------------------------------------------

    environment:

        Platform Independent, but with multi-LAN support

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/28/2013  initial revision.

**********************************************************************/


#include "cosa_apis.h"
#include "cosa_ethernet_apis_multilan.h"
#include "ccsp_psm_helper.h"
#include "dmsb_tr181_psm_definitions.h"

#include "linux/if.h"
#include "linux/sockios.h"
#include <sys/ioctl.h>
#include <linux/errno.h>
#include "safec_lib_common.h"

extern int errno;
/**********************************************************************
                    HELPER FUNCTION PROTOTYPES
**********************************************************************/

int                 _getMac(char* ifName, char* mac);
COSA_DML_IF_STATUS  getIfStatus(const PUCHAR name, struct ifreq *pIfr);
//unused function
#if 0
static int          setIfStatus(struct ifreq *pIfr);
#endif
BOOLEAN 			getIfAvailability( const PUCHAR name );


/**********************************************************************
                            MAIN ROUTINES
**********************************************************************/
static bool isValid(char *str)
{
    int i;
    int len = strlen(str);
    for (i = 0; i <= len; i++) {
        if (!isspace(str[i])) {
            break;
        }
    }
    if (str[i] == '\0') {
        return false;
    }
    return true;
}

ANSC_STATUS
CosaDmlEthMlanInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pEthContext = (PDMSB_TR181_ETH_CONTEXT)AnscAllocateMemory(sizeof(DMSB_TR181_ETH_CONTEXT));

    if ( pEthContext )
    {
        DMSB_TR181_ETH_CONTEXT_Init(pEthContext);

        *phContext = (ANSC_HANDLE)pEthContext;
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        return  ANSC_STATUS_RESOURCES;
    }

    /* Need the Unload function to free allocated context */
}

/**********************************************************************
                    Regular Ethernet Link Routines 
**********************************************************************/

PDMSB_TR181_ETH_LINK
CosaDmlEthLinkFindByInstNum
    (
        PDMSB_TR181_ETH_CONTEXT     pEthContext,
        ULONG                       ulInstNum
    )
{
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    PDMSB_TR181_ETH_LINK            pEthLink;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pSLinkEntry = AnscSListGetFirstEntry(&pEthContext->EthLinkList);

    while ( pSLinkEntry )
    {
        pEthLink    = ACCESS_DMSB_TR181_ETH_LINK(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pEthLink->Cfg.InstanceNumber == ulInstNum )
        {
            return  pEthLink;
        }
    }

    AnscTraceWarning(("%s -- failed to find %lu!\n", __FUNCTION__, ulInstNum));

    return  NULL;
}


ANSC_STATUS
CosaDmlEthLinkLoadPsm
    (
        PDMSB_TR181_ETH_CONTEXT     pEthContext
    )
{
    int                             iReturnValue    = CCSP_SUCCESS;
    int                             iNumInst        = 0;
    int*                            pInstArray      = NULL;
    ULONG                           ulIndex;
    PDMSB_TR181_ETH_LINK            pEthLink        = NULL;
    char                            pParamPath[64]  = {0};
    unsigned int                    RecordType      = 0;
    SLAP_VARIABLE                   SlapValue       = {0};
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    iReturnValue = 
        PsmGetNextLevelInstances
            (
                g_MessageBusHandle,
                g_SubsystemPrefix,
                DMSB_TR181_PSM_EthLink_Root,
                (unsigned int *)&iNumInst,
                (unsigned int **)&pInstArray
            );

    if ( iReturnValue != CCSP_SUCCESS )
    {
        AnscTraceWarning(("%s -- PsmGetNextLevelInstances failed, error code = %d!\n", __FUNCTION__, iReturnValue));
        goto  EXIT;
    }

    for ( ulIndex = 0; (int)ulIndex < iNumInst; ulIndex++ )
    {
        pEthLink = (PDMSB_TR181_ETH_LINK)AnscAllocateMemory(sizeof(DMSB_TR181_ETH_LINK));

        if ( !pEthLink )
        {
            AnscTraceWarning(("%s -- insufficient resources, EthLink instance %lu\n", __FUNCTION__, ulIndex));
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pEthLink->Cfg.InstanceNumber = pInstArray[ulIndex];
        }

        /* Fetch Cfg */

        if ( TRUE )     /* Enable */
        {
            SlapInitVariable(&SlapValue);

            rc = sprintf_s
                (
                    pParamPath,
                    sizeof(pParamPath),
                    DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_Enable,
                    pInstArray[ulIndex]
                );
            if(rc < EOK)
            {
               ERR_CHK(rc);
            }

            iReturnValue =
                PSM_Get_Record_Value
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        &RecordType,
                        &SlapValue
                    );

            if ( (iReturnValue != CCSP_SUCCESS) )
            {
                AnscTraceWarning(("%s -- failed to retrieve 'Enable' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
            }
            else if (RecordType == ccsp_boolean)
            {
                pEthLink->Cfg.bEnabled = SlapValue.Variant.varBool;
            }
            else if (RecordType == ccsp_string)
            {
                pEthLink->Cfg.bEnabled = (strcmp(SlapValue.Variant.varString, "true") == 0);
            }

            SlapCleanVariable(&SlapValue);
        }

        if ( TRUE )     /* Alias */
        {
            SlapInitVariable(&SlapValue);

            rc = sprintf_s
                (
                    pParamPath,
                    sizeof(pParamPath),
                    DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_Alias,
                    pInstArray[ulIndex]
                );
            if(rc < EOK)
            {
               ERR_CHK(rc);
            }

            iReturnValue =
                PSM_Get_Record_Value
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        &RecordType,
                        &SlapValue
                    );

            if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
            {
                AnscTraceWarning(("%s -- failed to retrieve 'Alias' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
            }
            else
            {
                rc = strcpy_s(pEthLink->Cfg.Alias, sizeof(pEthLink->Cfg.Alias), SlapValue.Variant.varString);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                }
            }

            SlapCleanVariable(&SlapValue);
        }

        if ( TRUE )     /* PriorityTagging */
        {
            SlapInitVariable(&SlapValue);

            rc = sprintf_s
                (
                    pParamPath,
                    sizeof(pParamPath),
                    DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_PriorityTagging,
                    pInstArray[ulIndex]
                );
            if(rc < EOK)
            {
               ERR_CHK(rc);
            }

            iReturnValue =
                PSM_Get_Record_Value
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        &RecordType,
                        &SlapValue
                    );

            if ( (iReturnValue != CCSP_SUCCESS) )
            {
                AnscTraceWarning(("%s -- failed to retrieve 'PriorityTagging' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
            }
            else if (RecordType == ccsp_boolean)
            {
                pEthLink->Cfg.bPriorityTagging = SlapValue.Variant.varBool;
            }

            SlapCleanVariable(&SlapValue);
        }

        if ( TRUE )     /* LowerLayerType */
        {
            SlapInitVariable(&SlapValue);

            rc = sprintf_s
                (
                    pParamPath,
                    sizeof(pParamPath),
                    DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_LowerLayerType,
                    pInstArray[ulIndex]
                );
            if(rc < EOK)
            {
              ERR_CHK(rc);
            }


            iReturnValue =
                PSM_Get_Record_Value
                    (
                        g_MessageBusHandle,
                        g_SubsystemPrefix,
                        pParamPath,
                        &RecordType,
                        &SlapValue
                    );

            if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
            {
                AnscTraceWarning(("%s -- failed to retrieve 'LowerLayerType' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
                pEthLink->Cfg.LinkType = COSA_DML_LINK_TYPE_LAST;
            }
            else
            {
                pEthLink->Cfg.LinkType = CosaUtilGetLinkTypeFromStr(SlapValue.Variant.varString);
            }

            SlapCleanVariable(&SlapValue);
        }

        if ( !pEthLink->Cfg.LinkType || (pEthLink->Cfg.LinkType != COSA_DML_LINK_TYPE_LAST) )
        {
            if ( TRUE )     /* LowerLayerInstNum */
            {
                SlapInitVariable(&SlapValue);

                rc = sprintf_s
                    (
                        pParamPath,
                        sizeof(pParamPath),
                        DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_LowerLayerInstNum,
                        pInstArray[ulIndex]
                    );
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }

                iReturnValue =
                    PSM_Get_Record_Value
                        (
                            g_MessageBusHandle,
                            g_SubsystemPrefix,
                            pParamPath,
                            &RecordType,
                            &SlapValue
                        );

                if ( iReturnValue != CCSP_SUCCESS )
                {
                    AnscTraceWarning(("%s -- failed to retrieve 'LowerLayerInstanceNumber' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
                }
                else if (RecordType == ccsp_unsignedInt)
                {
                    pEthLink->Cfg.LinkInstNum = SlapValue.Variant.varUint32;
                }
                else if (RecordType == ccsp_string)
                {
                    pEthLink->Cfg.LinkInstNum = _ansc_atoi(SlapValue.Variant.varString);
                }

                SlapCleanVariable(&SlapValue);
            }

            if ( TRUE )     /* Fetch StaticInfo */
            {
                ANSC_STATUS         returnStatus;
                ULONG               ulNameBufSize;

                ulNameBufSize = sizeof(pEthLink->StaticInfo.Name);
                returnStatus =
                    CosaUtilGetLowerLayerName
                        (
                            pEthLink->Cfg.LinkType,
                            pEthLink->Cfg.LinkInstNum,
                            pEthLink->StaticInfo.Name,
                            &ulNameBufSize
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    AnscTraceWarning(("%s -- failed to retrieve LowerLayer name parameter, error code %lu\n", __FUNCTION__, returnStatus));
                }
            }

            if ( TRUE )     /* Name */
            {
                SlapInitVariable(&SlapValue);

                rc = sprintf_s
                    (
                        pParamPath,
                        sizeof(pParamPath),
                        DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_Name,
                        pInstArray[ulIndex]
                    );
                if(rc < EOK)
                {
                   ERR_CHK(rc);
                }

                iReturnValue =
                    PSM_Get_Record_Value
                        (
                            g_MessageBusHandle,
                            g_SubsystemPrefix,
                            pParamPath,
                            &RecordType,
                            &SlapValue
                        );

                if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
                {
                    AnscTraceWarning(("%s -- failed to retrieve 'Name' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
                }
                else
                {
                    rc = strcpy_s(pEthLink->StaticInfo.Name, sizeof(pEthLink->StaticInfo.Name), SlapValue.Variant.varString);
                    if(rc != EOK)
                    {
                       ERR_CHK(rc);
                    }
                }

                SlapCleanVariable(&SlapValue);
            }
        }
        else /* LowerLayerType not configured, go with l2net/l2netType */
        {
            if ( TRUE )     /* l2net */
            {
                SlapInitVariable(&SlapValue);

                rc = sprintf_s
                    (
                        pParamPath,
                        sizeof(pParamPath),
                        DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_l2net,
                        pInstArray[ulIndex]
                    );
                if(rc < EOK)
                {
                   ERR_CHK(rc);
                }

                iReturnValue =
                    PSM_Get_Record_Value
                        (
                            g_MessageBusHandle,
                            g_SubsystemPrefix,
                            pParamPath,
                            &RecordType,
                            &SlapValue
                        );

                if ( (iReturnValue != CCSP_SUCCESS) )
                {
                    AnscTraceWarning(("%s -- failed to retrieve 'l2net' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
                }
                else if (RecordType == ccsp_unsignedInt)
                {
                    pEthLink->Cfg.LinkInstNum = SlapValue.Variant.varUint32;
                }
                else if (RecordType == ccsp_string)
                {
                    pEthLink->Cfg.LinkInstNum = _ansc_atoi(SlapValue.Variant.varString);
                }

                SlapCleanVariable(&SlapValue);
            }

            if ( TRUE )     /* l2netType */
            {
                SlapInitVariable(&SlapValue);

                rc = sprintf_s
                    (
                        pParamPath,
                        sizeof(pParamPath),
                        DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_l2netType,
                        pInstArray[ulIndex]
                    );
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }

                iReturnValue =
                    PSM_Get_Record_Value
                        (
                            g_MessageBusHandle,
                            g_SubsystemPrefix,
                            pParamPath,
                            &RecordType,
                            &SlapValue
                        );

                if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
                {
                    AnscTraceWarning(("%s -- failed to retrieve 'l2netType' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
                }
                else
                {
                    pEthLink->Cfg.LinkType = CosaUtilGetLinkTypeFromStr(SlapValue.Variant.varString);
                }

                SlapCleanVariable(&SlapValue);
            }

            /* Fetch StaticInfo */

            if ( pEthLink->Cfg.LinkInstNum )
            {
                /*
                 *  Fetch the "name" parameter from l2net PSM object
                 */
                SlapInitVariable(&SlapValue);

                rc = sprintf_s
                    (
                        pParamPath,
                        sizeof(pParamPath),
                        DMSB_TR181_PSM_l2net_Root DMSB_TR181_PSM_l2net_i DMSB_TR181_PSM_l2net_name,
                        (int)pEthLink->Cfg.LinkInstNum
                    );
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }

                iReturnValue =
                    PSM_Get_Record_Value
                        (
                            g_MessageBusHandle,
                            g_SubsystemPrefix,
                            pParamPath,
                            &RecordType,
                            &SlapValue
                        );

                if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
                {
                    AnscTraceWarning(("%s -- failed to retrieve 'l2net.name' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
                }
                else
                {
                    rc = strcpy_s(pEthLink->StaticInfo.Name, sizeof(pEthLink->StaticInfo.Name), SlapValue.Variant.varString);
                    if(rc != EOK)
                    {
                       ERR_CHK(rc);
                    }
                }

                SlapCleanVariable(&SlapValue);
            }

            if ( TRUE )     /* Name */
            {
                SlapInitVariable(&SlapValue);

                rc = sprintf_s
                    (
                        pParamPath,
                        sizeof(pParamPath),
                        DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_Name,
                        pInstArray[ulIndex]
                    );
                if(rc < EOK)
                {
                   ERR_CHK(rc);
                }

                iReturnValue =
                    PSM_Get_Record_Value
                        (
                            g_MessageBusHandle,
                            g_SubsystemPrefix,
                            pParamPath,
                            &RecordType,
                            &SlapValue
                        );

                if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
                {
                    AnscTraceWarning(("%s -- failed to retrieve 'Name' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
                }
                else
                {
                    rc = strcpy_s(pEthLink->StaticInfo.Name, sizeof(pEthLink->StaticInfo.Name), SlapValue.Variant.varString);
                    if(rc != EOK)
                    {
                       ERR_CHK(rc);
                    }
                }

                SlapCleanVariable(&SlapValue);
            }
        }

        if ( _ansc_strlen(pEthLink->StaticInfo.Name) > 0 )
        {
            _getMac(pEthLink->StaticInfo.Name,(char *) pEthLink->StaticInfo.MacAddress);
        }

        /* Fetch DynamicInfo */
        pEthLink->DynamicInfo.LastChange = AnscGetTickInSeconds();

        AnscSListPushEntryAtBack(&pEthContext->EthLinkList, &pEthLink->Linkage);
    }

EXIT:
    if ( pInstArray )
    {
        AnscFreeMemory(pInstArray);
        pInstArray = NULL;
    }

    return  iReturnValue;
}


ANSC_STATUS
CosaDmlEthLinkSavePsm
    (
        PDMSB_TR181_ETH_CONTEXT     pEthContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg
    )
{
    UNREFERENCED_PARAMETER(pEthContext);
    int                             iReturnValue    = CCSP_SUCCESS;
    char                            pParamPath[64]  = {0};
    unsigned int                    RecordType      = ccsp_string;
    char                            RecordValue[64] = {0};
    errno_t                         rc        = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( TRUE )     /* Enable */
    {
        rc = sprintf_s
            (
                pParamPath,
                sizeof(pParamPath),
                DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_Enable,
                (int)pCfg->InstanceNumber
            );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }

        RecordType = ccsp_boolean;
        rc = strcpy_s(RecordValue, sizeof(RecordValue), (pCfg->bEnabled ? PSM_TRUE : PSM_FALSE));
        if(rc != EOK)
        {
          ERR_CHK(rc);
        }

        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- failed to set 'Enable' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
        }
    }

    if ( TRUE )     /* Alias */
    {
        rc = sprintf_s
            (
                pParamPath,
                sizeof(pParamPath),
                DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_Alias,
                (int)pCfg->InstanceNumber
            );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }

        RecordType = ccsp_string;
        rc = strcpy_s(RecordValue, sizeof(RecordValue), pCfg->Alias);
        if(rc != EOK)
        {
          ERR_CHK(rc);
        }

        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- failed to set 'Alias' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
        }
    }

    if ( TRUE )     /* PriorityTagging */
    {
        rc = sprintf_s
            (
                pParamPath,
                sizeof(pParamPath),
                DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_PriorityTagging,
                (int)pCfg->InstanceNumber
            );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }

        RecordType = ccsp_boolean;
        rc = strcpy_s(RecordValue, sizeof(RecordValue), (pCfg->bPriorityTagging ? PSM_TRUE : PSM_FALSE));
        if(rc != EOK)
        {
          ERR_CHK(rc);
        }
        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- failed to set 'PriorityTagging' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
        }
    }

    if ( TRUE )     /* l2net */
    {
        rc = sprintf_s
            (
                pParamPath,
                sizeof(pParamPath),
                DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_l2net,
                (int)pCfg->InstanceNumber
            );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }

        RecordType = ccsp_unsignedInt;
        rc = sprintf_s(RecordValue,sizeof(RecordValue), "%lu", pCfg->LinkInstNum);
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }

        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- failed to set 'l2net' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
        }
    }

    if ( TRUE )     /* l2netType */
    {
        rc = sprintf_s
            (
                pParamPath,
                sizeof(pParamPath),
                DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i DMSB_TR181_PSM_EthLink_l2netType,
                (int)pCfg->InstanceNumber
            );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }

        RecordType = ccsp_string;
        rc = strcpy_s(RecordValue, sizeof(RecordValue), CosaUtilGetLinkTypeStr(pCfg->LinkType));
        if(rc != EOK)
        {
          ERR_CHK(rc);
        }
        iReturnValue =
            PSM_Set_Record_Value2
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath,
                    RecordType,
                    RecordValue
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- failed to set 'l2netType' parameter, error code %d, type %d\n", __FUNCTION__, iReturnValue, RecordType));
        }
    }

    return  CCSP_SUCCESS;
}


ANSC_STATUS
CosaDmlEthLinkDelPsm
    (
        PDMSB_TR181_ETH_CONTEXT     pEthContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg
    )
{
    UNREFERENCED_PARAMETER(pEthContext);
    int                             iReturnValue    = CCSP_SUCCESS;
    char                            pParamPath[64]  = {0};
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( TRUE )     /* Enable */
    {
        rc = sprintf_s
            (
                pParamPath,
                sizeof(pParamPath),
                DMSB_TR181_PSM_EthLink_Root DMSB_TR181_PSM_EthLink_i,
                (int)pCfg->InstanceNumber
            );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }

        iReturnValue =
            PSM_Del_Record
                (
                    g_MessageBusHandle,
                    g_SubsystemPrefix,
                    pParamPath
                );

        if ( iReturnValue != CCSP_SUCCESS )
        {
            AnscTraceWarning(("%s -- failed to delete PSM records, error code %d\n", __FUNCTION__, iReturnValue));
        }
    }

    /* Reset dmsb.l3net.{i}.EthLink */

    return  CCSP_SUCCESS;
}


ULONG
CosaDmlEthLinkGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pEthContext->EthLinkInit )
    {
        CosaDmlEthLinkLoadPsm(pEthContext);
        pEthContext->EthLinkInit = TRUE;
    }

    return  AnscSListQueryDepth(&pEthContext->EthLinkList);
}

ANSC_STATUS
CosaDmlEthLinkGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    )
{
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    PDMSB_TR181_ETH_LINK            pEthLink;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( ulIndex < AnscSListQueryDepth(&pEthContext->EthLinkList) )
    {
        pSLinkEntry = AnscSListGetEntryByIndex(&pEthContext->EthLinkList, ulIndex);

        if ( pSLinkEntry )
        {
            pEthLink = ACCESS_DMSB_TR181_ETH_LINK(pSLinkEntry);

            AnscCopyMemory(&pEntry->Cfg, &pEthLink->Cfg, sizeof(pEthLink->Cfg));
            AnscCopyMemory(&pEntry->StaticInfo, &pEthLink->StaticInfo, sizeof(pEthLink->StaticInfo));
            AnscCopyMemory(&pEntry->DynamicInfo, &pEthLink->StaticInfo, sizeof(pEthLink->DynamicInfo));

            pEntry->DynamicInfo.Status = getIfStatus((PUCHAR)pEthLink->StaticInfo.Name, NULL);

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            return  ANSC_STATUS_CANT_FIND;
        }
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
}

ANSC_STATUS
CosaDmlEthLinkSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    PDMSB_TR181_ETH_LINK            pEthLink;
    errno_t                         rc        =  -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pSLinkEntry = AnscSListGetEntryByIndex(&pEthContext->EthLinkList, ulIndex);

    if ( pSLinkEntry )
    {
        pEthLink = ACCESS_DMSB_TR181_ETH_LINK(pSLinkEntry);

        /* Instance number cannot be changed! */
        rc = strcpy_s(pEthLink->Cfg.Alias, sizeof(pEthLink->Cfg.Alias), pAlias);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return  ANSC_STATUS_FAILURE;
        }

        CosaDmlEthLinkSavePsm(pEthContext, &pEthLink->Cfg);
    }
    else
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    return  ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthLinkAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    )
{
    int                             iReturnValue    = CCSP_SUCCESS;
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PDMSB_TR181_ETH_LINK            pEthLink;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pEthLink = (PDMSB_TR181_ETH_LINK)AnscAllocateMemory(sizeof(DMSB_TR181_ETH_LINK));

        if ( !pEthLink )
        {
            AnscTraceWarning(("%s -- insufficient resources, EthLink instance %lu\n", __FUNCTION__, pEntry->Cfg.InstanceNumber));
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory(&pEthLink->Cfg,        &pEntry->Cfg,        sizeof(pEthLink->Cfg));
            AnscCopyMemory(&pEthLink->StaticInfo, &pEntry->StaticInfo, sizeof(pEthLink->StaticInfo));

            if ( pEthLink->Cfg.LinkInstNum )
            {
                char                            pParamPath[64]  = {0};
                unsigned int                    RecordType      = 0;
                SLAP_VARIABLE                   SlapValue       = {0};

                /*
                 *  Fetch the "name" parameter from l2net PSM object
                 */
                SlapInitVariable(&SlapValue);

                rc = sprintf_s
                    (
                        pParamPath,
                        sizeof(pParamPath),
                        DMSB_TR181_PSM_l2net_Root DMSB_TR181_PSM_l2net_i DMSB_TR181_PSM_l2net_name,
                        (int)pEthLink->Cfg.LinkInstNum
                    );
                if(rc < EOK)
                {
                   ERR_CHK(rc);
                }


                iReturnValue =
                    PSM_Get_Record_Value
                        (
                            g_MessageBusHandle,
                            g_SubsystemPrefix,
                            pParamPath,
                            &RecordType,
                            &SlapValue
                        );

                if ( (iReturnValue != CCSP_SUCCESS) || (RecordType != ccsp_string))
                {
                    AnscTraceWarning(("%s -- failed to retrieve 'l2net.name' parameter, error code %d, type %d", __FUNCTION__, iReturnValue, RecordType));
                }
                else
                {
                    rc = strcpy_s(pEthLink->StaticInfo.Name, sizeof(pEthLink->StaticInfo.Name), SlapValue.Variant.varString);
                    if(rc != EOK)
                    {
                       ERR_CHK(rc);
                    }
                }

                SlapCleanVariable(&SlapValue);
            }

            if ( _ansc_strlen(pEthLink->StaticInfo.Name) > 0 )
            {
                _getMac(pEthLink->StaticInfo.Name, (char *)pEthLink->StaticInfo.MacAddress);
            }

            pEntry->DynamicInfo.Status       = getIfStatus((PUCHAR)pEthLink->StaticInfo.Name, NULL);
            pEthLink->DynamicInfo.LastChange = AnscGetTickInSeconds();

            AnscSListPushEntryAtBack(&pEthContext->EthLinkList, &pEthLink->Linkage);

            CosaDmlEthLinkSavePsm(pEthContext, &pEthLink->Cfg);
        }
    }
    return  ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlEthLinkDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PDMSB_TR181_ETH_LINK            pEthLink;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pEthLink = CosaDmlEthLinkFindByInstNum(pEthContext, ulInstanceNumber);

    if ( !pEthLink )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        CosaDmlEthLinkDelPsm(pEthContext, &pEthLink->Cfg);

        AnscSListPopEntryByLink(&pEthContext->EthLinkList, &pEthLink->Linkage);
        AnscFreeMemory(pEthLink);

        return  ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlEthLinkSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg
    )
{
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PDMSB_TR181_ETH_LINK            pEthLink;
    errno_t                         rc              = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pEthLink = CosaDmlEthLinkFindByInstNum(pEthContext, pCfg->InstanceNumber);

    if ( !pEthLink )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        struct ifreq                ifr;
        BOOLEAN                     bChanged    = FALSE;

        if ( pEthLink->Cfg.LinkInstNum != pCfg->LinkInstNum )
        {
            bChanged = TRUE;
        }
        else if ( pEthLink->Cfg.bEnabled != pCfg->bEnabled )
        {
			COSA_DML_IF_STATUS enifStatus = COSA_DML_IF_STATUS_Unknown;
			BOOLEAN 		   bProceedFurther = TRUE;
			
			enifStatus = getIfStatus((PUCHAR)pEthLink->StaticInfo.Name, &ifr);
			
			if ( ( enifStatus == COSA_DML_IF_STATUS_Unknown ) ||
			     ( enifStatus == COSA_DML_IF_STATUS_NotPresent ) ||
			     ( enifStatus == COSA_DML_IF_STATUS_Error ) )
			{
				bProceedFurther = FALSE;
			}

            /* Check the actual if status to determine whether there is state change */
            if ( bProceedFurther )
            {
                if ( pCfg->bEnabled && !(pEthLink->Cfg.bEnabled) && !(ifr.ifr_flags & IFF_UP) )
                {
                    bChanged = TRUE;
                }
                else if ( !(pCfg->bEnabled) && pEthLink->Cfg.bEnabled )
                {
                    bChanged = TRUE;
                }
            }
        }

        /* Update the name -- just copy LinkName to Name field */
        rc = strcpy_s(pEthLink->StaticInfo.Name, sizeof(pEthLink->StaticInfo.Name), pCfg->LinkName);
        if(rc != EOK)
        {
          ERR_CHK(rc);
        }

        /* Save the configuraton */
        AnscCopyMemory(&pEthLink->Cfg, pCfg, sizeof(pEthLink->Cfg));
        CosaDmlEthLinkSavePsm(pEthContext, &pEthLink->Cfg);

        /* Reset the state if necessary */
        if ( bChanged )
        {
            pEthLink->DynamicInfo.LastChange = AnscGetTickInSeconds();
            CosaUtilGetIfStats(pEthLink->StaticInfo.Name, &pEthLink->LastStats);
        }

        return  ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlEthLinkUpdateStaticMac
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    )
{
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PDMSB_TR181_ETH_LINK            pEthLink;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pEthLink = CosaDmlEthLinkFindByInstNum(pEthContext, pCfg->InstanceNumber);

    /*RDKB-6734, CID-33535, 32988, perform null check before use*/
    if ( !pEthLink )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {

        UCHAR strMac[128] = {0};
        if ( -1 != _getMac(pEthLink->StaticInfo.Name,(char *) strMac) )
        {
            AnscCopyMemory(pEthLink->StaticInfo.MacAddress,strMac,6);
            AnscCopyMemory(pEntry->StaticInfo.MacAddress,strMac,6);
        }

        return  ANSC_STATUS_SUCCESS;
    }
}


ANSC_STATUS
CosaDmlEthLinkGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg
    )
{
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PDMSB_TR181_ETH_LINK            pEthLink;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pEthLink = CosaDmlEthLinkFindByInstNum(pEthContext, pCfg->InstanceNumber);

    if ( !pEthLink )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        AnscCopyMemory(pCfg, &pEthLink->Cfg, sizeof(pEthLink->Cfg));

        return  ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlEthLinkGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_LINK_DINFO    pInfo
    )
{
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PDMSB_TR181_ETH_LINK            pEthLink;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pEthLink = CosaDmlEthLinkFindByInstNum(pEthContext, ulInstanceNumber);

    if ( !pEthLink )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        pInfo->Status       = getIfStatus((PUCHAR)pEthLink->StaticInfo.Name, NULL);

        return  ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlEthLinkGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    )
{
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PDMSB_TR181_ETH_LINK            pEthLink;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pEthLink = CosaDmlEthLinkFindByInstNum(pEthContext, ulInstanceNumber);

    if ( !pEthLink )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        _ansc_memset(pStats, 0, sizeof(COSA_DML_ETH_STATS));

        CosaUtilGetIfStats(pEthLink->StaticInfo.Name, (PCOSA_DML_IF_STATS)pStats);
        
        pStats->BroadcastPacketsReceived    -= pEthLink->LastStats.BroadcastPacketsReceived;
        pStats->BroadcastPacketsSent        -= pEthLink->LastStats.BroadcastPacketsSent;
        pStats->BytesReceived               -= pEthLink->LastStats.BytesReceived;
        pStats->BytesSent                   -= pEthLink->LastStats.BytesSent;
        pStats->DiscardPacketsReceived      -= pEthLink->LastStats.DiscardPacketsReceived;
        pStats->DiscardPacketsSent          -= pEthLink->LastStats.DiscardPacketsSent;
        pStats->ErrorsReceived              -= pEthLink->LastStats.ErrorsReceived;
        pStats->ErrorsSent                  -= pEthLink->LastStats.ErrorsSent;
        pStats->MulticastPacketsReceived    -= pEthLink->LastStats.MulticastPacketsReceived;
        pStats->MulticastPacketsSent        -= pEthLink->LastStats.MulticastPacketsSent;
        pStats->PacketsReceived             -= pEthLink->LastStats.PacketsReceived;
        pStats->PacketsSent                 -= pEthLink->LastStats.PacketsSent;
        pStats->UnicastPacketsReceived      -= pEthLink->LastStats.UnicastPacketsReceived;
        pStats->UnicastPacketsSent          -= pEthLink->LastStats.UnicastPacketsSent;
        pStats->UnknownProtoPacketsReceived -= pEthLink->LastStats.UnknownProtoPacketsReceived;

        return ANSC_STATUS_SUCCESS;
    }
}

ANSC_STATUS
CosaDmlEthLinkUpdateStaticLowerLayerName
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    )
{
    PDMSB_TR181_ETH_CONTEXT         pEthContext     = (PDMSB_TR181_ETH_CONTEXT)hContext;
    PDMSB_TR181_ETH_LINK            pEthLink;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pEthLink = CosaDmlEthLinkFindByInstNum(pEthContext, ulInstanceNumber);

    if ( !pEthLink )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        AnscCopyMemory( pEthLink->StaticInfo.Name, pEntry->StaticInfo.Name,sizeof( pEntry->StaticInfo.Name ) );

        return  ANSC_STATUS_SUCCESS;
    }
}

/**********************************************************************
                    HELPER FUNCTION PROTOTYPES
**********************************************************************/

int _getMac(char* ifName, char* mac){

    struct ifreq ifr;
    int skfd=-1;
    errno_t rc = -1;
    
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    /* CID: 54085 Argument cannot be negative*/
    if(skfd == -1)
       return -1;

    rc = strcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), ifName);
    if(rc != EOK)
    {
       // CID 162577 : Resource leak (RESOURCE_LEAK)
       close(skfd);
       ERR_CHK(rc);
       return -1;
    }

    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
        if (errno == ENODEV) {
            close(skfd);
            return -1;
        }
    }
    
    if (ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {
        CcspTraceWarning(("cosa_ethernet_apis.c - getMac: Get interface %s error...\n", ifName));
        close(skfd);
        return -1;
    }
    close(skfd);

    AnscCopyMemory(mac, ifr.ifr_hwaddr.sa_data, 6);
    return 0; 

}


COSA_DML_IF_STATUS getIfStatus(const PUCHAR name, struct ifreq *pIfr)
{
    struct ifreq ifr;
    int skfd = -1;
    errno_t rc = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if (!isValid((char *) name)) {
        return COSA_DML_IF_STATUS_Error;
    }

    rc = strcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), (char *) name);
    if (rc != EOK) {
        ERR_CHK(rc);
        return COSA_DML_IF_STATUS_Error;
    }

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (skfd == -1) {
        return COSA_DML_IF_STATUS_Error;
    }

    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
        if (errno == ENODEV) {
            close(skfd);
            return COSA_DML_IF_STATUS_Error;
        }

        close(skfd);

        CcspTraceWarning(("cosa_ethernet_apis.c - getIfStatus: Get interface %s error...\n", name));

        if (getIfAvailability(name) == FALSE)
        {
            return COSA_DML_IF_STATUS_NotPresent;
        }

        return COSA_DML_IF_STATUS_Unknown;
    }

    close(skfd);

    if ( pIfr )
    {
        AnscCopyMemory(pIfr, &ifr, sizeof(struct ifreq));
    }

    if ( ifr.ifr_flags & IFF_UP )
    {
        return COSA_DML_IF_STATUS_Up;
    }

    return COSA_DML_IF_STATUS_Down;
}

//unused function
#if 0
static int setIfStatus(struct ifreq *pIfr)
{
    int skfd;
    
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    /* CID: 62903 Argument cannot be negative*/
    if (skfd == -1) 
        return -1;

    if (ioctl(skfd, SIOCSIFFLAGS, pIfr) < 0) {
        CcspTraceWarning(("cosa_ethernet_apis.c - setIfStatus: Set interface %s error...\n", pIfr->ifr_name));
        close(skfd);
        return -1;
    }
    close(skfd);

    return 0;
}
#endif

BOOLEAN getIfAvailability( const PUCHAR name )
{
    struct ifreq ifr;
    int skfd=-1;
    
    errno_t rc = -1;
    AnscTraceFlow(("%s... name %s\n", __FUNCTION__,name));

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    /* CID: 62903 fix*/
    if (skfd < 0)
    {
        CcspTraceWarning(("%s: Socket creation failed.\n", __FUNCTION__));
        return -1;
    }

    rc = strcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), (char *)name);
    if(rc != EOK)
    {
       ERR_CHK(rc);
       // CID 162578 : Resource leak (RESOURCE_LEAK)
       close(skfd);
       return -1;
    }

    if (!isValid((char*)name)) {
	// CID 162578 : Resource leak (RESOURCE_LEAK)
        close(skfd);
        return -1;
    }
    
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
        if (errno == ENODEV) {
            close(skfd);
            return -1;
        }
    }
    
    if (ioctl(skfd, SIOCGIFINDEX, &ifr) < 0) {
		
        CcspTraceWarning(("%s : Get interface %s error (%s)...\n", 
									__FUNCTION__, 
									name, 
									strerror( errno )));
        close( skfd );

		return FALSE;
    }
	
    close(skfd);

	return TRUE;
}
