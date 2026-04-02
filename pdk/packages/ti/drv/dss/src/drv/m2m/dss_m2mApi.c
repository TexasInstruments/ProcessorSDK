/*
 *  Copyright (c) Texas Instruments Incorporated 2018 - 2022
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
 *  \file dss_dispApi.c
 *
 *  \brief File containing the DSS display driver implementation.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/dss/dss.h>
#include <ti/drv/dss/src/drv/common/dss_evtMgr.h>
#include <ti/drv/dss/src/drv/disp/dss_dispDrv.h>
#include <ti/drv/dss/src/drv/disp/dss_dispPriv.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlDrv.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlPriv.h>
#include <ti/drv/dss/src/drv/m2m/dss_m2mDrv.h>
#include <dss_soc_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 *  Below ifdef __cplusplus is added so that C++ build passes without
 *  typecasting. This is because the prototype is build as C type
 *  whereas this file is build as CPP file. Hence we get C++ build error.
 *  Also if typecasting is used, then we get MisraC error Rule 11.1.
 */
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
DssM2MDrv_CommonObj gDssM2mCommonObj;

Dss_EvtMgrClientInfo gDss_M2MEvtMgrClientInfo[DSS_DISP_EVT_MGR_MAX_CLIENTS];
/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */
static int32_t Dss_m2mDrvCreateInstQueues(DssM2MDrv_VirtContext *context);

static int32_t Dss_m2mDrvDeleteInstQueues(DssM2MDrv_VirtContext *context);

/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
static uint32_t Dss_m2mDrvWbPipeCfgChk(const Dss_WbPipeCfgParams *instCfg,
                                       const Dss_WbPipeCfgParams *progCfg);

/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
static uint32_t Dss_m2mDrvWbDmaCfgChk(const CSL_DssWbPipeDmaCfg *instCfg,
                                      const CSL_DssWbPipeDmaCfg *progCfg);

/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
static uint32_t Dss_m2mDrvWbMFlagCfgChk(const Dss_WbPipeMflagParams *instCfg,
                                        const Dss_WbPipeMflagParams *progCfg);

/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
static uint32_t Dss_m2mDrvWbCscCfgChk(const CSL_DssCscCoeff *instCfg,
                                      const CSL_DssCscCoeff *progCfg);

static int32_t Dss_m2mDrvPrgramWbPipe(DssM2MDrv_VirtContext *context);
/*
 * Display IOCTLs
 */
static int32_t Dss_m2mDrvIoctlStart(DssM2MDrv_VirtContext *context);

static int32_t Dss_m2mDrvIoctlStop(DssM2MDrv_VirtContext *context);

static int32_t Dss_m2mDrvIoctlSetWbPipeParams(DssM2MDrv_VirtContext *context,
                                       const Dss_WbPipeCfgParams *pipeCfg);

static int32_t Dss_m2mDrvIoctlSetmFlagParams(DssM2MDrv_VirtContext *context,
                                      const Dss_WbPipeMflagParams *mFlagParams);

static int32_t Dss_m2mDrvIoctlGetCurrentStatus(DssM2MDrv_VirtContext *context,
                                               Dss_WbStatus *status);

static int32_t Dss_m2mDrvIoctlSetWbPipeCsc(DssM2MDrv_VirtContext *context,
                                    const CSL_DssCscCoeff *csc);

static int32_t Dss_m2mDrvIoctlSetDmaCfg(DssM2MDrv_VirtContext *context,
                                        const CSL_DssWbPipeDmaCfg *dmaCfg);

static void Dss_wbPipeCbFxn(const uint32_t *event,
                            uint32_t numEvents,
                            void *arg);

static void Dss_wbPipeDmaCompletionCbFxn(DssM2MDrv_InstObj *instObj);

static int32_t Dss_m2mValidateM2mPipeParams(const Dss_WbPipeCfgParams *wbPipeParams);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/* FVID2 independent functions: Init and deinit*/
int32_t Dss_m2mDrvInit(const Dss_InitParams *initParams)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_CommonObj *m2mObj;
    uint32_t loopCnt = 0U, contextCnt = 0U, eventGroup, pipeIdx = 0U;
    uint32_t numEvents, evtMgrId, numHandle;
    DssM2MDrv_VirtContext *virtContext;
    DssM2MDrv_InstObj *instObj;
    SemaphoreP_Params semParams;
    const Dss_SocInfo *socInfo;
    Dss_EvtMgrClientInfo *pClientInfo;
    uint32_t enabledEvents[DSS_EVT_MGR_MAX_CLIENT_EVENTS];

    m2mObj = &gDssM2mCommonObj;
    GT_assert(DssTrace, (NULL != m2mObj));

    /* Reset full driver object to '0' and set only required parameters */
    Fvid2Utils_memset(m2mObj, 0U, sizeof(DssM2MDrv_CommonObj));
    socInfo = Dss_getSocInfo();
    Fvid2Utils_memcpy(&m2mObj->socInfo,
                      socInfo,
                      sizeof(Dss_SocInfo));
    for(loopCnt = 0U ; loopCnt < DSS_DISP_EVT_MGR_MAX_CLIENTS ; loopCnt++)
    {
        pClientInfo = &gDss_M2MEvtMgrClientInfo[loopCnt];
        Fvid2Utils_memset(pClientInfo, 0U, sizeof (Dss_EvtMgrClientInfo));
    }

    m2mObj->getTimeStamp = NULL;
    m2mObj->isRegistered = UFALSE;

    /* Allocate instance semaphore */
    SemaphoreP_Params_init(&semParams);
    semParams.mode           = SemaphoreP_Mode_BINARY;
    m2mObj->lockSem = SemaphoreP_create(1U, &semParams);
    if (NULL == m2mObj->lockSem)
    {
        GT_0trace(DssTrace, GT_ERR, "Instance semaphore create failed!!\r\n");
        retVal = FVID2_EALLOC;
    }

    for (loopCnt = 0U ; loopCnt < DSSM2M_NUM_WB_PIPELINE ; loopCnt++)
    {
        m2mObj->numVirtContUsed[loopCnt] = 0U;
        instObj                          = &m2mObj->instObj[loopCnt];
        instObj->drvInstId               = loopCnt;
        instObj->inUse                   = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
        instObj->commonObjRef            = m2mObj;
        instObj->wbRegs                  = socInfo->wbRegs[loopCnt];
        instObj->commRegs                = NULL;
        instObj->numPipe                 = 0U;
        for (pipeIdx = 0U ;
             pipeIdx < DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE ;
             pipeIdx++)
        {
            instObj->pipeRegs[pipeIdx] = NULL;
            instObj->pipeId[pipeIdx]   = CSL_DSS_VID_PIPE_ID_MAX;
        }
        instObj->ovrRegs                 = NULL;
        instObj->overlayId               = CSL_DSS_OVERLAY_ID_MAX;
        instObj->pathCfgDone             = UFALSE;
        for (contextCnt = 0U ;
             contextCnt < DSSM2M_NUM_VIRTUAL_CONTEXT ;
             contextCnt++)
        {
            virtContext            = &m2mObj->virtContext[loopCnt][contextCnt];
            for (pipeIdx = 0U ;
                 pipeIdx < DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE ;
                 pipeIdx++)
            {
                virtContext->pipeId[pipeIdx] = CSL_DSS_VID_PIPE_ID_MAX;
            }
            virtContext->numPipe   = 0U;
            virtContext->overlayId = CSL_DSS_OVERLAY_ID_MAX;
            virtContext->contextId = contextCnt;
            virtContext->state     = DSSM2M_DRV_STATE_IDLE;
            virtContext->inUse     = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
            virtContext->instObj   = instObj;
        }
        /* Register WB Pipeline Events */
        Dss_convModuletoEventGroup(&eventGroup,
                                   instObj->drvInstId,
                                   DSS_EVENT_GROUP_TYPE_WB_PIPE);
        GT_assert(DssTrace, (DSS_EVENT_GROUP_INVALID != eventGroup));

        Dss_getEnabledWbPipeFuncEvents(&enabledEvents[0U], &numEvents);
        evtMgrId = Dss_getEvtMgrFuncIntrId();
        numHandle = instObj->numRegEvtHandle;
        instObj->evtGroupHandle[instObj->numRegEvtHandle] =
                    Dss_evtMgrRegister(
                        evtMgrId,
                        eventGroup,
                        (const uint32_t *)&enabledEvents[0U],
                        numEvents,
                        Dss_wbPipeCbFxn,
                        (void *)&gDss_M2MEvtMgrClientInfo[numHandle]);
        /* Enable Events */
        retVal = Dss_evtMgrEnable(instObj->evtGroupHandle[instObj->numRegEvtHandle]);
        instObj->numRegEvtHandle++;
        if (FVID2_SOK != retVal)
        {
            GT_0trace(DssTrace, GT_ERR,
                "Registering/Enabling to DSS Event manager failed\r\n");
        }
    }

    if (FVID2_SOK == retVal)
    {
        /* Initialize Driver operations */
        Fvid2DrvOps_init(&m2mObj->fvidDrvOps);
        m2mObj->fvidDrvOps.drvId                  = DSS_M2M_DRV_ID;
        m2mObj->fvidDrvOps.createFxn              = &Dss_m2mCreate;
        m2mObj->fvidDrvOps.deleteFxn              = &Dss_m2mDelete;
        m2mObj->fvidDrvOps.controlFxn             = &Dss_m2mControl;
        m2mObj->fvidDrvOps.processRequestFxn      = &Dss_m2mProcessRequest;
        m2mObj->fvidDrvOps.getProcessedRequestFxn = &Dss_m2mGetProcessedRequest;

        retVal = Fvid2_registerDriver(&m2mObj->fvidDrvOps);
        if (FVID2_SOK != retVal)
        {
            GT_0trace(DssTrace, GT_ERR,
                "Registering to FVID2 driver manager failed\r\n");
            /* Un-initialize the internal objects if error occurs: deInit() */
        }
        else
        {
            /* Init successful */
            m2mObj->isRegistered = UTRUE;
        }
    }


    return (retVal);
}

