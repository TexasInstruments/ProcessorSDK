/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 * \file  enet_ecc_test.c
 *
 * \brief This file contains the implementation of the Enet Ecc test.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */


#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>

#include <sdl_ecc.h>
#include <sdl_esm.h>
#include <sdl_types.h>
#include <src/sdl/sdl_ecc.h>
#include <sdl_exception.h>
#include <sdl_esm.h>
#include <osal/sdl_osal.h>
#include <ti/csl/csl_ecc_aggr.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* length of frame payload used to calculate frame size needed */
#define ENETECC_TEST_PKT_LEN                      (50U)

#if defined(SOC_J721E) || defined(SOC_J7200)
#define MCU_CPSW0_AGGR0_MAX_MEM_SECTIONS          (3U)
#else
#define MCU_CPSW0_AGGR0_MAX_MEM_SECTIONS          (5U)
#endif

#if defined(SOC_J721E)
#define SDL_ECC_MEMTYPE                           SDL_ECC_MEMTYPE_MCU_CPSW0
#elif defined(SOC_J7200)
#define SDL_ECC_MEMTYPE                           SDL_ECC_MCU_CPSW0_ECC_AGGR
#else
#define SDL_ECC_MEMTYPE                           SDL_MCU_CPSW0_ECC_AGGR
#endif

#define ENETLPBK_NUM_ITERATION                    (5U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct EnetEcc_Obj_s
{
    /* Enet driver */
    Enet_Handle hEnet;
    Enet_Type enetType;
    uint32_t instId;
    uint32_t coreId;
    uint32_t coreKey;
    uint32_t boardId;
    uint32_t expPort;
    Enet_MacPort macPort;
    uint8_t useSGMII;

    /* UDMA driver handle */
    Udma_DrvHandle hUdmaDrv;
    uint32_t rxFlowIdx;
    uint32_t rxStartFlowIdx;
    EnetDma_RxChHandle hRxCh;
    EnetDma_PktQ rxFreeQ;
    EnetDma_PktQ rxReadyQ;
    EnetDma_TxChHandle hTxCh;
    EnetDma_PktQ txFreePktInfoQ;
    uint32_t txChNum;
    uint8_t hostMacAddr[ENET_MAC_ADDR_LEN];
} EnetEcc_Obj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void EnetEcc_getEnetTypeIdMode(Enet_Type *enetType,
                                             uint32_t *instId,
                                             uint8_t *useSGMII);

static void EnetEcc_initCpswCfg(Cpsw_Cfg *cpswCfg);

static int32_t EnetEcc_setupCpswAle(void);

static int32_t EnetEcc_openEnet(void);

static int32_t EnetEcc_openDma(void);

static void EnetEcc_initTxFreePktQ(void);

static void EnetEcc_initTxFreePktQ(void);

static int32_t EnetEcc_setupCpswAle(void);

static void EnetEcc_txSingle(void);

static void EnetEcc_rxSingle(void);

static void EnetEcc_showCpswStats(void);

static void EnetEcc_closeDma(void);

static void EnetEcc_closeEnet(void);

static int32_t EnetEcc_waitForLinkUp(void);

int32_t cpsw_sdl_test(void);

int32_t runECC2Bit_RAMInjectTest(void);

int32_t SDL_TEST_osalInit(void);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Enet test object */
EnetEcc_Obj gEnetEcc;

/* Global transfer and receive interrupt flags */
volatile uint8_t txFlag;
volatile uint8_t rxFlag;
static uint32_t arg;
/* Global variable storing number of ecc error count packets received */
static uint32_t eccErrCount=0;

SDL_ESM_config ECC_Test_esmInitConfig_MCU =
{
    /* Self test error config */
    .esmErrorConfig = { 0u, 3u },

    /* All events enable: except timer and self test  events, and Main ESM output
     * Temporarily disabling vim compare error as well */
    .enableBitmap   = { 0xffffffffu, 0xfe3fffffu, 0x7fffffffu, 0x00000007u, },

    /* All events high priority: except timer, selftest error events, and Main ESM output */
    .priorityBitmap = { 0xffffffffu, 0xff3fffffu, 0x7fffffffu, 0x00000007u, },

    /* All events high priority: except timer, selftest error events, and Main ESM output */
    .errorpinBitmap = { 0xffffffffu, 0xff3fffffu, 0x7fffffffu, 0x00000007u, },
};

#if defined(SOC_J721E) || defined(SOC_J7200)
static SDL_ECC_MemSubType ECC_Test_SDL_MCU_CPSW0_0_subMemTypeList[MCU_CPSW0_AGGR0_MAX_MEM_SECTIONS] =
{
    SDL_MCU_CPSW0_P1_RX_FIFO_RAM_ID,
    SDL_MCU_CPSW0_P1_TX_FIFO_RAM_ID,
    SDL_MCU_CPSW0_EST_RAM_RAM_ID,
};

static SDL_ECC_InitConfig_t ECC_Test_SDL_MCU_CPSW0_0_initConfig =
{
    /* Number of Rams ECC is enabled */
    .numRams = MCU_CPSW0_AGGR0_MAX_MEM_SECTIONS,
    /* Subtype list */
    .pMemSubTypeList = &(ECC_Test_SDL_MCU_CPSW0_0_subMemTypeList[0]),
};
#else
static SDL_ECC_MemSubType ECC_Test_SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_subMemTypeList[MCU_CPSW0_AGGR0_MAX_MEM_SECTIONS] =
{
    SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_CPSW_2GU_CORE_ECC_ECC_CTRL1_RAM_ID,
    SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_CPSW_2GU_CORE_ECC_ECC_CTRL2_RAM_ID,
    SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_CPSW_2GU_CORE_ECC_ECC_CTRL3_RAM_ID,
    SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_CPSW_2GU_CORE_ECC_ECC_CTRL4_RAM_ID,
    SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_EST_RAM_RAM_ID,
};

