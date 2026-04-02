/**
 *  \file   fsi_app_cpucontrol.c
 *
 *  \brief  FSI CPU controlled data frame transfer test
 *
 *  The example sets up data frame transfers where trigger happens through CPU. 
 *  Automatic (Hw triggered) Ping frame transmission is also setup along
 *  with data. The example supports both interrupt mode and polling mode.
 *  
 *  The example works in internal loopback mode, no external connection is needed.
 *  For AM64x, each FSI_TX instance can be internally looped back up to 3 FSI_RX 
 *  instances (e.g. FSI_TX0 <--> FSI_RX0/1/2, FSI_TX1 <--> FSI_RX3/4/5)
 * 
 *  User can edit some of configuration parameters as per usecase. These are as
 *  below. Default values can be referred in code where these globals are defined
 * 
 *  nWords - Number of words per transfer may be from 1 -16
 *  nLanes - Choice to select single or double lane for frame transfers
 *  bufIdx - Index of FSI TX/RX buffer
 *  txUserData - User data to be sent with Data frame
 *  txDataFrameTag - Frame tag used for Data transfers
 *  txPingFrameTag - Frame tag used for Ping transfers
 *  txPingTimeRefCntr - Tx Ping timer reference counter
 *  rxWdTimeoutRefCntr - Rx Watchdog timeout reference counter
 *  loopCntr - TX/RX test loop counter
 * 
 *  For any errors during transfers i.e. error events such as Frame Overrun,
 *  Underrun, Watchdog timeout and CRC/EOF/TYPE errors, execution will stop
 *  immediately and status variables can be looked into for more details.
 *  Execution will also stop for any mismatch between received data and sent
 *  ones and also if transfers takes unusually long time (detected through
 *  software counters - txTimeOutCntr and rxTimeOutCntr)
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
 *
 */


#include "fsi_app_test.h"

/* FSI TXCLK */
#define FSI_APP_FSI_TXCLK_FREQ          (50000000U)   /* FSI TX clock */
#define FSI_APP_FSI_CLK_FREQ            (500000000U)  /* FSI module input clock */

/* Default FSI TX prescaler value */
#define FSI_APP_FSI_TX_PRESCALER_VAL    (FSI_APP_FSI_CLK_FREQ/FSI_APP_FSI_TXCLK_FREQ/2U)

/* Default FSI TX/RX completion timeout count */
#define FSI_APP_TXRX_TIMEOUT_COUNT      (0x100000U)

/* Default FSI TX/RX test loop count */
#define FSI_APP_TXRX_LOOP_COUNT         (1000U)

/* Enable benchmark profiling */
#define FSI_APP_PROFILE

/* Enable PMU Cycle measurement */
#ifdef FSI_APP_PROFILE
#ifdef BUILD_MCU
#define FSI_APP_PMU_PROFILE
#endif
#endif

/* App object data */
FSI_APP_Object fsiAppObj;

/*
 * User can modify these parameters as per usecase
 */

/* Number of words per transfer may be from 1 - 16 */
uint16_t nWords = 16U;

/* Index of FSI TX/RX buffer, bufIdx + nWords should be <= 16 */
uint16_t bufIdx = 0U;

/* Transfer can be happen over single or double lane */
FSI_DataWidth nLanes = FSI_DATA_WIDTH_1_LANE;


/* Frame tag used with Data/Ping transfers */
FSI_FrameTag txDataFrameTag = FSI_FRAME_TAG10;
FSI_FrameTag txPingFrameTag = FSI_FRAME_TAG15;

/* User data to be sent with Data frame */
uint16_t txUserData = 0x47U;

/* Tx Ping timer and Rx Watchdog reference counter values */
uint32_t txPingTimeRefCntr = 0x10000U;
uint32_t rxWdTimeoutRefCntr = 0x14000U;

/* flag to enable/disable Rx Frame Watchdog */
uint32_t isRxFrameWdEnable = TRUE;

/* TX/RX test loop count */
uint32_t loopCntr = FSI_APP_TXRX_LOOP_COUNT;

/*
 * This value can be anything suitable to generate a single interrupt event,
 * lower values may lead WD to trigger another event even before handler of 1st
 * one is not completed
 */
uint32_t rxFrameWdRefCntr = 0x1000000U;


