/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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

/*!
 * \file enet_mcm.c
 *
 * \brief This file contains Multi-client Manager related functionality.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* This is needed for memset/memcpy */
#include <string.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/ClockP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/MailboxP.h>
#include <ti/osal/MutexP.h>
#include <ti/osal/HwiP.h>

#if defined(SAFERTOS)
#include "SafeRTOS_API.h"
#endif

#include "include/enet_apputils.h"
#include "include/enet_appmemutils.h"
#include "include/enet_mcm.h"

#include <ti/drv/enet/include/core/enet_soc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENETMCM_MBOX_MSG_COUNT              (10U)
#define ENETMCM_TSK_PRIORITY                (2U)
#define ENETMCM_PERIODICTSK_PRIORITY        (7U)
#define ENETMCM_MAX_CLIENTS                 (12U)
#define ENETMCM_INTERNAL_CLIENT_IDX         (ENETMCM_MAX_CLIENTS)


#if defined(SAFERTOS)
#define ENETMCM_TSK_STACK_SIZE              (16U * 1024U)
#define ENETMCM_TSK_STACK_ALIGN             ENETMCM_TSK_STACK_SIZE
#define ENETMCM_MBOX_SIZE                   (sizeof(EnetMcm_mailboxObj) * ENETMCM_MBOX_MSG_COUNT + safertosapiQUEUE_OVERHEAD_BYTES)
#else
#define ENETMCM_TSK_STACK_SIZE              (8U * 1024U)
#define ENETMCM_TSK_STACK_ALIGN             (32U)
#define ENETMCM_MBOX_SIZE                   (sizeof(EnetMcm_mailboxObj) * ENETMCM_MBOX_MSG_COUNT)
#endif

/*! \brief Get the index of the given element within an array */
#define ENETMCM_UTILS_GETARRAYINDEX(member, array)   (member - &array[0])

/*! \brief Macro to determine if a member is part of an array */
#define ENETMCM_UTILS_ARRAYISMEMBER(member, array)                              \
    (((((uintptr_t)member - (uintptr_t) & array[0]) % sizeof(array[0])) == 0)   \
     && (member >= &array[0])                                                   \
     && (ENETMCM_UTILS_GETARRAYINDEX(member, array) < ENET_ARRAYSIZE(array)))

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/*!
 * \brief CPSW MCM handle
 *
 * CPSW MCM opaque handle.
 */
typedef struct EnetMcm_Obj_s *EnetMcm_Handle;

typedef struct EnetMcm_CoreAttachTableEntry_s
{
    uint32_t coreId;
    EnetPer_AttachCoreOutArgs coreInfo;
}  EnetMcm_CoreAttachTableEntry;

typedef struct EnetMcm_CoreAttachTable_s
{
    uint32_t numCoresAttached;
    EnetMcm_CoreAttachTableEntry entry[ENET_CFG_REMOTE_CLIENT_CORES_MAX];
}  EnetMcm_CoreAttachTable;

typedef enum EnetMcm_Command_e
{
    /*! GET CPSW & UDMA HANDLE */
    MCM_GET_HANDLE,

    /*! RELEASE HANDLE */
    MCM_RELEASE_HANDLE,

    /*! ATTACH core */
    MCM_CORE_ATTACH,

    /*! DETACH core */
    MCM_CORE_DETACH,

    /*! Run IOCTL */
    MCM_IOCTL,

    /*! SHUTDOWN MCM */
    MCM_SHUTDOWN,

    /*! Disconnecting client */
    MCM_CLIENT_DISCONNECT,

    /*! Save Context of Enet Peripheral */
    MCM_SAVE_CTXT,

    /*! Restore Context of Enet Peripheral */
    MCM_RESTORE_CTXT,

    /*! Open MAC Ports */
    MCM_OPEN_PORT,

    /*! Close MAC Ports */
    MCM_CLOSE_PORT,

    /*! Stop periodic ticks */
    MCM_STOP_CLOCK,

    /*! Start periodic ticks */
    MCM_START_CLOCK,

    /*! Response to MCM_GET_HANDLE command */
    MCM_RESPONSE_GET_HANDLE,

    /*! Response to MCM_RELEASE_HANDLE command */
    MCM_RESPONSE_RELEASE_HANDLE,

    /*! Response to MCM_CORE_ATTACH command */
    MCM_RESPONSE_CORE_ATTACH,

    /*! Response to MCM_CORE_DETACH command */
    MCM_RESPONSE_CORE_DETACH,

    /*! Response to MCM_IOCL command */
    MCM_RESPONSE_IOCTL,

    /*! Response to SHUTDOWN MCM */
    MCM_RESPONSE_SHUTDOWN,

    /*! Response to Client Disconnect */
    MCM_RESPONSE_CLIENT_DISCONNECT,

    /*! Save Context of Enet Peripheral */
    MCM_RESPONSE_SAVE_CTXT,

    /*! Response to Restore Context of Enet Peripheral */
    MCM_RESPONSE_RESTORE_CTXT,

    /*! Response to Open MAC Ports */
    MCM_RESPONSE_OPEN_PORT,

    /*! Response to Close MAC Ports */
    MCM_RESPONSE_CLOSE_PORT,

    /*! Response to Stop periodic ticks */
    MCM_RESPONSE_STOP_CLOCK,

    /*! Response to Start periodic ticks */
    MCM_RESPONSE_START_CLOCK,
}
EnetMcm_Command;

typedef struct EnetMcm_mailboxObj_s
{
    EnetMcm_Command cmd;
    MailboxP_Handle response;

    struct EnetMcm_mailboxMsgs_s
    {
        uint32_t coreId;
        uint32_t coreKey;
        uint32_t ioctlCmd;
        uint32_t status;
        Enet_IoctlPrms *ioctlPrms;
        EnetMcm_HandleInfo handleInfo;
        EnetPer_AttachCoreOutArgs attachInfo;
    } msgBody;
} EnetMcm_mailboxObj;

typedef struct EnetMcm_RespMboxObj_s
{
    QueueP_Elem qElem;
    bool isValidClient;
    MailboxP_Handle hMbox;
} EnetMcm_RespMboxObj;

