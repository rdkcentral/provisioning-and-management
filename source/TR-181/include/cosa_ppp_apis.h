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

    module: cosa_ppp_apis.h

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


#ifndef  _COSA_PPP_APIS_H
#define  _COSA_PPP_APIS_H

#include "cosa_apis.h"

#define MAXINSTANCE                 128

#define COSA_DML_PPP_SUPPORTED_NCP_ATCP   0x01
#define COSA_DML_PPP_SUPPORTED_NCP_IPCP   0x02
#define COSA_DML_PPP_SUPPORTED_NCP_IPXCP  0x04
#define COSA_DML_PPP_SUPPORTED_NCP_NBFCP  0x08
#define COSA_DML_PPP_SUPPORTED_NCP_IPv6CP 0x10

typedef  enum
_COSA_DML_PPP_LINK_TYPE
{
    COSA_DML_PPP_LINK_TYPE_Eth       = 1,
    COSA_DML_PPP_LINK_TYPE_EthVlan,
    COSA_DML_PPP_LINK_TYPE_Usb,
    COSA_DML_PPP_LINK_TYPE_Hpna,
    COSA_DML_PPP_LINK_TYPE_Moca,
    COSA_DML_PPP_LINK_TYPE_HomePlug,
    COSA_DML_PPP_LINK_TYPE_Upa,
    COSA_DML_PPP_LINK_TYPE_WiFiSsid,
    COSA_DML_PPP_LINK_TYPE_Bridge,
    COSA_DML_PPP_LINK_TYPE_PPP
}
COSA_DML_PPP_LINK_TYPE, *PCOSA_DML_PPP_LINK_TYPE;


typedef  enum
_COSA_DML_PPP_CONN_STATUS
{
    COSA_DML_PPP_CONN_STATUS_Unconfigured           = 1,
    COSA_DML_PPP_CONN_STATUS_Connecting,
    COSA_DML_PPP_CONN_STATUS_Authenticating,
    COSA_DML_PPP_CONN_STATUS_Connected,
    COSA_DML_PPP_CONN_STATUS_PendingDisconnect,
    COSA_DML_PPP_CONN_STATUS_Disconnecting,
    COSA_DML_PPP_CONN_STATUS_Disconnected
}
COSA_DML_PPP_CONN_STATUS, *PCOSA_DML_PPP_CONN_STATUS;


typedef  enum
_COSA_DML_PPP_CONN_ERROR
{
    COSA_DML_PPP_CONN_ERROR_NONE                    = 1,
    COSA_DML_PPP_CONN_ERROR_ISP_TIME_OUT,
    COSA_DML_PPP_CONN_ERROR_COMMAND_ABORTED,
    COSA_DML_PPP_CONN_ERROR_NOT_ENABLED_FOR_INTERNET,
    COSA_DML_PPP_CONN_ERROR_BAD_PHONE_NUMBER,
    COSA_DML_PPP_CONN_ERROR_USER_DISCONNECT,
    COSA_DML_PPP_CONN_ERROR_ISP_DISCONNECT,
    COSA_DML_PPP_CONN_ERROR_IDLE_DISCONNECT,
    COSA_DML_PPP_CONN_ERROR_FORCED_DISCONNECT,
    COSA_DML_PPP_CONN_ERROR_SERVER_OUT_OF_RESOURCES,
    COSA_DML_PPP_CONN_ERROR_RESTRICTED_LOGON_HOURS,
    COSA_DML_PPP_CONN_ERROR_ACCOUNT_DISABLED,
    COSA_DML_PPP_CONN_ERROR_ACCOUNT_EXPIRED,
    COSA_DML_PPP_CONN_ERROR_PASSWORD_EXPIRED,
    COSA_DML_PPP_CONN_ERROR_AUTHENTICATION_FAILURE,
    COSA_DML_PPP_CONN_ERROR_NO_DIALTONE,
    COSA_DML_PPP_CONN_ERROR_NO_CARRIER,
    COSA_DML_PPP_CONN_ERROR_NO_ANSWER,
    COSA_DML_PPP_CONN_ERROR_LINE_BUSY,
    COSA_DML_PPP_CONN_ERROR_UNSUPPORTED_BITSPERSECOND,
    COSA_DML_PPP_CONN_ERROR_TOO_MANY_LINE_ERRORS,
    COSA_DML_PPP_CONN_ERROR_IP_CONFIGURATION,
    COSA_DML_PPP_CONN_ERROR_UNKNOWN
}
COSA_DML_PPP_CONN_ERROR, *PCOSA_DML_PPP_CONN_ERROR;


typedef  enum
_COSA_DML_PPP_ENCRYPTION
{
    COSA_DML_PPP_ENCRYPTION_None                    = 1,
    COSA_DML_PPP_ENCRYPTION_MPPE
}
COSA_DML_PPP_ENCRYPTION, *PCOSA_DML_PPP_ENCRYPTION;


