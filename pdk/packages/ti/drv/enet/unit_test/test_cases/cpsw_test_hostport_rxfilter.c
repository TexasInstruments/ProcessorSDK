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
 * \file     cpsw_test_hostport_rxfilter.c
 *
 * \brief    This file contains the Host port rx filter test implementation.
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
#include "cpsw_test_hostport_rxfilter.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static void EnetTestRxFilter_validateRxFilter(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Multicast address */
static uint8_t mcastAddr[ENET_MAC_ADDR_LEN] =
{
    0x01U, 0xffU, 0xffU, 0xffU, 0xffU, 0x01U
};

/* nonRegisteredMcastAddr address */
static uint8_t nonRegisteredMcastAddr[ENET_MAC_ADDR_LEN] =
{
    0x01U, 0xffU, 0x00U, 0x00U, 0xffU, 0x02U
};

/* nonRegisteredUcastAddr address */
static uint8_t nonRegisteredUcastAddr[ENET_MAC_ADDR_LEN] =
{
    0x40U, 0xcdU, 0xefU, 0xfeU, 0xdcU, 0xbaU
};

/* Broadcast address */
static uint8_t bcastAddr[ENET_MAC_ADDR_LEN] =
{
    0xffU, 0xffU, 0xffU, 0xffU, 0xffU, 0xffU
};

typedef struct EnetTestRxFilterPktCounter_s
{
    uint32_t directCnt;
    uint32_t bcastCnt;
    uint32_t mcastCnt;
    uint32_t mcastAllCnt;
    uint32_t promiscousCnt;
} EnetTestRxFilterPktCounter_t;

EnetTestRxFilterPktCounter_t EnetTestRxFilterPktCounter;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestRxFilter_Run(EnetTestTaskObj *taskObj)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Enet_IoctlPrms prms;
    CpswAle_RxFilter setRxFilter;
    uint32_t rxReadyCnt, timeoutVal, elapsedTIme;

    /* For this test overwrite board host port mac address with standard UT
     * default mac addr so that test packet generation does not change
     * based on board
     */
    EnetTestCommon_getDefaultHostMacAddr(&stateObj->hostMacAddr[0U]);
    EnetTestRxFilter_validateRxFilter(taskObj);

    status = EnetTestCommon_setAleMulticastEntry(taskObj,
                                                 &mcastAddr[0],
                                                 0,  /* Vlan Id */
                                                 2,
                                                 (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK));

    EnetAppUtils_assert(status == ENET_SOK);
    setRxFilter = CPSW_ALE_RXFILTER_NOTHING;
    ENET_IOCTL_SET_IN_ARGS(&prms, &setRxFilter);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_RX_FILTER,
                        &prms);
    if (status == ENET_SOK)
    {
        EnetTestCommon_waitForPortLink(taskObj);

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

        /* Discard all queued packets before entering timeout loop
         * There maybe packets queued during the validateRxFilter
         * Rx filter transition
         */

       EnetAppUtils_assert(taskObj->taskCfg->numRxFlow == 1U);
        EnetTestCommon_discardRxPkts(taskObj, 0);

        timeoutVal  = 5000;
        elapsedTIme = 0;

        do
        {
            rxReadyCnt = EnetTestCommon_receivePkts(taskObj, 0U);
            if (rxReadyCnt != 0U)
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

    if (status == ENET_SOK)
    {
       EnetAppUtils_print("Rx Filter direct test\n");
        setRxFilter = CPSW_ALE_RXFILTER_DIRECT;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setRxFilter);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_RX_FILTER,
                            &prms);
        if (status == ENET_SOK)
        {
            memset(&EnetTestRxFilterPktCounter,0,sizeof(EnetTestRxFilterPktCounter));
            status = EnetTestCommon_pktTxRx(taskObj);
        }
    }

    return status;
}

