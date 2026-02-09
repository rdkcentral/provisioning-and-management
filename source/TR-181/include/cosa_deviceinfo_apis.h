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

    module: cosa_deviceinfo_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_DEVICEINFO_APIS_H
#define  _COSA_DEVICEINFO_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include <cjson/cJSON.h>
#ifdef FEATURE_SUPPORT_ONBOARD_LOGGING

#define LOGGING_MODULE           "PAM"
#define OnboardLog(...)          rdk_log_onboard(LOGGING_MODULE, __VA_ARGS__)
#else
#define OnboardLog(...)
#endif
#include <sys/sysinfo.h>

#include "cm_hal.h"

#define MAX_SECURITYNUM_LEN         128
#define MAX_SECURITYNAME_LEN        18      // includes room for NULL terminator

#define PARTNERID_FILE  "/nvram/.partner_ID"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#if !defined (RESOURCE_OPTIMIZATION)

enum
{
    COSA_DML_PROC_STATUS_Running = 1,
    COSA_DML_PROC_STATUS_Sleeping,
    COSA_DML_PROC_STATUS_Stopped,
    COSA_DML_PROC_STATUS_Idle,
    COSA_DML_PROC_STATUS_Uninterruptible,
    COSA_DML_PROC_STATUS_Zombie,
};

typedef  struct
_COSA_PROCESS_ENTRY
{
    ULONG                           Pid;
    char                            Command[256];
    ULONG                           Size;
    ULONG                           Priority;
    ULONG                           CPUTime;
    ULONG                           State;
}
COSA_PROCESS_ENTRY, *PCOSA_PROCESS_ENTRY;

#define  COSA_DATAMODEL_PROCESS_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    ULONG                           ProcessNumberOfEntries;                             \
    ULONG                           CPUUsage;                                           \
    PCOSA_PROCESS_ENTRY             pProcTable;                                       \

typedef  struct
_COSA_DATAMODEL_PROCESS
{
    COSA_DATAMODEL_PROCESS_CLASS_CONTENT
}
COSA_DATAMODEL_PROCSTATUS,  *PCOSA_DATAMODEL_PROCSTATUS;

#endif

typedef  struct
_COSA_BOOTSTRAP_STR
{
    CHAR                    ActiveValue[512];
    CHAR		    UpdateSource[16];
}
COSA_BOOTSTRAP_STR;

typedef  struct
_COSA_BOOTSTRAP_BOOL
{
    BOOL                    ActiveValue;
    CHAR		    UpdateSource[16];
}
COSA_BOOTSTRAP_BOOL;

typedef  struct
COSA_AUTO_REBOOT
{
    BOOL            Enable;
    INT	            UpTime;
}
COSA_AUTO_REBOOT;

typedef  struct
_COSA_DATAMODEL_RDKB_FOOTER_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR 		PartnerLink;
	COSA_BOOTSTRAP_STR 		UserGuideLink;
	COSA_BOOTSTRAP_STR 		CustomerCentralLink;
	COSA_BOOTSTRAP_STR		PartnerText;
	COSA_BOOTSTRAP_STR		UserGuideText;
	COSA_BOOTSTRAP_STR		CustomerCentralText;
}
COSA_DATAMODEL_RDKB_FOOTER, *PCOSA_DATAMODEL_RDKB_FOOTER;

typedef  struct
_COSA_DATAMODEL_RDKB_CONNECTION_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR 		MSOmenu;
	COSA_BOOTSTRAP_STR 		MSOinfo;
	COSA_BOOTSTRAP_STR 		StatusTitle;
	COSA_BOOTSTRAP_STR 		StatusInfo;
}
COSA_DATAMODEL_RDKB_CONNECTION, *PCOSA_DATAMODEL_RDKB_CONNECTION;

typedef  struct
_COSA_DATAMODEL_RDKB_NETWORKDIAGNOSTICTOOLS_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR 		ConnectivityTestURL;
}
COSA_DATAMODEL_RDKB_NETWORKDIAGNOSTICTOOLS, *PCOSA_DATAMODEL_RDKB_NETWORKDIAGNOSTICTOOLS;

typedef  struct
_COSA_DATAMODEL_RDKB_WIFIPERSONALIZATION_CLASS_CONTENT
{
	COSA_BOOTSTRAP_BOOL			Support;
	COSA_BOOTSTRAP_STR			PartnerHelpLink;
	COSA_BOOTSTRAP_BOOL			SMSsupport;
	COSA_BOOTSTRAP_BOOL			MyAccountAppSupport;
	COSA_BOOTSTRAP_STR			MSOLogo;
	COSA_BOOTSTRAP_STR			Title;
	COSA_BOOTSTRAP_STR			WelcomeMessage;
	COSA_BOOTSTRAP_STR			WelcomeMessage_fre;
}

COSA_DATAMODEL_RDKB_WIFIPERSONALIZATION, *PCOSA_DATAMODEL_RDKB_WIFIPERSONALIZATION;
typedef  struct
_COSA_DATAMODEL_RDKB_LOCALUI_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR			MSOLogo;
	COSA_BOOTSTRAP_STR			DefaultLoginUsername;
	COSA_BOOTSTRAP_STR			DefaultLoginPassword;
	COSA_BOOTSTRAP_STR			MSOLogoTitle;
	COSA_BOOTSTRAP_BOOL			HomeNetworkControl;
}
COSA_DATAMODEL_RDKB_LOCALUI, *PCOSA_DATAMODEL_RDKB_LOCALUI;

