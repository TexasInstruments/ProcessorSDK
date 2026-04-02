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
 *  \file   osal_extended_testapp_clock.c
 *
 *  \brief  OSAL Clock Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_INT_NUM_IRQ  29U

#if defined(FREERTOS)
#define OSAL_APP_MAX_CLOCK    OSAL_FREERTOS_CONFIGNUM_CLOCK
#elif defined(SAFERTOS)
#define OSAL_APP_MAX_CLOCK    OSAL_SAFERTOS_CONFIGNUM_CLOCK
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Clock handle used to clock stop in isr */
volatile ClockP_Handle gOsalAppclkhandle;
volatile uint8_t gOsalAppclockfxnexecute = 0U;
volatile bool gOsalAppclkISRstopExecuted = BFALSE;
volatile bool gOsalAppclkISRstartExecuted = BFALSE;

/* ========================================================================== */
/*                         Internal Function Declarations                     */
/* ========================================================================== */

#if defined (SAFERTOS)
/* 
 * Description: Negative Test for following ClockP APIs.
               1. ClockP_delete
               2. ClockP_start
               3. ClockP_stop
               4.ClockP_timerCallbackFunction
 */
static int32_t OsalApp_clockNegativeTest(void);
#endif
/* 
 * Description: Test for Clock Start ISR.
 */
static int32_t OsalApp_clockStartISRTest(void);
/* 
 * Description: Test for Clock Stop ISR.
 */
static int32_t OsalApp_clockStopISRTest(void);
/* 
 * Description: Callback function for clockp_create.
 */
static void OsalApp_clock_fxn(void *arg);
/* 
 * Description: Clock Start ISR callback function.
 */
static void OsalApp_startISR(void *arg);
/* 
 * Description: Clock Stop ISR callback function.
 */
static void OsalApp_stopISR(void *arg);
/* 
 * Description: Testing the below APIs
              1.ClockP_create
              2.ClockP_start
              3.Clockp_Stop
              4.ClockP_delete
 */
static int32_t OsalApp_clockGeneralTest(void);

#if defined (FREERTOS)
/* 
 * Description: Negative Test for following ClockP APIs
              1.ClockP_start
              2.Clockp_Stop
 */
static int32_t OsalApp_clockStartStopNegativeTest(void);
#endif

/* ========================================================================== */
/*                        Function Definitions                                */
/* ========================================================================== */

static void OsalApp_stopISR(void *arg)
{   
    gOsalAppclkISRstopExecuted = BTRUE;
    ClockP_stop(gOsalAppclkhandle);
}

static void OsalApp_startISR(void *arg)
{   
    gOsalAppclkISRstartExecuted = BTRUE;
    ClockP_start(gOsalAppclkhandle);
}

static void OsalApp_clock_fxn(void *arg)
{
    gOsalAppclockfxnexecute = 1U;
}

#if defined (SAFERTOS)
static int32_t OsalApp_clockNegativeTest(void)
{
    ClockP_Handle handle;
    ClockP_Params clockp_params;
    int32_t result = osal_OK;
    uint32_t *handleAddr;
    
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50U;  
    clockp_params.runMode = ClockP_RunMode_CONTINUOUS;
    clockp_params.startMode = ClockP_StartMode_AUTO;
    handle = ClockP_create(NULL,&clockp_params);
    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }  
      
    ClockP_Params_init(&clockp_params);
    /*ClockP_create will return NULL_PTR due to period = 0 */
    handle = ClockP_create(OsalApp_clock_fxn, &clockp_params);
    if(NULL_PTR != handle)
    {
        result = osal_FAILURE;
    }
        
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50U;  
    
    handle = ClockP_create(OsalApp_clock_fxn, &clockp_params);
    handleAddr = handle;    
    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK == result)
    {
        /* To achrive "pTimer->timerHndl = NULL" calling first time*/
        if(ClockP_OK == (ClockP_delete(handle)))
        {
          /* clock Handle is already deleted, we are corrupting the isUsed parameter
           * and passing to the driver to test how the driver reacts*/
           (*handleAddr) = 1U;
           
           /*when pTimer->timerHndl = NULL we can achrive negative test cases*/       
           if((ClockP_OK == ClockP_delete(handle)) || 
              (ClockP_OK == ClockP_start(handle)) || (ClockP_OK == ClockP_stop(handle)))
          {
              result = osal_FAILURE;
          }     
        }      
    }   
          
    if(osal_OK != result)
    {
        OSAL_log("\n Clock NegativeTest failed! \n");
    }
  
    return result;
}
#endif

