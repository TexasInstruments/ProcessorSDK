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
 *  \file   csitx_transmit.c
 *
 *  \brief  csitx transmit diagnostic test file.
 *
 *  Targeted Functionality: Verification of csitx interface.
 *
 *  Operation: This test verifies CSITX interface by transmitting the frames.
 *
 *  Supported SoCs: J784S4.
 *
 *  Supported Platforms: j784s4_evm.
 */

#include "csitx_rx_cfg.h"

int32_t BoardDiag_init(BoardDiag_CsiTxObj *csitxObj);

int32_t BoardDiag_create(BoardDiag_CsiTxObj *csitxObj);

int32_t BoardDiag_csitxTest(BoardDiag_CsiTxObj *csitxObj);

int32_t BoardDiag_delete(BoardDiag_CsiTxObj *csitxObj);

int32_t BoardDiag_deinit(BoardDiag_CsiTxObj *csitxObj);

int32_t BoardDiag_frameCompletionCb(Fvid2_Handle handle,
                                      Ptr appData,
                                      Ptr reserved);

void BoardDiag_initTxParams(BoardDiag_CsiTxObj *csitxObj);

int32_t BoardDiag_allocAndQFrames(BoardDiag_CsiTxObj *csitxObj);

int32_t BoardDiag_txFreeFrames(BoardDiag_CsiTxObj *csitxObj);

#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
/* Functions from CSIRX capture test application file */
extern void BoardDiag_initCaptParams(BoardDiag_CsiTxObj *csitxObj);
extern int32_t BoardDiag_captInit(BoardDiag_CsiTxObj *csitxObj);
extern int32_t BoardDiag_captCreate(BoardDiag_CsiTxObj *csitxObj);
extern int32_t BoardDiag_captDelete(BoardDiag_CsiTxObj *csitxObj);
extern int32_t BoardDiag_captDeinit(BoardDiag_CsiTxObj *csitxObj);
extern int32_t BoardDiag_captFreeFrames(BoardDiag_CsiTxObj *csitxObj);
extern int32_t BoardDiag_captAllocAndQFrames(BoardDiag_CsiTxObj *csitxObj);
extern void BoardDiag_csirxTest(BoardDiag_CsiTxObj *csitxObj);
#endif

BoardDiag_CsiTxObj gcsitxObj;
/* Memory buffer to hold data */
uint8_t gTxFrms[(BOARD_DIAG_TX_FRAMES_PER_CH * BOARD_DIAG_TX_CH_NUM)][BOARD_DIAG_TX_FRAME_SIZE] __attribute__(( aligned(128), section(".data_buffer")));
#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
uint8_t gFrmDropBuf[(BOARD_DIAG_TX_FRAME_PITCH)] __attribute__(( aligned(128), section(".data_buffer")));
uint8_t gRxFrms[(BOARD_DIAG_RX_FRAMES_PER_CH * BOARD_DIAG_TX_CH_NUM)][BOARD_DIAG_TX_FRAME_SIZE] __attribute__(( aligned(128), section(".data_buffer")));
#endif


/*
 * UDMA driver objects
 */
struct Udma_DrvObj gUdmaDrvObj;

SemaphoreP_Handle gLockSem;

/**
 * \brief   BoardDiag CSI test function: captures frames.
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 *
 */
