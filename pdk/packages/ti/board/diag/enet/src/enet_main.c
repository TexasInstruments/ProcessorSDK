/*
 *  Copyright (c) Texas Instruments Incorporated 2022
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
 *  \file   enet_main.c
 *
 *  \brief  QSGMII ENET ethernet diagnostic test file
 *
 *
 *  Operation: This is the Loopback test code for the Ethernet Interface.
 *  The test code showcases usage of the ENET Driver exported API for
 *  sending/receiving Ethernet packets.
 *
 *  Test Setup: To run the example,
 *              1) Connect ENET expansion board on J52 port of EVM board.
 *              2) Connect ethernet loopback cable to the Port0 and Port1 of
 *                 J2 connector on ENET board.
 *              3) Connect ethernet loopback cable to the Port0 and Port1 of
 *                 J1 connector on ENET board.
 *              4) Run the application and select the option for respective loopback
 *
 *  Supported SoCs: J784S4.
 *
 *  Supported Platforms:j784s4_evm.
 *
 */
 
/* Test application local header file */
#include "enet_main.h"

/* Enet loopback test object */
BoardDiag_EnetObj gEnetLpbk;

volatile bool txSem = BFALSE;
volatile bool rxSem = BFALSE;

uint32_t txScatterSegments[] =
{
    [0] = sizeof(EthFrameHeader),
    [1] = (ENETLPBK_TEST_PKT_LEN / 3),
    [2] = (ENETLPBK_TEST_PKT_LEN / 3),
    [3] = ((ENETLPBK_TEST_PKT_LEN / 3) + (ENETLPBK_TEST_PKT_LEN % 3)),
};

uint32_t rxScatterSegments[] = 
{
    [0] = (ENETLPBK_TEST_PKT_LEN + sizeof(EthFrameHeader)),
    [1] = (ENETLPBK_TEST_PKT_LEN / 3),
    [2] = (ENETLPBK_TEST_PKT_LEN / 3),
    [3] = ((ENETLPBK_TEST_PKT_LEN / 3) + (ENETLPBK_TEST_PKT_LEN % 3) + 32),
};

/**
 * \brief   ENET receive ISR function
 *
 * \param   appData      [IN]   Pointer to application data
 *
 */
void BoardDiag_enetRxIsrFxn(void *appData)
{
    rxSem = BTRUE;
}

/**
 * \brief   ENET transmit ISR function
 *
 * \param   appData      [IN]   Pointer to application data
 *
 */
void BoardDiag_enetTxIsrFxn(void *appData)
{
    txSem = BTRUE;
}

/**
 * \brief   This function is used to queue the received packets to rx ready queue
 *
 * \return  uint32_t - Receive ready queue count
 *
 */
uint32_t BoardDiag_enetLpbkReceivePkts(void)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pktInfo;
    int32_t status;
    uint32_t rxReadyCnt = 0U;

    EnetQueue_initQ(&rxReadyQ);

    /* Retrieve any ENET packets which are ready */
    status = EnetDma_retrieveRxPktQ(gEnetLpbk.hRxCh, &rxReadyQ);
    if (status == ENET_SOK)
    {
        rxReadyCnt = EnetQueue_getQCount(&rxReadyQ);

        /* Queue the received packet to rxReadyQ and pass new ones from rxFreeQ
        **/
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxReadyQ);
        while (pktInfo != NULL)
        {
            EnetDma_checkPktState(&pktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_DRIVER,
                                    ENET_PKTSTATE_APP_WITH_READYQ);

            EnetQueue_enq(&gEnetLpbk.rxReadyQ, &pktInfo->node);
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxReadyQ);
        }
    }
    else
    {
        UART_printf("receivePkts() failed to retrieve pkts: %d\n",
                           status);
    }

    return rxReadyCnt;
}

bool EnetLpbk_verifyRxFrame(EnetDma_Pkt *pktInfo, uint8_t rxCnt)
{
    uint8_t *rxPayload;
    EthFrame *rxframe;
    uint8_t verifyRxpkt = 0xA5+rxCnt;
    bool retval = BFALSE;
    uint32_t i,j;
    uint32_t segmentLen, headerLen;
    bool incorrectPayload = BFALSE;

    rxframe = (EthFrame *)pktInfo->sgList.list[0U].bufPtr;
    rxPayload = rxframe->payload;

    if (1 == pktInfo->sgList.numScatterSegments)
    {
        for (i = 0; i < ENETLPBK_TEST_PKT_LEN; i++)
        {
            if((rxPayload[i] != verifyRxpkt))
            {
                retval = BFALSE;
                break;
            }
            retval = BTRUE;
        }
    }
    else
    {
        headerLen = rxPayload - pktInfo->sgList.list[0U].bufPtr;
        for (i = 0; i < pktInfo->sgList.numScatterSegments; i++)
        {
            segmentLen = pktInfo->sgList.list[i].segmentFilledLen;
            if(0 == i)
            {
                segmentLen -= headerLen;
            }
            else
            {
                rxPayload = pktInfo->sgList.list[i].bufPtr;
            }
            for (j = 0; j < segmentLen; j++)
            {
                if((rxPayload[j] != verifyRxpkt))
                {
                    retval = BFALSE;
                    incorrectPayload = BTRUE;
                    break;
                }
                retval = BTRUE;
            }
            if(BTRUE == incorrectPayload)
            {
                break;
            }
        }
    }

    return retval;
}

/**
 * \brief   This function is used to retrieve's any CPSW packets that may be free now
 *
 * \param   NULL
 *
 * \return  uint32_t - Transmit free queue count
 *
 */
