/*
 *  Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 *
 */
/**
 * \file  mailbox_ipc_app.c
 * \brief This is a mailbox ipc application that runs on each of the cores
 *        The different cores used by the application is configured in
 *        the ipc_config files.
**/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/csl_mailbox.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/osal.h>
#include <ti/drv/sciclient/sciclient.h>

#include "ipc_config.h"
#include "mailbox_app.h"

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

#define LOCAL_DELAY_COUNT (0x10)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t gMsg             = 0u;
volatile uint32_t gMsgStatus       = MESSAGE_INVALID;
volatile uint32_t gDone            = 1u;
volatile uint32_t gEndTask         = 0u;
volatile uint32_t gNumMessagesRecd = 0u;
volatile uint32_t gSendToken       = 0u;
volatile uint16_t gSelfId;
volatile uint32_t gEndApplication  = 0u;
volatile uint16_t gMasterId        = MAILBOX_IPC_MPU1_0;
volatile uint32_t gSkipCoreId      = MAILBOX_IPC_MPU1_0;
volatile uint32_t gInterruptMode   = 0u;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
void mailboxIsr(uintptr_t handle);
int ipcAppSync(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
#if defined(BUILD_MCU) && defined(VIM_DIRECT_INTERRUPT_REGISTRATION)
__attribute__((interrupt("IRQ"))) __attribute__((target("arm"))) void mailboxIsr_0(void);
__attribute__((interrupt("IRQ"))) __attribute__((target("arm"))) void mailboxIsr_1(void);
__attribute__((interrupt("IRQ"))) __attribute__((target("arm"))) void mailboxIsr_2(void);

void mailboxIsr_0(void)
{
    mailboxIsr((uintptr_t)0u);
}

void mailboxIsr_1(void)
{
    mailboxIsr((uintptr_t)1u);
}

void mailboxIsr_2(void)
{
    mailboxIsr((uintptr_t)2u);
}

uintptr_t mailboxIsrArray[3] =
{
    (uintptr_t)&mailboxIsr_0,
    (uintptr_t)&mailboxIsr_1,
    (uintptr_t)&mailboxIsr_2,
};
#endif

uint32_t ipcGetTimerCount(void)
{

    uint32_t    val = 0;
#ifdef BUILD_MPU1_0
    CSL_gtc_cfg1Regs *gtcRegs = (CSL_gtc_cfg1Regs *) CSL_GTC0_GTC_CFG1_BASE;

    /* Read GTC counter register */
    val = (uint32_t)gtcRegs->CNTCV_LO;
#endif
#ifdef BUILD_MCU
    val = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);
#endif
    return val;
}

int32_t ipcInterruptInit(uint32_t intNum, uint16_t remoteId)
{
    int32_t retVal = 0;

    /* Register Mailbox interrupt */

#if defined(BARE_METAL) && defined(BUILD_MCU) && defined(VIM_DIRECT_INTERRUPT_REGISTRATION)
    CSL_vimCfgIntr((CSL_vimRegs *)(uintptr_t)TEST_VIM_BASE_ADDR, intNum,
                   0xFU,
                   (CSL_VimIntrMap)0u,
                   CSL_VIM_INTR_TYPE_LEVEL,
                   (uint32_t)mailboxIsrArray[remoteId] );
    CSL_vimSetIntrEnable((CSL_vimRegs *)(uintptr_t)TEST_VIM_BASE_ADDR,
                         intNum, true );   /* Enable interrupt in vim */
    Intc_SystemEnable();
#else
    OsalRegisterIntrParams_t intrPrms;
    OsalInterruptRetCode_e osalRetVal;
    HwiP_Handle hwiHandle;

    Osal_RegisterInterrupt_initParams(&intrPrms);
    intrPrms.corepacConfig.arg          = (uintptr_t)remoteId;
    intrPrms.corepacConfig.priority     = 1U;
    intrPrms.corepacConfig.corepacEventNum = 0U; /* NOT USED */
    intrPrms.corepacConfig.isrRoutine   = &mailboxIsr;
    intrPrms.corepacConfig.intVecNum    = intNum;

    osalRetVal = Osal_RegisterInterrupt(&intrPrms, &hwiHandle);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
       MailboxAppPrint("Error Could not register ISR !!!\n");
       retVal = -1;
    }

