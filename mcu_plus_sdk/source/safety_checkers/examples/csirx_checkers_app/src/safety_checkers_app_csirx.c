/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2024
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
 *  \file safety_checkers_app_csirx.c
 *
 *  \brief CSI RX Safety checkers example.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#if defined (SOC_J722S)
#include <string.h>
#include <csirx.h>
#include <kernel/dpl/TimerP.h>
#include <board/board_control.h>
#include "ti_drivers_config.h"
#else
#include <ti/drv/csirx/csirx.h>
#include <ti/board/src/devices/board_devices.h>
#endif
#include "safety_checkers_csirx.h"
#include "safety_checkers_common.h"
#include "safety_checkers_app_csirx.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*None*/

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/*None*/

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* App common object */
SafetyCheckersApp_CsirxCommonObj gSafetyCheckersAppCsirxCommonObj;
extern uint16_t gSafetyCheckersAppCsirxUb960SensorCfg[39][3];
#if defined (SOC_J722S)
extern uint32_t gTimerBaseAddr[TIMER_NUM_INSTANCES];
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Application main
 */
int SafetyCheckersApp_csirxMain(void)
{
    int32_t retVal = FVID2_SOK;
    SafetyCheckersApp_CsirxInstObj *appInstObj;
    SafetyCheckersApp_CsirxCommonObj *appCommonObj;
    appCommonObj = &gSafetyCheckersAppCsirxCommonObj;

    SafetyCheckersApp_csirxSetupApp(appCommonObj);
    appInstObj = &appCommonObj->appInstObj;
    memset(appInstObj, 0x0, sizeof (SafetyCheckersApp_CsirxInstObj));
    appInstObj->instId = SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID;
    SafetyCheckersApp_csirxInitParams(appInstObj);

#if !defined(SOC_J722S)
    SafetyCheckersApp_csirxSetupTimer(appCommonObj);
#endif

    /* App Init */
    retVal += SafetyCheckersApp_csirxInit(appCommonObj);
    if (FVID2_SOK != retVal)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: [ERROR]SafetyCheckersApp_csirxInit() FAILED!!!\r\n");
    }

    /* App Create */
    if (FVID2_SOK == retVal)
    {
       retVal += SafetyCheckersApp_csirxCreate(appInstObj);
       if (FVID2_SOK != retVal)
       {
           SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: [ERROR]SafetyCheckersApp_csirxCreate() FAILED!!!\r\n");
       }
    }

    SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: CSIRX safety checkers Application started\n");

    /*  App start */
    retVal = SafetyCheckersApp_csirxTest(appCommonObj);
    if (FVID2_SOK != retVal)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: [ERROR]SafetyCheckersApp_csirxTest() FAILED!!!\r\n");
    }

    /* App CSI delete function */
    if (FVID2_SOK == retVal)
    {
       retVal += SafetyCheckersApp_csirxDelete(appInstObj);
       if (FVID2_SOK != retVal)
       {
           SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: [ERROR]SafetyCheckersApp_csirxDelete() FAILED!!!\r\n");
       }
    }

    /* App CSI De-initialization function */
    if (FVID2_SOK == retVal)
    {
        retVal += SafetyCheckersApp_csirxDeinit(appCommonObj);
        if (FVID2_SOK != retVal)
        {
            SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: [ERROR]SafetyCheckersApp_csirxDeinit() FAILED!!!\r\n");
        }
    }

#if defined LDRA_DYN_COVERAGE_EXIT
    upload_execution_history();
#endif
    SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: Sample Application - DONE !!!\r\n");

    return (retVal);
}

static int32_t SafetyCheckersApp_csirxTest(SafetyCheckersApp_CsirxCommonObj* appCommonObj)
{
    int32_t retVal = FVID2_SOK;

    if (FVID2_SOK == retVal)
    {
        retVal += Fvid2_start(appCommonObj->appInstObj.drvHandle, NULL);
        if (FVID2_SOK != retVal)
        {
            SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: Capture Start Failed for instance %d!!!\r\n",appCommonObj->appInstObj.instId);
        }
    }

    /* Verify CSIRX configuration with safety checkers */
    retVal = SafetyCheckersApp_csirxVerifyCheckers(&appCommonObj->appInstObj);
    if(SAFETY_CHECKERS_SOK == retVal)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: Verification of safety checkers is successful\n");
    }
    else
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: Verification of safety checkers failed\n");
    }