static SDL_ECC_InitConfig_t ECC_Test_SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_initConfig =
{
    .numRams = MCU_CPSW0_AGGR0_MAX_MEM_SECTIONS,
    /**< Number of Rams ECC is enabled */
    .pMemSubTypeList = &(ECC_Test_SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_subMemTypeList[0]),
    /**< Sub type list */
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    EnetUtils_Cfg utilsCfg;
    Enet_IoctlPrms prms;
    int32_t status;
    uint32_t pktCnt;

    /* Initialize interrupt flags */
    txFlag = 0U;
    rxFlag = 0U;

    /* Initialize global test config object */
    memset(&gEnetEcc, 0U, sizeof(gEnetEcc));

    /* Basic common configs for simple MAC loopback test */
    gEnetEcc.boardId = ENETBOARD_LOOPBACK_ID;
    gEnetEcc.expPort = ENETBOARD_EXP_PORT_NONE;
    gEnetEcc.macPort = ENET_MAC_PORT_1;

    /* Different configs for different boards and cores */
    EnetEcc_getEnetTypeIdMode(&gEnetEcc.enetType,
                                     &gEnetEcc.instId,
                                     &gEnetEcc.useSGMII);

    /* Initialize the board */
    status = EnetBoard_init();
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to init enet board: %d\n", status);
    }
    EnetAppUtils_print("\nEnet ECC Test Start\n");

    /* Start the clock */
    EnetAppUtils_enableClocks(gEnetEcc.enetType, gEnetEcc.instId);

    /* Local core id */
    gEnetEcc.coreId = EnetSoc_getCoreId();

    /* Initialize Enet driver (use default utils) */
    Enet_initUtilsCfg(&utilsCfg);
    Enet_init(&utilsCfg);

    /* Open Enet driver */
    status = EnetEcc_openEnet();
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to open Enet driver: %d\n", status);
    }

    /* Attach the core with RM */
    if (status == ENET_SOK)
    {
        uint32_t coreId;
        EnetPer_AttachCoreOutArgs attachCoreOutArgs;
        coreId = gEnetEcc.coreId;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &coreId, &attachCoreOutArgs);
        status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, ENET_PER_IOCTL_ATTACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed ENET_PER_IOCTL_ATTACH_CORE: %d\n", status);
        }
        else
        {
            gEnetEcc.coreKey = attachCoreOutArgs.coreKey;
        }
    }

    /* Add broadcast entry in ALE table (DA of test packets) */
    if (status == ENET_SOK)
    {
        uint32_t setMcastOutArgs;
        CpswAle_SetMcastEntryInArgs setMcastInArgs;
        uint8_t bCastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

        memcpy(&setMcastInArgs.addr.addr[0], &bCastAddr[0U], sizeof(setMcastInArgs.addr.addr));
        setMcastInArgs.addr.vlanId     = 0U;
        setMcastInArgs.info.super      = BFALSE;
        setMcastInArgs.info.fwdState   = CPSW_ALE_FWDSTLVL_FWD;
        setMcastInArgs.info.portMask   = CPSW_ALE_ALL_PORTS_MASK;
        setMcastInArgs.info.numIgnBits = 0U;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);

        status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, CPSW_ALE_IOCTL_ADD_MCAST, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to add broadcast entry: %d\n", status);
        }
    }

    if (status == ENET_SOK)
    {
        /* memutils open should happen after Cpsw is opened as it uses CpswUtils_Q
         * functions */
        status = EnetMem_init();
        EnetAppUtils_assert(ENET_SOK == status);
    }

    /* Open DMA driver */
    if (status == ENET_SOK)
    {
        status = EnetEcc_openDma();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to open DMA: %d\n", status);
        }
    }

    /* Enable host port */
    if (status == ENET_SOK)
    {
        status = EnetEcc_setupCpswAle();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to setup CPSW ALE: %d\n", status);
        }

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_NO_ARGS(&prms);
            status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, ENET_HOSTPORT_IOCTL_ENABLE, &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Failed to enable host port: %d\n", status);
            }
        }
    }

    /* Wait for link up */
    if (status == ENET_SOK)
    {
        status = EnetEcc_waitForLinkUp();
    }

    /* Do packet transmission and reception */
    if (status == ENET_SOK)
    {
        /* Sending and receiving ENETLPBK_NUM_ITERATION number of packets */
        for (pktCnt = 0U; pktCnt < ENETLPBK_NUM_ITERATION; pktCnt++)
        {
            EnetEcc_txSingle();
            EnetEcc_rxSingle();
        }
        EnetEcc_showCpswStats();
    }

    /* Disable host port */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, ENET_HOSTPORT_IOCTL_DISABLE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to disable host port: %d\n", status);
        }
    }

    /* Close Enet DMA driver */
    EnetEcc_closeDma();

    /* Close Enet driver */
    EnetEcc_closeEnet();

    /* Disable peripheral clocks */
    EnetAppUtils_disableClocks(gEnetEcc.enetType, gEnetEcc.instId);

    /* Deinit Enet driver */
    Enet_deinit();

    /* Exactly 1 ecc error injected packet should be received by the application */
    if (eccErrCount == 1U)
    {
        EnetAppUtils_print("All tests have passed\n");
    }
    else
    {
        EnetAppUtils_print("Test failed %d ecc error injected packets received \n", eccErrCount);
    }

    return 0;
}

