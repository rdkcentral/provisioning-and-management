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

    module: cosa_deviceinfo_dml.c

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

        01/14/2011    initial revision.

**************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <syscfg/syscfg.h>
#include "ansc_platform.h"
#include "cosa_deviceinfo_dml.h"
#include "cosa_advsec_utils.h"
#include "cosa_rbus_handler_apis.h"
#include "dml_tr181_custom_cfg.h"
#include "safec_lib_common.h"
#include "secure_wrapper.h"
#include "cosa_drg_common.h"
#include "cosa_deviceinfo_apis.h"
#include "cosa_deviceinfo_internal.h"
#include "ccsp/platform_hal.h"
#ifdef _MACSEC_SUPPORT_
#include "ccsp/ccsp_hal_ethsw.h"
#endif
#include <syscfg/syscfg.h>
#include <sys/statvfs.h>
#include "dslh_definitions_database.h"

#if defined (_XB6_PRODUCT_REQ_)
#include "bt_hal.h"
#endif

#if defined (_ARRIS_XB6_PRODUCT_REQ_) //ARRISXB6-7328, ARRISXB6-7332
#include "cm_hal_oem.h"
#endif

#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
#include <unistd.h>
#include <sys/types.h>
#endif

#include "ccsp_base_api.h"
#include "messagebus_interface_helper.h"

#include <stdbool.h>
#include "cosa_deviceinfo_apis.h"
#include "ccsp_psm_helper.h"
#include <rbus/rbus.h>

#ifdef CORE_NET_LIB
#include <libnet.h>
#endif

extern ULONG g_currentBsUpdate;
extern char g_currentParamFullName[512];
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
extern void* g_pDslhDmlAgent;
static BOOL g_clearDB = false;
static char *g_AdvSecDefaultEndpointURL = "Advsecurity_DefaultEndpointURL";

void Send_Notification_Task(char* delay, char* startTime, char* download_status, char* status, char *system_ready_time, char * priority,  char *current_fw_ver, char *download_fw_ver);
void set_firmware_download_start_time(char *start_time);
char* get_firmware_download_start_time();
void *handleBleRestart(void *arg);
#if (defined _COSA_INTEL_XB3_ARM_)
BOOL CMRt_Isltn_Enable(BOOL status);
#endif

#define MAX_ALLOWABLE_STRING_LEN  256

#define BOOTSTRAP_INFO_FILE_BACKUP "/nvram/bootstrap.json"
#define CLEAR_TRACK_FILE "/nvram/ClearUnencryptedData_flags"
#define NVRAM_BOOTSTRAP_CLEARED (1 << 0)

#define MAX_T2_VER_LEN 16

#define IS_UPDATE_ALLOWED_IN_DM(paramName, requestorStr) ({                                                                                       \
    int rc = -1;                                                                                                                                  \
    int ind = -1;                                                                                                                                 \
    int found = 0;                                                                                                                                \
    if( g_currentBsUpdate == DSLH_CWMP_BS_UPDATE_firmware )                                                                                       \
    {                                                                                                                                             \
         found = 1;                                                                                                                               \
    }                                                                                                                                             \
    else if(g_currentBsUpdate == DSLH_CWMP_BS_UPDATE_rfcUpdate)                                                                                   \
    {                                                                                                                                             \
        rc = strcmp_s(BS_SOURCE_RFC_STR, strlen(BS_SOURCE_RFC_STR), requestorStr, &ind);                                                          \
        ERR_CHK(rc);                                                                                                                              \
        if((rc == EOK) && (ind))                                                                                                                  \
        {                                                                                                                                         \
            found = 1;                                                                                                                            \
        }                                                                                                                                         \
    }                                                                                                                                             \
    if(found == 1)                                                                                                                                \
    {                                                                                                                                             \
        CcspTraceWarning(("Do NOT allow override of param: %s bsUpdate = %lu, requestor = %s\n", paramName, g_currentBsUpdate, requestorStr));     \
        return FALSE;                                                                                                                             \
    }                                                                                                                                             \
})

// If the requestor is RFC but the param was previously set by webpa, do not override it.
#define IS_UPDATE_ALLOWED_IN_JSON(paramName, requestorStr, UpdateSource) ({                                                                        \
   int rc = -1;                                                                                                                                    \
   int ind = -1;                                                                                                                                   \
   rc = strcmp_s(BS_SOURCE_RFC_STR, strlen(BS_SOURCE_RFC_STR), requestorStr, &ind);                                                                \
   ERR_CHK(rc);                                                                                                                                    \
   if((rc == EOK) && (!ind))                                                                                                                       \
   {                                                                                                                                               \
      rc = strcmp_s(BS_SOURCE_WEBPA_STR, strlen(BS_SOURCE_WEBPA_STR), UpdateSource, &ind);                                                         \
      ERR_CHK(rc);                                                                                                                                 \
      if((rc == EOK) && (!ind))                                                                                                                    \
      {                                                                                                                                            \
        CcspTraceWarning(("Do NOT allow override of param: %s requestor = %lu updateSource = %s\n", paramName, g_currentWriteEntity, UpdateSource));  \
        return FALSE;                                                                                                                              \
      }                                                                                                                                            \
   }                                                                                                                                               \
})

#define DEVICE_PROPS_FILE  "/etc/device.properties"
#define SYSTEMCTL_CMD "systemctl start lxydnld.service &"
#ifdef FEATURE_COGNITIVE_WIFIMOTION
#define COGNITIVE_WIFIMOTION_CFG "wifimotion_enabled"
#endif
// CredDwnld_Use String is restricted to true/false
#define MAX_USE_LEN 8
// Box type will be XB3
#define BOX_TYPE_LEN 5
// ATOM IP length
#define IP_LEN 20

#ifdef COLUMBO_HWTEST
//RDKB-33114: Default values defined here due to objects not being persistent by design
#define DEFAULT_HWST_PTR_CPU_THRESHOLD 80
#define DEFAULT_HWST_PTR_DRAM_THRESHOLD 20
#define DEFAULT_HWST_PTR_FREQUENCY 720
#define HWSELFTEST_START_MIN_SPACE (200*1024) //200KB
#endif

#if defined(_COSA_INTEL_XB3_ARM_)
#if defined(_LXY_CXB3_ATOM_IP_)
#define ATOM_IP "169.254.101.2"
#elif defined(_LXY_AXB3_ATOM_IP_)
#define ATOM_IP "192.168.254.254"
#endif
static const char *atomIp = ATOM_IP;
#endif

#ifdef _COSA_INTEL_XB3_ARM_
    #define BLOCKLIST_FILE "/nvram/Blocklist_XB3.txt"
#else
    #define BLOCKLIST_FILE "/opt/secure/Blocklist_file.txt"
#endif

#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
int sock;
int id = 0;
#endif

#define NUM_OF_DEVICEINFO_VALUES (sizeof(deviceinfo_set_table)/sizeof(deviceinfo_set_table[0]))

#define CALC_SPACE_LEFT(x) (sizeof(x) - strlen(x) - 1)
enum  pString_val {
    UIACCESS,
    UISUCCESS,
    UIFAILED,
    REBOOTDEVICE,
    FACTORYRESET,
    CAPTIVEPORTALFAILURE
};

typedef struct {
  char     *name;
  enum  pString_val type;
} DEVICEINFO_SET_VALUE;

static const DEVICEINFO_SET_VALUE deviceinfo_set_table[] = {
    { "ui_access",UIACCESS },
    { "ui_success",UISUCCESS},
    { "ui_failed", UIFAILED },
    { "reboot_device", REBOOTDEVICE},
    { "factory_reset",	FACTORYRESET },
    {  "captiveportal_failure" , CAPTIVEPORTALFAILURE }
};

static int urlStartsWith(const char *haystack, const char *needle)
{
   if(strncmp(haystack, needle, strlen(needle)) == 0)
       return 0;
   return 1;
}

ANSC_STATUS isValidUrl( char *inputparam )
{
    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;

    if(urlStartsWith(inputparam, "https://"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
// To check for possible command injection
    if(strstr(inputparam, ";"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "&"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "&&"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "|"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "||"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "<"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, ">"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "`"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "'"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "\\"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "!"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "$("))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam, "\n"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }

    return returnStatus;
}

static int update_pValue (char *pValue, PULONG pulSize, char *str)
{
    if (!str)
        return -1;

    size_t len = strlen(str);

    if (len < *pulSize)
    {
        memcpy(pValue, str, len + 1);
        return 0;
    }

    *pulSize = len + 1;
    return 1;
}

static BOOL AnscValidateInputString (char *pString) {

    char disallowed_chars[] = "<>%`|'";       /*To avoid command injection */
    int i = 0;

   /* check if pstring doesn't hold NULL or whitespaces */
    if((pString == NULL) || (*pString =='\0')) {
        return FALSE;
    }

    while(pString[i] != '\0')
    {
        if (!strchr(disallowed_chars,pString[i]))
            i++;
        else
            return FALSE;
    }
    return TRUE;
}

static int get_deviceinfo_from_name(char *name, enum pString_val *type_ptr)
{
  int rc = -1;
  int ind = -1;
  int i = 0;
  size_t strsize = 0;
  if((name == NULL) || (type_ptr == NULL))
     return 0;

  strsize = strlen(name);

  for (i = 0 ; i < (int)NUM_OF_DEVICEINFO_VALUES ; ++i)
  {
      rc = strcmp_s(name, strsize, deviceinfo_set_table[i].name, &ind);
      ERR_CHK(rc);
      if((rc == EOK) && (!ind))
      {
          *type_ptr = deviceinfo_set_table[i].type;
          return 1;
      }
  }
  return 0;
}

#ifdef COLUMBO_HWTEST
static unsigned long long GetAvailableSpace_tmp()
{
    //Check if there is enough space to atleast start HHT.
    unsigned long long result = 0;
    struct statvfs sfs;
    if(statvfs("/tmp", &sfs) != -1)
    {
        result = (unsigned long long)sfs.f_bsize * sfs.f_bavail;
        CcspTraceInfo(("%llu space left in tmp\n", result));
    }
    return result;
}

static void UpdateSettingsFile( char param[64], char value[10] )
{
    CcspTraceInfo(("\nUpdateSettingsFile\n"));
    errno_t          rc                  = -1;

    FILE* fp = fopen( "/tmp/.hwselftest_settings", "r");
    if( fp == NULL)
    {
        fp = fopen ("/tmp/.hwselftest_settings", "w");

        if( fp != NULL)
        {
          fprintf(fp, "%s%s\n", param, value);
          fclose(fp);
        }
        return;
    }

    char Line[120] = {'\0'};
    char FileData[1200] = {'\0'};
    int isFound = 0;
    int firstLine = 0;

    while(NULL != fgets(Line, 120, fp))
    {
        if(NULL != strstr(Line, param))
        {
            int index  = 0;
            isFound = 1;
            int startIndex = strlen(Line) - (strlen(Line) - strlen(param));
            int endIndex = strlen(value);

            for(index = 0; index <  endIndex;index ++)
            {
                Line[startIndex] = value[index];
                startIndex++;
            }
            Line[startIndex] = '\0';
            strcat(Line,"\n");
        }

        if(0 == firstLine)
        {
            if(strstr(param, "FREQ") && strstr(Line, "FirstPTR"))
            {
                continue;
            }
            firstLine = 1;
            rc = strcpy_s(FileData, sizeof(FileData), Line);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               fclose(fp);
               return;
            }
        }
        else
        {
            if(strstr(param, "FREQ") && strstr(Line, "FirstPTR"))
            {
                continue;
            }
            strncat(FileData, Line, CALC_SPACE_LEFT(FileData));
        }

    }
    fclose(fp);

    if(0 == isFound)
    {
        rc = sprintf_s(Line, sizeof(Line), "%s%s\n", param,value);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return;
        }

        strncat(FileData, Line, CALC_SPACE_LEFT(FileData));
    }

    fp = fopen ("/tmp/.hwselftest_settings", "w");

    if( fp != NULL)
    {
        fputs(FileData,fp);
        fclose(fp);
    }
}
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

 APIs for Object:

    DeviceInfo.

    *  DeviceInfo_GetParamBoolValue
    *  DeviceInfo_GetParamIntValue
    *  DeviceInfo_GetParamUlongValue
    *  DeviceInfo_GetParamStringValue
    *  DeviceInfo_SetParamBoolValue
    *  DeviceInfo_SetParamIntValue
    *  DeviceInfo_SetParamUlongValue
    *  DeviceInfo_SetParamStringValue
    *  DeviceInfo_Validate
    *  DeviceInfo_Commit
    *  DeviceInfo_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamBoolValue
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
DeviceInfo_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    BOOL                            bReturnValue;
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    if (strcmp(ParamName, "ClearResetCount") == 0)
    {
        *pBool = FALSE ; 
        return TRUE;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_AkerEnable") == 0)
    {
        /* collect value */
        char buf[8];
        if( syscfg_get( NULL, "X_RDKCENTRAL-COM_AkerEnable", buf, sizeof(buf))==0)
        {
            if (strcmp(buf, "true") == 0)
            {
                *pBool = TRUE;
            }
            else
            {
                *pBool = FALSE;
            }
        }
        else
        {
            CcspTraceWarning(("%s syscfg_get failed  for AkerEnable\n",__FUNCTION__));
            *pBool = FALSE;
        }

        return TRUE;
    }
    if(strcmp(ParamName, "BoolParam") == 0)
    {
        *pBool = pMyObject->RollBackParam;
        return TRUE;
    }
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_OnBoarding_DeleteLogs") == 0)
    {
        *pBool = FALSE ;
        return TRUE;
    }

    if (strcmp(ParamName, "CustomDataModelEnabled") == 0)
    {
        CosaDmlGiGetCustomDataModelEnabled(NULL, pBool);
        return TRUE;
    }

  bReturnValue =
        DeviceInfo_GetParamBoolValue_Custom
            (
                hInsContext,
                ParamName,
                pBool
            );

    if ( bReturnValue )
    {
        return bReturnValue;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamIntValue
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
DeviceInfo_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO   pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "IntParam") == 0)
    {
        *pInt =  pMyObject->RollBackIntParam;
        return TRUE;
    }
    /* check the parameter name and return the corresponding value */
#ifndef FEATURE_FWUPGRADE_MANAGER
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadAndFactoryReset") == 0)
    {
        *pInt = 0;
        return TRUE;
    }
#else
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
#endif
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamUlongValue
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
DeviceInfo_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "UpTime") == 0)
    {
        /* collect value */
        *puLong = CosaDmlDiGetUpTime(NULL);
        return TRUE;
    }
	/* Required for xPC sync */
	if (strcmp(ParamName, "X_RDKCENTRAL-COM_ConfigureDocsicPollTime") == 0)
    {
     #if !defined(_COSA_BCM_MIPS_) && !defined(_ENABLE_DSL_SUPPORT_)
        /* collect value */
	   FILE *fp;
	   char buff[30];
	   int retValue;
	   memset(buff,0,sizeof(buff));
           *puLong = 0;
   	   fp = fopen("/nvram/docsispolltime.txt", "r");
	   if(!fp)
	   {
		 CcspTraceError(("%s falied to open /nvram/docsispolltime.txt file \n",__FUNCTION__));
		 return FALSE;
	   }
           retValue = fscanf(fp, "%29s", buff);      
            /* CID 64133 fix*/
           if( (retValue != -1) && (buff[0] != '\0' ) )
	   {
          	 *puLong = atoi(buff);
           }

	   fclose(fp);
	 #else
	   *puLong = 0;
	 #endif
           return TRUE;
    }
    
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_BootTime") == 0)
    {
        /* collect value */
       *puLong = CosaDmlDiGetBootTime(NULL);
        return TRUE;
    }
    
    if (strcmp(ParamName, "FactoryResetCount") == 0)
    {
        /* collect value */
        CosaDmlDiGetFactoryResetCount(NULL,puLong);
        return TRUE;
    }
	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DeviceInfo_GetParamStringValue
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
DeviceInfo_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    ULONG                           ReturnValue;
    errno_t                         rc        = -1;
    
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "DeviceCategory") == 0)
    {
#if defined (FEATURE_GPON)
        return update_pValue (pValue, pulSize, "Fiber_Gateway");
#else
        return update_pValue (pValue, pulSize, "DOCSIS_Gateway");
#endif
    }

    if (strcmp(ParamName, "Manufacturer") == 0)
    {
        CosaDmlDiGetManufacturer(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "ManufacturerOUI") == 0)
    {
        CosaDmlDiGetManufacturerOUI(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "ModelName") == 0)
    {
        if (*pulSize <= 64) {
            *pulSize = 64 + 1;
            return 1;
        }

        if (platform_hal_GetModelName(pValue) != RETURN_OK)
            return -1;

        return 0;
    }

    if (strcmp(ParamName, "Description") == 0)
    {
        CosaDmlDiGetDescription(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "ProductClass") == 0)
    {
        CosaDmlDiGetProductClass(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "SerialNumber") == 0)
    {
        CosaDmlDiGetSerialNumber(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "HardwareVersion") == 0)
    {
        CosaDmlDiGetHardwareVersion(NULL, pValue, pulSize);
        return 0;
    }

    if ((strcmp(ParamName, "SoftwareVersion") == 0) ||
        (strcmp(ParamName, "X_CISCO_COM_FirmwareName") == 0) ||
        (strcmp(ParamName, "X_RDK_FirmwareName") == 0))
    {
        if (*pulSize <= 64) {
            *pulSize = 64 + 1;
            return 1;
        }

        if (platform_hal_GetFirmwareName(pValue, *pulSize) != RETURN_OK)
            return -1;

        return 0;
    }

    if (strcmp(ParamName, "AdditionalHardwareVersion") == 0)
    {
        CosaDmlDiGetAdditionalHardwareVersion(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "AdditionalSoftwareVersion") == 0)
    {
        if (*pulSize <= 64) {
            *pulSize = 64 + 1;
            return 1;
        }

        if (platform_hal_GetSoftwareVersion(pValue, *pulSize) != RETURN_OK)
            return -1;

        return 0;
    }

    if (strcmp(ParamName, "ProvisioningCode") == 0)
    {
        CosaDmlDiGetProvisioningCode(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "FirstUseDate") == 0)
    {
        CosaDmlDiGetFirstUseDate(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "X_CISCO_COM_BootloaderVersion") == 0)
    {
        CosaDmlDiGetBootloaderVersion(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "X_CISCO_COM_FirmwareBuildTime") == 0)
    {
        CosaDmlDiGetFirmwareBuildTime(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "X_CISCO_COM_BaseMacAddress") == 0)
    {
        CosaDmlDiGetBaseMacAddress(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "Hardware") == 0)
    {
        CosaDmlDiGetHardware(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "Hardware_MemUsed") == 0)
    {
        CosaDmlDiGetHardware_MemUsed(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "Hardware_MemFree") == 0)
    {
        CosaDmlDiGetHardware_MemFree(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "X_CISCO_COM_AdvancedServices") == 0)
    {
        CosaDmlDiGetAdvancedServices(NULL, pValue, pulSize);
        return 0;
    }

    if (strcmp(ParamName, "X_CISCO_COM_ProcessorSpeed") == 0)
    {
        CosaDmlDiGetProcessorSpeed(NULL, pValue, pulSize);
        return 0;
    }

    /* Required for WebPA timestamp */
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_SystemTime") == 0)
    {
        struct timespec sysTime;

        if( clock_gettime(CLOCK_REALTIME, &sysTime) != -1 )
        {
              static long prevtime_sec = 0;
              static long prevtime_nsec = 0;
              static int count = 0;
              char sbuf[32] = {0};

              if( (sysTime.tv_sec == prevtime_sec) && (sysTime.tv_nsec == prevtime_nsec) )
              {
                    count++;
              } else {
                    count = 0;
              }
              prevtime_sec = sysTime.tv_sec;
              prevtime_nsec = sysTime.tv_nsec;
              sysTime.tv_nsec += count;

              if( sysTime.tv_nsec > 999999999L)
              {
                   sysTime.tv_sec = sysTime.tv_sec + 1;
                   sysTime.tv_nsec = sysTime.tv_nsec - 1000000000L;
              }                   

              rc = sprintf_s(sbuf, sizeof(sbuf), "%ld.%09ld", sysTime.tv_sec, sysTime.tv_nsec);
              if(rc < EOK)
              {
                ERR_CHK(rc);
                return -1;
              }

              rc = strcpy_s(pValue, *pulSize, sbuf);
              if(rc != EOK)
              {
                ERR_CHK(rc);
                return -1;
              }
        }
        return 0;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_LastRebootReason") == 0)
    {
        syscfg_get(NULL, "X_RDKCENTRAL-COM_LastRebootReason", pValue, *pulSize);
        return 0;
    }
    if (strcmp(ParamName, "StringParam") == 0)
    {
        syscfg_get(NULL, "StringParam", pValue, *pulSize);
        return 0;
    }

#if !defined(_SR213_PRODUCT_REQ_) && !defined (_WNXL11BWL_PRODUCT_REQ_) && !defined (_SCER11BEL_PRODUCT_REQ_) && !defined (_SCXF11BFL_PRODUCT_REQ_)
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_InActiveFirmware") == 0)
    {
        return CosaDmlDiGetInActiveFirmware(NULL, pValue, pulSize);
    }
#endif

    if (strcmp(ParamName, "X_RDK_RDKProfileName") == 0)
    {
        return update_pValue(pValue, pulSize, "RDKB");
    }

    /* Changes for EMS begins here */

    if (strcmp(ParamName, "X_COMCAST-COM_EMS_MobileNumber") == 0)
    {
        return update_pValue(pValue, pulSize, pMyObject->EMS_MobileNo);
    }

    if (strcmp(ParamName, "X_COMCAST-COM_EMS_ServerURL") == 0)
    {
        syscfg_get(NULL, "ems_server_url", pValue, *pulSize);
        return 0;
    }

    if (strcmp(ParamName, "RouterName") == 0)
    {
        syscfg_get(NULL, "router_name", pValue, *pulSize);
        return 0;
    }

    /* Changes for EMS end here */

#if !defined(_COSA_BCM_MIPS_) && !defined(_ENABLE_DSL_SUPPORT_)

    /* Changes for RDKB-6560 */

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CMTS_MAC") == 0)
    {
        CosaDmlDiGetCMTSMac(NULL, pValue, pulSize);
        return 0;
    }

    /* Changes for RDKB-6560 end */

#endif

    /* Changes for RDKB-5878 */

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudPersonalizationURL") == 0)
    {
        syscfg_get(NULL, "CloudPersonalizationURL", pMyObject->CloudPersonalizationURL, sizeof(pMyObject->CloudPersonalizationURL));
        return update_pValue(pValue, pulSize, pMyObject->CloudPersonalizationURL);
    }

    /* Changes for RDKB-5878 end */

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_OnBoarding_State") == 0)
    {
        if (access("/nvram/.device_onboarded", F_OK) == 0)
        {
            return update_pValue(pValue, pulSize, "OnBoarded");
        }

        return update_pValue(pValue, pulSize, "NONE");
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_UI_ACCESS") == 0)
    {
        /* not implemented */
        return -1;
    }

    ReturnValue =
        DeviceInfo_GetParamStringValue_Custom
            (
                hInsContext,
                ParamName,
                pValue,
                pulSize
            );

    if ( ReturnValue == 0 )
    {
        return ReturnValue;
    }
    else
    {
        /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
        return -1;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamBoolValue
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
DeviceInfo_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    BOOL                            bReturnValue;
#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
    id =getuid();    
#endif
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "ClearResetCount") == 0)
    {
        /* collect value */
        CosaDmlDiClearResetCount(NULL,bValue);
        return TRUE;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_AkerEnable") == 0)
    {
        if (syscfg_set_commit(NULL, "X_RDKCENTRAL-COM_AkerEnable", bValue ? "true" : "false") != 0)
        {
            AnscTraceWarning(("syscfg_set failed for AkerEnable\n"));
        }
        else
        {
		/* Restart Firewall */
		v_secure_system("sysevent set firewall-restart");
#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
               if(id!=0)
               {
                       char *lxcevt = "sysevent set firewall-restart";
                       send(sock , lxcevt , strlen(lxcevt) , 0 );
               }
#endif
        }

        return TRUE;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_OnBoarding_DeleteLogs") == 0)
    {
        if(bValue)
        {
            v_secure_system("/rdklogger/onboardLogUpload.sh delete &");
        }
        return TRUE;
    }

    if (strcmp(ParamName, "CustomDataModelEnabled") == 0)
    {
        pMyObject->CustomDataModelEnabled = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "BoolParam") == 0)
    {
        pMyObject->RollBackParam = bValue;
        return TRUE;
    }

    bReturnValue =
        DeviceInfo_SetParamBoolValue_Custom
            (
                hInsContext,
                ParamName,
                bValue
            );

    if ( bReturnValue )
    {
        return bReturnValue;
    }
    else
    {
        /*CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));*/
        return FALSE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamIntValue
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
DeviceInfo_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "IntParam") == 0)
    {
        PCOSA_DATAMODEL_DEVICEINFO   pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        char buf[16]={0};
        errno_t rc = -1;
        rc = sprintf_s(buf, sizeof(buf), "%d", iValue);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        if (syscfg_set_commit(NULL, "IntParam", buf) != 0)
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        } 
        else
        {
            pMyObject->RollBackIntParam = iValue;
        }
        return TRUE;
    }
    /* check the parameter name and set the corresponding value */
#ifndef FEATURE_FWUPGRADE_MANAGER
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadAndFactoryReset") == 0)
    {
        if( iValue == 1)
        {
            if(ANSC_STATUS_SUCCESS != CosaDmlDiSetFirmwareDownloadAndFactoryReset())
            {
                return FALSE;
            }
        }
	return TRUE;
    }
#else
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
#endif
	/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamUlongValue
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
DeviceInfo_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_ConfigureDocsicPollTime") == 0)
    {
        /* collect value */
           FILE *fp;
           char buff[30];
	   snprintf(buff,sizeof(buff),"%d",(int)uValue);

           fp = fopen("/nvram/docsispolltime.txt", "w+");
           if(!fp)
           {
                 CcspTraceError(("%s falied to open /nvram/docsispolltime.txt file \n",__FUNCTION__));
                 return FALSE;
           }
	   fprintf(fp, "%s\n", buff);
 	   fclose(fp);
	   return TRUE;
    } 
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**
 *  RFC Features TelemetryEndpoint
*/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TelemetryEndpoint_GetParamBoolValue
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
TelemetryEndpoint_GetParamBoolValue

(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL*                       pBool
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /* CID: 58662 Array compared against 0*/
        if(!syscfg_get( NULL, "TelemetryEndpointEnabled", buf, sizeof(buf)))
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;                }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TelemetryEndpoint_SetParamBoolValue
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
TelemetryEndpoint_SetParamBoolValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, TelemetryEndpoint_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit (NULL, "TelemetryEndpointEnabled", bValue ? "true" : "false") != 0)
        {
            CcspTraceError(("syscfg_set TelemetryEndpointEnabled failed\n"));
            return FALSE;
        }

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TelemetryEndpoint_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue
            );

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The string value buffer;


    return:     TRUE if succeeded;
                FALSE if not supported.

**********************************************************************/
ULONG
    TelemetryEndpoint_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    errno_t   rc   = -1;
    /* Required for xPC sync */
    if (strcmp(ParamName, "URL") == 0)
    {
        /* collect value */
        char buf[64];
        memset(buf, 0 ,sizeof(buf));
        /* CID: 70225 Array compared against 0*/
        /* CID: 56488 Logically dead code*/
        if(!syscfg_get( NULL, "TelemetryEndpointURL", buf, sizeof(buf)))
        {
            rc = strcpy_s(pValue, *pUlSize, buf);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        return -1;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TelemetryEndpoint_SetParamBoolValue
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
    TelemetryEndpoint_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
 )
{
    if (IsStringSame(hInsContext, ParamName, pString, Telemetry_GetParamStringValue))
        return TRUE;

    /* Required for xPC sync */
    if (strcmp(ParamName, "URL") == 0)
    {
        if (syscfg_set_commit(NULL, "TelemetryEndpointURL", pString) != 0)
        {
            CcspTraceError(("syscfg_set failed\n"));

        }
        else
        {
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
        AccountInfo_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The buffer of returned string value;

                ULONG*                      pulSize
                The buffer of returned string size;

    return:     TRUE if succeeded.

**********************************************************************/
#define ACCOUNT_ID_SIZE 32

ULONG
AccountInfo_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    errno_t  rc = -1;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "AccountID") == 0)
    {
        /* collect value */
           char buff[ACCOUNT_ID_SIZE]={'\0'};
           /* CID: 64641 Array compared against 0*/
           /*CID: 67589 Logically dead code*/
           if(!syscfg_get( NULL, "AccountID", buff, sizeof(buff)))
           {
                rc = strcpy_s(pValue, *pulSize, buff);
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return -1;
                }
                return 0;
           }
           return -1;
    }

    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AccountInfo_SetParamStringValue
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
AccountInfo_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    BOOL bReturnValue = FALSE;
    
    if (IsStringSame(hInsContext, ParamName, pString, AccountInfo_GetParamStringValue))
        return TRUE;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "AccountID") == 0)
    {
        /* collect value */
           char buff[ACCOUNT_ID_SIZE]={'\0'};
           int idlen = strlen(pString)-1;
           int i;
           /* CID: 66168 Out-of-bounds read - protect buff read in else check for idlen val 32*/
           if ( idlen >= ACCOUNT_ID_SIZE )
           {
                CcspTraceError(("%s Account ID size is too large %d characters..!! \n",__FUNCTION__, idlen));
                bReturnValue = FALSE;
           }
           else
           {
                snprintf(buff,sizeof(buff),"%s",pString);

                for(i=0; i < idlen; ++i)
                {
                   if(isalnum(buff[i]) == 0 && buff[i] != '_' && buff[i] != '-')
                   {
                       CcspTraceError(("[%s] Account ID is not alphanumeric contains special characters...!!\n",__FUNCTION__));
                       bReturnValue = FALSE;
                       break;
                   }
                   else
                   {
                       bReturnValue = TRUE;
                   }
               }
               if(bReturnValue == TRUE)
               {
                    CcspTraceInfo(("[%s] Account ID is alphanumeric...!!", __FUNCTION__));
                    if (syscfg_set_commit(NULL, "AccountID", pString) != 0)
                    {
                        CcspTraceError(("%s:syscfg_set failed for AccountID \n", __FUNCTION__));
                        bReturnValue = FALSE;
                    }
                }
           }
    }
    else
    {
           CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
           bReturnValue = FALSE;
    }

    return bReturnValue;
}

static BOOL valid_url (char *buff)
{
    int i=0;
    char *token=NULL;
    char *delim=".";
    char *str=NULL;
    int len=_ansc_strlen(buff);
    int count=0;

    while(buff[i] != '\0')
    {
        //Allowing only integers, alphabets(lower and upper) and certain special characters
        if(((buff[i] >= '-') && (buff[i] <= ':')) || ((buff[i]>='A') && (buff[i]<='Z')) || ((buff[i]>='a') && (buff[i]<='z')) || (buff[i]=='#') || (buff[i]=='@') || (buff[i]=='~'))
            i++;
        else
            return FALSE;
    }    
    if((strncasecmp(buff,"http://",_ansc_strlen("http://"))!=0) && (strncasecmp(buff,"https://",_ansc_strlen("https://"))!=0))
        return FALSE;

    if(buff[len-1] == '.')
        return FALSE;

    str = strdup(buff);
    if (! str) {
        return FALSE;
    }

    token=strtok(str,delim);
    while(token!=NULL)
    {
        count++;
        token=strtok(NULL,delim);
    }
    if(count<2)
    {
        AnscFreeMemory(str);
        return FALSE;
    }
    AnscFreeMemory(str);

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamStringValue
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
DeviceInfo_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    ANSC_STATUS ret=ANSC_STATUS_FAILURE;
    errno_t rc =-1;
    int ind =-1; 
    /* check the parameter name and set the corresponding value */
    rc = strcmp_s("ProvisioningCode", strlen("ProvisioningCode"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* save update to backup */
         rc = STRCPY_S_NOCLOBBER((char *)pMyObject->ProvisioningCode,sizeof(pMyObject->ProvisioningCode), pString);
         if(rc != EOK)
         {
              ERR_CHK(rc);
               return FALSE;
          }
        return TRUE;
    }
#ifdef CONFIG_INTERNET2P0
    rc = strcmp_s( "X_RDKCENTRAL-COM_CloudUIWebURL",strlen("X_RDKCENTRAL-COM_CloudUIWebURL"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
       
      
        char wrapped_inputparam[256]={0};
	   ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	  if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;
         /* input string size check to avoid truncated data update on database  */   
         if(sizeof(wrapped_inputparam ) < sizeof(pMyObject->WebURL))
        {    
	
    	 if (syscfg_set_commit(NULL, "redirection_url", wrapped_inputparam) != 0) {
             AnscTraceWarning(("syscfg_set failed\n"));
          } else {
		 v_secure_system("/etc/whitelist.sh %s", wrapped_inputparam);
		 rc =STRCPY_S_NOCLOBBER(pMyObject->WebURL,sizeof(pMyObject->WebURL), wrapped_inputparam);
                if(rc != EOK)
               {
                 ERR_CHK(rc);
                 return FALSE;
               }
		CcspTraceWarning(("CaptivePortal:Cloud URL is changed, new URL is %s ...\n",pMyObject->WebURL));
             }

	    return TRUE;
       } 
       
       else
       {
           return FALSE;
       }    

    }
#endif
#define BUFF_SIZE 2048
    int len=0;
    /*Changes for 5878 start*/
    
    if(pString == NULL)
    return FALSE;
    
    rc = strcmp_s("X_RDKCENTRAL-COM_CloudPersonalizationURL",strlen( "X_RDKCENTRAL-COM_CloudPersonalizationURL"),ParamName,&ind);
    ERR_CHK(rc);
    len=_ansc_strlen(pString);
    if (len > BUFF_SIZE) {
        return FALSE;
    }
    if((!ind) && (rc == EOK))
    {    
        /* input string size check to avoid truncated data on database  */
        if((valid_url(pString)) && (strlen(pString) < (sizeof(pMyObject->CloudPersonalizationURL))))
        {	
		   if (syscfg_set_commit(NULL, "CloudPersonalizationURL", pString) != 0)
		  {
	        AnscTraceWarning(("syscfg_set failed\n"));
	      } 
		 else
		 {
			rc = STRCPY_S_NOCLOBBER(pMyObject->CloudPersonalizationURL,sizeof(pMyObject->CloudPersonalizationURL), pString);
                        if(rc != EOK)
                        {
                          ERR_CHK(rc);
                          return FALSE;
                        }
			CcspTraceWarning(("CloudPersonalizationURL  URL is changed, new URL is %s ...\n",pMyObject->CloudPersonalizationURL));
	     }
         

	  	 return TRUE;
        } 
        else
       {
           return FALSE;
       }

 }
	/*Changes for 5878 end*/
   enum pString_val type;
   rc = strcmp_s( "X_RDKCENTRAL-COM_UI_ACCESS",strlen("X_RDKCENTRAL-COM_UI_ACCESS"),ParamName, &ind);
   ERR_CHK(rc);
   if((!ind) && (rc == EOK))

   {

     /* helper function to make code more readable by removing multiple if else */
     if(get_deviceinfo_from_name(pString, &type))  
     {    
           if (type == UIACCESS)
           {
		 CcspTraceInfo(("Local UI Access : RDKB_LOCAL_UI_ACCESS\n"));
           }
           else if (type ==  UISUCCESS)
           {
		CcspTraceInfo(("Local UI Access : RDKB_LOCAL_UI_SUCCESS\n"));
                CcspTraceInfo(("WebUi admin login success\n"));
           }
           else if(type == UIFAILED)
           {
         	CcspTraceInfo(("Local UI Access : RDKB_LOCAL_UI_FAILED\n"));
                CcspTraceInfo(("WebUi admin login failed\n"));
           }
	   else if(type == REBOOTDEVICE)
           {
                CcspTraceInfo(("RDKB_REBOOT : RebootDevice triggered from GUI\n"));
                OnboardLog("RDKB_REBOOT : RebootDevice triggered from GUI\n");
		 
                #if defined (_ARRIS_XB6_PRODUCT_REQ_) //ARRISXB6-7328, ARRISXB6-7332
                ARRIS_RESET_REASON("RDKB_REBOOT : RebootDevice triggered from GUI\n");
                #endif

		char buffer[8] = {0};
		syscfg_get( NULL, "restore_reboot", buffer, sizeof(buffer));

		rc = strcmp_s( "true",strlen("true"),buffer,&ind);
                ERR_CHK(rc);
                if((rc == EOK) && (ind))
		{
			if (syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootReason", "gui-reboot") != 0)
			{
				AnscTraceWarning(("RDKB_REBOOT : RebootDevice syscfg_set failed GUI\n"));
			}
	        
			if (syscfg_set_commit(NULL, "X_RDKCENTRAL-COM_LastRebootCounter", "1") != 0)
			{
				AnscTraceWarning(("syscfg_set failed\n"));
			}
	        }
		else
		{
			CcspTraceInfo(("RDKB_REBOOT : RebootDevice to restore configuration\n"));
		}
	 }
         else if(type == FACTORYRESET)
         {
               CcspTraceInfo(("RDKB_REBOOT : Reboot Device triggered through Factory reset from GUI\n"));
               OnboardLog("RDKB_REBOOT : Reboot Device triggered through Factory reset from GUI\n");
			 
               #if defined (_ARRIS_XB6_PRODUCT_REQ_) //ARRISXB6-7328, ARRISXB6-7332
               ARRIS_RESET_REASON("RDKB_REBOOT : Reboot Device triggered through Factory reset from GUI\n");
               #endif
         }
	 else if(type == CAPTIVEPORTALFAILURE) {

	 	CcspTraceInfo(("Local UI Access : Out of Captive Poratl, Captive Portal is disabled\n"));
	 }
      }
      else
      {
	        CcspTraceInfo(("Local UI Access : Unsupported value\n"));
      }
         return TRUE;
   }

   /* Changes for EMS */
    rc = strcmp_s("X_COMCAST-COM_EMS_ServerURL", strlen("X_COMCAST-COM_EMS_ServerURL"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
   
    {
       
        char wrapped_inputparam[256]={0};
	    ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	    if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;
	if(sizeof( wrapped_inputparam ) < sizeof(pMyObject->EMS_ServerURL) )
       {
	    if (syscfg_set_commit(NULL, "ems_server_url", wrapped_inputparam) != 0) {
             AnscTraceWarning(("syscfg_set failed\n"));
          } else {
		 char ems_url[150];	
		 rc = sprintf_s(ems_url,sizeof(ems_url),"/etc/whitelist.sh %s",wrapped_inputparam);
                if(rc < EOK)
               {
                   ERR_CHK(rc);
                   return FALSE;
               }

                v_secure_system("/etc/whitelist.sh %s",wrapped_inputparam);
		rc = STRCPY_S_NOCLOBBER(pMyObject->EMS_ServerURL,sizeof(pMyObject->EMS_ServerURL), wrapped_inputparam);
		if(rc != EOK)
         {
              ERR_CHK(rc);
               return FALSE;
         }
             }

	return TRUE;
      }
      
      else
     {
         return FALSE;
     }

    }
    
    rc = strcmp_s("X_RDKCENTRAL-COM_LastRebootReason", strlen("X_RDKCENTRAL-COM_LastRebootReason"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
        {
		OnboardLog("Device reboot due to reason %s\n", pString);
                if (syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootReason", pString) != 0)
	            {
			        AnscTraceWarning(("syscfg_set failed for Reason and counter \n"));
			    }
		        if (syscfg_set_commit(NULL, "X_RDKCENTRAL-COM_LastRebootCounter", "1") != 0)
	            {
                      AnscTraceWarning(("syscfg_set failed\n"));
                }
	    return TRUE;
				
        }

    rc = strcmp_s("StringParam", strlen("StringParam"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
        {
            OnboardLog("RollBack_StringParam %s\n", pString);
            char *p= strchr(pString, 'D');
            if(p != NULL)
            {
                if (syscfg_set(NULL, "StringParam", pString) != 0)
	            {
			        AnscTraceWarning(("syscfg_set failed for RollBack_StringParam \n"));
                    }
	        return TRUE;
            }
            else
                return FALSE;
        }
    
    rc = strcmp_s("X_COMCAST-COM_EMS_MobileNumber", strlen("X_COMCAST-COM_EMS_MobileNumber"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pMyObject->EMS_MobileNo,sizeof(pMyObject->EMS_MobileNo), pString);
	if(rc != EOK)
         {
              ERR_CHK(rc);
              return FALSE;
         }
        return TRUE;
		
    }

    rc = strcmp_s("RouterName", strlen("RouterName"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
      if (syscfg_set_commit(NULL, "router_name", pString) != 0)
      {
	AnscTraceWarning(("syscfg_set failed for RouterName\n"));
      } 
      else
      {
	CcspTraceWarning(("RouterName is changed, new RouterName: %s ...\n", pString));
      }
      return TRUE;

    }
	
/* Changes end here */
	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_Validate
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
DeviceInfo_Validate
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
        DeviceInfo_Commit
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
DeviceInfo_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    CosaDmlDiSetProvisioningCode(NULL, (char *)pMyObject->ProvisioningCode);

    //RDKB-33819: TR069 restart is not required for BWG platforms
#if !defined _CBR_PRODUCT_REQ_ && !defined _BWG_PRODUCT_REQ_
    if(pMyObject->CustomDataModelEnabled)
    {
        CosaDmlGiSetCustomDataModelEnabled(NULL, pMyObject->CustomDataModelEnabled);
    }
#endif

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DeviceInfo_Rollback
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
DeviceInfo_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    ULONG pulSize = 0;
    /*CID:78739 Out-of-bounds access - updted the ProvisioningCode with 256 in the declaration*/
    CosaDmlDiGetProvisioningCode(NULL,(char *)pMyObject->ProvisioningCode, &pulSize);
    
    return 0;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_WIFI_TELEMETRY.{i}.

    *  WiFi_Telemetry_SetParamIntValue
    *  WiFi_Telemetry_SetParamStringValue
    *  WiFi_Telemetry_GetParamIntValue
    *  WiFi_Telemetry_GetParamStringValue
***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFi_Telemetry_SetParamIntValue
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
WiFi_Telemetry_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    errno_t                         rc        = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "LogInterval") == 0)
    {
        char str[12];
        int retPsmGet = CCSP_SUCCESS;

        /* Updating the LogInterval  in PSM database  */
        rc = sprintf_s(str, sizeof(str),"%d",iValue);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.LogInterval", ccsp_string, str);
        if (retPsmGet != CCSP_SUCCESS) {
        CcspTraceError(("Set failed for LogInterval Support \n"));
        return FALSE;
        }
        CcspTraceInfo(("Successfully set  LogInterval in PSM \n"));

#ifdef RDK_ONEWIFI
        parameterValStruct_t pVal[1];
        char                 paramName[256] = "Device.WiFi.WHIX_LogInterval";
        char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
        char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
        char*                faultParam     = NULL;
        int                  ret            = 0;
        CCSP_MESSAGE_BUS_INFO *bus_info     = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

        pVal[0].parameterName  = paramName;
        pVal[0].parameterValue = str;
        pVal[0].type           = ccsp_int;

        ret = CcspBaseIf_setParameterValues(
                  bus_handle,
                  compName,
                  dbusPath,
                  0,
                  0,
                  pVal,
                  1,
                  TRUE,
                  &faultParam
              );

        if (ret != CCSP_SUCCESS)
        {
            CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
            bus_info->freefunc(faultParam);
            return FALSE;
        }
#endif
        /* save update to backup */
        pMyObject->WiFi_Telemetry.LogInterval = iValue;
        return TRUE;
    }

    if (strcmp(ParamName, "ChUtilityLogInterval") == 0)
    {
        char str[12];
        int retPsmGet = CCSP_SUCCESS;

        /* Updating the ChUtilityLogInterval  in PSM database  */
        sprintf(str,"%d",iValue);
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.ChUtilityLogInterval", ccsp_string, str);
        if (retPsmGet != CCSP_SUCCESS) {
        CcspTraceError(("Set failed for LogInterval Support \n"));
        return FALSE;
        }
        CcspTraceInfo(("Successfully set  LogInterval in PSM \n"));


#ifdef RDK_ONEWIFI
        parameterValStruct_t pVal[1];
        char                 paramName[256] = "Device.WiFi.WHIX_ChUtility_LogInterval";
        char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
        char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
        char*                faultParam     = NULL;
        int                  ret            = 0;
        CCSP_MESSAGE_BUS_INFO *bus_info     = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

        pVal[0].parameterName  = paramName;
        pVal[0].parameterValue = str;
        pVal[0].type           = ccsp_int;

        ret = CcspBaseIf_setParameterValues(
                  bus_handle,
                  compName,
                  dbusPath,
                  0,
                  0,
                  pVal,
                  1,
                  TRUE,
                  &faultParam
              );

        if (ret != CCSP_SUCCESS)
        {
            CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
            bus_info->freefunc(faultParam);
            return FALSE;
        }
#endif
        /* save update to backup */
        pMyObject->WiFi_Telemetry.ChUtilityLogInterval = iValue;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFi_Telemetry_SetParamStringValue
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
WiFi_Telemetry_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    errno_t   rc = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "NormalizedRssiList") == 0)
    {
        int retPsmGet = CCSP_SUCCESS;

        /* Updating the NormalizedRssiList in PSM database  */
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.NormalizedRssiList", ccsp_string, pString);
        if (retPsmGet != CCSP_SUCCESS) {
        CcspTraceError(("Set failed for NormalizedRssiList Support \n"));
        return FALSE;
        }
        CcspTraceInfo(("Successfully set  NormalizedRssiList in PSM \n"));
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pMyObject->WiFi_Telemetry.NormalizedRssiList, sizeof(pMyObject->WiFi_Telemetry.NormalizedRssiList), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "CliStatList") == 0)
    {
        int retPsmGet = CCSP_SUCCESS;

        /* Updating the CliStatList in PSM database  */
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.CliStatList", ccsp_string, pString);
        if (retPsmGet != CCSP_SUCCESS) {
        CcspTraceError(("Set failed for CliStatList Support \n"));
        return FALSE;
        }
        CcspTraceInfo(("Successfully set  CliStatList in PSM \n"));
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( pMyObject->WiFi_Telemetry.CliStatList, sizeof(pMyObject->WiFi_Telemetry.CliStatList), pString );
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "TxRxRateList") == 0)
    {
        int retPsmGet = CCSP_SUCCESS;

        /* Updating the TxRxRateList  in PSM database  */
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.TxRxRateList", ccsp_string, pString);
        if (retPsmGet != CCSP_SUCCESS) {
        CcspTraceError(("Set failed for TxRxRateList Support \n"));
        return FALSE;
        }
        CcspTraceInfo(("Successfully set  TxRxRateList in PSM \n"));
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER( pMyObject->WiFi_Telemetry.TxRxRateList, sizeof(pMyObject->WiFi_Telemetry.TxRxRateList), pString );
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "SNRList") == 0)
    {
        int retPsmSet = CCSP_SUCCESS;
    
        retPsmSet = PSM_Set_Record_Value2( bus_handle, g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WIFI_TELEMETRY.SNRList", ccsp_string, pString );

        if (retPsmSet != CCSP_SUCCESS )
        {
            CcspTraceInfo(("Failed to set SNRList in PSM"));
            return FALSE;
        }
        
        CcspTraceInfo(("Successfully set SNRList in PSM \n"));
    
        rc = STRCPY_S_NOCLOBBER( pMyObject->WiFi_Telemetry.SNRList, sizeof(pMyObject->WiFi_Telemetry.SNRList), pString );
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFi_Telemetry_GetParamIntValue
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
WiFi_Telemetry_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "LogInterval") == 0)
    {
        *pInt =  pMyObject->WiFi_Telemetry.LogInterval;
        return TRUE;
    }

    if (strcmp(ParamName, "ChUtilityLogInterval") == 0)
    {
        *pInt =  pMyObject->WiFi_Telemetry.ChUtilityLogInterval;
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WiFi_Telemetry_GetParamStringValue
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
WiFi_Telemetry_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    errno_t   rc = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "NormalizedRssiList") == 0)
    {
        /* collect value */
        rc = strcpy_s( pValue, *pulSize, pMyObject->WiFi_Telemetry.NormalizedRssiList);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "CliStatList") == 0)
    {
        /* collect value */
        rc = strcpy_s( pValue, *pulSize, pMyObject->WiFi_Telemetry.CliStatList);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }


    if (strcmp(ParamName, "TxRxRateList") == 0)
    {
        /* collect value */
        rc = strcpy_s( pValue, *pulSize, pMyObject->WiFi_Telemetry.TxRxRateList);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "SNRList") == 0)
    {
        /* collect value */
        rc = strcpy_s( pValue, *pulSize, pMyObject->WiFi_Telemetry.SNRList);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }


    return -1;
}

/***********************************************************************

 APIs for Object:

   .X_RDKCENTRAL-COM_RFC.Feature.SecureDB

    * SecureDB_SetParamStringValue
***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype

        BOOL
        SecureDB_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue
    );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                        pValue
                The buffer of returned BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
SecureDB_SetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue
)
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "ClearUnencryptedData") == 0)
    {
      if (pValue && strstr(pValue, "bootstrap"))
      {
            unsigned int flags = 0;
            FILE *fp = fopen(CLEAR_TRACK_FILE, "r");

            if (fp)
            {
                fscanf(fp, "%u", &flags);
                fclose(fp);
            }

            if ((flags & NVRAM_BOOTSTRAP_CLEARED) != 0)
            {
                CcspTraceInfo(("bootstrap.json already cleared, skipping\n"));
                return TRUE;
            }

            v_secure_system("rm -rf %s", BOOTSTRAP_INFO_FILE_BACKUP);
            CcspTraceInfo(("Successfully removed bootstrap.json from nvram\n"));
            flags |= NVRAM_BOOTSTRAP_CLEARED;

            fp = fopen(CLEAR_TRACK_FILE, "w");
            if (fp)
            {
                fprintf(fp, "%u\n", flags);
                fclose(fp);
                CcspTraceInfo(("Updated ClearUnencryptedData_flags with NVRAM_BOOTSTRAP_CLEARED bit\n"));
            }
            else
            {
                CcspTraceError(("Failed to open ClearUnencryptedData_flags for writing\n"));
            }

            return TRUE;
      }
    }

    return FALSE;
}

/***********************************************************************

 APIs for Object:

   .X_RDKCENTRAL-COM_RFC.Feature.ImageHealthChecker

    * IHC_GetParamStringValue
    * IHC_SetParamStringValue
***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype

        BOOL
        IHC_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

    description:

        This function is called to retrieve BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                        pValue
                The buffer of returned BOOL value;

    return:     pValue if succeeded.

**********************************************************************/

 BOOL 
 IHC_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
 {
    UNREFERENCED_PARAMETER(hInsContext);
    errno_t rc = -1;
    int ind    = -1;
    rc = strcmp_s("OperatingMode", strlen("OperatingMode"), ParamName, &ind);
    ERR_CHK(rc);
    if((rc == EOK) && (!ind))
    {
        char buf[64]={0};
        syscfg_get( NULL, "IHC_Mode", buf, sizeof(buf));
        if( buf[0] != '\0' )
        {
             rc = strcpy_s(pValue, *pUlSize, buf);
             ERR_CHK(rc);
             return TRUE;
        }
    }
    return FALSE;
 }


BOOL
IHC_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    errno_t rc = -1;
    int ind    = -1;
    rc = strcmp_s("OperatingMode", strlen("OperatingMode"), ParamName, &ind);
    ERR_CHK(rc);
    if((rc == EOK) && (!ind))
    {
        if(strcmp_s("Self Heal", strlen("Self Heal"), strValue , &ind) == EOK && !ind)
        {
            CcspTraceWarning(("%s: %s Mode will be implemented in next phase. So please use Monitor or Disabled \n", __FUNCTION__, strValue));
            return FALSE;
        }
        else if( (strcmp_s("Disabled", strlen("Disabled"), strValue , &ind) == EOK && !ind) || (strcmp_s("Monitor", strlen("Monitor"), strValue , &ind) == EOK && !ind ) )
        {
            if (syscfg_set_commit(NULL, "IHC_Mode", strValue) != 0)
            {
                    CcspTraceWarning(("%s: syscfg_set failed for %s\n", __FUNCTION__, ParamName));
                    return FALSE;
            }
            return TRUE;
        }
        CcspTraceWarning(("%s: invalid Parameter value for IHC: %s \n", __FUNCTION__, strValue));
    }
    else
    {
        CcspTraceWarning(("%s: invalid Parameter for IHC: %s \n", __FUNCTION__, ParamName));
    }
    return FALSE;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.UniqueTelemetryId.

    *  UniqueTelemetryId_GetParamBoolValue
    *  UniqueTelemetryId_GetParamStringValue
    *  UniqueTelemetryId_GetParamIntValue
    *  UniqueTelemetryId_SetParamBoolValue
    *  UniqueTelemetryId_SetParamStringValue
    *  UniqueTelemetryId_SetParamIntValue
***********************************************************************/

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UniqueTelemetryId_GetParamBoolValue
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
UniqueTelemetryId_GetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL*						pBool
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;


    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pMyObject->UniqueTelemetryId.Enable;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UniqueTelemetryId_GetParamStringValue
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
UniqueTelemetryId_GetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pUlSize
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "TagString") == 0)
    {
        return  update_pValue(pValue,pUlSize, pMyObject->UniqueTelemetryId.TagString);
	    /* CID: 64836 Structurally dead code*/
    }

    return -1;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        UniqueTelemetryId_GetParamIntValue
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
UniqueTelemetryId_GetParamIntValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		int*						pInt
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	
	/* check the parameter name and return the corresponding value */
	if (strcmp(ParamName, "TimingInterval") == 0)
	{
		*pInt =  pMyObject->UniqueTelemetryId.TimingInterval;
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UniqueTelemetryId_SetParamBoolValue
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
UniqueTelemetryId_SetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL						bValue
	)
{
    PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    char buf[8] = {0};
    errno_t rc  = -1;

    if (IsBoolSame(hInsContext, ParamName, bValue, UniqueTelemetryId_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        rc = strcpy_s(buf, sizeof(buf), (bValue ? "true" : "false"));
        if(rc != EOK) {
           ERR_CHK(rc);
           return FALSE;
        }

        if (syscfg_set_commit(NULL, "unique_telemetry_enable", buf) != 0)
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        } 
        else
        {
            pMyObject->UniqueTelemetryId.Enable = bValue;
        }

	UniqueTelemetryCronJob(pMyObject->UniqueTelemetryId.Enable, pMyObject->UniqueTelemetryId.TimingInterval, pMyObject->UniqueTelemetryId.TagString);

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        UniqueTelemetryId_SetParamStringValue
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
UniqueTelemetryId_SetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						strValue
	)
{
    PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    errno_t   rc  =  -1;

    if (IsStringSame(hInsContext, ParamName, strValue, UniqueTelemetryId_GetParamStringValue))
        return TRUE;

    if (strcmp(ParamName, "TagString") == 0)
    {

        if (syscfg_set_commit(NULL, "unique_telemetry_tag", strValue) != 0)
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        }
        else
        {
            rc = STRCPY_S_NOCLOBBER(pMyObject->UniqueTelemetryId.TagString, sizeof(pMyObject->UniqueTelemetryId.TagString),strValue);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        UniqueTelemetryId_SetParamIntValue
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
UniqueTelemetryId_SetParamIntValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		int 						value
	)
{
    PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    char buf[16]={0};
    errno_t rc = -1;

    if (IsIntSame(hInsContext, ParamName, value, UniqueTelemetryId_GetParamIntValue))
        return TRUE;

    if (strcmp(ParamName, "TimingInterval") == 0)
    {
        rc = sprintf_s(buf, sizeof(buf), "%d", value);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }

        if (syscfg_set_commit(NULL, "unique_telemetry_interval", buf) != 0)
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        } 
        else
        {
            pMyObject->UniqueTelemetryId.TimingInterval = value;
        }

	UniqueTelemetryCronJob(pMyObject->UniqueTelemetryId.Enable, pMyObject->UniqueTelemetryId.TimingInterval, pMyObject->UniqueTelemetryId.TagString);

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ManageableNotification_GetParamBoolValue
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
ManageableNotification_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];

    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /*CID: 67293 Array compared against 0*/
        if(!syscfg_get( NULL, "ManageableNotificationEnabled", buf, sizeof(buf)))
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
           return TRUE;
        } else
          return FALSE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ManageableNotification_SetParamBoolValue
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
ManageableNotification_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, ManageableNotification_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        syscfg_set_commit(NULL, "ManageableNotificationEnabled", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Snmpv3DHKickstart_GetParamBoolValue
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
Snmpv3DHKickstart_GetParamBoolValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL*                       pBool
 )
{
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_KICKSTART      pKickstart = (PCOSA_DATAMODEL_KICKSTART)&pMyObject->Kickstart;
    BOOL bRet = FALSE;
    /* check the parameter name and return the corresponding value */

    CcspTraceInfo(("Snmpv3DHKickstart_GetParamBoolValue: hInsContext = 0x%lx\n", (long unsigned)hInsContext));
    CcspTraceInfo(("Snmpv3DHKickstart_GetParamBoolValue: pKickstart = 0x%lx\n", (long unsigned)pKickstart));


    CcspTraceInfo(("Snmpv3DHKickstart_GetParamBoolValue: ParamName = %s\n", ParamName));

    if (strcmp(ParamName, "Enabled") == 0)
    {
        if( pKickstart != NULL )
        {
            *pBool = pKickstart->Enabled;
            bRet = TRUE;
        }
    }
    else if (strcmp(ParamName, "RFCUpdateDone") == 0)
    {
        if( pKickstart != NULL )
        {
            *pBool = FALSE;     // always FALSE as it's just an indicator whaen set that RFC Post Process has completed
            bRet = TRUE;
        }
    }

    if( bRet == TRUE )
    {
        CcspTraceInfo(("Snmpv3DHKickstart_GetParamBoolValue: %s = %s\n", ParamName, *pBool == TRUE ? "TRUE" : "FALSE"));
    }
    else
    {
        CcspTraceInfo(("Snmpv3DHKickstart_GetParamBoolValue: FAILED Unknown parameter, %s\n", ParamName));
    }
    return bRet;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Snmpv3DHKickstart_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       bValue
            );

    description:

        This function is called to set Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       bValue
                The buffer with updated value;

    return:     TRUE if succeeded.

**********************************************************************/
//#define LOG_KICKSTART_VALUES

static int hexstring2bin( unsigned char * pOut, char *pIn, int numbytes );
static int bin2hexstring( char *pOut, unsigned char *pIn, int numbytes );

#ifdef LOG_KICKSTART_VALUES
static void PrintBinStream( char *pName, unsigned char *pBin, int len );
#endif


BOOL
Snmpv3DHKickstart_SetParamBoolValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, Snmpv3DHKickstart_GetParamBoolValue))
        return TRUE;

    snmpv3_kickstart_table_t        Snmpv3_Kickstart_Table;
    snmp_kickstart_row_t            Snmp_Kickstart_Row[MAX_KICKSTART_ROWS], *pSnmp_Kickstart_Row;
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_KICKSTART       pKickstart = (PCOSA_DATAMODEL_KICKSTART)&pMyObject->Kickstart;
    PCOSA_DATAMODEL_KICKSTARTTABLE  pKickstartTable;
    int i;
    BOOL bRet = FALSE;

    CcspTraceInfo(("Snmpv3DHKickstart_SetParamBoolValue: hInsContext = 0x%lx\n", (long unsigned)hInsContext));
    CcspTraceInfo(("Snmpv3DHKickstart_SetParamBoolValue: pKickstart = 0x%lx\n", (long unsigned)pKickstart));
    CcspTraceInfo(("Snmpv3DHKickstart_SetParamBoolValue: ParamName = %s, bValue = %d\n", ParamName, (int)bValue));

    /*CID: 128676 Uninitialized pointer read*/
    Snmpv3_Kickstart_Table.n_rows = 0;

    if (strcmp(ParamName, "Enabled") == 0)
    {
        if( pKickstart != NULL )
        {
            pKickstart->Enabled = bValue;
            bRet = TRUE;
        }
    }
    else if (strcmp(ParamName, "RFCUpdateDone") == 0)
    {
        if( pKickstart != NULL )
        {
            if( bValue == TRUE && pKickstart->TableUpdated == TRUE )
            {
                CcspTraceInfo(("Snmpv3DHKickstart_SetParamBoolValue: Updating Snmpv3_Kickstart_Table\n"));
                pKickstartTable = pKickstart->KickstartTable;
                Snmpv3_Kickstart_Table.n_rows = pKickstart->KickstartTotal;
                pSnmp_Kickstart_Row = Snmp_Kickstart_Row;
                for( i=0; i < Snmpv3_Kickstart_Table.n_rows && i < MAX_KICKSTART_ROWS; i++ )
                {
                    pSnmp_Kickstart_Row->security_name.length = (unsigned short)strlen( pKickstartTable->SecurityName );
                    pSnmp_Kickstart_Row->security_name.buffer = (unsigned char *)pKickstartTable->SecurityName;

                    pSnmp_Kickstart_Row->security_number.length = (unsigned short)pKickstartTable->SecurityNumberLen;
                    pSnmp_Kickstart_Row->security_number.buffer = (unsigned char *)pKickstartTable->SecurityNumber;

                    Snmpv3_Kickstart_Table.kickstart_values[i] = pSnmp_Kickstart_Row;
#ifdef LOG_KICKSTART_VALUES
                    PrintBinStream( Snmpv3_Kickstart_Table.kickstart_values[i]->security_name.buffer,
                                    Snmpv3_Kickstart_Table.kickstart_values[i]->security_number.buffer,
                                    Snmpv3_Kickstart_Table.kickstart_values[i]->security_number.length );
#endif
                    ++pKickstartTable;
                    ++pSnmp_Kickstart_Row;
                }
            }
            bRet = TRUE;
        }
    }

    if( bRet == TRUE )
    {
        CcspTraceInfo(("Snmpv3DHKickstart_SetParamBoolValue: successfully set %s = %s\n", ParamName, bValue == TRUE ? "TRUE" : "FALSE"));
        if( pKickstart->TableUpdated == TRUE && pKickstart->Enabled == TRUE )
        {
#if !defined(_XER5_PRODUCT_REQ_) && !defined(_SR213_PRODUCT_REQ_) && !defined(PON_GATEWAY)
	    i = cm_hal_snmpv3_kickstart_initialize( &Snmpv3_Kickstart_Table );
            CcspTraceError(("cm_hal_snmpv3_kickstart_initialize: return value = %d\n", i));
            pKickstart->TableUpdated = FALSE;
#endif
        }
    }
    else
    {
        CcspTraceError(("Snmpv3DHKickstart_SetParamBoolValue: FAILED Unknown Parameter, %s\n", ParamName));
    }

    return bRet;
}

static int hexstring2bin( unsigned char *pOut, char *pIn, int numbytes )
{
    int i, x;
    uint8_t val;

    for( i=0; i < numbytes; i++ )
    {
        for( x=0; x < 2; x++ )      // 2 nibbles per character
        {
            if( *pIn >= '0' && *pIn <= '9' )
            {
                val = *pIn - 0x30;
            }
            else if( *pIn >= 'a' && *pIn <= 'f' )
            {
                val = *pIn - 0x57;
            }
            else if( *pIn >= 'A' && *pIn <= 'F' )
            {
                val = *pIn - 0x37;
            }
            else
            {
                ++i;
                return i;   // abort, invalid character
            }

            if( !x )
            {
                *pOut = val << 4;   // upper nibble
            }
            else
            {
                *pOut += val;       // lower nibble
            }
            ++pIn;
        }
        ++pOut;
    }
    return i;
}

static int bin2hexstring( char *pOut, unsigned char *pIn, int numbytes )
{
    int i, x, workval;
    uint8_t val[2];

    for( i=0; i < numbytes; i++ )
    {
        val[0] = *pIn >> 4;
        val[1] = *pIn & 0x0f;
        for( x=0; x < 2; x++ )      // 2 nibbles per character
        {
            workval = val[x];
            /* CID: 114994 Unsigned compared against 0*/
            if(workval <= 0x09 )
            {
                *pOut = workval + 0x30;
            }
            else if( workval >= 0x0a && workval <= 0x0f )
            {
                *pOut = workval + 0x57;
            }
            ++pOut;
        }
        ++pIn;
    }

    *(pOut + i) = 0;
    return i;
}

#ifdef LOG_KICKSTART_VALUES
static void PrintBinStream( char *pName, unsigned char *pBin, int len )
{
    FILE *fp;
    unsigned char *pStart = pBin;
    int i;

    if( (fp=fopen( "/rdklogs/logs/KickstartLog.txt", "a" )) != NULL )
    {
        fprintf( fp, "Outputting name %s of %d bytes at address 0x%lx in (value is in hex)\n", pName, len, (unsigned long)pBin );
        for( i=0; i < len; i++ )
        {
            fprintf( fp, "%02x", *pBin );
            ++pBin;
        }
        fprintf( fp, "\n" );
        fclose( fp );
    }
    else
    {
        CcspTraceError(("PrintBinStream: FAILED can't open output file\n"));
    }

    pBin = pStart;
    if( (fp=fopen( "/rdklogs/logs/KickstartLog.bin", "a" )) != NULL )
    {
        for( i=0; i < len; i++ )
        {
            fprintf( fp, "%c", *pBin );
            ++pBin;
        }
        fprintf( fp, "%c%c%c%c", 0xff, 0xff, 0xff, 0xff );
        fclose( fp );
    }
    else
    {
        CcspTraceError(("PrintBinStream: FAILED can't open binary output file\n"));
    }
}
#endif

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Snmpv3DHKickstart_GetParamUlongValue
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
Snmpv3DHKickstart_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_KICKSTART      pKickstart = (PCOSA_DATAMODEL_KICKSTART)&pMyObject->Kickstart;
    BOOL bRet = FALSE;

    CcspTraceInfo(("Snmpv3DHKickstart_GetParamUlongValue: hInsContext = 0x%lx\n", (unsigned long)hInsContext));
    CcspTraceInfo(("Snmpv3DHKickstart_GetParamUlongValue: ParamName = %s\n", ParamName));
    if( pKickstart != NULL )
    {
        /* check the parameter name and return the corresponding value */
        if (strcmp(ParamName, "KickstartTotal") == 0)
        {
            *puLong = pKickstart->KickstartTotal;
            CcspTraceInfo(("Snmpv3DHKickstart_GetParamUlongValue: KickstartTotal = %lu\n", (unsigned long)pKickstart->KickstartTotal));
            bRet = TRUE;
        }
        else if (strcmp(ParamName, "TableNumberOfEntries") == 0)
        {
            *puLong = pKickstart->KickstartTotal;
            CcspTraceInfo(("Snmpv3DHKickstart_GetParamUlongValue: TableNumberOfEntries = %lu\n", (unsigned long)pKickstart->TableNumberOfEntries));
            bRet = TRUE;
        }
        else
        {
            AnscTraceWarning(("Snmpv3DHKickstart_GetParamUlongValue: Unsupported parameter '%s'\n", ParamName));
        }
    }
    
    return bRet;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Snmpv3DHKickstart_SetParamUlongValue
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
Snmpv3DHKickstart_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    if (IsUlongSame(hInsContext, ParamName, uValue, Snmpv3DHKickstart_GetParamUlongValue))
        return TRUE;

    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_KICKSTART      pKickstart = (PCOSA_DATAMODEL_KICKSTART)&pMyObject->Kickstart;
    BOOL bRet = FALSE;

    CcspTraceInfo(("Snmpv3DHKickstart_SetParamUlongValue: hInsContext = 0x%lx\n", (unsigned long)hInsContext));
    CcspTraceInfo(("Snmpv3DHKickstart_SetParamUlongValue: ParamName = %s, uValue = %ld\n", ParamName, uValue));

    if( pKickstart != NULL )
    {
        /* check the parameter name and return the corresponding value */
        if ((strcmp(ParamName, "KickstartTotal") == 0) && uValue <= MAX_KICKSTART_ROWS )
        {
            pKickstart->KickstartTotal = uValue;
            CcspTraceInfo(("Snmpv3DHKickstart_SetParamUlongValue: KickstartTotal = %lu\n", (unsigned long)pKickstart->KickstartTotal));
            bRet = TRUE;
        }
        else
        {
            AnscTraceWarning(("Snmpv3DHKickstart_SetParamUlongValue: Unsupported parameter '%s'\n", ParamName));
        }
    }

    return bRet;
}


/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Snmpv3DHKickstart.KickstartTable{i}.

    *  KickstartTable_GetEntryCount
    *  KickstartTable_GetEntry
    *  KickstartTable_GetParamStringValue
    *  KickstartTable_SetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        KickstartTable_GetEntryCount
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
KickstartTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_KICKSTART      pKickstart = (PCOSA_DATAMODEL_KICKSTART)&pMyObject->Kickstart;
    ULONG ulEntries = 0;

    CcspTraceInfo(("KickstartTable_GetEntryCount: hInsContext = 0x%lx\n", (long unsigned)hInsContext));
    CcspTraceInfo(("KickstartTable_GetEntryCount: pKickstart = 0x%lx\n", (long unsigned)pKickstart));

    if( pKickstart )
    {
        pKickstart->TableNumberOfEntries = MAX_KICKSTART_ROWS;
        CcspTraceInfo(("KickstartTable_GetEntryCount: TableNumberOfEntries = %d\n", pKickstart->TableNumberOfEntries));
        ulEntries = pKickstart->TableNumberOfEntries;
    }
    CcspTraceInfo(("KickstartTable_GetEntryCount: ulEntries = %lu\n", ulEntries));
    return ulEntries;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        KickstartTable_GetEntry
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
KickstartTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_KICKSTART      pKickstart = (PCOSA_DATAMODEL_KICKSTART)&pMyObject->Kickstart;
    PCOSA_DATAMODEL_KICKSTARTTABLE pKickstartTable = NULL;
    ANSC_HANDLE hEntry = NULL;

    *pInsNumber  = nIndex + 1;

    CcspTraceInfo(("KickstartTable_GetEntry: hInsContext = 0x%lx\n", (long unsigned)hInsContext));
    CcspTraceInfo(("KickstartTable_GetEntry: pKickstart = 0x%lx\n", (long unsigned)pKickstart));
    CcspTraceInfo(("KickstartTable_GetEntry: nIndex = %lu, *pInsNumber = %lu\n", nIndex, *pInsNumber));
    if( pKickstart )
    {
        CcspTraceInfo(("KickstartTable_GetEntry: TableNumberOfEntries = %u\n", pKickstart->TableNumberOfEntries));
        pKickstartTable = pKickstart->KickstartTable;
        CcspTraceInfo(("KickstartTable_GetEntry: pKickstartTable = 0x%lx\n", (long unsigned)pKickstartTable));

        if( pKickstartTable && nIndex < MAX_KICKSTART_ROWS )
        {
            hEntry = (ANSC_HANDLE)(pKickstartTable + nIndex);
            CcspTraceInfo(("KickstartTable_GetEntry: returning 0x%lx\n", (long unsigned)hEntry));
        }
    }
    return hEntry;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        LONG
        KickstartTable_GetParamStringValue
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
LONG
KickstartTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_KICKSTARTTABLE  pKickstartTable = (PCOSA_DATAMODEL_KICKSTARTTABLE)hInsContext;
    LONG lRet = -1;
    char *pPtr = NULL;

    CcspTraceInfo(("KickstartTable_GetParamStringValue: hInsContext = 0x%lx\n", (unsigned long)hInsContext));
    CcspTraceInfo(("KickstartTable_GetParamStringValue: ParamName = %s, *pUlSize = %ld\n", ParamName, *pUlSize));

    if( pKickstartTable != NULL )
    {
        /* check the parameter name and return the corresponding value */
        if (strcmp(ParamName, "SecurityName") == 0)
        {
            pPtr = pKickstartTable->SecurityName;
            if( AnscSizeOfString( pPtr ) < *pUlSize)
            {
                if( pValue != NULL )
                {
                    snprintf( pValue, (size_t)*pUlSize, "%s", pPtr );
                    lRet = 0;
                }
            }
            else
            {
                *pUlSize = AnscSizeOfString( pPtr )+1;
                lRet = 1;
            }
        }
        if (strcmp(ParamName, "SecurityNumber") == 0)
        {
            pPtr = (char *)pKickstartTable->SecurityNumber;
            if( ((pKickstartTable->SecurityNumberLen*2) + 1) < *pUlSize)    // 2 output characters per input byte plus NULL terminator
            {
                if( pValue != NULL )
                {
                    bin2hexstring( pValue, (unsigned char *) pPtr, pKickstartTable->SecurityNumberLen );
                    lRet = 0;
                }
            }
            else
            {
                *pUlSize = pKickstartTable->SecurityNumberLen + 1;
                lRet = 1;
            }
        }
    }

    if( lRet == -1 )
    {
        AnscTraceWarning(("KickstartTable_GetParamStringValue: Unsupported parameter '%s'\n", ParamName));
    }
    else if( lRet == 0 && pValue != NULL && ParamName != NULL )
    {
        CcspTraceInfo(("KickstartTable_GetParamStringValue: %s = %s\n", ParamName, pValue));
    }
    else
    {
        CcspTraceError(("KickstartTable_GetParamStringValue: %s, output is too small, need %lu bytes\n", ParamName, *pUlSize));
    }

    return lRet;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        KickstartTable_SetParamStringValue
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
KickstartTable_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    if (IsStringSame(hInsContext, ParamName, pString, (GETSTRING_FUNC_PTR)KickstartTable_GetParamStringValue))
        return TRUE;

    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_KICKSTART      pKickstart = (PCOSA_DATAMODEL_KICKSTART)&pMyObject->Kickstart;
    PCOSA_DATAMODEL_KICKSTARTTABLE  pKickstartTable = (PCOSA_DATAMODEL_KICKSTARTTABLE)hInsContext;
    BOOL bRet = FALSE;


    CcspTraceInfo(("KickstartTable_SetParamStringValue: ParamName = %s\n", ParamName));

    if( pKickstartTable != NULL )
    {
        /* check the parameter name and set the corresponding value */
        if (strcmp(ParamName, "SecurityName") == 0)
        {
            if( pString != NULL && AnscSizeOfString( pString ) < (sizeof(pKickstartTable->SecurityName) - 1) )
            {
                snprintf( pKickstartTable->SecurityName, sizeof(pKickstartTable->SecurityName), "%s", pString );
                bRet = TRUE;
            }
        }
        else if (strcmp(ParamName, "SecurityNumber") == 0)
        {
            if( ( pString != NULL) && ((AnscSizeOfString( pString )/2) <= sizeof(pKickstartTable->SecurityNumber)) )
            {
                pKickstartTable->SecurityNumberLen = strlen( pString )/2;   // 2 nibbles per character
                hexstring2bin( pKickstartTable->SecurityNumber, pString, pKickstartTable->SecurityNumberLen );
                bRet = TRUE;
            }
        }
        else
        {
            AnscTraceWarning(("KickstartTable_SetParamStringValue: Unsupported parameter '%s'\n", ParamName));
        }
    }

    if( bRet == TRUE && pKickstart != NULL )
    {
        pKickstart->TableUpdated = TRUE;        // something changed so initialize kickstart when RFC post process complete
    }

    return bRet;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TR069support_GetParamBoolValue
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
//TR069 support is removed for XB8 (RDKB-32781)
#ifndef _XB8_PRODUCT_REQ_
BOOL
TR069support_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];

    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /* CID: 71587 Array compared against 0*/
        if(!syscfg_get( NULL, "EnableTR69Binary", buf, sizeof(buf)))
        {
            if (strcmp(buf, "false") == 0)
                *pBool = FALSE;
            else
                *pBool = TRUE;
        }
        else {
           //Returning as TRUE which was the behaviour prior to the coverity fix CID: 71587
           *pBool = TRUE;
           return FALSE;
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TR069support_SetParamBoolValue
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
TR069support_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, TR069support_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        if( bValue == FALSE)
        {
            AnscTraceWarning(("Disabling Tr069 from RFC \n"));
            v_secure_system("/usr/ccsp/pam/launch_tr69.sh disable &");
        }
        else
        {
            AnscTraceWarning(("Enabling Tr069 from RFC \n"));
            v_secure_system("/usr/ccsp/pam/launch_tr69.sh enable &");
        }
		
        return TRUE;
    }

    return FALSE;
}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        newNTP_GetParamBoolValue
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
newNTP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];

    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /*CID:54377 Array compared against 0*/
        if(!syscfg_get( NULL, "new_ntp_enabled", buf, sizeof(buf))) 
        {
            if (strcmp(buf, "false") == 0)
                *pBool = FALSE;
            else
                *pBool = TRUE;
        }
        return TRUE;
    }

    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        newNTP_SetParamBoolValue
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
newNTP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, newNTP_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if( ANSC_STATUS_SUCCESS != CosaDmlSetnewNTPEnable(bValue))
	    return FALSE;
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MACsecRequired_GetParamBoolValue
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
#ifdef _MACSEC_SUPPORT_
BOOL
MACsecRequired_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if ( RETURN_ERR == platform_hal_GetMACsecEnable( ETHWAN_DEF_INTF_NUM, pBool )) {
            return FALSE;
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MACsecRequired_SetParamBoolValue
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
MACsecRequired_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, MACsecRequired_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if ( RETURN_ERR == platform_hal_SetMACsecEnable( ETHWAN_DEF_INTF_NUM, bValue )) {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}
#endif

#if !defined (RESOURCE_OPTIMIZATION)

/***********************************************************************

 APIs for Object:

    DeviceInfo.VendorConfigFile.{i}.

    *  VendorConfigFile_GetEntryCount
    *  VendorConfigFile_GetEntry
    *  VendorConfigFile_GetParamBoolValue
    *  VendorConfigFile_GetParamIntValue
    *  VendorConfigFile_GetParamUlongValue
    *  VendorConfigFile_GetParamStringValue
    *  VendorConfigFile_SetParamBoolValue
    *  VendorConfigFile_SetParamIntValue
    *  VendorConfigFile_SetParamUlongValue
    *  VendorConfigFile_SetParamStringValue
    *  VendorConfigFile_Validate
    *  VendorConfigFile_Commit
    *  VendorConfigFile_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VendorConfigFile_GetEntryCount
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
VendorConfigFile_GetEntryCount
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

        ANSC_HANDLE
        VendorConfigFile_GetEntry
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
VendorConfigFile_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    *pInsNumber  = nIndex + 1; 
    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_GetParamBoolValue
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
VendorConfigFile_GetParamBoolValue
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
        VendorConfigFile_GetParamIntValue
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
VendorConfigFile_GetParamIntValue
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
        VendorConfigFile_GetParamUlongValue
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
VendorConfigFile_GetParamUlongValue
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
        VendorConfigFile_GetParamStringValue
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
VendorConfigFile_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "Name") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "Version") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "Date") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "Description") == 0)
    {
        /* collect value */
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_SetParamBoolValue
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
VendorConfigFile_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_SetParamIntValue
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
VendorConfigFile_SetParamIntValue
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
        VendorConfigFile_SetParamUlongValue
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
VendorConfigFile_SetParamUlongValue
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
        VendorConfigFile_SetParamStringValue
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
VendorConfigFile_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pString);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_Validate
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
VendorConfigFile_Validate
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
        VendorConfigFile_Commit
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
VendorConfigFile_Commit
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
        VendorConfigFile_Rollback
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
VendorConfigFile_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
}

#endif

/***********************************************************************

 APIs for Object:

    DeviceInfo.SupportedDataModel.{i}.

    *  SupportedDataModel_GetEntryCount
    *  SupportedDataModel_GetEntry
    *  SupportedDataModel_GetParamBoolValue
    *  SupportedDataModel_GetParamIntValue
    *  SupportedDataModel_GetParamUlongValue
    *  SupportedDataModel_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SupportedDataModel_GetEntryCount
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
SupportedDataModel_GetEntryCount
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

        ANSC_HANDLE
        SupportedDataModel_GetEntry
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
SupportedDataModel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    *pInsNumber  = nIndex + 1; 
    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SupportedDataModel_GetParamBoolValue
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
SupportedDataModel_GetParamBoolValue
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
        SupportedDataModel_GetParamIntValue
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
SupportedDataModel_GetParamIntValue
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
        SupportedDataModel_GetParamUlongValue
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
SupportedDataModel_GetParamUlongValue
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
        SupportedDataModel_GetParamStringValue
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
SupportedDataModel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "URL") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "URN") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "Features") == 0)
    {
        /* collect value */
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.MemoryStatus.

    *  MemoryStatus_GetParamBoolValue
    *  MemoryStatus_GetParamIntValue
    *  MemoryStatus_GetParamUlongValue
    *  MemoryStatus_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MemoryStatus_GetParamBoolValue
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
MemoryStatus_GetParamBoolValue
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
        MemoryStatus_GetParamIntValue
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
MemoryStatus_GetParamIntValue
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
        MemoryStatus_GetParamUlongValue
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
MemoryStatus_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Total") == 0)
    {
         /* collect value */
        *puLong = COSADmlGetMemoryStatus(ParamName);
        return TRUE;
    }

    if (strcmp(ParamName, "Free") == 0)
    {
        /* collect value */
        *puLong = COSADmlGetMemoryStatus(ParamName);
        return TRUE;
    }


    if (strcmp(ParamName, "Used") == 0)
    {
        /* collect value */
        *puLong = COSADmlGetMemoryStatus(ParamName);
        return TRUE;
    }


    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FreeMemThreshold") == 0)
    {
        char buf[12];
        syscfg_get (NULL, "MinMemoryThreshold_Value", buf, sizeof(buf));
        *puLong = atoi(buf);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        MemoryStatus_GetParamStringValue
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
MemoryStatus_GetParamStringValue
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



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MemoryStatus_SetParamUlongValue
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
MemoryStatus_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FreeMemThreshold") == 0)
    {
        /* CID 339641 Unchecked return value : fix */
        if (syscfg_set_u_commit (NULL, "MinMemoryThreshold_Value", uValue) != 0) {
            return FALSE;
        }
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM.

    *  X_RDKCENTRAL-COM_GetParamBoolValue
    *  X_RDKCENTRAL-COM_GetParamIntValue
    *  X_RDKCENTRAL-COM_GetParamUlongValue
    *  X_RDKCENTRAL-COM_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL-COM_GetParamBoolValue
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
X_RDKCENTRAL_COM_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "DhcpServDetectEnable") == 0)
    {
        *pBool = pMyObject->bDhcpServDetectEnable;
        return TRUE;
    }

    if (strcmp(ParamName, "MultipleGW") == 0)
    {
        *pBool = pMyObject->bMultipleGW;
        return TRUE;
    }

    if (strcmp(ParamName, "InternetStatus") == 0)
    {
        *pBool = CosaDmlGetInternetStatus();
        return TRUE;
    }
	
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL-COM_GetParamIntValue
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
X_RDKCENTRAL_COM_GetParamIntValue
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
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL-COM_GetParamUlongValue
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
X_RDKCENTRAL_COM_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_RDKCENTRAL-COM_GetParamStringValue
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
X_RDKCENTRAL_COM_GetParamStringValue
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
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_SetParamBoolValue
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
X_RDKCENTRAL_COM_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DhcpServDetectEnable") == 0)
    {
		/* Same value should not be process at anytime */
		if( bValue == pMyObject->bDhcpServDetectEnable )
		{
			return TRUE;
		}

		CosaDmlDiSetAndProcessDhcpServDetectionFlag( pMyObject, &bValue, &pMyObject->bDhcpServDetectEnable );

        return TRUE;
    }

    if (strcmp(ParamName, "MultipleGW") == 0)
    {
		pMyObject->bMultipleGW = bValue;

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM.Ops.{i}.

    *  Ops_GetEntryCount
    *  Ops_GetEntry
    *  Ops_IsUpdated
    *  Ops_Synchronize
    *  Ops_GetParamBoolValue
    *  Ops_GetParamIntValue
    *  Ops_GetParamUlongValue
    *  Ops_GetParamStringValue

***********************************************************************/




/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ops_GetParamBoolValue
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
Ops_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "UploadLogsNow") == 0)
    {
        /* collect value */
         *pBool = FALSE;
         
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ops_GetParamIntValue
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
Ops_GetParamIntValue
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
        Ops_GetParamUlongValue
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
Ops_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Ops_GetParamStringValue
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
Ops_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "LogsUploadStatus") == 0)
    {
        /* collect value */
	COSADmlUploadLogsStatus(NULL, pValue,pUlSize);
        return 0;
    }
    else if(strcmp(ParamName, "LogFileName") == 0)
    {
	/* collect value */
	COSADmlLogFileName(NULL, pValue,pUlSize);
	return 0;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ops_SetParamBoolValue
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
Ops_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    BOOL                            bReturnValue;

    if (strcmp(ParamName, "UploadLogsNow") == 0)
    {

	bReturnValue =
        	COSADmlUploadLogsNow
            	(
                	(ANSC_HANDLE)NULL, 
			bValue
            	);


    		if ( ! bReturnValue )
    		{
        		return TRUE;
    		}
    		else
    		{
        		/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
        		return FALSE;
    		}
    }
    return FALSE;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.ProcessStatus.

    *  ProcessStatus_GetParamBoolValue
    *  ProcessStatus_GetParamIntValue
    *  ProcessStatus_GetParamUlongValue
    *  ProcessStatus_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ProcessStatus_GetParamBoolValue
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
ProcessStatus_GetParamBoolValue
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
        ProcessStatus_GetParamIntValue
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
ProcessStatus_GetParamIntValue
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
        ProcessStatus_GetParamUlongValue
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
ProcessStatus_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "CPUUsage") == 0)
    {
        /* collect value */
         *puLong = COSADmlGetCpuUsage();
         
        return TRUE;
    }
#if defined (RESOURCE_OPTIMIZATION)
    if (strcmp(ParamName, "ProcessNumberOfEntries") == 0)
    {
        /* collect value */
         *puLong = COSADmlGetProcessNumberOfEntries();
         
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
        ProcessStatus_GetParamStringValue
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
ProcessStatus_GetParamStringValue
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

#if !defined (RESOURCE_OPTIMIZATION)

/***********************************************************************

 APIs for Object:

    DeviceInfo.ProcessStatus.Process.{i}.

    *  Process_GetEntryCount
    *  Process_GetEntry
    *  Process_IsUpdated
    *  Process_Synchronize
    *  Process_GetParamBoolValue
    *  Process_GetParamIntValue
    *  Process_GetParamUlongValue
    *  Process_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Process_GetEntryCount
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
Process_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_PROCSTATUS      pProc     = (PCOSA_DATAMODEL_PROCSTATUS)g_pCosaBEManager->hProcStatus;

    return pProc->ProcessNumberOfEntries;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Process_GetEntry
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
Process_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_PROCSTATUS      pProc     = (PCOSA_DATAMODEL_PROCSTATUS)g_pCosaBEManager->hProcStatus;

    *pInsNumber  = nIndex + 1;             

    if (nIndex < pProc->ProcessNumberOfEntries)
    {
        return pProc->pProcTable+nIndex;
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Process_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
static ULONG last_tick;
#define REFRESH_INTERVAL 120
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))
BOOL
Process_IsUpdated
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
        Process_Synchronize
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
Process_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_PROCSTATUS      pProc     = (PCOSA_DATAMODEL_PROCSTATUS)g_pCosaBEManager->hProcStatus;

    if (pProc->pProcTable != NULL)
    {
         AnscFreeMemory(pProc->pProcTable);
         pProc->pProcTable = NULL;
         pProc->ProcessNumberOfEntries = 0;
    }

    COSADmlGetProcessInfo(pProc);
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Process_GetParamBoolValue
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
Process_GetParamBoolValue
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
        Process_GetParamIntValue
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
Process_GetParamIntValue
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
        Process_GetParamUlongValue
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
Process_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_PROCESS_ENTRY        p_proc = (PCOSA_PROCESS_ENTRY)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "PID") == 0)
    {
        /* collect value */
        *puLong = p_proc->Pid;
        return TRUE;
    }

    if (strcmp(ParamName, "Size") == 0)
    {
        /* collect value */
        *puLong = p_proc->Size;        
        return TRUE;
    }

    if (strcmp(ParamName, "Priority") == 0)
    {
        /* collect value */
        *puLong = p_proc->Priority;                
        return TRUE;
    }

    if (strcmp(ParamName, "CPUTime") == 0)
    {
        /* collect value */
        *puLong = p_proc->CPUTime;                
        return TRUE;
    }

    if (strcmp(ParamName, "State") == 0)
    {
        /* collect value */
        *puLong = p_proc->State;                
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Process_GetParamStringValue
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
Process_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_PROCESS_ENTRY        p_proc = (PCOSA_PROCESS_ENTRY)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Command") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, p_proc->Command);
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

#endif

/**********************************************************************

 APIs for Object:

    DeviceInfo.NetworkProperties.

    *  NetworkProperties_GetParamBoolValue
    *  NetworkProperties_GetParamIntValue
    *  NetworkProperties_GetParamUlongValue
    *  NetworkProperties_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NetworkProperties_GetParamBoolValue
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
NetworkProperties_GetParamBoolValue
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
        NetworkProperties_GetParamIntValue
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
NetworkProperties_GetParamIntValue
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
        NetworkProperties_GetParamUlongValue
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
NetworkProperties_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "MaxTCPWindowSize") == 0)
    {
        /* collect value */
        *puLong = COSADmlGetMaxWindowSize();
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        NetworkProperties_GetParamStringValue
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
NetworkProperties_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "TCPImplementation") == 0)
    {
	/* collect value */
        CosaDmlGetTCPImplementation(NULL,pValue,pulSize);

        return 0;
    }
	
    return -1;
}
/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SHORTS.Enable

    *  SHORTS_SetParamBoolValue // Set args required for SHORTS
    *  SHORTS_GetParamBoolValue // Get args set for SHORTS

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SHORTS_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool,
            );

    description:

        This function is called to retrieve boolean  parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;
                BOOL*                       pBool,
                The buffer of returned boolean value;

     return:     TRUE if succeeded.


**********************************************************************/
BOOL
SHORTS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
        UNREFERENCED_PARAMETER(hInsContext);
        if (strcmp(ParamName, "Enable") == 0)
        {
                char buf[8]={0};
                if (syscfg_get( NULL, "ShortsEnabled", buf, sizeof(buf)) == 0)
                {
                if (strncmp(buf, "true", sizeof(buf)) == 0)
                      *pBool = TRUE;
                else
                     *pBool = FALSE;
                }
                else
                {
                         CcspTraceError(("%s syscfg_get failed  for SHORTSEnable\n",__FUNCTION__));
                         *pBool = FALSE;
                }
                return TRUE;
        }
        return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SHORTS_SetParamBoolValue
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
SHORTS_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
        if (IsBoolSame(hInsContext, ParamName, bValue, SHORTS_GetParamBoolValue))
            return TRUE;
        if (strcmp(ParamName, "Enable") == 0)
        {
                if(syscfg_set_commit(NULL, "ShortsEnabled", (bValue == TRUE) ? "true" : "false") != 0)
                {
                        CcspTraceError(("[%s] syscfg_set failed for SHORTS\n",__FUNCTION__));
                        return FALSE;
                }
                return TRUE;
         }
         return FALSE;
}
/* CodeBigFirst config download can be enabled/disabled               */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CodeBigFirst_GetParamBoolValue
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
CodeBigFirst_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    char buf[8];
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /* CID: 58325 Array compared against 0*/
        if(!syscfg_get( NULL, "CodeBigFirstEnabled", buf, sizeof(buf)))
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }

    return FALSE;
}

/* CodeBigFirst  download can be enabled/disabled                     */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CodeBig_First_SetParamBoolValue
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
CodeBig_First_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, CodeBigFirst_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        syscfg_set_commit(NULL, "CodeBigFirstEnabled", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }

    return FALSE;
}


/* PresenceDetect can be enabled/disabled to detect connected clients presence */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PresenceDetect_GetParamBoolValue
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
PresenceDetect_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    char buf[8];
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /*CID: 67242 Array compared against 0*/
        if(!syscfg_get( NULL, "PresenceDetectEnabled", buf, sizeof(buf)))
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        } else 
           return FALSE;

        return TRUE;
    }

    return FALSE;
}

/* PresenceDetect can be enabled/disabled to detect connected clients presence */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PresenceDetect_SetParamBoolValue
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
PresenceDetect_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, PresenceDetect_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "PresenceDetectEnabled", (bValue == TRUE) ? "true" : "false") != 0)
        {
             AnscTraceWarning(("syscfg_set failed for Presence feature param update\n"));
             return FALSE;
        }
        CosaDmlPresenceEnable(bValue);
        return TRUE;
    }

    return FALSE;
}

/* LostandFoundInternet can be blocked/allowed */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        LostandFoundInternet_GetParamBoolValue
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
LostandFoundInternet_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];

    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /* CID: 119814 Array compared against 0*/
       if(!syscfg_get( NULL, "BlockLostandFoundInternet", buf, sizeof(buf))) 
       {
        if (strcmp(buf, "true") == 0)
            *pBool = TRUE;
        else
            *pBool = FALSE;
       } else 
          return FALSE;

        return TRUE;
    }

    return FALSE;
}

/* BlockLostandFoundInternet can be enabled/disabled to allow internet to lnf clients */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        LostandFoundInternet_SetParamBoolValue
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
LostandFoundInternet_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "BlockLostandFoundInternet", (bValue == TRUE) ? "true" : "false") != 0)
        {
             AnscTraceWarning(("syscfg_set failed for block lnf internet update\n"));
             return FALSE;
        }
	    v_secure_system("sysevent set firewall-restart");
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        OAUTH_GetParamStringValue
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

    return:     0 if succeeded;
                -1 if not supported.

**********************************************************************/

ULONG
OAUTH_GetParamStringValue
    (
        ANSC_HANDLE        hInsContext,
        char*              ParamName,
        char*              pValue,
        ULONG*             pUlSize
    )
{
    LONG retval = -1;
    char buf[20];
    BOOL bAuthModeCheck = FALSE;

    hInsContext = hInsContext;    // prevent compiler unused variable warning
    if (strcmp(ParamName, "AuthMode") == 0)
    {
        retval = 0;
        bAuthModeCheck = TRUE;
    }
    else if (strcmp(ParamName, "ServerUrl") == 0)
    {
        retval = 0;
    }
    else
    {
        CcspTraceError(("[%s] Unrecognized param name %s\n", __FUNCTION__, ParamName));
    }
    if( retval == 0 && pValue != NULL )
    {
        snprintf( buf, sizeof(buf), "OAUTH%s", ParamName );  // buf should contain "OAUTH" plus ParamName
        if( (retval=syscfg_get( NULL, buf, pValue, *pUlSize )) != 0 )    // syscfg_get checks for NULL values also
        {
            CcspTraceError(("[%s] syscfg_get failed for %s\n", __FUNCTION__, buf));
            // if we're looking for the AuthMode but failed to find it, let's default to potd
            // we don't need to worry about the other values in this case
            if( bAuthModeCheck == TRUE )
            {
                snprintf( pValue, *pUlSize, "potd" );
            }
            else if( *pUlSize > 0 )
            {
                *pValue = 0;    // no value to return, just an empty string
            }
        }
    }
    return (ULONG)retval;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        OAUTH_SetParamStringValue
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

    return:     TRUE if succeeded,
                FALSE if failed.

**********************************************************************/
BOOL
OAUTH_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    char buf[20];
    BOOL bRet = FALSE;
    BOOL bParamNameGood = FALSE;

    if (IsStringSame(hInsContext, ParamName, pString, OAUTH_GetParamStringValue))
        return TRUE;

    if (strcmp(ParamName, "AuthMode") == 0)
    {
        bParamNameGood = TRUE;
    }
    else if (strcmp(ParamName, "ServerUrl") == 0)
    {
        bParamNameGood = TRUE;
    }
    else
    {
        CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    }

    if( bParamNameGood == TRUE )
    {
        snprintf( buf, sizeof(buf), "OAUTH%s", ParamName );  // buf should contain "OAUTH" plus ParamName
        if( syscfg_set_commit( NULL, buf, pString ) != 0 )
        {
            CcspTraceError(("[%s] syscfg_set failed for %s\n", __FUNCTION__, buf));
        }
        else
        {
            bRet = TRUE;
            CcspTraceInfo(("[%s] %s value set as %s success..!!\n", __FUNCTION__, buf, pString));
        }
    }

    return bRet;
}

/**********************************************************************
    caller:     owner of this object 

    prototype: 

        BOOL
        Iot_GetParamBoolValue
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
Iot_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_ENABLEIOT") == 0)
    {
        /* collect value */
        char buf[8];
        memset(buf, 0, sizeof(buf));
        syscfg_get( NULL, "X_RDKCENTRAL-COM_ENABLEIOT", buf, sizeof(buf));
        /* CID 66516 fix*/
        if( buf[0] != '\0' )
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Iot_SetParamBoolValue
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
Iot_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_ENABLEIOT") == 0)
    {
#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
       id=getuid();
#endif

        if (syscfg_set_commit(NULL, "X_RDKCENTRAL-COM_ENABLEIOT", bValue ? "true" : "false") != 0) 
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        }
        else 
        {
                if(bValue){
                   AnscTraceWarning(("IOT_LOG : Raise IOT event up from DML\n"));
                   v_secure_system("sysevent set iot_status up");
#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
               if(id!=0)
               {
                 char *lxcevt = "sysevent set iot_status up";
                 send(sock , lxcevt , strlen(lxcevt) , 0 );
               }
#endif
             }
                else{
                   AnscTraceWarning(("IOT_LOG : Raise IOT event down from DML\n"));
                   v_secure_system("sysevent set iot_status down");
#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
                if(id!=0)
                 {
                   char *lxcevt = "sysevent set iot_status down";
                   send(sock , lxcevt , strlen(lxcevt) , 0 );
                 }
#endif
                }
                return TRUE;
        }
    }

    return FALSE;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Control

    *  Control_GetParamUlongValue
    *  Control_SetParamUlongValue
***********************************************************************/


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Control_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                        pUlValue
            );

    description:

        This function is called to retrieve integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                        pUlValue
                The buffer of returned unsigned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Control_GetParamUlongValue
    (
	ANSC_HANDLE 		hInsContext,
	char*			ParamName,
	ULONG*			pUlValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
	/* check the parameter name and return the corresponding value */
	if (strcmp(ParamName, "RetrieveNow") == 0)
	{
		*pUlValue =  0;
		return TRUE;
	}

	return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Control_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                         ulValue
            );

    description:

        This function is called to set integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                         ulValue
                The updated unsigned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Control_SetParamUlongValue
    (
	ANSC_HANDLE 		hInsContext,
	char*			ParamName,
	ULONG 			ulValue
    )
{
    if (IsUlongSame(hInsContext, ParamName, ulValue, Control_GetParamUlongValue))
        return TRUE;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "RetrieveNow") == 0)
    {
	if( ulValue == 1 )
	{
		AnscTraceWarning((" Run RFC rfc.service\n"));
		v_secure_system("sh /lib/rdk/rfc.service &");
	}

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
                BOOL
                Control_GetParamBoolValue
                    (
                        ANSC_HANDLE         hInsContext,
                        char*               ParamName,
                        BOOL*               pBool
                    );

    description:

        This function is called to get bool parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Control_GetParamBoolValue
    (
        ANSC_HANDLE         hInsContext,
        char*               ParamName,
        BOOL*               pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(FEATURE_HOSTAP_AUTHENTICATOR)
    char *psmStrValue = NULL;

    *pBool = FALSE; //Initialize to FALSE

    if (strcmp(ParamName, "DisableNativeHostapd") == 0)
    {
        CcspTraceInfo(("[%s] Get DisableNativeHostapd Value \n",__FUNCTION__));

        if (PSM_Get_Record_Value2(bus_handle, g_Subsystem,
            "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Control.DisableNativeHostapd",
            NULL, &psmStrValue) == CCSP_SUCCESS)
        {
            *pBool = _ansc_atoi(psmStrValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(psmStrValue);
        }
        return TRUE;
    }
#else
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
#endif //FEATURE_HOSTAP_AUTHENTICATOR

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Control_SetParamBoolValue
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
Control_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(bValue);
    CcspTraceWarning(("g_currentParamFullName = %s\n", g_currentParamFullName));
    if (strcmp(ParamName, "ClearDB") == 0)
    {
        g_clearDB = true;
        StartRfcProcessing();
        return TRUE;
    }
    else if (strcmp(ParamName, "ClearDBEnd") == 0)
    {
        PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        g_clearDB = false;
        EndRfcProcessing(&pMyObject->pRfcStore);
        return TRUE;
    }
#if defined(FEATURE_HOSTAP_AUTHENTICATOR)
    if (strcmp(ParamName, "DisableNativeHostapd") == 0)
    {
        CcspTraceInfo(("[%s] set DisableNativeHostapd Value [%s]\n",__FUNCTION__, (bValue ? "TRUE" : "FALSE")));
        return CosaDmlSetNativeHostapdState(bValue);
    }
#endif //FEATURE_HOSTAP_AUTHENTICATOR
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Control_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The buffer of returned string value;

                ULONG*                      pulSize
                The buffer of returned string size;

    return:     TRUE if succeeded.

**********************************************************************/
#define XCONF_SELECTOR_SIZE 64
#define XCONF_URL_SIZE 512

ULONG
Control_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    errno_t  rc  = -1;
    /* check the "XconfSelector" parameter name and return the corresponding value */
    if (strcmp(ParamName, "XconfSelector") == 0)
    {
        /* collect value */
           char buff[XCONF_SELECTOR_SIZE]={'\0'};

           /* CID: 108145 Array compared against 0
             CID: 108121 Logically dead code*/
          if(!syscfg_get( NULL, "XconfSelector", buff, sizeof(buff))) {
           rc = strcpy_s(pValue, *pulSize, buff);
           if(rc != EOK)
           {
               ERR_CHK(rc);
               return -1;
           }
           return 0;
          }
          return -1;
    }

    /* check the "XconfUrl" parameter name and return the corresponding value */
    if (strcmp(ParamName, "XconfUrl") == 0)
    {
        /* collect value */
           char buff[XCONF_URL_SIZE]={'\0'};

         if(!syscfg_get( NULL, "XconfUrl", buff, sizeof(buff)))
         {
          /*CID: 108145 Array compared against 0*/
          rc = strcpy_s(pValue, *pulSize, buff);
          if(rc != EOK)
          {
              ERR_CHK(rc);
              return -1;
          }
          return 0;
         }
         return -1;
    }

    /* check the "XconfRecoveryUrl" parameter name and return the corresponding value */
    if (strcmp(ParamName, "XconfRecoveryUrl") == 0)
    {
        /* collect value */
        char buff[XCONF_URL_SIZE];
        buff[XCONF_URL_SIZE-1]='\0';
        if(!syscfg_get( NULL, "XconfRecoveryUrl", buff, sizeof(buff)-1))
        {
        if(pulSize == NULL)
        {
             return -1;
        }
         /*CID: 108145 Array compared against 0*/
        rc = strcpy_s(pValue, *pulSize, buff);
        if(rc != EOK)
        {
             ERR_CHK(rc);
             return -1;
        }
        return 0;
        }
        return -1;
    }

    return -1;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Identity_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The buffer of returned string value;

                ULONG*                      pulSize
                The buffer of returned string size;

    return:     TRUE if succeeded.

**********************************************************************/

ULONG
Identity_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buff[30];
    memset(buff,0,sizeof(buff));
    errno_t  rc  = -1;
    /* check the "DeviceType" parameter name and return the corresponding value */
    if (strcmp(ParamName, "DeviceType") == 0)
    {
          if(!syscfg_get( NULL, "DeviceType", buff, sizeof(buff))) {
           rc = strcpy_s(pValue, *pulSize, buff);
           if(rc != EOK)
           {
               ERR_CHK(rc);
               return -1;
           }
           return 0;
          }
          return -1;
    }
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Identity_SetParamStringValue
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
Identity_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    
    if (IsStringSame(hInsContext, ParamName, pString, Identity_GetParamStringValue))
        return TRUE;

    /* check the "DeviceType" parameter name and set the corresponding value */
    if (strcmp(ParamName, "DeviceType") == 0){
        if( (strcasecmp(pString, "prod") == 0) || (strcasecmp(pString, "PROD") == 0) || (strcasecmp(pString, "TEST") == 0) || (strcasecmp(pString, "test") == 0) ) {
            if (syscfg_set(NULL, "DeviceType", pString) != 0){
                CcspTraceError(("[%s] syscfg_set failed for DeviceType \n",__FUNCTION__));
                return FALSE;
            } else {
                if (syscfg_commit() != 0){
                    CcspTraceError(("[%s] syscfg_commit failed for DeviceType \n",__FUNCTION__));
                    return  FALSE;
                }
            }
            CcspTraceInfo(("[%s] DeviceType value set as %s \n",__FUNCTION__,pString));
        } else{
            CcspTraceInfo(("[%s] DeviceType value should 'prod' , 'PROD' , 'TEST' , 'test' \n",__FUNCTION__));
            return FALSE;
        }
    } else{
        CcspTraceWarning(("[%s] Unsupported parameter '%s'\n",__FUNCTION__,ParamName));
        return FALSE;
    }
    return TRUE;
}

/**
 *  RFC Feature for CrashUpload S3signing url
*/
/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        CrashUpload_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The buffer of returned string value;

                ULONG*                      pulSize
                The buffer of returned string size;

    return:     TRUE if succeeded.

**********************************************************************/
ULONG
    CrashUpload_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pulSize
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pulSize);
    errno_t   rc   = -1;

    if (strcmp(ParamName, "S3SigningUrl") == 0)
    {
        /* collect value */
        char buf[64] = {'\0'};
        memset(buf, 0 ,sizeof(buf));
        if(!syscfg_get( NULL, "CrashUpload_S3SigningUrl", buf, sizeof(buf)))
        {
            rc = strcpy_s(pValue, *pulSize, buf);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        return -1;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CrashUpload_SetParamStringValue
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
                The updated String value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
    CrashUpload_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
 )
{
    if (IsStringSame(hInsContext, ParamName, pString, CrashUpload_GetParamStringValue))
        return TRUE;

    if (strcmp(ParamName, "S3SigningUrl") == 0)
    {
        if (syscfg_set_commit(NULL, "CrashUpload_S3SigningUrl", pString) != 0)
        {
            CcspTraceError(("syscfg_set failed\n"));

        }
        else
        {
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
        Feature_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pint
            );

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                       pint
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
Feature_GetParamIntValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*	                    pint
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
   /* check the parameter name and return the corresponding value */
    CcspTraceInfo(("Feature_GetParamIntValue: RDKLowQueueRebootThreshold\n"));

    if (strcmp(ParamName, "RDKLowQueueRebootThreshold") == 0)
    {
         /* collect value */
         char buf[10];
         syscfg_get( NULL, "low_queue_reboot_threshold", buf, sizeof(buf));
         if( buf [0] != '\0' )
         {
             *pint= ( atoi(buf) );
             return TRUE;
         }
    }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

	BOOL
        Feature_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue
            );

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The buffer of returned string;

    return:     TRUE if succeeded.

**********************************************************************/

ULONG
Feature_GetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    int rc = -1;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "TrackRemotePortUsage") == 0)
    {
        CcspTraceInfo(("Feature_GetParamStringValue: TrackRemotePortUsage\n"));
        char buf[256]={0};

        if(!syscfg_get( NULL, "TrackRemotePortUsage", buf, sizeof(buf)))
        {
           rc = strcpy_s(pValue, sizeof(buf), buf);
           if(rc != EOK) {
              ERR_CHK(rc);
              return -1;
           }
        }
        return 0;
    }
    return -1;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Control_SetParamStringValue
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
Control_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    BOOL bReturnValue = FALSE;

    if (IsStringSame(hInsContext, ParamName, pString, Control_GetParamStringValue))
        return TRUE;

    /* check the "XconfSelector" parameter name and set the corresponding value */
    if (strcmp(ParamName, "XconfSelector") == 0)
    {
        /* collect value */
           int idlen = strlen(pString)-1;

           if ( idlen > XCONF_SELECTOR_SIZE )
           {
                CcspTraceError(("[%s] Invalid XconfSelector length ..!! \n",__FUNCTION__));
                bReturnValue = FALSE;
           }
           else
           {
                /*RDKB-28133 : any string is getting set to XconfSelector TR 181 apart from prod, ci, automation, local*/
                if( (strcasecmp(pString, "prod") == 0) || (strcasecmp(pString, "ci") == 0) || (strcasecmp(pString, "automation") == 0) || (strcasecmp(pString, "local") == 0) )
                {
                    if (syscfg_set_commit(NULL, "XconfSelector", pString) != 0)
                    {
                        CcspTraceError(("[%s] syscfg_set failed for XconfSelector \n",__FUNCTION__));
                        bReturnValue = FALSE;
                    }
                    else
                    {
                        bReturnValue = TRUE;
                        CcspTraceInfo(("[%s] XconfSelector value set as %s success..!!\n",__FUNCTION__,pString));
                    }
                }
                else
                {
                    CcspTraceInfo(("[%s] XconfSelector value should 'prod' , 'ci' , 'automation' , 'local' \n",__FUNCTION__));
                    bReturnValue = FALSE;
                }
           }
    }
    /* check the "XconfUrl" parameter name and set the corresponding value */
    else if (strcmp(ParamName, "XconfUrl") == 0)
    {
        /* collect value */
           int idlen = strlen(pString)-1;

           if ( idlen > XCONF_URL_SIZE )
           {
                CcspTraceError(("[%s] Invalid XconfUrl length ..!! \n",__FUNCTION__));
                bReturnValue = FALSE;
           }
           else
           {
               if (syscfg_set_commit(NULL, "XconfUrl", pString) != 0)
                {
                    CcspTraceError(("[%s] syscfg_set failed for XconfUrl \n",__FUNCTION__));
                    bReturnValue = FALSE;
                }
                else
                {
                    bReturnValue = TRUE;
                    CcspTraceInfo(("[%s] XconfUrl value set as %s success..!!\n",__FUNCTION__,pString));
                }
           }
    }
    /* check the "XconfRecoveryUrl" parameter name and set the corresponding value */
    else if (strcmp(ParamName, "XconfRecoveryUrl") == 0)
    {
        /* collect value */
           int idlen = strlen(pString)-1;

           if ( idlen > XCONF_URL_SIZE )
           {
                CcspTraceError(("[%s] Invalid XconfRecoveryUrl length ..!! \n",__FUNCTION__));
                bReturnValue = FALSE;
           }
           else
           {
               if (syscfg_set(NULL, "XconfRecoveryUrl", pString) != 0)
                {
                    CcspTraceError(("[%s] syscfg_set failed for XconfRecoveryUrl \n",__FUNCTION__));
                    bReturnValue = FALSE;
                }
                else
                {
                    if (syscfg_commit() != 0)
                    {
                         CcspTraceError(("[%s] syscfg_commit failed for XconfRecoveryUrl \n",__FUNCTION__));
                         bReturnValue = FALSE;
                    }
		    /*CID:340237 Unused Value Fix*/
		    else
                    { 
			  bReturnValue = TRUE;
                          CcspTraceInfo(("[%s] XconfRecoveryUrl value set as %s success..!!\n",__FUNCTION__,pString));
                    }
		}
           }
    }
    else
    {
           CcspTraceWarning(("[%s] Unsupported parameter '%s'\n",__FUNCTION__,ParamName));
           bReturnValue = FALSE;
    }

    return bReturnValue;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Feature_GetParamBoolValue
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
Feature_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
#if defined(MOCA_HOME_ISOLATION)
    if (strcmp(ParamName, "HomeNetworkIsolation") == 0)
    {
        /* collect value */
    char *strValue = NULL;
    int retPsmGet = CCSP_SUCCESS;

    retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.HomeNetworkIsolation", NULL, &strValue);
    if (retPsmGet == CCSP_SUCCESS) {
        *pBool = _ansc_atoi(strValue);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
    }
    else
        *pBool = FALSE;
     return TRUE;   
    }
#endif
    if (strcmp(ParamName, "XfinityHealthCheck") == 0)
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "XfinityHealthCheckEnable", value, sizeof(value)) == 0 )
        {
            if (strcmp(value, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
            return TRUE;
        }
        else
        {
            CcspTraceWarning(("syscfg_get failed for XfinityHealthCheckEnable\n"));
            //TCXB8-2563 Setting to False as XfinityHealthCheck is not set
            *pBool = FALSE;
            return TRUE;
        }
    }
    if (strcmp(ParamName, "CodebigSupport") == 0)
    {
         char value[8];
         memset(value, 0, sizeof(value));
         syscfg_get(NULL,"codebigsupport",value, sizeof(value));
         /* CID 66608 fix*/
         if( value[0] != '\0' )
         {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
         }     
         return TRUE;
    }

    if (strcmp(ParamName, "ContainerSupport") == 0)
    {
       /* Collect Value */
       char *strValue = NULL;
       int retPsmGet = CCSP_SUCCESS;


        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Container", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) 
        {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = FALSE;

         return TRUE;
    }

#if defined(_COSA_FOR_BCI_)
    if (strcmp(ParamName, "OneToOneNAT") == 0)
    {
         char value[8];
         /*CID: 66608 Array compared against 0*/
         if(!syscfg_get(NULL,"one_to_one_nat",value, sizeof(value)))
         {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
         } else
           return FALSE;

         return TRUE;
    }

    if (strcmp(ParamName, "EnableMultiProfileXDNS") == 0)
    {
        char buf[5] = {0};
         /*CID: 66608 Array compared against 0*/
        if(!syscfg_get( NULL, "MultiProfileXDNS", buf, sizeof(buf)))
        {
                if (strcmp(buf,"1") == 0)
                {
                        *pBool = TRUE;
                        return TRUE;
                }
        } else 
             return FALSE; 

        *pBool = FALSE;

        return TRUE;
    }
#endif

#if defined(_XB6_PRODUCT_REQ_)   
   if (strcmp(ParamName, "BLERadio") == 0)
    {
        BLE_Status_e status;
        if(!ble_GetStatus(&status))
        {
           if(status == BLE_ENABLE)
            {
                *pBool = TRUE;
            } else {
                *pBool = FALSE;
            }
            return TRUE;
        }
        else {
            CcspTraceWarning(("%s: ble_GetStatus failed\n", __func__));
	    }
    }
#endif
    if (strcmp(ParamName, "Xupnp") == 0)
    {
	 char value[8];
         /*CID: 66608 Array compared against 0*/
         if(!syscfg_get(NULL, "start_upnp_service", value, sizeof(value)))
         {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
            return TRUE;
         } else {
	    return FALSE;
         }
    }

#ifdef _BRIDGE_UTILS_BIN_ 
    if (strcmp(ParamName, "BridgeUtilsEnable") == 0)
    {
           char value[8] = {0};
           syscfg_get(NULL, "bridge_util_enable", value, sizeof(value));
           if( value[0] != '\0' )
           {
               if (strcmp(value, "true") == 0)
                   *pBool = TRUE;
               else
                   *pBool = FALSE;
           }
           return TRUE;
    }
#endif
    if (strcmp(ParamName, "IPModeEnable") == 0)
    {
        char *strValue = NULL;
        int retPsmGet = CCSP_SUCCESS;

        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.IPModeEnable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS)
        {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
        {
            *pBool = FALSE;
        }
        return TRUE;
    }
#if (defined _COSA_INTEL_XB3_ARM_)
    if (strcmp(ParamName, "CMRouteIsolationEnable") == 0)
    {
	   char value[8] = {0};
           syscfg_get(NULL, "CMRouteIsolation_Enable", value, sizeof(value));
           if( value[0] != '\0' )
           {
               if (strcmp(value, "true") == 0)
                   *pBool = TRUE;
               else
                   *pBool = FALSE;
           }
           return TRUE;
    }
#endif
    return FALSE;
}


BOOL
SyndicationFlowControl_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL SyndicatonFlowControl = (PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL)&(pMyObject->SyndicatonFlowControl);
    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = SyndicatonFlowControl->Enable.ActiveValue;
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        EncryptCloudUpload_GetParamBoolValue
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
EncryptCloudUpload_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8];
        /* CID: 69222 Array compared against 0*/
        if(!syscfg_get(NULL,"encryptcloudupload",value, sizeof(value)))
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
             return TRUE;
        } else {
            return FALSE;
        }     
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        UploadLogsOnUnscheduledReboot_GetParamBoolValue
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
UploadLogsOnUnscheduledReboot_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Disable") == 0)
    {
        char value[8];

        if(!syscfg_get(NULL,"UploadLogsOnUnscheduledRebootDisable",value, sizeof(value)))
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
             return TRUE;
        } else {
            return FALSE;
        }     
    }
    return FALSE;
}

ULONG
SyndicationFlowControl_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL SyndicatonFlowControl = (PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL)&(pMyObject->SyndicatonFlowControl);
    if (strcmp(ParamName, "InitialForwardedMark") == 0)
    {
        return  update_pValue(pValue,pUlSize, SyndicatonFlowControl->InitialForwardedMark.ActiveValue);
    }
    if (strcmp(ParamName, "InitialOutputMark") == 0)
    {
        return  update_pValue(pValue,pUlSize, SyndicatonFlowControl->InitialOutputMark.ActiveValue);
    }
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MEMSWAP_GetParamBoolValue
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
MEMSWAP_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
       /* Collect Value */
       char *strValue = NULL;
       int retPsmGet = CCSP_SUCCESS;


        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.MEMSWAP.Enable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = FALSE;

         return TRUE;
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DNSSTRICTORDER_GetParamBoolValue
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
DNSSTRICTORDER_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8];
        if(!syscfg_get(NULL,"DNSStrictOrder",value, sizeof(value))) {
        /* CID: 119695 Array compared against 0*/
        if (strcmp(value, "true") == 0)
            *pBool = TRUE;
        else
           *pBool = FALSE;
        } else
           return FALSE;

        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ActiveMeasurements_RFC_GetParamBoolValue
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
ActiveMeasurements_RFC_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
       /* Collect Value */
       char *strValue = NULL;
       int retPsmGet = CCSP_SUCCESS;


        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WifiClient.ActiveMeasurements.Enable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = TRUE;

         return TRUE;
    }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ActiveMeasurements_RFC_SetParamBoolValue
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
ActiveMeasurements_RFC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, ActiveMeasurements_RFC_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
       int retPsmGet = CCSP_SUCCESS;

       retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WifiClient.ActiveMeasurements.Enable", ccsp_string, bValue ? "1" : "0");
       if (retPsmGet != CCSP_SUCCESS) {
           CcspTraceError(("Set failed for Active Measurement RFC enable \n"));
           return FALSE;
       }
       CcspTraceInfo(("Successfully set Active Measurement RFC enable \n"));
       return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WPA3_Personal_Transition_RFC_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value for
		WPA3 Transition RFC;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
WPA3_Personal_Transition_RFC_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (!ParamName || !pBool)
        return FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
       /* Collect Value */
       char *strValue = NULL;
       int retPsmGet = CCSP_SUCCESS;


        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WPA3_Personal_Transition.Enable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = FALSE;

         return TRUE;
    }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WPA3_Personal_Transition_RFC_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value for
		WPA3 Transition RFC;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
WPA3_Personal_Transition_RFC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, WPA3_Personal_Transition_RFC_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        int retPsmGet = CCSP_SUCCESS;
        int ret = -1;
        char* faultParam = NULL;
#ifndef RDK_ONEWIFI
        componentStruct_t ** ppComponents = NULL;
        char dst_pathname_cr[64];
        int size = 0;
        char *secMode;
        errno_t rc  = -1;
#else
        char                 paramName[256] = "Device.WiFi.WPA3_Personal_Transition";
        char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
        char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
        parameterValStruct_t pVal[1];
#endif

        CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WPA3_Personal_Transition.Enable", ccsp_string, bValue ? "1" : "0");
        if (retPsmGet != CCSP_SUCCESS) {
            CcspTraceError(("Set failed for WPA3 Transition RFC Enable \n"));
            return FALSE;
        }
        CcspTraceInfo(("Successfully set WPA3 Transition RFC Enable \n"));

#ifndef RDK_ONEWIFI
        secMode = bValue ? "WPA3-Personal-Transition" : "WPA2-Personal";

       /* calling the DML code to set the security modes accordingly */
        rc = sprintf_s(dst_pathname_cr, sizeof(dst_pathname_cr),"%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                dst_pathname_cr,
                "Device.WiFi.X_CISCO_COM_ResetRadios",
                g_Subsystem,        /* prefix */
                &ppComponents,
                &size);

        if ( ret == CCSP_SUCCESS && size == 1)
        {
            parameterValStruct_t    val[] = 
            {
                { "Device.WiFi.AccessPoint.1.Security.ModeEnabled", secMode, ccsp_string},
                { "Device.WiFi.AccessPoint.2.Security.ModeEnabled", secMode, ccsp_string},
                { "Device.WiFi.AccessPoint.3.Security.ModeEnabled", secMode, ccsp_string},
                { "Device.WiFi.AccessPoint.4.Security.ModeEnabled", secMode, ccsp_string},
                { "Device.WiFi.AccessPoint.7.Security.ModeEnabled", secMode, ccsp_string},
                { "Device.WiFi.AccessPoint.8.Security.ModeEnabled", secMode, ccsp_string},
                { "Device.WiFi.AccessPoint.15.Security.ModeEnabled", secMode, ccsp_string},
                { "Device.WiFi.AccessPoint.16.Security.ModeEnabled", secMode, ccsp_string},
                { "Device.WiFi.AccessPoint.1.Security.X_RDKCENTRAL-COM_TransitionDisable", "false", ccsp_boolean},
                { "Device.WiFi.AccessPoint.2.Security.X_RDKCENTRAL-COM_TransitionDisable", "false", ccsp_boolean},
                { "Device.WiFi.AccessPoint.3.Security.X_RDKCENTRAL-COM_TransitionDisable", "false", ccsp_boolean},
                { "Device.WiFi.AccessPoint.4.Security.X_RDKCENTRAL-COM_TransitionDisable", "false", ccsp_boolean},
                { "Device.WiFi.AccessPoint.7.Security.X_RDKCENTRAL-COM_TransitionDisable", "false", ccsp_boolean},
                { "Device.WiFi.AccessPoint.8.Security.X_RDKCENTRAL-COM_TransitionDisable", "false", ccsp_boolean},
                { "Device.WiFi.AccessPoint.15.Security.X_RDKCENTRAL-COM_TransitionDisable", "false", ccsp_boolean},
                { "Device.WiFi.AccessPoint.16.Security.X_RDKCENTRAL-COM_TransitionDisable", "false", ccsp_boolean},
                { "Device.WiFi.Radio.1.X_CISCO_COM_ApplySetting", "true", ccsp_boolean},
                { "Device.WiFi.Radio.2.X_CISCO_COM_ApplySetting", "true", ccsp_boolean},
            };

            ret = CcspBaseIf_setParameterValues
                (
                 bus_handle,
                 ppComponents[0]->componentName,
                 ppComponents[0]->dbusPath,
                 0, 0x0,
                 val,
                 18,
                 TRUE,
                 &faultParam
                );

            if (ret != CCSP_SUCCESS && faultParam)
            {
                AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                bus_info->freefunc(faultParam);
            }
            free_componentStruct_t(bus_handle, size, ppComponents);
        }
#else
        pVal[0].parameterName  = paramName;
        pVal[0].parameterValue = bValue ? "true" : "false";
        pVal[0].type           = ccsp_boolean;
        ret = CcspBaseIf_setParameterValues(
                 bus_handle,
                 compName,
                 dbusPath,
                 0,
                 0,
                 pVal,
                 1,
                 TRUE,
                 &faultParam
             );

        if (ret != CCSP_SUCCESS)
        {
            CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
            bus_info->freefunc(faultParam);

            //Restore the value in PSM
            char *previous = bValue ? "0" : "1";
            PSM_Set_Record_Value2(bus_handle,g_Subsystem,
                               "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WPA3_Personal_Transition.Enable",
                               ccsp_string, previous);
            return FALSE;
        }
#endif
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ErrorsReceived_RFC_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value for
		ErrorsReceived RFC;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
ErrorsReceived_RFC_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (!ParamName || !pBool)
        return FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
       /* Collect Value */
       char *strValue = NULL;
       int retPsmGet = CCSP_SUCCESS;

        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.ErrorsReceived.Enable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = FALSE;

         return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ErrorsReceived_RFC_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value for
		ErrorsReceived RFC;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
ErrorsReceived_RFC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, ErrorsReceived_RFC_GetParamBoolValue))
        return TRUE;
    
    if (strcmp(ParamName, "Enable") == 0)
    {
       int retPsmGet = CCSP_SUCCESS;

       retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.ErrorsReceived.Enable", ccsp_string, bValue ? "1" : "0");
       if (retPsmGet != CCSP_SUCCESS) {
           CcspTraceError(("Set failed for ErrorsReceived RFC enable \n"));
           return FALSE;
       }
       CcspTraceInfo(("Successfully set ErrorsReceived RFC enable \n"));
       return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DLCaStore_RFC_GetParamBoolValue
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
DLCaStore_RFC_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char buf[8] = {0};
        /* collect value */
        if (syscfg_get( NULL, "DLCaStoreEnabled", buf, sizeof(buf)) == 0)
        {
            if (strncmp(buf, "true", sizeof(buf)) == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        else
        {
            CcspTraceError(("%s syscfg_get failed  for DLCaStoreEnable\n",__FUNCTION__));
            *pBool = FALSE;
        }

        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DLCaStore_RFC_SetParamBoolValue
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
DLCaStore_RFC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, DLCaStore_RFC_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "DLCaStoreEnabled", (bValue == TRUE) ? "true" : "false") != 0)
        {
            CcspTraceError(("[%s] syscfg_set failed for DLCaStore\n",__FUNCTION__));
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WPA3_Personal_Compatibility_RFC_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value for
		WPA3 personal compatibility RFC;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
WPA3_Personal_Compatibility_RFC_GetParamBoolValue
    (
         ANSC_HANDLE                hInsContext,
         char *                     ParamName,
         BOOL *                     pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "Enable") == 0)
    {
#ifdef RDK_ONEWIFI
        int ret = ANSC_STATUS_FAILURE;
        parameterValStruct_t    **valStructs = NULL;
        char *dstComponent = "eRT.com.cisco.spvtg.ccsp.wifi";
        char *dstPath = "/com/cisco/spvtg/ccsp/wifi";
        char *paramNames[]={"Device.WiFi.WPA3_Personal_Compatibility"};
        int  valNum = 0;
        errno_t rc = -1;
        int ind = -1;

        ret = CcspBaseIf_getParameterValues(
               bus_handle,
               dstComponent,
               dstPath,
               paramNames,
               1,
               &valNum,
               &valStructs);

        if (CCSP_Message_Bus_OK != ret)
        {
            CcspTraceError(("%s CcspBaseIf_getParameterValues %s error %d\n",__FUNCTION__,paramNames[0],ret));
            free_parameterValStruct_t(bus_handle, valNum, valStructs);
            *pBool = FALSE;
            return FALSE;
        }

        CcspTraceInfo(("valStructs[0]->parameterValue = %s\n",valStructs[0]->parameterValue));
        rc = strcmp_s("true",strlen("true"),valStructs[0]->parameterValue,&ind);
        ERR_CHK(rc);

        if ((ind == 0) && (rc == EOK))
        {
            free_parameterValStruct_t(bus_handle, valNum, valStructs);
            *pBool = TRUE;
            return TRUE;
        }
        else
        {
            free_parameterValStruct_t(bus_handle, valNum, valStructs);
            *pBool = FALSE;
            return FALSE;
        }
#else
        *pBool = FALSE;
        return FALSE;
#endif
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WPA3_Personal_Compatibility_RFC_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value for
		WPA3 Personal Compatibility RFC;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
WPA3_Personal_Compatibility_RFC_SetParamBoolValue
    (
         ANSC_HANDLE                 hInsContext,
         char*                       ParamName,
         BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "Enable") == 0) {
#ifdef RDK_ONEWIFI
        parameterValStruct_t pVal[1];
        char                 paramName[256] = "Device.WiFi.WPA3_Personal_Compatibility";
        char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
        char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
        char*                faultParam     = NULL;
        int                  ret            = 0;
        CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
        pVal[0].parameterName  = paramName;
        pVal[0].parameterValue = bValue ? "true" : "false";
        pVal[0].type           = ccsp_boolean;

        ret = CcspBaseIf_setParameterValues(
               bus_handle,
               compName,
               dbusPath,
               0,
               0,
               pVal,
               1,
               TRUE,
               &faultParam
              );

        if (ret != CCSP_SUCCESS)
        {
            CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
            bus_info->freefunc(faultParam);
            return FALSE;
        }
        CcspTraceInfo(("Successfully set WPA3_Personal_Compatibility RFC\n"));
        return TRUE;
#else
        CcspTraceInfo(("[%s] set WPA3 COMPAT RFC Value [%s]\n",__FUNCTION__, (bValue ? "TRUE" : "FALSE")));
        return FALSE;
#endif
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Feature_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         bValue
            );

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                        bValue
                The updated int value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Feature_SetParamIntValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    CcspTraceInfo(("Feature_SetParamIntValue: RDKLowQueueRebootThreshold"));

    if (strcmp(ParamName, "RDKLowQueueRebootThreshold") == 0)
    {
        char buf[8]={0};
        snprintf(buf, sizeof(buf), "%d", bValue);

        if (syscfg_set_commit(NULL, "low_queue_reboot_threshold", buf) != 0)
        {
               CcspTraceInfo(("syscfg_set low_queue_reboot_threshold failed\n"));
        }
	return TRUE;
    }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Feature_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       bValue
            );

    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       bValue
                The updated string;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Feature_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */

    if (strcmp(ParamName, "TrackRemotePortUsage") == 0)
    {
        CcspTraceInfo(("Feature_SetParamStringValue: TrackRemotePortUsage\n"));
        if (strcmp(bValue, "NULL") == 0) {
            if (syscfg_set_commit(NULL, "TrackRemotePortUsage", bValue) != 0) {
                CcspTraceInfo(("syscfg_set TrackRemotePortUsage failed\n"));
            }
            //Remove the existing cron job for RemotePortUsage
            v_secure_system("sed -i '/remote_port_usage.sh/d' /var/spool/cron/crontabs/root");
            return TRUE;
        }

        // Check for the presence of multiple semicolons
        char *firstSemicolon = strchr(bValue, ';');
        char *lastSemicolon = strrchr(bValue, ';');

        if (firstSemicolon == NULL || firstSemicolon != lastSemicolon) {
            CcspTraceError(("Invalid input. Format should be \"TimeInterval;<Max of 3 protocol:port pairs seperated by ,>\" \n"));
            return FALSE;
        }

        char buf[256]={0};
        int interval;
        char *pairs;
        snprintf(buf, sizeof(buf), "%s", bValue);

        interval = atoi(strtok(buf, ";"));
        pairs = strtok(NULL, ";");

        //Validation for Interval
        if (interval != 15 && interval != 30 && interval != 45 && interval != 60 &&
                                                interval != 120 && interval != 1440) {
            CcspTraceError(("Invalid interval value to check RemotePortUsage. Allowed values are 15, 30, 45, 60, 120, and 1440\n"));
            return FALSE;
        }

        //Validation for Protocol:Port pairs
        char* token = strtok(pairs, ",");
        int count = 0;

        while (token != NULL) {
            count++;
            token = strtok(NULL, ",");
        }
        if (count == 0) {
            CcspTraceError(("Atleast one protocol:port pair must be provided\n"));
            return FALSE;
        }
        if (count > 3) {
            CcspTraceError(("Maximum of 3 protocol:port pair is allowed\n"));
            return FALSE;
        }

        if (syscfg_set_commit(NULL, "TrackRemotePortUsage", bValue) != 0) {
            CcspTraceInfo(("syscfg_set TrackRemotePortUsage failed\n"));
        }

        //Remove the existing cron job for RemotePortUsage
        v_secure_system("sed -i '/remote_port_usage.sh/d' /var/spool/cron/crontabs/root");
        //Adding new cron job for RemotePortUsage with specified interval
        v_secure_system("(crontab -l 2>/dev/null; echo \"*/%d * * * * /usr/ccsp/tad/remote_port_usage.sh\") | crontab -", interval);
        CcspTraceInfo(("Scheduled cron job for RemotePortUsage\n"));

        return TRUE;
    }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DFSatBootUp_GetParamBoolValue
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
DFSatBootUp_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
       /* Collect Value */
       char *strValue = NULL;
       int retPsmGet = CCSP_SUCCESS;

       retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.DFSatBootUp.Enable", NULL, &strValue);
       if (retPsmGet == CCSP_SUCCESS) {
           *pBool = _ansc_atoi(strValue);
           ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
       }
       else
           *pBool = FALSE;

       return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DFS_GetParamBoolValue
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
DFS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
       /* Collect Value */
       char *strValue = NULL;
       int retPsmGet = CCSP_SUCCESS;

       retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.DFS.Enable", NULL, &strValue);
       if (retPsmGet == CCSP_SUCCESS) {
           *pBool = _ansc_atoi(strValue);
           ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
       }
       else
           *pBool = FALSE;

       return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        EasyConnect_GetParamBoolValue
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
EasyConnect_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
       /* Collect Value */
       char *strValue = NULL;
       int retPsmGet = CCSP_SUCCESS;


        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.EasyConnect.Enable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = TRUE;

         return TRUE;
    }

    if (strcmp(ParamName, "EnableAPISecurity") == 0)
    {
       /* Collect Value */
       char *strValue = NULL;
       int retPsmGet = CCSP_SUCCESS;


        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.EasyConnect.EnableAPISecurity", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = FALSE;

         return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DFSatBootUp_SetParamBoolValue
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
DFSatBootUp_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        CcspTraceInfo(("[%s] set  DFSatBootUp Value [%s]\n",__FUNCTION__, (bValue ? "TRUE" : "FALSE")));
        return CosaDmlSetDFSatBootUp(bValue);
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DFS_SetParamBoolValue
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
DFS_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        CcspTraceInfo(("[%s] set  DFS Value [%s]\n",__FUNCTION__, (bValue ? "TRUE" : "FALSE")));
        return CosaDmlSetDFS(bValue);
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        EasyConnect_SetParamBoolValue
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
EasyConnect_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, EasyConnect_GetParamBoolValue))
        return TRUE;


    if (strcmp(ParamName, "Enable") == 0)
    {
       int retPsmGet = CCSP_SUCCESS;

       retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.EasyConnect.Enable", ccsp_string, bValue ? "1" : "0");
       if (retPsmGet != CCSP_SUCCESS) {
           CcspTraceError(("Set failed for EasyConnect support \n"));
           return FALSE;
       }
       CcspTraceInfo(("Successfully set EasyConnect support \n"));
       return TRUE;
    }

    if (strcmp(ParamName, "EnableAPISecurity") == 0)
    {
       int retPsmGet = CCSP_SUCCESS;

       retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.EasyConnect.EnableAPISecurity", ccsp_string, bValue ? "1" : "0");
       if (retPsmGet != CCSP_SUCCESS) {
           CcspTraceError(("Set failed for EasyConnect APISecurity support \n"));
           return FALSE;
       }
       CcspTraceInfo(("Successfully set EasyConnect APISecurity support \n"));
       return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Feature_SetParamBoolValue
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
Feature_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, Feature_GetParamBoolValue))
        return TRUE;

#if defined(MOCA_HOME_ISOLATION)
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "HomeNetworkIsolation") == 0)
    {
    int retPsmGet = CCSP_SUCCESS;

   /* char *strValue = NULL;
    BOOL getVal = 0;
    retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.HomeNetworkIsolation", NULL, &strValue);
    if (retPsmGet == CCSP_SUCCESS) {
        getVal = _ansc_atoi(strValue);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
    }

   / if(getVal != bValue)*/
	{
             retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.HomeNetworkIsolation", ccsp_string, bValue ? "1" : "0");
             if (retPsmGet != CCSP_SUCCESS) {
             CcspTraceError(("Set failed for HomeNetworkIsolation \n"));
             return FALSE;
             }
                if(bValue)
                {
                    CcspTraceInfo(("Apply changes for HomeNetworkIsolation \n"));
#if defined(_COSA_BCM_MIPS_)
                    v_secure_system("sh /usr/ccsp/lan_handler.sh home_lan_isolation_enable &");
                    sleep(2);
#else
                    v_secure_system("sysevent set multinet-restart 1");
#endif
                    v_secure_system("sh /usr/ccsp/moca/MoCA_isolation.sh &");
                    
                }
                else
                {

                    CcspTraceInfo(("reverting changes for HomeNetworkIsolation \n"));
#if defined(_COSA_BCM_MIPS_)
                    v_secure_system("sh /usr/ccsp/lan_handler.sh home_lan_isolation_disable &");
                    v_secure_system("rm /tmp/MoCABridge_up");
#else
                    v_secure_system("sysevent set multinet-down 9");
                    v_secure_system("rm /tmp/MoCABridge_up");
                    v_secure_system("sysevent set multinet-restart 1");
#endif
                    v_secure_system("killall mcsender; killall MRD; killall smcroute;igmpproxy -c /tmp/igmpproxy.conf &");

                    v_secure_system("sh /usr/ccsp/moca/MoCA_isolation.sh &");
                }
	}
	//else
	//	CcspTraceInfo(("HomeNetworkIsolation is already %d \n",getVal));
	
    	return TRUE;
    }
#endif

    if (strcmp(ParamName, "CodebigSupport") == 0)
    {
        syscfg_set_commit(NULL, "codebigsupport", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }

  if (strcmp(ParamName, "XfinityHealthCheck") == 0)
    {
        if ((syscfg_set(NULL, "XfinityHealthCheckEnable", bValue ? "true" : "false") != 0) ||
            (syscfg_set(NULL, "XfinityHealthCheckDone", "false") != 0))
        {
            CcspTraceError(("syscfg_set failed for XfinityHealthCheckEnable\n"));
        }
        else
        {
            char value[8];
            int i,days = 0;

            if (syscfg_get(NULL, "XfinityHealthCheckCadence", value, sizeof(value)) == 0)
            {
                for (i = 0; value[i] != '\0'; i++)
                    days = days*10 + value[i] - 48;
            }

            if (days == 0)
            {
                if (syscfg_set(NULL, "XfinityHealthCheckCadence", "7") != 0)
                {
                    AnscTraceWarning(("syscfg_set failed\n"));
                    return FALSE;
                }
            }
            if ((syscfg_set(NULL, "XfinityHealthCheckReset", "0") != 0) ||
                (syscfg_set_commit(NULL, "XfinityHealthCheckRemDays", "-1") != 0))
            {
                AnscTraceWarning(("syscfg_set failed while resetting\n"));
                return FALSE;
            }
        }

        return TRUE;
    }

    if (strcmp(ParamName, "ContainerSupport") == 0)
    {
       int retPsmGet = CCSP_SUCCESS;

       syscfg_set_commit(NULL, "containersupport", (bValue == TRUE) ? "true" : "false");

       retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Container", ccsp_string, bValue ? "1" : "0");
       if (retPsmGet != CCSP_SUCCESS) 
       {
          CcspTraceError(("Set failed for ContainerSupport \n"));
          return FALSE;
       }
       CcspTraceInfo(("Successfully set ContainerSupport \n"));
       return TRUE;
    }

#if defined(_COSA_FOR_BCI_)
    if (strcmp(ParamName, "OneToOneNAT") == 0)
    {
        BOOL bNatEnable = FALSE;
        bNatEnable  = g_GetParamValueBool(g_pDslhDmlAgent, "Device.NAT.X_Comcast_com_EnableNATMapping");
        if ( bValue != bNatEnable )
        {
            g_SetParamValueBool("Device.NAT.X_Comcast_com_EnableNATMapping", bValue);
        }

        syscfg_set_commit(NULL, "one_to_one_nat", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }

    if (strcmp(ParamName, "EnableMultiProfileXDNS") == 0)
    {
        char buf[5];
        syscfg_get( NULL, "X_RDKCENTRAL-COM_XDNS", buf, sizeof(buf));
        if( buf != NULL && !strcmp(buf,"1") )
        {
                if(!setMultiProfileXdnsConfig(bValue))
                        return FALSE;

                if (syscfg_set_commit(NULL, "MultiProfileXDNS", bValue ? "1" : "0") != 0)
                {
                        AnscTraceWarning(("[XDNS] syscfg_set MultiProfileXDNS failed!\n"));
                }
        }
        else
        {
                CcspTraceError(("XDNS Feature is not Enabled. so,EnableMultiProfileXDNS set operation to %d failed \n",bValue));
                return FALSE;
        }

        return TRUE;

    }
#endif
#if defined (_XB6_PRODUCT_REQ_)
    if (strcmp(ParamName, "BLERadio") == 0)
    {
       BLE_Status_e status;
       if(bValue == TRUE)
       {
          status = BLE_ENABLE;
          CcspTraceInfo(("***BLE_ENABLED***\n"));
       }
       else
       {
          status = BLE_DISABLE;
          CcspTraceInfo(("***BLE_DISABLED***\n"));
       }
       if( !ble_Enable(status))
       {
          if (syscfg_set_commit(NULL, "BLEEnabledOnBoot", (status == BLE_ENABLE) ? "true" : "false") != 0) 
          {
             AnscTraceWarning(("syscfg_set BLEEnabledOnBoot failed\n"));
          }
          return TRUE;
       }
       else {
            CcspTraceWarning(("%s: ble_Enable failed\n", __func__));
       }
    }
#endif

    if (strcmp(ParamName, "Xupnp") == 0)
    {
       if ( bValue == TRUE)
       {
           if (syscfg_set_commit(NULL, "start_upnp_service", "true") != 0)
           {
               AnscTraceWarning(("syscfg_set start_upnp_service:true failed\n"));
           }
       }
       else
       {
           if (syscfg_set_commit(NULL, "start_upnp_service", "false") != 0)
           {
               AnscTraceWarning(("syscfg_set start_upnp_service:false failed\n"));
           }

		   if(access("/lib/rdk/start-upnp-service", F_OK) == 0)
		   {
			   v_secure_system("/lib/rdk/start-upnp-service stop;killall xcal-device;killall xdiscovery");
		   }
		   else
		   {
			   v_secure_system("systemctl stop xcal-device;systemctl stop xupnp");
		   }

		   if(access("/nvram/output.json", F_OK) == 0)
		   {
			   v_secure_system("rm /nvram/output.json");
		   }
#ifdef CORE_NET_LIB
           interface_down("brlan0:0");
#else
           v_secure_system("ifconfig brlan0:0 down");
#endif /* CORE_NET_LIB */
       }
       return TRUE;
    }
#ifdef _BRIDGE_UTILS_BIN_ 
    if (strcmp(ParamName, "BridgeUtilsEnable") == 0)
    {
        if (syscfg_set_commit(NULL, "bridge_util_enable", (bValue == TRUE) ? "true" : "false") != 0)
        {
            AnscTraceWarning(("syscfg_set bridge_util_enable failed\n"));
            return FALSE;
        }
        return TRUE;
    }
#endif
    if (strcmp(ParamName, "IPModeEnable") == 0)
    {
        if(CCSP_SUCCESS != (PSM_Set_Record_Value2(bus_handle,
                                                 g_Subsystem,
                                                 "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.IPModeEnable",
                                                 ccsp_string,
                                                 bValue ? "1" : "0")))
        {
            CcspTraceError(("Failed to set IPModeEnable \n"));
            return FALSE;
        }
        CcspTraceInfo(("Successfully set IPModeEnable \n"));
        return TRUE;
    }
#if (defined _COSA_INTEL_XB3_ARM_)
    if (strcmp(ParamName, "CMRouteIsolationEnable") == 0)
    {
       CcspTraceInfo(("CM Route Isolation Enable:%s\n",bValue?"true":"false"));
       
       if( CMRt_Isltn_Enable(bValue) == TRUE )
       {
          if (syscfg_set_commit(NULL, "CMRouteIsolation_Enable", bValue ? "true" : "false") != 0)
          {
             AnscTraceWarning(("syscfg_set CMRouteIsolationEnable failed\n"));
	     return FALSE;
          }
          return TRUE;
       }
       else {
            CcspTraceWarning(("%s: CMRouteIsolationEnable failed\n", __func__));
	    return FALSE;
       }
    }
#endif
    return FALSE;
}

#if (defined _COSA_INTEL_XB3_ARM_)
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL CMRt_Isltn_Enable
            (
                BOOL                        bValue
            );

    description:

        This function is called to configure/deconfigure CM Wan0
	Route isolation.

    argument:   BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL CMRt_Isltn_Enable(BOOL status)
{
    /* if Enable is true and wan0 is not configured earlier,
     * handle it here */	
    if (status == TRUE) 
    {
       if (access( "/tmp/wan0_configured", F_OK ) != 0)
       {
	  ipv6_addr_info_t * p_v6addr = NULL;
          int  v6addr_num = 0, i;
	  int rc = -1; 
	  rc = CosaUtilGetIpv6AddrInfo("wan0", &p_v6addr, &v6addr_num);     
	  if (0 == rc)
	  {	  
	     for(i = 0; i < v6addr_num; i++ )
	     {
                if ((p_v6addr[i].scope == IPV6_ADDR_SCOPE_GLOBAL)
		     && (strncmp(p_v6addr[i].v6addr, "fd", 2) != 0) 
		     && (strncmp(p_v6addr[i].v6addr, "fc", 2) != 0))
	        {
		    break;
	        }
	     }
	     if(strlen(p_v6addr[i].v6addr))
             {		     
	        _write_sysctl_file("/proc/sys/net/ipv6/conf/wan0/accept_ra_table", 1024);
#ifdef CORE_NET_LIB
	        rule_add_va_arg("-6 from %s lookup cmwan", p_v6addr[i].v6addr);
	        rule_add("-6 from all oif wan0 table cmwan");
	        route_delete("-6 default dev wan0");
	        v_secure_system( "touch /tmp/wan0_configured ");
#else
	        v_secure_system( "ip -6 rule add from %s lookup cmwan && "
                                 "ip -6 rule add from all oif wan0 table cmwan && "
                                 "ip -6 route del default dev wan0 && "
                                 "touch /tmp/wan0_configured ",p_v6addr[i].v6addr);
#endif /* CORE_NET_LIB */
	     }
	     else
	     {
		CcspTraceError(("WAN0 IPv6 null,Unable to configure route table cmwan\n"));
		if(p_v6addr)
                   free(p_v6addr);
		return FALSE;
	     }

	     if(p_v6addr)
                free(p_v6addr);

	  }
	  else
          {
                /* ??? we can't be empty, either the box management is ipv4 only or
                rare conditon of wan0 is empty. can't taken any action for now
                */
                CcspTraceError(("WAN0 IPv6 empty,Unable to configure route table cmwan\n"));
                return FALSE;
	   }
	}
	else
	  CcspTraceInfo(("CM Route Isolation already configured"));

        /* check to ensure, we are properly configured, partial configuration 
	 * may lead to indefinite behaviour,revert to original state
	 * and return false if not properly configured*/  
	if (access( "/tmp/wan0_configured", F_OK ) != 0)
	{
           CcspTraceError(("CM Route Isolation Enable failed for wan0,Reset tables and rules\n"));		
	   _write_sysctl_file("/proc/sys/net/ipv6/conf/wan0/accept_ra_table", 254);
#ifdef CORE_NET_LIB
           rule_delete("-6 lookup cmwan");
           rule_delete("-6 from all oif wan0 lookup cmwan");
           route_delete("-6 default dev wan0 table cmwan");
           v_secure_system("sysevent set wan-stop && "
                           "sysevent set wan-start ");
#else
           v_secure_system("ip -6 rule del lookup cmwan && "
			   "ip -6 rule del from all oif wan0 lookup cmwan && "
                           "ip -6 route del default dev wan0 table cmwan && "
                           "sysevent set wan-stop && "
                           "sysevent set wan-start ");
#endif /* CORE_NET_LIB */
	   return FALSE;
	 }
	 else
	  return TRUE;
     }
     else /*disable case*/
     {
        if (access( "/tmp/wan0_configured", F_OK ) == 0)
    	{
           /* After deleting and adding back to the original table, default
	      route for wan0 come as second entry, do restart wan at end to bring 
	      back to original state
	   */		
           CcspTraceInfo(("Reset Route table and routing rules for wan0\n"));
	   _write_sysctl_file("/proc/sys/net/ipv6/conf/wan0/accept_ra_table", 254);
	   v_secure_system("ip -6 rule del from all oif wan0 lookup cmwan && "
                           "ip -6 rule del lookup cmwan && "
			   "ip -6 route del default dev wan0 table cmwan && "
                           "rm /tmp/wan0_configured && "
			   "sysevent set wan-stop && "
			   "sysevent set wan-start ");
    	}
	return TRUE;
     }
}
#endif

#define BS_SOURCE_WEBPA_STR "webpa"
#define BS_SOURCE_RFC_STR "rfc"

char * getRequestorString()
{
   switch(g_currentWriteEntity)
   {
      case DSLH_MPA_ACCESS_CONTROL_WEBPA:
      case DSLH_MPA_ACCESS_CONTROL_XPC:
         return BS_SOURCE_WEBPA_STR;

      case DSLH_MPA_ACCESS_CONTROL_CLI:
      case DSLH_MPA_ACCESS_CONTROL_CLIENTTOOL:
         return BS_SOURCE_RFC_STR;

      default:
         return "unknown";
   }
}

char * getTime()
{
    time_t timer;
    static char buffer[50];
    struct tm tm_info = {0};
    time(&timer);
    localtime_r(&timer,&tm_info);
    strftime(buffer, 50, "%Y-%m-%d %H:%M:%S ", &tm_info);
    return buffer;
}

BOOL
SyndicationFlowControl_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL SyndicatonFlowControl = (PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL)&(pMyObject->SyndicatonFlowControl);

    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
    errno_t   rc = -1;

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

    if (strcmp(ParamName, "Enable") == 0)
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, SyndicatonFlowControl->Enable.UpdateSource);

        if(CosaDmlDiSet_SyndicationFlowControl_Enable(bValue) == 0)
        {
            SyndicatonFlowControl->Enable.ActiveValue =bValue;

            rc = STRCPY_S_NOCLOBBER( SyndicatonFlowControl->Enable.UpdateSource, sizeof(SyndicatonFlowControl->Enable.UpdateSource), requestorStr );
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }

            char *value = ( bValue ==TRUE ) ?  "true" : "false";
            char PartnerID[PARTNER_ID_LEN] = {0};
            if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
                UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl.Enable",PartnerID, value, requestorStr, currentTime);

            return TRUE;
        }
    }
    return FALSE;
}

BOOL
SyndicationFlowControl_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    CcspTraceWarning(("\nSyndicationFlowControl_SetParamStringValue\n"));
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL SyndicatonFlowControl = (PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL)&(pMyObject->SyndicatonFlowControl);

    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
    errno_t rc = -1;
    int ind = -1;

    if((ParamName == NULL) || (pString == NULL) || (requestorStr == NULL))
        return FALSE;

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

    char PartnerID[PARTNER_ID_LEN] = {0};
    getPartnerId(PartnerID);

    rc = strcmp_s("InitialForwardedMark", strlen("InitialForwardedMark"), ParamName, &ind);
    ERR_CHK(rc);
    if((rc == EOK) && (!ind))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, SyndicatonFlowControl->InitialForwardedMark.UpdateSource);

        /* pString needs to be passed rather than SyndicatonFlowControl->InitialForwardedMark as the CosaDmlDiSet_SyndicationFlowControl_InitialForwardedMark accept character pointer as parameter */
       /* Validation of pString length is necessary for buffer overflow issues while updating to Data Model*/
        if((strlen(pString) < sizeof(SyndicatonFlowControl->InitialForwardedMark.ActiveValue)) && (CosaDmlDiSet_SyndicationFlowControl_InitialForwardedMark((void*)pString) == 0))
        {
            rc = STRCPY_S_NOCLOBBER(SyndicatonFlowControl->InitialForwardedMark.ActiveValue, sizeof(SyndicatonFlowControl->InitialForwardedMark.ActiveValue), pString);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }

            rc = STRCPY_S_NOCLOBBER(SyndicatonFlowControl->InitialForwardedMark.UpdateSource, sizeof(SyndicatonFlowControl->InitialForwardedMark.UpdateSource), requestorStr);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }

            if (PartnerID[ 0 ] != '\0')
                UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl.InitialForwardedMark",PartnerID, pString, requestorStr, currentTime);

            return TRUE;
        }
    }
    rc = strcmp_s("InitialOutputMark", strlen("InitialOutputMark"), ParamName, &ind);
    ERR_CHK(rc);
    if((rc == EOK) && (!ind))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, SyndicatonFlowControl->InitialOutputMark.UpdateSource);

        if((strlen(pString) < sizeof(SyndicatonFlowControl->InitialOutputMark.ActiveValue)) && (CosaDmlDiSet_SyndicationFlowControl_InitialOutputMark((void*)pString)==0))
        {
            rc = STRCPY_S_NOCLOBBER(SyndicatonFlowControl->InitialOutputMark.ActiveValue, sizeof(SyndicatonFlowControl->InitialOutputMark.ActiveValue), pString);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }

            rc = STRCPY_S_NOCLOBBER(SyndicatonFlowControl->InitialOutputMark.UpdateSource, sizeof(SyndicatonFlowControl->InitialOutputMark.UpdateSource), requestorStr);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
            if (PartnerID[ 0 ] != '\0')
                UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl.InitialOutputMark",PartnerID, pString, requestorStr, currentTime);

            return TRUE;
        }
    }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        EncryptCloudUpload_SetParamBoolValue
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
EncryptCloudUpload_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, EncryptCloudUpload_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        syscfg_set_commit(NULL, "encryptcloudupload", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        UploadLogsOnUnscheduledReboot_SetParamBoolValue
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
UploadLogsOnUnscheduledReboot_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, UploadLogsOnUnscheduledReboot_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Disable") == 0)
    {
        syscfg_set_commit(NULL, "UploadLogsOnUnscheduledRebootDisable", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MEMSWAP_SetParamBoolValue
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
MEMSWAP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, MEMSWAP_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
       int retPsmGet = CCSP_SUCCESS;

       retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.MEMSWAP.Enable", ccsp_string, bValue ? "1" : "0");
       if (retPsmGet != CCSP_SUCCESS) {
           CcspTraceError(("Set failed for MEMSWAP support \n"));
           return FALSE;
       }
       CcspTraceInfo(("Successfully set MEMSWAP support \n"));
       return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DNSSTRICTORDER_SetParamBoolValue
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
DNSSTRICTORDER_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, DNSSTRICTORDER_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        syscfg_set_commit(NULL, "DNSStrictOrder", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ShortsDL_GetParamBoolValue
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
ShortsDL_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char buf[8];

        /* collect value */
        /* CID: 114926 Array compared against 0*/
       if(!syscfg_get( NULL, "ShortsDL", buf, sizeof(buf)))
       {
        if (strcmp(buf, "true") == 0)
            *pBool = TRUE;
        else
            *pBool = FALSE;
       } else
          return FALSE;

        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ShortsDL_SetParamBoolValue
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
ShortsDL_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "ShortsDL", bValue ? "true" : "false") != 0)
        {
            CcspTraceError(("syscfg_set ShortsDLEnabled failed\n"));
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SSIDPSWDCTRL_GetParamBoolValue
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
SSIDPSWDCTRL_GetParamBoolValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL*                       pBool
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];
    memset(buf, 0, sizeof(buf));
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "SnmpEnable") == 0)
    {
        /* collect value */
        /* CID: 54616 Array compared against 0*/
        if(!syscfg_get( NULL, "SNMPPSWDCTRLFLAG", buf, sizeof(buf)))
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }
    if (strcmp(ParamName, "Tr069Enable") == 0)
    {
        /* collect value */
        syscfg_get( NULL, "TR069PSWDCTRLFLAG", buf, sizeof(buf));
        /* CID 54616 fix*/
        if( buf[0] != '\0' )
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SSIDPSWDCTRL_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       bValue
            );

    description:

        This function is called to set Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       bValue
                The buffer with updated value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
SSIDPSWDCTRL_SetParamBoolValue

(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, SSIDPSWDCTRL_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "SnmpEnable") == 0)
    {
        syscfg_set_commit(NULL, "SNMPPSWDCTRLFLAG", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }
    if (strcmp(ParamName, "Tr069Enable") == 0)
    {
        syscfg_set_commit(NULL, "TR069PSWDCTRLFLAG", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AutoExcluded_SetParamBoolValue
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
AutoExcluded_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "AutoExcludedEnabled", bValue ? "true" : "false") != 0)
        {
            CcspTraceError(("syscfg_set AutoExcluded failed\n"));
            return FALSE;
        }
        else
        {
            CcspTraceInfo(("syscfg_set AutoExcluded.Enable success new value '%s'\n", bValue ? "true" : "false"));
            return TRUE;
        }
    }
    CcspTraceWarning(("Unsupported parameter AutoExcluded.'%s'\n", ParamName));
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AutoExcluded_GetParamBoolValue
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
AutoExcluded_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char buf[8];

        /* collect value */
        /* CID: 128899 Array compared against 0*/
        if(!syscfg_get( NULL, "AutoExcludedEnabled", buf, sizeof(buf)))
        {
          if (strcmp(buf, "true") == 0)
            *pBool = TRUE;
          else
            *pBool = FALSE;
        } else 
             return FALSE;

        return TRUE;
    }
    CcspTraceWarning(("Unsupported parameter AutoExcluded.'%s'\n", ParamName));
    return FALSE;
}

/**********************************************************************
    caller:     owner of this object
    prototype:
        BOOL
        AutoExcluded_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue
            );
    description:
        This function is called to retrieve string parameter value;
    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;
                char*                       ParamName,
                The parameter name;
                char*                       pValue
                The string value buffer;
    return:     TRUE if succeeded;
                FALSE if not supported.
**********************************************************************/
ULONG
AutoExcluded_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
)
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* Required for xPC sync */
    if (strcmp(ParamName, "XconfUrl") == 0)
    {
        /* collect value */
        char buf[64];
        errno_t  rc = -1;

        /* CID: 128900 Array compared against 0
           CID: 128898 Logically dead code*/
        if(!syscfg_get( NULL, "AutoExcludedURL", buf, sizeof(buf)))
        {
            rc = strcpy_s(pValue, *pUlSize, buf);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        return -1;
    }
    CcspTraceWarning(("Unsupported parameter AutoExcluded.'%s'\n", ParamName));
    return -1;
}

/**********************************************************************
    caller:     owner of this object
    prototype:
        BOOL
        AutoExcluded_SetParamStringValue
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
    AutoExcluded_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
)
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* Required for xPC sync */
    if (strcmp(ParamName, "XconfUrl") == 0)
    {
        if (syscfg_set_commit(NULL, "AutoExcludedURL", pString) != 0)
        {
            CcspTraceError(("syscfg_set failed\n"));
        }
        else
        {
            CcspTraceInfo(("syscfg_set AutoExcluded.XconfUrl success new value '%s'\n", pString));
            return TRUE;
        }
    }
    CcspTraceWarning(("Unsupported parameter AutoExcluded.'%s'\n", ParamName));
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PeriodicFWCheck_GetParamBoolValue
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
PeriodicFWCheck_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */

	if (strcmp(ParamName, "Enable") == 0)
		{
			/* collect value */
			char buf[8];
                        /* CID: 54518 Array compared against 0*/
			if(!syscfg_get( NULL, "PeriodicFWCheck_Enable", buf, sizeof(buf)))
			{
				if (strcmp(buf, "true") == 0)
					*pBool = TRUE;
				else
					*pBool = FALSE;
			}
			return TRUE;
		}

    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PeriodicFWCheck_SetParamBoolValue
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
PeriodicFWCheck_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
	    if (IsBoolSame(hInsContext, ParamName, bValue, PeriodicFWCheck_GetParamBoolValue))
	        return TRUE;

 	    if (strcmp(ParamName, "Enable") == 0)
		{
			/* collect value */
			if( bValue == TRUE)
			{
					syscfg_set_commit(NULL, "PeriodicFWCheck_Enable", "true");
					v_secure_system("/etc/firmwareSched.sh &");
			}
			else
			{
					syscfg_set_commit(NULL, "PeriodicFWCheck_Enable", "false");
					v_secure_system("sh /etc/firmwareSched.sh RemoveCronJob");
					v_secure_system("killall -9 firmwareSched.sh");
			}
			return TRUE;
		}	
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        AllowOpenPorts_GetParamBoolValue
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
AllowOpenPorts_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
        {
            /* collect value */
            char buf[8];
            /* CID: 63052 Array compared against 0*/
            if(!syscfg_get( NULL, "RFCAllowOpenPorts", buf, sizeof(buf)))
            {
                if (strcmp(buf, "true") == 0)
                    *pBool = TRUE;
                else
                    *pBool = FALSE;
               return TRUE;
            } else 
               return FALSE;
        }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AllowOpenPorts_SetParamBoolValue
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
AllowOpenPorts_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
        if (IsBoolSame(hInsContext, ParamName, bValue, AllowOpenPorts_GetParamBoolValue))
            return TRUE;

        if (strcmp(ParamName, "Enable") == 0)
        {
            /* collect value */
            syscfg_set_commit(NULL, "RFCAllowOpenPorts", (bValue == TRUE) ? "true" : "false");

            // Log that we are allowing open ports (or not)
            CcspTraceWarning(("RFC_AllowOpenPorts set to '%s'\n", (bValue == TRUE ? "true":"false")));

            // restart firewall
            v_secure_system("sysevent set firewall-restart");
#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
      if(id!=0)
       {
		   char *lxcevt = "sysevent set firewall-restart";
                   send(sock ,  lxcevt, strlen(lxcevt) , 0 );
       }
#endif
            return TRUE;
        }
    return FALSE;
}

/*********************************************************************************************

//BLUEZ RFC :: Box will run with RCP firmware, if this is enabled. Else run with NCP firmware

**********************************************************************************************

   caller: owner of this object

   prototype:

       BOOL
       BLUEZ_GetParamBoolValue
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
BLUEZ_GetParamBoolValue
(
ANSC_HANDLE                 hInsContext,
char*                       ParamName,
BOOL*                       pBool
)
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if(access("/nvram/bluez_enable", F_OK) != -1)
        {
            *pBool = TRUE;
        }
        else
        {
            *pBool = FALSE;
        }

        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BLUEZ_SetParamBoolValue
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
BLUEZ_SetParamBoolValue
(
ANSC_HANDLE                 hInsContext,
char*                       ParamName,
BOOL                        bValue
)
{
    if (IsBoolSame(hInsContext, ParamName, bValue, BLUEZ_GetParamBoolValue))
        return TRUE;
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (bValue == 1)
        {
            v_secure_system("touch /nvram/bluez_enable");
            CcspTraceInfo(("Successfully enabled Bluez \n"));
            return TRUE;
        }
        else if (bValue == 0)
        {
            v_secure_system("rm /nvram/bluez_enable");
            CcspTraceInfo(("Successfully disabled Bluez \n"));
            return TRUE;
        }
    }
    return FALSE;
}

/**********************************************************************  

//RBUS RFC :: Box will run in DBUS mode if this if disabled, RBUS mode if enabled

***********************************************************************

   caller: owner of this object

   prototype:

       BOOL
       RBUS_GetParamBoolValue
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
RBUS_GetParamBoolValue
(
ANSC_HANDLE                 hInsContext,
char*                       ParamName,
BOOL*                       pBool
)
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if(RBUS_ENABLED == rbus_checkStatus())
            *pBool = TRUE;
        else
            *pBool = FALSE;

        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SNMP_GetParamBoolValue
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
SNMP_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "V3Support") == 0)
    {
        char value[8];
        /*CID: 72231 Array compared against 0*/
        if(!syscfg_get(NULL,"V3Support",value, sizeof(value)))
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        } else 
           return FALSE;

        return TRUE;
    }
    if (strcmp(ParamName, "V2Support") == 0)
    {
        char value[8];
        /*CID:72231 Array compared against 0*/
        if(!syscfg_get(NULL,"V2Support",value, sizeof(value)))
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        } else 
            return FALSE;

        return TRUE;
    }
    if (strcmp(ParamName, "RestartMaintenanceEnable") == 0)
    {
        char value[8] = { 0 };
        /* CID: 72231 Array compared against 0*/
        if(!syscfg_get(NULL,"SNMP_RestartMaintenanceEnable",value, sizeof(value)))
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        } else 
            return FALSE;

        return TRUE;
    }

    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SNMP_SetParamBoolValue
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
SNMP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, SNMP_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "V3Support") == 0)
    {
        syscfg_set_commit(NULL, "V3Support", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }
    if (strcmp(ParamName, "V2Support") == 0)
    {
        syscfg_set_commit(NULL, "V2Support", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }
    if (strcmp(ParamName, "RestartMaintenanceEnable") == 0)
    {
        syscfg_set_commit(NULL, "SNMP_RestartMaintenanceEnable", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TDK_GetParamBoolValue
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
TDK_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8];
        memset(value, 0, sizeof(value));
        syscfg_get(NULL,"TDKEnable",value, sizeof(value));
        /*CID 164170 fix*/
        if( value[0] != '\0' )
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TDK_SetParamBoolValue
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
TDK_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "Enable") == 0)
    {
        if ( bValue == TRUE)
        {
            if(syscfg_set_commit(NULL, "TDKEnable", "true") != 0)
	    {
                AnscTraceWarning(("TDKEnable : Enabling TDK using syscfg_set failed!!!\n"));
            }
        }
        else
        {
            if(syscfg_set_commit(NULL, "TDKEnable", "false") != 0)
            {
                AnscTraceWarning(("TDKEnable : Disabling TDK using syscfg_set failed!!!\n"));
            }
        }
        
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Collectd_GetParamBoolValue
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
Collectd_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {0};
        /*CID: 253423 - Array Compared against null - fixed*/
        syscfg_get(NULL,"CollectdEnable",value, sizeof(value));
        /*CID: 173704 - Array Compared against null - fixed*/
        if( value[0] != '\0' )
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        COllectd_SetParamBoolValue
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
Collectd_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "Enable") == 0)
    {
        if(syscfg_set_commit(NULL, "CollectdEnable", (bValue == TRUE) ? "true" : "false") != 0)
        {
            AnscTraceWarning(("CollectdEnable : Enabling Collectd using syscfg_set failed!!!\n"));
        }

        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        WANLinkHeal_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
WANLinkHeal_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
  UNREFERENCED_PARAMETER(hInsContext);
  if (strcmp(ParamName, "Enable") == 0)
    {
      char value[8];

      if(syscfg_get(NULL,"wanlinkheal",value, sizeof(value)) == 0)
      {
	if( value[0]!='\0')
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        }
        return TRUE;
      }
      else
      {
	CcspTraceError(("syscfg_get failed for WanLinkHeal\n"));
      }
    }
  return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        WANLinkHeal_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
		BOOL                        bValue
            )


    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

		BOOL                        bValue
		The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
WANLinkHeal_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
	if (syscfg_set_commit(NULL, "wanlinkheal", bValue ? "true" : "false") != 0)
	{
		CcspTraceError(("syscfg_set wanlinkhealEnabled failed\n"));
	}
	else
	{
		return TRUE;
	}
     }
 return FALSE;
}
/**********************************************************************  

    caller:     owner of this object

    prototype: 

        BOOL
        IPv6subPrefix_GetParamBoolValue
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
IPv6subPrefix_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
        {
            /* collect value */
            char buf[8];
            memset(buf, 0, sizeof(buf));
            syscfg_get( NULL, "IPv6subPrefix", buf, sizeof(buf));
            /*CID 62757 fix*/
            if( buf[0] != '\0' )
            {
                if (strcmp(buf, "true") == 0)
                    *pBool = TRUE;
                else
                    *pBool = FALSE;
            }
            return TRUE;
        }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6subPrefix_SetParamBoolValue
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
IPv6subPrefix_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
        if (IsBoolSame(hInsContext, ParamName, bValue, IPv6subPrefix_GetParamBoolValue))
            return TRUE;

        if (strcmp(ParamName, "Enable") == 0)
        {
            syscfg_set_commit(NULL, "IPv6subPrefix", (bValue == TRUE) ? "true" : "false");
            return TRUE;
        }
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6onLnF_GetParamBoolValue
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
IPv6onLnF_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
        {
            /* collect value */
            char buf[128];
	    char Inf_name[32];

            memset(Inf_name, 0, sizeof(Inf_name));
            syscfg_get( NULL, "iot_brname", Inf_name, sizeof(Inf_name));
            if ( (Inf_name[0] == '\0') && (strlen(Inf_name)) == 0 )
            {
                syscfg_get( NULL, "iot_ifname", Inf_name, sizeof(Inf_name));

            }
        /* CID 71221 fix*/
	    if( Inf_name[0] != '\0' )
            {
              memset(buf, 0, sizeof(buf));
              syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf));

		    if( buf[0] != '\0' )
		    {
		        if (strstr(buf, Inf_name))
		            *pBool = TRUE;
		        else
		            *pBool = FALSE;
		    }
		    return TRUE;
	    }
        }

    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        WebUI_GetParamUlongValue
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
WebUI_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8] = {0};

    if (strcmp(ParamName, "Enable") == 0)
    {
        syscfg_get( NULL, "WebUIEnable", buf, sizeof(buf));
        if( 0 == strlen(buf) )
        {
             *puLong = 1; // default value of WebUIEnable
        }
        else
        {
             *puLong = atol(buf);
        }
        return TRUE;
    }
    AnscTraceWarning(("%s is invalid argument!\n", ParamName));
    return FALSE;
}
    
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        WebUI_SetParamUlongValue
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
WebUI_SetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG                       uValue
)
{

    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
	if (syscfg_set_u_commit(NULL, "WebUIEnable", uValue) != 0)
	{
		CcspTraceWarning(("syscfg_set failed to set WebUIEnable \n"));
		return FALSE;
	}
	if(uValue == 0 || uValue == 2)
	{
		CosaDmlDiSet_DisableRemoteManagement();
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
        WebUIRemoteMgtOption_GetParamBoolValue
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
WebUIRemoteMgtOption_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {0};
	/* collect value */
        if (syscfg_get(NULL,"WebUIRemoteMgtOptionEnable",value, sizeof(value)) == 0)
        {
             if (strncmp(value, "true", sizeof(value)) == 0)
                     *pBool = TRUE;
             else
                     *pBool = FALSE;
        }
        else
        {
             CcspTraceError(("%s syscfg_get failed  for WebUIRemoteMgtOptionEnable\n",__FUNCTION__));
             *pBool = TRUE;
        }
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WebUIRemoteMgtOption_SetParamBoolValue
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
WebUIRemoteMgtOption_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, WebUIRemoteMgtOption_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "WebUIRemoteMgtOptionEnable", (bValue == TRUE) ? "true" : "false") != 0)
        {
            CcspTraceError(("[%s] syscfg_set failed for RemoteMgtOptionEnable\n",__FUNCTION__));
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SWDLDirect_GetParamBoolValue_GetParamBoolValue
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
SWDLDirect_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {0};
        /* collect value */
        if (syscfg_get(NULL,"SWDLDirectEnable",value, sizeof(value)) == 0)
        {
             if (strncmp(value, "true", sizeof(value)) == 0)
                     *pBool = TRUE;
             else
                     *pBool = FALSE;
        }
        else
        {
             CcspTraceError(("%s syscfg_get failed  for SWDLDirectEnable\n",__FUNCTION__));
             *pBool = TRUE;
        }
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SWDLDirect_SetParamBoolValue
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
SWDLDirect_SetParamBoolValue
   (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, SWDLDirect_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if(syscfg_set(NULL, "SWDLDirectEnable", (bValue==TRUE)?"true":"false") != 0)
        {
            CcspTraceError(("[%s] syscfg_set failed for SWDLDirectEnable\n",__FUNCTION__));
            return FALSE;
        }
        if (syscfg_commit() != 0)
        {
                AnscTraceWarning(("syscfg_commit failed for SWDLDirectEnable param update\n"));
                return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        CognitiveMotionDetection_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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

#ifdef FEATURE_COGNITIVE_WIFIMOTION
BOOL
CognitiveMotionDetection_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    char buf[6] = { 0 };

    UNREFERENCED_PARAMETER(hInsContext);
    /* RDKB-38634: TR-181 implementation
     * DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.CognitiveMotionDetection.Enable
     */
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_get(NULL, COGNITIVE_WIFIMOTION_CFG, buf, sizeof(buf)) != 0)
        {
            *pBool = FALSE;
            return TRUE;
        }

        if (!strncmp(buf, "true", 4))
        {
            *pBool = TRUE;
        }
        else if (!strncmp(buf, "false", 5))
        {
            *pBool = FALSE;
        }
        else
        {
            CcspTraceWarning(("syscfg_get: value of %s is invalid!\n", COGNITIVE_WIFIMOTION_CFG));
            return FALSE;
        }

        return TRUE;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        CognitiveMotionDetection_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
CognitiveMotionDetection_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    char buf[6] = { 0 };

    UNREFERENCED_PARAMETER(hInsContext);
    /* RDKB-38634: TR-181 implementation
     * DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.CognitiveMotionDetection.Enable
     */
    if (strcmp(ParamName, "Enable") == 0)
    {
        char *value = (bValue == TRUE) ? "true" : "false";

        syscfg_get(NULL, COGNITIVE_WIFIMOTION_CFG, buf, sizeof(buf));

        if (!strncmp(buf, value, strlen(value)))
        {
            return TRUE;
        }

        if (syscfg_set_commit(NULL, COGNITIVE_WIFIMOTION_CFG, value) != 0)
        {
            CcspTraceWarning(("syscfg_set failed to set %s\n", COGNITIVE_WIFIMOTION_CFG));
            return FALSE;
        }

        if (bValue == TRUE)
        {
            v_secure_system("systemctl start systemd-cognitive_wifimotion.service");
        }
        else
        {
            v_secure_system("systemctl stop systemd-cognitive_wifimotion.service");
        }

        return TRUE;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}
#endif // FEATURE_COGNITIVE_WIFIMOTION

#ifdef USE_REMOTE_DEBUGGER
/**********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.RDKRemoteDebugger.Enable

    *  RDKRemoteDebugger_SetParamBoolValue // Set args required for RemoteDebugger
    *  RDKRemoteDebugger_GetParamBoolValue // Get args set for RemoteDebugger

    caller:     owner of this object

    prototype:
        BOOL
        RDKRemoteDebugger_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
RDKRemoteDebugger_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    char buf[6] = { 0 };

    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_get(NULL, "RemoteDebuggerEnabled", buf, sizeof(buf)) != 0)
        {
            *pBool = FALSE;
            return TRUE;
        }

        if (!strncmp(buf, "true", 4))
        {
            *pBool = TRUE;
        }
        else if (!strncmp(buf, "false", 5))
        {
            *pBool = FALSE;
        }
        else
        {
            CcspTraceWarning(("%s: syscfg_get: value of RemoteDebuggerEnabled invalid!\n", __FUNCTION__));
             return FALSE;
        }

        return TRUE;
    }

    CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __FUNCTION__, ParamName));
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        RDKRemoteDebugger_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
RDKRemoteDebugger_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    char buf[6] = { 0 };

    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char *value = (bValue == TRUE) ? "true" : "false";

        syscfg_get(NULL, "RemoteDebuggerEnabled", buf, sizeof(buf));

        if (!strncmp(buf, value, strlen(value)))
        {
            return TRUE;
        }

        if (syscfg_set_commit(NULL, "RemoteDebuggerEnabled", value) != 0)
        {
            CcspTraceWarning(("%s: syscfg_set failed on RemoteDebuggerEnabled\n", __FUNCTION__));
            return FALSE;
        }

        if (bValue == TRUE)
        {
            v_secure_system("systemctl restart remote-debugger.service");
        }
        else
        {
            v_secure_system("systemctl stop remote-debugger.service");
        }

        return TRUE;
    }

    CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __FUNCTION__, ParamName));
    return FALSE;
}

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_RDKDownloadManager.InstallPackage

    *  RDKDownloadManager_SetParamStringValue

    caller:     owner of this object

    prototype:
        BOOL
        RDKDownloadManager_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            )


    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pstring
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
RDKDownloadManager_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "InstallPackage") == 0 && pString != NULL)
    {
    int ret =-1;
    CcspTraceWarning(("[%s] Entering..\n", __FUNCTION__ ));

    if((!pString) || strlen(pString) == 0 ) {
        CcspTraceWarning(("[%s] Invalid parameter value\n", __FUNCTION__));
        return FALSE;
    }

    CcspTraceWarning(("[%s] Executing command - rdm -v %s & \n", __FUNCTION__, pString));

    ret = v_secure_system("/usr/bin/rdm -v \"%s\" >> /rdklogs/logs/rdm_status.log 2>&1 &", pString);

    if (ret != 0) {
        CcspTraceWarning(("[%s] Failed to execute the command. Returned error code '%d'\n", __FUNCTION__, ret));
        return FALSE;
    }

    CcspTraceWarning(("[%s] Exiting..\n", __FUNCTION__ ));
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

#endif

#ifdef FEATURE_ONEWIFI_TESTSUITE
/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        OneWiFiTestSuite_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
#define ONEWIFI_TESTSUITE_CFG "onewifi_testsuite"

BOOL
OneWiFiTestSuite_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    char buf[6] = { 0 };

    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_get(NULL, ONEWIFI_TESTSUITE_CFG, buf, sizeof(buf)) != 0)
        {
            *pBool = FALSE;
            return TRUE;
        }

        if (!strncmp(buf, "true", 4))
        {
            *pBool = TRUE;
        }
        else if (!strncmp(buf, "false", 5))
        {
            *pBool = FALSE;
        }
        else
        {
            CcspTraceWarning(("syscfg_get: value of %s is invalid!\n", ONEWIFI_TESTSUITE_CFG));
            return FALSE;
        }

        return TRUE;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        OneWiFiTestSuite_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
OneWiFiTestSuite_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    char buf[6] = { 0 };

    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char *value = (bValue == TRUE) ? "true" : "false";

        syscfg_get(NULL, ONEWIFI_TESTSUITE_CFG, buf, sizeof(buf));

        if (!strncmp(buf, value, strlen(value)))
        {
            return TRUE;
        }

        if (syscfg_set_commit(NULL, ONEWIFI_TESTSUITE_CFG, value) != 0)
        {
            CcspTraceWarning(("syscfg_set failed to set %s\n", COGNITIVE_WIFIMOTION_CFG));
            return FALSE;
        }

        if (bValue == TRUE)
        {
            v_secure_system("systemctl restart rdkfmac.service");
        }
        else
        {
            v_secure_system("systemctl stop rdkfmac.service");
        }

        return TRUE;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}
#endif

 #if defined (FEATURE_SUPPORT_INTERWORKING)
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFiInterworking_GetParamBoolValue
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
WiFiInterworking_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
	/* Collect Value */
	char *strValue = NULL;
	int retPsmGet = CCSP_SUCCESS;

	retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WiFi-Interworking.Enable", NULL, &strValue);
	if (retPsmGet == CCSP_SUCCESS) {
	    *pBool = _ansc_atoi(strValue);
	    ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
	}
	else
	    *pBool = FALSE;
	return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFiInterworking_SetParamBoolValue
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
WiFiInterworking_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, WiFiInterworking_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
	int retPsmGet = CCSP_SUCCESS;
	errno_t rc = -1;

        if(bValue == FALSE)
	{
		CcspTraceError(("turn off WiFiInterworkingSupport \n"));
		int ret = -1;
		int size = 0;
		componentStruct_t ** ppComponents = NULL;
		char* faultParam = NULL;
		char dst_pathname_cr[64]  =  {0};

		CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

		rc = sprintf_s(dst_pathname_cr,  sizeof(dst_pathname_cr),"%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
		if(rc < EOK)
		{
			ERR_CHK(rc);
			return FALSE;
		}

		ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
				dst_pathname_cr,
				"Device.WiFi.X_CISCO_COM_ResetRadios",
				g_Subsystem,        /* prefix */
				&ppComponents,
				&size);

		if ( ret == CCSP_SUCCESS && size == 1)
		{
			parameterValStruct_t    val[] = { 
				{ "Device.WiFi.AccessPoint.1.X_RDKCENTRAL-COM_InterworkingServiceEnable", "false", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.2.X_RDKCENTRAL-COM_InterworkingServiceEnable", "false", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.5.X_RDKCENTRAL-COM_InterworkingServiceEnable", "false", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.6.X_RDKCENTRAL-COM_InterworkingServiceEnable", "false", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.9.X_RDKCENTRAL-COM_InterworkingServiceEnable", "false", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.10.X_RDKCENTRAL-COM_InterworkingServiceEnable", "false", ccsp_boolean},	
				{ "Device.WiFi.AccessPoint.1.X_RDKCENTRAL-COM_InterworkingApplySettings", "true", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.2.X_RDKCENTRAL-COM_InterworkingApplySettings", "true", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.5.X_RDKCENTRAL-COM_InterworkingApplySettings", "true", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.6.X_RDKCENTRAL-COM_InterworkingApplySettings", "true", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.9.X_RDKCENTRAL-COM_InterworkingApplySettings", "true", ccsp_boolean},
				{ "Device.WiFi.AccessPoint.10.X_RDKCENTRAL-COM_InterworkingApplySettings", "true", ccsp_boolean},

			};

			ret = CcspBaseIf_setParameterValues
				(
				 bus_handle,
				 ppComponents[0]->componentName,
				 ppComponents[0]->dbusPath,
				 0, 0x0,
				 val,
				 12,
				 TRUE,
				 &faultParam
				);

			if (ret != CCSP_SUCCESS && faultParam)
			{
				AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
				bus_info->freefunc(faultParam);
			}
			free_componentStruct_t(bus_handle, size, ppComponents);
		}
        }

	retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WiFi-Interworking.Enable", ccsp_string, bValue ? "1" : "0");
	if (retPsmGet != CCSP_SUCCESS) {
	    CcspTraceError(("Set failed for WiFiInterworkingSupport \n"));
	    return FALSE;
	}
	if(bValue == 0) {
        /* CID 334909 Logically dead code fix */
		retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WiFi-Passpoint.Enable", ccsp_string, "0");
        if (retPsmGet != CCSP_SUCCESS) {
			CcspTraceError(("Set failed for WiFiPasspointSupport \n"));
			return FALSE;
		}
	}

	CcspTraceInfo(("Successfully set WiFiInterworkingSupport \n"));
	return TRUE;
    }
    return FALSE;
}
#endif

#if defined (FEATURE_SUPPORT_PASSPOINT)
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFiPasspoint_GetParamBoolValue
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
WiFiPasspoint_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
	/* Collect Value */
	char *strValue = NULL;
	int retPsmGet = CCSP_SUCCESS;

	retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WiFi-Passpoint.Enable", NULL, &strValue);
	if (retPsmGet == CCSP_SUCCESS) {
	    *pBool = _ansc_atoi(strValue);
	    ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
	}
	else
	    *pBool = FALSE;
	return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFiPasspoint_SetParamBoolValue
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
WiFiPasspoint_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    int interworking = FALSE;
    if (strcmp(ParamName, "Enable") == 0)
    {
		int retPsmGet = CCSP_SUCCESS;
		char *strValue = NULL;

		retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WiFi-Interworking.Enable", NULL, &strValue);
		if (retPsmGet == CCSP_SUCCESS) {
			interworking = _ansc_atoi(strValue);
			((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
 		}
		else
			interworking = FALSE;

		if(interworking == FALSE) {
 			CcspTraceError(("Passpoint cannot be enabled when interworking is disabled.\n"));
			return FALSE;
		}

		retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WiFi-Passpoint.Enable", ccsp_string, bValue ? "1" : "0");
		if (retPsmGet != CCSP_SUCCESS) {
			CcspTraceError(("Set failed for WiFiPasspointSupport \n"));
			return FALSE;
    	}
   	CcspTraceInfo(("Successfully set WiFiPasspointSupport \n"));
#ifndef RDK_ONEWIFI
    if(bValue == FALSE){
        int ret = -1;
        int size = 0;
        componentStruct_t ** ppComponents = NULL;
        char* faultParam = NULL;
        char dst_pathname_cr[64]  =  {0};
        errno_t rc = -1;

        CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

        rc = sprintf_s(dst_pathname_cr, sizeof(dst_pathname_cr),"%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }

        ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                dst_pathname_cr,
                "Device.WiFi.X_CISCO_COM_ResetRadios",
                g_Subsystem,        /* prefix */
                &ppComponents,
                &size);

        if ( ret == CCSP_SUCCESS && size == 1)
        {
            parameterValStruct_t    val[] = { { "Device.WiFi.AccessPoint.9.X_RDKCENTRAL-COM_Passpoint.Enable", "false", ccsp_boolean}, { "Device.WiFi.AccessPoint.10.X_RDKCENTRAL-COM_Passpoint.Enable", "false", ccsp_boolean} };

            ret = CcspBaseIf_setParameterValues
                (
                 bus_handle,
                 ppComponents[0]->componentName,
                 ppComponents[0]->dbusPath,
                 0, 0x0,
                 val,
                 2,
                 TRUE,
                 &faultParam
                );

            if (ret != CCSP_SUCCESS && faultParam)
            {
                AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                bus_info->freefunc(faultParam);
            }
            free_componentStruct_t(bus_handle, size, ppComponents);
        }
    }
#else
        parameterValStruct_t pVal[1];
        char                 paramName[256] = "Device.WiFi.WiFi-Passpoint";
        char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
        char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
        char*                faultParam     = NULL;
        int                  ret            = 0;
        CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
           pVal[0].parameterName  = paramName;
           pVal[0].parameterValue = bValue ? "true" : "false";
           pVal[0].type           = ccsp_boolean;
    
           ret = CcspBaseIf_setParameterValues(
                     bus_handle,
                     compName,
                     dbusPath,
                     0,
                     0,
                     pVal,
                     1,
                     TRUE,
                     &faultParam
                 );
    
           if (ret != CCSP_SUCCESS)
           {
               CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
               bus_info->freefunc(faultParam);
               return FALSE;
           }
#endif
    return TRUE;
    }
    return FALSE;
}
#endif


#if defined (FEATURE_OFF_CHANNEL_SCAN_5G)
BOOL
WiFiOffChannelScan_GetParamBoolValue
(ANSC_HANDLE                hInsContext,
 char *                     ParamName,
 BOOL *                     pBool
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* Collect Value */
        char *strValue = NULL;
        int retPsmGet = CCSP_SUCCESS;

        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDK_RFC.Feature.WiFiOffChannelScan.Enable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS)
        {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
        {
            *pBool = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL
WiFiOffChannelScan_SetParamBoolValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0) {
        int retPsmGet = CCSP_SUCCESS;

#ifdef RDK_ONEWIFI
        parameterValStruct_t pVal[1];
        char                 paramName[256] = "Device.WiFi.WiFi-OffChannelScan-APP";
        char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
        char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
        char*                faultParam     = NULL;
        int                  ret            = 0;
        CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
           pVal[0].parameterName  = paramName;
           pVal[0].parameterValue = bValue ? "true" : "false";
           pVal[0].type           = ccsp_boolean;

           ret = CcspBaseIf_setParameterValues(
                     bus_handle,
                     compName,
                     dbusPath,
                     0,
                     0,
                     pVal,
                     1,
                     TRUE,
                     &faultParam
                 );

           if (ret != CCSP_SUCCESS)
           {
               CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
               bus_info->freefunc(faultParam);
               return FALSE;
           }
#endif
            retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDK_RFC.Feature.WiFiOffChannelScan.Enable", ccsp_string, bValue ? "1" : "0");
            if (retPsmGet != CCSP_SUCCESS) {
                CcspTraceError(("Set failed for app Off Channel Scan RFC\n"));
                return FALSE;
            }
            CcspTraceInfo(("Successfully set app Off Channel Scan RFC\n"));
            return TRUE;
    }
    return FALSE;
}
#endif // (FEATURE_OFF_CHANNEL_SCAN_5G)
BOOL
off_channel_scan_GetParamBoolValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL*                       pBool
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* Collect Value */
        char *strValue = NULL;
        int retPsmGet = CCSP_SUCCESS;

        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDK_RFC.Feature.OffChannelScan.Enable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS)
        {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
        {
            *pBool = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL
off_channel_scan_SetParamBoolValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        int retPsmGet = CCSP_SUCCESS;

        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDK_RFC.Feature.OffChannelScan.Enable", ccsp_string, bValue ? "1" : "0");
        if (retPsmGet != CCSP_SUCCESS)
        {
            CcspTraceError(("Set failed for 5G off channel scan RFC  \n"));
            return FALSE;
        }
        CcspTraceInfo(("Successfully set 5g off channel scan RFC \n"));

        v_secure_system("sysevent set wifi_OffChannelScanEnable %s", bValue ? "true" : "false");

#ifdef RDK_ONEWIFI
        parameterValStruct_t pVal[1];
        char                 paramName[256] = "Device.WiFi.WiFi-OffChannelScan";
        char                 compName[256]  = "eRT.com.cisco.spvtg.ccsp.wifi";
        char                 dbusPath[256]  = "/com/cisco/spvtg/ccsp/wifi";
        char*                faultParam     = NULL;
        int                  ret            = 0;
        CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
           pVal[0].parameterName  = paramName;
           pVal[0].parameterValue = bValue ? "true" : "false";
           pVal[0].type           = ccsp_boolean;

           ret = CcspBaseIf_setParameterValues(
                     bus_handle,
                     compName,
                     dbusPath,
                     0,
                     0,
                     pVal,
                     1,
                     TRUE,
                     &faultParam
                 );

           if (ret != CCSP_SUCCESS)
           {
               CcspTraceError(("%s - %d - Failed to notify WiFi component - Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
               bus_info->freefunc(faultParam);
               return FALSE;
           }
#endif

        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFiPsmDb_GetParamBoolValue
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
WiFiPsmDb_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
//#if defined(FEATURE_SUPPORT_WIFIDB)
    if (strcmp(ParamName, "Enable") == 0)
    {
	/* Collect Value */
	char *strValue = NULL;
	int retPsmGet = CCSP_SUCCESS;

	retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WiFi-PSM-DB.Enable", NULL, &strValue);
	if (retPsmGet == CCSP_SUCCESS) {
	    *pBool = _ansc_atoi(strValue);
	    ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
	}
	else
	    *pBool = FALSE;
	return TRUE;
    }

    return FALSE;
/*#else
    CcspTraceInfo(("WiFi-PSM-DB not supported \n"));
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    return FALSE;
#endif*/
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFiPsmDb_SetParamBoolValue
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
WiFiPsmDb_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(FEATURE_SUPPORT_WIFIDB)
    int   ret   = 0;
    if (strcmp(ParamName, "Enable") == 0)
    {
	int retPsmGet = CCSP_SUCCESS;

	retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WiFi-PSM-DB.Enable", ccsp_string, bValue ? "1" : "0");
	if (retPsmGet != CCSP_SUCCESS) {
	    CcspTraceError(("Set failed for WiFi-PSM-DB \n"));
	    return FALSE;
	}
	CcspTraceInfo(("Successfully set WiFi-PSM-DB \n"));
        ret = CcspBaseIf_SendSignal_WithData(bus_handle, "WifiDbStatus", bValue ? "1" : "0");
        if ( ret != CCSP_SUCCESS ) {
            CcspTraceError(("%s : WifiDbStatus send data failed,  ret value is %d\n",__FUNCTION__ ,ret));
        }
	return TRUE;
    }
    return FALSE;
#else
    CcspTraceInfo(("WiFi-PSM-DB not supported \n"));
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
    return FALSE;
#endif
}

#if defined (FEATURE_SUPPORT_RADIUSGREYLIST)
/**********************************************************************
    caller:     owner of this object

    prototype:

	BOOL
	RadiusGreyList_GetParamBoolValue
	    (
		ANSC_HANDLE                 hInsContext,
		char*                       ParamName,
		BOOL*                       pBool
	    );

     description:

	This function is called to retrieve Boolean parameter value;

     argument:  ANSC_HANDLE                 hInsContext,
		The instance handle;

		char*                       ParamName,
		The parameter name;

		BOOL*                       pBool
		The buffer of returned boolean value;

     return:     TRUE if succeeded.
**********************************************************************/

BOOL
RadiusGreyList_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* Collect Value */
        char *strValue = NULL;
        int retPsmGet = CCSP_SUCCESS;
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.RadiusGreyList.Enable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = FALSE;
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

	BOOL
	RadiusGreyList_SetParamBoolValue
	    (
		ANSC_HANDLE                 hInsContext,
		char*                       ParamName,
		BOOL                        bValue
	    );

     description:

	This function is called to set BOOL parameter value;

     argument:  ANSC_HANDLE                 hInsContext,
		The instance handle;

		char*                       ParamName,
		The parameter name;

		BOOL                        bValue
		The updated BOOL value;

     return:     TRUE if succeeded.
**********************************************************************/
BOOL
RadiusGreyList_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue,RadiusGreyList_GetParamBoolValue)) {
         CcspTraceError(("[%s] set RadiusGreyListEnable [%s]\n",__FUNCTION__, (bValue ? "TRUE" : "FALSE")))
	return TRUE;
    }
    if (strcmp(ParamName, "Enable") == 0)
    {
        return CosaDmlSetRadiusGreyListEnable(bValue);
    }
    return FALSE;
}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6onLnF_SetParamBoolValue
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
IPv6onLnF_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
        char *token = NULL;char *pt;

        if (IsBoolSame(hInsContext, ParamName, bValue, IPv6onLnF_GetParamBoolValue))
            return TRUE;

        if (strcmp(ParamName, "Enable") == 0)
        {
     	    char buf[128], OutBuff[128];
	    char Inf_name[32];
	    BOOL bFound = FALSE;
	    errno_t rc = -1;
	
            memset(buf,0,sizeof(buf));
            memset(OutBuff,0,sizeof(OutBuff));
            memset(Inf_name, 0, sizeof(Inf_name));
 	    syscfg_get( NULL, "iot_brname", Inf_name, sizeof(Inf_name));
            if ( (Inf_name[0] == '\0') && (strlen(Inf_name)) == 0 )
            {
             	syscfg_get( NULL, "iot_ifname", Inf_name, sizeof(Inf_name));
            
       	    }
	    /* CID 64488 fix*/
	    if( Inf_name[0] != '\0' )
            {
            	    syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf));

		    if( buf[0] != '\0' )
		    {
                        if (strstr(buf, Inf_name))
		            bFound = TRUE;
		        else
		            bFound = FALSE;


			if(bValue)
	                {
					if(bFound == FALSE)
					{
					// interface is not present in the list, we need to add interface to enable IPv6 PD

							rc = sprintf_s(OutBuff, sizeof(OutBuff),"%s%s,",buf,Inf_name);
							if(rc < EOK)
							{
								ERR_CHK(rc);
								return FALSE;
							}
							syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);

					}
			}
			else
			{
				
					if(bFound == TRUE)
					{
					// interface is present in the list, we need to remove interface to disable IPv6 PD
						pt = buf;
						   while((token = strtok_r(pt, ",", &pt))) {
							if(strncmp(Inf_name,token,strlen(Inf_name)))
							{
                                /* CID 53147 Calling risky function : fix */
                                strncat(OutBuff,token,CALC_SPACE_LEFT(OutBuff));
                                strncat(OutBuff,",",CALC_SPACE_LEFT(OutBuff));
							}

						   }
					
						syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
					}
			}
		    }
		    else
			{
				if(bValue)
				{
				strncat(OutBuff,Inf_name,CALC_SPACE_LEFT(OutBuff));
				strncat(OutBuff,",",CALC_SPACE_LEFT(OutBuff));
				syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
				}
			}
		    return TRUE;
	    } else  
               return FALSE;

            return TRUE;
        }
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6onXHS_GetParamBoolValue
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
IPv6onXHS_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        char buf[128];
        char *Inf_name = NULL;
        char *token = NULL;
        char *pt = NULL;
        int retPsmGet = CCSP_SUCCESS;
        *pBool = FALSE;

        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.2.Port.1.Name", NULL, &Inf_name);
        if (retPsmGet == CCSP_SUCCESS) {
            if( Inf_name != NULL )
            {
                /* CID: 68662 Array compared against 0*/
                if(!syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf)))
                {
                    pt = buf;
                    while ((token = strtok_r(pt, ",", &pt)))
                    {
                        if (strcmp(Inf_name, token) == 0) {
                            *pBool = TRUE;
                            break;
                        }
                    }
                }

                ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
            }
        }

        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6onXHS_SetParamBoolValue
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
IPv6onXHS_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
 char *token = NULL;char *pt;

 if (IsBoolSame(hInsContext, ParamName, bValue, IPv6onXHS_GetParamBoolValue))
        return TRUE;

 if (strcmp(ParamName, "Enable") == 0)
 {
     char buf[128], OutBuff[128];
     char *Inf_name = NULL;
     BOOL bFound = FALSE;
     int retPsmGet = CCSP_SUCCESS;
     errno_t rc = -1;

     retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.2.Port.1.Name", NULL, &Inf_name);
     if (retPsmGet == CCSP_SUCCESS)
     {
         memset(buf,0,sizeof(buf));
         memset(OutBuff,0,sizeof(OutBuff));

         if( Inf_name != NULL )
         {

              /*CID: 65587 Array compared against 0*/
              /* CID: 53214 Logically dead code*/
              if(!syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf)))
              {
                  char *ptrBuf = strdup(buf);
		  if(ptrBuf == NULL)
			  return FALSE;
		  pt = buf;

                  while ((token = strtok_r(pt, ",", &pt)))
                  {
                      if (strcmp(Inf_name, token) == 0)
                      {
                          bFound = TRUE;
                          break;
                      }
                  }
                  if(bValue)
                  {
                      if(bFound == FALSE)
                      {
                          // interface is not present in the list, we need to add interface to enable IPv6 PD

			  CcspTraceDebug(("ptrBuf: %s\n", ptrBuf));
                          CcspTraceDebug(("buf: %s\n", buf));
                          CcspTraceDebug(("Inf_name: %s\n", Inf_name));
                          rc = snprintf(OutBuff,sizeof(OutBuff),"%s%s,",ptrBuf,Inf_name);
                          CcspTraceDebug(("OutBuff: %s\n", OutBuff));
                          if(rc < EOK)
                          {
                              ERR_CHK(rc);
			      free(ptrBuf);
                              return FALSE;
                          }
                          syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
                      }
                  }
                  else
                  {

                      if(bFound == TRUE)
                      {
                          // interface is present in the list, we need to remove interface to disable IPv6 PD
                          pt = buf;
                          while((token = strtok_r(pt, ",", &pt))) 
                          {
                              if(strcmp(Inf_name, token) != 0)
                              {
                                  strncat(OutBuff,token,CALC_SPACE_LEFT(OutBuff));
                                  strncat(OutBuff,",",CALC_SPACE_LEFT(OutBuff));
                              }

                          }

                          syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
                      }
                  }
		  free(ptrBuf);
              }
              else
              {
                  if(bValue)
                  {
                      strncat(OutBuff,Inf_name,CALC_SPACE_LEFT(OutBuff));
                      strncat(OutBuff,",",CALC_SPACE_LEFT(OutBuff));
                      syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
                  }
              }
              ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
              return TRUE;
          }
          ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
      }
      return TRUE;
    }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6onPOD_GetParamBoolValue
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
IPv6onPOD_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
        {
            /* collect value */
            char buf[128] = {0};
            char *Inf_name = NULL;
    	    int retPsmGet = CCSP_SUCCESS;

            retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.10.Name", NULL, &Inf_name);
			//Setting the interface name, in case it is null.
			if(!retPsmGet) {
				retPsmGet = CCSP_SUCCESS;
				Inf_name = "br403";		
			}
            if (retPsmGet == CCSP_SUCCESS) {
	    	if( Inf_name != NULL )
            	{
                    /*CID: 172844 - Array Compared against null - fixed*/
               	    syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf));
                    /*CID: 172805 - Array Compared against null - fixed*/
	        if( buf[0] != '\0' )
		    {
		        if (strstr(buf, Inf_name))
		            *pBool = TRUE;
		        else
		            *pBool = FALSE;
		    }
		    ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
		    return TRUE;
	        }
            }
            else
            *pBool = FALSE;
        }

    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6onPOD_SetParamBoolValue
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
IPv6onPOD_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
 char *token = NULL;char *pt;

 if (IsBoolSame(hInsContext, ParamName, bValue, IPv6onPOD_GetParamBoolValue))
        return TRUE;

 if (strcmp(ParamName, "Enable") == 0)
        {
     	    char buf[128] = {0}; 
            char OutBuff[128];
			char *Inf_name = NULL;
			BOOL bFound = FALSE;
    	    int retPsmGet = CCSP_SUCCESS;

            retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.10.Name", NULL, &Inf_name);
			if(!retPsmGet)
			{
				retPsmGet = CCSP_SUCCESS;
				Inf_name = "br403";		
			}
            if (retPsmGet == CCSP_SUCCESS)
				{
                    memset(OutBuff,0,sizeof(OutBuff));

					if( Inf_name != NULL )
						{
						syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf));
                                                /*CID: 172805 - Array Compared against null - fixed*/
						if( buf[0] != '\0' )
						{
							if (strstr(buf, Inf_name))
								bFound = TRUE;
							else
								bFound = FALSE;


							if(bValue)
							{
								if(bFound == FALSE)
								{
								// interface is not present in the list, we need to add interface to enable IPv6 PD
                                                                                // CID 175253 : Buffer not null terminated (BUFFER_SIZE)
										strncpy(OutBuff, buf, sizeof(OutBuff)-1);
										/* CID 172842 - Calling risky function (DC.STRING_BUFFER) */
										strncat(OutBuff,Inf_name,CALC_SPACE_LEFT(OutBuff));
										strncat(OutBuff,",",CALC_SPACE_LEFT(OutBuff));
										syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);

								}
							}
							else
							{

								if(bFound == TRUE)
								{
								// interface is present in the list, we need to remove interface to disable IPv6 PD
									pt = buf;
									   while((token = strtok_r(pt, ",", &pt))) {
										if(strncmp(Inf_name,token,strlen(Inf_name)))
										{
											strncat(OutBuff,token,CALC_SPACE_LEFT(OutBuff));
											strncat(OutBuff,",",CALC_SPACE_LEFT(OutBuff));
										}
									   }

									syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
								}
							}
						}
						else
						{
							if(bValue)
							{
							strncat(OutBuff,Inf_name,CALC_SPACE_LEFT(OutBuff));
							strncat(OutBuff,",",CALC_SPACE_LEFT(OutBuff));
							syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
							}
						}
					    ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
						return TRUE;
					}
				   ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
            }
            return TRUE;
        }
    return FALSE;
}
#if !defined (NO_MOCA_FEATURE_SUPPORT)
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6onMoCA_GetParamBoolValue
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
IPv6onMoCA_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
	    UNREFERENCED_PARAMETER(hInsContext);

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
        {			
		/* collect value */
		char buf[128] = {0};
                /*CID: 173704 - Array Compared against null - fixed*/
		syscfg_get( NULL, "ipv6_moca_bridge", buf, sizeof(buf));
                /*CID: 173704 - Array Compared against null - fixed*/
			if( buf[0] != '\0' )
			{
				if (strcmp(buf, "true") == 0)
					*pBool = TRUE;
				else
					*pBool = FALSE;
			}
		return TRUE;
		}
     return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6onMoCA_SetParamBoolValue
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
IPv6onMoCA_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    char *token = NULL;char *pt;

    if(IsBoolSame(hInsContext, ParamName, bValue, IPv6onMoCA_GetParamBoolValue)) {
        return TRUE;
    }


    if (strcmp(ParamName, "Enable") == 0)
    {
        char buf[128] = {0}; 
        char OutBuff[128];
        char *Inf_name = NULL;
        char *str = NULL;
        int HomeIsolationEnable = 0;
        BOOL bFound = FALSE;
        int retPsmGet, retPsmGet1 = CCSP_SUCCESS;

        if (syscfg_set( NULL, "ipv6_moca_bridge", bValue ? "true" : "false") != 0) {
            CcspTraceError(("syscfg_set failed for ipv6_moca_bridge\n"));
            return -1;
        }

        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.9.Name", NULL, &Inf_name);
        retPsmGet1 = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.HomeNetworkIsolation", NULL, &str);
        if(!retPsmGet)
        {
            retPsmGet = CCSP_SUCCESS;
            Inf_name = "brlan10";		
        }

        if(retPsmGet1 == CCSP_SUCCESS) {	
            HomeIsolationEnable = _ansc_atoi(str);
        }

        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(str);
        if (retPsmGet == CCSP_SUCCESS)
        {
            memset(OutBuff,0,sizeof(OutBuff));

            if( Inf_name != NULL )
            {
                syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf));
                /*CID: 173697 - Array Compared against null - fixed*/
                if( buf[0] != '\0' )
                {
                    if (strstr(buf, Inf_name))
                        bFound = TRUE;
                    else
                        bFound = FALSE;


                    if(bValue)
                    {								
                        if((bFound == FALSE) && (HomeIsolationEnable == 1))
                        {
                            CcspTraceWarning((">>>>Debug Entered  : %s at line : %d  \n",__FUNCTION__, __LINE__ ));

                            // interface is not present in the list, we need to add interface to enable IPv6 PD
                            strncpy(OutBuff, buf, sizeof(buf));
                            strncat(OutBuff,Inf_name,CALC_SPACE_LEFT(OutBuff));
                            strncat(OutBuff,",",CALC_SPACE_LEFT(OutBuff));

                            CcspTraceWarning((">>>>Debug 1 Value of  OutBuff : %s infname  : %s  HomeIsolationEnable: %d \n", OutBuff, Inf_name, HomeIsolationEnable ));
                            syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
                        }
                    }
                    else
                    {
                        if(bFound == TRUE)
                        {
                            // interface is present in the list, we need to remove interface to disable IPv6 PD
                            pt = buf;
                            while((token = strtok_r(pt, ",", &pt))) {
                                if(strncmp(Inf_name,token,strlen(Inf_name)))
                                {
                                    strncat(OutBuff,token,CALC_SPACE_LEFT(OutBuff));
                                    strncat(OutBuff,",",CALC_SPACE_LEFT(OutBuff));
                                }
                            }
                            CcspTraceWarning((">>>>Debug 2 Value of  OutBuff : %s infname  : %s  HomeIsolationEnable: %d \n", OutBuff, Inf_name, HomeIsolationEnable ));
                            syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
                        }
                    }
                }
                else
                {
                    if((bValue) && (HomeIsolationEnable == 1))
                    {
                        strncat(OutBuff,Inf_name,CALC_SPACE_LEFT(OutBuff));
                        /* CID Calling risky function : fix */
                        strncat(OutBuff, ",", CALC_SPACE_LEFT(OutBuff));
                        syscfg_set_commit(NULL, "IPv6_Interface",OutBuff);
                        CcspTraceWarning((">>>>Debug 3 Value of  OutBuff : %s infname  : %s  HomeIsolationEnable: %d \n", OutBuff, Inf_name, HomeIsolationEnable ));
                    }
                }
                ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
                v_secure_system("sysevent set zebra-restart");
                return TRUE;
            }
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
        }
        return TRUE;
    }
    return FALSE;	
}
#endif
/**********************************************************************  

    caller:     owner of this object

    prototype: 

        BOOL
        EvoStream_DirectConnect_GetParamBoolValue
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
EvoStream_DirectConnect_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
        {
            /* collect value */
            char buf[8] = {0};
            syscfg_get( NULL, "EvoStreamDirectConnect", buf, sizeof(buf));

            if( buf[0] != '\0')
            {
                if (strcmp(buf, "true") == 0)
                    *pBool = TRUE;
                else
                    *pBool = FALSE;
            }
            return TRUE;
        }

    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        EvoStream_DirectConnect_SetParamBoolValue
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
EvoStream_DirectConnect_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
        if (IsBoolSame(hInsContext, ParamName, bValue, EvoStream_DirectConnect_GetParamBoolValue))
            return TRUE;

        if (strcmp(ParamName, "Enable") == 0)
        {
            /* collect value */
            if(syscfg_set_commit(NULL, "EvoStreamDirectConnect", (bValue == TRUE) ? "true" : "false") != 0)
		{
			CcspTraceError(("EvoStreamDirectConnect :%d Failed to SET\n", bValue ));
		}
		else
		{
			CcspTraceInfo(("EvoStreamDirectConnect :%d Success\n", bValue ));
			v_secure_system("sysevent set firewall-restart");
		}
            return TRUE;
        }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Xconf_GetParamBoolValue
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
Xconf_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

	if (strcmp(ParamName, "xconfCheckNow") == 0)
	{
		*pBool = FALSE;
		return TRUE;
	}

        if (strcmp(ParamName, "xconfCheckNowReboot") == 0)
        {
                *pBool = FALSE;
                return TRUE;
        }
	return FALSE;

}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        RDKFirmwareUpgrader_GetParamBoolValue
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
RDKFirmwareUpgrader_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char buf[8];

        /* collect value */
        memset(buf, 0, sizeof(buf));
        syscfg_get( NULL, "RDKFirmwareUpgraderEnabled", buf, sizeof(buf));
        /* CID 108106 fix*/
        if( buf[0] != '\0' )
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        RDKFirmwareUpgrader_SetParamBoolValue
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
RDKFirmwareUpgrader_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "RDKFirmwareUpgraderEnabled", bValue ? "true" : "false") != 0)
        {
            CcspTraceError(("syscfg_set RDKFirmwareUpgraderEnabled failed\n"));
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Xconf_SetParamBoolValue
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
Xconf_SetParamBoolValue

(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    int status = 0;
    /* check the parameter name and set the corresponding value */
    if ((strcmp(ParamName, "xconfCheckNow") == 0) || (strcmp(ParamName, "xconfCheckNowReboot") == 0))
    {
        AnscTraceWarning(("Triggering firmware download check from TR181\n"));
        if( TRUE == bValue )
        {
            /* RDKB-29712 : Firmware Download scripts will not trigger download
             * if the device is waiting for maintenance window. ".waitingreboot"
             * file removed to force re-download */
            FILE *file = fopen( "/tmp/.waitingreboot","r" );
            if (file != NULL) {
                AnscTraceWarning(("XConf is waiting for reboot after download. Removing /tmp/.waitingreboot to force redownload image"));
                fclose(file);                
                /* CID: 155187 fix*/
                if (remove("/tmp/.waitingreboot") != 0)
                {
                    CcspTraceWarning(("%s: Error deleting the file\n", __FUNCTION__));
                }
            }
 
            file = fopen( "/tmp/.downloadingfw","r" );
            if (file != NULL) {
                AnscTraceWarning(("Removing /tmp/.downloadingfw to force redownload image\n"));
                fclose(file);
                if (remove("/tmp/.downloadingfw") != 0)
                {
                    CcspTraceWarning(("%s: Error deleting the file\n", __FUNCTION__));
                }

                AnscTraceInfo(("Killing XconfHttpDl process and its child process, if running\n"));
                //Kill XconfHttpDl process and its child process, if running
                char ps_cmd[] = "pidof XconfHttpDl";
                char buffer[1024];
                int pids[10];
                int num_pids = 0;

                FILE *fp = popen(ps_cmd, "r");
                if (fp != NULL) {
                    memset(buffer, 0, sizeof(buffer));
                    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                        char *token = strtok(buffer, " ");
                        while (token != NULL) {
                            pids[num_pids++] = atoi(token);
                            token = strtok(NULL, " ");
                        }
                    }
                    pclose(fp);
                }

                for (int i = 0; i < num_pids; i++) {
                    int pstree_pids[20];
                    char pstree_cmd[50] = {0};
                    int num_pstree_pids = 0;

                    snprintf(pstree_cmd, sizeof(pstree_cmd), "pstree -p %d | grep -o '[0-9]\\+' | tr '\\n' ' '", pids[i]);
                    FILE *pstree_fp = popen(pstree_cmd, "r");
                    memset(buffer, 0, sizeof(buffer));
                    if (pstree_fp != NULL) {
                        if (fgets(buffer, sizeof(buffer), pstree_fp) != NULL) {
                            char *token = strtok(buffer, " ");
                            while (token != NULL) {
                                pstree_pids[num_pstree_pids++] = atoi(token);
                                token = strtok(NULL, " ");
                            }
                        }
                        pclose(pstree_fp);
                    }

                    for (int i = num_pstree_pids - 1; i >= 0; i--) {
                         kill(pstree_pids[i], SIGKILL);
                    }
                }
            }

            // static collection as we don't want upgrade path to be changed without a reboot
            static BOOL rdkfwupgraderEnabledCollected = false;
            static BOOL RDKFWUpgraderEnabled = false;

            if (!rdkfwupgraderEnabledCollected) {
                if (RDKFirmwareUpgrader_GetParamBoolValue(hInsContext,
                                                          "Enable",
                                                          &RDKFWUpgraderEnabled) ) {
                    rdkfwupgraderEnabledCollected = true;
                }
            }

	    // NOTE:: this might have an addional issue, rfc enabled but rdkfwupgrader was disabled at 
            // build time. Do we need to take care of that like checking for presense /usr/bin/rdkfwupgrader
	    // at runtime 
            if (RDKFWUpgraderEnabled) {
                // FIXME:: call RDKFWUpgrader's dbus api. As of now we are calling using
                // dbus-send but we can bring in codegen generated proxy and call it using that.
                // and get rid of these system calls. But this is supposed to stay for short term only
                // not wasting time on this.
                AnscTraceWarning(("Triggering firmware download check using RDKFirmwareUpgrader TR181\n"));
                v_secure_system("/lib/rdk/rdkfwupgrader_check_now.sh &");
            } else {
                // NOTE:: Firmwaresched.sh used to check for reboot pending before killing, this one doesn't
                // leaving a note behind if it comes out to be a problem
#if defined(_RDKB_GLOBAL_PRODUCT_REQ_)
		char acBoxType[128] = {0};

		if( ( 0 == CheckAndGetDevicePropertiesEntry( acBoxType, sizeof( acBoxType ),"BOX_TYPE" ) ) ||
                    ( acBoxType[0] != '\0' ) )
		{
                    char acFwDwldScriptName[512] = {0};

		    AnscStrLower(acBoxType);

		    snprintf(acFwDwldScriptName, sizeof(acFwDwldScriptName), "%s_firmwareDwnld.sh",acBoxType);

            	    if ( 0 == v_secure_system("pidof %s", acFwDwldScriptName) )  
		    {
                       v_secure_system ("kill -9 `pidof %s `", acFwDwldScriptName);
                    }

                    status = v_secure_system("/etc/%s &", acFwDwldScriptName);

                    CcspTraceInfo(("Xconf firmware download script:%s \n",(status == 0) ? "sucessfully started" : "failed to start"));
		}
#else
#if defined(INTEL_PUMA7) || defined(_COSA_BCM_ARM_) || defined(_COSA_QCA_ARM_)
#ifdef _CBR_PRODUCT_REQ_
            if(0 == v_secure_system("pidof cbr_firmwareDwnld.sh"))  {
                           v_secure_system ("kill -9 `pidof cbr_firmwareDwnld.sh `");
                        }
                        if((strcmp(ParamName, "xconfCheckNow") == 0)) {
                           status = v_secure_system("/etc/cbr_firmwareDwnld.sh &");
                        }
                        else if((strcmp(ParamName, "xconfCheckNowReboot") == 0)) {
                           status = v_secure_system("/etc/cbr_firmwareDwnld.sh 7 1 &");
                        }
#elif defined(_SR213_PRODUCT_REQ_)
            if(0 == v_secure_system("pidof sr213_firmwareDwnld.sh"))  {
                           v_secure_system ("kill -9 `pidof sr213_firmwareDwnld.sh `");
                       }
                       if((strcmp(ParamName, "xconfCheckNow") == 0)) {
                           status = v_secure_system("/etc/sr213_firmwareDwnld.sh &");
                       }
                       else if((strcmp(ParamName, "xconfCheckNowReboot") == 0)) {
                           status = v_secure_system("/etc/sr213_firmwareDwnld.sh 7 1 &");
                       }
#elif defined(_SE501_PRODUCT_REQ_)
                       if(0 == v_secure_system("pidof se501_firmwareDwnld.sh"))  {
                           v_secure_system ("kill -9 `pidof se501_firmwareDwnld.sh `");
                       }
                       if((strcmp(ParamName, "xconfCheckNow") == 0)) {
                           status = v_secure_system("/etc/se501_firmwareDwnld.sh &");
                       }
                       else if((strcmp(ParamName, "xconfCheckNowReboot") == 0)) {
                           status = v_secure_system("/etc/se501_firmwareDwnld.sh 7 1 &");
                       }
#elif defined(_WNXL11BWL_PRODUCT_REQ_)
                       if(0 == v_secure_system("pidof wnxl11bwl_firmwareDwnld.sh"))  {
                           v_secure_system ("kill -9 `pidof wnxl11bwl_firmwareDwnld.sh `");
                       }
                       if((strcmp(ParamName, "xconfCheckNow") == 0)) {
                           status = v_secure_system("/etc/wnxl11bwl_firmwareDwnld.sh &");
                       }
                       else if((strcmp(ParamName, "xconfCheckNowReboot") == 0)) {
                           status = v_secure_system("/etc/wnxl11bwl_firmwareDwnld.sh 7 1 &");
                       }
#elif defined(_XER5_PRODUCT_REQ_)
                       if(0 == v_secure_system("pidof xer5_firmwareDwnld.sh"))  {
                           v_secure_system ("kill -9 `pidof xer5_firmwareDwnld.sh `");
                       }
                       if((strcmp(ParamName, "xconfCheckNow") == 0)) {
                           status = v_secure_system("/etc/xer5_firmwareDwnld.sh &");
                       }
                       else if((strcmp(ParamName, "xconfCheckNowReboot") == 0)) {
                           status = v_secure_system("/etc/xer5_firmwareDwnld.sh 7 1 &");
                       }
#elif defined(_SR300_PRODUCT_REQ_)
                       if(0 == v_secure_system("pidof sr300_firmwareDwnld.sh"))  {
                           v_secure_system ("kill -9 `pidof sr300_firmwareDwnld.sh `");
                       }
                       if((strcmp(ParamName, "xconfCheckNow") == 0)) {
                           status = v_secure_system("/etc/sr300_firmwareDwnld.sh &");
                       }
                       else if((strcmp(ParamName, "xconfCheckNowReboot") == 0)) {
                           status = v_secure_system("/etc/sr300_firmwareDwnld.sh 7 1 &");
                       }
#elif defined(_HUB4_PRODUCT_REQ_)
                       if(0 == v_secure_system("pidof hub4_firmwareDwnld.sh"))  {
                           v_secure_system ("kill -9 `pidof hub4_firmwareDwnld.sh `");
                       }
                       if((strcmp(ParamName, "xconfCheckNow") == 0)) {
                           status = v_secure_system("/etc/hub4_firmwareDwnld.sh &");
                       }
                       else if((strcmp(ParamName, "xconfCheckNowReboot") == 0)) {
                           status = v_secure_system("/etc/hub4_firmwareDwnld.sh 7 1 &");
                       }
#else
                if(0 == v_secure_system("pidof xb6_firmwareDwnld.sh"))  {
                    v_secure_system ("kill -9 `pidof xb6_firmwareDwnld.sh `");
                }
                if((strcmp(ParamName, "xconfCheckNow") == 0)) {
                    status = v_secure_system("/etc/xb6_firmwareDwnld.sh &");
                }
                else if((strcmp(ParamName, "xconfCheckNowReboot") == 0)) {
                    status = v_secure_system("/etc/xb6_firmwareDwnld.sh 7 1 &");
                }
#endif
#elif defined(_COSA_BCM_MIPS_)
                if(0 == v_secure_system("pidof xf3_firmwareDwnld.sh"))  {
                    v_secure_system ("kill -9 `pidof xf3_firmwareDwnld.sh `");
                }
                status = v_secure_system("/etc/xf3_firmwareDwnld.sh &");
#else
                if(0 == v_secure_system("pidof xb3_firmwareDwnld.sh"))  {
                    v_secure_system ("kill -9 `pidof xb3_firmwareDwnld.sh `");
                }
                status = v_secure_system("/etc/xb3_firmwareDwnld.sh &");
#endif
#endif /* _RDKB_GLOBAL_PRODUCT_REQ_ */
                if (0 == status)
                {
                    AnscTraceWarning(("xconf process started successfully\n"));
                }
                else
                {
                    AnscTraceWarning(("xconf process did not start successfully\n"));
                }
            }
            return TRUE;
        }
    }
     return FALSE;
}
/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.ReverseSSH

    *  ReverseSSH_SetParamStringValue // Set args required for reverse SSH
    *  ReverseSSH_GetParamStringValue // Get args set for reverse SSH

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        ReverseSSH_GetParamStringValue
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
ReverseSSH_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char* activeStr = "ACTIVE";
    char* inActiveStr = "INACTIVE";
    errno_t  rc = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "xOpsReverseSshArgs") == 0)
    {
        getXOpsReverseSshArgs(NULL, pValue,pulSize);
        return 0;
    }

    if (strcmp(ParamName, "xOpsReverseSshStatus") == 0)
    {
        if (isRevSshActive()) {
            rc = strcpy_s(pValue, *pulSize, activeStr);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
        } else {
            rc = strcpy_s(pValue, *pulSize, inActiveStr);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }
        }
        return 0;
    }

    if (strcmp(ParamName, "xOpsReverseSshTrigger") == 0)
    {
         rc = strcpy_s(pValue, *pulSize, "");
         if(rc != EOK)
         {
            ERR_CHK(rc);
            return -1;
         }
        return 0;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return -1;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ReverseSSH_SetParamStringValue
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

    return:     TRUE if succeeded,
                FALSE if failed.

**********************************************************************/
BOOL
ReverseSSH_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "xOpsReverseSshArgs") == 0)
    {
        setXOpsReverseSshArgs(pString);
        return TRUE ;
    }

    if (strcmp(ParamName, "xOpsReverseSshTrigger") == 0) {
        setXOpsReverseSshTrigger(pString);
        return TRUE ;

    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));

    return FALSE;
}

#if 0
/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_EthernetWAN.

    *  EthernetWAN_GetParamStringValue

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        EthernetWAN_GetParamStringValue
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

    return:     TRUE if succeeded;
                FALSE if failed

**********************************************************************/

ULONG
EthernetWAN_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pUlSize);
    errno_t     rc = -1;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "CurrentOperationalMode") == 0)
    {
        /* collect value */
        char buf[32];
		
        if( ( 0 == syscfg_get( NULL, "eth_wan_enabled", buf, sizeof( buf ) ) ) && \
			( '\0' != buf[ 0 ] )
		   )
        {
			if( 0 == strcmp( buf, "true" ) )
			{
				rc = strcpy_s( pValue, *pUlSize, "Ethernet");
				if(rc != EOK)
				{
					ERR_CHK(rc);
					return -1;
				}
			}
			else
			{
				rc = strcpy_s( pValue, *pUlSize, "DOCSIS");
				if(rc != EOK)
				{
					ERR_CHK(rc);
					return -1;
				}
			}
        }
        else
        {
            CcspTraceError(("%s syscfg_get failed for eth_wan_enabled. so giving default as DOCSIS\n",__FUNCTION__));
			rc = strcpy_s( pValue, *pUlSize, "DOCSIS");
			if(rc != EOK)
			{
				ERR_CHK(rc);
				return -1;
			}
        }
		
        return 0;
    }

    return -1;
}

#endif
/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_EthernetWAN.MACsec.OperationalStatus

    *  EthernetWAN_MACsec_GetParamStringValue

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        EthernetWAN_MACsec_GetParamStringValue
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
#ifdef _MACSEC_SUPPORT_
ULONG
EthernetWAN_MACsec_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    errno_t  rc = -1;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "OperationalStatus") == 0)
    {
        BOOL flag;

        if ( RETURN_OK == platform_hal_GetMACsecOperationalStatus( ETHWAN_DEF_INTF_NUM, &flag )) {
           rc = strcpy_s(pValue, *pUlSize, ((TRUE == flag) ? "Enabled" : "Disabled") );
           if(rc != EOK)
           {
              ERR_CHK(rc);
              return -1;
           }

           return 0;
        }
    }
    return 0;
}
#endif

/* Maintenance window can be customized for bci routers */


/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_MaintenanceWindow.

    *  MaintenanceWindow_GetParamStringValue
    *  MaintenanceWindow_SetParamStringValue

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MaintenanceWindow_GetParamStringValue
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

    return:     TRUE if succeeded;
                FALSE if failed

**********************************************************************/

BOOL
MaintenanceWindow_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    BOOL bReturnValue = FALSE;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "FirmwareUpgradeStartTime") == 0)
    {
        /* collect value */
        bReturnValue = CosaDmlDiGetFirmwareUpgradeStartTime(NULL, pValue,pulSize);
        return bReturnValue;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "FirmwareUpgradeEndTime") == 0)
    {
        /* collect value */
        bReturnValue = CosaDmlDiGetFirmwareUpgradeEndTime (NULL, pValue,pulSize);
        return bReturnValue;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return bReturnValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MaintenanceWindow_SetParamStringValue
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

    return:     TRUE if succeeded,
                FALSE if failed.

**********************************************************************/
BOOL
MaintenanceWindow_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    BOOL bReturnValue = FALSE;
    ANSC_STATUS retValue = ANSC_STATUS_FAILURE;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "FirmwareUpgradeStartTime") == 0)
    {
        retValue = CosaDmlDiSetFirmwareUpgradeStartTime (pString);
    }

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "FirmwareUpgradeEndTime") == 0)
    {
        retValue = CosaDmlDiSetFirmwareUpgradeEndTime (pString);
    }

    else
    {
        CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    }

    if (retValue == ANSC_STATUS_SUCCESS)
    {
        bReturnValue = TRUE;
    }
    else
    {
        bReturnValue = FALSE;
    }

    return bReturnValue;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CredDwnld_SetParamBoolValue
            (
                 ANSC_HANDLE                 hInsContext,
                 char*                       ParamName,
                 BOOL                        bValue
            )

        description:

                This function is called to retrieve Boolean parameter value;

        argument:
                ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

        return:     TRUE if succeeded.

**********************************************************************/
BOOL
CredDwnld_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        if(syscfg_set_commit(NULL, "CredDwnld_Enable", bValue ? "true" : "false") != 0 )
        {
            CcspTraceError(("syscfg_set failed\n"));
            return FALSE;
        }

#if defined(_COSA_INTEL_XB3_ARM_)
        // To address muliple processor platforms
        v_secure_system("/usr/bin/rpcclient %s '" SYSTEMCTL_CMD "' &", atomIp );
        return TRUE;
#endif
        v_secure_system( "systemctl start lxydnld.service &" );
        return TRUE;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CredDwnld_GetParamBoolValue
            (
                 ANSC_HANDLE                 hInsContext,
                 char*                       ParamName,
                 BOOL*                       pBool
            )

        description:

                This function is called to retrieve Boolean parameter value;

        argument:
                ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

        return:     TRUE if succeeded.

**********************************************************************/
BOOL
CredDwnld_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        char buf[8];
        /* collect value */
        if(syscfg_get(NULL, "CredDwnld_Enable", buf, sizeof(buf)) != 0 )
        {
            CcspTraceError(("syscfg_get failed\n"));
            return FALSE;
        }
        else
        {
            if (strncmp(buf,"true",sizeof(buf)) == 0)
            {
                *pBool = TRUE;
            }
            else
            {
                *pBool = FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        CredDwnld_GetParamStringValue
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

                char*                       pValue
                The buffer of returned string value;

                ULONG*                      pulSize
                The buffer of returned string size;

    return:     TRUE if succeeded.

**********************************************************************/
ULONG
CredDwnld_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    errno_t   rc  = -1;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Use") == 0)
    {
        char buf[MAX_USE_LEN];

        /* collect value */
        if( syscfg_get( NULL, "CredDwnld_Use", buf, sizeof(buf)) != 0)
        {
             CcspTraceError(("syscfg_get failed\n"));
             return FALSE;
        }
        else
        {
            rc = strcpy_s(pValue, *pulSize, buf);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
            *pulSize = AnscSizeOfString(pValue)+1;
            return TRUE;
        }
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CredDwnld_SetParamStringValue
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
CredDwnld_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Use") == 0)
    {
        if (syscfg_set_commit(NULL, "CredDwnld_Use", pString) != 0)
        {
            CcspTraceError(("syscfg_set failed\n"));
            return FALSE;
        }
        else
        {
#if defined(_COSA_INTEL_XB3_ARM_)
            // To address muliple processor platforms
            v_secure_system("/usr/bin/rpcclient %s '" SYSTEMCTL_CMD "' &", atomIp );
            return TRUE;
#endif
            v_secure_system( "systemctl start lxydnld.service &" );
            return TRUE;
        }
    }
    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

/***********************************************************************/





/***********************************************************************/

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.ForwardSSH

    *  ForwardSSH_SetParamStringValue // Set args required for Forward SSH
    *  ForwardSSH_GetParamStringValue // Get args set for Forward SSH

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ForwardSSH_GetParamBoolValue
            (
                 ANSC_HANDLE                 hInsContext,
                 char*                       ParamName,
                 BOOL*                       pBool
            )

        description:

                This function is called to retrieve Boolean parameter value; 

        argument:   
                ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

        return:     TRUE if succeeded.

**********************************************************************/
BOOL
ForwardSSH_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
         /* CID: 91711 Array compared against 0*/
        if(!syscfg_get( NULL, "ForwardSSH", buf, sizeof(buf)))
        {
            if (strcmp(buf,"true") == 0)
            {
                *pBool = TRUE;
            }
            else
            {
                *pBool = FALSE;
            }
        }
        else
        {
            CcspTraceWarning(("%s syscfg_get failed  for ForwardSSH \n",__FUNCTION__));
            *pBool = TRUE;
        }
        return TRUE;
    }
    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ForwardSSH_SetParamBoolValue
            (
                 ANSC_HANDLE                 hInsContext,
                 char*                       ParamName,
                 BOOL*                       bValue
            )

        description:

                This function is called to retrieve Boolean parameter value; 

        argument:   
                ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

        return:     TRUE if succeeded.

**********************************************************************/
BOOL
ForwardSSH_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        syscfg_set_commit(NULL, "ForwardSSH", (bValue == TRUE)? "true" : "false");
        return TRUE;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Logging_GetParamBoolValue
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
Logging_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "xOpsDMRetrieveConfigLogNow") == 0)
    {
        /* collect value */
		*pBool=FALSE;
		return TRUE;
        
    }
	if (strcmp(ParamName, "xOpsDMUploadLogsNow") == 0)
    {
        /* collect value */
		*pBool=FALSE;
		return TRUE;
    }

    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Logging_SetParamBoolValue
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
Logging_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
	BOOL bReturnValue;
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "xOpsDMRetrieveConfigLogNow") == 0)
    {
		if( TRUE == bValue )
		{
			AnscTraceWarning((" Run DCM service\n"));
			v_secure_system("sh /lib/rdk/dcm.service &");
		}

    	return TRUE;
	}
	else if (strcmp(ParamName, "xOpsDMUploadLogsNow") == 0)
	{
		bReturnValue = COSADmlUploadLogsNow((ANSC_HANDLE)NULL, bValue);
		if( !bReturnValue )
		{
			return TRUE;
		}
	}

    return FALSE;
}

ULONG
Logging_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "xOpsDMLogsUploadStatus") == 0)
    {
        /* collect value */
        COSADmlUploadLogsStatus(NULL, pValue,pUlSize);
        return 0;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Logging_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
	        ULONG*                      puLong
            );

    description:

        This function is called to retrieve ulong parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

     		   ULONG*                      puLong
                The buffer of returned ulong value;

    return:     TRUE if succeeded.

**********************************************************************/


BOOL
Logging_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "DmesgLogSyncInterval") == 0)
    {
        /* collect value */
	 char buf[8];

	 syscfg_get(NULL, "dmesglogsync_interval", buf, sizeof(buf));
         *puLong = atoi(buf);
        return TRUE;
    }

	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

BOOL
Logging_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    ) ;
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
Logging_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DmesgLogSyncInterval") == 0)
    {
		if (syscfg_set_u_commit(NULL, "dmesglogsync_interval", uValue) != 0) 
		{
			CcspTraceWarning(("syscfg_set failed to set DmesgLogSyncInterval \n"));
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
        SwitchToDibbler_GetParamBoolValue
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
#if defined(_COSA_INTEL_XB3_ARM_) || defined(INTEL_PUMA7)
BOOL
SwitchToDibbler_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
	/* This Get API is only for XB3,AXB6 devices */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        char buf[8];
        if( syscfg_get( NULL, "dibbler_client_enable_v2", buf, sizeof(buf))==0)
        {
            if (strcmp(buf, "true") == 0)
            {
                *pBool = TRUE;
            }
            else
            {
                *pBool = FALSE;
            }
        }
        else
        {
            CcspTraceWarning(("%s syscfg_get failed  for dibbler_client_enable_v2\n",__FUNCTION__));
            *pBool = FALSE;
        }

        return TRUE;
    }
    return FALSE;
}

void* dhcpSwitchThread(void* vptr_value)
{
        pthread_detach(pthread_self());
        v_secure_system("/etc/dhcpswitch.sh %s &", (char *) vptr_value);
        return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SwitchToDibbler_SetParamBoolValue
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
SwitchToDibbler_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
     /* This set API is only for XB3,AXB6 devices */

    if (strcmp(ParamName, "Enable") == 0)
    {
        char buf[8];
        char event[8];
        pthread_t tid;
        char* operation = NULL;
        int val;

        /* collect previous flag value */
       if( syscfg_get( NULL, "dibbler_client_enable_v2", buf, sizeof(buf)) == 0)
        {
                val = strcmp(buf,"true")?0:1;
                if (val != bValue)
                {
                        if (bValue)
                        {
                                commonSyseventGet("dhcpclient_v4", event, sizeof(event));
                                if (atoi(event) == 1)
                                {
                                        commonSyseventSet("dhcpclient_v6", "1");
                                        AnscTraceWarning(("dhcpclient_v4 event is enabled. So enabling dhcpclient_v6 event \n"));
                                }
                                else
                                {
                                        operation = "schedule_v6_cron";
                                        AnscTraceWarning(("dhcpclient_v4 event is not enabled.scheduling cron \n"));
                                        pthread_create(&tid, NULL, &dhcpSwitchThread, (void *)operation);
                                }
                        }
                        else
                        {
                                operation = "clear_v6_cron";
                                AnscTraceWarning(("dhcp client switching back to default \n"));
                                pthread_create(&tid, NULL, &dhcpSwitchThread, (void *)operation);
                        }
                }
                else if(!bValue)
                {
                        char v6event[8];
                        commonSyseventGet("dhcpclient_v6", v6event, sizeof(v6event));
                        if (atoi(v6event) == 1)
                        {
                                char v4event[8];
                                commonSyseventGet("dhcpclient_v4", v4event, sizeof(v4event));
                                if (atoi(v4event) ==1)
                                {
                                        commonSyseventSet("dhcpclient_v6", "0");
                                        AnscTraceWarning(("dhcpclient_v6 is disabled\n"));
                                }
                                else
                                {
                                        commonSyseventSet("dhcpclient_v6", "0");
                                        v_secure_system("sed -i '/dhcpswitch.sh/d' /var/spool/cron/crontabs/root &");
                                        AnscTraceWarning(("dhcpclient_v6 is disabled and scheduled cron removed\n"));
                                }

                        }
                        else
                        {
                                AnscTraceWarning(("No set operation done since dibbler_client_enable_v2 flag already set to %d\n", bValue));
                        }
                }
                else
                {
                        AnscTraceWarning(("No set operation done since dibbler_client_enable_v2 flag already set to %d\n", bValue));
                }
        }
        else
        {
                AnscTraceWarning(("syscfg_get failed for diibler_client_enable\n"));
                return FALSE;
        }


        return TRUE;
    }
   return FALSE;

}
#endif
/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        CDLDM_GetParamStringValue
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

                char*                       pValue
                The buffer of returned string value;

                ULONG*                      pulSize
                The buffer of returned string size;

    return:     TRUE if succeeded.

**********************************************************************/
ULONG
CDLDM_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_CDLDM pBindObj =      & pMyObject->CdlDM;

    if (strcmp(ParamName, "CDLModuleUrl") == 0)
    {
        return  update_pValue(pValue,pulSize, pBindObj->CDLModuleUrl.ActiveValue);
    }
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CDLDM_SetParamStringValue
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
CDLDM_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_CDLDM      pBindObj =      & pMyObject->CdlDM;

    char PartnerID[PARTNER_ID_LEN] = {0};
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
    errno_t rc = -1;
    int ind = -1;

    if((ParamName == NULL) || (pString == NULL))
        return FALSE;

    CcspTraceWarning(("%s: writeID=%lu, bsUpdate=%lu\n", __FUNCTION__, g_currentWriteEntity, g_currentBsUpdate));
    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
    if(AnscValidateInputString(pString) != TRUE)
        return FALSE;

    if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
    {
         /* check the parameter name and set the corresponding value */
            rc = strcmp_s("CDLModuleUrl", strlen("CDLModuleUrl"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
            {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->CDLModuleUrl.UpdateSource);

                /* Below condition is added to restrict the maximum pString length less than acceptable value for buffer overflow issues
                 * UpdateJsonParam function doesn't have the maximum permissible string length validation check.
                 */
                if(strlen(pString) < sizeof(pBindObj->CDLModuleUrl.ActiveValue))
                {
                        if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.CDLDM.CDLModuleUrl",PartnerID,pString, requestorStr, currentTime))
                        {
                                rc = STRCPY_S_NOCLOBBER(pBindObj->CDLModuleUrl.ActiveValue, sizeof(pBindObj->CDLModuleUrl.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->CDLModuleUrl.UpdateSource, sizeof(pBindObj->CDLModuleUrl.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                return TRUE;
                        }
                 }
                 else
                 {
                      CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                      return FALSE;
                 }
            }
   }

    return FALSE;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_Syndication.

    *  MaintenanceWindow_GetParamStringValue
***********************************************************************/

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Syndication_GetParamStringValue
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
Syndication_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    errno_t      rc  = -1;

    if (strcmp(ParamName, "PartnerId") == 0)
    {
        /* collect value */
        //CosaDmlDiGetSyndicationPartnerId(NULL, pValue,pulSize);
        rc = strcpy_s( pValue, *pulSize, pMyObject->PartnerID);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "TR69CertLocation") == 0)
    {
        /* collect value */
        CosaDmlDiGetSyndicationTR69CertLocation( hInsContext, pMyObject->TR69CertLocation.ActiveValue );
        rc = strcpy_s( pValue, *pulSize, pMyObject->TR69CertLocation.ActiveValue );
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "LocalUIBrandingTable") == 0)
    {
        /* collect value */
        CosaDmlDiGetSyndicationLocalUIBrandingTable(NULL, pValue,pulSize);
        return 0;
    }
    if (strcmp(ParamName, "WifiUIBrandingTable") == 0)
    {
        /* collect value */
        CosaDmlDiGetSyndicationWifiUIBrandingTable(NULL, pValue,pulSize);
        return 0;
    }
    if (strcmp(ParamName, "PauseScreenFileLocation") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pulSize, pMyObject->UiBrand.PauseScreenFileLocation.ActiveValue);
    }
    if (strcmp(ParamName, "HCMRecordingUploadURL") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pulSize, pMyObject->HCMRecordingUploadURL.ActiveValue);
    }
    if (strcmp(ParamName, "MeshRedirectorURL") == 0)
    {        
        /* collect value */
        return  update_pValue(pValue,pulSize, pMyObject->MeshRedirectorURL.ActiveValue);
    }
    if (strcmp(ParamName, "AdvsecRedirectorURL") == 0)
    {
        /* collect value */
        return update_pValue(pValue, pulSize, pMyObject->AdvsecRedirectorURL.ActiveValue);
    }
#if defined(_COSA_BCM_ARM_) && !defined(_CBR_PRODUCT_REQ_)
    if (strcmp(ParamName, "CMVoiceImageSelect") == 0)
    {
	char buf[64];

	if(0 == syscfg_get(NULL, "CMVoiceImg", buf, sizeof(buf)))
	{
	    return  update_pValue(pValue,pulSize, buf);
	}
	else
	{
		CcspTraceError(("syscfg_get for CMVoiceImg failed\n"));
		return 1;
	}
    }
#endif
    if (strcmp(ParamName, "XconfURL") == 0)
    {
        return  update_pValue(pValue,pulSize, pMyObject->XconfURL.ActiveValue);
    }
    if (strcmp(ParamName, "LogUploadURL") == 0)
    {
        return  update_pValue(pValue,pulSize, pMyObject->LogUploadURL.ActiveValue);
    }
    if (strcmp(ParamName, "Telemetry") == 0)
    {
        return  update_pValue(pValue,pulSize, pMyObject->TelemetryURL.ActiveValue);
    }
    if (strcmp(ParamName, "CrashPortal") == 0)
    {
        return  update_pValue(pValue,pulSize, pMyObject->CrashPortalURL.ActiveValue);
    }
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Syndication_SetParamStringValue
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

    return:     TRUE if succeeded,
                FALSE if failed.

**********************************************************************/
BOOL
Syndication_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    ANSC_STATUS 					retValue  = ANSC_STATUS_FAILURE;
    char PartnerID[PARTNER_ID_LEN] = {0};
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
    errno_t rc                     = -1;
    int ind                        = -1;

     if((pString == NULL) || (ParamName == NULL))
     {
         AnscTraceWarning(("RDK_LOG_WARN, %s %s:%d\n",__FILE__,__FUNCTION__,__LINE__));
         return FALSE;
     }

    /* check the parameter name and set the corresponding value */
    if ( !(rc = strcmp_s("TR69CertLocation", strlen("TR69CertLocation"), ParamName, &ind)) )
    {
        if(!ind)
        {
        /* check the length of pString and restricting the maximum length to 256 as CosaDmlDiGetSyndicationTR69CertLocation reading only 256 bytes*/
        /* the scope of this length check is particular to TR69CertLocation */
                if(strlen(pString) >= MAX_ALLOWABLE_STRING_LEN)
                {
                    AnscTraceWarning(("Maximum Allowable string length is less than 256 bytes %s %d\n", __FUNCTION__, __LINE__));
                    return FALSE;
                }

                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->TR69CertLocation.UpdateSource);

		retValue = CosaDmlDiSetSyndicationTR69CertLocation( hInsContext, pString );
		if( ANSC_STATUS_SUCCESS == retValue )
		{
                        rc = STRCPY_S_NOCLOBBER(pMyObject->TR69CertLocation.ActiveValue, sizeof(pMyObject->TR69CertLocation.ActiveValue), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                        } 
                        rc = STRCPY_S_NOCLOBBER(pMyObject->TR69CertLocation.UpdateSource, sizeof(pMyObject->TR69CertLocation.UpdateSource), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                        }
                        getPartnerId(PartnerID);
                        if ( PartnerID[ 0 ] != '\0')
                            UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.TR69CertLocation",PartnerID, pString, requestorStr, currentTime);
		}
	        return TRUE;
        }
    }
    else if(rc != EOK)
    {
        AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
        return FALSE;
    }

    if ( !(rc = strcmp_s("PartnerId", strlen("PartnerId"), ParamName, &ind)) )
    {
        if(!(ind))
        {
#if defined (_RDK_REF_PLATFORM_)
		ind = 0;
		if ( !(rc = strcmp_s("comcast", strlen("comcast"), pString, &ind) ) ) //Compare if input string is comcast
		{
			if( ind != 0 )//if input partner ID string is comcast,you wont enter this 'if' loop
			{
#endif
                if ( !(rc = strcmp_s(pMyObject->PartnerID, sizeof(pMyObject->PartnerID), pString, &ind)) )
		{
                        if(ind != 0)
                        {
			    retValue = setTempPartnerId( pString );
			    if( ANSC_STATUS_SUCCESS == retValue )
			    {
			        ULONG    size = 0;
				//Get the Factory PartnerID
			        memset(PartnerID, 0, sizeof(PartnerID));
			        getFactoryPartnerId(PartnerID, &size);
			
			        CcspTraceInfo(("[SET-PARTNERID] Factory_Partner_ID:%s\n", ( PartnerID[ 0 ] != '\0' ) ? PartnerID : "NULL" ));
			        CcspTraceInfo(("[SET-PARTNERID] Current_PartnerID:%s\n", pMyObject->PartnerID ));
			        CcspTraceInfo(("[SET-PARTNERID] Overriding_PartnerID:%s\n", pString ));
								
				return TRUE;
			    }
                        }
		}
#if defined (_RDK_REF_PLATFORM_)
			}
		}
#endif
                else if(rc != EOK)
                {
                     AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                     return FALSE;
                }
        }
    }
    else if(rc != EOK)
    {
        AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
        return FALSE;
    }

    if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
    {
   	/* check the parameter name and set the corresponding value */
        if ( !(rc = strcmp_s("PauseScreenFileLocation", strlen("PauseScreenFileLocation"), ParamName, &ind)) )
	{
            if(!(ind))
            {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->UiBrand.PauseScreenFileLocation.UpdateSource);

		if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.PauseScreenFileLocation",PartnerID, pString, requestorStr, currentTime))
		{
                        rc = STRCPY_S_NOCLOBBER(pMyObject->UiBrand.PauseScreenFileLocation.ActiveValue, sizeof(pMyObject->UiBrand.PauseScreenFileLocation.ActiveValue), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pMyObject->UiBrand.PauseScreenFileLocation.UpdateSource, sizeof(pMyObject->UiBrand.PauseScreenFileLocation.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }
			return TRUE;
		}
            }

	}
        else if(rc != EOK)
        {
            AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
            return FALSE;
        }

        if ( !(rc = strcmp_s("XconfURL", strlen("XconfURL"), ParamName, &ind)) )
        {
           if(!(ind))
            {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->XconfURL.UpdateSource);

                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.XconfURL",PartnerID, pString, requestorStr, currentTime))
                {
                        rc = STRCPY_S_NOCLOBBER(pMyObject->XconfURL.ActiveValue, sizeof(pMyObject->XconfURL.ActiveValue), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pMyObject->XconfURL.UpdateSource, sizeof(pMyObject->XconfURL.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }
                        return TRUE;
                }
            }
        }
        else if(rc != EOK)
        {
            AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
            return FALSE;
        }
        
        if ( !(rc = strcmp_s("LogUploadURL", strlen("LogUploadURL"), ParamName, &ind)) )
        {
           if(!(ind))
            {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->LogUploadURL.UpdateSource);

                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.LogUploadURL",PartnerID, pString, requestorStr, currentTime))
                {
                        rc = STRCPY_S_NOCLOBBER(pMyObject->LogUploadURL.ActiveValue, sizeof(pMyObject->LogUploadURL.ActiveValue), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pMyObject->LogUploadURL.UpdateSource, sizeof(pMyObject->LogUploadURL.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }
                        return TRUE;
                }
            }
        }
        else if(rc != EOK)
        {
            AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
            return FALSE;
        }
        
        if ( !(rc = strcmp_s("Telemetry", strlen("Telemetry"), ParamName, &ind)) )
        {
           if(!(ind))
            {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->TelemetryURL.UpdateSource);

                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.Telemetry",PartnerID, pString, requestorStr, currentTime))
                {
                        rc = STRCPY_S_NOCLOBBER(pMyObject->TelemetryURL.ActiveValue, sizeof(pMyObject->TelemetryURL.ActiveValue), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pMyObject->TelemetryURL.UpdateSource, sizeof(pMyObject->TelemetryURL.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }
                        return TRUE;
                }
            }
        }
        else if(rc != EOK)
        {
            AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
            return FALSE;
        }
        
        if ( !(rc = strcmp_s("CrashPortal", strlen("CrashPortal"), ParamName, &ind)) )
        {
           if(!(ind))
            {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->CrashPortalURL.UpdateSource);

                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.CrashPortal",PartnerID, pString, requestorStr, currentTime))
                {
                        rc = STRCPY_S_NOCLOBBER(pMyObject->CrashPortalURL.ActiveValue, sizeof(pMyObject->CrashPortalURL.ActiveValue), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pMyObject->CrashPortalURL.UpdateSource, sizeof(pMyObject->CrashPortalURL.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }
                        return TRUE;
                }
            }
        }
     }
 
    if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
    {
        /* check the parameter name and set the corresponding value */
        if ( !(rc = strcmp_s("HCMRecordingUploadURL", strlen("HCMRecordingUploadURL"), ParamName, &ind)) )
        {
            if(!(ind))
            {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->HCMRecordingUploadURL.UpdateSource);

                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.HCMRecordingUploadURL",PartnerID, pString, requestorStr, currentTime))
                {
                        rc = STRCPY_S_NOCLOBBER(pMyObject->HCMRecordingUploadURL.ActiveValue, sizeof(pMyObject->HCMRecordingUploadURL.ActiveValue), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pMyObject->HCMRecordingUploadURL.UpdateSource, sizeof(pMyObject->HCMRecordingUploadURL.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }
                        return TRUE;
                }
            }
        }
        else if(rc != EOK)
        {
            AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
            return FALSE;
        }
     }

    if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
    {   
        /* check the parameter name and set the corresponding value */
        if ( !(rc = strcmp_s("MeshRedirectorURL", strlen("MeshRedirectorURL"), ParamName, &ind)) )
        {   
            if(!(ind))
            {   
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->MeshRedirectorURL.UpdateSource);

                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.MeshRedirectorURL",PartnerID, pString, requestorStr, currentTime))
                {
                        rc = STRCPY_S_NOCLOBBER(pMyObject->MeshRedirectorURL.ActiveValue, sizeof(pMyObject->MeshRedirectorURL.ActiveValue), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pMyObject->MeshRedirectorURL.UpdateSource, sizeof(pMyObject->MeshRedirectorURL.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }
                        return TRUE;
                }
            }

        }
        else if(rc != EOK)
        {   
            AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
            return FALSE;
        }
     }

     if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
     {
        /* check the parameter name and set the corresponding value */
        if ( !(rc = strcmp_s("AdvsecRedirectorURL", strlen("AdvsecRedirectorURL"), ParamName, &ind)) )
        {
            if(!(ind))
            {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->AdvsecRedirectorURL.UpdateSource);

                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.AdvsecRedirectorURL",PartnerID, pString, requestorStr, currentTime))
                {
                    if ( ANSC_STATUS_SUCCESS == isValidUrl(pString))
                    {
                        retValue = CosaSetSysCfgString(g_AdvSecDefaultEndpointURL, pString);
                        if ( ANSC_STATUS_SUCCESS != retValue )
                        {
                            AnscTraceError(("Setting Syscfg Advsec Default EndPointURL is failed"));
                            return retValue;
                        }

                        rc = STRCPY_S_NOCLOBBER(pMyObject->AdvsecRedirectorURL.ActiveValue, sizeof(pMyObject->AdvsecRedirectorURL.ActiveValue), pString);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pMyObject->AdvsecRedirectorURL.UpdateSource, sizeof(pMyObject->AdvsecRedirectorURL.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                             AnscTraceWarning(("RDK_LOG_WARN, safeclib strcpy_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
                             return FALSE;
                        }
                        return TRUE;
                    }
                    else
                    {
                        AnscTraceError(("Setting Advsec Default EndPointURL: [%s] is Invalid URL\n", pString))
                    }
                }
            }
        }
        else if(rc != EOK)
        {
            AnscTraceWarning(("RDK_LOG_WARN, safeclib strcmp_s- %s %s:%d rc =%d \n",__FILE__, __FUNCTION__,__LINE__,rc));
            return FALSE;
        }
     }
     return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SwitchToUDHCPC_GetParamBoolValue
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
#if defined(_COSA_INTEL_XB3_ARM_) || defined(INTEL_PUMA7)
BOOL
SwitchToUDHCPC_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
	/* This Get API is only for XB3,AXB6 devices */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        char buf[8];
        if( syscfg_get( NULL, "UDHCPEnable_v2", buf, sizeof(buf))==0)
        {
            if (strcmp(buf, "true") == 0)
            {
                *pBool = TRUE;
            }
            else
            {
                *pBool = FALSE;
            }
        }
        else
        {
            CcspTraceWarning(("%s syscfg_get failed  for UDHCPEnable_v2\n",__FUNCTION__));
            *pBool = FALSE;
        }

        return TRUE;
    }
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SwitchToUDHCPC_SetParamBoolValue
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
SwitchToUDHCPC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
     /* This set API is only for XB3,AXB6 devices */

    if (strcmp(ParamName, "Enable") == 0)
    {
        char buf[8];
        char event[8];
        int val;

        /* collect previous flag value */
       if( syscfg_get( NULL, "UDHCPEnable_v2", buf, sizeof(buf)) == 0)
        {
        	val = strcmp(buf,"true")?0:1;
                if (val != bValue)
                {
                        if (bValue)
                        {
                                commonSyseventGet("dhcpclient_v6", event, sizeof(event));
                                if (atoi(event) == 1)
                                {
                                        commonSyseventSet("dhcpclient_v4", "1");
                                        AnscTraceWarning(("dhcpclient_v6 event is enabled.so,enabling dhcpclient_v4 event \n"));
                                }
                                else
                                {
                                        v_secure_system("sh /etc/dhcpswitch.sh schedule_v4_cron &");
                                        AnscTraceWarning(("dhcpclient_v6 event is not enabled.scheduling cron \n"));
                                }
                        }
                        else
                        {
                                v_secure_system("sh /etc/dhcpswitch.sh clear_v4_cron &");
                                AnscTraceWarning(("dhcp client switching back to default \n"));
                        }
                }
                else if(!bValue)
                {
                        char v4event[8];
                        commonSyseventGet("dhcpclient_v4", v4event, sizeof(v4event));
                        if (atoi(v4event) == 1)
                        {
                                char v6event[8];
                                commonSyseventGet("dhcpclient_v6", v6event, sizeof(v6event));
                                if (atoi(v6event) ==1)
                                {
                                        commonSyseventSet("dhcpclient_v4", "0");
                                        AnscTraceWarning(("dhcpclient_v4 is disabled and no wan restart\n"));
                                }
                                else
                                {
                                        commonSyseventSet("dhcpclient_v4", "0");
                                        v_secure_system("sh /etc/dhcpswitch.sh removecron &");
                                        AnscTraceWarning(("dhcpclient_v4 is disabled and scheduled cron removed\n"));
                                }


                        }
                        else
                        {
                                AnscTraceWarning(("No set operation done since UDHCPEnable_v2 flag already set to %d\n", bValue));
                        }
                }
                else
                {
                        AnscTraceWarning(("No set operation done since UDHCPEnable_v2 flag already set to %d\n", bValue));
                }
        }
        else
        {
                AnscTraceWarning(("syscfg_get failed for UDHCPEnable_v2\n"));
                return FALSE;
        }


        return TRUE;
    }
   return FALSE;
}
#endif
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Syndication_GetParamBoolValue
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
Syndication_GetParamBoolValue
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

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
       Syndication_SetParamBoolValue
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
Syndication_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
    return FALSE;
}

/***********************************************************************
APIs for Object:
        Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.WANsideSSH.
          *  WANsideSSH_GetParamBoolValue
          *  WANsideSSH_SetParamBoolValue

***********************************************************************/

BOOL
WANsideSSH_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
	/* check the parameter name and return the corresponding value */
        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;


        if (strcmp(ParamName, "Enable") == 0)
        {
		*pBool = pMyObject->bWANsideSSHEnable.ActiveValue;
                return TRUE;
        }


	return FALSE;
}

BOOL
WANsideSSH_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
   UNREFERENCED_PARAMETER(hInsContext);
   PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

   if (strcmp(ParamName, "Enable") == 0)
   {
        char * requestorStr = getRequestorString();
        char * currentTime = getTime();

        IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->bWANsideSSHEnable.UpdateSource);

	if (pMyObject->bWANsideSSHEnable.ActiveValue == bValue)
	{
		CcspTraceInfo(("WANsideSSH is already %s\n", ( bValue ==TRUE ) ?  "Enabled" : "Disabled"));
		return TRUE;
	}

	if (syscfg_set_commit(NULL, "WANsideSSH_Enable", (bValue == TRUE) ? "true" : "false") != 0)
	{
		return FALSE;
	}

	pMyObject->bWANsideSSHEnable.ActiveValue = bValue;
	if (bValue == TRUE)
		v_secure_system("sh /lib/rdk/wan_ssh.sh enable &");
	else
		v_secure_system("sh /lib/rdk/wan_ssh.sh disable &");

        char *value = ( bValue ==TRUE ) ?  "true" : "false";
        char PartnerID[PARTNER_ID_LEN] = {0};
        if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
            UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.WANsideSSH.Enable",PartnerID, value, requestorStr, currentTime);

	return TRUE;

   }
   return FALSE;
}
/***********************************************************************
APIs for Object:
	Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_Control.
	  *  RDKB_Control_GetParamBoolValue
	  *  RDKB_Control_SetParamBoolValue

***********************************************************************/

BOOL
RDKB_Control_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pBool);
    /* check the parameter name and return the corresponding value */

	if (strcmp(ParamName, "ActivatePartnerId") == 0)
	{
		return TRUE;
	}

	if (strcmp(ParamName, "ClearPartnerId") == 0)
	{
		return TRUE;
	}

    return FALSE;
}

BOOL
RDKB_Control_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    pthread_t tid ; 
    ANSC_STATUS 				   retValue  = ANSC_STATUS_FAILURE;

   if (strcmp(ParamName, "ActivatePartnerId") == 0)
    {
	if ( bValue )
	{
		retValue = activatePartnerId();
		if( ANSC_STATUS_SUCCESS == retValue )
		{
			return TRUE;
		}
	}
    }

    if (strcmp(ParamName, "ClearPartnerId") == 0)
    {
	if ( bValue )
	{
		CcspTraceWarning(("%s: Clearing PartnerId and device going Factory Reset  \n", __FUNCTION__));
		v_secure_system( "rm -rf  /nvram/.partner_ID" );
		pthread_create ( &tid, NULL, &CosaDmlDiPartnerIDChangeHandling, NULL );
		return TRUE;
	}
    }

    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.
	   *  RDKB_UIBranding_GetParamBoolValue
	   *  RDKB_UIBranding_GetParamStringValue
	   *  RDKB_UIBranding_SetParamStringValue
	  
***********************************************************************/
BOOL
RDKB_UIBranding_GetParamBoolValue

	(
		ANSC_HANDLE 			hInsContext,
		char*				ParamName,
		BOOL*                       	pBool
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

        if (strcmp(ParamName, "AllowEthernetWAN") == 0)
        {
		 /* collect value */
		*pBool = pBindObj->AllowEthernetWAN.ActiveValue;
		return TRUE;
        }
    return FALSE;
}


ULONG
RDKB_UIBranding_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;
	
	if (strcmp(ParamName, "DefaultLocalIPv4SubnetRange") == 0)
        {
            return  update_pValue(pValue,pulSize, pBindObj->DefaultLocalIPv4SubnetRange.ActiveValue);
        }
	if (strcmp(ParamName, "DefaultLanguage") == 0)
        {
            return  update_pValue(pValue,pulSize, pBindObj->DefaultLanguage.ActiveValue);
        }
	if (strcmp(ParamName, "DefaultAdminIP") == 0)
        {
            return  update_pValue(pValue,pulSize, pBindObj->DefaultAdminIP.ActiveValue);

        } 
	
	 return -1;
}


BOOL
RDKB_UIBranding_SetParamStringValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;
        char PartnerID[PARTNER_ID_LEN] = {0};
        char * requestorStr = getRequestorString();
        char * currentTime = getTime();
        errno_t rc = -1;
        int ind = -1;

        if((ParamName == NULL) || (pString == NULL))
            return FALSE;

        IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
        if(AnscValidateInputString(pString) != TRUE)
            return FALSE;

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
   {

            rc = strcmp_s("DefaultLanguage", strlen("DefaultLanguage"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
            {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->DefaultLanguage.UpdateSource);

                /* Below condition is added to restrict the maximum pString length less than acceptable value for buffer overflow issues
                 * UpdateJsonParam function doesn't have the maximum permissible string length validation check.
                 */
                if(strlen(pString) < sizeof(pBindObj->DefaultLanguage.ActiveValue))
                {
                        if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultLanguage",PartnerID,pString, requestorStr, currentTime))
                        {
                                rc = STRCPY_S_NOCLOBBER(pBindObj->DefaultLanguage.ActiveValue, sizeof(pBindObj->DefaultLanguage.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                     ERR_CHK(rc);
                                     return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->DefaultLanguage.UpdateSource, sizeof(pBindObj->DefaultLanguage.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                     ERR_CHK(rc);
                                     return FALSE;
                                }
                                return TRUE;
                        }
                 }
                 else
                 {
                     CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                     return FALSE;
                 }

            }

   }
   return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.
	  *  Footer_GetParamStringValue
	  *  Footer_SetParamStringValue
	  
***********************************************************************/
ULONG
Footer_GetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	if (strcmp(ParamName, "PartnerLink") == 0)
	{
		/* collect value */
	    return  update_pValue(pValue,pulSize, pBindObj->Footer.PartnerLink.ActiveValue);
	}

	if (strcmp(ParamName, "UserGuideLink") == 0)
	{
		return  update_pValue(pValue,pulSize, pBindObj->Footer.UserGuideLink.ActiveValue);
	}
	
	if (strcmp(ParamName, "CustomerCentralLink") == 0)
	{
	    return  update_pValue(pValue,pulSize, pBindObj->Footer.CustomerCentralLink.ActiveValue);
	}

	if (strcmp(ParamName, "PartnerText") == 0)
	{
	    return  update_pValue(pValue,pulSize, pBindObj->Footer.PartnerText.ActiveValue);
	}

	if (strcmp(ParamName, "UserGuideText") == 0)
	{
	    return  update_pValue(pValue,pulSize, pBindObj->Footer.UserGuideText.ActiveValue);
	}

	if (strcmp(ParamName, "CustomerCentralText") == 0)
	{
	    return  update_pValue(pValue,pulSize, pBindObj->Footer.CustomerCentralText.ActiveValue);
	}
	return -1;

}

BOOL
Footer_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

    char PartnerID[PARTNER_ID_LEN] = {0};
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
    errno_t rc = -1;
    int ind = -1;

    if((ParamName == NULL) || (pString == NULL))
        return FALSE;

    CcspTraceWarning(("%s: writeID=%lu, bsUpdate=%lu\n", __FUNCTION__, g_currentWriteEntity, g_currentBsUpdate));
    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
    if(AnscValidateInputString(pString) != TRUE)
        return FALSE;

    if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
    {
   	 /* check the parameter name and set the corresponding value */
            rc = strcmp_s("PartnerLink", strlen("PartnerLink"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
	    {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.PartnerLink.UpdateSource);

                /* Below condition is added to restrict the maximum pString length less than acceptable value for buffer overflow issues
                 * UpdateJsonParam function doesn't have the maximum permissible string length validation check.
                 */
                if(strlen(pString) < sizeof(pBindObj->Footer.PartnerLink.ActiveValue))
                {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerLink",PartnerID,pString, requestorStr, currentTime))
			{
                                rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.PartnerLink.ActiveValue, sizeof(pBindObj->Footer.PartnerLink.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.PartnerLink.UpdateSource, sizeof(pBindObj->Footer.PartnerLink.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

				return TRUE;
			}
                 }
                 else
                 {
                      CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                      return FALSE;
                 }
	    }
            rc = strcmp_s("UserGuideLink", strlen("UserGuideLink"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
	    {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.UserGuideLink.UpdateSource);

                if(strlen(pString) < sizeof(pBindObj->Footer.UserGuideLink.ActiveValue))
                {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideLink",PartnerID,pString, requestorStr, currentTime))
			{
                                rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.UserGuideLink.ActiveValue, sizeof(pBindObj->Footer.UserGuideLink.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.UserGuideLink.UpdateSource, sizeof(pBindObj->Footer.UserGuideLink.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

				return TRUE;
			}
                 }
                 else
                 {
                      CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                      return FALSE;
                 }

	    }
            rc = strcmp_s("CustomerCentralLink", strlen("CustomerCentralLink"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
	    {
                 IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.CustomerCentralLink.UpdateSource);

                 if(strlen(pString) < sizeof(pBindObj->Footer.CustomerCentralLink.ActiveValue))
                 {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralLink",PartnerID,pString, requestorStr, currentTime))
			{
                                rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.CustomerCentralLink.ActiveValue, sizeof(pBindObj->Footer.CustomerCentralLink.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.CustomerCentralLink.UpdateSource, sizeof(pBindObj->Footer.CustomerCentralLink.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

				return TRUE;
			}
                 }
                 else
                 {
                      CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                      return FALSE;
                 }
			
	    }

        rc = strcmp_s("PartnerText", strlen("PartnerText"), ParamName, &ind);
        ERR_CHK(rc);
        if((rc == EOK) && (!ind))
	{
              IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.PartnerText.UpdateSource);

              if(strlen(pString) < sizeof(pBindObj->Footer.PartnerText.ActiveValue))
              {
		    if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerText",PartnerID,pString, requestorStr, currentTime))
		    {
                        rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.PartnerText.ActiveValue, sizeof(pBindObj->Footer.PartnerText.ActiveValue), pString);
                        if(rc != EOK)
                        {
                            ERR_CHK(rc);
                            return FALSE;
                        }


                        rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.PartnerText.UpdateSource, sizeof(pBindObj->Footer.PartnerText.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                            ERR_CHK(rc);
                            return FALSE;
                        }

			return TRUE;
		    }
              }
              else
              {
                   CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                   return FALSE;
              }

	}

        rc = strcmp_s("UserGuideText", strlen("UserGuideText"), ParamName, &ind);
        ERR_CHK(rc);
        if((rc == EOK) && (!ind))
	{
             IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.UserGuideText.UpdateSource);

             if(strlen(pString) < sizeof(pBindObj->Footer.UserGuideText.ActiveValue))
             {
	         if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideText",PartnerID,pString, requestorStr, currentTime))
		 {
                        rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.UserGuideText.ActiveValue, sizeof(pBindObj->Footer.UserGuideText.ActiveValue), pString);
                        if(rc != EOK)
                        {
                            ERR_CHK(rc);
                            return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.UserGuideText.UpdateSource, sizeof(pBindObj->Footer.UserGuideText.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                            ERR_CHK(rc);
                            return FALSE;
                        }

			return TRUE;
		}
           }
           else
           {
               CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
               return FALSE;
           }

	}

        rc = strcmp_s("CustomerCentralText", strlen("CustomerCentralText"), ParamName, &ind);
        ERR_CHK(rc);
        if((rc == EOK) && (!ind))
 	{
            IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.CustomerCentralText.UpdateSource);

            if(strlen(pString) < sizeof(pBindObj->Footer.CustomerCentralText.ActiveValue))
            {
                 if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralText",PartnerID,pString, requestorStr, currentTime))
                 {
                        rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.CustomerCentralText.ActiveValue, sizeof(pBindObj->Footer.CustomerCentralText.ActiveValue), pString);
                        if(rc != EOK)
                        {
                            ERR_CHK(rc);
                            return FALSE;
                        }

                        rc = STRCPY_S_NOCLOBBER(pBindObj->Footer.CustomerCentralText.UpdateSource, sizeof(pBindObj->Footer.CustomerCentralText.UpdateSource), requestorStr);
                        if(rc != EOK)
                        {
                            ERR_CHK(rc);
                            return FALSE;
                        }

 			return TRUE;
                 }
            }
            else
            {
                CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                return FALSE;
            }
 	}
   }
   
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.
	  *  Connection_GetParamStringValue
	  
***********************************************************************/
ULONG
Connection_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	if (strcmp(ParamName, "MSOmenu") == 0)
	{
	    return  update_pValue(pValue,pulSize, pBindObj->Connection.MSOmenu.ActiveValue);
	}

	if (strcmp(ParamName, "MSOinfo") == 0)
	{
	    return  update_pValue(pValue,pulSize, pBindObj->Connection.MSOinfo.ActiveValue);
	}
	
	if (strcmp(ParamName, "StatusTitle") == 0)
	{
	    return  update_pValue(pValue,pulSize, pBindObj->Connection.StatusTitle.ActiveValue);
	}
	
	if (strcmp(ParamName, "StatusInfo") == 0)
	{
	    return  update_pValue(pValue,pulSize, pBindObj->Connection.StatusInfo.ActiveValue);
	}
	
	return -1;

}


BOOL
Connection_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

    char PartnerID[PARTNER_ID_LEN] = {0};
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
    errno_t rc = -1;
    int ind = -1;

    if((ParamName == NULL) || (pString == NULL))
         return FALSE;

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
    if(AnscValidateInputString(pString) != TRUE)
        return FALSE;

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0') )
   {
   	 /* check the parameter name and set the corresponding value */
            rc = strcmp_s("MSOmenu", strlen("MSOmenu"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
	    {
                 IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Connection.MSOmenu.UpdateSource);
                 /* Below condition is added to restrict the maximum pString length less than acceptable value for buffer overflow issues
                  * UpdateJsonParam function doesn't have the maximum permissible string length validation check.
                  */
                 if(strlen(pString) < sizeof(pBindObj->Connection.MSOmenu.ActiveValue))
                 {

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOmenu",PartnerID,pString, requestorStr, currentTime))
			{
                                rc = STRCPY_S_NOCLOBBER(pBindObj->Connection.MSOmenu.ActiveValue, sizeof(pBindObj->Connection.MSOmenu.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->Connection.MSOmenu.UpdateSource, sizeof(pBindObj->Connection.MSOmenu.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

				return TRUE;
			}
                  }
                  else
                  {
                      CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                      return FALSE;
                  }
	    }
            rc = strcmp_s("MSOinfo", strlen("MSOinfo"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
	    {
                 IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Connection.MSOinfo.UpdateSource);

                 if(strlen(pString) < sizeof(pBindObj->Connection.MSOinfo.ActiveValue))
                 {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOinfo",PartnerID,pString, requestorStr, currentTime) )
			{
                                rc = STRCPY_S_NOCLOBBER(pBindObj->Connection.MSOinfo.ActiveValue, sizeof(pBindObj->Connection.MSOinfo.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->Connection.MSOinfo.UpdateSource, sizeof(pBindObj->Connection.MSOinfo.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

				return TRUE;
			}
                  }
                  else
                  {
                      CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                      return FALSE;
                  }
	    }
            rc = strcmp_s("StatusTitle", strlen("StatusTitle"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
	    {
                  IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Connection.StatusTitle.UpdateSource);

                  if(strlen(pString) < sizeof(pBindObj->Connection.StatusTitle.ActiveValue))
                  {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusTitle",PartnerID,pString, requestorStr, currentTime) )
			{
                                rc = STRCPY_S_NOCLOBBER(pBindObj->Connection.StatusTitle.ActiveValue, sizeof(pBindObj->Connection.StatusTitle.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->Connection.StatusTitle.UpdateSource, sizeof(pBindObj->Connection.StatusTitle.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

				return TRUE;
			}
                  }
                  else
                  {
                      CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                      return FALSE;
                  }
			
	    }
            rc = strcmp_s("StatusInfo", strlen("StatusInfo"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
	    {
                 IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Connection.StatusInfo.UpdateSource);

                 if(strlen(pString) < sizeof(pBindObj->Connection.StatusInfo.ActiveValue))
                 {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusInfo",PartnerID,pString, requestorStr, currentTime))
			{
                                rc = STRCPY_S_NOCLOBBER(pBindObj->Connection.StatusInfo.ActiveValue, sizeof(pBindObj->Connection.StatusInfo.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->Connection.StatusInfo.UpdateSource, sizeof(pBindObj->Connection.StatusInfo.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

				return TRUE;
			}
                 }
                 else
                 {
                     CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                     return FALSE;
                 }
	    }
   }
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.NetworkDiagnosticTools.
	  *  NetworkDiagnosticTools_GetParamStringValue
	  
***********************************************************************/
ULONG
NetworkDiagnosticTools_GetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	if (strcmp(ParamName, "ConnectivityTestURL") == 0)
	{
	    return  update_pValue(pValue,pulSize, pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue);
	}
	
	return -1;

}


BOOL
NetworkDiagnosticTools_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

    char PartnerID[PARTNER_ID_LEN] = {0};
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
    errno_t rc = -1;
    int ind = -1;

    if((ParamName == NULL) || (pString == NULL))
        return FALSE;

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

    if(AnscValidateInputString(pString) != TRUE)
        return FALSE;

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
   {
   	 /* check the parameter name and set the corresponding value */
            rc = strcmp_s("ConnectivityTestURL", strlen("ConnectivityTestURL"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
	    {
                 IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->NDiagTool.ConnectivityTestURL.UpdateSource);

                 /* Below condition is added to restrict the maximum pString length less than acceptable value for buffer overflow issues
                  * UpdateJsonParam function doesn't have the maximum permissible string length validation check.
                  */
                 if(strlen(pString) < sizeof(pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue))
                 {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.NetworkDiagnosticTools.ConnectivityTestURL",PartnerID, pString, requestorStr, currentTime))
			{
                                rc = STRCPY_S_NOCLOBBER(pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue, sizeof(pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                     ERR_CHK(rc);
                                     return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->NDiagTool.ConnectivityTestURL.UpdateSource, sizeof(pBindObj->NDiagTool.ConnectivityTestURL.UpdateSource), requestorStr);
                                 if(rc != EOK)
                                {
                                     ERR_CHK(rc);
                                     return FALSE;
                                }

				return TRUE;
			}	
                 }
                 else
                 {
                     CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                     return FALSE;
                 }
	    }
	  
   }
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.
	  *  WiFiPersonalization_GetParamStringValue
	  
***********************************************************************/

BOOL
WiFiPersonalization_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

        if (strcmp(ParamName, "Support") == 0)
        {
		 /* collect value */
		*pBool = pBindObj->WifiPersonal.Support.ActiveValue;
		return TRUE;
        }    

	if (strcmp(ParamName, "SMSsupport") == 0)
        {
		/* collect value */
		*pBool = pBindObj->WifiPersonal.SMSsupport.ActiveValue;
		return TRUE;

        }
	
        if (strcmp(ParamName, "MyAccountAppSupport") == 0)
	{
		/* collect value */
		*pBool = pBindObj->WifiPersonal.MyAccountAppSupport.ActiveValue;
		return TRUE;
	} 
    return FALSE;
}

BOOL
WiFiPersonalization_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

   char PartnerID[PARTNER_ID_LEN] = {0};
   char *value = ( bValue ==TRUE ) ?  "true" : "false";
   char * requestorStr = getRequestorString();
   char * currentTime = getTime();
   errno_t  rc = -1;

   IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
   {
	    if (strcmp(ParamName, "Support") == 0)
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->WifiPersonal.Support.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Support",PartnerID, value, requestorStr, currentTime ))
			{
				pBindObj->WifiPersonal.Support.ActiveValue = bValue;

                                rc = STRCPY_S_NOCLOBBER( pBindObj->WifiPersonal.Support.UpdateSource, sizeof(pBindObj->WifiPersonal.Support.UpdateSource), requestorStr );
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }
				return TRUE;
			}	

	    }

	    if (strcmp(ParamName, "SMSsupport") == 0)
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->WifiPersonal.SMSsupport.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.SMSsupport",PartnerID, value, requestorStr, currentTime))
			{
				pBindObj->WifiPersonal.SMSsupport.ActiveValue = bValue;

                                rc = STRCPY_S_NOCLOBBER( pBindObj->WifiPersonal.SMSsupport.UpdateSource, sizeof(pBindObj->WifiPersonal.SMSsupport.UpdateSource), requestorStr );
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }
				return TRUE;
			}
			
	    }
		
	    if (strcmp(ParamName, "MyAccountAppSupport") == 0)
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->WifiPersonal.MyAccountAppSupport.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.MyAccountAppSupport",PartnerID, value, requestorStr, currentTime))
			{
				pBindObj->WifiPersonal.MyAccountAppSupport.ActiveValue = bValue;

                                rc = STRCPY_S_NOCLOBBER( pBindObj->WifiPersonal.MyAccountAppSupport.UpdateSource, sizeof(pBindObj->WifiPersonal.MyAccountAppSupport.UpdateSource), requestorStr );
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }
				return TRUE;
			}

	    }
   	}
    return FALSE;
}

ULONG
WiFiPersonalization_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;
	
	 if (strcmp(ParamName, "PartnerHelpLink") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue);
        } 
	if (strcmp(ParamName, "MSOLogo") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->WifiPersonal.MSOLogo.ActiveValue);

        } 
	if (strcmp(ParamName, "Title") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->WifiPersonal.Title.ActiveValue);

        } 
	if (strcmp(ParamName, "WelcomeMessage") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->WifiPersonal.WelcomeMessage.ActiveValue);

        } 
	if (strcmp(ParamName, "WelcomeMessage_fre") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->WifiPersonal.WelcomeMessage_fre.ActiveValue);

        } 
	 return -1;
}

BOOL
WiFiPersonalization_SetParamStringValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

    char PartnerID[PARTNER_ID_LEN] = {0};
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
    errno_t rc = -1;
    int ind = -1;

    if((ParamName == NULL) || (pString == NULL))
        return FALSE;

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
    if(AnscValidateInputString(pString) != TRUE)
        return FALSE;

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0') )
   {
   	 /* check the parameter name and set the corresponding value */
            rc = strcmp_s("PartnerHelpLink", strlen("PartnerHelpLink"), ParamName, &ind);
            ERR_CHK(rc);
            if((rc == EOK) && (!ind))
	    {
                 IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->WifiPersonal.PartnerHelpLink.UpdateSource);

                 /* Below condition is added to restrict the maximum pString length less than acceptable value for buffer overflow issues
                  * UpdateJsonParam function doesn't have the maximum permissible string length validation check.
                  */
                 if(strlen(pString) < sizeof(pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue))
                 {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.PartnerHelpLink",PartnerID, pString, requestorStr, currentTime))
			{
                                rc = STRCPY_S_NOCLOBBER(pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue, sizeof(pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue), pString);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

                                rc = STRCPY_S_NOCLOBBER(pBindObj->WifiPersonal.PartnerHelpLink.UpdateSource, sizeof(pBindObj->WifiPersonal.PartnerHelpLink.UpdateSource), requestorStr);
                                if(rc != EOK)
                                {
                                    ERR_CHK(rc);
                                    return FALSE;
                                }

				return TRUE;
			}			
                  }
                  else
                  {
                      CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                      return FALSE;
                  }
	    }
	  
   }
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.
	  *  LocalUI_GetParamStringValue
      *  LocalUI_SetParamStringValue
      *  LocalUI_GetParamBoolValue
      *  LocalUI_SetParamBoolValue
	  
***********************************************************************/

ULONG
LocalUI_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;
	
	 if (strcmp(ParamName, "MSOLogo") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->LocalUI.MSOLogo.ActiveValue);

        } 
	if (strcmp(ParamName, "DefaultLoginUsername") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->LocalUI.DefaultLoginUsername.ActiveValue);
        } 
	if (strcmp(ParamName, "DefaultLoginPassword") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->LocalUI.DefaultLoginPassword.ActiveValue);

        } 
	if (strcmp(ParamName, "MSOLogoTitle") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->LocalUI.MSOLogoTitle.ActiveValue);

        } 
	 return -1;
}

BOOL
LocalUI_SetParamStringValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pString);
	return FALSE;
}

BOOL
LocalUI_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

        if (strcmp(ParamName, "HomeNetworkControl") == 0)
        {
		 /* collect value */
		*pBool = pBindObj->LocalUI.HomeNetworkControl.ActiveValue;
		return TRUE;
        } 
    return FALSE;
}

BOOL
LocalUI_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
	return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.HelpTip.
	  *  HelpTip_GetParamStringValue
      *  HelpTip_SetParamStringValue
	  
***********************************************************************/

ULONG
HelpTip_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;
	
	 if (strcmp(ParamName, "NetworkName") == 0)
        {
	        return  update_pValue(pValue,pulSize, pBindObj->HelpTip.NetworkName.ActiveValue);
        } 
	 return -1;
}

BOOL
HelpTip_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pString);
	return FALSE;
}

/***********************************************************************
APIs for Object:
    DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.
      *  CloudUI_GetParamStringValue
      *  CloudUI_SetParamStringValue

***********************************************************************/

ULONG
CloudUI_GetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =  & pMyObject->UiBrand;

    if (strcmp(ParamName, "brandname") == 0)
    {
        return  update_pValue(pValue,pulSize, pBindObj->CloudUI.brandname.ActiveValue);
    }

    if (strcmp(ParamName, "productname") == 0)
    {
        return  update_pValue(pValue,pulSize, pBindObj->CloudUI.productname.ActiveValue);
    }

    if (strcmp(ParamName, "link") == 0)
    {
        return  update_pValue(pValue,pulSize, pBindObj->CloudUI.link.ActiveValue);
    }
     return -1;
}

BOOL
CloudUI_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =  & pMyObject->UiBrand;

    char PartnerID[PARTNER_ID_LEN] = {0};
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
    errno_t rc = -1;
    int ind = -1;

    if((ParamName == NULL) || (pString == NULL))
        return FALSE;

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
    if(AnscValidateInputString(pString) != TRUE)
        return FALSE;

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0') )
   {
     /* check the parameter name and set the corresponding value */
        rc = strcmp_s("brandname", strlen("brandname"), ParamName, &ind);
        ERR_CHK(rc);
        if((rc == EOK) && (!ind))
        {
            IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->CloudUI.brandname.UpdateSource);

            /* Below condition is added to restrict the maximum pString length less than acceptable value for buffer overflow issues
             * UpdateJsonParam function doesn't have the maximum permissible string length validation check.
             */
            if(strlen(pString) < sizeof(pBindObj->CloudUI.brandname.ActiveValue))
            {
                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.brandname",PartnerID, pString, requestorStr, currentTime))
                {
                    rc = STRCPY_S_NOCLOBBER(pBindObj->CloudUI.brandname.ActiveValue, sizeof(pBindObj->CloudUI.brandname.ActiveValue), pString);
                    if(rc != EOK)
                    {
                        ERR_CHK(rc);
                        return FALSE;
                    }

                    rc = STRCPY_S_NOCLOBBER(pBindObj->CloudUI.brandname.UpdateSource, sizeof(pBindObj->CloudUI.brandname.UpdateSource), requestorStr);
                    if(rc != EOK)
                    {
                        ERR_CHK(rc);
                        return FALSE;
                    }

                    return TRUE;
                }
            }
            else
            {
                CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                return FALSE;
            }
        }
        rc = strcmp_s("productname", strlen("productname"), ParamName, &ind);
        ERR_CHK(rc);
        if((rc == EOK) && (!ind))
        {
            IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->CloudUI.productname.UpdateSource);

            if(strlen(pString) < sizeof(pBindObj->CloudUI.productname.ActiveValue))
            {
                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.productname",PartnerID, pString, requestorStr, currentTime))
                {
                    rc = STRCPY_S_NOCLOBBER(pBindObj->CloudUI.productname.ActiveValue, sizeof(pBindObj->CloudUI.productname.ActiveValue), pString);
                    if(rc != EOK)
                    {
                        ERR_CHK(rc);
                        return FALSE;
                    }

                    rc = STRCPY_S_NOCLOBBER(pBindObj->CloudUI.productname.UpdateSource, sizeof(pBindObj->CloudUI.productname.UpdateSource), requestorStr);
                    if(rc != EOK)
                    {
                        ERR_CHK(rc);
                        return FALSE;
                    }

                    return TRUE;
                }
            }
            else
            {
                CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                return FALSE;
            }
        }
        rc = strcmp_s("link", strlen("link"), ParamName, &ind);
        ERR_CHK(rc);
        if((rc == EOK) && (!ind))
        {
            IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->CloudUI.link.UpdateSource);

            if(strlen(pString) < sizeof(pBindObj->CloudUI.link.ActiveValue))
            {
                if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.link",PartnerID, pString, requestorStr, currentTime))
                {
                    rc = STRCPY_S_NOCLOBBER(pBindObj->CloudUI.link.ActiveValue, sizeof(pBindObj->CloudUI.link.ActiveValue), pString);
                    if(rc != EOK)
                    {
                        ERR_CHK(rc);
                        return FALSE;
                    }

                    rc = STRCPY_S_NOCLOBBER(pBindObj->CloudUI.link.UpdateSource, sizeof(pBindObj->CloudUI.link.UpdateSource), requestorStr);
                    if(rc != EOK)
                    {
                        ERR_CHK(rc);
                        return FALSE;
                    }

                    return TRUE;
                 }
            }
            else
            {
                CcspTraceError(("pString length more than permissible value - %s:%d\n", __FUNCTION__, __LINE__));
                return FALSE;
            }
        }
    }
    return FALSE;
}


#if !defined(_COSA_FOR_BCI_) || defined(_CBR2_PRODUCT_REQ_)
/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.RPC.

    *  RPC_GetParamUlongValue
    *  RPC_SetParamUlongValue
    *  RPC_GetParamStringValue
    *  RPC_SetParamStringValue

***********************************************************************/

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_GetParamUlongValue
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
RPC_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

	
    /* check the parameter name and return the corresponding value */
#ifndef FEATURE_FWUPGRADE_MANAGER
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    if (strcmp(ParamName, "DeferFWDownloadReboot") == 0)
    {
        /* collect value */
        *puLong = pMyObject->DeferFWDownloadReboot;
        return TRUE;
    }	
#endif    
    if (strcmp(ParamName, "RebootPendingNotification") == 0)
    {
        /* collect value */
        *puLong = 0;
        return TRUE;
    }	    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_SetParamUlongValue
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
RPC_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
	
    /* check the parameter name and set the corresponding value */
#ifndef FEATURE_FWUPGRADE_MANAGER
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    if (strcmp(ParamName, "DeferFWDownloadReboot") == 0)
    {
        /* collect value */
		CosaDmlDiSet_DeferFWDownloadReboot(&(pMyObject->DeferFWDownloadReboot),uValue);
		return TRUE;
    } 
#endif    
    if (strcmp(ParamName, "RebootPendingNotification") == 0)
    {
        /* collect value */
        char buff[64] = {0};
        errno_t rc = -1;
        rc = sprintf_s(buff, sizeof(buff),"%lu", uValue);
        if(rc < EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        Send_Notification_Task(buff, NULL, NULL, "reboot-pending", NULL, NULL, NULL, NULL);
        return TRUE;
    }	       
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RPC_GetParamStringValue
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
RPC_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    errno_t  rc  = -1;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "RebootDevice") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pulSize, "");
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "FirmwareDownloadStartedNotification") == 0)
    {
	/* collect value */
	rc = strcpy_s( pValue, *pulSize, pMyObject->FirmwareDownloadStartedNotification);
	if(rc != EOK)
	{
		ERR_CHK(rc);
		return -1;
	}
	return 0;
    }

    if (strcmp(ParamName, "DeviceManageableNotification") == 0)
    {
	/* collect value */
	rc = strcpy_s( pValue, *pulSize, pMyObject->DeviceManageableNotification);
	if(rc != EOK)
	{
		ERR_CHK(rc);
		return -1;
	}
	return 0;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_SetParamStringValue
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
RPC_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    char *current_time = NULL;
    char *priority = NULL;
    char *current_fw_ver = NULL;
    char *download_fw_ver = NULL;
    const char s[2] = ",";
    char *notifyStr = NULL;
    errno_t rc =-1;
    int ind =-1;
    /* check the parameter name and set the corresponding value */

    rc = strcmp_s("RebootDevice",strlen( "RebootDevice"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))  
    {
      
        if( TRUE == CosaDmlDi_ValidateRebootDeviceParam( pString ) )
		{
			CosaDmlDiSet_RebootDevice(pString);
        	return TRUE;
		}
    }

        rc =  strcmp_s("FirmwareDownloadStartedNotification", strlen("FirmwareDownloadStartedNotification"),ParamName, &ind);
	ERR_CHK(rc);
        if((!ind) && (rc == EOK)) 
        {
	    char notifyEnable[64] = {0};
        

             syscfg_get( NULL, "ManageableNotificationEnabled", notifyEnable, sizeof(notifyEnable));
             if(notifyEnable[0] != '\0') 
             {
		  
		  rc = strcmp_s("true", strlen("true"),notifyEnable,&ind);
		  ERR_CHK(rc);
                 if((!ind) && (rc == EOK))  
		  {
		     /* collect value */
		     char buff[128];
		     char *timeValue = NULL;
		     memset(buff, 0, sizeof(buff));
		     snprintf(buff,sizeof(buff),"%s",pString);	
		     rc =  memset_s( pMyObject->FirmwareDownloadStartedNotification, sizeof( pMyObject->FirmwareDownloadStartedNotification ),0, sizeof( pMyObject->FirmwareDownloadStartedNotification ));
		     ERR_CHK(rc);
		     rc = STRCPY_S_NOCLOBBER( pMyObject->FirmwareDownloadStartedNotification,sizeof( pMyObject->FirmwareDownloadStartedNotification ), pString );
		     if(rc != EOK)
                    {
	               ERR_CHK(rc);
	               return FALSE;
                    }
		    
		     timeValue = notifyStr = strdup(buff);
		     // loop through the string to extract all tokens
			if( notifyStr != NULL )
			{
				current_time = strsep(&notifyStr, s);
				priority = strsep(&notifyStr, s);
				current_fw_ver = strsep(&notifyStr, s);
				download_fw_ver = strsep(&notifyStr, s);
			}
			if( current_time != NULL )
			{
				set_firmware_download_start_time(strdup(current_time));
			}
			if(current_time != NULL && priority != NULL && current_fw_ver != NULL && download_fw_ver !=NULL)
			{
				Send_Notification_Task(NULL,current_time, NULL, "firmware-download-started", NULL, priority,current_fw_ver,download_fw_ver);
			}
			else
			{
				CcspTraceWarning(("Received insufficient data to process notification, firmware download started notfication is not sent\n"));			
			}
			free(timeValue);
		}	
                  
                 else
	        {
		   CcspTraceWarning(("ManageableNotificationEnabled is false, firmware download start notification is not sent\n"));
	        }
	   }
	   else
	   {
		   CcspTraceWarning(("ManageableNotificationEnabled is false, firmware download start notification is not sent\n"));
	   }
	return TRUE;
      }

        rc = strcmp_s( "DeviceManageableNotification",strlen("DeviceManageableNotification"),ParamName, &ind);
	ERR_CHK(rc);
       if((!ind) && (rc == EOK)) 
	 
       {
          char status[64] = {0};
        
          syscfg_get( NULL, "ManageableNotificationEnabled", status, sizeof(status));
          if(status[0] != '\0') 
          {
		rc =strcmp_s("true", strlen("true"),status,&ind);
		ERR_CHK(rc);
                if((!ind) && (rc == EOK)) 
		{
                   
		    rc = memset_s( pMyObject->DeviceManageableNotification,sizeof( pMyObject->DeviceManageableNotification ), 0,sizeof( pMyObject->DeviceManageableNotification ));
		    ERR_CHK(rc);
	            rc = STRCPY_S_NOCLOBBER( pMyObject->DeviceManageableNotification,sizeof( pMyObject->DeviceManageableNotification ) ,pString );
	           if(rc != EOK)
                  {
                    ERR_CHK(rc);
                    return FALSE;
                  }
                 Send_Notification_Task(NULL, NULL, NULL, "fully-manageable", pString,NULL, NULL,NULL);
	      }	 
              else
	     {
		CcspTraceWarning(("ManageableNotificationEnabled is false, device manage notification is not sent\n"));
	     }
        }
	else
	{
		CcspTraceWarning(("ManageableNotificationEnabled is false, device manage notification is not sent\n"));
	}
        return TRUE;
     }

     CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
	 return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_GetParamBoolValue
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
RPC_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    if (strcmp(ParamName, "AbortReboot") == 0)
    {
        /*CcspTraceWarning(("supported parameter '%s'\n", ParamName));*/
        *pBool = pMyObject->AbortReboot;
        return TRUE;
    }

    if (strcmp(ParamName, "FirmwareDownloadCompletedNotification") == 0)
    {
	/*CcspTraceWarning(("supported parameter '%s'\n", ParamName));*/
	*pBool = pMyObject->FirmwareDownloadCompletedNotification;
	return TRUE;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_SetParamBoolValue
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
RPC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
   UNREFERENCED_PARAMETER(hInsContext);
   PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

   if (strcmp(ParamName, "AbortReboot") == 0)
   {
       {
           AnscTraceWarning(("Triggering abortReboot from TR181\n"));
           // static collection as we don't want upgrade path to be changed without a reboot
           static BOOL rdkfwupgraderEnabledCollected = false;
           static BOOL RDKFWUpgraderEnabled = false;

           if (!rdkfwupgraderEnabledCollected) {
               if (RDKFirmwareUpgrader_GetParamBoolValue(hInsContext,
                                                         "Enable",
                                                         &RDKFWUpgraderEnabled) ) {
                   rdkfwupgraderEnabledCollected = true;
               }
           }

           // NOTE:: this might have an addional issue, rfc enabled but rdkfwupgrader was disabled at
           // build time. Do we need to take care of that like checking for presense /usr/bin/rdkfwupgrader
           // at runtime
           if (RDKFWUpgraderEnabled) {
               v_secure_system("/lib/rdk/rdkfwupgrader_abort_reboot.sh &");
               return TRUE; //always true, let the statemachine decide if there is a reboot operation pending or not.
           } else {
               FILE *file = NULL;
               FILE *Abortfile = NULL;
               if ((file = fopen("/tmp/.deferringreboot", "r"))) {
                   if ((Abortfile = fopen("/tmp/AbortReboot", "r"))) {
                       fclose(Abortfile);
                       fclose(file);
                       CcspTraceWarning(("Abort already done '%s'\n", ParamName));
                       return TRUE;
                   }
                   pMyObject->AbortReboot = bValue;
                   if(pMyObject->AbortReboot == TRUE)
                       v_secure_system("touch /tmp/AbortReboot");
                   else
                       CcspTraceWarning(("Parameter '%s' set to false\n", ParamName));
                   fclose(file);
                   return TRUE;
               } else {
                   CcspTraceWarning(("Invalid request for parameter, no FW DL reboot wait in progress '%s'\n", ParamName));
                   return FALSE;
               }
           }
       }
       return FALSE;
   } 

	if (strcmp(ParamName, "FirmwareDownloadCompletedNotification") == 0)
	{
		char notifyEnable[64];
		memset(notifyEnable, 0, sizeof(notifyEnable));

		syscfg_get( NULL, "ManageableNotificationEnabled", notifyEnable, sizeof(notifyEnable));
		if((notifyEnable[0] != '\0') && (strncmp(notifyEnable, "true", strlen("true")) == 0))
		{
			/* collect value */
			char buff[8];

			memset(buff, 0, sizeof(buff));
			snprintf(buff, sizeof(buff), "%s", bValue ? "true" : "false");
			pMyObject->FirmwareDownloadCompletedNotification = bValue;
			char *start_time = get_firmware_download_start_time();
			Send_Notification_Task(NULL, start_time, buff, "firmware-download-completed", NULL, NULL, NULL, NULL);
		}
		else
		{
			CcspTraceWarning(("ManageableNotificationEnabled is false, firmware download completed notfication is not sent\n"));
		}
		return TRUE;
	}
	/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
	return FALSE;
}
#endif

/*********************************************************************************************

   caller: owner of this object

   prototype:

       BOOL
       SyseventTracer_GetParamBoolValue
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
SyseventTracer_GetParamBoolValue
(
ANSC_HANDLE                 hInsContext,
char*                       ParamName,
BOOL*                       pBool
)
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "Enable") == 0)
    {
        if(access("/nvram/sysevent_tracer_enabled", F_OK) == 0)
        {
            *pBool = TRUE;
        }
        else
        {
            *pBool = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SyseventTracer_SetParamBoolValue
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
SyseventTracer_SetParamBoolValue
(
ANSC_HANDLE                 hInsContext,
char*                       ParamName,
BOOL                        bValue
)
{
    if (IsBoolSame(hInsContext, ParamName, bValue, SyseventTracer_GetParamBoolValue))
        return TRUE;

    FILE *log_fp = fopen("/rdklogs/logs/sysevent_tracer.txt", "a+");

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (bValue == 1)
        {
            v_secure_system("touch /nvram/sysevent_tracer_enabled");
            CcspTraceInfo(("Successfully enabled Sysevent tracer\n"));
            fprintf(log_fp, "Successfully enabled Sysevent tracer\n");
            fclose(log_fp);
            return TRUE;
        }
        else if (bValue == 0)
        {
            v_secure_system("rm -f /nvram/sysevent_tracer_enabled");
            CcspTraceInfo(("Successfully disabled Sysevent tracer\n"));
            fprintf(log_fp, "Successfully disabled Sysevent tracer\n");
            fclose(log_fp);
            return TRUE;
        }
    }
    fclose(log_fp);
    return FALSE;
}

//sukant start
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BLE_GetParamBoolValue
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
BLE_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];
    if (strcmp(ParamName, "Discovery") == 0)
    {
        /* collect value */
        /* CID: 59124 Array compared against 0*/
        if(!syscfg_get( NULL, "BLEDiscovery", buf, sizeof(buf)))
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
           return TRUE;
        } else {
           return FALSE;
        }
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BLE_SetParamBoolValue
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
BLE_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, BLE_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Discovery") == 0)
    {
        syscfg_set_commit(NULL, "BLEDiscovery", (bValue == TRUE) ? "true" : "false");
        return TRUE;
    }


    return FALSE;
}



BOOL
PeriodicBeacon_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    CcspTraceInfo(("PeriodicBeacon_GetParamBoolValue \n"));
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        memset(buf, 0, sizeof(buf));
        syscfg_get( NULL, "BLEPeriodicBeacon", buf, sizeof(buf));
        /* CID 168163 fix*/
        if( buf[0] != '\0' )
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }

    return FALSE;
}


BOOL
PeriodicBeacon_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    CcspTraceInfo(("PeriodicBeacon_SetParamBoolValue \n"));
    UNREFERENCED_PARAMETER(hInsContext);
    pthread_t tid;
	
    if (IsBoolSame(hInsContext, ParamName, bValue, PeriodicBeacon_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        syscfg_set_commit(NULL, "BLEPeriodicBeacon", (bValue == TRUE) ? "true" : "false");

        pthread_create(&tid, NULL, handleBleRestart, NULL);
        return TRUE;
    }

    return FALSE;
}


BOOL
PeriodicBeacon_GetParamUlongValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{

    CcspTraceInfo(("PeriodicBeacon_GetParamUlongValue \n"));
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8] = {0};

    if (strcmp(ParamName, "frequency") == 0)
    {
        syscfg_get( NULL, "BLEPeriodicBeaconFrequency", buf, sizeof(buf));
        if( 0 == strlen(buf) )
	{
	    CcspTraceInfo(("Return the default Value \n"));
	    *puLong = 2; // default beacon frequency;
            return TRUE;
	}
        *puLong = atol(buf);
        return TRUE;
    }
    AnscTraceWarning(("%s is invalid argument!\n", ParamName));
    return FALSE;

}


BOOL
PeriodicBeacon_SetParamUlongValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue

    )
{

	 CcspTraceInfo(("PeriodicBeacon_SetParamUlongValue \n"));
	 UNREFERENCED_PARAMETER(hInsContext);
         pthread_t tid;

 	if (strcmp(ParamName, "frequency") == 0)
	{
        	if (syscfg_set_u_commit(NULL, "BLEPeriodicBeaconFrequency", uValue) != 0)
        	{
            		AnscTraceWarning(("%s syscfg_set failed!\n", ParamName));
            		return FALSE;
        	}
                pthread_create(&tid, NULL, handleBleRestart, NULL);
	}

	return TRUE;
}


BOOL 
Broadcast_GetParamBoolValue(
    ANSC_HANDLE hInsContext,
    char *ParamName,
    BOOL *pBool)
{
    CcspTraceInfo(("Broadcast_GetParamBoolValue \n"));
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8] = {0};
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /*CID: 334992 - Array Compared against null - fixed*/
        syscfg_get(NULL, "BLEBroadcast", buf, sizeof(buf));
        /*CID: 334992 - Array Compared against null - fixed*/
        if ( buf[0] != '\0')
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }

    return FALSE;
}


BOOL 
Broadcast_SetParamBoolValue(
    ANSC_HANDLE hInsContext,
    char *ParamName,
    BOOL bValue)
{
    CcspTraceInfo(("Broadcast_SetParamBoolValue \n"));
    UNREFERENCED_PARAMETER(hInsContext);

    if (IsBoolSame(hInsContext, ParamName, bValue, Broadcast_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        if (bValue == TRUE)
        {
            syscfg_set(NULL, "BLEBroadcast", "true");
            syscfg_commit();
        }
        else
        {
            syscfg_set(NULL, "BLEBroadcast", "false");
            syscfg_commit();
        }

        return TRUE;
    }

    return FALSE;
}

BOOL
xBlueTooth_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
          UNREFERENCED_PARAMETER(hInsContext);
          if (strcmp(ParamName, "LimitBeaconDetection") == 0)
          {
               char buf[8] = {0};
               *pBool = FALSE;
               syscfg_get( NULL, "limit_beacon_detection", buf, sizeof(buf));
               if( buf[0] != '\0' )
               {
                  if (!strncasecmp(buf, "true", 4))
                  {
                      *pBool = TRUE;
                      return TRUE;
                  }
                  else
                  {
                      *pBool = FALSE;
                      return TRUE;
                  }
               }
           }
        return FALSE;
}

void *handleBleRestart(void *arg)
{
	UNREFERENCED_PARAMETER(arg);
    CcspTraceInfo(("handleBleRestart Thread Invoked \n"));
    pthread_detach(pthread_self());
    v_secure_system("systemctl restart ble");
    CcspTraceInfo(("handleBleRestart Completed \n"));
    return NULL;
}

BOOL
xBlueTooth_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
     CcspTraceInfo(("xBlueTooth_SetParamBoolValue \n"));
     pthread_t tid;
    if (strcmp(ParamName, "LimitBeaconDetection") == 0)
    {
        if(syscfg_set_commit(NULL, "limit_beacon_detection", bValue ? "true" : "false"))
        {
            CcspTraceError(("syscfg_set failed\n"));
        }
        pthread_create(&tid, NULL, handleBleRestart, NULL); 
        return TRUE;
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        MessageBusSource_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
MessageBusSource_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
 UNREFERENCED_PARAMETER(hInsContext);
 if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "MessageBusSource", value, sizeof(value)) == 0 )
        {
            /* CID: 63644 Array compared against 0*/
                 if (strcmp(value, "true") == 0)
                     *pBool = TRUE;
                 else
                     *pBool = FALSE;

            return TRUE;
        }
        else
        {
            CcspTraceError(("syscfg_get failed for MessageBusSource\n"));
        }
    }
  return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        MessageBusSource_SetParamBoolValu
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
MessageBusSource_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
  if (IsBoolSame(hInsContext, ParamName, bValue, MessageBusSource_GetParamBoolValue))
        return TRUE;

  if (strcmp(ParamName, "Enable") == 0)
    {
        if( syscfg_set_commit(NULL, "MessageBusSource", bValue ? "true" : "false") != 0 )
        {
            CcspTraceError(("syscfg_set failed for MessageBusSource\n"));
        }
        else
        {
            return TRUE;
        }

    }
  return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        TR104_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
#ifdef MTA_TR104SUPPORT
BOOL
TR104_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "TR104enable", value, sizeof(value)) == 0 )
        {
		/* CID: 71977 Array compared against 0*/
		if (strcmp(value, "true") == 0)
			*pBool = TRUE;
		else
			*pBool = FALSE;
		return TRUE;
	}
        else
        {
            CcspTraceError(("syscfg_get failed for TR104enable\n"));
        }
    }
  return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        TR104_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
TR104_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
  if (IsBoolSame(hInsContext, ParamName, bValue, TR104_GetParamBoolValue))
        return TRUE;

  if (strcmp(ParamName, "Enable") == 0)
    {
        if( syscfg_set_commit(NULL, "TR104enable", bValue ? "true" : "false") != 0 )
        {
            CcspTraceError(("syscfg_set failed for TR104enable \n"));
        }
        else
        {
            return TRUE;
        }
    }
  return FALSE;
}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        UPnPRefactor_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
UPnPRefactor_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
 UNREFERENCED_PARAMETER(hInsContext);
 if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "Refactor", value, sizeof(value)) == 0 )
        {
            /* CID 71977 fix*/
            if( value[0] != '\0' )
            {
                 if (strcmp(value, "true") == 0)
                     *pBool = TRUE;
                 else
                     *pBool = FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceError(("syscfg_get failed for UPnP Refactor\n"));
        }
    }
  return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        UPnPRefactor_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
UPnPRefactor_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
  if (IsBoolSame(hInsContext, ParamName, bValue, UPnPRefactor_GetParamBoolValue))
        return TRUE;

  if (strcmp(ParamName, "Enable") == 0)
    {
        if( syscfg_set_commit(NULL, "Refactor", bValue ? "true" : "false") != 0 )
        {
            CcspTraceError(("syscfg_set failed for UPnP Refactor \n"));
        }
        else
        {
            return TRUE;
        }
    }
  return FALSE;
}

#if defined(FEATURE_MAPT) || defined(FEATURE_SUPPORT_MAPT_NAT46)
/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        MAPT_DeviceInfo_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
MAPT_DeviceInfo_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "MAPT_Enable", value, sizeof(value)) == 0 )
        {
		if (strcmp(value, "true") == 0)
			*pBool = TRUE;
		else
			*pBool = FALSE;
		return TRUE;
	}
        else
        {
            CcspTraceError(("syscfg_get failed for MAPT_Enable\n"));
        }
    }
  return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        MAPT_DeviceInfo_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
MAPT_DeviceInfo_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
  if (IsBoolSame(hInsContext, ParamName, bValue, MAPT_DeviceInfo_GetParamBoolValue))
        return TRUE;

  if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "MAPT_Enable", bValue ? "true" : "false") != 0 )
        {
            CcspTraceError(("syscfg_set failed for MAPT_Enable \n"));
        }
        else
        {
#if defined(FEATURE_MAPT) || defined(FEATURE_SUPPORT_MAPT_NAT46)
            v_secure_system("sysevent set MAPT_Enable %s", bValue ? "true" : "false");
#endif
            return TRUE;
        }
    }
  return FALSE;
}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        HwHealthTestEnable_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
#ifdef COLUMBO_HWTEST
BOOL
HwHealthTestEnable_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{

    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "hwHealthTest", value, sizeof(value)) == 0 )
        {
            /* CID 161852 fix*/
            if( value[0] != '\0' )
            {
                 if (strncmp(value, "true", sizeof(value)) == 0)
                     *pBool = TRUE;
                 else
                     *pBool = FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceError(("syscfg_get failed for hwHealthTest Enable\n"));
        }
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        HwHealthTestEnable_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
HwHealthTestEnable_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (IsBoolSame(hInsContext, ParamName, bValue, HwHealthTestEnable_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if( syscfg_set_commit(NULL, "hwHealthTest", bValue ? "true" : "false") != 0 )
        {
            CcspTraceError(("syscfg_set failed for hwHealthTest Enable \n"));
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        HwHealthTest_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            )



    description:

        This function is called to retrieve unsigned long parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned unsigned long value;

    return:     TRUE if succeeded.


**********************************************************************/

BOOL
HwHealthTest_GetParamUlongValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    if (strcmp(ParamName, "cpuThreshold") == 0)
    {
        *puLong = pMyObject->HwHealtTestPTR.CPUThreshold;
        if(0 == *puLong)
        {
            //Value is not set. Hence using default value
            *puLong = DEFAULT_HWST_PTR_CPU_THRESHOLD;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "dramThreshold") == 0)
    {
        *puLong = pMyObject->HwHealtTestPTR.DRAMThreshold;
        if(0 == *puLong)
        {
            //Value is not set. Hence using default value
            *puLong = DEFAULT_HWST_PTR_DRAM_THRESHOLD;
        }
        return TRUE;
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        HwHealthTest_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uLong
            )


    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uLong
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
HwHealthTest_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uLong
    )
{
    /*Changes for RDKB-31737: AC#2 and AC#3
      To reject the set request from any API other than RFC and dmcli
    */
    if(strcmp(getRequestorString(), BS_SOURCE_RFC_STR))
    {
        return FALSE;
    }

    if(GetAvailableSpace_tmp() < HWSELFTEST_START_MIN_SPACE)
    {
        CcspTraceError(("\nNot enough space in DRAM to save the threshold value in .hwselftest_settings file. Exit\n"));
        return FALSE;
    }

    //Check if the new value is same as the old. If so, it is
    //not required to update settings file.
    if (IsUlongSame(hInsContext, ParamName, uLong, HwHealthTest_GetParamUlongValue))
        return TRUE;

    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    if (strcmp(ParamName, "cpuThreshold") == 0)
    {
        //Threshold value should be in range as per the requirement in COLBO-132
        if( (uLong >= 1) && ( uLong <= 95))
        {
            pMyObject->HwHealtTestPTR.CPUThreshold = uLong;

            //Write the parameter to settings file
            char buf[8] = {'\0'};
            snprintf(buf, sizeof(buf), "%d", (int)uLong);
            UpdateSettingsFile("HWST_CPU_THRESHOLD=", buf);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    if (strcmp(ParamName, "dramThreshold") == 0)
    {
        //Threshold value should be in range as per the requirement in COLBO-132
        if( uLong >= 20 )
        {
            pMyObject->HwHealtTestPTR.DRAMThreshold = uLong;

            char buf[8] = {'\0'};
            snprintf(buf, sizeof(buf), "%d", (int)uLong);
            UpdateSettingsFile("HWST_DRAM_THRESHOLD=",buf);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        HwHealthTestPTREnable_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
HwHealthTestPTREnable_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "enable") == 0)
    {
        PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        *pBool = pMyObject->HwHealtTestPTR.PTREnable;
        return TRUE;
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        HwHealthTestPTREnable_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
HwHealthTestPTREnable_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /*Changes for RDKB-31737: AC#1 and AC#3
      To reject the set request from any API other than RFC and dmcli
    */
    if(strcmp(getRequestorString(), BS_SOURCE_RFC_STR))
    {
        return FALSE;
    }

    if(GetAvailableSpace_tmp() < HWSELFTEST_START_MIN_SPACE)
    {
        CcspTraceError(("\nNot enough space in DRAM to enable PTR. Exit\n"));
        return FALSE;
    }

    if (IsBoolSame(hInsContext, ParamName, bValue, HwHealthTestPTREnable_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "enable") == 0)
    {
        PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

        char buf[8] = {'\0'};
        errno_t rc  = -1;
        snprintf(buf, sizeof(buf), "%s", bValue ? "true" : "false");

        //Read client version
        FILE* fp = fopen("/tmp/.hwst_run", "r");
        char* clientVer = (char*)malloc(8*sizeof(char));
        char version[8] = {'\0'};
        if( NULL != fp)
        {
            if( NULL != clientVer )
            {
                /* CID: 163532 fix*/
                if (fscanf(fp, "%7s", clientVer) == 1)
                {
                    rc = strcpy_s(version,sizeof(version),clientVer);
                    if(rc != EOK)
                    {
                    ERR_CHK(rc);
                    free(clientVer);
                    fclose(fp);
                    return FALSE;
                    }
                }
                free(clientVer);
                clientVer = NULL;
            }
        }
        if( NULL != clientVer )
        {
            free(clientVer);
        }

        if( (NULL !=fp) && strcmp(version, "0002") && !strcmp(buf,"true"))
        {
            CcspTraceError(("\nMultiple connection not allowed"));
            fclose(fp);
            return FALSE;
        }

        pMyObject->HwHealtTestPTR.PTREnable = bValue;

        //If the PTR is enabled, add the hwselftest to crontab
        if(!strcmp(buf, "true"))
        {
            if(strcmp(version, "0002"))
            {
                char cmd[128] = {0};
                rc = strcpy_s(cmd, sizeof(cmd), "/usr/bin/hwselftest_cronjobscheduler.sh true &");
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return FALSE;
                }
                CcspTraceInfo(("\nExecuting command: %s\n", cmd));
                v_secure_system("/usr/bin/hwselftest_cronjobscheduler.sh true &");
            }
            else
            {
                CcspTraceInfo(("Hwselftest service already running through PTR"));
            }
        }
        else
        {
            //Remove all the hwselftest job from crontab
            v_secure_system("/usr/bin/hwselftest_cronjobscheduler.sh false &");
        }
	// CID 163522 : Resource leak (RESOURCE_LEAK)
	if(fp)
            fclose(fp);
        return TRUE;
   }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        HwHealthTestPTRFrequency_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            )



    description:

        This function is called to retrieve unsigned long parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned unsigned long value;

    return:     TRUE if succeeded.


**********************************************************************/

BOOL
HwHealthTestPTRFrequency_GetParamUlongValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "frequency") == 0)
    {
        PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        *puLong = pMyObject->HwHealtTestPTR.Frequency;
        if(0 == *puLong)
        {
            //Value is not set. Hence using default value
            *puLong = DEFAULT_HWST_PTR_FREQUENCY;
        }
        return TRUE;
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        HwHealthTestPTRFrequency_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uLong
            )


    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uLong
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
HwHealthTestPTRFrequency_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uLong
    )
{
    /*Changes for RDKB-31737: AC#1 and AC#3
      To reject the set request from any API other than RFC and dmcli
    */
    if(strcmp(getRequestorString(), BS_SOURCE_RFC_STR))
    {
        return FALSE;
    }

    if(GetAvailableSpace_tmp() < HWSELFTEST_START_MIN_SPACE)
    {
        CcspTraceError(("\nNot enough space in DRAM to save the value for PTR frequency in .hwselftest_settings file. Exit\n"));
        return FALSE;
    }

    //Check if the new value is same as the old. If so, it is
    //not required to update settings file.
    if (IsUlongSame(hInsContext, ParamName, uLong, HwHealthTestPTRFrequency_GetParamUlongValue))
        return TRUE;

    if (strcmp(ParamName, "frequency") == 0)
    {
        PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

        // Frequency should be minimum 2 minutes as per the requirement in COLBO 132.
        if( uLong >= 2 )
        {
            pMyObject->HwHealtTestPTR.Frequency = uLong;

            char buf[8] = {'\0'};
            snprintf(buf, sizeof(buf), "%d", (int)uLong);
            UpdateSettingsFile("HWST_PERIODIC_FREQ=", buf);

            // Call the cronjob scheduler.sh script to update the cron job

            //Read the PTR enable param
            if (IsBoolSame(hInsContext, "enable", true, HwHealthTestPTREnable_GetParamBoolValue))
            {
                CcspTraceInfo(("\n\nExecuting the command: /usr/bin/hwselftest_cronjobscheduler.sh true frequencyUpdate"));
                v_secure_system("/usr/bin/hwselftest_cronjobscheduler.sh true frequencyUpdate");
            }
            else
            {
                CcspTraceInfo(("\n\nExecuting the command: /usr/bin/hwselftest_cronjobscheduler.sh false"));
                v_secure_system("/usr/bin/hwselftest_cronjobscheduler.sh false" );
            }
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        Telemetry_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
Telemetry_GetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool) {
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0) {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "T2Enable", value, sizeof(value)) == 0 ) {
		/*CID: 60562 Array compared against 0*/
		if (strcmp(value, "true") == 0)
			*pBool = TRUE;
		else
			*pBool = FALSE;
		return TRUE;
	} else {
            CcspTraceError(("syscfg_get failed for MessageBusSource\n"));
        }
    }
    else if(AnscEqualString(ParamName, "DCMConfigForceDownload", TRUE)) {
        *pBool = FALSE;
       return TRUE;
    }
    else
    {
        CcspTraceError(("%s: Unknown parameter %s\n", __FUNCTION__, ParamName));
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        Telemetry_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
Telemetry_SetParamBoolValue (ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue) {
    if (IsBoolSame(hInsContext, ParamName, bValue, Telemetry_GetParamBoolValue))
        return TRUE;

    if (strcmp(ParamName, "Enable") == 0) {
        if (syscfg_set(NULL, "T2Enable", bValue ? "true" : "false") != 0) {
            CcspTraceError(("syscfg_set failed for Telemetry.Enable\n"));
        } else {
            if (syscfg_set_commit(NULL, "T2Version", bValue ? "2.0.1" : "1") != 0) {
                CcspTraceError(("syscfg_set failed\n"));
            } else {
                return TRUE;
            }
        }

    }
    else if (AnscEqualString(ParamName, "DCMConfigForceDownload", TRUE)) {
        if (bValue) {
            char value[8] = {'\0'};
            char enabled_version[MAX_T2_VER_LEN + 16] = {'\0'};
            snprintf(enabled_version, sizeof(enabled_version), "T2Enable");
            if (syscfg_get(NULL, enabled_version, value, sizeof(value)) == 0) {
                if (value[0] != '\0') {
                    if (strcmp(value, "true") == 0) {
                        pid_t pid = 0;
                        FILE *fp = popen("pidof telemetry2_0", "r");
                        if (fp) {
                            if (fscanf(fp, "%d", &pid) == 1) {
                                CcspTraceInfo(("Telemetry_SetParamBoolValue: Found telemetry2_0 PID: %d\n", pid));
                                if (kill(pid, 12) == -1) {
                                    CcspTraceError(("Telemetry_SetParamBoolValue: Error sending signal 12 to PID %d\n", pid));
                                } else {
                                    CcspTraceInfo(("Telemetry_SetParamBoolValue: Successfully sent signal 12 to PID %d\n", pid));
                                }
                            } else {
                                CcspTraceInfo(("Telemetry_SetParamBoolValue: telemetry2_0 process not found.\n"));
                            }
                            pclose(fp);
                        }
                    }
                }
            } else {
                CcspTraceError(("Telemetry_SetParamBoolValue: Failed to get value for %s\n", enabled_version));
                CcspTraceError(("Telemetry_SetParamBoolValue: syscfg_get returned an error for key %s\n", enabled_version));
            }
        }
        return TRUE;
    }
    else
    {
        CcspTraceError(("%s Unknown parameter %s\n", __FUNCTION__, ParamName));
    }
    return FALSE;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TelemetryEndpoint_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue
            );

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The string value buffer;


    return:     TRUE if succeeded;
                FALSE if not supported.

**********************************************************************/
ULONG
Telemetry_GetParamStringValue (ANSC_HANDLE hInsContext, char* ParamName, char* pValue,
                               ULONG* pUlSize) {
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pUlSize);
    errno_t   rc  = -1;
    /* Required for xPC sync */
    if (strcmp(ParamName, "ConfigURL") == 0) {
        /* collect value */
        char buf[128] = {'\0'};
        /* CID: 68487 Array compared against 0*/
        /* CID: 63245 Logically dead code*/
        if(!syscfg_get(NULL, "T2ConfigURL", buf, sizeof(buf)))
        {
            rc = strcpy_s(pValue, *pUlSize, buf);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
            return 0;
        }
        return -1;
    }

    if (strcmp(ParamName, "Version") == 0) {
        /* collect value */
        char buf[MAX_T2_VER_LEN] = {'\0'};
        /* CID: 68487 Array compared against 0*/
        if(!syscfg_get(NULL, "T2Version", buf, sizeof(buf)))
        {
            rc = strcpy_s(pValue, *pUlSize, buf);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
            return 0;
        }
        return -1;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Telemetry_SetParamStringValue
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
Telemetry_SetParamStringValue (ANSC_HANDLE hInsContext, char* ParamName, char* pString) {

    if (IsStringSame(hInsContext, ParamName, pString, Telemetry_GetParamStringValue))
        return TRUE;

    if (strcmp(ParamName, "ConfigURL") == 0) {
        if (syscfg_set_commit(NULL, "T2ConfigURL", pString) != 0) {
            CcspTraceError(("syscfg_set failed\n"));

        } else {
            return TRUE;
        }
    }

    if (strcmp(ParamName, "Version") == 0) {

        if ((strncmp(pString, "2", MAX_T2_VER_LEN) == 0) || (strncmp(pString, "2.0.1", MAX_T2_VER_LEN) == 0)) {
            if (syscfg_set_commit(NULL, "T2Version", pString) != 0) {
                CcspTraceError(("syscfg_set failed\n"));
            } else {
                return TRUE;
            }
        } else {
            CcspTraceError(("Unsupported version value : %s \n", pString ));
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
#if !defined (NO_MOCA_FEATURE_SUPPORT)
/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        MocaAccountIsolation_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
MocaAccountIsolation_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
	UNREFERENCED_PARAMETER(hInsContext);
	/*RDKB-28819 : TR-181 implementation
	DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.MocaAccountIsolation.Enable*/
	if (strcmp(ParamName, "Enable") == 0)
	{
		char value[8];
                memset(value, 0, sizeof(value));
		syscfg_get(NULL,"enableMocaAccountIsolation",value, sizeof(value));
        /* CID 143845 fix */
		if( value[0] != '\0' )
		{
			if (strcmp(value, "true") == 0)
			*pBool = TRUE;
			else
			*pBool = FALSE;
		}
                else
                *pBool = FALSE;
                
		return TRUE;
	}
  CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
  return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        MocaAccountIsolation_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
MocaAccountIsolation_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
	UNREFERENCED_PARAMETER(hInsContext);
	/*RDKB-28819 : TR-181 implementation
	DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.MocaAccountIsolation.Enable*/
	if (strcmp(ParamName, "Enable") == 0)
	{
		if (syscfg_set_commit(NULL, "enableMocaAccountIsolation", (bValue == TRUE) ? "true" : "false") != 0)
		{
			AnscTraceWarning(("syscfg_set enableMocaAccountIsolation failed\n"));
                        return FALSE;
		}

                v_secure_system("sysevent set firewall-restart");

		return TRUE;
	}
  CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
  return FALSE;
}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        CaptivePortalForNoCableRF_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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

#if defined (_XB6_PRODUCT_REQ_)
BOOL
CaptivePortalForNoCableRF_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
 if (strcmp(ParamName, "Enable") == 0)
    {
	 char value[8];
         /* CID: 92210 Array compared against 0*/
	 if(!syscfg_get(NULL,"enableRFCaptivePortal",value, sizeof(value)))
         {
		 if (strcmp(value, "true") == 0)
			 *pBool = TRUE;
		 else
	  		 *pBool = FALSE;
         } else 
            return FALSE;

	 return TRUE;
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        CaptivePortalForNoCableRF_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
CaptivePortalForNoCableRF_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
  UNREFERENCED_PARAMETER(hInsContext);

  if (strcmp(ParamName, "Enable") == 0)
    {
	  if (syscfg_set_commit(NULL, "enableRFCaptivePortal", (bValue == TRUE) ? "true" : "false") != 0)
	  {
		 AnscTraceWarning(("syscfg_set enableRFCaptivePortal failed\n"));
	  }
	  
	  return TRUE;
    }
    return FALSE;
}
#endif

/**
 *  RFC Features SecureWebUI
*/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SecureWebUI_GetParamBoolValue
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
SecureWebUI_GetParamBoolValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL*                       pBool
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8];

	/* check the parameter name and return the corresponding value */

    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /* CID: 68633 Array compared against 0*/
        if(!syscfg_get( NULL, "SecureWebUI_Enable", buf, sizeof(buf)))
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
            return TRUE;
        } else {
              return FALSE;
        }
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SecureWebUI_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       bValue
            );

    description:

        This function is called to set Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       bValue
                The buffer with updated value

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
SecureWebUI_SetParamBoolValue

(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (IsBoolSame(hInsContext, ParamName, bValue, SecureWebUI_GetParamBoolValue))
    {
        CcspTraceInfo(("[%s:] SecureWebUI Set current and previous values are same\n", __FUNCTION__ ));
        return TRUE;
    }

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "SecureWebUI_Enable", (bValue == TRUE) ? "true" : "false") != 0)
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        }
        else
        {
            v_secure_system("sysevent set dhcp_server-restart");
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SecureWebUI_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue
            );

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The string value buffer;


    return:     0 if succeeded;
                -1 if not supported.

**********************************************************************/
ULONG
    SecureWebUI_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pUlSize);
    errno_t  rc = -1;
    /* Required for xPC sync */
    if (strcmp(ParamName, "LocalFqdn") == 0)
    {
        /* collect value */
        char buf[64];
         /*CID: 59203 Array compared against 0*/
        if(!syscfg_get( NULL, "SecureWebUI_LocalFqdn", buf, sizeof(buf)))
        {
            rc = strcpy_s(pValue, *pUlSize, buf);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
            return 0;
        }
        return -1;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SecureWebUI_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value to be set;

    return:     TRUE if succeeded.

	**********************************************************************/
BOOL
    SecureWebUI_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "LocalFqdn") == 0)
    {
        if (syscfg_set_commit(NULL, "SecureWebUI_LocalFqdn", pString) != 0)
        {
            CcspTraceError(("syscfg_set failed\n"));

        }
        else
        {
            return TRUE;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**
 *  RFC Feature XHFW
*/
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        XHFW_GetParamBoolValue
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
#if defined(_XB6_PRODUCT_REQ_) || defined(_XB7_PRODUCT_REQ_)
BOOL
XHFW_GetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "XHFW_Enable", value, sizeof(value)) == 0 )
        {
            /* CID: 155008 Array name value  compared against 0*/
            *pBool = (strcmp(value, "true") == 0) ? TRUE : FALSE;
            return TRUE;
        }
        else
        {
            CcspTraceError(("syscfg_get failed for XHFW.Enable\n"));
        }
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        XHFW_SetParamBoolValue
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
XHFW_SetParamBoolValue (ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    UNREFERENCED_PARAMETER(hInsContext);
    BOOL result = FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "XHFW_Enable", bValue ? "true" : "false") != 0)
        {
            CcspTraceError(("syscfg_set failed for XHFW.Enable\n"));
        }
        else
        {
            result = TRUE;
        }

        if (bValue)
        {
            v_secure_system("systemctl start zilker");
        }
        else
        {
            v_secure_system("systemctl stop zilker");
        }
    }
    return result;
}

/**
 *  RFC Feature NIM
*/
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        device.nim_GetParamBoolValue
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
NIM_GetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "NIM_Enable", value, sizeof(value)) == 0 )
        {
            /* CID: 155008 Array name value  compared against 0*/
            *pBool = (strcmp(value, "true") == 0) ? TRUE : FALSE;
            return TRUE;
        }
        else
        {
            CcspTraceError(("syscfg_get failed for NIM_Enable\n"));
        }
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        NIM_SetParamBoolValue
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
NIM_SetParamBoolValue (ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    UNREFERENCED_PARAMETER(hInsContext);
    BOOL result = FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "NIM_Enable", bValue ? "true" : "false") != 0)
        {
            CcspTraceError(("syscfg_set failed for NIM_Enable\n"));
        }
        else
        {
            result = TRUE;
        }

        if (bValue)
        {
            v_secure_system("systemctl restart zilker");
            v_secure_system("systemctl reset-failed otbr-agent.path otbr-agent");
            v_secure_system("systemctl restart otbr-agent");
        }
        else
        {
            v_secure_system("systemctl stop otbr-agent");
            v_secure_system("systemctl restart zilker");
        }
    }
    return result;
}
#endif

static void Replace_AllOccurrence(char *str, int size, char ch, char Newch)
{
  int i=0;
  for(i = 0; i<size-1; i++)
  {
      if(str[i] == ch || str[i] == '\r')
      {
         str[i] = Newch;
      }
  }
  str[i]='\0';
}

ULONG
NonRootSupport_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
)
{
  UNREFERENCED_PARAMETER(hInsContext);
  #define APPARMOR_BLOCKLIST_FILE "/opt/secure/Apparmor_blocklist"
  #define APPARMOR_PROFILE_DIR "/etc/apparmor.d"
  #define SIZE_LEN 32
  char *buf = NULL;
  FILE *fp = NULL;
  size_t len = 0;
  ssize_t read = 0;
  DIR *dir=NULL;
  struct dirent *entry=NULL;
  char tmp[64]={0};
  char files_name[MAX_SIZE]={0};
  char *sptr=NULL;
  /* check the parameter name and return the corresponding value */
  if (strcmp(ParamName, "ApparmorBlocklist") == 0)
  {
      dir = opendir(APPARMOR_PROFILE_DIR);
      if( (dir == NULL) ) {
            CcspTraceError(("Failed to open the %s directory: profiles does not exist\n", APPARMOR_PROFILE_DIR));
            return FALSE;
      }
      memset(files_name,'\0',sizeof(files_name));
      while((entry = readdir(dir)) != NULL) {
             strncat(files_name,entry->d_name,strlen(entry->d_name));
      }
      closedir(dir);
      fp=fopen(APPARMOR_BLOCKLIST_FILE,"r");
      if(fp != NULL) {
         while((read = getline(&buf, &len, fp)) != -1) {
	     // CID 279876 : Buffer not null terminated (BUFFER_SIZE)
             strncpy(tmp,buf,sizeof(tmp)-1);
             tmp[sizeof(tmp)-1] = '\0';
             strtok_r(buf,":",&sptr);
             if(buf != NULL) {
                 strncat(pValue,tmp,strlen(tmp));
             }
         }
         fclose(fp);
         Replace_AllOccurrence( pValue, AnscSizeOfString(pValue), '\n', ',');
         CcspTraceWarning(("Apparmor profile configuration:%s\n", pValue));
         return 0;
      }
      else {
         CcspTraceWarning(("%s does not exist\n", APPARMOR_BLOCKLIST_FILE));
         strncpy(pValue,"Apparmorblocklist is empty",SIZE_LEN);
      }
  }
  //Blocklist RFC
  if (strcmp(ParamName, "Blocklist") == 0)
  {
      fp = fopen(BLOCKLIST_FILE,"r");
      if(fp!=NULL) {
         read = getdelim( &buf, &len, '\0', fp);
         if (read != -1) {
             AnscCopyString(pValue, buf);
             *pUlSize = AnscSizeOfString(pValue);
             Replace_AllOccurrence( pValue, *pUlSize, '\n', ',');
             CcspTraceWarning(("Blocklist processes:%s\n", pValue));
         }
	 //CID 279890 : Resource Leak
	 fclose(fp);
         return 0;
      }
      else if((fp == NULL) || (isspace(buf[0])) || (buf == NULL)) {
         CcspTraceWarning(("Blocklist file does not exist or no process to be blocklisted\n"));
         strncpy(pValue,"No blocklisted process",SIZE_LEN);
      }
  }
  return -1;
}

static BOOL ValidateInput_Arguments(char *input, FILE *tmp_fptr)
{
  #define APPARMOR_PROFILE_DIR "/etc/apparmor.d"
  #define BUF_SIZE 64
  struct dirent *entry=NULL;
  DIR *dir=NULL;
  char files_name[1024]={0};
  char *token=NULL;
  char *subtoken=NULL;
  char *sub_string=NULL;
  char *service_profile = NULL;
  char *sp=NULL;
  char *sptr=NULL;
  char tmp[BUF_SIZE]={0};
  char *arg=NULL;
  dir=opendir(APPARMOR_PROFILE_DIR);
  // CID 180947 : Resource leak (RESOURCE_LEAK)
  if( (dir == NULL) || (tmp_fptr == NULL) ) {
     if(tmp_fptr)
         fclose(tmp_fptr);
     if(dir)
	 closedir(dir);
     CcspTraceError(("Failed to open the %s directory\n", APPARMOR_PROFILE_DIR));
     return FALSE;
  }
  memset(files_name,'\0',sizeof(files_name));
  /* storing Apparmor profile (file) names into files_name which can be used to check with input arguments using strstr() */
  while((entry = readdir(dir)) != NULL) {
        strncat(files_name,entry->d_name,strlen(entry->d_name));
  }
  if (closedir(dir) != 0) {
      CcspTraceError(("Failed to close %s directory\n", APPARMOR_PROFILE_DIR));
      fclose(tmp_fptr);
      return FALSE;
  }
  /* Read the input arguments and ensure the corresponding profiles exist or not by searching in
     Apparmor profile directory (/etc/apparmor.d/). Returns false if input does not have the
     apparmor profile, Returns true if apparmor profile finds for the input */
  token=strtok_r( input,",", &sp);
  while(token != NULL) {
        arg=strchr(token,':');
        if(!arg)
        {
            CcspTraceWarning(("argument is  null in the parser:%s\n", token));
            return FALSE;
        }
        if ( (arg[0] != ':') || ((strcmp(arg+1,"disable") != 0) && (strcmp(arg+1,"complain") != 0) && (strcmp(arg+1,"enforce") != 0) ) ) {
              CcspTraceWarning(("Invalid input arguments in the parser:%s\n", token));
              return FALSE;
        }
	// CID 180948 : Buffer not null terminated (BUFFER_SIZE)
        strncpy(tmp,token,sizeof(tmp)-1);
        tmp[sizeof(tmp) - 1] = '\0';
        subtoken=strtok_r(tmp,":",&sptr);
        if (subtoken != NULL) {
           sub_string = strstr(files_name, subtoken);
           if (sub_string == NULL) {
                service_profile = strstr(subtoken, "service.sp");
           }
           if (sub_string != NULL || service_profile != NULL) {
              fprintf(tmp_fptr, "%s\n", token);
           } else {
              CcspTraceWarning(("Invalid arguments %s error found in the parser\n", subtoken));
              return FALSE;
           }
        }
  token=strtok_r(NULL,",",&sp);
  }
  return TRUE;
}

BOOL
NonRootSupport_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                        pValue
 )
{
  UNREFERENCED_PARAMETER(hInsContext);
  #define APPARMOR_BLOCKLIST_FILE "/opt/secure/Apparmor_blocklist"
  #define TMP_FILE "/opt/secure/Apparmor_blocklist_bck.txt"
  #define SIZE 128
  #define MAX_SIZE 1024
  FILE *fptr = NULL;
  FILE *tmp_fptr = NULL;
  char *boxType = NULL, *atomIp = NULL;
  if (strcmp(ParamName, "ApparmorBlocklist") == 0)
  {
     fptr = fopen(APPARMOR_BLOCKLIST_FILE,"r");
     tmp_fptr = fopen(TMP_FILE,"w+");
     if( (!pValue) || (strstr(pValue,":") == NULL) || (tmp_fptr == NULL) ) {
         CcspTraceError(("Failed to open the file or invalid argument\n"));
         if(fptr)
            fclose(fptr);
         if(tmp_fptr)
            fclose(tmp_fptr);
         return FALSE;
     }
     /* To ensure input arguments are valid or not */
     if (ValidateInput_Arguments(pValue, tmp_fptr) != TRUE) {
	 // CID 180949 : Resource leak (RESOURCE_LEAK)
	 if(fptr)
            fclose(fptr);
         return FALSE;
     }
     /* Copying tmp file contents into main file by using rename() */
     if(fptr != NULL)
        fclose(fptr);
     fclose(tmp_fptr);
     if(rename( TMP_FILE, APPARMOR_BLOCKLIST_FILE) != 0) {
        CcspTraceError(("Error in renaming  file\n"));
        return FALSE;
     }
     return TRUE;
  }

  //Blocklist RFC
  if (strcmp(ParamName, "Blocklist") == 0)
  {
      char buf[MAX_SIZE] = {'\0'};
      snprintf(buf,sizeof(buf),"%s",pValue);
      fptr = fopen(BLOCKLIST_FILE,"w+");
      boxType=getenv("BOX_TYPE");
      if(boxType != NULL)
      {
         if(strcmp(boxType, "XB3") ==0)
         {
            atomIp=getenv("ATOM_ARPING_IP");
            if(atomIp != NULL)
            {
               v_secure_system("/usr/bin/rpcclient %s \"echo '%s' > /nvram/Blocklist_XB3.txt\"", atomIp, buf);
            }
         }
      }
      if(!fptr){
      CcspTraceError(("%s failed to open %s file \n",__FUNCTION__,BLOCKLIST_FILE));
      return FALSE;
      }
      fprintf(fptr, "%s\n", buf);
      fclose(fptr);
      return TRUE;
  }
  CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
  return FALSE;
}

// Generic RFC get handlers
static char *Generic_GetParamJsonValue (void)
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    CcspTraceWarning(("g_currentParamFullName = %s\n", g_currentParamFullName));
    if (pMyObject->pRfcStore)
    {
        cJSON *valueObj = cJSON_GetObjectItem(pMyObject->pRfcStore, g_currentParamFullName);
        if (valueObj)
        {
            cJSON *value = cJSON_GetObjectItem(valueObj, "Value");
            if (value)
                return value->valuestring;
        }
        CcspTraceWarning(("Param %s not available in RFC store\n", g_currentParamFullName));
    }
    else
        CcspTraceWarning(("RFC store not present to retrieve %s\n", g_currentParamFullName));

    // If value is not present in RFC override store, find it in RFC defaults json.
    if (pMyObject->pRfcDefaults)
    {
        cJSON *value = cJSON_GetObjectItem(pMyObject->pRfcDefaults, g_currentParamFullName);
        if (value)
        {
            return value->valuestring;
        }
        CcspTraceWarning(("Param %s not available in RFC defaults\n", g_currentParamFullName));
    }
    else
        CcspTraceWarning(("RFC defaults not present to retrieve %s\n", g_currentParamFullName));

    return NULL;
}

ULONG
Generic_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    if ( !pValue )
    {
        CcspTraceWarning(("pValue is NULL in Generic_GetParamStringValue\n"));
        return -1;
    }

    UNREFERENCED_PARAMETER(hInsContext);
    errno_t   rc  = -1;

    char *strValue = Generic_GetParamJsonValue();
    if (strValue)
    {
        rc = strcpy_s( pValue, *pUlSize, strValue );
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        CcspTraceWarning(("param = %s, value = %s\n", ParamName, pValue));
        return 0;
    }

    return -1;
}

BOOL
Generic_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    char *value = Generic_GetParamJsonValue();
    if( value != NULL )
    {
        if (strcmp(value, "true") == 0)
            *pBool = TRUE;
        else
            *pBool = FALSE;

        return TRUE;
    }
    return FALSE;
}

BOOL
Generic_GetParamUlongValue
    (
        ANSC_HANDLE             hInsContext,
        char*                   ParamName,
        ULONG*                  pValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    char *value = Generic_GetParamJsonValue();
    if( value != NULL )
    {
        *pValue = atoi(value);
        return TRUE;
    }
    return FALSE;
}

BOOL
Generic_GetParamIntValue
        (
                ANSC_HANDLE                             hInsContext,
                char*                                           ParamName,
                int*                                            pInt
        )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);

    char *value = Generic_GetParamJsonValue();
    if( value != NULL )
    {
        *pInt = atoi(value);
        return TRUE;
    }
    return FALSE;
}

// Generic RFC set handlers

BOOL IsBoolSame(ANSC_HANDLE hInsContext,char* ParamName, BOOL bValue, GETBOOL_FUNC_PTR getBoolFunc)
{
    Generic_SetParamBoolValue(hInsContext, ParamName, bValue);
    BOOL prevValue = false;
    getBoolFunc( hInsContext, ParamName, &prevValue );
    if (prevValue == bValue)
    {
        CcspTraceWarning(("%s values are same...\n", __FUNCTION__));
        return TRUE;
    }
    CcspTraceWarning(("%s values are different...\n", __FUNCTION__));
    return FALSE;
}

BOOL IsStringSame(ANSC_HANDLE hInsContext,char* ParamName, char* pValue, GETSTRING_FUNC_PTR getStringFunc)
{

    /*CID-328971  Null pointer Dereference Fix*/
    if(pValue == NULL)
    {
      CcspTraceWarning(("%s pValue is NULL...\n", __FUNCTION__));
      return FALSE;
    }

    Generic_SetParamStringValue(hInsContext, ParamName, pValue);

    char prevValue[1024];
    ULONG size = 1024;
    getStringFunc( hInsContext, ParamName, (char *)&prevValue, &size );
    if ( strcmp(prevValue, pValue) == 0 )
    {
        CcspTraceWarning(("%s values are same...\n", __FUNCTION__));
        return TRUE;
    }
    CcspTraceWarning(("%s values are different...\n", __FUNCTION__));
    return FALSE;
}

BOOL IsUlongSame(ANSC_HANDLE hInsContext,char* ParamName, ULONG ulValue, GETULONG_FUNC_PTR getUlongFunc)
{
    Generic_SetParamUlongValue(hInsContext, ParamName, ulValue);
    ULONG prevValue = 0;
    getUlongFunc( hInsContext, ParamName, &prevValue );
    if (prevValue == ulValue)
    {
        CcspTraceWarning(("%s values are same...\n", __FUNCTION__));
        return TRUE;
    }
    CcspTraceWarning(("%s values are different...\n", __FUNCTION__));
    return FALSE;
}

BOOL IsIntSame(ANSC_HANDLE hInsContext,char* ParamName, int value, GETINT_FUNC_PTR getIntFunc)
{
    Generic_SetParamIntValue(hInsContext, ParamName, value);
    int prevValue = 0;
    getIntFunc( hInsContext, ParamName, &prevValue );
    if (prevValue == value)
    {
        CcspTraceWarning(("%s values are same...\n", __FUNCTION__));
        return TRUE;
    }
    CcspTraceWarning(("%s values are different...\n", __FUNCTION__));
    return FALSE;
}

BOOL
Generic_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
   UNREFERENCED_PARAMETER(hInsContext);
   AnscTraceWarning(("Generic_SetParamStringValue: param = %s, value = %s\n", ParamName, strValue));
   PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
   ProcessRfcSet(&pMyObject->pRfcStore, g_clearDB, g_currentParamFullName, strValue, requestorStr, currentTime);
   return TRUE;
}

BOOL
Generic_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
   AnscTraceWarning(("Generic_SetParamBoolValue: param = %s\n", ParamName));
   PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

   char * requestorStr = getRequestorString();
   char * currentTime = getTime();

   if ( bValue == TRUE)
      ProcessRfcSet(&pMyObject->pRfcStore, g_clearDB, g_currentParamFullName, "true", requestorStr, currentTime);
   else
      ProcessRfcSet(&pMyObject->pRfcStore, g_clearDB, g_currentParamFullName, "false", requestorStr, currentTime);
   return TRUE;
}

BOOL
Generic_SetParamUlongValue
    (
        ANSC_HANDLE             hInsContext,
        char*                   ParamName,
        ULONG                   ulValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[64]={0};
    errno_t rc  = -1;

    rc = sprintf_s(buf, sizeof(buf), "%lu", ulValue);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      return FALSE;
    }

   AnscTraceWarning(("Generic_SetParamIntValue: param = %s\n", ParamName));
   PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

   char * requestorStr = getRequestorString();
   char * currentTime = getTime();
   ProcessRfcSet(&pMyObject->pRfcStore, g_clearDB, g_currentParamFullName, buf, requestorStr, currentTime);
   return TRUE;
}

BOOL
Generic_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    )
{
    char buf[16]={0};
    errno_t rc  = -1;

    UNREFERENCED_PARAMETER(hInsContext);
    rc = sprintf_s(buf, sizeof(buf), "%d", value);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      return FALSE;
    }

    AnscTraceWarning(("Generic_SetParamIntValue: param = %s\n", ParamName));

    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();
    ProcessRfcSet(&pMyObject->pRfcStore, g_clearDB, g_currentParamFullName, buf, requestorStr, currentTime);

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
       AutoReboot_GetParamBoolValue
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
AutoReboot_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);  

    if (strcmp(ParamName, "Enable") == 0)
    {
        CcspTraceInfo(("[%s :] AutoReboot Getparam Enable value\n",__FUNCTION__));
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "AutoReboot", value, sizeof(value)) == 0 )
        {
            *pBool = (strcmp(value, "true") == 0) ? TRUE : FALSE;
            return TRUE;
        }
        else
        {
            CcspTraceError(("syscfg_get failed for AutoReboot.Enable\n"));
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
        AutoReboot_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to set Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       bValue
                The buffer with updated value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
AutoReboot_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{   
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO  pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    if (strcmp(ParamName, "Enable") == 0)
    {
        if (IsBoolSame(hInsContext, ParamName, bValue, AutoReboot_GetParamBoolValue))
        {
            CcspTraceInfo(("[%s:] AutoReboot Set current and previous values are same\n", __FUNCTION__ ));
            return TRUE;
        }
        if (syscfg_set_commit(NULL, "AutoReboot", bValue ? "true" : "false") != 0)
        {
            CcspTraceError(("syscfg_set failed for AutoReboot.Enable\n"));
        }
        CcspTraceInfo(("[%s:] AutoReboot Set param Enable value %d\n", __FUNCTION__, bValue));
        pMyObject->AutoReboot.Enable = bValue;
        CosaDmlScheduleAutoReboot( pMyObject->AutoReboot.UpTime, bValue );
        return TRUE;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        AutoReboot_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The buffer with updated value

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
AutoReboot_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */    
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    if (strcmp(ParamName, "UpTime") == 0)
    {
        CcspTraceInfo(("[%s:] AutoReboot Set uptime \n", __FUNCTION__ ));
        if((1 > iValue || iValue > 365))
        {
            CcspTraceWarning(("The value is not in the expected range. keeping the previous value \n"));
            return TRUE;
        }
        if(pMyObject->AutoReboot.Enable )
        {
            if( pMyObject->AutoReboot.UpTime != iValue)
            {
                CosaDmlScheduleAutoReboot( iValue, true );
            }
        }
        pMyObject->AutoReboot.UpTime = iValue;
    
        return TRUE;
    }
   /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        AutoReboot_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        iValue
            );

    description:

        This function is called to set Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                       iValue
                The buffer of returned int value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
AutoReboot_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */   
    const int DEFAULT_UPTIME = 120;
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    if (strcmp(ParamName, "UpTime") == 0)
    {
        *iValue = pMyObject->AutoReboot.UpTime;
        if( (1 > *iValue || *iValue > 365) )
        {
            *iValue  = DEFAULT_UPTIME;
            pMyObject->AutoReboot.UpTime=DEFAULT_UPTIME;  
        }
        return TRUE;
    }
   /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**
 *  RFC Features OCSP
*/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        EnableOCSPStapling_GetParamBoolValue
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
EnableOCSPStapling_GetParamBoolValue

(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL*                       pBool
 )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "Enable") == 0)
    {
        char buf[8];
        /* collect value */
        if(!syscfg_get( NULL, "EnableOCSPStapling", buf, sizeof(buf))) {
        /* CID: 154685 Array compared against 0*/
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        } else 
           return FALSE;

        return TRUE;                
    }

    if (strcmp(ParamName, "DirectOCSP") == 0)
    {
        char buf1[8];
        /* collect value */
        /* CID: 154685 Array compared against 0*/
        if(!syscfg_get( NULL, "EnableOCSPCA", buf1, sizeof(buf1)))
        {
            if (strcmp(buf1, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        } else 
           return FALSE;

        return TRUE;                
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        EnableOCSPStapling_SetParamBoolValue
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
EnableOCSPStapling_SetParamBoolValue

(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        if (syscfg_set_commit(NULL, "EnableOCSPStapling", bValue ? "true" : "false") != 0)
        {
            CcspTraceError(("syscfg_set EnableOCSPStapling failed\n"));
        }
        else
        {
            return TRUE;
        }
    }

    if (strcmp(ParamName, "DirectOCSP") == 0)
    {
        if (syscfg_set_commit(NULL, "EnableOCSPCA", bValue ? "true" : "false") != 0)
        {
            CcspTraceError(("syscfg_set EnableOCSPCA failed\n"));
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

static void copy_command_output (char *cmd, char *out, int len)
{
    FILE *fp;

    out[0] = 0;

    fp = popen (cmd, "r");
    if (fp)
    {
        if (fgets (out, len, fp) != NULL)
        {
          /* CID 252175 fix - Parse warning (PW.PARAMETER_HIDDEN) */
            size_t len_out = strlen (out);
            if ((len_out > 0) && (out[len_out - 1] == '\n'))
                out[len_out - 1] = 0;
        }

        pclose (fp);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

	BOOL
	SelfHeal_GetParamUlongValue
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
SelfHeal_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char buf[8] = {0};

    if (strcmp(ParamName, "AggressiveInterval") == 0)
    {
        syscfg_get( NULL, ParamName, buf, sizeof(buf));
        if( 0 == strlen(buf) )
            return FALSE;
        *puLong = atol(buf);
        return TRUE;
    }
    AnscTraceWarning(("%s is invalid argument!\n", ParamName));
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

	BOOL
	SelfHeal_SetParamUlongValue
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
SelfHeal_SetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "AggressiveInterval") == 0)
    {
        char buf[16];

        if (uValue < 2) /* Minimum interval is 2 as per the aggressive selfheal US [RDKB-25546] */
	{
	    AnscTraceWarning(("Minimum interval is 2 for %s !\n", ParamName));
	    return FALSE;
	}
#if defined(_ARRIS_XB6_PRODUCT_REQ_) || defined(_CBR_PRODUCT_REQ_) || defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_TURRIS_) || defined(_PLATFORM_BANANAPI_R4_) || \
(defined(_XB6_PRODUCT_REQ_) && defined(_COSA_BCM_ARM_))
	syscfg_get( NULL, "resource_monitor_interval", buf, sizeof(buf));
        if( 0 == strlen(buf) )
	{
	    AnscTraceWarning(("syscfg_get returns NULL for resource_monitor_interval !\n"));
	    return FALSE;
	}
	ULONG resource_monitor_interval = atol(buf);
	if (uValue >= resource_monitor_interval)
	{
	    CcspTraceWarning(("AggressiveInterval should be lesser than resource_monitor_interval\n"));
	    return FALSE;
	}
#endif
        if (syscfg_set_u_commit(NULL, ParamName, uValue) != 0)
        {
            AnscTraceWarning(("%s syscfg_set failed!\n", ParamName));
            return FALSE;
        }

        copy_command_output("pidof selfheal_aggressive.sh", buf, sizeof(buf));
        if (buf[0] != 0) {
          v_secure_system("kill -9 %s", buf);
        }
        v_secure_system("/usr/ccsp/tad/selfheal_aggressive.sh &");
    }
    else
    {
        AnscTraceWarning(("%s is invalid argument!\n", ParamName));
        return FALSE;
    }
    return TRUE;
}

#if defined(FEATURE_RDKB_NFC_MANAGER)
//NFC RFC :: NFC Feature should enable.

/***********************************************************************

   caller: owner of this object

   prototype:

       BOOL
       NFC_GetParamBoolValue
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
NFC_GetParamBoolValue
(
ANSC_HANDLE                 hInsContext,
char*                       ParamName,
BOOL*                       pBool
)
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        char nfc_status[8] = {0};
        if(!syscfg_get(NULL,"nfc_enabled",nfc_status, sizeof(nfc_status)))
        {
            CcspTraceInfo(("nfc_enabled = %s \n", nfc_status));
            if (strcmp(nfc_status, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }else{
            CcspTraceError(("Failed to get `nfc_enabled` syscfg status \n"));
            return FALSE;
        }
    }
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        NFC_SetParamBoolValue
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
NFC_SetParamBoolValue
(
ANSC_HANDLE                 hInsContext,
char*                       ParamName,
BOOL                        bValue
)
{
    if (IsBoolSame(hInsContext, ParamName, bValue, NFC_GetParamBoolValue))
    {
        return TRUE;
    }
    if (strcmp(ParamName, "Enable") == 0)
    {

       if ( bValue == TRUE)
       {
          syscfg_set_commit(NULL, "nfc_enabled", "true");
          //Start RdkNfcManager systemd service to start nfc services.
          v_secure_system("systemctl start RdkNfcManager.service");
       }
       else
       {
          syscfg_set_commit(NULL, "nfc_enabled", "false");
          //Stop RdkNfcManager systemd service to stop nfc services.
          v_secure_system("systemctl stop RdkNfcManager.service");
       }

       return TRUE;
    }
    return FALSE;
}
#endif //FEATURE_RDKB_NFC_MANAGER

/***********************************************************************

 APIs for Object:

	DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.LatencyMeasureTcpSetupIPv4.

    *  LatencyMeasureTcpSetupIPv4_GetParamBoolValue
	*  LatencyMeasureTcpSetupIPv4_SetParamBoolValue
***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        LatencyMeasureTcpSetupIPv4_GetParamBoolValue
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
LatencyMeasureTcpSetupIPv4_GetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL*						pBool
	)
{
    UNREFERENCED_PARAMETER(hInsContext);

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        char buf[8];
        if( syscfg_get( NULL, "LatencyMeasure_IPv4Enable", buf, sizeof(buf))==0)
        {
            if (strcmp(buf, "true") == 0)
            {
                *pBool = TRUE;
            }
            else
            {
                *pBool = FALSE;
            }
        }
        else
        {
            CcspTraceWarning(("%s syscfg_get failed  for TcpIpv4Enable\n",__FUNCTION__));
            *pBool = FALSE;
		}
        return TRUE;
	}
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        LatencyMeasureTcpSetupIPv4_SetParamBoolValue
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
LatencyMeasureTcpSetupIPv4_SetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL						bValue
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
    	parameterValStruct_t pVal[1];
    	char*                faultParam     = NULL;
	int                  ret            = 0;
	CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	pVal[0].parameterName  = PARAM_TCP_IPV4_ENABLE;
	pVal[0].parameterValue = bValue ? "true" : "false";
	pVal[0].type           = ccsp_boolean;

	ret = CcspBaseIf_setParameterValues(
		 bus_handle,
		 COMP_TCP_IPV4_ENABLE,
		 DBUSPATH_TCP_IPV4_ENABLE,
		 0,
		 0,
		 pVal,
		 1,
		 TRUE,
		 &faultParam
		);

	if (ret != CCSP_SUCCESS)
		{
			CcspTraceError(("%s - %d - Failed to set value to PAM TcpIpv4- Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
			bus_info->freefunc(faultParam);
            faultParam=NULL;
			return FALSE;
		}
        return TRUE;
    }

    return FALSE;

}

/***********************************************************************

 APIs for Object:

	DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.LatencyMeasureTcpSetupIPv6.

    *  LatencyMeasureTcpSetupIPv6_GetParamBoolValue
	*  LatencyMeasureTcpSetupIPv6_SetParamBoolValue
***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        LatencyMeasureTcpSetupIPv6_GetParamBoolValue
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
LatencyMeasureTcpSetupIPv6_GetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL*						pBool
	)
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        char buf[8];
        if( syscfg_get( NULL, "LatencyMeasure_IPv6Enable", buf, sizeof(buf))==0)
        {
            if (strcmp(buf, "true") == 0)
            {
                *pBool = TRUE;
            }
            else
            {
                *pBool = FALSE;
            }
        }
        else
        {
            CcspTraceWarning(("%s syscfg_get failed  for TcpIpv6Enable\n",__FUNCTION__));
            *pBool = FALSE;
        }
        return TRUE;
	}

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        LatencyMeasureTcpSetupIPv6_SetParamBoolValue
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
LatencyMeasureTcpSetupIPv6_SetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL						bValue
	)

{
	UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        parameterValStruct_t pVal[1];
        char*                faultParam     = NULL;
        int                  ret            = 0;
        CCSP_MESSAGE_BUS_INFO *bus_info               = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
        pVal[0].parameterName  = PARAM_TCP_IPV6_ENABLE;
        pVal[0].parameterValue = bValue ? "true" : "false";
        pVal[0].type           = ccsp_boolean;

        ret = CcspBaseIf_setParameterValues(
                 bus_handle,
                 COMP_TCP_IPV6_ENABLE,
                 DBUSPATH_TCP_IPV6_ENABLE,
                 0,
                 0,
                 pVal,
                 1,
                 TRUE,
                 &faultParam
                );

        if (ret != CCSP_SUCCESS)
                {
                        CcspTraceError(("%s - %d - Failed to set value to PAM TcpIpv4- Error [%s]\n", __FUNCTION__, __LINE__, faultParam));
                        bus_info->freefunc(faultParam);
                        faultParam = NULL;
                        return FALSE;
                }
        return TRUE;
    }

    return FALSE;

}