uint32_t BoardDiag_enetRetrieveFreeTxPkts(void)
{
    EnetDma_PktQ txFreeQ;
    EnetDma_Pkt *pktInfo;
    int32_t status;
    uint32_t txFreeQCnt = 0U;

    EnetQueue_initQ(&txFreeQ);

    /* Retrieve any ENET packets that may be free now */
    status = EnetDma_retrieveTxPktQ(gEnetLpbk.hTxCh, &txFreeQ);
    if (status == ENET_SOK)
    {
        txFreeQCnt = EnetQueue_getQCount(&txFreeQ);

        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        while (NULL != pktInfo)
        {
            EnetDma_checkPktState(&pktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_DRIVER,
                                    ENET_PKTSTATE_APP_WITH_FREEQ);

            EnetQueue_enq(&gEnetLpbk.txFreePktInfoQ, &pktInfo->node);
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        }
    }
    else
    {
        UART_printf("retrieveFreeTxPkts() failed to retrieve pkts: %d\n",
                           status);
    }

    return txFreeQCnt;
}

void BoardDiag_enetGetTestConfig(Enet_Type *enetType,
                           uint32_t *instId,
                           Enet_MacPort *macPort0,
                           Enet_MacPort *macPort1,
                           EnetMacPort_Interface *mii,
                           uint32_t *boardId,
                           uint32_t *expPort)
{

    uint32_t userInput = 0;
    uint32_t enetCard;
    
    *enetType = ENET_CPSW_9G;
    *instId   = 0U;
    
    UART_printf  ("************************************************\n");
    UART_printf  ("*             ENET QSGMII LOOPBACK Test        *\n");
    UART_printf  ("************************************************\n");

    enetCard = Board_detectEnetCard(BOARD_ID_ENET);

    if (enetCard == BOARD_ENET_QSGMII)
    {
        *boardId = ENETBOARD_QPENET_ID;
        *expPort = ENETBOARD_EXP_PORT_ENET1;
        mii->layerType    = ENET_MAC_LAYER_GMII;
        mii->variantType   = ENET_MAC_VARIANT_FORCED;
    }
    else
    {
        UART_printf("QP_ENET CARD is not detected\n");
    }

    UART_printf("Select any one option from below\n\r");
    UART_printf("1 - Port0 & Port1 Loopback Test\n\r");
    UART_printf("2 - Port2 & Port3 Loopback Test\n\r");

    UART_scanFmt("%d", (uint8_t*)&userInput);

    if(userInput == BOARD_DIAG_ENET_SGMII_P0P1_TEST)
    {
        *macPort0 = ENET_MAC_PORT_1;
        *macPort1 = ENET_MAC_PORT_3;
    }
    else
    {
        *macPort0 = ENET_MAC_PORT_4;
        *macPort1 = ENET_MAC_PORT_5;
    }

}

/**
 * \brief   This function is used for ENET packet transmission and reception
 *
 * \param   NULL
 *
 * \return  uint32_t
 *                  0 - Transmission and reception successfull
 *                  1 - Transmission or reception failed
 */
