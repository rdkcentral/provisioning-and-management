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

#ifdef __GNUC__
#if (!defined _BUILD_ANDROID) && (!defined _NO_EXECINFO_H_)
#include <execinfo.h>
#endif
#endif

#include <sys/stat.h>
#include <fcntl.h>

#include <syscfg/syscfg.h>
#include "ssp_global.h"
#include "stdlib.h"
#include "ccsp_dm_api.h"
#include "cosa_ip_apis.h"
#include <sys/sysinfo.h>
#ifdef USE_PCD_API_EXCEPTION_HANDLING
#include "pcdapi.h"
#endif

#ifdef INCLUDE_BREAKPAD
#include "breakpad_wrapper.h"
#endif

#include <semaphore.h>
#include <fcntl.h>

#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8081
#endif

#include "webconfig_framework.h"
#include "secure_wrapper.h"
#include "safec_lib_common.h"
#include "telemetry_busmessage_sender.h"

#define DEBUG_INI_NAME  "/etc/debug.ini"
// With WAN boot time optimization, in few cases P&M initialization is further delayed
// Since there is no evidence of P&M APIs being hung, increasing the timeout period to one more minute.
#define PAM_CRASH_TIMEOUT 360  //seconds
#define PAM_INIT_FILE "/tmp/pam_initialized"

//  Existing pam_initialized is removed from systemd/selfheal . Created this file to determine if component is coming after crashed to sync values from server.
#define PAM_INIT_FILE_BOOTUP "/tmp/pam_initialized_bootup"

PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController      = NULL;
PCOMPONENT_COMMON_DM            g_pComponent_Common_Dm  = NULL;
char                            g_Subsystem[32]         = {0};
PCCSP_COMPONENT_CFG             gpPnmStartCfg           = NULL;
PCCSP_FC_CONTEXT                pPnmFcContext           = (PCCSP_FC_CONTEXT           )NULL;
PCCSP_CCD_INTERFACE             pPnmCcdIf               = (PCCSP_CCD_INTERFACE        )NULL;
PCCC_MBI_INTERFACE              pPnmMbiIf               = (PCCC_MBI_INTERFACE         )NULL;
BOOL                            g_bActive               = FALSE;
extern  ANSC_HANDLE                     bus_handle;
int GetLogInfo(ANSC_HANDLE bus_handle, char *Subsytem, char *pParameterName);
#ifndef _COSA_INTEL_USG_ATOM_
void
CcspBaseIf_deadlock_detection_log_print
(
    int sig
);
#endif

void get_uptime(long *uptime)
{
    struct sysinfo info;
    sysinfo( &info );
    *uptime= info.uptime;
}

    sem_t *sem;


