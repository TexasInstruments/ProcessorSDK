/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
 * \file   dsitx_test.h
 *
 * \brief  This is the dsitx diagnostic header file.
 *
 */
#ifndef _DSITX_TEST_H_
#define _DSITX_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "board_cfg.h"
#include "board_internal.h"
#include "board_i2c_io_exp.h"
#include <ti/board/src/devices/board_devices.h>

#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/dss/dss.h>
#include <ti/drv/pm/pmlib.h>
#include <ti/drv/sciclient/sciclient.h>

#include "diag_common_cfg.h"

#define BOARD_DIAG_DSITX_VP_ID                      (CSL_DSS_VP_ID_3)
#define BOARD_DIAG_DSITX_OVERLAY_ID                 (CSL_DSS_OVERLAY_ID_3)
#define BOARD_DIAG_DSITX_VID_NODE_ID                (DSS_DCTRL_NODE_VID1)
#define BOARD_DIAG_DSITX_OVERLAY_NODE_ID            (DSS_DCTRL_NODE_OVERLAY3)
#define BOARD_DIAG_DSITX_VP_NODE_ID                 (DSS_DCTRL_NODE_VP3)
#define BOARD_DIAG_DSITX_OUT_NODE_ID                (DSS_DCTRL_NODE_DSI_DPI2)

#define BOARD_DIAG_DSITX_PCLK                       (74250000U)

#define BOARD_DIAG_DSITX_LCD_WIDTH                  (1280U)
#define BOARD_DIAG_DSITX_LCD_HEIGHT                 (800U)
#define BOARD_DIAG_DSITX_LCD_HFPORCH                (110U)
#define BOARD_DIAG_DSITX_LCD_HBPORCH                (220U)
#define BOARD_DIAG_DSITX_LCD_VFPORCH                (5U)
#define BOARD_DIAG_DSITX_LCD_VBPOTCH                (20U)
#define BOARD_DIAG_DSITX_LCD_HSYNC                  (40U)
#define BOARD_DIAG_DSITX_LCD_VSYNC                  (5U)

#define BOARD_DIAG_DSITX_MAX_BRIGHTNESS             (99U)

#define BOARD_DIAG_DSITX_FPD_LCD_PWM_SLV_ADDR       (0x27U)
#define BOARD_DIAG_DSITX_FPD_LCD_PWM_REG_ADDR       (0x00U)
#define BOARD_DIAG_DSITX_FPD_LED_EN                 (0xFEU)

typedef struct
{
    Fvid2_Handle dctrlHandle;
    /**< DCTRL handle */
    Dss_InitParams initParams;
    /**< DSS Initialization Parameters */
    Dss_DctrlPathInfo dctrlPathInfo;
    /**< DSS Path Information */
    Dss_DctrlVpParams vpParams;
    /**< VP Params */
    Dss_DctrlOverlayParams overlayParams;
    /**< Overlay Params */
    Dss_DctrlAdvVpParams advVpParams;
    /**< Advance VP Params */
} BoardDiag_DsiDisplayObj;

/**
 *  \brief    The function performs the DSITX test.
 *
 *  \return   int8_t  
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_DsiTest(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DSITX_TEST_H_ */
