/******************************************************************************
* Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the
* distribution.
*
* Neither the name of Texas Instruments Incorporated nor the names of
* its contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

/**
 *  \file   cpsw_eth_test.c
 *
 *  \brief  CPSW ethernet diagnostic test file
 *
 *  Targeted Functionality: Verification of basic functionality of
 *  CPSW Ethernet interface.
 *
 *  Operation: This is the Loopback test code for the Ethernet Interface.
 *  The test code showcases usage of the CPSW Driver exported API for
 *  sending/receiving Ethernet packets.
 *
 *  Supported SoCs: TPR12, AWR294x
 *
 *  Supported Platforms:tpr12_evm, awr294x_evm.
 *
 */

/* Test application local header file */
#include "cpsw_eth_test.h"

/* Trasnmitt and receive status variables */
static volatile bool gTxSem = BFALSE;
static volatile bool gRxSem = BFALSE;

/* Enet loopback test object */
static BoardDiag_EnetLpbkObj_t gEnetLpbk;

/**
 * \brief   ENET receive ISR function
 *
 * \param   void *appData   application data
 *
 * \return  NULL
 */
static void BoardDiag_enetLpbkRxIsrFxn(void *appData)
{
    gRxSem = BTRUE;
}

/**
 * \brief   ENET transmit ISR function
 *
 * \param   void *appData   application data
 *
 * \return  NULL
 */
static void BoardDiag_enetLpbkTxIsrFxn(void *appData)
{
    gTxSem = BTRUE;
}

/**
 * \brief   This function is used for close the ENET module.
 *
 * \param   NULL
 *
 * \return  NULL
 */
static void BoardDiag_enetLpbkCloseEnet(void)
{
    Enet_IoctlPrms prms;
    int32_t status;

    /* Close port link */
    ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetLpbk.macPort);

    status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                        ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
    if (status != ENET_SOK)
    {
        UART_printf("Failed to close port link: %d\n", status);
    }

#if !(defined(SOC_TPR12) || defined(SOC_AWR294X))
    /* Detach core */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetLpbk.coreKey);

        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                            ENET_PER_IOCTL_DETACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            UART_printf("Failed to detach core key %u: %d\n", gEnetLpbk.coreKey, status);
        }
    }
#endif
    /* Close Enet driver */
    Enet_close(gEnetLpbk.hEnet);

    gEnetLpbk.hEnet = NULL;
}

/**
 * \brief   This function is used for de-initialize the ENET DMA driver.
 *
 * \param   NULL
 *
 * \return  NULL
 */
static void BoardDiag_enetLpbkCloseDma()
{
    EnetDma_PktQ fqPktInfoQ;
    EnetDma_PktQ cqPktInfoQ;

    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    UART_printf("Closing DMA\n");
    /* There should not be any ready packet */
    if(EnetQueue_getQCount(&gEnetLpbk.rxReadyQ) != 0)
    {
        UART_printf("EnetQueue_getQCount Failed\n");
        return;
    }

    /* Close RX Flow */
    EnetAppUtils_closeRxCh(gEnetLpbk.hEnet,
                           gEnetLpbk.coreKey,
                           gEnetLpbk.coreId,
                           &fqPktInfoQ,
                           &cqPktInfoQ,
                           gEnetLpbk.hRxCh,
                           gEnetLpbk.rxChNum);
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

/**
 * \brief   This function is used to Mac port statistics
 *
 * \param   CpswStats_HostPort_2g *st   Cpsw stats
 *
 * \return  NULL
 */
static void BoardDiag_printMacPortStats2G(CpswStats_MacPort_2g *st)
{
    uint32_t i;

    UART_printf("rxGoodFrames            = %d\n", (uint32_t)st->rxGoodFrames);
    UART_printf("rxBcastFrames           = %d\n", (uint32_t)st->rxBcastFrames);
    UART_printf("rxOctets                = %d\n", (uint32_t)st->rxOctets);
    UART_printf("txGoodFrames            = %d\n", (uint32_t)st->txGoodFrames);
    UART_printf("txBcastFrames           = %d\n", (uint32_t)st->txBcastFrames);
    UART_printf("txOctets                = %d\n", (uint32_t)st->txOctets);
    UART_printf("octetsFrames512to1023   = %d\n",
                (uint32_t)st->octetsFrames512to1023);

    for (i = 0U; i < ENET_ARRAYSIZE((uint32_t)st->txPri); i++)
    {
        UART_printf("txPri[%u]                = %d\n", i,
                    (uint32_t)st->txPri[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE((uint32_t)st->txPriBcnt); i++)
    {
        UART_printf("txPriBcnt[%u]            = %d\n", i,
                    (uint32_t)st->txPriBcnt[i]);
    }
}

/**
 * \brief   This function is used to Host port statistics
 *
 * \param   CpswStats_HostPort_2g *st   Cpsw stats
 *
 * \return  NULL
 */
static void BoardDiag_printHostPortStats2G(CpswStats_HostPort_2g *st)
{
    UART_printf("rxGoodFrames            = %d\n", (uint32_t)st->rxGoodFrames);
    UART_printf("rxBcastFrames           = %u\n", (uint32_t)st->rxBcastFrames);
    UART_printf("rxOctets                = %d\n", (uint32_t)st->rxOctets);
    UART_printf("txGoodFrames            = %d\n", (uint32_t)st->txGoodFrames);
    UART_printf("txBcastFrames           = %d\n", (uint32_t)st->txBcastFrames);
    UART_printf("txOctets                = %d\n", (uint32_t)st->txOctets);
    UART_printf("octetsFrames512to1023   = %d\n",
                (uint32_t)st->octetsFrames512to1023);
}

/**
 * \brief   This function is used to display Mac and Host port statistics
 *
 * \param   NULL
 *
 * \return  NULL
 *
 */
static void BoardDiag_enetLpbkShowCpswStats(void)
{
    Enet_IoctlPrms prms;
    CpswStats_PortStats portStats;
    int32_t status;

    /* Show host port statistics */
    ENET_IOCTL_SET_OUT_ARGS(&prms, &portStats);
    status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                        ENET_STATS_IOCTL_GET_HOSTPORT_STATS, &prms);
    if (status == ENET_SOK)
    {
        UART_printf("\n Port 0 Statistics\n");
        UART_printf("-----------------------------------------\n");
        BoardDiag_printHostPortStats2G((CpswStats_HostPort_2g *)&portStats);
        UART_printf("\n");
    }
    else
    {
        UART_printf("Failed to get host stats: %d\n", status);
    }

    /* Show MAC port statistics */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &gEnetLpbk.macPort, &portStats);
        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                            ENET_STATS_IOCTL_GET_MACPORT_STATS, &prms);
        if (status == ENET_SOK)
        {
            UART_printf("\n Port 1 Statistics\n");
            UART_printf("-----------------------------------------\n");
            BoardDiag_printMacPortStats2G((CpswStats_MacPort_2g *)&portStats);
            UART_printf("\n");
        }
        else
        {
            UART_printf("Failed to get MAC stats: %d\n", status);
        }
    }
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
    UART_printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                       macAddr[0] & 0xFF,
                       macAddr[1] & 0xFF,
                       macAddr[2] & 0xFF,
                       macAddr[3] & 0xFF,
                       macAddr[4] & 0xFF,
                       macAddr[5] & 0xFF);
}

