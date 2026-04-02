/*
 *  Copyright (C) 2014 - 2022 Texas Instruments Incorporated
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
 */

/**
 *  \file stw_crcApp.c
 *
 *  \brief Main file invoking the test function.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/cslr_mcan.h>
#include <ti/csl/csl_mcan.h>
#if defined (SOC_TDA3XX) || defined (SOC_DRA78x) || (SOC_TDA2PX)
#include <ti/csl/example/utils/uart_console/inc/uartConfig.h>
#endif
#if defined (SOC_AM65XX) || defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_AM64X) || defined (SOC_J721S2)
#include <ti/board/board.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/csl/csl_gpio.h>
#include <ti/drv/i2c/i2c.h>
#endif
#if defined (SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_i2c_io_exp.h>
#include <ti/board/src/j721s2_evm/include/board_control.h>
#endif
#if defined (SOC_J7200) 
#include <ti/board/src/j7200_evm/include/board_i2c_io_exp.h>
#include <ti/board/src/j7200_evm/include/board_control.h>
#endif
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#if defined (SOC_TDA2XX) || defined (SOC_TDA2PX) || defined (SOC_TDA2EX) || defined (SOC_DRA72x) || defined (SOC_DRA75x)
uint32_t uartBaseAddr = SOC_UART1_BASE;
#endif
#if defined (SOC_TDA3XX) || defined (SOC_DRA78x)
uint32_t uartBaseAddr = SOC_UART3_BASE;
#endif

#if defined (SOC_AM65XX)
/* Port and pin number mask for GPIO Load pin.
   Bits 7-0: Pin number  and Bits 15-8: Port number */
#define AM65XX_IDK_GPIO_CAN0_STB_PIN    (0x012F)
#define AM65XX_IDK_GPIO_CAN1_STB_PIN    (0x0143)
#endif
#if defined (SOC_J721E)
#define J721E_GPIO_GESI_CAN_STB_PIN     (0x003C)
#define J721E_GPIO_MAIN_MCAN2_STB_PIN   (0x007F)
#endif
#if defined (SOC_AM64X)
/* TBD: Port and pin number mask for GPIO Load pin.
   Bits 7-0: Pin number  and Bits 15-8: Port number */
#define AM64X_GPIO_CAN0_STB_PIN          (0x012F)
#define AM64X_GPIO_CAN1_STB_PIN          (0x0143)
#endif

#define TCA6424_CMD_AUTO_INC            ((uint8_t) 0x80U)

/* Input status register */
#define TCA6424_REG_INPUT0              ((UInt8) 0x00U)
#define TCA6424_REG_INPUT1              ((UInt8) 0x01U)
#define TCA6424_REG_INPUT2              ((UInt8) 0x02U)

/* Output register to change state of output BIT set to 1, output set HIGH */
#define TCA6424_REG_OUTPUT0             ((uint8_t) 0x04U)
#define TCA6424_REG_OUTPUT1             ((uint8_t) 0x05U)
#define TCA6424_REG_OUTPUT2             ((uint8_t) 0x06U)

/* Configuration register. BIT = '1' sets port to input, BIT = '0' sets
 * port to output */
#define TCA6424_REG_CONFIG0             ((uint8_t) 0x0CU)
#define TCA6424_REG_CONFIG1             ((uint8_t) 0x0DU)
#define TCA6424_REG_CONFIG2             ((uint8_t) 0x0EU)

/* MCAN STB & Enable pins */
#define MCU_MCAN1_STB_PIN               (0x2)               
#define MCU_MCAN0_ENABLE_PIN            (0x0)

#if defined (SOC_J721E)
#define MCU_MCAN0_STBZ_PIN              (0x36)
#elif defined (SOC_J7200)
#define MCU_MCAN0_STBZ_PIN              (0x3A)
#elif defined (SOC_J721S2)
#define MCU_MCAN0_STBZ_PIN              (0x45)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
extern int32_t mcanParser(void);
void SetupI2CTransfer(I2C_Handle handle,  uint32_t slaveAddr,
                      uint8_t *writeData, uint32_t numWriteBytes,
                      uint8_t *readData,  uint32_t numReadBytes);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
#if defined (SOC_AM65XX)
/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] = {
    /* Output pin : AM65XX_IDK */
    AM65XX_IDK_GPIO_CAN0_STB_PIN | GPIO_CFG_OUTPUT,
    AM65XX_IDK_GPIO_CAN1_STB_PIN | GPIO_CFG_OUTPUT
};
#endif