typedef struct EnetMcm_Obj_s
{
    bool isInitDone;

    uint8_t refCnt;

    Enet_Type enetType;

    uint32_t instId;

    Enet_Handle hEnet;

    Udma_DrvHandle hUdmaDrv;

    Cpsw_Cfg cpswCfg;

    EnetDma_Cfg dmaCfg;

    uint32_t selfCoreId;

    EnetMcm_setPortLinkCfg setPortLinkCfg;

    Enet_MacPort macPortList[ENET_MAC_PORT_NUM];

    uint8_t numMacPorts;

    TaskP_Handle task;

    MailboxP_Handle hMboxCmd;

    EnetMcm_RespMboxObj *hInternalClientRespMbox;

    QueueP_Handle hMboxRespFreeQ;

    EnetMcm_RespMboxObj respMbox[(ENETMCM_MAX_CLIENTS + 1)];

    ClockP_Handle hTimer;

    TaskP_Handle task_periodicTick;

    SemaphoreP_Handle timerSem;

    volatile bool timerTaskShutDownFlag;

    MutexP_Object mutexObj;

    MutexP_Handle hMutex;

    uint32_t periodicTaskPeriod;

    EnetMcm_CoreAttachTable coreAttachTable;

    Enet_Print print;

    /* Trace timestamp provider */
    EnetTrace_TraceTsFunc traceTsFunc;

    /* Extended trace function */
    EnetTrace_ExtTraceFunc extTraceFunc;

    uint8_t mcmMainTaskStack[ENETMCM_TSK_STACK_SIZE] __attribute__ ((aligned(ENETMCM_TSK_STACK_ALIGN)));

    uint8_t mcmPrdTaskStack[ENETMCM_TSK_STACK_SIZE] __attribute__ ((aligned(ENETMCM_TSK_STACK_ALIGN)));

    uint8_t mcmRequestMbxBuf[ENETMCM_MBOX_SIZE] __attribute__ ((aligned(32)));

    uint8_t mcmResponseMbxBuf[(ENETMCM_MAX_CLIENTS + 1)][ENETMCM_MBOX_SIZE] __attribute__ ((aligned(32)));
}EnetMcm_Obj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t  EnetMcm_open(EnetMcm_Handle hMcm);

static void     EnetMcm_close(EnetMcm_Handle hMcm);

static void     EnetMcm_serverTask(void * hMcm,
                                   void * arg1);

static int32_t  EnetMcm_enablePorts(EnetMcm_Handle hMcm);

static void EnetMcm_shutdown(Enet_Type enetType,
                             EnetMcm_CmdIf *hMcmCmdIf);

