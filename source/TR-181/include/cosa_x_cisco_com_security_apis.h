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

    module: cosa_x_cisco_com_security_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

/*
 * This file defines data types and methods as the middle layer between COSA DM library
 * and backend implementation.
 */

#ifndef  _COSA_X_CISCO_SECURITY_APIS_H
#define  _COSA_X_CISCO_SECURITY_APIS_H

#include "ansc_platform.h"
#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_firewall_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

#define  COSA_DML_IA_LH_MAX_MAC                     10
#define  COSA_DML_IA_LH_MAX_IP                      6
#define  COSA_DML_IA_LH_MAX_IP_RANGE                4
#define  COSA_DML_IA_URL_LENGTH                     256
#define  COSA_DML_IA_KEYWORD_LENGTH                 128


struct
_COSA_DML_SECURITY_CFG
{
    BOOLEAN                         ApplyFirewallSettings;
    BOOLEAN                         CleanLog;
    BOOLEAN                         TriggerEmailLog;
    BOOLEAN                         EmailEnable;
    CHAR                            FilterWebTraffic[64];
    CHAR                            TrafficDetect[64];
    CHAR                            FilterLanTraffic[64];
    CHAR                            AllowPassthrough[64];
    CHAR                            EmailSendTo[64];
    CHAR                            EmailServer[64];
    CHAR                            EmailUserName[64];
    CHAR                            EmailPassword[64];
    CHAR                            EmailFromAddress[64];
    BOOLEAN                         EmailParentalControlBreach;
    BOOLEAN                         EmailAlertsOrWarnings;
    BOOLEAN                         EmailFirewallBreach;
    BOOLEAN                         EmailSendLogs;
}_struct_pack_;

typedef struct _COSA_DML_SECURITY_CFG COSA_DML_SECURITY_CFG,  *PCOSA_DML_SECURITY_CFG;

struct
_COSA_DML_FIREWALL_CFG2
{
    COSA_DML_FIREWALL_LEVEL         FirewallLevel;
    COSA_DML_FIREWALL_LEVEL         FirewallLevelV6;
    BOOLEAN                         WebBlockProxy;
    BOOLEAN                         WebBlockJava;
    BOOLEAN                         WebBlockActiveX;
    BOOLEAN                         WebBlockCookies;
    BOOLEAN                         FilterAnonymousInternetRequests;
    BOOLEAN                         FilterAnonymousInternetRequestsV6;
    BOOLEAN                         FilterMulticast;
    BOOLEAN                         FilterMulticastV6;
    BOOLEAN                         FilterNATRedirection;
    BOOLEAN                         IPSecPassthrough;
    BOOLEAN                         PPTPPassthrough;
    BOOLEAN                         L2TPPassthrough;
    BOOLEAN                         SSLPassthrough;
    BOOLEAN                         FilterHTTP;
    BOOLEAN                         FilterHTTPV6;
    BOOLEAN                         FilterHTTPs;
    BOOLEAN                         FilterHTTPsV6;
    BOOLEAN                         FilterP2P;
    BOOLEAN                         FilterP2PV6;
    BOOLEAN                         FilterIdent;
    BOOLEAN                         FilterIdentV6;
    BOOLEAN                         TrueStaticIpEnable;
    BOOLEAN                         TrueStaticIpEnableV6;
    BOOLEAN                         SmartPktDectionEnable;
    BOOLEAN                         SmartPktDectionEnableV6;
    BOOLEAN                         WanPingEnable;
    BOOLEAN                         WanPingEnableV6;
}_struct_pack_;

typedef struct _COSA_DML_FIREWALL_CFG2 COSA_DML_FIREWALL_CFG2,  *PCOSA_DML_FIREWALL_CFG2;


typedef  enum
_COSA_DML_IA_APP_PROTO
{
    COSA_DML_IA_APP_PROTO_Tcp       = 1,
    COSA_DML_IA_APP_PROTO_Udp,
    COSA_DML_IA_APP_PROTO_Both
}
COSA_DML_IA_APP_PROTO, *PCOSA_DML_IA_APP_PROTO;

typedef  struct
_COSA_DML_IA_POLICY_SCH
{
    BOOLEAN                         AllowAccess;
    BOOLEAN                         IncludeSunday;
    BOOLEAN                         IncludeMonday;
    BOOLEAN                         IncludeTuesday;
    BOOLEAN                         IncludeWednesday;
    BOOLEAN                         IncludeThursday;
    BOOLEAN                         IncludeFriday;
    BOOLEAN                         IncludeSaturday;
    BOOLEAN                         AllDay;
    char                            StartTime[32];
    char                            EndTime[32];
}
COSA_DML_IA_POLICY_SCH,  *PCOSA_DML_IA_POLICY_SCH;


