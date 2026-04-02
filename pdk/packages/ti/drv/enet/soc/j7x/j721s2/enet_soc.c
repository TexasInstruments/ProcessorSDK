/*
 *  Copyright (c) Texas Instruments Incorporated 2021-2024
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
 * \file  enet_soc.c
 *
 * \brief This file contains the implementation of J721S2 SoC Ethernet.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x404

#include <stdint.h>
#include <stdarg.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_cpswitch.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/ipc/ipc.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/priv/mod/cpsw_ale_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_cpts_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_hostport_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_macport_priv.h>
#include <ti/drv/enet/priv/mod/mdio_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_stats_priv.h>
#include <ti/drv/enet/priv/core/enet_rm_priv.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_osal.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/core/enet_per.h>
#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/priv/per/cpsw_priv.h>
#include <ti/drv/enet/soc/j7x/cpsw_soc.h>
#include <ti/drv/enet/src/dma/udma/enet_udma_priv.h>

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

static uint32_t EnetSoc_getCoreDevId(void);

static uint32_t EnetSoc_getMcuEnetControl(Enet_MacPort macPort,
                                          uint32_t *modeSel);

static uint32_t EnetSoc_getMainEnetControl(Enet_MacPort macPort,
                                           uint32_t *modeSel);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* -------------------------------- CPSW 2G --------------------------------- */

CpswSoc_Cfg gEnetSoc_cpsw2gSocCfg =
{
    /* As per the clocking specification the mcu_sysclk0 is 1000MHz with
     * fixed /3 divider. */
    .cppiClkFreqHz = 333333333LLU,
    .dmscDevId = TISCI_DEV_MCU_CPSW0,
    .intrs =
    {
#if defined(BUILD_MCU1_0)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_MCU_R5FSS0_CORE0_INTR_MCU_CPSW0_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_MCU_R5FSS0_CORE0_INTR_MCU_CPSW0_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_MCU_R5FSS0_CORE0_INTR_MCU_CPSW0_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#elif defined(BUILD_MCU2_0)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_R5FSS0_CORE0_INTR_MCU_CPSW0_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_R5FSS0_CORE0_INTR_MCU_CPSW0_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_R5FSS0_CORE0_INTR_MCU_CPSW0_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#elif defined(BUILD_MCU2_1)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_R5FSS0_CORE1_INTR_MCU_CPSW0_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_R5FSS0_CORE1_INTR_MCU_CPSW0_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_R5FSS0_CORE1_INTR_MCU_CPSW0_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#elif defined(BUILD_MCU3_0)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_R5FSS1_CORE0_INTR_MCU_CPSW0_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_R5FSS1_CORE0_INTR_MCU_CPSW0_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_R5FSS1_CORE0_INTR_MCU_CPSW0_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#elif defined(BUILD_MPU1_0)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_MCU_CPSW0_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_MCU_CPSW0_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_MCU_CPSW0_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#else
#error "Enet J721S2 SOC: Core not supported!!"
#endif
    },
    .clocks =
    {
        .cppiClkId        = TISCI_DEV_MCU_CPSW0_CPPI_CLK_CLK,
        .rgmii250MHzClkId = TISCI_DEV_MCU_CPSW0_RGMII_MHZ_250_CLK,
        .rgmii50MHzClkId  = TISCI_DEV_MCU_CPSW0_RGMII_MHZ_50_CLK,
        .rgmii5MHzClkId   = TISCI_DEV_MCU_CPSW0_RGMII_MHZ_5_CLK,
    },
    .txChPeerThreadId = CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILD_THREAD_OFFSET,
    .rxChPeerThreadId = CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILS_THREAD_OFFSET,
    .txChCount        = CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILD_THREAD_CNT,

    /* Note- Though CPSW supports 64 distinct flow Ids, there are only
     * 8 policer/classifier so can effectively assign only 8 flow ids in CPSW2G */
    .rxFlowCount     = 8U,
};

/* CPSW_2G MAC ports */
CpswMacPort_Obj gEnetSoc_cpsw2gMacObj[] =
{
    {
        .enetMod =
        {
            .name       = "cpsw2g.macport1",
            .physAddr   = (CSL_MCU_CPSW0_NUSS_BASE + CPSW_NU_OFFSET),
            .physAddr2  = (CSL_MCU_CPSW0_NUSS_BASE + CPSW_SGMII0_OFFSET(0U)),
            .features   = (ENET_FEAT_BASE |
                           CPSW_MACPORT_FEATURE_EST |
                           CPSW_MACPORT_FEATURE_SGMII |
                           CPSW_MACPORT_FEATURE_IET),
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswMacPort_open,
            .rejoin     = &CpswMacPort_rejoin,
            .ioctl      = &CpswMacPort_ioctl,
            .close      = &CpswMacPort_close,
        },
        .macPort = ENET_MAC_PORT_1,
    },
};