static EnetMcm_RespMboxObj* EnetMcm_getRespMbox(EnetMcm_Handle hMcm,
                                                MailboxP_Handle hMboxResp);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static EnetMcm_Obj gMcmObj[] =
{
    [ENET_CPSW_2G] =
    {
        .isInitDone             = BFALSE,
        .timerTaskShutDownFlag  = BFALSE,
        .hMutex                 = NULL,
        .task                   = NULL,
        .hMboxCmd               = NULL,
        .hInternalClientRespMbox = NULL,
        .hTimer                 = NULL,
        .task_periodicTick      = NULL,
        .timerSem               = NULL,
    },

    [ENET_CPSW_5G] =
    {
        .isInitDone             = BFALSE,
        .timerTaskShutDownFlag  = BFALSE,
        .hMutex                 = NULL,
        .task                   = NULL,
        .hMboxCmd               = NULL,
        .hInternalClientRespMbox = NULL,
        .hTimer                 = NULL,
        .task_periodicTick      = NULL,
        .timerSem               = NULL,
    },

    [ENET_CPSW_9G] =
    {
        .isInitDone             = BFALSE,
        .timerTaskShutDownFlag  = BFALSE,
        .hMutex                 = NULL,
        .task                   = NULL,
        .hMboxCmd               = NULL,
        .hInternalClientRespMbox = NULL,
        .hTimer                 = NULL,
        .task_periodicTick      = NULL,
        .timerSem               = NULL,
    },
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void EnetMcm_initAttachTable(EnetMcm_CoreAttachTable *attachTbl)
{
    uint32_t i;

    attachTbl->numCoresAttached = 0U;
    for (i = 0U; i < ENET_ARRAYSIZE(attachTbl->entry); i++)
    {
        attachTbl->entry[i].coreId = ENET_RM_INVALIDCORE;
    }
}

static EnetMcm_CoreAttachTableEntry *EnetMcm_getCoreAttachEntry(EnetMcm_CoreAttachTable *attachTbl,
                                                                uint32_t coreId)
{
    uint32_t i;
    EnetMcm_CoreAttachTableEntry *entry;

    entry = NULL;
    for (i = 0; i < attachTbl->numCoresAttached; i++)
    {
        if (attachTbl->entry[i].coreId == coreId)
        {
            entry = &attachTbl->entry[i];
            break;
        }
    }

    return entry;
}

static void EnetMcm_addCoreAttachEntry(EnetMcm_CoreAttachTable *attachTbl,
                                       uint32_t coreId,
                                       EnetPer_AttachCoreOutArgs *coreInfo)
{
    EnetMcm_CoreAttachTableEntry *entry;

    entry = EnetMcm_getCoreAttachEntry(attachTbl, coreId);
    if (entry == NULL)
    {
        EnetAppUtils_assert(attachTbl->numCoresAttached < ENET_ARRAYSIZE(attachTbl->entry));
        attachTbl->entry[attachTbl->numCoresAttached].coreId   = coreId;
        attachTbl->entry[attachTbl->numCoresAttached].coreInfo = *coreInfo;
        attachTbl->numCoresAttached++;
    }
    else
    {
        uint32_t i;

        EnetAppUtils_assert(entry->coreId == coreId);
        EnetAppUtils_assert(entry->coreInfo.coreKey == coreInfo->coreKey);
        EnetAppUtils_assert(entry->coreInfo.rxMtu == coreInfo->rxMtu);
        for (i = 0; i < ENET_ARRAYSIZE(entry->coreInfo.txMtu); i++)
        {
            EnetAppUtils_assert(entry->coreInfo.txMtu[i] == coreInfo->txMtu[i]);
        }
    }
}

static void EnetMcm_delCoreAttachEntry(EnetMcm_CoreAttachTable *attachTbl,
                                       uint32_t coreId,
                                       uint32_t coreKey)
{
    uint32_t i;
    EnetMcm_CoreAttachTableEntry *entry;

    entry = EnetMcm_getCoreAttachEntry(attachTbl, coreId);
    if (entry != NULL)
    {
        EnetAppUtils_assert(attachTbl->numCoresAttached > 0);
        for (i = 0; i < attachTbl->numCoresAttached; i++)
        {
            if (attachTbl->entry[i].coreId == coreId)
            {
                break;
            }
        }

        EnetAppUtils_assert(i < attachTbl->numCoresAttached);
        EnetAppUtils_assert(attachTbl->entry[i].coreId == coreId);
        EnetAppUtils_assert(attachTbl->entry[i].coreInfo.coreKey == coreKey);
        /* Move the last entry to the freed entry */
        attachTbl->numCoresAttached--;
        attachTbl->entry[i] = attachTbl->entry[attachTbl->numCoresAttached];
    }
}

static void EnetMcm_initMbox(EnetMcm_Handle hMcm)
{
    TaskP_Params params;
    EnetMcm_RespMboxObj *respMbox;
    QueueP_Params qPrms;
    MailboxP_Params mboxParams;
    uint32_t i;

    QueueP_Params_init(&qPrms);

    hMcm->hMboxRespFreeQ = QueueP_create(&qPrms);
    EnetAppUtils_assert(hMcm->hMboxRespFreeQ != NULL);

    MailboxP_Params_init(&mboxParams);
    mboxParams.name = (uint8_t *)"MCM Request MBX";
    mboxParams.size =  sizeof(EnetMcm_mailboxObj);
    mboxParams.count = ENETMCM_MBOX_MSG_COUNT;
    mboxParams.buf = (void *)hMcm->mcmRequestMbxBuf;
    mboxParams.bufsize = sizeof(hMcm->mcmRequestMbxBuf);

    hMcm->hMboxCmd = MailboxP_create(&mboxParams);
    EnetAppUtils_assert(hMcm->hMboxCmd != NULL);

    for (i = 0U; i < ENET_ARRAYSIZE(hMcm->respMbox); i++)
    {
        MailboxP_Params_init(&mboxParams);
        mboxParams.name = (uint8_t *)"MCM Response MBX";
        mboxParams.size =  sizeof(EnetMcm_mailboxObj);
        mboxParams.count = ENETMCM_MBOX_MSG_COUNT;
        mboxParams.buf = (void *)&hMcm->mcmResponseMbxBuf[i][0];
        mboxParams.bufsize = sizeof(hMcm->mcmResponseMbxBuf[i]);

        respMbox = &hMcm->respMbox[i];
        respMbox->hMbox = MailboxP_create(&mboxParams);
        respMbox->isValidClient = BFALSE;
        EnetAppUtils_assert(respMbox->hMbox != NULL);
        if (i != ENETMCM_INTERNAL_CLIENT_IDX)
        {
            QueueP_put(hMcm->hMboxRespFreeQ, &respMbox->qElem);
        }
        else
        {
            respMbox->isValidClient = BTRUE;
            hMcm->hInternalClientRespMbox = respMbox;
        }
    }

    TaskP_Params_init(&params);

    params.priority  = ENETMCM_TSK_PRIORITY;
    params.stack     = &hMcm->mcmMainTaskStack[0];
    params.stacksize = sizeof(hMcm->mcmMainTaskStack);
    params.arg0      = hMcm;

    switch (hMcm->enetType)
    {
        case ENET_CPSW_2G:
            params.name = (const char *)"MCM2G_Task";
            break;

#if defined(SOC_J7200)
    case ENET_CPSW_5G:
            params.name = (const char *)"MCM5G_Task";
            break;
#endif
#if defined(SOC_J721E) || defined(SOC_J784S4) || defined(SOC_J742S2)
    case ENET_CPSW_9G:
            params.name = (const char *)"MCM9G_Task";
            break;
#endif
        default:
            EnetAppUtils_assert(BFALSE);
    }

    hMcm->task = TaskP_create(&EnetMcm_serverTask, &params);

    EnetAppUtils_assert(hMcm->task != NULL);
}

static int32_t EnetMcm_validateInitCfg(const EnetMcm_InitConfig *pMcmInitCfg)
{
    int32_t status = ENET_SOK;
    EnetDma_Cfg *udmaCfg = NULL;
    Cpsw_Cfg *cpswCfg;
    Enet_Type enetType;
    uint32_t instId;

    if ((pMcmInitCfg != NULL) &&
        (pMcmInitCfg->perCfg != NULL) &&
        (pMcmInitCfg->setPortLinkCfg != NULL))
    {
        enetType = pMcmInitCfg->enetType;
        instId = pMcmInitCfg->instId;

        cpswCfg = (Cpsw_Cfg *) pMcmInitCfg->perCfg;
        udmaCfg = (EnetDma_Cfg *)cpswCfg->dmaCfg;
        if ((udmaCfg == NULL) || (udmaCfg->hUdmaDrv == NULL))
        {
            status = ENET_EINVALIDPARAMS;
        }

        if ((enetType == ENET_CPSW_2G) &&
            ((pMcmInitCfg->numMacPorts != EnetSoc_getMacPortMax(enetType, instId)) ||
             (pMcmInitCfg->macPortList[0] != ENET_MAC_PORT_1)))
        {
            status = ENET_EINVALIDPARAMS;
        }

        if ((pMcmInitCfg->numMacPorts > EnetSoc_getMacPortMax(enetType, instId)))
        {
            status = ENET_EINVALIDPARAMS;
        }

    }
    else
    {
        status = ENET_EINVALIDPARAMS;
    }

    return status;
}

int32_t  EnetMcm_init(const EnetMcm_InitConfig *pMcmInitCfg)
{
    int32_t status = ENET_SOK;
    uintptr_t key;
    Enet_Type enetType  = pMcmInitCfg->enetType;
    EnetMcm_Handle hMcm;
    Cpsw_Cfg *cpswCfg;
    EnetDma_Cfg *dmaCfg = NULL;

    key = HwiP_disable();

    hMcm = &gMcmObj[enetType];
    if (hMcm->hMutex == NULL)
    {
        hMcm->hMutex = MutexP_create(&hMcm->mutexObj);
        EnetAppUtils_assert(hMcm->hMutex != NULL);
    }

    HwiP_restore(key);
    EnetAppUtils_assert(hMcm->hMutex != NULL);
    MutexP_lock(hMcm->hMutex, MutexP_WAIT_FOREVER);

    if (hMcm->isInitDone == BFALSE)
    {
        status = EnetMcm_validateInitCfg(pMcmInitCfg);

        if (status == ENET_SOK)
        {
            cpswCfg = (Cpsw_Cfg *)pMcmInitCfg->perCfg;
            dmaCfg = (EnetDma_Cfg *)cpswCfg->dmaCfg;

            hMcm->selfCoreId         = cpswCfg->resCfg.selfCoreId;
            hMcm->cpswCfg            = *cpswCfg;
            hMcm->dmaCfg             = *dmaCfg;
            hMcm->refCnt             = 0U;
            hMcm->enetType           = enetType;
            hMcm->instId             = pMcmInitCfg->instId;
            hMcm->hEnet              = NULL;
            hMcm->hUdmaDrv           = NULL;
            hMcm->setPortLinkCfg     = pMcmInitCfg->setPortLinkCfg;
            hMcm->numMacPorts        = pMcmInitCfg->numMacPorts;
            hMcm->periodicTaskPeriod = pMcmInitCfg->periodicTaskPeriod;

            hMcm->print = pMcmInitCfg->print;
            if (hMcm->print == NULL)
            {
                hMcm->print = EnetAppUtils_print;
            }

            hMcm->traceTsFunc = pMcmInitCfg->traceTsFunc;
            hMcm->extTraceFunc = pMcmInitCfg->extTraceFunc;

            EnetMcm_initAttachTable(&hMcm->coreAttachTable);

            memcpy(&hMcm->macPortList[0U], &pMcmInitCfg->macPortList[0U], sizeof(pMcmInitCfg->macPortList));

            EnetMcm_initMbox(hMcm);

            hMcm->isInitDone = BTRUE;
        }
    }

    MutexP_unlock(hMcm->hMutex);

    return status;
}

void  EnetMcm_getCmdIf(Enet_Type enetType,
                       EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_Handle hMcm = &gMcmObj[enetType];
    EnetMcm_RespMboxObj *respMbox;
    QueueP_Elem *elem;

    EnetAppUtils_assert(hMcmCmdIf != NULL);
    EnetAppUtils_assert(hMcm->hMutex != NULL);

    MutexP_lock(hMcm->hMutex, MutexP_WAIT_FOREVER);
    EnetAppUtils_assert(hMcm->isInitDone == BTRUE);

    elem = (QueueP_Elem *)QueueP_get(hMcm->hMboxRespFreeQ);
    EnetAppUtils_assert(elem != NULL);
    respMbox = container_of(elem, EnetMcm_RespMboxObj, qElem);

    hMcmCmdIf->hMboxCmd      = hMcm->hMboxCmd;
    hMcmCmdIf->hMboxResponse = respMbox->hMbox;

    EnetAppUtils_assert(respMbox->isValidClient == BFALSE);
    respMbox->isValidClient = BTRUE;

    MutexP_unlock(hMcm->hMutex);

    EnetAppUtils_assert(hMcmCmdIf->hMboxCmd != NULL);
    EnetAppUtils_assert(hMcmCmdIf->hMboxResponse != NULL);
}

void EnetMcm_releaseCmdIf(Enet_Type enetType,
                          EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_Handle hMcm = &gMcmObj[enetType];
    EnetMcm_RespMboxObj *respMbox = NULL;
    EnetMcm_mailboxObj msg;

    EnetAppUtils_assert(hMcmCmdIf != NULL);
    EnetAppUtils_assert(hMcm->hMutex != NULL);

    memset(&msg, 0, sizeof(msg));
    msg.cmd = MCM_CLIENT_DISCONNECT;
    msg.response = hMcmCmdIf->hMboxResponse;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_CLIENT_DISCONNECT);
    EnetAppUtils_assert(MailboxP_getNumPendingMsgs(hMcmCmdIf->hMboxResponse) == 0);

    respMbox = EnetMcm_getRespMbox(hMcm, hMcmCmdIf->hMboxResponse);
    /* The mcm server should have cleared the isValidClient on receiving the disconnect command */
    EnetAppUtils_assert(respMbox->isValidClient == BFALSE);

    QueueP_put(hMcm->hMboxRespFreeQ, &respMbox->qElem);
}

