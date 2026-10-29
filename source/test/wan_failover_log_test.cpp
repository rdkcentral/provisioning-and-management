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

/* Regression coverage for WAN failover log preservation.
 *
 * This suite is built in a non-extender RBUS configuration
 * (RBUS_BUILD_FLAG_ENABLE + WAN_FAILOVER_SUPPORTED, RDKB_EXTENDER_ENABLED
 * undefined) so that Cosa_Rbus_Handler_WanStatus_EventHandler and
 * Cosa_Rbus_Handler_RfSignalStatus_EventHandler are compiled in. Both are
 * static, so they are captured via the handler argument passed to the mocked
 * rbusEvent_Subscribe() and then invoked directly.
 */

#include "pandmssp_mock.h"
#include "mocks/secureWrapperMock.h"

using namespace testing;

extern "C"
{
#include "cosa_rbus_handler_apis.h"
}

extern rbusMock* g_rbusMock;

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
        PandMSSPTestFixture::TearDown();
    }

    /* Drives Cosa_Rbus_Handler_SubscribeWanStatusEvent() and captures the
       WAN and RF event handlers from the mocked rbusEvent_Subscribe() calls. */
    void SubscribeAndCapture(rbusEventHandler_t* wanHandler, rbusEventHandler_t* rfHandler)
    {
        EXPECT_CALL(*g_rbusMock, rbusEvent_Subscribe(_, StrEq(WANMGR_CURRENT_STATUS_TR181), _, _, _))
            .WillOnce(DoAll(SaveArg<2>(wanHandler), Return(RBUS_ERROR_SUCCESS)));
        EXPECT_CALL(*g_rbusMock, rbusEvent_Subscribe(_, StrEq(CABLE_MODEM_RF_SIGNAL_STATUS), _, _, _))
            .WillOnce(DoAll(SaveArg<2>(rfHandler), Return(RBUS_ERROR_SUCCESS)));

        Cosa_Rbus_Handler_SubscribeWanStatusEvent();
    }
};

/* WAN status "Down" must preserve logs via the backup script. */
TEST_F(WanFailoverLogTest, WanStatusDown_PreservesLogs)
{
    rbusEventHandler_t wanHandler = nullptr;
    rbusEventHandler_t rfHandler = nullptr;
    SubscribeAndCapture(&wanHandler, &rfHandler);
    ASSERT_NE(wanHandler, nullptr);

    int dummy = 0;
    rbusValue_t value = reinterpret_cast<rbusValue_t>(&dummy);
    rbusObject_t data = reinterpret_cast<rbusObject_t>(&dummy);

    rbusEvent_t ev;
    memset(&ev, 0, sizeof(ev));
    ev.name = WANMGR_CURRENT_STATUS_TR181;
    ev.data = data;

    EXPECT_CALL(*g_rbusMock, rbusObject_GetValue(data, _)).WillOnce(Return(value));
    EXPECT_CALL(*g_rbusMock, rbusValue_GetString(value, _)).WillOnce(Return("Down"));
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(HasSubstr(kBackupCmd))).Times(1);

    wanHandler(nullptr, &ev, nullptr);
}

/* WAN status "Up" must not preserve logs (only the network-response script runs). */
TEST_F(WanFailoverLogTest, WanStatusUp_DoesNotPreserveLogs)
{
    rbusEventHandler_t wanHandler = nullptr;
    rbusEventHandler_t rfHandler = nullptr;
    SubscribeAndCapture(&wanHandler, &rfHandler);
    ASSERT_NE(wanHandler, nullptr);

    int dummy = 0;
    rbusValue_t value = reinterpret_cast<rbusValue_t>(&dummy);
    rbusObject_t data = reinterpret_cast<rbusObject_t>(&dummy);

    rbusEvent_t ev;
    memset(&ev, 0, sizeof(ev));
    ev.name = WANMGR_CURRENT_STATUS_TR181;
    ev.data = data;

    EXPECT_CALL(*g_rbusMock, rbusObject_GetValue(data, _)).WillOnce(Return(value));
    EXPECT_CALL(*g_rbusMock, rbusValue_GetString(value, _)).WillOnce(Return("Up"));
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(HasSubstr(kBackupCmd))).Times(0);
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(Not(HasSubstr(kBackupCmd)))).Times(AnyNumber());

    wanHandler(nullptr, &ev, nullptr);
}

