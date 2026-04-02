/*
 * Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   i2c_slave_extended_testapp.c
 *
 *  \brief  Slave Mode extended testapp file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/i2c/soc/i2c_soc.h>

#if defined (SOC_J721S2)
#include <ti/board/src/j721s2_evm/J721S2_pinmux.h>
#include <ti/board/src/j721s2_evm/include/board_cfg.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* I2C definitions */
#define I2C_SLAVE_CMD_SIZE            (2U)              /* in bytes */
#define I2C_SLAVE_ADDR                (0x71U)           /* I2C own slave address */
#define I2C_TRANSFER_LENGTH           (10U)             /* Read/write length in bytes */
#define I2C_INSTANCE_ID               (1U)
#define I2C_TRANSACTION_TIMEOUT       (10000U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

char gTransferData[I2C_TRANSFER_LENGTH] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                          0x09, 0x0A};

char readCommand[I2C_SLAVE_CMD_SIZE] = {0xAB};

volatile uint32_t gI2CAppCompleteCallbackFlag = UTRUE;

/* ========================================================================== */
/*                            Function Declarations                           */
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
 * \param  expData Pointer to Expected data
 * \param  rxData  Pointer to Received data
 * \param  length  Length of data
 *
 * \return BTRUE for success, BFALSE for failure
 */
bool I2cApp_compareData(char *expData, char *rxData, uint32_t length);

/* ========================================================================== */
/*                     External Function Declarations                         */
/* ========================================================================== */

extern Board_STATUS Board_pinmuxUpdate (pinmuxBoardCfg_t *pinmuxData,
                                 uint32_t domain);

/* ========================================================================== */
/*                          Structure Declarations                              */
/* ========================================================================== */

#if defined (SOC_J721S2)
    static pinmuxPerCfg_t gMcu_i3c0PinCfg[] =
    {
        /* MyMCU_I3C1 -> MCU_I2C1_SCL -> F24 */
        {
            PIN_WKUP_GPIO0_8, PIN_MODE(0) | \
            ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
        },
        /* MyMCU_I3C1 -> MCU_I2C1_SDA -> H26 */
        {
            PIN_WKUP_GPIO0_9, PIN_MODE(0) | \
            ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
        },
        {PINMUX_END}
    };
    
    static pinmuxModuleCfg_t gMcu_i3cPinCfg[] =
    {
        {0, TRUE, gMcu_i3c0PinCfg},
        {PINMUX_END}
    };
    
    pinmuxBoardCfg_t gMcuI2cPinmuxData[] =
    {
        {0, gMcu_i3cPinCfg},
        {PINMUX_END}
    };
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void I2CApp_callBackFxn(I2C_Handle handle, I2C_Transaction * transaction, int16_t transferStatus)
{
    gI2CAppCompleteCallbackFlag = UFALSE;
}

bool I2cApp_boardInit(void)
{
    bool retVal = BTRUE;
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (BOARD_SOK != boardStatus)
    {
        retVal = BFALSE;
    }

    /* Pinmux for MCU_I2C1 */
    Board_pinmuxUpdate(gMcuI2cPinmuxData, BOARD_SOC_DOMAIN_WKUP);
    
    I2C_init();

    UART_printf("\nI2C : Using Instance %d", I2C_INSTANCE_ID);
    return retVal;
}

void I2cApp_initConfig(uint32_t instance)
{
    I2C_HwAttrs   i2c_cfg;

    /* Get the default I2C init configurations */
    I2C_socGetInitCfg(instance, &i2c_cfg);

    i2c_cfg.enableIntr = BTRUE;

    /* Set the I2C configurations */
    I2C_socSetInitCfg(instance, &i2c_cfg);
}

bool I2cApp_compareData(char *expData, char *rxData, uint32_t length)
{
    uint32_t idx = 0U;
    uint32_t match = UTRUE;
    bool retVal = BFALSE;

    for(idx = 0U; ((idx < length) && (UFALSE != match)); idx++)
    {
        if(*expData != *rxData) match = UFALSE;
        expData++;
        rxData++;
    }

    if(UTRUE == match) retVal = BTRUE;

    return retVal;
}

