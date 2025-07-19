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

    module: cosa_com_cisco_spvtg_ccsp_pam_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes middle layer for the COSA Data Model Library

        *  Pam_GetFirstIpInterfaceObjectName

        *  Pam_GetParamBoolValue
        *  Pam_GetParamIntValue
        *  Pam_GetParamUlongValue
        *  Pam_GetParamStringValue
        
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        Ding Hua

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "cosa_com_cisco_spvtg_ccsp_pam_dml.h"
#include "safec_lib_common.h"

#define PAM_FIRST_IP_INTERFACE      "PAM_FIRST_IP_INTERFACE"
#define PAM_MAX_IP_INTERFACE_NUM    (256)

#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE )
char gFirstUpstreamIpInterface[128]    = {0};
char gFirstDownstreamIpInterface[128]    = {0};
#endif
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
                            UTILITY ROUTINES
***********************************************************************/

ANSC_STATUS
Pam_GetFirstIpInterfaceObjectName
    (
        BOOL                        bUpstream,
        char*                       pIfObjName,
        PULONG                      pulObjNameSize
    );

ANSC_STATUS
Pam_GetFirstIpInterfaceObjectName
    (
        BOOL                        bUpstream,
        char*                       pIfObjName,
        PULONG                      pulObjNameSize
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_CANT_FIND;
    int                             iReturnValue        = 0;
    ULONG                           ulTotal             = 0;
    ULONG                           ulIndex             = 0;
    ULONG                           ulInstNum           = 0;
    ULONG                           ulLen               = 0;
    char                            pObjName[PAM_MAX_IP_INTERFACE_NUM]       = {0};
    char                            Alias[PAM_MAX_IP_INTERFACE_NUM]          = {0};
    ULONG                           AliasSize           = 0;
    BOOL                            bAliasUpstream      = FALSE;
    char                            Buffer[PAM_MAX_IP_INTERFACE_NUM]         = {0};
    ULONG                           BufferSize          = 0;
    char                            LowerLayers[PAM_MAX_IP_INTERFACE_NUM]    = {0};
    ULONG                           LowerLayersSize     = 0;
    BOOL                            bLowerLayerUpstream = FALSE;
    char*                           EnvIndex            = NULL;
    ULONG                           ulEnvIndex          = PAM_MAX_IP_INTERFACE_NUM;
    errno_t                         rc                  = -1;


   CcspTraceInfo(("[%s] -- Enter \n", __FUNCTION__));

#ifdef _COSA_SIM_
    // Hard coded, RTian 09/20/2013
    {
        if(bUpstream) {
            rc = sprintf_s(pIfObjName, *pulObjNameSize, "Device.IP.Interface.1.")
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            *pulObjNameSize = rc + 1;
            //        else pulObjNameSize = sprintf(pIfObjName, "Device.IP.Interface.4.") + 1;
	    CcspTraceInfo(("[%s] -- Exit, defined _COSA_SIM_ pulObjNameSize: %d from \n", __FUNCTION__, pulObjNameSize));

            return ANSC_STATUS_SUCCESS;
        }
    }
#endif

    ulTotal = CosaGetParamValueUlong("Device.IP.InterfaceNumberOfEntries");

    EnvIndex = getenv(PAM_FIRST_IP_INTERFACE);
    if(EnvIndex != NULL)
    {
        ulEnvIndex = atol(EnvIndex);
    }
    
    for ( ulIndex = 0; ulIndex < ulTotal; ulIndex++ )
    {
        if(EnvIndex != NULL && ulEnvIndex < PAM_MAX_IP_INTERFACE_NUM)
        {
            ulInstNum = CosaGetInstanceNumberByIndex("Device.IP.Interface.", ulEnvIndex);
            if( 0 == ulInstNum)
            {
                EnvIndex    = NULL;
                ulEnvIndex  = PAM_MAX_IP_INTERFACE_NUM;
            }
            else
            {
                ulIndex     = (ULONG)EnvIndex;
            }
        }
        ulInstNum = CosaGetInstanceNumberByIndex("Device.IP.Interface.", ulIndex);

        if ( 0 == ulInstNum )
        {
            CcspTraceWarning(("Pam_GetFirstIpInterfaceObjectName -- invalid Device.IP.Interface instance number!\n"));
            returnStatus = ANSC_STATUS_FAILURE;
            break;
        }
        else
        {
            rc = sprintf_s(pObjName, sizeof(pObjName), "Device.IP.Interface.%lu.LowerLayers", ulInstNum);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              returnStatus = ANSC_STATUS_FAILURE;
              break;
            }
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)	    
#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
            /* For FirstDownstreamIpInterface ignore the FirstUpstreamIpInterface set by WanManager for WanUnification Builds. */
            if( bUpstream == FALSE && strlen(gFirstUpstreamIpInterface) > 0 &&
                strncmp(gFirstUpstreamIpInterface, pObjName, strlen(gFirstUpstreamIpInterface))==0)
            {
                    CcspTraceInfo(("%s %d Ignoring gFirstUpstreamIpInterface  %s for FirstDownstreamIpInterface\n", __FUNCTION__, __LINE__, pObjName));
                    continue;
            }
#endif
#endif	    
            /*CcspTraceInfo(("Checking %s...\n", pObjName));*/

            LowerLayersSize = sizeof(LowerLayers);
            iReturnValue    = CosaGetParamValueString(pObjName, LowerLayers, &LowerLayersSize);
            
            if ( iReturnValue != 0 )
            {
                returnStatus = ANSC_STATUS_FAILURE;
                break;
            }
            else if ( LowerLayersSize == 0 )
            {
                continue;
            }

            do
            {
                /*
                 *  We have to check whether LowerLayers is a partial object name (without '.' at the end) or not 
                 *  Assuming only one lower layer value for now
                 *  Look for the LowerLayer with a empty LowerLayers parameter
                 */
                ulLen = _ansc_strlen(LowerLayers);

                if ( LowerLayers[ulLen - 1] == '.' )
                {
                    LowerLayers[ulLen - 1] = '\0';
                }

                rc = strcpy_s(pObjName, sizeof(pObjName), LowerLayers);
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  returnStatus = ANSC_STATUS_FAILURE;
                  break;
                }
                rc = sprintf_s(Buffer, sizeof(Buffer), "%s.LowerLayers", LowerLayers);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  returnStatus = ANSC_STATUS_FAILURE;
                  break;
                }
            
                CcspTraceDebug(("Checking %s...\n", Buffer));
                
                LowerLayersSize = sizeof(LowerLayers);
                iReturnValue    = CosaGetParamValueString(Buffer, LowerLayers, &LowerLayersSize);

                if ( iReturnValue != 0 )
                {
                    break;
                }
                else if ( LowerLayersSize == 0 )
                {
                    break;
                }
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
                else if(strncmp(LowerLayers,pObjName,strlen(LowerLayers)) == 0)
                {
                    break;
                }
