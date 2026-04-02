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

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>

#include <ti/board/board.h>
#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/edma/edma.h>
/* SPI test include files */
#include "mss_Utils.h"

/**************************************************************************
 *************************** Local Definitions *********************************
 **************************************************************************/
#define MIBSPI_log          printf
/**************************************************************************
 *************************** Global Definitions ********************************
 **************************************************************************/
/* System DMA handle, created in init Task */
EDMA_Handle          gDmaHandle     = NULL;
enum MibSpi_InstanceId gMibspiInst = MIBSPI_INST_ID_MSS_SPIA;

/*
 *  ======== Board_initMIBSPI ========
 */
static bool Board_initMIBSPI(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  status;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
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
static void Test_initTask(UArg arg0, UArg arg1)
{
    int32_t         retVal = 0;
    MibSpi_HwCfg cfg;
    int32_t i;

    if (Board_initMIBSPI() != true)
    {
        MIBSPI_log("Board_initMIBSPI() failed\n");
        return;
    }
    retVal = MIBSPI_socGetInitCfg(gMibspiInst, &cfg);
    if (retVal < 0)
    {
        MIBSPI_log("SPI_socGetInitCfg failed with error=%d\n", retVal);
        return;
    }
#ifdef MIBSPI_DMA_ENABLE
    EDMA_instanceInfo_t instanceInfo;
    int32_t errorCode;

    gDmaHandle = EDMA_getHandle(cfg.edmaCCId, &instanceInfo);
    if(gDmaHandle == NULL)
    {
        EDMA3CCInitParams 	initParam;

        EDMA3CCInitParams_init(&initParam);
        initParam.initParamSet = TRUE;
        if (EDMA_init(cfg.edmaCCId, &initParam) != EDMA_NO_ERROR)
        {
            MIBSPI_log("EDMA_init failed \n");
            return;
        }
        /* Open DMA driver instance 0 for SPI test */
        gDmaHandle = EDMA_open(cfg.edmaCCId, &errorCode, &instanceInfo);
    }
    if(gDmaHandle == NULL)
    {
        MIBSPI_log("Open DMA driver failed with error=%d\n", retVal);
        return;
    }
#else
    gDmaHandle = NULL;
#endif

    /* Initialize the SPI */
    MSS_UtilsSPIInit();

    while (1)
    {
        MIBSPI_log("Enter the Testcase to Run:\n");
        MIBSPI_log("\t 1 : Slave SPI read test (PC read, Sensor Write) \n");
        MIBSPI_log("\t 2 : Slave SPI write test (Sensor read, PC Write) \n");
        MIBSPI_log("\t 3 : Slave SPI read and write test\n");

        scanf("%d",&i);
        if (i == 1)
        {
            spiReadTest();
            break;
        }
        else if (i == 2)
        {
            spiWriteTest();
            break;
        }
        else if (i == 3)
        {
            spiReadWriteTest();
            break;
        }
        else
        {
            MIBSPI_log("Invalid Input\n");
        }
    }

    MIBSPI_log("Debug: SPI Test is done!\n");

    /* After test all done, terminate DSP by calling BIOS_exit().
        This is required by MCPI test framework script.
        MCPI test framework script waits for DSP halt,
        if DSP doesn't halt, script will wait until timeout then claim test fail.
     */
    BIOS_exit(0);

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
    Task_Params      taskParams;

    /* Debug Message: */
    MIBSPI_log ("******************************************\n");
    MIBSPI_log ("Debug: MibSPI Driver Slavemode Test Start \n");
    MIBSPI_log ("******************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();

    return 0;

}
