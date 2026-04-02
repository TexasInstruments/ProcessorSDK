/******************************************************************************
 * Copyright (c) 2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   csirx_capture.c
 *
 *  \brief  csirx diagnostic test file.
 *
 *  Targeted Functionality: Verification of csirx interface.
 *
 *  Operation: This test verifies CSIRX interface by recieving the frames
               transmitted from CSITX interface.
 *
 *  Supported SoCs: J784S4.
 *
 *  Supported Platforms: j784s4_evm.
 */

#include "csitx_rx_cfg.h"

#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)

extern BoardDiag_CsiTxObj csirxObj;

/* Memory buffer to hold data */
extern uint8_t gFrmDropBuf[(BOARD_DIAG_TX_FRAME_PITCH)];
extern uint8_t gRxFrms[(BOARD_DIAG_RX_FRAMES_PER_CH * BOARD_DIAG_TX_CH_NUM)][BOARD_DIAG_TX_FRAME_SIZE];
uint8_t gCompareFrame = 1U;
SemaphoreP_Handle gRxLockSem;

/*
 * UDMA driver objects
 */
extern struct Udma_DrvObj gUdmaDrvObj;

/**
 * \brief   BoardDiag Callback function for frame completion.
 *
 * \param   handle        Fvid2 DRV handle
 *
 * \param   appData       BoardDiag based back by to CB function
 *
 * \param   reserved      reserved, not used
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_captFrameCompletionCb(Fvid2_Handle handle,
                                     Ptr appData,
                                     Ptr reserved)
{
    int32_t  retVal = FVID2_SOK;
    uint32_t frmIdx = 0U, idx = 0U;
    Fvid2_FrameList frmList;
    Fvid2_Frame *pFrm;
    BoardDiag_CsiTxObj *csirxObj = (BoardDiag_CsiTxObj *) appData;
    volatile uint32_t tempVar;

    Fvid2FrameList_init(&frmList);
    retVal = Fvid2_dequeue(
        csirxObj->captObj.drvHandle,
        &frmList,
        0U,
        FVID2_TIMEOUT_NONE);
    if (FVID2_SOK == retVal)
    {
        csirxObj->captObj.numFramesRcvd += frmList.numFrames;
        for (frmIdx = 0; frmIdx < frmList.numFrames; frmIdx++)
        {
            uint8_t *pData;
            int32_t chIndex;
            pFrm = frmList.frames[frmIdx];
            pData = (uint8_t *)pFrm->addr[0];
            csirxObj->captObj.chFrmCnt[pFrm->chNum]++;
            if (FVID2_FRAME_STATUS_COMPLETED != pFrm->status)
            {
                idx = (csirxObj->captObj.frameErrorCnt % BOARD_DIAG_ERR_FRAME_LOG_MAX);
                csirxObj->captObj.errFrmCh[idx] = pFrm->chNum;
                csirxObj->captObj.errFrmNo[idx] = csirxObj->captObj.chFrmCnt[pFrm->chNum];
                csirxObj->captObj.errFrmTs[idx] = (uint32_t)(pFrm->timeStamp64 / 1000U);
                csirxObj->captObj.frameErrorCnt++;
            }
            CacheP_Inv(pData, BOARD_DIAG_TX_FRAME_SIZE);
            chIndex = (((uint32_t)pData[0])<<24) | (((uint32_t)pData[1])<<16) | (((uint32_t)pData[2]) << 8) | ((uint32_t)pData[3]);
            if (0 != memcmp(pData, &gTxFrms[chIndex][0], BOARD_DIAG_TX_FRAME_SIZE))
            {
                gCompareFrame = 0U;
                UART_printf("Channel->%d, NumTx in Channel->%d, numFrames->%d!!!\r\n", pFrm->chNum, csirxObj->captObj.chFrmCnt[pFrm->chNum], frmList.numFrames);
                UART_printf("Data does not match!!!\r\n");
            }
        }

        /* Queue back de-queued frames,
           last param i.e. streamId is unused in DRV */
        retVal = Fvid2_queue(csirxObj->captObj.drvHandle, &frmList, 0U);
        if (FVID2_SOK != retVal)
        {
            UART_printf(" Capture Queue Failed!!!\r\n");
        }
    }

    /* always return 'FVID2_SOK' */
    /* Post semaphore here for sending next Image */
    SemaphoreP_post(gRxLockSem);

    return FVID2_SOK;
}

