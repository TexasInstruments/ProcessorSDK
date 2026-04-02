/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2024
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
 *  \file safety_checkers_app_csirx.h
 *
 *  \brief CSIRX checkers application header file.
 *
 */

#ifndef SAFETY_CHECKERS_APP_CSIRX_H_
#define SAFETY_CHECKERS_APP_CSIRX_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#if defined (SOC_J722S)
#include <csirx.h>
#else
#include <ti/drv/csirx/csirx.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SAFETY_CHECKERS_APP_CSIRX_INSTANCE_ID                     ((uint32_t)0U)
/**< CSIRX instance ID */
#define SAFETY_CHECKERS_APP_CSIRX_CH_NUM                          ((uint32_t)1U)
/**< Number of channels */
#define SAFETY_CHECKERS_APP_CSIRX_TEST_PERIOD_IN_SEC              ((uint32_t)10)
/**< Time period to receive frames in app */
#define SAFETY_CHECKERS_APP_CSIRX_FRAME_WIDTH                     ((uint32_t)1936U)
/**< Frame Attribute: Width in pixels */
#define SAFETY_CHECKERS_APP_CSIRX_FRAME_HEIGHT                    ((uint32_t)1100U)
/**< Frame Attribute: Height in pixels */
#define SAFETY_CHECKERS_APP_CSIRX_FRAME_BPP                       ((uint32_t)2U)
/**< Frame Attribute: Bytes per pixel */
#define SAFETY_CHECKERS_APP_CSIRX_FRAME_SIZE                      (SAFETY_CHECKERS_APP_CSIRX_FRAME_WIDTH* \
								   SAFETY_CHECKERS_APP_CSIRX_FRAME_HEIGHT* \
							           SAFETY_CHECKERS_APP_CSIRX_FRAME_BPP)
/**< Frame Attribute: Size of the frame in bytes */

#if defined (SOC_J722S)
#define BOARD_CSI_I2C_MUX_INSTANCE                                ((uint32_t)0U)	
/**< I2C MUX instance to enable CSIRX */
#define BOARD_CSI_I2C_SWITCH_INSTANCE                             ((uint32_t)1U)
/**< I2C switch instance to enable DPHY interface for CSIRX */
#define SafetyCheckersApp_csirxlog                                DebugP_log
#else
#define SafetyCheckersApp_csirxlog                                UART_printf
/**< API for application log */
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief CSIRX safety checkers capture application object.
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
   I2C_Handle i2cHandle;
   /* I2c Handle to access serdes */ 
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
    Fvid2_Frame frames[4U * SAFETY_CHECKERS_APP_CSIRX_CH_NUM];
   /**< FVID2 Frames that will be used for capture. */
    Csirx_InstStatus captStatus;
   /**< CSIRX Capture status. */
    uint32_t chFrmCnt[SAFETY_CHECKERS_APP_CSIRX_CH_NUM];
   /**< Number of frames captured per channel. */
    uint32_t errFrmCh[500U];
   /**< Channel to which error frame belongs. */
    uint32_t errFrmNo[500U];
   /**< Error frame number for the channel. */
    uint32_t errFrmTs[500U];
   /**< TS in ms. */
}SafetyCheckersApp_CsirxInstObj;

/**
 *  \brief CSIRX safety checkers Common application object.
 */
typedef struct 
{
    Csirx_InitParams initPrms;
   /**< Csirx init time parameters */
    SafetyCheckersApp_CsirxInstObj appInstObj;
   /**< Capture application objects */
#if !defined (SOC_J722S)
  struct Udma_DrvObj udmaDrvObj;
   /**< UDMA driver objects */
   TimerP_Handle timerHandle;
   /**< timer handle for app timer */
   SemaphoreP_Handle completionSem;
   /**< Semaphore to notify app completion */
#else
    Udma_DrvObject udmaDrvObj;
   /**< UDMA driver objects */
   SemaphoreP_Object completionSem;
#endif
   /**< Semaphore to notify app completion */
   bool i2cInstOpened;
   /* param to verify if the instance is opened or not */

}SafetyCheckersApp_CsirxCommonObj;

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

static int32_t SafetyCheckersApp_csirxVerifyCheckers(SafetyCheckersApp_CsirxInstObj
				      *appInstObj);

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   This function is to setup Timer for application
 *
 * \param   appCommonObj    CSI RX Capture Test Object
 *
 * \retval  none
 *                          
 */
void SafetyCheckersApp_csirxSetupTimer(SafetyCheckersApp_CsirxCommonObj *appCommonObj);

/**
 * \brief   This function is to initialize semaphores for application
 *
 * \param   appCommonObj    CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */

int32_t SafetyCheckersApp_csirxSetupApp(SafetyCheckersApp_CsirxCommonObj
					*appCommonObj);

/**
 * \brief   This function is ISR for timer interrupt
 *
 * \param   arg             CSI RX Capture Test Object
 *                          
 *
 * \retval  none.
 */
void SafetyCheckersApp_csirxTimerIsr(void *arg);
/**
 * \brief   This function is used to initialize test parameters
 *
 * \param   appInstObj      Type of print message.
 *                          Refer to struct #appCaptObj
 *
 * \retval  none.
 */
void SafetyCheckersApp_csirxInitParams(SafetyCheckersApp_CsirxInstObj *appInstObj);

/**
 * \brief   App Init function.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t SafetyCheckersApp_csirxInit(SafetyCheckersApp_CsirxCommonObj *appCommonObj);

/**
 * \brief   App create function.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t SafetyCheckersApp_csirxCreate(SafetyCheckersApp_CsirxInstObj *appInstObj);

/**
 * \brief   App CSI test function: captures frames.
 *
 * \param   appCommonObj    CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
static int32_t SafetyCheckersApp_csirxTest(SafetyCheckersApp_CsirxCommonObj *appCommonObj);

/**
 * \brief   App delete function.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t SafetyCheckersApp_csirxDelete(SafetyCheckersApp_CsirxInstObj *appInstObj);

/**
 * \brief   App Init function.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t SafetyCheckersApp_csirxDeinit(SafetyCheckersApp_CsirxCommonObj *appCommonObj);

/**
 * \brief   App Callback function for frame completion.
 *
 * \param   handle        Fvid2 DRV handle
 *
 * \param   appData       App based back by to CB function
 *
 * \param   reserved      reserved, not used
 *
 * \retval  status        FVID2_SOK on successful
 *                        else otherwise.
 */
static int32_t SafetyCheckersApp_csirxFrameCompletionCb(Fvid2_Handle handle,
                                                        Ptr appData,
                                                        Ptr reserved);

/**
 * \brief   App Callback function for frame completion.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t SafetyCheckersApp_csirxAllocAndQFrames(SafetyCheckersApp_CsirxInstObj *appInstObj);

/**
 * \brief   App Callback function for frame completion.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t SafetyCheckersApp_csirxFreeFrames(SafetyCheckersApp_CsirxInstObj *appInstObj);

/**
 * \brief   App function to configure remote sensors.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  Sensor configuration status.
 */
int32_t SafetyCheckersApp_csirxSensorConfig(SafetyCheckersApp_CsirxInstObj* appInstObj);

/**
 * \brief   App function to configure remote sensors.
 *
 * \param   appInstObj      CSI RX Capture Test Object
 *
 * \retval  I2C instance setup status.
 */
int32_t SafetyCheckersApp_csirxSetupI2CInst(SafetyCheckersApp_CsirxInstObj* appInstObj);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SAFETY_CHECKERS_APP_CSIRX_H_ */
/* @} */