static void EnetEcc_getEnetTypeIdMode(Enet_Type *enetType,
                                             uint32_t *instId,
                                             uint8_t *useSGMII)
{
/* Default Most Common Option */
    *enetType = ENET_CPSW_2G;
    *instId = 0U;
    *useSGMII = 0U;
/* J7200 */
#if defined(SOC_J7200)
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
    /* Default */
#elif defined(BUILD_MCU2_0)
    *enetType = ENET_CPSW_5G;
#else
    EnetAppUtils_print("Invalid Core for J7200\n");
    EnetAppUtils_assert(BFALSE);
#endif
/* J721E */
#elif defined(SOC_J721E)
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
    /* Default */
#elif defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    *enetType = ENET_CPSW_9G;
#else
    EnetAppUtils_print("Invalid Core for J721E\n");
    EnetAppUtils_assert(BFALSE);
#endif
/* J721S2 */
#elif defined(SOC_J721S2)
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
    /* Default */
#elif defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    *instId = 1U;
#else
    EnetAppUtils_print("Invalid Core for J721S2\n");
    EnetAppUtils_assert(BFALSE);
#endif
/* J784S4 */
#elif defined(SOC_J784S4) || defined(SOC_J742S2)
#if defined(BUILD_MCU1_0)
    /* Default */
#elif defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    *enetType = ENET_CPSW_9G;
    *useSGMII = 1U;
#elif defined(BUILD_MCU2_1)
    *instId = 1U;
#else
    EnetAppUtils_print("Invalid Core for J784S4\n");
    EnetAppUtils_assert(BFALSE);
#endif
#else
    EnetAppUtils_print("Invalid Board\n");
    EnetAppUtils_assert(BFALSE);
#endif
}

static void EnetEcc_initCpswCfg(Cpsw_Cfg *cpswCfg)
{
    CpswHostPort_Cfg *hostPortCfg = &cpswCfg->hostPortCfg;
    CpswCpts_Cfg *cptsCfg = &cpswCfg->cptsCfg;

    cpsw_sdl_test();

    /* Set initial config */
    Enet_initCfg(gEnetEcc.enetType, gEnetEcc.instId, cpswCfg, sizeof(*cpswCfg));

    /* Host port config */
    hostPortCfg->removeCrc = BTRUE;

    /* CPTS config */
    /* Note: Timestamping and MAC loopback are not supported together because of
     * IP limitation, so disabling timestamping for this application */
    cptsCfg->hostRxTsEn = BFALSE;

    EnetAppUtils_initResourceConfig(gEnetEcc.enetType, gEnetEcc.instId, gEnetEcc.coreId, &cpswCfg->resCfg);
}

static int32_t EnetEcc_openEnet(void)
{
    Cpsw_Cfg cpswCfg;
    EnetDma_Cfg dmaCfg;
    Enet_IoctlPrms prms;
    EnetPer_PortLinkCfg portLinkCfg;
    CpswMacPort_Cfg macCfg;
    int32_t status = ENET_SOK;

    cpswCfg.dmaCfg = &dmaCfg;

    /* Initialize peripheral config */
    EnetEcc_initCpswCfg(&cpswCfg);
    EnetAppUtils_print("CPSW Test\n");

    /* Set DMA receive channel priority as default */
    dmaCfg.rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;

    /* App should open UDMA first as UDMA handle is needed to initialize CPSW RX channel */
    gEnetEcc.hUdmaDrv = EnetAppUtils_udmaOpen(gEnetEcc.enetType, NULL);
    EnetAppUtils_assert(NULL != gEnetEcc.hUdmaDrv);

    /* Global DMA driver handle */
    dmaCfg.hUdmaDrv = gEnetEcc.hUdmaDrv;

    /* Set Enet global runtime log level */
    Enet_setTraceLevel(ENET_TRACE_DEBUG);

    /* Open the Enet driver */
    gEnetEcc.hEnet = Enet_open(gEnetEcc.enetType,
                                      gEnetEcc.instId,
                                      &cpswCfg,
                                      sizeof(cpswCfg));
    if (gEnetEcc.hEnet == NULL)
    {
        EnetAppUtils_print("Failed to open Enet driver\n");
        status = ENET_EFAIL;
    }

    /* Setup link open parameters */
    if (status == ENET_SOK)
    {
        EnetMacPort_LinkCfg *linkCfg = &portLinkCfg.linkCfg;
        EnetMacPort_Interface *mii = &portLinkCfg.mii;
        EnetPhy_Cfg *phyCfg = &portLinkCfg.phyCfg;

        /* Set port link params */
        portLinkCfg.macPort = gEnetEcc.macPort;
        portLinkCfg.macCfg = &macCfg;

        /* Enable MAC loopback */
        CpswMacPort_initCfg(&macCfg);
        macCfg.loopbackEn = BTRUE;

        /* No PHY loopback */
        phyCfg->phyAddr = ENETPHY_INVALID_PHYADDR;

        /* All CPSW9G ports in J784S4 are SGMII */
        if (!gEnetEcc.useSGMII)
        {
            /* Regular RGMII for everything else */
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_REDUCED;
            mii->variantType  = ENET_MAC_VARIANT_FORCED;
            macCfg.sgmiiMode  = ENET_MAC_SGMIIMODE_INVALID;
        }
        else
        {
            /* SGMII for J784S4 mcu2_0 */
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_SERIAL;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            macCfg.sgmiiMode  = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK;
        }

        /* Set up link params */
        linkCfg->speed     = ENET_SPEED_1GBIT;
        linkCfg->duplexity = ENET_DUPLEX_FULL;

        /* Enable TX and RX ECC error operations */
        macCfg.txEccErrEn = true;
        macCfg.rxEccErrEn = true;

        /* Submit link config to enet controls */
        EnetBoard_setEnetControl(gEnetEcc.enetType,
                                 gEnetEcc.instId,
                                 gEnetEcc.macPort,
                                 mii);
    }

    /* Open port link */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &portLinkCfg);

        status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, ENET_PER_IOCTL_OPEN_PORT_LINK, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to open port link: %d\n", status);
        }
    }

    return status;
}

