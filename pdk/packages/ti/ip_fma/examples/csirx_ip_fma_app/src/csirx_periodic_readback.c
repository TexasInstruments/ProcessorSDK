/*
 *  Copyright (c) Texas Instruments Incorporated 2026
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
 *  \file csirx_periodic_readback.c
 *
 *  \brief CSIRX Periodic Software Readback Example initializes the CSIRX
 *         module and performs periodic readback of the static registers.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/osal/LoadP.h>
#include <ti/osal/osal.h>
#include <ti/drv/csirx/csirx.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/board/src/devices/common/common.h>
#include <ti/board/board.h>
#include <ti/board/src/devices/board_devices.h>
#include <ti/board/src/devices/fpd/ds90ub960.h>
#if defined(SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_cfg.h>
#include <ti/board/src/j721s2_evm/include/board_utils.h>
#include <ti/board/src/j721s2_evm/include/board_i2c_io_exp.h>
#include <ti/csl/soc/j721s2/src/cslr_soc_baseaddress.h>
#elif defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_cfg.h>
#include <ti/board/src/j784s4_evm/include/board_utils.h>
#include <ti/board/src/j784s4_evm/include/board_i2c_io_exp.h>
#include <ti/csl/soc/j784s4/src/cslr_soc_baseaddress.h>
#endif
#include <ip_fma_csirx.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**< Application name */
#define APP_NAME                                  "CSIRX_PERIODIC_READBACK_APP"
/**< Number of CSIRX instances exist in SOC */
#define APP_TOTAL_CAPT_INST                       (CSIRX_INSTANCE_ID_MAX)
/**< Number of CSIRX instances to test parallelly */
#if defined (SOC_J784S4)
#define APP_CAPT_TEST_INST_NUM                    ((uint32_t)3U)
#else
#define APP_CAPT_TEST_INST_NUM                    ((uint32_t)2U)
#endif
/**< Number of channels */
#define APP_CAPT_CH_NUM                           ((uint32_t)4U)
/**< Input Image Data format */
#define APP_CAPT_IMAGE_DT                         (FVID2_CSI2_DF_RAW12)
/**< Frame storage format. Only valid for RAW12 DT. */
#define APP_CAPT_IMAGE_STORAGE_FORMAT             (FVID2_CCSF_BITS12_UNPACKED16)
/**< Number of frames per stream */
#define APP_CAPT_FRAMES_PER_CH                    ((uint32_t)4U)
/**< Frame Attribute: Width in pixels */
#define APP_CAPT_FRAME_WIDTH                      ((uint32_t)1936U)
/**< Frame Attribute: Height in pixels */
#define APP_CAPT_FRAME_HEIGHT                     ((uint32_t)1100U)
/**< Frame Attribute: Bytes per pixel */
#define APP_CAPT_FRAME_BPP                        ((uint32_t)2U)
/**< Frame Attribute: Pitch in bytes */
#define APP_CAPT_FRAME_PITCH                      ((uint32_t) (APP_CAPT_FRAME_WIDTH * APP_CAPT_FRAME_BPP))
/* Print buffer character limit for prints- UART or CCS Console */
#define APP_PRINT_BUFFER_SIZE                     ((uint32_t)4000)
/** \brief Log enable for CSIRX Sample application */
#define CsirxAppTrace                             ((uint32_t) GT_INFO | (uint32_t) GT_TraceState_Enable)
/**< Maximum number of error frame logs to store.
     It stores most recent errors.*/
#define APP_ERR_FRAME_LOG_MAX                     ((uint32_t)500U)
/**< Number of periodic readbacks */
#define PERIODIC_CHECK_NUM                        ((int8_t)10)
/**< Delay between checks */
#define PERIODIC_CHECK_DELAY                      ((uint32_t)1000U)
/**< Delay necessary after board initialization*/
#define INIT_DELAY                                ((uint32_t)100U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief Capture application object.
 */