typedef  enum
_COSA_DML_PPP_COMPRESSION
{
    COSA_DML_PPP_COMPRESSION_None                   = 1,
    COSA_DML_PPP_COMPRESSION_VanJacobson            ,
    COSA_DML_PPP_COMPRESSION_Lzs
}
COSA_DML_PPP_COMPRESSION, *PCOSA_DML_PPP_COMPRESSION;


typedef  enum
_COSA_DML_PPP_AUTH
{
    COSA_DML_PPP_AUTH_PAP                           = 1,
    COSA_DML_PPP_AUTH_CHAP,
    COSA_DML_PPP_AUTH_MS_CHAP
}
COSA_DML_PPP_AUTH, *PCOSA_DML_PPP_AUTH;


typedef  enum
_COSA_DML_PPP_CONN_TRIGGER
{
    COSA_DML_PPP_CONN_TRIGGER_OnDemand              = 1,
    COSA_DML_PPP_CONN_TRIGGER_AlwaysOn,
    COSA_DML_PPP_CONN_TRIGGER_Manual
}
COSA_DML_PPP_CONN_TRIGGER, *PCOSA_DML_PPP_CONN_TRIGGER;



/*
 *  Structure definitions for PPP Interface
 */
typedef  struct
_COSA_DML_PPP_IF_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_PPP_LINK_TYPE          LinkType;       /* LinkType/LinkName constitutes LowerLayers */
    char                            LinkName[COSA_DML_IF_NAME_LENGTH];
    ULONG                           AutoDisconnectTime;
    ULONG                           IdleDisconnectTime;
    ULONG                           WarnDisconnectDelay;
    char                            Username[65];
    char                            Password[65];
    USHORT                          MaxMRUSize;
    COSA_DML_PPP_CONN_TRIGGER       ConnectionTrigger;
    /*
     *  PPPoE
     */
    char                            ACName[257];
    char                            ServiceName[257];
    /*
     *  IPCP
     */
    BOOLEAN                         PassthroughEnable;
    char                            PassthroughDHCPPool[COSA_DML_IF_NAME_LENGTH];   /* Alias of the DHCP pool */
}
COSA_DML_PPP_IF_CFG,  *PCOSA_DML_PPP_IF_CFG;


typedef  struct
_COSA_DML_PPP_IF_INFO
{
    COSA_DML_IF_STATUS              Status;
    char                            Name[64];       /* netdev name in Linux */
    ULONG                           LastChange;
    COSA_DML_PPP_CONN_STATUS        ConnectionStatus;
    COSA_DML_PPP_CONN_ERROR         LastConnectionError;
    COSA_DML_PPP_ENCRYPTION         EncryptionProtocol;
    COSA_DML_PPP_COMPRESSION        CompressionProtocol;
    COSA_DML_PPP_AUTH               AuthenticationProtocol;
    USHORT                          CurrentMRUSize;
    ULONG                           LCPEcho;
    ULONG                           LCPEchoRetry;
    /*
     *  PPPoE
     */
    ULONG                           SessionID;
    /*
     *  IPCP
     */
    ANSC_IPV4_ADDRESS               LocalIPAddress;
    ANSC_IPV4_ADDRESS               RemoteIPAddress;
    ANSC_IPV4_ADDRESS               DNSServers[2];
}
COSA_DML_PPP_IF_INFO,  *PCOSA_DML_PPP_IF_INFO;


typedef  struct
_COSA_DML_PPP_IF_FULL
{
    COSA_DML_PPP_IF_CFG             Cfg;
    COSA_DML_PPP_IF_INFO            Info;
}
COSA_DML_PPP_IF_FULL, *PCOSA_DML_PPP_IF_FULL;


/**
 * @brief Initialize the Point-to-Point Protocol (PPP) interface subsystem.
 *
 * This function initializes the PPP interface subsystem during component startup.
 *
 * @param[in] hDml - Handle to the Data Model Library context.
 * @param[in,out] phContext - Pointer to handle for storing the initialized context.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if initialization completed successfully.
 */
ANSC_STATUS
CosaDmlPppInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
 * @brief Retrieve the supported Network Control Protocols (NCPs) for PPP.
 *
 * This function retrieves a bitmask indicating which Network Control Protocols
 * are supported by the PPP implementation.
 *
 * @param[in] hContext - Handle to the PPP context.
 * @param[out] puLong - Pointer to ULONG where the supported NCPs bitmask will be stored.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the supported NCPs were successfully retrieved.
 */
ANSC_STATUS
CosaDmlPPPGetSupportedNCPs
    (
        ANSC_HANDLE                 hContext,
        PULONG                      puLong
    );

/*
 *  PPP Interface
 */

/**
 * @brief Retrieve the number of PPP interface entries.
 *
 * This function retrieves the total number of PPP interface entries currently
 * configured in the system.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 *
 * @return The number of PPP interface entries in the system.
 * @retval Actual number of configured PPP interface entries.
 */
ULONG
CosaDmlPppIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