#endif
    return retVal;
}

int32_t ipcAppInit(void)
{
    int32_t retVal = 0;
#ifdef SYSTEM_MASTER
#ifdef CHECK_PROFILE_TIMER
    int i;
    volatile uint32_t prevTimeStamp, curTimeStamp, timeStampDiff=0;

    /* Print 10 dots , with 1 seconds each dot */
    for (i=0; i < 10; i++) {
        prevTimeStamp = ipcGetTimerCount();

        do
        {
            curTimeStamp = ipcGetTimerCount();
            timeStampDiff = curTimeStamp - prevTimeStamp;
        } while(timeStampDiff < MAILBOX_IPC_TIMER_FREQ_IN_HZ);
        MailboxAppPrint(".");
    }
#endif
#endif

#ifdef BUILD_MPU1_0
    gSelfId = MAILBOX_IPC_MPU1_0;
#endif

#ifdef BUILD_MCU1_0
    gSelfId = MAILBOX_IPC_MCU1_0;
#endif

#ifdef BUILD_MCU1_1
    gSelfId = MAILBOX_IPC_MCU1_1;
#endif

    retVal = ipcAppSync();
    if ( retVal != 0)
    {
        MailboxAppPrint("Failed ipcAppSync \r\n");
        return -1;
    }

    return retVal;
}


