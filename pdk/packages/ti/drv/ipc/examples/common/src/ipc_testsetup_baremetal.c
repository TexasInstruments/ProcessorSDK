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
 *  \file ipc_testsetup_baremetal.c
 *
 *  \brief IPC baremetal example code
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <ti/drv/ipc/examples/common/src/ipc_setup.h>

#include <ti/osal/HwiP.h>
#include <ti/osal/osal.h>
/* SCI Client */
#include <ti/drv/sciclient/sciclient.h>

#include <ti/drv/ipc/ipc.h>
#ifndef BUILD_MPU1_0
#if defined(SOC_AM65XX)
#include <ti/drv/ipc/examples/common/src/ipc_am65xx_rsctable.h>
#else
#include <ti/drv/ipc/examples/common/src/ipc_rsctable.h>
#endif
#endif

/* Size of message */
#define MSGSIZE  256U
/* Service name to be registered for the end point */
#define SERVICE_PING   "ti.ipc4.ping-pong"
/* End point number to be used for IPC communication */
#define ENDPT_PING     13U
/* Service name to be registered for chrdev end point */
#define SERVICE_CHRDEV "rpmsg_chrdev"
/* End point number to be used for chrdev end point */
#define ENDPT_CHRDEV   14U
#if defined(SIM_BUILD)
#define NUMMSGS  1000
#else
#define NUMMSGS  10000 /* number of message sent by the sender function */
#endif

typedef struct Ipc_TestParams_s
{
    uint32_t endPt;
    char     name[32];
} Ipc_TestParams;

Ipc_TestParams service_ping = { ENDPT_PING, SERVICE_PING };
Ipc_TestParams service_chrdev = { ENDPT_CHRDEV, SERVICE_CHRDEV };

extern uint8_t  *pCntrlBuf;
//extern uint8_t  *pTaskBuf;
extern uint8_t  *pSendTaskBuf;
extern uint8_t  *pRecvTaskBuf;
extern uint8_t  *pSysVqBuf;

extern uint32_t  selfProcId;
extern uint32_t *pRemoteProcArray;
extern uint32_t  gNumRemoteProc;

extern RPMessage_Handle *pHandleArray;
extern uint32_t         *pEndptArray;
extern uint32_t         *pCntPing;
extern uint32_t         *pCntPong;

uint32_t gRecvTaskBufIdx = 0;

uint32_t rpmsgDataSize = RPMSG_DATA_SIZE;

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
void rpmsg_startSender(uint16_t dstProc, uint32_t numProc, RPMessage_Handle *handle, uint32_t *myEndPt);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
volatile uint32_t gMessagesReceived = 0;

#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)
static uint32_t   RecvEndPt = 0;
#endif

//#define DEBUG_PRINT

/*
 * This "Task" waits for Linux vdev ready, and late create the vrings
 *
 */
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)
void rpmsg_vdevMonitorFxn(void)
{
    int32_t status;

    /* Wait for Linux VDev ready... */
    while(!Ipc_isRemoteReady(IPC_MPU1_0))
    {
//       Task_sleep(10);
    }

    /* Create the VRing now ... */
    status = Ipc_lateVirtioCreate(IPC_MPU1_0);
    if(status != IPC_SOK)
    {
        App_printf("%s: Ipc_lateVirtioCreate failed\n", __func__);
        return;
    }

    status = RPMessage_lateInit(IPC_MPU1_0);
    if(status != IPC_SOK)
    {
        App_printf("%s: RPMessage_lateInit failed\n", __func__);
        return;
    }

    status = RPMessage_announce(IPC_MPU1_0, RecvEndPt, SERVICE_PING);
    if(status != IPC_SOK)
    {
        App_printf("rpmsg_vdevMonitorFxn: RPMessage_announce() failed\n");
    }
}
#endif /* !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)*/


bool g_exitRespTsk = 0;

/*
 * This function can be used to stop the
 * responder task.
 */
void rpmsg_exit_responseTask()
{
    g_exitRespTsk = 1;
}

