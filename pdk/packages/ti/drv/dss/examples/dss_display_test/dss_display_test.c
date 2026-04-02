/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2022
 *  All rights reserved.
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
 *  \file dss_display_test.c
 *
 *  \brief DSS sample application that displays two ARGB32 buffers.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <dss_display_test.h>
#include <dss_display_buffer1.h>
#include <dss_display_buffer2.h>
#include <ti/drv/dss/examples/utils/app_utils.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#if defined(DSS_TESTAPP_TIRTOS)
#include <ti/drv/dss/examples/utils/app_utils_prf.h>
#endif

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

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined (SOC_AM65XX)
    #define TEST_VP_ID                      (CSL_DSS_VP_ID_1)
    #define TEST_OVERLAY_ID                 (CSL_DSS_OVERLAY_ID_1)
    #define TEST_DCTRL_OVERLAY_NODE_ID      (DSS_DCTRL_NODE_OVERLAY1)
    #define TEST_DCTRL_VP_NODE_ID           (DSS_DCTRL_NODE_VP1)
    #define TEST_DCTRL_OUT_NODE_ID          (DSS_DCTRL_NODE_OLDI)
#else
    #if(1U == DISP_APP_TEST_DSI)
        #if ((!defined (SOC_J721S2)) && (!defined (SOC_J784S4))) || ((defined (SOC_J721S2) || defined (SOC_J784S4)) && (DSI_INSTANCE_0 == 1))
        #define TEST_VP_ID                      (CSL_DSS_VP_ID_3)
        #define TEST_OVERLAY_ID                 (CSL_DSS_OVERLAY_ID_3)
        #define TEST_DCTRL_OVERLAY_NODE_ID      (DSS_DCTRL_NODE_OVERLAY3)
        #define TEST_DCTRL_VP_NODE_ID           (DSS_DCTRL_NODE_VP3)
        #define TEST_DCTRL_OUT_NODE_ID          (DSS_DCTRL_NODE_DSI_DPI2)
        #elif (defined (SOC_J721S2) || defined (SOC_J784S4)) && (DSI_INSTANCE_1 == 1)
        #define TEST_VP_ID                      (CSL_DSS_VP_ID_2)
        #define TEST_OVERLAY_ID                 (CSL_DSS_OVERLAY_ID_2)
        #define TEST_DCTRL_OVERLAY_NODE_ID      (DSS_DCTRL_NODE_OVERLAY2)
        #define TEST_DCTRL_VP_NODE_ID           (DSS_DCTRL_NODE_VP2)
        #define TEST_DCTRL_OUT_NODE_ID          (DSS_DCTRL_NODE_DSI_DPI1)
        #endif
    #elif(1U == DISP_APP_TEST_EDP)
        #define TEST_VP_ID                      (CSL_DSS_VP_ID_1)
        #define TEST_OVERLAY_ID                 (CSL_DSS_OVERLAY_ID_1)
        #define TEST_DCTRL_OVERLAY_NODE_ID      (DSS_DCTRL_NODE_OVERLAY1)
        #define TEST_DCTRL_VP_NODE_ID           (DSS_DCTRL_NODE_VP1)
        #define TEST_DCTRL_OUT_NODE_ID          (DSS_DCTRL_NODE_EDP_DPI0)
    #elif(1U == DISP_APP_TEST_OVERLAY_VP_4)
        #define TEST_VP_ID                      (CSL_DSS_VP_ID_4)
        #define TEST_OVERLAY_ID                 (CSL_DSS_OVERLAY_ID_4)
        #define TEST_DCTRL_OVERLAY_NODE_ID      (DSS_DCTRL_NODE_OVERLAY4)
        #define TEST_DCTRL_VP_NODE_ID           (DSS_DCTRL_NODE_VP4)
        #define TEST_DCTRL_OUT_NODE_ID          (DSS_DCTRL_NODE_DPI_DPI0)
    #else
        #define TEST_VP_ID                      (CSL_DSS_VP_ID_2)
        #define TEST_OVERLAY_ID                 (CSL_DSS_OVERLAY_ID_2)
        #define TEST_DCTRL_OVERLAY_NODE_ID      (DSS_DCTRL_NODE_OVERLAY2)
        #define TEST_DCTRL_VP_NODE_ID           (DSS_DCTRL_NODE_VP2)
        #define TEST_DCTRL_OUT_NODE_ID          (DSS_DCTRL_NODE_DPI_DPI0)
        #if(1U == DISP_APP_TEST_MULTISYNC)
            #define TEST_SYNC_VP_ID                 (CSL_DSS_VP_ID_1)
            #define TEST_DCTRL_SYNC_VP_NODE_ID      (DSS_DCTRL_NODE_VP1)
            #define TEST_DCTRL_SYNC_OUT_NODE_ID     (DSS_DCTRL_NODE_DISCSYNC0)
        #endif
    #endif
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void DispApp_init(DispApp_Obj *appObj);
static void DispApp_deInit(DispApp_Obj *appObj);
static void DispApp_create(DispApp_Obj *appObj);
static void DispApp_delete(DispApp_Obj *appObj);
static int32_t DispApp_configDctrl(DispApp_Obj *appObj);
static int32_t DispApp_runTest(DispApp_Obj *appObj);
static void DispApp_initParams(DispApp_Obj *appObj);
static int32_t DispApp_allocAndQueueFrames(const DispApp_Obj *appObj,
                                           DispApp_InstObj *instObj);
static int32_t DispApp_pipeCbFxn(Fvid2_Handle handle, void *appData);
static void DispApp_enableDP2HDMI( void );

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

