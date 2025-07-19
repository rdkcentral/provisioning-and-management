#include "cosa_managedwifi_dml.h"
#include "cosa_managedwifi_webconfig_apis.h"
#include "cosa_dhcpv6_apis.h"
#include "secure_wrapper.h"
#include "sysevent/sysevent.h"
#include "cosa_common_util.h"
#include "cosa_drg_common.h"

#ifdef CORE_NET_LIB
#include <libnet.h>
#else
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#if defined (AMENITIES_NETWORK_ENABLED)
#include "ccsp_trace.h"
#include "cosa_deviceinfo_apis_custom.h"
#include <unistd.h>

// Bitmask macros for Amenity Bridge flags
#define BIT_2G_ENABLE_MASK      (1 << 0)
#define BIT_5G_ENABLE_MASK      (1 << 1)
#define BIT_6G_ENABLE_MASK      (1 << 2)
#define BIT_2G_DISABLE_MASK     (1 << 3)
#define BIT_5G_DISABLE_MASK     (1 << 4)
#define BIT_6G_DISABLE_MASK     (1 << 5)
#define BIT_2G_DOWN_MASK        (1 << 6)
#define BIT_2G_UP_MASK          (1 << 7)
#define BIT_5G_DOWN_MASK        (1 << 8)
#define BIT_5G_UP_MASK          (1 << 9)
#define BIT_6G_DOWN_MASK        (1 << 10)
#define BIT_6G_UP_MASK          (1 << 11)

static char *l2netVlanID = "dmsb.l2net.%s.Vid";
static char *l2netVapName = "dmsb.l2net.%s.VapName";

amenityBridgeDetails_t sCurrAmenityBridgeDetails = {NULL, 0, false};
amenityBridgeDetails_t sBackupAmenityBridgeDetails = {NULL, 0, false};
amenityBridgeInfo_t *pRbusBridgeInfo = NULL;

VapMapping vapMappings[] = {
    {1, VAP_NAME_2G},
    {2, VAP_NAME_5G},
    {3, VAP_NAME_6G}
};

static const BridgeBandInfo bridgeBands[] = {
    { VAP_NAME_2G, BIT_2G_ENABLE_MASK, BIT_2G_DISABLE_MASK, BIT_2G_DOWN_MASK, BIT_2G_UP_MASK },
    { VAP_NAME_5G, BIT_5G_ENABLE_MASK, BIT_5G_DISABLE_MASK, BIT_5G_DOWN_MASK, BIT_5G_UP_MASK },
    { VAP_NAME_6G, BIT_6G_ENABLE_MASK, BIT_6G_DISABLE_MASK, BIT_6G_DOWN_MASK, BIT_6G_UP_MASK }
};


int iNumOfMappings = sizeof(vapMappings) / sizeof(vapMappings[0]);

static int iSyseventANFd = -1;
static token_t SyseventANToken;
pthread_t amenityMultinetThreadId;
pthread_mutex_t amenityWifi_exec = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t amenityWifi_exec_completed = PTHREAD_COND_INITIALIZER;
pthread_condattr_t amenityWifi_attr;
static BOOL bDisabledManagedWifi = FALSE;
#endif

#define RG_CM_COMMUNICATION_BEGIN "172.31"

extern rbusHandle_t handle;
extern char g_Subsystem[32];

static char *l2netBridgeEnable = "dmsb.l2net.%s.Enable";
static char *l2netBridgeName = "dmsb.l2net.%s.Name";
static char *l2netAlias = "dmsb.l2net.%s.Alias";
static char *l3netV4Addr = "dmsb.l3net.%s.V4Addr";
static char *l2netV4SubnetMask = "dmsb.l3net.%s.V4SubnetMask";
static char *l3netDhcpV4PoolSubnetMask =  "dmsb.dhcpv4.server.pool.%s.SubnetMask";
static char *l3netDhcpV4PoolEnable = "dmsb.dhcpv4.server.pool.%s.Enable";
static char *l3netDhcpV4PoolMinAddr = "dmsb.dhcpv4.server.pool.%s.MinAddress";
static char *l3netDhcpV4PoolMaxAddr = "dmsb.dhcpv4.server.pool.%s.MaxAddress";
static char *l3netDhcpV4PoolLeaseTime = "dmsb.dhcpv4.server.pool.%s.LeaseTime";
static char *l3netIPv6Enable = "dmsb.l3net.%s.IPv6Enable";
static char *l2netWiFiMembers = "dmsb.l2net.%s.Members.OneWiFi";

static ManageWiFiInfo_t sManageWiFiInfo = {{'\0'},{'\0'},{'\0'},{'\0'}, OTHER_UPDATE};
static backupLanconfig_t sBackupLanConfig = {false,{'\0'},false,{'\0'},{'\0'},{'\0'},{'\0'}, {'\0'},false, {'\0'}};
static backupLanconfig_t sCurrentLanConfig = {false,{'\0'},false,{'\0'},{'\0'},{'\0'},{'\0'}, {'\0'},false, {'\0'}};
char * pVapNames[BUFF_LEN_8];
/* static int iRestoreFlag = 0
 * 0th bit : return value 1 mean failure, 0 means success
 * 1st bit : Manage Wifi Enable
 * 2nd bit : Lan IP addr
 * 3rd bit : lan subnet mask
 * 4th bit : dhcp V4 pool subnet mask
 * 5th bit : DhcpV4Pool Enable
 * 6th bit : DhcpV4Pool Start
 * 7th bit : DhcpV4Pool End
 * 8th bit : DhcpV4Pool lease time
 * 9th bit : IPv6 Enable
 *10th bit : alias
*/
static int iRestoreFlag = 0;
pthread_mutex_t manageWifi_exec = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t manageWifi_exec_completed = PTHREAD_COND_INITIALIZER;
pthread_condattr_t manageWifi_attr;

int confirmManageWifiVap(void)
{
    rbusValue_t value;
    int ret = RBUS_ERROR_SUCCESS;
    char *pStrVal = NULL;
    char *saveptr = NULL;
    char aWiFiInterfaces[BUFF_LEN_64] = {'\0'};
    char aWiFiInterfacesPsmStr[BUFF_LEN_64] = {'\0'};
    char aBr106LnfInterfaces[BUFF_LEN_64] = {'\0'};
    char aParamVal[BUFF_LEN_64] = {'\0'};
    int len = 0, lenBr106Str = 0;

    CcspTraceInfo(("%s:%d, Entered \n",__FUNCTION__,__LINE__));
    if (NULL == handle)
    {
        CcspTraceError(("%s:%d, R-Bus handle is NULL\n",__FUNCTION__,__LINE__));
        return -1;
    }
    /* Init rbus variable */
    rbusValue_Init(&value);
    ret = rbus_get(handle,MANAGE_WIFI_INTERFACES, &value);

    if(ret != RBUS_ERROR_SUCCESS )
    {
        CcspTraceError(("%s:%d, Failed to do rbus_get\n",__FUNCTION__,__LINE__));
        return -1;
    }

    pStrVal = rbusValue_ToString(value, NULL, 0);
    CcspTraceInfo(("%s:%d, pStrVal:%s\n",__FUNCTION__,__LINE__,pStrVal));
    if (NULL != pStrVal)
    {
        snprintf (aWiFiInterfacesPsmStr, BUFF_LEN_64,l2netWiFiMembers, BR106_PSM_INDEX);
        CcspTraceInfo(("%s:%d, aWiFiInterfacesPsmStr:%s\n",__FUNCTION__,__LINE__,aWiFiInterfacesPsmStr));
        psmGet (aWiFiInterfacesPsmStr, aBr106LnfInterfaces, BUFF_LEN_64);
        CcspTraceInfo(("%s:%d, br106 Wifi interfaces:%s\n",__FUNCTION__,__LINE__,aBr106LnfInterfaces));
        CcspTraceInfo(("%s:%d, pStrVal:%s\n",__FUNCTION__,__LINE__,pStrVal));
        char * pKeyVal = strtok(pStrVal, ":");
        CcspTraceInfo(("%s:%d, pKeyVal:%s\n",__FUNCTION__,__LINE__,pKeyVal));
        pKeyVal = strtok(NULL, ":");
        CcspTraceInfo(("%s:%d, pKeyVal:%s\n",__FUNCTION__,__LINE__,pKeyVal));

        if (NULL != pKeyVal)
            pKeyVal = strtok_r(pKeyVal, ",", &saveptr);
        static int iCount = -1;
        int iSubStrLen = 0;
        while (pKeyVal != NULL)
        {
            CcspTraceInfo(("%s:%d, pKeyVal:%s\n",__FUNCTION__,__LINE__,pKeyVal));
            snprintf(aWiFiInterfaces + len, BUFF_LEN_64 -len," %s",pKeyVal);
            len = strlen(aWiFiInterfaces);
            if (true == sCurrentLanConfig.bMwEnable)
            {
                if(!removeSubstring(aBr106LnfInterfaces, pKeyVal, true))
                {
                    iSubStrLen = strlen(pKeyVal);
                    iCount++;
                    pVapNames[iCount] = (char*) malloc(iSubStrLen+1);
                    if(NULL != pVapNames[iCount])
                    {
                        strncpy(pVapNames[iCount], pKeyVal, iSubStrLen);
                    }
                    else
                        iCount--;
                }
            }
            else if (false == sCurrentLanConfig.bMwEnable)
            {
                lenBr106Str = strlen(aBr106LnfInterfaces);
                int iVar = 0;
                for (iVar = 0; iVar <= iCount; iVar++)
                {
                    if ((NULL != pVapNames[iVar]) && (!strcmp(pKeyVal,pVapNames[iVar])))
                    {
                        snprintf(aBr106LnfInterfaces + lenBr106Str, BUFF_LEN_64 - lenBr106Str, " %s", pKeyVal);
                        free(pVapNames[iVar]);
                        pVapNames[iVar] = NULL;
                        break;
                    }
                }
                if (iCount == iVar)
                    iCount = -1;
            }
            pKeyVal = strtok_r(NULL, ",", &saveptr);
        }

        if((true == sCurrentLanConfig.bMwEnable) && ('\0' != aWiFiInterfaces[0]))
        {
            snprintf(aWiFiInterfacesPsmStr, BUFF_LEN_64, l2netWiFiMembers, sManageWiFiInfo.aBridgeIndex);
            CcspTraceInfo(("%s:%d, aWiFiInterfacesPsmStr:%s\n",__FUNCTION__,__LINE__,aWiFiInterfacesPsmStr));
            CcspTraceInfo(("%s:%d, aWiFiInterfaces:%s\n",__FUNCTION__,__LINE__,aWiFiInterfaces));
            if (0 == psmSet(aWiFiInterfacesPsmStr,aWiFiInterfaces))
                strncpy(sBackupLanConfig.aWiFiInterfaces, aWiFiInterfaces, (sizeof(sBackupLanConfig.aWiFiInterfaces)-1));
            snprintf (aWiFiInterfacesPsmStr, BUFF_LEN_64,l2netWiFiMembers, BR106_PSM_INDEX);
            CcspTraceInfo(("%s:%d, aWiFiInterfacesPsmStr:%s\n",__FUNCTION__,__LINE__,aWiFiInterfacesPsmStr));
            CcspTraceInfo(("%s:%d, aBr106LnfInterfaces:%s\n",__FUNCTION__,__LINE__,aBr106LnfInterfaces));
            psmSet(aWiFiInterfacesPsmStr,aBr106LnfInterfaces);
        }
        else if ((false == sCurrentLanConfig.bMwEnable) && ('\0' != aWiFiInterfaces[0]))
        {
            snprintf (aWiFiInterfacesPsmStr, BUFF_LEN_64,l2netWiFiMembers, BR106_PSM_INDEX);
            CcspTraceInfo(("%s:%d, aWiFiInterfacesPsmStr:%s\n",__FUNCTION__,__LINE__,aWiFiInterfacesPsmStr));
            CcspTraceInfo(("%s:%d, aBr106LnfInterfaces:%s\n",__FUNCTION__,__LINE__,aBr106LnfInterfaces));
            psmSet(aWiFiInterfacesPsmStr,aBr106LnfInterfaces);
            snprintf(aWiFiInterfacesPsmStr, BUFF_LEN_64, l2netWiFiMembers, sManageWiFiInfo.aBridgeIndex);
            CcspTraceInfo(("%s:%d, aWiFiInterfacesPsmStr:%s\n",__FUNCTION__,__LINE__,aWiFiInterfacesPsmStr));
            memset(aWiFiInterfaces,0, BUFF_LEN_64);
            CcspTraceInfo(("%s:%d, aWiFiInterfaces:%s\n",__FUNCTION__,__LINE__,aWiFiInterfaces));
            if (0 == psmSet(aWiFiInterfacesPsmStr,aWiFiInterfaces))
                strncpy(sBackupLanConfig.aWiFiInterfaces, aWiFiInterfaces, (sizeof(sBackupLanConfig.aWiFiInterfaces)-1));
        }
        if (sBackupLanConfig.bMwEnable != sCurrentLanConfig.bMwEnable)
        {
            if (true == sCurrentLanConfig.bMwEnable)
                snprintf(aParamVal, BUFF_LEN_64,"true");
            else if (false == sCurrentLanConfig.bMwEnable)
            {
                snprintf(aParamVal, BUFF_LEN_64,"false");
                memset (&sCurrentLanConfig, 0, sizeof(backupLanconfig_t));
                sCurrentLanConfig.bMwEnable = false;
                sCurrentLanConfig.bDhcpServerEnable = false;
                sCurrentLanConfig.bIpv6Enable = false;
            }
            publishEvent(MANAGE_WIFI_ENABLE,aParamVal, RBUS);
        }
        updateBackupConfig();

    }
    else
        CcspTraceError(("%s:%d, pStrVal returned NULL by R-Bus\n",__FUNCTION__,__LINE__));

    /* release rbus variable */
    rbusValue_Release(value);
    free(pStrVal);
    return 0;
}

#if defined (AMENITIES_NETWORK_ENABLED)

void setRbus(char *pParamName, char *pParamVal)
{
    CcspTraceInfo(("%s:%d, Entered \n",__FUNCTION__,__LINE__));
    if (NULL == pParamName || NULL == pParamVal)
    {
        CcspTraceError(("%s:%d, pParamName or pParamVal is NULL\n",__FUNCTION__,__LINE__));
        return;
    }
    if (NULL == handle)
    {
        CcspTraceError(("%s:%d, R-Bus handle is NULL\n",__FUNCTION__,__LINE__));
        return;
    }
    CcspTraceInfo(("%s:%d, pParamName:%s\n",__FUNCTION__,__LINE__,pParamName));
    CcspTraceInfo(("%s:%d, pParamVal:%s\n",__FUNCTION__,__LINE__,pParamVal));

    rbusValue_t rbusValue;
    int iRbusRetVal = RBUS_ERROR_SUCCESS;

    /* Init rbus variable */
    rbusValue_Init(&rbusValue);
    rbusValue_SetString(rbusValue, pParamVal);
    iRbusRetVal = rbus_set(handle, pParamName, rbusValue, NULL);
    if(iRbusRetVal != RBUS_ERROR_SUCCESS )
    {
        CcspTraceError(("%s:%d, Failed to do rbus_set\n",__FUNCTION__,__LINE__));
        return;
    }
    /* release rbus variable */
    rbusValue_Release(rbusValue);
}

