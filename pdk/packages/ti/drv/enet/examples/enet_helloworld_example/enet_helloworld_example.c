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

/*!
 * \file  enet_helloworld_example.c
 *
 * \brief This file contains the implementation of the Enet hello world example.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */


#include <stdint.h>
#include <string.h>
#include <assert.h>

#if defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_serdes_cfg.h>
#endif

#if defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_serdes_cfg.h>
#endif

#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* length of frame payload used to calculate frame size needed */
#define ENETHELLOWORLD_TEST_PKT_LEN                      (50U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct EnetHelloWorld_Obj_s
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
} EnetHelloWorld_Obj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void EnetHelloWorld_getEnetTypeIdMode(Enet_Type *enetType,
                                             uint32_t *instId,
                                             uint8_t *useSGMII);

static void EnetHelloWorld_initCpswCfg(Cpsw_Cfg *cpswCfg);

static int32_t EnetHelloWorld_setupCpswAle(void);

static int32_t EnetHelloWorld_openEnet(void);

static int32_t EnetHelloWorld_openDma(void);

static void EnetHelloWorld_initTxFreePktQ(void);

static void EnetHelloWorld_initTxFreePktQ(void);

static int32_t EnetHelloWorld_setupCpswAle(void);

static void EnetHelloWorld_txSingle(void);

static void EnetHelloWorld_rxSingle(void);

static void EnetHelloWorld_showCpswStats(void);

static void EnetHelloWorld_closeDma(void);

static void EnetHelloWorld_closeEnet(void);

static int32_t EnetHelloWorld_waitForLinkUp(void);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Enet test object */
EnetHelloWorld_Obj gEnetHelloWorld;

/* Global transfer and receive interrupt flags */
volatile uint8_t txFlag;
volatile uint8_t rxFlag;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{ 
    EnetUtils_Cfg utilsCfg;
    Enet_IoctlPrms prms;
    int32_t status;

    /* Initialize interrupt flags */
    txFlag = 0U;
    rxFlag = 0U;

    /* Initialize global test config object */
    memset(&gEnetHelloWorld, 0U, sizeof(gEnetHelloWorld));

    /* Basic common configs for simple MAC loopback test */
    gEnetHelloWorld.boardId = ENETBOARD_LOOPBACK_ID;
    gEnetHelloWorld.expPort = ENETBOARD_EXP_PORT_NONE;
    gEnetHelloWorld.macPort = ENET_MAC_PORT_1;

    /* Different configs for different boards and cores */
    EnetHelloWorld_getEnetTypeIdMode(&gEnetHelloWorld.enetType,
                                     &gEnetHelloWorld.instId,
                                     &gEnetHelloWorld.useSGMII);

    /* Initialize the board */
    status = EnetBoard_init();
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to init enet board: %d\n", status);
    }
    EnetAppUtils_print("\nEnet Hello World Test Start\n");

    /* Start the clock */
    EnetAppUtils_enableClocks(gEnetHelloWorld.enetType, gEnetHelloWorld.instId);

    /* J784S4 mcu2_0 with SGMII requires SERDES */
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    if (gEnetHelloWorld.useSGMII)
    {
        Board_serdesCfgSgmii();
    }
#endif

    /* Local core id */
    gEnetHelloWorld.coreId = EnetSoc_getCoreId();

    /* Initialize Enet driver (use default utils) */
    Enet_initUtilsCfg(&utilsCfg);
    Enet_init(&utilsCfg);

    /* Open Enet driver */
    status = EnetHelloWorld_openEnet();
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to open Enet driver: %d\n", status);
    }

    /* Attach the core with RM */
    if (status == ENET_SOK)
    {
        uint32_t coreId;
        EnetPer_AttachCoreOutArgs attachCoreOutArgs;
        coreId = gEnetHelloWorld.coreId;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &coreId, &attachCoreOutArgs);
        status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, ENET_PER_IOCTL_ATTACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed ENET_PER_IOCTL_ATTACH_CORE: %d\n", status);
        }
        else
        {
            gEnetHelloWorld.coreKey = attachCoreOutArgs.coreKey;
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

        status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, CPSW_ALE_IOCTL_ADD_MCAST, &prms);
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
        status = EnetHelloWorld_openDma();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to open DMA: %d\n", status);
        }
    }

    /* Enable host port */
    if (status == ENET_SOK)
    {
        status = EnetHelloWorld_setupCpswAle();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to setup CPSW ALE: %d\n", status);
        }

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_NO_ARGS(&prms);
            status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, ENET_HOSTPORT_IOCTL_ENABLE, &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Failed to enable host port: %d\n", status);
            }
        }
    }

    /* Wait for link up */
    if (status == ENET_SOK)
    {
        status = EnetHelloWorld_waitForLinkUp();
    }

    /* Do packet transmission and reception */
    if (status == ENET_SOK)
    {
        EnetHelloWorld_txSingle();
        EnetHelloWorld_rxSingle();
        EnetHelloWorld_showCpswStats();
    }

    /* Disable host port */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, ENET_HOSTPORT_IOCTL_DISABLE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to disable host port: %d\n", status);
        }
    }

    /* Close Enet DMA driver */
    EnetHelloWorld_closeDma();

    /* Close Enet driver */
    EnetHelloWorld_closeEnet();

    /* Disable peripheral clocks */
    EnetAppUtils_disableClocks(gEnetHelloWorld.enetType, gEnetHelloWorld.instId);

    /* Deinit Enet driver */
    Enet_deinit();

    EnetAppUtils_print("All tests have passed\n");

    return 0;
}