static int32_t OsalApp_clockStopISRTest(void)
{
    HwiP_Params hwipParams;
    HwiP_Handle hHwi;
    int32_t   result = osal_OK;
    uint32_t timeout = 10000U;
    ClockP_Status clockpstatus;
    
    /*test case for ClockP_Params_init*/
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50U; 
    HwiP_Params_init(&hwipParams);    

    /*test case for ClockP_create */
    gOsalAppclkhandle = ClockP_create(OsalApp_clock_fxn, &clockp_params);

    hHwi = HwiP_create(OSAL_APP_INT_NUM_IRQ, (HwiP_Fxn)OsalApp_stopISR, (void *)&hwipParams);
    if((NULL_PTR == hHwi) && (NULL_PTR == gOsalAppclkhandle))
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK == result)
    {
        HwiP_enableInterrupt(OSAL_APP_INT_NUM_IRQ);
    
        clockpstatus = ClockP_start(gOsalAppclkhandle);
        if(ClockP_OK != clockpstatus)
        {
            result = osal_FAILURE;
        }

       if(HwiP_OK != HwiP_post(OSAL_APP_INT_NUM_IRQ))
       {
            result = osal_FAILURE;
       }
        
        if(osal_OK == result)
        {
            /* Wait for software timeout, ISR should hit
            * otherwise return the test as failed */
            while(timeout--)
            {
                if(1U == gOsalAppclkISRstopExecuted)
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
    
        if (ClockP_OK != ClockP_delete(gOsalAppclkhandle) || (HwiP_OK != HwiP_delete(hHwi)))
        {
            result = osal_FAILURE;
        }
    }    
    
    if(osal_OK != result)
    {
        OSAL_log("\n Clockp_Stop ISR_Test have failed! \n");
    }
    
    return result;
}

static int32_t OsalApp_clockStartISRTest(void)
{
    HwiP_Params hwipParams;
    HwiP_Handle hHwi;
    int32_t   result = osal_OK;
    uint32_t timeout = 10000U;
        
    /*test case for ClockP_Params_init*/
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50U; 
    HwiP_Params_init(&hwipParams);    

    /*test case for ClockP_create */
    gOsalAppclkhandle = ClockP_create(OsalApp_clock_fxn, &clockp_params);

    hHwi = HwiP_create(OSAL_APP_INT_NUM_IRQ, (HwiP_Fxn)OsalApp_startISR, (void *)&hwipParams);
    if((NULL_PTR == hHwi) && (NULL_PTR == gOsalAppclkhandle))
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK == result)
    {
        HwiP_enableInterrupt(OSAL_APP_INT_NUM_IRQ);
    
        if(HwiP_OK != HwiP_post(OSAL_APP_INT_NUM_IRQ))
        {
            result = osal_FAILURE;
        }
        
        if(osal_OK == result)
        {
            /* Wait for software timeout, ISR should hit
            * otherwise return the test as failed */
            while(timeout--)
            {
                if(1U == gOsalAppclkISRstartExecuted)
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
            
        if ((ClockP_OK != ClockP_stop(gOsalAppclkhandle)) || (ClockP_OK != ClockP_delete(gOsalAppclkhandle)) 
             || (HwiP_OK != HwiP_delete(hHwi)))
        {
            result = osal_FAILURE;
        }
    }    
    
    if(osal_OK != result)
    {
        OSAL_log("\n Clockp_Start ISR_Test have failed! \n");
    }
    
    return result;
}

static int32_t OsalApp_clockGeneralTest(void)
{
    ClockP_Handle handle;
    int32_t   result = osal_OK;
    ClockP_Params clockp_params;
  
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50U; 

    handle = ClockP_create(OsalApp_clock_fxn,&clockp_params);
    
    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }

    if(ClockP_OK != ClockP_start(handle))
    {
        result = osal_FAILURE;
    }

    while((osal_OK == result) && (0U == gOsalAppclockfxnexecute))
    {
        /* Do Nothing */    
    }

    if((ClockP_OK != ClockP_stop(handle)) || (ClockP_OK != ClockP_delete(handle)))
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK != result)
    {
        OSAL_log("\n OsalApp_GeneralTest has failed!!\n");
    }
       
    return result;
}