DispApp_Obj gDispApp_Obj;
uint32_t gTestStopTime, gTestStartTime;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * DSS display test
 */
int32_t Dss_displayTest(void)
{
    int32_t retVal = FVID2_SOK;

    DispApp_init(&gDispApp_Obj);

    App_print("DSS display application started...\r\n");

#if defined(DSS_TESTAPP_TIRTOS)
    Utils_prfLoadCalcStart();
    Utils_prfLoadRegister(TaskP_self(), "Display_testapp");
#endif

#if (1U == DISP_APP_TEST_DSI)
    gDispApp_Obj.dsiPrms.instId = 0;
    gDispApp_Obj.dsiPrms.numOfLanes = 2;
#if defined (SOC_J721E)
    gDispApp_Obj.dsiPrms.laneSpeedInKbps = 891000;
#else
    gDispApp_Obj.dsiPrms.laneSpeedInKbps = 768000;
#endif
    if (FVID2_SOK == retVal)
    {
        retVal = DispApp_runTest(&gDispApp_Obj);
    }
#else
    retVal = DispApp_runTest(&gDispApp_Obj);
#endif

#if defined(DSS_TESTAPP_TIRTOS)
    Utils_prfLoadCalcStop();
    Utils_prfLoadPrintAll(UTRUE, 0);
    Utils_prfLoadCalcReset();
    Utils_prfLoadUnRegister(TaskP_self());
#endif

    App_print("Number of frames = %d, elapsed msec = %d, fps = %0.2f\n",
            DISP_APP_RUN_COUNT,
            gTestStopTime - gTestStartTime,
            (float)((float)DISP_APP_RUN_COUNT / ((gTestStopTime - gTestStartTime)/1000.0)));

    DispApp_deInit(&gDispApp_Obj);

    if(FVID2_SOK == retVal)
    {
        App_print("DSS display test Passed!!\r\n");
    }
    else
    {
        App_print("DSS display test Failed!!\r\n");
    }

    return (0);
}

void DispApp_enableDP2HDMI( void )
{
    Board_STATUS boardStatus;
    Board_IoExpCfg_t ioExpCfg;

    App_print("Turning on DP0_PWR_SW_EN pin for eDP adapters ... !!!\n");

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
        App_print("Turning on DP0_PWR_SW_EN pin for eDP adapters ... Done!!!\n");
    }
    else
    {
        App_print("Turning on DP0_PWR_SW_EN pin for eDP adapters ... failed !!!\n");
    }
}

static void DispApp_init(DispApp_Obj *appObj)
{
    int32_t         retVal = FVID2_SOK;
    Fvid2_InitPrms  initPrms;

    Fvid2InitPrms_init(&initPrms);
    initPrms.printFxn = &App_print;
    retVal = Fvid2_init(&initPrms);
    if(FVID2_SOK != retVal)
    {
        App_print("Fvid2 Init Failed!!!\r\n");
    }

    Dss_initParamsInit(&appObj->initParams);
#if(1U == DISP_APP_ENABLE_COMMON1_REGION)
    appObj->initParams.socParams.irqParams.dssCommonRegionId                    = CSL_DSS_COMM_REG_ID_1;
    appObj->initParams.socParams.irqParams.numValidIrq                          = DSS_EVT_MGR_INST_ID_MAX;
    appObj->initParams.socParams.irqParams.irqNum[DSS_EVT_MGR_INST_ID_FUNC]     = 53U;
    appObj->initParams.socParams.irqParams.irqNum[DSS_EVT_MGR_INST_ID_SAFETY]   = 55U;
    appObj->initParams.socParams.irqParams.irqNum[DSS_EVT_MGR_INST_ID_SECURITY] = 57U;
#endif
    appObj->initParams.socParams.dpInitParams.isHpdSupported                    = UFALSE;
#if ((defined (SOC_J721S2)) && (1U == DO_MULTILINK) && (1U == DISP_APP_TEST_EDP))
    appObj->initParams.socParams.dpInitParams.multilinkPhyType                  = DSS_DP_MULTILINK_PHY_USB;
#endif
    Dss_init(&appObj->initParams);

#if (1U == ENABLE_DP_TO_HDMI_CONVERTER)
#if (1U != DISP_APP_TEST_EDP)
    App_print("Display interface must be DP for using the DP to HDMI converter!!\r\n");
    retVal = FVID2_EBADARGS;
#endif
    if(FVID2_SOK == retVal)
    {
        DispApp_enableDP2HDMI();
    }
#endif
    if(FVID2_SOK == retVal)
    {
        /* Create DCTRL handle, used for common driver configuration */
        appObj->dctrlHandle = Fvid2_create(
            DSS_DCTRL_DRV_ID,
            DSS_DCTRL_INST_0,
            NULL,
            NULL,
            NULL);
        if(NULL == appObj->dctrlHandle)
        {
            App_print("DCTRL Create Failed!!!\r\n");
        }
    }

    if(FVID2_SOK == retVal)
    {
         App_print("DispApp_init() - DONE !!!\r\n");
    }

    return;
}

static void DispApp_deInit(DispApp_Obj *appObj)
{
    int32_t  retVal = FVID2_SOK;

    /* Delete DCTRL handle */
    retVal = Fvid2_delete(appObj->dctrlHandle, NULL);
    retVal += Dss_deInit();
    retVal += Fvid2_deInit(NULL);
    if(FVID2_SOK != retVal)
    {
         App_print("DCTRL handle delete failed!!!\r\n");
    }
    else
    {
         App_print("DispApp_deInit() - DONE !!!\r\n");
    }

    return;
}

