/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2025
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
 *  \file csirx_asf_esm_test_main.c
 *
 *  \brief CSI RX Capture Example.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "csirx_asf_esm_test_main.h"
#include "serdes_sensor_config.h"
#include "csirx_esm.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * \brief   This function is wrapper function used to print message on
 *          respective consoles
 *
 * \param   pcString        Print contents.
 *
 * \retval  none.
 */
static void CsirxApp_consolePrintf(const char *pcString, ...);

/**
 * \brief   This function is used to initialize test parameters
 *
 * \param   appInstObj          Type of print message.
 *                          Refer to struct #appCaptObj
 *
 * \retval  none.
 */
static void CsirxApp_initCaptParams(CsirxApp_CaptInstObj *appInstObj);

/**
 * \brief   App Init function.
 *
 * \param   appInstObj          CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_init(CsirxApp_CaptCommonObj *appCommonObj);

/**
 * \brief   App create function.
 *
 * \param   appCommonObj        CSI RX Capture Test Common App Object to be used by callback
 * \param   appInstObj          CSI RX Capture Test Instance Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_create(CsirxApp_CaptInstObj *appInstObj,  CsirxApp_CaptCommonObj *appCommonObj);

/**
 * \brief   App CSI test function: captures frames.
 *
 * \param   appCommonObj    CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_csiTest(CsirxApp_CaptCommonObj *appCommonObj);

/**
 * \brief   App delete function.
 *
 * \param   appInstObj          CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_delete(CsirxApp_CaptInstObj *appInstObj);

/**
 * \brief   App Init function.
 *
 * \param   appInstObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_deinit(CsirxApp_CaptCommonObj *appCommonObj);

/**
 * \brief   App Callback function for frame completion.
 *
 * \param   handle        Fvid2 DRV handle
 *
 * \param   appData       App based back by to CB function
 *
 * \param   reserved      reserved, not used
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_frameCompletionCb(Fvid2_Handle handle,
                                     Ptr appData,
                                     Ptr reserved);

/**
 * \brief   App Callback function for frame completion.
 *
 * \param   appInstObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_allocAndQFrames(CsirxApp_CaptInstObj *appInstObj);

/**
 * \brief   App Callback function for frame completion.
 *
 * \param   appInstObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_captFreeFrames(CsirxApp_CaptInstObj *appInstObj);

/**
 * \brief   App print function for UDMA driver.
 *
 * \param   str             Print string
 *
 * \retval  None.
 */
static void CsirxApp_dmaPrint(const char *str);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* App common object */
CsirxApp_CaptCommonObj gAppCommonObj;

/* CSIRX instances to use in application */
#if defined(SOC_J784S4) || defined(SOC_J742S2)
uint32_t  gAppCapInst[APP_CAPT_TEST_INST_NUM] = {CSIRX_INSTANCE_ID_0, CSIRX_INSTANCE_ID_1, CSIRX_INSTANCE_ID_2};
#else
uint32_t  gAppCapInst[APP_CAPT_TEST_INST_NUM] = {CSIRX_INSTANCE_ID_0,CSIRX_INSTANCE_ID_1};
#endif

/* Memory buffer to hold data */
static uint8_t gFrmDropBuf[APP_TOTAL_CAPT_INST][(APP_CAPT_FRAME_PITCH)] __attribute__(( aligned(128), section(".data_buffer")));
static uint8_t gFrms[APP_TOTAL_CAPT_INST][(APP_CAPT_FRAMES_PER_CH * APP_CAPT_CH_NUM)][APP_CAPT_FRAME_SIZE]__attribute__(( aligned(128), section(".data_buffer")));

/* Timer handle */
TimerP_Handle gTimerHandle;

/* Semaphore to indicate app completion */
SemaphoreP_Handle gAppCompletionSem;

/* I2c Handle to access deserializer */
extern I2C_Handle gI2cHandle;

/* Fusion2 board detect flag */
bool gFusion2Det = BFALSE;

#if defined(FREERTOS)
/* Capture test Task handle */
extern TaskP_Handle gCsirxAppTask;
#endif


static uint32_t arg;
extern SDL_ESM_config CsirxAsf_Test_esmInitConfig_MAIN;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Application main
 */
