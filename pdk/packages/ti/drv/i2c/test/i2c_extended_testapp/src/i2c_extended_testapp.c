
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
 *  \file   i2c_extended_testapp.c
 *
 *  \brief  Example application file. This application will read the data
 *          from eeprom and compares it with the known data.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================= */

#include <stdio.h>
#include <string.h>
#include <ti/csl/soc.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>

/* ========================================================================== */
/*                             Macros                                         */
/* ========================================================================== */

#define I2C_APP_TRANSACTION_TIMEOUT                  (4000U)
#define I2C_DELAY_USEC                               ((uint32_t) 250U)

#define I2C_APP_CALLBACK_TRANSFER_COUNT              5
#if defined (SOC_J784S4) || defined (SOC_J742S2)
#define I2C_APP_MAX_FREQ_COUNT                       5
#elif defined (SOC_J721S2)
#define I2C_APP_MAX_FREQ_COUNT                       3
#else
#define I2C_APP_MAX_FREQ_COUNT                       2
#endif
#define I2C_APP_BITRATE_INVALID_ID                   5
#define I2C_APP_BUFSTAT_INVALID_COMMAND              4

#define I2C_APP_ID_BIT_RATE_INTERRUPT_MODE           0   /* I2C bit rate test in interrupt mode */
#define I2C_APP_ID_BIT_RATE_POLLING_MODE             1   /* I2C bit rate test in polling mode */
#define I2C_APP_ID_BIT_RATE_CALLBACK_MODE            2   /* I2C bit rate test in callback mode */
#define I2C_APP_ID_PROBE_BUS_FREQ                    3   /* I2C probe bus frequency Test in interrupt mode  */
#define I2C_APP_ID_TIMEOUT_INT                       4   /* I2C timeout test in interrupt mode */
#define I2C_APP_ID_NULL_CHECK                        5   /* I2C NULL check*/

#define I2C_APP_EEPROM_INSTANCE                      BOARD_I2C_EEPROM_INSTANCE
#define I2C_APP_EEPROM_ADDR                          BOARD_I2C_EEPROM_ADDR
#define I2C_APP_EEPROM_TEST_ADDR                     0x0001           /* Read and write test start address */
#define I2C_APP_EEPROM_ADDR_SIZE                     2   /* in bytes */
#define I2C_APP_EEPROM_TEST_LENGTH                   10  /* Read (and write) length in bytes */

#define I2C_APP_IRQSTATUS_STC_MASK                   (0x00000040U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* Define the I2C test interface */
typedef struct I2CApp_TestCfg_s
{
    bool     (*testFunc)(void *);
    int32_t  testId;
    bool     dmaMode;
    bool     intrMode;
    bool     cbMode;
    uint32_t timeout;
    size_t   readcount;
    size_t   writecount;
    uint32_t slaveAddress;
    uint8_t  testDesc[80];
} I2CApp_TestCfg;

/* ========================================================================== */
/*                            Functions Declarations                          */
/* ========================================================================== */

static bool I2CApp_boardInitI2C(void);
static bool I2CApp_probeBusFreqTest(void *arg);
static bool I2CApp_nullTest(void *arg);
static bool I2CApp_bitrateTest(void *arg);
static bool I2CApp_bitrateTestTxnInvdParams(void *arg);
static bool I2CApp_bitrateTestFrequency(I2C_BitRate bitRate, I2CApp_TestCfg *test);
static bool I2CApp_compareData(uint8_t *expData, uint8_t *rxData, uint32_t length);
static void I2CApp_initConfig(uint32_t instance, I2CApp_TestCfg *test);
static bool I2CApp_callBackTest(void *arg);
static bool I2CApp_callBackTransactionTest(void *arg);
static void I2CApp_callBackFxn(I2C_Handle handle, I2C_Transaction * transaction, int16_t transferStatus);
static bool I2CApp_callBackBusRecoveryTest(void * arg);
static bool I2CApp_receiveOverrunTest(void *arg);
static bool I2CApp_negativeTest(void *arg);
static bool I2CApp_wakeupEnableDiasbleTest(void *arg);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t gI2CAppCompleteCallbackFlag = UTRUE;
#if defined (SOC_J784S4)
uint8_t gI2CApp_EepromData[I2C_APP_EEPROM_TEST_LENGTH] = {85, 51, 238, 1, 11, 1, 16, 46, 0, 74};
#elif defined (SOC_J742S2)
uint8_t gI2CApp_EepromData[I2C_APP_EEPROM_TEST_LENGTH] = {85, 51, 238, 1, 1, 1, 16, 46, 0, 74};
#elif defined (SOC_J721S2)
uint8_t gI2CApp_EepromData[I2C_APP_EEPROM_TEST_LENGTH] = {85, 51, 238, 1, 60, 0, 16, 46, 0, 74};
#else
uint8_t gI2CApp_EepromData[I2C_APP_EEPROM_TEST_LENGTH] = {85, 51, 238, 1, 55, 0, 16, 46, 0, 74};
#endif

