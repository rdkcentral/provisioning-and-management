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

#ifndef  _COSA_X_COMCAST_COM_PARENTCONTROL_INTERNAL_H
#define  _COSA_X_COMCAST_COM_PARENTCONTROL_INTERNAL_H
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_comcast_com_parentalcontrol_apis.h"

#define COSA_IREP_FOLDER_NAME_PARENTALCONTROL           "ParentalControl"

#define COSA_DML_RR_NAME_PARENTALCONTROL_Alias          "PCAlias"
#define COSA_DML_RR_NAME_PARENTALCONTROL_InsNum         "PCInstanceNumber"
#define COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber  "PCNextInstanceNumber"
#define COSA_DML_RR_NAME_PARENTALCONTROL_NewlyAdded     "PCNewlyAdded"

#define COSA_DML_RR_NAME_MSSERV_Alias          "MSSERVAlias"
#define COSA_DML_RR_NAME_MSSERV_InsNum         "MSSERVInstanceNumber"
#define COSA_DML_RR_NAME_MSSERV_NextInsNumber  "MSSERVNextInstanceNumber"
#define COSA_DML_RR_NAME_MSSERV_NewlyAdded     "MSSERVNewlyAdded"

#define COSA_DML_RR_NAME_MSTRUSTEDUSER_Alias          "MSTRUSTEDUSERAlias"
#define COSA_DML_RR_NAME_MSTRUSTEDUSER_InsNum         "MSTRUSTEDUSERInstanceNumber"
#define COSA_DML_RR_NAME_MSTRUSTEDUSER_NextInsNumber  "MSTRUSTEDUSERNextInstanceNumber"
#define COSA_DML_RR_NAME_MSTRUSTEDUSER_NewlyAdded     "MSTRUSTEDUSERNewlyAdded"

#define COSA_DML_RR_NAME_MDDEV_Alias          "MDDEVAlias"
#define COSA_DML_RR_NAME_MDDEV_InsNum         "MDDEVInstanceNumber"
#define COSA_DML_RR_NAME_MDDEV_NextInsNumber  "MDDEVNextInstanceNumber"
#define COSA_DML_RR_NAME_MDDEV_NewlyAdded     "MDDEVNewlyAdded"

#define  COSA_DATAMODEL_PARENTALCONTROL_CLASS_CONTENT                         \
    COSA_BASE_CONTENT                                                       \
    /* ManagedSites */                                                      \
    COSA_DML_MANAGEDSITES                   ManagedSites;                    \
    COSA_DML_MANAGED_SERVS                  ManagedServs;                    \
    COSA_DML_MANAGED_DEVS                   ManagedDevs;                    \
    ANSC_HANDLE                             hIrepFolderCOSA;                \
    /* ManagedSites.BlockedURL.{i}. */                                       \
    SLIST_HEADER                            BlkUrlList;                    \
    ULONG                                   ulBlkUrlNextInsNum;            \
    ANSC_HANDLE                             hIrepFolderBlkUrl;              \
    /* ManagedSites.TrustedUser.{i}. */                                      \
    SLIST_HEADER                            TrustedUserList;                    \
    ULONG                                   ulTrustedUserNextInsNum;            \
    ANSC_HANDLE                             hIrepFolderTrustedUser;              \
    /* ManagedServices.Service.{i}. */                                      \
    SLIST_HEADER                            MSServList;                    \
    ULONG                                   ulMSServNextInsNum;            \
    ANSC_HANDLE                             hIrepFolderMSServ;              \
    /* ManagedServices.TrustedUser.{i}. */                                      \
    SLIST_HEADER                            MSTrustedUserList;                    \
    ULONG                                   ulMSTrustedUserNextInsNum;            \
    ANSC_HANDLE                             hIrepFolderMSTrustedUser;              \
    /* ManagedDevices.Redirect */                                      \
    COSA_DML_MD_RED                         MDRedirect;                \
    /* ManagedDevices.Device.{i}. */                                      \
    SLIST_HEADER                            MDDevList;                    \
    ULONG                                   ulMDDevNextInsNum;            \
    ANSC_HANDLE                             hIrepFolderMDDev;              \

typedef  struct
_COSA_DATAMODEL_PARETALCONTROL_CLASS_CONTENT
{
    COSA_DATAMODEL_PARENTALCONTROL_CLASS_CONTENT
}
COSA_DATAMODEL_PARENTALCONTROL, *PCOSA_DATAMODEL_PARENTALCONTROL;

/**
 * @brief Create the Comcast Parental Control backend object.
 *
 * @return Handle to the newly created Parental Control backend object
 * @retval Non-NULL Handle to the newly created Parental Control backend object.
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaParentalControlCreate
    (
        VOID
    );

/**
 * @brief Initialize the Comcast Parental Control backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful initialization.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaParentalControlInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Remove and cleanup the Comcast Parental Control backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on successful cleanup.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaParentalControlRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Loads BlockedURL entries from persistent storage into the Parental Control backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_BlkUrlGetInfo(
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Adds a new BlockedURL entry to persistent storage and updates the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the BlockedURL entry to add.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_BlkUrlAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Deletes a BlockedURL entry from persistent storage and the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the BlockedURL entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_BlkUrlDelInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Loads TrustedUser entries from persistent storage into the Parental Control backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_TrustedUserGetInfo(
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Adds a new TrustedUser entry to persistent storage and updates the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the TrustedUser entry to add.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_TrustedUserAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Deletes a TrustedUser entry from persistent storage and the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the TrustedUser entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_TrustedUserDelInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Loads Managed Service entries from persistent storage into the Parental Control backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_MSServGetInfo(
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Adds a new Managed Service entry to persistent storage and updates the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the Managed Service entry to add.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_MSServAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Deletes a Managed Service entry from persistent storage and the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the Managed Service entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_MSServDelInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Loads Managed Service TrustedUser entries from persistent storage into the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_MSTrustedUserGetInfo(
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Adds a new Managed Service TrustedUser entry to persistent storage and updates the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the TrustedUser entry to add.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_MSTrustedUserAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Deletes a Managed Service TrustedUser entry from persistent storage and the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the TrustedUser entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_MSTrustedUserDelInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Loads Managed Device entries from persistent storage into the Parental Control backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_MDDevGetInfo(
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Adds a new Managed Device entry to persistent storage and updates the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the Managed Device entry to add.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE on failure.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_MDDevAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Deletes a Managed Device entry from persistent storage and the backend.
 *
 * @param[in] hThisObject Handle to the Parental Control backend object.
 * @param[in] hCosaContext Context for the Managed Device entry to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS on success.
 * @retval ANSC_STATUS_FAILURE If folder deletion fails.
 * @retval Error code otherwise.
 */
ANSC_STATUS
CosaPcReg_MDDevDelInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

#endif
