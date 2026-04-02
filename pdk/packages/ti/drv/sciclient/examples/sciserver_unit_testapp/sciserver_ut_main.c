/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 *
 */

/**
 *  \file   sciserver_ut_main.c
 *
 *  \brief  Implementation of Sciserver Unit Test application
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <ti/drv/sciclient/src/sciclient/sciclient_priv.h>
#include <ti/drv/sciclient/sciserver.h>
#include <ti/drv/sciclient/sciserver_tirtos.h>
#include <ti/drv/sciclient/src/sciserver/sciserver_secproxyTransfer.h>
#include <ti/drv/sciclient/examples/common/sci_app_common.h>
#include <ti/drv/sciclient/examples/sciserver_unit_testapp/sciserver_ut_tests.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined(SOC_J784S4) || defined(SOC_J742S2) || defined(SOC_J721S2) || defined(SOC_J721E)
#define MAX_SEMAPHORE    360U
#elif defined(SOC_J7200)
#define MAX_SEMAPHORE    150U
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
/* IMPORTANT NOTE: For C7x,
 * - stack size and stack ptr MUST be 8KB aligned
 * - AND min stack size MUST be 32KB
 * - AND stack assigned for task context is "size - 8KB"
 *       - 8KB chunk for the stack area is used for interrupt handling in this task context
 */
#if defined(SAFERTOS) && defined (BUILD_MCU)
static uint8_t  gSciserverApp_TskStackMain[32*1024] __attribute__((aligned(32*1024))) = { 0 };
#else
static uint8_t  gSciserverApp_TskStackMain[32*1024] __attribute__((aligned(8192)));
#endif

extern SemaphoreP_Handle gSciserverUserSemHandles[];

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Function Declarations                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Internal Function Declarations                      */
/* ========================================================================== */

void mainTask(void* arg0, void* arg1);
static int32_t SciserverApp_serverTest(void);
static int32_t SciserverApp_secProxyTransferTest(void);
static int32_t SciserverApp_secproxyRoutingDescriptionTest(void);
static int32_t SciserverApp_rtosTest(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    uint32_t retVal = CSL_PASS;

    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    memset( gSciserverApp_TskStackMain, 0xFF, sizeof( gSciserverApp_TskStackMain ) );
    TaskP_Params_init(&taskParams);
    taskParams.priority   = 2;
    taskParams.stack      = gSciserverApp_TskStackMain;
    taskParams.stacksize  = sizeof (gSciserverApp_TskStackMain);
    task = TaskP_create(&mainTask, &taskParams);
    if(task == NULL)
    {
        OS_stop();
    }

    OS_start();

    return retVal;
}

/* ========================================================================== */
/*                        Internal Function Definitions                       */
/* ========================================================================== */

void mainTask(void* arg0, void* arg1)
{
    /*To suppress unused variable warning*/
    (void)arg0;
    (void)arg1;

    volatile uint32_t loopForever = 1U;

    SciApp_parser();

    while(loopForever);
}

uint32_t SciApp_getNumTests(void)
{
    return SCISERVER_UT_NUM_TESTCASES;
}

int32_t SciApp_testMain(SciApp_TestParams_t *testParams)
{
    switch (testParams->testcaseId)
    {
        case 1:
            testParams->testResult = SciserverApp_serverTest();
            break;
        case 2:
            testParams->testResult = SciserverApp_secProxyTransferTest();
            break;
        case 3:
            testParams->testResult = SciserverApp_secproxyRoutingDescriptionTest();
            break;
        case 4:
            testParams->testResult = SciserverApp_rtosTest();
            break;
        default:
            break;
    }
    
    return 0;
}

