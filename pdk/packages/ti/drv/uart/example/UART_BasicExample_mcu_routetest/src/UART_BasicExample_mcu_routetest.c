
/*
 * Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   UART_BasicExample_mcu_routetest.c
 *
 *  \brief  UART MCU Route Test Example - Tests MAIN domain UART interrupt routing to MCU R5
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================= */

#include <stdio.h>
#include <string.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/src/j721e_evm/include/board_utils.h>
#include <ti/board/src/j721e_evm/include/board_control.h>
#include <ti/board/src/j721e_evm/include/board_cfg.h>

/* ========================================================================== */
/*                             Macros                                         */
/* ========================================================================== */
#define UART_INSTANCE             (4U)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                            Functions Declarations                          */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern Board_initParams_t gBoardInitParams;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    Board_initCfg boardCfg;
    Board_IoExpCfg_t ioExpCfg;
    UART_HwAttrs uart_cfg;
    int retVal = CSL_PASS;
    UART_Params uartParams;
    UART_Handle uart_handle = NULL;
    char txBuf[] = "TEST PASSED FROM UART4!\n";

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK;
    retVal = Board_init(boardCfg);
    if(retVal == CSL_PASS)
    {
        UART_init();
        UART_Params_init(&uartParams);
        /* Disable the UART interrupt */  
        UART_socGetInitCfg(UART_INSTANCE, &uart_cfg);

        uart_cfg.enableInterrupt = true;
        UART_socSetInitCfg(UART_INSTANCE, &uart_cfg);

        /* MAIN UART4 instance is connected through mux which is controlled by
            IO expander. Need to configure the mux to use UART4 instance */

        ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
        ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
        ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
        ioExpCfg.enableIntr  = false;
        ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
        ioExpCfg.portNum     = PORTNUM_1;
        ioExpCfg.pinNum      = PIN_NUM_1;
        ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;

        retVal = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);
    }
    if(retVal == CSL_PASS)
    {
        ioExpCfg.pinNum      = PIN_NUM_2;
        ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

        retVal = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);
    }
    if(retVal == CSL_PASS)
    {
        uart_handle = UART_open(UART_INSTANCE, &uartParams);
        if(uart_handle == NULL)
        {
            printf("\nError opening UART driver\n");
            retVal = CSL_EFAIL;
        }
    }
    if(retVal == CSL_PASS)
    {
        retVal = UART_write(uart_handle, txBuf, sizeof(txBuf));
    }
    if(uart_handle != NULL)
    {
        UART_close(uart_handle);
    }
    if(!retVal)
    {
        printf("UART4 Test FAILED!\n");
        retVal = CSL_EFAIL;
    }

    return retVal;
}