typedef  struct
_COSA_DML_IA_LH_MAC
{
    UCHAR                           Mac[6];
}
COSA_DML_IA_LH_MAC,  *PCOSA_DML_IA_LH_MAC;

typedef  struct
_COSA_DML_IA_LH_IP
{
    ANSC_IPV4_ADDRESS               Ip;
}
COSA_DML_IA_LH_IP,  *PCOSA_DML_IA_LH_IP;

typedef  struct
_COSA_DML_IA_LH_IPR
{
    ANSC_IPV4_ADDRESS               StartIp;
    ANSC_IPV4_ADDRESS               EndIp;
}
COSA_DML_IA_LH_IPR,  *PCOSA_DML_IA_LH_IPR;


typedef  struct
_COSA_DML_IA_LH
{
    BOOLEAN                         bUseLanHosts;
    USHORT                          MacCount;
    USHORT                          IpCount;
    USHORT                          IprCount;
    COSA_DML_IA_LH_MAC              MacList[COSA_DML_IA_LH_MAX_MAC];
    COSA_DML_IA_LH_IP               IpList[COSA_DML_IA_LH_MAX_IP];
    COSA_DML_IA_LH_IPR              IprList[COSA_DML_IA_LH_MAX_IP_RANGE];
}
COSA_DML_IA_LH, *PCOSA_DML_IA_LH;

typedef  struct
_COSA_DML_IA_POLICY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH]; /* This is also the policy name */

    BOOLEAN                         bEnabled;
    BOOLEAN                         EnableLanHostMACAddresses;
    BOOLEAN                         AllowLanHostMACAddresses;
    COSA_DML_IA_LH                  LanHost;
}
COSA_DML_IA_POLICY,  *PCOSA_DML_IA_POLICY;

typedef  struct
_COSA_DML_IA_POLICY2
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH]; /* This is also the policy name */

    BOOLEAN                         bEnabled;
    BOOLEAN                         EnableLanHostMACAddresses;
    BOOLEAN                         AllowLanHostMACAddresses;
    COSA_DML_IA_LH                  LanHost;

    /* Tom Added */
    COSA_DML_IA_POLICY_SCH          Schedule;
    SLIST_HEADER                    URLList;
    SLIST_HEADER                    KeywordList;
    SLIST_HEADER                    AppList;
    ULONG                           ulNextURLInsNum;
    ULONG                           ulNextKeyInsNum;
    ULONG                           ulNextAppInsNum;
}
COSA_DML_IA_POLICY2,  *PCOSA_DML_IA_POLICY2;

typedef  struct
_COSA_DML_IA_POLICY_URL
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    char                            Url[COSA_DML_IA_URL_LENGTH];
}
COSA_DML_IA_POLICY_URL,  *PCOSA_DML_IA_POLICY_URL;

typedef  struct
_COSA_DML_IA_POLICY_KEYWORD
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    char                            Keyword[COSA_DML_IA_KEYWORD_LENGTH];
}
COSA_DML_IA_POLICY_KEYWORD,  *PCOSA_DML_IA_POLICY_KEYWORD;

typedef  struct
_COSA_DML_IA_POLICY_APP
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH]; /* This is also the app name */

    BOOLEAN                         IsWellKnown;
    COSA_DML_IA_APP_PROTO           Protocol;
    USHORT                          StartPortNumber;
    USHORT                          EndPortNumber;
}
COSA_DML_IA_POLICY_APP,  *PCOSA_DML_IA_POLICY_APP;


typedef  struct
_COSA_SECURITY_IA_POLICY2
{
    ULONG                           ulInstanceNumber;
    BOOLEAN                         bEnabled;
    BOOLEAN                         EnableLanHostMACAddresses;
    BOOLEAN                         AllowLanHostMACAddresses;
    char                            Name[64];
    BOOLEAN                         bUseLanHosts;
    COSA_DML_IA_POLICY_SCH          Schedule;
    USHORT                          ulNumOfMac;
    COSA_DML_IA_LH_MAC              MacList[10];
    USHORT                          ulNumOfIp;
    COSA_DML_IA_LH_IP               IPList[6];
    USHORT                          ulNumOfIpr;
    COSA_DML_IA_LH_IPR              IPRList[4];
    USHORT                          ulNumOfUrl;
    COSA_DML_IA_POLICY_URL          URLList[4];
    USHORT                          ulNumOfKeyword;
    COSA_DML_IA_POLICY_KEYWORD      KeywordList[4];
    USHORT                          ulNumOfApp;
    COSA_DML_IA_POLICY_APP          AppList[10];
}
COSA_SECURITY_IA_POLICY2,  *PCOSA_SECURITY_IA_POLICY2;


