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

    module: cosa_time_apis.h

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


#ifndef  _COSA_TIME_API_H
#define  _COSA_TIME_API_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

typedef  enum
_COSA_DML_TIME_STATUS
{
    COSA_DML_TIME_STATUS_Disabled           = 1,
    COSA_DML_TIME_STATUS_Unsynchronized,
    COSA_DML_TIME_STATUS_Synchronized,
    COSA_DML_TIME_STATUS_ErrorFailedToSync,
    COSA_DML_TIME_STATUS_Error
}
COSA_DML_TIME_STATUS, *PCOSA_DML_TIME_STATUS;

typedef  struct
_COSA_BOOTSTRAP_STRING
{
    CHAR                    ActiveValue[64];
    CHAR                    UpdateSource[16];
}
COSA_BOOTSTRAP_STRING;

typedef  struct
_COSA_DML_TIME_CFG
{
    BOOLEAN                         bEnabled;
    COSA_BOOTSTRAP_STRING                            NTPServer1;     /* Either a host name of IP address */
    COSA_BOOTSTRAP_STRING                            NTPServer2;     /* Either a host name of IP address */
    COSA_BOOTSTRAP_STRING                            NTPServer3;     /* Either a host name of IP address */
    COSA_BOOTSTRAP_STRING                            NTPServer4;     /* Either a host name of IP address */
    COSA_BOOTSTRAP_STRING                            NTPServer5;     /* Either a host name of IP address */
    char                            LocalTimeZone[256]; /* Local time zone definition, encoded in IEEE 1003.1 */
    BOOLEAN                         bDaylightSaving;
    LONG                            DaylightSavingOffset;
    ULONG                           cityIndex;
    BOOLEAN                         bUTCEnabled;
}
COSA_DML_TIME_CFG,  *PCOSA_DML_TIME_CFG;

#define DEV_PROPERTIES_FILE "/etc/device.properties"
/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
* @brief Initialize the Time module and retrieve time configuration.
*
* This function initializes the Time module and retrieves the current time configuration
* from the system configuration.
*
* @param[in] hDml  - Handle to the DML object.
* @param[in,out] phContext  - Pointer to the context handle where the time configuration will be stored.
*                    \n The context should point to a COSA_DML_TIME_CFG structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlTimeInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
* @brief Set the Time configuration parameters.
*
* This function sets the Time configuration parameters including NTP servers, timezone,
* daylight saving settings, and enables/disables NTP synchronization.
*
* @param[in] hContext  - Handle to the context. This parameter is currently unused.
* @param[in] pTimeCfg  - Pointer to the Time configuration structure containing all time settings.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if pTimeCfg is NULL.
* @retval ERR_UTCTX_INIT if Utopia context initialization fails.
* @retval ERR_SYSCFG_FAILED if system configuration update fails.
*
*/
ANSC_STATUS
CosaDmlTimeSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    );

/**
* @brief Get the Time configuration parameters.
*
* This function retrieves the Time configuration parameters from system configuration
* including NTP servers, timezone, daylight saving settings, UTC enable status, and
* NTP enable status.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pTimeCfg  - Pointer to the Time configuration structure where retrieved settings will be stored.

*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if pTimeCfg is NULL.
* @retval ERR_UTCTX_INIT if Utopia context initialization fails.
* @retval ERR_SYSCFG_FAILED if system configuration retrieval fails.
*
*/
ANSC_STATUS
CosaDmlTimeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_CFG          pTimeCfg
    );

/**
* @brief Get the current Time synchronization status.
*
* This function retrieves the current NTP synchronization status from the system.
*
* @param[in] hContext  - Handle to the context.
* @param[out] pStatus  - Pointer to store the Time synchronization status.
* @param[out] pCurrLocalTime  - Pointer to the universal time structure.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ERR_UTCTX_INIT if Utopia context initialization fails.
*
*/
ANSC_STATUS
CosaDmlTimeGetState
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TIME_STATUS       pStatus,
        PANSC_UNIVERSAL_TIME        pCurrLocalTime
    );