int32_t Dss_m2mDrvDeInit(void)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_CommonObj *m2mObj;
    uint32_t loopCnt = 0U, contextCnt = 0U, eventIdx = 0U, pipeIdx = 0U;
    DssM2MDrv_VirtContext *virtContext;
    DssM2MDrv_InstObj *instObj;

    m2mObj = &gDssM2mCommonObj;
    GT_assert(DssTrace, (NULL != m2mObj));

    if (NULL != m2mObj->lockSem)
    {
        /* Delete semaphore */
        (void)SemaphoreP_delete(m2mObj->lockSem);
    }
    for (loopCnt = 0U ; loopCnt < DSSM2M_NUM_WB_PIPELINE ; loopCnt++)
    {
        instObj                          = &m2mObj->instObj[loopCnt];
        instObj->inUse                   = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
        instObj->commonObjRef            = NULL;
        instObj->commRegs                = NULL;
        for (pipeIdx = 0U ;
             pipeIdx < DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE ;
             pipeIdx++)
        {
            instObj->pipeRegs[pipeIdx] = NULL;
            instObj->pipeId[pipeIdx]   = CSL_DSS_VID_PIPE_ID_MAX;
        }
        instObj->numPipe                 = 0U;
        instObj->ovrRegs                 = NULL;
        instObj->wbRegs                  = NULL;
        instObj->pathCfgDone             = UFALSE;
        for (contextCnt = 0U ;
             contextCnt < DSSM2M_NUM_VIRTUAL_CONTEXT ;
             contextCnt++)
        {
            virtContext            = &m2mObj->virtContext[loopCnt][contextCnt];
            virtContext->numPipe   = 0U;
            virtContext->state     = DSSM2M_DRV_STATE_UNINIT;
            virtContext->inUse     = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
            virtContext->instObj   = NULL;
        }
        /* Unregister event groups and delete object */
        for(eventIdx = 0U ; eventIdx < instObj->numRegEvtHandle ; eventIdx++)
        {
            retVal += Dss_evtMgrUnRegister(instObj->evtGroupHandle[eventIdx]);
            instObj->numRegEvtHandle--;
        }
    }

    /* unregister driver */
    retVal = Fvid2_unRegisterDriver(&m2mObj->fvidDrvOps);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(DssTrace, GT_ERR,
            "Unregistering from FVID2 driver manager failed\r\n");
    }

    return (retVal);
}