typedef struct
{
    uint32_t instId;
   /**< Csirx Drv Instance ID. */
    uint32_t boardCsiInstID;
   /**< Csirx Drv Instance ID. */
    uint32_t cameraSensor;
   /**< Camera sensor type */
    Csirx_CreateParams createPrms;
   /**< Csirx create time parameters */
    Csirx_CreateStatus createStatus;
   /**< Csirx create time status */
    Fvid2_Handle drvHandle;
   /**< FVID2 capture driver handle. */
    Fvid2_CbParams cbPrms;
   /**< Callback params. */
    volatile uint32_t numFramesRcvd;
   /**< Number of frames received */
    uint32_t frameErrorCnt;
   /**< Number of erroneous frames received */
    uint32_t maxWidth;
   /**< Max width in pixels - used for buffer allocation for all instance. */
    uint32_t maxHeight;
   /**< Max height in lines - used for buffer allocation for all instance. */
    Fvid2_Frame frames[APP_CAPT_FRAMES_PER_CH * APP_CAPT_CH_NUM];
   /**< FVID2 Frames that will be used for capture. */
    Csirx_InstStatus captStatus;
   /**< CSIRX Capture status. */
    uint32_t chFrmCnt[APP_CAPT_CH_NUM];
   /**< Number of frames captured per channel. */
    uint32_t errFrmCh[APP_ERR_FRAME_LOG_MAX];
   /**< Channel to which error frame belongs. */
    uint32_t errFrmNo[APP_ERR_FRAME_LOG_MAX];
   /**< Error frame number for the channel. */
    uint32_t errFrmTs[APP_ERR_FRAME_LOG_MAX];
   /**< TS in ms. */
} CsirxApp_CaptInstObj;

/**
 *  \brief Common application object.
 */
typedef struct
{
    Csirx_InitParams initPrms;
   /**< Csirx init time parameters */
    CsirxApp_CaptInstObj appInstObj[APP_CAPT_TEST_INST_NUM];
   /**< Capture application objects */
    struct Udma_DrvObj udmaDrvObj;
   /**< UDMA driver objects */
} CsirxApp_CaptCommonObj;

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
static void CsirxApp_ConsolePrintf(const char *pcString, ...);

/**
 * \brief   This function is used to initialize test parameters
 *
 * \param   appInstObj      Type of print message.
 *                          Refer to struct #appCaptObj
 *
 * \retval  none.
 */
static void CsirxApp_InitCaptParams(CsirxApp_CaptInstObj *appInstObj);

/**
 * \brief   App Init function.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_Init(CsirxApp_CaptCommonObj *appCommonObj);

/**
 * \brief   App create function.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_Create(CsirxApp_CaptInstObj *appInstObj);

/**
 * \brief   App delete function.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t CsirxApp_Delete(CsirxApp_CaptInstObj *appInstObj);

/**
 * \brief   App Callback function for frame completion.
 *
 *          It is an empty function, since it is not necessary
 *          it this example.
 *
 * \param   handle          Fvid2 DRV handle
 * \param   appData         App based back by to CB function
 * \param   reserved        reserved, not used
 *
 * \retval  status          FVID2_SOK on successful and else otherwise.
 */
static int32_t CsirxApp_FrameCompletionCb(Fvid2_Handle handle,
                                          Ptr appData,
                                          Ptr reserved);

/**
 * \brief   App print function for FVID2 driver.
 *
 * \param   str             Print string
 *
 * \retval  None.
 */
static void CsirxApp_FvidPrint(const char *str, ...);

/**
 * \brief   OS-based delay helper.
 *
 *          Suspends execution for the requested duration using the OSAL delay
 *          service.
 *
 * \param   waitInMs        Delay duration in milliseconds.
 *
 * \retval  None.
 */
static void CsirxApp_Delay(uint32_t waitInMs);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* App common object */
CsirxApp_CaptCommonObj gAppCommonObj;

/* CSIRX instances to use in application */
#if defined(SOC_J784S4)
uint32_t  gAppCapInst[APP_CAPT_TEST_INST_NUM] = {CSIRX_INSTANCE_ID_0, CSIRX_INSTANCE_ID_1, CSIRX_INSTANCE_ID_2};
#else
uint32_t  gAppCapInst[APP_CAPT_TEST_INST_NUM] = {CSIRX_INSTANCE_ID_0, CSIRX_INSTANCE_ID_1};
#endif

/* Fusion2 board detect flag */
int32_t gFusion2Det = BFALSE;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Application main
 */
