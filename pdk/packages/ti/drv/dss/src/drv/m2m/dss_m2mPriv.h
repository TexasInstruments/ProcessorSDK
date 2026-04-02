/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *  \file dss_m2mPriv.h
 *
 *  \brief DSS M2M driver private header file.
 */

#ifndef DSS_M2MPRIV_H_
#define DSS_M2MPRIV_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drv/dss/dss.h>

#ifdef __cplusplus
"C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \brief Minimum number of buffers needed for starting the display operation.
 */
#define DSS_M2M_DRV_MIN_PRIME_BUFFERS        (0U)

/**
 *  \anchor DSSM2M_DrvState
 *  \name   DSS M2M DRV State
 *  \brief  Represents the state of the DSS M2M Driver state.
 *
 *  @{
 */
/*
 * State transitions are explained below
 *
 * IDLE - initial or not open state
 *  |
 * CREATED - state after calling Fvid2_create
 *  |
 * DO_START - temporary state when Fvid2_start() is called
 *  |         this means driver has requested Capture Module to start the instance
 *  |
 * RUNNING  - this is the state after Fvid2_start() completes, this means
 *  |         Capture Module started capture and now it is running, it remains in this
 *  |         state
 *  |         until Fvid2_stop()  is called
 *  |
 * DO_STOP  - temporary state when Fvid2_stop is called()
 *  |         this means driver has request Capture Module to stop the instance
 *  |
 * STOPPED  - this is the state after Fvid2_stop() completes, this means
 *  |         Capture Module has stopped this instance
 *  |
 * IDLE     - this is the state after Fvid2_delete() completes
 */
/** \brief Driver is not open and is idle. */
#define DSSM2M_DRV_STATE_IDLE             ((uint32_t)0U)
/** \brief Driver is created. */
#define DSSM2M_DRV_STATE_CREATED          ((uint32_t)1U)
/** \brief Driver is requesting a start. */
#define DSSM2M_DRV_STATE_DO_START         ((uint32_t)2U)
/** \brief Driver is running. */
#define DSSM2M_DRV_STATE_RUNNING          ((uint32_t)3U)
/** \brief Driver is requesting a stop. */
#define DSSM2M_DRV_STATE_DO_STOP          ((uint32_t)4U)
/** \brief Driver is stopped. */
#define DSSM2M_DRV_STATE_STOPPED          ((uint32_t)5U)
/** \brief Driver is un-initialized. */
#define DSSM2M_DRV_STATE_UNINIT           ((uint32_t)6U)
/* @} */

/**
 *  \anchor DSSM2M_DrvUsageStatus
 *  \name   DSS M2M Usage status
 *  \brief  Usage status for module of DSS M2M DRV.
 *
 *  @{
 */
/** \brief Usage Status: Not used */
#define DSSM2M_DRV_USAGE_STATUS_NOT_USED               ((uint32_t) 0xDEADBABEU)
/** \brief Usage Status: In use */
#define DSSM2M_DRV_USAGE_STATUS_IN_USE                 ((uint32_t) 0xABCDEF01U)
/* @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
typedef struct DssM2MDrv_InstObj_t DssM2MDrv_InstObj;
typedef struct DssM2MDrv_VirtContext_t DssM2MDrv_VirtContext;
typedef struct DssM2MDrv_DispPipeCfg_t DssM2MDrv_DispPipeCfg;
typedef struct DssM2MDrv_dctrlCfg_t DssM2MDrv_dctrlCfg;
typedef struct DssM2MDrv_CommonObj_t DssM2MDrv_CommonObj;
typedef struct DssM2MDrv_WbPipeCfg_t DssM2MDrv_WbPipeCfg;
typedef struct DssM2MDrv_moduleCfg_t DssM2MDrv_moduleCfg;
typedef struct DssM2M_DrvQueObj_t DssM2M_DrvQueObj;
typedef struct DssM2M_DrvBufManObj_t DssM2M_DrvBufManObj;

/*
 * struct DssM2MDrv_DispPipeCfg_t
 * \brief DSS display pipe configurations handle object.
 *        These are object re-used from DSS_DISP_DRV
 */
