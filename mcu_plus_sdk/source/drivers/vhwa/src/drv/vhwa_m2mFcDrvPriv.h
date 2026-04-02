/**
 *   Copyright (c) Texas Instruments Incorporated 2021
 *   All rights reserved.
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
 *  \file vhwa_m2mFcDrvPriv.h
 *
 *  \brief Flex-Connect Private header file
 *
 */

#ifndef VHWA_FC_DRV_PRIV_H_
#define VHWA_FC_DRV_PRIV_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>


#include "src/csl/include/csl_hts.h"
#include "src/csl/include/csl_lse.h"
#include "src/csl/include/csl_vpactop.h"
#include "src/csl/include/csl_nf.h"

#include "include/flexConnect_cfg.h"

#include <include/vhwa_common.h>
#include "src/drv/vhwa_cfg.h"
#include "src/drv/vhwa_utils.h"

#include "src/drv/vhwa_m2mFcGraphPriv.h"
#include "src/drv/vhwa_m2mMscPriv.h"
#include "src/drv/vhwa_m2mVissPriv.h"


#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define VHWA_FC_DRV_QUEUE_ENTRIES           (15u)


/* Frame Frame offset for modules */
#define VHWA_VISS_IN_FRAME_OFFSET        (0u)
#define VHWA_MSC0_IN_FRAME_OFFSET        (VHWA_VISS_IN_FRAME_OFFSET + \
                                                VHWA_M2M_VISS_MAX_INPUTS)
#define VHWA_MSC1_IN_FRAME_OFFSET        (VHWA_MSC0_IN_FRAME_OFFSET + 1u)
#define VHWA_LDC_IN_FRAME_OFFSET         (VHWA_MSC1_IN_FRAME_OFFSET + 1u)
#define VHWA_NF_IN_FRAME_OFFSET          (VHWA_LDC_IN_FRAME_OFFSET + 1u)

#define VHWA_VISS_OUT_FRAME_OFFSET        (0u)
#define VHWA_MSC0_OUT_FRAME_OFFSET        (VHWA_VISS_OUT_FRAME_OFFSET + \
                                                VHWA_M2M_VISS_MAX_OUTPUTS)
#define VHWA_MSC1_OUT_FRAME_OFFSET        (VHWA_MSC0_OUT_FRAME_OFFSET + \
                                                MSC_MAX_OUTPUT)
#define VHWA_LDC_OUT_FRAME_OFFSET         (VHWA_MSC1_OUT_FRAME_OFFSET + \
                                                MSC_MAX_OUTPUT)
#define VHWA_NF_OUT_FRAME_OFFSET          (VHWA_LDC_OUT_FRAME_OFFSET + \
                                                LDC_MAX_OUTPUT)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    Vhwa_M2mMscSl2Params    mscSl2Prms;

    Vhwa_M2mVissSl2Prms     vissSl2Prms;
} Vhwa_m2mFcDrvSl2ResObj;

typedef struct
{
    Fvid2_FrameList         vissInFrameList;
    Fvid2_FrameList         vissOutFrameList;

    Fvid2_FrameList         msc0InFrameList;
    Fvid2_FrameList         msc0OutFrameList;

    Fvid2_FrameList         msc1InFrameList;
    Fvid2_FrameList         msc1OutFrameList;
}Vhwa_M2mFcDrvFrameList;

typedef struct
{
    uint32_t                isVissEnabled;
    uint32_t                isMsc0Enabled;
    uint32_t                isMsc1Enabled;

    Vhwa_m2mFcDrvSl2ResObj  sl2ResObj;
}Vhwa_M2mFcDrvModInfo;

/**
 *  \brief Flex-Connect Graph object.
 */
typedef struct
{
    void *graph;
    /**< Pointer to graph object */
    Vhwa_GraphInfo      graphInfo;
    /**< Graph object */
    Vhwa_GraphNodeList  graphObjNodeList;
    /**< Graph object node list */
    Vhwa_M2mFcGraphNodeInfo  graphNodeObj[VHWA_FC_MAX_NODE_INST];
    /**< node object list */
    uint32_t            numEdges;
    /**< Edge count */
    Vhwa_M2mFcEdgeInfo graphEdgeObj[VHWA_FC_MAX_EDGES];
    /**< edge object list */
} Vhwa_M2mFcGraphObj;