I2CApp_TestCfg gI2cApp_Tests[] =
{
    /*{testFunc,                      testID,                           dma,    intr,   cbMode, timeout,                 readcount,                  writecount,               slaveaddress,        testDesc} */
    {I2CApp_bitrateTest,              I2C_APP_ID_BIT_RATE_POLLING_MODE, BFALSE, BFALSE, BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in polling"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_POLLING_MODE, BFALSE, BFALSE, BFALSE, SemaphoreP_WAIT_FOREVER, 0, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in polling readCount=0"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_POLLING_MODE, BFALSE, BFALSE, BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, 0,I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in polling writeCount=0"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_POLLING_MODE, BFALSE, BFALSE, BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE,0x60, "\r\n I2C bit rate test in polling invalid slave"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_POLLING_MODE, BFALSE, BFALSE, BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, 0, 0x60, "\r\n I2C bit rate test in polling invalid slave"},

    {I2CApp_bitrateTest,              I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, BFALSE, BTRUE, BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in interrupt mode"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, BFALSE, BTRUE, BFALSE, 0U, 0, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in interrupt mode readCount=0, timeout=0"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, BFALSE, BTRUE, BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, 0, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in interrupt mode WriteCount=0"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, BFALSE, BTRUE, BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, 0x60, "\r\n I2C bit rate test in interrupt mode invalid slave address"},

    {I2CApp_callBackTest,             I2C_APP_ID_BIT_RATE_CALLBACK_MODE,  BFALSE, BTRUE,  BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in callback mode"},
    {I2CApp_callBackTransactionTest,  I2C_APP_ID_BIT_RATE_CALLBACK_MODE,  BFALSE, BTRUE,  BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in callback mode Transaction"},
    {I2CApp_callBackBusRecoveryTest,  I2C_APP_ID_BIT_RATE_CALLBACK_MODE,  BFALSE, BTRUE,  BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C Bus Recovery test in callback mode"},

    {I2CApp_probeBusFreqTest,         I2C_APP_ID_PROBE_BUS_FREQ,          BFALSE, BTRUE,  BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C probe bus frequency test in interrupt mode"},
    {I2CApp_receiveOverrunTest,       I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, BFALSE, BTRUE,  BFALSE, SemaphoreP_WAIT_FOREVER, 64, 0, I2C_APP_EEPROM_ADDR, "\r\n I2C Receive Overrun Test"},
    {I2CApp_negativeTest,             I2C_APP_ID_BIT_RATE_POLLING_MODE,   BFALSE, BFALSE, BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bitrate NegativeTest"},
    {I2CApp_wakeupEnableDiasbleTest,  I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, BFALSE, BTRUE,  BFALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C WakeUP enable and disable test"},
	{I2CApp_nullTest,                 I2C_APP_ID_NULL_CHECK,              BFALSE, BFALSE, BFALSE, 1, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C null check test"},
    {NULL, },
};

/* ========================================================================== */
/*                            Function Definitions                            */
/* ========================================================================== */

void I2CApp_testPrintTestDesc(I2CApp_TestCfg *test)
{
    uint8_t        testId[16] = {0, };

    /* Print unit test ID */
    sprintf((char*)testId, "%d", test->testId);
    UART_printf("\n I2C UT %s\n", testId);

    /* Print test description */
    UART_printf("\n %s\n", test->testDesc);
}

int I2C_test()
{
    bool       testResult = BTRUE;
    uint32_t   testCount;
    I2CApp_TestCfg *test;

    if(BFALSE == I2CApp_boardInitI2C())
    {
        return(0);
    }

    for(testCount = 0u; ; testCount++)
    {
        test = &gI2cApp_Tests[testCount];
        if(NULL == test->testFunc)
        {
            break;
        }
        I2CApp_testPrintTestDesc(test);
        if(BTRUE == test->testFunc((void *)test))
        {
            UART_printf("\r\n %s has passed\r\n", test->testDesc);
        }
        else
        {
            UART_printf("\r\n %s has failed\r\n", test->testDesc);
            testResult = BFALSE;
            break;
        }
    }

    #if defined LDRA_DYN_COVERAGE_EXIT
    UART_printf("\n LDRA ENTRY \n");
    upload_execution_history();
    UART_printf("\n LDRA EXIT \n");
    #endif

    if(BTRUE == testResult)
    {
        UART_printf("\n All tests have passed. \n");
    }
    else
    {
        UART_printf("\n Some tests have failed. \n");
    }

    return 0;
}

int main(void)
{
    I2C_test();
    return 0;
}

/* ========================================================================== */
/*                           Internal Function Definitions                    */
/* ========================================================================== */

static bool I2CApp_boardInitI2C(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    I2C_HwAttrs   i2cCfg;
    bool          status;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_UART_STDIO;

    boardStatus = Board_init(boardCfg);
    if(BOARD_SOK != boardStatus)
    {
        status = BFALSE;
    }
    else
    {
        status = BTRUE;
    }
    
    if(BTRUE == status)
    {
        I2C_init();
        /* Get the default I2C init configurations */
        I2C_socGetInitCfg(I2C_APP_EEPROM_INSTANCE, &i2cCfg);
        /* No I2C instanced connected to eeprom in main domain, use i2c instance in wakeup domain */
        i2cCfg.baseAddr = CSL_WKUP_I2C0_CFG_BASE;
        /* Pulsar R5 core is on the MCU domain */
        i2cCfg.intNum = CSLR_MCU_R5FSS0_CORE0_INTR_WKUP_I2C0_POINTRPEND_0;
        /* Set the default I2C init configurations */
        I2C_socSetInitCfg(I2C_APP_EEPROM_INSTANCE, &i2cCfg);
    }

    UART_printf("\n I2C Test: Using Instance %d", I2C_APP_EEPROM_INSTANCE);

    return (BTRUE);
}

static void I2CApp_initConfig(uint32_t instance, I2CApp_TestCfg *test)
{
    I2C_HwAttrs   i2cCfg;

    /* Get the default I2C init configurations */
    I2C_socGetInitCfg(instance, &i2cCfg);
    i2cCfg.enableIntr = test->intrMode;

    /* Set the SPI init configurations */
    I2C_socSetInitCfg(instance, &i2cCfg);
}

static bool I2CApp_compareData(uint8_t *expData, uint8_t *rxData, uint32_t length)
{
    uint32_t idx = 0U;
    uint32_t match = UTRUE;
    bool retVal = BFALSE;

    for(idx = 0U; ((idx < length) && (UFALSE != match)); idx++)
    {
        if(*expData != *rxData) 
        {
            match = UFALSE;
        }
        expData++;
        rxData++;
    }

    if(UTRUE == match)
    {
        retVal = BTRUE;
    }

    return retVal;
}

static bool I2CApp_bitrateTestFrequency(I2C_BitRate bitRate, I2CApp_TestCfg *test)
{
    I2C_Params      i2cParams;
    I2C_Handle      handle = NULL;
    I2C_Transaction i2cTransaction;
    uint8_t         txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x00, };
    uint8_t         rxBuf[I2C_APP_EEPROM_TEST_LENGTH];
    int16_t         status;
    bool            testStatus = BTRUE;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = bitRate;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = test->writecount;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = test->readcount;
    i2cTransaction.timeout   = I2C_APP_TRANSACTION_TIMEOUT;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = BFALSE;
    }
    else
    {
        testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        if(BFALSE == testStatus)
        {
            UART_printf("I2C Test: Data Mismatch. \n");
        }
    }

    if(handle)
    {
        I2C_close(handle);
    }

    return (testStatus);
}

static bool I2CApp_bitrateTestTxnInvdParams(void *arg)
{
    I2C_Params          i2cParams;
    I2C_Handle          handle = NULL;
    I2C_Transaction     i2cTransaction;
    uint8_t             txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x00, };
    uint8_t             rxBuf[I2C_APP_EEPROM_TEST_LENGTH];
    int16_t             status;
    bool                testStatus = BTRUE;
    I2CApp_TestCfg        *test = (I2CApp_TestCfg *)arg;
    I2C_HwAttrs const   *hwAttrs = NULL;
    uint32_t            retVal;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }
    /* EEPROM write disabled on K2, need copy data */
    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = test->writecount;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = test->readcount;
    i2cTransaction.timeout   = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    /* check master is busy */
    hwAttrs = (I2C_HwAttrs const *)handle->hwAttrs;
    retVal = I2CMasterBusy(hwAttrs->baseAddr);
    if(0U != retVal)
    {
        retVal = I2CMasterBusBusy(hwAttrs->baseAddr);
        if(0U != retVal)
        {
           UART_printf("I2C Test: MASTER BUS BUSY. \n");
           testStatus = BFALSE;
        }
    }

    /* check for invalid salve if NACK bit SET */
    retVal = I2CMasterErr(hwAttrs->baseAddr);
    if(0x02U == retVal)
    {
        UART_printf("I2C Test: NACK.\n");
    }

    /* check Arbitration bit */
    retVal = I2CMasterIntStatusEx(hwAttrs->baseAddr, 0x01);
    if(0U != retVal)
    {
        UART_printf("I2C Test: Arbitration LOST.\n");
        testStatus = BFALSE;
    }

    /* check the buffer status */
    retVal = I2CBufferStatus(hwAttrs->baseAddr, CSL_I2C_RX_BUFFER_STATUS);
    if(0U != retVal)
    {
        UART_printf("I2C Test:RX buffer Status :%d \n", retVal);
        testStatus = BFALSE;
    }

    /* check the buffer by passing invalid FLag */
    retVal = I2CBufferStatus(hwAttrs->baseAddr, I2C_APP_BUFSTAT_INVALID_COMMAND);
    if(0U != retVal)
    {
        UART_printf("I2C Test: Invalid case :%d \n", retVal);
        testStatus = BFALSE;
    }

    /* clear and check the interrupt status */
    I2CMasterIntRawStatusClearEx(hwAttrs->baseAddr, 0U);
    I2CMasterIntClearEx(hwAttrs->baseAddr, CSL_I2C_INT_ALL);
    retVal = I2CMasterIntRawStatus(hwAttrs->baseAddr);
    if(0U != retVal)
    {
        UART_printf("I2C Test: Interrupt Not cleared :%d \n", retVal);
        testStatus = BFALSE;
    }

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = BFALSE;
        if(I2C_APP_EEPROM_ADDR != test->slaveAddress)
        {
            UART_printf("I2C Test: Invalid slave Test: PASS \n");
            testStatus = BTRUE;
        }
    }
    else
    {
        if(0 == test->readcount)
        {
            for(int i=0; i < I2C_APP_EEPROM_TEST_LENGTH; i++)
            {
                if(0U != rxBuf[i])
                {
                    UART_printf("I2C Test: Data Mismatch \n");
                    testStatus = BFALSE;
                }
           }
        }
        else
        {
            testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
            if(BFALSE == testStatus)
            {
                UART_printf("I2C Test: Data Mismatch \n");
            }
        }
    }

    if(handle)
    {
        I2C_close(handle);
    }

    return (testStatus);
}

static bool I2CApp_bitrateTest(void *arg)
{
    bool       testResult = BTRUE;
    uint32_t   i;

    for(i = 0; i < I2C_APP_MAX_FREQ_COUNT; i++)
    {
        testResult = I2CApp_bitrateTestFrequency((I2C_BitRate)i, (I2CApp_TestCfg *)arg);
        if(BFALSE == testResult)
        {
            break;
        }
    }

    return (testResult);
}

static void I2CApp_callBackFxn(I2C_Handle handle, I2C_Transaction * transaction, int16_t transferStatus)
{
    gI2CAppCompleteCallbackFlag = UFALSE;
}

static bool I2CApp_callBackTest(void *arg)
{
    I2C_Params          i2cParams;
    I2C_Handle          handle = NULL;
    I2C_Transaction     i2cTransaction;
    uint8_t             txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x0, 0x00, 10, 20, 30};
    uint8_t             rxBuf[I2C_APP_EEPROM_TEST_LENGTH+I2C_APP_EEPROM_ADDR_SIZE];
    int16_t             status;
    bool                testResult = BTRUE;
    I2CApp_TestCfg      *test = (I2CApp_TestCfg *)arg;
    uint8_t             count;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    i2cParams.transferMode = I2C_MODE_CALLBACK;
    i2cParams.transferCallbackFxn = I2CApp_callBackFxn;

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testResult = BFALSE;
    }

    /* Initiate multiple callback transactions */
    for(count = 0; count <= I2C_APP_CALLBACK_TRANSFER_COUNT; count++)
    {
        memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
        I2C_transactionInit(&i2cTransaction);
        i2cTransaction.slaveAddress = test->slaveAddress;
        i2cTransaction.writeBuf     = (uint8_t *)&txBuf[0];
        i2cTransaction.writeCount   = test->writecount;
        i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
        i2cTransaction.readCount    = test->readcount;
        i2cTransaction.timeout      = test->timeout;
        status = I2C_transfer(handle, &i2cTransaction);
    }

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testResult = BFALSE;
    }
    else
    {
        while(UTRUE == gI2CAppCompleteCallbackFlag)
        {
            /* Wait for transaction to complete. */
        }
        testResult = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        if(BFALSE == testResult)
        {
            UART_printf("I2C Test: Data Mismatch \n");
        }
    }
    if (handle)
    {
	   I2C_close(handle);
    }

    return (testResult);
}

static bool I2CApp_callBackTransactionTest(void *arg)
{
    I2C_Params          i2cParams;
    I2C_Handle          handle = NULL;
    I2C_Transaction     i2cTransaction;
    I2C_Transaction     i2cTx;
    uint8_t             txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x0, 0x00, 10, 20, 30};
    uint8_t             rxBuf[I2C_APP_EEPROM_TEST_LENGTH+I2C_APP_EEPROM_ADDR_SIZE];
    int16_t             status;
    bool                testResult = BTRUE;
    I2CApp_TestCfg      *test = (I2CApp_TestCfg *)arg;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    i2cParams.transferMode = I2C_MODE_CALLBACK;
    i2cParams.transferCallbackFxn = I2CApp_callBackFxn;

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testResult = BFALSE;
    }

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf     = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount   = test->writecount;
    i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount    = test->readcount;
    i2cTransaction.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    /* Initiate another transaction */
    I2C_transactionInit(&i2cTx);
    i2cTx.slaveAddress = test->slaveAddress;
    i2cTx.writeBuf     = (uint8_t *)&txBuf[0];
    i2cTx.writeCount   = test->writecount;
    i2cTx.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTx.readCount    = test->readcount;
    i2cTx.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTx);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testResult = BFALSE;
    }
    else
    {
        while(UTRUE == gI2CAppCompleteCallbackFlag)
        {
            /* Wait for transaction to complete. */
        }
        UART_printf("callback Transaction completed.\n");
    }

    if (handle)
    {
	   I2C_close(handle);
    }

    return (testResult);
}

