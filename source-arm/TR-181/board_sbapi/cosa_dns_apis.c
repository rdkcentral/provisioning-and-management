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

/************************************************************************* 
    module: cosa_dns_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

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

#include "cosa_dns_internal.h"
#include "safec_lib_common.h"
#include "secure_wrapper.h"
#include <errno.h>
#include <stdio.h>

#define SYSCFG_DNS_FORWARD_MAX "dnsmasq_forward_max"
#define DEFAULT_DNS_FORWARD_MAX 150
#define DNSMASQ_CONF_FILE "/var/dnsmasq.conf"

/* Pending value for DNS forward max (for transaction support) */
static ULONG g_PendingDnsForwardMax = 0;
static BOOL g_PendingDnsForwardMaxSet = FALSE;

#if (defined(_COSA_SIM_))

COSA_DML_DNS_CLIENT_SERVER  g_DnsServerFull[10] =
{
    {1, "Server1", TRUE, COSA_DML_DNS_STATUS_Enabled,
        "\x40\x40\x40\x08", "wan8", COSA_DML_DNS_ADDR_SRC_DHCP}
};

COSA_DML_DNS_RELAY_ENTRY  g_DnsRelayFull[10] =
{
    {1, "Forwarding1", TRUE, COSA_DML_DNS_STATUS_Enabled, 
        "\x40\x40\x40\x08", "wan8", COSA_DML_DNS_ADDR_SRC_DHCP}
};

ULONG g_NumOfDnsServers = 1;

ULONG g_NumOfDnsRelays = 1;

BOOLEAN g_ClientEnable = TRUE; 

BOOLEAN g_ReplyEnable = TRUE;

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            )


    description:

       This function enables the DNS client.

    argument:  
            ANSC_HANDLE                  hDml,

            PANSC_HANDLE                phContext

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDmlDnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{

    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    return ANSC_STATUS_SUCCESS;
}

/*
 *  DNS Client
 */
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsEnableClient
            (
                ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled
            )

    description:

       This function enables the DNS client.

    argument:   ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsEnableClient
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    g_ClientEnable = bEnabled;
    
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlIpDnsGetClientStatus
            (
                ANSC_HANDLE                 hContext
            );

    description:

        This function gets the status of the DNS client.

    argument:   ANSC_HANDLE                 hContext
            

    return:     operation status.

**********************************************************************/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetClientStatus
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    COSA_DML_DNS_STATUS             DnsStatus = COSA_DML_DNS_STATUS_Disabled;

    if ( g_ClientEnable )
    {
          DnsStatus = COSA_DML_DNS_STATUS_Enabled;
    }
    
    return DnsStatus;
}
/*
 *  DNS Client Server
 */
/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_DNS_CLIENT_SERVER
        CosaDmlDnsClientGetServers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This routine is to retrieve the complete list of DNS servers, which is a dynamic table.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_DNS_CLIENT_SERVER
CosaDmlDnsClientGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    UNREFERENCED_PARAMETER(hContext);
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;
    ULONG                           ulSize     = 0;

    ulSize = sizeof(COSA_DML_DNS_CLIENT_SERVER) * g_NumOfDnsServers;
    
    pDnsServer = AnscAllocateMemory(ulSize);

    if ( !pDnsServer )
    {
        *pulCount = 0;
    } else
    {
        *pulCount = g_NumOfDnsServers;
        AnscCopyMemory(pDnsServer, g_DnsServerFull, ulSize);
    }
    
    return pDnsServer;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientSetServerValues
            (
                ANSC_HANDLE             hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                        pAlias
            )


    description:

        The API sets DNS server values. 

    argument:   
            ANSC_HANDLE             hContext,

            ULONG                       ulIndex,

            ULONG                       ulInstanceNumber,

            char*                        pAlias
    
    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientSetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    errno_t                         rc           = -1;

    g_DnsServerFull[ulIndex].InstanceNumber = ulInstanceNumber;
    rc = strcpy_s(g_DnsServerFull[ulIndex].Alias,sizeof(g_DnsServerFull[ulIndex].Alias), pAlias);
    ERR_CHK(rc);        
    return returnStatus;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientAddServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
            );

    description:

        The API adds one DNS server entry into DNS client. 

    argument:   ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
                Caller does not need to fill in Status or Alias fields. Field Type has to be COSA_DML_DNS_ADDR_SRC_Static. Caller does not have to fill in field Interface. Upon return, callee fills in the generated Alias.
    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if ( g_NumOfDnsServers >= 10 )
    {
        return ANSC_STATUS_FAILURE;
    }

    g_DnsServerFull[g_NumOfDnsServers].InstanceNumber  = pEntry->InstanceNumber;  
    g_DnsServerFull[g_NumOfDnsServers].bEnabled        = pEntry->bEnabled;        
    g_DnsServerFull[g_NumOfDnsServers].Status          = pEntry->Status;        
    g_DnsServerFull[g_NumOfDnsServers].Type            = pEntry->Type;      
    g_DnsServerFull[g_NumOfDnsServers].DNSServer.Value = pEntry->DNSServer.Value;             
    errno_t rc = -1;
    rc = strcpy_s(g_DnsServerFull[g_NumOfDnsServers].Alias,sizeof(g_DnsServerFull[g_NumOfDnsServers].Alias), pEntry->Alias);
    ERR_CHK(rc);
    rc = strcpy_s(g_DnsServerFull[g_NumOfDnsServers].Interface,sizeof(g_DnsServerFull[g_NumOfDnsServers].Interface), pEntry->Interface);
    ERR_CHK(rc);
    g_NumOfDnsServers++;

    return ANSC_STATUS_SUCCESS;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientDelServer
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    description:

        The API delete one DNS server entry from DNS client.

    argument:   ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG                           i = 0;
    ULONG                           j = 0;

     for ( i = 0; i < g_NumOfDnsServers; i++ )
    {
        if ( ulInstanceNumber == g_DnsServerFull[i].InstanceNumber )
        {
            for ( j = i; j < g_NumOfDnsServers; j++ )
            {
                AnscCopyMemory
                (
                    &g_DnsServerFull[j], 
                    &g_DnsServerFull[j+1],
                    sizeof(COSA_DML_DNS_CLIENT_SERVER)
                );
            }

            g_NumOfDnsServers--;

            return ANSC_STATUS_SUCCESS;
        }
    }
             
    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientSetServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
            );

    description:

        This function sets the server of the DNS client.

    argument:   ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientSetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG                           i = 0;
    errno_t                         rc = -1;
    for ( i = 0; i < g_NumOfDnsServers; i++)
    {
        if ( g_DnsServerFull[i].InstanceNumber == pEntry->InstanceNumber )
        {
            g_DnsServerFull[i].InstanceNumber  = pEntry->InstanceNumber;  
            g_DnsServerFull[i].bEnabled        = pEntry->bEnabled;        
            g_DnsServerFull[i].Status          = pEntry->Status;        
            g_DnsServerFull[i].Type            = pEntry->Type;      
            g_DnsServerFull[i].DNSServer.Value = pEntry->DNSServer.Value;             
            
            rc = strcpy_s(g_DnsServerFull[i].Alias,sizeof(g_DnsServerFull[i].Alias), pEntry->Alias);
            ERR_CHK(rc);
            rc = strcpy_s(g_DnsServerFull[i].Interface,sizeof(g_DnsServerFull[i].Interface), pEntry->Interface);
            ERR_CHK(rc);
 
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientGetServer
            (
                ANSC_HANDLE                                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER      pEntry
             )


    description:

        This routine is to retrieve the config of DNS servers, which is a dynamic table.

    argument:  
                    ANSC_HANDLE                 hContext,

                    PCOSA_DML_DNS_CLIENT_SERVER pEntry

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlDnsClientGetServer
    (
        ANSC_HANDLE hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
     )

{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    

    return returnStatus;
}



/*
 *  DNS Relay
 */
 
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsEnableRelay
            (
                ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled
            )

    description:

        This function enables the DNS relay.

    argument:   ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsEnableRelay
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
    g_ReplyEnable = bEnabled;

    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        COSA_DML_DNS_STATUS
        CosaDmlIpDnsGetRelayStatus
            (
                ANSC_HANDLE                 hContext
            )

    description:

        This function initiate  cosa nat object and return handle.

    argument:   ANSC_HANDLE                 hContext
          

    return:     operation status.

**********************************************************************/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetRelayStatus
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    COSA_DML_DNS_STATUS             RelayStatus = COSA_DML_DNS_STATUS_Disabled;

    if ( g_ReplyEnable )
    {
        RelayStatus = COSA_DML_DNS_STATUS_Enabled;
    }
    
    return RelayStatus;
}

/*
 *  DNS Relay Forwarding (server)
 */
/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_DNS_RELAY_ENTRY
        CosaDmlDnsRelayGetServers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This function Get the DNS relay server.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_DNS_RELAY_ENTRY
CosaDmlDnsRelayGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    UNREFERENCED_PARAMETER(hContext);
    PCOSA_DML_DNS_RELAY_ENTRY       pForward = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    ULONG                           ulSize     = 0;

    ulSize = sizeof(COSA_DML_DNS_RELAY_ENTRY) * g_NumOfDnsRelays;

    pForward = AnscAllocateMemory(ulSize);

    if ( !pForward )
    {
        *pulCount = 0;
    } else
    {
        *pulCount = g_NumOfDnsRelays;
        AnscCopyMemory(pForward, g_DnsRelayFull, ulSize);
    }
    
    return pForward;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelaySetServerValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIndex,
                ULONG                           ulInstanceNumber,
                char*                            pAlias
            )


    description:

        This function Get the DNS relay server.

    argument:  
                 ANSC_HANDLE                 hContext,

                 ULONG                           ulIndex,

                 ULONG                           ulInstanceNumber,

                 char*                            pAlias

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelaySetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    errno_t                         rc           = -1;
    g_DnsRelayFull[ulIndex].InstanceNumber = ulInstanceNumber;
    rc = strcpy_s(g_DnsRelayFull[ulIndex].Alias,sizeof(g_DnsRelayFull[ulIndex].Alias), pAlias);
    ERR_CHK(rc);
        
    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayAddServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
            );

    description:

        The API adds one DNS server entry into DNS relay. 

    argument:   ANSC_HANDLE                 hContext,
    
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
                Caller does not need to fill in Status or Alias fields. Field Type has to be COSA_DML_DNS_ADDR_SRC_Static. Caller does not have to fill in field Interface. Upon return, callee fills in the generated Alias.

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelayAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if ( g_NumOfDnsRelays >= 10 )
    {
        return ANSC_STATUS_FAILURE;
    }

    g_DnsRelayFull[g_NumOfDnsRelays].InstanceNumber  = pEntry->InstanceNumber;  
    g_DnsRelayFull[g_NumOfDnsRelays].bEnabled        = pEntry->bEnabled;        
    g_DnsRelayFull[g_NumOfDnsRelays].Status          = pEntry->Status;        
    g_DnsRelayFull[g_NumOfDnsRelays].Type            = pEntry->Type;      
    g_DnsRelayFull[g_NumOfDnsRelays].DNSServer.Value = pEntry->DNSServer.Value;             
    errno_t                                          = -1;
    rc = strcpy_s(g_DnsRelayFull[g_NumOfDnsRelays].Alias,sizeof(g_DnsRelayFull[g_NumOfDnsRelays].Alias), pEntry->Alias);
    ERR_CHK(rc);
    rc = strcpy_s(g_DnsRelayFull[g_NumOfDnsRelays].Interface,sizeof(g_DnsRelayFull[g_NumOfDnsRelays].Interface), pEntry->Interface);
    ERR_CHK(rc);

    g_NumOfDnsRelays++;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayDelServer
        (
            ANSC_HANDLE                 hContext,
            ULONG                       ulInstanceNumber
        );

    description:

        The API delete one DNS server entry from DNS client. 

    argument:   ANSC_HANDLE                 hContext,
    
                ULONG                       ulInstanceNumber

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelayDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_NumOfDnsRelays; i++ )
    {
        if ( ulInstanceNumber == g_DnsRelayFull[i].InstanceNumber )
        {
            for ( j = i; j < g_NumOfDnsRelays; j++ )
            {
                AnscCopyMemory
                (
                    &g_DnsRelayFull[j], 
                    &g_DnsRelayFull[j+1],
                    sizeof(COSA_DML_DNS_RELAY_ENTRY)
                );
            }

            g_NumOfDnsRelays--;

            return ANSC_STATUS_SUCCESS;
        }
    }
             
    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelaySetServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
            );

    description:

        This function set the relay server.

    argument:   ANSC_HANDLE                 hContext,
    
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelaySetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG                           i = 0;
    errno_t                         rc = -1;

    for ( i = 0; i < g_NumOfDnsRelays; i++)
    {
        if ( g_DnsRelayFull[i].InstanceNumber == pEntry->InstanceNumber )
        {
            g_DnsRelayFull[i].InstanceNumber  = pEntry->InstanceNumber;  
            g_DnsRelayFull[i].bEnabled        = pEntry->bEnabled;        
            g_DnsRelayFull[i].Status          = pEntry->Status;        
            g_DnsRelayFull[i].Type            = pEntry->Type;      
            g_DnsRelayFull[i].DNSServer.Value = pEntry->DNSServer.Value;             
            
            rc = strcpy_s(g_DnsRelayFull[i].Alias,sizeof(g_DnsRelayFull[i].Alias), pEntry->Alias);
            ERR_CHK(rc);
            rc = strcpy_s(g_DnsRelayFull[i].Interface,sizeof(g_DnsRelayFull[i].Interface), pEntry->Interface);
            ERR_CHK(rc);
 
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayGetServer
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry     
            )



    description:

        This routine is to retrieve the config of DNS servers, which is a dynamic table.

    argument:  
                    ANSC_HANDLE                          hContext,

                    PCOSA_DML_DNS_RELAY_ENTRY  pEntry

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlDnsRelayGetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry     
    )


