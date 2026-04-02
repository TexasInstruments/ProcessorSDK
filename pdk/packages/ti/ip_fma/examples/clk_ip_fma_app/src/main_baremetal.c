/*
 *   Copyright (c) Texas Instruments Incorporated 2026
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

 /**
 *  \file     main_baremetal.c
 *
 *  \brief    This example application demonstrates comparison of 2 clock
 *            sources.
 *
 *  \details  Different clock sources are provided to Counter 1 (Test Clock)
 *            and Counter 0 (Reference Clock). The application configures the
 *            DCC module to operate in single-shot mode and generate an
 *            interrupt when Counter 1 reaches 0. When Counter 0 along with
 *            the valid counter and Counter 1 reach 0 at the same time, a
 *            completion interrupt is generated, indicating that no clock
 *            drift was observed. In addition, the application performs
 *            periodic software readback of static DCC configuration
 *            registers and reports match or mismatch between expected
 *            register values and actual (read) values for validation and
 *            diagnostic purposes.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/csl_dcc.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>

/* Interrupt Registrations */
#include <ti/osal/osal.h>

#include <ip_fma_clk.h>
/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/**< Example Common display string */
#define APP_DCC_STR                     "CSL DCC Example"
/**< Instance of DCC. While changing the instance, ensure update clock sources*/
#define APP_DCC_MODULE_INST             (CSL_MCU_DCC0_BASE)
/**< One Shot mode, Stop counting when Counter 1, reaches 0. */
#define APP_DCC_MODE                    (DCC_OPERATION_MODE_SINGLE_SHOT2)
/**< Maximum value that can be held in the COUNT0 register (ref clock) */
#define APP_DCC_SRC0_MAX_VAL            (0xFFFFFU)
/**< Maximum value that can be held in the VALID0 register (ref clock) */
#define APP_DCC_SRC0_VALID_MAX_VAL      (0x0FFFFU)
/**< Maximum value that can be held in the COUNT1 register (test clock) */
#define APP_DCC_SRC1_MAX_VAL            (0xFFFFFU)
/* Defines that control the clock inputs to DCC and allowed variance */
#define APP_DCC_REF_CLOCK_SRC_0         (DCC_DCCCLKSRC0_CLKSRC0_0)
#define APP_DCC_TEST_CLOCK_SRC_1        (DCC_DCCCLKSRC1_CLKSRC_0)
/**< Allowed drift in percentage (+/-) */
#define APP_DCC_TEST_CLOCK_SRC_1_DRIFT  (5U)
/**< Clock source for Counter 0, 19.2 MHz */
#define APP_DCC_REF_CLOCK_FREQ_IN_KHZ   (19200U)
/**< Expected test clock frequency in KHz */
#define APP_DCC_TEST_CLOCK_FREQ_IN_KHZ  (333334U)
/**< Number of periodic readbacks */
#define PERIODIC_CHECK_NUM                        ((uint8_t)10U)
/**< Delay time in ms */
#define DELAY_MS                                  ((uint8_t)1000U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/**< Flag used to indicate occurrence of the completion interrupt */
volatile uint32_t isrFlag = 0U;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   Initializes board/UART
 */
static void ClkApp_InitUart(void);

/**
 * \brief   This function returns clock frequencies
 *
 * \param   dccInst     Instance of DCC
 * \param   clkSrc0     Clock source for counter 0
 * \param   clkSrc1     Clock source for counter 1
 * \param   clk0Freq    Clock frequency for counter 0 in KHz
 * \param   clk1Freq    Clock frequency for counter 1 in KHz
 *
 * \retval  status      Negative number in case of errors
 */
static int32_t ClkApp_GetClkfreqKHz(uint32_t dccInst,
                                    uint32_t clkSrc0, uint32_t clkSrc1,
                                    uint32_t *clk0Freq, uint32_t *clk1Freq);

