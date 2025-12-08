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

    module: cosa_nat_apis.h

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


#ifndef  _COSA_NAT_APIS_H
#define  _COSA_NAT_APIS_H

#include "cosa_apis.h"
#include "cosa_nat_apis_custom.h"

#include "cosa_drg_common.h"
#include <telemetry_busmessage_sender.h>

#define COSA_NAT_SYSCFG_NAMESPACE "CosaNAT"
#define COSA_NAT_ID_SYSCFG_NAMESPACE COSA_NAT_SYSCFG_NAMESPACE"IDs"
#define PORT_TRIGGER_ENABLED_KEY "port_trigger_enabled"
#define PORT_FORWARD_ENABLED_KEY "port_forward_enabled"
#define HS_PORT_FORWARD_ENABLED_KEY "port_hs_forward_enabled"
#define NAT_FORWARD_ENABLED_KEY "nat_forward_enabled"

extern volatile bool gWanStatus ; 
#if defined(FEATURE_MAPT) || defined(FEATURE_SUPPORT_MAPT_NAT46)
extern volatile int gMaptTotalPorts ;
extern volatile bool gMaptEnabled ;
#endif

/***********************************
    Actual definition declaration
************************************/

/*
    Nat Part
*/
typedef  enum
_COSA_DML_NAT_STATUS
{
    COSA_DML_NAT_STATUS_Disabled                = 1,
    COSA_DML_NAT_STATUS_Enabled,
    COSA_DML_NAT_STATUS_Error_Misconfigured,
    COSA_DML_NAT_STATUS_Error
}
COSA_DML_NAT_STATUS, *PCOSA_DML_NAT_STATUS;

typedef  struct
_COSA_DML_NAT
{
    ULONG                           X_CISCO_COM_TCPTimeout;
    ULONG                           X_CISCO_COM_UDPTimeout;
    ULONG                           X_CISCO_COM_ICMPTimeout;

    COSA_DML_NAT_CUSTOM
}
COSA_DML_NAT, *PCOSA_DML_NAT;

typedef  struct
_COSA_DML_NAT_DMZ
{
    BOOLEAN                         bEnabled;
    char                            RemoteIPStart[COSA_DML_IP_ADDR_LENGTH ];
    char                            RemoteIPEnd  [COSA_DML_IP_ADDR_LENGTH ];
    char                            InternalIP   [COSA_DML_IP_ADDR_LENGTH ];
    char                            InternalMAC  [COSA_DML_MAC_ADDR_LENGTH];
    char                            IPv6Host     [64];
}
COSA_DML_NAT_DMZ, *PCOSA_DML_NAT_DMZ;

typedef enum
{
    COSA_DML_NAT_PMAPPING_Origin_Static = 1,
    COSA_DML_NAT_PMAPPING_Origin_Dynamic
}
COSA_DML_NAT_PMAPPING_Origin;

typedef  struct
_COSA_DML_NAT_PMAPPING
{
    ULONG                           InstanceNumber;
    CHAR                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_NAT_STATUS             Status;
    CHAR                            Interface[COSA_DML_IF_NAME_LENGTH]; /* IP interface name */
    BOOLEAN                         AllInterfaces;
    ULONG                           LeaseDuration;
    ANSC_IPV4_ADDRESS               RemoteHost;
    ANSC_IPV4_ADDRESS               PublicIP;
    USHORT                          ExternalPort;
    USHORT                          ExternalPortEndRange;
    USHORT                          InternalPort;
    UCHAR                           Protocol;                           /* IP protocol number for TCP, UDP */
    ANSC_IPV4_ADDRESS               InternalClient;
    char                            Description[256];
    char                            X_CISCO_COM_InternalClientV6[64];
    COSA_DML_NAT_PMAPPING_Origin    X_CISCO_COM_Origin;
}
COSA_DML_NAT_PMAPPING,  *PCOSA_DML_NAT_PMAPPING;

