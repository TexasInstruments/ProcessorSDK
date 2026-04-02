/******************************************************************************
* Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
*****************************************************************************/

/**
 *  \file   csirx_test_tpr12.c
 *
 *  \brief  csirx diagnostic test file.
 *
 *  Targeted Functionality: Verification of csirx interface by receiving frames from
 *                          IWR1443 and AWR2243 radar sensors.
 *
 *  Operation: This test verifies CSIRX interface by receiving
 *              1. User defined fixed data pattern from IWR1443 radar sensor
 *                    - fixed data validation done in case of IWR1443 sensor along
 *					    with verifying number of frames and frame size.
 *              2. Actual frames transmitted from the AWR2243 radar sensor
 *                    - Varifying the number of frames received and
 *                      size of each frame received in case of AWR2243 sensor.
 *
 *  Supported SoCs: TPR12.
 *
 *  Supported Platforms: tp12_evm.
 */

#include <csirx_test_tpr12.h>

/* Global variable to indicate the frame received */
volatile bool gFrameReceived = 0;
/* Global variable to check the error code */
uint32_t gErrorCode = 0;
/* Global variable to count the number of frame received */
uint32_t gFrameCounter = 0;

/* L3 RAM Ping Buffer */
#pragma DATA_SECTION(gTestPingBufL3, ".l3ram");
uint8_t gTestPingBufL3[BOARD_DIAG_CSIRX_TEST_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_CSIRX_TEST_PING_PONG_ALIGNMENT)));
/* HWA RAM Ping Buffer */
#pragma DATA_SECTION(gTestPingBufHWA, ".hwaram");
uint8_t gTestPingBufHWA[BOARD_DIAG_CSIRX_TEST_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_CSIRX_TEST_PING_PONG_ALIGNMENT)));
/* L2 RAM Ping Buffer */
#pragma DATA_SECTION(gTestPingBufL2, ".l2ram");
uint8_t gTestPingBufL2[BOARD_DIAG_CSIRX_TEST_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_CSIRX_TEST_PING_PONG_ALIGNMENT)));

/* L3 RAM Pong Buffer */
#pragma DATA_SECTION(gTestPongBufL3, ".l3ram");
uint8_t gTestPongBufL3[BOARD_DIAG_CSIRX_TEST_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_CSIRX_TEST_PING_PONG_ALIGNMENT)));
/* HWA RAM Pong Buffer */
#pragma DATA_SECTION(gTestPongBufHWA, ".hwaram");
uint8_t gTestPongBufHWA[BOARD_DIAG_CSIRX_TEST_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_CSIRX_TEST_PING_PONG_ALIGNMENT)));
/* L2 RAM Pong Buffer */
#pragma DATA_SECTION(gTestPongBufL2, ".l2ram");
uint8_t gTestPongBufL2[BOARD_DIAG_CSIRX_TEST_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_CSIRX_TEST_PING_PONG_ALIGNMENT)));

/* Global structure to hold the states of CSIRX test */
static BoardDiag_State_t gTestState = {0};
/* Common callback function */
static void BoardDiag_commonCallback(CSIRX_Handle handle, uint32_t arg,
                              CSIRX_CommonIRQ_t *IRQ);
