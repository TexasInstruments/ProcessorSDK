/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   i2c_master_slave_cfg.h
 *
 *  \brief  This file contains the common configurations for I2C Master-Slave
 *          communication.
 *
 */

#ifndef I2C_MASTER_SLAVE_CFG_H_
#define I2C_MASTER_SLAVE_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* I2C definitions */
#define I2C_SLAVE_CMD_SIZE            2                /* in bytes */
#define I2C_SLAVE_ADDR                0x71             /* I2C own slave address */
#define I2C_TRANSFER_LENGTH           10               /* Read/write length in bytes */
#define I2C_INSTANCE_ID               1
#define I2C_TRANSACTION_TIMEOUT       (10000U)

/* Stack size for application task */
#define APP_TRANSFER_TASK_STACK_SIZE  (0x8000)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Structure Declarations                           */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief  This function does Board Init and additional I2C Pinmux.
 *
 * \param  None
 *
 * \return BTRUE for success, BFALSE for failure
 */
bool I2cApp_boardInit(void);

/**
 * \brief  Function to set I2C configurations.
 *
 * \param  instance I2C Instance ID
 *
 * \return None
 */
void I2cApp_initConfig(uint32_t instance);

/**
 * \brief  This function compares received data with expected data.
 *
 * \param  expData Expected data 
 * \param  rxData  Received data
 * \param  length  Length of data
 *
 * \return BTRUE for success, BFALSE for failure
 */
bool I2cApp_compareData(char *expData, char *rxData, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* I2C_MASTER_SLAVE_CFG_H_ */