/**
 * \brief   This function returns least integral ratio for given clocks.
 *
 * \param   refClkFreq      Reference clock frequency in KHz.
 * \param   testClkFreq     Test clock frequency in KHz.
 * \param   refClkRatioNum  Reference clock ratio number.
 * \param   testClkRatioNum Test clock ratio number.
 *
 * \retval  None.
 */
static void ClkApp_GetClkRatio(uint32_t  refClkFreq,
                              uint32_t  testClkFreq,
                              uint32_t *refClkRatioNum,
                              uint32_t *testClkRatioNum);

/**
 * \brief   This function returns seed value for COUNT1.
 *
 * \param   refClkFreq      Reference clock frequency in KHz.
 * \param   testClkFreq     Test clock frequency in KHz.
 * \param   refClkRatioNum  Reference clock ratio number.
 * \param   testClkRatioNum Test clock ratio number.
 * \param   drfitPer        Allowed drift in test clock in percentage.
 * \param   configParams    DCC configuration parameters.
 *                          Refer enum #dccConfigParams_t.
 *
 * \retval  None.
 */

static void ClkApp_SetSeedVals(uint32_t           refClkFreq,
                              uint32_t           testClkFreq,
                              uint32_t           refClkRatioNum,
                              uint32_t           testClkRatioNum,
                              uint32_t           drfitPer,
                              dccConfigParams_t *configParams);

/**
 * \brief   This function register ISR for a given instance of DCC
 *
 * \param   dccInst     Instance of DCC
 *
 * \retval  CSL_PASS on successful interrupt handler registration.
 */
static int32_t ClkApp_RegisterIsr(uint32_t dccInst);

/**
 * \brief   ISR for done interrupt, set a global flag
 *
 * \retval  None
 */
static void ClkApp_DoneIntrISR(uintptr_t handle);

/**
 * \brief   This function introduces a delay in milliseconds.
 *
 * \param   wait_in_ms   Number of milliseconds to wait.
 *
 * \retval  None.
 */

static void ClkApp_Delay(uint32_t wait_in_ms);

/**
 * \brief   Executes a single‑shot DCC (Dual Clock Comparator) test sequence and performs
 *          periodic software readback of DCC registers.
 * 
 * \retval  None.
 */
