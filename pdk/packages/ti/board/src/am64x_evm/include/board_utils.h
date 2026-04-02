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
 *  \file   board_utils.h
 *
 *  \brief  Board utility functions header file
 *
 */

#ifndef _BOARD_UTILS_H_
#define _BOARD_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Include Files                                                             *
 *****************************************************************************/
#include <ti/csl/csl_types.h>
#include <ti/csl/cslr_device.h>

#include <ti/board/board.h>
#include <ti/csl/tistdtypes.h>
#include <stdio.h>
#include <stdbool.h>

#define BOARD_PSC_DEVICE_MODE_EXCLUSIVE       (0)
#define BOARD_PSC_DEVICE_MODE_NONEXCLUSIVE    (1U)

#define BOARD_MAIN_CLOCK_GROUP_ALL            (0U)
#define BOARD_MAIN_CLOCK_GROUP1               (1U)
#define BOARD_MAIN_CLOCK_GROUP2               (2U)

#define BOARD_MCU_CLOCK_GROUP_ALL             (0U)
#define BOARD_MCU_CLOCK_GROUP1                (1U)
#define BOARD_MCU_CLOCK_GROUP2                (2U)

/**
 * \brief Structure to configure the board I2C parameters
 */
typedef struct Board_I2cInitCfg_s
{
    /** I2C controller instance */
    uint32_t i2cInst;
    /** SoC domain of the I2C controller */
    uint32_t socDomain;
    /** I2C controller interrupt enable/disable flag */
    bool enableIntr;
} Board_I2cInitCfg_t;

/**
 * \brief Structure to configure the board init parameters
 */
typedef struct Board_initParams_s
{
    /** UART controller instance */
    uint32_t uartInst;
    /** SoC domain of the UART controller */
    uint32_t uartSocDomain;
    /** Mode for PSC clock enable
        BOARD_PSC_DEVICE_MODE_EXCLUSIVE - Exclusive access to the core requesting access
        BOARD_PSC_DEVICE_MODE_NONEXCLUSIVE - Non-exclusive which allows other cores to get access */
    uint8_t pscMode;
    /** Group selection for MAIN domain clock enable
        BOARD_MAIN_CLOCK_GROUP_ALL - Enable clock for all groups in main domain
        BOARD_MAIN_CLOCK_GROUP1 - Enable clock for all group1 in main domain
        BOARD_MAIN_CLOCK_GROUP2 - Enable clock for all group2 in main domain */
    uint8_t mainClkGrp;
    /** Group selection for MCU domain clock enable
        BOARD_MCU_CLOCK_GROUP_ALL - Enable clock for all groups in mcu domain
        BOARD_MCU_CLOCK_GROUP1 - Enable clock for all group1 in mcu domain
        BOARD_MCU_CLOCK_GROUP2 - Enable clock for all group2 in mcu domain */
    uint8_t mcuClkGrp;
} Board_initParams_t;

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
Board_STATUS Board_getInitParams(Board_initParams_t *initParams);

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
Board_STATUS Board_setInitParams(Board_initParams_t *initParams);

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
 *
 * \return   TRUE if the given board is detected else FALSE.
 *
 */
bool Board_detectBoard(uint32_t boardID);

/**
 * \brief Function to get I2C configurations used by board
 *
 *  \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_getI2cInitConfig(Board_I2cInitCfg_t *i2cCfg);

/**
 * \brief Function to configure I2C configurations used by board
 *
 * This function is used to set the I2C controller instance and
 * SoC domain used by the board module for board ID info read.
 *
 * Usage:
 * Call Board_setI2cInitConfig to set the I2C configurations
 * Call Board ID info read/write
 *
 *  \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_setI2cInitConfig(Board_I2cInitCfg_t *i2cCfg);

/**
 * \brief Function to get the SoC domain
 *
 *  This function returns the domain of the SoC core on which
 *  it is executing.
 *
 * \return   SoC domain of the core.
 *
 */
uint32_t Board_getSocDomain(void);

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
void BOARD_delay(uint32_t usecs);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BOARD_UTILS_H_ */
