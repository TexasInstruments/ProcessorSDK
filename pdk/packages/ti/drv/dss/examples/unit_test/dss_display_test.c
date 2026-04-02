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
 *  \file dss_display_test.c
 *
 *  \brief DSS Unit test application.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>

#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/soc/cslr_soc_ctrl_mmr.h>
#include <ti/drv/sciclient/sciclient.h>

#include <dss_test_display_buffer1.h>
#include <dss_test_display_buffer2.h>
#include <dss_test.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DSS_TEST_MAX_FRAMES_PER_HANDLE  (2U)
#define DSS_TEST_OSAL_DELAY_COUNT       (30000U)
#define DSS_TEST_EXPECTED_FPS           (60.0)
#define DSS_FRAME_CORRUPT_NUM           (200U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t DssTest_init(DssTest_TestObj *test);
void DssTest_deInit(DssTest_TestObj *test);
int32_t DssTest_create(DssTest_TestObj *test);
int32_t DssTest_delete(DssTest_TestObj *test);
int32_t DssTest_runTest(DssTest_TestObj *test);
int32_t DssTest_allocAndQueueFrames(DssTest_TestObj *test, DssTest_PipelineParams *pipelineParams);
int32_t DssTest_pipeCbFxn(Fvid2_Handle handle, void *appData);
int32_t DssTest_configDctrl(DssTest_TestObj *test);
void DssTest_configureSoC(void);
static int32_t DssTest_logSafetyCheckData(DssTest_TestObj *test);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint32_t gDssTestStopTime, gDssTestStartTime;
static volatile uint32_t gLoopCount;
static Dss_safetyTestCbData gSafetyCallbackDataVP[DSS_NUM_SAFETY_REGIONS];
static Dss_safetyTestCbData gSafetyCallbackDataVID;
static Dss_safetyTestCbData gSafetyCallbackDataVIDL; 

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void DssTest_displayTest(DssTest_TestObj *test)
{
    float calcFps, expFps = DSS_TEST_EXPECTED_FPS;   
    int32_t retVal = FVID2_SOK, retVal2 = CSL_PASS;
    test->testResult = CSL_PASS;
    
    if (FVID2_SOK == retVal)
    {
        DssTest_print("DSS display application started...\r\n");
        retVal2 = DssTest_configureClock(test);
        if (CSL_PASS != retVal2)
        {
            retVal = FVID2_EFAIL;
            DssTest_print("Failed to configure LCD...\r\n");
        }
    }

    if (FVID2_SOK == retVal)
    {
        DssTest_configureSoC();
        retVal = DssTest_init(test);
        retVal = DssTest_runTest(test);
    }

    if (FVID2_SOK != retVal)
    {
        test->testResult = CSL_EFAIL;
        test->failReason = DSS_TEST_UNKNOWN_FAILURE;        
    }
    else if (DSS_TEST_COLORBAR != test->feature)
    {
        calcFps = (float)((float)DSS_TEST_RUN_COUNT / ((float)(gDssTestStopTime - gDssTestStartTime)/1000.0));
        DssTest_print("Number of frames = %d, elapsed msec = %d, fps = %0.2f\n",
                DSS_TEST_RUN_COUNT,
                gDssTestStopTime - gDssTestStartTime, calcFps);
        if (((expFps - calcFps) > (0.1f*expFps)) || ((calcFps - expFps) > (0.1f*expFps)))
        {
            test->testResult = CSL_EFAIL;
            test->failReason = DSS_TEST_FPS_FAILURE;
        }
    }

    DssTest_deInit(test);
}

