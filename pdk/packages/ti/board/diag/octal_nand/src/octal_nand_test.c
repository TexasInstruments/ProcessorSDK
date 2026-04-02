/******************************************************************************
 * Copyright (c) 2021-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   octal_nand_test.c
 *
 *  \brief  OSPI NAND diagnostic test main file
 *
 *  Targeted Functionality: This file implements OSPI NAND diagnostic test.
 *
 *  Operation: Verification of OSPI by accessing the device connected to it.
 *
 *  Supported SoCs : J721S2, J784S4
 *
 *  Supported Platforms: j721s2_evm, j784s4_evm.
 *
 */

#include "octal_nand_test.h"

uint8_t txBuf[BOARD_DIAG_OCTAL_NAND_TEST_BYTES];
uint8_t rxBuf[BOARD_DIAG_OCTAL_NAND_TEST_BYTES];

/**
 * \brief   This function writes data, reads back the written data and compares.
 *
 * \param   handle      [IN]    Board Flash Handle
 * \param   blockNum    [IN]    Block number to verify
 * \param   numOfBytes  [IN]    Number of bytes to write.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_octalNandReadWriteTest(Board_flashHandle handle,
                                               uint32_t blockNum,
                                               uint32_t numOfBytes)
{
    Board_flash_STATUS retVal;
    uint32_t failIndex, offset = 0;

    /* Erase block, to which data has to be written */
    retVal = Board_flashEraseBlk(handle, blockNum);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashEraseBlk failed\n");
        return -1;
    }

    BoardDiag_genPattern(&txBuf[0], numOfBytes,
                         BOARD_DIAG_TEST_PATTERN_RANDOM);

    /* Write buffer to Octal NAND */
    retVal = Board_flashWrite(handle, offset,
                              &txBuf[0], numOfBytes, NULL);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashWrite failed\n");
        return -1;
    }

    /* Read the NAND memory */
    retVal = Board_flashRead(handle, offset,
                             &rxBuf[0], numOfBytes, NULL);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashRead failed\n");
        return -1;
    }

    if (BoardDiag_memCompare(&txBuf[0], &rxBuf[0], numOfBytes, &failIndex)
                                                                    == BFALSE)
    {
        UART_printf("\nData mismatch at offset = 0x%x\n", failIndex);
        return -1;
    }
    return 0;
}

/**
 * \brief   This function executes OSPI NAND functionality test.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_octalNandFunctionalTest(void)
{
    Board_flashHandle boardHandle;
    Board_FlashInfo *flashInfo;
    int8_t ret;

    boardHandle = Board_flashOpen(BOARD_FLASH_ID_W35N01JWTBAG,
                                  BOARD_OCTAL_NAND_INSTANCE, NULL);
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

    UART_printf("\n\nVerifying NANG Page Access...");
    ret = BoardDiag_octalNandReadWriteTest(boardHandle,
                                           BOARD_DIAG_OCTAL_NAND_FIRST_VERIFY_ADDR,
                                           BOARD_DIAG_OCTAL_NAND_TEST_BYTES);
    if (ret != 0)
    {
        UART_printf("Verifying first 512 bytes failed\n");
        Board_flashClose(boardHandle);
        return -1;
    }
    UART_printf("\nVerification of NAND Page Acceess Successful!");

    return 0;
}

/**
 * \brief   The function performs the Octal NAND test.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_octalNandTest(void)
{
    int8_t ret;

    ret = BoardDiag_octalNandFunctionalTest();
    if (ret)
    {
        UART_printf("\nOctal NAND Test Failed!!\n");
    }
    else
    {
        UART_printf("\nOctal NAND Test Passed!");
    }

    return ret;
}

static void BoardDiag_ospiNandFlashMux(void)
{
    GPIO_v0_HwAttrs gpioCfg;
    GPIO_socGetInitCfg(0, &gpioCfg);
    gpioCfg.baseAddr = CSL_WKUP_GPIO0_BASE;
    GPIO_socSetInitCfg(0, &gpioCfg);

    GPIO_init();
    GPIO_write(BOARD_DIAG_OSPI_NAND_BUS_SEL_PIN, 1);
}

/**
 *  \brief   Octal NAND Diagnostic test main function
 *
 *  \return  int - Octal NAND Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 *
 */
#ifndef SPI_BOOT_FRAMEWORK
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int ret = 0;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO | 
               BOARD_INIT_PINMUX_CONFIG;
#endif

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    UART_printf("\n**********************************************\n");
    UART_printf  ("*                OSPI NAND Test              *\n");
    UART_printf  ("**********************************************\n");

    /* Enable Mux for routing OSPI lines to Nand Flash */
    BoardDiag_ospiNandFlashMux();

    ret = BoardDiag_octalNandTest();
    return ret;
}
#endif
