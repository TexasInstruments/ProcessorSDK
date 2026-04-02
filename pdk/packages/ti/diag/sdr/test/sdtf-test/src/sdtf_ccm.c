/*
 * SDTF CCM
 *
 * Software Diagnostics Reference Test for CCM module
 *
 *  Copyright (c) Texas Instruments Incorporated 2019-2020
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>

#include <sdr_ccm.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_ccm.h"
#include "sdtf_test.h"

/*********************************************************************
 * @fn      SDTF_runCCMSelfTest
 *
 * @brief   Execute CCM Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMSelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_CCM_ErrorStatus_t errorStatus;

    SDTF_printf("\n CCM self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_CCM_selfTest(SDR_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK,
                              SDR_CCM_SELFTEST_TYPE_NORMAL,
                              10000000,
                              &errorStatus);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CCM self test failed");
        SDTF_printf("\n Error status cmp error %d, self test error %d, Self test error type %d",
                    errorStatus.compareErrorFlag,
                    errorStatus.selfTestErrorFlag,
                    errorStatus.selfTestErrorTypeFlag);
        retVal = -1;
    } else {
        SDTF_printf("\n CCM Self Test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runCCMSelfTestErrorForce
 *
 * @brief   Execute CCM Self test with error force
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMSelfTestErrorForce(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_CCM_ErrorStatus_t errorStatus;

    SDTF_printf("\n CCM self test with error forcing: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_CCM_selfTest(SDR_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK,
                              SDR_CCM_SELFTEST_TYPE_ERR_FORCING,
                              10000000,
                              &errorStatus);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CCM self test with error forcing failed");
        SDTF_printf("\n Error status cmp error %d, self test error %d, Self test error type %d",
                    errorStatus.compareErrorFlag,
                    errorStatus.selfTestErrorFlag,
                    errorStatus.selfTestErrorTypeFlag);
        retVal = -1;
    } else {
        SDTF_printf("\n CCM Self Test with error forcing complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runCCMInjectError
 *
 * @brief   Execute CCM Inject Error test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMInjectError(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDTF_printf("\n CCM inject  error: test starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_CCM_injectError(SDR_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CCM inject failed");
       retVal = -1;
    } else {
        SDTF_printf("\n CCM inject Test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runCCMSelftestPolarityInvert
 *
 * @brief   Execute CCM with polarity inversion
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMSelftestPolarityInvert(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDTF_printf("\n CCM polarity invert self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_CCM_selfTestPolarityInvert(0xff, 100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CCM polarity invert self test failed");
       retVal = -1;
    } else {
        SDTF_printf("\n CCM polarity invert self test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runCCMVIMSelfTest
 *
 * @brief   Execute CCM VIM Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMVIMSelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_CCM_ErrorStatus_t errorStatus;

    SDTF_printf("\n CCM VIM self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_CCM_selfTest(SDR_CCM_MONITOR_TYPE_VIM,
                              SDR_CCM_SELFTEST_TYPE_NORMAL,
                              10000000,
                              &errorStatus);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CCM VIM self test failed");
        SDTF_printf("\n Error status cmp error %d, self test error %d, Self test error type %d",
                    errorStatus.compareErrorFlag,
                    errorStatus.selfTestErrorFlag,
                    errorStatus.selfTestErrorTypeFlag);
        retVal = -1;
    } else {
        SDTF_printf("\n CCM VIM Self Test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runCCMInactivitySelfTest
 *
 * @brief   Execute CCM inactivity monitor Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMInactivitySelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_CCM_ErrorStatus_t errorStatus;

    SDTF_printf("\n CCM inactivity monitor self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_CCM_selfTest(SDR_CCM_MONITOR_TYPE_INACTIVITY_MONITOR,
                              SDR_CCM_SELFTEST_TYPE_NORMAL,
                              10000000,
                              &errorStatus);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CCM inactivity monitor self test failed");
        SDTF_printf("\n Error status cmp error %d, self test error %d, Self test error type %d",
                    errorStatus.compareErrorFlag,
                    errorStatus.selfTestErrorFlag,
                    errorStatus.selfTestErrorTypeFlag);
        retVal = -1;
    } else {
        SDTF_printf("\n CCM inactivity monitor Self Test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}
