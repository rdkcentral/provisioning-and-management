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

    module: plugin_main.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the exported apis for SNMP plugin.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        06/25/03    initial revision.

**********************************************************************/


#ifndef  _PLUGIN_MAIN_H
#define  _PLUGIN_MAIN_H

#define ANSC_EXPORT_API

#ifdef __cplusplus
extern "C"{
#endif

/***************************************************************************
 *
 *  BMEL stands for "Broadway MIB Extension Library"
 *
 ***************************************************************************/
/**
 * @brief Initializes the COSA plugin.
 *
 * This function performs synchronous initialization of the COSA plugin.
 *
 * @param[in] uMaxVersionSupported Maximum plugin version supported by the caller.
 * @param[in] hCosaPlugInfo Handle to COSA plugin information structure (PCOSA_PLUGIN_INFO).
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval non-zero error code on failure.
 */
int ANSC_EXPORT_API
COSA_Init
    (
        ULONG                       uMaxVersionSupported,
        void*                       hCosaPlugInfo         /* PCOSA_PLUGIN_INFO passed in by the caller */
    );

/**
 * @brief Performs asynchronous initialization of the COSA plugin.
 *
 * This function initiates asynchronous initialization of the COSA plugin.
 *
 * @param[in] uMaxVersionSupported Maximum plugin version supported by the caller.
 * @param[in] hCosaPlugInfo Handle to COSA plugin information structure (PCOSA_PLUGIN_INFO).
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval non-zero error code on failure.
 */
int ANSC_EXPORT_API
COSA_Async_Init
    (
        ULONG                       uMaxVersionSupported,
        void*                       hCosaPlugInfo         /* PCOSA_PLUGIN_INFO passed in by the caller */
    );

/**
 * @brief Checks if a specific data model object is supported by the plugin.
 *
 * This function queries whether the specified object path is supported in the current plugin implementation.
 *
 * @param[in] pObjName object path name to check.
 *
 * @return The status of the operation.
 * @retval TRUE if object is supported.
 * @retval FALSE otherwise.
 */
BOOL ANSC_EXPORT_API
COSA_IsObjectSupported
    (
        char*                        pObjName
    );

/**
 * @brief Unloads the COSA plugin and releases all resources.
 *
 * This function performs cleanup operations, releasing all memory and resources
 * allocated by the plugin during initialization and operation.
 *
 * @return void
 */
void ANSC_EXPORT_API
COSA_Unload
    (
        void
    );

/**
 * @brief Performs memory integrity check for the COSA plugin.
 *
 * This function checks for memory leaks and corruption in the plugin's memory allocations.
 *
 * @return void
 */
void ANSC_EXPORT_API
COSA_MemoryCheck
    (
        void
    );

/**
 * @brief Reports current memory usage statistics for the COSA plugin.
 *
 * This function outputs memory usage information to help monitor plugin resource consumption.
 *
 * @return void
 */
void ANSC_EXPORT_API
COSA_MemoryUsage
    (
        void
    );

/**
 * @brief Displays detailed memory allocation table for the COSA plugin.
 *
 * This function outputs a detailed table of memory allocations in the plugin.

 * @return void
 */
void ANSC_EXPORT_API
COSA_MemoryTable
    (
        void
    );

#ifdef __cplusplus
}
#endif
#endif
