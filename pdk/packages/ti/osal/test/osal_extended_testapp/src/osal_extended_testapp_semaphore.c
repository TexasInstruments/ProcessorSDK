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
 *  \file   osal_extended_testapp_semaphore.c
 *
 *  \brief  OSAL Semaphore Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined(BARE_METAL)
#define OSAL_APP_MAX_SEMAPHORE    OSAL_NONOS_CONFIGNUM_SEMAPHORE
#elif defined(FREERTOS)
#define OSAL_APP_MAX_SEMAPHORE    OSAL_FREERTOS_CONFIGNUM_SEMAPHORE
#elif defined(SAFERTOS)
#define OSAL_APP_MAX_SEMAPHORE    OSAL_SAFERTOS_CONFIGNUM_SEMAPHORE
#endif
#define OSAL_APP_INT_NUM_IRQ       (30U)
#if defined(BUILD_MCU)
#define OSAL_APP_ISUSED_OFFSET     (0x25U)
#elif defined(BUILD_C7X)
#define OSAL_APP_ISUSED_OFFSET     (0x4AU)
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile SemaphoreP_Handle    gOsalAppsemhandle;
volatile bool gOsalAppsemISRisExecuted = BFALSE;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

#if !defined(BUILD_C66X)
/*
 * Description : Semaphore Callback Function
 */
static void OsalApp_semaphoreISR(void *arg);

/*
 * Description : Testing SemaphoreP_post and SemaphoreP_pend APIs from ISR context
 */
static int32_t OsalApp_isInISRsemaphoreTest(void);
#endif

/*
 * Description : Testing Null check on below semaphore APIs
 *                 1. SemaphoreP_Params_init
 *                 2. SemaphoreP_create
 *                 3. SemaphoreP_postFromISR
 *                 4. SemaphoreP_postFromClock
 *                 5. SemaphoreP_getCount
 */
static int32_t OsalApp_semaphoreNullTest(void);

/*
 * Description : Testing SemaphoreP_pend API with timeout
 */
static int32_t OsalApp_semaphorePendTest(void);

/*
 * Description : Testing Maximum semaphore creation
 */
static int32_t OsalApp_semaphoreMaxTest(void);

/*
 * Description : Testing Negative condition check for SemaphoreP_delete API
 */
static int32_t OsalApp_semaphoreNegativeTest(void);

/*
 * Description : Testing semaphore contruct APIs
 */
static int32_t OsalApp_semaphoreConstructTest(void);

/* ========================================================================== */
/*                    Internal Function Definitions                           */
/* ========================================================================== */

#if !defined(BUILD_C66X)
static void OsalApp_semaphoreISR(void *arg)
{
    gOsalAppsemISRisExecuted = BTRUE;
    SemaphoreP_post(gOsalAppsemhandle);
    SemaphoreP_pend(gOsalAppsemhandle, SemaphoreP_NO_WAIT);
    SemaphoreP_post(gOsalAppsemhandle);
}

static int32_t OsalApp_isInISRsemaphoreTest(void)
{
    uint32_t             interruptNum = OSAL_APP_INT_NUM_IRQ, timeout = 10000U;
    SemaphoreP_Params    semParams;
    HwiP_Params          hwiParams;
    HwiP_Handle          hwiHandle;
    int32_t              result = osal_OK;

    SemaphoreP_Params_init(&semParams);
    HwiP_Params_init(&hwiParams);

    semParams.mode = SemaphoreP_Mode_COUNTING;
    gOsalAppsemhandle = SemaphoreP_create(0, &semParams);
    hwiHandle = HwiP_create(interruptNum, (HwiP_Fxn)OsalApp_semaphoreISR, (void *)&hwiParams);
    if((NULL_PTR == hwiHandle) || (NULL_PTR == gOsalAppsemhandle))
    {
        result = osal_FAILURE;
    }
    else
    {
        HwiP_enableInterrupt(interruptNum);

        if(HwiP_OK != HwiP_post(interruptNum))
        {
            result = osal_FAILURE;
        }
        if(osal_OK == result)
        {
            /* Wait for software timeout, ISR should hit
             * otherwise return the test as failed */
            while(timeout--)
            {
                if(BTRUE == gOsalAppsemISRisExecuted)
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

        if((HwiP_OK != HwiP_delete(hwiHandle)) || (SemaphoreP_OK != SemaphoreP_delete(gOsalAppsemhandle)))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK != result)
    {
         OSAL_log("\t ISR context test for Semaphore has failed!! \n");
    }

    return result;
}
#endif

static int32_t OsalApp_semaphoreNullTest(void)
{
    SemaphoreP_Handle    semhandle, nullPtr = NULL_PTR;
    int32_t              result = osal_OK;
    
    SemaphoreP_Params_init(nullPtr);

    semhandle = SemaphoreP_create(0, nullPtr);
    if((nullPtr == semhandle) || (SemaphoreP_OK != SemaphoreP_delete(semhandle)))
    {
        result = osal_FAILURE;
    }
    else
    {
        if(SemaphoreP_OK == SemaphoreP_postFromISR(nullPtr))
        {
            result = osal_FAILURE;
        }
        if(SemaphoreP_OK == SemaphoreP_pend(nullPtr, SemaphoreP_WAIT_FOREVER))
        {
            result = osal_FAILURE;
        }
#if !defined(BARE_METAL)
        /* Only for RTOS */
        if(SemaphoreP_OK == SemaphoreP_postFromClock(nullPtr))
        {
            result = osal_FAILURE;
        }
#endif
        if((0U != SemaphoreP_getCount(semhandle)) || (0U != SemaphoreP_getCount(nullPtr)))
        {
            result = osal_FAILURE;
        }
        if(SemaphoreP_OK == SemaphoreP_delete(nullPtr))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Semaphore Null test has failed!\n");
    }

    return result;
}

static int32_t OsalApp_semaphorePendTest(void)
{
    SemaphoreP_Params    semParams;
    SemaphoreP_Handle    semhandle;
    int32_t              result = osal_OK;
    
    SemaphoreP_Params_init(&semParams);
    
    semParams.mode = SemaphoreP_Mode_COUNTING;
    semhandle = SemaphoreP_create(0, &semParams);
    if(NULL_PTR != semhandle)
    {
        if(SemaphoreP_OK != SemaphoreP_post(semhandle))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || (SemaphoreP_OK != SemaphoreP_pend(semhandle, SemaphoreP_NO_WAIT)))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || (0U != SemaphoreP_getCount(semhandle)))
        {
            result = osal_FAILURE;
        }

        if((osal_OK != result) || (SemaphoreP_OK != SemaphoreP_delete(semhandle)))
        {
            result = osal_FAILURE;
        }    
    }
    else
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Semaphore Pend test failed! \n");
    }
    
    return result;
}

