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
 *  \file   osal_extended_testapp_mutex.c
 *
 *  \brief  OSAL Mutex Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined (FREERTOS)
#define OSAL_APP_MAX_MUTEX         (OSAL_FREERTOS_CONFIGNUM_MUTEX)
#elif defined (SAFERTOS)
#define OSAL_APP_MAX_MUTEX         (OSAL_SAFERTOS_CONFIGNUM_MUTEX)
#endif
#define OSAL_APP_INT_NUM_IRQ       (28U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t gOsalAppmutISRisExecuted = UFALSE;
volatile MutexP_Handle gOsalAppmutexhandle;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/*
 * Description: Test Mutex nonos APIs by calling in this function
 */
static int32_t OsalApp_mutexGeneralTest(void);

/*
 * Description: Null check Test for Mutex API
 */
static int32_t OsalApp_mutexNullTest(void);

/*
 * Description: Testing Negative cases for Mutex APIs
 */
static int32_t OsalApp_mutexNegativeTest(void);

#if !defined(BARE_METAL)
/*
 * Description: Maximum creation Test for Mutex API
 */
static int32_t OsalApp_mutexMaxCreateTest(void);

/*
 * Description: Testing Mutex isUsed parameter for Mutex APIs
 */
static int32_t OsalApp_isUsedMutexTest(void);

/*
 * Description: Testing Negative cases for rtos related Mutex APIs
 */
static int32_t OsalApp_mutexRtosNegativeTest(void);
#endif

/* ========================================================================== */
/*                      Internal Function Definitions                         */
/* ========================================================================== */

static void OsalApp_mutexIRQ(uintptr_t arg)
{
    gOsalAppmutISRisExecuted = UTRUE;
    MutexP_lock(gOsalAppmutexhandle, MutexP_NO_WAIT);
    MutexP_unlock(gOsalAppmutexhandle);
}

static int32_t OsalApp_isInISRMutexTest(void)
{
    uint32_t             interruptNum = OSAL_APP_INT_NUM_IRQ, timeout = 10000U;
    MutexP_Object        mutexObj;
    HwiP_Params          hwiParams;
    HwiP_Handle          hwiHandle;
    int32_t              result = osal_OK;

    HwiP_Params_init(&hwiParams);
    gOsalAppmutexhandle = MutexP_create(&mutexObj);
    hwiHandle = HwiP_create(interruptNum, (HwiP_Fxn)OsalApp_mutexIRQ, (void *)&hwiParams);
    if((NULL_PTR == hwiHandle) || (NULL_PTR == gOsalAppmutexhandle))
    {
        result = osal_FAILURE;
    }
    else
    {
        HwiP_enableInterrupt(interruptNum);

#if defined(BUILD_C66X)
        /* Posting interrupt is not supported fro c66x cores */
        if(osal_UNSUPPORTED != HwiP_post(interruptNum))
#else
        if(HwiP_OK != HwiP_post(interruptNum))
#endif
        {
            result = osal_FAILURE;
        }
        if(osal_OK == result)
        {
            /* Wait for software timeout, ISR should hit
             * otherwise return the test as failed */
            while(timeout--)
            {
                if(BTRUE == gOsalAppmutISRisExecuted)
                {
                    break;
                }
            }
            /* Wait is over - did not get any interrupts posted/received
             * declare the test as fail
             */
            if(0U == timeout)
            {
                result = osal_FAILURE;
            }
        }
        if((HwiP_OK != HwiP_delete(hwiHandle)) || (MutexP_OK != MutexP_delete(gOsalAppmutexhandle)))
        {
            result = osal_FAILURE;
        }
    }
    
    if(osal_OK != result)
    {
        OSAL_log("\t ISR context test for Mutex has failed!! \n");
    }

    return result;
}

static int32_t OsalApp_mutexGeneralTest(void)
{
    MutexP_Object         mutexObj;
    MutexP_Handle         mutexHandle;
    int32_t               result = osal_OK;

    mutexHandle = MutexP_create(&mutexObj);
    if(NULL_PTR == mutexHandle)
    {
        result = osal_FAILURE;
    }
    else
    {
        if(MutexP_OK != MutexP_lock(mutexHandle, MutexP_NO_WAIT))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || (MutexP_OK != MutexP_unlock(mutexHandle)))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || (MutexP_OK != MutexP_delete(mutexHandle)))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Mutex general test failed! \n");
    }

    return result;
}

static int32_t OsalApp_mutexNullTest(void)
{
    MutexP_Handle         mutexHandle;
    int32_t               result = osal_OK;

    mutexHandle = MutexP_create(NULL_PTR);
    if(NULL != mutexHandle)
    {
        result = osal_FAILURE;
    }
    else
    {
        if(MutexP_OK == MutexP_lock(mutexHandle, MutexP_NO_WAIT))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || (MutexP_OK == MutexP_unlock(mutexHandle)))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || (MutexP_OK == MutexP_delete(mutexHandle)))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Mutex Null check test failed! \n");
    }

    return result;
}

