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
 *
 * \file   csitx_rx_cfg.h
 *
 * \brief  This is the header file for CSITx_Rx diagnostic test.
 *
 */

#ifndef CSITX_RX_CFG_H_
#define CSITX_RX_CFG_H_

#include <stdio.h>
#include <string.h>

#include <ti/osal/osal.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/csirx/csirx.h>
#include <ti/drv/csitx/csitx.h>
#include <ti/board/src/devices/common/common.h>
#include <ti/board/board.h>
#include <ti/board/src/devices/board_devices.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/pm/pmlib.h>

/**< Enable CSITX to CSITX loopback mode
     0: Disable loopback mode, CSITX will transmit frames continuously
     1: Enable loopback mode, CSITX will transmit and CSIRX will receive
        frames continuously */
#define BOARD_DIAG_ENABLE_LOOPBACK                     (1U)
/**< CSITX instance to be used. Valid values are: 
      0: For CSITX instance 0
      1: For CSITX instance 1 (Only for J721S2 and J784S4, as J721E has only instance 0) */
#define CSITX_INSTANCE                          (1U)
/**< Number of channels */
#define BOARD_DIAG_TX_CH_NUM                           ((uint32_t)4U)
/**< Input Image Data format */
#define BOARD_DIAG_TX_IMAGE_DT                         (FVID2_CSI2_DF_RAW12)
/**< Frame storage format. Only valid for RAW12 DT. */
#define BOARD_DIAG_TX_IMAGE_STORAGE_FORMAT             (FVID2_CCSF_BITS12_UNPACKED16)
/**< Number of frames to transmit. Set it to '0' for indefinite transmit */
#define BOARD_DIAG_TX_FRAMES_TX                        ((uint32_t)500U)
/**< Number of frames per Tx channel */
#define BOARD_DIAG_TX_FRAMES_PER_CH                    ((uint32_t)1U)
/**< Number of frames per Rx channel */
#define BOARD_DIAG_RX_FRAMES_PER_CH                    ((uint32_t)4U)
/**< Frame Attribute: Width in pixels */
#define BOARD_DIAG_TX_FRAME_WIDTH                      ((uint32_t)1920U)
/**< Frame Attribute: Height in pixels */
#define BOARD_DIAG_TX_FRAME_HEIGHT                     ((uint32_t)1080U)
/**< Frame Attribute: Bytes per pixel */
#define BOARD_DIAG_TX_FRAME_BPP                        ((uint32_t)2U)
/**< Frame storage format. Only valid for RAW12 DT. */
#define BOARD_DIAG_CAPT_IMAGE_STORAGE_FORMAT           (FVID2_CCSF_BITS12_UNPACKED16)

/**< Frame Attribute: Pitch in bytes */
#define BOARD_DIAG_TX_FRAME_PITCH                      ((uint32_t)\
                                (BOARD_DIAG_TX_FRAME_WIDTH * BOARD_DIAG_TX_FRAME_BPP))
/**< Frame Attribute: size in bytes */
#define BOARD_DIAG_TX_FRAME_SIZE                                ((uint32_t)\
            (BOARD_DIAG_TX_FRAME_HEIGHT * BOARD_DIAG_TX_FRAME_WIDTH * BOARD_DIAG_TX_FRAME_BPP))

/* Print buffer character limit for prints- UART or CCS Console */
#define BOARD_DIAG_PRINT_BUFFER_SIZE                   ((uint32_t)4000)

/**< Maximum number of error frame logs to store.
     It stores most recent errors.*/
#define BOARD_DIAG_ERR_FRAME_LOG_MAX                                  ((uint32_t)500U)

/**
 *  \brief Capture application object.
 */
typedef struct
{
    uint32_t instId;
   /**< Csirx Drv Instance ID. */
    Csirx_InitParams initPrms;
   /**< Csirx init time parameters */
    Csirx_CreateParams createPrms;
   /**< Csirx create time parameters */
    Csirx_CreateStatus createStatus;
   /**< Csirx create time status */
    Fvid2_Handle drvHandle;
   /**< FVID2 capture driver handle. */
    Fvid2_CbParams cbPrms;
   /**< Callback params. */
    uint32_t numFramesToCapture;
   /**< Number of frames to receive for a given configuration */
    volatile uint32_t numFramesRcvd;
   /**< Number of frames received */
    uint32_t frameErrorCnt;
   /**< Number of erroneous frames received */
    Fvid2_Frame frames[BOARD_DIAG_TX_FRAMES_PER_CH * BOARD_DIAG_TX_CH_NUM];
   /**< FVID2 Frames that will be used for capture. */
    Csirx_InstStatus captStatus;
   /**< CSIRX Capture status. */
    uint32_t chFrmCnt[BOARD_DIAG_TX_CH_NUM];
   /**< Number of frames captured per channel. */
    uint32_t errFrmCh[BOARD_DIAG_ERR_FRAME_LOG_MAX];
   /**< Channel to which error frame belongs. */
    uint32_t errFrmNo[BOARD_DIAG_ERR_FRAME_LOG_MAX];
   /**< Error frame number for the channel. */
    uint32_t errFrmTs[BOARD_DIAG_ERR_FRAME_LOG_MAX];
   /**< TS in ms. */
} BoardDiag_CsiRxObj;

/**
 *  \brief Tx application object.
 */
typedef struct
{
    uint32_t instId;
   /**< Csitx Drv Instance ID. */
    Csitx_InitParams initPrms;
   /**< Csitx init time parameters */
    Csitx_CreateParams createPrms;
   /**< Csitx create time parameters */
    Csitx_CreateStatus createStatus;
   /**< Csitx create time status */
    Fvid2_Handle drvHandle;
   /**< FVID2 transmit driver handle. */
    Fvid2_CbParams cbPrms;
   /**< Callback params. */
    uint32_t numFramesToTx;
   /**< Number of frames to transmit for a given configuration */
    volatile uint32_t numFramesTx;
   /**< Number of frames transmitted */
    uint32_t frameErrorCnt;
   /**< Number of erroneous frames transmitted */
    uint32_t maxWidth;
   /**< Max width in pixels - used for buffer allocation for all instance. */
    uint32_t maxHeight;
   /**< Max height in lines - used for buffer allocation for all instance. */
    Fvid2_Frame frames[BOARD_DIAG_TX_FRAMES_PER_CH * BOARD_DIAG_TX_CH_NUM];
   /**< FVID2 Frames that will be used for transmit. */
    Csitx_InstStatus txStatus;
   /**< CSITX Tx status. */
    uint32_t chFrmCnt[BOARD_DIAG_TX_CH_NUM];
   /**< Number of frames transmitted per channel. */
    uint32_t errFrmCh[BOARD_DIAG_ERR_FRAME_LOG_MAX];
   /**< Channel to which error frame belongs. */
    uint32_t errFrmNo[BOARD_DIAG_ERR_FRAME_LOG_MAX];
   /**< Error frame number for the channel. */
    uint32_t errFrmTs[BOARD_DIAG_ERR_FRAME_LOG_MAX];
   /**< TS in ms. */
   BoardDiag_CsiRxObj captObj;
   /**< CSIRX Capture Object. */
}BoardDiag_CsiTxObj;

extern uint8_t gTxFrms[(BOARD_DIAG_TX_FRAMES_PER_CH * BOARD_DIAG_TX_CH_NUM)][BOARD_DIAG_TX_FRAME_SIZE];
extern uint8_t gCompareFrame;


#endif /* CSITX_RX_CFG_H_ */