/*
 * This function waits for a "ping" message from any processor
 * then replies with a "pong" message.
 * This function also processes the "pong" messages from other
 * processors.
 */
void rpmsg_responderFxn(uintptr_t arg0, uintptr_t arg1)
{
    RPMessage_Handle handle;
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS)
    RPMessage_Handle handle_chrdev;
#endif
    RPMessage_Handle *responseHandle = NULL;
    RPMessage_Params params;
    uint32_t         myEndPt = 0;
    uint32_t         remoteEndPt;
    uint32_t         remoteProcId;
    uint16_t         len;
    int32_t          n;
    int32_t          status = 0;
    void             *buf;
    uint32_t         lastNumMessagesReceived = 0;
    uint32_t         emptyReceiveCalls = 0;
    uint8_t          responseSent = 0;
    uint8_t          i = 0;
    uint32_t         numProc = (uint32_t)arg0;

    uint32_t         bufSize = rpmsgDataSize;
    char             str[MSGSIZE];

    buf = &pRecvTaskBuf[gRecvTaskBufIdx++ * rpmsgDataSize];
    if(buf == NULL)
    {
        App_printf("RecvTask: buffer allocation failed\n");
        return;
    }

    RPMessageParams_init(&params);

    params.requestedEndpt = service_ping.endPt;

    params.buf = buf;
    params.bufSize = bufSize;

    handle = RPMessage_create(&params, &myEndPt);
    if(!handle)
    {
        App_printf("RecvTask: Failed to create endpoint\n");
        return;
    }

#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)
    if (requestedEpt == ENDPT_PING)
    {
        RecvEndPt = myEndPt;
    }
#endif

    status = RPMessage_announce(RPMESSAGE_ALL, myEndPt, service_ping.name);
    if(status != IPC_SOK)
    {
        App_printf("RecvTask: RPMessage_announce() for %s failed\n", service_ping.name);
        return;
    }

#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS)
    params.requestedEndpt = service_chrdev.endPt;
    buf = &pRecvTaskBuf[gRecvTaskBufIdx++ * rpmsgDataSize];
    params.buf = buf;
    params.bufSize = bufSize;

    handle_chrdev = RPMessage_create(&params, &myEndPt);
    if (!handle_chrdev)
    {
        App_printf("RecvTask: Failed to create chrdev endpoint\n");
        return;
    }
    status = RPMessage_announce(RPMESSAGE_ALL, myEndPt, service_chrdev.name);
    if (status != IPC_SOK)
    {
        App_printf("RecvTask: RPMessage_announce() for %s failed\n", service_chrdev.name);
    }
#endif

    /* initialize the sender handles */
    for (i = 0; i < numProc; i++)
    {
        pHandleArray[i] = NULL;
        pCntPing[i] = 0;
        pCntPong[i] = 0;
    }
    
    #if defined LDRA_DYN_COVERAGE_EXIT
    UART_printf("\n LDRA ENTRY... \n");
    upload_execution_history();
    UART_printf("\n LDRA EXIT... \n");
    #endif
    
    while(!g_exitRespTsk)
    {
        /* Wait for messages to show up */
        while(gMessagesReceived == lastNumMessagesReceived);

	/* Check for any new announcements first, and start the sender function */
        for (i = 0; i < numProc; i++)
        {
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS)
            /* Linux does not have a responder func running */
            if(pRemoteProcArray[i] == IPC_MPU1_0)
                continue;
#endif

            if (pHandleArray[i] == NULL)
            {
                status = RPMessage_getRemoteEndPt(pRemoteProcArray[i], SERVICE_PING, &remoteProcId,
                                                  &remoteEndPt, 0);
                if (status == IPC_SOK)
		{
                    rpmsg_startSender(pRemoteProcArray[i], i, &pHandleArray[i], &pEndptArray[i]);
                    pCntPing[i]++;
		}
            }
        }

        /* NOTE: The following function may need to be replaced by a blocking
          function RPMessage_recv in later implementations */
        status = RPMessage_recvNb(handle,
                                  (Ptr)str, &len, &remoteEndPt,
                                  &remoteProcId);
        responseHandle = &handle;
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS)
        if (status != IPC_SOK)
        {
            /* Try the chrdev handle */
            status = RPMessage_recvNb(handle_chrdev,
                                      (Ptr)str, &len, &remoteEndPt,
                                      &remoteProcId);
            responseHandle = &handle_chrdev;
        }
