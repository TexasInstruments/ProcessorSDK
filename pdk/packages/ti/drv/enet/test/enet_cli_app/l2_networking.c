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
#include "cli_lwip.h"
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
#ifndef SIM_BUILD
#define ENETLPBK_TEST_PKT_NUM                      (1000U)
#else
#define ENETLPBK_TEST_PKT_NUM                      (20U)
#endif
/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
 
 /*None*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t EnetApp_waitForLinkUp(Enet_MacPort macPort);

static void EnetApp_txIsrFxn(void *appData);

static void EnetApp_rxIsrFxn(void *appData);

static void EnetApp_initTxFreePktQ(EnetQ* dmaChannel);

static void EnetApp_initRxReadyPktQ(EnetQ* dmaChannelFreeQ, EnetQ* dmaChannelReadyQ, EnetDma_RxChHandle rxHandle, int32_t numPacketsPerRXChnl);

static int32_t EnetApp_openTxCh(uint8_t dmaChNum);

static int32_t EnetApp_openRxCh(uint8_t dmaChNum);

static int32_t EnetApp_transmitPkt(char *payload, uint8_t *destMacAddr,
        uint8_t *srcMacAddr, uint16_t vlanId, uint8_t priority, int8_t dmaChNum);

static uint32_t EnetApp_retrieveFreeTxPkts(int8_t dmaChNum);

static int32_t EnetApp_createRxTask(int8_t dmaChNum);

static int32_t EnetApp_recievePkt(int8_t dmaChNum);

static uint32_t EnetApp_retrieveRxPkts(uint8_t dmaChNum);

static void EnetApp_close(void);

static void EnetApp_closeDma(void);

static void EnetApp_printFrame(EthFrame *frame, uint32_t len);

static void EnetApp_initCpswCfg(Cpsw_Cfg *cpswCfg);

static int32_t EnetApp_setupCpswAle(void);

static int32_t EnetApp_showAlivePhys(void);

static void EnetLpbk_showCpswStats(void);

int32_t EnetApp_openDma(uint8_t dmaChNum);

static void EnetApp_closeEnet(void);

static void EnetApp_deleteRxTxTasks(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Link speed default 100Mbps */
Enet_Speed EnetApp_linkSpeed = 1;

/* Buffer to store packets received from ethernet */
EthFrame *EnetApp_rxBuffer[ENET_SYSCFG_RX_FLOWS_NUM][4];
int32_t EnetApp_payloadLen[ENET_SYSCFG_RX_FLOWS_NUM][4];

/* Index of buffer */
int8_t EnetApp_writeBufferPt[ENET_SYSCFG_RX_FLOWS_NUM];

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

BaseType_t EnetCLI_openTxChn(char *writeBuffer, size_t writeBufferLen,
        const char *commandString)
{
    int32_t status = ENET_SOK;
    uint8_t chNum;
    char *parameter;
    BaseType_t paramLen;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    chNum = atoi(parameter);
    if (EnetApp_inst.txDmaCh[chNum] != CH_CLOSE)
    {
        snprintf(writeBuffer, writeBufferLen,
                "Tx channel %d is already open\r\n", chNum);
        return pdFALSE;
    }

    /* Open Tx channel */
    status = EnetApp_openTxCh(chNum);
    if (status)
        snprintf(writeBuffer, writeBufferLen,
                "Failed to open new Tx channel\r\n");
    else
        snprintf(writeBuffer, writeBufferLen,
                "Opened new Tx channel with identifier %d\r\n", chNum);
    return pdFALSE;
}

BaseType_t EnetCLI_openRxChn(char *writeBuffer, size_t writeBufferLen,
        const char *commandString)
{
    int32_t status = ENET_SOK;
    uint8_t chNum;
    char *parameter;
    BaseType_t paramLen;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    chNum = atoi(parameter);
    if (EnetApp_inst.rxDmaCh[chNum] != CH_CLOSE)
    {
        snprintf(writeBuffer, writeBufferLen,
                "Rx channel %d is already open\r\n", chNum);
        return pdFALSE;
    }

    /* Open Rx channel */
    status = EnetApp_openRxCh(chNum);
    if (status)
        snprintf(writeBuffer, writeBufferLen, "Failed to open Rx channel\r\n");
    else
        snprintf(writeBuffer, writeBufferLen,
                "Opened new Rx channel with identifier %d\r\n", chNum);
    return pdFALSE;
}

BaseType_t EnetCLI_getHostMac(char *writeBuffer, size_t writeBufferLen,
        const char *commandString)
{
    EnetAppUtils_print("Host MAC address: ");
    EnetAppUtils_printMacAddr(EnetApp_inst.hostMacAddr);
    return pdFALSE;
}