#if defined (SOC_J722S)
    /* start the tick timer */
    TimerP_start(gTimerBaseAddr[CONFIG_TIMER0]);
    SemaphoreP_pend(&appCommonObj->completionSem, SystemP_WAIT_FOREVER);

    retVal += Fvid2_stop(appCommonObj->appInstObj.drvHandle, NULL);
    if (FVID2_SOK != retVal)
    {
        GT_1trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": Capture Stop Failed for instance %d!!!\r\n", appCommonObj->appInstObj.instId);
    }

#else
    TimerP_start(appCommonObj->timerHandle);
    SemaphoreP_pend(appCommonObj->completionSem, SemaphoreP_WAIT_FOREVER);
#endif


    retVal += SafetyCheckersApp_csirxFreeFrames(&appCommonObj->appInstObj);
    SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: Number of frames recieved is %d\n", appCommonObj->appInstObj.numFramesRcvd);

    return retVal;
}

void SafetyCheckersApp_csirxTimerIsr(void *arg)
{

    SafetyCheckersApp_CsirxCommonObj
	    *appCommonObj=(SafetyCheckersApp_CsirxCommonObj*)&gSafetyCheckersAppCsirxCommonObj;
#if defined (SOC_J722S)
    static volatile uint32_t timerIsrCount=0;
    timerIsrCount++;
    if (timerIsrCount == SAFETY_CHECKERS_APP_CSIRX_TEST_PERIOD_IN_SEC)
    {
        TimerP_stop(gTimerBaseAddr[CONFIG_TIMER0]);
        /* Post semaphore to print the results */
        SemaphoreP_post(&appCommonObj->completionSem);
    }
#else
    int32_t retVal = FVID2_SOK;

    /* Stop the streams immediately after the timeout is reached */
    retVal += Fvid2_stop(appCommonObj->appInstObj.drvHandle, NULL);
    if (FVID2_SOK != retVal)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: Capture Stop Failed for instance %d!!!\r\n", appCommonObj->appInstObj.instId);
    }

    /* Post semaphore to print the results */
    SemaphoreP_post(appCommonObj->completionSem);
#endif

}

