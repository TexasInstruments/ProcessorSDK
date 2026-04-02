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
 *   \file  main_mss.c
 *
 *   \brief
 *      Unit Test code for the CRC Driver which executes on the MSS
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdio.h>
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"
#include "ti/drv/crc/crc.h"
#include "crc_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define APP_TSK_STACK_MAIN              (16U * 1024U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static void Test_initTask(void* arg0, void* arg1);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static void Test_initTask(void* arg0, void* arg1)
{
    int32_t testResult = 0;
    /* Debug Message: */
    Test_crcAppPrint0 ("***********************************************\n");
    Test_crcAppPrint0 ("************** CRC Unit Tests *****************\n");
    Test_crcAppPrint0 ("***********************************************\n");

    /* Test the Basic CRC API: */
    if (Test_crcInitAPI() < 0)
    {
        Test_crcAppPrint0 ("Basic CRC API test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("Basic CRC API test Passes\n");
    }

    /* Test the Compute CRC API: */
    if (Test_crcComputeSignatureAPI() < 0)
    {
        Test_crcAppPrint0 ("Compute CRC Signature test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("Compute CRC Signature test Passes\n");
    }

    /* Test the Get CRC Signature API: */
    if (Test_crcGetSignatureAPI() < 0)
    {
        Test_crcAppPrint0 ("Get CRC Signature test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("Get CRC Signature test Passes\n");
    }

    /* Test the Cancel CRC API: */
    if (Test_crcCancelSignatureAPI() < 0)
    {
        Test_crcAppPrint0 ("Cancel CRC Signature test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("Cancel CRC Signature test Passes\n");
    }

    /*************************************************************************
     * CRC Signature for CRC Type: 16bit
     *************************************************************************/
    if (Test_crcAPI(CRC_Type_16BIT, CRC_DataLen_16_BIT) < 0)
    {
        Test_crcAppPrint0 ("CRC Type: 16bit Data Length: 16bit test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("CRC Type: 16bit Data Length: 16bit test Passes\n");
    }

    if (Test_crcAPI(CRC_Type_16BIT, CRC_DataLen_32_BIT) < 0)
    {
        Test_crcAppPrint0 ("CRC Type: 16bit Data Length: 32bit test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("CRC Type: 16bit Data Length: 32bit test Passes\n");
    }

    if (Test_crcAPI(CRC_Type_16BIT, CRC_DataLen_64_BIT) < 0)
    {
        Test_crcAppPrint0 ("CRC Type: 16bit Data Length: 64bit test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("CRC Type: 16bit Data Length: 64bit test Passes\n");
    }

    /*************************************************************************
     * CRC Signature for CRC Type: 32bit
     *************************************************************************/
    if (Test_crcAPI(CRC_Type_32BIT, CRC_DataLen_16_BIT) < 0)
    {
        Test_crcAppPrint0 ("CRC Type: 32bit Data Length: 16bit test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("CRC Type: 32bit Data Length: 16bit test Passes\n");
    }

    if (Test_crcAPI(CRC_Type_32BIT, CRC_DataLen_32_BIT) < 0)
    {
        Test_crcAppPrint0 ("CRC Type: 32bit Data Length: 32bit test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("CRC Type: 32bit Data Length: 32bit test Passes\n");
    }
    if (Test_crcAPI(CRC_Type_32BIT, CRC_DataLen_64_BIT) < 0)
    {
        Test_crcAppPrint0 ("CRC Type: 32bit Data Length: 64bit test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("CRC Type: 32bit Data Length: 64bit test Passes\n");
    }


    /*************************************************************************
     * CRC Signature for CRC Type: 64bit
     *************************************************************************/
    if (Test_crcAPI(CRC_Type_64BIT, CRC_DataLen_16_BIT) < 0)
    {
        Test_crcAppPrint0 ("CRC Type: 64bit Data Length: 16bit test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("CRC Type: 64bit Data Length: 16bit test Passes\n");
    }

    if (Test_crcAPI(CRC_Type_64BIT, CRC_DataLen_32_BIT) < 0)
    {
        Test_crcAppPrint0 ("CRC Type: 64bit Data Length: 32bit test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("CRC Type: 64bit Data Length: 32bit test Passes\n");
    }

    if (Test_crcAPI(CRC_Type_64BIT, CRC_DataLen_64_BIT) < 0)
    {
        Test_crcAppPrint0 ("CRC Type: 64bit Data Length: 64bit test Failed\n");
        testResult = -1;
    }
    else
    {
        Test_crcAppPrint0 ("CRC Type: 64bit Data Length: 64bit test Passes\n");
    }

    /* All the testing was successful: */
    if (testResult == 0)
    {
        Test_crcAppPrint0 ("All Tests PASSED\n");
    }
    else
    {
        Test_crcAppPrint0 ("All Tests did NOT Pass\n");
    }

}
#include <ti/board/board.h>
int32_t main (void)
{
    TaskP_Params     taskParams;
    Board_STATUS boardStatus;
    Board_initCfg boardCfg;

    OS_init();
    boardCfg = (BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK |
                BOARD_INIT_UART_STDIO);
    boardStatus = Board_init(boardCfg);

    DebugP_assert(boardStatus == BOARD_SOK);

    /* Initialize the Task Parameters. */
    TaskP_Params_init(&taskParams);
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    TaskP_create(&Test_initTask, &taskParams);

    OS_start();

    return(0);
}