int  cmd_dispatch(int  command)
{
    char*                           pParamNames[]      = {"Device.X_CISCO_COM_DDNS."};
    parameterValStruct_t**          ppReturnVal        = NULL;
    ULONG                           ulReturnValCount   = 0;
    ULONG                           i                  = 0;
    errno_t                         rc                 = -1;

    switch ( command )
    {
            case	'e' :

                CcspTraceInfo(("Connect to bus daemon...\n"));

            {
                char                            CName[256];

                rc = sprintf_s(CName, sizeof(CName), "%s%s", g_Subsystem, gpPnmStartCfg->ComponentId);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                    return -1;
                }

                ssp_PnmMbi_MessageBusEngage
                    ( 
                        CName,
                        CCSP_MSG_BUS_CFG,
                        gpPnmStartCfg->DbusPath
                    );
            }


                ssp_create_pnm(gpPnmStartCfg);
                ssp_engage_pnm(gpPnmStartCfg);

                g_bActive = TRUE;

                CcspTraceInfo(("Provision & Management Module loaded successfully...\n"));

            break;

            case    'r' :

            CcspCcMbi_GetParameterValues
                (
                    DSLH_MPA_ACCESS_CONTROL_ACS,
                    pParamNames,
                    1,
                    (int *)&ulReturnValCount,
                    &ppReturnVal,
                    NULL
                );



            for ( i = 0; i < ulReturnValCount; i++ )
            {
                CcspTraceWarning(("Parameter %lu name: %s value: %s \n", i+1, ppReturnVal[i]->parameterName, ppReturnVal[i]->parameterValue));
            }


/*
            CcspCcMbi_GetParameterNames
                (
                    "Device.DeviceInfo.",
                    0,
                    &ulReturnValCount,
                    &ppReturnValNames
                );

            for ( i = 0; i < ulReturnValCount; i++ )
            {
                CcspTraceWarning(("Parameter %d name: %s bWritable: %d \n", i+1, ppReturnValNames[i]->parameterName, ppReturnValNames[i]->writable));
            }
*/
/*
            CcspCcMbi_GetParameterAttributes
                (
                    pParamNames,
                    1,
                    &ulReturnValCount,
                    &ppReturnvalAttr
                );
*/
/*
            CcspCcMbi_DeleteTblRow
                (
                    123,
                    "Device.X_CISCO_COM_SWDownload.SWDownload.1."
                );
*/

			break;

        case    'm':

                AnscPrintComponentMemoryTable(pComponentName);

                break;

        case    't':

                AnscTraceMemoryTable();

                break;

        case    'c':

                ssp_cancel_pnm(gpPnmStartCfg);

                break;

        default:
            break;
    }

    return 0;
}


#if 0
static void _print_stack_backtrace(void)
{
#ifdef __GNUC__
#if (!defined _BUILD_ANDROID) && (!defined _NO_EXECINFO_H_)
        void* tracePtrs[100];
        char** funcNames = NULL;
        int i, count = 0;

        int fd;
        const char* path = CCSP_PNM_BACKTRACE_FILE;

        /*
         *  print backtrace to the backtrace file
         */
        count = backtrace( tracePtrs, 100 );

        fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd < 0)
        {
            fprintf(stderr, "failed to open backtrace file: %s", path);
        }
        else
        {
            backtrace_symbols_fd( tracePtrs, count, fd);
            close(fd);
        }
        /*
         *  print backtrace to stdout
         */
        backtrace_symbols_fd(tracePtrs, count, 1);
#endif
#endif
}
#endif