#ifdef FSI_APP_PROFILE
static uint64_t lastRxTime;      /* Saved previous RX time stamp in usec */
static uint64_t txTotalTime[2];  /* Total TX time in usec */
static uint64_t rxTotalTime[2];  /* Total RX time in usec */
static uint64_t lbTotalTime;
#ifdef FSI_APP_PMU_PROFILE
#include <ti/csl/arch/r5/csl_arm_r5_pmu.h>
extern uint32_t osal_TimestampProvider_getOverflowCCNT(void);
static uint64_t lastCycle;       /* Saved previous cycle time */
static uint64_t txTotalCycle[2]; /* Total TX time in usec */
static uint64_t rxTotalCycle[2]; /* Total RX time in usec */
static uint64_t lbTotalCycle;
#endif
#endif

    
/* Compares two 16 bit values and increments global error flag by 1 for mismatch */
static inline uint32_t compare16(uint16_t val1, uint16_t val2)
{
    uint32_t status = TRUE;

    if(val1 != val2)
    {
        status = FALSE;
    }
    
    return status;
}

#ifdef FSI_APP_PMU_PROFILE
static uint64_t FSI_App_getPmuCycleCnt (uint32_t hiCycleCnt)
{
    uint32_t    ovsrStatus;
    uint32_t    hiCnt = hiCycleCnt;
    uint64_t    cycleCnt;
    

    cycleCnt = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);
    ovsrStatus = osal_TimestampProvider_getOverflowCCNT();
    if (ovsrStatus != 0U)
    {
        hiCnt++;
    }
    cycleCnt |= (uint64_t)hiCnt << 32U;

    return (cycleCnt);
}
#endif

/* Compares if received data is same as transmitted ones */
static uint32_t compareBufData(FSI_APP_Object *pObj, uint32_t baseAddr, uint16_t rxBufIndex, uint16_t nWords)
{
    uint16_t i;
#if defined (SOC_AM64X)
    uint64_t rxDataArray[4];
#else
    uint16_t rxDataArray[16];
#endif
    uint16_t *rxBufPtr;
    uint16_t *txBufPtr;
    uint32_t status = TRUE;
    uint64_t startTime;
    uint64_t startCycle;
    
#ifdef FSI_APP_PROFILE
    startTime = TimerP_getTimeInUsecs();
#ifdef FSI_APP_PMU_PROFILE
    startCycle = FSI_App_getPmuCycleCnt (lastCycle>>32U);
#endif                
#endif
    rxBufPtr = (uint16_t *)rxDataArray;
    FSI_readRxBuffer(baseAddr, rxBufPtr, nWords, rxBufIndex);
#ifdef FSI_APP_PROFILE
    rxTotalTime[1] += TimerP_getTimeInUsecs() - startTime;
#ifdef FSI_APP_PMU_PROFILE
    lastCycle = FSI_App_getPmuCycleCnt (startCycle>>32U);
    rxTotalCycle[1] += lastCycle - startCycle;
#endif                
#endif                    

    txBufPtr = (uint16_t *)pObj->txBufData;
    for(i = 0; i < nWords; i++)
    {
        if(*rxBufPtr != *txBufPtr)
        {
            status = FALSE;
            break;
        }
        rxBufPtr++;
        txBufPtr++;
    }
    
    return status;
}

/* Prepare TX data in Tx data buffer */
void prepareTxBufData(FSI_APP_Object *pObj)
{
    uint16_t  i;
    uint16_t *txBufPtr;

    txBufPtr = (uint16_t *)pObj->txBufData;
    for(i = 0; i < nWords; i++)
    {
        txBufPtr[i] = i + txBufPtr[nWords - 1U];
    }
}

/* Checks received frame type/tag and updates global error flag */
uint32_t checkReceivedFrameTypeTag(uint32_t baseAddr, FSI_FrameType type, FSI_FrameTag tag)
{
    uint32_t      status;
    FSI_FrameType rxFrameType;
    uint16_t      data;
    
    FSI_getRxFrameType(baseAddr, &rxFrameType);
    status = compare16((uint16_t)rxFrameType, (uint16_t)type);
    if (status == TRUE)
    {
        if (type == FSI_FRAME_TYPE_PING)
        {
            FSI_getRxPingTag(baseAddr, &data);
            status = compare16(data, (uint16_t)tag);
        }
        else
        {
            FSI_getRxFrameTag(baseAddr, &data);
            status = compare16(data, (uint16_t)tag);
        }
    }
    
    return (status);
}

#if defined (BUILD_MCU)
/* Disables all event interrupts in both FSI Tx/Rx, also clear them */
static void FSI_APP_disableAllInterrupts(FSI_APP_Object *pObj)
{
    uint32_t i;

    for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
    {
        FSI_disableTxInterrupt(pObj->txBaseAddr[i], FSI_INT1, FSI_TX_EVTMASK);
        FSI_disableTxInterrupt(pObj->txBaseAddr[i], FSI_INT2, FSI_TX_EVTMASK);
        FSI_clearTxEvents(pObj->txBaseAddr[i], FSI_TX_EVTMASK);
    }
    
    for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
    {
        FSI_disableRxInterrupt(pObj->rxBaseAddr[i], FSI_INT1, FSI_RX_EVTMASK);
        FSI_disableRxInterrupt(pObj->rxBaseAddr[i], FSI_INT2, FSI_RX_EVTMASK);
        FSI_clearRxEvents(pObj->rxBaseAddr[i], FSI_RX_EVTMASK);
    }
}

