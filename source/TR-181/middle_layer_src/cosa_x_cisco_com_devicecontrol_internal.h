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

    module: cosa_x_cisco_com_devicecontrol_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        07/15/2011    initial revision.

**************************************************************************/

#ifndef  _COSA_X_CISCO_COM_DEVICECONTROL_INTERNAL_H
#define  _COSA_X_CISCO_COM_DEVICECONTROL_INTERNAL_H
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_devicecontrol_apis.h"

#define COSA_IREP_FOLDER_NAME_LAN_MANAGEMENT    "LanManagement"

#define COSA_DML_RR_NAME_LAN_MANAGEMENT_Alias          "LMAlias"
#define COSA_DML_RR_NAME_LAN_MANAGEMENT_InsNum         "LMInstanceNumber"
#define COSA_DML_RR_NAME_LAN_MANAGEMENT_NextInsNumber  "LMNextInstanceNumber"
#define COSA_DML_RR_NAME_LAN_MANAGEMENT_NewlyAdded     "LMNewlyAdded"

typedef  enum
_COSA_DML_DEVICE_MODE
{
    COSA_DML_DEVICE_MODE_Bridge = 1,
    COSA_DML_DEVICE_MODE_Ipv4,
    COSA_DML_DEVICE_MODE_Ipv6,
    COSA_DML_DEVICE_MODE_Dualstack
}
COSA_DML_DEVICE_MODE,*PCOSA_DML_DEVICE_MODE;

typedef  enum
_COSA_DML_MultiHomedHSD_FLAG
{
    COSA_DML_MultiHomedHSD_Primary  = 1,
    COSA_DML_MultiHomedHSD_BYOI,
    COSA_DML_MultiHomedHSD_None
}
COSA_DML_MultiHomedHSD_FLAG, *PCOSA_DML_MultiHomedHSD_FLAG;

typedef  enum
_COSA_DML_MultiHomedUIPageControl
{
    COSA_DML_MultiHomedUIPageControl_DOCSIS  = 1,
    COSA_DML_MultiHomedUIPageControl_NON_DOCSIS,
    COSA_DML_MultiHomedUIPageControl_User
}
COSA_DML_MultiHomedUIPageControl, *PCOSA_DML_MultiHomedUIPageControl;

typedef  enum
_COSA_DML_MultiHomedMode
{
    COSA_DML_MultiHomedMode_Cable  = 1,
    COSA_DML_MultiHomedMode_EthWan,
    COSA_DML_MultiHomedMode_None
}
COSA_DML_MultiHomedMode, *PCOSA_DML_MultiHomedMode;

#define  COSA_DATAMODEL_DEVICECONTROL_CLASS_CONTENT                         \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of DeviceControl object class content */                       \
    COSA_DML_MultiHomedHSD_FLAG             HsdFlag;                        \
    COSA_DML_MultiHomedUIPageControl        UIPageControl;                  \
    COSA_DML_MultiHomedMode                 Mode;                           \
    BOOL                                    BridgingStatus;                 \
    /* wan-dns */                                                           \
    COSA_DML_WanAddrMode                    WanAddrMode;                    \
    ANSC_IPV4_ADDRESS                       WanIPAddr;                      \
    ANSC_IPV4_ADDRESS                       WanSubnetMask;                  \
    ANSC_IPV4_ADDRESS                       WanGateway;                     \
    ANSC_IPV4_ADDRESS                       WanSecIPAddr;                   \
    BOOL                                    WanSecIPRIPAdv;                 \
    ANSC_IPV4_ADDRESS                       WanBackupGateway;               \
    BOOL                                    EnableStaticNameServer;         \
    ANSC_IPV4_ADDRESS                       NameServer1;                    \
    ANSC_IPV4_ADDRESS                       NameServer2;                    \
    UCHAR                                   HostName[256];                  \
    UCHAR                                   DomainName[256];                \
    UCHAR                                   StaticDomainName[256];                \
    BOOL                                    ReleaseWan;                     \
    BOOL                                    RenewWan;                       \
    /* Lan-Management */                                                    \
    SLIST_HEADER                            LanMngmList;                    \
    ULONG                                   ulLanMngmNextInsNum;            \
    ANSC_HANDLE                             hIrepFolderCOSA;                \
    ANSC_HANDLE                             hIrepFolderLm;                  \
    COSA_DML_DEVICE_MODE                    DeviceMode;                     \
    BOOL                                    ResetDefaultEnable;             \
    UCHAR                                   RebootDevice[256];              \
    UCHAR                                   FactoryReset[256];               \
    UCHAR                                   UserChangedFlags[64];           \
    BOOL                                    TelnetEnable;                   \
    BOOL                                    ReinitCmMac;                       \
    BOOL                                    SSHEnable;                      \
    BOOL                                    HNAPEnable;                     \
    BOOL                                    EnableZeroConfig;               \
    UCHAR                                   SNMPEnable[32];                 \
    UCHAR                                   DeviceConfigIgnore[32];         \
    BOOL                                    HTTPEnable;                     \
    BOOL                                    HTTPSEnable;                     \
    ULONG                                   HTTPPort;                     \
    ULONG                                   HTTPSPort;                     \
    ULONG                                   ReinitMacThreshold;                     \
    BOOL                                    WebServerChanged;               \
    ULONG                                   IGMPSnoopingEnable;                     \
    ULONG                                   WebUITimeout;                     \
    BOOL                                    EnableMonitorUserChangedParams; \
    ULONG                                   UserOpComponents;               \
    BOOL                                   bFactoryResetChanged;               \
    BOOL                                   bResetChanged;               \
    BOOL                                   EnableMsoRemoteMgmt;               \
    BOOL                                   EnableCusadminRemoteMgmt;               \
    char                                   GuestPassword[33];               \
    ULONG                                    NoOfGuests;               \
    char                                   ParentalControlPassword[33];               \
    char                                   ParentalControlDefaultPwd[33];               \
    char                                   ParentalControlQuestion[33];               \
    char                                   ParentalControlAnswer[33];               \
    BOOL                                   bXHSPortEnabled;              \
    BOOL                                   bXHSPortChanged;    \


