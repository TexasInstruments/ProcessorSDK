/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the HWA
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2019 - 2020 Texas Instruments, Inc.
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
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "hwa_log.h"

/* mmWave SK Include Files: */
#include <ti/osal/osal.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/CycleprofilerP.h>
#include <ti/osal/TaskP.h>
#include <ti/drv/hwa/hwa.h>
#include <ti/drv/hwa/soc/hwa_soc.h>
#include <ti/board/board_cfg.h>
#include <ti/board/board.h>

/* input vector */
#include "hwa_testvector_input.h"
#include "hwa_testvector_output.h"
#include "fft_window.h"

#define APP_TSK_STACK_MAIN              (2U * 1024U)

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/**************************************************************************
 *************************** Global Definitions ****************************
 **************************************************************************/

static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
// MCPI_LOGBUF_INIT(9216);

/* using this as global to facilitate debugging */
HWA_ParamConfig gHWATestParamConfig[16];
HWA_CommonConfig gCommonConfig;

uint32_t gHWATestParamSetISR = 0;
volatile uint32_t gHWATestDoneISR ;
volatile uint32_t gHWAcontextswitchTestParamSetIdx[90 + 7];
volatile uint8_t gHWAcontextswitchTestParamIdx = 0;
volatile uint8_t gHWAOneLoopDone = 0;
volatile uint8_t gHWAALTThreadDone = 0; //ALT thread done interrupt flag,
volatile uint8_t gHWAcontextswitchParamDone = 0; //flag for every paramset done interrupt
volatile uint8_t intr1ParamDoneCount = 0;
volatile uint8_t intr2ParamDoneCount = 0;
//volatile uint32_t contextswitchTestStartTime;


/* ADC data charateristics */
#define HWA_TEST_NUM_SAMPLES     225
#define HWA_TEST_1DFFT_SIZE      256
#define HWA_TEST_NUM_RX_ANT         4
#define HWA_TEST_COMPLEX_16BIT_SIZE 4
#define HWA_TEST_COMPLEX_32BIT_SIZE 8
/* Input data characteristics */
#define HWA_TEST_IS_INPUT_REAL            0
#define HWA_TEST_IS_INPUT_SIGNED          1
/* window RAM */
#define HWA_TEST_1DFFT_WINDOW_START       0
/* DMA channel */
#define HWA_TEST_SRC_TRIGGER_DMACH0       0
#define HWA_TEST_SRC_TRIGGER_DMACH1       1

#define HWA_MEMn_SIZE                     CSL_DSS_HWA_BANK_SIZE //16K size

/* benchmark collect flag, if defined, benchmark is collected after functional test pass, if un-defined, no benchmark is collcted after the funtional test
 * default is HWA_TEST_COLLECTBENCHMARK defined
 */
#define HWA_TEST_COLLECTBENCHMARK

/* hwa memory cache is disabeld by default in the test */
#define SOC_HWA_MEM0  CSL_DSS_HWA_DMA0_RAM_BANK0_BASE
#define SOC_HWA_MEM1  CSL_DSS_HWA_DMA0_RAM_BANK1_BASE

cmplx32ImRe_t     DCEstResult[2 * HWA_TEST_NUM_RX_ANT];
uint32_t          magEstResult[HWA_TEST_NUM_RX_ANT];
uint32_t          magDiffEstResult[HWA_TEST_NUM_RX_ANT];
cmplx64ImRe_t     DCAccResult[HWA_TEST_NUM_RX_ANT];
uint64_t          magAccResult[HWA_TEST_NUM_RX_ANT];
uint64_t          magDiffAccResult[HWA_TEST_NUM_RX_ANT];
uint16_t          shuffleIdx[HWA_TEST_NUM_SAMPLES];
uint32_t          fft_input_32bits[256];
uint32_t          vectorArray[HWA_TEST_NUM_SAMPLES * 2];
HWA_Stats         statsResults[4];
HWA_CdfThreshold  cdfThresholdResults[16];

#ifdef HWA_TEST_COLLECTBENCHMARK
#pragma DATA_SECTION(benchmarkData, ".benchmarkL2");
uint8_t           benchmarkData[16384];   //for benchmark data transfer from hwa to L2.

#ifdef BUILD_DSP_1

#pragma DATA_SECTION(benchmarkL1Data, ".benchmarkL1");
uint8_t           benchmarkL1Data[16384];
#endif
#ifdef BUILD_MCU
#pragma DATA_SECTION(benchmarkL1Data, ".benchmarkL1");
uint8_t           benchmarkL1Data[8192];
#endif
#endif

#define TEST_PARAMSET_INTERRUPT


#define   DRV_API_INIT       0
#define   DRV_API_OPEN       1
#define   DRV_API_CLOSE      2
#define   DRV_API_ENABLE     3
#define   DRV_API_SOFTWARETRIG  4
#define   DRV_API_DMAMANUALTRIG 5
#define   DRV_API_SETSRCADDR    6

uint32_t          drvApiCycles[7];

uint8_t            finalTestResultsPass  = 1;

/**************************************************************************
 *************************** HWA Test Functions **************************
 **************************************************************************/

 /**
 *  @b Description
 *  @n
 *      general paramset done interrupt call back function.
 *
 *  @retval
 *      Not Applicable.
 */
void HWA_Test_ParamSetISR_Callback(uint32_t intrIdx, uint32_t paramSet, void * arg)
{
    SemaphoreP_Handle       semHandle;
    gHWATestParamSetISR++;

    if (arg!=NULL) {
        semHandle = (SemaphoreP_Handle)arg;
        SemaphoreP_post(semHandle);
    }
}

/**
*  @b Description
*  @n
*      paramset done interrupt call back function for force switch test in context switch test.
*
*  @retval
*      Not Applicable.
*/
void HWA_contextswitch_Test_ParamSetISR_Callback(uint32_t intrIdx, uint32_t paramSet, void * arg)
{
    gHWAcontextswitchTestParamSetIdx[gHWAcontextswitchTestParamIdx] = paramSet;
    gHWAcontextswitchTestParamIdx++;
    /* paramset 5 is the last paramset in background thread, once it is done, trigger the next loop in background thread */
    if (paramSet == 5)
    {
        gHWAOneLoopDone = 1;
    }

    if (intrIdx == HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1)
        intr1ParamDoneCount++;
    else
        intr2ParamDoneCount++;

    /* for software triggered switch, every paramset is software triggered, set this flag to trigger next paramset */
    gHWAcontextswitchParamDone = 1;

}

/**
*  @b Description
*  @n
*      all loop is done interrupt call back function for background thread.
*
*  @retval
*      Not Applicable.
*/
void HWA_Test_DoneISR_Callback(uint32_t threadIdx, void * arg)
{
    SemaphoreP_Handle       semHandle;
    gHWATestDoneISR = threadIdx + 1;

    if (arg!=NULL) {
        semHandle = (SemaphoreP_Handle)arg;
        SemaphoreP_post(semHandle);
    }
}

/**
*  @b Description
*  @n
*      all loop is done interrupt call back function for ALT thread.
*
*  @retval
*      Not Applicable.
*/
void HWA_Test_ALT_DoneISR_Callback(uint32_t threadIdx, void * arg)
{
    SemaphoreP_Handle       semHandle;

    gHWATestDoneISR = threadIdx + 2;
    gHWAALTThreadDone = 1;

    if (arg != NULL) {
        semHandle = (SemaphoreP_Handle)arg;
        SemaphoreP_post(semHandle);
    }

}

