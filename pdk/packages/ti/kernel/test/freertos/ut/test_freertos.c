/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/HwiP.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <unity.h>
#include <unity_config.h>
#include <ti/csl/soc.h>
#include <FREERTOS_log.h>

/* 
 * IMPORTANT NOTES: 
 * 
 * Alignment of stack is not strictly needed for R5F but debug is easier if stack is nicely 
 * aligned.
 * 
 * Task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest
 * For this example any valid task priority can be set.
 * 
 * See FreeRTOSConfig.h for configMAX_PRIORITIES and StackType_t type.
 * FreeRTOSConfig.h can be found under kernel/freertos/config/${device}/${cpu}/
 * 
 * In this example, 
 * We create task's, semaphore's, ISR's and stack for the tasks using static allocation. 
 * We dont need to delete these semaphore's since static allocation is used.
 * 
 * One MUST not return out of a FreeRTOS task instead one MUST call vTaskDelete instead.
 */

#define NUM_TASK_SWITCHES (1000000u)

#if defined (SOC_TPR12) || defined (SOC_AWR294X)
    #if defined(BUILD_MCU1_0) || defined(BUILD_MCU1_1)
        #define PING_INT_NUM           (236u)
        #define PONG_INT_NUM           (237u)
    #endif
    #ifdef BUILD_C66X_1
        #define PING_INT_NUM           (8u)
        #define PING_EVT_ID            (CSL_DSS_INTR_DSS_RTIB_0)
        #define PONG_INT_NUM           (9u)
        #define PONG_EVT_ID            (CSL_DSS_INTR_DSS_RTIB_1)
    #endif
#endif

#ifdef SOC_AM65XX
    #if defined (BUILD_MCU1_0) || defined (BUILD_MCU1_1)
        #define PING_INT_NUM           (CSL_MCU0_INTR_MAIN2MCU_LVL_INTR0_OUTL_0)
        #define PONG_INT_NUM           (CSL_MCU0_INTR_MAIN2MCU_LVL_INTR0_OUTL_1)
    #endif
#endif

#ifdef SOC_J721E
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU1_1
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_R5FSS0_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_R5FSS0_INTROUTER0_OUTL_1)
    #endif
    #ifdef BUILD_MCU3_0
        #define PING_INT_NUM           (CSLR_R5FSS1_CORE0_INTR_R5FSS1_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS1_CORE0_INTR_R5FSS1_INTROUTER0_OUTL_1)
    #endif
    #ifdef BUILD_MCU3_1
        #define PING_INT_NUM           (CSLR_R5FSS1_CORE1_INTR_R5FSS1_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS1_CORE1_INTR_R5FSS1_INTROUTER0_OUTL_1)
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
        #define PONG_INT_NUM           (15u)
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
        #define PING_INT_NUM           (15U)
        #define PONG_INT_NUM           (16U)
    #endif
    #ifdef BUILD_C7X_2
        #define PING_INT_NUM           (16U)
        #define PONG_INT_NUM           (17U)
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
        #define PING_INT_NUM           (15U)
        #define PONG_INT_NUM           (16U)
    #endif
    #ifdef BUILD_C7X_2
        #define PING_INT_NUM           (16U)
        #define PONG_INT_NUM           (17U)
    #endif
    #ifdef BUILD_C7X_3
        #define PING_INT_NUM           (17U)
        #define PONG_INT_NUM           (18U)
    #endif
    #ifdef BUILD_C7X_4
        #define PING_INT_NUM           (18U)
        #define PONG_INT_NUM           (19U)
    #endif

#endif

#ifdef SOC_J742S2
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
        #define PING_INT_NUM           (15U)
        #define PONG_INT_NUM           (16U)
    #endif
    #ifdef BUILD_C7X_2
        #define PING_INT_NUM           (16U)
        #define PONG_INT_NUM           (17U)
    #endif
    #ifdef BUILD_C7X_3
        #define PING_INT_NUM           (17U)
        #define PONG_INT_NUM           (18U)
    #endif