static void EnetHelloWorld_getEnetTypeIdMode(Enet_Type *enetType,
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

static void EnetHelloWorld_initCpswCfg(Cpsw_Cfg *cpswCfg)
{
    CpswHostPort_Cfg *hostPortCfg = &cpswCfg->hostPortCfg;
    CpswCpts_Cfg *cptsCfg = &cpswCfg->cptsCfg;

    /* Set initial config */
    Enet_initCfg(gEnetHelloWorld.enetType, gEnetHelloWorld.instId, cpswCfg, sizeof(*cpswCfg));

    /* Host port config */
    hostPortCfg->removeCrc = BTRUE;

    /* CPTS config */
    /* Note: Timestamping and MAC loopback are not supported together because of
     * IP limitation, so disabling timestamping for this application */
    cptsCfg->hostRxTsEn = BFALSE;

    EnetAppUtils_initResourceConfig(gEnetHelloWorld.enetType, gEnetHelloWorld.instId, gEnetHelloWorld.coreId, &cpswCfg->resCfg);
}

static int32_t EnetHelloWorld_openEnet(void)
{
    Cpsw_Cfg cpswCfg;
    EnetDma_Cfg dmaCfg;
    Enet_IoctlPrms prms;
    EnetPer_PortLinkCfg portLinkCfg;
    CpswMacPort_Cfg macCfg;
    int32_t status = ENET_SOK;

    cpswCfg.dmaCfg = &dmaCfg;

    /* Initialize peripheral config */
    EnetHelloWorld_initCpswCfg(&cpswCfg);
    EnetAppUtils_print("CPSW Test\n");

    /* Set DMA receive channel priority as default */
    dmaCfg.rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;

    /* App should open UDMA first as UDMA handle is needed to initialize CPSW RX channel */
    gEnetHelloWorld.hUdmaDrv = EnetAppUtils_udmaOpen(gEnetHelloWorld.enetType, NULL);
    EnetAppUtils_assert(NULL != gEnetHelloWorld.hUdmaDrv);

    /* Global DMA driver handle */
    dmaCfg.hUdmaDrv = gEnetHelloWorld.hUdmaDrv;

    /* Set Enet global runtime log level */
    Enet_setTraceLevel(ENET_TRACE_DEBUG);

    /* Open the Enet driver */
    gEnetHelloWorld.hEnet = Enet_open(gEnetHelloWorld.enetType,
                                      gEnetHelloWorld.instId,
                                      &cpswCfg,
                                      sizeof(cpswCfg));
    if (gEnetHelloWorld.hEnet == NULL)
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
        portLinkCfg.macPort = gEnetHelloWorld.macPort;
        portLinkCfg.macCfg = &macCfg;

        /* Enable MAC loopback */
        CpswMacPort_initCfg(&macCfg);
        macCfg.loopbackEn = BTRUE;

        /* No PHY loopback */
        phyCfg->phyAddr = ENETPHY_INVALID_PHYADDR;

        /* All CPSW9G ports in J784S4 are SGMII */
        if (!gEnetHelloWorld.useSGMII)
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

        /* Submit link config to enet controls */
        EnetBoard_setEnetControl(gEnetHelloWorld.enetType,
                                 gEnetHelloWorld.instId,
                                 gEnetHelloWorld.macPort,
                                 mii);
    }

    /* Open port link */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &portLinkCfg);

        status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, ENET_PER_IOCTL_OPEN_PORT_LINK, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to open port link: %d\n", status);
        }
    }

    return status;
}