typedef  struct
_COSA_DML_IA_LOG_ENTRY
{
    /* ANSC_UNIVERSAL_TIME             OccuranceTime; */
    char                            OccuranceTime[32];
    char                            Action[256];
    ANSC_IPV4_ADDRESS               TargetIP;
    char                            User[64];
    ANSC_IPV4_ADDRESS               SourceIP;
    ULONG                           Count;
    char                            Description[64];
}
COSA_DML_IA_LOG_ENTRY,  *PCOSA_DML_IA_LOG_ENTRY;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the Internet Access (IA) module.
*
* This function initializes the Internet Access backend module, which handles security policies,
* email notifications, and firewall logging.
*
* @param[in] hDml - Opaque handle from DM adapter.
* @param[out] phContext - Opaque handle passed back from backend, needed by CosaDmlIaXyz() routines.
*                         \n The buffer pointed by phContext should be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or email file generation fails.
*
*/
ANSC_STATUS
CosaDmlIaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Get the security configuration settings.
*
* This function retrieves security-related configuration including email notification settings.
*
* @param[in] hContext - Reserved for future use.
* @param[out] pCfg - Pointer to a COSA_DML_SECURITY_CFG structure where the security configuration
*                    will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or configuration retrieval fails.
*
*/
ANSC_STATUS
CosaDmlSecurityGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_SECURITY_CFG      pCfg
    );

/**
* @brief Set the security configuration settings.
*
* This function applies security-related configuration including email notification settings.
*
* @param[in] hContext - Reserved for future use.
* @param[in] pCfg - Pointer to a COSA_DML_SECURITY_CFG structure containing the security configuration
*                   to be set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization, configuration set, or file generation fails.
*
*/
ANSC_STATUS
CosaDmlSecuritySetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_SECURITY_CFG      pCfg
    );

/**
* @brief Get the firewall extended configuration settings.
*
* This function retrieves firewall configuration including firewall level, web traffic filtering,
* VPN passthrough settings, and traffic filtering options.
*
* @param[in] hContext - Reserved for future use.
* @param[out] pCfg - Pointer to a COSA_DML_FIREWALL_CFG2 structure where the firewall configuration
*                    will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE otherwise.
*
*/
ANSC_STATUS
CosaDmlFirewallGetConfig2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG2     pCfg
    );

/**
* @brief Set the firewall extended configuration settings.
*
* This function applies firewall configuration including firewall level, web traffic filtering,
* VPN passthrough settings, and traffic filtering options.
*
* @param[in] hContext - Reserved for future use.
* @param[in] pCfg - Pointer to a COSA_DML_FIREWALL_CFG2 structure containing the firewall configuration
*                   to be set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE otherwise.
*
*/
ANSC_STATUS
CosaDmlFirewallSetConfig2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG2     pCfg
    );

#if !defined (RESOURCE_OPTIMIZATION)

/**
* @brief Get the number of Internet Access policies configured.
*
* This function returns the count of Internet Access (IA) policies currently configured in the system.
*
* @param[in] hContext - Reserved for future use.
*
* @return The number of Internet Access policies.
*
*/
ULONG
CosaDmlIaGetNumberOfPolicies
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get an Internet Access policy by index.
*
* This function retrieves an Internet Access policy configuration by its index position.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulIndex - Index of the policy to retrieve (0-based).
* @param[out] pEntry - Pointer to a COSA_DML_IA_POLICY structure where the policy will be returned.
*                      \n The buffer pointed by pEntry should be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy at the specified index does not exist.
*
*/
ANSC_STATUS
CosaDmlIaGetPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY         pEntry
    );

/**
* @brief Set instance number and alias for an Internet Access policy.
*
* This function updates the instance number and alias (policy name) for an existing policy
* identified by its index.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulIndex - Index of the policy to update (0-based).
* @param[in] ulInstanceNumber - Instance number to assign to the policy.
* @param[in] pAlias - Alias (policy name) to assign.
*                     \n The buffer should contain a null-terminated string with maximum length of 64 bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE otherwise.
*
*/
ANSC_STATUS
CosaDmlIaSetPolicyValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Get an Internet Access policy by instance number.
*
* This function retrieves an Internet Access policy configuration by its instance number.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulInsNum - Instance number of the policy to retrieve.
* @param[out] pEntry - Pointer to a COSA_DML_IA_POLICY structure where the policy will be returned.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if no policy with the specified instance number exists.
*
*/
ANSC_STATUS
CosaDmlIaGetPolicyByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInsNum,
        PCOSA_DML_IA_POLICY         pEntry
    );