/**
 *  @b Description
 *  @n
 *      Takes a 32bit integer to find floor(log2(x))
 *
 *  @param[in]  x       32bit integer.
 *
 *  @retval     floor(log2(x))
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint32_t mathUtils_floorLog2(uint32_t x)
{
#ifdef BUILD_DSP_1
    return (30 - _norm(x)); // On DSP _norm(x) counts number of number of redundant sign bits
#endif
#ifdef BUILD_MCU
    return (31 - _norm(x)); // On Arm _norm(x) counts number of leading zeros
#endif
}


uint32_t log2Approx(uint32_t x)
{
    uint32_t idx,detectFlag=0;

    if ( x < 2)
    {
        return (0);
    }

    idx=32;
    while((detectFlag==0)||(idx==0))
    {
        if(x&0x80000000)
        {
            detectFlag=1;
        }
        x<<=1;
        idx--;
    }

    return(idx);
}

/**
*  @b Description
*  @n
*      context switch test function, includes force switch, software triggerd switch and DMA triggered switch.
*
*  @retval
*      Not Applicable.
*/
static void HWA_contextswitch_test(HWA_Handle handle)
{
    int32_t                 errCode, compareCode1, compareCode2;
    uint8_t                 paramsetIdx;
    uint8_t                 ii;
    HWA_InterruptConfig     paramISRConfig;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM1;

    SemaphoreP_Handle       doneSem;
    SemaphoreP_Handle       doneSemALT;
    SemaphoreP_Params       semParams;
    SemaphoreP_Status       status;
    uint32_t                numLoops;


    HWA_log("\n\n ------- HWA context switch tests ------- \n\n");

    /*configure paramset used for the context switch tests */
    memset(gHWATestParamConfig, 0, sizeof(gHWATestParamConfig));  //init to zero
    /* gHWATestParamConfig[0] is used in background thread */
    paramsetIdx = 0;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;
    gHWATestParamConfig[paramsetIdx].source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(srcAddr) ; //(uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = HWA_TEST_NUM_SAMPLES - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = HWA_TEST_NUM_RX_ANT - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;

    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(dstAddr) ;//(uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = HWA_TEST_NUM_SAMPLES - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping


    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = 0;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = 0; //disable
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = 0; //start of window RAM
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.winSymm = 0;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_VECTOR_MULT;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.vectorMultiplyMode1.vecMultiMode1RamAddrOffset = 5;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.vectorMultiplyMode1.cmultScaleEn = 0;

    /* gHWATestParamConfig[1] is the ALT thread paramset */
    paramsetIdx = 1;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;
    gHWATestParamConfig[paramsetIdx].source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA ( (uint32_t) srcAddr + (2*0x4000)); //(uint32_t)((uint32_t)srcAddr + (2 * 0x4000) - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = HWA_TEST_NUM_SAMPLES - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = HWA_TEST_NUM_RX_ANT - 1; //no iterations here
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.bpmEnable = HWA_FEATURE_BIT_DISABLE;
    /* M1*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA ((uint32_t) dstAddr + (2 * 0x4000));//(uint32_t)((uint32_t)dstAddr + (2 * 0x4000) - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = HWA_TEST_1DFFT_SIZE - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = log2Approx(HWA_TEST_1DFFT_SIZE);
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x3; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE; //enabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = HWA_TEST_1DFFT_WINDOW_START; //start of window RAM
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.winSymm = HWA_FFT_WINDOW_NONSYMMETRIC; //non-symmetric - in demo do we make this symmetric

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT; // output FFT samples
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

    /*gHWATestParamConfig[2] is a dummy paramset used in background thread */
    paramsetIdx = 2;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_NONE; //dummy

    /* configure the paramsetdone interrupt */
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
    paramISRConfig.cpu.callbackFn = HWA_contextswitch_Test_ParamSetISR_Callback;
    paramISRConfig.cpu.callbackArg = NULL;


    errCode = HWA_reset(handle);

    /* enable the context switch */
    errCode = HWA_enableContextSwitch(handle, 1);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableContextSwitch return %d\n", errCode);
    }

    /* set up the data */
    for (ii = 0; ii < HWA_TEST_NUM_SAMPLES; ii++)
    {
        vectorArray[2 * ii] = 0xfffff; //Q20 1
        vectorArray[2 * ii + 1] = 0x0; //imag
    }
    errCode = HWA_configRam(handle, HWA_RAM_TYPE_VECTORMULTIPLY_RAM, (uint8_t *)vectorArray,
        HWA_TEST_NUM_SAMPLES * 8,  //complex coefficients, with 21 bits I and 21 bits Q
        gHWATestParamConfig[0].accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.vectorMultiplyMode1.vecMultiMode1RamAddrOffset * 8);


    /**************************************************************************
    * Prepare the input data for test vector based testing
    **************************************************************************/
    /* memcopy the data to MEM0 */
    memcpy(srcAddr, (uint8_t *)gHWATest_1DFFT_input,
        HWA_TEST_NUM_SAMPLES*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    memcpy((uint8_t*)((uint32_t)srcAddr + 2 * 0x4000), (uint8_t *)gHWATest_1DFFT_input,
        HWA_TEST_NUM_SAMPLES*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    /* m1 vector multiply results */
    memset((uint8_t *)dstAddr, 0, HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);

    compareCode2 = 0;

    /* force context switch tests */
    HWA_log("\n Debug: HWA context switch (force switch) : test starts \n");


    /* set up the background thread, 6 paramsets, loop 3, all paramsetDone interrupt enabled
       paramset 0: software trigger, INTR2
       paramset 1: immediate trigger, INTR2
       paramset 2: immediate trigger + force context switch, INTR2
       paramset 3: software trigger, will triggered by ALT allloopDone interrupt, INTR2
       paramset 4: immediate trigger, INTR2
       paramset 5: immediate trigger, INTR2, in the paramsetDone interrupt, will trigger the paramset 0 in the next loop
    */
    paramsetIdx = 0;  //paramset 0
    gHWATestParamConfig[0].triggerMode =  HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

    paramsetIdx++;  //pareamset 1
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);


    paramsetIdx++; //paramset 2
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_FORCE_ENABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);


    paramsetIdx++; //paramset 3 set as software trigger
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);


    paramsetIdx++; //paramset 4
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

    paramsetIdx++; //paramset 5
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

    /* set up the interrupt for background thread paramsets*/
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2;
    for (ii = 0; ii < 6 ; ii++)
    {
        errCode = HWA_enableParamSetInterrupt(handle, ii, &paramISRConfig);
    }
    HWA_log("Debug: background thread %d paramsets configuration done\n", paramsetIdx + 1);

    /* set up the ALT thread paramsets, 2 paramsets, loop 2, all paramsetDone interrupt enabled
       paramset 10: immediate triggered, INTR1
       paramset 11: immediate triggered, INTR1
       ALT all loop done interrupt is alse enabled, to trigger paramset 3 in the background thread.
    */

    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;

    paramsetIdx = 10;
    gHWATestParamConfig[1].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[1].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[1], NULL);
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);
    HWA_log("Debug: ALT (high priority) thread paramset %d, ", paramsetIdx);

    paramsetIdx++; //paramset 11
    gHWATestParamConfig[1].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[1].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);
    HWA_log("paramset %d configuration done\n ", paramsetIdx);

    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG | HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG_ALT;

    gCommonConfig.paramStartIdx = 0;
    gCommonConfig.paramStopIdx = 5;
    gCommonConfig.numLoops = 3;

    gCommonConfig.paramStartIdxALT = 10;
    gCommonConfig.paramStopIdxALT = 11;
    gCommonConfig.numLoopsALT = 2;

    errCode = HWA_configCommon(handle, &gCommonConfig);

    /* param all done interrupt for background thread */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    doneSem = SemaphoreP_create(0, &semParams);
    errCode = HWA_enableDoneInterrupt(handle, 0, HWA_Test_DoneISR_Callback, doneSem);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableDoneInterrupt return %d\n", errCode);
    }

    /* param all done interrupt for ALT thread  */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    doneSemALT = SemaphoreP_create(0, &semParams);

    errCode = HWA_enableDoneInterrupt(handle, 1, HWA_Test_ALT_DoneISR_Callback, doneSemALT);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableDoneInterrupt return %d\n", errCode);
    }



    numLoops = gCommonConfig.numLoops;

    //contextswitchTestStartTime = TSCL;

    /* hwa enable */
    errCode = HWA_enable(handle, 1);

    /* trigger background thread */
    errCode = HWA_setSoftwareTrigger(handle);
    numLoops--;

    while (!gHWAALTThreadDone)
    {

    }
    //wait for ALT treahd  done, trigger paramset 3 in the background thread
    gHWAALTThreadDone = 0;
    errCode = HWA_setSoftwareTrigger(handle);



    while (numLoops)
    {
        while (!gHWAOneLoopDone) //wait for background thread loop is done
        {

        }
        gHWAOneLoopDone = 0;
        numLoops--;
        errCode = HWA_setSoftwareTrigger(handle); //trigger next loop for background thread

        while (!gHWAALTThreadDone)
        {

        }
        gHWAALTThreadDone = 0;
        //wait for ALT thread done, trigger paramset 3
        errCode = HWA_setSoftwareTrigger(handle);
    }

    while (!gHWAOneLoopDone) //wait for last paramset ISR in background thread is finish
    {

    }
    gHWAOneLoopDone = 0;
    /* then wait for ALT thread done interrupt */
    status = SemaphoreP_pend(doneSemALT, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        HWA_log("Error: SemaphoreP_pend returned %d\n", status);
    }

    /* then wait for background thread done interrupt */
    status = SemaphoreP_pend(doneSem, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        HWA_log("Error: SemaphoreP_pend returned %d\n", status);
    }

    /* disable interrupt */
    for (ii = 0; ii < 6; ii++)
    {
        errCode = HWA_disableParamSetInterrupt(handle, ii, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
    }

    errCode = HWA_disableParamSetInterrupt(handle, 10, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
    errCode = HWA_disableParamSetInterrupt(handle, 11, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
    errCode = HWA_disableDoneInterrupt(handle, 0);
    errCode = HWA_disableDoneInterrupt(handle, 1);


    HWA_log("\n Debug:  HWA completed, check the results \n\n");
    /* check the results in gHWAcontextswitchTestParamSetIdx */
    compareCode1 = memcmp((uint8_t *)gHWAcontextswitchTestParamSetIdx, (uint8_t*)gHWATest_contextswitchParamIdx_output, 10 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (force switch) : first loop output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (force switch) : first loop  generated by HWA found correct\n");
    }
    compareCode2 += compareCode1;
    compareCode1 = memcmp((uint8_t *) (gHWAcontextswitchTestParamSetIdx + 10), (uint8_t*)gHWATest_contextswitchParamIdx_output, 10 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (force switch) : second loop output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (force switch) : second loop  generated by HWA found correct\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t *)(gHWAcontextswitchTestParamSetIdx + 20), (uint8_t*)gHWATest_contextswitchParamIdx_output, 10 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (force switch) : third loop output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (force switch) : third loop  generated by HWA found correct\n");
    }

    /* check the interrupt counts from two threads*/
    if ((intr2ParamDoneCount == 18) && (intr1ParamDoneCount==12))
    {
        HWA_log("Debug: HWA context switch (force switch) : INTR1/INTR2 paramsetDone interrupts generated by HWA found correct\n");
        compareCode1 = 0;
    }
    else
    {
        HWA_log("Debug: HWA context switch (force switch) : INTR1/INTR2 paramsetDone interrupts generated by HWA found incorrect : %d, %d\n", intr2ParamDoneCount, intr1ParamDoneCount);
        compareCode1 = 1;
    }

    compareCode2 += compareCode1;
    if (compareCode2 == 0)
    {
        HWA_log("\n Debug: HWA context switch (force switch) : test passes \n");
    }
    /* disable HWA */
    errCode = HWA_enable(handle, 0);
    /*  delete SemaphoreP */
    if (doneSem != NULL)
        SemaphoreP_delete(doneSem);

    if (doneSemALT != NULL)
        SemaphoreP_delete(doneSemALT);

    /* software trigger context switch */

    HWA_log("\n");

    /* 6 background thread paramsets same as forced context switch test, except all paramsets are software triggered,
       and in paramset 2, context switch is triggered by software
       background thread, paramset 0,1,2, INTR2, paramset 3,4,5 INTR1
       ALT thread, paramset 10, INTR1,pramset 11, INTR2
    */

    HWA_log("\n Debug: HWA context switch (software trigger switch) : test starts \n");
    paramsetIdx = 0;
	gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

    paramsetIdx++;
	gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_SOFTWARE;
	gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
	errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

	paramsetIdx++;
	gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_SOFTWARE;
	gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_NONFORCE_ENABLE;
	errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);
	
	paramsetIdx++; 
	gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_SOFTWARE;
	gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
	errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);


	paramsetIdx++;
	errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

	paramsetIdx++;
	errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

	/* set up the interrupt for background thread paramsets*/
	paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2;
	for (ii = 0; ii < 3; ii++)
	{
		errCode = HWA_enableParamSetInterrupt(handle, ii, &paramISRConfig);
	}

	paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
	for (ii = 3; ii < 6; ii++)
	{
		errCode = HWA_enableParamSetInterrupt(handle, ii, &paramISRConfig);
	}
	HWA_log("Debug: background thread %d paramsets configuration done\n", paramsetIdx + 1);

	/* 2 paramsets in ALT thread, same as foced context switch test, except all software triggered */
	paramsetIdx = 10;
	gHWATestParamConfig[1].triggerMode = HWA_TRIG_MODE_SOFTWARE;
	gHWATestParamConfig[1].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
	errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[1], NULL);

	paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
	errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);
	HWA_log("Debug: high-priority thread paramset %d, ", 10);


	paramsetIdx++; 
	errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[1], NULL);
	paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2;
	errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);
	HWA_log("paramset %d configurations done\n", 11);


    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG | HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG_ALT | HWA_COMMONCONFIG_CONTEXTSWITCH_TRIG_CFG;

    gCommonConfig.paramStartIdx = 0;
    gCommonConfig.paramStopIdx = 5;
    gCommonConfig.numLoops = 3;

    gCommonConfig.paramStartIdxALT = 10;
    gCommonConfig.paramStopIdxALT = 11;
    gCommonConfig.numLoopsALT = 2;

    gCommonConfig.contextswitchTriggerMode = HWA_CONTEXTSWITCH_TRIG_MODE_SOFTWARE;

    errCode = HWA_configCommon(handle, &gCommonConfig);

    /* param all done interrupt for background thread */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    doneSem = SemaphoreP_create(0, &semParams);
    errCode = HWA_enableDoneInterrupt(handle, 0, HWA_Test_DoneISR_Callback, doneSem);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableDoneInterrupt return %d\n", errCode);
    }

    /* param all done interrupt for ALT thread */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    doneSemALT = SemaphoreP_create(0, &semParams);
    errCode = HWA_enableDoneInterrupt(handle, 1, HWA_Test_ALT_DoneISR_Callback, doneSemALT);

    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableDoneInterrupt return %d\n", errCode);
    }

    numLoops = gCommonConfig.numLoops;

    errCode = HWA_enableContextSwitch(handle, 1);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableContextSwitch return %d\n", errCode);
    }

    errCode = HWA_enable(handle, 1);

    //contextswitchTestStartTime = TSCL;

    for (ii = 0; ii < numLoops; ii++)
    {
        /* trigger backgroud thread */
        errCode = HWA_setSoftwareTrigger(handle); //param 0
        while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;
		errCode = HWA_setSoftwareTrigger(handle); //param 1
		while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;
        /* trigger high-priority ALT thread */
        errCode = HWA_setContextswitchSoftwareTrigger(handle);
        errCode = HWA_setSoftwareTrigger(handle); //param 2
        while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;
        errCode = HWA_setSoftwareTrigger(handle); //param 10
        while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;
        errCode = HWA_setSoftwareTrigger(handle); //param 11
        while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;
        errCode = HWA_setSoftwareTrigger(handle); //param 10
        while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;
        errCode = HWA_setSoftwareTrigger(handle); //param 11
        while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;
		errCode = HWA_setSoftwareTrigger(handle); //param 3
        while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;
        errCode = HWA_setSoftwareTrigger(handle); //param 4
        while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;
		errCode = HWA_setSoftwareTrigger(handle); //param 5
		while (!gHWAcontextswitchParamDone)
        {

        }
        gHWAcontextswitchParamDone = 0;

    }

    /* then wait for ALT thread done interrupt */
    status = SemaphoreP_pend(doneSemALT, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        HWA_log("Error: SemaphoreP_pend returned %d\n", status);
    }


    /* then wait for background thread done interrupt */
    status = SemaphoreP_pend(doneSem, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        HWA_log("Error: SemaphoreP_pend returned %d\n", status);
    }


    for (ii = 0; ii < 3; ii++)
    {
        errCode = HWA_disableParamSetInterrupt(handle, ii, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
    }
    for (ii = 3; ii < 6; ii++)
    {
        errCode = HWA_disableParamSetInterrupt(handle, ii, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
    }

    errCode = HWA_disableParamSetInterrupt(handle, 10, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
    errCode = HWA_disableParamSetInterrupt(handle, 11, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);

    errCode = HWA_disableDoneInterrupt(handle, 0);
    errCode = HWA_disableDoneInterrupt(handle, 1);


    HWA_log("\n Debug:  HWA completed, check the results \n\n");
    /* check the results in gHWAcontextswitchTestParamSetIdx */
    compareCode1 = memcmp((uint8_t *) (gHWAcontextswitchTestParamSetIdx + 30), (uint8_t*)gHWATest_contextswitchParamIdx_output, 10 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (software trigger switch) : first loop output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (software trigger switch) : first loop  generated by HWA found correct\n");
    }
    compareCode2 += compareCode1;
    compareCode1 = memcmp((uint8_t *)(gHWAcontextswitchTestParamSetIdx + 40), (uint8_t*)gHWATest_contextswitchParamIdx_output, 10 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (software trigger switch) : second loop output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (software trigger switch) : second loop  generated by HWA found correct\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t *)(gHWAcontextswitchTestParamSetIdx + 50), (uint8_t*)gHWATest_contextswitchParamIdx_output, 10 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (software trigger switch) : third loop output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (software trigger switch) : third loop  generated by HWA found correct\n");
    }

    /* check the interrupt counts from two threads*/
    if ((intr2ParamDoneCount == 18  + 15) && (intr1ParamDoneCount == 12 + 15))
    {
        HWA_log("Debug: HWA context switch (software trigger switch) : INTR1/INTR2 paramsetDone interrupts generated by HWA found correct\n");
        compareCode1 = 0;
    }
    else
    {
        HWA_log("Debug: HWA context switch (software trigger switch) : INTR1/INTR2 paramsetDone interrupts generated by HWA found incorrect : %d, %d\n", intr2ParamDoneCount, intr1ParamDoneCount);
        compareCode1 = 1;
    }



    if (compareCode2 == 0)
    {
        HWA_log("\n Debug: HWA context switch (software trigger switch) : test passes \n");
    }

    errCode = HWA_enable(handle, 0);
    /*  delete SemaphoreP */
    if (doneSem != NULL)
        SemaphoreP_delete(doneSem);

    if (doneSemALT != NULL)
        SemaphoreP_delete(doneSemALT);



    /* DMA trigger context switch */
    HWA_log("\n");

    /* 6 paramsets in background thread are same as force context switch test, except
       paramset 0, 1 are immediate trigger,
       paramset 2 is software trigger, and also set the DMA trigger context switch
       paramset 3 software trigger, triggered by ALT thread allloopDone interrupt
       paramset 4, 5 all immediate trigger.
       and all use INTR1
   */

    HWA_log("\n Debug: HWA context switch (DMA trigger switch) : test starts \n");
    paramsetIdx = 0;
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

    paramsetIdx++;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

    paramsetIdx++;
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_NONFORCE_ENABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

    paramsetIdx++;
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);


    paramsetIdx++;
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

    paramsetIdx++;
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);

    /* set up the interrupt for background thread paramsets*/
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
    for (ii = 0; ii < 6; ii++)
    {
        errCode = HWA_enableParamSetInterrupt(handle, ii, &paramISRConfig);
    }

    HWA_log("Debug: background thread %d paramsets configuration done\n", paramsetIdx + 1);

    /* 2 paramsets in ALT thread are same as forced context switch, except all immediate trigger */
    paramsetIdx = 10;
    gHWATestParamConfig[1].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[1], NULL);
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);
    HWA_log("Debug: high-priority thread paramset %d, ", 10);


    paramsetIdx++;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[1], NULL);
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);
    HWA_log("paramset %d configurations done\n", 11);


    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG | HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG_ALT | HWA_COMMONCONFIG_CONTEXTSWITCH_TRIG_CFG;

    gCommonConfig.paramStartIdx = 0;
    gCommonConfig.paramStopIdx = 5;
    gCommonConfig.numLoops = 3;

    gCommonConfig.paramStartIdxALT = 10;
    gCommonConfig.paramStopIdxALT = 11;
    gCommonConfig.numLoopsALT = 2;

    gCommonConfig.contextswitchTriggerMode = HWA_CONTEXTSWITCH_TRIG_MODE_DMA;
    gCommonConfig.contextswitchTriggerSrc = 4; //DMA channel number
    errCode = HWA_configCommon(handle, &gCommonConfig);

    /* param all done interrupt for background thread */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    doneSem = SemaphoreP_create(0, &semParams);
    errCode = HWA_enableDoneInterrupt(handle, 0, HWA_Test_DoneISR_Callback, doneSem);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableDoneInterrupt return %d\n", errCode);
    }

    /* param all done interrupt for ALT thread */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    doneSemALT = SemaphoreP_create(0, &semParams);
    errCode = HWA_enableDoneInterrupt(handle, 1, HWA_Test_ALT_DoneISR_Callback, doneSemALT);

    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableDoneInterrupt return %d\n", errCode);
    }


    errCode = HWA_enableContextSwitch(handle, 1);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableContextSwitch return %d\n", errCode);
    }

    errCode = HWA_enable(handle, 1);

    //contextswitchTestStartTime = TSCL;

    for (ii = 0; ii < numLoops; ii++)
    {

        errCode = HWA_setContextswitchDMAManualTrigger(handle, gCommonConfig.contextswitchTriggerSrc); //manually triger the DMA trigger for context switch
        /* trigger backgroud thread, paramset 2*/
        errCode = HWA_setSoftwareTrigger(handle); //param 2

        /* then wait for ALT thread done interrupt */
        status = SemaphoreP_pend(doneSemALT, SemaphoreP_WAIT_FOREVER);
        if (status != SemaphoreP_OK)
        {
            HWA_log("Error: SemaphoreP_pend returned %d\n", status);
        }
        //when ALT thread is done, trigger param3
        errCode = HWA_setSoftwareTrigger(handle); //param 3
    }


    /* then wait for background done interrupt */
    status = SemaphoreP_pend(doneSem, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        HWA_log("Error: SemaphoreP_pend returned %d\n", status);
    }


    for (ii = 0; ii < 6; ii++)
    {
        errCode = HWA_disableParamSetInterrupt(handle, ii, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
    }

    errCode = HWA_disableParamSetInterrupt(handle, 10, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
    errCode = HWA_disableParamSetInterrupt(handle, 11, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);

    errCode = HWA_disableDoneInterrupt(handle, 0);
    errCode = HWA_disableDoneInterrupt(handle, 1);


    HWA_log("\n Debug:  HWA completed, check the results \n\n");
    /* check the results in gHWAcontextswitchTestParamSetIdx */
    compareCode1 = memcmp((uint8_t *)(gHWAcontextswitchTestParamSetIdx + 60), (uint8_t*)gHWATest_contextswitchParamIdx_output, 10 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (DMA trigger switch) : first loop output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (DMA trigger switch) : first loop  generated by HWA found correct\n");
    }
    compareCode2 += compareCode1;
    compareCode1 = memcmp((uint8_t *)(gHWAcontextswitchTestParamSetIdx + 70), (uint8_t*)gHWATest_contextswitchParamIdx_output, 10 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (DMA trigger switch) : second loop output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (DMA trigger switch) : second loop  generated by HWA found correct\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t *)(gHWAcontextswitchTestParamSetIdx + 80), (uint8_t*)gHWATest_contextswitchParamIdx_output, 10 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (DMA trigger switch) : third loop output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (DMA trigger switch) : third loop  generated by HWA found correct\n");
    }


    /* check the interrupt counts from two threads*/
    if ((intr2ParamDoneCount == (18 + 15)) && (intr1ParamDoneCount == (12 + 15 + 30)))
    {
        HWA_log("Debug: HWA context switch (DMA trigger switch) : INTR1/INTR2 paramsetDone interrupts generated by HWA found correct\n");
        compareCode1 = 0;
    }
    else
    {
        HWA_log("Debug: HWA context switch (DMA trigger switch) : INTR1/INTR2 paramsetDone interrupts generated by HWA found incorrect : %d, %d\n", intr2ParamDoneCount, intr1ParamDoneCount);
        compareCode1 = 1;
    }

    if (compareCode2 == 0)
    {
        HWA_log("\n Debug: HWA context switch (DMA trigger switch) : test passes \n");
    }


    errCode = HWA_enable(handle, 0);
    /*  delete SemaphoreP */
    if (doneSem != NULL)
        SemaphoreP_delete(doneSem);

    if (doneSemALT != NULL)
        SemaphoreP_delete(doneSemALT);



    /* multiple context switches in background switch with dummy paramsets,
       context switch is enabled by either force, or software trigger,
       all use INTR2 */
    HWA_log("\n");

    /* set up paramsets for background thread */

    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2;

    paramsetIdx = 32;
    gHWATestParamConfig[2].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[2].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_FORCE_ENABLE; //first one is force context swith
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[2], NULL);
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);

    paramsetIdx++;
    gHWATestParamConfig[2].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[2].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_NONFORCE_ENABLE; //second one is software triggerd context switch;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[2], NULL);
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);

    paramsetIdx++;
    gHWATestParamConfig[0].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[0].contextswitchCfg = HWA_PARAMSET_CONTEXTSWITCH_DISABLE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[0], NULL);
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);


    /* 2 paramsets in ALT thread are same as forced context switch, except all immediate trigger */
    paramsetIdx = 10;
    gHWATestParamConfig[1].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[1], NULL);
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);
    HWA_log("Debug: high-priority thread paramset %d, ", 10);


    paramsetIdx++;
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[1], NULL);
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);
    HWA_log("paramset %d configurations done\n", 11);


    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG | HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG_ALT | HWA_COMMONCONFIG_CONTEXTSWITCH_TRIG_CFG;

    gCommonConfig.paramStartIdx = 32;
    gCommonConfig.paramStopIdx = 34;
    gCommonConfig.numLoops = 1;

    gCommonConfig.paramStartIdxALT = 10;
    gCommonConfig.paramStopIdxALT = 11;
    gCommonConfig.numLoopsALT = 1;

    gCommonConfig.contextswitchTriggerMode = HWA_CONTEXTSWITCH_TRIG_MODE_SOFTWARE;
    errCode = HWA_configCommon(handle, &gCommonConfig);

    /* param all done interrupt for background thread */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    doneSem = SemaphoreP_create(0, &semParams);
    errCode = HWA_enableDoneInterrupt(handle, 0, HWA_Test_DoneISR_Callback, doneSem);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableDoneInterrupt return %d\n", errCode);
    }

    /* param all done interrupt for ALT thread */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    doneSemALT = SemaphoreP_create(0, &semParams);
    errCode = HWA_enableDoneInterrupt(handle, 1, HWA_Test_ALT_DoneISR_Callback, doneSemALT);

    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableDoneInterrupt return %d\n", errCode);
    }


    errCode = HWA_enableContextSwitch(handle, 1);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableContextSwitch return %d\n", errCode);
    }

    errCode = HWA_enable(handle, 1);

    errCode = HWA_setSoftwareTrigger(handle); //trigger paramset 32
    /* force context switch, then wait for ALT thread done interrupt */
    status = SemaphoreP_pend(doneSemALT, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        HWA_log("Error: SemaphoreP_pend returned %d\n", status);
    }

    /* trigger high-priority ALT thread */
    errCode = HWA_setContextswitchSoftwareTrigger(handle);

    errCode = HWA_setSoftwareTrigger(handle); //trigger paramset 33

    /* force context switch, then wait for ALT thread done interrupt */
    status = SemaphoreP_pend(doneSemALT, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        HWA_log("Error: SemaphoreP_pend returned %d\n", status);
    }

    errCode = HWA_setSoftwareTrigger(handle); //trigger paramset 34

    /* wait for background thread done */
    status = SemaphoreP_pend(doneSem, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        HWA_log("Error: SemaphoreP_pend returned %d\n", status);
    }


    errCode = HWA_disableParamSetInterrupt(handle, 32, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
    errCode = HWA_disableParamSetInterrupt(handle, 33, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
    errCode = HWA_disableParamSetInterrupt(handle, 34, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);

    errCode = HWA_disableParamSetInterrupt(handle, 10, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
    errCode = HWA_disableParamSetInterrupt(handle, 11, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);

    errCode = HWA_enable(handle, 0);
    /*  delete SemaphoreP */
    if (doneSem != NULL)
        SemaphoreP_delete(doneSem);

    if (doneSemALT != NULL)
        SemaphoreP_delete(doneSemALT);

    HWA_log("\n Debug:  HWA completed, check the results \n\n");

    compareCode1 = memcmp((uint8_t *)(gHWAcontextswitchTestParamSetIdx + 90), (uint8_t*) (gHWATest_contextswitchParamIdx_output + 10), 7 * sizeof(uint32_t));
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA context switch (multiple switches from dummy paramset) : output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA context switch (multiple switches from dummy paramset) : output generated by HWA found correct\n");
    }
    compareCode2 += compareCode1;

    /* check the interrupt counts from two threads*/
    if ((intr2ParamDoneCount == (18 + 15 + 7)) && (intr1ParamDoneCount == (12 + 15 + 30)))
    {
        HWA_log("Debug: HWA context switch (multiple switches from dummy paramset) : INTR1/INTR2 paramsetDone interrupts generated by HWA found correct\n");
        compareCode1 = 0;
    }
    else
    {
        HWA_log("Debug: HWA context switch (multiple switches from dummy paramset) : INTR1/INTR2 paramsetDone interrupts generated by HWA found incorrect : %d, %d\n", intr2ParamDoneCount, intr1ParamDoneCount);
        compareCode1 = 1;
    }
    compareCode2 += compareCode1;

    if (compareCode2 == 0)
    {
        HWA_log("\n Debug: HWA context switch (multiple switches from dummy paramset) : test passes \n");
    }

    HWA_log("\n");
    if (compareCode2 != 0)
    {
        //MCPI_setFeatureTestResult("HWA context switch Tests", MCPI_TestResult_FAIL);
        HWA_log("Feature :  HWA context switch Tests FAIL\n");
        finalTestResultsPass = 0;
    }
    else
    {
        //MCPI_setFeatureTestResult("HWA context switch Tests", MCPI_TestResult_PASS);
        HWA_log("Feature :  HWA context switch Tests PASS\n");
    }

    HWA_log("\n HWA context switch tests complelted \n\n");


}
/**
*  @b Description
*  @n
*      test function for FFT stitching - 4K fft .
*
*  @retval
*      Not Applicable.
*/
static void HWA_fft4k_test(HWA_Handle handle)
{
    int32_t                 errCode, compareCode2;
    uint8_t                 paramsetIdx = 0;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM1;
    uint16_t                fftSize = 4096;

    compareCode2 = 0;
    HWA_log("\n\n ------- HWA FFT 4K tests ------- \n\n");

    /* FFT size 4096 */
    paramsetIdx = 0;
    memset(gHWATestParamConfig, 0, sizeof(gHWATestParamConfig));  //init to zero
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;

    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = 2048 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_COMPLEX_16BIT_SIZE * 2; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = 2 - 1; //2 fft
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_16BIT_SIZE ; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 0;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.bpmEnable = HWA_FEATURE_BIT_DISABLE;
    /* M1*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = 2048 - 1;
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = 16;
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = 8; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 8;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = 11;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x7ff; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE; //enabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = 0; //start of window RAM
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.winSymm = HWA_FFT_WINDOW_NONSYMMETRIC; //non-symmetric - in demo do we make this symmetric

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT; // output FFT samples
    /* first paramset, only set the window interpolation configuration */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.FFTstitching.winInterpolateMode = HWA_FFT_WINDOW_INTERPOLATE_MODE_4K;

    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);

    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) (Immediate trigged, AccelMode FFT - 4K) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() [Immediate trigged, AccelMode FFT - 4K]", MCPI_TestResult_FAIL);
        HWA_log("Feature :  API HWA_configParamSet() [Immediate trigged, AccelMode FFT - 4K] FAIL\n");
        finalTestResultsPass  =0;
        return;
    }
    else
    {
        HWA_log("Debug: paramset %d : Immediate Trigger AccelMode FFT - 4K - first paramset \n", paramsetIdx);

    }

    /* second paramset */
    paramsetIdx++;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;

    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = 2 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = 8; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = 2048 - 1; //2 fft
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = 16; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_32BIT; //32-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 0;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.bpmEnable = HWA_FEATURE_BIT_DISABLE;
    /* M1*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr + 2 * HWA_MEMn_SIZE - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = 2 - 1;
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = 2048*8;
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = 8; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 8;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = 1;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x3ff; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_DISABLE; //disable

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT; // output FFT samples
    /* second paramset, enable the fft stitching */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_FFT_STITCHING;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.FFTstitching.twiddlePattern = HWA_FFT_STITCHING_TWID_PATTERN_4K;

    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) (Immediate triggered, AccelMode FFT - 4K) returned %d\n", errCode, paramsetIdx);
        //PI_setFeatureTestResult("API HWA_configParamSet() [Immediate triggered, AccelMode FFT - 4K]", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() [Immediate triggered, AccelMode FFT - 4K] FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
        HWA_log("Debug: paramset %d : Immediate Trigger AccelMode FFT - 4K - second paramset \n", paramsetIdx);
    }

    /* common configuration */
    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG |
                    HWA_COMMONCONFIG_MASK_TWIDDITHERENABLE |
                    HWA_COMMONCONFIG_MASK_LFSRSEED;

     gCommonConfig.fftConfig.twidDitherEnable = 1;
     gCommonConfig.fftConfig.lfsrSeed = 0x1234567;


    gCommonConfig.paramStartIdx = 0;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1;

    errCode = HWA_configCommon(handle, &gCommonConfig);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configCommon(%d) (Immediate triggered, AccelMode FFT - 4K) returned %d\n", errCode, paramsetIdx);
       // MCPI_setFeatureTestResult("API HWA_configParamSet() [Immediate triggered, AccelMode FFT - 4K]", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() [Immediate triggered, AccelMode FFT - 4 FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
        HWA_log("Debug: paramset %d : HWA_configCommon FFT - 4K \n", paramsetIdx);
    }

    /* copy the input data */
    memcpy(srcAddr, (uint8_t*)gHWATest_4KFFT_input, sizeof(uint32_t) *fftSize);

    /* configure the window */
    errCode = HWA_configRam(handle, HWA_RAM_TYPE_WINDOW_RAM, (uint8_t *)win_fft4k, sizeof(win_fft4k), gHWATestParamConfig[0].accelModeArgs.fftMode.windowStart);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM) FAIL\n");
		finalTestResultsPass = 0;
        return;
    }


    /* enable HWA */
    errCode = HWA_enable(handle, 1);

    /* wait for hwa is done, both paramsets */
    errCode = HWA_singleParamSetDonePolling(handle, 0);
    errCode = HWA_singleParamSetDonePolling(handle, paramsetIdx);


    /* disable  HWA */
    errCode = HWA_enable(handle, 0);

    /* check the results */
    HWA_log("\n");

    compareCode2 = memcmp((uint8_t *)((uint32_t)dstAddr + 2 * HWA_MEMn_SIZE) , (uint8_t*)gHWATest_FFT4K_output, 4 * 2 * fftSize); //output is 32bits I/32 bitsQ

    if (compareCode2 != 0)
    {
        //MCPI_setFeatureTestResult("HWA 4KFFT Tests", MCPI_TestResult_FAIL);
        HWA_log("Feature : HWA 4KFFT Tests FAIL\n");
        finalTestResultsPass  = 0;
    }
    else
    {
       // MCPI_setFeatureTestResult("HWA 4KFFT Tests", MCPI_TestResult_PASS);
        HWA_log("Feature : HWA 4KFFT Tests PASS\n");
    }

    HWA_log("\n HWA 4KFFT test completed \n\n");

}
/**
*  @b Description
*  @n
*      function for 2D FFT .
*
*  @retval
*      Not Applicable.
*/
static void HWA_2dfft_test(HWA_Handle handle)
{

    int32_t                 errCode, compareCode2;
    uint8_t                 paramsetIdx = 0;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM1;
    uint16_t                fftSize1 = 8, fftSize2 = 16;
    uint16_t                numInputSamples = 96;


    compareCode2 = 0;
    HWA_log("\n\n ------- HWA 2D FFT tests ------- \n\n");

    /* 2D fft test  */
    paramsetIdx = 0;
    memset(gHWATestParamConfig, 0, sizeof(gHWATestParamConfig));  //init to zero
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;

    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = numInputSamples - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = 1 - 1; //no iterations here
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_16BIT_SIZE * numInputSamples; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.bpmEnable = HWA_FEATURE_BIT_DISABLE;
    /* M1*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = fftSize2 * fftSize1 - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = HWA_TEST_COMPLEX_16BIT_SIZE * fftSize2 * fftSize1; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = log2Approx(fftSize2 * fftSize1);
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSizeDim2 = log2Approx(fftSize2);
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x1ff; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE; //enabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = 0; //start of window RAM
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.winSymm = HWA_FFT_WINDOW_NONSYMMETRIC; //non-symmetric - in demo do we make this symmetric

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT; // output FFT samples
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

    /* add DC and interference statistics processing*/
    /* DC estimation */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
    /* interference statistics */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfStat.resetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;

    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);

    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) (Immediate triggered, AccelMode FFT - 2DFFT) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() [Immediate triggered, AccelMode FFT - 2DFFT]", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() [Immediate triggered, AccelMode FFT - 2DFFT] FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
        HWA_log("Debug: paramset %d : Immediate Trigger AccelMode FFT - 2DFFT\n", paramsetIdx);
    }

    /* configure the common register */
    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG;

    gCommonConfig.paramStartIdx = 0;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1;

    errCode = HWA_configCommon(handle, &gCommonConfig);

    /* copy the input data */
    memcpy(srcAddr, (uint8_t*)gHWATest_2DFFT_input, sizeof(uint32_t) *numInputSamples);

    /* configure the window */
    errCode = HWA_configRam(handle, HWA_RAM_TYPE_WINDOW_RAM, (uint8_t *)win_fft2d, sizeof(win_fft2d), gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM) returned %d\n", errCode);
       // MCPI_setFeatureTestResult("API HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /* enable HWA */
    errCode = HWA_enable(handle, 1);

    /* wait for hwa is done */
    errCode = HWA_singleParamSetDonePolling(handle, paramsetIdx);


    /* disable  HWA */
    errCode = HWA_enable(handle, 0);

    /* check the results */
    HWA_log("\n");

    compareCode2 = memcmp(dstAddr, (uint8_t*)gHWATest_2DFFT_output, 4 * fftSize1 * fftSize2);

    if (compareCode2 != 0)
    {
        //MCPI_setFeatureTestResult("HWA 2DFFT Tests", MCPI_TestResult_FAIL);
        HWA_log("Feature : HWA 2DFFT Tests FAIL\n");
        finalTestResultsPass = 0;
    }
    else
    {
       // MCPI_setFeatureTestResult("HWA 2DFFT Tests", MCPI_TestResult_PASS);
        HWA_log("Feature : HWA 2DFFT Tests PASS\n");
    }

    HWA_log("\n HWA 2DFFT test completed \n\n");

}


