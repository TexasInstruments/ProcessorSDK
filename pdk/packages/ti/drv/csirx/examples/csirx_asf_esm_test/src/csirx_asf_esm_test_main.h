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
 *  \file csirx_asf_esm_test_main.h
 *
 *  \brief CSI RX Capture Example Header File.
 *
 */
#ifndef csirx_asf_esm_test_main_H_
#define csirx_asf_esm_test_main_H_

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
#include "ti/osal/LoadP.h"
#include <ti/drv/csirx/csirx.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/board/src/devices/common/common.h>
#include <ti/board/board.h>
#include <ti/board/src/devices/board_devices.h>
#include "csirx_esm.h"

#if defined(SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_utils.h>
#include <ti/board/src/j721e_evm/include/board_i2c_io_exp.h>
#elif defined(SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_utils.h>
#include <ti/board/src/j721s2_evm/include/board_i2c_io_exp.h>
#elif defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_utils.h>
#include <ti/board/src/j784s4_evm/include/board_i2c_io_exp.h>
#elif defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_utils.h>
#include <ti/board/src/j742s2_evm/include/board_i2c_io_exp.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/**< Application name */
#define APP_NAME                                  "CSIRX_TRANS_TO_MON_APP"
/**< Number of CSIRX instances exist in SOC */
#define APP_TOTAL_CAPT_INST                       (CSIRX_INSTANCE_ID_MAX)
/**< Number of CSIRX instances to test parallelly */
#define APP_CAPT_TEST_INST_NUM                    ((uint32_t)2U)
/**< Number of channels */
#define APP_CAPT_CH_NUM                           ((uint32_t)1U)
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

/**< Time period to receive frames in app */
#define APP_TEST_PERIOD_IN_SEC                    ((uint32_t)10)

/**
 * @{
 * Macros to control Fusion board and Camera Sensor version for the capture
 */
/**< Fusion Board Revision.
     '0': for Rev B or older boards.
     '1': for Rev C board. */
#define FUSION_BOARD_VER                       (1U)
/**< D3 IMX390 sensor type. */
#define D3IMX390_CM_MODULE                     (0U)
#define D3IMX390_RCM_MODULE                    (1U)

#define UB960_USE_PATTERN_GENERATOR            (0U)
/** @} */

/**< I2C transaction timeout */
#define APP_I2C_TRANSACTION_TIMEOUT               ((uint32_t)2000U)

#define APP_CSIRX_INST0_CAMERA_SENSOR          (D3IMX390_RCM_MODULE)
#define APP_CSIRX_INST1_CAMERA_SENSOR          (D3IMX390_RCM_MODULE)
#define APP_CSIRX_INST2_CAMERA_SENSOR          (D3IMX390_RCM_MODULE)

/**< Test to verify CP_ASF events for CSIRX */
#define CSIRX_ASF_TRANS_TO_TEST                        (1U) /* Transaction timeout monitor test */
#define CSIRX_ASF_DAP_TEST                             (1U) /* Data and Address Parity test */

/**< Transaction timeout monitor */
#define APP_TRANS_TIMEOUT_MON_EN                  ((uint32_t)(CSIRX_ASF_TRANS_TO_TEST ? 1U : 0U))
#define APP_TRANS_TIMEOUT_MON_PCLKS               ((uint32_t)2000U)

/**< Transaction Timeout Event Fatal (1U) / NonFatal (0U) Select */
#define APP_TRANS_TIMEOUT_MON_FNF_SELECT          ((uint32_t)0U)

/**< Trigger ASF events */
#define APP_TRIGGER_ASF_TRANS_TO_ERR              ((uint32_t)(CSIRX_ASF_TRANS_TO_TEST ? 1U : 0U))
#define APP_TRIGGER_ASF_DAP_ERR                   ((uint32_t)(CSIRX_ASF_DAP_TEST ? 1U : 0U))

/**< Frame Attribute: Pitch in bytes */
#define APP_CAPT_FRAME_PITCH                      ((uint32_t)\
                                (APP_CAPT_FRAME_WIDTH * APP_CAPT_FRAME_BPP))
/**< Frame Attribute: size in bytes */
#define APP_CAPT_FRAME_SIZE                                ((uint32_t)\
            (APP_CAPT_FRAME_HEIGHT * APP_CAPT_FRAME_WIDTH * APP_CAPT_FRAME_BPP))