int Csirx_captureTest(void)
{
    int retVal = FVID2_SOK;
    bool bDet = BFALSE;
    CsirxApp_CaptInstObj *appInstObj[APP_CAPT_TEST_INST_NUM];
    CsirxApp_CaptCommonObj *appCommonObj;
    appCommonObj = &gAppCommonObj;
    SemaphoreP_Params semParams;
    SDL_ErrType_t result;
    void *ptr = (void *)&arg;

    TimerP_Params timerParams;
    TimerP_Params_init(&timerParams);
    timerParams.runMode    = TimerP_RunMode_ONESHOT;
    timerParams.startMode  = TimerP_StartMode_USER;
    timerParams.periodType = TimerP_PeriodType_MICROSECS;
    timerParams.period     = ((APP_TEST_PERIOD_IN_SEC) * (1000000));
    timerParams.arg        = (void*)appCommonObj;
    /* Creating a timer */
    gTimerHandle = TimerP_create(0x1, (TimerP_Fxn)&CsirxApp_timerIsr, &timerParams);
    if (NULL_PTR == gTimerHandle)
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Timer Create error\r\n");
    }
    /* Creating semaphore to indicate application completion of each Instance */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    gAppCompletionSem = SemaphoreP_create(0U, &semParams);

    /* Initialize ESM */
    result = ESM_init(SDL_ESM_INST_MAIN_ESM0, &CsirxAsf_Test_esmInitConfig_MAIN, (SDL_ESM_applicationCallback) SDL_ESM_applicationCallbackFunction, ptr);
    if (result != SDL_PASS) {
        /* print error and quit */
        GT_1trace(CsirxAppTrace, GT_ERR,
                APP_NAME ": Error initializing MAIN ESM: result = %d\n", result);
    } else {
        GT_0trace(CsirxAppTrace, GT_INFO,
              APP_NAME ": Init MAIN ESM complete\n");
    }

    /* Check for the Fusion2 board */
    bDet = Board_detectBoard(BOARD_ID_FUSION2);
    if(BTRUE == bDet)
    {
        gFusion2Det = BTRUE;
    }

    if(BTRUE == gFusion2Det)
    {
        Board_I2cInitCfg_t i2cCfg;
        i2cCfg.i2cInst    = BOARD_I2C_IOEXP_DEVICE5_INSTANCE;
        i2cCfg.socDomain  = BOARD_SOC_DOMAIN_MAIN;
        i2cCfg.enableIntr = BFALSE;
        Board_setI2cInitConfig(&i2cCfg);

        Board_i2cIoExpInit();

        /* Pulling P0(CSI2_EXP_RSTZ) line high for the normal operation */
        Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE5_ADDR,
                                        ONE_PORT_IOEXP,
                                        PORTNUM_0,
                                        PIN_NUM_0,
                                        PIN_DIRECTION_OUTPUT);

        Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE5_ADDR,
                                    ONE_PORT_IOEXP,
                                    PORTNUM_0,
                                    PIN_NUM_0,
                                    GPIO_SIGNAL_LEVEL_HIGH);
        Board_i2cDeInit();
        App_wait(100);
    }

    for(uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
    {
        appInstObj[i] = &appCommonObj->appInstObj[i];
        memset(appInstObj[i], 0x0, sizeof (CsirxApp_CaptInstObj));
    }
    /* Initialize application object for current capture context */
    for(uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
    {
        appInstObj[i]->instId = gAppCapInst[i];
        CsirxApp_initCaptParams(appInstObj[i]);
    }

    /* App Init */
    retVal += CsirxApp_init(appCommonObj);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
              APP_NAME ": [ERROR]CsirxApp_init() FAILED!!!\r\n");
    }

    GT_0trace(CsirxAppTrace, GT_INFO,
              APP_NAME ": Sample Application - STARTS !!!\r\n");
    GT_0trace(CsirxAppTrace, GT_INFO,
              APP_NAME ": ===================Setup Details===================\r\n");


    if ((1U == APP_CAPT_TEST_INST_NUM) && (CSIRX_INSTANCE_ID_0 == appInstObj[0]->instId))
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Capture Instance: 0\r\n");
    }
    else if ((1U == APP_CAPT_TEST_INST_NUM) && (CSIRX_INSTANCE_ID_1 == appInstObj[0]->instId))
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Capture Instance: 1\r\n");
    }
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    else if ((1U == APP_CAPT_TEST_INST_NUM) && (CSIRX_INSTANCE_ID_2 == appInstObj[0]->instId))
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Capture Instance: 2\r\n");
    }
