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
#ifndef __SPEEDBOOSTDOC_H__
#define __SPEEDBOOSTDOC_H__

#include <stdio.h>
#include "scheduler_interface.h"

#define ABSOLUTE_SCHEDULE "absolute"
#define RELATIVE_TIME_STR "time"
#define UNIX_TIME_STR "unix_time"
#define INDEXES_STR "indexes"

#define UNUSED_PARAMETER(x) (void)(x)

enum {
    SPEEDBOOST_INFO_OK = 0,
    SPEEDBOOST_INFO_ABSOLUTE_ERR,
    SPEEDBOOST_INFO_ACTIONS_ERR,
    SPEEDBOOST_INFO_TIMEZONE_ERR,
    SPEEDBOOST_INFO_ERR
};


typedef struct speedBoostDoc{
    schedule_info_t * pSchedulerInfo;
    char   *          pSubDocName;
    uint32_t          ui32Version;
    uint16_t          ui16TranscationId;
} speedBoostDoc_t;

/**
 * @brief Converts a msgpack buffer into an speedBoostDoc_t structure
 *  if possible.
 *
 *  @param[in] pBuf the buffer to convert
 *  @param[in] iLen the length of the buffer in bytes
 *
 * @return Pointer to the converted speedBoostDoc_t structure.
 * @retval Pointer to speedBoostDoc_t on success.
 * @retval NULL if an error occurred.
 */
speedBoostDoc_t* speedBoostDocConvert( const void *pBuf, size_t iLen );

/**
 * @brief Destroys an speedBoostDoc_t object.
 *
 *  @param[in] pSpeedBoostDoc the speedBoostDoc_t to destroy.
 *
 * @return None.
 */
void speedBoostDocDestroy( speedBoostDoc_t *pSpeedBoostDoc );

/**
 * @brief Allocate a memory for scheduler input_t structures
 *
 *  @param[in] iActionExecCount  Number of input_t structures of scheduler
 *
 * @return Pointer to allocated input_t structure array.
 * @retval NULL if allocation failed.
 * @retval Pointer to input_t on success.
 */

input_t* createScheduleInput(size_t iActionExecCount);

/**
 * @brief Convert the msgpack map into the schedule_info_t structure.
 *
 *  @param[in] pSchedulerInfo the entry pointer
 *  @param[in] pMsgPackObjMap  the msgpack map pointer
 *
 * @return The status of the operation.
 * @retval 0 if successful.
 * @retval Non-zero error code if any error is detected.
 */
int processSpeedBoostInfo( schedule_info_t * pSchedulerInfo, msgpack_object_map *pMsgPackObjMap );
#endif /*__SPEEDBOOSTDOC_H__s*/
