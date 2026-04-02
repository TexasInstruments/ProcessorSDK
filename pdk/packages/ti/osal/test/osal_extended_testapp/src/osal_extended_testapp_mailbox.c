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
 *  \file    osal_extended_testapp_mailbox.c
 *
 *  \brief  OSAL Mailbox Sub Module testcases file.
 *
 */

/* ================================================================================== */
/*                                      Include Files                                 */
/* ================================================================================== */

#include "osal_extended_test.h"
#if defined(SAFERTOS)
#include "SafeRTOS_API.h"
#endif

/* ================================================================================== */
/*                                    Macros & Typedefs                               */
/* ================================================================================== */

#if defined(FREERTOS)
#define OSAL_APP_MAX_NUM_MAILBOX   (OSAL_FREERTOS_CONFIGNUM_MAILBOX)
#define OSAL_APP_MAILBOX_OVERHEAD  (0U)
#elif defined(SAFERTOS)
/* MailboxP_create API is called for safertos 5 times*/ 
#define OSAL_APP_MAX_NUM_MAILBOX   (OSAL_SAFERTOS_CONFIGNUM_MAILBOX - 5U)
#define OSAL_APP_MAILBOX_OVERHEAD  (safertosapiQUEUE_OVERHEAD_BYTES)
#endif
#define OSAL_APP_MB_TIMEOUT        (1U)
#define OSAL_APP_MB_SEND_STR       ("Texas Instruments\0")
#define OSAL_APP_MAX_MB_BUF_LEN    (512U)
#define OSAL_APP_IRQ_INT_NUM       (32U)
#define OSAL_APP_MB_WAIT_TIMEOUT   (~((uint32_t)0U))

/* ================================================================================== */
/*                                     Global Variables                               */
/* ================================================================================== */

/* Mailbox handle used to mailbox API's*/
volatile MailboxP_Handle gmailboxpHandle;
uint8_t gOsalAppMbQueueBuf[OSAL_APP_MAX_MB_BUF_LEN] __attribute__((aligned(64)));
uint8_t gOsalAppMbMultiCreateQueueBuf[OSAL_APP_MAX_NUM_MAILBOX][OSAL_APP_MAX_MB_BUF_LEN] __attribute__((aligned(64)));
uint8_t gOsalAppStrToSend[64] = {'T', 'e', 'x', 'a', 's', ' ', 'I', 'n', 's', 't', 'r', 'u','m', 'e', 'n', 't', 's', '\0',};
volatile bool gOsalAppmbISRisExecuted = BFALSE;
uint8_t gOsalApprecvBuf[OSAL_APP_MAX_MB_BUF_LEN];

/* ================================================================================== */
/*                            Function Declarations                             */
/* ================================================================================== */

/* 
 * Description: Test creation of as many mailboxes as supported.
 */
static int32_t OsalApp_mailboxCreateMultipleTest(void);

/* 
 * Description: Test the mailbox pend and post functionality by calling below mentioned APIs :
 *      1. MailboxP_Params_init
 *      2. MailboxP_create
 *      3. MailboxP_delete
 *      4. MailboxP_post
 *      5. MailboxP_pend
 */
static int32_t OsalApp_mailboxPendPostTest(uint32_t timeout);

/* 
 * Description: Test the mailbox ISR call back function..
 */
static void OsalApp_mbISR(void *arg);

/*
 * Description : Testing from ISR context on below mailbox APIs
 *      1. MailboxP_post
 *      2. MailboxP_pend
 *      3. MailboxP_getNumPendingMsgs
 */
static int32_t OsalApp_mailboxISRTest(void);

#if defined(SAFERTOS)
/* 
 * Description: Test the mailbox create Negative condition..
 */
static int32_t osalApp_mailboxCreateNegativeTest(void);

/* 
 * Description: Test the mailbox post and pend Negative condition..
 */
static int32_t OsalApp_mailboxPostPendNegativeTest(void);

#endif

#if defined(FREERTOS)