#endif
    else if ((2U == APP_CAPT_TEST_INST_NUM) && (CSIRX_INSTANCE_ID_0 == appInstObj[0]->instId) && (CSIRX_INSTANCE_ID_1 == appInstObj[1]->instId))
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": Both Capture Instance 0 and Capture Instance 1\r\n");
    }
    GT_1trace(CsirxAppTrace, GT_INFO,
              APP_NAME ": Capture DF:0x%x\r\n", APP_CAPT_IMAGE_DT);
    GT_2trace(CsirxAppTrace, GT_INFO,
              APP_NAME ": Capture Resolution:%d x %d\r\n",
              APP_CAPT_FRAME_WIDTH,
              APP_CAPT_FRAME_HEIGHT);
    GT_0trace(CsirxAppTrace, GT_INFO,
              APP_NAME ": ===================================================\r\n");

    /* App Create */
    if (FVID2_SOK == retVal)
    {
        for(uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
        {
            retVal += CsirxApp_create(appInstObj[i], appCommonObj);
            if (FVID2_SOK != retVal)
            {
                GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": [ERROR]CsirxApp_create() FAILED!!!\r\n");
            }
        }
    }
    
    /*  APP start */
    retVal = CsirxApp_csiTest(appCommonObj);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
              APP_NAME ": [ERROR]CsirxApp_csiTest() FAILED!!!\r\n");
    }

    /**
     * Trigger ASF event using test register if could not be validated 
     * via a realtime sequence.
     * 
     * Since the interrupts are from ESM, the CSIRX ASF ISR handling will
     * check for all the CSIRX instances and handle it, irrespective of
     * which instance triggered it. 
     */
    if (FVID2_SOK == retVal)
    {
        for(uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
        {
            if(appCommonObj->appInstObj[i].asfTransToEvent == 0U)
            {
                GT_0trace(CsirxAppTrace, GT_INFO,
                            APP_NAME ": [Transaction timeout systematic validation not achieved!]Test Trigger Transaction Timeout ASF event");
            }

            if(appCommonObj->appInstObj[i].asfDapErrEvent == 0U)
            {
                GT_0trace(CsirxAppTrace, GT_INFO,
                            APP_NAME ": [DAP parity error systematic validation not achieved!]Test Trigger DAP Parity ASF error event");
            }

            if((appCommonObj->appInstObj[i].asfTransToEvent == 0U) || (appCommonObj->appInstObj[i].asfDapErrEvent == 0U))
            {
                retVal = Fvid2_control(appCommonObj->appInstObj[i].drvHandle,
                        IOCTL_CSIRX_TRIG_ASF_EVENT,
                        NULL,
                        NULL);
            }
        }
    }

    /* App CSI delete function */
    if (FVID2_SOK == retVal)
    {
        for(uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
        {
            retVal += CsirxApp_delete(appInstObj[i]);
            if (FVID2_SOK != retVal)
            {
                GT_0trace(CsirxAppTrace, GT_ERR,
                          APP_NAME ": [ERROR]CsirxApp_delete() FAILED!!!\r\n");
            }
        }
    }

    /* App CSI De-initialization function */
    if (FVID2_SOK == retVal)
    {
        retVal += CsirxApp_deinit(appCommonObj);
        if (FVID2_SOK != retVal)
        {
            GT_0trace(CsirxAppTrace, GT_ERR,
                      APP_NAME ": [ERROR]CsirxApp_deinit() FAILED!!!\r\n");
        }
    }

    /* using 'CsirxApp_consolePrintf' instead of GT trace as Fvid2_deInit has happend */
    CsirxApp_consolePrintf("Sample Application - DONE !!!\r\n");

    return (retVal);
}

void CsirxApp_timerIsr(void *arg)
{
    CsirxApp_CaptCommonObj *appCommonObj=(CsirxApp_CaptCommonObj*)arg;
    int32_t retVal = FVID2_SOK;
    /* Stop the streams immediately after the timeout is reached */
    for(uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
    {
        retVal += Fvid2_stop(appCommonObj->appInstObj[i].drvHandle, NULL);
        if (FVID2_SOK != retVal)
        {
            GT_1trace(CsirxAppTrace, GT_ERR,
                      APP_NAME ": Capture Stop Failed for instance %d!!!\r\n", appCommonObj->appInstObj[i].instId);
        }

    }
    /* Post semaphore to print the results */
    SemaphoreP_post(gAppCompletionSem);
}

static void CsirxApp_initCaptParams(CsirxApp_CaptInstObj* appInstObj)
{
    uint32_t loopCnt = 0U;

    if (CSIRX_INSTANCE_ID_0 == appInstObj->instId)
    {
        appInstObj->boardCsiInstID = BOARD_CSI_INST_0;
        appInstObj->cameraSensor = APP_CSIRX_INST0_CAMERA_SENSOR;
    }
    else if (CSIRX_INSTANCE_ID_1 == appInstObj->instId)
    {
        appInstObj->boardCsiInstID = BOARD_CSI_INST_1;
        appInstObj->cameraSensor = APP_CSIRX_INST1_CAMERA_SENSOR;
    }
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    else if (CSIRX_INSTANCE_ID_2 == appInstObj->instId)
    {
        appInstObj->boardCsiInstID = BOARD_CSI_INST_2;
        appInstObj->cameraSensor = APP_CSIRX_INST2_CAMERA_SENSOR;
    }
#endif
    else
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": Invalid Capture Instance\r\n");
    }

    /* set instance configuration parameters */
    Csirx_createParamsInit(&appInstObj->createPrms);
    appInstObj->createPrms.numCh = APP_CAPT_CH_NUM;
    /* set channel configuration parameters */
    for (loopCnt = 0U ; loopCnt < appInstObj->createPrms.numCh ; loopCnt++)
    {
        appInstObj->chFrmCnt[loopCnt] = 0U;
        appInstObj->createPrms.chCfg[loopCnt].chId = loopCnt;
        appInstObj->createPrms.chCfg[loopCnt].chType = CSIRX_CH_TYPE_CAPT;
        appInstObj->createPrms.chCfg[loopCnt].vcNum = loopCnt;
        appInstObj->createPrms.chCfg[loopCnt].inCsiDataType = APP_CAPT_IMAGE_DT;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.width = APP_CAPT_FRAME_WIDTH;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.height = APP_CAPT_FRAME_HEIGHT;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.pitch[0U] =
                                                APP_CAPT_FRAME_PITCH;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.dataFormat =
                                                FVID2_DF_BGRX32_8888;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.ccsFormat =
                                                APP_CAPT_IMAGE_STORAGE_FORMAT;
    }
    /* set module configuration parameters */
    appInstObj->createPrms.instCfg.enableCsiv2p0Support = UTRUE;
    appInstObj->createPrms.instCfg.numDataLanes = 4U;
    appInstObj->createPrms.instCfg.enableErrbypass = UFALSE;
    appInstObj->createPrms.instCfg.enableStrm[CSIRX_CAPT_STREAM_ID] = 1U;
    for (loopCnt = 0U ;
         loopCnt < appInstObj->createPrms.instCfg.numDataLanes ;
         loopCnt++)
    {
        appInstObj->createPrms.instCfg.dataLanesMap[loopCnt] = (loopCnt + 1U);
    }
    /* set monitor configuration parameters */
    appInstObj->createPrms.instCfg.transToMonCfg.transToEnable = APP_TRANS_TIMEOUT_MON_EN;
    appInstObj->createPrms.instCfg.transToMonCfg.transToCtrl = APP_TRANS_TIMEOUT_MON_PCLKS;

    /* set transaction timeout event to be non-fatal */
    appInstObj->createPrms.instCfg.asfFatalNonFatalCfg.asfTransToErr = APP_TRANS_TIMEOUT_MON_FNF_SELECT;

    /* ASF test intr config */
    appInstObj->createPrms.instCfg.trigAsfTestIntr.asfTransToErrTest = APP_TRIGGER_ASF_TRANS_TO_ERR;    
    appInstObj->createPrms.instCfg.trigAsfTestIntr.asfDapErrTest = APP_TRIGGER_ASF_DAP_ERR;    
    /* set frame drop buffer parameters */
    appInstObj->createPrms.frameDropBufLen =
                                (APP_CAPT_FRAME_WIDTH * APP_CAPT_FRAME_BPP);
    appInstObj->createPrms.frameDropBuf = (uint64_t)&gFrmDropBuf;
    /* This will be updated once Fvid2_create() is done */
    appInstObj->createStatus.retVal = FVID2_SOK;
    appInstObj->drvHandle = NULL;
    Fvid2CbParams_init(&appInstObj->cbPrms);
    appInstObj->cbPrms.cbFxn   = (Fvid2_CbFxn) &CsirxApp_frameCompletionCb;
    appInstObj->cbPrms.appData = appInstObj;

    appInstObj->numFramesRcvd = 0U;
    appInstObj->frameErrorCnt = 0U;
    appInstObj->maxWidth = APP_CAPT_FRAME_WIDTH;
    appInstObj->maxHeight = APP_CAPT_FRAME_HEIGHT;
    appInstObj->asfTransToEvent = 0U;

    /* Initialize capture instance status */
    Csirx_instStatusInit(&appInstObj->captStatus);
}


