/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file dss_mst_test.c
 *
 *  \brief DSS sample application that displays two ARGB32 buffers.
 *  Defaults for 2 Stream : VP1 - 1080P; VP2 - 720P
 *  Defaults for 4 Stream : VP1,VP3 - 1080P; VP2,VP4 - 720P
 */

/* The default is for VP2 and VP4, uncomment any one of the below macro based the VP Choice you want */
// #define MST_TEST_FOR_TWO_STREAMS_VP2_AND_3
#define MST_TEST_FOR_TWO_STREAMS_VP2_AND_4
// #define MST_TEST_FOR_TWO_STREAMS_VP1_AND_3

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/dss/examples/utils/app_utils.h>
#include <dss_mst_buffer1.h>
#include <dss_mst_buffer2.h>

#if defined (MST_TEST_FOR_TWO_STREAMS_VP2_AND_3)
#include <dss_mst_test_vp23.h>
#elif defined (MST_TEST_FOR_TWO_STREAMS_VP2_AND_4)
#include <dss_mst_test_vp24.h>
#elif defined (MST_TEST_FOR_TWO_STREAMS_VP1_AND_3)
#include <dss_mst_test_vp13.h>
#else 
#include <dss_mst_test_vp12.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DSS_MST_TSK_STACK         (10U * 1024U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void DssMst_init();
static void DssMst_deInit(DssMst_testConfig *appObj);
static void DssMst_createDctrl(DssMst_testConfig *appObj, uint32_t streamId);
static void DssMst_delete(DssMst_testConfig *appObj, uint32_t streamId);
static int32_t DssMst_configDctrl(DssMst_testConfig *appObj, uint32_t streamId);
static int32_t DssMst_runTest(void);
static void DssMst_initParams(DssMst_testConfig *appObj, uint32_t streamId);
static int32_t DssMst_allocAndQueueFrames(DssMst_dispInstObj *instObj, uint32_t streamId);
static int32_t DssMst_pipeCbFxn(Fvid2_Handle handle, void *appData);
static int32_t DssMst_commonDctrlConfig(DssMst_testConfig *appObj, uint32_t streamId);
static int32_t DssMst_configureClockN(uint64_t clk1Rate, uint64_t clk2Rate);
static void DssMst_printClkFreq();
void DssMst_frameUpdate(void *a0, void*a1);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint32_t gTestStopTime, gTestStartTime;
DssMst_testConfig gDssTestConfig[DSS_MST_NUM_STREAMS];
SemaphoreP_Handle gDssMstSem;

/* Test application stack */
static uint8_t gStreamTaskStack[DSS_MST_NUM_PIPES_USED][DSS_MST_TSK_STACK];
/* Task handle */
TaskP_Handle gMstStreamTask[DSS_MST_NUM_PIPES_USED];
TaskP_Params streamTaskParams;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * DSS display test
 */
int32_t DssMst_test(void)
{
    int32_t retVal = FVID2_SOK;

    retVal = DssMst_configureClockN(CLOCK1_RATE, CLOCK2_RATE);    
    DssMst_init();

    DssMst_print("DSS display application started...\r\n");

    retVal = DssMst_runTest();

    DssMst_print("Number of frames = %d, elapsed msec = %d, fps = %0.2f\n",
            DSS_MST_RUN_COUNT,
            gTestStopTime - gTestStartTime,
            (float)((float)DSS_MST_RUN_COUNT / ((gTestStopTime - gTestStartTime)/1000.0)));

    DssMst_deInit(&gDssTestConfig[0]);

    if(FVID2_SOK == retVal)
    {
        DssMst_print("DSS display test Passed!!\r\n");
    }
    else
    {
        DssMst_print("DSS display test Failed!!\r\n");
    }

    return retVal;
}