typedef  struct
_COSA_DML_NAT_PTRIGGER
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_LENGTH];
    char*                           pOriAlias;

    BOOLEAN                         bEnabled;
    UCHAR                           TriggerProtocol;
    USHORT                          TriggerPortStart;
    USHORT                          TriggerPortEnd;
    UCHAR                           ForwardProtocol;
    USHORT                          ForwardPortStart;
    USHORT                          ForwardPortEnd;
    char                            Description[64];
}
COSA_DML_NAT_PTRIGGER,  *PCOSA_DML_NAT_PTRIGGER;

#define COSA_DML_NAT_PTRIGGER_INIT(pPortTrigger)                                            \
{                                                                                           \
    (pPortTrigger)->bEnabled                 = FALSE;                                       \
    (pPortTrigger)->pOriAlias                = NULL;                                        \
    (pPortTrigger)->TriggerProtocol          = 0;                                           \
    (pPortTrigger)->TriggerPortStart         = 0;                                           \
    (pPortTrigger)->TriggerPortEnd           = 0;                                           \
    (pPortTrigger)->ForwardProtocol          = 0;                                           \
    (pPortTrigger)->ForwardPortStart         = 0;                                           \
    (pPortTrigger)->ForwardPortEnd           = 0;                                           \
}                                                                                           \


/*
Description:
    This callback routine is provided for backend to call middle layer to generate instance number/alias, when backend loads a NAT port mapping from persistent configuration and finds out it doesn't contain instance number/alias.
Arguments:
    hDml        Opaque handle passed in from CosaDmlNatInit.
    pEntry        Pointer to NAT port mapping to receive the generated values.
Return:
    Status of operation


*/
typedef ANSC_STATUS
(*PFN_COSA_DML_NAT_GEN)
    (
        ANSC_HANDLE                 hDml,
        PCOSA_DML_NAT_PMAPPING      pEntry
);


/*************************************
    The actual function declaration
**************************************/

/**
 * @brief Initialize the NAT backend subsystem.
 *
 * This is the initialization routine for the NAT backend layer.
 *
 * @param[in] hDml - Opaque handle from Data Model adapter.
 * @param[out] phContext - Opaque handle passed back from backend, needed by subsequent
 *                         CosaDmlNatXyz() API routines for context.
 * @param[in] pValueGenFn - Function pointer to instance number/alias generation callback.
 *
 * @return The status of the initialization operation.
 * @retval ANSC_STATUS_SUCCESS if NAT backend initialization is successful.
 * @retval ANSC_STATUS_FAILURE if initialization fails.
 */
ANSC_STATUS
CosaDmlNatInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext,
        PFN_COSA_DML_NAT_GEN        pValueGenFn
    );

/**
 * @brief Get global NAT configuration parameters.
 *
 * This function retrieves the current global NAT configuration settings.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[out] pDmlNat - Pointer to NAT configuration structure to receive current settings.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if NAT configuration retrieval is successful.
 * @retval ANSC_STATUS_FAILURE if retrieval fails.
 */
ANSC_STATUS
CosaDmlNatGet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    );

/**
 * @brief Set global NAT configuration parameters.
 *
 * This function applies new global NAT configuration settings.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in] pDmlNat - Pointer to NAT configuration structure containing new settings to apply.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if NAT configuration update is successful.
 * @retval ANSC_STATUS_FAILURE if update fails.
 */
ANSC_STATUS
CosaDmlNatSet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    );

/**
 * @brief Get NAT DMZ configuration.
 *
 * This function retrieves the current NAT DMZ host configuration.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[out] pDmlDmz - Pointer to DMZ configuration structure to receive current settings.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if DMZ configuration retrieval is successful.
 * @retval ANSC_STATUS_FAILURE if retrieval fails.
 */
ANSC_STATUS
CosaDmlNatGetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmlDmz
    );

/**
 * @brief Set NAT DMZ configuration.
 *
 * This function applies new NAT DMZ host configuration.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in] pDmlDmz - Pointer to DMZ configuration structure containing new settings to apply.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if DMZ configuration update is successful.
 * @retval ANSC_STATUS_FAILURE if update fails.
 */
ANSC_STATUS
CosaDmlNatSetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmlDmz
    );