BaseType_t EnetCLI_transmitPkt(char *writeBuffer, size_t writeBufferLen,
        const char *commandString)
{
    int32_t status = ENET_SOK;
    uint8_t destAddr[6] = { 255, 255, 255, 255, 255, 255 };
    uint8_t srcAddr[6];
    EnetUtils_copyMacAddr(srcAddr, EnetApp_inst.hostMacAddr);
    int8_t dmaChNum = -1;
    uint16_t vlanId = 1;
    uint8_t priority = 0;
    char payloadMsg[100] = "";
    strcpy(payloadMsg, "Test packet from AM243x");
    char *parameter;
    BaseType_t paramLen;
    uint32_t paramCnt = 1;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, paramCnt,
            &paramLen);
    while (parameter != NULL)
    {
        if (paramCnt == 1)
        {
            dmaChNum = atoi(parameter);
            if(dmaChNum > ENET_SYSCFG_TX_CHANNELS_NUM)
            {
                snprintf(writeBuffer, writeBufferLen,
                        "Invalid DMA channel.\r\n");
                return pdFALSE;
            }
            else if(EnetApp_inst.txDmaCh[dmaChNum] == CH_CLOSE)
            {
                snprintf(writeBuffer, writeBufferLen,
                        "DMA channel is not open.\r\n");
                return pdFALSE;
            }
            paramCnt += 1;
        }
        else if (strncmp("-dm", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                    paramCnt + 1, &paramLen);
            status = EnetAppUtils_macAddrAtoI(parameter, destAddr);
            if (status)
            {
                snprintf(writeBuffer, writeBufferLen,
                        "Invalid dest MAC address\r\n");
                return pdFALSE;
            }
            paramCnt += 2;
        }
        else if (strncmp("-sm", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                    paramCnt + 1, &paramLen);
            status = EnetAppUtils_macAddrAtoI(parameter, srcAddr);
            if (status)
            {
                snprintf(writeBuffer, writeBufferLen,
                        "Invalid source MAC address\r\n");
                return pdFALSE;
            }
            paramCnt += 2;
        }
        else if (strncmp("-v", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                    paramCnt + 1, &paramLen);
            vlanId = atoi(parameter);
            paramCnt += 2;
        }
        else if (strncmp("-pcp", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                    paramCnt + 1, &paramLen);
            priority = atoi(parameter);
            paramCnt += 2;
        }
        else if (strncmp("-m", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                    paramCnt + 1, &paramLen);
            strcpy(payloadMsg, parameter);
            break;
        }
        parameter = (char*) FreeRTOS_CLIGetParameter(commandString, paramCnt,
                &paramLen);
    }

    if (dmaChNum < 0 || dmaChNum >= ENET_SYSCFG_TX_CHANNELS_NUM)
    {
        snprintf(writeBuffer, writeBufferLen, "Invalid Tx channel\r\n");
        return pdFALSE;
    }
    else if (EnetApp_inst.txDmaCh[dmaChNum] == CH_CLOSE)
    {
        snprintf(writeBuffer, writeBufferLen, "Tx channel %d is not open\r\n",
                dmaChNum);
        return pdFALSE;
    }

    status = EnetApp_transmitPkt(payloadMsg, destAddr, srcAddr, vlanId,
            priority, dmaChNum);
    if (status)
        snprintf(writeBuffer, writeBufferLen, "Send Failed\r\n");
    else
        snprintf(writeBuffer, writeBufferLen, "Done\r\n");
    return pdFALSE;
}

BaseType_t EnetCLI_capturePkt(char *writeBuffer, size_t writeBufferLen,
        const char *commandString)
{
    int32_t status = ENET_SOK;
    char *parameter;
    BaseType_t paramLen;
    int8_t dmaChNum;
    bool isStart;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    if (strncmp(parameter, "start", paramLen) == 0)
        isStart = true;
    else if (strncmp(parameter, "stop", paramLen) == 0)
        isStart = false;
    else
    {
        snprintf(writeBuffer, writeBufferLen, "Invalid Arg\r\n");
        return pdFALSE;
    }
    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 2, &paramLen);
    dmaChNum = atoi(parameter);

    if (dmaChNum < 0 || dmaChNum >= ENET_SYSCFG_TX_CHANNELS_NUM)
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
        if (status)
            snprintf(writeBuffer, writeBufferLen,
                    "Failed to start Rx task\r\n");
        else
            snprintf(writeBuffer, writeBufferLen,
                    "Listening to packets at Rx channel %d\r\n", dmaChNum);
    }
    else
    {
        if (EnetApp_inst.rxDmaCh[dmaChNum] == CH_IDLE)
        {
            snprintf(writeBuffer, writeBufferLen,
                    "Rx channel %d is not listening to packets\r\n", dmaChNum);
            return pdFALSE;
        }

        EnetApp_inst.rxDmaCh[dmaChNum] = CH_STOP;

        /* Wait for Rx channel to stop listening to incoming packets */
        while (EnetApp_inst.rxDmaCh[dmaChNum] != CH_IDLE);

        snprintf(writeBuffer, writeBufferLen,
                "Rx channel %d stopped listening to packets\r\n", dmaChNum);
    }

    return pdFALSE;
}

BaseType_t EnetCLI_dumpRxBuffer(char *writeBuffer, size_t writeBufferLen,
        const char *commandString)
{
    int8_t dmaChNum = 0;
    static int8_t rxBufferPt = -1;
    static int8_t readStartPt = -1;
    char *parameter;
    BaseType_t paramLen;
    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    if (parameter != NULL)
    {
        dmaChNum = atoi(parameter);
    }

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

    rxBufferPt = rxBufferPt == 3 ? 0 : rxBufferPt + 1;
    if (rxBufferPt == readStartPt)
    {
        rxBufferPt = -1;
        return pdFALSE;
    }
    else
        return pdTRUE;
}

BaseType_t EnetCLI_quitTerminal(char *writeBuffer, size_t writeBufferLen,
        const char *commandString)
{
    snprintf(writeBuffer, writeBufferLen, "Exit\r\n");
    EnetApp_close();
    return pdFALSE;
}

