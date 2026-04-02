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
 *  \file   main.c
 *
 *  \brief  Master Mode application main file.
 *          Master performs read operation from slave device (Board-to-Board communication)
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <ti/osal/osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include "i2c_master_slave_cfg.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

char transferData[I2C_TRANSFER_LENGTH] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                          0x09, 0x0A};

/* Stack for application task */
uint8_t gReadAppStack[APP_TRANSFER_TASK_STACK_SIZE];

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static bool I2cApp_masterRead()
{
    I2C_Params      i2cParams;
    I2C_Handle      handle = NULL;
    I2C_Transaction i2cTransaction;
    char            txBuf[I2C_SLAVE_CMD_SIZE] = {0xAB, };
    char            rxBuf[I2C_TRANSFER_LENGTH];
    int16_t         status;
    bool            testStatus = BTRUE;

    I2cApp_initConfig(I2C_INSTANCE_ID);

    I2C_Params_init(&i2cParams);

    handle = I2C_open(I2C_INSTANCE_ID, &i2cParams);
    if (NULL == handle)
    {
        UART_printf("\nMaster Mode: I2C Open Failed.\r");
        testStatus = BFALSE;
    }

    /* Write command to Slave device */
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = I2C_SLAVE_CMD_SIZE;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nMaster Mode: I2C Write Command Transfer Failed.\r");
        testStatus = BFALSE;
    }
    else
    {
        UART_printf("\nMaster Mode: I2C Write Command Transfer Successful.\r");
        /* Request data from the slave */
        memset(rxBuf, 0, I2C_TRANSFER_LENGTH);
        I2C_transactionInit(&i2cTransaction);
        i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
        i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
        i2cTransaction.readCount = I2C_TRANSFER_LENGTH;
        i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
        status = I2C_transfer(handle, &i2cTransaction);
        if(I2C_STS_SUCCESS != status)
        {
            UART_printf("\nMaster Mode: I2C Read Command Failed.\r");
            testStatus = BFALSE;
        }
        else
        {
            UART_printf("\nMaster Mode: Received data from slave : ");
            for(uint32_t i = 0;i < I2C_TRANSFER_LENGTH;i++)
            {
                UART_printf("%x ",rxBuf[i]);
            }
            testStatus = I2cApp_compareData(&transferData[0], &rxBuf[0], I2C_TRANSFER_LENGTH);
            if(BFALSE == testStatus)
            {
                UART_printf("\nMaster Mode: Data Mismatch.\r");
                testStatus = BFALSE;
            }
            else
            {
                UART_printf("\nMaster Mode: Successfully received data.\r");              
            }
        }
        
    }

    if (handle)
    {
        I2C_close(handle);
    }

    return (testStatus);
}

void I2cApp_masterTest(void *arg0, void *arg1)
{
    if (BFALSE == I2cApp_boardInit())
    {
        UART_printf("\r\nBoard Init failed! \r\n");
    }
    else
    {
        /* Read request by master */
        if (BTRUE == I2cApp_masterRead())
        {
            UART_printf("\r\n\nMaster Mode: Test passed! \r\n");
        }
        else
        {
            UART_printf("\r\n\nMaster Mode: Test failed! \r\n");
        }
    }
    while (BTRUE)
    {
    }
}

int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init ();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);

    /* Set the task priority higher than the default priority (1) */
    taskParams.priority = 2;
    taskParams.stacksize = 0x8000;
    taskParams.stack = (void *) gReadAppStack;

    task = TaskP_create(&I2cApp_masterTest, &taskParams);
    if (NULL == task) {
        OS_stop();
        OSAL_Assert(NULL == task);
    }

    /* Start RTOS */
    OS_start();
    return (0);
}


