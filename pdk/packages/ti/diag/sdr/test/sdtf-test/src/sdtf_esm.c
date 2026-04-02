/*
 * SDTF ESM
 *
 * Software Diagnostics Reference Test for ESM module
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

#include <sdr_esm.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_esm.h"
#include "sdtf_test.h"

static int32_t SDTF_runESMSelfTestInstance(SDR_ESM_InstanceType esmType);
static int32_t SDTF_runESMInjectInstance(SDR_ESM_InstanceType esmType,
                                         uint32_t groupNumber,
                                         uint32_t bitNumber);

static int32_t SDTF_runESMSelfTestInstance(SDR_ESM_InstanceType esmType)
{
    SDR_Result result;
    int32_t retVal=0;

    SDTF_printf("\n ESM self test, ESM instance %d: starting", esmType);
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run esm test 1*/
    result = SDR_ESM_selfTest(esmType, 1000);
    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    if (result != SDR_PASS ) {
        SDTF_printf("\n ESM self test  failed");
        retVal = -1;
    } else {
        SDTF_printf("\n ESM self test  passed");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMInject
 *
 * @brief   Execute ESM Inject
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
static int32_t SDTF_runESMInjectInstance(SDR_ESM_InstanceType esmType,
                                         uint32_t groupNumber,
                                         uint32_t bitNumber)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ESM_ErrorConfig_t esmErrorConfig;

    esmErrorConfig.groupNumber = groupNumber;
    esmErrorConfig.bitNumber = bitNumber;

    SDTF_printf("\n ESM inject: test starting for Esm instance %d", esmType);

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run esm test 2*/
    result = SDR_ESM_errorInsert(esmType, &esmErrorConfig);
    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    if (result != SDR_PASS ) {
        SDTF_printf("\n ESM inject test for Esm instance %d failed", esmType);
        retVal = -1;
    } else {
        SDTF_printf("\n ESM inject test for Esm instance %d Done", esmType);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMSelfTest_MCU
 *
 * @brief   Execute ESM Self test for MCU
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMSelfTest_MCU(void)
{
    int32_t retVal=0;

    retVal = SDTF_runESMSelfTestInstance(SDR_ESM_INSTANCE_MCU);

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMSelfTest_WKUP
 *
 * @brief   Execute ESM Self test for WKUP
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMSelfTest_WKUP(void)
{
    int32_t retVal=0;

    retVal = SDTF_runESMSelfTestInstance(SDR_ESM_INSTANCE_WKUP);

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMSelfTest_MAIN
 *
 * @brief   Execute ESM Self test for MAIN
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMSelfTest_MAIN(void)
{
    int32_t retVal=0;

    retVal = SDTF_runESMSelfTestInstance(SDR_ESM_INSTANCE_MAIN);

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMInject_MCU
 *
 * @brief   Execute ESM Inject for MCU
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMInject_MCU(void)
{
    int32_t retVal=0;

    retVal = SDTF_runESMInjectInstance(SDR_ESM_INSTANCE_MCU, 1, 4);

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMInject_WKUP
 *
 * @brief   Execute ESM Inject for WKUP
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMInject_WKUP(void)
{
    int32_t retVal=0;

    retVal = SDTF_runESMInjectInstance(SDR_ESM_INSTANCE_WKUP, 0, 9);

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMInject_MAIN
 *
 * @brief   Execute ESM Inject for MAIN
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMInject_MAIN(void)
{
    int32_t retVal=0;

    retVal = SDTF_runESMInjectInstance(SDR_ESM_INSTANCE_MAIN, 3, 9);

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMAPITest
 *
 * @brief   Execute ESM API test, covers APIs not covered by functional test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMAPITest(void)
{
    int32_t retVal=0;
    volatile bool errorStatus;

    SDTF_printf("\n ESM API test: starting");

    /* Run Set N Error */
    SDR_ESM_setNError(SDR_ESM_INSTANCE_MCU);

    /* Get error status to check */
    errorStatus = SDR_ESM_getNErrorStatus(SDR_ESM_INSTANCE_MCU);
    if (errorStatus) {
        retVal = -1;
    }

    if (retVal == 0) {
        /* Run Reset N Error */
        SDR_ESM_resetNError(SDR_ESM_INSTANCE_MCU);
        /* Get error status to check */
        errorStatus = SDR_ESM_getNErrorStatus(SDR_ESM_INSTANCE_MCU);
        if (!errorStatus) {
            retVal = -2;
        }
    }

    if (retVal == 0) {
        SDTF_printf("\n ESM API test  passed");
    } else {
        SDTF_printf("\n ESM API test  failed");
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMNegativeTest
 *
 * @brief   Execute ESM Negative tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMNegativeTest(void)
{
    SDR_Result result;
    int32_t retVal=0;
    SDR_ESM_ErrorConfig_t esmErrorConfig;
    volatile bool errorStatus;

    SDTF_printf("\n ESM Negative  test: starting");

    if (SDR_ESM_errorInsert((SDR_ESM_InstanceType)((uint16_t)SDR_ESM_INSTANCE_MCU - 1U),
                            &esmErrorConfig) == SDR_PASS)
    {
        retVal = -1;
    }

    if (retVal == 0)
    {
        if (SDR_ESM_errorInsert((SDR_ESM_InstanceType)((uint16_t)SDR_ESM_INSTANCE_MAIN + 1U),
                                &esmErrorConfig) == SDR_PASS)
        {
            retVal = -1;
        }
    }

    result = SDR_ESM_errorInsert(SDR_ESM_INSTANCE_MCU, NULL);

    if ( result == SDR_PASS)
    {
        retVal = -1;
    }

    if (retVal != 0 ) {
        SDTF_printf("\n ESM Negative test failed");
    } else {
        SDTF_printf("\n ESM Negative test passed");
    }

    return retVal;
}