int32_t ipcAppInterruptModeInit(void)
{
    int32_t retVal = 0;
    uint16_t remoteId;
    uint16_t intNum;

#if defined(SOC_AM65XX) || defined(SOC_J721E)
    uint16_t interruptOffset;
    struct tisci_msg_rm_irq_set_req  rmIrqReq = {0};
    struct tisci_msg_rm_irq_set_resp rmIrqResp = {0};
    /* Enable interrupt router settings to connect interrupt event */
    struct tisci_msg_rm_get_resource_range_resp res = {0};
    struct tisci_msg_rm_get_resource_range_req  req = {0};
    uint16_t intStartNum, intRangeNum;
#endif

#if defined(SOC_AM65XX) || defined(SOC_J721E)
#ifdef BUILD_MPU1_0
    #define SELF_DEVICE_ID TISCI_DEV_GIC0
    req.type           = TISCI_DEV_NAVSS0_INTR_ROUTER_0;
    req.subtype        = TISCI_RESASG_SUBTYPE_IR_OUTPUT;
    req.secondary_host = (uint8_t)TISCI_HOST_ID_A53_0;
#endif

#ifdef BUILD_MCU1_0
    #define SELF_DEVICE_ID TISCI_DEV_MCU_ARMSS0_CPU0
    req.type           = TISCI_DEV_MAIN2MCU_LVL_INTRTR0;
    req.subtype        = TISCI_RESASG_SUBTYPE_IR_OUTPUT;
    req.secondary_host = (uint8_t)TISCI_HOST_ID_R5_0;
#endif

#ifdef BUILD_MCU1_1
    #define SELF_DEVICE_ID TISCI_DEV_MCU_ARMSS0_CPU1
    req.type           = TISCI_DEV_MAIN2MCU_LVL_INTRTR0;
    req.subtype        = TISCI_RESASG_SUBTYPE_IR_OUTPUT;
    req.secondary_host = (uint8_t)TISCI_HOST_ID_R5_2;
#endif

    /* Get interrupt number range */
    retVal =  Sciclient_rmGetResourceRange(
              &req,
              &res,
              MAILBOX_SCICLIENT_TIMEOUT);
    if (CSL_PASS != retVal || res.range_num == 0) {
        /* Try with HOST_ID_ALL */
        req.secondary_host = TISCI_HOST_ID_ALL;

        retVal = Sciclient_rmGetResourceRange(
                 &req,
                 &res,
                 MAILBOX_SCICLIENT_TIMEOUT);
    }
    if (CSL_PASS == retVal)
    {
        intRangeNum = res.range_num;
        if (intRangeNum == 0)
        {
            retVal = -2;
        }
    }

    if (CSL_PASS == retVal)
    {
        /* Translation must happen after this offset */
        retVal = Sciclient_rmIrqTranslateIrOutput(req.type,
                                                  res.range_start,
                                                  SELF_DEVICE_ID,
                                                  &intStartNum);
        if(CSL_PASS != retVal)
        {
            retVal = -3;
        }
    }
#endif
    if (retVal == 0)
    {
#if defined(SOC_AM65XX) || defined(SOC_J721E)
#ifdef BUILD_MPU1_0
        rmIrqReq.dst_id         = TISCI_DEV_GIC0;
        rmIrqReq.secondary_host = TISCI_HOST_ID_A53_0;
#endif

#ifdef BUILD_MCU1_0
        rmIrqReq.dst_id         = TISCI_DEV_MCU_ARMSS0_CPU0;
        rmIrqReq.secondary_host = TISCI_HOST_ID_R5_0;
#endif

#ifdef BUILD_MCU1_1
        rmIrqReq.dst_id         = TISCI_DEV_MCU_ARMSS0_CPU1;
        rmIrqReq.secondary_host = TISCI_HOST_ID_R5_2;
#endif
#endif
#if defined(SOC_AM65XX) || defined(SOC_J721E)
        interruptOffset = 0;
#endif
        for (remoteId = 0; remoteId < MAILBOX_IPC_MAX_PROCS; remoteId++)
        {
            /* Skip self */
            if (remoteId == gSelfId)
            {
                continue;
            }
#ifdef SOC_AM64X
            intNum = gMailboxIpc_MailboxInterruptInfo[remoteId];
#endif

#if defined(SOC_AM65XX) || defined(SOC_J721E)
            intNum = intStartNum + interruptOffset;
            /* Store the interrupt number */
            gMailboxIpc_MailboxInterruptInfo[remoteId] = intNum;

            if (retVal == 0)
            {

                rmIrqReq.ia_id                  = 0U;
                rmIrqReq.vint                   = 0U;
                rmIrqReq.global_event           = 0U;
                rmIrqReq.vint_status_bit_index  = 0U;

                rmIrqReq.valid_params   = TISCI_MSG_VALUE_RM_DST_ID_VALID
                                          | TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID
                                          | TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID;
                rmIrqReq.src_id         = gMailboxIpc_MailboxClusterIdArray[gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.cluster];
                rmIrqReq.src_index      = gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.user;
                rmIrqReq.dst_host_irq   = intNum;

                /* Config event */
                retVal = Sciclient_rmIrqSet(
                             &rmIrqReq, &rmIrqResp, MAILBOX_SCICLIENT_TIMEOUT);
            }
#endif
            if (retVal == 0)
            {
                /* Configure and initalize interrupt handler */
                retVal = ipcInterruptInit(intNum, remoteId);
            }

            if (retVal == 0)
            {
                /* Enable Interrupt at the mailbox */
                MailboxEnableNewMsgInt(gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.cluster],
                                       gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.user,
                                       gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.fifo);

            }
            if ( retVal != 0)
            {
                break;
            }
#if defined(SOC_AM65XX) || defined(SOC_J721E)
            interruptOffset++;
#endif
        }
    }

    if (retVal == 0)
    {
        /* Reset all global variables */
        gEndTask = 0u;
        gEndApplication = 0u;
        gInterruptMode = 1u;
#ifdef SYSTEM_MASTER
        gSendToken = 1u;
#else
        gSendToken = 0u;
#endif
    }
    return retVal;
}

#ifdef DETAILED_PROFILING
uint32_t readTimerStartCount0, readTimerEndCount0;
uint32_t ackTimerStartCount0, ackTimerEndCount0;
#endif

void mailboxIsr(uintptr_t handle)
{
    int16_t remoteId;
    uint32_t baseAddr;
    uint32_t fifo;
    uint32_t user;

    remoteId = (uint16_t)((uintptr_t)handle);

    baseAddr = gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.cluster];
    fifo = gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.fifo;
    user = gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.user;

#ifdef DETAILED_PROFILING
    readTimerStartCount0 = ipcGetTimerCount();
#endif

    MailboxReadMessage(baseAddr, fifo, (uint32_t *)&gMsg);
#ifdef DETAILED_PROFILING
    readTimerEndCount0 = ipcGetTimerCount();
