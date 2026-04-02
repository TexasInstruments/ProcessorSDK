/*
 * Copyright (C) 2014 - 2024 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   main_test.c
 *
 *  \brief  Example application main file. This application will read the data
 *          from eeprom and compares it with the known data.
 *
 *          On J7200 - The EEPROM is connected to the i2c which is in wakeup
 *                      domain. The I2C driver, by default supports all
 *                      all instances in MCU and Main Domain only.
 *
 *                      This test demonstrates steps required to add support for
 *                      an instance that's not natively supported by the driver.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/osal.h>

#include <stdio.h>
#include <string.h>

#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include "i2c_log.h"
#include "i2c_board.h"
#include <ti/csl/soc.h>
#include <ti/drv/sciclient/sciclient.h>

#if defined (BUILD_C7X)
#endif

#include <ti/csl/arch/csl_arch.h>

#if defined (BUILD_C7X)
#include <ti/csl/arch/csl_arch.h>
#endif

#if defined(UNITY_INCLUDE_CONFIG_H)
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Define the I2C test interface */
typedef struct I2C_Tests_s
{
    bool     (*testFunc)(void *);
    int32_t  testId;
    bool     dmaMode;
    bool     intrMode;
    bool     cbMode;
    uint32_t timeout;
    char     testDesc[80];

} I2C_Tests;

/* I2C test ID definitions */
#define I2C_TEST_ID_BIT_RATE          0   /* I2C bit rate test in interrupt mode */
#define I2C_TEST_ID_PROBE_BUS_FREQ    1   /* I2C probe bus frequency Test in interrupt mode  */
#define I2C_TEST_ID_TIMEOUT_INT       2   /* I2C timeout test in interrupt mode */
#define I2C_TEST_ID_TIMEOUT_POLLING   3   /* I2C timeout test in polling mode */

#define I2C_TRANSACTION_TIMEOUT         (10000U)

/* Number of I2C Bitrate tests */
#if defined (SOC_J784S4)
#define I2C_BITRATE_TEST_MAX_COUNT      (4U)
#else
#define I2C_BITRATE_TEST_MAX_COUNT      (2U)
#endif

#if defined (SOC_J721E)
/* By default for, first available output from IR */
#define I2C_INST_WKUP_I2C0_INT_NUM_MAIN (CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_0)
#define I2C_INST_WKUP_I2C0_INT_NUM_MCU (CSLR_MCU_R5FSS0_CORE0_INTR_WKUP_I2C0_POINTRPEND_0)
/* Interrupt reserved for Core ID 1. 128 is based on RM */
#define I2C_INST_WKUP_I2C0_INT_OFFSET   (128U)
#endif

#if defined (SOC_J784S4) || defined (SOC_J742S2)
/* WKUP I2C0 interrupt goes directly into MAIN R5 cores. Interrupt Numbers for all R5 main cores are same. */
#define I2C_INST_WKUP_I2C0_INT_NUM_MAIN (CSLR_R5FSS0_CORE0_INTR_WKUP_I2C0_POINTRPEND_0)
#define I2C_INST_WKUP_I2C0_INT_NUM_MCU (CSLR_MCU_R5FSS0_CORE0_INTR_WKUP_I2C0_POINTRPEND_0)
/* Interrupt reserved for Core ID 1. 128 is based on RM */
#define I2C_INST_WKUP_I2C0_INT_OFFSET   (0U)
#endif

#if defined (SOC_J721S2)
/* WKUP I2C0 interrupt goes directly into MAIN R5 cores. Interrupt Numbers for all R5 main cores are same. */
#define I2C_INST_WKUP_I2C0_INT_NUM_MAIN (CSLR_R5FSS0_CORE0_INTR_WKUP_I2C0_POINTRPEND_0)
#define I2C_INST_WKUP_I2C0_INT_NUM_MCU (CSLR_MCU_R5FSS0_CORE0_INTR_WKUP_I2C0_POINTRPEND_0)
/* Interrupt reserved for Core ID 1. 128 is based on RM */
#define I2C_INST_WKUP_I2C0_INT_OFFSET   (0U)
#endif

#if defined (SOC_J7200)
/* Applicable for MCU 20/21 only */
#define I2C_INST_WKUP_I2C0_INT_NUM_MAIN (CSLR_R5FSS0_CORE0_INTR_WKUP_I2C0_POINTRPEND_0)
#define I2C_INST_WKUP_I2C0_INT_NUM_MCU (CSLR_MCU_R5FSS0_CORE0_INTR_WKUP_I2C0_POINTRPEND_0)
/* No interrupt router, its directly connected */
#define I2C_INST_WKUP_I2C0_INT_OFFSET   (0U)
#endif /* J7200 Specific */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

