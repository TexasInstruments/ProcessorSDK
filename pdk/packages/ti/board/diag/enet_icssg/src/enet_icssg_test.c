/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 * \file  enet_icssg_test.c
 *
 * \brief This file contains the implementation of the Enet lld with icssg example.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/osal.h>
#include "enet_icssg_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t EnetIg_init(void);

static void EnetIg_deinit(void);

static int32_t EnetIg_open(EnetIg_PerCtxt *perCtxts,
                           uint32_t numPerCtxts);

static void EnetIg_close(EnetIg_PerCtxt *perCtxts,
                         uint32_t numPerCtxts);

static int32_t EnetIg_openPort(EnetIg_PerCtxt *perCtxt);

static void EnetIg_closePort(EnetIg_PerCtxt *perCtxt);

static void EnetIg_printStats(EnetIg_PerCtxt *perCtxts,
                              uint32_t numPerCtxts);

static void EnetIg_resetStats(EnetIg_PerCtxt *perCtxts, uint32_t numPerCtxts);

static int32_t EnetIg_waitForLinkUp(EnetIg_PerCtxt *perCtxt);

static void EnetIg_macMode2MacMii(emac_mode macMode,
                                  EnetMacPort_Interface *mii);

static int32_t EnetIg_openDma(EnetIg_PerCtxt *perCtxt);

static void EnetIg_closeDma(EnetIg_PerCtxt *perCtxt);

static void EnetIg_initTxFreePktQ(void);

static void EnetIg_initRxReadyPktQ(EnetDma_RxChHandle hRxCh);

static uint32_t EnetIg_retrieveFreeTxPkts(EnetIg_PerCtxt *perCtxt);

static uint32_t BoardDiag_EnetIg_LpbkTxRx(void);

static uint32_t BoardDiag_EnetIg_Receive(EnetIg_PerCtxt *perCtxt);

static void BoardDiag_EnetIg_txIsrFxn(void *appData);

static void BoardDiag_EnetIg_rxIsrFxn(void *appData);

static int32_t BoardDiag_EnetIg_LpbckTest(EnetIg_PerCtxt *perCtxt, uint32_t numPerCtxts);

static void BoardDiag_printFrame(EthFrame *frame, uint32_t len);

static void BoardDiag_printMacAddr(uint8_t macAddr[]);

static uint32_t BoardDiag_EnetIg_Transmit(EnetIg_PerCtxt *txPerCtxt, EnetIg_PerCtxt *rxPerCtxt);

static void EnetIg_PortSelect(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Enet Icssg Test Object */
EnetIg_Obj gEnetIg;

/* Statistics */
IcssgStats_MacPort gEnetIg_icssgStats;
IcssgStats_Pa gEnetIg_icssgPaStats;

bool printFrameEnable = BFALSE;
bool printStatsEnable = BFALSE;

static uint32_t gEnetLpbk_IterationCount;

/* Use this array to select the ports that will be used in the test */
const EnetIg_TestParams testParams[ENETIG_ICSSG_MAX_PORTS] =
{
    /* ICSSG0 mode: DUALMAC */
    { ENET_ICSSG_DUALMAC, 0U, { ENET_MAC_PORT_1 },                  1U, "icssg0-p1", },
    { ENET_ICSSG_DUALMAC, 1U, { ENET_MAC_PORT_1 },                  1U, "icssg0-p2", },
    /* ICSSG1 mode: DUALMAC */
    { ENET_ICSSG_DUALMAC, 2U, { ENET_MAC_PORT_1 },                  1U, "icssg1-p1", },
    { ENET_ICSSG_DUALMAC, 3U, { ENET_MAC_PORT_1 },                  1U, "icssg1-p2", },
    /* ICSSG2 mode: DUALMAC */
    { ENET_ICSSG_DUALMAC, 4U, { ENET_MAC_PORT_1 },                  1U, "icssg2-p1", },
    { ENET_ICSSG_DUALMAC, 5U, { ENET_MAC_PORT_1 },                  1U, "icssg2-p2", },
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    int32_t status;

    /* Board level initialization, UART might not be setup before this hence no prints yet */
    status = EnetBoard_init();
    if(status != ENET_SOK)
    {
        EnetAppUtils_print("\nBoard init failed with status: %d\n", status);
        return status;
    }

    EnetIg_PortSelect();

    /* Allocating memory to the tx and rx frames */
    gEnetIg.txFrame = malloc(BOARD_DIAG_ENETLPBK_TEST_PKT_LEN + sizeof(EthFrameHeader));
    gEnetIg.rxFrame = malloc(BOARD_DIAG_ENETLPBK_TEST_PKT_LEN + sizeof(EthFrameHeader));

    for (gEnetIg.loopCnt = 0U; gEnetIg.loopCnt < gEnetLpbk_IterationCount; gEnetIg.loopCnt++)
    {
        EnetAppUtils_print("\n\n*******Iteration %d********\n", gEnetIg.loopCnt + 1);

        /* Init driver */
        status = EnetIg_init();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("\nInitialization failed: %d\n", status);
            return status;
        }

        /* Open all peripherals */
        if (status == ENET_SOK)
        {
            status = EnetIg_open(gEnetIg.perCtxt, gEnetIg.numPerCtxts);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("\nFailed to open peripherals: %d\n", status);
            }
        }

        if (status == ENET_SOK)
        {
            status = BoardDiag_EnetIg_LpbckTest(gEnetIg.perCtxt, gEnetIg.numPerCtxts);
        }

        if(status == ENET_SOK)
        {
            EnetAppUtils_print("\n\nTest Passed");
        }
        else
        {
            EnetAppUtils_print("\n\nTest Failed");
        }

        /* Close all peripherals */
        EnetIg_close(gEnetIg.perCtxt, gEnetIg.numPerCtxts);

        /* Deinit driver */
        EnetIg_deinit();
    }

    /* Board level deinitialization */
    EnetBoard_deinit();

    return 0;
}

static void EnetIg_PortSelect(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t port[ENETIG_ICSSG_INSTANCE_MAX] = {0};

#if defined(am65xx_evm)
    /* Select port 4 and port 5 */
    port[0] = 4;
    port[1] = 5;
#elif defined(am65xx_idk)
    /* Can change this to 0 and 1, 4 and 5 for IDK */
    /* Select port 2 and port 3 */
    port[0] = 2;
    port[1] = 3;
#else
    EnetAppUtils_print("\nBoard not supported for this test");
#endif

    /* Initialize test config */
    memset(&gEnetIg, 0, sizeof(gEnetIg));

    gEnetIg.numPerCtxts = ENETIG_ICSSG_INSTANCE_MAX;

    for(i = 0; i < gEnetIg.numPerCtxts; i++)
    {
        gEnetIg.perCtxt[i].enetType = testParams[port[i]].enetType;
        gEnetIg.perCtxt[i].instId   = testParams[port[i]].instId;
        gEnetIg.perCtxt[i].name     = testParams[port[i]].name;

        gEnetIg.perCtxt[i].macPortNum = testParams[port[i]].macPortNum;
        for (j = 0; j < gEnetIg.perCtxt[port[i]].macPortNum; j++)
        {
            gEnetIg.perCtxt[i].macPort[j]  = testParams[port[i]].macPort[j];
        }
    }
    EnetAppUtils_print("\nEnter number of iterations to test:");
    gEnetLpbk_IterationCount = (uint32_t)EnetAppUtils_getNum();
}