/* CPSW 2G Peripheral */
Cpsw_Obj gEnetSoc_cpsw2g =
{
    .enetPer =
    {
        .name         = "cpsw2g",
        .enetType     = ENET_CPSW_2G,
        .instId       = 0U,
        .magic        = ENET_NO_MAGIC,
        .physAddr     = (CSL_MCU_CPSW0_NUSS_BASE + CPSW_NU_OFFSET),
        .physAddr2    = (CSL_MCU_CPSW0_NUSS_BASE + CPSW_NUSS_OFFSET),
        .features     = (ENET_FEAT_BASE |
                         CPSW_FEATURE_EST |
                         CPSW_FEATURE_INTERVLAN |
                         CPSW_FEATURE_IET),
        .errata       = ENET_ERRATA_NONE,
        .initCfg      = &Cpsw_initCfg,
        .open         = &Cpsw_open,
        .rejoin       = &Cpsw_rejoin,
        .ioctl        = &Cpsw_ioctl,
        .poll         = &Cpsw_poll,
        .convertTs    = NULL,
        .periodicTick = &Cpsw_periodicTick,
        .registerEventCb   = NULL,
        .unregisterEventCb = NULL,
        .close        = &Cpsw_close,
    },

    /* Host port module object */
    .hostPortObj =
    {
        .enetMod =
        {
            .name       = "cpsw2g.hostport",
            .physAddr   = (CSL_MCU_CPSW0_NUSS_BASE + CPSW_NU_OFFSET),
            .features   = ENET_FEAT_BASE,
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswHostPort_open,
            .rejoin     = &CpswHostPort_rejoin,
            .ioctl      = &CpswHostPort_ioctl,
            .close      = &CpswHostPort_close,
        }
    },

    /* MAC port module objects */
    .macPortObj = gEnetSoc_cpsw2gMacObj,
    .macPortNum = ENET_ARRAYSIZE(gEnetSoc_cpsw2gMacObj),

    /* ALE module object */
    .aleObj =
    {
        .enetMod =
        {
            .name       = "cpsw2g.ale",
            .physAddr   = (CSL_MCU_CPSW0_NUSS_BASE + CPSW_ALE_OFFSET),
            .features   = (ENET_FEAT_BASE |
                           CPSW_ALE_FEATURE_FLOW_PRIORITY |
                           CPSW_ALE_FEATURE_IP_HDR_WHITELIST),
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswAle_open,
            .rejoin     = &CpswAle_rejoin,
            .ioctl      = &CpswAle_ioctl,
            .close      = &CpswAle_close,
        },
    },

    /* CPTS module object */
    .cptsObj =
    {
        .enetMod =
        {
            .name       = "cpsw2g.cpts",
            .physAddr   = (CSL_MCU_CPSW0_NUSS_BASE + CPSW_CPTS_OFFSET),
            .features   = ENET_FEAT_BASE,
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswCpts_open,
            .rejoin     = &CpswCpts_rejoin,
            .ioctl      = &CpswCpts_ioctl,
            .close      = &CpswCpts_close,
        },
        .hwPushCnt      = 4U,
    },

    /* MDIO module object */
    .mdioObj =
    {
        .enetMod =
        {
            .name       = "cpsw2g.mdio",
            .physAddr   = (CSL_MCU_CPSW0_NUSS_BASE + CPSW_MDIO_OFFSET),
            .features   = (ENET_FEAT_BASE |
                           MDIO_FEATURE_CLAUSE45),
            .errata     = ENET_ERRATA_NONE,
            .open       = &Mdio_open,
            .rejoin     = &Mdio_rejoin,
#if defined(ENABLE_MDIO_MANUAL_MODE)
            .ioctl      = &Mdio_ioctlManualMode,
#else
            .ioctl      = &Mdio_ioctl,
#endif
            .close      = &Mdio_close,
        },
    },

    /* Statistics module object */
    .statsObj =
    {
        .enetMod =
        {
            .name       = "cpsw2g.stats",
            .physAddr   = (CSL_MCU_CPSW0_NUSS_BASE + CPSW_NU_OFFSET),
            .features   = ENET_FEAT_BASE,
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswStats_open,
            .rejoin     = &CpswStats_rejoin,
            .ioctl      = &CpswStats_ioctl,
            .close      = &CpswStats_close,
        },
    },

    /* RM module object */
    .rmObj =
    {
        .enetMod =
        {
            .name       = "cpsw2g.rm",
            .physAddr   = 0U,
            .features   = ENET_FEAT_BASE,
            .errata     = ENET_ERRATA_NONE,
            .open       = &EnetRm_open,
            .rejoin     = &EnetRm_rejoin,
            .ioctl      = &EnetRm_ioctl,
            .close      = &EnetRm_close,
        },
    },
};

/* -------------------------------- MAIN CPSW 2G --------------------------------- */

