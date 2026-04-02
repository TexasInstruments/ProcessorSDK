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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* lwIP header files */
#include <lwipopts.h>
#include <lwip/api.h>
#include <lwip/netif.h>
#include <arch/sys_arch.h>
#include <ti/drv/enet/lwipif/inc/lwipif2enet_appif.h>

/* TI Driver Header files */
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/udma/udma.h>
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils_rtos.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_mcm.h>
#include <ti/drv/enet/examples/utils/include/enet_apprm.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>

#include <ti/drv/enet/include/core/enet_per.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/include/mod/cpsw_ale.h>
#include <ti/drv/enet/include/mod/cpsw_macport.h>

#include <ti/drv/mmcsd/MMCSD.h>
#include <ti/drv/mmcsd/src/MMCSD_osal.h>
#include <ti/drv/mmcsd/soc/MMCSD_soc.h>
#include <ti/fs/fatfs/ff.h>
#include <ti/fs/fatfs/FATFS.h>
#include <ti/drv/uart/UART_stdio.h>

/* OSAL Header files */
#include <ti/osal/osal.h>
#include <ti/osal/LoadP.h>

/* TSN Stack Header and example app Header files */
#include <tsn_combase/combase.h>
#include "debug_log.h"
#include "tsninit.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Macro to abort app during fatal erros */
#define EnetAppAbort(message) \
    EnetAppUtils_print(message); \
    EnetAppUtils_assert(BFALSE);

/* How often lwIP adaptation layer will poll for RX packets (pacing timer period) */
#define ENETAPP_PACKET_POLL_PERIOD_US     (1000U)

/* Maximum number of Ethernet ports (CPSW9G in J721E/J784S4) */
#define MAX_NUM_MAC_PORTS               (8U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* Example application parameters: CPSW instance and ports to be tested */
typedef struct EnetApp_Cfg_s
{
    /* Peripheral instance type */
    Enet_Type enetType;

    /* Peripheral instance id */
    uint32_t instId;

    /* Ethernet ports to be enabled */
    EnetBoard_EthPort ethPorts[MAX_NUM_MAC_PORTS];

    /* Number of Ethernet ports to be enabled */
    uint32_t numEthPorts;

    /* Whether or not to use RX default flow */
    bool useDfltFlow;
} EnetApp_Cfg;

/* Container structure of all example app variables */
typedef struct EnetApp_Obj_s
{
    /* Host MAC address */
    uint8_t macAddr[ENET_MAC_ADDR_LEN];

    /* Enet MCM handle */
    EnetMcm_CmdIf hMcmCmdIf[ENET_TYPE_NUM];

    /* UDMA driver handle */
    Udma_DrvHandle hUdmaDrv;

    /* Handle to task which performs board initialization, initializes
     * TSN stack and LwIP stack, and run CPU load periodically */
    TaskP_Handle hAppInitTask;

    /* FATFS handle */
    FATFS_Handle fatfsHandle;
} EnetApp_Obj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t EnetApp_init(void);
static void EnetApp_deinit(void);
static void EnetApp_appInitTask(void *a0, void *a1);
static int32_t EnetApp_initMMCSD(void);
static int EnetApp_initTsn(void);