static int32_t EnetIg_init(void)
{
    EnetUtils_Cfg utilsCfg;
    int32_t status = ENET_SOK;

    /* Initialize Enet driver (use default utils) */
    EnetAppUtils_print("\nInit Enet's OSAL and utils to use defaults");
    Enet_initUtilsCfg(&utilsCfg);
    Enet_init(&utilsCfg);

    gEnetIg.coreId = EnetSoc_getCoreId();

    /* Initialize memory */
    EnetAppUtils_print("\nInit memory utils");
    status = EnetMem_init();
    if (ENET_SOK != status)
    {
        EnetAppUtils_print("\nFailed to initialize memory utils: %d\n", status);
        EnetAppUtils_assert(BFALSE);
    }

    /* Initialize all queues */
    EnetQueue_initQ(&gEnetIg.txFreePktInfoQ);

    /* Open UDMA driver which is the same handle to be used for all peripherals */
    if (ENET_SOK == status)
    {
        EnetAppUtils_print("\nOpen Main UDMA driver");
        gEnetIg.hMainUdmaDrv = EnetAppUtils_udmaOpen(ENET_ICSSG_DUALMAC, NULL);
        if (NULL == gEnetIg.hMainUdmaDrv)
        {
            EnetAppUtils_print("\nFailed to open Main UDMA driver: %d\n", status);
            status = ENET_EALLOC;
            EnetAppUtils_assert(BFALSE);
        }
    }

    return status;
}

static void EnetIg_asyncIoctlCb(Enet_Event evt,
                                uint32_t evtNum,
                                void *evtCbArgs,
                                void *arg1,
                                void *arg2)
{
    EnetIg_PerCtxt *perCtxt = (EnetIg_PerCtxt *)evtCbArgs;

    EnetAppUtils_print("\n%s: Async IOCTL completed\n", perCtxt->name);
    perCtxt->hAsyncIoctlSem = BTRUE;
}

static void EnetIg_txTsCb(Enet_Event evt,
                          uint32_t evtNum,
                          void *evtCbArgs,
                          void *arg1,
                          void *arg2)
{
    EnetIg_PerCtxt *perCtxt = (EnetIg_PerCtxt *)evtCbArgs;
    Icssg_TxTsEvtCbInfo *txTsInfo = (Icssg_TxTsEvtCbInfo *)arg1;
    Enet_MacPort macPort = *(Enet_MacPort *)arg2;
    uint32_t tsId = txTsInfo->txTsId;
    uint64_t txTs = txTsInfo->ts;
    uint64_t rxTs = perCtxt->rxTs[tsId % ENET_MEM_NUM_RX_PKTS];
    uint64_t prevTs;
    int64_t dt;
    bool status = BTRUE;

    dt = txTs - rxTs;

    EnetAppUtils_print("\n%s: Port %u: RX-to-TX timestamp delta = %10lld (RX=%llu, TX=%llu)\n",
                       perCtxt->name, ENET_MACPORT_ID(macPort), dt, rxTs, txTs);

    /* Check correct timestamp delta */
    if (0 > dt)
    {
        EnetAppUtils_print("\n%s: Port %u: ERROR: RX timestamp > TX timestamp: %llu > %llu\n",
                           perCtxt->name, ENET_MACPORT_ID(macPort), rxTs, txTs);
            status = BFALSE;
    }

    /* Check monotonicity of the TX and RX timestamps */
    if (0U < txTsInfo->txTsId)
    {
        prevTs = perCtxt->rxTs[(tsId - 1) % ENET_MEM_NUM_RX_PKTS];
        if (prevTs > rxTs)
        {
            EnetAppUtils_print("\n%s: Port %u: ERROR: Non monotonic RX timestamp: %llu -> %llu\n",
                               perCtxt->name, ENET_MACPORT_ID(macPort), prevTs, rxTs);
            status = BFALSE;
        }

        prevTs = perCtxt->txTs[(tsId - 1) % ENET_MEM_NUM_RX_PKTS];
        if (prevTs > txTs)
        {
            EnetAppUtils_print("\n%s: Port %u: ERROR: Non monotonic TX timestamp: %llu -> %llu\n",
                               perCtxt->name, ENET_MACPORT_ID(macPort), prevTs, txTs);
            status = BFALSE;
        }
    }

    if (!status)
    {
        EnetAppUtils_print("\n");
    }

    /* Save current timestamp for future monotonicity checks */
     perCtxt->txTs[txTsInfo->txTsId % ENET_MEM_NUM_RX_PKTS] = txTs;

    perCtxt->hTxTsSem = BTRUE;
}

static void EnetIg_deinit(void)
{
    /* Close UDMA driver if not closed */
    EnetAppUtils_print("\nClose UDMA driver");
    if(NULL != gEnetIg.hMainUdmaDrv)
    {
        EnetAppUtils_udmaclose(gEnetIg.hMainUdmaDrv);
    }

    if(NULL != gEnetIg.hMcuUdmaDrv)
    {
        EnetAppUtils_udmaclose(gEnetIg.hMcuUdmaDrv);
    }

    /* Deinitialize Enet driver (use default utils) */
    EnetAppUtils_print("\nDeinit Enet driver");
    Enet_deinit();

    /* Deinitialize memory */
    EnetAppUtils_print("\nDeinit memory utils");
    EnetMem_deInit();

    EnetAppUtils_print("\nDeinit complete");
}

