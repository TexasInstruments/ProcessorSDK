/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for CANFD
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <string.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/CycleprofilerP.h>

#include <ti/osal/DebugP.h>
#include <ti/board/board.h>
#include <ti/drv/esm/esm.h>
#include <ti/drv/canfd/canfd.h>
#if defined (SIM_BUILD)
#include <stdio.h>
#define UART_printf     printf
#else
#include <ti/drv/uart/UART_stdio.h>
#endif

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/** \brief Test type defines */
#define MCAN_APP_TEST_INTERNAL_LOOPBACK     1U
#define MCAN_APP_TEST_EXTERNAL_LOOPBACK     2U
#define MCAN_APP_TEST_MULTIPLE_TRANSMISSION 3U
#define MCAN_APP_TEST_EXTERNAL_DATA         4U
#define MCAN_APP_TEST_EVM_EVM               5U
#define MCAN_APP_TEST_TX_CANCEL             6U
#define MCAN_APP_TEST_POWERDOWN             7U
#define MCAN_APP_TEST_MSGID_RANGE           8U

#define MCAN_APP_TEST_EVM_MODE_TX           1U
#define MCAN_APP_TEST_EVM_MODE_RX           2U
#define MCAN_APP_TEST_EVM_MODE_FIFO         3U

/** \brief Number of messages sent */
#define MCAN_APP_TEST_MESSAGE_COUNT         100U

/** \brief Number of messages sent */
#define MCAN_APP_TEST_DATA_SIZE             64U

#define MSS_SYS_VCLK                        (200U * 1000U * 1000U)
/** \brief VBUSP Frequency in MHz */
#define VBUSP_FREQ                          (MSS_SYS_VCLK /1000000U)

#define APP_TSK_STACK_MAIN              (16U * 1024U)

/* Global Variables */
CANFD_Reason            gErrorReason;
volatile uint32_t       testSelection = 0;
volatile uint32_t       gTxDoneFlag = 0, gRxDoneFlag = 0;
volatile uint32_t       iterationCount = 0U;
volatile uint32_t       gTxPkts = 0, gRxPkts = 0, gTxCancelPkts = 0, gErrStatusInt = 0;
uint8_t                 rxData[MCAN_APP_TEST_DATA_SIZE];
uint32_t                txDataLength, rxDataLength, testDataLength = 0U;
uint32_t                gErrMsgLstCnt = 0U;
uint32_t                gErrDataMissMatchCnt = 0U;
uint32_t                gErrGetData = 0U;
uint32_t                gErrFrameType = 0U;
uint32_t                rxTicks[MCAN_APP_TEST_MESSAGE_COUNT];
uint32_t                txTicks[MCAN_APP_TEST_MESSAGE_COUNT];
uint32_t                minRxTicks;
uint32_t                maxRxTicks;
uint32_t                minTxTicks;
uint32_t                maxTxTicks;
uint32_t                totalTxTicks;
uint32_t                totalRxTicks;
uint8_t                 txData[128U] =
{0xA1, 0x1A, 0xFF, 0xFF, 0xC1, 0x1C, 0xB1, 0x1B,
0xA2, 0x2A, 0xFF, 0xFF, 0xC2, 0x2C, 0xB2, 0x2B,
0xA3, 0x3A, 0xFF, 0xFF, 0xC3, 0x3C, 0xB3, 0x3B,
0xA4, 0x4A, 0xFF, 0xFF, 0xC4, 0x4C, 0xB4, 0x4B,
0xA5, 0x5A, 0xFF, 0xFF, 0xC5, 0x5C, 0xB5, 0x5B,
0xA6, 0x6A, 0xFF, 0xFF, 0xC6, 0x6C, 0xB6, 0x6B,
0xA7, 0x7A, 0xFF, 0xFF, 0xC7, 0x7C, 0xB7, 0x7B,
0xA8, 0x8A, 0xFF, 0xFF, 0xC8, 0x8C, 0xB8, 0x8B,
0xA1, 0x1A, 0xFF, 0xFF, 0xC1, 0x1C, 0xB1, 0x1B,
0xA2, 0x2A, 0xFF, 0xFF, 0xC2, 0x2C, 0xB2, 0x2B,
0xA3, 0x3A, 0xFF, 0xFF, 0xC3, 0x3C, 0xB3, 0x3B,
0xA4, 0x4A, 0xFF, 0xFF, 0xC4, 0x4C, 0xB4, 0x4B,
0xA5, 0x5A, 0xFF, 0xFF, 0xC5, 0x5C, 0xB5, 0x5B,
0xA6, 0x6A, 0xFF, 0xFF, 0xC6, 0x6C, 0xB6, 0x6B,
0xA7, 0x7A, 0xFF, 0xFF, 0xC7, 0x7C, 0xB7, 0x7B,
0xA8, 0x8A, 0xFF, 0xFF, 0xC8, 0x8C, 0xB8, 0x8B
};

uint32_t                gDisplayStats = 0;
uint32_t                gTerminate = 0;
uint32_t                gMCANMode = 2U;
CANFD_MCANFrameType     testFrameType = CANFD_MCANFrameType_FD;

static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));

/**
 *  @b Description
 *  @n
 *      This function starts the PMU counter.
 *
 *   @param[in] counter
 *      Counter id used for benchmarking
 *
 *  @retval
 *      Not Applicable.
 */
uint32_t Test_benchmarkStart(void)
{
    return CycleprofilerP_getTimeStamp();
}

/**
 *  @b Description
 *  @n
 *      This function stops a PMU counter and returns the current
 *      counter value.
 *
 *   @param[in] counter
 *      Counter id used for benchmarking
 *
 *  @retval
 *      Current PMU counter value.
 */
uint32_t Test_benchmarkStop(uint32_t startCount)
{
    uint32_t endCount = CycleprofilerP_getTimeStamp();

    return (endCount - startCount);
}

/**
 *  @b Description
 *  @n
 *      Application implemented callback function to handle error and status interrupts.
 *
 *   @param[in] handle
 *      Handle to the CANFD Driver
 *  @param[in]  reason
 *      Cause of the interrupt which prompted the callback.
 *  @param[in]  errStatusResp
 *      Response structure populated with the value of the fields that caused the error or status interrupt.
 *      Processing of this structure is dependent on the callback reason.
 *
 *  @retval
 *      Not Applicable.
 */
static void MCANAppErrStatusCallback(CANFD_Handle handle, CANFD_Reason reason, CANFD_ErrStatusResp* errStatusResp)
{
    gErrStatusInt++;
    gErrorReason = reason;
    return;
}