int BoardDiag_csitxTransmit(void)
{
    int retVal = FVID2_SOK;
    BoardDiag_CsiTxObj *csitxObj;


    csitxObj = &gcsitxObj;
    memset(csitxObj, 0x0, sizeof (BoardDiag_CsiTxObj));

    /* Initialize application object for current transmit context */
    BoardDiag_initTxParams(csitxObj);
    /* BoardDiag Init */
    retVal += BoardDiag_init(csitxObj);

    UART_printf("Tx DF:0x%x\r\n", BOARD_DIAG_TX_IMAGE_DT);
    UART_printf("Tx Resolution:%d x %d\r\n",
              BOARD_DIAG_TX_FRAME_WIDTH,
              BOARD_DIAG_TX_FRAME_HEIGHT);
    if (FVID2_SOK != retVal)
    {
        UART_printf( "BoardDiag_init() FAILED!!!\r\n");
    }

    /* BoardDiag Init */
    if (FVID2_SOK == retVal)
    {
        retVal += BoardDiag_create(csitxObj);
        if (FVID2_SOK != retVal)
        {
            UART_printf( "BoardDiag_create() FAILED!!!\r\n");
        }
    }

    /* BoardDiag CSI test function */
    if (FVID2_SOK == retVal)
    {
        retVal += BoardDiag_csitxTest(csitxObj);
        if (FVID2_SOK != retVal)
        {
            UART_printf("BoardDiag_csitxTest() FAILED!!!\r\n");
        }
    }

    /* BoardDiag CSI delete function */
    if (FVID2_SOK == retVal)
    {
        retVal += BoardDiag_delete(csitxObj);
        if (FVID2_SOK != retVal)
        {
            UART_printf("BoardDiag_delete() FAILED!!!\r\n");
        }
    }

    /* BoardDiag CSI De-initialization function */
    if (FVID2_SOK == retVal)
    {
        retVal += BoardDiag_deinit(csitxObj);
        if (FVID2_SOK != retVal)
        {
            UART_printf("BoardDiag_deinit() FAILED!!!\r\n");
        }
    }

#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    if(1 == gCompareFrame)
    {
		UART_printf("CSI Tx Rx loopback test successful\r\n");
    }
    else
    {
        UART_printf("One or more transmitted frames do not Match the received frames\r\n");
    }
#endif

    return (retVal);
}

/**
 * \brief   This function is used to initialize test parameters
 *
 * \param   csitxObj          Type of print message.
 *                          Refer to struct #BoardDiag_CsiTxObj
 *
 * \retval  none.
 */
void BoardDiag_initTxParams(BoardDiag_CsiTxObj *csitxObj)
{
    uint32_t loopCnt = 0U;

    /* set instance to be used for transmit */
    if (0U == CSITX_INSTANCE)    
    {
        csitxObj->instId = CSITX_INSTANCE_ID_0;
    }
#if defined (SOC_J721S2) || defined (SOC_J784S4)
    else
    {
        csitxObj->instId = CSITX_INSTANCE_ID_1;
    }  
#endif    
    /* set instance initialization parameters */
    /* This will be updated once UDMA init is done */
    Csitx_initParamsInit(&csitxObj->initPrms);
    /* set instance configuration parameters */
    Csitx_createParamsInit(&csitxObj->createPrms);
    if (0U == CSITX_INSTANCE)    
    {
        csitxObj->createPrms.instCfg.dphyCfg.inst = CSITX_INSTANCE_ID_0;
    }  
#if defined (SOC_J721S2) || defined (SOC_J784S4)
    else
    {
        csitxObj->createPrms.instCfg.dphyCfg.inst = CSITX_INSTANCE_ID_1;
    }
#endif    
    csitxObj->createPrms.numCh = BOARD_DIAG_TX_CH_NUM;
    /* set channel configuration parameters */
    for (loopCnt = 0U ; loopCnt < csitxObj->createPrms.numCh ; loopCnt++)
    {
        csitxObj->chFrmCnt[loopCnt]                          = 0U;
        csitxObj->createPrms.chCfg[loopCnt].chId             = loopCnt;
        csitxObj->createPrms.chCfg[loopCnt].chType           = CSITX_CH_TYPE_TX;
        csitxObj->createPrms.chCfg[loopCnt].vcNum            = loopCnt;
        csitxObj->createPrms.chCfg[loopCnt].outCsiDataType   = BOARD_DIAG_TX_IMAGE_DT;
        csitxObj->createPrms.chCfg[loopCnt].inFmt.width      = BOARD_DIAG_TX_FRAME_WIDTH;
        csitxObj->createPrms.chCfg[loopCnt].inFmt.height     = BOARD_DIAG_TX_FRAME_HEIGHT;
        csitxObj->createPrms.chCfg[loopCnt].inFmt.pitch[0U]  =
                                                BOARD_DIAG_TX_FRAME_PITCH;
        csitxObj->createPrms.chCfg[loopCnt].inFmt.dataFormat =
                                                FVID2_DF_BGRX32_8888;
        csitxObj->createPrms.chCfg[loopCnt].inFmt.ccsFormat  =
                                                BOARD_DIAG_TX_IMAGE_STORAGE_FORMAT;
        csitxObj->createPrms.chCfg[loopCnt].vBlank           =
                                            22U;
        csitxObj->createPrms.chCfg[loopCnt].hBlank           =
                                            40U;
        csitxObj->createPrms.chCfg[loopCnt].startDelayPeriod =
                                            40U;
    }
    /* set module configuration parameters */
    csitxObj->createPrms.instCfg.rxCompEnable    = (uint32_t)1U;
    csitxObj->createPrms.instCfg.rxv1p3MapEnable = (uint32_t)1U;
    csitxObj->createPrms.instCfg.numDataLanes    = 4U;
    for (loopCnt = 0U ;
         loopCnt < csitxObj->createPrms.instCfg.numDataLanes ;
         loopCnt++)
    {
        csitxObj->createPrms.instCfg.lanePolarityCtrl[loopCnt] = 0U;
    }
    /* This will be updated once Fvid2_create() is done */
    csitxObj->createStatus.retVal = FVID2_SOK;
    csitxObj->drvHandle           = NULL;
    Fvid2CbParams_init(&csitxObj->cbPrms);
    csitxObj->cbPrms.cbFxn        = (Fvid2_CbFxn) &BoardDiag_frameCompletionCb;
    csitxObj->cbPrms.appData      = csitxObj;

    csitxObj->numFramesToTx = BOARD_DIAG_TX_FRAMES_TX;
    csitxObj->numFramesTx = 0U;
    csitxObj->frameErrorCnt = 0U;
    csitxObj->maxWidth      = BOARD_DIAG_TX_FRAME_WIDTH;
    csitxObj->maxHeight     = BOARD_DIAG_TX_FRAME_HEIGHT;

    /* Initialize transmit instance status */
    Csitx_instStatusInit(&csitxObj->txStatus);
#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    /* Initialize CSIRX capture parameters */
    BoardDiag_initCaptParams(csitxObj);
#endif
}

