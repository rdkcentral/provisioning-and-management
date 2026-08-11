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

    module: cosa_time_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlTimeInit
        *  CosaDmlTimeSetCfg
        *  CosaDmlTimeGetCfg
        *  CosaDmlTimeGetState
        *  CosaDmlTimeGetLocalTime
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

#include "cosa_time_apis.h"
#include <cjson/cJSON.h>
#include "secure_wrapper.h"
#include "safec_lib_common.h"
#include <errno.h>
#include <unistd.h>

#define PARTNERS_INFO_FILE              "/nvram/partners_defaults.json"
#define BOOTSTRAP_INFO_FILE             "/opt/secure/bootstrap.json"
#define MAX_COSATIMEOFFSET_SIZE   256

ANSC_STATUS
CosaNTPInitJournal
    (
        PCOSA_DML_TIME_CFG pTimeCfg
    );

#ifdef _COSA_SIM_

COSA_DML_TIME_CFG                   g_TimeCfg = {0};

ANSC_STATUS
CosaDmlTimeInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (pTimeCfg)
        AnscCopyMemory(&g_TimeCfg, pTimeCfg, sizeof(COSA_DML_TIME_CFG));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (pTimeCfg)
        AnscCopyMemory(pTimeCfg, &g_TimeCfg, sizeof(COSA_DML_TIME_CFG));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetState
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_STATUS       pStatus,
        PANSC_UNIVERSAL_TIME        pCurrLocalTime
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (pStatus && pCurrLocalTime)
    {
        *pStatus = COSA_DML_TIME_STATUS_Synchronized;
        _ansc_memset(pCurrLocalTime, 0, sizeof(ANSC_UNIVERSAL_TIME));
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetLocalTime
    (
       ANSC_HANDLE                 hContext,
       char                       *pCurrLocalTime
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCurrLocalTime);
    return ANSC_STATUS_SUCCESS;
}

#elif defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)

#include <utctx/utctx.h>
#include <utctx/utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <syscfg/syscfg.h>
#include "cosa_drg_common.h"
#include "platform_hal.h"
#define UTOPIA_TR181_PARAM_SIZE1   256

#define MAXBUF              512

/**
 * ntpclient config file and it's parameters
 * XXX: ntpclient can only support one NTP server.
 */
#define NTPC_CONF           "/mnt/jffs2/ntpclient.conf"
#define     NTPPAR_ENABLE       "Enable"
#define     NTPPAR_NTPSERVER1   "NTPServer"
#define     NTPPAR_NTPSERVER2   "NTPServer2"
#define     NTPPAR_TIMEZONE     "TimeZone"

#define DEF_NTPSERVER1      "ntp1.sbcglobal.net"
#define DEF_NTPSERVER2      "ntp2.sbcglobal.net"
#define DEF_TIMEZONE        "PacificTime"

#define NTPC_STATUS         "/tmp/ntpclient.log"
#define NTPC_STARSTATUS     "/tmp/ntpstatus"

#ifndef NELEMS
#define NELEMS(arr)             (sizeof(arr) / sizeof((arr)[0]))
#endif

ANSC_STATUS updateTimeZone(const char *timezone)
{
            char regionTime[120];
            struct tm tm_local = {0};
            if (strptime(regionTime, "%a %b %d %H:%M:%S %Y", &tm_local) != NULL)
            {
                tm_local.tm_isdst = -1;
		printff("tm_local.tm_isdst= %d\n",(int) tm_local.tm_isdst);
            }

    if(timezone == NULL)
        return ANSC_STATUS_FAILURE;

    FILE *fp = NULL;
    char zone[MAXBUF] = {0};

    if ((fp = fopen("/etc/TZ", "w")) == NULL)
    {
        AnscTraceWarning(("%s: cannot open file /etc/TZ\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    snprintf(zone, sizeof(zone), "%s\n", timezone);
    if (fwrite(zone, strlen(zone), 1, fp) != 1)
    {
        AnscTraceWarning(("%s: fail to write\n", __FUNCTION__));
        fclose(fp);
        return ANSC_STATUS_FAILURE;
    }
    fclose(fp);
    return ANSC_STATUS_SUCCESS;
}

#if defined (_COSA_BCM_MIPS_)
/* The XF3 and CFG3 use the thin client version of ntpd called timesyncd. This version uses a
 * config file at /etc/systemd/timesyncd.conf. If the enable option is set, we'll update the value
 * in the conf file and restart the timesyncd service to pick up the changes.
 */
/* The /etc/systemd/timesyncd.conf file is mounted copy bind to /tmp/systemd-timesyncd.conf so
 * we will need to edit the file there.
 */
const char timesyncConfFile[] = "/tmp/systemd-timesyncd.conf";
const char updateTimesyncConf[] = "/usr/ccsp/updateTimesyncdConf.sh";

/*
 * Timesyncd uses "timedatectl status" to fetch whether or not we are synced.
 *
 * From the output of the above command, we are looking for one of these lines:
 * "NTP synchronized: yes"
 * "NTP synchronized: no"
 */
BOOL isTimeSynced()
{
     char buf[MAXBUF] = {0};
     BOOL isSync = FALSE;

     AnscTraceWarning(("%s: isTimeSynced.\n", __FUNCTION__));

     FILE *fp = popen("timedatectl status", "r");

     if ( fp != NULL){
         while (fgets(buf, sizeof(buf), fp) != NULL)
         {
             if (strstr(buf, "NTP synchronized: yes") != NULL) {
                 isSync = TRUE;
                 break;
             } else if (strstr(buf, "NTP synchronized: no") != NULL) {
                 break;
             }
         }

         pclose(fp);
     }

     return isSync;
}


void* startNTP(void* arg)
{
    char buf[MAXBUF]= {0};
    char server[MAXBUF] = {0};
    int  i = 0;
    PCOSA_DML_TIME_CFG pTimeCfg = (PCOSA_DML_TIME_CFG)arg;

    AnscTraceWarning(("%s: Start Function...\n", __FUNCTION__));
    if (pTimeCfg->bEnabled)
        snprintf(buf, sizeof(buf), "1");
    else
        snprintf(buf, sizeof(buf), "0");

    /*update time zone*/
    if(ANSC_STATUS_SUCCESS != updateTimeZone(pTimeCfg->LocalTimeZone))
    {
        AnscTraceWarning(("%s: Fail to update time zone!\n", __FUNCTION__));
        return NULL;
    }

    /* Timesynd supports multiple NTP Servers, so we'll build up a list */
    if (pTimeCfg->bEnabled)
    {
        for (i=0;i<=5;i++) {
            switch (i) {
            case 1:
                /* CID: 54208 Array compared against 0*/
                if (strlen(pTimeCfg->NTPServer1.ActiveValue) > 0)
                {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer1.ActiveValue);
                }
                break;
            case 2:
                /* CID: 54208 Array compared against 0*/
                if (strlen(pTimeCfg->NTPServer2.ActiveValue) > 0)
                {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer2.ActiveValue);
                }
                break;
            case 3:
                /* CID: 54208 Array compared against 0*/
                if (strlen(pTimeCfg->NTPServer3.ActiveValue) > 0)
                {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer3.ActiveValue);
                }
                break;
            case 4:
                /* CID: 54208 Array compared against 0*/
                if (strlen(pTimeCfg->NTPServer4.ActiveValue) > 0)
                {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer4.ActiveValue);
                }
                break;
            case 5:
                /* CID: 54208 Array compared against 0*/
                if (strlen(pTimeCfg->NTPServer5.ActiveValue) > 0) {
                    if (server[0] != '\0') {
                        strcat(server, " "); // add spacer
                    }
                    strcat(server, pTimeCfg->NTPServer5.ActiveValue);
                }
                break;
            default:
                break;
            }
        }

        if (strlen(server) != 0)
        {
            /**
             * Kill the old timesyncd process and unmount the timesyncd.conf file
             */
            AnscTraceWarning(("%s: stopping ntpclient \n", __FUNCTION__));
            v_secure_system("systemctl stop tmp-systemd-timesyncd.conf.service");
            v_secure_system("systemctl stop systemd-timesyncd");

            // Copy the new server(s) into the temp config file
            v_secure_system("sed -i '/^[#\\s]*NTP=/s/.*/NTP=%s/' %s", server, timesyncConfFile);

            // remount the modified timesyncd.conf file
            v_secure_system("systemctl start tmp-systemd-timesyncd.conf.service");

            // Restart timesyncd
            AnscTraceWarning(("%s: starting ntpclient with host %s,command:%s\n", __FUNCTION__, server, buf));
            sleep(2);
            if (v_secure_system("systemctl start systemd-timesyncd") != 0)
            {
                AnscTraceWarning(("%s: fail to execute ntpclient\n", __FUNCTION__));
            }
        }
    } else {
        if (access(updateTimesyncConf, F_OK) == 0)
        {
            AnscTraceWarning(("%s: Set NTP Server via default method\n", __FUNCTION__));
            // Update the timesyncd.conf file using the default method
            v_secure_system("/usr/ccsp/updateTimesyncdConf.sh");
        }
    }
    return NULL;
}

