#include <stdio.h>
#include <syscfg/syscfg.h>
#include "ccsp_trace.h"
#include "speedboost_apis.h"
#include "speedboost_scheduler_apis.h"
#include "speedboost_rbus_handler.h"
#include "cosa_drg_common.h"

pthread_mutex_t pvd_lock = PTHREAD_MUTEX_INITIALIZER;

static bool is_pvd_thread_running = false ;
//function to create a thread which runs only if there is any change in the TR181 parameters
//if there is a change in the TR181 parameters, it will run the sysevent set zebra-restart command

SpeedBoost sSpeedBoost;
SpeedBoostType boostType;

#define SYSCTL_PATH "/proc/sys/net/ipv4/ip_local_reserved_ports"
#define MAX_VAL_LEN 4096
#define MAX_PORT_RANGE_DIFF 2000

static int readSysctl(char *pBuf, size_t iBuflen)
{
    FILE *pFile = fopen(SYSCTL_PATH, "r");
    if (NULL == pFile)
        return -1;
    if (NULL == fgets(pBuf, iBuflen, pFile))
    {
        fclose(pFile);
        return -1;
    }
    pBuf[strcspn(pBuf, "\n")] = '\0';
    fclose(pFile);
    return 0;
}

static int writeSysctl(const char *pBuf)
{
    FILE *pFile = fopen(SYSCTL_PATH, "w");
    if (NULL == pFile)
        return -1;
    if (fputs(pBuf, pFile) == EOF)
    {
        fclose(pFile);
        return -1;
    }
    fclose(pFile);
    return 0;
}