/* FVID2 Functions */
Fdrv_Handle Dss_m2mCreate(uint32_t drvId,
                          uint32_t instId,
                          void *createArgs,
                          void *createStatusArgs,
                          const Fvid2_DrvCbParams *fdmCbParams)
{
    Fdrv_Handle drvHandle = NULL;
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_CommonObj *m2mObj;
    Dss_WbCreateParams *createParams;
    Dss_WbCreateStatus *createStatus;
    DssM2MDrv_VirtContext *virtContext;
    DssM2MDrv_InstObj *instObj;
    uint32_t loopCnt = 0U;

    m2mObj = &gDssM2mCommonObj;
    GT_assert(DssTrace, (NULL != m2mObj));

    /* Check for NULL pointers and invalid arguments */
    if ((NULL == createStatusArgs) ||
        (NULL == fdmCbParams) ||
        (DSS_M2M_DRV_ID != drvId) ||
        (DSS_M2M_WB_PIPELINE_ID_MAX <= instId))
    {
        GT_0trace(DssTrace, GT_ERR, "Invalid arguments\r\n");
        retVal = FVID2_EBADARGS;
    }
    else
    {
        createParams = (Dss_WbCreateParams *)createArgs;
        createStatus = (Dss_WbCreateStatus *)createStatusArgs;
        /* Get free Virtual Context */
        for (loopCnt = 0U ; loopCnt < DSSM2M_NUM_VIRTUAL_CONTEXT ; loopCnt++)
        {
            if (m2mObj->virtContext[instId][loopCnt].inUse ==
                                DSSM2M_DRV_USAGE_STATUS_NOT_USED)
            {
                break;
            }
        }
        if (DSSM2M_NUM_VIRTUAL_CONTEXT <= loopCnt)
        {
            retVal = FVID2_EALLOC;
            GT_0trace(DssTrace, GT_ERR, "No more instances to allocate...\r\n");
        }
        else
        {
            virtContext = &m2mObj->virtContext[instId][loopCnt];
            instObj = virtContext->instObj;
            GT_assert(DssTrace, (NULL != instObj->commonObjRef));

            if(NULL != instObj->commonObjRef->lockSem)
            {
                /* Take the instance semaphore */
                (void) SemaphoreP_pend(instObj->commonObjRef->lockSem,
                                       SemaphoreP_WAIT_FOREVER);
            }

            for (loopCnt = 0U ;
                 loopCnt < createParams->numPipe ;
                 loopCnt++)
            {
                if (((CSL_DSS_VID_PIPE_ID_MAX != instObj->pipeId[loopCnt]) &&
                    (instObj->pipeId[loopCnt] != createParams->pipeId[loopCnt])) ||
                    (CSL_DSS_VID_PIPE_ID_MAX  == createParams->pipeId[loopCnt]))
                {
                    break;
                }
            }
            if (loopCnt < createParams->numPipe)
            {
                retVal = FVID2_EBADARGS;
                GT_0trace(DssTrace, GT_ERR, "Wrong Pipe instance used!!!\r\n");
            }

            /* Check if same DSS Pipe and Overlay is provided within
               different Create calls */
            if (((CSL_DSS_OVERLAY_ID_MAX != instObj->overlayId) &&
                 (instObj->overlayId     != createParams->overlayId)) ||
                 (CSL_DSS_OVERLAY_ID_MAX == createParams->overlayId))
            {
                retVal = FVID2_EBADARGS;
                GT_0trace(DssTrace, GT_ERR, "Wrong Overlay instance used!!!\r\n");
            }

            if (FVID2_SOK == retVal)
            {
                /* Update app provided configurations in the instance object */
                /* Copy create and Call-back parameters */
                Fvid2Utils_memcpy(&virtContext->createParams,
                                  createParams,
                                  sizeof(Dss_WbCreateParams));
                Fvid2Utils_memcpy(&virtContext->fdmCbParams,
                                  fdmCbParams,
                                  sizeof(Fvid2_DrvCbParams));
                /* Set status to '0' */
                Fvid2Utils_memset(&virtContext->wbStatus, 0U, sizeof(Dss_WbStatus));
                /* Clear instance configurations,
                   these will be later set by IOCTLs */
                Fvid2Utils_memset(&virtContext->instCfg,
                                  0U,
                                  sizeof(DssM2MDrv_moduleCfg));
                for (loopCnt = 0U ;
                     loopCnt < createParams->numPipe ;
                     loopCnt++)
                {
                    virtContext->pipeId[loopCnt] = createParams->pipeId[loopCnt];
                }
                virtContext->numPipe   = createParams->numPipe;
                virtContext->overlayId = createParams->overlayId;
            }

            if (FVID2_SOK == retVal)
            {
                virtContext->doneQ = NULL;
                retVal = Fvid2Utils_constructQ(&virtContext->doneLlObj);
                GT_assert(DssTrace, (FVID2_SOK == retVal));
                virtContext->doneQ = &virtContext->doneLlObj;
            }

            if (FVID2_SOK == retVal)
            {
                if ((0U == m2mObj->numVirtContUsed[instId]) &&
                    (DSSM2M_DRV_USAGE_STATUS_NOT_USED == instObj->inUse))
                {
                    for (loopCnt = 0U ;
                         loopCnt < createParams->numPipe ;
                         loopCnt++)
                    {
                        instObj->pipeId[loopCnt] = virtContext->pipeId[loopCnt];
                    }
                    instObj->overlayId = virtContext->overlayId;
                    instObj->numPipe   = virtContext->numPipe;
                    /* Set instance status */
                    Fvid2Utils_memset(&instObj->status,
                                      0U,
                                      sizeof(Dss_WbStatus));
                    /* Create Queues for Buffer Management - Do this for
                       only first create call*/
                    retVal = Dss_m2mDrvCreateInstQueues(virtContext);
                }

                if (FVID2_SOK == retVal)
                {
                    /* Set states of driver object and instance object */
                    virtContext->numBufQed = 0U;
                    instObj->commRegs      =
                        instObj->commonObjRef->socInfo.commRegs[CSL_DSS_COMM_REG_ID_0];
                    instObj->ovrRegs       =
                        instObj->commonObjRef->socInfo.overlayRegs[createParams->overlayId];
                    for (loopCnt = 0U ;
                         loopCnt < DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE ;
                         loopCnt++)
                    {
                        instObj->pipeRegs[createParams->pipeId[loopCnt]] =
                            instObj->commonObjRef->socInfo.pipeRegs[createParams->pipeId[loopCnt]];
                    }
                    virtContext->inUse     = DSSM2M_DRV_USAGE_STATUS_IN_USE;
                    virtContext->state     = DSSM2M_DRV_STATE_CREATED;
                    instObj->inUse         = DSSM2M_DRV_USAGE_STATUS_IN_USE;
                    m2mObj->numVirtContUsed[instId]++;
                    drvHandle = (Fdrv_Handle)virtContext;
                }
            }
            /* Configure WB pipeline input connection */
            if (FVID2_SOK == retVal)
            {
                switch (createParams->overlayId)
                {
                    case CSL_DSS_OVERLAY_ID_1:
                        CSL_dssSetWbInputCh(instObj->commRegs,
                                            CSL_DSS_WB_INPUT_OVERLAY1);
                    break;
                    case CSL_DSS_OVERLAY_ID_2:
                        CSL_dssSetWbInputCh(instObj->commRegs,
                                            CSL_DSS_WB_INPUT_OVERLAY2);
                    break;
                    case CSL_DSS_OVERLAY_ID_3:
                        CSL_dssSetWbInputCh(instObj->commRegs,
                                            CSL_DSS_WB_INPUT_OVERLAY3);
                    break;
                    case CSL_DSS_OVERLAY_ID_4:
                        CSL_dssSetWbInputCh(instObj->commRegs,
                                            CSL_DSS_WB_INPUT_OVERLAY4);
                    break;
                    default:
                        GT_0trace(DssTrace,
                                  GT_ERR,
                                  "Wrong Overlay instance used!!!\r\n");
                    break;
                }
            }

            if(NULL != instObj->commonObjRef->lockSem)
            {
                /* Post the instance semaphore */
                (void) SemaphoreP_post(instObj->commonObjRef->lockSem);
            }
        }
        if (NULL != createStatusArgs)
        {
            createStatus->retVal = retVal;
        }
    }

    return (drvHandle);
}

int32_t Dss_m2mDelete(Fdrv_Handle handle, void *reserved)
{
    int32_t retVal = FVID2_SOK;
    uint32_t pipeIdx;
    DssM2MDrv_CommonObj *m2mObj;
    DssM2MDrv_VirtContext *virtContext;
    DssM2MDrv_InstObj *instObj;
    DssM2M_DrvQueObj *qObj;

    m2mObj = &gDssM2mCommonObj;
    GT_assert(DssTrace, (NULL != m2mObj));

    if (NULL == handle)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        virtContext = (DssM2MDrv_VirtContext *)handle;
        instObj = virtContext->instObj;

        /* Set states of driver object and instance object */
        virtContext->inUse = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
        virtContext->state = DSSM2M_DRV_STATE_IDLE;
        m2mObj->numVirtContUsed[virtContext->instObj->drvInstId]--;

        if(NULL != virtContext->doneQ)
        {
            /* Free-up all the queued free queue objects */
            do
            {
                qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(virtContext->doneQ);
                if (NULL != qObj)
                {
                    Fvid2Utils_queue(instObj->bmObj.freeQ, &qObj->qElem, qObj);
                }
            } while (NULL != qObj);

            /* Delete the free Q */
            Fvid2Utils_destructQ(virtContext->doneQ);
            virtContext->doneQ = NULL;
        }
        for (pipeIdx = 0U ;
             pipeIdx < DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE ;
             pipeIdx++)
        {
            virtContext->pipeId[pipeIdx] = CSL_DSS_VID_PIPE_ID_MAX;
        }
        virtContext->numPipe   = 0U;
        virtContext->overlayId = CSL_DSS_OVERLAY_ID_MAX;

        /* Delete Queues only on delete call for last opened Virtual context */
        if ((0U == m2mObj->numVirtContUsed[virtContext->instObj->drvInstId]) &&
                    (DSSM2M_DRV_USAGE_STATUS_IN_USE == instObj->inUse))
        {
            for (pipeIdx = 0U ;
                 pipeIdx < DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE ;
                 pipeIdx++)
            {
                instObj->pipeRegs[pipeIdx] = NULL;
                instObj->pipeId[pipeIdx]   = CSL_DSS_VID_PIPE_ID_MAX;
            }
            instObj->numPipe   = 0U;
            instObj->overlayId = CSL_DSS_OVERLAY_ID_MAX;
            instObj->commRegs  = NULL;
            instObj->ovrRegs   = NULL;
            if(NULL != instObj->commonObjRef->lockSem)
            {
                /* Take the instance semaphore */
                (void) SemaphoreP_pend(instObj->commonObjRef->lockSem,
                                       SemaphoreP_WAIT_FOREVER);
            }

            retVal = Dss_m2mDrvDeleteInstQueues(virtContext);

            if(NULL != instObj->commonObjRef->lockSem)
            {
                /* Post the instance semaphore */
                (void) SemaphoreP_post(instObj->commonObjRef->lockSem);
            }
            instObj->inUse = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
        }
    }

    return retVal;
}

