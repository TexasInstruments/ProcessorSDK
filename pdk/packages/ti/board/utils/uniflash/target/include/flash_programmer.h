/*
 * Copyright (C) 2019-2022 Texas Instruments Incorporated - http://www.ti.com/
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

#ifndef _FLASH_PROGRAMMER_H_
#define _FLASH_PROGRAMMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#define UFP_BAUDRATE_115200         (115200U)
#define UFP_BAUDRATE_230400         (230400U)
#define UFP_BAUDRATE_460800         (460800U)
#define UFP_BAUDRATE_500000         (500000U)
#define UFP_BAUDRATE_576000         (576000U)
#define UFP_BAUDRATE_921600         (921600U)
#define UFP_BAUDRATE_1000000        (1000000U)
#define UFP_BAUDRATE_1152000        (1152000U)
#define UFP_BAUDRATE_1500000        (1500000U)
#define UFP_BAUDRATE_2000000        (2000000U)
#define UFP_BAUDRATE_2500000        (2500000U)
#define UFP_BAUDRATE_3000000        (3000000U)
#define UFP_BAUDRATE_3500000        (3500000U)
#define UFP_BAUDRATE_4000000        (4000000U)
#define UFP_BAUDRATE_6000000        (6000000U)

#define UFP_BAUDRATE_115200_ID      (0x0U)
#define UFP_BAUDRATE_230400_ID      (0x1U)
#define UFP_BAUDRATE_460800_ID      (0x2U)
#define UFP_BAUDRATE_500000_ID      (0x3U)
#define UFP_BAUDRATE_576000_ID      (0x4U)
#define UFP_BAUDRATE_921600_ID      (0x5U)
#define UFP_BAUDRATE_1000000_ID     (0x6U)
#define UFP_BAUDRATE_1152000_ID     (0x7U)
#define UFP_BAUDRATE_1500000_ID     (0x8U)
#define UFP_BAUDRATE_2000000_ID     (0x9U)
#define UFP_BAUDRATE_2500000_ID     (0xAU)
#define UFP_BAUDRATE_3000000_ID     (0xBU)
#define UFP_BAUDRATE_3500000_ID     (0xCU)
#define UFP_BAUDRATE_4000000_ID     (0xDU)
#define UFP_BAUDRATE_6000000_ID     (0xEU)

#if defined(iceAMIC110) || defined(icev2AM335x)
#define SPI_FLASH
#define MAX_BAUDRATE_SUPPORTED			(UFP_BAUDRATE_230400_ID)
#endif

#if defined(evmAM335x)
#define SPI_FLASH
#define MAX_BAUDRATE_SUPPORTED			(UFP_BAUDRATE_115200_ID)
#endif 

#if defined(idkAM437x)
#define QSPI_FLASH
#define MAX_BAUDRATE_SUPPORTED			(UFP_BAUDRATE_230400_ID)
#endif 

#if defined(idkAM571x) || defined(idkAM572x) || defined(idkAM574x) || defined(iceK2G)
#define QSPI_FLASH
#define MAX_BAUDRATE_SUPPORTED			(UFP_BAUDRATE_3000000_ID)
#endif

#if defined(evmK2G)
#define QSPI_FLASH
#define MAX_BAUDRATE_SUPPORTED			(UFP_BAUDRATE_115200_ID)
#endif

#if defined(j721e_evm)
#define MAX_BAUDRATE_SUPPORTED          (UFP_BAUDRATE_6000000_ID)
#define MAX_BAUDRATE_SUPPORTED_LINUX    (UFP_BAUDRATE_3000000_ID)

#define HPF_FLASH
#define OSPI_FLASH
#define QSPI_FLASH
#define EMMC_FLASH
#endif

#if defined(j7200_evm)
#define MAX_BAUDRATE_SUPPORTED          (UFP_BAUDRATE_6000000_ID)
#define MAX_BAUDRATE_SUPPORTED_LINUX    (UFP_BAUDRATE_3000000_ID)

#define HPF_FLASH
#define OSPI_FLASH
#define EMMC_FLASH
#endif

#if (defined(j721s2_evm) || defined(j784s4_evm) || defined(j742s2_evm))
#define MAX_BAUDRATE_SUPPORTED          (UFP_BAUDRATE_6000000_ID)
#define MAX_BAUDRATE_SUPPORTED_LINUX    (UFP_BAUDRATE_3000000_ID)

#define OSPI_FLASH
#define OSPI_NAND_FLASH
#define EMMC_FLASH
#define QSPI_FLASH
#endif

#if defined(am65xx_evm) || defined(am65xx_idk)
#define MAX_BAUDRATE_SUPPORTED          (UFP_BAUDRATE_6000000_ID)
#define MAX_BAUDRATE_SUPPORTED_LINUX    (UFP_BAUDRATE_3000000_ID)

#define OSPI_FLASH
#define EMMC_FLASH
#endif

#if defined(am64x_evm)
#define MAX_BAUDRATE_SUPPORTED          (UFP_BAUDRATE_1500000_ID)  /* 1500000 */
#define MAX_BAUDRATE_SUPPORTED_LINUX    (UFP_BAUDRATE_1500000_ID)  /* 1500000 */