/**
 * \brief   This function is used to display Mac address
 *
 * \param   EthFrame EthFrame[] frame to be displayed
 *          uint32_t len        length of the frame
 * \return  NULL
 *
 */
static void BoardDiag_printFrame(EthFrame *frame,
                                uint32_t len)
{
    uint8_t *payload;
    uint32_t i;

    UART_printf("Dst addr : ");
    BoardDiag_printMacAddr(&frame->hdr.dstMac[0]);

    UART_printf("Src addr : ");
    BoardDiag_printMacAddr(&frame->hdr.srcMac[0]);

    if (frame->hdr.etherType == Enet_htons(ETHERTYPE_VLAN_TAG))
    {
        EthVlanFrame *vlanFrame = (EthVlanFrame *)frame;

        UART_printf("TPID     : 0x%04x\n", 
                    Enet_ntohs(vlanFrame->hdr.tpid) & 0xFFFFU);
        UART_printf("Priority : %d\n",
                    (Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFFU) >> 13);
        UART_printf("VLAN Id  : %d\n",
                    Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFU);
        UART_printf("EtherType: 0x%04x\n",
                    Enet_ntohs(vlanFrame->hdr.etherType) & 0xFFFFU);
        payload = vlanFrame->payload;
        len    -= ETH_VLAN_TAG_LEN;
    }
    else
    {
        UART_printf("EtherType: 0x%04x\n", 
                    Enet_ntohs(frame->hdr.etherType) & 0xFFFFU);
        payload = frame->payload;
    }

    UART_printf("Payload  : ");
    for (i = 0; i < len; i++)
    {
        UART_printf("0x%02x ", payload[i]);
        if (i && (((i + 1) % OCTETS_PER_ROW) == 0))
        {
            UART_printf("\n           ");
        }
    }

    if (len && ((len % OCTETS_PER_ROW) != 0))
    {
        UART_printf("\n");
    }

    UART_printf("\n");
}

/**
 * \brief   This function is used to queue the received packets to rx ready queue
 *
 * \param   NULL
 *
 * \return  uint32_t - Receive ready queue count
 *
 */
static uint32_t BoardDiag_enetLpbkReceivePkts(void)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pktInfo;
    int32_t status;
    uint32_t rxReadyCnt = 0U;

    EnetQueue_initQ(&rxReadyQ);

    /* Retrieve any CPSW packets which are ready */
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

/**
 * \brief   This function is used to retrieve's any CPSW packets that may be free now
 *
 * \param   NULL
 *
 * \return  uint32_t - Transmit free queue count
 *
 */
