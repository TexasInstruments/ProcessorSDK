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
 *  \file   osal_extended_testapp_timer.c
 *
 *  \brief  OSAL timer Sub Module testcase file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"
#include <ti/csl/csl_timer.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_TIMER_PERIOD     (5000U)

#if defined(SOC_J721E)
  #if defined (BUILD_MCU1_0)
    #define OSAL_TEST_TIMER_ID                (2U)
  #elif defined (BUILD_C66X_1)
    #define OSAL_TEST_TIMER_ID                (2U)
  #elif defined (BUILD_C66X_2)
    #define OSAL_TEST_TIMER_ID                (2U)
  #elif defined (BUILD_C7X_1)
    #define OSAL_TEST_TIMER_ID                (0U)
  #else
    #define OSAL_TEST_TIMER_ID                (2U)
  #endif
#elif defined(SOC_J721S2)
  #if defined (BUILD_MCU1_0)
    #define OSAL_TEST_TIMER_ID                (2U)
  #elif defined (BUILD_C7X_1)
    #define OSAL_TEST_TIMER_ID                (1U)
  #elif defined (BUILD_C7X_2)
    #define OSAL_TEST_TIMER_ID                (2U)
  #else
    #define OSAL_TEST_TIMER_ID                (6U)
  #endif
#elif defined(SOC_J784S4)
  #if defined (BUILD_MCU1_0)
    #define OSAL_TEST_TIMER_ID                (2U)
  #elif defined (BUILD_C7X_1)
    #define OSAL_TEST_TIMER_ID                (1U)
  #elif defined (BUILD_C7X_2)
    #define OSAL_TEST_TIMER_ID                (2U)
  #elif defined (BUILD_C7X_3)
    #define OSAL_TEST_TIMER_ID                (3U)
  #elif defined (BUILD_C7X_4)
    #define OSAL_TEST_TIMER_ID                (4U)
  #else
    #define OSAL_TEST_TIMER_ID                (6U)
  #endif
#elif defined(SOC_J742S2)
  #if defined (BUILD_MCU1_0)
    #define OSAL_TEST_TIMER_ID                (2U)
  #elif defined (BUILD_C7X_1)
    #define OSAL_TEST_TIMER_ID                (1U)
  #elif defined (BUILD_C7X_2)
    #define OSAL_TEST_TIMER_ID                (2U)
  #elif defined (BUILD_C7X_3)
    #define OSAL_TEST_TIMER_ID                (3U)
  #else
    #define OSAL_TEST_TIMER_ID                (6U)
  #endif
#elif defined(SOC_J7200)
    #define OSAL_TEST_TIMER_ID                (2U)
#endif

#if defined(BARE_METAL)
#define OSAL_APP_TIMER_ID         (OSAL_TEST_TIMER_ID)
#else
#define OSAL_APP_TIMER_ID         (TimerP_ANY)
#endif


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/*
 * Description  : Event Callback Function
 */
static void OsalApp_dummytimerFxn(void *arg);

/*
 * Description: Testing Null tickfxn for Timer APIs
 */
static int32_t OsalApp_timerTickFxnNullTest(void);

/*
 * Description: Testing Timer APIs with different params
 */
static int32_t OsalApp_timerGeneralTest(void);

/*
 * Description: Testing invalid params condition for Timer APIs
 */
static int32_t OsalApp_timerSetMicroNegTest(void);

/* ========================================================================== */
/*                       Internal Function Definitions                        */
/* ========================================================================== */

static void OsalApp_dummytimerFxn(void *arg)
{
    /* Do Nothing */
}

static int32_t OsalApp_timerTickFxnNullTest(void)
{
    TimerP_Params params;
    TimerP_Handle tmhandle;
    int32_t result = osal_OK;

    TimerP_Params_init(&params);
    params.startMode = TimerP_StartMode_AUTO;
    params.runMode    = TimerP_RunMode_ONESHOT;
    tmhandle = TimerP_create(OSAL_APP_TIMER_ID, NULL, &params);

    if(NULL_PTR == tmhandle)
    {
        result = osal_FAILURE;
    }
    else
    {
        if(TimerP_OK != TimerP_start(tmhandle))
        {
            result = osal_FAILURE;
        }
        if((0U != TimerP_getReloadCount(tmhandle)) || (0U == TimerP_getCount(tmhandle)))
        {
            result = osal_FAILURE;
        }
        if(TimerP_OK != TimerP_stop(tmhandle))
        {
            result = osal_FAILURE;
        }
        if(TimerP_OK != TimerP_delete(tmhandle))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK != result)
    {
        OSAL_log(" \n Timer Negative test has failed!!\n");
    }
    return result;
}

static int32_t OsalApp_timerGeneralTest(void)
{
    TimerP_Params    params;
    TimerP_Handle    tmhandle;
    int32_t          result = osal_OK;

    TimerP_Params_init(&params);
    params.startMode = TimerP_StartMode_USER;
    params.runMode    = TimerP_RunMode_CONTINUOUS;

    tmhandle = TimerP_create(OSAL_APP_TIMER_ID, (TimerP_Fxn)OsalApp_dummytimerFxn, &params);
    if(NULL_PTR != tmhandle)
    {
        if(TimerP_OK != TimerP_setPeriodMicroSecs(tmhandle, OSAL_APP_TIMER_PERIOD))
        {      
            result = osal_FAILURE;
        }
        if((0U == TimerP_getReloadCount(tmhandle)) || (0U == TimerP_getCount(tmhandle)))
        {
            result = osal_FAILURE;
        }
        if(TimerP_OK != TimerP_delete(tmhandle))
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
        OSAL_log(" \n Timer test has failed!!\n");
    }
    return result;
}

static int32_t OsalApp_timerSetMicroNegTest(void)
{
    TimerP_Params    params;
    TimerP_Handle    tmhandle;
    int32_t          result = osal_OK;

    TimerP_Params_init(&params);
    params.startMode  = TimerP_StartMode_USER;
    /* Invalid run Mode */
    params.runMode    = 0xFU;

    tmhandle = TimerP_create(OSAL_APP_TIMER_ID, (TimerP_Fxn)OsalApp_dummytimerFxn, &params);
    if(NULL_PTR != tmhandle)
    {
        if(TimerP_OK != TimerP_setPeriodMicroSecs(tmhandle, 0U))
        {
            result = osal_FAILURE;
        }
        if((TimerP_OK != TimerP_start(tmhandle)) && (TimerP_OK != TimerP_stop(tmhandle)))
        {
            result = osal_FAILURE;
        }
        if(TimerP_OK != TimerP_delete(tmhandle))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK != result)
    {
        OSAL_log(" \n Timer with Invalid params test has failed!!\n");
    }
    return result;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_timerTests(void)
{
    int32_t result = osal_OK;
    result += OsalApp_timerTickFxnNullTest();
    result += OsalApp_timerGeneralTest();
    result += OsalApp_timerSetMicroNegTest();

    if(osal_OK == result)
    {
        OSAL_log("\n All Timer tests have passed!!\n");
    }
    else
    {
        OSAL_log("\n All or some timer tests have failed!!\n");
    }

    return result;
}