#endif

#define PING_TASK_PRI (2u)
#define PONG_TASK_PRI (3u)

#define PING_TASK_SIZE (32*1024u)
StackType_t gPingTaskStack[PING_TASK_SIZE] __attribute__((aligned(32)));

#define PONG_TASK_SIZE (32*1024u)
StackType_t gPongTaskStack[PONG_TASK_SIZE] __attribute__((aligned(32)));

StaticTask_t gPingTaskObj;
StaticSemaphore_t gPingSemObj;
TaskHandle_t gPingTask;
SemaphoreHandle_t gPingSem;

StaticTask_t gPongTaskObj;
StaticSemaphore_t gPongSemObj;
TaskHandle_t gPongTask;
SemaphoreHandle_t gPongSem;

#ifdef BUILD_C7X
uint8_t srcBuf[1024];
uint8_t dstBuf[1024];
#endif

uint32_t ping_isr_1_count = 0;
static void ping_isr_1(uintptr_t arg)
{
    BaseType_t doTaskSwitch = 0;

    ping_isr_1_count++;
    xSemaphoreGiveFromISR(gPingSem, &doTaskSwitch); /* wake up ping task */
    #ifdef BUILD_C7X
    /* For C7x do memcpy in ISR to confirm Vector registers are restored correctly from ISR */
    memcpy(dstBuf, srcBuf, sizeof(dstBuf));
    #endif
    portYIELD_FROM_ISR(doTaskSwitch);
}

uint32_t ping_isr_2_count = 0;
static void ping_isr_2(uintptr_t arg)
{
    BaseType_t doTaskSwitch = 0;

    ping_isr_2_count++;
    xSemaphoreGiveFromISR(gPongSem, &doTaskSwitch); /* wake up pong task */
    portYIELD_FROM_ISR(doTaskSwitch);
}

uint32_t ping_isr_3_count = 0;
static void ping_isr_3(uintptr_t arg)
{
    BaseType_t doTaskSwitch = 0;

    ping_isr_3_count++;
    vTaskNotifyGiveFromISR(gPingTask, &doTaskSwitch); /* wake up ping task */
    portYIELD_FROM_ISR(doTaskSwitch);
}

uint32_t ping_isr_4_count = 0;
static void ping_isr_4(uintptr_t arg)
{
    BaseType_t doTaskSwitch = 0;

    ping_isr_4_count++;
    vTaskNotifyGiveFromISR(gPongTask, &doTaskSwitch); /* wake up pong task */
    portYIELD_FROM_ISR(doTaskSwitch);
}

double gFloat = 10.0;
static void ping_isr_5(uintptr_t arg)
{
    BaseType_t doTaskSwitch = 0;

    gFloat = gFloat + 0.1;
    xSemaphoreGiveFromISR(gPingSem, &doTaskSwitch); /* wake up ping task */
    portYIELD_FROM_ISR(doTaskSwitch);
}

uint32_t pong_isr_2_count = 0;
static void pong_isr_2(uintptr_t arg)
{
    BaseType_t doTaskSwitch = 0;

    pong_isr_2_count++;
    xSemaphoreGiveFromISR(gPingSem, &doTaskSwitch); /* wake up ping task */
    portYIELD_FROM_ISR(doTaskSwitch);
}

uint32_t pong_isr_4_count = 0;
static void pong_isr_4(uintptr_t arg)
{
    BaseType_t doTaskSwitch = 0;

    pong_isr_4_count++;
    vTaskNotifyGiveFromISR(gPingTask, &doTaskSwitch); /* wake up ping task */
    portYIELD_FROM_ISR(doTaskSwitch);
}