static void DssMst_init()
{
    int32_t         retVal = FVID2_SOK;
    Fvid2_InitPrms  initPrms;
    DssMst_testConfig *appObj = &gDssTestConfig[0];

    Fvid2InitPrms_init(&initPrms);
    initPrms.printFxn = &DssMst_print;
    retVal = Fvid2_init(&initPrms);
    if(FVID2_SOK != retVal)
    {
        DssMst_print("Fvid2 Init Failed!!!\r\n");
    }

    Dss_initParamsInit(&appObj->initParams);
    appObj->initParams.socParams.dpInitParams.isMstEnabled = UTRUE;
    Dss_init(&appObj->initParams);

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
            DssMst_print("DCTRL Create Failed!!!\r\n");
        }
        else
        {
            /* All test objects to use same DCTRL handle */
            for (uint32_t i=1; i < DSS_MST_NUM_STREAMS; i++)
            {
                gDssTestConfig[i].dctrlHandle = appObj->dctrlHandle;
            }
        }
    }

    if(FVID2_SOK == retVal)
    {
        DssMst_print("DssMst_init() - DONE !!!\r\n");
    }

    return;
}

static void DssMst_deInit(DssMst_testConfig *appObj)
{
    int32_t  retVal = FVID2_SOK;

    /* Delete DCTRL handle */
    retVal = Fvid2_delete(appObj->dctrlHandle, NULL);
    retVal += Dss_deInit();
    retVal += Fvid2_deInit(NULL);
    if(FVID2_SOK != retVal)
    {
        DssMst_print("DCTRL handle delete failed!!!\r\n");
    }
    else
    {
        DssMst_print("DssMst_deInit() - DONE !!!\r\n");
    }

    return;
}

static void DssMst_createDisp(DssMst_testConfig *appObj, uint32_t streamId)
{
    uint32_t retVal = FVID2_SOK;
    uint32_t instCnt = 0U;
    DssMst_dispInstObj *instObj;
    SemaphoreP_Params semParams;
    DssMst_initParams(appObj, streamId);

    for(instCnt = 0U; instCnt<gDssMstPipelineParams[streamId].numTestPipes; instCnt++)
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
            DssMst_print("Display Create Failed!!!\r\n");
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
                DssMst_print("DSS Set Params IOCTL Failed!!!\r\n");
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
                DssMst_print("DSS Set Mflag Params IOCTL Failed!!!\r\n");
            }
        }

        if(FVID2_SOK == retVal)
        {
            retVal = DssMst_allocAndQueueFrames(instObj, streamId);
            if(FVID2_SOK != retVal)
            {
                DssMst_print("Display Alloc and Queue Failed!!!\r\n");
            }
        }

        if(FVID2_SOK != retVal)
        {
            break;
        }
    }

    if(FVID2_SOK == retVal)
    {
        DssMst_print("Display create complete!!\r\n");
    }
}