static void EnetTestRxFilter_validateRxFilter(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_RxFilter setRxFilter;
    CpswAle_RxFilter getRxFilter;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    int32_t i;
    CpswAle_RxFilter rxFilterType[] = {CPSW_ALE_RXFILTER_NOTHING,
                                          CPSW_ALE_RXFILTER_DIRECT,
                                          CPSW_ALE_RXFILTER_BCAST,
                                          CPSW_ALE_RXFILTER_MCAST,
                                          CPSW_ALE_RXFILTER_ALLMCAST,
                                          CPSW_ALE_RXFILTER_ALL};

    for (i = 0; i < ENET_ARRAYSIZE(rxFilterType); i++)
    {
        setRxFilter = rxFilterType[i];

        ENET_IOCTL_SET_IN_ARGS(&prms, &setRxFilter);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_RX_FILTER,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

        ENET_IOCTL_SET_OUT_ARGS(&prms, &getRxFilter);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_GET_RX_FILTER,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

       EnetAppUtils_assert(getRxFilter == setRxFilter);
    }
}

void EnetTestRxFilter_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                        Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn           = BFALSE;
}

void EnetTestRxFilter_setOpenPrms(EnetTestTaskObj *taskObj,
                                  Cpsw_Cfg *pCpswCfg,
                                  EnetUtils_Cfg *pUtilsPrms)
{
    // VLAN configurations
    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode               = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanNoLearn             = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.unknownForceUntaggedEgressMask = 0U;
    pCpswCfg->aleCfg.vlanCfg.unknownRegMcastFloodMask       = (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK);
    pCpswCfg->aleCfg.vlanCfg.unknownUnregMcastFloodMask     = (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK);
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask      = (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK);
}

int32_t EnetTestRxFilter_processRxPkt(EnetTestTaskObj *taskObj,
                                      uint32_t rxFlowId,
                                      uint32_t pktNum,
                                      EnetDma_Pkt *pktInfo,
                                      bool *testComplete)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Enet_IoctlPrms prms;
    CpswAle_RxFilter setRxFilter, getRxFilter;
    uint32_t numIgnBits = 2U;
    uint32_t portMask   = (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK);
    uint32_t vlanId     = 0;
    EthFrame *frame;

    /* Consume the packet by just printing its content */
    frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;

#ifdef ENABLE_PRINTFRAME
   EnetAppUtils_printFrame
        (frame, (pktInfo->sgList.list[0].segmentFilledLen - sizeof(EthFrameHeader)));