#endif
    switch(gMsg)
    {
        case MAILBOX_APP_STOP_MESSAGE:
            gEndTask = 1u;
            break;

        case MAILBOX_APP_SEND_TOKEN_MESSAGE:
#ifdef SYSTEM_MASTER
            /* Now time to stop */
            gEndApplication = 1U;
#else
            gSendToken = 1U;
#endif
            gEndTask = 1u;
            break;

        default:
            /* If acting as a slave send message back */
            if (gSendToken != 1u)
            {
                MailboxWriteMessage(gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.cluster],
                                   gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.fifo,
                                   (uint32_t) MAILBOX_APP_MSG_TO_SEND);
            }
            gNumMessagesRecd++;
            break;
    }
#ifdef DETAILED_PROFILING
    ackTimerStartCount0 = ipcGetTimerCount();
#endif
    /* clear mlb intr */
    MailboxClrNewMsgStatus(baseAddr, fifo, user);
#if defined(BUILD_MCU) && defined(VIM_DIRECT_INTERRUPT_REGISTRATION)
    CSL_vimClrIntrPending((CSL_vimRegs *)(uintptr_t)TEST_VIM_BASE_ADDR,
                           gMailboxIpc_MailboxInterruptInfo[remoteId]);
    /* Acknowledge interrupt servicing */
    CSL_vimAckIntr((CSL_vimRegs *)(uintptr_t)TEST_VIM_BASE_ADDR, \
                    (CSL_VimIntrMap)0u );

#endif
#ifdef DETAILED_PROFILING
    ackTimerEndCount0 = ipcGetTimerCount();
#endif

}

int ipcAppSync(void)
{
    uint16_t remoteId;
    int retVal = 0;

#ifdef SYSTEM_MASTER
    gSendToken = 1u;

    /* Send sync message to all cores */
    for (remoteId = 0; remoteId < MAILBOX_IPC_MAX_PROCS; remoteId++)
    {
        /* Skip self */
        if (remoteId == gSelfId)
        {
            continue;
        }
        /* Send initial Sync message */
        MailboxSendMessage(gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.cluster],
                           gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.fifo,
                           (uint32_t) MAILBOX_APP_SYNC_MESSAGE);
    }

    /* Receive Ack message back for all cores */
    for (remoteId = 0; remoteId < MAILBOX_IPC_MAX_PROCS; remoteId++)
    {
        /* Skip self */
        if (remoteId == gSelfId)
        {
            continue;
        }
        do
        {
            gMsgStatus = MailboxGetMessage(gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.cluster],
                                           gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.fifo,
                                           (uint32_t *) &gMsg);
        } while (gMsgStatus == MESSAGE_INVALID);
        if (gMsg != MAILBOX_APP_ACK_MESSAGE)
        {
            retVal = -1;
        }
    }
#else
    remoteId = gMasterId;
    do
    {
        gMsgStatus = MailboxGetMessage(gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.cluster],
                                       gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.fifo,
                                       (uint32_t *) &gMsg);
    } while (gMsgStatus == MESSAGE_INVALID);
    if (gMsg != MAILBOX_APP_SYNC_MESSAGE)
    {
        retVal = -1;
    }
    if ( retVal == 0)
    {
        /* Send initial Sync message */
        MailboxSendMessage(gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.cluster],
                           gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.fifo,
                           (uint32_t) MAILBOX_APP_ACK_MESSAGE);
    }
#endif
    return retVal;
}