typedef  struct
_COSA_DATAMODEL_RDKB_HELPTIP_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR			NetworkName;
}
COSA_DATAMODEL_RDKB_HELPTIP, *PCOSA_DATAMODEL_RDKB_HELPTIP;

typedef  struct
_COSA_DATAMODEL_RDKB_CLOUDUI_CLASS_CONTENT
{
    COSA_BOOTSTRAP_STR            brandname;
    COSA_BOOTSTRAP_STR            productname;
    COSA_BOOTSTRAP_STR            link;
}
COSA_DATAMODEL_RDKB_CLOUDUI, *PCOSA_DATAMODEL_RDKB_CLOUDUI;

typedef  struct
_COSA_DATAMODEL_RDKB_UIBRANDING_CLASS_CONTENT
{
	COSA_DATAMODEL_RDKB_FOOTER			Footer;
	COSA_DATAMODEL_RDKB_CONNECTION			Connection;
	COSA_DATAMODEL_RDKB_NETWORKDIAGNOSTICTOOLS	NDiagTool;
	COSA_DATAMODEL_RDKB_WIFIPERSONALIZATION		WifiPersonal;
	COSA_DATAMODEL_RDKB_LOCALUI			LocalUI;
	COSA_DATAMODEL_RDKB_HELPTIP			HelpTip;
	COSA_DATAMODEL_RDKB_CLOUDUI			CloudUI;
	COSA_BOOTSTRAP_STR				DefaultAdminIP;
	COSA_BOOTSTRAP_STR				DefaultLocalIPv4SubnetRange;
	COSA_BOOTSTRAP_STR                        	DefaultLanguage;
	COSA_BOOTSTRAP_STR				PauseScreenFileLocation;
	COSA_BOOTSTRAP_BOOL				AllowEthernetWAN;
}
COSA_DATAMODEL_RDKB_UIBRANDING, *PCOSA_DATAMODEL_RDKB_UIBRANDING;

typedef  struct
_COSA_DATAMODEL_RDKB_CDLDM_CLASS_CONTENT
{
        COSA_BOOTSTRAP_STR                      CDLModuleUrl;
}
COSA_DATAMODEL_RDKB_CDLDM, *PCOSA_DATAMODEL_RDKB_CDLDM;

typedef  struct
_COSA_DATAMODEL_RDKB_WIFI_TELEMETRY_CLASS_CONTENT
{
    INT                 LogInterval;
    INT                 ChUtilityLogInterval;
    CHAR            NormalizedRssiList[256];
    CHAR            CliStatList[256];
    CHAR            TxRxRateList[256];
    CHAR            SNRList[256];
}
COSA_DATAMODEL_RDKB_WIFI_TELEMETRY, *PCOSA_DATAMODEL_RDKB_WIFI_TELEMETRY;

typedef  struct
_COSA_DATAMODEL_RDKB_RFC_TELEMETRY_CLASS_CONTENT
{
    COSA_BOOTSTRAP_BOOL               Enable;
    COSA_BOOTSTRAP_STR                Version;
    COSA_BOOTSTRAP_STR                ConfigURL;

}
COSA_DATAMODEL_RDKB_RFC_TELEMETRY, *PCOSA_DATAMODEL_RDKB_RFC_TELEMETRY;

typedef  struct
_COSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID_CLASS_CONTENT
{
    BOOL		Enable;
    CHAR            	TagString[256];
    INT                 TimingInterval;

}
COSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID, *PCOSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID;

//RDKB-47490
typedef  struct
_COSA_DATAMODEL_RDKB_LATENCYMEASURE_TCP_IPV4_CLASS_CONTENT
{
    BOOL		Enable;
}
COSA_DATAMODEL_RDKB_LATENCYMEASURE_TCP_IPV4, *PCOSA_DATAMODEL_RDKB_LATENCYMEASURE_TCP_IPV4;

//RDKB-47490
typedef  struct
_COSA_DATAMODEL_RDKB_LATENCYMEASURE_TCP_IPV6_CLASS_CONTENT
{
    BOOL		Enable;
}
COSA_DATAMODEL_RDKB_LATENCYMEASURE_TCP_IPV6, *PCOSA_DATAMODEL_RDKB_LATENCYMEASURE_TCP_IPV6;

typedef  struct
_COSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL_CLASS_CONTENT
{
    COSA_BOOTSTRAP_BOOL    Enable;
    COSA_BOOTSTRAP_STR    InitialForwardedMark;
    COSA_BOOTSTRAP_STR    InitialOutputMark;
}
COSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL, *PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL;


typedef  struct
    _COSA_DATAMODEL_KICKSTARTTABLE
{
    UINT    SecurityNumberLen;
    uint8_t SecurityNumber[MAX_SECURITYNUM_LEN];
    CHAR    SecurityName[MAX_SECURITYNAME_LEN];
}
COSA_DATAMODEL_KICKSTARTTABLE, *PCOSA_DATAMODEL_KICKSTARTTABLE;