#endif
            }
            while (TRUE);
            
            /*
             *  Check the Upstream parameter.
             *  Note, not all error cases are covered well here, which happened to help to pass the test
             *  on DRG -- LowerLayers parameter of IP.Interface on top of Bridge.1 is not actually correct.  
             */
            rc = sprintf_s(Buffer, sizeof(Buffer), "%s.Upstream", pObjName);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              returnStatus = ANSC_STATUS_FAILURE;
              break;
            }
            bLowerLayerUpstream = CosaGetParamValueBool(Buffer);
            
            if ( (bUpstream == TRUE) && (bLowerLayerUpstream == FALSE) )
            {
                /* Consider a case of DOCSIS Wan, check Alias of LowerLayers interface */
                rc = sprintf_s(Buffer, sizeof(Buffer), "%s.Alias", pObjName);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  returnStatus = ANSC_STATUS_FAILURE;
                  break;
                }

                AliasSize = sizeof(Alias);
                iReturnValue = CosaGetParamValueString(Buffer, Alias, &AliasSize);
                if ( (iReturnValue == 0) && (AliasSize > 0) )
                {
                    if ( strstr(Alias, "Wan") != NULL )
                    {
                        bAliasUpstream = TRUE;
                    }
                }
            }

            if ( (bUpstream == bLowerLayerUpstream) || bAliasUpstream )
            {
                CcspTraceDebug
                    ((
                         "Found the lowest %s layer %s for IP interface Device.IP.Interface.%lu.\n",
                         bUpstream ? "upstream" : "downstream",
                         LowerLayers,
                         ulInstNum
                    ));

                rc = sprintf_s(Buffer, sizeof(Buffer), "Device.IP.Interface.%lu.", ulInstNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  returnStatus = ANSC_STATUS_FAILURE;
                  break;
                }
                BufferSize = _ansc_strlen(Buffer);
                
                if ( BufferSize < *pulObjNameSize )
                {
                    rc = strcpy_s(pIfObjName,*pulObjNameSize, Buffer);
                    ERR_CHK(rc);
                    *pulObjNameSize = BufferSize+1;
                    returnStatus    = ANSC_STATUS_SUCCESS;
                }
                else
                {
                    *pulObjNameSize = BufferSize+1;
                    returnStatus = ANSC_STATUS_MORE_DATA;
                }

                break;                
            }
        }
    }

    CcspTraceInfo(("[%s] -- Exit returnStatus: %lu\n", __FUNCTION__, returnStatus));
    return  returnStatus;
}