#endif

        if (status != IPC_SOK)
        {
            /* Try the sender function handles */
            for (i = 0; i < gNumRemoteProc; i++)
            {
                if (pHandleArray[i] == NULL)
                {
                    continue;
                }

                status = RPMessage_recvNb(pHandleArray[i],
                                          (Ptr)str, &len, &remoteEndPt,
                                          &remoteProcId);
                if (status == IPC_SOK)
                {
                    break;
                }
            }
        }

        if(status != IPC_SOK)
        {
#ifdef DEBUG_PRINT
            App_printf("RecvTask: failed with code %d\n", status);
#endif
            emptyReceiveCalls++;
            lastNumMessagesReceived++;
        }
        else
        {
            lastNumMessagesReceived++;

            /* NULL terminated string */
            if (len >= MSGSIZE)
            {
                str[MSGSIZE-1] = '\0';
            }
            else
            {
                str[len] = '\0';
            }
#ifdef DEBUG_PRINT
            App_printf("RecvTask: Revcvd msg \"%s\" len %d from %s\n",
                    str, len, Ipc_mpGetName(remoteProcId));
#endif
        }
        if(status == IPC_SOK)
        {
            status = sscanf(str, "ping %d", &n);
            if(status == 1)
            {
                memset(str, 0, MSGSIZE);
                len = snprintf(str, 255, "pong %d", n);
                if(len > 255)
                {
                    App_printf("RecvTask: snprintf failed, len %d\n", len);
                    len = 255;
                }
                str[len++] = '\0';
            }
            else
            {
                status = sscanf(str, "pong %d", &n);
                if (status == 1)
                {
                    pCntPong[i]++;

                    /* This is a resopnse from a peer */
                    if (pCntPing[i] < NUMMSGS)
                    {
#ifdef DEBUG_PRINT
                        App_printf("SendTask%d: Received \"%s\" len %d from %s endPt %d \n",
                                      remoteProcId, str, len, Ipc_mpGetName(remoteProcId),
                                      remoteEndPt);
#endif
                        /* Send data to remote endPt: */
                        memset(str, 0, 256);
                        len = snprintf(str, 255, "ping %d", pCntPing[i]++);
                        if (len > 255)
                        {
                            App_printf("SendTask%d: snprintf failed, len %d\n", remoteProcId, len);
                            len = 255;
                        }
                        str[len++] = '\0';

#ifdef DEBUG_PRINT
                        App_printf("SendTask%d: Sending \"%s\" from %s to %s...\n", remoteProcId,
                                      str, Ipc_mpGetSelfName(),
                                      Ipc_mpGetName(remoteProcId));
#endif

                        status = RPMessage_send(pHandleArray[i], remoteProcId, ENDPT_PING, pEndptArray[i], (Ptr)str, len);
                        if (status != IPC_SOK)
                        {
                            App_printf("SendTask%d: RPMessage_send Failed Msg-> \"%s\" from %s to %s...\n",
                                          remoteProcId,
                                          str, Ipc_mpGetSelfName(),
                                          Ipc_mpGetName(remoteProcId));
                        }
                    }
                    else if (pCntPing[i] == NUMMSGS)
                    {
                        App_printf("%s <--> %s, Ping- %d, pong - %d completed\n",
                                      Ipc_mpGetSelfName(),
                                      Ipc_mpGetName(remoteProcId),
                                      pCntPing[i], pCntPong[i]);
                        pCntPing[i]++;
                    }
                    responseSent = 1;
                }
                else
                {
                    /* If this is not ping/pong message, just print the message */
                    App_printf("%s <--> %s : %s recvd : %d:%d:%d\n",
                                  Ipc_mpGetSelfName(),
                                  Ipc_mpGetName(remoteProcId),
                                  str,
                                  gMessagesReceived,
                                  lastNumMessagesReceived,
                                  emptyReceiveCalls);
                }
            }
            if (responseSent == 0)
            {
#ifdef DEBUG_PRINT
                App_printf("RecvTask: Sending msg \"%s\" len %d from %s to %s\n",
                              str, len, Ipc_mpGetSelfName(),
                              Ipc_mpGetName(remoteProcId));
#endif
                status = RPMessage_send(*responseHandle, remoteProcId, remoteEndPt, myEndPt, str, len);
                if (status != IPC_SOK)
                {
                    App_printf("RecvTask: Sending msg \"%s\" len %d from %s to %s failed!!!\n",
                                  str, len, Ipc_mpGetSelfName(),
                                  Ipc_mpGetName(remoteProcId));
                }
            }
            responseSent = 0;
        }
    }

    App_printf("%s responder task exiting ...\n",
                    Ipc_mpGetSelfName());
}

