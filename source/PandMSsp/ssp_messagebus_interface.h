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

/**********************************************************************

    module: ssp_messagebus_interface.h

        For CCSP Secure Software Download

    ---------------------------------------------------------------

    description:

        The header file for the CCSP Message Bus Interface
        Service.

    ---------------------------------------------------------------

    environment:

        Embedded Linux

    ---------------------------------------------------------------

    author:

        Tom Chang

    ---------------------------------------------------------------

    revision:

        06/23/2011  initial revision.

**********************************************************************/

#ifndef  _SSP_MESSAGEBUS_INTERFACE_
#define  _SSP_MESSAGEBUS_INTERFACE_

/**
* @brief Engage message bus for PandM component.
*
* @param[in] component_id  - Component ID string.
* @param[in] config_file  - Configuration file path.
* @param[in] path  - DBus path.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if message bus is engaged successfully.
* @retval ANSC_STATUS_FAILURE if component_id or path is NULL.
*
*/
ANSC_STATUS
ssp_PnmMbi_MessageBusEngage
    (
        char * component_id,
        char * config_file,
        char * path
    );

/**
* @brief Initialize PandM message bus interface.
*
* @param[in] user_data  - User data pointer.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if initialization is successful.
*
*/
int
ssp_PnmMbi_Initialize
    (
        void * user_data
    );

/**
* @brief Finalize PandM message bus interface.
*
* @param[in] user_data  - User data pointer.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if finalization is successful.
*
*/
int
ssp_PnmMbi_Finalize
    (
        void * user_data
    );

/**
* @brief Check message bus status.
*
* @param[in] user_data  - User data pointer.
*
* @return The status of the bus check.
* @retval 0 if bus check is successful.
*
*/
int
ssp_PnmMbi_Buscheck
    (
        void * user_data
    );

/**
* @brief Free resources based on priority.
*
* @param[in] priority  - Priority level for resource freeing.
* @param[in] user_data  - User data pointer.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if resources are freed successfully.
*
*/
int
ssp_PnmMbi_FreeResources
    (
        int priority,
        void * user_data
    );

/**
* @brief Send parameter value change signal.
*
* @param[in] pPamameterName  - Parameter name.
* @param[in] oldValue  - Old parameter value.
* @param[in] newValue  - New parameter value.
* @param[in] pAccessList  - Access list string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if signal is sent successfully.
*
*/
ANSC_STATUS
ssp_PnmMbi_SendParameterValueChangeSignal
    (
        char * pPamameterName,
        SLAP_VARIABLE * oldValue,
        SLAP_VARIABLE * newValue,
        char * pAccessList
    );

/**
* @brief Send transfer complete signal.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if signal is sent successfully.
*
*/
ANSC_STATUS
ssp_PnmMbi_SendTransferCompleteSignal
    (
        void
    );


/*
static DBusHandlerResult
path_message_func
    (
        DBusConnection  *conn,
        DBusMessage     *message,
        void            *user_data
    );
*/

/**
* @brief Register PandM component to Component Registrar.
*
* @param[in] hThisObject  - Handle to this object.
* @param[in] pParameterArray  - Parameter namespace array.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if registration is successful.
*
*/
ANSC_STATUS
ssp_PnmMbi_RegisterToCR
    (
        ANSC_HANDLE                     hThisObject,
        name_spaceType_t*               pParameterArray
    );


#endif
