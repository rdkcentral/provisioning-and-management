/*************************************************************************
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
**************************************************************************/

#ifndef COSA_DEVICEINFO_APIS_MOCK_H
#define COSA_DEVICEINFO_APIS_MOCK_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#define ANSC_STATUS unsigned long

class cosaDeviceInfoApis
{
    public:
        virtual ~cosaDeviceInfoApis() {}
        virtual void CosaDmlDiSet_DisableRemoteManagement() = 0;
        virtual ANSC_STATUS CosaDmlDiSetEnableMoCAforXi5Flag (void*, bool*, bool*) = 0;
};

class cosaDeviceInfoApisMock : public cosaDeviceInfoApis
{
    public:
        virtual ~cosaDeviceInfoApisMock() {}
        MOCK_METHOD0(CosaDmlDiSet_DisableRemoteManagement, void());
        MOCK_METHOD3(CosaDmlDiSetEnableMoCAforXi5Flag, ANSC_STATUS(void*, bool*, bool*));
};
#endif