extern void EnetApp_initLwIP(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint8_t gAvbAppTskStack[TSN_TSK_STACK_SIZE]
__attribute__ ((aligned(TSN_TSK_STACK_ALIGN)));

#if defined(SOC_J721E)
#define CFG_NUM_MAC_PORTS (4U)
static EnetApp_Cfg gEnetAppCfg =
{
    .enetType = ENET_CPSW_9G,
    .instId   = 0U,
    .ethPorts =
    {
#if defined(QSGMII_EXPANSION_BOARD) && defined(RGMII_EXPANSION_BOARD)
        {   /* 'P0' port in QpENet board (QSGMII expansion board) */
            .macPort = ENET_MAC_PORT_2,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET,
        },
        {   /* 'P0' port in QpENet board (QSGMII expansion board) */
            .macPort = ENET_MAC_PORT_5,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET,
        },
        {    /* 'PRG1_RGMII1_B' in GESI expansion board */
            .macPort = ENET_MAC_PORT_1,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
            .boardId = ENETBOARD_GESI_ID,
            .expPort = ENETBOARD_EXP_PORT_GESI,
        },
        {   /* 'PRG0_RGMII1_B' in GESI expansion board */
            .macPort = ENET_MAC_PORT_3,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
            .boardId = ENETBOARD_GESI_ID,
            .expPort = ENETBOARD_EXP_PORT_GESI,
        }
#elif defined(QSGMII_EXPANSION_BOARD)
        {   /* 'P0' port in QpENet board (QSGMII expansion board) */
            .macPort = ENET_MAC_PORT_2,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET,
        },
        {   /* 'P0' port in QpENet board (QSGMII expansion board) */
            .macPort = ENET_MAC_PORT_5,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET,
        },
        {
            .macPort = ENET_MAC_PORT_6,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET,
        },
        {
            .macPort = ENET_MAC_PORT_7,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET,
        },
#else // RGMII_EXPANSION_BOARD
        {    /* 'PRG1_RGMII1_B' in GESI expansion board */
            .macPort = ENET_MAC_PORT_1,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
            .boardId = ENETBOARD_GESI_ID,
            .expPort = ENETBOARD_EXP_PORT_GESI,
        },
        {   /* 'PRG0_RGMII1_B' in GESI expansion board */
            .macPort = ENET_MAC_PORT_3,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
            .boardId = ENETBOARD_GESI_ID,
            .expPort = ENETBOARD_EXP_PORT_GESI,
        },
        {
            .macPort = ENET_MAC_PORT_4,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
            .boardId = ENETBOARD_GESI_ID,
            .expPort = ENETBOARD_EXP_PORT_GESI,
       },
       {
           .macPort = ENET_MAC_PORT_8,
           .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
           .boardId = ENETBOARD_GESI_ID,
           .expPort = ENETBOARD_EXP_PORT_GESI,
       }
#endif
    },
    .numEthPorts = CFG_NUM_MAC_PORTS,
    .useDfltFlow    = BTRUE,
};
#elif defined(SOC_J7200)
#define CFG_NUM_MAC_PORTS (2U)
static EnetApp_Cfg gEnetAppCfg =
{
    .enetType = ENET_CPSW_5G,
    .instId   = 0U,
    .ethPorts =
    {
        {   /* 'P0' port in QpENet board (QSGMII expansion board) */
            .macPort = ENET_MAC_PORT_1,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET,
        },
        {   /* 'P0' port in QpENet board (QSGMII expansion board) */
            .macPort = ENET_MAC_PORT_2,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET,
        }
    },
    .numEthPorts = CFG_NUM_MAC_PORTS,
    .useDfltFlow    = BTRUE,
};
#elif defined(SOC_J721S2)
#define CFG_NUM_MAC_PORTS (1U)
static EnetApp_Cfg gEnetAppCfg =
{
    .enetType = ENET_CPSW_2G,
    .instId   = 1U,
    .ethPorts =
    {
        {    /* 'PRG0_RGMII1_B' port in GESI expansion board */
            .macPort = ENET_MAC_PORT_1,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
            .boardId = ENETBOARD_GESI_ID,
            .expPort = ENETBOARD_EXP_PORT_GESI,
        },
    },
    .numEthPorts = CFG_NUM_MAC_PORTS,
    .useDfltFlow    = BTRUE,
};
#elif defined(SOC_J784S4) || defined(SOC_J742S2)
#define CFG_NUM_MAC_PORTS (4U)
static EnetApp_Cfg gEnetAppCfg =
{
    .enetType = ENET_CPSW_9G,
    .instId   = 0U,
    .ethPorts =
    {
        {   /* 'P0' port in QpENet board (QSGMII expansion board) */
            .macPort = ENET_MAC_PORT_1,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET1,
        },
        {
            .macPort = ENET_MAC_PORT_3,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET1,
        },
        {
            .macPort = ENET_MAC_PORT_4,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET1,
        },
        {
            .macPort = ENET_MAC_PORT_5,
            .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
            .boardId = ENETBOARD_QPENET_ID,
            .expPort = ENETBOARD_EXP_PORT_ENET1,
        },
    },
    .numEthPorts = CFG_NUM_MAC_PORTS,
    .useDfltFlow    = BTRUE,
};
#else
#error "SOC is not supported"
#endif

/* MMCSD function table for MMCSD implementation */
FATFS_DrvFxnTable FATFS_drvFxnTable =
{
    MMCSD_close,
    MMCSD_control,
    MMCSD_init,
    MMCSD_open,
    MMCSD_write,
    MMCSD_read
};

/* FATFS configuration structure */
FATFS_HwAttrs FATFS_initCfg[_VOLUMES] =
{
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    {  /* MMC1 is SD card  for J7 GP EVM */
        1U
    },
#else
    {
        0U
    },
#endif
    {
        1U
    },
    {
        2U
    },
    {
        3U
    }
};
/* FATFS objects */
FATFS_Object FATFS_objects[_VOLUMES];

/* FATFS configuration structure */
const FATFSConfigList FATFS_config = {
    {
        &FATFS_drvFxnTable,
        &FATFS_objects[0],
        &FATFS_initCfg[0]
    },

    {
         &FATFS_drvFxnTable,
         &FATFS_objects[1],
         &FATFS_initCfg[1]
    },

    {
         &FATFS_drvFxnTable,
         &FATFS_objects[2],
         &FATFS_initCfg[2]
    },

    {NULL, NULL, NULL},

    {NULL, NULL, NULL}
};

/* Container object of all TSN example app related parameters/variables */
static EnetApp_Obj gEnetAppObj =
{
    .hMcmCmdIf =
    {
        [ENET_CPSW_2G] = {.hMboxCmd = NULL, .hMboxResponse = NULL},
        [ENET_CPSW_5G] = {.hMboxCmd = NULL, .hMboxResponse = NULL},
        [ENET_CPSW_9G] = {.hMboxCmd = NULL, .hMboxResponse = NULL},
    },

    .hAppInitTask  = NULL,
};

/* The following vars are required to be in global memory */
static char gEnetApp_netDevs[CFG_NUM_MAC_PORTS][IFNAMSIZ] = {0};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Params params;

    OS_init();

    TaskP_Params_init(&params);
    params.name      = (char *)"tsnapp_task";
    params.priority  = DEFAULT_THREAD_PRIO;
    params.stack     = gAvbAppTskStack;
    params.stacksize = sizeof(gAvbAppTskStack);

    gEnetAppObj.hAppInitTask = TaskP_create(EnetApp_appInitTask, &params);
    if (gEnetAppObj.hAppInitTask == NULL)
    {
        EnetAppUtils_print("Failed to create app initialization task\n");
        EnetAppUtils_assert(BFALSE);
    }

    OS_start();    /* does not return */

    return(0);
}