static int32_t EnetIg_open(EnetIg_PerCtxt *perCtxts,
                           uint32_t numPerCtxts)
{
    EnetDma_Cfg *dmaCfg;
    EnetRm_ResCfg *resCfg;
    Enet_IoctlPrms prms;
    EnetPer_AttachCoreOutArgs attachCoreOutArgs;

    uint32_t i;
    uint32_t j;
    int32_t status = ENET_SOK;

    /* Do peripheral dependent initalization */
    EnetAppUtils_print("\nInit all peripheral clocks");
    EnetAppUtils_print("\n----------------------------------------------\n");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &perCtxts[i];
        EnetAppUtils_enableClocks(perCtxt->enetType, perCtxt->instId);

        /*Initialize aync IOCTL and TX timestamp flags*/
        perCtxt->hAsyncIoctlSem = BFALSE;
        perCtxt->hTxTsSem = BFALSE;
    }

    /* Prepare init configuration for all peripherals */
    EnetAppUtils_print("\nInit all configs");
    EnetAppUtils_print("\n----------------------------------------------");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &perCtxts[i];

        dmaCfg = &perCtxt->dmaCfg;
        dmaCfg->rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;

        EnetAppUtils_print("\n%s: init config", perCtxt->name);

        Icssg_Cfg *icssgCfg = &perCtxt->icssgCfg;

        Enet_initCfg(perCtxt->enetType, perCtxt->instId, icssgCfg, sizeof(*icssgCfg));
        resCfg = &icssgCfg->resCfg;

        /* Set DMA configuration */
        dmaCfg->hUdmaDrv = gEnetIg.hMainUdmaDrv;
        icssgCfg->dmaCfg = (void *)dmaCfg;

        /* Initialize RM */
        EnetAppUtils_initResourceConfig(perCtxt->enetType, EnetSoc_getCoreId(), resCfg);

        /* We use software MAC address pool from apputils, but it will give same MAC address.
         * Add port index to make them unique */
        for (j = 0U; j < ENETIG_PORT_MAX; j++)
        {
            resCfg->macList.macAddress[j][ENET_MAC_ADDR_LEN - 1] += (i * ENETIG_PORT_MAX) + j;
        }
        resCfg->macList.numMacAddress = ENETIG_PORT_MAX;
    }

    /* Open Enet driver for all peripherals */
    EnetAppUtils_print("\n\nOpen all peripherals");
    EnetAppUtils_print("\n----------------------------------------------");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &perCtxts[i];
        EnetAppUtils_print("\n%s: Open enet", perCtxt->name);

        Icssg_Cfg *icssgCfg = &perCtxt->icssgCfg;
        perCtxt->hEnet = Enet_open(perCtxt->enetType, perCtxt->instId, icssgCfg, sizeof(*icssgCfg));

        if (perCtxt->hEnet == NULL)
        {
            EnetAppUtils_print("\n%s: failed to open enet\n", perCtxt->name);
            status = ENET_EFAIL;
            break;
        }

        if (Enet_isIcssFamily(perCtxt->enetType))
        {
            EnetAppUtils_print("\n%s: Register async IOCTL callback", perCtxt->name);
            Enet_registerEventCb(perCtxt->hEnet,
                                ENET_EVT_ASYNC_CMD_RESP,
                                0U,
                                EnetIg_asyncIoctlCb,
                                (void *)perCtxt);

            EnetAppUtils_print("\n%s: Register TX timestamp callback", perCtxt->name);
            perCtxt->txTsSeqId = 0U;
            Enet_registerEventCb(perCtxt->hEnet,
                                ENET_EVT_TIMESTAMP_TX,
                                0U,
                                EnetIg_txTsCb,
                                (void *)perCtxt);
        }
    }

    /* Attach the core with RM */
    if (status == ENET_SOK)
    {
        EnetAppUtils_print("\n\nAttach core id %u on all peripherals", gEnetIg.coreId);
        EnetAppUtils_print("\n----------------------------------------------");
        for (i = 0U; i < numPerCtxts; i++)
        {
            EnetIg_PerCtxt *perCtxt = &perCtxts[i];

            EnetAppUtils_print("\n%s: Attach core", perCtxt->name);

            ENET_IOCTL_SET_INOUT_ARGS(&prms, &gEnetIg.coreId, &attachCoreOutArgs);

            status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ENET_PER_IOCTL_ATTACH_CORE, &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("\n%s: failed to attach: %d", perCtxt->name, status);
            }
            else
            {
                gEnetIg.coreKey = attachCoreOutArgs.coreKey;
            }
        }
    }

    return status;
}

static void EnetIg_close(EnetIg_PerCtxt *perCtxts,
                         uint32_t numPerCtxts)
{
    Enet_IoctlPrms prms;
    uint32_t i;
    int32_t status;

    EnetAppUtils_print("\n\nClose DMA for all peripherals");
    EnetAppUtils_print("\n----------------------------------------------");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &perCtxts[i];

        EnetAppUtils_print("\n%s: Close DMA", perCtxt->name);

        EnetIg_closePort(perCtxt);
        EnetIg_closeDma(perCtxt);
    }

    /* Detach core */
    EnetAppUtils_print("\nDetach core from all peripherals");
    EnetAppUtils_print("\n----------------------------------------------");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &perCtxts[i];

        EnetAppUtils_print("\n%s: Detach core", perCtxt->name);

        ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetIg.coreKey);
        status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ENET_PER_IOCTL_DETACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("\n%s: Failed to detach: %d\n", perCtxt->name);
        }
    }

    /* Close opened Enet drivers if any peripheral failed */
    EnetAppUtils_print("\n\nClose all peripherals");
    EnetAppUtils_print("\n----------------------------------------------");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &perCtxts[i];

        EnetAppUtils_print("\n%s: Close enet", perCtxt->name);

        if (Enet_isIcssFamily(perCtxt->enetType))
        {
            EnetAppUtils_print("\n%s: Unregister async IOCTL callback", perCtxt->name);
            Enet_unregisterEventCb(perCtxt->hEnet,
                                   ENET_EVT_ASYNC_CMD_RESP,
                                   0U);

            EnetAppUtils_print("\n%s: Unregister TX timestamp callback", perCtxt->name);
            Enet_unregisterEventCb(perCtxt->hEnet,
                                   ENET_EVT_TIMESTAMP_TX,
                                   0U);
        }

        Enet_close(perCtxt->hEnet);
        perCtxt->hEnet = NULL;
    }

    /* Do peripheral dependent initalization */
    EnetAppUtils_print("\n\nDeinit all peripheral clocks");
    EnetAppUtils_print("\n----------------------------------------------\n");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &perCtxts[i];
        EnetAppUtils_disableClocks(perCtxt->enetType, perCtxt->instId);
    }

    /* Delete the semaphores */
    EnetAppUtils_print("\nDeleting all the semaphores");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &perCtxts[i];
        if(perCtxt->hRxSem != NULL)
        {
            SemaphoreP_delete(perCtxt->hRxSem);
        }

        if(perCtxt->hTxSem != NULL)
        {
            SemaphoreP_delete(perCtxt->hTxSem);
        }
    }
}