static void FSI_APP_txISR1 (uintptr_t arg, uint32_t txInst)
{
    FSI_APP_Object *pObj = (FSI_APP_Object *)arg;
    
    FSI_getTxEventStatus(pObj->txBaseAddr[txInst], &(pObj->txEventSts[txInst]));

    /* Clear the interrupt flag and issue ACK */
    FSI_clearTxEvents(pObj->txBaseAddr[txInst], FSI_TX_EVTMASK);

    pObj->txInt1Received[txInst] = TRUE;
}

static void FSI_APP_txISR2 (uintptr_t arg, uint32_t txInst)
{
    FSI_APP_Object *pObj = (FSI_APP_Object *)arg;

    FSI_getTxEventStatus(pObj->txBaseAddr[txInst], &(pObj->txEventSts[txInst]));

    /* Clear the interrupt flag and issue ACK */
    FSI_clearTxEvents(pObj->txBaseAddr[txInst], FSI_TX_EVTMASK);

    /*
     * INT2 line is set to fire for error events, stop immediately. Actual Error
     * is captured in txEventSts for debug
     */
    FSI_APP_disableAllInterrupts(pObj);

    pObj->txInt2Received[txInst] = TRUE;
}

static void FSI_APP_tx0ISR1 (uintptr_t arg)
{
    FSI_APP_txISR1(arg, 0U);
}

static void FSI_APP_tx0ISR2 (uintptr_t arg)
{
    FSI_APP_txISR2(arg, 0U);
}

static void FSI_APP_tx1ISR1 (uintptr_t arg)
{
    FSI_APP_txISR1(arg, 1U);
}

static void FSI_APP_tx1ISR2 (uintptr_t arg)
{
    FSI_APP_txISR2(arg, 1U);
}

static void FSI_APP_rxISR1 (uintptr_t arg, uint32_t rxInst)
{
    FSI_APP_Object *pObj = (FSI_APP_Object *)arg;
    uint32_t        status;
    uint16_t        rxUserData;

    FSI_getRxEventStatus(pObj->rxBaseAddr[rxInst], &(pObj->rxEventSts[rxInst]));

    /* Verify Frame attributes and data */
    status = checkReceivedFrameTypeTag(pObj->rxBaseAddr[rxInst], FSI_FRAME_TYPE_NWORD_DATA, txDataFrameTag);
    if (status == FALSE)
    {
        pObj->errorCntr++;
    }
    
    FSI_getRxUserDefinedData(pObj->rxBaseAddr[rxInst], &rxUserData);
    status = compare16(rxUserData, txUserData);
    if (status == FALSE)
    {
        pObj->errorCntr++;
    }
    status = compareBufData(pObj, pObj->rxBaseAddr[rxInst], bufIdx, nWords);
    if (status == FALSE)
    {
        pObj->errorCntr++;
    }

    /* Clear the interrupt flag and issue ACK */
    FSI_clearRxEvents(pObj->rxBaseAddr[rxInst], pObj->rxEventSts[rxInst]);

    /* Stop execution if errors encountered in frame verification */
    if(pObj->errorCntr != 0U)
    {
        FSI_APP_disableAllInterrupts(pObj);
    }

    pObj->rxInt1Received[rxInst] = 1U;
    pObj->dataFrameCntr[rxInst]++;
}

static void FSI_APP_rxISR2 (uintptr_t arg, uint32_t rxInst)
{
    FSI_APP_Object *pObj = (FSI_APP_Object *)arg;

    FSI_getRxEventStatus(pObj->rxBaseAddr[0], &(pObj->rxEventSts[rxInst]));
    /* Clear the interrupt flag and issue ACK*/
    FSI_clearRxEvents(pObj->rxBaseAddr[rxInst], pObj->rxEventSts[rxInst]);

    /* 
     * INT2 line is set to fire for error events, stop immediately. Error
     * is captured in rxEventSts for debug */
    FSI_APP_disableAllInterrupts(pObj);

    pObj->rxInt2Received[rxInst] = 1U;
}

static void FSI_APP_rx0ISR1 (uintptr_t arg)
{
    FSI_APP_rxISR1(arg, 0U);
}