/* switch between ping and pong tasks using semaphores */
void test_taskSwitchWithSemaphore(void)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of usecs */

    FREERTOS_log("\r\n");
    FREERTOS_log("[FreeRTOS] ping task ... start !!!\r\n");
    count = NUM_TASK_SWITCHES;
    curTime = uiPortGetRunTimeCounterValue();
    while (count--)
    {
        xSemaphoreGive(gPongSem);                /* wake up pong task */
        xSemaphoreTake(gPingSem, portMAX_DELAY); /* wait for pong to signal */
    }
    curTime = uiPortGetRunTimeCounterValue() - curTime;

        FREERTOS_log("\r\n");
        FREERTOS_log("execution time for task switches = %d ms\r\n", (uint32_t)(curTime/1000));
        FREERTOS_log("number of task switches = %d \r\n", (uint32_t)NUM_TASK_SWITCHES*2);
        FREERTOS_log("time per task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime*1000/(NUM_TASK_SWITCHES*2)));
}

/* switch between ping and pong tasks using direct-to-task notifications */
void test_taskSwitchWithTaskNotify(void)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of usecs */

    count = NUM_TASK_SWITCHES;
    curTime = uiPortGetRunTimeCounterValue();
    while (count--)
    {
        xTaskNotifyGive(gPongTask);              /* wake up pong task */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); /* wait for pong to signal */
    }
    curTime = uiPortGetRunTimeCounterValue() - curTime;

        FREERTOS_log("\r\n");
        FREERTOS_log("execution time for task switches = %d ms\r\n", (uint32_t)(curTime/1000));
        FREERTOS_log("number of task switches = %d \r\n", (uint32_t)NUM_TASK_SWITCHES*2);
        FREERTOS_log("time per task switch (direct-to-task notification give/take) = %d ns\r\n", (uint32_t)(curTime*1000/(NUM_TASK_SWITCHES*2)));
}

/* just invoke the task switch logic without any semaphores or direct-to-task notifications */
void test_taskYeild(void)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of usecs */

    count = NUM_TASK_SWITCHES;
    curTime = uiPortGetRunTimeCounterValue();
    while (count--)
    {
        taskYIELD();
    }
    curTime = uiPortGetRunTimeCounterValue() - curTime;

    FREERTOS_log("\r\n");
    FREERTOS_log("execution time for task yields = %d ms\r\n", (uint32_t)(curTime/1000));
    FREERTOS_log("number of task yields = %d \r\n", (uint32_t)NUM_TASK_SWITCHES);
    FREERTOS_log("time per task yield = %d ns\r\n", (uint32_t)(curTime * 1000 / (NUM_TASK_SWITCHES)));
}

/* switch from ping to ISR and back to the same task using semaphores, here there is no task switch */
void test_taskToIsrUsingSemaphoreAndNoTaskSwitch(void)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of usecs */
    HwiP_Params hwiParams;
    HwiP_Handle hHwi;
    HwiP_Status hwiStatus;

    HwiP_Params_init(&hwiParams);
/* Need to configure event Id for c66x due to eventCombiner */
#ifdef _TMS320C6X
    hwiParams.evtId = PING_EVT_ID;
#endif
    hHwi = HwiP_create(PING_INT_NUM, ping_isr_1, &hwiParams);
    DebugP_assert(hHwi != NULL);


    count = NUM_TASK_SWITCHES;
    ping_isr_1_count = 0;
    curTime = uiPortGetRunTimeCounterValue();
    while (count--)
    {
        HwiP_post(PING_INT_NUM);
        xSemaphoreTake(gPingSem, portMAX_DELAY); /* wait for ISR to signal */
    }
    curTime = uiPortGetRunTimeCounterValue() - curTime;

    hwiStatus = HwiP_delete(hHwi);
    DebugP_assert(hwiStatus == HwiP_OK);

    FREERTOS_log("\r\n");
    FREERTOS_log("execution time for task - ISR - task switches = %d ms\r\n", (uint32_t)(curTime/1000));
    FREERTOS_log("number of task switches = %d, num ISRs = %d \r\n", (uint32_t)NUM_TASK_SWITCHES, ping_isr_1_count);
    FREERTOS_log("time per task - ISR - task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime * 1000 / (NUM_TASK_SWITCHES)));
}