/**
 * @brief Get LAN IP address for NAT operations.
 *
 * This function retrieves the current LAN IP address used for NAT operations.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if LAN IP retrieval is successful.
 * @retval ANSC_STATUS_FAILURE if retrieval fails.
 */
ANSC_STATUS
CosaDmlNatGetLanIP
    (
        ANSC_HANDLE                 hContext
    );

/**
 * @brief Get a specific NAT port mapping by instance number.
 *
 * This function retrieves the configuration of a single NAT port mapping entry.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in] InstanceNumber - Unique instance number identifying the port mapping entry to retrieve.
 * @param[out] pNatPMapping - Pointer to port mapping structure to receive the configuration data.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port mapping retrieval is successful.
 * @retval ANSC_STATUS_FAILURE if retrieval fails.
 */
ANSC_STATUS
CosaDmlNatGetPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                      InstanceNumber,
        PCOSA_DML_NAT_PMAPPING      pNatPMapping
    );

/**
 * @brief Retrieve the complete list of NAT port mappings.
 *
 * This function retrieves the complete list of all NAT port mapping entries configured in the system.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[out] pulCount - Pointer to receive the actual number of port mapping entries in the returned array.
 *
 * @return Pointer to an array of NAT port mapping structures, allocated by callee.
 * @retval Non-NULL pointer to COSA_DML_NAT_PMAPPING array if entries exist.
 * @retval NULL if no port mapping entries are found or allocation fails.
 */
PCOSA_DML_NAT_PMAPPING
CosaDmlNatGetPortMappings
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

/**
 * @brief Add a new NAT port mapping entry.
 *
 * This function adds a new NAT port mapping rule to the system.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in,out] pEntry - Pointer to port mapping structure containing the configuration to add.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port mapping addition is successful.
 * @retval ANSC_STATUS_FAILURE if addition fails.
 *
 */
ANSC_STATUS
CosaDmlNatAddPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry
    );


/**
 * @brief Delete a NAT port mapping entry.
 *
 * This function removes a NAT port mapping rule from the system.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in] ulInstanceNumber - Instance number of the port mapping entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port mapping deletion is successful.
 * @retval ANSC_STATUS_FAILURE if deletion fails.
 */
ANSC_STATUS
CosaDmlNatDelPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

#ifdef _BWG_PRODUCT_REQ_
/**
 * @brief Update NAT list for usable statics.
 *
 * This function updates the list for NAT port mappings.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if list update is successful.
 * @retval ANSC_STATUS_FAILURE if update fails.
 */
//CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: START
ANSC_STATUS UpdateList(ANSC_HANDLE hContext);
//CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: END
#endif

/**
 * @brief Update an existing NAT port mapping entry.
 *
 * This function re-configures an existing NAT port mapping rule.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in] pEntry - Pointer to port mapping structure containing new configuration.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port mapping update is successful.
 * @retval ANSC_STATUS_FAILURE if update fails.
 */
ANSC_STATUS
CosaDmlNatSetPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry          /* Identified by InstanceNumber */
    );


/* APIs for PortTrigger */

/**
 * @brief Retrieve the complete list of NAT port trigger rules.
 *
 * This function retrieves all configured NAT port triggering rules from the system.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[out] pulCount - Pointer to receive the actual number of port trigger entries in the returned array.
 * @param[in] bCommit - Commit flag indicating whether to persist changes.
 *                      \n TRUE: commit configuration changes.
 *                      \n FALSE: retrieve without committing.
 *
 * @return Pointer to an array of NAT port trigger structures, allocated by callee.
 * @retval Non-NULL pointer to COSA_DML_NAT_PTRIGGER array if entries exist.
 * @retval NULL if no port trigger entries are found or allocation fails.
 */
PCOSA_DML_NAT_PTRIGGER
CosaDmlNatGetPortTriggers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        BOOLEAN                     bCommit
    );

/**
 * @brief Add a new NAT port trigger entry.
 *
 * This function adds a new NAT port triggering rule to the system.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in] pEntry - Pointer to port trigger structure containing the configuration to add.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port trigger addition is successful.
 * @retval ANSC_STATUS_FAILURE if addition fails.
 */
ANSC_STATUS
CosaDmlNatAddPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    );

