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
 *  \file   board_control.c
 *
 *  \brief  Implements multiple board control functions
 *
 */

#include "board_control.h"
/**
 * \brief Configures IO expander pins
 *
 *  \return   Board_SOK in case of success or appropriate error code.
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
    if(status == BOARD_SOK)
    {
        /* Setting the pin direction as output */
        status = Board_i2cIoExpSetPinDirection(cfg->slaveAddr,
                                               cfg->ioExpType,
                                               cfg->portNum,
                                               cfg->pinNum,
                                               PIN_DIRECTION_OUTPUT);
        BOARD_delay(1000);
        /* Setting the pin level */
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
 * \brief   Configures IO mux on the EVM
 *
 * \param   mask  [IN] Mask value for the IO expander pins to be configured
 * \param   value [IN] Value to be written to IO expander pins
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setIOMux(uint8_t mask,
                                   uint8_t value)
{
    Board_I2cInitCfg_t i2cCfg;
    Board_STATUS status;
    uint8_t ioExpData;

    i2cCfg.i2cInst    = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    i2cCfg.socDomain  = BOARD_SOC_DOMAIN_MAIN;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    status = Board_i2cIoExpInit();
    if(status == BOARD_SOK)
    {
        /* Setting the port direction as output */
        status = Board_i2cIoExpSetPortDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                                THREE_PORT_IOEXP,
                                                PORTNUM_1,
                                                0);
        BOARD_delay(1000);

        /* Reading the IO expander current port settings */
        status |= Board_i2cIoExpReadOutputPort(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                               THREE_PORT_IOEXP,
                                               PORTNUM_1,
                                               &ioExpData);
        BOARD_delay(1000);

        ioExpData = (ioExpData & ~(mask)) | value;

        /* Modify the IO expander port settings to enable audio Mux */
        status |= Board_i2cIoExpWritePort(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                          THREE_PORT_IOEXP,
                                          PORTNUM_1,
                                          ioExpData);

        BOARD_delay(1000);

        Board_i2cIoExpDeInit();
    }

    return status;
}

/**
 * \brief   Configures the IO mux on the board to route port A to Port B1
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setIOMUXPortB1(void)
{
    Board_STATUS status;

    status = Board_setIOMux(BOARD_CTRL_CMD_IO_MUX_PORTB_MASK,
                            BOARD_CTRL_CMD_IO_MUX_PORTB1_ENABLE);

    return status;
}

/**
 * \brief   Configures the IO mux on board to route port A to Port B2
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setIOMUXPortB2(void)
{
    Board_STATUS status;

    status = Board_setIOMux(BOARD_CTRL_CMD_IO_MUX_PORTB_MASK,
                            BOARD_CTRL_CMD_IO_MUX_PORTB2_ENABLE);

    return status;
}

/**
 * \brief   Configures the IO mux on board to route port A to Port B3
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setIOMUXPortB3(void)
{
    Board_STATUS status;

    status = Board_setIOMux(BOARD_CTRL_CMD_IO_MUX_PORTB_MASK,
                            BOARD_CTRL_CMD_IO_MUX_PORTB3_ENABLE);

    return status;
}

/**
 * \brief   Configures DSI to EDP bridge enable pin to high
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_enableDSI2EDPBridge(void)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE4_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE4_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_2;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_setIoExpPinOutput(&ioExpCfg);
    if(status != BOARD_SOK)
    {
        return status;
    }

    return status;
}

/**
 * \brief   Controls the DP port power
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_configureDPPPower(bool enable)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;
    i2cIoExpSignalLevel_t signalLevel;
    
    if(BTRUE == enable)
    {
        signalLevel = GPIO_SIGNAL_LEVEL_HIGH;
    }
    else
    {
        signalLevel = GPIO_SIGNAL_LEVEL_LOW;
    }

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE4_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE4_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_0;
    ioExpCfg.signalLevel = signalLevel;

    status = Board_setIoExpPinOutput(&ioExpCfg);
    if(status != BOARD_SOK)
    {
        return status;
    }

    ioExpCfg.pinNum = PIN_NUM_1;

    status = Board_setIoExpPinOutput(&ioExpCfg);
    if(status != BOARD_SOK)
    {
        return status;
    }

    return status;
}

/**
 * \brief   Enables the IO Mux for MCAN ports
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_configureMCANMux()
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;
    
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = TWO_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_1;
    ioExpCfg.pinNum      = PIN_NUM_4;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_setIoExpPinOutput(&ioExpCfg);
    if(status != BOARD_SOK)
    {
        return status;
    }

    return status;
}

/**
 * \brief   Enables/Disables LIN transcievers
 *
 * \param   enable  [IN] LIN transciever enable/disable control param
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_linConfig(bool enable)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;
    i2cIoExpSignalLevel_t signalLevel;
    
    if(BTRUE == enable)
    {
        signalLevel = GPIO_SIGNAL_LEVEL_HIGH;
    }
    else
    {
        signalLevel = GPIO_SIGNAL_LEVEL_LOW;
    }
    
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_6;
    ioExpCfg.signalLevel = signalLevel;

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

        case BOARD_CTRL_CMD_SET_IO_MUX_PORTB1:
            status = Board_setIOMUXPortB1();
            break;

        case BOARD_CTRL_CMD_SET_IO_MUX_PORTB2:
            status = Board_setIOMUXPortB2();
            break;

        case BOARD_CTRL_CMD_SET_IO_MUX_PORTB3:
            status = Board_setIOMUXPortB3();
            break;

        case BOARD_CTRL_CMD_ENABLE_DSI2DP_BRIDGE:
            status = Board_enableDSI2EDPBridge();
            break;

        case BOARD_CTRL_CMD_ENABLE_DP_PWR:
            status = Board_configureDPPPower(BTRUE);
            break;

        case BOARD_CTRL_CMD_DISABLE_DP_PWR:
            status = Board_configureDPPPower(BFALSE);
            break;

        case BOARD_CTRL_CMD_ENABLE_MCAN_MUX:
            status = Board_configureMCANMux();
            break;

        case BOARD_CTRL_CMD_LIN_ENABLE:
            status = Board_linConfig(BTRUE);
            break;

        case BOARD_CTRL_CMD_LIN_DISABLE:
            status = Board_linConfig(BFALSE);
            break;

        default:
            status = BOARD_INVALID_PARAM;
            break;
    }

    return status;
}
