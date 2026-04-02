/*
 *  Copyright (c) Texas Instruments Incorporated 2020
 *  All rights reserved.
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
 */

/**
 *  \file ipc_neg_testsetup.c
 *
 *  \brief IPC  example code
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <ti/drv/ipc/examples/common/src/ipc_setup.h>
#include <ti/drv/ipc/ipc.h>
#include <ti/drv/ipc/examples/rtos/ipc_negative_test/ipc_neg_setup.h>

#include <ti/osal/osal.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#define MSGSIZE  256U
#define SERVICE  "ti.ipc4.ping-pong"
#define FAKE_SERVICE "negative-test"
#define ENDPT1   13U
#define ENDPT2   14U
#define NUMMSGS  10000 /* number of message sent per task */
//#define NUMMSGS  1000000   /* number of message sent per task */

extern uint8_t  *pCntrlBuf;
extern uint8_t  *pTaskBuf;
extern uint8_t  *pSendTaskBuf;
extern uint8_t  *pRecvTaskBuf;
extern uint8_t  *pTimeoutBuf;
extern uint8_t  *pSysVqBuf;

extern uint32_t  selfProcId;
extern uint32_t *pRemoteProcArray;
extern uint32_t  gNumRemoteProc;

uint32_t rpmsgNegDataSize = RPMSG_DATA_SIZE;

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define RP_MSG_TEST_BUF_SIZE     (512 + 2)
/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)
static uint32_t        RecvEndPt = 0;
#endif

static uint32_t gTotalTests = 0;
static uint32_t gTotalTestsPassed = 0;
static uint32_t gTotalTestsFailed = 0;

typedef enum Ipc_NegTestId_e {
    TEST_RPMSG_PKT_LARGE_THAN_VRING_BUF,
    TEST_VIRTIO_INIT_PARAM_NULL,
    TEST_RPMSG_INIT_PARAM_NULL,
    TEST_RPMSG_CREATE_NULL_ENDPNT,
    TEST_RPMSG_SND_NULL_DATA,
    TEST_RPMSG_SND_UNPAIR_CORE,
    TEST_RPMSG_RCV_HNDL_NULL,
    TEST_RPMSG_SND_HNDL_NULL,
    TEST_VIRTIO_PARAM_VQBUF_SIZE_ZERO,
    TEST_VIRTIO_PARAM_VRINGBUF_SIZE_ZERO,
    TEST_RPMSG_PARAM_BUF_NULL,
    TEST_RPMSG_PARAM_BUF_SIZE_ZERO,
    TEST_RPMSG_PARAM_STKBUF_NULL,
    TEST_RPMSG_PARAM_STK_SIZE_ZERO,
    TEST_RPMSG_RCV_TIMEOUT,
    TEST_RPMSG_REMOTE_END_TIMEOUT,
    TEST_END
} Ipc_NegTestId;

typedef struct Ipc_NegTestParams_s
{
    uint32_t  testId;
    char     *testName;
} Ipc_TestParams;

Ipc_TestParams negTestCases[] =
{
    { TEST_RPMSG_PKT_LARGE_THAN_VRING_BUF, "test_rpmsg_pkt_large_than_vring_buf" },
    { TEST_VIRTIO_INIT_PARAM_NULL, "test_virtio_init_param_null" },
    { TEST_RPMSG_INIT_PARAM_NULL, "test_rpmsg_init_param_null" },
    { TEST_RPMSG_CREATE_NULL_ENDPNT, "test_rpmsg_create_null_endpnt" },
    { TEST_RPMSG_SND_NULL_DATA, "test_rpmsg_snd_null_data" },
    { TEST_RPMSG_SND_UNPAIR_CORE, "test_rpmsg_snd_unpair_core" },
    { TEST_RPMSG_RCV_HNDL_NULL, "test_rpmsg_rcv_hndl_null" },
    { TEST_RPMSG_SND_HNDL_NULL, "test_rpmsg_snd_hndl_null" },
    { TEST_VIRTIO_PARAM_VQBUF_SIZE_ZERO, "test_virtio_param_vqbuf_size_zero" },
    { TEST_VIRTIO_PARAM_VRINGBUF_SIZE_ZERO, "test_virtio_param_vringbuf_size_zero" },
    { TEST_RPMSG_PARAM_BUF_NULL, "test_rpmsg_param_buf_null" },
    { TEST_RPMSG_PARAM_BUF_SIZE_ZERO, "test_rpmsg_param_buf_size_zero" },
    { TEST_RPMSG_PARAM_STKBUF_NULL, "test_rpmsg_param_stkbuf_null" },
    { TEST_RPMSG_PARAM_STK_SIZE_ZERO, "test_rpmsg_param_stk_size_zero" },
    { TEST_RPMSG_RCV_TIMEOUT, "test_rpmsg_rcv_timeout" },
    { TEST_RPMSG_REMOTE_END_TIMEOUT, "test_rpmsg_remote_end_timeout" },
    { TEST_END, NULL },
};

