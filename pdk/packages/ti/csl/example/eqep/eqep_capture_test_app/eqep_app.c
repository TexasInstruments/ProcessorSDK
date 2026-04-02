/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file     eqep_app.c
 *
 *  \brief    This file contains eQEP test application
 *            Example configures the eQEP to capture in quadrature input signal.
 *            and prints the position count values at index event.
 *            Example configures the eQEP to calculate frequency using unit timeout event.
 *            Based on the position count values calculate the frequency of the input signal.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "eqep_app.h"
#include "eqep_pattern_gen.h"

/* ========================================================================== */
/*                          Macros And Typedefs                               */
/* ========================================================================== */
#define KICK0_UNLOCK_VAL                        (0x68EF3490)
#define KICK1_UNLOCK_VAL                        (0xD172BC5A)

#define EQEP_BASE                               CSL_EQEP0_REG_BASE

#define EQEP_INT_ALL                            EQEP_INT_GLOBAL            | \
                                                EQEP_INT_POS_CNT_ERROR     | \
                                                EQEP_INT_PHASE_ERROR       | \
                                                EQEP_INT_DIR_CHANGE        | \
                                                EQEP_INT_WATCHDOG          | \
                                                EQEP_INT_UNDERFLOW         | \
                                                EQEP_INT_OVERFLOW          | \
                                                EQEP_INT_POS_COMP_READY    | \
                                                EQEP_INT_POS_COMP_MATCH    | \
                                                EQEP_INT_STROBE_EVNT_LATCH | \
                                                EQEP_INT_INDEX_EVNT_LATCH  | \
                                                EQEP_INT_UNIT_TIME_OUT     | \
                                                EQEP_INT_QMA_ERROR

#define EQEP_INT_MAX                            (10U)

/* DEVICE_SYSCLK_FREQ is functional clock of EQEP. For AM64x this is fixed clock.
   Sysclk (500MHz) / 4 = 125MHz. */
#define DEVICE_SYSCLK_FREQ                      (125000000U)

/* Frequence of EQEP signal for testing unit timeout event in Hz. */
#define UNIT_TIMEOUT_EQEP_FREQ                       (500U)

/* Frequence of unit timeout event in Hz.
 * Should be less than the EQEP signal frequency to capture multiple clocks at
 * Timeout event. So configuring it to 1/10th the EQEP signal frequency. */
#define UNIT_TIMEOUT_FREQ                       (UNIT_TIMEOUT_EQEP_FREQ / 10U)

#define APP_TSK_STACK_SIZE                      (16U * 1024U)

#define EqepAppPrint                            UART_printf

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* Eqep interrupt handle. */
HwiP_Handle          gHwiHandle;

/* Pos Count capture at different events. */
volatile uint32_t gIdxIsrCnt = 0U;
uint32_t          gIdxPosCnt[EQEP_INT_MAX];
volatile uint32_t gUnitTimeIsrCnt = 0U;
uint32_t          gUnitTimPosCnt[EQEP_INT_MAX];

/* Synchronization between main task and pattern generator. */
volatile uint32_t    gTestExit = 0;
volatile uint32_t    gStartEqepSignal = 0;
EqepAppPatternParams gPattern = {0};
SemaphoreP_Handle    gEqepSem, gEqepSem1;

/* Test application stack */
static uint8_t  gAppTskStackMain[APP_TSK_STACK_SIZE] __attribute__((aligned(32)));;
static uint8_t  gAppTskStackPatternGen[APP_TSK_STACK_SIZE] __attribute__((aligned(32)));;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static void EqepAppCfgEqepPads(void);
static void EqepPatternCfgGpioPad(void);
static void EqepAppClockEnable(void);
static void AppEqepIntrISR(void *handle);
static void padConfig_prcmEnable(void);
static void EqepAppInitQuadratureWave(void);
static void EqepAppInitFrequencyCalculation(void);
static uint32_t EqepAppCalculateFrequencyUnitTimeout(void);
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static void EqepAppCfgEqepPads(void)
{
    /* Unlock all MMR */
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK0_KICK0, KICK0_UNLOCK_VAL);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK0_KICK1, KICK1_UNLOCK_VAL);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK0, KICK0_UNLOCK_VAL);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK1, KICK1_UNLOCK_VAL);

    /* Configure PADCONFIG 40 to 43 to mux mode 3 for EQEP A,B.S and I signal and input pin. */
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG40, 0x250003);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG41, 0x250003);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG42, 0x250003);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG43, 0x250003);
}