void appendToLocalReservedPorts(const char * pPrevVal, char * pCurrentStartPort, char * pCurrentEndPort)
{
    char cLocalReservedPorts[MAX_VAL_LEN] = {0};
    char cTmp[MAX_VAL_LEN] = {0};
    char cOut[MAX_VAL_LEN] = {0};
    char cPrevRange[64] = {0};
    char cCurrntRange[64] = {0};
    char *pToken = NULL, *pSave = NULL;

    /* validate current range */
    if (NULL == pCurrentStartPort || NULL == pCurrentEndPort) {
        CcspTraceError(("%s:%d, NULL parameter passed\n", __FUNCTION__, __LINE__));
        return;
    }
    if (pCurrentStartPort[0] == '\0' || pCurrentEndPort[0] == '\0') {
        CcspTraceError(("%s:%d, Empty port range passed\n", __FUNCTION__, __LINE__));
        return;
    }

    /* read existing sysctl */
    if (0 != readSysctl(cLocalReservedPorts, sizeof(cLocalReservedPorts))) {
        CcspTraceError(("%s:%d, Failed to read %s\n", __FUNCTION__, __LINE__, SYSCTL_PATH));
        return;
    }

    CcspTraceInfo(("%s:%d, previous port range: %s\n", __FUNCTION__, __LINE__, pPrevVal));
    CcspTraceInfo(("%s:%d, current local reserved ports: %s\n", __FUNCTION__, __LINE__, cLocalReservedPorts));
    /* prepare current range */
    snprintf(cCurrntRange, sizeof(cCurrntRange), "%s-%s", pCurrentStartPort, pCurrentEndPort);

    /* extract IPv4 start/end from pPrevVal (format: "IPv4 PROTO START END,IPv6 ...") */
    if (pPrevVal && pPrevVal[0]) {
        char *pVal = strdup(pPrevVal);
        if (pVal) {
            char *pToken1 = NULL, *pSave1 = NULL;
            for (pToken1 = strtok_r(pVal, ",", &pSave1); pToken1 != NULL; pToken1 = strtok_r(NULL, ",", &pSave1)) {
                char *pToken = pToken1;
                while (*pToken == ' ' || *pToken == '\t') pToken++;
                char *pToken2 = NULL, *pSave2 = NULL;
                pToken2 = strtok_r(pToken, " ", &pSave2); /* "IPv4" or "IPv6" */
                if (pToken2 && strcasecmp(pToken2, "IPv4") == 0) {
                    /* skip proto */
                    (void)strtok_r(NULL, " ", &pSave2);
                    char *pStart = strtok_r(NULL, " ", &pSave2);
                    char *pEnd = strtok_r(NULL, " ", &pSave2);
                    if (pStart && pEnd) {
                        /* if prev range span > MAX_PORT_RANGE_DIFF, clamp prev end */
                        int iPrevStart = atoi(pStart);
                        int iPrevEnd = atoi(pEnd);
                        if (iPrevEnd - iPrevStart > MAX_PORT_RANGE_DIFF) {
                            iPrevEnd = iPrevStart + MAX_PORT_RANGE_DIFF;
                        }
                        snprintf(cPrevRange, sizeof(cPrevRange), "%d-%d", iPrevStart, iPrevEnd);
                        CcspTraceInfo(("%s:%d, extracted previous range: %s\n", __FUNCTION__, __LINE__, cPrevRange));
                    }
                    break;
                }
            }
            free(pVal);
        }
    }

    /* If prev and current are identical, nothing to do */
    if (cPrevRange[0] != '\0' && strcmp(cPrevRange, cCurrntRange) == 0) {
        CcspTraceInfo(("%s:%d, Prev range equals current range (%s) - no change\n", __FUNCTION__, __LINE__, cCurrntRange));
        return;
    }

    /* Flags to control behavior */
    bool bCurrentFound = false;

    /* If existing list non-empty, scan tokens.
       If prev_range present -> rebuild cOut skipping it.
       If prev_range not present -> keep full list in cOut. */
    if (cLocalReservedPorts[0] != '\0') {
        strncpy(cTmp, cLocalReservedPorts, sizeof(cTmp) - 1);
        for (pToken = strtok_r(cTmp, ",", &pSave); pToken != NULL; pToken = strtok_r(NULL, ",", &pSave)) {
            /* trim leading/trailing spaces */
            char *pStr = pToken;
            while (*pStr == ' ' || *pStr == '\t') pStr++;
            size_t iLen = strlen(pStr);
            while (iLen > 0 && (pStr[iLen - 1] == ' ' || pStr[iLen - 1] == '\t' || pStr[iLen - 1] == '\n' || pStr[iLen - 1] == '\r')) {
                pStr[--iLen] = '\0';
            }

            if (cPrevRange[0] != '\0' && strcmp(pStr, cPrevRange) == 0) {
                /* skip adding prev token */
                continue;
            }

            if (strcmp(pStr, cCurrntRange) == 0) {
                bCurrentFound = true;
            }

            if (cOut[0] != '\0') {
                strncat(cOut, ",", sizeof(cOut) - strlen(cOut) - 1);
            }
            strncat(cOut, pStr, sizeof(cOut) - strlen(cOut) - 1);
        }
    }

    /*
     Behavior required:
      - if prev_range found and prev != current: remove prev and append current (unless current already present)
      - if prev_range found and prev == current: do nothing (handled above)
      - if prev_range NOT found: append current (avoid duplicate)
    */

    /* If prev was not found we kept original list in cOut; if prev was found we rebuilt skipping it. */
    if (false == bCurrentFound) {
        /* Append current range */
        if (cOut[0] != '\0') {
            if (strlen(cOut) + 1 + strlen(cCurrntRange) + 1 < sizeof(cOut)) {
                strncat(cOut, ",", sizeof(cOut) - strlen(cOut) - 1);
                strncat(cOut, cCurrntRange, sizeof(cOut) - strlen(cOut) - 1);
            } else {
                CcspTraceError(("%s:%d, Resulting reserved ports string too long\n", __FUNCTION__, __LINE__));
                return;
            }
        } else {
            if (strlen(cCurrntRange) + 1 < sizeof(cOut)) {
                strncpy(cOut, cCurrntRange, sizeof(cOut) - 1);
            } else {
                CcspTraceError(("%s:%d, Resulting reserved ports string too long\n", __FUNCTION__, __LINE__));
                return;
            }
        }
    } else {
        CcspTraceInfo(("%s:%d, Current range %s already present; not appending\n", __FUNCTION__, __LINE__, cCurrntRange));
    }

    CcspTraceInfo(("%s:%d, Local reserved ports updated: %s\n", __FUNCTION__, __LINE__, cOut));

    if (0 != writeSysctl(cOut)) {
        CcspTraceError(("%s:%d, Failed to write %s\n", __FUNCTION__, __LINE__, SYSCTL_PATH));
    }
}