{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
    return returnStatus;
}

#elif defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_ARM_) || defined(_COSA_BCM_MIPS_)

#include <utctx/utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <ulog/ulog.h>
#include <syscfg/syscfg.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "cosa_x_cisco_com_devicecontrol_apis.h"


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            )


    description:

       This function enables the DNS client.

    argument:  
            ANSC_HANDLE                  hDml,

            PANSC_HANDLE                phContext

    return:     operation status.

**********************************************************************/


ANSC_STATUS
CosaDmlDnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    return ANSC_STATUS_SUCCESS;
}

/*
 *  DNS Client
 */
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsEnableClient
            (
                ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled
            )

    description:

       This function enables the DNS client.

    argument:   ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsEnableClient
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    /* USGv2 not support Disable DNS Client */
    UNREFERENCED_PARAMETER(hContext);
    if( TRUE == bEnabled ){
        return ANSC_STATUS_SUCCESS;
    }else{
        return ANSC_STATUS_FAILURE;
    }
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlIpDnsGetClientStatus
            (
                ANSC_HANDLE                 hContext
            );

    description:

        This function gets the status of the DNS client.

    argument:   ANSC_HANDLE                 hContext
            

    return:     operation status.

**********************************************************************/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetClientStatus
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return COSA_DML_DNS_STATUS_Enabled;    
}