static void EqepPatternCfgGpioPad(void)
{
    /* Configure PADCONFIG 156 to mux mode 0 for ECAP0_IN_APWM_OUT and output pin. */
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG26, 0x010007);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG27, 0x010007);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG44, 0x010007);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG45, 0x010007);
}

static void EqepAppClockEnable(void)
{
    /* For AM64x clock is default on. derived from SYSCLK with 1/2, 250MHz */
}

static void AppEqepIntrISR(void *handle)
{
    uint16_t intEnabled, intStatus;

    intStatus = EQEP_getInterruptStatus(EQEP_BASE);
    intEnabled = EQEP_getEnabledInterrupt(EQEP_BASE);

    if (((intStatus & EQEP_INT_INDEX_EVNT_LATCH) != 0) &&
        ((intEnabled & EQEP_INT_INDEX_EVNT_LATCH) != 0))
    {
        /* Get the position latch count */
        if (gIdxIsrCnt < EQEP_INT_MAX)
        {
            gIdxPosCnt[gIdxIsrCnt] = EQEP_getIndexPositionLatch(EQEP_BASE);
        }
        if (gIdxIsrCnt == (EQEP_INT_MAX - 1))
        {
            EQEP_disableInterrupt(EQEP_BASE, EQEP_INT_ALL);
            SemaphoreP_post(gEqepSem);
        }
        gIdxIsrCnt++;
    }
    if (((intStatus & EQEP_INT_UNIT_TIME_OUT) != 0) &&
        ((intEnabled & EQEP_INT_UNIT_TIME_OUT) != 0))
    {

        /* Get the position latch count */
        if (gUnitTimeIsrCnt < EQEP_INT_MAX)
        {
            gUnitTimPosCnt[gUnitTimeIsrCnt] = EQEP_getPositionLatch(EQEP_BASE);
        }
        if (gUnitTimeIsrCnt == (EQEP_INT_MAX - 1))
        {
            EQEP_disableInterrupt(EQEP_BASE, EQEP_INT_ALL);
            SemaphoreP_post(gEqepSem1);
        }
        gUnitTimeIsrCnt++;
    }


    /* Clear Eqep Interrupt. */
    EQEP_clearInterruptStatus(EQEP_BASE, EQEP_INT_ALL);
}

static void padConfig_prcmEnable(void)
{

    HwiP_Params hwipParams;

    HwiP_Params_init (&hwipParams);

    /* Configure eQEP pads */
    EqepAppCfgEqepPads();

    /* Configure the GPIO pads for generating pattern. */
    EqepPatternCfgGpioPad();

    /* Enable eQEP module time base clock */
    EqepAppClockEnable();

    /* Register & enable interrupt */

    gHwiHandle = HwiP_create(CSLR_R5FSS0_CORE0_INTR_EQEP0_EQEP_INT_0,
                                (HwiP_Fxn) AppEqepIntrISR,
                                &hwipParams);
    HwiP_enableInterrupt(CSLR_R5FSS0_CORE0_INTR_EQEP0_EQEP_INT_0);

}

static void EqepAppInitQuadratureWave(void)
{
    EQEP_setInitialPosition(EQEP_BASE, 0U);
    EQEP_setPosition(EQEP_BASE, 0U);
    /* Configure the decoder for quadrature mode, counting rising edge
      (that is, 1x resolution) */
    EQEP_setDecoderConfig(EQEP_BASE, (EQEP_CONFIG_1X_RESOLUTION |
                                        EQEP_CONFIG_QUADRATURE |
                                        EQEP_CONFIG_NO_SWAP));
    EQEP_setEmulationMode(EQEP_BASE, EQEP_EMULATIONMODE_RUNFREE);

    /* Configure the position counter to reset on an index event */
    EQEP_setPositionCounterConfig(EQEP_BASE, EQEP_POSITION_RESET_IDX, 0xFFFFFFFFU);

    /* Configure the position counter to be latched on rising edge index */
    EQEP_setLatchMode(EQEP_BASE, EQEP_LATCH_RISING_INDEX);


    EQEP_enableModule(EQEP_BASE);
    EQEP_enableInterrupt(EQEP_BASE, EQEP_INT_INDEX_EVNT_LATCH);
}