#ifndef BUILD_DSP_1
/* Combined EOF callback function */
static void BoardDiag_combinedEOFcallback(CSIRX_Handle handle, uint32_t arg);
#endif
/* Intialization of CSIRX configuration structure */
static BoardDiag_Config_t gTestConfig =
{
    /* DDR clock set to 300 MHz */
    .DPHYcfg.ddrClockInHz = 300000000U,
    .DPHYcfg.isClockMissingDetectionEnabled = true,
    .DPHYcfg.triggerEscapeCode[0] = 0x0,
    .DPHYcfg.triggerEscapeCode[1] = 0x0,
    .DPHYcfg.triggerEscapeCode[2] = 0x0,
    .DPHYcfg.triggerEscapeCode[3] = 0x0,

    .complexIOcfg.lanesConfig.dataLane[0].polarity = 0,
    .complexIOcfg.lanesConfig.dataLane[0].position = CSIRX_LANE_POSITION_1,
    .complexIOcfg.lanesConfig.dataLane[1].polarity = 0,
    .complexIOcfg.lanesConfig.dataLane[1].position = CSIRX_LANE_POSITION_2,
    .complexIOcfg.lanesConfig.dataLane[2].polarity = 0,
    .complexIOcfg.lanesConfig.dataLane[2].position = CSIRX_LANE_POSITION_4,
    .complexIOcfg.lanesConfig.dataLane[3].polarity = 0,
    .complexIOcfg.lanesConfig.dataLane[3].position = CSIRX_LANE_POSITION_5,
    .complexIOcfg.lanesConfig.clockLane.polarity = 0,
    .complexIOcfg.lanesConfig.clockLane.position = CSIRX_LANE_POSITION_3,
    .complexIOcfg.lanesIRQ.isAllLanesULPMenter = true,
    .complexIOcfg.lanesIRQ.isAllLanesULPMexit = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isControlError = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isStartOfTransmisionError = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isControlError = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isStartOfTransmisionError = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isControlError = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isStartOfTransmisionError = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isControlError = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isStartOfTransmisionError = true,
    .complexIOcfg.lanesIRQ.clockLane.isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.clockLane.isControlError = true,
    .complexIOcfg.lanesIRQ.clockLane.isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.clockLane.isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.clockLane.isStartOfTransmisionError = true,

    .commonCfg.isSoftStoppingOnInterfaceDisable = true,
    .commonCfg.isECCenabled = false,
    .commonCfg.isEXP16SignExtensionEnabled = false,
    .commonCfg.isBurstSizeExpand = false,
    .commonCfg.isNonPostedWrites = true,
    .commonCfg.isOCPautoIdle = true,
    .commonCfg.stopStateFSMtimeoutInNanoSecs = 200000U,
    .commonCfg.burstSize = 8,
    .commonCfg.endianness = CSIRX_ALL_LITTLE_ENDIAN,
    .commonCfg.startOfFrameIRQ0contextId = BOARD_DIAG_CSIRX_TEST_CONTEXT,
    .commonCfg.startOfFrameIRQ1contextId = 0,
    .commonCfg.endOfFrameIRQ0contextId = BOARD_DIAG_CSIRX_TEST_CONTEXT,
    .commonCfg.endOfFrameIRQ1contextId = 0,
    .commonCfg.IRQ.isOCPerror = true,
    .commonCfg.IRQ.isGenericShortPacketReceive = false,
    .commonCfg.IRQ.isECConeBitShortPacketErrorCorrect = false,
    .commonCfg.IRQ.isECCmoreThanOneBitCannotCorrect = false,
    .commonCfg.IRQ.isComplexIOerror = true,
    .commonCfg.IRQ.isFIFOoverflow = true,
    .commonCfg.IRQ.isContext[0] = false,
    .commonCfg.IRQ.isContext[1] = false,
    .commonCfg.IRQ.isContext[2] = false,
    .commonCfg.IRQ.isContext[3] = false,
    .commonCfg.IRQ.isContext[4] = false,
    .commonCfg.IRQ.isContext[5] = false,
    .commonCfg.IRQ.isContext[6] = false,
    .commonCfg.IRQ.isContext[7] = false,
    .commonCfg.IRQcallbacks.common.fxn = BoardDiag_commonCallback,
    .commonCfg.IRQcallbacks.common.arg = BOARD_DIAG_CSIRX_TEST_COMMON_CB_ARG,
    .commonCfg.IRQcallbacks.combinedEndOfLine.fxn = NULL,
    .commonCfg.IRQcallbacks.combinedEndOfLine.arg = 0,
#ifdef BUILD_DSP_1
    .commonCfg.IRQcallbacks.combinedEndOfFrame.fxn = NULL,
    .commonCfg.IRQcallbacks.combinedEndOfFrame.arg = 0,
#else
    .commonCfg.IRQcallbacks.combinedEndOfFrame.fxn =
        BoardDiag_combinedEOFcallback,
    .commonCfg.IRQcallbacks.combinedEndOfFrame.arg =
        BOARD_DIAG_CSIRX_TEST_COMBINED_EOF_CB_ARG,
#endif
    .commonCfg.IRQcallbacks.startOfFrameIRQ0.fxn = NULL,
    .commonCfg.IRQcallbacks.startOfFrameIRQ0.arg = 0,
    .commonCfg.IRQcallbacks.startOfFrameIRQ1.fxn = NULL,
    .commonCfg.IRQcallbacks.startOfFrameIRQ1.arg = 0,
    .commonCfg.IRQcallbacks.endOfFrameIRQ0.fxn = NULL,
    .commonCfg.IRQcallbacks.endOfFrameIRQ0.arg = 0,
    .commonCfg.IRQcallbacks.endOfFrameIRQ1.fxn = NULL,
    .commonCfg.IRQcallbacks.endOfFrameIRQ1.arg = 0,

    .contextCfg.virtualChannelId = BOARD_DIAG_CSIRX_TEST_VC,
    .contextCfg.format = BOARD_DIAG_CSIRX_TEST_FORMAT,
    .contextCfg.userDefinedMapping =
            BOARD_DIAG_CSIRX_TEST_USER_DEFINED_MAPPING,
    .contextCfg.isByteSwapEnabled = false,
    .contextCfg.isGenericEnabled = false,
    .contextCfg.isTranscodingEnabled = false,
    .contextCfg.transcodeConfig.transcodeFormat =
                                CSIRX_TRANSCODE_FORMAT_NO_TRANSCODE,
    .contextCfg.transcodeConfig.isHorizontalDownscalingBy2Enabled = false,
    .contextCfg.transcodeConfig.crop.horizontalCount = 0,
    .contextCfg.transcodeConfig.crop.horizontalSkip = 0,
    .contextCfg.transcodeConfig.crop.verticalCount = 0,
    .contextCfg.transcodeConfig.crop.verticalSkip = 0,
    .contextCfg.alpha = 0,
    .contextCfg.pingPongConfig.pingPongSwitchMode =
            CSIRX_PING_PONG_FRAME_SWITCHING,
    .contextCfg.pingPongConfig.numFramesForFrameBasedPingPongSwitching = 1,
    .contextCfg.pingPongConfig.lineOffset =
                            CSIRX_LINEOFFSET_CONTIGUOUS_STORAGE,
    .contextCfg.pingPongConfig.pingAddress = NULL,
    .contextCfg.pingPongConfig.pongAddress = NULL,
    .contextCfg.numFramesToAcquire = CSIRX_NUM_FRAMES_TO_ACQUIRE_INFINITE,
    .contextCfg.IRQ.isNumLines = false,
    .contextCfg.isNumLinesForIRQmoduloWithinFrame = false,
    .contextCfg.numLinesForIRQ = 0,
    .contextCfg.IRQ.isFramesToAcquire = false,
    .contextCfg.IRQ.isPayloadChecksumMismatch = false,
    .contextCfg.IRQ.isFrameStartCodeDetect = true,
    .contextCfg.IRQ.isFrameEndCodeDetect = true,
    .contextCfg.IRQ.isLineStartCodeDetect = false,
    .contextCfg.IRQ.isLineEndCodeDetect = true,
    .contextCfg.IRQ.isECConeBitLongPacketCorrect = false,
    .contextCfg.endOfLineIRQcallback.fxn = NULL,
    .contextCfg.endOfLineIRQcallback.arg = NULL,
    .contextCfg.isEndOfFramePulseEnabled = true,
    .contextCfg.isEndOfLinePulseEnabled = false,
    .contextCfg.isChecksumEnabled = true
};