CpswSoc_Cfg gEnetSoc_cpsw2gMainSocCfg =
{
    .cppiClkFreqHz = 320000000U, /* 320 MHz */
    .dmscDevId = TISCI_DEV_CPSW1,
    .intrs =
    {
#if defined(BUILD_MCU1_0)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_MAIN2MCU_LVL_INTRTR0_IN_CPSW1_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_MAIN2MCU_LVL_INTRTR0_IN_CPSW1_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_MAIN2MCU_LVL_INTRTR0_IN_CPSW1_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#elif defined(BUILD_MCU2_0)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_R5FSS0_CORE0_INTR_CPSW1_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_R5FSS0_CORE0_INTR_CPSW1_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_R5FSS0_CORE0_INTR_CPSW1_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#elif defined(BUILD_MCU2_1)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_R5FSS0_CORE1_INTR_CPSW1_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_R5FSS0_CORE1_INTR_CPSW1_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_R5FSS0_CORE1_INTR_CPSW1_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#elif defined(BUILD_MCU3_0)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_R5FSS1_CORE0_INTR_CPSW1_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_R5FSS1_CORE0_INTR_CPSW1_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_R5FSS1_CORE0_INTR_CPSW1_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#elif defined(BUILD_MPU1_0)
        {   /* EVNT_PEND - Event pending interrupt (CPTS) */
            .intrId     = CPSW_INTR_EVNT_PEND,
            .coreIntNum = CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_CPSW1_EVNT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* STATS_PEND - Statistics pending interrupt */
            .intrId     = CPSW_INTR_STATS_PEND0,
            .coreIntNum = CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_CPSW1_STAT_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
        {   /* MDIO_PEND - MDIO interrupt */
            .intrId     = CPSW_INTR_MDIO_PEND,
            .coreIntNum = CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_CPSW1_MDIO_PEND_0,
            .srcIdx     = ENET_SOC_DIRECT_INTR_SRCIDX_INVALID,
        },
#else
#error "Enet J721S2 SOC: Core not supported!!"
#endif
    },
    .clocks =
    {
        .cppiClkId        = TISCI_DEV_CPSW1_CPPI_CLK_CLK,
        .rgmii250MHzClkId = TISCI_DEV_CPSW1_RGMII_MHZ_250_CLK,
        .rgmii50MHzClkId  = TISCI_DEV_CPSW1_RGMII_MHZ_50_CLK,
        .rgmii5MHzClkId   = TISCI_DEV_CPSW1_RGMII_MHZ_5_CLK,
    },
    .txChPeerThreadId = CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILD_THREAD_OFFSET,
    .rxChPeerThreadId = CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILS_THREAD_OFFSET,
    .txChCount        = CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILD_THREAD_CNT,

    /* Note- Though CPSW supports 64 distinct flow Ids, there are only
     * 8 policer/classifier so can effectively assign only 8 flow ids in CPSW2G */
    .rxFlowCount     = 8U,
};

/* MAIN CPSW_2G MAC ports */
CpswMacPort_Obj gEnetSoc_cpsw2gMainMacObj[] =
{
    {
        .enetMod =
        {
            .name       = "main.cpsw2g.macport1",
            .physAddr   = (CSL_CPSW1_NUSS_BASE + CPSW_NU_OFFSET),
            .physAddr2  = (CSL_CPSW1_NUSS_BASE + CPSW_SGMII0_OFFSET(0U)),
            .features   = (ENET_FEAT_BASE |
                           CPSW_MACPORT_FEATURE_EST |
                           CPSW_MACPORT_FEATURE_SGMII |
                           CPSW_MACPORT_FEATURE_IET),
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswMacPort_open,
            .rejoin     = &CpswMacPort_rejoin,
            .ioctl      = &CpswMacPort_ioctl,
            .close      = &CpswMacPort_close,
        },
        .macPort = ENET_MAC_PORT_1,
    },
};

/* MAIN CPSW 2G Peripheral */
Cpsw_Obj gEnetSoc_cpsw2gMain =
{
    .enetPer =
    {
        .name         = "main.cpsw2g",
        .enetType     = ENET_CPSW_2G,
        .instId       = 1U,
        .magic        = ENET_NO_MAGIC,
        .physAddr     = (CSL_CPSW1_NUSS_BASE + CPSW_NU_OFFSET),
        .physAddr2    = (CSL_CPSW1_NUSS_BASE + CPSW_NUSS_OFFSET),
        .features     = (ENET_FEAT_BASE |
                         CPSW_FEATURE_EST |
                         CPSW_FEATURE_INTERVLAN |
                         CPSW_FEATURE_IET),
        .errata       = ENET_ERRATA_NONE,
        .initCfg      = &Cpsw_initCfg,
        .open         = &Cpsw_open,
        .rejoin       = &Cpsw_rejoin,
        .ioctl        = &Cpsw_ioctl,
        .poll         = &Cpsw_poll,
        .convertTs    = NULL,
        .periodicTick = &Cpsw_periodicTick,
        .registerEventCb   = NULL,
        .unregisterEventCb = NULL,
        .close        = &Cpsw_close,
    },

    /* Host port module object */
    .hostPortObj =
    {
        .enetMod =
        {
            .name       = "main.cpsw2g.hostport",
            .physAddr   = (CSL_CPSW1_NUSS_BASE + CPSW_NU_OFFSET),
            .features   = ENET_FEAT_BASE,
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswHostPort_open,
            .rejoin     = &CpswHostPort_rejoin,
            .ioctl      = &CpswHostPort_ioctl,
            .close      = &CpswHostPort_close,
        }
    },

    /* MAC port module objects */
    .macPortObj = gEnetSoc_cpsw2gMainMacObj,
    .macPortNum = ENET_ARRAYSIZE(gEnetSoc_cpsw2gMainMacObj),

    /* ALE module object */
    .aleObj =
    {
        .enetMod =
        {
            .name       = "main.cpsw2g.ale",
            .physAddr   = (CSL_CPSW1_NUSS_BASE + CPSW_ALE_OFFSET),
            .features   = (ENET_FEAT_BASE |
                           CPSW_ALE_FEATURE_FLOW_PRIORITY |
                           CPSW_ALE_FEATURE_IP_HDR_WHITELIST),
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswAle_open,
            .rejoin     = &CpswAle_rejoin,
            .ioctl      = &CpswAle_ioctl,
            .close      = &CpswAle_close,
        },
    },

    /* CPTS module object */
    .cptsObj =
    {
        .enetMod =
        {
            .name       = "main.cpsw2g.cpts",
            .physAddr   = (CSL_CPSW1_NUSS_BASE + CPSW_CPTS_OFFSET),
            .features   = ENET_FEAT_BASE,
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswCpts_open,
            .rejoin     = &CpswCpts_rejoin,
            .ioctl      = &CpswCpts_ioctl,
            .close      = &CpswCpts_close,
        },
        .hwPushCnt      = 4U,
    },

    /* MDIO module object */
    .mdioObj =
    {
        .enetMod =
        {
            .name       = "main.cpsw2g.mdio",
            .physAddr   = (CSL_CPSW1_NUSS_BASE + CPSW_MDIO_OFFSET),
            .features   = (ENET_FEAT_BASE |
                           MDIO_FEATURE_CLAUSE45),
            .errata     = ENET_ERRATA_NONE,
            .open       = &Mdio_open,
            .rejoin     = &Mdio_rejoin,
#if defined(ENABLE_MDIO_MANUAL_MODE)
            .ioctl      = &Mdio_ioctlManualMode,
#else
            .ioctl      = &Mdio_ioctl,
#endif
            .close      = &Mdio_close,
        },
    },

    /* Statistics module object */
    .statsObj =
    {
        .enetMod =
        {
            .name       = "main.cpsw2g.stats",
            .physAddr   = (CSL_CPSW1_NUSS_BASE + CPSW_NU_OFFSET),
            .features   = ENET_FEAT_BASE,
            .errata     = ENET_ERRATA_NONE,
            .open       = &CpswStats_open,
            .rejoin     = &CpswStats_rejoin,
            .ioctl      = &CpswStats_ioctl,
            .close      = &CpswStats_close,
        },
    },

    /* RM module object */
    .rmObj =
    {
        .enetMod =
        {
            .name       = "main.cpsw2g.rm",
            .physAddr   = 0U,
            .features   = ENET_FEAT_BASE,
            .errata     = ENET_ERRATA_NONE,
            .open       = &EnetRm_open,
            .rejoin     = &EnetRm_rejoin,
            .ioctl      = &EnetRm_ioctl,
            .close      = &EnetRm_close,
        },
    },
};

/* ---------------------------- Enet Peripherals ---------------------------- */

Enet_Obj gEnetSoc_perObj[] =
{
    /* CPSW_2G Enet driver/peripheral */
    {
        .enetPer = &gEnetSoc_cpsw2g.enetPer,
    },

    /* MAIN CPSW_2G Enet driver/peripheral */
    {
        .enetPer = &gEnetSoc_cpsw2gMain.enetPer,
    },
};

EnetPer_Obj gEnetSoc_enetObj[] =
{
#if defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    {
        .enetType = ENET_CPSW_2G,
        .instId   = 1U,
    },
#elif defined(BUILD_MPU1_0)
    {
        .enetType = ENET_CPSW_2G,
        .instId   = 0U,
    },
    {
        .enetType = ENET_CPSW_2G,
        .instId   = 1U,
    },
#else
    {
        .enetType = ENET_CPSW_2G,
        .instId   = 0U,
    },
#endif
};

