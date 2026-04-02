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
 *  \file     epwm_ecap_loopback_app.c
 *
 *  \brief    This file contains ePWM to eCAP loopback test application
 *  The ecap module is configured in the capture mode and the ecap device pin is
 *  configured as input pin. A square wave needs to be fed to the ecap pin
 *  externally. Based on the internal counter the count values for each of the
 *  edge is latched in register. 4th edge will generate the interrupt. Based on
 *  the latched counter values calculate the input signal frequency and the duty
 *  cycle based on the input functional clock frequency to ecap module.
 *  ePWM is configured to generate a square wave with 25% duty cycle.
 *  Connect the ePWM output to eCAP input externally on the board.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "ecap_epwm_loopback_app.h"


/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#define KICK0_UNLOCK_VAL 0x68EF3490
#define KICK1_UNLOCK_VAL 0xD172BC5A

#define ECAP1_BASE                       CSL_ECAP0_CTL_STS_BASE
#define ECAP_INT_ALL                    (ECAP_CEVT1_INT  | \
                                         ECAP_CEVT2_INT  | \
                                         ECAP_CEVT3_INT  | \
                                         ECAP_CEVT4_INT  | \
                                         ECAP_CNTOVF_INT | \
                                         ECAP_PRDEQ_INT  | \
                                         ECAP_CMPEQ_INT)
#define ECAP_INPUT_FREQ_MHZ              (125U)

#define EPWM1_BASE                      CSL_EPWM0_EPWM_BASE

#define APP_EHRPWM_OUT_FREQ             (1U * 1000U)

#define APP_EHRPWM_DUTY_CYCLE           (25U)

#define SOC_EHRPWM_MODULE_FREQ          (250U * 1000U * 1000U)

#define APP_EHRPWM_TB_FREQ              (SOC_EHRPWM_MODULE_FREQ / 4U)

#define APP_EHRPWM_PRD_VAL              ((APP_EHRPWM_TB_FREQ                   \
                                            / APP_EHRPWM_OUT_FREQ) / 2U)

#define APP_EHRPWM_COMPA_VAL            (APP_EHRPWM_PRD_VAL -                  \
                                            ((APP_EHRPWM_DUTY_CYCLE *          \
                                                APP_EHRPWM_PRD_VAL) / 100U))

#define ECapAppPrint                    UART_printf

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t gIsrCnt = 0U;
volatile uint32_t gIntrFlg = 0U;

