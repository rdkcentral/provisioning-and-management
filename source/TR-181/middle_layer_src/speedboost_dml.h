/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
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
 *
**********************************************************************/

#ifndef __COSA_SPEEDBOOST_DML_H__
#define __COSA_SPEEDBOOST_DML_H__

#include "cosa_apis.h"

/**
 * @brief Sets blob data for SpeedBoost webconfig parameter.
 *
 * This function sets binary blob data.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] pParamName Parameter name string
 * @param[in] pParamVal Blob data string to set
 *
 * @return The status of the operation.
 * @retval true if set successful.
 * @retval false otherwise.
 */
bool setBlobDataOfSpeedBoost(ANSC_HANDLE hInsContext, char* pParamName,char* pParamVal);

/**
 * @brief Retrieves blob data for SpeedBoost webconfig parameter.
 *
 * This function retrieves binary blob data.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] pParamName Parameter name string
 * @param[out] pVal Buffer to receive blob data
 * @param[in] iValSize Size of pVal buffer in bytes
 *
 * @return The status of the operation.
 * @retval true if retrieval successful.
 * @retval false otherwise.
 */
bool getBlobDataOfSpeedBoost(ANSC_HANDLE hInsContext, char* pParamName, char *pVal, int iValSize);

/**
 * @brief Retrieves an integer parameter value from SpeedBoost configuration.
 *
 * This function retrieves integer SpeedBoost parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] pParamName Parameter name string
 * @param[out] pVal Pointer to integer value to be returned
 *
 * @return The status of the operation.
 * @retval true if retrieval successful.
 * @retval false otherwise.
 */
bool getParamIntValOfSpeedBoost (ANSC_HANDLE hInsContext, char* pParamName, int* pVal);

/**
 * @brief Retrieves a string parameter value from SpeedBoost configuration.
 *
 * This function retrieves string SpeedBoost parameters.
 *
 * @param[in] hInsContext Instance context handle
 * @param[in] pParamName Parameter name string
 * @param[out] pParamVal Buffer to receive parameter value string
 * @param[in] iParamValSize Size of pParamVal buffer in bytes
 *
 * @return The status of the operation.
 * @retval true if retrieval successful.
 * @retval false otherwise
 */
bool getParamStringValOfSpeedBoost(ANSC_HANDLE hInsContext, char* pParamName, char* pParamVal, int iParamValSize);

#endif /*__COSA_SPEEDBOOST_DML_H__*/
