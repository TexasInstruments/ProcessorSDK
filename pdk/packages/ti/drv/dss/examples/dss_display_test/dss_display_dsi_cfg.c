/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2022
 *  All rights reserved.
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
 */

/**
 *  \file dss_display_test.c
 *
 *  \brief DSS sample application that displays two ARGB32 buffers.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/board/src/devices/board_devices.h>
#include <ti/board/board.h>
#include <ti/board/board_cfg.h>
#if defined (SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_i2c_io_exp.h>
#endif
#if defined (SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_control.h>
#endif
#if defined (SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_control.h>
#endif
#if defined (SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_control.h>
#endif
#include <dss_display_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DISP_APP_I2C_TIMEOUT 1000U
#define DSS_BRIDGE_720P      0U
#define DSS_BRIDGE_1080P     1U

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

extern void App_wait(uint32_t wait_in_ms);
int32_t DispApp_SetBoardMux();


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

I2C_Handle gI2cHandle = NULL;

/* SN65DSI bridge configurations. Default is for 720p. */
uint8_t gI2cDsiBridgeCfg[][2] = {
{0xFF, 0x7},
{0x16, 0x1},
{0xFF, 0x0},
{0x0A, 0x2},
{0x10, 0x36},
{0x12, 0x4C},
{0x13, 0x4C},
{0x94, 0x80},
{0x0D, 0x1},
{0x5A, 0x4},
{0x5F, 0x28},
{0x93, 0x10},
{0x96, 0x0A},
{0x20, 0x00},
{0x21, 0x05},
{0x22, 0x0},
{0x23, 0x0},
{0x24, 0xD0},
{0x25, 0x02},
{0x2C, 0x20},
{0x2D, 0x00},
{0x30, 0x05},
{0x31, 0x00},
{0x34, 0x50},
{0x36, 0x0C},
{0x38, 0x30},
{0x3A, 0x04},
{0x5B, 0x0},
{0x3C, 0x00},
{0x5A, 0x0C},
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


uint8_t serdesConfig[][4] = {
{0x16, 0x01, 0x0A, 0x5},
{0x16, 0x03, 0x9A, 0x5},
{0x16, 0x17, 0x9E, 0x5},
{0x16, 0x07, 0x58, 0x5},
{0x16, 0x08, 0x22, 0x5},
{0x16, 0x70, 0x80, 0x5},
{0x16, 0x77, 0x24, 0x5},


{0x16, 0x01, 0x08, 0x5},
{0x16, 0x1E, 0x01, 0x5},
{0x16, 0x03, 0x9A, 0x5},
{0x16, 0x03, 0x9A, 0x5},
{0x16, 0x03, 0x9A, 0x5},
{0x16, 0x40, 0x04, 0x5},
{0x16, 0x40, 0x05, 0x5},
{0x16, 0x41, 0x21, 0x5},
{0x16, 0x42, 0x60, 0x5},
{0x16, 0x40, 0x09, 0x5},
{0x16, 0x40, 0x09, 0x5},
{0x16, 0x41, 0x21, 0x5},
{0x16, 0x42, 0x60, 0x5},
{0x16, 0x5b, 0x85, 0x5},
{0x16, 0x4f, 0x8c, 0x5},
{0x16, 0x4f, 0x84, 0x5},
{0x16, 0x40, 0x05, 0x5},
{0x16, 0x40, 0x04, 0x5},
{0x16, 0x41, 0x05, 0x5},
{0x16, 0x42, 0x16, 0x5},
{0x16, 0x40, 0x08, 0x5},
{0x16, 0x40, 0x08, 0x5},
{0x16, 0x41, 0x05, 0x5},
{0x16, 0x42, 0x0c, 0x5},
{0x16, 0x01, 0x00, 0x5},
{0x16, 0x66, 0x03, 0x5},
{0x16, 0x67, 0x03, 0x5},
{0x16, 0x65, 0x01, 0x5},
{0x16, 0x64, 0x00, 0x5},
{0x16, 0x64, 0x04, 0x5},


{0x27, 0x00, 0xFE, 0x5},

{0x11, 0x1D, 0x28, 0x5},
{0x11, 0x1D, 0x29, 0x5},

{0x11, 0x01, 0x06, 0x5},
{0x11, 0x01, 0x04, 0x5},
{0x11, 0x03, 0xf0, 0x5},
{0x11, 0x03, 0xf0, 0x5},
{0x11, 0x03, 0xf8, 0x5},
{0x11, 0x29, 0x00, 0x5},
{0x11, 0x29, 0x00, 0x5},
{0x11, 0x65, 0x00, 0x5},
{0x11, 0x65, 0x00, 0x5},

{0x12, 0x0C, 0x20, 0x5},
{0x12, 0x00, 0x01, 0x5},
{0x12, 0x04, 0xE6, 0x5},

};

int32_t DispApp_SetBoardMux()
{
#if defined (SOC_J721E)
    Board_I2cInitCfg_t i2cCfg;

    /*setting power mux for dsi lcd*/
    i2cCfg.i2cInst   = BOARD_I2C_IOEXP_DEVICE4_INSTANCE;
    i2cCfg.socDomain = BOARD_SOC_DOMAIN_MAIN;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    /* Enable DSI in IO Expander */
    Board_i2cIoExpInit();

    /* Enable Reset for external Power */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                                  ONE_PORT_IOEXP,
                                  PORTNUM_0,
                                  PIN_NUM_7,
                                  PIN_DIRECTION_OUTPUT);
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                              ONE_PORT_IOEXP,
                              PORTNUM_0,
                              PIN_NUM_7,
                              GPIO_SIGNAL_LEVEL_HIGH);

    /* Enable Power Switch control */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                                  ONE_PORT_IOEXP,
                                  PORTNUM_0,
                                  PIN_NUM_2,
                                  PIN_DIRECTION_OUTPUT);

    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE4_ADDR,
                              ONE_PORT_IOEXP,
                              PORTNUM_0,
                              PIN_NUM_2,
                              GPIO_SIGNAL_LEVEL_HIGH);
    Board_i2cIoExpDeInit();