/* 
 * Description: Test the mailbox Delete by making handle as NULL_PTR..
 */
static int32_t OsalApp_mailboxDeleteNegativeTest(void);

/*
 * Description  : Tests MailboxP create when the used parameter of mailbox is 0.
 */
static int32_t OsalApp_mailboxUsedTest(void);

/* 
 * Description: Test the mailbox pend negative condition
 */
static int32_t OsalApp_mailboxPendNegativeTest(void);

#endif

/* ================================================================================== */
/*                               Internal Function Definitions                        */
/* ================================================================================== */

static void OsalApp_mbISR(void *arg)
{   
    uint32_t iter;
    uint32_t len = strlen((char *)gOsalAppStrToSend);
    
    for(iter = 0U; iter < len; iter++)
    {
        MailboxP_post(gmailboxpHandle, &gOsalAppStrToSend[iter], OSAL_APP_MB_TIMEOUT);
    }
    
    MailboxP_getNumPendingMsgs(gmailboxpHandle);

    for(iter = 0U; iter < len; iter++)
    {
        MailboxP_pend(gmailboxpHandle, &gOsalApprecvBuf[iter], OSAL_APP_MB_TIMEOUT);
    }
    
    gOsalAppmbISRisExecuted = BTRUE;
}

static int32_t OsalApp_mailboxCreateMultipleTest(void)
{
    MailboxP_Params params;
    MailboxP_Handle handles[OSAL_APP_MAX_NUM_MAILBOX];
    uint32_t mbindex;
    int32_t result = osal_OK;
    
    MailboxP_Params_init(NULL_PTR);
    MailboxP_Params_init(&params);
    params.count = strlen((char *)gOsalAppStrToSend);
    params.size  = sizeof(uint8_t);
    params.bufsize = (params.count * params.size) + OSAL_APP_MAILBOX_OVERHEAD;

    for(mbindex = 0U; mbindex < (OSAL_APP_MAX_NUM_MAILBOX); mbindex++)
    {
        params.buf = gOsalAppMbMultiCreateQueueBuf[mbindex];
        handles[mbindex] = MailboxP_create(&params);
        if(NULL == handles[mbindex])
        {
            result = osal_FAILURE;
            break;
        }
    }

    if(osal_OK == result)
    {
        if(NULL != MailboxP_create(&params))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        for(mbindex = 0U; mbindex < (OSAL_APP_MAX_NUM_MAILBOX); mbindex++)
        {
#if defined (SAFERTOS)
            /* MailboxP_delete is a dummy call in SafeRTOS. */
            MailboxP_delete(handles[mbindex]);
#elif defined (FREERTOS)
            if( MailboxP_OK != MailboxP_delete(handles[mbindex]))
            {
                result = osal_FAILURE;
                break;
            }
#endif
        }
    }
    
    if(osal_OK == result)
    {
        OSAL_log("\n Multiple mailbox create test has passed!!\n");
    }
    else
    {
        OSAL_log("\n Multiple mailbox create test has failed!!\n");
    }

    return result;
}

