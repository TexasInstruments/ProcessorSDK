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
 *  \file   osal_extended_testapp_event.c
 *
 *  \brief  OSAL Event Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_INT_NUM_IRQ      (29U)
/* Offset for uxEventMirror of EventP safertos object structure */
#if defined(BUILD_C7X)
#define OSAL_APP_EVTMIRROR_OFFSET (0x0EU*2U)
#define OSAL_APP_EVTHANDLE_OFFSET (0x1EU)
#else
#define OSAL_APP_EVTMIRROR_OFFSET (0x0EU)
#define OSAL_APP_EVTHANDLE_OFFSET (0x0FU)
#endif
#if defined (FREERTOS)
#define OSAL_APP_MAX_EVENT        (OSAL_FREERTOS_CONFIGNUM_EVENT)
#elif defined (SAFERTOS)
#define OSAL_APP_MAX_EVENT        (OSAL_SAFERTOS_CONFIGNUM_EVENT)
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile EventP_Handle gOsalAppISRCtxEventHandle;
volatile uint32_t gOsalAppEventMaskA = EventP_ID_01, gOsalAppEventMaskB = EventP_ID_02, gOsalAppISRisExecuted = 0U;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */


/*
 * Description  : Event Callback Function
 */
static void OsalApp_eventIRQ(void *arg);

#if defined(SAFERTOS)
/*
 * Description: Testing Negative condition for eventPost APIs on safertos
 */
static int32_t OsalApp_eventSafeNegPostTest(void);
#endif

/*
 * Description  : Test EventP_post and EventP_getPostedEvents APIs from ISR context
 */
static int32_t OsalApp_isInISRCtxEventTest(void);

/*
 * Description: Testing Null checks for Event APIs
 */
static int32_t OsalApp_eventNullTest(void);

/*
 * Description: Testing negative check for Used parameter of Event structure
 */
static int32_t OsalApp_eventisUsedTest(void);

/*
 * Description: Tests creation of maximum supported events
 */
static int32_t OsalApp_maxEventTest(void);

/* ========================================================================== */
/*                          Internal Function Definitions                     */
/* ========================================================================== */

static void OsalApp_eventIRQ(void *arg)
{
    gOsalAppISRisExecuted = 1U;
    EventP_post(gOsalAppISRCtxEventHandle, gOsalAppEventMaskA);

    if((gOsalAppEventMaskB & EventP_getPostedEvents(gOsalAppISRCtxEventHandle)) != gOsalAppEventMaskB)
    {
        gOsalAppISRisExecuted = 0U;
    }
}

#if defined(SAFERTOS) 
static int32_t OsalApp_eventSafeNegPostTest(void)
{
    EventP_Params   params;
    EventP_Handle   eventHandle;
    uint32_t        *handleAddr;
    int32_t         result = osal_OK;

    EventP_Params_init(&params);

    eventHandle = EventP_create(&params);
    /* Here handleAddr is used to get the memory location of the handle and eventHndl of EventP structure*/
    handleAddr = (uint32_t *)eventHandle;
    if((NULL_PTR == eventHandle) || (EventP_OK != EventP_delete(&eventHandle)))
    {
          result = osal_FAILURE;
    }
    /* This handle is already deleted, we are corrupting the content of the handle "used" parameter
     * (forcfully corrupting) and evthandle of struct EventP and 
     * passing in a corrupt handle to the driver to check how the driver reacts */
    (*handleAddr) = 1U;
    *(handleAddr + OSAL_APP_EVTHANDLE_OFFSET) = 0U;
    if((EventP_OK == EventP_post(eventHandle, EventP_ID_01)) || (0U != EventP_getPostedEvents(eventHandle)))
    {
        result = osal_FAILURE;
    }
    if(EventP_OK == EventP_delete(&eventHandle))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\t Safertos Negative test for Eventpost has failed!! \n");
    }

    return result;
}

