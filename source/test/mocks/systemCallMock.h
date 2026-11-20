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

#ifndef SYSTEM_CALL_MOCK_H
#define SYSTEM_CALL_MOCK_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class systemCallInterface {
public:
        virtual ~systemCallInterface() {}
        virtual int system(const char* command) = 0;
};

class systemCallMock : public systemCallInterface {
public:
        virtual ~systemCallMock() {}
        MOCK_METHOD1(system, int(const char* command));
};

extern systemCallMock* g_systemCallMock;

#endif /* SYSTEM_CALL_MOCK_H */

