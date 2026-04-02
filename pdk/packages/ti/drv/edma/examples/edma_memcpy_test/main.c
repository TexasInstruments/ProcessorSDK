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

/**
 *   @file  main.c
 *
 *   @brief
 *      example code for the EDMA driver.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* Standard Include Files. */
#include <stdint.h>
#include <stdio.h>

/* CSL Header files */
#include <ti/csl/soc.h>
#include <ti/csl/csl_types.h>

/* Driver Include files. */
#include <ti/osal/osal.h>
#include <ti/drv/edma/edma.h>
#include <ti/board/board.h>

#if !defined(BAREMETAL)
#include <ti/osal/TaskP.h>
#endif

#include "edma_log.h"
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define EDMA_UNTIED_CH0                     (63U)
#define EDMA_EVENT_QUEUE_ID_CH0             (0U)
#define TEST_PARAM_ID                       (0U)
#define TEST_TRANSFER_COMPLETION_CODE_CH0   (0U)

#define TEST_BUF_LENGTH                     (2000U)

/*
 * Define the CACHE_LINSEZ based on system Cache configuration
 * TPR12 use case: only L1 cache is enabled
 */
#define TF_CACHE_LINESZ                     CSL_CACHE_L1D_LINESIZE
#define TF_ROUND_UP(x, y)                   CSL_NEXT_MULTIPLE_OF_POW2(x,y)

#define TEST_TIMEOUT_CYCLES                 (1000000U)

#if !defined(BAREMETAL)
#define APP_TSK_STACK_MAIN              (16U * 1024U)
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static bool Test_isTransferSuccessful(void);
static bool Test_instance(uint8_t instanceId);
static void Test_initializeBuffers(void);
void Test_ErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo);
void Test_transferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo);


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
#pragma DATA_SECTION(testSrcBuf, ".l3ram");
#pragma DATA_SECTION(testDstBuf, ".l3ram");

#if (defined(_TMS320C6X) || defined (__TI_ARM_V7M4__))
#pragma DATA_ALIGN (testSrcBuf, TF_CACHE_LINESZ)
#pragma DATA_ALIGN (testDstBuf, TF_CACHE_LINESZ)
uint8_t testSrcBuf[TF_ROUND_UP(TEST_BUF_LENGTH, TF_CACHE_LINESZ)];
uint8_t testDstBuf[TF_ROUND_UP(TEST_BUF_LENGTH, TF_CACHE_LINESZ)];

#else
uint8_t testSrcBuf[TF_ROUND_UP(TEST_BUF_LENGTH, TF_CACHE_LINESZ)]   __attribute__ ((aligned (TF_CACHE_LINESZ)));
uint8_t testDstBuf[TF_ROUND_UP(TEST_BUF_LENGTH, TF_CACHE_LINESZ)]   __attribute__ ((aligned (TF_CACHE_LINESZ)));
#endif

#if defined(_TMS320C6X)
uint32_t gInstanceId = EDMA_DRV_INST_DSS_A;
#else
uint32_t gInstanceId = EDMA_DRV_INST_MSS_A;
#endif

volatile uint32_t gCcErrCnt = 0U, gTcErrCnt = 0U;

#if !defined(BAREMETAL)
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static bool Test_isTransferSuccessful(void)
{
    uint32_t indx;
    bool result = true;

    CacheP_Inv(&testSrcBuf[0], TEST_BUF_LENGTH);
    CacheP_Inv(&testDstBuf[0], TEST_BUF_LENGTH);
    for (indx = 0; indx < TEST_BUF_LENGTH; indx++)
    {
        if (testDstBuf[indx] != testSrcBuf[indx])
        {
            result = false;
            break;
        }
    }
    return(result);
}

static void Test_initializeBuffers(void)
{
    uint32_t indx;
    for (indx = 0; indx < TEST_BUF_LENGTH; indx++)
    {
        testSrcBuf[indx] = indx;
        testDstBuf[indx] = 0;
    }
    CacheP_wb(&testSrcBuf[0], TEST_BUF_LENGTH);
    CacheP_wb(&testDstBuf[0], TEST_BUF_LENGTH);
}

void Test_ErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    gCcErrCnt++;
}

void Test_transferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo)
{
    gTcErrCnt++;
}