void EnetHelloWorld_txIsrFxn(void * appData)
{
    /* tx ISR will set global transmit flag when transmitted*/
    txFlag = 1;
}

void EnetHelloWorld_rxIsrFxn(void * appData)
{
    /* rx ISR will set global receive flag when received*/
    rxFlag = 1;
}

static void EnetHelloWorld_initTxFreePktQ(void)
{
    EnetDma_Pkt *pPktInfo;
    uint32_t frameSize = ENETHELLOWORLD_TEST_PKT_LEN + sizeof(EthFrameHeader);

    /* Initialize a queue to hold frames before transfer */
    EnetQueue_initQ(&gEnetHelloWorld.txFreePktInfoQ);

    /* Initialize a TX EthPkt and queue it to txFreePktInfoQ to hold a packet to be transferred */
    pPktInfo = EnetMem_allocEthPkt(&gEnetHelloWorld,
                                    ENETDMA_CACHELINE_ALIGNMENT,
                                    1U,
                                    &frameSize);
    EnetAppUtils_assert(pPktInfo != NULL);
    ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);
    EnetQueue_enq(&gEnetHelloWorld.txFreePktInfoQ, &pPktInfo->node);

    EnetAppUtils_print("initQs() txFreePktInfoQ initialized with %d pkts\n",
                       EnetQueue_getQCount(&gEnetHelloWorld.txFreePktInfoQ));
}

static void EnetHelloWorld_initRxReadyPktQ(void)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pPktInfo;
    int32_t status;
    uint32_t frameSize = ENETHELLOWORLD_TEST_PKT_LEN + sizeof(EthFrameHeader);

    /* rxReadyQ is used for temporarily holding the unprocessed received packets
     * rxFreeQ is ued for storing the processed received packets
     * to be re-submitted to the DMA handler to be recycled for future use */
    EnetQueue_initQ(&gEnetHelloWorld.rxFreeQ);
    EnetQueue_initQ(&gEnetHelloWorld.rxReadyQ);
    EnetQueue_initQ(&rxReadyQ);

    /* Initialize a RX EthPkt and queue it to rxFreeQ to hold received packets */
    pPktInfo = EnetMem_allocEthPkt(&gEnetHelloWorld,
                                    ENETDMA_CACHELINE_ALIGNMENT,
                                    1U,
                                    &frameSize);
    EnetAppUtils_assert(pPktInfo != NULL);
    ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);
    EnetQueue_enq(&gEnetHelloWorld.rxFreeQ, &pPktInfo->node);

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(gEnetHelloWorld.hRxCh, &rxReadyQ);
    EnetAppUtils_assert(status == ENET_SOK);
    /* There should not be any packet with DMA during init */
    EnetAppUtils_assert(EnetQueue_getQCount(&rxReadyQ) == 0U);

    /* Make sure rxFreeQ is ready to be used */
    EnetAppUtils_validatePacketState(&gEnetHelloWorld.rxFreeQ,
                                     ENET_PKTSTATE_APP_WITH_FREEQ,
                                     ENET_PKTSTATE_APP_WITH_DRIVER);

    /* Submit the queue to the DMA receive handler to be used */
    EnetDma_submitRxPktQ(gEnetHelloWorld.hRxCh,
                         &gEnetHelloWorld.rxFreeQ);

    /* Assert here as during init no. of DMA descriptors should be equal to
     * no. of free Ethernet buffers available with app */

    EnetAppUtils_assert(0U == EnetQueue_getQCount(&gEnetHelloWorld.rxFreeQ));
}

