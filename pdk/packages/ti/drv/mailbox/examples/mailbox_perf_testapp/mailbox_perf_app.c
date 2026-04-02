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
 * \file  mailbox_perf_app.c
 * \brief This is a mailbox perf application that runs on each of the cores
 *        The different cores used by the application is configured in
 *        the mailbox_config files.
**/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
#include <ti/osal/osal.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/mailbox/mailbox.h>

#include "mailbox_config.h"
#include "mailbox_app.h"

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

#define LOCAL_DELAY_COUNT (0x10)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t gMsg[MAILBOX_MAX_INST] = {0u};
volatile int32_t  gMsgStatus       = MAILBOX_EINVAL;
volatile uint32_t gDone            = 1u;
volatile uint32_t gEndTask         = 0u;
volatile uint32_t gEndTaskNum      = 0u;
volatile uint32_t gEndTaskLast     = 0u;
volatile uint32_t gNumMessagesRecd = 0u;
volatile uint32_t gSendToken       = 0u;
volatile uint16_t gSelfId;
volatile uint32_t gEndApplication  = 0u;
volatile uint16_t gMasterId        = MAILBOX_INST_MPU1_0;
volatile uint32_t gSkipCoreId      = MAILBOX_INST_MPU1_0;
volatile uint32_t gInterruptMode   = 0u;
volatile Mbox_Handle gHandles[MAILBOX_MAX_INST];

/* minus 1 because don't need to keep data for self */
uint32_t gTimeIntArray[MAILBOX_MAX_INST-1][MAILBOX_SEND_NUM_MESSAGES] __attribute__ ((section(".benchmark_buffer"), aligned (8)));
uint32_t gTimePollArray[MAILBOX_MAX_INST-1][MAILBOX_SEND_NUM_MESSAGES] __attribute__ ((section(".benchmark_buffer"), aligned (8)));

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static void mailboxIsr(Mbox_Handle handle, Mailbox_Instance remoteEndpoint);
int perfTestSync(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
uint32_t perfTestGetTimerCount(void)
{

    uint32_t    val = 0;
#if defined(BUILD_MPU1_0)
    CSL_gtc_cfg1Regs *gtcRegs = (CSL_gtc_cfg1Regs *) CSL_GTC0_GTC_CFG1_BASE;

    /* Read GTC counter register */
    val = (uint32_t)gtcRegs->CNTCV_LO;
#endif
#ifdef BUILD_MCU
    val = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);
#endif
    return val;
}

int32_t perfTestInterruptInit(uint32_t intNum, uint16_t remoteId)
{
    int32_t retVal = 0;

    /* Register Mailbox interrupt */
    /* This is done in MB LLD */

    return retVal;

}

int32_t perfTestInit(void)
{
    int32_t retVal = 0;
    Mailbox_initParams initParam;
    Mailbox_openParams openParam;
    uint16_t remoteId;
    int32_t         errCode;
#ifdef SYSTEM_MASTER
#ifdef CHECK_PROFILE_TIMER
    int i;
    volatile uint32_t prevTimeStamp, curTimeStamp, timeStampDiff=0;

    /* Print 10 dots , with 1 seconds each dot */
    for (i=0; i < 10; i++) {
        prevTimeStamp = perfTestGetTimerCount();

        do
        {
            curTimeStamp = perfTestGetTimerCount();
            timeStampDiff = curTimeStamp - prevTimeStamp;
        } while(timeStampDiff < MAILBOX_TEST_TIMER_FREQ_IN_HZ);
        MailboxAppPrint(".");
    }
#endif
#endif

#ifdef BUILD_MPU1_0
    gSelfId = MAILBOX_INST_MPU1_0;
#endif

#ifdef BUILD_MCU1_0
    gSelfId = MAILBOX_INST_MCU1_0;
#endif

#ifdef BUILD_MCU1_1
    gSelfId = MAILBOX_INST_MCU1_1;
#endif

#ifdef BUILD_MCU2_0
    gSelfId = MAILBOX_INST_MCU2_0;
#endif

#ifdef BUILD_MCU2_1
    gSelfId = MAILBOX_INST_MCU2_1;
#endif

#ifdef BUILD_M4F_0
    gSelfId = MAILBOX_INST_M4F_0;
#endif

    /* Setup the default Mailbox init Parameters */
    Mailbox_initParams_init(&initParam);
    initParam.localEndpoint = gSelfId;

    /* Initialize the Mailbox */
    Mailbox_init(&initParam);

    /* Setup the default Mailbox open Parameters */
    Mailbox_openParams_init(&openParam);
    if (gInterruptMode == 1u)
    {
        openParam.cfg.writeMode = MAILBOX_MODE_FAST;
        openParam.cfg.readMode = MAILBOX_MODE_FAST;
        openParam.cfg.readCallback = mailboxIsr;
        openParam.cfg.enableVIMDirectInterrupt = true;
    }
    else
    {
        openParam.cfg.writeMode = MAILBOX_MODE_FAST;
        openParam.cfg.readMode = MAILBOX_MODE_FAST;
        openParam.cfg.readCallback = NULL;
        openParam.cfg.enableVIMDirectInterrupt = true;
    }

    /* Send sync message to all cores */
    for (remoteId = 0; remoteId < MAILBOX_MAX_INST; remoteId++)
    {
        /* Skip self */
        if (remoteId == gSelfId)
        {
            continue;
        }
#ifdef BUILD_RTOS
        /* M4F is not supported for RTOS */
        if (remoteId == MAILBOX_INST_M4F_0)
        {
            continue;
        }
#endif
        openParam.remoteEndpoint = remoteId;

        /* Open the  Instance */
        gHandles[remoteId] = Mailbox_open(&openParam, &errCode);

        if (gHandles[remoteId] == NULL)
        {
            MailboxAppPrint("Error: Unable to open the Mailbox Instance\n");
            return -1;
        }
    }

    if (gInterruptMode == 0)
    {
        retVal = perfTestSync();
        if ( retVal != 0)
        {
            MailboxAppPrint("Failed perfTestSync \r\n");
            return -1;
        }
    }

    return retVal;
}


