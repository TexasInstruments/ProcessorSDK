/*
 *  Copyright (c) Texas Instruments Incorporated 2023
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

/* TI Driver Header files */
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_mcm.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>
#include <ti/csl/csl_intr_router.h>


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
#include <tsn_combase/combase_link.h>
#include <tsn_combase/tilld/cb_lld_ethernet.h>
#include "debug_log.h"
#include "tsninit.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Macro to abort app during fatal erros */
#define EnetAppAbort(message) \
    EnetAppUtils_print(message); \
    EnetAppUtils_assert(BFALSE);

/* Maximum number of Ethernet ports (CPSW9G in J721E/J784S4) */
#define MAX_NUM_MAC_PORTS               (8U)

/* TimeSync router SYNC0_OUT source interrupt */
#define TIMESYNC_INTR_SYNC0_OUT_PIN     (34U)
/* TimeSync router SYNC1_OUT source interrupt */
#define TIMESYNC_INTR_SYNC1_OUT_PIN     (35U)
/* TimeSync router SYNC2_OUT source interrupt */
#define TIMESYNC_INTR_SYNC2_OUT_PIN     (36U)
/* TimeSync router SYNC3_OUT source interrupt */
#define TIMESYNC_INTR_SYNC3_OUT_PIN     (37U)

/* Function generator length value for a given ref clk */
#define TIMESYNC_REF_CLK_LEN            (500*1000)

/* PPS Compare time in nsecs (20secs) */
#define TIMESYNC_GENF_COMP_LEN          (20000000000U)

/* Macro to round off time to nano second boundary */
#define TIMESYNC_TIME_SEC_TO_NS         (1000000000U)

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
} EnetApp_Cfg;

/* DMA resources allocated to a given core */
typedef struct DmaResource_s
{
    /* Processing core Id */
    uint32_t coreId;

    /* Number of TX channels */
    uint32_t numTxCh;

    /* Array of TX channels allocated for a given core */
    EnetRm_TxCh txCh[ENET_CFG_TX_CHANNELS_NUM];

    /* Keep track current index of the DMA array
     * above for allocating new DMA channel
     */
    uint32_t  txChAllocIdx;
} DmaResource;

typedef struct EnetTsn_DmaResource_s
{
    /*! Number of cores */
    uint32_t numCores;

    /* DMA resource info allocated to all the cores */
    DmaResource dmaResInfo[ENET_CFG_REMOTE_CLIENT_CORES_MAX];
} EnetTsn_DmaResource;

/* Container structure of all example app variables */
typedef struct EnetApp_Obj_s
{
    /* Enet driver handle for this peripheral type/instance */
    Enet_Handle hEnet;

    /* Processing core Id */
    uint32_t coreId;

    /* UDMA driver handle */
    Udma_DrvHandle hUdmaDrv;

    /* Handle to task which performs board initialization, initializes
     * TSN stack and run CPU load periodically */
    TaskP_Handle hAppInitTask;

    /* FATFS handle */
    FATFS_Handle fatfsHandle;

    /* Static configuration for DMA channels */
    EnetTsn_DmaResource dmaRes;
} EnetApp_Obj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t EnetApp_init(void);
static void EnetApp_deinit(void);
static void EnetApp_appInitTask(void *a0, void *a1);
static int32_t EnetApp_initMMCSD(void);
static int EnetApp_initTsn(void);
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
    params.priority  = 15U;
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

    EnetAppUtils_print("==========================\n");
    EnetAppUtils_print("     TSN Example App      \n");
    EnetAppUtils_print("==========================\n");

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

    /* Open Enet LLD and UDMA LLD */
    status = EnetApp_init();
    EnetAppUtils_assert(status == ENET_SOK);
    EnetAppUtils_print("%s: EnetApp_init() done\n", __func__);

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
    cb_lld_notify_linkchange();
}

/*
 * Callback called by Enet LLD when MDIO reports a link status change, if MDIO is in normal mode
 */
static void EnetApp_mdioLinkStatusChange(Cpsw_MdioLinkStateChangeInfo *info,
                                         void *appArg)
{
}

