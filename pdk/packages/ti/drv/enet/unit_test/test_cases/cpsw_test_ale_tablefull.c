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
 * \file     cpsw_test_ale_tablefull.c
 *
 * \brief    This file contains the cpsw_test_ale_tablefull test implementation.
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

#include <ti/osal/osal.h>

#include <ti/board/board.h>

#include "enet_test_base.h"
#include "enet_test_entry.h"
#include "cpsw_test_ale_tablefull.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t EnetTestAleTableFull_pktRxTx(EnetTestTaskObj *taskObj,
                                             uint32_t txCfgIndex);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestAleTableFull_Run(EnetTestTaskObj *taskObj)
{
    int32_t status = ENET_SOK;

   EnetAppUtils_assert(taskObj->taskCfg->numTxCh == 1U);
    EnetTestCommon_waitForPortLink(taskObj);
    status = EnetTestAleTableFull_pktRxTx(taskObj, 0);

    return status;
}

void EnetTestAleTableFull_setOpenPrms(EnetTestTaskObj *taskObj,
                                      Cpsw_Cfg *pCpswCfg,
                                      EnetUtils_Cfg *pUtilsPrms)
{
    pCpswCfg->aleCfg.agingCfg.autoAgingEn = BFALSE;
}

void EnetTestAleTableFull_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                            Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg   = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn        = BFALSE;
}

static int32_t EnetTestAleTableFull_pktRxTx(EnetTestTaskObj *taskObj,
                                             uint32_t txCfgIndex)
{
    EnetDma_PktQ txSubmitQ;
    EnetDma_Pkt *pktInfo;
    EthFrame *frame;
    uint32_t txRetrievePktCnt, totalTxCnt;
    uint32_t pktCnt;
    int32_t status = ENET_SOK;
    uint32_t i;

    /* Broadcast address */
    uint8_t bcastAddr[ENET_MAC_ADDR_LEN] =
    {
        0xffU, 0xffU, 0xffU, 0xffU, 0xffU, 0xffU
    };
    /* Test ALE Entry Address */
    static uint8_t testAddr[ENET_MAC_ADDR_LEN] =
    {
        0x40U, 0xcdU, 0xefU, 0xfeU, 0xdcU, 0xbaU
    };

    totalTxCnt = 0U;

    EnetQueue_initQ(&txSubmitQ);

    for (i = 0; i < taskObj->stateObj.txChCfgInfo[txCfgIndex]->pktSendLoopCount; i++)
    {
        pktCnt = 0U;
        /* Transmit a single packet */
        EnetQueue_initQ(&txSubmitQ);
        /* Dequeue one free TX Eth packet */
        pktInfo =
            (EnetDma_Pkt *)EnetQueue_deq(&taskObj->stateObj.txChObj[txCfgIndex].txFreePktInfoQ);
        while (pktCnt < taskObj->stateObj.txChCfgInfo[txCfgIndex]->pktSendCount)
        {
            pktCnt++;
            /* Fill the TX Eth frame with test content */
            frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
            memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
            memcpy(frame->hdr.srcMac, &testAddr[0U], ENET_MAC_ADDR_LEN);
            testAddr[0U]        += (i * 2U);
            testAddr[1U]        += 2U;
            frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
            memset(&frame->payload[0U], (uint8_t)(0xA5 + pktCnt),
                   taskObj->stateObj.txChCfgInfo[txCfgIndex]->maxTxPktLen);
            pktInfo->sgList.list[0].segmentFilledLen = taskObj->stateObj.txChCfgInfo[txCfgIndex]->maxTxPktLen +
                                  sizeof(EthFrameHeader);
            pktInfo->appPriv = taskObj;

            /* Enqueue the packet for later transmission */
            EnetQueue_enq(&txSubmitQ, &pktInfo->node);

            /* Dequeue one free TX Eth packet */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&taskObj->stateObj.txChObj[txCfgIndex].txFreePktInfoQ);
        }

        while (0U != EnetQueue_getQCount(&txSubmitQ))
        {
            uint32_t txCnt = EnetQueue_getQCount(&txSubmitQ);
            status =EnetDma_submitTxPktQ(taskObj->stateObj.txChObj[txCfgIndex].hTxCh,
                                                  &txSubmitQ);
            /* Retrieve TX free packets */
            if (status == ENET_SOK)
            {
                txCnt            = txCnt - EnetQueue_getQCount(&txSubmitQ);
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

        totalTxCnt += pktCnt;
    }

    if (status == ENET_SOK)
    {
       EnetAppUtils_print("Transmitted %d packets\n", totalTxCnt);
    }

    totalTxCnt   = 0U;
    testAddr[0U] = 0x40U;
    testAddr[1U] = 0xcdU;

    for (i = 0; i < taskObj->stateObj.txChCfgInfo[txCfgIndex]->pktSendLoopCount; i++)
    {
        pktCnt = 0U;
        /* Dequeue one free TX Eth packet */
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&taskObj->stateObj.txChObj[txCfgIndex].txFreePktInfoQ);

        while (pktCnt < taskObj->stateObj.txChCfgInfo[txCfgIndex]->pktSendCount)
        {
            pktCnt++;
            /* Fill the TX Eth frame with test content */
            frame         = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
            testAddr[0U] += (i * 2U);
            testAddr[1U] += 2U;
            memcpy(frame->hdr.dstMac, &testAddr[0U], ENET_MAC_ADDR_LEN);
            testAddr[0U] += (i * 2U);
            testAddr[1U] += 2U;
            memcpy(frame->hdr.srcMac, &testAddr[0U], ENET_MAC_ADDR_LEN);
            frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
            memset(&frame->payload[0U], (uint8_t)(0xA5 + pktCnt),
                   taskObj->stateObj.txChCfgInfo[txCfgIndex]->maxTxPktLen);
            pktInfo->sgList.list[0].segmentFilledLen = taskObj->stateObj.txChCfgInfo[txCfgIndex]->maxTxPktLen +
                                  sizeof(EthFrameHeader);
            pktInfo->appPriv = &taskObj;

            /* Enqueue the packet for later transmission */
            EnetQueue_enq(&txSubmitQ, &pktInfo->node);

            /* Dequeue one free TX Eth packet */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&taskObj->stateObj.txChObj[txCfgIndex].txFreePktInfoQ);
        }

        while (0U != EnetQueue_getQCount(&txSubmitQ))
        {
            uint32_t txCnt = EnetQueue_getQCount(&txSubmitQ);
            status =EnetDma_submitTxPktQ(taskObj->stateObj.txChObj[txCfgIndex].hTxCh,
                                                  &txSubmitQ);
            /* Retrieve TX free packets */
            if (status == ENET_SOK)
            {
                txCnt            = txCnt - EnetQueue_getQCount(&txSubmitQ);
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

        totalTxCnt += pktCnt;
    }

    if (status == ENET_SOK)
    {
       EnetAppUtils_print("Transmitted %d packets\n", totalTxCnt);
    }
    else
    {
       EnetAppUtils_print("Failed to transmit/receive packets: %d, transmitted: %d \n", taskObj->stateObj.txChCfgInfo[txCfgIndex]->pktSendCount, totalTxCnt);
    }

    return status;
}