#endif
    return (FVID2_SOK);
}

void DispApp_ErrorRegRead(void)
{
    int i = 0;
    uint8_t readVal;

    for(i = 0xF0; i < 0xF8; i++)
    {
        Board_i2c8BitRegRd(gI2cHandle,
                           0x2C,
                           i & 0xFF,
                           &readVal,
                           1,
                           DISP_APP_I2C_TIMEOUT);
        if (0 != readVal)
        {
            printf("Status at 0x%x in the SN65DSI bridge is 0x%x...\n", i, readVal);
        }
    }
}

void DisapApp_prepareI2cConfig(uint32_t resolution)
{
    int32_t i = 0;
    for(i = 0; i<sizeof(gI2cDsiBridgeCfg)/sizeof(gI2cDsiBridgeCfg[0]); i++)
    {
        if (0x10 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x26;
            }
        }
        else if (0x12 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x4F;
            }
        }
        else if (0x93 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x20;
            }
        }
        else if (0x20 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x80;
            }
        }
        else if (0x21 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x07;
            }
        }
        else if (0x24 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x38;
            }
        }
        else if (0x25 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x4;
            }
        }
        else if (0x30 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x08;
            }
        }
        else if (0x34 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x28;
            }
        }
        else if (0x36 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x06;
            }
        }
        else if (0x38 == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x08;
            }
        }
        else if (0x3A == gI2cDsiBridgeCfg[i][0])
        {
            if (DSS_BRIDGE_1080P == resolution)
            {
                gI2cDsiBridgeCfg[i][1] = 0x10;
            }
        }

    }
}