int32_t BoardDiag_enetPktRxTx(void)
{
    EnetDma_PktQ txSubmitQ;
    EnetDma_Pkt *pktInfo;
    EthFrame *frame;
    uint32_t         txRetrievePktCnt;
    uint32_t         loopTxPktCnt;
    uint32_t         totalTxCnt;
    uint32_t         rxReadyCnt;
    uint32_t         loopCnt;
    uint32_t         pktCnt;
    uint32_t         rxPktCnt = 0;
    uint32_t         loopRxPktCnt;
    int32_t          status = ENET_SOK;
    uint8_t bcastAddr[ENET_MAC_ADDR_LEN] = {0xffU, 0xffU, 0xffU, 0xffU, 0xffU, 0xffU};
     
    totalTxCnt = 0U;
    for (loopCnt = 0U; loopCnt < ENETLPBK_NUM_ITERATION; loopCnt++)
    {
        pktCnt = 0U;
        while (pktCnt < ENETLPBK_TEST_PKT_NUM)
        {
            loopRxPktCnt = loopTxPktCnt = 0U;
            /* Transmit a single packet */
            EnetQueue_initQ(&txSubmitQ);

            /* Dequeue one free TX Eth packet */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetLpbk.txFreePktInfoQ);

            while ( NULL != pktInfo )
            {
                pktCnt++;
                /* Fill the TX Eth frame with test content */
                frame = (EthFrame *)pktInfo->sgList.list[0U].bufPtr;

                memcpy(frame->hdr.dstMac,
                       bcastAddr,
                       ENET_MAC_ADDR_LEN);

                memcpy(frame->hdr.srcMac,
                       &gEnetLpbk.hostMacAddr0[0U],
                       ENET_MAC_ADDR_LEN);

                frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
                memset(&frame->payload[0U],
                       (uint8_t)(0xA5 + pktCnt),
                       ENETLPBK_TEST_PKT_LEN);
                       
                pktInfo->sgList.list[0U].segmentFilledLen = ENETLPBK_TEST_PKT_LEN + sizeof (EthFrameHeader);
                pktInfo->appPriv    = &gEnetLpbk;
                pktInfo->chkSumInfo = 0U;
                
                EnetDma_checkPktState(&pktInfo->pktState,
                                        ENET_PKTSTATE_MODULE_APP,
                                        ENET_PKTSTATE_APP_WITH_FREEQ,
                                        ENET_PKTSTATE_APP_WITH_DRIVER);

                /* Enqueue the packet for later transmission */
                EnetQueue_enq(&txSubmitQ, &pktInfo->node);

                if (pktCnt >= ENETLPBK_TEST_PKT_NUM)
                {
                    break;
                }
                
                /* Dequeue one free TX Eth packet */
                pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetLpbk.txFreePktInfoQ);
            }

            loopTxPktCnt = EnetQueue_getQCount(&txSubmitQ);
            while (0U != EnetQueue_getQCount(&txSubmitQ))
            {
                uint32_t txCnt = EnetQueue_getQCount(&txSubmitQ);
                status = EnetDma_submitTxPktQ(gEnetLpbk.hTxCh,
                                                      &txSubmitQ);
                while (BTRUE != txSem);
                txSem = BFALSE;
                
                /* Retrieve TX free packets */
                if (status == ENET_SOK)
                {
                    txCnt = txCnt - EnetQueue_getQCount(&txSubmitQ);
                    txRetrievePktCnt = 0U;
                    while (txRetrievePktCnt != txCnt)
                    {
                        BOARD_delay(1);
                        txRetrievePktCnt |= BoardDiag_enetRetrieveFreeTxPkts();
                    }
                }
                else
                {
                    break;
                }
                
            }

            /* Wait for packet reception */
            do
            {
                while (BTRUE != rxSem);
                rxSem = BFALSE;

                /* Get the packets received so far */
                rxReadyCnt = BoardDiag_enetLpbkReceivePkts();
                if (rxReadyCnt > 0U)
                {
                    /* Consume the received packets and release them */
                    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetLpbk.rxReadyQ);
                    while (pktInfo != NULL)
                    {
                    rxPktCnt++;
                        EnetDma_checkPktState(&pktInfo->pktState,
                                            ENET_PKTSTATE_MODULE_APP,
                                            ENET_PKTSTATE_APP_WITH_READYQ,
                                            ENET_PKTSTATE_APP_WITH_FREEQ);

                        /* Consume the packet by just printing its content */
                        if (gEnetLpbk.printFrame)
                        {
                            EnetAppUtils_printFrame(frame,
                                                    (pktInfo->sgList.list[0U].segmentFilledLen - sizeof (EthFrameHeader)));
                        }
                    EnetAppUtils_assert(BTRUE == EnetLpbk_verifyRxFrame(pktInfo, rxPktCnt));
                       /* Release the received packet */
                        EnetQueue_enq(&gEnetLpbk.rxFreeQ, &pktInfo->node);
                        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetLpbk.rxReadyQ);
                    }

                    /*Submit now processed buffers */
                    if (status == ENET_SOK)
                    {
                        EnetAppUtils_validatePacketState(&gEnetLpbk.rxFreeQ,
                                                     ENET_PKTSTATE_APP_WITH_FREEQ,
                                                     ENET_PKTSTATE_APP_WITH_DRIVER);

                    EnetDma_submitRxPktQ(gEnetLpbk.hRxCh,
                                         &gEnetLpbk.rxFreeQ);
                    }
                }
                loopRxPktCnt |= rxReadyCnt;
            }
            while (loopRxPktCnt < loopTxPktCnt);

            totalTxCnt |= loopTxPktCnt;
        }
    }

    if (status == ENET_SOK)
    {
        UART_printf("\nTransmitted & Received %d packets\n\n", totalTxCnt);
    }
    else
    {
        UART_printf("Failed to transmit/receive packets: %d, transmitted: %d \n",
                    ENETLPBK_TEST_PKT_NUM, totalTxCnt);
    }

    return status;
}

/**
 * \brief   This function is used to perform the ENET
 *          Ethernet external loopback test
 *
 * \param   NULL
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int32_t BoardDiag_enetLoopbackTest(void)
{
    EnetUtils_Cfg utilsCfg;
    Enet_IoctlPrms prms;
    int32_t status;

    EnetAppUtils_enableClocks(gEnetLpbk.enetType, gEnetLpbk.instId);

    /* Local core id */
    gEnetLpbk.coreId = EnetSoc_getCoreId();

    /* Initialize Enet driver (use default utils) */
    Enet_initUtilsCfg(&utilsCfg);
    Enet_init(&utilsCfg);

    /* Open Enet driver */
    status = BoardDiag_enetOpenEnet();
    if (status != ENET_SOK)
    {
        UART_printf("Failed to open Enet driver: %d\n", status);
    }

    /* Open DMA driver */
    if (status == ENET_SOK)
    {
        status = BoardDiag_enetOpenDma();
        if (status != ENET_SOK)
        {
            UART_printf("Failed to open DMA: %d\n", status);
        }
    }

    /* Enable host port */
    if (status == ENET_SOK)
    {
        if (Enet_isCpswFamily(gEnetLpbk.enetType))
        {
            status = BoardDiag_enetSetupCpswAle();
            if (status != ENET_SOK)
            {
                UART_printf("Failed to setup CPSW ALE: %d\n", status);
            }
        }

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_NO_ARGS(&prms);
            status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, ENET_HOSTPORT_IOCTL_ENABLE, &prms);
            if (status != ENET_SOK)
            {
                UART_printf("Failed to enable host port: %d\n", status);
            }
        }
    }

    /* Show alive PHYs */
    if (status == ENET_SOK)
    {
        status = BoardDiag_enetShowAlivePhys();
    }


    /* Wait for link up */
    if ((status == ENET_SOK))
    {
        status = BoardDiag_enetWaitForLinkUp();
    }

    /* Do packet transmission and reception */
    if (status == ENET_SOK)
    {
        BoardDiag_enetPktRxTx();

    }

    /* Disable host port */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, ENET_HOSTPORT_IOCTL_DISABLE, &prms);
        if (status != ENET_SOK)
        {
            UART_printf("Failed to disable host port: %d\n", status);
        }
    }

    /* Close Enet DMA driver */
    BoardDiag_enetCloseDma();

    /* Close Enet driver */
    BoardDiag_enetCloseEnet();

    /* Disable peripheral clocks */
    EnetAppUtils_disableClocks(gEnetLpbk.enetType, gEnetLpbk.instId);

    /* Deinit Enet driver */
    Enet_deinit();

    return status;
}