/***********************************************************************

 APIs for Object:

    InterfaceStack.{i}.

    *  Pam_GetParamBoolValue
    *  Pam_GetParamIntValue
    *  Pam_GetParamUlongValue
    *  Pam_GetParamStringValue

***********************************************************************/


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceStack_GetParamBoolValue
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
Pam_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pam_GetParamIntValue
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
Pam_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pam_GetParamUlongValue
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
Pam_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);

    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pam_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pulSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pulSize = required size)
                -1 if not supported.

**********************************************************************/

ULONG
Pam_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    int                             iReturnValue        = 0;
    char                            IpIfObjName[128]    = {0};
    ULONG                           IpIfObjNameSize     = 0;
    ULONG                           ulInstNum           = 0;
    char                            Buffer[128]         = {0};
    ULONG                           BufferSize          = 0;
    errno_t                         rc                  = -1;

    UNREFERENCED_PARAMETER(hInsContext);
    /* CID: 269727 fix*/
    // CcspTraceInfo(("[%s] -- '%s', pValue addr: 0x%s, pulSize addr: 0x%ln \n", __FUNCTION__, ParamName, pValue, pulSize));


    IpIfObjNameSize = sizeof(IpIfObjName) - 1;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "FirstUpstreamIpInterface") == 0)
    {
        /* We hardcode the value here temporarily. In the future, if LowerLayers mechanism works well, we can change back*/
        /*
        AnscCopyString(pValue, "Device.IP.Interface.1.");
        *pulSize = _ansc_strlen("Device.IP.Interface.1.");
        return  0;
        */
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE )
        if(_ansc_strlen(gFirstUpstreamIpInterface) > 0)
        {
            /* Find FirstUpstreamIpInterface only if WanIface changed */
            AnscCopyString(pValue, gFirstUpstreamIpInterface);
            *pulSize = _ansc_strlen(gFirstUpstreamIpInterface);
            return  0;
        }
#endif
        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(TRUE, IpIfObjName, &IpIfObjNameSize) )
        {
            if ( IpIfObjNameSize < *pulSize )
            {
                rc = strcpy_s(pValue, *pulSize, IpIfObjName);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return -1;
                }
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE )
                /* Update Global variable */
                strcpy_s(gFirstUpstreamIpInterface,sizeof(gFirstUpstreamIpInterface), IpIfObjName);