static void EnetIg_printStats(EnetIg_PerCtxt *perCtxts,
                              uint32_t numPerCtxts)
{
    Enet_IoctlPrms prms;
    Enet_MacPort macPort;
    uint32_t i;
    uint32_t j;
    int32_t status;

    EnetAppUtils_print("\nPrint statistics\n");
    EnetAppUtils_print("----------------------------------------------\n");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &gEnetIg.perCtxt[i];

        if (Enet_isIcssFamily(perCtxt->enetType))
        {
            EnetAppUtils_print("\n%s - PA statistics\n", perCtxt->name);
            EnetAppUtils_print("--------------------------------\n");
            ENET_IOCTL_SET_OUT_ARGS(&prms, &gEnetIg_icssgPaStats);
            status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ENET_STATS_IOCTL_GET_HOSTPORT_STATS, &prms);
            if (ENET_SOK != status)
            {
                EnetAppUtils_print("\n%s: Failed to get PA stats\n", perCtxt->name);
            }

            EnetAppUtils_printIcssgPaStats(&gEnetIg_icssgPaStats);
            EnetAppUtils_print("\n");
        }

        for (j = 0U; j < perCtxt->macPortNum; j++)
        {
            macPort = perCtxt->macPort[j];

            EnetAppUtils_print("\n%s - Port %u statistics\n", perCtxt->name, ENET_MACPORT_ID(macPort));
            EnetAppUtils_print("--------------------------------\n");

            ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPort, &gEnetIg_icssgStats);

            status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ENET_STATS_IOCTL_GET_MACPORT_STATS, &prms);
            if (ENET_SOK != status)
            {
                EnetAppUtils_print("\n%s: Failed to get port %u stats\n", perCtxt->name, ENET_MACPORT_ID(macPort));
                continue;
            }

            EnetAppUtils_printIcssgMacPortStats(&gEnetIg_icssgStats, BFALSE);
            EnetAppUtils_print("\n");
        }
    }
}

static void EnetIg_resetStats(EnetIg_PerCtxt *perCtxts,
                              uint32_t numPerCtxts)
{
    Enet_IoctlPrms prms;
    Enet_MacPort macPort;
    uint32_t i;
    uint32_t j;
    int32_t status;

    EnetAppUtils_print("\nReset statistics\n");
    EnetAppUtils_print("----------------------------------------------\n");
    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &gEnetIg.perCtxt[i];

        EnetAppUtils_print("\n%s: Reset statistics", perCtxt->name);

        for (j = 0U; j < perCtxt->macPortNum; j++)
        {
            macPort = perCtxt->macPort[j];

            ENET_IOCTL_SET_IN_ARGS(&prms, &macPort);
            status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ENET_STATS_IOCTL_RESET_MACPORT_STATS, &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("\n%s: Failed to reset port %u stats", perCtxt->name, ENET_MACPORT_ID(macPort));
                continue;
            }
        }
    }
}

static int32_t EnetIg_openPort(EnetIg_PerCtxt *perCtxt)
{
    Enet_IoctlPrms prms;
    EnetBoard_EthPort ethPort;
    const EnetBoard_PhyCfg *boardPhyCfg;
    EnetPer_PortLinkCfg portLinkCfg;

    IcssgMacPort_Cfg icssgMacCfg;
    EnetMacPort_LinkCfg *linkCfg = &portLinkCfg.linkCfg;
    EnetMacPort_Interface *mii = &portLinkCfg.mii;
    EnetPhy_Cfg *phyCfg = &portLinkCfg.phyCfg;
    Enet_MacPort macPort;
    uint32_t i;
    int32_t status = ENET_SOK;

    EnetAppUtils_print("\n\nOpen ports");
    EnetAppUtils_print("\n----------------------------------------------");

    for (i = 0U; i < perCtxt->macPortNum; i++)
    {
        macPort = perCtxt->macPort[i];

        EnetAppUtils_print("\n%s: Open port %u", perCtxt->name, ENET_MACPORT_ID(macPort));

        /* Setup board for requested Ethernet port */
        ethPort.enetType = perCtxt->enetType;
        ethPort.instId   = perCtxt->instId;
        ethPort.macPort  = macPort;
        ethPort.boardId  = ENETBOARD_CPB_ID;
        EnetIg_macMode2MacMii(RGMII, &ethPort.mii);

        status = EnetBoard_setupPorts(&ethPort, 1U);
        if (ENET_SOK != status)
        {
            EnetAppUtils_print("\n%s: Failed to setup MAC port %u\n", perCtxt->name, ENET_MACPORT_ID(macPort));
            EnetAppUtils_assert(BFALSE);
        }

        IcssgMacPort_initCfg(&icssgMacCfg);
        portLinkCfg.macCfg = &icssgMacCfg;

        /* Set port link params */
        portLinkCfg.macPort = macPort;

        mii->layerType     = ethPort.mii.layerType;
        mii->sublayerType  = ethPort.mii.sublayerType;
        mii->variantType   = ENET_MAC_VARIANT_FORCED;

        linkCfg->speed     = ENET_SPEED_AUTO;
        linkCfg->duplexity = ENET_DUPLEX_AUTO;

        boardPhyCfg = EnetBoard_getPhyCfg(&ethPort);
        if (NULL != boardPhyCfg)
        {
            EnetPhy_initCfg(phyCfg);
            phyCfg->phyAddr     = boardPhyCfg->phyAddr;
            phyCfg->isStrapped  = boardPhyCfg->isStrapped;
            phyCfg->loopbackEn  = BFALSE;
            phyCfg->skipExtendedCfg = boardPhyCfg->skipExtendedCfg;
            phyCfg->extendedCfgSize = boardPhyCfg->extendedCfgSize;
            memcpy(phyCfg->extendedCfg, boardPhyCfg->extendedCfg, phyCfg->extendedCfgSize);
        }
        else
        {
            EnetAppUtils_print("%s: No PHY configuration found\n", perCtxt->name);
            EnetAppUtils_assert(BFALSE);
        }

        /* Open port link */
        if (ENET_SOK == status)
        {
            ENET_IOCTL_SET_IN_ARGS(&prms, &portLinkCfg);

            EnetAppUtils_print("\n%s: Open port %u link", perCtxt->name, ENET_MACPORT_ID(macPort));
            status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ENET_PER_IOCTL_OPEN_PORT_LINK, &prms);
            if (ENET_SOK != status)
            {
                EnetAppUtils_print("\n%s: Failed to open port link: %d\n", perCtxt->name, status);
            }
        }
    }

    return status;
}

static void EnetIg_closePort(EnetIg_PerCtxt *perCtxt)
{
    Enet_IoctlPrms prms;
    Enet_MacPort macPort;
    uint32_t i;
    int32_t status;

    for (i = 0U; i < perCtxt->macPortNum; i++)
    {
        macPort = perCtxt->macPort[i];

        EnetAppUtils_print("\n%s: Close port %u", perCtxt->name, ENET_MACPORT_ID(macPort));

        /* Close port link */
        ENET_IOCTL_SET_IN_ARGS(&prms, &macPort);

        EnetAppUtils_print("\n%s: Close port %u link", perCtxt->name, ENET_MACPORT_ID(macPort));
        status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("\n%s: Failed to close port link: %d\n", perCtxt->name, status);
        }
    }
}

