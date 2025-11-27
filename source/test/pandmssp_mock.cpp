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
#include <gmock/gmock.h>
#include "pandmssp_mock.h"

#ifndef PANDMSSP_FIXTURE_G_MOCK
#define PANDMSSP_FIXTURE_G_MOCK

PCOSA_BACKEND_MANAGER_OBJECT g_pCosaBEManager = NULL;

rbusHandlerMock * g_rbusHandlerMock = nullptr;
PsmMock * g_psmMock = nullptr;
BaseAPIMock * g_baseapiMock = nullptr;
rbusMock *g_rbusMock = nullptr;
utopiaMock *g_utopiaMock = nullptr;
UserTimeMock * g_usertimeMock = nullptr;
TraceMock * g_traceMock = nullptr;
SyscfgMock * g_syscfgMock = nullptr;
SyseventMock * g_syseventMock = nullptr;
SafecLibMock * g_safecLibMock = nullptr;
PtdHandlerMock * g_PtdHandlerMock = nullptr;
helpersMock * g_helpersMock = nullptr;
cosaWebconfigApisMock * g_cosaWebconfigApisMock = nullptr;
speedboostRbusHandlersMock * g_speedboostRbusHandlersMock = nullptr;
speedboostSchedulerApisMock * g_speedboostSchedulerApisMock = nullptr;
scheduleLibMock * g_scheduleLibMock = nullptr;
webconfigFwMock *g_webconfigFwMock = nullptr;
AnscMemoryMock * g_anscMemoryMock = nullptr;
base64Mock *g_base64Mock = nullptr;
msgpackMock *g_msgpackMock = nullptr;
cosaDeviceInfoApisMock * g_cosaDeviceInfoApisMock = nullptr;
parodusInterfaceMock *g_parodusInterfaceMock = nullptr;
cosaDeviceInfoApisCustomMock * g_cosaDeviceInfoApisCustomMock = nullptr;

PandMSSPTestFixture::PandMSSPTestFixture()
{
    g_rbusHandlerMock = new rbusHandlerMock;
    g_rbusMock = new rbusMock;
    g_psmMock = new PsmMock;
    g_baseapiMock = new BaseAPIMock;
    g_utopiaMock = new utopiaMock;
    g_usertimeMock = new UserTimeMock;
    g_traceMock = new TraceMock;
    g_syscfgMock = new SyscfgMock;
    g_safecLibMock = new SafecLibMock;
    g_PtdHandlerMock = new ::testing::NiceMock<PtdHandlerMock>();
    g_helpersMock = new helpersMock;
    g_scheduleLibMock = new scheduleLibMock;
    g_webconfigFwMock = new webconfigFwMock;
    g_cosaWebconfigApisMock = new cosaWebconfigApisMock;
    g_speedboostRbusHandlersMock = new speedboostRbusHandlersMock;
    g_speedboostSchedulerApisMock = new speedboostSchedulerApisMock;
    g_anscMemoryMock = new AnscMemoryMock;
    g_base64Mock = new base64Mock;
    g_msgpackMock = new msgpackMock;
    g_cosaDeviceInfoApisMock = new cosaDeviceInfoApisMock;
    g_parodusInterfaceMock = new parodusInterfaceMock;
    g_cosaDeviceInfoApisCustomMock = new cosaDeviceInfoApisCustomMock;
}

PandMSSPTestFixture::~PandMSSPTestFixture()
{
    delete g_rbusHandlerMock;
    delete g_psmMock;
    delete g_baseapiMock;
    delete g_rbusMock;
    delete g_utopiaMock;
    delete g_usertimeMock;
    delete g_traceMock;
    delete g_syscfgMock;
    delete g_safecLibMock;
    delete g_PtdHandlerMock;
    delete g_helpersMock;
    delete g_cosaWebconfigApisMock;
    delete g_speedboostRbusHandlersMock;
    delete g_speedboostSchedulerApisMock;
    delete g_webconfigFwMock;
    delete g_scheduleLibMock;
    delete g_anscMemoryMock;
    delete g_base64Mock;
    delete g_msgpackMock;
    delete g_cosaDeviceInfoApisMock;
    delete g_parodusInterfaceMock;
    delete g_cosaDeviceInfoApisCustomMock;

    g_rbusHandlerMock = nullptr;
    g_utopiaMock = nullptr;
    g_rbusMock = nullptr;
    g_usertimeMock = nullptr;
    g_traceMock = nullptr;
    g_syscfgMock = nullptr;
    g_safecLibMock = nullptr;
    g_PtdHandlerMock = nullptr;
    g_psmMock = nullptr;
    g_baseapiMock = nullptr;
    g_helpersMock = nullptr;
    g_scheduleLibMock = nullptr;
    g_webconfigFwMock = nullptr;
    g_cosaWebconfigApisMock = nullptr;
    g_speedboostRbusHandlersMock = nullptr;
    g_speedboostSchedulerApisMock = nullptr;
    g_anscMemoryMock = nullptr;
    g_base64Mock = nullptr;
    g_msgpackMock = nullptr;
    g_cosaDeviceInfoApisMock = nullptr;
    g_parodusInterfaceMock = nullptr;
    g_cosaDeviceInfoApisCustomMock = nullptr;
}

void PandMSSPTestFixture::SetUp()
{
}

void PandMSSPTestFixture::TearDown() {}
void PandMSSPTestFixture::TestBody() {}

#endif