static int32_t DispApp_runTest(DispApp_Obj *appObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t instCnt = 0U;
    volatile uint32_t loopCount = 0U;
    DispApp_InstObj *instObj;
    Fvid2_FrameList  frmList;

    /* Create driver */
    DispApp_create(appObj);

    App_print("Starting display ... !!!\r\n");
    App_print("Display in progress ... DO NOT HALT !!!\r\n");

    /* Start driver */
    for(instCnt = 0U; instCnt<gDispAppTestParams.numTestPipes; instCnt++)
    {
        instObj = &appObj->instObj[instCnt];

        retVal = Fvid2_start(instObj->drvHandle, NULL);
        if(FVID2_SOK != retVal)
        {
            App_print("Display Start Failed!!!\r\n");
            break;
        }
    }
#if (1==DISP_APP_TEST_DSI) && (defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2))
    DispApp_ErrorRegRead();
#endif

    gTestStartTime = (TimerP_getTimeInUsecs() / 1000U);

    while(loopCount++ < DISP_APP_RUN_COUNT)
    {
        for(instCnt = 0U; instCnt<gDispAppTestParams.numTestPipes; instCnt++)
        {
            instObj = &appObj->instObj[instCnt];
            (void) SemaphoreP_pend(instObj->syncSem, SemaphoreP_WAIT_FOREVER);
            retVal = Fvid2_dequeue(instObj->drvHandle,
                                   &frmList,
                                   0U,
                                   FVID2_TIMEOUT_NONE);

            if(FVID2_SOK == retVal)
            {
                retVal = Fvid2_queue(instObj->drvHandle, &frmList, 0U);
                if(FVID2_SOK != retVal)
                {
                    App_print("Display Queue Failed!!!\r\n");
                    break;
                }
            }
            else if (FVID2_EAGAIN == retVal)
            {
                /* Do nothing as this is first callback */
            }
            else
            {
                /* Error */
                App_print("Display Dequeue Failed!!!\r\n");
                break;
            }
        }
    }

    for(instCnt = 0U; instCnt<gDispAppTestParams.numTestPipes; instCnt++)
    {
        instObj = &appObj->instObj[instCnt];
        retVal  = Fvid2_stop(instObj->drvHandle, NULL);
        if(FVID2_SOK != retVal)
        {
            App_print("Display Stop Failed!!!\r\n");
            break;
        }
    }

    gTestStopTime = (TimerP_getTimeInUsecs() / 1000U);

    if(FVID2_SOK == retVal)
    {
        /* Delete driver */
        DispApp_delete(appObj);
    }

    return retVal;
}

