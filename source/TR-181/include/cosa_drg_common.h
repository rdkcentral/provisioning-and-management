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


#ifndef _COSADRGCOMMON_
#if !defined(_COSA_SIM_)
#define _COSADRGCOMMON_
#include<stdint.h>
#include "sysevent/sysevent.h"

typedef struct _os_stats {
    uint64_t rxBytes;
    uint32_t rxPackets;
    uint32_t rxErrors;
    uint32_t rxDrops;
    uint32_t rxMulticast;
    uint64_t txBytes;
    uint32_t txPackets;
    uint32_t txErrors;
    uint32_t txDrops;
    uint32_t txMulticast;
} OSStats, *POSStats;

/**
* @brief Get operating system statistics for a network interface.
*
* This function retrieves network statistics (bytes, packets, errors, drops, multicast)
* for the specified network interface.
*
* @param[in] iface - Pointer to a null-terminated string containing the interface name
* @param[out] stats - Pointer to an OSStats structure where the statistics will be returned.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval -1 if the interface statistics cannot be retrieved.
*
*/
int getOSStats(char* iface, POSStats stats);



extern int commonSyseventFd;
extern token_t commonSyseventToken;


/**
* @brief Set a sysevent key-value pair.
*
* This function sets a key-value pair in the sysevent system using the common sysevent connection.
*
* @param[in] key - Pointer to a null-terminated string containing the sysevent key name.
* @param[in] value - Pointer to a null-terminated string containing the value to set for the key.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval Non-zero error code if the operation fails.
*
*/
int commonSyseventSet(char* key, char* value);

/**
* @brief Get a sysevent value by key.
*
* This function retrieves the value associated with a key from the sysevent system
* using the common sysevent connection.
*
* @param[in] key - Pointer to a null-terminated string containing the sysevent key name.
* @param[out] value - Pointer to a buffer where the retrieved value will be returned.
* @param[in] valLen - Maximum length of the value buffer.
*
* @return The status of the operation.
* @retval 0 if the operation is successful.
* @retval Non-zero error code if the operation fails.
*
*/
int commonSyseventGet(char* key, char* value, int valLen);

/**
* @brief Close the common sysevent connection.
*
* This function closes the common sysevent connection and releases associated resources.
*
* @return The status of the operation.
* @retval 0 if the operation is successful or if the connection was not open.
* @retval Non-zero error code if the operation fails.
*
*/
int commonSyseventClose();

#define CCSP_COMMON_FIFO "/tmp/ccsp_common_fifo"

#endif
#endif