/* switch from ping to ISR and back to the same task using direct-to-task notify, here there is no task switch */
void test_taskToIsrUsingTaskNotifyAndNoTaskSwitch(void)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of usecs */
    HwiP_Params hwiParams;
    HwiP_Handle hHwi;
    HwiP_Status hwiStatus;

    HwiP_Params_init(&hwiParams);
#ifdef _TMS320C6X
    hwiParams.evtId = PING_EVT_ID;
#endif
    hHwi = HwiP_create(PING_INT_NUM, ping_isr_3, &hwiParams);
    DebugP_assert(hHwi != NULL);


    count = NUM_TASK_SWITCHES;
    ping_isr_3_count = 0;
    curTime = uiPortGetRunTimeCounterValue();
    while (count--)
    {
        HwiP_post(PING_INT_NUM);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); /* wait for ISR to signal */
    }
    curTime = uiPortGetRunTimeCounterValue() - curTime;

    hwiStatus = HwiP_delete(hHwi);
    DebugP_assert(hwiStatus == HwiP_OK);

    FREERTOS_log("\r\n");
    FREERTOS_log("execution time for task - ISR - task switches = %d ms\r\n", (uint32_t)(curTime/1000));
    FREERTOS_log("number of task switches = %d , num ISR = %d\r\n", (uint32_t)NUM_TASK_SWITCHES, ping_isr_3_count);
    FREERTOS_log("time per task - ISR - task switch (direct-to-task notification give/take) = %d ns\r\n", (uint32_t)(curTime * 1000 / (NUM_TASK_SWITCHES)));
}

/* switch from ping task to ISR to pong task and back to ping task using semaphores, here there is a task switch */
void test_taskToIsrUsingSemaphoreAndWithTaskSwitch(void)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of usecs */
    HwiP_Params hwiParams;
    HwiP_Handle hHwi;
    HwiP_Status hwiStatus;

    HwiP_Params_init(&hwiParams);
#ifdef _TMS320C6X
    hwiParams.evtId = PING_EVT_ID;
#endif
    hHwi = HwiP_create(PING_INT_NUM, ping_isr_2, &hwiParams);
    DebugP_assert(hHwi != NULL);

    count = NUM_TASK_SWITCHES;
    ping_isr_2_count = 0;
    curTime = uiPortGetRunTimeCounterValue();
    while (count--)
    {
        HwiP_post(PING_INT_NUM);
        xSemaphoreTake(gPingSem, portMAX_DELAY); /* wait for ISR to signal */
    }
    curTime = uiPortGetRunTimeCounterValue() - curTime;

    hwiStatus = HwiP_delete(hHwi);
    DebugP_assert(hwiStatus == HwiP_OK);

    FREERTOS_log("\r\n");
    FREERTOS_log("execution time for task - ISR - task - task switches = %d ms\r\n", (uint32_t)(curTime/1000));
    FREERTOS_log("number of ISRs = %d.Ping ISR Count:%d, Pong ISR Count:%d  \r\n", (uint32_t)NUM_TASK_SWITCHES * 2, ping_isr_2_count, pong_isr_2_count);
    FREERTOS_log("time per task - ISR - task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime * 1000 / (2 * NUM_TASK_SWITCHES)));
}

/* switch from ping task to ISR to pong task and back to ping task using direct-to-task notify, here there is task switch */
void test_taskToIsrUsingTaskNotifyAndWithTaskSwitch(void)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of usecs */
    HwiP_Params hwiParams;
    HwiP_Handle hHwi;
    HwiP_Status hwiStatus;

    HwiP_Params_init(&hwiParams);
#ifdef _TMS320C6X
    hwiParams.evtId = PING_EVT_ID;