static void EnetMcm_releaseInternalRespMbox(EnetMcm_Handle hMcm)
{
    EnetMcm_mailboxObj msg;

    memset(&msg, 0, sizeof(msg));
    msg.cmd = MCM_CLIENT_DISCONNECT;
    EnetAppUtils_assert(hMcm->hInternalClientRespMbox != NULL);
    msg.response = hMcm->hInternalClientRespMbox->hMbox;

    MailboxP_post(hMcm->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcm->hInternalClientRespMbox->hMbox, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_CLIENT_DISCONNECT);
    EnetAppUtils_assert(MailboxP_getNumPendingMsgs(hMcm->hInternalClientRespMbox->hMbox) == 0);

    EnetAppUtils_assert(hMcm->hInternalClientRespMbox->isValidClient == BFALSE);
}

void EnetMcm_deInit(Enet_Type enetType)
{
    EnetMcm_Handle hMcm = &gMcmObj[enetType];
    uint32_t i;
    EnetMcm_CmdIf mcmCmdIf;

    EnetAppUtils_assert(hMcm->hMutex != NULL);

    MutexP_lock(hMcm->hMutex, MutexP_WAIT_FOREVER);

    if (hMcm->isInitDone == BTRUE)
    {
        /*! Releasing response queue mailbox per client */
        for (i = 0U; i < ENETMCM_MAX_CLIENTS; i++)
        {
            if ((i != ENETMCM_INTERNAL_CLIENT_IDX) && (hMcm->respMbox[i].isValidClient == BTRUE))
            {
                mcmCmdIf.hMboxCmd = hMcm->hMboxCmd;
                mcmCmdIf.hMboxResponse = hMcm->respMbox[i].hMbox;
                EnetMcm_releaseCmdIf(enetType, &mcmCmdIf);
            }
        }

        /* For shutting down MCM*/
        EnetMcm_releaseInternalRespMbox(hMcm);
        mcmCmdIf.hMboxCmd = hMcm->hMboxCmd;
        EnetAppUtils_assert(hMcm->hInternalClientRespMbox != NULL);
        mcmCmdIf.hMboxResponse = hMcm->hInternalClientRespMbox->hMbox;
        EnetMcm_shutdown(enetType, &mcmCmdIf);

        /* Deleting hMboxCmd Mailbox*/
        EnetAppUtils_assert(MailboxP_getNumPendingMsgs(hMcm->hMboxCmd) == 0);
        MailboxP_delete(hMcm->hMboxCmd);
        hMcm->hMboxCmd = NULL;

        /* Deleting all hMboxResp Mailboxes */
        for (i = 0U; i < ENET_ARRAYSIZE(hMcm->respMbox); i++)
        {
            EnetAppUtils_assert(MailboxP_getNumPendingMsgs(hMcm->respMbox[i].hMbox) == 0);
            MailboxP_delete(hMcm->respMbox[i].hMbox);
        }
        hMcm->hInternalClientRespMbox = NULL;

        QueueP_delete(hMcm->hMboxRespFreeQ);

        EnetMcm_initAttachTable(&hMcm->coreAttachTable);
        while (TaskP_isTerminated(hMcm->task) != 1)
        {
            TaskP_sleep(10U);
        }
        TaskP_delete(&hMcm->task);
        hMcm->isInitDone = BFALSE;
    }

    MutexP_unlock(hMcm->hMutex);

    MutexP_delete(hMcm->hMutex);
    hMcm->hMutex = NULL;
}