/*
 *  DNS Client Server
 */
typedef enum dns_family{
    DNS_FAMILY_NONE = 0,
    DNS_FAMILY_IPV4,
    DNS_FAMILY_IPV6
}dns_family_t;

static int CHECK_V4_V6(char *str){
    if( NULL != strchr(str, ':'))
        return DNS_FAMILY_IPV6;
    if( NULL != strchr(str, '.'))
        return DNS_FAMILY_IPV4;

    return DNS_FAMILY_NONE;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_DNS_CLIENT_SERVER
        CosaDmlDnsClientGetServers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This routine is to retrieve the complete list of DNS servers, which is a dynamic table.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_DNS_CLIENT_SERVER
CosaDmlDnsClientGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    DNS_Client_t dns;
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext ctx;
    int i;
    ULONG j;    
    PCOSA_DML_DNS_CLIENT_SERVER pServer = NULL;
    *pulCount = 0;
    errno_t rc = -1;
    if (Utopia_Init(&ctx)){
        if(SUCCESS == Utopia_GetDNSServer(&ctx, &dns)){
            for(i = DNS_CLIENT_NAMESERVER_CNT -1; i >= 0 && (dns.dns_server[i][0] == 0) ; i--);
            *pulCount = i + 1;
            if(*pulCount > 0)
                 pServer = (PCOSA_DML_DNS_CLIENT_SERVER)AnscAllocateMemory(*pulCount * sizeof(COSA_DML_DNS_CLIENT_SERVER));
            if(pServer == NULL){
                *pulCount = 0;
            }else{
                for(i = 0, j = 0; j < *pulCount; j++ ){
                    int af = CHECK_V4_V6(dns.dns_server[j]);
                    //CcspTraceWarning(("CosaDmlDnsClientGetServers -- af:%d,i:%d,j:%lu, server:%s \n", af, i, j, dns.dns_server[j]));
                    switch (af){
                    case DNS_FAMILY_IPV4:
                            /*inet_pton(AF_INET, dns.dns_server[i], &(pServer[i].DNSServer));*/
                            rc = strcpy_s(pServer[i].DNSServer,sizeof(pServer[i].DNSServer), dns.dns_server[j]);
                            ERR_CHK(rc);
                            pServer[i].Order          = 1 + i;
                            pServer[i].InstanceNumber = 1 + i;
                            pServer[i].bEnabled       = TRUE;
                            pServer[i].Type           = COSA_DML_DNS_ADDR_SRC_DHCPV4;
                            i++;
                            break;
                    case DNS_FAMILY_IPV6:
                            rc = strcpy_s(pServer[i].DNSServer,sizeof(pServer[i].DNSServer), dns.dns_server[j]);
                            ERR_CHK(rc);
                            pServer[i].Order          = 1 + i;
                            pServer[i].InstanceNumber = 1 + i;
                            pServer[i].bEnabled       = TRUE;
                            pServer[i].Type           = COSA_DML_DNS_ADDR_SRC_DHCPV6;
                            i++;
                            break;
                    default:
                            AnscFreeMemory(pServer);
                            pServer = NULL;
                            *pulCount = 0;
                            break;

                    }
                }
                *pulCount = i;
            }
        }
        Utopia_Free(&ctx, 0);
    }
    return pServer;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientSetServerValues
            (
                ANSC_HANDLE             hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                        pAlias
            )


    description:

        The API sets DNS server values. 

    argument:   
            ANSC_HANDLE             hContext,

            ULONG                       ulIndex,

            ULONG                       ulInstanceNumber,

            char*                        pAlias
    
    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientSetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pAlias);
    char inst_str[10];
    char inst_num[32];
    errno_t safec_rc = -1;
    UtopiaContext ctx;
   
    if (Utopia_Init(&ctx))
    {

        safec_rc = sprintf_s(inst_num, sizeof(inst_num), "dns_client_server_instance_%lu", ulIndex);
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
        }
        safec_rc = sprintf_s(inst_str, sizeof(inst_str), "%lu", ulInstanceNumber);
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
        }
        Utopia_RawSet(&ctx, NULL, inst_num, inst_str);
        Utopia_Free(&ctx, 1); 
    }
       
    return SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientAddServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
            );

    description:

        The API adds one DNS server entry into DNS client. 

    argument:   ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
                Caller does not need to fill in Status or Alias fields. Field Type has to be COSA_DML_DNS_ADDR_SRC_Static. Caller does not have to fill in field Interface. Upon return, callee fills in the generated Alias.
    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDmlDnsClientAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientDelServer
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    description:

        The API delete one DNS server entry from DNS client.

    argument:   ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    return ANSC_STATUS_FAILURE;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientSetServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
            );

    description:

        This function sets the server of the DNS client.

    argument:   ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientSetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    ULONG ipAddr;

    inet_pton(AF_INET, pEntry->DNSServer, &ipAddr);
	
    return CosaDmlDcSetWanNameServer(NULL, ipAddr, pEntry->InstanceNumber);
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientGetServer
            (
                ANSC_HANDLE                                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER      pEntry
             )


    description:

        This routine is to retrieve the config of DNS servers, which is a dynamic table.

    argument:  
                    ANSC_HANDLE                 hContext,

                    PCOSA_DML_DNS_CLIENT_SERVER pEntry

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlDnsClientGetServer
    (
        ANSC_HANDLE hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
     )

