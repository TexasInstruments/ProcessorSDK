/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file dss_test_parser.c
 *
 *  \brief This file parses and initializes various input params.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <ti/board/board.h>
#if defined (SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_control.h>
#include <ti/board/src/j721e_evm/include/board_i2c_io_exp.h>
#elif defined (SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_control.h>
#include <ti/board/src/j721s2_evm/include/board_i2c_io_exp.h>
#elif defined (SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_control.h>
#include <ti/board/src/j784s4_evm/include/board_i2c_io_exp.h>
#elif defined (SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_control.h>
#include <ti/board/src/j742s2_evm/include/board_i2c_io_exp.h>
#endif
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <dss_test.h>
#include <dss_test_configs.h>
#include <dss_safety_config.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DSS_TEST_NUM_OF_TESTS           ((sizeof(gDssTestChoices) / \
                                        sizeof(gDssTestChoices[0])) - 1)
#define DSS_TEST_OUTPUT_LINE_SIZE       (100U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void DssTest_populateObject(DssTest_TestObj *testObject);
void DssTest_createObject(DssTest_TestObj *testObject);
void DssTest_printInterfaces(void);
void DssTest_printTests(DssTest_Params *testList);
int32_t DssTest_getNum(void);
void DssTest_dispCfgInit(DssTest_TestObj *testObject, uint32_t pipeCnt);
void DssTest_dctrlCfgInit(DssTest_TestObj *testObject);
void DssTest_inferDctrInfoFromTestCfg(DssTest_TestObj *testObject);
void DssTest_updateDispInfoFromTestCfg(DssTest_TestObj *testObject);
void DssTest_inferPipelineParamsFromTestCfg(DssTest_TestObj *testObject);
void DssTest_populateCompleteTestInfo(DssTest_TestObj *testObject);
void DssTest_udpateBppFromInDataFmt(uint32_t inDataFmt, uint32_t *bpp);
int32_t DssTest_powerOnModules(DssTest_TestObj *testObject);
int32_t DssTest_powerOffModules();
void DssTest_fillVPSafetyChkParams(DssDctrlTest_Cfg *dctrlCfg);
void DssTest_fillPipelineSafetyChckParams(DssTest_PipelineParams *pipelineParams, uint32_t instCnt);
int32_t DssTest_logSafetyCheckData();
void DssTest_enableDP2HDMI();

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

char gDssTestChoices[][DSS_TEST_OUTPUT_LINE_SIZE] = 
{
    "[PDK-3885]: DSS RGB16_565 test.\n\r",
    "[PDK-3884]: DSS BGR16_565 test.\n\r",
    "[PDK-3543]: DSS flip test with BGRA32.\n\r",
    "[PDK-3542]: DSS crop test with BGRA32.\n\r",
    "[PDK-3540]: DSS Overlay4 and VP4 test with BGRA32.\n\r",
    "[PDK-3310]: DSS RGB24 with buffer needed.\n\r",
    "[PDK-3314]: DSS YUV422I_UYVY with buffer needed.\n\r",
    "[PDK-3313]: DSS YUV422I_YUYV with buffer needed.\n\r",
    "[PDK-3312]: DSS YUV420 Packed test.\n\r",
    "[PDK-3886]: DSS YUV420 unpacked test.\n\r",
    "[PDK-3311]: DSS BGR24 with buffer needed.\n\r",
    "[PDK-3309]: DSS ARGB32 test.\n\r",
    "[PDK-3308]: DSS Display VID2/VIDL2 test(with BGRA32).\n\r",
    "[PDK-3544]: DSS Display RTOS run time scaling test.\n\r",
    "[PDK-4734]: DSS flip test, with RGB24 and buffer needed.\n\r",
    "Execute a Full Regression.\n\r",
};

