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
 *  \file   osal_extended_testapp_load.c
 *
 *  \brief  OSAL load Sub Module testcase file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/LoadP.h>
#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#if defined(BUILD_C7X)
#define OSAL_APP_TASK_STACK_SIZE  (32U*1024U)
#else
#define OSAL_APP_TASK_STACK_SIZE  (1*1024U)
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined(SAFERTOS) && defined(BUILD_MCU)
uint8_t gOsalAppTskStack[OSAL_APP_TASK_STACK_SIZE] __attribute__((aligned(OSAL_APP_TASK_STACK_SIZE)));
#else
uint8_t gOsalAppTskStack[OSAL_APP_TASK_STACK_SIZE] __attribute__((aligned(0x2000)));
#endif
extern void LoadP_removeTask(uint32_t tskId);
extern uint32_t TaskP_getTaskId(TaskP_Handle handle);

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/*
 * Description: Testing Nullcheck for LoadP_getTaskLoad API
 */
static int32_t OsalApp_freertosLoadNullTest(void);

/*
 * Description: Testing Negative condition for LoadP_calcCounterDiff API
 */
static int32_t OsalApp_freertosLoadCounterDiffTest(void);

/*
 * Description: Testing Negative condition for LoadP_getTaskLoad API
 */
static int32_t OsalApp_taskLoadNegativeTest(void);

/* ========================================================================== */
/*                    Internal Function Definitions                           */
/* ========================================================================== */

static int32_t OsalApp_freertosLoadNullTest(void)
{
    TaskP_Handle      hLoadTestTask = NULL_PTR;
    LoadP_Stats       *loadStatsTask = NULL_PTR;
    int32_t	          result = osal_OK;

    LoadP_reset();

    if(LoadP_OK == LoadP_getTaskLoad(hLoadTestTask, loadStatsTask))
    {
        result = osal_FAILURE;
        OSAL_log("\n getTaskLoad Null test failed!\n");
    }

    return result;
}

static int32_t OsalApp_freertosLoadCounterDiffTest(void)
{
    int32_t result = osal_OK;
    uint32_t delta = 0U;

    /* Providing the last value to be greater than current value 
     * delta = ((0xFFFFFFFFU - 0xDEADDEADU) + 0xDEADBAD0U) */
    delta = LoadP_calcCounterDiff(0xDEADBAD0U, 0xDEADDEADU);
    if(0xFFFFDC22 != delta)
    {
        result = osal_FAILURE;
        OSAL_log("\n LoadP_calcCounterDiff negative test failed!\n");
    }

    return result;
}

static void OsalApp_dummytaskFxn(void *arg)
{
    /* Do Nothing */
}

static int32_t OsalApp_taskLoadNegativeTest(void)
{
    TaskP_Params    params;
    TaskP_Handle    tskHandle;
    LoadP_Stats     stats;
    int32_t         result = osal_OK;

    TaskP_Params_init(&params);
    params.priority = 1;
    memset(gOsalAppTskStack, 0, sizeof(gOsalAppTskStack));
    params.stack    = gOsalAppTskStack;
    params.stacksize = sizeof(gOsalAppTskStack);

    tskHandle = TaskP_create((TaskP_Fxn)OsalApp_dummytaskFxn, &params);
    if(NULL == tskHandle)
    {
        result = osal_FAILURE;
    }
    else
    {
        LoadP_removeTask(TaskP_getTaskId(tskHandle));
        if(LoadP_OK == LoadP_getTaskLoad(tskHandle, &stats))
        {
            result = osal_FAILURE;
        }
        if(TaskP_OK != TaskP_delete(&tskHandle))
        {
            result = osal_FAILURE;
        }
    }
    
    if(osal_OK != result)
    {
        OSAL_log("Negative test for task Load has failed!!\n");
    }

    return result;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_freertosLoadTests(void)
{
    int32_t result = osal_OK;
    
    result += OsalApp_freertosLoadNullTest();
    result += OsalApp_freertosLoadCounterDiffTest();
    result += OsalApp_taskLoadNegativeTest();

    if(osal_OK == result)
    {
        OSAL_log("\n All Load Freertos tests have passed!\n");
    }
    else
    {
        OSAL_log("\n Some or All Load Freertos tests have failed!\n");
    }
    
    return result;
}
