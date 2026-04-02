/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* BIOS/XDC Include Files. */
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/SemaphoreP.h>

/* ADCBUF Driver: */
#include <ti/drv/adcbuf/adcbuf.h>
#include <ti/drv/edma/edma.h>
#include <ti/osal/DebugP.h>
#include <ti/board/board.h>
#include <ti/csl/soc.h>
#include "ADCBUF_log.h"

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */
static uint8_t  gAppTskStackMain[4 * 1024] __attribute__((aligned(32)));

/* Test definition specific to the device */
#define TOTAL_TEST_CASES                    (4U)
#define TEST_ADCBUF_NUM_SAMPLES             (1024U)
#define TEST_ADCBUF_DMADATA_BASE_ADDRESS    (CSL_DSS_L3_U_BASE + 0x8000U)

#if defined(_TMS320C6X)
#define RSS_ADC_CAPTURE_COMPLETE_IRQ        (57U)
#else
#define RSS_ADC_CAPTURE_COMPLETE_IRQ        (142U)
#endif

#define OSAL_DEFAULT_PRIORITY   (~((uint32_t) 0U))

#define TEST_ADCBUF_EDMA_CHANNEL            (16U)

/* ========================================================================== */
/*                            Structures                                      */
/* ========================================================================== */
typedef struct ADCBUFTestParams_t
{
    /* Number of ADC samples */
    uint32_t    numSamples;

    /* Device specific ADCBUF memory address */
    uint32_t    ADCBUFMemAddr;

    /* Memory address to store ADC samples */
    uint32_t    dstMemAddr;

    /* Chirp Interrupt Number */
    uint32_t    chirpIntNumber;

    /* Enable Edma for copy data */
    uint8_t     enableEdma;
}ADCBUFTestParams;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Device specific test parameters */
ADCBUFTestParams gADCBUFTestParam =
{
    TEST_ADCBUF_NUM_SAMPLES,
	CSL_RSS_ADCBUF_READ_U_BASE,
    TEST_ADCBUF_DMADATA_BASE_ADDRESS,
#if defined(_TMS320C6X)
	RSS_ADC_CAPTURE_COMPLETE_IRQ,
#else
	RSS_ADC_CAPTURE_COMPLETE_IRQ,
#endif
    0
};

/* Test cases with different data format */
ADCBuf_dataFormat dataFmtTestCase_awr2944[TOTAL_TEST_CASES]=
{
    /* Complex, Q+I, non-interleaved */
    {0, 0, 0},

    /* Complex, I+Q, non-interleaved */
    {0, 1, 0},

    /* Real, I, non-interleaved */
    {1, 0, 0},

    /* Real, Q, non-interleaved */
    {1, 1, 0},
};

/* Test Control variables */
volatile uint32_t           gTestRun = 1U;
SemaphoreP_Handle  gIntSemaHandle = NULL;

/* Test statistics */
volatile uint32_t  gIntCounter = 0U;
volatile uint32_t  gVerifyFailCount = 0U;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t Test_ADCBUFApiTest(void);
static int32_t Test_ADCBUFCmdParamsCheckTest(void);
static EDMA_Handle Test_ADCBUFInitEDMA(void);
static void Test_edmaErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo);
static void Test_edmaTransferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo);
static void Test_ADCBUFIntCallBackFunc(uintptr_t arg);
static void Test_ADCBUFContDataPathWithTestPattern(ADCBuf_Handle handle, ADCBuf_dataFormat *ptrDataFormat);
static void Test_ADCBUFContModeConfig(ADCBuf_Handle handle, ADCBuf_dataFormat *ptrDataFormat);
static int32_t Test_ADCBUFConfigEdma(EDMA_Handle handle, uint8_t chId, uint32_t* pSrcAddress, uint32_t * pDestAddress,
                                     uint16_t aCount, uint16_t bCount, uint16_t cCount,uint16_t linkChId);
static int32_t Test_ADCBUFEDMA_setup_shadow_link (EDMA_Handle handle, uint32_t chId, uint32_t linkChId,
                                                  EDMA_paramSetConfig_t *config,
												  EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn);
static uint32_t Test_ADCBUFVerifyDataPattern(void *memAddr, ADCBuf_dataFormat *ptrDataFormat);
static void Test_ADCBUFCloseEDMA(EDMA_Handle handle);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/**
 *  @b Description
 *  @n
 *      Call back function for EDMA CC (Channel controller) error as per EDMA API.
 *      Declare fatal error if happens, the output errorInfo can be examined if code
 *      gets trapped here.
 */
void Test_edmaErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    DebugP_assert(0);
}

/**
 *  @b Description
 *  @n
 *      Call back function for EDMA transfer controller error as per EDMA API.
 *      Declare fatal error if happens, the output errorInfo can be examined if code
 *      gets trapped here.
 */
void Test_edmaTransferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo)
{
    DebugP_assert(0);
}

/**
 *  @b Description
 *  @n
 *      Interrupt handler callback for chirp available. It runs in the ISR context.
 *
 *  @param[in]  handle
 *      ADCBUF Instance Handle
 *
 *  @retval
 *      Not Applicable.
 */
