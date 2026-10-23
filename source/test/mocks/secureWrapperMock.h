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

#ifndef SECURE_WRAPPER_MOCK_H
#define SECURE_WRAPPER_MOCK_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

/* The real v_secure_system() is variadic; the shim in secureWrapperMock.cpp
   formats the command into a single string before forwarding here. */
class secure_wrapperInterface {
public:
        virtual ~secure_wrapperInterface() {}
        virtual int v_secure_system(const char* command) = 0;
};

class secure_wrapperMock : public secure_wrapperInterface {
public:
        virtual ~secure_wrapperMock() {}
        MOCK_METHOD1(v_secure_system, int(const char* command));
};

extern secure_wrapperMock* g_secure_wrapperMock;

#endif /* SECURE_WRAPPER_MOCK_H */