static void EqepAppInitFrequencyCalculation(void)
{
    EQEP_setInitialPosition(EQEP_BASE, 0U);
    EQEP_setPosition(EQEP_BASE, 0U);
    /* Configure the decoder for up-count mode, counting both rising and
       falling edges (that is, 2x resolution) */
    EQEP_setDecoderConfig(EQEP_BASE, (EQEP_CONFIG_2X_RESOLUTION |
                                       EQEP_CONFIG_UP_COUNT |
                                       EQEP_CONFIG_NO_SWAP));
    EQEP_setEmulationMode(EQEP_BASE, EQEP_EMULATIONMODE_RUNFREE);

    /* Configure the position counter to reset on an unit timeout event */
    EQEP_setPositionCounterConfig(EQEP_BASE, EQEP_POSITION_RESET_UNIT_TIME_OUT, 0xFFFFFFFF);

    /* Enable the unit timer, setting the frequency to UNIT_TIMEOUT_FREQ */
    EQEP_enableUnitTimer(EQEP_BASE, (DEVICE_SYSCLK_FREQ / (UNIT_TIMEOUT_FREQ)));

    /* Configure the position counter to be latched on a unit time out */
    EQEP_setLatchMode(EQEP_BASE, EQEP_LATCH_UNIT_TIME_OUT);

    /* Enable the eQEP1 module */
    EQEP_enableModule(EQEP_BASE);

    /* Configure and enable the edge-capture unit. The capture clock divider is
       SYSCLKOUT/128. The unit-position event divider is QCLK/8. */
    EQEP_setCaptureConfig(EQEP_BASE, EQEP_CAPTURE_CLK_DIV_128,
                          EQEP_UNIT_POS_EVNT_DIV_8);
    EQEP_enableCapture(EQEP_BASE);

    /* Enable unit timeout interrupt. */
    EQEP_enableInterrupt(EQEP_BASE, EQEP_INT_UNIT_TIME_OUT);
}

static uint32_t EqepAppCalculateFrequencyUnitTimeout(void)
{
    /* This function calculates the average frequency.
       based on the latched pos values at the unit timeout event.
       It assumes that the array gUnitTimPosCnt is filled. */
    uint32_t i;
    uint32_t posCnt;
    uint32_t freq, avgFreq = 0U;

    for (i=1; i<EQEP_INT_MAX; i++)
    {
        /* posCnt is reset upon the unit timeout event.
           So each value is the diff from prev pos Cnt at unit time out */
        posCnt = gUnitTimPosCnt[i];

        /* Unit timeout is configured as UNIT_TIMEOUT_FREQ Hz. and position count is calculating on both edges. */
        freq = posCnt * (UNIT_TIMEOUT_FREQ) / 2;
        /* Note: this is a simplified equation. Boundary condition not taken care. */
        avgFreq += freq;
    }
    /* Ignoring first timeout event position for frequency calculation,
     * as it could contain older value. */
    avgFreq /= (EQEP_INT_MAX - 1);

    return avgFreq;
}