/**
 *  \brief    Used to read the payload data from the ping-pong buffers
 *            This function compares the data received from the buffers
 *            to expected value.
 *
 *  \param    handle              [IN]     CSIRX Handler
 *
 *  \return   NULL
 *
 */
static void BoardDiag_checkPayloadReceived(CSIRX_Handle handle)
{
    uint32_t numBytes = 0;
    uint32_t buffer, bufIndx = 0;
    uint8_t *pBuf;

    gErrorCode = CSIRX_getContextReceivedBuffer(handle,
                        BOARD_DIAG_CSIRX_TEST_CONTEXT, &buffer);
    DebugP_assert(gErrorCode == CSIRX_NO_ERROR);
    buffer = CSL_globToLocAddr(buffer);
    pBuf = (uint8_t *)buffer;
    CacheP_Inv(pBuf,BOARD_DIAG_CSIRX_TEST_PING_OR_PONG_BUF_SIZE_ALIGNED);
/* Verifying fixed data pattern received for IWR1443 device only */
#if defined(FE_IWR1443)
    for(numBytes = 0; numBytes <
        BOARD_DIAG_CSIRX_TEST_PING_OR_PONG_BUF_SIZE_ALIGNED; numBytes++)
    {
        if(pBuf[bufIndx++] != BOARD_DIAG_CSIRX_TEST_PATTERN)
        {
            gTestState.isReceivedPayloadCorrect = false;
            UART_printf("Frame - %d is invalid\n",
            gTestState.contextIRQcounts[BOARD_DIAG_CSIRX_TEST_CONTEXT].
                                        frameEndCodeDetect);
            break;
        }
    }
#else
    numBytes = BOARD_DIAG_CSIRX_TEST_PING_OR_PONG_BUF_SIZE_ALIGNED;
    bufIndx = BOARD_DIAG_CSIRX_TEST_PING_OR_PONG_BUF_SIZE_ALIGNED;
#endif
/* Verifying the buffer overflow check */
    if (numBytes == BOARD_DIAG_CSIRX_TEST_PING_OR_PONG_BUF_SIZE_ALIGNED)
    {
        for (; numBytes < (BOARD_DIAG_CSIRX_TEST_INIT_PATTERN_SIZE);numBytes++)
        {
            if(pBuf[bufIndx++] != BOARD_DIAG_CSIRX_TEST_BUF_INIT_PATTERN)
            {
                gTestState.isReceivedPayloadCorrect = false;
                UART_printf("Buffer corruption - %d is invalid\n",
                gTestState.contextIRQcounts[BOARD_DIAG_CSIRX_TEST_CONTEXT].
                                            frameEndCodeDetect);
                break;
            }
        }
    }
}