void EnetEcc_txIsrFxn(void * appData)
{
    /* tx ISR will set global transmit flag when transmitted*/
    txFlag = 1;
}

void EnetEcc_rxIsrFxn(void * appData)
{
    /* rx ISR will set global receive flag when received*/
    rxFlag = 1;
}

static void EnetEcc_initTxFreePktQ(void)
{
    EnetDma_Pkt *pPktInfo;
    uint32_t frameSize = ENETECC_TEST_PKT_LEN + sizeof(EthFrameHeader);

    /* Initialize a queue to hold frames before transfer */
    EnetQueue_initQ(&gEnetEcc.txFreePktInfoQ);

    /* Initialize a TX EthPkt and queue it to txFreePktInfoQ to hold a packet to be transferred */
    pPktInfo = EnetMem_allocEthPkt(&gEnetEcc,
                                    ENETDMA_CACHELINE_ALIGNMENT,
                                    1U,
                                    &frameSize);
    EnetAppUtils_assert(pPktInfo != NULL);
    ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);
    EnetQueue_enq(&gEnetEcc.txFreePktInfoQ, &pPktInfo->node);

    EnetAppUtils_print("initQs() txFreePktInfoQ initialized with %d pkts\n",
                       EnetQueue_getQCount(&gEnetEcc.txFreePktInfoQ));
}

static void EnetEcc_initRxReadyPktQ(void)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pPktInfo;
    int32_t status;
    uint32_t frameSize = ENETECC_TEST_PKT_LEN + sizeof(EthFrameHeader);

    /* rxReadyQ is used for temporarily holding the unprocessed received packets
     * rxFreeQ is ued for storing the processed received packets
     * to be re-submitted to the DMA handler to be recycled for future use */
    EnetQueue_initQ(&gEnetEcc.rxFreeQ);
    EnetQueue_initQ(&gEnetEcc.rxReadyQ);
    EnetQueue_initQ(&rxReadyQ);

    /* Initialize a RX EthPkt and queue it to rxFreeQ to hold received packets */
    pPktInfo = EnetMem_allocEthPkt(&gEnetEcc,
                                    ENETDMA_CACHELINE_ALIGNMENT,
                                    1U,
                                    &frameSize);
    EnetAppUtils_assert(pPktInfo != NULL);
    ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);
    EnetQueue_enq(&gEnetEcc.rxFreeQ, &pPktInfo->node);

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(gEnetEcc.hRxCh, &rxReadyQ);
    EnetAppUtils_assert(status == ENET_SOK);
    /* There should not be any packet with DMA during init */
    EnetAppUtils_assert(EnetQueue_getQCount(&rxReadyQ) == 0U);

    /* Make sure rxFreeQ is ready to be used */
    EnetAppUtils_validatePacketState(&gEnetEcc.rxFreeQ,
                                     ENET_PKTSTATE_APP_WITH_FREEQ,
                                     ENET_PKTSTATE_APP_WITH_DRIVER);

    /* Submit the queue to the DMA receive handler to be used */
    EnetDma_submitRxPktQ(gEnetEcc.hRxCh,
                         &gEnetEcc.rxFreeQ);

    /* Assert here as during init no. of DMA descriptors should be equal to
     * no. of free Ethernet buffers available with app */

    EnetAppUtils_assert(0U == EnetQueue_getQCount(&gEnetEcc.rxFreeQ));
}

