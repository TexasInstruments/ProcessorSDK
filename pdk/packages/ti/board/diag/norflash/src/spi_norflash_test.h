/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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

/**
 *  \file   spi_norflash_test.h
 *
 *  \brief  This is the header file for spi norflash diagnostic test.
 *
 */

#ifndef _SPI_NORFLASH_TEST_H_
#define _SPI_NORFLASH_TEST_H_

#include <ti/csl/src/ip/mcspi/V0/mcspi.h>
#include <ti/csl/src/ip/mcspi/V0/hw_mcspi.h> 

#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/SPI_soc.h>

#include "board.h"
#include "board_cfg.h"

#include "board_control.h"

#include "diag_common_cfg.h"

#define BOARD_DIAG_NOR_PAGE_SIZE                           (256U)
#define BOARD_DIAG_TEST_DATA_LEN                           (BOARD_DIAG_NOR_PAGE_SIZE)
#define BOARD_DIAG_BUFF_SIZE                               (BOARD_DIAG_TEST_DATA_LEN)
#define BOARD_DIAG_MAX_BUFF_SIZE                           (BOARD_DIAG_NOR_PAGE_SIZE)

#define BOARD_DIAG_PAGES_PER_SECTOR                        (256U)
#define BOARD_DIAG_SECTOR_SIZE                             (BOARD_DIAG_PAGES_PER_SECTOR * BOARD_DIAG_NOR_PAGE_SIZE)
#define BOARD_DIAG_MAX_SECTORS                             (1024U)
#define BOARD_DIAG_TESTPAGES                               (1U)

#ifdef DIAG_STRESS_TEST
#define BOARD_DIAG_SPI_FLASH_TEST_PAGES                    (BOARD_DIAG_MAX_SECTORS * BOARD_DIAG_PAGES_PER_SECTOR)
#else
#define BOARD_DIAG_SPI_FLASH_TEST_PAGES                    (1U)
#endif

/* Flash device commands */
#define BOARD_DIAG_NOR_CMD_SECTOR_ERASE                    (0xD8U)
#define BOARD_DIAG_NOR_CMD_WREN                            (0x06U)
#define BOARD_DIAG_NOR_CMD_RDSR                            (0x05U)
#define BOARD_DIAG_NOR_CMD_READ                            (0x03U)
#define BOARD_DIAG_NOR_CMD_PAGE_PROG                       (0x02U)
#define BOARD_DIAG_NOR_CMD_RDID                            (0x9FU)

#define BOARD_DIAG_SPI_BASE_ADDR                           CSL_MCSPI5_CFG_BASE

#define BOARD_DIAG_SPI_CH                                  MCSPI_CHANNEL_0

#define BOARD_DIAG_MCSPI_OUT_FREQ                          (24000000U)
#define BOARD_DIAG_MCSPI_IN_CLK                            (48000000U)

#define BOARD_DIAG_SPI_FLASH_DEVID_BYTE0                   (0x20)
#define BOARD_DIAG_SPI_FLASH_DEVID_BYTE1                   (0xBB)
#define BOARD_DIAG_SPI_FLASH_DEVID_BYTE2                   (0x20)

/**
 *  \brief    This function runs spi norflash test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_SpiNorflashTest(void);

#endif /* _SPI_NORFLASH_TEST_H_ */