static int32_t SciserverApp_processtaskTest(void)
{
    int32_t   status                = CSL_PASS;
    int32_t   processtaskTestStatus = CSL_PASS;
    Sciserver_taskData utdTest      =
    {
          .task_id                  = SCISERVER_TASK_USER_HI,
          .hw_msg_buffer_list       = 0U,
          .hw_msg_buffer_count      = SCISERVER_SECPROXY_INSTANCE_COUNT,
          .hw_msg_buffer_sz         = SCISERVER_HW_QUEUE_SIZE,
          .semaphore_id             = SCISERVER_SEMAPHORE_USER_HI,
          .state                    = 0U,
          .user_msg_data            = 0U,
          .stack                    = 0U
    };
    
    /* To cover fail condition for Sciserver_processtask */
    utdTest.state->state = SCISERVER_TASK_PENDING;
    status = Sciserver_processtask(&utdTest);
    if(status == CSL_EFAIL)
    {
        processtaskTestStatus += CSL_PASS;
        SciApp_printf("Sciserver_processtask Negative Arg Test PASSED \n");
    }
    else
    {
        processtaskTestStatus += CSL_EFAIL;
        SciApp_printf("Sciserver_processtask Negative Arg Test FAILED \n");
    }

    /* To cover Sciserver_SetMsgHostId API */
    utdTest.user_msg_data[utdTest.state->current_buffer_idx]->host       = TISCI_HOST_ID_DMSC2DM;
    utdTest.user_msg_data[utdTest.state->current_buffer_idx]->is_pending = true;
    status = Sciserver_processtask(&utdTest);
    if(status == CSL_PASS)
    {
        processtaskTestStatus += CSL_PASS;
        SciApp_printf("Sciserver_SetMsgHostId test PASSED \n");
    }
    else
    {
        processtaskTestStatus += CSL_EFAIL;
        SciApp_printf("Sciserver_SetMsgHostId test FAILED \n");
    }
    
    /* Covers default test for Sciserver_UserProcessMsg */
    utdTest.state->state = SCISERVER_TASK_PROCESSING_USER_MSG;
    status = Sciserver_processtask(&utdTest);
    if (status != CSL_EFAIL)
    {
        processtaskTestStatus = CSL_PASS;
        SciApp_printf ("Sciserver_SproxyConfLookup: Negative Arg Test Passed.\n");
    }
    else
    {
       processtaskTestStatus = CSL_EFAIL;
       SciApp_printf ("Sciserver_SproxyConfLookup: Negative Arg Test Failed.\n");
    }  
  
    return processtaskTestStatus;
}

