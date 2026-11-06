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

/* Regression coverage for log preservation on WAN link down.
 *
 * Log preservation is driven by the generic "wan-status" sysevent (set for
 * every WAN mode), handled by EvtDispterWanStatusCallback().
 */

#include "pandmssp_mock.h"
#include "mocks/secureWrapperMock.h"

using namespace testing;

extern "C"
{
#include "cosa_common_util.h"
#include "speedboost_apis.h"
#include "cosa_rbus_handler_apis.h"
ANSC_HANDLE bus_handle = NULL;
SpeedBoost sSpeedBoost = {};
extern volatile bool gWanStatus;
}

namespace
{
constexpr const char* kBackupCmd = "/rdklogger/backupLogs.sh";
}

class WanFailoverLogTest : public PandMSSPTestFixture
{
protected:
    void SetUp() override
    {
        PandMSSPTestFixture::SetUp();
        g_secure_wrapperMock = new secure_wrapperMock();
    }

    void TearDown() override
    {
        delete g_secure_wrapperMock;
        g_secure_wrapperMock = nullptr;
        gWanStatus = false;
        PandMSSPTestFixture::TearDown();
    }
};

/* wan-status started -> stopped must preserve logs. */
TEST_F(WanFailoverLogTest, WanStatusStopped_AfterStarted_PreservesLogs)
{
    gWanStatus = true;
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(HasSubstr(kBackupCmd))).Times(1);

    EvtDispterWanStatusCallback("stopped");

    EXPECT_FALSE(gWanStatus);
}

/* A repeated wan-status stopped must not preserve logs again. */
TEST_F(WanFailoverLogTest, WanStatusStopped_WhenAlreadyStopped_DoesNotPreserveLogs)
{
    gWanStatus = false;
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(_)).Times(0);

    EvtDispterWanStatusCallback("stopped");

    EXPECT_FALSE(gWanStatus);
}

/* wan-status started must not preserve logs. */
TEST_F(WanFailoverLogTest, WanStatusStarted_DoesNotPreserveLogs)
{
    gWanStatus = false;
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(_)).Times(0);

    EvtDispterWanStatusCallback("started");

    EXPECT_TRUE(gWanStatus);
}

/* Intermediate states (starting/stopping) must not preserve logs or change state. */
TEST_F(WanFailoverLogTest, WanStatusIntermediate_DoesNotPreserveLogs)
{
    gWanStatus = true;
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(_)).Times(0);

    EvtDispterWanStatusCallback("stopping");
    EvtDispterWanStatusCallback("starting");

    EXPECT_TRUE(gWanStatus);
}

/* Each WAN down after recovery must preserve logs. */
TEST_F(WanFailoverLogTest, WanStatusFlap_PreservesLogsOnEachDown)
{
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(HasSubstr(kBackupCmd))).Times(2);

    EvtDispterWanStatusCallback("started");
    EvtDispterWanStatusCallback("stopped");
    EvtDispterWanStatusCallback("started");
    EvtDispterWanStatusCallback("stopped");
}