char gDssTestInterfaces[][DSS_TEST_OUTPUT_LINE_SIZE] = 
{
    "DSI\n\r",
    "EDP(Automatic Full Regression not supported yet).\n\r",
#if defined(SOC_J721E)
    "HDMI.\n\r",
#endif    
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void DssTest_main(void)
{
    DssTest_Params *testList;
    uint32_t loopCnt, testIndex, validChoiceEntered = FALSE, keepLooping = TRUE;

    Board_initCfg boardCfg;
    
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
                BOARD_INIT_UNLOCK_MMR |
                BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    /* Take the user input for interface and test choice */
    DssTest_print("\r Please choose from the Menu, and enter your choice: \n");
    while (keepLooping == TRUE)
    {
        DssTest_print("\n\rChoices for Interfaces:\n\r");
        DssTest_printInterfaces();
        validChoiceEntered = FALSE;
        gIntfSelected = DssTest_getNum();
        if ((0 <= gIntfSelected) && ((sizeof(gDssTestInterfaces)/sizeof(gDssTestInterfaces[0])) > gIntfSelected))
        {
            testList = &(gDssTestCases[gIntfSelected]);
            while (validChoiceEntered == FALSE)
            {
                DssTest_print("\n\rChoices for Tests\n\r");
                DssTest_printTests(testList);
                testIndex = DssTest_getNum();
                if ((0U <= testIndex) && (DSS_TEST_MAX_TESTS >= testIndex))
                {
                    validChoiceEntered = TRUE;
                }
                else
                {
                    DssTest_print("Invalid choice entered...");
                    continue;
                }

                for (loopCnt=0; loopCnt < testList->numTests; loopCnt++)
                {
                    if (testIndex != DSS_TEST_MAX_TESTS)
                    {
                        DssTest_populateObject(&testList->testObj[testIndex]);
                        DssTest_createObject(&testList->testObj[testIndex]);
                        break;
                    }
                    else
                    {
                        DssTest_populateObject(&testList->testObj[loopCnt]);
                        DssTest_createObject(&testList->testObj[loopCnt]);
                    }
                }            
            }

            DssTest_print("Press 0 to exit or any other key to continue...");
            keepLooping = DssTest_getNum();
        }
        else
        {
            DssTest_print("Invalid choice entered...");
        }
    }

}

void DssTest_populateObject(DssTest_TestObj *testObject)
{
    uint32_t loopCnt;
    DssTest_populateCompleteTestInfo(testObject);
    DssTest_dctrlCfgInit(testObject);
    for (loopCnt = 0; loopCnt < testObject->numPipes; loopCnt++)
    {
        DssTest_dispCfgInit(testObject, loopCnt);
    }
}

void DssTest_createObject(DssTest_TestObj *testObject)
{
    int32_t retVal = CSL_PASS;
    Sciclient_init(NULL);
    retVal += DssTest_powerOnModules(testObject);
    if (retVal == CSL_PASS)
    {
        DssTest_displayTest(testObject);
    }

    retVal += DssTest_powerOffModules();

    if (testObject->testResult == CSL_PASS && retVal == CSL_PASS)
    {
        DssTest_print("\n\r Test %d %s has passed!!!.\n", testObject->testId, testObject->testDesc);
    }
    else
    {
        DssTest_print("\n\r Test %d  %s has failed.\n", testObject->testId, testObject->testDesc);
    }
}

void DssTest_printInterfaces()
{
    uint32_t loopCnt;

    for (loopCnt=0; loopCnt<(sizeof(gDssTestInterfaces)/sizeof(gDssTestInterfaces[0])); loopCnt++)
    {
        DssTest_print("Press %d for-> %s", loopCnt, gDssTestInterfaces[loopCnt]);
    }
}

void DssTest_printTests(DssTest_Params *testList)
{
    uint32_t loopCnt;
    
    for (loopCnt=0; loopCnt<testList->numTests; loopCnt++)
    {
        DssTest_print("Press %d for-> Id : %d - %s", loopCnt, testList->testObj[loopCnt].testId, testList->testObj[loopCnt].testDesc);
    }
    DssTest_print("Press %d to %s", DSS_TEST_MAX_TESTS, "Execute a full Regression ");
}

int32_t DssTest_getNum(void)
{
    int32_t num;

#if (DSS_TEST_UART_INPUT == 1U)
    UART_scanFmt("%d", &num);
#elif (DSS_TEST_CIO_ENABLE == 1U)
    scanf("%d", &num);
#endif

    return (num);
}

int32_t DssTest_powerOnModules(DssTest_TestObj *testObject)
{
    int32_t retVal = CSL_PASS;
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);
    if (DSS_TEST_DSI == gIntfSelected)
    {
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS_DSI0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    else if (DSS_TEST_EDP == gIntfSelected)
    {
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS_EDP0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);

        retVal += Sciclient_pmSetModuleState(TISCI_DEV_SERDES_10G0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);

        if (testObject->feature == DSS_TEST_DP_TO_HDMI)
        {
            DssTest_enableDP2HDMI();
        }
    }
    return retVal;
}