{
    UNREFERENCED_PARAMETER(hContext);
    ULONG count;
    PCOSA_DML_DNS_CLIENT_SERVER pTable;
    pTable = CosaDmlDnsClientGetServers(hContext, &count);
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    ULONG i;
    
    if(pTable == NULL)
        return ret;
    for(i = 0; i < count ;i++){
        if( pTable[i].InstanceNumber == pEntry->InstanceNumber)
            memcpy(pEntry, &pTable[i], sizeof(COSA_DML_DNS_CLIENT_SERVER));
        ret = ANSC_STATUS_SUCCESS;
    }

    AnscFreeMemory(pTable);/*RDKB-6837, CID-33471, free unused resource before exit*/
    return ret;
}

/*
 *  DNS Relay
 */
 
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsEnableRelay
            (
                ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled
            )

    description:

        This function enables the DNS relay.

    argument:   ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsEnableRelay
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(bEnabled);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     self

    prototype:

        COSA_DML_DNS_STATUS
        CosaDmlIpDnsGetRelayStatus
            (
                ANSC_HANDLE                 hContext
            )

    description:

        This function initiate  cosa nat object and return handle.

    argument:   ANSC_HANDLE                 hContext
          

    return:     operation status.

**********************************************************************/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetRelayStatus
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return ANSC_STATUS_FAILURE;
}

/*
 *  DNS Relay Forwarding (server)
 */
/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_DNS_RELAY_ENTRY
        CosaDmlDnsRelayGetServers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This function Get the DNS relay server.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_DNS_RELAY_ENTRY
CosaDmlDnsRelayGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pulCount);
    return NULL;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelaySetServerValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIndex,
                ULONG                           ulInstanceNumber,
                char*                            pAlias
            )


    description:

        This function Get the DNS relay server.

    argument:  
                 ANSC_HANDLE                 hContext,

                 ULONG                           ulIndex,

                 ULONG                           ulInstanceNumber,

                 char*                            pAlias

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelaySetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    UNREFERENCED_PARAMETER(pAlias);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayAddServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
            );

    description:

        The API adds one DNS server entry into DNS relay. 

    argument:   ANSC_HANDLE                 hContext,
    
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
                Caller does not need to fill in Status or Alias fields. Field Type has to be COSA_DML_DNS_ADDR_SRC_Static. Caller does not have to fill in field Interface. Upon return, callee fills in the generated Alias.

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelayAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayDelServer
        (
            ANSC_HANDLE                 hContext,
            ULONG                       ulInstanceNumber
        );

    description:

        The API delete one DNS server entry from DNS client. 

    argument:   ANSC_HANDLE                 hContext,
    
                ULONG                       ulInstanceNumber

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelayDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    return ANSC_STATUS_FAILURE;
}
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelaySetServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
            );

    description:

        This function set the relay server.

    argument:   ANSC_HANDLE                 hContext,
    
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelaySetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayGetServer
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry     
            )



    description:

        This routine is to retrieve the config of DNS servers, which is a dynamic table.

    argument:  
                    ANSC_HANDLE                          hContext,

                    PCOSA_DML_DNS_RELAY_ENTRY  pEntry

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlDnsRelayGetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry     
    )


{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_FAILURE;

}

/***********************************************************************

    caller:     self

    prototype:

        ULONG
        CosaDmlDnsGetForwardMax
            (
                void
            );

    description:

        This routine retrieves the DNS forward max value from syscfg.

    return:     DNS forward max value (default: 150)

**********************************************************************/

ULONG CosaDmlDnsGetForwardMax(void)
{
    char buf[16] = {0};
    ULONG value = DEFAULT_DNS_FORWARD_MAX;
    
    printf("[DNS-GET] CosaDmlDnsGetForwardMax CALLED\n");
    fflush(stdout);
    
    /* Try to read from syscfg */
    if (syscfg_get(NULL, SYSCFG_DNS_FORWARD_MAX, buf, sizeof(buf)) == 0)
    {
        value = (ULONG)atoi(buf);
        printf("[DNS-GET] Read from syscfg: buf='%s', value=%lu\n", buf, value);
        fflush(stdout);
        
        if (value < 1 || value > 600)
        {
            printf("[DNS-GET] Invalid value %lu, using default %d\n", value, DEFAULT_DNS_FORWARD_MAX);
            fflush(stdout);
            CcspTraceWarning(("%s: Invalid stored value %lu, using default %d\n", 
                            __FUNCTION__, value, DEFAULT_DNS_FORWARD_MAX));
            value = DEFAULT_DNS_FORWARD_MAX;
        }
    }
    else
    {
        printf("[DNS-GET] syscfg_get FAILED, using default %d\n", DEFAULT_DNS_FORWARD_MAX);
        fflush(stdout);
        CcspTraceInfo(("%s: No stored value, using default %d\n", 
                      __FUNCTION__, DEFAULT_DNS_FORWARD_MAX));
    }
    
    printf("[DNS-GET] Returning value=%lu\n", value);
    fflush(stdout);
    
    return value;
}

/***********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsSetForwardMax
            (
                ULONG value
            );

    description:

        This routine sets the DNS forward max value, stores in syscfg,
        updates dnsmasq.conf, and triggers a dnsmasq restart.

    argument:   ULONG value - The DNS forward max value (1-65535)

    return:     operation status

**********************************************************************/

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDnsSetForwardMax
            (
                ULONG                   value
            );

    description:

        This function validates and stages the DNS forward max value.
        The actual update happens in CosaDmlDnsCommitForwardMax().

    argument:   ULONG                   value
                New value (1-600).

    return:     ANSC_STATUS_SUCCESS or ANSC_STATUS_FAILURE

