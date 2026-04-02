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
 *  \file   board_utils.c
 *
 *  \brief  Implements multiple board utility functions
 *
 */

#include <stdio.h>
#include <string.h>
#include "board_i2c_io_exp.h"
#include "board_internal.h"
#include "board_utils.h"
#include "board_cfg.h"
#include <ti/osal/osal.h>

Board_I2cInitCfg_t gBoardI2cInitCfg = {0, BOARD_SOC_DOMAIN_MAIN, 0};
Board_initParams_t gBoardInitParams = {BOARD_UART_INSTANCE, BOARD_UART_SOC_DOMAIN, BOARD_PSC_DEVICE_MODE_NONEXCLUSIVE, BOARD_DDR_AUTO};

/**
 * \brief Function to get board init params
 *
 *  This function shall be used to know the current board init
 *  parameters and update them if needed using the function Board_setInitParams.
 *
 * \param   initParams  [IN]  Board init params structure
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_getInitParams(Board_initParams_t *initParams)
{
    if(initParams == NULL)
    {
        return BOARD_INVALID_PARAM;
    }

    *initParams = gBoardInitParams;

    return BOARD_SOK;
}

/**
 * \brief Function to configure board init parameters
 *
 *  Board init params includes the parameters used by Board_init
 *  function for different operations. Default init parameters
 *  used by Board_init can be updated using this function.
 *  All the default params can be overwritten by calling this function
 *  or some of can be changed by reading the existing init parameters
 *  using Board_getInitParams function.
 *
 * Usage:
 * Call Board_getInitParams to get the default board init parameters
 * Update the parameters as needed
 * Call Board_setInitParams to update the default board init parameters
 *
 * \param   initCfg  [IN]  Board Init config structure
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_setInitParams(Board_initParams_t *initParams)
{
    if(initParams == NULL)
    {
        return BOARD_INVALID_PARAM;
    }

    gBoardInitParams = *initParams;

    return BOARD_SOK;
}

/**
 * \brief Function to configure I2C configurations used by board
 *
 * This function is used to set the I2C controller instance and
 * SoC domain used by the board module for IO expander and board
 * ID info read.
 *
 * Usage:
 * Call Board_setI2cInitConfig to set the I2C configurations
 * Call IO expander Init or Board ID info read/write
 *
 *  \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_setI2cInitConfig(Board_I2cInitCfg_t *i2cCfg)
{
    if(i2cCfg == NULL)
    {
        return BOARD_INVALID_PARAM;
    }

    gBoardI2cInitCfg = *i2cCfg;

    return BOARD_SOK;
}

/**
 * \brief Function to get I2C configurations used by board
 *
 *  \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_getI2cInitConfig(Board_I2cInitCfg_t *i2cCfg)
{
    if(i2cCfg == NULL)
    {
        return BOARD_INVALID_PARAM;
    }

    *i2cCfg = gBoardI2cInitCfg;

    return BOARD_SOK;
}

/**
 * \brief Board detect function
 *
 *  Checks if the board with given 'boardID' is connected to the
 *  base board.
 *
 *  \n Note: Board ID EEPROM should be programmed for this function
 *           to work properly.
 *
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_FMZ(0x1)            - FMZ board
 * \n                      BOARD_ID_VPS(0x2)            - VPS Board
 * \n                      BOARD_ID_SERDES_SMA(0x3)     - Serdes SMA Board
 * \n                      BOARD_ID_SVB_CNTR(0x4)       - SVB Controller Board
 * \n                      BOARD_ID_DDR4(0x5)           - DDR4 Board
 * \n                      BOARD_ID_DUT(0x6)            - DUT board
 * \n                      BOARD_ID_LPDDR4(0x7)         - LPDDR4 board
 * \n                      BOARD_ID_SERDES_PCIE_X1(0x8) - Serdes PCIe X1 board
 * \n                      BOARD_ID_SERDES_USBA(0x9)    - Serdes USB Type A board
 * \n                      BOARD_ID_SERDES_USBAB(0x10)   - Serdes USB Type AB board
 *
 * \return   TRUE if the given board is detected else FALSE.
 *
 */