static int32_t SciserverApp_serverTest(void)
{
    int32_t status                  = CSL_PASS;
    int32_t sciserverFuncTestStatus = CSL_PASS;
    uint8_t setProcessState         = 1U;
    uint8_t getProcessState         = 0U;
    Sciserver_CfgPrms_t prms;
    uint32_t messageType[13]         =
    {
        TISCI_MSG_RM_UDMAP_FLOW_DELEGATE, TISCI_MSG_RM_IRQ_SET, TISCI_MSG_BOARD_CONFIG_PM,
        TISCI_MSG_SET_CLOCK, TISCI_MSG_GET_CLOCK, TISCI_MSG_SET_CLOCK_PARENT, TISCI_MSG_GET_CLOCK_PARENT,
        TISCI_MSG_GET_NUM_CLOCK_PARENTS, TISCI_MSG_SET_FREQ, TISCI_MSG_QUERY_FREQ, TISCI_MSG_GET_FREQ,
        TISCI_MSG_SET_DEVICE_RESETS, TISCI_MSG_BOARD_CONFIG_RM
    };
    uint8_t  num;

    /* Trying to deinit sciserver even if it was not init before to achieve fail condition */
    status = Sciserver_deinit();
    if(status == CSL_EFAIL)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf("Sciserver was not initialized \n");
    }
    else
    {
        sciserverFuncTestStatus += CSL_EFAIL;
        SciApp_printf("Sciserver_deinit \n");
    }

    /* Passing NULL parameter to Sciserver_init*/
    status = Sciserver_init(NULL);
    if(status == CSL_EBADARGS)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf("Sciserver_init NULL Arg Test PASSED \n");
    }
    else
    {
        sciserverFuncTestStatus += CSL_EFAIL;
        SciApp_printf("Sciserver_init NULL Arg Test FAILED \n");
    }

    /* Passing valid parameters to Sciserver_init */
    status = Sciserver_init(&prms);
    if(status == CSL_PASS)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf("Sciserver_init PASSED \n");
    }
    else
    {
        sciserverFuncTestStatus += CSL_EFAIL;
        SciApp_printf("Sciserver_init FAILED \n");
    }

    /* Sciserver_init already done, trying to initialize it again to achieve fail condition */
    status = Sciserver_init(&prms);
    if(status == CSL_EFAIL)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf("Sciserver_init Already Done \n");
    }
    else
    {
        sciserverFuncTestStatus += CSL_EFAIL;
        SciApp_printf("Sciserver_init Done \n");
    }

    Sciserver_setProcessState(setProcessState);
    getProcessState = Sciserver_getProcessState();
    if(getProcessState == setProcessState)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf("Sciserver_setProcessState, Sciserver_getProcessState Tests PASSED \n");
    }
    else
    {
        sciserverFuncTestStatus += CSL_EFAIL;
        SciApp_printf("Sciserver_setProcessState, Sciserver_getProcessState Tests FAILED \n");
    }

    status = Sciserver_interruptHandler(NULL, NULL);
    if(status == CSL_EFAIL)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf("Sciserver_interruptHandler Null Arg Test PASSED \n");
    }
    else
    {
        sciserverFuncTestStatus += CSL_EFAIL;
        SciApp_printf("Sciserver_interruptHandler Null Arg Test FAILED \n");
    }

    status += SciserverApp_processtaskTest();
        
    status = Sciserver_deinit();
    if(status == CSL_PASS)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf("Sciserver_deinit PASSED \n");
    }
    else
    {
        sciserverFuncTestStatus += CSL_EFAIL;
        SciApp_printf("Sciserver_deinit FAILED \n");
    }

    Sciserver_taskData utdSetMsgHostId =
    {
        .task_id                 = SCISERVER_TASK_USER_HI,
        .hw_msg_buffer_count     = SCISERVER_SECPROXY_INSTANCE_COUNT,
        .hw_msg_buffer_sz        = SCISERVER_HW_QUEUE_SIZE,
        .semaphore_id            = SCISERVER_SEMAPHORE_USER_HI,
    };
    utdSetMsgHostId.state->state = SCISERVER_TASK_PROCESSING_USER_MSG;
    /* Passing valid parameters to cover Sciserver_SetMsgHostId function */
    status = Sciserver_processtask(&utdSetMsgHostId);
    if (status == CSL_PASS)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf ("Sciserver_processtask: Sciserver_SetMsgHostId Test Passed.\n");
    }
    else
    {
       sciserverFuncTestStatus += CSL_EFAIL;
       SciApp_printf ("Sciserver_processtask: Sciserver_SetMsgHostId Test Failed.\n");
    }

    Sciserver_msgData invalid_user_msg_data = 
    {
      .host = 0U,
      .hwi_id = 0U,
      .is_pending = false
    };
    Sciserver_msgData *const invalid_user_msg_data_list[SCISERVER_SECPROXY_INSTANCE_COUNT] =
    {
      &invalid_user_msg_data,
      &invalid_user_msg_data
    };
    Sciserver_taskData utdTestFail =
    {
        .user_msg_data       = invalid_user_msg_data_list,
        .task_id             = SCISERVER_TASK_USER_HI,
        .hw_msg_buffer_count = SCISERVER_SECPROXY_INSTANCE_COUNT,
        .hw_msg_buffer_sz    = SCISERVER_HW_QUEUE_SIZE,
        .semaphore_id        = SCISERVER_SEMAPHORE_USER_HI,
    };
    utdTestFail.state->state = SCISERVER_TASK_PROCESSING_SECURE_MSG;
    utdTestFail.state->current_buffer_idx   = 1;
    /* Passing task state as SCISERVER_TASK_PROCESSING_SECURE_MSG to cover fail condition in Sciserver_processtask */
    status = Sciserver_processtask(&utdTestFail);
    if (status == CSL_EFAIL)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf ("Sciserver_processtask: Negative Arg Test Passed.\n");
    }
    else
    {
       sciserverFuncTestStatus += CSL_EFAIL;
       SciApp_printf ("Sciserver_processtask: Negative Arg Test Failed.\n");
    }

    uint32_t user_hi_msg_buffer[SCISERVER_HW_QUEUE_SIZE] =
    {
        2U, 3U  /* Values to update the tisci_flags in the Sciserver_processtask function */
    };
    uint32_t user_hi_main_msg_buffer[SCISERVER_HW_QUEUE_SIZE];
    uint32_t *const user_hi_msg_buffer_list[SCISERVER_SECPROXY_INSTANCE_COUNT] = 
    {
        user_hi_msg_buffer,
        user_hi_main_msg_buffer
    };
    invalid_user_msg_data.is_pending = true;
    Sciserver_taskData utdTisciMsgResponseFail =
    {
        .user_msg_data      = invalid_user_msg_data_list,
        .hw_msg_buffer_list = user_hi_msg_buffer_list
    };
    utdTisciMsgResponseFail.state->state                = SCISERVER_TASK_PROCESSING_USER_MSG;
    utdTisciMsgResponseFail.state->current_buffer_idx   = 1;
    /* Passing invalid user_msg_data to cover fail condition in Sciserver_TisciMsgResponse function */
    status = Sciserver_processtask(&utdTisciMsgResponseFail);
    if (status == CSL_EFAIL)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf ("Sciserver_processtask: Negative Arg Test Passed.\n");
    }
    else
    {
       sciserverFuncTestStatus += CSL_EFAIL;
       SciApp_printf ("Sciserver_processtask: Negative Arg Test Failed.\n");
    }

    uint32_t user_hi_msg_buffer_rmirq[SCISERVER_HW_QUEUE_SIZE] =
    {
        TISCI_MSG_RM_IRQ_SET
    };
    uint32_t *const user_hi_msg_buffer_list_rmirq[SCISERVER_SECPROXY_INSTANCE_COUNT] = {
        user_hi_msg_buffer_rmirq,
        user_hi_main_msg_buffer
    };
    Sciserver_taskData utdRmMessage =
    {
        .hw_msg_buffer_list = user_hi_msg_buffer_list_rmirq
    };
    utdRmMessage.state->state              = SCISERVER_TASK_PROCESSING_USER_MSG;
    utdRmMessage.state->current_buffer_idx = 0U;
    utdRmMessage.user_msg_data[0]->host    = TISCI_HOST_ID_DMSC2DM;
    /* Passing user_msg_data host as TISCI_HOST_ID_DMSC2DM to cover rm message pass conditions in Sciserver_UserProcessMsg function */
    status = Sciserver_processtask(&utdRmMessage);
    if (status == CSL_PASS)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf ("Sciserver_processtask: Sciserver_UserProcessMsg Test Passed.\n");
    }
    else
    {
       sciserverFuncTestStatus += CSL_EFAIL;
       SciApp_printf ("Sciserver_processtask: Sciserver_UserProcessMsg Test Failed.\n");
    }

    for(num = 0; num < 12; num++)
    {
        uint32_t user_hi_msg_buffer_message_types[SCISERVER_HW_QUEUE_SIZE] =
        {
            messageType[num]
        };

        uint32_t *const user_hi_msg_buffer_list_message_types[SCISERVER_SECPROXY_INSTANCE_COUNT] = {
            user_hi_msg_buffer_message_types,
            user_hi_main_msg_buffer
        };
        Sciserver_taskData utdUserProcessMsg =
        {
            .hw_msg_buffer_list = user_hi_msg_buffer_list_message_types
        };
        utdUserProcessMsg.state->state               = SCISERVER_TASK_PROCESSING_USER_MSG;
        utdUserProcessMsg.state->current_buffer_idx  = 0U;
        utdUserProcessMsg.user_msg_data[0]->host     = TISCI_HOST_ID_DMSC2DM;
        /* Passing different message types to Sciserver_UserProcessMsg function to process different message types */
        status = Sciserver_processtask(&utdUserProcessMsg);
        if (status == CSL_PASS)
        {
            sciserverFuncTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_processtask: Sciserver_UserProcessMsg Test Passed.\n");
        }
        else
        {
            sciserverFuncTestStatus += CSL_EFAIL;
            SciApp_printf ("Sciserver_processtask: Sciserver_UserProcessMsg Test Failed.\n");
        }
    }

    uint32_t user_hi_msg_buffer_rm_message_type[SCISERVER_HW_QUEUE_SIZE] =
    {
        messageType[12]
    };

    uint32_t *const user_hi_msg_buffer_list_rm_message_type[SCISERVER_SECPROXY_INSTANCE_COUNT] = {
        user_hi_msg_buffer_rm_message_type,
        user_hi_main_msg_buffer
    };
    Sciserver_taskData utdUserProcessRmMsg =
    {
        .hw_msg_buffer_list = user_hi_msg_buffer_list_rm_message_type
    };
    utdUserProcessRmMsg.state->state               = SCISERVER_TASK_PROCESSING_USER_MSG;
    utdUserProcessRmMsg.state->current_buffer_idx  = 0U;
    utdUserProcessRmMsg.user_msg_data[0]->host     = TISCI_HOST_ID_DMSC2DM;
    /* Passing message type as TISCI_MSG_BOARD_CONFIG_RM to Sciserver_UserProcessMsg function to process rm message type */
    status = Sciserver_processtask(&utdUserProcessRmMsg);
    if (status == CSL_EFAIL)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf ("Sciserver_processtask: Sciserver_UserProcessMsg Test Passed.\n");
    }
    else
    {
        sciserverFuncTestStatus += CSL_EFAIL;
        SciApp_printf ("Sciserver_processtask: Sciserver_UserProcessMsg Test Failed.\n");
    }

    status = Sciserver_getCtrlState();
    if (status != CSL_EFAIL)
    {
        sciserverFuncTestStatus += CSL_PASS;
        SciApp_printf ("Sciserver_getCtrlState Test Passed.\n");
    }
    else
    {
        sciserverFuncTestStatus += CSL_EFAIL;
        SciApp_printf ("Sciserver_getCtrlState Test Failed.\n");
    }

    return sciserverFuncTestStatus;
}

