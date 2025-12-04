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

    module: cosa_x_cisco_com_Diag_apis.h

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


#ifndef  _COSA_DIAGNOSTICS_APIS_H
#define  _COSA_DIAGNOSTICS_APIS_H

#include "cosa_apis.h"


/***********************************
    Actual definition declaration
************************************/

/*
    Nat Part
*/
typedef  enum
_COSA_DML_DIAGNOSTICS_LEVEL
{
    COSA_DML_DIAGNOSTICS_LEVEL_EMERGENCY                = 1,
    COSA_DML_DIAGNOSTICS_LEVEL_ALERT,
    COSA_DML_DIAGNOSTICS_LEVEL_CRITICAL,
    COSA_DML_DIAGNOSTICS_LEVEL_ERROR,
    COSA_DML_DIAGNOSTICS_LEVEL_WARNING,
    COSA_DML_DIAGNOSTICS_LEVEL_NOTICE,
    COSA_DML_DIAGNOSTICS_LEVEL_INFORMATIONAL,
    COSA_DML_DIAGNOSTICS_LEVEL_DEBUG
}
COSA_DML_DIAGNOSTICS_LEVEL, *PCOSA_DML_DIAGNOSTICS_LEVEL;

typedef  struct
_COSA_DML_DIAGNOSTICS_ENTRY
{
    CHAR                            Time[64];
    CHAR                            Tag[16];
    COSA_DML_DIAGNOSTICS_LEVEL      Level;
    PCHAR                           pMessage;     /* The memory is supposed to be created by system malloc().
                                                                                            So it will be freed by offical system free() by caller. */
}
COSA_DML_DIAGNOSTICS_ENTRY,  *PCOSA_DML_DIAGNOSTICS_ENTRY;

#if 0
typedef  struct
_COSA_DML_DIAGNOSTICS_EVENTLOG
{
    CHAR                            Time[64];
    CHAR                            Tag[16];
    COSA_DML_DIAGNOSTICS_LEVEL      Level;
    PCHAR                           pMessage;     /* The memory is supposed to be created by system malloc().
                                                                                            So it will be freed by offical system free() by caller. */
}
COSA_DML_DIAGNOSTICS_EVENTLOG,  *PCOSA_DML_DIAGNOSTICS_EVENTLOG;
#endif

/*************************************
    The actual function declaration
**************************************/

/**
* @brief Initialize the diagnostics subsystem.
*
* This function initializes the diagnostics module.
*
* @param[in] hDml  - Handle to the DML.
* @param[out] phContext  - Pointer to handle for storing context.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlDiagnosticsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Get system log diagnostics entries.
*
* This function retrieves system log entries from the configured log files.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pulCount  - Pointer to store the number of diagnostics entries returned.
* @param[out] ppDiagEntry  - Pointer to an array of COSA_DML_DIAGNOSTICS_ENTRY structures.

*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if log file retrieval fails.
*
*/
ANSC_STATUS
CosaDmlDiagnosticsGetEntry
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_DIAGNOSTICS_ENTRY *ppDiagEntry
    );

/**
* @brief Get event log diagnostics entries.
*
* This function retrieves event log entries from the configured log files.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pulCount  - Pointer to store the number of diagnostics entries returned.
* @param[out] ppDiagnosticsEntry  - Pointer to an array of COSA_DML_DIAGNOSTICS_ENTRY structures.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if log file retrieval fails.
*
*/
ANSC_STATUS
CosaDmlDiagnosticsGetEventlog
    (
        ANSC_HANDLE                    hContext,
        PULONG                         pulCount,
        PCOSA_DML_DIAGNOSTICS_ENTRY *ppDiagnosticsEntry
    );

/**
* @brief Get all event log entries as a formatted string.
*
* This function retrieves all event log entries and formats them as a string with each entry.
*
* @param[out] pValue  - Pointer to a buffer where the formatted event log string will be returned.
* @param[in,out] pUlSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies the buffer size allocated.
*                    \n On output, if buffer is insufficient, contains the required size.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if buffer size is insufficient or EVT_LOG_FILE_V2 sysevent not found.
*
*/
ANSC_STATUS
CosaDmlDiagnosticsGetAllEventlog
    (
        char*                          pValue,
        ULONG*                         pUlSize
    );

/**
* @brief Get all system log entries as a formatted string.
*
* This function retrieves all system log entries and formats them as a string with each entry.
*
* @param[out] pValue  - Pointer to a buffer where the formatted system log string will be returned.
* @param[in,out] pUlSize  - Pointer to the size of pValue buffer.
*                    \n On input, specifies the buffer size allocated.
*                    \n On output, if buffer is insufficient, contains the required size.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if buffer size is insufficient or sysevent not found.
*
*/
ANSC_STATUS
CosaDmlDiagnosticsGetAllSyslog
    (
        char*                          pValue,
        ULONG*                         pUlSize
    );
#endif