/**
 *  @b Description
 *  @n
 *      Application implemented callback function to handle Tx complete and receive interrupts.
 *
 *   @param[in] handle
 *      Handle to the message object
 *   @param[in] reason
 *      Cause of the interrupt which prompted the callback.
 *
 *  @retval
 *      Not Applicable.
 */
static void MCANAppCallback(CANFD_MsgObjHandle handle, CANFD_Reason reason)
{
    int32_t                 errCode, retVal;
    uint32_t                id;
    CANFD_MCANFrameType     rxFrameType;
    CANFD_MCANXidType       rxIdType;

    if (reason == CANFD_Reason_TX_COMPLETION)
    {
        {
            gTxPkts++;
            gTxDoneFlag = 1;
            return;
        }
    }
    if (reason == CANFD_Reason_RX)
    {
        {
            uint32_t stratCnt;
            /* Reset the receive buffer */
            memset(&rxData, 0, sizeof (rxData));
            testDataLength = 0;

            if ((testSelection != MCAN_APP_TEST_EXTERNAL_DATA) && (gRxPkts < MCAN_APP_TEST_MESSAGE_COUNT))
            {
                /* Reset the counter: */
                stratCnt = Test_benchmarkStart();
            }
            retVal = CANFD_getData (handle, &id, &rxFrameType, &rxIdType, &rxDataLength, &rxData[0], &errCode);
            if (retVal < 0)
            {
                gErrGetData++;
                return;
            }

            if (rxFrameType != testFrameType)
            {
                gErrFrameType++;
                return;
            }

            if ((testSelection != MCAN_APP_TEST_EXTERNAL_DATA) && (gRxPkts < MCAN_APP_TEST_MESSAGE_COUNT))
            {
                /* Stop the counter: */
                rxTicks[gRxPkts] = Test_benchmarkStop(stratCnt);

                /* Update the receive statistics: */
                minRxTicks   = (minRxTicks < rxTicks[gRxPkts]) ? minRxTicks : rxTicks[gRxPkts];
                maxRxTicks   = (maxRxTicks > rxTicks[gRxPkts]) ? maxRxTicks : rxTicks[gRxPkts];
                totalRxTicks = totalRxTicks + rxTicks[gRxPkts];
            }

            /* Validate the data */
            if ((testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK) || (testSelection == MCAN_APP_TEST_EXTERNAL_LOOPBACK) ||
                            (testSelection == MCAN_APP_TEST_EVM_EVM))
            {
                /* Check if sent data has been received */
                if (rxDataLength == txDataLength)
                {
                    while (testDataLength < rxDataLength)
                    {
                        if (rxData[testDataLength] != txData[testDataLength])
                        {
                            gErrDataMissMatchCnt++;
                        }
                        testDataLength++;
                    }
                }

            }
            gRxPkts++;
            gRxDoneFlag = 1U;
            if (id == 0xDA)
            {
                /* Terminate message received */
                gTerminate = 1U;
            }
            return;
        }
    }
    if (reason == CANFD_Reason_TX_CANCELED)
    {
        {
            gTxCancelPkts++;
            gTxDoneFlag = 1U;
            return;
        }
    }
}

/**
 *  @b Description
 *  @n
 *      Platform specific intializations.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */

static int32_t PlatformInit(void)
{
    Board_STATUS boardStatus;
    Board_initCfg boardCfg;

    boardCfg = (BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK | 
                BOARD_INIT_UART_STDIO);
    boardStatus = Board_init(boardCfg);

    DebugP_assert(boardStatus == BOARD_SOK);
    return 0;
}

static void MCANAppInitParams(CANFD_MCANInitParams* mcanCfgParams)
{
    /*Intialize MCAN Config Params*/
    memset (mcanCfgParams, sizeof (CANFD_MCANInitParams), 0);

    mcanCfgParams->fdMode          = 0x1U;
    mcanCfgParams->brsEnable       = 0x1U;
    mcanCfgParams->txpEnable       = 0x0U;
    mcanCfgParams->efbi            = 0x0U;
    mcanCfgParams->pxhddisable     = 0x0U;
    mcanCfgParams->darEnable       = 0x1U;
    mcanCfgParams->wkupReqEnable   = 0x1U;
    mcanCfgParams->autoWkupEnable  = 0x1U;
    mcanCfgParams->emulationEnable = 0x0U;
    mcanCfgParams->emulationFAck   = 0x0U;
    mcanCfgParams->clkStopFAck     = 0x0U;
    mcanCfgParams->wdcPreload      = 0x0U;
    mcanCfgParams->tdcEnable       = 0x1U;
    mcanCfgParams->tdcConfig.tdcf  = 0U;
    mcanCfgParams->tdcConfig.tdco  = 8U;
    mcanCfgParams->monEnable       = 0x0U;
    mcanCfgParams->asmEnable       = 0x0U;
    mcanCfgParams->tsPrescalar     = 0x0U;
    mcanCfgParams->tsSelect        = 0x0U;
    mcanCfgParams->timeoutSelect   = CANFD_MCANTimeOutSelect_CONT;
    mcanCfgParams->timeoutPreload  = 0x0U;
    mcanCfgParams->timeoutCntEnable= 0x0U;
    mcanCfgParams->filterConfig.rrfe        = 0x1U;
    mcanCfgParams->filterConfig.rrfs        = 0x1U;
    mcanCfgParams->filterConfig.anfe        = 0x3U;
    mcanCfgParams->filterConfig.anfs        = 0x3U;
    mcanCfgParams->msgRAMConfig.lss         = 127U;
    mcanCfgParams->msgRAMConfig.lse         = 64U;
    mcanCfgParams->msgRAMConfig.txBufNum    = 32U;
    mcanCfgParams->msgRAMConfig.txFIFOSize  = 0U;
    mcanCfgParams->msgRAMConfig.txBufMode   = 0U;
    mcanCfgParams->msgRAMConfig.txEventFIFOSize         = 0U;
    mcanCfgParams->msgRAMConfig.txEventFIFOWaterMark    = 0U;
    mcanCfgParams->msgRAMConfig.rxFIFO0size             = 64U;
    mcanCfgParams->msgRAMConfig.rxFIFO0OpMode           = 0U;
    mcanCfgParams->msgRAMConfig.rxFIFO0waterMark        = 0U;
    mcanCfgParams->msgRAMConfig.rxFIFO1size             = 64U;
    mcanCfgParams->msgRAMConfig.rxFIFO1waterMark        = 0U;
    mcanCfgParams->msgRAMConfig.rxFIFO1OpMode           = 0U;

    mcanCfgParams->eccConfig.enable         = 1U;
    mcanCfgParams->eccConfig.enableChk      = 1U;
    mcanCfgParams->eccConfig.enableRdModWr  = 1U;

    mcanCfgParams->errInterruptEnable   = 1U;
    mcanCfgParams->dataInterruptEnable  = 1U;
    mcanCfgParams->appErrCallBack       = MCANAppErrStatusCallback;
    mcanCfgParams->appDataCallBack      = MCANAppCallback;
}