#define COSA_DATAMODEL_KICKSTART_CLASS_CONTENT                      \
    /* duplication of the base object class content */              \
    /* COSA_BASE_CONTENT */                                         \
    BOOL                            TableUpdated;                   \
    BOOL                            Enabled;                        \
    UINT                            KickstartTotal;                 \
    UINT                            TableNumberOfEntries;           \
    COSA_DATAMODEL_KICKSTARTTABLE   KickstartTable[MAX_KICKSTART_ROWS];

typedef  struct
    _COSA_DATAMODEL_KICKSTART
{
    COSA_DATAMODEL_KICKSTART_CLASS_CONTENT
}
COSA_DATAMODEL_KICKSTART,   *PCOSA_DATAMODEL_KICKSTART;

typedef struct
    _COSA_HHT_PTR
{
    UINT CPUThreshold;
    UINT DRAMThreshold;
    UINT Frequency;
    BOOL PTREnable;
}
COSA_HHT_PTR;

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/
#if !defined (NO_MOCA_FEATURE_SUPPORT)

/**
* @brief Get the Enable MoCA for Xi5 flag status.
*
* This function retrieves the current status of the MoCA enable flag for Xi5 devices.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a boolean variable where the flag status will be returned.
*                      \n TRUE: MoCA is enabled for Xi5.
*                      \n FALSE: MoCA is disabled for Xi5.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetEnableMoCAforXi5Flag
  (
         ANSC_HANDLE                             hContext,
         BOOLEAN*                                        pValue
  );

/**
* @brief Set the Enable MoCA for Xi5 flag status.
*
* This function updates the MoCA enable flag for Xi5 devices with the specified value.
*
* @param[in] hContext - Handle to the context.
* @param[in] pValue - Pointer to a boolean variable containing the new flag value to be set.
*                     \n TRUE: Enable MoCA for Xi5.
*                     \n FALSE: Disable MoCA for Xi5.
* @param[in,out] pEnableMoCAforXi5Flag - Pointer to the current MoCA Xi5 flag value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSetEnableMoCAforXi5Flag
   (
          ANSC_HANDLE                             hContext,
          BOOLEAN*                                        pValue,
          BOOLEAN*                                        pEnableMoCAforXi5Flag
   );

/**
* @brief Check and enable MoCA if required.
*
* This function checks the current MoCA configuration and enables MoCA
* functionality if the conditions are met for Xi5 devices.
*
* @return None.
*
*/
void CosaDmlDiCheckAndEnableMoCA( void );
#endif

/**
* @brief Get the reboot counter value.
*
* This function retrieves the current reboot counter which tracks the number of device reboots.
*
* @return The reboot counter value.
*
*/
int getRebootCounter();

/**
* @brief Increment and set the reboot counter.
*
* This function increments the reboot counter and persists the updated value.
*
* @return The status of the operation.
* @retval 0 if successful.
* @retval -1 if any error occurs.
*
*/
int setRebootCounter();

/**
* @brief Set unknown reboot reason.
*
* This function records an unknown reboot reason when the cause of reboot cannot be determined.
*
* @return The status of the operation.
* @retval 0 if successful.
* @retval -1 if any error occurs.
*
*/
int setUnknownRebootReason();
#if !defined (RESOURCE_OPTIMIZATION)

/**
* @brief Create a process status object.
*
* This function creates and initializes a process status data model object.
*
* @return Handle to the created process status object.
* @retval Non-NULL Handle to the created process status.
* @retval NULL if failed.
*/
ANSC_HANDLE CosaProcStatusCreate();

/**
* @brief Remove process information.
*
* This function cleans up and removes process information from the process status object.
*
* @param[in,out] pObj - Pointer to the process status data model object.
*
* @return None.
*
*/
void COSADmlRemoveProcessInfo(PCOSA_DATAMODEL_PROCSTATUS pObj);

/**
* @brief Get process information.
*
* This function retrieves current process information.
*
* @param[out] p_info - Pointer to the process status data model structure where
*                      process information will be returned.
*
* @return None.
*
*/
void COSADmlGetProcessInfo(PCOSA_DATAMODEL_PROCSTATUS p_info);
#endif

/**
* @brief Get the CPU usage percentage.
*
* This function retrieves the current CPU usage as a percentage.
*
* @return CPU usage percentage value(Valid range: 1-100)
*
*/
ULONG COSADmlGetCpuUsage();
#if defined (RESOURCE_OPTIMIZATION)

/**
* @brief Get the number of process entries.
*
* This function retrieves the total count of active processes in the system.
*
* @return The number of process entries.
*
*/
ULONG COSADmlGetProcessNumberOfEntries();
#endif

/**
* @brief Get memory status information.
*
* This function retrieves specific memory status information based on the parameter name provided.
*
* @param[in] ParamName - Pointer to a null-terminated string specifying the
*                        memory parameter to query.
*
* @return The requested memory status value in appropriate units.
* @retval The requested memory status value on success
* @retval 0 on failure
*
*/
ULONG COSADmlGetMemoryStatus(char * ParamName);

/**
* @brief Get the maximum TCP window size.
*
* This function retrieves the maximum TCP window size configured for the system.
*
* @return The maximum TCP window size value in bytes.
* @retval The maximum window size in bytes on success.
* @retval 0 on failure.
*
*/
ULONG COSADmlGetMaxWindowSize();

