/**
 *  \file   main_gpmc_probing_example.c
 *
 *  \brief  Probing test application main file. This application will write the data
 *          to GPMC SRAM memory using CPU multiple times to enable probing
 *
 */

/*
 * Copyright (C) 2016 -2020 Texas Instruments Incorporated - http://www.ti.com/
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

#include <stdio.h>
#include <string.h>

/* GPMC Header files */
#include <ti/drv/gpmc/GPMC.h>
#include <ti/drv/gpmc/soc/GPMC_soc.h>
#include <ti/drv/gpmc/test/src/GPMC_board.h>
#include <ti/drv/gpmc/test/src/GPMC_log.h>

/* Board Header files */
#include <ti/board/board.h>
#include <ti/board/src/flash/include/board_flash.h>

#include <ti/drv/sciclient/sciclient.h>
#include <ti/osal/TimerP.h>

/**********************************************************************
 ************************** Macros ************************************
 **********************************************************************/
#define GPMC_PROFILE                                    /* Enable profiling */
#define GPMC_APP_STATUS_SUCCESS     ((int32_t)(0))      /* Succes code */
#define GPMC_APP_STATUS_ERROR       (-((int32_t)1))     /* Failure code */

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
uint8_t txBuf[TEST_DATA_LEN]  __attribute__((aligned(4))) __attribute__((section(".benchmark_buffer")));
GPMC_Transaction transaction;


/**********************************************************************
 ************************** Internal functions ************************
 **********************************************************************/
/*
 *  ======== GeneratePattern ========
 */
static void GeneratePattern(uint8_t *txBuf, uint32_t length)
{
    volatile uint32_t idx;
    volatile uint8_t *txPtr = txBuf;

    for(idx = 0; idx < length; idx++)
    {
        *txPtr++ = (uint8_t)idx;
    }
}

int32_t gpmc_test()
{
    int32_t     status = GPMC_APP_STATUS_SUCCESS;
    uint32_t    i;
    GPMC_Params params;
    GPMC_Handle handle;
#ifdef GPMC_PROFILE
    uint32_t    testLen = TEST_DATA_LEN;
    uint64_t    startTime;                  /* start time stamp in usec */
    uint64_t    elapsedTime;                /* elapsed time in usec */
    float       xferRate;
    uint32_t    xferRateInt;
#endif

    GPMC_Params_init(&params);
    handle = GPMC_open(BOARD_GPMC_INSTANCE, &params);
    if (handle == NULL)
    {
        GPMC_log("\n GPMC open failed. \n");
        status = GPMC_APP_STATUS_ERROR;
        goto Err;
    }

    /* Generate the data */
    GeneratePattern(txBuf, TEST_DATA_LEN);

    /* Write data */
    transaction.transType = GPMC_TRANSACTION_TYPE_WRITE;
    transaction.offset    = 0U;
    transaction.count     = TEST_DATA_LEN;
    transaction.txBuf     = txBuf;
    transaction.rxBuf     = NULL;
    transaction.arg       = NULL;

#ifdef GPMC_PROFILE
    /* Get start time stamp for the write performance measurement */
    startTime = TimerP_getTimeInUsecs();
#endif

    for(i=0; i<1000; i++)
    {
    	if(!GPMC_transfer(handle, &transaction))
    	{
	        GPMC_log("[Error] GPMC write failed. \n");
	        status = GPMC_APP_STATUS_ERROR;
	        break;
	    }
	}

#ifdef GPMC_PROFILE
    elapsedTime = TimerP_getTimeInUsecs() - startTime;
    /* calculate the write transfer rate in MBps */
    xferRate = (float) (((float)(testLen*i)) / elapsedTime);
    xferRateInt = (uint32_t)xferRate;
    GPMC_log("\n GPMC write %d bytes at transfer rate %d MBps \n", testLen, xferRateInt);
#endif

Err:
    if (handle != NULL)
    {
        GPMC_close(handle);
    }

    return (status);
}