static int32_t EnetIg_waitForLinkUp(EnetIg_PerCtxt *perCtxt)
{
    Enet_IoctlPrms prms;
    Enet_MacPort macPort;
    IcssgMacPort_SetPortStateInArgs setPortStateInArgs;
    bool linked;

    uint32_t i;
    uint32_t linkCheckTime = 0;
    int32_t status = ENET_SOK;

    EnetAppUtils_print("\n%s: Waiting for link up...\n", perCtxt->name);

    for (i = 0U; i < perCtxt->macPortNum; i++)
    {
        macPort = perCtxt->macPort[i];
        linked = BFALSE;
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPort, &linked);

        while (!linked)
        {
            status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ENET_PER_IOCTL_IS_PORT_LINK_UP, &prms);
            if (ENET_SOK != status)
            {
                EnetAppUtils_print("\n%s: Failed to get port %u link status: %d\n",
                                   perCtxt->name, ENET_MACPORT_ID(macPort), status);
                linked = BFALSE;
                break;
            }

            if (!linked)
            {
                Enet_periodicTick(perCtxt->hEnet);
                EnetUtils_delay(10U);
            }

            linkCheckTime++;
            if(BOARD_DIAG_ICSSG_LINKUP_TIMEOUT < linkCheckTime)
            {
                status = ENET_ETIMEOUT;
                EnetAppUtils_print("\n%s: Port %u link up timed out\n",
                                    perCtxt->name, ENET_MACPORT_ID(macPort));
                break;
            }
        }

        EnetAppUtils_print("\n%s: Port %u link is %s",
                        perCtxt->name, ENET_MACPORT_ID(macPort), linked ? "up" : "down");

        /* Set port to 'Forward' state */
        if (ENET_SOK == status)
        {
            EnetAppUtils_print("\n%s: Set port state to 'Forward'", perCtxt->name);

            setPortStateInArgs.macPort   = macPort;
            setPortStateInArgs.portState = ICSSG_PORT_STATE_FORWARD;
            ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);

            status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ICSSG_PER_IOCTL_SET_PORT_STATE, &prms);
            if (ENET_SINPROGRESS == status)
            {
                /* Wait for asyc ioctl to complete */
                while(BTRUE != perCtxt->hAsyncIoctlSem)
                {
                    Enet_poll(perCtxt->hEnet, ENET_EVT_ASYNC_CMD_RESP, NULL, 0U);
                }
                perCtxt->hAsyncIoctlSem = BFALSE;

                status = ENET_SOK;
            }
            else
            {
                EnetAppUtils_print("\n%s: Failed to set port state: %d\n", perCtxt->name, status);
            }
        }
    }

    return status;
}

static void EnetIg_macMode2MacMii(emac_mode macMode,
                                  EnetMacPort_Interface *mii)
{
    switch (macMode)
    {
#if defined(SOC_AM65XX) || defined(SOC_J7200) || defined(SOC_J721E) || defined(SOC_TPR12) || defined(SOC_AWR294X)
        case RMII:
            mii->layerType    = ENET_MAC_LAYER_MII;
            mii->sublayerType = ENET_MAC_SUBLAYER_REDUCED;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case RGMII:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_REDUCED;
            mii->variantType  = ENET_MAC_VARIANT_FORCED;
            break;
#endif
#if defined(SOC_J7200) || defined(SOC_J721E)
        case GMII:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_STANDARD;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case SGMII:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_SERIAL;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case QSGMII:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case QSGMII_SUB:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case XFI:
            mii->layerType    = ENET_MAC_LAYER_XGMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_STANDARD;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;
#endif
        default:
            EnetAppUtils_print("\nInvalid MAC mode: %u\n", macMode);
            EnetAppUtils_assert(BFALSE);
            break;
    }
}

static void BoardDiag_EnetIg_rxIsrFxn(void *appData)
{
    EnetIg_PerCtxt *perCtxt = (EnetIg_PerCtxt *)appData;

    SemaphoreP_post(perCtxt->hRxSem);
}

static void BoardDiag_EnetIg_txIsrFxn(void *appData)
{
    EnetIg_PerCtxt *perCtxt = (EnetIg_PerCtxt *)appData;

    SemaphoreP_post(perCtxt->hTxSem);
}

static int32_t EnetIg_openDma(EnetIg_PerCtxt *perCtxt)
{
    EnetUdma_OpenRxFlowPrms rxChCfg;
    EnetUdma_OpenTxChPrms txChCfg;
    int32_t status = ENET_SOK;

    /* Open the TX channel */
    EnetUdma_initTxChParams(&txChCfg);

    txChCfg.hUdmaDrv = (ENET_CPSW_2G == perCtxt->enetType) ? gEnetIg.hMcuUdmaDrv : gEnetIg.hMainUdmaDrv;
    txChCfg.cbArg    = perCtxt;
    txChCfg.notifyCb = BoardDiag_EnetIg_txIsrFxn;
    txChCfg.useGlobalEvt = BTRUE;

    EnetAppUtils_setCommonTxChPrms(&txChCfg);

    EnetAppUtils_openTxCh(perCtxt->hEnet,
                          gEnetIg.coreKey,
                          gEnetIg.coreId,
                          &perCtxt->txChNum,
                          &perCtxt->hTxCh,
                          &txChCfg);
    if (NULL == perCtxt->hTxCh)
    {
#if FIX_RM
        /* Free the channel number if open Tx channel failed */
        EnetAppUtils_freeTxCh(gEnetIg.hEnet,
                              gEnetIg.coreKey,
                              gEnetIg.coreId,
                              gEnetIg.txChNum);
#endif
        EnetAppUtils_print("\nEnetIg_openDma() failed to open TX channel");
        status = ENET_EFAIL;
        EnetAppUtils_assert(NULL != perCtxt->hTxCh);
    }

    /* Allocate TX packets and keep them locally enqueued */
    if (ENET_SOK == status)
    {
        EnetIg_initTxFreePktQ();
    }

    /* Open the RX flow */
    if (ENET_SOK == status)
    {
        EnetUdma_initRxFlowParams(&rxChCfg);

        rxChCfg.hUdmaDrv = (ENET_CPSW_2G == perCtxt->enetType) ? gEnetIg.hMcuUdmaDrv : gEnetIg.hMainUdmaDrv;
        rxChCfg.notifyCb = BoardDiag_EnetIg_rxIsrFxn;
        rxChCfg.cbArg    = perCtxt;
        rxChCfg.useGlobalEvt = BTRUE;
        rxChCfg.flowPrms.sizeThreshEn = 0U;

        EnetAppUtils_setCommonRxFlowPrms(&rxChCfg);
        EnetAppUtils_openRxFlow(perCtxt->enetType,
                                perCtxt->hEnet,
                                gEnetIg.coreKey,
                                gEnetIg.coreId,
                                BTRUE,
                                &perCtxt->rxStartFlowIdx,
                                &perCtxt->rxFlowIdx,
                                &perCtxt->macAddr[0U],
                                &perCtxt->hRxCh,
                                &rxChCfg);

        if (NULL == perCtxt->hRxCh)
        {
            EnetAppUtils_print("\nEnetIg_openRxCh() failed to open RX flow");
            status = ENET_EFAIL;
            EnetAppUtils_assert(NULL != perCtxt->hRxCh);
        }
    }

    /* Submit all ready RX buffers to DMA */
    if (ENET_SOK == status)
    {
        EnetIg_initRxReadyPktQ(perCtxt->hRxCh);
    }

    return status;
}

