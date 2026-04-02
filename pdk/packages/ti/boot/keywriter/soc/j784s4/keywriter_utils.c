/**
 *  \file   keywriter_utils.c
 *
 *  \brief  This file contains functions specific to j784s4 keywriter support
 *
 */

/*
 * Copyright (C) 2022-2023 Texas Instruments Incorporated - http://www.ti.com/
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

#include "keywriter_utils.h"

#include <string.h>

#include <ti/csl/soc.h>

#include <ti/board/board.h>
#include <ti/board/src/j784s4_evm/include/board_utils.h>

#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>

GPIO_PinConfig gpioPinConfigs[] = {
    EN_EFUSE_VPP_CTRL | GPIO_CFG_OUTPUT,
    SK_EN_EFUSE_VPP_CTRL | GPIO_CFG_OUTPUT
};

/* GPIO Driver call back functions */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,
    NULL
};

/* GPIO Driver configuration structure */
GPIO_v0_Config GPIO_v0_config = {
    gpioPinConfigs,
    gpioCallbackFunctions,
    sizeof(gpioPinConfigs) / sizeof(GPIO_PinConfig),
    sizeof(gpioCallbackFunctions) / sizeof(GPIO_CallbackFxn),
    0,
};

void OTP_VppEn(void)
{
    Board_initCfg      boardCfg;
    Board_I2cInitCfg_t i2cCfg;
    Board_IDInfo_v2    info;
    Board_STATUS       status;
    uint32_t           gpioIndex = UFALSE;

    UART_printf("OTP_VppEn\n");

    /* Initialize pinmux */
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK  |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    i2cCfg.i2cInst    = BOARD_I2C_EEPROM_INSTANCE;
    i2cCfg.socDomain  = BOARD_SOC_DOMAIN_WKUP;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    /* Check if the board is SK */
    status = Board_getIDInfo_v2(&info, KEYWRITER_SK_EEPROM_SLAVE_ADDR);
    if(BOARD_SOK == status)
    {
        if(!(strncmp(info.boardInfo.boardName,
                     "AM69-SK",
                     BOARD_BOARD_NAME_LEN)))
        {
            UART_printf("AM69 SK Detected!!\n");
            gpioIndex = UTRUE;
        }
    }

    /* Configure GPIO base address */
    GPIO_v0_HwAttrs gpioCfg;
    GPIO_socGetInitCfg(gpioIndex, &gpioCfg);
    gpioCfg.baseAddr = CSL_WKUP_GPIO0_BASE;
    GPIO_socSetInitCfg(gpioIndex, &gpioCfg);

    /* Set GPIO to HIGH */
    GPIO_init();
    GPIO_write(gpioIndex, 1);

    UART_printf("WKUP_GPIO0_VPP_CTRL output high\n");
}