/**
 * \brief   This function is used to initialize cpsw configurations.
 *
 * \param   cpswCfg           cpsw configurations
 *
 * \return  None
 *
 */
void BoardDiag_enetInitCpswCfg(Cpsw_Cfg *cpswCfg)
{
    CpswHostPort_Cfg *hostPortCfg = &cpswCfg->hostPortCfg;
    CpswAle_Cfg *aleCfg = &cpswCfg->aleCfg;
    CpswCpts_Cfg *cptsCfg = &cpswCfg->cptsCfg;

    /* Set initial config */
    Enet_initCfg(gEnetLpbk.enetType, gEnetLpbk.instId, cpswCfg, sizeof(*cpswCfg));

    /* Peripheral config */
    cpswCfg->vlanCfg.vlanAware = UFALSE;

    /* Host port config */
    hostPortCfg->removeCrc      = BTRUE;
    hostPortCfg->padShortPacket = BTRUE;
    hostPortCfg->passCrcErrors  = BTRUE;

    /* ALE config */
    aleCfg->modeFlags                          = CPSW_ALE_CFG_MODULE_EN;
    aleCfg->agingCfg.autoAgingEn               = BTRUE;
    aleCfg->agingCfg.agingPeriodInMs           = 1000;
    aleCfg->nwSecCfg.vid0ModeEn                = BTRUE;
    aleCfg->vlanCfg.aleVlanAwareMode           = BFALSE;
    aleCfg->vlanCfg.cpswVlanAwareMode          = BFALSE;
    aleCfg->vlanCfg.unknownUnregMcastFloodMask = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownRegMcastFloodMask   = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownVlanMemberListMask  = CPSW_ALE_ALL_PORTS_MASK;

    /* CPTS config */
    /* Note: Timestamping and MAC loopback are not supported together because of
     * IP limitation, so disabling timestamping for this application */
    cptsCfg->hostRxTsEn = BFALSE;

    EnetAppUtils_initResourceConfig(gEnetLpbk.enetType, gEnetLpbk.instId, gEnetLpbk.coreId, &cpswCfg->resCfg);
}

/**
 * \brief   This function is used to set the ALE port state to forward.
 *
 * \param   NULL
 *
 * \return  int32_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int32_t BoardDiag_enetSetupCpswAle(void)
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
    EnetUtils_copyMacAddr(&setUcastInArgs.addr.addr[0U], gEnetLpbk.hostMacAddr0);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &entryIdx);

    status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, CPSW_ALE_IOCTL_ADD_UCAST, &prms);
    if (ENET_SOK != status)
    {
        UART_printf("Failed to add ucast entry: %d\n", status);
    }

    /* Set host port to 'forwarding' state */
    if (ENET_SOK == status)
    {
        setPortStateInArgs.portNum   = CPSW_ALE_HOST_PORT_NUM;
        setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_FORWARD;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);

        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        if (ENET_SOK != status)
        {
            UART_printf("Failed to set ALE port state: %d\n", status);
        }
    }

    return status;
}

