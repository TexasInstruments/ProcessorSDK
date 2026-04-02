/*
 * Copyright (C) 2024 Texas Instruments Incorporated
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
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
 */

/*!
 * \file  l2_networking.c
 *
 * \brief This file contains all functions related to layer 2 networking
 *        that is used by the CLI application
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "cli_common.h"
#include "l2_networking.h"
#include <enet_board.h>
#include <enet_apputils.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENETAPP_VLAN_PCP_OFFSET                      (13U)
#define ENETAPP_VLAN_PCP_MASK                        (0x7U)
#define ENETAPP_VLAN_DEI_OFFSET                      (12U)
#define ENETAPP_VLAN_DEI_MASK                        (0x1U)
#define ENETAPP_VLAN_VID_MASK                        (0xFFFU)
#define ENETAPP_VLAN_TCI(pcp, dei, vid)              ((((pcp) & ENETAPP_VLAN_PCP_MASK) << ENETAPP_VLAN_PCP_OFFSET) | \
                                                      (((dei) & ENETAPP_VLAN_DEI_MASK) << ENETAPP_VLAN_DEI_OFFSET) | \
                                                      (((vid) & ENETAPP_VLAN_VID_MASK)))
#define ENETAPP_TEST_PKT_LEN                         (500U)

/* When Phy linkup is not available, this macro tell us
*  how many times should we poll and wait for linkup.
*  A value of 150 means we wait a total of 150 seconds. */
#define MAX_NUM_OF_TRIES                             (150U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
 
 /*None*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t EnetApp_waitForLinkUp(Enet_MacPort macPort);

static void EnetApp_initTxFreePktQ(int8_t dmaChNum);

static int32_t EnetApp_openTxCh(uint8_t dmaChNum);

static int32_t EnetApp_openRxCh(uint8_t dmaChNum);

static uint32_t EnetApp_retrieveFreeTxPkts(int8_t dmaChNum);

static int32_t EnetApp_createRxTask(int8_t dmaChNum);

static void EnetApp_recievePkt(void *arg0, void *arg1);

static uint32_t EnetApp_retrieveRxPkts(uint8_t dmaChNum);

static void EnetApp_close(void);

static void EnetApp_printFrame(EthFrame *frame, uint32_t len);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Buffer to store packets received from ethernet */
EthFrame *EnetApp_rxBuffer[ENET_RX_FLOWS_NUM][4];
int32_t EnetApp_payloadLen[ENET_RX_FLOWS_NUM][4];

/* Index of buffer */
int8_t EnetApp_writeBufferPt[ENET_RX_FLOWS_NUM];

static uint8_t EnetApp_rxTaskStack[ENET_RX_FLOWS_NUM][10U * 1024U] __attribute__ ((aligned(32)));

uint32_t rxScatterSegments[] = 
{

#if (ENETAPP_SCATTER_GATHER_ENABLE_RX != 1U)
    [0] = (ENETAPP_TEST_PKT_LEN + sizeof(EthFrameHeader)),
#else
    [0] = (ENETAPP_TEST_PKT_LEN + sizeof(EthFrameHeader)),
    [1] = (ENETAPP_TEST_PKT_LEN / 3),
    [2] = (ENETAPP_TEST_PKT_LEN / 3),
    [3] = ((ENETAPP_TEST_PKT_LEN / 3) + (ENETAPP_TEST_PKT_LEN % 3) + 32),
#endif
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

BaseType_t EnetCLI_openTxChn(char *writeBuffer,
                             size_t writeBufferLen,
                             const char *commandString)
{
    int32_t status = ENET_SOK;
    uint8_t chNum;
    char *parameter;
    BaseType_t paramLen;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);

    if (parameter == NULL)
    {
        snprintf(writeBuffer, writeBufferLen, "No Tx Channel specified\r\n");
        status = ENET_EBADARGS;
    }
    else
    {
        chNum = atoi(parameter);
        if (EnetApp_inst.txDmaCh[chNum] != CH_CLOSE)
        {
            snprintf(writeBuffer, writeBufferLen,
                    "Tx channel %d is already open\r\n", chNum);
            return pdFALSE;
        }

        /* Open Tx channel */
        status = EnetApp_openTxCh(chNum);
        if (status != ENET_SOK)
            snprintf(writeBuffer, writeBufferLen,
                    "Failed to open new Tx channel\r\n");
        else
            snprintf(writeBuffer, writeBufferLen,
                    "Opened new Tx channel with identifier %d\r\n", chNum);
    }
    return pdFALSE;
}