/**
 * \brief   BoardDiag_init function.
 *
 * \param   csitxObj          CSI Tx Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_init(BoardDiag_CsiTxObj *csitxObj)
{
    int32_t retVal = FVID2_SOK, dmaStatus = UDMA_SOK;
    uint32_t instId;
    Fvid2_InitPrms initPrms;
    Udma_InitPrms   udmaInitPrms;
    Udma_DrvHandle drvHandle = &gUdmaDrvObj;

    csitxObj->initPrms.drvHandle = drvHandle;
    csitxObj->captObj.initPrms.drvHandle = drvHandle;
    /* Fvid2 init */
    Fvid2InitPrms_init(&initPrms);
    retVal = Fvid2_init(&initPrms);
    if (FVID2_SOK != retVal)
    {
        UART_printf( " Fvid2 Init Failed!!!\r\n");
    }

    /* Do UDMA init before CSITX Init */
    /* UDMA driver init */
#if defined (SOC_J721E)
    instId = UDMA_INST_ID_MAIN_0;
#endif
#if defined (SOC_J721S2) || defined (SOC_J784S4)
    instId = UDMA_INST_ID_BCDMA_0;
#endif
    UdmaInitPrms_init(instId, &udmaInitPrms);
    dmaStatus = Udma_init(drvHandle, &udmaInitPrms);
    if(UDMA_SOK != dmaStatus)
    {
        retVal = FVID2_EFAIL;
        UART_printf(" UDMA Init Failed!!!\r\n");
    }

    /* System init */
    retVal = Csitx_init(&csitxObj->initPrms);
    if (FVID2_SOK != retVal)
    {
        UART_printf(" System Init Failed!!!\r\n");
    }

#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    /* CSIRX initialization */
    if (FVID2_SOK == retVal)
    {
        retVal = BoardDiag_captInit(csitxObj);
        if (FVID2_SOK != retVal)
        {
            UART_printf( " CSIRX System Init Failed!!!\r\n");
        }
    }
#endif

    return (retVal);
}

/**
 * \brief   BoardDiag create function.
 *
 * \param   csitxObj        CSI Tx Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_create(BoardDiag_CsiTxObj *csitxObj)
{
    int32_t retVal = FVID2_SOK;
    SemaphoreP_Params semParams;
    Fvid2_TimeStampParams tsParams;

#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    /* CSIRX DRV Create */
    retVal = BoardDiag_captCreate(csitxObj);
    if (FVID2_SOK != retVal)
    {
        UART_printf("CSIRX DRV create FAILED!!!\r\n");
    }