static void I2CApp_udelay(uint32_t delay)
{
    volatile uint32_t del = delay;

    while (0U != del)
    {
        del = del - 1U;
    }
}

static bool I2CApp_checkTimeout(uint32_t *pUsecCnt)
{
    bool timeout = BFALSE;

    *pUsecCnt = *pUsecCnt + 1U;
    if (1000U == *pUsecCnt)
    {
        *pUsecCnt = 0U;
        timeout = BTRUE;
    }

    return (timeout);
}

static bool I2CApp_callBackBusRecoveryTest(void *arg)
{
    I2C_Params          i2cParams;
    I2C_Handle          handle = NULL;
    I2C_Transaction     i2cTransaction;
    uint8_t             txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x0, 0x00, 10, 20, 30};
    uint8_t             rxBuf[I2C_APP_EEPROM_TEST_LENGTH+I2C_APP_EEPROM_ADDR_SIZE];
    int16_t             status;
    bool                testResult = BTRUE;
    I2CApp_TestCfg      *test = (I2CApp_TestCfg *)arg;
    uint32_t            delayValue;
    uint32_t            uSecTimeout = 0U;
    uint32_t            timeout = I2C_APP_TRANSACTION_TIMEOUT;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    i2cParams.transferMode = I2C_MODE_CALLBACK;
    i2cParams.transferCallbackFxn = I2CApp_callBackFxn;

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testResult = BFALSE;
    }

    /* Initiate Transaction */
    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf     = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount   = test->writecount;
    i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount    = test->readcount;
    i2cTransaction.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testResult = BFALSE;
    }
    else
    {
        while((UTRUE == gI2CAppCompleteCallbackFlag)  && (0 != timeout))
        {
            /* Wait for transaction to complete. */
            I2CApp_udelay(I2C_DELAY_USEC);
            if (I2CApp_checkTimeout(&uSecTimeout))
            {
                timeout--;
            }
        }
        if(0 == timeout)
        {
            UART_printf("Transaction timed out, perform bus recovery. \n");
            delayValue = 2000U;
            status = I2C_control(handle, I2C_CMD_RECOVER_BUS, &delayValue);
            if(I2C_STATUS_SUCCESS == status)
            {
                memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
                I2C_transactionInit(&i2cTransaction);
                i2cTransaction.slaveAddress = test->slaveAddress;
                i2cTransaction.writeBuf     = (uint8_t *)&txBuf[0];
                i2cTransaction.writeCount   = test->writecount;
                i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
                i2cTransaction.readCount    = test->readcount;
                i2cTransaction.timeout      = test->timeout;
                status = I2C_transfer(handle, &i2cTransaction);

                if(I2C_STS_SUCCESS != status)
                {
                    UART_printf(" Bus recovery test failed. \n");
                    testResult = BFALSE;
                }
            }
            else
            {
                testResult = BFALSE;
            }
        }
        else
        {
            UART_printf(" Callback Transaction completed.\n");
        }
    }

    if (handle)
    {
	   I2C_close(handle);
    }

    return (testResult);
}