/**< For Ub960 Pattern Generator, most significant byte of active line length in
 * bytes 
 */
#define APP_CAPT_FRAME_LINE_LEN_HIGH           ((APP_CAPT_FRAME_PITCH & 0xFF00)>>8)

/**< For Ub960 Pattern Generator, least significant byte of active line length in
 * bytes 
 */
#define APP_CAPT_FRAME_LINE_LEN_LOW            (APP_CAPT_FRAME_PITCH & 0x00FF)


/**< For Ub960 Pattern Generator, most significant byte of number of active
 * lines in frame
 */
#define APP_CAPT_FRAME_HEIGHT_HIGH             ((APP_CAPT_FRAME_HEIGHT & 0xFF00)>>8)

/**< For Ub960 Pattern Generator, least significant byte of number of active
 * lines per frame 
 */
#define APP_CAPT_FRAME_HEIGHT_LOW              (APP_CAPT_FRAME_HEIGHT & 0x00FF)

/* Print buffer character limit for prints- UART or CCS Console */
#define APP_PRINT_BUFFER_SIZE                   ((uint32_t)4000)

/** \brief Log enable for CSIRX Sample application */
#define CsirxAppTrace                       ((uint32_t) GT_INFO   |\
                                             (uint32_t) GT_TraceState_Enable)

/**< Maximum number of error frame logs to store.
     It stores most recent errors.*/
#define APP_ERR_FRAME_LOG_MAX                                  ((uint32_t)500U)

/**< Print Driver Logs. Set to '1' to enable printing. */
#define APP_PRINT_DRV_LOGS                                     (0U)

/**< Number of channels */
#define APP_CAPT_CH_MAX                           ((uint32_t)4U)

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
   uint32_t asfTransToEvent;
   /**< Tracks the occurence ASF Transaction timeout error event. Used in CsirxApp_asfEventCallback */
   uint32_t asfDapErrEvent;
   /**< Tracks the occurence ASF DAP parity error event. Used in CsirxApp_asfEventCallback */
}CsirxApp_CaptInstObj;

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
}CsirxApp_CaptCommonObj;

typedef struct
{
     uint32_t transToTimeout;
     /**<  Asf transaction monitor timeout value (in pclk units) */
     uint32_t transToEnable;
     /**<  Asf transaction monitor enable/disable */
}CsirxApp_TransToMonObj;

/* ========================================================================== */
/*                          Static Function Declarations                      */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * \brief   App print function for FVID2 driver.
 *
 * \param   str             Print string
 *
 * \retval  None.
 */
void CsirxApp_fvidPrint(const char *str, ...);

/**
 * \brief   App event callback function for CSIRX ASF events
 *
 * \param   status             status of the event
 *                          
 *          appData            pointer to the CSIRX test Object
 * \retval  none.
 */
void CsirxApp_asfEventCallback(Csirx_EventStatus status,void *appData);

/**
 * \brief   This function is ISR for timer interrupt
 *
 * \param   arg             CSI RX Capture Test Object
 *                          
 *
 * \retval  none.
 */
void CsirxApp_timerIsr(void *arg);

/**
 * \brief   App function to get current time in msec.
 *
 * \param   None.
 *
 * \retval  I2C instance setup status.
 */
uint32_t CsirxApp_getCurTimeInMsec(void);

/**
 * \brief   App function to calculate the elapsed time from 'startTime' in msec.
 *
 * \param   None.
 *
 * \retval  I2C instance setup status.
 */
uint32_t CsirxApp_getElapsedTimeInMsec(uint32_t startTime);

/**
 * \brief   App Callback function for ESM interrupt. vrshn: add more.
 */
void SDL_ESM_applicationCallbackFunction(SDL_ESM_Inst esmInst,
                                            SDL_ESM_IntType esmIntrType,
                                            uint32_t grpChannel,
                                            uint32_t index,
                                            uint32_t intSrc,
                                            void *arg);

#if defined(FREERTOS)
/**
 * \brief   App function to print CPU load and Task load.
 *
 * \param   None.
 *
 * \retval  None.
 */
void CsirxApp_printLoad(void);
#endif

extern void App_wait(uint32_t wait_in_ms);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */


#endif /* End of csirx_asf_esm_test_main_H_ */