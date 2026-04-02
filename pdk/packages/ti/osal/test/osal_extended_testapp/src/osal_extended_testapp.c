/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   osal_extended_testapp.c
 *
 *  \brief  OSAL driver test application main file.
 *
 */

/*===========================================================================*/
/*                             Includes                                      */
/*===========================================================================*/

#if !defined(BARE_METAL)
/* include for both safertos and freertos. */
#include <ti/osal/TaskP.h>
#endif

#if defined (FREERTOS)
#include "FreeRTOS.h"
#include <ti/osal/MemoryP.h>
#include <ti/osal/LoadP.h>
#endif

#if defined (SAFERTOS)
#include "SafeRTOS.h"
#endif

#if defined (BUILD_C7X)
#include <ti/csl/arch/csl_arch.h>
#endif

#if defined (BARE_METAL)
#include <ti/csl/csl_timer.h>
#endif

#include "osal_extended_test.h"

/*===========================================================================*/
/*                             Macros & Typedefs                             */
/*===========================================================================*/

#define TWO_TIMER_INTERRUPT_TEST 0

#undef  ENABLE_GET_TIME_TEST
#define ENABLE_GET_TIME_TEST     1

#ifndef NULL_PTR
#define NULL_PTR ((void *)0x0)
#endif

#define INT_NUM_IRQ 32
#define LOOP_CNT 2

/*Semaphore*/
#if defined(FREERTOS)
    #define SEMP_BLOCK_SIZE (1 * OSAL_FREERTOS_SEMAPHOREP_SIZE_BYTES)
#endif

#if defined(SAFERTOS)
    #define SEMP_BLOCK_SIZE (1 * OSAL_SAFERTOS_SEMAPHOREP_SIZE_BYTES)
#endif

#if defined(BARE_METAL)
    #define SEMP_BLOCK_SIZE (1 * OSAL_NONOS_SEMAPHOREP_SIZE_BYTES)
#endif

/* Test application stack size */
#if defined (BUILD_C7X)
/* Temp workaround to avoid assertion failure: A_stackSizeTooSmall : Task stack size must be >= 16KB.
  * until the Bug PDK-7605 is resolved */
#define APP_TSK_STACK_MAIN              (64U * 1024U)
#else
#define APP_TSK_STACK_MAIN              (16U * 1024U)
#endif

#ifdef BARE_METAL
/* No task support for Bare metal */
#else
/* Test application stack */
/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
#if defined(SAFERTOS) && defined (BUILD_MCU)
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__(( aligned( APP_TSK_STACK_MAIN ))) = { 0 };
#else
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__(( aligned( 0x2000 )));
#endif
#endif

/* ==========================================================================*/
/*                         Structure Declarations                            */
/* ==========================================================================*/

/* None */

/* ==========================================================================*/
/*                          Function Declarations                            */
/* ==========================================================================*/

/* None */

/*===========================================================================*/
/*                           Global Variables                                */
/*===========================================================================*/

volatile uint8_t ghwip_isr_got_execute = 0U;
extern uint32_t gOsalHwiAllocCnt;

volatile uint64_t gTestlocalTimeout = 0x100000U;
volatile bool gFlagSwi = false, gFlagIRQ = false;

/*Semaphore block*/
uint8_t semPMemBlock[SEMP_BLOCK_SIZE]
__attribute__ ((aligned(64)));

/*===========================================================================*/
/*                          Callback Functions                               */
/*===========================================================================*/

/* ISR for SwiP_nonos Test */
void SwiP_nonosIRQ(uintptr_t arg0, uintptr_t arg1)
{
   gFlagSwi = true;
}

/*=============================================================================*/
/*                          Function Definitions                               */
/*=============================================================================*/

/*
 *  ======== Board_initOSAL ========
 */
void Board_initOSAL(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  status;
    boardCfg = BOARD_INIT_PINMUX_CONFIG;
    boardCfg |= BOARD_INIT_MODULE_CLOCK;

    #if defined (UART_CONSOLE)
        boardCfg |= BOARD_INIT_UART_STDIO;
    #endif
    status = Board_init(boardCfg);

    if (BOARD_SOK != status)
    {
        OSAL_log(" Board_init() is not successful...unexpected results may happen \n");
    }

    return;
}

#if defined (BARE_METAL)
/*
 * Description: Testing Negative condition check for the below mentioned SwiP APIs :
 *      1. SwiP_Params_init
 *      2. SwiP_create
 *      3. SwiP_post
 *      4. SwiP_delete
 */