/**
 * \brief   This function is used to open the ENET driver
 *
 * \param   NULL
 *
 * \return  int32_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int32_t BoardDiag_enetOpenEnet(void)
{
    Cpsw_Cfg cpswCfg;
    EnetDma_Cfg dmaCfg;
    Enet_IoctlPrms prms;
    emac_mode macMode;

    EnetPer_PortLinkCfg port0LinkCfg;
    EnetPer_PortLinkCfg port1LinkCfg;

    int32_t status = ENET_SOK;

    cpswCfg.dmaCfg = &dmaCfg;

    /* Initialize peripheral config */
    BoardDiag_enetInitCpswCfg(&cpswCfg);

    dmaCfg.rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;

    /* App should open UDMA first as UDMA handle is needed to initialize
     * CPSW RX channel */
    gEnetLpbk.hUdmaDrv = EnetAppUtils_udmaOpen(gEnetLpbk.enetType, NULL);
    EnetAppUtils_assert(NULL != gEnetLpbk.hUdmaDrv);

    dmaCfg.hUdmaDrv = gEnetLpbk.hUdmaDrv;

    /* Open the Enet driver */
    gEnetLpbk.hEnet = Enet_open(gEnetLpbk.enetType, gEnetLpbk.instId, &cpswCfg, sizeof(cpswCfg));
    if (NULL == gEnetLpbk.hEnet)
    {
        UART_printf("Failed to open Enet driver\n");
        status = ENET_EFAIL;
    }

    if (ENET_SOK == status)
    {
        /* Attach the core with RM */
        uint32_t coreId;
        EnetPer_AttachCoreOutArgs attachCoreOutArgs;
        coreId = gEnetLpbk.coreId;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &coreId, &attachCoreOutArgs);
        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, ENET_PER_IOCTL_ATTACH_CORE, &prms);
        if (ENET_SOK != status)
        {
            UART_printf("EnetLpbk_loopbackTest failed ENET_PER_IOCTL_ATTACH_CORE: %d\n", status);
        }
        else
        {
            gEnetLpbk.coreKey = attachCoreOutArgs.coreKey;
        }
    }

    if (ENET_SOK == status)
    {
        /* memutils open should happen after Cpsw is opened as it uses CpswUtils_Q
         * functions */
        status = EnetMem_init();
        if (ENET_SOK != status)
        {
            UART_printf("Failed to open memutils %d\n", status);
            return ENET_EFAIL;
        }
    }

    /* Setup port link open parameters */
    if (ENET_SOK == status)
    {
        CpswMacPort_Cfg macCfg;
        EnetPhy_Mii phyMii;

         /* Configuration of First port */
        EnetBoard_EthPort ethPort0;
        
        EnetMacPort_LinkCfg *linkCfg = &port0LinkCfg.linkCfg;
        EnetMacPort_Interface *mii = &port0LinkCfg.mii;
        EnetPhy_Cfg *phyCfg = &port0LinkCfg.phyCfg;

        /* Setup board for requested Ethernet port */
        ethPort0.macPort  = gEnetLpbk.macPort0;
        ethPort0.boardId  = gEnetLpbk.boardId;
        ethPort0.expPort  = gEnetLpbk.expPort;
        if((ENET_MAC_PORT_1 == gEnetLpbk.macPort0) || (ENET_MAC_PORT_2 == gEnetLpbk.macPort0))
        {
            /* ENET_MAC_PORT_1 and ENET_MAC_PORT_2 are considered as MAIN Ports */
            macMode = QSGMII;
        }
        else
        {
           macMode = QSGMII_SUB;
        }

        BoardDiag_enetMacMode2MacMii(macMode, &ethPort0.mii);

        status = EnetBoard_setupPorts(gEnetLpbk.enetType, gEnetLpbk.instId, &ethPort0, 1U);
        if (ENET_SOK != status)
        {
            UART_printf("Failed to setup ports: %d\n", status);
            return ENET_EFAIL;
        }

        port0LinkCfg.macPort = gEnetLpbk.macPort0;
        port0LinkCfg.macCfg = &macCfg;

        CpswMacPort_initCfg(&macCfg);
        BoardDiag_enetMacMode2MacMii(macMode, mii);

        const EnetBoard_PortCfg *board0PhyCfg = NULL;

        /* Set PHY configuration params */
        EnetPhy_initCfg(phyCfg);
        status = BoardDiag_enetMacMode2PhyMii(gEnetLpbk.macMode, &phyMii);
        
         board0PhyCfg = EnetBoard_getPortCfg(gEnetLpbk.enetType, gEnetLpbk.instId, &ethPort0);
        if (NULL != board0PhyCfg)
        {
            phyCfg->phyAddr     = board0PhyCfg->phyCfg.phyAddr;
            phyCfg->isStrapped  = board0PhyCfg->phyCfg.isStrapped;
            phyCfg->skipExtendedCfg = board0PhyCfg->phyCfg.skipExtendedCfg;
            phyCfg->extendedCfgSize = board0PhyCfg->phyCfg.extendedCfgSize;
            memcpy(phyCfg->extendedCfg, board0PhyCfg->phyCfg.extendedCfg, phyCfg->extendedCfgSize);
        }
        else
        {
            UART_printf("PHY info not found\n");
            return ENET_EFAIL;
        }
 
        linkCfg->speed = ENET_SPEED_AUTO;
        linkCfg->duplexity = ENET_DUPLEX_AUTO;
        phyCfg->loopbackEn = BFALSE;
        macCfg.loopbackEn = BFALSE;
        
        if (Enet_isCpswFamily(gEnetLpbk.enetType))
        {
                macCfg.sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_WITH_PHY;
        }
         /* Open port link */
        if (ENET_SOK == status)
        {
            ENET_IOCTL_SET_IN_ARGS(&prms, &port0LinkCfg);
        
            status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, ENET_PER_IOCTL_OPEN_PORT_LINK, &prms);
            if (ENET_SOK != status)
            {
                UART_printf("Failed to open port link: %d\n", status);
            }
        }

        /* Configuration of second port */
        EnetBoard_EthPort ethPort1;
        
        linkCfg = &port1LinkCfg.linkCfg;
        mii = &port1LinkCfg.mii;
        phyCfg = &port1LinkCfg.phyCfg;
        
        
        /* Setup board for requested Ethernet port */
        ethPort1.macPort  = gEnetLpbk.macPort1;
        ethPort1.boardId  = gEnetLpbk.boardId;
        ethPort1.expPort  = gEnetLpbk.expPort;
        if((ENET_MAC_PORT_1 == gEnetLpbk.macPort1) || (ENET_MAC_PORT_2 == gEnetLpbk.macPort1))
        {
            /* ENET_MAC_PORT_1 and ENET_MAC_PORT_2 are considered as MAIN Ports */
            macMode = QSGMII;
        }
        else
        {
           macMode = QSGMII_SUB;
        }
        BoardDiag_enetMacMode2MacMii(macMode, &ethPort1.mii);
        
        status = EnetBoard_setupPorts(gEnetLpbk.enetType, gEnetLpbk.instId, &ethPort1, 1U);
        if (ENET_SOK != status)
        {
            UART_printf("Failed to setup ports: %d\n", status);
            return ENET_EFAIL;
        }
        
        port1LinkCfg.macPort = gEnetLpbk.macPort1;
        port1LinkCfg.macCfg = &macCfg;
       
        CpswMacPort_initCfg(&macCfg);
        BoardDiag_enetMacMode2MacMii(macMode, mii);
        
        const EnetBoard_PortCfg *board1PhyCfg = NULL;
        /* Set PHY configuration params */
        EnetPhy_initCfg(phyCfg);
        status = BoardDiag_enetMacMode2PhyMii(macMode, &phyMii);
        
         board1PhyCfg = EnetBoard_getPortCfg(gEnetLpbk.enetType, gEnetLpbk.instId, &ethPort1);
         if (NULL != board1PhyCfg)
        {
            phyCfg->phyAddr     = board1PhyCfg->phyCfg.phyAddr;
            phyCfg->isStrapped  = board1PhyCfg->phyCfg.isStrapped;
            phyCfg->skipExtendedCfg = board1PhyCfg->phyCfg.skipExtendedCfg;
            phyCfg->extendedCfgSize = board1PhyCfg->phyCfg.extendedCfgSize;
            memcpy(phyCfg->extendedCfg, board1PhyCfg->phyCfg.extendedCfg, phyCfg->extendedCfgSize);
        }
        else
        {
            UART_printf("PHY info not found\n");
            return ENET_EFAIL;
        }        
        
        linkCfg->speed = ENET_SPEED_AUTO;
        linkCfg->duplexity = ENET_DUPLEX_AUTO;
        phyCfg->loopbackEn = BFALSE;
        macCfg.loopbackEn = BFALSE;

        if (Enet_isCpswFamily(gEnetLpbk.enetType))
        {
                macCfg.sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_WITH_PHY;
        }
        /* Open port link */
        if (ENET_SOK == status)
        {
            ENET_IOCTL_SET_IN_ARGS(&prms, &port1LinkCfg);
    
            status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, ENET_PER_IOCTL_OPEN_PORT_LINK, &prms);
            if (ENET_SOK != status)
            {
                UART_printf("Failed to open port link: %d\n", status);
            }
            
        }
    
    }
    
    return status;
}