static uint32_t BoardDiag_enetLpbkRetrieveFreeTxPkts(void)
{
    EnetDma_PktQ txFreeQ;
    EnetDma_Pkt *pktInfo;
    int32_t status;
    uint32_t txFreeQCnt = 0U;

    EnetQueue_initQ(&txFreeQ);

    /* Retrieve any CPSW packets that may be free now */
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

/**
 * \brief   This function is used for CPSW packet transmission
 *          and reception
 *
 * \param   NULL
 *
 * \return  int32_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int32_t BoardDiag_cpswPktRxTx(void)
{
    EnetDma_PktQ txSubmitQ;
    EnetDma_Pkt *pktInfo;
    EthFrame *frame;
    uint32_t txRetrievePktCnt = 0;
    uint32_t loopTxPktCnt = 0;
    uint32_t loopRxPktCnt = 0;
    uint32_t loopCnt = 0, pktCnt = 0;
    uint32_t rxReadyCnt = 0;
    int32_t status = ENET_SOK;
    uint8_t bcastAddr[ENET_MAC_ADDR_LEN] = 
            {0xffU, 0xffU, 0xffU, 0xffU, 0xffU, 0xffU};

    gEnetLpbk.totalTxCnt = 0U;
    for (loopCnt = 0U; loopCnt < BOARD_DIAG_ENETLPBK_NUM_ITERATION; loopCnt++)
    {
        pktCnt = 0U;
        while (pktCnt < BOARD_DIAG_ENETLPBK_TEST_PKT_NUM)
        {
            loopRxPktCnt = loopTxPktCnt = 0U;
            /* Transmit a single packet */
            EnetQueue_initQ(&txSubmitQ);

            /* Dequeue one free TX Eth packet */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetLpbk.txFreePktInfoQ);

            while (NULL != pktInfo)
            {
                pktCnt++;
                /* Fill the TX Eth frame with test content */
                frame = (EthFrame *)pktInfo->bufPtr;
                memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
                memcpy(frame->hdr.srcMac, &gEnetLpbk.hostMacAddr[0U],
                        ENET_MAC_ADDR_LEN);
                frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
                memset(&frame->payload[0U], (uint8_t)(0xA5 + pktCnt),
                        BOARD_DIAG_ENETLPBK_TEST_PKT_LEN);
                pktInfo->userBufLen = BOARD_DIAG_ENETLPBK_TEST_PKT_LEN +
                                      sizeof(EthFrameHeader);
                pktInfo->appPriv    = &gEnetLpbk;
                EnetDma_checkPktState(&pktInfo->pktState,
                                        ENET_PKTSTATE_MODULE_APP,
                                        ENET_PKTSTATE_APP_WITH_FREEQ,
                                        ENET_PKTSTATE_APP_WITH_DRIVER);

                /* Enqueue the packet for later transmission */
                EnetQueue_enq(&txSubmitQ, &pktInfo->node);

                if (pktCnt >= BOARD_DIAG_ENETLPBK_TEST_PKT_NUM)
                {
                    break;
                }

                /* Dequeue one free TX Eth packet */
                pktInfo =
                (EnetDma_Pkt *)EnetQueue_deq(&gEnetLpbk.txFreePktInfoQ);
            }
            loopTxPktCnt = EnetQueue_getQCount(&txSubmitQ);
            while (0U != EnetQueue_getQCount(&txSubmitQ))
            {
                uint32_t txCnt = EnetQueue_getQCount(&txSubmitQ);
                status = EnetDma_submitTxPktQ(gEnetLpbk.hTxCh,
                                                   &txSubmitQ);
                while (BTRUE != gTxSem);
                gTxSem = BFALSE;

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
                        BOARD_delay(1);
                        txRetrievePktCnt += BoardDiag_enetLpbkRetrieveFreeTxPkts();
                    }
                }
                else
                {
                    break;
                }
            }
            
            /* wait to receive the packet */
            do
            {
                while (BTRUE != gRxSem);
                gRxSem = BFALSE;

                /* Get the packets received so far */
                rxReadyCnt = BoardDiag_enetLpbkReceivePkts();
                if (0U < rxReadyCnt)
                {
                    /* Consume the received packets and release them */
                    pktInfo =
                    (EnetDma_Pkt *)EnetQueue_deq(&gEnetLpbk.rxReadyQ);
                    while (NULL != pktInfo)
                    {
                        EnetDma_checkPktState(&pktInfo->pktState,
                                                ENET_PKTSTATE_MODULE_APP,
                                                ENET_PKTSTATE_APP_WITH_READYQ,
                                                ENET_PKTSTATE_APP_WITH_FREEQ);

                        /* Consume the packet by just printing its content */
                        if (gEnetLpbk.printFrame)
                        {
                            frame = (EthFrame *)pktInfo->bufPtr;

                            BoardDiag_printFrame(frame, pktInfo->userBufLen -
                                        sizeof(EthFrameHeader));
                        }

                        /* Release the received packet */
                        EnetQueue_enq(&gEnetLpbk.rxFreeQ, &pktInfo->node);
                        pktInfo =
                        (EnetDma_Pkt *)EnetQueue_deq(&gEnetLpbk.rxReadyQ);
                    }

                    /*Submit now processed buffers */
                    if (ENET_SOK == status)
                    {
                        EnetAppUtils_validatePacketState(
                                                &gEnetLpbk.rxFreeQ,
                                                ENET_PKTSTATE_APP_WITH_FREEQ,
                                                ENET_PKTSTATE_APP_WITH_DRIVER);

                        EnetDma_submitRxPktQ(gEnetLpbk.hRxCh,
                                         &gEnetLpbk.rxFreeQ);
                    }
                }

                loopRxPktCnt += rxReadyCnt;
            }
            while (loopRxPktCnt < loopTxPktCnt);

            gEnetLpbk.totalRxCnt += loopTxPktCnt;
        }

        gEnetLpbk.totalTxCnt += pktCnt;
    }
    
    if (ENET_SOK == status)
    {
        if(gEnetLpbk.totalTxCnt != gEnetLpbk.totalRxCnt)
        {
            UART_printf("Packet loss, Test Failed\n");
            return -1;
        }
        UART_printf("\nTransmitted - %d packets\nReceived - %d packets\n",
                        gEnetLpbk.totalTxCnt, gEnetLpbk.totalRxCnt);
    }
    else
    {
        UART_printf("Failed to transmit/receive packets: %d,transmitted: %d\n",
                    BOARD_DIAG_ENETLPBK_TEST_PKT_NUM, gEnetLpbk.totalTxCnt);
    }

    return status;
}

