/*
 *   Copyright (c) Texas Instruments Incorporated 2018 - 2020
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
 *  \file     rti_app_dwwd.c
 *
 *  \brief    This file contains RTI DWWD test code for window size 100%.
 *
 *  \details  RTI Instance selected depends on the Core used for test
 *            MCU will use MCU RTI1 and MPU will use MAIN RTI0
 *            RTI clock source is selected as 32KHz.
 *            DWWD window size is set to 100%.
 *            DWWD reaction after expiry is set to interrupt mode.
 *            DWWD preload value is set for 10sec.
 *            DWWD time-out value is specified in "APP_RTI_DWWD_TIMEOUT_VALUE"
 *            macro in mili-seconds.
 *            DWWD will expire in 10sec and generate interrupt to Core.
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/hw_types.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/csl_rti.h>
#include <ti/csl/soc.h>
#include <ti/board/board.h>
#include <ti/osal/osal.h>
#ifdef WDT_RESET
#include <ti/drv/sciclient/sciclient.h>
#endif
#ifdef LINUX_HOST_PROC_WDT_CONFIG
#include <ti/csl/example/rti/rti_dwwdtest_app/rsc_table_basic.h>
#endif

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

#include "rti_app_dwwd.h"

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#define APP_NAME                       "CSL RTI APP"
/**< CSL Sample App Name */


/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */
/**
 * \brief  Enum to select the clock source for RTI module.
 */
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

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
volatile uint32_t isrFlag = 0U;
/**< Flag used to indicate interrupt is generated */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
/* Unity test app function */
void test_csl_rti_dwwd_test_app(void);
/* Unity test app runner function */
void test_csl_rti_dwwd_test_app_runner(void);

#ifndef WDT_RESET
/**
 * \brief   This API to select clock source for RTI module.
 *
 *
 * \param   rtiClockSource  RTI module clock source
 *                          Values given by enum #rtiClockSource_t
 *
 * \return  none.
 */
static void RTISetClockSource(uint32_t rtiClockSourceSelect);

/**
 * \brief   This API to calculate preload value from given time-out value.
 *
 * \param   rtiClockSource  RTI module clock source
 *                          Values given by enum #rtiClockSource_t
 *
 * \param   timeoutVal      RTI DWWD time-out value in mili-seconds.
 *
 * \return  Preload value   Time-out value in RTI source clock cycles.
 */
static uint32_t RTIGetPreloadValue(uint32_t rtiClkSource, uint32_t timeoutVal);
#endif

/**
 * \brief   This API is used for app prints.
 *
 * \param   None.
 *
 * \return  None.
 */
static void RTIAppUtilsPrint (const char *pcString, ...);
/**
 * \brief   This API to register interrupt for a given instance.
 *
 *
 * \return  0: Success, -1: Failure
 */
static int32_t RTIInterruptConfig(void);

/**
 * \brief   ISR after interrupt generation, sets global flag
 *
 * \retval  None
 */
static void RTIAppISR(uintptr_t handle);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#ifdef UNITY_INCLUDE_CONFIG_H
/*
 *  ======== Unity set up and tear down ========
 */
void setUp(void)
{
    /* Do nothing */
}

void tearDown(void)
{
    /* Do nothing */
}
#endif