void trigger_ra_service_restart() 
{
        pthread_mutex_lock(&pvd_lock);
        if(is_pvd_thread_running)
        {
            CcspTraceInfo(("%s :The thread is already running\n",__FUNCTION__));
            pthread_mutex_unlock(&pvd_lock);
            return;
        }
        else
        {
            pthread_t thread_id;
            is_pvd_thread_running = true;
            pthread_create(&thread_id, NULL, monitor_thread_for_ra_restart, NULL);
        }
        pthread_mutex_unlock(&pvd_lock);
        return;
}

// if there is any change in the pvd tr181 run the zebra-restart command
void* monitor_thread_for_ra_restart(void* arg) 
{
    (void)arg;
    pthread_detach(pthread_self());
    sleep(WAIT_TIME_TO_TRIGGER_RA_RESTART);
    CcspTraceInfo(("%s The PVD parameters have changed, running the command\n",__FUNCTION__));
    if (0 != commonSyseventSet("zebra-restart", ""))
    {
        CcspTraceError(("%s :Failed to set zebra restart sysevent\n",__FUNCTION__));
    }    
    pthread_mutex_lock(&pvd_lock);
    is_pvd_thread_running = false;
    pthread_mutex_unlock(&pvd_lock);
    return NULL;
}

/************************************************************************************

    Get handler for pvd Bool

*************************************************************************************/
bool compareBuffer(const char* buf, bool* result) {
    if ( (strcmp(buf, "0") == 0 ) || ( strcmp(buf, "false") == 0 ) )
    {
        *result = false;
    } 
    else if ( (strcmp(buf, "1") == 0)  || ( strcmp(buf, "true") == 0 ) )
    {
        *result = true;
    } 
    return true;
}

