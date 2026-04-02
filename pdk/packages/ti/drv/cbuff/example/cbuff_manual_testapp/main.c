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

/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the CBUFF MSS and DSS
 *      Streams a chunk of data to LVDS interface.
 *      Runs on MSS and DSS.
 *      User needs to manually verify that data is captured on DCA1000 EVM.
 *
 */

/* *************** Test definition: ***************************************************
1. User needs to configure DCA1000GUI to receive data with default settings.
2. MSS/DSS comes out of reset and initializes CBUFF driver.
3. CBUFF driver is configured for a SW triggered session.
4. MSS/DSS activates the CBUFF Session which triggers data transfer from
   local RAM to CBUFF FIFO.
5. Verify that data is captured on DCA1000.
6. It is expected to capture data on DCA1000 EVM. If not the test is considered as
   FAIL.
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* Standard Include Files. */
#include <string.h>
#include <stdio.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/CacheP.h>

#include <ti/board/board.h>
#include <ti/csl/cslr.h>
#include <ti/drv/edma/soc/edma_soc.h>

/* Cbuff Driver: */
#include <ti/drv/cbuff/cbuff.h>
#include <ti/drv/cbuff/src/cbuff_internal.h>
#include "CBUFF_log.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * @brief   This is the number of EDMA Channels which have been allocated and can be used
 * by the CBUFF Driver.
 */
#define TEST_APP_MAX_EDMA_TABLE_ENTRIES             3

/**
 * @brief
 *  The DCA1000EVM FPGA needs a minimum delay of 12ms between Bit clock starts and
 *  actual LVDS Data start to lock the LVDS PLL IP. This is documented in the DCA UG
 */
#define HSI_DCA_MIN_DELAY_MSEC                  12U

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
static uint8_t  gAppTskStackMain[4 * 1024] __attribute__((aligned(32)));

uint16_t gUserBuffer0[CSL_NEXT_MULTIPLE_OF_POW2(1024,CSL_CACHE_L1D_LINESIZE)] __attribute__ ((aligned(CSL_CACHE_L1D_LINESIZE)));

/**
 * @brief   This is the DSS_A instance of EDMA which have been used by the CBUFF Driver.
 */
volatile int8_t gInstanceId = EDMA_DRV_INST_DSS_A;

 /**
  * @brief   Global Variable which is used to test the failure of EDMA Channel allocation
  */
 uint8_t gTestEDMAChannelAllocationFailure = 0U;

 /**
  * @brief   This is a global variable which tracks the EDMA Channels which have been allocated to the
  * CBUFF.
  */
 uint8_t  gCBUFFEDMAChannelResourceCounter = 0;

/**
 * @brief   This is a global variable which tracks the SW trigger Frame Done interrupt count.
 */
 volatile uint32_t  gCBUFFSwTriggerFrameDoneCounter = 0;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


 /**
  * @brief   Global CBUFF EDMA Channel Resource Table:
  */
 static CBUFF_EDMAChannelCfg gCBUFFEDMAChannelResource [TEST_APP_MAX_EDMA_TABLE_ENTRIES] =
 {
     /* EDMA Channel Identifier, Shadow Link Channel Identifier */
     {  EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ0,  65 },
     {  16,   66 },
     {  17,   67 }
 };


/**
 *  @b Description
 *  @n
 *      Performs Board Initialization
 *
 *  @retval
 *      Success -   true
 *  @retval
 *      Error   -   false
 */
static bool Test_BoardInit(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  status;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK  |
               BOARD_INIT_UART_STDIO    |
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
 *      Registered callback function with the EDMA to be invoked on an error
 *
 *  @retval
 *      Not Applicable.
 */
void Test_edmaErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    DebugP_assert(0);
}

/**
 *  @b Description
 *  @n
 *      Registered callback function with the EDMA to be invoked on an error
 *
 *  @retval
 *      Not Applicable.
 */
void Test_edmaTransferControllerErrorCallbackFxn
(
    EDMA_Handle                         handle,
    EDMA_transferControllerErrorInfo_t* errorInfo
)
{
    DebugP_assert(0);
}

