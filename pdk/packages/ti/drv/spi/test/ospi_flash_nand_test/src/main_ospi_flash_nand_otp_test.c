/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   main_ospi_flash_nand_otp_test.c
 *
 *  \brief  Test application main file. This application will verify
 *          OSPI NAND flash OTP configuration functions.
 *
 */
#include <stdint.h>
#include <stdio.h>

#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/src/SPI_osal.h>
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/drv/spi/test/src/SPI_log.h>

#include <ti/board/board.h>
#include <ti/board/board_cfg.h>
#include <ti/board/src/flash/include/board_flash.h>
#include <ti/board/src/flash/nor/ospi/nor_spi_patterns.h>
#include <ti/board/src/flash/nand/ospi/nand_ospi.h>

#if defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/csl/csl_gpio.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#endif

#ifdef SPI_DMA_ENABLE
#include <ti/osal/CacheP.h>
#endif

#include <ti/csl/cslr_fss.h>
#include <ti/csl/csl_rat.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/sciclient/sciclient.h>
#ifdef SPI_DMA_ENABLE
#include <ti/drv/udma/udma.h>
#endif
#include <ti/board/src/flash/nand/device/w35n01jwtbag.h>

#if defined(SOC_J721S2)
#include <ti/csl/soc/j721s2/src/cslr_soc_baseaddress.h>
#include <ti/csl/soc/j721s2/src/cslr_mcu_ctrl_mmr.h>
#include <ti/csl/soc/j721s2/src/cslr_mcu_pll_mmr.h>
#include <ti/csl/soc/j721s2/src/cslr_wkup_ctrl_mmr.h>
#endif

#if defined(SOC_J784S4)
#include <ti/csl/soc/j784s4/src/cslr_soc_baseaddress.h>
#include <ti/csl/soc/j784s4/src/cslr_mcu_ctrl_mmr.h>
#include <ti/csl/soc/j784s4/src/cslr_mcu_pll_mmr.h>
#include <ti/csl/soc/j784s4/src/cslr_wkup_ctrl_mmr.h>
#endif

#if defined(SOC_J742S2) /* Use j784s4 clsr files, since cslr files are same for both j784s4 and j742s2*/
#include <ti/csl/soc/j784s4/src/cslr_soc_baseaddress.h>
#include <ti/csl/soc/j784s4/src/cslr_mcu_ctrl_mmr.h>
#include <ti/csl/soc/j784s4/src/cslr_mcu_pll_mmr.h>
#include <ti/csl/soc/j784s4/src/cslr_wkup_ctrl_mmr.h>
#endif

static uint32_t gTuneEnable = UFALSE;

extern uint8_t nor_attack_vector[NOR_ATTACK_VECTOR_SIZE];
uint8_t txBuf[NOR_ATTACK_VECTOR_SIZE];
uint8_t rxBuf[NOR_ATTACK_VECTOR_SIZE];

/*
 *  ======== CompareData ========
 */
bool VerifyData(uint8_t *expData,
                uint8_t *rxData,
                uint32_t length)
{
    uint32_t idx = 0;
    uint32_t match = UTRUE;
    bool ret = BFALSE;

    for(idx = 0; ((idx < length) && (UFALSE != match)); idx++)
    {
        if(*expData != *rxData)
        {
            match = UFALSE;
            UART_printf("Data mismatch at idx %d\n", idx);
            UART_printf("exp data:%x, rxData:%x\n", *expData, *rxData);
        }
        expData++;
        rxData++;
    }

    if(UTRUE == match)
    {
        ret = BTRUE;
    }

    return ret;
}

/**
 * \brief   This function executes OSPI NAND OTP test.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t ospi_nand_otp_test(void)
{
    Board_flashHandle boardHandle;
    Board_FlashInfo *flashInfo;
    int8_t ret;
    OSPI_v0_HwAttrs ospi_cfg;

    /* Get the default OSPI init configurations */
    OSPI_socGetInitCfg(BOARD_OSPI_DOMAIN, BOARD_OSPI_NAND_INSTANCE, &ospi_cfg);

    gTuneEnable = UFALSE;

    /* Modify the default OSPI configurations if necessary */
    ospi_cfg.intrEnable = BFALSE;