/**
* @brief Add a new Internet Access policy.
*
* This function creates a new Internet Access policy with the provided configuration.
*
* @param[in] hContext - Reserved for future use.
* @param[in] pEntry - Pointer to a COSA_DML_IA_POLICY structure containing the policy to add.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy addition fails.
*
*/
ANSC_STATUS
CosaDmlIaAddPolicy
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IA_POLICY         pEntry
    );

/**
* @brief Delete an Internet Access policy.
*
* This function removes an Internet Access policy identified by its instance number.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulInstanceNumber - Instance number of the policy to delete.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if no policy with the specified instance number exists.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy deletion fails.
*
*/
ANSC_STATUS
CosaDmlIaDelPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/**
* @brief Update an Internet Access policy configuration.
*
* This function updates an existing Internet Access policy identified by its instance number.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulInstanceNumber - Instance number of the policy to update.
* @param[in] pEntry - Pointer to a COSA_DML_IA_POLICY structure containing the new policy configuration.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if no policy with the specified instance number exists.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaSetPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY         pEntry
    );

/**
* @brief Get the schedule settings for an Internet Access policy.
*
* This function retrieves the time-of-day schedule configuration for a specified policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulInstanceNumber - Instance number of the policy.
* @param[out] pEntry - Pointer to a COSA_DML_IA_POLICY_SCH structure where the schedule will be returned.
*                      \n The buffer pointed by pEntry should be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if no policy with the specified instance number exists.
*
*/
ANSC_STATUS
CosaDmlIaGetPolicySchedule
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY_SCH     pEntry
    );

/**
* @brief Set the schedule settings for an Internet Access policy.
*
* This function updates the time-of-day schedule configuration for a specified policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulInstanceNumber - Instance number of the policy.
* @param[in] pEntry - Pointer to a COSA_DML_IA_POLICY_SCH structure containing the schedule to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if no policy with the specified instance number exists.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaSetPolicySchedule
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY_SCH     pEntry
    );

/**
* @brief Get the number of blocked URLs for a policy.
*
* This function returns the count of blocked URL entries configured for a specific Internet Access policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
*
* @return The number of blocked URL entries in the policy.
*
*/
ULONG
CosaDmlIaPolicyGetNumberOfUrls
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    );

/**
* @brief Get a blocked URL entry by index.
*
* This function retrieves a blocked URL entry from a policy by its index position.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] ulIndex - Index of the URL entry to retrieve (0-based).
* @param[out] pUrl - Pointer to a COSA_DML_IA_POLICY_URL structure where the URL entry will be returned.
*                    \n The buffer pointed by pUrl should be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or URL entry does not exist.
*
*/
ANSC_STATUS
CosaDmlIaPolicyGetUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_URL     pUrl
    );

/**
* @brief Get a blocked URL entry by instance number.
*
* This function retrieves a blocked URL entry from a policy by its instance number.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] ulURLInstanceNumber - Instance number of the URL entry to retrieve.
* @param[out] pUrl - Pointer to a COSA_DML_IA_POLICY_URL structure where the URL entry will be returned.
*                    \n The buffer pointed by pUrl should be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or URL entry with the specified instance number does not exist.
*
*/
ANSC_STATUS
CosaDmlIaPolicyGetUrlByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulURLInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl
    );

/**
* @brief Set instance number and alias for a blocked URL entry.
*
* This function updates the instance number and alias for an existing URL entry identified by its index.

*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] ulIndex - Index of the URL entry to update (0-based).
* @param[in] ulInstanceNumber - Instance number to assign to the URL entry.
* @param[in] pAlias - Alias to assign to the URL entry.
*                     \n The buffer should contain a null-terminated string with maximum length of 64 bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicySetUrlValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a blocked URL entry to a policy.
*
* This function adds a new blocked URL entry to a specified Internet Access policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] pUrl - Pointer to a COSA_DML_IA_POLICY_URL structure containing the URL entry to add.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicyAddUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl
    );

/**
* @brief Delete a blocked URL entry from a policy.
*
* This function removes a blocked URL entry identified by its instance number from a policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] pUrl - Pointer to a COSA_DML_IA_POLICY_URL structure containing the URL entry to delete.
*                   \n The entry is identified by InstanceNumber field only.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or URL entry does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicyDelUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl        /* Identified by InstanceNumber */
    );

