/*
 *  Copyright (c) Texas Instruments Incorporated 2025
 *  All rights reserved.
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
 *  \file    gtc_timer_test.c
 *
 *  \brief  Example application main file that tests the GTC timer functionality
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ti/drv/gtc/gtc.h>
#include <ti/osal/osal.h>
#include <ti/csl/soc.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>
#include <ti/csl/arch/csl_arch.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define APP_TSK_STACK_MAIN              (0x8000U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Test GTC timer by initialising it and then reading the counter value .
 * Returns CSL_PASS if successful else returns CSL_EFAIL*/
void GTC_timerTest(void* arg0, void* arg1)
{
    int32_t retVal = CSL_EFAIL;
    uint64_t timerStart = 0;
    uint64_t timerEnd = 0;
    uint64_t timerDiff = 0;
    uint64_t setCounter = 0xDEADFADEABCDEFEF;
    uint32_t ticks = 2000;

    Sciclient_ConfigPrms_t config;
    Board_STATUS  status;
    Board_initCfg boardCfg;


    /* Init UART */
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    status = Board_init(boardCfg);
    DebugP_assert(BOARD_SOK == status);

    /*Sciclient init */
    Sciclient_configPrmsInit(&config);
    retVal = Sciclient_init(&config);

    /* Init GTC */
    if(CSL_PASS == retVal){
        retVal = GTC_init();
    }

    if(CSL_PASS == retVal){

        GTC_setCounter64(setCounter);
        UART_printf("Starting GTC Counter \n");
        UART_printf("GTC frequency : 200 MHz \n");
        GTC_enable();
        timerStart = GTC_readCounter64();
        /*Osal Delay for 1000 ticks */
        Osal_delay(ticks);
        timerEnd = GTC_readCounter64();
        timerDiff = (timerEnd - timerStart);
        UART_printf("GTC timer diff for %d ticks(2s): %d \n",ticks,(uint32_t)timerDiff);
        if((uint32_t)timerDiff >= (ticks-1000)){
            UART_printf("All tests have passed \n"); 
        }
        else{
            UART_printf("All tests have failed \n");
        }
    }
    else{
        UART_printf("GTC init failed \n");
    }

}

/*-------------------------------------------------------------------------- */

int main()
{
    TaskP_Params taskParams;

    OS_init();

    TaskP_Params_init(&taskParams);

    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    TaskP_create(&GTC_timerTest, &taskParams);

    OS_start();
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */
