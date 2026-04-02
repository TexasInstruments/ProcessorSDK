/*
 * WWDT Example
 *
 * Windowed Watchdog Timer (WWDT) Example Application
 *
 *  Copyright (c) 2020 Texas Instruments Incorporated
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
 *  \file main_r5f_mcu2_0_perm_fifty.c
 *
 *  \brief This file demonstrates using the Windowed Watchdog Timer (WWDT) to
 *         induce a permanent service failure with 50% service window,
 *         utilizing the WWDT Software Diagnostic Reference (SDR) functions.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>

/* For Timer functions */
#include <ti/osal/osal.h>

/* For Mailbox functions */
#include <ti/csl/csl_mailbox.h>

/* Diagnostic example utility functions, e.g. print outputs */
#include <diag_utils.h>

#include <ti/csl/hw_types.h>
#include <ti/csl/csl_rti.h>
#include <ti/csl/csl_esm.h>
#include <ti/csl/soc/cslr_soc_intr.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_timer.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Usecase-specific macros */
#define FIFTY_WINDOW_SIZE

#if defined(FIFTY_WINDOW_SIZE)
#define WWDT_WINDOW           (RTI_RTIDWWDSIZECTRL_DWWDSIZE_50_PERCENT)
#else
#define WWDT_WINDOW           (RTI_RTIDWWDSIZECTRL_DWWDSIZE_100_PERCENT)
#endif

/* Specifies how many times the WWDT should be serviced during the usecase before
   inducing a usecase-specific condition */
#define FEED_ITER_PRECOUNT    (20U)

#define WWDT_MAIN_TIMER_ID_0  (0U)

/* Specifies service interval for normal watchdog feeding */
#define TIMER_PERIOD_USECS    (400U)
#define TIMER_EARLY_USECS     (100U)
#define TIMER_LATE_USECS      (700U)

#define INTR_NUM_12           (CSLR_R5FSS0_CORE0_INTR_TIMER12_INTR_PEND_0)

#define MAX_BUFF_LEN          (4096U)

#define MAILBOX_BASE_ADDRESS  CSL_NAVSS0_MAILBOX_REGS0_BASE

/* Preload of 0U corresponds to a ~682us WWDT period */
#define WWDT_PRELOAD          (0U)

/* Additional compile-time macro used below:
 * UART_PRINTF - Prints test execution state and errors to UART when enabled
 */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void feedWatchdogTimerIntr(void);
int32_t WWDT_Example_WwdtInit(void);
void waitForMCUDone(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Handle for the timer that triggers normal WWDT servicing */
volatile TimerP_Handle serviceTimerHandle;

/* Indicates how many times timer ISRs have been triggered during usecase */
volatile uint32_t iterCount = 0U;

/* Indicates whether an additional FEED_ITER_POSTCOUNT services should be
   skipped */
volatile uint32_t skipMultipleServices = 0U;

/* Buffers all desired text to be printed after all usecases complete, except
   for bad inits */
char print_buf[MAX_BUFF_LEN];

/* Pointer to be written to next inside print_buf */
char* volatile print_ptr = print_buf;

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


/*****************************************************************************
 * This is the main function in the Windowed Watchdog (WWDT) example
 * application that services the windowed watchdog on the main domain core.
 * It runs through the permanent failure (windowed) usecase (UC-2) to
 * demonstrate usage of the WWDT module for detecting and handling permanent
 * failures (i.e. writing key completely stops in windowed mode).
 */
int32_t main(void)
{
    int32_t init_status;
    TimerP_Params serviceTimerParams;
    TimerP_Status status;

#ifdef UART_PRINTF
    /* Initialize UART instance */
    DIAG_uartStdioMainInit();
#endif

    TimerP_Params_init(&serviceTimerParams);

    /* Initialize Main Timer 1 */
    serviceTimerParams.periodType = TimerP_PeriodType_MICROSECS;
    serviceTimerParams.startMode = TimerP_StartMode_USER;
    serviceTimerParams.runMode = TimerP_RunMode_ONESHOT;
    serviceTimerParams.period = TIMER_PERIOD_USECS;
    serviceTimerParams.timerMode = TimerP_Timer64Mode_UNCHAINED;
    serviceTimerParams.intNum     = INTR_NUM_12;

    serviceTimerHandle = TimerP_create(WWDT_MAIN_TIMER_ID_0,
                            (TimerP_Fxn)&feedWatchdogTimerIntr,
                            &serviceTimerParams);
    /* don't expect the handle to be null */
    if (0U == serviceTimerHandle)
    {
        DIAG_printf("\nWWDT Example: Service Timer Create error \n");
    }

    /* Gate until MCU WWDT init portion done */
    waitForMCUDone();

    init_status = WWDT_Example_WwdtInit();
    if (0 != init_status)
    {
        /* print error and quit */
        DIAG_printf("\nWWDT Example: WWDT init failed! \n");
    }

    status = TimerP_start(serviceTimerHandle);

    if (TimerP_OK != status)
    {
        DIAG_printf("\nWWDT Example: Service Timer start error \n");
    }

    /* Wait for the MCU application to finish servicing the expected single
        ESM interrupt */
    waitForMCUDone();

#ifdef UART_PRINTF
    DIAG_printf("%s", print_buf);
#endif

    return 0;
}

/*****************************************************************************
 * This is the feeding timer interrupt of the watchdog example; it determines
 * when to service the watchdog as normal as well as generating
 * usecase-specific conditions by adjusting service timing, window size, or
 * number of consecutive service misses.
 */
void feedWatchdogTimerIntr(void)
{
    TimerP_Status status;

    if ((FEED_ITER_PRECOUNT > iterCount)
#ifdef RESUME_SERVICE
           || (FEED_ITER_PRECOUNT < iterCount)
#endif
           )
    {
        RTIDwwdService(CSL_RTI28_CFG_BASE);

        if (FEED_ITER_PRECOUNT < iterCount)
        {
#ifdef EARLY_SERVICE
            status = TimerP_setPeriodMicroSecs(serviceTimerHandle, TIMER_PERIOD_USECS);
            if (TimerP_OK != status)
            {
                print_ptr += sprintf(print_ptr,
                                "\nService Timer set period error \n");
            }
#endif
#ifdef LATE_SERVICE
            status = TimerP_setPeriodMicroSecs(serviceTimerHandle, TIMER_PERIOD_USECS);
            if (TimerP_OK != status)
            {
                print_ptr += sprintf(print_ptr,
                                "\nService Timer set period error \n");
            }
#endif
        }

        status = TimerP_start(serviceTimerHandle);
        if (TimerP_OK != status)
        {
            print_ptr += sprintf(print_ptr,"\nService Timer start error \n");
        }
    }
#ifdef RESUME_SERVICE
    else
    {
#ifdef EARLY_SERVICE
        RTIDwwdService(CSL_RTI28_CFG_BASE);

        status = TimerP_setPeriodMicroSecs(serviceTimerHandle, TIMER_EARLY_USECS);
        if (TimerP_OK != status)
        {
            print_ptr += sprintf(print_ptr,
                            "\nService Timer set period error \n");
        }
#endif
#ifdef LATE_SERVICE
        RTIDwwdService(CSL_RTI28_CFG_BASE);

        status = TimerP_setPeriodMicroSecs(serviceTimerHandle, TIMER_LATE_USECS);
        if (TimerP_OK != status)
        {
            print_ptr += sprintf(print_ptr,
                            "\nService Timer set period error \n");
        }
#endif
        status = TimerP_start(serviceTimerHandle);
        if (TimerP_OK != status)
        {
            print_ptr += sprintf(print_ptr,"\nService Timer start error \n");
        }
    }
#endif

    iterCount++;
}


/*********************************************************************
* @fn      WWDT_Example_WwdtInitHandlerInit
*
* @brief   Set WWDT clock and config
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t WWDT_Example_WwdtInit(void)
{
    int32_t result = 0;
    uint32_t preloadValue;
    int32_t cslStatus = CSL_PASS;

    /* Preload of 0 corresponds to ~682us with 12MHz clk */
    preloadValue = 0U;

    /* Configure Watchdog */
    cslStatus = RTIDwwdWindowConfig(CSL_RTI28_CFG_BASE,
                                    RTI_DWWD_REACTION_GENERATE_NMI,
                                    preloadValue,
                                    WWDT_WINDOW);
    if (cslStatus !=  CSL_PASS)
    {
        result = -1;
    }

    if (result == 0)
    {
        cslStatus =  RTIDwwdWindowVerifyConfig(CSL_RTI28_CFG_BASE,
                                               RTI_DWWD_REACTION_GENERATE_NMI,
                                               preloadValue,
                                               WWDT_WINDOW);
        if (cslStatus !=  CSL_PASS)
        {
            result = -1;
        }
    }

    RTIDwwdCounterEnable(CSL_RTI28_CFG_BASE);

    return result;
}


/*********************************************************************
* @fn      waitForMCUDone()
*
* @brief   Poll mailbox until message received from MCU1_0
*
* @param   None
*
* @return    None
*/

void waitForMCUDone(void)
{
    uint32_t gMsgStatus = MESSAGE_INVALID;
    uint32_t gMsg;

    while (gMsgStatus == MESSAGE_INVALID)
    {
        gMsgStatus =
            MailboxGetMessage(MAILBOX_BASE_ADDRESS, MAILBOX_QUEUE_0,
                                (uint32_t *) &gMsg);
    }
}