#endif

    if (FVID2_SOK == retVal)
    {
        /* Fvid2_create() */
        csitxObj->drvHandle = Fvid2_create(
            CSITX_TX_DRV_ID,
            csitxObj->instId,
            &csitxObj->createPrms,
            &csitxObj->createStatus,
            &csitxObj->cbPrms);

        if ((NULL      == csitxObj->drvHandle) ||
            (FVID2_SOK != csitxObj->createStatus.retVal))
        {
            UART_printf("Tx Create Failed!!!\r\n");
            retVal = csitxObj->createStatus.retVal;
        }
    }
    if (FVID2_SOK == retVal)
    {
        /* Allocate instance semaphore */
        SemaphoreP_Params_init(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;
        gLockSem = SemaphoreP_create(0U, &semParams);
        if (NULL == gLockSem)
        {
            UART_printf("Instance semaphore create failed!!\r\n");
            retVal = FVID2_EALLOC;
        }
    }

    if (FVID2_SOK == retVal)
    {
        tsParams.timeStampFxn = (Fvid2_TimeStampFxn)&TimerP_getTimeInUsecs;
        /* register time stamping function */
        retVal = Fvid2_control(csitxObj->drvHandle,
                               FVID2_REGISTER_TIMESTAMP_FXN,
                               &tsParams,
                               NULL);
    }
    if (FVID2_SOK != retVal)
    {
        UART_printf("CSITX DRV create failed\r\n");
    }

    return (retVal);
}

/**
 * \brief   BoardDiag CSI test function: transmits frames.
 *
 * \param   csitxObj        CSI Tx Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_csitxTest(BoardDiag_CsiTxObj *csitxObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t loopCnt;
    uint64_t tempVar;

    /* Allocate and queue all available frames */
    retVal += BoardDiag_allocAndQFrames(csitxObj);
#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    retVal += BoardDiag_captAllocAndQFrames(csitxObj);

    /* start CSIRX first as it is consumer here */
    if (FVID2_SOK == retVal)
    {
        retVal += Fvid2_start(csitxObj->captObj.drvHandle, NULL);
        if (FVID2_SOK != retVal)
        {
            UART_printf("Capture Start Failed!!!\r\n");
        }
    }
#endif

    /* start CSITX later as it is source/producer here */
    if (FVID2_SOK == retVal)
    {
        retVal += Fvid2_start(csitxObj->drvHandle, NULL);
        if (FVID2_SOK != retVal)
        {
            UART_printf(" Tx Start Failed!!!\r\n");
        }
    }

    UART_printf("Tx streaming has started\n");

    /* Wait for transfer completion */
    while (csitxObj->numFramesTx < csitxObj->numFramesToTx)
    {
        /* Pend on semaphore until last frame is transmitted */
        SemaphoreP_pend(gLockSem, SemaphoreP_WAIT_FOREVER);
        /* Check for Image data here */
    }

#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    /* Begin Rx Capture test*/
    BoardDiag_csirxTest(csitxObj);
#endif

    /* start CSITX first as it is source/producer here */
    retVal += Fvid2_stop(csitxObj->drvHandle, NULL);
    if (FVID2_SOK != retVal)
    {
        UART_printf( " Tx Stop Failed!!!\r\n");
        return retVal;
    }

#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    /* start CSIRX later as it is consumer here */
    retVal += Fvid2_stop(csitxObj->captObj.drvHandle, NULL);
    if (FVID2_SOK != retVal)
    {
        UART_printf("Capture Stop Failed!!!\r\n");
        return retVal;
    }
#endif

    retVal += BoardDiag_txFreeFrames(csitxObj);
#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    retVal += BoardDiag_captFreeFrames(csitxObj);
#endif
    if (FVID2_SOK != retVal)
    {
        UART_printf("Tx/Capture free frames FAILED!!!\r\n");
    }

    /* Print CSITX status */
    Csitx_instStatusInit(&csitxObj->txStatus);

    retVal += Fvid2_control(csitxObj->drvHandle,
                            IOCTL_CSITX_GET_INST_STATUS,
                            &csitxObj->txStatus,
                            NULL);
    if(FVID2_SOK != retVal)
    {
        UART_printf("Get Tx Status Failed!!!\r\n");
    }
    return (retVal);
}