int32_t DssTest_init(DssTest_TestObj *test)
{
    int32_t retVal = FVID2_SOK;
    Fvid2_InitPrms initPrms;

    Fvid2InitPrms_init(&initPrms);
    initPrms.printFxn = &DssTest_print;
    retVal = Fvid2_init(&initPrms);
    if (FVID2_SOK != retVal)
    {
        DssTest_print("Fvid2 Init Failed!!!\r\n");
    }
    else
    {
        Dss_initParamsInit(&gInitParams);
        if (test->feature == DSS_TEST_COMM_REG_1)
        {
            gInitParams.socParams.irqParams.dssCommonRegionId = CSL_DSS_COMM_REG_ID_1;
            gInitParams.socParams.irqParams.numValidIrq = DSS_EVT_MGR_INST_ID_MAX;
            gInitParams.socParams.irqParams.irqNum[DSS_EVT_MGR_INST_ID_FUNC] = 53U;
            gInitParams.socParams.irqParams.irqNum[DSS_EVT_MGR_INST_ID_SAFETY] = 55U;
            gInitParams.socParams.irqParams.irqNum[DSS_EVT_MGR_INST_ID_SECURITY] = 57U;
        }
        gInitParams.socParams.dpInitParams.isHpdSupported = FALSE;
        if (DSS_TEST_EDP != gIntfSelected)
        {
            gInitParams.socParams.dpInitParams.isAvailable = FALSE;
        }

        Dss_init(&gInitParams);
        
        /* Create DCTRL handle, used for common driver configuration */
        gDctrlCfg.dctrlDrvHandle = Fvid2_create(
            gDctrlCfg.dctrlDrvId,
            gDctrlCfg.dctrlInstId,
            NULL,
            NULL,
            NULL);
        if (NULL == gDctrlCfg.dctrlDrvHandle)
        {
            retVal = FVID2_EFAIL;
            DssTest_print("DCTRL Create Failed!!!\r\n");
        }
    }

    if (FVID2_SOK == retVal)
    {
         DssTest_print("DssTest_init() - DONE !!!\r\n");
    }

    return retVal;
}

void DssTest_deInit(DssTest_TestObj *test)
{
    int32_t  retVal = FVID2_SOK;

    retVal = Fvid2_delete(gDctrlCfg.dctrlDrvHandle, NULL);
    if (FVID2_SOK == retVal)
    {
        retVal = Dss_deInit();
    }
    if (FVID2_SOK == retVal)
    {
        retVal = Fvid2_deInit(NULL);
    }
    if (FVID2_SOK != retVal)
    {
        DssTest_print("DssTest_deInit() - FAILED!!!\r\n");
    }
    else
    {
        DssTest_print("DssTest_deInit() - DONE !!!\r\n");
    }

    return;
}

