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

    module: cosa_nat_internal.h

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


#ifndef  _COSA_NAT_INTERNAL_H
#define  _COSA_NAT_INTERNAL_H

#include "cosa_apis.h"


/***********************************
    Actual definition declaration
************************************/
#define  COSA_IREP_FOLDER_NAME_NAT                       "NAT"
#define  COSA_IREP_FOLDER_NAME_PORTMAPPING               "PORTMAPPING"
#define  COSA_IREP_FOLDER_NAME_PORTTRIGGER               "PORTTRIGGER"
#define  COSA_DML_RR_NAME_NATNextInsNumber               "NextInstanceNumber"
#define  COSA_DML_RR_NAME_NATAlias                       "Alias"
#define  COSA_DML_RR_NAME_NATbNew                        "bNew"


#define   COSA_DML_NAT_PORTMAPPING_ACCESS_INTERVAL   10 /* seconds*/


#define  COSA_DATAMODEL_NAT_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                                \
    COSA_BASE_CONTENT                                                                       \
    /* start of NAT object class content */                                                        \
    SLIST_HEADER                    NatPMappingList;                                        \
    SLIST_HEADER                    NatPTriggerList;                                        \
    COSA_DML_NAT                    Nat;                                                    \
    COSA_DML_NAT_DMZ                Dmz;                                                    \
    ULONG                           MaxInstanceNumber;                                      \
    ULONG                           ulPtNextInstanceNumber;                                 \
    ULONG                           PreviousVisitTime;                                      \
    UCHAR                           AliasOfPortMapping[64];                                 \
    ANSC_HANDLE                     hIrepFolderNat;                                         \
    ANSC_HANDLE                     hIrepFolderNatPt;                                       \

typedef  struct
_COSA_DATAMODEL_NAT
{
    COSA_DATAMODEL_NAT_CLASS_CONTENT
}
COSA_DATAMODEL_NAT,  *PCOSA_DATAMODEL_NAT;

/*
*  This struct is for creating entry context link in writable table when call GetEntry()
*/
#define  COSA_CONTEXT_PMAPPING_LINK_CLASS_CONTENT                                  \
        COSA_CONTEXT_LINK_CLASS_CONTENT                                            \
        BOOL                            bFound;                                    \

typedef  struct
_COSA_CONTEXT_PMAPPING_LINK_OBJECT
{
    COSA_CONTEXT_PMAPPING_LINK_CLASS_CONTENT
}
COSA_CONTEXT_PMAPPING_LINK_OBJECT,  *PCOSA_CONTEXT_PMAPPING_LINK_OBJECT;

#define  ACCESS_COSA_CONTEXT_PMAPPING_LINK_OBJECT(p)              \
         ACCESS_CONTAINER(p, COSA_CONTEXT_PMAPPING_LINK_OBJECT, Linkage)

#define NAT_PMAPPING_INITIATION_CONTEXT(pPmapping)                                   \
    COSA_CONTEXT_LINK_INITIATION_CONTENT(((PCOSA_CONTEXT_LINK_OBJECT)(pPmapping)))   \
    (pPmapping)->bFound                     =  FALSE;                                \


#define NAT_PORTMAPPING_SET_DEFAULTVALUE(pPortMapping)                                      \
    (pPortMapping)->bEnabled                 = FALSE;                                       \
    (pPortMapping)->Status                   = COSA_DML_NAT_STATUS_Disabled;                \
    (pPortMapping)->AllInterfaces            = FALSE;                                       \
    (pPortMapping)->LeaseDuration            = 0;                                           \
    (pPortMapping)->RemoteHost.Value         = 0;                                           \
    (pPortMapping)->ExternalPort             = 0;                                           \
    (pPortMapping)->ExternalPortEndRange     = 0;                                           \
    (pPortMapping)->InternalPort             = 0;                                           \
    (pPortMapping)->Protocol                 = 0;                                           \
    (pPortMapping)->InternalClient.Value     = 0x00000000;                                  \
    /*AnscZeroMemory( (pPortMapping)->Description, sizeof( (pPortMapping)->Description ) );   \
    AnscZeroMemory( (pPortMapping)->Interface, sizeof( (pPortMapping)->Interface ) );       \ */

/**********************************
    Standard function declaration
***********************************/

/**
 * @brief Create the NAT backend object.
 *
 * @return  Handle to the NAT backend object
 * @retval Handle to the newly created NAT backend object
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaNatCreate
    (
        VOID
    );

/**
 * @brief Initializes the NAT backend object.
 *
 * @param[in] hThisObject Handle to the NAT backend object.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful initialization
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNatInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the NAT backend object.
 *
 * @param[in] hThisObject Handle to the NAT backend object to be removed.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful removal
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNatRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Generates NAT port mapping rules.
 *
 * @param[in] hDml Handle to the NAT DML backend object.
 * @param[in] pEntry Pointer to the NAT port mapping configuration entry.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful rule generation
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNatGen
    (
        ANSC_HANDLE                 hDml,
        PCOSA_DML_NAT_PMAPPING      pEntry
    );

/**
 * @brief Generates NAT port triggering rules.
 *
 * @param[in] hDml Handle to the NAT DML backend object.
 * @param[in] pEntry Pointer to the NAT port trigger configuration entry.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful rule generation
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNatGenForTriggerEntry
    (
        ANSC_HANDLE                 hDml,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    );

/**
 * @brief Saves NAT configuration to persistent registry storage.
 *
 * @param[in] hThisObject Handle to the NAT backend object.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful configuration save
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNatRegSetNatInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Loads NAT configuration from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the NAT backend object.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful configuration load
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNatRegGetNatInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Synchronizes port trigger middle layer with backend on PAM load.
 *
 * @param[in] hThisObject Handle to the NAT backend object.
 *
 * @return status of operation
 * @retval ANSC_STATUS_SUCCESS on successful synchronization
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaNatSyncPortTrigger              /* Sync middle layer with backend on P&M load */
    (
        ANSC_HANDLE                 hThisObject
    );














#endif