int32_t Csirx_ReadbackTest(void)
{
    int32_t retVal = FVID2_SOK;
    int32_t bDet   = BFALSE;
    CsirxApp_CaptInstObj*   appInstObj[APP_CAPT_TEST_INST_NUM];
    CsirxApp_CaptCommonObj* appCommonObj;
    appCommonObj = &gAppCommonObj;

    /* Check for the Fusion2 board */
    bDet = Board_detectBoard(BOARD_ID_FUSION2);
    if (BTRUE == bDet)
    {
        gFusion2Det = BTRUE;
    }

    if (BTRUE == gFusion2Det)
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
        CsirxApp_Delay(INIT_DELAY);
    }

    for (uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
    {
        appInstObj[i] = &appCommonObj->appInstObj[i];
        memset(appInstObj[i], 0x0, sizeof (CsirxApp_CaptInstObj));
    }

    /* Initialize application object for current capture context */
    for (uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
    {
        appInstObj[i]->instId = gAppCapInst[i];
        CsirxApp_InitCaptParams(appInstObj[i]);
    }

    /* App Init */
    retVal += CsirxApp_Init(appCommonObj);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME ": [ERROR]CsirxApp_Init() FAILED!!!\r\n");
    }
    else
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": CsirxApp_Init() Successfully Passed!!!\r\n\n");
    }

    /* App Create */
    if (FVID2_SOK == retVal)
    {
        for (uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
        {
            retVal += CsirxApp_Create(appInstObj[i]);
            if (FVID2_SOK != retVal)
            {
                GT_1trace(CsirxAppTrace, GT_ERR, APP_NAME ": [ERROR]CsirxApp_Create() FAILED for CSIRX instance %d!!!\r\n\n",i);
            }
            else
            {
                GT_1trace(CsirxAppTrace, GT_INFO, APP_NAME ": CsirxApp_Create() Successfully Passed for CSIRX instance %d!!!\r\n\n",i);
            }
        }
    }

    if (FVID2_SOK == retVal)
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Initialization of CSIRX module DONE!!!\r\n");
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ":===================================================\r\n");
    }

    if (FVID2_SOK == retVal)
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Register check starts for instance 0...\r\n");

        IpFma_Status status = IPFMA_OK;

        // Get expected values from shim registers
        IpFma_CsirxShimRegs shimRegsExpValues;
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Loading expected shim register values...");
        memset(&shimRegsExpValues, 0, sizeof(IpFma_CsirxShimRegs));
        // status = IpFma_Csirx_GetShimRegs(CSL_CSI_RX_IF0_RX_SHIM_VBUSP_MMR_CSI2RXIF_BASE, &shimRegsExpValues);

        if (IPFMA_OK == status)
        {
            GT_0trace(CsirxAppTrace, GT_INFO, " DONE!\n");

            int8_t i = PERIODIC_CHECK_NUM;
            while (i > 0)
            {
                // Read the actual values from registers
                IpFma_CsirxShimRegs shimRegsActualValues;
                status = IpFma_Csirx_GetShimRegs(CSL_CSI_RX_IF0_RX_SHIM_VBUSP_MMR_CSI2RXIF_BASE, &shimRegsActualValues);

                if (IPFMA_OK == status)
                {
                    // Compare expected and actual values periodically
                    GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Comparing expected-actual shim register values...");
                    status = IpFma_Csirx_CompareShimRegs(&shimRegsExpValues, &shimRegsActualValues);

                    if (IPFMA_OK == status)
                    {
                        GT_0trace(CsirxAppTrace, GT_INFO, " values MATCH!\n");
                    }
                    else
                    {
                        GT_0trace(CsirxAppTrace, GT_ERR, " values MISMATCH!\n");
                    }
                    i--;
                }
                CsirxApp_Delay(PERIODIC_CHECK_DELAY);
            }
        }

        // Get expected values from dphy registers
        IpFma_CsirxDphyRegs dphyRegsExpValues;
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Loading expected dphy register values...");
        status = IpFma_Csirx_GetDphyRegs(CSL_DPHY_RX0_VBUS2APB_WRAP_VBUSP_K3_DPHY_RX_BASE, &dphyRegsExpValues);

        if (IPFMA_OK == status)
        {
            GT_0trace(CsirxAppTrace, GT_INFO, " DONE!\n");

            int8_t i = PERIODIC_CHECK_NUM;
            while (i > 0)
            {
                // Read the actual values from registers
                IpFma_CsirxDphyRegs dphyRegsActualValues;
                status = IpFma_Csirx_GetDphyRegs(CSL_DPHY_RX0_VBUS2APB_WRAP_VBUSP_K3_DPHY_RX_BASE, &dphyRegsActualValues);

                if (IPFMA_OK == status)
                {
                    // Compare expected and actual values periodically
                    GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Comparing expected-actual dphy register values...");
                    status = IpFma_Csirx_CompareDphyRegs(&dphyRegsExpValues, &dphyRegsActualValues);

                    if (IPFMA_OK == status)
                    {
                        GT_0trace(CsirxAppTrace, GT_INFO, " values MATCH!\n");
                    }
                    else
                    {
                        GT_0trace(CsirxAppTrace, GT_ERR, " values MISMATCH!\n");
                    }
                    i--;
                }
                CsirxApp_Delay(PERIODIC_CHECK_DELAY);
            }
        }

        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Register check done for instance 0!!!\n");
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ":===================================================\r\n\n");
    }

    /* TODO: Update example with loopback flow */

    /* App CSI delete function */
    if (FVID2_SOK == retVal)
    {
        for (uint32_t i = 0U; i < APP_CAPT_TEST_INST_NUM; i++)
        {
            retVal += CsirxApp_Delete(appInstObj[i]);
            if (FVID2_SOK != retVal)
            {
                GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME ": [ERROR]CsirxApp_Delete() FAILED!!!\r\n");
            }
        }
    }

    /* using 'CsirxApp_ConsolePrintf' instead of GT trace as Fvid2_deInit has happend */
    if (IPFMA_OK == retVal)
    {
        /* using 'CsirxApp_ConsolePrintf' instead of GT trace as Fvid2_deInit has happend */
        CsirxApp_ConsolePrintf("Application - DONE !!!\r\n");
        CsirxApp_ConsolePrintf("All tests have passed!!\n");
    }
    else
    {
        CsirxApp_ConsolePrintf("Application - FAILED !!!\r\n");
        CsirxApp_ConsolePrintf("Some tests have failed!!\n");
    }

    return (retVal);
}