static bool I2CApp_nullTest(void *arg)
{
    I2C_Params          i2cParams;
    I2C_Handle          handle = NULL;
    I2CApp_TestCfg      *test = (I2CApp_TestCfg *)arg;
    I2C_v1_Object       *object = NULL;
    I2C_Transaction     testHeadPtr;
    bool                testResult = BTRUE;
    I2C_Transaction     i2cTransaction;
    uint32_t            busFrequency = I2C_400kHz;

    /*null check for i2c_drv.c file*/
    I2C_init();
    I2C_Params_init(NULL);
    I2C_open(I2C_APP_EEPROM_INSTANCE, NULL);
    I2C_transfer(NULL, &i2cTransaction);
    I2C_control(NULL, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);
    I2C_close(NULL);

    /* Null check test for i2c_api.c file */
    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
       testResult = BFALSE;
    }

    /* Headptr null check */
    object = handle->object;
    object->headPtr = &testHeadPtr;
    I2C_close(handle);

    return (testResult);
}

static bool I2CApp_probeBusFreqTest(void *arg)
{
    I2C_Handle      handle;
    I2C_Params      i2cParams;
    uint32_t        busFrequency;
    int16_t         transferStatus;
    I2C_Transaction i2cTransaction;
    uint32_t        slaveAddress;
    int32_t         controlStatus;
    bool            testStatus = BTRUE;
    uint8_t         txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x00, };
    uint8_t         rxBuf[I2C_APP_EEPROM_TEST_LENGTH];
    uint32_t        delayValue;
    I2CApp_TestCfg      *test = (I2CApp_TestCfg *)arg;

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */


    /* Test Runtime Configuration of Bus Frequency */

    /* Test runtime configuration of 400 kHz */
    busFrequency = I2C_400kHz;
    I2C_control(handle, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = test->writecount;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = test->readcount;
    i2cTransaction.timeout   = I2C_APP_TRANSACTION_TIMEOUT;
    transferStatus = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != transferStatus)
    {
        UART_printf("\n I2C Test: Dynamic configuration of bus Freq failed. \n");
        testStatus = BFALSE;
    }
    else
    {
        testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
    }

    if(BTRUE == testStatus)
    {
        /* Test runtime configuration of 100 kHz */
        busFrequency = I2C_100kHz;
        I2C_control(handle, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);

        memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
        transferStatus = I2C_transfer(handle, &i2cTransaction);

        if(I2C_STS_SUCCESS != transferStatus)
        {
            UART_printf("\n I2C Test: Dynamic configuration of bus Freq failed. \n");
            testStatus = BFALSE;
        }
        else 
        {
            testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        }
    }

    if(BTRUE == testStatus)
    {
        /* Test runtime configuration default value */
        busFrequency = I2C_1P0Mhz;
        I2C_control(handle, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);

        memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
        transferStatus = I2C_transfer(handle, &i2cTransaction);

        if(I2C_STS_SUCCESS != transferStatus)
        {
            UART_printf("\n I2C Test: Dynamic configuration of bus Freq failed. \n");
            testStatus = BFALSE;
        }
        else{
            testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        }
    }

    /* Test Probe functionality */

    if(BTRUE == testStatus)
    {
        /* Probe test with valid slave address */
        slaveAddress = I2C_APP_EEPROM_ADDR;
        controlStatus = I2C_control(handle, I2C_CMD_PROBE, &slaveAddress);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            testStatus = BTRUE;
        }
        else
        {
            testStatus = BFALSE;
            UART_printf("\n I2C Test: Probe test failed. \n");
        }
    }

    /* Test Invalid CMD */

    if(BTRUE == testStatus)
    {
        /* Probe test with valid slave address */
        slaveAddress = test->slaveAddress;
        controlStatus = I2C_control(handle, 0x04, &slaveAddress);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            UART_printf("\n I2C Test: VALID CMD... \n");
            testStatus = BFALSE;
        }
        else
        {
            testStatus = BTRUE;
        }
    }

    if(BTRUE == testStatus)
    {
        /* Probe test with invalid slave address */
        slaveAddress = 0x70U;
        controlStatus = I2C_control(handle, I2C_CMD_PROBE, &slaveAddress);

        if(I2C_STATUS_ERROR == controlStatus)
        {
            UART_printf("\n I2C Test: Probe test passed. \n");
        }
        else
        {
            UART_printf("\n I2C Test: Probe test failed. \n");
            testStatus = BFALSE;
        }
    }

    if(BTRUE == testStatus)
    {
        /* Test bus recovery functionality */
        delayValue = 2000U;
        controlStatus = I2C_control(handle, I2C_CMD_RECOVER_BUS, &delayValue);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
            transferStatus = I2C_transfer(handle, &i2cTransaction);

            if(I2C_STS_SUCCESS != transferStatus)
            {
                UART_printf("\n I2C Test: Bus recovery test failed. \n");
                testStatus = BFALSE;
            }
            else
            {
                testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
            }
        }
        else
        {
            testStatus = BFALSE;
        }
    }

    if(BTRUE == testStatus)
    {
        /* Test bus recovery functionality with passing timeout valve to zero*/
        delayValue = 2000U;
        controlStatus = I2C_control(handle, I2C_CMD_RECOVER_BUS, &delayValue);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
            I2C_transactionInit(&i2cTransaction);
            i2cTransaction.slaveAddress = test->slaveAddress;
            i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
            i2cTransaction.writeCount = test->writecount;
            i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
            i2cTransaction.readCount = test->readcount;
            i2cTransaction.timeout   = 0;
            transferStatus = I2C_transfer(handle, &i2cTransaction);

            if(I2C_STS_SUCCESS != transferStatus)
            {
                UART_printf("\n I2C Test: Bus recovery test failed. \n");
                testStatus = BFALSE;
            }
            else
            {
                testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
            }
        }
        else
        {
            testStatus = BFALSE;
        }
    }

    if(handle)
    {
        I2C_close(handle);
    }
    return testStatus;
}

