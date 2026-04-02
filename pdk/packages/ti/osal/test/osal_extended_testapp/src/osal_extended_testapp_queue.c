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
 *  \file    osal_extended_testapp_queue.c
 *
 *  \brief  OSAL Queue Sub Module testcases file.
 *
 */

/* ================================================================================== */
/*                                      Include Files                                 */
/* ================================================================================== */

#include "osal_extended_test.h"

/* ================================================================================== */
/*                                    Macros & Typedefs                               */
/* ================================================================================== */

#if defined(FREERTOS)
#define OSAL_APP_NUM_TEST_QUEUES   (OSAL_FREERTOS_MAX_QUEUEP_PER_SOC)
#endif

/* ================================================================================== */
/*                                     Global Variables                               */
/* ================================================================================== */

/* None */

/* ================================================================================== */
/*                                    Function Declarations                           */
/* ================================================================================== */

/*
 * Description : Testing Null check for below APIs
 *                  1. QueueP_put
 *                  2. QueueP_delete
 */
static int32_t OsalApp_queueNullTest(void);

/*
 * Description: Testing negative condition check for the below mentioned APIs :
 *                  1. QueueP_Params_init
 *                  2. QueueP_create
 *                  3. QueueP_getQPtr
 *                  4. QueueP_put
 *                  5. QueueP_get
 *                  6. QueueP_delete
 */
static int32_t OsalApp_queueNegativeTest(void);

/* ================================================================================== */
/*                           Internal Function Definitions                            */
/* ================================================================================== */

static int32_t OsalApp_queueNegativeTest(void)
{
    QueueP_Params params;
    QueueP_Handle handles[OSAL_APP_NUM_TEST_QUEUES], handle;
    QueueP_Status status;
    uint32_t item = 0xDEADBEAF;
    void * elem = &item;
    uint32_t queueLoop;
    uint32_t *handleAddr, temp;
    int32_t result = osal_OK;

    /* checking QueueP_Params_init else condition */
    QueueP_Params_init(&params);

    /* Creating OSAL_APP_NUM_TEST_QUEUES number of queues */
    for(queueLoop = 0U; queueLoop < OSAL_APP_NUM_TEST_QUEUES; queueLoop++)
    {
        handles[queueLoop] = QueueP_create(&params);
        /*  Handle must not be NULL */
        if((NULL_PTR == handles[queueLoop]) || (NULL == QueueP_getQPtr(handles[queueLoop])))
        {
            result = osal_FAILURE;
            break;
        }
        /* Should not be able to put a NULL item onto the queue */
        else if(QueueP_OK == QueueP_put(handles[queueLoop], NULL))
        {
            result = osal_FAILURE;
            break;
        }
    }

    if(osal_OK == result)
    {
        if(NULL != QueueP_create(&params))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        for(queueLoop = 0U; queueLoop < OSAL_APP_NUM_TEST_QUEUES; queueLoop++)
        {
            status = QueueP_delete(handles[queueLoop]);
            if(QueueP_OK != status)
            {
                result = osal_FAILURE;
                break;
            }
        }
    }

    handle = QueueP_create(&params);
    if(NULL == handle)
    {
        result = osal_FAILURE;
    }
    
    handleAddr = (uint32_t *)handle;
    temp = (*handleAddr);

    if(osal_OK == result)
    {
        
        /* Corrupt the QueueP handle and set used as false.
         * QueueP_put should not be able to put items onto the queue,
         * or even delete the queue as the queue is not effectively, not used.
         */
        (*handleAddr) = 0U;
        if(NULL_PTR != QueueP_get(handle))
        {
            result = osal_FAILURE;
        }
        if((QueueP_OK == QueueP_put(handle, elem)) || (QueueP_OK == QueueP_delete(handle)))
        {
            result = osal_FAILURE;
        }
    }

    /* Restore the halue of the handle */
    (*handleAddr) = temp; 
    if(QueueP_OK != QueueP_delete(handle))
    {
        result = osal_FAILURE;
    }
    
    /* This handle is already deleted, but we are setting the 
     * isUsed parameter to 1(forced corruption), to see how the driver reacts. */
    (*handleAddr) = 1U;
    if(QueueP_OK != QueueP_delete(handle))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n QueueP negative tests have falied! \n");
    }

    return result;
}

static int32_t OsalApp_queueNullTest(void)
{
    int32_t            result = osal_OK;
    QueueP_Handle      nullPtr = NULL_PTR;

    /* checking QueueP_Params_init else condition */
    QueueP_Params_init(NULL_PTR);
    
    if(QueueP_OK == QueueP_put(nullPtr, NULL))
    {
        result = osal_FAILURE;
    }

    if((osal_OK != result) || (QueueP_OK == QueueP_delete(nullPtr)))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n QueueP NULL test failed! \n");
    }

    return result;
}

/* ================================================================================== */
/*                                  Function Definitions                              */
/* ================================================================================== */

int32_t OsalApp_queueTests(void)
{
    int32_t result = osal_OK;

    result += OsalApp_queueNullTest();
    result += OsalApp_queueNegativeTest();

    if(osal_OK == result)
    {
        OSAL_log("\n All QueueP tests have passed! \n");
    }
    else
    {
        OSAL_log("\n Some or all QueueP tests have failed! \n");
    }

    return result;
}