static void EnetApp_appInitTask(void *a0, void *a1)
{
    int32_t status;

    /* Board initialization which will configure pinmux, UART through PDK board library */
    EnetBoard_init();

    EnetAppUtils_print("==================================\n");
    EnetAppUtils_print("   NETCONF+TSN GPTP Example App   \n");
    EnetAppUtils_print("==================================\n");

    /* Configure CPSW clocks: CPPI_CLK, RGMII_MHZ_[5,50,250]_CLK and
     * CPTS_REF_CLK to 500MHz */
    EnetAppUtils_enableClocks(gEnetAppCfg.enetType, gEnetAppCfg.instId);

    /* Setup requested Ethernet ports in the board, including SERDES,
     * clocking, PHY config params, etc. */
    status = EnetBoard_setupPorts(gEnetAppCfg.enetType,
                                  gEnetAppCfg.instId,
                                  &gEnetAppCfg.ethPorts[0U],
                                  gEnetAppCfg.numEthPorts);
    EnetAppUtils_assert(status == ENET_SOK);
    EnetAppUtils_print("%s: EnetBoard_setupPorts() done\n", __func__);

#ifndef DISABLE_FAT_FS
    /* Initialize MMCSD Driver */
    status = EnetApp_initMMCSD();
    EnetAppUtils_assert(status == ENET_SOK);
    EnetAppUtils_print("%s: EnetApp_initMMCSD() done\n", __func__);
#endif

    /* Initialize LwIP stack - w/c will open Enet and UDMA LLD */
    EnetApp_initLwIP();
    EnetAppUtils_print("%s: EnetApp_initLwIP() done\n", __func__);

    /* Initialize TSN stack */
    if (EnetApp_initTsn())
    {
        EnetAppAbort("EnetApp_startApplications failed\n");
    }

    while (BTRUE)
    {
#if defined(FREERTOS)
        EnetAppUtils_print("CPU Load: %u%%\n", LoadP_getCPULoad());
        LoadP_reset();
#endif
        TaskP_sleep(10000U);
    }
    if (gEnetAppObj.fatfsHandle)
    {
        FATFS_close(gEnetAppObj.fatfsHandle);
    }
}

/*
 * Set ALE config parameters
 */
void EnetApp_initAleConfig(CpswAle_Cfg *aleCfg)
{
    aleCfg->modeFlags = CPSW_ALE_CFG_MODULE_EN;

    aleCfg->agingCfg.autoAgingEn     = BTRUE;
    aleCfg->agingCfg.agingPeriodInMs = 1000;

    aleCfg->nwSecCfg.vid0ModeEn = BTRUE;

    aleCfg->vlanCfg.aleVlanAwareMode           = BFALSE;
    aleCfg->vlanCfg.cpswVlanAwareMode          = BFALSE;
    aleCfg->vlanCfg.unknownUnregMcastFloodMask = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownRegMcastFloodMask   = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownVlanMemberListMask  = CPSW_ALE_ALL_PORTS_MASK;

    aleCfg->policerGlobalCfg.policingEn         = BTRUE;
    aleCfg->policerGlobalCfg.yellowDropEn       = BFALSE;
    aleCfg->policerGlobalCfg.redDropEn          = BFALSE;
    aleCfg->policerGlobalCfg.policerNoMatchMode = CPSW_ALE_POLICER_NOMATCH_MODE_GREEN;
}

/*
 * Callback called by Enet MCM to request app to populate the link parameters (PHY config,
 * MAC port config, speed/duplex) to be used to open a given MAC port.
 * MCM will call this callback for each port in enetMcmCfg.macPortList.
 */