/**
*  @b Description
*  @n
*      compress/decompress test function, includes EGE compress/decompress tests.
*
*  @retval
*      Not Applicable.
*/
static void HWA_compress_test(HWA_Handle handle)
{
    int32_t                 errCode, compareCode1, compareCode2;
    uint8_t                 paramsetIdx = 0;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM1;
    uint8_t                 numEGEInputData = 128;
    uint8_t                 numEGEBlockSize = 16; //number of complex data
    uint8_t                 numEGECompressedWord;



    compareCode2 = 0;
    HWA_log("\n\n ------- HWA compress/decompress tests ------- \n\n");
    /* Do dma trigger mode test - simple FFT - one pass */
    paramsetIdx = 0;
    memset(gHWATestParamConfig, 0, sizeof(gHWATestParamConfig));  //init to zero
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    /*EGE algorithm */
    numEGECompressedWord = numEGEBlockSize >> 1; //50%
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_COMPRESS;

    /* compress 16 range bins, per chirp, per rx antenna, needs to check the data format */
    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = numEGEBlockSize - 1; // 16 complex values per block
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_COMPLEX_16BIT_SIZE ;
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = numEGEInputData/ numEGEBlockSize - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_16BIT_SIZE * numEGEBlockSize; //next block
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 0;

    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = numEGECompressedWord - 1; //16 * 4 bytes 50% -> 8 words
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = 4;     // 1 word size
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = 4 * numEGECompressedWord; // 8 words per block
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED;
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.ditherEnable = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.compressDecompress = HWA_CMP_DCMP_COMPRESS;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.method = HWA_COMPRESS_METHOD_EGE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.passSelect = HWA_COMPRESS_PATHSELECT_BOTHPASSES;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.headerEnable = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.scaleFactorBW = 4; //log2(sample bits)
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.EGEKarrayLength = 3; //log2(8)

    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) (software triggered, AccelMode compress) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() [software triggered, AccelMode compress]", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() [software triggered, AccelMode compress] FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
        HWA_log("\nDebug: paramset %d : software Trigger AccelMode Compression(EGE)\n", paramsetIdx);
    }

    paramsetIdx++;
    /* decompression */
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_COMPRESS;

    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = numEGECompressedWord - 1; // 8 words
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = 4; // word size
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = numEGEInputData/ numEGEBlockSize - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = 4 * numEGECompressedWord; //block
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_REAL; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_32BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_UNSIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 0;

    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr + HWA_MEMn_SIZE  - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = numEGEBlockSize - 1; //16 * 4 bytes 50% -> 8 words
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_COMPLEX_16BIT_SIZE ;
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = HWA_TEST_COMPLEX_16BIT_SIZE * numEGEBlockSize;
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED;
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.ditherEnable = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.compressDecompress = HWA_CMP_DCMP_DECOMPRESS;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.method = HWA_COMPRESS_METHOD_EGE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.passSelect = HWA_COMPRESS_PATHSELECT_BOTHPASSES;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.headerEnable = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.scaleFactorBW = 4; //log2(sample bits)
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.EGEKarrayLength = 3; //log2(8)

    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) (immediate triggered, AccelMode decompress) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() [immediate triggered, AccelMode decompress]", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() [software triggered, AccelMode compress] FAIL\n");
		finalTestResultsPass = 0;
        return;
    }
    else
    {
        HWA_log("Debug: paramset %d : immediate Trigger AccelMode deCompression(EGE)\n", paramsetIdx);
    }

    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));
    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG | HWA_COMMONCONFIG_MASK_EGECOMRESS_KPARAM;

    gCommonConfig.paramStartIdx = paramsetIdx - 1; //compression
    gCommonConfig.paramStopIdx = paramsetIdx; //decompression
    gCommonConfig.numLoops = 1; //do only one iteration


    gCommonConfig.compressConfig.EGEKparam[0] = 0;
    gCommonConfig.compressConfig.EGEKparam[1] = 2;
    gCommonConfig.compressConfig.EGEKparam[2] = 4;
    gCommonConfig.compressConfig.EGEKparam[3] = 6;
    gCommonConfig.compressConfig.EGEKparam[4] = 8;
    gCommonConfig.compressConfig.EGEKparam[5] = 10;
    gCommonConfig.compressConfig.EGEKparam[6] = 12;
    gCommonConfig.compressConfig.EGEKparam[7] = 15;
    errCode = HWA_configCommon(handle, &gCommonConfig);


    /* copy the input data */
    memcpy(srcAddr, (uint8_t*)gHWATest_compressEGE_input, sizeof(uint32_t) * numEGEInputData);

    errCode = HWA_enable(handle, 1);
    errCode = HWA_setSoftwareTrigger(handle);
    errCode = HWA_singleParamSetDonePolling(handle, paramsetIdx);

    HWA_log("\n");

    /* checked the decompressed data in M2*/
    compareCode1 = memcmp((uint8_t *)((uint32_t)dstAddr + HWA_MEMn_SIZE), (uint8_t*)HWA_EGEcompdecomp_output, 4 * numEGEInputData);
    if (compareCode1 != 0)
    {
        HWA_log("Error: Compression/decompression EGE output found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: Compression/decompression EGE output generated by HWA found correct\n");
    }
    compareCode2 += compareCode1;


    errCode = HWA_enable(handle, 0);

    HWA_log("\n");
    if (compareCode2 != 0)
    {
       // MCPI_setFeatureTestResult("HWA compression/decompression Tests", MCPI_TestResult_FAIL);
        HWA_log("Feature : HWA compression/decompression Tests FAIL\n");
        finalTestResultsPass = 0;
    }
    else
    {
       // MCPI_setFeatureTestResult("HWA compression/decompression Tests", MCPI_TestResult_PASS);
        HWA_log("Feature : HWA compression/decompression Tests PASS\n");
    }

    HWA_log("\n HWA compression/decompression test completed \n");

}
/**
*  @b Description
*  @n
*      test the fft and DC, and interference features in the pre-processing module
*
*  @retval
*      Not Applicable.
*/

