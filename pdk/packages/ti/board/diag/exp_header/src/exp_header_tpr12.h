/******************************************************************************
* Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
*****************************************************************************/

/**
 *
 * \file   exp_header_tpr12.h
 *
 * \brief  This is the header file for expansion header diagnostic test.
 *
 */

#ifndef _EXP_HEADER_TPR12_H_
#define _EXP_HEADER_TPR12_H_

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include <ti/csl/soc.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/SPI_soc.h>

#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/mibspi/soc/MIBSPI_soc.h>

#include <ti/drv/gpio/soc/GPIO_soc.h>
#include "diag_common_cfg.h"

#include "board.h"
#include "board_cfg.h"
#include "board_pinmux.h"

#define BOARD_DIAG_TEST_SIZE                         (64U)
#define BOARD_DIAG_GPIO_PIN_HIGH                     (0x01U)
#define BOARD_DIAG_GPIO_PIN_LOW                      (0x00U)
#define BOARD_DIAG_MAX_GPIO_CNTRL_LINES              (0x02U)
#define BOARD_DIAG_GPIO_TOGGLE_FREQUENCY             (0x05U)
#define BOARD_DIAG_NUM_MIBSPI_INSTANCES              (2U)
#define BOARD_DIAG_MAX_GPIO_LINES_PER_SPI_INST       (2U)
/*Total number of gpio lines */
#define BOARD_DIAG_TOTAL_NUM_GPIO_LINES    \
    (BOARD_DIAG_MAX_GPIO_LINES_PER_SPI_INST * BOARD_DIAG_NUM_MIBSPI_INSTANCES)


#define BOARD_DIAG_SPI_MASTER_START_COMMAND            (0x5AU)
#define BOARD_DIAG_SPI_SLAVE_ACK                       (0xA5U)
#define BOARD_DIAG_SPI_SLAVE_STOP_COMMAND              (0xAAU)

/* MIBPSIA_HOST_IRQ pinmux offset */
#define BOARD_DIAG_PAD_CY_OFFSET_ADDRESS            (0x130U)
/* SYNC_FE1 pinmux offset */
#define BOARD_DIAG_PAD_CS_OFFSET_ADDRESS            (0x118U)
/* MIBPSIB_HOST_IRQ pinmux offset */
#define BOARD_DIAG_PAD_CZ_OFFSET_ADDRESS            (0x134U)
/* SYNC_FE2 pinmux offset */
#define BOARD_DIAG_PAD_CX_OFFSET_ADDRESS            (0x12CU)

/* gpio Mux mode for each pin */
#define BOARD_DIAG_GPIO_42_MUX_MODE                (0U)
#define BOARD_DIAG_GPIO_34_MUX_MODE                (7U)
#define BOARD_DIAG_GPIO_43_MUX_MODE                (0U)
#define BOARD_DIAG_GPIO_35_MUX_MODE                (10U)

/* Lower 4 bits for    setting mux mode */
#define BOARD_DIAG_MUX_MODE_PIN_MASK               (0x0FU)

#define BOARD_DIAG_SPI_A_TEST    (1U)

/**
 *  \brief    The function performs Expansion header verification test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_expansionHeaderTest(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* #ifndef _EXP_HEADER_TPR12_H_ */