void EnetApp_initLinkArgs(EnetPer_PortLinkCfg *linkArgs,
                          Enet_MacPort macPort)
{
    CpswMacPort_Cfg *macCfg = (CpswMacPort_Cfg *)linkArgs->macCfg;
    EnetPhy_Cfg *phyCfg = &linkArgs->phyCfg;
    EnetMacPort_LinkCfg *linkCfg = &linkArgs->linkCfg;
    EnetMacPort_Interface *mii = &linkArgs->mii;
    const EnetBoard_EthPort *ethPort = NULL;
    const EnetBoard_PortCfg *portCfg;
    uint32_t i;

    /* Setup board for requested Ethernet port */
    for (i = 0U; i < gEnetAppCfg.numEthPorts; i++)
    {
        if (gEnetAppCfg.ethPorts[i].macPort == macPort)
        {
            ethPort = &gEnetAppCfg.ethPorts[i];
        }
    }

    /* Check that we have config params for the requested MAC port */
    if (ethPort == NULL)
    {
        EnetAppUtils_print("MAC port %u is not enabled in the test\n", ENET_MACPORT_ID(macPort));
        EnetAppUtils_assert(BFALSE);
    }

    /* Initialize MAC configuration with driver's default values */
    CpswMacPort_initCfg(macCfg);

    /* Get Ethernet port configuration params from Enet board library, including MAC port
     * params, PHY config params, MII type, speed/duplexity */
    portCfg = EnetBoard_getPortCfg(gEnetAppCfg.enetType, gEnetAppCfg.instId, ethPort);
    if (portCfg != NULL)
    {
        EnetPhy_initCfg(phyCfg);
        phyCfg->phyAddr     = portCfg->phyCfg.phyAddr;
        phyCfg->isStrapped  = portCfg->phyCfg.isStrapped;
        phyCfg->loopbackEn  = BFALSE;
        phyCfg->skipExtendedCfg = portCfg->phyCfg.skipExtendedCfg;
        phyCfg->extendedCfgSize = portCfg->phyCfg.extendedCfgSize;
        memcpy(phyCfg->extendedCfg, portCfg->phyCfg.extendedCfg, phyCfg->extendedCfgSize);

        *linkCfg = portCfg->linkCfg;

        mii->layerType    = ethPort->mii.layerType;
        mii->sublayerType = ethPort->mii.sublayerType;
        mii->variantType  = ENET_MAC_VARIANT_FORCED;
        macCfg->sgmiiMode = portCfg->sgmiiMode;
    }
    else
    {
        EnetAppUtils_print("No port configuration found for MAC port %u\n", ENET_MACPORT_ID(macPort));
        EnetAppUtils_assert(BFALSE);
    }
}

/*
 * Callback called by Enet LLD when there is link-up or link-down events reported by PHY driver
 */
static void EnetApp_portLinkStatusChangeCb(Enet_MacPort macPort,
                                           bool isLinkUp,
                                           void *appArg)
{
    EnetAppUtils_print("MAC Port %u: link %s\n", ENET_MACPORT_ID(macPort), isLinkUp ? "up" : "down");
}

/*
 * Callback called by Enet LLD when MDIO reports a link status change, if MDIO is in normal mode
 */
static void EnetApp_mdioLinkStatusChange(Cpsw_MdioLinkStateChangeInfo *info,
                                         void *appArg)
{
}

/*
 * Set link status change callback functions
 */
static void EnetApp_initEnetLinkCbPrms(Cpsw_Cfg *cpswCfg)
{
    cpswCfg->mdioLinkStateChangeCb = EnetApp_mdioLinkStatusChange;
    cpswCfg->mdioLinkStateChangeCbArg = &gEnetAppObj;

    cpswCfg->portLinkStatusChangeCb    = &EnetApp_portLinkStatusChangeCb;
    cpswCfg->portLinkStatusChangeCbArg = &gEnetAppObj;
}

/*
 * Open UDMA LLD and Enet LLD (via Enet MCM)
 */
static int32_t EnetApp_init(void)
{
    int32_t status = ENET_SOK;
    EnetMcm_InitConfig enetMcmCfg;
    Cpsw_Cfg cpswCfg;
    EnetRm_ResCfg *resCfg;
    EnetDma_Cfg dmaCfg;
    uint32_t i;

    EnetAppUtils_assert(gEnetAppCfg.numEthPorts <=
                        Enet_getMacPortMax(gEnetAppCfg.enetType, gEnetAppCfg.instId));

    /* Open UDMA */
    gEnetAppObj.hUdmaDrv = EnetAppUtils_udmaOpen(gEnetAppCfg.enetType, NULL);
    EnetAppUtils_assert(NULL != gEnetAppObj.hUdmaDrv);

    dmaCfg.rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;
    dmaCfg.hUdmaDrv = gEnetAppObj.hUdmaDrv;

    resCfg = &cpswCfg.resCfg;
    cpswCfg.dmaCfg = &dmaCfg;

    Enet_initCfg(gEnetAppCfg.enetType, gEnetAppCfg.instId, &cpswCfg, sizeof(cpswCfg));
    cpswCfg.vlanCfg.vlanAware = BFALSE;
    cpswCfg.hostPortCfg.removeCrc = BTRUE;
    cpswCfg.hostPortCfg.padShortPacket = BTRUE;
    cpswCfg.hostPortCfg.passCrcErrors = BTRUE;

    /* CPTS_RFT_CLK is sourced from MAIN_SYSCLK0 (500MHz) */
    cpswCfg.cptsCfg.cptsRftClkFreq = CPSW_CPTS_RFTCLK_FREQ_500MHZ;

    EnetApp_initEnetLinkCbPrms(&cpswCfg);
    resCfg = &cpswCfg.resCfg;

    EnetApp_initAleConfig(&cpswCfg.aleCfg);

    // Do this to avoid future param added to EnetMcm_InitConfig without init
    memset(&enetMcmCfg, 0, sizeof(EnetMcm_InitConfig));
    /* Configure MCM (Multi-Client Manager) parameters: peripheral type, port ids, etc */
    enetMcmCfg.perCfg = &cpswCfg;
    EnetAppUtils_assert(NULL != enetMcmCfg.perCfg);

    EnetAppUtils_initResourceConfig(gEnetAppCfg.enetType, gEnetAppCfg.instId,
                                    EnetSoc_getCoreId(), resCfg);

    enetMcmCfg.enetType           = gEnetAppCfg.enetType;
    enetMcmCfg.instId             = gEnetAppCfg.instId;
    enetMcmCfg.setPortLinkCfg     = EnetApp_initLinkArgs;
    enetMcmCfg.numMacPorts        = gEnetAppCfg.numEthPorts;
    enetMcmCfg.periodicTaskPeriod = ENETPHY_FSM_TICK_PERIOD_MS;
    enetMcmCfg.print              = EnetAppUtils_print;
    enetMcmCfg.extTraceFunc       = NULL;
    enetMcmCfg.traceTsFunc        = NULL;

    for (i = 0U; i < gEnetAppCfg.numEthPorts; i++)
    {
        enetMcmCfg.macPortList[i] = gEnetAppCfg.ethPorts[i].macPort;
    }

    /* Initialize MCM which internally opens Enet LLD */
    status = EnetMcm_init(&enetMcmCfg);

    return status;
}