static int32_t CsirxApp_init(CsirxApp_CaptCommonObj* appCommonObj)
{
    int32_t retVal = FVID2_SOK, dmaStatus = UDMA_SOK;
    uint32_t instId, loopCnt;
    Fvid2_InitPrms initPrms;
    Udma_InitPrms   udmaInitPrms;
    Udma_DrvHandle drvHandle;
    I2C_HwAttrs i2cConfig;
    
    /* set instance initialization parameters */
    /* This will be updated once UDMA init is done */
    Csirx_initParamsInit(&appCommonObj->initPrms);
    drvHandle = &appCommonObj->udmaDrvObj;
    appCommonObj->initPrms.drvHandle = drvHandle;
    /* Fvid2 init */
    Fvid2InitPrms_init(&initPrms);
    initPrms.printFxn = &CsirxApp_fvidPrint;
    retVal = Fvid2_init(&initPrms);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": Fvid2 Init Failed!!!\r\n");
    }

    /* Do UDMA init before CSIRX Init */
    /* UDMA driver init */
#if defined (SOC_J721E)
    instId = UDMA_INST_ID_MAIN_0;
#endif
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
    instId = UDMA_INST_ID_BCDMA_0;
#endif
    UdmaInitPrms_init(instId, &udmaInitPrms);
    udmaInitPrms.printFxn = &CsirxApp_dmaPrint;
    dmaStatus = Udma_init(drvHandle, &udmaInitPrms);
    if(UDMA_SOK != dmaStatus)
    {
        retVal = FVID2_EFAIL;
        GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": UDMA Init Failed!!!\r\n");
    }
    retVal = Csirx_init(&appCommonObj->initPrms);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": System Init Failed!!!\r\n");
    }

    if (FVID2_SOK == retVal)
    {
        /* Initialize I2C Driver */
        for(loopCnt = 0U; loopCnt < I2C_HWIP_MAX_CNT; loopCnt++)
        {
            I2C_socGetInitCfg(loopCnt, &i2cConfig);
            i2cConfig.enableIntr = BFALSE;
            I2C_socSetInitCfg(loopCnt, &i2cConfig);
        }

        /* Initializes the I2C */
        I2C_init();
    }
    return (retVal);
}