#endif
    hHwi = HwiP_create(PING_INT_NUM, ping_isr_4, &hwiParams);
    DebugP_assert(hHwi != NULL);

    count = NUM_TASK_SWITCHES;
    ping_isr_4_count = 0;
    curTime = uiPortGetRunTimeCounterValue();
    while (count--)
    {
        HwiP_post(PING_INT_NUM);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); /* wait for ISR to signal */
    }
    curTime = uiPortGetRunTimeCounterValue() - curTime;

    hwiStatus = HwiP_delete(hHwi);
    DebugP_assert(hwiStatus == HwiP_OK);

    FREERTOS_log("\r\n");
    FREERTOS_log("execution time for task - ISR - task switches = %d ms\r\n", (uint32_t)(curTime/1000));
    FREERTOS_log("number of task switches = %d \r\n", (uint32_t)NUM_TASK_SWITCHES * 2);
    FREERTOS_log("number of isrs = %d,%d \r\n", ping_isr_4_count, pong_isr_4_count);
    FREERTOS_log("time per task - ISR - task switch (direct-to-task notification give/take) = %d ns\r\n", (uint32_t)(curTime * 1000 / (NUM_TASK_SWITCHES * 2)));
}

/* switch between ping and pong tasks and do float operations in between */
void test_taskSwitchWithFloatOperations(void)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of usecs */
    double f;

#if (configFLOATING_POINT_CONTEXT==0)
    /* When configFLOATING_POINT_CONTEXT = 0 ,Any task that uses the floating point unit 
     * MUST call portTASK_USES_FLOATING_POINT()
     * before any floating point instructions are executed. 
     */
    portTASK_USES_FLOATING_POINT();
#endif

    f = 0.0;
    count = NUM_TASK_SWITCHES;
    curTime = uiPortGetRunTimeCounterValue();
    while (count--)
    {
        f = f + 0.01;
        xSemaphoreGive(gPongSem);                /* wake up pong task */
        xSemaphoreTake(gPingSem, portMAX_DELAY); /* wait for pong to signal */
    }
    curTime = uiPortGetRunTimeCounterValue() - curTime;

    FREERTOS_log("\r\n");
    FREERTOS_log("execution time for task switches = %d ms\r\n", (uint32_t)(curTime/1000));
    FREERTOS_log("number of task switches = %d \r\n", (uint32_t)NUM_TASK_SWITCHES * 2);
    FREERTOS_log("time per task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime * 1000 / (NUM_TASK_SWITCHES * 2)));
    TEST_ASSERT_UINT32_WITHIN(1, (NUM_TASK_SWITCHES / 100), (uint32_t)f);

    f = 1.07;
    count = 10;
    double e = f;
    xSemaphoreGive(gPongSem); /* signal pong to start */
    /* pong (higher priority task) sleeps for 1ms and do some floating opearion in a while loop.
     * Following pow() calculation takes places in b/w when pong sleeps */
    while (count--)
    {
        f = pow(f,1.02);
    }
    xSemaphoreGive(gPongSem); /* signal pong to stop */
    /* This will cause pong (higher priority task) to exit from while loop */
    count = 10; /* pong task is completed now */ 
    while (count--)
    {
        e = pow(e,1.02);
    }

    FREERTOS_log("\r\n");
    FREERTOS_log("value after pow() computed b/w task switch = %d.%d \r\n", (uint32_t)f, (uint32_t)((f-(uint32_t)f)*100000));
    FREERTOS_log("expected value = %d.%d \r\n", (uint32_t)e, (uint32_t)((e-(uint32_t)e)*100000));
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

    HwiP_Params_init(&hwiParams);
#ifdef _TMS320C6X
    hwiParams.evtId = PING_EVT_ID;
#endif
    hHwi = HwiP_create(PING_INT_NUM, ping_isr_5, &hwiParams);
    DebugP_assert(hHwi != NULL);

    f = 0.0;
    count = NUM_TASK_SWITCHES;
    double e = (NUM_TASK_SWITCHES / 100);
    while (count--)
    {
        f = f + 0.01;
        HwiP_post(PING_INT_NUM);
        xSemaphoreTake(gPingSem, portMAX_DELAY); /* wait for ISR to signal */
    }

    hwiStatus = HwiP_delete(hHwi);
    DebugP_assert(hwiStatus == HwiP_OK);
    
    TEST_ASSERT_DOUBLE_WITHIN(0.01, e, f);
    FREERTOS_log("\r\n");
    FREERTOS_log("test_taskToIsrWithFloatOperations PASSED \r\n");
}