void EnetApp_netIfCb(struct netif *netif)
{
    const ip4_addr_t *ipAddr;

    if (netif_is_up(netif))
    {
        ipAddr = netif_ip4_addr(netif);

        if (ipAddr->addr != 0)
        {
            EnetAppUtils_print("Enet lwIP App: Added Network IP address I/F %c%c%d: %s\n",
                               netif->name[0], netif->name[1], netif->num,
                               ip4addr_ntoa(ipAddr));
        }
    }
    else
    {
        EnetAppUtils_print("Enet lwIP App: Removed Network IP address I/F %c%c%u\n",
                           netif->name[0], netif->name[1], netif->num);

        /* Deinit the app as it would be again initialized in LwipifEnetAppCb_getHandle */
        EnetApp_deinit();
    }
}

static bool EnetApp_isPortLinked(struct netif *netif,
                                 void *handleArg)
{
    Enet_Handle hEnet = (Enet_Handle)handleArg;
    uint32_t coreId = EnetSoc_getCoreId();
    uint32_t i;
    bool isLinked = BFALSE;

    for (i = 0U; i < gEnetAppCfg.numEthPorts; i++)
    {
        isLinked = EnetAppUtils_isPortLinkUp(hEnet, coreId, gEnetAppCfg.ethPorts[i].macPort);
        if (isLinked)
        {
            break;
        }
    }

    return isLinked;
}

void LwipifEnetAppCb_getHandle(LwipifEnetAppIf_GetHandleInArgs *inArgs,
                               LwipifEnetAppIf_GetHandleOutArgs *outArgs)
{
    int32_t status;
    EnetMcm_HandleInfo handleInfo;
    EnetPer_AttachCoreOutArgs attachInfo;
    LwipifEnetAppIf_RxHandleInfo *rxInfo;
    LwipifEnetAppIf_RxConfig *rxCfg;
    EnetMcm_CmdIf *pMcmCmdIf = &gEnetAppObj.hMcmCmdIf[gEnetAppCfg.enetType];
    EnetUdma_OpenRxFlowPrms enetRxFlowCfg;
    EnetUdma_OpenTxChPrms enetTxChCfg;
    Enet_IoctlPrms prms;
    bool csumOffloadFlg;
    bool useRingMon = BFALSE;
    bool useDfltFlow = gEnetAppCfg.useDfltFlow;
    uint32_t coreId = EnetSoc_getCoreId();

    if (pMcmCmdIf->hMboxCmd == NULL)
    {
        status = EnetApp_init();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to open Enet app: %d\n", status);
            EnetAppUtils_assert(status == ENET_SOK);
        }
        EnetAppUtils_print("%s: EnetApp_init() done\n", __func__);

        EnetMcm_getCmdIf(gEnetAppCfg.enetType, pMcmCmdIf);
    }

    EnetAppUtils_assert(pMcmCmdIf->hMboxCmd != NULL);
    EnetAppUtils_assert(pMcmCmdIf->hMboxResponse != NULL);
    EnetMcm_acquireHandleInfo(pMcmCmdIf, &handleInfo);
    EnetMcm_coreAttach(pMcmCmdIf, coreId, &attachInfo);

    /* Confirm HW checksum offload is enabled when LWIP chksum offload is enabled */
    ENET_IOCTL_SET_OUT_ARGS(&prms, &csumOffloadFlg);
    status = Enet_ioctl(handleInfo.hEnet,
                        coreId,
                        ENET_HOSTPORT_IS_CSUM_OFFLOAD_ENABLED,
                        &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to get checksum offload info: %d\n", status);
    }

    EnetAppUtils_assert(BTRUE == csumOffloadFlg);

    /* Open TX channel */
    EnetUdma_initTxChParams(&enetTxChCfg);
    EnetAppUtils_setCommonTxChPrms(&enetTxChCfg);

    enetTxChCfg.hUdmaDrv  = handleInfo.hUdmaDrv;
    enetTxChCfg.useProxy  = BTRUE;
    enetTxChCfg.numTxPkts = inArgs->txCfg.numPackets;
    enetTxChCfg.cbArg     = inArgs->txCfg.cbArg;
    enetTxChCfg.notifyCb  = inArgs->txCfg.notifyCb;

    EnetAppUtils_openTxCh(handleInfo.hEnet,
                          attachInfo.coreKey,
                          coreId,
                          &outArgs->txInfo.txChNum,
                          &outArgs->txInfo.hTxChannel,
                          &enetTxChCfg);

    /* Open first RX channel/flow */
    rxInfo = &outArgs->rxInfo[0U];
    rxCfg = &inArgs->rxCfg[0U];

    EnetUdma_initRxFlowParams(&enetRxFlowCfg);
    EnetAppUtils_setCommonRxFlowPrms(&enetRxFlowCfg);
    enetRxFlowCfg.notifyCb  = rxCfg->notifyCb;
    enetRxFlowCfg.numRxPkts = rxCfg->numPackets;
    enetRxFlowCfg.cbArg     = rxCfg->cbArg;
    enetRxFlowCfg.hUdmaDrv  = handleInfo.hUdmaDrv;
    enetRxFlowCfg.useProxy  = BTRUE;

    /* Use ring monitor for the CQ ring of RX flow */
    EnetUdma_UdmaRingPrms *pFqRingPrms = &enetRxFlowCfg.udmaChPrms.fqRingPrms;
    pFqRingPrms->useRingMon = useRingMon;
    pFqRingPrms->ringMonCfg.mode = TISCI_MSG_VALUE_RM_MON_MODE_THRESHOLD;
    /* Ring mon low threshold */

