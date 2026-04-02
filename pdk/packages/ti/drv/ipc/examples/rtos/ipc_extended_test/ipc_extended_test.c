/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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
 *  \file ipc_extended_test.c
 *
 *  \brief IPC extended test application to cover IPC APIs for different
 *         parameters and sequences like invalid configurations, deinit, etc.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* Standard Includes */
#include <stdint.h>

/* PDK Generic Includes */
#include <ti/drv/uart/UART_stdio.h>

/* IPC driver includes; contains internal header files for this example */
#include <ti/drv/ipc/ipc.h>
#include <ti/drv/ipc/src/ipc_mailbox.h>
#include <ti/drv/ipc/src/ipc_virtioPrivate.h>
#include <ti/drv/ipc/src/ipc_utils.h>
#include <ti/drv/ipc/src/ipc_priv.h>
#include <ti/drv/ipc/src/mailbox/V0/mailbox.h>
#include <ti/drv/ipc/src/ipc_osal.h>

/* IPC example includes */
#include <ti/drv/ipc/examples/common/src/ipc_setup.h>
#include <ti/drv/ipc/examples/rtos/ipc_extended_test/ipc_extended_setup.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* IPC test macros for message and end point */
#define IPC_APP_MSGSIZE             256U
#define IPC_APP_ENDPT1              13U
#define IPC_APP_INVALID_ENDPT       5U
#define IPC_APP_CSL_CLEC_MAX_EVT_IN 2047U

/* Macros for invalid ids */
#define IPC_APP_INVALID_ID    50U
#define IPC_APP_INVALID_PROC  17U
#define IPC_APP_MP_INVALID_ID 0xFFFFFFFFU