static void DispApp_create(DispApp_Obj *appObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t instCnt = 0U;
    SemaphoreP_Params semParams;
    Dss_DctrlVpParams *vpParams;
    Dss_DctrlAdvVpParams *advVpParams;
    DispApp_InstObj *instObj;
#if (1==DISP_APP_TEST_EDP)
    int32_t dpConnectedCmdArg;
#endif
    DispApp_initParams(appObj);
    vpParams = &appObj->vpParams;
    advVpParams = &appObj->advVpParams;
    Dss_dctrlVpParamsInit(vpParams);
    Dss_dctrlAdvVpParamsInit(advVpParams);

    vpParams->vpId = TEST_VP_ID;
    advVpParams->vpId = TEST_VP_ID;

#if defined (SOC_AM65XX)
    vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_CUSTOM;
    vpParams->lcdOpTimingCfg.mInfo.width = DISP_APP_LCD_WIDTH;
    vpParams->lcdOpTimingCfg.mInfo.height = DISP_APP_LCD_HEIGHT;
    vpParams->lcdOpTimingCfg.mInfo.hFrontPorch = 48U;
    vpParams->lcdOpTimingCfg.mInfo.hBackPorch = 80U;
    vpParams->lcdOpTimingCfg.mInfo.hSyncLen = 32U;
    vpParams->lcdOpTimingCfg.mInfo.vFrontPorch = 3U;
    vpParams->lcdOpTimingCfg.mInfo.vBackPorch = 14U;
    vpParams->lcdOpTimingCfg.mInfo.vSyncLen = 6U;
#else
    #if(1U == DISP_APP_TEST_MULTISYNC)
        vpParams->syncOpCfg.enabled      = UTRUE;
        vpParams->syncOpCfg.isPrimary    = UTRUE;
        vpParams->syncOpCfg.numSyncVpIds = 1U;
        vpParams->syncOpCfg.syncVpIds[0] = TEST_SYNC_VP_ID;
    #endif
    #if(DISP_APP_BGR24 == DISP_APP_USE_TEST_PARAMS)
        #if(1U == DISP_APP_TEST_EDP)
            vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_CUSTOM;
            vpParams->lcdOpTimingCfg.mInfo.width = 1280U;
            vpParams->lcdOpTimingCfg.mInfo.height = 720U;
            vpParams->lcdOpTimingCfg.mInfo.pixelClock = 74250U;
            vpParams->lcdOpTimingCfg.mInfo.hFrontPorch = 110U;
            vpParams->lcdOpTimingCfg.mInfo.hBackPorch = 220U;
            vpParams->lcdOpTimingCfg.mInfo.hSyncLen = 40U;
            vpParams->lcdOpTimingCfg.mInfo.vFrontPorch = 5U;
            vpParams->lcdOpTimingCfg.mInfo.vBackPorch = 20U;
            vpParams->lcdOpTimingCfg.mInfo.vSyncLen = 5U;
        #else
            vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_720P_60;
        #endif
    #else
        #if(1U == DISP_APP_TEST_EDP)
            vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_CUSTOM;
            vpParams->lcdOpTimingCfg.mInfo.width = 1920U;
            vpParams->lcdOpTimingCfg.mInfo.height = 1080U;
            vpParams->lcdOpTimingCfg.mInfo.pixelClock = 148500U;
            vpParams->lcdOpTimingCfg.mInfo.hFrontPorch = 88U;
            vpParams->lcdOpTimingCfg.mInfo.hBackPorch = 148U;
            vpParams->lcdOpTimingCfg.mInfo.hSyncLen = 44U;
            vpParams->lcdOpTimingCfg.mInfo.vFrontPorch = 4U;
            vpParams->lcdOpTimingCfg.mInfo.vBackPorch = 36U;
            vpParams->lcdOpTimingCfg.mInfo.vSyncLen = 5U;
        #else
            vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_1080P_60;
        #endif
    #endif

#endif

    vpParams->lcdOpTimingCfg.dvoFormat = FVID2_DV_GENERIC_DISCSYNC;
#if(1U == DISP_APP_TEST_EDP)
    vpParams->lcdOpTimingCfg.videoIfWidth = FVID2_VIFW_36BIT;
#else
    vpParams->lcdOpTimingCfg.videoIfWidth = FVID2_VIFW_24BIT;
#endif

    vpParams->lcdPolarityCfg.actVidPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.hsPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.vsPolarity = FVID2_POL_HIGH;

#if(1U == DISP_APP_TEST_EDP)
    vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_RISING;

    advVpParams->lcdAdvSignalCfg.hVAlign = CSL_DSS_VP_HVSYNC_ALIGNED;
    advVpParams->lcdAdvSignalCfg.hVClkControl = CSL_DSS_VP_HVCLK_CONTROL_ON;
    advVpParams->lcdAdvSignalCfg.hVClkRiseFall = FVID2_EDGE_POL_RISING;
#else
    vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_FALLING;
#endif

#if(1U == DISP_APP_TEST_MULTISYNC)
    vpParams = &appObj->syncVpParams;
    advVpParams = &appObj->syncAdvVpParams;
    Dss_dctrlVpParamsInit(vpParams);
    Dss_dctrlAdvVpParamsInit(advVpParams);

    vpParams->vpId = TEST_SYNC_VP_ID;
    advVpParams->vpId = TEST_SYNC_VP_ID;

    vpParams->syncOpCfg.enabled      = UTRUE;
    vpParams->syncOpCfg.isPrimary    = UFALSE;
    vpParams->syncOpCfg.numSyncVpIds = 0U;

#if(DISP_APP_BGR24 == DISP_APP_USE_TEST_PARAMS)
    vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_720P_60;
#else
    vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_1080P_60;
#endif

    vpParams->lcdOpTimingCfg.dvoFormat = FVID2_DV_GENERIC_DISCSYNC;
    vpParams->lcdOpTimingCfg.videoIfWidth = FVID2_VIFW_24BIT;
    vpParams->lcdPolarityCfg.actVidPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.hsPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.vsPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_FALLING;
#endif

#if (1U == DISP_APP_TEST_DSI)
    vpParams->lcdOpTimingCfg.dvoFormat = FVID2_DV_GENERIC_DISCSYNC;
    vpParams->lcdOpTimingCfg.videoIfWidth = FVID2_VIFW_24BIT;
    vpParams->lcdPolarityCfg.actVidPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.hsPolarity = FVID2_POL_LOW;
    vpParams->lcdPolarityCfg.vsPolarity = FVID2_POL_LOW;
    advVpParams->lcdAdvSignalCfg.hVAlign = CSL_DSS_VP_HVSYNC_ALIGNED;

    vpParams->lcdOpTimingCfg.mInfo.standard = FVID2_STD_CUSTOM;
    vpParams->lcdOpTimingCfg.mInfo.width = DISP_APP_LCD_WIDTH;
    vpParams->lcdOpTimingCfg.mInfo.height = DISP_APP_LCD_HEIGHT;
#if defined (SOC_J721E)
    vpParams->lcdOpTimingCfg.mInfo.pixelClock = 74250000ULL;
    vpParams->lcdOpTimingCfg.mInfo.scanFormat = FVID2_SF_PROGRESSIVE;
    vpParams->lcdOpTimingCfg.mInfo.hFrontPorch = 110U;
    vpParams->lcdOpTimingCfg.mInfo.hBackPorch = 220U;
    vpParams->lcdOpTimingCfg.mInfo.hSyncLen = 40U;
    vpParams->lcdOpTimingCfg.mInfo.vFrontPorch = 5U;
    vpParams->lcdOpTimingCfg.mInfo.vBackPorch = 20U;
    vpParams->lcdOpTimingCfg.mInfo.vSyncLen = 5U;
#else
    vpParams->lcdOpTimingCfg.mInfo.pixelClock = 64000000ULL;
    vpParams->lcdOpTimingCfg.mInfo.scanFormat = FVID2_SF_PROGRESSIVE;
    vpParams->lcdOpTimingCfg.mInfo.hFrontPorch = 48U;
    vpParams->lcdOpTimingCfg.mInfo.hBackPorch = 80U;
    vpParams->lcdOpTimingCfg.mInfo.hSyncLen = 32U;
    vpParams->lcdOpTimingCfg.mInfo.vFrontPorch = 4U;
    vpParams->lcdOpTimingCfg.mInfo.vBackPorch = 12U;
#endif

    vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_RISING;
#endif
#if (1==DISP_APP_TEST_EDP)
    retVal = Fvid2_control(appObj->dctrlHandle, IOCTL_DSS_DCTRL_IS_DP_CONNECTED, &dpConnectedCmdArg, NULL);
    if ((FVID2_SOK == retVal) && (1 == dpConnectedCmdArg))
    {
#endif
        DispApp_configDctrl(appObj);
        for(instCnt = 0U; instCnt<gDispAppTestParams.numTestPipes; instCnt++)
        {
            instObj = &appObj->instObj[instCnt];
            SemaphoreP_Params_init(&semParams);
            semParams.mode = SemaphoreP_Mode_BINARY;
            instObj->syncSem = SemaphoreP_create(0U, &semParams);
            instObj->drvHandle = Fvid2_create(
                DSS_DISP_DRV_ID,
                instObj->instId,
                &instObj->createParams,
                &instObj->createStatus,
                &instObj->cbParams);
            if((NULL == instObj->drvHandle) ||
            (FVID2_SOK != instObj->createStatus.retVal))
            {
                App_print("Display Create Failed!!!\r\n");
                retVal = instObj->createStatus.retVal;
            }

            if(FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                    instObj->drvHandle,
                    IOCTL_DSS_DISP_SET_DSS_PARAMS,
                    &instObj->dispParams,
                    NULL);
                if(FVID2_SOK != retVal)
                {
                    App_print("DSS Set Params IOCTL Failed!!!\r\n");
                }
            }
            if(FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                    instObj->drvHandle,
                    IOCTL_DSS_DISP_SET_PIPE_MFLAG_PARAMS,
                    &instObj->mflagParams,
                    NULL);
                if(FVID2_SOK != retVal)
                {
                    App_print("DSS Set Mflag Params IOCTL Failed!!!\r\n");
                }
            }

            if(FVID2_SOK == retVal)
            {
                retVal = DispApp_allocAndQueueFrames(appObj, instObj);
                if(FVID2_SOK != retVal)
                {
                    App_print("Display Alloc and Queue Failed!!!\r\n");
                }
            }

            if(FVID2_SOK != retVal)
            {
                break;
            }
        }

        if(FVID2_SOK == retVal)
        {
            App_print("Display create complete!!\r\n");
        }
