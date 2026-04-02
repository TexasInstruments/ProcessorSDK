/*
 * SDTF MPU
 *
 * Software Diagnostics Reference Test for MPU module
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

#include <sdr_mpu.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_mpu.h"
#include "sdtf_test.h"

/* SDTF MPU SelfTest local defines */

#define SDTF_MPU_SELF_TEST_MEM_BASE  (0x41C7EFE0U)
#define SDTF_MPU_SELF_TEST_MEM_SIZE  (32U)

/*********************************************************************
 * @fn      SDTF_runMPUSelfTest
 *
 * @brief   Execute MPU Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runMPUSelfTest(void)
{
    SDR_Result result;
    SDR_MPU_memConfig_t  triggerFaultMemRd, triggerFaultMemWr;
    uint32_t             loopCnt = 10U;
    int32_t              retVal=0;

    /* Initialize the MPU for the test */
    triggerFaultMemWr.pMemLocation  = (uint32_t *) SDTF_MPU_SELF_TEST_MEM_BASE;
    triggerFaultMemWr.memAccessType = SDR_MPU_DATA_WRITE_ACCESS;
    triggerFaultMemWr.mpuRegionId   = 8;

    triggerFaultMemRd.pMemLocation  = (uint32_t *) SDTF_MPU_SELF_TEST_MEM_BASE;
    triggerFaultMemRd.memAccessType = SDR_MPU_DATA_READ_ACCESS;
    triggerFaultMemRd.mpuRegionId   = 8;

    /* Initialize the memory for triggering MPU exception */
    SDTF_printf("\n MPU self test for Mem Write Access Fault: starting");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_MPU_selfTest(&triggerFaultMemWr, loopCnt);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n MPU self test for Mem Write Access Fault failed");
        retVal = -1;
    } else {
        SDTF_printf("\n MPU self test for Mem Write Access Fault complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    SDTF_printf("\n MPU self test for Mem Read Access Fault: starting");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_MPU_selfTest(&triggerFaultMemRd, loopCnt);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n MPU self test for Mem Read Access Fault failed");
        retVal = -1;
    } else {
        SDTF_printf("\n MPU self test for Mem Read Access Fault complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }


    SDTF_printf("\n MPU self test for Mem Write Access Fault (Wait Forever): starting");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_MPU_selfTest(&triggerFaultMemWr, 0U);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n MPU self test for Mem Write Access Fault (Wait Forever) failed");
        retVal = -1;
    } else {
        SDTF_printf("\n MPU self test for Mem Write Access Fault  (Wait Forever) complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    SDTF_printf("\n MPU self test for Mem Read Access Fault  (Wait Forever): starting");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_MPU_selfTest(&triggerFaultMemRd, 0U);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n MPU self test for Mem Read Access Fault (Wait Forever) failed");
        retVal = -1;
    } else {
        SDTF_printf("\n MPU self test for Mem Read Access Fault  (Wait Forever) complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}
