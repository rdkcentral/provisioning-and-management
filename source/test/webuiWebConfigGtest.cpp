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

extern "C"
{
    #include "webui_doc.h"
    #include "webui_webconfig_dml.h"
    #include "webui_webconfig_apis.h"
    enum WEBUI_MODE getModeFromParam(char *mode);
    int process_webui_params( webui_param_t *e, msgpack_object_map *map );
    int process_webui_doc( webui_doc_t *doc,int num, ... );
}

using namespace testing;
extern SyscfgMock * g_syscfgMock;
extern SafecLibMock * g_safecLibMock;
extern AnscMemoryMock * g_anscMemoryMock;
extern base64Mock *g_base64Mock;
extern msgpackMock *g_msgpackMock;
extern helpersMock * g_helpersMock;
extern webconfigFwMock *g_webconfigFwMock;
extern parodusInterfaceMock *g_parodusInterfaceMock;
extern cosaDeviceInfoApisMock * g_cosaDeviceInfoApisMock;

/* webui_webconfig_apis.c */
TEST_F(PandMSSPTestFixture, getModeFromParam_Enable)
{
    char mode[] = "Enable";
    int result;

    result = getModeFromParam(mode);
    EXPECT_EQ(result, 1);
}

TEST_F(PandMSSPTestFixture, getModeFromParam_Disable)
{
    char mode[] = "Disable";
    int result;

    result = getModeFromParam(mode);
    EXPECT_EQ(result, 0);
}

TEST_F(PandMSSPTestFixture, getModeFromParam_MSOonly)
{
    char mode[] = "MSOonly";
    int result;

    result = getModeFromParam(mode);
    EXPECT_EQ(result, 2);
}

TEST_F(PandMSSPTestFixture, getModeFromParam_Invalid)
{
    char mode[] = "Invalid";
    int result;

    result = getModeFromParam(mode);
    EXPECT_EQ(result, -1);
}

TEST_F(PandMSSPTestFixture, webui_webconfig_handle_blob_CASE1)
{
    webui_param_t param;
    param.mode = strdup("Invalid");

    int result;

    result = webui_webconfig_handle_blob(&param);
    EXPECT_EQ(result, ANSC_STATUS_FAILURE);
}

TEST_F(PandMSSPTestFixture, webui_webconfig_handle_blob_CASE2)
{
    webui_param_t param;
    param.mode = strdup("Enable");

    int result;

    EXPECT_CALL(*g_syscfgMock, syscfg_set_nns_u_commit(StrEq("WebUIEnable"), _)).WillOnce(Return(-1));

    result = webui_webconfig_handle_blob(&param);
    EXPECT_EQ(result, SYSCFG_FAILURE);
}

TEST_F(PandMSSPTestFixture, webui_webconfig_handle_blob_CASE3)
{
    webui_param_t param;
    param.mode = strdup("Disable");
    int result;

    EXPECT_CALL(*g_syscfgMock, syscfg_set_nns_u_commit(StrEq("WebUIEnable"), _)).WillOnce(Return(0));
    EXPECT_CALL(*g_cosaDeviceInfoApisMock, CosaDmlDiSet_DisableRemoteManagement()).Times(1);

    result = webui_webconfig_handle_blob(&param);
    EXPECT_EQ(result, BLOB_EXEC_SUCCESS);
}