static int32_t SafetyCheckersApp_csirxVerifyCheckers(SafetyCheckersApp_CsirxInstObj *appInstObj)
{
    SafetyCheckers_CsirxFdmChannel      *channel   = NULL;
    SafetyCheckers_CsirxVimCfg vimCfg;
#if !defined (SOC_J722S)
    uint8_t i2cInst = 0U;
#endif
    uint8_t i2cAddr = 0U;
    SafetyCheckers_CsirxQoSSettings qosSettings;
    int32_t status = SAFETY_CHECKERS_SOK;
    uint32_t regCfg[SAFETY_CHECKERS_CSIRX_STRM_CTRL_REGS_LENGTH];

    /* Safety checker APIs */
    channel = (SafetyCheckers_CsirxFdmChannel*)(appInstObj->drvHandle);
    
    status = SafetyCheckers_csirxVerifyCsiAvailBandwidth(channel , 30);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : IP limits exceeded for requested resolution and fps!!! \r\n");
    }

    status = SafetyCheckers_csirxGetVimCfg(channel, &vimCfg);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Cannot read vim configuration of CSIRX \r\n");
    }

    status = SafetyCheckers_csirxVerifyVimCfg(channel, &vimCfg);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] CSIRX vim configuration validation failed \r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(regCfg,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_STRM_CTRL,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID );
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Get API for stream control register configuration failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_STRM_CTRL,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Stream control register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(regCfg,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_CONFIG,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Get API for DPHY  register configuration failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_CONFIG,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] DPHY register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(regCfg,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_PLL,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Get API for DPHY PLL register configuration failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_PLL,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] DPHY PLL register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(regCfg,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_VIRTUAL_CHANNEL,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Get API for virtual channel register configuration failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_VIRTUAL_CHANNEL,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Virtual channel register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(regCfg,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_DATATYPE_FRAMESIZE,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Get API for Data type,frame size register configuration failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DATATYPE_FRAMESIZE,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Data type, frame size register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(regCfg,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_LANE_CONFIG,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Get API for DPHY Lane register configuration failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_LANE_CONFIG,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] 1: DPHY Lane register configuration verification failed\r\n");
    }
 
    /* Negative testcases for safety checkers */
    status = SafetyCheckers_csirxVerifyCsiAvailBandwidth(NULL , 30);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed\r\n");
    }

    status = SafetyCheckers_csirxGetVimCfg(NULL, &vimCfg);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed\r\n");
    }

    status = SafetyCheckers_csirxGetVimCfg(channel, NULL);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed\r\n");
    }
    
    status = SafetyCheckers_csirxVerifyVimCfg(NULL, &vimCfg);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyVimCfg(channel, NULL);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }
    
    status = SafetyCheckers_csirxGetRegCfg(NULL,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_STRM_CTRL,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(NULL,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_STRM_CTRL,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(NULL,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_CONFIG,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(NULL,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_CONFIG,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(NULL,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_PLL,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(NULL,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_PLL,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");


    }

    status = SafetyCheckers_csirxGetRegCfg(NULL,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_LANE_CONFIG,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(NULL,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_LANE_CONFIG,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(NULL,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_VIRTUAL_CHANNEL,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(NULL,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_VIRTUAL_CHANNEL,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxGetRegCfg(NULL,
                                           SAFETY_CHECKERS_CSIRX_REG_TYPE_DATATYPE_FRAMESIZE,
                                           SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(NULL,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DATATYPE_FRAMESIZE,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }
    
    status = SafetyCheckers_csirxGetQoSCfg(&qosSettings, channel);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : Get QoS settigs API failed!!!\r\n");
    }

    status = SafetyCheckers_csirxVerifyQoSCfg(&qosSettings, channel);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : QoS settigs verification failed!!!\r\n");
    }

    status = SafetyCheckers_csirxGetQoSCfg(NULL, channel);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyQoSCfg(NULL, channel);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxGetQoSCfg(&qosSettings, NULL);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyQoSCfg(&qosSettings, NULL);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] NULL handle check failed \r\n");
    }

    status = SafetyCheckers_csirxVerifyCsiAvailBandwidth(channel , 400);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : IP limits verification failed \r\n");
    }

    status = SafetyCheckers_csirxGetVimCfg(channel, &vimCfg);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Cannot read vim configuration of CSIRX \r\n");
    }

    vimCfg.pri = 0x5U;
    status = SafetyCheckers_csirxVerifyVimCfg(channel, &vimCfg);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] CSIRX vim configuration validation failed \r\n");
    }

    regCfg[0] = 0xFU;
    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_STRM_CTRL,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Stream control register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_CONFIG,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] DPHY register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_PLL,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: [ERROR] DPHY PLL register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_LANE_CONFIG,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] DPHY Lane register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_VIRTUAL_CHANNEL,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Virtual channel register configuration verification failed\r\n");
    }

    status = SafetyCheckers_csirxVerifyRegCfg(regCfg,
                                              SAFETY_CHECKERS_CSIRX_REG_TYPE_DATATYPE_FRAMESIZE,
                                              SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID);
    if (SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : [ERROR] Data type, frame size register configuration verification failed\r\n");
    }
    
#if defined (SOC_J722S)
    Board_fpdUb960GetI2CAddr(&i2cAddr, appInstObj->boardCsiInstID);
#else
    Board_fpdU960GetI2CAddr(&i2cInst, &i2cAddr, appInstObj->boardCsiInstID);
#endif
    status = SafetyCheckers_csirxGetSensorCfg(appInstObj->i2cHandle, i2cAddr,
                                              gSafetyCheckersAppCsirxUb960SensorCfg);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : Get Sensor configuration failed!!!\r\n");
    }
 
    status = SafetyCheckers_csirxVerifySensorCfg(appInstObj->i2cHandle, i2cAddr,
                                                 gSafetyCheckersAppCsirxUb960SensorCfg);
    if (SAFETY_CHECKERS_SOK != status)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP : Sensor configuration validation failed!!!\r\n");
    }

    return status;
}
