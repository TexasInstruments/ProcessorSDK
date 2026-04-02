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
 * \file      spi_eeprom_test.h
 *
 * \brief     This file contains spi eeprom specific structure, typedefs,
 *            function prototypes.
 *
 */

#ifndef _SPI_EEPROM_TEST_H_
#define _SPI_EEPROM_TEST_H_

#include <stdlib.h>

#include <ti/drv/spi/SPI.h>
#include <ti/board/src/flash/include/board_flash.h>
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/csl/src/ip/mcspi/V0/mcspi.h>
#include <ti/csl/src/ip/mcspi/V0/hw_mcspi.h>

#include "board.h"
#include "board_cfg.h"
#include "diag_common_cfg.h"
#include "soc.h"

#ifdef DIAG_STRESS_TEST
/* Total memory is 64 * 2 bytes is 128 bytes or 1Kb */
#define BOARD_DIAG_MAX_BUFF_SIZE               (64U)
#else
#define BOARD_DIAG_MAX_BUFF_SIZE               (32U)
#endif

#define BOARD_DIAG_NOR_CMD_SECTOR_ERASE        (0x1C0U)
#define BOARD_DIAG_NOR_CMD_ERASE_ALL           (0x120U)
#define BOARD_DIAG_NOR_CMD_WREN                (0x130U)
#define BOARD_DIAG_NOR_CMD_READ                (0x180U) /* Read opcode - 10, address trying to read is 0x0 */
#define BOARD_DIAG_NOR_CMD_PAGE_PROG           (0x140U) /* Write opcode - 01, address is 0x0 */
#define BOARD_DIAG_NOR_CMD_WRITE_ALL           (0x110U) /* Opcode - 00, LAst 4 bits are don't care */

#define BOARD_DIAG_SPI0_BASE_ADDRESS           CSL_MCSPI0_CFG_BASE

#define BOARD_DIAG_MCSPI_OUT_FREQ              (1000000U)
#define BOARD_DIAG_MCSPI_IN_CLK                (48000000U)

/**
 * \brief  spi flash test function
 *
 *  This test executes SPI NOR flash memory access test and boundary test.
 *  Memory access test can be verified using the erase, read and write commands
 *  Boundary test can be verified by performing memory access test on first and last
 *  pages of the memory.
 *
 * \return  int
 *             0  - in case of success
 *             -1  - in case of failure
 */
int BoardDiag_SpiFlashTest(void);

/**
 * \brief  spi test function
 *
 * This function executes spi flash test
 *
 * \return  int
 *              0  - in case of success
 *             -1  - in case of failure
 */
int BoardDiag_SpiTest(void);

#endif // _SPI_EEPROM_TEST_H_