TEST_F(PandMSSPTestFixture, webui_webconfig_process_request_CASE1)
{
    webui_doc_t webui;
    webui.subdoc_name = strdup("webui");
    webui.param = (webui_param_t *)malloc(sizeof(webui_param_t));
    webui.param->mode = strdup("Invalid");

    EXPECT_CALL(*g_safecLibMock, _memset_s_chk(_,_,_,_,_)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(*g_safecLibMock, _strcmp_s_chk(_, _, StrEq(WEBUI_WEBCONFIG_SUBDOC_NAME), _, _, _))
        .WillOnce(DoAll(
            testing::SetArgPointee<3>(-1),
            Return(1)
        ));

    pErr execRetVal = webui_webconfig_process_request(&webui);
    EXPECT_EQ(execRetVal->ErrorCode, SUBDOC_NOT_SUPPORTED);
}

TEST_F(PandMSSPTestFixture, webui_webconfig_process_request_CASE2)
{
    EXPECT_CALL(*g_safecLibMock, _memset_s_chk(_,_,_,_,_)).Times(1).WillOnce(Return(0));
    pErr execRetVal = webui_webconfig_process_request(NULL);
    EXPECT_EQ(execRetVal->ErrorCode, NULL_BLOB_EXEC_POINTER);
}

TEST_F(PandMSSPTestFixture, webui_webconfig_process_request_CASE3)
{
    webui_doc_t webui;
    webui.subdoc_name = strdup("webui");
    webui.param = (webui_param_t *)malloc(sizeof(webui_param_t));
    webui.param->mode = strdup("Enable");

    EXPECT_CALL(*g_safecLibMock, _memset_s_chk(_,_,_,_,_)).Times(1).WillOnce(Return(0));

    EXPECT_CALL(*g_safecLibMock, _strcmp_s_chk(_, _, StrEq(WEBUI_WEBCONFIG_SUBDOC_NAME), _, _, _))
        .WillOnce(DoAll(
            testing::SetArgPointee<3>(0),
            Return(0)
        ));
    EXPECT_CALL(*g_syscfgMock, syscfg_set_nns_u_commit(StrEq("WebUIEnable"), _)).WillOnce(Return(1));

    pErr execRetVal = webui_webconfig_process_request(&webui);
    EXPECT_EQ(execRetVal->ErrorCode, SYSCFG_FAILURE);
}

TEST_F(PandMSSPTestFixture, webui_webconfig_process_request_CASE4)
{
    webui_doc_t webui;
    webui.subdoc_name = strdup("webui");
    webui.param = (webui_param_t *)malloc(sizeof(webui_param_t));
    webui.param->mode = strdup("Enable");

    EXPECT_CALL(*g_safecLibMock, _memset_s_chk(_,_,_,_,_)).Times(1).WillOnce(Return(0));

    EXPECT_CALL(*g_safecLibMock, _strcmp_s_chk(_, _, StrEq(WEBUI_WEBCONFIG_SUBDOC_NAME), _, _, _))
        .WillOnce(DoAll(
            testing::SetArgPointee<3>(0),
            Return(0)
        ));
    EXPECT_CALL(*g_syscfgMock, syscfg_set_nns_u_commit(StrEq("WebUIEnable"), _)).WillOnce(Return(0));

    pErr execRetVal = webui_webconfig_process_request(&webui);
    EXPECT_EQ(execRetVal->ErrorCode, BLOB_EXEC_SUCCESS);
}

TEST_F(PandMSSPTestFixture, webui_webconfig_rollback)
{
    int result;

    result = webui_webconfig_rollback();
    EXPECT_EQ(result, 0);
}

TEST_F(PandMSSPTestFixture, webui_webconfig_free_resources)
{
    execData blob_exec_data;
    webui_doc_t doc;
    doc.subdoc_name = strdup("webui");
    doc.param = (webui_param_t *)malloc(sizeof(webui_param_t));
    doc.param->mode = strdup("Enable");

    blob_exec_data.user_data = &doc;

    EXPECT_CALL(*g_anscMemoryMock, AnscFreeMemoryOrig(_))
               .WillRepeatedly(Return());

    webui_webconfig_free_resources(&blob_exec_data);
}

/* webui_webconfig_dml.c */
TEST_F(PandMSSPTestFixture, WebUIconfig_GetParamStringValue_CASE1) {
    char ParamName[] = "Data";
    char pValue[10];
    ULONG pUlSize = 10;
    ULONG result;

    result = WebUIconfig_GetParamStringValue(NULL, ParamName, pValue, &pUlSize);
    EXPECT_EQ(result, 0);
}

TEST_F(PandMSSPTestFixture, WebUIconfig_GetParamStringValue_CASE2) {
    char ParamName[] = "Invalid";
    char pValue[10];
    ULONG pUlSize = 10;
    ULONG result;

    result = WebUIconfig_GetParamStringValue(NULL, ParamName, pValue, &pUlSize);
    EXPECT_EQ(result, -1);
}

TEST_F(PandMSSPTestFixture, WebUIconfig_SetParamStringValue_CASE1) {
    char ParamName[] = "Invalid";
    BOOL result;

    result = WebUIconfig_SetParamStringValue(NULL, ParamName, NULL);
    EXPECT_EQ(result, FALSE);
}

TEST_F(PandMSSPTestFixture, WebUIconfig_SetParamStringValue_CASE2) {
    char ParamName[] = "Data";
    char pString[] = "YmFzZTY0IGVuY29kZWQgZGF0YQ==";
    webui_doc_t *doc = (webui_doc_t *)malloc(sizeof(webui_doc_t));
    BOOL result;

    EXPECT_CALL(*g_base64Mock, b64_get_decoded_buffer_size(_)).WillOnce(Return(16));
    EXPECT_CALL(*g_base64Mock, b64_decode(_, _, _)).WillOnce(Return(16));
    EXPECT_CALL(*g_parodusInterfaceMock, getCurrentTime(_)).Times(2);
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_init(_, _)).Times(1);
    EXPECT_CALL(*g_msgpackMock, msgpack_unpack(_, _, _, _, _)).WillOnce(Return(MSGPACK_UNPACK_SUCCESS));
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_destroy(_)).Times(1);
    EXPECT_CALL(*g_helpersMock, helper_convert(_, _, _, _, _, _, _, _)).Times(1).WillOnce(Return(doc));
    EXPECT_CALL(*g_webconfigFwMock, PushBlobRequest(_)).Times(1);

    result = WebUIconfig_SetParamStringValue(NULL, ParamName, pString);
    EXPECT_EQ(result, TRUE);
    delete doc;
    doc = nullptr;
}