int32_t DssTest_runTest(DssTest_TestObj *test)
{
    int32_t retVal = FVID2_SOK;
    uint32_t instCnt = 0U;
    Fvid2_FrameList  frmList;
    DssTest_PipelineParams *pipelineParams;
    gLoopCount = 0U;

    /* Create driver */
    retVal = DssTest_create(test);
    if (FVID2_SOK == retVal)
    {
        DssTest_print("Starting display ...\r\n");
        DssTest_print("Display in progress ... DO NOT HALT !!!\r\n");
    }
    else
    {
        DssTest_print("Display create failed...\r\n");
    }
    /* Start driver */
    if ((FVID2_SOK == retVal) && (DSS_TEST_COLORBAR != test->feature))
    {
        for (instCnt=0U; instCnt < test->numPipes; instCnt++)
        {
            pipelineParams = &gPipelineParams[instCnt];
            retVal = Fvid2_start(pipelineParams->dispDrvHandle, NULL);
            if (FVID2_SOK != retVal)
            {
                DssTest_print("Display Start Failed!!!\r\n");
                break;
            }
        }
        if(gIntfSelected == DSS_TEST_DSI)
        {
            DispApp_ErrorRegRead();
        }

        if (FVID2_SOK == retVal)
        {
            gDssTestStartTime = (TimerP_getTimeInUsecs() / 1000U);
            while (gLoopCount++ < DSS_TEST_RUN_COUNT)
            {
                for (instCnt=0U; instCnt < test->numPipes; instCnt++)
                {
                    pipelineParams = &gPipelineParams[instCnt];
                    (void) SemaphoreP_pend(pipelineParams->syncSem, SemaphoreP_WAIT_FOREVER);
                    retVal = Fvid2_dequeue(pipelineParams->dispDrvHandle,
                                        &frmList,
                                        0U,
                                        FVID2_TIMEOUT_NONE);
                    /* Corrupt frame to introduce data integrity error */
                    if(test->feature == DSS_TEST_FREEZE_FRAME && gLoopCount == DSS_FRAME_CORRUPT_NUM)
                    {
                        for(uint32_t count = 0; count < test->pipeConfig[instCnt]->inHeight * \
                            test->pipeConfig[instCnt]->pitch[0]; count++)
                        {
                            *(uint8_t *)(frmList.frames[0]->addr[0] + count) = 0xAA;
                        }
                    }

                    if (FVID2_SOK == retVal)
                    {
                        retVal = Fvid2_queue(pipelineParams->dispDrvHandle, &frmList, 0U);
                        if (FVID2_SOK != retVal)
                        {
                            DssTest_print("Display Queue Failed!!!\r\n");
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
                        DssTest_print("Display Dequeue Failed!!!\r\n");
                        break;
                    }
                }
            }
        }

        if (FVID2_SOK == retVal)
        {
            for (instCnt=0U; instCnt < test->numPipes; instCnt++)
            {
                pipelineParams = &gPipelineParams[instCnt];
                retVal  = Fvid2_stop(pipelineParams->dispDrvHandle, NULL);
                if (FVID2_SOK != retVal)
                {
                    DssTest_print("Display Stop Failed!!!\r\n");
                    break;
                }
            }
        }

        gDssTestStopTime = (TimerP_getTimeInUsecs() / 1000U);
    }
    else if ((FVID2_SOK == retVal) && (DSS_TEST_COLORBAR == test->feature))
    {
        Osal_delay(DSS_TEST_OSAL_DELAY_COUNT);
    }
    else
    {
        /* Error already printed. */
    }
    if (FVID2_SOK == retVal)
    {
        /* Delete driver */
        DssTest_delete(test);
    }

    if (test->feature == DSS_TEST_FREEZE_FRAME)
    {
        retVal = DssTest_logSafetyCheckData(test);

        if(CSL_PASS == retVal)
        {
            DssTest_print("DSS Safety Test Passed!!\r\n");
        }
        else
        {
            DssTest_print("DSS Safety Test Failed!!\r\n");
        }
    
    }
    return retVal;
}

int32_t DssTest_create(DssTest_TestObj *test)
{
    int32_t retVal = FVID2_SOK;
    uint32_t instCnt = 0U;
    SemaphoreP_Params semParams;
    DssTest_PipelineParams *pipelineParams;

    retVal = DssTest_configDctrl(test);
    if (FVID2_SOK != retVal)
    {
        DssTest_print("DCTRL congiuration failed...\n\r");
    }

    if ((FVID2_SOK == retVal) && (DSS_TEST_COLORBAR != test->feature))
    {
        for (instCnt=0U; instCnt < test->numPipes; instCnt++)
        {
            pipelineParams = &gPipelineParams[instCnt];
            pipelineParams->cbParamsObj.cbFxn = &DssTest_pipeCbFxn;
            pipelineParams->cbParamsObj.appData = pipelineParams;
            SemaphoreP_Params_init(&semParams);
            semParams.mode = SemaphoreP_Mode_BINARY;
            pipelineParams->syncSem = SemaphoreP_create(0U, &semParams);

            pipelineParams->dispDrvHandle = Fvid2_create(
                pipelineParams->dssDispDrvId,
                pipelineParams->dssDispDrvInstId,
                &pipelineParams->createParamsObj,
                &pipelineParams->createStatusObj,
                &pipelineParams->cbParamsObj);
            if ((NULL == pipelineParams->dispDrvHandle) ||
            (pipelineParams->createStatusObj.retVal != FVID2_SOK))
            {
                DssTest_print("Display Create Failed!!!\r\n");
                retVal = pipelineParams->createStatusObj.retVal;
            }

            if (FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                    pipelineParams->dispDrvHandle,
                    IOCTL_DSS_DISP_SET_DSS_PARAMS,
                    &pipelineParams->dispParamsObj,
                    NULL);
                if (FVID2_SOK != retVal)
                {
                    DssTest_print("DSS Set Params IOCTL Failed!!!\r\n");
                }
            }
        
            if (FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                    pipelineParams->dispDrvHandle,
                    IOCTL_DSS_DISP_SET_PIPE_MFLAG_PARAMS,
                    &pipelineParams->mflagParamsObj,
                    NULL);
                if (FVID2_SOK != retVal)
                {
                    DssTest_print("DSS Set Mflag Params IOCTL Failed!!!\r\n");
                }
            }

            if(FVID2_SOK == retVal)
            {
                if(test->feature == DSS_TEST_FREEZE_FRAME)
                {
                    retVal = Fvid2_control(
                        pipelineParams->dispDrvHandle,
                        IOCTL_DSS_DISP_SET_PIPE_SAFETY_CHK_PARAMS,
                        &(pipelineParams->pipeSafetyObj),
                        NULL);
                    if(retVal != FVID2_SOK)
                    {
                        DssTest_print("DSS Set Safety Params IOCTL Failed!!!\r\n");
                    }
                }
            }

            if (FVID2_SOK == retVal)
            {
                retVal = DssTest_allocAndQueueFrames(test, pipelineParams);
                if (FVID2_SOK != retVal)
                {
                    DssTest_print("Display Alloc and Queue Failed!!!\r\n");
                }
            }

            if (FVID2_SOK != retVal)
            {
                break;
            }
        }

        if (FVID2_SOK == retVal)
        {
            DssTest_print("Display create complete!!\r\n");
        }
    }

    return retVal;
}