static void EnetIg_closeDma(EnetIg_PerCtxt *perCtxt)
{
    EnetDma_PktQ fqPktInfoQ;
    EnetDma_PktQ cqPktInfoQ;

    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    /* Close RX channel */
    EnetAppUtils_closeRxFlow(perCtxt->enetType,
                            perCtxt->hEnet,
                            gEnetIg.coreKey,
                            gEnetIg.coreId,
                            BTRUE,
                            &fqPktInfoQ,
                            &cqPktInfoQ,
                            perCtxt->rxStartFlowIdx,
                            perCtxt->rxFlowIdx,
                            perCtxt->macAddr,
                            perCtxt->hRxCh);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    /* Close TX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    /* Retrieve any pending TX packets from driver */
    EnetIg_retrieveFreeTxPkts(perCtxt);

    EnetAppUtils_closeTxCh(perCtxt->hEnet,
                           gEnetIg.coreKey,
                           gEnetIg.coreId,
                           &fqPktInfoQ,
                           &cqPktInfoQ,
                           perCtxt->hTxCh,
                           perCtxt->txChNum);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&gEnetIg.txFreePktInfoQ);
}

static void EnetIg_initTxFreePktQ(void)
{
    EnetDma_Pkt *pPktInfo;
    uint32_t i;

    /* Initialize TX EthPkts and queue them to txFreePktInfoQ */
    for (i = 0U; i < ENET_MEM_NUM_TX_PKTS; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&gEnetIg,
                                       ENET_MEM_LARGE_POOL_PKT_SIZE,
                                       ENETDMA_CACHELINE_ALIGNMENT);
        EnetAppUtils_assert(NULL != pPktInfo);
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);

        EnetQueue_enq(&gEnetIg.txFreePktInfoQ, &pPktInfo->node);
    }

    EnetAppUtils_print("\ninitQs() txFreePktInfoQ initialized with %d pkts",
                       EnetQueue_getQCount(&gEnetIg.txFreePktInfoQ));
}

static void EnetIg_initRxReadyPktQ(EnetDma_RxChHandle hRxCh)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_PktQ rxFreeQ;
    EnetDma_Pkt *pPktInfo;
    uint32_t i;
    int32_t status;

    EnetQueue_initQ(&rxFreeQ);

    for (i = 0U; i < ENET_MEM_NUM_RX_PKTS; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&gEnetIg,
                                       ENET_MEM_LARGE_POOL_PKT_SIZE,
                                       ENETDMA_CACHELINE_ALIGNMENT);
        EnetAppUtils_assert(NULL != pPktInfo);

        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);

        EnetQueue_enq(&rxFreeQ, &pPktInfo->node);
    }

    /* Retrieve any packets which are ready */
    EnetQueue_initQ(&rxReadyQ);
    status = EnetDma_retrieveRxPktQ(hRxCh, &rxReadyQ);
    EnetAppUtils_assert(ENET_SOK == status);

    /* There should not be any packet with DMA during init */
    EnetAppUtils_assert(0U == EnetQueue_getQCount(&rxReadyQ));

    EnetDma_submitRxPktQ(hRxCh, &rxFreeQ);

    /* Assert here, as during init, the number of DMA descriptors should be equal to
     * the number of free Ethernet buffers available with app */
    EnetAppUtils_assert(0U == EnetQueue_getQCount(&rxFreeQ));
}

static uint32_t EnetIg_retrieveFreeTxPkts(EnetIg_PerCtxt *perCtxt)
{
    EnetDma_PktQ txFreeQ;
    EnetDma_Pkt *pktInfo;
    uint32_t txFreeQCnt = 0U;
    int32_t status;

    EnetQueue_initQ(&txFreeQ);

    /* Retrieve any packets that may be free now */
    status = EnetDma_retrieveTxPktQ(perCtxt->hTxCh, &txFreeQ);
    if (ENET_SOK == status)
    {
        txFreeQCnt = EnetQueue_getQCount(&txFreeQ);

        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        while (NULL != pktInfo)
        {
            EnetDma_checkPktState(&pktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_DRIVER,
                                    ENET_PKTSTATE_APP_WITH_FREEQ);

            EnetQueue_enq(&gEnetIg.txFreePktInfoQ, &pktInfo->node);
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        }
    }
    else
    {
        EnetAppUtils_print("\nretrieveFreeTxPkts() failed to retrieve pkts: %d", status);
    }

    return txFreeQCnt;
}