char eepromData[I2C_EEPROM_TEST_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00};

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

bool Board_initI2C(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    I2C_HwAttrs   i2c_cfg;
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_MODULE_CLOCK |
        BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (BOARD_SOK != boardStatus)
    {
        return (BFALSE);
    }
    
    I2C_init();

   /* Get the default I2C init configurations */
    I2C_socGetInitCfg(I2C_EEPROM_INSTANCE, &i2c_cfg);

    /* Modify the default I2C configurations if necessary */
    /* No I2C instanced connected to eeprom in main domain, use i2c instance in wakeup domain */
    i2c_cfg.baseAddr = CSL_WKUP_I2C0_CFG_BASE;
#if defined (BUILD_MPU)
    i2c_cfg.intNum = CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_WKUP_I2C0_POINTRPEND_0;
#endif

#if defined (BUILD_MCU)
    CSL_ArmR5CPUInfo info;

    CSL_armR5GetCpuID(&info);
    if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 != info.grpId)
    {
        /*
         * Pulsar R5 core is on the Main domain, use the Main Pulsar
         * interrupt router
         */
        if(0U == info.cpuID)
        {
            i2c_cfg.intNum = I2C_INST_WKUP_I2C0_INT_NUM_MAIN;
        }
        else
        {
            i2c_cfg.intNum = I2C_INST_WKUP_I2C0_INT_NUM_MAIN +
                                I2C_INST_WKUP_I2C0_INT_OFFSET;
        }
    }
    else
    {
        /* Pulsar R5 core is on the MCU domain */
        i2c_cfg.intNum = I2C_INST_WKUP_I2C0_INT_NUM_MCU;
    }
#endif /* for mcu builds only */

#if defined (BUILD_C7X_1)
    i2c_cfg.eventId = CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_WKUP_I2C0_POINTRPEND_0 + 992U, /* eventId, input event # to CLEC */
#endif

    /* Set the default I2C init configurations */
    I2C_socSetInitCfg(I2C_EEPROM_INSTANCE, &i2c_cfg);

    I2C_log("\n I2C Test: Using Instance %d", I2C_EEPROM_INSTANCE);
    return (BTRUE);
}

static void I2C_initConfig(uint32_t instance, I2C_Tests *test)
{
    I2C_HwAttrs   i2c_cfg;

    /* Get the default SPI init configurations */
    I2C_socGetInitCfg(instance, &i2c_cfg);

#if defined (SOC_J721E) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if defined (BUILD_C66X)
    /*
     * There is no interrupt routing supported in sciclient to
     * route wakeup domain I2C0 interrupt to C66x cores due to
     * the hardware limitation, use polling mode for eeprom test
     */
    i2c_cfg.enableIntr = BFALSE;
#else
    i2c_cfg.enableIntr = test->intrMode;
#endif
#else
    i2c_cfg.enableIntr = test->intrMode;
#endif

    /* Set the SPI init configurations */
    I2C_socSetInitCfg(instance, &i2c_cfg);
}

static bool CompareData(char *expData, char *rxData, uint32_t length)
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