int getAmenityRbusData(amenityBridgeInfo_t ** ppBridgeRbusData, int *pVapCount)
{
    if (NULL == ppBridgeRbusData || NULL == pVapCount)
    {
        CcspTraceError(("%s:%d, ppBridgeRbusData or pVapCount is NULL\n",__FUNCTION__,__LINE__));
        return -1;
    }
    *ppBridgeRbusData = pRbusBridgeInfo;
    if (sCurrAmenityBridgeDetails.iBridgeCount > 0)
    {
        *pVapCount = sCurrAmenityBridgeDetails.iBridgeCount;
    }
    else
    {
        *pVapCount = sBackupAmenityBridgeDetails.iBridgeCount;
    }
    CcspTraceInfo(("%s:%d,pVapCount:%d\n",__FUNCTION__,__LINE__,*pVapCount));
    CcspTraceInfo(("%s:%d,ppBridgeRbusData:%p\n",__FUNCTION__,__LINE__,*ppBridgeRbusData));
    CcspTraceInfo(("%s:%d,pRbusBridgeInfo:%p\n",__FUNCTION__,__LINE__,pRbusBridgeInfo));
    return 0;
}
int confirmAmentitiesNetworkVap(void)
{
    rbusValue_t rbusValue;
    int iRbusRetVal = RBUS_ERROR_SUCCESS;
    char cParamName [BUFF_LEN_64] = {'\0'};
    char *pStrVal = NULL;
    int iRetVal = 0;
    static bool bIs1stAmenityExecuted = false;
    CcspTraceInfo(("%s:%d, Entered \n",__FUNCTION__,__LINE__));
    if (true == sCurrAmenityBridgeDetails.bIsAmenityEnabled)
    {
        if (NULL == handle)
        {
            CcspTraceError(("%s:%d, R-Bus handle is NULL\n",__FUNCTION__,__LINE__));
            return -1;
        }

        for (int iCount = 0; iCount < sCurrAmenityBridgeDetails.iBridgeCount; iCount++)
        {
            /* Init rbus variable */
            rbusValue_Init(&rbusValue);

            snprintf(cParamName, BUFF_LEN_64, "%s%d.%s", LAN_BRIDGES_TABLE, (iCount + 1), WIFI_IF_STR);
            CcspTraceInfo(("%s:%d, cParamName:%s\n",__FUNCTION__,__LINE__,cParamName));
            iRbusRetVal = rbus_get(handle,cParamName, &rbusValue);
            if(iRbusRetVal != RBUS_ERROR_SUCCESS )
            {
                CcspTraceError(("%s:%d, Failed to do rbus_get\n",__FUNCTION__,__LINE__));
                rbusValue_Release(rbusValue);
                return -1;
            }

            pStrVal = rbusValue_ToString(rbusValue, NULL, 0);
            CcspTraceInfo(("%s:%d, pStrVal:%s\n",__FUNCTION__,__LINE__,pStrVal));
            if (NULL == pStrVal)
            {
                CcspTraceError(("%s:%d, pStrVal returned NULL by R-Bus\n",__FUNCTION__,__LINE__));
                rbusValue_Release(rbusValue);
                return -1;
            }
            if(!strncmp(pRbusBridgeInfo[iCount].cVapName, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName, sizeof(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName)))
            {
                CcspTraceInfo(("%s:%d, VAP name is same\n",__FUNCTION__,__LINE__));
                snprintf(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cWiFiInterface, sizeof(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cWiFiInterface), "%s", pStrVal);
                CcspTraceInfo(("%s:%d, sCurrAmenityBridgeDetails.pBridgeInfoStruct[%d].cWiFiInterface:%s\n",__FUNCTION__,__LINE__,iCount, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cWiFiInterface));
                snprintf(cParamName, BUFF_LEN_64, l2netWiFiMembers, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex);
                CcspTraceInfo(("%s:%d, cParamName:%s\n",__FUNCTION__,__LINE__,cParamName));
                if (0 != psmSet(cParamName, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cWiFiInterface))
                {
                    CcspTraceError(("%s:%d, psmSet failed for %s\n",__FUNCTION__,__LINE__,cParamName));
                    iRetVal = -1;
                }
            }
            else
            {
                CcspTraceError(("%s:%d, VAP name is different\n",__FUNCTION__,__LINE__));
            }
            /* release rbus variable */
            rbusValue_Release(rbusValue);
            free(pStrVal);
            pStrVal = NULL;
        }
    }
    if (0 == iRetVal)
    {
        if (NULL == sBackupAmenityBridgeDetails.pBridgeInfoStruct)
        {
            CcspTraceInfo(("%s:%d, Initialize Backup config as the Amenity wifi is enbled for the first timee \n",__FUNCTION__,__LINE__));
            sBackupAmenityBridgeDetails.pBridgeInfoStruct = (amenityBridgeInfo_t *)malloc(sizeof(amenityBridgeInfo_t) * sCurrAmenityBridgeDetails.iBridgeCount);
            if (NULL == sBackupAmenityBridgeDetails.pBridgeInfoStruct)
            {
                CcspTraceError(("%s:%d, Failed to allocate memory for back up config bridgeInfoStruct\n",__FUNCTION__,__LINE__));
                return -1;
            }
        }
        if (false == sCurrAmenityBridgeDetails.bIsAmenityEnabled)
        {
            CcspTraceInfo(("%s:%d, clearing the psm entry\n",__FUNCTION__,__LINE__));
            // Clear PSM records for all amenity bridges
            for (int iIndex = 0; iIndex < sBackupAmenityBridgeDetails.iBridgeCount; iIndex++)
            {
                lanTunnelPsmClear(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iIndex].cBridgeIndex);
            }
            memset(sBackupAmenityBridgeDetails.pBridgeInfoStruct, 0, sizeof(amenityBridgeInfo_t) * sBackupAmenityBridgeDetails.iBridgeCount);
        }
        memset(sBackupAmenityBridgeDetails.pBridgeInfoStruct, 0, sizeof(amenityBridgeInfo_t) * sCurrAmenityBridgeDetails.iBridgeCount);
        sBackupAmenityBridgeDetails.iBridgeCount = sCurrAmenityBridgeDetails.iBridgeCount;
        sBackupAmenityBridgeDetails.bIsAmenityEnabled = sCurrAmenityBridgeDetails.bIsAmenityEnabled;
        CcspTraceInfo(("%s:%d, Moving currentAmenity network into backup config \n",__FUNCTION__,__LINE__));
        CcspTraceInfo(("%s:%d, sCurrAmenityBridgeDetails.iBridgeCount = %d  \n",__FUNCTION__,__LINE__, sCurrAmenityBridgeDetails.iBridgeCount));
        for (int iCount = 0; iCount < sCurrAmenityBridgeDetails.iBridgeCount; iCount++)
        {
            memcpy(&sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount], &sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount], sizeof(amenityBridgeInfo_t));
        }
        if (TRUE == bDisabledManagedWifi)
        {
            bDisabledManagedWifi = FALSE;
            if (0 != commonSyseventSet("zebra-restart", ""))
                CcspTraceError(("%s:%d, commonSyseventSet failed for zebra-restart\n",__FUNCTION__,__LINE__));
            if (0 != commonSyseventSet("dhcp_server-restart", ""))
                CcspTraceError(("%s:%d, commonSyseventSet failed for dhcp_server-restart\n",__FUNCTION__,__LINE__));

        }
        if (iRetVal == 0 )
        {
            if (0 != commonSyseventSet("firewall-restart", ""))
                CcspTraceError(("%s:%d, commonSyseventSet failed for firewall-restart\n",__FUNCTION__,__LINE__));
        }
        if (false == bIs1stAmenityExecuted && true == sCurrAmenityBridgeDetails.bIsAmenityEnabled)
        {
            // Is_Amenity_Received will set when first Amenity blob is executed after that it will be never unset until downgrade the image
            syscfg_set_commit(NULL, "Is_Amenity_Received", "true");
            bIs1stAmenityExecuted = true;
        }

        if (true == sCurrAmenityBridgeDetails.bIsAmenityEnabled)
        {
            syscfg_set_commit(NULL, "Amenity_Network_Enabled", "true");
        }
        else if (false == sCurrAmenityBridgeDetails.bIsAmenityEnabled)
        {
            syscfg_set_commit(NULL, "Amenity_Network_Enabled", "false");
        }
        syscfg_set_u_commit(NULL, "Amenity_Bridge_Count", sCurrAmenityBridgeDetails.iBridgeCount);

        CcspTraceInfo(("%s:%d, Successfully confirmed Amenity Network VAP\n",__FUNCTION__,__LINE__));
    }

    return iRetVal;
}
static void getMultinetAction(const char* pVapName, uint16_t ui16Flag, char *pAction, int iActionSize)
{
    if (!strncmp(pVapName, VAP_NAME_2G, sizeof(VAP_NAME_2G)))
    {
        if ((ui16Flag & BIT_2G_DOWN_MASK) || (ui16Flag & BIT_2G_DISABLE_MASK))
            snprintf (pAction, iActionSize,"multinet-down");
        else if (ui16Flag & BIT_2G_ENABLE_MASK)
            snprintf(pAction, iActionSize, "multinet-up");
    }
    else if (!strncmp(pVapName, VAP_NAME_5G, sizeof(VAP_NAME_5G)))
    {
        if ((ui16Flag & BIT_5G_DOWN_MASK) || (ui16Flag & BIT_5G_DISABLE_MASK))
            snprintf (pAction, iActionSize,"multinet-down");
        else if (ui16Flag & BIT_5G_ENABLE_MASK)
            snprintf(pAction, iActionSize, "multinet-up");
    }
    else if (!strncmp(pVapName, VAP_NAME_6G, sizeof(VAP_NAME_6G)))
    {
        if ((ui16Flag & BIT_6G_DOWN_MASK) || (ui16Flag & BIT_6G_DISABLE_MASK))
            snprintf (pAction, iActionSize,"multinet-down");
        else if (ui16Flag & BIT_6G_ENABLE_MASK)
            snprintf(pAction, iActionSize, "multinet-up");
    }
    return;
}
int processTunnelInfo(amenityBridgeDetails_t * pCurrBrInfo, uint16_t ui16Flag, pErr pErrVar)
{
    struct timespec abs_time = {0};
    if ((NULL == pCurrBrInfo) || (NULL == pErrVar))
    {
        CcspTraceError(("%s:%d, pCurrBrInfo or pErrVar is NULL\n", __FUNCTION__, __LINE__));
        snprintf(pErrVar->ErrorMsg, BUFF_LEN_128, "pCurrBrInfo or pErrVar is NULL\n");
        pErrVar->ErrorCode = BLOB_EXEC_FAILURE;
        return -1;
    }
    if (NULL == pCurrBrInfo->pBridgeInfoStruct)
    {
        CcspTraceError(("%s:%d, pBridgeInfoStruct is NULL\n", __FUNCTION__, __LINE__));
        snprintf(pErrVar->ErrorMsg, BUFF_LEN_128, "pBridgeInfoStruct is NULL\n");
        pErrVar->ErrorCode = BLOB_EXEC_FAILURE;
        return -1;
    }

    pthread_condattr_init(&amenityWifi_attr);
    pthread_condattr_setclock(&amenityWifi_attr, CLOCK_MONOTONIC);
    pthread_cond_init(&amenityWifi_exec_completed, &amenityWifi_attr);

    AmenityThread_t sAmenityThread = { &pCurrBrInfo, ui16Flag };
    CcspTraceInfo(("%s:%d, creating amenityMultinetNotifyHandler 3 \n", __FUNCTION__, __LINE__));
    int iPthreadRetVal = pthread_create(&amenityMultinetThreadId, NULL, &amenityMultinetNotifyHandler, (void*)&sAmenityThread);
    if (iPthreadRetVal != 0)
    {
        CcspTraceError(("%s:%d, Failed to create pthread\n", __FUNCTION__, __LINE__));
        return -1;
    }

    for (int iCount = 0; iCount < pCurrBrInfo->iBridgeCount; iCount++)
    {
        amenityBridgeInfo_t *pBridge = &pCurrBrInfo->pBridgeInfoStruct[iCount];
        if (NULL == pBridge->cBridgeName)
        {
            CcspTraceError(("%s:%d, cBridgeName is NULL\n", __FUNCTION__, __LINE__));
            snprintf(pErrVar->ErrorMsg, BUFF_LEN_128, "cBridgeName is NULL\n");
            pErrVar->ErrorCode = BLOB_EXEC_FAILURE;
            return -1;
        }
        char cAction [16] = {0};
        getMultinetAction(pBridge->cVapName, ui16Flag, cAction, sizeof(cAction));
        if (strlen(cAction) > 0)
        {
            CcspTraceInfo(("%s:%d, cBridgeIndex:%s, VapName:%s, Action:%s\n", __FUNCTION__, __LINE__, pBridge->cBridgeIndex, pBridge->cVapName, cAction));
            if (0 != commonSyseventSet(cAction, pBridge->cBridgeIndex))
            {
                CcspTraceError(("%s:%d, commonSyseventSet failed for %s\n", __FUNCTION__, __LINE__, pBridge->cBridgeIndex));
                snprintf(pErrVar->ErrorMsg, BUFF_LEN_128, "commonSyseventSet failed for %s\n", pBridge->cBridgeIndex);
                pErrVar->ErrorCode = BLOB_EXEC_FAILURE;
                return -1;
            }
        }
    }
    if (0 == iPthreadRetVal)
    {
        clock_gettime(CLOCK_MONOTONIC, &abs_time);
        abs_time.tv_sec += 30; //wait 30sec
        abs_time.tv_nsec += 0;
        pthread_mutex_lock(&amenityWifi_exec);
       // Wait few sec for amenityMultinetNotifyHandler thread to complete
        int iErr = pthread_cond_timedwait(&amenityWifi_exec_completed, &amenityWifi_exec, &abs_time);
        if (iErr == ETIMEDOUT)
        {
            CcspTraceInfo(("%s:%d, Timedout Cancelling the Amenity multinet thread\n",__FUNCTION__,__LINE__));
            pthread_cancel(amenityMultinetThreadId);
        }
        pthread_mutex_unlock(&amenityWifi_exec);
    }
    else
    {
        CcspTraceInfo(("%s:%d, amenityMultinetNotifyHandler thread completed successfully\n", __FUNCTION__, __LINE__));
    }

    updateFirewallSyscfg(pCurrBrInfo->iBridgeCount, pCurrBrInfo->bIsAmenityEnabled);

    snprintf(pErrVar->ErrorMsg, BUFF_LEN_128,"Success\n");
    pErrVar->ErrorCode = BLOB_EXEC_SUCCESS;
    CcspTraceInfo(("%s:%d, Success \n", __FUNCTION__, __LINE__));
    return 0;
}

static void setBridgeUpFlag(const char *pVapName, uint16_t *pFlag)
{
    if (!strncmp(pVapName, VAP_NAME_2G, sizeof(VAP_NAME_2G)))
        *pFlag |= BIT_2G_ENABLE_MASK;
    else if (!strncmp(pVapName, VAP_NAME_5G, sizeof(VAP_NAME_5G)))
        *pFlag |= BIT_5G_ENABLE_MASK;
    else if (!strncmp(pVapName, VAP_NAME_6G, sizeof(VAP_NAME_6G)))
        *pFlag |= BIT_6G_ENABLE_MASK;
}

void initializeAmenityBridges(void)
{
    char cAmenityBridgeCount[BUFF_LEN_8] = {0};
    char cAmenityEnabled[BUFF_LEN_8] = {0};
    pErr pErrRetVal = (pErr)malloc(sizeof(Err));
    if (!pErrRetVal)
    {
        CcspTraceError(("%s: Failed to allocate memory for pErrRetVal\n", __FUNCTION__));
        return;
    }
    memset(pErrRetVal, 0, sizeof(Err));
    pErrRetVal->ErrorCode = BLOB_EXEC_SUCCESS;

    //To read the Manage WiFi bridge Name
    getBridgeDetailsFromPsm();

    //Write these values to syscfg after confirmVap executed successfully
    syscfg_get(NULL, "Amenity_Bridge_Count", cAmenityBridgeCount, BUFF_LEN_8);

    syscfg_get(NULL, "Amenity_Network_Enabled", cAmenityEnabled, BUFF_LEN_8);

    sBackupAmenityBridgeDetails.bIsAmenityEnabled = (!strncmp(cAmenityEnabled, "true", 4)) ? true : false;
    int iBridgeCount = atoi(cAmenityBridgeCount);

    if ((iBridgeCount > 0) && (0 != readTunnelDetailsFromPsm(iBridgeCount)))
    {
        CcspTraceError(("%s:%d, Failed to read Tunnel details from PSM\n", __FUNCTION__, __LINE__));
        free(pErrRetVal);
        return;
    }
    sBackupAmenityBridgeDetails.iBridgeCount = iBridgeCount;
    if (NULL == pRbusBridgeInfo)
    {
        pRbusBridgeInfo = (amenityBridgeInfo_t *)malloc(iBridgeCount * sizeof(amenityBridgeInfo_t));
        if (NULL == pRbusBridgeInfo)
        {
            CcspTraceError(("%s:%d, pRbusBridgeInfo malloc failed\n", __FUNCTION__, __LINE__));
            free(pErrRetVal);
            return;
        }
        memset(pRbusBridgeInfo, 0, iBridgeCount * sizeof(amenityBridgeInfo_t));
    }

    // Populate pRbusBridgeInfo from backup details
    for (int iCount= 0; iCount < iNumOfMappings; iCount++)
    {
        const char *pVapName = vapMappings[iCount].pVapName;
        for (int iVar = 0; iVar < sBackupAmenityBridgeDetails.iBridgeCount; iVar++)
        {
            if (!strncmp(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName, pVapName, sizeof(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName)))
            {

                snprintf(pRbusBridgeInfo[iCount].cBridgeIndex, sizeof(pRbusBridgeInfo[iCount].cBridgeIndex), "%s", sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex);
                snprintf(pRbusBridgeInfo[iCount].cVapName, sizeof(pRbusBridgeInfo[iCount].cVapName), "%s", sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName);
                snprintf(pRbusBridgeInfo[iCount].cBridgeName, sizeof(pRbusBridgeInfo[iCount].cBridgeName), "%s", sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].cBridgeName);
                snprintf(pRbusBridgeInfo[iCount].cWiFiInterface, sizeof(pRbusBridgeInfo[iCount].cWiFiInterface), "%s", sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].cWiFiInterface);
                pRbusBridgeInfo[iCount].iVlanId = sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].iVlanId;
                CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d]: idx=%s, vap=%s, br=%s, wifi=%s, vlan=%d\n",
                    __FUNCTION__, __LINE__, iCount,
                    pRbusBridgeInfo[iCount].cBridgeIndex,
                    pRbusBridgeInfo[iCount].cVapName,
                    pRbusBridgeInfo[iCount].cBridgeName,
                    pRbusBridgeInfo[iCount].cWiFiInterface,
                    pRbusBridgeInfo[iCount].iVlanId));
                break;
            }
        }
    }

    // Set enable flags for bridges with vlan id != 0
    uint16_t ui16Flag = 0;
    /*
     * 0th bit : Enable 2G
     * 1st bit : Enable 5G
     * 2nd bit : Enable 6G
     *---------Above bits are set to Enable the respective bridges ------------*
     * 3rd bit : Disable 2G
     * 4th bit : Disable 5G
     * 5th bit : Disable 6G
     *---------Above bits are set to Disable the respective bridges ------------*
     * 6th bit : 2G Down --> Make the old bridge down
     * 7th bit : 2G Up --> bring the new bridge up
     * 8th bit : 5G Down --> Make the old bridge down
     * 9th bit : 5G Up --> bring the new bridge up
     * 10th bit : 6G Down --> Make the old bridge down
     * 11th bit : 6G Up --> bring the new bridge up
     *---------Above bits are set When there is a change in Vlan ID------------*/
    for (int iCount = 0; iCount < sBackupAmenityBridgeDetails.iBridgeCount; iCount++)
    {
        CcspTraceInfo(("%s:%d, Backup config Vlan=%d\n", __FUNCTION__, __LINE__, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId));
        if (sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId != 0)
        {
            setBridgeUpFlag(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName, &ui16Flag);
        }
    }

    CcspTraceInfo(("%s:%d, Creating Amenity Wifi network - from PSM records for %d bridge counts and vlanFlag = %d\n",
        __FUNCTION__, __LINE__, sBackupAmenityBridgeDetails.iBridgeCount, ui16Flag));

    if (0 != processTunnelInfo(&sBackupAmenityBridgeDetails, ui16Flag, pErrRetVal))
    {
        CcspTraceError(("%s:%d, Failed to process Tunnel Info, error code:%d\n", __FUNCTION__, __LINE__, pErrRetVal->ErrorCode));
        CcspTraceInfo(("%s:%d, Error message:%s\n", __FUNCTION__, __LINE__, pErrRetVal->ErrorMsg));
    }
    else
    {
        CcspTraceInfo(("%s:%d, processTunnelInfo success\n", __FUNCTION__, __LINE__));
    }

    free(pErrRetVal);
}

int updateFirewallSyscfg(int iBridgeCount, bool bIsAmenityEnabled )
{
    char cAmenityBridgeIdx[BUFF_LEN_32] = {'\0'};
    char cAmenityBridgeVal[BUFF_LEN_8] = {'\0'};
    for(int iCount = 0 ; iCount < iBridgeCount ; iCount++)
    {
        snprintf(cAmenityBridgeIdx , sizeof(cAmenityBridgeIdx) , "Amenity_Bridge_%d" , iCount);
        syscfg_get(NULL, cAmenityBridgeIdx , cAmenityBridgeVal, BUFF_LEN_8);
        CcspTraceInfo(("%s:%d, %s: current value %s=%s \n",__FUNCTION__,__LINE__, bIsAmenityEnabled ? "Adding" : "Removing" , cAmenityBridgeIdx, cAmenityBridgeVal ));
        if (bIsAmenityEnabled ? ( (!strncmp(cAmenityBridgeVal, "false" , 5 )) || (!strlen(cAmenityBridgeVal))) : (strncmp(cAmenityBridgeVal, "true" , 4 ) == 0))
        {
            CcspTraceInfo(("%s:%d, Changing %s to %s\n",__FUNCTION__,__LINE__, cAmenityBridgeIdx , bIsAmenityEnabled ? "true" : "false"));
            syscfg_set_commit(NULL, cAmenityBridgeIdx, bIsAmenityEnabled ? "true":"false");
        }
    }
    syscfg_get(NULL, "Amenity_Bridge_Count" , cAmenityBridgeVal, BUFF_LEN_8 );
    if (atoi(cAmenityBridgeVal) != iBridgeCount)
        syscfg_set_u_commit(NULL, "Amenity_Bridge_Count", iBridgeCount);
    return 0;
}

#if !defined (CORE_NET_LIB)
// Returns 1 if interface exists and is up, 0 if exists but down, -1 if not present
int isInterfaceUp(const char *pIfname)
{
    if (NULL == pIfname)
        return -1;

    int iSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSock < 0)
        return -1;

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, pIfname, IFNAMSIZ-1);

    int iRet = ioctl(iSock, SIOCGIFFLAGS, &ifr);
    close(iSock);

    if (iRet < 0)
        return -1; // Interface does not exist

    return (ifr.ifr_flags & IFF_UP) ? 1 : 0;
}
#endif

static void setVlanChangeFlags(
    const char *pVapName, int iCount,
    amenityBridgeDetails_t *pBackup, uint16_t *pFlag)
{
    if (!strncmp(pVapName, VAP_NAME_2G, sizeof(VAP_NAME_2G)))
    {
#ifdef CORE_NET_LIB
        if (interface_exist(pBackup->pBridgeInfoStruct[iCount].cBridgeName) == CNL_STATUS_SUCCESS)
#else
        if (1 == isInterfaceUp (pBackup->pBridgeInfoStruct[iCount].cBridgeName))
#endif
            *pFlag |= BIT_2G_DOWN_MASK;
        *pFlag |= BIT_2G_UP_MASK;
    }
    else if (!strncmp(pVapName, VAP_NAME_5G, sizeof(VAP_NAME_5G)))
    {
#ifdef CORE_NET_LIB
        if (interface_exist(pBackup->pBridgeInfoStruct[iCount].cBridgeName) == CNL_STATUS_SUCCESS)
#else
        if (1 == isInterfaceUp (pBackup->pBridgeInfoStruct[iCount].cBridgeName))
#endif
            *pFlag |= BIT_5G_DOWN_MASK;
        *pFlag |= BIT_5G_UP_MASK;
    }
    else if (!strncmp(pVapName, VAP_NAME_6G, sizeof(VAP_NAME_6G)))
    {
#ifdef CORE_NET_LIB
        if (interface_exist(pBackup->pBridgeInfoStruct[iCount].cBridgeName) == CNL_STATUS_SUCCESS)
#else
        if (1 == isInterfaceUp (pBackup->pBridgeInfoStruct[iCount].cBridgeName))
#endif
            *pFlag |= BIT_6G_DOWN_MASK;
        *pFlag |= BIT_6G_UP_MASK;
    }
}

