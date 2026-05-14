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
#define WAN_FAILOVER_SUPPORTED
#define RDKB_EXTENDER_ENABLED

using namespace testing;

extern "C"
{
#include "cosa_rbus_handler_apis.h"
}

char  g_Subsystem[32]         = {0};
extern rbusMock *g_rbusMock;
extern rbusHandlerMock * g_rbusHandlerMock;
extern PtdHandlerMock * g_PtdHandlerMock;
extern SyseventMock * g_syseventMock;

extern unsigned int gSubscribersCount;
extern unsigned int gSubscribersCount_IPv4;
extern unsigned int gSubscribersCount_IPv6;
extern unsigned int gWanStateSubscribersCount;
extern rbusHandle_t handle;

TEST (eventDevctrlSubHandler1, eventDevctrlSubHandler_Test1)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_SUBSCRIBE;
    const char *eventName = "Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount = 1;

    EXPECT_EQ(eventName,"Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode");
    EXPECT_EQ(action, RBUS_EVENT_ACTION_SUBSCRIBE);
    gSubscribersCount += 1;

    rbusError_t ret = eventDevctrlSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventDevctrlSubHandlerTest2, eventDevctrlSubHandler_Test2)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = "Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount = 1;

    EXPECT_EQ(eventName, "Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode");
    EXPECT_EQ(action, RBUS_EVENT_ACTION_UNSUBSCRIBE);
    EXPECT_NE(0, gSubscribersCount);
    gSubscribersCount -= 1;

    rbusError_t ret = eventDevctrlSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventDevctrlSubHandlerTest3, eventDevctrlSubHandler_Test3)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action;
    const char *eventName = "Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount = 0;

    rbusError_t ret = eventDevctrlSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventDevctrlSubHandlerTest4, eventDevctrlSubHandler_Test4)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = "Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount = 1;

    EXPECT_EQ(eventName, "Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode");
    EXPECT_EQ(action, RBUS_EVENT_ACTION_UNSUBSCRIBE);
    EXPECT_GT(gSubscribersCount, 0);

    rbusError_t ret = eventDevctrlSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(0, gSubscribersCount);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (sendUpdateEventTest1, sendUpdateEvent_Test1)
{
    void* new_val = nullptr;
    uint32_t old_val = 1;
    rbusValueType_t rbus_type = RBUS_INT32;
    char* pParamName = "Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode";

    rbusError_t ret = sendUpdateEvent(pParamName,new_val,(void*)&old_val,rbus_type);
    EXPECT_EQ(ret, RBUS_ERROR_BUS_ERROR);
}

TEST (sendUpdateEventTest2, sendUpdateEvent_Test2)
{
    uint32_t new_val = 0;
    uint32_t old_val = 1;
    rbusValueType_t rbus_type = RBUS_INT32;
    char* pParamName = nullptr;

    rbusError_t ret = sendUpdateEvent(pParamName,(void*)&new_val,(void*)&old_val,rbus_type);
    EXPECT_EQ(ret, RBUS_ERROR_BUS_ERROR);
}

TEST (sendUpdateEventTest3, sendUpdateEvent_Test3)
{
    uint32_t new_val = 0;
    uint32_t old_val = 1;
    rbusValueType_t rbus_type = RBUS_INT32;
    char* pParamName = "Device.X_RDKCENTRAL-COM_DeviceControl.DeviceNetworkingMode";
    rbusHandle_t handle = nullptr;

    rbusError_t ret = sendUpdateEvent(pParamName,(void*)&new_val,(void*)&old_val,rbus_type);
    EXPECT_EQ(ret, RBUS_ERROR_BUS_ERROR);
}

TEST (GetParamNameTest1, GetParamName_1)
{
    const char* path = "Device.DeviceInfo.X_COMCAST-COM_WAN_IP";
    EXPECT_STREQ(GetParamName(path), "X_COMCAST-COM_WAN_IP");
}