#if(1U==DISP_APP_TEST_EDP)     
    }
    else
    {
        printf("The display cable is not connected!!\n");
    }
#endif  
    return;
}

static void DispApp_delete(DispApp_Obj *appObj)
{
    int32_t retVal;
    uint32_t instCnt;
    Dss_DctrlVpParams *vpParams;
#if(1U == DISP_APP_TEST_MULTISYNC)
    Dss_DctrlVpParams *syncVpParams;
#endif
    Dss_DctrlPathInfo *pathInfo;
    Dss_DctrlVpErrorStats *pErrorStats;
    DispApp_InstObj *instObj;
    Dss_DispCurrentStatus currStatus;
    Fvid2_FrameList frmList;

    vpParams = &appObj->vpParams;
#if(1U == DISP_APP_TEST_MULTISYNC)
    syncVpParams = &appObj->syncVpParams;
#endif
    pathInfo = &appObj->dctrlPathInfo;
    pErrorStats = &appObj->errorStats;

    for(instCnt = 0U; instCnt<gDispAppTestParams.numTestPipes; instCnt++)
    {
        instObj = &appObj->instObj[instCnt];

        /* Check for DSS underflow errors */
        retVal = Fvid2_control(
            instObj->drvHandle,
            IOCTL_DSS_DISP_GET_CURRENT_STATUS,
            &currStatus,
            NULL);
        if(FVID2_SOK != retVal)
        {
            App_print("Failed to get Display Stats!!!\r\n");
        }

        /* Print Synclost errors */
        if(0U != currStatus.underflowCount)
        {
            GT_2trace(DssTrace, GT_ERR, "No of Underflows for Inst %d: %d\r\n", instCnt, currStatus.underflowCount);
        }
        else
        {
            App_print("Underflow did not occur\r\n");
        }

        /* Dequeue all the request from the driver */
        while (BTRUE)
        {
            retVal = Fvid2_dequeue(
                instObj->drvHandle,
                &frmList,
                0U,
                FVID2_TIMEOUT_NONE);
            if(FVID2_SOK != retVal)
            {
                break;
            }
        }

        retVal = Fvid2_delete(instObj->drvHandle, NULL);
        if(FVID2_SOK != retVal)
        {
            App_print("Display Delete Failed!!!\r\n");
            break;
        }
    }

    /* Check for DSS synclost errors */
    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_GET_VP_ERROR_STATS,
        pErrorStats,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("Failed to get VP Stats!!!\r\n");
    }

    /* Print Synclost errors */
    if(0U != pErrorStats->syncLost)
    {
        GT_1trace(DssTrace, GT_ERR, "No of Sync Lost: %d\r\n", pErrorStats->syncLost);
    }
    else
    {
        App_print("Sync Lost did not occur\r\n");
    }

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_CLEAR_PATH,
        pathInfo,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("Clear Path Failed!!!\r\n");
    }

#if(1U == DISP_APP_TEST_MULTISYNC)
    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_STOP_VP,
        syncVpParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("VP Stop Failed!!!\r\n");
    }
#endif

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_STOP_VP,
        vpParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("VP Stop Failed!!!\r\n");
    }

    if(FVID2_SOK == retVal)
    {
         App_print("Display delete complete!!\r\n");
    }

    return;
}