int32_t Dss_m2mProcessRequest(Fdrv_Handle handle,
                              Fvid2_FrameList *inProcessList,
                              Fvid2_FrameList *outProcessList,
                              uint32_t timeout)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_VirtContext *virtContext = NULL;
    DssM2MDrv_InstObj *instObj = NULL;
    uint32_t semTimeout, frmCnt, cookie, loopCnt;
    DssM2M_DrvQueObj *qObj;
    DssM2M_DrvBufManObj *bmObj;

    /** Steps
     *  1. Get a queue object from the freeQ. Assert error if NULL.
     *  2. Copy handle into queue object
     *  3. Copy frame lists into queue object
     *  4. Move the queue object to reqQ
     *  5. Increment Queue Count in the status
     *  6. If number of elements in currQ < 1, then do following
     *     a. Get a queue object from the reqQ. Assert error if NULL
     *     b. Disable DSS and WB pipeline
     *     c. Re-program DSS pipeline, Overlay and WB Pipeline if needed
     *     d. Set buffer addresses in the WB & DSS Pipeline
     *     e. Enable DSS and WB pipeline
     *     f. Move the queue object to currQ
     */
    if ((NULL == handle) || (NULL == inProcessList) || (NULL == outProcessList))
    {
        retVal = FVID2_EBADARGS;
    }
    if (FVID2_SOK == retVal)
    {
        virtContext = (DssM2MDrv_VirtContext *)handle;
        instObj     = virtContext->instObj;
        GT_assert(DssTrace, (NULL != instObj));
        bmObj       = &instObj->bmObj;
        GT_assert(DssTrace, (NULL != bmObj));

        if ((DSSM2M_DRV_USAGE_STATUS_IN_USE != instObj->inUse)     ||
            (DSSM2M_DRV_USAGE_STATUS_IN_USE != virtContext->inUse) ||
            ((DSSM2M_DRV_STATE_IDLE   == virtContext->state) ||
             (DSSM2M_DRV_STATE_UNINIT == virtContext->state)))
        {
            retVal = FVID2_EFAIL;
            GT_0trace(DssTrace, GT_ERR,
                "Invalid state: Can't submit request to an un-opened instance!!\r\n");
        }
    }
    if (FVID2_TIMEOUT_FOREVER == timeout)
    {
        semTimeout = SemaphoreP_WAIT_FOREVER;
    }
    else if (FVID2_TIMEOUT_NONE == timeout)
    {
        semTimeout = SemaphoreP_NO_WAIT;
    }
    else
    {
        semTimeout = timeout;
    }
    if ((NULL != instObj) && (NULL != instObj->commonObjRef->lockSem))
    {
        /* Take the instance semaphore */
        (void) SemaphoreP_pend(instObj->commonObjRef->lockSem, semTimeout);
    }

    cookie = HwiP_disable();
    /* Steps 1 to 5 */
    if (FVID2_SOK == retVal)
    {
        /* Check frame list for error and NULL pointer check */
        retVal = Fvid2_checkFrameList(inProcessList, FVID2_MAX_FRAME_PTR);
        retVal += Fvid2_checkFrameList(outProcessList, FVID2_MAX_FRAME_PTR);

    }
    if ((FVID2_SOK == retVal) &&
        (inProcessList->numFrames != virtContext->createParams.numPipe) &&
        (1U != outProcessList->numFrames))
    {
        retVal = FVID2_EBADARGS;
        GT_0trace(DssTrace, GT_ERR,
            "Number of frames in input (must be equal to number of pipes used) OR "
            "output (must be equal to 1) frame list does not match !!\r\n");
    }
    if (FVID2_SOK == retVal)
    {
        qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->freeQ);
        if (NULL == qObj)
        {
            GT_0trace(DssTrace, GT_ERR,
                      "ALLOC: Q object allocation failed\r\n");
            retVal = FVID2_EALLOC;
        }
        if (FVID2_SOK == retVal)
        {
            qObj->virtContext = virtContext;
            qObj->outFrm      = outProcessList->frames[0U];
            for (frmCnt = 0U ; frmCnt < inProcessList->numFrames ; frmCnt++)
            {
                qObj->inFrm[frmCnt] = inProcessList->frames[frmCnt];
            }
            Fvid2Utils_queue(bmObj->reqQ, &qObj->qElem, qObj);
            virtContext->wbStatus.queueCount++;
            instObj->status.queueCount++;
        }
    }
    /* Step 6 */
    if (FVID2_SOK == retVal)
    {
        GT_assert(DssTrace, (NULL != instObj->commRegs));
        for (loopCnt = 0U ; loopCnt < virtContext->numPipe ; loopCnt++)
        {
            GT_assert(DssTrace,
                    (NULL != instObj->pipeRegs[virtContext->pipeId[loopCnt]]));
        }
        GT_assert(DssTrace, (NULL != instObj->ovrRegs));
        GT_assert(DssTrace, (NULL != instObj->wbRegs));

        if (0U == Fvid2Utils_getNumQElem(bmObj->currQ))
        {
            qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->reqQ);
            GT_assert(DssTrace, (NULL != qObj));
            for (loopCnt = 0U ; loopCnt < virtContext->numPipe ; loopCnt++)
            {
                CSL_dssVidPipeEnable(instObj->pipeRegs[virtContext->pipeId[loopCnt]],
                                     UFALSE);
            }
            CSL_dssWbPipeEnable(instObj->wbRegs, UFALSE);

            /* DCTRL has to be configurred before DISP */
            retVal += Dss_m2mDrvPrgramDctrl(qObj->virtContext);
            retVal += Dss_m2mDrvPrgramDisp(qObj->virtContext);
            retVal += Dss_m2mDrvPrgramWbPipe(qObj->virtContext);

            if (FVID2_SOK == retVal)
            {
                /* Queue to CurrQ */
                Fvid2Utils_queue(bmObj->currQ, &qObj->qElem, qObj);

                for (loopCnt = 0U ; loopCnt < virtContext->numPipe ; loopCnt++)
                {
                    CSL_dssVidPipeSetBuffAddr(
                            instObj->pipeRegs[qObj->inFrm[loopCnt]->chNum],
                            FVID2_FID_TOP,
                            qObj->inFrm[loopCnt]->addr[0U],
                            qObj->inFrm[loopCnt]->addr[1U]);
                }
                CSL_dssWbPipeSetBuffAddr(instObj->wbRegs,
                                         FVID2_FID_TOP,
                                         qObj->outFrm->addr[0U],
                                         qObj->outFrm->addr[1U]);

                for (loopCnt = 0U ; loopCnt < virtContext->numPipe ; loopCnt++)
                {
                    CSL_dssVidPipeEnable(instObj->pipeRegs[virtContext->pipeId[loopCnt]],
                                         UTRUE);
                }
                CSL_dssWbPipeSetGoBit(instObj->wbRegs);
                CSL_dssWbPipeEnable(instObj->wbRegs, UTRUE);
            }
            else
            {
                /* Queue back in case of failure */
                Fvid2Utils_queue(bmObj->reqQ, &qObj->qElem, qObj);
                GT_0trace(DssTrace, GT_ERR,
                          "Programming Failed: DSS Pipe/Disp/WB Pipe\r\n");
            }

        }
    }
    HwiP_restore(cookie);

    if ((NULL != instObj) && (NULL != instObj->commonObjRef->lockSem))
    {
        /* Post the instance semaphore */
        (void) SemaphoreP_post(instObj->commonObjRef->lockSem);
    }

    return retVal;
}

