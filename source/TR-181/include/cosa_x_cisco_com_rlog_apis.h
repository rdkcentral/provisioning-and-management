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

#ifndef _COSA_RLOG_APIS_H
#define _COSA_RLOG_APIS_H
#include "cosa_apis.h"

typedef enum
_COSA_DML_RLOG_PROTO
{
    COSA_DML_RLOG_PROTO_TCP     = 1,
    COSA_DML_RLOG_PROTO_UDP     = 2,
}
COSA_DML_RLOG_PROTO;

typedef struct
_COSA_DML_RLOG
{
    BOOL                Enable;
    char                Host[256];
    ULONG               Port;
    COSA_DML_RLOG_PROTO Protocol;
    char                Patterns[65];
}
COSA_DML_RLOG, *PCOSA_DML_RLOG;

/**
* @brief Initialize the remote logging subsystem.
*
* This function initializes the remote logging module by loading configuration from Utopia storage
* and restarting syslogd if remote logging is enabled.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if initialization is successful or syslogd is already running without remote logging.
* @retval ANSC_STATUS_FAILURE if configuration loading fails or syslogd restart fails.
*
*/
ANSC_STATUS
CosaDmlRLog_Init(void);

/**
* @brief Terminate the remote logging subsystem.
*
* This function terminates the remote logging module.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if termination is successful.
*
*/
ANSC_STATUS
CosaDmlRLog_Term(void);

/**
* @brief Validate the remote logging configuration.
*
* This function validates the remote logging configuration parameters. If Enable is TRUE, it checks
* that Host is not empty and Port is within valid range (0-65535).
*
* @param[in] pRLog  - Pointer to a COSA_DML_RLOG structure containing the configuration to validate.
*                    \n Enable must be TRUE for host/port validation.
*                    \n Host (max 256 bytes) must be non-empty if enabled.
*                    \n Port must be 0-65535.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if validation is successful.
* @retval ANSC_STATUS_FAILURE if pRLog is NULL, Host is empty when enabled, or Port exceeds 65535.
*
*/
ANSC_STATUS
CosaDmlRLog_Validate(PCOSA_DML_RLOG pRLog);

/**
* @brief Set the remote logging configuration.
*
* This function sets the remote logging configuration by restarting syslogd with new parameters and
* saving configuration to Utopia storage.
*
* @param[in] pRLog  - Pointer to a COSA_DML_RLOG structure containing the configuration to set.
*                    \n Enable controls whether remote logging is active.
*                    \n Host (max 256 bytes) specifies remote syslog server.
*                    \n Port (0-65535) specifies remote server port (0 uses default).
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if configuration is applied and saved successfully.
* @retval ANSC_STATUS_FAILURE if pRLog is NULL, syslogd restart fails, or configuration save fails.
*
*/
ANSC_STATUS
CosaDmlRLog_SetConf(PCOSA_DML_RLOG pRLog);

/**
* @brief Get the remote logging configuration.
*
* This function retrieves the remote logging configuration from Utopia storage.
*
* @param[out] pRLog  - Pointer to a COSA_DML_RLOG structure where configuration will be returned.
*                    \n Enable indicates if remote logging is active.
*                    \n Host (max 256 bytes) contains remote syslog server address.
*                    \n Port contains remote server port number.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if configuration is retrieved successfully.
* @retval ANSC_STATUS_FAILURE if pRLog is NULL or Utopia initialization/read fails.
*
*/
ANSC_STATUS
CosaDmlRLog_GetConf(PCOSA_DML_RLOG pRLog);

#endif /* _COSA_RLOG_APIS_H */
