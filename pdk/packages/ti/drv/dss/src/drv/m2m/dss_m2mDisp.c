/*
 *  Copyright (c) Texas Instruments Incorporated 2022
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
 *  \file dss_m2mDisp.c
 *
 *  \brief File containing the DSS M2M driver DSS_DISP APIs.
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
uint32_t Dss_m2mDrvDispPipeCfgChk(const Dss_DispParams *instCfg,
                                  const Dss_DispParams *progCfg);

/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
uint32_t Dss_m2mDrvDispMFlagCfgChk(const Dss_DispPipeMflagParams *instCfg,
                                   const Dss_DispPipeMflagParams *progCfg);

/* Returns UTRUE if instance and channel/context configurations matches,
   returns UFALSE otherwise */
uint32_t Dss_m2mDrvDispCscCfgChk(const CSL_DssCscCoeff *instCfg,
                                 const CSL_DssCscCoeff *progCfg);

int32_t Dss_m2mDrvPrgramDisp(DssM2MDrv_VirtContext *context);

/*
 * Display IOCTLs
 */
static int32_t Dss_m2mDrvValidateDispParams(DssM2MDrv_InstObj *instObj,
                                            const Dss_PipeCfgParams *pipeCfg);

int32_t Dss_m2mDrvIoctlSetDssPipeParams(DssM2MDrv_VirtContext *context,
                                        const Dss_PipeCfgParams *pipeCfg);

int32_t Dss_m2mDrvIoctlSetPipeMflagParams(DssM2MDrv_VirtContext *context,
                                  const Dss_PipeMflagParams *mFlagParams);

int32_t Dss_m2mDrvIoctlSetPipeCsc(DssM2MDrv_VirtContext *context,
                                  const Dss_PipeCscParams *csc);

/* Internal functions to check parameters. */
static int32_t Dss_m2mDrvDispCheckFieldMerge(const Fvid2_Format *instCfg, const Fvid2_Format *progCfg, uint32_t scanFmt);

static int32_t Dss_m2mDrvDispCheckPitch(const Fvid2_Format *instCfg, const Fvid2_Format *progCfg, uint32_t scanFmt);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t Dss_m2mDrvDispCheckPitch(const Fvid2_Format *instCfg, const Fvid2_Format *progCfg, uint32_t scanFmt)
{
    uint32_t numFields, fieldIdx, planeIdx;
    int32_t retVal = FVID2_SOK;

    if (FVID2_SF_INTERLACED == scanFmt)
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
            if (instCfg->pitch[fieldIdx*FVID2_MAX_PLANES_PER_FIELD + planeIdx] != progCfg->pitch[fieldIdx*FVID2_MAX_PLANES_PER_FIELD + planeIdx])
            {
                retVal = FVID2_EINVALID_PARAMS;
                break;
            }
        }
    }
    return retVal;
}

int32_t Dss_m2mDrvDispCheckFieldMerge(const Fvid2_Format *instCfg, const Fvid2_Format *progCfg, uint32_t scanFmt)
{
    int32_t retVal = FVID2_SOK;
    uint32_t planeIdx;

    if (FVID2_SF_INTERLACED == scanFmt)
    {
        for (planeIdx = 0U; planeIdx < FVID2_MAX_PLANES_PER_FIELD; planeIdx++)
        {
            if(instCfg->fieldMerged[planeIdx] != progCfg->fieldMerged[planeIdx])
            {
                retVal = FVID2_EINVALID_PARAMS;
                break;
            }
        }
    }
    else
    {
        /* Field merged is not used for Progressive scan format. */
    }
    return retVal;
}