int32_t Dss_m2mGetProcessedRequest(Fdrv_Handle handle,
                                   Fvid2_FrameList *inProcessList,
                                   Fvid2_FrameList *outProcessList,
                                   uint32_t timeout)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_VirtContext *virtContext = NULL;
    DssM2MDrv_InstObj *instObj = NULL;
    uint32_t semTimeout, cookie, loopCnt;
    DssM2M_DrvQueObj *qObj;
    DssM2M_DrvBufManObj *bmObj;

    /** Steps
     *  1. Get a queue object from the doneQ. Assert error if NULL.
     *  2. Copy frames into inProcessList and outProcessList
     *  3. Set frames and context to NULL
     *  4. Move the queue object to freeQ
     *  5. Increment De-queue Count in the status
     */
    if ((NULL == handle) || (NULL == inProcessList) || (NULL == outProcessList))
    {
        retVal = FVID2_EBADARGS;
    }
    if (FVID2_SOK == retVal)
    {
        virtContext = (DssM2MDrv_VirtContext *)handle;
        instObj     = virtContext->instObj;
        GT_assert(DssTrace, (NULL != instObj));
        bmObj       = &instObj->bmObj;
        GT_assert(DssTrace, (NULL != bmObj));

        if ((DSSM2M_DRV_USAGE_STATUS_IN_USE != instObj->inUse)     ||
            (DSSM2M_DRV_USAGE_STATUS_IN_USE != virtContext->inUse) ||
            ((DSSM2M_DRV_STATE_IDLE   == virtContext->state) ||
             (DSSM2M_DRV_STATE_UNINIT == virtContext->state)))
        {
            retVal = FVID2_EFAIL;
            GT_0trace(DssTrace, GT_ERR,
                "Invalid state: Can't submit request to an un-opened instance!!\r\n");
        }
    }
    if (FVID2_TIMEOUT_FOREVER == timeout)
    {
        semTimeout = SemaphoreP_WAIT_FOREVER;
    }
    else if (FVID2_TIMEOUT_NONE == timeout)
    {
        semTimeout = SemaphoreP_NO_WAIT;
    }
    else
    {
        semTimeout = timeout;
    }
    if ((NULL != instObj) && (NULL != instObj->commonObjRef->lockSem))
    {
        /* Take the instance semaphore */
        (void) SemaphoreP_pend(instObj->commonObjRef->lockSem, semTimeout);
    }

    cookie = HwiP_disable();
    /* Steps 1 to 5 */
    if (FVID2_SOK == retVal)
    {
        /* init frame list fields */
        inProcessList->numFrames   = 0U;
        inProcessList->perListCfg  = NULL;
        outProcessList->numFrames  = 0U;
        outProcessList->perListCfg = NULL;
        /* Only De-queue one frame per call */
        qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(virtContext->doneQ);
        if (NULL != qObj)
        {
            for (loopCnt = 0U ; loopCnt < virtContext->numPipe ; loopCnt++)
            {
                inProcessList->frames[loopCnt] = qObj->inFrm[loopCnt];
                inProcessList->numFrames++;
                qObj->inFrm[loopCnt] = NULL;
            }

            outProcessList->frames[outProcessList->numFrames] = qObj->outFrm;
            outProcessList->numFrames++;
            qObj->virtContext = NULL;
            qObj->outFrm      = NULL;
            virtContext->wbStatus.dequeueCount++;
            instObj->status.dequeueCount++;
            Fvid2Utils_queue(bmObj->freeQ, &qObj->qElem, qObj);
        }
    }

    if ((FVID2_SOK == retVal) &&
        (0U == inProcessList->numFrames) &&
        (0U == outProcessList->numFrames))
    {
        if ((DSSM2M_DRV_STATE_CREATED == virtContext->state) ||
            (DSSM2M_DRV_STATE_STOPPED == virtContext->state))
        {
            retVal = FVID2_ENO_MORE_BUFFERS;
        }
    }
    HwiP_restore(cookie);

    if ((NULL != instObj) && (NULL != instObj->commonObjRef->lockSem))
    {
        /* Post the instance semaphore */
        (void) SemaphoreP_post(instObj->commonObjRef->lockSem);
    }

    return retVal;
}