int32_t DssTest_allocAndQueueFrames(DssTest_TestObj *test, DssTest_PipelineParams *pipelineParams)
{
    int32_t  retVal = FVID2_SOK;
    uint32_t frmId=0, numFrames, height, width;
    char uartInput;
    Fvid2_Frame *frm;
    Fvid2_FrameList frmList;

    if (test->bufferRequired == DSS_TEST_BUFFER_REQURIED)
    {
        if (gIntfSelected == DSS_TEST_DSI)
        {
            height=800U;
            width=1280U;
            uartInput = '0';
            DssTest_print("Load Image using loadRaw command and then press '1'\n");
            DssTest_print("Command is:\n");
            DssTest_print("loadRaw(0x82000000, 0, \"C:\\\\PSP4_yuyv422_prog_packed_1280_800.yuv\", 32, false);\n");
            do
            {
                scanf("%c", &uartInput);
            } while ('1' != uartInput);
        }
        else
        {
            height=1080U;
            width=1920U;
            uartInput = '0';
            DssTest_print("Load Image using loadRaw command and then press '1'\n");
            DssTest_print("Command is:\n");
            DssTest_print("loadRaw(0x82000000, 0, \"C:\\\\display_bgr888_prog_packed_1920_1080.tigf\", 32, false);\n");
            do
            {
                scanf("%c", &uartInput);
            } while ('1' != uartInput);
        }
    }

    Fvid2FrameList_init(&frmList);
    frm = &pipelineParams->frames[0U];
    numFrames = DSS_TEST_MAX_FRAMES_PER_HANDLE;
    /* init memory pointer for 'numFrames'  */
    for (frmId=0U; frmId < numFrames; frmId++)
    {
        /* init Fvid2_Frame to 0's  */
        Fvid2Frame_init(&frm[frmId]);
        if (test->bufferRequired == DSS_TEST_BUFFER_REQURIED)
        {
            /* Checking for the first pipe data format only */
            if (test->inDataFmt[0] == FVID2_DF_YUV420SP_UV)
            {
                frm[frmId].addr[0U] =
                            (uint64_t)(DSS_TEST_DDR_LOAD_ADDRESS +
                                    frmId*height*width*3U/2U);
                frm[frmId].addr[1U] =
                                (uint64_t)(DSS_TEST_DDR_LOAD_ADDRESS +
                                        frmId*height*width*3U/2U +
                                        height*width);
            }
            else if (test->inDataFmt[0] == FVID2_DF_YUV420SP_UV)
            {
                frm[frmId].addr[0U] =
                                (uint64_t)(DSS_TEST_DDR_LOAD_ADDRESS);
                frm[frmId].addr[1U] =
                                (uint64_t)(DSS_TEST_DDR_LOAD_ADDRESS +
                                        height*width*2U);
            }
            else
            {    frm[frmId].addr[0U] =
                                (uint64_t)(DSS_TEST_DDR_LOAD_ADDRESS +
                                            frmId*height*width*(test->pipeConfig[0]->bpp));

            }
        }
        else
        {
            if (0U == frmId)
            {
                frm[frmId].addr[0U] = (uint64_t) gDssTestDispArray1;
            }
            else
            {
                /* Using same frame in case of freeze frame detection */
                if (test->feature == DSS_TEST_FREEZE_FRAME)
                {
                    frm[frmId].addr[0U] = (uint64_t) gDssTestDispArray1;
                }
                else
                {
                    frm[frmId].addr[0U] = (uint64_t) gDssTestDispArray2;
                }
            }
        }
        frm[frmId].fid = FVID2_FID_FRAME;
        frm[frmId].appData = test;

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
        retVal = Fvid2_queue(pipelineParams->dispDrvHandle, &frmList, 0U);
        if (FVID2_SOK != retVal)
        {
            DssTest_print("Display Queue Failed!!!\r\n");
            break;
        }
    }

    return (retVal);
}