/* wait some msecs, this is just to show how delay API can be used, 
* there is no need to delay before deleting the task 
*/
void test_taskDelay(void)
{
    volatile uint64_t curTime; /* time in units of usecs */
    uint32_t delay1 = 30 * 1000, delay2 = 30 * 1000; /* in msecs */

    curTime = uiPortGetRunTimeCounterValue();
    /* convert to ticks before pass to vTaskDelay */
    vTaskDelay(delay1 / portTICK_PERIOD_MS);
    vTaskDelay(delay2 / portTICK_PERIOD_MS);
    curTime = uiPortGetRunTimeCounterValue() - curTime;
#if defined(SOC_J721E) && defined(BUILD_C7X)
    TEST_ASSERT_UINT32_WITHIN(portTICK_PERIOD_MS * 100000, (delay1 + delay2) * 1000, (uint32_t)curTime);
#else 
    TEST_ASSERT_UINT32_WITHIN(portTICK_PERIOD_MS * 1000, (delay1 + delay2) * 1000, (uint32_t)curTime);
#endif
}

void ping_main(void *args)
{
    FREERTOS_log("Starting freertos ut\n");
    UNITY_BEGIN();

    RUN_TEST(test_taskSwitchWithSemaphore, 1, NULL);
    RUN_TEST(test_taskSwitchWithTaskNotify, 2, NULL);
    RUN_TEST(test_taskYeild, 3, NULL);
    RUN_TEST(test_taskToIsrUsingSemaphoreAndNoTaskSwitch, 4, NULL);
    RUN_TEST(test_taskToIsrUsingTaskNotifyAndNoTaskSwitch, 5, NULL);
    RUN_TEST(test_taskToIsrUsingSemaphoreAndWithTaskSwitch, 6, NULL);
    RUN_TEST(test_taskToIsrUsingTaskNotifyAndWithTaskSwitch, 7, NULL);
    RUN_TEST(test_taskSwitchWithFloatOperations, 8, NULL);
    RUN_TEST(test_taskToIsrWithFloatOperations, 9, NULL);
    RUN_TEST(test_taskDelay, 10, NULL);

    UNITY_END();

#if defined LDRA_DYN_COVERAGE_EXIT
    FREERTOS_log("\n LDRA ENTRY... \n");
    upload_execution_history();
    FREERTOS_log("\n LDRA EXIT... \n");
#endif

    if (Unity.TestFailures == 0)
    {
        FREERTOS_log("\r\n");
        FREERTOS_log("All tests have passed!!\r\n");
    }
    /* One MUST not return out of a FreeRTOS task instead one MUST call vTaskDelete */
    vTaskDelete(NULL);
}