static void setBridgeDownFlag(const char *pVapName, uint16_t *pFlag)
{
    if (!strncmp(pVapName, VAP_NAME_2G, sizeof(VAP_NAME_2G)))
        *pFlag |= BIT_2G_DISABLE_MASK;
    else if (!strncmp(pVapName, VAP_NAME_5G, sizeof(VAP_NAME_5G)))
        *pFlag |= BIT_5G_DISABLE_MASK;
    else if (!strncmp(pVapName, VAP_NAME_6G, sizeof(VAP_NAME_6G)))
        *pFlag |= BIT_6G_DISABLE_MASK;
}
pErr createAmenitiesBridge(lanconfigTunnelInfo_t * pLanCfgTunnelInfo)
{
    pErr pErrRetVal = (pErr) malloc(sizeof(Err));
    BOOL bHotSpot = FALSE;

    uint16_t ui16Flag = 0;
    /*
     * 0th bit : Enable 2G
     * 1st bit : Enable 5G
     * 2nd bit : Enable 6G
     *---------Above bits are set to Enable the respective bridges ------------*
     * 3rd bit : Disable 2G
     * 4th bit : Disable 5G
     * 5th bit : Disable 6G
     *---------Above bits are set to Disable the respective bridges ------------*
     * 6th bit : 2G Down --> Make the old bridge down
     * 7th bit : 2G Up --> bring the new bridge up
     * 8th bit : 5G Down --> Make the old bridge down
     * 9th bit : 5G Up --> bring the new bridge up
     * 10th bit : 6G Down --> Make the old bridge down
     * 11th bit : 6G Up --> bring the new bridge up
     *---------Above bits are set When there is a change in Vlan ID------------*/

    if (NULL == pErrRetVal)
    {
        CcspTraceError(("%s,%d : malloc failed\n",__FUNCTION__,__LINE__));
        return NULL;
    }
    memset(pErrRetVal, 0, sizeof(Err));
    pErrRetVal->ErrorCode = BLOB_EXEC_SUCCESS;
    if (NULL == pLanCfgTunnelInfo)
    {
        CcspTraceError(("%s:%d, pLanCfgTunnelInfo is NULL\n",__FUNCTION__,__LINE__));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "NULL parameter passed\n");
        pErrRetVal->ErrorCode = NULL_BLOB_EXEC_POINTER;
        return pErrRetVal;
    }

    bool bIsAmenityEnabled = false;
    if (pLanCfgTunnelInfo->pIsAmenityEnabled)
    {
        bIsAmenityEnabled = *(pLanCfgTunnelInfo->pIsAmenityEnabled);
        CcspTraceInfo(("%s:%d, IsAmenityEnabled:%s\n",__FUNCTION__, __LINE__, bIsAmenityEnabled ? "TRUE":"FALSE"));
    }
    if (ANSC_STATUS_SUCCESS != CosaDmlDiGetXfinityWiFiEnable(&bHotSpot))
    {
        CcspTraceError(("%s:%d, CosaDmlDiGetXfinityWiFiEnable failed\n",__FUNCTION__,__LINE__));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "CosaDmlDiGetXfinityWiFiEnable failed\n");
        pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
        return pErrRetVal;
    }
    if (!bIsAmenityEnabled && !sBackupAmenityBridgeDetails.bIsAmenityEnabled)
    {
        CcspTraceError(("%s:%d, Amenity network was already in disabled state\n",__FUNCTION__,__LINE__));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "Amenity network was already in disabled state\n");
        pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
        return pErrRetVal;
    }

    sCurrAmenityBridgeDetails.bIsAmenityEnabled = bIsAmenityEnabled;
    if (bIsAmenityEnabled)
    {
        if (FALSE == bHotSpot)
        {
            CcspTraceError(("%s:%d, Hotspot is disabled\n",__FUNCTION__,__LINE__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "Hotspot is disabled\n");
            pErrRetVal->ErrorCode = HOTSPOT_DISABLED;
            return pErrRetVal;
        }

        if (0 == pLanCfgTunnelInfo->iVapCount)
        {
            CcspTraceError(("%s:%d, iVapCount is 0\n",__FUNCTION__,__LINE__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "iVapCount is 0\n");
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return pErrRetVal;
        }
        if (NULL == pLanCfgTunnelInfo->ppVapNames || NULL == pLanCfgTunnelInfo->pVlanIds)
        {
            CcspTraceError(("%s:%d, ppVapNames or pVlanIds is NULL\n",__FUNCTION__,__LINE__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "ppVapNames or pVlanIds is NULL\n");
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return pErrRetVal;
        }

        sCurrAmenityBridgeDetails.iBridgeCount = pLanCfgTunnelInfo->iVapCount;
        if (NULL == sCurrAmenityBridgeDetails.pBridgeInfoStruct)
        {
            CcspTraceInfo(("%s:%d, allocating memory for sCurrAmenityBridgeDetails.pBridgeInfoStruct \n",__FUNCTION__,__LINE__));
            sCurrAmenityBridgeDetails.pBridgeInfoStruct = (amenityBridgeInfo_t *)malloc(sizeof(amenityBridgeInfo_t) * sCurrAmenityBridgeDetails.iBridgeCount);
            if (NULL == sCurrAmenityBridgeDetails.pBridgeInfoStruct)
            {
                CcspTraceError(("%s:%d, pBridgeInfoStruct malloc failed\n",__FUNCTION__,__LINE__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "pBridgeInfoStruct malloc failed\n");
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }
            memset(sCurrAmenityBridgeDetails.pBridgeInfoStruct, 0, sizeof(amenityBridgeInfo_t) * sCurrAmenityBridgeDetails.iBridgeCount);
        }

        if (NULL == pRbusBridgeInfo)
        {
            pRbusBridgeInfo = (amenityBridgeInfo_t *)malloc(pLanCfgTunnelInfo->iVapCount * sizeof(amenityBridgeInfo_t));
            if (NULL == pRbusBridgeInfo)
            {
                CcspTraceError(("%s:%d, pRbusBridgeInfo malloc failed\n",__FUNCTION__,__LINE__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "pRbusBridgeInfo malloc failed\n");
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }
            memset(pRbusBridgeInfo, 0, pLanCfgTunnelInfo->iVapCount * sizeof(amenityBridgeInfo_t));
        }
        for (int iCount = 0; iCount < pLanCfgTunnelInfo->iVapCount; iCount++)
        {
            sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId = pLanCfgTunnelInfo->pVlanIds[iCount];
            strncpy(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName, pLanCfgTunnelInfo->ppVapNames[iCount], sizeof(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName)-1);

            // Update PSM
            if (!strncmp(pLanCfgTunnelInfo->ppVapNames[iCount], VAP_NAME_2G, sizeof(VAP_NAME_2G)))
            {
                if (updateAmenityPSM(VAP_NAME_2G_INDEX, pLanCfgTunnelInfo->ppVapNames[iCount], iCount, pErrRetVal))
                    return pErrRetVal;
            }
            else if (!strncmp(pLanCfgTunnelInfo->ppVapNames[iCount], VAP_NAME_5G, sizeof(VAP_NAME_5G)))
            {
                if (updateAmenityPSM(VAP_NAME_5G_INDEX, pLanCfgTunnelInfo->ppVapNames[iCount], iCount, pErrRetVal))
                    return pErrRetVal;
            }
            else if (!strncmp(pLanCfgTunnelInfo->ppVapNames[iCount], VAP_NAME_6G, sizeof(VAP_NAME_6G)))
            {
                if (updateAmenityPSM(VAP_NAME_6G_INDEX, pLanCfgTunnelInfo->ppVapNames[iCount], iCount, pErrRetVal))
                    return pErrRetVal;
            }
            else
            {
                CcspTraceError(("%s:%d, Invalid VapName %s\n",__FUNCTION__,__LINE__,sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "Invalid VapName %s\n", sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName);
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }

            // Check for VLAN ID change
            if ((iCount < sBackupAmenityBridgeDetails.iBridgeCount) && (NULL != sBackupAmenityBridgeDetails.pBridgeInfoStruct))
            {
                for (int iVar = 0; iVar < sBackupAmenityBridgeDetails.iBridgeCount; iVar++)
                {
                    if (!strncmp(pLanCfgTunnelInfo->ppVapNames[iCount], sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName, sizeof(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName)) &&
                        (pLanCfgTunnelInfo->pVlanIds[iCount] != sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].iVlanId))
                    {
                        setVlanChangeFlags(pLanCfgTunnelInfo->ppVapNames[iCount], iCount,&sBackupAmenityBridgeDetails, &ui16Flag);
                    }
                }
            }
            else
            {
                setBridgeUpFlag(pLanCfgTunnelInfo->ppVapNames[iCount], &ui16Flag);
            }
        }

        if (strlen(sManageWiFiInfo.aBridgeName) > 0)
        {
#ifdef CORE_NET_LIB
            libnet_status eLibNetStatus = interface_exist(sManageWiFiInfo.aBridgeName);
            if (CNL_STATUS_SUCCESS == eLibNetStatus)
#else
            if (1 == isInterfaceUp (sManageWiFiInfo.aBridgeName))
#endif
            {
                CcspTraceInfo(("%s:%d, Managed Wifi Bridge interface brlan15 is present. Bring Down \n",__FUNCTION__,__LINE__));
                if (bringDownManagedWifiBridge(pErrRetVal))
                {
                    return pErrRetVal;
                }
                bDisabledManagedWifi = TRUE;
            }
        }

        CcspTraceInfo(("%s:%d, calling processTunnelInfo with vlanflag=%d \n",__FUNCTION__,__LINE__, ui16Flag));
        if (0 != processTunnelInfo(&sCurrAmenityBridgeDetails, ui16Flag, pErrRetVal))
        {
            CcspTraceError(("%s:%d, Failed to process Tunnel Info, error code:%d\n", __FUNCTION__, __LINE__, pErrRetVal->ErrorCode));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, " Failed to process Tunnel Info, error code:%d\n",pErrRetVal->ErrorCode);
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return pErrRetVal;
        }
        // Update pRbusBridgeInfo
        for (int iCount = 0; iCount < iNumOfMappings; iCount++)
        {
            int iBridgeIndex = vapMappings[iCount].iBridgeIndex;
            const char *pVapName = vapMappings[iCount].pVapName;

            for (int iVar = 0; iVar < sCurrAmenityBridgeDetails.iBridgeCount; iVar++)
            {
                if (!strncmp(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName, pVapName, sizeof(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName)))
                {
                    snprintf(pRbusBridgeInfo[iVar].cBridgeIndex, sizeof(pRbusBridgeInfo[iVar].cBridgeIndex), "%d", iBridgeIndex);
                    strncpy(pRbusBridgeInfo[iVar].cBridgeName, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iVar].cBridgeName, sizeof(pRbusBridgeInfo[iVar].cBridgeName)-1);
                    strncpy(pRbusBridgeInfo[iVar].cVapName, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName, sizeof(pRbusBridgeInfo[iVar].cVapName)-1);
                    CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d].cBridgeIndex:%s\n",__FUNCTION__,__LINE__,iVar,pRbusBridgeInfo[iVar].cBridgeIndex));
                    CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d].cBridgeName:%s\n",__FUNCTION__,__LINE__,iVar,pRbusBridgeInfo[iVar].cBridgeName));
                    CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d].cVapName:%s\n",__FUNCTION__,__LINE__,iVar,pRbusBridgeInfo[iVar].cVapName));
                    break;
                }
            }
        }
    }
    else
    {
        // Disabled Amenity
        for (int iCount = 0; iCount < sBackupAmenityBridgeDetails.iBridgeCount; iCount++)
        {
#ifdef CORE_NET_LIB
            if (interface_exist(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName) == CNL_STATUS_SUCCESS)
#else
            if (1 == isInterfaceUp (sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName))
#endif
            {
                setBridgeDownFlag(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName, &ui16Flag);
            }
        }
        bool bTempAmenityEnabled = sBackupAmenityBridgeDetails.bIsAmenityEnabled ;
        sBackupAmenityBridgeDetails.bIsAmenityEnabled = bIsAmenityEnabled;
        CcspTraceInfo(("%s:%d, calling processTunnelInfo with vlanflag=%d \n",__FUNCTION__,__LINE__, ui16Flag));
        if (0 != processTunnelInfo(&sBackupAmenityBridgeDetails, ui16Flag, pErrRetVal))
        {
            sBackupAmenityBridgeDetails.bIsAmenityEnabled=bTempAmenityEnabled;
            CcspTraceError(("%s:%d, Failed to process Tunnel Info, error code:%d\n", __FUNCTION__, __LINE__, pErrRetVal->ErrorCode));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, " Failed to process Tunnel Info, error code:%d\n",pErrRetVal->ErrorCode);
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return pErrRetVal;
        }
        sBackupAmenityBridgeDetails.bIsAmenityEnabled=bTempAmenityEnabled;
        // Update pRbusBridgeInfo
        for (int iCount = 0; iCount < iNumOfMappings; iCount++)
        {
            const char *pVapName = vapMappings[iCount].pVapName;

            for (int iVar = 0; iVar < sBackupAmenityBridgeDetails.iBridgeCount; iVar++)
            {
                if (!strncmp(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName, pVapName, sizeof(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName)))
                {
                    snprintf(pRbusBridgeInfo[iVar].cBridgeIndex, sizeof(pRbusBridgeInfo[iVar].cBridgeIndex), "0");
                    strncpy(pRbusBridgeInfo[iVar].cBridgeName, "", sizeof(pRbusBridgeInfo[iVar].cBridgeName)-1);
                    strncpy(pRbusBridgeInfo[iVar].cVapName, "", sizeof(pRbusBridgeInfo[iVar].cVapName)-1);
                    CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d].cBridgeIndex:%s\n",__FUNCTION__,__LINE__,iVar,pRbusBridgeInfo[iVar].cBridgeIndex));
                    CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d].cBridgeName:%s\n",__FUNCTION__,__LINE__,iVar,pRbusBridgeInfo[iVar].cBridgeName));
                    CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d].cVapName:%s\n",__FUNCTION__,__LINE__,iVar,pRbusBridgeInfo[iVar].cVapName));
                    break;
                }
            }
        }
        sCurrAmenityBridgeDetails.bIsAmenityEnabled = bIsAmenityEnabled;
        sCurrAmenityBridgeDetails.iBridgeCount = 0;
    }

    snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128, "Success");
    pErrRetVal->ErrorCode = BLOB_EXEC_SUCCESS;
    CcspTraceInfo(("%s:%d, BLOB_EXEC_SUCCESS \n",__FUNCTION__,__LINE__));
    return pErrRetVal;
}

BOOL bringDownManagedWifiBridge(pErr pErrRetVal)
{
    char cManageWiFiBridgeIndex[BUFF_LEN_8] = {'\0'};
    int pthreadRetValue=0 , err=0;
    pthread_t manageWifiBridgeThreadId;
    struct timespec abs_time = {0};
    if (0 != psmGet(MANAGE_WIFI_BRIDGE_INDEX, cManageWiFiBridgeIndex, sizeof(cManageWiFiBridgeIndex)))
    {
        CcspTraceError(("%s:%d, Failed to get %s\n",__FUNCTION__,__LINE__,MANAGE_WIFI_BRIDGE_INDEX));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Failed to get %s\n",MANAGE_WIFI_BRIDGE_INDEX);
        if (pErrRetVal != NULL) pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
        return 1;
    }
    if ('\0' == cManageWiFiBridgeIndex[0])
    {
        strncpy(cManageWiFiBridgeIndex, "17", sizeof(cManageWiFiBridgeIndex)-1);
    }
    CcspTraceInfo(("%s:%d, creating Pthread for bring down manageWiFi bridge interface \n",__FUNCTION__,__LINE__));
    threadStruct_t sThread = {false, 0xFF};
    pthread_condattr_init(&manageWifi_attr);
    pthread_condattr_setclock(&manageWifi_attr, CLOCK_MONOTONIC);
    pthread_cond_init(&manageWifi_exec_completed, &manageWifi_attr);
    pthreadRetValue = pthread_create(&manageWifiBridgeThreadId, NULL, &ManageWiFiBridgeCreationThread, (void*)&sThread);
    if (0 != commonSyseventSet("multinet-down", cManageWiFiBridgeIndex))
    {
        CcspTraceError(("%s:%d, Failed to set multinet-down\n",__FUNCTION__,__LINE__));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Failed to set multinet-down\n");
        if (pErrRetVal != NULL) pErrRetVal->ErrorCode = SYSEVENT_FAILURE;
        return 1;
    }
    if (0 == pthreadRetValue)
    {
        clock_gettime(CLOCK_MONOTONIC, &abs_time);
        abs_time.tv_sec += 4; //wait 4sec
        abs_time.tv_nsec += 0;

        pthread_mutex_lock(&manageWifi_exec);
        err = pthread_cond_timedwait(&manageWifi_exec_completed, &manageWifi_exec, &abs_time);
        if (err == ETIMEDOUT)
        {
            CcspTraceInfo(("%s:%d, Timedout Cancelling the ManageWiFiThread\n",__FUNCTION__,__LINE__));
            pthread_cancel(manageWifiBridgeThreadId);
        }
        pthread_mutex_unlock(&manageWifi_exec);
    }
    CcspTraceInfo(("%s:%d, Restarting the firewall\n",__FUNCTION__,__LINE__));
    if (0 != commonSyseventSet("firewall-restart", sManageWiFiInfo.aBridgeIndex))
    {
        CcspTraceError(("%s: commonSyseventSet Failed for Firewall-restart\n", __FUNCTION__));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"commonSyseventSet Failed For Firewall-restart\n");
        if (pErrRetVal != NULL) pErrRetVal->ErrorCode = SYSEVENT_FAILURE;
        return 1;
    }

    CcspTraceInfo(("%s:%d, Success :  Managed wifi multinet bridge index(%s) down \n",__FUNCTION__,__LINE__, cManageWiFiBridgeIndex));
    return 0;
}


BOOL updateAmenityPSM(char *cVapIndex, char *cVapName , int iCount , pErr pErrRetVal)
{
    char cParamName[BUFF_LEN_64] = {'\0'};
    char cVlanId[BUFF_LEN_8] = {'\0'};
    CcspTraceInfo(("%s:%d, cVapIndex = %s ,  pLancfgVapName=%s for Count = %d\n",__FUNCTION__,__LINE__, cVapIndex, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName,iCount));
    if (0 != psmGet (cVapIndex,sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex,sizeof(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex)))
    {
        CcspTraceError(("%s:%d, Failed to get %s\n",__FUNCTION__,__LINE__,cVapIndex));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Failed to get %s\n",cVapIndex);
        pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
        return 1;
    }
    snprintf(cParamName, BUFF_LEN_64, l2netBridgeName, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex);
    CcspTraceInfo(("%s:%d, cParamName:%s\n",__FUNCTION__,__LINE__,cParamName));
    if (0 != psmGet(cParamName, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName, sizeof(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName)))
    {
        CcspTraceError(("%s:%d, Failed to get %s\n",__FUNCTION__,__LINE__,cParamName));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Failed to get %s\n",cParamName);
        pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
        return 1;
    }
    if ((NULL == sBackupAmenityBridgeDetails.pBridgeInfoStruct) || (strncmp(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName , sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName , strlen(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName)) != 0 ))
    {
        snprintf (cParamName, BUFF_LEN_64, l2netVapName, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex);
        CcspTraceInfo(("%s:%d, cParamName:%s\n",__FUNCTION__,__LINE__,cParamName));
        snprintf(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName,sizeof(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName),"%s", cVapName);
        CcspTraceInfo(("%s:%d, sCurrAmenityBridgeDetails.pBridgeInfoStruct[%d].cVapName:%s\n",__FUNCTION__,__LINE__,iCount,sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName));
        if (0 != psmSet(cParamName, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName))
        {
            CcspTraceError(("%s:%d, psmSet failed for %s\n",__FUNCTION__,__LINE__,cParamName));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Failed to set %s\n",cParamName);
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return 1;
        }
     }
     if ((NULL == sBackupAmenityBridgeDetails.pBridgeInfoStruct) || (0 == sBackupAmenityBridgeDetails.iBridgeCount))
     {
        snprintf(cParamName,BUFF_LEN_64,l2netVlanID,sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex);
        snprintf(cVlanId, BUFF_LEN_8, "%d", sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId);
        CcspTraceInfo(("%s:%d, cParamName:%s\n",__FUNCTION__,__LINE__,cParamName));
        CcspTraceInfo(("%s:%d, cVlanId:%s\n",__FUNCTION__,__LINE__,cVlanId));
        if (0 != psmSet (cParamName, cVlanId))
        {
            CcspTraceError(("%s:%d, psmSet failed for %s\n",__FUNCTION__,__LINE__,cParamName));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Failed to set %s\n",cParamName);
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return 1;
        }
     }
     snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM values are updated\n");
     pErrRetVal->ErrorCode = BLOB_EXEC_SUCCESS;
     return 0;
}
#endif