int32_t DssTest_configDctrl(DssTest_TestObj *test)
{
    int32_t retVal = FVID2_SOK;

    if (DSS_TEST_COLORBAR != test->feature)
    {
        retVal = Fvid2_control(gDctrlCfg.dctrlDrvHandle,
                        IOCTL_DSS_DCTRL_SET_PATH,
                        &gDctrlCfg.pathInfoObj,
                        NULL);
        if (FVID2_SOK != retVal)
        {
            DssTest_print("Dctrl Set Path IOCTL Failed!!!\r\n");
        }
    
        if (test->feature == DSS_TEST_MULTISYNC)
        {
            retVal = Fvid2_control(
                gDctrlCfg.dctrlDrvHandle,
                IOCTL_DSS_DCTRL_SET_VP_PARAMS,
                &gDctrlCfg.syncVpParamsObj,
                NULL);
            if (FVID2_SOK != retVal)
            {
                DssTest_print("Dctrl Set VP Params IOCTL Failed!!!\r\n");
            }

            retVal = Fvid2_control(
                gDctrlCfg.dctrlDrvHandle,
                IOCTL_DSS_DCTRL_SET_ADV_VP_PARAMS,
                &gDctrlCfg.syncAdvVpParamsObj,
                NULL);
            if (FVID2_SOK != retVal)
            {
                DssTest_print("DCTRL Set Advance VP Params IOCTL Failed!!!\r\n");
            }
        }

        if (gIntfSelected == DSS_TEST_DSI)
        {
            DispApp_cfgAdditionalDsiPeripherals(test->lcdOutRes);
            if (FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                    gDctrlCfg.dctrlDrvHandle,
                    IOCTL_DSS_DCTRL_SET_DSI_PARAMS,
                    &gDctrlCfg.dsiParamsObj,
                    NULL);
                if (FVID2_SOK != retVal)
                {
                    DssTest_print("DSS Set DSI Params IOCTL Failed!!!\r\n");
                }
            }
        }
    }

    retVal = Fvid2_control(
        gDctrlCfg.dctrlDrvHandle,
        IOCTL_DSS_DCTRL_SET_VP_PARAMS,
        &gDctrlCfg.vpParamsObj,
        NULL);
    if (FVID2_SOK != retVal)
    {
        DssTest_print("Dctrl Set VP Params IOCTL Failed!!!\r\n");
    }
    if (DSS_TEST_COLORBAR != test->feature)
    {
        retVal = Fvid2_control(
            gDctrlCfg.dctrlDrvHandle,
            IOCTL_DSS_DCTRL_SET_ADV_VP_PARAMS,
            &gDctrlCfg.advVpParamsObj,
            NULL);
        if (FVID2_SOK != retVal)
        {
            DssTest_print("DCTRL Set Advance VP Params IOCTL Failed!!!\r\n");
        }
    }
#if defined (SOC_J721E) || defined (SOC_J721S2)
    else
    {
        uint32_t regVal2;

        /* Select DPI0 connection */
        regVal2 = CSL_REG32_RD(CSL_DSS0_DISPC_0_COMMON_M_BASE +
                            CSL_DSS_COMMON_M_DISPC_CONNECTIONS);
        CSL_FINS(regVal2,
                DSS_COMMON_M_DISPC_CONNECTIONS_DPI_0_CONN,
                CSL_DSS_COMMON_M_DISPC_CONNECTIONS_DPI_0_CONN_VAL_VP2);
        CSL_REG32_WR(CSL_DSS0_DISPC_0_COMMON_M_BASE +
                 CSL_DSS_COMMON_M_DISPC_CONNECTIONS, regVal2);
    }
#endif    
    retVal = Fvid2_control(
        gDctrlCfg.dctrlDrvHandle,
        IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS,
        &gDctrlCfg.overlayParamsObj,
        NULL);
    if (FVID2_SOK != retVal)
    {
        DssTest_print("DCTRL Set Overlay Params IOCTL Failed!!!\r\n");
    }
    if (DSS_TEST_COLORBAR != test->feature)
    {
        retVal = Fvid2_control(
            gDctrlCfg.dctrlDrvHandle,
            IOCTL_DSS_DCTRL_SET_LAYER_PARAMS,
            &gDctrlCfg.layerParamsObj,
            NULL);
        if (FVID2_SOK != retVal)
        {
            DssTest_print("DCTRL Set Layer Params IOCTL Failed!!!\r\n");
        }

        retVal = Fvid2_control(
            gDctrlCfg.dctrlDrvHandle,
            IOCTL_DSS_DCTRL_SET_GLOBAL_DSS_PARAMS,
            &gDctrlCfg.globalDssParamsObj,
            NULL);
        if (FVID2_SOK != retVal)
        {
            DssTest_print("DCTRL Set Global DSS Params IOCTL Failed!!!\r\n");
        }

        if (test->feature == DSS_TEST_FREEZE_FRAME)
        {
            Dss_DctrlVpSafetyChkParams *vpSafetyParams;
            vpSafetyParams = (Dss_DctrlVpSafetyChkParams*)&gDctrlCfg.vpSafetyObj;
            for(uint8_t count = 0; count < DSS_NUM_SAFETY_REGIONS; count++)
            {
                retVal = Fvid2_control(
                    gDctrlCfg.dctrlDrvHandle,
                    IOCTL_DSS_DCTRL_SET_VP_SAFETY_CHK_PARAMS,
                    vpSafetyParams + count,
                    NULL);
                if(retVal != FVID2_SOK)
                {
                    DssTest_print("DCTRL Set Safety Params IOCTL Failed!!!\r\n");
                    break;
                }
            }
        }
    }

    return (retVal);
}