/**
* @brief Update a blocked URL entry in a policy.
*
* This function updates an existing blocked URL entry identified by its instance number.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] pUrl - Pointer to a COSA_DML_IA_POLICY_URL structure containing the URL entry to update.
*                   \n The entry is identified by InstanceNumber field.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or URL entry does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicySetUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl        /* Identified by InstanceNumber */
    );

/**
* @brief Get the number of blocked keywords for a policy.
*
* This function returns the count of blocked keyword entries configured for a specific Internet Access policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
*
* @return The number of blocked keyword entries in the policy.
*
*/
ULONG
CosaDmlIaPolicyGetNumberOfKeywords
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    );

/**
* @brief Get a blocked keyword entry by index.
*
* This function retrieves a blocked keyword entry from a policy by its index position.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] ulIndex - Index of the keyword entry to retrieve (0-based).
* @param[out] pKeyword - Pointer to a COSA_DML_IA_POLICY_KEYWORD structure where the keyword entry will be returned.
*                        \n The buffer pointed by pKeyword should be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or keyword entry does not exist.
*
*/
ANSC_STATUS
CosaDmlIaPolicyGetKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    );

/**
* @brief Get a blocked keyword entry by instance number.
*
* This function retrieves a blocked keyword entry from a policy by its instance number.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] ulKeywordInstanceNumber - Instance number of the keyword entry to retrieve.
* @param[out] pKeyword - Pointer to a COSA_DML_IA_POLICY_KEYWORD structure where the keyword entry will be returned.
*                        \n The buffer pointed by pKeyword should be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or keyword entry with the specified instance number does not exist.
*
*/
ANSC_STATUS
CosaDmlIaPolicyGetKeywordByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulKeywordInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    );

/**
* @brief Set instance number and alias for a blocked keyword entry.
*
* This function updates the instance number and alias for an existing keyword entry identified by its index.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] ulIndex - Index of the keyword entry to update (0-based).
* @param[in] ulInstanceNumber - Instance number to assign to the keyword entry.
* @param[in] pAlias - Alias to assign to the keyword entry.
*                     \n The buffer should contain a null-terminated string with maximum length of 64 bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicySetKeywordValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a blocked keyword entry to a policy.
*
* This function adds a new blocked keyword entry to a specified Internet Access policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] pKeyword - Pointer to a COSA_DML_IA_POLICY_KEYWORD structure containing the keyword entry to add.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicyAddKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    );

/**
* @brief Delete a blocked keyword entry from a policy.
*
* This function removes a blocked keyword entry identified by its instance number from a policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] pKeyword - Pointer to a COSA_DML_IA_POLICY_KEYWORD structure containing the keyword entry to delete.
*                       \n The entry is identified by InstanceNumber field only.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or keyword entry does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicyDelKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword    /* Identified by InstanceNumber */
    );

/**
* @brief Update a blocked keyword entry in a policy.
*
* This function updates an existing blocked keyword entry identified by its instance number.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] pKeyword - Pointer to a COSA_DML_IA_POLICY_KEYWORD structure containing the keyword entry to update.
*                       \n The entry is identified by InstanceNumber field. Fields to update: Alias, Keyword.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or keyword entry does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicySetKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword    /* Identified by InstanceNumber */
    );

/**
* @brief Get the number of blocked applications for a policy.
*
* This function returns the count of blocked application entries configured for a specific Internet Access policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
*
* @return The number of blocked application entries in the policy.
*
*/
ULONG
CosaDmlIaPolicyGetNumberOfApps
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    );

/**
* @brief Get a blocked application entry by index.
*
* This function retrieves a blocked application entry from a policy by its index position.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] ulIndex - Index of the application entry to retrieve (0-based).
* @param[out] pApp - Pointer to a COSA_DML_IA_POLICY_APP structure where the application entry will be returned.
*                    \n The buffer pointed by pApp should be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or application entry does not exist.
*
*/
ANSC_STATUS
CosaDmlIaPolicyGetApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_APP     pApp
    );

