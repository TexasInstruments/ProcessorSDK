/*
 *  Copyright (c) Texas Instruments Incorporated 2019
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
 *  \file main_rtos.c
 *
 *  \brief Main file for TI-RTOS build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdio.h>
/* BIOS Header files */
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"

#include <ti/board/board.h>
#include <csirx_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define APP_TSK_STACK_MAIN              (20U * 1024U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void taskFxn(void * a0, void * a1);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));;
/*
 * UDMA driver objects
 */
struct Udma_DrvObj gUdmaDrvObj;

/** \brief Log enable for CSIRX Unit Test  application */
uint32_t gAppTrace = ((uint32_t) GT_INFO   |\
                      (uint32_t) GT_TraceState_Enable);
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    task = TaskP_create(&taskFxn, &taskParams);
    if(NULL == task)
    {
        OS_stop();
    }

    OS_start();    /* does not return */

    return(0);
}

static void taskFxn(void * a0, void * a1)
{
    int32_t                  dmaStatus = UDMA_SOK;
    int32_t                  testPassed = FVID2_SOK, retVal = FVID2_SOK;
    Board_initCfg            boardCfg;
    Fvid2_InitPrms           fvid2InitPrms;
    uint32_t                 instId, loopCnt;
    Udma_InitPrms            udmaInitPrms;
    Udma_DrvHandle           drvHandle = &gUdmaDrvObj;
    I2C_HwAttrs              i2cConfig;

    /* TODO: Board Init here */
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
                BOARD_INIT_UNLOCK_MMR |
                BOARD_INIT_UART_STDIO;
    retVal = Board_init(boardCfg);
    if (FVID2_SOK == retVal)
    {

        Sciclient_init(NULL);

#if defined (SOC_J721E)
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_CSI_PSILSS0,
                                       TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                       TISCI_MSG_FLAG_AOP,
                                       SCICLIENT_SERVICE_WAIT_FOREVER);
#endif
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_CSI_PSILSS0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
#endif
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_CSI_RX_IF0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_CSI_RX_IF1,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_DPHY_RX0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_DPHY_RX1,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    if (FVID2_SOK == retVal)
    {
        Fvid2InitPrms_init(&fvid2InitPrms);
        fvid2InitPrms.printFxn = &App_fvidPrint;
        Fvid2_init(&fvid2InitPrms);

        /* UDMA driver init */
        instId = CSIRX_TEST_DEF_UDMA_INST;
        UdmaInitPrms_init(instId, &udmaInitPrms);
        udmaInitPrms.printFxn = &App_dmaPrint;
        dmaStatus = Udma_init(drvHandle, &udmaInitPrms);
        if(UDMA_SOK != dmaStatus)
        {
            testPassed = FVID2_EFAIL;
            GT_0trace(gAppTrace,
                      GT_ERR,
                      APP_NAME ": UDMA Init Failed!!!\r\n");
        }
 
        if (FVID2_SOK == retVal)
        {
            /* Initialize I2C Driver */
            for(loopCnt = 0U; loopCnt < I2C_HWIP_MAX_CNT; loopCnt++)
            {
                I2C_socGetInitCfg(loopCnt, &i2cConfig);
                i2cConfig.enableIntr = BFALSE;
                I2C_socSetInitCfg(loopCnt, &i2cConfig);
            }
 
            /* Initializes the I2C */
            I2C_init();
        }
 
        if (FVID2_SOK != App_setupI2CInst())
        {
            GT_0trace(gAppTrace,
                      GT_ERR,
                      APP_NAME ": I2C Setup failed!!!\r\n");
        }
 
        if (FVID2_SOK == testPassed)
        {
            testPassed = csirxTestParser();
        }
 
        if (FVID2_SOK != testPassed)
        {
            GT_0trace(gAppTrace, GT_INFO1, " CSIRX Unit Test Failed!!!\n");
        }
 
        App_closeI2CInst();
 
        dmaStatus = Udma_deinit(drvHandle);
        if(UDMA_SOK != dmaStatus)
        {
            testPassed = FVID2_EFAIL;
            GT_0trace(gAppTrace,
                      GT_ERR,
                      APP_NAME ": UDMA De-init Failed!!!\r\n");
        }
        Fvid2_deInit(NULL);
    } 
    return;
}