int32_t DssTest_powerOffModules()
{
    int32_t retVal = CSL_PASS;
    
    if (DSS_TEST_EDP == gIntfSelected)
    {
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS_EDP0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);

        retVal = Sciclient_pmSetModuleState(TISCI_DEV_SERDES_10G0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    else if (DSS_TEST_DSI == gIntfSelected)
    {
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS_DSI0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
        TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
        TISCI_MSG_FLAG_AOP,
        SCICLIENT_SERVICE_WAIT_FOREVER);

    return retVal;
}

void DssTest_udpateBppFromInDataFmt(uint32_t inDataFmt, uint32_t *bpp)
{
    switch (inDataFmt)
    {
        case FVID2_DF_RGB16_565: 
        case FVID2_DF_BGR16_565:
        case FVID2_DF_YUV422I_UYVY:
        case FVID2_DF_YUV422I_YUYV: 
            *bpp = 2U;
            break;
        case FVID2_DF_YUV420SP_UV:
            *bpp = 3U/2U;
            break;
        case FVID2_DF_BGR24_888:
        case FVID2_DF_RGB24_888:
            *bpp = 3U;
            break;
        case FVID2_DF_ARGB32_8888:
        case FVID2_DF_BGRA32_8888:
            *bpp = 4U;
            break;
        default:
            *bpp = 0U;
            break;
    }
}

void DssTest_inferPipelineParamsFromTestCfg(DssTest_TestObj *testObject)
{
    /* update bpp and pitch based on test format */
    uint32_t i = 0u;
    for (i = 0U; i < testObject->numPipes; i++)
    {
        DssTest_udpateBppFromInDataFmt(testObject->inDataFmt[i], &testObject->pipeConfig[i]->bpp);
        /* bpp is relevant for the the YUV420SP format and pitch varies based on packed and unpacked, 
         * therefore, it should come for config and following calculation doesn't hold for it */
        if (testObject->inDataFmt[i] != FVID2_DF_YUV420SP_UV)
        {
            testObject->pipeConfig[i]->pitch[0] = testObject->pipeConfig[i]->inWidth * (uint32_t)(testObject->pipeConfig[i]->bpp);
        }
    }
}

void DssTest_inferDctrInfoFromTestCfg(DssTest_TestObj *testObject)
{
    gDctrlCfg.dctrlDrvId = DSS_DCTRL_DRV_ID,
    gDctrlCfg.dctrlInstId = DSS_DCTRL_INST_0,
    
    pDctrlParamsInfo = &gDctrlParamsInfo;
    /* Set LCD parameters */
    if (testObject->lcdOutRes == DSS_LCD_OUT_RES_1080P)
    {
        gDctrlParamsInfo.lcdOutHeight = 1080U;
        gDctrlParamsInfo.lcdOutWidth = 1920U;
        
        /* DSI using different blanking params, therefore different pixel clock */
        gDctrlParamsInfo.pixelClk = gIntfSelected == DSS_TEST_DSI ? DSS_TEST_DSI_1080P_PIXEL_CLK : DSS_TEST_1080P_PIXEL_CLK;
    }
    else if (testObject->lcdOutRes == DSS_LCD_OUT_RES_720P)
    {
        gDctrlParamsInfo.lcdOutHeight = 720U;
        gDctrlParamsInfo.lcdOutWidth = 1280U;
        
        /* DSI using different blanking params, therefore different pixel clock */
        gDctrlParamsInfo.pixelClk = gIntfSelected == DSS_TEST_DSI ? DSS_TEST_DSI_720P_PIXEL_CLK : DSS_TEST_720P_PIXEL_CLK;
    }
    else if (testObject->lcdOutRes == DSS_LCD_OUT_RES_CUSTOM)
    {
        /* TODO : implement custom case, update base on some input from user. */
    }

    /* Set node parameters */
    gDctrlParamsInfo.vpId = testObject->vpId;
    if (testObject->vpId == CSL_DSS_VP_ID_1)
    {
        gDctrlParamsInfo.vpNodeId = DSS_DCTRL_NODE_VP1;
        gDctrlParamsInfo.overlayId = CSL_DSS_OVERLAY_ID_1;
        gDctrlParamsInfo.overlayNodeId = DSS_DCTRL_NODE_OVERLAY1;
        /* Only HDMI and EDP output possible via VP1 */
        gDctrlParamsInfo.outNodeId = gIntfSelected == DSS_TEST_HDMI ? DSS_DCTRL_NODE_DPI_DPI0 : DSS_DCTRL_NODE_EDP_DPI0;
    }
    else if (testObject->vpId == CSL_DSS_VP_ID_2)
    {
        gDctrlParamsInfo.vpNodeId = DSS_DCTRL_NODE_VP2;
        gDctrlParamsInfo.overlayId = CSL_DSS_OVERLAY_ID_2;
        gDctrlParamsInfo.overlayNodeId = DSS_DCTRL_NODE_OVERLAY2;
        
        if (gIntfSelected == DSS_TEST_HDMI)
        {
            gDctrlParamsInfo.outNodeId = DSS_DCTRL_NODE_DPI_DPI0;
        }
        else if (gIntfSelected == DSS_TEST_EDP)
        {
            gDctrlParamsInfo.outNodeId = DSS_DCTRL_NODE_EDP_DPI1;
        }
        else /* DSS_TEST_DSI */
        {
            gDctrlParamsInfo.outNodeId = DSS_DCTRL_NODE_DSI_DPI1;
        }
    }
    else if (testObject->vpId == CSL_DSS_VP_ID_3)
    {
        gDctrlParamsInfo.vpNodeId = DSS_DCTRL_NODE_VP3;
        gDctrlParamsInfo.overlayId = CSL_DSS_OVERLAY_ID_3;
        gDctrlParamsInfo.overlayNodeId = DSS_DCTRL_NODE_OVERLAY3;
        /* Only DSI and EDP output possible via VP1 */
        gDctrlParamsInfo.outNodeId = gIntfSelected == DSS_TEST_EDP ? DSS_DCTRL_NODE_EDP_DPI2 : DSS_DCTRL_NODE_DSI_DPI2;
    }
    else if (testObject->vpId == CSL_DSS_VP_ID_4) 
    {
        gDctrlParamsInfo.vpNodeId = DSS_DCTRL_NODE_VP4;
        gDctrlParamsInfo.overlayId = CSL_DSS_OVERLAY_ID_4;
        gDctrlParamsInfo.overlayNodeId = DSS_DCTRL_NODE_OVERLAY4;
        /* Only DSI and EDP output possible via VP4 */
        gDctrlParamsInfo.outNodeId = gIntfSelected == DSS_TEST_EDP ? DSS_DCTRL_NODE_EDP_DPI3 : DSS_DCTRL_NODE_DSI_DPI3;
    }
}

void DssTest_enableDP2HDMI()
{
    Board_STATUS boardStatus;
    Board_IoExpCfg_t ioExpCfg;

    DssTest_print("Turning on DP0_PWR_SW_EN pin for eDP adapters ... !!!\n");

    /* DP0_PWR_SW_EN is connected to I2C1 on J721E. I2C1 is mapped to I2C4 on Quad devices. */
#if defined (SOC_J721E)
    ioExpCfg.i2cInst     = 1U;
#elif defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
    ioExpCfg.i2cInst     = 4U;
#endif
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = 0x20;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_0;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    boardStatus = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)(&ioExpCfg));
    Osal_delay(500U);

    if (BOARD_SOK == boardStatus)
    {
        DssTest_print("Turning on DP0_PWR_SW_EN pin for eDP adapters ... Done!!!\n");
    }
    else
    {
        DssTest_print("Turning on DP0_PWR_SW_EN pin for eDP adapters ... failed !!!\n");
    }
}