#if defined(UDMA_ENABLE)
    ospi_cfg.dmaEnable  = BTRUE;
    ospiUdmaInit(&ospi_cfg);
#else
    ospi_cfg.dmaEnable  = BFALSE;
#endif
#if defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    ospi_cfg.phyEnable  = BFALSE;
    ospi_cfg.dtrEnable  = BTRUE;
    ospi_cfg.dacEnable  = BFALSE;
#endif

    /* Set the default OSPI init configurations */
    OSPI_socSetInitCfg(BOARD_OSPI_DOMAIN, BOARD_OSPI_NAND_INSTANCE, &ospi_cfg);

    boardHandle = Board_flashOpen(BOARD_FLASH_ID_W35N01JWTBAG,
                                  BOARD_OSPI_NAND_INSTANCE, (void *)(&gTuneEnable));
    if (!boardHandle)
    {
        UART_printf("Board_flashOpen Failed\n");
        return -1;
    }
    else
    {
        flashInfo = (Board_FlashInfo *)boardHandle;
        UART_printf("\nOSPI NAND Device ID: 0x%x, Manufacturer ID: 0x%x \n",
                           flashInfo->device_id, flashInfo->manufacturer_id);
    }

    /* Enable OTP access */
    ret = Board_flashControl(boardHandle, BOARD_FLASH_CTRL_ENABLE_OTP_ACCESS, NULL);
    if (ret != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashControl failed\n");
        return -1;
    }

    memcpy((void *)&txBuf[0], (void *)&nor_attack_vector[0], NOR_ATTACK_VECTOR_SIZE);

    /* Read the OTP area */
    ret = Board_flashRead(boardHandle, 0,
                             &rxBuf[0], NOR_ATTACK_VECTOR_SIZE, NULL);
    if (ret != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashRead failed\n");
        return -1;
    }

    if (VerifyData(&txBuf[0], &rxBuf[0], NOR_ATTACK_VECTOR_SIZE) == BFALSE)
    {
        UART_printf("\nData Mismatch\n");
        UART_printf("\nOTP Data Might not be Written\n");
        UART_printf("\nWriting OTP Data\n");

        /* Write OTP area */
        ret = Board_flashWrite(boardHandle, 0,
                                  &txBuf[0], NOR_ATTACK_VECTOR_SIZE, NULL);
        if (ret != BOARD_FLASH_EOK)
        {
            UART_printf("\nBoard_flashWrite failed\n");
            return -1;
        }

        /* Read the OTP area */
        ret = Board_flashRead(boardHandle, 0,
                                 &rxBuf[0], NOR_ATTACK_VECTOR_SIZE, NULL);
        if (ret != BOARD_FLASH_EOK)
        {
            UART_printf("\nBoard_flashRead failed\n");
            return -1;
        }

        if (VerifyData(&txBuf[0], &rxBuf[0], NOR_ATTACK_VECTOR_SIZE) == BFALSE)
        {
            UART_printf("\nOTP Access Failed!\n");
            return -1;
        }

        /* Lock OTP area */
        ret = Board_flashControl(boardHandle, NAND_FLASH_CTRL_LOCK_OTP, NULL);
        if (ret != BOARD_FLASH_EOK)
        {
            UART_printf("\nBoard_flashControl failed\n");
            return -1;
        }
    }

    /* Disable OTP area */
    ret = Board_flashControl(boardHandle, NAND_FLASH_CTRL_DISABLE_OTP_ACCESS, NULL);
    if (ret != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashControl failed\n");
        return -1;
    }

    return 0;
}

/*
 *  ======== main ========
 */
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_MODULE_CLOCK  |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ospi_nand_otp_test();

    return (0);
}

