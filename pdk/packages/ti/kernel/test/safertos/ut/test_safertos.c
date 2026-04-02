/*
 *  Copyright ( C ) 2022 Texas Instruments Incorporated
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/TimerP.h>
#include <ti/osal/HwiP.h>
#include <unity.h>
#include <unity_config.h>
#include <ti/csl/soc.h>
#include <SAFERTOS_log.h>
#include <SafeRTOS_API.h>
#include <ti/board/board.h>

#include "safertos_config.h"

#if defined (BUILD_C7X)
#include <c7x.h>    /* for C7x intrinsics */
#endif

/*
 * IMPORTANT NOTES:
 *
 * Task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest
 * For this example any valid task priority can be set.
 *
 * See SafeRTOSConfig.h for configMAX_PRIORITIES and portStackType type.
 * SafeRTOSConfig.h can be found under kernel/SafeRTOS/config/${device}/${cpu}/
 *
 * In this example,
 * We create task's, semaphore's, ISR's and stack for the tasks using static allocation.
 * We dont need to delete these semaphore's since static allocation is used.
 *
 * One MUST not return out of a SafeRTOS task instead one MUST call vTaskDelete instead.
 */

#define NUM_TASK_SWITCHES      ( 1000000u )

#ifdef SOC_J721E
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           ( CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0 )
        #define PONG_INT_NUM           ( CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1 )
    #endif
    #ifdef BUILD_MCU1_1
        #define PING_INT_NUM           ( CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0 )
        #define PONG_INT_NUM           ( CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1 )
	#endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           ( CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_0 )
        #define PONG_INT_NUM           ( CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_1 )
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           ( CSLR_R5FSS0_CORE1_INTR_R5FSS0_INTROUTER0_OUTL_0 )
        #define PONG_INT_NUM           ( CSLR_R5FSS0_CORE1_INTR_R5FSS0_INTROUTER0_OUTL_1 )
    #endif
    #ifdef BUILD_MCU3_0
        #define PING_INT_NUM           ( CSLR_R5FSS1_CORE0_INTR_R5FSS1_INTROUTER0_OUTL_0 )
        #define PONG_INT_NUM           ( CSLR_R5FSS1_CORE0_INTR_R5FSS1_INTROUTER0_OUTL_1 )
    #endif
    #ifdef BUILD_MCU3_1
        #define PING_INT_NUM           ( CSLR_R5FSS1_CORE1_INTR_R5FSS1_INTROUTER0_OUTL_0 )
        #define PONG_INT_NUM           ( CSLR_R5FSS1_CORE1_INTR_R5FSS1_INTROUTER0_OUTL_1 )
    #endif
    #ifdef BUILD_C66X_1
        #define PING_INT_NUM           (8u)
        #define PING_EVT_ID            (CSLR_C66SS0_CORE0_C66_EVENT_IN_SYNC_C66SS0_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (9u)
        #define PONG_EVT_ID            (CSLR_C66SS0_CORE0_C66_EVENT_IN_SYNC_C66SS0_INTROUTER0_OUTL_1)
    #endif
    #ifdef BUILD_C66X_2
        #define PING_INT_NUM           (8u)
        #define PING_EVT_ID            (CSLR_C66SS1_CORE0_C66_EVENT_IN_SYNC_C66SS1_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (9u)
        #define PONG_EVT_ID            (CSLR_C66SS1_CORE0_C66_EVENT_IN_SYNC_C66SS1_INTROUTER0_OUTL_1)
    #endif
    #ifdef BUILD_C7X_1
        #define PING_INT_NUM           (14u)
        #define PING_EVT_ID            (0x4E0)
        #define PONG_INT_NUM           (15u)
        #define PONG_EVT_ID            (0x4E1)
    #endif
#endif

#ifdef SOC_J7200
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU1_1
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_1)
    #endif
#endif

#ifdef SOC_J721S2
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU1_1
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU3_0
        #define PING_INT_NUM           (CSLR_R5FSS1_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS1_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU3_1
        #define PING_INT_NUM           (CSLR_R5FSS1_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS1_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_C7X_1
        /* Interrupt number 16 of Timer2 is used as kernel tick interrupt. */
        #define PING_INT_NUM           (18U)
        #define PING_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER2_INTR_PEND_0 + 992)
        #define PONG_INT_NUM           (17U)
        #define PONG_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER3_INTR_PEND_0 + 992)
    #endif
    #ifdef BUILD_C7X_2
        #define PING_INT_NUM           (18U)
        #define PING_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER4_INTR_PEND_0 + 992)
        #define PONG_INT_NUM           (19U)
        #define PONG_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER5_INTR_PEND_0 + 992)
    #endif
