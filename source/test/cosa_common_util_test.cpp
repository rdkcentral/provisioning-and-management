/*****************************************************************************
* If not stated otherwise in this file or this component's LICENSE file the
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
*******************************************************************************/

#include "pandmssp_mock.h"
#define RBUS_WAN_IP

using namespace testing;

extern "C"
{
#include "cosa_common_util.h"
extern void* Set_Notifi_ParamName(void *args);
}

ANSC_HANDLE bus_handle;
extern SyseventMock * g_syseventMock;

TEST (Set_Notifi_ParamNameTest, Set_Notifi_ParamName_Test1)
{
    arg_struct_t* args = nullptr;
    EXPECT_EQ(args, nullptr);
    EXPECT_EQ(Set_Notifi_ParamName(args), nullptr);
}
