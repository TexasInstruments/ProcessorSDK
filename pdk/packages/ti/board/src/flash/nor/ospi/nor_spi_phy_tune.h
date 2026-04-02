/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
 * \file  nor_spi_phy_tune.h
 *
 * \brief This file contains OSPI/xSPI PHY tuning structure, macro definitions
 *
 *****************************************************************************/
#ifndef NOR_SPI_PHY_TUNE_H_
#define NOR_SPI_PHY_TUNE_H_

#include <ti/board/src/flash/nor/nor.h>

/*!
 *  @brief  PHY configuration data
 *
 *  PHY setting for Read Delay, txDLL, and rxDLL values to describe a "point" in the PHY shmoo
 */
typedef struct NOR_PhyConfig_s
{
    uint32_t txDLL;
    uint32_t rxDLL;
    uint32_t rdDelay;
} NOR_PhyConfig;

#define NOR_SPI_PHY_TXDLL_LOW_WINDOW_START     (28U)
#define NOR_SPI_PHY_TXDLL_LOW_WINDOW_END       (48U)

#define NOR_SPI_PHY_TXDLL_HIGH_WINDOW_START    (96U)
#define NOR_SPI_PHY_TXDLL_HIGH_WINDOW_END      (60U)

#define NOR_SPI_PHY_VTM_TARGET      (700)

#define MIN(A,B)                    ((A<B)?A:B)
#define MAX(A,B)                    ((A>B)?A:B)
/* 
 * Note: All the ranges below are inclusive,
 * users need to be aware of it while adjusting below
 * ranges to minimize tuning time
 */
#define PHY_DDR_SEARCH_STEP         (4U)
#define RX_DLL_LOW_SEARCH_START     (0U)
#define RX_DLL_LOW_SEARCH_END       (40U)
#define RX_DLL_HIGH_SEARCH_START    (127U)
#define RX_DLL_HIGH_SEARCH_END      (24U)
#define TX_DLL_LOW_SEARCH_START     (0U)
#define TX_DLL_LOW_SEARCH_END       (64U)
#define TX_DLL_HIGH_SEARCH_START    (127U)
#define TX_DLL_HIGH_SEARCH_END      (78U)
#define TX_DLL_SEC_SEARCH_OFFSET    (8U)
#define PHY_DDR_TUNE_RD_DELAY_START (1U)
#define PHY_DDR_TUNE_RD_DELAY_END   (4U)
#define PHY_DDR_TUNE_DLL_MAX        (128U)

#define PHY_SDR_DLL_START         (0U)
#define PHY_SDR_DLL_END           (127U)
#define PHY_SDR_DLL_MAX           (PHY_SDR_DLL_END + 1)
#define PHY_SDR_TX_DLL            (127U)
#define PHY_SDR_RD_DELAY_START    (1U)
#define PHY_SDR_RD_DELAY_END      (4U)

extern NOR_STATUS Nor_spiPhyTune(OSPI_Handle handle, uint32_t offset);
extern void Nor_spiPhyTuneReset(bool ddrMode);
#endif /* NOR_SPI_PHY_TUNE_H_ */