#endif
#ifdef SOC_J784S4
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU1_1
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU3_0
        #define PING_INT_NUM           (CSLR_R5FSS1_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS1_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU3_1
        #define PING_INT_NUM           (CSLR_R5FSS1_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS1_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU4_0
        #define PING_INT_NUM           (CSLR_R5FSS2_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS2_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU4_1
        #define PING_INT_NUM           (CSLR_R5FSS2_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS2_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_C7X_1
        /* Interrupt number 16 of Timer2 is used as kernel tick interrupt. */
        #define PING_INT_NUM           (18U)
        #define PING_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER2_INTR_PEND_0 + 992)
        #define PONG_INT_NUM           (17U)
        #define PONG_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER3_INTR_PEND_0 + 992)
    #endif
    #ifdef BUILD_C7X_2
        #define PING_INT_NUM           (18U)
        #define PING_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER4_INTR_PEND_0 + 992)
        #define PONG_INT_NUM           (19U)
        #define PONG_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER5_INTR_PEND_0 + 992)
    #elif BUILD_C7X_3
        #define PING_INT_NUM           (20U)
        #define PING_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER6_INTR_PEND_0 + 992)
        #define PONG_INT_NUM           (21U)
        #define PONG_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER7_INTR_PEND_0 + 992)
    #endif
    #ifdef BUILD_C7X_4
        #define PING_INT_NUM           (22U)
        #define PING_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER8_INTR_PEND_0 + 992)
        #define PONG_INT_NUM           (23U)
        #define PONG_EVT_ID            (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER9_INTR_PEND_0 + 992)
    #endif
#endif

#define PING_TASK_PRI  ( 2u )
#define PONG_TASK_PRI  ( 3u )


#if !defined (BUILD_C7X)
#define PING_TASK_SIZE ( 4096u )
#define PONG_TASK_SIZE ( 4096u )
#else
#define PING_TASK_SIZE ( 16U * 1024U)
#define PONG_TASK_SIZE ( 16U * 1024U)
#endif

#if defined (BUILD_MCU)
static portInt8Type  gPingTaskStack[PING_TASK_SIZE] __attribute__( ( aligned( PING_TASK_SIZE ) ) );
static portInt8Type  gPongTaskStack[PONG_TASK_SIZE] __attribute__( ( aligned( PONG_TASK_SIZE ) ) );
#elif defined (BUILD_C7X)
static portInt8Type  gPingTaskStack[PING_TASK_SIZE] __attribute__( ( aligned( 0x2000 ) ) );
static portInt8Type  gPongTaskStack[PONG_TASK_SIZE] __attribute__( ( aligned( 0x2000 ) ) );
#else
static portInt8Type  gPingTaskStack[PING_TASK_SIZE] __attribute__( ( aligned( 128 ) ) );
static portInt8Type  gPongTaskStack[PONG_TASK_SIZE] __attribute__( ( aligned( 128 ) ) );
#endif

static xTCB xPingTaskTCB = { 0 };
static xTCB xPongTaskTCB = { 0 };


/* Semaphore buffers. No actual data is stored into these buffers, so the
 * buffer need only be large enough to hold the queue structure itself. */
portInt8Type gPingSemBuf[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned ( safertosapiWORD_ALIGNMENT ) ) ) = { 0 };
portInt8Type gPongSemBuf[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned ( safertosapiWORD_ALIGNMENT ) ) ) = { 0 };

/* Semaphore Parameters. */
static xSemaphoreHandle gPingSem = NULL;
static xSemaphoreHandle gPongSem = NULL;

#ifdef BUILD_C7X
uint8_t srcBuf[1024];
uint8_t dstBuf[1024];
#endif

uint32_t ping_isr_1_count = 0;
static void ping_isr_1(uintptr_t arg)
{
    ping_isr_1_count++;
    xSemaphoreGiveFromISR(gPingSem); /* wake up ping task */
    #ifdef BUILD_C7X
    /* For C7x do memcpy in ISR to confirm Vector registers are restored correctly from ISR */
    memcpy(dstBuf, srcBuf, sizeof(dstBuf));
    #endif
    safertosapiYIELD_FROM_ISR();
}

