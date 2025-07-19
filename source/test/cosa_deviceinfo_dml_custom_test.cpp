/*****************************************************************************
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2025 RDK Management
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

using namespace testing;

extern PCOSA_BACKEND_MANAGER_OBJECT g_pCosaBEManager;
extern SyscfgMock * g_syscfgMock;
extern cosaDeviceInfoApisMock * g_cosaDeviceInfoApisMock;
extern cosaDeviceInfoApisCustomMock * g_cosaDeviceInfoApisCustomMock;

TEST_F (PandMSSPTestFixture, DeviceInfo_SetParamBoolValue_Custom_ConfigureWiFi_False)
{
    char ParamName[] = "X_RDKCENTRAL-COM_ConfigureWiFi";
    BOOL bValue = true;
    char mockVal[] = "false";

    g_pCosaBEManager = (PCOSA_BACKEND_MANAGER_OBJECT)malloc(sizeof(COSA_BACKEND_MANAGER_OBJECT));
    g_pCosaBEManager->hDeviceInfo = (PCOSA_DATAMODEL_DEVICEINFO)malloc(sizeof(COSA_DATAMODEL_DEVICEINFO));

    EXPECT_CALL(*g_syscfgMock, syscfg_get(_, StrEq("WiFiPersonalizationSupport"), _, _)).Times(1)
    .WillOnce(testing::DoAll(
              testing::SetArrayArgument<2>(mockVal, mockVal + strlen(mockVal) + 1),
              Return(0)
    ));

    BOOL res = DeviceInfo_SetParamBoolValue_Custom(NULL, ParamName, bValue);
    EXPECT_EQ(res, true);
}