static int32_t CsirxApp_create(CsirxApp_CaptInstObj* appInstObj, CsirxApp_CaptCommonObj *appCommonObj)
{
    int32_t retVal = FVID2_SOK;
    Fvid2_TimeStampParams tsParams;
    Csirx_DPhyCfg dphyCfg;
    Csirx_EventPrms eventPrms;

    /* Fvid2_create() */
    appInstObj->drvHandle = Fvid2_create(
        CSIRX_CAPT_DRV_ID,
        appInstObj->instId,
        &appInstObj->createPrms,
        &appInstObj->createStatus,
        &appInstObj->cbPrms);

    if ((NULL == appInstObj->drvHandle) ||
        (FVID2_SOK != appInstObj->createStatus.retVal))
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": Capture Create Failed!!!\r\n");
        retVal = appInstObj->createStatus.retVal;
    }
    if (FVID2_SOK == retVal)
    {
        /* Set CSIRX D-PHY configuration parameters */
        Csirx_initDPhyCfg(&dphyCfg);
        dphyCfg.inst = appInstObj->instId;
        retVal = Fvid2_control(appInstObj->drvHandle,
                                IOCTL_CSIRX_SET_DPHY_CONFIG,
                                &dphyCfg,
                                NULL);
        if(FVID2_SOK != retVal)
        {
            GT_1trace(CsirxAppTrace,
                      GT_ERR,
                      APP_NAME
                      ": Set D-PHY Configuration FAILED for CSIRX instance %d!!!\r\n",appInstObj->instId);
        }
        else
        {
            GT_1trace(CsirxAppTrace,
                      GT_INFO,
                      APP_NAME
                      ": Set D-PHY Configuration Successful for CSIRX instance %d!!!\r\n",appInstObj->instId);
        }
    }
    if (FVID2_SOK == retVal)
    {
        /* Register Error Events */
        Csirx_eventPrmsInit(&eventPrms);
        retVal = Fvid2_control(appInstObj->drvHandle,
                               IOCTL_CSIRX_REGISTER_EVENT,
                               &eventPrms,
                               NULL);
#if CSIRX_ASF_TRANS_TO_TEST
        eventPrms.eventGroup = CSIRX_ESM_HI_EVENT_GROUP_ASF;
        eventPrms.eventMasks = CSIRX_EVENT_TYPE_ASF_ALL;
        eventPrms.eventCb = CsirxApp_asfEventCallback;
        eventPrms.appData = appCommonObj;     

        retVal = Fvid2_control(appInstObj->drvHandle,
                               IOCTL_CSIRX_REGISTER_EVENT,
                               &eventPrms,
                               NULL);
#endif
        if(FVID2_SOK != retVal)
        {
            GT_1trace(CsirxAppTrace,
                      GT_ERR,
                      APP_NAME
                      ": Error Events Registration FAILED for CSIRX instance %d!!!\r\n",appInstObj->instId);
        }
        else
        {
            GT_1trace(CsirxAppTrace,
                      GT_INFO,
                      APP_NAME
                      ": Error Events Registration Successful for CSIRX instance %d!!!\r\n",appInstObj->instId);
        }
    }
    if (FVID2_SOK == retVal)
    {
        retVal = CsirxApp_setupI2CInst(appInstObj);
    }

    if (FVID2_SOK == retVal)
    {
        tsParams.timeStampFxn = (Fvid2_TimeStampFxn)&TimerP_getTimeInUsecs;
        /* register time stamping function */
        retVal = Fvid2_control(appInstObj->drvHandle,
                               FVID2_REGISTER_TIMESTAMP_FXN,
                               &tsParams,
                               NULL);
    }
    if (FVID2_SOK == retVal)
    {
        GT_1trace(CsirxAppTrace,
                  GT_INFO,
                  APP_NAME ": CSIRX Capture created for CSIRX instance %d\r\n",appInstObj->instId);
    }
    /* Allocate and queue all available frames */
    retVal += CsirxApp_allocAndQFrames(appInstObj);
    /* Configure sensor here */
    retVal += CsirxApp_sensorConfig(appInstObj);
    return (retVal);
}