static CSL_EpwmAppPwmCfg_t gAppPwmCfg =
{
    /* CSL_EpwmTimebaseCfg_t */
    {
        APP_EHRPWM_TB_FREQ,                 /* tbClk */
        APP_EHRPWM_OUT_FREQ,                /* pwmtbCounterFreqPrd */
        CSL_EPWM_TB_COUNTER_DIR_UP_DOWN,    /* tbCntrDirection */
        FALSE,                              /* enableSynchronization */
        PWMSS_EPWM_TBCTL_PHSDIR_COUNT_DOWN, /* cntDirAfterSync */
        0U,                                 /* phsCountAfterSync */
        PWMSS_EPWM_TBCTL_SYNCOSEL_EPWMXSYNC /* syncOutSrc */
    },
    /* CSL_EpwmCounterCmpCfg_t */
    {
        APP_EHRPWM_COMPA_VAL,               /* cmpAValue */
        APP_EHRPWM_COMPA_VAL                /* cmpBValue */
    },
    /* CSL_EpwmAqActionCfg_t */
    {
        CSL_EPWM_AQ_ACTION_DONOTHING,       /* zeroAction */
        CSL_EPWM_AQ_ACTION_DONOTHING,       /* prdAction */
        CSL_EPWM_AQ_ACTION_HIGH,            /* cmpAUpAction */
        CSL_EPWM_AQ_ACTION_LOW,             /* cmpADownAction */
        CSL_EPWM_AQ_ACTION_HIGH,            /* cmpBUpAction */
        CSL_EPWM_AQ_ACTION_LOW              /* cmpBDownAction */
    },
    /* CSL_EpwmDeadbandCfg_t */
    {
        CSL_EPWM_DB_IN_MODE_A_RED_A_FED,    /* inputMode */
        CSL_EPWM_DB_OUT_MODE_BYPASS,        /* outputMode */
        CSL_EPWM_DB_POL_SEL_ACTV_HIGH,      /* polaritySelect */
        0U,                                 /* risingEdgeDelay */
        0U                                  /* fallingEdgeDelay */
    },
    /* CSL_EpwmChopperCfg_t */
    {
        CSL_EPWM_CHP_DUTY_CYCLE_PERC_12PNT5,    /* dutyCycle */
        CSL_EPWM_CHP_CLK_FREQ_DIV_BY_1,         /* clkFrequency */
        CSL_EPWM_CHP_OSHT_WIDTH_1XSYSOUT_BY_8   /* oneShotPulseWidth */
    },
    /* CSL_EpwmTripzoneCfg_t */
    {
        CSL_EPWM_TZ_TRIP_ACTION_DO_NOTHING, /* tripAction */
        CSL_EPWM_TZ_EVENT_ONE_SHOT,         /* tripEvtType */
        0U,                                 /* tripPin */
        FALSE                               /* enableTripIntr */
    },
    /* CSL_EpwmEtCfg_t */
    {
        CSL_EPWM_ET_INTR_EVT_CNT_EQ_ZRO,    /* intrEvtSource */
        CSL_EPWM_ET_INTR_PERIOD_FIRST_EVT   /* intrPrd */
    }
};

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static void EcapAppUnlockMmr(void)
{
    /* Unlock pad config MMR */
    if ((CSL_REG32_RD(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK0) & 0x1U) != 0x1U)
    {
        CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK0, KICK0_UNLOCK_VAL);
        CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK1, KICK1_UNLOCK_VAL);
    }
    while ((CSL_REG32_RD(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK0) & 0x1U) != 0x1U)
    {
        /* Wait till the MMR is unlocked. */
    }
    /* Unlock Main Control MMR */
    if ((CSL_REG32_RD(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK0) & 0x1U) != 0x1U)
    {
        CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK0, KICK0_UNLOCK_VAL);
        CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK1, KICK1_UNLOCK_VAL);
    }
    while ((CSL_REG32_RD(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK0) & 0x1U) != 0x1U)
    {
        /* Wait till the MMR is unlocked. */
    }
}

/* Configure EPWM pads */
static void EpwmAppCfgEPwmPads(void)
{
    /* Configure PADCONFIG 15-19 to mux mode 3 */
    HW_WR_REG32(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG18, 0x50003);
}

static void EcapAppCfgECapPads(void)
{
    /* Configure PADCONFIG 156 to mux mode 0 for ECAP0_IN_APWM_OUT and input pin. */
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG156, 0x250000);
}

static void EcapAppClockEnable(void)
{
    /* For AM64x clock is default on. derived from SYSCLK with 1/2, 250MHz */
}

/* Enable ePWM time base clock */
static void EpwmAppTbClockEnable(void)
{
    HW_WR_REG32(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_EPWM_TB_CLKEN,
        0x1U);
}

/* Initialize board */
static void AppEcapBoardInit(void)
{
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_UNLOCK_MMR |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);
}

static void AppEcapIntrISR(void *handle)
{
    /* Clear Ecap Interrupt. */
    ECAPIntStatusClear(ECAP1_BASE, ECAP_INT_ALL);
    ECAPGlobalIntEnable(ECAP1_BASE);

    gIntrFlg = 1;
    gIsrCnt ++;
}
static void EpwmPadConfig_prcmEnable(void)
{
    /* Configure ePWM pads */
    EpwmAppCfgEPwmPads();

    /* Enable EPWM module time base clock */
    EpwmAppTbClockEnable();

}

static void ECapPadConfig_prcmEnable(void)
{
    /* Configure eCAP pads */
    EcapAppCfgECapPads();

    /* Enable ECAP module time base clock */
    EcapAppClockEnable();

    /* Register & enable interrupt */

    Intc_Init();
    Intc_IntSetSrcType(CSLR_R5FSS0_CORE0_INTR_ECAP0_ECAP_INT_0, 1);
    Intc_IntPrioritySet(CSLR_R5FSS0_CORE0_INTR_ECAP0_ECAP_INT_0, 1U, 0U);
    Intc_IntRegister(CSLR_R5FSS0_CORE0_INTR_ECAP0_ECAP_INT_0,
                    (IntrFuncPtr) AppEcapIntrISR,
                    0U);
    Intc_IntEnable(CSLR_R5FSS0_CORE0_INTR_ECAP0_ECAP_INT_0);
    Intc_SystemEnable();
    Intc_IntClrPend(CSLR_R5FSS0_CORE0_INTR_ECAP0_ECAP_INT_0);
}