#if defined _DEBUG_
    /* In debug mode as CPU is processing lesser packets per event, keep threshold more */
    pFqRingPrms->ringMonCfg.data0 = (rxCfg->numPackets - 10U);
#else
    pFqRingPrms->ringMonCfg.data0 = (rxCfg->numPackets - 20U);
#endif
    /* Ring mon high threshold - to get only low  threshold event, setting high threshold as more than ring depth*/
    pFqRingPrms->ringMonCfg.data1 = rxCfg->numPackets;

    EnetAppUtils_openRxFlow(gEnetAppCfg.enetType,
                            handleInfo.hEnet,
                            attachInfo.coreKey,
                            coreId,
                            useDfltFlow,
                            &rxInfo->rxFlowStartIdx,
                            &rxInfo->rxFlowIdx,
                            &rxInfo->macAddr[0U],
                            &rxInfo->hRxFlow,
                            &enetRxFlowCfg);

    outArgs->coreId          = coreId;
    outArgs->coreKey         = attachInfo.coreKey;
    outArgs->handleArg       = (void *)handleInfo.hEnet;
    outArgs->hostPortRxMtu   = attachInfo.rxMtu;
    ENET_UTILS_ARRAY_COPY(outArgs->txMtu, attachInfo.txMtu);
    outArgs->print           = &EnetAppUtils_print;
    outArgs->isPortLinkedFxn = &EnetApp_isPortLinked;
    outArgs->timerPeriodUs   = ENETAPP_PACKET_POLL_PERIOD_US;
    outArgs->txCsumOffloadEn = BTRUE;
    outArgs->rxCsumOffloadEn = BTRUE;

    outArgs->hUdmaDrv = handleInfo.hUdmaDrv;
    rxInfo->disableEvent = !useRingMon;
    outArgs->txInfo.txPortNum = ENET_MAC_PORT_INV;
    /* Use optimized processing where TX packets are relinquished in next TX submit call */
    outArgs->txInfo.disableEvent = BTRUE;

    EnetAppUtils_print("Host MAC address: ");
    EnetAppUtils_printMacAddr(&rxInfo->macAddr[0U]);

    EnetUtils_copyMacAddr(&gEnetAppObj.macAddr[0U], &rxInfo->macAddr[0U]);

    rxInfo->handlePktFxn = NULL;
}

void LwipifEnetAppCb_releaseHandle(LwipifEnetAppIf_ReleaseHandleInfo *releaseInfo)
{
    Enet_Handle hEnet = (Enet_Handle)releaseInfo->handleArg;
    EnetMcm_CmdIf *pMcmCmdIf = &gEnetAppObj.hMcmCmdIf[gEnetAppCfg.enetType];
    Lwip2EnetAppIf_FreePktInfo *freePktInfo;
    EnetDma_PktQ fqPktInfoQ;
    EnetDma_PktQ cqPktInfoQ;
    LwipifEnetAppIf_RxHandleInfo *rxInfo;
    bool useDfltFlow = gEnetAppCfg.useDfltFlow;

    EnetAppUtils_assert(pMcmCmdIf->hMboxCmd != NULL);
    EnetAppUtils_assert(pMcmCmdIf->hMboxResponse != NULL);

    /* Close TX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);
    EnetAppUtils_closeTxCh(hEnet,
                           releaseInfo->coreKey,
                           releaseInfo->coreId,
                           &fqPktInfoQ,
                           &cqPktInfoQ,
                           releaseInfo->txInfo.hTxChannel,
                           releaseInfo->txInfo.txChNum);
    releaseInfo->txFreePkt.cb(releaseInfo->txFreePkt.cbArg, &fqPktInfoQ, &cqPktInfoQ);

    /* Close first RX channel/flow */
    freePktInfo = &releaseInfo->rxFreePkt[0U];

    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    rxInfo = &releaseInfo->rxInfo[0U];
    EnetAppUtils_closeRxFlow(gEnetAppCfg.enetType,
                             hEnet,
                             releaseInfo->coreKey,
                             releaseInfo->coreId,
                             useDfltFlow,
                             &fqPktInfoQ,
                             &cqPktInfoQ,
                             rxInfo->rxFlowStartIdx,
                             rxInfo->rxFlowIdx,
                             rxInfo->macAddr,
                             rxInfo->hRxFlow);

    freePktInfo->cb(freePktInfo->cbArg, &fqPktInfoQ, &cqPktInfoQ);

    EnetMcm_coreDetach(pMcmCmdIf, releaseInfo->coreId, releaseInfo->coreKey);
    EnetMcm_releaseHandleInfo(pMcmCmdIf);
    EnetMcm_releaseCmdIf(gEnetAppCfg.enetType, pMcmCmdIf);
}