/**
 *  \brief    Callback function for common.irq interrupt, generated when
 *            end of frame code and line code detected.
 *
 *  \param    handle              [IN]     CSIRX Handler
 *  \param    arg                 [IN]     CALLBACK function argument
 *  \param    IRQ                 [OUT]    CSIRX common irq
 *
 *  \return   NULL
 *
 */
static void BoardDiag_commonCallback(CSIRX_Handle handle, uint32_t arg,
                              CSIRX_CommonIRQ_t *IRQ)
{
    uint8_t context;
    uint32_t frameCounter =
    gTestState.contextIRQcounts[BOARD_DIAG_CSIRX_TEST_CONTEXT].
                                frameEndCodeDetect + 1;

    DebugP_assert(handle != NULL);
    DebugP_assert(arg == BOARD_DIAG_CSIRX_TEST_COMMON_CB_ARG);
    gTestState.callbackCount.common++;

    gTestState.IRQ.common = *IRQ;

    /* Counts book-keeping */
    if(IRQ->isOCPerror == true)
    {
        gTestState.commonIRQcount.isOCPerror++;
    }
    if(IRQ->isComplexIOerror == true)
    {
        gTestState.commonIRQcount.isComplexIOerror++;
    }
    if(IRQ->isFIFOoverflow == true)
    {
        gTestState.commonIRQcount.isFIFOoverflow++;
    }

    if(IRQ->isComplexIOerror)
    {
        gErrorCode = CSIRX_getComplexIOlanesIRQ(handle,
                                               &gTestState.IRQ.complexIOlanes);
        if(gErrorCode != CSIRX_NO_ERROR)
        {
            UART_printf("Error occured while recieving the frame-%d\n",
                        frameCounter);
        }
        DebugP_assert(gErrorCode == CSIRX_NO_ERROR);

        gErrorCode = CSIRX_clearAllcomplexIOlanesIRQ(handle);
        DebugP_assert(gErrorCode == CSIRX_NO_ERROR);
    }

    for(context = 0; context < CSIRX_NUM_CONTEXTS; context++)
    {
        if(IRQ->isContext[context] == true)
        {
            gErrorCode = CSIRX_getContextIRQ(handle, context,
                                            &gTestState.IRQ.context[context]);
            DebugP_assert(gErrorCode == CSIRX_NO_ERROR);

            if(gTestState.IRQ.context[context].isFrameEndCodeDetect == true)
            {
                gTestState.contextIRQcounts[context].frameEndCodeDetect++;
                  /* Single frame is received */
                gFrameReceived = true;

            }

            if(gTestState.IRQ.context[context].isLineEndCodeDetect == true)
            {
                gTestState.contextIRQcounts[context].lineEndCodeDetect++;
            }

            gErrorCode = CSIRX_clearAllcontextIRQ(handle, context);
            DebugP_assert(gErrorCode == CSIRX_NO_ERROR);
        }
    }
}