/**
* @brief Initialize the Device Info subsystem.
*
* This function initializes the Device Info data model subsystem and sets up the platform HAL database.
*
* @param[in] hDml - Handle to the DML context.
* @param[in,out] phContext - Pointer to handle for returning context information.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Get the device manufacturer name.
*
* This function retrieves the manufacturer name of the device from the vendor configuration.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the manufacturer name will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetManufacturer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the manufacturer OUI (Organizationally Unique Identifier).
*
* This function retrieves the manufacturer's OUI which uniquely identifies the vendor organization.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the manufacturer OUI will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetManufacturerOUI
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the device description.
*
* This function retrieves a human-readable description of the device.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the device description will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetDescription
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the device product class.
*
* This function retrieves the product class identifier of the device.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the product class will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetProductClass
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the device serial number.
*
* This function retrieves the unique serial number of the device from the platform HAL.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the serial number will be returned as a null-terminated string.
*                      \n Recommended minimum buffer size: 128 bytes.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetSerialNumber
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the device hardware version.
*
* This function retrieves the hardware version string of the device from the platform HAL.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the hardware version will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetHardwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get additional hardware version information.
*
* This function retrieves additional hardware version details.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the additional hardware version will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetAdditionalHardwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the provisioning code.
*
* This function retrieves the current provisioning code configured for the device.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the provisioning code will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetProvisioningCode
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Set the provisioning code.
*
* This function updates the provisioning code for the device.
*
* @param[in] hContext - Handle to the context.
* @param[in] pProvisioningCode - Pointer to a null-terminated string containing the new provisioning code to be set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ERR_UTCTX_INIT on failure
*
*/
ANSC_STATUS
CosaDmlDiSetProvisioningCode
    (
        ANSC_HANDLE                 hContext,
        char*                       pProvisioningCode
    );

/**
* @brief Get the device uptime.
*
* This function retrieves the elapsed time in seconds since the device was last rebooted.
*
* @param[in] hContext - Handle to the context.
*
* @return The device uptime in seconds.
* @retval Device uptime in seconds on success.
* @retval 0 on failure.
*
*/
ULONG
CosaDmlDiGetUpTime
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get the first use date of the device.
*
* This function retrieves the timestamp of when the device was first powered on or activated.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the first use date will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetFirstUseDate
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Get the hardware information.
*
* This function retrieves detailed hardware information of the device.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the hardware information will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetHardware
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Get the bootloader version.
*
* This function retrieves the version string of the device's bootloader.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the bootloader version will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetBootloaderVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Get the firmware build time.
*
* This function retrieves the build timestamp of the currently running firmware.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the firmware build time will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetFirmwareBuildTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Get the base MAC address.
*
* This function retrieves the base MAC address of the device.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the base MAC address will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetBaseMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Get the TCP implementation information.
*
* This function retrieves information about the TCP/IP stack implementation used by the device.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the TCP implementation information will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlGetTCPImplementation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get advanced services information.
*
* This function retrieves information about advanced services available on the device.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the advanced services information will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetAdvancedServices
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the processor speed.
*
* This function retrieves the processor speed information of the device.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the processor speed will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetProcessorSpeed
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Validate and sanitize input parameters.
*
* This function validates input parameters.
*
* @param[in] inputparam - Pointer to the input parameter string to be validated.
* @param[out] wrapped_inputparam - Pointer to a buffer where the sanitized and wrapped parameter will be stored.
* @param[in] lengthof_inputparam - Length of the input parameter string.
* @param[in] sizeof_wrapped_inputparam - Size of the wrapped_inputparam buffer in bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
isValidInput
    (
        char                       *inputparam,
        char                       *wrapped_inputparam,
        int                         lengthof_inputparam,
        int                         sizeof_wrapped_inputparam
    );

/**
* @brief Get the firmware upgrade start time.
*
* This function retrieves the timestamp when the last firmware upgrade process was initiated.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the start time will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeStartTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the firmware upgrade end time.
*
* This function retrieves the timestamp when the last firmware upgrade process was completed.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the end time will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeEndTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Set the firmware upgrade start time.
*
* This function records the start time of a firmware upgrade process.
*
* @param[in] pString - Pointer to a null-terminated string containing the start time timestamp to be recorded.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSetFirmwareUpgradeStartTime (char* pString);

/**
* @brief Set the firmware upgrade end time.
*
* This function records the end time of a firmware upgrade process.
*
* @param[in] pString - Pointer to a null-terminated string containing the end time timestamp to be recorded.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSetFirmwareUpgradeEndTime (char* pString);

/**
* @brief Get the custom data model enabled status.
*
* This function retrieves whether the custom data model is enabled on the device.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a boolean variable where the status will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGiGetCustomDataModelEnabled
(
    ANSC_HANDLE                 hContext,
    BOOL                        *pValue
);

/**
* @brief Set the custom data model enabled status.
*
* This function enables or disables the custom data model on the device.
*
* @param[in] hContext - Handle to the context.
* @param[in] bValue - Boolean value to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ULONG
CosaDmlGiSetCustomDataModelEnabled
(
    ANSC_HANDLE                 hContext,
    BOOL                        bValue
);

/**
* @brief Get and process DHCP server detection flag.
*
* This function retrieves the current state of the DHCP server detection flag.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a boolean variable where the flag status will be returned.
*                      \n TRUE: DHCP server detection is enabled.
*                      \n FALSE: DHCP server detection is disabled.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetAndProcessDhcpServDetectionFlag
   (
	   ANSC_HANDLE				   hContext,
	   BOOLEAN*			   		   pValue
   );

/**
* @brief Set and process DHCP server detection flag.
*
* This function updates the DHCP server detection flag.
*
* @param[in] hContext - Handle to the context.
* @param[in] pValue - Pointer to a boolean variable containing the new flag value.
*                     \n TRUE: Enable DHCP server detection.
*                     \n FALSE: Disable DHCP server detection.
* @param[in,out] pDhcpServDetectEnable - Pointer to the current DHCP server detection enable flag.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSetAndProcessDhcpServDetectionFlag
   (
	   ANSC_HANDLE				   hContext,
	   BOOLEAN*					   pValue,
	   BOOLEAN*					   pDhcpServDetectEnable
   );

/**
* @brief Trigger XOps reverse SSH connection.
*
* This function initiates a reverse SSH connection for XOps remote support operations.
*
* @param[in] input - Pointer to a null-terminated string containing the trigger command.
*
* @return The status of the operation.
* @retval OK if successful.
* @retval NOK if any error occurs.
*
*/
int setXOpsReverseSshTrigger(char *input);

