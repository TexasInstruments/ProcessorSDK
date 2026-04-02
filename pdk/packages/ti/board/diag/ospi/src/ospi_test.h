/*
 * Copyright (C) 2018-2022 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   ospi_test.h
 *
 *  \brief  ospi diagnostic test header file.
 *
 */

#ifndef _OSPI_TEST_H_
#define _OSPI_TEST_H_

#include <stdlib.h>

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4)
#include <ti/csl/csl_gpio.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#endif

#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/SPI_soc.h>

#include <ti/board/src/flash/include/board_flash.h>
#if defined(j7200_evm) || defined (am64x_evm) || defined(j721s2_evm) || defined(j784s4_evm)
#include <ti/board/src/flash/nor/ospi/nor_xspi.h>
#else
#include <ti/board/src/flash/nor/ospi/nor_ospi.h>
#endif
#include "board.h"
#include "board_cfg.h"
#include "diag_common_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(am64x_evm) && !defined(j721s2_evm) && !defined(j784s4_evm)
#define UDMA_ENABLE
#endif

#if defined(UDMA_ENABLE)
/* for enabling SPI_DMA */
#include <ti/drv/udma/udma.h>

/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much memcpy operations */
#define UDMA_TEST_APP_RING_ENTRIES      (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define UDMA_TEST_APP_RING_ENTRY_SIZE   (sizeof(uint64_t))
/** \brief Total ring memory */
#define UDMA_TEST_APP_RING_MEM_SIZE     (UDMA_TEST_APP_RING_ENTRIES * \
                                         UDMA_TEST_APP_RING_ENTRY_SIZE)
/**
 *  \brief UDMA TR packet descriptor memory.
 *  This contains the CSL_UdmapCppi5TRPD + Padding to sizeof(CSL_UdmapTR15) +
 *  one Type_15 TR (CSL_UdmapTR15) + one TR response of 4 bytes.
 *  Since CSL_UdmapCppi5TRPD is less than CSL_UdmapTR15, size is just two times
 *  CSL_UdmapTR15 for alignment.
 */
#define UDMA_TEST_APP_TRPD_SIZE         ((sizeof(CSL_UdmapTR15) * 2U) + 4U)
#endif

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4)
#define BOARD_DIAG_OSPI_HYPER_BUS_SEL_PIN     (0U)
#endif

#if defined(j7200_evm) || defined (am64x_evm) || defined(am64x_svb) || defined(j721s2_evm) || defined(j784s4_evm)
#define BOARD_DIAG_OSPI_FLASH_ID              (BOARD_FLASH_ID_S28HS512T)
#else
#define BOARD_DIAG_OSPI_FLASH_ID              (BOARD_FLASH_ID_MT35XU512ABA1G12)
#endif

#ifdef DIAG_STRESS_TEST
#define ONE_KB_SIZE                           (1024U) /* 1024 Bytes */

#define TEST_DATA_LEN                         (NOR_BLOCK_SIZE)
#define MAX_BUFF_SIZE                         (TEST_DATA_LEN + ONE_KB_SIZE)


#else /* #ifdef DIAG_STRESS_TEST */

#define TEST_DATA_LEN                         (NOR_PAGE_SIZE)
#define MAX_BUFF_SIZE                         (TEST_DATA_LEN + 4)

#endif /* #ifdef DIAG_STRESS_TEST */

#define BOARD_OSPI_FIRST_PAGE        (0x0000)
#define BOARD_OSPI_LAST_PAGE         (NOR_SIZE - TEST_DATA_LEN)

/**
 * \brief  ospi test function
 *
 * This function executes ospi flash test
 *
 * \return  int8_t
 *              0  - in case of success
 *             -1  - in case of failure
 */
int8_t BoardDiag_OspiTest(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OSPI_TEST_H_ */