static int32_t BoardDiag_EnetIg_LpbckTest(EnetIg_PerCtxt *perCtxts, uint32_t numPerCtxts)
{
    uint32_t i;
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    SemaphoreP_Params semParams;

    for (i = 0U; i < numPerCtxts; i++)
    {
        EnetIg_PerCtxt *perCtxt = &perCtxts[i];

        SemaphoreP_Params_init(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;

        perCtxt->hRxSem = SemaphoreP_create(0, &semParams);
        EnetAppUtils_assert(NULL != perCtxt->hRxSem);

        perCtxt->hTxSem = SemaphoreP_create(0, &semParams);
        EnetAppUtils_assert(NULL != perCtxt->hTxSem);

        status = EnetIg_openPort(perCtxt);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("\n%s: Failed to open port link: %d\n", perCtxt->name, status);
        }

        status = EnetIg_waitForLinkUp(perCtxt);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("\n%s: Failed to wait for link up: %d\n", perCtxt->name, status);
        }

        /* Open DMA for peripheral/port */
        if (status == ENET_SOK)
        {
            EnetAppUtils_print("\n%s: Open DMA", perCtxt->name);

            status = EnetIg_openDma(perCtxt);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("\n%s: failed to open DMA: %d\n", perCtxt->name, status);
            }
        }

        /* Add port MAC entry */
        if ((status == ENET_SOK) && (Enet_isIcssFamily(perCtxt->enetType)))
        {
            EnetAppUtils_print("\n%s: Set MAC addr: ", perCtxt->name);
            EnetAppUtils_printMacAddr(&perCtxt->macAddr[0U]);

            if (perCtxt->enetType == ENET_ICSSG_DUALMAC)
            {
                IcssgMacPort_SetMacAddressInArgs inArgs;

                memset(&inArgs, 0, sizeof(inArgs));
                inArgs.macPort = perCtxt->macPort[0U];
                EnetUtils_copyMacAddr(&inArgs.macAddr[0U], &perCtxt->macAddr[0U]);
                ENET_IOCTL_SET_IN_ARGS(&prms, &inArgs);

                status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ICSSG_MACPORT_IOCTL_SET_MACADDR, &prms);
            }
            else
            {
                Icssg_MacAddr addr; // FIXME Icssg_MacAddr type

                /* Set host port's MAC address */
                EnetUtils_copyMacAddr(&addr.macAddr[0U], &perCtxt->macAddr[0U]);
                ENET_IOCTL_SET_IN_ARGS(&prms, &addr);

                status = Enet_ioctl(perCtxt->hEnet, gEnetIg.coreId, ICSSG_HOSTPORT_IOCTL_SET_MACADDR, &prms);
            }

            if (status != ENET_SOK)
            {
                EnetAppUtils_print("\n%s: Failed to set MAC address entry: %d\n", perCtxt->name, status);
            }
        }
    }

    if(status == ENET_SOK)
    {
        status = BoardDiag_EnetIg_LpbkTxRx();
    }
    return status;
}

static uint32_t BoardDiag_EnetIg_Receive(EnetIg_PerCtxt *perCtxt)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_PktQ rxFreeQ;
    EnetDma_Pkt *rxPktInfo;

    uint32_t rxReadyCnt = 0U;
    int32_t status;

    EnetQueue_initQ(&rxReadyQ);
    EnetQueue_initQ(&rxFreeQ);

    /* Get the packets received so far */
    status = EnetDma_retrieveRxPktQ(perCtxt->hRxCh, &rxReadyQ);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("\nreceivePkts() failed to retrieve pkts: %d\n", status);
    }
    else
    {
        rxReadyCnt = EnetQueue_getQCount(&rxReadyQ);

        /* Consume the received packets */
        rxPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxReadyQ);

        while(rxPktInfo != NULL)
        {
            EnetDma_checkPktState(&rxPktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_DRIVER,
                                    ENET_PKTSTATE_APP_WITH_READYQ);

            gEnetIg.rxFrame = (EthFrame *)rxPktInfo->bufPtr;

            EnetDma_checkPktState(&rxPktInfo->pktState,
                                ENET_PKTSTATE_MODULE_APP,
                                ENET_PKTSTATE_APP_WITH_READYQ,
                                ENET_PKTSTATE_APP_WITH_FREEQ);

            /* Release the received packet */
            EnetQueue_enq(&rxFreeQ, &rxPktInfo->node);
            rxPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxReadyQ);
        }

        EnetAppUtils_validatePacketState(&rxFreeQ,
                                    ENET_PKTSTATE_APP_WITH_FREEQ,
                                    ENET_PKTSTATE_APP_WITH_DRIVER);

        /* Submit now processed buffers */
        EnetDma_submitRxPktQ(perCtxt->hRxCh, &rxFreeQ);
    }

    return rxReadyCnt;
}

static uint32_t BoardDiag_EnetIg_Transmit(EnetIg_PerCtxt *txPerCtxt, EnetIg_PerCtxt *rxPerCtxt)
{
    EnetDma_PktQ txSubmitQ;
    EnetDma_Pkt *txPktInfo;

    int32_t status = ENET_SOK;
    uint8_t bcastAddr[ENET_MAC_ADDR_LEN] =
            {0xffU, 0xffU, 0xffU, 0xffU, 0xffU, 0xffU};

    uint32_t pktCnt = 0;
    uint32_t txRetrievePktCnt = 0;
    uint32_t loopTxPktCnt = 0;
    uint32_t loopRxPktCnt = 0;
    uint32_t rxReadyCnt = 0;
    uint32_t txCnt = 0;
    SemaphoreP_Status semStatus;

    while (pktCnt < BOARD_DIAG_ENETLPBK_TEST_PKT_NUM)
    {
        loopRxPktCnt = loopTxPktCnt = 0U;

        /* Retrieve TX packets from driver and recycle them */
        EnetIg_retrieveFreeTxPkts(txPerCtxt);

        /* Transmit a single packet */
        EnetQueue_initQ(&txSubmitQ);

        /* Dequeue one free TX Eth packet */
        txPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetIg.txFreePktInfoQ);

        memset(gEnetIg.txFrame, 0, BOARD_DIAG_ENETLPBK_TEST_PKT_LEN + sizeof(EthFrameHeader));
        memset(gEnetIg.rxFrame, 0, BOARD_DIAG_ENETLPBK_TEST_PKT_LEN + sizeof(EthFrameHeader));

        while (NULL != txPktInfo)
        {
            pktCnt++;
            /* Fill the TX Eth frame with test content */
            gEnetIg.txFrame = (EthFrame *)txPktInfo->bufPtr;
            memcpy(gEnetIg.txFrame->hdr.dstMac, &bcastAddr[0U], ENET_MAC_ADDR_LEN);
            memcpy(gEnetIg.txFrame->hdr.srcMac, &txPerCtxt->macAddr[0U], ENET_MAC_ADDR_LEN);

            gEnetIg.txFrame->hdr.etherType = Enet_htons(0x0800);

            memset(&gEnetIg.txFrame->payload[0U], (uint8_t)(0xA5 + pktCnt),
                    BOARD_DIAG_ENETLPBK_TEST_PKT_LEN);

            txPktInfo->userBufLen = BOARD_DIAG_ENETLPBK_TEST_PKT_LEN +
                                  sizeof(EthFrameHeader);

            txPktInfo->appPriv = &gEnetIg;
            txPktInfo->tsInfo.enableHostTxTs = BFALSE;

            /* Enqueue the packet for later transmission */
            EnetQueue_enq(&txSubmitQ, &txPktInfo->node);

            if (BOARD_DIAG_ENETLPBK_TEST_PKT_NUM <= pktCnt)
            {
                break;
            }

            /* Dequeue one free TX Eth packet */
            txPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetIg.txFreePktInfoQ);
        }

        loopTxPktCnt = EnetQueue_getQCount(&txSubmitQ);
        while (0U != EnetQueue_getQCount(&txSubmitQ))
        {
            txCnt = EnetQueue_getQCount(&txSubmitQ);
            status = EnetDma_submitTxPktQ(txPerCtxt->hTxCh, &txSubmitQ);

            semStatus = SemaphoreP_pend(txPerCtxt->hTxSem, BOARD_DIAG_ENETLPBK_TIMEOUT);

            if(semStatus == SemaphoreP_TIMEOUT)
            {
                status = ENET_ETIMEOUT;
                EnetAppUtils_print("\n\nTrasmit timeout at packet %d", loopTxPktCnt);
                break;
            }


            /* Retrieve TX free packets */
            if (ENET_SOK == status)
            {
                txCnt            = txCnt - EnetQueue_getQCount(&txSubmitQ);
                txRetrievePktCnt = 0U;
                while (txRetrievePktCnt != txCnt)
                {
                    /* TODO this is not failure as HW is busy sending
                     * packets, we need to wait and again call retrieve
                     * packets */
                    Osal_delay(10);
                    txRetrievePktCnt += EnetIg_retrieveFreeTxPkts(txPerCtxt);
                }
            }
            else
            {
                break;
            }
        }

        if(ENET_SOK == status)
        {
            /* wait to receive the packet */
            do
            {
                semStatus = SemaphoreP_pend(rxPerCtxt->hRxSem, BOARD_DIAG_ENETLPBK_TIMEOUT);

                if(semStatus == SemaphoreP_TIMEOUT)
                {
                    status = ENET_ETIMEOUT;
                    EnetAppUtils_print("\n\nReceive timeout at packet %d", loopRxPktCnt);
                    break;
                }

                rxReadyCnt = 0;

                if(ENET_SOK == status)
                {
                    rxReadyCnt = BoardDiag_EnetIg_Receive(rxPerCtxt);
                    loopRxPktCnt += rxReadyCnt;

                    status = memcmp(&gEnetIg.txFrame->payload[0], &gEnetIg.rxFrame->payload[0], rxReadyCnt);
                    if(ENET_SOK != status)
                    {
                        EnetAppUtils_print("\n\nPacket mismatch error at packet %d", pktCnt);
                        EnetAppUtils_print("\n\nRx Byte Count = %d", rxReadyCnt);
                        if(BTRUE == printFrameEnable)
                        {
                            EnetAppUtils_print("\n\nTransmitted Packets: ", pktCnt);
                            BoardDiag_printFrame(gEnetIg.rxFrame, rxReadyCnt);
                        }
                    }
                }
                else
                {
                    break;
                }

            }while(loopRxPktCnt < loopTxPktCnt);

            gEnetIg.totalRxCnt += loopTxPktCnt;
        }
    }

    gEnetIg.totalTxCnt += pktCnt;

    EnetAppUtils_print("\n\nTransmitted - %d packets\nReceived - %d packets\n",
                        loopTxPktCnt, loopRxPktCnt);

    return status;
}