uint32_t Dss_m2mDrvDispPipeCfgChk(const Dss_DispParams *instCfg,
                                  const Dss_DispParams *progCfg)
{
    uint32_t retVal = UTRUE;
    uint32_t loopCnt = 0U, scanFmt;

    scanFmt = instCfg->pipeCfg.inFmt.scanFormat;
    /* check for Pipe configurations */
    if ((instCfg->pipeCfg.pipeType          != progCfg->pipeCfg.pipeType)                                           ||
        (instCfg->pipeCfg.pixelInc          != progCfg->pipeCfg.pixelInc)                                           ||
        (instCfg->pipeCfg.yuvAlign          != progCfg->pipeCfg.yuvAlign)                                           ||
        (instCfg->pipeCfg.outWidth          != progCfg->pipeCfg.outWidth)                                           ||
        (instCfg->pipeCfg.outHeight         != progCfg->pipeCfg.outHeight)                                          ||
        (instCfg->pipeCfg.scEnable          != progCfg->pipeCfg.scEnable)                                           ||
        (instCfg->pipeCfg.cscRange          != progCfg->pipeCfg.cscRange)                                           ||
        (instCfg->pipeCfg.flipType          != progCfg->pipeCfg.flipType)                                           ||
        (instCfg->pipeCfg.nibbleModeEnable  != progCfg->pipeCfg.nibbleModeEnable)                                   ||
        (instCfg->pipeCfg.gammaEnable       != progCfg->pipeCfg.gammaEnable)                                        ||
        (instCfg->pipeCfg.inFmt.width       != progCfg->pipeCfg.inFmt.width)                                        ||
        (instCfg->pipeCfg.inFmt.height      != progCfg->pipeCfg.inFmt.height)                                       ||
        (FVID2_SOK                          != Dss_m2mDrvDispCheckPitch(&(instCfg->pipeCfg.inFmt), &(progCfg->pipeCfg.inFmt), scanFmt))      ||
        (instCfg->pipeCfg.inFmt.dataFormat  != progCfg->pipeCfg.inFmt.dataFormat)                                   ||
        (instCfg->pipeCfg.inFmt.scanFormat  != progCfg->pipeCfg.inFmt.scanFormat)                                   ||
        (instCfg->pipeCfg.inFmt.ccsFormat   != progCfg->pipeCfg.inFmt.ccsFormat)                                    ||
        (FVID2_SOK                          != Dss_m2mDrvDispCheckFieldMerge(&(instCfg->pipeCfg.inFmt), &(progCfg->pipeCfg.inFmt), scanFmt)))
    {
        retVal = UFALSE;
    }
    else
    {
        for (loopCnt = 0U ; loopCnt < CSL_DSS_NUM_LUT_ENTRIES ; loopCnt++)
        {
            if (instCfg->pipeCfg.clutData[loopCnt] !=
                progCfg->pipeCfg.clutData[loopCnt])
            {
                break;
            }
        }
        if (CSL_DSS_NUM_LUT_ENTRIES > loopCnt)
        {
            retVal = UFALSE;
        }
    }
    /* check for DMA configurations */
    if (UTRUE == retVal)
    {
        if ((instCfg->dmaCfg.bufPreloadControl  != progCfg->dmaCfg.bufPreloadControl)  ||
            (instCfg->dmaCfg.preloadVal         != progCfg->dmaCfg.preloadVal)         ||
            (instCfg->dmaCfg.bufLowThreshold    != progCfg->dmaCfg.bufLowThreshold)    ||
            (instCfg->dmaCfg.bufHighThreshold   != progCfg->dmaCfg.bufHighThreshold)   ||
            (instCfg->dmaCfg.selfRefreshControl != progCfg->dmaCfg.selfRefreshControl) ||
            (instCfg->dmaCfg.selfRefreshEnable  != progCfg->dmaCfg.selfRefreshEnable)  ||
            (instCfg->dmaCfg.arbitration        != progCfg->dmaCfg.arbitration))
        {
            retVal = UFALSE;
        }
    }
    /* check for Alpha configurations */
    if (UTRUE == retVal)
    {
        if ((instCfg->alphaCfg.globalAlpha      != progCfg->alphaCfg.globalAlpha)  ||
            (instCfg->alphaCfg.preMultiplyAlpha != progCfg->alphaCfg.preMultiplyAlpha))
        {
            retVal = UFALSE;
        }
    }
    /* check for VC1 configurations */
    if (UTRUE == retVal)
    {
        if ((instCfg->vc1Cfg.vc1Enable != progCfg->vc1Cfg.vc1Enable) ||
            (instCfg->vc1Cfg.rangeY    != progCfg->vc1Cfg.rangeY)    ||
            (instCfg->vc1Cfg.rangeUV   != progCfg->vc1Cfg.rangeUV))
        {
            retVal = UFALSE;
        }
    }
    /* check for Crop configurations */
    if (UTRUE == retVal)
    {
        if ((instCfg->cropParams.cropEnable         != progCfg->cropParams.cropEnable)         ||
            (instCfg->cropParams.cropCfg.cropTop    != progCfg->cropParams.cropCfg.cropTop)    ||
            (instCfg->cropParams.cropCfg.cropBottom != progCfg->cropParams.cropCfg.cropBottom) ||
            (instCfg->cropParams.cropCfg.cropLeft   != progCfg->cropParams.cropCfg.cropLeft)   ||
            (instCfg->cropParams.cropCfg.cropRight  != progCfg->cropParams.cropCfg.cropRight))
        {
            retVal = UFALSE;
        }
    }
    /* check for Layer configurations */
    if (UTRUE == retVal)
    {
        if ((instCfg->layerPos.startX != progCfg->layerPos.startX) ||
            (instCfg->layerPos.startY != progCfg->layerPos.startY))
        {
            retVal = UFALSE;
        }
    }

    return retVal;
}