uint64_t EnetApp_getCurrentTime(void)
{
    Enet_IoctlPrms prms;
    int32_t status;
    uint64_t tsVal = 0ULL;

    /* Software push event */
    ENET_IOCTL_SET_OUT_ARGS(&prms, &tsVal);
    status = Enet_ioctl(gEnetAppObj.hEnet, gEnetAppObj.coreId, ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP, &prms);
    if (status != ENET_SOK)
    {
        tsVal = 0ULL;
    }

    return tsVal;
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
    uint32_t j;
    uint32_t numTxCh;
    Enet_IoctlPrms prms;
    CSL_IntrRouterCfg irRegs;
    EnetMcm_CmdIf hMcmCmdIf;
    EnetMcm_HandleInfo handleInfo;
    CpswCpts_SetFxnGenInArgs setGenFInArgs;
    Enet_Version version;
    uint32_t tsrIn;
    uint32_t tsrOut;

#if defined(SOC_J721E) || defined (SOC_J7200)
    tsrIn = CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_GENF0_0;
    tsrOut = TIMESYNC_INTR_SYNC2_OUT_PIN;
#elif defined(SOC_J784S4) || defined(SOC_J742S2)
    tsrIn = CSLR_TIMESYNC_INTRTR0_IN_CPSW_9XUSSM0_CPTS_GENF0_0;
    tsrOut = TIMESYNC_INTR_SYNC3_OUT_PIN;
#elif defined(SOC_J721S2)
    tsrIn = CSLR_TIMESYNC_INTRTR0_IN_CPSW1_CPTS_GENF0_0;
    tsrOut = TIMESYNC_INTR_SYNC3_OUT_PIN;
#else
#error "Unsupported SoC"
#endif

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

    /* Hardware switch priority is taken from packet's PCP or DSCP */
    cpswCfg.hostPortCfg.rxVlanRemapEn     = BTRUE;
    cpswCfg.hostPortCfg.rxDscpIPv4RemapEn = BTRUE;
    cpswCfg.hostPortCfg.rxDscpIPv6RemapEn = BTRUE;

    /* CPTS_RFT_CLK is sourced from MAIN_SYSCLK0 (500MHz) */
    cpswCfg.cptsCfg.cptsRftClkFreq = CPSW_CPTS_RFTCLK_FREQ_500MHZ;
    cpswCfg.cptsCfg.tsGenfClrEn = BTRUE;

    EnetApp_initEnetLinkCbPrms(&cpswCfg);
    gEnetAppObj.coreId = EnetSoc_getCoreId();

    EnetApp_initAleConfig(&cpswCfg.aleCfg);

    // Do this to avoid future param added to EnetMcm_InitConfig without init
    memset(&enetMcmCfg, 0, sizeof(EnetMcm_InitConfig));
    /* Configure MCM (Multi-Client Manager) parameters: peripheral type, port ids, etc */
    enetMcmCfg.perCfg = &cpswCfg;
    EnetAppUtils_assert(NULL != enetMcmCfg.perCfg);

    EnetAppUtils_initResourceConfig(gEnetAppCfg.enetType, gEnetAppCfg.instId,
                                    EnetSoc_getCoreId(), resCfg);

    resCfg->resPartInfo.isStaticTxChanAllocated = BTRUE;
    for (i = 0U; i < ENET_CFG_REMOTE_CLIENT_CORES_MAX; i++)
    {
        if (resCfg->resPartInfo.coreResInfo[i].coreId == gEnetAppObj.coreId)
        {
            DmaResource *dmaResInfo = &gEnetAppObj.dmaRes.dmaResInfo[i];
            dmaResInfo->coreId = resCfg->resPartInfo.coreResInfo[i].coreId;
            dmaResInfo->numTxCh = resCfg->resPartInfo.coreResInfo[i].numTxCh;
            numTxCh = resCfg->resPartInfo.coreResInfo[i].numTxCh;
            for (j = 0U; j < numTxCh; j++)
            {
                if (j == 0)
                {
                    resCfg->resPartInfo.coreResInfo[i].txCh[j] = ENET_RM_TX_CH_7;
                }
                else
                {
                    resCfg->resPartInfo.coreResInfo[i].txCh[j] = j;
                }
                dmaResInfo->txCh[j] = resCfg->resPartInfo.coreResInfo[i].txCh[j];
                /* Reserve channel 0 for PTP and remaining channels are for other applications */
                dmaResInfo->txChAllocIdx = 1U;
            }
        }
    }

    enetMcmCfg.enetType           = gEnetAppCfg.enetType;
    enetMcmCfg.instId             = gEnetAppCfg.instId;
    enetMcmCfg.setPortLinkCfg     = EnetApp_initLinkArgs;
    enetMcmCfg.numMacPorts        = gEnetAppCfg.numEthPorts;
    enetMcmCfg.periodicTaskPeriod = ENETPHY_FSM_TICK_PERIOD_MS;
    enetMcmCfg.print              = EnetAppUtils_print;

    for (i = 0U; i < gEnetAppCfg.numEthPorts; i++)
    {
        enetMcmCfg.macPortList[i] = gEnetAppCfg.ethPorts[i].macPort;
    }

    /* Initialize MCM which internally opens Enet LLD */
    status = EnetMcm_init(&enetMcmCfg);

    EnetMcm_getCmdIf(gEnetAppCfg.enetType, &hMcmCmdIf);
    EnetMcm_acquireHandleInfo(&hMcmCmdIf, &handleInfo);
    gEnetAppObj.hEnet = handleInfo.hEnet;

    irRegs.pIntrRouterRegs = (CSL_intr_router_cfgRegs *) CSL_TIMESYNC_INTRTR0_INTR_ROUTER_CFG_BASE;
    irRegs.pIntdRegs       = (CSL_intr_router_intd_cfgRegs *) NULL;
    irRegs.numInputIntrs   = ENET_TIMESYNCRTR_NUM_INPUT;
    irRegs.numOutputIntrs  = ENET_TIMESYNCRTR_NUM_OUTPUT;

    CSL_intrRouterCfgMux(&irRegs, tsrIn, tsrOut);

    ENET_IOCTL_SET_OUT_ARGS(&prms, &version);
    status = Enet_ioctl(gEnetAppObj.hEnet,
                        gEnetAppObj.coreId,
                        ENET_TIMESYNC_IOCTL_GET_VERSION,
                        &prms);

    EnetAppUtils_print("CPTS version: %u.%u.%u \n",version.maj,
                       version.min, version.rtl);

    if (ENET_SOK == status)
    {
        /* Configure GENF0 to generate pulse signal */
        /* Length should be same for both master and slave */
        setGenFInArgs.index  = 0U;
        setGenFInArgs.length = TIMESYNC_REF_CLK_LEN;

        /* Add compare value 20 secs in the future to ensure GENF compare happens
         * and PPS gets generated after the master and slave gets synchronized and reaches
         * stable state */
        setGenFInArgs.compare = ((EnetApp_getCurrentTime()/TIMESYNC_TIME_SEC_TO_NS)+1)*TIMESYNC_TIME_SEC_TO_NS
                                + TIMESYNC_GENF_COMP_LEN;
        setGenFInArgs.polarityInv = BTRUE;
        setGenFInArgs.ppmVal  = 0U;
        setGenFInArgs.ppmDir  = ENET_TIMESYNC_ADJDIR_INCREASE;
        setGenFInArgs.ppmMode = ENET_TIMESYNC_ADJMODE_DISABLE;

        ENET_IOCTL_SET_IN_ARGS(&prms, &setGenFInArgs);
        status = Enet_ioctl(gEnetAppObj.hEnet,
                            gEnetAppObj.coreId,
                            CPSW_CPTS_IOCTL_SET_GENF,
                            &prms);
    }

    return status;
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

static int EnetAppTsnCfgUpdateCb(cb_socket_lldcfg_update_t *updatedCfg)
{
    int res = -1;
    uint32_t i;
    uint32_t coreId = EnetSoc_getCoreId();
    for (i = 0U; i < ENET_CFG_REMOTE_CLIENT_CORES_MAX; i++)
    {
        DmaResource *dmaResInfo = &gEnetAppObj.dmaRes.dmaResInfo[i];
        if (dmaResInfo->coreId == coreId)
        {
            if (updatedCfg->proto == ETH_P_NETLINK)
            {
                /* No DMA channel is allocated for netlink sock */
                updatedCfg->dmaTxChId = -1;
            }
            else if (updatedCfg->proto == ETH_P_1588)
            {
                /* We always allocate DMA channel at the index 0 for the PTP */
                updatedCfg->dmaTxChId = dmaResInfo->txCh[0];
            }
            else
            {
                EnetAppUtils_assert(dmaResInfo->txChAllocIdx < dmaResInfo->numTxCh);
                updatedCfg->dmaTxChId = dmaResInfo->txCh[dmaResInfo->txChAllocIdx];
                dmaResInfo->txChAllocIdx++;
            }
            res = 0;
            break;
        }
    }
    return res;
}

static int EnetApp_initTsn(void)
{
    lld_ethdev_t ethdevs[MAX_NUMBER_ENET_DEVS] = {0};
    int i;
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
    }
    appTsnCfg.netdevs[i] = NULL;

    cb_socket_set_lldcfg_update_cb(EnetAppTsnCfgUpdateCb);

    if (EnetApp_initTsnByCfg(&appTsnCfg) < 0)
    {
        EnetAppAbort("Failed to EnetApp_initTsnByCfg()!\n");
    }
    if (cb_lld_init_devs_table(ethdevs, i, gEnetAppCfg.enetType,
                               gEnetAppCfg.instId,
                               tsSource) < 0)
    {
        EnetAppAbort("Failed to cb_lld_init_devs_table()!\n");
    }
    if (EnetApp_startTsn() < 0)
    {
        EnetAppAbort("Failed to EnetApp_startTsn()!\n");
    }
    EnetAppUtils_print("%s:EnetApp_startTsn() done!\n", __func__);
    return 0;
}