static bool I2CApp_wakeupEnableDiasbleTest(void *arg)
{
    I2C_Params          i2cParams;
    I2C_Handle          handle = NULL;
    I2C_Transaction     i2cTransaction;
    uint8_t             txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x00, };
    uint8_t             rxBuf[I2C_APP_EEPROM_TEST_LENGTH];
    int16_t             status;
    bool                testStatus = BTRUE;
    I2CApp_TestCfg      *test = (I2CApp_TestCfg *)arg;
    I2C_HwAttrs const   *i2cCfg = NULL;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xff; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xff;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(handle == NULL)
    {
        testStatus = BFALSE;
    }

    /* Global Wake up enable*/
    i2cCfg = (I2C_HwAttrs const *)handle->hwAttrs;
    I2CGlobalWakeUpEnable(i2cCfg->baseAddr);

    /* Setting Mode smart idle*/
	I2CIdleModeSelect(i2cCfg->baseAddr, 2U);

	/* Set STC: Start Condition IRQ wakeup set Invalid Flag*/
	I2CWakeUpEnable(i2cCfg->baseAddr, I2C_APP_IRQSTATUS_STC_MASK, 0U);

	/* Set STC: Start Condition IRQ wakeup set*/
	I2CWakeUpEnable(i2cCfg->baseAddr, I2C_APP_IRQSTATUS_STC_MASK, 1U);

	/* Select clock activity OCP_CLK-> OFF, SYS_CLK->OFF */
	I2CClockActivitySelect(i2cCfg->baseAddr, 2U);

    /* EEPROM write disabled on K2, need copy data */
    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = test->writecount;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = test->readcount;
    i2cTransaction.timeout   = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = BFALSE;
    }
    else
    {
        testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        if(FALSE == testStatus)
        {
            UART_printf("I2C Test: Data Mismatch. \n");
        }
    }

    /*Auto Idle Enable set and check transaction*/
    I2CAutoIdleEnable(i2cCfg->baseAddr);

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = test->writecount;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = test->readcount;
    i2cTransaction.timeout   = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = BFALSE;
    }
    else
    {
        testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        if(BFALSE == testStatus)
        {
            UART_printf("I2C Test: Data Mismatch. \n");
        }
    }

    /*Disable I2C Wake up*/
    I2CGlobalWakeUpDisable(i2cCfg->baseAddr);

	/* Disabled STC: Start Condition IRQ wakeup clear with invalid Flag*/
    I2CWakeUpDisable(i2cCfg->baseAddr, I2C_APP_IRQSTATUS_STC_MASK, 0U);

	/* Disabled STC: Start Condition IRQ wakeup clear*/
    I2CWakeUpDisable(i2cCfg->baseAddr, I2C_APP_IRQSTATUS_STC_MASK, 1U);

    if(handle)
    {
        I2C_close(handle);
    }

    return (testStatus);
}