**********************************************************************/
ANSC_STATUS CosaDmlDnsSetForwardMax(ULONG value)
{
    printf("[DNS-SET] CosaDmlDnsSetForwardMax CALLED with value=%lu\n", value);
    fflush(stdout);
    
    CcspTraceInfo(("CosaDmlDnsSetForwardMax: Staging value=%lu\n", value));
    
    if (value < 1 || value > 600)
    {
        printf("[DNS-SET] Validation FAILED - out of range\n");
        fflush(stdout);
        CcspTraceError(("CosaDmlDnsSetForwardMax: Invalid value %lu (must be 1-600)\n", value));
        return ANSC_STATUS_FAILURE;
    }
    
    /* Stage the value for commit */
    g_PendingDnsForwardMax = value;
    g_PendingDnsForwardMaxSet = TRUE;
    
    printf("[DNS-SET] Value staged successfully, pending=%lu\n", g_PendingDnsForwardMax);
    fflush(stdout);
    
    CcspTraceInfo(("CosaDmlDnsSetForwardMax: Validation passed, staged value=%lu\n", value));
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDnsCommitForwardMax
            (
                void
            );

    description:

        This function commits the pending DNS forward max value by:
        1. Updating /var/dnsmasq.conf
        2. Restarting dnsmasq
        3. Persisting to syscfg
        4. Clearing pending state

    argument:   None

    return:     ANSC_STATUS_SUCCESS or ANSC_STATUS_FAILURE

**********************************************************************/
ANSC_STATUS CosaDmlDnsCommitForwardMax(void)
{
    char buf[16] = {0};
    FILE *fp = NULL;
    char line[512] = {0};
    char tempFile[128];
    int found = 0;
    errno_t rc = -1;
    int ret = 0;
    ULONG value;
    
    printf("[DNS-COMMIT] ===== CosaDmlDnsCommitForwardMax ENTRY =====\n");
    fflush(stdout);
    
    if (!g_PendingDnsForwardMaxSet)
    {
        printf("[DNS-COMMIT] No pending value to commit (g_PendingDnsForwardMaxSet=%d)\n", g_PendingDnsForwardMaxSet);
        fflush(stdout);
        CcspTraceWarning(("CosaDmlDnsCommitForwardMax: No pending value to commit\n"));
        return ANSC_STATUS_SUCCESS;
    }
    
    value = g_PendingDnsForwardMax;
    printf("[DNS-COMMIT] Committing pending value=%lu\n", value);
    fflush(stdout);
    
    /* Use PID-based unique temp file in /tmp (writable for all processes) */
    rc = sprintf_s(tempFile, sizeof(tempFile), "/tmp/dnsmasq.conf.tmp.%d", getpid());
    if (rc < EOK)
    {
        printf("[DNS-COMMIT] sprintf_s FAILED for tempFile\n");
        fflush(stdout);
        CcspTraceError(("CosaDmlDnsCommitForwardMax: sprintf_s for tempFile failed\n"));
        return ANSC_STATUS_FAILURE;
    }
    
    printf("[DNS-COMMIT] STEP 1: Update config file '%s' using temp '%s'\n", DNSMASQ_CONF_FILE, tempFile);
    fflush(stdout);
    
    CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Committing value=%lu, temp file='%s'\n", value, tempFile));
    
    /* STEP 1: Update /var/dnsmasq.conf */
    CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Opening config file '%s'\n", DNSMASQ_CONF_FILE));
    
    fp = fopen(DNSMASQ_CONF_FILE, "r");
    if (!fp)
    {
        CcspTraceError(("CosaDmlDnsCommitForwardMax: Failed to open '%s' for reading (errno=%d)\n", 
               DNSMASQ_CONF_FILE, errno));
        return ANSC_STATUS_FAILURE;
    }
    
    FILE *fp_tmp = fopen(tempFile, "w");
    if (!fp_tmp)
    {
        CcspTraceError(("CosaDmlDnsCommitForwardMax: Failed to open '%s' for writing (errno=%d)\n", 
               tempFile, errno));
        fclose(fp);
        return ANSC_STATUS_FAILURE;
    }
    
    /* Read existing config and update dns-forward-max if it exists */
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strncmp(line, "dns-forward-max=", 16) == 0)
        {
            CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Found existing dns-forward-max line, replacing\n"));
            fprintf(fp_tmp, "dns-forward-max=%lu\n", value);
            found = 1;
        }
        else
        {
            fputs(line, fp_tmp);
        }
    }
    
    /* If dns-forward-max wasn't in the file, add it */
    if (!found)
    {
        CcspTraceInfo(("CosaDmlDnsCommitForwardMax: dns-forward-max NOT found, adding new line\n"));
        fprintf(fp_tmp, "dns-forward-max=%lu\n", value);
    }
    
    fclose(fp);
    fclose(fp_tmp);
    
    CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Renaming '%s' to '%s'\n", tempFile, DNSMASQ_CONF_FILE));
    
    /* Replace the original file - try rename first */
    if (rename(tempFile, DNSMASQ_CONF_FILE) != 0)
    {
        if (errno == EXDEV)
        {
            /* Cross-device link error - /tmp and /var on different filesystems
             * Fallback: copy temp file to target, then delete temp */
            CcspTraceWarning(("CosaDmlDnsCommitForwardMax: rename failed with EXDEV, using copy fallback\n"));
            
            FILE *fp_src = fopen(tempFile, "r");
            FILE *fp_dst = fopen(DNSMASQ_CONF_FILE, "w");
            
            if (fp_src && fp_dst)
            {
                char copyBuf[4096];
                size_t n;
                
                while ((n = fread(copyBuf, 1, sizeof(copyBuf), fp_src)) > 0)
                {
                    if (fwrite(copyBuf, 1, n, fp_dst) != n)
                    {
                        CcspTraceError(("CosaDmlDnsCommitForwardMax: fwrite failed during copy\n"));
                        fclose(fp_src);
                        fclose(fp_dst);
                        unlink(tempFile);
                        return ANSC_STATUS_FAILURE;
                    }
                }
                
                fclose(fp_src);
                fclose(fp_dst);
                unlink(tempFile);  /* Remove temp file after successful copy */
                
                CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Successfully copied temp file to target\n"));
            }
            else
            {
                CcspTraceError(("CosaDmlDnsCommitForwardMax: Failed to open files for copy (src=%p, dst=%p)\n",
                               (void*)fp_src, (void*)fp_dst));
                if (fp_src) fclose(fp_src);
                if (fp_dst) fclose(fp_dst);
                unlink(tempFile);
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            CcspTraceError(("CosaDmlDnsCommitForwardMax: rename() failed (errno=%d)\n", errno));
            unlink(tempFile);
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Successfully renamed temp file\n"));
    }
    
    CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Updated dnsmasq.conf with dns-forward-max=%lu\n", value));
    
    /* STEP 2: Restart dnsmasq to apply the change */
    printf("[DNS-COMMIT] STEP 2: Triggering dnsmasq restart\n");
    fflush(stdout);
    
    CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Triggering dnsmasq restart via sysevent\n"));
    
    ret = v_secure_system("sysevent set dhcp_server-restart");
    if (ret != 0)
    {
        printf("[DNS-COMMIT] sysevent FAILED (ret=%d)\n", ret);
        fflush(stdout);
        CcspTraceError(("CosaDmlDnsCommitForwardMax: sysevent failed (ret=%d)\n", ret));
        return ANSC_STATUS_FAILURE;
    }
    
    printf("[DNS-COMMIT] sysevent succeeded\n");
    fflush(stdout);
    
    CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Triggered dnsmasq restart\n"));
    
    /* STEP 3: Store in syscfg (only after file update and restart succeeded) */
    printf("[DNS-COMMIT] STEP 3: Writing to syscfg key='%s' value='%lu'\n", SYSCFG_DNS_FORWARD_MAX, value);
    fflush(stdout);
    
    rc = sprintf_s(buf, sizeof(buf), "%lu", value);
    if (rc < EOK)
    {
        printf("[DNS-COMMIT] sprintf_s for syscfg FAILED\n");
        fflush(stdout);
        CcspTraceError(("CosaDmlDnsCommitForwardMax: sprintf_s failed, rc=%d\n", rc));
        return ANSC_STATUS_FAILURE;
    }
    
    if (syscfg_set_commit(NULL, SYSCFG_DNS_FORWARD_MAX, buf) != 0)
    {
        printf("[DNS-COMMIT] syscfg_set_commit FAILED\n");
        fflush(stdout);
        CcspTraceError(("CosaDmlDnsCommitForwardMax: syscfg_set_commit FAILED\n"));
        return ANSC_STATUS_FAILURE;
    }
    
    printf("[DNS-COMMIT] syscfg write SUCCESS\n");
    fflush(stdout);
    
    CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Stored DNSForwardMax=%lu in syscfg\n", value));
    
    /* STEP 4: Clear pending state */
    g_PendingDnsForwardMaxSet = FALSE;
    g_PendingDnsForwardMax = 0;
    
    printf("[DNS-COMMIT] ===== Commit COMPLETE, cleared pending state =====\n");
    fflush(stdout);
    
    CcspTraceInfo(("CosaDmlDnsCommitForwardMax: Commit complete\n"));
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlDnsRollbackForwardMax
            (
                void
            );

    description:

        This function rolls back the pending DNS forward max value.

    argument:   None

    return:     ANSC_STATUS_SUCCESS

**********************************************************************/
ANSC_STATUS CosaDmlDnsRollbackForwardMax(void)
{
    if (g_PendingDnsForwardMaxSet)
    {
        CcspTraceInfo(("CosaDmlDnsRollbackForwardMax: Discarding pending value=%lu\n", 
                       g_PendingDnsForwardMax));
        g_PendingDnsForwardMaxSet = FALSE;
        g_PendingDnsForwardMax = 0;
    }
    else
    {
        CcspTraceInfo(("CosaDmlDnsRollbackForwardMax: No pending value to rollback\n"));
    }
    
    return ANSC_STATUS_SUCCESS;
}

#endif