/*
 * This function sends a message to kick start the ping/pong messaging that will
 * repeat for predetermined number of messages. The response is handled in the responderFxn
 */
void rpmsg_startSender(uint16_t dstProc, uint32_t numProc, RPMessage_Handle *handle, uint32_t *myEndPt)
{
    RPMessage_Params    params;
    uint16_t            len;
    int32_t             status = 0;
    char                buf[256];
    uint8_t            *buf1;

    buf1 = &pSendTaskBuf[rpmsgDataSize * numProc];

    /* Create the endpoint for receiving. */
    RPMessageParams_init(&params);
    params.numBufs = 2;
    params.buf = buf1;
    params.bufSize = rpmsgDataSize;
    *handle = RPMessage_create(&params, myEndPt);
    if(!(*handle))
    {
        App_printf("SendTask %d: Failed to create message endpoint\n",
                dstProc);
        return;
    }

#if 0 /* RPMessage_getRemoteEndPt isn't currently supported in baremetal ipc library */
    status = RPMessage_getRemoteEndPt(dstProc, SERVICE_PING, &remoteProcId,
            &remoteEndPt, TEST_TIMEOUT_FOREVER);
    if(dstProc != remoteProcId)
    {
        App_printf("SendTask%d: RPMessage_getRemoteEndPt() malfunctioned, status %d\n",
                dstProc, status);
        return;
    }
#endif
    /* Let's kick start the ping/pong by sending the first message.
     * We'll have to wait for response and reply together with the responderFxn.
     */

    /* Send data to remote endPt: */
    memset(buf, 0, 256);
    len = snprintf(buf, 255, "ping %d", 1);
    if (len > 255)
    {
        App_printf("SendTask%d: snprintf failed, len %d\n", dstProc, len);
        len = 255;
    }
    buf[len++] = '\0';

#ifdef DEBUG_PRINT
    App_printf("SendTask%d: Sending \"%s\" from %s to %s...\n", dstProc,
                  buf, Ipc_mpGetSelfName(),
                  Ipc_mpGetName(dstProc));
#endif

    status = RPMessage_send(*handle, dstProc, ENDPT_PING, *myEndPt, (Ptr)buf, len);
    if (status != IPC_SOK)
    {
        App_printf("SendTask%d: RPMessage_send Failed Msg-> \"%s\" from %s to %s...\n",
            dstProc,
            buf, Ipc_mpGetSelfName(),
            Ipc_mpGetName(dstProc));
    }
}

/*
 * This function is the callback function the ipc lld library calls when a
 * message is received.
 */
static void IpcTestBaremetalNewMsgCb(uint32_t srcEndPt, uint32_t procId)
{
    /* Add code here to take action on any incoming messages */
    gMessagesReceived++;
    return;
}