/**
 *  \brief    State error check function, this function will be called
 *            Once all the frames are received to check the error states.
 *
 *  \param    pIsTestPass              [OUT]    true if there is no state error
 *
 *  \return   NULL
 *
 */
static void BoardDiag_checkStateError(bool *pIsTestPass)
{
    if(gTestState.commonIRQcount.isOCPerror != 0)
    {
        UART_printf("OCP error has occured %d number of times \n",
                    gTestState.commonIRQcount.isOCPerror);
        *pIsTestPass = false;
    }
    if(gTestState.commonIRQcount.isComplexIOerror != 0)
    {
        UART_printf("Complex IO error has occured %d number of times \n",
                    gTestState.commonIRQcount.isComplexIOerror);
        *pIsTestPass = false;
    }
    if(gTestState.commonIRQcount.isFIFOoverflow != 0)
    {
        UART_printf("FIFO Overflow error has occured %d number of times \n",
                    gTestState.commonIRQcount.isFIFOoverflow);
        *pIsTestPass = false;
    }
}
#ifndef BUILD_DSP_1
/**
 *  \brief    Callback function to be called once the Combined EOF detected.
 *
 *  \param    handle              [IN]     CSIRX Handler
 *  \param    arg                 [IN]     CB function argument
 *
 *  \return   NULL
 *
 */
static void BoardDiag_combinedEOFcallback(CSIRX_Handle handle, uint32_t arg)
{
    DebugP_assert(handle != NULL);
    DebugP_assert(arg == BOARD_DIAG_CSIRX_TEST_COMBINED_EOF_CB_ARG);
    gTestState.callbackCount.combinedEOF++;
}
#endif
/**
 *  \brief    Initialize the ping pong buffers to reset values
 *
 *  \param    pingBuf   [OUT]    Ping buffer
 *  \param    pongBuf   [OUT]    Pong buffer
 *  \param    sizeBuf   [IN]    Size of the buffer
 *
 *  \return   NULL
 *
 */
static void BoardDiag_initBuf(uint32_t pingBuf, uint32_t pongBuf,
                              uint32_t sizeBuf)
{
    /* initialize ping/pong bufs to known failing pattern */
    memset((void *)pingBuf, BOARD_DIAG_CSIRX_TEST_BUF_INIT_PATTERN, sizeBuf);
    CacheP_wbInv((void *)pingBuf, sizeBuf);
    memset((void *)pongBuf, BOARD_DIAG_CSIRX_TEST_BUF_INIT_PATTERN, sizeBuf);
    CacheP_wbInv((void *)pongBuf, sizeBuf);
}

/**
 *  \brief    This function initializes test state variable.
 *
 *  \param    NULL
 *
 *  \return   NULL
 *
 */
static void BoardDiag_testInit(void)
{
    memset(&gTestState, 0, sizeof(gTestState));

    gTestState.isReceivedPayloadCorrect = true;

}

