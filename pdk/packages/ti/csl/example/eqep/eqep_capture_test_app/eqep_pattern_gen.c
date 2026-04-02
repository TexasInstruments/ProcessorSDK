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
 *  \file     eqep_pattern_gen.c
 *
 *  \brief    This file contains eQEP waveform generation.
 *            This generated the eQEP signal by toggeling the gpio signals.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "eqep_pattern_gen.h"

/* ========================================================================== */
/*                          Macros And Typedefs                               */
/* ========================================================================== */
#define EQEP_A_GPIO_PIN             (26U)
#define EQEP_B_GPIO_PIN             (27U)
#define EQEP_S_GPIO_PIN             (43U)
#define EQEP_I_GPIO_PIN             (44U)

#define EQEP_PINS_GPIO_BASE         (CSL_GPIO0_BASE)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* Synchronization between main task and pattern generator. */
extern volatile uint32_t    gTestExit;
extern volatile uint32_t    gStartEqepSignal;
extern EqepAppPatternParams gPattern;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static void time_delay_usec(uint32_t usec);
static void generateEqepPattern (EqepAppPatternParams *patParam);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void time_delay_usec(uint32_t usec)
{
    uint64_t startTime;
    startTime = TimerP_getTimeInUsecs();
    while ((TimerP_getTimeInUsecs() - startTime) < usec)
    {
        /* Do nothing. */
    }
}

static void generateEqepPattern (EqepAppPatternParams *patParam)
{
    const uint32_t gpio_base = EQEP_PINS_GPIO_BASE;
    uint32_t clockDelayCnt;
    uint32_t i, j;

    /* Configure all GPIO pins as output for generating EQEP Signals. */
    GPIOSetDirMode_v0(gpio_base, EQEP_A_GPIO_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOSetDirMode_v0(gpio_base, EQEP_B_GPIO_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOSetDirMode_v0(gpio_base, EQEP_S_GPIO_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOSetDirMode_v0(gpio_base, EQEP_I_GPIO_PIN, GPIO_DIRECTION_OUTPUT);

    /* delay required for each quadrature of the EQEP signal in usec. */
    clockDelayCnt = (1000000 / patParam->eqepClockFreq) / 4;

    for (i = 0; i< patParam->idxEvtCnt; i++)
    {
        for (j =0; j< patParam->loopCnt; j++)
        {
            if (patParam->direction == 1)
            {
                /* Configure all EQEP A and B lines to generate clockwise rotation. */
                GPIOPinWrite_v0(gpio_base, EQEP_A_GPIO_PIN, GPIO_PIN_LOW);
                GPIOPinWrite_v0(gpio_base, EQEP_B_GPIO_PIN, GPIO_PIN_HIGH);
                time_delay_usec(clockDelayCnt);

                GPIOPinWrite_v0(gpio_base, EQEP_A_GPIO_PIN, GPIO_PIN_LOW);
                GPIOPinWrite_v0(gpio_base, EQEP_B_GPIO_PIN, GPIO_PIN_LOW);
                time_delay_usec(clockDelayCnt);

                GPIOPinWrite_v0(gpio_base, EQEP_A_GPIO_PIN, GPIO_PIN_HIGH);
                GPIOPinWrite_v0(gpio_base, EQEP_B_GPIO_PIN, GPIO_PIN_LOW);
                time_delay_usec(clockDelayCnt);

                GPIOPinWrite_v0(gpio_base, EQEP_A_GPIO_PIN, GPIO_PIN_HIGH);
                GPIOPinWrite_v0(gpio_base, EQEP_B_GPIO_PIN, GPIO_PIN_HIGH);
                time_delay_usec(clockDelayCnt);
            }
            else
            {
                /* Configure all EQEP A and B lines to generate anti clockwise rotation. */
                GPIOPinWrite_v0(gpio_base, EQEP_A_GPIO_PIN, GPIO_PIN_LOW);
                GPIOPinWrite_v0(gpio_base, EQEP_B_GPIO_PIN, GPIO_PIN_HIGH);
                time_delay_usec(clockDelayCnt);

                GPIOPinWrite_v0(gpio_base, EQEP_A_GPIO_PIN, GPIO_PIN_HIGH);
                GPIOPinWrite_v0(gpio_base, EQEP_B_GPIO_PIN, GPIO_PIN_HIGH);
                time_delay_usec(clockDelayCnt);

                GPIOPinWrite_v0(gpio_base, EQEP_A_GPIO_PIN, GPIO_PIN_HIGH);
                GPIOPinWrite_v0(gpio_base, EQEP_B_GPIO_PIN, GPIO_PIN_LOW);
                time_delay_usec(clockDelayCnt);

                GPIOPinWrite_v0(gpio_base, EQEP_A_GPIO_PIN, GPIO_PIN_LOW);
                GPIOPinWrite_v0(gpio_base, EQEP_B_GPIO_PIN, GPIO_PIN_LOW);
                time_delay_usec(clockDelayCnt);
            }
        }
        if (patParam->generateIdxPulse == 1)
        {
            /* Generate the Index pulse by toggeling EQEP I signal. */
            GPIOPinWrite_v0(gpio_base, EQEP_I_GPIO_PIN, GPIO_PIN_HIGH);
            time_delay_usec(clockDelayCnt);
            GPIOPinWrite_v0(gpio_base, EQEP_I_GPIO_PIN, GPIO_PIN_LOW);
        }
    }
}

Void eqepPatternGenerator(UArg a0, UArg a1)
{
    while (gTestExit == 0)
    {
        if (gStartEqepSignal == 0)
        {
            Task_yield();
        }
        else
        {
            gStartEqepSignal = 0;
            generateEqepPattern(&gPattern);
        }
    }
}