/* Macros for ping */
#define IPC_APP_SERVICE_PING      "ti.ipc4.ping-pong"
#define IPC_APP_SERVICE_PING_LONG "ti.ipc4.ping-pong.ti.ipc4.ping-pong"

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void IpcApp_virtioTests(void);
static void IpcApp_multiprocessorTests(void);
static void IpcApp_mailboxTests(void);
static void IpcApp_utilTests(void);
static void IpcApp_initTests(void);
static void IpcApp_rpmsgTests(void);
static void IpcApp_otherTests(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Variables defined by test config */
extern uint8_t  *gIpcApp_CntrlBufPtr;
extern uint8_t  *gIpcApp_TaskStackBufPtr;
extern uint8_t  *gIpcApp_SendBufPtr;
extern uint8_t  *gIpcApp_RspBufPtr;
extern uint8_t  *gIpcApp_SysVqBufPtr;
extern uint32_t  gIpcApp_SelfProcId;
extern uint32_t *gIpcApp_RemoteProcArray;
extern uint32_t  gIpcApp_NumRemoteProc;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t IpcApp_extTest(void)
{
    UART_printf("Starting IPC extended tests\n");

    IpcApp_multiprocessorTests();

    IpcApp_mailboxTests();

    IpcApp_utilTests();

    IpcApp_initTests();
    
    IpcApp_virtioTests();

    IpcApp_rpmsgTests();

    IpcApp_otherTests();
        
    UART_printf("IPC extended tests have completed\n");

    #if defined LDRA_DYN_COVERAGE_EXIT
    UART_printf("\n LDRA ENTRY... \n");
    upload_execution_history();
    UART_printf("\n LDRA EXIT... \n");
    #endif

    UART_printf("All tests have passed.\n\n\n");

    return 0;
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */


static void IpcApp_virtioTests(void)
{
    Virtio_Handle vq = NULL;
    uint32_t p     = 0U;

    UART_printf("IPC extended tests: Running Virtio Tests\n");

    /* Test virtio enable and disable callback */
    Virtio_enableCallback(vq);
    Virtio_disableCallback(vq);

    /* Test NULL callback setting for Virtio */
    Virtio_setCallback(p, NULL, NULL);

    UART_printf("IPC extended tests: Virtio Tests Done\n");
}

static void IpcApp_multiprocessorTests(void)
{
    uint32_t numProc = gIpcApp_NumRemoteProc;
    char* name       = "mcu2_0";

    UART_printf("IPC extended tests: Running Multiprocessor Tests\n");

    /* Test multiprocessor config for invalid params */
    Ipc_mpSetConfig(gIpcApp_SelfProcId, numProc, gIpcApp_RemoteProcArray);
    Ipc_mpSetConfig(IPC_APP_INVALID_ID, numProc, gIpcApp_RemoteProcArray);
    Ipc_mpSetConfig(IPC_APP_INVALID_ID, IPC_APP_INVALID_ID, gIpcApp_RemoteProcArray);
    Ipc_mpSetConfig(gIpcApp_SelfProcId, 5U, gIpcApp_RemoteProcArray);

    /* Test multiprocessor get id for different params */
    Ipc_mpGetId(name);
    Ipc_mpGetId((char*)(NULL));
    Ipc_mpGetId((char*)(gIpcApp_SelfProcId));
    Ipc_mpGetId(0U);
#if defined (BUILD_MCU1_0)
    Ipc_mpGetId((char*)(IPC_APP_MP_INVALID_ID));
#endif
    /* Test multiprocessor get name API for different params */
    Ipc_mpGetName((uint32_t)(NULL));
    Ipc_mpGetName(gIpcApp_SelfProcId);
    Ipc_mpGetName(IPC_APP_INVALID_PROC);

    /* Other tests */
    Ipc_isCacheCoherent();
    Ipc_mpGetRemoteProcId(IPC_APP_ENDPT1);
    Ipc_getCoreName(IPC_APP_INVALID_PROC);

    UART_printf("IPC extended tests: Multiprocessor Tests Done\n");
}

static void IpcApp_mailboxTests(void)
{
    uintptr_t baseAddr    = 0U;
    uint32_t  queueId     = 0U;
    uint32_t  msg         = 0U;
    uint32_t remoteProcId = 0U;
    uint32_t timeoutCnt   = 0U;
    uint32_t arg          = 0U;
    uint32_t selfId       = Ipc_getCoreId();
    Mailbox_hwiCallback func = NULL;
    Ipc_Object *ipcObjPtr    = getIpcObjInst(0U);
    Ipc_OsalPrms *pOsalPrms  = &ipcObjPtr->initPrms.osalPrms;
    Ipc_InitPrms initPrms;
    uint32_t userId;
    uint32_t clusterId;

    Ipc_init(&initPrms);

    UART_printf("IPC extended tests: Running Mailbox Tests\n");

    /* Test mailbox register API for different params */
    Ipc_mailboxRegister(gIpcApp_SelfProcId, IPC_APP_INVALID_ID, func, arg, timeoutCnt);
    Ipc_mailboxRegister(gIpcApp_SelfProcId, remoteProcId, func, arg, timeoutCnt);
    Ipc_mailboxRegister(gIpcApp_SelfProcId, remoteProcId, func, arg, 100U);

    /* Test mailbox interrupt enable API for different params */
    Ipc_mailboxEnableNewMsgInt(IPC_APP_INVALID_ENDPT, remoteProcId);
    Ipc_mailboxEnableNewMsgInt(gIpcApp_SelfProcId, IPC_APP_INVALID_ENDPT);

    /* Test mailbox interrupt disable API for different params */
    Ipc_mailboxDisableNewMsgInt(gIpcApp_SelfProcId, remoteProcId);
    Ipc_mailboxDisableNewMsgInt(IPC_APP_INVALID_ENDPT, IPC_APP_INVALID_ENDPT);
    Ipc_mailboxDisableNewMsgInt(IPC_APP_INVALID_ENDPT, 15U);
    Ipc_mailboxDisableNewMsgInt(15U, IPC_APP_INVALID_ENDPT);

    /* Test mailbox send API */
    pOsalPrms -> disableAllIntr = NULL ;
    pOsalPrms -> restoreAllIntr = NULL ;
    Ipc_mailboxSend(selfId, remoteProcId, 1U, timeoutCnt);
    Mailbox_sendMessage(baseAddr, queueId, msg);

    /* Test mailbox clear API */
    Ipc_mailboxClear(baseAddr,queueId);

    /* Test mailbox get APIs */
    Ipc_getMailboxInfoTx(IPC_APP_INVALID_PROC, remoteProcId, &clusterId, &userId, &queueId);
    Ipc_getMailboxInfoRx(IPC_APP_INVALID_PROC, remoteProcId, &clusterId, &userId, &queueId);
    Ipc_getNavss512MailboxInputIntr(19U, userId);
    Ipc_getNavss512MailboxInputIntr(clusterId, 5U);
    Ipc_getNavss512MailboxInputIntr(clusterId, MAILBOX_USER_INVALID);
    Ipc_getNavss512MailboxInputIntr(MAILBOX_CLUSTER_INVALID, userId);

    UART_printf("IPC extended tests: Mailbox Tests Done\n");
}

static void IpcApp_utilTests(void)
{
    uint32_t size  = 0U;
    uint32_t align = 0U;
    IpcUtils_HeapParams hparam;
    IpcUtils_HeapHandle pHndl;
    IpcUtils_QHandle qhandle;
    IpcUtils_QElem   qelem;
    RPMessage_Handle *payload = NULL;

    /* Assign NULL to heap params */
    hparam.buf = NULL;

    UART_printf("IPC extended tests: Running Util Tests\n");

    /* Test queue APIs */
    IpcUtils_Qcreate(NULL);
    IpcUtils_QgetHeadNode(&qhandle);
    IpcUtils_QgetHeadNode(NULL_PTR);
    IpcUtils_Qenqueue(&qhandle, &qelem);
    IpcUtils_Qenqueue(&qhandle, NULL);
    IpcUtils_Qdequeue(&qhandle);
    IpcUtils_Qnext(&qelem);
    IpcUtils_Qnext(NULL_PTR);
    IpcUtils_Qdelete(&qhandle);
    IpcUtils_Qdelete(NULL_PTR);
    IpcUtils_QisEmpty(NULL);
    IpcUtils_Qremove(NULL);

    /* Test heap APIs */
    IpcUtils_HeapCreate(NULL, NULL);
    IpcUtils_HeapCreate(&pHndl, &hparam);
    IpcUtils_HeapAlloc(&pHndl, size, align);
    IpcUtils_HeapAlloc(NULL_PTR, size, align);
    IpcUtils_HeapFree(&pHndl, NULL, size);
    IpcUtils_HeapFree(&pHndl, &payload, size);
    IpcUtils_HeapFree(NULL, &payload, size);
    IpcUtils_HeapDelete(&pHndl);
    IpcUtils_HeapDelete(NULL_PTR);

    /* Test IPC Utils init/deinit */
    IpcUtils_Init(NULL);
    IpcUtils_DeInit();

    UART_printf("IPC extended tests: Util Tests Done\n");
}

static void IpcApp_initTests(void)
{
    uint32_t numProc = gIpcApp_NumRemoteProc;
    Ipc_VirtIoParams vqParam;

    UART_printf("IPC extended tests: Running Init Tests\n");

    /* Initialize Virtio */
    vqParam.vqObjBaseAddr = (void*)gIpcApp_SysVqBufPtr;
    vqParam.vqBufSize     = numProc * Ipc_getVqObjMemoryRequiredPerCore();
    vqParam.vringBaseAddr = (void*)VRING_BASE_ADDRESS;
    vqParam.vringBufSize  = IPC_VRING_BUFFER_SIZE;
    vqParam.timeoutCnt    = 100;  /* Wait for counts */

    /* Test IPC null initialization */
    IpcInitPrms_init(0U, NULL_PTR);
    Ipc_init(NULL);

    /* Test Ipc_initVirtIO with NULL and valid parameters */
    Ipc_initVirtIO(NULL);
    Ipc_initVirtIO(&vqParam);

    UART_printf("IPC extended tests: Init Tests Done\n");
}

static void IpcApp_rpmsgTests(void)
{
    uint32_t announcedEndpts = 0U;
    uint32_t remoteProcId = 0U;
    uint32_t srcEndPt     = 0U;
    uint32_t procId       = Ipc_mpGetSelfId();
    uint32_t selfId       = Ipc_getCoreId();
    uint16_t len          = 0U;
    void     *buf         = NULL;
    char     str[IPC_APP_MSGSIZE];
    uint32_t remoteEndPt;
    RPMessage_Handle handle = NULL;
    Ipc_OsalPrms initPrms;

    UART_printf("IPC extended tests: Running RPMessage Tests\n");

    /* Test RP message announce */
    RPMessage_announce(0U, announcedEndpts, NULL_PTR);
    RPMessage_announce(0U, announcedEndpts, NULL_PTR);
    RPMessage_announce(0U, 0U, NULL);
    RPMessage_announce(32U, 0U, NULL);
    RPMessage_announce(IPC_APP_INVALID_PROC, 0U, IPC_APP_SERVICE_PING_LONG);
    RPMessage_announce(IPC_APP_INVALID_PROC, 0U, IPC_APP_SERVICE_PING);

    /* Test RPMessage send/receive with invalid parameters */
    RPMessage_send(NULL, IPC_APP_INVALID_ID, IPC_APP_ENDPT1, srcEndPt, (Ptr)buf, len);
    RPMessage_send(NULL, procId, IPC_APP_ENDPT1, srcEndPt, (Ptr)buf, len);
    RPMessage_recvNb(handle, (Ptr)str, &len, &remoteEndPt, &remoteProcId);

    /* Test RPMessage unblock */
    Ipc_Object *ipcObjPtr = getIpcObjInst(0U);
    Ipc_OsalPrms *pOsalPrms = &ipcObjPtr->initPrms.osalPrms;
    pOsalPrms -> unlockMutex = NULL ;
    RPMessage_unblock(handle);
    IpcOsalPrms_init(&initPrms);
    RPMessage_unblock(handle);

    /* Test RPMessage get remote end point APIs */
    RPMessage_getRemoteEndPt(selfId, NULL, &remoteProcId, &remoteEndPt, 1000);
    RPMessage_getRemoteEndPtToken(gIpcApp_SelfProcId, NULL, &remoteProcId, &remoteEndPt, 1000, 0);

    UART_printf("IPC extended tests: RPMessage Tests Done\n");
}

static void IpcApp_otherTests(void)
{
    uintptr_t arg         = 0U;
    uint32_t remoteProcId = 0U;
    uint32_t index        = 0U;
    uint32_t size         = 0U;
    uint32_t token        = 0U;
    uint32_t daAddr       = 0U;
    uint32_t remoteEndPt;
    Ipc_InitPrms *intcfg  = NULL;
    Ipc_MbConfig *cfg     = NULL;
    Ipc_OsalIsrFxn func   = NULL;
    RPMessage_Params cntrlParam;
    RPMessage_Params params;
    
    /* Initialize an RPMessage_Params structure */
    RPMessageParams_init(&cntrlParam);

    UART_printf("IPC extended tests: Running Other Tests\n");

    /* Set HeapMemory for control task */
    cntrlParam.buf         = gIpcApp_CntrlBufPtr;
    cntrlParam.bufSize     = RPMSG_DATA_SIZE;
    cntrlParam.stackBuffer = &gIpcApp_TaskStackBufPtr[index * IPC_TASK_STACKSIZE];
    cntrlParam.stackSize   = IPC_TASK_STACKSIZE;

    /* Test RPMessage_init with NULL parameters */
    {
      Ipc_Object *ipcObjPtr = getIpcObjInst(0U);
      Ipc_OsalPrms *pOsalPrms = &ipcObjPtr->initPrms.osalPrms;
      pOsalPrms -> registerIntr = NULL_PTR ;
      RPMessage_init(&cntrlParam);
    }  
    
    {
      Ipc_Object *ipcObjPtr = getIpcObjInst(0U);
      Ipc_OsalPrms *pOsalPrms = &ipcObjPtr->initPrms.osalPrms;
      pOsalPrms -> createHIsrGate = NULL_PTR ;
      pOsalPrms -> createHIsr = NULL_PTR ;
      RPMessage_init(&cntrlParam);
    }    
    
    RPMessage_init(NULL);

    cntrlParam.stackSize   = IPC_TASK_STACKSIZE;
    RPMessage_init(&cntrlParam);
        
    RPMessage_getMessageBufferSize();

    RPMessage_unblockGetRemoteEndPt(token);

    /*Test RPMessage_unblockGetRemoteEndPt with NULL Pointer */
    {
        Ipc_Object *ipcObjPtr = getIpcObjInst(0U);
        Ipc_OsalPrms *pOsalPrms = &ipcObjPtr->initPrms.osalPrms;
        pOsalPrms -> unLockHIsrGate = NULL_PTR ;
        pOsalPrms -> lockHIsrGate = NULL_PTR ;
        pOsalPrms -> unlockMutex = NULL_PTR ;
        RPMessage_unblockGetRemoteEndPt(token);
    }

    cfg->eventId = IPC_APP_CSL_CLEC_MAX_EVT_IN;	
    Mailbox_plugInterrupt(cfg, func, arg);
    
    IpcUtils_Qput(NULL,NULL);
    
    IpcUtils_DeInit(); 
                                       
    Ipc_Object *ipcObjPtr = getIpcObjInst(0U);
    Ipc_OsalPrms *pOsalPrms = &ipcObjPtr->initPrms.osalPrms;
    pOsalPrms -> createMutex = NULL_PTR ;
    pOsalPrms -> lockMutex =  NULL_PTR ;
    pOsalPrms -> deleteMutex = NULL_PTR ;
    pOsalPrms -> lockHIsrGate = NULL_PTR ;
    pOsalPrms -> unLockHIsrGate = NULL_PTR ;

    RPMessage_getRemoteEndPtToken(gIpcApp_SelfProcId, IPC_APP_SERVICE_PING_LONG, &remoteProcId,
                &remoteEndPt,1000,0);
                
    RPMessage_getRemoteEndPt(Ipc_getCoreId(), NULL, &remoteProcId, &remoteEndPt, 1000);
    
    RPMessage_getRemoteEndPtToken(gIpcApp_SelfProcId, NULL, &remoteProcId, &remoteEndPt, 1000, 0);

    RPMessageParams_init(NULL);

    RPMessageParams_init(&params);

    Ipc_getVqObjMemoryRequired();

    IpcUtils_getMemoryAddress(daAddr,size);

    Ipc_lateVirtioCreate(IPC_MCU1_0);

    RPMessage_lateInit(IPC_MCU1_0);

    RPMessage_deInit();
    
    Ipc_loadResourceTable(NULL);
    
    Ipc_getResourceTraceBufPtr();
    
    Ipc_resetCoreVirtIO(IPC_MCU1_0);
        
    {
      Ipc_Object *ipcObjPtr = getIpcObjInst(0U);
      Ipc_OsalPrms *pOsalPrms = &ipcObjPtr->initPrms.osalPrms;
      pOsalPrms -> disableAllIntr = NULL_PTR ;
      pOsalPrms -> restoreAllIntr =  NULL_PTR ;
      Ipc_init(intcfg);          
    }
    
    Ipc_init(intcfg);  

    {
      Ipc_Object *ipcObjPtr = getIpcObjInst(0U);
      Ipc_OsalPrms *pOsalPrms = &ipcObjPtr->initPrms.osalPrms;
      pOsalPrms -> createMutex = NULL_PTR ;
      pOsalPrms -> deleteMutex =  NULL_PTR ;
      pOsalPrms -> lockMutex = NULL_PTR ;
      pOsalPrms -> unlockMutex =  NULL_PTR ;
      Ipc_init(intcfg);  
    }
            
    ipcObjPtr->printLock = NULL;
    Ipc_deinit();
    
    Ipc_deinit();
    
    UART_printf("IPC extended tests: Other Tests Done\n");
}