/**
 * \brief   CPSW delay function
 *
 * \param   waitTime     [IN]   Wait time
 *
 */
static void BoardDiag_enetWait(uint32_t waitTime)
{
    volatile uint32_t index;

    /* we multiply waitTime by 10000 as 400MHz R5 takes 2.5ns for single cycle
     * and we assume for loop takes 4 cycles */
    for (index = 0; index < (waitTime*1000); index++);
}

/**
 * \brief   This function Waits till phy link is up.
 *
 * \param   NULL
 *
 * \return  int32_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int32_t BoardDiag_enetLpbkWaitForLinkUp(void)
{
    Enet_IoctlPrms prms;
    bool linked = BFALSE;
    int32_t status = ENET_SOK;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &gEnetLpbk.macPort, &linked);

    while (!linked)
    {
        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                            ENET_PER_IOCTL_IS_PORT_LINK_UP, &prms);
        if (ENET_SOK != status)
        {
            UART_printf("Failed to get port %u's link status: %d\n",
                            ENET_MACPORT_ID(gEnetLpbk.macPort), status);
            linked = BFALSE;
            break;
        }

        if (!linked)
        {
            Enet_periodicTick(gEnetLpbk.hEnet);
            BoardDiag_enetWait(1000U);
        }
    }

    return status;
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
static int32_t BoardDiag_enetLpbkShowAlivePhys(void)
{
    Enet_IoctlPrms prms;
    bool alive = BFALSE;
    int8_t phyCnt;
    int32_t status;

    for (phyCnt = 0U; phyCnt < ENET_MDIO_PHY_CNT_MAX; phyCnt++)
    {
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &phyCnt, &alive);

        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                            ENET_MDIO_IOCTL_IS_ALIVE, &prms);
        if (ENET_SOK == status)
        {
            if (BTRUE == alive)
            {
                UART_printf("PHY %u is alive\n", phyCnt);
            }
        }
        else
        {
            UART_printf("Failed to get PHY %u alive status: %d\n", phyCnt, status);
            return ENET_EFAIL;
        }
    }

    return status;
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
static int32_t BoardDiag_enetLpbkSetupCpswAle(void)
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

    EnetUtils_copyMacAddr(&setUcastInArgs.addr.addr[0U],
                          gEnetLpbk.hostMacAddr);

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &entryIdx);

    status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                        CPSW_ALE_IOCTL_ADD_UCAST, &prms);
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

        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                            CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        if (ENET_SOK != status)
        {
            UART_printf("Failed to set ALE port state: %d\n", status);
        }
    }

    return status;
}

/**
 * \brief   This function is used to initialize the receive ready packet queue
 *
 * \param   NULL
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_enetLpbkInitRxReadyPktQ(void)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pPktInfo;
    int32_t status;
    uint32_t memUtilsRxPkts;

    EnetQueue_initQ(&gEnetLpbk.rxFreeQ);
    EnetQueue_initQ(&gEnetLpbk.rxReadyQ);
    EnetQueue_initQ(&rxReadyQ);

    for (memUtilsRxPkts = 0U; memUtilsRxPkts < ENET_MEM_NUM_RX_PKTS; memUtilsRxPkts++)
    {
        pPktInfo =
        EnetMem_allocEthPkt(&gEnetLpbk,
                            ENET_MEM_LARGE_POOL_PKT_SIZE,
                            ENETDMA_CACHELINE_ALIGNMENT);
        if(pPktInfo == NULL)
        {
            UART_printf("EnetMem_allocEthPkt failed\n");
            return -1;
        }
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState,
                                     ENET_PKTSTATE_APP_WITH_FREEQ);
        EnetQueue_enq(&gEnetLpbk.rxFreeQ, &pPktInfo->node);
    }

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(gEnetLpbk.hRxCh, &rxReadyQ);
    if(status != ENET_SOK)
    {
        UART_printf("EnetDma_retrieveRxPktQ failed\n");
        return -1;
    }
    /* There should not be any packet with DMA during init */
    if(EnetQueue_getQCount(&rxReadyQ) != 0U)
    {
        UART_printf("EnetQueue_getQCount failed\n");
        return -1;
    }

    EnetAppUtils_validatePacketState(&gEnetLpbk.rxFreeQ,
                                     ENET_PKTSTATE_APP_WITH_FREEQ,
                                     ENET_PKTSTATE_APP_WITH_DRIVER);

    EnetDma_submitRxPktQ(gEnetLpbk.hRxCh,
                         &gEnetLpbk.rxFreeQ);

    /* Assert here as during init no. of DMA descriptors should be equal to
     * no. of free Ethernet buffers available with app */
    if(EnetQueue_getQCount(&gEnetLpbk.rxFreeQ) != 0U)
    {
        UART_printf("EnetQueue_getQCount failed\n");
        return -1;
    }
    return 0;
}