static int32_t EnetEcc_openDma(void)
{
    int32_t status = ENET_SOK;
    EnetUdma_OpenRxFlowPrms rxChCfg;
    EnetUdma_OpenTxChPrms   txChCfg;

    /* Open the CPSW TX channel  */
    if (status == ENET_SOK)
    {
        /* Initialize peripheral config */
        EnetUdma_initTxChParams(&txChCfg);

        /* Assign global handler and transmit callback function from ISR */
        txChCfg.hUdmaDrv = gEnetEcc.hUdmaDrv;
        txChCfg.cbArg    = &gEnetEcc;
        txChCfg.notifyCb = EnetEcc_txIsrFxn;

        /* Set config */
        EnetAppUtils_setCommonTxChPrms(&txChCfg);

        /* Open DMA transmit channel and point to it with global handlers */
        EnetAppUtils_openTxCh(gEnetEcc.hEnet,
                              gEnetEcc.coreKey,
                              gEnetEcc.coreId,
                              &gEnetEcc.txChNum,
                              &gEnetEcc.hTxCh,
                              &txChCfg);

        /* Initialize a queue to contain packets to be transferred later */
        EnetEcc_initTxFreePktQ();

        if (NULL != gEnetEcc.hTxCh)
        {
            /* Tx event init to enable interrupt callback */
            status = EnetDma_enableTxEvent(gEnetEcc.hTxCh);
            if (ENET_SOK != status)
            {
                EnetAppUtils_print("EnetUdma_startTxCh() failed: %d\n", status);
                status = ENET_EFAIL;
            }
        }
        else
        {
            EnetAppUtils_print("EnetUdma_openTxCh() failed to open: %d\n", status);
            status = ENET_EFAIL;
        }
    }

    /* Open the CPSW RX flow  */
    if (status == ENET_SOK)
    {
        /* Initialize peripheral config */
        EnetUdma_initRxFlowParams(&rxChCfg);

        /* Assign global handler and receive callback function from ISR */
        rxChCfg.hUdmaDrv = gEnetEcc.hUdmaDrv;
        rxChCfg.notifyCb = EnetEcc_rxIsrFxn;
        rxChCfg.cbArg   = &gEnetEcc;

        /* Open DMA receive flow channel and point to it with global handlers */
        EnetAppUtils_setCommonRxFlowPrms(&rxChCfg);
        EnetAppUtils_openRxFlow(gEnetEcc.enetType,
                                gEnetEcc.hEnet,
                                gEnetEcc.coreKey,
                                gEnetEcc.coreId,
                                BTRUE,
                                &gEnetEcc.rxStartFlowIdx,
                                &gEnetEcc.rxFlowIdx,
                                &gEnetEcc.hostMacAddr[0U],
                                &gEnetEcc.hRxCh,
                                &rxChCfg);
        if (NULL == gEnetEcc.hRxCh)
        {
            EnetAppUtils_print("EnetUdma_openRxFlow() failed to open: %d\n", status);
            EnetAppUtils_assert(NULL != gEnetEcc.hRxCh);
        }
        else
        {
            /* Destination Address */
            EnetAppUtils_print("Host MAC address: ");
            EnetAppUtils_printMacAddr(gEnetEcc.hostMacAddr);
            /* Submit all RX queues to DMA to hold packets */
            EnetEcc_initRxReadyPktQ();
            /* Rx event init to enable interrupt callback */
            status = EnetDma_enableRxEvent(gEnetEcc.hRxCh);
            if (ENET_SOK != status)
            {
                EnetAppUtils_print("EnetUdma_startRxCh() failed: %d\n", status);
                status = ENET_EFAIL;
            }
        }
    }

    return status;
}

static int32_t EnetEcc_setupCpswAle(void)
{
    Enet_IoctlPrms prms;
    CpswAle_SetPortStateInArgs setPortStateInArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    uint32_t entryIdx;
    int32_t status;

    /* ALE entry with "secure" bit cleared is required for loopback */
    setUcastInArgs.addr.vlanId  = 0U;
    setUcastInArgs.info.portNum = CPSW_ALE_HOST_PORT_NUM;
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure  = BFALSE;
    setUcastInArgs.info.super   = BFALSE;
    setUcastInArgs.info.ageable = BFALSE;
    setUcastInArgs.info.trunk   = BFALSE;
    EnetUtils_copyMacAddr(&setUcastInArgs.addr.addr[0U], gEnetEcc.hostMacAddr);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &entryIdx);

    status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, CPSW_ALE_IOCTL_ADD_UCAST, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to add ucast entry: %d\n", status);
    }

    /* Set host port to 'forwarding' state */
    if (status == ENET_SOK)
    {
        setPortStateInArgs.portNum   = CPSW_ALE_HOST_PORT_NUM;
        setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_FORWARD;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);

        status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to set ALE port state: %d\n", status);
        }
    }

    return status;
}

static void EnetEcc_txSingle(void)
{
    EnetDma_PktQ txSubmitQ, txFreeQ;
    EnetDma_Pkt *pktInfo;
    EthFrame *frame;
    int32_t status = ENET_SOK;
    uint8_t bcastAddr[ENET_MAC_ADDR_LEN] = {0xffU, 0xffU, 0xffU, 0xffU, 0xffU, 0xffU};
    uint32_t i;

    /* Transmit a single packet */
    EnetQueue_initQ(&txSubmitQ);

    /* Dequeue one free TX Eth packet to be used to transfer */
    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetEcc.txFreePktInfoQ);

     /* Fill the TX Eth frame with test content */
    frame = (EthFrame *)pktInfo->sgList.list[0U].bufPtr;
    memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
    memcpy(frame->hdr.srcMac, &gEnetEcc.hostMacAddr[0U], ENET_MAC_ADDR_LEN);
    frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);

    /* Frame will contain incremental data easy to spot */
    for (i = 0U; i < ENETECC_TEST_PKT_LEN; i++)
    {
        memset(&frame->payload[i], (uint8_t)(0x00 + i), 1U);
    }
    pktInfo->sgList.list[0U].segmentFilledLen = sizeof(EthFrameHeader) + ENETECC_TEST_PKT_LEN;

    /* Set up other DMA transfer info */
    pktInfo->appPriv    = &gEnetEcc;
    pktInfo->chkSumInfo = 0U;
    pktInfo->txPortNum  = ENET_MAC_PORT_INV;
    pktInfo->tsInfo.enableHostTxTs = BFALSE;

    /* Print the frame before transfer */
    EnetAppUtils_print("\ntx frame:\n");
    EnetAppUtils_printSGFrame(pktInfo);

    /* Enqueue the packet for later transmission */
    EnetQueue_enq(&txSubmitQ, &pktInfo->node);

    /* Submit the queue that contains the packet to the DMA transmit handler*/
    status = EnetDma_submitTxPktQ(gEnetEcc.hTxCh, &txSubmitQ);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("submitTxPktQ() failed to submit pkts: %d\n", status);
    }
    else
    {
        /* Wait for transmit to complete and set the txFlag */
        while(!txFlag);
        /* Transmit completed, reset transmit flag */
        txFlag = 0;

        /* Init a temporary queue to hold the packet that completed trasmitting*/
        EnetQueue_initQ(&txFreeQ);

        /* Retrieve any CPSW packets that may be free now */
        status = EnetDma_retrieveTxPktQ(gEnetEcc.hTxCh, &txFreeQ);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("retrieveTxPktQ() failed to retrieve pkts: %d\n", status);
        }
        else
        {
            /* This should be the packet that was transmitted */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
            /* Put this packet back into the global queue to be reused */
            EnetQueue_enq(&gEnetEcc.txFreePktInfoQ, &pktInfo->node);
        }
    }
}