/**
 * \brief   This function is used for close the ENET module.
 *
 * \param   NULL
 *
 * \return  None
 */
void BoardDiag_enetCloseEnet(void)
{
    Enet_IoctlPrms prms;
    int32_t status;
    
    /* Detach core */

    ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetLpbk.coreKey);

    status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, ENET_PER_IOCTL_DETACH_CORE, &prms);
    if (ENET_SOK != status)
    {
        UART_printf("Failed to detach core1 key %u: %d\n", gEnetLpbk.coreKey, status);
    }

    if (ENET_SOK == status)
    {
            /* Close port link */
        ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetLpbk.macPort0);
        
        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
        if (ENET_SOK != status)
        {
            UART_printf("Failed to close port link: %d\n", status);
        }
    }
    
     if (ENET_SOK == status)
    {
            /* Close port link */
        ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetLpbk.macPort1);
        
        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
        if (ENET_SOK != status)
        {
            UART_printf("Failed to close port link: %d\n", status);
        }
    }
    
    /* Close Enet driver */
    Enet_close(gEnetLpbk.hEnet);

    /* Close UDMA */
    EnetAppUtils_udmaclose(gEnetLpbk.hUdmaDrv);

    gEnetLpbk.hEnet = NULL;
}

/**
 * \brief   This function checks the live status of the phy
 *
 * \param   NULL
 *
 * \return  int32_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int32_t BoardDiag_enetShowAlivePhys(void)
{
    Enet_IoctlPrms prms;
    bool alive = BFALSE;
    int8_t i;
    int32_t status;

    for (i = 0U; i < ENET_MDIO_PHY_CNT_MAX; i++)
    {
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &i, &alive);

        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId, ENET_MDIO_IOCTL_IS_ALIVE, &prms);
        if (ENET_SOK == status)
        {
            if (BTRUE == alive)
            {
                UART_printf("PHY %u is alive\n", i);
            }
        }
        else
        {
            UART_printf("Failed to get PHY %u alive status: %d\n", i, status);
        }
    }

    return status;
}

void BoardDiag_enetWait(uint32_t waitTime)
{
    volatile uint32_t index;

    /* we multiply waitTime by 10000 as 400MHz R5 takes 2.5ns for single cycle
     * and we assume for loop takes 4 cycles */
    for (index = 0U; index < (waitTime*1000); index++);
}

/**
 * \brief   This function Waits till phy link is up.
 *
 * \param   NULL
 *
 * \return  int32_t
 *               0 - in case of phy link is up
 *              -1 - in case of phy link is down.
 *
 */
int32_t BoardDiag_enetWaitForLinkUp(void)
{
    int32_t status = ENET_SOK;

   UART_printf("Waiting for link to up for portNum-%d and portNum-%d...\n\r",
                 gEnetLpbk.macPort0+1,
                     gEnetLpbk.macPort1+1);
    
    while (!EnetAppUtils_isPortLinkUp(gEnetLpbk.hEnet, gEnetLpbk.coreId, gEnetLpbk.macPort0))
        {
            BoardDiag_enetWait(1000);
            Enet_periodicTick(gEnetLpbk.hEnet);
        }
     while (!EnetAppUtils_isPortLinkUp(gEnetLpbk.hEnet, gEnetLpbk.coreId, gEnetLpbk.macPort1))
        {
            BoardDiag_enetWait(1000);
            Enet_periodicTick(gEnetLpbk.hEnet);
        }
    
     UART_printf("Link up for portNum-%d and portNum-%d\n\r",
                     gEnetLpbk.macPort0+1,
                     gEnetLpbk.macPort1+1);
                     
    return status;
}