//Unpack the managedwifi blob data
BOOL unpackAndProcessManagedWifiData(char* pString)
{
    char * decodeMsg =NULL;
    char * wifi_encoded_data = NULL;
    int size =0;
    int retval = 0;
    msgpack_unpack_return unpack_ret = MSGPACK_UNPACK_SUCCESS;
    if (NULL == pString)
    {
        CcspTraceError(("%s:%d, NULL parameter passed\n",__FUNCTION__,__LINE__));
        return FALSE;
    }
    retval = get_base64_decodedbuffer(pString, &decodeMsg, &size);
    if (retval == 0)
    {
        unpack_ret = get_msgpack_unpack_status(decodeMsg,size);
    }
    else
    {
        if (decodeMsg)
        {
            free(decodeMsg);
            decodeMsg = NULL;
        }
        return FALSE;
    }
    if(unpack_ret == MSGPACK_UNPACK_SUCCESS)
    {
        managedwifidoc_t *md = NULL;
        #if defined (AMENITIES_NETWORK_ENABLED)
        lanconfigTunnelInfo_t *ld = NULL;
        #else
        lanconfigdoc_t *ld = NULL;
        #endif

        md = managedwifidoc_convert( decodeMsg, size+1 );
        if ( decodeMsg )
        {
            free(decodeMsg);
            decodeMsg = NULL;
        }

        if (NULL !=md)
        {
            CcspTraceInfo(("The transaction id is %d\n", md->transaction_id));
            CcspTraceInfo(("The version is %lu\n", (long)md->version));
            CcspTraceInfo(("The subdoc_name is %s\n", md->subdoc_name));

            //Convert the lanconfig doc
            ld = lanconfigdoc_convert( md->entries[0].value, md->entries[0].value_size );
            if (ld == NULL ) {
                if ( decodeMsg ) {
                     free(decodeMsg);
                     decodeMsg = NULL;
                  }
                CcspTraceInfo(("Corrupted lanconfig value\n"));
                return FALSE;
                }

            //wifi_encoded_data is the b64 encoded data obtained after appending subdoc_name, version and transaction_id
            wifi_encoded_data = mg_append_wifi_doc(md->subdoc_name,md->version,md->transaction_id,md->entries[1].value,md->entries[1].value_size);
            CcspTraceInfo(("The wifi_encoded_data is %s\n", wifi_encoded_data));

            execData *execDataManagedWifi = NULL;
            policySequence *sequenceDetails = NULL;
            execDataManagedWifi = (execData*) malloc (sizeof(execData));
            sequenceDetails = (policySequence*) malloc (sizeof (policySequence) );
            memset(sequenceDetails, 0, sizeof(policySequence));
            sequenceDetails->isExecInSequenceNeeded = 1 ;
            sequenceDetails->numOfComponents = 2 ;
            // Post exec settings
            #if defined (AMENITIES_NETWORK_ENABLED)
            sequenceDetails->postExecSettings = confirmAmentitiesNetworkVap;
            #else
            sequenceDetails->postExecSettings = confirmManageWifiVap;
            #endif

            sequenceDetails->multiCompExecData = (MultiComp_ExecInfo*) malloc (sequenceDetails->numOfComponents * sizeof(MultiComp_ExecInfo));
            memset(sequenceDetails->multiCompExecData, 0, sequenceDetails->numOfComponents * sizeof(MultiComp_ExecInfo));

            sequenceDetails->multiCompExecData->isMaster = 1 ;
            sequenceDetails->multiCompExecData->comp_exec_data = (void*) ld ;
            CcspTraceInfo(("DEBUG : master data %p\n",sequenceDetails->multiCompExecData->comp_exec_data));

            sequenceDetails->multiCompExecData++;
            sequenceDetails->multiCompExecData->isMaster =0 ;

            #if defined (RDK_ONEWIFI)
            strncpy(sequenceDetails->multiCompExecData->CompName,"OneWifi",sizeof(sequenceDetails->multiCompExecData->CompName)-1);
            #else
            strncpy(sequenceDetails->multiCompExecData->CompName,"CcspWifiSsp",sizeof(sequenceDetails->multiCompExecData->CompName)-1);
            #endif
            CcspTraceInfo(("DEBUG : wifi_encoded_data %p\n",wifi_encoded_data));

            sequenceDetails->multiCompExecData->comp_exec_data = (void*) wifi_encoded_data ;

            CcspTraceInfo(("DEBUG : sequenceDetails->multiCompExecData->comp_exec_data pointer is %p\n",sequenceDetails->multiCompExecData->comp_exec_data));

            sequenceDetails->multiCompExecData--;

            if ( execDataManagedWifi != NULL )
            {
                memset(execDataManagedWifi, 0, sizeof(execData));
                execDataManagedWifi->txid = md->transaction_id;
                execDataManagedWifi->version = (uint32_t)md->version;
                execDataManagedWifi->numOfEntries = 0;
                strncpy(execDataManagedWifi->subdoc_name,"connectedbuilding",sizeof(execDataManagedWifi->subdoc_name)-1);
                execDataManagedWifi->executeBlobRequest = processManagedWebconfigConfig;
                #if defined (AMENITIES_NETWORK_ENABLED)
                execDataManagedWifi->rollbackFunc = rollbackTunnelLanconfig;
                #else
                execDataManagedWifi->rollbackFunc = rollbackLanconfig;
                #endif
                execDataManagedWifi->freeResources = freeMem_managedwifi;
                execDataManagedWifi->calcTimeout = calculateTimeout;
                execDataManagedWifi->user_data = (void*) sequenceDetails;
                execDataManagedWifi->multiCompRequest =1;
                PushMultiCompBlobRequest(execDataManagedWifi);
                CcspTraceInfo(("PushBlobRequest complete\n"));
                return TRUE;
            }
            else
            {
                CcspTraceWarning(("execData memory allocation failed\n"));
                #if defined (AMENITIES_NETWORK_ENABLED)
                tunnelLanConfigDocdestroy(ld);
                #else
                lanConfigDoc_destroy(ld);
                #endif
                connectedbuilding_destroy(md);
                return FALSE;
            }
        }
        return TRUE;
    }
    else
    {
        if ( decodeMsg )
        {
            free(decodeMsg);
            decodeMsg = NULL;
        }
        CcspTraceInfo(("Corrupted managedwifi value\n"));
        return FALSE;
    }
    return TRUE;
}


pErr processManagedWebconfigConfig(void *Data)
{
    pErr execRetVal = NULL;

    if (NULL == Data)
    {
        CcspTraceError(("%s:%d, Data is NULL\n",__FUNCTION__,__LINE__));
        execRetVal = (pErr) malloc (sizeof(Err));
        if (execRetVal != NULL )
        {
            memset(execRetVal,0,sizeof(Err));
            snprintf(execRetVal->ErrorMsg, BUFF_LEN_128,"NULL parameter passed\n");
            execRetVal->ErrorCode = NULL_BLOB_EXEC_POINTER;
        }
        return execRetVal;
    }
    #if defined (AMENITIES_NETWORK_ENABLED)
    lanconfigTunnelInfo_t *pLanCfgTunnelInfo = (lanconfigTunnelInfo_t *) Data;
    CcspTraceInfo(("%s:%d,VapCount = %d\n",__FUNCTION__,__LINE__,pLanCfgTunnelInfo->iVapCount));
    CcspTraceInfo(("%s:%d,AmenityEnabled = %s\n",__FUNCTION__,__LINE__,*(pLanCfgTunnelInfo->pIsAmenityEnabled) ? "TRUE" : "FALSE"));
    for (int iCount = 0; iCount < pLanCfgTunnelInfo->iVapCount; iCount++)
    {
        CcspTraceInfo(("%s:%d, VAPName = %s\n",__FUNCTION__,__LINE__,pLanCfgTunnelInfo->ppVapNames[iCount]));
        CcspTraceInfo(("%s:%d, VlanId = %d\n",__FUNCTION__,__LINE__,pLanCfgTunnelInfo->pVlanIds[iCount]));
    }
    return createAmenitiesBridge(pLanCfgTunnelInfo);
    #else
    lanconfigdoc_t *mwd = (lanconfigdoc_t *) Data ;

    CcspTraceInfo(("%s : DhcpServerEnable - %d\n",__FUNCTION__,mwd->entries->dhcp_server_enable));
    CcspTraceInfo(("%s : LanIPAddress - %s\n",__FUNCTION__,mwd->entries->lan_ip_address));
    CcspTraceInfo(("%s : LanSubnetMask - %s\n",__FUNCTION__,mwd->entries->lan_subnet_mask));
    CcspTraceInfo(("%s : DhcpStartIPAddress - %s\n",__FUNCTION__,mwd->entries->dhcp_start_ip_address));
    CcspTraceInfo(("%s : DhcpEndIPAddress - %s\n",__FUNCTION__,mwd->entries->dhcp_end_ip_address));
    CcspTraceInfo(("%s : LeaseTime - %s\n",__FUNCTION__,mwd->entries->leasetime));
    CcspTraceInfo(("%s : enable - %d\n",__FUNCTION__,mwd->entries->mwenable));
    CcspTraceInfo(("%s : ipv6enable - %d\n",__FUNCTION__,mwd->entries->ipv6enable));
    CcspTraceInfo(("%s : firewall - %s\n",__FUNCTION__,mwd->entries->firewall));
    CcspTraceInfo(("%s : alias - %s\n",__FUNCTION__,mwd->entries->alias));
    execRetVal = create_manage_wifi_bridge(mwd->entries);
    return execRetVal;
    #endif
}

void freeMem_managedwifi(void* arg)
{
    execData *blob_exec_data  = (execData*) arg;

    if (NULL == arg)
        return;

    policySequence *sequenceDetails = (policySequence*)blob_exec_data->user_data;
    /*CID-346806  Null pointer Dereference Fix*/
    if( sequenceDetails == NULL)
    {
        CcspTraceInfo(("sequenceDetails is NULL in %s \n",__FUNCTION__));
        return;
    }
    #if defined (AMENITIES_NETWORK_ENABLED)
    lanconfigTunnelInfo_t *pLanCfgTunnelInfo = (lanconfigTunnelInfo_t *) sequenceDetails->multiCompExecData->comp_exec_data;
    if ( pLanCfgTunnelInfo != NULL )
    {
        tunnelLanConfigDocdestroy( pLanCfgTunnelInfo );
        pLanCfgTunnelInfo = NULL;
    }
    #else
    lanconfigdoc_t *mwd = (lanconfigdoc_t *) sequenceDetails->multiCompExecData->comp_exec_data;
    if ( mwd != NULL )
    {
        lanConfigDoc_destroy( mwd );
        mwd = NULL;
    }
    #endif
    sequenceDetails->multiCompExecData++;
    char * wifi_encoded_data = (char*) sequenceDetails->multiCompExecData->comp_exec_data;

    if ( wifi_encoded_data != NULL )
    {
        free(wifi_encoded_data);
        wifi_encoded_data = NULL;
    }

    sequenceDetails->multiCompExecData--;
    if (sequenceDetails->multiCompExecData)
    {
        free(sequenceDetails->multiCompExecData);
        sequenceDetails->multiCompExecData = NULL;
    }

    if (sequenceDetails)
    {
        free(sequenceDetails);
        sequenceDetails = NULL;
    }

    if ( blob_exec_data != NULL )
    {
        free(blob_exec_data);
        blob_exec_data = NULL ;
    }
}

size_t calculateTimeout(size_t numOfEntries){
    UNREFERENCED_PARAMETER(numOfEntries);
    CcspTraceInfo(("calling calculateTimeout\n"));
    return 30;
}

void * ManageWiFiBridgeCreationThread(void * vArg)
{
    UNREFERENCED_PARAMETER(vArg);
    char aMultiNetStatus[BUFF_LEN_64] = {0};
    char aParamVal[BUFF_LEN_32] = {0};
    async_id_t interfaceManageWiFiAsyncid;
    static int syseventManageWiFiFd;
    static token_t syseventManageWiFiToken;
    int err;
    char name[25] = {0}, val[42] = {0}, buf[BUFF_LEN_128], cmd[BUFF_LEN_128];
    int oldtype;
    threadStruct_t * pThreadStruct = (threadStruct_t*) vArg;

    /* allow the thread to be killed at any time */
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    pthread_detach(pthread_self());

    syseventManageWiFiFd = sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "ManageWiFiBridgeHandler", &syseventManageWiFiToken);

    CcspTraceInfo(("%s:%d,pThreadStruct->bMwEnable:%d\n",__FUNCTION__,__LINE__, pThreadStruct->bMwEnable));
    CcspTraceInfo(("%s:%d,aBridgeIndex:%s\n",__FUNCTION__,__LINE__, sManageWiFiInfo.aBridgeIndex));
    snprintf (aMultiNetStatus,BUFF_LEN_64,"multinet_%s-status",sManageWiFiInfo.aBridgeIndex);
    CcspTraceInfo(("%s:%d,aMultiNetStatus:%s\n",__FUNCTION__,__LINE__, aMultiNetStatus));
    if (pThreadStruct->cFlag != 0xFF)
    {
        if(0 == sysevent_get(syseventManageWiFiFd, syseventManageWiFiToken, aMultiNetStatus, aParamVal,BUFF_LEN_32 ) && '\0' != aParamVal[0])
        {
            CcspTraceInfo(("%s:%d,aParamVal:%s\n",__FUNCTION__,__LINE__, aParamVal));
            CcspTraceInfo(("%s:%d,pThreadStruct->bMwEnable:%d\n",__FUNCTION__,__LINE__, pThreadStruct->bMwEnable));
            if ((!strncmp(aParamVal,"ready", strlen("ready"))) && (true == pThreadStruct->bMwEnable))
            {
                snprintf(aMultiNetStatus, BUFF_LEN_64, "%s%s",sManageWiFiInfo.aKey,sManageWiFiInfo.aBridgeName);
                CcspTraceInfo(("%s:%d,aMultiNetStatus:%s\n",__FUNCTION__,__LINE__, aMultiNetStatus));
                publishEvent(MANAGE_WIFI_LAN_BRIDGE,aMultiNetStatus, RBUS);
                /* wake up the caller if execution is completed in time */
                pthread_cond_signal(&manageWifi_exec_completed);
                if (0 < syseventManageWiFiFd)
                    sysevent_close(syseventManageWiFiFd, syseventManageWiFiToken);
                return vArg;
            }
            else if ((!strcmp ((const char*)val, "stopped")) && (false == pThreadStruct->bMwEnable))
            {
                publishEvent(MANAGE_WIFI_LAN_BRIDGE,sManageWiFiInfo.aKey, RBUS);
                CcspTraceInfo(("%s:%d, Restarting the zebra\n",__FUNCTION__,__LINE__));
                if (0 != commonSyseventSet("zebra-restart", ""))
                    CcspTraceError(("%s:%d, commonSyseventSet failed for zebra-restart\n",__FUNCTION__,__LINE__));
                CcspTraceInfo(("%s:%d, Restarting the dhcp_server\n",__FUNCTION__,__LINE__));
                int fd = creat("/var/tmp/lan_not_restart", O_WRONLY|O_CREAT);
                if (-1 != fd)
                    close(fd);
                else
                    CcspTraceError(("%s:%d, Failed to create the /var/tmp/lan_not_restart file\n",__FUNCTION__,__LINE__));
                if (0 != commonSyseventSet("dhcp_server-restart", ""))
                    CcspTraceError(("%s:%d, commonSyseventSet failed for dhcp_server-restart\n",__FUNCTION__,__LINE__));
                /* wake up the caller if execution is completed in time */
                pthread_cond_signal(&manageWifi_exec_completed);
                if (0 < syseventManageWiFiFd)
                    sysevent_close(syseventManageWiFiFd, syseventManageWiFiToken);
                return vArg;
            }
        }
    }

    sysevent_set_options(syseventManageWiFiFd, syseventManageWiFiToken, aMultiNetStatus, TUPLE_FLAG_EVENT);
    sysevent_setnotification(syseventManageWiFiFd, syseventManageWiFiToken,aMultiNetStatus,  &interfaceManageWiFiAsyncid);

    while(1)
    {
        async_id_t getnotification_asyncid;
        memset(name,0,sizeof(name));
        memset(val,0,sizeof(val));
        memset(cmd,0,sizeof(cmd));
        memset(buf,0,sizeof(buf));

        int namelen = sizeof(name);
        int vallen  = sizeof(val);
        err = sysevent_getnotification(syseventManageWiFiFd, syseventManageWiFiToken, name, &namelen,  val, &vallen, &getnotification_asyncid);

        if (err)
        {
            CcspTraceWarning(("sysevent_getnotification failed with error: %d %s\n", err,__FUNCTION__));
            CcspTraceWarning(("sysevent_getnotification failed name: %s val : %s\n", name,val));
            if ( 0 != v_secure_system("pidof syseventd"))
            {
                CcspTraceWarning(("%s syseventd not running ,breaking the receive notification loop \n",__FUNCTION__));
                break;
            }
        }
        else
        {
            CcspTraceInfo(("%s:%d,Received the Notification :%s:%s\n",__FUNCTION__,__LINE__, aMultiNetStatus,val));
            CcspTraceWarning(("%s Recieved notification event  %s\n",__FUNCTION__,name));
            if(!strcmp((const char*)name,aMultiNetStatus))
            {
                CcspTraceInfo(("%s:%d,val:%s\n",__FUNCTION__,__LINE__, val));
                if(!strcmp((const char*)val, "ready"))
                {
                    snprintf(aMultiNetStatus, BUFF_LEN_64, "%s%s",sManageWiFiInfo.aKey,sManageWiFiInfo.aBridgeName);
                    CcspTraceInfo(("%s:%d,notifying:%s\n",__FUNCTION__,__LINE__, aMultiNetStatus));
                    publishEvent(MANAGE_WIFI_LAN_BRIDGE,aMultiNetStatus, RBUS);
                    if (pThreadStruct->cFlag & (1 << 4))
                    {
                        CcspTraceInfo(("%s:%d, Restarting the zebra\n",__FUNCTION__,__LINE__));
                        if (0 != commonSyseventSet("zebra-restart", ""))
                            CcspTraceError(("%s:%d, commonSyseventSet failed for zebra-restart\n",__FUNCTION__,__LINE__));
                    }
                    if ((pThreadStruct->cFlag & (1 << 3)) || (pThreadStruct->cFlag & (1 << 2)))
                    {
                        CcspTraceInfo(("%s:%d, Restarting the dhcp_server\n",__FUNCTION__,__LINE__));
                        int fd = creat("/var/tmp/lan_not_restart", O_WRONLY|O_CREAT);
                        if (-1 != fd)
                            close(fd);
                        else
                            CcspTraceError(("%s:%d, Failed to create the /var/tmp/lan_not_restart file\n",__FUNCTION__,__LINE__));
                        if (0 != commonSyseventSet("dhcp_server-restart", ""))
                            CcspTraceError(("%s:%d, commonSyseventSet failed for dhcp_server-restart\n",__FUNCTION__,__LINE__));
                    }
                    break;
                }
                else if (!strcmp ((const char*)val, "stopped"))
                {
                    CcspTraceInfo(("%s:%d,notifying:%s\n",__FUNCTION__,__LINE__, sManageWiFiInfo.aKey));
                    publishEvent(MANAGE_WIFI_LAN_BRIDGE,sManageWiFiInfo.aKey, RBUS);
                    CcspTraceInfo(("%s:%d, Restarting the zebra\n",__FUNCTION__,__LINE__));
                    if (0 != commonSyseventSet("zebra-restart", ""))
                        CcspTraceError(("%s:%d, commonSyseventSet failed for zebra-restart\n",__FUNCTION__,__LINE__));
                    CcspTraceInfo(("%s:%d, Restarting the dhcp_server\n",__FUNCTION__,__LINE__));
                    int fd = creat("/var/tmp/lan_not_restart", O_WRONLY|O_CREAT);
                    if (-1 != fd)
                        close(fd);
                    else
                        CcspTraceError(("%s:%d, Failed to create the /var/tmp/lan_not_restart file\n",__FUNCTION__,__LINE__));
                    if (0 != commonSyseventSet("dhcp_server-restart", ""))
                        CcspTraceError(("%s:%d, commonSyseventSet failed for dhcp_server-restart\n",__FUNCTION__,__LINE__));
                    break;
                }
            }
        }
    }
    /* wake up the caller if execution is completed in time */
    pthread_cond_signal(&manageWifi_exec_completed);
    if (0 < syseventManageWiFiFd)
        sysevent_close(syseventManageWiFiFd, syseventManageWiFiToken);
    return vArg;
}