void DssTest_populateCompleteTestInfo(DssTest_TestObj *testObject)
{
    /* Api calls infer all Dctrl config and pipeline params based on test config input */
    DssTest_updateDispInfoFromTestCfg(testObject);
    DssTest_inferDctrInfoFromTestCfg(testObject);
    DssTest_inferPipelineParamsFromTestCfg(testObject);
}


void DssTest_fillDctrlPathInfo(DssTest_TestObj *testObject)
{
    uint32_t i;
    Dss_DctrlPathInfo *pathInfo = &gDctrlCfg.pathInfoObj;
    Dss_dctrlPathInfoInit(pathInfo);
    pathInfo->edgeInfo[pathInfo->numEdges].startNode = gPipelineParams[0U].pipeNodeId;
    pathInfo->edgeInfo[pathInfo->numEdges].endNode = pDctrlParamsInfo->overlayNodeId;
    pathInfo->numEdges++;
    pathInfo->edgeInfo[pathInfo->numEdges].startNode = pDctrlParamsInfo->overlayNodeId;
    pathInfo->edgeInfo[pathInfo->numEdges].endNode = pDctrlParamsInfo->vpNodeId;
    pathInfo->numEdges++;
    pathInfo->edgeInfo[pathInfo->numEdges].startNode = pDctrlParamsInfo->vpNodeId;
    pathInfo->edgeInfo[pathInfo->numEdges].endNode = pDctrlParamsInfo->outNodeId;
    pathInfo->numEdges++;

    if (testObject->numPipes > 1U)
    {
        for (i=1U; i<testObject->numPipes; i++)
        {
            pathInfo->edgeInfo[pathInfo->numEdges].startNode =
                                            gPipelineParams[i].pipeNodeId;
            pathInfo->edgeInfo[pathInfo->numEdges].endNode =
                                            pDctrlParamsInfo->overlayNodeId;
            pathInfo->numEdges++;
        }
    }

    if (testObject->feature == DSS_TEST_MULTISYNC)
    {
        pathInfo->edgeInfo[pathInfo->numEdges].startNode = pDctrlParamsInfo->syncVPId;
        pathInfo->edgeInfo[pathInfo->numEdges].endNode = pDctrlParamsInfo->syncOutNodeId;
        pathInfo->numEdges++;
    }
}