static int32_t DssMst_runTest()
{
    int32_t retVal = FVID2_SOK;
    uint32_t instCnt = 0U;
    DssMst_dispInstObj *instObj;
    SemaphoreP_Params semParams;
    Dss_DctrlDpMstParams mstParams;

    /* DCTRL APIs common to all test objects, using the first object for it */
    DssMst_commonDctrlConfig(&gDssTestConfig[0], 0U);
    /* Create driver */
    for (uint32_t streamId=0U; streamId < DSS_MST_NUM_STREAMS; streamId++)
    {
        DssMst_createDctrl(&gDssTestConfig[streamId], streamId);
    }

    DssMst_testConfig *appObj = &gDssTestConfig[0];
    mstParams.numStreams = DSS_MST_NUM_STREAMS;
    /* VP Ids passed should be in order */
    for (uint32_t streamId=0U; streamId < DSS_MST_NUM_STREAMS; streamId++){
        mstParams.vpIds[streamId] = gDssMstDctrlInfo[streamId].vpId;
    }
    
    /* DP is enabled only after VPs have been configured */
    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_ENABLE_DP_MST,
        &mstParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        DssMst_print("Dctrl MST enable IOCTL Failed!!!\r\n");
    }


    for (uint32_t streamId=0U; streamId < DSS_MST_NUM_STREAMS; streamId++)
    {
        DssMst_createDisp(&gDssTestConfig[streamId], streamId);
    }

    DssMst_print("Starting display ... !!!\r\n");
    DssMst_print("Display in progress ... DO NOT HALT !!!\r\n");

    gTestStartTime = (TimerP_getTimeInUsecs() / 1000U);

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_COUNTING;
    gDssMstSem = SemaphoreP_create(0, &semParams);
    uint32_t index = 0;

    for(uint32_t streamId = 0; streamId < DSS_MST_NUM_STREAMS; streamId++)
    {
        for (uint32_t instCnt = 0U; instCnt < gDssMstPipelineParams[streamId].numTestPipes; instCnt++)
        {
            TaskP_Params_init(&streamTaskParams);
            
            /* Setting priority one less as compared to current executing task */
            streamTaskParams.priority  = 2;
            streamTaskParams.stack     = gStreamTaskStack[index];
            streamTaskParams.stacksize = sizeof(gStreamTaskStack[index]);
            streamTaskParams.arg0      = (void *)&(gDssTestConfig[streamId].instObj[instCnt]);
    
            gMstStreamTask[streamId] = TaskP_create(DssMst_frameUpdate, &streamTaskParams);
            index++;
        }
    }

    /* Start driver */
    for (uint32_t streamId=0; streamId < DSS_MST_NUM_STREAMS; streamId++)
    {
        DssMst_testConfig *appObj = &gDssTestConfig[streamId];
        for(instCnt = 0U; instCnt<gDssMstPipelineParams[streamId].numTestPipes; instCnt++)
        {
            instObj = &appObj->instObj[instCnt];
    
            retVal = Fvid2_start(instObj->drvHandle, NULL);
            if(FVID2_SOK != retVal)
            {
                DssMst_print("Display Start Failed!!!\r\n");
                break;
            }
        }
    }

    for (uint32_t index=0; index < DSS_MST_NUM_PIPES_USED; index++)
    {
        SemaphoreP_pend(gDssMstSem, SemaphoreP_WAIT_FOREVER);
    }

    for (uint32_t streamId = 0; streamId < DSS_MST_NUM_STREAMS; streamId++)
    {
        DssMst_testConfig *appObj = &gDssTestConfig[streamId];
        for(instCnt = 0U; instCnt<gDssMstPipelineParams[streamId].numTestPipes; instCnt++)
        {
            instObj = &appObj->instObj[instCnt];
            retVal  = Fvid2_stop(instObj->drvHandle, NULL);
            if(FVID2_SOK != retVal)
            {
                DssMst_print("Display Stop Failed!!!\r\n");
                break;
            }
        }
    }

    gTestStopTime = (TimerP_getTimeInUsecs() / 1000U);

    if(FVID2_SOK == retVal)
    {
        /* Delete driver */
        for (uint32_t streamId=0; streamId < DSS_MST_NUM_STREAMS; streamId++)
        {
            DssMst_delete(&gDssTestConfig[streamId], streamId);
        }
    }

    return retVal;
}

static int32_t DssMst_commonDctrlConfig(DssMst_testConfig *appObj, uint32_t streamId)
{
    uint32_t retVal = FVID2_SOK;
    uint32_t dpConnectedCmdArg = 0U;
    Dss_DctrlPathInfo *pathInfo;
    Dss_DctrlGlobalDssParams *globalDssParams;
    pathInfo = &gDssMstDctrlPathInfo;
    globalDssParams= &appObj->globalDssParams;

    // Dss_dctrlPathInfoInit(pathInfo); // removed as we don't want init to be done as graph is already set in the global path variable
    Dss_dctrlGlobalDssParamsInit(globalDssParams);

    retVal = Fvid2_control(appObj->dctrlHandle, IOCTL_DSS_DCTRL_IS_DP_CONNECTED, &dpConnectedCmdArg, NULL);
    if ((FVID2_SOK != retVal) || (1U != dpConnectedCmdArg))
    {
        DssMst_print("DP cable not connected !!! \n");
    }
    
    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_PATH,
        pathInfo,
        NULL);
    
    if(FVID2_SOK != retVal)
    {
        DssMst_print("Dctrl Set Path IOCTL Failed!!!\r\n");
    }

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_GLOBAL_DSS_PARAMS,
        globalDssParams,
        NULL);
    
    if(FVID2_SOK != retVal)
    {
        DssMst_print("DCTRL Set Global DSS Params IOCTL Failed!!!\r\n");
    }

    return retVal;
}