void ipcSendReceiveMessages(uint16_t remoteId)
{
    uint32_t startTimerCount, endTimerCount;
    uint32_t clockDivideFactor;
#ifdef DETAILED_PROFILING
    uint32_t midTimerCount0;
    uint32_t rxTimerCount, minRxTimerCount=0xffffffffu, maxRxTimerCount=0u, averageRxTimerCount=0u;
    uint32_t txTimerCount, minTxTimerCount=0xffffffffu, maxTxTimerCount=0u, averageTxTimerCount=0u;
    uint32_t readonlyTimerCount, minReadonlyTimerCount=0xffffffffu, maxReadonlyTimerCount=0u, averageReadonlyTimerCount=0u;
    uint32_t intAckTimerCount, minIntAckTimerCount=0xffffffffu, maxIntAckTimerCount=0u, averageIntAckTimerCount=0u;
    uint32_t intRestoreTimerCount, minIntRestoreTimerCount=0xffffffffu, maxIntRestoreTimerCount=0u, averageIntRestoreTimerCount=0u;
#endif
#if defined(MEASURE_LATENCY_DISTRIBUTION) || defined(DETAILED_PROFILING)
    uint32_t startTimerCount0, endTimerCount0;
    uint32_t totalTimerCount, minTotalTimerCount=0xffffffffu, maxTotalTimerCount=0u, averageTotalTimerCount=0u;
#endif


    int i;
    uint32_t lastNumMessagesRecd;
    uint32_t baseAddr, fifo;
    uint32_t remoteBaseAddr, remoteFifo;

    lastNumMessagesRecd = gNumMessagesRecd;

    remoteBaseAddr = gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.cluster];
    remoteFifo = gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.fifo;

    baseAddr = gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.cluster];
    fifo = gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.fifo;

    startTimerCount = ipcGetTimerCount();
    for (i = 0; i < MAILBOX_SEND_NUM_MESSAGES; i++)
    {
#if defined(MEASURE_LATENCY_DISTRIBUTION) || defined(DETAILED_PROFILING)
        startTimerCount0 = ipcGetTimerCount();
#endif
        MailboxWriteMessage(remoteBaseAddr,
                           remoteFifo,
                           (uint32_t) MAILBOX_APP_MSG_TO_SEND);
#ifdef DETAILED_PROFILING
        midTimerCount0 = ipcGetTimerCount();
#endif
        if (gInterruptMode == 1u)
        {
            /* Wait for a reply message to arrive */
            while(gNumMessagesRecd == lastNumMessagesRecd);
        }
        else
        {

            /* Wait for Messages */
            while(0u == MailboxGetMessageCount(baseAddr, fifo));
#ifdef DETAILED_PROFILING
            readTimerStartCount0 = ipcGetTimerCount();
#endif
            /* Get the message */
            MailboxReadMessage(baseAddr,
                               fifo,
                               (uint32_t *) &gMsg);
#ifdef DETAILED_PROFILING
            readTimerEndCount0 = ipcGetTimerCount();
#endif
        }
#if defined(MEASURE_LATENCY_DISTRIBUTION) || defined(DETAILED_PROFILING)
        endTimerCount0 = ipcGetTimerCount();
#endif

#ifdef DETAILED_PROFILING

        /* Calculate the time deltas */
        txTimerCount = midTimerCount0 - startTimerCount0;
        rxTimerCount = endTimerCount0 - midTimerCount0;
        minRxTimerCount = (rxTimerCount < minRxTimerCount) ? rxTimerCount : minRxTimerCount;
        maxRxTimerCount = (rxTimerCount > maxRxTimerCount) ? rxTimerCount : maxRxTimerCount;
        averageRxTimerCount = (rxTimerCount + averageRxTimerCount) >> 1;
        minTxTimerCount = (txTimerCount < minTxTimerCount) ? txTimerCount : minTxTimerCount;
        maxTxTimerCount = (txTimerCount > maxTxTimerCount) ? txTimerCount : maxTxTimerCount;
        averageTxTimerCount = (txTimerCount + averageTxTimerCount) >> 1;
        readonlyTimerCount = readTimerEndCount0 - readTimerStartCount0;
        minReadonlyTimerCount = (readonlyTimerCount < minReadonlyTimerCount) ? readonlyTimerCount : minReadonlyTimerCount;
        maxReadonlyTimerCount = (readonlyTimerCount > maxReadonlyTimerCount) ? readonlyTimerCount : maxReadonlyTimerCount;
        averageReadonlyTimerCount = (readonlyTimerCount + averageReadonlyTimerCount) >> 1;
        if (gInterruptMode == 1u)
        {
            intAckTimerCount = ackTimerEndCount0 - ackTimerStartCount0;
            minIntAckTimerCount = (intAckTimerCount < minIntAckTimerCount) ? intAckTimerCount : minIntAckTimerCount;
            maxIntAckTimerCount = (intAckTimerCount > maxIntAckTimerCount) ? intAckTimerCount : maxIntAckTimerCount;
            averageIntAckTimerCount = (intAckTimerCount + averageIntAckTimerCount) >> 1;
            intRestoreTimerCount = endTimerCount0 - ackTimerEndCount0;
            minIntRestoreTimerCount = (intRestoreTimerCount < minIntRestoreTimerCount) ? intRestoreTimerCount : minIntRestoreTimerCount;
            maxIntRestoreTimerCount = (intRestoreTimerCount > maxIntRestoreTimerCount) ? intRestoreTimerCount : maxIntRestoreTimerCount;
            averageIntRestoreTimerCount = (intRestoreTimerCount + averageIntRestoreTimerCount) >> 1;
        }
#endif
#if defined(MEASURE_LATENCY_DISTRIBUTION) || defined(DETAILED_PROFILING)
        totalTimerCount = endTimerCount0 - startTimerCount0;
        minTotalTimerCount = (totalTimerCount < minTotalTimerCount) ? totalTimerCount : minTotalTimerCount;
        maxTotalTimerCount = (totalTimerCount > maxTotalTimerCount) ? totalTimerCount : maxTotalTimerCount;
        averageTotalTimerCount = (totalTimerCount + averageTotalTimerCount) >> 1;
#endif

        lastNumMessagesRecd++;
    }
    endTimerCount = ipcGetTimerCount();

    if (gInterruptMode == 1u)
    {
        MailboxAppPrint("\nInterrupt mode: benchmark results \n");
    }
    else
    {
        MailboxAppPrint("\nPoll mode: benchmark results \n");
    }

    MailboxAppPrintf("\nSelf id: %d, Remote id: %d, Delta_time: %d\n",
                     gSelfId, remoteId, endTimerCount-startTimerCount);

    clockDivideFactor = MAILBOX_IPC_TIMER_FREQ_IN_HZ/100000000;  /* This helps convert to 100 MHz scale */

    MailboxAppPrintf("\n Round trip Delay Average: %d in ns\n",
             10*(endTimerCount-startTimerCount)/(clockDivideFactor*MAILBOX_SEND_NUM_MESSAGES));