/**
 *  \brief    Gets test buffer address from a bunch of input parameters
 *
 *  \param    BoardDiag_RamType   [IN]    Type of buffer RAM
 *  \param    pBuf                [OUT]   Pointer to where the buffer address
 *                                        is returned
 *  \param    isPing              [IN]    true if ping buffer else pong buffer
 *
 *  \return   NULL
 *
 */
static void BoardDiag_getBuf(BoardDiag_RamType bufRAMtype, uint32_t *pBuf,
                             bool isPing)
{
    if(isPing == true)
    {
        switch(bufRAMtype)
        {
        case BOARD_DIAG_CSIRX_TEST_L3RAM:
            *pBuf = (uint32_t) &gTestPingBufL3;
            break;
        case BOARD_DIAG_CSIRX_TEST_HWARAM:
            *pBuf = (uint32_t) &gTestPingBufHWA;
            break;
        case BOARD_DIAG_CSIRX_TEST_L2RAM:
            *pBuf = (uint32_t) &gTestPingBufL2;
            break;
        }
    }
    else
    {
        switch(bufRAMtype)
        {
        case BOARD_DIAG_CSIRX_TEST_L3RAM:
            *pBuf = (uint32_t) &gTestPongBufL3;
            break;
        case BOARD_DIAG_CSIRX_TEST_HWARAM:
            *pBuf = (uint32_t) &gTestPongBufHWA;
            break;
        case BOARD_DIAG_CSIRX_TEST_L2RAM:
            *pBuf = (uint32_t) &gTestPongBufL2;
            break;
        }
    }
}

/**
 *  \brief    The function performs the CSI-Rx Diagnostic
 *            test.
 *
 *  \param    instanceId        [IN]        Csirx instance ID
 *
 *  \return   bool
 *              true  - in case of success.
 *              false - in case of failure.
 *
 */
static bool BoardDiag_csirxTestRun(uint8_t instanceId)
{
    CSIRX_Handle         handle;
    int32_t errorCode;
    uint32_t pingBuf, pongBuf;
    CSIRX_InstanceInfo_t instanceInfo;
    bool isTestPass = true;
    CSL_rcss_rcmRegs *pRcssRcmRegs = (CSL_rcss_rcmRegs *)CSL_RCSS_RCM_U_BASE;
    volatile bool isComplexIOresetDone;
    volatile uint8_t isComplexIOpowerStatus;

    /* get ping-pong buffer addresses based on the RAM type and context */
    BoardDiag_getBuf(BOARD_DIAG_CSIRX_TEST_HWARAM, &pingBuf, true);
    BoardDiag_getBuf(BOARD_DIAG_CSIRX_TEST_HWARAM, &pongBuf, false);

    BoardDiag_initBuf(pingBuf, pongBuf,
                      BOARD_DIAG_CSIRX_TEST_INIT_PATTERN_SIZE);
    /* initialize the ping-pong buffers */
    BoardDiag_testInit();

    CSL_FINS(pRcssRcmRegs->RCSS_CSI2A_RST_CTRL,
             RCSS_RCM_RCSS_CSI2A_RST_CTRL_RCSS_CSI2A_RST_CTRL_ASSERT, 0x7);

    CSL_FINS(pRcssRcmRegs->RCSS_CSI2A_RST_CTRL,
            RCSS_RCM_RCSS_CSI2A_RST_CTRL_RCSS_CSI2A_RST_CTRL_ASSERT, 0U);

    /* Initialize CSIRX */
    errorCode = CSIRX_init();
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_init failed with errorCode = %d\n", errorCode);
        isTestPass = false;
        return isTestPass;
    }

    /* Open the CSI Instance */
    handle = CSIRX_open(instanceId, NULL, &errorCode, &instanceInfo);
    if(handle == NULL)
    {
        if(errorCode == CSIRX_E_INVALID__INSTANCE_ID)
        {
            UART_printf("Csirx Instance not supported\n");
        }
        else
        {
            UART_printf("Unable to open the csirx Instance, erorCode = %d\n",
                        errorCode);
        }
        isTestPass = false;
        return isTestPass;
    }
    UART_printf("Instance opened, Revision = %d.%d, Number of "
            "Contexts = %d\n", instanceInfo.majorRevisionId,
            instanceInfo.minorRevisionId,
            instanceInfo.numContexts);

    /* reset csi */
    errorCode = CSIRX_reset(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_reset failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        goto exit;
    }
    /* config complex IO - lanes and IRQ */
    errorCode = CSIRX_configComplexIO(handle, &gTestConfig.complexIOcfg);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_configComplexIO failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
        goto exit;
    }

    /* deassert complex IO reset */
    errorCode = CSIRX_deassertComplexIOreset(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_deassertComplexIOreset failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
        goto exit;
    }

    /* config DPHY */
    errorCode = CSIRX_configDPHY(handle, &gTestConfig.DPHYcfg);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_configDPHY failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        goto exit;
    }

    errorCode = CSIRX_setComplexIOpowerCommand(handle, 1);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_setComplexIOpowerCommand failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
        goto exit;
    }
    UART_printf("Wait till the complex IO power up!\n");

    do
    {
        errorCode = CSIRX_getComplexIOpowerStatus(handle,
                                            (uint8_t*)&isComplexIOpowerStatus);
        if(errorCode != CSIRX_NO_ERROR)
        {
            UART_printf("CSIRX_getComplexIOpowerStatus failed, errorCode = "
            " %d\n", errorCode);
            isTestPass = false;
            goto exit;
        }

    } while((isComplexIOpowerStatus == 0));
    UART_printf("Complex IO Powered up.Run FE binrary config now\n");
    /* config common */
    gTestConfig.commonCfg.IRQ.isContext[BOARD_DIAG_CSIRX_TEST_CONTEXT] = true,
    errorCode = CSIRX_configCommon(handle, &gTestConfig.commonCfg);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_configCommon failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        goto exit;
    }