uint32_t Dss_m2mDrvDispMFlagCfgChk(const Dss_DispPipeMflagParams *instCfg,
                                   const Dss_DispPipeMflagParams *progCfg)
{
    uint32_t retVal = UTRUE;

    if ((instCfg->mflagCfg.thresholdLow  != progCfg->mflagCfg.thresholdLow)  ||
        (instCfg->mflagCfg.thresholdHigh != progCfg->mflagCfg.thresholdHigh))
    {
        retVal = UFALSE;
    }

    return retVal;
}

uint32_t Dss_m2mDrvDispCscCfgChk(const CSL_DssCscCoeff *instCfg,
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

int32_t Dss_m2mDrvPrgramDisp(DssM2MDrv_VirtContext *context)
{
    int32_t retVal = FVID2_SOK;
    uint32_t copyCfg = UFALSE;
    uint32_t layerNum, pipeIdx;
    DssM2MDrv_InstObj *instObj;
    DssM2MDrv_DispPipeCfg *instCfg, *progCfg;
    CSL_DssOverlayPipePosCfg overlayPosCfg;

    instObj = context->instObj;

    for (pipeIdx = 0U ; pipeIdx < context->numPipe ; pipeIdx++)
    {
        instCfg = &context->instCfg.pipeCfg[context->pipeId[pipeIdx]];
        progCfg = &instObj->progCfg.pipeCfg[context->pipeId[pipeIdx]];
        /* Re-program DSS pipe only if channel/instance configurations
           are different than programmed */
        if (FVID2_SOK == retVal)
        {
            /* Check DSS Pipeline configuration */
            if (UFALSE == Dss_m2mDrvDispPipeCfgChk(&instCfg->cfgParams,
                                                               &progCfg->cfgParams))
            {
                if (FVID2_SOK == retVal)
                {
                    /* Program: DSS Pipeline */
                    /* Need to program CSC params only is cscRange is CUSTOM. CSL has prediefined params
                     * for LIMITED and FULL ranges which it uses for programming pipe.
                     */
                    if(CSL_DSS_CSC_RANGE_CUSTOM == instCfg->cfgParams.pipeCfg.cscRange)
                    {
                        Fvid2Utils_memcpy(&instCfg->cfgParams.pipeCfg.custCscCoeff,
                                &progCfg->cfgParams,
                                sizeof(CSL_DssCscCoeff));
                    }

                    retVal = CSL_dssVidPipeSetConfig(instObj->pipeRegs[context->pipeId[pipeIdx]],
                                (const CSL_DssVidPipeCfg *)(&instCfg->cfgParams.pipeCfg),
                                (const CSL_DssVidPipeVC1Cfg *)(&instCfg->cfgParams.vc1Cfg));
                }
                if (FVID2_SOK == retVal)
                {
                    /* Program: DMA, Alpha, Crop, Layer configurations */
                    CSL_dssVidPipeSetDmaConfig(instObj->pipeRegs[context->pipeId[pipeIdx]],
                                (const CSL_DssVidPipeDmaCfg *)(&instCfg->cfgParams.dmaCfg));

                    CSL_dssVidPipeSetAlphaConfig(instObj->pipeRegs[context->pipeId[pipeIdx]],
                                (const CSL_DssVidPipeAlphaCfg *)(&instCfg->cfgParams.alphaCfg));

                    CSL_dssVidPipeSetCropConfig(instObj->pipeRegs[context->pipeId[pipeIdx]],
                            (const Fvid2_EdgeCropConfig *)(&instCfg->cfgParams.cropParams.cropCfg),
                            instCfg->cfgParams.cropParams.cropEnable);

                    layerNum = CSL_dssOverlayGetEnabledPipeLayerNum(instObj->ovrRegs,
                                                                    context->pipeId[pipeIdx]);
                    GT_assert(DssTrace, (CSL_DSS_OVERLAY_LAYER_MAX > layerNum));
                    overlayPosCfg.layerPos.startX = instCfg->cfgParams.layerPos.startX;
                    overlayPosCfg.layerPos.startY = instCfg->cfgParams.layerPos.startY;
                    CSL_dssOverlaySetPipePosConfig(instObj->ovrRegs,
                                (const CSL_DssOverlayPipePosCfg *)(&overlayPosCfg),
                                layerNum);
                }
                if (FVID2_SOK == retVal)
                {
                    copyCfg = UTRUE;
                }
            }
        }
        if (FVID2_SOK == retVal)
        {
            /* Check DSS Pipeline MFlag configuration */
            if (UFALSE == Dss_m2mDrvDispMFlagCfgChk(&instCfg->mFlagParams,
                                                                &progCfg->mFlagParams))
            {
                if (FVID2_SOK == retVal)
                {
                    /* Program: DSS Pipeline MFlag */
                    CSL_dssVidPipeSetMflagConfig(instObj->pipeRegs[context->pipeId[pipeIdx]],
                        (const CSL_DssVidPipeMFlagCfg *)(&instCfg->mFlagParams.mflagCfg));
                    copyCfg = UTRUE;
                }
            }
        }
        if (FVID2_SOK == retVal)
        {
            /* Check DSS Pipeline Csc configuration */
            if (UFALSE == Dss_m2mDrvDispCscCfgChk(&instCfg->cscCoeff,
                                                              &progCfg->cscCoeff))
            {
                if (FVID2_SOK == retVal)
                {
                    /* Program: DSS Pipeline Csc */
                    CSL_dssVidPipeSetCSCCoeff(instObj->pipeRegs[context->pipeId[pipeIdx]],
                            (const CSL_DssCscCoeff *)(&instCfg->cscCoeff));
                    copyCfg = UTRUE;
                }
            }
        }
        if ((FVID2_SOK == retVal) && (UTRUE == copyCfg))
        {
            /* Update instance configurations */
            Fvid2Utils_memcpy(progCfg, instCfg, sizeof(DssM2MDrv_DispPipeCfg));
        }
    }

    return retVal;
}

static int32_t Dss_m2mDrvValidateDispParams(DssM2MDrv_InstObj *instObj,
                                            const Dss_PipeCfgParams *pipeCfg)
{
    int32_t retVal = FVID2_SOK;
    const Dss_DispParams *dispParams;

    dispParams = &pipeCfg->cfgParams;
    /* Not Checked: Interlaced to progressive or vice versa, not supported in M2M */
    /* Scaling ratio check */
    if(((dispParams->pipeCfg.inFmt.height * 16U) <
                                        (dispParams->pipeCfg.outHeight)) ||
       ((dispParams->pipeCfg.inFmt.width * 16U) <
                                        (dispParams->pipeCfg.outWidth)))
    {
        GT_0trace(DssTrace,
                  GT_ERR,
                  "M2M: Upscaling ratio is more than 16x \r\n");
        retVal = FVID2_EINVALID_PARAMS;
    }
    if(((dispParams->pipeCfg.inFmt.height) >
                                    (dispParams->pipeCfg.outHeight * 4U)) ||
       ((dispParams->pipeCfg.inFmt.width) >
                                    (dispParams->pipeCfg.outWidth * 4U)))
    {
        GT_0trace(DssTrace,
                  GT_ERR,
                  "M2M: Downscaling ratio is more than 0.25x \r\n");
        retVal = FVID2_EINVALID_PARAMS;
    }

    if((dispParams->pipeCfg.inFmt.height != dispParams->pipeCfg.outHeight) ||
       (dispParams->pipeCfg.inFmt.width != dispParams->pipeCfg.outWidth))
    {
        if(UTRUE == Dss_dispIsVidLInst(pipeCfg->pipeId))
        {
            GT_0trace(DssTrace,
                      GT_ERR,
                      "M2M: Scaling cant be enabled for video lite pipe \r\n");
            retVal = FVID2_EINVALID_PARAMS;
        }
        if(UFALSE == dispParams->pipeCfg.scEnable)
        {
            GT_0trace(DssTrace,
                      GT_ERR,
                      "M2M: Scaling should be enabled \r\n");
            retVal = FVID2_EINVALID_PARAMS;
        }
    }

    if((32U <= dispParams->cropParams.cropCfg.cropTop   ) ||
       (32U <= dispParams->cropParams.cropCfg.cropBottom) ||
       (32U <= dispParams->cropParams.cropCfg.cropLeft  ) ||
       (32U <= dispParams->cropParams.cropCfg.cropRight ))
    {
        GT_0trace(DssTrace,
                  GT_ERR,
                  "M2M: Crop Parameter(s) should be less than 32 \r\n");
        retVal = FVID2_EINVALID_PARAMS;
    }

    if(((FVID2_FLIP_TYPE_V  == dispParams->pipeCfg.flipType) ||
        (FVID2_FLIP_TYPE_H  == dispParams->pipeCfg.flipType)) &&
       ((FVID2_DF_RGB24_888 == dispParams->pipeCfg.inFmt.dataFormat) ||
        (FVID2_DF_BGR24_888 == dispParams->pipeCfg.inFmt.dataFormat)))
    {
        GT_0trace(DssTrace,
                  GT_ERR,
                  "M2M: Flip Parameter(s) should not be used for 24-bit RGB/BGR formats \r\n");
        retVal = FVID2_EINVALID_PARAMS;
    }

    /* There is a limitation with scaling in DSS, where maximum scaling ratio
     * supported is the ratio of the DSS functional clock and Pixel Clock.
     * Driver can't check this condition as it is unaware of Pixel Clock.
     * So it should be checked by the application.
     */

    return retVal;
}

int32_t Dss_m2mDrvIoctlSetDssPipeParams(DssM2MDrv_VirtContext *context,
                                        const Dss_PipeCfgParams *pipeCfg)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == pipeCfg)
    {
        retVal = FVID2_EBADARGS;
    }
    else if (CSL_DSS_VID_PIPE_ID_MAX <= pipeCfg->pipeId)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        retVal = Dss_m2mDrvValidateDispParams(context->instObj, pipeCfg);
        if (FVID2_SOK == retVal)
        {
            /* upgrade configurations into context object and re-program HW module
               on buffer submission */
            Fvid2Utils_memcpy(&context->instCfg.pipeCfg[pipeCfg->pipeId].cfgParams,
                              &pipeCfg->cfgParams,
                              sizeof(Dss_DispParams));
        }
    }

    return retVal;
}