bool Board_detectBoard(uint32_t boardID)
{
    uint8_t boardDetectLineNum;
    bool isBoardDetected = false;
    uint8_t rdRegData;
    Board_IDInfo_v2 info;
    int32_t status;
    int8_t ret;
    
    Board_i2cIoExpInit();

    /* Reading the I/O expansion port to detect the board */
    ret = Board_i2cIoExpReadInputPort(BOARD_PRES_DETECT_SLAVE_ADDR,
                                      ONE_PORT_IOEXP,
                                      PORTNUM_NONE,
                                      &rdRegData);
    if(ret != BOARD_SOK)
    {
        isBoardDetected = false;
    }
    else
    {
        isBoardDetected = false;
        for(boardDetectLineNum = FMZ_BRD_DETECT; boardDetectLineNum <= DDR_DIMM_BRD_DETECT; boardDetectLineNum++)
        {
            /* Check if the board connected to the corresponding line is detected or not */
            if((!((bool)((rdRegData >> boardDetectLineNum) & 0x01))))
            {
                if((boardDetectLineNum == FMZ_BRD_DETECT) &&
                   (boardDetectLineNum == boardID))
                {
                    isBoardDetected = true;
                    break;
                }
                else if((boardDetectLineNum == VPS_BRD_DETECT) &&
                        (boardDetectLineNum == boardID))
                {
                    isBoardDetected = true;
                    break;
                }
                else if((boardDetectLineNum == SERDES_BRD_DETECT) &&
                        ((boardID == BOARD_ID_SERDES_SMA) ||
                         (boardID == BOARD_ID_SERDES_PCIE_X1) ||
                         (boardID == BOARD_ID_SERDES_USBA) ||
                         (boardID == BOARD_ID_SERDES_USBAB)))
                {
                    status = Board_getIDInfo_v2(&info, BOARD_SERDES_EEPROM_SLAVE_ADDR);
                    if(status == 0)
                    {
                        if((boardID == BOARD_ID_SERDES_SMA) &&
                           ((strcmp(info.boardInfo.boardName, "SVB_64_SMA_x1")) == 0))
                        {
                            isBoardDetected = true;
                            break;
                        }
                        else if((boardID == BOARD_ID_SERDES_PCIE_X1) &&
                                ((strcmp(info.boardInfo.boardName, "SVB_M4_PCIe_x1")) == 0))
                        {
                            isBoardDetected = true;
                            break;
                        }
                        else if((boardID == BOARD_ID_SERDES_USBA) &&
                           ((strcmp(info.boardInfo.boardName, "SVB_M4_USB3_A")) == 0))
                        {
                            isBoardDetected = true;
                            break;
                        }
                        else if((boardID == BOARD_ID_SERDES_USBAB) &&
                                ((strcmp(info.boardInfo.boardName, "SVB_M4_USB3_AB")) == 0))
                        {
                            isBoardDetected = true;
                            break;
                        }
                    }
                }
                else if((boardDetectLineNum == SVB_CNTR_BRD_DETECT) &&
                        (boardDetectLineNum == boardID))
                {
                    isBoardDetected = true;
                    break;
                }
                else if((boardDetectLineNum == DDR_DIMM_BRD_DETECT) &&
                        ((boardID == BOARD_ID_DDR4) ||
                         (boardID == BOARD_ID_LPDDR4)))
                {
                    status = Board_getIDInfo_v2(&info, BOARD_DDR_DIMM_EEPROM_SLAVE_ADDR);
                    if(status == 0)
                    {
                        if((boardID == BOARD_ID_DDR4) &&
                           ((strcmp(info.boardInfo.boardName, "SVB_64_DDR4")) == 0))
                        {
                            isBoardDetected = true;
                            break;
                        }
                        else if((boardID == BOARD_ID_LPDDR4) &&
                                ((strcmp(info.boardInfo.boardName, "SVB_64_LDDR4")) == 0))
                        {
                            isBoardDetected = true;
                            break;
                        }
                    }
                }
            }
        }
        if(boardID == DUT_BRD_DETECT)
        {
            isBoardDetected = true;
        }
    }

    Board_i2cIoExpDeInit();

    return isBoardDetected;
}

/**
 * \brief Function to get the SoC domain
 *
 *  This function returns the domain of the SoC core on which
 *  it is executing.
 *
 * \return   SoC domain of the core.
 *
 */
uint32_t Board_getSocDomain(void)
{
    uint32_t socDomain;

#if defined (BUILD_MPU)
    socDomain = BOARD_SOC_DOMAIN_MAIN;
#elif defined (BUILD_MCU)
    socDomain = BOARD_SOC_DOMAIN_MAIN;
#elif defined (BUILD_M4F)
    socDomain = BOARD_SOC_DOMAIN_MCU;
#else
#error "Unsupported core id"
#endif

  return socDomain;
}

/**
 *  \brief    Function to generate delay in micro seconds
 *
 *  This function takes the delay parameters in usecs but the generated
 *  delay will be in multiples of msecs due to the osal function which
 *  generates delay in msecs. Delay parameter passed will be converted to
 *  msecs and fractional value will be adjusted to nearest msecs value.
 *  Minimum delay generated by this function is 1 msec.
 *  Function parameter is kept in usecs to match with existing
 *  platforms which has delay function for usecs.
 *
 *  \param    usecs [IN]  Specifies the time to delay in micro seconds.
 *
 */
void BOARD_delay(uint32_t usecs)
{
    uint32_t msecs;

    msecs = usecs/1000;
    if(usecs%1000)
    {
        msecs += 1;
    }

    Osal_delay(msecs);
}