/**
* @brief Get the current local time.
*
* This function retrieves the current local time adjusted for timezone.
*
* @param[in] hContext  - Handle to the context. This parameter is currently unused.
* @param[out] pCurrLocalTime  - Pointer to a buffer where the current local time string will be stored.
*                    \n The buffer should be allocated with sufficient size to hold the formatted time string.
*                    \n The format is "YYYY-MM-DD HH:MM:SS".
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlTimeGetLocalTime
    (
       ANSC_HANDLE                 hContext,
       char                       *pCurrLocalTime
    );

/**
* @brief Get the current UTC time.
*
* This function retrieves the current UTC time from the system .
*
* @param[in] hContext  - Handle to the context. This parameter is currently unused.
* @param[out] pCurrUTCTime  - Pointer to a buffer where the current UTC time string will be stored.
*                    \n The buffer should be allocated with sufficient size to hold the formatted time string.
*                    \n The format is "YYYY-MM-DD HH:MM:SS".
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlTimeGetUTCTime
    (
       ANSC_HANDLE                 hContext,
       char                       *pCurrUTCTime
    );

/**
* @brief Get the time offset from the server.
*
* This function retrieves the time offset value from the system.
*
* @param[in] hContext  - Handle to the context. This parameter is currently unused.
* @param[out] pTimeOffset  - Pointer to a buffer where the time offset string will be stored.
*                    \n The buffer should be allocated with at least MAX_COSATIMEOFFSET_SIZE (256) bytes.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
*
*/
ANSC_STATUS
CosaDmlTimeGetTimeOffset
    (
       ANSC_HANDLE                 hContext,
       char                       *pTimeOffset
    );

/**
* @brief Update a JSON parameter value in the partners configuration file.
*
* This function updates a specific parameter value in the partners_defaults.json file
* for a given partner ID.
*
* @param[in] pKey  - Pointer to the parameter key string to be updated .
* @param[in] PartnerId  - Pointer to the partner ID string for which the parameter should be updated.
* @param[in] pValue  - Pointer to the new value string to be set for the parameter.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if file opening fails, JSON parsing fails, partner ID not found, or parameter key not found.
*
*/
extern ANSC_STATUS UpdateJsonParamLegacy
       (
               char*                       pKey,
               char*                   PartnerId,
               char*                   pValue
    );

/**
* @brief Update a JSON parameter value in the bootstrap configuration file with metadata.
*
* This function updates a specific parameter value in the bootstrap.json file for a given
* partner ID.
*
* @param[in] pKey  - Pointer to the parameter key string to be updated.
* @param[in] PartnerId  - Pointer to the partner ID string for which the parameter should be updated.
* @param[in] pValue  - Pointer to the new value string to be set for the parameter.
* @param[in] pSource  - Pointer to the update source string indicating the origin of the update.
* @param[in] pCurrentTime  - Pointer to the current time string when the update is performed.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if file opening fails, JSON parsing fails, partner ID not found, or parameter key not found.
*
*/
extern ANSC_STATUS UpdateJsonParam
       (
               char*           pKey,
               char*                   PartnerId,
               char*                   pValue,
               char*                   pSource,
               char*                   pCurrentTime
    );

/**
* @brief Retrieve the current partner ID from system configuration.
*
* This function retrieves the current partner ID value from the system configuration.
*
* @param[out] pValue  - Pointer to a buffer where the partner ID string will be stored.
*                    \n The buffer should be allocated with sufficient size (typically PARTNER_ID_LEN bytes).
* @param[out] pulSize  - Pointer to store the size of the returned partner ID string.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if the operation is successful.
* @retval ANSC_STATUS_FAILURE if syscfg_get fails to retrieve the partner ID.
*
*/
extern ANSC_STATUS fillCurrentPartnerId
        (
                char*                       pValue,
        PULONG                      pulSize
    );

/**
* @brief Calculate the timezone offset in seconds.
*
* This function calculates and returns the timezone offset from UTC in seconds.
*
* @return The timezone offset in seconds from UTC.
*
*/
time_t getOffset();
#endif