static int32_t SciserverApp_secProxyTransferTest(void)
{
  int32_t   status               = CSL_PASS;
  int32_t   sciserverInitStatus  = CSL_PASS;
  int32_t   secProxyTestStatus   = CSL_PASS;
  uint16_t  txConfigId           = 0U;
  uint32_t  *txBuff              = NULL;
  uint32_t  txNumWords           = 0x0FU;
  uint32_t  offset               = 0U;
  uint16_t  rxConfigId           = 0x55U;
  uint32_t  rxNumWords           = 0x0FU;
  uint16_t  validHostId          = TISCI_HOST_ID_MCU_0_R5_0;
  uint16_t  invalidHostId        = 1000;
  uint16_t  invalidTxConfigId    = 0xFF;
  uint16_t  invalidRxConfigId    = 0xFF;
  uint32_t  invalidSenderHostId  = 0;
  Sciserver_CfgPrms_t prms;
  
   status = Sciserver_init(&prms);
   sciserverInitStatus = status;

   if(status == CSL_PASS)
   {
        SciApp_printf ("Sciserver_init PASSED.\n");
        status = Sciserver_SproxyMsgGetSenderHostId(0U, NULL);
        if (status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyMsgGetSenderHostId: Negative Arg Test-1 Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyMsgGetSenderHostId: Negative Arg Test-1 Failed.\n");
        }

        status = Sciserver_SproxyMsgGetSenderHostId(invalidRxConfigId, &invalidSenderHostId);
        if(status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyMsgGetSenderHostId: Negative Arg Test-2 Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyMsgGetSenderHostId: Negative Arg Test-2 Failed.\n");
        }

        status = Sciserver_SproxyMsgSendAtOffset(txConfigId, txBuff, txNumWords, offset);
        if (status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyMsgSendAtOffset: txNumWords Negative Arg Test-1 Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyMsgSendAtOffset: txNumWords Negative Arg Test-1 Failed.\n");
        }

        txNumWords = 0U;
        status = Sciserver_SproxyMsgSendAtOffset(txConfigId, txBuff, txNumWords, offset);
        if (status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyMsgSendAtOffset: txNumWords=0 Negative Arg Test-2 Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyMsgSendAtOffset: txNumWords=0 Negative Arg Test-2 Failed.\n");
        }

        txConfigId = 0xB2U;
        status = Sciserver_SproxyCanSend(txConfigId);
        if (status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyTxIsReady: Negative Arg Test Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyTxIsReady: Negative Arg Test Failed.\n");
        }

        status = Sciserver_SproxyMsgIsPending(rxConfigId);
        if (status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyRxIsReady: Negative Arg Test Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyRxIsReady: Negative Arg Test Failed.\n");
        }

        status = Sciserver_SproxyMsgReadAtOffset(rxConfigId, NULL, rxNumWords, offset);
        if (status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyMsgReadAtOffset: Negative Arg Test-1 Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyMsgReadAtOffset: Negative Arg Test-1 Failed.\n");
        }

        status = Sciserver_SproxyMsgReadAtOffset(rxConfigId, NULL, 0, offset);
        if(status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyMsgReadAtOffset: Negative Arg Test-2 Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyMsgReadAtOffset: Negative Arg Test-2 Failed.\n");
        }

        status = Sciserver_SproxyMsgValidateHostId(validHostId);
        if(status == CSL_PASS)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyMsgValidateHostId: Positive Arg Test Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyMsgValidateHostId: Positive Arg Test Failed.\n");
        }

        status = Sciserver_SproxyMsgValidateHostId(invalidHostId);
        if(status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyMsgValidateHostId: Negative Arg Test Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyMsgValidateHostId: Negative Arg Test Failed.\n");
        }

        status = Sciserver_SproxyMsgFinish(invalidTxConfigId);
        if(status == CSL_EFAIL)
        {
            secProxyTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_SproxyMsgFinish: Negative Arg Test Passed.\n");
        }
        else
        {
           secProxyTestStatus += CSL_EFAIL;
           SciApp_printf ("Sciserver_SproxyMsgFinish: Negative Arg Test Failed.\n");
        }
  }
  else
  {
      secProxyTestStatus += CSL_EFAIL;
      SciApp_printf ("Sciserver_init FAILED.\n");
  }

  if(sciserverInitStatus == CSL_PASS)
  {
      status = Sciserver_deinit();
      if(status == CSL_PASS)
      {
          secProxyTestStatus += CSL_PASS;
          SciApp_printf ("Sciserver_deinit PASSED.\n");
      }
      else
      {
          secProxyTestStatus += CSL_EFAIL;
          SciApp_printf ("Sciserver_deinit FAILED.\n");
      }
  }

  return secProxyTestStatus;
}