static int32_t OsalApp_isInISRCNegEventTest(void)
{
    uint32_t      interruptNum = OSAL_APP_INT_NUM_IRQ, timeout = 0x10000U;
    int32_t       result = osal_OK;
    HwiP_Params   hwipParams;
    HwiP_Handle   handle;
    EventP_Params eventParams;

    EventP_Params_init(&eventParams);
    HwiP_Params_init(&hwipParams);
    handle = HwiP_create(interruptNum, (HwiP_Fxn)OsalApp_eventIRQ, &hwipParams);
    gOsalAppISRCtxEventHandle = EventP_create(&eventParams);

    /* Storing the handle, Eventhandler in the temp which will be corrupted later */
    uint32_t * handleAddr = (uint32_t *)gOsalAppISRCtxEventHandle;
    uint32_t temp = *(handleAddr + OSAL_APP_EVTHANDLE_OFFSET);
    if((NULL_PTR == handle) || (NULL_PTR == gOsalAppISRCtxEventHandle))
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        HwiP_enableInterrupt(interruptNum);

        /* Corrupting the eventHandler with zero value*/
        *(handleAddr + OSAL_APP_EVTHANDLE_OFFSET) = 0U;
#if defined(BUILD_C66X)
        /* Posting interrupt is not supported fro c66x cores */
        if(osal_UNSUPPORTED != HwiP_post(interruptNum))
#else
        if(HwiP_OK != HwiP_post(interruptNum))
#endif
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        /* Wait for software timeout, ISR should hit
        * otherwise return the test as failed */
        while(timeout--)
        {
            if(1U == gOsalAppISRisExecuted)
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

    if(NULL_PTR != handle)
    {
        if(HwiP_OK != HwiP_delete(handle))
        {
            result = osal_FAILURE;
        }
    }
    /* Restoring eventHandler value and deleting the event */
    *(handleAddr + OSAL_APP_EVTHANDLE_OFFSET) = temp;
    if(NULL_PTR != gOsalAppISRCtxEventHandle)
    {
        if(EventP_OK != EventP_delete((EventP_Handle)&gOsalAppISRCtxEventHandle))
        {
            result = osal_FAILURE;
        }
    }

    if(result != osal_OK)
    {
        OSAL_log("\t ISR context Negative test for Event has failed!! \n");
    }
    
    return result;
}

#endif

static int32_t OsalApp_isInISRCtxEventTest(void)
{
    uint32_t      interruptNum = OSAL_APP_INT_NUM_IRQ, timeout = 0x10000U;
    uint32_t      unionEvent = gOsalAppEventMaskA | gOsalAppEventMaskB;
    int32_t       result = osal_OK;
    HwiP_Params   hwipParams;
    HwiP_Handle   handle;
    EventP_Params eventParams;

    EventP_Params_init(&eventParams);
    HwiP_Params_init(&hwipParams);
    handle = HwiP_create(interruptNum, (HwiP_Fxn)OsalApp_eventIRQ, &hwipParams);
    gOsalAppISRCtxEventHandle = EventP_create(&eventParams);

    if((NULL_PTR == handle) || (NULL_PTR == gOsalAppISRCtxEventHandle))
    {
        result = osal_FAILURE;
    }

#if !defined(BUILD_C66X)
    if(osal_OK == result)
    {
        HwiP_enableInterrupt(interruptNum);

        EventP_post(gOsalAppISRCtxEventHandle, gOsalAppEventMaskB);

        if(HwiP_OK != HwiP_post(interruptNum))
        {
            result = osal_FAILURE;
        }
    }
#endif

    if(osal_OK == result)
    {
        /* Wait for software timeout, ISR should hit
        * otherwise return the test as failed */
        while(timeout--)
        {
            if(1U == gOsalAppISRisExecuted)
            {
                break;
            }
        }
#if !defined(BUILD_C66X)
        /* Wait is over - did not get any interrupts posted/received
        * declare the test as fail
        */
        if((0U == timeout) || ((unionEvent & EventP_getPostedEvents(gOsalAppISRCtxEventHandle)) != unionEvent))
        {
            result = osal_FAILURE;
        }
#endif
    }

    if(NULL_PTR != handle)
    {
        if(HwiP_OK != HwiP_delete(handle))
        {
            result = osal_FAILURE;
        }
    }

    if(NULL_PTR != gOsalAppISRCtxEventHandle)
    {
        if(EventP_OK != EventP_delete((EventP_Handle)&gOsalAppISRCtxEventHandle))
        {
            result = osal_FAILURE;
        }
    }

    if(result != osal_OK)
    {
        OSAL_log("\t ISR context test for Event has failed!! \n");
    }
    
    return result;
}

static int32_t OsalApp_eventNullTest(void)
{
    EventP_Handle   nullPtr = NULL_PTR;
    int32_t         result = osal_OK;

    if((EventP_OK == EventP_post(nullPtr, EventP_ID_01)) || (0U != EventP_getPostedEvents(nullPtr)))
    {
        result = osal_FAILURE;
    }
    if(0U != EventP_wait(nullPtr, EventP_ID_01, EventP_WaitMode_ALL, EventP_WAIT_FOREVER))
    {
        result = osal_FAILURE;
    }
    if(EventP_OK == EventP_delete(&nullPtr))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log(" EventP Null Test Failed! \n");
    }

    return result;
}