static void FSI_APP_rx0ISR2 (uintptr_t arg)
{
    FSI_APP_rxISR2(arg, 0U);
}

static void FSI_APP_rx1ISR1 (uintptr_t arg)
{
    FSI_APP_rxISR1(arg, 1U);
}

static void FSI_APP_rx1ISR2 (uintptr_t arg)
{
    FSI_APP_rxISR2(arg, 1U);
}

static void FSI_APP_rx2ISR1 (uintptr_t arg)
{
    FSI_APP_rxISR1(arg, 2U);
}

static void FSI_APP_rx2ISR2 (uintptr_t arg)
{
    FSI_APP_rxISR2(arg, 2U);
}

static void FSI_APP_rx3ISR1 (uintptr_t arg)
{
    FSI_APP_rxISR1(arg, 3U);
}

static void FSI_APP_rx3ISR2 (uintptr_t arg)
{
    FSI_APP_rxISR2(arg, 3U);
}

static void FSI_APP_rx4ISR1 (uintptr_t arg)
{
    FSI_APP_rxISR1(arg, 4U);
}

static void FSI_APP_rx4ISR2 (uintptr_t arg)
{
    FSI_APP_rxISR2(arg, 4U);
}

static void FSI_APP_rx5ISR1 (uintptr_t arg)
{
    FSI_APP_rxISR1(arg, 5U);
}

static void FSI_APP_rx5ISR2 (uintptr_t arg)
{
    FSI_APP_rxISR2(arg, 5U);
}
#endif

static uint32_t FSI_App_txCompleted (FSI_APP_Object *pObj)
{
    uint32_t retVal = TRUE;
    uint32_t i;

    for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
    {
        if (pObj->txBaseAddr[i] != 0U)
        {
            if (pObj->intEnable)
            {
                if (pObj->txInt1Received[i] == FALSE)
                {
                    retVal = FALSE;
                    break;
                }
            }
            else
            {
                FSI_getTxEventStatus(pObj->txBaseAddr[i], &(pObj->txEventSts[i]));
                if ((pObj->txEventSts[i] & (FSI_TX_EVT_BUF_UNDERRUN | FSI_TX_EVT_BUF_OVERRUN)) != 0U)
                {
                    /* Error case */
                    pObj->errorCntr++;
                    retVal = FALSE;
                    break;
                }
                else if ((pObj->txEventSts[i] & FSI_TX_EVT_FRAME_DONE) != 0U)
                {
                    /* TX frame done */
                }
                else
                {
                    /* TX frame in progress */
                    retVal = FALSE;
                    break;
                }
            }
        }
    }   
    
    return (retVal);
}

static uint32_t FSI_App_rxCompleted (FSI_APP_Object *pObj)
{
    uint32_t        retVal = TRUE;
    uint32_t        status;
    uint32_t        i;
    uint16_t        rxUserData;
    
    for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
    {
        if (pObj->rxBaseAddr[i] != 0U)
        {
            if (pObj->intEnable)
            {
                /* Wait till frame done event is received by both Tx/Rx modules */
                if (pObj->rxInt1Received[i] == FALSE)
                {
                    retVal = FALSE;
                    break;
                }
            }
            else
            {
                FSI_getRxEventStatus(pObj->rxBaseAddr[i], &(pObj->rxEventSts[i]));
                if ((pObj->rxEventSts[i] &          \
                     (FSI_RX_EVT_PING_WD_TIMEOUT  | \
                      FSI_RX_EVT_FRAME_WD_TIMEOUT | \
                      FSI_RX_EVT_CRC_ERR          | \
                      FSI_RX_EVT_EOF_ERR          | \
                      FSI_RX_EVT_TYPE_ERR)) != 0U)
                {
                    /* Error case */
                    pObj->errorCntr++;
                    retVal = FALSE;
                    break;
                }
                else if ((pObj->rxEventSts[i] & FSI_RX_EVT_DATA_FRAME) != 0U)
                {
                    /* RX frame done */
#ifdef FSI_APP_PROFILE
                   rxTotalTime[0] += TimerP_getTimeInUsecs() - lastRxTime;
#ifdef FSI_APP_PMU_PROFILE
                   uint64_t startCycle = lastCycle;
                   lastCycle = FSI_App_getPmuCycleCnt (startCycle>>32U);
                   rxTotalCycle[0] += lastCycle - startCycle;
#endif                
#endif                    
                    /* Verify Frame attributes and data */
                    status = checkReceivedFrameTypeTag(pObj->rxBaseAddr[i], FSI_FRAME_TYPE_NWORD_DATA, txDataFrameTag);
                    if (status == FALSE)
                    {
                        pObj->errorCntr++;
                        retVal = FALSE;
                        break;
                    }
                    
                    FSI_getRxUserDefinedData(pObj->rxBaseAddr[0], &rxUserData);
                    status = compare16(rxUserData, txUserData);
                    if (status == FALSE)
                    {
                        pObj->errorCntr++;
                        retVal = FALSE;
                        break;
                    }
                    status = compareBufData(pObj, pObj->rxBaseAddr[0], bufIdx, nWords);
                    if (status == FALSE)
                    {
                        pObj->errorCntr++;
                        retVal = FALSE;
                        break;
                    }
                }
                else
                {
                    /* RX frame in progress */
                    retVal = FALSE;
                    break;
                }
            }
        }
    }   

    return (retVal);
}

