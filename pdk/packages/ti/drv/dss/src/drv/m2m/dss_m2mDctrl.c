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
 *  \file dss_m2mDctrl.c
 *
 *  \brief File containing the DSS M2M driver DSS_DCTRL APIs.
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

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
uint32_t Dss_m2mDrvDctrlOvlCfgChk(const Dss_DctrlOverlayParams *instCfg,
                                  const Dss_DctrlOverlayParams *progCfg);

/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
uint32_t Dss_m2mDrvDctrlOvlLayerCfgChk(const Dss_DctrlOverlayLayerParams *instCfg,
                                       const Dss_DctrlOverlayLayerParams *progCfg);

/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
uint32_t Dss_m2mDrvDctrlGlobalCfgChk(const Dss_DctrlGlobalDssParams *instCfg,
                                     const Dss_DctrlGlobalDssParams *progCfg);

/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
uint32_t Dss_m2mDrvDctrlPathCfgChk(const Dss_DctrlPathInfo * instCfg,
                                   const Dss_DctrlPathInfo * progCfg);

int32_t Dss_m2mDrvPrgramDctrl(DssM2MDrv_VirtContext *context);

/*
 * Display IOCTLs
 */
int32_t Dss_m2mDrvIoctlSetOverlayParams(DssM2MDrv_VirtContext *context,
                                        const Dss_DctrlOverlayParams *ovlParams);

int32_t Dss_m2mDrvIoctlSetLayerParams(DssM2MDrv_VirtContext *context,
                        const Dss_DctrlOverlayLayerParams *ovlLayerParams);

int32_t Dss_m2mDrvIoctlSetGlobalParams(DssM2MDrv_VirtContext *context,
                               const Dss_DctrlGlobalDssParams *globalParams);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
extern DssM2MDrv_CommonObj gDssM2mCommonObj;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
uint32_t Dss_m2mDrvDctrlOvlCfgChk(const Dss_DctrlOverlayParams *instCfg,
                                         const Dss_DctrlOverlayParams *progCfg)
{
    uint32_t retVal = UTRUE;

    if ((instCfg->overlayId                   != progCfg->overlayId)                   ||
        (instCfg->colorbarEnable              != progCfg->colorbarEnable)              ||
        (UFALSE                               != instCfg->colorbarEnable)              ||
        (instCfg->overlayCfg.colorKeyEnable   != progCfg->overlayCfg.colorKeyEnable)   ||
        (instCfg->overlayCfg.colorKeySel      != progCfg->overlayCfg.colorKeySel)      ||
        (instCfg->overlayCfg.transColorKeyMin != progCfg->overlayCfg.transColorKeyMin) ||
        (instCfg->overlayCfg.transColorKeyMax != progCfg->overlayCfg.transColorKeyMax) ||
        (instCfg->overlayCfg.backGroundColor  != progCfg->overlayCfg.backGroundColor))
    {
        retVal = UFALSE;
    }

    return retVal;
}

uint32_t Dss_m2mDrvDctrlOvlLayerCfgChk(const Dss_DctrlOverlayLayerParams *instCfg,
                                       const Dss_DctrlOverlayLayerParams *progCfg)
{
    uint32_t retVal = UTRUE;
    uint32_t loopCnt = 0U;

    if ((instCfg->overlayId  != progCfg->overlayId))
    {
        retVal = UFALSE;
    }
    else
    {
        for (loopCnt = 0U ; loopCnt < CSL_DSS_VID_PIPE_ID_MAX ; loopCnt++)
        {
            if (instCfg->pipeLayerNum[loopCnt] != progCfg->pipeLayerNum[loopCnt])
            {
                break;
            }
        }
        if (CSL_DSS_VID_PIPE_ID_MAX > loopCnt)
        {
            retVal = UFALSE;
        }
    }

    return retVal;
}