/* ------------------------------- DMA objects ------------------------------ */

EnetUdma_DrvObj gEnetSoc_dmaObj[ENET_ARRAYSIZE(gEnetSoc_perObj)];

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetSoc_init(void)
{
    memset(gEnetSoc_dmaObj, 0, sizeof(gEnetSoc_dmaObj));

    return ENET_SOK;
}

void EnetSoc_deinit(void)
{
}

EnetDma_Handle EnetSoc_getDmaHandle(Enet_Type enetType,
                                    uint32_t instId)
{
    EnetDma_Handle hDma = NULL;

    switch (enetType)
    {
        case ENET_CPSW_2G:
            if (instId == 0U)
            {
                hDma = &gEnetSoc_dmaObj[0U];
            }
            else if (instId == 1U)
            {
                hDma = &gEnetSoc_dmaObj[1U];
            }
            break;

        default:
            break;
    }

    return hDma;
}

Enet_Handle EnetSoc_getEnetHandleByIdx(uint32_t idx)
{
    Enet_Handle hEnet = NULL;
    Enet_Type enetType;
    uint32_t instId;
    uint32_t count = ENET_ARRAYSIZE(gEnetSoc_enetObj);

    if (idx < count)
    {
        enetType = gEnetSoc_enetObj[idx].enetType;
        instId = gEnetSoc_enetObj[idx].instId;

        hEnet = EnetSoc_getEnetHandle(enetType, instId);
    }

    return hEnet;
}