/**
 * @brief Retrieve the complete information of a PPP interface entry by index.
 *
 * This function retrieves the full configuration and status information for a PPP
 * interface designated by the specified index.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 * @param[in] ulIndex - Zero-based index of the PPP interface entry to retrieve.
 * @param[out] pEntry - Pointer to COSA_DML_PPP_IF_FULL structure where the interface
 *                      information will be stored.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the interface entry was successfully retrieved.
 * @retval ANSC_STATUS_FAILURE if pEntry is NULL or index is out of range.
 */
ANSC_STATUS
CosaDmlPppIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_PPP_IF_FULL       pEntry
    );

/**
 * @brief Set the instance number and alias for a PPP interface entry.
 *
 * This backend implementation function is used to update the instance number and alias
 * for a PPP interface entry.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 * @param[in] ulIndex - Zero-based index of the interface entry to update.
 * @param[in] ulInstanceNumber - The instance number to assign to the interface entry.
 * @param[in] pAlias - Pointer to a null-terminated string containing the alias name.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the values were successfully set.
 */
ANSC_STATUS
CosaDmlPppIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
 * @brief Add a new PPP interface entry to the system.
 *
 * This function adds a new PPP interface entry to the system's interface table.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 * @param[in] pEntry - Pointer to COSA_DML_PPP_IF_FULL structure containing the complete
 *                     interface configuration and information to be added.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the interface entry was successfully added.
 * @retval ANSC_STATUS_FAILURE if pEntry is NULL or maximum instances reached.
 */
ANSC_STATUS
CosaDmlPppIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_FULL       pEntry
    );

/**
 * @brief Delete a PPP interface entry from the system.
 *
 * This function removes a PPP interface entry from the system's interface table identified by the specified instance number.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 * @param[in] ulInstanceNumber - The instance number of the interface entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the interface entry was successfully deleted.
 * @retval ANSC_STATUS_CANT_FIND if no entry with the specified instance number exists.
 *
 */
ANSC_STATUS
CosaDmlPppIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
 * @brief Set the configuration for a PPP interface.
 *
 * This function updates the configuration parameters for a PPP interface identified by the instance number in pCfg.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 * @param[in] pCfg - Pointer to COSA_DML_PPP_IF_CFG structure containing the new
 *                   configuration parameters.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the configuration was successfully set.
 * @retval ANSC_STATUS_FAILURE if instance number is invalid or configuration fails.
 * @retval ANSC_STATUS_CANT_FIND if no entry with the specified instance number exists.
 */
ANSC_STATUS
CosaDmlPppIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

/**
 * @brief Retrieve the configuration for a PPP interface.
 *
 * This function retrieves the current configuration parameters for a PPP interface
 * identified by the instance number in pCfg.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 * @param[in,out] pCfg - Pointer to COSA_DML_PPP_IF_CFG structure.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the configuration was successfully retrieved.
 * @retval ANSC_STATUS_FAILURE if pCfg is NULL, instance number is invalid, or configuration fails.
 * @retval ANSC_STATUS_CANT_FIND if no entry with the specified instance number exists.
 */
ANSC_STATUS
CosaDmlPppIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

/**
 * @brief Retrieve the runtime information for a PPP interface.
 *
 * This function retrieves the runtime status and operational information for a PPP
 * interface identified by the specified instance number.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 * @param[in] ulInstanceNumber - The instance number of the interface entry.
 * @param[out] pInfo - Pointer to COSA_DML_PPP_IF_INFO structure where the runtime
 *                     information will be stored.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the information was successfully retrieved.
 * @retval ANSC_STATUS_FAILURE if pInfo is NULL, instance number is invalid.
 * @retval ANSC_STATUS_CANT_FIND if no entry with the specified instance number exists.
 */
ANSC_STATUS
CosaDmlPppIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_PPP_IF_INFO       pInfo
    );

/**
 * @brief Reset a PPP interface to reinitiate the connection.
 *
 * This function resets the specified PPP interface by triggering a WAN restart operation.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 * @param[in] ulInstanceNumber - The instance number of the interface entry to reset.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the interface was successfully reset.
 */
ANSC_STATUS
CosaDmlPppIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/*
 *  PPP Interface statistics
 */

/**
 * @brief Retrieve interface statistics for a PPP interface.
 *
 * This function retrieves the current network traffic statistics for the specified PPP interface.
 *
 * @param[in] hContext - Handle to the PPP interface context.
 * @param[in] ulPppIfInstanceNumber - The instance number of the PPP interface.
 * @param[out] pStats - Pointer to COSA_DML_IF_STATS structure where the statistics will be stored.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the statistics were successfully retrieved.
 * @retval ANSC_STATUS_FAILURE if pStats is NULL or interface statistics retrieval fails.
 */
ANSC_STATUS
CosaDmlPppIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPppIfInstanceNumber,
        PCOSA_DML_IF_STATS          pStats
);

/**
 * @brief Remove and cleanup PPP API resources.
 *
 * This function performs cleanup and removal of resources associated with the PPP API subsystem.
 *
 * @param[in] hContext - Handle to the PPP API context to be removed.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if PPP API resources were successfully removed or cleanup completed.
 */
ANSC_STATUS
CosaPPPApiRemove(ANSC_HANDLE  hContext);

#endif