int32_t EnetApp_init(void)
{
    Cpsw_Cfg cpswCfg;
    EnetDma_Cfg dmaCfg;
    Enet_IoctlPrms prms;
    EnetPer_PortLinkCfg portLinkCfg;
    CpswMacPort_Cfg macCfg;
    int32_t status = ENET_SOK;

    cpswCfg.dmaCfg = &dmaCfg;
    EnetApp_initCpswCfg(&cpswCfg);
    dmaCfg.rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;

    EnetApp_inst.macMode = RGMII;
    
    EnetApp_inst.hUdmaDrv = EnetAppUtils_udmaOpen(EnetApp_inst.enetType, NULL);
    EnetAppUtils_assert(NULL != EnetApp_inst.hUdmaDrv);

    dmaCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;

    /* Set Enet global runtime log level */
    Enet_setTraceLevel(ENET_TRACE_DEBUG);
    
    /* Set Enet global runtime log level */
    EnetApp_inst.hEnet = Enet_open(EnetApp_inst.enetType, EnetApp_inst.instId, &cpswCfg, sizeof(cpswCfg));
    if (EnetApp_inst.hEnet == NULL)
    {
        EnetAppUtils_print("Failed to open Enet driver\n");
        status = ENET_EFAIL;
    }

    uint8_t macPortNum;
    for (macPortNum = 0; macPortNum < EnetApp_inst.numMacPorts && status == ENET_SOK; macPortNum++)
    {
        if (EnetApp_inst.macPort[macPortNum] != ENET_MAC_PORT_INV)
        {
            /* Setup port link open parameters */
            if (status == ENET_SOK)
            {
                EnetBoard_EthPort ethPort;

                /* Setup board for requested Ethernet port */
                ethPort.macPort  = EnetApp_inst.macPort[macPortNum];
                ethPort.boardId  = EnetApp_inst.boardId;

                status = EnetBoard_setupPorts(EnetApp_inst.enetType, EnetApp_inst.instId, &ethPort, 1U);
                EnetAppUtils_assert(status == ENET_SOK);

                /* Set port link params */
                portLinkCfg.macPort = EnetApp_inst.macPort[macPortNum];
                portLinkCfg.macCfg = &macCfg;

                CpswMacPort_initCfg(&macCfg);

            }
            /* Open port link */
            if (status == ENET_SOK)
            {
                ENET_IOCTL_SET_IN_ARGS(&prms, &portLinkCfg);

                status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, ENET_PER_IOCTL_OPEN_PORT_LINK, &prms);
                if (status != ENET_SOK)
                {
                    EnetAppUtils_print("Failed to open port link: %d\n", status);
                }
            }
        }
    }

    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to open Enet driver: %d\n", status);
    }
    if (status == ENET_SOK)
    {   
        /* Attach the core with RM */
        uint32_t coreId;
        EnetPer_AttachCoreOutArgs attachCoreOutArgs;
        coreId = EnetApp_inst.coreId;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &coreId, &attachCoreOutArgs);
        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, ENET_PER_IOCTL_ATTACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("EnetL2Networking failed ENET_PER_IOCTL_ATTACH_CORE: %d\n", status);
        }
        else
        {
            EnetApp_inst.coreKey = attachCoreOutArgs.coreKey;
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

        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, CPSW_ALE_IOCTL_ADD_MCAST, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("EnetLpbk_loopbackTest failed add broadcast entry: %d\n", status);
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
        status = EnetApp_openDma(0);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to open DMA: %d\n", status);
        }
    }

    /* Enable host port */
    if (status == ENET_SOK)
    {
        status = EnetApp_setupCpswAle();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to setup CPSW ALE: %d\n", status);
        }

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_NO_ARGS(&prms);
            status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, ENET_HOSTPORT_IOCTL_ENABLE, &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Failed to enable host port: %d\n", status);
            }
        }
    }

    /* Show alive PHYs */
    if (status == ENET_SOK)
    {
        status = EnetApp_showAlivePhys();
    }

    /* Wait for link up */
    for (macPortNum = 0; macPortNum < EnetApp_inst.numMacPorts && status == ENET_SOK; macPortNum++)
        EnetApp_waitForLinkUp(EnetApp_inst.macPort[macPortNum]);

    /* Print network statistics */
    if (status == ENET_SOK)
    {
        EnetLpbk_showCpswStats();
    }

    /* Disable host port */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, ENET_HOSTPORT_IOCTL_DISABLE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to disable host port: %d\n", status);
        }
    }

    /* Print DMA statistics */
    if (status == ENET_SOK)
    {
        uint8_t num;
        for(num=0; num<ENET_SYSCFG_RX_FLOWS_NUM; num++)
        EnetAppUtils_showRxChStats(EnetApp_inst.hRxCh[num]);

        for(num=0; num<ENET_SYSCFG_TX_CHANNELS_NUM; num++)
        EnetAppUtils_showTxChStats(EnetApp_inst.hTxCh[num]);
    }

#if ENET_SYSCFG_TX_CHANNELS_NUM >= 1
    EnetApp_pktPerTxCh[0] = ENET_DMA_TX_CH0_NUM_PKTS;
#endif
#if ENET_SYSCFG_TX_CHANNELS_NUM >= 2
    EnetApp_pktPerTxCh[1] = ENET_DMA_TX_CH1_NUM_PKTS;
#endif
#if ENET_SYSCFG_TX_CHANNELS_NUM >= 3
    EnetApp_pktPerTxCh[2] = ENET_DMA_TX_CH2_NUM_PKTS;
#endif
#if ENET_SYSCFG_TX_CHANNELS_NUM >= 4
    EnetApp_pktPerTxCh[3] = ENET_DMA_TX_CH3_NUM_PKTS;
#endif

#if ENET_SYSCFG_RX_FLOWS_NUM >= 1
    EnetApp_pktPerRxCh[0] = ENET_DMA_RX_CH0_NUM_PKTS;
#endif
#if ENET_SYSCFG_RX_FLOWS_NUM >= 2
    EnetApp_pktPerRxCh[1] = ENET_DMA_RX_CH1_NUM_PKTS;
#endif
#if ENET_SYSCFG_RX_FLOWS_NUM >= 3
    EnetApp_pktPerRxCh[2] = ENET_DMA_RX_CH2_NUM_PKTS;
#endif
#if ENET_SYSCFG_RX_FLOWS_NUM >= 4
    EnetApp_pktPerRxCh[3] = ENET_DMA_RX_CH3_NUM_PKTS;
#endif


    EnetApp_inst.initFlag = ENET_UP;

 /* Close Enet DMA driver */
    EnetApp_closeDma();

    /* Close Enet driver */
    EnetApp_closeEnet();

    /* Disable peripheral clocks */
    EnetAppUtils_disableClocks(EnetApp_inst.enetType, EnetApp_inst.instId);

    /* Delete RX and TX tasks */
    EnetApp_deleteRxTxTasks();

    /* Deinit Enet driver */
    Enet_deinit();

    /* Delete all TX/RX semaphores */
    SemaphoreP_delete(EnetApp_inst.hRxDoneSem);
    EnetApp_inst.hRxDoneSem = NULL;
    SemaphoreP_delete(EnetApp_inst.hTxDoneSem);
    EnetApp_inst.hTxDoneSem = NULL;
    SemaphoreP_delete(EnetApp_inst.txSemObj[0]);
    EnetApp_inst.txSemObj[0] = NULL;
    SemaphoreP_delete(EnetApp_inst.rxSemObj[0]);
    EnetApp_inst.rxSemObj[0] = NULL;

    EnetAppUtils_print("Test complete: %s\n", (status == ENET_SOK) ? "PASS" : "FAIL");

    return 0;
}