void LwipifEnetAppCb_openDma(LwipifEnetAppIf_GetHandleInArgs *inArgs,
                             LwipifEnetAppIf_GetHandleOutArgs *outArgs)
{
    EnetMcm_HandleInfo handleInfo;
    EnetDma_Handle hDma;
    EnetPer_AttachCoreOutArgs attachInfo;
    LwipifEnetAppIf_RxHandleInfo *rxInfo;
    LwipifEnetAppIf_RxConfig *rxCfg;
    EnetUdma_OpenRxFlowPrms enetRxFlowCfg;
    EnetUdma_OpenTxChPrms enetTxChCfg;
    bool useRingMon = BFALSE;
    EnetMcm_CmdIf *pMcmCmdIf = &gEnetAppObj.hMcmCmdIf[gEnetAppCfg.enetType];
    uint32_t coreId = EnetSoc_getCoreId();
    int32_t status = ENET_SOK;

    EnetAppUtils_assert(pMcmCmdIf->hMboxCmd != NULL);
    EnetAppUtils_assert(pMcmCmdIf->hMboxResponse != NULL);
    EnetMcm_acquireHandleInfo(pMcmCmdIf, &handleInfo);
    EnetMcm_coreAttach(pMcmCmdIf, coreId, &attachInfo);

    /* Open TX channel */
    EnetUdma_initTxChParams(&enetTxChCfg);
    EnetAppUtils_setCommonTxChPrms(&enetTxChCfg);

    enetTxChCfg.hUdmaDrv  = handleInfo.hUdmaDrv;
    enetTxChCfg.useProxy  = BTRUE;
    enetTxChCfg.numTxPkts = inArgs->txCfg.numPackets;
    enetTxChCfg.cbArg     = inArgs->txCfg.cbArg;
    enetTxChCfg.notifyCb  = inArgs->txCfg.notifyCb;
    enetTxChCfg.chNum     = outArgs->txInfo.txChNum;

    hDma = Enet_getDmaHandle(handleInfo.hEnet);
    EnetAppUtils_assert(hDma != NULL);

    outArgs->txInfo.hTxChannel = EnetUdma_openTxCh(hDma, &enetTxChCfg);
    EnetAppUtils_assert(outArgs->txInfo.hTxChannel != NULL);

    /* Open first RX channel/flow */
    rxInfo = &outArgs->rxInfo[0U];
    rxCfg = &inArgs->rxCfg[0U];

    EnetUdma_initRxFlowParams(&enetRxFlowCfg);
    EnetAppUtils_setCommonRxFlowPrms(&enetRxFlowCfg);
    enetRxFlowCfg.notifyCb  = rxCfg->notifyCb;
    enetRxFlowCfg.numRxPkts = rxCfg->numPackets;
    enetRxFlowCfg.cbArg     = rxCfg->cbArg;
    enetRxFlowCfg.hUdmaDrv  = handleInfo.hUdmaDrv;
    enetRxFlowCfg.useProxy  = BTRUE;
    enetRxFlowCfg.startIdx  = rxInfo->rxFlowStartIdx;
    enetRxFlowCfg.flowIdx   = rxInfo->rxFlowIdx;
    rxInfo->handlePktFxn    = NULL;

    /* Use ring monitor for the CQ ring of RX flow */
    EnetUdma_UdmaRingPrms *pFqRingPrms = &enetRxFlowCfg.udmaChPrms.fqRingPrms;
    pFqRingPrms->useRingMon = useRingMon;
    pFqRingPrms->ringMonCfg.mode = TISCI_MSG_VALUE_RM_MON_MODE_THRESHOLD;
    /* Ring mon low threshold */

#if defined _DEBUG_
    /* In debug mode as CPU is processing lesser packets per event, keep threshold more */
    pFqRingPrms->ringMonCfg.data0 = (rxCfg->numPackets - 10U);
#else
    pFqRingPrms->ringMonCfg.data0 = (rxCfg->numPackets - 20U);
#endif
    /* Ring mon high threshold - to get only low  threshold event, setting high threshold as more than ring depth*/
    pFqRingPrms->ringMonCfg.data1 = rxCfg->numPackets;

    rxInfo->hRxFlow = EnetUdma_openRxFlow(hDma, &enetRxFlowCfg);
    EnetAppUtils_assert(rxInfo->hRxFlow != NULL);

    status = EnetAppUtils_regDfltRxFlow(handleInfo.hEnet,
                                        attachInfo.coreKey,
                                        coreId,
                                        rxInfo->rxFlowStartIdx,
                                        rxInfo->rxFlowIdx);
    EnetAppUtils_assert(status == ENET_SOK);
}