uint32_t Dss_m2mDrvDctrlGlobalCfgChk(const Dss_DctrlGlobalDssParams *instCfg,
                                     const Dss_DctrlGlobalDssParams *progCfg)
{
    uint32_t retVal = UTRUE;

    if ((instCfg->globalMflagCfg.globalMflagStart  != progCfg->globalMflagCfg.globalMflagStart)  ||
        (instCfg->globalMflagCfg.globalMflagCtrl   != progCfg->globalMflagCfg.globalMflagCtrl)   ||
        (instCfg->cbaCfg.priHigh                   != progCfg->cbaCfg.priHigh)                   ||
        (instCfg->cbaCfg.priLow                    != progCfg->cbaCfg.priLow))
    {
        retVal = UFALSE;
    }

    return retVal;
}

uint32_t Dss_m2mDrvDctrlPathCfgChk(const Dss_DctrlPathInfo * instCfg,
                                   const Dss_DctrlPathInfo * progCfg)
{
    uint32_t retVal = UTRUE;
    uint32_t loopCnt = 0U;

    if ((instCfg->numEdges  != progCfg->numEdges))
    {
        retVal = UFALSE;
    }
    else
    {
        for (loopCnt = 0U ; loopCnt < DSS_DCTRL_MAX_EDGES ; loopCnt++)
        {
            if ((instCfg->edgeInfo[loopCnt].startNode !=
                                        progCfg->edgeInfo[loopCnt].startNode) ||
               (instCfg->edgeInfo[loopCnt].endNode   !=
                                        progCfg->edgeInfo[loopCnt].endNode))
            {
                break;
            }
        }
        if (DSS_DCTRL_MAX_EDGES > loopCnt)
        {
            retVal = UFALSE;
        }
    }

    return retVal;
}

int32_t Dss_m2mDrvPrgramDctrl(DssM2MDrv_VirtContext *context)
{
    int32_t retVal = FVID2_SOK;
    uint32_t copyCfg = UFALSE;
    uint32_t loopCnt = 0U;
    DssM2MDrv_InstObj *instObj;
    DssM2MDrv_dctrlCfg *instCfg, *progCfg;
    CSL_DssOverlayLayerCfg layerCfg;

    instObj = context->instObj;
    instCfg = &context->instCfg.dctrlCfg;
    progCfg = &instObj->progCfg.dctrlCfg;
    GT_assert(DssTrace, (NULL != instObj->ovrRegs));

    /* Re-program DCTRL only if channel/instance configurations
       are different than programmed */
    if (FVID2_SOK == retVal)
    {
        /* Check DCTRL Overlay configuration */
        if (UFALSE == Dss_m2mDrvDctrlOvlCfgChk(&instCfg->ovlParams,
                                                           &progCfg->ovlParams))
        {
            if ((CSL_DSS_OVERLAY_ID_MAX <= instCfg->ovlParams.overlayId) ||
                (instCfg->ovlParams.overlayId != instObj->overlayId))
            {
                GT_0trace(DssTrace, GT_ERR, "Invalid argument: Overlay ID\r\n");
                retVal = FVID2_EBADARGS;
            }
            if (FVID2_SOK == retVal)
            {
                /* Always Disable Color-bar */
                CSL_dssOverlayColorBarEnable(instObj->ovrRegs, UFALSE);
                CSL_dssOverlaySetConfig(instObj->ovrRegs,
                                        &instCfg->ovlParams.overlayCfg);
                copyCfg = UTRUE;
            }
        }
    }
    if (FVID2_SOK == retVal)
    {
        /* Check DCTRL Overlay Layer configuration */
        if (UFALSE == Dss_m2mDrvDctrlOvlLayerCfgChk(&instCfg->ovlLayerParams,
                                                                &progCfg->ovlLayerParams))
        {
            if ((CSL_DSS_OVERLAY_ID_MAX <= instCfg->ovlLayerParams.overlayId) ||
                (instCfg->ovlLayerParams.overlayId != instObj->overlayId))
            {
                GT_0trace(DssTrace, GT_ERR, "Invalid argument: Overlay ID(Layer)\r\n");
                retVal = FVID2_EBADARGS;
            }
            if (FVID2_SOK == retVal)
            {
                /* Initialize layer configuration */
                CSL_dssOverlayLayerCfgInit(&layerCfg);
                /* Call CSL APIs */
                for(loopCnt = 0U ; loopCnt < CSL_DSS_VID_PIPE_ID_MAX ; loopCnt++)
                {
                    if(CSL_DSS_OVERLAY_LAYER_INVALID != instCfg->ovlLayerParams.pipeLayerNum[loopCnt])
                    {
                        layerCfg.layerEnable = UTRUE;
                        layerCfg.layerNum = instCfg->ovlLayerParams.pipeLayerNum[loopCnt];
                        layerCfg.inputPipe = loopCnt;
                        CSL_dssOverlaySetLayerConfig(
                                            instObj->ovrRegs,
                                            (const CSL_DssOverlayLayerCfg *) &layerCfg);
                    }
                }
                copyCfg = UTRUE;
            }
        }
    }
    if (FVID2_SOK == retVal)
    {
        /* Check DCTRL Global configuration */
        if (UFALSE == Dss_m2mDrvDctrlGlobalCfgChk(&instCfg->globalParams,
                                                              &progCfg->globalParams))
        {
            CSL_dssSetGlobalMflagConfig(instObj->commRegs,
                                &instCfg->globalParams.globalMflagCfg);
            CSL_dssSetCbaConfig(instObj->commRegs,
                                &instCfg->globalParams.cbaCfg);
            copyCfg = UTRUE;
        }
    }
    if ((FVID2_SOK == retVal) && (UTRUE == copyCfg))
    {
        /* Update instance configurations */
        Fvid2Utils_memcpy(progCfg, instCfg, sizeof(DssM2MDrv_dctrlCfg));
    }

    return retVal;
}