static int32_t SciserverApp_secproxyRoutingDescriptionTest(void)
{
    int32_t   status                = CSL_PASS;
    int32_t   sciserverInitStatus   = CSL_PASS;
    int32_t   routingDesTestStatus  = CSL_PASS;
    uint16_t  hostId                = 0xFFU;
    uint16_t  txConfigId            = 0U;
    Sciserver_CfgPrms_t prms;
  
     status = Sciserver_init(&prms);
     sciserverInitStatus = status;

     if(status == CSL_PASS)
     {
          SciApp_printf ("Sciserver_init PASSED.\n");
          status = Sciserver_SproxyGetResponseConfId(hostId, &txConfigId);
          if (status == CSL_EFAIL)
          {
              routingDesTestStatus += CSL_PASS;
              SciApp_printf ("Sciserver_SproxyConfLookup: Negative Arg Test Passed.\n");
          }
          else
          {
             routingDesTestStatus += CSL_EFAIL;
             SciApp_printf ("Sciserver_SproxyConfLookup: Negative Arg Test Failed.\n");
          }

          hostId = 0xFEU;
          status = Sciserver_SproxyGetNotifyConfId(hostId, &txConfigId);
          if (status == CSL_EFAIL)
          {
              routingDesTestStatus += CSL_PASS;
              SciApp_printf ("Sciserver_SproxyConfLookup: Negative Arg Test Passed.\n");
          }
          else
          {
             routingDesTestStatus += CSL_EFAIL;
             SciApp_printf ("Sciserver_SproxyConfLookup: Negative Arg Test Failed.\n");
          }

          hostId = 3U;
          status = Sciserver_SproxyGetNotifyConfId(hostId, &txConfigId);
          if (status == CSL_PASS)
          {
              routingDesTestStatus += CSL_PASS;
              SciApp_printf ("Sciserver_SproxyGetNotifyConfId: Test Passed.\n");
          }
          else
          {
             routingDesTestStatus += CSL_EFAIL;
             SciApp_printf ("Sciserver_SproxyGetNotifyConfId: Test Failed.\n");
          }
    }
    else
    {
        routingDesTestStatus += CSL_EFAIL;
        SciApp_printf ("Sciserver_init FAILED.\n");
    }

    if(sciserverInitStatus == CSL_PASS)
    {
        status = Sciserver_deinit();
        if(status == CSL_PASS)
        {
            routingDesTestStatus += CSL_PASS;
            SciApp_printf ("Sciserver_deinit PASSED.\n");
        }
        else
        {
            routingDesTestStatus += CSL_EFAIL;
            SciApp_printf ("Sciserver_deinit FAILED.\n");
        }
    }

  return routingDesTestStatus;
}

