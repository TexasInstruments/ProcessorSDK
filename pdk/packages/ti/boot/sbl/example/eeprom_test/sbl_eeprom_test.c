/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 */
/**
 *  \file sbl_eeprom_dump_test.c
 *
 *  \brief File to check whether EEPROM data which is dumped to DDR is proper or not
 */

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/csl/soc.h>
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#if defined(SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_info_ddr.h>
#include <ti/board/src/j721e_evm/include/board_cfg.h>
#elif defined(SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_info_ddr.h>
#include <ti/board/src/j7200_evm/include/board_cfg.h>
#elif defined(SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_info_ddr.h>
#include <ti/board/src/j721s2_evm/include/board_cfg.h>
#elif defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_info_ddr.h>
#include <ti/board/src/j784s4_evm/include/board_cfg.h>
#elif defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_info_ddr.h>
#include <ti/board/src/j742s2_evm/include/board_cfg.h>
#endif

#define DDR_ADDRESS_TO_DUMP_EEPROM_CONTENT 0x90000000UL
#define MAGIC_NUMBER_OFFSET 0X0
#define BOARD_NAME_OFFSET 0XA

int main()
{
    Board_initCfg           boardCfg;
    uint32_t* pBoardDatamagicNumber = (uint32_t*)(DDR_ADDRESS_TO_DUMP_EEPROM_CONTENT + MAGIC_NUMBER_OFFSET);
    uint8_t* pBoardDataBoardName = (uint8_t*)(DDR_ADDRESS_TO_DUMP_EEPROM_CONTENT + BOARD_NAME_OFFSET);

    boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    UART_printf("Copying EEPROM data ... to 0x%x \n", DDR_ADDRESS_TO_DUMP_EEPROM_CONTENT);
    UART_printf("Copying EEPROM data ...done \n");

    if ((*pBoardDatamagicNumber == BOARD_EEPROM_MAGIC_NUMBER) && ((*pBoardDataBoardName == 'J' && *(pBoardDataBoardName+1) == '7')))
    {
        UART_printf("All tests have passed \n");
    }
    else
    {
        UART_printf("EEPROM data check test has failed \n");
    }

    return 0;
}