void DssTest_fillDctrlVPParamsInfo(DssTest_TestObj *testObject)
{
    Dss_DctrlVpParams *vpParams = &gDctrlCfg.vpParamsObj;
    Dss_dctrlVpParamsInit(vpParams);
    vpParams->vpId = pDctrlParamsInfo->vpId;

    /* the below vp params are for colorbar test, some are common to DSI-HDMI, DP-DSI, DP-HDMI 
        therfore, will get overwritten accordingly for a specific output port */
    vpParams->lcdOpTimingCfg.dvoFormat = FVID2_DV_GENERIC_DISCSYNC;
    vpParams->lcdOpTimingCfg.videoIfWidth = FVID2_VIFW_24BIT;
    vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_1080P_60;
    vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_FALLING;
    vpParams->lcdPolarityCfg.actVidPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.hsPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.vsPolarity = FVID2_POL_HIGH;

    if (testObject->feature == DSS_TEST_COLORBAR)
    {
        // nothing more required for colorbar test
        return;
    }

    if (testObject->lcdOutRes == DSS_LCD_OUT_RES_720P)
    {
        vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_720P_60;
    }

    if (gIntfSelected == DSS_TEST_EDP)
    {
        vpParams->lcdOpTimingCfg.videoIfWidth = FVID2_VIFW_36BIT;
        vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_RISING;
    }
    else if (gIntfSelected == DSS_TEST_DSI)
    {
        vpParams->lcdPolarityCfg.hsPolarity = FVID2_POL_LOW;
        vpParams->lcdPolarityCfg.vsPolarity = FVID2_POL_LOW;
        vpParams->lcdPolarityCfg.actVidPolarity = FVID2_POL_HIGH;
        vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_RISING;
    }
    else // outPort == DSS_TEST_HDMI
    {
        //nothing to be done
    }

    if (testObject->blankingParams != NULL)
    {
        vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_CUSTOM,
        vpParams->lcdOpTimingCfg.mInfo.scanFormat = FVID2_SF_PROGRESSIVE,
        vpParams->lcdOpTimingCfg.mInfo.width = pDctrlParamsInfo->lcdOutWidth,
        vpParams->lcdOpTimingCfg.mInfo.height = pDctrlParamsInfo->lcdOutHeight,
        vpParams->lcdOpTimingCfg.mInfo.pixelClock = testObject->blankingParams->pixelClk;
        vpParams->lcdOpTimingCfg.mInfo.hFrontPorch = testObject->blankingParams->hfp;
        vpParams->lcdOpTimingCfg.mInfo.hBackPorch = testObject->blankingParams->hbp;
        vpParams->lcdOpTimingCfg.mInfo.hSyncLen = testObject->blankingParams->hsync;
        vpParams->lcdOpTimingCfg.mInfo.vFrontPorch = testObject->blankingParams->vfp;
        vpParams->lcdOpTimingCfg.mInfo.vBackPorch = testObject->blankingParams->vbp;
        vpParams->lcdOpTimingCfg.mInfo.vSyncLen = testObject->blankingParams->vsync;
    }

    if (testObject->feature == DSS_TEST_MULTISYNC)
    {
        vpParams->syncOpCfg.enabled = TRUE;
        vpParams->syncOpCfg.isPrimary = TRUE;
        vpParams->syncOpCfg.numSyncVpIds = 1U;
        vpParams->syncOpCfg.syncVpIds[0] = pDctrlParamsInfo->syncVPId;

        vpParams = &gDctrlCfg.syncVpParamsObj;
        Dss_dctrlVpParamsInit(vpParams);
        vpParams->vpId = pDctrlParamsInfo->vpId;
        vpParams->syncOpCfg.enabled = TRUE;
        vpParams->syncOpCfg.isPrimary = FALSE;
        vpParams->syncOpCfg.numSyncVpIds = 0U;

        /* the below params look redundant but note that vpParams address has been updated above */
        if (pDctrlParamsInfo->lcdOutHeight == DSS_LCD_OUT_RES_720P)
        {
            vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_720P_60;
        }
        else
        {
            vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_1080P_60;
        }
        vpParams->lcdOpTimingCfg.dvoFormat = FVID2_DV_GENERIC_DISCSYNC;
        vpParams->lcdOpTimingCfg.videoIfWidth = FVID2_VIFW_24BIT;
        vpParams->lcdPolarityCfg.actVidPolarity = FVID2_POL_HIGH;
        vpParams->lcdPolarityCfg.hsPolarity = FVID2_POL_HIGH;
        vpParams->lcdPolarityCfg.vsPolarity = FVID2_POL_HIGH;
        vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_FALLING;
    }    
}

void DssTest_fillDctrlOverlayParamsInfo(DssTest_TestObj *testObject)
{
    Dss_DctrlOverlayParams *overlayParams = &gDctrlCfg.overlayParamsObj;
    Dss_dctrlOverlayParamsInit(overlayParams);
    overlayParams->overlayId = pDctrlParamsInfo->overlayId;
    
    if (testObject->feature == DSS_TEST_COLORBAR)
    {
        overlayParams->colorbarEnable = TRUE;
    }
    else
    {
        overlayParams->colorbarEnable = FALSE;
        overlayParams->overlayCfg.colorKeyEnable = TRUE;
        overlayParams->overlayCfg.colorKeySel = CSL_DSS_OVERLAY_TRANS_COLOR_DEST;
        
        /* checking for the first pipe data format as current yuv tests use only 1 pipe */
        if (testObject->inDataFmt[0] == FVID2_DF_YUV420SP_UV)
        {
            overlayParams->overlayCfg.colorKeySel = CSL_DSS_OVERLAY_TRANS_COLOR_SRC;
        }

        overlayParams->overlayCfg.backGroundColor = 0xc8c800U;
    }
}

