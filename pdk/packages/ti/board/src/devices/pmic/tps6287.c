/******************************************************************************
 * Copyright (c) 2025 Texas Instruments Incorporated - http://www.ti.com
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
 *   \file    tps6287.c
 *
 *   \brief   This file contains the default API's for configuring TPS6287
 *            PMIC.
 *
 */

#include "tps6287.h"
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/uart/UART_stdio.h>

/* Register configuration defines for TPS6287x PMIC */
#define TPS6287X_REG_VSET                   0x0
#define TPS6287X_REG_CONTROL1               0x1
#define TPS6287X_REG_CONTROL2               0x2
#define TPS6287X_REG_CONTROL3               0x3
#define TPS6287X_REG_STATUS                 0x4

#define TPS6287X_REG_VSET_VSET_MASK         0xff

#define TPS6287X_REG_CONTROL2_VRANGE_MASK   0xc
#define TPS6287X_REG_CONTROL2_VRANGE_SHIFT  2

#define TPS6287X_MIN_VOLTAGE_MV             (400U)
#define TPS6287X_MAX_VOLTAGE_MV             (3350U)

/** I2C number of bytes */
#define BOARD_I2C_NUM_OF_BYTES_01           (1U)

Board_STATUS Board_TPS6287SetVoltage(void *handle,
                                     uint8_t slaveAddr,
                                     uint8_t powerResource,
                                     uint16_t millivolt)
{
    Board_STATUS boardStatus = BOARD_SOK;
    uint8_t voutCode;
    uint8_t regData = 0;
    uint32_t microvolt = millivolt * 1000;

    if ((NULL == handle) ||
        (TPS6287X_MIN_VOLTAGE_MV >= millivolt) ||
        (TPS6287X_MAX_VOLTAGE_MV <= millivolt))
    {
        return BOARD_INVALID_PARAM;
    }

    /*
     * Based on the value of VRANGE bit field of CONTROL2 reg the range
     * varies.
     */
    boardStatus = Board_i2c8BitRegRd(handle,
                                        slaveAddr,
                                        TPS6287X_REG_CONTROL2,
                                        &regData,
                                        BOARD_I2C_NUM_OF_BYTES_01,
                                        BOARD_I2C_TRANSACTION_TIMEOUT);
    if(BOARD_SOK != boardStatus)
    {
        return BOARD_I2C_TRANSFER_FAIL;
    }

    regData = (regData & TPS6287X_REG_CONTROL2_VRANGE_MASK) >> TPS6287X_REG_CONTROL2_VRANGE_SHIFT;

    /*
     * VRANGE = 0. Increment step 1250 uV starting with 0 --> 400000 uV
     * VRANGE = 1. Increment step 2500 uV starting with 0 --> 400000 uV
     * VRANGE = 2. Increment step 5000 uV starting with 0 --> 400000 uV
     * VRANGE = 3. Increment step 10000 uV starting with 0 --> 800000 uV
     */
    switch (regData) {
    case 0:
        voutCode = (microvolt - 400000) / 1250;
        break;
    case 1:
        voutCode = (microvolt - 400000) / 2500;
        break;
    case 2:
        voutCode = (microvolt - 400000) / 5000;
        break;
    case 3:
        voutCode = (microvolt - 800000) / 10000;
        break;
    default:
        return BOARD_FAIL;
    }

    boardStatus = Board_i2c8BitRegWr(handle,
                                        slaveAddr,
                                        TPS6287X_REG_VSET,
                                        &voutCode,
                                        BOARD_I2C_NUM_OF_BYTES_01,
                                        BOARD_I2C_TRANSACTION_TIMEOUT);
    if(BOARD_SOK != boardStatus)
    {
        return BOARD_I2C_TRANSFER_FAIL;
    }

    /* Read back the VSET register and verify the value set */
    boardStatus = Board_i2c8BitRegRd(handle,
                                        slaveAddr,
                                        TPS6287X_REG_VSET,
                                        &regData,
                                        BOARD_I2C_NUM_OF_BYTES_01,
                                        BOARD_I2C_TRANSACTION_TIMEOUT);
    if(BOARD_SOK != boardStatus)
    {
        return BOARD_I2C_TRANSFER_FAIL;
    }

    if(regData != voutCode)
    {
        return BOARD_FAIL;
    }

    return boardStatus;
}