#ifndef USE_BIOS
#ifdef BUILD_C66X_1
    /* !! HACK to workaround c66x OSAL baremetal issue with not enabling Hwi interrupt associated with EventCombiner */
    /* Osal baremetal lib for c66x does not enable the Hwi interrupt number when Osal_enableInterrupt is called.
     * This is correct behaviour but the issue is when registering interrupt the Hwi interrupt number is not
     * enabled if Osal_registerInterrupt enableIntr param is set to FALSE> This iswrong and the Hwi interrupt
     * number should be enabled irrespective of a particular eventCombiner event is enabled or not.
     * Until this is fixed in c6xx osal baremetal library, adding hack in application to enable the interrupt
     */
    OsalArch_enableInterrupt(CSIRX_C66X_COMMON_ISR_HWI_INT_NUM);
#endif
#endif
    /* config contexts */
    /* assign ping pong address */
    gTestConfig.contextCfg.pingPongConfig.pingAddress =
            (uint32_t) CSL_locToGlobAddr(pingBuf);
    gTestConfig.contextCfg.pingPongConfig.pongAddress =
            (uint32_t) CSL_locToGlobAddr(pongBuf);

    errorCode = CSIRX_configContext(handle, BOARD_DIAG_CSIRX_TEST_CONTEXT,
                                &gTestConfig.contextCfg);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_configContext failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        goto exit;
    }

    /* enable context */
    errorCode = CSIRX_enableContext(handle, BOARD_DIAG_CSIRX_TEST_CONTEXT);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_enableContext failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        goto exit;
    }

    /* enable interface */
    errorCode = CSIRX_enableInterface(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_enableInterface failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
        goto exit;
    }

    /* Wait until complex IO reset complete */
    do
    {
        errorCode = CSIRX_isComplexIOresetDone(handle,
                                            (bool *)&isComplexIOresetDone);
        if(errorCode != CSIRX_NO_ERROR)
        {
            UART_printf("CSIRX_isComplexIOresetDone failed, errorCode = "
            " %d\n", errorCode);
            isTestPass = false;
            goto exit;
        }
        if (isComplexIOresetDone == false)
        {
            /* NOTE: This delay should be much smaller than frame time,
                     default BIOS tick = 1 ms */
            Osal_delay(1);
        }
    }while((isComplexIOresetDone == false));

    if(isComplexIOresetDone == false)
    {
        UART_printf("CSIRX_isComplexIOresetDone attempts exceeded\n");
        isTestPass = false;
        goto exit;
    }

    /*Wait csirx receive the data */
    while(gFrameCounter !=
          BOARD_DIAG_CSIRX_TEST_NUM_FRAMES)
    {
        if(gFrameReceived)
        {
            gFrameCounter++;
            BoardDiag_checkPayloadReceived(handle);
            gFrameReceived = false;

            /* Test is considered as failed even if any one of the frame
               received is invalid */
            if(isTestPass != false)
            {
                isTestPass = isTestPass & gTestState.isReceivedPayloadCorrect;
            }
        }
        Osal_delay(1);
    }

    if(gFrameCounter != BOARD_DIAG_CSIRX_TEST_NUM_FRAMES)
    {
        UART_printf("Number of frames recieved does not match\n");
        isTestPass = false;
    }

    UART_printf("Number of frame received - %d\n", gFrameCounter);

    BoardDiag_checkStateError(&isTestPass);