/**
 * \brief   BoardDiag delete function.
 *
 * \param   csitxObj        CSI Tx Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_delete(BoardDiag_CsiTxObj *csitxObj)
{
    int32_t retVal = FVID2_SOK;
    Fvid2_FrameList frmList;

    Fvid2FrameList_init(&frmList);
    /* Dequeue all the request from the driver */
    retVal = Fvid2_dequeue(
                    csitxObj->drvHandle,
                    &frmList,
                    0U,
                    FVID2_TIMEOUT_NONE);

    if ((FVID2_SOK != retVal) && (FVID2_ENO_MORE_BUFFERS != retVal))
    {
        UART_printf("Tx De-queue Failed!!!\r\n");
    }
    else
    {

        retVal = Fvid2_delete(csitxObj->drvHandle, NULL);
        if (FVID2_SOK != retVal)
        {
            UART_printf("FVID2 Delete Failed!!!\r\n");
        }
        else
        {
            csitxObj->drvHandle = NULL;
        }

        if (FVID2_SOK == retVal)
        {
            /* Delete semaphore */
            SemaphoreP_delete(gLockSem);
        }
    }

#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    /* BoardDiag CSI delete function */
    if (FVID2_SOK == retVal)
    {
        retVal = BoardDiag_captDelete(csitxObj);
        if (FVID2_SOK != retVal)
        {
            UART_printf("BoardDiag_captDelete() FAILED!!!\r\n");
        }
    }
#endif

    return (retVal);
}

/**
 * \brief   BoardDiag Init function.
 *
 * \param   csitxObj        CSI Tx Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_deinit(BoardDiag_CsiTxObj *csitxObj)
{
    int32_t retVal = FVID2_SOK;
    Udma_DrvHandle drvHandle = &gUdmaDrvObj;

    /* System de-init */
    retVal = Csitx_deInit();
#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    if (FVID2_SOK == retVal)
    {
        retVal = BoardDiag_captDeinit(csitxObj);
        if (FVID2_SOK != retVal)
        {
            UART_printf("BoardDiag_captDeinit() FAILED!!!\r\n");
        }
    }
#endif
    if(UDMA_SOK != Udma_deinit(drvHandle))
    {
        retVal = FVID2_EFAIL;
        UART_printf("UDMA deinit failed!!!\r\n");
    }
    Fvid2_deInit(NULL);

    return (retVal);
}


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
int32_t BoardDiag_frameCompletionCb(Fvid2_Handle handle,
                                     Ptr appData,
                                     Ptr reserved)
{
    int32_t  retVal = FVID2_SOK;
    uint32_t frmIdx = 0U, idx = 0U;
    Fvid2_FrameList frmList;
    Fvid2_Frame *pFrm;
    BoardDiag_CsiTxObj *csitxObj = (BoardDiag_CsiTxObj *) appData;
    volatile uint32_t tempVar;

    Fvid2FrameList_init(&frmList);
    retVal = Fvid2_dequeue(
        csitxObj->drvHandle,
        &frmList,
        0U,
        FVID2_TIMEOUT_NONE);
    if (FVID2_SOK == retVal)
    {
        csitxObj->numFramesTx += frmList.numFrames;
        for (frmIdx = 0; frmIdx < frmList.numFrames; frmIdx++)
        {
            pFrm = frmList.frames[frmIdx];
            csitxObj->chFrmCnt[pFrm->chNum]++;
            /* Update the frame first word with frame number to be transmitted.
               This is used for testing. */
            if (FVID2_FRAME_STATUS_COMPLETED != pFrm->status)
            {
                idx = (csitxObj->frameErrorCnt % BOARD_DIAG_ERR_FRAME_LOG_MAX);
                csitxObj->errFrmCh[idx] = pFrm->chNum;
                csitxObj->errFrmNo[idx] = csitxObj->chFrmCnt[pFrm->chNum];
                csitxObj->errFrmTs[idx] = (uint32_t)(pFrm->timeStamp64 / 1000U);
                csitxObj->frameErrorCnt++;
            }
        }

        /* Queue back de-queued frames,
           last param i.e. streamId is unused in DRV */
        retVal = Fvid2_queue(csitxObj->drvHandle, &frmList, 0U);
        if (FVID2_SOK != retVal)
        {
            printf( " Tx Queue Failed!!!\r\n");
        }
    }

    /* always return 'FVID2_SOK' */
    /* Post semaphore here for sending next Image */
    SemaphoreP_post(gLockSem);

    return FVID2_SOK;
}