static int32_t EnetMcm_enablePorts(EnetMcm_Handle hMcm)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    Enet_Handle hEnet = hMcm->hEnet;
    uint32_t coreId   = hMcm->selfCoreId;
    uint8_t i;
    bool alive;

    /* Show alive PHYs */
    for (i = 0U; i < ENET_MDIO_PHY_CNT_MAX; i++)
    {
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &i, &alive);
        status = Enet_ioctl(hEnet,
                            coreId,
                            ENET_MDIO_IOCTL_IS_ALIVE,
                            &prms);
        if (status == ENET_SOK)
        {
            if (alive == BTRUE)
            {
                hMcm->print("PHY %d is alive\n", i);
            }
        }
        else
        {
            hMcm->print("Failed to get PHY %d alive status: %d\n", i, status);
        }
    }

    for (i = 0U; i < hMcm->numMacPorts; i++)
    {
        EnetPer_PortLinkCfg linkArgs;
        CpswMacPort_Cfg cpswMacCfg;

        linkArgs.macCfg = &cpswMacCfg;
        linkArgs.macPort = hMcm->macPortList[i];
        hMcm->setPortLinkCfg(&linkArgs, hMcm->macPortList[i]);

        ENET_IOCTL_SET_IN_ARGS(&prms, &linkArgs);
        status = Enet_ioctl(hEnet,
                            coreId,
                            ENET_PER_IOCTL_OPEN_PORT_LINK,
                            &prms);
        if (status != ENET_SOK)
        {
            hMcm->print("EnetMcm_enablePorts() failed to open MAC port: %d\n", status);
        }
    }

    if (status == ENET_SOK)
    {
        CpswAle_SetPortStateInArgs setPortStateInArgs;

        setPortStateInArgs.portNum   = CPSW_ALE_HOST_PORT_NUM;
        setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_FORWARD;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);
        prms.outArgs = NULL;
        status       = Enet_ioctl(hEnet,
                                  coreId,
                                  CPSW_ALE_IOCTL_SET_PORT_STATE,
                                  &prms);
        if (status != ENET_SOK)
        {
            hMcm->print("EnetMcm_enablePorts() failed CPSW_ALE_IOCTL_SET_PORT_STATE: %d\n", status);
        }

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_NO_ARGS(&prms);
            status = Enet_ioctl(hEnet,
                                coreId,
                                ENET_HOSTPORT_IOCTL_ENABLE,
                                &prms);
            if (status != ENET_SOK)
            {
                hMcm->print("EnetMcm_enablePorts() Failed to enable host port: %d\n", status);
            }
        }
    }

    return status;
}

static void EnetMcm_timerCb(void * arg)
{
    SemaphoreP_Handle timerSem = (SemaphoreP_Handle)arg;

    /* Tick! */
    SemaphoreP_post(timerSem);
}

static void EnetMcm_periodicTick(void * hTimerSem,
                                 void * mcmHandle)
{
    SemaphoreP_Handle timerSem = (SemaphoreP_Handle)hTimerSem;
    EnetMcm_Handle hMcm       = (EnetMcm_Handle)mcmHandle;

    while (hMcm->timerTaskShutDownFlag != BTRUE)
    {
        SemaphoreP_pend(timerSem, SemaphoreP_WAIT_FOREVER);
        /* Enet_periodicTick should be called from only task context */
        Enet_periodicTick(hMcm->hEnet);
    }
}

static void EnetMcm_createClock(EnetMcm_Handle hMcm)
{
    TaskP_Params params;
    SemaphoreP_Params semParams;
    ClockP_Params clkParams;

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_COUNTING;
    hMcm->timerSem = SemaphoreP_create(0, &semParams);

    ClockP_Params_init(&clkParams);
    clkParams.startMode = ClockP_StartMode_USER;
    clkParams.period    = hMcm->periodicTaskPeriod;
    clkParams.runMode   = ClockP_RunMode_CONTINUOUS;
    clkParams.arg       = hMcm->timerSem;

    /* Creating timer and setting timer callback function*/
    hMcm->hTimer = ClockP_create(&EnetMcm_timerCb,
                                &clkParams);
    if (hMcm->hTimer != NULL)
    {
        ClockP_start(hMcm->hTimer);
        hMcm->timerTaskShutDownFlag = BFALSE;
    }
    else
    {
        hMcm->print("EnetMcm_createClock() failed to create clock\n");
    }

    /* Initialize the taskperiodicTick params. Set the task priority higher than the
     * default priority (1) */
    TaskP_Params_init(&params);
    params.priority       = ENETMCM_PERIODICTSK_PRIORITY;
    params.stack          = &hMcm->mcmPrdTaskStack[0];
    params.stacksize      = sizeof(hMcm->mcmPrdTaskStack);
    params.arg0           = hMcm->timerSem;
    params.arg1           = hMcm;
    params.name           = (const char *)"Enet_PeriodicTickTask";

    hMcm->task_periodicTick = TaskP_create(&EnetMcm_periodicTick, &params);
    EnetAppUtils_assert(hMcm->task_periodicTick != NULL);
}

