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
 * \file     enet_switching_logic.c
 *
 * \brief    This file contains the enet_switching_logic test implementation.
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
#include "cpsw_test_fifostats.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define ENET_TEST_FIFO_STATS_NUM_STATS_ENTRY                             (512)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */
typedef struct EnetTestFifoStats_s
{
    uint32_t packetCount;
    CpswHostPort_FifoStats hostportFifoStats;
    CpswMacPort_FifoStats macportFifoStats;
} EnetTestFifoStats_t;

typedef struct EnetTestFifoStatsTable_s
{
    uint32_t tableIndex;
    EnetTestFifoStats_t EnetTestFifoStats[ENET_TEST_FIFO_STATS_NUM_STATS_ENTRY];
} EnetTestFifoStatsTable_t;
/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

EnetTestFifoStatsTable_t EnetTestFifoStatsTable;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static void EnetTestFifoStats_getFifoStats(EnetTestTaskObj *taskObj,
                                           EnetTestFifoStatsTable_t *pfifoStatsTbl,
                                           uint32_t packetCount)
{
    int32_t status;
    Enet_IoctlPrms hostprms;
    Enet_IoctlPrms macprms;
    EnetMacPort_GenericInArgs macPortInArgs;
    EnetTestMacPortList_t enabledPorts;

    if (pfifoStatsTbl->tableIndex < ENET_ARRAYSIZE(pfifoStatsTbl->EnetTestFifoStats))
    {
        ENET_IOCTL_SET_OUT_ARGS(&hostprms, &pfifoStatsTbl->EnetTestFifoStats[pfifoStatsTbl->tableIndex].hostportFifoStats);

        status =
            Enet_ioctl(taskObj->stateObj.hEnet, taskObj->stateObj.coreId, CPSW_HOSTPORT_IOCTL_GET_FIFO_STATS,
                       &hostprms);
        EnetAppUtils_assert(status == ENET_SOK);

        EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
        /* Take any port from enabled ports */
        macPortInArgs.macPort = enabledPorts.macPortList[0];

        ENET_IOCTL_SET_INOUT_ARGS(&macprms, &macPortInArgs, &pfifoStatsTbl->EnetTestFifoStats[pfifoStatsTbl->tableIndex].macportFifoStats);
        status =
            Enet_ioctl(taskObj->stateObj.hEnet, taskObj->stateObj.coreId, CPSW_MACPORT_IOCTL_GET_FIFO_STATS,
                       &macprms);
        pfifoStatsTbl->EnetTestFifoStats[pfifoStatsTbl->tableIndex].packetCount = packetCount;
        pfifoStatsTbl->tableIndex++;
    }
}

static void EnetTestFifoStats_initStatsTable(EnetTestFifoStatsTable_t *pfifoStatsTbl)
{
    pfifoStatsTbl->tableIndex = 0;
}

void EnetTestFifoStats_printFifoStats(EnetTestFifoStatsTable_t *pfifoStatsTbl)
{
    uint32_t i;

    for (i = 0; i < pfifoStatsTbl->tableIndex; i++)
    {
       EnetAppUtils_print(" --------FIFO Stats at Packet #:%u------\n", pfifoStatsTbl->EnetTestFifoStats[i].packetCount);
       EnetAppUtils_print(" --------Host Port------\n");
       EnetAppUtils_print(" RxThroughputRate:%d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].hostportFifoStats.rxThroughputRate);
       EnetAppUtils_print("FIFO Active status priority 0: %d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].hostportFifoStats.txActiveFifo[0]);
       EnetAppUtils_print("FIFO Active status priority 1: %d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].hostportFifoStats.txActiveFifo[1]);
       EnetAppUtils_print("FIFO Active status priority 2: %d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].hostportFifoStats.txActiveFifo[2]);
       EnetAppUtils_print("FIFO Active status priority 3: %d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].hostportFifoStats.txActiveFifo[3]);
       EnetAppUtils_print("FIFO Active status priority 4: %d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].hostportFifoStats.txActiveFifo[4]);
       EnetAppUtils_print("FIFO Active status priority 5: %d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].hostportFifoStats.txActiveFifo[5]);
       EnetAppUtils_print("FIFO Active status priority 6: %d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].hostportFifoStats.txActiveFifo[6]);
       EnetAppUtils_print("FIFO Active status priority 7: %d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].hostportFifoStats.txActiveFifo[7]);

       EnetAppUtils_print(" \n\n--------Port %d------\n", ENET_MAC_PORT_1 + 1);
       EnetAppUtils_print(" RxThroughputRate:%d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].macportFifoStats.rxThroughputRate);
       EnetAppUtils_print(" RxPreemptBlockCount:%d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].macportFifoStats.rxPreemptBlockCount);
       EnetAppUtils_print(" RxPreemptBlockCount:%d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].macportFifoStats.rxPreemptBlockCount);
       EnetAppUtils_print(" RxExpressBlockCount:%d \n",
                           pfifoStatsTbl->EnetTestFifoStats[i].macportFifoStats.rxExpressBlockCount);
       EnetAppUtils_print(" TxBlockCount:%d \n", pfifoStatsTbl->EnetTestFifoStats[i].macportFifoStats.txBlockCount);
       EnetAppUtils_print(" RxMaxBlocks:%d \n", pfifoStatsTbl->EnetTestFifoStats[i].macportFifoStats.rxMaxBlocks);
    }
}

int32_t EnetTestFifoStats_Run(EnetTestTaskObj *taskObj)
{
    int32_t status;

    EnetTestFifoStats_initStatsTable(&EnetTestFifoStatsTable);
   EnetAppUtils_print("\n\n----------------FIFO stats before RX TX---------------- \n");
    EnetTestFifoStats_getFifoStats(taskObj, &EnetTestFifoStatsTable, 0);
    EnetTestFifoStats_printFifoStats(&EnetTestFifoStatsTable);

    status = EnetTestCommon_Run(taskObj);
    if (status == ENET_SOK)
    {
        EnetTestFifoStats_printFifoStats(&EnetTestFifoStatsTable);
    }

    return status;
}

int32_t EnetTestFifoStats_processRxPkt(EnetTestTaskObj *taskObj,
                                       uint32_t rxFlowId,
                                       uint32_t pktNum,
                                       EnetDma_Pkt *pktInfo,
                                       bool *testComplete)
{
#ifdef ENABLE_PRINTFRAME
    EthFrame *frame;

    /* Consume the packet by just printing its content */
    frame = (EthFrame *)pktInfo->bufPtr;
   EnetAppUtils_printFrame
        (frame, (pktInfo->userBufLen - sizeof(EthFrameHeader)));
#endif

    if ((pktNum % 100U) == 0U)
    {
        EnetTestFifoStats_getFifoStats(taskObj, &EnetTestFifoStatsTable, pktNum);
    }

    *testComplete = BFALSE;
    return ENET_SOK;
}

void EnetTestFifoStats_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                         Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    /* This test also exercises a different code path PHY present with mac loopback, hence we don'tableIndex
     * set phy config to NO PHY mode */
    pLinkArgs->mii.variantType = ENET_MAC_VARIANT_FORCED;
    macCfg->loopbackEn = BTRUE;
}