static void EnetEcc_rxSingle(void)
{
    EnetDma_Pkt *pktInfo;
    int32_t status;

    /* Wait for receive to complete and set the rxFlag */
    while(!rxFlag);
    /* Receive completed, reset receive flag */
    rxFlag = 0;

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(gEnetEcc.hRxCh, &gEnetEcc.rxReadyQ);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("retrieveRxPktQ() failed to retrieve pkts: %d\n", status);
    }
    else
    {
        /* Get the packet from the temporary holding queue */
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetEcc.rxReadyQ);

        /* Process the packet by printing the packet frame */
        EnetAppUtils_print("\nrx frame:\n");
        EnetAppUtils_printSGFrame(pktInfo);

        /* Put the packet into rxFreeQ after processing */
        EnetQueue_enq(&gEnetEcc.rxFreeQ, &pktInfo->node);

        /* Submit the rxFreeQ with the processed packet to be recycled */
        status = EnetDma_submitRxPktQ(gEnetEcc.hRxCh, &gEnetEcc.rxFreeQ);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("submitRxPktQ() failed to submit pkts: %d\n", status);
        }
    }
}

static void EnetEcc_showCpswStats(void)
{
    Enet_IoctlPrms prms;
    CpswStats_PortStats portStats;
    int32_t status;

    /* Show host port statistics */
    ENET_IOCTL_SET_OUT_ARGS(&prms, &portStats);
    status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, ENET_STATS_IOCTL_GET_HOSTPORT_STATS, &prms);
    if (status == ENET_SOK)
    {
        EnetAppUtils_print("\n Port 0 Statistics\n");
        EnetAppUtils_print("-----------------------------------------\n");
        EnetAppUtils_printHostPortStats2G((CpswStats_HostPort_2g *)&portStats);
        EnetAppUtils_print("\n");
    }
    else
    {
        EnetAppUtils_print("Failed to get host stats: %d\n", status);
    }

    /* Show MAC port statistics */
    if ((status == ENET_SOK) &&
        (gEnetEcc.macPort != ENET_MAC_PORT_INV))
    {
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &gEnetEcc.macPort, &portStats);
        status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, ENET_STATS_IOCTL_GET_MACPORT_STATS, &prms);
        if (status == ENET_SOK)
        {
            EnetAppUtils_print("\n Port 1 Statistics\n");
            EnetAppUtils_print("-----------------------------------------\n");
            EnetAppUtils_printMacPortStats2G((CpswStats_MacPort_2g *)&portStats);
            EnetAppUtils_print("\n");
        }
        else
        {
            EnetAppUtils_print("Failed to get MAC stats: %d\n", status);
        }
    }
}

static void EnetEcc_closeDma(void)
{
    EnetDma_PktQ fqPktInfoQ;
    EnetDma_PktQ cqPktInfoQ;

    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    /* Close RX channel */
    EnetAppUtils_closeRxFlow(gEnetEcc.enetType,
                             gEnetEcc.hEnet,
                             gEnetEcc.coreKey,
                             gEnetEcc.coreId,
                             BTRUE,
                             &fqPktInfoQ,
                             &cqPktInfoQ,
                             gEnetEcc.rxStartFlowIdx,
                             gEnetEcc.rxFlowIdx,
                             gEnetEcc.hostMacAddr,
                             gEnetEcc.hRxCh);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    /* Close TX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    EnetAppUtils_closeTxCh(gEnetEcc.hEnet,
                           gEnetEcc.coreKey,
                           gEnetEcc.coreId,
                           &fqPktInfoQ,
                           &cqPktInfoQ,
                           gEnetEcc.hTxCh,
                           gEnetEcc.txChNum);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    EnetAppUtils_freePktInfoQ(&gEnetEcc.rxFreeQ);
    EnetAppUtils_freePktInfoQ(&gEnetEcc.txFreePktInfoQ);

    EnetMem_deInit();
}

static void EnetEcc_closeEnet(void)
{
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;

    /* Close port link */
    if (gEnetEcc.macPort != ENET_MAC_PORT_INV)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetEcc.macPort);

        status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to close port link: %d\n", status);
        }
    }

    /* Detach core */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetEcc.coreKey);

        status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, ENET_PER_IOCTL_DETACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to detach core key %u: %d\n", gEnetEcc.coreKey, status);
        }
    }

    /* Close Enet driver */
    Enet_close(gEnetEcc.hEnet);

    /* Close UDMA */
    EnetAppUtils_udmaclose(gEnetEcc.hUdmaDrv);

    gEnetEcc.hEnet = NULL;
}

