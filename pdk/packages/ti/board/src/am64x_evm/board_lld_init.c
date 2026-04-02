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
 *  \file board_lld_init.c
 *
 *  \brief This file initializes UART and I2C LLD modules 
 *
 */

#include "board_internal.h"
#include "board_utils.h"
#include "board_cfg.h"

extern Board_I2cInitCfg_t gBoardI2cInitCfg;

extern Board_initParams_t gBoardInitParams;

Board_I2cObj_t gBoardI2cObj[BOARD_I2C_PORT_CNT] = {
    {NULL, BOARD_SOC_DOMAIN_MAIN, 0, 0},
    {NULL, BOARD_SOC_DOMAIN_MAIN, 1, 0},
    {NULL, BOARD_SOC_DOMAIN_MAIN, 3, 0},
};

/**
 *  \brief   This function initializes the i2c instance connected to the
 *           board Id EEPROM.
 *
 *  This function disables the interrupt mode as the Board i2c instance 
 *  doesn't require interrupt mode and restores back original at the end.
 *
 *  \param   i2cInst        [IN]        I2C instance.
 *
 *  \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_internalInitI2C(uint8_t i2cInst)
{
    return BOARD_SOK;
}

/**
 *  \brief   This function initializes the default UART instance for use for
 *           console operations.
 *
 *  \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_uartStdioInit(void)
{
    UART_stdioInit(gBoardInitParams.uartInst);

    return BOARD_SOK;
}

/**
 *  \brief   This function is to get the i2c handle of the requested
 *           instance of the specifed domain
 *
 *  \param   domainType [IN] Domain of I2C controller
 *                             BOARD_SOC_DOMAIN_MAIN - Main Domain
 *                             BOARD_SOC_DOMAIN_MCU - MCU domain
 *
 *  \param   i2cInst    [IN]        I2C instance
 *
 *  \return  Valid I2C handle in case of success or NULL in case of failure.
 *
 */
I2C_Handle Board_getI2CHandle(uint8_t domainType,
                              uint32_t i2cInst)
{
    Board_STATUS status;
    Board_I2cInitCfg_t i2cCfg;

    i2cCfg.i2cInst    = i2cInst;
    i2cCfg.socDomain  = domainType;
    i2cCfg.enableIntr = false;
    Board_setI2cInitConfig(&i2cCfg);

    status = Board_i2cInit();
    if(status != BOARD_SOK)
    {
        return NULL;
    }

    return (gBoardI2cObj[i2cInst].i2cHandle);
}

/**
 *  \brief   This function is to release the i2c handle acquired using
 *           Board_getI2CHandle function
 *
 *  \param   hI2c [IN] I2C handle
 *
 *  \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_releaseI2CHandle(I2C_Handle hI2c)
{
    Board_STATUS status = BOARD_FAIL;
    uint32_t i2cInst;

    if(hI2c != NULL)
    {
        for (i2cInst = 0; i2cInst < BOARD_I2C_PORT_CNT; i2cInst++)
        {
            if((hI2c == gBoardI2cObj[i2cInst].i2cHandle))
            {
                break;
            }
        }

        if(i2cInst != BOARD_I2C_PORT_CNT)
        {
            I2C_close(gBoardI2cObj[i2cInst].i2cHandle);
            gBoardI2cObj[i2cInst].i2cHandle = NULL;

            status = BOARD_SOK;
        }
    }

    return status;
}

/**
  *  \brief   This function initializes the i2c instance set using
  *           Board_setI2cInitConfig API.
  *
  *  \return  Board_STATUS in case of success or appropriate error code.
  *
  */
Board_STATUS Board_i2cInit(void)
{
    I2C_Params i2cParams;
    I2C_HwAttrs i2c_cfg;
    uint32_t i2cInst;
    uint32_t i2cDomain;

    i2cInst   = gBoardI2cInitCfg.i2cInst;
    i2cDomain = gBoardI2cInitCfg.socDomain;

    if(gBoardI2cObj[i2cInst].i2cHandle == NULL)
    {

        I2C_init();
        I2C_socGetInitCfg(i2cInst, &i2c_cfg);
        i2c_cfg.enableIntr = gBoardI2cInitCfg.enableIntr;

        I2C_socSetInitCfg(i2cInst, &i2c_cfg);
        I2C_Params_init(&i2cParams);

        gBoardI2cObj[i2cInst].i2cHandle = I2C_open(i2cInst, &i2cParams);
        if (gBoardI2cObj[i2cInst].i2cHandle == NULL)
        {
            return BOARD_I2C_OPEN_FAIL;
        }

        gBoardI2cObj[i2cInst].i2cDomain = i2cDomain;
        gBoardI2cObj[i2cInst].instNum   = i2cInst;
    }
    else
    {
        if(gBoardI2cObj[i2cInst].i2cDomain != i2cDomain)
        {
            return BOARD_I2C_OPEN_FAIL;
        }
    }

    return BOARD_SOK;
}

/**
 *  \brief   This function is used to close the initialized board I2C handle.
 */
Board_STATUS Board_i2cDeInit(void)
{
    uint32_t i2cInst;

    i2cInst = gBoardI2cInitCfg.i2cInst;

    if(gBoardI2cObj[i2cInst].i2cHandle != NULL)
    {
        I2C_close(gBoardI2cObj[i2cInst].i2cHandle);
        gBoardI2cObj[i2cInst].i2cHandle = NULL;
    }

    return BOARD_SOK;
}

/**
 *  \brief   This function is used to de-initialize board UART handles.
 */
Board_STATUS Board_uartDeInit(void)
{
    UART_stdioDeInit();

    return BOARD_SOK;
}