static int32_t OsalApp_NULLTests(void)
{
    ClockP_Handle handle;
    int32_t   result = osal_OK;
      
    ClockP_Params_init(NULL_PTR);
    
    handle = ClockP_create(NULL,NULL);
    if(NULL != handle)
    {
        result = osal_FAILURE;
    }
    if((ClockP_FAILURE != ClockP_start(NULL_PTR)) ||(ClockP_FAILURE != ClockP_stop(NULL_PTR)) || 
          (ClockP_FAILURE != ClockP_delete(NULL_PTR)))
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK != result)
    {
        OSAL_log("\n OsalApp_NULLTests has failed!!\n");
    }
     
    return result;
}

static int32_t OsalApp_clockMaxTest(void)
{
  uint32_t i = 0U;
  int32_t result = osal_OK;
  ClockP_Params clockp_params;
  ClockP_Handle handle[(OSAL_APP_MAX_CLOCK)];

  ClockP_Params_init(&clockp_params);
  
  clockp_params.period = 50U;  /*to get 50msec period*/
  clockp_params.runMode = ClockP_RunMode_CONTINUOUS;
  clockp_params.startMode = ClockP_StartMode_AUTO;
  
  for(i = 0U; i < (OSAL_APP_MAX_CLOCK); i++)
  {
      handle[i] = ClockP_create(OsalApp_clock_fxn,&clockp_params);
  }
  
  /*when max count reached and this creation will return NULL*/
  if(NULL != (ClockP_create(OsalApp_clock_fxn,&clockp_params)))
  {
    result = osal_FAILURE;
  }
  
  for(i = 0U; i < (OSAL_APP_MAX_CLOCK); i++)
  {
        if(ClockP_OK != ClockP_delete(handle[i]))
        {
          result = osal_FAILURE;
        }
  }
  
  if(osal_OK != result)
  {
          OSAL_log("\n OsalApp_clockMaxTest has failed!!\n");
  }
  
  return result;
}

#if defined (FREERTOS) 
static int32_t OsalApp_clockStartStopNegativeTest(void)
{
    ClockP_Handle handle;
    int32_t result = osal_OK;
    /*test case for ClockP_Params_init*/
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50U;  /*to get 50msec period*/

    /*test case for ClockP_create */
    handle = ClockP_create(OsalApp_clock_fxn,&clockp_params);
    
    /* ClockP_delete will make isused = BFALSE and start/stop
     * APIs negative case will covered
     */
    if((ClockP_OK != ClockP_delete(handle)) || 
       (ClockP_FAILURE != ClockP_start(handle)) || 
       (ClockP_FAILURE != ClockP_stop(handle)))
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK != result)
    {
        OSAL_log("\n OsalApp_clockStartStopNegativeTest has failed!!\n");
    }
    
    return result;
}
#endif    

int32_t OsalApp_clockTests(void)
{
    int32_t result = osal_OK;
    
    result += OsalApp_clockMaxTest(); 
    result += OsalApp_clockStartISRTest();
    result += OsalApp_clockStopISRTest();
    result += OsalApp_clockGeneralTest();
    result += OsalApp_NULLTests();
    #if defined (FREERTOS)  
    result +=  OsalApp_clockStartStopNegativeTest();
    #endif
    #if defined (SAFERTOS)  
    result += OsalApp_clockNegativeTest();
    #endif 
        
    if(osal_OK == result)
    {
        OSAL_log("\n All ClockP tests have passed! \n");
    }
    else
    {
        OSAL_log("\n Some or All ClockP tests have failed! \n");
    }

    return result;
}