static int32_t CsirxApp_csiTest(CsirxApp_CaptCommonObj* appCommonObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t elapsedTime, fps;
    uint64_t tempVar;

#if defined(FREERTOS)
    LoadP_reset();
    GT_0trace(CsirxAppTrace,
              GT_INFO,
              APP_NAME
              ": Before stream start\r\n");
    CsirxApp_printLoad();
#endif
    for(uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
    {
        if (FVID2_SOK == retVal)
        {
            retVal += Fvid2_start(appCommonObj->appInstObj[i].drvHandle, NULL);
            if (FVID2_SOK != retVal)
            {
                GT_1trace(CsirxAppTrace, GT_ERR,
                         APP_NAME ": Capture Start Failed for instance %d!!!\r\n",appCommonObj->appInstObj[i].instId);
            }
        }
    }
    TimerP_start(gTimerHandle);
    SemaphoreP_pend(gAppCompletionSem, SemaphoreP_WAIT_FOREVER);

#if defined(FREERTOS)
    GT_0trace(CsirxAppTrace,
              GT_INFO,
              APP_NAME
              ": After stream end\r\n");
    CsirxApp_printLoad();
#endif
    /* fps calculation and some x100 for precision */
    for(uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
    {
        retVal += CsirxApp_captFreeFrames(&appCommonObj->appInstObj[i]);
        if (FVID2_SOK != retVal)
        {
            GT_1trace(CsirxAppTrace, GT_ERR,
                      APP_NAME ": Capture Stop Failed for instance %d!!!\r\n", appCommonObj->appInstObj[i].instId);
        }
        elapsedTime = (APP_TEST_PERIOD_IN_SEC * 1000);
        tempVar = ((uint64_t)(appCommonObj->appInstObj[i].numFramesRcvd * 100000U)) / elapsedTime;
        fps = (uint32_t)tempVar;
        Csirx_instStatusInit(&appCommonObj->appInstObj[i].captStatus);
#if (1U == APP_PRINT_DRV_LOGS)
        /* print debug logs if enabled */
        retVal += Fvid2_control(appCommonObj->appInstObj[i].drvHandle,
                                IOCTL_CSIRX_PRINT_DEBUG_LOGS,
                                NULL,
                                NULL);
#endif
 
        retVal += Fvid2_control(appCommonObj->appInstObj[i].drvHandle,
                                IOCTL_CSIRX_GET_INST_STATUS,
                                &appCommonObj->appInstObj[i].captStatus,
                                NULL);
        if(FVID2_SOK != retVal)
        {
            GT_0trace(CsirxAppTrace,
                      GT_INFO,
                      APP_NAME
                      ":Get Capture Status Failed!!!\r\n");
        }
        GT_0trace(CsirxAppTrace, GT_INFO,
            "\n\r==========================================================\r\n");
        GT_0trace(CsirxAppTrace, GT_INFO,
                  APP_NAME ": Capture Status:\r\n");
        GT_1trace(CsirxAppTrace, GT_INFO,
                  APP_NAME ": Capture instance:%d\r\n",appCommonObj->appInstObj[i].instId);
        GT_0trace(CsirxAppTrace, GT_INFO,
                  "==========================================================\r\n");
        GT_1trace(CsirxAppTrace, GT_INFO,
                  APP_NAME ": Frames Received: %d\r\n",
                  appCommonObj->appInstObj[i].numFramesRcvd);
        GT_1trace(CsirxAppTrace, GT_INFO,
                  APP_NAME ": Frames Received with errors: %d\r\n",
                  appCommonObj->appInstObj[i].frameErrorCnt);
#if (1U == APP_PRINT_DRV_LOGS)
        uint32_t loopCnt;
        if (0U < appInstObj->frameErrorCnt)
        {
            GT_0trace(CsirxAppTrace, GT_INFO,
                  APP_NAME ": Error Frames Info...\r\n");
            tempVar = appCommonObj->appInstObj[i].frameErrorCnt;
            if (appCommonObj->appInstObj[i].frameErrorCnt > APP_ERR_FRAME_LOG_MAX)
            {
                tempVar = APP_ERR_FRAME_LOG_MAX;
            }
            for (loopCnt = 0U ; loopCnt < tempVar ; loopCnt++)
            {
                GT_4trace(CsirxAppTrace, GT_INFO,
                  APP_NAME ": [Frame No.: %d] | Channel Id: %d |"
                  " Ch Error Frame Number: %d | Time-stamp(ms): %d \r\n",
                  loopCnt,
                  appCommonObj->appInstObj[i].errFrmCh[loopCnt],
                  appCommonObj->appInstObj[i].errFrmNo[loopCnt],
                  appCommonObj->appInstObj[i].errFrmTs[loopCnt]);
            }
 
        }
#endif
        GT_4trace(CsirxAppTrace, GT_INFO,
                  APP_NAME ": %d frames captured in %d msec"
                  " at the rate of %d.%2d frames/sec.\r\n",
                  appCommonObj->appInstObj[i].numFramesRcvd,
                  elapsedTime,
                  (fps / 100U),
                  (fps % 100U));
    }
    return retVal;
}

static int32_t CsirxApp_delete(CsirxApp_CaptInstObj* appInstObj)
{
    int32_t retVal = FVID2_SOK;
    static Fvid2_FrameList frmList;

    Fvid2FrameList_init(&frmList);
    /* Dequeue all the request from the driver */
    retVal = Fvid2_dequeue(
                    appInstObj->drvHandle,
                    &frmList,
                    0U,
                    FVID2_TIMEOUT_NONE);

    if ((FVID2_SOK != retVal) && (FVID2_ENO_MORE_BUFFERS != retVal))
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": Capture De-queue Failed!!!\r\n");
    }
    if ((FVID2_SOK == retVal) || (FVID2_ENO_MORE_BUFFERS == retVal))
    {
        retVal = FVID2_SOK;
        /* Disable Error Events */
        retVal = Fvid2_control(appInstObj->drvHandle,
                               IOCTL_CSIRX_UNREGISTER_EVENT,
                               (void *)CSIRX_EVENT_GROUP_ERROR,
                               NULL);
        if(FVID2_SOK != retVal)
        {
            GT_0trace(CsirxAppTrace,
                      GT_ERR,
                      APP_NAME
                      ":Error Events un-registration FAILED!!!\r\n");
        }
    }
    if (FVID2_SOK == retVal)
    {
        retVal = Fvid2_delete(appInstObj->drvHandle, NULL);
    }
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": FVID2 Delete Failed!!!\r\n");
    }
    else
    {
        appInstObj->drvHandle = NULL;
    }

    if (FVID2_SOK == retVal)
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Capture Driver deleted\r\n");
    }

    return (retVal);
}

