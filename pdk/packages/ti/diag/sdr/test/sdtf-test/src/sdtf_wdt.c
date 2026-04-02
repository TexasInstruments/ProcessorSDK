/*
 * SDTF WDT
 *
 * Software Diagnostics Reference Test
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

#include <ti/csl/hw_types.h>
#include <ti/csl/csl_rti.h>
#include <ti/csl/soc.h>

#include <sdr_common.h>
#include <sdr_wdt.h>

#include <ti/osal/osal.h>

#include "sdtf_common.h"
#include "sdtf_wdt.h"
#include "sdtf_test.h"
#include "sdtf_profile.h"
#include <sdtf_core.h>

/* define the unlock and lock values */
#define KICK0_UNLOCK_VAL 0x68EF3490
#define KICK1_UNLOCK_VAL 0xD172BC5A
#define KICK_LOCK_VAL    0x00000000

#define CSL2PTR (uint32_t *)(uintptr_t)

typedef enum rtiClockSource
{
    RTI_CLOCK_SOURCE_HFOSC0_CLKOUT = 0U,
    /**< to select clock frequency of hfosc0 */
    RTI_CLOCK_SOURCE_LFOSC_CLKOUT = 1U,
    /**< to select clock frequency of lfosc */
    RTI_CLOCK_SOURCE_12MHZ = 2U,
    /**< to select clock frequency of 12 MHz */
    RTI_CLOCK_SOURCE_32KHZ = 3U,
    /**< to select clock frequency of 32KHz */
}rtiClockSource_t;


/* Static variables */

static volatile bool gWDTStartedFlag = false;
static volatile uint32_t gTimerCount;
static volatile bool gSelfTestFlag = false;

#ifdef SDTF_WDT_THROUGH_INTERRUPT
/* ============================================= */
volatile uint32_t isrFlag = 0U;


static void RTIAppISR(uintptr_t handle)
{
    uint32_t intrStatus;

    RTIDwwdGetStatus(CSL_MCU_RTI1_CFG_BASE, &intrStatus);
    RTIDwwdClearStatus(CSL_MCU_RTI1_CFG_BASE, intrStatus);
    SDTF_printf(": Interrupt Generated!!!\n");

    isrFlag  = 1U;
}

static void RTIInterruptConfig()
{
    OsalRegisterIntrParams_t intrPrms;
    OsalInterruptRetCode_e osalRetVal;
    HwiP_Handle hwiHandle;
    uint32_t rtiIntNum;

    isrFlag  = 0U;

    Osal_RegisterInterrupt_initParams(&intrPrms);
    intrPrms.corepacConfig.arg          = (uintptr_t)0;
    intrPrms.corepacConfig.priority     = 1U;
    intrPrms.corepacConfig.corepacEventNum = 0U; /* NOT USED ? */

    rtiIntNum = CSL_MCU0_INTR_RTI1_INTR_WWD;

    intrPrms.corepacConfig.isrRoutine   = &RTIAppISR;
    intrPrms.corepacConfig.intVecNum    = rtiIntNum;

    osalRetVal = Osal_RegisterInterrupt(&intrPrms, &hwiHandle);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        SDTF_printf("Error Could not register ISR !!!\n");
    }

    return;
}
#endif
static uint32_t MMR_unlock_one(uint32_t * kick0, uint32_t * kick1)
{
    /* initialize the status variable */
    uint32_t status = 1;

    /* if either of the kick lock registers are locked */
    if (!(*kick0 & 0x1) | !(*kick1 & 0x1))
    {
        /* unlock the partition by writing the unlock values to the kick lock registers */
        *kick0 = KICK0_UNLOCK_VAL;
        *kick1 = KICK1_UNLOCK_VAL;
    }

    /* check to see if either of the kick registers are unlocked. */
    if (!(*kick0 & 0x1))
    {
        status = 0;
    }

    /* return the status to the calling program */
    return status;

}

static uint32_t MMR_lock_one(uint32_t * kick0, uint32_t * kick1)
{
    /* create status return variable */
    uint32_t status = 1;

    /* check to see if either of the kick registers are unlocked. */
    if ((*kick0 & 0x1))
    {
        /* write the kick lock value to the kick lock registers to lock the partition */
        *kick0 = KICK_LOCK_VAL;
        *kick1 = KICK_LOCK_VAL;
    }

    /* check to see if either of the kick registers are still unlocked. */
    if ((*kick0 & 0x1))
    {
        status = 0;
    }
    /* return success or failure */
    return status;
}