void Test_ADCBUFIntCallBackFunc(uintptr_t arg)
{
    ADCBuf_Handle     handle;

    handle = (ADCBuf_Handle)arg;

    /* Increment interrupt counter for debugging purpose */
    gIntCounter++;

    /* Check the buffer every 10 chirp avail interrupt */
    if(gIntCounter % 10 == 0)
    {
        /* Stop Continuous mode */
        ADCBuf_control(handle, ADCBufMMWave_CMD_STOP_CONTINUOUS_MODE, NULL);

        /* Stop Test Pattern generation */
        ADCBuf_control(handle, ADCBufMMWave_CMD_STOP_TEST_PATTERN, NULL);

        /* Semaphore post for test thread to copy samples and verify */
        SemaphoreP_post(gIntSemaHandle);
    }
}

/**
 *  @b Description
 *  @n
 *      Continuous mode with Test pattern generation test.
 *
 *  @param[in]  handle
 *      ADCBUF Instance Handle
 *  @param[in]  ptrDataFormat
 *      Pointer to the ADCBuf Data format configuration
 *
 *  @retval
 *      Not Applicable.
 */
void Test_ADCBUFContDataPathWithTestPattern(ADCBuf_Handle handle, ADCBuf_dataFormat *ptrDataFormat)
{
    int32_t retVal = 0;
    uint32_t numOfClks = 0x32;

    /* Configure test pattern */
    Test_ADCBUFContModeConfig(handle, ptrDataFormat);

    /* Start Test Pattern generation */
    if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_START_TEST_PATTERN, (void *)&numOfClks)) < 0)
       ADCBUF_log("Error: ADCBufMMWave_CMD_START_TEST_PATTERN failed with [Error=%d]\n", retVal);
}

/**
 *  @b Description
 *  @n
 *      Continuous mode with Test patern generation test.
 *
 *  @param[in]  handle
 *      ADCBUF Instance Handle
 *  @param[in]  ptrDataFormat
 *      Pointer to the ADCBuf Data format configuration
 *
 *  @retval
 *      Not Applicable.
 */
void Test_ADCBUFContModeConfig(ADCBuf_Handle handle, ADCBuf_dataFormat *ptrDataFormat)
{
    uint32_t                 arg;
    ADCBuf_RxChanConf        rxChanConf;
    ADCBuf_TestPatternConf   testPatternConf;
    uint8_t                  channel;
    uint16_t                 offset = 0;
    uint32_t                 numSamples;
    int32_t                  retVal = 0;

    /* Configure ADC buffer data format */
    if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_CONF_DATA_FORMAT, (void *)ptrDataFormat)) < 0)
        ADCBUF_log("Error: ADCBufMMWave_CMD_CONF_DATA_FORMAT failed with [Error=%d]\n", retVal);

    /* Enable all 4  Rx Channel */
    for (channel=0; channel < NUM_RX_CHANNEL; channel++)
    {
        rxChanConf.channel = channel;
        rxChanConf.offset    = offset;
        if((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_CHANNEL_ENABLE, (void *)&rxChanConf)) < 0)
            ADCBUF_log("Error: ADCBufMMWave_CMD_CHANNEL_ENABLE failed for channel %d offset =0x%x with [Error = %d]\n",
                                     channel, offset, retVal);
        /* Test purpose only : Verify channel address */
        {
            uint32_t channelAddr = 0;

            if ( (channelAddr = ADCBuf_getChanBufAddr(handle, channel, &retVal)) != 0)
            {
                channelAddr -= gADCBUFTestParam.ADCBUFMemAddr;

                if(channelAddr != offset)
                {
                    ADCBUF_log("Error: ADCBuf_getChanBufAddr() return mismatched channel(%d) buffer address [%x: %x]\n",
                                 channel, offset, channelAddr);

                    retVal = -1;
                }
            }
            else
            {
                ADCBUF_log("Error: ADCBuf_getChanBufAddr failed for channel %d with [Error = %d]\n",
                                         channel, retVal);
            }
        }

        offset += gADCBUFTestParam.numSamples * 4;
    }

    /* Configure ADC buffer in continuous mode */
    arg = 1;
    if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_SET_CONTINUOUS_MODE, (void *)&arg)) < 0)
        ADCBUF_log("Error: ADCBufMMWave_CMD_SET_CONTINUOUS_MODE failed with [Error=%d]\n", retVal);

    /* Start the continuous streaming mode in ADCBUFF */
    numSamples = gADCBUFTestParam.numSamples;
    if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_START_CONTINUOUS_MODE, (void *)&numSamples)) < 0)
        ADCBUF_log("Error: ADCBufMMWave_CMD_START_CONTINUOUS_MODE failed with [Error=%d]\n", retVal);

    /* Configure Test Pattern generation */
    testPatternConf.period = 255;
    testPatternConf.numSamples = gADCBUFTestParam.numSamples;

    for (channel=0; channel < 4; channel++)
    {
        testPatternConf.rxConfig[channel].rxIOffset   = 0x0000;
        testPatternConf.rxConfig[channel].rxIInc      = 2;
        testPatternConf.rxConfig[channel].rxQOffset   = 0x1000 ;
        testPatternConf.rxConfig[channel].rxQInc      = 2;
    }

    /* Send control command to driver */
    if ( (retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_CONF_TEST_PATTERN, (void *)&testPatternConf)) < 0)
        ADCBUF_log("Error: ADCBufMMWave_CMD_CONF_TEST_PATTERN failed with [Error=%d]\n", retVal);

}

