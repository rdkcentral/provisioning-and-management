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
#include <utapi/utapi_dns.h>
#include "ccsp_psm_helper.h"
#include "secure_wrapper.h"

extern char g_Subsystem[32];
extern ANSC_HANDLE bus_handle;

#define DEFAULT_WAN_INTERFACE  "Device.IP.Interface.1"

static boolean_t g_DnsRelayEnabled = FALSE;
static const char *DNS_RELAY_RESOLV_CONF = "/var/resolv_lan.dnsmasq";
static const char *DNS_KEY_RESOLVCONF = "nameserver";

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
    FILE *fd = NULL;
    char dnsProxyStatus[256] = {};

    UtopiaContext ctx = {};

    if(g_DnsRelayEnabled == bEnabled)
    {
        CcspTraceWarning(("%s Relay is enabled \n",__FUNCTION__));
        return ANSC_STATUS_SUCCESS;
    }

    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    g_DnsRelayEnabled = bEnabled;
    int rc = Utopia_SetDnsRelayEnabled(&ctx, g_DnsRelayEnabled);
    Utopia_Free(&ctx, !rc);

    if (rc == 0)
    {
          if(bEnabled == FALSE)
          {
                v_secure_system("sysevent set dns_proxy_status stopped");
          }
          else
          {
              v_secure_system("sysevent set dns_proxy_status InProgress");
          }

          v_secure_system("sysevent set dhcp_server-stop");

          snprintf(dnsProxyStatus,sizeof(dnsProxyStatus),"sysevent get dhcp_server-status");
          fd = popen(dnsProxyStatus, "r");

          if (fd)
          {
              fgets(dnsProxyStatus,sizeof(dnsProxyStatus),fd);
              CcspTraceWarning(("dhcp_server status %s \n" ,dnsProxyStatus));
              pclose(fd);
          }

          if(strcmp(dnsProxyStatus,"stopped"))
          {
              v_secure_system("sysevent set dhcp_server-start");
              v_secure_system("sysevent set dns_proxy_status started");
          }
    }
    return ANSC_STATUS_SUCCESS ;
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
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY         pRelay
    )
{
    UNREFERENCED_PARAMETER(hContext);
    COSA_DML_DNS_STATUS status = COSA_DML_DNS_STATUS_Error;
    FILE *fp = NULL;
    const char *dnsmasqConfig = "/var/dnsmasq_lan.conf";
    int readLength = 256;
    char searchString[200];
    char buffer[readLength];
    boolean_t isRunning = FALSE;

    UtopiaContext ctx = {};
    if (!Utopia_Init(&ctx))
    {
        return status;
    }

    int rc = Utopia_GetDnsRelayEnabled(&ctx, &g_DnsRelayEnabled);
    if (rc != 0)
    {
        CcspTraceError(("%s Failed to get enable\n",__FUNCTION__));
    }
    else
    {
        pRelay->bEnabled = g_DnsRelayEnabled;
        if(g_DnsRelayEnabled == FALSE)
        {
            v_secure_system("sysevent set dns_proxy_status stopped");
            status = COSA_DML_DNS_STATUS_Disabled;
            CcspTraceWarning(("%s DNS Relay is Disabled \n",__FUNCTION__));
        }
        else
        {
            snprintf(searchString, sizeof(searchString), "ps | grep -i dnsmasq | grep '%s' | grep -v grep", dnsmasqConfig);
            fp = popen(searchString, "r");
            if(fp != NULL)
            {
                if (fgets(buffer, readLength-1, fp) != NULL)
                {
                    buffer[readLength-1] = '\0';
                    if( _ansc_strstr(buffer, "/var/dnsmasq_lan.conf")){
                        CcspTraceWarning(("DNS Relay LAN dnsmasq processs is Running \n"));
                        isRunning = TRUE;
                    }
                    else
                    {
                        CcspTraceWarning(("No DNS Relay LAN dnsmasq processs Found \n"));
                    }
                }
                pclose(fp);
            }
        }

        if((g_DnsRelayEnabled == TRUE) && (isRunning == TRUE))
        {
            status =  COSA_DML_DNS_STATUS_Enabled;
        }
        else if ((g_DnsRelayEnabled == TRUE) && (isRunning == FALSE))
        {
             status =  COSA_DML_DNS_STATUS_Error;
        }
        else
        {
             status = COSA_DML_DNS_STATUS_Disabled;
        }

        pRelay->Status = status;
    }

    Utopia_Free(&ctx, 0);
    return status;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlIpDnsGetRelayEnable
            (
                ANSC_HANDLE                 hContext
                PCOSA_DML_DNS_RELAY         pRelay
            )

    description:

        This function gets the Relay Enable Status.

    argument:   ANSC_HANDLE                         hContext
    argument:   PCOSA_DML_DNS_RELAY                 pRelay

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlIpDnsGetRelayEnable
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY         pRelay
    )
{
    UNREFERENCED_PARAMETER(hContext);
    pRelay->bEnabled = g_DnsRelayEnabled;
    return ANSC_STATUS_SUCCESS ;
}

