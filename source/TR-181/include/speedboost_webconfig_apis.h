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
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __SPEEDBOOST_WEBCONFIG_APIS_H__
#define __SPEEDBOOST_WEBCONFIG_APIS_H__

#include "webconfig_framework.h"

#define SPEEDBOOST_BLOB_DATA                    "Device.X_RDK_Speedboost.Data"
#define SPEEDBOOST_NUMBER_OF_CONFIGURED_DEVICES "Device.X_RDK_Speedboost.NumberConfiguredDevices"
#define SPEEDBOOST_NUMBER_OF_ELIGIBLE_DEVICES   "Device.X_RDK_Speedboost.NumberEligibleDevices"
#define SPEEDBOOST_CURRENT_ACTIVE_DEVICE_LIST   "Device.X_RDK_Speedboost.CurrentDeviceList"

/* As per Schema*/
#define XM_SPEED_BOOST_SUBDOC "xmspeedboost"
#define XM_SPEED_BOOST_RULES  "xm_speedboost_rules"

#define NINETY_DAYS_IN_SECONDS  (90 * 24 * 60 * 60)
#define MAX_DELAY_TIME_FOR_BLOB_RECEIVE 60

#define BUFF_SIZE_256 256
#define BUFF_SIZE_64  64
#define BUFF_SIZE_16  16
#define BUFF_SIZE_8   8

/**
* @brief Get the number of configured SpeedBoost clients.
*
* @return The number of configured SpeedBoost clients.
*
*/
int getNumOfConfiguredClients(void);

/**
* @brief Check if MAC address has valid hexadecimal characters and colon separators.
*
* @param[in] pMac  - MAC address string to validate.
*
* @return The validation result.
* @retval true if MAC address format is valid.
* @retval false if MAC address is NULL or has invalid characters.
*
*/
bool CheckMacHasValidCharacter (char* pMac);

/**
* @brief Validate MAC address format and content.
*
* @param[in] pPhysAddress  - MAC address string to validate.
*
* @return The validation result.
* @retval true if MAC address is valid.
* @retval false if MAC address is NULL, invalid length, all zeros, or has invalid format.
*
*/
bool ValidateMac(char* pPhysAddress);

/**
* @brief Validate Unix timestamp is within 90 days from current time.
*
* @param[in] unixTime  - Unix timestamp to validate.
* @param[out] pErrMessage  - Buffer to store error message if validation fails.
* @param[in] iSizeOfBuffer  - Size of the error message buffer.
*
* @return The validation result.
* @retval true if Unix time is valid.
* @retval false if Unix time is more than 90 days from now.
*
*/
bool validateUnixTime(int64_t unixTime, char * pErrMessage, int iSizeOfBuffer);

/**
* @brief Free resources allocated for SpeedBoost document.
*
* @param[in] pVoidArg  - Pointer to execData structure containing SpeedBoost document.
*
* @return None.
*/
void freeResourcesSpeedBoostDoc(void *pVoidArg);

/**
* @brief Process SpeedBoost webconfig request.
*
* @param[in] pVoidData  - Pointer to speedBoostDoc_t structure.
*
* @return Pointer to error structure.
* @retval BLOB_EXEC_SUCCESS if processing is successful.
* @retval NULL_BLOB_EXEC_POINTER if NULL pointer is passed.
* @retval VALIDATION_FALIED if validation fails.
* @retval SYSCFG_FAILURE if syscfg operation fails.
* @retval BLOB_EXEC_FAILURE if scheduler operation fails.
*
*/
pErr processSpeedBoostWebConfigRequest(void *pVoidData);

/**
* @brief Unpack and process SpeedBoost data from base64 encoded string.
*
* @param[in] pString  - Base64 encoded msgpack string.
*
* @return The processing result.
* @retval true if unpacking and processing are successful.
* @retval false if input is NULL, decoding fails, or processing fails.
*
*/
bool unpackAndProcessSpeedBoostData(char *pString);
#endif /*__SPEEDBOOST_WEBCONFIG_APIS_H__ */
