/*
 * SDTF ECC
 *
 * Software Diagnostics Reference Test for ECC module
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
#include <string.h>

#include <sdr_ecc.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_ecc.h"
#include "sdtf_test.h"

/* Local functions */
void SDTF_readVIMRAM(uint32_t offset);
int32_t SDTF_ECC_waitDEDTrigger(uint64_t timeCount);

__attribute__((section(".sdtf_ecc_testcodesection"))) void SDTF_ECC_dummyFunction(void);

void SDTF_ECC_dummyFunction(void)
{
    volatile int i;

    i = 10;
    return;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitInjectTest
 *
 * @brief   Execute ECC 1 bit inject test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitInjectTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n ATCM Single bit error inject: test starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x00);

    /* Run one shot test for ATCM 1 bit error */
    injectErrorConfig.flipBitMask = 0x10;
    result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                 SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n ATCM Single bit error inject at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        SDTF_printf("\n ATCM Single bit error inject at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitInjectTest
 *
 * @brief   Execute ECC 2 bit Inject test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitInjectTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n ATCM Double bit error inject: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for ATCM 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x200u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                 SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    /* Access the memory where injection is expected */
    testLocationValue = injectErrorConfig.pErrMem[0];

    if (result != SDR_PASS ) {
        SDTF_printf("\n ATCM Double bit error inject: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n ATCM Double bit error inject: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


void SDTF_readVIMRAM(uint32_t offset)
{

    uint32_t *vimAddr = (uint32_t *)0x40F82000;
    volatile uint32_t tmpValue;

    tmpValue = vimAddr[offset >> 2u];

}

/*********************************************************************
 * @fn      SDTF_runECC1BitVIMRAMInjectTest
 *
 * @brief   Execute ECC 1 bit inject test on VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitVIMRAMInjectTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Single bit error inject: test starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is Actual error address */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82000u);

    /* Run one shot test for ATCM 1 bit error */
    injectErrorConfig.flipBitMask = 0x10;
    result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                                 SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n VIM RAM Single bit error inject at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        /* Note this should result in actual error call back */
        SDTF_printf("\n VIM RAM Single bit error inject at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMInjectTest
 *
 * @brief   Execute ECC 2 bit Inject test with VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMInjectTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Double bit error inject: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for VIM RAM 2 bit error */
    /* Note the address is actual address */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82004u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                                 SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n VIM RAM Double bit error inject: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        SDTF_printf("\n VIM RAM Double bit error inject: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMDEDvector
 *
 * @brief   Execute ECC 2 bit Inject test with VIM RAM,but expect to
 *          trigger VIM DED vector
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMDEDvector(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Double bit error inject: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for VIM RAM 2 bit error */
    /* This is expected to trigger the DED vector */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82098u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                                 SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n VIM RAM Double bit error inject: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n VIM RAM Double bit error inject: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }
    SDTF_printf("\n waiting for DED Vector Trigger");
    if (retVal == 0) {
        retVal = SDTF_ECC_waitDEDTrigger(0x100000000ull);
    }
    if (retVal == 0) {
        SDTF_printf("\n DED Vector Trigger: Success");
    } else {
        SDTF_printf("\n DED Vector Trigger: Fail");
    }
    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC1BitVIMRAMSelfTest
 *
 * @brief   Execute ECC 1 bit self test on VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitVIMRAMSelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is Actual error address */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82000u);

    /* Run one shot test for ATCM 1 bit error */
    injectErrorConfig.flipBitMask = 0x10;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n VIM RAM Single bit error self test: at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        /* Note this should result in actual error call back */
        SDTF_printf("\n VIM RAM Single bit error self test: at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMSelfTest
 *
 * @brief   Execute ECC 2 bit Self test with VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMSelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for VIM RAM 2 bit error */
    /* Note the address is actual address */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82004u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n VIM RAM Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        SDTF_printf("\n VIM RAM Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitSelfTest
 *
 * @brief   Execute ECC 1 bit Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitSelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n ATCM Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x100);

    /* Run one shot test for ATCM 1 bit error */
    injectErrorConfig.flipBitMask = 0x10;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
         SDTF_printf("\n ATCM Single bit error self test at pErrMem 0x%p test failed",
                     injectErrorConfig.pErrMem);
        retVal = -1;
    } else {

        SDTF_printf("\n ATCM Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitSelfTest
 *
 * @brief   Execute ECC 2 bit Self Test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitSelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n ATCM Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for ATCM 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x400u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n ATCM Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n ATCM Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC single bit error test on BTCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB0TCM0Bank0SelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B0TCM0 Bank0 Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010100u);

    /* Run one shot test for BTCM0 1 bit error */
    injectErrorConfig.flipBitMask = 0x20;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n B0TCM0 Bank0 Single bit error self test at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n B0TCM0 Bank0 Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC2BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on BTCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB0TCM0Bank0SelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B0TCM0 Bank0 Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for BTCM0 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010200u);

    injectErrorConfig.flipBitMask = 0x201;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n B0TCM0 Bank0 Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n B0TCM0 Bank0 Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Single bit error test on BTCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB0TCM0Bank1SelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B0TCM0 Bank1 Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010104u);

    /* Run one shot test for BTCM0 1 bit error */
    injectErrorConfig.flipBitMask = 0x20;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n B0TCM0 Bank1 Single bit error self test at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n B0TCM0 Bank1 Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on BTCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB0TCM0Bank1SelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B0TCM0 Bank1 Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for BTCM0 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010204u);

    injectErrorConfig.flipBitMask = 0x201;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n B0TCM0 Bank1 Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n B0TCM0 Bank1 Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitB1TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on B1TCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB1TCM0Bank0SelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B1TCM0 Bank0 Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
     /* Note the address is relative to start of ram */
     injectErrorConfig.pErrMem = (uint32_t *)(0x41010108u);

     /* Run one shot test for BTCM0 1 bit error */
     injectErrorConfig.flipBitMask = 0x20;
     result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                               SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID,
                               SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                               &injectErrorConfig,
                               100000);

     SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

     if (result != SDR_PASS ) {
         SDTF_printf("\n B1TCM0 Bank0 Single bit error self test at pErrMem 0x%p test failed",
                     injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n B1TCM0 Bank0 Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitB1TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on B1TCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB1TCM0Bank0SelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B1TCM0 Bank0 Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for BTCM0 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010208u);

    injectErrorConfig.flipBitMask = 0x201;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n B1TCM0 Bank0 Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n B1TCM0 Bank0 Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitB1TCM0Bank1SelfTest
 *
 * @brief   Execute ECC Single bit error test on B1TCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB1TCM0Bank1SelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B1TCM0 Bank1 Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x4101010cu);

    /* Run one shot test for BTCM0 1 bit error */
    injectErrorConfig.flipBitMask = 0x20;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n B1TCM0 Bank1 Single bit error self test at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n B1TCM0 Bank1 Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitB1TCM0Bank1SelfTest
 *
 * @brief   Execute ECC Single bit error test on B1TCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB1TCM0Bank1SelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B1TCM0 Bank1 Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for BTCM0 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x4101020cu);

    injectErrorConfig.flipBitMask = 0x201;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n B1TCM0 Bank1 Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n B1TCM0 Bank1 Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC1BitCBASSSelfTest
 *
 * @brief   Execute ECC Single bit error self test on CBASS ECC aggregator
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitCBASSSelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;
    uint32_t subType;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n CBASS Single bit error self test: starting");

    memset(&injectErrorConfig, 0, sizeof(injectErrorConfig));

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for CBASS 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0u);

    injectErrorConfig.flipBitMask = 0x1;
    injectErrorConfig.chkGrp = 0x0;
    subType = SDR_ECC_MCU_CBASS_MEM_SUBTYPE_EDC_CTRL_ID;

    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0,
                              subType,
                              SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CBASS  Single bit error self test: Subtype %d: test failed",
                    subType);
        retVal = -1;
    } else {
        SDTF_printf("\n CBASS Single bit error self test: Subtype 0x%p test complete",
                    subType);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitCBASSInjectTest
 *
 * @brief   Execute ECC Single bit error inject test on CBASS ECC aggregator
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitCBASSInjectTest(void)
{
    SDR_Result result;
    int32_t retVal=0;
    uint32_t subType;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    memset(&injectErrorConfig, 0, sizeof(injectErrorConfig));

    SDTF_printf("\n CBASS Single bit error inject test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for CBASS 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0u);

    injectErrorConfig.flipBitMask = 0x1;
    injectErrorConfig.chkGrp = 0x0;
    subType = SDR_ECC_MCU_CBASS_MEM_SUBTYPE_EDC_CTRL_ID;
    result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0,
                              subType,
                              SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig
                              );

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CBASS  Single bit error inject test: Subtype %d: test failed",
                    subType);
        retVal = -1;
    } else {
        SDTF_printf("\n CBASS Single bit error inject test: Subtype 0x%p test complete",
                    subType);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC2BitCBASSSelfTest
 *
 * @brief   Execute ECC Double bit error self test on CBASS ECC aggregator
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitCBASSSelfTest(void)
{
    SDR_Result result;
    int32_t retVal=0;
    uint32_t subType;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n CBASS Double bit error self test: starting");

    memset(&injectErrorConfig, 0, sizeof(injectErrorConfig));

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for CBASS 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0u);

    injectErrorConfig.flipBitMask = 0x5;
    injectErrorConfig.chkGrp = 0x0;
    subType = SDR_ECC_MCU_CBASS_MEM_SUBTYPE_EDC_CTRL_ID;

    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0,
                              subType,
                              SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CBASS  Double bit error self test: Subtype %d: fixed location once test failed",
                    subType);
        retVal = -1;
    } else {
        SDTF_printf("\n CBASS Double bit error self test: Subtype 0x%p fixed location once test complete",
                    subType);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitCBASSInjectTest
 *
 * @brief   Execute ECC Doule bit error inject test on CBASS ECC aggregator
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitCBASSInjectTest(void)
{
    SDR_Result result;
    int32_t retVal=0;
    uint32_t subType;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    memset(&injectErrorConfig, 0, sizeof(injectErrorConfig));

    SDTF_printf("\n CBASS Double bit error inject test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for CBASS 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0u);

    injectErrorConfig.flipBitMask = 0x5;
    injectErrorConfig.chkGrp = 0x0;
    subType = SDR_ECC_MCU_CBASS_MEM_SUBTYPE_EDC_CTRL_ID;
    result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0,
                              subType,
                              SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig
                              );

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n CBASS  Double bit error inject test: Subtype %d: test failed",
                    subType);
        retVal = -1;
    } else {
        SDTF_printf("\n CBASS Double bit error inject test: Subtype 0x%p test complete",
                    subType);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}