TEST_F(PandMSSPTestFixture, WebUIconfig_SetParamStringValue_CASE3) {
    char ParamName[] = "Data";
    char pString[] = "YmFzZTY0IGVuY29kZWQgZGF0YQ==";
    BOOL result;

    EXPECT_CALL(*g_base64Mock, b64_get_decoded_buffer_size(_)).WillOnce(Return(16));
    EXPECT_CALL(*g_base64Mock, b64_decode(_, _, _)).WillOnce(Return(16));
    EXPECT_CALL(*g_parodusInterfaceMock, getCurrentTime(_)).Times(2);
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_init(_, _)).Times(1);
    EXPECT_CALL(*g_msgpackMock, msgpack_unpack(_, _, _, _, _)).WillOnce(Return(MSGPACK_UNPACK_SUCCESS));
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_destroy(_)).Times(1);
    EXPECT_CALL(*g_helpersMock, helper_convert(_, _, _, _, _, _, _, _)).Times(1).WillOnce(Return(nullptr));

    result = WebUIconfig_SetParamStringValue(NULL, ParamName, pString);
    EXPECT_EQ(result, TRUE);
}

TEST_F(PandMSSPTestFixture, WebUIconfig_SetParamStringValue_CASE4) {
    char ParamName[] = "Data";
    char pString[] = "YmFzZTY0IGVuY29kZWQgZGF0YQ==";
    BOOL result;

    EXPECT_CALL(*g_base64Mock, b64_get_decoded_buffer_size(_)).WillOnce(Return(16));
    EXPECT_CALL(*g_base64Mock, b64_decode(_, _, _)).WillOnce(Return(16));
    EXPECT_CALL(*g_parodusInterfaceMock, getCurrentTime(_)).Times(2);
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_init(_, _)).Times(1);
    EXPECT_CALL(*g_msgpackMock, msgpack_unpack(_, _, _, _, _)).WillOnce(Return(MSGPACK_UNPACK_EXTRA_BYTES));
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_destroy(_)).Times(1);

    result = WebUIconfig_SetParamStringValue(NULL, ParamName, pString);
    EXPECT_EQ(result, FALSE);
}

TEST_F(PandMSSPTestFixture, WebUIconfig_SetParamStringValue_CASE5) {
    char ParamName[] = "Data";
    char pString[] = "YmFzZTY0IGVuY29kZWQgZGF0YQ==";
    BOOL result;

    EXPECT_CALL(*g_base64Mock, b64_get_decoded_buffer_size(_)).WillOnce(Return(16));
    EXPECT_CALL(*g_base64Mock, b64_decode(_, _, _)).WillOnce(Return(16));
    EXPECT_CALL(*g_parodusInterfaceMock, getCurrentTime(_)).Times(2);
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_init(_, _)).Times(1);
    EXPECT_CALL(*g_msgpackMock, msgpack_unpack(_, _, _, _, _)).WillOnce(Return(MSGPACK_UNPACK_CONTINUE));
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_destroy(_)).Times(1);

    result = WebUIconfig_SetParamStringValue(NULL, ParamName, pString);
    EXPECT_EQ(result, FALSE);
}

TEST_F(PandMSSPTestFixture, WebUIconfig_SetParamStringValue_CASE6) {
    char ParamName[] = "Data";
    char pString[] = "YmFzZTY0IGVuY29kZWQgZGF0YQ==";
    BOOL result;

    EXPECT_CALL(*g_base64Mock, b64_get_decoded_buffer_size(_)).WillOnce(Return(16));
    EXPECT_CALL(*g_base64Mock, b64_decode(_, _, _)).WillOnce(Return(16));
    EXPECT_CALL(*g_parodusInterfaceMock, getCurrentTime(_)).Times(2);
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_init(_, _)).Times(1);
    EXPECT_CALL(*g_msgpackMock, msgpack_unpack(_, _, _, _, _)).WillOnce(Return(MSGPACK_UNPACK_PARSE_ERROR));
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_destroy(_)).Times(1);

    result = WebUIconfig_SetParamStringValue(NULL, ParamName, pString);
    EXPECT_EQ(result, FALSE);
}