int32_t EnetApp_openDma(uint8_t dmaChNum)
{
    int32_t status = ENET_SOK;
    EnetUdma_OpenRxFlowPrms rxChCfg;
    EnetUdma_OpenTxChPrms   txChCfg;

    /* Open the CPSW TX channel  */
    if (status == ENET_SOK)
    {
        EnetUdma_initTxChParams(&txChCfg);

        txChCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;
        txChCfg.cbArg    = &EnetApp_inst;
        txChCfg.notifyCb = EnetApp_txIsrFxn;

        EnetAppUtils_setCommonTxChPrms(&txChCfg);

        EnetAppUtils_openTxCh(EnetApp_inst.hEnet,
                              EnetApp_inst.coreKey,
                              EnetApp_inst.coreId,
                              &EnetApp_inst.txChNum,
                              &EnetApp_inst.hTxCh[dmaChNum],
                              &txChCfg);

        EnetApp_initTxFreePktQ(&EnetApp_inst.txFreePktInfoQ[dmaChNum]);

        if (NULL != EnetApp_inst.hTxCh[dmaChNum])
        {
            status = EnetDma_enableTxEvent(EnetApp_inst.hTxCh[dmaChNum]);
            if (ENET_SOK != status)
            {
#if FIX_RM
                /* Free the Ch Num if enable event failed */
                EnetAppUtils_freeTxCh(EnetApp_inst.hEnet,
                                      EnetApp_inst.coreKey,
                                      EnetApp_inst.coreId,
                                      EnetApp_inst.txChNum);
#endif
                EnetAppUtils_print("EnetUdma_startTxCh() failed: %d\n", status);
                status = ENET_EFAIL;
            }
        }
        else
        {
#if FIX_RM
            /* Free the Ch Num if open Tx Ch failed */
            EnetAppUtils_freeTxCh(EnetApp_inst.hEnet,
                                  EnetApp_inst.coreKey,
                                  EnetApp_inst.coreId,
                                  EnetApp_inst.txChNum);
#endif
            EnetAppUtils_print("EnetUdma_openTxCh() failed to open: %d\n",
                               status);
            status = ENET_EFAIL;
        }
    }

    /* Open the CPSW RX flow  */
    if (status == ENET_SOK)
    {
        EnetUdma_initRxFlowParams(&rxChCfg);

        rxChCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;
        rxChCfg.notifyCb = EnetApp_rxIsrFxn;
        rxChCfg.cbArg   = &EnetApp_inst;

        EnetAppUtils_setCommonRxFlowPrms(&rxChCfg);
        EnetAppUtils_openRxFlow(EnetApp_inst.enetType,
                                EnetApp_inst.hEnet,
                                EnetApp_inst.coreKey,
                                EnetApp_inst.coreId,
                                BTRUE,
                                &EnetApp_inst.rxStartFlowIdx,
                                &EnetApp_inst.rxFlowIdx,
                                &EnetApp_inst.hostMacAddr[0U],
                                &EnetApp_inst.hRxCh[dmaChNum],
                                &rxChCfg);
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
            EnetApp_initRxReadyPktQ(&EnetApp_inst.rxFreeQ[dmaChNum], &EnetApp_inst.rxReadyQ[dmaChNum], EnetApp_inst.hRxCh[dmaChNum], EnetApp_pktPerRxCh[dmaChNum]);        }
    }

    return status;
}


/* ========================================================================== */
/*                   Static Function Definitions                              */
/* ========================================================================== */

static void EnetLpbk_showCpswStats(void)
{
    Enet_IoctlPrms prms;
    CpswStats_PortStats portStats;
    int32_t status;

    /* Show host port statistics */
    ENET_IOCTL_SET_OUT_ARGS(&prms, &portStats);
    status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, ENET_STATS_IOCTL_GET_HOSTPORT_STATS, &prms);
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
    uint8_t macPortNum;
    for (macPortNum = 0; macPortNum < EnetApp_inst.numMacPorts; macPortNum++)
    {
        if ((status == ENET_SOK) &&
            (EnetApp_inst.macPort[macPortNum] != ENET_MAC_PORT_INV))
        {
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &EnetApp_inst.macPort[macPortNum], &portStats);
            status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, ENET_STATS_IOCTL_GET_MACPORT_STATS, &prms);
            if (status == ENET_SOK)
            {
                EnetAppUtils_print("\n Port %d Statistics\n", macPortNum);
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
}

static int32_t EnetApp_showAlivePhys(void)
{
    Enet_IoctlPrms prms;
    bool alive = BFALSE;
    int8_t i;
    int32_t status;

    for (i = 0U; i < ENET_MDIO_PHY_CNT_MAX; i++)
    {
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &i, &alive);

        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, ENET_MDIO_IOCTL_IS_ALIVE, &prms);
        if (status == ENET_SOK)
        {
            if (alive == BTRUE)
            {
                EnetAppUtils_print("PHY %u is alive\n", i);
            }
        }
        else
        {
            EnetAppUtils_print("Failed to get PHY %u alive status: %d\n", i, status);
        }
    }

    return status;
}