BaseType_t EnetCLI_openRxChn(char *writeBuffer,
                             size_t writeBufferLen,
                             const char *commandString)
{
    int32_t status = ENET_SOK;
    uint8_t chNum;
    char *parameter;
    BaseType_t paramLen;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);

    if (parameter == NULL)
    {
        snprintf(writeBuffer, writeBufferLen, "No Tx Channel specified\r\n");
        status = ENET_EBADARGS;
    }
    else
    {
        chNum = atoi(parameter);
        if ((EnetApp_inst.rxDmaCh[chNum] != CH_CLOSE) || (chNum == 0))
        {
            snprintf(writeBuffer, writeBufferLen,
                    "Rx channel %d is already open\r\n", chNum);
            return pdFALSE;
        }
        else if (chNum < 0 || chNum >= ENET_RX_FLOWS_NUM)
        {
            snprintf(writeBuffer, writeBufferLen,
                    "Rx channel incorrect. Channel Number must be between 1 and %d (inclusive) \r\n", ENET_RX_FLOWS_NUM-1);
            return pdFALSE;
        }

        /* Open Rx channel */
        status = EnetApp_openRxCh(chNum);
        if (status != ENET_SOK)
            snprintf(writeBuffer, writeBufferLen, "Failed to open Rx channel\r\n");
        else
            snprintf(writeBuffer, writeBufferLen,
                    "Opened new Rx channel with identifier %d\r\n", chNum);
    }
    return pdFALSE;
}

BaseType_t EnetCLI_getHostMac(char *writeBuffer,
                              size_t writeBufferLen,
                              const char *commandString)
{
    EnetAppUtils_print("Host MAC address: ");
    EnetAppUtils_printMacAddr(EnetApp_inst.hostMacAddr);
    return pdFALSE;
}

BaseType_t EnetCLI_capturePkt(char *writeBuffer,
                              size_t writeBufferLen,
                              const char *commandString)
{
    int32_t status = ENET_SOK;
    char *parameter;
    BaseType_t paramLen;
    int8_t dmaChNum;
    bool isStart = BTRUE;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    if (parameter == NULL)
    {
        snprintf(writeBuffer, writeBufferLen, "Bad Argument, please see help for correct usage.\r\n");
        status = ENET_EBADARGS;
    }
    if ((status == ENET_SOK) && (strncmp(parameter, "start", paramLen) == 0))
        isStart = BTRUE;
    else if ((status == ENET_SOK) && (strncmp(parameter, "stop", paramLen) == 0))
        isStart = BFALSE;
    else
    {
        snprintf(writeBuffer, writeBufferLen, "Invalid Arg\r\n");
        return pdFALSE;
    }
    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 2, &paramLen);
    if (parameter == NULL)
    {
        snprintf(writeBuffer, writeBufferLen, "DMA Channel Not Specified.\r\n");
        status = ENET_EBADARGS;
    }
    else
    {
        dmaChNum = atoi(parameter);

        if (dmaChNum == 0)
        {
            /* The channel number at index 0 is always open since
            *  it is the default flow. */
            EnetApp_inst.rxDmaCh[0] = CH_IDLE;
        }

        if (dmaChNum < 0 || dmaChNum >= ENET_RX_FLOWS_NUM)
        {
            snprintf(writeBuffer, writeBufferLen, "Invalid Rx channel\r\n");
            return pdFALSE;
        }
        else if (EnetApp_inst.rxDmaCh[dmaChNum] == CH_CLOSE)
        {
            snprintf(writeBuffer, writeBufferLen, "Rx channel %d is not open\r\n",
                    dmaChNum);
            return pdFALSE;
        }

        if (isStart)
        {
            if (EnetApp_inst.rxDmaCh[dmaChNum] != CH_IDLE)
            {
                snprintf(writeBuffer, writeBufferLen,
                        "Rx channel %d is already listening for packets\r\n",
                        dmaChNum);
                return pdFALSE;
            }

            status = EnetApp_createRxTask(dmaChNum);
            if (status != ENET_SOK)
                snprintf(writeBuffer, writeBufferLen,
                        "Failed to start Rx task\r\n");
            else
                snprintf(writeBuffer, writeBufferLen,
                        "Listening to packets at Rx channel %d\r\n", dmaChNum);
        }
        else
        {
            if ((dmaChNum > 0) && EnetApp_inst.rxDmaCh[dmaChNum] == CH_IDLE)
            {
                snprintf(writeBuffer, writeBufferLen,
                        "Rx channel %d is not listening to packets\r\n", dmaChNum);
                return pdFALSE;
            }

            EnetApp_inst.rxDmaCh[dmaChNum] = CH_STOP;

            /* Wait for Rx channel to stop listening to incoming packets */
            while (EnetApp_inst.rxDmaCh[dmaChNum] != CH_IDLE)
            {
                TaskP_sleepInMsecs(500);
            }

            snprintf(writeBuffer, writeBufferLen,
                    "Rx channel %d stopped listening to packets\r\n", dmaChNum);
        }
    }
    return pdFALSE;
}

