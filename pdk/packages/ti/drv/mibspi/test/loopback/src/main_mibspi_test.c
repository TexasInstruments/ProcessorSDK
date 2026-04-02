/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test common code for the MibSpi Driver
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
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "MIBSPI_log.h"

#include <ti/board/board.h>
#include <ti/drv/edma/edma.h>
/* SPI test include files */
#include "mibspi_test_common.h"

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

#define APP_TSK_STACK_MAIN              (16U * 1024U)
/**************************************************************************
 *************************** Local Definitions *********************************
 **************************************************************************/

/**************************************************************************
 *************************** Global Definitions ********************************
 **************************************************************************/
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));
/* System DMA handle, created in init Task */
EDMA_Handle          gDmaHandle[MIBSPI_TEST_NUM_SPIINSTANCES] = {NULL,NULL};
#ifdef BUILD_MCU1_0
enum MibSpi_InstanceId gMibspiInst[MIBSPI_TEST_NUM_SPIINSTANCES] = {MIBSPI_INST_ID_MSS_SPIA, MIBSPI_INST_ID_MSS_SPIB};
#endif
#ifdef BUILD_DSP_1
#ifdef SOC_TPR12
enum MibSpi_InstanceId gMibspiInst[MIBSPI_TEST_NUM_SPIINSTANCES] = {MIBSPI_INST_ID_RCSS_SPIA, MIBSPI_INST_ID_RCSS_SPIB};
#endif 
#endif

/* Test case global variables */
bool gXWR1xxxLoopbackTest = true;
bool gXWR1xxxSlaveReady = false;


/*
 *  ======== Board_initMIBSPI ========
 */
static bool Board_initMIBSPI(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  status;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO    |
               BOARD_INIT_MODULE_CLOCK  |
               BOARD_INIT_UNLOCK_MMR;

    status = Board_init(boardCfg);

    if (status != BOARD_SOK)
    {
        return false;
    }
    else
    {
        return true;
    }
}


/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *   @param[in] arg0               Task Arg0
 *   @param[in] arg1               Task Arg1
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(void* arg0, void* arg1)
{
    int32_t         retVal = 0;
    MibSpi_HwCfg cfg;
    int32_t i;

    if (Board_initMIBSPI() != true)
    {
        printf("Board_initMIBSPI() failed\n");
        return;
    }
    for (i = 0; i < sizeof(gMibspiInst)/sizeof(gMibspiInst[0]); i++)
    {
        retVal = MIBSPI_socGetInitCfg(gMibspiInst[i], &cfg);
        if (retVal < 0)
        {
            printf("SPI_socGetInitCfg failed with error=%d\n", retVal);
            return;
        }
#ifdef MIBSPI_DMA_ENABLE
        EDMA_instanceInfo_t instanceInfo;
        int32_t errorCode;

        gDmaHandle[i] = EDMA_getHandle(cfg.edmaCCId, &instanceInfo);
        if(gDmaHandle[i] == NULL)
        {
            EDMA3CCInitParams 	initParam;

            EDMA3CCInitParams_init(&initParam);
            initParam.initParamSet = TRUE;
            if (EDMA_init(cfg.edmaCCId, &initParam) != EDMA_NO_ERROR)
            {
                printf("EDMA_init failed \n");
                return;
            }
            /* Open DMA driver instance 0 for SPI test */
            gDmaHandle[i] = EDMA_open(cfg.edmaCCId, &errorCode, &instanceInfo);
        }
        if(gDmaHandle[i] == NULL)
        {
            printf("Open DMA driver failed with error=%d\n", retVal);
            return;
        }
#else
        gDmaHandle[i] = NULL;
#endif
    }
    /* Initialize the SPI */
    MIBSPI_init();

    /* MibSPIA slave mode analog loopback test */
    //Test_loopbackSlave_oneInstance(1);
    //MIBSPI_log("Debug: Finished API Test for SPIA!\n");

    /**************************************************************************
     * Test: One instace API test - SPIA
     **************************************************************************/
    Test_spiAPI_oneInstance(0);
    MIBSPI_log("Debug: Finished API Test for SPIA!\n");

        /**************************************************************************
     * Test: One instace API test - SPIB
         **************************************************************************/
    Test_spiAPI_oneInstance(1);
    MIBSPI_log("Debug: Finished API Test for SPIB!\n");

        /**************************************************************************
     * Test: two instaces API test - SPIA & SPIB
         **************************************************************************/
    Test_spiAPI_twoInstance();
    MIBSPI_log("Debug: Finished API Test for SPIA + SPIB!\n");

        /**************************************************************************
     * Test: LoopBack Test
         **************************************************************************/
    if(gXWR1xxxLoopbackTest)
    {
        MIBSPI_log("Debug: SPI loopback Test for SPIA!\n");

        /* MibSPIA loopback test , MibSPIA only supports one slave */
        Test_loopback_oneInstance(0U, 0U);

        MIBSPI_log("Debug: SPI loopback Test for SPIB on Slave 0!\n");

        /* MibSPIB loopback test, slave -0 */
        Test_loopback_oneInstance(1U, 0U);

        MIBSPI_log("Debug: SPI loopback Test for SPIB on Slave 1!\n");

        /* MibSPIB loopback test, slave -1 */
        Test_loopback_oneInstance(1U, 1U);

        MIBSPI_log("Debug: SPI loopback Test for SPIB on Slave 2!\n");

        /* MibSPIB loopback test, slave -2 */
        Test_loopback_oneInstance(1U, 2U);
        MIBSPI_log("Debug: Loopback test finished!\n");

#ifdef MIBSPI_DMA_ENABLE
        /* MibSPIA loopback test , MibSPIA only supports one slave */
        Test_loopback_oneInstance_callback(0U, 0U);
        MIBSPI_log("Debug: Loopback test in callback mode finished!\n");
#endif

    }


    MIBSPI_log("Debug: SPI Test is done!\n");

    /* After test all done, terminate DSP by calling BIOS_exit().
        This is required by MCPI test framework script.
        MCPI test framework script waits for DSP halt,
        if DSP doesn't halt, script will wait until timeout then claim test fail.
     */
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
    TaskP_Params      taskParams;

    OS_init();

    /* Debug Message: */
    MIBSPI_log ("******************************************\n");
    MIBSPI_log ("Debug: MibSPI Driver Test Application Start \n");
    MIBSPI_log ("******************************************\n");

    /* Initialize the Task Parameters. */
    /* Initialize the Task Parameters. */
    TaskP_Params_init(&taskParams);
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    TaskP_create(&Test_initTask, &taskParams);

    OS_start();

    return 0;

}