static int32_t EnetMcm_open(EnetMcm_Handle hMcm)
{
    int32_t status = ENET_SOK;
    EnetUtils_Cfg utilsPrms;
    void *perCfg = NULL;
    EnetDma_Cfg *udmaCfg = &hMcm->dmaCfg;
    uint32_t cfgSize = 0U;

    perCfg = &hMcm->cpswCfg;
    hMcm->cpswCfg.dmaCfg = &hMcm->dmaCfg;
    cfgSize = sizeof(hMcm->cpswCfg);

    if (ENET_CPSW_2G == hMcm->enetType)
    {
        hMcm->print("EnetMcm: CPSW_2G on MCU NAVSS\n");
    }
    else if (ENET_CPSW_9G == hMcm->enetType)
    {
        hMcm->print("EnetMcm: CPSW_9G on MAIN NAVSS\n");
    }
    else if (ENET_CPSW_5G == hMcm->enetType)
    {
        hMcm->print("EnetMcm: CPSW_5G on MAIN NAVSS\n");
    }
    else
    {
        hMcm->print("EnetMcm: Invalid Enet Type\n");
    }

    /* Initialize CPSW driver with default utils */
    utilsPrms.print     = hMcm->print;
    utilsPrms.physToVirt = &EnetAppUtils_phyToVirtFxn;
    utilsPrms.virtToPhys = &EnetAppUtils_virtToPhyFxn;
    utilsPrms.traceTsFunc = hMcm->traceTsFunc;
    utilsPrms.extTraceFunc = hMcm->extTraceFunc;

    Enet_init(&utilsPrms);

    status = EnetMem_init();
    EnetAppUtils_assert(ENET_SOK == status);

    hMcm->hUdmaDrv = udmaCfg->hUdmaDrv;

    hMcm->hEnet = Enet_open(hMcm->enetType, hMcm->instId, perCfg, cfgSize);
    if(hMcm->hEnet == NULL)
    {
        EnetAppUtils_print("Enet_open failed\n");
        EnetAppUtils_assert(hMcm->hEnet != NULL);
    }

    /* Enable Host and MAC Ports */
    status = EnetMcm_enablePorts(hMcm);
    if(status != ENET_SOK)
    {
        EnetAppUtils_print("EnetMcm_enablePorts failed\n");
        EnetAppUtils_assert(ENET_SOK == status);
    }

    EnetMcm_createClock(hMcm);

    return status;
}

static void EnetMcm_coreAttachHandler(EnetMcm_Handle hMcm,
                                      uint32_t coreId,
                                      EnetPer_AttachCoreOutArgs *pAttachCoreOutArgs)
{
    Enet_IoctlPrms prms;
    EnetMcm_CoreAttachTableEntry *entry;
    int32_t status;

    if (NULL != pAttachCoreOutArgs)
    {
        entry = EnetMcm_getCoreAttachEntry(&hMcm->coreAttachTable, coreId);
        if (entry == NULL)
        {
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &coreId, pAttachCoreOutArgs);
            status = Enet_ioctl(hMcm->hEnet,
                                coreId,
                                ENET_PER_IOCTL_ATTACH_CORE,
                                &prms);
            if (status != ENET_SOK)
            {
                hMcm->print("EnetMcm_open failed ENET_PER_IOCTL_ATTACH_CORE: %d\n", status);
                EnetAppUtils_assert(BFALSE);
            }

            EnetMcm_addCoreAttachEntry(&hMcm->coreAttachTable, coreId, pAttachCoreOutArgs);
            entry = EnetMcm_getCoreAttachEntry(&hMcm->coreAttachTable, coreId);
        }

        EnetAppUtils_assert((entry != NULL));
        if (entry != NULL)
        {
            *pAttachCoreOutArgs = entry->coreInfo;
        }
    }
    else
    {
        EnetAppUtils_assert(BFALSE);
    }
}

static void EnetMcm_coreDetachHandler(EnetMcm_Handle hMcm,
                                      uint32_t coreId,
                                      uint32_t coreKey)
{
    Enet_IoctlPrms prms;
    EnetMcm_CoreAttachTableEntry *entry;

    entry = EnetMcm_getCoreAttachEntry(&hMcm->coreAttachTable, coreId);

    if (entry != NULL)
    {
        int32_t status;

        ENET_IOCTL_SET_IN_ARGS(&prms, &coreKey);
        status = Enet_ioctl(hMcm->hEnet,
                            coreId,
                            ENET_PER_IOCTL_DETACH_CORE,
                            &prms);
        if (status != ENET_SOK)
        {
            hMcm->print("close() failed ENET_PER_IOCTL_DETACH_CORE: %d\n", status);
            EnetAppUtils_assert(BFALSE);
        }

        EnetMcm_delCoreAttachEntry(&hMcm->coreAttachTable, coreId, coreKey);
    }
}

static int32_t EnetMcm_ioctlHandler(EnetMcm_Handle hMcm,
                                    uint32_t cmd,
                                    Enet_IoctlPrms *prms)
{
    int32_t status;

    if (prms != NULL)
    {
        status = Enet_ioctl(hMcm->hEnet, hMcm->selfCoreId, cmd, prms);
        if (status != ENET_SOK)
        {
            hMcm->print("EnetMcm_ioctlHandler failed: %d\n", status);
        }
    }
    else
    {
        status = ENET_EBADARGS;
        EnetAppUtils_assert(BFALSE);
    }

    return status;
}