static bool I2CApp_receiveOverrunTest(void *arg)
{
    I2C_Params          i2cParams;
    I2C_Handle          handle = NULL;
    I2C_Transaction     i2cTransaction;
    uint8_t             txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x00, };
    uint8_t             rxBuf[I2C_APP_EEPROM_TEST_LENGTH];
    int16_t             status;
    bool                testStatus = BTRUE;
    I2CApp_TestCfg      *test = (I2CApp_TestCfg *)arg;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.validParams  = 0U;
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf     = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount   = test->writecount;
    i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount    = test->readcount;
    i2cTransaction.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = BFALSE;
    }

    if(handle)
    {
        I2C_close(handle);
    }

    return (testStatus);
}

static bool I2CApp_negativeTest(void *arg)
{
    I2C_Params          i2cParams;
    I2C_Handle          handle = NULL;
    I2C_Handle          handleOpen = NULL;
    I2C_Transaction     i2cTransaction;
    uint8_t             txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x00, };
    uint8_t             rxBuf[I2C_APP_EEPROM_TEST_LENGTH];
    int16_t             status;
    bool                testStatus = BTRUE;
    I2CApp_TestCfg      *test = (I2CApp_TestCfg *)arg;
    I2C_HwAttrs const   *i2cCfg = NULL;
    I2C_HwAttrs         i2cDefaultCfg;
