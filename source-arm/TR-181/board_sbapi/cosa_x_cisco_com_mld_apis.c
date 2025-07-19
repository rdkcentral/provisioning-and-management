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

    module: cosa_x_cisco_com_mld_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlNatGetPortMappingNumber
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
#include "cosa_x_cisco_com_mld_apis.h"
#include "cosa_x_cisco_com_mld_internal.h"
#include "plugin_main_apis.h"
#include "safec_lib_common.h"

#if ( defined _COSA_SIM_ )

ANSC_STATUS
CosaDmlMldInit
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
CosaDmlMldGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCfg);
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlMldSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCfg);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_INFO          pInfo
    )    
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pInfo);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetGroup
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_MLD_GROUP         pMldGroupArray        
    )    
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pulCount);
    UNREFERENCED_PARAMETER(pMldGroupArray);
    return ANSC_STATUS_SUCCESS;
}

#elif defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)

ANSC_STATUS
CosaDmlMldInit
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
CosaDmlMldGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    FILE *pid_f, *fd;
    char buffer[256];
    pid_t pid;

    if (!pCfg)
    {
        return ANSC_STATUS_FAILURE;
    }

    pCfg->bSnoopingEnabled = FALSE;

    fd = fopen("/sys/class/net/brlan0/bridge/multicast_snooping", "r");

    if (fd)
    {
        if ( 1 == fread(buffer, 1, 1, fd) )
        {
            if ( buffer[0] == '1' )
            {
                pCfg->bSnoopingEnabled = TRUE;
            }
        }
        fclose(fd);        
    }

    if ( (pid_f = fopen("/var/run/ecmh.pid", "r")) != NULL )
    {
        if(fgets(buffer, sizeof(buffer) ,pid_f) != NULL)
        {
            fclose(pid_f);
            pid_f = NULL;
            sscanf( buffer, "%d", &pid);
            CcspTraceWarning(("Sending SIGUSR1 to proccess %d...\n", pid));
            if ( kill(pid,SIGUSR1) == -1 )
            {
                pCfg->bEnabled = FALSE;
                return ANSC_STATUS_SUCCESS;
            }
            else
            {
                pCfg->bEnabled = TRUE;
            }
        }
        if (pid_f != NULL)
        {
            fclose(pid_f);
        }
    }
    else
    {
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_CFG           pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (!pCfg)
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( pCfg->bSnoopingEnabled )
    {
        _write_sysctl_file("/sys/class/net/brlan0/bridge/multicast_snooping", 1);
    }
    else
    {
        _write_sysctl_file("/sys/class/net/brlan0/bridge/multicast_snooping", 0);
    }

#if !defined (_COSA_BCM_MIPS_) && !defined(_ENABLE_DSL_SUPPORT_)
    FILE *pid_f;
    char buffer[256];
    pid_t pid;

    if ( (pid_f = fopen("/var/run/ecmh.pid", "r")) != NULL )
    {
        if(fgets(buffer, sizeof(buffer), pid_f) != NULL)
        {
            fclose(pid_f);
            pid_f = NULL;
            sscanf( buffer, "%d", &pid);
            CcspTraceWarning(("Sending SIGUSR1 to proccess %d...\n", pid));
            if ( kill(pid,SIGUSR1) == -1 )
            {
                if ( pCfg->bEnabled == TRUE )
                {
                    system("/usr/sbin/ecmh -i wan0");
                }
            }
            else
            {
                if ( pCfg->bEnabled == FALSE )
                {
                    kill(pid,SIGKILL);
                }
            }
        }

        if(pid_f) /*RDKB-6846, CID-33252, CID-33248, Free unused resources before exit*/
            fclose(pid_f);

    }
    else
    {
        if ( pCfg->bEnabled == TRUE )
        {
            system("/usr/sbin/ecmh -i wan0");
        }
    }
#endif

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MLD_INFO          pInfo
    )
{
    UNREFERENCED_PARAMETER(hContext);
    FILE *pid_f;
    char buffer[256];
    pid_t pid;

    if (!pInfo)
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( (pid_f = fopen("/var/run/ecmh.pid", "r")) != NULL )
    {
        if(fgets(buffer, sizeof(buffer), pid_f) != NULL)
        {
            fclose(pid_f);
            pid_f = NULL;
            sscanf( buffer, "%d", &pid);
            CcspTraceWarning(("Sending SIGUSR1 to proccess %d...\n", pid));
            if ( kill(pid,SIGUSR1) == -1 )
            {
                pInfo->Status = COSA_DML_MLD_STATUS_Disabled;
            }
            else
            {
                pInfo->Status = COSA_DML_MLD_STATUS_Enabled;
            }
        }

        if(pid_f) /*RDKB-6846, CID-33365, free unused resources before exit*/
            fclose(pid_f);

    }
    else
    {
        pInfo->Status = COSA_DML_MLD_STATUS_Disabled;
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlMldGetGroup
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_MLD_GROUP         pMldGroupArray        
    )
{
    UNREFERENCED_PARAMETER(hContext);
    FILE *stat_f, *pid_f;
    char buffer[1024];
    pid_t pid;
    char *pch;
    long count;
    char *st = NULL;
    errno_t safec_rc = -1;

    if (!pulCount || !pMldGroupArray)
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( (pid_f = fopen("/var/run/ecmh.pid", "r")) != NULL )
    {
        if(fgets(buffer, sizeof(buffer), pid_f) != NULL)
        {
            fclose(pid_f);
            pid_f = NULL;
            sscanf( buffer, "%d", &pid);
            CcspTraceWarning(("Sending SIGUSR1 to proccess %d...\n", pid));
            if ( kill(pid,SIGUSR1) == -1 )
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        if(pid_f)
            fclose(pid_f); /*RDKB-6846, CID-33197, free unused resources before exit*/

    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    sleep(1);   /* waits until the stat file is generated */

    *pulCount = 0;
    count = -1;

    if ( (stat_f = fopen("/var/run/ecmh.dump", "r")) != NULL )
    {
        fgets(buffer, sizeof(buffer), stat_f); /* Ignore the first line */
        
        while(fgets(buffer, sizeof(buffer), stat_f) != NULL)
        {
			st = NULL;
            pch = strtok_r(buffer, " \t\r\n", &st);
            if (pch == NULL)
            {
                continue;
            }

            if (strcmp(pch, "Group") == 0)
            {
                pch = strtok_r(NULL, " \t\r\n", &st);
                while(pch)
                {
                    if (*pch != ':')
                    {
                        count++;
                        if (count >= MLD_MAXINSTANCE)
                        {
                            *pulCount = count;
                            fclose(stat_f); /*RDKB-6846, CID-33403, free unused resources before exit*/
                            return ANSC_STATUS_SUCCESS;
                        }
                        
                        safec_rc = strcpy_s(pMldGroupArray[count].GroupAddress, sizeof(pMldGroupArray[count].GroupAddress), pch);
                        if(safec_rc != EOK)
                        {
                            ERR_CHK(safec_rc);
                        }
                        pMldGroupArray[count].Interfaces[0] = '\0';

                        break;
                    }
                    pch = strtok_r(NULL, " \t\r\n", &st);
                }
            }
            else if (strcmp(pch, "Interface:") == 0)
            {
                pch =strtok_r(NULL, " \t\r\n", &st);
                if (pch)
                {
                    /*CID: 63836 - Calling risky function - Fix*/
                    strncat(pMldGroupArray[count].Interfaces, pch, sizeof(pMldGroupArray[count].Interfaces)-strlen(pMldGroupArray[count].Interfaces)-1);
                    strncat(pMldGroupArray[count].Interfaces, ",", sizeof(pMldGroupArray[count].Interfaces)-strlen(pMldGroupArray[count].Interfaces)-1);
                }
            }
            else if (strcmp(pch, "***") == 0)
            {
                break;
            }
        }

        fclose(stat_f);
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    *pulCount = count+1;
    return ANSC_STATUS_SUCCESS;
}

#endif
