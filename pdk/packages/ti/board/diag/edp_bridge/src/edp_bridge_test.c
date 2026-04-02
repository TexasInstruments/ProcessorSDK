/******************************************************************************
 * Copyright (c) 2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   edp_bridge_test.c
 *
 *  \brief  DSI to eDP bridge test file
 *
 *  Targeted Functionality: Verification of basic functionality of DSI to eDP
 *  bridge module.
 *
 *  Operation: This test configures the DSI to eDP bridge in color bar mode to 
 *  verify the I2C access and DP output of the module.
 *
 *  Test Setup: Connect DSI to DP port of the board to DP monitor
 *
 *  Supported SoCs: J721S2, J784S4
 *
 *  Supported Platforms: j721s2_evm, j784s4_evm
 *
 */

#include "edp_bridge_test.h"

#define BOARD_I2C_DSI2DP_DEVICE_ADDR (0x2CU)

uint8_t gDpBridgeConfig[][2] =
{
    {0xFF, 0x07},
    {0x16, 0x01},
    {0xFF, 0x00},
    {0x0A, 0x02},
    {0x10, 0x26},
    {0x12, 0x42},
    {0x13, 0x42},
    {0x94, 0x80},
    {0x0D, 0x01},
    {0x5A, 0x04},
    {0x93, 0x20},
    {0x96, 0x0A},
    {0x20, 0x80},
    {0x21, 0x07},
    {0x22, 0x00},
    {0x23, 0x00},
    {0x24, 0x38},
    {0x25, 0x04},
    {0x2C, 0x2C},
    {0x2D, 0x00},
    {0x30, 0x05},
    {0x31, 0x00},
    {0x34, 0x94},
    {0x36, 0x24},
    {0x38, 0x58},
    {0x3A, 0x04},
    {0x5B, 0x00},
    {0x3C, 0x10},
    {0x5A, 0x0C}
};

static int32_t BoardDiag_edp_bridge_test(void)
{
    Board_STATUS retVal;
    uint32_t index = 0;
    I2C_Params i2cParams;
    I2C_Handle i2cHandle = NULL;
    uint8_t readVal = 0;
    uint8_t input;

	UART_printf("\n*************************************************\n"); 
	UART_printf("*              DSI to eDP Bridge Test           *\n");
	UART_printf("*************************************************\n");    

    /* Initializes the I2C */
    Board_control(BOARD_CTRL_CMD_ENABLE_DSI2DP_BRIDGE, NULL);
    BOARD_delay(10000);

    I2C_init();

    /* Initializes the I2C Parameters */
    I2C_Params_init(&i2cParams);
    i2cHandle = I2C_open(BOARD_I2C_DSI2DP_DEVICE_INSTANCE, &i2cParams);
    if(i2cHandle == NULL)
    {
        UART_printf("\nI2C Open Failed!\n");
        return -1;
    }

    UART_printf("\nConfiguring DSI to DP Bridge for Colour Bar...\n");
    for(index = 0; index < sizeof(gDpBridgeConfig)/sizeof(gDpBridgeConfig[0]); index++)
    {
        retVal = Board_i2c8BitRegWr(i2cHandle,
                           BOARD_I2C_DSI2DP_DEVICE_ADDR,
                           gDpBridgeConfig[index][0] & 0xFF,
                           &(gDpBridgeConfig[index][1]),
                           1,
                           BOARD_I2C_TRANSACTION_TIMEOUT);
        if(retVal != BOARD_SOK)
        {
            UART_printf("\nDSI to DP Bridge Register Write Failed!\n");
        }

        BOARD_delay(100000);

        if(gDpBridgeConfig[index][0] == BOARD_DIAG_DP_BRIDGE_TRAINING_REG_ADDR)
        {
            UART_printf("\nChecking Link Training Status\n");
            /* Wait some time for training to complete */
            BOARD_delay(1000000);

            Board_i2c8BitRegRd(i2cHandle,
                               BOARD_I2C_DSI2DP_DEVICE_ADDR,
                               BOARD_DIAG_DP_BRIDGE_TRAINING_REG_ADDR,
                               &readVal,
                               1,
                               BOARD_I2C_TRANSACTION_TIMEOUT);
            if(readVal == 0x00)
            {
                UART_printf("Link Training Failed!!\n");
                return -1;
            }
            else
            {
                UART_printf("Link Training Completed!\n");
            }
        }
    }

    I2C_close(i2cHandle);

    UART_printf("DSI to DP Bridge Configuration Successful!\n");
    UART_printf("\nPress 'y' if colour bar is displayed on the monitor, or any other character to indicate failure:");
    UART_scanFmt("%c", &input);
    if ((input == 'y') || (input == 'Y'))
    {
        UART_printf("\n\nDSI to DP Bridge Test Passed!\n");
        return 0;
    }
    else
    {
        UART_printf("\n\nDSI to DP Bridge Test Failed!!\n");
        return -1;
    }
}

/**
 *  \brief   DSI to eDP bridge Diagnostic test main function
 *
 *  \return  int - Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 *
 */
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

	return BoardDiag_edp_bridge_test();
}