static void CsirxApp_InitCaptParams(CsirxApp_CaptInstObj* appInstObj)
{
    uint32_t loopCnt = 0U;

    if (CSIRX_INSTANCE_ID_0 == appInstObj->instId)
    {
        appInstObj->boardCsiInstID = BOARD_CSI_INST_0;
    }
    else if (CSIRX_INSTANCE_ID_1 == appInstObj->instId)
    {
        appInstObj->boardCsiInstID = BOARD_CSI_INST_1;
    }
#if defined(SOC_J784S4)
    else if (CSIRX_INSTANCE_ID_2 == appInstObj->instId)
    {
        appInstObj->boardCsiInstID = BOARD_CSI_INST_2;
    }
#endif
    else
    {
        GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME ": Invalid Capture Instance\r\n");
    }

    /* set instance configuration parameters */
    Csirx_createParamsInit(&appInstObj->createPrms);
    appInstObj->createPrms.numCh = APP_CAPT_CH_NUM;

    /* set channel configuration parameters */
    for (loopCnt = 0U ; loopCnt < appInstObj->createPrms.numCh ; loopCnt++)
    {
        appInstObj->chFrmCnt[loopCnt]                           = 0U;
        appInstObj->createPrms.chCfg[loopCnt].chId              = loopCnt;
        appInstObj->createPrms.chCfg[loopCnt].chType            = CSIRX_CH_TYPE_CAPT;
        appInstObj->createPrms.chCfg[loopCnt].vcNum             = loopCnt;
        appInstObj->createPrms.chCfg[loopCnt].inCsiDataType     = APP_CAPT_IMAGE_DT;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.width      = APP_CAPT_FRAME_WIDTH;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.height     = APP_CAPT_FRAME_HEIGHT;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.pitch[0U]  = APP_CAPT_FRAME_PITCH;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.dataFormat = FVID2_DF_BGRX32_8888;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.ccsFormat  = APP_CAPT_IMAGE_STORAGE_FORMAT;
    }
    /* set module configuration parameters */
    appInstObj->createPrms.instCfg.enableCsiv2p0Support             = UTRUE;
    appInstObj->createPrms.instCfg.numDataLanes                     = 4U;
    appInstObj->createPrms.instCfg.enableErrbypass                  = UFALSE;
    appInstObj->createPrms.instCfg.enableStrm[CSIRX_CAPT_STREAM_ID] = 1U;
    for (loopCnt = 0U; loopCnt < appInstObj->createPrms.instCfg.numDataLanes; loopCnt++)
    {
        appInstObj->createPrms.instCfg.dataLanesMap[loopCnt] = (loopCnt + 1U);
    }
    /* set frame drop buffer parameters */
    // appInstObj->createPrms.frameDropBufLen = (APP_CAPT_FRAME_WIDTH * APP_CAPT_FRAME_BPP);
    /* This will be updated once Fvid2_create() is done */
    appInstObj->createStatus.retVal = FVID2_SOK;
    appInstObj->drvHandle           = NULL;
    Fvid2CbParams_init(&appInstObj->cbPrms);
    appInstObj->cbPrms.cbFxn   = (Fvid2_CbFxn) &CsirxApp_FrameCompletionCb;
    appInstObj->cbPrms.appData = appInstObj;
    appInstObj->numFramesRcvd  = 0U;
    appInstObj->frameErrorCnt  = 0U;
    appInstObj->maxWidth       = APP_CAPT_FRAME_WIDTH;
    appInstObj->maxHeight      = APP_CAPT_FRAME_HEIGHT;

    /* Initialize capture instance status */
    Csirx_instStatusInit(&appInstObj->captStatus);
}