int32_t Dss_m2mControl(Fdrv_Handle handle,
                       uint32_t cmd,
                       void *cmdArgs,
                       void *cmdStatusArgs)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_VirtContext *virtContext;
    Fvid2_TimeStampParams *tsParams;

    /* Check for NULL pointers */
    if (NULL == handle)
    {
        GT_0trace(DssTrace, GT_ERR, "Invalid argument!!\r\n");
        retVal = FVID2_EBADARGS;
    }
    else
    {
        virtContext = (DssM2MDrv_VirtContext *) handle;
        switch (cmd)
        {
            case FVID2_START:
                retVal = Dss_m2mDrvIoctlStart(virtContext);
            break;
            case FVID2_STOP:
                retVal = Dss_m2mDrvIoctlStop(virtContext);
            break;
            case IOCTL_DSS_M2M_SET_WB_PIPE_PARAMS:
                retVal = Dss_m2mDrvIoctlSetWbPipeParams(virtContext,
                                    (const Dss_WbPipeCfgParams *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_SET_WB_PIPE_MFLAG_PARAMS:
                retVal = Dss_m2mDrvIoctlSetmFlagParams(virtContext,
                                    (const Dss_WbPipeMflagParams *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_GET_CURRENT_STATUS:
                retVal = Dss_m2mDrvIoctlGetCurrentStatus(virtContext,
                                    (Dss_WbStatus *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_SET_WB_PIPE_CSC_COEFF:
                retVal = Dss_m2mDrvIoctlSetWbPipeCsc(virtContext,
                                    (const CSL_DssCscCoeff *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_SET_WB_PIPE_DMA_CFG:
                retVal = Dss_m2mDrvIoctlSetDmaCfg(virtContext,
                                    (const CSL_DssWbPipeDmaCfg *)cmdArgs);
            break;
            case FVID2_REGISTER_TIMESTAMP_FXN:
                tsParams = (Fvid2_TimeStampParams *)cmdArgs;
                gDssM2mCommonObj.getTimeStamp = tsParams->timeStampFxn;
            break;
            case IOCTL_DSS_M2M_SET_PIPE_PARAMS:
                retVal = Dss_m2mDrvIoctlSetDssPipeParams(virtContext,
                                    (const Dss_PipeCfgParams *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_SET_PIPE_MFLAG_PARAMS:
                retVal = Dss_m2mDrvIoctlSetPipeMflagParams(virtContext,
                                    (const Dss_PipeMflagParams *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_SET_PIPE_CSC_COEFF:
                retVal = Dss_m2mDrvIoctlSetPipeCsc(virtContext,
                                    (const Dss_PipeCscParams *)cmdArgs);
            break;
            case IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS:
                retVal = Dss_m2mDrvIoctlSetOverlayParams(virtContext,
                                    (const Dss_DctrlOverlayParams *)cmdArgs);
            break;
            case IOCTL_DSS_DCTRL_SET_LAYER_PARAMS:
                retVal = Dss_m2mDrvIoctlSetLayerParams(virtContext,
                                (const Dss_DctrlOverlayLayerParams *)cmdArgs);
            break;
            case IOCTL_DSS_DCTRL_SET_GLOBAL_DSS_PARAMS:
                retVal = Dss_m2mDrvIoctlSetGlobalParams(virtContext,
                                (const Dss_DctrlGlobalDssParams *)cmdArgs);
            break;
            default:
            {
                GT_0trace(DssTrace, GT_ERR,
                            "UNSUPPORTED_CMD: IOCTL not supported\r\n");
                retVal = FVID2_EUNSUPPORTED_CMD;
            }
            break;
        }
    }

    return retVal;
}
/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */
int32_t Dss_m2mDrvSetWbPipeDmaCfg(DssM2MDrv_InstObj *instObj,
                                  const CSL_DssWbPipeDmaCfg *dmaCfg)
{
    int32_t retVal = FVID2_SOK;

    if (NULL == instObj->wbRegs)
    {
        retVal = FVID2_EFAIL;
    }
    else
    {
        /* Configure HW module */
        CSL_dssWbPipeSetDmaConfig(instObj->wbRegs, dmaCfg);
        /* Update programmed configurations in the instance object */
        Fvid2Utils_memcpy(&instObj->progCfg.wbPipeCfg.dmaCfg,
                          dmaCfg,
                          sizeof(CSL_DssWbPipeDmaCfg));
    }

    return retVal;
}

static int32_t Dss_m2mDrvCreateInstQueues(DssM2MDrv_VirtContext *context)
{
    int32_t retVal = FVID2_SOK;
    DssM2M_DrvBufManObj *bmObj;
    uint32_t qCnt = 0U, loopCnt;
    DssM2M_DrvQueObj *qObj;
    DssM2MDrv_InstObj *instObj;

    instObj = context->instObj;
    bmObj = &instObj->bmObj;

    bmObj->freeQ = NULL;
    bmObj->reqQ = NULL;
    bmObj->currQ = NULL;

    /* Create Queues */
    retVal = Fvid2Utils_constructQ(&bmObj->freeLlObj);
    GT_assert(DssTrace, (FVID2_SOK == retVal));
    bmObj->freeQ = &bmObj->freeLlObj;

    retVal = Fvid2Utils_constructQ(&bmObj->reqLlObj);
    GT_assert(DssTrace, (FVID2_SOK == retVal));
    bmObj->reqQ = &bmObj->reqLlObj;

    retVal = Fvid2Utils_constructQ(&bmObj->curLlObj);
    GT_assert(DssTrace, (FVID2_SOK == retVal));
    bmObj->currQ = &bmObj->curLlObj;

    if(FVID2_SOK == retVal)
    {
        for (qCnt = 0U ;
             qCnt < (DSS_DEF_QUEUE_LEN_PER_M2M_DRV_INST * DSSM2M_NUM_VIRTUAL_CONTEXT) ;
             qCnt++)
        {
            qObj              = &bmObj->m2mQObj[qCnt];
            qObj->virtContext = context;
            for (loopCnt = 0U ; loopCnt < context->numPipe ; loopCnt++)
            {
                qObj->inFrm[loopCnt] = NULL;
            }
            qObj->outFrm      = NULL;
            Fvid2Utils_queue(bmObj->freeQ, &qObj->qElem, qObj);
        }
    }

    return retVal;
}

static int32_t Dss_m2mDrvDeleteInstQueues(DssM2MDrv_VirtContext *context)
{
    int32_t retVal = FVID2_SOK;
    DssM2M_DrvBufManObj *bmObj;
    DssM2M_DrvQueObj *qObj;
    DssM2MDrv_InstObj *instObj;

    instObj = context->instObj;
    bmObj = &instObj->bmObj;

    if(NULL != bmObj->freeQ)
    {
        /* Free-up all the queued free queue objects */
        do
        {
            qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->freeQ);
        } while (NULL != qObj);

        /* Delete the free Q */
        Fvid2Utils_destructQ(bmObj->freeQ);
        bmObj->freeQ = NULL;
    }

    if(NULL != bmObj->reqQ)
    {
        /* Free-up all the queued free queue objects */
        do
        {
            qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->reqQ);
        } while (NULL != qObj);

        /* Delete the free Q */
        Fvid2Utils_destructQ(bmObj->reqQ);
        bmObj->reqQ = NULL;
    }

    if(NULL != bmObj->currQ)
    {
        /* Free-up all the queued free queue objects */
        do
        {
            qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->currQ);
        } while (NULL != qObj);

        /* Delete the free Q */
        Fvid2Utils_destructQ(bmObj->currQ);
        bmObj->currQ = NULL;
    }

    return retVal;
}

static uint32_t Dss_m2mDrvWbPipeCfgChk(const Dss_WbPipeCfgParams *instCfg,
                                       const Dss_WbPipeCfgParams *progCfg)
{
    uint32_t retVal = UTRUE;

    if ((instCfg->pipeCfg.wbMode             != progCfg->pipeCfg.wbMode)             ||
        (instCfg->pipeCfg.yuvAlign           != progCfg->pipeCfg.yuvAlign)           ||
        (instCfg->pipeCfg.cscRange           != progCfg->pipeCfg.cscRange)           ||
        (instCfg->pipeCfg.scEnable           != progCfg->pipeCfg.scEnable)           ||
        (instCfg->pipeCfg.alphaEnable        != progCfg->pipeCfg.alphaEnable)        ||
        (instCfg->pipeCfg.inFmt.width        != progCfg->pipeCfg.inFmt.width)        ||
        (instCfg->pipeCfg.inFmt.height       != progCfg->pipeCfg.inFmt.height)       ||
        (instCfg->pipeCfg.inFmt.dataFormat   != progCfg->pipeCfg.inFmt.dataFormat)   ||
        (instCfg->pipeCfg.outFmt.width       != progCfg->pipeCfg.outFmt.width)       ||
        (instCfg->pipeCfg.outFmt.height      != progCfg->pipeCfg.outFmt.height)      ||
        (instCfg->pipeCfg.outFmt.pitch       != progCfg->pipeCfg.outFmt.pitch)       ||
        (instCfg->pipeCfg.outFmt.dataFormat  != progCfg->pipeCfg.outFmt.dataFormat)  ||
        (instCfg->pipeCfg.outFmt.scanFormat  != progCfg->pipeCfg.outFmt.scanFormat)  ||
        (instCfg->pipeCfg.outFmt.ccsFormat   != progCfg->pipeCfg.outFmt.ccsFormat)   ||
        (instCfg->pipeCfg.outFmt.fieldMerged != progCfg->pipeCfg.outFmt.fieldMerged) ||
        (instCfg->pipeCfg.inPos.startX       != progCfg->pipeCfg.inPos.startX)       ||
        (instCfg->pipeCfg.inPos.startY       != progCfg->pipeCfg.inPos.startY))
    {
        retVal = UFALSE;
    }

    return retVal;
}

static uint32_t Dss_m2mDrvWbDmaCfgChk(const CSL_DssWbPipeDmaCfg *instCfg,
                                      const CSL_DssWbPipeDmaCfg *progCfg)
{
    uint32_t retVal = UTRUE;

    if ((instCfg->bufLowThreshold  != progCfg->bufLowThreshold)  ||
        (instCfg->bufHighThreshold != progCfg->bufHighThreshold) ||
        (instCfg->arbitration      != progCfg->arbitration)      ||
        (instCfg->frameCaptureRate != progCfg->frameCaptureRate) ||
        (instCfg->idleSize         != progCfg->idleSize)         ||
        (instCfg->idleNumber       != progCfg->idleNumber))
    {
        retVal = UFALSE;
    }

    return retVal;
}

static uint32_t Dss_m2mDrvWbMFlagCfgChk(const Dss_WbPipeMflagParams *instCfg,
                                        const Dss_WbPipeMflagParams *progCfg)
{
    uint32_t retVal = UTRUE;

    if ((instCfg->mFlagCfg.thresholdLow  != progCfg->mFlagCfg.thresholdLow)  ||
        (instCfg->mFlagCfg.thresholdHigh != progCfg->mFlagCfg.thresholdHigh))
    {
        retVal = UFALSE;
    }

    return retVal;
}

static uint32_t Dss_m2mDrvWbCscCfgChk(const CSL_DssCscCoeff *instCfg,
                                      const CSL_DssCscCoeff *progCfg)
{
    uint32_t retVal = UTRUE;

    if ((instCfg->cscRange     != progCfg->cscRange)    ||
        (instCfg->c00          != progCfg->c00)         ||
        (instCfg->c01          != progCfg->c01)         ||
        (instCfg->c02          != progCfg->c02)         ||
        (instCfg->c10          != progCfg->c10)         ||
        (instCfg->c11          != progCfg->c11)         ||
        (instCfg->c12          != progCfg->c12)         ||
        (instCfg->c20          != progCfg->c20)         ||
        (instCfg->c21          != progCfg->c21)         ||
        (instCfg->c22          != progCfg->c22)         ||
        (instCfg->preOffset1   != progCfg->preOffset1)  ||
        (instCfg->preOffset2   != progCfg->preOffset2)  ||
        (instCfg->preOffset3   != progCfg->preOffset3)  ||
        (instCfg->postOffset1  != progCfg->postOffset1) ||
        (instCfg->postOffset2  != progCfg->postOffset2) ||
        (instCfg->postOffset3  != progCfg->postOffset3))
    {
        retVal = UFALSE;
    }

    return retVal;
}

static int32_t Dss_m2mDrvPrgramWbPipe(DssM2MDrv_VirtContext *context)
{
    int32_t retVal = FVID2_SOK;
    uint32_t copyCfg = UFALSE;
    DssM2MDrv_InstObj *instObj;
    DssM2MDrv_WbPipeCfg *instCfg, *progCfg;

    instObj = context->instObj;
    instCfg = &context->instCfg.wbPipeCfg;
    progCfg = &instObj->progCfg.wbPipeCfg;
    GT_assert(DssTrace, (NULL != instObj->wbRegs));
    /* Re-program WB pipe only if channel/instance configurations
       are different than programmed */
    if (FVID2_SOK == retVal)
    {
        /* Check write-back pipe configuration */
        if (UFALSE == Dss_m2mDrvWbPipeCfgChk(
                                        &instCfg->cfgParams,
                                        &progCfg->cfgParams))
        {
            /* Program WB pipeline */
            if (CSL_PASS != CSL_dssWbPipeSetConfig(instObj->wbRegs,
                                &instCfg->cfgParams.pipeCfg))
            {
                retVal = FVID2_EFAIL;
            }
            else
            {
                copyCfg = UTRUE;
            }
        }
    }
    if (FVID2_SOK == retVal)
    {
        /* Check write-back pipe DMA configuration */
        if (UFALSE == Dss_m2mDrvWbDmaCfgChk(
                                        &instCfg->dmaCfg,
                                        &progCfg->dmaCfg))
        {
            /* Program WB pipeline */
            CSL_dssWbPipeSetDmaConfig(instObj->wbRegs,
                                      &instCfg->dmaCfg);
            copyCfg = UTRUE;
        }
    }
    if (FVID2_SOK == retVal)
    {
        /* Check write-back pipe MFlag configuration */
        if (UFALSE == Dss_m2mDrvWbMFlagCfgChk(
                                    &instCfg->mFlagParams,
                                    &progCfg->mFlagParams))
        {
            /* Program WB pipeline */
            CSL_dssWbPipeSetMflagConfig(instObj->wbRegs,
                                        &instCfg->mFlagParams.mFlagCfg);
            copyCfg = UTRUE;
        }
    }
    if (FVID2_SOK == retVal)
    {
        /* Check write-back pipe CSC configuration */
        if (UFALSE == Dss_m2mDrvWbCscCfgChk(
                                    &instCfg->cscCoeff,
                                    &progCfg->cscCoeff))
        {
            /* Program WB pipeline */
            CSL_dssWbPipeSetCSCCoeff(instObj->wbRegs,
                                     &instCfg->cscCoeff);
            copyCfg = UTRUE;
        }
    }

    if ((FVID2_SOK == retVal) && (UTRUE == copyCfg))
    {
        /* Update instance configurations */
        Fvid2Utils_memcpy(progCfg, instCfg, sizeof(DssM2MDrv_WbPipeCfg));
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlStart(DssM2MDrv_VirtContext *context)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_InstObj *instObj;

    /* Currently this API only does maintaining SW Driver states */
    instObj = context->instObj;
    GT_assert(DssTrace, (NULL != context));
    GT_assert(DssTrace, (NULL != instObj));

    if (NULL != instObj->commonObjRef->lockSem)
    {
        /* Take the instance semaphore */
        (void) SemaphoreP_pend(instObj->commonObjRef->lockSem,
                               SemaphoreP_WAIT_FOREVER);
    }

    context->state = DSSM2M_DRV_STATE_RUNNING;

    if (NULL != instObj->commonObjRef->lockSem)
    {
        /* Take the instance semaphore */
        (void) SemaphoreP_post(instObj->commonObjRef->lockSem);
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlStop(DssM2MDrv_VirtContext *context)
{
    int32_t retVal = FVID2_SOK;
    uint32_t numReqQNum, loopCnt;
    DssM2MDrv_InstObj *instObj;
    DssM2M_DrvQueObj *qObj = NULL;
    DssM2M_DrvBufManObj *bmObj = NULL;

    /* Currently this API only does maintaining SW Driver states */
    instObj = context->instObj;
    GT_assert(DssTrace, (NULL != context));
    GT_assert(DssTrace, (NULL != instObj));

    if (NULL != instObj->commonObjRef->lockSem)
    {
        /* Take the instance semaphore */
        (void) SemaphoreP_pend(instObj->commonObjRef->lockSem,
                               SemaphoreP_WAIT_FOREVER);
    }

    context->state = DSSM2M_DRV_STATE_STOPPED;

    /* When driver is stopped, put buffers from reqQ to doneQ for
       current virtual Context. This has to be done only for doneQ and
       not for reqQ as existing frame will be processed completely */
    if ((FVID2_SOK == retVal) &&
        (DSSM2M_DRV_STATE_STOPPED == context->state))
    {
        bmObj = &instObj->bmObj;
        GT_assert(DssTrace, (NULL != bmObj));
        numReqQNum = Fvid2Utils_getNumQElem(bmObj->reqQ);
        for (loopCnt = 0U ; loopCnt < numReqQNum ; loopCnt++)
        {
            qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->reqQ);
            GT_assert(DssTrace, (NULL != qObj));
            if (qObj->virtContext == context)
            {
                /* Move to doneQ, if it matches with currently closed context */
                Fvid2Utils_queue(context->doneQ, &qObj->qElem, qObj);
            }
            else
            {
                /* Queue back to reqQ */
                Fvid2Utils_queue(bmObj->reqQ, &qObj->qElem, qObj);
            }
        }
    }

    if (NULL != instObj->commonObjRef->lockSem)
    {
        /* Take the instance semaphore */
        (void) SemaphoreP_post(instObj->commonObjRef->lockSem);
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlSetWbPipeParams(DssM2MDrv_VirtContext *context,
                                       const Dss_WbPipeCfgParams *pipeCfg)
{
    int32_t retVal = FVID2_SOK, retVal2 = CSL_PASS;

    /* check for parameters */
    if (NULL == pipeCfg)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        if (CSL_DSS_WB_PIPE_MODE_M2M != pipeCfg->pipeCfg.wbMode)
        {
            /* Only M2M mode is supported in this driver */
            retVal = FVID2_EBADARGS;
            GT_0trace(DssTrace, GT_ERR, "Only M2M mode supported!!\r\n");
        }

        if (FVID2_SOK == retVal)
        {
            retVal2 = CSL_dssWbPipeCheckFmt(pipeCfg->pipeCfg.outFmt.dataFormat);
            if (CSL_PASS != retVal2)
            {
                GT_0trace(DssTrace, GT_ERR, "Given destination Fvid2_format not supported!!\r\n");
                retVal = FVID2_EBADARGS;
            }
        }

        if (FVID2_SOK == retVal)
        {
            retVal = Dss_m2mValidateM2mPipeParams(pipeCfg);
            if (FVID2_SOK != retVal)
            {
                GT_0trace(DssTrace, GT_ERR, "Invalid arguments have been passed!!\r\n");
            }
        }

        if (FVID2_SOK == retVal)
        {
            /* Only copy provided configurations to channel/virtual context object.
               This will be programmed to HW module when buffers are submitted
               through 'Fvid2_processRequest()' */
               Fvid2Utils_memcpy(&context->instCfg.wbPipeCfg.cfgParams,
                                 pipeCfg,
                                 sizeof(Dss_WbPipeCfgParams));
        }
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlSetmFlagParams(DssM2MDrv_VirtContext *context,
                                      const Dss_WbPipeMflagParams *mFlagParams)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == mFlagParams)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* Only copy provided configurations to channel/virtual context object.
           This will be programmed to HW module when buffers are submitted
           through 'Fvid2_processRequest()' */
           Fvid2Utils_memcpy(&context->instCfg.wbPipeCfg.mFlagParams,
                             mFlagParams,
                             sizeof(Dss_WbPipeMflagParams));
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlGetCurrentStatus(DssM2MDrv_VirtContext *context,
                                               Dss_WbStatus *status)
{
    int32_t retVal = FVID2_SOK;


    /* check for parameters */
    if (NULL == status)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* Only copy provided configurations to channel/virtual context object.
           This will be programmed to HW module when buffers are submitted
           through 'Fvid2_processRequest()' */
        Fvid2Utils_memcpy(status,
                          &context->wbStatus,
                          sizeof(Dss_WbStatus));
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlSetWbPipeCsc(DssM2MDrv_VirtContext *context,
                                    const CSL_DssCscCoeff *csc)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == csc)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* Only copy provided configurations to channel/virtual context object.
           This will be programmed to HW module when buffers are submitted
           through 'Fvid2_processRequest()' */
        Fvid2Utils_memcpy(&context->instCfg.wbPipeCfg.cscCoeff,
                          csc,
                          sizeof(CSL_DssCscCoeff));
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlSetDmaCfg(DssM2MDrv_VirtContext *context,
                                        const CSL_DssWbPipeDmaCfg *dmaCfg)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == dmaCfg)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* upgrade configurations into context object and re-program HW module
           on buffer submission */
        Fvid2Utils_memcpy(&context->instCfg.wbPipeCfg.dmaCfg,
                          dmaCfg,
                          sizeof(CSL_DssWbPipeDmaCfg));
    }

    return retVal;
}

static void Dss_wbPipeCbFxn(const uint32_t *event,
                            uint32_t numEvents,
                            void *arg)
{
    uint32_t  loopCnt, currEvent, wbPipeId = 0U;
    Dss_EvtMgrClientInfo *pClientObj = (Dss_EvtMgrClientInfo *)arg;
    uint32_t eventGroup = pClientObj->eventGroup;
    DssM2MDrv_InstObj *instObj;

    Dss_convEventGrouptoModule(eventGroup, &wbPipeId);
    GT_assert(DssTrace, (CSL_DSS_MODULE_INVALID != wbPipeId));
    instObj = &(gDssM2mCommonObj.instObj[wbPipeId]);
    for(loopCnt = 0U; loopCnt < numEvents; loopCnt++)
    {
        currEvent = event[loopCnt];
        if(DSS_WB_PIPE_EVENT_BUFF_FRAME_DONE == currEvent)
        {
           /* Application CB will be issued in the following function */
           Dss_wbPipeDmaCompletionCbFxn(instObj);
        }
        else if(DSS_WB_PIPE_EVENT_BUFF_OVERFLOW == currEvent)
        {
           instObj->status.underflowCount++;
        }
        else
        {
            GT_assert(DssTrace, BFALSE);
        }
    }

    return;
}

static void Dss_wbPipeDmaCompletionCbFxn(DssM2MDrv_InstObj *instObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t loopCnt;
    DssM2MDrv_VirtContext *virtContext = NULL;
    DssM2M_DrvQueObj *qObj = NULL;
    DssM2M_DrvBufManObj *bmObj = NULL;

    GT_assert(DssTrace, (NULL != instObj));
    bmObj = &instObj->bmObj;
    GT_assert(DssTrace, (NULL != bmObj));

    /** Steps
     *  1. Get a queue object from the currQ. Assert error if NULL.
     *  2. Move the queue object to doneQ
     *  3. Increment wb frame count Count in the status
     *  4. If number of elements in currQ < 1, then do following
     *     a. Get a queue object from the reqQ. Assert error if NULL
     *     b. Disable DSS and WB pipeline
     *     c. Re-program DSS pipeline, Overlay and WB Pipeline if needed
     *     d. Set buffer addresses in the WB & DSS Pipeline
     *     e. Enable DSS and WB pipeline
     *     f. Move the queue object to currQ
     */
    /* Steps 1 to 3 */
    if (FVID2_SOK == retVal)
    {
        qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->currQ);
        GT_assert(DssTrace, (NULL != qObj));
        virtContext = qObj->virtContext;
        GT_assert(DssTrace, (NULL != virtContext));
        Fvid2Utils_queue(virtContext->doneQ, &qObj->qElem, qObj);
        virtContext->wbStatus.wbFrmCount++;
        instObj->status.wbFrmCount++;
        /* Issue application CB */
        if(NULL != virtContext->fdmCbParams.fdmCbFxn)
        {
            virtContext->fdmCbParams.fdmCbFxn(virtContext->fdmCbParams.fdmData);
        }
    }

    /* Step 6 */
    if (FVID2_SOK == retVal)
    {
        GT_assert(DssTrace, (NULL != instObj->commRegs));
        for (loopCnt = 0U ; loopCnt < virtContext->numPipe ; loopCnt++)
        {
            GT_assert(DssTrace,
                    (NULL != instObj->pipeRegs[virtContext->pipeId[loopCnt]]));
        }
        GT_assert(DssTrace, (NULL != instObj->ovrRegs));
        GT_assert(DssTrace, (NULL != instObj->wbRegs));

        /* Only program module if a frame is submitted */
        if ((0U == Fvid2Utils_getNumQElem(bmObj->currQ)) &&
            (0U <  Fvid2Utils_getNumQElem(bmObj->reqQ)))
        {
            qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->reqQ);
            GT_assert(DssTrace, (NULL != qObj));
            for (loopCnt = 0U ; loopCnt < virtContext->numPipe ; loopCnt++)
            {
                CSL_dssVidPipeEnable(instObj->pipeRegs[virtContext->pipeId[loopCnt]],
                                     UFALSE);
            }
            CSL_dssWbPipeEnable(instObj->wbRegs, UFALSE);

            /* DCTRL has to be configurred before DISP */
            retVal += Dss_m2mDrvPrgramDctrl(qObj->virtContext);
            retVal += Dss_m2mDrvPrgramDisp(qObj->virtContext);
            retVal += Dss_m2mDrvPrgramWbPipe(qObj->virtContext);

            if (FVID2_SOK == retVal)
            {
                /* Queue to CurrQ */
                Fvid2Utils_queue(bmObj->currQ, &qObj->qElem, qObj);

                for (loopCnt = 0U ; loopCnt < virtContext->numPipe ; loopCnt++)
                {
                    CSL_dssVidPipeSetBuffAddr(
                            instObj->pipeRegs[qObj->inFrm[loopCnt]->chNum],
                            FVID2_FID_TOP,
                            qObj->inFrm[loopCnt]->addr[0U],
                            qObj->inFrm[loopCnt]->addr[1U]);
                }
                CSL_dssWbPipeSetBuffAddr(instObj->wbRegs,
                                         FVID2_FID_TOP,
                                         qObj->outFrm->addr[0U],
                                         qObj->outFrm->addr[1U]);

                for (loopCnt = 0U ; loopCnt < virtContext->numPipe ; loopCnt++)
                {
                    CSL_dssVidPipeEnable(instObj->pipeRegs[virtContext->pipeId[loopCnt]],
                                         UTRUE);
                }
                CSL_dssWbPipeSetGoBit(instObj->wbRegs);
                CSL_dssWbPipeEnable(instObj->wbRegs, UTRUE);
            }
            else
            {
                /* Queue back in case of failure */
                Fvid2Utils_queue(bmObj->reqQ, &qObj->qElem, qObj);
                GT_0trace(DssTrace, GT_ERR,
                          "Programming Failed: DSS Pipe/Disp/WB Pipe\r\n");
            }

        }
    }

    GT_assert(DssTrace, (FVID2_SOK == retVal));

    return;
}

static int32_t Dss_m2mValidateM2mPipeParams(const Dss_WbPipeCfgParams *wbPipeParams)
{
    int32_t retVal = FVID2_SOK, planeIdx, numFields, fieldIdx;
    uint32_t pitchValArr[FVID2_MAX_PLANES_PER_FIELD];
    CSL_DssWbPipeCfg wbPipeCfg;

    wbPipeCfg = wbPipeParams->pipeCfg;
    if (FVID2_DF_ARGB48_12121212 != wbPipeCfg.inFmt.dataFormat)
    {
        GT_0trace(DssTrace, GT_ERR, "Input format can only be ARGB12121212!!\r\n");
        retVal = FVID2_EBADARGS;
    }

    if( ((wbPipeCfg.inFmt.width != wbPipeCfg.outFmt.width) ||
         (wbPipeCfg.inFmt.height != wbPipeCfg.outFmt.height)) &&
        (UTRUE != wbPipeCfg.scEnable) )
    {
        GT_0trace(DssTrace, GT_ERR, "Scaling must be enabled if input and output widths/heights are different!!\r\n");
        retVal = FVID2_EBADARGS;
    }

    if (FVID2_DF_YUV420SP_UV == wbPipeCfg.outFmt.dataFormat)
    {
        if ((wbPipeCfg.inFmt.width > (2*wbPipeCfg.outFmt.width)) ||
            (wbPipeCfg.inFmt.height > (2*wbPipeCfg.outFmt.height)))
        {
            GT_0trace(DssTrace, GT_ERR, "For YUV420 output, can not downscale more than a factor of 2!!\r\n");
            retVal = FVID2_EBADARGS;
        }
    }

    /* The the pitch factors in bits per pixel. */
    retVal = CSL_dssWbGetPitchFactors(wbPipeCfg.outFmt.dataFormat, pitchValArr);

    if (CSL_PASS == retVal)
    {
        if (FVID2_SF_INTERLACED == wbPipeCfg.outFmt.scanFormat)
        {
            numFields = 2;
        }
        else
        {
            numFields = 1;
        }
        for(fieldIdx = 0U ; fieldIdx < numFields; fieldIdx++)
        {
            for(planeIdx = 0U ; planeIdx < FVID2_MAX_PLANES_PER_FIELD; planeIdx++)
            {
                /* Application programmes pitch in terms of Byte, so divide referance pitch factor by 8. */
                /* Application can allocate buffer in a way that the programmed pitch is greater than the minimum required pitch. */
                if (wbPipeCfg.outFmt.pitch[fieldIdx * FVID2_MAX_PLANES_PER_FIELD + planeIdx] < (wbPipeCfg.outFmt.width*pitchValArr[planeIdx])/8)
                {
                    retVal = FVID2_EBADARGS;
                    GT_0trace(DssTrace, GT_ERR, "Programmed pitch is too little for given data format parameters!!\r\n");
                    break;
                }
            }
        }
    }

    return retVal;
}