#if defined(SOC_AM64X)
uint32_t GPMC_configClk(uint32_t freq)
{
    GPMC_v1_HwAttrs gpmc_cfg;
    int32_t         retVal;
    uint64_t        gpmc_clk;
    uint32_t        parClk;
    uint32_t        clkID = TISCI_DEV_GPMC0_FUNC_CLK;
    uint32_t        devID = TISCI_DEV_GPMC0;

    /* Get the default GPMC init configurations */
    GPMC_socGetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);

    retVal = Sciclient_pmModuleClkRequest(devID,
                                          clkID,
                                          TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                          TISCI_MSG_FLAG_AOP,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (retVal != CSL_PASS)
    {
        GPMC_log("\n Sciclient_pmModuleClkRequest failed");
        goto clk_cfg_exit;
    }

    /* Set parent clock */
    parClk = TISCI_DEV_GPMC0_FUNC_CLK_PARENT_HSDIV4_16FFT_MAIN_0_HSDIVOUT3_CLK;
    retVal = Sciclient_pmSetModuleClkParent(devID,
                                            clkID,
                                            parClk,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);

    if (retVal != CSL_PASS)
    {
        GPMC_log("\n Sciclient_pmSetModuleClkParent failed");
        goto clk_cfg_exit;
    }

    gpmc_clk = (uint64_t)freq;
    retVal = Sciclient_pmSetModuleClkFreq(devID,
                                          clkID,
                                          gpmc_clk,
                                          TISCI_MSG_FLAG_AOP,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);

    if (retVal != CSL_PASS)
    {
        GPMC_log("\n Sciclient_pmSetModuleClkFreq failed");
        goto clk_cfg_exit;
    }

    gpmc_clk = 0;
    retVal = Sciclient_pmGetModuleClkFreq(devID,
                                          clkID,
                                          &gpmc_clk,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (retVal != CSL_PASS)
    {
        GPMC_log("\n Sciclient_pmGetModuleClkFreq failed");
        goto clk_cfg_exit;
    }

    GPMC_log("\n GPMC CLK running at %d Hz. \n", freq);

clk_cfg_exit:
    return retVal;
}
#endif

int32_t Board_initGPMC(void)
{
    int32_t       status = GPMC_APP_STATUS_SUCCESS;
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    Board_PinmuxConfig_t gpmcPinmux;

    /* Pinmux configuration to enable probing from expansion board */
    Board_pinmuxGetCfg(&gpmcPinmux);
    gpmcPinmux.muxCfg = BOARD_PINMUX_CUSTOM;
    gpmcPinmux.expBoardMux = BOARD_PINMUX_EXP_GPMC;
    Board_pinmuxSetCfg(&gpmcPinmux);

    boardCfg = BOARD_INIT_MODULE_CLOCK  |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        GPMC_log("[Error] Board init failed!!\n");
        status = GPMC_APP_STATUS_ERROR;
    }

#if defined(SOC_AM64X)
    if (status == GPMC_APP_STATUS_SUCCESS)
    {
        status += GPMC_configClk(GPMC_MODULE_CLK_80MHZ);
        if(status != GPMC_APP_STATUS_SUCCESS)
        {
            GPMC_log("[Error] Config clock failed!!\n");
        }
    }
#endif

    GPMC_init();

    return (status);
}