static void DssMst_createDctrl(DssMst_testConfig *appObj, uint32_t streamId)
{
    Dss_DctrlVpParams *vpParams;
    Dss_DctrlAdvVpParams *advVpParams;
    vpParams = &appObj->vpParams;
    advVpParams = &appObj->advVpParams;
    Dss_dctrlVpParamsInit(vpParams);
    Dss_dctrlAdvVpParamsInit(advVpParams);

    vpParams->vpId = gDssMstDctrlInfo[streamId].vpId;
    advVpParams->vpId = gDssMstDctrlInfo[streamId].vpId;
    
    vpParams->lcdOpTimingCfg.mInfo.standard = gDssMstDctrlInfo[streamId].standard;
    vpParams->lcdOpTimingCfg.dvoFormat = FVID2_DV_GENERIC_DISCSYNC;
    vpParams->lcdOpTimingCfg.videoIfWidth = FVID2_VIFW_36BIT;
    vpParams->lcdPolarityCfg.actVidPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.hsPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.vsPolarity = FVID2_POL_HIGH;
    vpParams->lcdPolarityCfg.pixelClkPolarity = FVID2_EDGE_POL_RISING;

    advVpParams->lcdAdvSignalCfg.hVAlign = CSL_DSS_VP_HVSYNC_ALIGNED;
    advVpParams->lcdAdvSignalCfg.hVClkControl = CSL_DSS_VP_HVCLK_CONTROL_ON;
    advVpParams->lcdAdvSignalCfg.hVClkRiseFall = FVID2_EDGE_POL_RISING;

    DssMst_configDctrl(appObj, streamId);
    return;
}

void DssMst_frameUpdate(void *a0, void*a1)
{
    uint32_t retVal = FVID2_SOK;
    DssMst_dispInstObj *instObj = (DssMst_dispInstObj *)a0;
    Fvid2_FrameList  frmList;
    
    volatile uint32_t loopCount = 0U;

    while(loopCount++ < DSS_MST_RUN_COUNT)
    {
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
                DssMst_print("Display Queue Failed!!!\r\n");
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
            DssMst_print("Display Dequeue Failed!!!\r\n");
            break;
        }
    }

    SemaphoreP_post(gDssMstSem);
}

