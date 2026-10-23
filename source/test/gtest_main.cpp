/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2021 RDK Management
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
*/ 

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define GTEST_DEFAULT_RESULT_FILEPATH "/tmp/Gtest_Report/"
#define GTEST_DEFAULT_RESULT_STEM "CcspPandMSsp_gtest"
#define GTEST_REPORT_FILEPATH_SIZE 256

class PandMSSPEnvironment : public ::testing::Environment
{
    public:
        virtual ~PandMSSPEnvironment()
        {
        }
        virtual void SetUp()
        {
            printf("%s Environment\n", __func__);
        }
        virtual void TearDown()
        {
            printf("%s Environment\n", __func__);
        }
};

GTEST_API_ int main(int argc, char *argv[])
{
    char testresults_fullfilepath[GTEST_REPORT_FILEPATH_SIZE];
    char reportname[GTEST_REPORT_FILEPATH_SIZE];

    memset( testresults_fullfilepath, 0, GTEST_REPORT_FILEPATH_SIZE );
    memset( reportname, 0, GTEST_REPORT_FILEPATH_SIZE );

    /* Derive the report name from the binary name so multiple test binaries
       don't overwrite each other's JSON report in the shared directory. */
    const char* base = (argc > 0 && argv[0] != NULL && argv[0][0] != '\0') ? argv[0] : GTEST_DEFAULT_RESULT_STEM;
    const char* slash = strrchr(base, '/');
    if (slash != NULL)
    {
        base = slash + 1;
    }
    snprintf( reportname, sizeof(reportname), "%s", base );
    size_t namelen = strlen(reportname);
    if (namelen > 4 && strcmp(reportname + namelen - 4, ".bin") == 0)
    {
        reportname[namelen - 4] = '\0';
    }

    snprintf( testresults_fullfilepath, sizeof(testresults_fullfilepath), "json:%s%s_report.json", GTEST_DEFAULT_RESULT_FILEPATH, reportname );
    ::testing::GTEST_FLAG(output) = testresults_fullfilepath;
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

