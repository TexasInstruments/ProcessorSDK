/**
 *   Copyright (c) Texas Instruments Incorporated 2018
 *   All rights reserved.
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
 */

/**
*  \file vhwa_examples_common.c
*
*  \brief Routines common to all VHWA applications
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdio.h>
#include <stdint.h>
#include <examples/include/vhwa_examples_common.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* Print buffer character limit for prints- UART or CCS Console */
#define APP_PRINT_BUFFER_SIZE                   ((uint32_t)2000)
/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
char gPrintBuffer[APP_PRINT_BUFFER_SIZE];

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Vhwa_print(const char *format, ...)
{
    va_list arguments;

    /* Start the var args processing. */
    va_start(arguments, format);
    vsnprintf(gPrintBuffer, sizeof(gPrintBuffer), format, arguments);
    #if defined(VHWA_MCU_PLUS_SDK)
    DebugP_log("%s", gPrintBuffer);
    #elif defined (VHWA_PDK)
    App_print("%s", gPrintBuffer);
    #endif
    /* End the var args processing. */
    va_end(arguments);
}
uint64_t Vhwa_getTime(void)
{
    #if defined(VHWA_MCU_PLUS_SDK)
    return ClockP_getTimeUsec();
    #elif defined(VHWA_PDK)
    return App_getTimerTicks();
    #endif
}

uint64_t Vhwa_getTimeUsec(void)
{
    #if defined(VHWA_MCU_PLUS_SDK)
    return ClockP_getTimeUsec();
    #elif defined(VHWA_PDK)
    return TimerP_getTimeInUsecs();
    #endif
}

#if !defined (VHWA_MCU_PLUS_SDK)
void App_print(const char *format, ...)
{
    va_list arguments;

    /* Start the var args processing. */
    va_start(arguments, format);
    vsnprintf (gPrintBuffer, sizeof(gPrintBuffer), format, arguments);

    UART_printf(gPrintBuffer);

    /* End the var args processing. */
    va_end(arguments);
}

void App_startTimer(void)
{
    /* Configure GTC Timer - running at 250MHz as per config and default mux mode */
    /* 250 MHz depends on 'MCU_PLL1' and is selected through 'GTCCLK_SEL' mux */
    /* Enable GTC */
    HW_WR_REG32(CSL_GTC0_GTC_CFG1_BASE + 0x0U, 0x1);
}

uint64_t App_getTimerTicks(void)
{
    return (*((uint64_t *)(CSL_GTC0_GTC_CFG1_BASE + 0x8U)));
}
void App_udmaPrint(const char *str)
{
    Vhwa_print (str);

    return;
}
#endif