int32_t DssTest_delete(DssTest_TestObj *test)
{
    int32_t retVal;
    uint32_t instCnt;
    Dss_DctrlVpParams *vpParams;
    Dss_DctrlVpParams *syncVpParams;
    Dss_DctrlPathInfo *pathInfo;
    Dss_DctrlVpErrorStats *pErrorStats;
    Dss_DispCurrentStatus currStatus;
    Fvid2_FrameList frmList;
    DssTest_PipelineParams *pipelineParams;

    vpParams = &gDctrlCfg.vpParamsObj;
    syncVpParams = &gDctrlCfg.syncVpParamsObj;
    pathInfo = &gDctrlCfg.pathInfoObj;
    pErrorStats = &gDctrlCfg.errorStatsObj;
    if (DSS_TEST_COLORBAR != test->feature)
    {
        for (instCnt=0U; instCnt < test->numPipes; instCnt++)
        {
            pipelineParams = &gPipelineParams[instCnt];

            /* Check for DSS underflow errors */
            retVal = Fvid2_control(
                pipelineParams->dispDrvHandle,
                IOCTL_DSS_DISP_GET_CURRENT_STATUS,
                &currStatus,
                NULL);
            if (FVID2_SOK != retVal)
            {
                DssTest_print("Failed to get Display Stats!!!\r\n");
            }

            /* Print Synclost errors */
            if (0U != currStatus.underflowCount)
            {
                GT_2trace(DssTrace, GT_ERR, "No of Underflows for Inst %d: %d\r\n", instCnt, currStatus.underflowCount);
            }
            else
            {
                DssTest_print("Underflow did not occur\r\n");
            }

            /* Dequeue all the request from the driver */
            while (1U)
            {
                retVal = Fvid2_dequeue(
                    pipelineParams->dispDrvHandle,
                    &frmList,
                    0U,
                    FVID2_TIMEOUT_NONE);
                if (FVID2_SOK != retVal)
                {
                    break;
                }
            }

            retVal = Fvid2_delete(pipelineParams->dispDrvHandle, NULL);
            if (FVID2_SOK != retVal)
            {
                DssTest_print("Display Delete Failed!!!\r\n");
                break;
            }
        }
    }
    /* Check for DSS synclost errors */
    retVal = Fvid2_control(
        gDctrlCfg.dctrlDrvHandle,
        IOCTL_DSS_DCTRL_GET_VP_ERROR_STATS,
        pErrorStats,
        NULL);
    if (FVID2_SOK != retVal)
    {
        DssTest_print("Failed to get VP Stats!!!\r\n");
    }

    /* Print Synclost errors */
    if (0U != pErrorStats->syncLost)
    {
        GT_1trace(DssTrace, GT_ERR, "No of Sync Lost: %d\r\n", pErrorStats->syncLost);
    }
    else
    {
        DssTest_print("Sync Lost did not occur\r\n");
    }

    if (test->feature == DSS_TEST_MULTISYNC)
    {
        retVal = Fvid2_control(
        gDctrlCfg.dctrlDrvHandle,
        IOCTL_DSS_DCTRL_STOP_VP,
        syncVpParams,
        NULL);
        if (FVID2_SOK != retVal)
        {
            DssTest_print("VP Stop Failed!!!\r\n");
        }
    }

    retVal = Fvid2_control(
        gDctrlCfg.dctrlDrvHandle,
        IOCTL_DSS_DCTRL_STOP_VP,
        vpParams,
        NULL);
    if (FVID2_SOK != retVal)
    {
        DssTest_print("VP Stop Failed!!!\r\n");
    }

    retVal = Fvid2_control(
        gDctrlCfg.dctrlDrvHandle,
        IOCTL_DSS_DCTRL_CLEAR_PATH,
        pathInfo,
        NULL);
    if (FVID2_SOK != retVal)
    {
        DssTest_print("Clear Path Failed!!!\r\n");
    }
    else
    {
         DssTest_print("Display delete complete!!\r\n");
    }

    return retVal;
}

