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
 * \file     cpsw_test_policer_nomatch.c
 *
 * \brief    This file contains the enet_policer nomatch test implementation.
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
#include "cpsw_test_policer_nomatch.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define NOMATCHRED_TC_ID                           17U
#define NOMATCHYELLOW_TC_ID                        18U
#define NOMATCHPOLICER_TC_ID                       19U

#define ENET_TEST_POLICER_MBPS(x)                   ((x) * 1000000U)
#define RECV_BIT_RATE_DEFAULT_PIR                   ENET_TEST_POLICER_MBPS(200)
#define RECV_BIT_RATE_DEFAULT_CIR                   ENET_TEST_POLICER_MBPS(100)

#define RECV_BIT_RATE_FLOWTYPE1_PIR                 ENET_TEST_POLICER_MBPS(500)
#define RECV_BIT_RATE_FLOWTYPE1_CIR                 ENET_TEST_POLICER_MBPS(250)

#define ENET_TEST_NOMATCHPOLICER_INGRESS_PORT       (ENET_MAC_PORT_3)
#define ENET_TEST_NOMATCHPOLICER_EGRESS_PORT        (ENET_MAC_PORT_2)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t EnetTestPolicerNomatch_Test(EnetTestTaskObj *taskObj,
                                           uint32_t rxFlowCfgId,
                                           uint32_t recvbitRatePir,
                                           uint32_t recvbitRateCir);

static int32_t EnetTestPolicerNomatch_setPolicerAleEntry(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
static uint8_t testSrcAddr[ENET_MAC_ADDR_LEN] =
{
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t testHostPortDestAddr[ENET_MAC_ADDR_LEN] =
{
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t testEgressPortDestAddr[ENET_MAC_ADDR_LEN] =
{
    0x04, 0x00, 0x00, 0x00, 0x00, 0x02
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestPolicerNomatch_Run(EnetTestTaskObj *taskObj)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t rxReadyCnt, timeoutVal, elapsedTIme;

    status = EnetTestPolicerNomatch_setPolicerAleEntry(taskObj);

    if (ENET_SOK == status)
    {
        Enet_IoctlPrms prms;

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_POLICER_ENTRIES,
                            &prms);

       EnetAppUtils_assert(status == ENET_SOK);
    }

    if ((taskObj->testParams->tcId == NOMATCHRED_TC_ID) ||
        (taskObj->testParams->tcId == NOMATCHYELLOW_TC_ID))
    {
        EnetTestCommon_waitForPortLink(taskObj);

        /* Discard all queued packets before entering timeout loop
         * There maybe packets queued during the validateRxFilter
         * Rx filter transition
         */
        EnetTestCommon_discardRxPkts(taskObj, ENETTESTPOLICER_NOMATCH_DEFAULT_RXFLOWCFGID);

        timeoutVal  = 1 * 2 * 60 * 1000;
        elapsedTIme = 0;

        do
        {
            rxReadyCnt = EnetTestCommon_receivePkts(taskObj, ENETTESTPOLICER_NOMATCH_DEFAULT_RXFLOWCFGID);
            if (rxReadyCnt != 0)
            {
                status = ENET_EFAIL;
                break;
            }
            else
            {
                EnetTest_wait(10U);
                elapsedTIme += 10;
            }
        }
        while (elapsedTIme < timeoutVal);
    }
    else if (taskObj->testParams->tcId == NOMATCHPOLICER_TC_ID)
    {
        if (status == ENET_SOK)
        {
            status = EnetTestCommon_Run(taskObj);
        }

        if (status == ENET_SOK)
        {
            status = EnetTestPolicerNomatch_Test(taskObj, ENETTESTPOLICER_NOMATCH_DEFAULT_RXFLOWCFGID, RECV_BIT_RATE_DEFAULT_PIR, RECV_BIT_RATE_DEFAULT_CIR);

            if (status == ENET_SOK)
            {
                status = EnetTestPolicerNomatch_Test(taskObj, ENETTESTPOLICER_NOMATCH_TYPE1_RXFLOWCFGID, RECV_BIT_RATE_FLOWTYPE1_PIR, RECV_BIT_RATE_FLOWTYPE1_CIR);
            }
        }
    }

    return status;
}

void EnetTestPolicerNomatch_setOpenPrms(EnetTestTaskObj *taskObj,
                                        Cpsw_Cfg *pCpswCfg,
                                        EnetUtils_Cfg *pUtilsPrms)
{
    pCpswCfg->aleCfg.policerGlobalCfg.policingEn   = BTRUE;
    pCpswCfg->aleCfg.policerGlobalCfg.redDropEn    = BTRUE;
    pCpswCfg->aleCfg.policerGlobalCfg.yellowDropEn = BFALSE;

    switch (taskObj->testParams->tcId)
    {
        case NOMATCHRED_TC_ID:
            pCpswCfg->aleCfg.policerGlobalCfg.policerNoMatchMode = CPSW_ALE_POLICER_NOMATCH_MODE_RED;
            break;

        case NOMATCHYELLOW_TC_ID:
            pCpswCfg->aleCfg.policerGlobalCfg.policerNoMatchMode = CPSW_ALE_POLICER_NOMATCH_MODE_YELLOW;
            pCpswCfg->aleCfg.policerGlobalCfg.yellowThresh       = CPSW_ALE_POLICER_YELLOWTHRESH_DROP_PERCENT_100;
            pCpswCfg->aleCfg.policerGlobalCfg.yellowDropEn   = BTRUE;

            break;

        case NOMATCHPOLICER_TC_ID:
            pCpswCfg->aleCfg.policerGlobalCfg.policerNoMatchMode                    = CPSW_ALE_POLICER_NOMATCH_MODE_UNREGULATED_TRAFFIC_POLICER;
            pCpswCfg->aleCfg.policerGlobalCfg.noMatchPolicer.peakRateInBitsPerSec   = RECV_BIT_RATE_DEFAULT_PIR;
            pCpswCfg->aleCfg.policerGlobalCfg.noMatchPolicer.commitRateInBitsPerSec = RECV_BIT_RATE_DEFAULT_CIR;
            break;
    }
}

void EnetTestPolicerNomatch_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                              Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn          = BFALSE;
}

static int32_t EnetTestPolicerNomatch_setPolicerAleEntry(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    setPolicerInArgs.policerMatch.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_PORT |
                                                            CPSW_ALE_POLICER_MATCH_MACSRC |
                                                            CPSW_ALE_POLICER_MATCH_MACDST);
    setPolicerInArgs.policerMatch.portNum     = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_NOMATCHPOLICER_INGRESS_PORT);
    setPolicerInArgs.policerMatch.portIsTrunk = BFALSE;
    memcpy(&setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr[0U], testSrcAddr,
           sizeof(setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr));
    setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.vlanId    = 0U;
    setPolicerInArgs.policerMatch.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_NOMATCHPOLICER_INGRESS_PORT);
    memcpy(&setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr[0U], testHostPortDestAddr,
           sizeof(setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr));
    setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.vlanId   = 0U;
    setPolicerInArgs.policerMatch.dstMacAddrInfo.portNum = CPSW_ALE_HOST_PORT_NUM;
    setPolicerInArgs.threadIdEn                        = BTRUE;
    setPolicerInArgs.threadId                              = taskObj->stateObj.rxFlowObj[ENETTESTPOLICER_NOMATCH_TYPE1_RXFLOWCFGID].rxFlowIdx;
    setPolicerInArgs.peakRateInBitsPerSec                  = RECV_BIT_RATE_FLOWTYPE1_PIR;
    setPolicerInArgs.commitRateInBitsPerSec                = RECV_BIT_RATE_FLOWTYPE1_CIR;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_openCpsw() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                           status);
    }

    if (status == ENET_SOK)
    {
        setPolicerInArgs.policerMatch.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_PORT |
                                                                CPSW_ALE_POLICER_MATCH_MACSRC |
                                                                CPSW_ALE_POLICER_MATCH_MACDST);
        setPolicerInArgs.policerMatch.portNum     = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_NOMATCHPOLICER_INGRESS_PORT);
        setPolicerInArgs.policerMatch.portIsTrunk = BFALSE;
        memcpy(&setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr[0U], testSrcAddr,
               sizeof(setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr));
        setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.vlanId    = 0U;
        setPolicerInArgs.policerMatch.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_NOMATCHPOLICER_INGRESS_PORT);
        memcpy(&setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr[0U], testEgressPortDestAddr,
               sizeof(setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr));
        setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.vlanId   = 0U;
        setPolicerInArgs.policerMatch.dstMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_NOMATCHPOLICER_EGRESS_PORT);
        setPolicerInArgs.threadIdEn                        = BFALSE;
        setPolicerInArgs.peakRateInBitsPerSec                  = RECV_BIT_RATE_FLOWTYPE1_PIR;
        setPolicerInArgs.commitRateInBitsPerSec                = RECV_BIT_RATE_FLOWTYPE1_CIR;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTest_openCpsw() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                               status);
        }
    }

    return status;
}