static int32_t SciserverApp_rtosTest(void)
{
    int32_t                   status           = CSL_PASS;
    int32_t                   rtosTestStatus   = CSL_PASS;
    uint32_t                  ptr              = 53U;
    uint32_t                  index            = 0U;
    uint32_t                  maxindex         = index;
    extern   HwiP_Handle      gSciserverHwiHandles[SCISERVER_HWI_NUM];
    gSciserverHwiHandles[0]                    = (void *)ptr;
    Sciserver_TirtosCfgPrms_t pAppPrms;
    SemaphoreP_Params         semParams;
    SemaphoreP_Handle         semhandle[MAX_SEMAPHORE];

    SemaphoreP_Params_init(&semParams);
    
    semParams.mode = SemaphoreP_Mode_COUNTING;
    /* Creating the maximium number of semaphores */
    for(index = 0U; index < MAX_SEMAPHORE; index++)
    {
        semhandle[index] = SemaphoreP_create(0, &semParams);
        if(NULL_PTR == semhandle[index])
        {
            SciApp_printf ("Maximum semaphores created breaking the loop\n"); 
            break;
        }
    }
    maxindex = index;

    /* Sciserver_tirtosInitSemaphores function should fail after creating maximum number of semaphores */
    status = Sciserver_tirtosInit(&pAppPrms);
    if (status != CSL_PASS)
    {
        rtosTestStatus += CSL_PASS;
        SciApp_printf ("Sciserver_tirtosInit: Negative Arg Test Passed.\n");
    }
    else
    {
        rtosTestStatus += CSL_EFAIL;
        SciApp_printf ("Sciserver_tirtosInit: Negative Arg Test Failed.\n");
    }

    for(index = 0U ; index < maxindex; index++)
    {
        if(CSL_PASS != SemaphoreP_delete(semhandle[index]))
        {
            rtosTestStatus = CSL_EFAIL;
        }
    }
    Sciserver_tirtosDeinit();

    /* Passing NULL parameter to tirtos init function */
    status = Sciserver_tirtosInit(NULL);
    if (status != CSL_PASS)
    {
        rtosTestStatus += CSL_PASS;
        SciApp_printf ("Sciserver_tirtosInit: Negative Arg Test Passed.\n");
    }
    else
    {
        rtosTestStatus += CSL_EFAIL;
        SciApp_printf ("Sciserver_tirtosInit: Negative Arg Test Failed.\n");
    }

    return rtosTestStatus;
}