static int32_t CsirxApp_deinit(CsirxApp_CaptCommonObj *appCommonObj)
{
    int32_t retVal = FVID2_SOK;
    Udma_DrvHandle drvHandle = &appCommonObj->udmaDrvObj;

    /* TODO: sensor config de-init if needed */
    retVal = Csirx_deInit();
    /* System de-init */
    if(UDMA_SOK != Udma_deinit(drvHandle))
    {
        retVal = FVID2_EFAIL;
        GT_0trace(CsirxAppTrace,
                  GT_ERR,
                  APP_NAME ": UDMA deinit failed!!!\r\n");
    }
    Fvid2_deInit(NULL);
    /* Close I2C channel */
    I2C_close(gI2cHandle);
    /* Delete semaphore */
    SemaphoreP_delete(gAppCompletionSem);
    /* Delete Timer */
    TimerP_delete(gTimerHandle);
    return (retVal);
}

static void CsirxApp_consolePrintf(const char *pcString, ...)
{
    char printBuffer[APP_PRINT_BUFFER_SIZE];
    va_list arguments;

    /* Start the var args processing. */
    va_start(arguments, pcString);
    vsnprintf (printBuffer, sizeof(printBuffer), pcString, arguments);
    printf("%s",printBuffer);
#if !defined (QT_BUILD)
    UART_printf(printBuffer);
#endif
    /* End the var args processing. */
    va_end(arguments);
}

void CsirxApp_fvidPrint(const char *str, ...)
{
    CsirxApp_consolePrintf(str);

    return;
}

static void CsirxApp_dmaPrint(const char *str)
{
    CsirxApp_consolePrintf(str);

    return;
}

static int32_t CsirxApp_frameCompletionCb(Fvid2_Handle handle,
                                     Ptr appData,
                                     Ptr reserved)
{
    int32_t  retVal = FVID2_SOK;
    uint32_t frmIdx = 0U, idx = 0U;
    static Fvid2_FrameList frmList;
    Fvid2_Frame *pFrm;
    CsirxApp_CaptInstObj *appInstObj = (CsirxApp_CaptInstObj *) appData;

    GT_assert(CsirxAppTrace, (NULL != appData));

    Fvid2FrameList_init(&frmList);
    retVal = Fvid2_dequeue(
        appInstObj->drvHandle,
        &frmList,
        0U,
        FVID2_TIMEOUT_NONE);
    if (FVID2_SOK == retVal)
    {
        appInstObj->numFramesRcvd += frmList.numFrames;
        for (frmIdx = 0U; frmIdx < frmList.numFrames; frmIdx++)
        {
            pFrm = frmList.frames[frmIdx];
            appInstObj->chFrmCnt[pFrm->chNum]++;
            if (FVID2_FRAME_STATUS_COMPLETED != pFrm->status)
            {
                idx = (appInstObj->frameErrorCnt % APP_ERR_FRAME_LOG_MAX);
                appInstObj->errFrmCh[idx] = pFrm->chNum;
                appInstObj->errFrmNo[idx] = appInstObj->chFrmCnt[pFrm->chNum];
                appInstObj->errFrmTs[idx] = (uint32_t)(pFrm->timeStamp64 / 1000U);
                appInstObj->frameErrorCnt++;
            }
        }

        /* Queue back de-queued frames,
           last param i.e. streamId is unused in DRV */
        retVal = Fvid2_queue(appInstObj->drvHandle, &frmList, 0U);
        if (FVID2_SOK != retVal)
        {
            GT_0trace(CsirxAppTrace, GT_ERR,
                      APP_NAME ": Capture Queue Failed!!!\r\n");
        }
    }

    /* always return 'FVID2_SOK' */

    return FVID2_SOK;
}

static int32_t CsirxApp_allocAndQFrames(CsirxApp_CaptInstObj *appInstObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t chIdx = 0U, frmIdx = 0U;
    static Fvid2_FrameList frmList;
    Fvid2_Frame  *pFrm;

    /* for every channel in a capture handle,
       allocate memory for and queue frames */
    Fvid2FrameList_init(&frmList);
    frmList.numFrames = 0U;
    for (chIdx = 0U; chIdx < appInstObj->createPrms.numCh ; chIdx++)
    {
        for (frmIdx = 0U; frmIdx < APP_CAPT_FRAMES_PER_CH ; frmIdx++)
        {
            /* assign frames memory */
            /* Only following fields are used in CSIRX DRV */
            pFrm = (Fvid2_Frame *)
                    &appInstObj->frames[(chIdx * APP_CAPT_FRAMES_PER_CH) + frmIdx];
            pFrm->addr[0U] =
               (uint64_t)&gFrms[appInstObj->instId][(chIdx * APP_CAPT_FRAMES_PER_CH) + frmIdx][0U];
            pFrm->chNum = appInstObj->createPrms.chCfg[chIdx].chId;
            pFrm->appData = appInstObj;
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
    retVal = Fvid2_queue(appInstObj->drvHandle, &frmList, 0U);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR,
                  APP_NAME ": Capture Queue Failed!!!\r\n");
    }

    return retVal;
}