#else
/* TURE means synced, FALSE means fail or un-synced */
BOOL isTimeSynced()
{
     FILE *fp = NULL;
     char buf[128] = {0};
     BOOL isSync = FALSE;

     AnscTraceWarning(("%s: isTimeSynced.\n", __FUNCTION__));
     
     if (access(NTPC_STARSTATUS, F_OK) == 0)
     {
          AnscTraceWarning(("%s: open NTPC_STARSTATUS,.\n", __FUNCTION__));
	  fp = fopen(NTPC_STARSTATUS , "r");
	  if(fp == NULL) {
	       return FALSE;
	  }
      /* CID 63062 fix */
	  size_t bytes_read = fread(buf, 1, sizeof(buf), fp);
      if (bytes_read > 0)
      {
        buf[sizeof(buf)-1] = '\0';
        if (strncmp(buf, "Synchronized", 12) == 0)
            isSync = TRUE;
        else
            isSync = FALSE;
      }
	  fclose(fp);
     }
     else
     {
         AnscTraceWarning(("%s: open NTPC_STATUS,,.\n", __FUNCTION__));
	 fp = fopen(NTPC_STATUS, "r");
	 if(fp == NULL) {
	      isSync = FALSE;
	      return isSync;
	 }

	 size_t bytes_read = fread(buf, 1, sizeof(buf), fp);
     if (bytes_read > 0)
     {
        /* CID: 135459 String not null terminated*/
        buf[sizeof(buf)-1] = '\0';
        if (strstr(buf, "Synchronized")!= NULL) 
            isSync = TRUE;
        else
            isSync = FALSE;
     }
	 fclose(fp);
     }

     return isSync;
}

void* startNTP(void* arg)
{
    char buf[MAXBUF]= {0};
    char *server = NULL;
    char *back_server = NULL;
    int  i = 0;
    char wan_interface[32] = {0};
    PCOSA_DML_TIME_CFG pTimeCfg = (PCOSA_DML_TIME_CFG)arg;

    AnscTraceWarning(("%s: Start Function...\n", __FUNCTION__));
    if (pTimeCfg->bEnabled)
        snprintf(buf, sizeof(buf), "1");
    else
        snprintf(buf, sizeof(buf), "0");

    /*update time zone*/
    if(ANSC_STATUS_SUCCESS != updateTimeZone(pTimeCfg->LocalTimeZone))
    {
        AnscTraceWarning(("%s: Fail to update time zone!\n", __FUNCTION__));
        return NULL;
    }

    /**
     * to kill the old process whenever NTP is enabled or not, 
     * since NTPServer may changed.
     */
    v_secure_system("killall ntpclient >/dev/null 2>&1");
    v_secure_system("rm -rf /tmp/ntpstatus");
    v_secure_system("rm -rf /tmp/ntpclient.log");
    AnscTraceWarning(("%s: stopping ntpclient \n", __FUNCTION__));

    /*get current eRT interface*/
    commonSyseventGet("current_wan_ifname", wan_interface, sizeof(wan_interface));
    if('\0' == wan_interface[0])
    {
        /*default wan interface*/
        commonSyseventGet("wan_ifname", wan_interface, sizeof(wan_interface));
    }

    /* XXX: ntpclient only support one NTP Server */
    if (pTimeCfg->bEnabled)
    {
        /* CID: 54208 Array compared against 0*/
        if ( strlen(pTimeCfg->NTPServer1.ActiveValue) > 0)
        {
            server = pTimeCfg->NTPServer1.ActiveValue;
            //back_server = pTimeCfg->NTPServer2;
        }
        /* CID: 54208 Array compared against 0*/
        else if (strlen(pTimeCfg->NTPServer2.ActiveValue) > 0) {
            server = pTimeCfg->NTPServer2.ActiveValue;
        }
        /* CID: 54208 Array compared against 0*/
        else if (strlen(pTimeCfg->NTPServer3.ActiveValue) > 0) {
            server = pTimeCfg->NTPServer3.ActiveValue;
        }
        /* CID: 54208 Array compared against 0*/
        else if (strlen(pTimeCfg->NTPServer4.ActiveValue) > 0) {
            server = pTimeCfg->NTPServer4.ActiveValue;
        }
        /* CID: 54208 Array compared against 0*/
        else if (strlen(pTimeCfg->NTPServer5.ActiveValue) > 0) {
            server = pTimeCfg->NTPServer5.ActiveValue;
        }
        else
            server = NULL;

        if (server)
        {
            /* XXX: sleep a while to prevent be killed by "killall" above.
             * if we can found PID for kill instead of using killall 
             * (e.g., ps | awk '/ntpclient/ {print $1}' | xargs kill )
             * then we needn't sleep here. */

            AnscTraceWarning(("%s: starting ntpclient with host %s,command:%s\n", __FUNCTION__, server, buf));
            sleep(1);
            if (v_secure_system("ntpclient -i 2 -s -h %s -I %s 2>%s", server, wan_interface, NTPC_STATUS) != 0)
            {
                AnscTraceWarning(("%s: fail to execute ntpclient\n", __FUNCTION__));
            }

            for(i=1;i<=5;i++) {
                /* CID: 54208 Array compared against 0*/
                if (i == 1 && strlen(pTimeCfg->NTPServer1.ActiveValue) > 0) { 
                    back_server = pTimeCfg->NTPServer1.ActiveValue;
                } else if(i == 2 && strlen(pTimeCfg->NTPServer2.ActiveValue) > 0){
                    back_server = pTimeCfg->NTPServer2.ActiveValue;
                } else if(i == 3 && strlen(pTimeCfg->NTPServer3.ActiveValue) > 0){
                    back_server = pTimeCfg->NTPServer3.ActiveValue;
                } else if(i == 4 && strlen(pTimeCfg->NTPServer4.ActiveValue) > 0){
                    back_server = pTimeCfg->NTPServer4.ActiveValue;
                } else if(i == 5 && strlen(pTimeCfg->NTPServer5.ActiveValue) > 0){
                    back_server = pTimeCfg->NTPServer5.ActiveValue;
                } 
                /* try the back up ntp server */
                if (back_server && strcmp(back_server,server)!=0)
                {
                    AnscTraceWarning(("%s: trying backup server:%s\n", __FUNCTION__,back_server));
                    sleep(2); /* Wait ntpclient finished */

                    if (!isTimeSynced())
                    {
                        AnscTraceWarning(("%s: backup server not synced.\n", __FUNCTION__));
                        /**
                         * to kill the old process whenever NTP is enabled or not, 
                         * since NTPServer may changed.
                         */
                        v_secure_system("killall ntpclient >/dev/null 2>&1");
                        v_secure_system("rm -rf /tmp/ntpstatus");
                        v_secure_system("rm -rf /tmp/ntpclient.log");
                        AnscTraceWarning(("%s: stopping ntpclient 2\n", __FUNCTION__));

                        /* XXX: sleep a while to prevent be killed by "killall" above.
                         * if we can found PID for kill instead of using killall 
                         * (e.g., ps | awk '/ntpclient/ {print $1}' | xargs kill )
                         * then we needn't sleep here. */

                        AnscTraceWarning(("%s: starting ntpclient with host %s\n", __FUNCTION__, back_server));
                        sleep(1);
                        if (v_secure_system("ntpclient -i 2 -s -h %s -I %s 2>%s", back_server, wan_interface, NTPC_STATUS) != 0)
                        {
                            AnscTraceWarning(("%s: fail to execute ntpclient\n", __FUNCTION__));
                        }
                    }
                }
            }
        }
    }
    return NULL;
}
#endif