struct DssM2MDrv_DispPipeCfg_t
{
    Dss_DispParams cfgParams;
    /**< DSS pipeline parameters */
    Dss_DispPipeMflagParams mFlagParams;
    /**< Video Pipeline Mflag Information */
    CSL_DssCscCoeff cscCoeff;
    /**< CSC co-efficients for DSS_DISP_DRV */
};

/*
 * struct DssM2MDrv_dctrlCfg_t
 * \brief DSS display controller configurations handle object.
 *        These are object re-used from DSS_DCTRL_DRV
 */
struct DssM2MDrv_dctrlCfg_t
{
    Dss_DctrlOverlayParams ovlParams;
    /**< Overlay configurations information */
    Dss_DctrlOverlayLayerParams ovlLayerParams;
    /**< Overlay layer information */
    Dss_DctrlGlobalDssParams globalParams;
    /**< global DSS Configuration */
};

/*
 * struct DssM2MDrv_WbPipeCfg_t
 * \brief DSS display write-back pipe configurations handle object.
 */
struct DssM2MDrv_WbPipeCfg_t
{
    Dss_WbPipeCfgParams cfgParams;
    /**< DSS write-back pipe configuration */
    CSL_DssWbPipeDmaCfg dmaCfg;
    /**< DMA configuration for WB pipeline */
    Dss_WbPipeMflagParams  mFlagParams;
    /**< Video write-back Pipe Mflag Information */
    CSL_DssCscCoeff cscCoeff;
    /**< CSC co-efficients for write-back pipeline */
};

/*
 * struct DssM2MDrv_moduleCfg_t
 * \brief Object containing DSS module configurations.
 */