/**
 *  @b Description
 *  @n
 *      This is the registered allocation function which is hooked up with the
 *      CBUFF driver to allocate EDMA Channels
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t Test_EDMAAllocateCBUFFChannel (CBUFF_EDMAInfo* ptrEDMAInfo, CBUFF_EDMAChannelCfg* ptrEDMAChannelCfg)
{
    /* Are we testing the EDMA allocation failure mode? */
    if (gTestEDMAChannelAllocationFailure == 0U)
    {
        /* NO: Have we allocated all the EDMA channels? */
        if (gCBUFFEDMAChannelResourceCounter >= TEST_APP_MAX_EDMA_TABLE_ENTRIES)
        {
            /* Error: Exceeded the allocated table. Failure */
            DebugP_assert (0);
            return -1;
        }

        /* Special case handling: First EDMA channel which is being allocated */
        if (ptrEDMAInfo->isFirstEDMAChannel == true)
        {
            switch (ptrEDMAInfo->dmaNum)
            {
                case 0:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ0;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 65;
                    break;
                }
                case 1:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ1;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 100;
                    break;
                }
                case 2:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ2;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 101;
                    break;
                }
                case 3:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ3;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 102;
                    break;
                }
                case 4:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ4;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 103;
                    break;
                }
                case 5:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ5;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 104;
                    break;
                }
                case 6:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ6;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 105;
                    break;
                }
                default:
                {
                    DebugP_assert (0);
                    break;
                }
            }
        }
        else
        {
            /* Copy over the allocated EDMA configuration. */
            memcpy ((void *)ptrEDMAChannelCfg, (void*)&gCBUFFEDMAChannelResource[gCBUFFEDMAChannelResourceCounter],
                    sizeof(CBUFF_EDMAChannelCfg));
        }

        /* Increment the number of EDMA Channels which have been allocated */
        gCBUFFEDMAChannelResourceCounter++;
        return 0;
    }
    else
    {
        /* YES: Return a failure to indicate that the channel allocation failed. We use this
         * to ensure that the CBUFF driver correctly reports errors. */
        return -1;
    }
}

/**
 *  @b Description
 *  @n
 *      This is the registered free function which is hooked up with the
 *      CBUFF driver to free the allocated EDMA Channels
 *
 *  @retval
 *      Not applicable
 */
void Test_EDMAFreeCBUFFChannel(CBUFF_EDMAChannelCfg* ptrEDMACfg)
{
    /* Debug Message: */
    gCBUFFEDMAChannelResourceCounter--;
    return;
}

/**
 *  @b Description
 *  @n
 *      This is the registered function which is hooked up with the
 *      CBUFF driver to indicate frame done interrupt.
 *
 *  @retval
 *      Not applicable
 */
