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

#ifndef  _SSP_INTERNAL_
#define  _SSP_INTERNAL_

#define  CCSP_COMMON_COMPONENT_HEALTH_Red                   1
#define  CCSP_COMMON_COMPONENT_HEALTH_Yellow                2
#define  CCSP_COMMON_COMPONENT_HEALTH_Green                 3

#define  CCSP_COMMON_COMPONENT_STATE_Initializing           1
#define  CCSP_COMMON_COMPONENT_STATE_Running                2
#define  CCSP_COMMON_COMPONENT_STATE_Blocked                3
#define  CCSP_COMMON_COMPONENT_STATE_Paused                 3

#define  CCSP_COMMON_COMPONENT_FREERESOURCES_PRIORITY_High  1
#define  CCSP_COMMON_COMPONENT_FREERESOURCES_PRIORITY_Low   2


extern   PCCSP_COMPONENT_CFG                        gpPnmStartCfg;

typedef  struct
_COMPONENT_COMMON_DM
{
    char*                           Name;
    ULONG                           Version;
    char*                           Author;
    ULONG                           Health;
    ULONG                           State;

    BOOL                            LogEnable;
    ULONG                           LogLevel;

    ULONG                           MemMaxUsage;
    ULONG                           MemMinUsage;
    ULONG                           MemConsumed;
}
COMPONENT_COMMON_DM,  *PCOMPONENT_COMMON_DM;

#define ComponentCommonDmInit(component_common_dm)                                          \
        {                                                                                   \
            AnscZeroMemory(component_common_dm, sizeof(COMPONENT_COMMON_DM));               \
            component_common_dm->Name        = NULL;                                        \
            component_common_dm->Version     = 1;                                           \
            component_common_dm->Author      = NULL;                                        \
            component_common_dm->Health      = CCSP_COMMON_COMPONENT_HEALTH_Red;            \
            component_common_dm->State       = CCSP_COMMON_COMPONENT_STATE_Running;         \
            if(g_iTraceLevel >= CCSP_TRACE_LEVEL_EMERGENCY)                                 \
                component_common_dm->LogLevel = (ULONG) g_iTraceLevel;                     \
            component_common_dm->LogEnable   = TRUE;                                        \
            component_common_dm->MemMaxUsage = 0;                                           \
            component_common_dm->MemMinUsage = 0;                                           \
            component_common_dm->MemConsumed = 0;                                           \
        }


#define  ComponentCommonDmClean(component_common_dm)                                        \
         {                                                                                  \
            if ( component_common_dm->Name )                                                \
            {                                                                               \
                AnscFreeMemory(component_common_dm->Name);                                  \
            }                                                                               \
                                                                                            \
            if ( component_common_dm->Author )                                              \
            {                                                                               \
                AnscFreeMemory(component_common_dm->Author);                                \
            }                                                                               \
         }


#define  ComponentCommonDmFree(component_common_dm)                                         \
         {                                                                                  \
            ComponentCommonDmClean(component_common_dm);                                    \
            AnscFreeMemory(component_common_dm);                                            \
         }

/**
* @brief Dispatch commands for SSP operations.
*
* @param[in] command  - Command character ('e' = engage, 'r' = read, 'm' = memory, 't' = trace, 'c' = cancel).
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval -1 if any error is detected during the operation.
*
*/
int  cmd_dispatch(int  command);

/**
* @brief Load the data model.
*
* @return None.
*/
void load_data_model();

/**
* @brief Create PandM SSP component.
*
* @param[in] pStartCfg  - Component configuration structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if component is created successfully.
* @retval ANSC_STATUS_RESOURCES if memory allocation fails.
*
*/
ANSC_STATUS
ssp_create_pnm
    (
        PCCSP_COMPONENT_CFG         pStartCfg
    );

/**
* @brief Engage PandM SSP component and register with CR.
*
* @param[in] pStartCfg  - Component configuration structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if component is engaged successfully.
* @retval ANSC_STATUS_FAILURE if operation fails.
*
*/
ANSC_STATUS
ssp_engage_pnm
    (
        PCCSP_COMPONENT_CFG         pStartCfg
    );

/**
* @brief Cancel PandM SSP component and unregister from CR.
*
* @param[in] pStartCfg  - Component configuration structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if component is cancelled successfully.
* @retval ANSC_STATUS_FAILURE if operation fails.
*
*/
ANSC_STATUS
ssp_cancel_pnm
    (
        PCCSP_COMPONENT_CFG         pStartCfg
    );

/**
* @brief Load COSA plugin library.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if plugin library is loaded successfully.
*
*/
ANSC_STATUS
ssp_LoadCosaPluginLibrary
(
);

/**
* @brief Message bus task simulation.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if simulation is successful.
*
*/
ANSC_STATUS
MessageBusTaskSim
    (
        ANSC_HANDLE hThisObject
    );

/**
* @brief Get PandM component name.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Component name string.
*
*/
char*
ssp_PnmCCDmGetComponentName
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Get PandM component version.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Component version number.
*
*/
ULONG
ssp_PnmCCDmGetComponentVersion
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Get PandM component author.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Component author string.
*
*/
char*
ssp_PnmCCDmGetComponentAuthor
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Get PandM component health status.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Component health status.
*
*/
ULONG
ssp_PnmCCDmGetComponentHealth
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Get PandM component state.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Component state.
*
*/
ULONG
ssp_PnmCCDmGetComponentState
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Get PandM logging enabled status.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Logging enabled status.
* @retval TRUE if logging is enabled.
* @retval FALSE if logging is disabled.
*
*/
BOOL
ssp_PnmCCDmGetLoggingEnabled
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Set PandM logging enabled status.
*
* @param[in] hThisObject  - Handle to this object.
* @param[in] bEnabled  - Logging enabled flag (TRUE to enable, FALSE to disable).
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
ssp_PnmCCDmSetLoggingEnabled
    (
        ANSC_HANDLE                     hThisObject,
        BOOL                            bEnabled
    );

/**
* @brief Get PandM logging level.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Current logging level.
*
*/
ULONG
ssp_PnmCCDmGetLoggingLevel
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Set PandM logging level.
*
* @param[in] hThisObject  - Handle to this object.
* @param[in] LogLevel  - Logging level to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
ssp_PnmCCDmSetLoggingLevel
    (
        ANSC_HANDLE                     hThisObject,
        ULONG                           LogLevel
    );

/**
* @brief Get PandM maximum memory usage.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Maximum memory usage in bytes.
*
*/
ULONG
ssp_PnmCCDmGetMemMaxUsage
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Get PandM minimum memory usage.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Minimum memory usage in bytes.
*
*/
ULONG
ssp_PnmCCDmGetMemMinUsage
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Get PandM current memory consumed.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return Current memory consumed in bytes.
* @retval Current memory consumed
* @retval 0 if component memory size cannot be determined.
*
*/
ULONG
ssp_PnmCCDmGetMemConsumed
    (
        ANSC_HANDLE                     hThisObject
    );

/**
* @brief Apply changes to PandM component configuration.
*
* @param[in] hThisObject  - Handle to this object.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
ssp_PnmCCDmApplyChanges
    (
        ANSC_HANDLE                     hThisObject
    );

#endif