#define OSPI_FLASH
#define EMMC_FLASH
#endif

#if (defined(tpr12_evm) || defined(awr294x_evm))
#define QSPI_FLASH
#define MAX_BAUDRATE_SUPPORTED			(0x4U)
#endif

/* Configurations for UART HW flow control */
#if defined(am64x_evm)
#define UFP_FLOW_CTRL_STS               (0x1U)

#define MAX_BAUDRATE_SUPPORTED_FC       (UFP_BAUDRATE_3000000_ID)  /* 3000000 */
#define MAX_BAUDRATE_SUPPORTED_LINUX_FC (UFP_BAUDRATE_3000000_ID)  /* 3000000 */

#else
#define UFP_FLOW_CTRL_STS               (0x0U)

#if (defined(j7200_evm) || defined(j721e_evm) || defined(am65xx_evm) || defined(am65xx_idk) || defined(j721s2_evm) || defined(j784s4_evm) || defined(j742s2_evm))
#define MAX_BAUDRATE_SUPPORTED_FC       (MAX_BAUDRATE_SUPPORTED)
#define MAX_BAUDRATE_SUPPORTED_LINUX_FC (MAX_BAUDRATE_SUPPORTED_LINUX)
#else
#define MAX_BAUDRATE_SUPPORTED_FC       (MAX_BAUDRATE_SUPPORTED)
#endif

#endif   /* #if defined(am64x_evm) */

#define FLASH_DEVICE_MAX          (6U)

#define UFP_FLASH_DEV_NAND        (0x0U)
#define UFP_FLASH_DEV_SPI         (0x1U)
#define UFP_FLASH_DEV_QSPI        (0x2U)
#define UFP_FLASH_DEV_OSPI        (0x3U)
#define UFP_FLASH_DEV_EMMC        (0x4U)
#define UFP_FLASH_DEV_HPF         (0x5U)

#define UFP_IMAGE_FP              (0x0U)
#define UFP_IMAGE_MLO             (0x1U)
#define UFP_IMAGE_UBOOT           (0x2U)
#define UFP_IMAGE_UIMAGE          (0x3U)
#define UFP_IMAGE_SYSFW           (0x4U)
#define UFP_IMAGE_CUST            (0x5U)

#define UFP_SYSFW_SIZE            (0x42000U)

#define UFP_HEADER_PKT_SIZE        (14U)

#define UFP_BAUDRATE_LIST_COUNT    (15U)

#define UFP_CMD_PROGRAM             (0x50U)
#define UFP_CMD_PROGRAM_XIP         (0x51U)
#define UFP_CMD_ERASE               (0x45U)
#define UFP_CMD_GET_MAX_BAUDRATE    (0x47U)
#define UFP_CMD_SET_BAUDRATE        (0x53U)
#define UFP_CMD_GET_FLOWCTRL        (0x48U)
#define UFP_CMD_SET_FLOWCTRL        (0x49U)

/*
 *  @brief      A function pointer to initialize flash device.
 */
typedef int8_t  (*initFxn)  (void);

/*
 *  @brief      A function pointer to erase the data in flash device.
 */
typedef int8_t  (*eraseFxn) (uint32_t offset, uint32_t length);

/*
 *  @brief      A function pointer to flash the image to flash device.
 */
typedef int8_t  (*flashFxn) (uint8_t *flashAddr, uint8_t *checkAddr,
                             uint32_t offset, uint32_t size);

/*
 *  @brief      A function pointer to de-initialize flash device.
 */
typedef int8_t  (*CloseFxn) (void);

/*
 *  @brief      The definition of Flash Programmer function table that contains
 *              the required set of functions to flash/erase the image
 *              on specified flash device.
 */
typedef struct UFP_fxnTable_t {
    /* Initialize Flash Device */
    initFxn       UFP_flashInit;

    /* Erase  the Content of Flash Device */
    eraseFxn      UFP_flashErase;

    /* Copy Received image on the Flash Device */
    flashFxn      UFP_flashProgram;

    /* Close the Initialized Flash Device */
    CloseFxn      UFP_flashClose;

} UFP_fxnTable;

/*
 *  @brief      UFP_flashConfig structure contains function pointer
 *              for all supported flash device.
 */
typedef struct UFP_flashConfig_t {

    UFP_fxnTable const *UPF_fxnTablePtr;

}UFP_flashConfig;

/**
 *  \brief   Returns maximum baudrate configured by host
 *
 */
uint32_t UFP_getMaxBaudrate(void);

/**
 *  \brief   Initializes UART module
 *
 */
void UFP_uartInit(void);

/**
 *  \brief   Configures the UART instance with given baudrate
 *
 *  \return
 *				 0	- in case of success
 *				-1	- in case of failure.
 *
 */
int8_t UFP_uartConfig(uint32_t baudrate);

/**
 *  \brief   Closes UART module
 *
 */
void UFP_uartClose(void);

#ifdef __cplusplus
}
#endif

#endif /* _FLASH_PROGRAMMER_H_ */