/**
* @brief Set XOps reverse SSH arguments.
*
* This function configures the arguments for XOps reverse SSH connection.
*
* @param[in] pString - Pointer to a null-terminated string containing the SSH arguments.
*
* @return The status of the operation.
* @retval 0 if successful.
* @retval 1 if any error occurs.
*
*/
int setXOpsReverseSshArgs(char* pString);

/**
* @brief Get XOps reverse SSH arguments.
*
* This function retrieves the currently configured arguments for XOps reverse SSH.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the SSH arguments will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS getXOpsReverseSshArgs
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Check if reverse SSH is active.
*
* This function checks whether a reverse SSH connection is currently active.
*
* @return Status of reverse SSH.
* @retval OK if reverse SSH is active.
* @retval NOK if reverse SSH is not active.
*
*/
int isRevSshActive(void);

/**
* @brief Get the syndication partner ID.
*
* This function retrieves the current syndication partner ID from the device configuration.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the partner ID will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetSyndicationPartnerId
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Get the syndication TR-069 certificate location.
*
* This function retrieves the file path where the TR-069 certificate for syndication is stored.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the certificate location path will be returned as a null-terminated string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetSyndicationTR69CertLocation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
* @brief Set the syndication TR-069 certificate location.
*
* This function updates the file path where the TR-069 certificate for syndication stored or retrieved from.
*
* @param[in] hContext - Handle to the context.
* @param[in] pValue - Pointer to a null-terminated string containing the new certificate location path.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSetSyndicationTR69CertLocation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

#if defined(_ONESTACK_PRODUCT_REQ_)
/**
* @brief Get the syndication device mode.
*
* This function retrieves the current device mode from syscfg.
* The device mode can be either "business" or "residential".
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the device mode will be returned as a null-terminated string.
* @param[in] size - Size of the pValue buffer (must be at least 32 bytes).
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetSyndicationDeviceMode
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        size_t                      size
    );
#endif

/**
* @brief Handle partner ID change operations.
*
* This function processes partner ID changes and performs necessary configuration updates.
*
* @param[in] buff - Pointer to a buffer containing partner ID change information.
*
* @return Pointer to result data or NULL.
*
*/
void *CosaDmlDiPartnerIDChangeHandling( void* buff );

/**
* @brief Get the syndication local UI branding table.
*
* This function retrieves the branding configuration table for the local user interface
* based on the current syndication partner.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the branding table will be returned
*                      as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetSyndicationLocalUIBrandingTable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Get the syndication WiFi UI branding table.
*
* This function retrieves the WiFi UI branding configuration table based on the current syndication partner.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the WiFi branding table will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetSyndicationWifiUIBrandingTable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

#ifndef FEATURE_FWUPGRADE_MANAGER
/**
* @brief Initiate firmware download and factory reset.
*
* This function triggers a firmware download followed by a factory reset operation.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSetFirmwareDownloadAndFactoryReset(void);

/**
* @brief Get the defer firmware download reboot flag.
*
* This function retrieves the current status of the defer firmware download reboot flag.
*
* @param[out] puLong - Pointer to a variable where the flag value will be returned.
*
* @return None.
*
*/
void CosaDmlDiGet_DeferFWDownloadReboot(ULONG* puLong);

/**
* @brief Set the defer firmware download reboot flag.
*
* This function updates the defer firmware download reboot flag.
*
* @param[in,out] DeferFWDownloadReboot - Pointer to the current flag value.
* @param[in] uValue - New value to set for the defer reboot flag.
*
* @return None.
*
*/
void CosaDmlDiSet_DeferFWDownloadReboot(ULONG* DeferFWDownloadReboot , ULONG uValue);
#endif

/**
* @brief Reboot the device.
*
* This function initiates a device reboot with the specified reboot reason.
*
* @param[in] pValue - Pointer to a null-terminated string containing the reboot reason.
*
* @return None.
*
*/
void CosaDmlDiSet_RebootDevice(char* pValue);

/**
* @brief Disable remote management.
*
* This function disables remote management capabilities on the device.
*
* @return None.
*
*/
void CosaDmlDiSet_DisableRemoteManagement();