int32_t SDTF_WDT_init(void)
{
    int32_t result = 0;
    uint32_t preloadValue;
    int32_t cslStatus = CSL_PASS;
    uint32_t intrStatus;
    volatile uint32_t *hwRegPtr;
    uint32_t regValue;
    
#ifdef SDTF_WDT_THROUGH_INTERRUPT
    /* Add RTI handling through direct CPU interrupt */
    /* Disable this for now to avoid interference with self test */
    RTIInterruptConfig();
#endif
    /* Clear any pending interrupt */
    RTIDwwdGetStatus(CSL_MCU_RTI1_CFG_BASE, &intrStatus);
    RTIDwwdClearStatus(CSL_MCU_RTI1_CFG_BASE, intrStatus);

    /* Calculate Reload value */
    preloadValue = (uint32_t)SDTF_RTI_CLOCK_SOURCE_32KHZ*(uint32_t)SDTF_RTI_DWWD_TIMEOUT_VALUE;

     /* Unlock the 2nd partition */
    (void)MMR_unlock_one(
            CSL2PTR ( CSL_MCU_CTRL_MMR0_CFG0_BASE
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK0),
            CSL2PTR ( CSL_MCU_CTRL_MMR0_CFG0_BASE
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK1));

    /* Select RTI module clock source */
    hwRegPtr = (uint32_t *) (CSL_MCU_CTRL_MMR0_CFG0_BASE + CSL_MCU_CTRL_MMR_CFG0_MCU_RTI1_CLKSEL);
    regValue = (RTI_CLOCK_SOURCE_32KHZ << CSL_MCU_CTRL_MMR_CFG0_MCU_RTI1_CLKSEL_CLK_SEL_SHIFT);
    hwRegPtr[0] = (hwRegPtr[0]
                  & (~CSL_MCU_CTRL_MMR_CFG0_MCU_RTI1_CLKSEL_CLK_SEL_MASK))
                  | regValue;

    (void)MMR_lock_one(
            CSL2PTR ( CSL_CTRL_MMR0_CFG0_BASE
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK0),
            CSL2PTR ( CSL_CTRL_MMR0_CFG0_BASE
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK1));

    /* Read back to check if the value is set */
    if ((hwRegPtr[0] & CSL_MCU_CTRL_MMR_CFG0_MCU_RTI1_CLKSEL_CLK_SEL_MASK)
        != regValue)
    {
        result = -1;
    }

    if (result == 0) {
        /* Configure Watchdog */
        cslStatus = RTIDwwdWindowConfig(CSL_MCU_RTI1_CFG_BASE,
                                        RTI_DWWD_REACTION_GENERATE_NMI,
                                        preloadValue,
                                        RTI_RTIDWWDSIZECTRL_DWWDSIZE_50_PERCENT);
        if (cslStatus !=  CSL_PASS) {
            result = -1;
        }
    }

    if (result == 0) {
        cslStatus =  RTIDwwdWindowVerifyConfig(CSL_MCU_RTI1_CFG_BASE,
                                               RTI_DWWD_REACTION_GENERATE_NMI,
                                               preloadValue,
                                               RTI_RTIDWWDSIZECTRL_DWWDSIZE_50_PERCENT);
        if (cslStatus !=  CSL_PASS) {
            result = -1;
        }
    }

    return result;

}


void SDTF_WDT_feedWatchdogTimer(void)
{
    if(gWDTStartedFlag == false) {
        gWDTStartedFlag = true;
        RTIDwwdCounterEnable(CSL_MCU_RTI1_CFG_BASE);
        gTimerCount = 0U;
    } else {
        gTimerCount++;
        /* Once it crosses the 50% window, feed the Watchdog */
        if (gTimerCount > (SDTF_RTI_DWWD_TIMEOUT_VALUE/2000U)) {
            /* Skip if in self test */
            if (gSelfTestFlag == false)
            {
                RTIDwwdService(CSL_MCU_RTI1_CFG_BASE);
            }
            gTimerCount = 0U;
        }
    }
}