static void daemonize(void) {
	int s,i;
        pid_t   pid;
        int svalue = -1;
        long uptime1=0, uptime2=0, diff=0;
	
	/* initialize semaphores for shared processes */
	sem = sem_open ("pSemPnm", O_CREAT | O_EXCL, 0644, 0);
	if(SEM_FAILED == sem)
	{
	       AnscTrace("Failed to create semaphore %d - %s\n", errno, strerror(errno));
	       _exit(1);
	}
	/* name of semaphore is "pSemPnm", semaphore is reached using this name */
        s = sem_unlink("pSemPnm");
        if (s < 0)
        {
            CcspTraceInfo(("-------------------PAM_DBG: sem_unlink returns error %d - %s\n----------------", errno, strerror(errno)));
            _exit(1);
        }
	/* unlink prevents the semaphore existing forever */
	/* if a crash occurs during the execution         */
	AnscTrace("Semaphore initialization Done!!\n");

        CcspTraceInfo(("PAM_DBG:---------------------daemonize calls fork------------------------\n"));
	switch (pid = fork()) {
	case 0:
                CcspTraceInfo(("PAM_DBG:--------------child process begins---------------\n"));
		break;
	case -1:
		// Error
		CcspTraceInfo(("Error daemonizing (fork)! %d - %s\n", errno, strerror(
				errno)));
		exit(0);
		break;
	default:
                CcspTraceInfo(("PAM_DBG:--------------waiting for child process to initialize------------\n"));

                get_uptime(&uptime1);
                CcspTraceInfo(("PAM_DBG:------------uptime before waiting for sem is %ld seconds ---------------\n", uptime1));

                s = sem_getvalue(sem, &svalue);
                if (s < 0)
                {
                    CcspTraceInfo(("PAM_DBG:-------------sem_getvalue returns error %d - %s, continuing-------------\n", errno, strerror(errno)));
                }
                CcspTraceInfo(("PAM_DBG:----------------sem_getvalue returns value = %d -------------\n", svalue));

                for (i = 0 ; i < PAM_CRASH_TIMEOUT; i += 2)
                {
                    if (sem_trywait(sem) == 0)
                    {
                        CcspTraceInfo(("PAM_DBG:---------------sem_trywait() returns 0 ---------------------\n"));
                        break;
                    }
                    sleep(2);
                }

                get_uptime(&uptime2);
                CcspTraceInfo(("PAM_DBG:---------------uptime after waiting for sem is %ld seconds ---------------------\n", uptime2));

                s = sem_close(sem);
                if (s < 0)
                {
                    CcspTraceInfo(("PAM_DEBUG:---------------sem_close returns error %d - %s, continuing-------------\n", errno, strerror(errno)));
                }

                // difference time exceeds TIMEOUT, please kill the child pam process
                diff = uptime2 - uptime1;
                CcspTraceInfo(("PAM_DBG:------------diff time is %ld -------------\n", diff));
                if(diff >= PAM_CRASH_TIMEOUT)
                {
                    CcspTraceInfo(("PAM_DBG:------------kill process after timeout, pid = %d -----------\n", pid));
                    s = kill(pid, SIGKILL);
                    if (s < 0)
                    {
                        CcspTraceInfo(("PAM_DBG: kill returns error %d - %s ---------------\n", errno, strerror(errno)));
                    }
                    else
                    {
                        CcspTraceInfo(("PAM_DBG:-------------kill success------------\n"));
                    }
                    CcspTraceInfo(("PAM_DBG:----------remove /tmp/pam_initialized---------\n"));
                    s = unlink(PAM_INIT_FILE);
                    if (s < 0)
                    {
                        /*CID: 133860 Argument cannot be negative*/
                        CcspTraceInfo(("PAM_DBG:------------unlink returned %d - %s ------------\n", s, strerror(errno)));
                    }
                }
		_exit(0);
	}

	if (setsid() < 	0) {
		CcspTraceInfo(("Error demonizing (setsid)! %d - %s\n", errno, strerror(errno)));
		exit(0);
	}

    /*
     *  What is the point to change current directory?
     *
    chdir("/");
     */

#ifndef  _DEBUG

	fd = open("/dev/null", O_RDONLY);
	if (fd != 0) {
		dup2(fd, 0);
		close(fd);
	}
	fd = open("/dev/null", O_WRONLY);
	if (fd != 1) {
		dup2(fd, 1);
		close(fd);
	}
	fd = open("/dev/null", O_WRONLY);
	if (fd != 2) {
		dup2(fd, 2);
		close(fd);
	}
#endif
}