void LwipifEnetAppCb_closeDma(LwipifEnetAppIf_ReleaseHandleInfo *releaseInfo)
{
    Enet_Handle hEnet = (Enet_Handle)releaseInfo->handleArg;
    Lwip2EnetAppIf_FreePktInfo *freePktInfo;
    EnetDma_PktQ fqPktInfoQ;
    EnetDma_PktQ cqPktInfoQ;
    LwipifEnetAppIf_RxHandleInfo *rxInfo;
    int32_t status;

    /* Close TX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    EnetDma_disableTxEvent(releaseInfo->txInfo.hTxChannel);
    status = EnetUdma_closeTxCh(releaseInfo->txInfo.hTxChannel, &fqPktInfoQ, &cqPktInfoQ);
    EnetAppUtils_assert(status == ENET_SOK);

    releaseInfo->txFreePkt.cb(releaseInfo->txFreePkt.cbArg, &fqPktInfoQ, &cqPktInfoQ);

    /* Close first RX channel/flow */
    freePktInfo = &releaseInfo->rxFreePkt[0U];

    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    rxInfo = &releaseInfo->rxInfo[0U];

    status = EnetAppUtils_unregDfltRxFlow(hEnet,
                                          releaseInfo->coreKey,
                                          releaseInfo->coreId,
                                          rxInfo->rxFlowStartIdx,
                                          rxInfo->rxFlowIdx);
    EnetAppUtils_assert(status == ENET_SOK);

    status = EnetUdma_closeRxFlow(rxInfo->hRxFlow, &fqPktInfoQ, &cqPktInfoQ);
    EnetAppUtils_assert(status == ENET_SOK);

    freePktInfo->cb(freePktInfo->cbArg, &fqPktInfoQ, &cqPktInfoQ);
}

/*
 * Close UDMA LLD and Enet LLD
 */
static void EnetApp_deinit(void)
{
    EnetAppUtils_udmaclose(gEnetAppObj.hUdmaDrv);
    EnetMcm_deInit(gEnetAppCfg.enetType);
    memset(&gEnetAppObj, 0U, sizeof(gEnetAppObj));
}

static int32_t EnetApp_initMMCSD(void)
{
    int32_t status = ENET_SOK;
    FATFS_Error fsErr;

    fsErr = FATFS_init();
    if (fsErr == FATFS_ERR)
    {
        EnetAppUtils_print("%s: Failed to init FATFS\n", __func__);
        status=ENET_EFAIL;
    }
    fsErr = FATFS_open(0U, NULL, &gEnetAppObj.fatfsHandle);
    if (fsErr == FATFS_ERR)
    {
        EnetAppUtils_print("%s: Failed to open FATFS\n", __func__);
        status=ENET_EFAIL;
    }

    return status;
}

static int EnetApp_initTsn(void)
{
    lld_ethdev_t ethdevs[MAX_NUMBER_ENET_DEVS] = {0};
    int i;
    int res = -1;
    const LLDTsyncTsSource tsSource = LLDTSYNC_TS_SOURCE_CPTS;
    AppTsnCfg_t appTsnCfg =
    {
        .consoleOutCb = UART_printf,
    };

    for (i = 0; i < gEnetAppCfg.numEthPorts; i++)
    {
        snprintf(&gEnetApp_netDevs[i][0], CB_MAX_NETDEVNAME, "tilld%d", i);
        appTsnCfg.netdevs[i] = &gEnetApp_netDevs[i][0];
        ethdevs[i].netdev = gEnetApp_netDevs[i];
        ethdevs[i].macport = gEnetAppCfg.ethPorts[i].macPort;
        if (i == 0)
        {
            /* tilld0 reuses the allocated source mac, other interfaces will allocate
             * the mac by themself */
            memcpy(ethdevs[i].srcmac, &gEnetAppObj.macAddr[0U], ENET_MAC_ADDR_LEN);
        }
    }
    appTsnCfg.netdevs[i] = NULL;
    if (EnetApp_initTsnByCfg(&appTsnCfg) < 0)
    {
        EnetAppAbort("Failed to tsn_app_init!\n");
    }
    if (cb_lld_init_devs_table(ethdevs, i, gEnetAppCfg.enetType,
                               gEnetAppCfg.instId,
                               tsSource) < 0)
    {
        EnetAppAbort("Failed to int devs table!\n");
    }
    if (EnetApp_startTsn() < 0)
    {
        EnetAppAbort("Failed to start gptp!\n");
    }
    EnetAppUtils_print("%s:gptp start done!\n", __func__);
    res = 0;
    return res;
}

