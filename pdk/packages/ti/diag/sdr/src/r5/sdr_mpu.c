/*
 * SDR MPU
 *
 * Software Diagnostics Reference module for MPU
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sdr_common.h>
#include <sdr_mpu.h>
#include <sdr_exception.h>
#include <sdr_mpuCfg.h>

#include <ti/csl/soc.h>

#include "sdr_utils.h"

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the values for mpu self test flag
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_MPU_SELF_TEST_FLAG_NONE=0,
    SDR_MPU_SELF_TEST_FLAG_STARTING=0x55555555U,
    SDR_MPU_SELF_TEST_FLAG_TRIGGERED=0xAAAAAAAAU,

} SDR_mpuSelfTestFlag;

/** ---------------------------------------------------------------------------
 * @brief This structure defines the data used for tracking the current
 *        self test
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_MpuSelfTest_s
{
    volatile SDR_mpuSelfTestFlag    mpuSelfTestState;
    /**< MPU self test state in progress */
    uint32_t               regId;
    /**< MPU self test memory configuration region ID */
    uint32_t               faultAddr;
    /**< MPU self test fault address */
    SDR_MPU_memAccessType  faultType;
    /**< MPU self test fault type */
}  SDR_MpuSelfTest_t;

/* static Global objects */
static SDR_MpuSelfTest_t gSDR_MPUSelfTest;

/* Global Objects */
/* None */

/* Local functions */
static void  SDR_trigger_fault(const SDR_MPU_memConfig_t *pMemConfig);
bool SDR_MPU_exceptionChkSelfTest(SDR_MPU_memAccessType faultType, uint32_t faultAddr);

static void  SDR_trigger_fault(const SDR_MPU_memConfig_t *pMemConfig)
{
    uint32_t      *pMemLocation = pMemConfig->pMemLocation;
    volatile       uint32_t       dummy = 1U;

    if (pMemConfig->memAccessType == SDR_MPU_DATA_WRITE_ACCESS) {
        /* write to the memory to trigger the exception */
        *pMemLocation = dummy;
    }
    else {
        /* read from the memory to trigger the exception */
        dummy = *pMemLocation;
    }
    return;
}

/* Global Functions */

/** ============================================================================*
 *
 * \brief   MPU SELF TEST Check Handler (NOT AN API for SDR MPU)
 *          corrects any mpu self test faults
 *
 * \return  true : if it is in self test; false otherwise
 */
bool SDR_MPU_exceptionChkSelfTest(SDR_MPU_memAccessType faultType, uint32_t faultAddr)
{
    bool    selfTestFlag = (bool)false;

    if ( (gSDR_MPUSelfTest.mpuSelfTestState == SDR_MPU_SELF_TEST_FLAG_STARTING) &&
         (gSDR_MPUSelfTest.faultAddr        == faultAddr) &&
         (gSDR_MPUSelfTest.faultType        == faultType) )
    {
        /* It is in self test flag */
        selfTestFlag = (bool)true;

        /* indicate self test exception trigger flag */
        gSDR_MPUSelfTest.mpuSelfTestState = SDR_MPU_SELF_TEST_FLAG_TRIGGERED;

        /* Provide the necessary privillage for the MPU exception to resolve */
        SDR_MPUResolve(gSDR_MPUSelfTest.faultAddr, gSDR_MPUSelfTest.regId);
    }
    return (selfTestFlag);
}

/** ============================================================================
 *
 * \brief   Runs self test by injecting a fault and monitor response
 *          Assumes MPU Settings from higher layer is already enabled.
 *
 * \param1  pMemConfig: pointer to memory configuration to be used for exception
 * \param2  loopCount: Loopcount for diagnostics to wait in a loop
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_MPU_selfTest(const SDR_MPU_memConfig_t *pMemConfig,
                            uint32_t             loopCount)
{
    SDR_Result retVal = SDR_FAIL;
    uint32_t   loopCnt = loopCount;

    /* Reset self test flag */
    gSDR_MPUSelfTest.mpuSelfTestState = SDR_MPU_SELF_TEST_FLAG_NONE;

    if ((pMemConfig != NULL) &&
        (pMemConfig->mpuRegionId < 16u)) {

        /* start self test for MPU */
        gSDR_MPUSelfTest.mpuSelfTestState = SDR_MPU_SELF_TEST_FLAG_STARTING;
        /* Copy the mpu memory configuration */
        gSDR_MPUSelfTest.regId  = pMemConfig->mpuRegionId;
        /* Record the mpu fault addresss */
        gSDR_MPUSelfTest.faultAddr = (uint32_t) (uintptr_t) pMemConfig->pMemLocation;
        /* Record the mpu fault type */
        gSDR_MPUSelfTest.faultType = pMemConfig->memAccessType;

        /* Add the MPU entry for the fault trigger */
        SDR_MPUCfgAddRegion(pMemConfig);

        /* trigger the mpu self test */
        SDR_trigger_fault(pMemConfig);

        /* wait until loopCount (> 0) for exception to trigger */
        if (loopCnt > 0U) {
            while(loopCnt != 0U)  {
                loopCnt--;
                /* If exception is triggered, self test is successful */
                if (gSDR_MPUSelfTest.mpuSelfTestState == SDR_MPU_SELF_TEST_FLAG_TRIGGERED)
                {
                    retVal = SDR_PASS;
                    break;
                }
            }
       } else { /* Wait forever configuration */
            while (gSDR_MPUSelfTest.mpuSelfTestState != SDR_MPU_SELF_TEST_FLAG_TRIGGERED);
           /* If exception is triggered, self test is successful */
           retVal = SDR_PASS;
       }
       /* Remove the MPU entry for the fault trigger */
       SDR_MPUCfgRemoveRegion(pMemConfig);

       /* Reset self test flag */
       gSDR_MPUSelfTest.mpuSelfTestState = SDR_MPU_SELF_TEST_FLAG_NONE;
    }
    return retVal;
}

/* Nothing past this point */