static void DssMst_delete(DssMst_testConfig *appObj, uint32_t streamId)
{
    int32_t retVal;
    uint32_t instCnt;
    Dss_DctrlVpParams *vpParams;
    Dss_DctrlPathInfo *pathInfo;
    Dss_DctrlVpErrorStats *pErrorStats;
    DssMst_dispInstObj *instObj;
    Dss_DispCurrentStatus currStatus;
    Fvid2_FrameList frmList;

    vpParams = &appObj->vpParams;
    pathInfo = &appObj->dctrlPathInfo;
    pErrorStats = &appObj->errorStats;

    for(instCnt = 0U; instCnt<gDssMstPipelineParams[streamId].numTestPipes; instCnt++)
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
            DssMst_print("Failed to get Display Stats!!!\r\n");
        }

        /* Print Synclost errors */
        if(0U != currStatus.underflowCount)
        {
            GT_2trace(DssTrace, GT_ERR, "No of Underflows for Inst %d: %d\r\n", instCnt, currStatus.underflowCount);
        }
        else
        {
            DssMst_print("Underflow did not occur\r\n");
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
            DssMst_print("Display Delete Failed!!!\r\n");
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
        DssMst_print("Failed to get VP Stats!!!\r\n");
    }

    /* Print Synclost errors */
    if(0U != pErrorStats->syncLost)
    {
        GT_1trace(DssTrace, GT_ERR, "No of Sync Lost: %d\r\n", pErrorStats->syncLost);
    }
    else
    {
        DssMst_print("Sync Lost did not occur\r\n");
    }

    /* Clear path only after stopping VP, as we validate the output interface is DP MST
       before calling the disableVideoDP API */
    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_STOP_VP,
        vpParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        DssMst_print("VP Stop Failed!!!\r\n");
    }

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_CLEAR_PATH,
        pathInfo,
        NULL);
    if(FVID2_SOK != retVal)
    {
        DssMst_print("Clear Path Failed!!!\r\n");
    }

    if(FVID2_SOK == retVal)
    {
         DssMst_print("Display delete complete!!\r\n");
    }

    return;
}

static int32_t DssMst_allocAndQueueFrames(DssMst_dispInstObj *instObj, uint32_t streamId)
{
    int32_t  retVal = FVID2_SOK;
    uint32_t frmId, numFrames;
    Fvid2_Frame *frm;
    Fvid2_FrameList frmList;

    Fvid2FrameList_init(&frmList);
    frm = &instObj->frames[0U];
    numFrames = DSS_MST_MAX_FRAMES_PER_HANDLE;
    /* init memory pointer for 'numFrames'  */
    for(frmId = 0U; frmId<numFrames; frmId++)
    {
        /* init Fvid2_Frame to 0's  */
        Fvid2Frame_init(&frm[frmId]);
        if(instObj->instId == gDssMstPipelineParams[streamId].instId[0U])
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
            DssMst_print("Display Queue Failed!!!\r\n");
            break;
        }
    }

    return (retVal);
}

static void DssMst_initParams(DssMst_testConfig *appObj, uint32_t streamId)
{
    uint32_t instCnt = 0U, numPipes = 0U, i;
    Dss_DispParams *dispParams;
    DssMst_dispInstObj *instObj;

    numPipes = gDssMstPipelineParams[streamId].numTestPipes;
    for(instCnt = 0U; instCnt < numPipes; instCnt++)
    {
        /* Initialize video pipes */
        instObj = &appObj->instObj[instCnt];
        instObj->instId = gDssMstPipelineParams[streamId].instId[instCnt];

        Dss_dispCreateParamsInit(&instObj->createParams);
        Fvid2CbParams_init(&instObj->cbParams);
        instObj->cbParams.cbFxn = &DssMst_pipeCbFxn;
        instObj->cbParams.appData = instObj;

        dispParams = &instObj->dispParams;
        Dss_dispParamsInit(dispParams);
        dispParams->pipeCfg.pipeType = gDssMstPipelineParams[streamId].pipeType[instCnt];
        dispParams->pipeCfg.inFmt.width = gDssMstPipelineParams[streamId].inWidth[instCnt];
        dispParams->pipeCfg.inFmt.height = gDssMstPipelineParams[streamId].inHeight[instCnt];
        for(i = 0U; i < FVID2_MAX_PLANES; i++)
        {
            dispParams->pipeCfg.inFmt.pitch[i] =
                                        gDssMstPipelineParams[streamId].pitch[instCnt][i];
        }
        dispParams->pipeCfg.inFmt.dataFormat =
                                        gDssMstPipelineParams[streamId].inDataFmt[instCnt];
        dispParams->pipeCfg.inFmt.scanFormat =
                                        gDssMstPipelineParams[streamId].inScanFmt[instCnt];
        dispParams->pipeCfg.outWidth = gDssMstPipelineParams[streamId].outWidth[instCnt];
        dispParams->pipeCfg.outHeight = gDssMstPipelineParams[streamId].outHeight[instCnt];
        dispParams->pipeCfg.scEnable = gDssMstPipelineParams[streamId].scEnable[instCnt];
        dispParams->alphaCfg.globalAlpha =
                                gDssMstPipelineParams[streamId].globalAlpha[instCnt];
        dispParams->alphaCfg.preMultiplyAlpha =
                                gDssMstPipelineParams[streamId].preMultiplyAlpha[instCnt];
        dispParams->layerPos.startX = gDssMstPipelineParams[streamId].posx[instCnt];
        dispParams->layerPos.startY = gDssMstPipelineParams[streamId].posy[instCnt];
        Dss_dispPipeMflagParamsInit(&instObj->mflagParams);
    }
}