/**
 * @brief Get a specific NAT port trigger by instance number.
 *
 * This function retrieves the configuration of a single NAT port trigger entry.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in] InstanceNum - Unique instance number identifying the port trigger entry to retrieve.
 * @param[out] pNatPTrigger - Pointer to port trigger structure to receive the configuration data.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port trigger retrieval is successful.
 * @retval ANSC_STATUS_FAILURE if retrieval fails.
 */
ANSC_STATUS
CosaDmlNatGetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        ULONG                       InstanceNum,
        PCOSA_DML_NAT_PTRIGGER      pNatPTrigger
    );

/**
 * @brief Delete a NAT port trigger entry.
 *
 * This function removes a NAT port triggering rule from the system.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in] pEntry - Pointer to port trigger structure identifying the entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port trigger deletion is successful.
 * @retval ANSC_STATUS_FAILURE if deletion fails
 */
ANSC_STATUS
CosaDmlNatDelPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    );

/**
 * @brief Update an existing NAT port trigger entry.
 *
 * This function re-configures an existing NAT port triggering rule.
 *
 * @param[in] hContext - Backend context handle from CosaDmlNatInit.
 * @param[in] pEntry - Pointer to port trigger structure containing new configuration.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port trigger update is successful.
 * @retval ANSC_STATUS_FAILURE if update fails.
 */
ANSC_STATUS
CosaDmlNatSetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry          /* Identified by InstanceNumber */
    );

/**
 * @brief Get NAT port triggering global enable status.
 *
 * This function retrieves the global enable/disable status of the NAT port triggering feature.
 *
 * @param[out] pBool - Pointer to receive the port triggering enable status.
 *                     \n TRUE: port triggering feature is enabled.
 *                     \n FALSE: port triggering feature is disabled.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if status retrieval is successful.
 * @retval ANSC_STATUS_FAILURE if retrieval fails.
 */
ANSC_STATUS
CosaDmlNatGetPortTriggerEnable(BOOL *pBool);

/**
 * @brief Set NAT port triggering global enable status.
 *
 * This function sets the global enable/disable status of the NAT port triggering feature.
 *
 * @param[in] vBool - Port triggering enable status to set.
 *                    \n TRUE: enable port triggering feature.
 *                    \n FALSE: disable port triggering feature.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if status update is successful.
 * @retval ANSC_STATUS_FAILURE if update fails.
 */
ANSC_STATUS
CosaDmlNatSetPortTriggerEnable(BOOL vBool);

/**
 * @brief Validate IPv6 address for port mapping.
 *
 * This function validates an IPv6 address string for use as an internal client address
 * in a NAT port mapping rule.
 *
 * @param[in] address - IPv6 address string to validate.
 *
 * @return The status of the validation.
 * @retval TRUE if the IPv6 address is valid.
 * @retval FALSE if the IPv6 address is invalid.
 */
BOOL
CosaDmlNatChkPortMappingIPV6Address(char* address);

/**
 * @brief Validate internal client IP address for port mapping.
 *
 * This function validates an internal IPv4 client address for a NAT port mapping rule.
 *
 * @param[in] client - Internal IPv4 client address in network byte order (ULONG).
 *                     \n Special value 0xFFFFFFFF is allowed for IPv6-only port forwarding.
 *
 * @return The status of the validation.
 * @retval TRUE if the client address is valid and within LAN range.
 * @retval FALSE if the client address is invalid or unreachable.
 */
BOOL
CosaDmlNatChkPortMappingClient(ULONG client);

/**
 * @brief Check for port range conflicts in port mappings.
 *
 * This function validates that a port range for a port mapping rule does not overlap
 * with existing port mappings or port trigger rules.
 *
 * @param[in] InstanceNumber - Instance number of the entry being validated (0 for new entries).
 * @param[in] enabled - Enable status of the entry being validated.
 *                      FALSE to skip validation.
 * @param[in] start - Start port number of the range to validate.
 * @param[in] end - End port number of the range to validate.
 * @param[in] protocol - Protocol type: TCP, UDP
 * @param[in] is_trigger - Flag indicating if this is a port trigger validation.
 *
 * @return The status of the validation.
 * @retval TRUE if  port is not overlapping the port mappings
 * @retval FALSE if port overlaps port mappings
 * @retval ANSC_STATUS_FAILURE if Utopia context initialization fails.
 */