static int32_t CsirxApp_captFreeFrames(CsirxApp_CaptInstObj *appInstObj)
{
    int32_t retVal = FVID2_SOK;
    static Fvid2_FrameList frmList;

    /* for every stream and channel in a capture handle */
    Fvid2FrameList_init(&frmList);

    /* Deq-queue any frames queued more than needed */
    retVal = Fvid2_dequeue(
                    appInstObj->drvHandle,
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

uint32_t CsirxApp_getCurTimeInMsec(void)
{
    uint64_t curTimeMsec, curTimeUsec;

    curTimeUsec = TimerP_getTimeInUsecs();
    curTimeMsec = (curTimeUsec / 1000U);

    return ((uint32_t) curTimeMsec);
}

uint32_t CsirxApp_getElapsedTimeInMsec(uint32_t startTime)
{
    uint32_t     elapsedTimeMsec = 0U, currTime;

    currTime = CsirxApp_getCurTimeInMsec();
    if (currTime < startTime)
    {
        /* Counter overflow occured */
        elapsedTimeMsec = (0xFFFFFFFFU - startTime) + currTime + 1U;
    }
    else
    {
        elapsedTimeMsec = currTime - startTime;
    }

    return (elapsedTimeMsec);
}

void SDL_ESM_applicationCallbackFunction(SDL_ESM_Inst esmInst,
                                            SDL_ESM_IntType esmIntrType,
                                            uint32_t grpChannel,
                                            uint32_t index,
                                            uint32_t intSrc,
                                            void *arg)
{
    GT_5trace(CsirxAppTrace, GT_INFO,
            APP_NAME ": [ESM App Callback]ASF Interrupt ESM Source Details - esmInst: %d esmIntrType: %d \
            grpChannel: %d index: %d intSrc: %d\r\n", esmInst, esmIntrType, grpChannel, index, intSrc);
    
    /* Nothing to do */
}

void CsirxApp_asfEventCallback(Csirx_EventStatus status,void *appData)
{
    uint32_t instIdx;
    CsirxApp_CaptInstObj *appInstObj;
    CsirxApp_CaptCommonObj *appCommonObj = (CsirxApp_CaptCommonObj *)appData;

    for(instIdx = 0U; instIdx < APP_CAPT_TEST_INST_NUM; instIdx++)
    {
        appInstObj = (CsirxApp_CaptInstObj *)&appCommonObj->appInstObj[instIdx];
        if(status.eventMasks & CSIRX_EVENT_TYPE_ASF_TRANS_TO_ERR) 
        {
            appInstObj->asfTransToEvent++;
            if(appInstObj->asfTransToEvent) {
                GT_1trace(CsirxAppTrace, GT_INFO,
                    APP_NAME ": [CSIRX ASF App Callback]Transaction Timeout Error Occurred for CSIRX Instance %d\r\n", instIdx);
            }
        }

        if(status.eventMasks & CSIRX_EVENT_TYPE_ASF_DAP_ERR)
        {
            appInstObj->asfDapErrEvent++;
            if(appInstObj->asfDapErrEvent) {
                GT_1trace(CsirxAppTrace, GT_INFO,
                    APP_NAME ": [CSIRX ASF App Callback]DAP Parity Error Occurred for CSIRX Instance %d\r\n", instIdx);
            }
        }
    }

    /**
     * ESM handler called via application callback for processing and clearing esm interrupt.
     * This needs to be changed once ESM Main Domain driver enablement is natively done as part of PDK. 
     */
    CSIRX_ESM_appCallbackIntrHandler(CSIRX_ASF_ESM_INTR_PRIORITY_LEVEL_HIGH, arg);
}

#if defined(FREERTOS)
void CsirxApp_printLoad(void)
{
    LoadP_Stats loadStatsTask;
    uint32_t cpuLoad;

    /* Query CPU Load */
    cpuLoad = LoadP_getCPULoad();
    GT_1trace(CsirxAppTrace, GT_INFO,
              APP_NAME ": CPU Load is:%d percent\r\n", cpuLoad);
    /* Get task loads */
    LoadP_getTaskLoad(gCsirxAppTask, &loadStatsTask);
    if(0U < loadStatsTask.percentLoad)
    {
        GT_1trace(CsirxAppTrace, GT_INFO,
                  APP_NAME ": Task Load is: %d percent\r\n", loadStatsTask.percentLoad);
    }
    else
    {
        GT_0trace(CsirxAppTrace, GT_INFO,
                  APP_NAME ": Task Load is: < 1 percent \n");
    }
}
#endif