#ifdef DETAILED_PROFILING

    MailboxAppPrintf("\n  Tx time in ns: Min %d Max %d average %d \n",
                     10*(minTxTimerCount)/(clockDivideFactor),
                     10*(maxTxTimerCount)/(clockDivideFactor),
                     10*(averageTxTimerCount)/(clockDivideFactor));
    MailboxAppPrintf("\n  Rx time in ns: Min %d Max %d average %d \n",
                     10*(minRxTimerCount)/(clockDivideFactor),
                     10*(maxRxTimerCount)/(clockDivideFactor),
                     10*(averageRxTimerCount)/(clockDivideFactor));
    MailboxAppPrintf("\n  Read only time in ns: Min %d Max %d average %d \n",
                     10*(minReadonlyTimerCount)/(clockDivideFactor),
                     10*(maxReadonlyTimerCount)/(clockDivideFactor),
                     10*(averageReadonlyTimerCount)/(clockDivideFactor));
    if (gInterruptMode == 1u)
    {
        MailboxAppPrintf("\n  Int Ack time in ns: Min %d Max %d average %d \n",
                         10*(minIntAckTimerCount)/(clockDivideFactor),
                         10*(maxIntAckTimerCount)/(clockDivideFactor),
                         10*(averageIntAckTimerCount)/(clockDivideFactor));
        MailboxAppPrintf("\n  Int Restore time in ns: Min %d Max %d average %d \n",
                         10*(minIntRestoreTimerCount)/(clockDivideFactor),
                         10*(maxIntRestoreTimerCount)/(clockDivideFactor),
                         10*(averageIntRestoreTimerCount)/(clockDivideFactor));
    }
#endif
#if defined(MEASURE_LATENCY_DISTRIBUTION) || defined(DETAILED_PROFILING)
    MailboxAppPrintf("\n  Total time in ns: Min %d Max %d average %d \n",
                     10*(minTotalTimerCount)/(clockDivideFactor),
                     10*(maxTotalTimerCount)/(clockDivideFactor),
                     10*(averageTotalTimerCount)/(clockDivideFactor));
#endif
}