uint32_t ping_isr_2_count = 0;
static void ping_isr_2(uintptr_t arg)
{
    ping_isr_2_count++;
    xSemaphoreGiveFromISR(gPongSem); /* wake up pong task */
    safertosapiYIELD_FROM_ISR();
}

uint32_t pong_isr_2_count = 0;
static void pong_isr_2(uintptr_t arg)
{
    pong_isr_2_count++;
    xSemaphoreGiveFromISR(gPingSem); /* wake up ping task */
    safertosapiYIELD_FROM_ISR();
}

double gFloat = 10.0;
static void ping_isr_5(uintptr_t arg)
{
    gFloat = gFloat + 0.1;
    xSemaphoreGiveFromISR(gPingSem); /* wake up ping task */
    safertosapiYIELD_FROM_ISR();
}

/* switch between ping and pong tasks using semaphores */
void test_taskSwitchWithSemaphore(void)
{
    /* switch between ping and pong tasks using semaphores */
    uint32_t count; 				/* loop `count` times */
	volatile uint64_t curTime; 		/* time in units of usecs */
    count = NUM_TASK_SWITCHES;
    curTime = TimerP_getTimeInUsecs();
	portBaseType semaphoreResult = pdPASS;
	while( count-- && ( pdPASS == semaphoreResult ) )
	{
		semaphoreResult = xSemaphoreGive( gPongSem );                                       /* wake up pong task */
		if( pdPASS == semaphoreResult )
		{
			semaphoreResult = xSemaphoreTake( gPingSem, safertosapiMAX_DELAY );             /* wait for pong to signal */
		}
	}
	DebugP_assert( pdPASS == semaphoreResult );

	curTime = TimerP_getTimeInUsecs() - curTime;

	SAFERTOS_log("\r\n");
	SAFERTOS_log("test_taskSwitchWithSemaphore\r\n");
    SAFERTOS_log("execution time for task switches = %d ms\r\n", (uint32_t)(curTime/1000));
    SAFERTOS_log("number of task switches = %d \r\n", (uint32_t)NUM_TASK_SWITCHES*2);
    SAFERTOS_log("time per task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime*1000/(NUM_TASK_SWITCHES*2)));
    SAFERTOS_log("[SAFERTOS] ping taskSwitchWithSemaphore ... end !!!\r\n");
}

/* just invoke the task switch logic without any semaphores or direct-to-task notifications */
void test_taskYeild(void)
{
    uint32_t count;             /* loop `count` times */
    volatile uint64_t curTime;  /* time in units of usecs */

    count = NUM_TASK_SWITCHES;
    curTime = TimerP_getTimeInUsecs();
    while (count--)
    {
    	safertosapiYIELD();
    }
    curTime = TimerP_getTimeInUsecs() - curTime;

    SAFERTOS_log("\r\n");
    SAFERTOS_log("test_taskYeild\r\n");
    SAFERTOS_log("execution time for task yields = %d ms\r\n", (uint32_t)(curTime/1000));
    SAFERTOS_log("number of task yields = %d \r\n", (uint32_t)NUM_TASK_SWITCHES);
    SAFERTOS_log("time per task yield = %d ns\r\n", (uint32_t)(curTime * 1000 / (NUM_TASK_SWITCHES)));
}

/* switch from ping to ISR and back to the same task using semaphores, here there is no task switch */
void test_taskToIsrUsingSemaphoreAndNoTaskSwitch(void)
{
    uint32_t count;             /* loop `count` times */
    volatile uint64_t curTime;  /* time in units of usecs */
    HwiP_Params hwiParams;
    HwiP_Handle hHwi;
    HwiP_Status hwiStatus;
    portBaseType semaphoreResult = pdPASS;

    HwiP_Params_init(&hwiParams);
#if defined (BUILD_C66X)
        hwiParams.evtId = PING_EVT_ID;
#endif
    hHwi = HwiP_create(PING_INT_NUM, ping_isr_1, &hwiParams);
    DebugP_assert(hHwi != NULL);

    count = NUM_TASK_SWITCHES;
    ping_isr_1_count = 0;
    curTime = TimerP_getTimeInUsecs();
    while( count-- && ( pdPASS == semaphoreResult ) )
    {
        HwiP_post(PING_INT_NUM);
        semaphoreResult = xSemaphoreTake(gPingSem, safertosapiMAX_DELAY); /* wait for ISR to signal */
    }
    DebugP_assert( pdPASS == semaphoreResult );

    curTime = TimerP_getTimeInUsecs() - curTime;

    hwiStatus = HwiP_delete(hHwi);
    DebugP_assert(hwiStatus == HwiP_OK);

    SAFERTOS_log("\r\n");
    SAFERTOS_log("test_taskToIsrUsingSemaphoreAndNoTaskSwitch\r\n");
    SAFERTOS_log("execution time for task - ISR - task switches = %d ms\r\n", (uint32_t)(curTime/1000));
    SAFERTOS_log("number of task switches = %d, num ISRs = %d \r\n", (uint32_t)NUM_TASK_SWITCHES, ping_isr_1_count);
    SAFERTOS_log("time per task - ISR - task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime * 1000 / (NUM_TASK_SWITCHES)));
}

/* switch from ping task to ISR to pong task and back to ping task using semaphores, here there is a task switch */
void test_taskToIsrUsingSemaphoreAndWithTaskSwitch(void)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of usecs */
    HwiP_Params hwiParams;
    HwiP_Handle hHwi;
    HwiP_Status hwiStatus;
    portBaseType semaphoreResult = pdPASS;

    HwiP_Params_init(&hwiParams);
#if defined (BUILD_C66X)
        hwiParams.evtId = PING_EVT_ID;
#endif
    hHwi = HwiP_create(PING_INT_NUM, ping_isr_2, &hwiParams);
    DebugP_assert(hHwi != NULL);

    count = NUM_TASK_SWITCHES;
    ping_isr_2_count = 0;
    curTime = TimerP_getTimeInUsecs();
    while( count-- && ( pdPASS == semaphoreResult ) )
    {
        HwiP_post(PING_INT_NUM);
        semaphoreResult = xSemaphoreTake(gPingSem, safertosapiMAX_DELAY); /* wait for ISR to signal */
    }
    DebugP_assert( pdPASS == semaphoreResult );

    curTime = TimerP_getTimeInUsecs() - curTime;

    hwiStatus = HwiP_delete(hHwi);
    DebugP_assert(hwiStatus == HwiP_OK);

    SAFERTOS_log("\r\n");
    SAFERTOS_log("test_taskToIsrUsingSemaphoreAndWithTaskSwitch\r\n");
    SAFERTOS_log("execution time for task - ISR - task - task switches = %d ms\r\n", (uint32_t)(curTime/1000));
    SAFERTOS_log("number of ISRs = %d.Ping ISR Count:%d, Pong ISR Count:%d  \r\n", (uint32_t)NUM_TASK_SWITCHES * 2, ping_isr_2_count, pong_isr_2_count);
    SAFERTOS_log("time per task - ISR - task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime * 1000 / (2 * NUM_TASK_SWITCHES)));
}

/* switch between ping and pong tasks and do float operations in between */
void test_taskSwitchWithFloatOperations(void)
{
    uint32_t count;             /* loop `count` times */
    volatile uint64_t curTime;  /* time in units of usecs */
    volatile double f;
    portBaseType semaphoreResult = pdPASS;

    f = 0.0;
    count = NUM_TASK_SWITCHES;
    curTime = TimerP_getTimeInUsecs();
    while( count-- && ( pdPASS == semaphoreResult ) )
    {
        f = f + 0.01;
        semaphoreResult = xSemaphoreGive(gPongSem);                                                 /* wake up pong task */
        if( pdPASS == semaphoreResult ){
        	semaphoreResult = xSemaphoreTake(gPingSem, safertosapiMAX_DELAY);                       /* wait for pong to signal */
        }
    }
    DebugP_assert( pdPASS == semaphoreResult );

    curTime = TimerP_getTimeInUsecs() - curTime;

    SAFERTOS_log("\r\n");
	SAFERTOS_log("test_taskSwitchWithFloatOperations\r\n");
    SAFERTOS_log("execution time for task switches = %d ms\r\n", (uint32_t)(curTime/1000));
    SAFERTOS_log("number of task switches = %d \r\n", (uint32_t)NUM_TASK_SWITCHES * 2);
    SAFERTOS_log("time per task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime * 1000 / (NUM_TASK_SWITCHES * 2)));
    TEST_ASSERT_UINT32_WITHIN(1, (NUM_TASK_SWITCHES / 100), (uint32_t)f);

    f = 1.07;
    count = 10;
    volatile double e = f;
    semaphoreResult = xSemaphoreGive(gPongSem); /* signal pong to start */
    /* pong (higher priority task) sleeps for 1ms and do some floating opearion in a while loop.
     * Following pow() calculation takes places in b/w when pong sleeps */
    while (count-- && pdPASS==semaphoreResult)
    {
        f = pow(f,1.02);
    }
    DebugP_assert( pdPASS == semaphoreResult );
    semaphoreResult = xSemaphoreGive(gPongSem); /* signal pong to stop */
    /* This will cause pong (higher priority task) to exit from while loop */
    count = 10; /* pong task is completed now */
    while (count-- && pdPASS==semaphoreResult)
    {
        e = pow(e,1.02);
    }
    DebugP_assert( pdPASS == semaphoreResult );

    SAFERTOS_log("\r\n");
    SAFERTOS_log("value after pow() computed b/w task switch = %d.%d \r\n", (uint32_t)f, (uint32_t)((f-(uint32_t)f)*100000));
    SAFERTOS_log("expected value = %d.%d \r\n", (uint32_t)e, (uint32_t)((e-(uint32_t)e)*100000));
    TEST_ASSERT_EQUAL_DOUBLE(e, f);
}

/* switch between ping tasks and ISR and do float operations in between */
void test_taskToIsrWithFloatOperations(void)
{
    uint32_t count; /* loop `count` times */
    double f;
    HwiP_Params hwiParams;
    HwiP_Handle hHwi;
    HwiP_Status hwiStatus;
    portBaseType semaphoreResult = pdPASS;

    HwiP_Params_init(&hwiParams);
#if defined (BUILD_C66X)
        hwiParams.evtId = PING_EVT_ID;
#endif
    hHwi = HwiP_create(PING_INT_NUM, ping_isr_5, &hwiParams);
    DebugP_assert(hHwi != NULL);

    f = 0.0;
    count = NUM_TASK_SWITCHES;
    double e = (NUM_TASK_SWITCHES / 100);
    while (count-- && semaphoreResult == pdPASS)
    {
        f = f + 0.01;
        HwiP_post(PING_INT_NUM);
        semaphoreResult = xSemaphoreTake(gPingSem, safertosapiMAX_DELAY); /* wait for ISR to signal */
    }
    DebugP_assert( pdPASS == semaphoreResult );

    hwiStatus = HwiP_delete(hHwi);
    DebugP_assert(hwiStatus == HwiP_OK);

    TEST_ASSERT_DOUBLE_WITHIN(0.01, e, f);
    SAFERTOS_log("\r\n");
    SAFERTOS_log("test_taskToIsrWithFloatOperations PASSED \r\n");
}

/* wait some msecs, this is just to show how delay API can be used,
* there is no need to delay before deleting the task
*/
void test_taskDelay(void)
{
	SAFERTOS_log( "\r\n" );
	SAFERTOS_log( "[SafeRTOS] test_taskDelay started !!!\r\n" );
    volatile uint64_t curTime; /* time in units of usecs */
    uint32_t delay1 = 30 * 1000, delay2 = 30 * 1000; /* in msecs */

    curTime = TimerP_getTimeInUsecs();
    /* convert to ticks before pass to xTaskDelay */
    xTaskDelay(delay1 / configTICK_RATE_MS);
    xTaskDelay(delay2 / configTICK_RATE_MS);
    curTime = TimerP_getTimeInUsecs() - curTime;

#if defined(SOC_J721E) && defined(BUILD_C7X)
    TEST_ASSERT_UINT32_WITHIN(configTICK_RATE_MS * 100000, (delay1 + delay2) * 1000, (uint32_t)curTime);
#else 
    TEST_ASSERT_UINT32_WITHIN(configTICK_RATE_MS * 1000, (delay1 + delay2) * 1000, (uint32_t)curTime);
#endif
    SAFERTOS_log( "[SafeRTOS] test_taskDelay ended !!!\r\n" );
}

#if defined (BUILD_C7X)
/* return current counter value of high speed counter in units of 10's of usecs */
uint32_t uiPortGetRunTimeCounterValue()
{
    uint64_t ts = __TSC;
    uint64_t timeInUsecs;

    timeInUsecs = (ts * 1000000) / configCPU_CLOCK_HZ;
    /* note, there is no overflow protection for this 32b value in SafeRTOS
     *
     * Dividing by 10 to reduce the resolution and avoid overflow for longer duration
     * This will overflow after
     * ((0x100000000/1000000)/(60*60))*10 hours ~ 12 hrs
     */
    return (uint32_t)timeInUsecs;
}
#endif

void ping_main( void *args )
{
    SAFERTOS_log( "\r\n" );
    SAFERTOS_log( "[SafeRTOS] ping task ... start !!!\r\n" );
    UNITY_BEGIN();

    RUN_TEST(test_taskSwitchWithSemaphore, 1, NULL);
    RUN_TEST(test_taskYeild, 3, NULL);
    RUN_TEST(test_taskToIsrUsingSemaphoreAndNoTaskSwitch, 4, NULL);
    RUN_TEST(test_taskToIsrUsingSemaphoreAndWithTaskSwitch, 6, NULL);
    RUN_TEST(test_taskSwitchWithFloatOperations, 8, NULL);
    RUN_TEST(test_taskToIsrWithFloatOperations, 9, NULL);
    RUN_TEST(test_taskDelay, 10, NULL);

    UNITY_END();
    if (Unity.TestFailures == 0)
	{
    	SAFERTOS_log("\r\n");
    	SAFERTOS_log("All tests have passed!!\r\n");
	}
    ( void )xTaskDelete(  NULL  );
}

void setUp(void)
{
}

void tearDown(void)
{
}

void pong_main( void *args )
{
	SAFERTOS_log( "[SafeRTOS] pong task ... start !!!\r\n" );
	uint32_t count; /* loop `count` times */
	portBaseType semaphoreResult = pdPASS;

	count = NUM_TASK_SWITCHES;
	while( count-- && ( pdPASS == semaphoreResult ) )
	{
		semaphoreResult = xSemaphoreTake( gPongSem, safertosapiMAX_DELAY );
		if( pdPASS == semaphoreResult )
		{
			semaphoreResult = xSemaphoreGive( gPingSem );
		}
	}
	DebugP_assert( pdPASS == semaphoreResult );

	{
		HwiP_Params hwiParams;
		HwiP_Handle hHwi;
		HwiP_Status hwiStatus;

		HwiP_Params_init(&hwiParams);
#if defined (BUILD_C66X)
        hwiParams.evtId = PONG_EVT_ID;
#endif
		hHwi = HwiP_create(PONG_INT_NUM, pong_isr_2, &hwiParams);
		DebugP_assert(hHwi != NULL);

		count = NUM_TASK_SWITCHES;
		pong_isr_2_count = 0;
		while( count-- && ( pdPASS == semaphoreResult ) )
		{
			semaphoreResult = xSemaphoreTake(gPongSem, safertosapiMAX_DELAY); /* wait for ISR to signal */
			if( pdPASS == semaphoreResult )
			{
				HwiP_post(PONG_INT_NUM);
			}
		}
		DebugP_assert( pdPASS == semaphoreResult );

		hwiStatus = HwiP_delete(hHwi);
		DebugP_assert(hwiStatus == HwiP_OK);
	}

	{ /* switch between ping and pong tasks and do float operations in between */
		double f;
		f = 10.0;

		count = NUM_TASK_SWITCHES;
		while( count-- && ( pdPASS == semaphoreResult ) )
		{
			f = f + 0.1;
			semaphoreResult = xSemaphoreTake(gPongSem, safertosapiMAX_DELAY); /* wait for ping to signal */
			if( pdPASS == semaphoreResult ){
				semaphoreResult = xSemaphoreGive(gPingSem);                   /* wakeup ping task */
			}
		}
		DebugP_assert( pdPASS == semaphoreResult );

		f = 2.3;
		semaphoreResult = xSemaphoreTake(gPongSem, safertosapiMAX_DELAY);     /* wait for ping to signal */
		while (1 && pdPASS==semaphoreResult)
		{
			xTaskDelay(1);
			f = pow(f, 1.3);
			if(xSemaphoreTake(gPongSem, 0)==pdPASS)                           /* check if ping signalled to stop */
			{
				break;
			}
		}
		DebugP_assert( pdPASS == semaphoreResult );
	}
	( void )xTaskDelete(  NULL  );
}

void test_safertos_main( void *args )
{
    portBaseType xStatus;
#if defined(BUILD_MCU1_0)
    Board_initCfg boardCfg;
    Board_STATUS  status;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;

    status = Board_init(boardCfg);

    DebugP_assert(status == BOARD_SOK);

#endif

    /* Create the semaphore used by the first two tasks. */
    /* safertos creates binary semaphore with init value of 1 */
    /* to reset the semaphore , do a xSemaphoreTake immediately after create */
    xSemaphoreCreateBinary( gPingSemBuf, &gPingSem );
    if( gPingSem == NULL )
    {
        xStatus = pdFAIL;
    }
    else
    {
        xStatus = xSemaphoreTake( gPingSem, safertosapiMAX_DELAY );
    }
    DebugP_assert(xStatus != pdFAIL);
    xSemaphoreCreateBinary( gPongSemBuf, &gPongSem );
    if( gPongSem == NULL )
    {
        xStatus = pdFAIL;
    }
    else
    {
        xStatus = xSemaphoreTake( gPongSem, safertosapiMAX_DELAY );
    }
    DebugP_assert(xStatus != pdFAIL);

    xTaskParameters xPongMainParameters =
    {
    		.pvTaskCode = &pong_main,                       /* Task code */
			.pcTaskName = "pong",                           /* Task name */
			.pxTCB = &xPongTaskTCB,                         /* TCB */
			.pcStackBuffer = gPongTaskStack,                /* Stack buffer */
			.uxStackDepthBytes = PONG_TASK_SIZE,            /* Stack depth bytes */
			.pvParameters = NULL,                           /* Parameters */
			.uxPriority = PONG_TASK_PRI,                    /* Priority */
			.pvObject = NULL,                               /* TLS object */
#if defined (BUILD_MCU)
			.xUsingFPU = pdTRUE,                            /* Check task does not use the FPU. */
			.xMPUParameters = {                             /* MPU task parameters. */
                mpuPRIVILEGED_TASK,                         /* Check task is privileged. */
                {
                    { NULL, 0U, 0U, 0U },                   /* No additional region definitions are required. */
                    { NULL, 0U, 0U, 0U },
                }
			}                                               /* Port-specific task parameters */
#endif
#if defined (BUILD_C66X)
    		.uxPrivilegeLevel = safertosapiUNPRIVILEGED_TASK,       /* The idle hook will not be executed in privileged mode. */
#endif
    };

    xTaskParameters xPingMainParameters =
    {
    		.pvTaskCode = &ping_main,                       /* Task code */
			.pcTaskName = "ping",                           /* Task name */
			.pxTCB = &xPingTaskTCB,                         /* TCB */
			.pcStackBuffer = gPingTaskStack,                /* Stack buffer */
			.uxStackDepthBytes = PING_TASK_SIZE,            /* Stack depth bytes */
			.pvParameters = NULL,                           /* Parameters */
			.uxPriority = PING_TASK_PRI,                    /* Priority */
			.pvObject = NULL,                               /* TLS object */
#if defined (BUILD_MCU)
			.xUsingFPU = pdTRUE,                            /* Check task does not use the FPU. */
			.xMPUParameters = {                             /* MPU task parameters. */
                mpuPRIVILEGED_TASK,                         /* Check task is privileged. */
                {
                    { NULL, 0U, 0U, 0U },                   /* No additional region definitions are required. */
                    { NULL, 0U, 0U, 0U },
                }
            }                                               /* Port-specific task parameters */
#endif
#if defined (BUILD_C66X)
    		.uxPrivilegeLevel = safertosapiUNPRIVILEGED_TASK,       /* The idle hook will not be executed in privileged mode. */
#endif
    };
    
    /* Create the privileged test tasks. */
    if( xTaskCreate( &xPongMainParameters, NULL ) != pdPASS )
    {
        xStatus = pdFAIL;
    }
    DebugP_assert(xStatus != pdFAIL);

    if( xTaskCreate( &xPingMainParameters, NULL ) != pdPASS )
    {
        xStatus = pdFAIL;
    }
    DebugP_assert(xStatus != pdFAIL);

    /* Dont close drivers to keep the UART driver open for console */
    /* Drivers_close(  ); */
    ( void )xTaskDelete( NULL );
}

