/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
 *
 *****************************************************************************/

/**
 *  \file   dsitx_test.c
 *
 *  \brief  dsitx diagnostic test file.
 *
 *  Targeted Functionality: Verification of DSI-Tx interface.
 *
 *  Operation: This test verifies DSI-Tx interface by displaying
 *             color bar in the connected display monitor.
 *
 *  Supported SoCs: J721E.
 *
 *  Supported Platforms: j721e_evm.
 *
 */

#include "dsitx_test.h"

Board_FpdModuleObj fpdModParams = {0x2C, 0x16, 0x11};

Board_FpdRmtDevObj fpdRmtParams[8] =
{
    {0x2c, 0x11}, //Remote derializer(UB924) divice physical and alias addresses
    {0x40, 0x12}, //Remote sensor device0 physial and alias addresses
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00}
};

/**
 *  \brief    This function de-initializes DSS.
 *
 *  \param    displayObj    [IN/OUT]  Display object structure.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dsitxDeInitDss(BoardDiag_DsiDisplayObj *displayObj)
{
    int32_t  retVal = FVID2_SOK;

    /* Delete DCTRL handle */
    retVal = Fvid2_delete(displayObj->dctrlHandle, NULL);
    retVal |= Dss_deInit();
    retVal |= Fvid2_deInit(NULL);
    if(FVID2_SOK != retVal)
    {
        UART_printf("DCTRL handle delete failed\n");
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function stops displaying color bar.
 *
 *  \param    displayObj    [IN]      Display object structure.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dsitxStopDisplay(BoardDiag_DsiDisplayObj *displayObj)
{
    int32_t retVal;
    Dss_DctrlVpParams *vpParams;

    vpParams = &displayObj->vpParams;

    retVal = Fvid2_control(displayObj->dctrlHandle,
                           IOCTL_DSS_DCTRL_STOP_VP,
                           vpParams,
                           NULL);
    if(FVID2_SOK != retVal)
    {
        UART_printf("VP Stop failed\n");
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function enables the color bar.
 *
 *  \param    displayObj    [IN/OUT]  DSI display object structure.
 *
 *  \return   int8_t  
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dsitxEnableColorBar(BoardDiag_DsiDisplayObj *displayObj)
{
    Dss_DctrlVpParams *vpParams;
    Dss_DctrlOverlayParams *overlayParams;
    Dss_DctrlPathInfo *pathInfo;
    Dss_DctrlAdvVpParams *advVpParams;
    Dss_DctrlDsiParams dsiPrms;
    int32_t ret;

    vpParams       = &displayObj->vpParams;
    overlayParams  = &displayObj->overlayParams;
    pathInfo       = &displayObj->dctrlPathInfo;
    advVpParams    = &displayObj->advVpParams;

    Dss_dctrlOverlayParamsInit(overlayParams);
    Dss_dctrlPathInfoInit(pathInfo);

    pathInfo->edgeInfo[pathInfo->numEdges].startNode =
                                           BOARD_DIAG_DSITX_VID_NODE_ID;

    pathInfo->edgeInfo[pathInfo->numEdges].endNode =
                                           BOARD_DIAG_DSITX_OVERLAY_NODE_ID;

    pathInfo->numEdges++;

    pathInfo->edgeInfo[pathInfo->numEdges].startNode =
                                           BOARD_DIAG_DSITX_OVERLAY_NODE_ID;

    pathInfo->edgeInfo[pathInfo->numEdges].endNode =
                                           BOARD_DIAG_DSITX_VP_NODE_ID;

    pathInfo->numEdges++;

    pathInfo->edgeInfo[pathInfo->numEdges].startNode =
                                           BOARD_DIAG_DSITX_VP_NODE_ID;

    pathInfo->edgeInfo[pathInfo->numEdges].endNode =
                                           BOARD_DIAG_DSITX_OUT_NODE_ID;

    pathInfo->numEdges++;

    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_SET_PATH,
                        pathInfo,
                        NULL);
    if (FVID2_SOK != ret)
    {
        UART_printf("Setting Display path failed\n");
        return -1;
    }

    dsiPrms.numOfLanes = 2U;
    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_SET_DSI_PARAMS,
                        &dsiPrms,
                        NULL);
    if (FVID2_SOK != ret)
    {
        UART_printf("Setting DSI params IOCTL failed\n");
        return -1;
    }

    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_SET_VP_PARAMS,
                        vpParams,
                        NULL);
    if (FVID2_SOK != ret)
    {
        UART_printf("Setting VP param failed\n");
        return -1;
    }

    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_SET_ADV_VP_PARAMS,
                        advVpParams,
                        NULL);
    if (FVID2_SOK != ret)
    {
        UART_printf("Setting Advance VP Params failed\n");
        return -1;
    }

    overlayParams->overlayId = BOARD_DIAG_DSITX_OVERLAY_ID;
    overlayParams->colorbarEnable = UTRUE;

    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS,
                        overlayParams,
                        NULL);
    if (FVID2_SOK != ret)
    {
        UART_printf("Setting overlay params failed\n");
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function configures display timing parameters.
 *
 *  \param    displayObj    [IN/OUT]  DSI display object structure.
 *
 *  \return   int8_t  
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static void BoardDiag_dsitxSetTimingParam(BoardDiag_DsiDisplayObj *dispObj)
{
    Dss_DctrlVpParams *vpParams;

    vpParams = &dispObj->vpParams;
    Dss_DctrlAdvVpParams *advVpParams;
    Dss_dctrlVpParamsInit(vpParams);

    advVpParams = &dispObj->advVpParams;

    Dss_dctrlAdvVpParamsInit(advVpParams);

    vpParams->vpId = BOARD_DIAG_DSITX_VP_ID;
    advVpParams->vpId = BOARD_DIAG_DSITX_VP_ID;
    vpParams->lcdOpTimingCfg.mInfo.standard    = FVID2_STD_CUSTOM;
    vpParams->lcdOpTimingCfg.dvoFormat         = FVID2_DV_GENERIC_DISCSYNC;
    vpParams->lcdOpTimingCfg.videoIfWidth      = FVID2_VIFW_24BIT;

    vpParams->lcdPolarityCfg.actVidPolarity    = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.pixelClkPolarity  = FVID2_EDGE_POL_RISING;
    vpParams->lcdPolarityCfg.hsPolarity        = FVID2_POL_LOW;
    vpParams->lcdPolarityCfg.vsPolarity        = FVID2_POL_LOW;

    vpParams->lcdOpTimingCfg.mInfo.width       = BOARD_DIAG_DSITX_LCD_WIDTH;
    vpParams->lcdOpTimingCfg.mInfo.height      = BOARD_DIAG_DSITX_LCD_HEIGHT;
    vpParams->lcdOpTimingCfg.mInfo.pixelClock  = BOARD_DIAG_DSITX_PCLK;
    vpParams->lcdOpTimingCfg.mInfo.hFrontPorch = BOARD_DIAG_DSITX_LCD_HFPORCH;
    vpParams->lcdOpTimingCfg.mInfo.hBackPorch  = BOARD_DIAG_DSITX_LCD_HBPORCH;
    vpParams->lcdOpTimingCfg.mInfo.hSyncLen    = BOARD_DIAG_DSITX_LCD_HSYNC;
    vpParams->lcdOpTimingCfg.mInfo.vFrontPorch = BOARD_DIAG_DSITX_LCD_VFPORCH;
    vpParams->lcdOpTimingCfg.mInfo.vBackPorch  = BOARD_DIAG_DSITX_LCD_VBPOTCH;
    vpParams->lcdOpTimingCfg.mInfo.vSyncLen    = BOARD_DIAG_DSITX_LCD_VSYNC;
}

/**
 *  \brief    This function initializes DSI-Tx and gets handle.
 *
 *  \param    displayObj    [IN/OUT]  DSI display object structure.
 *
 *  \return   int8_t  
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dsitxGetHandle(BoardDiag_DsiDisplayObj *displayObj)
{
    int32_t retVal;
    Fvid2_InitPrms initPrms;

    Fvid2InitPrms_init(&initPrms);

    retVal = Fvid2_init(&initPrms);
    if(FVID2_SOK != retVal)
    {
        UART_printf("Initializing Fvid2 Failed\n");
        return -1;
    }

    Dss_initParamsInit(&displayObj->initParams);

    UART_printf("Initializing DSS...\n");
    retVal = Dss_init(&displayObj->initParams);
    if(FVID2_SOK != retVal)
    {
        UART_printf("DSS init failed!\n");
        return -1;
    }

    /* Create DCTRL handle, used for common driver configuration */
    displayObj->dctrlHandle = Fvid2_create(DSS_DCTRL_DRV_ID,
                                           DSS_DCTRL_INST_0,
                                           NULL,
                                           NULL,
                                           NULL);
    if(NULL == displayObj->dctrlHandle)
    {
        UART_printf("Display Handle Create Failed\n");
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function configures DSI-Tx and run colorbar test.
 *
 *  \return   int8_t  
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dsitxConfig(void)
{
    BoardDiag_DsiDisplayObj dispObj;
    uint8_t userInput;
    int8_t ret;

    ret = BoardDiag_dsitxGetHandle(&dispObj);
    if(0 != ret)
    {
        UART_printf("\nDSS getting handle failed\n");
        return -1;
    }

    BoardDiag_dsitxSetTimingParam(&dispObj);

    UART_printf("Enabling color bar...\n");
    ret = BoardDiag_dsitxEnableColorBar(&dispObj);
    if(0 != ret)
    {
        UART_printf("Enabling color bar failed\n");
        return -1;
    }

    UART_printf("\nIf color bar is displaying on the display panel, press 'y'");
    UART_printf(" or anyother key to indicate failure ");
    UART_scanFmt("%c", &userInput);

    ret = BoardDiag_dsitxStopDisplay(&dispObj);
    if(0 != ret)
    {
        UART_printf("Display stop failed\n");
        return -1;
    }

    ret = BoardDiag_dsitxDeInitDss(&dispObj);
    if(0 != ret)
    {
        UART_printf("DSS De-Initialization failed\n");
        return -1;
    }

    if ((userInput != 'y') && (userInput != 'Y'))
    {
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function initializes FPD, LCD and DSI-Tx.
 *
 *  \return   int8_t  
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dsitxRunTest(void)
{
    uint8_t domain,regData;
    uint8_t i2cInst;
    uint8_t slaveAddr;
    int8_t ret = 0;
    I2C_Handle handle = NULL;
    I2C_Params i2cParams;

	Board_fpdUb941GetI2CAddr(&domain, &i2cInst, &slaveAddr);

    fpdModParams.serSlvAddr = slaveAddr;

    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;

    /* Configures the I2C instance with the passed parameters*/
	handle = Board_getI2CHandle(domain, i2cInst); 
	if (NULL == handle)
	{
		UART_printf("\nI2C Open failed!\n");
		return -1;
	}

    Board_delay(100U);
    UART_printf("Initializing host serializer...\n");
    ret = Board_fpdUb941SerInit((void *)handle,
                                &fpdModParams,
                                &fpdRmtParams[0]);
    if (0 != ret)
    {
        UART_printf("FPD serializer initialization failed\n");
        return -1;
    }

    /* Led Enabling through IO Expander */
    UART_printf("Enabling LED through IO Expander...\n");
    regData = BOARD_DIAG_DSITX_FPD_LED_EN;
    ret = Board_i2c8BitRegWr((void *)handle,
                             BOARD_DIAG_DSITX_FPD_LCD_PWM_SLV_ADDR,
                             BOARD_DIAG_DSITX_FPD_LCD_PWM_REG_ADDR,
                             &regData, //enabling led_en
                             1U,
                             BOARD_I2C_TRANSACTION_TIMEOUT);
    if (0 != ret)
    {
        UART_printf("LED enabling through IO EXP failed\n");
        return -1;
    }

    UART_printf("Configuring LED driver ...\n");
    ret = Board_tlc59108LedDrvCfg((void *)handle,
                                  &fpdModParams,
                                  &fpdRmtParams[1]);
    if (0 != ret)
    {
        UART_printf("Configuring LED failed\n");
        return -1;
    }

    UART_printf("Configuring Duty cycle for %d%% \n", BOARD_DIAG_DSITX_MAX_BRIGHTNESS+1);
    ret = Board_tlc59108PwmDutycycleCntrl((void *)handle,
                                          &fpdRmtParams[1],
                                          BOARD_FPD_TLC59108_PWM_CH2,
                                          BOARD_DIAG_DSITX_MAX_BRIGHTNESS);
    if (0 != ret)
    {
        UART_printf("Configuring Duty Cycle failed\n");
        return -1;
    }

    UART_printf("Initializing the remote deserializer...\n");
    ret = Board_fpdUb941DsiModuleInit((void *)handle,
                                      &fpdModParams);
    if(0 != ret)
    {
        UART_printf("FPD serializer DSI module init failed\n");
        return -1;
    }

    Board_i2cDeInit();

    ret = BoardDiag_dsitxConfig();
    if (0 != ret)
    {
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function configures DSS DSI-Tx clock
 *
 *  \param    dsiPclk   DSI-Tx Pixel clock to set
 *
 *  \return   int8_t  
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dsitxConfigClk(uint32_t dsiPclk)
{
    int32_t status;

    status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_18_HSDIVOUT0_CLK,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    if (PM_SUCCESS != status)
    {
        return -1;
    }

    /* Set the clock at the desirable frequency*/
    status = Sciclient_pmSetModuleClkFreq(TISCI_DEV_DSS0,
                                          TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                                          dsiPclk,
                                          TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (PM_SUCCESS != status)
    {
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function runs DSI-Tx test.
 *
 *  \return   int8_t  
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_DsiTest(void)
{
	Board_I2cInitCfg_t i2cCfg;
    Board_STATUS status;
    int8_t ret;

	UART_printf("\n*********************************************\n");
	UART_printf  ("*                DSITX  TEST                *\n");
	UART_printf  ("*********************************************\n");

    UART_printf("\nSetting DSS DSI-Tx clock\n");
    ret = BoardDiag_dsitxConfigClk(BOARD_DIAG_DSITX_PCLK);
    if (0 != ret)
    {
        return -1;
    }

    /* Setting power mux for FPD lcd */
    i2cCfg.i2cInst   = BOARD_I2C_IOEXP_DEVICE4_INSTANCE;
    i2cCfg.socDomain = BOARD_SOC_DOMAIN_MAIN;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    Board_i2cIoExpInit();

    status = Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                                           ONE_PORT_IOEXP,
                                           PORTNUM_0,
                                           PIN_NUM_7,
                                           PIN_DIRECTION_OUTPUT);
    if (BOARD_SOK != status)
    {
        UART_printf("UB941 serializer enabling external power reset failed\n");
        return -1;
    }

    status = Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                                       ONE_PORT_IOEXP,
                                       PORTNUM_0,
                                       PIN_NUM_7,
                                       GPIO_SIGNAL_LEVEL_HIGH);
    if (BOARD_SOK != status)
    {
        return -1;
    }

    status = Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                                           ONE_PORT_IOEXP,
                                           PORTNUM_0,
                                           PIN_NUM_2,
                                           PIN_DIRECTION_OUTPUT);
    if (BOARD_SOK != status)
    {
        UART_printf("DSI power switch control enable failed\n");
        return -1;
    }

    status = Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                                       ONE_PORT_IOEXP,
                                       PORTNUM_0,
                                       PIN_NUM_2,
                                       GPIO_SIGNAL_LEVEL_HIGH);
    if (BOARD_SOK != status)
    {
        return -1;
    }

    Board_i2cIoExpDeInit();

    ret = BoardDiag_dsitxRunTest();

    return ret;
}

/**
 *  \brief   DSITX Diagnostic test main function
 *
 *  \return  int - DSITX Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 *
 */
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int8_t ret;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;
#endif

    status = Board_init(boardCfg);
    if(BOARD_SOK != status)
    {
        return -1;
    }

	ret = BoardDiag_DsiTest();
    if (0 != ret)
    {
        UART_printf("\nDSI-Tx Test Failed\n");
        return -1;
    }
    else
    {
        UART_printf("\nDSI-Tx Test Passed\n");
    }
    return 0;
}
