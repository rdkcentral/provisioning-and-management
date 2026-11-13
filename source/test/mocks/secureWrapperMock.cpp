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

#include "secureWrapperMock.h"
#include <cstdarg>
#include <cstdio>

secure_wrapperMock* g_secure_wrapperMock = nullptr;

extern "C" int v_secure_system(const char* format, ...)
{
    char command[1024] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(command, sizeof(command), format, args);
    va_end(args);

    if (g_secure_wrapperMock)
    {
        return g_secure_wrapperMock->v_secure_system(command);
    }
    return 0;
}