/**
 *  @b Description
 *  @n
 *      Config EDMA channel for data buffer copy.
 *
 *  @param[in]  handle
 *      EDMA driver instance Handle
 *  @param[in]  chId
 *      EDMA Channle id used to copy data buffer
 *  @param[in]  pSrcAddress
 *      Source address
 *  @param[in]  pDestAddress
 *      Destination address
 *  @param[in]  aCount
 *      EDMA channel Acount configuration
 *  @param[in]  bCount
 *      EDMA channel Bcount configuration
 *  @param[in]  cCount
 *      EDMA channel Ccount configuration
 *  @param[in]  linkChId
 *      EDMA link Id
 *
 *  @retval
 *      Success    - 0
 *      Fail       < 0 (ErrorCode)
 */
int32_t Test_ADCBUFConfigEdma(EDMA_Handle handle, uint8_t chId,
                                    uint32_t* pSrcAddress, uint32_t * pDestAddress,
                                    uint16_t aCount, uint16_t bCount, uint16_t cCount,
                                    uint16_t linkChId)
{
    EDMA_channelConfig_t config;
    int32_t errorCode = EDMA_NO_ERROR;

    config.channelId = chId;
    config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    config.paramId = chId;
    config.eventQueueId = 0;

    config.paramSetConfig.sourceAddress = (uint32_t) pSrcAddress;
    config.paramSetConfig.destinationAddress = (uint32_t) pDestAddress;

    config.paramSetConfig.aCount = aCount;
    config.paramSetConfig.bCount = bCount;
    config.paramSetConfig.cCount = cCount;
    config.paramSetConfig.bCountReload = config.paramSetConfig.bCount;

    config.paramSetConfig.sourceBindex = 0;
    config.paramSetConfig.destinationBindex = 0;

    config.paramSetConfig.sourceCindex = 0;
    config.paramSetConfig.destinationCindex = 0;

    config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A;
    config.paramSetConfig.transferCompletionCode = 0;
    config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;
    config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;

    /* don't care because of linear addressing modes above */
    config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT;

    config.paramSetConfig.isStaticSet = false;
    config.paramSetConfig.isEarlyCompletion = false;
    config.paramSetConfig.isFinalTransferInterruptEnabled = true;
    config.paramSetConfig.isIntermediateTransferInterruptEnabled = false;
    config.paramSetConfig.isFinalChainingEnabled = false;
    config.paramSetConfig.isIntermediateChainingEnabled = false;
    config.transferCompletionCallbackFxn = NULL;

    if ((errorCode = EDMA_configChannel(handle, &config, false)) != EDMA_NO_ERROR)
    {
        ADCBUF_log("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
        goto Exit;
    }

    errorCode = Test_ADCBUFEDMA_setup_shadow_link(handle, chId, linkChId,
        &config.paramSetConfig, config.transferCompletionCallbackFxn);

Exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      Setup shadow link for EDMA copy.
 *
 *  @param[in]  handle
 *      EDMA driver instance Handle
 *  @param[in]  chId
 *      EDMA Channle id used to copy data buffer
 *  @param[in]  linkChId
 *      EDMA link id used to copy data buffer
 *  @param[in]  config
 *      EDMA param set used to copy data buffer
 *  @param[in]  transferCompletionCallbackFxn
 *      Callback function when data copy is completed
 *
 *  @retval
 *      Success    - 0
 *      Fail       < 0 (ErrorCode)
 */
static int32_t Test_ADCBUFEDMA_setup_shadow_link (EDMA_Handle handle, uint32_t chId, uint32_t linkChId,
    EDMA_paramSetConfig_t *config, EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn)
{
    EDMA_paramConfig_t paramConfig;
    int32_t errorCode = EDMA_NO_ERROR;

    paramConfig.paramSetConfig = *config;
    paramConfig.transferCompletionCallbackFxn = transferCompletionCallbackFxn;
    paramConfig.transferCompletionCallbackFxnArg = (uintptr_t) handle;
    if ((errorCode = EDMA_configParamSet(handle, linkChId, &paramConfig)) != EDMA_NO_ERROR)
    {
        ADCBUF_log("Error: EDMA_configParamSet() failed with error code = %d\n", errorCode);
        goto exit;
    }

    if ((errorCode = EDMA_linkParamSets(handle, chId, linkChId)) != EDMA_NO_ERROR)
    {
        ADCBUF_log("Error: EDMA_linkParamSets() failed with error code = %d\n", errorCode);
        goto exit;
    }

    if ((errorCode = EDMA_linkParamSets(handle, linkChId, linkChId)) != EDMA_NO_ERROR)
    {
        ADCBUF_log("Error: EDMA_linkParamSets() failed with error code = %d\n", errorCode);
        goto exit;
    }


exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      Verify the test data pattern .
 *
 *  @param[in]  memAddr
 *      Memory base address that holds the data copied from ADC buffer
 *  @param[in]  ptrDataFormat
 *      Pointer to the ADCBuf Data format configuration
 *
 *  @retval
 *      Not Applicable.
 */
uint32_t Test_ADCBUFVerifyDataPattern(void *memAddr, ADCBuf_dataFormat *ptrDataFormat)
{
    volatile uint32_t*   ptrDataAddress;
    uint8_t              channel;
    uint32_t             sampleIdx;
    volatile uint32_t    addrOffset;
    uint32_t             data, firstSample;
    uint16_t             idata, qdata;
    uint32_t             incr = 2;
    uint32_t             failFlag = 0;
    uint32_t             dataIncr;
    uint32_t             numSamples;

    ptrDataAddress = (volatile uint32_t *)memAddr;
    ADCBUF_log("Debug: First 4 32 bit words 0x%x     0x%x    0x%x    0x%x\n", ptrDataAddress[0],
                        ptrDataAddress[1], ptrDataAddress[2], ptrDataAddress[3]);

    /* Set the address offset */
    addrOffset = 0;

    /* Get number of samples from test params */
    numSamples = gADCBUFTestParam.numSamples;

    /* Get the first sample */
    firstSample = *ptrDataAddress;

    /* Channel interleaved-mode */
    if (ptrDataFormat->channelInterleave == 0)
    {
        /* Test 10 samples for all channels */
        for (sampleIdx=0; sampleIdx < numSamples; sampleIdx++)
        {
            for (channel = 0; channel < 4; channel++)
            {
                addrOffset = sampleIdx * 4;
                if(ptrDataFormat->adcOutFormat == 0)
                {
                    data = ptrDataAddress[addrOffset + channel];

                    /* Complex data format, get I and Q data */
                    if(ptrDataFormat->sampleInterleave == 0)
                    {
                        qdata = (data & 0xffff0000) >> 16;
                        idata  = (data & 0xffff);
                    }
                    else
                    {
                        idata = (data & 0xffff0000) >> 16;
                        qdata  = (data & 0xffff);
                    }

                    /* Verify data 1: increment = 2, */
                    dataIncr =( ( sampleIdx * incr)<<16) + (incr * sampleIdx);
                    if((data - firstSample) != dataIncr)
                    {
                         failFlag = 1;
                    }

                    /* Verify data 2: Q = I + 0x1000 */
                    if( (idata + 0x1000) != qdata)
                    {
                         failFlag = 1;
                    }
                }
                else
                {
                    uint16_t *ptrRealData = (uint16_t *)ptrDataAddress;
                    uint16_t  realData;
                    uint16_t  firstRealSample;

                    /* Address offset in terms of 16bits real data */
                    addrOffset = sampleIdx * 4;
                    realData = ptrRealData[addrOffset + channel];
                    firstRealSample = ptrRealData[channel];

                    /* Verify data 1: increment = 2, */
                    dataIncr =incr * sampleIdx;
                    if((realData - firstRealSample) != dataIncr)
                    {
                         failFlag = 1;
                    }
                }
            }
        }

    }
    else
    {
        for (channel = 0; channel < 4; channel++)
        {
            addrOffset = (channel * numSamples) ;
            firstSample = ptrDataAddress[addrOffset];

            for (sampleIdx=0; sampleIdx<numSamples; sampleIdx++)
            {
                data = ptrDataAddress[addrOffset + sampleIdx];

                if(ptrDataFormat->adcOutFormat == 0)
                {
                    /* Complex data format, get I and Q data */
                    if(ptrDataFormat->sampleInterleave == 0)
                    {
                        qdata = (data & 0xffff0000) >> 16;
                        idata  = (data & 0xffff);
                    }
                    else
                    {
                        idata = (data & 0xffff0000) >> 16;
                        qdata  = (data & 0xffff);
                    }

                    /* Verify data 1: increment = 2, */
                    dataIncr =( ( sampleIdx * incr)<<16) + (incr * sampleIdx);
                    if((data - firstSample) != dataIncr)
                    {
                         failFlag = 1;
                    }

                    /* Verify data 2: Q = I + 0x1000 */
                    if( (idata + 0x1000) != qdata)
                    {
                         failFlag = 1;
                    }
                }
                else
                {
                    volatile uint16_t *ptrRealData = (uint16_t *)ptrDataAddress;
                    uint16_t  realData;
                    uint16_t  firstRealSample;

                    /* Address offset in terms of 16bits real data */
                    addrOffset = (channel * numSamples * 2) ;
                    realData = ptrRealData[addrOffset + sampleIdx];
                    firstRealSample = ptrRealData[addrOffset];

                    /* Verify data 1: increment = 2, */
                    dataIncr =incr * sampleIdx;
                    if((realData - firstRealSample) != dataIncr)
                    {
                         failFlag = 1;
                    }
                }
            }

        }
    }
    return ((failFlag == 0) ? 0 : 1);
}

/**
 *  @b Description
 *  @n
 *      Close EDMA driver instance
 *
 *  @param[in]  handle
 *      EDMA driver instance Handle
 *
 *  @retval
 *      Not applicable
 */
void Test_ADCBUFCloseEDMA(EDMA_Handle handle)
{
    int32_t    retVal = 0;

    if ((retVal = EDMA_close(handle)) != EDMA_NO_ERROR)
    {
        ADCBUF_log("Debug: edma Instance %p closing error, errorCode = %d\n", handle, retVal);
    }
}


/**
 *  @b Description
 *  @n
 *      ADCBuf driver API test.
 *
 *  @retval
 *      =0       Test passed
 *      <0       Test Failed
 */
static int32_t Test_ADCBUFApiTest(void)
{
    ADCBuf_Params        params;
    ADCBuf_Handle        handle;
    int32_t              retVal = 0;

    /* Setup the default ADCBUF Parameters */
    ADCBuf_Params_init(&params);

    /* Open the first ADCBUF Instance */
    handle = ADCBuf_open(0, &params);
    if (handle == NULL)
    {
        ADCBUF_log("Error: Unable to open the ADCBUF Instance\n");
        retVal = -1;
        goto Exit;
    }
    ADCBUF_log("Debug: ADCBUF Instance(0) %p has been opened successfully\n", handle);

    /**************************************************************************
     * Test: Graceful shutdown
     **************************************************************************/
    ADCBuf_close(handle);
    ADCBUF_log("Debug: ADCBUF Instance %p has been closed successfully\n", handle);

Exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      ADCBuf driver test to validate command parameters.
 *
 *  @retval
 *      =0       Test passed
 *      <0       Test Failed
 */
static int32_t Test_ADCBUFCmdParamsCheckTest(void)
{
    ADCBuf_Params        params;
    ADCBuf_Handle        handle;
    uint32_t             arg;
    int32_t              retVal = 0;
    ADCBufMMWave_CMD     command;
    ADCBuf_dataFormat    dataFormat;
    ADCBuf_RxChanConf    rxChanConf;
    ADCBuf_CQConf        cqConf;
    int32_t              testResult = 0;

    /* Setup the default ADCBUF Parameters */
    ADCBuf_Params_init(&params);

    /* Open the first ADCBUF Instance */
    handle = ADCBuf_open(0, &params);
    if (handle == NULL)
    {
        ADCBUF_log("Error: Unable to open the ADCBUF Instance\n");
        testResult = -1;
        goto Exit;
    }
    ADCBUF_log("Debug: ADCBUF Instance(0) %p has been opened successfully\n", handle);

    /* Params check Test */
    command = ADCBufMMWave_CMD_SET_SRC;
    arg = 0x5;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with arg=%d\n", command, arg);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with arg=%d\n", command, arg);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_SET_CONTINUOUS_MODE;
    arg = 0x8;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with arg=%d\n", command, arg);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with arg=%d\n", command, arg);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CONF_DATA_FORMAT;
    memset ((void *)&dataFormat, 0, sizeof(dataFormat));
    dataFormat.adcOutFormat = 2;
    if ((retVal = ADCBuf_control(handle, command, (void *)&dataFormat)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with adcOutFormat=%d \n", command, dataFormat.adcOutFormat);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with adcOutFormat=%d\n", command, dataFormat.adcOutFormat);
        testResult = -1;
    }

    memset ((void *)&dataFormat, 0, sizeof(dataFormat));
    dataFormat.sampleInterleave = 2;
    if ((retVal = ADCBuf_control(handle, command, (void *)&dataFormat)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with sampleInterleave=%d\n", command, dataFormat.sampleInterleave);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with sampleInterleave=%d\n", command, dataFormat.sampleInterleave);
        testResult = -1;
    }

    memset ((void *)&dataFormat, 0, sizeof(dataFormat));
    dataFormat.channelInterleave = 2;
    if ((retVal = ADCBuf_control(handle, command, (void *)&dataFormat)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d channelInterleave=%d\n", command, dataFormat.channelInterleave);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with channelInterleave=%d\n", command, dataFormat.channelInterleave);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CONF_CQ;
    memset ((void *)&cqConf, 0, sizeof(cqConf));
    cqConf.cqDataWidth= 5;
    if ((retVal = ADCBuf_control(handle, command, (void *)&cqConf)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with cqDataWidth=%d \n", command, cqConf.cqDataWidth);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with cqDataWidth=%d\n", command, cqConf.cqDataWidth);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CONF_CQ;
    memset ((void *)&cqConf, 0, sizeof(cqConf));
    cqConf.cq96BitPackEn = 4;
    if ((retVal = ADCBuf_control(handle, command, (void *)&cqConf)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with cq96BitPackEn=%d \n", command, cqConf.cqDataWidth);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with cq96BitPackEn=%d\n", command, cqConf.cq96BitPackEn);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_START_CONTINUOUS_MODE;
    arg = 0x1U<<16;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d \n", command);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with arg=%d\n", command, arg);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CHANNEL_ENABLE;
    memset ((void *)&rxChanConf, 0, sizeof(rxChanConf));
    rxChanConf.channel = 6;
    if ((retVal = ADCBuf_control(handle, command, (void *)&rxChanConf)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with channel = %d\n", command, rxChanConf.channel);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with channel=%d\n", command, rxChanConf.channel);
        testResult = -1;
    }

    memset ((void *)&rxChanConf, 0, sizeof(rxChanConf));
    rxChanConf.channel = 2;
    rxChanConf.offset = 0x1U << 15U;
    if ((retVal = ADCBuf_control(handle, command, (void *)&rxChanConf)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with offset = %d\n", command, rxChanConf.offset);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with offset=%d\n", command, rxChanConf.offset);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD;
    arg = 35;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with arg = %d , retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD;
    arg = 35;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD;
    arg = 35;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CHANNEL_DISABLE;
    arg = 0xc;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
    {
        ADCBUF_log("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }
    else
    {
        ADCBUF_log("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    }

    command = ADCBufMMWave_CMD_CHANNEL_DISABLE;
    arg = 0x1c;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CHANNEL_DISABLE;
    arg = 0x0;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        ADCBUF_log("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        ADCBUF_log("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    /* Close ADCbuf driver */
    ADCBuf_close(handle);
    ADCBUF_log("Debug: ADCBUF Instance %p has been closed successfully\n", handle);

Exit:
    return testResult;
}

/**
 *  @b Description
 *  @n
 *      Init EDMA driver for data buffer copy.
 *
 *  @retval
 *      Success     - EDMA handle
 *      Fail        - NULL pointer
 */
EDMA_Handle Test_ADCBUFInitEDMA(void)
{
    EDMA_errorInfo_t  EDMAErrorInfo;
    EDMA_transferControllerErrorInfo_t EDMATransferControllerErrorInfo;
    EDMA_Handle         EdmaHandle = NULL;
    EDMA_instanceInfo_t instanceInfo;
    EDMA_errorConfig_t errorConfig;

    /*******************/
    EDMA3CCInitParams initParam;
    int8_t gInstanceId = EDMA_DRV_INST_DSS_A;

    int32_t    retVal = 0;

    EDMA3CCInitParams_init(&initParam);
    initParam.initParamSet = TRUE;

    retVal = EDMA_init(gInstanceId, &initParam);
    if (retVal != EDMA_NO_ERROR)
    {
        ADCBUF_log ("Debug: EDMA instance 0 initialization returned error %d\n", retVal);
        return NULL;
    }
    ADCBUF_log ("Debug: EDMA instance 0 has been initialized\n");

    memset(&EDMAErrorInfo, 0, sizeof(EDMAErrorInfo));
    memset(&EDMATransferControllerErrorInfo, 0, sizeof(EDMATransferControllerErrorInfo));

    /* Open the first EDMA Instance */
    EdmaHandle = EDMA_open(gInstanceId, &retVal, &instanceInfo);
    if (EdmaHandle == NULL)
    {
        ADCBUF_log("Error: Unable to open the edma Instance, erorCode = %d\n", retVal);
    }

    /* Configure EDMA Error Monitor */
    errorConfig.isConfigAllEventQueues = true;
    errorConfig.isConfigAllTransferControllers = true;
    errorConfig.isEventQueueThresholdingEnabled = true;
    errorConfig.eventQueueThreshold = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
    errorConfig.isEnableAllTransferControllerErrors = true;
    errorConfig.callbackFxn = Test_edmaErrorCallbackFxn;
    errorConfig.transferControllerCallbackFxn = Test_edmaTransferControllerErrorCallbackFxn;
    if ((retVal = EDMA_configErrorMonitoring(EdmaHandle, &errorConfig)) != EDMA_NO_ERROR)
    {
        ADCBUF_log("Debug: EDMA_configErrorMonitoring() failed with errorCode = %d\n", retVal);
        return NULL;
    }

    return EdmaHandle;
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
void Test_ADCBUFInitTask(void* arg0, void* arg1)
{
	EDMA_Handle          EdmaHandle = NULL;
	SemaphoreP_Params    semParams;
	ADCBuf_Params        params;
	ADCBuf_Handle        handle;
	ADCBuf_dataFormat    dataFormat;
	bool                 edmaTransComplete ;
	uint8_t              index;
    volatile uint32_t    srcMemAddr;
    volatile uint32_t    dstMemAddr;
    uint32_t             numSamples;
    ADCBuf_dataFormat    *ptrDataFormat = &dataFormat;
    int32_t              retVal = 0;

    OsalRegisterIntrParams_t    intrPrms;
    HwiP_Handle                 hwiHandle = NULL;
    uint32_t                    intrPriority;

    ADCBUF_log("\n/*******************************************************************/ \n");
    ADCBUF_log("/************************* ADCBUF Test *****************************/ \n");
    ADCBUF_log("/*******************************************************************/ \n");

    /* Initialize the ADCBUF */
    ADCBuf_init();

    if(gADCBUFTestParam.enableEdma)
    {
        EdmaHandle = Test_ADCBUFInitEDMA();
        if(EdmaHandle == NULL)
        {
            ADCBUF_log("Debug: EDMA Init failed \n");
            goto Exit;
        }
    }

    /**************************************************************************
     * Test: Basic API test
     **************************************************************************/

    /* Basic API Test */
    if(Test_ADCBUFApiTest() < 0)
        ADCBUF_log ("ADCBUF Basic API test failed\n");
    else
        ADCBUF_log ("ADCBUF Basic API test passed\n");

    /**************************************************************************
     * Test: Parameter validation test
     **************************************************************************/

    /* Test input parameters */
    if (Test_ADCBUFCmdParamsCheckTest() < 0)
        ADCBUF_log ("ADCBUF Input Parameters Validation Test failed\n");
    else
        ADCBUF_log ("ADCBUF Input Parameters Validation Test passed\n");

    /**************************************************************************
     * Test: Reopen the driver to test in Continuous Mode
     **************************************************************************/

    /* YES: Create a binary semaphore which is used to handle interrupt handling. */
    SemaphoreP_Params_init(&semParams);
    semParams.mode             = SemaphoreP_Mode_BINARY;
    gIntSemaHandle             = SemaphoreP_create(0, &semParams);

    /* Setup the default ADCBUF Parameters */
    ADCBuf_Params_init(&params);
    params.continousMode = 1;

    /* Go through different data format test cases */
    for (index = 0; index < TOTAL_TEST_CASES; index++)
    {
        handle = ADCBuf_open(0, &params);
        if (handle == NULL)
        {
            ADCBUF_log("Error: Unable to open the ADCBUF Instance\n");
            return;
        }
        ADCBUF_log("Debug: ADCBUF Instance %p has been reopened successfully\n", handle);

        /* Register chirp interrupt */
        #if defined(_TMS320C6X)
        intrPriority = OSAL_DEFAULT_PRIORITY;
        #else
        intrPriority = 0x1U;
        #endif

        Osal_RegisterInterrupt_initParams(&intrPrms);

        /* Populate the interrupt parameters */
        intrPrms.corepacConfig.arg              = (uintptr_t) handle;
        intrPrms.corepacConfig.isrRoutine       = Test_ADCBUFIntCallBackFunc;
        intrPrms.corepacConfig.priority         = intrPriority;
        intrPrms.corepacConfig.name             = (char *)"ADC_CAPTURE_COMPLETE";

        #if defined (_TMS320C6X)
        /* On C66x, we use Event Combiner to map the interrupt to the CPU Intc.  To
         * do this, OSAL expects that event number holds the interrupt number and we
         * use the macro for interrupt number to specify we wish to use Event
         * Combiner.
         */
        intrPrms.corepacConfig.corepacEventNum  = (int32_t)gADCBUFTestParam.chirpIntNumber;
        intrPrms.corepacConfig.intVecNum        = OSAL_REGINT_INTVEC_EVENT_COMBINER;
        #else
        /* Other (non-C66x) CPUs don't use event number and interrupt number is
         * passed in and programmed to CPU Intc directly.
         */
        intrPrms.corepacConfig.corepacEventNum  = (int32_t)0;
        intrPrms.corepacConfig.intVecNum        = (int32_t)gADCBUFTestParam.chirpIntNumber;
        #endif

        /* Register interrupts */
        if(OSAL_INT_SUCCESS != Osal_RegisterInterrupt(&intrPrms, &hwiHandle))
        {
            ADCBUF_log("Error: Unable to register Chirp interrupt\n");
            hwiHandle = NULL;
            goto Exit;
        }

        /* Initialize the data format for the test */
        dataFormat = dataFmtTestCase_awr2944[index];

        /* Print out the test params */
        ADCBUF_log("ADCBUF Test Pattern with dataFormat=%d, sampleSwap=%d, interleave=%d \n",
                       dataFormat.adcOutFormat, dataFormat.sampleInterleave,
					   dataFormat.channelInterleave);

        /* Number of loops that test run for current data Format
            Data is validated for every 10 ping/pong buffer swap.
          */
        gTestRun = 20;

        /* ADCBuf Test Pattern Test, this function will be blocked until gTestRun is set to 0 */
        Test_ADCBUFContDataPathWithTestPattern(handle, &dataFormat);

        /* Assign memory base address to save the ADC samples */
        dstMemAddr = CSL_locToGlobAddr(gADCBUFTestParam.dstMemAddr);
        srcMemAddr = CSL_locToGlobAddr(gADCBUFTestParam.ADCBUFMemAddr);
        numSamples = gADCBUFTestParam.numSamples;

        while(gTestRun)
        {
            /* Wait for the Semaphore */
            SemaphoreP_pend(gIntSemaHandle, SemaphoreP_WAIT_FOREVER);


            if(gADCBUFTestParam.enableEdma)
            {
                edmaTransComplete = false;
                /* Trigger DMA copy */
                Test_ADCBUFConfigEdma(EdmaHandle, TEST_ADCBUF_EDMA_CHANNEL,
                                      (uint32_t *)srcMemAddr, (uint32_t *)dstMemAddr,
                                      16 * numSamples, 1, 1, EDMA_NUM_DMA_CHANNELS);

                EDMA_startDmaTransfer(EdmaHandle, TEST_ADCBUF_EDMA_CHANNEL);

                /* Polling transfer completion */
                while(edmaTransComplete == false)
                {
                    EDMA_isTransferComplete(EdmaHandle, 0, &edmaTransComplete);
                    TaskP_sleepInMsecs(1);
                }
            }
            else
            {
                /* Copy the adcbuf data to memory */
                memcpy( (void *)(dstMemAddr),  (void *)(srcMemAddr),  16 * numSamples );

                CacheP_wbInv ((const void*)dstMemAddr, 16 * numSamples);
            }

            CacheP_Inv((const void*)srcMemAddr, (16 * numSamples));
            CacheP_Inv((const void*)dstMemAddr, (16 * numSamples));

            /* Verify data integrity */
            if (Test_ADCBUFVerifyDataPattern((void *)dstMemAddr, ptrDataFormat) !=0 )
            {
                gVerifyFailCount++;
            }

            gTestRun--;

            /* Re-Start Test Pattern generation if test is not stopped */
            if(gTestRun)
            {
                uint32_t  rxChanMask;

                /* Disable all channels */
                rxChanMask = 0xF;
                if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_CHANNEL_DISABLE, (void *)&rxChanMask)) < 0)
                {
                   ADCBUF_log("Error: ADCBufMMWave_CMD_CHANNEL_DISABLE failed with [Error=%d]\n", retVal);
                }

                /* ADCBuf Test Pattern Test, this function will be blocked until gTestRun is set to 0 */
                Test_ADCBUFContDataPathWithTestPattern(handle, &dataFormat);
            }
        }

        /* StopTest Pattern generation */
        if (( retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_STOP_TEST_PATTERN, NULL)) < 0)
           ADCBUF_log("Error: ADCBufMMWave_CMD_STOP_TEST_PATTERN failed with [Error=%d]\n", retVal);

        /* Close  ADCBuf driver */
        ADCBuf_close(handle);
        ADCBUF_log("Debug: ADCBUF Instance %p has been closed successfully\n", handle);

        /* Deregister chirp interrupt listener */
        #if defined (_TMS3206X)
        corepacEventNum = (int32_t)gADCBUFTestParam.chirpIntNumber;
        Osal_DisableInterrupt(corepacEventNum, (int32_t)OSAL_REGINT_INTVEC_EVENT_COMBINER);
        #else
        Osal_DisableInterrupt(0, (int32_t)gADCBUFTestParam.chirpIntNumber);
        #endif

        #if defined (_TMS320C6x)
        corepacEventNum = (int32_t)coreIntrNum;
        #endif

        /* Delete interrupts */
        #if defined (_TMS3206X)
        corepacEventNum = (int32_t)gADCBUFTestParam.chirpIntNumber;
        (void) Osal_DeleteInterrupt((HwiP_Handle) hwiHandle, corepacEventNum);
        #else
        (void) Osal_DeleteInterrupt((HwiP_Handle) hwiHandle, gADCBUFTestParam.chirpIntNumber);

        #endif

        /* Send test results to logger */
        if (gVerifyFailCount > 0)
        {
            ADCBUF_log ("ADCBUF Test Pattern failed\n");
            ADCBUF_log ("gVerifyFailCount = %d\n", gVerifyFailCount);
        }
        else
            ADCBUF_log ("ADCBUF Test Pattern passed\n");

        /* Clear the counter */
        gIntCounter = 0;
        gVerifyFailCount = 0;

    }

    /**************************************************************************
     * Test: Graceful shutdown
     **************************************************************************/
    /* Close EDMA driver */
    if(EdmaHandle != NULL)
        Test_ADCBUFCloseEDMA(EdmaHandle);
    /* Delete semaphore */
    SemaphoreP_delete(gIntSemaHandle);
    ADCBUF_log("Debug: Semaphore Instance %p has been closed successfully\n", gIntSemaHandle);

Exit:
    /* Stop OS */
    OS_stop();

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    TaskP_Params    taskParams;
    Board_STATUS boardStatus;
    Board_initCfg boardCfg;

    boardCfg = (BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK | 
                BOARD_INIT_UART_STDIO);
    boardStatus = Board_init(boardCfg);

    DebugP_assert(boardStatus == BOARD_SOK);

    OS_init();

    /* Initialize the Task Parameters. */
    TaskP_Params_init(&taskParams);
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof(gAppTskStackMain);
    TaskP_create(&Test_ADCBUFInitTask, &taskParams);

    /* Start OS */
    OS_start();

    return 0;
}