/**
 * \brief   This function is used to initialize the free packet
 *          info queue with the Ethernet packets to be transmitted.
 *
 * \param   NULL
 *
 * \return  None
 *
 */
void BoardDiag_enetInitTxFreePktQ(void)
{
    EnetDma_Pkt *pPktInfo;
    uint32_t i;

    /* Initialize all queues */
    EnetQueue_initQ(&gEnetLpbk.txFreePktInfoQ);

    /* Initialize TX EthPkts and queue them to txFreePktInfoQ */
    for (i = 0U; i < ENET_MEM_NUM_TX_PKTS; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&gEnetLpbk,
                                       ENETDMA_CACHELINE_ALIGNMENT,
                                       ENET_ARRAYSIZE(txScatterSegments),
                                       txScatterSegments);
        EnetAppUtils_assert(NULL != pPktInfo);
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);

        EnetQueue_enq(&gEnetLpbk.txFreePktInfoQ, &pPktInfo->node);
    }

    UART_printf("initQs() txFreePktInfoQ initialized with %d pkts\n",
                       EnetQueue_getQCount(&gEnetLpbk.txFreePktInfoQ));
}

/**
 * \brief   This function is used to initialize the receive ready packet queue
 *
 * \param   NULL
 *
 * \return  None
 *
 */
void BoardDiag_enetInitRxReadyPktQ(void)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pPktInfo;
    int32_t status;
    uint32_t i;

    EnetQueue_initQ(&gEnetLpbk.rxFreeQ);
    EnetQueue_initQ(&gEnetLpbk.rxReadyQ);
    EnetQueue_initQ(&rxReadyQ);

    for (i = 0U; i < ENET_MEM_NUM_RX_PKTS; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&gEnetLpbk,
                                       ENETDMA_CACHELINE_ALIGNMENT,
                                       ENET_ARRAYSIZE(rxScatterSegments),
                                       rxScatterSegments);
        EnetAppUtils_assert(NULL != pPktInfo);
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);
        EnetQueue_enq(&gEnetLpbk.rxFreeQ, &pPktInfo->node);
    }

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(gEnetLpbk.hRxCh, &rxReadyQ);
    EnetAppUtils_assert(ENET_SOK == status);
    /* There should not be any packet with DMA during init */
    EnetAppUtils_assert(0U == EnetQueue_getQCount(&rxReadyQ));

    EnetAppUtils_validatePacketState(&gEnetLpbk.rxFreeQ,
                                     ENET_PKTSTATE_APP_WITH_FREEQ,
                                     ENET_PKTSTATE_APP_WITH_DRIVER);

    EnetDma_submitRxPktQ(gEnetLpbk.hRxCh,
                         &gEnetLpbk.rxFreeQ);

    /* Assert here as during init no. of DMA descriptors should be equal to
     * no. of free Ethernet buffers available with app */

    EnetAppUtils_assert(0U == EnetQueue_getQCount(&gEnetLpbk.rxFreeQ));
}

/**
 * \brief   This function is used to open the CPSW DMA module
 *
 * \param   NULL
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int32_t BoardDiag_enetOpenDma(void)
{
    int32_t status = ENET_SOK;
    EnetUdma_OpenRxFlowPrms rxChCfg;
    EnetUdma_OpenTxChPrms   txChCfg;

    /* Open the CPSW TX channel  */
    if (ENET_SOK == status)
    {
        EnetUdma_initTxChParams(&txChCfg);

        txChCfg.hUdmaDrv = gEnetLpbk.hUdmaDrv;
        txChCfg.cbArg    = &gEnetLpbk;
        txChCfg.notifyCb = BoardDiag_enetTxIsrFxn;

        EnetAppUtils_setCommonTxChPrms(&txChCfg);

        EnetAppUtils_openTxCh(gEnetLpbk.hEnet,
                              gEnetLpbk.coreKey,
                              gEnetLpbk.coreId,
                              &gEnetLpbk.txChNum,
                              &gEnetLpbk.hTxCh,
                              &txChCfg);

        BoardDiag_enetInitTxFreePktQ();

        if (NULL != gEnetLpbk.hTxCh)
        {
            status = EnetDma_enableTxEvent(gEnetLpbk.hTxCh);
            if (ENET_SOK != status)
            {

                /* Free the Ch Num if enable event failed */
                EnetAppUtils_freeTxCh(gEnetLpbk.hEnet,
                                      gEnetLpbk.coreKey,
                                      gEnetLpbk.coreId,
                                      gEnetLpbk.txChNum);
                UART_printf("EnetUdma_startTxCh() failed: %d\n", status);
                status = ENET_EFAIL;
            }
        }
        else
        {
            /* Free the Ch Num if open Tx Ch failed */
            EnetAppUtils_freeTxCh(gEnetLpbk.hEnet,
                                  gEnetLpbk.coreKey,
                                  gEnetLpbk.coreId,
                                  gEnetLpbk.txChNum);
            UART_printf("EnetUdma_openTxCh() failed to open: %d\n",
                               status);
            status = ENET_EFAIL;
        }
    }

    /* Open the CPSW RX flow  */
    if (ENET_SOK == status)
    {
        EnetUdma_initRxFlowParams(&rxChCfg);

        rxChCfg.hUdmaDrv = gEnetLpbk.hUdmaDrv;
        rxChCfg.notifyCb = BoardDiag_enetRxIsrFxn;
        rxChCfg.cbArg   = &gEnetLpbk;

        EnetAppUtils_setCommonRxFlowPrms(&rxChCfg);
        EnetAppUtils_openRxFlow(gEnetLpbk.enetType,
                                gEnetLpbk.hEnet,
                                gEnetLpbk.coreKey,
                                gEnetLpbk.coreId,
                                BTRUE,
                                &gEnetLpbk.rxStartFlowIdx,
                                &gEnetLpbk.rxFlowIdx,
                                &gEnetLpbk.hostMacAddr0[0U],
                                &gEnetLpbk.hRxCh,
                                &rxChCfg);
        if (NULL == gEnetLpbk.hRxCh)
        {
            UART_printf("EnetUdma_openRxFlow() failed to open: %d\n",
                               status);
            EnetAppUtils_assert(NULL != gEnetLpbk.hRxCh);
        }
        else
        {
            UART_printf("Host MAC address: ");
            EnetAppUtils_printMacAddr(gEnetLpbk.hostMacAddr0);
             
            /* Submit all ready RX buffers to DMA.*/
            BoardDiag_enetInitRxReadyPktQ();
        }
    }

    return status;
}