#if defined (SOC_J721E)
/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] = {
    /* Output pin : CAN STB */
    J721E_GPIO_GESI_CAN_STB_PIN | GPIO_CFG_OUTPUT,
    J721E_GPIO_MAIN_MCAN2_STB_PIN | GPIO_CFG_OUTPUT,
};
#endif

#if defined (SOC_J7200)
/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] = {
};
#endif

#if defined (SOC_J721S2)
/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] = {
};
#endif

#if defined (SOC_AM64X)
/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] = {
    /* Output pin : AM65XX_IDK */
    AM64X_GPIO_CAN0_STB_PIN | GPIO_CFG_OUTPUT,
    AM64X_GPIO_CAN1_STB_PIN | GPIO_CFG_OUTPUT
};
#endif

/* GPIO Driver call back functions */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
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

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void mcanTestPrint(const char *str, ...)
{
    printf("%s", str);
    return;
}

/**
 *  main
 *  Application main function.
 */
int32_t main_task(void)
{
#if defined (SOC_AM65XX) || defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_AM64X) || defined (SOC_J721S2)
    Board_initCfg   boardCfg;
    Board_STATUS    boardStatus;
    Sciclient_ConfigPrms_t  sciClientCfg;
#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) 
    I2C_Params      i2cParams;
    I2C_Handle      handle = NULL;
    uint8_t         dataToSlave[4];
#endif

    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        mcanTestPrint("[Error] Board init failed!!\n");
    }
#if defined (SOC_J721E)
    /* Pin mux for CAN STB used in GESI board */
    *(volatile unsigned int *)(0x0011c0f4) = 0x20007;
    /* Pinmux for MAIN_MCAN4 */
    *(volatile unsigned int *)(0x0011c020) = 0x60006;
    *(volatile unsigned int *)(0x0011c024) = 0x60006;
    /* Pinmux for MAIN_MCAN5 */
    *(volatile unsigned int *)(0x0011c04c) = 0x60006;
    *(volatile unsigned int *)(0x0011c050) = 0x60006;
    /* Pinmux for MAIN_MCAN6 */
    *(volatile unsigned int *)(0x0011c054) = 0x60006;
    *(volatile unsigned int *)(0x0011c06C) = 0x60006;
    /* Pinmux for MAIN_MCAN7 */
    *(volatile unsigned int *)(0x0011c074) = 0x60006;
    *(volatile unsigned int *)(0x0011c078) = 0x60006;
    /* Pinmux for MAIN_MCAN9 */
    *(volatile unsigned int *)(0x0011c0cc) = 0x60006;
    *(volatile unsigned int *)(0x0011c0d0) = 0x60006;
    /* Pinmux for MAIN_MCAN11 */
    *(volatile unsigned int *)(0x0011c11c) = 0x60006;
    *(volatile unsigned int *)(0x0011c120) = 0x60006;
#endif
    /* GPIO initialization */
    GPIO_init();

    /* Enable CAN transceivers by setting the STB pins */
#if defined (SOC_AM65XX) || defined (SOC_AM64X)
    GPIO_write(0, GPIO_PIN_HIGH);
    GPIO_write(1, GPIO_PIN_HIGH);
#elif defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)
    /* Enable the TCAN on GESI board.
     * Main Domain MCAN instances 4,5,6,7,9,11.
     */
    GPIO_write(0, GPIO_PIN_LOW);

    /* Set MCU_MCAN1_STB to LEVEL_LOW to exit CAN1 from Standby mode */
    GPIOSetDirMode_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN1_STB_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN1_STB_PIN, GPIO_PIN_LOW);

#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)
    /* Set MCU_MCAN0_EN Enable to LEVEL_HIGH to Enable MCU MCAN 0*/
    GPIOSetDirMode_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_ENABLE_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_ENABLE_PIN, GPIO_PIN_HIGH);

    /* Set MCU_MCAN0_STBz pin to LEVEL_High to exit CAN0 from Standby mode */
    GPIOSetDirMode_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_STBZ_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_STBZ_PIN, GPIO_PIN_HIGH);