static int32_t EnetMcm_disablePorts(EnetMcm_Handle hMcm)
{
    Enet_IoctlPrms prms;
    Enet_MacPort macPort;
    uint32_t i;
    int32_t status = ENET_SOK;

    /* Close all Ports*/
    for (i = 0U; i < hMcm->numMacPorts; i++)
    {
        macPort = hMcm->macPortList[i];

        ENET_IOCTL_SET_IN_ARGS(&prms, &macPort);
        status = Enet_ioctl(hMcm->hEnet, hMcm->selfCoreId, ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
    }
    return status;
}

static void EnetMcm_serverTask(void * McmHandle,
                               void * arg1)
{
    volatile bool isShutdownMcm = BFALSE;
    EnetMcm_Handle hMcm         = (EnetMcm_Handle)McmHandle;

    while (!isShutdownMcm)
    {
        int32_t status = ENET_SOK;
        EnetMcm_mailboxObj msg;
        EnetMcm_RespMboxObj *respMbox;
        uint32_t idx;

        MailboxP_pend(hMcm->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);

        respMbox = EnetMcm_getRespMbox(hMcm, msg.response);
        /* For MCM_SHUTDOWN the cmd is sent by internal MCM client at deInit time.
         * This client should have isValidClient set to BFALSE */
        EnetAppUtils_assert((respMbox->isValidClient == BTRUE) || (msg.cmd == MCM_SHUTDOWN));

        switch (msg.cmd)
        {
            case MCM_GET_HANDLE:
                if (hMcm->refCnt == 0)
                {
                    status = EnetMcm_open(hMcm);
                    EnetAppUtils_assert(ENET_SOK == status);
                }

                if (status == ENET_SOK)
                {
                    hMcm->refCnt++;
                }

                msg.cmd                         = MCM_RESPONSE_GET_HANDLE;
                msg.msgBody.handleInfo.hEnet    = hMcm->hEnet;
                msg.msgBody.handleInfo.hUdmaDrv = hMcm->hUdmaDrv;
                break;

            case MCM_RELEASE_HANDLE:
                EnetAppUtils_assert(hMcm->refCnt > 0);
                hMcm->refCnt--;

                if (hMcm->refCnt == 0)
                {
                    EnetMcm_close(hMcm);
                }

                msg.cmd = MCM_RESPONSE_RELEASE_HANDLE;
                break;

            case MCM_CORE_ATTACH:
                EnetAppUtils_assert(hMcm->hEnet != NULL);
                EnetMcm_coreAttachHandler(hMcm, msg.msgBody.coreId, &msg.msgBody.attachInfo);
                msg.cmd = MCM_RESPONSE_CORE_ATTACH;
                break;

            case MCM_CORE_DETACH:
                EnetAppUtils_assert(hMcm->hEnet != NULL);
                EnetMcm_coreDetachHandler(hMcm, msg.msgBody.coreId, msg.msgBody.coreKey);
                msg.cmd = MCM_RESPONSE_CORE_DETACH;
                break;

            case MCM_IOCTL:
                EnetAppUtils_assert(hMcm->hEnet != NULL);
                msg.msgBody.status = EnetMcm_ioctlHandler(hMcm, msg.msgBody.ioctlCmd, msg.msgBody.ioctlPrms);
                msg.cmd = MCM_RESPONSE_IOCTL;
                break;

            case MCM_SHUTDOWN:
                isShutdownMcm = BTRUE;
                msg.cmd       = MCM_RESPONSE_SHUTDOWN;
                EnetAppUtils_assert(ENETMCM_UTILS_ARRAYISMEMBER(respMbox, hMcm->respMbox) == BTRUE);
                idx = ENETMCM_UTILS_GETARRAYINDEX(respMbox, hMcm->respMbox);
                EnetAppUtils_assert(idx == ENETMCM_INTERNAL_CLIENT_IDX);
                break;

            case MCM_CLIENT_DISCONNECT:
                msg.cmd = MCM_RESPONSE_CLIENT_DISCONNECT;
                /* Clear client valid flag on receiving disconnect message */
                respMbox->isValidClient = BFALSE;
                break;

            case MCM_SAVE_CTXT:
                EnetAppUtils_assert(hMcm->hEnet != NULL);
                Enet_saveCtxt(hMcm->hEnet);
                msg.cmd = MCM_RESPONSE_SAVE_CTXT;
                break;

            case MCM_RESTORE_CTXT:
                EnetAppUtils_assert(hMcm->hEnet != NULL);
                msg.msgBody.status = Enet_restoreCtxt(hMcm->hEnet);
                EnetAppUtils_assert(msg.msgBody.status == ENET_SOK);
                msg.cmd = MCM_RESPONSE_RESTORE_CTXT;
                break;

            case MCM_OPEN_PORT:
                EnetAppUtils_assert(hMcm->hEnet != NULL);
                msg.msgBody.status = EnetMcm_enablePorts(hMcm);
                EnetAppUtils_assert(msg.msgBody.status == ENET_SOK);
                msg.cmd = MCM_RESPONSE_OPEN_PORT;
                break;

            case MCM_CLOSE_PORT:
                EnetAppUtils_assert(hMcm->hEnet != NULL);
                msg.msgBody.status = EnetMcm_disablePorts(hMcm);
                EnetAppUtils_assert(msg.msgBody.status == ENET_SOK);
                msg.cmd = MCM_RESPONSE_CLOSE_PORT;
                break;

            case MCM_STOP_CLOCK:
                EnetAppUtils_assert(hMcm->hEnet != NULL);
                ClockP_stop(hMcm->hTimer);
                msg.cmd = MCM_RESPONSE_STOP_CLOCK;
                break;

            case MCM_START_CLOCK:
                EnetAppUtils_assert(hMcm->hEnet != NULL);
                ClockP_start(hMcm->hTimer);
                msg.cmd = MCM_RESPONSE_START_CLOCK;
                break;

            default:
                /* Unahandle MCM command */
                EnetAppUtils_assert(BFALSE);
        }
        MailboxP_post(msg.response, &msg, SemaphoreP_WAIT_FOREVER);
    }
}

static void EnetMcm_close(EnetMcm_Handle hMcm)
{
    Enet_IoctlPrms prms;
    uint8_t i;
    int32_t status;

    /* Disable host port */
    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = Enet_ioctl(hMcm->hEnet,
                        hMcm->selfCoreId,
                        ENET_HOSTPORT_IOCTL_DISABLE,
                        &prms);
    if (status != ENET_SOK)
    {
        hMcm->print("Failed to disable host port: %d\n", status);
    }

    for (i = 0U; i < hMcm->numMacPorts; i++)
    {
        Enet_MacPort macPort = hMcm->macPortList[i];

        ENET_IOCTL_SET_IN_ARGS(&prms, &macPort);
        status = Enet_ioctl(hMcm->hEnet,
                            hMcm->selfCoreId,
                            ENET_PER_IOCTL_CLOSE_PORT_LINK,
                            &prms);
        if (status != ENET_SOK)
        {
            hMcm->print("close() failed to close MAC port: %d\n", status);
        }
    }

    hMcm->timerTaskShutDownFlag = BTRUE;

    if (hMcm->hTimer != NULL)
    {
        ClockP_stop(hMcm->hTimer);
        ClockP_delete(hMcm->hTimer);
    }

    /* Post Timer Sem once to get the Periodic Tick task terminated */
    SemaphoreP_post(hMcm->timerSem);

    while (TaskP_isTerminated(hMcm->task_periodicTick) != 1)
    {
        TaskP_sleep(10U);
    }
    TaskP_delete(&hMcm->task_periodicTick);

    if (hMcm->timerSem != NULL)
    {
        SemaphoreP_delete(hMcm->timerSem);
    }

    Enet_close(hMcm->hEnet);

    EnetMem_deInit();

    Enet_deinit();

    hMcm->hEnet = NULL;
}

void EnetMcm_acquireHandleInfo(const EnetMcm_CmdIf *hMcmCmdIf,
                               EnetMcm_HandleInfo *handleInfo)
{
    EnetMcm_mailboxObj msg;

    memset(&msg, 0, sizeof(msg));
    msg.cmd      = MCM_GET_HANDLE;

    if ((hMcmCmdIf != NULL) && (handleInfo != NULL))
    {
        msg.response = hMcmCmdIf->hMboxResponse;

        MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
        MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
        EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_GET_HANDLE);

        *handleInfo = msg.msgBody.handleInfo;
    }
    else
    {
        EnetAppUtils_assert(BFALSE);
    }
}