static int32_t mcanLoopbackTest()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle;
    CANFD_MsgObjHandle          rxMsgObjHandle;
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_OptionTLV             optionTLV;
    uint8_t                     value;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams;
    CANFD_MCANMsgObjCfgParams   rxMsgObjectParams;
    CANFD_MCANLoopbackCfgParams mcanloopbackParams;
    CANFD_MCANMsgObjectStats    msgObjStats;

    gTxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(0U, &mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        UART_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (80)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (80)/(((2+2+3)+1)*BRP) = 5Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x2U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    mcanloopbackParams.enable = 1U;
    if (testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK)
        mcanloopbackParams.mode = CANFD_MCANLoopBackMode_INTERNAL;
    else
        mcanloopbackParams.mode = CANFD_MCANLoopBackMode_EXTERNAL;

    optionTLV.type = CANFD_Option_MCAN_LOOPBACK;
    optionTLV.length = sizeof(CANFD_MCANLoopbackCfgParams);
    optionTLV.value = (void*) &mcanloopbackParams;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD set option Loopback failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjectParams.direction = CANFD_Direction_TX;
    txMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    txMsgObjectParams.msgIdentifier = 0x29E;

    txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        UART_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the receive message object */
    rxMsgObjectParams.direction = CANFD_Direction_RX;
    rxMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    rxMsgObjectParams.msgIdentifier = 0x29E;

    rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);
    if (rxMsgObjHandle == NULL)
    {
        UART_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the measurement counters */
    minRxTicks   = 0xFFFFFFFFU;
    maxRxTicks   = 0U;
    minTxTicks   = 0xFFFFFFFFU;
    maxTxTicks   = 0U;
    totalTxTicks = 0U;
    totalRxTicks = 0U;

    while (iterationCount != MCAN_APP_TEST_MESSAGE_COUNT)
    {
        uint32_t stratCnt;
        /* Reset the counter: */
        stratCnt = Test_benchmarkStart();

        {
            /* Send data over Tx message object */
            if (testFrameType == CANFD_MCANFrameType_FD)
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
            else
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, 8U, &txData[0], &errCode);

            if (retVal < 0)
            {
                UART_printf ("Error: CANFD transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
                return -1;
            }
        }
        /* Stop the counter: */
        txTicks[iterationCount] = Test_benchmarkStop(stratCnt);

        /* Update the transmit statistics: */
        minTxTicks   = (minTxTicks < txTicks[iterationCount]) ? minTxTicks : txTicks[iterationCount];
        maxTxTicks   = (maxTxTicks > txTicks[iterationCount]) ? maxTxTicks : txTicks[iterationCount];
        totalTxTicks = totalTxTicks + txTicks[iterationCount];

        while (gTxDoneFlag == 0);
        gTxDoneFlag = 0;

        while (gRxDoneFlag == 0);
        gRxDoneFlag = 0;

        iterationCount++;
    }

    UART_printf("Debug: Number of iterations              : %d\n", iterationCount);
    UART_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
    UART_printf("Debug: Number of messages received       : %d\n", gRxPkts);
    UART_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
    UART_printf("Debug: Number of data mismatch           : %d\n", gErrDataMissMatchCnt);
    UART_printf("Debug: Number of Frame mismatch          : %d\n", gErrFrameType);
    UART_printf("Debug: Number of Get_Data errors         : %d\n", gErrGetData);
    UART_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
    UART_printf("\n\n");

    msgObjStats.handle = txMsgObjHandle;
    optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
    optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
    optionTLV.value = (void*) &msgObjStats;

    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
    UART_printf("\n\n");

    msgObjStats.handle = rxMsgObjHandle;
    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);

    UART_printf("Debug: Receive & Transmit Measurements\n");
    UART_printf("Debug: Rx Min:%d Max: %d Average:%d ticks\n", minRxTicks, maxRxTicks, totalRxTicks/gRxPkts);
    UART_printf("Debug: Tx Min:%d Max: %d Average:%d ticks\n", minTxTicks, maxTxTicks, totalTxTicks/gTxPkts);

    UART_printf("\n\n");

    /* Reset the driver */
    optionTLV.type = CANFD_Option_MCAN_MODE;
    optionTLV.length = sizeof(uint8_t);
    value = 1U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD set option Mode - SW INIT failed [Error code %d]\n", errCode);
        return -1;
    }

    optionTLV.type = CANFD_Option_MCAN_MODE;
    optionTLV.length = sizeof(uint8_t);
    value = 0U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD set option Mode - NORMAL failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deleteMsgObject(txMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD delete Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deleteMsgObject(rxMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD delete Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deinit(canHandle, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD deinit failed [Error code %d]\n", errCode);
        return -1;
    }

    if ((gErrMsgLstCnt == 0) && (gErrDataMissMatchCnt == 0) && (gErrFrameType == 0) && (gErrGetData == 0))
    {
        if (testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK)
        {
            UART_printf("Debug: Internal loopback testing for %d iterations Passed\n", iterationCount);
        }
        else
        {
            UART_printf("Debug: External loopback testing for %d iterations Passed\n", iterationCount);
        }
    }
    else
    {
        if (testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK)
        {
            UART_printf("Debug: Internal loopback testing for %d iterations Failed\n", iterationCount);
            retVal = -1;
        }
        else
        {
            UART_printf("Debug: External loopback testing for %d iterations Failed\n", iterationCount);
            retVal = -1;
        }
    }
    return retVal;
}


static int32_t mcanTestTxCancel()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle;
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_OptionTLV             optionTLV;
    uint8_t                     value;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams;
    CANFD_MCANLoopbackCfgParams mcanloopbackParams;
    CANFD_MCANMsgObjectStats    msgObjStats;

    gTxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(0U, &mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        UART_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (80)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (80)/(((2+2+3)+1)*BRP) = 5Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x2U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    mcanloopbackParams.enable = 1U;
    mcanloopbackParams.mode = CANFD_MCANLoopBackMode_EXTERNAL;

    optionTLV.type = CANFD_Option_MCAN_LOOPBACK;
    optionTLV.length = sizeof(CANFD_MCANLoopbackCfgParams);
    optionTLV.value = (void*) &mcanloopbackParams;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD set option Loopback failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjectParams.direction = CANFD_Direction_TX;
    txMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    txMsgObjectParams.msgIdentifier = 0x29E;

    txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        UART_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    while (iterationCount != MCAN_APP_TEST_MESSAGE_COUNT)
    {
        /* Send data over Tx message object */
        if (testFrameType == CANFD_MCANFrameType_FD)
            retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
        else
            retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, 8U, &txData[0], &errCode);

        if (retVal < 0)
        {
            UART_printf ("Error: CANFD transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
            return -1;
        }

        /* Cancel data over Tx message object */
        retVal = CANFD_transmitDataCancel (txMsgObjHandle, &errCode);
        if (retVal < 0)
        {
            UART_printf ("Error: CANFD transmit data cancel for iteration %d failed [Error code %d]\n", iterationCount, errCode);
            return -1;
        }

        while (gTxDoneFlag == 0);
        gTxDoneFlag = 0;

        iterationCount++;
    }

    UART_printf("Debug: Number of iterations              : %d\n", iterationCount);
    UART_printf("Debug: Number of messages Cancelled      : %d\n", gTxCancelPkts);
    UART_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
    UART_printf("Debug: Number of data mismatch           : %d\n", gErrDataMissMatchCnt);
    UART_printf("Debug: Number of Frame mismatch          : %d\n", gErrFrameType);
    UART_printf("Debug: Number of Get_Data errors         : %d\n", gErrGetData);
    UART_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
    UART_printf("\n\n");

    msgObjStats.handle = txMsgObjHandle;
    optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
    optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
    optionTLV.value = (void*) &msgObjStats;

    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
    UART_printf("\n\n");

    /* Reset the driver */
    optionTLV.type = CANFD_Option_MCAN_MODE;
    optionTLV.length = sizeof(uint8_t);
    value = 1U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD set option Mode - SW INIT failed [Error code %d]\n", errCode);
        return -1;
    }

    optionTLV.type = CANFD_Option_MCAN_MODE;
    optionTLV.length = sizeof(uint8_t);
    value = 0U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD set option Mode - NORMAL failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deleteMsgObject(txMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD delete Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deinit(canHandle, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD deinit failed [Error code %d]\n", errCode);
        return -1;
    }

    UART_printf("Debug: Tx transmit cancel testing for %d iterations Passed\n", iterationCount);
    return retVal;
}

static int32_t mcanPowerDownTest()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle;
    CANFD_MsgObjHandle          rxMsgObjHandle;
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_OptionTLV             optionTLV;
    uint8_t                     value;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams;
    CANFD_MCANMsgObjCfgParams   rxMsgObjectParams;
    CANFD_MCANLoopbackCfgParams mcanloopbackParams;
    CANFD_MCANMsgObjectStats    msgObjStats;

    gTxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(0U, &mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        UART_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (80)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (80)/(((2+2+3)+1)*BRP) = 5Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x2U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    mcanloopbackParams.enable = 1;
    mcanloopbackParams.mode = CANFD_MCANLoopBackMode_EXTERNAL;

    optionTLV.type = CANFD_Option_MCAN_LOOPBACK;
    optionTLV.length = sizeof(CANFD_MCANLoopbackCfgParams);
    optionTLV.value = (void*) &mcanloopbackParams;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD set option Loopback failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjectParams.direction = CANFD_Direction_TX;
    txMsgObjectParams.msgIdType = CANFD_MCANXidType_11_BIT;
    txMsgObjectParams.msgIdentifier = 0x29E;

    txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        UART_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the receive message object */
    rxMsgObjectParams.direction = CANFD_Direction_RX;
    rxMsgObjectParams.msgIdType = CANFD_MCANXidType_11_BIT;
    rxMsgObjectParams.msgIdentifier = 0x29E;

    rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);
    if (rxMsgObjHandle == NULL)
    {
        UART_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the measurement counters */
    minRxTicks   = 0xFFFFFFFFU;
    maxRxTicks   = 0U;
    minTxTicks   = 0xFFFFFFFFU;
    maxTxTicks   = 0U;
    totalTxTicks = 0U;
    totalRxTicks = 0U;

    while (iterationCount != MCAN_APP_TEST_MESSAGE_COUNT)
    {
        uint32_t stratCnt;
        /* Reset the counter: */
        stratCnt = Test_benchmarkStart();

        {
            /* Send data over Tx message object */
            if (testFrameType == CANFD_MCANFrameType_FD)
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
            else
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);

            if (retVal < 0)
            {
                UART_printf ("Error: CANFD transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
                return -1;
            }
        }
        /* Stop the counter: */
        txTicks[iterationCount] = Test_benchmarkStop(stratCnt);

        /* Update the transmit statistics: */
        minTxTicks   = (minTxTicks < txTicks[iterationCount]) ? minTxTicks : txTicks[iterationCount];
        maxTxTicks   = (maxTxTicks > txTicks[iterationCount]) ? maxTxTicks : txTicks[iterationCount];
        totalTxTicks = totalTxTicks + txTicks[iterationCount];

        while (gTxDoneFlag == 0);
        gTxDoneFlag = 0;

        while (gRxDoneFlag == 0);
        gRxDoneFlag = 0;

        iterationCount++;
    }

    UART_printf("Debug: Number of iterations              : %d\n", iterationCount);
    UART_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
    UART_printf("Debug: Number of messages received       : %d\n", gRxPkts);
    UART_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
    UART_printf("Debug: Number of data mismatch           : %d\n", gErrDataMissMatchCnt);
    UART_printf("Debug: Number of Frame mismatch          : %d\n", gErrFrameType);
    UART_printf("Debug: Number of Get_Data errors         : %d\n", gErrGetData);
    UART_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
    UART_printf("\n\n");

    msgObjStats.handle = txMsgObjHandle;
    optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
    optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
    optionTLV.value = (void*) &msgObjStats;

    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
    UART_printf("\n\n");

    msgObjStats.handle = rxMsgObjHandle;
    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);

    UART_printf("\n\n");

    /* Power Down the module */
    optionTLV.type = CANFD_Option_MCAN_POWER_DOWN;
    optionTLV.length = sizeof(uint8_t);
    value = 1U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD set option Power Down failed [Error code %d]\n", errCode);
        return -1;
    }
    else
        UART_printf ("Debug: CANFD Module Power Down sucessful\n");

    optionTLV.type = CANFD_Option_MCAN_POWER_DOWN;
    optionTLV.length = sizeof(uint8_t);
    value = 0;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD set option Wake Up failed [Error code %d]\n", errCode);
        return -1;
    }
    else
        UART_printf ("Debug: CANFD Module is Wake Up sucessful\n");

    UART_printf("\n\n");

    iterationCount = 0;
    while (iterationCount != MCAN_APP_TEST_MESSAGE_COUNT)
    {
        uint32_t stratCnt;
        /* Reset the counter: */
        stratCnt = Test_benchmarkStart();

        {
            /* Send data over Tx message object */
            if (testFrameType == CANFD_MCANFrameType_FD)
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
            else
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);

            if (retVal < 0)
            {
                UART_printf ("Error: CANFD transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
                return -1;
            }
        }
        /* Stop the counter: */
        txTicks[iterationCount] = Test_benchmarkStop(stratCnt);

        /* Update the transmit statistics: */
        minTxTicks   = (minTxTicks < txTicks[iterationCount]) ? minTxTicks : txTicks[iterationCount];
        maxTxTicks   = (maxTxTicks > txTicks[iterationCount]) ? maxTxTicks : txTicks[iterationCount];
        totalTxTicks = totalTxTicks + txTicks[iterationCount];

        while (gTxDoneFlag == 0);
        gTxDoneFlag = 0;

        while (gRxDoneFlag == 0);
        gRxDoneFlag = 0;

        iterationCount++;
    }

    UART_printf("\n\n");
    UART_printf("Debug: Number of iterations              : %d\n", iterationCount);
    UART_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
    UART_printf("Debug: Number of messages received       : %d\n", gRxPkts);
    UART_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
    UART_printf("Debug: Number of data mismatch           : %d\n", gErrDataMissMatchCnt);
    UART_printf("Debug: Number of Frame mismatch          : %d\n", gErrFrameType);
    UART_printf("Debug: Number of Get_Data errors         : %d\n", gErrGetData);
    UART_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
    UART_printf("\n\n");

    msgObjStats.handle = txMsgObjHandle;
    optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
    optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
    optionTLV.value = (void*) &msgObjStats;

    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
    UART_printf("\n\n");

    msgObjStats.handle = rxMsgObjHandle;
    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);

    UART_printf("\n\n");

    retVal = CANFD_deleteMsgObject(txMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD delete Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deleteMsgObject(rxMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD delete Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deinit(canHandle, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD deinit failed [Error code %d]\n", errCode);
        return -1;
    }

    if ((gErrMsgLstCnt != 0) || (gErrDataMissMatchCnt != 0) && (gErrFrameType == 0) && (gErrGetData == 0))
    {
        UART_printf("Debug: Power down testing with data failed\n");
        retVal = -1;
    }
    return retVal;
}

static int32_t mcanTransmitTest()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle;
    CANFD_MsgObjHandle          rxMsgObjHandle;
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_OptionTLV             optionTLV;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams;
    CANFD_MCANMsgObjCfgParams   rxMsgObjectParams;
    CANFD_MCANMsgObjectStats    msgObjStats;
    CANFD_MCANErrCntStatus      errCounter;
    CANFD_MCANProtocolStatus    protoStatus;
    uint32_t                    length = MCAN_APP_TEST_DATA_SIZE;
    uint32_t                    index = 0U;
    uint32_t                    msgId ;
    uint32_t                    idIndex = 0;
    uint32_t                    numIterations;

    gTxDoneFlag = 0;
    gRxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(0U, &mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        UART_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (80)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (80)/(((2+2+3)+1)*BRP) = 5Mhz
    */
    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x2U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjectParams.direction = CANFD_Direction_TX;
    txMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    txMsgObjectParams.msgIdentifier = 0xC1;

    txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        UART_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the receive message object */
    rxMsgObjectParams.direction = CANFD_Direction_RX;
    rxMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    rxMsgObjectParams.msgIdentifier = 0xC1;

    rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);
    if (rxMsgObjHandle == NULL)
    {
        UART_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    numIterations = 0;
    while (numIterations < 5)
    {
        /* Send data over Tx message object */
        if (testFrameType == CANFD_MCANFrameType_FD)
        {

            length = 128U;
            if(idIndex > 10)
            {
                idIndex = 0;
            }
            msgId = txMsgObjectParams.msgIdentifier + idIndex;
            TaskP_sleep(100U);
            while(length > MCAN_APP_TEST_DATA_SIZE)
            {

                retVal = CANFD_transmitData (txMsgObjHandle, msgId, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
                length = length - MCAN_APP_TEST_DATA_SIZE;
                TaskP_sleep(100);
            }
            idIndex++;
        }
        else
        {

            length = 128U;
            index = 0;
            msgId = txMsgObjectParams.msgIdentifier;
            TaskP_sleep(100);
            while(length > 8U)
            {

                retVal = CANFD_transmitData (txMsgObjHandle, msgId, CANFD_MCANFrameType_CLASSIC, 8U, &txData[index], &errCode);
                length = length - 8U;
                index = index + 8U;
                msgId = msgId + 1U;
                TaskP_sleep(100U);
            }
            retVal = CANFD_transmitData (txMsgObjHandle, msgId, CANFD_MCANFrameType_CLASSIC, length, &txData[index], &errCode);

        }
        TaskP_sleep(200U);
        if (retVal < 0)
        {
            gErrMsgLstCnt++;
            continue;
        }

        iterationCount++;

        if (gDisplayStats == 1U)
        {
            gDisplayStats = 0;

            UART_printf("Debug: Number of iterations              : %d\n", iterationCount);
            UART_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
            UART_printf("Debug: Number of messages received       : %d\n", gRxPkts);
            UART_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
            UART_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
            UART_printf("Debug: Error Reason                      : %d\n", gErrorReason);
            UART_printf("\n\n");

            msgObjStats.handle = txMsgObjHandle;
            optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
            optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
            optionTLV.value = (void*) &msgObjStats;

            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
            }

            UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
            UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
            UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
            UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
            UART_printf("\n\n");

            msgObjStats.handle = rxMsgObjHandle;
            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
            }

            UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
            UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
            UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
            UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
            UART_printf("\n\n");

            optionTLV.type = CANFD_Option_MCAN_ERROR_COUNTER;
            optionTLV.length = sizeof(CANFD_MCANErrCntStatus);
            optionTLV.value = (void*) &errCounter;

            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                UART_printf ("Error: CANFD get error counter failed [Error code %d]\n", errCode);
            }

            UART_printf("Debug: Receive passive status            : %d\n", errCounter.rpStatus);
            UART_printf("Debug: Transmit Error Counter            : %d\n", errCounter.transErrLogCnt);
            UART_printf("Debug: Receive Error Counter             : %d\n", errCounter.recErrCnt);
            UART_printf("Debug: Error Logging Counter             : %d\n", errCounter.canErrLogCnt);
            UART_printf("\n\n");

            optionTLV.type = CANFD_Option_MCAN_PROTOCOL_STATUS;
            optionTLV.length = sizeof(CANFD_MCANProtocolStatus);
            optionTLV.value = (void*) &protoStatus;

            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                UART_printf ("Error: CANFD get protocol status failed [Error code %d]\n", errCode);
            }

            UART_printf("Debug: LEC                               : %d\n", protoStatus.lastErrCode);
            UART_printf("Debug: Activity                          : %d\n", protoStatus.act);
            UART_printf("Debug: Error Passive                     : %d\n", protoStatus.errPassive);
            UART_printf("Debug: Warning Status                    : %d\n", protoStatus.warningStatus);
            UART_printf("Debug: Bus Off Status                    : %d\n", protoStatus.busOffStatus);
            UART_printf("Debug: Data Phase LEC                    : %d\n", protoStatus.dlec);
            UART_printf("Debug: Rx ESI flag                       : %d\n", protoStatus.resi);
            UART_printf("Debug: Rx BRS flag                       : %d\n", protoStatus.rbrs);
            UART_printf("Debug: Rx CAN FD                         : %d\n", protoStatus.rfdf);
            UART_printf("Debug: Protocol Exception Event          : %d\n", protoStatus.pxe);
            UART_printf("Debug: TDC value                         : %d\n", protoStatus.tdcv);
            UART_printf("\n\n");
        }
        numIterations++;
    }
    return 0;
}