int
CosaDmlNatChkPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger);

/**
 * @brief Check for port range conflicts in port triggers.
 *
 * This function validates that a trigger port range for a port trigger rule does not overlap
 * with existing port trigger rules.
 *
 * @param[in] InstanceNumber - Instance number of the entry being validated (0 for new entries).
 * @param[in] enabled - Enable status of the entry being validated.
 *                      FALSE to skip validation.
 * @param[in] start - Start port number of the trigger range to validate (1-65535).
 * @param[in] end - End port number of the trigger range to validate (1-65535).
 * @param[in] protocol - Protocol type: 6=TCP, 17=UDP, other=both.
 * @param[in] is_trigger - Flag indicating if this is a port trigger validation (should be 1).
 *
 * @return The status of the validation.
 * @retval TRUE if no trigger port range overlap exists.
 * @retval FALSE if trigger port range overlaps with existing rules.
 * @retval ANSC_STATUS_FAILURE if Utopia context initialization fails.
 */
int
CosaDmlNatChkTriggerPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger);

/**
 * @brief Validate description string.
 *
 * This function validates a description string for port mapping or port trigger entries.
 * Ensures the description is not NULL, empty, or starting with a space character.
 *
 * @param[in] desp - Description string to validate.
 *
 * @return The status of the validation.
 * @retval TRUE if description is valid.
 * @retval FALSE if description is NULL, empty, or starts with space.
 */
int
CosaDmlChkDesp(char *desp);

/**
 * @brief Check enable flag for port mapping.
 *
 * This function checks the enable flag and related conditions for a port mapping entry
 * to determine if the entry should be activated.
 *
 * @param[in] pPortMapping - Pointer to port mapping structure to check.
 *
 * @return Enable flag check result.
 * @retval TRUE if the port mapping should be enabled.
 * @retval FALSE if the port mapping should not be enabled.
 *
 */
BOOL
CosaDmlNatChkEnableFlg(PCOSA_DML_NAT_PMAPPING pPortMapping);

/**
 * @brief Validate all parameters of a port forwarding entry.
 *
 * This function performs comprehensive validation of all parameters in a port forwarding entry including port ranges,
 * client addresses, protocols, and checks for conflicts with existing rules and reserved ports.
 *
 * @param[in] pPortMapping - Pointer to port mapping structure to validate.
 *
 * @return The status of the validation.
 * @retval TRUE if all parameters are valid.
 * @retval FALSE if validation fails.
 */
int
_Check_PF_parameter(PCOSA_DML_NAT_PMAPPING pPortMapping);

/**
 * @brief Validate all parameters of a port trigger entry.
 *
 * This function performs comprehensive validation of all parameters in a port triggering
 * entry including trigger port ranges, forward port ranges and protocols.
 *
 * @param[in] pPortTrigger - Pointer to port trigger structure to validate.
 *
 * @return The status of the validation.
 * @retval TRUE if all parameters are valid.
 * @retval FALSE if any parameter is invalid.
*/
int
_Check_PT_parameter(PCOSA_DML_NAT_PTRIGGER pPortTrigger);

/**
 * @brief Check if maximum port mapping rule limit is reached.
 *
 * This function checks whether adding a new port mapping entry would exceed the maximum
 * allowed number of port mapping rules configured in the system.
 *
 * @param[in] pEntry - Pointer to port mapping structure to check.
 *
 * @return Limit check result.
 * @retval TRUE if maximum rule limit is reached.
 * @retval FALSE if more rules can be added.
 */
BOOL
CosaDmlNatChkPortMappingMaxRuleNum(PCOSA_DML_NAT_PMAPPING pEntry);

/**
 * @brief Delete all dynamic port mapping entries.
 *
 * This function removes all dynamically created port mapping entries from the system.
 *
 * @return None.
 */
void CosaDmlNatDelDynPortMappings (void);