static void ClkApp_TestCslDccSingleshotmodeApp(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void ClkApp_TestCslDccSingleshotmodeApp(void)
{
    int32_t status;
    uint32_t clk0Freq, clk1Freq, refClkRatioNum, testClkRatioNum;
    dccConfigParams_t configParams;

    ClkApp_InitUart();

    UART_printf("\r\n" APP_DCC_STR ": Start\r\n");
    /* Steps
    1. Determine the clock frequencies for the sources
    2. Figure out the seed values for successful completion
    3. Configure Interrupts for normal completion
    4. Configure DCC instance
    5. Enable DCC
    6. Wait for normal completion or error interrupt and
       perform periodic software readback of DCC registers
    */

    /* Step 1 Determine the clock frequencies for the sources */
    status = ClkApp_GetClkfreqKHz(APP_DCC_MODULE_INST,
                                      APP_DCC_REF_CLOCK_SRC_0,
                                      APP_DCC_TEST_CLOCK_SRC_1,
                                      &clk0Freq, &clk1Freq);
    if (CSL_PASS == status)
    {
        /* Step 2 Figure out the seed values for successful completion */
        /* Get clock ratio */
        ClkApp_GetClkRatio(clk0Freq,
                      clk1Freq,
                      &refClkRatioNum,
                      &testClkRatioNum);

        configParams.mode    = APP_DCC_MODE;
        configParams.clkSrc0 = APP_DCC_REF_CLOCK_SRC_0;
        configParams.clkSrc1 = APP_DCC_TEST_CLOCK_SRC_1;
        configParams.alternateClkSrc1 = FALSE;
        /* Get the seed values for given clock selections and allowed drift */
        ClkApp_SetSeedVals(clk0Freq,
                          clk1Freq,
                          refClkRatioNum,
                          testClkRatioNum,
                          APP_DCC_TEST_CLOCK_SRC_1_DRIFT,
                          &configParams);

        /* Step 3 Configure Interrupts for normal completion */
        ClkApp_RegisterIsr(APP_DCC_MODULE_INST);

        /* Step 4 Configure DCC instance */
        status = DCCSetConfig(APP_DCC_MODULE_INST, &configParams);

        if (CSL_PASS == status)
        {
            /* Clear the interrupt flag, Wait for completion interrupt will pend
               on this */
            isrFlag = 0U;

            UART_printf(APP_DCC_STR ": DCC configured \n");
            DCCEnableIntr(APP_DCC_MODULE_INST, DCC_INTERRUPTS_ERROR);
            DCCEnableIntr(APP_DCC_MODULE_INST, DCC_INTERRUPTS_DONE);

            UART_printf(APP_DCC_STR ": Enabling DCC and waiting for "
                        "completion interrupt \n");

            UART_printf(APP_DCC_STR ": Register check starts...\r\n");
            /* Get expected values from DCC registers for MCU_DCC0 instance */
            IpFma_DccRegs dccRegsExpValues;
            IpFma_Status statusRb = IPFMA_OK;
            statusRb = IpFma_Clk_GetDccRegs(APP_DCC_MODULE_INST, &dccRegsExpValues);
            //memset(&dccRegsExpValues, 0x0, sizeof(dccRegsExpValues));
            UART_printf(APP_DCC_STR ": Loading expected DCC register values...\n");

            /* Step 5 Enable DCC */
            DCCEnable(APP_DCC_MODULE_INST, TRUE);

            volatile uint32_t intStatus = 0U;
            /* Step 6 Wait for completion interrupt or error flag */
            while ((0U == isrFlag) &&
                (DCC_DCCSTAT_ERRFLG_MASK != (intStatus & DCC_DCCSTAT_ERRFLG_MASK)))
            {
                intStatus = DCCGetIntrStatus(APP_DCC_MODULE_INST);
                if (IPFMA_OK == statusRb)
                {
                    UART_printf(APP_DCC_STR ": DONE!\n");

                    int8_t i = PERIODIC_CHECK_NUM;
                    while (i > 0)
                    {
                        /* Read the actual values from registers */
                        IpFma_DccRegs dccRegsActualValues;
                        statusRb = IpFma_Clk_GetDccRegs(APP_DCC_MODULE_INST, &dccRegsActualValues);

                        if (IPFMA_OK == statusRb)
                        {
                            /* Compare expected and actual values periodically */
                            UART_printf(APP_DCC_STR ": Comparing expected-actual DCC register values...");
                            statusRb = IpFma_Clk_CompareDccRegs(&dccRegsExpValues, &dccRegsActualValues);

                            if (IPFMA_OK == statusRb)
                            {
                                UART_printf(" values MATCH!\n");
                            }
                            else
                            {
                            UART_printf(" values MISMATCH!\n");
                            }
                            i--;
                        }
                        ClkApp_Delay(DELAY_MS);
                    }
                }
               
            }
            /* Ensure no error */
            if (DCC_DCCSTAT_ERRFLG_MASK == (intStatus & DCC_DCCSTAT_ERRFLG_MASK))
            {
                UART_printf(APP_DCC_STR ": DCC Generated completion interrupt \n");
                UART_printf(APP_DCC_STR ": Error : DCC Generated error interrupt\n");
                UART_printf(APP_DCC_STR ": Error interrupt is not expected \n");
            }
            else
            {
                UART_printf(APP_DCC_STR ": DCC Generated completion interrupt \n");
                UART_printf(APP_DCC_STR ": No Clock Drift was observed \n");
            }

            if (statusRb == IPFMA_OK)
            {
                UART_printf(APP_DCC_STR ": All tests have passed. \n");
            }
            else
            {
                UART_printf(APP_DCC_STR ": DCC register check failed!! \n");
            }            
        }
        else
        {
            UART_printf(APP_DCC_STR ": Some/All Tests have failed. \n");            
        }
    }
    else
    {
        UART_printf(APP_DCC_STR ": Error : Could not derive clock "
                        "frequency!!!\r\n");
    }
   
    UART_printf(APP_DCC_STR ": Completes!!!\r\n");

    return;
}

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */
static void ClkApp_InitUart(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        UART_printf("[Error] Board init failed!!\n");
    }
}