static int32_t mcanEVM_EVMTest()
{
    CANFD_Handle                        canHandle;
    CANFD_MsgObjHandle                  txMsgObjHandle;
    CANFD_MsgObjHandle                  rxMsgObjHandle;
    CANFD_MsgObjHandle                  rxRangeMsgObjHandle;
    int32_t                             retVal = 0;
    int32_t                             errCode = 0;
    int32_t                             i;
    volatile float                      throughput;
    CANFD_OptionTLV                     optionTLV;
    CANFD_MCANInitParams                mcanCfgParams;
    CANFD_MCANBitTimingParams           mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams           txMsgObjectParams;
    CANFD_MCANMsgObjCfgParams           rxMsgObjectParams;
    CANFD_MCANRxMsgObjRangeCfgParams    rxRangeMsgObjectParams;
    CANFD_MCANMsgObjectStats            msgObjStats;
    CANFD_MCANErrCntStatus              errCounter;
    CANFD_MCANProtocolStatus            protoStatus;

    gTxDoneFlag = 0;
    gRxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(0U, &mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        UART_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (40)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (40)/(((2+2+3)+1)*BRP) = 5Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x2U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the measurement counters */
    minRxTicks   = 0xFFFFFFFFU;
    maxRxTicks   = 0U;
    minTxTicks   = 0xFFFFFFFFU;
    maxTxTicks   = 0U;
    totalTxTicks = 0U;
    totalRxTicks = 0U;

    /* Set the EVM in transmit mode*/
    if (gMCANMode == MCAN_APP_TEST_EVM_MODE_TX)
    {
        /* Setup the transmit message object */
        txMsgObjectParams.direction = CANFD_Direction_TX;
        txMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
        txMsgObjectParams.msgIdentifier = 0xC1;

        txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
        if (txMsgObjHandle == NULL)
        {
            UART_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
            return -1;
        }
        else
        {
            UART_printf ("Debug: EVM in transmit mode...\n");
            UART_printf("\n\n");
        }

        for (i = 0; i < MCAN_APP_TEST_MESSAGE_COUNT; i++)
        {
            /* Send data over Tx message object */
            if (testFrameType == CANFD_MCANFrameType_FD)
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
            else
            {
                uint32_t len = MCAN_APP_TEST_DATA_SIZE;
                uint32_t index = 0U;
                while(len > 8U)
                {
                    retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, 8U, &txData[index], &errCode);
                    len = len - 8U;
                    index = index + 8U;
                }
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, len, &txData[index], &errCode);

            }
            if (retVal < 0)
            {
                return -1;
            }
            /* Wait for the transmit complete interrupt */
            while (gTxDoneFlag == 0);
            gTxDoneFlag = 0;
        }

        UART_printf("Debug: Number of iterations              : %d\n", iterationCount);
        UART_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
        UART_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
        UART_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
        UART_printf("\n\n");

        msgObjStats.handle = txMsgObjHandle;
        optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
        optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
        optionTLV.value = (void*) &msgObjStats;

        retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
        if (retVal < 0)
        {
            UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        }

        UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
        UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
        UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
        UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
        UART_printf("\n\n");
    }

    /* Set the EVM in recieve mode*/
    if ((gMCANMode == MCAN_APP_TEST_EVM_MODE_RX) || (gMCANMode == MCAN_APP_TEST_EVM_MODE_FIFO))
    {
        if (gMCANMode == MCAN_APP_TEST_EVM_MODE_RX)
        {
            /* Setup the receive message object */
            rxMsgObjectParams.direction = CANFD_Direction_RX;
            rxMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
            rxMsgObjectParams.msgIdentifier = 0xC1;

            rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);
            if (rxMsgObjHandle == NULL)
            {
                UART_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
                return -1;
            }
            else
            {
                UART_printf ("Debug: EVM in Rx buffer mode...\n");
            }

        }
        else if (gMCANMode == MCAN_APP_TEST_EVM_MODE_FIFO)
        {
            rxRangeMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
            rxRangeMsgObjectParams.startMsgIdentifier = 0xC0;
            rxRangeMsgObjectParams.endMsgIdentifier = 0xC4;

            rxRangeMsgObjHandle = CANFD_createRxRangeMsgObject (canHandle, &rxRangeMsgObjectParams, &errCode);
            if (rxRangeMsgObjHandle == NULL)
            {
                UART_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
                return -1;
            }
            else
            {
                UART_printf ("Debug: EVM in Rx FIFO mode...\n");
            }
        }

        /* Wait for the packet to arrive */
        while (gRxPkts != MCAN_APP_TEST_MESSAGE_COUNT);

        gRxDoneFlag = 0;

        if (gMCANMode == MCAN_APP_TEST_EVM_MODE_RX)
            msgObjStats.handle = rxMsgObjHandle;
        else
            msgObjStats.handle = rxRangeMsgObjHandle;

        optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
        optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
        optionTLV.value = (void*) &msgObjStats;

        retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
        if (retVal < 0)
        {
            UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        }

        UART_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
        UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
        UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
        UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
        UART_printf("Debug: Number of messages received       : %d\n", gRxPkts);
        UART_printf("\n\n");


        UART_printf("Debug: Receive & Transmit Measurements\n");
        UART_printf("Debug: Rx Min:%d Max:%d Total:%d ticks, Total packets: %d\n", minRxTicks, maxRxTicks, totalRxTicks, gRxPkts);

        /* 8 bits per byte * (data size + header) * Number of packet * VBUS frequency / total ticks */
        throughput = 8.0 * (MCAN_APP_TEST_DATA_SIZE + 16U) * gRxPkts * VBUSP_FREQ / totalRxTicks;
        UART_printf("Debug: Rx Throughput: %.2f Mbps\n", throughput);

        UART_printf("\n\n");

        optionTLV.type = CANFD_Option_MCAN_ERROR_COUNTER;
        optionTLV.length = sizeof(CANFD_MCANErrCntStatus);
        optionTLV.value = (void*) &errCounter;

        retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
        if (retVal < 0)
        {
            UART_printf ("Error: CANFD get error counter failed [Error code %d]\n", errCode);
        }

        UART_printf("Debug: Receive passive status            : %d\n", errCounter.rpStatus);
        UART_printf("Debug: Transmit Error Counter            : %d\n", errCounter.transErrLogCnt);
        UART_printf("Debug: Receive Error Counter             : %d\n", errCounter.recErrCnt);
        UART_printf("Debug: Error Logging Counter             : %d\n", errCounter.canErrLogCnt);
        UART_printf("\n\n");

        optionTLV.type = CANFD_Option_MCAN_PROTOCOL_STATUS;
        optionTLV.length = sizeof(CANFD_MCANProtocolStatus);
        optionTLV.value = (void*) &protoStatus;

        retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
        if (retVal < 0)
        {
            UART_printf ("Error: CANFD get protocol status failed [Error code %d]\n", errCode);
        }

        UART_printf("Debug: LEC                               : %d\n", protoStatus.lastErrCode);
        UART_printf("Debug: Activity                          : %d\n", protoStatus.act);
        UART_printf("Debug: Error Passive                     : %d\n", protoStatus.errPassive);
        UART_printf("Debug: Warning Status                    : %d\n", protoStatus.warningStatus);
        UART_printf("Debug: Bus Off Status                    : %d\n", protoStatus.busOffStatus);
        UART_printf("Debug: Data Phase LEC                    : %d\n", protoStatus.dlec);
        UART_printf("Debug: Rx ESI flag                       : %d\n", protoStatus.resi);
        UART_printf("Debug: Rx BRS flag                       : %d\n", protoStatus.rbrs);
        UART_printf("Debug: Rx CAN FD                         : %d\n", protoStatus.rfdf);
        UART_printf("Debug: Protocol Exception Event          : %d\n", protoStatus.pxe);
        UART_printf("Debug: TDC value                         : %d\n", protoStatus.tdcv);
        UART_printf("\n\n");
    }

    return 0;
}