/* RF signal outage (CableRfSignalStatus == false) must preserve logs. */
TEST_F(WanFailoverLogTest, RfSignalOutage_PreservesLogs)
{
    rbusEventHandler_t wanHandler = nullptr;
    rbusEventHandler_t rfHandler = nullptr;
    SubscribeAndCapture(&wanHandler, &rfHandler);
    ASSERT_NE(rfHandler, nullptr);

    int dummy = 0;
    rbusValue_t value = reinterpret_cast<rbusValue_t>(&dummy);
    rbusObject_t data = reinterpret_cast<rbusObject_t>(&dummy);

    rbusEvent_t ev;
    memset(&ev, 0, sizeof(ev));
    ev.name = CABLE_MODEM_RF_SIGNAL_STATUS;
    ev.data = data;

    EXPECT_CALL(*g_rbusMock, rbusObject_GetValue(data, _)).WillOnce(Return(value));
    EXPECT_CALL(*g_rbusMock, rbusValue_GetType(value)).WillOnce(Return(RBUS_BOOLEAN));
    EXPECT_CALL(*g_rbusMock, rbusValue_GetBoolean(value)).WillOnce(Return(false));
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(HasSubstr(kBackupCmd))).Times(1);

    rfHandler(nullptr, &ev, nullptr);
}

/* RF signal present (CableRfSignalStatus == true) must not preserve logs. */
TEST_F(WanFailoverLogTest, RfSignalPresent_DoesNotPreserveLogs)
{
    rbusEventHandler_t wanHandler = nullptr;
    rbusEventHandler_t rfHandler = nullptr;
    SubscribeAndCapture(&wanHandler, &rfHandler);
    ASSERT_NE(rfHandler, nullptr);

    int dummy = 0;
    rbusValue_t value = reinterpret_cast<rbusValue_t>(&dummy);
    rbusObject_t data = reinterpret_cast<rbusObject_t>(&dummy);

    rbusEvent_t ev;
    memset(&ev, 0, sizeof(ev));
    ev.name = CABLE_MODEM_RF_SIGNAL_STATUS;
    ev.data = data;

    EXPECT_CALL(*g_rbusMock, rbusObject_GetValue(data, _)).WillOnce(Return(value));
    EXPECT_CALL(*g_rbusMock, rbusValue_GetType(value)).WillOnce(Return(RBUS_BOOLEAN));
    EXPECT_CALL(*g_rbusMock, rbusValue_GetBoolean(value)).WillOnce(Return(true));
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(_)).Times(0);

    rfHandler(nullptr, &ev, nullptr);
}

/* A non-boolean RF value is invalid and must not preserve logs. */
TEST_F(WanFailoverLogTest, RfSignalInvalidType_DoesNotPreserveLogs)
{
    rbusEventHandler_t wanHandler = nullptr;
    rbusEventHandler_t rfHandler = nullptr;
    SubscribeAndCapture(&wanHandler, &rfHandler);
    ASSERT_NE(rfHandler, nullptr);

    int dummy = 0;
    rbusValue_t value = reinterpret_cast<rbusValue_t>(&dummy);
    rbusObject_t data = reinterpret_cast<rbusObject_t>(&dummy);

    rbusEvent_t ev;
    memset(&ev, 0, sizeof(ev));
    ev.name = CABLE_MODEM_RF_SIGNAL_STATUS;
    ev.data = data;

    EXPECT_CALL(*g_rbusMock, rbusObject_GetValue(data, _)).WillOnce(Return(value));
    EXPECT_CALL(*g_rbusMock, rbusValue_GetType(value)).WillOnce(Return(RBUS_STRING));
    EXPECT_CALL(*g_secure_wrapperMock, v_secure_system(_)).Times(0);

    rfHandler(nullptr, &ev, nullptr);
}

/* The RF subscription must still be attempted even if the WAN subscription fails. */
TEST_F(WanFailoverLogTest, WanSubscribeFails_RfSubscribeStillAttempted)
{
    EXPECT_CALL(*g_rbusMock, rbusEvent_Subscribe(_, StrEq(WANMGR_CURRENT_STATUS_TR181), _, _, _))
        .WillOnce(Return(RBUS_ERROR_BUS_ERROR));
    EXPECT_CALL(*g_rbusMock, rbusEvent_Subscribe(_, StrEq(CABLE_MODEM_RF_SIGNAL_STATUS), _, _, _))
        .WillOnce(Return(RBUS_ERROR_SUCCESS));

    Cosa_Rbus_Handler_SubscribeWanStatusEvent();
}
