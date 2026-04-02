/*
 * Copyright (c) 2016-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *
 * \file  nor_qspi.h
 *
 * \brief This file contains structure, typedefs, functions and
 *        prototypes used for QSPI interface for NOR flash.
 *
 *****************************************************************************/
#ifndef NOR_QSPI_H_
#define NOR_QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include <ti/board/src/flash/nor/nor.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/SPI_soc.h>

#if defined (evmK2G)
#include <ti/board/src/flash/nor/device/s25fl512s.h>
#elif defined (iceK2G)
#include <ti/board/src/flash/nor/device/s25fl256s.h>
#elif defined (tpr12_evm) || defined (awr294x_evm)
#include <ti/board/src/flash/nor/device/gd25b64cw2g.h>
#elif defined (tpr12_qt)
#include <ti/board/src/flash/nor/device/w25q16fwsf.h>
#endif

/**************************************************************************
 **                       Macro Definitions
 **************************************************************************/
#ifdef __cplusplus
}
#endif

#if defined (tpr12_evm) || defined (tpr12_qt) || defined (awr294x_evm)

#define SPI_CONFIG_OFFSET       (0)

#define SPI_CMD_TRANSFER_CMD    (SPI_V1_CMD_MMAP_TRANSFER_CMD)
#define SPI_CMD_TRANSFERMODE_RW (SPI_V1_CMD_TRANSFERMODE_RW)
#define SPI_CMD_SETCONFIGMODE   (SPI_V1_CMD_SETCONFIGMODE)
#define SPI_CMD_SETQSPIMODE     (SPI_V1_CMD_SETMEMMORYMAPMODE)
#define SPI_CMD_SETXFERLINES    (SPI_V1_CMD_SETRXLINES)

#define QSPI_IO_LINES_SINGLE    (QSPI_RX_LINES_SINGLE)
#define QSPI_IO_LINES_DUAL      (QSPI_RX_LINES_DUAL)
#define QSPI_IO_LINES_QUAD      (QSPI_RX_LINES_QUAD)

#else

#define SPI_CONFIG_OFFSET       (CSL_SPI_CNT)

#define SPI_CMD_TRANSFER_CMD    (SPI_V0_CMD_IND_TRANSFER_CMD)
#define SPI_CMD_TRANSFERMODE_RW (SPI_V0_CMD_TRANSFERMODE_RW)
#define SPI_CMD_SETCONFIGMODE   (SPI_V0_CMD_SETCONFIGMODE)
#define SPI_CMD_SETQSPIMODE     (SPI_V0_CMD_SETINDXFERMODE)
#define SPI_CMD_SETXFERLINES    (SPI_V0_CMD_SETXFERLINES)

#define QSPI_IO_LINES_SINGLE    (QSPI_XFER_LINES_SINGLE)
#define QSPI_IO_LINES_DUAL      (QSPI_XFER_LINES_DUAL)
#define QSPI_IO_LINES_QUAD      (QSPI_XFER_LINES_QUAD)

#endif

#endif /* NOR_QSPI_H_ */

/* Nothing past this point */