int32_t Dss_m2mDrvIoctlSetPipeMflagParams(DssM2MDrv_VirtContext *context,
                                  const Dss_PipeMflagParams *mFlagParams)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == mFlagParams)
    {
        retVal = FVID2_EBADARGS;
    }
    else if (CSL_DSS_VID_PIPE_ID_MAX <= mFlagParams->pipeId)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* upgrade configurations into context object and re-program HW module
           on buffer submission */
        Fvid2Utils_memcpy(&context->instCfg.pipeCfg[mFlagParams->pipeId].mFlagParams,
                          &mFlagParams->mFlagCfg,
                          sizeof(Dss_DispPipeMflagParams));
    }

    return retVal;
}

int32_t Dss_m2mDrvIoctlSetPipeCsc(DssM2MDrv_VirtContext *context,
                                  const Dss_PipeCscParams *csc)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == csc)
    {
        retVal = FVID2_EBADARGS;
    }
    else if (CSL_DSS_VID_PIPE_ID_MAX <= csc->pipeId)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* upgrade configurations into context object and re-program HW module
           on buffer submission */
        Fvid2Utils_memcpy(&context->instCfg.pipeCfg[csc->pipeId].cscCoeff,
                          &csc->csc,
                          sizeof(CSL_DssCscCoeff));
    }

    return retVal;
}
