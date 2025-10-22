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


extern "C"
{
	ANSC_HANDLE
X_CISCO_COM_StaticIPv4Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

#include "mocks/cosa_routing_dml_mock.h"
}

extern SafecLibMock * g_safecLibMock;

int MockSprintf_s_chk(char* buf, size_t size, size_t n, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vsnprintf(buf, size, format, args);
    va_end(args);
    return ret;
}

TEST_F(PandMSSPTestFixture, X_CISCO_COM_StaticIPv4Forwarding_AddEntry_Success) {
    
    PCOSA_DATAMODEL_ROUTING pRoutingObj;
    ULONG insNumber;
    pRoutingObj = (PCOSA_DATAMODEL_ROUTING)malloc(sizeof(COSA_DATAMODEL_ROUTING));
        ASSERT_NE(pRoutingObj, nullptr);
        memset(pRoutingObj, 0, sizeof(COSA_DATAMODEL_ROUTING));

        pRoutingObj->NextInstanceNumber = 1;

        //pRoutingObj->StaticRoute = nullptr;

        memset(&pRoutingObj->StaticRoute, 0, sizeof(SLIST_HEADER));

        g_pCosaBEManager = (PCOSA_BACKEND_MANAGER_OBJECT)malloc(sizeof(COSA_BACKEND_MANAGER_OBJECT));
        ASSERT_NE(g_pCosaBEManager, nullptr);
        g_pCosaBEManager->hRouting = pRoutingObj;

        EXPECT_CALL(*g_safecLibMock, _sprintf_s_chk(_, _, _, testing::HasSubstr("StaticRoute_")))
        .WillOnce(Invoke(MockSprintf_s_chk));

        ANSC_HANDLE handle = X_CISCO_COM_StaticIPv4Forwarding_AddEntry(nullptr, &insNumber);

    ASSERT_NE(handle, nullptr);
    auto pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)handle;
    EXPECT_EQ(pCxtLink->InstanceNumber, 1);
    EXPECT_EQ(insNumber, 1);

    auto pEntry = (PCOSA_DML_STATICROUTE_CFG)pCxtLink->hContext;
    ASSERT_NE(pEntry, nullptr);
    char pEntry_Name[64]={0};
    snprintf(pEntry_Name, 64, "StaticRoute_%p_%lu", pEntry, pCxtLink->InstanceNumber);
    //printf("static route value = %s, actual=%s", pEntry_Name,pEntry->Name);
    EXPECT_TRUE(strncmp(pEntry->Name, pEntry_Name, strlen(pEntry_Name)) == 0);
}