static int32_t OsalApp_semaphoreMaxTest(void)
{
    SemaphoreP_Params    semParams;
    SemaphoreP_Handle    semhandle[OSAL_APP_MAX_SEMAPHORE];
    int32_t              result = osal_OK;
    uint32_t             index = 0U, maxindex = 0U;

    SemaphoreP_Params_init(&semParams);
    
    semParams.mode = SemaphoreP_Mode_COUNTING;
    for(index = 0U; index < OSAL_APP_MAX_SEMAPHORE; index++)
    {
        semhandle[index] = SemaphoreP_create(0, &semParams);
        if(NULL_PTR == semhandle[index])
        {
            break;
        }
    }
    maxindex = index;
    if(NULL_PTR != SemaphoreP_create(0, &semParams))
    {
        result = osal_FAILURE;
    }
    for(index = 0U ; index < maxindex; index++)
    {
        if(SemaphoreP_OK != SemaphoreP_delete(semhandle[index]))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Multiple Semaphore create test failed! \n");
    }

    return result;
}

static int32_t OsalApp_semaphoreNegativeTest(void)
{
    SemaphoreP_Params    semParams;
    SemaphoreP_Handle    semhandle;
    int32_t              result = osal_OK;

    SemaphoreP_Params_init(&semParams);
    semhandle = SemaphoreP_create(0, &semParams);
    if((NULL_PTR == semhandle) || (SemaphoreP_OK != SemaphoreP_delete(semhandle)))
    {
        result = osal_FAILURE;
    }

#if !defined (BARE_METAL)
    if(osal_OK == result)
    {
        semParams.mode = SemaphoreP_Mode_BINARY;
        /* Pass a non-binary count for a binary semaphore and see how OSAL reacts. */
        semhandle = SemaphoreP_create(10, &semParams);
        if(NULL_PTR != semhandle)
        {
            result = osal_FAILURE;
        }
    }
#endif

    if(osal_OK != result)
    {
        OSAL_log("\n Semaphore Negative test failed!\n");
    }
    return result;
}

static int32_t OsalApp_semaphoreConstructTest(void)
{
    SemaphoreP_Params    semParams;
    SemaphoreP_Handle    semhandle;
    int32_t              result = osal_OK;

    SemaphoreP_Params_init(&semParams);
  
    semParams.mode = SemaphoreP_Mode_BINARY;
    semhandle = SemaphoreP_create(0U, &semParams);
    if(NULL_PTR != semhandle)
    {
        if(SemaphoreP_OK != SemaphoreP_post(semhandle))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || (SemaphoreP_OK != SemaphoreP_pend(semhandle, SemaphoreP_WAIT_FOREVER)))
        {
            result = osal_FAILURE;
        }
        if((osal_OK != result) || (SemaphoreP_OK != SemaphoreP_delete(semhandle)))
        {
            result = osal_FAILURE;
        }
    }
#if defined(SAFERTOS)
    semParams.mode = SemaphoreP_Mode_COUNTING;
    semhandle = SemaphoreP_create(OSAL_APP_MAX_SEMAPHORE, &semParams);
    if(NULL_PTR != semhandle)
    {
        if(SemaphoreP_OK != SemaphoreP_delete(semhandle))
        {
            result = osal_FAILURE;
        }
    }
#endif
    if(osal_OK != result)
    {
        OSAL_log("\n Semaphore construct create test failed!! \n");
    }
    return result;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_semaphoreTests(void)
{
    int32_t result = osal_OK;
    
    result += OsalApp_semaphoreNullTest();
    result += OsalApp_semaphorePendTest();
    result += OsalApp_semaphoreMaxTest();
#if !defined(BUILD_C66X)
    result += OsalApp_isInISRsemaphoreTest();
#endif
    result += OsalApp_semaphoreConstructTest();
    result += OsalApp_semaphoreNegativeTest();

    if(osal_OK == result)
    {
        OSAL_log("\n All Semaphore test have passed!\n");
    }
    else
    {
        OSAL_log("\n Some or All Semaphore test have failed!\n");
    }
    
    return result;
}

