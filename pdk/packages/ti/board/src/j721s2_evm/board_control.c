/******************************************************************************
 * Copyright (c) 2021 Texas Instruments Incorporated - http://www.ti.com
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

    i2cCfg.i2cInst    = BOARD_I2C_IOEXP_SOM_INSTANCE;
    i2cCfg.socDomain  = BOARD_SOC_DOMAIN_MAIN;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    status = Board_i2cIoExpInit();
    if(BOARD_SOK == status)
    {
        /* Setting the port direction as output */
        status = Board_i2cIoExpSetPortDirection(BOARD_I2C_IOEXP_SOM_ADDR,
                                                ONE_PORT_IOEXP,
                                                PORTNUM_0,
                                                0);
        BOARD_delay(1000);

        /* Reading the IO expander current port settings */
        status |= Board_i2cIoExpReadOutputPort(BOARD_I2C_IOEXP_SOM_ADDR,
                                               ONE_PORT_IOEXP,
                                               PORTNUM_0,
                                               &ioExpData);
        BOARD_delay(1000);

        ioExpData = (ioExpData & ~(mask)) | value;

        /* Modify the IO expander port settings to enable audio Mux */
        status |= Board_i2cIoExpWritePort(BOARD_I2C_IOEXP_SOM_ADDR,
                                          ONE_PORT_IOEXP,
                                          PORTNUM_0,
                                          ioExpData);

        BOARD_delay(1000);

        Board_i2cIoExpDeInit();
    }

    return status;
}

/**
 * \brief   Configures the mux on SoM board to route port A to Port B1
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setSoMMUXPortB1(void)
{
    Board_STATUS status;

    status = Board_setSomMux(BOARD_CTRL_CMD_SOM_MUX_PORTB_MASK,
                             BOARD_CTRL_CMD_SOM_MUX_PORTB1_ENABLE);

    return status;
}

/**
 * \brief   Configures the mux on SoM board to route port A to Port B2
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setSoMMUXPortB2(void)
{
    Board_STATUS status;

    status = Board_setSomMux(BOARD_CTRL_CMD_SOM_MUX_PORTB_MASK,
                             BOARD_CTRL_CMD_SOM_MUX_PORTB2_ENABLE);

    return status;
}

/**
 * \brief   Configures the mux on SoM board to route port A to Port B3
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setSoMMUXPortB3(void)
{
    Board_STATUS status;

    status = Board_setSomMux(BOARD_CTRL_CMD_SOM_MUX_PORTB_MASK,
                             BOARD_CTRL_CMD_SOM_MUX_PORTB3_ENABLE);

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

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_5;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_setIoExpPinOutput(&ioExpCfg);
    if(BOARD_SOK != status)
    {
        return status;
    }

    return status;
}

/**
 * \brief Board control function
 *
 * \param   cmd  [IN]  Board control command
 * \param   arg  [IN]  Control command argument. 
 *                     Changes based on the command
 *
 * \return   TRUE if the given board is detected else 0.
 *           SoM board will be always connected to the base board.
 *           For SoM boardID return value TRUE indicates dual PMIC
 *           SoM and FALSE indicates alternate PMIC SoM
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

        case BOARD_CTRL_CMD_SET_SOM_MUX_PORTB1:
            status = Board_setSoMMUXPortB1();
            break;

        case BOARD_CTRL_CMD_SET_SOM_MUX_PORTB2:
            status = Board_setSoMMUXPortB2();
            break;

        case BOARD_CTRL_CMD_SET_SOM_MUX_PORTB3:
            status = Board_setSoMMUXPortB3();
            break;

        case BOARD_CTRL_CMD_ENABLE_DSI2DP_BRIDGE:
            status = Board_enableDSI2EDPBridge();
            break;

        default:
            status = BOARD_INVALID_PARAM;
            break;
    }

    return status;
}