void DssTest_fillDctrlLayerParamsInfo(DssTest_TestObj *testObject)
{
    /* Marking all pipes as invalid initially , 0 mean invalid and 1 means valid here */
    uint32_t i = 0U, pipeIdStatus[CSL_DSS_VID_PIPE_ID_MAX] = {0U};
    Dss_DctrlOverlayLayerParams *layerParams;
    layerParams = &gDctrlCfg.layerParamsObj;
    Dss_dctrlOverlayLayerParamsInit(layerParams);
    layerParams->overlayId = pDctrlParamsInfo->overlayId;
    layerParams->pipeLayerNum[testObject->pipeId[0]] = CSL_DSS_OVERLAY_LAYER_NUM_0;

    if (testObject->numPipes > 1U)
    {
        for (i=1U; i<testObject->numPipes; i++)
        {
            layerParams->pipeLayerNum[testObject->pipeId[i]] = i;
        }
    }

    if (testObject->numPipes < CSL_DSS_VID_PIPE_ID_MAX)
    {
        /* Update the valid pipe ids in the pipeIdStatus array */
        for (i=0U; i<testObject->numPipes; i++)
        {
            pipeIdStatus[testObject->pipeId[i]] = 1U;
        }

        for (i=0U; i<CSL_DSS_VID_PIPE_ID_MAX; i++)
        {
            if (pipeIdStatus[i] == 0U)
            {
                layerParams->pipeLayerNum[i] = CSL_DSS_OVERLAY_LAYER_INVALID;
            }
        }
    }
}

void DssTest_fillDctrlAdvVpParamsInfo(DssTest_TestObj *testObject)
{
    Dss_DctrlAdvVpParams *advVpParams;
    advVpParams = &gDctrlCfg.advVpParamsObj;
    Dss_dctrlAdvVpParamsInit(advVpParams);
    advVpParams->vpId = pDctrlParamsInfo->vpId;

    if (gIntfSelected == DSS_TEST_EDP)
    {
        advVpParams->lcdAdvSignalCfg.hVAlign = CSL_DSS_VP_HVSYNC_ALIGNED;
        advVpParams->lcdAdvSignalCfg.hVClkControl = CSL_DSS_VP_HVCLK_CONTROL_ON;
        advVpParams->lcdAdvSignalCfg.hVClkRiseFall = FVID2_EDGE_POL_RISING;
    }
    else if (gIntfSelected == DSS_TEST_DSI)
    {
        advVpParams->lcdAdvSignalCfg.hVAlign = CSL_DSS_VP_HVSYNC_ALIGNED;
    }   

    if (testObject->feature == DSS_TEST_MULTISYNC)
    {
        advVpParams = &gDctrlCfg.syncAdvVpParamsObj;
        Dss_dctrlAdvVpParamsInit(advVpParams);
        advVpParams->vpId = pDctrlParamsInfo->syncVPId;
    }

}

void DssTest_fillDsiParamsInfo(Dss_DctrlDsiParams *dsiParams, uint32_t outResolution)
{
    dsiParams->instId = 0U;
#if defined (SOC_J721E)
    dsiParams->numOfLanes = 2;
    dsiParams->laneSpeedInKbps = 891000;
#else
    if (outResolution == DSS_LCD_OUT_RES_1080P)
    {
        dsiParams->numOfLanes = 4;
        dsiParams->laneSpeedInKbps = 950400;
    }
    else if (outResolution == DSS_LCD_OUT_RES_720P)
    {
        dsiParams->numOfLanes = 2;
        dsiParams->laneSpeedInKbps = 768000;
    }
#endif
    return;
}

void DssTest_fillVPSafetyChkParams(DssDctrlTest_Cfg *dctrlCfg)
{   
    Dss_DctrlVpSafetyChkParams *vpSafetyParams;
    vpSafetyParams = &dctrlCfg->vpSafetyObj[0];
    for(uint8_t count = 0; count < DSS_NUM_SAFETY_REGIONS; count++)
    {
        vpSafetyParams[count].vpId = gDssVpSafetyParams[count].vpId;
        vpSafetyParams[count].safetySignSeedVal = gDssVpSafetyParams[count].safetySignSeedVal;
        vpSafetyParams[count].regionSafetyChkCfg.regionId = gDssVpSafetyParams[count].regionSafetyChkCfg.regionId;
        vpSafetyParams[count].regionSafetyChkCfg.referenceSign = gDssVpSafetyParams[count].regionSafetyChkCfg.referenceSign;
        vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.safetyChkEnable = gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.safetyChkEnable;
        vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.safetyChkMode = gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.safetyChkMode;
        vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.seedSelectEnable = gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.seedSelectEnable;
        vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.thresholdValue = gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.thresholdValue;
        vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.frameSkip = gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.frameSkip;
        vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.regionPos.startX = gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.regionPos.startX;
        vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.regionPos.startY = gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.regionPos.startY;
        vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.regionSize.width = gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.regionSize.width;
        vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.regionSize.height = gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.regionSize.height;

        vpSafetyParams[count].safetyErrCbFxn = DssTest_vpSafetyErrCallback;
        vpSafetyParams[count].appData = &vpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.safetyChkMode;
    }
}

