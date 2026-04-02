/******************************************************************************
 * Copyright (c) 2019-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   display_port_test.c
 *
 *  \brief  displayport diagnostic test file.
 *
 *  Targeted Functionality: Verification of displayport interface.
 *
 *  Operation: This test verifies DisplayPort interface by displaying color bar
 *             on connected DisplayPort monitor.
 *
 *  Supported SoCs: J721E, J721S2, J784S4
 *
 *  Supported Platforms: j721e_evm, j721s2_evm, j784s4_evm
 */

#include "display_port_test.h"

/**
 *  \brief    This function de-initializes DSS DP.
 *
 *  \param    displayObj    [IN/OUT]  Display object structure.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dpDeInitDss(BoardDiag_DpDisplayObj *displayObj)
{
    int32_t ret;

    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_STOP_VP,
                        &displayObj->vpParams,
                        NULL);
    if(FVID2_SOK != ret)
    {
        return -1;
    }

    /* Delete DCTRL handle */
    ret = Fvid2_delete(displayObj->dctrlHandle,
                       NULL);
    if(FVID2_SOK != ret)
    {
        return -1;
    }

    ret = Dss_deInit();
    if(FVID2_SOK != ret)
    {
        return -1;
    }

    ret = Fvid2_deInit(NULL);
    if(FVID2_SOK != ret)
    {
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function enables internal color bar
 *
 *  \param    displayObj    [IN]      Display object structure.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dpEnableColorBar(BoardDiag_DpDisplayObj *displayObj)
{
    int32_t ret;
    Dss_DctrlVpParams *vpParams;
    Dss_DctrlOverlayParams *overlayParams;
    Dss_DctrlPathInfo *pathInfo;
    Dss_DctrlAdvVpParams *advVpParams;

    vpParams      = &displayObj->vpParams;
    overlayParams = &displayObj->overlayParams;
    pathInfo      = &displayObj->dctrlPathInfo;
    advVpParams   = &displayObj->advVpParams;

    Dss_dctrlOverlayParamsInit(overlayParams);
    Dss_dctrlPathInfoInit(pathInfo);

    pathInfo->edgeInfo[pathInfo->numEdges].startNode =
                                           BOARD_DIAG_DP_DCTRL_VID_NODE_ID;

    pathInfo->edgeInfo[pathInfo->numEdges].endNode =
                                           BOARD_DIAG_DP_DCTRL_OVERLAY_NODE_ID;

    pathInfo->numEdges++;

    pathInfo->edgeInfo[pathInfo->numEdges].startNode =
                                           BOARD_DIAG_DP_DCTRL_OVERLAY_NODE_ID;

    pathInfo->edgeInfo[pathInfo->numEdges].endNode =
                                           BOARD_DIAG_DP_DCTRL_VP_NODE_ID;

    pathInfo->numEdges++;

    pathInfo->edgeInfo[pathInfo->numEdges].startNode =
                                           BOARD_DIAG_DP_DCTRL_VP_NODE_ID;

    pathInfo->edgeInfo[pathInfo->numEdges].endNode =
                                           BOARD_DIAG_DP_DCTRL_OUT_NODE_ID;

    pathInfo->numEdges++;

    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_SET_PATH,
                        pathInfo,
                        NULL);
    if(FVID2_SOK != ret)
    {
        UART_printf("Setting Display path failed\n");
        return -1;
    }

    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_SET_VP_PARAMS,
                        vpParams,
                        NULL);
    if(FVID2_SOK != ret)
    {
        UART_printf("Setting VP param failed\n");
        return -1;
    }

    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_SET_ADV_VP_PARAMS,
                        advVpParams,
                        NULL);
    if(ret != FVID2_SOK)
    {
        UART_printf("Setting Advance VP Params failed\n");
        return -1;
    }

    overlayParams->overlayId = BOARD_DIAG_DP_OVERLAY_ID;
    overlayParams->colorbarEnable = UTRUE;

    ret = Fvid2_control(displayObj->dctrlHandle,
                        IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS,
                        overlayParams,
                        NULL);
    if(FVID2_SOK != ret)
    {
        UART_printf("Setting overlay params failed\n");
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function configures Display Timing parameters.
 *
 *  \param    displayObj    [IN]      Display object structure.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dpSetDispConfig(BoardDiag_DpDisplayObj *dispObj)
{
    Dss_DctrlVpParams *vpParams;
    int8_t ret;

    vpParams = &dispObj->vpParams;
    Dss_DctrlAdvVpParams *advVpParams;
    Dss_dctrlVpParamsInit(vpParams);

    advVpParams = &dispObj->advVpParams;

    Dss_dctrlAdvVpParamsInit(advVpParams);

    vpParams->vpId = BOARD_DIAG_DP_VP_ID;
    advVpParams->vpId = BOARD_DIAG_DP_VP_ID;
    vpParams->lcdOpTimingCfg.mInfo.standard   = FVID2_STD_1080P_60;
    vpParams->lcdOpTimingCfg.dvoFormat        = FVID2_DV_GENERIC_DISCSYNC;
    vpParams->lcdOpTimingCfg.videoIfWidth     = FVID2_VIFW_36BIT;

    vpParams->lcdPolarityCfg.actVidPolarity   = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_FALLING;
    vpParams->lcdPolarityCfg.hsPolarity       = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.vsPolarity       = FVID2_POL_HIGH;

    advVpParams->lcdAdvSignalCfg.hVAlign = CSL_DSS_VP_HVSYNC_ALIGNED;
    advVpParams->lcdAdvSignalCfg.hVClkControl = CSL_DSS_VP_HVCLK_CONTROL_ON;
    advVpParams->lcdAdvSignalCfg.hVClkRiseFall = FVID2_EDGE_POL_RISING;

    ret = BoardDiag_dpEnableColorBar(dispObj);
    if(0 != ret)
    {
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function initializes DSS DP.
 *
 *  \param    displayObj    [IN/OUT]  Display object structure.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dpInitDss(BoardDiag_DpDisplayObj *displayObj)
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
    displayObj->initParams.socParams.dpInitParams.isHpdSupported = UFALSE;

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
 *  \brief   This function sets DP clock to generate 148.5MHz.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_dpConfigDpClk(void)
{
    int32_t status = PM_SUCCESS, regVal;
    int8_t  retVal = 0;

    UART_printf("Configuring DP clock...\n");

#if defined(j721e_evm)
    status = Sciclient_pmSetModuleState(TISCI_DEV_SERDES_10G0,
                                        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                        TISCI_MSG_FLAG_AOP,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS != status)
    {
        return -1;
    }

    status = Sciclient_pmSetModuleState(TISCI_DEV_DSS_EDP0,
                                        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                        TISCI_MSG_FLAG_AOP,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS != status)
    {
        return -1;
    }

    status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                                        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                        TISCI_MSG_FLAG_AOP,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS != status)
    {
        return -1;
    }

    status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_18_HSDIVOUT0_CLK,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS != status)
    {
        return -1;
    }

    status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK_PARENT_DPI1_EXT_CLKSEL_OUT0,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS != status)
    {
        return -1;
    }

    status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS != status)
    {
        return -1;
    }

    status = Sciclient_pmSetModuleClkFreq(TISCI_DEV_DSS0,
                                          TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                                          BOARD_DIAG_DP_DISPLAY_CLOCK,
                                          ADDITIONAL_CLK_STATE_FLAG,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS != status)
    {
        return -1;
    }

    status = Sciclient_pmModuleClkRequest(TISCI_DEV_DSS0,
                                          TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                                          TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                          0,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS != status)
    {
        return -1;
    }