/**
 * \brief   This function is used to initialize the free packet
 *          info queue with the Ethernet packets to be transmitted.
 *
 * \param   NULL
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_enetLpbkInitTxFreePktQ(void)
{
    EnetDma_Pkt *pPktInfo;
    uint32_t memUtilsTxPkts;

    /* Initialize all queues */
    EnetQueue_initQ(&gEnetLpbk.txFreePktInfoQ);

    /* Initialize TX EthPkts and queue them to txFreePktInfoQ */
    for (memUtilsTxPkts = 0U; memUtilsTxPkts < ENET_MEM_NUM_TX_PKTS; memUtilsTxPkts++)
    {
        pPktInfo = 
        EnetMem_allocEthPkt(&gEnetLpbk,
                                       ENET_MEM_LARGE_POOL_PKT_SIZE,
                                       ENETDMA_CACHELINE_ALIGNMENT);
        if(pPktInfo == NULL)
        {
            UART_printf("EnetMem_allocEthPkt failed\n");
            return -1;
        }
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState,
                                     ENET_PKTSTATE_APP_WITH_FREEQ);

        EnetQueue_enq(&gEnetLpbk.txFreePktInfoQ, &pPktInfo->node);
    }

    UART_printf("initQs() txFreePktInfoQ initialized with %d pkts\n",
                       EnetQueue_getQCount(&gEnetLpbk.txFreePktInfoQ));
    return 0;
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
static int8_t BoardDiag_enetLpbkOpenDma()
{
    int32_t status = ENET_SOK;
    EnetCpdma_OpenRxChPrms rxChCfg;
    EnetCpdma_OpenTxChPrms txChCfg;

    /* Open the CPSW TX channel  */
    if (status == ENET_SOK)
    {
        EnetUdma_initTxChParams(&txChCfg);

        txChCfg.cbArg   = &gEnetLpbk;
        txChCfg.notifyCb = BoardDiag_enetLpbkTxIsrFxn;

        EnetAppUtils_setCommonTxChPrms(&txChCfg);

        EnetAppUtils_openTxCh(gEnetLpbk.hEnet,
                              gEnetLpbk.coreKey,
                              gEnetLpbk.coreId,
                              &gEnetLpbk.txChNum,
                              &gEnetLpbk.hTxCh,
                              &txChCfg);

        if(BoardDiag_enetLpbkInitTxFreePktQ() != BOARD_DIAG_SUCCESS)
        {
            UART_printf("BoardDiag_enetLpbkInitTxFreePktQ Failed\n");
            return -1;
        }

        if (NULL == gEnetLpbk.hTxCh)
        {
            /* Free the Ch Num if open Tx Ch failed */
            EnetAppUtils_freeTxCh(gEnetLpbk.hEnet,
                                  gEnetLpbk.coreKey,
                                  gEnetLpbk.coreId,
                                  gEnetLpbk.txChNum);
            UART_printf("EnetUdma_openTxCh() failed to open: %d\n",
                               status);
        }
    }

    /* Open the CPSW RX Channel  */
    if (status == ENET_SOK)
    {
        EnetUdma_initRxFlowParams(&rxChCfg);
        rxChCfg.notifyCb = BoardDiag_enetLpbkRxIsrFxn;
        rxChCfg.cbArg   = &gEnetLpbk;

        EnetAppUtils_setCommonRxChPrms(&rxChCfg);

        EnetAppUtils_openRxCh(gEnetLpbk.hEnet,
                              gEnetLpbk.coreKey,
                              gEnetLpbk.coreId,
                              &gEnetLpbk.rxChNum,
                              &gEnetLpbk.hRxCh,
                              &rxChCfg);

        if (NULL == gEnetLpbk.hRxCh)
        {
            UART_printf("EnetUdma_openRxFlow() failed to open: %d\n",
                               status);

	    /* TODO: should we close TxCh here */
        }
        else
        {
            status = EnetAppUtils_allocMac(gEnetLpbk.hEnet,
                                           gEnetLpbk.coreKey,
                                           gEnetLpbk.coreId,
                                           gEnetLpbk.hostMacAddr);
            UART_printf("Host MAC address: ");
            BoardDiag_printMacAddr(gEnetLpbk.hostMacAddr);
            /* Submit all ready RX buffers to DMA.*/
            if(BoardDiag_enetLpbkInitRxReadyPktQ() != BOARD_DIAG_SUCCESS)
            {
                UART_printf("BoardDiag_enetLpbkInitRxReadyPktQ failed\n");
                return -1;
            }
        }
    }
    if(status != ENET_SOK)
    {
        return -1;
    }
    return 0;
}