void DssTest_fillPipelineSafetyChkParams(DssTest_PipelineParams *pipelineParams, uint32_t instCnt)
{
    Dss_DispPipeSafetyChkParams *pipeSafetyObj = &(pipelineParams->pipeSafetyObj);    
    pipeSafetyObj->safetySignSeedVal = gDssPipelineSafetyParams[instCnt].safetySignSeedVal;
    pipeSafetyObj->referenceSign = gDssPipelineSafetyParams[instCnt].referenceSign;
    pipeSafetyObj->safetyChkCfg.safetyChkEnable = gDssPipelineSafetyParams[instCnt].safetyChkCfg.safetyChkEnable;
    pipeSafetyObj->safetyChkCfg.safetyChkMode = gDssPipelineSafetyParams[instCnt].safetyChkCfg.safetyChkMode;
    pipeSafetyObj->safetyChkCfg.seedSelectEnable = gDssPipelineSafetyParams[instCnt].safetyChkCfg.seedSelectEnable;
    pipeSafetyObj->safetyChkCfg.thresholdValue = gDssPipelineSafetyParams[instCnt].safetyChkCfg.thresholdValue;
    pipeSafetyObj->safetyChkCfg.frameSkip = gDssPipelineSafetyParams[instCnt].safetyChkCfg.frameSkip;
    pipeSafetyObj->safetyChkCfg.regionPos.startX = gDssPipelineSafetyParams[instCnt].safetyChkCfg.regionPos.startX;
    pipeSafetyObj->safetyChkCfg.regionPos.startY = gDssPipelineSafetyParams[instCnt].safetyChkCfg.regionPos.startY;
    pipeSafetyObj->safetyChkCfg.regionSize.width = gDssPipelineSafetyParams[instCnt].safetyChkCfg.regionSize.width;
    pipeSafetyObj->safetyChkCfg.regionSize.height = gDssPipelineSafetyParams[instCnt].safetyChkCfg.regionSize.height;

    pipeSafetyObj->safetyErrCbFxn = DssTest_pipelineSafetyErrCallback;
    pipeSafetyObj->appData = &(pipelineParams->pipeType);
}

void DssTest_dctrlCfgInit(DssTest_TestObj *testObject)
{
    DssDctrlTest_Cfg *dctrlCfg;
    dctrlCfg = &gDctrlCfg;
    DssTest_fillDctrlPathInfo(testObject);
    DssTest_fillDctrlVPParamsInfo(testObject);
    DssTest_fillDctrlOverlayParamsInfo(testObject);
    DssTest_fillDctrlLayerParamsInfo(testObject);
    DssTest_fillDctrlAdvVpParamsInfo(testObject);
    Dss_dctrlLcdBlankTimingParamsInit(&(dctrlCfg->lcdBlankTiminfObj));
    Dss_dctrlVpErrorStatsInit(&(dctrlCfg->errorStatsObj));
    Dss_dctrlGlobalDssParamsInit(&(dctrlCfg->globalDssParamsObj));
    Dss_dctrlSyncLostCbParamsInit(&(dctrlCfg->syncLostCbParamsObj));
    Dss_dctrlLineNumCbParamsInit(&(dctrlCfg->lineNumCbParamsObj));
    DssTest_fillDsiParamsInfo(&(dctrlCfg->dsiParamsObj), testObject->lcdOutRes);
    if (testObject->feature == DSS_TEST_FREEZE_FRAME)
    {
        for(uint8_t count = 0; count < CSL_DSS_VP_SAFETY_REGION_MAX; count++)
        {
            Dss_dctrlVpSafetyChkParamsInit(&(dctrlCfg->vpSafetyObj[count]));
        }
        DssTest_fillVPSafetyChkParams(dctrlCfg);
    }
    return;
}