static int32_t CsirxApp_Init(CsirxApp_CaptCommonObj* appCommonObj)
{
    int32_t  retVal    = FVID2_SOK;
    int32_t  dmaStatus = UDMA_SOK;
    uint32_t instId, loopCnt;
    Fvid2_InitPrms initPrms;
    Udma_InitPrms  udmaInitPrms;
    Udma_DrvHandle drvHandle;
    I2C_HwAttrs    i2cConfig;

    /* set instance initialization parameters */
    /* This will be updated once UDMA init is done */
    Csirx_initParamsInit(&appCommonObj->initPrms);
    drvHandle = &appCommonObj->udmaDrvObj;
    appCommonObj->initPrms.drvHandle = drvHandle;

    /* Fvid2 init */
    Fvid2InitPrms_init(&initPrms);
    initPrms.printFxn = &CsirxApp_FvidPrint;
    retVal = Fvid2_init(&initPrms);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME ": Fvid2 Init Failed!!!\r\n");
    }
    else
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Fvid2 Init Successfully Passed!!!\r\n");
    }

    /* Do UDMA init before CSIRX Init */

    instId = UDMA_INST_ID_BCDMA_0;
    UdmaInitPrms_init(instId, &udmaInitPrms);
    dmaStatus = Udma_init(drvHandle, &udmaInitPrms);
    if (UDMA_SOK != dmaStatus)
    {
        retVal = FVID2_EFAIL;
        GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME ": UDMA Init Failed!!!\r\n");
    }
    else
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": UDMA Init Successfully Passed!!!\r\n");
    }

    /* Csirx init */
    retVal = Csirx_init(&appCommonObj->initPrms);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME ": System Init Failed!!!\r\n");
    }
    else
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": System Init Successfully Passed!!!\r\n");
    }

    if (FVID2_SOK == retVal)
    {
        /* Initialize I2C Driver */
        for (loopCnt = 0U; loopCnt < I2C_HWIP_MAX_CNT; loopCnt++)
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

static int32_t CsirxApp_Create(CsirxApp_CaptInstObj* appInstObj)
{
    int32_t retVal = FVID2_SOK;
    Csirx_DPhyCfg   dphyCfg;
    Csirx_EventPrms eventPrms;

    /* Fvid2_create() */
    appInstObj->drvHandle = Fvid2_create(CSIRX_CAPT_DRV_ID,
                                        appInstObj->instId,
                                        &appInstObj->createPrms,
                                        &appInstObj->createStatus,
                                        &appInstObj->cbPrms);

    if ((NULL == appInstObj->drvHandle) || (FVID2_SOK != appInstObj->createStatus.retVal))
    {
        GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME ": Capture Create Failed!!!\r\n");
        retVal = appInstObj->createStatus.retVal;
    }
    else
    {
        GT_0trace(CsirxAppTrace, GT_INFO, APP_NAME ": Capture Create Successfully Passed!!!\r\n");
    }

    /* CSIRX D-PHY */
    if (FVID2_SOK == retVal)
    {
        /* Set CSIRX D-PHY configuration parameters */
        Csirx_initDPhyCfg(&dphyCfg);
        dphyCfg.inst = appInstObj->instId;
        retVal = Fvid2_control(appInstObj->drvHandle,
                                IOCTL_CSIRX_SET_DPHY_CONFIG,
                                &dphyCfg,
                                NULL);
        if (FVID2_SOK != retVal)
        {
            GT_1trace(CsirxAppTrace, GT_ERR, APP_NAME
                      ": Set D-PHY Configuration FAILED for CSIRX instance %d!!!\r\n",appInstObj->instId);
        }
        else
        {
            GT_1trace(CsirxAppTrace, GT_INFO, APP_NAME
                      ": Set D-PHY Configuration Successful for CSIRX instance %d!!!\r\n",appInstObj->instId);
        }
    }

    /* CSIRX Error Event */
    if (FVID2_SOK == retVal)
    {
        /* Register Error Events */
        Csirx_eventPrmsInit(&eventPrms);
        retVal = Fvid2_control(appInstObj->drvHandle,
                               IOCTL_CSIRX_REGISTER_EVENT,
                               &eventPrms,
                               NULL);
        if (FVID2_SOK != retVal)
        {
            GT_1trace(CsirxAppTrace, GT_ERR, APP_NAME
                      ": Error Events Registration FAILED for CSIRX instance %d!!!\r\n",appInstObj->instId);
        }
        else
        {
            GT_1trace(CsirxAppTrace, GT_INFO, APP_NAME
                      ": Error Events Registration Successful for CSIRX instance %d!!!\r\n",appInstObj->instId);
        }
    }

    return (retVal);
}

static int32_t CsirxApp_Delete(CsirxApp_CaptInstObj* appInstObj)
{
    int32_t retVal = FVID2_SOK;
    static Fvid2_FrameList frmList;

    Fvid2FrameList_init(&frmList);

    /* Dequeue all the request from the driver */
    retVal = Fvid2_dequeue(appInstObj->drvHandle,
                           &frmList,
                           0U,
                           FVID2_TIMEOUT_NONE);

    if ((FVID2_SOK != retVal) && (FVID2_ENO_MORE_BUFFERS != retVal))
    {
        GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME ": Capture De-queue Failed!!!\r\n");
    }

    if ((FVID2_SOK == retVal) || (FVID2_ENO_MORE_BUFFERS == retVal))
    {
        retVal = FVID2_SOK;
        /* Disable Error Events */
        retVal = Fvid2_control(appInstObj->drvHandle,
                               IOCTL_CSIRX_UNREGISTER_EVENT,
                               (void *)CSIRX_EVENT_GROUP_ERROR,
                               NULL);
        if (FVID2_SOK != retVal)
        {
            GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME
                      ":Error Events un-registration FAILED!!!\r\n");
        }
    }
    if (FVID2_SOK == retVal)
    {
        retVal = Fvid2_delete(appInstObj->drvHandle, NULL);
    }
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsirxAppTrace, GT_ERR, APP_NAME ": FVID2 Delete Failed!!!\r\n");
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

static void CsirxApp_ConsolePrintf(const char *pcString, ...)
{
    char printBuffer[APP_PRINT_BUFFER_SIZE];
    va_list arguments;

    /* Start the var args processing. */
    va_start(arguments, pcString);
    vsnprintf (printBuffer, sizeof(printBuffer), pcString, arguments);
    printf("%s",printBuffer);

    UART_printf(printBuffer);

    /* End the var args processing. */
    va_end(arguments);
}

static void CsirxApp_FvidPrint(const char *str, ...)
{
    CsirxApp_ConsolePrintf(str);

    return;
}

static int32_t CsirxApp_FrameCompletionCb(Fvid2_Handle handle,
                                          Ptr appData,
                                          Ptr reserved)
{
    return FVID2_SOK;
}

static void CsirxApp_Delay(uint32_t waitInMs)
{
    Osal_delay(waitInMs);
}