static int32_t EnetEcc_waitForLinkUp(void)
{
    Enet_IoctlPrms prms;
    bool linked = BFALSE;
    int32_t status = ENET_SOK;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &gEnetEcc.macPort, &linked);

    while (!linked)
    {
        status = Enet_ioctl(gEnetEcc.hEnet, gEnetEcc.coreId, ENET_PER_IOCTL_IS_PORT_LINK_UP, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to get port %u's link status: %d\n",
                            ENET_MACPORT_ID(gEnetEcc.macPort), status);
            linked = BFALSE;
            break;
        }

        if (!linked)
        {
            EnetUtils_delay(100U);
            Enet_periodicTick(gEnetEcc.hEnet);
        }
    }

    return status;
}

uint32_t popcount32(uint32_t v)
{
    uint32_t c;

    c =             ((v & 0xfff) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
    c += (((v & 0xfff000) >> 12) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
    c +=              ((v >> 24) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;

    return c;
}

int32_t runECC2Bit_RAMInjectTest(void)
{
    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
    SDL_ECC_InjectErrorType errorType;
    SDL_ErrType_t result;
    uint32_t subType;
    int32_t status = 0;

    uint32_t injectEccBitMask = 1 << 5;

    memset(&injectErrorConfig, 0, sizeof(injectErrorConfig));

    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem     = (uint32_t *)(0x0u);
    injectErrorConfig.flipBitMask = injectEccBitMask;
    injectErrorConfig.chkGrp      = 0x0;

#if defined(SOC_J721E) || defined(SOC_J7200)
    subType = SDL_MCU_CPSW0_P1_RX_FIFO_RAM_ID;
#else
    subType = SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_CPSW_2GU_CORE_ECC_ECC_CTRL3_RAM_ID;
#endif
    errorType = (popcount32(injectErrorConfig.flipBitMask) > 1) ?
                   SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE :
                   SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE;

    EnetAppUtils_print("MCU CPSW %d-bit error inject test: Subtype %d: flipBitMask 0x%08x\n",
                       (errorType == SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE) ? 2 : 1,
                       subType,
                       injectErrorConfig.flipBitMask);

    result = SDL_ECC_injectError(SDL_ECC_MEMTYPE,
                                 subType,
                                 errorType,
                                 &injectErrorConfig);
    if (result != SDL_PASS)
    {
        EnetAppUtils_print("Failed inject test\n");
        EnetAppUtils_assert(false);
        status = -1;
    }

    return status;
}

int32_t clear_cpsw_ecc_injection(void)
{
    Enet_IoctlPrms macPrms;
    EnetMacPort_SetEccTestModeInArgs setMacInArgs;
    EnetHostPort_SetEccTestModeInArgs eccTestMode;
    uint32_t coreId = EnetSoc_getCoreId();
    /* Clear any pending error injection */
    uint32_t regValue = 0x0;
    int32_t status;
    SDL_ecc_aggrRegs *eccAggrRegs = (SDL_ecc_aggrRegs *)SDL_MCU_CPSW0_ECC_BASE; // base address of ECC aggregator

#if defined(SOC_J721E) || defined(SOC_J7200)
    uint32_t ramId = SDL_MCU_CPSW0_P1_RX_FIFO_RAM_ID;// the RAM ID being tested
#else
    uint32_t ramId = SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_CPSW_2GU_CORE_ECC_ECC_CTRL3_RAM_ID;
#endif

    EnetAppUtils_print("Clearing ECC error injection\n");

    /* Read the current register contents */
    SDL_ecc_aggrReadEccRamCtrlReg(eccAggrRegs,
                                  ramId,
                                  (uint32_t *)((uint32_t)&(regValue)));

    /* Clear ERROR_ONCE, FORCE_N_ROW, FORCE_DED, and FORCE_SEC */
    regValue &= ((uint32_t)(~(CSL_ECC_RAM_CTRL_ERROR_ONCE_MASK | CSL_ECC_RAM_CTRL_FORCE_N_ROW_MASK | CSL_ECC_RAM_CTRL_FORCE_DED_MASK | CSL_ECC_RAM_CTRL_FORCE_SEC_MASK)));

    /* Write back the updated value */
    SDL_ecc_aggrWriteEccRamCtrlReg(eccAggrRegs, ramId, regValue);

    /* Even if the error_once bit is set and force_sec goes low on the ecc ctrl, the errors will continue to be injected since these do not control the actual FIFO ECC.
     * Hence to disable the error injection you must write a 0 to the “pn_rx_ecc_err_en” register bit */
    eccTestMode.txEccErrEn = BFALSE;
    eccTestMode.rxEccErrEn = BFALSE;
    setMacInArgs.eccTestMode = &eccTestMode;
    setMacInArgs.macPort    = ENET_MAC_PORT_1;
    ENET_IOCTL_SET_IN_ARGS(&macPrms, &setMacInArgs);
    status = Enet_ioctl(gEnetEcc.hEnet, coreId, ENET_MACPORT_IOCTL_SET_ECC_TEST_MODE,
                        &macPrms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("EnetTest_openCpsw() failed ENET_MACPORT_IOCTL_SET_ECC_TEST_MODE: %d\n",
                            status);
    }
    return status;
}

int32_t SDL_ESM_applicationCallbackFunction(SDL_ESM_Inst esmInst,
                                            SDL_ESM_IntType esmIntrType,
                                            uint32_t grpChannel,
                                            uint32_t index,
                                            uint32_t intSrc,
                                            void *arg)
{
    SDL_ECC_MemType eccmemtype;
    SDL_Ecc_AggrIntrSrc eccIntrSrc;
    SDL_ECC_ErrorInfo_t eccErrorInfo;
    int32_t status;

    eccErrCount++;
    EnetAppUtils_print("instType 0x%x, intType 0x%x, grpChannel 0x%x, index 0x%x, intSrc 0x%x\n",
                       esmInst, esmIntrType, grpChannel, index, intSrc);
    EnetAppUtils_print("%s Priority Interrupt Executed\n", (esmIntrType == 1u) ? "High" : "Low");

    status = SDL_ECC_getESMErrorInfo(esmInst, intSrc, &eccmemtype, &eccIntrSrc);

    /* Any additional customer specific actions can be added here */
    status = SDL_ECC_getErrorInfo(eccmemtype, eccIntrSrc, &eccErrorInfo);

    EnetAppUtils_print("eccMemType %d, errorSrc 0x%x, ramId %d, bitErrorOffset 0x%04x%04x, bitErrorGroup %d\n",
                       eccmemtype, eccIntrSrc, eccErrorInfo.memSubType, (uint32_t)(eccErrorInfo.bitErrorOffset >> 32),
                       (uint32_t)(eccErrorInfo.bitErrorOffset & 0x00000000FFFFFFFF), eccErrorInfo.bitErrorGroup);

    if (eccErrorInfo.injectBitErrCnt != 0)
    {
        SDL_ECC_clearNIntrPending(eccmemtype,
                                  eccErrorInfo.memSubType,
                                  eccIntrSrc,
                                  SDL_ECC_AGGR_ERROR_SUBTYPE_INJECT,
                                  eccErrorInfo.injectBitErrCnt);
    }
    else
    {
        SDL_ECC_clearNIntrPending(eccmemtype,
                                  eccErrorInfo.memSubType,
                                  eccIntrSrc,
                                  SDL_ECC_AGGR_ERROR_SUBTYPE_NORMAL,
                                  eccErrorInfo.bitErrCnt);
    }

    status = SDL_ECC_ackIntr(eccmemtype, eccIntrSrc);

    status = clear_cpsw_ecc_injection();

    return status;
}

int32_t ECC_Example_init(void)
{
    void *ptr = (void *)&arg;
    SDL_ErrType_t result;
    int32_t status = 0;

    /* Initialize MAIN ESM module */
    if (status == 0)
    {
        result = SDL_ESM_init(SDL_ESM_INST_MCU_ESM0,
                              &ECC_Test_esmInitConfig_MCU,
                              &SDL_ESM_applicationCallbackFunction, ptr);
        if (result != SDL_PASS)
        {
            EnetAppUtils_print("ECC_Example_init: Error initializing MAIN ESM: result = %d\n", result);
            status = -1;
        }
    }

    /* Initialize ECC */
    if (status == 0)
    {
#if defined(SOC_J721E) || defined(SOC_J7200)
        result = SDL_ECC_init(SDL_ECC_MEMTYPE,
                              &ECC_Test_SDL_MCU_CPSW0_0_initConfig);
#else
        result = SDL_ECC_init(SDL_ECC_MEMTYPE,
                              &ECC_Test_SDL_MCU_CPSW0_CPSW_2GUSS_CORE_ECC_CPSW_ECC_AGGR_initConfig);
#endif
        if (result != SDL_PASS)
        {
            EnetAppUtils_print("ECC_Example_init: Error initializing R5F core ECC: result = %d\n", result);
            status = -1;
        }
    }

    /* Initialize ECC callbacks within the Main ESM */
    if (status == 0)
    {
        result = SDL_ECC_initEsm(SDL_ESM_INST_MCU_ESM0);
        if (result != SDL_PASS)
        {
            EnetAppUtils_print("ECC_Example_init: Error initializing ECC callback for Main ESM: result = %d\n", result);
            status = -1;
        }
    }

    return status;
}

int32_t cpsw_sdl_test(void)
{
    SDL_ErrType_t result;
    int32_t status = 0;

    result = SDL_TEST_osalInit();
    if (result != SDL_PASS)
    {
        EnetAppUtils_print("Error: Init Failed\n");
        status = -1;
    }

    if (status == 0)
    {
        status = ECC_Example_init();
        if (status != 0)
        {
            EnetAppUtils_print("ECC Safety Example tests: unsuccessful\n");
        }
    }

    if (status == 0)
    {
        status = runECC2Bit_RAMInjectTest();
    }

    return status;
}

/* SDL_ECC_applicationCallbackFunction is expected to be defined by the application. It is
 * required by the SDL ECC module. It is called by the SDL ECC module to notify the
 * application of certain ECC errors that are reported as Exception events.
 * Note, however, that it is not executed in this example */
void SDL_ECC_applicationCallbackFunction(SDL_ECC_MemType eccMemType,
                                         uint32_t errorSrc,
                                         uint32_t address,
                                         uint32_t ramId,
                                         uint64_t bitErrorOffset,
                                         uint32_t bitErrorGroup)
{
    EnetAppUtils_print("======================================================================\n");
    EnetAppUtils_print("ECC Error callback function called : eccMemType %d, errorSrc 0x%x, address 0x%x, ramId %d, bitErrorOffset 0x%04x%04x, bitErrorGroup %d\n",
                       eccMemType, errorSrc, address, ramId, (uint32_t)(bitErrorOffset >> 32),
                       (uint32_t)(bitErrorOffset & 0x00000000FFFFFFFF), bitErrorGroup);

    /* Any additional customer specific actions can be added here */
}