int main(void)
{
    I2C_Params         i2cParams;
    I2C_Handle         handle = NULL;
    I2C_HwAttrs       *hwAttrs = NULL;
    I2C_Transaction    i2cTransaction;
    I2C_Transaction    i2cTransactionNext;
    char               txBuf[I2C_SLAVE_CMD_SIZE] = {0xAB};
    char               rxBuf[I2C_SLAVE_CMD_SIZE];
    int16_t            status;
    bool               testStatus = BTRUE;
    uint32_t           retVal;
    uint8_t            putData;
    uint8_t            getData;
    I2C_HwAttrs        i2cConfig;

    if (BFALSE == I2cApp_boardInit())
    {
        UART_printf("\r\nBoard Init failed! \r\n");
    }
    
    I2cApp_initConfig(I2C_INSTANCE_ID);

    I2C_Params_init(&i2cParams);

    handle = I2C_open(I2C_INSTANCE_ID, &i2cParams);
    if (NULL == handle)
    {
        UART_printf("\nSlave Mode: I2C Open Failed.\r");
        testStatus = BFALSE;
    }

    /* Read command sent by master device */
    memset(rxBuf, 0, I2C_SLAVE_CMD_SIZE);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = I2C_SLAVE_CMD_SIZE;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransaction.masterMode = BFALSE;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nSlave Mode: I2C Read Command Failed.\r");
        testStatus = BFALSE;
    }
    else
    {
        testStatus = I2cApp_compareData(&readCommand[0], &rxBuf[0], I2C_SLAVE_CMD_SIZE);
        if(BFALSE == testStatus)
        {
            UART_printf("\nSlave Mode: Incorrect command\r");
        }
        else
        {
            /* Write requested data to master */
            I2C_transactionInit(&i2cTransaction);
            i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
            i2cTransaction.writeBuf = (uint8_t *)&gTransferData[0];
            i2cTransaction.writeCount = I2C_TRANSFER_LENGTH;
            i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
            i2cTransaction.masterMode = BFALSE;

            status = I2C_transfer(handle, &i2cTransaction);
            if(I2C_STS_SUCCESS != status)
            {
                UART_printf("\nSlave Mode: I2C Write Data Transfer Failed.\r");
                testStatus = BFALSE;
            }
            else
            {
                UART_printf("\nSlave Mode: Write command received\r");
                UART_printf("\nSlave Mode: Successfully transmitted data.\r");

                /* TC:1 Slave Receiver Mode, readCount = 0 */
                UART_printf("\n\n [TC:1] Slave Receiver Mode, readCount = 0 \r\n");

                /* Read data from master */
                memset(rxBuf, 0, I2C_SLAVE_CMD_SIZE);
                I2C_transactionInit(&i2cTransaction);
                i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
                i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
                i2cTransaction.readCount = 1U; // To achieve readCount = 0
                i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
                i2cTransaction.masterMode = BFALSE;
                status = I2C_transfer(handle, &i2cTransaction);
                if(I2C_STS_SUCCESS != status)
                {
                    UART_printf("\nSlave Mode: I2C Read Data Transfer Failed.\r");
                    testStatus = BFALSE;
                }
                else
                {
                    UART_printf("\nSlave Mode: Successfully received data.\r");
                }
            }
        }
    }

    /* TC:2 Repeated Start condition */
    UART_printf("\n\n [TC:2] Repeated Start condition\r\n");

    memset(rxBuf, 0, I2C_SLAVE_CMD_SIZE);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = I2C_SLAVE_CMD_SIZE;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = I2C_SLAVE_CMD_SIZE;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransaction.masterMode = BFALSE;
    status = I2C_transfer(handle, &i2cTransaction);
    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nSlave Mode: I2C Transfer Failed.\r");
    }
    else
    {
        UART_printf("\nSlave Mode: I2C Transfer Successful.\r");
    }

    /* TC:3 Repeated Start condition with CSL_I2C_INT_TRANSMIT_UNDER_FLOW */
    UART_printf("\n\n [TC:3] Repeated Start condition with CSL_I2C_INT_TRANSMIT_UNDER_FLOW\r\n");

    memset(rxBuf, 0, I2C_SLAVE_CMD_SIZE);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = 0U;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = I2C_SLAVE_CMD_SIZE;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransaction.masterMode = BFALSE;
    status = I2C_transfer(handle, &i2cTransaction);
    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nSlave Mode: I2C Transfer Failed.\r");
    }
    else
    {
        UART_printf("\nSlave Mode: I2C Transfer Successful.\r");
    }

    if (handle)
    {
        I2C_close(handle);
    }

    /* TC:4 Repeated Start condition with Multiple TX from Slave to Master */
    UART_printf("\n\n [TC:4] Repeated Start condition with Multiple TX from Slave to Master\r\n");

    /* Get the default I2C init configurations */
    I2C_socGetInitCfg(I2C_INSTANCE_ID, &i2cConfig);
    i2cConfig.enableIntr = BFALSE; // Polling mode
    /* Set the I2C configurations */
    I2C_socSetInitCfg(I2C_INSTANCE_ID, &i2cConfig);    

    I2C_Params_init(&i2cParams);

    i2cParams.transferMode = I2C_MODE_CALLBACK;
    i2cParams.transferCallbackFxn = I2CApp_callBackFxn;

    handle = I2C_open(I2C_INSTANCE_ID, &i2cParams);
    if (NULL == handle)
    {
        UART_printf("\nSlave Mode: I2C Open Failed.\r");
        testStatus = BFALSE;
    }

    /* Transfer 1 */
    memset(rxBuf, 0, I2C_SLAVE_CMD_SIZE);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = 0U;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = I2C_SLAVE_CMD_SIZE;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransaction.masterMode = BFALSE;

    /* Transfer 2 */
    I2C_transactionInit(&i2cTransactionNext);
    i2cTransactionNext.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransactionNext.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransactionNext.writeCount = I2C_SLAVE_CMD_SIZE;
    i2cTransactionNext.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransactionNext.readCount = I2C_SLAVE_CMD_SIZE;
    i2cTransactionNext.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransactionNext.masterMode = BFALSE;

    status = I2C_transfer(handle, &i2cTransaction);
    status = I2C_transfer(handle, &i2cTransactionNext);

    while(UTRUE == gI2CAppCompleteCallbackFlag)
    {
        /* Wait for transaction to complete. */
    }

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nSlave Mode: I2C Write Data Transfer Failed.\r");
        testStatus = BFALSE;
    }
    else
    {
        UART_printf("\nSlave Mode: Successfully transmitted data.\r");
    }

    if (handle)
    {
        I2C_close(handle);
    }

    /* TC:5 Trasnmit from Slave to achieve writeCount=0U */
    UART_printf("\n\n [TC:5] Trasnmit from Slave to achieve writeCount=0U\r\n");
    
    I2cApp_initConfig(I2C_INSTANCE_ID);

    I2C_Params_init(&i2cParams);
    
    handle = I2C_open(I2C_INSTANCE_ID, &i2cParams);
    if (NULL == handle)
    {
        UART_printf("\nSlave Mode: I2C Open Failed.\r");
        testStatus = BFALSE;
    }

    /* Write data to the master */
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = 1U;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransaction.masterMode = BFALSE;
    status = I2C_transfer(handle, &i2cTransaction);
    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nSlave Mode: I2C Transfer Failed.\r");
        testStatus = BFALSE;
    }
    else
    {
        UART_printf("\nSlave Mode: I2C Transfer Successful.\r");
    }

    if (handle)
    {
        I2C_close(handle);
    }

    /* TC:6 Transmit from Slave to achieve writeCount=0U with Back to Back Transfers */
    UART_printf("\n\n [TC:6] Transmit from Slave to achieve writeCount=0U with Back to Back Transfers\r\n");

    /* Get the default I2C init configurations */
    I2C_socGetInitCfg(I2C_INSTANCE_ID, &i2cConfig);
    i2cConfig.enableIntr = BFALSE; // Polling mode
    /* Set the I2C configurations */
    I2C_socSetInitCfg(I2C_INSTANCE_ID, &i2cConfig);

    I2C_Params_init(&i2cParams);

    i2cParams.transferMode = I2C_MODE_CALLBACK;
    i2cParams.transferCallbackFxn = I2CApp_callBackFxn;

    handle = I2C_open(I2C_INSTANCE_ID, &i2cParams);
    if (NULL == handle)
    {
        UART_printf("\nSlave Mode: I2C Open Failed.\r");
        testStatus = BFALSE;
    }

    /* Transfer 1 */
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = 1U;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransaction.masterMode = BFALSE;

    /* Transfer 2 */
    I2C_transactionInit(&i2cTransactionNext);
    i2cTransactionNext.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransactionNext.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransactionNext.writeCount = I2C_SLAVE_CMD_SIZE;
    i2cTransactionNext.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransactionNext.masterMode = BFALSE;

    status = I2C_transfer(handle, &i2cTransaction);
    status = I2C_transfer(handle, &i2cTransactionNext);

    while(UTRUE == gI2CAppCompleteCallbackFlag)
    {
        /* Wait for transaction to complete. */
    }

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nSlave Mode: I2C Data Transfer Failed.\r");
        testStatus = BFALSE;
    }
    else
    {
        UART_printf("\nSlave Mode: Successful data transfer.\r");
    }

    if (handle)
    {
        I2C_close(handle);
    }

    /* TC:7 10-bit addressing Mode */
    UART_printf("\n\n [TC:7] 10-bit addressing Mode\r\n");

    I2cApp_initConfig(I2C_INSTANCE_ID);

    I2C_Params_init(&i2cParams);

    handle = I2C_open(I2C_INSTANCE_ID, &i2cParams);
    if (NULL == handle)
    {
        UART_printf("\nSlave Mode: I2C Open Failed.\r");
        testStatus = BFALSE;
    }

    /* Write data to the master */
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&gTransferData[0];
    i2cTransaction.writeCount = I2C_TRANSFER_LENGTH;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransaction.masterMode = BFALSE;
    i2cTransaction.expandSA = BTRUE;
    status = I2C_transfer(handle, &i2cTransaction);
    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nSlave Mode: I2C Write Data Transfer Failed.\r");
    }
    else
    {
        UART_printf("\nSlave Mode: Successfully transmitted data.\r");
    }

    if (handle)
    {
        I2C_close(handle);
    }

    /* TC:8 HS Mode */
    UART_printf("\n\n [TC:8] High Speed Mode: 1MHz\r\n");

    I2cApp_initConfig(I2C_INSTANCE_ID);

    I2C_Params_init(&i2cParams);

    i2cParams.bitRate = I2C_1P0Mhz;

    handle = I2C_open(I2C_INSTANCE_ID, &i2cParams);
    if (NULL == handle)
    {
        UART_printf("\nSlave Mode: I2C Open Failed.\r");
        testStatus = BFALSE;
    }

    /* Write data to the master */
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&gTransferData[0];
    i2cTransaction.writeCount = I2C_TRANSFER_LENGTH;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransaction.masterMode = BFALSE;
    status = I2C_transfer(handle, &i2cTransaction);
    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nSlave Mode: I2C Write Data Transfer Failed.\r");
    }
    else
    {
        UART_printf("\nSlave Mode: Successfully transmitted data.\r");
    }

    if (handle)
    {
        I2C_close(handle);
    }

    /* TC:9 Polling Mode */
    UART_printf("\n\n [TC:9] Polling Mode \r\n");

    I2C_socGetInitCfg(I2C_INSTANCE_ID, &i2cConfig);
    i2cConfig.enableIntr = BFALSE; // Polling mode
    I2C_socSetInitCfg(I2C_INSTANCE_ID, &i2cConfig);

    I2C_Params_init(&i2cParams);

    handle = I2C_open(I2C_INSTANCE_ID, &i2cParams);
    if (NULL == handle)
    {
        UART_printf("\nSlave Mode: I2C Open Failed.\r");
        testStatus = BFALSE;
    }

    /* Write data to the master */
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_SLAVE_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&gTransferData[0];
    i2cTransaction.writeCount = I2C_TRANSFER_LENGTH;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    i2cTransaction.masterMode = BFALSE;
    status = I2C_transfer(handle, &i2cTransaction);
    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("\nSlave Mode: I2C Write Data Transfer Failed.\r");
    }
    else
    {
        /* Polling mode not supported - No transfer to check */
        UART_printf("\nSlave Mode: Successfully transmitted data.\r");
    }

    /* TC:10 Getting Slave Interrupt status */
    UART_printf("\n\n [TC:10] Getting Slave Interrupt status \r\n");

    hwAttrs = (I2C_HwAttrs *)handle->hwAttrs;

    retVal = I2CSlaveIntStatus(hwAttrs->baseAddr);
    if (0U == retVal)
    {
        UART_printf("\nI2CSlaveIntStatus: No Interrupt\r");
    }
    else
    {
        UART_printf("\nI2CSlaveIntStatus: %d\n",retVal);
    }

    /* TC:11 Checking Interrupt Status for Bus Busy Interrupt */
    UART_printf("\n\n [TC:11] Checking Interrupt Status for Bus Busy Interrupt \r\n");

    retVal = I2CSlaveIntStatusEx(hwAttrs->baseAddr, CSL_I2C_INT_BUS_BUSY);
    if(0U != retVal)
    {
        UART_printf("\nI2CSlaveIntStatusEx: Bus Busy\r");
    }
    else
    {
        UART_printf("\nI2CSlaveIntStatusEx: Bus Free\r");
    }
    
    /* TC:12 Checking Interrupt Raw Status for Bus Busy Interrupt */
    UART_printf("\n\n [TC:12] Checking Interrupt Raw Status for Bus Busy Interrupt \r\n");

    retVal = I2CSlaveIntRawStatusEx(hwAttrs->baseAddr, CSL_I2C_INT_BUS_BUSY);
    if(0U != retVal)
    {
        UART_printf("\nI2CSlaveIntRawStatusEx: Bus Busy\r");
    }
    else
    {
        UART_printf("\nI2CSlaveIntRawStatusEx: Bus Free\r");
    }
    
    /* TC:13 Clear Interrupt Raw Status for Bus Busy Interrupt */
    UART_printf("\n\n [TC:13] Clear Interrupt Raw Status for Bus Busy Interrupt \r");

    I2CSlaveIntRawStatusClearEx(hwAttrs->baseAddr, CSL_I2C_INT_BUS_BUSY);

    /* TC:14 Get Own address being used */
    UART_printf("\n\n [TC:14] Get Own address being used \r\n");

    retVal = I2CActiveOwnAddressGet(hwAttrs->baseAddr);
    {
        UART_printf("\nI2CActiveOwnAddressGet: Address = %d\r",retVal);
    }

    /* TC:15 Set Own address to be used */
    UART_printf("\n\n [TC:15] Set Own address to be used \r");

    /* Set I2C own address 0 */
    I2COwnAddressSet(hwAttrs->baseAddr,hwAttrs->ownSlaveAddr[0],CSL_I2C_OWN_ADDR_0);

    /* Set I2C own address 1 */
    I2COwnAddressSet(hwAttrs->baseAddr,hwAttrs->ownSlaveAddr[1],CSL_I2C_OWN_ADDR_1);
    
    /* Set I2C own address 2 */
    I2COwnAddressSet(hwAttrs->baseAddr,hwAttrs->ownSlaveAddr[2],CSL_I2C_OWN_ADDR_2);

    /* Set I2C own address 3 */
    I2COwnAddressSet(hwAttrs->baseAddr,hwAttrs->ownSlaveAddr[3],CSL_I2C_OWN_ADDR_3);

    /* Set I2C own address for default case */
    I2COwnAddressSet(hwAttrs->baseAddr,hwAttrs->ownSlaveAddr[3],4U);
                  
    /* TC:16 Unblock clock for all addresses */
    UART_printf("\n\n [TC:16] Unblock clock for all addresses \r");

    I2CClockBlockingControl(hwAttrs->baseAddr,0U,0U,0U,0U);

    /* TC:17 Transmit a byte in I2C slave mode */
    UART_printf("\n\n [TC:17] Transmit a byte in I2C slave mode \r");

    putData = 127U;
    I2CSlaveDataPut(hwAttrs->baseAddr,putData);
    
    /* TC:18 Receive a byte in I2C slave mode */
    UART_printf("\n\n [TC:18] Receive a byte in I2C slave mode \r\n");

    getData = I2CSlaveDataGet(hwAttrs->baseAddr);
    UART_printf("\nI2CSlaveDataPut = %d, I2CSlaveDataGet = %d\r\n",putData,getData);

    if (handle)
    {
        I2C_close(handle);
    }

    #if defined LDRA_DYN_COVERAGE_EXIT
    UART_printf("\n LDRA ENTRY \n");
    upload_execution_history();
    UART_printf("\n LDRA EXIT \n");
    #endif

    if (BTRUE == testStatus)
    {
        UART_printf("\n All tests have passed. \n");
    }
    else
    {
        UART_printf("\n Some tests have failed. \n");
    }

    return 0;
}