void gpmc_registerDump()
{
    uint32_t baseAddr = CSL_GPMC0_CFG_BASE;
    uint32_t csOnTime, csRdOffTime, csWrOffTime, advOnTime, advRdOffTime, advWrOffTime, advAadMuxOnTime, advAadMuxRdOffTime, advAadMuxWrOffTime, weOnTime, weOffTime, oeOnTime, oeOffTime, oeAadMuxOnTime, oeAadMuxOffTime, pageBurstAccessTime, readAccessTime, writeAccessTime, rdCycleTime, wrCycleTime, wrDataOnMuxBus, cycleToCycleDelay, cycleToCycleDelaySameCS, cycleToCycleDelayDiffCS, busTurnAroundTime, csExtraDelay;

    csOnTime    = HW_RD_FIELD32(baseAddr + GPMC_CONFIG2_N(0),
                                GPMC_CONFIG2_N_CSONTIME);
    csRdOffTime = HW_RD_FIELD32(baseAddr + GPMC_CONFIG2_N(0),
                                GPMC_CONFIG2_N_CSRDOFFTIME);
    csWrOffTime = HW_RD_FIELD32(baseAddr + GPMC_CONFIG2_N(0),
                                GPMC_CONFIG2_N_CSWROFFTIME);

    advOnTime    = HW_RD_FIELD32(baseAddr + GPMC_CONFIG3_N(0),
                                 GPMC_CONFIG3_N_ADVONTIME);
    advRdOffTime = HW_RD_FIELD32(baseAddr + GPMC_CONFIG3_N(0),
                                 GPMC_CONFIG3_N_ADVRDOFFTIME);
    advWrOffTime = HW_RD_FIELD32(baseAddr + GPMC_CONFIG3_N(0),
                                 GPMC_CONFIG3_N_ADVWROFFTIME);

    advAadMuxOnTime     = HW_RD_FIELD32(baseAddr + GPMC_CONFIG3_N(0),
                                        GPMC_CONFIG3_N_ADVAADMUXONTIME);
    advAadMuxRdOffTime  = HW_RD_FIELD32(baseAddr + GPMC_CONFIG3_N(0),
                                        GPMC_CONFIG3_N_ADVAADMUXRDOFFTIME);
    advAadMuxWrOffTime  = HW_RD_FIELD32(baseAddr + GPMC_CONFIG3_N(0),
                                        GPMC_CONFIG3_N_ADVAADMUXWROFFTIME);

    weOnTime    = HW_RD_FIELD32(baseAddr + GPMC_CONFIG4_N(0),
                                GPMC_CONFIG4_N_WEONTIME);
    weOffTime   = HW_RD_FIELD32(baseAddr + GPMC_CONFIG4_N(0),
                                GPMC_CONFIG4_N_WEOFFTIME);

    oeOnTime    = HW_RD_FIELD32(baseAddr + GPMC_CONFIG4_N(0),
                                GPMC_CONFIG4_N_OEONTIME);
    oeOffTime   = HW_RD_FIELD32(baseAddr + GPMC_CONFIG4_N(0),
                                GPMC_CONFIG4_N_OEOFFTIME);

    oeAadMuxOnTime  = HW_RD_FIELD32(baseAddr + GPMC_CONFIG4_N(0),
                                    GPMC_CONFIG4_N_OEAADMUXONTIME);
    oeAadMuxOffTime = HW_RD_FIELD32(baseAddr + GPMC_CONFIG4_N(0),
                                    GPMC_CONFIG4_N_OEAADMUXOFFTIME);

    pageBurstAccessTime = HW_RD_FIELD32(baseAddr + GPMC_CONFIG5_N(0),
                                        GPMC_CONFIG5_N_PAGEBURSTACCESSTIME);
    readAccessTime      = HW_RD_FIELD32(baseAddr + GPMC_CONFIG5_N(0),
                                        GPMC_CONFIG5_N_RDACCESSTIME);
    writeAccessTime     = HW_RD_FIELD32(baseAddr + GPMC_CONFIG6_N(0),
                                        GPMC_CONFIG6_N_WRACCESSTIME);

    rdCycleTime     =  HW_RD_FIELD32(baseAddr + GPMC_CONFIG5_N(0),
                                     GPMC_CONFIG5_N_RDCYCLETIME);
    wrCycleTime     =  HW_RD_FIELD32(baseAddr + GPMC_CONFIG5_N(0),
                                     GPMC_CONFIG5_N_WRCYCLETIME);

    wrDataOnMuxBus  = HW_RD_FIELD32(baseAddr + GPMC_CONFIG6_N(0),
                                    GPMC_CONFIG6_N_WRDATAONADMUXBUS);

    cycleToCycleDelay       = HW_RD_FIELD32(baseAddr + GPMC_CONFIG6_N(0),
                                            GPMC_CONFIG6_N_CYCLE2CYCLEDELAY);
    cycleToCycleDelaySameCS = HW_RD_FIELD32(baseAddr + GPMC_CONFIG6_N(0),
                                            GPMC_CONFIG6_N_CYCLE2CYCLESAMECSEN);
    cycleToCycleDelayDiffCS = HW_RD_FIELD32(baseAddr + GPMC_CONFIG6_N(0),
                                            GPMC_CONFIG6_N_CYCLE2CYCLEDIFFCSEN);

    busTurnAroundTime   = HW_RD_FIELD32(baseAddr + GPMC_CONFIG6_N(0),
                                        GPMC_CONFIG6_N_BUSTURNAROUND);
    csExtraDelay        = HW_RD_FIELD32(baseAddr + GPMC_CONFIG2_N(0),
                                        GPMC_CONFIG2_N_CSEXTRADELAY);

    GPMC_log("csOnTime = %d",csOnTime);
    GPMC_log("csRdOffTime = %d",csRdOffTime);
    GPMC_log("csWrOffTime = %d",csWrOffTime);
    GPMC_log("advOnTime = %d",advOnTime);
    GPMC_log("advRdOffTime = %d",advRdOffTime);
    GPMC_log("advWrOffTime = %d",advWrOffTime);
    GPMC_log("advAadMuxOnTime = %d",advAadMuxOnTime);
    GPMC_log("advAadMuxRdOffTime = %d",advAadMuxRdOffTime);
    GPMC_log("advAadMuxWrOffTime = %d",advAadMuxWrOffTime);
    GPMC_log("weOnTime = %d",weOnTime);
    GPMC_log("weOffTime = %d",weOffTime);
    GPMC_log("oeOnTime = %d",oeOnTime);
    GPMC_log("oeOffTime = %d",oeOffTime);
    GPMC_log("oeAadMuxOnTime = %d",oeAadMuxOnTime);
    GPMC_log("oeAadMuxOffTime = %d",oeAadMuxOffTime);
    GPMC_log("pageBurstAccessTime = %d",pageBurstAccessTime);
    GPMC_log("readAccessTime = %d",readAccessTime);
    GPMC_log("writeAccessTime = %d",writeAccessTime);
    GPMC_log("rdCycleTime = %d",rdCycleTime);
    GPMC_log("wrCycleTime = %d",wrCycleTime);
    GPMC_log("wrDataOnMuxBus = %d",wrDataOnMuxBus);
    GPMC_log("cycleToCycleDelay = %d",cycleToCycleDelay);
    GPMC_log("cycleToCycleDelaySameCS = %d",cycleToCycleDelaySameCS);
    GPMC_log("cycleToCycleDelayDiffCS = %d",cycleToCycleDelayDiffCS);
    GPMC_log("busTurnAroundTime = %d",busTurnAroundTime);
    GPMC_log("csExtraDelay = %d",csExtraDelay);
}

int main(void)
{
    int32_t status = GPMC_APP_STATUS_SUCCESS;

    status += Board_initGPMC();
    if(status != GPMC_APP_STATUS_SUCCESS)
    {
        GPMC_log("[Error] GPMC init failed!!\n");
        goto testfail;
    }

    gpmc_registerDump();

    status += gpmc_test();

testfail:
    if (status == GPMC_APP_STATUS_SUCCESS)
    {
        GPMC_log("GPMC probing test Passed!!\n");
        GPMC_log("All tests have passed!!\n");
    }
    else
    {
        GPMC_log("GPMC probing test Failed!!\n");
        GPMC_log("Some tests have failed!!\n");
    }

    return(0);
}