#endif
                return  0;
            }
            else
            {
                *pulSize = IpIfObjNameSize + 1;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }
    else if (strcmp(ParamName, "FirstDownstreamIpInterface") == 0)
    {
        /* We hardcode the value here temporarily. In the future, if LowerLayers mechanism works well, we can change back*/
        /*
        AnscCopyString(pValue, "Device.IP.Interface.4.");
        *pulSize = _ansc_strlen("Device.IP.Interface.4.");
        return  0;
        */

#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE )
        if(_ansc_strlen(gFirstDownstreamIpInterface) > 0)
        {
            /* Find FirstDownstreamIpInterface only if WanIface changed */
            AnscCopyString(pValue, gFirstDownstreamIpInterface);
            *pulSize = _ansc_strlen(gFirstDownstreamIpInterface);
            return  0;
        }
#endif
      //      CcspTraceInfo(("Pam_GetParamStringValue -- FirstDownstreamIpInterface case\n"));

        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(FALSE, IpIfObjName, &IpIfObjNameSize) )
        {

	  //	  CcspTraceInfo(("Pam_GetParamStringValue -- '%s' Pam_GetFirstIpInterfaceObjectName ret success\n", ParamName));
	  CcspTraceInfo(("[%s] -- '%s' IpIfObjNameSize: %lu, pulSize: %lu\n", __FUNCTION__, ParamName, IpIfObjNameSize, *pulSize));

            if ( IpIfObjNameSize < *pulSize )
            {
                rc = strcpy_s(pValue, *pulSize, IpIfObjName);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return -1;
                }
		CcspTraceInfo(("[%s] -- pValue: %s\n", __FUNCTION__, pValue));
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE )
                /* Update Global variable */
                strcpy_s(gFirstDownstreamIpInterface,sizeof(gFirstDownstreamIpInterface), IpIfObjName);