/*********************************************************************
 *
 * \brief   Execute Watchdog self test for end violation
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTSelfTestEndViolation(void)
{
    SDR_WDT_TimerConfig_t timerConfig;
    SDR_WDT_TestConfig_t testConfig;
    SDR_Result result;
    int32_t retVal=0;

    SDTF_printf("\n Watchdog self test end violation: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    timerConfig.timerId = SDR_WDT_TIMER_1;
    timerConfig.timerPeriod = SDTF_RTI_DWWD_TIMEOUT_VALUE * SDTF_PROFILE_CLOCK_FREQUENCY;
    timerConfig.windowSize = SDTF_RTI_DWWDSIZE;

    testConfig.testType = SDR_WDT_TEST_TYPE_END_VIOLATION;
    testConfig.timeoutPeriod = (uint64_t)20000*SDTF_PROFILE_CLOCK_FREQUENCY;

    gSelfTestFlag = true;
    result = SDR_WDT_selftest (&timerConfig, &testConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n Watchdog self test end violation failed");
       retVal = -1;
    } else {
        SDTF_printf("\n Watchdog self test end violation complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }
    gSelfTestFlag = false;

    return retVal;
}

/*********************************************************************
 *
 * \brief   Execute Watchdog self test for window Violation
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTSelfTestWindowViolation(void)
{
    SDR_WDT_TimerConfig_t timerConfig;
    SDR_WDT_TestConfig_t testConfig;
    SDR_Result result;
    int32_t retVal=0;

    SDTF_printf("\n Watchdog self test window violation: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    timerConfig.timerId = SDR_WDT_TIMER_1;
    timerConfig.timerPeriod = SDTF_RTI_DWWD_TIMEOUT_VALUE * SDTF_PROFILE_CLOCK_FREQUENCY;
    timerConfig.windowSize = SDTF_RTI_DWWDSIZE;

    testConfig.testType = SDR_WDT_TEST_TYPE_WINDOW_VIOLATION;
    testConfig.timeoutPeriod = (uint64_t)20000*SDTF_PROFILE_CLOCK_FREQUENCY;
    gSelfTestFlag = true;

    result = SDR_WDT_selftest (&timerConfig, &testConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);


    if (result != SDR_PASS ) {
        SDTF_printf("\n Watchdog self test window violation failed");
       retVal = -1;
    } else {
        SDTF_printf("\n Watchdog self test window violation complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }
    gSelfTestFlag = false;
    return retVal;
}

/*********************************************************************
 *
 * \brief   Execute Watchdog negative tests
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTNegativeTests(void)
{
    SDR_WDT_TimerConfig_t timerConfig;
    SDR_WDT_TestConfig_t testConfig;
    SDR_Result result;
    int32_t retVal=0;

    SDTF_printf("\n Watchdog negative tests: starting");

    timerConfig.timerId = SDR_WDT_TIMER_1;
    timerConfig.timerPeriod = SDTF_RTI_DWWD_TIMEOUT_VALUE * SDTF_PROFILE_CLOCK_FREQUENCY;
    timerConfig.windowSize = SDTF_RTI_DWWDSIZE;

    testConfig.testType = SDR_WDT_TEST_TYPE_END_VIOLATION;
    testConfig.timeoutPeriod = (uint64_t)20000*SDTF_PROFILE_CLOCK_FREQUENCY;

    result = SDR_WDT_selftest (NULL_PTR, &testConfig);

    /* There is expected error here */
    if (result == SDR_PASS) {
        retVal = -1;
    }

    if (retVal == 0) {
        result = SDR_WDT_selftest (&timerConfig, NULL_PTR);

        /* There is expected error here */
        if (result == SDR_PASS) {
            retVal = -1;
        }
    }

    if (retVal == 0) {

        /* Specifically give lower timeout */
        testConfig.timeoutPeriod = (uint64_t)10*SDTF_PROFILE_CLOCK_FREQUENCY;
        gSelfTestFlag = true;
        result = SDR_WDT_selftest (&timerConfig, &testConfig);
        gSelfTestFlag = false;

        /* There is expected error here */
        if (result == SDR_PASS) {
            retVal = -1;
        }
    }

    if (retVal != 0 ) {
        SDTF_printf("\n Watchdog negative tests failed");
       retVal = -1;
    } else {
        SDTF_printf("\n Watchdog negative tests complete");
    }

    return retVal;
}