TEST (GetParamNameTest2, GetParamName_2)
{
    const char* path = "X_COMCAST-COM_WAN_IPv6";
    EXPECT_STREQ(GetParamName(path), "X_COMCAST-COM_WAN_IPv6");
}

TEST (GetParamNameTest3, GetParamName_3)
{
    const char* path = ".X_COMCAST-COM_WAN_IP";
    EXPECT_STREQ(GetParamName(path), "X_COMCAST-COM_WAN_IP");
}

TEST (publishWanIpAddrTest1, publishWanIpAddrTest1)
{
    char* event_name = "Device.DeviceInfo.X_COMCAST-COM_WAN_IP";
    char* new_val = "67.164.67.77";
    char* old_val = nullptr;

    rbusError_t ret1 = publishWanIpAddr(event_name, new_val, old_val);
    EXPECT_EQ(ret1, RBUS_ERROR_BUS_ERROR);
}

TEST (publishWanIpAddrTest2, publishWanIpAddrTest2)
{
    char* event_name = "Device.DeviceInfo.X_COMCAST-COM_WAN_IP";
    char* new_val = nullptr;
    char* old_val = "67.164.67.77";

    rbusError_t ret1 = publishWanIpAddr(event_name, new_val, old_val);
    EXPECT_EQ(ret1, RBUS_ERROR_BUS_ERROR);
}

TEST (publishWanIpAddrTest3, publishWanIpAddrTest3)
{
    char* event_name = nullptr;
    char* new_val = "67.164.67.77";
    char* old_val = "67.164.67.76";

    rbusError_t ret1 = publishWanIpAddr(event_name, new_val, old_val);
    EXPECT_EQ(ret1, RBUS_ERROR_BUS_ERROR);
}

TEST_F(PandMSSPTestFixture, PublishWanIpAddr_ThreadCreationFailure)
{
    char* event_name = "Device.DeviceInfo.X_COMCAST-COM_WAN_IP";
    char* new_val = "67.164.67.77";
    char* old_val = "0.0.0.0";

    EXPECT_CALL(*g_PtdHandlerMock, pthread_create(_, _, _, _))
        .WillOnce(Return(-1));

    rbusError_t result = publishWanIpAddr(event_name, new_val, old_val);
    EXPECT_EQ(result, RBUS_ERROR_BUS_ERROR);
}

TEST_F(PandMSSPTestFixture, PublishWanIpAddr_ThreadCreationSuccess)
{
    char* event_name = "Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6";
    char* new_val = "67.164.67.77";
    char* old_val = "10.0.0.1";

    EXPECT_CALL(*g_PtdHandlerMock, pthread_create(_, _, _, _))
        .WillOnce(Return(0));

    rbusError_t result = publishWanIpAddr(event_name, new_val, old_val);
    EXPECT_EQ(result, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest1, eventWANIPSubHandler_Test1)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_SUBSCRIBE;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IP";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount_IPv4 = 1;

    EXPECT_EQ(eventName,"Device.DeviceInfo.X_COMCAST-COM_WAN_IP");
    EXPECT_EQ(action, RBUS_EVENT_ACTION_SUBSCRIBE);
    gSubscribersCount_IPv4 += 1;

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest2, eventWANIPSubHandler_Test2)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IP";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount_IPv4 = 1;

    EXPECT_EQ(eventName,"Device.DeviceInfo.X_COMCAST-COM_WAN_IP");
    EXPECT_EQ(action, RBUS_EVENT_ACTION_UNSUBSCRIBE);
    EXPECT_NE(0, gSubscribersCount_IPv4);
    gSubscribersCount_IPv4 -= 1;

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest3, eventWANIPSubHandler_Test3)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_SUBSCRIBE;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount_IPv6 = 1;

    EXPECT_EQ(eventName,"Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6");
    EXPECT_EQ(action, RBUS_EVENT_ACTION_SUBSCRIBE);
    gSubscribersCount_IPv6 += 1;

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest4, eventWANIPSubHandler_Test4)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount_IPv6 = 1;

    EXPECT_EQ(eventName,"Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6");
    EXPECT_EQ(action, RBUS_EVENT_ACTION_UNSUBSCRIBE);
    EXPECT_NE(0, gSubscribersCount_IPv6);
    gSubscribersCount_IPv6 -= 1;

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest5, eventWANIPSubHandler_Test5)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IP";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount_IPv4 = 0;

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest6, eventWANIPSubHandler_Test6)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount_IPv6 = 1;

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest4, eventWANIPSubHandler_Test7)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount_IPv6 = 1;

    EXPECT_EQ(eventName,"Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6");
    EXPECT_EQ(action, RBUS_EVENT_ACTION_UNSUBSCRIBE);
    EXPECT_GT(gSubscribersCount_IPv6, 0);

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(0, gSubscribersCount_IPv6);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest4, eventWANIPSubHandler_Test8)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IP";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    gSubscribersCount_IPv4 = 1;

    EXPECT_EQ(eventName,"Device.DeviceInfo.X_COMCAST-COM_WAN_IP");
    EXPECT_EQ(action, RBUS_EVENT_ACTION_UNSUBSCRIBE);
    EXPECT_GT(gSubscribersCount_IPv4, 0);

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(0, gSubscribersCount_IPv4);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest4, eventWANIPSubHandler_Test9)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IP";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    EXPECT_NE(eventName,"X_COMCAST-COM_WAN_IP");

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST (eventWANIPSubHandlerTest4, eventWANIPSubHandler_Test10)
{
    rbusHandle_t handle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = "Device.DeviceInfo.X_COMCAST-COM_WAN_IPv6";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = false;
    EXPECT_NE(eventName,"X_COMCAST-COM_WAN_IPv6");

    rbusError_t ret = eventWANIPSubHandler(handle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST_F (PandMSSPTestFixture, devCtrlRbusInit_Test1)
{
    int rc = RBUS_ERROR_SUCCESS;
    rbusStatus_t status = RBUS_ENABLED;

    EXPECT_CALL(*g_rbusMock, rbus_checkStatus())
        .Times(1)
        .WillOnce(Return(RBUS_DISABLED));
    EXPECT_NE(status, RBUS_DISABLED);

    rbusError_t ret = devCtrlRbusInit();
    EXPECT_EQ(ret, RBUS_ERROR_BUS_ERROR);
}

TEST_F (PandMSSPTestFixture, devCtrlRbusInit_Test2)
{
    int rc = RBUS_ERROR_SUCCESS;
    rbusStatus_t status = RBUS_ENABLED;

    EXPECT_CALL(*g_rbusMock, rbus_checkStatus())
        .Times(1)
        .WillOnce(Return(RBUS_ENABLED));
    EXPECT_NE(status, RBUS_DISABLED);

    EXPECT_CALL(*g_rbusMock, rbus_open(_, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_BUS_ERROR));
    EXPECT_NE(rc, RBUS_ERROR_BUS_ERROR);

    rbusError_t ret = devCtrlRbusInit();
    EXPECT_EQ(ret, RBUS_ERROR_NOT_INITIALIZED);
}

TEST_F (PandMSSPTestFixture, devCtrlRbusInit_Test3)
{
    int rc = RBUS_ERROR_SUCCESS;
    rbusStatus_t status = RBUS_ENABLED;

    EXPECT_CALL(*g_rbusMock, rbus_checkStatus())
        .Times(1)
        .WillOnce(Return(RBUS_ENABLED));
    EXPECT_NE(status, RBUS_DISABLED);

    EXPECT_CALL(*g_rbusMock, rbus_open(_, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_SUCCESS));
    EXPECT_NE(rc, RBUS_ERROR_BUS_ERROR);

    EXPECT_CALL(*g_rbusMock, rbus_regDataElements(_, _, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_BUS_ERROR));
    EXPECT_NE(rc, RBUS_ERROR_BUS_ERROR);

    EXPECT_CALL(*g_rbusMock, rbus_close(_))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_BUS_ERROR));
    EXPECT_NE(rc, RBUS_ERROR_BUS_ERROR);

    rbusError_t ret = devCtrlRbusInit();
    EXPECT_EQ(ret, RBUS_ERROR_BUS_ERROR);
}

int sysevent_fd = -1;

TEST_F (PandMSSPTestFixture, devCtrlRbusInit_Test4)
{
    int rc = RBUS_ERROR_SUCCESS;
    rbusStatus_t status = RBUS_ENABLED;

    EXPECT_CALL(*g_rbusMock, rbus_checkStatus())
        .Times(1)
        .WillOnce(Return(RBUS_ENABLED));
    EXPECT_NE(status, RBUS_DISABLED);

    EXPECT_CALL(*g_rbusMock, rbus_open(_, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_SUCCESS));
    EXPECT_NE(rc, RBUS_ERROR_BUS_ERROR);

    EXPECT_CALL(*g_rbusMock, rbus_regDataElements(_, _, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_SUCCESS));
    EXPECT_NE(rc, RBUS_ERROR_BUS_ERROR);

    sysevent_fd = 1;
    EXPECT_LT(0, sysevent_fd);

    bool res = PAM_Rbus_SyseventInit();
    EXPECT_EQ(res, true);

    rbusError_t ret = devCtrlRbusInit();
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

TEST_F (PandMSSPTestFixture, devCtrlRbusInit_Test6)
{
    int rc = RBUS_ERROR_SUCCESS;
    rbusStatus_t status = RBUS_ENABLED;

    EXPECT_CALL(*g_rbusMock, rbus_checkStatus())
        .Times(1)
        .WillOnce(Return(RBUS_ENABLED));
    EXPECT_NE(status, RBUS_DISABLED);

    EXPECT_CALL(*g_rbusMock, rbus_open(_, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_SUCCESS));
    EXPECT_NE(rc, RBUS_ERROR_BUS_ERROR);

    EXPECT_CALL(*g_rbusMock, rbus_regDataElements(_, _, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_SUCCESS));
    EXPECT_NE(rc, RBUS_ERROR_BUS_ERROR);

    sysevent_fd = 1;
    EXPECT_GT(sysevent_fd, 0);

    bool res = PAM_Rbus_SyseventInit();
    EXPECT_FALSE(res);

    rbusError_t ret = devCtrlRbusInit();
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
}

/***********************************************************************

  Unit Tests for getWanStateHandler

 ***********************************************************************/

TEST (getWanStateHandlerTest1, getWanStateHandler_ReturnsDefaultState)
{
    rbusHandle_t testHandle = NULL;
    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);

    rbusError_t ret = getWanStateHandler(testHandle, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusValue_t value = rbusProperty_GetValue(property);
    ASSERT_NE(value, nullptr);
    const char *str = rbusValue_GetString(value, NULL);
    ASSERT_NE(str, nullptr);

    rbusProperty_Release(property);
}

TEST (getWanStateHandlerTest2, getWanStateHandler_ReturnsUpdatedState)
{
    /* Set WAN state by calling publishWanStateEvent with no subscribers to
       update the cached g_wanState without actually publishing */
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 0;
    publishWanStateEvent("Manageable");
    handle = savedHandle;

    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);

    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusValue_t value = rbusProperty_GetValue(property);
    ASSERT_NE(value, nullptr);
    const char *str = rbusValue_GetString(value, NULL);
    ASSERT_NE(str, nullptr);
    EXPECT_STREQ(str, "Manageable");

    rbusProperty_Release(property);
}

TEST (getWanStateHandlerTest3, getWanStateHandler_ReturnsUnknownForNullInput)
{
    /* publishWanStateEvent with NULL should set state to "Unknown" */
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 0;
    publishWanStateEvent(NULL);
    handle = savedHandle;

    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);

    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusValue_t value = rbusProperty_GetValue(property);
    ASSERT_NE(value, nullptr);
    const char *str = rbusValue_GetString(value, NULL);
    ASSERT_NE(str, nullptr);
    EXPECT_STREQ(str, "Unknown");

    rbusProperty_Release(property);
}

/***********************************************************************

  Unit Tests for eventWanStateSubHandler

 ***********************************************************************/

TEST (eventWanStateSubHandlerTest1, eventWanStateSubHandler_Subscribe)
{
    rbusHandle_t testHandle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_SUBSCRIBE;
    const char *eventName = WANMGR_WAN_STATE_EVENT;
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 0;

    rbusError_t ret = eventWanStateSubHandler(testHandle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
    EXPECT_FALSE(autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 1u);
}

TEST (eventWanStateSubHandlerTest2, eventWanStateSubHandler_Unsubscribe)
{
    rbusHandle_t testHandle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = WANMGR_WAN_STATE_EVENT;
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 1;

    rbusError_t ret = eventWanStateSubHandler(testHandle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
    EXPECT_FALSE(autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 0u);
}

TEST (eventWanStateSubHandlerTest3, eventWanStateSubHandler_UnsubscribeAtZero)
{
    rbusHandle_t testHandle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = WANMGR_WAN_STATE_EVENT;
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 0;

    rbusError_t ret = eventWanStateSubHandler(testHandle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
    EXPECT_FALSE(autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 0u);
}

TEST (eventWanStateSubHandlerTest4, eventWanStateSubHandler_MultipleSubscribes)
{
    rbusHandle_t testHandle = NULL;
    const char *eventName = WANMGR_WAN_STATE_EVENT;
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 0;

    eventWanStateSubHandler(testHandle, RBUS_EVENT_ACTION_SUBSCRIBE, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 1u);

    eventWanStateSubHandler(testHandle, RBUS_EVENT_ACTION_SUBSCRIBE, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 2u);

    eventWanStateSubHandler(testHandle, RBUS_EVENT_ACTION_UNSUBSCRIBE, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 1u);
}

TEST (eventWanStateSubHandlerTest5, eventWanStateSubHandler_UnexpectedEventName)
{
    rbusHandle_t testHandle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_SUBSCRIBE;
    const char *eventName = "Device.Some.Other.Event";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 0;

    rbusError_t ret = eventWanStateSubHandler(testHandle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
    EXPECT_FALSE(autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 0u);
}

/***********************************************************************

  Unit Tests for publishWanStateEvent

 ***********************************************************************/

TEST (publishWanStateEventTest1, publishWanStateEvent_NullHandle)
{
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 1;

    /* Should skip publish when handle is NULL */
    publishWanStateEvent("Manageable");

    /* Verify state was still cached via getWanStateHandler */
    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);
    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusValue_t value = rbusProperty_GetValue(property);
    const char *str = rbusValue_GetString(value, NULL);
    EXPECT_STREQ(str, "Manageable");

    rbusProperty_Release(property);
    handle = savedHandle;
}

TEST (publishWanStateEventTest2, publishWanStateEvent_ZeroSubscribers)
{
    gWanStateSubscribersCount = 0;

    /* Should skip publish when there are no subscribers */
    publishWanStateEvent("Serviceable");

    /* Verify state was still cached */
    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);
    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusValue_t value = rbusProperty_GetValue(property);
    const char *str = rbusValue_GetString(value, NULL);
    EXPECT_STREQ(str, "Serviceable");

    rbusProperty_Release(property);
}

TEST (publishWanStateEventTest3, publishWanStateEvent_NullInput)
{
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 0;

    publishWanStateEvent(NULL);

    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);
    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusValue_t value = rbusProperty_GetValue(property);
    const char *str = rbusValue_GetString(value, NULL);
    EXPECT_STREQ(str, "Unknown");

    rbusProperty_Release(property);
    handle = savedHandle;
}

TEST (publishWanStateEventTest4, publishWanStateEvent_EmptyString)
{
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 0;

    publishWanStateEvent("");

    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);
    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusValue_t value = rbusProperty_GetValue(property);
    const char *str = rbusValue_GetString(value, NULL);
    EXPECT_STREQ(str, "Unknown");

    rbusProperty_Release(property);
    handle = savedHandle;
}