#endif

    if (EnetTestRxFilterPktCounter.directCnt >= 1000)
    {
        //EnetAppUtils_print("Rx Filter Broadcast test\n");
        setRxFilter = CPSW_ALE_RXFILTER_BCAST;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setRxFilter);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_RX_FILTER,
                            &prms);
        if (status != ENET_SOK)
        {
            status = ENET_EFAIL;
        }

        EnetTestRxFilterPktCounter.directCnt = 0;
    }

    if (EnetTestRxFilterPktCounter.bcastCnt >= 1000)
    {
        //EnetAppUtils_print("Rx Filter registered mcast test\n");
        status = EnetTestCommon_setAleMulticastEntry(taskObj,
                                                     &mcastAddr[0],
                                                     vlanId,  /* Vlan Id */
                                                     numIgnBits,
                                                     portMask);
        if (ENET_SOK == status)
        {
            setRxFilter = CPSW_ALE_RXFILTER_MCAST;
            ENET_IOCTL_SET_IN_ARGS(&prms, &setRxFilter);
            status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_RX_FILTER,
                                &prms);
            if (status != ENET_SOK)
            {
                status = ENET_EFAIL;
            }
        }
        else
        {
            status = ENET_EFAIL;
        }

        EnetTestRxFilterPktCounter.bcastCnt = 0;
    }

    if (EnetTestRxFilterPktCounter.mcastCnt >= 1000)
    {
        //EnetAppUtils_print("Rx Filter all mcast test\n");
        setRxFilter = CPSW_ALE_RXFILTER_ALLMCAST;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setRxFilter);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_RX_FILTER,
                            &prms);
        if (status != ENET_SOK)
        {
            status = ENET_EFAIL;
        }

        EnetTestRxFilterPktCounter.mcastCnt = 0;
    }

    if (EnetTestRxFilterPktCounter.mcastAllCnt >= 1000)
    {
        //EnetAppUtils_print("Rx Filter promiscous mode test\n");
        setRxFilter = CPSW_ALE_RXFILTER_ALL;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setRxFilter);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_RX_FILTER,
                            &prms);
        if (status != ENET_SOK)
        {
            status = ENET_EFAIL;
        }

        EnetTestRxFilterPktCounter.mcastAllCnt = 0;
    }

    ENET_IOCTL_SET_OUT_ARGS(&prms, &getRxFilter);
    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_GET_RX_FILTER,
                        &prms);

    if (status == ENET_SOK)
    {
        switch (getRxFilter)
        {
            case CPSW_ALE_RXFILTER_DIRECT:
                if (memcmp(&frame->hdr.dstMac[0], &stateObj->hostMacAddr[0U], sizeof(frame->hdr.dstMac)) == 0)
                {
                    status = ENET_SOK;
                    EnetTestRxFilterPktCounter.directCnt++;
                }
                else
                {
                    status = ENET_EFAIL;
                }

                break;

            case CPSW_ALE_RXFILTER_BCAST:
                if ((memcmp(&frame->hdr.dstMac[0], &stateObj->hostMacAddr[0U], sizeof(frame->hdr.dstMac)) == 0) ||
                    (memcmp(&frame->hdr.dstMac[0], &bcastAddr[0U], sizeof(frame->hdr.dstMac)) == 0))
                {
                    status = ENET_SOK;
                    if (memcmp(&frame->hdr.dstMac[0], &bcastAddr[0U], sizeof(frame->hdr.dstMac)) == 0)
                    {
                        EnetTestRxFilterPktCounter.bcastCnt++;
                    }
                }
                else
                {
                    status = ENET_EFAIL;
                }

                break;

            case CPSW_ALE_RXFILTER_MCAST:
                if ((memcmp(&frame->hdr.dstMac[0], &stateObj->hostMacAddr[0U], sizeof(frame->hdr.dstMac)) == 0) ||
                    (memcmp(&frame->hdr.dstMac[0], &bcastAddr[0U], sizeof(frame->hdr.dstMac)) == 0) ||
                    (memcmp(&frame->hdr.dstMac[0], &mcastAddr[0U], sizeof(frame->hdr.dstMac)) == 0))
                {
                    status = ENET_SOK;
                    if (memcmp(&frame->hdr.dstMac[0], &mcastAddr[0U], sizeof(frame->hdr.dstMac)) == 0)
                    {
                        EnetTestRxFilterPktCounter.mcastCnt++;
                    }
                }
                else
                {
                    status = ENET_EFAIL;
                }

                break;

            case CPSW_ALE_RXFILTER_ALLMCAST:
                if ((memcmp(&frame->hdr.dstMac[0], &stateObj->hostMacAddr[0U], sizeof(frame->hdr.dstMac)) == 0) ||
                    ((frame->hdr.dstMac[0] & 0x1) == 0x1))
                {
                    if (memcmp(&frame->hdr.dstMac[0], &nonRegisteredMcastAddr[0U], sizeof(frame->hdr.dstMac)) == 0)
                    {
                        EnetTestRxFilterPktCounter.mcastAllCnt++;
                    }

                    status = ENET_SOK;
                }
                else
                {
                    status = ENET_EFAIL;
                }

                break;

            case CPSW_ALE_RXFILTER_ALL:
                /* For Rx filter ALL it is in promiscous mode and will receive all packets */
                status = ENET_SOK;
                if (memcmp(&frame->hdr.dstMac[0], &nonRegisteredUcastAddr[0U], sizeof(frame->hdr.dstMac)) == 0)
                {
                    EnetTestRxFilterPktCounter.promiscousCnt++;
                }

                break;

            default:
                status = ENET_EFAIL;
                break;
        }
    }

    if (EnetTestRxFilterPktCounter.promiscousCnt >= 1000)
    {
        *testComplete = BTRUE;
    }
    else
    {
        *testComplete = BFALSE;
    }

    return status;
}