static void HWA_fftwithPreproc_test(HWA_Handle handle)
{
    int32_t                 errCode, compareCode1, compareCode2;
    uint8_t                 paramsetIdx = 0;
    uint8_t                 pingParamSetIdx, pongParamSetIdx;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM1;

#ifdef TEST_PARAMSET_INTERRUPT
    HWA_InterruptConfig     paramISRConfig;
    SemaphoreP_Handle       paramSetSem;
#endif
    SemaphoreP_Handle       doneSem;
    SemaphoreP_Params       semParams;
    SemaphoreP_Status       status;
    //    HWA_Stats appHWAStats[3];
    volatile uint32_t        startTime;
    volatile uint32_t        endTime;

    //HWA_DebugStats           debugStats;
    //uint8_t                  interruptParamIdx;

    /**************************************************************************
     **************************************************************************
     * Test: DMA triggered mode
     **************************************************************************
     **************************************************************************/
    HWA_log("\n------- HWA FFT with pre-processing test ------- \n\n");
    /* Do dma trigger mode test - simple FFT - one pass */
    paramsetIdx = 0;
    memset(gHWATestParamConfig, 0, sizeof(gHWATestParamConfig));  //init to zero
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    /****************************************************************************************
    ping dummy + FFT enable with DC and interference statistics estimation
    pong dummy + FFT enable intereference localization + mitigation (all pass + all zeroout)
    FFT with shuffle addressing
    ***************************************************************************************/
    /**************************************************************************
     * Step 1 - config the paramset
     **************************************************************************/
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA; //Software triggered  - in demo this will be HWA_TRIG_MODE_DMA
    gHWATestParamConfig[paramsetIdx].triggerSrc = HWA_TEST_SRC_TRIGGER_DMACH0; //in demo this will be first EDMA Src channel id
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_NONE; //dummy
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) (DMA Trigger paramset for ping buffer, AccelMode None) returned %d\n", errCode, paramsetIdx);
       // MCPI_setFeatureTestResult("API HWA_configParamSet() [DMA Trigger paramset for ping buffer, AccelMode None]", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() [DMA Trigger paramset for ping buffer, AccelMode None] FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
        //HWA_log("Debug: API HWA_configParamSet() [DMA Trigger paramset for ping buffer, AccelMode None] Pass\n");
        HWA_log("Debug: paramset %d : DMA Trigger AccelMode None\n", paramsetIdx);
    }
    paramsetIdx++;
    pingParamSetIdx = paramsetIdx;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE; //Immediate following first - in demo this should be HWA_TRIG_MODE_DFE
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT; //do FFT
    gHWATestParamConfig[paramsetIdx].source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(srcAddr);// (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = HWA_TEST_NUM_SAMPLES - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = HWA_TEST_NUM_RX_ANT - 1; //no iterations here
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.bpmEnable = HWA_FEATURE_BIT_DISABLE;
    /* M1*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(dstAddr); //(uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = HWA_TEST_1DFFT_SIZE - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = log2Approx(HWA_TEST_1DFFT_SIZE);
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x3; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE; //enabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = HWA_TEST_1DFFT_WINDOW_START; //start of window RAM
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.winSymm = HWA_FFT_WINDOW_NONSYMMETRIC; //non-symmetric - in demo do we make this symmetric

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT; // output FFT samples
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

    /* add DC and interference statistics processing*/
    /* DC estimation */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_PARAMRESET;
    /* interference statistics */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfStat.resetMode = HWA_DCEST_INTERFSUM_RESET_MODE_PARAMRESET;

    /* add some non-zero DC subtraction value, not enable the DC sub */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCEST;
    /* add some non-zero value, not enable the feature  */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramScaleSelect = 8;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramSizeSelect = 4;

    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() [Immediate Triggered, Ping buffer FFT processing with DC and Interference Statistics ]", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() [Immediate Triggered, Ping buffer FFT processing with DC and Interference Statistics] FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
       // HWA_log("Debug: API HWA_configParamSet() [Immediate Triggered, Ping buffer FFT processing with DC and Interference Statistics] PASS\n");
        HWA_log("Debug: paramset %d : Immediate Trigger AccelMode FFT with DC, interference estimation enabled \n", paramsetIdx);
    }
    paramsetIdx++;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA; //Immediate triggered  - in demo this will be HWA_TRIG_MODE_DMA
    gHWATestParamConfig[paramsetIdx].triggerSrc = HWA_TEST_SRC_TRIGGER_DMACH1; //in demo this will be second EDMA Src channel id
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_NONE; //dummy
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() [DMA Trigger paramset for pong buffer, AccelMode None]", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() [DMA Trigger paramset for pong buffer, AccelMode None] FAIL\n");
		finalTestResultsPass = 0;
	    return;
    }
    else
    {
       // HWA_log("Debug: API HWA_configParamSet() [DMA Trigger paramset for pong buffer, AccelMode None] PASS\n");
        HWA_log("Debug: paramset %d : DMA Trigger AccelMode None\n", paramsetIdx);
    }
    paramsetIdx++;
    pongParamSetIdx = paramsetIdx;
    gHWATestParamConfig[paramsetIdx] = gHWATestParamConfig[pingParamSetIdx];

    /* M2*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA ((uint32_t) dstAddr + HWA_MEMn_SIZE) ;//(uint32_t)((uint32_t)dstAddr + HWA_MEMn_SIZE - SOC_HWA_MEM0); // M2

    /*disable the DC estimation */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
    /* disable interference statistics */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfStat.resetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;

    /* enable interference localization, but threshold is set to maximum*/
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdEnable = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdMode = HWA_INTERFTHRESH_MODE_MAG_AND_MAGDIFF;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdSelect = HWA_INTERFTHRESH_SELECT_SW;

    /* enabled interference mitigation */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.enable = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.countThreshold = 4;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.leftHystOrder = 3;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.rightHystOrder = 3;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.pathSelect = HWA_INTERFMITIGATION_PATH_ZEROOUT;

    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered, Pong buffer FFT processing with interference localization/mitigation enabled (all pass) )", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() (Immediate Triggered, Pong buffer FFT processing with interference localization/mitigation enabled (all pass)) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
        //HWA_log("Debug: API HWA_configParamSet() (Immediate Triggered, Pong buffer FFT processing with interference localization/mitigation enabled (all pass) ) PASS\n");
        HWA_log("Debug: paramset %d : Immediate Trigger AccelMode FFT, with intereference mitigation enabled \n", paramsetIdx);
    }

    /* this paramset also enable the threshold mitigation, by zero out all the samples to zero*/
    paramsetIdx++;
    gHWATestParamConfig[paramsetIdx] = gHWATestParamConfig[pongParamSetIdx];
    /*M3*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA ( (uint32_t ) dstAddr + (2 * HWA_MEMn_SIZE));//(uint32_t)((uint32_t)dstAddr + HWA_MEMn_SIZE * 2 - SOC_HWA_MEM0); // M3

    /* enable interference localization*/
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdEnable = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdMode = HWA_INTERFTHRESH_MODE_MAGDIFF;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdSelect = HWA_INTERFTHRESH_SELECT_SW;

    /* enabled interference mitigation */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.enable = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.countThreshold = 0;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.leftHystOrder = 3;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.rightHystOrder = 3;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfMitigation.pathSelect = HWA_INTERFMITIGATION_PATH_ZEROOUT;

    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered, FFT processing with localization/interference mitigation(all zero out) )", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() (Immediate Triggered, FFT processing with localization/interference mitigation(all zero out)) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
        //HWA_log("Debug: API HWA_configParamSet() (Immediate Triggered, FFT processing with localization/interference mitigation(all zero out)) PASS\n");
        HWA_log("Debug: paramset %d : Immediate Triggered, AccelMode FFT, interference mitigation with threshold 0\n", paramsetIdx);
    }


    /*this paramset enable the fft not use linear addressing, instead of using shuffle addressing*/
    paramsetIdx++;
    gHWATestParamConfig[paramsetIdx] = gHWATestParamConfig[pingParamSetIdx];

    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_COMPLEX_16BIT_SIZE;
    gHWATestParamConfig[paramsetIdx].source.shuffleMode = HWA_SRC_SHUFFLE_AB_MODE_ADIM;  //A dimension shuffle
    gHWATestParamConfig[paramsetIdx].source.wrapComb = HWA_TEST_NUM_SAMPLES * HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = HWA_TEST_NUM_SAMPLES; //start of window RAM in terms of word

    /* DC estimation */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
    /* interference statistics */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.interfStat.resetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;

    /* add some non-zero DC subtraction value, not enable the DC sub */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCSW;
    /* add some non-zero value, not enable the feature  */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramScaleSelect = 0;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramSizeSelect = 0;

    /* and enable the channel combine, but not combine any data*/
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.chanCombEn = HWA_FEATURE_BIT_ENABLE;
    /* insert a zero*/
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.zeroInsertEn = HWA_FEATURE_BIT_ENABLE;

    /* mem4*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA ( (uint32_t) dstAddr + 3 * HWA_MEMn_SIZE);//(uint32_t)((uint32_t)dstAddr + HWA_MEMn_SIZE * 3 - SOC_HWA_MEM0); // address is relative to start of MEM0
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered, FFT enabled with shuffle addressing(wraparound) + channel combine + zeroinsert )", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() (Immediate Triggered, FFT enabled with shuffle addressing(wraparound) + channel combine + zeroinsert) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
       //HWA_log("Debug: API HWA_configParamSet() (Immediate Triggered,FFT enabled with shuffle addressing(wraparound) + channel combine + zeroinsert) PASS\n");
       HWA_log("Debug: paramset %d : Immediate Trigger AccelMode FFT with shuffle addressing \n", paramsetIdx);
    }
    /* config shuffle ram */
    {
        uint32_t ii;
        for (ii = 0; ii < HWA_TEST_NUM_SAMPLES; ii++)
        {
            shuffleIdx[ii] = ii * HWA_TEST_NUM_RX_ANT + HWA_TEST_NUM_SAMPLES * HWA_TEST_NUM_RX_ANT;
        }
    }

    errCode = HWA_configRam(handle, HWA_RAM_TYPE_SHUFFLE_RAM, (uint8_t*)shuffleIdx, HWA_TEST_NUM_SAMPLES * sizeof(uint16_t), 0);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configRam(HWA_RAM_TYPE_SHUFFLE_RAM) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configRam(HWA_RAM_TYPE_SHUFFLE_RAM)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configRam(HWA_RAM_TYPE_SHUFFLE_RAM) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /**************************************************************************
     * Step 2 - load window coefficients
     **************************************************************************/
     /* if windowing is enabled, load the window coefficients in RAM */
    errCode = HWA_configRam(handle, HWA_RAM_TYPE_WINDOW_RAM, (uint8_t *)win_data225, sizeof(win_data225), HWA_TEST_1DFFT_WINDOW_START);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /* if windowing is enabled, load the window coefficients in RAM for shuffling address */
    errCode = HWA_configRam(handle, HWA_RAM_TYPE_WINDOW_RAM, (uint8_t *)win_data225, sizeof(win_data225), sizeof(win_data225));  //in terms of byte
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM with offset not 0) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM with offset not 0)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configRam(HWA_RAM_TYPE_WINDOW_RAM with offset not 0) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

#ifdef TEST_PARAMSET_INTERRUPT
    /* Enable Paramset Interrupt for CPU and create semaphore*/
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    paramSetSem = SemaphoreP_create(0, &semParams);
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
    paramISRConfig.cpu.callbackFn = HWA_Test_ParamSetISR_Callback;
    paramISRConfig.cpu.callbackArg = paramSetSem;
    //interruptParamIdx = paramsetIdx;
    errCode = HWA_enableParamSetInterrupt(handle, paramsetIdx, &paramISRConfig);
    if (errCode != 0)
    {
        //retCode = HWA_TEST_ERROR;
        HWA_log("Error: HWA_enableParamSetInterrupt returned %d\n", errCode);
       // MCPI_setFeatureTestResult("HWA_enableParamSetInterrupt", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_enableParamSetInterrupt FAIL\n");
        return;
    }