Enet_Handle EnetSoc_getEnetHandle(Enet_Type enetType,
                                  uint32_t instId)
{
    Enet_Handle hEnet = NULL;

    switch (enetType)
    {
        case ENET_CPSW_2G:
            if (instId == 0U)
            {
                hEnet = &gEnetSoc_perObj[0U];
            }
            else if (instId == 1U)
            {
                hEnet = &gEnetSoc_perObj[1U];
            }
            break;

        default:
            {
                EnetSoc_assert(BFALSE, "Invalid peripheral (eneType=%u instId=%u)\n", enetType, instId);
            }
            break;
    }

    return hEnet;
}

uint32_t EnetSoc_getCoreId(void)
{
    uint32_t coreId;

#if defined(BUILD_MPU1_0)
    coreId = IPC_MPU1_0;
#elif defined(BUILD_MCU1_0)
    coreId = IPC_MCU1_0;
#elif defined(BUILD_MCU2_0)
    coreId = IPC_MCU2_0;
#elif defined(BUILD_MCU2_1)
    coreId = IPC_MCU2_1;
#elif defined(BUILD_MCU3_0)
    coreId = IPC_MCU3_0;
#else
#error "Enet J721S2 SOC: Core not supported!!"
#endif

    return coreId;
}

bool EnetSoc_isCoreAllowed(Enet_Type enetType,
                           uint32_t instId,
                           uint32_t coreId)
{
    return BTRUE;
}

uint32_t EnetSoc_getEnetNum(void)
{
    return ENET_ARRAYSIZE(gEnetSoc_enetObj);
}

uint32_t EnetSoc_getMacPortMax(Enet_Type enetType,
                               uint32_t instId)
{
    uint32_t numPorts = 0U;

    if ((enetType == ENET_CPSW_2G) && (instId == 0U))
    {
        numPorts = gEnetSoc_cpsw2g.macPortNum;
    }
    else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
    {
        numPorts = gEnetSoc_cpsw2gMain.macPortNum;
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid peripheral (eneType=%u instId=%u)\n", enetType, instId);
    }

    return numPorts;
}

uint32_t EnetSoc_isIpSupported(Enet_Type enetType,
                               uint32_t instId)
{
    bool supported = BFALSE;

    if ( ((enetType == ENET_CPSW_2G) && (0U == instId)) ||
         ((enetType == ENET_CPSW_2G) && (1U == instId)) )
    {
        supported = BTRUE;
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid Enet & instId type %d, %d\n", enetType, instId);
    }

    return supported;
}

uint32_t EnetSoc_getRxFlowCount(Enet_Type enetType,
                                uint32_t instId)
{
    uint32_t rxFlowCount = 0U;

    /* Get SoC array index */
    if ((enetType == ENET_CPSW_2G) && (instId == 0U))
    {
        rxFlowCount = gEnetSoc_cpsw2gSocCfg.rxFlowCount;
    }
    else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
    {
        rxFlowCount = gEnetSoc_cpsw2gMainSocCfg.rxFlowCount;
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid Enet type %d\n", enetType);
    }

    return rxFlowCount;
}

uint32_t EnetSoc_getRxChPeerId(Enet_Type enetType,
                               uint32_t instId,
                               uint32_t chIdx)
{
    uint32_t peerChNum = 0U;

    /* Get SoC array index */
    if ((enetType == ENET_CPSW_2G) && (instId == 0U))
    {
        peerChNum = gEnetSoc_cpsw2gSocCfg.rxChPeerThreadId;
    }
    else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
    {
        peerChNum = gEnetSoc_cpsw2gMainSocCfg.rxChPeerThreadId;
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid Enet type %d\n", enetType);
    }

    return peerChNum;
}