void initializeSpeedBoostStructVal()
{
    char buf[256] = {0};
    bool result =false;

    char protoV4[256] = {0}, protoV6[256] = {0}, startPortV4[256] = {0}, startPortV6[256] = {0}, endPortV4[256] = {0}, endPortV6[256] = {0};
    char startPortNormalV4[254] = {0}, endPortNormalV4[256] = {0}, startPortNormalV6[256] = {0}, endPortNormalV6[256] = {0};
    memset(buf, 0, sizeof(buf));

    if(syscfg_get(NULL, "Advertisement_pvd_enable", buf, sizeof(buf)) == 0)
    {
        compareBuffer(buf, &result);
        sSpeedBoost.pvd_enabled = result;
    }
    memset(buf, 0, sizeof(buf));
    if(syscfg_get(NULL, "Advertisement_pvd_hflag", buf, sizeof(buf)) == 0)
    {
        compareBuffer(buf, &result);
        sSpeedBoost.pvd_h_flag = result;
    }
    memset(buf, 0, sizeof(buf));
    if(syscfg_get(NULL, "Advertisement_pvd_delay", buf, sizeof(buf)) == 0)
    {
        sSpeedBoost.pvd_delay = atoi(buf);
    }
    memset(buf, 0, sizeof(buf));

    if(syscfg_get(NULL, "Advertisement_pvd_seqNum", buf, sizeof(buf)) == 0)
    {
        sSpeedBoost.pvd_seqNum = atoi(buf);
    }
    memset(buf, 0, sizeof(buf));
    if(syscfg_get(NULL, "Advertisement_pvd_fqdn", buf, sizeof(buf)) == 0)
    {
        strncpy(sSpeedBoost.pvd_fqdn, buf, sizeof(sSpeedBoost.pvd_fqdn));
    }
    memset(buf, 0, sizeof(buf));
    if (syscfg_get(NULL, SYSCFG_SPEEDBOOST_NUM_OF_CONFIG_CLINETS, buf, sizeof(buf)) == 0)
    {
        sSpeedBoost.sClientInfoSpeedBoost.iNumOfConfigOfClients = atoi(buf);
    }
    memset(buf, 0, sizeof(buf));
    if (syscfg_get(NULL, SYSCFG_SPEEDBOOST_CLIENT_COUNT, buf, sizeof(buf)) == 0)
    {
        sSpeedBoost.sClientInfoSpeedBoost.iNumOfActiveClients = atoi(buf);
    }
    memset(buf, 0, sizeof(buf));
    for (int iCount = 1; iCount <= sSpeedBoost.sClientInfoSpeedBoost.iNumOfActiveClients; iCount++)
    {
        char syscfgName[64] = {0};
        snprintf(syscfgName, sizeof(syscfgName), "%s%d", SYSCFG_SPEEDBOOST_CLIENT_MAC_PREFIX, iCount);
        if (syscfg_get(NULL, syscfgName, buf, sizeof(buf)) == 0)
        {
            strncpy(sSpeedBoost.sClientInfoSpeedBoost.cCurrentDeviceList[iCount - 1], buf, sizeof(sSpeedBoost.sClientInfoSpeedBoost.cCurrentDeviceList[iCount - 1]));
        }
    }
    // To initialize the speedboost values
    if(syscfg_get(NULL, "SpeedBoost_ProtoV4", protoV4, sizeof(protoV4)) == 0 &&
       syscfg_get(NULL, "SpeedBoost_ProtoV6", protoV6, sizeof(protoV6)) == 0 &&
       syscfg_get(NULL, "SpeedBoost_Port_StartV4", startPortV4, sizeof(startPortV4)) == 0 &&
       syscfg_get(NULL, "SpeedBoost_Port_StartV6", startPortV6, sizeof(startPortV6)) == 0 &&
       syscfg_get(NULL, "SpeedBoost_Port_EndV4", endPortV4, sizeof(endPortV4)) == 0 &&
       syscfg_get(NULL, "SpeedBoost_Port_EndV6", endPortV6, sizeof(endPortV6)) == 0) 
    {
        snprintf(current_speedboost_value, sizeof(current_speedboost_value),
                 "IPv4 %s %s %s,IPv6 %s %s %s",
                 protoV4, startPortV4, endPortV4, protoV6, startPortV6, endPortV6);
        CcspTraceInfo(("current_speedboost_value: %s\n", current_speedboost_value));

        if (atoi(endPortV4) - atoi(startPortV4) <= MAX_PORT_RANGE_DIFF)
        {
            appendToLocalReservedPorts(NULL, startPortV4, endPortV4);
        }
        else
        {
            CcspTraceInfo(("%s:%d, Port range is more than 2000 ports \n", __FUNCTION__, __LINE__));
            char cEndPort[16] = {0};
            snprintf(cEndPort, sizeof(cEndPort), "%d", atoi(startPortV4) + MAX_PORT_RANGE_DIFF);
            appendToLocalReservedPorts(NULL, startPortV4, cEndPort);
        }
    }
    
    //initialise speedboost normal value
    if(syscfg_get(NULL, "SpeedBoost_ProtoV4", protoV4, sizeof(protoV4)) == 0 &&
       syscfg_get(NULL, "SpeedBoost_ProtoV6", protoV6, sizeof(protoV6)) == 0 &&
       syscfg_get(NULL, "NoSpeedBoost_Port_StartV4", startPortNormalV4, sizeof(startPortNormalV4)) == 0 &&
       syscfg_get(NULL, "NoSpeedBoost_Port_StartV6", startPortNormalV6, sizeof(startPortNormalV6)) == 0 &&
       syscfg_get(NULL, "NoSpeedBoost_Port_EndV4", endPortNormalV4, sizeof(endPortNormalV4)) == 0 &&
       syscfg_get(NULL, "NoSpeedBoost_Port_EndV6", endPortNormalV6, sizeof(endPortNormalV6)) == 0)
    {
    	snprintf(current_normal_speedboost_value, sizeof(current_normal_speedboost_value),
                 "IPv4 %s %s %s,IPv6 %s %s %s",
                 protoV4, startPortNormalV4, endPortNormalV4, protoV6, startPortNormalV6, endPortNormalV6);
    	CcspTraceInfo(("current_normal_speedboost_value: %s\n", current_normal_speedboost_value));
    }
}