struct DssM2MDrv_moduleCfg_t
{
    DssM2MDrv_DispPipeCfg pipeCfg[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Display pipeline configuration parameters */
    DssM2MDrv_dctrlCfg dctrlCfg;
    /**< Display controller configuration parameters  */
    DssM2MDrv_WbPipeCfg wbPipeCfg;
    /**< Write-back pipeline configuration parameters */
};

/*
 * struct DssM2MDrv_VirtContext_t
 * \brief DSS M2M driver create handle object.
 */
struct DssM2MDrv_VirtContext_t
{
    uint32_t contextId;
    /**< Context ID */
    uint32_t inUse;
    /**< Current is being used or not.
      *   See \ref DSSM2M_DrvUsageStatus for details */
    uint32_t state;
    /**< Instance state.
      *  See \ref DSSM2M_DrvState for details */
    DssM2MDrv_InstObj *instObj;
    /**< Reference to instance object. */
    Dss_WbCreateParams createParams;
    /**< DSS M2M module create configurations.
      *  See \ref Dss_WbCreateParams for details */
    Fvid2_DrvCbParams fdmCbParams;
    /**< FVID2 driver manager callback function parameters. */
    Dss_WbStatus wbStatus;
    /**< DSS M2M driver status.
      *  See \ref Dss_WbStatus for details */
    DssM2MDrv_moduleCfg instCfg;
    /**< DSS module configurations provided by application.
      *  See \ref DssM2MDrv_moduleCfg for details */
    Fvid2UtilsLinkListObj *doneQ;
    /**< Queue object to put the displayed output requests */
    Fvid2UtilsLinkListObj doneLlObj;
    /**< Linked List object for doneQ */
    uint32_t numBufQed;
    /**< Number of buffers Queued to HW */
    uint32_t numPipe;
    /**< Number of Display pipe-lines used for given M2M/WB pipeline. */
    uint32_t pipeId[DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE];
    /**< ID DSS pipeline to use.
     *   See \ref CSL_DssVidPipeId for details */
    uint32_t overlayId;
    /**< ID DSS overlay to use. CSL_DssOverlayId.
     *   See \ref CSL_DssOverlayId for details */
};

/**
 *  \brief Structure defining the queue object used in queue/dequeue operation.
 *   Instead of creating frame objects, this is used so that any other
 *   information could be queued/dequeued along with the frame.
 *   Already qElem is added to avoid dynamic allocation of Queue nodes.
 */
struct DssM2M_DrvQueObj_t
{
    Fvid2Utils_QElem qElem;
    /**< FVID2 Utils queue element used in node addition */
    DssM2MDrv_VirtContext *virtContext;
    /**< Reference to the instance object for this queue object */
    uint32_t numPipe;
    /**< Number of DSS pipes used. This will be also equal to number of frames in inFrm */
    Fvid2_Frame *inFrm[DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE];
    /**< #FVID2_frame to read the incoming frame packets from memory */
    Fvid2_Frame *outFrm;
    /**< #FVID2_frame to store the outgoing frame packets to memory */
};

/**
 *  \brief Structure to store the buffer management functionality variables.
 */
struct DssM2M_DrvBufManObj_t
{
    /*
     * Initially all the queue elements will be in freeQ.
     *
     * For every application submit request operation,      freeQ -> reqQ
     * For every request submitted to hardware,             reqQ  -> currQ
     * For every request completion from hardware,          currQ  -> doneQ
     * For every application get request operation,         doneQ -> freeQ
     */
    Fvid2UtilsLinkListObj *freeQ;
    /**< Queue for queuing all the free queue objects */
    Fvid2UtilsLinkListObj freeLlObj;
    /**< Linked List object for freeQ */
    Fvid2UtilsLinkListObj *reqQ;
    /**< Queue object to put the input requests */
    Fvid2UtilsLinkListObj reqLlObj;
    /**< Linked List object for reqQ */
    Fvid2UtilsLinkListObj *currQ;
    /**< Buffers that are queued to hardware but not yet fully displayed */
    Fvid2UtilsLinkListObj curLlObj;
    /**< Linked List object for currQ */
    DssM2M_DrvQueObj m2mQObj[(DSS_DEF_QUEUE_LEN_PER_M2M_DRV_INST * DSSM2M_NUM_VIRTUAL_CONTEXT)];
    /**< Display queue objects */
};

/**
 *  \brief DSS M2M Driver Instance Object.
 */
struct DssM2MDrv_InstObj_t
{
    uint32_t drvInstId;
    /**< Instance ID. */
    uint32_t inUse;
    /**< Current is being used or not.
      *   See \ref DSSM2M_DrvUsageStatus for details */
    uint32_t numRegEvtHandle;
    /**< Number of registered event group handles */
    void *evtGroupHandle[DSS_DISP_INST_EVT_MGR_MAX_CLIENTS];
    /**< Handle to registered event groups */
    DssM2MDrv_CommonObj *commonObjRef;
    /**< Reference to driver common object */
    Dss_WbStatus status;
    /**< DSS M2M driver status */
    DssM2MDrv_moduleCfg progCfg;
    /**< DSS module configurations actually programmed into the HW module.
      *  See \ref DssM2MDrv_moduleCfg for details */
    CSL_dss_commRegs *commRegs;
    /**< Write-back Pipe registers */
    CSL_dss_pipeRegs *pipeRegs[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Write-back Pipe registers */
    CSL_dss_overlayRegs *ovrRegs;
    /**< Write-back Pipe registers */
    CSL_dss_wbRegs *wbRegs;
    /**< Write-back Pipe registers */
    DssM2M_DrvBufManObj bmObj;
    /**< Buffer management object */
    uint32_t numPipe;
    /**< Number of Display pipe-lines used for given M2M/WB pipeline. */
    uint32_t pipeId[DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE];
    /**< ID of DSS pipeline used. */
    uint32_t overlayId;
    /**< ID of DSS overlay used. */
    uint32_t pathCfgDone;
    /**< Path configuration done. True if done, false otherwise. */
    Dss_DctrlPathInfo pathInfo;
    /**< DSS path information */
};

/*
 * struct DssM2MDrv_CommonObj_t
 * \brief DSS M2M driver global/common driver object.
 */
struct DssM2MDrv_CommonObj_t
{
    Fvid2_DrvOps fvidDrvOps;
    /**< FVID2 driver ops. */
    uint32_t isRegistered;
    /**< FVID2 registration complete flag. */
    uint32_t numVirtContUsed[DSSM2M_NUM_WB_PIPELINE];
    /**< Number of virtual context used */
    DssM2MDrv_VirtContext virtContext[DSSM2M_NUM_WB_PIPELINE][DSSM2M_NUM_VIRTUAL_CONTEXT];
    /**< Virtual context objects. */
    DssM2MDrv_InstObj instObj[DSSM2M_NUM_WB_PIPELINE];
    /**< DSS M2M HW instance objects. */
    Dss_SocInfo socInfo;
    /**< Structure containing Register addresses for DSS regions */
    Fvid2_TimeStampFxn getTimeStamp;
    /**< Get time stamp function. */
    SemaphoreP_Handle lockSem;
    /**< Semaphore to protect the open/close calls and other memory
     *   allocation per instance. */
};
/* ========================================================================== */
/*                  Internal/Private Function Declarations                   */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
int32_t Dss_m2mDrvIoctlSetDssPipeParams(DssM2MDrv_VirtContext *context,
                                        const Dss_PipeCfgParams *pipeCfg);

int32_t Dss_m2mDrvIoctlSetPipeMflagParams(DssM2MDrv_VirtContext *context,
                                  const Dss_PipeMflagParams *mFlagParams);

int32_t Dss_m2mDrvIoctlSetPipeCsc(DssM2MDrv_VirtContext *context,
                                  const Dss_PipeCscParams *csc);

int32_t Dss_m2mDrvIoctlSetOverlayParams(DssM2MDrv_VirtContext *context,
                                    const Dss_DctrlOverlayParams *ovlParams);

int32_t Dss_m2mDrvIoctlSetLayerParams(DssM2MDrv_VirtContext *context,
                        const Dss_DctrlOverlayLayerParams *ovlLayerParams);

int32_t Dss_m2mDrvIoctlSetGlobalParams(DssM2MDrv_VirtContext *context,
                               const Dss_DctrlGlobalDssParams *globalParams);

uint32_t Dss_m2mDrvDctrlOvlCfgChk(const Dss_DctrlOverlayParams *instCfg,
                                  const Dss_DctrlOverlayParams *progCfg);

uint32_t Dss_m2mDrvDctrlOvlLayerCfgChk(const Dss_DctrlOverlayLayerParams *instCfg,
                                       const Dss_DctrlOverlayLayerParams *progCfg);

uint32_t Dss_m2mDrvDctrlGlobalCfgChk(const Dss_DctrlGlobalDssParams *instCfg,
                                     const Dss_DctrlGlobalDssParams *progCfg);

uint32_t Dss_m2mDrvDctrlPathCfgChk(const Dss_DctrlPathInfo * instCfg,
                                   const Dss_DctrlPathInfo * progCfg);

int32_t Dss_m2mDrvPrgramDctrl(DssM2MDrv_VirtContext *context);

uint32_t Dss_m2mDrvDispPipeCfgChk(const Dss_DispParams *instCfg,
                                  const Dss_DispParams *progCfg);

uint32_t Dss_m2mDrvDispMFlagCfgChk(const Dss_DispPipeMflagParams *instCfg,
                                   const Dss_DispPipeMflagParams *progCfg);

uint32_t Dss_m2mDrvDispCscCfgChk(const CSL_DssCscCoeff *instCfg,
                                 const CSL_DssCscCoeff *progCfg);

int32_t Dss_m2mDrvPrgramDisp(DssM2MDrv_VirtContext *context);

int32_t Dss_m2mDrvSetWbPipeDmaCfg(DssM2MDrv_InstObj *instObj,
                                  const CSL_DssWbPipeDmaCfg *dmaCfg);
/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSS_M2MPRIV_H_ */