int32_t Dss_m2mDrvIoctlSetOverlayParams(DssM2MDrv_VirtContext *context,
                                        const Dss_DctrlOverlayParams *ovlParams)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == ovlParams)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        if ((CSL_DSS_OVERLAY_ID_MAX <= ovlParams->overlayId) ||
            (ovlParams->overlayId != context->instObj->overlayId))
        {
            retVal = FVID2_EBADARGS;
            GT_0trace(DssTrace, GT_ERR, "Invalid argument: Overlay ID\r\n");
        }
        else
        {
            /* upgrade configurations into context object and re-program HW module
               on buffer submission */
            Fvid2Utils_memcpy(&context->instCfg.dctrlCfg.ovlParams,
                              ovlParams,
                              sizeof(Dss_DctrlOverlayParams));
        }
    }

    return retVal;
}

int32_t Dss_m2mDrvIoctlSetLayerParams(DssM2MDrv_VirtContext *context,
                            const Dss_DctrlOverlayLayerParams *ovlLayerParams)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == ovlLayerParams)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        if ((CSL_DSS_OVERLAY_ID_MAX <= ovlLayerParams->overlayId) ||
            (ovlLayerParams->overlayId != context->instObj->overlayId))
        {
            retVal = FVID2_EBADARGS;
            GT_0trace(DssTrace, GT_ERR, "Invalid argument: Overlay ID\r\n");
        }
        else
        {
            /* upgrade configurations into context object and re-program HW module
               on buffer submission */
            Fvid2Utils_memcpy(&context->instCfg.dctrlCfg.ovlLayerParams,
                              ovlLayerParams,
                              sizeof(Dss_DctrlOverlayLayerParams));
        }
    }

    return retVal;
}

int32_t Dss_m2mDrvIoctlSetGlobalParams(DssM2MDrv_VirtContext *context,
                               const Dss_DctrlGlobalDssParams *globalParams)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == globalParams)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* upgrade configurations into context object and re-program HW module
           on buffer submission */
        Fvid2Utils_memcpy(&context->instCfg.dctrlCfg.globalParams,
                          globalParams,
                          sizeof(Dss_DctrlGlobalDssParams));
    }

    return retVal;
}