/**
* @brief Validate reboot device parameter.
*
* This function validates the reboot device parameter to ensure a valid reboot reason.
*
* @param[in] pValue - Pointer to a null-terminated string containing the parameter to validate.
*
* @return Validation result.
* @retval TRUE if the parameter is valid.
* @retval FALSE if the parameter is invalid.
*
*/
BOOL CosaDmlDi_ValidateRebootDeviceParam(char *pValue);

/**
* @brief Initialize UI branding subsystem.
*
* This function initializes the UI branding data model with partner-specific branding configurations.
*
* @param[in] hContext - Handle to the context.
* @param[out] PUiBrand - Pointer to the UI branding data model structure to be initialized.
* @param[out] PCdlDM - Pointer to the CDL DM (Code Download) data model structure.
* @param[out] PRfcTelemetry - Pointer to the RFC telemetry data model structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiUiBrandingInit
  (
	ANSC_HANDLE                 hContext,
	PCOSA_DATAMODEL_RDKB_UIBRANDING	PUiBrand,
	PCOSA_DATAMODEL_RDKB_CDLDM PCdlDM,
	PCOSA_DATAMODEL_RDKB_RFC_TELEMETRY PRfcTelemetry
  );

/**
* @brief Fill partner ID specific values.
*
* This function populates the branding data structures with partner-specific configuration values from JSON.
*
* @param[in] json - Pointer to the cJSON object containing partner configuration.
* @param[in] partnerID - Pointer to a null-terminated string containing the partner ID.
* @param[out] PUiBrand - Pointer to the UI branding structure to be populated.
* @param[out] PCdlDM - Pointer to the CDL DM structure to be populated.
* @param[out] PRfcTelemetry - Pointer to the RFC telemetry structure to be populated.
* @param[in] hContext - Handle to the context.
*
* @return None.
*
*/
void FillPartnerIDValues(cJSON *json , char *partnerID , PCOSA_DATAMODEL_RDKB_UIBRANDING PUiBrand, PCOSA_DATAMODEL_RDKB_CDLDM PCdlDM, PCOSA_DATAMODEL_RDKB_RFC_TELEMETRY PRfcTelemetry, ANSC_HANDLE hContext);

/**
* @brief Update JSON parameter in configuration.
*
* This function updates a specific JSON parameter with the provided value and metadata.
*
* @param[in] pKey - Pointer to a null-terminated string containing the parameter key.
* @param[in] PartnerId - Pointer to a null-terminated string containing the partner ID.
* @param[in] pValue - Pointer to a null-terminated string containing the new value.
* @param[in] pSource - Pointer to a null-terminated string indicating the update source.
* @param[in] pCurrentTime - Pointer to a null-terminated string containing the current timestamp.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS UpdateJsonParam
	(
		char*           pKey,
		char*			PartnerId,
		char*			pValue,
		char*                   pSource,
		char*			pCurrentTime
    );

/**
* @brief Initialize WiFi telemetry subsystem.
*
* This function initializes the WiFi telemetry data model with configuration.
*
* @param[out] PWiFi_Telemetry - Pointer to the WiFi telemetry data model structure to be initialized.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiWiFiTelemetryInit
  (
	PCOSA_DATAMODEL_RDKB_WIFI_TELEMETRY PWiFi_Telemetry
  );

/**
* @brief Initialize unique telemetry ID subsystem.
*
* This function initializes the unique telemetry ID data model for telemetry data with unique identifiers.
*
* @param[out] PUniqueTelemetryId - Pointer to the unique telemetry ID data model structure to be initialized.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiUniqueTelemetryIdInit
  (
	PCOSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID PUniqueTelemetryId
  );

/**
* @brief Convert time to day, hour, and minutes.
*
* This function converts a time value in minutes into separate day, hour, and minute string components.
*
* @param[in] time - Time value in minutes to be converted.
* @param[out] day - Array to store the day component as a string.
* @param[out] hour - Array to store the hour component as a string.
* @param[out] mins - Array to store the minute component as a string.
*
* @return None.
*
*/
void ConvertTime(int time, char day[], char hour[], char mins[]);

/**
* @brief Configure unique telemetry cron job.
*
* This function enables or disables the unique telemetry cron job with specified time interval and tag string.
*
* @param[in] enable - Boolean flag to enable or disable the cron job.
*                     \n TRUE: Enable cron job.
*                     \n FALSE: Disable cron job.
* @param[in] timeInterval - Time interval in minutes for the cron job execution.
* @param[in] tagString - Pointer to a null-terminated string containing the tag for telemetry.
*
* @return None.
*
*/
void UniqueTelemetryCronJob(BOOL enable, INT timeInterval, char* tagString);

/**
* @brief Get the device boot time.
*
* This function retrieves the absolute time when the device was last booted.
*
* @param[in] Context - Handle to the context.
*
* @return The boot time as a UNIX timestamp in seconds.
* @retval The device boot time
* @retval 0 on failure
*
*/
ULONG
CosaDmlDiGetBootTime
    (
        ANSC_HANDLE                 Context
    );

