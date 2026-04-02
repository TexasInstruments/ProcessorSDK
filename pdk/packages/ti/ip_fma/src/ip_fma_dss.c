/*
 *  Copyright (c) Texas Instruments Incorporated 2026
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
 *  \ingroup  IP_FMA_DSS
 *  \defgroup IP_FMA_DSS_IMPLEMENTATION Register readback implementation
 *
 *  @{
 */

/**
 *  \file     ip_fma_dss.c
 *
 *  \brief    Register readback implementation for dss static registers.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ip_fma_dss.h>
#include <ti/drv/dss/dss.h>

#include <ti/csl/src/ip/dss/V4/csl_dssTop.h>
#include <ti/drv/dss/soc/V2/dss_soc_priv.h>

#if defined(SOC_J721S2)
#include <ti/csl/soc/j721s2/src/cslr_soc_baseaddress.h>
#elif defined(SOC_J784S4)
#include <ti/csl/soc/j784s4/src/cslr_soc_baseaddress.h>
#endif

/* ========================================================================== */
/*                           File Scope Variables                             */
/* ========================================================================== */

static const Dss_CommRegInfo gDss_commRegInfo[] = {
    {CSL_DSS_COMM_REG_ID_0, (void *)CSL_DSS0_DISPC_0_COMMON_M_BASE},
    {CSL_DSS_COMM_REG_ID_1, (void *)CSL_DSS0_DISPC_0_COMMON_S0_BASE},
    {CSL_DSS_COMM_REG_ID_2, (void *)CSL_DSS0_DISPC_0_COMMON_S1_BASE},
    {CSL_DSS_COMM_REG_ID_3, (void *)CSL_DSS0_DISPC_0_COMMON_S2_BASE}
};

static const Dss_PipeRegInfo gDss_pipeRegInfo[] = {
    {CSL_DSS_VID_PIPE_ID_VID1, (void *)CSL_DSS0_VID1_BASE},
    {CSL_DSS_VID_PIPE_ID_VIDL1, (void *)CSL_DSS0_VIDL1_BASE},
    {CSL_DSS_VID_PIPE_ID_VID2, (void *)CSL_DSS0_VID2_BASE},
    {CSL_DSS_VID_PIPE_ID_VIDL2, (void *)CSL_DSS0_VIDL2_BASE}
};

static const Dss_OverlayRegInfo gDss_overlayRegInfo[] = {
    {CSL_DSS_OVERLAY_ID_1, (void *)CSL_DSS0_OVR1_BASE},
    {CSL_DSS_OVERLAY_ID_2, (void *)CSL_DSS0_OVR2_BASE},
    {CSL_DSS_OVERLAY_ID_3, (void *)CSL_DSS0_OVR3_BASE},
    {CSL_DSS_OVERLAY_ID_4, (void *)CSL_DSS0_OVR4_BASE}
};

static const Dss_VpRegInfo gDss_vpRegInfo[] = {
    {CSL_DSS_VP_ID_1, (void *)CSL_DSS0_VP1_BASE},
    {CSL_DSS_VP_ID_2, (void *)CSL_DSS0_VP2_BASE},
    {CSL_DSS_VP_ID_3, (void *)CSL_DSS0_VP3_BASE},
    {CSL_DSS_VP_ID_4, (void *)CSL_DSS0_VP4_BASE}
};

static const Dss_WbPipeRegInfo gDss_wbRegInfo[] = {
    {CSL_DSS_WB_PIPE_ID_1, (void *)CSL_DSS0_WB_BASE},
};

/* ========================================================================== */
/*                       Static Function Declarations                         */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

IpFma_Status IpFma_Dss_GetCommonMRegs(IpFma_DssCommonMRegs* out, uint32_t instanceId)
{
    IpFma_Status status = IPFMA_OK;
    uintptr_t instanceBaseAddress = 0U;

    if (NULL_PTR == out)
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        IpFma_RegDesc commonRegs[] =
        {
            { CSL_DSS_COMMON_M_DSS_REVISION, 0U, IPFMA_WIDTH_32 },
            { CSL_DSS_COMMON_M_DSS_CBA_CFG,  0U, IPFMA_WIDTH_32 }
        };

        for (uint32_t index = 0U; index < CSL_DSS_COMM_REG_ID_MAX; index++)
        {
            if (gDss_commRegInfo[index].commRegId == instanceId)
            {
                instanceBaseAddress = (uintptr_t) gDss_commRegInfo[index].commRegs;
            }
        }

        status = IpFma_GetRegsValues(instanceBaseAddress, commonRegs, sizeof(commonRegs)/sizeof(IpFma_RegDesc));

        if (IPFMA_OK == status)
        {
            out->dssRevision    = (uint32_t) commonRegs[0].value;
            out->dssCbaCfg      = (uint32_t) commonRegs[1].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_GetCommonSRegs(IpFma_DssCommonSRegs* out, uint32_t instanceId)
{
    IpFma_Status status = IPFMA_OK;
    uintptr_t instanceBaseAddress = 0U;

    if (NULL_PTR == out)
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        IpFma_RegDesc commonRegs[] =
        {
            { CSL_DSS_COMMON_S0_DISPC_IRQSTATUS, 0U, IPFMA_WIDTH_32 },
            { CSL_DSS_COMMON_S0_WB_IRQSTATUS,    0U, IPFMA_WIDTH_32 }
        };

        for (uint32_t index = 0U; index < CSL_DSS_COMM_REG_ID_MAX; index++)
        {
            if (gDss_commRegInfo[index].commRegId == instanceId)
            {
                instanceBaseAddress = (uintptr_t) gDss_commRegInfo[index].commRegs;
            }
        }

        status = IpFma_GetRegsValues(instanceBaseAddress, commonRegs, sizeof(commonRegs)/sizeof(IpFma_RegDesc));

        if (IPFMA_OK == status)
        {
            out->dssDispcIrqStatus  = (uint32_t) commonRegs[0].value;
            out->dssWbIrqStatus     = (uint32_t) commonRegs[1].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_GetVideoPipeRegs(IpFma_DssVideoPipeRegs* out, uint32_t instanceId)
{
    IpFma_Status status = IPFMA_OK;
    uintptr_t instanceBaseAddress = 0U;

    if (NULL_PTR == out)
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        /* Example using BUF_SIZE_STATUS register and 9 FIR_COEF_H0 registers. */
        const uint32_t videoPipeRegsNum = 1U;
        const uint32_t totalNumberOfVideoPipeRegs = IPFMA_DSS_VIDEO_PIPE_FIR_COEF_H0_REG_NUM + videoPipeRegsNum;

        IpFma_RegDesc videoPipeRegs[totalNumberOfVideoPipeRegs];
        videoPipeRegs[0] = (IpFma_RegDesc) { CSL_DSS_VID1_BUF_SIZE_STATUS,  0U,  IPFMA_WIDTH_32 };
        for (uint32_t index = 0U; index < IPFMA_DSS_VIDEO_PIPE_FIR_COEF_H0_REG_NUM; index++)
        {
            videoPipeRegs[index + 1] = (IpFma_RegDesc) { CSL_DSS_VID1_FIR_COEF_H0(index),  0U,  IPFMA_WIDTH_32 };
        }

        for (uint32_t index = 0U; index < CSL_DSS_VID_PIPE_ID_MAX; index++)
        {
            if (gDss_pipeRegInfo[index].pipeId == instanceId)
            {
                instanceBaseAddress = (uintptr_t) gDss_pipeRegInfo[index].pipeRegs;
            }
        }

        status = IpFma_GetRegsValues(instanceBaseAddress, videoPipeRegs, sizeof(videoPipeRegs)/sizeof(IpFma_RegDesc));

        if(IPFMA_OK == status)
        {
            out->dssBufSizeStatus   = (uint32_t) videoPipeRegs[0].value;
            for (uint32_t index = 0U; index < IPFMA_DSS_VIDEO_PIPE_FIR_COEF_H0_REG_NUM; index++)
            {
                out->dssFirCoefH0[index]    = (uint32_t) videoPipeRegs[index + 1].value;
            }
        }
    }

    return status;
}


IpFma_Status IpFma_Dss_GetVideoPipeLayerRegs(IpFma_DssVideoPipeLayerRegs* out, uint32_t instanceId)
{
    IpFma_Status status = IPFMA_OK;
    uintptr_t instanceBaseAddress = 0U;

    if (NULL_PTR == out)
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        IpFma_RegDesc videoPipeLayer[] =
        {
            { CSL_DSS_VIDL1_BUF_SIZE_STATUS,    0U, IPFMA_WIDTH_32 },
            { CSL_DSS_VIDL1_SAFETY_ATTRIBUTES,  0U, IPFMA_WIDTH_32 }
        };

        for (uint32_t index = 0U; index < CSL_DSS_VID_PIPE_ID_MAX; index++)
        {
            if (gDss_pipeRegInfo[index].pipeId == instanceId)
            {
                instanceBaseAddress = (uintptr_t) gDss_pipeRegInfo[index].pipeRegs;
            }
        }

        status = IpFma_GetRegsValues(instanceBaseAddress, videoPipeLayer, sizeof(videoPipeLayer)/sizeof(IpFma_RegDesc));

        if (IPFMA_OK == status)
        {
            out->dssBufSizeStatus       = (uint32_t) videoPipeLayer[0].value;
            out->dssSafetyAttributes    = (uint32_t) videoPipeLayer[1].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_GetOverlayRegs(IpFma_DssOverlayRegs* out, uint32_t instanceId)
{
    IpFma_Status status = IPFMA_OK;
    uintptr_t instanceBaseAddress = 0U;

    if (NULL_PTR == out)
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        IpFma_RegDesc overlayRegs[] = {
            { CSL_DSS_OVR1_CONFIG,      0U, IPFMA_WIDTH_32 },
            { CSL_DSS_OVR1_VIRTUALVP,   0U, IPFMA_WIDTH_32 }
        };

        for (uint32_t index = 0U; index < CSL_DSS_OVERLAY_ID_MAX; index++)
        {
            if (gDss_overlayRegInfo[index].overlayId == instanceId)
            {
                instanceBaseAddress = (uintptr_t) gDss_overlayRegInfo[index].overlayRegs;
            }
        }

        status = IpFma_GetRegsValues(instanceBaseAddress, overlayRegs, sizeof(overlayRegs)/sizeof(IpFma_RegDesc));

        if(IPFMA_OK == status)
        {
            out->dssConfig      = (uint32_t) overlayRegs[0].value;
            out->dssVirtualVp   = (uint32_t) overlayRegs[1].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_GetVideoPortRegs(IpFma_DssVideoPortRegs* out, uint32_t instanceId)
{
    IpFma_Status status = IPFMA_OK;
    uintptr_t instanceBaseAddress = 0U;

    if (NULL_PTR == out)
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        IpFma_RegDesc videoPortRegs[] = {
            { CSL_DSS_VP1_CONFIG,   0U, IPFMA_WIDTH_32 },
            { CSL_DSS_VP1_CONTROL,  0U, IPFMA_WIDTH_32 }
        };

        for (uint32_t index = 0U; index < CSL_DSS_VP_ID_MAX; index++)
        {
            if (gDss_vpRegInfo[index].vpId == instanceId)
            {
                instanceBaseAddress = (uintptr_t) gDss_vpRegInfo[index].vpRegs;
            }
        }

        status = IpFma_GetRegsValues(instanceBaseAddress, videoPortRegs, sizeof(videoPortRegs)/sizeof(IpFma_RegDesc));

        if (IPFMA_OK == status)
        {
            out->dssConfig  = (uint32_t) videoPortRegs[0].value;
            out->dssControl = (uint32_t) videoPortRegs[1].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_GetWriteBackPipeRegs(IpFma_DssWriteBackPipeRegs* out, uint32_t instanceId)
{
    IpFma_Status status = IPFMA_OK;
    uintptr_t instanceBaseAddress = 0U;

    if (NULL_PTR == out)
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        IpFma_RegDesc writeBackPipeRegs[] = {
            { CSL_DSS_WB_ATTRIBUTES, 0U, IPFMA_WIDTH_32 },
            { CSL_DSS_WB_POSITION,   0U, IPFMA_WIDTH_32 }
        };

        for (uint32_t index = 0U; index < CSL_DSS_WB_PIPE_ID_MAX; index++)
        {
            if (gDss_wbRegInfo[index].pipeId == instanceId)
            {
                instanceBaseAddress = (uintptr_t) gDss_wbRegInfo[index].wbPipeRegs;
            }
        }

        status = IpFma_GetRegsValues(instanceBaseAddress, writeBackPipeRegs, sizeof(writeBackPipeRegs)/sizeof(IpFma_RegDesc));

        if (IPFMA_OK == status)
        {
            out->dssAttributes  = (uint32_t) writeBackPipeRegs[0].value;
            out->dssPosition    = (uint32_t) writeBackPipeRegs[1].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_CompareCommonMRegs(const IpFma_DssCommonMRegs* expected, const IpFma_DssCommonMRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == expected) || (NULL_PTR == actual))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if (expected->dssCbaCfg != actual->dssCbaCfg)
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->dssRevision != actual->dssRevision))
        {
            status = IPFMA_E_MISMATCH;
        }
    }

    return status;
}


IpFma_Status IpFma_Dss_CompareCommonSRegs(const IpFma_DssCommonSRegs* expected, const IpFma_DssCommonSRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == expected) || (NULL_PTR == actual))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if (expected->dssDispcIrqStatus != actual->dssDispcIrqStatus)
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->dssWbIrqStatus != actual->dssWbIrqStatus))
        {
            status = IPFMA_E_MISMATCH;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_CompareVideoPipeRegs(const IpFma_DssVideoPipeRegs* expected, const IpFma_DssVideoPipeRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == expected) || (NULL_PTR == actual))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if (expected->dssBufSizeStatus != actual->dssBufSizeStatus)
        {
            status = IPFMA_E_MISMATCH;
        }
        for (uint32_t index = 0U; index < IPFMA_DSS_VIDEO_PIPE_FIR_COEF_H0_REG_NUM; index++)
        {
            if ((IPFMA_OK == status) && (expected->dssFirCoefH0[index] != actual->dssFirCoefH0[index]))
            {
                status = IPFMA_E_MISMATCH;
                break;
            }
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_CompareVideoPipeLayerRegs(const IpFma_DssVideoPipeLayerRegs* expected, const IpFma_DssVideoPipeLayerRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == expected) || (NULL_PTR == actual))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if (expected->dssBufSizeStatus != actual->dssBufSizeStatus)
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->dssSafetyAttributes != actual->dssSafetyAttributes))
        {
            status = IPFMA_E_MISMATCH;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_CompareOverlayRegs(const IpFma_DssOverlayRegs* expected, const IpFma_DssOverlayRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == expected) || (NULL_PTR == actual))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if (expected->dssConfig != actual->dssConfig)
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->dssVirtualVp != actual->dssVirtualVp))
        {
            status = IPFMA_E_MISMATCH;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_CompareVideoPortRegs(const IpFma_DssVideoPortRegs* expected, const IpFma_DssVideoPortRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == expected) || (NULL_PTR == actual))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if (expected->dssConfig != actual->dssConfig)
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->dssControl != actual->dssControl))
        {
            status = IPFMA_E_MISMATCH;
        }
    }

    return status;
}

IpFma_Status IpFma_Dss_CompareWriteBackPipeRegs(const IpFma_DssWriteBackPipeRegs* expected, const IpFma_DssWriteBackPipeRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == expected) || (NULL_PTR == actual))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if (expected->dssAttributes != actual->dssAttributes)
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->dssPosition != actual->dssPosition))
        {
            status = IPFMA_E_MISMATCH;
        }
    }

    return status;
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/** @} */