void DssTest_fillDispParamsInfo(DssTest_TestObj *testObject, uint32_t pipeCnt)
{
    uint32_t i=0U;
    Dss_DispParams *dispParams;
    DssTest_PipelineParams *dispCfg;
    
    dispCfg = &(gPipelineParams[pipeCnt]);
    dispParams = &(dispCfg->dispParamsObj);
    Dss_dispParamsInit(dispParams);
    dispParams->pipeCfg.pipeType = dispCfg->pipeType;
    dispParams->pipeCfg.inFmt.width = testObject->pipeConfig[pipeCnt]->inWidth;
    dispParams->pipeCfg.inFmt.height = testObject->pipeConfig[pipeCnt]->inHeight;

    for (i = 0U; i < FVID2_MAX_PLANES; i++)
    {
        dispParams->pipeCfg.inFmt.pitch[i] = testObject->pipeConfig[pipeCnt]->pitch[i];
    }

    dispParams->pipeCfg.inFmt.dataFormat = testObject->inDataFmt[pipeCnt];
    dispParams->pipeCfg.inFmt.scanFormat = testObject->pipeConfig[pipeCnt]->inScanFmt;
    dispParams->pipeCfg.outWidth = testObject->pipeConfig[pipeCnt]->outWidth;
    dispParams->pipeCfg.outHeight = testObject->pipeConfig[pipeCnt]->outHeight;
    dispParams->pipeCfg.scEnable = testObject->pipeConfig[pipeCnt]->scEnable;

    if (testObject->inDataFmt[0] == FVID2_DF_YUV420SP_UV)
    {
        dispParams->pipeCfg.inFmt.ccsFormat = testObject->pipeConfig[pipeCnt]->ccsFormat;
        dispParams->pipeCfg.yuvAlign = CSL_DSS_VID_PIPE_YUV_ALIGN_LSB;
    }

    if (testObject->feature == DSS_TEST_FLIP)
    {
        dispParams->pipeCfg.flipType = FVID2_FLIP_TYPE_V;
    }

    dispParams->alphaCfg.globalAlpha = testObject->pipeConfig[pipeCnt]->globalAlpha;
    dispParams->alphaCfg.preMultiplyAlpha = testObject->pipeConfig[pipeCnt]->preMultiplyAlpha;
    dispParams->layerPos.startX = testObject->pipeConfig[pipeCnt]->posx;
    dispParams->layerPos.startY = testObject->pipeConfig[pipeCnt]->posy;
    
    if (testObject->feature == DSS_TEST_CROP)
    {
        dispParams->cropParams.cropEnable = TRUE;
        dispParams->cropParams.cropCfg.cropTop = 31;
        dispParams->cropParams.cropCfg.cropBottom = 31;
        dispParams->cropParams.cropCfg.cropLeft = 31;
        dispParams->cropParams.cropCfg.cropRight = 31;
    }
}

void DssTest_updateDispInfoFromTestCfg(DssTest_TestObj *testObject)
{
    for (uint32_t i = 0U; i < testObject->numPipes; i++)
    {
        gPipelineParams[i].dssDispDrvId = DSS_DISP_DRV_ID;
        /* See the Macro for Disp driver instance id to see the mapping to pipe id */
        gPipelineParams[i].dssDispDrvInstId = testObject->pipeId[i];

        if ((testObject->pipeId[i] == CSL_DSS_VID_PIPE_ID_VID1) || (testObject->pipeId[i] == CSL_DSS_VID_PIPE_ID_VID2))
        {
            gPipelineParams[i].pipeType = CSL_DSS_VID_PIPE_TYPE_VID;
        }
        else
        {
            gPipelineParams[i].pipeType = CSL_DSS_VID_PIPE_TYPE_VIDL;
        }

        if (testObject->pipeId[i] == CSL_DSS_VID_PIPE_ID_VID1)
        {
            gPipelineParams[i].pipeNodeId = DSS_DCTRL_NODE_VID1;
        }
        else if (testObject->pipeId[i] == CSL_DSS_VID_PIPE_ID_VID2)
        {
            gPipelineParams[i].pipeNodeId = DSS_DCTRL_NODE_VID2;
        }
        else if (testObject->pipeId[i] == CSL_DSS_VID_PIPE_ID_VIDL1)
        {
            gPipelineParams[i].pipeNodeId = DSS_DCTRL_NODE_VIDL1;
        }
        else if (testObject->pipeId[i] == CSL_DSS_VID_PIPE_ID_VIDL2)
        {
            gPipelineParams[i].pipeNodeId = DSS_DCTRL_NODE_VIDL2;
        }
    }
}


void DssTest_dispCfgInit(DssTest_TestObj *testObject, uint32_t pipeCnt)
{
    uint32_t pipeid;
    pipeid = testObject->pipeId[pipeCnt];
    
    DssTest_PipelineParams *pipelineParams = &gPipelineParams[pipeid];

    Dss_dispCreateParamsInit(&(pipelineParams->createParamsObj));
    Dss_dispPipeMflagParamsInit(&(pipelineParams->mflagParamsObj));
    Dss_dispBufPrgmCbParamsInit(&(pipelineParams->bufCbParamObj));
    Dss_dispPipePrgmCbParamsInit(&(pipelineParams->pipePrgrmCbObj));
    Dss_dispUnderFlowCbParamsInit(&(pipelineParams->underflowCbObj));
    DssTest_fillDispParamsInfo(testObject, pipeCnt);
    Dss_dispRtParamsInit(&(pipelineParams->rtParamsObj));
    Fvid2CbParams_init(&(pipelineParams->cbParamsObj));
    if (testObject->feature == DSS_TEST_FREEZE_FRAME)
    {
        Dss_dispPipeSafetyChkParamsInit(&(pipelineParams->pipeSafetyObj));
        DssTest_fillPipelineSafetyChkParams(pipelineParams, pipeCnt);
    }
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif
