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
 * \file     cpsw_test_sanity.c
 *
 * \brief    This file contains the cpsw sanity test implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/csl_cpswitch.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethpatterns.h>

#include <ti/osal/osal.h>

#include <ti/board/board.h>

#include "enet_test_entry.h"
#include "enet_test_base.h"
#include "cpsw_test_sanity.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
uint32_t EnetTest_recvTest(EnetTestTaskObj *taskObj,
                            uint32_t rxCfgIndex);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* Broadcast address */
static uint8_t bcastAddr[ENET_MAC_ADDR_LEN] =
{
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static int32_t EnetTestSanityMacSpeedCommon_Run(EnetTestTaskObj *taskObj)
{
    int32_t status = ENET_SOK;

    /*fix for gcc unused variable warning */
    Enet_DataPattern2[0U] = Enet_DataPattern2[0U];
    Enet_DataPattern3[0U] = Enet_DataPattern3[0U];
    Enet_DataPattern4[0U] = Enet_DataPattern4[0U];

   EnetAppUtils_assert(taskObj->taskCfg->numTxCh == 1U);
   EnetAppUtils_assert(taskObj->taskCfg->numRxFlow == 1U);

    /* Send command indicating that we're ready to start the test stage */
    EnetTest_sendCmd(taskObj, CTRL_FRAME_CMD_DUT_READY, 0U);

    /* test_0001: Receive test packets from host app */
    EnetTest_test_0001(taskObj, 0U, 0U);

    /* test_0002: Transmit test packets to host app */
    EnetTest_test_0002(taskObj, 0U, 0U);

    /* test_0100: Transmit throughput */
    EnetTest_test_0100(taskObj, 0U, 0U);

    EnetTest_discardRxPkts(taskObj, 0U);

    EnetTest_freeRxBuffer(taskObj, 0U);

    return status;
}

void EnetTestSanity_updatePortLinkCfgAutoNeg(EnetPer_PortLinkCfg *pLinkArgs,
                                             Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn          = BFALSE;

}

void EnetTestSanity_updatePortLinkCfg100Mbps(EnetPer_PortLinkCfg *pLinkArgs,
                                             Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn       = BFALSE;
    pLinkArgs->linkCfg.speed = ENET_SPEED_100MBIT;
    pLinkArgs->linkCfg.duplexity     = ENET_DUPLEX_FULL;
}

int32_t EnetTest_transmitPkts(EnetTestTaskObj *taskObj,
                               EnetDma_PktQ *pTxSubmitQ,
                               uint32_t txCfgIndex)
{
    uint32_t txRetrievePktCnt, txCnt;
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    while (0U != EnetQueue_getQCount(pTxSubmitQ))
    {
        txCnt  = EnetQueue_getQCount(pTxSubmitQ);
        status =EnetDma_submitTxPktQ(stateObj->txChObj[txCfgIndex].hTxCh,
                                              pTxSubmitQ);
        /* Retrieve TX free packets */
        if (status == ENET_SOK)
        {
            txCnt            = txCnt - EnetQueue_getQCount(pTxSubmitQ);
            txRetrievePktCnt = 0U;
            while (txRetrievePktCnt != txCnt)
            {
                /* This is not failure as HW is busy sending packets, we need
                 * to wait and again call retrieve packets */
                EnetTest_wait(1U);
                txRetrievePktCnt += EnetTestCommon_retrieveFreeTxPkts(taskObj, txCfgIndex);
#if DEBUG
               EnetAppUtils_print("Failed to retrieve consumed transmit packets: %d\n",
                                   status);
#endif
            }
        }
        else
        {
            break;
        }
    }

    return status;
}

int32_t EnetTestMacSpeed_Run(EnetTestTaskObj *taskObj)
{
    int32_t status = ENET_SOK;
    EnetTestMacPortList_t enabledPorts;

    EnetTestCommon_waitForPortLink(taskObj);

    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    EnetPhy_GenericInArgs inArgs;

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
    /* Take any port from enabled ports */
    inArgs.macPort = enabledPorts.macPortList[0];

    ENET_IOCTL_SET_IN_ARGS(&prms, &inArgs);
    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_PHY_IOCTL_PRINT_REGS,
                        &prms);

    EnetTestSanityMacSpeedCommon_Run(taskObj);

    return status;
}

int32_t EnetTest_xmitTest(EnetTestTaskObj *taskObj,
                           uint32_t txCfgIndex,
                           uint32_t num,
                           const uint8_t *dstAddr,
                           const uint8_t *srcAddr,
                           uint16_t etherType,
                           uint16_t length)
{
    int32_t retVal = ENET_SOK;
    EnetDma_Pkt *pktInfo;
    uint16_t len    = length - ETH_TEST_DATA_HDR_LEN;
    uint32_t pktCnt = 0U;
    DataFramePayload *payload;
    EnetDma_PktQ txSubmitQ;
    EthFrame *frame;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    while (pktCnt < num)
    {
        EnetQueue_initQ(&txSubmitQ);

        /* Dequeue one free TX Eth packet */
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->txChObj[txCfgIndex].txFreePktInfoQ);

        while (NULL != pktInfo)
        {
            pktCnt++;
            /* Fill the TX Eth frame with test content */
            frame   = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
            payload = (DataFramePayload *)frame->payload;
            memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
            memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U], ENET_MAC_ADDR_LEN);
            frame->hdr.etherType = Enet_htons(etherType);

            payload->type = 0U; /* DataPattern1 */
            payload->len  = Enet_htons(len);
            memcpy(payload->data, Enet_DataPattern1, len);

            pktInfo->sgList.list[0].segmentFilledLen = length + sizeof(EthFrameHeader);
            pktInfo->appPriv    = &stateObj;

            /* Enqueue the packet for later transmission */
            EnetQueue_enq(&txSubmitQ, &pktInfo->node);

            if (pktCnt >= num)
            {
                break;
            }

            /* Dequeue one free TX Eth packet */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->txChObj[txCfgIndex].txFreePktInfoQ);
        }

        retVal = EnetTest_transmitPkts(taskObj, &txSubmitQ, txCfgIndex);
    }

    return retVal;
}

uint32_t EnetTest_recvTest(EnetTestTaskObj *taskObj,
                            uint32_t rxCfgIndex)
{
    EnetDma_Pkt *pktInfo;
    EthFrame *frame;
    uint32_t num = 0U;
    uint8_t cmd;
    bool runTest = BTRUE;
    uint32_t rxReadyCnt;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

   EnetAppUtils_print("EnetTest_recvTest() receiving packets until STOP cmd\n");

    /* Wait for packet reception */
    while (runTest)
    {
        /* Get the packets received so far */
        rxReadyCnt = EnetTestCommon_receivePkts(taskObj, rxCfgIndex);
        if (rxReadyCnt > 0U)
        {
            /* Consume the received packets and release them */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
            while (NULL != pktInfo)
            {
                EnetAppUtils_assert(pktInfo->sgList.numScatterSegments == 1U);
                /* Consume the packet by just printing its content */
                frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
                /* Release the received packet */
                if (Enet_ntohs(frame->hdr.etherType) == ETHERTYPE_EXP_CONTROL)
                {
                    cmd = frame->payload[0];
                    if (cmd == CTRL_FRAME_CMD_STOP)
                    {
                       EnetAppUtils_print("EnetTest_recvTest() STOP cmd has been received\n");
                        runTest = BFALSE;
                    }

                    if (cmd == CTRL_FRAME_CMD_PC_READY)
                    {
                       EnetAppUtils_print("EnetTest_recvTest() PC Ready cmd has been received\n");
                        runTest = BFALSE;
                    }
                }
                else
                {
                    /* Consume the packet by just printing its content */
#ifdef ENABLE_PRINTFRAME
                   EnetAppUtils_printFrame(frame, pktInfo->sgList.list[0].segmentFilledLen -
                                            sizeof(EthFrameHeader));
#endif
                    num++;
                }

                EnetQueue_enq(&stateObj->rxFlowObj[rxCfgIndex].rxFreeQ, &pktInfo->node);
                if (runTest)
                {
                    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
                }
                else
                {
                    break;
                }
            }

            /*Submit now processed buffers */
            {
               EnetDma_submitRxPktQ(stateObj->rxFlowObj[rxCfgIndex].hRxFlow,
                                             &stateObj->rxFlowObj[rxCfgIndex].rxFreeQ);
            }
        }

        EnetTest_wait(10U);
    }

   EnetAppUtils_print("EnetTest_recvTest() %u test packets received\n", num);

    return num;
}

void EnetTest_discardRxPkts(EnetTestTaskObj *taskObj,
                             uint32_t rxCfgIndex)
{
    EnetDma_Pkt *pkt;
    uint32_t count;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    /* Discard any packets sitting in rxReadyQ before the test starts */
    count = EnetQueue_getQCount(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
    if (count != 0U)
    {
       EnetAppUtils_print("EnetTest_discardRxPkts() rxReadyQ has %d packets, discarding all\n",
                           count);
        pkt = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
        while (pkt != NULL)
        {
            /* Release the received packet */
            EnetQueue_enq(&stateObj->rxFlowObj[rxCfgIndex].rxFreeQ, &pkt->node);
            pkt = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
        }
    }
}

void EnetTest_sendCmd(EnetTestTaskObj *taskObj,
                       uint8_t cmd,
                       uint32_t txCfgIndex)
{
    EnetDma_Pkt *pktInfo;
    EthFrame *frame;
    CtrlFramePayload *payload;
    int32_t retVal;
    EnetDma_PktQ txSubmitQ;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    /* Transmit a single packet */
    EnetQueue_initQ(&txSubmitQ);

    /* Dequeue one free TX Eth packet */
    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->txChObj[txCfgIndex].txFreePktInfoQ);
    frame   = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
    payload = (CtrlFramePayload *)frame->payload;

    memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
    memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U], ENET_MAC_ADDR_LEN);
    frame->hdr.etherType = Enet_htons(ETHERTYPE_EXP_CONTROL);

    memset(payload, 0, sizeof(CtrlFramePayload));
    payload->cmd = cmd;

    pktInfo->sgList.list[0].segmentFilledLen = sizeof(CtrlFramePayload) + sizeof(EthFrameHeader);
    pktInfo->appPriv    = &stateObj;

    /* Enqueue the packet for later transmission */
    EnetQueue_enq(&txSubmitQ, &pktInfo->node);

    EnetTest_wait(1000U);

    retVal = EnetTest_transmitPkts(taskObj, &txSubmitQ, txCfgIndex);

    if (retVal != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_sendCmd() failed to send cmd %d: %d\n", cmd, retVal);
    }
}