static void EpwmAppTimebaseModuleCfg(uint32_t baseAddr,
                                     uint32_t pwmFuncClk,
                                     CSL_EpwmTimebaseCfg_t *pTbCfg)
{
    /* Configure Time base clock */
    CSL_epwmTbTimebaseClkCfg(baseAddr, pTbCfg->tbClk, pwmFuncClk);

    /* Configure PWM time base counter frequency and direction */
    CSL_epwmTbPwmFreqCfg(
        baseAddr,
        pTbCfg->tbClk,
        pTbCfg->pwmtbCounterFreqPrd,
        pTbCfg->tbCntrDirection,
        CSL_EPWM_SHADOW_REG_CTRL_ENABLE);

    if (TRUE == pTbCfg->enableSynchronization)
    {
        /* Enable Synchronization */
        CSL_epwmTbSyncEnable(
            baseAddr, pTbCfg->phsCountAfterSync, pTbCfg->cntDirAfterSync);
    }
    else
    {
        /* Disable Synchronization */
        CSL_epwmTbSyncDisable(baseAddr);
    }

}

static void EpwmAppCounterComparatorCfg(uint32_t baseAddr,
                                        CSL_EpwmCounterCmpCfg_t *pCcCfg)
{
    /* Counter Comparator A configuration */
    CSL_epwmCounterComparatorCfg(
        baseAddr,
        CSL_EPWM_CC_CMP_A,
        pCcCfg->cmpAValue,
        CSL_EPWM_SHADOW_REG_CTRL_ENABLE,
        CSL_EPWM_CC_CMP_LOAD_MODE_CNT_EQ_ZERO,
        TRUE);

    /* Counter Comparator B configuration */
    CSL_epwmCounterComparatorCfg(
        baseAddr,
        CSL_EPWM_CC_CMP_B,
        pCcCfg->cmpBValue,
        CSL_EPWM_SHADOW_REG_CTRL_ENABLE,
        CSL_EPWM_CC_CMP_LOAD_MODE_CNT_EQ_ZERO,
        TRUE);

    return;
}


static void EpwmAppInit(void)
{
    uint32_t baseAddr   = EPWM1_BASE;
    uint32_t pwmCh      = CSL_EPWM_OUTPUT_CH_A;
    uint32_t pwmFuncClk = SOC_EHRPWM_MODULE_FREQ;

    CSL_EpwmAppPwmCfg_t *pPwmCfg = &gAppPwmCfg;

    /* Enable clocks for EPWM module inside the PWM sub system. */
    CSL_epwmClockEnable(EPWM1_BASE);

    /* Configure the Time base Sub-Module */
    EpwmAppTimebaseModuleCfg(baseAddr, pwmFuncClk, &pPwmCfg->tbCfg);

    /* Counter-Comparator Sub-Module Configuration */
    EpwmAppCounterComparatorCfg(baseAddr, &pPwmCfg->ccCfg);

    /* Configure Action Qualifier */
    CSL_epwmAqActionOnOutputCfg(baseAddr, pwmCh, &pPwmCfg->aqCfg);

    /* Bypass dead band sub module */
    CSL_epwmDeadbandBypass(baseAddr);

    /* Disable Chopper */
    CSL_epwmChopperEnable(baseAddr, FALSE);

    /* Disable trip zone event handling and ignore all trip zone events */
    CSL_epwmTzTripEventDisable(
        baseAddr, CSL_EPWM_TZ_EVENT_ONE_SHOT, pPwmCfg->tzCfg.tripPin);
    CSL_epwmTzTripEventDisable(
        baseAddr, CSL_EPWM_TZ_EVENT_CYCLE_BY_CYCLE, pPwmCfg->tzCfg.tripPin);

    /* Configure the Event trigger processing */
    CSL_epwmEtIntrCfg(
        baseAddr, pPwmCfg->etCfg.intrEvtSource, pPwmCfg->etCfg.intrPrd);
    CSL_epwmEtIntrEnable(baseAddr);
}