/**
 * \brief   This function is used set MII mode.
 *
 * \param   emac_mode       macMode   mac mode
 *          EnetPhy_Mii     *mii      MII mode
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_enetLpbkMacMode2PhyMii(emac_mode macMode,
                                    EnetPhy_Mii *mii)
{
    switch (macMode)
    {
        case RMII:
            *mii = ENETPHY_MAC_MII_RMII;
            break;
        case RGMII:
            *mii = ENETPHY_MAC_MII_RGMII;
            break;
        default:
            UART_printf("Invalid MAC mode: %u\n", macMode);
            return -1;
    }
    return 0;
}


/**
 * \brief   This function is used set layerType,sublayerType and variantType.
 *
 * \param   emac_mode                   macMode   mac mode
 *          EnetMacPort_Interface       *mii      MII mode
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_enetLpbkMacMode2MacMii(emac_mode macMode,
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
        default:
            UART_printf("Invalid MAC mode: %u\n", macMode);
            return -1;
    }
    return 0;
}

/**
 * \brief   This function is used init cpsw configurations.
 *
 * \param   Cpsw_Cfg *cpswCfg           cpsw configurations
 *
 * \return  NULL
 *
 */
static void BoardDiag_enetLpbkInitCpswCfg(Cpsw_Cfg *cpswCfg)
{
    CpswHostPort_Cfg *hostPortCfg = &cpswCfg->hostPortCfg;
    CpswAle_Cfg *aleCfg = &cpswCfg->aleCfg;
    CpswCpts_Cfg *cptsCfg = &cpswCfg->cptsCfg;

    /* Set initial config */
    Enet_initCfg(gEnetLpbk.enetType, gEnetLpbk.instId, cpswCfg,
                sizeof(*cpswCfg));

    /* Peripheral config */
    cpswCfg->vlanCfg.vlanAware = BFALSE;

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
    /* Note: Timestamping and MAC loopback are not supported together because
     * of IP limitation, so disabling timestamping for this application */
    cptsCfg->hostRxTsEn = BFALSE;

    EnetAppUtils_initResourceConfig(gEnetLpbk.enetType, gEnetLpbk.coreId,
                                    &cpswCfg->resCfg);
}