ANSC_STATUS
CosaDmlTimeInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    PCOSA_DML_TIME_CFG          pTimeCfg=(PCOSA_DML_TIME_CFG)phContext;

    CosaDmlTimeGetCfg(NULL, pTimeCfg);

/*
    if(ANSC_STATUS_SUCCESS != updateTimeZone(pTimeCfg->LocalTimeZone))
    {
        AnscTraceWarning(("%s: Fail to update time zone!\n", __FUNCTION__));
    }
*/
    return ANSC_STATUS_SUCCESS;
}



ANSC_STATUS
CosaDmlTimeSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    )
{
    UtopiaContext ctx;
    char buf[32] = {0};
    int rc = 0;
    errno_t  safec_rc = -1;
    UNREFERENCED_PARAMETER(hContext);
    if (!pTimeCfg)
        return ANSC_STATUS_FAILURE;

//    startNTP(pTimeCfg);

    if (pTimeCfg)
    {
       /* Initialize a Utopia Context */
       if(!Utopia_Init(&ctx))
          return ERR_UTCTX_INIT;
       /* Set Local TZ to SysCfg */
       rc = Utopia_Set_DeviceTime_LocalTZ(&ctx, (char*)&(pTimeCfg->LocalTimeZone));

       /*set city index */
       safec_rc = sprintf_s(buf, sizeof(buf), "%lu",pTimeCfg->cityIndex);
       if(safec_rc < EOK)
       {
          ERR_CHK(safec_rc);
       }
       rc = Utopia_RawSet(&ctx, NULL, "ntp_cityindex", buf);

       /*Set NTP Server 1 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer1.ActiveValue), 1);

       /*Set NTP Server 2 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer2.ActiveValue), 2);

       /*Set NTP Server 3 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer3.ActiveValue), 3);

        /*Set NTP Server 4 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer4.ActiveValue), 4);

       /*Set NTP Server 5 to SysCfg */
       rc = Utopia_Set_DeviceTime_NTPServer(&ctx, (char*)&(pTimeCfg->NTPServer5.ActiveValue), 5);
       
       /*Set NTP DaylightSaving Enabled or not to SysCfg */
       rc = Utopia_Set_DeviceTime_DaylightEnable(&ctx, pTimeCfg->bDaylightSaving);

       /*Set NTP DaylightSaving Offset to SysCfg */
       rc = Utopia_Set_DeviceTime_DaylightOffset(&ctx,pTimeCfg->DaylightSavingOffset);

       /*Set NTP Enabled or not to SysCfg */
       rc = Utopia_Set_DeviceTime_Enable(&ctx,pTimeCfg->bEnabled);

       /* Free Utopia Context */
       Utopia_Free(&ctx,!rc);
     }

#ifdef NTPD_ENABLE
       /* Restart whichever NTP client is currently active so it reloads the new
        * server list: chronyd when the RFC flag is set, otherwise ntpd. */
           char chronyEnabled[8] = {0};
           syscfg_get(NULL, "chrony_enabled", chronyEnabled, sizeof(chronyEnabled));
           if (strcmp(chronyEnabled, "true") == 0)
           {
               CcspTraceWarning(("%s: chrony active - triggering event to restart chronyd \n", __FUNCTION__));
               commonSyseventSet("chronyd-restart", "");
           }
           else
           {
               CcspTraceWarning(("%s: Triggering event to restart ntpd \n", __FUNCTION__));
               commonSyseventSet("ntpd-restart", "");
           }
#else
    pthread_t ntp_thread;
    int err = 0;
    err = pthread_create(&ntp_thread, NULL, startNTP, (void *)pTimeCfg);

    if(0 != err)
    {
        CcspTraceError(("%s: create the ntp syn thread error!\n", __FUNCTION__));
    }
    else
        pthread_detach(ntp_thread);

#endif

     if (rc != 0)
       return ERR_SYSCFG_FAILED;
     else
       return ANSC_STATUS_SUCCESS;
}

int checkIfUTCEnabled(const char *fname)
{
#if 0
    FILE *file;
    if (file = fopen(fname, "r"))
    {
        fclose(file);
        return 0;
    }
    return 1;
#endif


       FILE *fp;
       char temp[32]={0};
       char *str="UTC_ENABLE=true";
       if((fp = fopen(fname, "r")) == NULL) {
		
                return 1;
       }

        while(fgets(temp, 32, fp) != NULL) {
                if((strstr(temp, str)) != NULL) {
			fclose(fp);
			return 0;
                }
        }

        if(fp) {
                fclose(fp);
        }

        return 1;

}