static void IpcTestPrint(const char *str)
{
    App_printf("%s", str);

    return;
}
uint32_t Ipc_exampleVirtToPhyFxn(const void *virtAddr)
{
    return ((uint32_t) virtAddr);
}

void *Ipc_examplePhyToVirtFxn(uint32_t phyAddr)
{
    uint32_t temp = phyAddr;

#if defined (BUILD_M4F_0)
    /*
     * For M4, need the translation for the mailbox access.
     *
     * The address for mailbox access is defined in the default
     * RAT config which can be found in csl/arch/m4/src/startup/startup.c
     */
    temp = phyAddr + 0x40000000;
#endif

    return ((void *) temp);
}

/*
 * This is the main test function which initializes and runs the Sender and
 * responder functions.
 * NOTE: Sender function is not actually active in this example.
 *       Only the receiver function does a echo back of incoming messages
 */
/* ==========================================*/
int32_t Ipc_echo_test(void)
{
    uint32_t          numProc = gNumRemoteProc;
    Ipc_InitPrms      initPrms;
    Ipc_VirtIoParams  vqParam;
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && !defined(A72_LINUX_OS_IPC_ATTACH)
    uint32_t          t;
#endif

    /* Step1 : Initialize the multiproc */
    if (IPC_SOK == Ipc_mpSetConfig(selfProcId, numProc, pRemoteProcArray))
    {
        App_printf("IPC_echo_test (core : %s) .....\r\n", Ipc_mpGetSelfName());

        /* Initialize params with defaults */
        IpcInitPrms_init(0U, &initPrms);

        initPrms.newMsgFxn = &IpcTestBaremetalNewMsgCb;
        initPrms.virtToPhyFxn = &Ipc_exampleVirtToPhyFxn;
        initPrms.phyToVirtFxn = &Ipc_examplePhyToVirtFxn;
        initPrms.printFxn = &IpcTestPrint;

        if (IPC_SOK != Ipc_init(&initPrms))
        {
            return -1;
        }
    }
#ifdef DEBUG_PRINT
    App_printf("Required Local memory for Virtio_Object = %d\r\n",
                  numProc * Ipc_getVqObjMemoryRequiredPerCore());
#endif
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS)
    /* If A72 remote core is running Linux OS, then
     * load resource table
     */
    Ipc_loadResourceTable((void*)&ti_ipc_remoteproc_ResourceTable);

#if !defined(A72_LINUX_OS_IPC_ATTACH)
    /* Wait for Linux VDev ready... */
    for(t = 0; t < numProc; t++)
    {
        while(!Ipc_isRemoteReady(pRemoteProcArray[t]))
        {
            // Task_sleep(100);
        }
    }
    App_printf("Linux VDEV ready now .....\n");
#endif
#endif

    /* Step2 : Initialize Virtio */
    vqParam.vqObjBaseAddr = (void*)pSysVqBuf;
    vqParam.vqBufSize     = numProc * Ipc_getVqObjMemoryRequiredPerCore();
    vqParam.vringBaseAddr = (void*)VRING_BASE_ADDRESS;
    vqParam.vringBufSize  = IPC_VRING_BUFFER_SIZE;
    vqParam.timeoutCnt    = 100;  /* Wait for counts */
    Ipc_initVirtIO(&vqParam);

    /* Step 3: Initialize RPMessage */
    RPMessage_Params cntrlParam;

#ifdef DEBUG_PRINT
    App_printf("Required Local memory for RPMessage Object = %d\n",
                  RPMessage_getObjMemRequired());
#endif

    /* Initialize the param */
    RPMessageParams_init(&cntrlParam);

    /* Set memory for HeapMemory for control task */
    cntrlParam.buf         = pCntrlBuf;
    cntrlParam.bufSize     = rpmsgDataSize;
    cntrlParam.stackBuffer = NULL;
    cntrlParam.stackSize   = 0U;
    RPMessage_init(&cntrlParam);

#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)
    rpmsg_vdevMonitorFxn();
#endif

    /* run responder function to receive and reply messages back */
    rpmsg_responderFxn(numProc,0);

    return 1;
}