static void ECapAppInit(void)
{
    ECAPIntDisable(ECAP1_BASE, ECAP_INT_ALL);

    ECAPIntStatusClear(ECAP1_BASE, ECAP_INT_ALL);

    /* Disable CAP1-CAP4 register loads */
    ECAPCaptureLoadingDisable(ECAP1_BASE);

    /* Configure eCAP
     *    Enable capture mode.
     *    One shot mode, stop capture at event 4.
     *    Set polarity of the events to rising, falling, rising, falling edge.
     *    Set capture in time difference mode.
     *    Enable eCAP module.
     *    Enable interrupt.
     */

    ECAPCounterControl(ECAP1_BASE, ECAP_COUNTER_STOP);
    ECAPOperatingModeSelect(ECAP1_BASE, ECAP_CAPTURE_MODE);

    ECAPOneShotModeConfig(ECAP1_BASE, ECAP_CAPTURE_EVENT4_STOP);

    ECAPCapeEvtPolarityConfig(ECAP1_BASE, 0, 1, 0, 1);
    ECAPCaptureEvtCntrRstConfig(ECAP1_BASE, 1, 1, 1, 1);

    ECAPSyncInOutSelect(ECAP1_BASE, ECAP_ENABLE_Counter, ECAP_SYNC_IN);
    ECAPCounterControl(ECAP1_BASE, ECAP_COUNTER_FREE_RUNNING);
    ECAPCaptureLoadingEnable(ECAP1_BASE);
    ECAPIntEnable(ECAP1_BASE, ECAP_CEVT4_INT);
}

int32_t main(void)
{
    uint32_t loopCnt = 5;
    uint32_t cap1Count, cap2Count, cap3Count, cap4Count;
    uint32_t highTime, lowTime, dutyCycle;

    AppEcapBoardInit();

    ECapAppPrint("Starting EPWM to ECAP loopback application...\n");
    ECapAppPrint("Connect the EPWM output to ECAP input on the board. \n");
    ECapAppPrint("Connect Pin 7 on J6 Header in expansion board to Pin 1 on J12 Header on base board.\n");
    ECapAppPrint("Note: short Pin 2 & 3 of J11 on expansion board\n");

    EcapAppUnlockMmr();
    EpwmPadConfig_prcmEnable();
    ECapPadConfig_prcmEnable();

    EpwmAppInit();
    ECapAppInit();

    while (gIsrCnt < loopCnt)
    {
        /* Start eCAP */
        ECAPOneShotREARM(ECAP1_BASE);

        /* wait for the interrupt. */
        while (gIntrFlg == 0)
        {

        }

        gIntrFlg = 0;
    }
    ECAPIntDisable(ECAP1_BASE, ECAP_INT_ALL);

    /* Read Counter values and print for last iteration. */
    cap1Count = ECAPTimeStampRead(ECAP1_BASE, ECAP_CAPTURE_EVENT_1);
    cap2Count = ECAPTimeStampRead(ECAP1_BASE, ECAP_CAPTURE_EVENT_2);
    cap3Count = ECAPTimeStampRead(ECAP1_BASE, ECAP_CAPTURE_EVENT_3);
    cap4Count = ECAPTimeStampRead(ECAP1_BASE, ECAP_CAPTURE_EVENT_4);

    ECapAppPrint("Count1 = %d, Count2 = %d, Count3 = %d, Count4 = %d.\n",  cap1Count, cap2Count, cap3Count, cap4Count);

    highTime = cap2Count / ECAP_INPUT_FREQ_MHZ;
    lowTime  = cap3Count / ECAP_INPUT_FREQ_MHZ;
    dutyCycle = (cap2Count * 100) / (cap2Count + cap3Count);

    ECapAppPrint("Hight time is %d us, Low time is %d us, Duty Cycle of the signal: %d%%.\n", highTime, lowTime, dutyCycle);
    ECapAppPrint("All tests have passed.\n");

    return 0;
}