static int32_t mcanMultiTransmission()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle[5U];
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams[5U];
    int32_t                     index;
    uint32_t                    numIterations;

    gTxDoneFlag = 0;
    gRxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(0U, &mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        UART_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (80)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (80)/(((2+8+6)+5)*2) = 1Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x4U;
    mcanBitTimingParams.dataPropSeg = 0x8U;
    mcanBitTimingParams.dataPseg1   = 0x6U;
    mcanBitTimingParams.dataPseg2   = 0x5U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    numIterations = 0;
    while (numIterations < 5)
    {
        /* Setup the transmit message object */
        for(index = 0; index < 5U ; index++)
        {
            txMsgObjectParams[index].direction = CANFD_Direction_TX;
            txMsgObjectParams[index].msgIdType = CANFD_MCANXidType_29_BIT;
            txMsgObjectParams[index].msgIdentifier = 0xC1 + index;

            txMsgObjHandle[index] = CANFD_createMsgObject (canHandle, &txMsgObjectParams[index], &errCode);
            if (txMsgObjHandle[index] == NULL)
            {
                UART_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
                return -1;
            }
        }
        gTxPkts = 0;
        for(index = 0; index < 5U ; index++)
        {
            retVal = CANFD_transmitData (txMsgObjHandle[index], txMsgObjectParams[index].msgIdentifier, CANFD_MCANFrameType_CLASSIC, 8U, &txData[0], &errCode);
            if (retVal < 0)
            {
                UART_printf ("Error: CANFD transmit data retry failed [Error code %d]\n", errCode);
            }
            TaskP_sleep(100);
        }

        /* Delete the transmit message object */
        for(index = 0; index < 5U ; index++)
        {
            retVal = CANFD_deleteMsgObject(txMsgObjHandle[index], &errCode);
            if (retVal < 0)
            {
                UART_printf ("Error: CANFD delete Tx message object failed [Error code %d]\n", errCode);
                return -1;
            }
        }
        numIterations++;
    }
    return 0;
}