static int32_t FSI_App_startXfer (FSI_APP_Object *pObj)
{
    int32_t  status = CSL_PASS;
    uint32_t i;
#ifdef FSI_APP_PROFILE
    uint64_t startTime;
#ifdef FSI_APP_PMU_PROFILE
    uint64_t startCycle;
#endif                
#endif    
    if (status == CSL_PASS)
    {
        pObj->txTimeOutCntr = FSI_APP_TXRX_TIMEOUT_COUNT;
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                pObj->txInt1Received[i] = FALSE;
                pObj->txInt2Received[i] = FALSE;
                status = FSI_setTxBufferPtr(pObj->txBaseAddr[i], bufIdx);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }
    
    if ((status == CSL_PASS) && (pObj->lbEnable == TRUE))
    {
        pObj->rxTimeOutCntr = FSI_APP_TXRX_TIMEOUT_COUNT;

        /* Get base addresses of Tx data buffer */
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                pObj->rxInt1Received[i] = FALSE;
                pObj->rxInt2Received[i] = FALSE;
                status = FSI_setRxBufferPtr(pObj->rxBaseAddr[i], bufIdx);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }
    
    if (status == CSL_PASS)
    {
        /* Write data into Tx buffer and set other Frame specific fields */
        prepareTxBufData(pObj);
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
#ifdef FSI_APP_PROFILE
                /* Start measuring the time to write data to TX buffer */
                startTime = TimerP_getTimeInUsecs();
#ifdef FSI_APP_PMU_PROFILE
                startCycle = FSI_App_getPmuCycleCnt (lastCycle>>32U);
#endif                
                status = FSI_writeTxBuffer(pObj->txBaseAddr[i], (uint16_t *)pObj->txBufData, nWords, bufIdx);
                /* Add the write data to TX buffer time to total time */
                txTotalTime[0] += TimerP_getTimeInUsecs() - startTime;
#ifdef FSI_APP_PMU_PROFILE
                lastCycle = FSI_App_getPmuCycleCnt (startCycle>>32U);
                txTotalCycle[0] += lastCycle - startCycle;
#endif                
#endif
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        /* Start Transfer */
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_startTxTransmit(pObj->txBaseAddr[i]);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }
    
    return (status);
}