#endif
                return  0;
            }
            else
            {
                *pulSize = IpIfObjNameSize + 1;
                return  1;
            }
        }
        else
        { 
	  CcspTraceInfo(("[%s] -- '%s' Pam_GetFirstIpInterfaceObjectName ret failed\n", __FUNCTION__, ParamName));
            return  -1;
        }
    }
    else if (strcmp(ParamName, "FirstUpstreamIpv4Address") == 0)
    {
        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(TRUE, IpIfObjName, &IpIfObjNameSize) )
        {
            /*CID: 61333 - Calling risky function - Fix*/
            strncat(IpIfObjName, "IPv4Address.", sizeof(IpIfObjName)-strlen(IpIfObjName)-1);            
            ulInstNum = CosaGetInstanceNumberByIndex(IpIfObjName, 0);
            
            if ( ulInstNum == 0 )
            {
                 return  -1;
            }
            else
            {
                rc = sprintf_s(Buffer, sizeof(Buffer), "%lu.", ulInstNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  return -1;
                }
                strncat(IpIfObjName, Buffer, sizeof(IpIfObjName)-strlen(IpIfObjName)-1);
                strncat(IpIfObjName, "IPAddress", sizeof(IpIfObjName)-strlen(IpIfObjName)-1);
            }

            BufferSize   = sizeof(Buffer);
            iReturnValue = CosaGetParamValueString(IpIfObjName, Buffer, &BufferSize);
            
            if ( iReturnValue != 0 )
            {
                CcspTraceWarning(("Pam_GetParamStringValue -- failed to retrieve IP address, error code %d!\n", iReturnValue));
                
                return  -1;
            }

            if ( BufferSize < *pulSize )
            {
                rc = strcpy_s(pValue, *pulSize, Buffer);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return -1;
                }
                return  0;
            }
            else
            {
                *pulSize = IpIfObjNameSize + 1;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }
    else if (strcmp(ParamName, "FirstUpstreamIpv4SubnetMask") == 0)
    {
        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(TRUE, IpIfObjName, &IpIfObjNameSize) )
        {
            _ansc_strcat(IpIfObjName, "IPv4Address.");
            ulInstNum = CosaGetInstanceNumberByIndex(IpIfObjName, 0);
            
            if ( ulInstNum == 0 )
            {
                 return  -1;
            }
            else
            {
                rc = sprintf_s(Buffer, sizeof(Buffer), "%lu.", ulInstNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  return -1;
                }
                _ansc_strcat(IpIfObjName, Buffer);
                _ansc_strcat(IpIfObjName, "SubnetMask");
            }

            BufferSize   = sizeof(Buffer);
            iReturnValue = CosaGetParamValueString(IpIfObjName, Buffer, &BufferSize);

            if ( iReturnValue != 0 )
            {
                CcspTraceWarning(("Pam_GetParamStringValue -- failed to retrieve subnet mask, error code %d!\n", iReturnValue));
                
                return  -1;
            }

            if ( BufferSize < *pulSize )
            {
                rc = strcpy_s(pValue, *pulSize, Buffer);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return -1;
                }
                return  0;
            }
            else
            {
                _ansc_strncpy(pValue, Buffer, *pulSize);
                *pulSize = IpIfObjNameSize + 1;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }
    else if (strcmp(ParamName, "FirstDownstreamIpv4Address") == 0)
    {
        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(FALSE, IpIfObjName, &IpIfObjNameSize) )
        {
            _ansc_strcat(IpIfObjName, "IPv4Address.");            
            ulInstNum = CosaGetInstanceNumberByIndex(IpIfObjName, 0);
            
            if ( ulInstNum == 0 )
            {
                 return  -1;
            }
            else
            {
                rc = sprintf_s(Buffer, sizeof(Buffer), "%lu.", ulInstNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  return -1;
                }
                strncat(IpIfObjName, Buffer, sizeof(IpIfObjName) -strlen(IpIfObjName)-1);
                strncat(IpIfObjName, "IPAddress", sizeof(IpIfObjName)-strlen(IpIfObjName)-1);
            }

            BufferSize   = sizeof(Buffer);
            iReturnValue = CosaGetParamValueString(IpIfObjName, Buffer, &BufferSize);
            
            if ( iReturnValue != 0 )
            {
                CcspTraceWarning(("Pam_GetParamStringValue -- failed to retrieve IP address, error code %d!\n", iReturnValue));
                
                return  -1;
            }

            if ( BufferSize < *pulSize )
            {
                rc = strcpy_s(pValue, *pulSize, Buffer);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return -1;
                }
                return  0;
            }
            else
            {
                _ansc_strncpy(pValue, Buffer, *pulSize);       
                *pulSize = IpIfObjNameSize + 1;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }
    else if (strcmp(ParamName, "FirstDownstreamIpv4SubnetMask") == 0)
    {
        if ( ANSC_STATUS_SUCCESS == Pam_GetFirstIpInterfaceObjectName(FALSE, IpIfObjName, &IpIfObjNameSize) )
        {
            _ansc_strcat(IpIfObjName, "IPv4Address.");
            ulInstNum = CosaGetInstanceNumberByIndex(IpIfObjName, 0);
            
            if ( ulInstNum == 0 )
            {
                 return  -1;
            }
            else
            {
                rc = sprintf_s(Buffer, sizeof(Buffer), "%lu.", ulInstNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  return -1;
                }
                _ansc_strcat(IpIfObjName, Buffer);
                _ansc_strcat(IpIfObjName, "SubnetMask");
            }

            BufferSize   = sizeof(Buffer);
            iReturnValue = CosaGetParamValueString(IpIfObjName, Buffer, &BufferSize);

            if ( iReturnValue != 0 )
            {
                CcspTraceWarning(("Pam_GetParamStringValue -- failed to retrieve subnet mask, error code %d!\n", iReturnValue));
                
                return  -1;
            }

            if ( BufferSize < *pulSize )
            {
                rc = strcpy_s(pValue, *pulSize, Buffer);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return -1;
                }
                return  0;
            }
            else
            {
                _ansc_strncpy(pValue, Buffer, *pulSize);
                *pulSize = IpIfObjNameSize + 1;
                return  1;
            }
        }
        else
        {
            return  -1;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