/**
* @brief Get a blocked application entry by instance number.
*
* This function retrieves a blocked application entry from a policy by its instance number.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] ulAppInstanceNumber - Instance number of the application entry to retrieve.
* @param[out] pApp - Pointer to a COSA_DML_IA_POLICY_APP structure where the application entry will be returned.
*                    \n The buffer pointed by pApp should be allocated by the caller.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or application entry with the specified instance number does not exist.
*
*/
ANSC_STATUS
CosaDmlIaPolicyGetAppByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulAppInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    );

/**
* @brief Set instance number and alias for a blocked application entry.
*
* This function updates the instance number and alias (application name) for an existing application
* entry identified by its index.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] ulIndex - Index of the application entry to update (0-based).
* @param[in] ulInstanceNumber - Instance number to assign to the application entry.
* @param[in] pAlias - Alias (application name) to assign to the application entry.
*                     \n The buffer should contain a null-terminated string with maximum length of 64 bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicySetAppValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

/**
* @brief Add a blocked application entry to a policy.
*
* This function adds a new blocked application entry to a specified Internet Access policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] pApp - Pointer to a COSA_DML_IA_POLICY_APP structure containing the application entry to add.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicyAddApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    );

/**
* @brief Delete a blocked application entry from a policy.
*
* This function removes a blocked application entry identified by its instance number from a policy.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] pApp - Pointer to a COSA_DML_IA_POLICY_APP structure containing the application entry to delete.
*                   \n The entry is identified by InstanceNumber field only.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or application entry does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicyDelBlockedApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp        /* Identified by InstanceNumber */
    );

/**
* @brief Update a blocked application entry in a policy.
*
* This function updates an existing blocked application entry identified by its instance number.
*
* @param[in] hContext - Reserved for future use.
* @param[in] ulPolicyInstanceNumber - Instance number of the policy.
* @param[in] pApp - Pointer to a COSA_DML_IA_POLICY_APP structure containing the application entry to update.
*                   \n The entry is identified by InstanceNumber field.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_CANT_FIND if the policy or application entry does not exist.
* @retval ANSC_STATUS_FAILURE if Utopia initialization or policy update fails.
*
*/
ANSC_STATUS
CosaDmlIaPolicySetBlockedApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp        /* Identified by InstanceNumber */
    );

#endif

/**
* @brief Get firewall log entries for Internet Access policies.
*
* This function retrieves firewall log entries related to Internet Access policy violations.
*
* @param[in] hContext - Reserved for future use.
* @param[out] pulCount - Pointer to a ULONG variable where the count of log entries will be returned.
*                        \n The buffer pointed by pulCount should be allocated by the caller.
*
* @return Pointer to firewall log entries.
* @retval Pointer to an array of COSA_DML_IA_LOG_ENTRY structures.
* @retval NULL if no logs are available or on first call.
*
*/
PCOSA_DML_IA_LOG_ENTRY
CosaDmlIaGetLogEntries
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

/**
* @brief Check if firewall logs have been updated.
*
* This function checks if the firewall log cache should be refreshed based on a 30-second
* interval (REFRESH_INTERVAL).
*
* @param[in] hContext - Reserved for future use.
*
* @return The update status.
* @retval TRUE if logs should be refreshed.
* @retval FALSE otherwise.
*
*/
BOOL
CosaDmlIaLogIsUpdated
    (
        ANSC_HANDLE                 hContext
    );

/**
* @brief Get all firewall log entries as formatted string.
*
* This function retrieves all firewall log entries and formats them as a newline-separated string.
*
* @param[out] pValue - Pointer to a character buffer where the formatted log entries will be returned.
*                      \n The buffer should be allocated by the caller with size specified in pUlSize.
* @param[in,out] pUlSize - Pointer to a ULONG variable containing the size of pValue buffer.
*                          \n On input: available buffer size. On output: required size.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if logs were successfully retrieved and formatted.
* @retval ANSC_STATUS_FAILURE if buffer size is insufficient.
*
*/
ANSC_STATUS
CosaDmlIaGetALLLogEntries
    (
        char*                          pValue,
        ULONG*                         pUlSize
    );

/**
* @brief Check if an application name is a well-known network service.
*
* This function determines if a given application name matches one of the predefined well-known
* network services.
*
* @param[in] name - Null-terminated string containing the application/service name to check.
*
* @return The status of operation.
* @retval TRUE if the name matches a well-known service (case-insensitive)
* @retval FALSE otherwise.
*
*/
BOOL isWellKnownService (const char *name);

/**
* @brief Clean up Internet Access module resources.
*
* This function releases all resources allocated by the Internet Access module.
*
*/
void CosaDmlIaRemove();
#endif