int32_t perfTestInterruptModeInit(void)
{
    int32_t retVal = 0;
    uint16_t remoteId;

    /* Close all open handles */
    for (remoteId = 0; remoteId < MAILBOX_MAX_INST; remoteId++)
    {
        /* Skip self */
        if (remoteId == gSelfId)
        {
            continue;
        }
#ifdef BUILD_RTOS
        /* M4F is not supported for RTOS */
        if (remoteId == MAILBOX_INST_M4F_0)
        {
            continue;
        }
#endif

        Mailbox_close(gHandles[remoteId]);
    }

    gInterruptMode = 1u;

    /* Re-init in Interrupt Mode */
    perfTestInit();

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

static void mailboxIsr(Mbox_Handle handle, Mailbox_Instance remoteEndpoint)
{
    uint32_t msg = MAILBOX_APP_MSG_TO_SEND;
#ifdef DETAILED_PROFILING
    readTimerStartCount0 = perfTestGetTimerCount();
#endif

    Mailbox_read(handle, (uint8_t *)&gMsg[remoteEndpoint], sizeof(uint32_t));
#ifdef DETAILED_PROFILING
    readTimerEndCount0 = perfTestGetTimerCount();
#endif
    switch(gMsg[remoteEndpoint])
    {
        case MAILBOX_APP_STOP_MESSAGE:
            gEndTask = 1u;
            gEndTaskNum++;
            break;

        case MAILBOX_APP_SEND_TOKEN_MESSAGE:
#ifdef SYSTEM_MASTER
            /* Now time to stop */
            gEndApplication = 1U;
#else
            gSendToken = 1U;
#endif
            gEndTask = 1u;
            gEndTaskNum++;
            break;

        default:
            /* If acting as a slave send message back */
            if (gSendToken != 1u)
            {
                Mailbox_write(handle, (uint8_t *)&msg, sizeof(uint32_t));
            }
            gNumMessagesRecd++;
            break;
    }
#ifdef DETAILED_PROFILING
    ackTimerStartCount0 = perfTestGetTimerCount();
#endif
#ifdef DETAILED_PROFILING
    ackTimerEndCount0 = perfTestGetTimerCount();
#endif

}

int perfTestSync(void)
{
    uint16_t remoteId;
    int retVal = 0;
    uint32_t msg = 0;

#ifdef SYSTEM_MASTER
    gSendToken = 1u;

    /* Send sync message to all cores */
    for (remoteId = 0; remoteId < MAILBOX_MAX_INST; remoteId++)
    {
        /* Skip self */
        if (remoteId == gSelfId)
        {
            continue;
        }
#ifdef BUILD_RTOS
        /* M4F is not supported for RTOS */
        if (remoteId == MAILBOX_INST_M4F_0)
        {
            continue;
        }
#endif
        msg = MAILBOX_APP_SYNC_MESSAGE;

        /* Send initial Sync message */
        Mailbox_write(gHandles[remoteId], (uint8_t *)&msg, sizeof(uint32_t));
    }

    /* Receive Ack message back for all cores */
    for (remoteId = 0; remoteId < MAILBOX_MAX_INST; remoteId++)
    {
        /* Skip self */
        if (remoteId == gSelfId)
        {
            continue;
        }
#ifdef BUILD_RTOS
        /* M4F is not supported for RTOS */
        if (remoteId == MAILBOX_INST_M4F_0)
        {
            continue;
        }
#endif
        if (gInterruptMode == 0u)
        {
            do
            {
                gMsgStatus = Mailbox_read(gHandles[remoteId], (uint8_t *)&gMsg[remoteId], sizeof(uint32_t));
            } while (gMsg[remoteId] != MAILBOX_APP_ACK_MESSAGE);
        }
        else
        {
            while (gMsg[remoteId] != MAILBOX_APP_ACK_MESSAGE);
        }
    }
#else
    remoteId = gMasterId;
    if (gInterruptMode == 0u)
    {
        do
        {
            gMsgStatus = Mailbox_read(gHandles[remoteId], (uint8_t *)&gMsg[remoteId], sizeof(uint32_t));
        } while (gMsg[remoteId] != MAILBOX_APP_SYNC_MESSAGE);
    }
    else
    {
        while (gMsg[remoteId] != MAILBOX_APP_SYNC_MESSAGE);
    }
    if ( retVal == 0)
    {
        msg = MAILBOX_APP_ACK_MESSAGE;
        /* Send initial Sync message */
        Mailbox_write(gHandles[remoteId], (uint8_t *)&msg, sizeof(uint32_t));
    }
#endif
    return retVal;
}

void perfTestSendReceiveMessages(uint16_t remoteId)
{
    uint32_t allTimerCount=0u;
    uint32_t allTimerCountWithoutFirst=0u;
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
    uint32_t totalTimerCount, minTotalTimerCount=0xffffffffu, maxTotalTimerCount=0u;
    uint32_t maxTotalTimerCountAfterFirst=0u;
#endif
    uint32_t * timeArray;
    uint32_t timeArrayIdx = (remoteId < gSelfId) ? remoteId : (remoteId - 1);
    uint32_t * timeArrayInt = gTimeIntArray[timeArrayIdx];
    uint32_t * timeArrayPoll = gTimePollArray[timeArrayIdx];

    int i;
    uint32_t lastNumMessagesRecd;
    uint32_t msg = MAILBOX_APP_MSG_TO_SEND;

    lastNumMessagesRecd = gNumMessagesRecd;

    if (gInterruptMode == 1u)
    {
        timeArray = timeArrayInt;
    }
    else
    {
        timeArray = timeArrayPoll;
    }

    for (i = 0; i < MAILBOX_SEND_NUM_MESSAGES; i++)
    {
#if defined(MEASURE_LATENCY_DISTRIBUTION) || defined(DETAILED_PROFILING)
        startTimerCount0 = perfTestGetTimerCount();
#endif
        Mailbox_write(gHandles[remoteId], (uint8_t *)&msg, sizeof(uint32_t));
#ifdef DETAILED_PROFILING
        midTimerCount0 = perfTestGetTimerCount();
#endif
        if (gInterruptMode == 1u)
        {
            /* Wait for a reply message to arrive */
            while(gNumMessagesRecd == lastNumMessagesRecd);
        }
        else
        {

            /* Wait for Messages */
            while(0u == Mailbox_GetMessageCount(gHandles[remoteId]));
#ifdef DETAILED_PROFILING
            readTimerStartCount0 = perfTestGetTimerCount();
#endif
            /* Get the message */
            Mailbox_read(gHandles[remoteId], (uint8_t *)&gMsg[remoteId], sizeof(uint32_t));
#ifdef DETAILED_PROFILING
            readTimerEndCount0 = perfTestGetTimerCount();
#endif
        }
#if defined(MEASURE_LATENCY_DISTRIBUTION) || defined(DETAILED_PROFILING)
        endTimerCount0 = perfTestGetTimerCount();
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
        timeArray[i] = totalTimerCount;
        allTimerCount += totalTimerCount;
        if (i > 0)
        {
            allTimerCountWithoutFirst += totalTimerCount;
        }
        minTotalTimerCount = (totalTimerCount < minTotalTimerCount) ? totalTimerCount : minTotalTimerCount;
        if (i > 0)
        {
            maxTotalTimerCountAfterFirst = (totalTimerCount > maxTotalTimerCountAfterFirst) ? totalTimerCount : maxTotalTimerCountAfterFirst;
        }
        maxTotalTimerCount = (totalTimerCount > maxTotalTimerCount) ? totalTimerCount : maxTotalTimerCount;
#endif

        lastNumMessagesRecd++;
    }

    if (gInterruptMode == 1u)
    {
        MailboxAppPrint("\nInterrupt mode: benchmark results \n");
    }
    else
    {
        MailboxAppPrint("\nPoll mode: benchmark results \n");
    }

    MailboxAppPrintf("\nSelf id: %d, Remote id: %d, Delta_time: %d\n",
                     gSelfId, remoteId, allTimerCount);

    clockDivideFactor = MAILBOX_TEST_TIMER_FREQ_IN_HZ/100000000;  /* This helps convert to 100 MHz scale */

    MailboxAppPrintf("\n Round trip Delay Average: %d in ns, %d in ns (without first run)\n",
             10*(allTimerCount)/(clockDivideFactor*MAILBOX_SEND_NUM_MESSAGES),
             10*(allTimerCountWithoutFirst)/(clockDivideFactor*(MAILBOX_SEND_NUM_MESSAGES-1)));
    MailboxAppPrintf("\n    For per-run values, dump %d bytes at address %p\n",
                     sizeof(uint32_t)*MAILBOX_SEND_NUM_MESSAGES, timeArray);
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
    MailboxAppPrintf("\n  Total time in ns: Min %d Max %d, Min %d Max %d (without first run) \n",
                     10*(minTotalTimerCount)/(clockDivideFactor),
                     10*(maxTotalTimerCount)/(clockDivideFactor),
                     10*(minTotalTimerCount)/(clockDivideFactor),
                     10*(maxTotalTimerCountAfterFirst)/(clockDivideFactor));
#endif
}

void perfTestLoopbackMessages(uint16_t remoteId)
{
    uint32_t msg = MAILBOX_APP_MSG_TO_SEND;

    while (!gEndTask)
    {
        while(0u == Mailbox_GetMessageCount(gHandles[remoteId]));

        Mailbox_read(gHandles[remoteId], (uint8_t *)&gMsg[remoteId], sizeof(uint32_t));

        if (gMsg[remoteId] == MAILBOX_APP_STOP_MESSAGE)
        {
            break;
        }
        if (gMsg[remoteId] == MAILBOX_APP_SEND_TOKEN_MESSAGE)
        {
#ifdef SYSTEM_MASTER
            /* Now time to stop */
            gEndApplication = 1U;
#else
            gSendToken = 1U;
#endif
            break;
        }

        Mailbox_write(gHandles[remoteId], (uint8_t *)&msg, sizeof(uint32_t));
    }
}

int32_t perfTestRun(void)
{
    int retVal = 0;
    uint16_t remoteId;
    uint32_t msg;
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
            for (remoteId = 0; remoteId < MAILBOX_MAX_INST; remoteId++)
            {
                if (remoteId == gSelfId)
                {
                    continue;
                }
#ifdef BUILD_RTOS
                /* M4F is not supported for RTOS */
                if (remoteId == MAILBOX_INST_M4F_0)
                {
                    continue;
                }
#endif
#ifdef SOC_AM64X
                if ((gSelfId == gSkipCoreId) || (remoteId == gSkipCoreId))
                {
                    continue;
                }
#endif
                perfTestSendReceiveMessages(remoteId);
            }
            /* Now pass the token to next processor */
            gTokenHolder++;
#ifdef BUILD_RTOS
            if (gTokenHolder == MAILBOX_INST_M4F_0)
            {
                gTokenHolder++;
            }
#endif
            if (gTokenHolder >= MAILBOX_MAX_INST)
            {
                    gTokenHolder = 0u;
            }

            for (remoteId = 0; remoteId < MAILBOX_MAX_INST; remoteId++)
            {
                if ((remoteId == gSelfId) || (remoteId == gTokenHolder))
                {
                    continue;
                }
#ifdef BUILD_RTOS
                /* M4F is not supported for RTOS */
                if (remoteId == MAILBOX_INST_M4F_0)
                {
                    continue;
                }
#endif
                /* Finish up and send end message */
                msg = MAILBOX_APP_STOP_MESSAGE;
                Mailbox_write(gHandles[remoteId], (uint8_t *)&msg, sizeof(uint32_t));
            }
            /* Delay a bit */
            {
                volatile int j;
                for( j = 0; j < LOCAL_DELAY_COUNT; j++);
            }

            /* Pass the send token to next processor */
            msg = MAILBOX_APP_SEND_TOKEN_MESSAGE;
            Mailbox_write(gHandles[gTokenHolder], (uint8_t *)&msg, sizeof(uint32_t));

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
                while (gEndTaskNum <= gEndTaskLast);
                gEndTaskLast++;
            }
            else
            {
                perfTestLoopbackMessages(gTokenHolder);
            }

            gTokenHolder++;
#ifdef BUILD_RTOS
            if (gTokenHolder == MAILBOX_INST_M4F_0)
            {
                gTokenHolder++;
            }
#endif
            if (gTokenHolder >= MAILBOX_MAX_INST)
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