/**
 * \brief   This function is used to open the ENET driver
 *
 * \param   NULL
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_enetLpbkOpenEnet(void)
{
    Cpsw_Cfg cpswCfg;
    EnetDma_Cfg dmaCfg;
    Enet_IoctlPrms prms;
    EnetPer_PortLinkCfg portLinkCfg;
    CpswMacPort_Cfg macCfg;
    int32_t status = ENET_SOK;

    cpswCfg.dmaCfg = &dmaCfg;
    /* Initialize peripheral config */
    BoardDiag_enetLpbkInitCpswCfg(&cpswCfg);

    UART_printf("CPSW_2G Test on MCU NAVSS\n");
 
    /* Set Enet global runtime log level */
    Enet_setTraceLevel(ENET_TRACE_DEBUG);

    /* Open the Enet driver */
    gEnetLpbk.hEnet = Enet_open(gEnetLpbk.enetType, gEnetLpbk.instId, &cpswCfg,
                                sizeof(cpswCfg));

    if (gEnetLpbk.hEnet == NULL)
    {
        UART_printf("Failed to open Enet driver\n");
        return -1;
    }

    /* Setup port link open parameters */
    if (status == ENET_SOK)
    {
        EnetBoard_EthPort ethPort;
        EnetMacPort_LinkCfg *linkCfg = &portLinkCfg.linkCfg;
        EnetMacPort_Interface *mii = &portLinkCfg.mii;
        EnetPhy_Cfg *phyCfg = &portLinkCfg.phyCfg;
        EnetPhy_Mii phyMii;

        /* Setup board for requested Ethernet port */
        ethPort.enetType = gEnetLpbk.enetType;
        ethPort.instId   = gEnetLpbk.instId;
        ethPort.macPort  = gEnetLpbk.macPort;
        ethPort.boardId  = gEnetLpbk.boardId;
        if(BoardDiag_enetLpbkMacMode2MacMii(gEnetLpbk.macMode, &ethPort.mii) != 
                                            BOARD_DIAG_SUCCESS)
        {
            UART_printf("BoardDiag_enetLpbkMacMode2MacMii failed\n");
            return -1;
        }

        status = EnetBoard_setupPorts(&ethPort, 1U);
        if(status != ENET_SOK)
        {
            UART_printf("EnetBoard_setupPorts failed\n");
            return ENET_EFAIL;
        }

        /* Set port link params */
        portLinkCfg.macPort = gEnetLpbk.macPort;
        portLinkCfg.macCfg = &macCfg;

        CpswMacPort_initCfg(&macCfg);

        if(BoardDiag_enetLpbkMacMode2MacMii(gEnetLpbk.macMode, mii) 
                                            != BOARD_DIAG_SUCCESS)
        {
            UART_printf("BoardDiag_enetLpbkMacMode2MacMii failed\n");
            return ENET_EFAIL;
        }

        if (gEnetLpbk.testPhyLoopback)
        {
            const EnetBoard_PhyCfg *boardPhyCfg = NULL;

            /* Set PHY configuration params */
            EnetPhy_initCfg(phyCfg);

            if (BoardDiag_enetLpbkMacMode2PhyMii(gEnetLpbk.macMode, &phyMii) 
                                                == BOARD_DIAG_SUCCESS)
            {
                boardPhyCfg = EnetBoard_getPhyCfg(&ethPort);
                if (boardPhyCfg != NULL)
                {
                    phyCfg->phyAddr     = boardPhyCfg->phyAddr;
                    phyCfg->isStrapped  = boardPhyCfg->isStrapped;
                    phyCfg->skipExtendedCfg = boardPhyCfg->skipExtendedCfg;
                    phyCfg->extendedCfgSize = boardPhyCfg->extendedCfgSize;
                    memcpy(phyCfg->extendedCfg, boardPhyCfg->extendedCfg,
                            phyCfg->extendedCfgSize);
                }
                else
                {
                    UART_printf("PHY info not found\n");
                    return ENET_EFAIL;
                }

                if ((phyMii == ENETPHY_MAC_MII_MII) ||
                    (phyMii == ENETPHY_MAC_MII_RMII))
                {
                    linkCfg->speed = ENET_SPEED_100MBIT;
                }
                else
                {
                    /* TODO: TPR12 always 100 Mbits */
                    linkCfg->speed = ENET_SPEED_100MBIT;
                }

                linkCfg->duplexity = ENET_DUPLEX_FULL;
            }
            else
            {
                UART_printf("BoardDiag_enetLpbkMacMode2PhyMii failed\n");
                return ENET_EFAIL;
            }
        }
        else
        {
            phyCfg->phyAddr = ENETPHY_INVALID_PHYADDR;

            if (mii->layerType == ENET_MAC_LAYER_MII)
            {
                linkCfg->speed = ENET_SPEED_100MBIT;
            }
            else
            {
                linkCfg->speed = ENET_SPEED_1GBIT;
            }

            linkCfg->duplexity = ENET_DUPLEX_FULL;

        }

        /* MAC and PHY loopbacks are mutually exclusive */
        phyCfg->loopbackEn = gEnetLpbk.testPhyLoopback &&
                                 !gEnetLpbk.testExtLoopback;

        macCfg.loopbackEn = !gEnetLpbk.testPhyLoopback;
    }

    /* Open port link */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &portLinkCfg);

        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                            ENET_PER_IOCTL_OPEN_PORT_LINK, &prms);
        if (status != ENET_SOK)
        {
            UART_printf("Failed to open port link: %d\n", status);
            return ENET_EFAIL;
        }
    }

    return 0;
}

/**
 * \brief   This function is used to initialiaze the enet
            parameters(gEnetLpbk) Ethernet external loopback test.
 *
 * \param   NULL
 *
 * \return  NULL
 *
 */
static void BoardDiag_enetIniParams()
{
     /* Initialize loopback test config */
    memset(&gEnetLpbk, 0, sizeof(gEnetLpbk));
    
    gEnetLpbk.enetType        = ENET_CPSW_9G;
    gEnetLpbk.instId          = 0U;
    gEnetLpbk.testPhyLoopback = BTRUE;
    gEnetLpbk.testExtLoopback = BTRUE;
    gEnetLpbk.macPort         = ENET_MAC_PORT_1;
    gEnetLpbk.macMode         = RGMII;
    gEnetLpbk.enetType        = ENET_CPSW_2G;
    gEnetLpbk.boardId         = ENETBOARD_CPB_ID;

}

/**
 * \brief   This function is used to perform the CPSW
 *          Ethernet external loopback test
 *
 * \param   NULL
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_cpswLoopbackTest()
{
    EnetUtils_Cfg utilsCfg;
    Enet_IoctlPrms prms;
    int32_t status;

    /* Initialize the phy configurations */
    EnetBoard_init();

    /* Initialize the enet parameters */
    BoardDiag_enetIniParams();

    EnetAppUtils_enableClocks(gEnetLpbk.enetType, gEnetLpbk.instId);

    /* Local core id */
    gEnetLpbk.coreId = EnetSoc_getCoreId();

    /* Initialize Enet driver (use default utils) */
    Enet_initUtilsCfg(&utilsCfg);
    utilsCfg.print = EnetAppUtils_print;
    Enet_init(&utilsCfg);

    /* Open Enet driver */
    status = BoardDiag_enetLpbkOpenEnet();
    if (ENET_SOK != status)
    {
        UART_printf("Failed to open Enet driver: %d\n", status);
        return ENET_EFAIL;
    }