static int32_t DssMst_configDctrl(DssMst_testConfig *appObj, uint32_t streamId)
{
    int32_t retVal = FVID2_SOK;
    uint32_t i = 0U, j = 0U;
    Dss_DctrlVpParams *vpParams;
    Dss_DctrlOverlayParams *overlayParams;
    Dss_DctrlOverlayLayerParams *layerParams;
    Dss_DctrlAdvVpParams *advVpParams;
    vpParams = &appObj->vpParams;
    overlayParams = &appObj->overlayParams;
    layerParams = &appObj->layerParams;
    advVpParams = &appObj->advVpParams;

    Dss_dctrlOverlayParamsInit(overlayParams);
    Dss_dctrlOverlayLayerParamsInit(layerParams);

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_VP_PARAMS,
        vpParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        DssMst_print("Dctrl Set VP Params IOCTL Failed!!!\r\n");
    }

    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_ADV_VP_PARAMS,
        advVpParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        DssMst_print("DCTRL Set Advance VP Params IOCTL Failed!!!\r\n");
    }

    overlayParams->overlayId = gDssMstDctrlInfo[streamId].overlayId;
    overlayParams->colorbarEnable = UFALSE;
    overlayParams->overlayCfg.colorKeyEnable = UTRUE;
    overlayParams->overlayCfg.colorKeySel = CSL_DSS_OVERLAY_TRANS_COLOR_DEST;
    overlayParams->overlayCfg.backGroundColor = 0xC8C800U;
    retVal = Fvid2_control(
        appObj->dctrlHandle,
        IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS,
        overlayParams,
        NULL);
    if(FVID2_SOK != retVal)
    {
        DssMst_print("DCTRL Set Overlay Params IOCTL Failed!!!\r\n");
    }

    layerParams->overlayId = gDssMstDctrlInfo[streamId].overlayId;
    layerParams->pipeLayerNum[gDssMstPipelineParams[streamId].pipeId[0U]] =
                                                CSL_DSS_OVERLAY_LAYER_NUM_0;
    if(gDssMstPipelineParams[streamId].numTestPipes > 1U)
    {
        for(i = 1U; i<gDssMstPipelineParams[streamId].numTestPipes;i++)
        {
            layerParams->pipeLayerNum[gDssMstPipelineParams[streamId].pipeId[i]] = i;
        }
    }

    if(CSL_DSS_VID_PIPE_ID_MAX > gDssMstPipelineParams[streamId].numTestPipes)
    {
        for(i = gDssMstPipelineParams[streamId].numTestPipes; i < CSL_DSS_VID_PIPE_ID_MAX; i++)
        {
            layerParams->pipeLayerNum[gDssMstPipelineParams[streamId].invalidPipeId[j++]] =
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
        DssMst_print("DCTRL Set Layer Params IOCTL Failed!!!\r\n");
    }

    return (retVal);
}

static int32_t DssMst_pipeCbFxn(Fvid2_Handle handle, void *appData)
{
    int32_t retVal  = FVID2_SOK;
    DssMst_dispInstObj *instObj = (DssMst_dispInstObj *) appData;
    GT_assert (DssTrace, (NULL != instObj));
    (void) SemaphoreP_post(instObj->syncSem);

    return (retVal);
}