#endif

    /**************************************************************************
     * Step 3 - Enable Interrupts
     **************************************************************************/
     /* Enable Done Interrut */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    doneSem = SemaphoreP_create(0, &semParams);
    errCode = HWA_enableDoneInterrupt(handle, 0, HWA_Test_DoneISR_Callback, doneSem);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enableDoneInterrupt returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_enableDoneInterrupt()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_enableDoneInterrupt() FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /**************************************************************************
     * Step 4 - Config Common Registers
     **************************************************************************/
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG | HWA_COMMONCONFIG_MASK_INTERFMAG_THRESHOLD |
        HWA_COMMONCONFIG_MASK_DCEST_SCALESHIFT |
        HWA_COMMONCONFIG_MASK_INTERFSUM_MAG |
        HWA_COMMONCONFIG_MASK_INTERFSUM_MAGDIFF |
        //  HWA_COMMONCONFIG_MASK_COMPLEXMULT_SCALECONST |
        HWA_COMMONCONFIG_MASK_CHANCOMB_VEC_SIZE |
        HWA_COMMONCONFIG_MASK_ZEROINSERT_NUM_MASK;
    /* mag threshold */
    gCommonConfig.interfConfig.thresholdMagSw[0] = 0xffffff;
    gCommonConfig.interfConfig.thresholdMagSw[1] = 0xffffff;
    gCommonConfig.interfConfig.thresholdMagSw[2] = 0xffffff;
    gCommonConfig.interfConfig.thresholdMagSw[3] = 0xffffff;
    /* DC estimate scale, shift*/
    gCommonConfig.dcEstimateConfig.scale = 256;
    gCommonConfig.dcEstimateConfig.shift = 2;
    /* interference estimate scale, shift */
    gCommonConfig.interfConfig.sumMagScale = 8;
    gCommonConfig.interfConfig.sumMagShift = 2;
    gCommonConfig.interfConfig.sumMagDiffScale = 8;
    gCommonConfig.interfConfig.sumMagDiffShift = 2;

    //  gCommonConfig.complexMultiplyConfig.Iscale[0] = 0xfffff;    //constant scale, in Q20 format
    //   gCommonConfig.complexMultiplyConfig.Qscale[0] = 0;

      /* channel combine */
    gCommonConfig.chanCombConfig.size = HWA_TEST_NUM_SAMPLES;
    gCommonConfig.chanCombConfig.vector[0] = 0xaaaaaaaa;
    gCommonConfig.chanCombConfig.vector[1] = 0xaaaaaaaa;
    gCommonConfig.chanCombConfig.vector[2] = 0xaaaaaaaa;
    gCommonConfig.chanCombConfig.vector[3] = 0xaaaaaaaa;
    gCommonConfig.chanCombConfig.vector[4] = 0xaaaaaaaa;
    gCommonConfig.chanCombConfig.vector[5] = 0xaaaaaaaa;
    gCommonConfig.chanCombConfig.vector[6] = 0xaaaaaaaa;
    gCommonConfig.chanCombConfig.vector[7] = 0x0;

    /* zero insert, at the end */
    gCommonConfig.zeroInsertConfig.mask[0] = 0xffffffff;
    gCommonConfig.zeroInsertConfig.mask[1] = 0xffffffff;
    gCommonConfig.zeroInsertConfig.mask[2] = 0xffffffff;
    gCommonConfig.zeroInsertConfig.mask[3] = 0xffffffff;
    gCommonConfig.zeroInsertConfig.mask[4] = 0xffffffff;
    gCommonConfig.zeroInsertConfig.mask[5] = 0xffffffff;
    gCommonConfig.zeroInsertConfig.mask[6] = 0xffffffff;
    gCommonConfig.zeroInsertConfig.mask[7] = 0x00000001;
    gCommonConfig.zeroInsertConfig.number = 1;

    gCommonConfig.paramStartIdx = 0 ;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1; //do only one iteration

    errCode = HWA_configCommon(handle, &gCommonConfig);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configCommon returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configCommon()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_enableDoneInterrupt() FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /**************************************************************************
     * Step 5 - Enable the HWA
     **************************************************************************/
    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_enable(handle, 1); // set 1 to enable
    endTime = CycleprofilerP_getTimeStamp();
    drvApiCycles[DRV_API_ENABLE] = endTime - startTime;
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enable(1) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_enable() to enable HWA", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_enable() to enable HWA FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /**************************************************************************
     * Step 6 - Reset the HWA state machine
     **************************************************************************/
    errCode = HWA_reset(handle);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_reset returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_reset()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_reset( ) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }


    /**************************************************************************
     * Prepare the input data for test vector based testing
     **************************************************************************/
     /* memcopy the data to MEM0 */
    memcpy(srcAddr, (uint8_t *)gHWATest_1DFFT_input,
        HWA_TEST_NUM_SAMPLES*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    /* m1 fft result */
    memset((uint8_t *)dstAddr, 0, HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    /* m2 fft result */
    memset((uint8_t *)((uint32_t)dstAddr + HWA_MEMn_SIZE), 0, HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    /* m3 fft result all zero*/
    memset((uint8_t *)((uint32_t)dstAddr + 2 * HWA_MEMn_SIZE), 0xffffffff, HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    /* m3 fft with shuffle address*/
    memset((uint8_t *)((uint32_t)dstAddr + 3 * HWA_MEMn_SIZE), 0, HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);


    /**************************************************************************
     * Step 7: trigger HWA
     **************************************************************************/
     /* trigger the HWA using HWA_setDMA2ACCManualTrig if triggerMode is set to DMA */
    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_setDMA2ACCManualTrig(handle, HWA_TEST_SRC_TRIGGER_DMACH0);
    endTime = CycleprofilerP_getTimeStamp();
    drvApiCycles[DRV_API_DMAMANUALTRIG] = endTime - startTime;

    if (errCode != 0)
    {
        HWA_log("Error: HWA_setDMA2ACCManualTrig(0) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_setDMA2ACCManualTrig(ch:0)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_setDMA2ACCManualTrig(ch:0) FAIL\n");
        finalTestResultsPass = 0;
		return;
    }

    errCode = HWA_setDMA2ACCManualTrig(handle, HWA_TEST_SRC_TRIGGER_DMACH1);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_setDMA2ACCManualTrig(1) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_setDMA2ACCManualTrig(ch:1)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_setDMA2ACCManualTrig(ch:1) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }


    /**************************************************************************
     * Step 7: Wait for completion
     **************************************************************************/

     /* now wait for the interrupts */
#ifdef TEST_PARAMSET_INTERRUPT
    /* first wait for the paramSet done interrupt */
    status = SemaphoreP_pend(paramSetSem, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        //retCode = HWA_TEST_ERROR;
        HWA_log("Error: SemaphoreP_pend returned %d\n", status);
    }
#endif
    /* then wait for the all paramSets done interrupt */
    status = SemaphoreP_pend(doneSem, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        HWA_log("Error: Wait for all paramsets done failed %d\n", status);
        return;
    }

    /**************************************************************************
    * check the DC and interference statistics result
    ****************************************************************************/
    errCode = HWA_readDCEstimateReg(handle, DCEstResult, 0, HWA_TEST_NUM_RX_ANT);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_readDCEstimateReg returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_readDCEstimateReg", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_readDCEstimateReg() FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /* mag results*/
    errCode = HWA_readInterfThreshReg(handle, magEstResult, 0, HWA_TEST_NUM_RX_ANT, HWA_INTERFERENCE_THRESHOLD_TYPE_MAG);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_readInterfThreshReg(MAG) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_readInterfThreshReg(MAG)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_readInterfThreshReg(MAG) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /* magdiff threshold reg check */
    errCode = HWA_readInterfThreshReg(handle, magDiffEstResult, 0, HWA_TEST_NUM_RX_ANT, HWA_INTERFERENCE_THRESHOLD_TYPE_MAGDIFF);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_readInterfThreshReg(MAGDIFF) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_readInterfThreshReg(MAGDIFF)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_readInterfThreshReg(MAGDIFF) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    errCode = HWA_readDCAccReg(handle, DCAccResult, 0, HWA_TEST_NUM_RX_ANT);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_readDCAccReg returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_readDCAccReg", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_readDCAccReg() FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    errCode = HWA_readIntfAccReg(handle, magAccResult, HWA_INTERFERENCE_THRESHOLD_TYPE_MAG, 0, HWA_TEST_NUM_RX_ANT);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_readIntfAccReg(MAG) returned %d\n", errCode);
       // MCPI_setFeatureTestResult("API HWA_readIntfAccReg(MAG)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_readIntfAccReg(MAG) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    errCode = HWA_readIntfAccReg(handle, magDiffAccResult, HWA_INTERFERENCE_THRESHOLD_TYPE_MAGDIFF, 0, HWA_TEST_NUM_RX_ANT);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_readIntfAccReg(MAGDIFF) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_readIntfAccReg(MAGDIFF)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_readIntfAccReg(MAGDIFF) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    HWA_log("\nDebug: HWA FFT with pre-processing test result check\n");
    HWA_log("\n");

    /* check the results*/
    compareCode2 = 0;
    compareCode1 = memcmp((uint8_t *)DCEstResult, (uint8_t*)gHWATest_dcest_output, 2 * HWA_TEST_NUM_RX_ANT * 4);
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA DC ESTIMATION found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA DC ESTIMATION found correct\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t *)magEstResult, (uint8_t*)gHWATest_intefMag_output, HWA_TEST_NUM_RX_ANT * 4);
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA Interference Statistics Magnitude found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA Interference Statistics Magnitude found correct\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t *)magDiffEstResult, (uint8_t*)gHWATest_intefMagDiff_output, HWA_TEST_NUM_RX_ANT * 4);
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA Interference Statistics Magnitude DIFF found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA Interference Statistics Magnitude DIFF found correct\n");
    }
    compareCode2 += compareCode1;


    compareCode1 = memcmp((uint8_t *)DCAccResult, (uint8_t*)HWA_dcAccumulator_output, 2 * HWA_TEST_NUM_RX_ANT * 8);
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA DC Accumulator found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA DC Accumulator found correct\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t *)magAccResult, (uint8_t*)HWA_magAccumulator_output, HWA_TEST_NUM_RX_ANT * 8);
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA Interference Mag Accumulator found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA Interference Mag Accumulator found correct\n");
    }

    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t *)magDiffAccResult, (uint8_t*)HWA_magDiffAccumulator_output, HWA_TEST_NUM_RX_ANT * 8);
    if (compareCode1 != 0)
    {
        HWA_log("Error: HWA  Interference Magdiff Accumulator found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: HWA  Interference Magdiff Accumulator found correct\n");
    }
    compareCode2 += compareCode1;

    /**************************************************************************
     * Check output data
     **************************************************************************/
     /* output data is ready - compare, PING fft + DC/interference  */
    compareCode1 = memcmp(dstAddr, (uint8_t *)gHWATest_1DFFT_output,
        HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);

    compareCode2 += compareCode1;
    if (compareCode1 != 0)
    {
        HWA_log("Error: fft output produced by HWA in MEM1 found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: fft output produced by HWA (paramset 1) found correct\n");
    }

    /* pong fft + interference all pass through*/
    compareCode1 = memcmp((uint8_t *)((uint32_t)dstAddr + HWA_MEMn_SIZE), (uint8_t *)gHWATest_1DFFT_output,
        HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    compareCode2 += compareCode1;

    if (compareCode1 != 0)
    {
        HWA_log("Error: fft output produced by HWA in MEM2 found incorrect: error %d\n", compareCode2);
    }
    else
    {
        HWA_log("Debug: fft output produced by HWA (paramset 3) found correct\n");
    }

     /* fft interference all zero-out */
    if ((*((uint8_t *)((uint32_t)dstAddr + HWA_MEMn_SIZE * 2))) == 0) //first one is 0
    {
        //the rest are all 0
        compareCode1 = memcmp((uint8_t *)((uint32_t)dstAddr + HWA_MEMn_SIZE * 2), (uint8_t *)((uint32_t)dstAddr + HWA_MEMn_SIZE * 2 + 1),
            HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE - 1);
    }
    else
    {
        compareCode1 = 1;
    }
    if (compareCode1 != 0)
    {
        HWA_log("Error: fft output produced by HWA in MEM3 found incorrect: error %d\n", compareCode2);
    }
    else
    {
        HWA_log("Debug: fft output produced by HWA (paramset 4) found correct\n");
    }
    compareCode2 += compareCode1;


    /* fft + shuffle addressing output */
    compareCode1 = memcmp((uint8_t *)((uint32_t)dstAddr + HWA_MEMn_SIZE * 3), (uint8_t *)gHWATest_1DFFT_output,
        HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    compareCode2 += compareCode1;
    if (compareCode1 != 0)
    {
        HWA_log("Error: fft output produced by HWA in MEM4 found incorrect: error %d\n", compareCode2);
    }
    else
    {
        HWA_log("Debug: fft output produced by HWA (paramset 5) found correct\n");
    }


    /**************************************************************************
     * Cleanup
     **************************************************************************/
    errCode = HWA_disableParamSetInterrupt(handle, paramsetIdx, HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_disableParamSetInterrupt returned %d\n", errCode);
       // MCPI_setFeatureTestResult("API HWA_disableParamSetInterrupt() ", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_readIntfAccReg(MAGDIFF) FAIL\n");
		finalTestResultsPass = 0;
    }

    errCode = HWA_disableDoneInterrupt(handle, 0);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_disableDoneInterrupt returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_disableDoneInterrupt() ", MCPI_TestResult_FAIL);
        HWA_log("Feature :  API HWA_disableDoneInterrupt() FAIL\n");
        finalTestResultsPass = 0;
    }

    /* disable HWA*/
    errCode = HWA_enable(handle, 0); // set 0 to disable
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enable(0) returned %d\n", errCode);
       // MCPI_setFeatureTestResult("API HWA_enable() to disable HWA", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_enable() to disable HWA FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

#ifdef TEST_PARAMSET_INTERRUPT
    /* cleanup semaphores */
    if (paramSetSem != NULL)
        SemaphoreP_delete(paramSetSem);
#endif
    if (doneSem != NULL)
        SemaphoreP_delete(doneSem);

    HWA_log("\n");

    /* final FFT test results*/
    if (compareCode2 != 0)
    {
        //MCPI_setFeatureTestResult("HWA FFT with pre-processing Tests", MCPI_TestResult_FAIL);
        HWA_log("Feature : HWA FFT with pre-processing Tests FAIL\n");
        finalTestResultsPass= 0;
    }
    else
    {
       // MCPI_setFeatureTestResult("HWA FFT with pre-processing Tests", MCPI_TestResult_PASS);
        HWA_log("Feature : HWA FFT with pre-processing Tests PASS\n");
    }

    HWA_log("\n HWA FFT with pre-processing test completed \n");

}