static int32_t ClkApp_GetClkfreqKHz(uint32_t dccInst,
                                        uint32_t clkSrc0, uint32_t clkSrc1,
                                        uint32_t *clk0Freq, uint32_t *clk1Freq)
{
    int32_t retVal = CSL_EFAIL;
    switch (dccInst)
    {
        case CSL_MCU_DCC0_BASE :
            if (DCC_DCCCLKSRC0_CLKSRC0_0 == clkSrc0)
            {
                /* 19.2 MHz */
                *clk0Freq = APP_DCC_REF_CLOCK_FREQ_IN_KHZ;
                retVal = CSL_PASS;
            }
            if ((DCC_DCCCLKSRC1_CLKSRC_0 == clkSrc1) &&
                (CSL_PASS == retVal))
            {
                /* 333 MHz */
                *clk1Freq = APP_DCC_TEST_CLOCK_FREQ_IN_KHZ;
            }
            else
            {
                retVal = CSL_EFAIL;
            }
            if (CSL_PASS != retVal)
            {
                UART_printf(APP_DCC_STR ": Error : Selected clock sources is"
                        "not supported in this example !!!\r\n");
            }
        break;

        default :
            UART_printf(APP_DCC_STR ": Error : DCC Instance not supported in"
                        " this example !!!\r\n");
        break;
    }
    return (retVal);
}

static void ClkApp_GetClkRatio(uint32_t  refClkFreq,
                              uint32_t  testClkFreq,
                              uint32_t *refClkRatioNum,
                              uint32_t *testClkRatioNum)
{
    uint32_t loopCnt, hcf = 1U;

    for (loopCnt = 1;
         (loopCnt <= refClkFreq) || (loopCnt <= testClkFreq);
         loopCnt++)
    {
        if ((refClkFreq % loopCnt == 0) && (testClkFreq % loopCnt == 0))
        {
            hcf = loopCnt;
        }
    }
    *refClkRatioNum  = (refClkFreq / hcf);
    *testClkRatioNum = (testClkFreq / hcf);
}