static int32_t DispApp_allocAndQueueFrames(const DispApp_Obj *appObj,
                                           DispApp_InstObj *instObj)
{
    int32_t  retVal = FVID2_SOK;
    uint32_t frmId, numFrames;
    Fvid2_Frame *frm;
    Fvid2_FrameList frmList;

#if(1U == DISP_APP_LOAD_BUFFERS_RUNTIME)

#if (1U == DISP_APP_TEST_DSI)
    uint32_t height = 800U, width = 1280U;
    char uartInput = '0';
    App_print("Load Image using loadRaw command and then press '1'\n");
    App_print("Command is:\n");
    App_print("loadRaw(0x82000000, 0, \"C:\\\\PSP4_yuyv422_prog_packed_1280_800.yuv\", 32, false);\n");
    do
    {
        scanf("%c", &uartInput);
    } while ('1' != uartInput);
#elif (1U == DISP_APP_YUV422_SP)
    uint32_t height=1080U, width=1920U;
    char uartInput = '0';
    App_print("Load Image using loadRaw command and then press '1'\n");
    App_print("Command is:\n");
    App_print("loadRaw(0x82000000, 0, \"C:\\\\yuv422_sp.tigf.tigf\", 32, false);\n");
    do
    {
        scanf("%c", &uartInput);
    } while ('1' != uartInput);
#else
    uint32_t height = 1080U, width = 1920U;
    char uartInput = '0';
    App_print("Load Image using loadRaw command and then press '1'\n");
    App_print("Command is:\n");
    App_print("loadRaw(0x82000000, 0, \"C:\\\\display_bgr888_prog_packed_1920_1080.tigf\", 32, false);\n");
    do
    {
        scanf("%c", &uartInput);
    } while ('1' != uartInput);
#endif

#endif

    Fvid2FrameList_init(&frmList);
    frm = &instObj->frames[0U];
    numFrames = DISP_APP_MAX_FRAMES_PER_HANDLE;
    /* init memory pointer for 'numFrames'  */
    for(frmId = 0U; frmId<numFrames; frmId++)
    {
        /* init Fvid2_Frame to 0's  */
        Fvid2Frame_init(&frm[frmId]);
#if(1U == DISP_APP_LOAD_BUFFERS_RUNTIME)
    #if(DISP_APP_YUV420 == DISP_APP_USE_TEST_PARAMS)
        frm[frmId].addr[0U] =
                        (uint64_t)(DISP_APP_DDR_LOAD_ADDRESS +
                                   frmId*height*width*3U/2U);
        frm[frmId].addr[1U] =
                        (uint64_t)(DISP_APP_DDR_LOAD_ADDRESS +
                                   frmId*height*width*3U/2U +
                                   height*width);
    #elif(DISP_APP_YUV420_12 == DISP_APP_USE_TEST_PARAMS)
        frm[frmId].addr[0U] =
                        (uint64_t)(DISP_APP_DDR_LOAD_ADDRESS);
        frm[frmId].addr[1U] =
                        (uint64_t)(DISP_APP_DDR_LOAD_ADDRESS +
                                   height*width*2U);
        #if(1U == DISP_APP_RAW_DATA_INPUT)
            /* For raw image , treat vid buffer as Luma and initialize chroma */
            uint32_t temp_addr = frm[frmId].addr[1U];
            int32_t j;
            for(j = 0;j < 518400; j++)
            {
                CSL_REG32_WR(temp_addr + 4*j, 0x08000800U);
            }
        #endif
    #elif(DISP_APP_YUV422_SP == DISP_APP_USE_TEST_PARAMS)
        frm[frmId].addr[0U] =
                        (uint64_t)(DISP_APP_DDR_LOAD_ADDRESS +
                                   frmId*(2*height*width));
        frm[frmId].addr[1U] =
                        (uint64_t)(DISP_APP_DDR_LOAD_ADDRESS +
                                   frmId*(2*height*width) +
                                   height*width);
    #else
        frm[frmId].addr[0U] =
                         (uint64_t)(DISP_APP_DDR_LOAD_ADDRESS +
                                    frmId*height*width*gDispAppTestParams.bpp);

    #endif
#else
        if(instObj->instId == gDispAppTestParams.instId[0U])
        {
            if(0U == frmId)
            {
                frm[frmId].addr[0U] = (uint64_t) gDispArray1;
            }
            else
            {
                frm[frmId].addr[0U] = (uint64_t)gDispArray2;
            }
        }
        else
        {
            if(0U == frmId)
            {
                frm[frmId].addr[0U] = (uint64_t)gDispArray1;
            }
            else
            {
                frm[frmId].addr[0U] = (uint64_t)gDispArray2;
            }
        }
#endif
        frm[frmId].fid = FVID2_FID_FRAME;
        frm[frmId].appData = instObj;

        /* Set number of frame in frame list - one at a time */
        frmList.numFrames  = 1U;
        frmList.frames[0U] = &frm[frmId];

        /*
         * queue the frames in frmList
         * All allocate frames are queued here as an example.
         * In general atleast 2 frames per channel need to queued
         * before starting display,
         * else frame will get dropped until frames are queued
         */
        retVal = Fvid2_queue(instObj->drvHandle, &frmList, 0U);
        if(FVID2_SOK != retVal)
        {
            App_print("Display Queue Failed!!!\r\n");
            break;
        }
    }

    return (retVal);
}