void SwiP_nonos_Test(void)
{
    SwiP_Handle handle;
    SwiP_Params swiParams;
    SwiP_Status status;
    bool test_pass = true;

    SwiP_Params_init(&swiParams);
    handle = SwiP_create((SwiP_Fxn)&SwiP_nonosIRQ, &swiParams);
    if (NULL_PTR != handle)
    {
        OSAL_log("\n Failed to create software interrupt \n");
        test_pass = false;
    }

    if(true == test_pass)
    {
        status = SwiP_post(handle);
        if (SwiP_OK != status)
        {
            test_pass = false;
        }
        status = SwiP_delete(&handle);
        if(SwiP_OK == status)
        {
            OSAL_log("\n SwiP nonos Negative Test Passed!! \n");
        }
        else
        {
            OSAL_log("\n SwiP nonos Negative Test Failed!! \n");
        }
    }
    else
    {
            OSAL_log("\n SwiP nonos Negative Test Failed!! \n");
    }
}
#endif

/*
 * Description  : Testing below mentioned DebugP APIs by passing required data :
 *      1. DebugP_log0
 *      2. DebugP_log1
 *      3. DebugP_log2
 *      4. DebugP_log3
 *      5. DebugP_log4
 */
void DebugP_log_Test()
{
    const char *format = NULL_PTR;
    uintptr_t p1=1,p2=1,p3=1,p4=1;

    DebugP_log0(format);

    DebugP_log1(format, p1);

    DebugP_log2(format, p1, p2);

    DebugP_log3(format, p1, p2, p3);

    DebugP_log4(format, p1, p2, p3, p4);

    OSAL_log("\n DebugP_log_Test passed!!\n");
}

#if defined(BARE_METAL)
void OSAL_tests()
#else
void OSAL_tests(void *arg0, void *arg1)
#endif
{
    int32_t result = osal_OK;
    Board_initOSAL();

#if defined(SAFERTOS)
#if !defined(BUILD_MCU)
    result += OsalApp_ArchutilsTests();
#endif
#endif

#if defined(FREERTOS)

    DebugP_log_Test();

    result += OsalApp_heapFreertosTest();

    result += OsalApp_queueTests();

    result += OsalApp_memoryTests();
    
    result += OsalApp_freertosLoadTests();

#if defined(BUILD_C7X)
    result += OsalApp_freertosPortableTests();
#endif

#endif

#if defined(BARE_METAL)

    result += OsalApp_cycleProfilerTest();

    SwiP_nonos_Test();

#endif

#if !defined(BARE_METAL)

    result += OsalApp_mailboxTests();

    result += OsalApp_eventTests();

#if !defined(BUILD_C66X)
    result += OsalApp_clockTests();

    result += OsalApp_registerIntrTests();
#endif

    result += OsalApp_taskTests();
#if defined(BUILD_MCU)
    result += OsalApp_timerTests();
#endif

    result += OsalApp_c7xArchTests();

#endif

    result += OsalApp_utilsTests();

    result += OsalApp_cacheTests();

    result += OsalApp_mutexTests();

    result += OsalApp_semaphoreTests();

    result += OsalApp_hwiTests();

#if defined LDRA_DYN_COVERAGE_EXIT
    OSAL_log("\n LDRA ENTRY... \n");
    upload_execution_history();
    OSAL_log("\n LDRA EXIT... \n");
#endif

    if(osal_OK == result)
    {
        OSAL_log("\n All tests have passed. \n");
    }
    else
    {
        OSAL_log("\n Some tests have failed. \n");
    }

#ifdef BARE_METAL
    while (1)
    {
    }
#endif
}


/*
 *  ======== main ========
 */
int main(void)
{
#ifdef BARE_METAL
    OSAL_tests();
#else

/* Loading before Task is created */
#if defined(FREERTOS)
extern void vPortTimerTickHandler(void);
vPortTimerTickHandler();
#endif

Osal_getThreadType();

#if defined(BUILD_C7X)
OsalCfgClecAccessCtrl(BFALSE);
#endif

    TaskP_Params taskParams;
    OS_init();
    memset( gAppTskStackMain, 0xFF, sizeof( gAppTskStackMain ) );
    TaskP_Params_init(&taskParams);
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    if (NULL_PTR != TaskP_self())
    {
        OSAL_log("TaskP self should return NULL, as this is main thread..!");
    }
    else
    {
        TaskP_create(&OSAL_tests, &taskParams);
        OS_start();
#if defined(FREERTOS)
        LoadP_update();
#endif
    }
#endif
    return (0);
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    /* Setup CLEC access/configure in secure mode */
    OsalCfgClecAccessCtrl(BTRUE);
    /* Reverting to non-secure mode */
    Osal_initMmuDefault();
}
#endif