/**
* @brief Get the factory reset count.
*
* This function retrieves the number of times the device has been factory reset.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a variable where the factory reset count will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetFactoryResetCount
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

/**
* @brief Get the hardware memory used.
*
* This function retrieves the amount of memory currently in use by the system.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the memory used value will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetHardware_MemUsed
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Get the hardware memory free.
*
* This function retrieves the amount of free memory available in the system.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the memory free value will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetHardware_MemFree
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Get the CMTS MAC address.
*
* This function retrieves the MAC address of the Cable Modem Termination System (CMTS).
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the CMTS MAC address will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetCMTSMac
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

/**
* @brief Get the inactive firmware version.
*
* This function retrieves the firmware version of the inactive firmware bank.
* The .bin extension is automatically removed from the returned value.
*
* @param[in] hContext - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the inactive firmware version will be returned as a null-terminated string.
*                      \n Minimum recommended buffer size: 64 bytes.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiGetInActiveFirmware
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Clear the factory reset count.
*
* This function resets the factory reset counter to zero.
*
* @param[in] hContext - Handle to the context.
* @param[in] bValue - Boolean value to trigger the clear operation.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS CosaDmlDiClearResetCount
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bValue
   );

/**
* @brief Set new NTP enable status.
*
* This function enables or disables the Network Time Protocol (NTP) client.
*
* @param[in] bValue - Boolean value to set.
*                     \n TRUE: Enable NTP client.
*                     \n FALSE: Disable NTP client.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlSetnewNTPEnable(BOOL bValue);

/**
* @brief Get the internet connection status.
*
* This function checks whether the device has active internet connectivity.
*
* @return Internet connection status.
* @retval TRUE if internet is connected.
* @retval FALSE if internet is not connected.
*
*/
BOOL CosaDmlGetInternetStatus();

/**
* @brief Get the upload logs status.
*
* This function retrieves the current status of the log upload operation.
*
* @param[in] Context - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the status will be returned as a null-terminated string.
* @param[in,out] pUlSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
COSADmlUploadLogsStatus
    (
        ANSC_HANDLE                 Context,
        char*   pValue,
        ULONG*  pUlSize
    );

/**
* @brief Trigger immediate log upload.
*
* This function initiates an immediate upload of device logs to the configured server.
*
* @param[in] hContext - Handle to the context.
* @param[in] bEnable - Boolean flag to trigger the upload.
*                      \n TRUE: Start log upload immediately.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
COSADmlUploadLogsNow
        (
                ANSC_HANDLE                 hContext,
                BOOL                        bEnable
        );

/**
* @brief Get the log file name.
*
* This function retrieves the name of the current log file being used.
*
* @param[in] Context - Handle to the context.
* @param[out] pValue - Pointer to a buffer where the log file name will be returned as a null-terminated string.
* @param[in,out] pUlSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
COSADmlLogFileName
    (
        ANSC_HANDLE                 Context,
    	char* 	pValue,
    	ULONG*	pUlSize
    );

/**
* @brief Enable or disable presence detection.
*
* This function enables or disables the presence detection feature.
*
* @param[in] enable - Boolean flag to control presence detection.
*                     \n TRUE: Enable presence detection.
*                     \n FALSE: Disable presence detection.
*
* @return None.
*
*/
void CosaDmlPresenceEnable(BOOL enable);

/**
* @brief Start RFC (Remote Feature Control) processing.
*
* This function initiates the RFC processing to handle remote feature configurations.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
StartRfcProcessing();

/**
* @brief End RFC processing.
*
* This function finalizes the RFC processing and updates the RFC store.
*
* @param[in,out] pRfcStore - Pointer to a pointer to the cJSON RFC store object.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
EndRfcProcessing(cJSON **pRfcStore);

/**
* @brief Set syndication flow control enable status.
*
* This function enables or disables syndication flow control for traffic management.
*
* @param[in] bValue - Character value representing the enable/disable state.
*                     \n '1' or non-zero: Enable flow control.
*                     \n '0': Disable flow control.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSet_SyndicationFlowControl_Enable
    (
        char bValue
    );

/**
* @brief Set syndication flow control initial forwarded mark.
*
* This function configures the initial forwarded packet marking for syndication flow control.
*
* @param[in] pString - Pointer to a null-terminated string containing the mark value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSet_SyndicationFlowControl_InitialForwardedMark
    (
        char *pString
    );

/**
* @brief Set syndication flow control initial output mark.
*
* This function configures the initial output packet marking for syndication flow control.
*
* @param[in] pString - Pointer to a null-terminated string containing the mark value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSet_SyndicationFlowControl_InitialOutputMark
    (
        char *pString
    );

/**
* @brief Set temporary partner ID.
*
* This function stores a temporary partner ID for syndication purposes.
*
* @param[in] pValue - Pointer to a null-terminated string containing the temporary partner ID.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
setTempPartnerId
    (
        char*                       pValue
    );

/**
* @brief Get the factory default partner ID.
*
* This function retrieves the factory default partner ID.
*
* @param[out] pValue - Pointer to a buffer where the factory partner ID will be returned as a null-terminated string.
* @param[in,out] pulSize - Pointer to a variable containing the buffer size.
*                          \n [in] Size of the pValue buffer in bytes.
*                          \n [out] Actual length of the returned string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
getFactoryPartnerId
    (
        char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Derive syndication partner ID.
*
* This function derives and validates the syndication partner ID.
*
* @param[out] Partner_ID - Pointer to a buffer where the derived partner ID will be stored.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDeriveSyndicationPartnerID
    (
        char *Partner_ID
    );

/**
* @brief Initialize syndication flow control.
*
* This function initializes the syndication flow control data model with configuration parameters.
*
* @param[out] pSyndicatonFlowControl - Pointer to the syndication flow control data model
*                                       structure to be initialized.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiSyndicationFlowControlInit
    (
        PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL pSyndicatonFlowControl
    );

/**
* @brief Initialize RFC defaults.
*
* This function initializes the RFC defaults configuration from stored settings.
*
* @param[in,out] pRfcDefaults - Pointer to a pointer to the cJSON RFC defaults object.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiRfcDefaultsInit
    (
        cJSON **pRfcDefaults
    );

/**
* @brief Initialize RFC store.
*
* This function initializes the RFC store for storing remote feature control settings.
*
* @param[in,out] pRfcStore - Pointer to a pointer to the cJSON RFC store object.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlDiRfcStoreInit
    (
        cJSON **pRfcStore
    );

/**
* @brief Activate partner ID.
*
* This function activates the currently configured syndication partner ID.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
activatePartnerId();

/**
* @brief Process RFC set operation.
*
* This function processes RFC parameter set requests and updates the RFC store.
*
* @param[in,out] pRfcStore - Pointer to a pointer to the cJSON RFC store object.
* @param[in] clearDB - Boolean flag to indicate if database should be cleared.
*                      \n TRUE: Clear database before setting.
*                      \n FALSE: Update database without clearing.
* @param[in] paramFullName - Pointer to a null-terminated string containing the full parameter name.
* @param[in] value - Pointer to a null-terminated string containing the parameter value.
* @param[in] pSource - Pointer to a null-terminated string indicating the source of the change.
* @param[in] pCurrentTime - Pointer to a null-terminated string containing the current timestamp.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
ProcessRfcSet(cJSON **pRfcStore, BOOL clearDB, char *paramFullName, char *value, char *pSource, char *pCurrentTime);

/**
* @brief Schedule automatic reboot.
*
* This function schedules an automatic device reboot after the specified uptime.
*
* @param[in] ConfiguredUpTime - Configured uptime in seconds after which device should reboot.
* @param[in] bValue - Boolean value to enable or disable auto-reboot.
*                     \n TRUE: Enable auto-reboot.
*                     \n FALSE: Disable auto-reboot.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if any error is detected during the operation.
*
*/
ANSC_STATUS
CosaDmlScheduleAutoReboot(int ConfiguredUpTime, BOOL bValue);

/**
* @brief Set multi-profile XDNS configuration.
*
* This function enables or disables multi-profile XDNS (eXtended DNS) configuration.
*
* @param[in] bValue - Boolean value to set.
*                     \n TRUE: Enable multi-profile XDNS.
*                     \n FALSE: Disable multi-profile XDNS.
*
* @return The status of the operation.
* @retval 1 if successful.
* @retval 0 if any error occurs.
*
*/
int
setMultiProfileXdnsConfig(BOOL bValue);

#if defined (FEATURE_SUPPORT_RADIUSGREYLIST)
/**
* @brief Set RADIUS grey list enable status.
*
* This function enables or disables the RADIUS grey list feature for access control.
*
* @param[in] bValue - Boolean value to set.
*                     \n TRUE: Enable RADIUS grey list.
*                     \n FALSE: Disable RADIUS grey list.
*
* @return The status of the operation.
* @retval TRUE if the operation is successful.
* @retval FALSE if any error is detected during the operation.
*
*/
BOOL
CosaDmlSetRadiusGreyListEnable
    (
        BOOL        bValue
    );
#endif
#if defined(FEATURE_HOSTAP_AUTHENTICATOR)
/**
* @brief Set native hostapd state.
*
* This function enables or disables the native hostapd authenticator functionality.
*
* @param[in] bValue - Boolean value to set.
*                     \n TRUE: Enable native hostapd.
*                     \n FALSE: Disable native hostapd.
*
* @return The status of the operation.
* @retval TRUE if the operation is successful.
* @retval FALSE if any error is detected during the operation.
*
*/
BOOL
CosaDmlSetNativeHostapdState
    (
        BOOL        bValue
    );
#endif //FEATURE_HOSTAP_AUTHENTICATOR

/**
* @brief Set DFS (Dynamic Frequency Selection) state.
*
* This function enables or disables the Dynamic Frequency Selection feature for
* WiFi operation in DFS channels.
*
* @param[in] bValue - Boolean value to set.
*                     \n TRUE: Enable DFS.
*                     \n FALSE: Disable DFS.
*
* @return The status of the operation.
* @retval TRUE if the operation is successful.
* @retval FALSE if any error is detected during the operation.
*
*/
BOOL CosaDmlSetDFS(BOOL bValue);

/**
* @brief Set DFS state at boot up.
*
* This function configures whether DFS should be enabled or disabled during device boot up initialization.
*
* @param[in] bValue - Boolean value to set.
*                     \n TRUE: Enable DFS at boot up.
*                     \n FALSE: Disable DFS at boot up.
*
* @return The status of the operation.
* @retval TRUE if the operation is successful.
* @retval FALSE if any error is detected during the operation.
*
*/
BOOL CosaDmlSetDFSatBootUp(BOOL bValue);

#endif