static int32_t OsalApp_mutexNegativeTest(void)
{
    MutexP_Object         mutexObj;
    MutexP_Handle         mutexHandle;
    uint32_t              *handleAddr,temp;
    int32_t               result = osal_OK;

    mutexHandle = MutexP_create(&mutexObj);
    if(NULL == mutexHandle)
    {
       result = osal_FAILURE;
    }
    /* Here handleAddr is used to get the memory location of the handle
     * we are corrupting the content of the handle and passing to the driver
     * to test how the driver reacts
     */
    handleAddr = (uint32_t *)mutexHandle;
    temp = (*handleAddr);
    *handleAddr = 0U;
    if(osal_OK == result)
    {
        if(MutexP_OK == MutexP_lock(mutexHandle, MutexP_WAIT_FOREVER))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || (MutexP_OK == MutexP_unlock(mutexHandle)))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || MutexP_OK == MutexP_delete(mutexHandle))
        {
            result = osal_FAILURE;
        }
    }
    /* Restoring handle */
    *(handleAddr) = temp;
    if(MutexP_OK != MutexP_delete(mutexHandle))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n MutexP nonos Negative Test has failed!! \n");
    }

    return result;
}
    
#if !defined(BARE_METAL)
static int32_t OsalApp_mutexMaxCreateTest(void)
{
    MutexP_Object         mutexObj[OSAL_APP_MAX_MUTEX];
    MutexP_Handle         mutexHandles[OSAL_APP_MAX_MUTEX];
    int32_t               result = osal_OK;
    uint32_t              index = 0U;

    for(index = 0U; index < OSAL_APP_MAX_MUTEX; index++)
    {
        mutexHandles[index] = MutexP_create(&mutexObj[index]);
        if(NULL_PTR == mutexHandles[index])
        {
            result = osal_FAILURE;
        }
    }
    if((osal_OK != result) || (NULL_PTR != MutexP_create(&mutexObj[index])))
    {
        result = osal_FAILURE;
    }
    if(osal_OK == result)
    {
        for(index = 0U; index < OSAL_APP_MAX_MUTEX; index++)
        {
            if(MutexP_OK != MutexP_delete(mutexHandles[index]))
            {
                result = osal_FAILURE;
            }
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Multiple Mutex create test failed! \n");
    }
    
    return result;
}

static int32_t OsalApp_isUsedMutexTest(void)
{
    MutexP_Object         mutexObj;
    MutexP_Handle         mutexHandle;
    uint32_t              **handleAddr = NULL;
    int32_t               result = osal_OK;

    mutexHandle = MutexP_create(&mutexObj);
    /* Here handleAddr is used to get the memory location of the handle */
    handleAddr = (uint32_t **)mutexHandle;
    if((NULL_PTR == mutexHandle) || (MutexP_OK != MutexP_delete(mutexHandle)))
    {
        result = osal_FAILURE;
    }
    if((osal_OK != result) || (MutexP_OK == MutexP_lock(mutexHandle, MutexP_NO_WAIT)))
    {
        result = osal_FAILURE;
    }
    if((osal_OK != result) || (MutexP_OK == MutexP_unlock(mutexHandle)))
    {
        result = osal_FAILURE;
    }
    /* Mutex Handle is already deleted, we are corrupting the isUsed parameter
     * and passing to the driver to test how the driver reacts
     */
    **(handleAddr) = 1U;
#if defined(SAFERTOS)
    if((osal_OK != result) || (MutexP_OK == MutexP_lock(mutexHandle, MutexP_NO_WAIT)))
    {
        result = osal_FAILURE;
    }
#endif
    if((osal_OK != result) || (MutexP_OK != MutexP_delete(mutexHandle)))
    {
       result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Mutex isUsed negative test failed! \n");
    }

    return result;
}

static int32_t OsalApp_mutexRtosNegativeTest(void)
{
    MutexP_Object         mutexObj;
    MutexP_Handle         mutexHandle;
    int32_t               result = osal_OK;

    mutexHandle = MutexP_create(&mutexObj);
    if(NULL_PTR == mutexHandle)
    {
        result = osal_FAILURE;
    }
    else
    {
        if(MutexP_OK != MutexP_lock(mutexHandle, MutexP_WAIT_FOREVER))
        {
            result = osal_FAILURE;
        }
#if defined(FREERTOS)
        if((osal_OK != result) || (MutexP_TIMEOUT != MutexP_lock(mutexHandle, MutexP_NO_WAIT)))
        {
            result = osal_FAILURE;
        }
#endif
        if((osal_OK != result) || (MutexP_OK != MutexP_unlock(mutexHandle)))
        {
            result = osal_FAILURE;
        }
#if defined(SAFERTOS)
        if((osal_OK != result) || (MutexP_TIMEOUT != MutexP_unlock(mutexHandle)))
        {
            result = osal_FAILURE;
        }
#endif
        if((osal_OK != result) || (MutexP_OK != MutexP_delete(mutexHandle)))
        {
            result = osal_FAILURE;
        }
    }
    if(osal_OK != result)
    {
        OSAL_log("\n MutexP Negative Test has failed!! \n");
    }

    return result;
}
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_mutexTests(void)
{
    int32_t result = osal_OK;

    result += OsalApp_mutexGeneralTest();
    result += OsalApp_mutexNullTest();
    result += OsalApp_isInISRMutexTest();
    result += OsalApp_mutexNegativeTest();
#if !defined(BARE_METAL)
    result += OsalApp_mutexMaxCreateTest();
    result += OsalApp_isUsedMutexTest();
    result += OsalApp_mutexRtosNegativeTest();
#endif

    if(osal_OK != result)
    {
        OSAL_log("\n Some or all MutexP tests have failed! \n");
    }
    else
    {
        OSAL_log("\n All MutexP tests have passed!! \n");
    }

    return result;
}