int getNumOfConfiguredClients(void)
{
    CcspTraceInfo(("%s:%d, Number of configured clients: %d\n", __FUNCTION__, __LINE__, sSpeedBoost.sClientInfoSpeedBoost.iNumOfConfigOfClients));
    return sSpeedBoost.sClientInfoSpeedBoost.iNumOfConfigOfClients;
}
int getNumOfActiveClients(void)
{
    CcspTraceInfo(("%s:%d, Number of active clients: %d\n", __FUNCTION__, __LINE__, sSpeedBoost.sClientInfoSpeedBoost.iNumOfActiveClients));
    return sSpeedBoost.sClientInfoSpeedBoost.iNumOfActiveClients;
}
bool getActiveDevicesListOfSpeedBoost(char * pActiveDevicesList, int iLen)
{
    if (NULL == pActiveDevicesList)
    {
        CcspTraceError(("%s:%d, NULL parameter passed\n", __FUNCTION__, __LINE__));
        return false;
    }
    if (0 == sSpeedBoost.sClientInfoSpeedBoost.iNumOfActiveClients)
    {
        CcspTraceInfo(("%s:%d, No active devices\n", __FUNCTION__, __LINE__));
        return true;
    }
    if (iLen < sSpeedBoost.sClientInfoSpeedBoost.iNumOfActiveClients * MAC_ADDR_SIZE)
    {
        CcspTraceError(("%s:%d, Insufficient buffer\n", __FUNCTION__, __LINE__));
        return false;
    }
    for (int iCount = 0; iCount < sSpeedBoost.sClientInfoSpeedBoost.iNumOfActiveClients; iCount++)
    {
        //After copying mac address append the pipe symbol
        strncat(pActiveDevicesList, sSpeedBoost.sClientInfoSpeedBoost.cCurrentDeviceList[iCount], MAC_ADDR_SIZE);
        if ((iCount + 1) < sSpeedBoost.sClientInfoSpeedBoost.iNumOfActiveClients)
            strncat(pActiveDevicesList, ",", 1);
    }
    CcspTraceInfo(("%s:%d, Active devices list: %s\n", __FUNCTION__, __LINE__, pActiveDevicesList));
    return true;
}