/**
 * \brief   This function is used to initialize test parameters
 *
 * \param   csirxObj          Type of print message.
 *                          Refer to struct #BoardDiag_CsiTxObj
 *
 * \retval  none.
 */
void BoardDiag_initCaptParams(BoardDiag_CsiTxObj *csirxObj)
{
    uint32_t loopCnt = 0U;

    /* set instance to be used for capture */
    csirxObj->captObj.instId = CSIRX_INSTANCE_ID_0;
    /* set instance initialization parameters */
    /* This will be updated once UDMA init is done */
    Csirx_initParamsInit(&csirxObj->captObj.initPrms);
    /* set instance configuration parameters */
    Csirx_createParamsInit(&csirxObj->captObj.createPrms);
    csirxObj->captObj.createPrms.numCh = BOARD_DIAG_TX_CH_NUM;
    /* set channel configuration parameters */
    for (loopCnt = 0U ; loopCnt < csirxObj->captObj.createPrms.numCh ; loopCnt++)
    {
        csirxObj->captObj.chFrmCnt[loopCnt] = 0U;
        csirxObj->captObj.createPrms.chCfg[loopCnt].chId = loopCnt;
        csirxObj->captObj.createPrms.chCfg[loopCnt].chType = CSIRX_CH_TYPE_CAPT;
        csirxObj->captObj.createPrms.chCfg[loopCnt].vcNum = loopCnt;
        csirxObj->captObj.createPrms.chCfg[loopCnt].inCsiDataType = BOARD_DIAG_TX_IMAGE_DT;
        csirxObj->captObj.createPrms.chCfg[loopCnt].outFmt.width = BOARD_DIAG_TX_FRAME_WIDTH;
        csirxObj->captObj.createPrms.chCfg[loopCnt].outFmt.height = BOARD_DIAG_TX_FRAME_HEIGHT;
        csirxObj->captObj.createPrms.chCfg[loopCnt].outFmt.pitch[0U] =
                                                BOARD_DIAG_TX_FRAME_PITCH;
        csirxObj->captObj.createPrms.chCfg[loopCnt].outFmt.dataFormat =
                                                FVID2_DF_BGRX32_8888;
        csirxObj->captObj.createPrms.chCfg[loopCnt].outFmt.ccsFormat =
                                                BOARD_DIAG_TX_IMAGE_STORAGE_FORMAT;
    }
    /* set module configuration parameters */
    csirxObj->captObj.createPrms.instCfg.enableCsiv2p0Support = UTRUE;
    csirxObj->captObj.createPrms.instCfg.numDataLanes = 4U;
    csirxObj->captObj.createPrms.instCfg.enableErrbypass = UFALSE;
    csirxObj->captObj.createPrms.instCfg.enableStrm[CSIRX_CAPT_STREAM_ID] = 1U;
    for (loopCnt = 0U ;
         loopCnt < csirxObj->captObj.createPrms.instCfg.numDataLanes ;
         loopCnt++)
    {
        csirxObj->captObj.createPrms.instCfg.dataLanesMap[loopCnt] = (loopCnt + 1U);
    }
    /* set frame drop buffer parameters */
    csirxObj->captObj.createPrms.frameDropBufLen =
                                (BOARD_DIAG_TX_FRAME_WIDTH * BOARD_DIAG_TX_FRAME_BPP);
    csirxObj->captObj.createPrms.frameDropBuf = (uint64_t)&gFrmDropBuf;
    /* This will be updated once Fvid2_create() is done */
    csirxObj->captObj.createStatus.retVal = FVID2_SOK;
    csirxObj->captObj.drvHandle = NULL;
    Fvid2CbParams_init(&csirxObj->captObj.cbPrms);
    csirxObj->captObj.cbPrms.cbFxn   = (Fvid2_CbFxn) &BoardDiag_captFrameCompletionCb;
    csirxObj->captObj.cbPrms.appData = csirxObj;

    csirxObj->captObj.numFramesToCapture = BOARD_DIAG_TX_FRAMES_TX;
    csirxObj->captObj.numFramesRcvd = 0U;
    csirxObj->captObj.frameErrorCnt = 0U;

    /* Initialize capture instance status */
    Csirx_instStatusInit(&csirxObj->captObj.captStatus);
}

/**
 * \brief   BoardDiag Init function.
 *
 * \param   csirxObj          CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_captInit(BoardDiag_CsiTxObj *csirxObj)
{
    int32_t retVal = FVID2_SOK;

    /* Csirx init */
    retVal = Csirx_init(&csirxObj->captObj.initPrms);
    if (FVID2_SOK != retVal)
    {
        UART_printf(" Csirx Init Failed!!!\r\n");
    }

    return (retVal);
}

/**
 * \brief   BoardDiag create function.
 *
 * \param   csirxObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_captCreate(BoardDiag_CsiTxObj *csirxObj)
{
    int32_t retVal = FVID2_SOK;
    SemaphoreP_Params semParams;
    Fvid2_TimeStampParams tsParams;
    Csirx_DPhyCfg dphyCfg;

    /* Fvid2_create() */
    csirxObj->captObj.drvHandle = Fvid2_create(
        CSIRX_CAPT_DRV_ID,
        csirxObj->captObj.instId,
        &csirxObj->captObj.createPrms,
        &csirxObj->captObj.createStatus,
        &csirxObj->captObj.cbPrms);

    if ((NULL      == csirxObj->captObj.drvHandle) ||
        (FVID2_SOK != csirxObj->captObj.createStatus.retVal))
    {
        UART_printf( " Capture Create Failed!!!\r\n");
        retVal = csirxObj->captObj.createStatus.retVal;
    }
    if (FVID2_SOK == retVal)
    {
        /* Set CSIRX D-PHY configuration parameters */
        Csirx_initDPhyCfg(&dphyCfg);
        dphyCfg.inst               = CSIRX_INSTANCE_ID_0;
        dphyCfg.leftLaneBandSpeed  = CSIRX_LANE_BAND_SPEED_800_TO_880_MBPS;
        dphyCfg.rightLaneBandSpeed = CSIRX_LANE_BAND_SPEED_800_TO_880_MBPS;
        retVal = Fvid2_control(csirxObj->captObj.drvHandle,
                                IOCTL_CSIRX_SET_DPHY_CONFIG,
                                &dphyCfg,
                                NULL);
        if(FVID2_SOK != retVal)
        {
            UART_printf("Set D-PHY Configuration FAILED!!!\r\n");
        }
        else
        {
            UART_printf("Set D-PHY Configuration Successful!!!\r\n");
        }
    }
    if (FVID2_SOK == retVal)
    {
        /* Allocate instance semaphore */
        SemaphoreP_Params_init(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;
        gRxLockSem = SemaphoreP_create(0U, &semParams);
        if (NULL == gRxLockSem)
        {
            UART_printf(
                "Instance semaphore create failed!!\r\n");
            retVal = FVID2_EALLOC;
        }
    }

    if (FVID2_SOK == retVal)
    {
        tsParams.timeStampFxn = (Fvid2_TimeStampFxn)&TimerP_getTimeInUsecs;
        /* register time stamping function */
        retVal = Fvid2_control(csirxObj->captObj.drvHandle,
                               FVID2_REGISTER_TIMESTAMP_FXN,
                               &tsParams,
                               NULL);
    }
    if (FVID2_SOK == retVal)
    {
        UART_printf("CSIRX Capture created\r\n");
    }
    return (retVal);
}

/**
 * \brief   BoardDiag CSI test function: captures frames.
 *
 * \param   csirxObj        CSI RX Capture Test Object
 *
 * \retval  NULL
 *
 */
void BoardDiag_csirxTest(BoardDiag_CsiTxObj *csirxObj)
{
    /* Wait for reception completion */    

    while (csirxObj->captObj.numFramesRcvd < csirxObj->captObj.numFramesToCapture)
    {
        /* Pend on semaphore until last frame is received */
        SemaphoreP_pend(gRxLockSem, SemaphoreP_WAIT_FOREVER);
        /* Check for Image data here */
    }


    UART_printf("\n%d Frames captured out of %d transferred frames\r\n\n",
          csirxObj->captObj.numFramesRcvd,
          csirxObj->captObj.numFramesToCapture);
}

/**
 * \brief   Board Diag cature delete function.
 *
 * \param   csirxObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_captDelete(BoardDiag_CsiTxObj *csirxObj)
{
    int32_t retVal = FVID2_SOK;
    Fvid2_FrameList frmList;

    Fvid2FrameList_init(&frmList);
    /* Dequeue all the request from the driver */
    retVal = Fvid2_dequeue(
                    csirxObj->captObj.drvHandle,
                    &frmList,
                    0U,
                    FVID2_TIMEOUT_NONE);

    if ((FVID2_SOK != retVal) && (FVID2_ENO_MORE_BUFFERS != retVal))
    {
        UART_printf("Capture De-queue Failed!!!\r\n");
    }
    else
    {
        retVal = Fvid2_delete(csirxObj->captObj.drvHandle, NULL);
        if (FVID2_SOK != retVal)
        {
            UART_printf( " FVID2 Delete Failed!!!\r\n");
        }
        else
        {
            csirxObj->captObj.drvHandle = NULL;
        }

        if (FVID2_SOK == retVal)
        {
            /* Delete semaphore */
            SemaphoreP_delete(gRxLockSem);
        }
    }

    return (retVal);
}

/**
 * \brief   Board Diag Capture De-Init function.
 *
 * \param   csirxObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_captDeinit(BoardDiag_CsiTxObj *csirxObj)
{
    int32_t retVal = FVID2_SOK;

    retVal = Csirx_deInit();

    return (retVal);
}

/**
 * \brief   BoardDiag Callback function for frame completion.
 *
 * \param   csirxObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_captAllocAndQFrames(BoardDiag_CsiTxObj *csirxObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t chIdx = 0U, frmIdx = 0U;
    Fvid2_FrameList frmList;
    Fvid2_Frame  *pFrm;

    /* for every channel in a capture handle,
       allocate memory for and queue frames */
    Fvid2FrameList_init(&frmList);
    frmList.numFrames = 0U;
    for (chIdx = 0U; chIdx < csirxObj->captObj.createPrms.numCh ; chIdx++)
    {
        for (frmIdx = 0U; frmIdx < BOARD_DIAG_RX_FRAMES_PER_CH ; frmIdx++)
        {
            /* assign frames memory */
            /* Only following fields are used in CSIRX DRV */
            pFrm = (Fvid2_Frame *)
                    &csirxObj->captObj.frames[(chIdx * BOARD_DIAG_RX_FRAMES_PER_CH) + frmIdx];
            pFrm->addr[0U] =
               (uint64_t)&gRxFrms[(chIdx * BOARD_DIAG_RX_FRAMES_PER_CH) + frmIdx][0U];
            pFrm->chNum = csirxObj->captObj.createPrms.chCfg[chIdx].chId;
            pFrm->appData = csirxObj;
            frmList.frames[frmList.numFrames] = pFrm;
            frmList.numFrames++;
        }
    }
    /* queue the frames in frmList
     * All allocated frames are queued here as an example.
     * In general at least 2 frames per stream/channel need to queued
     * before capture can be started.
     * Failing which, frame could be dropped.
     */
    /* last parameter, i.e. streamId is unused in CSIRX DRV */
    retVal = Fvid2_queue(csirxObj->captObj.drvHandle, &frmList, 0U);
    if (FVID2_SOK != retVal)
    {
        UART_printf("Capture Queue Failed!!!\r\n");
    }

    return retVal;
}

/**
 * \brief   BoardDiag Callback function for frame completion.
 *
 * \param   csirxObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_captFreeFrames(BoardDiag_CsiTxObj *csirxObj)
{
    int32_t retVal = FVID2_SOK;
    Fvid2_FrameList frmList;

    /* for every stream and channel in a capture handle */
    Fvid2FrameList_init(&frmList);

    /* Deq-queue any frames queued more than needed */
    retVal = Fvid2_dequeue(
                    csirxObj->captObj.drvHandle,
                    &frmList,
                    0U,
                    FVID2_TIMEOUT_NONE);
    if (FVID2_ENO_MORE_BUFFERS == retVal)
    {
        /* All buffer might be de-queued during stop,
           in this case no error shall be returned */
        retVal = FVID2_SOK;
    }
    /* TODO: Free up frame allocated memories here */
    /* it is global variable here so not needed */

    return (retVal);
}
#endif