void EnetMcm_releaseHandleInfo(const EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_mailboxObj msg;

    EnetAppUtils_assert(hMcmCmdIf != NULL);

    memset(&msg, 0, sizeof(msg));
    msg.cmd = MCM_RELEASE_HANDLE;
    msg.response = hMcmCmdIf->hMboxResponse;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_RELEASE_HANDLE);
}

void EnetMcm_coreAttach(const EnetMcm_CmdIf *hMcmCmdIf,
                        uint32_t coreId,
                        EnetPer_AttachCoreOutArgs *attachInfo)
{
    EnetMcm_mailboxObj msg;

    memset(&msg, 0, sizeof(msg));
    if ((hMcmCmdIf != NULL) && (attachInfo != NULL))
    {
        msg.cmd            = MCM_CORE_ATTACH;
        msg.response       = hMcmCmdIf->hMboxResponse;
        msg.msgBody.coreId = coreId;

        MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
        MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
        EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_CORE_ATTACH);
        *attachInfo = msg.msgBody.attachInfo;
    }
    else
    {
        EnetAppUtils_assert(BFALSE);
    }
}

void     EnetMcm_coreDetach(const EnetMcm_CmdIf *hMcmCmdIf,
                            uint32_t coreId,
                            uint32_t coreKey)
{
    EnetMcm_mailboxObj msg;

    memset(&msg, 0, sizeof(msg));
    EnetAppUtils_assert(hMcmCmdIf != NULL);
    msg.cmd             = MCM_CORE_DETACH;
    msg.response        = hMcmCmdIf->hMboxResponse;
    msg.msgBody.coreId  = coreId;
    msg.msgBody.coreKey = coreKey;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_CORE_DETACH);
}

int32_t EnetMcm_ioctl(const EnetMcm_CmdIf *hMcmCmdIf,
                      uint32_t cmd,
                      Enet_IoctlPrms *prms)
{
    EnetMcm_mailboxObj msg;
    int32_t status;

    memset(&msg, 0, sizeof(msg));

    if ((hMcmCmdIf != NULL) && (prms != NULL))
    {
        msg.cmd               = MCM_IOCTL;
        msg.response          = hMcmCmdIf->hMboxResponse;
        msg.msgBody.ioctlCmd  = cmd;
        msg.msgBody.ioctlPrms = prms;

        MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
        MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
        EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_IOCTL);
        status = msg.msgBody.status;
    }
    else
    {
        status = ENET_EBADARGS;
        EnetAppUtils_assert(BFALSE);
    }

    return status;
}

static void EnetMcm_shutdown(Enet_Type enetType, EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_mailboxObj msg;

    msg.cmd = MCM_SHUTDOWN;
    msg.response = hMcmCmdIf->hMboxResponse;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_SHUTDOWN);
}

static EnetMcm_RespMboxObj* EnetMcm_getRespMbox(EnetMcm_Handle hMcm, MailboxP_Handle hMboxResp)
{
    EnetMcm_RespMboxObj *respMbox = NULL;
    uint32_t i;

    for (i = 0U; i < ENET_ARRAYSIZE(hMcm->respMbox); i++)
    {
        if(hMboxResp == hMcm->respMbox[i].hMbox)
        {
            respMbox = &hMcm->respMbox[i];
            break;
        }
    }
    EnetAppUtils_assert(respMbox != NULL);

    return respMbox;
}

void EnetMcm_saveCtxt(const EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_mailboxObj msg;

    EnetAppUtils_assert(hMcmCmdIf != NULL);

    memset(&msg, 0, sizeof(msg));
    msg.cmd = MCM_SAVE_CTXT;
    msg.response = hMcmCmdIf->hMboxResponse;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_SAVE_CTXT);
}

int32_t EnetMcm_restoreCtxt(const EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_mailboxObj msg;

    EnetAppUtils_assert(hMcmCmdIf != NULL);

    memset(&msg, 0, sizeof(msg));
    msg.cmd = MCM_RESTORE_CTXT;
    msg.response = hMcmCmdIf->hMboxResponse;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_RESTORE_CTXT);

    return msg.msgBody.status;
}

int32_t EnetMcm_openMacPorts(const EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_mailboxObj msg;

    EnetAppUtils_assert(hMcmCmdIf != NULL);

    memset(&msg, 0, sizeof(msg));
    msg.cmd = MCM_OPEN_PORT;
    msg.response = hMcmCmdIf->hMboxResponse;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_OPEN_PORT);

    return msg.msgBody.status;
}

int32_t EnetMcm_closeMacPorts(const EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_mailboxObj msg;

    EnetAppUtils_assert(hMcmCmdIf != NULL);

    memset(&msg, 0, sizeof(msg));
    msg.cmd = MCM_CLOSE_PORT;
    msg.response = hMcmCmdIf->hMboxResponse;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_CLOSE_PORT);

    return msg.msgBody.status;
}

void EnetMcm_stopPeriodicTick(const EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_mailboxObj msg;

    EnetAppUtils_assert(hMcmCmdIf != NULL);

    memset(&msg, 0, sizeof(msg));
    msg.cmd = MCM_STOP_CLOCK;
    msg.response = hMcmCmdIf->hMboxResponse;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_STOP_CLOCK);
}

void EnetMcm_startPeriodicTick(const EnetMcm_CmdIf *hMcmCmdIf)
{
    EnetMcm_mailboxObj msg;

    EnetAppUtils_assert(hMcmCmdIf != NULL);

    memset(&msg, 0, sizeof(msg));
    msg.cmd = MCM_START_CLOCK;
    msg.response = hMcmCmdIf->hMboxResponse;

    MailboxP_post(hMcmCmdIf->hMboxCmd, &msg, SemaphoreP_WAIT_FOREVER);
    MailboxP_pend(hMcmCmdIf->hMboxResponse, &msg, SemaphoreP_WAIT_FOREVER);
    EnetAppUtils_assert(msg.cmd == MCM_RESPONSE_START_CLOCK);
}