static void EnetApp_initCpswCfg(Cpsw_Cfg *cpswCfg)
{
    CpswHostPort_Cfg *hostPortCfg = &cpswCfg->hostPortCfg;
    CpswAle_Cfg *aleCfg = &cpswCfg->aleCfg;
    CpswCpts_Cfg *cptsCfg = &cpswCfg->cptsCfg;

    /* Set initial config */
    Enet_initCfg(EnetApp_inst.enetType, EnetApp_inst.instId, cpswCfg, sizeof(*cpswCfg));

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
    
    uint8_t macPortNum;
    for (macPortNum = 0; macPortNum < EnetApp_inst.numMacPorts; macPortNum++)
    {
        if (EnetApp_inst.macPort[macPortNum] == ENET_MAC_PORT_INV)
        {
            aleCfg->modeFlags                     |= CPSW_ALE_CFG_MULTIHOST;
        }
    }

    /* CPTS config */
    /* Note: Timestamping and MAC loopback are not supported together because of
     * IP limitation, so disabling timestamping for this application */
    cptsCfg->hostRxTsEn = BFALSE;

    EnetAppUtils_initResourceConfig(EnetApp_inst.enetType, EnetApp_inst.instId, EnetApp_inst.coreId, &cpswCfg->resCfg);
}

static int32_t EnetApp_setupCpswAle(void)
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
    EnetUtils_copyMacAddr(&setUcastInArgs.addr.addr[0U], EnetApp_inst.hostMacAddr);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &entryIdx);

    status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, CPSW_ALE_IOCTL_ADD_UCAST, &prms);
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

        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to set ALE port state: %d\n", status);
        }
    }

    return status;
}

static int32_t EnetApp_waitForLinkUp(Enet_MacPort macPort)
{
    uint8_t timeout = 0;
    Enet_IoctlPrms prms;
    bool linked = false;
    int32_t status = ENET_SOK;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPort, &linked);

    while (!linked)
    {
        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                ENET_PER_IOCTL_IS_PORT_LINK_UP, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print(
                    "[WRN] %s: Failed to get port %u's link status: %d\r\n",
                    __func__, ENET_MACPORT_ID(macPort), status);
            linked = false;
            break;
        }
        if (timeout >= 150)
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

static void EnetApp_closeEnet(void)
{
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;

    /* Close port link */
    
    uint8_t macPortNum;
    for (macPortNum = 0; macPortNum < EnetApp_inst.numMacPorts; macPortNum++)
    {
        if (EnetApp_inst.macPort[macPortNum] != ENET_MAC_PORT_INV)
        {
            ENET_IOCTL_SET_IN_ARGS(&prms, &EnetApp_inst.macPort[macPortNum]);

            status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Failed to close port link: %d\n", status);
            }
        }
    }

    /* Detach core */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &EnetApp_inst.coreKey);

        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, ENET_PER_IOCTL_DETACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to detach core key %u: %d\n", EnetApp_inst.coreKey, status);
        }
    }

    /* Close Enet driver */
    Enet_close(EnetApp_inst.hEnet);

    /* Close UDMA */
    EnetAppUtils_udmaclose(EnetApp_inst.hUdmaDrv);

    EnetApp_inst.hEnet = NULL;
}


static void EnetApp_txIsrFxn(void *appData)
{
    SemaphoreP_post(appData);
}

static void EnetApp_rxIsrFxn(void *appData)
{
    SemaphoreP_post(appData);
}

static void EnetApp_initTxFreePktQ(EnetQ* dmaChannel)
{
    EnetDma_Pkt *pPktInfo;
    uint32_t i;
    uint32_t scatterSegments[] = {
    ENET_MEM_LARGE_POOL_PKT_SIZE, };

    /* Initialize all queues */
    EnetQueue_initQ(dmaChannel);

    /* Initialize TX EthPkts and queue them to txFreePktInfoQ */
    for (i = 0U; i < ENET_MEM_NUM_TX_PKTS; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&EnetApp_inst,
                ENETDMA_CACHELINE_ALIGNMENT, ENET_ARRAYSIZE(scatterSegments),
                scatterSegments);
        EnetAppUtils_assert(pPktInfo != NULL);
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState,
                ENET_PKTSTATE_APP_WITH_FREEQ);

        EnetQueue_enq(dmaChannel, &pPktInfo->node);
    }
}

static void EnetApp_initRxReadyPktQ(EnetQ* dmaChannelFreeQ, EnetQ* dmaChannelReadyQ, EnetDma_RxChHandle rxHandle, int32_t numPacketsPerRXChnl)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pPktInfo;
    int32_t status;
    uint32_t i;
    uint32_t scatterSegments[] = {
    ENET_MEM_LARGE_POOL_PKT_SIZE, };

    EnetQueue_initQ(dmaChannelFreeQ);
    EnetQueue_initQ(dmaChannelReadyQ);
    EnetQueue_initQ(&rxReadyQ);

    for (i = 0U; i < numPacketsPerRXChnl; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&EnetApp_inst,
                ENETDMA_CACHELINE_ALIGNMENT, ENET_ARRAYSIZE(scatterSegments),
                scatterSegments);
        EnetAppUtils_assert(pPktInfo != NULL);
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState,
                ENET_PKTSTATE_APP_WITH_FREEQ);
        EnetQueue_enq(dmaChannelFreeQ, &pPktInfo->node);
    }

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(rxHandle, &rxReadyQ);
    EnetAppUtils_assert(status == ENET_SOK);
    /* There should not be any packet with DMA during init */
    EnetAppUtils_assert(EnetQueue_getQCount(&rxReadyQ) == 0U);

    EnetAppUtils_validatePacketState(dmaChannelFreeQ,
            ENET_PKTSTATE_APP_WITH_FREEQ, ENET_PKTSTATE_APP_WITH_DRIVER);

    EnetDma_submitRxPktQ(rxHandle,
            dmaChannelFreeQ);

    while (0U != EnetQueue_getQCount(dmaChannelFreeQ))
        EnetDma_submitRxPktQ(rxHandle,
                dmaChannelFreeQ);
}