#if defined (SOC_J721S2) || defined (SOC_J721E) || defined (SOC_J7200)
    uint32_t            bitRateId;
#endif

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }

    /* Test1: Called I2C_open multiple times */
    handleOpen = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL != handleOpen)
    {
        testStatus = BFALSE;
    }

    /* Set Rx and Tx FIFO threshold value */
    i2cCfg = (I2C_HwAttrs const *)handle->hwAttrs;
    I2CFIFOThresholdConfig(i2cCfg->baseAddr, 0U, CSL_I2C_TX_MODE);
    I2CFIFOThresholdConfig(i2cCfg->baseAddr, 0U, CSL_I2C_RX_MODE);

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.validParams  = 0U;
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf     = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount   = test->writecount;
    i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount    = test->readcount;
    i2cTransaction.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = BFALSE;
    }
    else
    {
        testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        if(BFALSE == testStatus)
        {
            UART_printf("I2C Test: Data Mismatch \n");
        }
    }

    /* Test2: 10 bit Addressing */
    i2cTransaction.validParams = I2C_TRANS_VALID_PARAM_MASTER_MODE | I2C_TRANS_VALID_PARAM_EXPAND_SA;
    i2cTransaction.expandSA = BTRUE;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: 10bit Address not supporting \n");
        testStatus = BTRUE;
    }

    /* Test3: Restart I2C communication for 3.4Mhz with interrupt enable */
    I2C_close(handle);
    test->intrMode = BTRUE;
    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    #if defined (SOC_J784S4) || defined (SOC_J742S2)
    i2cParams.bitRate = I2C_3P4Mhz;
    #else
    i2cParams.bitRate = I2C_100kHz;
    #endif
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf     = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount   = 0U;
    i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount    = test->readcount;
    i2cTransaction.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = BFALSE;
    }

    /* Test4: Restart I2C communication for 1Mhz with interrupt enable */
    I2C_close(handle);
    test->intrMode = BTRUE;
    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    #if defined (SOC_J784S4) || defined (SOC_J742S2) || defined (SOC_J721S2)
    i2cParams.bitRate = I2C_1P0Mhz;
    #else
    i2cParams.bitRate = I2C_100kHz;
    #endif
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf     = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount   = 0U;
    i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount    = test->readcount;
    i2cTransaction.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = BFALSE;
    }

    /* Test5: Restart I2C communication for 100khz with polling mode */
    I2C_close(handle);
    test->intrMode = BFALSE;
    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf     = NULL;
    i2cTransaction.writeCount   = 0U;
    i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount    = test->readcount;
    i2cTransaction.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = BFALSE;
    }

    /* Test6: Restart I2C communication with callback transfer NULL  */
    I2C_close(handle);
    test->intrMode = BTRUE;
    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    i2cParams.transferMode = I2C_MODE_CALLBACK;
    i2cParams.transferCallbackFxn = NULL;

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf     = NULL;
    i2cTransaction.writeCount   = 0U;
    i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount    = test->readcount;
    i2cTransaction.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
       UART_printf("I2C Test: Callback transfer check.  \n");
       testStatus = BTRUE;
    }

    /* Test7: Restart I2C communication with readcount=0 and writecount=0  */
    I2C_close(handle);
    test->intrMode = BTRUE;
    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(NULL == handle)
    {
        testStatus = BFALSE;
    }

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf     = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount   = 0U;
    i2cTransaction.readBuf      = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount    = 0U;
    i2cTransaction.timeout      = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
       testStatus = BTRUE;
    }

    /* Test8: Called I2CMasterInitExpClk with internalClk=0U & outputClk=0U */
    I2CMasterInitExpClk(i2cCfg->baseAddr, i2cCfg->funcClk, 0U, 0U);

    /* Test9: Set and get default I2C init configurations for instance = I2C_HWIP_MAX_CNT */

    status = I2C_socGetInitCfg(I2C_HWIP_MAX_CNT, &i2cDefaultCfg);

    if(I2C_STS_SUCCESS != status)
    {
        testStatus = BTRUE;
    }

    status = I2C_socSetInitCfg(I2C_HWIP_MAX_CNT, &i2cDefaultCfg);

    if(I2C_STS_SUCCESS != status)
    {
        testStatus = BTRUE;
    }