void sig_handler(int sig)
{

#ifndef _COSA_INTEL_USG_ATOM_
    CcspBaseIf_deadlock_detection_log_print(sig);
#endif

    if ( sig == SIGINT ) {
    	signal(SIGINT, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGINT received!\n"));
        exit(0);
    }
    else if ( sig == SIGUSR1 ) {
    	signal(SIGUSR1, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGUSR1 received!\n"));
    }
    else if ( sig == SIGUSR2 ) {
    	CcspTraceInfo(("SIGUSR2 received!\n"));
    }
    else if ( sig == SIGCHLD ) {
    	signal(SIGCHLD, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGCHLD received!\n"));
    }
    else if ( sig == SIGPIPE ) {
    	signal(SIGPIPE, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGPIPE received!\n"));
    }
    else if ( sig == SIGTERM )
    {
        CcspTraceInfo(("SIGTERM received!\n"));
        exit(0);
    }
    else if ( sig == SIGKILL )
    {
        CcspTraceInfo(("SIGKILL received!\n"));
        exit(0);
    }
	else if ( sig == SIGALRM ) 
	{

    	signal(SIGALRM, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGALRM received!\n"));
		
	}
    else {
    	/* get stack trace first */
    	//_print_stack_backtrace();
    	CcspTraceInfo(("Signal %d received, exiting!\n", sig));
    	exit(0);
    }
}

#ifndef INCLUDE_BREAKPAD
static int is_core_dump_opened(void)
{
    FILE *fp;
    char path[256];
    char line[1024];
    char *start, *tok, *sp;
#define TITLE   "Max core file size"

    snprintf(path, sizeof(path), "/proc/%d/limits", getpid());
    if ((fp = fopen(path, "rb")) == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if ((start = strstr(line, TITLE)) == NULL)
            continue;

        start += strlen(TITLE);
        if ((tok = strtok_r(start, " \t\r\n", &sp)) == NULL)
            break;

        fclose(fp);

        if (strcmp(tok, "0") == 0)
            return 0;
        else
            return 1;
    }

    fclose(fp);
    return 0;
}
#endif

#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
int sock = 0;
#endif


int main(int argc, char* argv[])
{
    int                             cmdChar            = 0;
    BOOL                            bRunAsDaemon       = TRUE;
    int                             idx                = 0;
    char                            cmd[1024]          = {0};
    FILE                           *fd                 = NULL;
    DmErr_t                         err;
    char                            *subSys            = NULL;
    extern ANSC_HANDLE bus_handle;
    BOOL bDebugSlowChildProcess = FALSE;
    FILE *fp;
    int s, svalue = -1;
    errno_t rc    = -1;
    int     ret   = 0;

    // Buffer characters till newline for stdout and stderr
    setlinebuf(stdout);
    setlinebuf(stderr);

#ifdef FEATURE_SUPPORT_RDKLOG
    RDK_LOGGER_INIT();
#endif

#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
	int id=0;
	id=getuid();
#endif

    /*
     *  Load the start configuration
     */
    gpPnmStartCfg = (PCCSP_COMPONENT_CFG)AnscAllocateMemory(sizeof(CCSP_COMPONENT_CFG));
    
    if ( gpPnmStartCfg )
    {
        CcspComponentLoadCfg(CCSP_PNM_START_CFG_FILE, gpPnmStartCfg);
    }
    else
    {
        CcspTraceError(("RDKB_SYSTEM_BOOT_UP_LOG : PandM,Insufficient resources for start configuration, quit!\n"));
        printf("Insufficient resources for start configuration, quit!\n");
        exit(1);
    }
    
    /* Set the global pComponentName */
    pComponentName = gpPnmStartCfg->ComponentName;

//REFPLTV-5 : RDKB Container lxcclient code to send events from pandm to host
#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_BANANAPI_R4_)
if(id != 0)
{
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
}
#endif


#if defined(_DEBUG) && defined(_COSA_SIM_)
    AnscSetTraceLevel(CCSP_TRACE_LEVEL_INFO);
#endif

    for (idx = 1; idx < argc; idx++)
    {
        if ( (strcmp(argv[idx], "-subsys") == 0) )
        {
           if ((idx+1) < argc)
           {
               rc = strcpy_s(g_Subsystem, sizeof(g_Subsystem), argv[idx+1]);
               if(rc != EOK)
               {
                  ERR_CHK(rc);
                  CcspTraceError(("exit ERROR %s:%d\n", __FUNCTION__, __LINE__));
                  exit(1);
               }
           }
           else
           {
               CcspTraceError(("Argument missing after -subsys\n"));
           }
           CcspTraceWarning(("\nSubsystem is %s\n", g_Subsystem));
        }
        else if ( strcmp(argv[idx], "-c" ) == 0 )
        {
            bRunAsDaemon = FALSE;
        }
        else if (strcmp(argv[idx], "-debugslowchildprocess") == 0)
        {
            bDebugSlowChildProcess = TRUE;
            CcspTraceInfo(("PAM_DBG:-------bDebugSlowChildProcess hit-----------\n"));
        }
    }

    // To identify slow child process
    fp = fopen("/tmp/debugslowchildprocess", "r");
    if (fp)
    {
        bDebugSlowChildProcess = TRUE;
        fclose(fp);
    }

    if ( bRunAsDaemon )
    {
        CcspTraceInfo(("PAM_DBG:------- daemonize call hit -----------\n"));
        daemonize();
    }

    /*This is used for ccsp recovery manager */
    fd = fopen("/var/tmp/CcspPandMSsp.pid", "w+");
    if ( !fd )
    {
        CcspTraceWarning(("Create /var/tmp/CcspPandMSsp.pid error. \n"));
    }
    else
    {
        rc = sprintf_s(cmd, sizeof(cmd),"%d", getpid());
        if(rc < EOK)
        {
          ERR_CHK(rc);
          CcspTraceError(("exit ERROR %s:%d\n", __FUNCTION__, __LINE__));
          fclose(fd);
          exit(1);
        }
        fputs(cmd, fd);
        fclose(fd);
    }
#ifdef INCLUDE_BREAKPAD
    breakpad_ExceptionHandler();
    signal(SIGUSR1, sig_handler);
#endif
#ifndef INCLUDE_BREAKPAD

    if (is_core_dump_opened())
    {
        signal(SIGUSR1, sig_handler);
        signal(SIGPIPE, SIG_IGN);
        CcspTraceWarning(("Core dump is opened, do not catch signal\n"));
    }
    else
    {
        CcspTraceWarning(("Core dump is NOT opened, backtrace if possible\n"));
        signal(SIGTERM, sig_handler);
        signal(SIGINT, sig_handler);
        signal(SIGUSR1, sig_handler);
        signal(SIGUSR2, sig_handler);

        signal(SIGSEGV, sig_handler);
        signal(SIGBUS, sig_handler);
        signal(SIGKILL, sig_handler);
        signal(SIGFPE, sig_handler);
        signal(SIGILL, sig_handler);
        signal(SIGQUIT, sig_handler);
        signal(SIGHUP, sig_handler);
        signal(SIGPIPE, SIG_IGN);
    }

#ifdef USE_PCD_API_EXCEPTION_HANDLING
    printf("Registering PCD exception handler\n");
    CcspTraceWarning(("RDKB_SYSTEM_BOOT_UP_LOG : PandM Registering PCD exception handler... \n"));
    PCD_api_register_exception_handlers( argv[0], NULL );
#endif
#endif

   t2_init("CcspPandM");
   ret = cmd_dispatch('e');
   if(ret != 0)
   {
     CcspTraceError(("exit ERROR %s:%d\n", __FUNCTION__, __LINE__));
     exit(1);
   }

    // printf("Calling Docsis\n");

    // ICC_init();
    // DocsisIf_StartDocsisManager();

#ifdef _COSA_SIM_
    subSys = "";        /* PC simu use empty string as subsystem */
#else
    subSys = NULL;      /* use default sub-system */
#endif
    err = Cdm_Init(bus_handle, subSys, NULL, NULL, pComponentName);
    if (err != CCSP_SUCCESS)
    {
        fprintf(stderr, "Cdm_Init: %s\n", Cdm_StrError(err));
        exit(1);
    }

    check_component_crash(PAM_INIT_FILE_BOOTUP);

    CcspTraceInfo(("PAM_DBG:----------------------touch /tmp/pam_initialized-------------------\n"));
    v_secure_system("touch " PAM_INIT_FILE " ; touch " PAM_INIT_FILE_BOOTUP);

#ifdef FEATURE_COGNITIVE_WIFIMOTION
    char value[6] = { 0 };
    if (syscfg_get(NULL, "wifimotion_enabled", value, sizeof(value)) == 0)
    {
        if (!strncmp(value, "true", 4))
        {
            v_secure_system("systemctl start systemd-cognitive_wifimotion.service");
        }
    }
#endif

#if !defined(_COSA_INTEL_XB3_ARM_)
    char buf[64] = {'\0'};
    if(syscfg_get( NULL, "EnableTR69Binary", buf, sizeof(buf))==0)
    {
         if (strncmp(buf, "false", 5) == 0)
             /*check this file during Tr069 service start*/
             v_secure_system("touch /tmp/disableTr069");
    }
#endif
	
    if ( bRunAsDaemon )
    {
       if (bDebugSlowChildProcess)
       {
           CcspTraceInfo(("PAM_DBG:-----------------waiting for 600 seconds to trigger parent to child this process-----------------\n"));
           sleep(600);
       }
       s = sem_getvalue(sem, &svalue);
       if (s < 0)
       {
           CcspTraceInfo(("PAM_DEBUG:------------sem_getvalue returns error %d - %s, continuing-------------\n", errno, strerror(errno)));
       }
       CcspTraceInfo(("PAM_DEBUG:------------sem_getvalue returns value = %d ------------\n", svalue));
       CcspTraceInfo(("PAM_DBG:----------------Before calling sem_post--------------------\n"));
       s = sem_post(sem);
       if (s < 0)
       {
           CcspTraceInfo(("PAM_DEBUG:-------------sem_post returns error %d - %s, continuing-------------\n", errno, strerror(errno)));
       }
       s = sem_close(sem);
       if (s < 0)
       {
           CcspTraceInfo(("PAM_DEBUG:---------------sem_close returns error %d - %s, continuing-------------\n", errno, strerror(errno)));
       }
    }

#if defined(_COSA_INTEL_USG_ARM_) 
    {
        #define DATA_SIZE 1024
        FILE *fp1;
        char buf[DATA_SIZE] = {0};
        char *urlPtr = NULL;

        // Grab the ATOM RPC IP address
        // sprintf(cmd1, "cat /etc/device.properties | grep ATOM_ARPING_IP | cut -f 2 -d\"=\"");

        fp1 = fopen("/etc/device.properties", "r");
        if (fp1 == NULL) {
            CcspTraceError(("Error opening properties file! \n"));
            return FALSE;
        }

        CcspTraceInfo(("Searching for ATOM ARP IP\n"));

        while (fgets(buf, DATA_SIZE, fp1) != NULL) {
            // Look for ATOM_ARPING_IP
            if (strstr(buf, "ATOM_ARPING_IP") != NULL) {
                buf[strcspn(buf, "\r\n")] = 0; // Strip off any carriage returns

                // grab URL from string
                urlPtr = strstr(buf, "=");
                urlPtr++;
                break;
            }
        }

        if (fclose(fp1) != 0) {
            /* Error reported by pclose() */
            CcspTraceError(("Error closing properties file! \n"));
        }

        if (urlPtr != NULL && urlPtr[0] != 0 && strlen(urlPtr) > 0) {
            CcspTraceInfo(("Reported an ATOM IP of %s \n", urlPtr));
            CcspTraceInfo(("PAM_DBG:-----------------touch pam_initialized in atom ----------------\n"));
            v_secure_system("/usr/bin/rpcclient %s '/bin/touch /tmp/pam_initialized'&", urlPtr);
            CcspTraceInfo(("PAM_DBG:-----------------created pam_initialized in atom ----------------\n"));
        }
    }
#endif

    printf("Entering P&M loop\n");
    CcspTraceWarning(("RDKB_SYSTEM_BOOT_UP_LOG : Entering P&M loop... \n"));

    if ( bRunAsDaemon )
    {
        while(1)
        {
            sleep(30);
        }
    }
    else
    {
        while ( cmdChar != 'q' )
        {
            cmdChar = getchar();

            ret = cmd_dispatch(cmdChar);
            if(ret != 0)
            {
              CcspTraceError(("exit ERROR %s:%d\n", __FUNCTION__, __LINE__));
              exit(1);
            }
        }
    }

    err = Cdm_Term();
    if (err != CCSP_SUCCESS)
    {
        fprintf(stderr, "Cdm_Term: %s\n", Cdm_StrError(err));
        exit(1);
    }

    if ( g_bActive )
    {
        ssp_cancel_pnm(gpPnmStartCfg);

        g_bActive = FALSE;
    }
	


    return 0;
}


