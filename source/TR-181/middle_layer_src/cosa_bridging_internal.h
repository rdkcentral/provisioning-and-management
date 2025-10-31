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

    module: cosa_bridging_internal.h

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


#ifndef  _COSA_BRIDGING_INTERNAL_H
#define  _COSA_BRIDGING_INTERNAL_H

#include "cosa_bridging_apis.h"

#define COSA_IREP_FOLDER_NAME_BRIDGING                                "Bridging"
#define COSA_IREP_FOLDER_NAME_BRIDGINGHA                              "BridgingHalfAdded"
#define COSA_IREP_FOLDER_NAME_BRIDGING_BRG                            "Bridge"

#define COSA_DML_RR_NAME_Bridge_NextInsNunmber                        "NextBridgeInstanceNum"
#define COSA_DML_RR_NAME_Bridge_Upper_InsNum                          "UpperInstanceNumber"
#define COSA_DML_RR_NAME_Bridge_InsNum                                "InstanceNumber"
#define COSA_DML_RR_NAME_Bridge_Alias                                 "Alias"
#define COSA_DML_RR_NAME_Bridge_Prefix                                "Bridge"
#define COSA_DML_RR_NAME_Bridge_Port_NextInsNumber                    "NextPortInstanceNum"
#define COSA_DML_RR_NAME_Bridge_VLAN_NextInsNumber                    "NextVLANInstanceNum"
#define COSA_DML_RR_NAME_Bridge_VLANPort_NextInsNumber                "NextVLANPortInstanceNum"
#define COSA_DML_RR_NAME_Bridge_Port_Prefix                           "Port"
#define COSA_DML_RR_NAME_Bridge_VLAN_Prefix                           "Vlan"
#define COSA_DML_RR_NAME_Bridge_VLANPort_Prefix                       "VlanPort"

/***********************************
    Actual definition declaration
************************************/

/*
    Object definition
*/

#define  COSA_DATAMODEL_BRIDGING_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    /* start of NAT object class content */                                                 \
    SLIST_HEADER                     BridgeList;                                            \
    ULONG                            ulNextBridgeInstance;                                  \
    ANSC_HANDLE                      hIrepFolderCOSA;                                       \
    ANSC_HANDLE                      hIrepFolderBRG;                                        \
    ANSC_HANDLE                      hIrepFolderBRGHA;                                      \


typedef  struct
_COSA_DATAMODEL_BRIDGING
{
    COSA_DATAMODEL_BRIDGING_CLASS_CONTENT
}
COSA_DATAMODEL_BRIDGING,  *PCOSA_DATAMODEL_BRIDGING;

/**********************************
    Standard function declaration
***********************************/
/**
 * @brief Create the Bridging backend object.
 *
 * This function constructs Bridging backend object and return handle.
 *
 * @return  newly created Bridging backend object.
 * @retval Handle to the newly created Bridging backend object
 * @retval NULL on failure.
 */
ANSC_HANDLE
CosaBridgingCreate
    (
        VOID
    );

/**
 * @brief Initialize the Bridging data model object.
 *
 * This function initializes the Bridging data model object and loads configuration data.
 *
 * @param[in] hThisObject - Handle to the Bridging object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the initialization is successful.
 * @retval ANSC_STATUS_FAILURE if the initialization fails.
 *
 */
ANSC_STATUS
CosaBridgingInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Remove the Bridging data model object.
 *
 * This function removes and frees the Bridging data model object and associated resources.
 *
 * @param[in] hThisObject - Handle to the Bridging object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the removal is successful.
 * @retval ANSC_STATUS_FAILURE if the removal fails.
 *
 */
ANSC_STATUS
CosaBridgingRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Get Bridging configuration information from registry.
 *
 * This function retrieves Bridging configuration information from the persistent registry.
 *
 * @param[in] hThisObject - Handle to the Bridging object.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ANSC_STATUS
CosaBridgingRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief Add Bridging configuration information to registry.
 *
 * This function adds Bridging configuration information to the persistent registry.
 *
 * @param[in] hThisObject      - Handle to the Bridging object.
 * @param[in] pNextInsNumName  - Pointer to the next instance number name.
 * @param[in] ulNextInsNum     - The next instance number value.
 * @param[in] pPreffix         - Pointer to the prefix string.
 * @param[in] ulUpperInsNum    - The upper instance number.
 * @param[in] pAlias           - Pointer to the alias string.
 * @param[in] hCosaContext     - Handle to the COSA context.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ANSC_STATUS
CosaBridgingRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pNextInsNumName,
        ULONG                       ulNextInsNum,
        char*                       pPreffix,
        ULONG                       ulUpperInsNum,
        char*                       pAlias,
        ANSC_HANDLE                 hCosaContext
    );

/**
 * @brief Delete Bridging configuration information from registry.
 *
 * This function deletes Bridging configuration information from the persistent registry.
 *
 * @param[in] hThisObject  - Handle to the Bridging object.
 * @param[in] hCosaContext - Handle to the COSA context to delete.
 *
 * @return The status of the operation.
 * @retval ANSC_STATUS_SUCCESS if the operation is successful.
 * @retval ANSC_STATUS_FAILURE if the operation fails.
 *
 */
ANSC_STATUS
CosaBridgingRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

#endif