/**
*  @b Description
*  @n
*      complex multiply (HWA_COMPLEX_MULTIPLY_MODE_VECTOR_MULT) test function.
*
*  @retval
*      Not Applicable.
*/
static void HWA_complexmultiply_test(HWA_Handle handle)
{
    int32_t                 errCode, compareCode1, compareCode2;
    uint8_t                 paramsetIdx = 0;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM1;
    uint16_t                startParamIdx = 31;
    uint32_t                ii;
    uint8_t     paramsetCheckArray[16];
    uint8_t     numCheckParamSets;

    HWA_log("\n------- HWA complex multiply Tests -------\n\n");

    paramsetIdx = 0;
    memset(gHWATestParamConfig, 0, sizeof(gHWATestParamConfig));  //init to zero
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

     /*this paramset disable the FFT, vector multiplication mode 1 using data from internal ram */
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;
    gHWATestParamConfig[paramsetIdx].source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(srcAddr); //(uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = HWA_TEST_NUM_SAMPLES - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = HWA_TEST_NUM_RX_ANT - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;

    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(dstAddr); //(uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = HWA_TEST_NUM_SAMPLES - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping


    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = 0;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = 0; //disable
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = 0; //start of window RAM
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.winSymm = 0;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_VECTOR_MULT;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.vectorMultiplyMode1.vecMultiMode1RamAddrOffset = 5;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.vectorMultiplyMode1.cmultScaleEn = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr  - SOC_HWA_MEM0); // address is relative to start of MEM0
    errCode = HWA_configParamSet(handle, paramsetIdx + startParamIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", paramsetIdx + startParamIdx, errCode);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered, FFT disabled, scale multiplication mode )", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() (Immediate Triggered, AccelMode FFT, complex multiply: vector multiplication) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
        //HWA_log("Debug: API HWA_configParamSet() (Immediate Triggered, complex multiply: vector multiplication mode PASS\n");
        HWA_log("Debug: paramset %d :Immediate Triggered, AccelMode FFT, complex multiply: vector multiplication\n", paramsetIdx + startParamIdx);
    }


    /*this paramset disable the FFT, scale multiply set to magnitude squared mode*/
    paramsetIdx++;
    gHWATestParamConfig[paramsetIdx] = gHWATestParamConfig[0];
    gHWATestParamConfig[paramsetIdx].source.srcScale = 0; //left shift 8 bits from 16 bits to 24 bits
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 8;   //no shift from 24 bits to 32 bits

    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED; //


    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_SUM_STATS;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_MAG_SQUARED;

     /* mem2*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA ( (uint32_t) dstAddr + 0x4000); //(uint32_t)((uint32_t)dstAddr + HWA_MEMn_SIZE - SOC_HWA_MEM0); // address is relative to start of MEM0
    errCode = HWA_configParamSet(handle, paramsetIdx + startParamIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", errCode, paramsetIdx + startParamIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered, magnitude mode with sum statistics output )", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() (Immediate Triggered, magnitude mode with sum statistics output) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
        //HWA_log("Debug: API HWA_configParamSet() (Immediate Triggered, magnitude mode with sum statistics output PASS\n");
        HWA_log("Debug: paramset %d :Immediate Triggered, AccelMode FFT, magnitude mode with sum statistics output\n", paramsetIdx + startParamIdx);
    }

    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG;//|
                               //HWA_COMMONCONFIG_MASK_COMPLEXMULT_SCALECONST;

   // gCommonConfig.complexMultiplyConfig.Iscale[0] = 0xfffff;    //constant scale, in Q20 format
   // gCommonConfig.complexMultiplyConfig.Qscale[0] = 0;

    gCommonConfig.paramStartIdx = 0 + startParamIdx;
    gCommonConfig.paramStopIdx = paramsetIdx + startParamIdx;
    gCommonConfig.numLoops = 1; //do only one iteration

    errCode = HWA_configCommon(handle, &gCommonConfig);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configCommon returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configCommon()", MCPI_TestResult_FAIL);
        return;
    }

    /* config the vector multiply internal ram */
    for (ii = 0; ii < HWA_TEST_NUM_SAMPLES; ii++)
    {
        vectorArray[2 * ii] = 0xfffff; //Q20 1
        vectorArray[2 * ii + 1] = 0x0; //imag
    }
    errCode = HWA_configRam(handle, HWA_RAM_TYPE_VECTORMULTIPLY_RAM, (uint8_t *)vectorArray,
                  HWA_TEST_NUM_SAMPLES * 8,  //complex coefficients, with 21 bits I and 21 bits Q
                  gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.vectorMultiplyMode1.vecMultiMode1RamAddrOffset * 8);

    if (errCode != 0)
    {
        HWA_log("Error: HWA_configRam returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configRam(HWA_RAM_TYPE_VECTORMULTIPLY_RAM) ", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configRam(HWA_RAM_TYPE_VECTORMULTIPLY_RAM) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }


    /**************************************************************************
    * Prepare the input data for test vector based testing
    **************************************************************************/
    /* memcopy the data to MEM0 */
    memcpy(srcAddr, (uint8_t *)gHWATest_1DFFT_input,
        HWA_TEST_NUM_SAMPLES*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    /* m1 vector multiply results */
    memset((uint8_t *)dstAddr, 0, HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    /* m2 vector multiply results */
    memset((uint8_t *)((uint32_t) dstAddr + HWA_MEMn_SIZE), 0, HWA_TEST_NUM_RX_ANT*4*2); //for magquare, 2 words


    /**************************************************************************
    * Step 5 - Enable the HWA
    **************************************************************************/
    errCode = HWA_enable(handle, 1); // set 1 to enable
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enable(1) returned %d\n", errCode);
       // MCPI_setFeatureTestResult("API HWA_enable() to enable HWA", MCPI_TestResult_FAIL);
        return;
    }


    /* wait for hwa to finish */
    numCheckParamSets = 2;
    paramsetCheckArray[0] = startParamIdx + 0;
    paramsetCheckArray[1] = startParamIdx + 1;
    errCode = HWA_paramSetDonePolling(handle, numCheckParamSets, paramsetCheckArray);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_paramSetDonePolling() returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_paramSetDonePolling()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_paramSetDonePolling() FAIL\n");
        finalTestResultsPass = 0;
		return;
    }

    HWA_log("\n Debug: HWA complex multiply test result check \n");
    HWA_log("\n");

    /*statistics block output check, I_SUM1_LSB  */
    errCode = HWA_readStatsReg(handle, statsResults, 4);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_readStatsReg() returned %d\n", errCode);
       // MCPI_setFeatureTestResult("API HWA_readStatsReg()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_readStatsReg() FAIL \n");
        finalTestResultsPass = 0;
        return;
    }

    /* check the vector multiplication results */
    compareCode2 = 0;
    compareCode1 = memcmp((uint8_t *)gHWATest_1DFFT_input, (uint8_t*)((uint32_t)dstAddr),
        HWA_TEST_NUM_SAMPLES*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    if (compareCode1 != 0)
    {
        HWA_log("Error: output produced by HWA in MEM1 found incorrect: error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: vector multiply output produced by HWA (paramset 31) found correct\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = 0;
    for (ii = 0; ii < HWA_TEST_NUM_RX_ANT; ii++)
    { //hwa is in the reverse order
        if ((gHWATest_magSquareSum_output[2 * ii + 1] != statsResults[3 - ii].iSumLSB) || (0 != statsResults[3 - ii].iSumMSB))
            compareCode1++;
    }

    if (compareCode1 != 0)
    {
        HWA_log("Error: magnitude square sum outputs in common registers produced by HWA : error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: magnitude square sum outputs in common registers produced by HWA (paramset 32) found correct\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t *)((uint32_t)dstAddr + HWA_MEMn_SIZE), (uint8_t *)gHWATest_magSquareSum_output, sizeof(uint32_t) * 2 * HWA_TEST_NUM_RX_ANT);
    if (compareCode1 != 0)
    {
        HWA_log("Error: magnitude square sum outputs in memory produced by HWA : error %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: magnitude square sum outputs in data memory produced by HWA (paramset 32) found correct\n");
    }
    compareCode2 += compareCode1;

    /* disable HWA*/
    errCode = HWA_enable(handle, 0); // set 0 to disable
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enable(0) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_enable() to disable HWA", MCPI_TestResult_FAIL);
        return;
    }

    HWA_log("\n");
    /* final results*/
    if (compareCode2 != 0)
    {
        //MCPI_setFeatureTestResult("Immediate triggered complex multiply test ", MCPI_TestResult_FAIL);
        HWA_log("Feature : Immediate triggered complex multiply tests FAIL\n");
        finalTestResultsPass = 0;
    }
    else
    {
        //MCPI_setFeatureTestResult("Immediate triggered complex multiply test", MCPI_TestResult_PASS);
        HWA_log("Feature : Immediate triggered complex multiply tests PASS\n");
    }
    HWA_log("\nHWA complex multiple test completed\n");

    return;
}


/**
*  @b Description
*  @n
*      azim fft test function, include zero insersion, channel combine and shuffle addressing test.
*
*  @retval
*      Not Applicable.
*/
static void HWA_azimfft_test(HWA_Handle handle)
{
    int32_t                 errCode, compareCode1, compareCode2;
    uint8_t                 paramsetIdx = 0;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM1;
    uint8_t                 paramsetCheckArray[16];
    uint8_t                 numCheckParamSets;
    uint8_t                 numIterations = 2;
    uint8_t                 numInputSymbols = 12;
    uint8_t                 fftSize = 16;
    uint32_t                ii, jj;
    uint8_t                 cdfCntBitNum[16];
    uint16_t                cdfCntCdfValue[16];
    uint16_t                cdfCntHistValue[16];
    uint32_t                 ramMemAddress;
    volatile uint32_t        startTime;
    volatile uint32_t        endTime;


    HWA_log("\n------- HWA Azim FFT calculation test ------- \n\n");

    paramsetIdx = 0;
    memset(gHWATestParamConfig, 0, sizeof(gHWATestParamConfig));  //init to zero
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    /* add complex multiply with HWA_COMPLEX_MULTIPLY_MODE_VECTOR_MULT mode, with 32 bits complex input */
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;
    gHWATestParamConfig[paramsetIdx].source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(srcAddr) ;//(uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0

    gHWATestParamConfig[paramsetIdx].source.srcAcnt = numInputSymbols - 1;
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_COMPLEX_32BIT_SIZE; //32 bits complex
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = numIterations - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_32BIT_SIZE * HWA_TEST_NUM_RX_ANT;
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_32BIT; //32-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 0;
    gHWATestParamConfig[paramsetIdx].source.shuffleMode = HWA_SRC_SHUFFLE_AB_MODE_ADIM;  //A dimension shuffle
    gHWATestParamConfig[paramsetIdx].source.wrapComb = 0;

    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = fftSize - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_COMPLEX_32BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = HWA_TEST_COMPLEX_32BIT_SIZE * fftSize; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 8;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = log2Approx(fftSize);
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x0; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_DISABLE; //disable

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.bpmEnable = HWA_FEATURE_BIT_DISABLE;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT; // output FFT samples

    /* channel combine */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.chanCombEn = HWA_FEATURE_BIT_ENABLE;
    /* insert a zero*/
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.zeroInsertEn = HWA_FEATURE_BIT_ENABLE;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_VECTOR_MULT;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.vectorMultiplyMode1.cmultScaleEn = HWA_FEATURE_BIT_ENABLE;

    /* mem1*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(dstAddr);//(uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() (Software Triggered, AccelMode FFT, with channel combine, and zero insert )", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() (Software Triggered, AccelMode FFT, with channel combine, and zero insert) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
       // HWA_log("Debug: API HWA_configParamSet() (Software Triggered, azim FFT PASS\n");
        HWA_log("Debug: paramset %d Software Triggered, AccelMode FFT, with channel combine, and zero insert\n", paramsetIdx);
    }

    /* configure shuffle index */
    shuffleIdx[0] = 0;
    shuffleIdx[1] = 1;
    shuffleIdx[2] = 2;
    shuffleIdx[3] = 4 * 8;
    shuffleIdx[4] = 4 * 8 + 1;
    shuffleIdx[5] = 3;
    shuffleIdx[6] = 4 * 8 + 2;
    shuffleIdx[7] = 4 * 8 + 3;
    shuffleIdx[8] = 4 * 16;
    shuffleIdx[9] = 4 * 16 + 1;
    shuffleIdx[10] = 4 * 16 + 2;
    shuffleIdx[11] = 4 * 16 + 3;
    errCode = HWA_configRam(handle, HWA_RAM_TYPE_SHUFFLE_RAM, (uint8_t*)shuffleIdx, numInputSymbols * sizeof(uint16_t), 0);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configRam(HWA_RAM_TYPE_SHUFFLE_RAM) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configRam(HWA_RAM_TYPE_SHUFFLE_RAM)", MCPI_TestResult_FAIL);
        HWA_log("Feature: API HWA_configRam(HWA_RAM_TYPE_SHUFFLE_RAM) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /* random pick some input data */
    for (jj = 0; jj < numIterations; jj++)
    {
        for (ii = 0; ii < numInputSymbols; ii++)
        {
            fft_input_32bits[2 * shuffleIdx[ii] + jj * HWA_TEST_NUM_RX_ANT * 2] = (int32_t)((int16_t)gHWATest_1DFFT_input[2 * ii]);       //imag
            fft_input_32bits[2 * shuffleIdx[ii] + 1 + jj * HWA_TEST_NUM_RX_ANT * 2] = (int32_t)((int16_t)gHWATest_1DFFT_input[2 * ii + 1]);   //real
        }
    }

    /* copy input data */
    memcpy(srcAddr, (uint8_t *)fft_input_32bits, sizeof(fft_input_32bits));

    /* second histogram calculation */
    paramsetIdx++;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;
    //M2
    gHWATestParamConfig[paramsetIdx].source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA((uint32_t) dstAddr + 0x4000); //(uint32_t)((uint32_t)dstAddr + HWA_MEMn_SIZE - SOC_HWA_MEM0); // address is relative to start of MEM0

    gHWATestParamConfig[paramsetIdx].source.srcAcnt = 16 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = 4;
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = 256 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = 16 * 4;
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //real data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //un-signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8; //0

    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = 16 - 1;
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = 2;
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = 16 * 2;
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED;
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn =  HWA_FFT_MODE_MAGNITUDE_LOG2_ENABLED;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;

    /* enable the 2D maximum */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.max2Denable = HWA_FEATURE_BIT_ENABLE;

    /* histogram with cdf threshold */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramMode = HWA_HISTOGRAM_MODE_CDF_THRESHOLD;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramScaleSelect = 11;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramSizeSelect = 6;
    /* M3*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr =  HWADRV_ADDR_TRANSLATE_CPU_TO_HWA((uint32_t) dstAddr + 2 * 0x4000); //(uint32_t)((uint32_t)dstAddr + 2 * HWA_MEMn_SIZE - SOC_HWA_MEM0);

    /* memcopy the data to MEM2 */
    memcpy((uint8_t *)((uint32_t)dstAddr + HWA_MEMn_SIZE), (uint8_t *)gHWATest_1DFFT_input, 256 * 16 * 4);
    /* clear M3*/
    memset((uint8_t *)((uint32_t)dstAddr + 2 * HWA_MEMn_SIZE), 0, 256 * 16 * 4);


    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered,AccelMode FFT with CDF calculation, and 2D maximum enabled)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() (Immediate Triggered,AccelMode FFT with CDF calculation, and 2D maximum enabled) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
       // HWA_log("Debug: API HWA_configParamSet() (Immediate Triggered,CDF calculation PASS\n");
        HWA_log("Debug: paramset %d Immediate Triggered,AccelMode FFT with CDF calculation, and 2D maximum enabled\n", paramsetIdx);

    }

    /* configure common register */
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG |
        HWA_COMMONCONFIG_MASK_CHANCOMB_VEC_SIZE |
        HWA_COMMONCONFIG_MASK_ZEROINSERT_NUM_MASK |
        HWA_COMMONCONFIG_MASK_COMPLEXMULT_SCALEARRAY |
        HWA_COMMONCONFIG_MASK_CDFCNT_THRESHOLD |
        HWA_COMMONCONFIG_MASK_MAX2D_OFFSETBOTHDIM |
        HWA_COMMONCONFIG_MASK_TWIDDITHERENABLE |
        HWA_COMMONCONFIG_MASK_LFSRSEED;

    gCommonConfig.fftConfig.twidDitherEnable = 1;
    gCommonConfig.fftConfig.lfsrSeed = 0x1234567;

    /* 2D maximum value offset, visually check the values written into the register */
    gCommonConfig.advStatConfig.max2DoffsetDim1 = -1;
    gCommonConfig.advStatConfig.max2DoffsetDim2 = 1;

    /* histogram calculation */
    gCommonConfig.advStatConfig.cdfCntThresh = 110;

    /* channel combine */
    gCommonConfig.chanCombConfig.size = 9;
    gCommonConfig.chanCombConfig.vector[0] = 0xA6D;
    gCommonConfig.chanCombConfig.vector[1] = 0x0;
    gCommonConfig.chanCombConfig.vector[2] = 0x0;
    gCommonConfig.chanCombConfig.vector[3] = 0x0;
    gCommonConfig.chanCombConfig.vector[4] = 0x0;
    gCommonConfig.chanCombConfig.vector[5] = 0x0;
    gCommonConfig.chanCombConfig.vector[6] = 0x0;
    gCommonConfig.chanCombConfig.vector[7] = 0x0;

    /* zero insert, at the end */
    gCommonConfig.zeroInsertConfig.mask[0] = 0x5DF;
    gCommonConfig.zeroInsertConfig.mask[1] = 0x0;
    gCommonConfig.zeroInsertConfig.mask[2] = 0x0;
    gCommonConfig.zeroInsertConfig.mask[3] = 0x0;
    gCommonConfig.zeroInsertConfig.mask[4] = 0x0;
    gCommonConfig.zeroInsertConfig.mask[5] = 0x0;
    gCommonConfig.zeroInsertConfig.mask[6] = 0x0;
    gCommonConfig.zeroInsertConfig.mask[7] = 0x0;
    gCommonConfig.zeroInsertConfig.number = 2;

    /* vector multiplication vector */
    for (ii = 0; ii < 12; ii++)
    {
        gCommonConfig.complexMultiplyConfig.Iscale[ii] = 0xFFFFF; //Q20 format
        gCommonConfig.complexMultiplyConfig.Qscale[ii] = 0;
    }

    gCommonConfig.paramStartIdx = 0;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1;

    errCode = HWA_configCommon(handle, &gCommonConfig);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configCommon returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configCommon()", MCPI_TestResult_FAIL);
        return;
    }


    /* clear the histgoram*/
    errCode = HWA_clearHistogramRAM(handle);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_clearHistgoramRAM(1) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_clearHistgoramRAM() to clear Histogram RAM", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_clearHistgoramRAM() to clear Histogram RAM FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /**************************************************************************
    * Step 5 - Enable the HWA
    **************************************************************************/
    errCode = HWA_enable(handle, 1); // set 1 to enable
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enable(1) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_enable() to enable HWA", MCPI_TestResult_FAIL);
        return;
    }


    /* trigger the HWA manually using HWA_setSoftwareTrigger if triggerMode is set to Software */
    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
    endTime = CycleprofilerP_getTimeStamp();
    drvApiCycles[DRV_API_SOFTWARETRIG] = endTime - startTime;
    if (errCode != 0)
    {
        //retCode = HWA_TEST_ERROR;
        HWA_log("Error: HWA_setSoftwareTrigger returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_setSoftwareTrigger()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_setSoftwareTrigger() FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /* wait for hwa to finish */
    numCheckParamSets = 2;
    paramsetCheckArray[0] = 0;  //azim fft
    paramsetCheckArray[1] = 1; //histogram
    errCode = HWA_paramSetDonePolling(handle, numCheckParamSets, paramsetCheckArray);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_paramSetDonePolling() returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_paramSetDonePolling()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_paramSetDonePolling() FAIL\n");
        finalTestResultsPass = 0;
        return;
    }


    HWA_log("\nDebug:  HWA Azim FFT test result check\n");
    HWA_log("\n");
    /* compare the results*/
    compareCode2 = 0;
    for (ii = 0; ii < numIterations; ii++)
    {
        compareCode1 = memcmp((uint8_t *)((uint32_t)dstAddr + ii * HWA_TEST_COMPLEX_32BIT_SIZE * fftSize), (uint8_t*)gHWATest_azimfft_output, HWA_TEST_COMPLEX_32BIT_SIZE * fftSize);
        if (compareCode1 != 0)
        {
            HWA_log("Error: output produced by HWA Azim %d Iteration found incorrect: error %d\n", ii, compareCode1);
        }
        else
        {
            HWA_log("Debug: HWA Azim FFT %d Iteration found correct (paramset 0)\n", ii);
        }
        compareCode2 += compareCode1;
    }
    ramMemAddress = HWA_getRamAddress(HWA_RAM_TYPE_HISTOGRAM_RAM);
    compareCode1 = memcmp((uint8_t *)ramMemAddress, (uint8_t*)gHWATest_histogram_output, 64 * 16);
    if (compareCode1 != 0)
    {
        HWA_log("Error: output produced by HWA Histogram found incorrect: error %d\n", compareCode1);

    }
    else
    {
        HWA_log("Debug: output produced by HWA Histogram found correct (paramset 1)\n");

    }
    compareCode2 += compareCode1;


    /* check the cdf threshold value*/
    errCode = HWA_readHistThresholdRam(handle, cdfThresholdResults, 16, 0);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_readHistThresholdRam() returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_readHistThresholdRam()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_readHistThresholdRam() FAIL\n");
        finalTestResultsPass = 0;
        return;
    }

    /*verify the structure definition*/
    for (ii = 0; ii < 16; ii++)
    {
        cdfCntBitNum[ii] = cdfThresholdResults[ii].binNumber;
        cdfCntCdfValue[ii] = cdfThresholdResults[ii].cdfValue;
        cdfCntHistValue[ii] = cdfThresholdResults[ii].pdfValue;
    }
    compareCode1 = memcmp((uint8_t*)cdfCntBitNum, (uint8_t*)HWA_cdfBitNum_output, 16);
    if (compareCode1 != 0)
    {
        HWA_log("Error: CDF threshold bit number returned %d\n", compareCode1);
      //  MCPI_setFeatureTestResult("CDF threshold bit number", MCPI_TestResult_FAIL);
        HWA_log("Feature: CDF threshold bit number FAIL\n");
		finalTestResultsPass = 0;
		return;
    }
    else
    {
        HWA_log("Debug: CDF threshold bit number output by HWA found correct (paramset 1)\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t*)cdfCntCdfValue, (uint8_t*)HWA_cdfCdfValue_output, 16 * 2);
    if (compareCode1 != 0)
    {
        HWA_log("Error: CDF threshold CDF value returned %d\n", compareCode1);
        //MCPI_setFeatureTestResult("CDF threshold CDF value ", MCPI_TestResult_FAIL);
        HWA_log("Feature : CDF threshold CDF value FAIL\n");
        finalTestResultsPass = 0;
    }
    else
    {
        HWA_log("Debug: CDF threshold CDF value output by HWA found correct (paramset 1)\n");
    }
    compareCode2 += compareCode1;

    compareCode1 = memcmp((uint8_t*)cdfCntHistValue, (uint8_t*)HWA_cdfPdfValue_output, 16 * 2);
    if (compareCode1 != 0)
    {
        HWA_log("Error: CDF threshold PDF value returned %d\n", compareCode1);
        //MCPI_setFeatureTestResult("CDF threshold PDF value ", MCPI_TestResult_FAIL);
        HWA_log("Feature : CDF threshold PDF value FAIL\n");
		finalTestResultsPass = 0;
		return;
    }
    else
    {
        HWA_log("Debug: CDF threshold PDF value output by HWA found correct (paramset 1)\n");
    }
    compareCode2 += compareCode1;

    ramMemAddress = HWA_getRamAddress(HWA_RAM_TYPE_2DSTAT_ITER_IDX);
    compareCode1 = memcmp((uint8_t *)ramMemAddress, (uint8_t *)HWA_2Dmax_dim1_maxIdx_output, sizeof(uint16_t) * 256);
    if (compareCode1 != 0)
    {
        HWA_log("Error: 2D maximum Iteration max location idx returned %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: 2D maximum Iteration max location idx output by HWA found correct (paramset 1)\n");
    }
    compareCode2 += compareCode1;

    ramMemAddress = HWA_getRamAddress(HWA_RAM_TYPE_2DSTAT_ITER_VAL);
    compareCode1 = memcmp((uint8_t *)ramMemAddress, (uint8_t *)HWA_2Dmax_dim1_maxValue_output, sizeof(uint32_t) * 256);
    if (compareCode1 != 0)
    {
        HWA_log("Error: 2D maximum Iteration max value returned %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: 2D maximum Iteration max value output by HWA found correct (paramset 1)\n");
    }
    compareCode2 += compareCode1;

    ramMemAddress = HWA_getRamAddress(HWA_RAM_TYPE_2DSTAT_SAMPLE_IDX);
    compareCode1 = memcmp((uint8_t *)ramMemAddress, (uint8_t *)HWA_2Dmax_dim2_maxIdx_output, sizeof(uint16_t) * 16);
    if (compareCode1 != 0)
    {
        HWA_log("Error: 2D maximum sample max location idx returned %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: 2D maximum sample max location idx value output by HWA found correct (paramset 1)\n");
    }
    compareCode2 += compareCode1;

    ramMemAddress = HWA_getRamAddress(HWA_RAM_TYPE_2DSTAT_SAMPLE_VAL);
    compareCode1 = memcmp((uint8_t *)ramMemAddress, (uint8_t *)HWA_2Dmax_dim2_maxValue_output, sizeof(uint32_t) * 16);
    if (compareCode1 != 0)
    {
        HWA_log("Error: 2D maximum sample max Value returned %d\n", compareCode1);
    }
    else
    {
        HWA_log("Debug: 2D maximum sample max value output by HWA found correct (paramset 1)\n");
    }
    compareCode2 += compareCode1;


    /* disable HWA*/
    errCode = HWA_enable(handle, 0); // set 0 to disable
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enable(0) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_enable() to disable HWA", MCPI_TestResult_FAIL);
        return;
    }

    HWA_log("\n");
    /* final results*/
    if (compareCode2 != 0)
    {
        //MCPI_setFeatureTestResult("Software triggered azim FFT test ", MCPI_TestResult_FAIL);
        HWA_log("Feature : Software triggered azim FFT test FAIL\n");
        finalTestResultsPass = 0;
    }
    else
    {
        //MCPI_setFeatureTestResult("Software triggered azim FFT test ", MCPI_TestResult_PASS);
        HWA_log("Feature : Software triggered azim FFT test PASS\n");
    }

    HWA_log("\nHWA azim FFT test completed\n");
    return;
}

/**
*  @b Description
*  @n
*      Histogram test to test the paramsetDone is properly genereated using polling driver function.
*
*  @retval
*      Not Applicable.
*/
void HWA_histogram_test(HWA_Handle handle)
{
    int32_t                 errCode, compareCode1, compareCode2;
    uint8_t                 paramsetIdx = 0;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM1;


    HWA_log("\n ------- HWA Histogram test ------- \n\n");

    paramsetIdx = 0;
    memset(gHWATestParamConfig, 0, sizeof(gHWATestParamConfig));  //init to zero
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    /* histogram calculation */
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;
    gHWATestParamConfig[paramsetIdx].source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA (srcAddr);//(uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0

    gHWATestParamConfig[paramsetIdx].source.srcAcnt = 5 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = 2;
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = 256 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = 16 * 2;
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_REAL; //real data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_UNSIGNED; //un-signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8; //0

    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = 5 - 1;
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = 2;
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = 16 * 2;
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED;
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_ONLY_ENABLED; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;

    /* enable the 2D maximum */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.max2Denable = HWA_FEATURE_BIT_ENABLE;

    /* histogram with cdf threshold */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramMode = HWA_HISTOGRAM_MODE_CDF_THRESHOLD;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramScaleSelect = 11;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramSizeSelect = 6;

    gHWATestParamConfig[paramsetIdx].dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA (dstAddr);//(uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0);

    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configParamSet(%d) returned %d\n", errCode, paramsetIdx);
        //MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered, AccelMode FFT, Histogram calculation)", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_configParamSet() (Immediate Triggered, AccelMode FFT, Histogram calculation) FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    else
    {
       // MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered,FFT enabled enabled with vector multiply)", MCPI_TestResult_PASS);
        HWA_log("Debug: paramset %d Immediate Triggered, AccelMode FFT, Histogram calculation\n", paramsetIdx);
    }

    /* configure common register */
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG | HWA_COMMONCONFIG_MASK_CDFCNT_THRESHOLD;
    gCommonConfig.advStatConfig.cdfCntThresh = 140;

    gCommonConfig.paramStartIdx = 0;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1;

    errCode = HWA_configCommon(handle, &gCommonConfig);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_configCommon returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_configCommon()", MCPI_TestResult_FAIL);
        return;
    }

    /* memcopy the data to MEM0 */
    memcpy(srcAddr, (uint8_t *)gHWATest_1DFFT_input,  256*16*2 );

    memset((uint8_t *)dstAddr, 0, 256 * 16 * 2);

    /* clear the histgoram*/
    errCode = HWA_clearHistogramRAM(handle);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_clearHistgoramRAM(1) returned %d\n", errCode);
       // MCPI_setFeatureTestResult("API HWA_clearHistgoramRAM() to clear Histogram RAM", MCPI_TestResult_FAIL);
        return;
    }



    /**************************************************************************
    * Step 5 - Enable the HWA
    **************************************************************************/
    errCode = HWA_enable(handle, 1); // set 1 to enable
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enable(1) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_enable() to enable HWA", MCPI_TestResult_FAIL);
        return;
    }

    /* trigger the HWA manually using HWA_setSoftwareTrigger if triggerMode is set to Software */
    errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
    if (errCode != 0)
    {
        //retCode = HWA_TEST_ERROR;
        HWA_log("Error: HWA_setSoftwareTrigger returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_setSoftwareTrigger()", MCPI_TestResult_FAIL);
        return;
    }


    /* wait for hwa to finish */
    errCode = HWA_singleParamSetDonePolling(handle, paramsetIdx);
    if (errCode != 0)
    {
        HWA_log("Error: HWA_paramSetDonePolling() returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_paramSetDonePolling()", MCPI_TestResult_FAIL);
        return;
    }


     /*HWA stuck for this parameters, all the functionality result check are done in HWA_azimfft_test
       so for this test, not check the output, just check if hwa can run to finish
     */
    compareCode2 = 0;
    compareCode1 = 0;

    compareCode2 += compareCode1;

    HWA_log("\nHWA run to complete, paramdone is generated \n");
    /* disable HWA*/
    errCode = HWA_enable(handle, 0); // set 0 to disable
    if (errCode != 0)
    {
        HWA_log("Error: HWA_enable(0) returned %d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_enable() to disable HWA", MCPI_TestResult_FAIL);
        return;
    }

    HWA_log("\n");
    /* final results*/
    if (compareCode2 != 0)
    {
        //MCPI_setFeatureTestResult("Software triggered Histogram test ", MCPI_TestResult_FAIL);
        HWA_log("Feature : Software triggered Histogram test FAIL\n");
		finalTestResultsPass = 0; 
    }
    else
    {
        //MCPI_setFeatureTestResult("Software triggered Histogram test ", MCPI_TestResult_PASS);
        HWA_log("Feature : Software triggered Histogram test PASS\n");
    }

    HWA_log("\nHWA Histogram test completed\n");
    return;

}