int32_t DssTest_pipeCbFxn(Fvid2_Handle handle, void *appData)
{
    int32_t retVal  = FVID2_SOK;
    DssTest_PipelineParams *dispObj = (DssTest_PipelineParams *) appData;
    GT_assert (DssTrace, (NULL != dispObj));
    (void) SemaphoreP_post(dispObj->syncSem);

    return (retVal);
}

void DssTest_configureSoC(void)
{
    /* Set drive strength */
    CSL_REG32_WR(CSL_WKUP_CTRL_MMR0_CFG0_BASE +
                 CSL_WKUP_CTRL_MMR_CFG0_H_IO_DRVSTRNGTH0_PROXY, 0xFU);

    CSL_REG32_WR(CSL_WKUP_CTRL_MMR0_CFG0_BASE +
                 CSL_WKUP_CTRL_MMR_CFG0_V_IO_DRVSTRNGTH0_PROXY, 0xFU);
}

void DssTest_print(const char *format, ...)
{
    char printBuffer[DSS_TEST_PRINT_BUFFER_SIZE];
    va_list arguments;

    /* Start the var args processing. */
    va_start(arguments, format);
    vsnprintf (printBuffer, sizeof(printBuffer), format, arguments);
    UART_printf("%s\n", printBuffer);
#if (1U == DSS_TEST_CIO_ENABLE)
    printf("%s\n\r", printBuffer);
#endif
    /* End the var args processing. */
    va_end(arguments);
}

void DssTest_vpSafetyErrCallback(uint32_t vpID,
                                        Dss_DctrlVpSafetyCbData safetyCbData,
                                        void *appData)
{
    if(gSafetyCallbackDataVP[safetyCbData.regionId].safetyInterupt != true && gLoopCount < DSS_TEST_RUN_COUNT)
    {
        gSafetyCallbackDataVP[safetyCbData.regionId].safetyInterupt = true;
        gSafetyCallbackDataVP[safetyCbData.regionId].vpId = vpID;
        gSafetyCallbackDataVP[safetyCbData.regionId].frameCount = gLoopCount;
        gSafetyCallbackDataVP[safetyCbData.regionId].safetyCheckMode = *(uint32_t *)appData;
        gSafetyCallbackDataVP[safetyCbData.regionId].cbData.capturedSign = safetyCbData.capturedSign;
        gSafetyCallbackDataVP[safetyCbData.regionId].cbData.regionId = safetyCbData.regionId;
    }

}

void DssTest_pipelineSafetyErrCallback(uint32_t pipeId,
                                              uint32_t capturedSign,
                                              void *appData)
{
    if(pipeId == CSL_DSS_VID_PIPE_ID_VID1)
    {
        if(gSafetyCallbackDataVID.safetyInterupt != true && gLoopCount < DSS_TEST_RUN_COUNT)
        {
            gSafetyCallbackDataVID.safetyInterupt = true;
            gSafetyCallbackDataVID.pipeId = pipeId;
            gSafetyCallbackDataVID.frameCount = gLoopCount;
            gSafetyCallbackDataVID.safetyCheckMode = *(uint32_t *)appData;
            gSafetyCallbackDataVID.cbData.capturedSign = capturedSign;
        }
    }
    else if(pipeId == CSL_DSS_VID_PIPE_ID_VIDL1)
    {
        if(gSafetyCallbackDataVIDL.safetyInterupt != true && gLoopCount < DSS_TEST_RUN_COUNT)
        {
            gSafetyCallbackDataVIDL.safetyInterupt = true;
            gSafetyCallbackDataVIDL.pipeId = pipeId;
            gSafetyCallbackDataVIDL.frameCount = gLoopCount;
            gSafetyCallbackDataVIDL.safetyCheckMode = *(uint32_t *)appData;
            gSafetyCallbackDataVIDL.cbData.capturedSign = capturedSign;
        }
    }

}