/**
 * \brief   BoardDiag Callback function for frame completion.
 *
 * \param   csitxObj        CSI Tx Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_allocAndQFrames(BoardDiag_CsiTxObj *csitxObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t chIdx = 0U, frmIdx = 0U , loopCnt;
    Fvid2_FrameList frmList;
    uint32_t pixCnt;
    Fvid2_Frame  *pFrm;


    /* for every channel in a transmit handle,
       allocate memory for and queue frames */
    Fvid2FrameList_init(&frmList);
    frmList.numFrames = 0U;
    for (chIdx = 0U; chIdx < csitxObj->createPrms.numCh ; chIdx++)
    {
        for (frmIdx = 0U; frmIdx < BOARD_DIAG_TX_FRAMES_PER_CH ; frmIdx++)
        {
            pixCnt = 0U;
            /* assign frames memory */
            /* Initialize frames to be transmitted */
            for (loopCnt = 0U ;
                 loopCnt < (BOARD_DIAG_TX_FRAME_HEIGHT * BOARD_DIAG_TX_FRAME_PITCH) ;
                 )
            {
                CSL_REG32_WR(&gTxFrms[(chIdx * BOARD_DIAG_TX_FRAMES_PER_CH) + frmIdx][loopCnt],
                                (uint32_t)(((pixCnt + 1U) << 16U) | pixCnt));
                pixCnt = (pixCnt + 2U) % BOARD_DIAG_TX_FRAME_WIDTH;
                loopCnt += 4U;
            }
            uint32_t val = ((chIdx * BOARD_DIAG_TX_FRAMES_PER_CH) + frmIdx);
            gTxFrms[(chIdx * BOARD_DIAG_TX_FRAMES_PER_CH) + frmIdx][3U] = val & 0xFF;
            gTxFrms[(chIdx * BOARD_DIAG_TX_FRAMES_PER_CH) + frmIdx][2U] = (val >> 8) & 0xFF;
            gTxFrms[(chIdx * BOARD_DIAG_TX_FRAMES_PER_CH) + frmIdx][1U] = (val >> 16) & 0xFF;
            gTxFrms[(chIdx * BOARD_DIAG_TX_FRAMES_PER_CH) + frmIdx][0U] = (val >> 24) & 0xFF;

            CacheP_wb(&gTxFrms[(chIdx * BOARD_DIAG_TX_FRAMES_PER_CH) + frmIdx][0], BOARD_DIAG_TX_FRAME_SIZE);
            /* Only following fields are used in CSITX DRV */
            /*scanf("%d",loopCnt);*/
            pFrm = (Fvid2_Frame *)
                    &csitxObj->frames[(chIdx * BOARD_DIAG_TX_FRAMES_PER_CH) + frmIdx];
            pFrm->addr[0U] =
               (uint64_t)&gTxFrms[(chIdx * BOARD_DIAG_TX_FRAMES_PER_CH) + frmIdx][0U];
            pFrm->chNum = csitxObj->createPrms.chCfg[chIdx].chId;
            pFrm->appData = csitxObj;
            frmList.frames[frmList.numFrames] = pFrm;
            frmList.numFrames++;
        }
    }
    /* queue the frames in frmList
     * All allocated frames are queued here as an example.
     * In general at least 2 frames per stream/channel need to queued
     * before transmit can be started.
     * Failing which, frame could be dropped.
     */
    /* last parameter, i.e. streamId is unused in CSITX DRV */
    retVal = Fvid2_queue(csitxObj->drvHandle, &frmList, 0U);
    if (FVID2_SOK != retVal)
    {
        UART_printf("Tx Queue Failed!!!\r\n");
    }

    return retVal;
}

/**
 * \brief   BoardDiag Callback function for frame completion.
 *
 * \param   csitxObj        CSI Tx Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t BoardDiag_txFreeFrames(BoardDiag_CsiTxObj *csitxObj)
{
    int32_t retVal = FVID2_SOK;
    Fvid2_FrameList frmList;

    /* for every stream and channel in a transmit handle */
    Fvid2FrameList_init(&frmList);

    /* Deq-queue any frames queued more than needed */
    retVal = Fvid2_dequeue(
                    csitxObj->drvHandle,
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