TEST_F(PandMSSPTestFixture, WebUIconfig_SetParamStringValue_CASE7) {
    char ParamName[] = "Data";
    char pString[] = "YmFzZTY0IGVuY29kZWQgZGF0YQ==";
    BOOL result;

    EXPECT_CALL(*g_base64Mock, b64_get_decoded_buffer_size(_)).WillOnce(Return(16));
    EXPECT_CALL(*g_base64Mock, b64_decode(_, _, _)).WillOnce(Return(16));
    EXPECT_CALL(*g_parodusInterfaceMock, getCurrentTime(_)).Times(2);
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_init(_, _)).Times(1);
    EXPECT_CALL(*g_msgpackMock, msgpack_unpack(_, _, _, _, _)).WillOnce(Return(MSGPACK_UNPACK_NOMEM_ERROR));
    EXPECT_CALL(*g_msgpackMock, msgpack_zone_destroy(_)).Times(1);

    result = WebUIconfig_SetParamStringValue(NULL, ParamName, pString);
    EXPECT_EQ(result, FALSE);
}

/* webui_doc.c */
TEST_F(PandMSSPTestFixture, process_webui_params_CASE1) {
    webui_param_t e;
    msgpack_object_map map;
    msgpack_object_kv p;
    int result;

    p.key.type = MSGPACK_OBJECT_STR;
    p.val.type = MSGPACK_OBJECT_STR;
    p.key.via.str.ptr = "Mode";
    p.key.via.str.size = strlen("Mode");
    p.val.via.str.ptr = "Enable";
    p.val.via.str.size = strlen("Enable");

    map.ptr = &p;

    result = process_webui_params(&e, &map);
    EXPECT_EQ(result, 0);
}

TEST_F(PandMSSPTestFixture, process_webui_doc_CASE1) {
    webui_doc_t doc;
    msgpack_object obj;
    msgpack_object_map mapobj;
    msgpack_object obj1, obj2, obj3;
    msgpack_object_kv kvpair;
    int result;

    obj.type = MSGPACK_OBJECT_MAP;
    obj.via.map.size = 1;

    obj1.type = MSGPACK_OBJECT_STR;
    obj1.via.str.ptr = "webui";
    obj1.via.str.size = strlen("webui");

    obj2.type = MSGPACK_OBJECT_POSITIVE_INTEGER;
    obj2.via.u64 = 123;

    obj3.type = MSGPACK_OBJECT_POSITIVE_INTEGER;
    obj3.via.u64 = 246178;

    kvpair.key = obj1;
    kvpair.val = obj2;

    mapobj.ptr = &kvpair;
    mapobj.size = 1;

    obj.via.map.ptr = &kvpair;

    result = process_webui_doc(&doc, 3, &obj, &obj1, &obj2, &obj3);
    EXPECT_EQ(result, 0);
}

TEST_F(PandMSSPTestFixture, process_webui_doc_CASE2) {
    webui_doc_t doc;
    msgpack_object obj;
    msgpack_object_map mapobj;
    msgpack_object obj1, obj2, obj3;
    msgpack_object_kv kvpair;
    int result;

    obj.type = MSGPACK_OBJECT_MAP;
    obj.via.map.size = 1;

    obj1.type = MSGPACK_OBJECT_POSITIVE_INTEGER;
    obj2.via.u64 = 1;

    obj2.type = MSGPACK_OBJECT_POSITIVE_INTEGER;
    obj2.via.u64 = 123;

    obj3.type = MSGPACK_OBJECT_POSITIVE_INTEGER;
    obj3.via.u64 = 246178;

    kvpair.key = obj1;
    kvpair.val = obj2;

    mapobj.ptr = &kvpair;
    mapobj.size = 1;

    obj.via.map.ptr = &kvpair;

    result = process_webui_doc(&doc, 3, &obj, &obj1, &obj2, &obj3);
    EXPECT_EQ(result, -1);
}

TEST_F(PandMSSPTestFixture, webui_doc_strerror) {
    int errnum = 1;
    const char *result;

    result = webui_doc_strerror(errnum);
    EXPECT_STREQ(result, "Out of memory.");
}