/**
 * @brief Check if port range overlaps with management access ports.
 *
 * This function validates that a port forwarding rule's external port range does not overlap
 * with ports reserved for remote management access (HTTP/HTTPS).
 *
 * @param[in] PortStart - Start of external port range to check.
 * @param[in] PortEndRange - End of external port range to check.
 *
 * @return Overlap check result.
 * @retval TRUE if the port range overlaps with a management port.
 * @retval FALSE if management access is not enabled OR there is no port overlap.
 */
int IsPortOverlapWithManagementAccess(int PortStart, int PortEndRange);

#if defined (SPEED_BOOST_SUPPORTED)
/**
 * @brief Check if port range overlaps with Speed Boost reserved ports.
 *
 * This function validates that a port forwarding rule's port range does not overlap with
 * ports reserved for the Speed Boost feature.
 * Only available when SPEED_BOOST_SUPPORTED is defined.

 * @param[in] ExternalPort - External port number or start of external port range.
 * @param[in] ExternalPortEndRange - End of external port range.
 * @param[in] InternalPort - Internal port number or start of internal port range.
 * @param[in] InternalPortend - End of internal port range.
 *
 * @return Overlap check result.
 * @retval TRUE if port range overlaps with Speed Boost reserved ports.
 * @retval FALSE if no overlap with Speed Boost port ranges.
 */
int
IsPortOverlapWithSpeedboostPortRange(int ExternalPort, int ExternalPortEndRange, int InternalPort , int InternalPortend);
#endif

#if defined(FEATURE_MAPT) || defined(FEATURE_SUPPORT_MAPT_NAT46)
/**
 * @brief Get count of active IPv4 UDP internal ports for MAPT.
 *
 * This function retrieves the number of active IPv4 UDP internal ports currently in use for NAT operations.
 *
 * @param[out] nports - Pointer to receive the count of active IPv4 UDP internal ports.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port count retrieval is successful.
 * @retval ANSC_STATUS_FAILURE if retrieval fails.
 */
//if defined (_XB6_PRODUCT_REQ_) || defined (_XB7_PRODUCT_REQ_)
#define SYSEVENT_MAPT_TOTAL_PORTS "mapt_total_ports"
#define SYSEVENT_MAPT_CONFIG_FLAG "mapt_config_flag"

ANSC_STATUS
CosaDmlNatGetActiveIPv4UdpInternalPorts
    (
        int*                       nports
    );

/**
 * @brief Get count of active IPv4 TCP internal ports for MAPT.
 *
 * This function retrieves the number of active IPv4 TCP internal ports currently in use for NAT operations.
 *
 * @param[out] nports - Pointer to receive the count of active IPv4 TCP internal ports.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if port count retrieval is successful.
 * @retval ANSC_STATUS_FAILURE if retrieval fails.
 */
ANSC_STATUS
CosaDmlNatGetActiveIPv4TcpInternalPorts
    (
        int*                       nports
    );

/**
 * @brief Get total port usage percentage for MAPT.
 *
 * This function calculates and retrieves the percentage of ports currently in use for the
 * specified protocol when MAPT (MAP-T) is active.
 *
 * @param[in] protocol - Protocol type string to query port usage.
 * @param[out] pValue - Pointer to buffer to receive the port usage percentage string.
 * @param[in,out] pUlSize - Pointer to buffer size.
 *                          \n [in] Size of pValue buffer.
 *                          \n [out] Actual size of returned string.
 *
 * @return The status of the operation.
 * @retval 0 if port usage percentage retrieval is successful.
 * @retval -1 error code if retrieval fails.
 */
int GetTotalPortsUsagePerc(char *protocol, char *pValue, ULONG *pUlSize);
/**
 * @brief Count unique active ports for the specified protocol.
 *
 * This function counts the number of unique active ports currently in use for the specified
 * protocol in MAPT deployments.
 *
 * @param[in] proto - Protocol type string to count ports.
 *
 * @return Number of unique active ports.
 * @retval Non-zero Count of unique active ports for the specified protocol.
 * @retval 0 if no active ports found or on error.
 */
int count_unique_ports(const char *proto, bool neededNATSessionCount);

#endif
#endif