static int32_t mcanMsgIdRangeTest()
{
    CANFD_Handle                        canHandle;
    CANFD_MsgObjHandle                  rxMsgObjHandle;
    CANFD_MsgObjHandle                  rxMsgEndObjHandle;
    CANFD_MsgObjHandle                  rxRangeMsgObjHandle[3U];
    int32_t                             retVal = 0;
    int32_t                             errCode = 0;
    CANFD_MCANInitParams                mcanCfgParams;
    CANFD_MCANBitTimingParams           mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams           rxMsgObjectParams;
    CANFD_MCANRxMsgObjRangeCfgParams    rxRangeMsgObjectParams;
    CANFD_MCANMsgObjectStats            msgObjStats;
    CANFD_OptionTLV                     optionTLV;
    int32_t                             index;

    gRxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(0U, &mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        UART_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (80)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Data Bit rate = (80)/(((1+2+1)+1)*BRP) = 4Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x4U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x1U;
    mcanBitTimingParams.dataPseg2   = 0x1U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        UART_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the single receive message object */
    rxMsgObjectParams.direction = CANFD_Direction_RX;
    rxMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    rxMsgObjectParams.msgIdentifier = 0xC1;

    rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);

    if (rxMsgObjHandle == NULL)
    {
        UART_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }


    /* Setup the single receive message object used to terminate the test */
    rxMsgObjectParams.direction = CANFD_Direction_RX;
    rxMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    rxMsgObjectParams.msgIdentifier = 0xDA;

    rxMsgEndObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);

    if (rxMsgEndObjHandle == NULL)
    {
        UART_printf ("Error: CANFD create Terminate Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the range receive message object */
    for(index = 0; index < 3U ; index++)
    {
        rxRangeMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
        rxRangeMsgObjectParams.startMsgIdentifier = 0xC2 + (10 * index);
        rxRangeMsgObjectParams.endMsgIdentifier = 0xC4 + (10 * index);

        rxRangeMsgObjHandle[index] = CANFD_createRxRangeMsgObject (canHandle, &rxRangeMsgObjectParams, &errCode);
        if (rxRangeMsgObjHandle[index] == NULL)
        {
            UART_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
            return -1;
        }
    }


    while (1)
    {

        /* Wait for the packet to arrive */
        while (gRxDoneFlag == 0);
        gRxDoneFlag = 0;

        TaskP_sleep(100);

        if (gTerminate == 1)
        {
            gDisplayStats = 1U;
        }

        if (gDisplayStats == 1U)
        {
            gDisplayStats = 0;

            msgObjStats.handle = rxMsgObjHandle;
            optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
            optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
            optionTLV.value = (void*) &msgObjStats;

            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
            }

            UART_printf("Debug: Single Message Identifier\n");
            UART_printf("Debug: Start Message Identifier          : 0x%x\n", msgObjStats.startMsgIdentifier);
            UART_printf("Debug: End Message Identifier            : 0x%x\n", msgObjStats.endMsgIdentifier);
            UART_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
            UART_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
            UART_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
            UART_printf("\n\n");

            for(index = 0; index < 3U ; index++)
            {
                msgObjStats.handle = rxRangeMsgObjHandle[index];

                retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
                if (retVal < 0)
                {
                    UART_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
                }

                UART_printf("Debug: Range Message Identifier\n");
                UART_printf("Debug: %d Start Message Identifier          : 0x%x\n", index, msgObjStats.startMsgIdentifier);
                UART_printf("Debug: %d End Message Identifier            : 0x%x\n", index, msgObjStats.endMsgIdentifier);
                UART_printf("Debug: %d Direction                         : %s\n", index, (msgObjStats.direction == 0) ? "Receive" : "Transmit");
                UART_printf("Debug: %d Number of interrupts received     : %d\n", index, msgObjStats.interruptsRxed);
                UART_printf("Debug: %d Number of messages processed      : %d\n", index, msgObjStats.messageProcessed);
                UART_printf("\n\n");
            }
            UART_printf("Debug: Number of Frame mismatch            : %d\n", gErrFrameType);
            UART_printf("Debug: Number of Get_Data errors           : %d\n", gErrGetData);
            UART_printf("Debug: Error Status Interrupt              : %d\n", gErrStatusInt);
        }

        /* Terminate the test */
        if (gTerminate == 1)
        {
            break;
        }
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(void* arg0, void* arg1)
{
    int32_t         retVal = 0;

    /* Initialize the platform */
    retVal = PlatformInit();
    if (retVal < 0)
    {
        OS_stop();
    }

    CycleprofilerP_init();

    while (1)
    {
#if defined (SIM_BUILD)
        UART_printf ("CANFD Unit Test\nMCAN Internal loopback test\n");
        testSelection = MCAN_APP_TEST_INTERNAL_LOOPBACK;
#else
        UART_printf ("*******************************************************\n");
        UART_printf ("CANFD Unit Test Menu                        \n");
        UART_printf ("Please select the type of test to execute:  \n");
        UART_printf ("1. MCAN Internal loopback test              \n");
        UART_printf ("2. MCAN External loopback test              \n");
        UART_printf ("3. MCAN Multiple Tx test                    \n");
        UART_printf ("4. MCAN External Tx/Rx test                 \n");
        UART_printf ("5. MCAN EVM-EVM test                        \n");
        UART_printf ("6. MCAN Tx Cancel test                      \n");
        UART_printf ("7. MCAN Power down test                     \n");
        UART_printf ("8. MCAN Message Id Range test               \n");
        UART_printf ("*******************************************************\n");
        UART_printf ("> Enter your selection: ");

        UART_scanFmt ("%d",&testSelection);
#endif

        /* Validate the selection: */
        if ((testSelection >= MCAN_APP_TEST_INTERNAL_LOOPBACK) && (testSelection <= MCAN_APP_TEST_MSGID_RANGE))
            break;
    }

    if (testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK)
    {
        UART_printf("Debug: Internal loopback testing\n");
        retVal = mcanLoopbackTest();
        if (retVal == -1)
            UART_printf("Internal loopback testing : Fail\n");
        else
            UART_printf("Internal loopback testing : Pass\n");
    }
    else if (testSelection == MCAN_APP_TEST_EXTERNAL_LOOPBACK)
    {
        UART_printf("Debug: External loopback testing\n");
        retVal = mcanLoopbackTest();
        if (retVal == -1)
            UART_printf("External loopback testing : Fail\n");
        else
            UART_printf("External loopback testing : Pass\n");
    }
    else if (testSelection == MCAN_APP_TEST_TX_CANCEL)
    {
        UART_printf("Debug: Tx Cancel testing\n");
        retVal = mcanTestTxCancel();
        if (retVal == -1)
            UART_printf("Tx Cancel testing: Fail \n");
        else
            UART_printf("Tx Cancel testing: Pass \n");
    }
    else if (testSelection == MCAN_APP_TEST_EXTERNAL_DATA)
    {
        UART_printf("Debug: External transmit testing\n");
        retVal = mcanTransmitTest();
        if (retVal == -1)
            UART_printf("External transmit testing: Fail \n");
        else
            UART_printf("External transmit testing: Pass \n");
    }
    else if (testSelection == MCAN_APP_TEST_EVM_EVM)
    {
        UART_printf("Debug: EVM-EVM testing\n");
        retVal = mcanEVM_EVMTest();
        if (retVal == -1)
            UART_printf("EVM to EVM testing FAIL");
        else
            UART_printf("EVM to EVM testing PASS");
    }
    else if (testSelection == MCAN_APP_TEST_POWERDOWN)
    {
        UART_printf("Debug: Power Down testing\n");
        retVal = mcanPowerDownTest();
        if (retVal == -1)
            UART_printf("Power Down testing FAIL");
        else
            UART_printf("Power Down testing PASS");
    }
    else if (testSelection == MCAN_APP_TEST_MULTIPLE_TRANSMISSION)
    {
        UART_printf("Debug: Multiple transmission testing\n");
        retVal = mcanMultiTransmission();
        if (retVal == -1)
            UART_printf("Multiple transmission testing FAIL");
        else
            UART_printf("Multiple transmission testing PASS");
    }
    else if (testSelection == MCAN_APP_TEST_MSGID_RANGE)
    {
        UART_printf("Debug: Message Id Range testing\n");
        retVal = mcanMsgIdRangeTest();
        if (retVal == -1)
            UART_printf("Message Id Range testing FAIL");
        else
            UART_printf("Message Id Range testing PASS");
    }

    if (retVal < 0)
    {
        UART_printf("Debug:CANFD testing failed\n");
    }


    /* Exit BIOS */
    OS_stop();

    return;
}

/**
 *  @b Description
 *  @n
 *      This is the entry point into the unit test code
 *
 *  @retval
 *      Not Applicable.
 */
int32_t main (void)
{
    TaskP_Params taskParams;

    OS_init();

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U, 0U);

    /* Initialize the Task Parameters. */
    TaskP_Params_init(&taskParams);
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    TaskP_create(&Test_initTask, &taskParams);

    OS_start();

    return 0;
}