typedef  struct
_COSA_DATAMODEL_DEVICECONTROL_CLASS_CONTENT
{
    COSA_DATAMODEL_DEVICECONTROL_CLASS_CONTENT
}
COSA_DATAMODEL_DEVICECONTROL, *PCOSA_DATAMODEL_DEVICECONTROL;

/*
    Standard function declaration
*/
/**
 * @brief Creates a new Cisco DeviceControl backend object.
 *
 * @return Handle to the Cisco DeviceControl backend object
 * @retval Non-NULL Handle to the newly created Cisco DeviceControl backend object.
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaDeviceControlCreate
    (
        VOID
    );

/**
 * @brief Initializes the Cisco DeviceControl backend object.
 *
 * @param[in] hThisObject Handle to the Cisco DeviceControl backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDeviceControlInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the Cisco DeviceControl backend object.
 *
 * @param[in] hThisObject Handle to the Cisco DeviceControl backend object to be removed.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful removal.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDeviceControlRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Loads user-changed parameters control settings from persistent registry.
 *
 * @param[in] hThisObject Handle to the Cisco DeviceControl backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful load.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDevCtrlReg_GetUserChangedParamsControl
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Saves user-changed parameters control settings to persistent registry.
 *
 * @param[in] hThisObject Handle to the Cisco DeviceControl backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful save.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDevCtrlReg_SetUserChangedParamsControl
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Retrieves list of user-changed parameters.
 *
 * @param[in] hThisObject Handle to the Cisco DeviceControl backend object.
 * @param[out] pValue Pointer to buffer to receive the comma-separated parameter list.
 * @param[in,out] pulSize Pointer to size variable; input: buffer size, output: actual string length.
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 if error.
 */
ULONG CosaDevCtrlReg_GetUserChangedParams(
        ANSC_HANDLE                 hThisObject,
        char*                       pValue,
        ULONG*                      pulSize
    );
/**
* @brief Loads LAN management entries from persistent registry.
*
* @param[in] hThisObject - Handle to the Cisco DeviceControl backend object.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval error code otherwise.
*/
ANSC_STATUS
CosaDevCtrlReg_GetLanMngmInfo(
        ANSC_HANDLE                 hThisObject
    );
/**
* @brief Adds a new LAN management entry to persistent registry.
*
* @param[in] hThisObject  - Handle to the Cisco DeviceControl backend object.
* @param[in] hCosaContext - Handle to the COSA context object containing the LAN management entry to be added.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval error code otherwise.
*/
ANSC_STATUS
CosaDevCtrlReg_AddLanMngmInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
/**
* @brief Deletes a LAN management entry from persistent registry.
*
* @param[in] hThisObject  - Handle to the Cisco DeviceControl backend object.
* @param[in] hCosaContext - Handle to the COSA context object containing the LAN management entry to be deleted.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval error code otherwise.
*/
ANSC_STATUS
CosaDevCtrlReg_DelLanMngmInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
#endif
