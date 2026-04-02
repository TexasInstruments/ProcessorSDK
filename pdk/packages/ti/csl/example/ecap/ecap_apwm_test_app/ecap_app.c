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
 *  \file     ecap_app.c
 *
 *  \brief    This file contains eCAP test application
 *  The ecap module is configured in the apwm mode and the ecap device pin is
 *  configured as output pin. Configure the ecap to generate a 50% duty cycle
 *  square wave. Update the frequency of the output signal.
 *  The output can be measured externally by an osciloscope.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "ecap_app.h"


/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#define KICK0_UNLOCK_VAL 0x68EF3490
#define KICK1_UNLOCK_VAL 0xD172BC5A

#define ECAP1_BASE                                      CSL_ECAP0_CTL_STS_BASE
#define ECAP_INT_ALL                                   (ECAP_CEVT1_INT  | \
                                                        ECAP_CEVT2_INT  | \
                                                        ECAP_CEVT3_INT  | \
                                                        ECAP_CEVT4_INT  | \
                                                        ECAP_CNTOVF_INT | \
                                                        ECAP_PRDEQ_INT  | \
                                                        ECAP_CMPEQ_INT)

#define DECREASE_FREQUENCY                              (0U)
#define INCREASE_FREQUENCY                              (1U)

#define ECAP_PERIOD_COUNT                               (200U)
#define ECAP_PERIOD_DIFF_COUNT                          (20U)

#define ECapAppPrint                                    UART_printf

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static void EcapAppCfgECapPads(void)
{
    /* Unlock all MMR */
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK0_KICK0, KICK0_UNLOCK_VAL);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK0_KICK1, KICK1_UNLOCK_VAL);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK0, KICK0_UNLOCK_VAL);
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK1, KICK1_UNLOCK_VAL);

    /* Configure PADCONFIG 156 to mux mode 0 for ECAP0_IN_APWM_OUT and output pin. */
    CSL_REG32_WR(CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG156, 0x010000);
}

static void EcapAppClockEnable(void)
{
    /* For AM64x clock is default on. derived from SYSCLK with 1/4, 125MHz */
}

static void padConfig_prcmEnable(void)
{
    /* Configure eCAP pads */
    EcapAppCfgECapPads();

    /* Enable ECAP module time base clock */
    EcapAppClockEnable();
}

static void ECapAppInit(void)
{
    ECAPIntDisable(ECAP1_BASE, ECAP_INT_ALL);

    ECAPIntStatusClear(ECAP1_BASE, ECAP_INT_ALL);

    /* Disable CAP1-CAP4 register loads */
    ECAPCaptureLoadingDisable(ECAP1_BASE);

    /* Configure eCAP in apwm mode. */
    ECAPCounterControl(ECAP1_BASE, ECAP_COUNTER_STOP);
    ECAPOperatingModeSelect(ECAP1_BASE, ECAP_APWM_MODE);

    /* Load the PWM Counter values for 50% duty cycle signal. */
    ECAPAPWMCaptureConfig(ECAP1_BASE, (ECAP_PERIOD_COUNT / 2), ECAP_PERIOD_COUNT);
    ECAPAPWMShadowCaptureConfig(ECAP1_BASE, (ECAP_PERIOD_COUNT / 2), ECAP_PERIOD_COUNT);

    ECAPCounterControl(ECAP1_BASE, ECAP_COUNTER_FREE_RUNNING);
}

int32_t main(void)
{
    int32_t  loopCnt = 100000;
    uint32_t cap1Count, cap1CountUpdate = ECAP_PERIOD_COUNT;
    uint32_t direction;
    volatile uint32_t update_freq = 0;
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    ECapAppPrint("\nStarting ECAP test application...\n");
    ECapAppPrint("Probe the Pin 1 on J12 Header on base board to get square wave of varying frequency.\n");

    padConfig_prcmEnable();

    ECapAppInit();

    /* Change the frequence. */
    while (loopCnt > 0)
    {
        cap1Count = ECAPTimeStampRead(ECAP1_BASE, ECAP_CAPTURE_EVENT_1);

        if (cap1Count == cap1CountUpdate)
        {
            /* Updated new value from shadow register. */
            /* TODO: Add timeout based break. */
            loopCnt--;
            if (update_freq != 0U)
            {
                /* change update_freq = 1 to test changing the apwm frequency. */
                if(cap1Count >= ECAP_PERIOD_COUNT)
                {
                direction = INCREASE_FREQUENCY;
                }
                else if (cap1Count <= (ECAP_PERIOD_COUNT / 2))
                {
                direction = DECREASE_FREQUENCY;
                }

                if(direction == INCREASE_FREQUENCY)
                {
                    cap1CountUpdate = cap1Count - ECAP_PERIOD_DIFF_COUNT;
                }
                else
                {
                    cap1CountUpdate = cap1Count + ECAP_PERIOD_DIFF_COUNT;
                }

                /* Update the frequency in shadow register with 50% duty cycle. */
                ECAPAPWMShadowCaptureConfig(ECAP1_BASE, (cap1CountUpdate >> 1U), cap1CountUpdate);
            }
        }
    }
    ECapAppPrint("All tests have passed.\n");
    return 0;
}