static int32_t OsalApp_mailboxPendPostTest(uint32_t timeout)
{
    MailboxP_Params params;
    MailboxP_Handle handle;
    MailboxP_Status status = MailboxP_OK;
    uint32_t index;
    int32_t result = osal_OK;
    uint8_t recvBuf[OSAL_APP_MAX_MB_BUF_LEN];

    MailboxP_Params_init(&params);
    params.count = strlen((char *)gOsalAppStrToSend);
    params.size  = sizeof(uint8_t);
    memset(gOsalAppMbQueueBuf, 0U, sizeof(gOsalAppMbQueueBuf)/sizeof(gOsalAppMbQueueBuf[0]));
    params.buf = gOsalAppMbQueueBuf;
    params.bufsize = (params.count * params.size) + OSAL_APP_MAILBOX_OVERHEAD;

    handle = MailboxP_create(&params);
    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK == result)
    {
        for(index = 0U; index < params.count; index++)
        {
            status += MailboxP_post(handle, &gOsalAppStrToSend[index], timeout);
        }
        if(MailboxP_OK != status)
        {
            result = osal_FAILURE;
        }
    }
    
    if(osal_OK == result)
    {
        for(index = 0U; index < params.count; index++)
        {
            status += MailboxP_pend(handle, &recvBuf[index], timeout);
        }
        if(MailboxP_OK != status)
        {
            result = osal_FAILURE;
        }
    }
    
    if((osal_OK != result) ||
       (0U != MailboxP_getNumPendingMsgs(handle)) || 
       (0U != strncmp((char *)gOsalAppStrToSend, (char *)recvBuf, params.count)))
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        for(index = 0U; index < params.count; index++)
        {
            status += MailboxP_post(handle, &gOsalAppStrToSend[index], timeout);
        }
        if(MailboxP_OK != status)
        {
            result = osal_FAILURE;
        }
        else
        {
            /* Posting the mailbox one additional time, should cause it to fail. */
            if(MailboxP_OK == MailboxP_post(handle, &gOsalAppStrToSend[index], 0))
            {
                result = osal_FAILURE;
            }
        }
    }

    if(osal_OK == result)
    {
#if defined (FREERTOS)
        if(MailboxP_OK != MailboxP_delete(handle))
#elif defined (SAFERTOS)
        /* MailboxP_delete is a dummy call in SafeRTOS. */
        if(MailboxP_FAILURE != MailboxP_delete(handle))
#endif
        {
            result = osal_FAILURE;
        }
    }
    
    if(osal_OK == result)
    {
        OSAL_log("\n MailboxP test passed!!\n");
    }
    else
    {
        OSAL_log("\n MailboxP test failed!!\n");
    }

    return result;
}

static int32_t OsalApp_mailboxISRTest(void)
{
    int32_t result = osal_OK;
    HwiP_Params hwipParams;
    HwiP_Handle hHwi;
    MailboxP_Params params;
    uint32_t        timeout = 10000U;

    MailboxP_Params_init(&params);
    params.count = strlen((char *)gOsalAppStrToSend);
    params.size  = sizeof(uint8_t);
    memset(gOsalAppMbQueueBuf, 0U, sizeof(gOsalAppMbQueueBuf)/sizeof(gOsalAppMbQueueBuf[0]));
    params.buf = gOsalAppMbQueueBuf;
    params.bufsize = (params.count * params.size) + OSAL_APP_MAILBOX_OVERHEAD;

    gmailboxpHandle = MailboxP_create(&params);
    if(NULL == gmailboxpHandle)
    {
        result = osal_FAILURE;
    }

    HwiP_Params_init(&hwipParams);

    hHwi = HwiP_create(OSAL_APP_IRQ_INT_NUM, (HwiP_Fxn)OsalApp_mbISR, (void *)&hwipParams);
    if(NULL_PTR == hHwi) 
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        HwiP_enableInterrupt(OSAL_APP_IRQ_INT_NUM);
#if defined(_TMS320C6X)
        if(osal_UNSUPPORTED != HwiP_post(OSAL_APP_IRQ_INT_NUM))
#else
        if(HwiP_OK != HwiP_post(OSAL_APP_IRQ_INT_NUM))
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
                if(BTRUE == gOsalAppmbISRisExecuted)
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
      
#if defined (FREERTOS)
        if(MailboxP_OK != MailboxP_delete(gmailboxpHandle))
#elif defined (SAFERTOS)
        /* MailboxP_delete is a dummy call in SafeRTOS. */
        if(MailboxP_FAILURE != MailboxP_delete(gmailboxpHandle))
#endif
        {
            result = osal_FAILURE;
        }
    }
#if !defined(_TMS320C6X)
    if(0U != memcmp((char *)gOsalApprecvBuf, (char *)gOsalAppStrToSend, params.count))
    {
        result = osal_FAILURE;       
    }