static void DispApp_initParams(DispApp_Obj *appObj)
{
    uint32_t instCnt = 0U, numPipes = 0U, i;
    Dss_DispParams *dispParams;
    DispApp_InstObj *instObj;

    numPipes = gDispAppTestParams.numTestPipes;

    for(instCnt = 0U; instCnt < numPipes; instCnt++)
    {
        /* Initialize video pipes */
        instObj = &appObj->instObj[instCnt];
        instObj->instId = gDispAppTestParams.instId[instCnt];
        Dss_dispCreateParamsInit(&instObj->createParams);
        Fvid2CbParams_init(&instObj->cbParams);
        instObj->cbParams.cbFxn = &DispApp_pipeCbFxn;
        instObj->cbParams.appData = instObj;

        dispParams = &instObj->dispParams;
        Dss_dispParamsInit(dispParams);
        dispParams->pipeCfg.pipeType = gDispAppTestParams.pipeType[instCnt];
        dispParams->pipeCfg.inFmt.width = gDispAppTestParams.inWidth[instCnt];
        dispParams->pipeCfg.inFmt.height = gDispAppTestParams.inHeight[instCnt];
        for(i = 0U; i < FVID2_MAX_PLANES; i++)
        {
            dispParams->pipeCfg.inFmt.pitch[i] =
                                        gDispAppTestParams.pitch[instCnt][i];
        }
        dispParams->pipeCfg.inFmt.dataFormat =
                                        gDispAppTestParams.inDataFmt[instCnt];
        dispParams->pipeCfg.inFmt.scanFormat =
                                        gDispAppTestParams.inScanFmt[instCnt];
        dispParams->pipeCfg.outWidth = gDispAppTestParams.outWidth[instCnt];
        dispParams->pipeCfg.outHeight = gDispAppTestParams.outHeight[instCnt];
        dispParams->pipeCfg.scEnable = gDispAppTestParams.scEnable[instCnt];
#if(DISP_APP_YUV420_12 == DISP_APP_USE_TEST_PARAMS)
        dispParams->pipeCfg.inFmt.ccsFormat = FVID2_CCSF_BITS12_UNPACKED16;
        dispParams->pipeCfg.yuvAlign = CSL_DSS_VID_PIPE_YUV_ALIGN_LSB;
#endif
#if(1U==DISP_APP_ENABLE_FLIP)
        dispParams->pipeCfg.flipType = FVID2_FLIP_TYPE_V;
#endif
        dispParams->alphaCfg.globalAlpha =
                                gDispAppTestParams.globalAlpha[instCnt];
        dispParams->alphaCfg.preMultiplyAlpha =
                                gDispAppTestParams.preMultiplyAlpha[instCnt];
        dispParams->layerPos.startX = gDispAppTestParams.posx[instCnt];
        dispParams->layerPos.startY = gDispAppTestParams.posy[instCnt];
#if(1U == DISP_APP_ENBALE_PIPE_CROP)
        dispParams->cropParams.cropEnable = UTRUE;
        dispParams->cropParams.cropCfg.cropTop = 31;
        dispParams->cropParams.cropCfg.cropBottom = 31;
        dispParams->cropParams.cropCfg.cropLeft = 31;
        dispParams->cropParams.cropCfg.cropRight = 31;
#endif
        Dss_dispPipeMflagParamsInit(&instObj->mflagParams);
    }
}

static int32_t DispApp_configDctrl(DispApp_Obj *appObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t i = 0U, j = 0U;
    Dss_DctrlVpParams *vpParams;
#if(1U == DISP_APP_TEST_MULTISYNC)
    Dss_DctrlVpParams *syncVpParams;
#endif
    Dss_DctrlOverlayParams *overlayParams;
    Dss_DctrlOverlayLayerParams *layerParams;
    Dss_DctrlPathInfo *pathInfo;
    Dss_DctrlAdvVpParams *advVpParams;
#if(1U == DISP_APP_TEST_MULTISYNC)
    Dss_DctrlAdvVpParams *syncAdvVpParams;
#endif
    Dss_DctrlGlobalDssParams *globalDssParams;
#if defined (SOC_AM65XX)
    Dss_DctrlOldiParams *oldiParams;
    oldiParams = &appObj->oldiParams;
    Dss_dctrlOldiParamsInit(oldiParams);
#endif
    vpParams = &appObj->vpParams;
#if(1U == DISP_APP_TEST_MULTISYNC)
    syncVpParams = &appObj->syncVpParams;
#endif
    overlayParams = &appObj->overlayParams;
    layerParams = &appObj->layerParams;
    pathInfo = &appObj->dctrlPathInfo;
    advVpParams = &appObj->advVpParams;
#if(1U == DISP_APP_TEST_MULTISYNC)
    syncAdvVpParams = &appObj->syncAdvVpParams;
#endif
    globalDssParams= &appObj->globalDssParams;

    Dss_dctrlOverlayParamsInit(overlayParams);
    Dss_dctrlOverlayLayerParamsInit(layerParams);
    Dss_dctrlPathInfoInit(pathInfo);
    Dss_dctrlGlobalDssParamsInit(globalDssParams);

    pathInfo->edgeInfo[pathInfo->numEdges].startNode = gDispAppTestParams.pipeNodeId[0U];
    pathInfo->edgeInfo[pathInfo->numEdges].endNode = TEST_DCTRL_OVERLAY_NODE_ID;
    pathInfo->numEdges++;
    pathInfo->edgeInfo[pathInfo->numEdges].startNode = TEST_DCTRL_OVERLAY_NODE_ID;
    pathInfo->edgeInfo[pathInfo->numEdges].endNode = TEST_DCTRL_VP_NODE_ID;
    pathInfo->numEdges++;
    pathInfo->edgeInfo[pathInfo->numEdges].startNode = TEST_DCTRL_VP_NODE_ID;
    pathInfo->edgeInfo[pathInfo->numEdges].endNode = TEST_DCTRL_OUT_NODE_ID;
    pathInfo->numEdges++;
    if(1U < gDispAppTestParams.numTestPipes)
    {
        for(i = 1U; i < gDispAppTestParams.numTestPipes; i++)
        {
            pathInfo->edgeInfo[pathInfo->numEdges].startNode =
                                            gDispAppTestParams.pipeNodeId[i];
            pathInfo->edgeInfo[pathInfo->numEdges].endNode =
                                            TEST_DCTRL_OVERLAY_NODE_ID;
            pathInfo->numEdges++;
        }
    }
#if(1U == DISP_APP_TEST_MULTISYNC)
        pathInfo->edgeInfo[pathInfo->numEdges].startNode = TEST_DCTRL_SYNC_VP_NODE_ID;
        pathInfo->edgeInfo[pathInfo->numEdges].endNode = TEST_DCTRL_SYNC_OUT_NODE_ID;
        pathInfo->numEdges++;
#endif

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_PATH,
        pathInfo,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("Dctrl Set Path IOCTL Failed!!!\r\n");
    }