typedef struct
{
    uint32_t                    isUsed;
    /**< Flag to indicate if this object is free or not */

    int32_t                     firstNode;
    /**< First Node in Graph */
    int32_t                     lastNode;
    /**< Last Node in Graph */

    Vhwa_M2mFcModuleHdls        modHdls;

    uint32_t                    pipeline;
    /**< HTS Pipeline number,
         Same pipeline is used for all handles,
         Allocated on the first open
         Refer to #CSL_HtsPipeline for valid values. */

    Vhwa_M2mFcGraphObj          graphObj;

    Vhwa_M2mFcGraphPathInfo     pathInfo;

    Fvid2_DrvCbParams           cbPrms;
    /** Callback parameters, copied locally */

    Vhwa_M2mFcCreatePrms        createPrms;

    Vhwa_M2mFcDrvModInfo        modInfo;

    Vhwa_M2mFcDrvFrameList      frameList;

    Vhwa_M2mVissFcConPrms       vissFcConPrms;

    Vhwa_M2mVissFcUpdatePrms    vissFcPrms;

    Vhwa_M2mMscFcConPrms        msc0FcConPrms;
    Vhwa_M2mMscFcConPrms        msc1FcConPrms;

    Vhwa_M2mMscFcUpdatePrms     msc0FcPrms;
    Vhwa_M2mMscFcUpdatePrms     msc1FcPrms;

    Vhwa_M2mMscFcGetPrms        msc0FcGetPrms;
    Vhwa_M2mMscFcGetPrms        msc1FcGetPrms;

    uint32_t                    hIdx;
    /**< The index of this handle object in the array,
         used in allocating UDMA memory */

    volatile uint64_t           perfNum;
    /**< Performance number for the last submitted request,
         Used only if GetTimeStamp function pointer is set in
         Create Args */

    Fvid2UtilsLinkListObj       doneQObj;
    /**< Done Queue, queue containing queue objects,
     *   for which SD processing is completed */
    Fvid2UtilsLinkListObj       *doneQ;
    /**< Pointer to Done Queue */

    uint32_t                    numPendReq;
    /**< Count of pending requests */

    Fc_ErrEventParams           eePrms;
    /**< Parameters for the Error Events */
} Vhwa_M2mFcHandleObj;

/**
 *  \brief Structure defining the queue object used in queue/dequeue operation.
 *         This is used for enqueueing pending request as well as
 *         the completed requested to the drivers request queue
 *         and done queue resepctively. This stores pointer to handle
 *         object, for easy access to configurations and also input
 *         and output frame lists.
 */
typedef struct
{
    Fvid2Utils_QElem            qElem;
    /**< FVID2 Utils queue element used in node addition. */
    Vhwa_M2mFcHandleObj         *hObj;
    /**< Handle Object reference for this Queue object */
    Fvid2_FrameList         inFrmList;
    /**< FVID2 frame list for store list of input frames. */
    Fvid2_FrameList         outFrmList;
    /**< FVID2 frame list for store list of output frames. */
} Vhwa_M2mFcQueueObj;

typedef struct
{
    uint32_t                    initDone;
    /**< Flag to know if SD is initialized or not,
         Essentially init is called or not */
    uint32_t                    isSl2AllocDone;
    /**< Flag to know if SL2 memory is allocated or not.
     *   Essentially if Vhwa_m2mNfAllocSl2 is called and is successfully
     *   executed or not.
     *   Driver open is allowed only if this flag is set to UTRUE */

    Vhwa_M2mFcInitPrms          initPrms;
    /**< Init paramters, copied here */

    CSL_htsRegs                 *htsRegs;

    uint32_t                    isRegistered;
    /**< Flag to indicate if this instance object is registered to FVID */


    VhwaAl_SemaphoreP_Struct           lock;
    /**< Semaphore to protect instance object's variables. */
    VhwaAl_SemaphoreP_Struct           pLock;
    /**< Semaphore to protect SD HWA.
     *   Only one processing request can be submitted to the SD HW.
     *   This lock makes sure that except submitted one, all other
     *   requests are waiting on this semaphore.
     *   There is no longer support for submission from ISR and so
     *   no ISR protection in processRequest. */

    uint32_t                    openCnt;
    /**< Keeps track of number of opens,
         used to initialize common configuration only once
         for example, enabling SD at the top level or
            initializing ring accelerator */

    Vhwa_M2mFcSl2AllocPrms      sl2Prms;
    /**< Sl2 prameters for this handle Memory */

    Vhwa_M2mFcQueueObj          *actQObj;
    /**< Pointer points to queue object, whose request is submitted to
     * the HW and not yet completed */
    Vhwa_M2mFcHandleObj         *lastHndlObj;
    /**< Pointer to the handle object, whose request was submitted
     *   and completed */

    Fvid2UtilsLinkListObj       freeQObj;
    /**< Free Queue, queue containing free queue objcts */
    Fvid2UtilsLinkListObj       *freeQ;
    /**< Pointer to Free Queue */
    Vhwa_M2mFcQueueObj          fcQObj[VHWA_FC_DRV_QUEUE_ENTRIES];
    /**< SD queue objects */

    uint32_t                    totalReqCnt;
    /**< Keeps track of total number of Request submitte */
} Vhwa_M2mFcDrvInstObj;


/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

int32_t Vhwa_m2mFcInitAndAllocRes(const Vhwa_M2mFcGraphObj *graphObj);
int32_t Vhwa_m2mFcGetFreeScedular(uint32_t schType);

int32_t Vhwa_m2mVissFrameComplCb(Fvid2_Handle handle, void *appData);
int32_t Vhwa_m2mMsc0FrameComplCb(Fvid2_Handle handle, void *appData);
int32_t Vhwa_m2mMsc1FrameComplCb(Fvid2_Handle handle, void *appData);

const Vhwa_M2mFcGraphNodeInfo* Vhwa_getDefaultNodeInfo(uint32_t *nodeMemSize);

int32_t Vhwa_m2mFcDrvConfigVissSch(Vhwa_M2mFcHandleObj *hObj,
                                 const Vhwa_M2mFcGraphNodeInfo *node);
int32_t Vhwa_m2mFcDrvConfigMscSch(Vhwa_M2mFcHandleObj *hObj,
                                    const Vhwa_M2mFcGraphNodeInfo *node);

#ifdef __cplusplus
}
#endif

#endif  /* VHWA_FC_DRV_PRIV_H_ */
