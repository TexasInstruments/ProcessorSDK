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
 * \file  cli_common.c
 *
 * \brief This file contains definitions of some commmon functions used by
 *        the CLI application.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "cli_common.h"
#include "netif/ethernet.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void Enetcli_backspace() ;

static void Enetcli_printTypedCharacter(const char rxChar);

void EnetApp_createClock(void);

void EnetApp_deleteClock(void);

void EnetApp_tickTask(void *a0, void *a1);

void EnetApp_timerCallback(void *arg);

int32_t EnetApp_openDma();

void EnetApp_closeDma();

void EnetApp_rxIsrFxn(void *appData);

void EnetApp_txIsrFxn(void *appData);


void EnetApp_deleteRxTxTasks(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* End of line */
const char UART_endOfLine[3] = "\r\n";

static uint8_t gEnetCliTaskStackTick[ENETCli_TASK_STACK_SZ] __attribute__((aligned(ENETCli_TASK_STACK_ALIGN)));

/*! Enet object instance declaration */
EnetApp_Obj EnetApp_inst;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Emulates a backspace to remove last character in CLI terminal */
static void Enetcli_backspace()
{
    char consolePrint[2];

    /* Move the cursor by one character to the left */
    consolePrint[0] = '\b';
    consolePrint[1] = '\0';
    EnetAppUtils_print(consolePrint);
    /* Write a whitespace which will overwrite the last character */
    consolePrint[0] = ' ';
    consolePrint[1] = '\0';
    EnetAppUtils_print(consolePrint);
    /* Move the cursor one step to the left */
    consolePrint[0] = '\b';
    consolePrint[1] = '\0';
    EnetAppUtils_print(consolePrint);
}

/* Prints the characters entered by the user by converting each character 
 * to a string first */
static void Enetcli_printTypedCharacter(const char rxChar)
{
    char consolePrint[2];
    consolePrint[0] = rxChar;
    consolePrint[1] = '\0';
    EnetAppUtils_print(consolePrint);
}

void Enetcli_read(char *pRxBuffer, const uint32_t rxBufferLen)
{
    char rxChar;
    bool isEnd = BFALSE;
    uint8_t isArrow = 0;
    /* Initiate read */
    while (!isEnd)
    {
        rxChar = EnetAppUtils_getChar();

        /* End of command */
        if ((strlen(pRxBuffer) > 0) && ((rxChar == '\n') || (strlen(pRxBuffer) > rxBufferLen)))
        {
            isEnd = 1;
            pRxBuffer[strlen(pRxBuffer)] = '\0';
            EnetAppUtils_print(UART_endOfLine);
        }

        /* Handle backspaces */
        else if (rxChar == '\b')
        {
            if (strlen(pRxBuffer) != 0)
            {
                Enetcli_backspace();
                pRxBuffer[strlen(pRxBuffer) - 1] = '\0';
            }
        }

        /* Ignore arrow keys */
        else if ((rxChar == '\x1b') || (isArrow != 0))
        {
            isArrow++;
            if (isArrow == 3)
            {
                isArrow = 0;
            }
        }

        /* Alphanumeric and special characters will be appended to the receive buffer */
        else if(rxChar != '\n')
        {
            Enetcli_printTypedCharacter(rxChar);
            strncat(pRxBuffer, &rxChar, 1);
        }
        rxChar = 0x00;
    }
}

void Enetcli_write(char *txBuffer)
{
    EnetAppUtils_print(txBuffer);
}

void EnetApp_createClock(void)
{
    TaskP_Params taskParams;
    SemaphoreP_Params semParams;
    ClockP_Params clkParams;

    /* Initialize timer semaphore params */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_COUNTING;

    /* Create timer semaphore */
    EnetApp_inst.hTimerSem = SemaphoreP_create(0, &semParams);

    /* Reset the exitFlag */
    EnetApp_inst.exitFlag = BFALSE;

    /* Initialize the periodic tick task params */
    TaskP_Params_init(&taskParams);
    taskParams.priority = 7U;
    taskParams.stack = gEnetCliTaskStackTick;
    taskParams.stacksize = sizeof(gEnetCliTaskStackTick);
    taskParams.arg0 = (void *)EnetApp_inst.hTimerSem;
    taskParams.name = (const char *)"Periodic tick task";

    /* Create periodic tick task */
    EnetApp_inst.hTickTask = TaskP_create(&EnetApp_tickTask, &taskParams);
    if (EnetApp_inst.hTickTask == NULL)
    {
        EnetAppUtils_print("EnetApp_createClock() failed to create tick task\n");
        OS_stop();
    }

    ClockP_Params_init(&clkParams);
    clkParams.startMode = ClockP_StartMode_USER;
    clkParams.period = ENETAPP_PERIODIC_TICK_MS;
    clkParams.runMode = ClockP_RunMode_CONTINUOUS;
    clkParams.arg = (void *)EnetApp_inst.hTimerSem;

    /* Creating timer and setting timer callback function*/
    EnetApp_inst.hTickTimer = ClockP_create(EnetApp_timerCallback, &clkParams);
    if (EnetApp_inst.hTickTimer == NULL)
    {
        EnetAppUtils_print("EnetApp_createClock() failed to create clock\n");
        OS_stop();
    }
}

void EnetApp_deleteClock(void)
{
    EnetApp_inst.exitFlag = BTRUE;

    /* Delete periodic tick timer */
    if (EnetApp_inst.hTickTimer != NULL)
    {
        ClockP_delete(EnetApp_inst.hTickTimer);
        EnetApp_inst.hTickTimer = NULL;
    }

    /* Delete periodic tick task */
    if (EnetApp_inst.hTickTask != NULL)
    {
        EnetApp_inst.hTickTask = NULL;
    }

    /* Delete periodic tick timer */
    if (EnetApp_inst.hTimerSem != NULL)
    {
        SemaphoreP_delete(EnetApp_inst.hTimerSem);
        EnetApp_inst.hTimerSem = NULL;
    }
}

void EnetApp_tickTask(void *a0, void *a1)
{
    SemaphoreP_Handle hSem = (SemaphoreP_Handle)a0;

    while (!EnetApp_inst.exitFlag)
    {
        SemaphoreP_pend(hSem, SemaphoreP_WAIT_FOREVER);

        /* PeriodicTick should be called from non-ISR context */
        Enet_periodicTick(EnetApp_inst.hEnet);
    }
    EnetAppUtils_print("EnetApp_tickTask() exiting..\n");
}

void EnetApp_timerCallback(void *arg)
{
    SemaphoreP_Handle hSem = (SemaphoreP_Handle)arg;

    /* Tick! */
    SemaphoreP_post(hSem);
}

int32_t EnetApp_openDma()
{
    int32_t status = ENET_SOK;
    EnetUdma_OpenRxFlowPrms rxChCfg;
    EnetUdma_OpenTxChPrms txChCfg;

    /* Open the CPSW TX channel  */
    if (status == ENET_SOK)
    {
        EnetUdma_initTxChParams(&txChCfg);

        txChCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;
        txChCfg.cbArg = &EnetApp_inst.txSemObj[0];
        txChCfg.notifyCb = EnetApp_txIsrFxn;

        EnetAppUtils_setCommonTxChPrms(&txChCfg);

        EnetAppUtils_openTxCh(EnetApp_inst.hEnet,
                              EnetApp_inst.coreKey,
                              EnetApp_inst.coreId,
                              &EnetApp_inst.txChNum,
                              &EnetApp_inst.hTxCh[0],
                              &txChCfg);
        if (NULL != EnetApp_inst.hTxCh[0])
        {
            status = EnetDma_enableTxEvent(EnetApp_inst.hTxCh[0]);
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
        }
    }

    /* Open the CPSW RX flow  */
    if (status == ENET_SOK)
    {
        EnetUdma_initRxFlowParams(&rxChCfg);

        rxChCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;
        rxChCfg.notifyCb = EnetApp_rxIsrFxn;
        rxChCfg.cbArg = (void *)&EnetApp_inst.rxSemObj[0];

        EnetAppUtils_setCommonRxFlowPrms(&rxChCfg);
        EnetAppUtils_openRxFlow(EnetApp_inst.enetType,
                                EnetApp_inst.hEnet,
                                EnetApp_inst.coreKey,
                                EnetApp_inst.coreId,
                                BTRUE,
                                &EnetApp_inst.rxStartFlowIdx,
                                &EnetApp_inst.rxFlowIdx,
                                &EnetApp_inst.hostMacAddr[0U],
                                &EnetApp_inst.hRxCh[0],
                                &rxChCfg);
        if (NULL == EnetApp_inst.hRxCh[0])
        {
            EnetAppUtils_print("EnetUdma_openRxFlow() failed to open: %d\n",
                               status);
            EnetAppUtils_assert(NULL != EnetApp_inst.hRxCh[0]);
        }
        else
        {
            EnetAppUtils_print("Host MAC address: ");
            EnetAppUtils_printMacAddr(EnetApp_inst.hostMacAddr);
            EnetApp_initRxReadyPktQ(0U);
        }
    }

    return status;
}

void EnetApp_closeDma()
{
    EnetDma_PktQ fqPktInfoQ;
    EnetDma_PktQ cqPktInfoQ;

    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    /* There should not be any ready packet */
    EnetAppUtils_assert(0U == EnetQueue_getQCount(&EnetApp_inst.rxReadyQ[0]));

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
                             EnetApp_inst.hRxCh[0]);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    /* Close TX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    EnetAppUtils_closeTxCh(EnetApp_inst.hEnet,
                           EnetApp_inst.coreKey,
                           EnetApp_inst.coreId,
                           &fqPktInfoQ,
                           &cqPktInfoQ,
                           EnetApp_inst.hTxCh[0],
                           EnetApp_inst.txChNum);
    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    EnetAppUtils_freePktInfoQ(&EnetApp_inst.rxFreeQ[0]);
    EnetAppUtils_freePktInfoQ(&EnetApp_inst.txFreePktInfoQ[0]);

    EnetMem_deInit();
}

void EnetApp_rxIsrFxn(void *appData)
{
    SemaphoreP_Handle local_hRxSem = *(SemaphoreP_Handle *)appData;
    SemaphoreP_post(local_hRxSem);
}

void EnetApp_txIsrFxn(void *appData)
{
    SemaphoreP_Handle local_hTxSem = *(SemaphoreP_Handle *)appData;
    SemaphoreP_post(local_hTxSem);
}

void EnetApp_deleteRxTxTasks(void)
{
    if (EnetApp_inst.hTxTask != NULL)
    {
        EnetApp_inst.hTxTask = NULL;
    }

    if (EnetApp_inst.hRxTask != NULL)
    {
        EnetApp_inst.hRxTask = NULL;
    }
    EnetAppUtils_print("EnetApp_deleteRxTxTasks() done..\n");
}

void EnetApp_initRxReadyPktQ(uint8_t dmaChNum)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_Pkt *pPktInfo;
    int32_t status;
    uint32_t i;
    uint32_t scatterSegments[] = {
    ENET_MEM_LARGE_POOL_PKT_SIZE, };

    #if ENET_RX_FLOWS_NUM >= 1
        EnetApp_pktPerRxCh[0] = ENET_DMA_RX_CH0_NUM_PKTS;
    #endif
    #if ENET_RX_FLOWS_NUM >= 2
        EnetApp_pktPerRxCh[1] = ENET_DMA_RX_CH1_NUM_PKTS;
    #endif
    #if ENET_RX_FLOWS_NUM >= 3
        EnetApp_pktPerRxCh[2] = ENET_DMA_RX_CH2_NUM_PKTS;
    #endif
    #if ENET_RX_FLOWS_NUM >= 4
        EnetApp_pktPerRxCh[3] = ENET_DMA_RX_CH3_NUM_PKTS;
    #endif

    EnetQueue_initQ(&EnetApp_inst.rxFreeQ[dmaChNum]);
    EnetQueue_initQ(&EnetApp_inst.rxReadyQ[dmaChNum]);
    EnetQueue_initQ(&rxReadyQ);

    for (i = 0U; i < EnetApp_pktPerRxCh[dmaChNum]; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&EnetApp_inst,
                                       ENETDMA_CACHELINE_ALIGNMENT,
                                       ENET_ARRAYSIZE(scatterSegments),
                                       scatterSegments);
        EnetAppUtils_assert(pPktInfo != NULL);
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState,
                                     ENET_PKTSTATE_APP_WITH_FREEQ);
        EnetQueue_enq(&EnetApp_inst.rxFreeQ[dmaChNum], &pPktInfo->node);
    }

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(EnetApp_inst.hRxCh[dmaChNum], &rxReadyQ);
    EnetAppUtils_assert(status == ENET_SOK);
    /* There should not be any packet with DMA during init */
    EnetAppUtils_assert(EnetQueue_getQCount(&rxReadyQ) == 0U);

    EnetAppUtils_validatePacketState(&EnetApp_inst.rxFreeQ[dmaChNum],
                                     ENET_PKTSTATE_APP_WITH_FREEQ,
                                     ENET_PKTSTATE_APP_WITH_DRIVER);

    EnetDma_submitRxPktQ(EnetApp_inst.hRxCh[dmaChNum],
                         &EnetApp_inst.rxFreeQ[dmaChNum]);

    while (0U != EnetQueue_getQCount(&EnetApp_inst.rxFreeQ[dmaChNum]))
    {
        EnetDma_submitRxPktQ(EnetApp_inst.hRxCh[dmaChNum],
                             &EnetApp_inst.rxFreeQ[dmaChNum]);
    }
}

/* ========================================================================== */
/*                   Static Function Definitions                              */
/* ========================================================================== */

/* None */