uint32_t EnetSoc_getTxChCount(Enet_Type enetType,
                              uint32_t instId)
{
    uint32_t txChCount = 0U;

    /* Get SoC array index */
    if ((enetType == ENET_CPSW_2G) && (instId == 0U))
    {
        txChCount = gEnetSoc_cpsw2gSocCfg.txChCount;
    }
    else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
    {
        txChCount = gEnetSoc_cpsw2gMainSocCfg.txChCount;
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid Enet type %d\n", enetType);
    }

    return txChCount;
}

uint32_t EnetSoc_getHwPushCount(Enet_Type enetType,
                                uint32_t instId)
{
    uint32_t hwPushCount = 0U;

    /* Get SoC array index */
    if ((enetType == ENET_CPSW_2G) && (instId == 0U))
    {
        hwPushCount = gEnetSoc_cpsw2g.cptsObj.hwPushCnt;
    }
    else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
    {
        hwPushCount = gEnetSoc_cpsw2gMain.cptsObj.hwPushCnt;
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid Enet type %d\n", enetType);
    }

    return hwPushCount;
}

uint32_t EnetSoc_getTxChPeerId(Enet_Type enetType,
                               uint32_t instId,
                               uint32_t chNum)
{
    uint32_t peerChNum = 0U;

    /* Get SoC array index */
    if ((enetType == ENET_CPSW_2G) && (instId == 0U))
    {
        peerChNum = gEnetSoc_cpsw2gSocCfg.txChPeerThreadId;
    }
    else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
    {
        peerChNum = gEnetSoc_cpsw2gMainSocCfg.txChPeerThreadId;
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid Enet type %d\n", enetType);
    }

    /* Get PSI-L destination thread offset for Tx channel */
    peerChNum = (peerChNum + chNum);

    return peerChNum;
}

uint32_t EnetSoc_getClkFreq(Enet_Type enetType,
                            uint32_t instId,
                            uint32_t clkId)
{
    uint32_t freq = 0U;

    if (clkId == CPSW_CPPI_CLK)
    {
        if ((enetType == ENET_CPSW_2G) && (instId == 0U))
        {
            freq = gEnetSoc_cpsw2gSocCfg.cppiClkFreqHz;
        }
        else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
        {
            freq = gEnetSoc_cpsw2gMainSocCfg.cppiClkFreqHz;
        }
        else
        {
            EnetSoc_assert(BFALSE, "Invalid peripheral (eneType=%u instId=%u)\n", enetType, instId);
        }
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid clk id %u\n", clkId);
    }

    return freq;
}

int32_t EnetSoc_setupIntrCfg(Enet_Type enetType,
                             uint32_t instId,
                             uint32_t intrId)
{
    const EnetSoc_IntrConnCfg *socIntrs = NULL;
    uint32_t numSocIntrs = 0U;
    uint16_t coreDevId = EnetSoc_getCoreDevId();
    uint16_t perDevId = 0U;
    int32_t status = ENET_SOK;

    if ((enetType == ENET_CPSW_2G) && (instId == 0U))
    {
        perDevId    = gEnetSoc_cpsw2gSocCfg.dmscDevId;
        socIntrs    = gEnetSoc_cpsw2gSocCfg.intrs;
        numSocIntrs = ENET_ARRAYSIZE(gEnetSoc_cpsw2gSocCfg.intrs);
    }
    else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
    {
        perDevId    = gEnetSoc_cpsw2gMainSocCfg.dmscDevId;
        socIntrs    = gEnetSoc_cpsw2gMainSocCfg.intrs;
        numSocIntrs = ENET_ARRAYSIZE(gEnetSoc_cpsw2gMainSocCfg.intrs);
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid peripheral (eneType=%u instId=%u)\n", enetType, instId);
        status = ENET_EINVALIDPARAMS;
    }

    if (status == ENET_SOK)
    {
        status = EnetSocJ7x_setupIntrCfg(intrId, coreDevId, perDevId, socIntrs, numSocIntrs);
    }

    return status;
}

int32_t EnetSoc_releaseIntrCfg(Enet_Type enetType,
                               uint32_t instId,
                               uint32_t intrId)
{
    const EnetSoc_IntrConnCfg *socIntrs = NULL;
    uint32_t numSocIntrs = 0U;
    uint16_t coreDevId = EnetSoc_getCoreDevId();
    uint16_t perDevId = 0U;
    int32_t status = ENET_SOK;

    if ((enetType == ENET_CPSW_2G) && (instId == 0U))
    {
        perDevId    = gEnetSoc_cpsw2gSocCfg.dmscDevId;
        socIntrs    = gEnetSoc_cpsw2gSocCfg.intrs;
        numSocIntrs = ENET_ARRAYSIZE(gEnetSoc_cpsw2gSocCfg.intrs);
    }
    else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
    {
        perDevId    = gEnetSoc_cpsw2gMainSocCfg.dmscDevId;
        socIntrs    = gEnetSoc_cpsw2gMainSocCfg.intrs;
        numSocIntrs = ENET_ARRAYSIZE(gEnetSoc_cpsw2gMainSocCfg.intrs);
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid peripheral (eneType=%u instId=%u)\n", enetType, instId);
        status = ENET_EINVALIDPARAMS;
    }

    if (status == ENET_SOK)
    {
        status = EnetSocJ7x_releaseIntrCfg(intrId, coreDevId, perDevId, socIntrs, numSocIntrs);
    }

    return status;
}