ANSC_STATUS
CosaDmlTimeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    )
{
    UtopiaContext ctx;
    int rc = 0;
    int utc_enabled=0;
    char val[UTOPIA_TR181_PARAM_SIZE1];
    UNREFERENCED_PARAMETER(hContext);
    errno_t safec_rc = -1;

    /*CID: 56962 Dereference after null check*/
    if (!pTimeCfg)
    {
        CcspTraceWarning(("%s-%d : NULL param\n" , __FUNCTION__, __LINE__ ));
        return ANSC_STATUS_FAILURE;
    }

       /* Initialize a Utopia Context */
       if(!Utopia_Init(&ctx))
           return ERR_UTCTX_INIT;
       _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);

       /* Fill Local TZ from SysCfg */
       if( (Utopia_Get_DeviceTime_LocalTZ(&ctx,val)) == UT_SUCCESS)
       {
          safec_rc = strcpy_s(pTimeCfg->LocalTimeZone,sizeof(pTimeCfg->LocalTimeZone),val);
          ERR_CHK(safec_rc);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          /* CID 340025 Unused value : fix */
          // rc = 0;
       }

       rc = Utopia_RawGet(&ctx, NULL, "ntp_cityindex", val, sizeof(val)-1 );
       pTimeCfg->cityIndex = atoi(val);

       /*Fill NTP Server 1 from SysCfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,1)) == UT_SUCCESS)
       {
          safec_rc = strcpy_s(pTimeCfg->NTPServer1.ActiveValue,sizeof(pTimeCfg->NTPServer1.ActiveValue),val);
          ERR_CHK(safec_rc);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 2 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,2)) == UT_SUCCESS)
       {
          safec_rc = strcpy_s(pTimeCfg->NTPServer2.ActiveValue,sizeof(pTimeCfg->NTPServer2.ActiveValue),val);
          ERR_CHK(safec_rc);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 3 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,3)) == UT_SUCCESS)
       {
          safec_rc = strcpy_s(pTimeCfg->NTPServer3.ActiveValue,sizeof(pTimeCfg->NTPServer3.ActiveValue),val);
          ERR_CHK(safec_rc);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 4 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,4)) == UT_SUCCESS)
       {
          safec_rc = strcpy_s(pTimeCfg->NTPServer4.ActiveValue,sizeof(pTimeCfg->NTPServer4.ActiveValue),val);
          ERR_CHK(safec_rc);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill NTP Server 5 from Syscfg */
       if( (Utopia_Get_DeviceTime_NTPServer(&ctx,val,5)) == UT_SUCCESS)
       {
          safec_rc = strcpy_s(pTimeCfg->NTPServer5.ActiveValue,sizeof(pTimeCfg->NTPServer5.ActiveValue),val);
          ERR_CHK(safec_rc);
          _ansc_memset(val,0,UTOPIA_TR181_PARAM_SIZE1);
          rc = 0;
       }

       /* Fill DaylightSaving Enabled or not from syscfg */
       pTimeCfg->bDaylightSaving = Utopia_Get_DeviceTime_DaylightEnable(&ctx);

       /* Fill DaylightSaving Offset from syscfg */
       rc = Utopia_Get_DeviceTime_DaylightOffset(&ctx, (int*)&(pTimeCfg->DaylightSavingOffset));

       /* Fill NTP Enabled or not from syscfg */
       pTimeCfg->bEnabled = Utopia_Get_DeviceTime_Enable(&ctx);

       /* Free Utopia Context */
       Utopia_Free(&ctx,0);

       CosaNTPInitJournal(pTimeCfg);

     
    utc_enabled = checkIfUTCEnabled(DEV_PROPERTIES_FILE);
     if (0 == utc_enabled)
     {
       /*   CcspTraceWarning(("%s: UTC Enable file exists\n", __FUNCTION__));
            printf("%s: UTC Enable file exists\n", __FUNCTION__);  */
        pTimeCfg->bUTCEnabled = TRUE;
      }
    else
      {
        /*  CcspTraceWarning(("%s: UTC Enable file not exists\n", __FUNCTION__));
            printf("%s: UTC Enable file not exists\n", __FUNCTION__); */
	 pTimeCfg->bUTCEnabled = FALSE;
       }

     if (rc != 0)
       return ERR_SYSCFG_FAILED;    
     else
       return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetState
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_STATUS       pStatus,
        PANSC_UNIVERSAL_TIME        pCurrLocalTime
    )
{
    UtopiaContext ctx;
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCurrLocalTime);
    /* Initialize a Utopia Context */
    if(!Utopia_Init(&ctx))
        return ERR_UTCTX_INIT;

    *pStatus = Utopia_Get_DeviceTime_Status(&ctx);

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetLocalTime
    (
       ANSC_HANDLE                 hContext,
       char                       *pCurrLocalTime
    )
{
    UNREFERENCED_PARAMETER(hContext);
    time_t t;
#if defined(UTC_ENABLE) && !defined(_XF3_PRODUCT_REQ_)
struct tm *pLcltime, temp;
   time(&t);
   t = t + getOffset();
   gmtime_r(&t, &temp); // already adjusted for TZ with offset
   pLcltime = &temp;
#else
    struct tm *pLcltime;
#ifdef _XF3_PRODUCT_REQ_
    char timeOffset[MAX_COSATIMEOFFSET_SIZE];
    int offset;
    t = time(NULL);
    CosaDmlTimeGetTimeOffset((ANSC_HANDLE)NULL, timeOffset);
    offset = atoi(timeOffset);
    t += (time_t)offset;
#else
    t = time(NULL);
#endif
    pLcltime = localtime(&t);
#endif
    _ansc_sprintf(pCurrLocalTime, "%.4u-%.2u-%.2u %.2u:%.2u:%.2u",
            (pLcltime->tm_year)+1900,
            (pLcltime->tm_mon)+1,
            pLcltime->tm_mday,
            pLcltime->tm_hour,
            pLcltime->tm_min,
            pLcltime->tm_sec);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeGetUTCTime
    (
       ANSC_HANDLE                 hContext,
       char                       *pCurrUTCTime
    )
{
    UNREFERENCED_PARAMETER(hContext);
    time_t t;
    struct tm *pUTCtime;
    t = time(NULL);
    pUTCtime = gmtime(&t);

    if(NULL != pUTCtime){
       _ansc_sprintf(pCurrUTCTime, "%.4u-%.2u-%.2u %.2u:%.2u:%.2u",
            (pUTCtime->tm_year)+1900,
            (pUTCtime->tm_mon)+1,
            pUTCtime->tm_mday,
            pUTCtime->tm_hour,
            pUTCtime->tm_min,
            pUTCtime->tm_sec);
    }
    return ANSC_STATUS_SUCCESS;
}

#define VALUE_ALREADY_IN_DECIMAL       -1
#define INVALID_TIME_OFFSET_VAL         0

static int hexToInt(char s[])
{
    int hexdigit, i, num;
    bool inputIsValid;
    bool hexDigitFound = false ;

    i=0;
    if(s[i] == '0') {
        ++i;
        if(s[i] == 'x' || s[i] == 'X'){
            ++i;
        }
    }
    else if ( s[i] == '-' )
    {
        ++i;
        if ( s[i] == '\0' )
        {
            return INVALID_TIME_OFFSET_VAL ;
        }
    }
    num = 0;
    inputIsValid = true;
    for(; inputIsValid == true; ++i) {
        if ( s[i] == '\0' )
        {   
            break;
        }
        else if(s[i] >= '0' && s[i] <= '9') {
            hexdigit = s[i] - '0';
        } else if(s[i] >= 'a' && s[i] <= 'f') {
            hexDigitFound = true;
            hexdigit = s[i] - 'a' + 10;
        } else if(s[i] >= 'A' && s[i] <= 'F') {
            hexDigitFound = true;
            hexdigit = s[i] - 'A' + 10;
        } else {
            inputIsValid = false;
            break;
        }
        if(inputIsValid == true ) {
            num = 16 * num + hexdigit;
        }

    }
         
    if (inputIsValid == false )
    {
        return INVALID_TIME_OFFSET_VAL ;
    }
    else if (hexDigitFound == false)
    {
        return VALUE_ALREADY_IN_DECIMAL ;
    }
    else
    {
        return num;
    }
}

ANSC_STATUS getTimeOffset(char *name, char **timeOffset, int version)
{
    char offset_value[100]={0};
    char tempStr[100] = {0};
    errno_t safec_rc = -1;
    int decimal_Conv_Value = 0 ;
    memset(offset_value,0,sizeof(offset_value));
    commonSyseventGet(name, offset_value, sizeof(offset_value));
    if ( (*timeOffset != NULL) && ('\0' != offset_value[0] ) && ( 0 != strlen(offset_value) ) ) 
    {
        CcspTraceWarning(("%s: offset_value received from %s is %s \n", __FUNCTION__, name, offset_value));
        if ( offset_value[0] == '@' )
        {
            if( version == 6)
            {

                decimal_Conv_Value = hexToInt(offset_value+1) ;

                if ( decimal_Conv_Value == VALUE_ALREADY_IN_DECIMAL )
                {
                    safec_rc = strcpy_s(*timeOffset, MAX_COSATIMEOFFSET_SIZE, offset_value+1 );
                }
                else
                {
                    sprintf(tempStr, "%d", decimal_Conv_Value);
                    safec_rc = strcpy_s(*timeOffset, MAX_COSATIMEOFFSET_SIZE, tempStr );   
                }

            }
            else
            {
                safec_rc = strcpy_s(*timeOffset, MAX_COSATIMEOFFSET_SIZE, offset_value+1 );
            }
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
         }
         else
         {
            //hex to int conversion is only for ipv6-timeoffset
            if(version == 6)
            {
                    decimal_Conv_Value = hexToInt(offset_value) ;

                    if ( decimal_Conv_Value == VALUE_ALREADY_IN_DECIMAL )
                    {
                        safec_rc = strcpy_s(*timeOffset, MAX_COSATIMEOFFSET_SIZE, offset_value );
                    }
                    else
                    {
                        sprintf(tempStr, "%d", decimal_Conv_Value);
                        safec_rc = strcpy_s(*timeOffset, MAX_COSATIMEOFFSET_SIZE, tempStr );   
                    }
            }
            else
            {
             	safec_rc = strcpy_s(*timeOffset, MAX_COSATIMEOFFSET_SIZE, offset_value );
            }
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
         }
         return ANSC_STATUS_SUCCESS;
    }
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlTimeGetTimeOffset
    (
       ANSC_HANDLE                 hContext,
       char                       *pTimeOffset
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t safec_rc = -1;
    if(access("/nvram/ETHWAN_ENABLE", 0))
    {
	if( platform_hal_getTimeOffSet(pTimeOffset) == RETURN_OK )
	{
	    CcspTraceWarning(("%s: pTimeOffset is %s \n", __FUNCTION__,pTimeOffset));
	    return ANSC_STATUS_SUCCESS;
	}

    }
    if(ANSC_STATUS_SUCCESS == getTimeOffset("ipv6-timeoffset", &pTimeOffset, 6))
    {
        CcspTraceWarning(("%s: pTimeOffset is %s \n", __FUNCTION__,pTimeOffset));
        return ANSC_STATUS_SUCCESS;
    }
    if(ANSC_STATUS_SUCCESS == getTimeOffset("ipv4-timeoffset", &pTimeOffset, 4))
    {
        CcspTraceWarning(("%s: pTimeOffset is %s \n", __FUNCTION__,pTimeOffset));
        return ANSC_STATUS_SUCCESS;
    }
    safec_rc = sprintf_s(pTimeOffset, MAX_COSATIMEOFFSET_SIZE, "%d",0);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
   
    return ANSC_STATUS_SUCCESS;
}


#define PARTNER_ID_LEN 64
void FillParamUpdateSourceNTP(cJSON *partnerObj, char *key, char *paramUpdateSource)
{
    cJSON *paramObj = cJSON_GetObjectItem( partnerObj, key);
    errno_t rc = -1;
    if ( paramObj != NULL )
    {
        char *valuestr = NULL;
        cJSON *paramObjVal = cJSON_GetObjectItem(paramObj, "UpdateSource");
        if (paramObjVal)
            valuestr = paramObjVal->valuestring;
        if (valuestr != NULL)
        {
            rc = strcpy_s(paramUpdateSource, 16, valuestr);
            ERR_CHK(rc);
            valuestr = NULL;
        }
        else
        {
            CcspTraceWarning(("%s - %s UpdateSource is NULL\n", __FUNCTION__, key ));
        }
    }
    else
    {
        CcspTraceWarning(("%s - %s Object is NULL\n", __FUNCTION__, key ));
    }
}

void FillPartnerIDNTPJournal
    (
        cJSON *json ,
        char *partnerID ,
        PCOSA_DML_TIME_CFG pTimeCfg
    )
{
                cJSON *partnerObj = cJSON_GetObjectItem( json, partnerID );
                if( partnerObj != NULL)
                {
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer1", (char*)&pTimeCfg->NTPServer1.UpdateSource);
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer2", (char*)&pTimeCfg->NTPServer2.UpdateSource);
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer3", (char*)&pTimeCfg->NTPServer3.UpdateSource);
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer4", (char*)&pTimeCfg->NTPServer4.UpdateSource);
                      FillParamUpdateSourceNTP(partnerObj, "Device.Time.NTPServer5", (char*)&pTimeCfg->NTPServer5.UpdateSource);
                }
                else
                {
                      CcspTraceWarning(("%s - PARTNER ID OBJECT Value is NULL\n", __FUNCTION__ ));
                }
}

//Get the UpdateSource info from /opt/secure/bootstrap.json. This is needed to know for override precedence rules in set handlers
ANSC_STATUS
CosaNTPInitJournal
    (
        PCOSA_DML_TIME_CFG pTimeCfg
    )
{
        char *data = NULL;
        cJSON *json = NULL;
        FILE *fileRead = NULL;
        char PartnerID[PARTNER_ID_LEN] = {0};
        ULONG size = PARTNER_ID_LEN - 1;
        int len;
        errno_t safec_rc = -1;
        if (!pTimeCfg)
        {
                CcspTraceWarning(("%s-%d : NULL param\n" , __FUNCTION__, __LINE__ ));
                return ANSC_STATUS_FAILURE;
        }

        if (access(BOOTSTRAP_INFO_FILE, F_OK) != 0)
        {
                return ANSC_STATUS_FAILURE;
        }

         fileRead = fopen( BOOTSTRAP_INFO_FILE, "r" );
         if( fileRead == NULL )
         {
                 CcspTraceWarning(("%s-%d : Error in opening JSON file\n" , __FUNCTION__, __LINE__ ));
                 return ANSC_STATUS_FAILURE;
         }

         fseek( fileRead, 0, SEEK_END );
         len = ftell( fileRead );
         /* CID: 53991 Argument cannot be negative*/
         if(len < 0) {
            CcspTraceWarning(("%s-%d: File read negative return\n" , __FUNCTION__, __LINE__ ));
            fclose(fileRead);
            return ANSC_STATUS_FAILURE;
         }
         fseek( fileRead, 0, SEEK_SET );
         data = ( char* )malloc( sizeof(char) * (len + 1) );
         if (data != NULL)
         {
                memset( data, 0, ( sizeof(char) * (len + 1) ));
                /* CID 55708 fix*/
                size_t bytes_read = fread( data, 1, len, fileRead );
                if (bytes_read == 0)
                {
                        CcspTraceWarning(("%s-%d : Error in reading file\n", __FUNCTION__, __LINE__));
                }
         }
         else
         {
                 CcspTraceWarning(("%s-%d : Memory allocation failed \n", __FUNCTION__, __LINE__));
                 fclose( fileRead );
                 return ANSC_STATUS_FAILURE;
         }

         fclose( fileRead );
         /* CID: 135503 String not null terminated*/
         data[len] = '\0';
         if ( data == NULL )
         {
                CcspTraceWarning(("%s-%d : fileRead failed \n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
         }
         else if ( strlen(data) != 0)
         {
                 json = cJSON_Parse( data );
                 if( !json )
                 {
                         CcspTraceWarning((  "%s : json file parser error : [%d]\n", __FUNCTION__,__LINE__));
                         free(data);
                         return ANSC_STATUS_FAILURE;
                 }
                 else
                 {
                         if(ANSC_STATUS_SUCCESS == fillCurrentPartnerId(PartnerID, &size))
                         {
                                if ( PartnerID[0] != '\0' )
                                {
                                        CcspTraceWarning(("%s : Partner = %s \n", __FUNCTION__, PartnerID));
                                        FillPartnerIDNTPJournal(json, PartnerID, pTimeCfg);
                                }
                                else
                                {
                                        CcspTraceWarning(( "Reading Deafult PartnerID Values \n" ));
                                        safec_rc = strcpy_s(PartnerID, PARTNER_ID_LEN, "comcast");
                                        if(safec_rc != EOK)
                                        {
                                            ERR_CHK(safec_rc);
                                        }
                                        FillPartnerIDNTPJournal(json, PartnerID, pTimeCfg);
                                }
                        }
                        else{
                                CcspTraceWarning(("Failed to get Partner ID\n"));
                        }
                        cJSON_Delete(json);
                }
                free(data);
                data = NULL;
         }
         else
         {
                CcspTraceWarning(("BOOTSTRAP_INFO_FILE %s is empty\n", BOOTSTRAP_INFO_FILE));
                /*CID: 58474 Resource leak*/
                if(data)
                   free(data);
                return ANSC_STATUS_FAILURE;
         }
         return ANSC_STATUS_SUCCESS;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * CosaDmlTimeGetChronyEnable
 *
 * Returns TRUE in *pEnabled when syscfg key equals "true"
 * (chrony RFC flag is set), FALSE otherwise.  Always returns ANSC_STATUS_SUCCESS.
 * ─────────────────────────────────────────────────────────────────────────────*/
ANSC_STATUS
CosaDmlTimeGetChronyEnable
    (
        BOOL                       *pEnabled
    )
{
	 if (pEnabled == NULL)
     {
         CcspTraceError(("CosaDmlTimeGetChronyEnable: NULL output pointer\n"));
         return ANSC_STATUS_FAILURE;
     }

    char syscfgVal[8] = {0};
    syscfg_get(NULL, "chrony_enabled", syscfgVal, sizeof(syscfgVal));
    *pEnabled = (strcmp(syscfgVal, "true") == 0) ? TRUE : FALSE;
    CcspTraceInfo(("CosaDmlTimeGetChronyEnable: %s\n", *pEnabled ? "true" : "false"));
    return ANSC_STATUS_SUCCESS;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * CosaChronyNormalizeThreshold
 *
 * Format a makestep threshold float into canonical string form that always
 * carries a decimal point (e.g. 2 -> "2.0", 2.5 -> "2.5"). Shared by the
 * Makestep getter and setter so set/get round-trips stay consistent.
 * ─────────────────────────────────────────────────────────────────────────────*/
static void
CosaChronyNormalizeThreshold(float threshold, char *out, size_t outLen)
{
    snprintf(out, outLen, "%g", threshold);
    if (strchr(out, '.') == NULL)
    {
        size_t n = strlen(out);
        if (n + 2 < outLen)
            snprintf(out + n, outLen - n, ".0");
    }
}

/* ─────────────────────────────────────────────────────────────────────────────
 * CosaDmlTimeGetMakestep
 *
 * Reads syscfg key  and converts "makestep <threshold> <limit>"
 * to "<threshold>,<limit>" form in pValue.
 * Falls back to the compiled default "1.0,3" if the key is unset or malformed.
 * ─────────────────────────────────────────────────────────────────────────────*/
ANSC_STATUS
CosaDmlTimeGetMakestep
    (
        char                       *pValue,
        ULONG                       bufLen
    )
{
   char line[64] = {0};
    syscfg_get(NULL, "chrony_makestep", line, sizeof(line));
    if (line[0] != '\0')
    {
        float threshold = 0.0f;
        int   limit     = 0;
        if (sscanf(line, "makestep %f %d", &threshold, &limit) == 2)
        {
            char normThreshold[32] = {0};
            CosaChronyNormalizeThreshold(threshold, normThreshold, sizeof(normThreshold));
            snprintf(pValue, bufLen, "%s,%d", normThreshold, limit);
        }
        else {
            snprintf(pValue, bufLen, "1.0,3");
        }
	}
    else
    {
        snprintf(pValue, bufLen, "1.0,3");
    }
    CcspTraceInfo(("CosaDmlTimeGetMakestep: %s\n", pValue));
    return ANSC_STATUS_SUCCESS;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * CosaDmlTimeGetChronyServerSettings
 *
 * Reads syscfg key chrony_server{serverIdx}_settings and writes the value into
 * pValue.  Falls back to "pool,4,true,10,12" when the key is absent or empty.
 * ─────────────────────────────────────────────────────────────────────────────*/
ANSC_STATUS
CosaDmlTimeGetChronyServerSettings
    (
        int                         serverIdx,
        char                       *pValue,
        ULONG                       bufLen
    )
{
    char syscfgKey[32]  = {0};
    char syscfgVal[128] = {0};
    snprintf(syscfgKey, sizeof(syscfgKey), "chrony_server%d_settings", serverIdx);
    syscfg_get(NULL, syscfgKey, syscfgVal, sizeof(syscfgVal));
    if (syscfgVal[0] != '\0')
        snprintf(pValue, bufLen, "%s", syscfgVal);
    else
        snprintf(pValue, bufLen, "pool,4,true,10,12");
    CcspTraceInfo(("CosaDmlTimeGetChronyServerSettings[%d]: %s\n", serverIdx, pValue));
    return ANSC_STATUS_SUCCESS;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * CosaDmlTimeSetChronyEnable
 *
 * Enable or disable the Chrony NTP client via the syscfg RFC flag and sysevents.
 * Enable:  sysevent ntpd-stop  →  syscfg chrony_enabled=true  →  sysevent chrony-start
 * Disable: sysevent chrony-stop →  syscfg chrony_enabled=false →  sysevent ntpd-restart
 * ─────────────────────────────────────────────────────────────────────────────*/
ANSC_STATUS
CosaDmlTimeSetChronyEnable
    (
        BOOL                        bEnabled
    )
{
    int rc;

    /* No-op when the requested state already matches the current state:
     * do not rewrite the syscfg key or fire any sysevent. */
	 char syscfgVal[8] = {0};
    syscfg_get(NULL, "chrony_enabled", syscfgVal, sizeof(syscfgVal));
    BOOL bReq     = bEnabled ? TRUE : FALSE;
    BOOL bCurrent = (strcmp(syscfgVal, "true") == 0) ? TRUE : FALSE;
    if (bReq == bCurrent)
    {
        CcspTraceInfo(("CosaDmlTimeSetChronyEnable: already %s, no action\n",
                       bReq ? "enabled" : "disabled"));
        return ANSC_STATUS_SUCCESS;
    }

    if (bEnabled)
    {
        CcspTraceInfo(("CosaDmlTimeSetChronyEnable: enabling chrony\n"));

        /* Persist the RFC flag before starting chronyd */
        if (syscfg_set_commit(NULL, "chrony_enabled", "true") != 0)
        {
            CcspTraceError(("CosaDmlTimeSetChronyEnable: failed to set syscfg chrony_enabled=true"));
            return ANSC_STATUS_FAILURE;
        }
   
		rc = v_secure_system("sysevent set ntpd-stop");
        if (rc != 0)
            CcspTraceWarning(("CosaDmlTimeSetChronyEnable: ntpd-stop sysevent returned %d\n", rc));
		
        rc = v_secure_system("sysevent set chronyd-start");
        if (rc != 0)
            CcspTraceWarning(("CosaDmlTimeSetChronyEnable: chronyd-start sysevent returned %d\n", rc));
    }
    else
    {
        CcspTraceInfo(("CosaDmlTimeSetChronyEnable: disabling chrony\n"));
        rc = v_secure_system("sysevent set chronyd-stop");
        if (rc != 0)
            CcspTraceWarning(("CosaDmlTimeSetChronyEnable: chronyd-stop sysevent returned %d\n", rc));

        if (syscfg_set_commit(NULL, "chrony_enabled", "false") != 0)
        {
            CcspTraceError(("CosaDmlTimeSetChronyEnable: failed to set syscfg chrony_enabled=false\n"));
            return ANSC_STATUS_FAILURE;
        }

        rc = v_secure_system("sysevent set ntpd-restart");
        if (rc != 0)
            CcspTraceWarning(("CosaDmlTimeSetChronyEnable: ntpd-restart sysevent returned %d\n", rc));
    }

    return ANSC_STATUS_SUCCESS;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * CosaDmlTimeSetMakestep
 *
 * Parse "threshold,limit" (e.g. "1.0,3"), validate, and write
 * "makestep <threshold> <limit>" to syscfg key chrony_makestep
 * Persists across reboots. Fires chrony-restart sysevent when chrony is
 * currently enabled (syscfg chrony_enabled==true) so the daemon picks
 * up the new value immediately.
 * ─────────────────────────────────────────────────────────────────────────────
 */

ANSC_STATUS
CosaDmlTimeSetMakestep
    (
        char                       *pValue
    )
{
    if (pValue == NULL)
    {
        CcspTraceError(("CosaDmlTimeSetMakestep: NULL input\n"));
        return ANSC_STATUS_FAILURE;
    }

    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "%s", pValue);

    char *comma = strchr(buf, ',');
    if (comma == NULL)
    {
        CcspTraceError(("CosaDmlTimeSetMakestep: missing comma separator in '%s'\n", pValue));
        return ANSC_STATUS_FAILURE;
    }
    *comma = '\0';
    char *limitStr = comma + 1;

    /* Validate threshold: must be a float in the range 1.0 - 10.0 */
    char *endptr = NULL;
    float threshold = strtof(buf, &endptr);
    if (endptr == buf || *endptr != '\0' || threshold < 1.0f || threshold > 10.0f)
    {
        CcspTraceError(("CosaDmlTimeSetMakestep: invalid threshold '%s' (must be 1-10)\n", buf));
        return ANSC_STATUS_FAILURE;
    }

    /* Validate limit: must be an integer >= 1, no extra text (rejects e.g. "1,0,0") */
    errno = 0;
    long limit = strtol(limitStr, &endptr, 10);
    if (endptr == limitStr || *endptr != '\0' || errno != 0 || limit < 1)
    {
        CcspTraceError(("CosaDmlTimeSetMakestep: invalid limit '%s' (must be >= 1 integer)\n", limitStr));
        return ANSC_STATUS_FAILURE;
    }

    /* Normalize the threshold to canonical form (e.g. "2." -> "2.0") and build the
     * directive that will be persisted. */
    char normThreshold[32] = {0};
    CosaChronyNormalizeThreshold(threshold, normThreshold, sizeof(normThreshold));

    char directive[80] = {0};
    snprintf(directive, sizeof(directive), "makestep %s %ld", normThreshold, limit);

    /* No-op when the normalized directive already matches the persisted value:
     * do not rewrite the syscfg or fire a restart.
	 */
    char cur[80] = {0};
    syscfg_get(NULL, "chrony_makestep", cur, sizeof(cur));
    if (cur[0] != '\0')
    {
        cur[strcspn(cur, "\n")] = '\0';
        if (strcmp(cur, directive) == 0)
        {
            CcspTraceInfo(("CosaDmlTimeSetMakestep: '%s' unchanged, no action\n", directive));
            return ANSC_STATUS_SUCCESS;
        }
    }

     /* Persist the directive to syscfg so it survives reboots */
    if (syscfg_set_commit(NULL, "chrony_makestep", directive) != 0)
	{
       CcspTraceError(("CosaDmlTimeSetMakestep: cannot set syscfg chrony_makestep\n"));
        return ANSC_STATUS_FAILURE;
    }
 
    CcspTraceInfo(("CosaDmlTimeSetMakestep: wrote '%s' to syscfg chrony_makestep\n", directive));

    /* Trigger a live config reload — service_chronyd.sh guards on RFC flag internally */
    int rc = v_secure_system("sysevent set chronyd-restart");
    if (rc != 0)
        CcspTraceWarning(("CosaDmlTimeSetMakestep: sysevent set chronyd-restart returned %d\n", rc));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlTimeSetChronyServerSettings
    (
        int                         serverIdx,
        const char                 *pValue
    )
{
    if (pValue == NULL || serverIdx < 1 || serverIdx > 5)
    {
        CcspTraceError(("CosaDmlTimeSetChronyServerSettings: invalid args idx=%d value=%s\n",
                        serverIdx, pValue ? pValue : "NULL"));
        return ANSC_STATUS_FAILURE;
    }

    /* Parse on a local copy */
    char buf[128] = {0};
    snprintf(buf, sizeof(buf), "%s", pValue);

    /* Split positionally on every comma: unlike strtok, this does NOT collapse
     * consecutive delimiters, so empty fields are preserved and detected. This
     * matches how build_chrony_conf.sh consumes the value (positional cut -d','),
     * ensuring an accepted value maps to the intended directive fields. */
    char *fields[5] = {NULL};
    int   nfields   = 1;      /* one field precedes the first comma */
    fields[0]       = buf;
    for (char *p = buf; *p != '\0'; p++)
    {
        if (*p == ',')
        {
            *p = '\0';
            if (nfields < 5)
                fields[nfields] = p + 1;
            nfields++;        /* count total fields to catch > 5 */
        }
    }
    /* Reject if the field count is not exactly 5 (too few or too many). */
    if (nfields != 5)
    {
        CcspTraceError(("CosaDmlTimeSetChronyServerSettings: expected 5 fields, got %d in '%s'\n",
                        nfields, pValue));
        return ANSC_STATUS_FAILURE;
    }
    /* Reject if any field is empty (empty leading, middle, trailing, or "" input). */
    for (int k = 0; k < 5; k++)
    {
        if (fields[k][0] == '\0')
        {
            CcspTraceError(("CosaDmlTimeSetChronyServerSettings: empty field %d in '%s'\n",
                            k + 1, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }

    /* Validate source_type */
    char *source_type = fields[0];
    if (strcmp(source_type, "pool") != 0 && strcmp(source_type, "server") != 0)
    {
        CcspTraceError(("CosaDmlTimeSetChronyServerSettings: invalid source_type '%s' (must be pool|server)\n",
                        source_type));
        return ANSC_STATUS_FAILURE;
    }

    /* Validate maxsources: must be a valid integer whose allowed range depends
     * on source_type:
     *   pool   -> 1-10
     *   server -> must be 0 (maxsources is not applicable to a single server directive) */
    char *endptr = NULL;
    errno = 0;
    long maxsources = strtol(fields[1], &endptr, 10);
    if (endptr == fields[1] || *endptr != '\0' || errno != 0)
    {
        CcspTraceError(("CosaDmlTimeSetChronyServerSettings: invalid maxsources '%s' (must be an integer)\n",
                        fields[1]));
        return ANSC_STATUS_FAILURE;
    }
    if (strcmp(source_type, "server") == 0)
    {
        if (maxsources != 0)
        {
            CcspTraceError(("CosaDmlTimeSetChronyServerSettings: maxsources must be 0 for source_type server (got '%s')\n",
                            fields[1]));
            return ANSC_STATUS_FAILURE;
        }
    }
    else /* pool */
    {
        if (maxsources < 1 || maxsources > 10)
        {
            CcspTraceError(("CosaDmlTimeSetChronyServerSettings: maxsources must be 1-10 for source_type pool (got '%s')\n",
                            fields[1]));
            return ANSC_STATUS_FAILURE;
        }
    }

    /* Validate iburst */
    char *iburst = fields[2];
    if (strcmp(iburst, "true") != 0 && strcmp(iburst, "false") != 0)
    {
        CcspTraceError(("CosaDmlTimeSetChronyServerSettings: invalid iburst '%s' (must be true|false)\n",
                        iburst));
        return ANSC_STATUS_FAILURE;
    }

    /* Validate minpoll: integer 4-24 */
    errno = 0;
    long minpoll = strtol(fields[3], &endptr, 10);
    if (endptr == fields[3] || *endptr != '\0' || errno != 0 || minpoll < 4 || minpoll > 24)
    {
        CcspTraceError(("CosaDmlTimeSetChronyServerSettings: invalid minpoll '%s' (must be 4-24)\n",
                        fields[3]));
        return ANSC_STATUS_FAILURE;
    }

    /* Validate maxpoll: integer 4-24, >= minpoll */
    errno = 0;
    long maxpoll = strtol(fields[4], &endptr, 10);
    if (endptr == fields[4] || *endptr != '\0' || errno != 0 || maxpoll < 4 || maxpoll > 24 || maxpoll < minpoll)
    {
        CcspTraceError(("CosaDmlTimeSetChronyServerSettings: invalid maxpoll '%s' (must be 4-24, >= minpoll %ld)\n",
                        fields[4], minpoll));
        return ANSC_STATUS_FAILURE;
    }

    /* All fields valid — persist to syscfg */
    char syscfgKey[32] = {0};
    snprintf(syscfgKey, sizeof(syscfgKey), "chrony_server%d_settings", serverIdx);

    /* No-op when the incoming value already matches the stored value:
     * do not re-commit syscfg or fire a restart. */
    char curVal[128] = {0};
    syscfg_get(NULL, syscfgKey, curVal, sizeof(curVal));
    if (strcmp(curVal, pValue) == 0)
    {
        CcspTraceInfo(("CosaDmlTimeSetChronyServerSettings: %s unchanged (%s), no action\n",
                       syscfgKey, pValue));
        return ANSC_STATUS_SUCCESS;
    }

    if (syscfg_set(NULL, syscfgKey, pValue) != 0)
    {
        CcspTraceError(("CosaDmlTimeSetChronyServerSettings: syscfg_set(%s) failed\n", syscfgKey));
        return ANSC_STATUS_FAILURE;
    }
    if (syscfg_commit() != 0)
    {
        CcspTraceError(("CosaDmlTimeSetChronyServerSettings: syscfg_commit failed\n"));
        return ANSC_STATUS_FAILURE;
    }

    CcspTraceInfo(("CosaDmlTimeSetChronyServerSettings: %s = %s\n", syscfgKey, pValue));

    /* Trigger a live config reload — service_chronyd.sh guards on RFC flag internally */
    int rc = v_secure_system("sysevent set chronyd-restart");
    if (rc != 0)
        CcspTraceWarning(("CosaDmlTimeSetChronyServerSettings: sysevent set chronyd-restart returned %d\n", rc));

    return ANSC_STATUS_SUCCESS;
}

#endif

