/******************************************************************************
 * Copyright (c) 2018-2021 Texas Instruments Incorporated - http://www.ti.com
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
 *
 * \file   temeprature_test.h
 *
 * \brief  This is the header file for temperature sensor diagnostic test.
 *
 */
#ifndef _TEMPERATURE_TEST_H_
#define _TEMPERATURE_TEST_H_ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/csl/soc.h>

#include "board.h"
#include "board_cfg.h"

#if (defined(SOC_AM65XX) || defined(SOC_K2G) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_TPR12) || defined(SOC_AM64X) || defined(SOC_AWR294X) || defined(SOC_J721S2) || defined(SOC_J784S4))
#include "diag_common_cfg.h"
#endif

#if defined(SOC_J7200)
#include "board_utils.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Board specific definitions */
#if defined (EVM_K2G) || defined (EVM_AM335x) || defined (EVM_AM437x)
#define I2C_INSTANCE                  (1U)
#elif (defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_TPR12) || defined(SOC_AM64X) || defined(SOC_AWR294X) || defined(SOC_J721S2) || defined(SOC_J784S4))
#define I2C_INSTANCE                  (BOARD_TEMP_SENSOR_I2C_INSTANCE)
#else
#define I2C_INSTANCE                  (0U)
#endif

#if defined(evmK2G) || defined(evmAM335x) || defined(evmAM437x) || defined(evmAM571x) || defined(evmAM572x)
#define TEMP_SLAVE_DEVICE1_ADDR       (0x48U)
#elif (defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4))
#define TEMP_SLAVE_DEVICE1_ADDR       BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE1_ADDR
#define TEMP_SLAVE_DEVICE2_ADDR       BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE2_ADDR
#elif (defined(SOC_TPR12) || defined(SOC_AWR294X))
#define TEMP_SLAVE_DEVICE1_ADDR       (BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE_ADDR)
#endif

/**
 *  \brief    Generates a random delay.
 *
 *  \param    delayVal            [IN]   Delay count.
 *
 */ 
void BoardDiag_AppDelay(uint32_t delayVal);


/**
 *  \brief    The function performs the temperature sensor test by reading the
 *            temperate register and display it on to the serial console by
 *            converting it into degree centigrade.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_temperature_test(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TEMPERATURE_TEST_H_ */