static int32_t EnetHelloWorld_openDma(void)
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
        txChCfg.hUdmaDrv = gEnetHelloWorld.hUdmaDrv;
        txChCfg.cbArg    = &gEnetHelloWorld;
        txChCfg.notifyCb = EnetHelloWorld_txIsrFxn;

        /* Set config */
        EnetAppUtils_setCommonTxChPrms(&txChCfg);

        /* Open DMA transmit channel and point to it with global handlers */
        EnetAppUtils_openTxCh(gEnetHelloWorld.hEnet,
                              gEnetHelloWorld.coreKey,
                              gEnetHelloWorld.coreId,
                              &gEnetHelloWorld.txChNum,
                              &gEnetHelloWorld.hTxCh,
                              &txChCfg);

        /* Initialize a queue to contain packets to be transferred later */
        EnetHelloWorld_initTxFreePktQ();

        if (NULL != gEnetHelloWorld.hTxCh)
        {
            /* Tx event init to enable interrupt callback */
            status = EnetDma_enableTxEvent(gEnetHelloWorld.hTxCh);
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
        rxChCfg.hUdmaDrv = gEnetHelloWorld.hUdmaDrv;
        rxChCfg.notifyCb = EnetHelloWorld_rxIsrFxn;
        rxChCfg.cbArg   = &gEnetHelloWorld;

        /* Open DMA receive flow channel and point to it with global handlers */
        EnetAppUtils_setCommonRxFlowPrms(&rxChCfg);
        EnetAppUtils_openRxFlow(gEnetHelloWorld.enetType,
                                gEnetHelloWorld.hEnet,
                                gEnetHelloWorld.coreKey,
                                gEnetHelloWorld.coreId,
                                BTRUE,
                                &gEnetHelloWorld.rxStartFlowIdx,
                                &gEnetHelloWorld.rxFlowIdx,
                                &gEnetHelloWorld.hostMacAddr[0U],
                                &gEnetHelloWorld.hRxCh,
                                &rxChCfg);
        if (NULL == gEnetHelloWorld.hRxCh)
        {
            EnetAppUtils_print("EnetUdma_openRxFlow() failed to open: %d\n", status);
            EnetAppUtils_assert(NULL != gEnetHelloWorld.hRxCh);
        }
        else
        {
            /* Destination Address */
            EnetAppUtils_print("Host MAC address: ");
            EnetAppUtils_printMacAddr(gEnetHelloWorld.hostMacAddr);
            /* Submit all RX queues to DMA to hold packets */
            EnetHelloWorld_initRxReadyPktQ();
            /* Rx event init to enable interrupt callback */
            status = EnetDma_enableRxEvent(gEnetHelloWorld.hRxCh);
            if (ENET_SOK != status)
            {
                EnetAppUtils_print("EnetUdma_startRxCh() failed: %d\n", status);
                status = ENET_EFAIL;
            }
        }
    }

    return status;
}

static int32_t EnetHelloWorld_setupCpswAle(void)
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
    EnetUtils_copyMacAddr(&setUcastInArgs.addr.addr[0U], gEnetHelloWorld.hostMacAddr);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &entryIdx);

    status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, CPSW_ALE_IOCTL_ADD_UCAST, &prms);
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

        status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to set ALE port state: %d\n", status);
        }
    }

    return status;
}

static void EnetHelloWorld_txSingle(void)
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
    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetHelloWorld.txFreePktInfoQ);

     /* Fill the TX Eth frame with test content */
    frame = (EthFrame *)pktInfo->sgList.list[0U].bufPtr;
    memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
    memcpy(frame->hdr.srcMac, &gEnetHelloWorld.hostMacAddr[0U], ENET_MAC_ADDR_LEN);
    frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);

    /* Frame will contain incremental data easy to spot */
    for (i = 0U; i < ENETHELLOWORLD_TEST_PKT_LEN; i++)
    {
        memset(&frame->payload[i], (uint8_t)(0x00 + i), 1U);
    }
    pktInfo->sgList.list[0U].segmentFilledLen = sizeof(EthFrameHeader) + ENETHELLOWORLD_TEST_PKT_LEN;

    /* Set up other DMA transfer info */
    pktInfo->appPriv    = &gEnetHelloWorld;
    pktInfo->chkSumInfo = 0U;
    pktInfo->txPortNum  = ENET_MAC_PORT_INV;
    pktInfo->tsInfo.enableHostTxTs = BFALSE;

    /* Print the frame before transfer */
    EnetAppUtils_print("\ntx frame:\n");
    EnetAppUtils_printSGFrame(pktInfo);

    /* Enqueue the packet for later transmission */
    EnetQueue_enq(&txSubmitQ, &pktInfo->node);

    /* Submit the queue that contains the packet to the DMA transmit handler*/
    status = EnetDma_submitTxPktQ(gEnetHelloWorld.hTxCh, &txSubmitQ);
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
        status = EnetDma_retrieveTxPktQ(gEnetHelloWorld.hTxCh, &txFreeQ);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("retrieveTxPktQ() failed to retrieve pkts: %d\n", status);
        }
        else
        {
            /* This should be the packet that was transmitted */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
            /* Put this packet back into the global queue to be reused */
            EnetQueue_enq(&gEnetHelloWorld.txFreePktInfoQ, &pktInfo->node);
        }
    }
}