static void ClkApp_SetSeedVals(uint32_t           refClkFreq,
                              uint32_t           testClkFreq,
                              uint32_t           refClkRatioNum,
                              uint32_t           testClkRatioNum,
                              uint32_t           drfitPer,
                              dccConfigParams_t *configParams)
{
    uint32_t maxFreqKHz, maxCntLimit;
    uint32_t maxRefCnt, minRefCnt;
    uint64_t mulVar;

    /* Find maximum frequency between test and reference clock */
    if (refClkFreq > testClkFreq)
    {
        maxFreqKHz  = refClkFreq;
        maxCntLimit = APP_DCC_SRC0_MAX_VAL;
    }
    else
    {
        maxFreqKHz  = testClkFreq;
        maxCntLimit = APP_DCC_SRC1_MAX_VAL;
    }
    /* Calculate seed values for 0% drift */
    if (maxFreqKHz == refClkFreq)
    {
        configParams->seedSrc0 = maxCntLimit / refClkRatioNum;
        configParams->seedSrc0 = configParams->seedSrc0 * refClkRatioNum;
        mulVar = ((uint64_t) (configParams->seedSrc0) *
                  (uint32_t) (testClkRatioNum));
        configParams->seedSrc1   = (uint32_t) (mulVar / refClkRatioNum);
        configParams->seedValid0 = refClkRatioNum;
    }
    else
    {
        configParams->seedSrc1 = maxCntLimit / testClkRatioNum;
        configParams->seedSrc1 = configParams->seedSrc1 * testClkRatioNum;
        mulVar = ((uint64_t) (configParams->seedSrc1) *
                  (uint32_t) (refClkRatioNum));
        configParams->seedSrc0   = (uint32_t) (mulVar / testClkRatioNum);
        configParams->seedValid0 = 1U;
    }
    /* Applying allowed drift */
    if (((APP_DCC_SRC0_MAX_VAL + APP_DCC_SRC0_VALID_MAX_VAL) <
         (configParams->seedSrc0 * (100U + drfitPer) / 100U)))
    {
        /* Seed values with drift exceeds maximum range */
        UART_printf(APP_DCC_STR ": Seed values with drift exceeds"
                        " allowed range\n");
        UART_printf(APP_DCC_STR ": Application will run with 0% "
                        " allowed drift\n");
    }
    else if (100U < drfitPer)
    {
        /* Error percentage is greater than 100 */
        UART_printf(APP_DCC_STR ": Warning Wrong drift %,Not applying drift\n");
        UART_printf(APP_DCC_STR ": Application will run with 0% drift\n");
    }
    else
    {
        maxRefCnt = (configParams->seedSrc0 * (100U + drfitPer) / 100U);
        minRefCnt = (configParams->seedSrc0 * (100U - drfitPer) / 100U);
        if (APP_DCC_SRC0_VALID_MAX_VAL < (maxRefCnt - minRefCnt))
        {
            UART_printf(APP_DCC_STR ": Warning Seed value for valid count "
                        "exceeds allowed range.\n");
            UART_printf(APP_DCC_STR "Application will run with 0% allowed"
                        " drift.\n");
        }
        else
        {
            if (maxRefCnt == minRefCnt)
            {
                configParams->seedValid0 = 1U;
            }
            else
            {
                configParams->seedSrc0   = minRefCnt;
                configParams->seedValid0 = (maxRefCnt - minRefCnt);
            }
        }
    }
    UART_printf(APP_DCC_STR ": Seed values calculation done.\n");
}

static int32_t ClkApp_RegisterIsr(uint32_t dccInst)
{
    int32_t retVal = CSL_EFAIL;
    uint32_t dccInt;
    OsalRegisterIntrParams_t intrPrms;
    OsalInterruptRetCode_e osalRetVal;
    HwiP_Handle hwiHandle;

    Osal_RegisterInterrupt_initParams(&intrPrms);
    intrPrms.corepacConfig.arg          = (uintptr_t)0;
    intrPrms.corepacConfig.priority     = 1U;
    intrPrms.corepacConfig.corepacEventNum = 0U;

    switch (dccInst)
    {
        case CSL_MCU_DCC0_BASE :

        dccInt = CSLR_MCU_R5FSS0_CORE0_INTR_MCU_DCC0_INTR_DONE_LEVEL_0;
        intrPrms.corepacConfig.isrRoutine   = &ClkApp_DoneIntrISR;
        retVal = CSL_PASS;
        break;

        default :
            UART_printf(APP_DCC_STR ": Error : DCC Instance not supported in"
                        " this example !!!\r\n");
        break;
    }
    if (CSL_PASS == retVal)
    {
        intrPrms.corepacConfig.intVecNum = dccInt;

        osalRetVal = Osal_RegisterInterrupt(&intrPrms, &hwiHandle);
        if(OSAL_INT_SUCCESS != osalRetVal)
        {
            retVal = CSL_EFAIL;
            UART_printf(APP_DCC_STR ": Error Could not register ISR !!!\n");
        }
    }
    return (retVal);
}

static void ClkApp_DoneIntrISR(uintptr_t handle)
{
    DCCClearIntrStatus(APP_DCC_MODULE_INST, DCC_INTERRUPTS_DONE);
    isrFlag  = 1U;
}

static void ClkApp_Delay(uint32_t wait_in_ms)
{
    while (wait_in_ms--) {
        asm("   NOP");
    }
}

int main(void)
{
    (void) ClkApp_TestCslDccSingleshotmodeApp();

    return(0);
}

/********************************* End of file ******************************/