uint32_t EnetSoc_getIntrNum(Enet_Type enetType,
                            uint32_t instId,
                            uint32_t intrId)
{
    const EnetSoc_IntrConnCfg *socIntrs = NULL;
    uint32_t numSocIntrs = 0U;
    uint32_t intrNum = 0U;
    int32_t status = ENET_SOK;

    if ((enetType == ENET_CPSW_2G) && (instId == 0U))
    {
        socIntrs    = gEnetSoc_cpsw2gSocCfg.intrs;
        numSocIntrs = ENET_ARRAYSIZE(gEnetSoc_cpsw2gSocCfg.intrs);
    }
    else if ((enetType == ENET_CPSW_2G) && (instId == 1U))
    {
        socIntrs    = gEnetSoc_cpsw2gMainSocCfg.intrs;
        numSocIntrs = ENET_ARRAYSIZE(gEnetSoc_cpsw2gMainSocCfg.intrs);
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid peripheral (eneType=%u instId=%u)\n", enetType, instId);
        status = ENET_EINVALIDPARAMS;
    }

    if (status == ENET_SOK)
    {
        intrNum = EnetSocJ7x_getIntrNum(intrId, socIntrs, numSocIntrs);
    }

    return intrNum;
}

uint32_t EnetSoc_getIntrTriggerType(Enet_Type enetType,
                                    uint32_t instId,
                                    uint32_t intrId)
{
    return OSAL_ARM_GIC_TRIG_TYPE_LEVEL;
}

int32_t EnetSoc_getEFusedMacAddrs(uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                                  uint32_t *num)
{
    CSL_mcu_ctrl_mmr_cfg0Regs *mmrRegs;
    uint32_t val;

    if (*num >= 1U)
    {
        mmrRegs = (CSL_mcu_ctrl_mmr_cfg0Regs *)(uintptr_t)CSL_MCU_CTRL_MMR0_CFG0_BASE;

        val = CSL_REG32_RD(&mmrRegs->MAC_ID0);
        macAddr[0][5] = (uint8_t)((val & 0x000000FFU) >> 0U);
        macAddr[0][4] = (uint8_t)((val & 0x0000FF00U) >> 8U);
        macAddr[0][3] = (uint8_t)((val & 0x00FF0000U) >> 16U);
        macAddr[0][2] = (uint8_t)((val & 0xFF000000U) >> 24U);

        val = CSL_REG32_RD(&mmrRegs->MAC_ID1);
        macAddr[0][1] = (uint8_t)((val & 0x000000FFU) >> 0U);
        macAddr[0][0] = (uint8_t)((val & 0x0000FF00U) >> 8U);

        *num = 1U;
    }

    return ENET_SOK;
}

uint32_t EnetSoc_getMacPortCaps(Enet_Type enetType,
                                uint32_t instId,
                                Enet_MacPort macPort)
{
    uint32_t linkCaps = 0U;

    if ((enetType == ENET_CPSW_2G) && (macPort == ENET_MAC_PORT_1) && (instId == 0U))
    {
        linkCaps = (ENETPHY_LINK_CAP_HD10 | ENETPHY_LINK_CAP_FD10 |
                    ENETPHY_LINK_CAP_HD100 | ENETPHY_LINK_CAP_FD100 |
                    ENETPHY_LINK_CAP_FD1000);
    }
    else if ((enetType == ENET_CPSW_2G) && (macPort == ENET_MAC_PORT_1) && (instId == 1U))
    {
        linkCaps = (ENETPHY_LINK_CAP_HD10 | ENETPHY_LINK_CAP_FD10 |
                    ENETPHY_LINK_CAP_HD100 | ENETPHY_LINK_CAP_FD100 |
                    ENETPHY_LINK_CAP_FD1000);
    }
    else
    {
        EnetSoc_assert(BFALSE, "Invalid peripheral (eneType=%u instId=%u)\n", enetType, instId);
    }

    return linkCaps;
}