static void Test_SwTriggerFrameDone (CBUFF_SessionHandle sessionHandle)
{
    /* Increment stats*/
    gCBUFFSwTriggerFrameDoneCounter++;
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the invalid data size
 *
 *  @param[in]  edmaHandle
 *      EDMA Driver Handle
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -  <0
 */
int32_t Test_Cbuff
(
    EDMA_Handle     edmaHandle
)
{
    CBUFF_SessionCfg    sessionCfg;
    CBUFF_Handle        cbuffHandle;
    CBUFF_InitCfg       initCfg;
    int32_t             errCode;
    CBUFF_SessionHandle sessionHandle;
    uint32_t index = 0;

    /* Initialize the configuration: */
    memset ((void*)&initCfg, 0, sizeof(CBUFF_InitCfg));

    /* Populate the configuration: */
    initCfg.outputDataFmt            = CBUFF_OutputDataFmt_16bit;
    initCfg.enableECC                = 0U;
    initCfg.crcEnable                = 0U;
    initCfg.enableDebugMode          = false;
    initCfg.maxSessions              = 2U;
    initCfg.interface                = CBUFF_Interface_LVDS;
    initCfg.lvdsCfg.crcEnable        = 0U;
    initCfg.lvdsCfg.msbFirst         = 1U;
    initCfg.lvdsCfg.ddrClockMode     = 1U;
    initCfg.lvdsCfg.ddrClockModeMux  = 1U;

    /* Setup the lanes */
    initCfg.lvdsCfg.lvdsLaneEnable = 0x1U;

    /* Initialize the CBUFF Driver: */
    cbuffHandle = CBUFF_init (&initCfg, &errCode);
    if (cbuffHandle == NULL)
    {
        CBUFF_log ("Error: CBUFF Driver initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure LVDS PAD Control registers. */
    /* Probably this functionality has to be from Board library. */
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_LVDS_PAD_CTRL0 , 0x0);

    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_LVDS_PAD_CTRL1 , 0x02000000);

    /*The delay below is needed only if the DCA1000EVM is being used to capture the data traces.
      This is needed because the DCA1000EVM FPGA needs the delay to lock to the
      bit clock before they can start capturing the data correctly. */
    TaskP_sleep(HSI_DCA_MIN_DELAY_MSEC);

    /* Initialize the configuration */
    memset ((void*)&sessionCfg, 0, sizeof(CBUFF_SessionCfg));

    /* Initialize the configuration: */
    for(index = 0; index < (sizeof(gUserBuffer0)/2); index++)
    {
    	gUserBuffer0[index] = index;
    }
    CacheP_wbInv (gUserBuffer0, sizeof(gUserBuffer0));

    /* Populate the configuration: */
    sessionCfg.executionMode                      = CBUFF_SessionExecuteMode_SW;
    sessionCfg.edmaHandle                         = edmaHandle;
    sessionCfg.allocateEDMAChannelFxn             = Test_EDMAAllocateCBUFFChannel;
    sessionCfg.freeEDMAChannelFxn                 = Test_EDMAFreeCBUFFChannel;
    sessionCfg.frameDoneCallbackFxn               = Test_SwTriggerFrameDone;
    sessionCfg.dataType                           = CBUFF_DataType_REAL;
    sessionCfg.u.swCfg.userBufferInfo[0].size     = sizeof(gUserBuffer0)/2;
    sessionCfg.u.swCfg.userBufferInfo[0].address  = (uint32_t)&gUserBuffer0[0];


    /* Create the SW CBUFF Session: */
    sessionHandle = CBUFF_open (cbuffHandle, &sessionCfg, &errCode);
    if (sessionHandle == NULL)
    {
        CBUFF_log ("Error: Unable to create the session [Error code %d]\n", errCode);
        return -1;
    }

    /* Debug Message: Display the EDMA Channel Usage for the test. */
    CBUFF_log ("Debug: EDMA Channel Usage = %d\n", gCBUFFEDMAChannelResourceCounter);

    /* Activate the session: */
    if (CBUFF_activateSession (sessionHandle, &errCode) < 0)
    {
        CBUFF_log ("Error: Unable to activate the session [Error code %d]\n", errCode);
        return -1;
    }

    while (gCBUFFSwTriggerFrameDoneCounter != 1)
    {
        CBUFF_log("waiting for frameDone interrupt : %d\n", gCBUFFSwTriggerFrameDoneCounter);
        TaskP_sleep(1);
    }

    CBUFF_log("Received frameDone interrupt : %d\n", gCBUFFSwTriggerFrameDoneCounter);

    CBUFF_log("Data is transmitted over LVDS successfully.\n");


    /* Deactivate the session: */
    if (CBUFF_deactivateSession (sessionHandle, &errCode) < 0)
    {
        CBUFF_log ("Error: Unable to deactivate the session [Error code %d]\n", errCode);
        return -1;
    }

    /* Delete the session: */
    if (CBUFF_close (sessionHandle, &errCode) < 0)
    {
        CBUFF_log ("Error: Unable to delete the session [Error code %d]\n", errCode);
        return -1;
    }

    /* Sanity Check: Ensure that all the EDMA resources have been cleaned up */
    if (gCBUFFEDMAChannelResourceCounter != 0)
    {
        CBUFF_log ("Error: EDMA Channel Memory leak detected\n");
        return -1;
    }

    CBUFF_log("---End of the Test---.\n");

    return 0;
}

void Test_initTask(void* arg0, void* arg1)
{
	EDMA_Handle         edmaHandle;
	int32_t             errCode;

    EDMA_instanceInfo_t instanceInfo;

    EDMA3CCInitParams initParam;
    EDMA_errorConfig_t errorConfig;

    char instName[25];
    EDMA_getInstanceName(gInstanceId, &instName[0], sizeof(instName));
    CBUFF_log("EDMA instance #%d: %s\n", gInstanceId, instName);

    EDMA3CCInitParams_init(&initParam);
    initParam.initParamSet = TRUE;

    /* Initialize the edma instance.  */
    errCode = EDMA_init(gInstanceId, &initParam);

    if (errCode == EDMA_NO_ERROR)
    {
        /* Open the first edma Instance */
    	edmaHandle = EDMA_open(gInstanceId, &errCode, &instanceInfo);
        if (edmaHandle == NULL)
        {
            CBUFF_log("Error: Unable to open the edma Instance, erorCode = %d\n", errCode);
        }
    }

    /* Setup the EDMA Error Monitoring: */
    errorConfig.isConfigAllEventQueues              = true;
    errorConfig.isConfigAllTransferControllers      = true;
    errorConfig.isEventQueueThresholdingEnabled     = true;
    errorConfig.eventQueueThreshold                 = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
    errorConfig.isEnableAllTransferControllerErrors = true;
    errorConfig.callbackFxn                         = Test_edmaErrorCallbackFxn;
    errorConfig.transferControllerCallbackFxn       = Test_edmaTransferControllerErrorCallbackFxn;
    errCode = EDMA_configErrorMonitoring(edmaHandle, &errorConfig);
    if (errCode != EDMA_NO_ERROR)
    {
        CBUFF_log("Debug: EDMA_configErrorMonitoring() failed with errorCode = %d\n", errCode);
        return;
    }

    /* Test */
    if (Test_Cbuff (edmaHandle) < 0)
        return;

    /* Stop OS */
    OS_stop();

    return;
}

int main (void)
{
    TaskP_Params    taskParams;

    OS_init();
    Test_BoardInit();

    /* Configure HSI interface Clock */
    /* Configured by Board library. */
    /*HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_HSI_CLK_SRC_SEL, 0x444);  //Div by 2
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_HSI_DIV_VAL, 0x111);
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_HSI_CLK_GATE , 0x0);*/

    /* Select MDO source as CBUFF. By default it is set to AURORA. */
    /* Probably this functionality has to be from Board library. */
    HW_WR_REG32((CSL_TOP_CTRL_U_BASE + CSL_TOP_CTRL_MDO_CTRL), 0x10);

    /* Debug Message: */
    CBUFF_log ("***********************************************\n");
    CBUFF_log ("************** CBUFF Unit Tests ***************\n");
    CBUFF_log ("***********************************************\n");

    /* Initialize the Task Parameters. */
    TaskP_Params_init(&taskParams);
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof(gAppTskStackMain);
    TaskP_create(&Test_initTask, &taskParams);

    /* Start OS */
    OS_start();
    return 0;
}