#if !(defined(SOC_TPR12) || defined(SOC_AWR294X))
    if (ENET_SOK == status)
    {
        /* Attach the core with RM */
        uint32_t coreId;
        EnetPer_AttachCoreOutArgs attachCoreOutArgs;
        coreId = gEnetLpbk.coreId;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &coreId, &attachCoreOutArgs);
        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                            ENET_PER_IOCTL_ATTACH_CORE, &prms);
        if (ENET_SOK != status)
        {
            UART_printf("EnetLpbk_loopbackTest failed"
                         "ENET_PER_IOCTL_ATTACH_CORE: %d\n", status);
            return ENET_EFAIL;
        }
        else
        {
            gEnetLpbk.coreKey = attachCoreOutArgs.coreKey;
        }
    }
#endif
    if (ENET_SOK == status)
    {
        /* memutils open should happen after Cpsw is opened as it uses 
         * CpswUtils_Q functions */
        status = EnetMem_init();
        if (ENET_SOK != status)
        {
            UART_printf("EnetMem_init failed: "
                        "%d\n", status);
            return ENET_EFAIL;
        }
    }

    /* Open DMA driver */
    if (ENET_SOK == status)
    {
        status = BoardDiag_enetLpbkOpenDma();
        if (ENET_SOK != status)
        {
            UART_printf("Failed to open DMA: %d\n", status);
            return ENET_EFAIL;
        }
    }

    /* Enable host port */
    if (ENET_SOK == status)
    {
        if (Enet_isCpswFamily(gEnetLpbk.enetType))
        {
            status = BoardDiag_enetLpbkSetupCpswAle();
            if (ENET_SOK != status)
            {
                UART_printf("Failed to setup CPSW ALE: %d\n", status);
                return ENET_EFAIL;
            }
        }

        if (ENET_SOK == status)
        {
            ENET_IOCTL_SET_NO_ARGS(&prms);
            status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                                ENET_HOSTPORT_IOCTL_ENABLE, &prms);
            if (ENET_SOK != status)
            {
                UART_printf("Failed to enable host port: %d\n", status);
                return ENET_EFAIL;
            }
        }
    }

    /* Show alive PHYs */
    if (ENET_SOK == status)
    {
        status = BoardDiag_enetLpbkShowAlivePhys();
    }

    /* Wait for link up */
    if ((ENET_SOK == status) && gEnetLpbk.testPhyLoopback)
    {
        status = BoardDiag_enetLpbkWaitForLinkUp();
    }

    /* Do packet transmission and reception */
    if (ENET_SOK == status)
    {
       status = BoardDiag_cpswPktRxTx();
    }

    /* Print network statistics */
    if (ENET_SOK == status)
    {
        if (Enet_isCpswFamily(gEnetLpbk.enetType))
        {
            BoardDiag_enetLpbkShowCpswStats();
        }
    }

    /* Disable host port */
    if (ENET_SOK == status)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(gEnetLpbk.hEnet, gEnetLpbk.coreId,
                            ENET_HOSTPORT_IOCTL_DISABLE, &prms);
        if (ENET_SOK != status)
        {
            UART_printf("Failed to disable host port: %d\n", status);
            return ENET_EFAIL;
        }
    }

    /* Print DMA statistics */
    if (ENET_SOK == status)
    {
        EnetAppUtils_showRxChStats(gEnetLpbk.hRxCh);
        EnetAppUtils_showTxChStats(gEnetLpbk.hTxCh);
    }

    /* Close Enet DMA driver */
    BoardDiag_enetLpbkCloseDma();

    /* Close Enet driver */
    BoardDiag_enetLpbkCloseEnet();

    /* Disable peripheral clocks */
    EnetAppUtils_disableClocks(gEnetLpbk.enetType, gEnetLpbk.instId);

    /* Deinit Enet driver */
    Enet_deinit();
    UART_printf("Deinitializing of Enet driver done\n");

    if(ENET_SOK == status)
    {
        UART_printf("Test Passed\n");
    }
    else
    {
        UART_printf("Test Failed\n");
        return ENET_EFAIL;
    }

    return ENET_SOK;
}

/**
 *  \brief    This function runs CPSW ethernet test
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_cpswEthRunTest(void)
{
    int8_t ret;

    UART_printf("\n*********************************************\n");
    UART_printf  ("*            ENET Ethernet Test             *\n");
    UART_printf  ("*********************************************\n");

    /* Run the loopback test */
    ret = BoardDiag_cpswLoopbackTest();

    return ret;
}

/**
 * \brief  CPSW diagnostic test main function
 *
 *  This function performs board initializations and calls cpsw ethernet test
 *
 * \return  int
 *              0  - in case of success
 *             -1  - in case of failure
 *
 */
#ifndef SPI_BOOT_FRAMEWORK
int main(void)
{
    Board_initCfg boardCfg;
    Board_STATUS status;
    int8_t ret = ENET_SOK;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;
#endif

    status = Board_init(boardCfg);
    if(BOARD_SOK != status)
    {
        return ENET_EFAIL;
    }

    ret = BoardDiag_cpswEthRunTest();
    if(ENET_SOK == ret)
    {
        UART_printf("CPSW Loopback Test Passed\n\r");
        UART_printf("All tests have passed\n\r");
    }
    else
    {
        UART_printf("CPSW Loopback Test failed\n\r");
    }

    return ret;

}
#endif