#ifdef  HWA_TEST_COLLECTBENCHMARK
/**
*  @b Description
*  @n
*      benchmark function to collect benchmarks.
*
*  @retval
*      Not Applicable.
*/
void HWA_benchmark()
{
    volatile uint32_t     startTime;
    volatile uint32_t     endTime;
    uint32_t ii;
    HWA_Handle              handle;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM1;
    uint32_t                ramAddr;
    int32_t                 errCode;
    uint32_t                fftSize, cfarLength, localMaxCcnt, localMaxBcnt, histgramBcnt,histgramAcnt;
    uint8_t                 paramsetIdx = 0;
    uint32_t                dataSizeInByte;
    uint32_t               *srcWordAddr = (uint32_t*)SOC_HWA_MEM0;
#ifdef BUILD_MCU
    HWA_OpenConfig          hwaCfg;
    hwaCfg.interruptPriority.backgroundDone = 1U;
    hwaCfg.interruptPriority.ALTDone = 1U;
    hwaCfg.interruptPriority.paramsetDone1 = 1U;
    hwaCfg.interruptPriority.paramsetDone2 = 1U;

#endif

    startTime = CycleprofilerP_getTimeStamp();
#ifdef BUILD_MCU
    handle = HWA_open(0,
    		&hwaCfg,
    		&errCode);
#endif
#ifdef BUILD_DSP_1
    handle = HWA_open(0,
        		NULL,
        		&errCode);
#endif
    endTime = CycleprofilerP_getTimeStamp();
    drvApiCycles[DRV_API_OPEN] = endTime - startTime;

    if (handle == NULL)
    {
        HWA_log("Error: Unable to open the HWA Instance err:%d\n", errCode);
    }

    HWA_log("\n\n----------------  HWA Benchmark ------------------------\n\n");

    /* memcpy to and from L2 DSP to M0, 1k or 2k*/
    HWA_log("------ Data copied from L2 DSP to M0 benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 2; ii < 15; ii++)
    {
        startTime = CycleprofilerP_getTimeStamp();
        memcpy(srcAddr, (uint8_t *)benchmarkData, (1 << ii));
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", (1 << ii), endTime - startTime);
    }
    HWA_log("\n");

    HWA_log("------Data copied from M0 to DSP L2 benchmark: ------\n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 2; ii < 15; ii++)
    {
        startTime = CycleprofilerP_getTimeStamp();
        memcpy((uint8_t *)benchmarkData, srcAddr, (1 << ii));
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", (1 << ii), endTime - startTime);
    }
    HWA_log("\n");


    HWA_log("------one word read/copied back to M0 from L2 benchmark: ------\n");
    startTime = CycleprofilerP_getTimeStamp();
    *srcWordAddr = (++(*srcWordAddr));
    endTime = CycleprofilerP_getTimeStamp();
    HWA_log("cycle is %d\n", endTime - startTime);
    HWA_log("\n");

    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_WINDOW_RAM);
    HWA_log("------ Data from L2 copied to window ram benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 4; ii < 14; ii++)
    {
        dataSizeInByte = (1 << ii);
        startTime = CycleprofilerP_getTimeStamp();
        memcpy((uint8_t*)ramAddr, benchmarkData, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");

    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_VECTORMULTIPLY_RAM);
    HWA_log("------ Data from L2 copied to vector multiply ram benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 4; ii < 14; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy((uint8_t*)ramAddr, benchmarkData, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");

    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_LUT_FREQ_DEROTATE_RAM);
    HWA_log("------ Data from L2 copied to frequency derotate ram benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 2; ii < 9; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy((uint8_t*)ramAddr, benchmarkData, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");

    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_SHUFFLE_RAM);
    HWA_log("------ Data from L2 copied to shuffle ram benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 2; ii < 10; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy((uint8_t*)ramAddr, benchmarkData, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");

    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_HIST_THRESH_RAM);
    HWA_log("------ Data read from histogram threshold RAM to L2 benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 2; ii < 9; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy(benchmarkData, (uint8_t*)ramAddr, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");

    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_HISTOGRAM_RAM);
    HWA_log("------ Data read from histogram RAM to L2 benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 4; ii < 14; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy(benchmarkData, (uint8_t*)ramAddr, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");

    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_2DSTAT_ITER_VAL);
    HWA_log("------ Data read from 2D iteration maxima value RAM to L2 benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 4; ii < 13; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy(benchmarkData, (uint8_t*)ramAddr, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");

    HWA_log("------ Data write to 2D iteration maxima value RAM from L2 benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 4; ii < 13; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy((uint8_t*)ramAddr, benchmarkData, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");


    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_2DSTAT_ITER_IDX);
    HWA_log("------ Data read from 2D iteration maxima index RAM to L2 benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 4; ii < 12; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy(benchmarkData, (uint8_t*)ramAddr, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");

    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_2DSTAT_SAMPLE_VAL);
    HWA_log("------ Data read from 2D sample maxima value RAM to L2 benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 4; ii < 11; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy(benchmarkData, (uint8_t*)ramAddr, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");


    HWA_log("------ Data write to 2D sample maxima value RAM from L2 benchmark:------ \n");
    HWA_log("DataSizeInBytes \t cycles \n");
    for (ii = 4; ii < 11; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy((uint8_t*)ramAddr, benchmarkData,  dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");



    ramAddr = HWA_getRamAddress(HWA_RAM_TYPE_2DSTAT_SAMPLE_IDX);
    HWA_log("------ Data copied from 2D sample maxima value index RAM to L2 benchmark:------ \n");
    for (ii = 4; ii < 10; ii++)
    {
        dataSizeInByte = 1 << ii;
        startTime = CycleprofilerP_getTimeStamp();
        memcpy(benchmarkData, (uint8_t*)ramAddr, dataSizeInByte);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", dataSizeInByte, endTime - startTime);
    }
    HWA_log("\n");

    /* memcpy to and from L1 DSP to M0, 1k or 2k for DSP*/
    /*TODO: cache or not cache */
    HWA_log("------ Data copied from L1D to M0 benchmark:------ \n");
    HWA_log("DataSizeInBytes \t L1D-M0 cycles \n");
#ifdef BUILD_DSP_1
    for (ii = 2; ii < 15; ii++)
#endif
#ifdef BUILD_MCU
    for (ii = 2; ii < 14; ii++)
#endif
    {
        startTime = CycleprofilerP_getTimeStamp();
        memcpy(srcAddr, (uint8_t *)benchmarkL1Data, (1 << ii));
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", (1 << ii), endTime - startTime);
    }
    HWA_log("\n");


    HWA_log("DataSizeInBytes \t M0-L1D cycles \n");
#ifdef BUILD_DSP_1
    for (ii = 2; ii < 15; ii++)
#endif
#ifdef BUILD_MCU
    for (ii = 2; ii < 14; ii++)
#endif
    {
        startTime = CycleprofilerP_getTimeStamp();
        memcpy((uint8_t *)benchmarkL1Data, srcAddr, (1 << ii));
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d  \t%d\n", (1 << ii), endTime - startTime);
    }
    HWA_log("\n");

    /* DSP to HWA param + common , compared with 18 (modify 18 test bench )*/
    HWA_log("------ HWA FFT mode benchmark:------\n");

    fftSize = 256;
    memset((void *)&gHWATestParamConfig[paramsetIdx], 0, sizeof(HWA_ParamConfig));
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;

    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = fftSize - 1;
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = 1 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.bpmEnable = HWA_FEATURE_BIT_DISABLE;
    /* M1*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = fftSize - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = fftSize * HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = log2Approx(fftSize);
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x3; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE; //enabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = HWA_TEST_1DFFT_WINDOW_START; //start of window RAM
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.winSymm = HWA_FFT_WINDOW_NONSYMMETRIC; //non-symmetric - in demo do we make this symmetric
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT; // output FFT samples

    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_setSourceAddress(handle, paramsetIdx, gHWATestParamConfig[paramsetIdx].source.srcAddr);
    endTime = CycleprofilerP_getTimeStamp();
    drvApiCycles[DRV_API_SETSRCADDR] = endTime - startTime;

    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    endTime = CycleprofilerP_getTimeStamp();
    HWA_log("write to paramset cycles:  %d\n", endTime - startTime);

    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));
    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG;

    gCommonConfig.paramStartIdx = paramsetIdx;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1; //do only one iteration

    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_configCommon(handle, &gCommonConfig);
    endTime = CycleprofilerP_getTimeStamp();
    HWA_log("write to common register cycles:  %d\n", endTime - startTime);

    errCode = HWA_configRam(handle, HWA_RAM_TYPE_WINDOW_RAM, (uint8_t *)win_data225, sizeof(win_data225), HWA_TEST_1DFFT_WINDOW_START);
    memcpy(srcAddr, (uint8_t *)gHWATest_1DFFT_input,
        HWA_TEST_NUM_SAMPLES*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);

    HWA_log("\n");
    HWA_log("1iteration\n FFTSize \t  cycles(w/softwaretrig)\n");
    for (ii = 8; ii < 12; ii++)
    {
        fftSize = 1 << ii;
        gHWATestParamConfig[paramsetIdx].source.srcAcnt = fftSize - 1;
        gHWATestParamConfig[paramsetIdx].dest.dstAcnt = fftSize - 1; //this is samples - 1
        gHWATestParamConfig[paramsetIdx].dest.dstBIdx = fftSize * HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
        gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = log2Approx(fftSize);
        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);


        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d \t  %d\n", fftSize, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }

    //fft with size 256, upto 16 iteration
    HWA_log("\n");
    fftSize = 256;
    HWA_log("FFTSize=%d\n iterationNum \t  cycles(w/softwaretrig)\n", fftSize);


    gHWATestParamConfig[paramsetIdx].source.srcAcnt = fftSize - 1;
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = fftSize - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = fftSize * HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = log2Approx(fftSize);

    for (ii = 1; ii < 17; ii++)
    {
        gHWATestParamConfig[paramsetIdx].source.srcBcnt = ii - 1;
        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);


        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d \t  %d\n", ii, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }

    HWA_log("\n------ HWA CFAR mode benchmark:------\n");
    paramsetIdx = 1;
    memset((void *)&gHWATestParamConfig[paramsetIdx], 0, sizeof(HWA_ParamConfig));
    cfarLength = 16;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_CFAR;

    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = cfarLength - 1;
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = sizeof(uint16_t) * 2;
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = 1 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = cfarLength * sizeof (uint16_t) * 2; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_UNSIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;

    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = cfarLength - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = sizeof(uint32_t) * 2;
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED;
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 8;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    gHWATestParamConfig[paramsetIdx].accelModeArgs.cfarMode.peakGroupEn = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.cfarMode.operMode = HWA_CFAR_OPER_MODE_LOG_INPUT_COMPLEX; // HWA_CFAR_OPER_MODE_LOG_INPUT_REAL;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.cfarMode.numGuardCells = 4;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.cfarMode.nAvgDivFactor = log2Approx(16);
    gHWATestParamConfig[paramsetIdx].accelModeArgs.cfarMode.cyclicModeEn = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.cfarMode.nAvgMode = HWA_NOISE_AVG_MODE_CFAR_CA;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.cfarMode.numNoiseSamplesRight = 8 >> 1;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.cfarMode.numNoiseSamplesLeft = 8 >> 1;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.cfarMode.outputMode = HWA_CFAR_OUTPUT_MODE_I_PEAK_IDX_Q_NEIGHBOR_NOISE_VAL;

    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    endTime = CycleprofilerP_getTimeStamp();
    HWA_log("write to paramset cycles:  %d\n", endTime - startTime);

    gCommonConfig.paramStartIdx = paramsetIdx;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1; //do only one iteration

    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_configCommon(handle, &gCommonConfig);
    endTime = CycleprofilerP_getTimeStamp();
    HWA_log("write to common register cycles:  %d\n", endTime - startTime);

    HWA_log("\n");
    HWA_log("1 iteration \n CFAR length \t cycles(w/softwaretrig): \n");

    for (ii = 4; ii < 11; ii++)
    {
        cfarLength = 1 << ii;
        gHWATestParamConfig[paramsetIdx].source.srcAcnt = cfarLength - 1;
        gHWATestParamConfig[paramsetIdx].dest.dstAcnt = cfarLength - 1; //this is samples - 1
        gHWATestParamConfig[paramsetIdx].source.srcBIdx = cfarLength * sizeof(uint16_t) * 2; //should be dont care
        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);

        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log(" %d \t  %d\n", cfarLength, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");

    HWA_log("CFAR length = 256\n ItemrationNumb \t cycles(w/softwaretrig): \n");

    cfarLength = 256;
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = cfarLength - 1;
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = cfarLength - 1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = cfarLength * sizeof(uint16_t) * 2; //should be dont care

    for (ii = 1; ii < 17; ii++)
    {
        gHWATestParamConfig[paramsetIdx].source.srcBcnt = ii - 1;
        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);

        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log(" %d \t  %d\n", ii, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");


    HWA_log("------ HWA local maxima mode benchmark:------\n");

    paramsetIdx = 2;
    memset((void *)&gHWATestParamConfig[paramsetIdx], 0, sizeof(HWA_ParamConfig));
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_LOCALMAX;

    localMaxCcnt = 256;
    localMaxBcnt = 16;
    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = 3 - 1; //fixed
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = localMaxBcnt * sizeof(uint16_t);
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = localMaxBcnt /4;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = 8; //fixed
    gHWATestParamConfig[paramsetIdx].source.srcBcircShiftWrap = mathUtils_floorLog2(localMaxBcnt / 4);
    gHWATestParamConfig[paramsetIdx].source.srcCIdx = gHWATestParamConfig[paramsetIdx].source.srcAIdx;
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_32BIT;
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_UNSIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 0;

    gHWATestParamConfig[paramsetIdx].source.wrapComb = localMaxBcnt * localMaxCcnt * 2;
    gHWATestParamConfig[paramsetIdx].source.srcCcnt = localMaxCcnt - 1;

    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = 2 - 1; //(64+31)>>5 - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = 4;
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = 2 * 4; //(64+31)>>5 * 4
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    gHWATestParamConfig[paramsetIdx].accelModeArgs.localMaxMode.neighbourBitmask = 0x55;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.localMaxMode.thresholdBitMask = HWA_LOCALMAX_THRESH_BITMASK_BOTH_EN;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.localMaxMode.thresholdMode = HWA_LOCALMAX_THRESH_SELECT_DIMBREG_DIMCREG;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.localMaxMode.dimBNonCyclic = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.localMaxMode.dimCNonCyclic = HWA_FEATURE_BIT_DISABLE;
    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    endTime = CycleprofilerP_getTimeStamp();
    HWA_log("write to paramset cycles:  %d\n", endTime - startTime);

    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));
    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG |
                               HWA_COMMONCONFIG_MASK_LOCALMAXDIMBTHRESH_OFFSET |
                               HWA_COMMONCONFIG_MASK_LOCALMAXDIMCTHRESH_OFFSET |
                               HWA_COMMONCONFIG_MASK_LOCALMAXDIMB_THRESHOLDSW |
                               HWA_COMMONCONFIG_MASK_LOCALMAXDIMC_THRESHOLDSW;

    gCommonConfig.paramStartIdx = paramsetIdx;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1; //do only one iteration
    gCommonConfig.localMaxConfig.dimBBaseAddress = 0;
    gCommonConfig.localMaxConfig.dimCBaseAddress = 0;
    gCommonConfig.localMaxConfig.dimBThreshold = 28000;
    gCommonConfig.localMaxConfig.dimCThreshold = 28000;

    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_configCommon(handle, &gCommonConfig);
    endTime = CycleprofilerP_getTimeStamp();
    HWA_log("write to common register cycles:  %d\n", endTime - startTime);

    HWA_log("fix Bcnt = %d/4 \n", localMaxBcnt);
    HWA_log("Ccount length \t cycles(w/softwaretrig)\n");

    for (ii = 3; ii < 11; ii++)
    {
        localMaxCcnt = (1 << ii);
        gHWATestParamConfig[paramsetIdx].source.wrapComb = localMaxBcnt * localMaxCcnt * 2;
        gHWATestParamConfig[paramsetIdx].source.srcCcnt = localMaxCcnt - 1;

        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);

        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d \t  %d\n", localMaxCcnt, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");

    localMaxBcnt = 64;
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = localMaxBcnt * sizeof(uint16_t);
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = localMaxBcnt / 4;
    gHWATestParamConfig[paramsetIdx].source.srcBcircShiftWrap = mathUtils_floorLog2(localMaxBcnt / 4);
    gHWATestParamConfig[paramsetIdx].source.wrapComb = localMaxBcnt * localMaxCcnt * 2;

    HWA_log("fix Bcnt = %d/4 \n", localMaxBcnt);
    HWA_log("Ccount length \t cycles(w/softwaretrig)\n");

    for (ii = 3; ii < 11; ii++)
    {
        localMaxCcnt = (1 << ii);
        gHWATestParamConfig[paramsetIdx].source.wrapComb = localMaxBcnt * localMaxCcnt * 2;
        gHWATestParamConfig[paramsetIdx].source.srcCcnt = localMaxCcnt - 1;

        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);

        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d \t  %d\n", localMaxCcnt, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");


    HWA_log("------ HWA Histogram CDF only Calculation benchmark:------\n");
    paramsetIdx = 3;
    memset((void *)&gHWATestParamConfig[paramsetIdx], 0, sizeof(HWA_ParamConfig));
    histgramBcnt = 256;
    histgramAcnt = 16;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;

    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0);
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = histgramAcnt - 1;
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = 4;
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = histgramBcnt - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = histgramAcnt * 4;
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //real data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //un-signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8; //0

    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = histgramAcnt - 1;
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = 2;
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = histgramAcnt * 2;
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED;
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_ENABLED;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;

    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.max2Denable = HWA_FEATURE_BIT_DISABLE;

    /* histogram with cdf threshold */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramMode = HWA_HISTOGRAM_MODE_CDF;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramScaleSelect = 11;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.postProcCfg.histogramSizeSelect = 6;
    /* M3*/
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0);

    //errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);


    memcpy(srcAddr, (uint8_t *)gHWATest_1DFFT_input,
        HWA_TEST_NUM_SAMPLES*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);

    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));
    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG;

    gCommonConfig.paramStartIdx = paramsetIdx;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1; //do only one iteration
    //errCode = HWA_configCommon(handle, &gCommonConfig);

    HWA_log("Acount fixed to 16\n");
    HWA_log("Bcount length \t cycles(w/softwaretrig)\n");

    for (ii = 3; ii < 11; ii++)
    {
        histgramBcnt = (1 << ii);
        gHWATestParamConfig[paramsetIdx].source.srcBcnt = histgramBcnt - 1;

        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);

        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d \t  %d\n", histgramBcnt, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");

    histgramBcnt = 256;
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = histgramBcnt - 1;
    HWA_log("Bcount fixed to %d\n", histgramBcnt);
    HWA_log("Acount  \t cycles(w/softwaretrig)\n");
    for (ii = 1; ii < 7; ii++)
    {
        histgramAcnt = (1 << ii);
        gHWATestParamConfig[paramsetIdx].source.srcAcnt = histgramAcnt - 1;
        gHWATestParamConfig[paramsetIdx].source.srcBIdx = histgramAcnt * 4;
        gHWATestParamConfig[paramsetIdx].dest.dstAcnt = histgramAcnt - 1;
        gHWATestParamConfig[paramsetIdx].dest.dstBIdx = histgramAcnt * 2;

        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);

        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d \t  %d\n", histgramAcnt, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");



    HWA_log("------ HWA Histogram with CDF Calculation benchmark:------\n");
    paramsetIdx = 4;
    gHWATestParamConfig[paramsetIdx] = gHWATestParamConfig[paramsetIdx - 1];

    histgramAcnt = 16;
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = histgramAcnt - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = histgramAcnt * 4;
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = histgramAcnt - 1;
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = histgramAcnt * 2;

    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));
    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG |
                               HWA_COMMONCONFIG_MASK_CDFCNT_THRESHOLD;

    gCommonConfig.paramStartIdx = paramsetIdx;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1; //do only one iteration
    /* histogram calculation */
    gCommonConfig.advStatConfig.cdfCntThresh = 110;
    HWA_log("Acount fixed to %d \n", histgramAcnt);
    HWA_log("Bcount length \t cycles(w/softwaretrig)\n");

    for (ii = 3; ii < 11; ii++)
    {
        histgramBcnt = (1 << ii);
        gHWATestParamConfig[paramsetIdx].source.srcBcnt = histgramBcnt - 1;
        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        gCommonConfig.advStatConfig.cdfCntThresh = histgramBcnt / 2;
        errCode = HWA_configCommon(handle, &gCommonConfig);

        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d \t  %d\n", histgramBcnt, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");


    histgramBcnt = 256;
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = histgramBcnt - 1;
    HWA_log("Bcount fixed to 256\n");
    HWA_log("Acount  \t cycles(w/softwaretrig)\n");
    for (ii = 1; ii < 7; ii++)
    {
        histgramAcnt = (1 << ii);
        gHWATestParamConfig[paramsetIdx].source.srcAcnt = histgramAcnt - 1;
        gHWATestParamConfig[paramsetIdx].source.srcBIdx = histgramAcnt * 4;
        gHWATestParamConfig[paramsetIdx].dest.dstAcnt = histgramAcnt - 1;
        gHWATestParamConfig[paramsetIdx].dest.dstBIdx = histgramAcnt * 2;

        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);

        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log("%d \t  %d\n", histgramAcnt, endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");


    HWA_log("------ HWA compression mode benchmark:------\n");
    /* compression EGE */
    paramsetIdx = 5;
    memset((void *)&gCommonConfig, 0, sizeof(HWA_CommonConfig));

    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_COMPRESS;

    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = 16 - 1; //16 complex samples
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = HWA_TEST_1DFFT_SIZE/16 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = 16 * HWA_TEST_COMPLEX_16BIT_SIZE; //16 bits
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;

    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = 8 - 1; // 8 words
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = 4;  // 1 words
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = 4 * 8 ; // next compress block
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    /* two path EGE */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.ditherEnable = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.compressDecompress = HWA_CMP_DCMP_COMPRESS;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.method = HWA_COMPRESS_METHOD_EGE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.passSelect = HWA_COMPRESS_PATHSELECT_BOTHPASSES;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.headerEnable = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.scaleFactorBW = 4; //log2(sample bits)
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.EGEKarrayLength = 3;

    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
    endTime = CycleprofilerP_getTimeStamp();
    HWA_log("write to paramset cycles:  %d\n", endTime - startTime);

    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG | HWA_COMMONCONFIG_MASK_EGECOMRESS_KPARAM;

    gCommonConfig.compressConfig.EGEKparam[0] = 0;
    gCommonConfig.compressConfig.EGEKparam[1] = 2;
    gCommonConfig.compressConfig.EGEKparam[2] = 4;
    gCommonConfig.compressConfig.EGEKparam[3] = 6;
    gCommonConfig.compressConfig.EGEKparam[4] = 8;
    gCommonConfig.compressConfig.EGEKparam[5] = 10;
    gCommonConfig.compressConfig.EGEKparam[6] = 12;
    gCommonConfig.compressConfig.EGEKparam[7] = 15;

    gCommonConfig.paramStartIdx = paramsetIdx;
    gCommonConfig.paramStopIdx = paramsetIdx;
    gCommonConfig.numLoops = 1; //do only one iteration

	memcpy(srcAddr, (uint8_t*)gHWATest_4KFFT_input, sizeof(uint32_t) * 4096);
    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_configCommon(handle, &gCommonConfig);
    endTime = CycleprofilerP_getTimeStamp();
    HWA_log("write to common register cycles:  %d\n", endTime - startTime);

    HWA_log("\n EGE compress: 16 16bits-complex values (16 * 4 bytes)--> 8 words (8 * 4bytes) \n");
    HWA_log("number of compressed blocks \t cycles(w/softwaretrig): \n");
    for (ii = 2; ii < 9; ii++)
    {
        gHWATestParamConfig[paramsetIdx].source.srcBcnt = (1 << ii) - 1;
        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);
        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log(" %d \t  %d\n", (1 << ii), endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");

    /* EGE decompression */
    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint32_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = 8 - 1; // 8 words
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = 4;  // 1 words
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = HWA_TEST_1DFFT_SIZE / 16 - 1;
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = 4 * 8; // next compress block
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_REAL; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_32BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_UNSIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;

    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint32_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = 16 - 1; //16 complex samples
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = 16 * HWA_TEST_COMPLEX_16BIT_SIZE; //16 bits
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    /* two path EGE */
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.ditherEnable = HWA_FEATURE_BIT_DISABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.compressDecompress = HWA_CMP_DCMP_DECOMPRESS;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.method = HWA_COMPRESS_METHOD_EGE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.passSelect = HWA_COMPRESS_PATHSELECT_BOTHPASSES;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.headerEnable = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.scaleFactorBW = 4; //log2(sample bits)
    gHWATestParamConfig[paramsetIdx].accelModeArgs.compressMode.EGEKarrayLength = 3;

    HWA_log("\n EGE decompress: 8 words (8 * 4bytes) ---> 16 16bits-complex values (16 * 4 bytes)  \n");
    HWA_log("number of decompressed blocks \t cycles(w/softwaretrig): \n");
    for (ii = 2; ii < 9; ii++)
    {
        gHWATestParamConfig[paramsetIdx].source.srcBcnt = (1 << ii) - 1;
        errCode = HWA_configParamSet(handle, paramsetIdx, &gHWATestParamConfig[paramsetIdx], NULL);
        errCode = HWA_configCommon(handle, &gCommonConfig);
        errCode = HWA_enable(handle, 1);
        startTime = CycleprofilerP_getTimeStamp();
        errCode = HWA_setSoftwareTrigger(handle);  //through DSP software trigger
        HWA_singleParamSetDonePolling(handle, paramsetIdx);
        endTime = CycleprofilerP_getTimeStamp();
        HWA_log(" %d \t  %d\n", (1 << ii), endTime - startTime);
        errCode = HWA_enable(handle, 0);
    }
    HWA_log("\n");



    HWA_log("------ HWA Driver API benchmark:------\n");
    HWA_log("HWA_init: %d\n", drvApiCycles[DRV_API_INIT]);
    HWA_log("HWA_open: %d\n", drvApiCycles[DRV_API_OPEN]);
    HWA_log("HWA_close: %d\n", drvApiCycles[DRV_API_CLOSE]);
    HWA_log("HWA_enable: %d\n", drvApiCycles[DRV_API_ENABLE]);
    HWA_log("HWA_setSoftwareTrigger: %d\n", drvApiCycles[DRV_API_SOFTWARETRIG]);
    HWA_log("HWA_setDMA2ACCManualTrig: %d\n", drvApiCycles[DRV_API_DMAMANUALTRIG]);
    HWA_log("HWA_setSourceAddress: %d\n", drvApiCycles[DRV_API_SETSRCADDR]);
    HWA_log("\n");

    errCode = HWA_close(handle);

    return;
}
#endif
/**
*  @b Descriptions
*  @n
*      System Initialization Task which initializes the various
*      components in the system.
*
*  @retval
*      Not Applicable.
*/
static void Test_initTask(void* arg0, void* arg1)
{
    HWA_Handle              handle;
    int32_t                 errCode;
    volatile uint32_t       startTime;
    volatile uint32_t       endTime;


    /**************************************************************************
    * Initialize the HWA
    **************************************************************************/
    startTime = CycleprofilerP_getTimeStamp();
    HWA_init();
    endTime = CycleprofilerP_getTimeStamp();
    drvApiCycles[DRV_API_INIT] = endTime - startTime;

    HWA_log("Debug: HWA has been initialized\n");

    /**************************************************************************
    * Open the HWA Instance
    **************************************************************************/
    handle = HWA_open(0,
    		NULL,  // use default interrupt priority for ARM, and NULL for DSP always
    		&errCode);
    if (handle == NULL)
    {
        HWA_log("Error: Unable to open the HWA Instance err:%d\n", errCode);
        //MCPI_setFeatureTestResult("API HWA_open()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_open() FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    HWA_log("Debug: HWA Instance %p has been opened successfully\n", handle);

    /**************************************************************************
    * Test: Graceful shutdown
    **************************************************************************/
    errCode = HWA_close(handle);
    if (errCode)
    {
        HWA_log("Error: HWA Instance %p closed with err:%d\n", handle, errCode);
        //MCPI_setFeatureTestResult("API HWA_close()", MCPI_TestResult_FAIL);
        HWA_log("Feature : API HWA_close() FAIL\n");
		finalTestResultsPass = 0;
        return;
    }
    HWA_log("Debug: HWA Instance %p closed successfully\n", handle);

    /**************************************************************************
    * Test: Reopen the driver
    **************************************************************************/
    handle = HWA_open(0,
    		NULL,   // use default interrupt priority for ARM, and NULL for DSP always
    		&errCode);
    if (handle == NULL)
    {
        HWA_log("Error: Unable to open the HWA Instance err:%d\n", errCode);
        //MCPI_setFeatureTestResult("HWA reopen", MCPI_TestResult_FAIL);
        HWA_log("Feature : HWA reopen FAIL\n");
        finalTestResultsPass = 0;
        return;
    }
    HWA_log("Debug: HWA Instance %p has been reopened successfully\n", handle);


    /* test fft with preprocessing */
    HWA_fftwithPreproc_test(handle);

    /* test complex multiply*/
    errCode = HWA_reset(handle);
    HWA_complexmultiply_test(handle);

    /* test the azim fft calculation*/
    errCode = HWA_reset(handle);
    HWA_azimfft_test(handle);

    /* test histogram only */
    errCode = HWA_reset(handle);
    HWA_histogram_test(handle);

    /* test compression/decompression */
    errCode = HWA_reset(handle);
    HWA_compress_test(handle);

    /* 2D fft test */
    errCode = HWA_reset(handle);
    HWA_2dfft_test(handle);

    /*fft stitching test */
    HWA_reset(handle);
    HWA_fft4k_test(handle);

    /* test context switch */
    HWA_contextswitch_test(handle);
    HWA_log("\n");


    HWA_controlPeripheralSuspendMode(handle, 0); //disable the peripheral suspende

    /* Close the driver: */
    startTime = CycleprofilerP_getTimeStamp();
    errCode = HWA_close(handle);
    endTime = CycleprofilerP_getTimeStamp();
    drvApiCycles[DRV_API_CLOSE] = endTime - startTime;
    if (errCode != 0) {
        HWA_log("Error: HWA_close() failed with err:%d\n", errCode);
        //MCPI_setFeatureTestResult("HWA_close()", MCPI_TestResult_FAIL);
    }
    else
    {
       HWA_log("Debug: HWA Instance %p has been closed successfully\n", handle);
    }

    //MCPI_setTestResult();


#ifdef HWA_TEST_COLLECTBENCHMARK
    HWA_benchmark();
#endif

    if (finalTestResultsPass == 1)
	{
		HWA_log("ALL HWA TESTS PASS\n");
	}
	else
	{
		HWA_log("HWA TESTS FAIL\n");
	}
    HWA_log("\n\n------- HWA driver test completed ----------\n\n");

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
    TaskP_Params     taskParams;
    Board_initCfg   boardCfg;
    Board_STATUS  boardStatus;

    OS_init();

    boardCfg = BOARD_INIT_UNLOCK_MMR |
               BOARD_INIT_UART_STDIO |
               BOARD_INIT_MODULE_CLOCK;

    boardStatus = Board_init (boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        HWA_log ("Debug: Board Init Fail \n");
    }

    CycleprofilerP_init();

    /* Initialize the Task Parameters. */
    TaskP_Params_init(&taskParams);
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    TaskP_create(&Test_initTask, &taskParams);

    /* Debug Message: */
    HWA_log ("Debug: Launching BIOS \n");

    OS_start();
    return 0;
}