static bool SDTF_ECC_DEDTriggerFlag = false;

/*********************************************************************
 *
 * @brief   indicate DED trigger
 *
 *
 * @return  0 : Success; < 0 for failures
 */
void SDTF_ECC_indicateDEDTrigger(void)
{
    SDTF_ECC_DEDTriggerFlag = true;
}


int32_t SDTF_ECC_waitDEDTrigger(uint64_t timeCount)
{
    int32_t retVal = 0;
    uint32_t startTimeStamp, curTimeStamp, lastTimeStamp, deltaTime;
    uint64_t elapsedTime = 0u;

    startTimeStamp = SDR_getTime();
    lastTimeStamp = startTimeStamp;
    while(SDTF_ECC_DEDTriggerFlag != true) {
        curTimeStamp = SDR_getTime();
        deltaTime = curTimeStamp-lastTimeStamp;
        lastTimeStamp = curTimeStamp;
        elapsedTime += deltaTime;
        if (elapsedTime > timeCount) {
            break;
        }
    }

    if (SDTF_ECC_DEDTriggerFlag == true ) {
        SDTF_ECC_DEDTriggerFlag = false;
    } else {
        retVal = -1;
    }
    return retVal;
}

/*********************************************************************
 *
 * @brief   Run Negative tests on ECC module
 *
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_ECC_runNegativeTests(void)
{
    SDR_Result result;
    int32_t retVal = 0;
    SDR_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n ECC negative tests: starting");

    /* Negative tests with invalid mem type 33U */
    result = SDR_ECC_initMemory(33U,
                                0U);
    if (result == SDR_PASS) {
        retVal = -1;
    }

    if (retVal == 0) {
        /* Negative tests with invalid mem subtype 50U */
        result = SDR_ECC_initMemory(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                    50U);
        if (result == SDR_PASS) {
            retVal = -1;
        }
    }

    if (retVal == 0) {
        /* Negative tests with invalid mem subtype 50U */
        result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                  SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                  SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                  NULL,
                                  1000);
        if (result != SDR_BADARGS) {
            retVal = -1;
        }
    }

    if (retVal == 0) {
        /* Negative tests with invalid mem subtype 50U */
        result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                  SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                  SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                  NULL
                                  );
        if (result != SDR_BADARGS) {
            retVal = -1;
        }
    }


    if (retVal == 0) {
        /* Pass invalid error configuration. No bits set */
        injectErrorConfig.flipBitMask = 0x00;
        /* Set Error address */
        injectErrorConfig.pErrMem = (uint32_t *)(0x100);
        /* Negative tests with invalid mem subtype 50U */
        result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                  SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                  SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                  &injectErrorConfig,
                                  1000);
        if (result == SDR_PASS) {
            retVal = -1;
        }
    }

    if (retVal == 0) {
        /* Pass invalid error configuration. No bits set */
        injectErrorConfig.flipBitMask = 0x80000000;
        /* Set Error address */
        injectErrorConfig.pErrMem = (uint32_t *)(0x100);
        /* Negative tests with invalid mem subtype 50U */
        result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                  SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                  SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                  &injectErrorConfig,
                                  1000);
        if (result == SDR_PASS) {
            retVal = -1;
        }
    }

    if ( retVal == 0) {
        SDTF_printf("\n ECC negative tests: success");
    } else {
        SDTF_printf("\n ECC negative tests: failed");
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitCodeInjectTest
 *
 * @brief   Execute ECC 2 bit Inject error on program section
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitCodeInjectTest(void)
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n ATCM Double bit Program section error inject: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Error injection on program memory */
    injectErrorConfig.pErrMem = (uint32_t *)(&SDTF_ECC_dummyFunction);

    injectErrorConfig.flipBitMask = 0x101;

    result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                 SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    /* Call progrem for the ECC error to take effect */
    SDTF_ECC_dummyFunction();

    if (result != SDR_PASS ) {
        SDTF_printf("\n ATCM Double bit Program section error inject: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n ATCM Double bit Program section error inject: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