void ipcLoopbackMessages(uint16_t remoteId)
{
    uint32_t baseAddr, fifo;
    uint32_t remoteBaseAddr, remoteFifo;


    remoteBaseAddr = gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.cluster];
    remoteFifo = gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.fifo;
    baseAddr = gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.cluster];
    fifo = gMailboxIpc_MailboxInfo[gSelfId][remoteId].rx.fifo;

    while (!gEndTask)
    {
        while(0u == MailboxGetMessageCount(baseAddr, fifo));

        MailboxReadMessage(baseAddr, fifo, (uint32_t *)&gMsg);

        if (gMsg == MAILBOX_APP_STOP_MESSAGE)
        {
            break;
        }
        if (gMsg == MAILBOX_APP_SEND_TOKEN_MESSAGE)
        {
#ifdef SYSTEM_MASTER
            /* Now time to stop */
            gEndApplication = 1U;
#else
            gSendToken = 1U;
#endif
            break;
        }

        MailboxWriteMessage(remoteBaseAddr, remoteFifo, (uint32_t) MAILBOX_APP_MSG_TO_SEND);
    }
}

int32_t ipcAppRun(void)
{
    int retVal = 0;
    uint16_t remoteId;
    volatile uint16_t gTokenHolder = gMasterId;

    do
    {
        if (gSendToken)
        {
            /* Delay a bit */
            {
                volatile int j;
                for( j = 0; j < LOCAL_DELAY_COUNT; j++);
            }
            if (gInterruptMode == 1u)
            {
                MailboxAppPrintf("\nInterrupt mode: Starting Send and receive messages on %d \n",
                                 gSelfId);
            }
            else
            {
                MailboxAppPrintf("\nPoll mode: Starting Send and receive messages on %d \n",
                                 gSelfId);
            }

            /* Communicate with each of the cores */
            for (remoteId = 0; remoteId < MAILBOX_IPC_MAX_PROCS; remoteId++)
            {
                if (remoteId == gSelfId)
                {
                    continue;
                }
#ifdef SOC_AM64X
                if ((gSelfId == gSkipCoreId) || (remoteId == gSkipCoreId))
                {
                    continue;
                }
#endif
                ipcSendReceiveMessages(remoteId);
            }
                /* Now pass the token to next processor */
            gTokenHolder++;
            if (gTokenHolder >= MAILBOX_IPC_MAX_PROCS)
            {
                    gTokenHolder = 0u;
            }

            for (remoteId = 0; remoteId < MAILBOX_IPC_MAX_PROCS; remoteId++)
            {
                if ((remoteId == gSelfId) || (remoteId == gTokenHolder))
                {
                    continue;
                }
                /* Finish up and send end message */
                MailboxSendMessage(gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.cluster],
                                   gMailboxIpc_MailboxInfo[remoteId][gSelfId].rx.fifo,
                                   (uint32_t) MAILBOX_APP_STOP_MESSAGE);

            }
            /* Delay a bit */
            {
                volatile int j;
                for( j = 0; j < LOCAL_DELAY_COUNT; j++);
            }

            /* Pass the send token to next processor */
            MailboxSendMessage(gMailboxIpc_MailboxBaseAddressArray[gMailboxIpc_MailboxInfo[gTokenHolder][gSelfId].rx.cluster],
                               gMailboxIpc_MailboxInfo[gTokenHolder][gSelfId].rx.fifo,
                               (uint32_t) MAILBOX_APP_SEND_TOKEN_MESSAGE);

            if (gTokenHolder == gMasterId)
            {
                /* Now time to stop */
                gEndApplication = 1U;
            }

            gSendToken = 0u;
        }
        else
        {
            gEndTask = 0u;
            if (gInterruptMode == 1u)
            {
                while (!gEndTask);
            }
            else
            {
                ipcLoopbackMessages(gTokenHolder);
            }

            gTokenHolder++;
            if (gTokenHolder >= MAILBOX_IPC_MAX_PROCS)
            {
                gTokenHolder = 0u;
            }
            if (gTokenHolder == gMasterId)
            {
                /* Now time to stop */
                gEndApplication = 1U;
            }
        }
    } while (!gEndApplication);

    return retVal;
}
/********************************* End of file ******************************/