#endif
    /* Enable Main MCAN 2, GPIO0_127. */
    GPIO_write(1, GPIO_PIN_LOW);

    /*
     * Configuring TCA6424 IO Exp 2 with addr 0x22
     * This io expander is controlled by i2c0
     * For Main MCAN2 P13 and P14 should be set to 0, This should route the MCAN2 STB line to transciver.
     * For Main MCAN0 P06 and P07 should be set to 1.
     */
    /* I2C initialization */
    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cParams.transferMode = I2C_MODE_BLOCKING;
    i2cParams.bitRate = I2C_400kHz;
    i2cParams.transferCallbackFxn = NULL;

    handle = I2C_open(0U, &i2cParams);

    /* Make all pins as input */
    dataToSlave[0] = TCA6424_REG_CONFIG0 | TCA6424_CMD_AUTO_INC;
    dataToSlave[1] = 0xFFU;
    dataToSlave[2] = 0xFFU;
    dataToSlave[3] = 0xFFU;
    SetupI2CTransfer(handle, 0x22, &dataToSlave[0], 4, NULL, 0);

    /* Get the default pin values. */
    dataToSlave[0] = TCA6424_REG_INPUT0 | TCA6424_CMD_AUTO_INC;
    dataToSlave[1] = 0x0U;
    dataToSlave[2] = 0x0U;
    dataToSlave[3] = 0x0U;
    SetupI2CTransfer(handle, 0x22, &dataToSlave[0], 1, &dataToSlave[1], 3);

    /* Set the default values in output port. */
    dataToSlave[0] = TCA6424_REG_OUTPUT0 | TCA6424_CMD_AUTO_INC;
    SetupI2CTransfer(handle, 0x22, &dataToSlave[0], 4, NULL, 0);

    /* Configure the output ports */
    dataToSlave[0] = TCA6424_REG_CONFIG0 | TCA6424_CMD_AUTO_INC;
    dataToSlave[1] = 0x0U;
    dataToSlave[2] = 0x0U;
    dataToSlave[3] = 0x2CU;
    SetupI2CTransfer(handle, 0x22, &dataToSlave[0], 4, NULL, 0);

    /* Read output port */
    dataToSlave[0] = TCA6424_REG_OUTPUT0 | TCA6424_CMD_AUTO_INC;
    dataToSlave[1] = 0x0U;
    dataToSlave[2] = 0x0U;
    dataToSlave[3] = 0x0U;
    SetupI2CTransfer(handle, 0x22, &dataToSlave[0], 1, &dataToSlave[1], 3);

    /* Set P06 and P07 to 1.
     * Set P13 and P14 to 0.
     */
    dataToSlave[0] = TCA6424_REG_OUTPUT0 | TCA6424_CMD_AUTO_INC;
    dataToSlave[1] |= 0xC0;
    dataToSlave[2] &= ~(0x18);
    SetupI2CTransfer(handle, 0x22, &dataToSlave[0], 4, NULL, 0);
#endif

    Sciclient_configPrmsInit(&sciClientCfg);
    Sciclient_init(&sciClientCfg);
#endif

#if defined (SOC_J7200)
    //configuring the i2c to program GPIO expander, and set CAN STB to LEVEL_LOW
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_7;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;

    Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_1;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_2;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

   status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_3;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_5;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status !=0)
    {
        printf("Board_control API failed. status=%d\n", status);
    }
    else
    {
        printf("Board_control API passed\n");
    }
#endif
#if defined (SOC_J721S2)
    //configuring the i2c to program GPIO expander, and set CAN STB to LEVEL_LOW
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_7;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;

    Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
     ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_1;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_2;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_3;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status !=0)
    {
        printf("Board_control API failed. status=%d\n", status);
    }
    else
    {
        printf("Board_control API passed\n");
    }

#endif

    mcanParser();

#if defined (SOC_AM65XX) || defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_AM64X) || defined (SOC_J721S2)
    Sciclient_deinit();
#endif

    return 0;
}

void SetupI2CTransfer(I2C_Handle handle,  uint32_t slaveAddr,
                      uint8_t *writeData, uint32_t numWriteBytes,
                      uint8_t *readData,  uint32_t numReadBytes)
{
    bool status;
    I2C_Transaction i2cTransaction;

    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = slaveAddr;
    i2cTransaction.writeBuf = (uint8_t *)&writeData[0];
    i2cTransaction.writeCount = numWriteBytes;
    i2cTransaction.readBuf = (uint8_t *)&readData[0];
    i2cTransaction.readCount = numReadBytes;
    status = I2C_transfer(handle, &i2cTransaction);
    if(FALSE == status)
    {
        printf("\n Data Transfer failed. \n");
    }
}