static COSA_DML_DNS_STATUS GetDnsServerStatus(const char *ip)
{
    COSA_DML_DNS_STATUS status = COSA_DML_DNS_STATUS_Disabled;
    FILE *fd = NULL;
    char buff[256] = {0};
    const char *resolvConf = DNS_RELAY_RESOLV_CONF;
    const char *dnsKey = DNS_KEY_RESOLVCONF;

    if ((resolvConf) && (strlen(ip) > 1))
    {
        snprintf(buff, sizeof(buff) - 1, "cat %s | grep '%s' | grep -c -E '%s' ", resolvConf, dnsKey, ip);
        fd = popen(buff, "r");
        if (fd)
        {
            if ((NULL != fgets(buff, sizeof(buff), fd)) && (strncmp(buff,"0",1) != 0))
            {
                status = COSA_DML_DNS_STATUS_Enabled;
            }
            pclose(fd);
        }
    }

    return status;
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
    PCOSA_DML_DNS_RELAY_ENTRY pForward = NULL;
    DNS_Client_t dhcpcDns = {0};

    UtopiaContext ctx = {};
    int retPsmGet = CCSP_SUCCESS;
    int forwardCount = 0;
    int dynCount = 0;
    int staticCount = 0;
    int i = 0;
    *pulCount = 0;
    char* param_value = NULL;
    char param_name[256]= {0};
    unsigned int dns_static_enable = 0;

    if (!Utopia_Init(&ctx))
    {
        return NULL;
    }

    if(SUCCESS == Utopia_GetDNSServer(&ctx, &dhcpcDns)){
            for(i = DNS_CLIENT_NAMESERVER_CNT -1; i >= 0 && (dhcpcDns.dns_server[i][0] == 0) ; i--);
            dynCount = i+1;
    }

    staticCount = Utopia_GetNumberOfDnsForwards(&ctx);

    forwardCount = staticCount + dynCount;

    if (forwardCount == 0)
    {
        Utopia_Free(&ctx, 0);
        return NULL;
    }

    pForward = (PCOSA_DML_DNS_RELAY_ENTRY)AnscAllocateMemory(forwardCount * sizeof(COSA_DML_DNS_RELAY_ENTRY));
    if (!pForward)
    {
        Utopia_Free(&ctx, 0);
        return NULL;
    }

    *pulCount = forwardCount;
    int af = DNS_FAMILY_NONE;

    /*Getting static dns entries for instance 1-4*/
    for (i = 0; i < staticCount; i++)
    {
       pForward[i].InstanceNumber = i + 1;
       pForward[i].Type = COSA_DML_DNS_ADDR_SRC_Static;

       sprintf(param_name, FORWARDING_DNS_SERVER_ENABLE, i+1);
       retPsmGet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, param_name, NULL, &param_value);
       if (retPsmGet == CCSP_SUCCESS && param_value != NULL)
       {
           dns_static_enable = atoi(param_value);
           pForward[i].bEnabled = (dns_static_enable == 1) ? TRUE : FALSE;
           ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
       }

       sprintf(param_name, FORWARDING_DNS_SERVER_ALIAS, i+1);
       retPsmGet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, param_name, NULL, &param_value);
       if (retPsmGet == CCSP_SUCCESS && param_value != NULL)
       {
           AnscCopyString(pForward[i].Alias,param_value);
           ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
       }

       sprintf(param_name, FORWARDING_DNS_SERVER_INTERFACE, i+1);
       retPsmGet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, param_name, NULL, &param_value);
       if (retPsmGet == CCSP_SUCCESS && param_value != NULL)
       {
           AnscCopyString(pForward[i].Interface,param_value);
           ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
       }

       sprintf(param_name, FORWARDING_DNS_SERVER, i+1);
       retPsmGet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, param_name, NULL, &param_value);
       if (retPsmGet == CCSP_SUCCESS && param_value != NULL)
       {
           AnscCopyString(pForward[i].DNSServer,param_value);
           ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
       }

       pForward[i].Status = GetDnsServerStatus(pForward[i].DNSServer);

    }

    PCOSA_DML_DNS_RELAY_ENTRY pDynServer = pForward + staticCount;

    /*Getting Dynamic dns entries from instance 5*/
    for (i=0; i < dynCount; i++)
    {
       pDynServer[i].InstanceNumber = i + (staticCount + 1);
       AnscCopyString(pDynServer[i].DNSServer,  dhcpcDns.dns_server[i]);
       AnscCopyString(pDynServer[i].Interface, DEFAULT_WAN_INTERFACE);
       AnscCopyString(pDynServer[i].Alias, dhcpcDns.s_alias[i]);
       pDynServer[i].bEnabled = TRUE;
       pDynServer[i].Status = COSA_DML_DNS_STATUS_Enabled;
       af = CHECK_V4_V6(dhcpcDns.dns_server[i]);
       switch (af)
       {
           case DNS_FAMILY_IPV4:
               pDynServer[i].Type = COSA_DML_DNS_ADDR_SRC_DHCPV4;
               break;
           case DNS_FAMILY_IPV6:
               pDynServer[i].Type = COSA_DML_DNS_ADDR_SRC_DHCPV6;
               break;
           default:
               break;
        }
    }

    Utopia_Free(&ctx, 0);
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