void EnetTest_test_0001(EnetTestTaskObj *taskObj,
                         uint32_t txCfgIndex,
                         uint32_t rxCfgIndex)
{
    uint32_t num;

   EnetAppUtils_print("Test_0001: START\n");

    EnetTest_discardRxPkts(taskObj, rxCfgIndex);

    /* Send START cmd */
    EnetTest_sendCmd(taskObj, CTRL_FRAME_CMD_START, txCfgIndex);

    /* Receive frames until STOP cmd is detected */
    num = EnetTest_recvTest(taskObj, rxCfgIndex);

    if (num != ETH_TEST_ITER_M_COUNT)
    {
       EnetAppUtils_print("Test_0001: received %d of %d frames\n", num, ETH_TEST_ITER_M_COUNT);
    }

   EnetAppUtils_print("Test_0001: END\n\n");
}

void EnetTest_test_0002(EnetTestTaskObj *taskObj,
                         uint32_t txCfgIndex,
                         uint32_t rxCfgIndex)
{
    uint32_t iterations = ETH_TEST_ITER_M_COUNT;
    uint16_t len        = 1500U;
    uint32_t retVal;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

   EnetAppUtils_print("Test_0002: START\n");

    EnetTest_discardRxPkts(taskObj, rxCfgIndex);

    /* Send START cmd */
    EnetTest_sendCmd(taskObj, CTRL_FRAME_CMD_START, txCfgIndex);

    EnetTest_wait(10U);

    /* Transmit frames */
    retVal = EnetTest_xmitTest(taskObj,
                                txCfgIndex,
                                iterations,
                                bcastAddr,
                                &stateObj->hostMacAddr[0U],
                                ETHERTYPE_EXPERIMENTAL1,
                                len);
    if (retVal != ENET_SOK)
    {
       EnetAppUtils_print("Test_0002: failed to transmit %d packets\n", iterations);
    }

    /* Send STOP cmd */
    EnetTest_sendCmd(taskObj, CTRL_FRAME_CMD_STOP, txCfgIndex);

   EnetAppUtils_print("Test_0002: END\n\n");
}

void EnetTest_test_0100(EnetTestTaskObj *taskObj,
                         uint32_t txCfgIndex,
                         uint32_t rxCfgIndex)
{
    EnetDma_PktQ tempQ;
    EnetDma_Pkt *pkt;
    EthFrame *frame;
    uint32_t iterations = ETH_TEST_ITER_L_COUNT;
    uint16_t len        = 1500U;
    uint32_t retVal;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

   EnetAppUtils_print("Test_0100: START\n");

    EnetTest_discardRxPkts(taskObj, rxCfgIndex);

    /* Send START cmd */
    EnetTest_sendCmd(taskObj, CTRL_FRAME_CMD_START, txCfgIndex);

    EnetTest_wait(10U);

    /* Transmit frames (only fill L2 header) */
    while (iterations > 0U)
    {
        EnetQueue_initQ(&tempQ);

        while (iterations > 0U)
        {
            pkt = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->txChObj[txCfgIndex].txFreePktInfoQ);
            if (pkt == NULL)
            {
                break;
            }

            frame = (EthFrame *)pkt->sgList.list[0].bufPtr;
            memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
            memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U], ENET_MAC_ADDR_LEN);
            frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
            pkt->sgList.list[0].segmentFilledLen      = len + sizeof(EthFrameHeader);

            EnetQueue_enq(&tempQ, &pkt->node);
            iterations--;
        }

        retVal = EnetTest_transmitPkts(taskObj, &tempQ, txCfgIndex);
        if (retVal != ENET_SOK)
        {
           EnetAppUtils_print("Test_0100() failed to transmit: %d\n", retVal);
        }
    }

    /* Send STOP cmd */
    EnetTest_sendCmd(taskObj, CTRL_FRAME_CMD_STOP, txCfgIndex);

   EnetAppUtils_print("Test_0100: END\n\n");
}

void EnetTest_freeRxBuffer(EnetTestTaskObj *taskObj,
                            uint32_t rxCfgIndex)
{
    EnetDma_Pkt *pktInfo;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxFreeQ);
    while (pktInfo != NULL)
    {
        EnetMem_freeEthPkt(pktInfo);
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxFreeQ);
    }
}

int32_t EnetTestSanity_Run(EnetTestTaskObj *taskObj)
{
    int32_t status = ENET_SOK;

    EnetTestCommon_waitForPortLink(taskObj);

    status = EnetTestSanityMacSpeedCommon_Run(taskObj);

    return status;
}

/* end of file */