TEST_F (PandMSSPTestFixture, publishWanStateEvent_SuccessfulPublish)
{
    handle = (rbusHandle_t)0x1;  /* Non-NULL fake handle */
    gWanStateSubscribersCount = 1;

    EXPECT_CALL(*g_rbusMock, rbusEvent_Publish(_, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_SUCCESS));

    publishWanStateEvent("Manageable");

    /* Verify cached state */
    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);
    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusValue_t value = rbusProperty_GetValue(property);
    const char *str = rbusValue_GetString(value, NULL);
    EXPECT_STREQ(str, "Manageable");

    rbusProperty_Release(property);
    handle = NULL;
}

TEST_F (PandMSSPTestFixture, publishWanStateEvent_PublishFailure)
{
    handle = (rbusHandle_t)0x1;  /* Non-NULL fake handle */
    gWanStateSubscribersCount = 1;

    EXPECT_CALL(*g_rbusMock, rbusEvent_Publish(_, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_BUS_ERROR));

    publishWanStateEvent("Serviceable");

    /* State should still be cached even on publish failure */
    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);
    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusValue_t value = rbusProperty_GetValue(property);
    const char *str = rbusValue_GetString(value, NULL);
    EXPECT_STREQ(str, "Serviceable");

    rbusProperty_Release(property);
    handle = NULL;
}

/***********************************************************************

  Unit Tests for getWanStateHandler

 ***********************************************************************/

TEST_F (PandMSSPTestFixture, getWanStateHandler_ReturnsDefaultState)
{
    rbusHandle_t testHandle = NULL;
    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);

    rbusError_t ret = getWanStateHandler(testHandle, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusProperty_Release(property);
}

TEST_F (PandMSSPTestFixture, getWanStateHandler_ReturnsUpdatedState)
{
    /* Set WAN state by calling publishWanStateEvent with no subscribers to
       update the cached g_wanState without actually publishing */
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 0;
    publishWanStateEvent("Manageable");
    handle = savedHandle;

    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);

    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusProperty_Release(property);
}

TEST_F (PandMSSPTestFixture, getWanStateHandler_ReturnsUnknownForNullInput)
{
    /* publishWanStateEvent with NULL should set state to "Unknown" */
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 0;
    publishWanStateEvent(NULL);
    handle = savedHandle;

    rbusProperty_t property;
    rbusProperty_Init(&property, WANMGR_WAN_STATE_EVENT, NULL);

    rbusError_t ret = getWanStateHandler(NULL, property, NULL);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);

    rbusProperty_Release(property);
}

/***********************************************************************

  Unit Tests for eventWanStateSubHandler

 ***********************************************************************/