void pong_main(void *args)
{
    uint32_t count; /* loop `count` times */

    count = NUM_TASK_SWITCHES;
    while (count--)
    {
        xSemaphoreTake(gPongSem, portMAX_DELAY); /* wait for ping to signal */
        xSemaphoreGive(gPingSem);                /* wakeup ping task */
    }

    count = NUM_TASK_SWITCHES;
    while (count--)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); /* wait for ping to signal */
        xTaskNotifyGive(gPingTask);              /* wake up ping task */
    }
    {
        HwiP_Params hwiParams;
        HwiP_Handle hHwi;
        HwiP_Status hwiStatus;

        HwiP_Params_init(&hwiParams);
#ifdef _TMS320C6X
        hwiParams.evtId = PONG_EVT_ID;
#endif
        hHwi = HwiP_create(PONG_INT_NUM, pong_isr_2, &hwiParams);
        DebugP_assert(hHwi != NULL);

        count = NUM_TASK_SWITCHES;
        pong_isr_2_count = 0;
        while (count--)
        {
            xSemaphoreTake(gPongSem, portMAX_DELAY); /* wait for ISR to signal */
            HwiP_post(PONG_INT_NUM);
        }
        hwiStatus = HwiP_delete(hHwi);
        DebugP_assert(hwiStatus == HwiP_OK);
    }
    {
        HwiP_Params hwiParams;
        HwiP_Handle hHwi;
        HwiP_Status hwiStatus;

        HwiP_Params_init(&hwiParams);
#ifdef _TMS320C6X
        hwiParams.evtId = PONG_EVT_ID;
#endif
        hHwi = HwiP_create(PONG_INT_NUM, pong_isr_4, &hwiParams);
        DebugP_assert(hHwi != NULL);

        count = NUM_TASK_SWITCHES;
        pong_isr_4_count = 0;
        while (count--)
        {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY); /* wait for ISR to signal */
            HwiP_post(PONG_INT_NUM);
        }
        hwiStatus = HwiP_delete(hHwi);
        DebugP_assert(hwiStatus == HwiP_OK);
    }
    { /* switch between ping and pong tasks and do float operations in between */
        double f;

    #if (configFLOATING_POINT_CONTEXT==0)
        /* When configFLOATING_POINT_CONTEXT = 0 ,Any task that uses the floating point unit 
         * MUST call portTASK_USES_FLOATING_POINT()
         * before any floating point instructions are executed. 
         */
        portTASK_USES_FLOATING_POINT();
    #endif

        f = 10.0;

        count = NUM_TASK_SWITCHES;
        while (count--)
        {
            f = f + 0.1;
            xSemaphoreTake(gPongSem, portMAX_DELAY); /* wait for ping to signal */
            xSemaphoreGive(gPingSem);                /* wakeup ping task */
        }

        f = 2.3;
        xSemaphoreTake(gPongSem, portMAX_DELAY); /* wait for ping to signal */
        while (1)
        {
            vTaskDelay(1);
            f = pow(f, 1.3);
            if(xSemaphoreTake(gPongSem, 0)) /* check if ping signalled to stop */
            {
                break;
            }
        }
    }

    /* One MUST not return out of a FreeRTOS task instead one MUST call vTaskDelete */
    vTaskDelete(NULL);
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_freertos_main(void *args)
{
    /* Open drivers to open the UART driver for console */
    //Drivers_open();

    /* first create the semaphores */
    gPingSem = xSemaphoreCreateBinaryStatic(&gPingSemObj);
    configASSERT(gPingSem != NULL);
    gPongSem = xSemaphoreCreateBinaryStatic(&gPongSemObj);
    configASSERT(gPongSem != NULL);

    /* then create the tasks, order of task creation does not matter for this example */
    gPongTask = xTaskCreateStatic(pong_main,      /* Pointer to the function that implements the task. */
                                  "pong",         /* Text name for the task.  This is to facilitate debugging only. */
                                  PONG_TASK_SIZE, /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                  NULL,           /* We are not using the task parameter. */
                                  PONG_TASK_PRI,  /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                  gPongTaskStack, /* pointer to stack base */
                                  &gPongTaskObj); /* pointer to statically allocated task object memory */
    configASSERT(gPongTask != NULL);

    gPingTask = xTaskCreateStatic(ping_main,      /* Pointer to the function that implements the task. */
                                  "ping",         /* Text name for the task.  This is to facilitate debugging only. */
                                  PING_TASK_SIZE, /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                  NULL,           /* We are not using the task parameter. */
                                  PING_TASK_PRI,  /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                  gPingTaskStack, /* pointer to stack base */
                                  &gPingTaskObj); /* pointer to statically allocated task object memory */
    configASSERT(gPingTask != NULL);

    /* Dont close drivers to keep the UART driver open for console */
    /* Drivers_close(); */
}

#if defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
#include <ti/csl/arch/csl_arch.h>

void InitMmu(void)
{
    Osal_initMmuDefault();
}


#endif