//#define DEBUG_PRINT

bool g_neg_exitRespTsk = 0;

void rpmsg_neg_exit_responseTask()
{
    g_neg_exitRespTsk = 1;
}

void Ipc_reportResult(uint32_t id, int32_t result)
{
    gTotalTests++;
    if (result == IPC_SOK)
    {
        App_printf("[%s] [%s] : [TEST_PASS]\n", Ipc_mpGetSelfName(), negTestCases[id].testName);
        UART_printf("[%s] [%s] : [TEST_PASS]\n", Ipc_mpGetSelfName(), negTestCases[id].testName);
        gTotalTestsPassed++;
    }
    else
    {
        App_printf("[%s] [%s] : [TEST_FAIL]\n", Ipc_mpGetSelfName(), negTestCases[id].testName);
        UART_printf("[%s] [%s] : [TEST_FAIL]\n", Ipc_mpGetSelfName(), negTestCases[id].testName);
        gTotalTestsFailed++;
    }
}

/*
 * This "Task" waits for a "ping" message from any processor
 * then replies with a "pong" message.
 */
int32_t rpmsg_neg_responderFxn(uint32_t testId)
{
    RPMessage_Handle    handle;
    RPMessage_Handle    handleTimeout;
    RPMessage_Params    params;
    uint32_t        myEndPt = 0;
    uint32_t        myTimeoutEndPt = 0;
    uint32_t        remoteEndPt;
    uint32_t        remoteProcId;
    uint16_t        len;
    int32_t         status = 0;
    void            *buf;
    void            *buf2;

    uint32_t            bufSize = rpmsgNegDataSize;
    char                str[MSGSIZE];
    char                test_buf[RP_MSG_TEST_BUF_SIZE + 1];


    buf = pRecvTaskBuf;
    if (buf == NULL)
    {
        App_printf("RecvTask: buffer allocation failed\n");
        return IPC_EFAIL;
    }

    /* Testing TEST_RPMSG_REMOTE_END_TIMEOUT */
    {
        status = RPMessage_getRemoteEndPt(IPC_MCU1_0, FAKE_SERVICE, &remoteProcId,
                &remoteEndPt, 1000);
        if (status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_REMOTE_END_TIMEOUT, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_REMOTE_END_TIMEOUT, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    RPMessageParams_init(&params);
    params.requestedEndpt = ENDPT1;
    params.buf = buf;
    params.bufSize = bufSize;

    /* Testing TEST_RPMSG_CREATE_NULL_ENDPNT */
    {
        App_printf("RPMessage_create NULL Endpt\n");
        handle = RPMessage_create(&params, NULL);
        if (!handle)
        {
            Ipc_reportResult(TEST_RPMSG_CREATE_NULL_ENDPNT, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_CREATE_NULL_ENDPNT, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    handle = RPMessage_create(&params, &myEndPt);
    if (!handle)
    {
        App_printf("RecvTask: Failed to create endpoint\n");
        return IPC_EFAIL;
    }
    buf2 = pTimeoutBuf;
    params.requestedEndpt = ENDPT2;
    params.buf = buf2;
    params.bufSize = bufSize;
    handleTimeout = RPMessage_create(&params, &myTimeoutEndPt);
    if (!handleTimeout)
    {
        App_printf("RecvTask: Failed to create timeout endpoint\n");
        RPMessage_delete(&handle);
        return IPC_EFAIL;
    }

#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)
    RecvEndPt = myEndPt;
#endif

#if defined (BUILD_MPU1_0)
    remoteProcId = IPC_MCU1_0;
#else
    remoteProcId = IPC_MPU1_0;
#endif

    /* Testing TEST_RPMSG_PKT_LARGE_THAN_VRING_BUF */
    {
        /* Send data to remote endPt: */
        memset(test_buf, 0, RP_MSG_TEST_BUF_SIZE);

        len = snprintf(test_buf, RP_MSG_TEST_BUF_SIZE, "ping %d", 0);

        if(len > RP_MSG_TEST_BUF_SIZE)
        {
            App_printf("SendTask%d: snprintf failed, len %d\n", remoteProcId, len);
            len = RP_MSG_TEST_BUF_SIZE;
        }

        len = RP_MSG_TEST_BUF_SIZE;
        test_buf[len++] = '\0';

        status = RPMessage_send(handle, remoteProcId, ENDPT1, myEndPt, (Ptr)test_buf, len);
        if (status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_PKT_LARGE_THAN_VRING_BUF, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_PKT_LARGE_THAN_VRING_BUF, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    /* Testing TEST_RPMSG_SND_NULL_DATA */
    {
        status = RPMessage_send(handle, remoteProcId, ENDPT1, myEndPt, NULL, len);

        if (status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_SND_NULL_DATA, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_SND_NULL_DATA, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    /* Testing TEST_RPMSG_SND_UNPAIR_CORE */
    {
#ifdef BUILD_C66X_1
        remoteProcId = IPC_C66X_1; // todo: what should dstProc be??
#endif
        status = RPMessage_send(handle, remoteProcId, ENDPT1, myEndPt, (Ptr)buf, len);

        if (status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_SND_UNPAIR_CORE, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_SND_UNPAIR_CORE, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    /* Testing TEST_RPMSG_SND_HNDL_NULL */
    {
        status = RPMessage_send(NULL, remoteProcId, ENDPT1, myEndPt, (Ptr)buf, len);
        if (status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_SND_HNDL_NULL, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_SND_HNDL_NULL, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    /* Testing TEST_RPMSG_RCV_HNDL_NULL */
    {
        status = RPMessage_recv(NULL, (Ptr)str, &len, &remoteEndPt, &remoteProcId,
                    IPC_RPMESSAGE_TIMEOUT_FOREVER);
        if (status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_RCV_HNDL_NULL, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_RCV_HNDL_NULL, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    /* Testing TEST_RPMSG_RCV_TIMEOUT */
    {
        App_printf("[%s] calling RPMessage_recv with timeout 100...\n", Ipc_mpGetSelfName());
        status = RPMessage_recv(handleTimeout, (Ptr)str, &len, &remoteEndPt, &remoteProcId, 100);

        if (status != IPC_ETIMEOUT)
        {
            Ipc_reportResult(TEST_RPMSG_RCV_TIMEOUT, IPC_EFAIL);
            return IPC_EFAIL;
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_RCV_TIMEOUT, IPC_SOK);
        }
    }

    return IPC_SOK;
}

int32_t ipc_neg_test(uint32_t testId)
{
    uint32_t          numProc = gNumRemoteProc;
    Ipc_VirtIoParams  vqParam;
    uint32_t          index = 0;
    int32_t           status;

    /* Step1 : Initialize the multiproc */
    Ipc_mpSetConfig(selfProcId, numProc, pRemoteProcArray);

    App_printf("IPC_echo_test (core : %s) .....\r\n", Ipc_mpGetSelfName());

    Ipc_init(NULL);

    /* Step2 : Initialize Virtio */
    vqParam.vqObjBaseAddr = (void*)pSysVqBuf;
    vqParam.vqBufSize     = numProc * Ipc_getVqObjMemoryRequiredPerCore();
    vqParam.vringBaseAddr = (void*)VRING_BASE_ADDRESS;
    vqParam.vringBufSize  = IPC_VRING_BUFFER_SIZE;
    vqParam.timeoutCnt    = 100;  /* Wait for counts */

    /* Testing TEST_VIRTIO_INIT_PARAM_NULL */
    {
        status = Ipc_initVirtIO( NULL );
        if(status != IPC_SOK)
        {
            Ipc_reportResult(TEST_VIRTIO_INIT_PARAM_NULL, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_VIRTIO_INIT_PARAM_NULL, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    /* Testing TEST_VIRTIO_PARAM_VQBUF_SIZE_ZERO */
    {
        vqParam.vqBufSize     = 0;

        status = Ipc_initVirtIO(&vqParam);
        if(status != IPC_SOK)
        {
            Ipc_reportResult(TEST_VIRTIO_PARAM_VQBUF_SIZE_ZERO, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_VIRTIO_PARAM_VQBUF_SIZE_ZERO, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    /* Testing TEST_VIRTIO_PARAM_VRINGBUF_SIZE_ZERO */
    {
        vqParam.vqBufSize     = numProc * Ipc_getVqObjMemoryRequiredPerCore();
        vqParam.vringBufSize  = 0;

        status = Ipc_initVirtIO(&vqParam);
        if(status != IPC_SOK)
        {
            Ipc_reportResult(TEST_VIRTIO_PARAM_VRINGBUF_SIZE_ZERO, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_VIRTIO_PARAM_VRINGBUF_SIZE_ZERO, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }


    App_printf("Ipc_initVirtIO\n");
    vqParam.vringBufSize  = IPC_VRING_BUFFER_SIZE;
    status = Ipc_initVirtIO(&vqParam);
    if (status != IPC_SOK)
    {
        App_printf("[%s] Ipc_initVirtIO failed\n");
        return IPC_EFAIL;
    }

    /* Step 3: Initialize RPMessage */
    RPMessage_Params cntrlParam;

    /* Initialize the param */
    RPMessageParams_init(&cntrlParam);

    /* Set memory for HeapMemory for control task */
    cntrlParam.buf         = pCntrlBuf;
    cntrlParam.bufSize     = rpmsgNegDataSize;
    cntrlParam.stackBuffer = &pTaskBuf[index * IPC_TASK_STACKSIZE];
    cntrlParam.stackSize   = IPC_TASK_STACKSIZE;

    /* Testing TEST_RPMSG_INIT_PARAM_NULL */
    {
        status = RPMessage_init(NULL);
        if(status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_INIT_PARAM_NULL, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_INIT_PARAM_NULL, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    /* Testing TEST_RPMSG_PARAM_BUF_NULL */
    {
        cntrlParam.buf =  NULL;

        status = RPMessage_init(&cntrlParam);
        if(status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_PARAM_BUF_NULL, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_PARAM_BUF_NULL, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }

    /* Testing TEST_RPMSG_PARAM_BUF_SIZE_ZERO */
    {
        cntrlParam.buf = pCntrlBuf;
        cntrlParam.bufSize = 0;

        status = RPMessage_init(&cntrlParam);
        if(status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_PARAM_BUF_SIZE_ZERO, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_PARAM_BUF_SIZE_ZERO, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }
#ifndef IPC_EXCLUDE_CTRL_TASKS
    /* Testing TEST_RPMSG_PARAM_STKBUF_NULL */
    {
        cntrlParam.bufSize     = rpmsgNegDataSize;
        cntrlParam.stackBuffer = NULL;

        status = RPMessage_init(&cntrlParam);
        if(status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_PARAM_STKBUF_NULL, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_PARAM_STKBUF_NULL, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }
    /* Testing TEST_RPMSG_PARAM_STK_SIZE_ZERO */
    {
        cntrlParam.stackBuffer = &pTaskBuf[index * IPC_TASK_STACKSIZE];
        cntrlParam.stackSize = 0;

        status = RPMessage_init(&cntrlParam);
        if(status != IPC_SOK)
        {
            Ipc_reportResult(TEST_RPMSG_PARAM_STK_SIZE_ZERO, IPC_SOK);
        }
        else
        {
            Ipc_reportResult(TEST_RPMSG_PARAM_STK_SIZE_ZERO, IPC_EFAIL);
            return IPC_EFAIL;
        }
    }
#endif /* IPC_EXCLUDE_CTRL_TASKS */

    App_printf("RPMessage_init\n");
    cntrlParam.stackSize   = IPC_TASK_STACKSIZE;
    status = RPMessage_init(&cntrlParam);
    if (status != IPC_SOK)
    {
        App_printf("[%s] RPMessage_init failed\n", Ipc_mpGetSelfName());
        return IPC_EFAIL;
    }

    return rpmsg_neg_responderFxn(testId);
}

int32_t Ipc_echo_neg_test(void)
{
    uint32_t i = 0;

    ipc_neg_test(i);
    
    UART_printf("Tests finished\n");
    UART_printf("Total tests: %d Passed: %d Failed %d\n", gTotalTests, gTotalTestsPassed, gTotalTestsFailed);
    
    #if defined LDRA_DYN_COVERAGE_EXIT
        UART_printf("\n LDRA ENTRY... \n");
        upload_execution_history();
        UART_printf("\n LDRA EXIT... \n");
    #endif
    
    if (gTotalTests == gTotalTestsPassed)
    {
        UART_printf("All tests have passed.\n\n\n");
    }
    return 0;
}