static int32_t EnetApp_openTxCh(uint8_t dmaChNum)
{
    int32_t status = ENET_SOK;
    EnetUdma_OpenTxChPrms   txChCfg;

    /* Open the CPSW TX channel  */
    if (status == ENET_SOK)
    {
        EnetUdma_initTxChParams(&txChCfg);

        txChCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;
        txChCfg.cbArg    = &EnetApp_inst;
        txChCfg.notifyCb = EnetApp_txIsrFxn;

        EnetAppUtils_setCommonTxChPrms(&txChCfg);

        EnetAppUtils_openTxCh(EnetApp_inst.hEnet,
                              EnetApp_inst.coreKey,
                              EnetApp_inst.coreId,
                              &EnetApp_inst.txChNum,
                              &EnetApp_inst.hTxCh[dmaChNum],
                              &txChCfg);

        EnetApp_initTxFreePktQ(&EnetApp_inst.txFreePktInfoQ[dmaChNum]);

        if (NULL != EnetApp_inst.hTxCh[dmaChNum])
        {
            status = EnetDma_enableTxEvent(EnetApp_inst.hTxCh[dmaChNum]);
            if (ENET_SOK != status)
            {
#if FIX_RM
                /* Free the Ch Num if enable event failed */
                EnetAppUtils_freeTxCh(gEnetLpbk.hEnet,
                                      gEnetLpbk.coreKey,
                                      gEnetLpbk.coreId,
                                      gEnetLpbk.txChNum);
#endif
                EnetAppUtils_print("EnetUdma_startTxCh() failed: %d\n", status);
                status = ENET_EFAIL;
            }
        }
        else
        {
#if FIX_RM
            /* Free the Ch Num if open Tx Ch failed */
            EnetAppUtils_freeTxCh(gEnetLpbk.hEnet,
                                  gEnetLpbk.coreKey,
                                  gEnetLpbk.coreId,
                                  gEnetLpbk.txChNum);
#endif
            EnetAppUtils_print("EnetUdma_openTxCh() failed to open: %d\n",
                               status);
            status = ENET_EFAIL;
        }
    }
    return status;
}

static int32_t EnetApp_openRxCh(uint8_t dmaChNum)
{
    int32_t status = ENET_SOK;
    EnetUdma_OpenRxFlowPrms rxChCfg;

    /* Open the CPSW RX flow  */
    if (status == ENET_SOK)
    {
        EnetUdma_initRxFlowParams(&rxChCfg);

        rxChCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;
        rxChCfg.notifyCb = EnetApp_rxIsrFxn;
        rxChCfg.cbArg   = &EnetApp_inst;

        EnetAppUtils_setCommonRxFlowPrms(&rxChCfg);
        EnetAppUtils_openRxFlow(EnetApp_inst.enetType,
                                EnetApp_inst.hEnet,
                                EnetApp_inst.coreKey,
                                EnetApp_inst.coreId,
                                BTRUE,
                                &EnetApp_inst.rxStartFlowIdx,
                                &EnetApp_inst.rxFlowIdx,
                                &EnetApp_inst.hostMacAddr[0U],
                                &EnetApp_inst.hRxCh[dmaChNum],
                                &rxChCfg);
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
        EnetApp_initRxReadyPktQ(&EnetApp_inst.rxFreeQ[dmaChNum], &EnetApp_inst.rxReadyQ[dmaChNum], EnetApp_inst.hRxCh[dmaChNum], EnetApp_pktPerRxCh[dmaChNum]);        }
    }

    return status;
}