#if(1U == DISP_APP_TEST_MULTISYNC)
    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_VP_PARAMS,
        syncVpParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("Dctrl Set VP Params IOCTL Failed!!!\r\n");
    }

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_ADV_VP_PARAMS,
        syncAdvVpParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("DCTRL Set Advance VP Params IOCTL Failed!!!\r\n");
    }
#endif

#if (1U == DISP_APP_TEST_DSI)
    if(FVID2_SOK == retVal)
    {
        Dss_DctrlDsiParams dsiPrms;
        dsiPrms.instId = appObj->dsiPrms.instId;
        dsiPrms.numOfLanes = appObj->dsiPrms.numOfLanes;
        dsiPrms.laneSpeedInKbps = appObj->dsiPrms.laneSpeedInKbps;
#if (1==DISP_APP_TEST_DSI)
    DispApp_cfgAdditionalDsiPeripherals();
#endif
        retVal = Fvid2_control(
            appObj->dctrlHandle,
            IOCTL_DSS_DCTRL_SET_DSI_PARAMS,
            &dsiPrms,
            NULL);
        if(FVID2_SOK != retVal)
        {
            App_print("DSS Set DSI Params IOCTL Failed!!!\r\n");
        }
    }
#endif

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_VP_PARAMS,
        vpParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("Dctrl Set VP Params IOCTL Failed!!!\r\n");
    }

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_ADV_VP_PARAMS,
        advVpParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("DCTRL Set Advance VP Params IOCTL Failed!!!\r\n");
    }


#if defined (SOC_AM65XX)
    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_OLDI_PARAMS,
        oldiParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("DCTRL Set OLDI Params IOCTL Failed!!!\r\n");
    }
#endif

    overlayParams->overlayId = TEST_OVERLAY_ID;
    overlayParams->colorbarEnable = UFALSE;
    overlayParams->overlayCfg.colorKeyEnable = UTRUE;
    overlayParams->overlayCfg.colorKeySel = CSL_DSS_OVERLAY_TRANS_COLOR_DEST;
#if(DISP_APP_YUV420 == DISP_APP_USE_TEST_PARAMS)
    overlayParams->overlayCfg.colorKeySel = CSL_DSS_OVERLAY_TRANS_COLOR_SRC;
#endif
    overlayParams->overlayCfg.backGroundColor = 0xC8C800U;
    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS,
        overlayParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("DCTRL Set Overlay Params IOCTL Failed!!!\r\n");
    }

    layerParams->overlayId = TEST_OVERLAY_ID;
    layerParams->pipeLayerNum[gDispAppTestParams.pipeId[0U]] =
                                                CSL_DSS_OVERLAY_LAYER_NUM_0;
    if(gDispAppTestParams.numTestPipes > 1U)
    {
        for(i = 1U; i<gDispAppTestParams.numTestPipes;i++)
        {
            layerParams->pipeLayerNum[gDispAppTestParams.pipeId[i]] = i;
        }
    }

    if(CSL_DSS_VID_PIPE_ID_MAX > gDispAppTestParams.numTestPipes)
    {
        for(i = gDispAppTestParams.numTestPipes; i < CSL_DSS_VID_PIPE_ID_MAX; i++)
        {
            layerParams->pipeLayerNum[gDispAppTestParams.invalidPipeId[j++]] =
                                                CSL_DSS_OVERLAY_LAYER_INVALID;
        }
    }

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_LAYER_PARAMS,
        layerParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("DCTRL Set Layer Params IOCTL Failed!!!\r\n");
    }

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_GLOBAL_DSS_PARAMS,
        globalDssParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        App_print("DCTRL Set Global DSS Params IOCTL Failed!!!\r\n");
    }

    return (retVal);
}

static int32_t DispApp_pipeCbFxn(Fvid2_Handle handle, void *appData)
{
    int32_t retVal  = FVID2_SOK;
    DispApp_InstObj *instObj = (DispApp_InstObj *) appData;
    GT_assert (DssTrace, (NULL != instObj));
    (void) SemaphoreP_post(instObj->syncSem);

    return (retVal);
}

void App_print(const char *format, ...)
{
    char printBuffer[DISP_APP_PRINT_BUFFER_SIZE];
    va_list arguments;

    /* Start the var args processing. */
    va_start(arguments, format);
    vsnprintf (printBuffer, sizeof(printBuffer), format, arguments);
    DSS_log("%s", printBuffer);
    /* End the var args processing. */
    va_end(arguments);
}

