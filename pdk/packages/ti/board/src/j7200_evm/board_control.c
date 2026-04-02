/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   board_control.c
 *
 *  \brief  Implements multiple board control functions
 *
 */

#include "board_control.h"

/**
 * \brief   Configures IO expander pin level
 *
 * \param   cfg [IN] Structure to configure IO expander
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setIoExpPinOutput(Board_IoExpCfg_t *cfg)
{
    Board_I2cInitCfg_t i2cCfg;
    Board_STATUS status;

    i2cCfg.i2cInst    = cfg->i2cInst;
    i2cCfg.socDomain  = cfg->socDomain;
    i2cCfg.enableIntr = cfg->enableIntr;
    Board_setI2cInitConfig(&i2cCfg);

    status = Board_i2cIoExpInit();
    if(BOARD_SOK == status)
    {
        /* Setting the pin direction as output */
        status = Board_i2cIoExpSetPinDirection(cfg->slaveAddr,
                                               cfg->ioExpType,
                                               cfg->portNum,
                                               cfg->pinNum,
                                               PIN_DIRECTION_OUTPUT);
        BOARD_delay(1000);
        /* Pulling the hdmi power pin to low */
        status |= Board_i2cIoExpPinLevelSet(cfg->slaveAddr,
                                            cfg->ioExpType,
                                            cfg->portNum,
                                            cfg->pinNum,
                                            cfg->signalLevel);
        BOARD_delay(1000);

        Board_i2cIoExpDeInit();
    }

    return status;
}

/**
 * \brief   Configures CPSW5G mux to high
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setCpsw5GMdioMux(void)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = TWO_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_1;
    ioExpCfg.pinNum      = PIN_NUM_5;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_setIoExpPinOutput(&ioExpCfg);
    if(BOARD_SOK != status)
    {
        return status;
    }

    ioExpCfg.pinNum = PIN_NUM_6;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;
    status = Board_setIoExpPinOutput(&ioExpCfg);

    return status;
}

/**
 * \brief   Configures IO mux on SoM board
 *
 * \param   mask  [IN] Mask value for the IO expander pins to be configured
 * \param   value [IN] Value to be written to IO expander pins
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setSomMux(uint8_t mask,
                                    uint8_t value)
{
    Board_I2cInitCfg_t i2cCfg;
    Board_STATUS status;
    uint8_t ioExpData;

    i2cCfg.i2cInst    = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE;
    i2cCfg.socDomain  = BOARD_SOC_DOMAIN_MAIN;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    status = Board_i2cIoExpInit();
    if(BOARD_SOK == status)
    {
        /* Setting the port direction as output */
        status = Board_i2cIoExpSetPortDirection(BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR,
                                                ONE_PORT_IOEXP,
                                                PORTNUM_0,
                                                0);
        BOARD_delay(1000);

        /* Reading the IO expander current port settings */
        status |= Board_i2cIoExpReadOutputPort(BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR,
                                               ONE_PORT_IOEXP,
                                               PORTNUM_0,
                                               &ioExpData);
        BOARD_delay(1000);

        ioExpData = (ioExpData & ~(mask)) | value;

        /* Modify the IO expander port settings to enable audio Mux */
        status |= Board_i2cIoExpWritePort(BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR,
                                          ONE_PORT_IOEXP,
                                          PORTNUM_0,
                                          ioExpData);

        BOARD_delay(1000);        

        Board_i2cIoExpDeInit();
    }

    return status;
}

/**
 * \brief   Enables Audio mux on SoM board
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setSomAudioMux(void)
{
    Board_STATUS status;

    status = Board_setSomMux(BOARD_CTRL_CMD_SOM_AUDIO_MUX_MASK,
                             BOARD_CTRL_CMD_SOM_AUDIO_MUX_ENABLE);

    return status;
}

/**
 * \brief   Enables CAN and UART mux on SoM board
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setSomCanMux(void)
{
    Board_STATUS status;

    status = Board_setSomMux(BOARD_CTRL_CMD_SOM_CAN_MUX_MASK,
                             BOARD_CTRL_CMD_SOM_CAN_MUX_ENABLE);

    return status;
}

/**
 * \brief   Enables Profibus and SPI mux on SoM board
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setSomProfibusMux(void)
{
    Board_STATUS status;

    status = Board_setSomMux(BOARD_CTRL_CMD_SOM_PROFIBUS_MUX_MASK,
                             BOARD_CTRL_CMD_SOM_PROFIBUS_MUX_ENABLE);

    return status;
}

/**
 * \brief   Enables LIN mux on SoM board
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setSomLinMux(void)
{
    Board_STATUS status;

    status = Board_setSomMux(BOARD_CTRL_CMD_SOM_LIN_MUX_MASK,
                             BOARD_CTRL_CMD_SOM_LIN_MUX_ENABLE);

    return status;
}

/**
 * \brief   Enables UART mux on SoM board
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setSomUartMux(void)
{
    Board_STATUS status;

    status = Board_setSomMux(BOARD_CTRL_CMD_SOM_UART_MUX_MASK,
                             BOARD_CTRL_CMD_SOM_UART_MUX_ENABLE);

    return status;
}

/**
 * \brief   Enables/Disables LIN transcievers
 *
 * \param   pinLevel  [IN] IO expnader pin level
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_linConfig(i2cIoExpSignalLevel_t pinLevel)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_6;
    ioExpCfg.signalLevel = pinLevel;

    status = Board_setIoExpPinOutput(&ioExpCfg);

    return status;
}

/**
 * \brief Board control function
 *
 * \param   cmd  [IN]  Board control command
 * \param   arg  [IN]  Control command argument. 
 *                     Changes based on the command
 *
 * \return   Board_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_control(uint32_t cmd, void *arg)
{
    Board_STATUS status;

    switch (cmd)
    {
        case BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT:
            status = Board_setIoExpPinOutput((Board_IoExpCfg_t *)arg);
            break;

        case BOARD_CTRL_CMD_SET_GESI_CPSW_MDIO_MUX:
            status = Board_setCpsw5GMdioMux();
            break;

        case BOARD_CTRL_CMD_SET_SOM_CAN_MUX:
            status = Board_setSomCanMux();
            break;

        case BOARD_CTRL_CMD_SET_SOM_AUDIO_MUX:
            status = Board_setSomAudioMux();
            break;

        case BOARD_CTRL_CMD_SET_SOM_PROFIBUS_MUX:
            status = Board_setSomProfibusMux();
            break;

        case BOARD_CTRL_CMD_SET_SOM_LIN_MUX:
            status = Board_setSomLinMux();
            break;

        case BOARD_CTRL_CMD_SET_SOM_UART_MUX:
            status = Board_setSomUartMux();
            break;
            
      case BOARD_CTRL_CMD_LIN_ENABLE:
            status = Board_linConfig(GPIO_SIGNAL_LEVEL_HIGH);
            break;

      case BOARD_CTRL_CMD_LIN_DISABLE:
            status = Board_linConfig(GPIO_SIGNAL_LEVEL_LOW);
            break;

        default:
            status = BOARD_INVALID_PARAM;
            break;
    }

    return status;
}