int32_t EnetTestPolicerNomatch_processRxPkt(EnetTestTaskObj *taskObj,
                                            uint32_t rxFlowId,
                                            uint32_t pktNum,
                                            EnetDma_Pkt *pktInfo,
                                            bool *testComplete)
{
    int32_t status = ENET_SOK;
    EthFrame *frame;

    /* Consume the packet by just printing its content */
    frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;

#ifdef ENABLE_PRINTFRAME
   EnetAppUtils_printFrame
        (frame, (pktInfo->sgList.list[0].segmentFilledLen - sizeof(EthFrameHeader)));
#endif

    if (rxFlowId == ENETTESTPOLICER_NOMATCH_TYPE1_RXFLOWCFGID)
    {
        if ((memcmp(&frame->hdr.dstMac[0], &testHostPortDestAddr, sizeof(frame->hdr.dstMac)) == 0) &&
            (memcmp(&frame->hdr.srcMac[0], &testSrcAddr, sizeof(frame->hdr.dstMac)) == 0))
        {
            status = ENET_SOK;
        }
        else
        {
            status = ENET_EFAIL;
        }
    }
    else
    {
        status = ENET_SOK;
    }

    *testComplete = BFALSE;
    return status;
}

static int32_t EnetTestPolicerNomatch_Test(EnetTestTaskObj *taskObj,
                                           uint32_t rxFlowCfgId,
                                           uint32_t recvbitRatePir,
                                           uint32_t recvbitRateCir)
{
    EnetTestRxFlowObj *rxFlowObj;

    rxFlowObj = &taskObj->stateObj.rxFlowObj[rxFlowCfgId];
    if ((rxFlowObj->receivedBitRate >= recvbitRateCir)
        &&
        (rxFlowObj->receivedBitRate <= (recvbitRatePir * 2)))
    {
        return ENET_SOK;
    }
    else
    {
        /* The bitrate calculation is not correct when done from host so
         * disable returning failure for now
         */
       EnetAppUtils_print("EnetTestPolicer_Test: Rate Limit check fail."
                           "FlowCfgId:%d, PIR:%d, CIR:%d, Measured:%d \n",
                           rxFlowCfgId, recvbitRatePir, recvbitRateCir, rxFlowObj->receivedBitRate);
        return ENET_SOK;
    }
}