static int32_t DssTest_logSafetyCheckData(DssTest_TestObj *test)
{
    int32_t status = CSL_PASS;
    uint32_t count;

    for(count = 0U; count < DSS_NUM_SAFETY_REGIONS; count++)
    {
        DssTest_print("****************************************************\r\n");

        DssTest_print("Safety Data Log Region : %d\r\n", gSafetyCallbackDataVP[count].cbData.regionId);
        DssTest_print("Safety Data VP ID: %s\r\n",gSafetyCallbackDataVP[count].vpId == CSL_DSS_VP_ID_1 ? "VP1" : "Invalid VP");

        if(gSafetyCallbackDataVP[count].safetyInterupt == true)
        {
            DssTest_print("Safety Check Interupt Trigger: TRUE\r\n");

            if(gSafetyCallbackDataVP[count].safetyCheckMode == CSL_DSS_SAFETY_CHK_FRAME_FREEZE_DETECT)
            {
                DssTest_print("Safety Check Mode: FRAME_FREEZE_DETECT\r\n");

                if(gSafetyCallbackDataVP[count].frameCount < gDssVpSafetyParams[count].regionSafetyChkCfg.safetyChkCfg.thresholdValue)
                {
                    status = CSL_EFAIL;
                }
            }
            else if(gSafetyCallbackDataVP[count].safetyCheckMode == CSL_DSS_SAFETY_CHK_DATA_INTEGRITY)
            {
                DssTest_print("Safety Check Mode: DATA_INTEGRITY\r\n");
            }

            DssTest_print("Safety Data Frame count: %d\r\n", gSafetyCallbackDataVP[count].frameCount);
            DssTest_print("Safety Data Captured Signature: 0x%lX\r\n", gSafetyCallbackDataVP[count].cbData.capturedSign);
        }
        else
        {
            status = CSL_EFAIL;
            DssTest_print("Safety Check Interupt Trigger: FALSE\r\n");
        }

    }

    for(uint32_t instCnt=0U; instCnt<test->numPipes; instCnt++)
    {
       
        if(test->pipeId[instCnt] == CSL_DSS_VID_PIPE_ID_VID1)
        {
            DssTest_print("****************************************************\r\n");

            DssTest_print("Safety Data Pipeline ID: VID1\r\n");

            if(gSafetyCallbackDataVID.safetyInterupt == true)
            {
                DssTest_print("Safety Check Interupt Trigger: TRUE\r\n");

                if(gSafetyCallbackDataVID.safetyCheckMode == CSL_DSS_SAFETY_CHK_FRAME_FREEZE_DETECT)
                {
                    DssTest_print("Safety Check Mode: FRAME_FREEZE_DETECT\r\n");

                    if(gSafetyCallbackDataVID.frameCount < gDssPipelineSafetyParams[instCnt].safetyChkCfg.thresholdValue)
                    {
                        status = CSL_EFAIL;
                    }
                }
                else if(gSafetyCallbackDataVID.safetyCheckMode == CSL_DSS_SAFETY_CHK_DATA_INTEGRITY)
                {
                    DssTest_print("Safety Check Mode: DATA_INTEGRITY\r\n");
                }

                DssTest_print("Safety Data Frame count: %d\r\n", gSafetyCallbackDataVID.frameCount);
                DssTest_print("Safety Data Captured Signature: 0x%lX\r\n", gSafetyCallbackDataVID.cbData.capturedSign);
            }
            else
            {
                status = CSL_EFAIL;
                DssTest_print("Safety Check Interupt Trigger: FALSE\r\n");
            }

        }
        else if(test->pipeId[instCnt] == CSL_DSS_VID_PIPE_ID_VIDL1)
        {
            DssTest_print("****************************************************\r\n");

            DssTest_print("Safety Data Pipeline ID: VIDL1\r\n");

            if(gSafetyCallbackDataVIDL.safetyInterupt == true)
            {
                DssTest_print("Safety Check Interupt Trigger: TRUE\r\n");

                if(gSafetyCallbackDataVIDL.safetyCheckMode == CSL_DSS_SAFETY_CHK_FRAME_FREEZE_DETECT)
                {
                    DssTest_print("Safety Check Mode: FRAME_FREEZE_DETECT\r\n");

                    if(gSafetyCallbackDataVIDL.frameCount < gDssPipelineSafetyParams[instCnt].safetyChkCfg.thresholdValue)
                    {
                        status = CSL_EFAIL;
                    }
                }
                else if(gSafetyCallbackDataVIDL.safetyCheckMode == CSL_DSS_SAFETY_CHK_DATA_INTEGRITY)
                {
                    DssTest_print("Safety Check Mode: DATA_INTEGRITY\r\n");
                }

                DssTest_print("Safety Data Frame count: %d\r\n", gSafetyCallbackDataVIDL.frameCount);
                DssTest_print("Safety Data Captured Signature: 0x%lX\r\n", gSafetyCallbackDataVIDL.cbData.capturedSign);
            }
            else
            {
                status = CSL_EFAIL;
                DssTest_print("Safety Check Interupt Trigger: FALSE\r\n");
            }

        }
    
    }

    return status;
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif
