/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 * \file  enet_hostport_udma.c
 *
 * \brief This file contains the implementation of the peripheral specific
          APIs for UDMA interface */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x104

#include <stdint.h>
#include <stdarg.h>

#include <string.h>
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/include/core/enet_base.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/core/enet_per.h>
#include <ti/drv/enet/include/core/enet_osal.h>
#include <ti/drv/enet/priv/core/enet_base_priv.h>
#include <ti/drv/enet/priv/core/enet_trace_priv.h>
#include <ti/drv/enet/include/common/enet_utils_dflt.h>
#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/priv/mod/cpsw_ale_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_cpts_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_hostport_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_macport_priv.h>
#include <ti/drv/enet/priv/mod/mdio_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_stats_priv.h>
#include <ti/drv/enet/priv/core/enet_rm_priv.h>
#include <ti/drv/enet/priv/per/cpsw_priv.h>
#include <ti/drv/enet/priv/per/enet_hostport_udma.h>

#include <ti/drv/enet/src/dma/udma/enet_udma_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static uint32_t EnetHostPortDma_getTotalRxFlowCount(const EnetRm_ResPrms *resPrms);

static int32_t EnetHostPortDma_openRxCh(EnetDma_Handle hDma,
                                        const EnetDma_Cfg *dmaCfg,
                                        const EnetRm_ResCfg *resCfg);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void EnetHostPortDma_initCfg(const EnetDma_Cfg *dmaCfg)
{
    EnetUdma_initCfg((EnetDma_Cfg *)dmaCfg);
}

EnetDma_Handle EnetHostPortDma_open(EnetPer_Handle hPer,
                                    const EnetDma_Cfg *udmaCfg,
                                    const EnetRm_ResCfg *resCfg)
{
    int32_t status = ENET_EFAIL;
    EnetDma_Handle hDma = NULL;

    hDma = EnetUdma_open(hPer->enetType, hPer->instId, udmaCfg);
    ENETTRACE_ERR_IF(NULL == hDma, status, "Failed to open Enet DMA");

    if (NULL != hDma)
    {
        hDma->hPer = hPer;

        status = EnetHostPortDma_openRxCh(hDma, udmaCfg, resCfg);
        if (ENET_SOK != status)
        {
            ENETTRACE_ERR(status, "Failed to open Enet DMA RX channel");
            status = EnetUdma_close(hDma);
            ENETTRACE_ERR_IF((status != ENET_SOK), status, "Failed to close Enet DMA");
            Enet_assert(status == ENET_SOK);
            hDma->hPer = NULL;
            hDma = NULL;
        }
    }

    return hDma;
}

static int32_t EnetHostPortDma_openRxCh(EnetDma_Handle hDma,
                                        const EnetDma_Cfg *udmaCfg,
                                        const EnetRm_ResCfg *resCfg)
{
    int32_t status = ENET_EFAIL;
    uint32_t i;

    if (NULL != hDma)
    {
        uint32_t totalRxFlowCount;

        /* Compute the total number of rx flows required for the Rx channel
         * by summing up the total number of rx flows allocated for the
         * different cores in the CPSW RM resCfg
         */
        totalRxFlowCount = EnetHostPortDma_getTotalRxFlowCount(&resCfg->resPartInfo);

        /* Open & start channel immediately as we have not opened default flow
         * UDMA should not get any packets in CPSW channel */
        for (i = 0U; i < hDma->numRxCh; i++)
        {
            status = EnetUdma_openRxCh(hDma,
                                       &udmaCfg->rxChInitPrms,
                                       totalRxFlowCount,
                                       i);
        }
    }

    return status;
}

void EnetHostPortDma_close(EnetDma_Handle hDma)
{
    uint32_t i;
    int32_t status;

    if (hDma->initFlag)
    {
        /* Stop and check RX channel */
        for (i = 0U; i < hDma->numRxCh; i++)
        {
            status = EnetUdma_closeRxCh(hDma, i);
            Enet_assert(status == ENET_SOK);
        }
    }
    if (hDma->initFlag)
    {
        /* Close dma module */
        status = EnetUdma_close(hDma);
        Enet_assert(status == ENET_SOK);
    }
}

void EnetHostPortDma_getDmaResInfo(EnetDma_Handle hDma,
                                   Enet_dmaResInfo *dmaResInfo,
                                   uint32_t chIdx)
{
    dmaResInfo->rxStartIdx = EnetUdma_getRxChFlowStartIdx(hDma, chIdx);
    dmaResInfo->rxIdxCnt   = EnetUdma_getRxFlowCnt(hDma, chIdx);
}

uint32_t EnetHostPortDma_getTotalRxFlowCount(const EnetRm_ResPrms *resPrms)
{
    uint32_t i;
    uint32_t rxFlowCount;

    rxFlowCount = 0;
    for (i = 0; i < resPrms->numCores; i++)
    {
        rxFlowCount += resPrms->coreResInfo[i].numRxFlows;
    }

    return rxFlowCount;
}

EnetDma_RxChHandle EnetHostPortDma_openRsvdFlow(const EnetDma_Cfg *dmaCfg,
                                                uint32_t startIdx,
                                                uint32_t flowIdx)
{
    EnetDma_RxChHandle hRxRsvdFlow;
    EnetUdma_RsvdRxFlowPrms rxFlowCfg;

    rxFlowCfg.startIdx = startIdx;
    rxFlowCfg.flowIdx  = flowIdx;
    rxFlowCfg.hUdmaDrv = dmaCfg->hUdmaDrv;

    hRxRsvdFlow = EnetUdma_openRxRsvdFlow(&rxFlowCfg);

    return hRxRsvdFlow;
}

int32_t EnetHostPortDma_closeRsvdFlow(EnetDma_RxChHandle hRxRsvdFlow)
{
    int32_t status = ENET_EFAIL;

    if (hRxRsvdFlow != NULL)
    {
        status = EnetUdma_closeRxRsvdFlow(hRxRsvdFlow);
    }

    return status;

}