exit:
    /* disable context */
    errorCode = CSIRX_disableContext(handle, BOARD_DIAG_CSIRX_TEST_CONTEXT);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_disableContext failed,errorCode = %d\n", errorCode);
        isTestPass = false;
    }

    /* disable interface */
    errorCode = CSIRX_disableInterface(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_disableInterface failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
    }

    /* close instance */
    errorCode = CSIRX_close(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        UART_printf("CSIRX_close failed, errorCode = %d\n", errorCode);
        isTestPass = false;
    }

    return(isTestPass);
}

/**
 *  \brief    The function performs the CSIRX Diagnostic
 *            test.
 *
 *  \param    NULL      For Bare Metal test.
 *
 *  \param    arg0      [IN]    argument-0 for BIOS test
 *  \param    arg1      [IN]    argument-1 for BIOS test
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
#ifdef USE_BIOS
int8_t BoardDiag_csirxTest(UArg arg0, UArg arg1)
#else
int8_t BoardDiag_csirxTest(void)
#endif
{
    uint8_t result;
    uint32_t instanceId;
    uint8_t instName[25];

    UART_printf("\n**********************************************\n");
    UART_printf  ("*                CSI-Rx Test                 *\n");
    UART_printf  ("**********************************************\n");

    UART_printf("Please select the CSIRX instance id\n");
    UART_printf("0 -  CSIRX-A\n1 -  CSIRX-B\n");
    UART_scanFmt("%d\n", &instanceId);

    if((instanceId != CSIRX_INST_A_ID) && (instanceId != CSIRX_INST_B_ID))
    {
        UART_printf("Please select the valid CSIRX instance\n");
        return -1;
    }

    CSIRX_getInstanceName(instanceId, (char *)instName, sizeof(instName));

    UART_printf("Receiving data from CSIRX instance #%d: %s\n", instanceId,
                 instName);

    result = BoardDiag_csirxTestRun(instanceId);
    if(result != true)
    {
        UART_printf("Failed to receive data from CSIRX instance #%d\n",
                    instanceId);
        return -1;
    }
    else
    {
        UART_printf("CSIRX diag test passed\n");
    }
    UART_printf("csirx test finished\n");

    return 0;

}

/**
 *  \brief   CSIRX Diagnostic test main function
 *
 *  \param   NULL
 *
 *  \return  int - CSIRX Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 *
 */
#ifndef SPI_BOOT_FRAMEWORK
int main (void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;

#ifdef USE_BIOS
    Task_Handle task;
    Error_Block eb;

    Error_init(&eb);
    task = Task_create((Task_FuncPtr)BoardDiag_csirxTest, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }
#else
    int8_t ret;
#endif

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO | BOARD_INIT_MODULE_CLOCK;
#endif

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

#ifdef USE_BIOS
    /* Start BIOS */
    BIOS_start();
#else
    ret = BoardDiag_csirxTest();
    if(ret != 0)
    {
        UART_printf("\nCSIRX Test Failed\n");
        return -1;
    }
    else
    {
        UART_printf("\nCSIRX Test Passed\n");
    }
#endif

    return 0;
}
#endif