void DssMst_print(const char *format, ...)
{
    char printBuffer[DSS_MST_MAX_PRINT_BUFFER];
    va_list arguments;

    /* Start the var args processing. */
    va_start(arguments, format);
    vsnprintf (printBuffer, sizeof(printBuffer), format, arguments);
    DSS_MST_LOG("%s", printBuffer);
    /* End the var args processing. */
    va_end(arguments);
}

static int32_t DssMst_configureClockN(uint64_t clk1Rate, uint64_t clk2Rate)
{
    int32_t status = CSL_PASS;
    
    /* Configure Clock for VP1 , PLL16 */
    /* Same PLL is used for VP1 and VP3 input */    
    if(CSL_PASS == status)
    {
        /* Power on SerDes module. */
        status = Sciclient_pmSetModuleState(TISCI_DEV_SERDES_10G0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if(CSL_PASS == status)
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
    if(status == CSL_PASS)
    {
        DssMst_print("\n TISCI_DEV_DSS0 device shutdown successful !\r\n");
    } else {
        DssMst_print("\n TISCI_DEV_DSS0 device shutdown NOT successful !!!\r\n");
    }

    if (status == CSL_PASS)
    {
        /* Set the pixel clock. */
        status = Sciclient_pmSetModuleClkFreq(
                                    TISCI_DEV_DSS0,
                                    CLOCK_ID_1,
                                    clk1Rate,
                                    0,
                                    SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    
    if (CSL_PASS == status)
    {
        status = Sciclient_pmSetModuleClkFreq(TISCI_DEV_DSS0,
                    CLOCK_ID_2,
                    clk2Rate,
                    0,
                    SCICLIENT_SERVICE_WAIT_FOREVER);      
    }


    DssMst_printClkFreq();

    status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            SCICLIENT_SERVICE_WAIT_FOREVER);

    return status;
}


static void DssMst_printClkFreq()
{
    uint64_t clkFreq;
    int32_t status = CSL_PASS;

    printf("Finally getting the following clock frequencies !!! \n");

    status = Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
        TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
        &clkFreq,
        SCICLIENT_SERVICE_WAIT_FOREVER);
    
    if (CSL_PASS == status)
    {
        DssMst_print("\nSciclient_pmGetModuleClkFreq Passed, DPI0 getting clock frequency = %lld Hz \n", clkFreq);
    }
    else
    {
        DssMst_print("\nSciclient_pmGetModuleClkFreq failed for DPI0 clk \n");
    }


    status = Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
        TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK,
        &clkFreq,
        SCICLIENT_SERVICE_WAIT_FOREVER);
    
    if (CSL_PASS == status)
    {
        DssMst_print("\nSciclient_pmGetModuleClkFreq Passed, DPI1 getting clock frequency = %lld Hz \n", clkFreq);
    }
    else
    {
        DssMst_print("\nSciclient_pmGetModuleClkFreq failed for DPI1 clk \n");
    }

    status = Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
        TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
        &clkFreq,
        SCICLIENT_SERVICE_WAIT_FOREVER);
    
    if (CSL_PASS == status)
    {
        DssMst_print("\nSciclient_pmGetModuleClkFreq Passed, DPI2 getting clock frequency = %lld Hz \n", clkFreq);
    }
    else
    {
        DssMst_print("\nSciclient_pmGetModuleClkFreq failed for DPI2 clk \n");
    }


    status = Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
        TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK,
        &clkFreq,
        SCICLIENT_SERVICE_WAIT_FOREVER);
    
    if (CSL_PASS == status)
    {
        DssMst_print("\nSciclient_pmGetModuleClkFreq Passed, DPI3 getting clock frequency = %lld Hz \n", clkFreq);
    }
    else
    {
        DssMst_print("\nSciclient_pmGetModuleClkFreq failed for DPI3 clk \n");
    }
}