pErr create_manage_wifi_bridge(lanconfig_t * pLanConfigParams)
{
    char aParamName[BUFF_LEN_64]  = {0};
    char aParamVal[BUFF_LEN_64] = {0};
    pErr pErrRetVal = NULL;

    /*
     * 0th bit : Manage Wifi Enable
     * 1st bit : Lan IP addr
     * 2nd bit : lan subnet mask, dhcp V4 pool subnet mask
     * 3rd bit : DhcpV4Pool Enable, Start and End address, lease time
     * 4th bit : IPv6 Enable
     * 5th bit : alias
     */
    char cFlag = 0;

    pErrRetVal = (pErr) malloc (sizeof(Err));
    if (NULL == pErrRetVal)
    {
        CcspTraceError(("%s,%d : malloc failed\n",__FUNCTION__,__LINE__));
        return pErrRetVal;
    }
    memset(pErrRetVal,0,sizeof(Err));
    pErrRetVal->ErrorCode = BLOB_EXEC_SUCCESS;

    if (NULL == pLanConfigParams)
    {
        CcspTraceError(("%s: NULL parameter passed\n",__FUNCTION__));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"NULL parameter passed\n");
        pErrRetVal->ErrorCode = NULL_BLOB_EXEC_POINTER;
        return pErrRetVal;
    }

    if (true == pLanConfigParams->mwenable)
    {
        if (0 != validateIpRange(pLanConfigParams->lan_ip_address, pLanConfigParams->dhcp_start_ip_address, pLanConfigParams->dhcp_end_ip_address, pErrRetVal))
            return pErrRetVal;

        if (0 != validateLeaseTime(pLanConfigParams->leasetime, pErrRetVal))
            return pErrRetVal;
    }
    if (pLanConfigParams->mwenable != sBackupLanConfig.bMwEnable)
    {
        sCurrentLanConfig.bMwEnable = pLanConfigParams->mwenable;
        if (true == pLanConfigParams->mwenable)
            snprintf(aParamVal, BUFF_LEN_64,"true");
        else if (false == pLanConfigParams->mwenable)
            snprintf(aParamVal, BUFF_LEN_64,"false");
        snprintf(aParamName, BUFF_LEN_64, l2netBridgeEnable, sManageWiFiInfo.aBridgeIndex);
        if (0 != psmSet(aParamName, aParamVal))
        {
            iRestoreFlag |= (1 << 0);
            CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi Enable\n");
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return pErrRetVal;
        }
        iRestoreFlag |= (1 << 1);
        cFlag |= (1 << 0);
    }
    else if ((false == pLanConfigParams->mwenable) && (false == sBackupLanConfig.bMwEnable))
    {
        CcspTraceError(("%s: previously MW Enable is false, again received false\n",__FUNCTION__));
        return pErrRetVal;
    }
    else
        sCurrentLanConfig.bMwEnable = pLanConfigParams->mwenable;

    if (NULL != pLanConfigParams->alias)
    {
        if (strcmp(sBackupLanConfig.aAlias, pLanConfigParams->alias))
        {
            snprintf(aParamName, BUFF_LEN_64, l2netAlias, sManageWiFiInfo.aBridgeIndex);
            snprintf(aParamVal, BUFF_LEN_64, "%s", pLanConfigParams->alias);
            if (0 != psmSet(aParamName, aParamVal))
            {
                iRestoreFlag |= (1 << 0);
                CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi Alias\n");
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }
            iRestoreFlag |= (1 << 10);
            cFlag |= (1 << 5);
        }
        strncpy(sCurrentLanConfig.aAlias, pLanConfigParams->alias, (sizeof(sCurrentLanConfig.aAlias)-1));
    }

    if (NULL != pLanConfigParams->lan_ip_address)
    {
        if (strcmp(pLanConfigParams->lan_ip_address, sBackupLanConfig.aLanIpAddr))
        {
            snprintf(aParamName, BUFF_LEN_64, l3netV4Addr, sManageWiFiInfo.aBridgeIndex);
            if( 0 != psmSet(aParamName, pLanConfigParams->lan_ip_address))
            {
                iRestoreFlag |= (1 << 0);
                CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi Lan ip address\n");
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }
            iRestoreFlag |= (1 << 2);
            cFlag |= (1 << 1);
        }
        strncpy(sCurrentLanConfig.aLanIpAddr, pLanConfigParams->lan_ip_address, (sizeof(sCurrentLanConfig.aLanIpAddr)-1));
    }

    if (NULL != pLanConfigParams->lan_subnet_mask)
    {
        if(strcmp(pLanConfigParams->lan_subnet_mask, sBackupLanConfig.aLanSubnetMask))
        {
            snprintf(aParamName, BUFF_LEN_64, l2netV4SubnetMask, sManageWiFiInfo.aBridgeIndex);
            if( 0 != psmSet(aParamName, pLanConfigParams->lan_subnet_mask))
            {
                iRestoreFlag |= (1 << 0);
                CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi Lan subnet mask\n");
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }
            strncpy(sCurrentLanConfig.aLanSubnetMask, pLanConfigParams->lan_subnet_mask, (sizeof(sCurrentLanConfig.aLanSubnetMask)-1));
            iRestoreFlag |= (1 << 3);
            cFlag |= (1 << 2);
            snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolSubnetMask, sManageWiFiInfo.aBridgeIndex);
            if( 0 != psmSet(aParamName, pLanConfigParams->lan_subnet_mask))
            {
                iRestoreFlag |= (1 << 0);
                CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi Dhcp range subnet mask\n");
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }
            iRestoreFlag |= (1 << 4);
            cFlag |= (1 << 2);
        }
        strncpy(sCurrentLanConfig.aLanSubnetMask, pLanConfigParams->lan_subnet_mask, (sizeof(sCurrentLanConfig.aLanSubnetMask)-1));
    }

    if(false == pLanConfigParams->mwenable)
    {
        sCurrentLanConfig.bDhcpServerEnable = false;
        iRestoreFlag |= (1 << 5);
        cFlag |= (1 << 3);
    }
    else if (pLanConfigParams->dhcp_server_enable != sBackupLanConfig.bDhcpServerEnable)
    {
        snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolEnable, sManageWiFiInfo.aBridgeIndex);
        if (true == pLanConfigParams->dhcp_server_enable)
            snprintf(aParamVal, BUFF_LEN_64,"true");
        else if (false == pLanConfigParams->dhcp_server_enable)
            snprintf(aParamVal, BUFF_LEN_64,"false");
        if( 0 != psmSet(aParamName, aParamVal))
        {
            iRestoreFlag |= (1 << 0);
            CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi Dhcp Server Enable\n");
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return pErrRetVal;
        }
        sCurrentLanConfig.bDhcpServerEnable = pLanConfigParams->dhcp_server_enable;
        iRestoreFlag |= (1 << 5);
        cFlag |= (1 << 3);
    }
    else
        sCurrentLanConfig.bDhcpServerEnable = pLanConfigParams->dhcp_server_enable;

    if (NULL != pLanConfigParams->dhcp_start_ip_address)
    {
        if (strcmp(pLanConfigParams->dhcp_start_ip_address, sBackupLanConfig.aDhcpStartIpAdd))
        {
            snprintf(aParamName,BUFF_LEN_64, l3netDhcpV4PoolMinAddr,sManageWiFiInfo.aBridgeIndex);
            if( 0 != psmSet(aParamName, pLanConfigParams->dhcp_start_ip_address))
            {
                iRestoreFlag |= (1 << 0);
                CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi Dhcp start ip addr\n");
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }
            iRestoreFlag |= (1 << 6);
            cFlag |= (1 << 3);
        }
        strncpy(sCurrentLanConfig.aDhcpStartIpAdd, pLanConfigParams->dhcp_start_ip_address, (sizeof(sCurrentLanConfig.aDhcpStartIpAdd)-1));
    }

    if (NULL != pLanConfigParams->dhcp_end_ip_address)
    {
        if (strcmp(pLanConfigParams->dhcp_end_ip_address, sBackupLanConfig.aDhcpEndIpAdd))
        {
            snprintf(aParamName,BUFF_LEN_64, l3netDhcpV4PoolMaxAddr,sManageWiFiInfo.aBridgeIndex);
            if( 0 != psmSet(aParamName, pLanConfigParams->dhcp_end_ip_address))
            {
                iRestoreFlag |= (1 << 0);
                CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi Dhcp end ip addr\n");
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }
            iRestoreFlag |= (1 << 7);
            cFlag |= (1 << 3);
        }
        strncpy(sCurrentLanConfig.aDhcpEndIpAdd, pLanConfigParams->dhcp_end_ip_address, (sizeof(sCurrentLanConfig.aDhcpEndIpAdd)-1));
    }
    if (NULL != pLanConfigParams->leasetime)
    {
        if (strcmp(pLanConfigParams->leasetime, sBackupLanConfig.aLeaseTime))
        {
            snprintf(aParamName,BUFF_LEN_64, l3netDhcpV4PoolLeaseTime,sManageWiFiInfo.aBridgeIndex);
            if( 0 != psmSet(aParamName, pLanConfigParams->leasetime))
            {
                iRestoreFlag |= (1 << 0);
                CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi Dhcp lease time\n");
                pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return pErrRetVal;
            }
            iRestoreFlag |= (1 << 8);
            cFlag |= (1 << 3);
        }
        strncpy(sCurrentLanConfig.aLeaseTime, pLanConfigParams->leasetime, (sizeof(sCurrentLanConfig.aLeaseTime)-1));
    }
    if (false == pLanConfigParams->mwenable)
    {
        sCurrentLanConfig.bIpv6Enable = false;
        iRestoreFlag |= (1 << 9);
        cFlag |= (1 << 4);
    }
    else if (pLanConfigParams->ipv6enable != sBackupLanConfig.bIpv6Enable)
    {
        snprintf(aParamName, BUFF_LEN_64, l3netIPv6Enable, sManageWiFiInfo.aBridgeIndex);
        if (true == pLanConfigParams->ipv6enable)
            snprintf(aParamVal,BUFF_LEN_64, "true");
        else if (false == pLanConfigParams->ipv6enable)
            snprintf(aParamVal,BUFF_LEN_64, "false");
        if( 0 != psmSet(aParamName, aParamVal))
        {
            iRestoreFlag |= (1 << 0);
            CcspTraceError(("%s: psm Set failed\n",__FUNCTION__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"PSM set failed for Manage WiFi IPv6 Enable\n");
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return pErrRetVal;
        }
        sCurrentLanConfig.bIpv6Enable = pLanConfigParams->ipv6enable;
        iRestoreFlag |= (1 << 9);
        cFlag |= (1 << 4);
    }
    else
        sCurrentLanConfig.bIpv6Enable = pLanConfigParams->ipv6enable;

    processManageWifiData (&sCurrentLanConfig, cFlag, pErrRetVal);
    if (NULL != pErrRetVal)
    {
        if (BLOB_EXEC_SUCCESS != pErrRetVal->ErrorCode)
            iRestoreFlag |= (1 << 0);
    }
    return pErrRetVal;
}

void processManageWifiData(backupLanconfig_t * pLanConfig, char cFlag, pErr pErrRetVal)
{
    pthread_t manageWifiBridgeThreadId;
    struct timespec abs_time = {0};
    int pthreadRetValue=0, err = 0;
    char buf[BUFF_LEN_128] = {'\0'};

    if ((NULL == pLanConfig) || (NULL == pErrRetVal))
        return;

    if (cFlag & (1 << 4))
    {
        CcspTraceInfo(("%s:%d, ipv6enable is enabled =%d\n", __FUNCTION__,__LINE__,pLanConfig->bIpv6Enable));
        if (true == pLanConfig->bIpv6Enable)
        {
            syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf));
            if (('\0' != sManageWiFiInfo.aBridgeName[0]) && (NULL == strstr(buf, sManageWiFiInfo.aBridgeName)))
                append_interface(sManageWiFiInfo.aBridgeName);
        }
        if (false == pLanConfig->bIpv6Enable)
        {
            syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf));
            if (('\0' != sManageWiFiInfo.aBridgeName[0]) && (NULL != strstr(buf, sManageWiFiInfo.aBridgeName)))
                remove_interface(sManageWiFiInfo.aBridgeName);
        }
    }

    if (cFlag & (1 << 0))
    {
        threadStruct_t sThread = {false, 0};
        CcspTraceInfo(("%s:%d, bMwEnable :%d\n",__FUNCTION__,__LINE__,pLanConfig->bMwEnable));
        pthread_condattr_init(&manageWifi_attr);
        pthread_condattr_setclock(&manageWifi_attr, CLOCK_MONOTONIC);
        pthread_cond_init(&manageWifi_exec_completed, &manageWifi_attr);

        sThread.bMwEnable = pLanConfig->bMwEnable;
        sThread.cFlag = cFlag;
        pthreadRetValue = pthread_create(&manageWifiBridgeThreadId, NULL, &ManageWiFiBridgeCreationThread, (void*)&sThread);

        if (true == pLanConfig->bMwEnable)
        {
            syscfg_set_commit(NULL, "Manage_WiFi_Enabled", "true");

            CcspTraceInfo(("%s: Setting the multinet-up %s\n",__FUNCTION__,sManageWiFiInfo.aBridgeIndex));
            if (0 != commonSyseventSet("multinet-up", sManageWiFiInfo.aBridgeIndex))
            {
                CcspTraceError(("%s: commonSyseventSet Failed for multinet-Up %s\n", __FUNCTION__, sManageWiFiInfo.aBridgeIndex));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"commonSyseventSet Failed For Multinet-Up \n");
                pErrRetVal->ErrorCode = SYSEVENT_FAILURE;
                if (0 == pthreadRetValue)
                {
                    CcspTraceInfo(("%s:%d, Timedout Cancelling the ManageWiFiThread\n",__FUNCTION__,__LINE__));
                    pthread_cancel(manageWifiBridgeThreadId);
                }
                return ;
            }
        }
        if (false == pLanConfig->bMwEnable)
        {
            syscfg_set_commit(NULL, "Manage_WiFi_Enabled", "false");

            if (0 != commonSyseventSet("multinet-down", sManageWiFiInfo.aBridgeIndex))
            {
                CcspTraceError(("%s: sysevnt_set Failed for multinet-down sysevent %s\n", __FUNCTION__, sManageWiFiInfo.aBridgeIndex));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"commonSyseventSet Failed For Multinet-Down \n");
                pErrRetVal->ErrorCode = SYSEVENT_FAILURE;
                if (0 == pthreadRetValue)
                {
                    CcspTraceInfo(("%s:%d, Timedout Cancelling the ManageWiFiThread\n",__FUNCTION__,__LINE__));
                    pthread_cancel(manageWifiBridgeThreadId);
                }
                return ;
            }
        }
        if (0 == pthreadRetValue)
        {
            clock_gettime(CLOCK_MONOTONIC, &abs_time);
            abs_time.tv_sec += MAX_MANAGE_WIFI_BRIDGE_THREAD_EXEC_TIMEOUT * 2;
            abs_time.tv_nsec += 0;

            pthread_mutex_lock(&manageWifi_exec);
            err = pthread_cond_timedwait(&manageWifi_exec_completed, &manageWifi_exec, &abs_time);
            if (err == ETIMEDOUT)
            {
                CcspTraceInfo(("%s:%d, Timedout Cancelling the ManageWiFiThread\n",__FUNCTION__,__LINE__));
                pthread_cancel(manageWifiBridgeThreadId);
            }
            pthread_mutex_unlock(&manageWifi_exec);
        }
        CcspTraceInfo(("%s:%d, Restarting the firewall\n",__FUNCTION__,__LINE__));
        if (0 != commonSyseventSet("firewall-restart", sManageWiFiInfo.aBridgeIndex))
        {
            CcspTraceError(("%s: commonSyseventSet Failed for Firewall-restart\n", __FUNCTION__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"commonSyseventSet Failed For Firewall-restart\n");
            pErrRetVal->ErrorCode = SYSEVENT_FAILURE;
            return ;
        }
    }

    if ((!(cFlag & (1 << 0))) && ((cFlag & (1 << 3)) || (cFlag & (1 << 2))))
    {
        CcspTraceInfo(("%s:%d, Restarting the dhcp_server\n",__FUNCTION__,__LINE__));
        int fd = creat("/var/tmp/lan_not_restart", O_WRONLY|O_CREAT);
        if (-1 != fd)
            close(fd);
        else
            CcspTraceError(("%s:%d, Failed to create the /var/tmp/lan_not_restart file\n",__FUNCTION__,__LINE__));
        if (0 != commonSyseventSet("dhcp_server-restart", ""))
        {
            CcspTraceError(("%s:%d, commonSyseventSet failed for dhcp_server-restart\n",__FUNCTION__,__LINE__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Failed to set dhcp_server-restart Sysevent\n");
            pErrRetVal->ErrorCode = SYSEVENT_FAILURE;
            return ;
        }
    }
    if ((!(cFlag & (1 << 0))) && (cFlag & (1 << 4)))
    {
        CcspTraceInfo(("%s:%d, Restarting the zebra\n",__FUNCTION__,__LINE__));
        if (0 != commonSyseventSet("zebra-restart", ""))
        {
            CcspTraceError(("%s:%d, commonSyseventSet failed for zebra-restart\n",__FUNCTION__,__LINE__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"sysevent set failed for zebra-restart \n");
            pErrRetVal->ErrorCode = SYSEVENT_FAILURE;
            return ;
        }
    }
    if ((!(cFlag & (1 << 0))) && ((cFlag & (1 << 1)) || (cFlag & (1 << 2))))
    {
        char aLanIpAddress [BUFF_LEN_32] = {'\0'};
        char aLanSubnetMask[BUFF_LEN_32] = {'\0'};
        CcspTraceInfo(("%s:%d, bridge:%s, Current lan IP:%s and current Lan subnetMask:%s\n",__FUNCTION__,__LINE__,sManageWiFiInfo.aBridgeName,pLanConfig->aLanIpAddr, pLanConfig->aLanSubnetMask));
        if ('\0' == pLanConfig->aLanIpAddr[0])
        {
            strncpy(aLanIpAddress, sBackupLanConfig.aLanIpAddr, (sizeof(aLanIpAddress)-1));
            CcspTraceInfo(("%s:%d, lan IP:%s \n",__FUNCTION__,__LINE__,aLanIpAddress));
        }
        else
            strncpy(aLanIpAddress, pLanConfig->aLanIpAddr, (sizeof(aLanIpAddress)-1));
        if ('\0'== pLanConfig->aLanSubnetMask[0])
        {
            strncpy(aLanSubnetMask, sBackupLanConfig.aLanSubnetMask, (sizeof(aLanSubnetMask)-1));
            CcspTraceInfo(("%s:%d, lan subnetMask :%s \n",__FUNCTION__,__LINE__,aLanSubnetMask));
        }
        else
            strncpy(aLanSubnetMask, pLanConfig->aLanSubnetMask, (sizeof(aLanSubnetMask)-1));
        CcspTraceInfo(("%s:%d Updating the IP, bridge:%s, IP:%s and subnetMask:%s\n",__FUNCTION__,__LINE__,sManageWiFiInfo.aBridgeName,aLanIpAddress, aLanSubnetMask));
#ifdef CORE_NET_LIB
        interface_up(sManageWiFiInfo.aBridgeName);
        if (0 != addr_add_va_arg("%s/%s dev %s",aLanIpAddress, aLanSubnetMask,sManageWiFiInfo.aBridgeName))
#else
        if (0 != v_secure_system("ifconfig %s %s netmask %s up",sManageWiFiInfo.aBridgeName,aLanIpAddress, aLanSubnetMask))
#endif /* CORE_NET_LIB */
        {
            CcspTraceError(("%s:%d, Failed to udpate the IP and subnetmask\n",__FUNCTION__,__LINE__));
            snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Failed to update the IP and SubnetMask\n");
            pErrRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return ;
        }
    }
    return;
}

void getBridgeDetailsFromPsm(void)
{
    char aParamName[BUFF_LEN_64]  = {0};

    /* Update key of manage wifi structure*/
    strncpy(sManageWiFiInfo.aKey, "ManagedWifi:", sizeof(sManageWiFiInfo.aKey)-1);

    psmGet(MANAGE_WIFI_BRIDGE_INDEX, sManageWiFiInfo.aBridgeIndex, BUFF_LEN_8);
    CcspTraceInfo(("%s: aBridgeIndex=%s\n", __FUNCTION__,sManageWiFiInfo.aBridgeIndex));
    if ('\0' == sManageWiFiInfo.aBridgeIndex[0])
    {
        strncpy(sManageWiFiInfo.aBridgeIndex,"17", sizeof(sManageWiFiInfo.aBridgeIndex)-1);
    }

    snprintf(aParamName, BUFF_LEN_64, l2netBridgeName, sManageWiFiInfo.aBridgeIndex);
#if !defined(_64BIT_ARCH_SUPPORT_)
    CcspTraceInfo(("%s: aBridgeName=%d\n", __FUNCTION__,sizeof(sManageWiFiInfo.aBridgeName)));
#else
    CcspTraceInfo(("%s: aBridgeName=%zu\n", __FUNCTION__,sizeof(sManageWiFiInfo.aBridgeName)));
#endif
    CcspTraceInfo(("%s: aParamName=%s\n", __FUNCTION__,aParamName));

    /* CID 347167 Unchecked return value fix */
    int ret = psmGet(aParamName, sManageWiFiInfo.aBridgeName, BUFF_LEN_32);
    if (ret != 0) {
        CcspTraceError(("%s:%d, Failed to get bridge details\n",__FUNCTION__,__LINE__));
    } else {
        CcspTraceInfo(("%s: aBridgeName=%s\n", __FUNCTION__,sManageWiFiInfo.aBridgeName));
    }
    if ('\0' == sManageWiFiInfo.aBridgeName[0])
    {
        strncpy(sManageWiFiInfo.aBridgeName,"brlan15", sizeof(sManageWiFiInfo.aBridgeName)-1);
    }
}

void getManageWiFiDetails(ManageWiFiInfo_t * pManageWifiInfo)
{
    if (NULL == pManageWifiInfo)
    {
        CcspTraceError(("%s:%d pManageWifiInfo is NULL\n",__FUNCTION__,__LINE__));
        return;
    }
    if ((true == sBackupLanConfig.bMwEnable) || (true == sCurrentLanConfig.bMwEnable))
    {
        if ('\0' != sManageWiFiInfo.aBridgeName[0])
        {
            strncpy(pManageWifiInfo->aBridgeName,sManageWiFiInfo.aBridgeName, (sizeof(pManageWifiInfo->aBridgeName)-1));
            pManageWifiInfo->aBridgeName[sizeof(pManageWifiInfo->aBridgeName) - 1] = '\0';
            CcspTraceInfo(("%s:%d pManageWifiInfo->aBridgeName:%s\n",__FUNCTION__,__LINE__,pManageWifiInfo->aBridgeName));
        }

        if ('\0' != sManageWiFiInfo.aWiFiInterfaces[0])
        {
            /* CID 347175 : fix */
            strncpy(pManageWifiInfo->aWiFiInterfaces, sManageWiFiInfo.aWiFiInterfaces, (sizeof(pManageWifiInfo->aWiFiInterfaces)-1));
            pManageWifiInfo->aWiFiInterfaces[sizeof(pManageWifiInfo->aWiFiInterfaces) - 1] = '\0';
            CcspTraceInfo(("%s:%d pManageWifiInfo->aWiFiInterfaces:%s\n",__FUNCTION__,__LINE__,pManageWifiInfo->aWiFiInterfaces));
        }
    }
    strncpy(pManageWifiInfo->aKey,sManageWiFiInfo.aKey, (sizeof(pManageWifiInfo->aKey)-1));
    pManageWifiInfo->aKey[sizeof(pManageWifiInfo->aKey) - 1] = '\0';
    CcspTraceInfo(("%s:%d pManageWifiInfo->aKey:%s\n",__FUNCTION__,__LINE__,pManageWifiInfo->aKey));
}

void setManageWiFiDetails(ManageWiFiInfo_t * pManageWifiInfo)
{
    if (NULL != pManageWifiInfo)
    {
        CcspTraceInfo(("%s:%d pManageWifiInfo->aKey:%s\n",__FUNCTION__,__LINE__,pManageWifiInfo->aKey));
        CcspTraceInfo(("%s:%d pManageWifiInfo->aBridgeName:%s\n",__FUNCTION__,__LINE__,pManageWifiInfo->aBridgeName));
        CcspTraceInfo(("%s:%d pManageWifiInfo->aWiFiInterfaces:%s\n",__FUNCTION__,__LINE__,pManageWifiInfo->aWiFiInterfaces));
        strncpy(sManageWiFiInfo.aKey, pManageWifiInfo->aKey,(sizeof(sManageWiFiInfo.aKey)-1));
        strncat(sManageWiFiInfo.aKey, ":", (sizeof(sManageWiFiInfo.aKey)-1));
        if (BRIDGE_NAME == pManageWifiInfo->eUpdateType)
            strncpy(sManageWiFiInfo.aBridgeName, pManageWifiInfo->aBridgeName, (sizeof(sManageWiFiInfo.aBridgeName)-1));
        if (WIFI_INTERFACES == pManageWifiInfo->eUpdateType)
            strncpy(sManageWiFiInfo.aWiFiInterfaces, pManageWifiInfo->aWiFiInterfaces, (sizeof(sManageWiFiInfo.aWiFiInterfaces)-1));
        CcspTraceInfo(("%s:%d sManageWiFiInfo.aKey:%s\n",__FUNCTION__,__LINE__,sManageWiFiInfo.aKey));
        CcspTraceInfo(("%s:%d sManageWiFiInfo.aBridgeName:%s\n",__FUNCTION__,__LINE__,sManageWiFiInfo.aBridgeName));
        CcspTraceInfo(("%s:%d sManageWiFiInfo.aWiFiInterfaces:%s\n",__FUNCTION__,__LINE__,sManageWiFiInfo.aWiFiInterfaces));
    }
}

rbusError_t notifyViaRbus(char * pTr181Param, char *pTr181ParamVal)
{
    int ret = RBUS_ERROR_BUS_ERROR ;
    rbusEvent_t event;
    rbusObject_t data;
    rbusValue_t value;

    rbusValue_Init(&value);
    rbusValue_SetString(value, pTr181ParamVal);

    rbusObject_Init(&data, NULL);
    rbusObject_SetValue(data, pTr181Param, value);

    event.name = pTr181Param;
    event.data = data;
    event.type = RBUS_EVENT_GENERAL;

    if (NULL == handle)
    {
        CcspTraceError(("%s: Rbus handler is NULL\n", __FUNCTION__));
        return ret;
    }
    /* Process the event publish*/
    ret = rbusEvent_Publish(handle, &event);
    if(ret != RBUS_ERROR_SUCCESS)
    {
        if (ret == RBUS_ERROR_NOSUBSCRIBERS)
        {
            ret = RBUS_ERROR_SUCCESS;
            CcspTraceError(("%s: No subscribers found\n", __FUNCTION__));
        }
        else
        {
            CcspTraceError(("Unable to Publish event data %s  rbus error code : %d\n",pTr181Param, ret));
        }
    }
    else
    {
        CcspTraceInfo(("%s : Publish to %s ret value is %d\n", __FUNCTION__,pTr181Param,ret));
    }
    /* release rbus value and object variable */
    rbusValue_Release(value);
    return ret;
}

int readLanConfigFromPSM(backupLanconfig_t *pBackupLanConfig)
{
    char aParamName[BUFF_LEN_64] = {'\0'};
    char aParamVal [BUFF_LEN_32] = {'\0'};
    if (NULL == pBackupLanConfig)
        return -1;

    snprintf(aParamName,BUFF_LEN_64,l2netBridgeEnable, sManageWiFiInfo.aBridgeIndex);
    if (0 != psmGet(aParamName,aParamVal,BUFF_LEN_32))
        return -1;
    /*CID: 347105 - Array compared against null - fixed*/
    if ( '\0' != aParamVal[0] )
    {
        if (!strncmp ("true",aParamVal, 4))
            pBackupLanConfig->bMwEnable = true;
        if (!strncmp ("false",aParamVal, 4))
            pBackupLanConfig->bMwEnable = false;
        CcspTraceInfo(("%s:%d, pBackupLanConfig->bMwEnable:%d\n",__FUNCTION__,__LINE__,pBackupLanConfig->bMwEnable));
    }
    snprintf(aParamName, BUFF_LEN_64, l2netAlias, sManageWiFiInfo.aBridgeIndex);
    if (0 != psmGet(aParamName, pBackupLanConfig->aAlias, sizeof(pBackupLanConfig->aAlias)))
        return -1;

    snprintf(aParamName, BUFF_LEN_64, l3netV4Addr, sManageWiFiInfo.aBridgeIndex);
    if (0 != psmGet(aParamName, pBackupLanConfig->aLanIpAddr, sizeof(pBackupLanConfig->aLanIpAddr)))
        return -1;

    snprintf(aParamName, BUFF_LEN_64, l2netV4SubnetMask, sManageWiFiInfo.aBridgeIndex);
    if (0 != psmGet(aParamName, pBackupLanConfig->aLanSubnetMask, sizeof(pBackupLanConfig->aLanSubnetMask)))
        return -1;

    snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolEnable, sManageWiFiInfo.aBridgeIndex);
    memset(aParamVal, 0, BUFF_LEN_32);
    if (0 != psmGet(aParamName, aParamVal, BUFF_LEN_32))
        return -1;

    if ('\0' != aParamVal)
    {
        if (!strncmp ("true",aParamVal, 4))
            pBackupLanConfig->bDhcpServerEnable= true;
        if (!strncmp ("false",aParamVal, 4))
            pBackupLanConfig->bDhcpServerEnable= false;
        CcspTraceInfo(("%s:%d, pBackupLanConfig->bDhcpServerEnable:%d\n",__FUNCTION__,__LINE__,pBackupLanConfig->bDhcpServerEnable));
    }

    snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolMinAddr, sManageWiFiInfo.aBridgeIndex);
    if (0 != psmGet(aParamName, pBackupLanConfig->aDhcpStartIpAdd, sizeof(pBackupLanConfig->aDhcpStartIpAdd)))
        return -1;

    snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolMaxAddr, sManageWiFiInfo.aBridgeIndex);
    if (0 != psmGet(aParamName, pBackupLanConfig->aDhcpEndIpAdd, sizeof(pBackupLanConfig->aDhcpEndIpAdd)))
        return -1;

    snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolLeaseTime, sManageWiFiInfo.aBridgeIndex);
    if (0 != psmGet(aParamName, pBackupLanConfig->aLeaseTime, sizeof(pBackupLanConfig->aLeaseTime)))
        return -1;

    snprintf(aParamName, BUFF_LEN_64, l3netIPv6Enable, sManageWiFiInfo.aBridgeIndex);
    memset(aParamVal, 0, BUFF_LEN_32);
    if (0 != psmGet(aParamName, aParamVal, BUFF_LEN_32))
        return -1;

    /* CID 347105 Array compared against 0 : fix */
    if (aParamVal[0] != '\0')
    {
        if (!strncmp ("true",aParamVal, 4))
            pBackupLanConfig->bIpv6Enable= true;
        if (!strncmp ("false",aParamVal, 4))
            pBackupLanConfig->bIpv6Enable= false;
        CcspTraceInfo(("%s:%d, pBackupLanConfig->bIpv6Enable:%d\n",__FUNCTION__,__LINE__,pBackupLanConfig->bIpv6Enable));
    }

    snprintf(aParamName, BUFF_LEN_64,l2netWiFiMembers, sManageWiFiInfo.aBridgeIndex);
    if (0 != psmGet(aParamName, pBackupLanConfig->aWiFiInterfaces, sizeof(pBackupLanConfig->aWiFiInterfaces)))
        return -1;

    if ('\0' != pBackupLanConfig->aWiFiInterfaces[0])
        strncpy (sManageWiFiInfo.aWiFiInterfaces,pBackupLanConfig->aWiFiInterfaces, (sizeof(sManageWiFiInfo.aWiFiInterfaces)-1));
    return 0;
}

#if defined(AMENITIES_NETWORK_ENABLED)
int readTunnelDetailsFromPsm(int iBridgeCount)
{
    char aParamName[BUFF_LEN_64] = {'\0'};
    char aParamVal[BUFF_LEN_32] = {'\0'};
    char aBkpupBridgeIndex[BUFF_LEN_64] = VAP_NAME_2G_INDEX;
    int iRetVal = 0;

    sBackupAmenityBridgeDetails.iBridgeCount = iBridgeCount;

    if (NULL == sBackupAmenityBridgeDetails.pBridgeInfoStruct)
    {
          sBackupAmenityBridgeDetails.pBridgeInfoStruct = (amenityBridgeInfo_t *)malloc(sizeof(amenityBridgeInfo_t) * iBridgeCount);
          if (NULL == sBackupAmenityBridgeDetails.pBridgeInfoStruct)
          {
               CcspTraceError(("%s:%d, Failed to allocate memory for bridgeInfoStruct\n",__FUNCTION__,__LINE__));
               return -1;
          }
    }
    memset(sBackupAmenityBridgeDetails.pBridgeInfoStruct, 0, sizeof(amenityBridgeInfo_t) * iBridgeCount);
    for (int iCount = 0; iCount < iBridgeCount; iCount++)
    {
        if (iCount == 1)
        {
            memset(aBkpupBridgeIndex,0,sizeof(aBkpupBridgeIndex));
            memcpy(aBkpupBridgeIndex, VAP_NAME_5G_INDEX, sizeof(VAP_NAME_5G_INDEX));
        }
        else if (iCount == 2)
        {
            memset(aBkpupBridgeIndex,0,sizeof(aBkpupBridgeIndex));
            memcpy(aBkpupBridgeIndex, VAP_NAME_6G_INDEX, sizeof(VAP_NAME_6G_INDEX));
        }

        if (0 != psmGet(aBkpupBridgeIndex, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex, sizeof(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex)))
        {
            CcspTraceError(("%s:%d, Failed to get bridge index\n",__FUNCTION__,__LINE__));
            iRetVal = -1;
        }

        snprintf(aParamName, BUFF_LEN_64, l2netVapName, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex);
        if (0 != psmGet(aParamName, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName, sizeof(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName)))
        {
            CcspTraceError(("%s:%d, Failed to get vap name\n",__FUNCTION__,__LINE__));
            iRetVal = -1;
        }

        snprintf(aParamName, BUFF_LEN_64, l2netWiFiMembers, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex);
        if (0 != psmGet(aParamName, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cWiFiInterface, sizeof(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cWiFiInterface)))
        {
            CcspTraceError(("%s:%d, Failed to get wifi members\n",__FUNCTION__,__LINE__));
            iRetVal = -1;
        }

        snprintf(aParamName, BUFF_LEN_64, l2netBridgeName, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex);
        if (0 != psmGet(aParamName, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName, sizeof(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName)))
        {
            CcspTraceError(("%s:%d, Failed to get bridge name\n",__FUNCTION__,__LINE__));
            iRetVal = -1;
        }

        snprintf(aParamName, BUFF_LEN_64, l2netVlanID, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeIndex);
        if (0 != psmGet(aParamName, aParamVal, sizeof(aParamVal)))
        {
            CcspTraceError(("%s:%d, Failed to get bridge vlan id\n",__FUNCTION__,__LINE__));
            iRetVal = -1;
        }
        else
        {
            sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId = atoi(aParamVal);
            CcspTraceInfo(("%s:%d, VlanId:%d\n",__FUNCTION__,__LINE__,sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId));
        }

    }
    return iRetVal;
}
#endif

void initManageWiFiBacupStruct(void)
{
    char aManageWiFiEnabled[BUFF_LEN_8] = {0};

    syscfg_get(NULL, "Manage_WiFi_Enabled", aManageWiFiEnabled, BUFF_LEN_8);

    getBridgeDetailsFromPsm();
    if ((!strncmp(aManageWiFiEnabled, "true", 4)) || (!strncmp(aManageWiFiEnabled, "false", 4)))
    {
        if (0 != readLanConfigFromPSM(&sBackupLanConfig))
            CcspTraceError(("%s:%d, Failed to read Manage Wifi Config from PSM\n",__FUNCTION__,__LINE__));
    }
}
#if defined(AMENITIES_NETWORK_ENABLED)

// Helper: Register for sysevent notifications for a bridge
static void registerMultinetNotification(int iFd, token_t token, const char *pBridgeIndex, async_id_t *pAsyncId)
{
    char cStatusName[BUFF_LEN_64] = {0};
    snprintf(cStatusName, sizeof(cStatusName), "multinet_%s-status", pBridgeIndex);
    sysevent_set_options(iFd, token, cStatusName, TUPLE_FLAG_EVENT);
    sysevent_setnotification(iFd, token, cStatusName, pAsyncId);
}

// Helper: Check if any flag in mask is set
static BOOL isAnyFlagSet(uint16_t ui16Flags, uint16_t ui16Mask)
{
    return (ui16Flags & ui16Mask) ? TRUE : FALSE;
}

// Helper: Handle notification for a single bridge
static void handleBridgeNotification(
    amenityBridgeInfo_t *pBridge,
    uint16_t *pFlag,
    const char *pNotifName,
    const char *pNotifVal)
{
    char cStatusName[BUFF_LEN_64] = {0};
    char cParamName[BUFF_LEN_64] = {0};
    char cVlanId[BUFF_LEN_8] = {0};

    snprintf(cStatusName, sizeof(cStatusName), "multinet_%s-status", pBridge->cBridgeIndex);
    if (strncmp(pNotifName, cStatusName, strlen(pNotifName)) != 0)
        return;

    CcspTraceInfo(("%s:%d,Received the Notification :%s:%s\n",__FUNCTION__,__LINE__, cStatusName,pNotifVal));
    for (int iCount = 0; iCount < (int)(sizeof(bridgeBands)/sizeof(bridgeBands[0])); ++iCount)
    {
        const BridgeBandInfo *pBand = &bridgeBands[iCount];
        if (!strncmp(pBridge->cVapName, pBand->vapName, strlen(pBand->vapName)))
        {
            // Handle "stopped"
            if (isAnyFlagSet(*pFlag, pBand->bitDown | pBand->bitDisable) && !strncmp(pNotifVal, "stopped", strlen(pNotifVal)))
            {
                CcspTraceInfo(("%s:%d,Received the stopped multinet status for %s\n",__FUNCTION__,__LINE__,pBand->vapName));
                //Unset the Bridge Disable bit
                *pFlag &= ~pBand->bitDisable;
                //Vlan Id change
                if (*pFlag & pBand->bitDown)
                {
                    *pFlag &= ~pBand->bitDown;
                    if (*pFlag & pBand->bitUp)
                    {
                        snprintf(cParamName, sizeof(cParamName), l2netVlanID, pBridge->cBridgeIndex);
                        snprintf(cVlanId, sizeof(cVlanId), "%d", pBridge->iVlanId);
                        if (0!= psmSet(cParamName, cVlanId))
                            CcspTraceError(("%s:%d, psmSet failed for %s\n",__FUNCTION__,__LINE__,cParamName));
                        if (0 != commonSyseventSet("multinet-up", pBridge->cBridgeIndex))
                            CcspTraceError(("%s:%d, commonSyseventSet failed for %s\n",__FUNCTION__,__LINE__,pBridge->cBridgeIndex));
                    }
                }
            }
            // Handle "ready"
            if (isAnyFlagSet(*pFlag, pBand->bitUp | pBand->bitEnable) && !strncmp(pNotifVal, "ready", strlen(pNotifVal)))
            {
                CcspTraceInfo(("%s:%d,Received the ready multinet status for %s\n",__FUNCTION__,__LINE__,pBand->vapName));
                *pFlag &= ~pBand->bitEnable;
                *pFlag &= ~pBand->bitUp;
            }
            break;
        }
    }
}