uint32_t BoardDiag_EnetIg_LpbkTxRx(void)
{
    int32_t status = ENET_SOK;

    gEnetIg.totalTxCnt = 0U;
    gEnetIg.totalRxCnt = 0U;

    EnetAppUtils_print("\n\nPort %d to Port %d", gEnetIg.perCtxt[0].instId, gEnetIg.perCtxt[1].instId);

    status |= BoardDiag_EnetIg_Transmit(&gEnetIg.perCtxt[0], &gEnetIg.perCtxt[1]);
    Osal_delay(100);

    EnetAppUtils_print("\n\nPort %d to Port %d", gEnetIg.perCtxt[1].instId, gEnetIg.perCtxt[0].instId);

    status |= BoardDiag_EnetIg_Transmit(&gEnetIg.perCtxt[1], &gEnetIg.perCtxt[0]);
    Osal_delay(100);

    if(ENET_SOK == status)
    {
        if(gEnetIg.totalTxCnt != gEnetIg.totalRxCnt)
        {
            EnetAppUtils_print("\n\nPacket Loss, Test Failed");
            EnetAppUtils_print("\n\nTest Iteration %d failed", gEnetIg.loopCnt+1);
            gEnetIg.failIterations++;
            printStatsEnable = BTRUE;
        }
        else
        {
            EnetAppUtils_print("\n\nTest Iteration %d passed", gEnetIg.loopCnt+1);
            gEnetIg.passIterations++;
        }
    }
    else
    {
        gEnetIg.failIterations++;
        printStatsEnable = BTRUE;
        EnetAppUtils_print("\n\nFailed to transmit/receive packets");
    }

    EnetAppUtils_print("\n\nNo. of tests passed: %d", gEnetIg.passIterations);
    EnetAppUtils_print("\n\nNo. of tests failed: %d", gEnetIg.failIterations);
    if(BTRUE == printStatsEnable)
    {
        /* Print statistics */
        EnetIg_printStats(gEnetIg.perCtxt, gEnetIg.numPerCtxts);

        EnetIg_resetStats(gEnetIg.perCtxt, gEnetIg.numPerCtxts);

        printStatsEnable = BFALSE;
    }

    return status;
}

/**
 * \brief   This function is used to display Mac address
 *
 * \param   EthFrame EthFrame[] frame to be displayed
 *          uint32_t len        length of the frame
 * \return  NULL
 *
 */
static void BoardDiag_printFrame(EthFrame *frame, uint32_t len)
{
    uint8_t *payload;
    uint32_t i;

    EnetAppUtils_print("\nDst addr : ");
    BoardDiag_printMacAddr(&frame->hdr.dstMac[0]);

    EnetAppUtils_print("\nSrc addr : ");
    BoardDiag_printMacAddr(&frame->hdr.srcMac[0]);


    EnetAppUtils_print("\nEtherType: 0x%04x",
                Enet_ntohs(frame->hdr.etherType) & 0xFFFFU);
    payload = frame->payload;

    EnetAppUtils_print("\nPayload  : ");
    for (i = 0; i < len; i++)
    {
        EnetAppUtils_print("0x%02x ", payload[i]);
        if (i && (((i + 1) % OCTETS_PER_ROW) == 0))
        {
            EnetAppUtils_print("\n           ");
        }
    }

    if (len && ((len % OCTETS_PER_ROW) != 0))
    {
        EnetAppUtils_print("\n");
    }

    EnetAppUtils_print("\n");
}

/**
 * \brief   This function is used to display Mac address
 *
 * \param   uint8_t macAddr[] MAC Address
 *
 * \return   NULL
 *
 */
static void BoardDiag_printMacAddr(uint8_t macAddr[])
{
    EnetAppUtils_print("%02x:%02x:%02x:%02x:%02x:%02x\n",
                       macAddr[0] & 0xFF,
                       macAddr[1] & 0xFF,
                       macAddr[2] & 0xFF,
                       macAddr[3] & 0xFF,
                       macAddr[4] & 0xFF,
                       macAddr[5] & 0xFF);
}