int32_t EnetSoc_getMacPortMii(Enet_Type enetType,
                              uint32_t instId,
                              Enet_MacPort macPort,
                              EnetMacPort_Interface *mii)
{
    EnetMac_LayerType *enetLayer = &mii->layerType;
    EnetMac_SublayerType *enetSublayer = &mii->sublayerType;
    uint32_t modeSel = CPSW_ENET_CTRL_MODE_RGMII;
    int32_t status = ENET_EFAIL;

    switch (enetType)
    {
        case ENET_CPSW_2G:
            if (instId == 0U)
            {
                status = EnetSoc_getMcuEnetControl(macPort, &modeSel);
            }
            else if (instId == 1U)
            {
                status = EnetSoc_getMainEnetControl(macPort, &modeSel);
            }
            else
            {
                EnetSoc_assert(BFALSE, "Invalid peripheral (eneType=%u instId=%u)\n", enetType, instId);
            }
            break;

        default:
            EnetSoc_assert(BFALSE, "Invalid peripheral type: %u\n", enetType);
            break;
    }

    if (status == ENET_SOK)
    {
        switch (modeSel)
        {
            /* RMII */
            case CPSW_ENET_CTRL_MODE_RMII:
                *enetLayer    = ENET_MAC_LAYER_MII;
                *enetSublayer = ENET_MAC_SUBLAYER_REDUCED;
                break;

            /* RGMII */
            case CPSW_ENET_CTRL_MODE_RGMII:
                *enetLayer    = ENET_MAC_LAYER_GMII;
                *enetSublayer = ENET_MAC_SUBLAYER_REDUCED;
                break;

            /* SGMII */
            case CPSW_ENET_CTRL_MODE_SGMII:
                *enetLayer    = ENET_MAC_LAYER_GMII;
                *enetSublayer = ENET_MAC_SUBLAYER_SERIAL;
                break;

            default:
                status = ENET_EINVALIDPARAMS;
                break;
        }
    }

    return status;
}

static uint32_t EnetSoc_getMcuEnetControl(Enet_MacPort macPort,
                                          uint32_t *modeSel)
{
    CSL_mcu_ctrl_mmr_cfg0Regs *regs =
        (CSL_mcu_ctrl_mmr_cfg0Regs *)(uintptr_t)CSL_MCU_CTRL_MMR0_CFG0_BASE;
    int32_t status = ENET_SOK;

    switch (macPort)
    {
        case ENET_MAC_PORT_1:
            *modeSel = CSL_FEXT(regs->MCU_ENET_CTRL, MCU_CTRL_MMR_CFG0_MCU_ENET_CTRL_MODE_SEL);
            break;

        default:
            status = ENET_EINVALIDPARAMS;
            break;
    }

    if (status == ENET_SOK)
    {
        switch (*modeSel)
        {
            case CPSW_ENET_CTRL_MODE_RMII:
            case CPSW_ENET_CTRL_MODE_RGMII:
                break;

            default:
                status = ENET_EINVALIDPARAMS;
                break;
        }
    }

    return status;
}

static uint32_t EnetSoc_getMainEnetControl(Enet_MacPort macPort,
                                           uint32_t *modeSel)
{
    CSL_main_ctrl_mmr_cfg0Regs *regs =
        (CSL_main_ctrl_mmr_cfg0Regs *)(uintptr_t)CSL_CTRL_MMR0_CFG0_BASE;
    int32_t status = ENET_SOK;

    switch (macPort)
    {
        case ENET_MAC_PORT_1:
            *modeSel = CSL_FEXT(regs->ENET1_CTRL, MAIN_CTRL_MMR_CFG0_ENET1_CTRL_PORT_MODE_SEL);
            break;

        default:
            status = ENET_EINVALIDPARAMS;
            break;
    }

    if (status == ENET_SOK)
    {
        switch (*modeSel)
        {
            case CPSW_ENET_CTRL_MODE_RMII:
            case CPSW_ENET_CTRL_MODE_RGMII:
            case CPSW_ENET_CTRL_MODE_SGMII:
                break;

            default:
                status = ENET_EINVALIDPARAMS;
                break;
        }
    }

    return status;
}

static uint32_t EnetSoc_getCoreDevId(void)
{
    uint32_t coreDevId;

#if defined(BUILD_MPU1_0)
    coreDevId = TISCI_DEV_A72SS0_CORE0;
#elif defined(BUILD_MCU1_0)
    coreDevId = TISCI_DEV_MCU_R5FSS0_CORE0;
#elif defined(BUILD_MCU2_0)
    coreDevId = TISCI_DEV_R5FSS0_CORE0;
#elif defined(BUILD_MCU2_1)
    coreDevId = TISCI_DEV_R5FSS0_CORE1;
#elif defined(BUILD_MCU3_0)
    coreDevId = TISCI_DEV_R5FSS1_CORE0;
#else
#error "Enet J721S2 SOC: Core not supported!!"
#endif

    return coreDevId;
}

int32_t EnetSoc_validateQsgmiiCfg(Enet_Type enetType,
                                  uint32_t instId)
{
    return ENET_ENOTSUPPORTED;
}

int32_t EnetSoc_mapPort2QsgmiiId(Enet_Type enetType,
                                 uint32_t instId,
                                 Enet_MacPort macPort,
                                 uint32_t *qsgmiiId)
{
    return ENET_ENOTSUPPORTED;
}