void *amenityMultinetNotifyHandler(void *vArg)
{
    int iOldType;
    int iErr;
    async_id_t Asyncid;
    char cName[25] = {0}, cVal[42] = {0};
    int iNameLen, iValLen;

    AmenityThread_t *pAmenityThread = (AmenityThread_t*)vArg;
    amenityBridgeDetails_t *pAmenityBridgeInfo = *(pAmenityThread->ppAmenityDetails);
    uint16_t ui16Flag = pAmenityThread->ui16Flag;
    /*
     * 0th bit : Enable 2G
     * 1st bit : Enable 5G
     * 2nd bit : Enable 6G
     *---------Above bits are set to Enable the respective bridges ------------*
     * 3rd bit : Disable 2G
     * 4th bit : Disable 5G
     * 5th bit : Disable 6G
     *---------Above bits are set to Disable the respective bridges ------------*
     * 6th bit : 2G Down --> Make the old bridge down
     * 7th bit : 2G Up --> bring the new bridge up
     * 8th bit : 5G Down --> Make the old bridge down
     * 9th bit : 5G Up --> bring the new bridge up
     * 10th bit : 6G Down --> Make the old bridge down
     * 11th bit : 6G Up --> bring the new bridge up
     *---------Above bits are set When there is a change in Vlan ID------------*/

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &iOldType);
    pthread_detach(pthread_self());

    CcspTraceInfo(("%s:%d pthread to Register Amenity network multinet sysevents \n",__FUNCTION__,__LINE__));
    iSyseventANFd = sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION,  "AmenityMultinetStatus", &SyseventANToken);
    if (iSyseventANFd < 0)
    {
        CcspTraceError(("%s:%d, sysevent_open failed\n",__FUNCTION__,__LINE__));
        return NULL;
    }

    // Register notifications for all bridges
    for (int i = 0; i < pAmenityBridgeInfo->iBridgeCount; i++)
    {
        amenityBridgeInfo_t *pBridge = &pAmenityBridgeInfo->pBridgeInfoStruct[i];
        if (pBridge->cBridgeIndex && pBridge->cBridgeIndex[0] != '\0')
        {
            BOOL bShouldSet = FALSE;
            if (!strncmp(pBridge->cVapName, VAP_NAME_2G, sizeof(VAP_NAME_2G)))
                bShouldSet = isAnyFlagSet(ui16Flag, BIT_2G_DOWN_MASK | BIT_2G_UP_MASK | BIT_2G_ENABLE_MASK | BIT_2G_DISABLE_MASK);
            else if (!strncmp(pBridge->cVapName, VAP_NAME_5G, sizeof(VAP_NAME_5G)))
                bShouldSet = isAnyFlagSet(ui16Flag, BIT_5G_DOWN_MASK | BIT_5G_UP_MASK | BIT_5G_ENABLE_MASK | BIT_5G_DISABLE_MASK);
            else if (!strncmp(pBridge->cVapName, VAP_NAME_6G, sizeof(VAP_NAME_6G)))
                bShouldSet = isAnyFlagSet(ui16Flag, BIT_6G_DOWN_MASK | BIT_6G_UP_MASK | BIT_6G_ENABLE_MASK | BIT_6G_DISABLE_MASK);

            if (bShouldSet)
                registerMultinetNotification(iSyseventANFd, SyseventANToken, pBridge->cBridgeIndex, &Asyncid);
        }
    }
    async_id_t getnotification_asyncid;
    while (1)
    {
        memset(cName, 0, sizeof(cName));
        memset(cVal, 0, sizeof(cVal));
        iNameLen = sizeof(cName);
        iValLen = sizeof(cVal);

        iErr = sysevent_getnotification(iSyseventANFd, SyseventANToken, cName, &iNameLen, cVal, &iValLen, &getnotification_asyncid);

        if (iErr)
        {
            CcspTraceWarning(("sysevent_getnotification failed with error: %d %s\n", iErr, __FUNCTION__));
            CcspTraceWarning(("sysevent_getnotification failed name: %s val : %s\n", cName, cVal));
            if (0 != v_secure_system("pidof syseventd"))
            {
                CcspTraceWarning(("%s syseventd not running, breaking the receive notification loop \n", __FUNCTION__));
                break;
            }
        }
        else
        {
            for (int iCount = 0; iCount < pAmenityBridgeInfo->iBridgeCount; iCount++)
            {
                handleBridgeNotification(&pAmenityBridgeInfo->pBridgeInfoStruct[iCount], &ui16Flag, cName, cVal);
            }
            if ((0 == ui16Flag) && (0 < iSyseventANFd))
            {
                sysevent_close(iSyseventANFd, SyseventANToken);
                pthread_cond_signal(&amenityWifi_exec_completed);
                break;
            }
        }
    }
    return NULL;
}

void lanTunnelPsmClear(char *pBridgeIndex)
{
    char cParamName[BUFF_LEN_64] = {'\0'};
    char cVlanId[BUFF_LEN_8] = {'\0'};

    if(NULL == pBridgeIndex)
    {
        CcspTraceError(("%s:%d, NULL pointer passed \n",__FUNCTION__,__LINE__));
        return;
    }
    snprintf (cParamName, BUFF_LEN_64, l2netVlanID, pBridgeIndex);
    snprintf(cVlanId, BUFF_LEN_8, "0");
    CcspTraceInfo(("%s:%d, l2netVlanID is cParamName:%s\n",__FUNCTION__,__LINE__,cParamName));
    CcspTraceInfo(("%s:%d, cVlanId:%s\n",__FUNCTION__,__LINE__,cVlanId));
    if (0 != psmSet (cParamName, cVlanId))
    {
        CcspTraceError(("%s:%d, psmSet failed for %s\n",__FUNCTION__,__LINE__,cParamName));
    }
    snprintf(cParamName, BUFF_LEN_64, l2netVapName, pBridgeIndex);
    CcspTraceInfo(("%s:%d, l2netVapName cParamName:%s\n",__FUNCTION__,__LINE__,cParamName));
    if (0 != psmSet (cParamName, ""))
    {
        CcspTraceError(("%s:%d, psmSet failed for %s\n",__FUNCTION__,__LINE__,cParamName));
    }


}

int rollbackTunnelLanconfig(void)
{
    pErr pErrRetVal = (pErr) malloc(sizeof(Err));
    int retVal = 0;
    uint16_t ui16Flag = 0;
    /*
     * 0th bit : Enable 2G
     * 1st bit : Enable 5G
     * 2nd bit : Enable 6G
     *---------Above bits are set to Enable the respective bridges ------------*
     * 3rd bit : Disable 2G
     * 4th bit : Disable 5G
     * 5th bit : Disable 6G
     *---------Above bits are set to Disable the respective bridges ------------*
     * 6th bit : 2G Down --> Make the old bridge down
     * 7th bit : 2G Up --> bring the new bridge up
     * 8th bit : 5G Down --> Make the old bridge down
     * 9th bit : 5G Up --> bring the new bridge up
     * 10th bit : 6G Down --> Make the old bridge down
     * 11th bit : 6G Up --> bring the new bridge up
     *---------Above bits are set When there is a change in Vlan ID------------*/

    if (NULL == pErrRetVal)
    {
        CcspTraceError(("%s,%d : malloc failed\n", __FUNCTION__, __LINE__));
        return -1;
    }
    memset(pErrRetVal, 0, sizeof(Err));
    pErrRetVal->ErrorCode = BLOB_EXEC_SUCCESS;


    char cAmenityNetworkEnabled[BUFF_LEN_8] = {0};
    char cManageWiFiEnabled[BUFF_LEN_8] = {0};
    CcspTraceInfo((" Entering %s \n", __FUNCTION__));

    syscfg_get(NULL, "Amenity_Network_Enabled", cAmenityNetworkEnabled, BUFF_LEN_8);
    syscfg_get(NULL, "Manage_WiFi_Enabled", cManageWiFiEnabled, BUFF_LEN_8);
    CcspTraceInfo(("%s:%d, Amenity_Network_Enabled:%s\n", __FUNCTION__, __LINE__, cAmenityNetworkEnabled));
    CcspTraceInfo(("%s:%d, Manage_WiFi_Enabled:%s\n", __FUNCTION__, __LINE__, cManageWiFiEnabled));
    CcspTraceInfo(("%s:%d: Backup - Amenity Bridge count :%d\n", __FUNCTION__, __LINE__, sBackupAmenityBridgeDetails.iBridgeCount));

    // If first amenity blob is failed
    if (0 == sBackupAmenityBridgeDetails.iBridgeCount && (strlen(cAmenityNetworkEnabled) == 0))
    {
        CcspTraceInfo(("%s:%d, Amenity_Network_Enabled is false\n", __FUNCTION__, __LINE__));
        if (sCurrAmenityBridgeDetails.iBridgeCount > 0)
        {
            for (int iCount = 0; iCount < sCurrAmenityBridgeDetails.iBridgeCount; iCount++)
            {
#ifdef CORE_NET_LIB
                if (interface_exist(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName) == CNL_STATUS_SUCCESS)
#else
                if (1 == isInterfaceUp (sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName))
#endif
                {
                    setBridgeDownFlag(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName, &ui16Flag);
                }
            }
            CcspTraceInfo(("%s:%d, calling processTunnelInfo with vlanflag=%d \n", __FUNCTION__, __LINE__, ui16Flag));
            if (0 != processTunnelInfo(&sCurrAmenityBridgeDetails, ui16Flag, pErrRetVal))
            {
                CcspTraceError(("%s:%d, Failed to process Tunnel Info, error code:%d\n", __FUNCTION__, __LINE__, pErrRetVal->ErrorCode));
            }
            else
            {
                // Clear the Data Model tree of Amenity
                for (int iCount = 0; iCount < iNumOfMappings; iCount++)
                {
                    const char *pVapName = vapMappings[iCount].pVapName;

                    for (int iVar = 0; iVar < sCurrAmenityBridgeDetails.iBridgeCount; iVar++)
                    {
                        if (!strncmp(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName, pVapName, sizeof(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iVar].cVapName)))
                        {
                            snprintf(pRbusBridgeInfo[iVar].cBridgeIndex, sizeof(pRbusBridgeInfo[iVar].cBridgeIndex), "0");
                            strncpy(pRbusBridgeInfo[iVar].cBridgeName, "", sizeof(pRbusBridgeInfo[iVar].cBridgeName) - 1);
                            strncpy(pRbusBridgeInfo[iVar].cVapName, "", sizeof(pRbusBridgeInfo[iVar].cVapName) - 1);
                            CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d].cBridgeIndex:%s\n", __FUNCTION__, __LINE__, iVar, pRbusBridgeInfo[iVar].cBridgeIndex));
                            CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d].cBridgeName:%s\n", __FUNCTION__, __LINE__, iVar, pRbusBridgeInfo[iVar].cBridgeName));
                            CcspTraceInfo(("%s:%d, pRbusBridgeInfo[%d].cVapName:%s\n", __FUNCTION__, __LINE__, iVar, pRbusBridgeInfo[iVar].cVapName));
                            break;
                        }
                    }
                }
                CcspTraceInfo(("%s:%d, Clear the psm entries\n", __FUNCTION__, __LINE__));
                // Clear PSM records for all amenity bridges after receiving all multinet down
                for (int iIndex = 0; iIndex < sCurrAmenityBridgeDetails.iBridgeCount; iIndex++)
                {
                    lanTunnelPsmClear(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iIndex].cBridgeIndex);
                }
            }
        }
        if (sBackupAmenityBridgeDetails.pBridgeInfoStruct != NULL)
            free(sBackupAmenityBridgeDetails.pBridgeInfoStruct);
        if (sCurrAmenityBridgeDetails.pBridgeInfoStruct != NULL)
            free(sCurrAmenityBridgeDetails.pBridgeInfoStruct);

        //First time, if the Amenity Network blob is failed, if Manage Wifi was enabled, rollback the config
        if (strlen(cAmenityNetworkEnabled) == 0 && !strncmp(cManageWiFiEnabled, "true", 4))
        {
            if (BLOB_EXEC_SUCCESS != rollbackLanconfig())
            {
                CcspTraceError(("%s:%d, Failed to rollback Manage Wifi Config from PSM\n", __FUNCTION__, __LINE__));
                free(pErrRetVal);
                pErrRetVal = NULL;
                return -1;
            }
            CcspTraceInfo(("%s:%d, rollback Manage Wifi Config from PSM\n", __FUNCTION__, __LINE__));
        }
    }
    // Rollback to previous config if Amenity_Network_Enabled is true
    else if ((0 != sBackupAmenityBridgeDetails.iBridgeCount) && (!strncmp(cAmenityNetworkEnabled, "true", 4)))
    {
        CcspTraceInfo(("%s:%d, Amenity_Network_Enabled is true\n", __FUNCTION__, __LINE__));
        for (int iCount = 0; iCount < sBackupAmenityBridgeDetails.iBridgeCount; iCount++)
        {
            CcspTraceInfo(("%s:%d, Backup config Vlan=%d \n", __FUNCTION__, __LINE__, sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId));
            CcspTraceInfo(("%s:%d, Current config Vlan=%d \n", __FUNCTION__, __LINE__, sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId));
            if (sBackupAmenityBridgeDetails.bIsAmenityEnabled && sCurrAmenityBridgeDetails.bIsAmenityEnabled)
            {
                //Check for VlanId change
                if (sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId != sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].iVlanId)
                {
                    setVlanChangeFlags(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName, iCount,&sBackupAmenityBridgeDetails ,&ui16Flag);
                }
            }
            // Disabling of Amenity Network failed, roll back to previous
            else if (sBackupAmenityBridgeDetails.bIsAmenityEnabled && !sCurrAmenityBridgeDetails.bIsAmenityEnabled)
            {
                setBridgeUpFlag(sBackupAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName, &ui16Flag);
            }
        }
        CcspTraceInfo(("%s:%d, calling processTunnelInfo with vlanflag=%d \n", __FUNCTION__, __LINE__, ui16Flag));
        if (0 != processTunnelInfo(&sBackupAmenityBridgeDetails, ui16Flag, pErrRetVal))
        {
            CcspTraceError(("%s:%d, Failed to process Tunnel Info, error code:%d\n", __FUNCTION__, __LINE__, pErrRetVal->ErrorCode));
        }
    }
    // Enabling of Amenity Network failed, Previously it was disabled
    else if ((!strncmp(cAmenityNetworkEnabled, "true", 4)) && !sBackupAmenityBridgeDetails.bIsAmenityEnabled && sCurrAmenityBridgeDetails.bIsAmenityEnabled)
    {
        for (int iCount = 0; iCount < sCurrAmenityBridgeDetails.iBridgeCount; iCount++)
        {
#ifdef CORE_NET_LIB
            if (interface_exist(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName) == CNL_STATUS_SUCCESS)
#else
            if (1 == isInterfaceUp (sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cBridgeName))
#endif
            {
                setBridgeDownFlag(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iCount].cVapName, &ui16Flag);
            }
        }
        CcspTraceInfo(("%s:%d, calling processTunnelInfo with vlanflag=%d \n", __FUNCTION__, __LINE__, ui16Flag));
        if (0 != processTunnelInfo(&sBackupAmenityBridgeDetails, ui16Flag, pErrRetVal))
        {
            CcspTraceError(("%s:%d, Failed to process Tunnel Info, error code:%d\n", __FUNCTION__, __LINE__, pErrRetVal->ErrorCode));
        }
        else
        {
            CcspTraceInfo(("%s:%d, Clear the psm entries\n", __FUNCTION__, __LINE__));
            //Clear PSM records for all amenity bridges after receiving all multinet down
            for (int iIndex = 0; iIndex < sCurrAmenityBridgeDetails.iBridgeCount; iIndex++)
            {
                lanTunnelPsmClear(sCurrAmenityBridgeDetails.pBridgeInfoStruct[iIndex].cBridgeIndex);
            }
            sCurrAmenityBridgeDetails.iBridgeCount = 0;
        }
    }
    if (NULL != pErrRetVal)
    {
        if (BLOB_EXEC_SUCCESS != pErrRetVal->ErrorCode)
        {
            retVal = pErrRetVal->ErrorCode;
        }
        free(pErrRetVal);
    }
    return retVal;
}
#endif

/* Callback function to rollback when Lan blob execution fails */
int rollbackLanconfig(void)
{
    pErr pErrRetVal = NULL;

    int retVal = 0;
    /*
     * 0th bit : Manage Wifi Enable
     * 1st bit : Lan IP addr
     * 2nd bit : lan subnet mask, dhcp V4 pool subnet mask
     * 3rd bit : DhcpV4Pool Enable, Start and End address, lease time
     * 4th bit : IPv6 Enable
     * 5th bit : alias
     */
    char cFlag = 0;

    pErrRetVal = (pErr) malloc (sizeof(Err));
    if (NULL == pErrRetVal)
    {
        CcspTraceError(("%s,%d : malloc failed\n",__FUNCTION__,__LINE__));
        return -1;
    }
    memset(pErrRetVal,0,sizeof(Err));
    pErrRetVal->ErrorCode = BLOB_EXEC_SUCCESS;

    CcspTraceInfo((" Entering %s \n",__FUNCTION__));
    if (sCurrentLanConfig.bMwEnable != sBackupLanConfig.bMwEnable)
        cFlag |= (1 << 0);

    if (strcmp(sCurrentLanConfig.aLanIpAddr, sBackupLanConfig.aLanIpAddr))
        cFlag |= (1 << 1);

    if (strcmp(sCurrentLanConfig.aLanSubnetMask, sBackupLanConfig.aLanSubnetMask))
        cFlag |= (1 << 2);

    if (sCurrentLanConfig.bDhcpServerEnable != sBackupLanConfig.bDhcpServerEnable)
        cFlag |= (1 << 3);

    if (strcmp(sCurrentLanConfig.aDhcpStartIpAdd, sBackupLanConfig.aDhcpStartIpAdd))
        cFlag |= (1 << 3);

    if (strcmp(sCurrentLanConfig.aDhcpEndIpAdd, sBackupLanConfig.aDhcpEndIpAdd))
        cFlag |= (1 << 3);

    if (strcmp(sCurrentLanConfig.aLeaseTime, sBackupLanConfig.aLeaseTime))
        cFlag |= (1 << 3);

    if (sCurrentLanConfig.bIpv6Enable != sBackupLanConfig.bIpv6Enable)
        cFlag |= (1 << 4);

    if (strcmp(sCurrentLanConfig.aAlias, sBackupLanConfig.aAlias))
        cFlag |= (1 << 5);

    processManageWifiData (&sBackupLanConfig, cFlag, pErrRetVal);
    restorePreviousPsmValue();
    if (NULL != pErrRetVal)
    {
        retVal = pErrRetVal->ErrorCode;
        free(pErrRetVal);
    }
    return retVal;
}

static int remSubstr(char *pMainStr, char *pSubstr)
{
    char *pStr1 = NULL,*pStr2 = NULL;
    int iLen = 0;

    if ((NULL == pMainStr) || (NULL == pSubstr))
        return -1;

    pStr1 = strstr(pMainStr,pSubstr);
    if(pStr1 != NULL)
    {
        iLen = strlen(pSubstr);
        pStr2 = pStr1 + iLen;
        while((*pStr1++ = *pStr2++));
    }
    else
        return -1;
    return 0;
}