/**
 * \brief   This function is used for de-initialize the ENET DMA driver.
 *
 * \param   NULL
 *
 * \return  None
 */
void BoardDiag_enetCloseDma(void)
{
    EnetDma_PktQ fqPktInfoQ;
    EnetDma_PktQ cqPktInfoQ;

    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    /* There should not be any ready packet */
    EnetAppUtils_assert(0U == EnetQueue_getQCount(&gEnetLpbk.rxReadyQ));

    /* Close RX channel */
    EnetAppUtils_closeRxFlow(gEnetLpbk.enetType,
                             gEnetLpbk.hEnet,
                             gEnetLpbk.coreKey,
                             gEnetLpbk.coreId,
                             BTRUE,
                             &fqPktInfoQ,
                             &cqPktInfoQ,
                             gEnetLpbk.rxStartFlowIdx,
                             gEnetLpbk.rxFlowIdx,
                             gEnetLpbk.hostMacAddr0,
                             gEnetLpbk.hRxCh);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    /* Close TX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    EnetAppUtils_closeTxCh(gEnetLpbk.hEnet,
                           gEnetLpbk.coreKey,
                           gEnetLpbk.coreId,
                           &fqPktInfoQ,
                           &cqPktInfoQ,
                           gEnetLpbk.hTxCh,
                           gEnetLpbk.txChNum);
    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    EnetAppUtils_freePktInfoQ(&gEnetLpbk.rxFreeQ);
    EnetAppUtils_freePktInfoQ(&gEnetLpbk.txFreePktInfoQ);

    EnetMem_deInit();
}


int32_t BoardDiag_enetMacMode2PhyMii(emac_mode macMode,
                                EnetPhy_Mii *mii)
{
    int32_t status = ENET_SOK;

    switch (macMode)
    {
        case RMII:
            *mii = ENETPHY_MAC_MII_RMII;
            break;

        case RGMII:
            *mii = ENETPHY_MAC_MII_RGMII;
            break;

        case GMII:
            *mii = ENETPHY_MAC_MII_GMII;
            break;

        case SGMII:
            *mii = ENETPHY_MAC_MII_SGMII;
            break;

        case QSGMII:
        case QSGMII_SUB:
            *mii = ENETPHY_MAC_MII_QSGMII;
            break;

        case XFI:
            status = ENET_EFAIL;
            UART_printf("XFI is not supported by EnetPhy driver\n");
            EnetAppUtils_assert(BFALSE);
            break;

        default:
            status = ENET_EFAIL;
            UART_printf("Invalid MAC mode: %u\n", macMode);
            EnetAppUtils_assert(BFALSE);
            break;
    }

    return status;
}

void BoardDiag_enetMacMode2MacMii(emac_mode macMode,
                             EnetMacPort_Interface *mii)
{
    switch (macMode)
    {
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
            mii->sublayerType = ENET_MAC_SUBLAYER_SERIAL;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        default:
            UART_printf("Invalid MAC mode: %u\n", macMode);
            EnetAppUtils_assert(BFALSE);
            break;
    }
}

/**
 * \brief  ENET diagnostic test main function
 *
 *  This function performs board initializations and calls ENET ethernet test
 *
 * \return  int
 *              0  - in case of success
 *             -1  - in case of failure
 *
 */
int main(void)
{

    Board_initCfg boardCfg;
    Board_STATUS status;
    int8_t ret = 0;
    
    memset(&gEnetLpbk, 0, sizeof(gEnetLpbk));

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_ENETCTRL_CPSW9G |
               BOARD_INIT_UART_STDIO;

    status = Board_init(boardCfg);
    if(BOARD_SOK != status)
    {
        return -1;
    }

    BoardDiag_enetGetTestConfig(&gEnetLpbk.enetType,
                          &gEnetLpbk.instId,
                          &gEnetLpbk.macPort0,
                          &gEnetLpbk.macPort1,
                          &gEnetLpbk.mii,
                          &gEnetLpbk.boardId,
                          &gEnetLpbk.expPort);


   /* Run the loopback test */
    status = BoardDiag_enetLoopbackTest();

    if (ENET_SOK == status)
    {
        UART_printf("QSGMII Port-Port Loopback completed\n");
        UART_printf("\nAll tests have passed\n\r");
    }
    else
    {
        UART_printf("QSGMII Port-Port Loopback failed \n");
        return -1;
    }
    
    return status;
}

