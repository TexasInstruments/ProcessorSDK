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
 *  \file   osal_extended_testapp_archutils.c
 *
 *  \brief  OSAL archutils Sub Module testcase for c7x file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"
#include "TimestampProvider.h"
#include "portmacro.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_TASK_STACK_SIZE  (32U*1024U)
#define OSAL_APP_TSK_IRQ_INT_NUM  (27U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint8_t      gOsalAppPortTskStack[OSAL_APP_TASK_STACK_SIZE] __attribute__((aligned(0x2000)));
uint32_t     gOsalAppProgress = UFALSE;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/*
 * Description: Testing portable layer API for c7x
 */
static int32_t OsalApp_portc7xGeneralTest(void);

/* ========================================================================== */
/*                    Internal Function Definitions                           */
/* ========================================================================== */

static void OsalApp_dummytaskFxn(void *arg)
{
    /* Do Nothing */
}

static void OsalApp_hwiTskIRQ(uintptr_t arg)
{
    gOsalAppProgress = UTRUE;
    vPortYieldAsyncFromISR();
}

static int32_t OsalApp_portc7xGeneralTest(void)
{
    HwiP_Params    hwiParams;
    TaskP_Params   params;
    TaskP_Handle   taskHandle;
    HwiP_Handle    hwiHandle;
    uint32_t       testTimeout = 100000U;
    int32_t        result = osal_OK;

    TaskP_Params_init(&params);
    HwiP_Params_init(&hwiParams);
    params.priority = 1;
    memset(gOsalAppPortTskStack, 0, sizeof(gOsalAppPortTskStack));
    params.stack    = gOsalAppPortTskStack;
    params.stacksize = sizeof(gOsalAppPortTskStack);

    hwiHandle = HwiP_create(OSAL_APP_TSK_IRQ_INT_NUM, (HwiP_Fxn)OsalApp_hwiTskIRQ, &hwiParams);
    taskHandle = TaskP_create((TaskP_Fxn)OsalApp_dummytaskFxn, &params);
    if((NULL_PTR == hwiHandle) || (NULL_PTR == taskHandle))
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        /* Check the timestamp */
        if(0U == TimestampProvider_get32())
        {
            result = osal_FAILURE;
        }

        HwiP_post(OSAL_APP_TSK_IRQ_INT_NUM);
        /* Wait till the interupt is hit */
        while((UFALSE == gOsalAppProgress) && (testTimeout--))
        {
            /* Do nothing */
        }
        gOsalAppProgress = UFALSE;
    }
    
    if((HwiP_OK != HwiP_delete(hwiHandle)) || (TaskP_OK != TaskP_delete(&taskHandle)))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n C7x portable general test have failed!!\n");
    }

#if defined(FREERTOS) && defined(BUILD_MCU)
    ulGetDataFaultStatusRegister();
    ulGetDataFaultAddressRegister();
    ulGetInstructionFaultStatusRegister();
    ulGetInstructionFaultAddressRegister();
    ulGetCPSR();
#endif

    return result;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_freertosPortableTests(void)
{
    int32_t result = osal_OK;

    result += OsalApp_portc7xGeneralTest();

    if(osal_OK == result)
    {
        OSAL_log("\n All freertos portable tests have passed!!\n");
    }
    else
    {
        OSAL_log("\n Some or All freertos portable tests have failed!!\n");
    }
    
    return result;
}

