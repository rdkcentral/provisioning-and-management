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

#include "rbushandler_mock.h"

using namespace std;

extern rbusHandlerMock * g_rbusHandlerMock;

extern "C" ULONG DeviceInfo_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    if (!g_rbusHandlerMock)
    {
        return -1;
    }
    return g_rbusHandlerMock->DeviceInfo_GetParamStringValue(hInsContext, ParamName, pValue, pUlSize);
}

extern "C" void configureIpv6Route(uint32_t DeviceMode)
{
   if (!g_rbusHandlerMock)
   {
       return;
   }
   return g_rbusHandlerMock->configureIpv6Route(DeviceMode);
}