TEST (eventWanStateSubHandlerTest1, eventWanStateSubHandler_Subscribe)
{
    rbusHandle_t testHandle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_SUBSCRIBE;
    const char *eventName = WANMGR_WAN_STATE_EVENT;
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 0;

    rbusError_t ret = eventWanStateSubHandler(testHandle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
    EXPECT_FALSE(autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 1u);
}

TEST (eventWanStateSubHandlerTest2, eventWanStateSubHandler_Unsubscribe)
{
    rbusHandle_t testHandle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = WANMGR_WAN_STATE_EVENT;
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 1;

    rbusError_t ret = eventWanStateSubHandler(testHandle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
    EXPECT_FALSE(autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 0u);
}

TEST (eventWanStateSubHandlerTest3, eventWanStateSubHandler_UnsubscribeAtZero)
{
    rbusHandle_t testHandle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_UNSUBSCRIBE;
    const char *eventName = WANMGR_WAN_STATE_EVENT;
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 0;

    rbusError_t ret = eventWanStateSubHandler(testHandle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
    EXPECT_FALSE(autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 0u);
}

TEST (eventWanStateSubHandlerTest4, eventWanStateSubHandler_MultipleSubscribes)
{
    rbusHandle_t testHandle = NULL;
    const char *eventName = WANMGR_WAN_STATE_EVENT;
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 0;

    eventWanStateSubHandler(testHandle, RBUS_EVENT_ACTION_SUBSCRIBE, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 1u);

    eventWanStateSubHandler(testHandle, RBUS_EVENT_ACTION_SUBSCRIBE, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 2u);

    eventWanStateSubHandler(testHandle, RBUS_EVENT_ACTION_UNSUBSCRIBE, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 1u);
}

TEST (eventWanStateSubHandlerTest5, eventWanStateSubHandler_UnexpectedEventName)
{
    rbusHandle_t testHandle = NULL;
    rbusEventSubAction_t action = RBUS_EVENT_ACTION_SUBSCRIBE;
    const char *eventName = "Device.Some.Other.Event";
    rbusFilter_t filter = NULL;
    int32_t interval = 1000;
    bool autoPublish = true;
    gWanStateSubscribersCount = 0;

    rbusError_t ret = eventWanStateSubHandler(testHandle, action, eventName, filter, interval, &autoPublish);
    EXPECT_EQ(ret, RBUS_ERROR_SUCCESS);
    EXPECT_FALSE(autoPublish);
    EXPECT_EQ(gWanStateSubscribersCount, 0u);
}

/***********************************************************************

  Unit Tests for publishWanStateEvent

 ***********************************************************************/

TEST_F (PandMSSPTestFixture, publishWanStateEvent_NullHandle)
{
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 1;

    /* rbusEvent_Publish should NOT be called when handle is NULL */
    EXPECT_CALL(*g_rbusMock, rbusEvent_Publish(_, _))
        .Times(0);

    publishWanStateEvent("Manageable");

    handle = savedHandle;
}

TEST_F (PandMSSPTestFixture, publishWanStateEvent_ZeroSubscribers)
{
    handle = (rbusHandle_t)0x1;
    gWanStateSubscribersCount = 0;

    /* rbusEvent_Publish should NOT be called when there are no subscribers */
    EXPECT_CALL(*g_rbusMock, rbusEvent_Publish(_, _))
        .Times(0);

    publishWanStateEvent("Serviceable");

    handle = NULL;
}

TEST_F (PandMSSPTestFixture, publishWanStateEvent_NullInput)
{
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 0;

    /* Should not crash with NULL input; sets state to "Unknown" internally */
    EXPECT_CALL(*g_rbusMock, rbusEvent_Publish(_, _))
        .Times(0);

    publishWanStateEvent(NULL);

    handle = savedHandle;
}

TEST_F (PandMSSPTestFixture, publishWanStateEvent_EmptyString)
{
    rbusHandle_t savedHandle = handle;
    handle = NULL;
    gWanStateSubscribersCount = 0;

    /* Should not crash with empty string; sets state to "Unknown" internally */
    EXPECT_CALL(*g_rbusMock, rbusEvent_Publish(_, _))
        .Times(0);

    publishWanStateEvent("");

    handle = savedHandle;
}

TEST_F (PandMSSPTestFixture, publishWanStateEvent_SuccessfulPublish)
{
    handle = (rbusHandle_t)0x1;  /* Non-NULL fake handle */
    gWanStateSubscribersCount = 1;

    EXPECT_CALL(*g_rbusMock, rbusEvent_Publish(_, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_SUCCESS));

    publishWanStateEvent("Manageable");

    handle = NULL;
}

TEST_F (PandMSSPTestFixture, publishWanStateEvent_PublishFailure)
{
    handle = (rbusHandle_t)0x1;  /* Non-NULL fake handle */
    gWanStateSubscribersCount = 1;

    EXPECT_CALL(*g_rbusMock, rbusEvent_Publish(_, _))
        .Times(1)
        .WillOnce(Return(RBUS_ERROR_BUS_ERROR));

    /* Should not crash even when publish fails */
    publishWanStateEvent("Serviceable");

    handle = NULL;
}