#if defined (SOC_J721S2) || defined (SOC_J721E) || defined (SOC_J7200)
    /* Test10: HS Mode Tests for J721E/J7200/J721S2 */
    if(handle)
    {
        I2C_close(handle);
    }

    /* I2C transfer for Interrupt Mode */
    for(bitRateId = I2C_APP_MAX_FREQ_COUNT; bitRateId < I2C_APP_BITRATE_INVALID_ID; bitRateId++)
    {
        status = I2CApp_bitrateTestFrequency((I2C_BitRate)bitRateId, (I2CApp_TestCfg *)test);
        if(I2C_STS_SUCCESS != status)
        {
            testStatus = BTRUE;
        }
    }

    /* I2C transfer for Polling Mode */
    test->intrMode = BFALSE;
    for(bitRateId = I2C_APP_MAX_FREQ_COUNT; bitRateId < I2C_APP_BITRATE_INVALID_ID; bitRateId++)
    {
        status = I2CApp_bitrateTestFrequency((I2C_BitRate)bitRateId, (I2CApp_TestCfg *)test);
        if(I2C_STS_SUCCESS != status)
        {
            testStatus = BTRUE;
        }
    }

    /* I2C transfer with writecount = 0 for Polling Mode */
    test->writecount = 0;
    for(bitRateId = I2C_APP_MAX_FREQ_COUNT; bitRateId < I2C_APP_BITRATE_INVALID_ID; bitRateId++)
    {
        status = I2CApp_bitrateTestFrequency((I2C_BitRate)bitRateId, (I2CApp_TestCfg *)test);
        if(I2C_STS_SUCCESS != status)
        {
            testStatus = BTRUE;
        }
    }

    /* I2C transfer with writecount = 0 for Interrupt Mode */
    test->intrMode = BTRUE;
    for(bitRateId = I2C_APP_MAX_FREQ_COUNT; bitRateId < I2C_APP_BITRATE_INVALID_ID; bitRateId++)
    {
        status = I2CApp_bitrateTestFrequency((I2C_BitRate)bitRateId, (I2CApp_TestCfg *)test);
        if(I2C_STS_SUCCESS != status)
        {
            testStatus = BTRUE;
        }
    }

#endif

    /* Clear the FIFO*/
    I2CFlushFifo(i2cCfg->baseAddr);

    if(handle)
    {
        I2C_close(handle);
    }

    return (testStatus);
}