static bool i2c_bitrate_test (I2C_BitRate bitRate, I2C_Tests *test)
{
    char            bitRateLog[4][10] = {"100Kbps", "400Kbps", "1Mbps", "3.4Mbps"};
    I2C_Params      i2cParams;
    I2C_Handle      handle = NULL;
    I2C_Transaction i2cTransaction;
    char            txBuf[I2C_EEPROM_TEST_LENGTH + I2C_EEPROM_ADDR_SIZE] = {0x00, };
    char            rxBuf[I2C_EEPROM_TEST_LENGTH];
    int16_t         status;
    bool            copyData = BFALSE;
    bool            testStatus = BTRUE;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */

    I2C_initConfig(I2C_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = bitRate;
    handle = I2C_open(I2C_EEPROM_INSTANCE, &i2cParams);
    if (handle == NULL)
    {
        goto Err;
    }

#ifdef I2C_EEPROM_WRITE_ENABLE
    /* Write to EEPROM */
    memcpy(&txBuf[I2C_EEPROM_ADDR_SIZE], eepromData, I2C_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_EEPROM_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = I2C_EEPROM_TEST_LENGTH + I2C_EEPROM_ADDR_SIZE;
    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        I2C_log("\n I2C Test: ");
        I2C_log(bitRateLog[bitRate]);
        I2C_log(": Write Data Transfer failed. \n");
        testStatus = BFALSE;
        goto Err;
    }
    BOARD_delay(I2C_EEPROM_TEST_DELAY);
#else
    /* EEPROM write disabled on K2, need copy data */
    copyData = BTRUE;
#endif
    memset(rxBuf, 0, I2C_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_EEPROM_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = I2C_EEPROM_ADDR_SIZE;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = I2C_EEPROM_TEST_LENGTH;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        I2C_log("\n I2C Test: ");
        I2C_log(bitRateLog[bitRate]);
        I2C_log(": Read Data Transfer failed. \n");
        testStatus = BFALSE;
        goto Err;
    }
    else
    {
        copyData = BTRUE;

        /* read only test, copy data from rx buffer to eepromData to pass the test */
        if (copyData)
            memcpy(eepromData, rxBuf, I2C_EEPROM_TEST_LENGTH);

        testStatus = CompareData(&eepromData[0], &rxBuf[0], I2C_EEPROM_TEST_LENGTH);
        if(BFALSE == testStatus)
        {
            I2C_log("\n I2C Test: ");
            I2C_log(bitRateLog[bitRate]);
            I2C_log(": Data Mismatch \n");
        }
    }

Err:
    if (handle)
    {
        I2C_close(handle);
    }

    return (testStatus);
}

static bool I2C_bitrate_test(void *arg)
{
    bool       testResult = BTRUE;
    uint32_t   i;

    for (i = 0; i < I2C_BITRATE_TEST_MAX_COUNT; i++)
    {
        testResult = i2c_bitrate_test((I2C_BitRate)i, (I2C_Tests *)arg);
        if (BFALSE == testResult)
        {
            break;
        }
    }

    return (testResult);
}

static bool I2C_Probe_BusFrequency_test(void *arg)
{
    I2C_Handle      handle;
    I2C_Params      i2cParams;
    uint32_t        busFrequency;
    bool            status = BFALSE;
    int16_t         transferStatus;
    I2C_Transaction i2cTransaction;
    uint32_t        slaveAddress;
    int32_t         controlStatus;
    char            txBuf[I2C_EEPROM_TEST_LENGTH + I2C_EEPROM_ADDR_SIZE] = {0x00, };
    char            rxBuf[I2C_EEPROM_TEST_LENGTH];
    uint32_t        delayValue;
    I2C_Tests      *test = (I2C_Tests *)arg;

    I2C_initConfig(I2C_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);
    handle = I2C_open(I2C_EEPROM_INSTANCE, &i2cParams);
    if (NULL == handle)
    {
        goto Err;
    }

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_EEPROM_TEST_ADDR >> 8) & 0xFF; /* EEPROM memory high address byte */
    txBuf[1] = I2C_EEPROM_TEST_ADDR & 0xFF;        /* EEPROM memory low address byte */


    /* Test Runtime Configuration of Bus Frequency */

    /* Test runtime configuration of 400 kHz */
    busFrequency = I2C_400kHz;
    I2C_control(handle, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);

    memset(rxBuf, 0, I2C_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_EEPROM_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = I2C_EEPROM_ADDR_SIZE;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = I2C_EEPROM_TEST_LENGTH;
    i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
    transferStatus = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != transferStatus)
    {
        I2C_log("\n I2C Test: Dynamic configuration of bus Freq failed. \n");
        goto Err;
    }

    status = CompareData(&eepromData[0], &rxBuf[0], I2C_EEPROM_TEST_LENGTH);

    if(BTRUE == status)
    {
        /* Test runtime configuration of 100 kHz */
        busFrequency = I2C_100kHz;
        I2C_control(handle, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);

        memset(rxBuf, 0, I2C_EEPROM_TEST_LENGTH);
        I2C_transactionInit(&i2cTransaction);
        i2cTransaction.slaveAddress = I2C_EEPROM_ADDR;
        i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
        i2cTransaction.writeCount = I2C_EEPROM_ADDR_SIZE;
        i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
        i2cTransaction.readCount = I2C_EEPROM_TEST_LENGTH;
        i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
        transferStatus = I2C_transfer(handle, &i2cTransaction);

        if(I2C_STS_SUCCESS != transferStatus)
        {
            I2C_log("\n I2C Test: Dynamic configuration of bus Freq failed. \n");
            goto Err;
        }

        status = CompareData(&eepromData[0], &rxBuf[0], I2C_EEPROM_TEST_LENGTH);
    }

    /* Test Probe functionality */

    if(BTRUE == status)
    {
        /* Probe test with valid slave address */
        slaveAddress = I2C_EEPROM_ADDR;
        controlStatus = I2C_control(handle, I2C_CMD_PROBE, &slaveAddress);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            status = BTRUE;
        }
        else
        {
            status = BFALSE;
            I2C_log("\n I2C Test: Probe test failed. \n");
            goto Err;
        }
    }

    if(BTRUE == status)
    {
        /* Probe test with invalid slave address */
        slaveAddress = 0x70U;
        controlStatus = I2C_control(handle, I2C_CMD_PROBE, &slaveAddress);

        if(I2C_STATUS_ERROR == controlStatus)
        {
            status = BTRUE;
        }
        else
        {
            status = BFALSE;
            I2C_log("\n I2C Test: Probe test failed. \n");
            goto Err;
        }
    }

    if(BTRUE == status)
    {
        /* Test bus recovery functionality */
        delayValue = 2000U;
        controlStatus = I2C_control(handle, I2C_CMD_RECOVER_BUS, &delayValue);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            memset(rxBuf, 0, I2C_EEPROM_TEST_LENGTH);
            I2C_transactionInit(&i2cTransaction);
            i2cTransaction.slaveAddress = I2C_EEPROM_ADDR;
            i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
            i2cTransaction.writeCount = I2C_EEPROM_ADDR_SIZE;
            i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
            i2cTransaction.readCount = I2C_EEPROM_TEST_LENGTH;
            i2cTransaction.timeout   = I2C_TRANSACTION_TIMEOUT;
            transferStatus = I2C_transfer(handle, &i2cTransaction);

            if(I2C_STS_SUCCESS != transferStatus)
            {
                I2C_log("\n I2C Test: Bus recovery test failed. \n");
                goto Err;
            }

            status = CompareData(&eepromData[0], &rxBuf[0], I2C_EEPROM_TEST_LENGTH);
        }
        else
        {
            status = BFALSE;
        }
    }

