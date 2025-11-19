/**********************************************************************
 * Copyright 2017-2019 ARRIS Enterprises, LLC.
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
**********************************************************************/


#ifndef  _COSA_DSLITE_INTERNAL_H
#define  _COSA_DSLITE_INTERNAL_H

#include "cosa_apis.h"
#include "cosa_dslite_dml.h"


#define  COSA_IREP_FOLDER_NAME_DSLITE_IFS                  "DsliteInterfaceSetting"
#define  COSA_DML_RR_NAME_DsliteNextInsNumber              "NextInstanceNumber"
#define  COSA_DML_RR_NAME_DsliteAlias                       "Alias"
#define  COSA_DML_RR_NAME_DslitebNew                        "bNew"

#define  COSA_DSLITE_ROLLBACK_TEST                          0
/*
*  This struct is for dslite.
*/
#define  COSA_DATAMODEL_DSLITE_CLASS_CONTENT                                    \
    /* duplication of the base object class content */                          \
    COSA_BASE_CONTENT                                                           \
    /* start of InterfaceSetting object class content */                        \
    SLIST_HEADER                    DsliteList;                                 \
    ULONG                           maxInstanceOfDslite;                        \
    CHAR                            Alias[64+1];                                \
    ANSC_HANDLE                     hIrepFolderDslite;                          \

typedef  struct
_COSA_DATAMODEL_DSLITE
{
    COSA_DATAMODEL_DSLITE_CLASS_CONTENT
}
COSA_DATAMODEL_DSLITE,  *PCOSA_DATAMODEL_DSLITE;

#define DSLITE_SET_DEFAULTVALUE(pDslite)                                        \
    (pDslite)->active                               = FALSE;                    \
    (pDslite)->status                               = 2;                        \
    AnscZeroMemory((pDslite)->alias, sizeof((pDslite)->alias) );                \
    (pDslite)->mode                                 = 1;                        \
    (pDslite)->addr_type                            = 2;                        \
    AnscZeroMemory((pDslite)->addr_inuse, sizeof((pDslite)->addr_inuse));       \
    AnscZeroMemory((pDslite)->addr_fqdn, sizeof((pDslite)->addr_fqdn));         \
    AnscZeroMemory((pDslite)->addr_ipv6, sizeof((pDslite)->addr_ipv6));         \
    (pDslite)->origin                               = 0;                        \
    AnscZeroMemory((pDslite)->tunnel_interface, sizeof((pDslite)->tunnel_interface));       \
    AnscZeroMemory((pDslite)->tunneled_interface, sizeof((pDslite)->tunneled_interface));   \
    (pDslite)->mss_clamping_enable                  = 0;                        \
    (pDslite)->tcpmss                               = 1420;                     \
    (pDslite)->ipv6_frag_enable                     = 0;                        \

/*
    Function declaration
*/

/**
 * @brief Create the DS-Lite backend object.
 *
 * @return Handle to the DS-Lite backend object.
 * @retval Handle to the newly created DS-Lite backend object
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaDsliteCreate
    (
        VOID
    );

/**
 * @brief Initializes the DS-Lite backend object.
 *
 * @param[in] hThisObject Handle to the DS-Lite backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDsliteInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Removes and deallocates the DS-Lite backend object.
 *
 * @param[in] hThisObject Handle to the DS-Lite backend object to be removed.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful removal.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDsliteRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Fetches DS-Lite information from backend HAL APIs.
 *
 * @param[in] hThisObject Handle to the DS-Lite backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful backend fetch.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDsliteBackendGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Loads DS-Lite configuration from persistent registry storage.
 *
 * @param[in] hThisObject Handle to the DS-Lite backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful configuration load.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDsliteRegGetDsliteInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Saves DS-Lite configuration to persistent registry storage.
 *
 * @param[in] hThisObject Handle to the DS-Lite backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful configuration save.
 * @retval error code otherwise.
 */
ANSC_STATUS
CosaDsliteRegSetDsliteInfo
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