int ForwardingDNS_SetParamValuesToDB( char *pParamName, char *pParamVal )
{
    int     retPsmSet  = CCSP_SUCCESS;
    /* Input Validation */
    if( ( NULL == pParamName) || ( NULL == pParamVal ) )
    {
        CcspTraceError(("%s Invalid Input Parameters\n",__FUNCTION__));
        return CCSP_FAILURE;
    }

    retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, pParamName, ccsp_string, pParamVal);
    if (retPsmSet != CCSP_SUCCESS) {
        CcspTraceError(("%s Error %d writing %s\n", __FUNCTION__, retPsmSet, pParamName));
    }

    return retPsmSet;
}

/**********************************************************************

    caller:     self

    prototype:

        void
        update_proxy_state
            (
                BOOLEAN                     bEnabled
            )

    description:

        This function update the DNS relay.Proxy State

    argument:    BOOLEAN          bEnabled

    return:     operation status.

**********************************************************************/
static int update_proxy_state (BOOLEAN bEnabled)
{
    char* dns_relay_1 = NULL;
    char* dns_relay_2 = NULL;
    char* dns_relay_3 = NULL;
    char* dns_relay_4 = NULL;
    int retPsmGet = CCSP_SUCCESS;
    int retStatus = 0;
    FILE *fp = NULL;

    retPsmGet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, "dmsb.dns.forwarding.1.dnsserver", NULL, &dns_relay_1);
    if ((retPsmGet == CCSP_SUCCESS) && (dns_relay_1 != NULL) && (bEnabled == TRUE ))
    {
        PSM_Get_Record_Value2(bus_handle, g_Subsystem, "dmsb.dns.forwarding.2.dnsserver", NULL, &dns_relay_2);
        PSM_Get_Record_Value2(bus_handle, g_Subsystem, "dmsb.dns.forwarding.3.dnsserver", NULL, &dns_relay_3);
        PSM_Get_Record_Value2(bus_handle, g_Subsystem, "dmsb.dns.forwarding.4.dnsserver", NULL, &dns_relay_4);

        if((strlen(dns_relay_1) <= 1) && (strlen(dns_relay_2) <= 1) && (strlen(dns_relay_3) <= 1) && (strlen(dns_relay_4) <= 1))
        {
            CcspTraceWarning(("DNS Relay is Enabled without any Static Server\n"));
            CosaDmlDnsEnableRelay(NULL, FALSE);
            fp = popen("sysevent set dhcp_server-stop;sysevent set dhcp_server-start","r");
            pclose(fp);
            retStatus = 1;
        }
        else {
            CcspTraceWarning(("DNS Relay is Enabled and Static Server Data found\n"));
        }
    }
    return retStatus;
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
    int retPsmSet = CCSP_SUCCESS;
    char param_name[256] = {0};
    char param_value[256] = {0};
    int instancenum = 0;
    char* oldDnsserver = NULL;
    FILE *fp = NULL;
    int retUpdateState = 0;

    instancenum = pEntry->InstanceNumber;

    CcspTraceWarning(("%s-%d: instancenum=%d \n",__FUNCTION__, __LINE__, instancenum));

    memset(param_value, 0, sizeof(param_value));
    memset(param_name, 0, sizeof(param_name));

    if(pEntry->bEnabled)
    {
        sprintf(param_value, "1");
    }
    else
    {
        sprintf(param_value, "0");
    }
    sprintf(param_name, FORWARDING_DNS_SERVER_ENABLE, instancenum);
    retPsmSet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, param_name, NULL, &oldDnsserver);
    if (retPsmSet == CCSP_SUCCESS && oldDnsserver != NULL)
    {
        if((strncasecmp(oldDnsserver,param_value,sizeof(param_value))!=0) && (g_DnsRelayEnabled == TRUE))
        {
            fp = popen("sysevent set dhcp_server-stop;sysevent set dhcp_server-start","r");
            pclose(fp);
        }
    }
    ForwardingDNS_SetParamValuesToDB(param_name,param_value);

    memset(param_value, 0, sizeof(param_value));
    memset(param_name, 0, sizeof(param_name));

    sprintf(param_value, "%s", pEntry->Alias);
    sprintf(param_name, FORWARDING_DNS_SERVER_ALIAS, instancenum);
    ForwardingDNS_SetParamValuesToDB(param_name,param_value);

    memset(param_value, 0, sizeof(param_value));
    memset(param_name, 0, sizeof(param_name));

    sprintf(param_value, "%s", pEntry->Interface);
    sprintf(param_name, FORWARDING_DNS_SERVER_INTERFACE, instancenum);
    ForwardingDNS_SetParamValuesToDB(param_name,param_value);

    memset(param_value, 0, sizeof(param_value));
    memset(param_name, 0, sizeof(param_name));

    sprintf(param_value, "%s", pEntry->DNSServer);
    sprintf(param_name, FORWARDING_DNS_SERVER, instancenum);

    retPsmSet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, param_name, NULL, &oldDnsserver);
    if (retPsmSet == CCSP_SUCCESS && oldDnsserver != NULL)
    {
        if((strncasecmp(oldDnsserver,param_value,sizeof(param_value))!=0) && (g_DnsRelayEnabled == TRUE))
        {
            fp = popen("sysevent set dhcp_server-stop;sysevent set dhcp_server-start","r");
            pclose(fp);
        }
    }
    ForwardingDNS_SetParamValuesToDB(param_name,param_value);
    retUpdateState = update_proxy_state(g_DnsRelayEnabled);
    if(retUpdateState)
    {
        pEntry->bEnabled = g_DnsRelayEnabled;
        CcspTraceWarning(("DNS Relay activation status updated %d - %d\n", g_DnsRelayEnabled, pEntry->bEnabled));
    }
    return ANSC_STATUS_SUCCESS ;
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
#endif