void test_csl_rti_dwwd_test_app(void)
{
    /* @description: Test that runs a basic functionality test of WDT

       @requirements: PRSDK-5575

       @cores: mpu1_0 */

#ifdef LINUX_HOST_PROC_WDT_CONFIG

    Board_initCfg   boardCfg;

    boardCfg = BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    /* Register Interrupt */
    isrFlag = 0U;
    if(RTIInterruptConfig(APP_RTI_MODULE) != 0)
    {
        RTIAppUtilsPrint("\r\nRTI Interrupt configuration failed. \r\n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_FAIL();
#endif
        return;
    }
    RTIAppUtilsPrint(APP_NAME ": RTI App WDT timeout interrupt configured.\n");
    while (0U == isrFlag)
    {
        /* Wait for interrupt */
    }
#else
    uint32_t rtiModule, rtiWindow_size, rtiPreload_value, rtiReaction;
    int32_t  config_status;
#ifndef DISABLE_UART_PRINT
    Board_initCfg   boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO |
               BOARD_INIT_UNLOCK_MMR;
    Board_init(boardCfg);
#endif
    RTIAppUtilsPrint("\n Board init complete\n");

    /* Register Interrupt */
    isrFlag = 0U;
    if(RTIInterruptConfig() != 0)
    {
        RTIAppUtilsPrint("\r\nRTI Interrupt configuration failed. \r\n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_FAIL();
#endif
        return;
    }
    RTIAppUtilsPrint("\n Interrupt config complete\n");

    /* Configure RTI parameters */
    rtiModule        = RTI_APP_RTI_CFG_BASE;
    rtiWindow_size   = APP_RTI_DWWD_WINDOW_SIZE;
    rtiReaction      = APP_RTI_DWWD_REACTION;
    rtiPreload_value = RTIGetPreloadValue((uint32_t) RTI_CLOCK_SOURCE_32KHZ,
                                          (uint32_t) APP_RTI_DWWD_TIMEOUT_VALUE);

    /* Select RTI module clock source */
    RTISetClockSource((uint32_t) RTI_CLOCK_SOURCE_32KHZ);
    config_status = RTIDwwdWindowConfig(rtiModule, rtiReaction,
                                        rtiPreload_value,
                                        rtiWindow_size);
    if (config_status == CSL_EFAIL)
    {
        RTIAppUtilsPrint(APP_NAME ": Error during Window configuration.\n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_FAIL();
#endif
    }
    else
    {
        RTIAppUtilsPrint(APP_NAME ": DWWD is configured for %u ms time-out \n",
            APP_RTI_DWWD_TIMEOUT_VALUE);
        RTIAppUtilsPrint(APP_NAME ": DWWD will generate interrupt after "
            "above time-out period.\n");
        RTIDwwdCounterEnable(rtiModule);
        /* Let DWWD expire here */
        RTIAppUtilsPrint(APP_NAME ": Wait for %u ms for interrupt "
            "to be generated by DWWD.\n", APP_RTI_DWWD_TIMEOUT_VALUE);

        while (0U == isrFlag)
        {
            /* Wait for interrupt */
        }
        RTIAppUtilsPrint(APP_NAME ": RTI App completed successfully.\n");
        UART_printStatus("\n All tests have passed. \n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_PASS();
#endif
    }
#endif
}
void test_csl_rti_dwwd_test_app_runner(void)
{
    /* @description: Test runner for WDT tests

       @requirements: PRSDK-5575, PDK-6053, PDK-6039

       @cores: mpu1_0 */

#ifdef UNITY_INCLUDE_CONFIG_H
    UNITY_BEGIN();
    RUN_TEST(test_csl_rti_dwwd_test_app);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_csl_rti_dwwd_test_app();
#endif
}

int main(void)
{
    test_csl_rti_dwwd_test_app_runner();
    return 0;
}

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */
static void RTIAppUtilsPrint (const char *pcString, ...)
{
    static char printBuffer[2000U];
    va_list arguments;

    /* Start the varargs processing. */
    va_start(arguments, pcString);
    vsnprintf (printBuffer, sizeof(printBuffer), pcString, arguments);
#ifdef DISABLE_UART_PRINT
    printf(printBuffer);
#else
    UART_printf(printBuffer);
#endif
    /* End the varargs processing. */
    va_end(arguments);

    return;
}

#ifndef LINUX_HOST_PROC_WDT_CONFIG
static void RTISetClockSource(uint32_t rtiClockSourceSelect)
{
    volatile uint32_t *hwRegPtr;

    hwRegPtr = (uint32_t *) (RTI_APP_CTRL_MMR_CFG_BASE + RTI_APP_CTRL_MMR_CLKSEL_OFFSET);
    hwRegPtr[0] = (hwRegPtr[0] & (~RTI_APP_RTI_CLK_SEL_FIELD_MASK)) | (rtiClockSourceSelect << RTI_APP_RTI_CLK_SEL_FIELD_SHIFT);
}
#endif

static int32_t RTIInterruptConfig(void)
{
    int32_t retVal = 0;
    OsalRegisterIntrParams_t intrPrms;
    OsalInterruptRetCode_e osalRetVal;
    HwiP_Handle hwiHandle;
    uint32_t rtiIntNum;

    Osal_RegisterInterrupt_initParams(&intrPrms);
    intrPrms.corepacConfig.arg          = (uintptr_t)0;
    intrPrms.corepacConfig.priority     = 1U;
    intrPrms.corepacConfig.corepacEventNum = 0U; /* NOT USED ? */

    rtiIntNum = RTI_APP_WDT_INT_NUM;

    if (retVal == 0)
    {
        intrPrms.corepacConfig.isrRoutine   = &RTIAppISR;
        intrPrms.corepacConfig.intVecNum    = rtiIntNum;

        osalRetVal = Osal_RegisterInterrupt(&intrPrms, &hwiHandle);
        if(OSAL_INT_SUCCESS != osalRetVal)
        {
            RTIAppUtilsPrint(APP_NAME ": Error Could not register ISR !!!\n");
            retVal = -1;
        }
    }

    return retVal;
}

#ifndef LINUX_HOST_PROC_WDT_CONFIG
static uint32_t RTIGetPreloadValue(uint32_t rtiClkSource, uint32_t timeoutVal)
{
    uint32_t clkFreqKHz       = (uint32_t) RTI_CLOCK_SOURCE_32KHZ_FREQ_KHZ,
             timeoutNumCycles = 0;

    switch (rtiClkSource)
    {
        case RTI_CLOCK_SOURCE_32KHZ:
            clkFreqKHz = (uint32_t) RTI_CLOCK_SOURCE_32KHZ_FREQ_KHZ;
            break;
        default:
            break;
    }
    /* Get the clock ticks for given time-out value */
    timeoutNumCycles = timeoutVal * clkFreqKHz;
    return timeoutNumCycles;
}
#endif

static void RTIAppISR(uintptr_t handle)
{
    uint32_t intrStatus;
#ifdef WDT_RESET
    int32_t status;
#endif

    RTIDwwdGetStatus(RTI_APP_RTI_CFG_BASE, &intrStatus);
    RTIDwwdClearStatus(RTI_APP_RTI_CFG_BASE, intrStatus);
    RTIAppUtilsPrint(APP_NAME ": Interrupt Generated!!!\n");
#ifdef WDT_RESET
    status = Sciclient_pmDeviceReset(SCICLIENT_SERVICE_WAIT_FOREVER);

    if (status != 0)
    {
        RTIAppUtilsPrint(APP_NAME ": Sciclient_pmDeviceReset Failed : stauts = %d!!!\n", status);
    }
#endif
    isrFlag  = 1U;
}
/********************************* End of file ******************************/