#endif
    if(HwiP_OK != HwiP_delete(hHwi))
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        OSAL_log("\n Mailbox ISR test has passed!!\n");
    }
    else
    {
        OSAL_log("\n Mailbox ISR test has failed!!\n");      
    }

    return result;
}

#if defined(SAFERTOS)
static int32_t osalApp_mailboxCreateNegativeTest(void)
{
    MailboxP_Params params;
    MailboxP_Handle handle;
    int32_t result = osal_OK;

    MailboxP_Params_init(&params);
    params.count = strlen((char *)gOsalAppStrToSend);
    params.size  = sizeof(uint8_t);
    memset(gOsalAppMbQueueBuf, 0U, sizeof(gOsalAppMbQueueBuf)/sizeof(gOsalAppMbQueueBuf[0]));
    params.buf = gOsalAppMbQueueBuf;
    params.bufsize = params.count * params.size;

    handle = MailboxP_create(&params);
    if(NULL_PTR != handle)
    {
        result = osal_FAILURE; 
    }
  
    if(osal_OK == result)
    {
        OSAL_log("\n Mailbox CreateNegativeTest has passed!!\n");
    }
    else
    {
        OSAL_log("\n Mailbox CreateNegativeTest has failed!!\n");
    }
  
    return result;
}

static int32_t OsalApp_mailboxPostPendNegativeTest(void)
{
    MailboxP_Params params;
    MailboxP_Handle handle;
    MailboxP_Status status = MailboxP_OK;
    int32_t result = osal_OK;
    void *msg = (void *)NULL;

    MailboxP_Params_init(&params);
    params.count = strlen((char *)gOsalAppStrToSend);
    params.size  = sizeof(uint8_t);
    memset(gOsalAppMbQueueBuf, 0U, sizeof(gOsalAppMbQueueBuf)/sizeof(gOsalAppMbQueueBuf[0]));
    params.buf = gOsalAppMbQueueBuf;
    params.bufsize = (params.count * params.size) + OSAL_APP_MAILBOX_OVERHEAD;

    handle = MailboxP_create(&params);
    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK == result)
    {
        /* Mailbox post will retuen errINVALID_QUEUE_HANDLE 
        while passing msg as NULLto achrive the negative case*/
        status = MailboxP_post(handle, msg, OSAL_APP_MB_WAIT_TIMEOUT);
        
        if((MailboxP_OK == status))
        {
            result = osal_FAILURE;
        }
        
        /* Mailbox pend will retuen errINVALID_QUEUE_HANDLE 
        while passing msg as NULLto achrive the negative case*/
        status = MailboxP_pend(handle, msg, OSAL_APP_MB_WAIT_TIMEOUT);
        
        if(osal_OK == result)
        {
            /* MainlboxP_delete is a dummy call for SafeRTOS. */
            if((MailboxP_FAILURE != MailboxP_delete(handle)) || (MailboxP_OK == status))
            {
                result = osal_FAILURE;
            }
        
        }
    }
    
    if(osal_OK == result)
    {
        OSAL_log("\n Mailbox PostPend_NegativeTest has passed!!\n");
    }
    else
    {
        OSAL_log("\n Mailbox PostPend_NegativeTest has failed!!\n");
    }

    return result;
}
#endif

#if defined(FREERTOS)

static int32_t OsalApp_mailboxUsedTest(void)
{
    MailboxP_Params params;
    MailboxP_Handle handle;
    uint32_t *handleAddr, temp;
    int32_t result = osal_OK;

    MailboxP_Params_init(&params);
    params.count = strlen((char *)gOsalAppStrToSend);
    params.size  = sizeof(uint8_t);
    memset(gOsalAppMbQueueBuf, 0U, sizeof(gOsalAppMbQueueBuf)/sizeof(gOsalAppMbQueueBuf[0]));
    params.buf = gOsalAppMbQueueBuf;
    params.bufsize = (params.count * params.size) + OSAL_APP_MAILBOX_OVERHEAD;

    handle = MailboxP_create(&params);
    if(NULL == handle)
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        handleAddr = handle;
        temp = (uint32_t)(*handleAddr);
        (*handleAddr) = 0U;
        
        /* To make mailbox->used zero to achieve negative case.
        * This tests a scenario when the handle gets corrupted for some reason.
        */
        if(MailboxP_FAILURE != MailboxP_delete(handle))
        {
            result = osal_FAILURE;
        }

        /* Restore the value of the handle. */
        (*handleAddr) = temp;
        if(MailboxP_OK != (MailboxP_delete(handle)))
        {
            result = osal_FAILURE;
        }
        
       /* This handle is already deleted, but we are setting the isUsed parameter to 1(forced corruption),
        * to see how the driver reacts. */
        *handleAddr = 1U;
        if((osal_OK != result) || (MailboxP_OK != MailboxP_delete(handle)))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        OSAL_log("\n MailboxP Used test passed!!\n");
    }
    else
    {
        OSAL_log("\n MailboxP Used test failed!!\n");
    }

    return result;
}

static int32_t OsalApp_mailboxDeleteNegativeTest(void)
{
  
    MailboxP_Handle handle;
    MailboxP_Status status = MailboxP_OK;
    int32_t result = osal_OK;
   
    handle = NULL_PTR;
    status = MailboxP_delete(handle);
  
    if(status == MailboxP_OK)
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK == result)
    {
        OSAL_log("\n Mailbox Delete_NegativeTest has passed!!\n");
    }
    else
    {
        OSAL_log("\n Mailbox Delete_NegativeTest has failed!!\n");
    }

    return result;
  
}

static int32_t OsalApp_mailboxPendNegativeTest(void)
{
    MailboxP_Params    params;
    MailboxP_Handle    handle;
    int32_t            result = osal_OK;

    MailboxP_Params_init(&params);
    params.count = strlen((char *)gOsalAppStrToSend);
    params.size  = sizeof(uint8_t);
    memset(gOsalAppMbQueueBuf, 0U, sizeof(gOsalAppMbQueueBuf)/sizeof(gOsalAppMbQueueBuf[0]));
    params.buf = gOsalAppMbQueueBuf;
    params.bufsize = (params.count * params.size) + OSAL_APP_MAILBOX_OVERHEAD;

    handle = MailboxP_create(&params);
    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }
    else
    {
        /* Trying to pend without having the mailbox posted, to check the negative condition
         * This is acceptable, as we are not blocking. The timeout being used is 0U.
         */
        if(MailboxP_OK == MailboxP_pend(handle, gOsalApprecvBuf, 0U))
        {
            result = osal_FAILURE;
        }
#if defined(SAFERTOS)
        MailboxP_delete(handle);
#elif defined(FREERTOS)
        /* MainlboxP_delete is a dummy call for SafeRTOS. */
        if(MailboxP_OK != MailboxP_delete(handle))
        {
            result = osal_FAILURE;
        }
#endif
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Mailbox Pend Negative has failed!!\n");
    }

    return result;
}

#endif

int32_t OsalApp_mailboxTests(void)
{
    int32_t result = osal_OK;

#if defined(SAFERTOS)
    result += osalApp_mailboxCreateNegativeTest();
    result += OsalApp_mailboxPostPendNegativeTest();
#endif
#if defined(FREERTOS)
    result += OsalApp_mailboxDeleteNegativeTest();
    result += OsalApp_mailboxUsedTest();
    result += OsalApp_mailboxPendNegativeTest(); 
#endif
    result += OsalApp_mailboxPendPostTest(OSAL_APP_MB_TIMEOUT);
    result += OsalApp_mailboxPendPostTest(0);
    result += OsalApp_mailboxPendPostTest(OSAL_APP_MB_WAIT_TIMEOUT);
    result += OsalApp_mailboxISRTest();
    result += OsalApp_mailboxCreateMultipleTest(); 
  
    if(osal_OK == result)
    {
        OSAL_log("\n All Mailbox tests have passed!!\n");
    }
    else
    {
        OSAL_log("\n Some or all Mailbox tests have failed!!\n");
    }

    return result;
}