static Void eqepTaskFxn(UArg a0, UArg a1)
{
    uint32_t i, avgFreq;
    SemaphoreP_Params semParam;
    int32_t expCnt;
    int32_t  testResult = 0;
    Board_initCfg           boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    EqepAppPrint("Starting EQEP test application...\n");

    SemaphoreP_Params_init(&semParam);
    gEqepSem = SemaphoreP_create (0, &semParam);
    gEqepSem1 = SemaphoreP_create (0, &semParam);
    padConfig_prcmEnable();

    EQEP_disableInterrupt(EQEP_BASE, EQEP_INT_ALL);
    EQEP_clearInterruptStatus(EQEP_BASE, EQEP_INT_ALL);

    gIdxIsrCnt = 0;
    gUnitTimeIsrCnt = 0;

    EqepAppPrint("\nStarting Quadrature input capture test\n");

    EqepAppPrint("Sending the quadrature wave for 50 cycles in clockwisedirection.\n"
                 "With index event in between, Captures 4 edges per cycle\n");

    /* The count values expected is 199
     * Count starts from 0 and counts 4 edges per cycle. */
    expCnt = 199U;

    /* Enable capture from quadrature wave with index event. */
    EqepAppInitQuadratureWave();

    gPattern.eqepClockFreq = 500;
    gPattern.direction = 1;
    gPattern.idxEvtCnt = EQEP_INT_MAX;
    gPattern.loopCnt = 50;
    gPattern.generateIdxPulse = 1;
    gStartEqepSignal = 1;

    /* wait for the EQEP_INT_MAX number of EQEP Index Latch interrupt. */
    SemaphoreP_pend (gEqepSem, SemaphoreP_WAIT_FOREVER);

    /* check position Cnt. */
    for (i = 1; i < EQEP_INT_MAX; i++)
    {
        if (expCnt != gIdxPosCnt[i])
        {
            EqepAppPrint("Quadrature capture count doesnot match\n");
            testResult = -1;
            break;
        }
    }
    if (testResult == -1)
    {
        EqepAppPrint("Captured Position Count values are: \n");
        for (i = 0; i < EQEP_INT_MAX; i++)
        {
            EqepAppPrint(" %d,", gIdxPosCnt[i]);
        }
        EqepAppPrint("\n");
    }

    EqepAppPrint("\nSending the quadrature wave for 50 cycles in anticlockwise direction.\n"
                 "With index event in between, Captures 4 edges per cycle\n");
    /* The count values expected is -199
     * Count starts from 0 and counts 4 edges per cycle. */
    expCnt = -199U;

    gIdxIsrCnt = 0;
    gUnitTimeIsrCnt = 0;

    /* Enable capture from quadrature wave with index event. */
    EqepAppInitQuadratureWave();

    gPattern.eqepClockFreq = 500;
    gPattern.direction = 0;
    gPattern.idxEvtCnt = EQEP_INT_MAX;
    gPattern.loopCnt = 50;
    gPattern.generateIdxPulse = 1;
    gStartEqepSignal = 1;

    /* wait for the EQEP_INT_MAX number of EQEP Index Latch interrupt. */
    SemaphoreP_pend (gEqepSem, SemaphoreP_WAIT_FOREVER);

    /* check position Cnt. */
    for (i = 1; i < EQEP_INT_MAX; i++)
    {
        if (expCnt != gIdxPosCnt[i])
        {
            EqepAppPrint("Quadrature capture count doesnot match\n");
            testResult = -1;
            break;
        }
    }
    if (testResult == -1)
    {
        EqepAppPrint("Captured Position Count values are: \n");
        for (i = 0; i < EQEP_INT_MAX; i++)
        {
            EqepAppPrint(" %d,", gIdxPosCnt[i]);
        }
        EqepAppPrint("\n");
    }

    if (testResult == 0)
    {
        EqepAppPrint("Quadrature input capture test Passed\n");
    }

    EqepAppPrint("\nStarting Frequency calculation test\n");
    EqepAppPrint("Expected Frequency is %d Hz\n", UNIT_TIMEOUT_EQEP_FREQ);

    gIdxIsrCnt = 0;
    gUnitTimeIsrCnt = 0;

    /* Enable Frequency Calculation. */
    EqepAppInitFrequencyCalculation();

    gPattern.eqepClockFreq = UNIT_TIMEOUT_EQEP_FREQ;
    gPattern.direction = 1;
    gPattern.idxEvtCnt = 10;
    gPattern.loopCnt = 50;
    gPattern.generateIdxPulse = 0;
    gStartEqepSignal = 1;

    SemaphoreP_pend (gEqepSem1, SemaphoreP_WAIT_FOREVER);

    avgFreq = EqepAppCalculateFrequencyUnitTimeout();
    EqepAppPrint("\nAverage frequency is %d Hz\n", avgFreq);

    if (avgFreq != UNIT_TIMEOUT_EQEP_FREQ)
    {
        EqepAppPrint("Captured Unit time Position Count values are: \n");
        for (i = 1; i < EQEP_INT_MAX; i++)
        {
            EqepAppPrint(" %d,", gUnitTimPosCnt[i]);
        }
        EqepAppPrint("\n");

        EqepAppPrint("Frequency calculation test failed\n");
        testResult = -1;
    }
    else
    {
        EqepAppPrint("\nFrequency calculation test passed\n");
    }
    if (testResult == 0)
    {
        EqepAppPrint("\nAll tests have passed.\n");
    }
    gTestExit = 1;
}

int main(void)
{
    Task_Handle task;
    Error_Block eb;
    Task_Params taskParams;

    Error_init(&eb);

    /* Initialize the task params */
    Task_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stackSize    = sizeof (gAppTskStackMain);

    task = Task_create(eqepTaskFxn, &taskParams, &eb);
    if(NULL == task)
    {
        BIOS_exit(0);
    }

    /* Initialize the task params */
    Task_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackPatternGen;
    taskParams.stackSize    = sizeof (gAppTskStackPatternGen);

    task = Task_create(eqepPatternGenerator, &taskParams, &eb);
    if(NULL == task)
    {
        BIOS_exit(0);
    }

    BIOS_start();    /* does not return */

    return(0);
}