Err:
    if (handle)
    {
        I2C_close(handle);
    }
    return status;
}

static bool I2C_timeout_test(void *arg)
{
    I2C_Handle      handle;
    I2C_Params      i2cParams;
    uint32_t        busFrequency;
    bool            status = BFALSE;
    int16_t         transferStatus;
    I2C_Transaction i2cTransaction;
    char            txBuf[I2C_EEPROM_TEST_LENGTH + I2C_EEPROM_ADDR_SIZE] = {0x00, };
    char            rxBuf[I2C_EEPROM_TEST_LENGTH];
    I2C_Tests      *test = (I2C_Tests *)arg;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_EEPROM_TEST_ADDR >> 8) & 0xff; /* EEPROM memory high address byte */
    txBuf[1] = I2C_EEPROM_TEST_ADDR & 0xff;        /* EEPROM memory low address byte */

    I2C_initConfig(I2C_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);
    handle = I2C_open(I2C_EEPROM_INSTANCE, &i2cParams);
    if (NULL == handle)
    {
        goto Err;
    }

    /* Test Runtime Configuration of Bus Frequency */

    /* Test runtime configuration of 400 kHz */
    busFrequency = I2C_100kHz;
    I2C_control(handle, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);

    memset(rxBuf, 0, I2C_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = I2C_EEPROM_ADDR;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = I2C_EEPROM_ADDR_SIZE;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = I2C_EEPROM_TEST_LENGTH;
    i2cTransaction.timeout   = test->timeout;
    transferStatus = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_ERR_TIMEOUT == transferStatus)
    {
        status = BTRUE;
    }

Err:
    if (handle)
    {
        I2C_close(handle);
    }

    return status;
}

void I2C_test_print_test_desc(I2C_Tests *test)
{
    char        testId[16] = {0, };

    /* Print unit test ID */
    sprintf(testId, "%d", test->testId);
    I2C_log("\n I2C UT %s\n", testId);

    /* Print test description */
    I2C_log("\n %s\n", test->testDesc);
}

I2C_Tests I2c_tests[] =
{
    /* testFunc                   testID                       dma     intr    cbMode  timeout                  testDesc */
    {I2C_bitrate_test,            I2C_TEST_ID_BIT_RATE,        BFALSE, BTRUE,  BFALSE, SemaphoreP_WAIT_FOREVER, "\r\n I2C bit rate test in interrupt mode"},
    {I2C_Probe_BusFrequency_test, I2C_TEST_ID_PROBE_BUS_FREQ,  BFALSE, BTRUE,  BFALSE, SemaphoreP_WAIT_FOREVER, "\r\n I2C probe bus freq test in interrupt mode"},
    {I2C_timeout_test,            I2C_TEST_ID_TIMEOUT_INT,     BFALSE, BTRUE,  BFALSE, 1,                       "\r\n I2C timeout test in interrupt mode"},
    {NULL, },
};