int removeSubstring(char * pMainString, char * pSubstring, bool bExtraSpaceRemoval)
{
    char *pSubstr = NULL;
    int iRet = 0;
    if ((NULL == pMainString) || (NULL == pSubstring))
        return -1;

    if (true == bExtraSpaceRemoval)
    {
        int iLenOfSubstr = (strlen(pSubstring) + 2);
        pSubstr = (char*) malloc (iLenOfSubstr);
        if (NULL != pSubstr)
        {
            snprintf(pSubstr, iLenOfSubstr, "%s ",pSubstring);
            if ( 0 != remSubstr(pMainString, pSubstr))
            {
                memset(pSubstr, 0, iLenOfSubstr);
                snprintf(pSubstr, iLenOfSubstr, " %s",pSubstring);
                if ( 0 != remSubstr(pMainString, pSubstr))
                    iRet = -1;
                else
                    iRet = 0;
            }
            free(pSubstr);
        }
    }
    else
        iRet = remSubstr(pMainString, pSubstring);

    if (0 == iRet)
        printf("%s:%d, Substring found in MainString and deleted:%s\n",__FUNCTION__,__LINE__,pMainString);
    else
        printf("%s:%d, Substring:%s is not found in MainString:%s\n",__FUNCTION__,__LINE__,pSubstring,pMainString);
    return iRet;
}

void updateBackupConfig(void)
{
    char aParamName[BUFF_LEN_64] = {'\0'};

    sBackupLanConfig.bMwEnable = sCurrentLanConfig.bMwEnable;
    strncpy (sBackupLanConfig.aAlias, sCurrentLanConfig.aAlias, (sizeof(sBackupLanConfig.aAlias)-1));
    sBackupLanConfig.bDhcpServerEnable = sCurrentLanConfig.bDhcpServerEnable;
    strncpy (sBackupLanConfig.aLanIpAddr,sCurrentLanConfig.aLanIpAddr, (sizeof(sBackupLanConfig.aLanIpAddr)-1));
    strncpy (sBackupLanConfig.aLanSubnetMask,sCurrentLanConfig.aLanSubnetMask, (sizeof(sBackupLanConfig.aLanSubnetMask)-1));
    strncpy (sBackupLanConfig.aDhcpStartIpAdd,sCurrentLanConfig.aDhcpStartIpAdd, (sizeof(sBackupLanConfig.aDhcpStartIpAdd)-1));
    strncpy (sBackupLanConfig.aDhcpEndIpAdd,sCurrentLanConfig.aDhcpEndIpAdd, (sizeof(sBackupLanConfig.aDhcpEndIpAdd)-1));
    strncpy (sBackupLanConfig.aLeaseTime,sCurrentLanConfig.aLeaseTime, (sizeof(sBackupLanConfig.aLeaseTime)-1));
    sBackupLanConfig.bIpv6Enable = sCurrentLanConfig.bIpv6Enable;
    if (false == sCurrentLanConfig.bMwEnable)
    {
        snprintf(aParamName, BUFF_LEN_64, l2netBridgeEnable, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, "false");
        snprintf(aParamName, BUFF_LEN_64, l2netAlias, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName,sBackupLanConfig.aAlias);
        snprintf(aParamName, BUFF_LEN_64, l3netV4Addr, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aLanIpAddr);
        snprintf(aParamName, BUFF_LEN_64, l2netV4SubnetMask, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aLanSubnetMask);
        snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolSubnetMask, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aLanSubnetMask);
        snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolEnable, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, "false");
        snprintf(aParamName,BUFF_LEN_64, l3netDhcpV4PoolMinAddr,sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aDhcpStartIpAdd);
        snprintf(aParamName,BUFF_LEN_64, l3netDhcpV4PoolMaxAddr,sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aDhcpEndIpAdd);
        snprintf(aParamName,BUFF_LEN_64, l3netDhcpV4PoolLeaseTime,sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aLeaseTime);
        snprintf(aParamName, BUFF_LEN_64, l3netIPv6Enable, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, "false");
    }
}

void getManageWiFiEnable(BOOL * pWiFiEnable)
{
    if (NULL == pWiFiEnable)
        return;
    *pWiFiEnable = sBackupLanConfig.bMwEnable;
    CcspTraceInfo(("%s:%d, sBackupLanConfig.bMwEnable:%d\n",__FUNCTION__,__LINE__,sBackupLanConfig.bMwEnable));
    CcspTraceInfo(("%s:%d, *pWiFiEnable:%d\n",__FUNCTION__,__LINE__,*pWiFiEnable));
}

void publishEvent(char * pTr181Param, char *pTr181ParamVal, EventType eEventType)
{
    if ((NULL == pTr181Param) && (NULL == pTr181ParamVal))
        return;

    switch(eEventType)
    {
        case RBUS:
        {
            if (RBUS_ERROR_SUCCESS != notifyViaRbus(pTr181Param,pTr181ParamVal))
            {
                CcspTraceError(("%s:%d, Failed to publish the %s:%s via R-BUS\n",__FUNCTION__,__LINE__,pTr181Param, pTr181ParamVal));
            }
            break;
        }
        default:
            break;
    }
}

void restorePreviousPsmValue(void)
{
    char aParamName[BUFF_LEN_64]  = {0};
    char aParamVal[BUFF_LEN_64] = {0};

    if (iRestoreFlag & (1 << 1))
    {
        if (true == sBackupLanConfig.bMwEnable)
            snprintf(aParamVal, BUFF_LEN_64,"true");
        else if (false == sBackupLanConfig.bMwEnable)
            snprintf(aParamVal, BUFF_LEN_64,"false");
        snprintf(aParamName, BUFF_LEN_64, l2netBridgeEnable, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, aParamVal);
    }
    if (iRestoreFlag & (1 << 2))
    {
        snprintf(aParamName, BUFF_LEN_64, l3netV4Addr, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aLanIpAddr);
    }

    if (iRestoreFlag & ( 1 << 3))
    {
        snprintf(aParamName, BUFF_LEN_64, l2netV4SubnetMask, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aLanSubnetMask);
    }
    if (iRestoreFlag & ( 1 << 4))
    {
        snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolSubnetMask, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aLanSubnetMask);
    }
    if (iRestoreFlag & ( 1 << 5))
    {
        snprintf(aParamName, BUFF_LEN_64, l3netDhcpV4PoolEnable, sManageWiFiInfo.aBridgeIndex);
        if (true == sBackupLanConfig.bDhcpServerEnable)
            snprintf(aParamVal, BUFF_LEN_64,"true");
        else if (false == sBackupLanConfig.bDhcpServerEnable)
            snprintf(aParamVal, BUFF_LEN_64,"false");
        psmSet(aParamName, aParamVal);
    }
    if (iRestoreFlag & ( 1 << 6))
    {
        snprintf(aParamName,BUFF_LEN_64, l3netDhcpV4PoolMinAddr,sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aDhcpStartIpAdd);
    }
    if (iRestoreFlag & ( 1 << 7))
    {
        snprintf(aParamName,BUFF_LEN_64, l3netDhcpV4PoolMaxAddr,sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aDhcpEndIpAdd);
    }
    if (iRestoreFlag & ( 1 << 8))
    {
        snprintf(aParamName,BUFF_LEN_64, l3netDhcpV4PoolLeaseTime,sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aLeaseTime);
    }
    if (iRestoreFlag & ( 1 << 9))
    {
        snprintf(aParamName, BUFF_LEN_64, l3netIPv6Enable, sManageWiFiInfo.aBridgeIndex);
        if (true == sBackupLanConfig.bIpv6Enable)
            snprintf(aParamVal, BUFF_LEN_64,"true");
        else if (false == sBackupLanConfig.bIpv6Enable)
            snprintf(aParamVal, BUFF_LEN_64,"false");
        psmSet(aParamName, aParamVal);
    }
    if (iRestoreFlag & ( 1 << 10))
    {
        snprintf(aParamName, BUFF_LEN_64, l2netAlias, sManageWiFiInfo.aBridgeIndex);
        psmSet(aParamName, sBackupLanConfig.aAlias);
    }
}

void readLanDetailsFromSyscfg(LanDetails_t *pLanDetails)
{
    char aBuf[BUFF_LEN_64] = {'\0'};
    int iLenOfBuf = 0;
    if (NULL == pLanDetails)
        return;

    memset(aBuf, 0, sizeof(aBuf));
    if (PRIVATE == pLanDetails->eInterfaceType)
    {
        syscfg_get( NULL, "lan_ipaddr", aBuf, sizeof(aBuf));
        iLenOfBuf = sizeof(pLanDetails->aIpAddr);
        if (('\0' != aBuf[0]) && (1 < iLenOfBuf))
        {
            strncpy(pLanDetails->aIpAddr, aBuf, iLenOfBuf-1);
        }
        memset(aBuf, 0, sizeof(aBuf));
        syscfg_get( NULL, "dhcp_start", aBuf, sizeof(aBuf));
        iLenOfBuf = sizeof(pLanDetails->aStartIpAddr);
        if (('\0' != aBuf[0]) && (1 < iLenOfBuf))
        {
            strncpy(pLanDetails->aStartIpAddr, aBuf, iLenOfBuf-1);
        }
        memset(aBuf, 0, sizeof(aBuf));
        syscfg_get( NULL, "dhcp_end", aBuf, sizeof(aBuf));
        iLenOfBuf = sizeof(pLanDetails->aEndIpAddr);
        if (('\0' != aBuf[0]) && (1 < iLenOfBuf))
        {
            strncpy(pLanDetails->aEndIpAddr, aBuf, iLenOfBuf-1);
        }
    }
}

int isIpInRange(const char *pIpStr, const char *pRangeStartStr, const char *pRangeEndStr)
{
    struct in_addr sIp, sRangeStart, sRangeEnd;

    if ((NULL == pIpStr) || (NULL == pRangeEndStr) || (NULL == pRangeStartStr))
        return -1;

    if (1 != inet_pton(AF_INET, pIpStr, &sIp))
        return -1;
    if (1 != inet_pton(AF_INET, pRangeStartStr, &sRangeStart))
        return -1;
    if (1 != inet_pton(AF_INET, pRangeEndStr, &sRangeEnd))
        return -1;

    uint32_t ip_val = ntohl(sIp.s_addr);
    uint32_t start_val = ntohl(sRangeStart.s_addr);
    uint32_t end_val = ntohl(sRangeEnd.s_addr);

    if (start_val <= end_val)
    {
        return (ip_val >= start_val && ip_val <= end_val);
    }
    else
        return -1;
}

int validateIpRange(char *pIpAddr, char *pStartAddr, char *pEndAddr, pErr pErrRetVal)
{
    LanDetails_t sLanDetails;

    if ((NULL == pIpAddr) || (NULL == pStartAddr) || (NULL == pEndAddr) || (NULL == pErrRetVal))
        return -1;


    if(!strncmp(pIpAddr, RG_CM_COMMUNICATION_BEGIN, strlen(RG_CM_COMMUNICATION_BEGIN)))
    {
        CcspTraceError(("%s:%d, bridge ip begins with 172.31, which is used for RG to CM communication\n",__FUNCTION__, __LINE__));
        snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Invalid IP for manage Wifi,begins with 172.31.X.X which is used for RG to CM communication\n");
        pErrRetVal->ErrorCode = VALIDATION_FALIED;
        return -1;
    }

    memset(&sLanDetails, 0, sizeof(sLanDetails));
    sLanDetails.eInterfaceType = PRIVATE;

    readLanDetailsFromSyscfg(&sLanDetails);

    CcspTraceInfo(("%s:%d, sLanDetails.aIpAddr:%s\n",__FUNCTION__, __LINE__, sLanDetails.aIpAddr));
    CcspTraceInfo(("%s:%d, sLanDetails.aStartIpAddr:%s\n",__FUNCTION__, __LINE__, sLanDetails.aStartIpAddr));
    CcspTraceInfo(("%s:%d, sLanDetails.aEndIpAddr:%s\n",__FUNCTION__, __LINE__, sLanDetails.aEndIpAddr));
    if (('\0' != sLanDetails.aStartIpAddr[0]) && ('\0' != sLanDetails.aEndIpAddr[0]))
    {
        if ((NULL != pIpAddr) && ('\0' != sLanDetails.aIpAddr[0]) && (strcmp(pIpAddr, sBackupLanConfig.aLanIpAddr)))
        {
            CcspTraceInfo(("%s:%d, pIpAddr:%s, sLanDetails.aIpAddr:%s\n",__FUNCTION__, __LINE__, pIpAddr, sLanDetails.aIpAddr));
            if(!strncmp(pIpAddr, sLanDetails.aIpAddr, strlen(pIpAddr)))
            {
                CcspTraceError(("%s:%d, bridge ip is same as private lan IP\n",__FUNCTION__, __LINE__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Invalid IP for manage Wifi, it is same as private Lan IP\n");
                pErrRetVal->ErrorCode = INVALID_IP;
                return -1;
            }
            if (0 != isIpInRange(pIpAddr,sLanDetails.aStartIpAddr,sLanDetails.aEndIpAddr))
            {
                CcspTraceError(("%s:%d, Bridge IP is in the range of private lan IP \n",__FUNCTION__, __LINE__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"ManageWiFi bridge IP is in the range of private Lan IP\n");
                pErrRetVal->ErrorCode = INVALID_IP;
                return -1;
            }
        }
        if ((NULL != pStartAddr) && (strcmp(pStartAddr, sBackupLanConfig.aDhcpStartIpAdd)))
        {
            CcspTraceInfo(("%s:%d, pStartAddr:%s\n",__FUNCTION__, __LINE__, pStartAddr));
            if (0 != isIpInRange(pStartAddr,sLanDetails.aStartIpAddr,sLanDetails.aEndIpAddr))
            {
                CcspTraceError(("%s:%d, Manage Wifi dhcp Start Addr is in the range of private lan IP \n",__FUNCTION__, __LINE__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"ManageWiFi dhcp StartAddr is in the range of private Lan IP\n");
                pErrRetVal->ErrorCode = LAN_PARAM_IP_RANGE_INVALID;
                return -1;
            }
        }
        if ((NULL != pEndAddr) && (strcmp(pEndAddr, sBackupLanConfig.aDhcpEndIpAdd)))
        {
            CcspTraceInfo(("%s:%d, pEndAddr:%s\n",__FUNCTION__, __LINE__, pEndAddr));
            if (0 != isIpInRange(pEndAddr,sLanDetails.aStartIpAddr,sLanDetails.aEndIpAddr))
            {
                CcspTraceError(("%s:%d, Manage Wifi dhcp EndAddr is in the range of private lan IP \n",__FUNCTION__, __LINE__));
                snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"ManageWiFi dhcp EndAddr is in the range of private Lan IP\n");
                pErrRetVal->ErrorCode = LAN_PARAM_IP_RANGE_INVALID;
                return -1;
            }
        }
    }
    return 0;
}

int validateLeaseTime(char *pLeaseTime, pErr pErrRetVal)
{
    if ((NULL == pLeaseTime) || (NULL == pErrRetVal))
        return -1;

    char *pStr = pLeaseTime;

    if ('-' == *pStr)
    {
        CcspTraceError(("%s:%d, Lease Time is in negative \n",__FUNCTION__, __LINE__));
        goto Error;
    }
    else if ('0' <= *pStr && '9' >= *pStr)
    {
        int iValid = false;

        if ('0' == *pStr)
        {
            pStr++;
            if ('\0' == *pStr)
                goto Error;
            else if (('a' <= *pStr && 'z' >= *pStr) || ('A' <= *pStr && 'Z' >= *pStr))
                goto Error;
        }
        while (('\0' != *pStr) && ('0' <= *pStr && '9' >= *pStr))
            pStr++;
        if ('\0' != *pStr)
        {
            switch(*pStr)
            {
                case 'D':
                case 'd':
                {
                    iValid = true;
                    CcspTraceInfo(("%s:%d, Days\n",__FUNCTION__, __LINE__));
                    break;
                }
                case 'W':
                case 'w':
                {
                    iValid = true;
                    CcspTraceInfo(("%s:%d, Weeks \n",__FUNCTION__, __LINE__));
                    break;
                }
                case 'H':
                case 'h':
                {
                    iValid = true;
                    CcspTraceInfo(("%s:%d, Hours \n",__FUNCTION__, __LINE__));
                    break;
                }
                case 'M':
                case 'm':
                {
                    int iVal = 0;
                    iValid = true;
                    extractLeaseTime(pLeaseTime, pStr, &iVal);
                    CcspTraceInfo(("%s:%d, %d minutes\n",__FUNCTION__, __LINE__, iVal));
                    /* Lease time is less then 2 minutes is invalid*/
                    if (iVal < 2)
                    {
                        CcspTraceError(("%s:%d, lease time :%d minutes\n",__FUNCTION__, __LINE__, iVal));
                        goto Error;
                    }
                    break;
                }
                case 'S':
                case 's':
                {
                    int iVal = 0;
                    iValid = true;
                    extractLeaseTime(pLeaseTime, pStr, &iVal);
                    CcspTraceInfo(("%s:%d, %d Seconds \n",__FUNCTION__, __LINE__, iVal));
                    /* Lease time is less then 120 seconds is invalid*/
                    if (iVal < 120)
                    {
                        CcspTraceError(("%s:%d, Lease Time :%d seconds\n",__FUNCTION__, __LINE__, iVal));
                        goto Error;
                    }
                    break;
                }
            }
            if ((true == iValid) && ('\0' == *(pStr+1)))
            {
                CcspTraceInfo(("%s:%d, Valid lease Time\n",__FUNCTION__, __LINE__));
                return 0;
            }
            else
            {
                goto Error;
            }
        }
        else
        {
            /* Lease time is less then 120 seconds is invalid*/
            int iVal = atoi(pLeaseTime);
            if (iVal < 120)
            {
                CcspTraceError(("%s:%d, Lease Time :%d seconds\n",__FUNCTION__, __LINE__, iVal));
                goto Error;
            }
        }
    }
    else
    {
        goto Error;
    }

    return 0;

Error:
    CcspTraceError(("%s:%d, Invalid lease time\n",__FUNCTION__, __LINE__));
    snprintf(pErrRetVal->ErrorMsg, BUFF_LEN_128,"Invalid lease time\n");
    pErrRetVal->ErrorCode = VALIDATION_FALIED;
    return -1;
}

void extractLeaseTime(char *pBegin, char *pEnd, int * pLeaseTime)
{
    if((NULL == pBegin) || (NULL == pEnd) || (NULL == pLeaseTime))
        return;

    int iStrLen = pEnd - pBegin;
    iStrLen += 1;
    char *pValue = (char*) malloc (iStrLen);
    if (NULL != pValue)
    {
        memset(pValue, '\0', iStrLen);
        strncpy(pValue, pBegin, iStrLen);
        *pLeaseTime = atoi(pValue);
        free(pValue);
        pValue = NULL;
    }
}

void getManageWiFiAddrRange(LanDetails_t * pManageWiFiAddrDetails)
{
    char aManageWiFiEnabled[BUFF_LEN_8] = {0};

    if (NULL == pManageWiFiAddrDetails)
    {
        CcspTraceError(("%s:%d, NULL parameter passed\n",__FUNCTION__,__LINE__));
        return;
    }
    syscfg_get(NULL, "Manage_WiFi_Enabled", aManageWiFiEnabled, BUFF_LEN_8);

    if ((!strncmp(aManageWiFiEnabled, "true", 4)) && (MANAGE_WIFI == pManageWiFiAddrDetails->eInterfaceType))
    {
        strncpy(pManageWiFiAddrDetails->aIpAddr, sBackupLanConfig.aLanIpAddr, sizeof(pManageWiFiAddrDetails->aIpAddr)-1);
        strncpy(pManageWiFiAddrDetails->aStartIpAddr, sBackupLanConfig.aDhcpStartIpAdd, sizeof(pManageWiFiAddrDetails->aStartIpAddr)-1);
        strncpy(pManageWiFiAddrDetails->aEndIpAddr, sBackupLanConfig.aDhcpEndIpAdd, sizeof(pManageWiFiAddrDetails->aEndIpAddr)-1);
    }
}
