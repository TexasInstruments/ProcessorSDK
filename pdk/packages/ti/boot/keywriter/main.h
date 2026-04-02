/**
 *  \file   main.h
 *
 *  \brief  This file contains all macros required for OTP Keywriter
 *
 */

/*
 * Copyright (C) 2021-2023 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef KEYWR_MAIN_H_
#define KEYWR_MAIN_H_

#include <string.h>
#include <ti/csl/cslr_device.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/csl_gpio.h>
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/src/UART_osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>
#include <ti/drv/sciclient/sciclient.h>

#include <ti/boot/sbl/soc/k3/sbl_soc_cfg.h>
#include <ti/boot/sbl/soc/k3/sbl_sci_client.h>

#include <ti/board/board.h>
#include <ti/csl/soc.h>

#include <keywr_defaultBoardcfg_hex.h>
#include <keywr_defaultBoardcfg_rm_hex.h>
#include <keywr_defaultBoardcfg_pm_hex.h>
#include <keywr_defaultBoardcfg_security_hex.h>

#include <keywriter_utils.h>

/* Discard any previous definitions
 * These two macros are defined in tifs_keywriter.h file.
 * */
#undef TIFS_KEYWRITER_BIN_SIZE_IN_BYTES
#undef TIFS_KEYWRITER_BIN
#include "tifs_keywriter.h"

#define OTP_VERSION_STR  "OTP Keywriter Version: 02.00.00.00"

#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)

#define MCU_UART_TXD_MUX_ADDR           (BOARD_WKUP_PMUX_CTRL_ADDR + \
                                             PIN_WKUP_GPIO0_12)
#define WKUP_UART_TXD_MUX_ADDR          (BOARD_WKUP_PMUX_CTRL_ADDR + \
                                             PIN_WKUP_UART0_TXD)

#define PIN_INPUT                       (PIN_INPUT_ENABLE | \
                                             PIN_PULL_DISABLE)
#define PIN_OUTPUT                      (PIN_PULL_DISABLE)

#define KEYWRITER_BOARD_UART_INSTANCE   (0U)
#define SBL_ROM_UART_MODULE_INPUT_CLK   (48000000U)
#define SBL_SYSFW_UART_MODULE_INPUT_CLK (96000000U)
#define MCU_FSS0_S0_FWID                (1036)
#define MCU_FSS0_S0_FW_REGIONS          (8)

#define BOARD_WKUP_PMUX_CTRL_ADDR       (CSL_WKUP_CTRL_MMR0_CFG0_BASE + \
                                             0x1C000)
#define CTRLMMR_LOCK_KICK0_UNLOCK_VAL   (0x68EF3490)
#define CTRLMMR_LOCK_KICK1_UNLOCK_VAL   (0xD172BC5A)
#define WKUP_CTRL_BASE                  (0x43000000)
#define CTRL_MMR0_PARTITION_SIZE        (0x4000)
#define CTRLMMR_LOCK_KICK0              (0x01008)
#define CTRLMMR_LOCK_KICK1              (0x0100C)

#endif

const uint32_t gKeywr_boardCfgLow[(KEYWR_BOARDCFG_SIZE_IN_BYTES + 3U) / 4U]
__attribute__((aligned(128), section(".boardcfg_data"))) =
    KEYWR_BOARDCFG;
const uint32_t gKeywr_boardCfgLow_rm[(KEYWR_BOARDCFG_RM_SIZE_IN_BYTES + 3U) /
                                     4U]
__attribute__((aligned(128), section(".boardcfg_data"))) =
    KEYWR_BOARDCFG_RM;
const uint32_t gKeywr_boardCfgLow_sec[(KEYWR_BOARDCFG_SECURITY_SIZE_IN_BYTES +
                                       3U) / 4U]
__attribute__((aligned(128), section(".boardcfg_data"))) =
    KEYWR_BOARDCFG_SECURITY;
const uint32_t gKeywr_boardCfgLow_pm[(KEYWR_BOARDCFG_PM_SIZE_IN_BYTES + 3U) /
                                     4U]
__attribute__((aligned(128), section(".boardcfg_data"))) =
    KEYWR_BOARDCFG_PM;

/**
 * \brief  CSL Reset Vectors
 *
 * \param  fileName
 * \param  lineNo
 *
 * \return None
 */
void KeywrErrLoop(const char *fileName, int32_t lineNo);

/**
 * \brief SoC specific implementation to pull VPP high for efuse programming
 *
 * \return None
 */
void OTP_VppEn(void);

#endif  /* KEYWR_MAIN_H_ */
