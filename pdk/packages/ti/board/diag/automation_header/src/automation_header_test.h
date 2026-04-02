/******************************************************************************
 * Copyright (c) 2018-2022 Texas Instruments Incorporated - http://www.ti.com
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
 * \file   automation_header_test.h
 *
 * \brief  This is the header file for automation header diagnostic test.
 *
 */
#ifndef _AUTOMATION_HEADER_TEST_H_
#define _AUTOMATION_HEADER_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/csl/soc.h>

#include "board.h"
#include "board_cfg.h"

#include "diag_common_cfg.h"

#if (defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4))
#include "board_internal.h"
#include "board_i2c_io_exp.h"
#include "board_control.h"
#include <ti/csl/csl_gpio.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SIGNAL_LEVEL_LOW                           (0U)

#define CMD_INPUT_PORT0_REG_ACCESS                 (0x00U)
#define CMD_INPUT_PORT1_REG_ACCESS                 (0x01U)
#define CMD_INPUT_PORT2_REG_ACCESS                 (0x02U)

#define CMD_OUTPUT_PORT0_REG_ACCESS                (0x4U)
#define CMD_OUTPUT_PORT1_REG_ACCESS                (0x5U)
#define CMD_OUTPUT_PORT2_REG_ACCESS                (0x6U)

#define CMD_CFG_PORT0_REG_ACCESS                   (0xCU)
#define CMD_CFG_PORT1_REG_ACCESS                   (0xDU)
#define CMD_CFG_PORT2_REG_ACCESS                   (0xEU)

#define NUM_OF_SW2_CONFIGURABLE_POSITIONS          (9U)
#define NUM_OF_SW3_CONFIGURABLE_POSITIONS          (9U)

#if defined(SOC_J721E)
#define BOOTMODE_CFG_SET1_PIN_POS                  (0x00U)
#define BOOTMODE_CFG_SET2_PIN_POS                  (0x07U)
#elif defined(SOC_J7200)
#define BOOTMODE_CFG_SET1_PIN_POS                  (0x00U)
#define BOOTMODE_CFG_SET2_PIN_POS                  (0x07U)
#elif defined(SOC_J721S2)
#define BOOTMODE_CFG_SET1_PIN_POS                  (0x00U)
#define BOOTMODE_CFG_SET2_PIN_POS                  (0x07U)
#elif defined(SOC_J784S4)
#define BOOTMODE_CFG_SET1_PIN_POS                  (0x0EU) // SW7
#define BOOTMODE_CFG_SET2_PIN_POS                  (0x00U) // SW11
#elif defined(SOC_AM64X)
#define BOOTMODE_CFG_SET1_PIN_POS                  (0x3BU) /* SW2 */
#define BOOTMODE_CFG_SET2_PIN_POS                  (0x00U) /* SW3 */
#else
#define BOOTMODE_CFG_SET1_PIN_POS                  (0x06U) /* SW3 */
#define BOOTMODE_CFG_SET2_PIN_POS                  (0x10U) /* SW2 */
#endif

#if defined(SOC_AM64X)
#define BOARD_DIAG_AUTO_HDR_I2C_INSTANCE           (1U)
#define BOARD_DIAG_AUTO_HDR_I2C_BASEADDR           (CSL_I2C1_CFG_BASE)
#else
#define BOARD_DIAG_AUTO_HDR_I2C_INSTANCE           (2U)
#define BOARD_DIAG_AUTO_HDR_I2C_BASEADDR           (CSL_I2C2_CFG_BASE)
#endif
/**
 *  \brief    The function performs the Automation header test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_automationHeaderTest(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _AUTOMATION_HEADER_TEST_H_ */