static void EnetHelloWorld_rxSingle(void)
{
    EnetDma_Pkt *pktInfo;
    int32_t status;

    /* Wait for receive to complete and set the rxFlag */
    while(!rxFlag);
    /* Receive completed, reset receive flag */
    rxFlag = 0;

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(gEnetHelloWorld.hRxCh, &gEnetHelloWorld.rxReadyQ);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("retrieveRxPktQ() failed to retrieve pkts: %d\n", status);
    }
    else
    {
        /* Get the packet from the temporary holding queue */
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetHelloWorld.rxReadyQ);

        /* Process the packet by printing the packet frame */
        EnetAppUtils_print("\nrx frame:\n");
        EnetAppUtils_printSGFrame(pktInfo);

        /* Put the packet into rxFreeQ after processing */
        EnetQueue_enq(&gEnetHelloWorld.rxFreeQ, &pktInfo->node);

        /* Submit the rxFreeQ with the processed packet to be recycled */
        status = EnetDma_submitRxPktQ(gEnetHelloWorld.hRxCh, &gEnetHelloWorld.rxFreeQ);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("submitRxPktQ() failed to submit pkts: %d\n", status);
        }
    }
}

static void EnetHelloWorld_showCpswStats(void)
{
    Enet_IoctlPrms prms;
    CpswStats_PortStats portStats;
    int32_t status;

    /* Show host port statistics */
    ENET_IOCTL_SET_OUT_ARGS(&prms, &portStats);
    status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, ENET_STATS_IOCTL_GET_HOSTPORT_STATS, &prms);
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
        (gEnetHelloWorld.macPort != ENET_MAC_PORT_INV))
    {
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &gEnetHelloWorld.macPort, &portStats);
        status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, ENET_STATS_IOCTL_GET_MACPORT_STATS, &prms);
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

static void EnetHelloWorld_closeDma(void)
{
    EnetDma_PktQ fqPktInfoQ;
    EnetDma_PktQ cqPktInfoQ;

    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    /* Close RX channel */
    EnetAppUtils_closeRxFlow(gEnetHelloWorld.enetType,
                             gEnetHelloWorld.hEnet,
                             gEnetHelloWorld.coreKey,
                             gEnetHelloWorld.coreId,
                             BTRUE,
                             &fqPktInfoQ,
                             &cqPktInfoQ,
                             gEnetHelloWorld.rxStartFlowIdx,
                             gEnetHelloWorld.rxFlowIdx,
                             gEnetHelloWorld.hostMacAddr,
                             gEnetHelloWorld.hRxCh);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    /* Close TX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    EnetAppUtils_closeTxCh(gEnetHelloWorld.hEnet,
                           gEnetHelloWorld.coreKey,
                           gEnetHelloWorld.coreId,
                           &fqPktInfoQ,
                           &cqPktInfoQ,
                           gEnetHelloWorld.hTxCh,
                           gEnetHelloWorld.txChNum);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    EnetAppUtils_freePktInfoQ(&gEnetHelloWorld.rxFreeQ);
    EnetAppUtils_freePktInfoQ(&gEnetHelloWorld.txFreePktInfoQ);

    EnetMem_deInit();
}

static void EnetHelloWorld_closeEnet(void)
{
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;

    /* Close port link */
    if (gEnetHelloWorld.macPort != ENET_MAC_PORT_INV)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetHelloWorld.macPort);

        status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to close port link: %d\n", status);
        }
    }

    /* Detach core */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetHelloWorld.coreKey);

        status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, ENET_PER_IOCTL_DETACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to detach core key %u: %d\n", gEnetHelloWorld.coreKey, status);
        }
    }

    /* Close Enet driver */
    Enet_close(gEnetHelloWorld.hEnet);

    /* Close UDMA */
    EnetAppUtils_udmaclose(gEnetHelloWorld.hUdmaDrv);

    gEnetHelloWorld.hEnet = NULL;
}

static int32_t EnetHelloWorld_waitForLinkUp(void)
{
    Enet_IoctlPrms prms;
    bool linked = BFALSE;
    int32_t status = ENET_SOK;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &gEnetHelloWorld.macPort, &linked);

    while (!linked)
    {
        status = Enet_ioctl(gEnetHelloWorld.hEnet, gEnetHelloWorld.coreId, ENET_PER_IOCTL_IS_PORT_LINK_UP, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to get port %u's link status: %d\n",
                            ENET_MACPORT_ID(gEnetHelloWorld.macPort), status);
            linked = BFALSE;
            break;
        }

        if (!linked)
        {
            EnetUtils_delay(100U);
            Enet_periodicTick(gEnetHelloWorld.hEnet);
        }
    }

    return status;
}