static int32_t OsalApp_eventisUsedTest(void)
{
    EventP_Params   params;
    EventP_Handle   eventHandle;
    uint32_t        *handleAddr;
    int32_t         result = osal_OK;

    EventP_Params_init(&params);

    eventHandle = EventP_create(&params);

    /* Here handleAddr is used to get the memory location of the handle and uxEventMirror */
    handleAddr = (uint32_t *)eventHandle;
#if defined(SAFERTOS)
    uint32_t evtMirrorLAddr = (*(handleAddr + OSAL_APP_EVTMIRROR_OFFSET));
#if defined(BUILD_C7X)
    uint32_t evtMirrorHAddr = (*(handleAddr + OSAL_APP_EVTMIRROR_OFFSET+1U));
#endif
#endif
    if((NULL_PTR == eventHandle) || (EventP_OK != EventP_delete(&eventHandle)))
    {
        result = osal_FAILURE;
    }

    if(0U != EventP_wait(eventHandle, EventP_ID_01, EventP_WaitMode_ALL, EventP_WAIT_FOREVER))
    {
        result = osal_FAILURE;
    }
    /* we are corrupting the content of the handle "used" parameter
     * and passing in a corrupt handle to the driver to check how the driver reacts */
    (*handleAddr) = 1U;
#if defined(FREERTOS)
    if(EventP_OK != EventP_delete(&eventHandle))
#elif defined(SAFERTOS)
    /* restoring the EventMirror of Structure EventP_safertos->eventGroupType->uxEventMirror,
     * to get the return value of xEventGroupDelete as passed to check the gOsalEventAllocCnt
     * value whether 0 or not */
    *(handleAddr + OSAL_APP_EVTMIRROR_OFFSET) = evtMirrorLAddr;
#if defined(BUILD_C7X)
    *(handleAddr + OSAL_APP_EVTMIRROR_OFFSET+1U) = evtMirrorHAddr;
#endif
    if(EventP_OK != EventP_delete(&eventHandle))
#endif
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log(" EventP isUsed parameter negative Test Failed! \n");
    }

    return result;
}

static int32_t OsalApp_maxEventTest(void)
{
    EventP_Params   params;
    EventP_Handle   handles[OSAL_APP_MAX_EVENT];
    uint32_t        eventCnt;
    int32_t         result = osal_OK;


    EventP_Params_init(&params);
    for(eventCnt = 0U; eventCnt < OSAL_APP_MAX_EVENT; eventCnt++)
    {
        handles[eventCnt] = EventP_create(&params);
        if(NULL_PTR == handles[eventCnt])
        {
            result = osal_FAILURE;
            OSAL_log("\t Creation failed at: %d!! \n", eventCnt);
            break;
        }
    }

    if(NULL_PTR != EventP_create(&params))
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        for(eventCnt = 0U; eventCnt < OSAL_APP_MAX_EVENT; eventCnt++)
        {
            if(EventP_OK != EventP_delete(&handles[eventCnt]))
            {
                result = osal_FAILURE;
                OSAL_log("\t Deletion failed at: %d!! \n", eventCnt);
                break;
            }
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\t Maximum event creation test has failed!! \n");
    }

    return result;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_eventTests(void)
{
    int32_t result = osal_OK;

    result += OsalApp_isInISRCtxEventTest();
    result += OsalApp_eventNullTest();
    result += OsalApp_eventisUsedTest();
    result += OsalApp_maxEventTest();
#if defined(SAFERTOS)
    result += OsalApp_eventSafeNegPostTest();
    result += OsalApp_isInISRCNegEventTest();
#endif

    if(osal_OK == result)
    {
        OSAL_log("\n All Event tests have passed! \n");
    }
    else
    {
        OSAL_log("\n Some or All Event tests have failed! \n");
    }

    return result;
}