int processSpeedBoostValue(const char* str, const char* prev_val, char* target_value, size_t target_size, SpeedBoostType boostType) 
{
    char* strCopy = strdup(str);
    strncpy(target_value, strCopy, target_size - 1);
    char *token, *subtoken, *saveptr1, *saveptr2;
    bool ipv4Valid = false;
    bool ipv6Valid = false;
    char* protocol_ipv4 = NULL;
    char* startPortStr_ipv4 = NULL;
    char* endPortStr_ipv4 = NULL;
    char* protocol_ipv6 = NULL;
    char* startPortStr_ipv6 = NULL;
    char* endPortStr_ipv6 = NULL;

    if(strchr(str, ',') == NULL) 
    {
    	CcspTraceError(("Input string does not contain a comma\n"));
    	strncpy(target_value, prev_val, target_size - 1);
	free(strCopy);
    	return 1;
    }

    for(token = strtok_r(strCopy, ",", &saveptr1); token != NULL; token = strtok_r(NULL, ",", &saveptr1))
    {
        subtoken = strtok_r(token, " ", &saveptr2);
        
        if (subtoken == NULL || (strcasecmp(subtoken, "IPv4") != 0 && strcasecmp(subtoken, "IPv6") != 0))
        {
            CcspTraceError(("IPv4 or IPV6 not found\n"));
            strncpy(target_value, prev_val, target_size - 1);
            free(strCopy);
            return 1;
        }

        char* protocol = strtok_r(NULL, " ", &saveptr2);
        if (protocol == NULL || (strcasecmp(protocol, "TCP") != 0 && strcasecmp(protocol, "UDP") != 0 && strcasecmp(protocol, "BOTH") != 0))
        {
            CcspTraceError(("TCP, UDP or BOTH not found\n"));
            strncpy(target_value, prev_val, target_size - 1);
            free(strCopy);
            return 1;
        }

        char* startPortStr = strtok_r(NULL, " ", &saveptr2);
        if (startPortStr == NULL)
        {
            CcspTraceError(("Start port not found\n"));
            strncpy(target_value, prev_val, target_size - 1);
            free(strCopy);
            return 1;
        }   
        int startPort = atoi(startPortStr);
        if (startPort < 0 || startPort > 65535)
        {
            CcspTraceError(("start port lt 0 or gt 65535\n"));
            strncpy(target_value, prev_val, target_size - 1);
            free(strCopy);
            return 1;
        }
        CcspTraceInfo(("startPortStr: %s, startPort: %d\n", startPortStr, startPort));

        char* endPortStr = strtok_r(NULL, " ", &saveptr2);
        if (endPortStr == NULL)
        {
            CcspTraceError(("End port not found\n"));
            strncpy(target_value, prev_val, target_size - 1);
            free(strCopy);
            return 1;
        }
        int endPort = atoi(endPortStr);
        if (endPort < startPort || endPort > 65535)
        {
            CcspTraceError(("End port lt start port or gt 65535\n"));
            strncpy(target_value, prev_val, target_size - 1);
            free(strCopy);
            return 1;
        }
        CcspTraceInfo(("endPortStr: %s, endPort: %d\n", endPortStr, endPort));
        
        // Update flags so that only if both ipv4 and ipv6 format is correct do syscfg_set_commit
        if (strcasecmp(subtoken, "IPv4") == 0) 
        {
            ipv4Valid = true;
            protocol_ipv4 = protocol;
            startPortStr_ipv4 = startPortStr;
            endPortStr_ipv4 = endPortStr;
        }
        else if (strcasecmp(subtoken, "IPv6") == 0) 
        {
            ipv6Valid = true;
            protocol_ipv6 = protocol;
            startPortStr_ipv6 = startPortStr;
            endPortStr_ipv6 = endPortStr;
        }
	
    }
    // Check if both IPv4 and IPv6 configurations are valid
    //outside the loop since if ipv4 format was correct and ipv6 foramt was wrong, it would still syscfg_set_commit for ipv4 which would be wrong.
    if (ipv4Valid && ipv6Valid) 
    {
      	// Perform commit actions for both IPv4 and IPv6
    	switch (boostType)
        {
            case CURRENT_SPEEDBOOST_VALUE:
	    	CcspTraceInfo(("syscfg_set_commit for current_speedboost_value\n"));
		CcspTraceInfo(("subtoken is IPv4\n"));
	        
                syscfg_set_commit(NULL, "SpeedBoost_ProtoV4", protocol_ipv4);
                syscfg_set_commit(NULL, "SpeedBoost_Port_StartV4", startPortStr_ipv4);
                syscfg_set_commit(NULL, "SpeedBoost_Port_EndV4", endPortStr_ipv4);
		
            if (atoi(endPortStr_ipv4) - atoi(startPortStr_ipv4) <= MAX_PORT_RANGE_DIFF)
            {
                appendToLocalReservedPorts(prev_val, startPortStr_ipv4, endPortStr_ipv4);
            }
            else
            {
                CcspTraceInfo(("%s:%d, Port range is more than 2000 ports \n", __FUNCTION__, __LINE__));
                char cEndPort[16] = {0};
                snprintf(cEndPort, sizeof(cEndPort), "%d", atoi(startPortStr_ipv4) + MAX_PORT_RANGE_DIFF);
                appendToLocalReservedPorts(prev_val, startPortStr_ipv4, cEndPort);
            }
                CcspTraceInfo(("subtoken is IPv6\n"));
	        
                syscfg_set_commit(NULL, "SpeedBoost_ProtoV6", protocol_ipv6);
	        syscfg_set_commit(NULL, "SpeedBoost_Port_StartV6", startPortStr_ipv6);
	        syscfg_set_commit(NULL, "SpeedBoost_Port_EndV6", endPortStr_ipv6);
            	break;
	    case CURRENT_NORMAL_SPEEDBOOST_VALUE:
	    	CcspTraceInfo(("syscfg_set_commit for current_normal_speedboost_value\n"));
                CcspTraceInfo(("subtoken is IPv4\n"));
                
                syscfg_set_commit(NULL, "NoSpeedBoost_Port_StartV4", startPortStr_ipv4);
                syscfg_set_commit(NULL, "NoSpeedBoost_Port_EndV4", endPortStr_ipv4);
            	
                CcspTraceInfo(("subtoken is IPv6\n"));
                
                syscfg_set_commit(NULL, "NoSpeedBoost_Port_StartV6", startPortStr_ipv6);
                syscfg_set_commit(NULL, "NoSpeedBoost_Port_EndV6", endPortStr_ipv6);
            	break;
            default:
            	printf("Unknown SpeedBoostType\n");
            	break;
	}
    }
    else
    {
	CcspTraceError(("IPv4 or IPv6 port range format is not correct\n"));
	strncpy(target_value, prev_val, target_size - 1);
	free(strCopy);
	return 1;
    }
    free(strCopy);

    //restart the firewall upon successful set of the speedboost port ranges
    return (triggerFirewallRestart() == false) ? 1 : 0;
}