int32_t DispApp_InitI2c()
{
    int32_t status = FVID2_SOK;

#if defined (SOC_J721E)
    uint32_t loopCnt;
    uint8_t domain, i2cInst, slaveAddr;
    I2C_Params i2cParams;
    I2C_HwAttrs i2cConfig;

    /* Initialize I2C Driver */
    for(loopCnt = 0U; loopCnt < I2C_HWIP_MAX_CNT; loopCnt++)
    {
        I2C_socGetInitCfg(loopCnt, &i2cConfig);
        i2cConfig.enableIntr = BFALSE;
        I2C_socSetInitCfg(loopCnt, &i2cConfig);
    }

    /* Initializes the I2C */
    I2C_init();

    /* Initializes the I2C Parameters */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz; /* 400KHz */

    Board_fpdUb941GetI2CAddr(&domain, &i2cInst, &slaveAddr);

    /* Configures the I2C instance with the passed parameters*/
    gI2cHandle = I2C_open(i2cInst, &i2cParams);
    if(NULL == gI2cHandle)
    {
        App_print("\nI2C Open failed!\n");
        status = FVID2_EFAIL;
    }
#elif defined (SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    I2C_Params i2cParams;

    /* Enable the DSI to eDP bridge. */
    Board_control(BOARD_CTRL_CMD_ENABLE_DSI2DP_BRIDGE, NULL);
    Osal_delay(1000);
    App_print("DSI to EDP bridge is enabled...\n");

    /* Do the i2c driver init */
    I2C_init();

    /* Initializes the I2C Parameters for instance 4*/
    I2C_Params_init(&i2cParams);
    gI2cHandle = I2C_open(4, &i2cParams);
    if(NULL == gI2cHandle)
    {
        printf("\nI2C Open failed!\n");
        status = FVID2_EFAIL;
    }
#endif

    return (status);
}

int32_t DispApp_cfgAdditionalDsiPeripherals()
{
    int32_t status = FVID2_SOK;
#if defined(SOC_J721E)
    uint32_t cnt, clientAddr;
#endif

    status = DispApp_SetBoardMux();

    if (FVID2_SOK == status)
    {
        status = DispApp_InitI2c();
    }
/* SerDes configuration is required only for J721E. */
#if defined (SOC_J721E)
    if (FVID2_SOK == status)
    {
        for (cnt = 0U; cnt < sizeof(serdesConfig)/4; cnt ++)
        {
            clientAddr = serdesConfig[cnt][0];
            status = Board_i2c8BitRegWr(
                gI2cHandle, clientAddr, serdesConfig[cnt][1],
                &serdesConfig[cnt][2], 1U, BOARD_I2C_TRANSACTION_TIMEOUT);

            App_wait((uint32_t)serdesConfig[cnt][3]);

            if (BOARD_SOK != status)
            {
                App_print("\n Write Failed for 0x%x 0x%x 0x%x !\n",
                    clientAddr, serdesConfig[cnt][1], serdesConfig[cnt][2]);
                break;
            }
        }
    }
#elif defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
    uint8_t readVal, wrVal = 0xFF, i;
    for(i = 0xF0; i < 0xF9; i++)
    {
        Board_i2c8BitRegWr(gI2cHandle,
                            0x2C,
                            i & 0xFF,
                            &wrVal,
                            1,
                            DISP_APP_I2C_TIMEOUT);
    }
    DisapApp_prepareI2cConfig(DSS_BRIDGE_720P);
    for(i = 0; i < sizeof(gI2cDsiBridgeCfg)/sizeof(gI2cDsiBridgeCfg[0]); i++)
    {
        /* Read the value at bridge address. */
        Board_i2c8BitRegWr(gI2cHandle,
                            0x2C,
                            gI2cDsiBridgeCfg[i][0] & 0xFF,
                            &(gI2cDsiBridgeCfg[i][1]),
                            1,
                            DISP_APP_I2C_TIMEOUT);
        Osal_delay(200);
        /* Wait untill the DP_PLL has been locked. */
        if (0x0D == gI2cDsiBridgeCfg[i][0])
        {
            do {
                Board_i2c8BitRegRd(gI2cHandle,
                            0x2C,
                            0x0A,
                            &readVal,
                            1,
                            DISP_APP_I2C_TIMEOUT);
            } while (0x00 == (readVal & 0x80));
        }

        /* Main link should not be off. So keep waiting as long as the main links are off. */
        if (0x96 == gI2cDsiBridgeCfg[i][0])
        {
            do {
                Board_i2c8BitRegRd(gI2cHandle,
                            0x2C,
                            0x96,
                            &readVal,
                            1,
                            DISP_APP_I2C_TIMEOUT);
            } while (0x00 == (readVal));
        }
    }
    DispApp_ErrorRegRead();
#endif

    I2C_close(gI2cHandle);

    return (status);
}
