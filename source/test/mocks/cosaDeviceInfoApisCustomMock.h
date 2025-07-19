/*************************************************************************
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
**************************************************************************/

#ifndef COSA_DEVICEINFO_APIS_CUSTOM_MOCK_H
#define COSA_DEVICEINFO_APIS_CUSTOM_MOCK_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define ANSC_STATUS unsigned long 

class cosaDeviceInfoApisCustom
{
    public:
        virtual ~cosaDeviceInfoApisCustom() {}
        virtual ANSC_STATUS CosaDmlGetCloudUIReachableStatus (char*, bool*) = 0;
        virtual ANSC_STATUS CosaDmlGetCaptivePortalEnable (bool *) = 0;
        virtual ANSC_STATUS CosaDmlSetCaptivePortalEnable (bool) = 0;
};

class cosaDeviceInfoApisCustomMock : public cosaDeviceInfoApisCustom
{
    public:
        virtual ~cosaDeviceInfoApisCustomMock() {}
        MOCK_METHOD2(CosaDmlGetCloudUIReachableStatus, ANSC_STATUS(char*, bool*));
        MOCK_METHOD1(CosaDmlGetCaptivePortalEnable, ANSC_STATUS(bool*));
        MOCK_METHOD1(CosaDmlSetCaptivePortalEnable, ANSC_STATUS(bool));
};
#endif