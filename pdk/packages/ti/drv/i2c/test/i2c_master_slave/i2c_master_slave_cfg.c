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
 *  \file   i2c_master_slave_cfg.c
 *
 *  \brief  File containing definitions for common configurations used across 
 *          both master and slave applications.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include "i2c_master_slave_cfg.h"

#if defined (SOC_J721S2)
#include <ti/board/src/j721s2_evm/J721S2_pinmux.h>
#include <ti/board/src/j721s2_evm/include/board_cfg.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                     External Function Declarations                         */
/* ========================================================================== */

extern Board_STATUS Board_pinmuxUpdate (pinmuxBoardCfg_t *pinmuxData,
                                 uint32_t domain);

/* ========================================================================== */
/*                          Structure Declarations                              */
/* ========================================================================== */

#if defined (SOC_J721S2)
    static pinmuxPerCfg_t gMcu_i3c0PinCfg[] =
    {
        /* MyMCU_I3C1 -> MCU_I2C1_SCL -> F24 */
        {
            PIN_WKUP_GPIO0_8, PIN_MODE(0) | \
            ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
        },
        /* MyMCU_I3C1 -> MCU_I2C1_SDA -> H26 */
        {
            PIN_WKUP_GPIO0_9, PIN_MODE(0) | \
            ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
        },
        {PINMUX_END}
    };
    
    static pinmuxModuleCfg_t gMcu_i3cPinCfg[] =
    {
        {0, TRUE, gMcu_i3c0PinCfg},
        {PINMUX_END}
    };
    
    pinmuxBoardCfg_t gMcuI2cPinmuxData[] =
    {
        {0, gMcu_i3cPinCfg},
        {PINMUX_END}
    };
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

bool I2cApp_boardInit(void)
{
    bool retVal = BTRUE;
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (BOARD_SOK != boardStatus)
    {
        retVal = BFALSE;
    }

    /* Pinmux for MCU_I2C1 */
    Board_pinmuxUpdate(gMcuI2cPinmuxData, BOARD_SOC_DOMAIN_WKUP);
    
    I2C_init();

    UART_printf("\nI2C : Using Instance %d", I2C_INSTANCE_ID);
    return retVal;
}

void I2cApp_initConfig(uint32_t instance)
{
    I2C_HwAttrs   i2c_cfg;

    /* Get the default I2C init configurations */
    I2C_socGetInitCfg(instance, &i2c_cfg);

    i2c_cfg.enableIntr = BTRUE;

    /* Set the SPI I2C configurations */
    I2C_socSetInitCfg(instance, &i2c_cfg);
}

bool I2cApp_compareData(char *expData, char *rxData, uint32_t length)
{
    uint32_t idx = 0U;
    uint32_t match = UTRUE;
    bool retVal = BFALSE;

    for(idx = 0U; ((idx < length) && (UFALSE != match)); idx++)
    {
        if(*expData != *rxData) match = UFALSE;
        expData++;
        rxData++;
    }

    if(UTRUE == match) retVal = BTRUE;

    return retVal;
}