#if defined(UNITY_INCLUDE_CONFIG_H)
/*
 *  ======== Unity set up and tear down ========
 */
void setUp(void)
{
    /* Do nothing */
}

void tearDown(void)
{
    /* Do nothing */
}

bool test_I2C_Eeprom_common(void)
{
    bool       testResult = BTRUE;
    uint32_t   i;
    I2C_Tests *test;

    if (BFALSE == Board_initI2C())
    {
        I2C_log("\r\n %s Board_initI2C failed\r\n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_FAIL();
#endif
        while(1);
    }

    for (i = 0U; ; i++)
    {
        test = &I2c_tests[i];
        if (NULL == test->testFunc)
        {
            break;
        }
        I2C_test_print_test_desc(test);
        if (BTRUE == test->testFunc((void *)test))
        {
            I2C_log("\r\n %s have passed\r\n", test->testDesc);
        }
        else
        {
            I2C_log("\r\n %s have failed\r\n", test->testDesc);
            testResult = BFALSE;
            break;
        }
    }
    return testResult;
}

void test_I2C_Eeprom_TestApp(void)
{
    bool       testResult = BTRUE;
    testResult = test_I2C_Eeprom_common();

    if(BTRUE == testResult)
    {
        I2C_log("\n All tests have passed. \n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_PASS();
#endif
    }
    else
    {
        I2C_log("\n Some tests have failed. \n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_FAIL();
#endif
    }

    while (BTRUE)
    {
    }
}

void test_I2C_Eeprom_BareMetal_TestApp(void)
{
    bool       testResult = BTRUE;
    testResult = test_I2C_Eeprom_common();

    if(BTRUE == testResult)
    {
        I2C_log("\n All tests have passed. \n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_PASS();
#endif
    }
    else
    {
        I2C_log("\n Some tests have failed. \n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_FAIL();
#endif
    }

    while (BTRUE)
    {
    }
}

void test_I2C_Eeprom_TestApp_runner(void)
{
    /* @description: Test runner for I2C_Eeprom Test App
                     and I2C_Eeprom BareMetal Test App

       @requirements:PRSDK-6918, PRSDK-6919, PRSDK-6922,
                     PRSDK-6924, PRSDK-6930

       @cores: mpu1_0, mcu1_0 */

    UNITY_BEGIN();
#ifdef RTOS_ENV
    RUN_TEST(test_I2C_Eeprom_TestApp);
#else
    RUN_TEST(test_I2C_Eeprom_BareMetal_TestApp);
#endif
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
}
#endif

#ifdef RTOS_ENV
/*
 *  ======== test function ========
 */
void i2c_test(void *arg0, void *arg1)
#else
int main ()
#endif
{
#if defined(UNITY_INCLUDE_CONFIG_H)
    test_I2C_Eeprom_TestApp_runner();
#else
    bool       testResult = BTRUE;
    uint32_t   i;
    I2C_Tests *test;

    if (BFALSE == Board_initI2C())
    {
#ifdef RTOS_ENV
        return;
#else
        return(0);
#endif        
    }

    for (i = 0; ; i++)
    {
        test = &I2c_tests[i];
        if (NULL == test->testFunc)
        {
            break;
        }
        I2C_test_print_test_desc(test);
        if (BTRUE == test->testFunc((void *)test))
        {
            I2C_log("\r\n %s have passed\r\n", test->testDesc);
        }
        else
        {
            I2C_log("\r\n %s have failed\r\n", test->testDesc);
            testResult = BFALSE;
            break;
        }
    }

    #if defined LDRA_DYN_COVERAGE_EXIT
    I2C_log("\n LDRA ENTRY \n");
    upload_execution_history();
    I2C_log("\n LDRA EXIT \n");
    #endif

    if(BTRUE == testResult)
    {
        I2C_log("\n All tests have passed. \n");
    }
    else
    {
        I2C_log("\n Some tests have failed. \n");
    }

    while (BTRUE)
    {
    }
#endif
}

#ifdef RTOS_ENV
/*
 *  ======== main ========
 */

/**< Stack for application task */
#define APP_EEPROMREAD_TASK_STACK_SIZE  (0x8000)
uint8_t gEepromReadAppStack[APP_EEPROMREAD_TASK_STACK_SIZE];

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
    taskParams.stack = (void *) gEepromReadAppStack;

    task = TaskP_create(&i2c_test, &taskParams);
    if (NULL == task) {
        OS_stop();
        OSAL_Assert(NULL == task);
    }

    /* Start RTOS */
    OS_start();
    return (0);
}
#endif /* #ifdef RTOS_ENV */

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

