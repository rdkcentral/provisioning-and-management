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

#ifndef _COSA_RLOG_DML_H
#define _COSA_RLOG_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_rlog_internal.h"
#include "cosa_x_cisco_com_rlog_apis.h"

/**
 * @brief Retrieves a boolean parameter value from Device.X_CISCO_COM_RLog.
 *
 * This function retrieves boolean RLog configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pBool Pointer to boolean value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
RLog_GetParamBoolValue(ANSC_HANDLE hInsContext, char *ParamName, BOOL *pBool);

/**
 * @brief Retrieves an unsigned long parameter value from Device.X_CISCO_COM_RLog.
 *
 * This function retrieves numeric RLog configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pUlong Pointer to unsigned long value to be returned
 *
 * @return The status of the operation.
 * @retval TRUE Parameter retrieved successfully.
 * @retval FALSE Parameter not found.
 */
BOOL
RLog_GetParamUlongValue(ANSC_HANDLE hInsContext, char *ParamName, ULONG *pUlong);

/**
 * @brief Retrieves a string parameter value from Device.X_CISCO_COM_RLog.
 *
 * This function retrieves string RLog configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[out] pStr Buffer to receive parameter value string
 * @param[in,out] pSize Pointer to buffer size (in: buffer size, out: actual string length)
 *
 * @return The status of the operation.
 * @retval 0 Parameter retrieved successfully.
 * @retval 1 Buffer too small.
 * @retval -1 Parameter not found.
 */
ULONG
RLog_GetParamStringValue(ANSC_HANDLE hInsContext, char *ParamName, char *pStr, ULONG *pSize);

/**
 * @brief Sets a boolean parameter value for Device.X_CISCO_COM_RLog.
 *
 * This function sets boolean RLog configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] bValue Boolean value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RLog_SetParamBoolValue(ANSC_HANDLE hInsContext, char *ParamName, BOOL bValue);

/**
 * @brief Sets an unsigned long parameter value for Device.X_CISCO_COM_RLog.
 *
 * This function sets numeric RLog configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] ulValue Unsigned long value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RLog_SetParamUlongValue(ANSC_HANDLE hInsContext, char *ParamName, ULONG ulValue);

/**
 * @brief Sets a string parameter value for Device.X_CISCO_COM_RLog.
 *
 * This function sets string RLog configuration parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] ParamName Parameter name string
 * @param[in] sValue String value to set
 *
 * @return The status of the operation.
 * @retval TRUE Parameter set successfully.
 * @retval FALSE Parameter not found or setting failed.
 */
BOOL
RLog_SetParamStringValue(ANSC_HANDLE hInsContext, char *ParamName, char * sValue);

/**
 * @brief Validates Device.X_CISCO_COM_RLog pending configuration changes.
 *
 * This function validates RLog configuration parameters before committing.
 *
 * @param[in] hInsContext Instance context handle
 * @param[out] pFaultName Buffer to receive fault parameter name, if there's validation.(128 bytes)
 * @param[in,out] pLen Pointer to fault name buffer length
 *
 * @return The status of the validation.
 * @retval TRUE if there's no validation.
 * @retval FALSE if there's validation.
 */
BOOL
RLog_Validate(ANSC_HANDLE hInsContext, char *pFaultName, ULONG *pLen);

/**
 * @brief Commits Device.X_CISCO_COM_RLog configuration changes.
 *
 * This function applies validated RLog configuration changes, establishes connection
 * to remote sysl.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 Commit successful.
 * @retval non-zero Commit failed.
 */
ULONG
RLog_Commit(ANSC_HANDLE hInsContext);

/**
 * @brief Rolls back Device.X_CISCO_COM_RLog configuration changes.
 *
 * Roll back the update whenever there's a validation found.
 *
 * @param[in] hInsContext Instance context handle
 *
 * @return The status of the operation.
 * @retval 0 on success.
 * @retval Non-zero error code otherwise.
 */
ULONG
RLog_Rollback(ANSC_HANDLE hInsContext);

#endif /* _COSA_RLOG_DML_H */