static bool Test_instance(uint8_t instanceId)
{
    EDMA_Handle         handle;
    int32_t errorCode;
    EDMA_instanceInfo_t instanceInfo;

    EDMA3CCInitParams initParam;
    EDMA_channelConfig_t config;

    bool isTestPass = false;
    bool isEnableChannel, isTransferDone;
    volatile uint32_t timeout;

    EDMA3CCInitParams_init(&initParam);
    initParam.initParamSet = TRUE;

    /* Initialize the buffers. */
    Test_initializeBuffers();

    /* Initialize the edma instance to be tested */
    errorCode = EDMA_init(instanceId, &initParam);

    if (errorCode == EDMA_NO_ERROR)
    {
        /* Open the first edma Instance */
        handle = EDMA_open(instanceId, &errorCode, &instanceInfo);
        if (handle == NULL)
        {
            EDMA_log("Error: Unable to open the edma Instance, erorCode = %d\n", errorCode);
            isTestPass = false;
        }
    }

    if (errorCode == EDMA_NO_ERROR)
    {
        config.channelId = EDMA_UNTIED_CH0,
        config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
        config.paramId = EDMA_UNTIED_CH0,
        config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH0,
        config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[0]),
        config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[0]),
        config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
        config.paramSetConfig.bCount = 1,
        config.paramSetConfig.cCount = 1,
        config.paramSetConfig.bCountReload = 0,
        config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
        config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
        config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
        config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
        config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
        config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
        config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
        config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
        config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

        /* don't care because of linear addressing modes above */
        config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

        config.paramSetConfig.isStaticSet = false,
        config.paramSetConfig.isEarlyCompletion = false,
        config.paramSetConfig.isFinalTransferInterruptEnabled = true,
        config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
        config.paramSetConfig.isFinalChainingEnabled = false,
        config.paramSetConfig.isIntermediateChainingEnabled = false,
        config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,

        /* This is DMA channel. Donot enbale transfer during channel config. */
        isEnableChannel = false;
        if ((errorCode = EDMA_configChannel(handle, &config, isEnableChannel)) != EDMA_NO_ERROR)
        {
            EDMA_log("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
        }
    }

    if (errorCode == EDMA_NO_ERROR)
    {
        if ((errorCode = EDMA_startTransfer(handle, config.channelId, config.channelType)) != EDMA_NO_ERROR)
        {
            EDMA_log("Error: EDMA_StartTransfer() failed with error code = %d\n", errorCode);
        }
    }

    if (errorCode == EDMA_NO_ERROR)
    {
        isTransferDone = false;
        timeout = TEST_TIMEOUT_CYCLES;

        while ((isTransferDone != true) && (timeout != 0U))
        {
            if ((errorCode = EDMA_isTransferComplete(handle,
                    config.paramSetConfig.transferCompletionCode,
                    (bool *) &isTransferDone)) != EDMA_NO_ERROR)
            {
                EDMA_log("Error: EDMA_isTransferComplete() failed with error code = %d\n", errorCode);
            }
            timeout--;
        }
    }

    if (errorCode == EDMA_NO_ERROR)
    {
        if (timeout == 0)
        {
            EDMA_log("FAIL: Test timed out\n");
        }
        else
        {
            isTestPass = Test_isTransferSuccessful();
        }
    }

    return(isTestPass);
}

#if !defined(BAREMETAL)
void Test_task(void* arg0, void* arg1)
#else
void main (void)
#endif
{
    /* this necessary otherwise optimizer optimizes out if previous instance
       tests fail */
    volatile bool isTestPass;

    char instName[25];
    Board_STATUS boardStatus;
    Board_initCfg boardCfg;

    boardCfg = (BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK |
                BOARD_INIT_UART_STDIO);
    boardStatus = Board_init(boardCfg);

    DebugP_assert(boardStatus == BOARD_SOK);

    EDMA_getInstanceName(gInstanceId, &instName[0], sizeof(instName));
    EDMA_log("Testing EDMA instance #%d: %s\n", gInstanceId, instName);
    isTestPass = Test_instance(gInstanceId);

    EDMA_log("edma test finished\n");
    if (isTestPass == true)
    {
        EDMA_log("All tests have passed.\n");
    }
    else
    {
        EDMA_log("All Tests did NOT Pass\n");
    }

#if !defined(BAREMETAL)
    OS_stop();
#endif
}

#if !defined(BAREMETAL)
void main (void)
{
    TaskP_Params taskParams;

    OS_init();

    /* Initialize the Task Parameters. */
    TaskP_Params_init(&taskParams);
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    TaskP_create(&Test_task, &taskParams);

    OS_start();
    return;
}
#endif
