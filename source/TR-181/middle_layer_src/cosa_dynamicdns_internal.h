/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2021 RDK Management
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

#if defined(DDNS_BROADBANDFORUM)

#ifndef  _COSA_DYNAMICDNS_INTERNAL_H
#define  _COSA_DYNAMICDNS_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_dynamicdns_apis.h"

#define COSA_IREP_FOLDER_NAME_DDNS_CLIENT           "DynamicDnsClient"
#define COSA_DML_RR_NAME_DDNS_CLIENT_Alias          "DynamicDnsClientAlias"
#define COSA_DML_RR_NAME_DDNS_CLIENT_InsNum         "DynamicDnsClientInstanceNumber"
#define COSA_DML_RR_NAME_DDNS_CLIENT_NextInsNumber  "DynamicDnsClientNextInstanceNumber"
#define COSA_DML_RR_NAME_DDNS_CLIENT_NewlyAdded     "DynamicDnsClientNewlyAdded"
#define COSA_IREP_FOLDER_NAME_DDNS_HOST             "DynamicDnsHost"
#define COSA_DML_RR_NAME_DDNS_HOST_Alias            "DynamicDnsHostAlias"
#define COSA_DML_RR_NAME_DDNS_HOST_InsNum           "DynamicDnsHostInstanceNumber"
#define COSA_DML_RR_NAME_DDNS_HOST_NextInsNumber    "DynamicDnsHostNextInstanceNumber"
#define COSA_IREP_FOLDER_NAME_DDNS_SERVER           "DynamicDnsServer"
#define COSA_DML_RR_NAME_DDNS_SERVER_Alias          "DynamicDnsServerAlias"
#define COSA_DML_RR_NAME_DDNS_SERVER_InsNum         "DynamicDnsServerInstanceNumber"
#define COSA_DML_RR_NAME_DDNS_SERVER_NextInsNumber  "DynamicDnsServerNextInstanceNumber"

#define  COSA_DATAMODEL_TR181_DDNS_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                             \
    COSA_BASE_CONTENT                                                                   \
    /* start of DynamicDNS.client object class content */                                          \
    SLIST_HEADER                    DDNSClientList;                                     \
    ULONG                           DDNSClientNextInsNum;                               \
    ANSC_HANDLE                     hIrepFolderDDNSClient;                              \
    /* start of DynamicDNS.client.{i}.hostname object class content */                             \
    SLIST_HEADER                    DDNSHostList;                                       \
    ULONG                           DDNSHostNextInsNum;                                 \
    ANSC_HANDLE                     hIrepFolderDDNSHost;                                \
    /* start of DynamicDNS.Server.{i}. */                                                          \
    SLIST_HEADER                    DDNSServerList;                                     \
    ULONG                           DDNSServerNextInsNum;                               \
    ANSC_HANDLE                     hIrepFolderDDNSServer;                              \

typedef  struct
_COSA_DATAMODEL_TR181_DDNS
{
    COSA_DATAMODEL_TR181_DDNS_CLASS_CONTENT
}
COSA_DATAMODEL_TR181_DDNS,  *PCOSA_DATAMODEL_TR181_DDNS;

/**********************************
    Standard function declaration
***********************************/

/**
 * @brief Create the Dynamic DNS backend object.
 *
 * @return Handle to the DS-Lite backend object.
 * @retval Handle to the newly created DS-Lite backend object
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaDynamicDnsCreate
    (
        VOID
    );

/**
 * @brief Initializes the Dynamic DNS backend object.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDynamicDnsInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the Dynamic DNS backend object.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object to be removed.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful removal.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDynamicDnsRemove
    (
        ANSC_HANDLE                 hThisObject
    );

//Dynamic DNS Client
/**
 * @brief Retrieves Dynamic DNS client information from persistent registry.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful retrieval.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDynamicDns_ClientGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );
/**
 * @brief Adds Dynamic DNS client entry to persistent registry.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the client entry.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful addition.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDynamicDns_ClientAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
/**
 * @brief Deletes Dynamic DNS client entry from persistent registry.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the client entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful deletion.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDynamicDns_ClientDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

//Dynamic DNS Host
/**
 * @brief Retrieves Dynamic DNS host information from persistent registry.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful retrieval.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDynamicDns_HostGetInfo
    (
       ANSC_HANDLE                 hThisObject
    );
/**
 * @brief Adds Dynamic DNS host entry to persistent registry.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the host entry.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful addition.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDynamicDns_HostAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
/**
 * @brief Deletes Dynamic DNS host entry from persistent registry.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the host entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful deletion.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDynamicDns_HostDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

//Dynamic DNS Server
/**
 * @brief Retrieves Dynamic DNS server information from persistent registry.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful retrieval.
 * @retval  error code otherwise.
 */
ANSC_STATUS
CosaDynamicDns_ServerGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );
/**
 * @brief Adds Dynamic DNS server entry to persistent registry.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the server entry.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful addition.
 * @retval  error code otherwise.
 */
ANSC_STATUS
CosaDynamicDns_ServerAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
/**
 * @brief Deletes Dynamic DNS server entry from persistent registry.
 *
 * @param[in] hThisObject Handle to the Dynamic DNS backend object.
 * @param[in] hCosaContext Handle to the COSA context object for the server entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful deletion.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDynamicDns_ServerDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
#endif //_COSA_DYNAMICDNS_INTERNAL_H


#endif