BaseType_t EnetCLI_dumpRxBuffer(char *writeBuffer,
                                size_t writeBufferLen,
                                const char *commandString)
{
    int8_t dmaChNum = 0;
    static int8_t rxBufferPt = -1;
    static int8_t readStartPt = -1;
    char *parameter;
    BaseType_t paramLen;
    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    if (parameter == NULL)
    {
        snprintf(writeBuffer, writeBufferLen, "Bad Arguments, check help for correct usage.\r\n");
    }
    else
    {
        dmaChNum = atoi(parameter);

        if (EnetApp_inst.rxDmaCh[dmaChNum] == CH_CLOSE)
        {
            snprintf(writeBuffer, writeBufferLen, "Rx channel %d is not open\r\n",
                    dmaChNum);
            return pdFALSE;
        }

        if (rxBufferPt == -1)
        {
            rxBufferPt = EnetApp_writeBufferPt[dmaChNum];
            readStartPt = rxBufferPt;
        }
        if (EnetApp_rxBuffer[dmaChNum][rxBufferPt] != NULL)
        {
            snprintf(writeBuffer, writeBufferLen,
                    "------PACKET ENDS HERE------\r\n\n");
            EnetApp_printFrame(EnetApp_rxBuffer[dmaChNum][rxBufferPt],
                    EnetApp_payloadLen[dmaChNum][rxBufferPt]);
        }
    }
    rxBufferPt = rxBufferPt == 3 ? 0 : rxBufferPt + 1;
    if (rxBufferPt == readStartPt)
    {
        rxBufferPt = -1;
        return pdFALSE;
    }
    else
    {
        return pdTRUE;
    }
}

/* ========================================================================== */
/*                   Static Function Definitions                              */
/* ========================================================================== */

static int32_t EnetApp_waitForLinkUp(Enet_MacPort macPort)
{
    uint8_t timeout = 0;
    Enet_IoctlPrms prms;
    bool linked = BFALSE;
    int32_t status = ENET_SOK;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPort, &linked);

    while (!linked)
    {
        status = Enet_ioctl(EnetApp_inst.hEnet,
                            EnetApp_inst.coreId,
                            ENET_PER_IOCTL_IS_PORT_LINK_UP, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print(
                    "[WRN] %s: Failed to get port %u's link status: %d\r\n",
                    __func__, ENET_MACPORT_ID(macPort), status);
            linked = BFALSE;
            break;
        }

        if (timeout >= MAX_NUM_OF_TRIES)
        {
            break;
        }
        if (!linked)
        {
            /* wait for 1s and poll again*/
            EnetUtils_delay(1U);
            timeout++;
        }
    }
    return status;
}

static void EnetApp_initTxFreePktQ(int8_t dmaChNum)
{
    EnetDma_Pkt *pPktInfo;
    uint32_t i;
    uint32_t scatterSegments[] = {
    ENET_MEM_LARGE_POOL_PKT_SIZE, };

    #if ENET_TX_CHANNELS_NUM >= 1
        EnetApp_pktPerTxCh[0] = ENET_DMA_TX_CH0_NUM_PKTS;
    #endif
    #if ENET_TX_CHANNELS_NUM >= 2
        EnetApp_pktPerTxCh[1] = ENET_DMA_TX_CH1_NUM_PKTS;
    #endif
    #if ENET_TX_CHANNELS_NUM >= 3
        EnetApp_pktPerTxCh[2] = ENET_DMA_TX_CH2_NUM_PKTS;
    #endif
    #if ENET_TX_CHANNELS_NUM >= 4
        EnetApp_pktPerTxCh[3] = ENET_DMA_TX_CH3_NUM_PKTS;
    #endif

    /* Initialize all queues */
    EnetQueue_initQ(&EnetApp_inst.txFreePktInfoQ[dmaChNum]);

    /* Initialize TX EthPkts and queue them to txFreePktInfoQ */
    for (i = 0U; i < ENET_MEM_NUM_TX_PKTS; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&EnetApp_inst,
                ENETDMA_CACHELINE_ALIGNMENT, ENET_ARRAYSIZE(scatterSegments),
                scatterSegments);
        EnetAppUtils_assert(pPktInfo != NULL);
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState,
                ENET_PKTSTATE_APP_WITH_FREEQ);

        EnetQueue_enq(&EnetApp_inst.txFreePktInfoQ[dmaChNum], &pPktInfo->node);
    }
}

static int32_t EnetApp_openTxCh(uint8_t dmaChNum)
{
    int32_t status = ENET_SOK;
    EnetUdma_OpenTxChPrms   txChCfg;
    SemaphoreP_Params params;

    params.mode = SemaphoreP_Mode_BINARY;
    EnetApp_inst.txSemObj[dmaChNum] = SemaphoreP_create(0, &params);

    /* Open the CPSW TX channel  */
    if (status == ENET_SOK)
    {
        EnetUdma_initTxChParams(&txChCfg);

        txChCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;
        txChCfg.cbArg    = &EnetApp_inst.txSemObj[dmaChNum];
        txChCfg.notifyCb = EnetApp_txIsrFxn;

        EnetAppUtils_setCommonTxChPrms(&txChCfg);

        EnetAppUtils_openTxCh(EnetApp_inst.hEnet,
                              EnetApp_inst.coreKey,
                              EnetApp_inst.coreId,
                              &EnetApp_inst.txChNum,
                              &EnetApp_inst.hTxCh[dmaChNum],
                              &txChCfg);
        EnetDma_enableTxEvent(EnetApp_inst.hTxCh[dmaChNum]);
        EnetApp_initTxFreePktQ(dmaChNum);

        if (NULL != EnetApp_inst.hTxCh[dmaChNum])
        {
            status = EnetDma_enableTxEvent(EnetApp_inst.hTxCh[dmaChNum]);
            if (ENET_SOK != status)
            {
                EnetAppUtils_print("EnetUdma_startTxCh() failed: %d\n", status);
                status = ENET_EFAIL;
            }
        }
        else
        {
            EnetAppUtils_print("EnetUdma_openTxCh() failed to open: %d\n",
                               status);
            status = ENET_EFAIL;
            return status;
        }
    }
    EnetApp_inst.txDmaCh[dmaChNum] = CH_IDLE;
    return status;
}

static int32_t EnetApp_openRxCh(uint8_t dmaChNum)
{
    int32_t status = ENET_SOK;
    EnetUdma_OpenRxFlowPrms rxChCfg;
    SemaphoreP_Params params;

    params.mode = SemaphoreP_Mode_BINARY;
    EnetApp_inst.rxSemObj[dmaChNum] = SemaphoreP_create(0, &params);

    /* Open the CPSW RX flow  */
    if (status == ENET_SOK)
    {
        EnetUdma_initRxFlowParams(&rxChCfg);

        rxChCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;
        rxChCfg.notifyCb = EnetApp_rxIsrFxn;
        rxChCfg.cbArg   = (void *)&EnetApp_inst.rxSemObj[dmaChNum];

        EnetAppUtils_setCommonRxFlowPrms(&rxChCfg);
        EnetAppUtils_openRxFlow(EnetApp_inst.enetType,
                                EnetApp_inst.hEnet,
                                EnetApp_inst.coreKey,
                                EnetApp_inst.coreId,
                                BFALSE,
                                &EnetApp_inst.rxStartFlowIdx,
                                &EnetApp_inst.rxFlowIdx,
                                &EnetApp_inst.hostMacAddr[0U],
                                &EnetApp_inst.hRxCh[dmaChNum],
                                &rxChCfg);
        EnetDma_enableRxEvent(EnetApp_inst.hRxCh[dmaChNum]);

        if (NULL == EnetApp_inst.hRxCh[dmaChNum])
        {
            EnetAppUtils_print("EnetUdma_openRxFlow() failed to open: %d\n",
                               status);
            EnetAppUtils_assert(NULL != EnetApp_inst.hRxCh[dmaChNum]);
        }
        else
        {
            EnetAppUtils_print("Host MAC address: ");
            EnetAppUtils_printMacAddr(EnetApp_inst.hostMacAddr);
            /* Submit all ready RX buffers to DMA.*/
            EnetApp_initRxReadyPktQ(dmaChNum);
        }
    }

    EnetApp_inst.rxDmaCh[dmaChNum] = CH_IDLE;
    return status;
}

static uint32_t EnetApp_retrieveFreeTxPkts(int8_t dmaChNum)
{
    EnetDma_PktQ txFreeQ;
    EnetDma_Pkt *pktInfo;
    int32_t status;
    uint32_t txFreeQCnt = 0U;

    EnetQueue_initQ(&txFreeQ);

    /* Retrieve any CPSW packets that may be free now */
    status = EnetDma_retrieveTxPktQ(EnetApp_inst.hTxCh[dmaChNum], &txFreeQ);
    if (status == ENET_SOK)
    {
        txFreeQCnt = EnetQueue_getQCount(&txFreeQ);

        pktInfo = (EnetDma_Pkt*) EnetQueue_deq(&txFreeQ);
        while (NULL != pktInfo)
        {
            EnetDma_checkPktState(&pktInfo->pktState,
                                  ENET_PKTSTATE_MODULE_APP,
                                  ENET_PKTSTATE_APP_WITH_DRIVER,
                                  ENET_PKTSTATE_APP_WITH_FREEQ);

            EnetQueue_enq(&EnetApp_inst.txFreePktInfoQ[dmaChNum],
                          &pktInfo->node);
            pktInfo = (EnetDma_Pkt*) EnetQueue_deq(&txFreeQ);
        }
    }
    else
    {
        EnetAppUtils_print("[ERR] %s: Failed to retrieve pkts: %d\r\n",
                __func__, status);
    }

    return txFreeQCnt;
}

static int32_t EnetApp_createRxTask(int8_t dmaChNum)
{
    TaskP_Params taskParams;
    TaskP_Handle task;
    EnetApp_inst.rxDmaCh[dmaChNum] = CH_RUNNING;
    TaskP_Params_init(&taskParams);
    taskParams.priority = 3U;
    taskParams.stack = EnetApp_rxTaskStack[dmaChNum];
    taskParams.stacksize = sizeof(EnetApp_rxTaskStack[dmaChNum]);
    taskParams.arg0 = (void*) &dmaChNum;
    taskParams.name = "Rx Task";
    uint32_t status = ENET_SOK;

    task = TaskP_create(&EnetApp_recievePkt, &taskParams);
    
    if (task == NULL)
    {
        EnetAppUtils_print("[ERR] %s: Failed to create Rx task %d\r\n",
                __func__, task);
        status = ENET_EBADARGS;
        return status;
    }
    else
    {
        status = ENET_SOK;
        return status;
    }
}

static void EnetApp_recievePkt(void *arg0, void *arg1)
{
    EnetDma_Pkt *pktInfo;
    uint32_t rxReadyCnt;
    int32_t status = ENET_SOK;
    uint32_t rxPktCnt = 0;
    int8_t dmaChNum = *(int8_t*) arg0;
    while (EnetApp_inst.rxDmaCh[dmaChNum] != CH_STOP)
    {
        status = SemaphoreP_pend(EnetApp_inst.rxSemObj[dmaChNum], 500);

        if (status != ENET_SOK)
            continue;

        /* Get the packets received so far */
        rxReadyCnt = EnetApp_retrieveRxPkts(dmaChNum);
        if (rxReadyCnt > 0U)
        {
            /* Consume the received packets and release them */
            pktInfo = (EnetDma_Pkt*) EnetQueue_deq(
                    &EnetApp_inst.rxReadyQ[dmaChNum]);
            while (NULL != pktInfo)
            {
                rxPktCnt++;
                EnetDma_checkPktState(&pktInfo->pktState,
                                      ENET_PKTSTATE_MODULE_APP,
                                      ENET_PKTSTATE_APP_WITH_READYQ,
                                      ENET_PKTSTATE_APP_WITH_FREEQ);

                /* Store payload to rx buffer */
                EnetApp_rxBuffer[dmaChNum][EnetApp_writeBufferPt[dmaChNum]] =
                        (EthFrame*) pktInfo->sgList.list[0].bufPtr;
                EnetApp_payloadLen[dmaChNum][EnetApp_writeBufferPt[dmaChNum]] =
                        pktInfo->sgList.list[0].segmentFilledLen
                                - sizeof(EthFrameHeader);

                EnetApp_writeBufferPt[dmaChNum] =
                        EnetApp_writeBufferPt[dmaChNum] == 3 ?
                                0 : EnetApp_writeBufferPt[dmaChNum] + 1;

                EnetQueue_enq(&EnetApp_inst.rxFreeQ[dmaChNum], &pktInfo->node);
                pktInfo = (EnetDma_Pkt*) EnetQueue_deq(
                        &EnetApp_inst.rxReadyQ[dmaChNum]);
            }

            /*Submit now processed buffers */
            if (status == ENET_SOK)
            {
                EnetAppUtils_validatePacketState(
                        &EnetApp_inst.rxFreeQ[dmaChNum],
                        ENET_PKTSTATE_APP_WITH_FREEQ,
                        ENET_PKTSTATE_APP_WITH_DRIVER);

                EnetDma_submitRxPktQ(EnetApp_inst.hRxCh[dmaChNum],
                                     &EnetApp_inst.rxFreeQ[dmaChNum]);
            }
        }
    }
    EnetAppUtils_print(
            "Stopped capturing at Rx channel %d. Recieved %d packets\r\n",
            dmaChNum, rxPktCnt);
    EnetApp_inst.rxDmaCh[dmaChNum] = CH_IDLE;
}

static uint32_t EnetApp_retrieveRxPkts(uint8_t dmaChNum)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pktInfo;
    int32_t status;
    uint32_t rxReadyCnt = 0U;

    EnetQueue_initQ(&rxReadyQ);

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(EnetApp_inst.hRxCh[dmaChNum], &rxReadyQ);
    if (status == ENET_SOK)
    {
        rxReadyCnt = EnetQueue_getQCount(&rxReadyQ);

        /* Queue the received packet to rxReadyQ and pass new ones from rxFreeQ */
        pktInfo = (EnetDma_Pkt*) EnetQueue_deq(&rxReadyQ);
        while (pktInfo != NULL)
        {
            EnetDma_checkPktState(&pktInfo->pktState,
                                  ENET_PKTSTATE_MODULE_APP,
                                  ENET_PKTSTATE_APP_WITH_DRIVER,
                                  ENET_PKTSTATE_APP_WITH_READYQ);

            EnetQueue_enq(&EnetApp_inst.rxReadyQ[dmaChNum], &pktInfo->node);
            pktInfo = (EnetDma_Pkt*) EnetQueue_deq(&rxReadyQ);
        }
    }
    else
    {
        EnetAppUtils_print("[ERR] %s: Failed to retrieve pkts: %d\r\n",
                            __func__, status);
    }

    return rxReadyCnt;
}

static void EnetApp_close(void)
{
    /* if TSN stack is still running, stop it first */
    if (EnetApp_inst.tsnFlag == 1)
    {
        Enetcli_write("Stopping TSN stack...\r\n");
    }

    /* Close Enet DMA driver */
    Enetcli_write("Closing DMA channels...\r\n");
    EnetApp_closeDma();

    /*Release Handle Info*/
    Enetcli_write("Releasing handles...\r\n");
    EnetApp_inst.hEnet = NULL;

    Enetcli_write("Deinitializing drives...\r\n");
    EnetMem_deInit();
    Enet_deinit();

    /* Disable peripheral clocks */
    Enetcli_write("Diabling clocks...\r\n");
    EnetAppUtils_disableClocks(EnetApp_inst.enetType, EnetApp_inst.instId);
}

static void EnetApp_printFrame(EthFrame *frame, uint32_t len)
{
    uint8_t *payload;
    uint32_t i;
    char buffer[50] = "";

    Enetcli_write("Dst addr : ");
    sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
            frame->hdr.dstMac[0] & 0xFF, frame->hdr.dstMac[1] & 0xFF,
            frame->hdr.dstMac[2] & 0xFF, frame->hdr.dstMac[3] & 0xFF,
            frame->hdr.dstMac[4] & 0xFF, frame->hdr.dstMac[5] & 0xFF);
    Enetcli_write(buffer);

    Enetcli_write("Src addr : ");
    sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
            frame->hdr.srcMac[0] & 0xFF, frame->hdr.srcMac[1] & 0xFF,
            frame->hdr.srcMac[2] & 0xFF, frame->hdr.srcMac[3] & 0xFF,
            frame->hdr.srcMac[4] & 0xFF, frame->hdr.srcMac[5] & 0xFF);
    Enetcli_write(buffer);

    if (frame->hdr.etherType == Enet_htons(ETHERTYPE_VLAN_TAG))
    {
        EthVlanFrame *vlanFrame = (EthVlanFrame*) frame;

        sprintf(buffer, "TPID     : 0x%04x\r\n",
                (Enet_ntohs(vlanFrame->hdr.tpid) & 0xFFFFU));
        Enetcli_write(buffer);
        sprintf(buffer, "Priority : %d\r\n",
                (Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFFU) >> 13);
        Enetcli_write(buffer);
        sprintf(buffer, "VLAN Id  : %d\r\n",
                (Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFU));
        Enetcli_write(buffer);
        sprintf(buffer, "EtherType: 0x%04x\r\n",
                (Enet_ntohs(vlanFrame->hdr.etherType) & 0xFFFFU));
        Enetcli_write(buffer);
        payload = vlanFrame->payload;
        len -= ETH_VLAN_TAG_LEN;
    }
    else
    {
        sprintf(buffer, "EtherType: 0x%04x\r\n",
                (Enet_ntohs(frame->hdr.etherType) & 0xFFFFU));
        Enetcli_write(buffer);
        payload = frame->payload;
    }

    Enetcli_write("Payload  : ");
    for (i = 0; i < len; i++)
    {
        sprintf(buffer, "0x%02x ", payload[i]);
        Enetcli_write(buffer);
        if (i && (((i + 1) % OCTETS_PER_ROW) == 0))
        {
            Enetcli_write("\r\n           ");
        }
    }

    if (len && ((len % OCTETS_PER_ROW) != 0))
    {
        Enetcli_write("\r\n");
    }

    Enetcli_write("\r\n");
}