static int32_t FSI_App_cpuCtrlTest (FSI_APP_Object *pObj)
{
    int32_t            status = CSL_PASS;
    uint32_t           i;
    uint32_t           txComp = FALSE;
    uint32_t           rxComp = FALSE;
    uint32_t           loopCnt;
#ifdef FSI_APP_PROFILE
    uint32_t           txTotalFrames;
    uint32_t           rxTotalFrames;
#endif
    /*
     * First setup Ping transfer and then Data
     */

    /*
     * Performing a reset on PING WD counter before its usage is recommended
     * Done on both FSI Tx/Rx sides
     */
    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_resetTxModule(pObj->txBaseAddr[i],
                                           FSI_TX_PING_TIMEOUT_CNT_RESET);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_clearTxModuleReset(pObj->txBaseAddr[i],
                                                FSI_TX_PING_TIMEOUT_CNT_RESET);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if ((status == CSL_PASS) && (fsiAppObj.lbEnable == TRUE))
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_resetRxModule(pObj->rxBaseAddr[i],
                                           FSI_RX_PING_WD_CNT_RESET);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if ((status == CSL_PASS) && (fsiAppObj.lbEnable == TRUE))
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_clearRxModuleReset(pObj->rxBaseAddr[i],
                                                FSI_RX_PING_WD_CNT_RESET);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    /*
     * Enable Rx Ping Watchdog timeout event on INT2 line
     */
    if ((status == CSL_PASS) && (fsiAppObj.lbEnable == TRUE) && (pObj->intEnable == TRUE))
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_enableRxInterrupt(pObj->rxBaseAddr[i],
                                               FSI_INT2,
                                               FSI_RX_EVT_PING_WD_TIMEOUT);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    /*
     * Now enable PING WD timer in both FSI Tx/Rx sides
     * Keeping reference counter for Rx little wide to ensure its not too sharp
     * to generate a WD timeout
     */
    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_enableTxPingTimer(pObj->txBaseAddr[i], txPingTimeRefCntr, txPingFrameTag);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if ((status == CSL_PASS) && (fsiAppObj.lbEnable == TRUE))
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_enableRxPingWatchdog(pObj->rxBaseAddr[i],
                                                  rxWdTimeoutRefCntr);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    /*
     * Automatic Ping transmission is setup, now configure for data transfers
     */

    /*
     * Setting for requested nWords and nLanes with transfers
     */
    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_setTxSoftwareFrameSize(pObj->txBaseAddr[i], nWords);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if ((status == CSL_PASS) && (fsiAppObj.lbEnable == TRUE))
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_setRxSoftwareFrameSize(pObj->rxBaseAddr[i],
                                                    nWords);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_setTxDataWidth(pObj->txBaseAddr[i], nLanes);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if ((status == CSL_PASS) && (fsiAppObj.lbEnable == TRUE))
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_setRxDataWidth(pObj->rxBaseAddr[i],
                                            nLanes);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    /*
     * Enable normal data transfer events to be sent over INT1 line
     */
    if ((status == CSL_PASS) && (pObj->intEnable == TRUE))
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_enableTxInterrupt(pObj->txBaseAddr[i], FSI_INT1, FSI_TX_EVT_FRAME_DONE);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if ((status == CSL_PASS) && (fsiAppObj.lbEnable == TRUE) && (pObj->intEnable == TRUE))
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_enableRxInterrupt(pObj->rxBaseAddr[i],
                                               FSI_INT1,
                                               FSI_RX_EVT_DATA_FRAME);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    /*
     * Enable transmit/receive error events to be sent over INT2 line
     * Overrun and Underrun conditions in Rx are not enabled as buffer pointers
     * are always overwritten to first location for sending data frames.
     */
    if ((status == CSL_PASS) && (fsiAppObj.lbEnable == TRUE) && (pObj->intEnable == TRUE))
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_enableRxInterrupt(pObj->rxBaseAddr[i],
                                               FSI_INT2,
                                               FSI_RX_EVT_CRC_ERR  |
                                               FSI_RX_EVT_EOF_ERR  |
                                               FSI_RX_EVT_TYPE_ERR);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_setTxUserDefinedData(pObj->txBaseAddr[i], txUserData);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_setTxFrameTag(pObj->txBaseAddr[i], txDataFrameTag);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_setTxFrameType(pObj->txBaseAddr[i], FSI_FRAME_TYPE_NWORD_DATA);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if ((fsiAppObj.lbEnable == TRUE) && (isRxFrameWdEnable == TRUE))
    {
        /*
         * Performing a reset on frame WD before its usage is recommended
         */
        if (status == CSL_PASS)
        {
            for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
            {
                if (pObj->rxBaseAddr[i] != 0U)
                {
                    status = FSI_resetRxModule(pObj->rxBaseAddr[i],
                                               FSI_RX_FRAME_WD_CNT_RESET);
                    if (status != CSL_PASS)
                    {
                        break;
                    }
                }
            }
        }

        if ((status == CSL_PASS) && (pObj->intEnable == TRUE))
        {
            for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
            {
                if (pObj->rxBaseAddr[i] != 0U)
                {
                    status = FSI_enableRxInterrupt(pObj->rxBaseAddr[i],
                                                   FSI_INT2,
                                                   FSI_RX_EVT_FRAME_WD_TIMEOUT);
                    if (status != CSL_PASS)
                    {
                        break;
                    }
                }
            }
        }

        if (status == CSL_PASS)
        {
            for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
            {
                if (pObj->rxBaseAddr[i] != 0U)
                {
                    status = FSI_enableRxFrameWatchdog(pObj->rxBaseAddr[i],
                                                       rxFrameWdRefCntr);
                    if (status != CSL_PASS)
                    {
                        break;
                    }
                }
            }
        }
    }

#ifdef FSI_APP_PROFILE
    /* Initialize the profiling time counters */
    lastRxTime      = 0U;
    txTotalTime[0]  = 0U;
    txTotalTime[1]  = 0U;
    rxTotalTime[0]  = 0U;
    rxTotalTime[1]  = 0U;
#ifdef FSI_APP_PMU_PROFILE
    lastCycle       = 0U;
    txTotalCycle[0] = 0U;
    txTotalCycle[1] = 0U;
    rxTotalCycle[0] = 0U;
    rxTotalCycle[1] = 0U;
#endif
#endif

    loopCnt = loopCntr;
#if defined (SOC_AM64X)
    /* bufIdx (0 - 15) should be aligned to a 64 bit buffer address */
    bufIdx &= 0xCU;
#endif

#ifdef FSI_APP_PROFILE
    lbTotalTime  = TimerP_getTimeInUsecs();
#ifdef FSI_APP_PMU_PROFILE
    lbTotalCycle = FSI_App_getPmuCycleCnt (lastCycle>>32U);
#endif
#endif
    
    /* start transmitting first data frame */
    status = FSI_App_startXfer (pObj);

    while (status == CSL_PASS)
    {
    #ifdef FSI_APP_PROFILE
        /* Start measuring TX time: TX kick off --> TX data sent */ 
        uint64_t startTime = TimerP_getTimeInUsecs();
    #ifdef FSI_APP_PMU_PROFILE
        uint64_t startCycle = FSI_App_getPmuCycleCnt (lastCycle>>32U);
    #endif                
    #endif

        /* Check if TX transfer is done */
        if (status == CSL_PASS)
        {
            txComp = FSI_App_txCompleted(pObj);
        }
        if (txComp == TRUE)
        {
#ifdef FSI_APP_PROFILE
            /* Start measuring the RX time (TX loopback start -> RX data received */
            lastRxTime = TimerP_getTimeInUsecs(); 
            /* Add the TX time to total TX time */
            txTotalTime[1] += lastRxTime - startTime;
#ifdef FSI_APP_PMU_PROFILE
            lastCycle = FSI_App_getPmuCycleCnt (startCycle>>32U);
            txTotalCycle[1] += lastCycle - startCycle;
#endif                
#endif
        }
        else
        {
            pObj->txTimeOutCntr--;
        }

        if (fsiAppObj.lbEnable == TRUE)
        {
            rxComp = FSI_App_rxCompleted(pObj);
            if (rxComp == FALSE)
            {
                pObj->rxTimeOutCntr--;
            }
        }

        if ((pObj->txTimeOutCntr == 0)                                     ||
            ((fsiAppObj.lbEnable == TRUE) && (pObj->rxTimeOutCntr == 0))   ||
            (pObj->errorCntr != 0U))
        {
            /* Timeout for one of expected Tx/Rx evetns, or errors */
            status = FSI_APP_STATUS_ERROR;
        }
        
        if ((txComp == TRUE) && 
            ((fsiAppObj.lbEnable == TRUE) && (rxComp == TRUE)) || (fsiAppObj.lbEnable == FALSE))
        {
            /* TX/RX transfer completed successfully */
            loopCnt--;
            if (loopCnt == 0U)
            {
                break;
            }
        
            /* Re-initialize the TX/RX complete flag before next transmission */
            txComp = FALSE;
            rxComp = FALSE;

            /* start transmitting next frame */
            status = FSI_App_startXfer (pObj);

        }
    }
#ifdef FSI_APP_PROFILE
    lbTotalTime  = TimerP_getTimeInUsecs() - lbTotalTime;
#ifdef FSI_APP_PMU_PROFILE
    lbTotalCycle = FSI_App_getPmuCycleCnt (lastCycle>>32U) - lbTotalCycle;
#endif
    txTotalFrames = loopCntr;
    rxTotalFrames = loopCntr;

    FSI_log("\n TXed %d frames of total %d bytes in %d (write TX buffer) + %d (TX time)"
            " usec, %ul (write TX buffer) + %ul (TX time) PMU cycles\n",
            txTotalFrames,
            txTotalFrames * nWords * 2U,
            txTotalTime[0],
            txTotalTime[1],
            txTotalCycle[0],
            txTotalCycle[1]);
    FSI_log("\n RXed %d frames of total %d bytes in %d (Read RX buffer) + %d (RX time)"
            "usec, %ul (Read RX buffer) + %ul (RX time) PMU cycles\n",
            rxTotalFrames,
            rxTotalFrames * nWords * 2U,
            rxTotalTime[1],
            rxTotalTime[0],
            rxTotalCycle[0],
            rxTotalCycle[1]);
#endif

    return status;
}

int32_t FSI_App_cpu_control_test (void *pTest)
{
    int32_t            status;
    uint32_t           i;
    uint32_t           txInstCnt;
    FSI_APP_Tests     *pFsiTest = (FSI_APP_Tests *)pTest;
    Osal_IsrRoutine    txIsr[CSL_FSITX_MAIN_CNT][FSI_APP_MAX_NUM_INTS] =
    {
        {FSI_APP_tx0ISR1, FSI_APP_tx0ISR2},
        {FSI_APP_tx1ISR1, FSI_APP_tx1ISR2}
    };
    Osal_IsrRoutine    rxIsr[CSL_FSIRX_MAIN_CNT][FSI_APP_MAX_NUM_INTS] =
    {
        {FSI_APP_rx0ISR1, FSI_APP_rx0ISR2},
        {FSI_APP_rx1ISR1, FSI_APP_rx1ISR2},
        {FSI_APP_rx2ISR1, FSI_APP_rx2ISR2},
        {FSI_APP_rx3ISR1, FSI_APP_rx3ISR2},
        {FSI_APP_rx4ISR1, FSI_APP_rx4ISR2},
        {FSI_APP_rx5ISR1, FSI_APP_rx5ISR2}
    };

    status = FSI_App_init(&fsiAppObj);
    
    /* Set interrupt mode */
    fsiAppObj.intEnable = pFsiTest->intEnable;

    /* Set loopback mode */
    fsiAppObj.lbEnable = pFsiTest->lbEnable;

    /* Set FSI TX prescaler value */
    fsiAppObj.txPreScaler = FSI_APP_FSI_TX_PRESCALER_VAL;

#ifdef FSI_APP_PROFILE
    txInstCnt = 1U;
#else
    txInstCnt = CSL_FSITX_MAIN_CNT;
#endif
    for (i = 0; i < txInstCnt; i++)
    {
        if (status == FSI_APP_STATUS_SUCCESS)
        {
            /* Set base address of FSI TX/RX instance */
            fsiAppObj.txBaseAddr[i] = CSL_FSITX0_CFG_BASE + i * 0x10000U;
            if (pFsiTest->lbEnable == TRUE)
            {
                fsiAppObj.rxBaseAddr[i*3+0] = CSL_FSIRX0_CFG_BASE + i * 0x30000U;
#ifndef FSI_APP_PROFILE
                fsiAppObj.rxBaseAddr[i*3+1] = CSL_FSIRX1_CFG_BASE + i * 0x30000U;
                fsiAppObj.rxBaseAddr[i*3+2] = CSL_FSIRX2_CFG_BASE + i * 0x30000U;
#endif
            }

#if defined (BUILD_MCU)
            if (fsiAppObj.intEnable == TRUE)
            {
                /* Set interrupt # of FSI TX/RX instance */
                fsiAppObj.txIntNum[i][0]     = CSLR_R5FSS0_CORE0_INTR_FSITX0_FSI_TX_OINT1_0 + i * 2U;
                fsiAppObj.txIntNum[i][1]     = CSLR_R5FSS0_CORE0_INTR_FSITX0_FSI_TX_OINT2_0 + i * 2U;
                fsiAppObj.rxIntNum[i*3+0][0] = CSLR_R5FSS0_CORE0_INTR_FSIRX0_FSI_RX_OINT1_0 + i * 6U;
                fsiAppObj.rxIntNum[i*3+0][1] = CSLR_R5FSS0_CORE0_INTR_FSIRX0_FSI_RX_OINT2_0 + i * 6U;
#ifndef FSI_APP_PROFILE
                fsiAppObj.rxIntNum[i*3+1][0] = CSLR_R5FSS0_CORE0_INTR_FSIRX1_FSI_RX_OINT1_0 + i * 6U;
                fsiAppObj.rxIntNum[i*3+1][1] = CSLR_R5FSS0_CORE0_INTR_FSIRX1_FSI_RX_OINT2_0 + i * 6U;
                fsiAppObj.rxIntNum[i*3+2][0] = CSLR_R5FSS0_CORE0_INTR_FSIRX2_FSI_RX_OINT1_0 + i * 6U;
                fsiAppObj.rxIntNum[i*3+2][1] = CSLR_R5FSS0_CORE0_INTR_FSIRX2_FSI_RX_OINT2_0 + i * 6U;
#endif
                /* Set ISR for FSI TX/RX instance */
                memcpy (fsiAppObj.txIsr, txIsr, sizeof(txIsr));
                memcpy (fsiAppObj.rxIsr, rxIsr, sizeof(rxIsr));
            }
#endif
        }
    }

    if (status == FSI_APP_STATUS_SUCCESS)
    {
        status = FSI_App_open(&fsiAppObj);
    }

    if (status == FSI_APP_STATUS_SUCCESS)
    {
        status = FSI_App_cpuCtrlTest(&fsiAppObj);
    }

    if (status == FSI_APP_STATUS_SUCCESS)
    {
        status = FSI_App_close(&fsiAppObj);
    }

    return (status);
}