#endif

#if defined(j721s2_evm)
    int32_t clockStatus;
    uint64_t respClkRate;

    if(PM_SUCCESS == status)
    {
        /* Power on SerDes module. */
        status = Sciclient_pmSetModuleState(TISCI_DEV_SERDES_10G0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if(PM_SUCCESS == status)
    {
        /* Power on EDP module. */
        status = Sciclient_pmSetModuleState(TISCI_DEV_DSS_EDP0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    /* Power off DSS before configuring the clocks. */
    status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);

    if(PM_SUCCESS == status)
    {
        UART_printf("\n TISCI_DEV_DSS0 device shutdown successful !\r\n");
    }
    else
    {
        UART_printf("\n TISCI_DEV_DSS0 device shutdown NOT successful !!!\r\n");
    }

    /* Checking if the required pixel clock can be set by the system firmware. */
    status = Sciclient_pmQueryModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                        BOARD_DIAG_DP_DISPLAY_CLOCK,
                                        &respClkRate,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);

    if(PM_SUCCESS == status)
    {
        UART_printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK possible rate = %lld Hz\r\n", respClkRate);
    }
    else
    {
        UART_printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK requested rate range NOT possible !!\r\n");
    }

    if(PM_SUCCESS == status)
    {
        /* Check if the clock is enabled or not. */
        status = Sciclient_pmModuleGetClkStatus(TISCI_DEV_DSS0,
                                                TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                                &clockStatus,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if ((PM_SUCCESS == status) && (BOARD_DIAG_DP_DISPLAY_CLOCK <= respClkRate))
    {
        /* Set the pixel clock. */
        status = Sciclient_pmSetModuleClkFreq(
                                  TISCI_DEV_DSS0,
                                  TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                  respClkRate,
                                  TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                  SCICLIENT_SERVICE_WAIT_FOREVER);
        if (PM_SUCCESS == status)
        {
            if (TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ == clockStatus)
            {
                /* Enable the required clock. */
                status = Sciclient_pmModuleClkRequest(
                                                    TISCI_DEV_DSS0,
                                                    TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                                    TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                                    0U,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
            }
        }
    }

    if(PM_SUCCESS == status)
    {
        UART_printf("\nTISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_CLK is now ENABLED !\r\n");
        /* Enable the DSS device as clock configuration has been successful. */
        status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    else
    {
        UART_printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_CLK is STILL DISABLED !!!\r\n");
    }
#endif

#if defined(j784s4_evm)
    int32_t clockStatus;
    uint64_t respClkRate;

    /* Power on SerDes module. */
    status = Sciclient_pmSetModuleState(TISCI_DEV_SERDES_10G0,
                                        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                        TISCI_MSG_FLAG_AOP,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS == status)
    {
        /* Power on EDP module. */
        status = Sciclient_pmSetModuleState(TISCI_DEV_DSS_EDP0,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            TISCI_MSG_FLAG_AOP,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    /* Power off DSS before configuring the clocks. */
    status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                                        TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                        TISCI_MSG_FLAG_AOP,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS == status)
    {
        UART_printf("\n TISCI_DEV_DSS0 device shutdown successful !\r\n");
    }
    else
    {
        UART_printf("\n TISCI_DEV_DSS0 device shutdown NOT successful !!!\r\n");
    }

    /* Checking if the required pixel clock can be set by the system firmware. */
    status = Sciclient_pmQueryModuleClkFreq(TISCI_DEV_DSS0,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                            BOARD_DIAG_DP_DISPLAY_CLOCK,
                                            &respClkRate,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    if(PM_SUCCESS == status)
    {
        UART_printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK possible rate = %lld Hz\r\n", respClkRate);
    }
    else
    {
        UART_printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK requested rate range NOT possible !!\r\n");
    }

    if(PM_SUCCESS == status)
    {
        /* Check if the clock is enabled or not. */
        status = Sciclient_pmModuleGetClkStatus(TISCI_DEV_DSS0,
                                                TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                                &clockStatus,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if ((PM_SUCCESS == status) && (BOARD_DIAG_DP_DISPLAY_CLOCK <= respClkRate))
    {
        /* Set the pixel clock. */
        status = Sciclient_pmSetModuleClkFreq(TISCI_DEV_DSS0,
                                              TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                              respClkRate,
                                              TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
        if (PM_SUCCESS == status)
        {
            if (TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ == clockStatus)
            {
                /* Enable the required clock. */
                status = Sciclient_pmModuleClkRequest(TISCI_DEV_DSS0,
                                                      TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                                      TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                                      0U,
                                                      SCICLIENT_SERVICE_WAIT_FOREVER);
            }
        }
    }

    if(PM_SUCCESS == status)
    {
        uint64_t clkFreq = 0U;
        status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                                                TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                                                TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);

        PMLIBClkRateGet(TISCI_DEV_DSS0,
                        TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                        &clkFreq);
        UART_printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK Expected %lld and getting %lld Hz\r\n", BOARD_DIAG_DP_DISPLAY_CLOCK, clkFreq);

        clkFreq = 0U;
        PMLIBClkRateGet(TISCI_DEV_DSS0,
                        TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                        &clkFreq);
        UART_printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK Expected %lld and getting %lld Hz\r\n", BOARD_DIAG_DP_DISPLAY_CLOCK, clkFreq);

        UART_printf("\nTISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_CLK is now ENABLED !\r\n");
        /* Enable the DSS device as clock configuration has been successful. */
        status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            TISCI_MSG_FLAG_AOP,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    else
    {
        UART_printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_CLK is STILL DISABLED !!!\r\n");
    }
#endif

    if(PM_SUCCESS ==status)
    {
        retVal = 0;
    }
    else
    {
        retVal = -1;
    }

    return (retVal);
}

/**
 *  \brief    This function runs DisplayPort test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_DpRunTest(void)
{
    BoardDiag_DpDisplayObj dispObj;
    int8_t ret;
    char userInput;

    UART_printf("\n***********************************************\n");
    UART_printf  ("*               DISPLAYPORT Test              *\n");
    UART_printf  ("***********************************************\n");

    UART_printf("\nRunning DisplayPort Test...\n");

    ret = BoardDiag_dpConfigDpClk();
    if(0 != ret)
    {
        UART_printf("Configuring DP clock failed!\n");
        return -1;
    }

    ret = BoardDiag_dpInitDss(&dispObj);
    if(0 != ret)
    {
        UART_printf("\nDisplayPort test failed");
        return -1;
    }

    ret = BoardDiag_dpSetDispConfig(&dispObj);
    if(0 != ret)
    {
        UART_printf("\nDisplayPort test failed");
        return -1;
    }

    UART_printf("\nIf color bar displayed on the connected DisplayPort monitor,");
    UART_printf("\n\rpress 'y' or press any other key ");
    UART_scanFmt("%c", &userInput);

    ret = BoardDiag_dpDeInitDss(&dispObj);
    if(0 != ret)
    {
        UART_printf("Failed in DSS de-initializes\n");
        UART_printf("\nDisplayPort test failed");
        return -1;
    }

    if((userInput != 'y') && (userInput != 'Y'))
    {
        UART_printf("\nDisplayPort test failed");
        return -1;
    }

    UART_printf("\nDisplayPort test passed");
    return 0;
}

/**
 *  \brief   DP Diagnostic test main function
 *
 *  \return  int - DP Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 *
 */
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif

    /* Board Library Init. */
    status = Board_init(boardCfg);
    if(BOARD_SOK != status)
    {
        return -1;
    }

    return BoardDiag_DpRunTest();
}