static int32_t EnetApp_transmitPkt(char *payload, uint8_t *destMacAddr,
        uint8_t *srcMacAddr, uint16_t vlanId, uint8_t priority, int8_t dmaChNum)
{
    EnetDma_PktQ txSubmitQ;
    EnetDma_Pkt *pktInfo;
    uint32_t txRetrievePktCnt;
    size_t pktLen = ETH_PAYLOAD_LEN;
    int32_t status = ENET_SOK;
    size_t payloadLen = strlen(payload);
    size_t txLen = 0;

    while (txLen < payloadLen)
    {
        EnetQueue_initQ(&txSubmitQ);

        /* Dequeue one free TX Eth packet */
        pktInfo = (EnetDma_Pkt*) EnetQueue_deq(
                &EnetApp_inst.txFreePktInfoQ[dmaChNum]);

        while (NULL != pktInfo)
        {
            if (payloadLen - txLen < ETH_PAYLOAD_LEN)
                pktLen = payloadLen - txLen;
            /* Fill the TX Eth frame with test content */
            if (vlanId == 1)
            {
                EthFrame *frame = (EthFrame*) pktInfo->sgList.list[0].bufPtr;
                memcpy(frame->hdr.dstMac, destMacAddr, ENET_MAC_ADDR_LEN);
                memcpy(frame->hdr.srcMac, srcMacAddr,
                ENET_MAC_ADDR_LEN);

                frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
                memcpy(&frame->payload[0U], (payload + txLen), pktLen);
                pktInfo->sgList.list[0].segmentFilledLen = pktLen
                        + sizeof(EthFrameHeader);
            }
            else
            {
                EthVlanFrame *vlanFrame =
                        (EthVlanFrame*) pktInfo->sgList.list[0].bufPtr;
                memcpy(vlanFrame->hdr.dstMac, destMacAddr, ENET_MAC_ADDR_LEN);
                memcpy(vlanFrame->hdr.srcMac, srcMacAddr,
                ENET_MAC_ADDR_LEN);

                vlanFrame->hdr.tpid = Enet_htons(0x8100U);
                vlanFrame->hdr.tci = Enet_htons(
                        ENETAPP_VLAN_TCI(priority, 0, vlanId));
                vlanFrame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
                memcpy(&vlanFrame->payload[0U], (payload + txLen), pktLen);
                pktInfo->sgList.list[0].segmentFilledLen = pktLen
                        + sizeof(EthVlanFrameHeader);
            }

            pktInfo->sgList.numScatterSegments = 1;
            pktInfo->chkSumInfo = 0U;
            pktInfo->appPriv = &EnetApp_inst;
            EnetDma_checkPktState(&pktInfo->pktState, ENET_PKTSTATE_MODULE_APP,
                    ENET_PKTSTATE_APP_WITH_FREEQ,
                    ENET_PKTSTATE_APP_WITH_DRIVER);

            /* Enqueue the packet for later transmission */
            EnetQueue_enq(&txSubmitQ, &pktInfo->node);
            txLen = txLen + pktLen;
            if (txLen >= payloadLen)
            {
                break;
            }

            /* Dequeue one free TX Eth packet */
            pktInfo = (EnetDma_Pkt*) EnetQueue_deq(
                    &EnetApp_inst.txFreePktInfoQ[dmaChNum]);
        }

        while (0U != EnetQueue_getQCount(&txSubmitQ))
        {
            uint32_t txCnt = EnetQueue_getQCount(&txSubmitQ);
            status = EnetDma_submitTxPktQ(EnetApp_inst.hTxCh[dmaChNum],
                    &txSubmitQ);
            SemaphoreP_pend(&EnetApp_inst.txSemObj[dmaChNum],
            SemaphoreP_WAIT_FOREVER);

            /* Retrieve TX free packets */
            if (status == ENET_SOK)
            {
                txCnt = txCnt - EnetQueue_getQCount(&txSubmitQ);
                txRetrievePktCnt = 0U;
                while (txRetrievePktCnt != txCnt)
                {
                    /* This is not failure as HW is busy sending packets, we
                     * need to wait and again call retrieve packets */
                    TaskP_sleep(1000U);
                    txRetrievePktCnt += EnetApp_retrieveFreeTxPkts(dmaChNum);
                }
            }
            else
            {
                EnetAppUtils_print("[ERR] %s: Failed to transmit packet %d",
                        __func__, status);
                return 1;
            }
        }
    }

    EnetAppUtils_print(
            "Send details:\r\n channel: %d,\r\n payload: \"%s\",\r\n Source MAC: ",
            dmaChNum, payload);
    EnetAppUtils_printMacAddr(srcMacAddr);
    EnetAppUtils_print(" Destination MAC: ");
    EnetAppUtils_printMacAddr(destMacAddr);
    return 0;
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
            EnetDma_checkPktState(&pktInfo->pktState, ENET_PKTSTATE_MODULE_APP,
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
    EnetDma_Pkt *pktInfo;
    uint32_t rxReadyCnt;
    uint32_t loopRxPktCnt;
    int32_t status = ENET_SOK;

    EnetApp_inst.totalRxCnt = 0U;

        loopRxPktCnt = 0U;
        /* Wait for packet reception */
        do
        {
            SemaphoreP_pend(EnetApp_inst.rxSemObj[dmaChNum], SemaphoreP_WAIT_FOREVER);
            /* Get the packets received so far */
            rxReadyCnt = EnetApp_recievePkt(dmaChNum);
            if (rxReadyCnt > 0U)
            {
                /* Consume the received packets and release them */
                pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&EnetApp_inst.rxReadyQ[dmaChNum]);
                while (NULL != pktInfo)
                {
                    EnetDma_checkPktState(&pktInfo->pktState,
                                            ENET_PKTSTATE_MODULE_APP,
                                            ENET_PKTSTATE_APP_WITH_READYQ,
                                            ENET_PKTSTATE_APP_WITH_FREEQ);

                    /* Release the received packet */
                    EnetQueue_enq(&EnetApp_inst.rxFreeQ[dmaChNum], &pktInfo->node);
                    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&EnetApp_inst.rxReadyQ[dmaChNum]);
                }

                /*Submit now processed buffers */
                if (status == ENET_SOK)
                {
                    EnetAppUtils_validatePacketState(&EnetApp_inst.rxFreeQ[dmaChNum],
                                                     ENET_PKTSTATE_APP_WITH_FREEQ,
                                                     ENET_PKTSTATE_APP_WITH_DRIVER);

                    EnetDma_submitRxPktQ(EnetApp_inst.hRxCh[dmaChNum],
                                         &EnetApp_inst.rxFreeQ[dmaChNum]);
                }
            }

            loopRxPktCnt += rxReadyCnt;
        }
        while (loopRxPktCnt < ENETLPBK_TEST_PKT_NUM);

        EnetApp_inst.totalRxCnt += loopRxPktCnt;

    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to transmit/receive packets: %d, transmitted: %d \n", ENETLPBK_TEST_PKT_NUM, EnetApp_inst.totalRxCnt);
    }
    else
    {
        EnetAppUtils_print("Received %d packets\n", EnetApp_inst.totalRxCnt);
    }

    SemaphoreP_post(EnetApp_inst.hRxDoneSem);
    return status;
}

static int32_t EnetApp_recievePkt(int8_t dmaChNum)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pktInfo;
    int32_t status;
    uint32_t rxReadyCnt = 0U;

    EnetQueue_initQ(&rxReadyQ);

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(EnetApp_inst.rxSemObj[dmaChNum], &rxReadyQ);
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

            EnetQueue_enq(&EnetApp_inst.rxReadyQ[dmaChNum], &pktInfo->node);
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxReadyQ);
        }
    }
    else
    {
        EnetAppUtils_print("receivePkts() failed to retrieve pkts: %d\n",
                           status);
    }

    return rxReadyCnt;
}

static void EnetApp_deleteRxTxTasks(void)
{
    if (EnetApp_inst.hTxTask != NULL)
    {
#if !defined (FREERTOS)
        EnetAppUtils_assert(TaskP_isTerminated(EnetApp_inst.hTxTask) == 1);
        TaskP_delete(&EnetApp_inst.hTxTask);
#endif
        EnetApp_inst.hTxTask = NULL;
    }

    if (EnetApp_inst.hRxTask != NULL)
    {
#if !defined (FREERTOS)
        EnetAppUtils_assert(TaskP_isTerminated(EnetApp_inst.hRxTask) == 1);
        TaskP_delete(&EnetApp_inst.hRxTask);
#endif
        EnetApp_inst.hRxTask = NULL;
    }
    EnetAppUtils_print("EnetLpbk_deleteRxTxTasks() done..\n");
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

        /* Queue the received packet to rxReadyQ and pass new ones from rxFreeQ
         **/
        pktInfo = (EnetDma_Pkt*) EnetQueue_deq(&rxReadyQ);
        while (pktInfo != NULL)
        {
            EnetDma_checkPktState(&pktInfo->pktState, ENET_PKTSTATE_MODULE_APP,
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
        UART_writeCLI("Stopping TSN stack...\r\n");
    }

    /* Close Enet DMA driver */
    UART_writeCLI("Closing DMA channels...\r\n");
    EnetApp_closeDma();

    /*Release Handle Info*/
    UART_writeCLI("Releasing handles...\r\n");
    EnetApp_inst.hEnet = NULL;

    UART_writeCLI("Deinitializing drives...\r\n");
    EnetMem_deInit();
    Enet_deinit();

    /* Disable peripheral clocks */
    UART_writeCLI("Diabling clocks...\r\n");
    EnetAppUtils_disableClocks(EnetApp_inst.enetType, EnetApp_inst.instId);
}

static void EnetApp_closeDma(void)
{
    /* Close Rx flows */
    for (int8_t i = ENET_SYSCFG_RX_FLOWS_NUM - 1; i >= 0; i--)
    {
        EnetDma_PktQ fqPktInfoQ;
        EnetDma_PktQ cqPktInfoQ;

        EnetQueue_initQ(&fqPktInfoQ);
        EnetQueue_initQ(&cqPktInfoQ);

        /* There should not be any ready packet */
        EnetAppUtils_assert(0U == EnetQueue_getQCount(&EnetApp_inst.rxReadyQ[i]));

    /* Close RX channel */
        EnetAppUtils_closeRxFlow(EnetApp_inst.enetType,
                                EnetApp_inst.hEnet,
                                EnetApp_inst.coreKey,
                                EnetApp_inst.coreId,
                                BTRUE,
                                &fqPktInfoQ,
                                &cqPktInfoQ,
                                EnetApp_inst.rxStartFlowIdx,
                                EnetApp_inst.rxFlowIdx,
                                EnetApp_inst.hostMacAddr,
                                EnetApp_inst.hRxCh[i]);

        EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
        EnetAppUtils_freePktInfoQ(&cqPktInfoQ);
    }

    /* Close TX channels */
    for (int8_t i = ENET_SYSCFG_RX_FLOWS_NUM - 1; i >= 0; i--)
    {
        EnetDma_PktQ fqPktInfoQ;
        EnetDma_PktQ cqPktInfoQ;

        EnetQueue_initQ(&fqPktInfoQ);
        EnetQueue_initQ(&cqPktInfoQ);

        EnetAppUtils_closeTxCh(EnetApp_inst.hEnet,
                           EnetApp_inst.coreKey,
                           EnetApp_inst.coreId,
                           &fqPktInfoQ,
                           &cqPktInfoQ,
                           EnetApp_inst.hTxCh[i],
                           EnetApp_inst.txChNum);
        EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
        EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

        EnetAppUtils_freePktInfoQ(&EnetApp_inst.rxFreeQ[i]);
        EnetAppUtils_freePktInfoQ(&EnetApp_inst.txFreePktInfoQ[i]);

        EnetMem_deInit();
    }
}

static void EnetApp_printFrame(EthFrame *frame, uint32_t len)
{
    uint8_t *payload;
    uint32_t i;
    char buffer[50] = "";

    UART_writeCLI("Dst addr : ");
    sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
            frame->hdr.dstMac[0] & 0xFF, frame->hdr.dstMac[1] & 0xFF,
            frame->hdr.dstMac[2] & 0xFF, frame->hdr.dstMac[3] & 0xFF,
            frame->hdr.dstMac[4] & 0xFF, frame->hdr.dstMac[5] & 0xFF);
    UART_writeCLI(buffer);

    UART_writeCLI("Src addr : ");
    sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
            frame->hdr.srcMac[0] & 0xFF, frame->hdr.srcMac[1] & 0xFF,
            frame->hdr.srcMac[2] & 0xFF, frame->hdr.srcMac[3] & 0xFF,
            frame->hdr.srcMac[4] & 0xFF, frame->hdr.srcMac[5] & 0xFF);
    UART_writeCLI(buffer);

    if (frame->hdr.etherType == Enet_htons(ETHERTYPE_VLAN_TAG))
    {
        EthVlanFrame *vlanFrame = (EthVlanFrame*) frame;

        sprintf(buffer, "TPID     : 0x%04x\r\n",
                (Enet_ntohs(vlanFrame->hdr.tpid) & 0xFFFFU));
        UART_writeCLI(buffer);
        sprintf(buffer, "Priority : %d\r\n",
                (Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFFU) >> 13);
        UART_writeCLI(buffer);
        sprintf(buffer, "VLAN Id  : %d\r\n",
                (Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFU));
        UART_writeCLI(buffer);
        sprintf(buffer, "EtherType: 0x%04x\r\n",
                (Enet_ntohs(vlanFrame->hdr.etherType) & 0xFFFFU));
        UART_writeCLI(buffer);
        payload = vlanFrame->payload;
        len -= ETH_VLAN_TAG_LEN;
    }
    else
    {
        sprintf(buffer, "EtherType: 0x%04x\r\n",
                (Enet_ntohs(frame->hdr.etherType) & 0xFFFFU));
        UART_writeCLI(buffer);
        payload = frame->payload;
    }

    UART_writeCLI("Payload  : ");
    for (i = 0; i < len; i++)
    {
        sprintf(buffer, "0x%02x ", payload[i]);
        UART_writeCLI(buffer);
        if (i && (((i + 1) % OCTETS_PER_ROW) == 0))
        {
            UART_writeCLI("\r\n           ");
        }
    }

    if (len && ((len % OCTETS_PER_ROW) != 0))
    {
        UART_writeCLI("\r\n");
    }

    UART_writeCLI("\r\n");
}
