/******************************************************************************
 * Copyright (c) 2021-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   pmic_ut_common.h
 *
 *  \brief  Unit test related common APIs
 *
 */

#include <pmic_ut_common.h>
#include <stdio.h>
#include <string.h>
#include <pmic.h>

/* CSL Header files */
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/csl_timer.h>
#include <ti/csl/csl_gpio.h>

/* OSAL Header files */
#include <ti/osal/osal.h>

/* Board Header files */
#include <ti/board/board.h>
#include <ti/board/board_cfg.h>
#include <ti/board/src/devices/common/common.h>

/* I2C Header files */
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>

/* GPIO Header Files */
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>

#include <ti/build/unit-test/Unity/src/unity.h>

/* Pointer holds the pPmicCoreHandle for I2C */
Pmic_CoreHandle_t *pPmicCoreHandleI2C = NULL;

bool enableFaultInjectionRead  = BFALSE;
bool enableFaultInjectionWrite = BFALSE;
bool enableBenchMark   = BFALSE;
uint8_t startup_type   = 0U;
uint8_t readCount      = 0U;
uint8_t writeCount     = 0U;
uint8_t skipReadCount  = 0U;
uint8_t skipWriteCount = 0U;

/* CRC8 Table with polynomial value:0x7 */
uint8_t crc8_tlb[] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
    0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
    0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
    0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
    0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
    0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
    0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
    0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
    0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
    0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
    0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
    0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
    0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
    0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
    0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
    0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
    0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};

/*!
 * \brief   OS specific Critical section locking Variable
 *          Should be OS specific locking varaible to
 *          use OS locking system for PMIC
 */
static SemaphoreP_Handle pmic_Sem = NULL;

/*!
 * \brief   GPIO Driver board specific pin configuration structure
 */
GPIO_PinConfig gpioPinConfigs[] = {
#if defined(SOC_J721E)
    /* Input pin with interrupt enabled */
    GPIO_DEVICE_CONFIG(J7_WAKEUP_GPIO0_PORT_NUM, J7_WAKEUP_GPIO0_9_PIN_NUM) |
    GPIO_CFG_IN_INT_FALLING | GPIO_CFG_INPUT
#endif
#if defined(SOC_J7200)
    /* Input pin with interrupt enabled */
    GPIO_DEVICE_CONFIG(J7_WAKEUP_GPIO0_PORT_NUM, J7_WAKEUP_GPIO0_84_PIN_NUM) |
    GPIO_CFG_IN_INT_FALLING | GPIO_CFG_INPUT
#endif
};

/*!
 * \brief   GPIO Driver call back functions
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL
};

/*!
 * \brief   GPIO Driver configuration structure
 */
GPIO_v0_Config GPIO_v0_config = {
    gpioPinConfigs,
    gpioCallbackFunctions,
    sizeof(gpioPinConfigs) / sizeof(GPIO_PinConfig),
    sizeof(gpioCallbackFunctions) / sizeof(GPIO_CallbackFxn),
#if ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R'))
    0x8U
#else
#if defined(BUILD_C7X)
    0x01U
#else
    0x20U
#endif
#endif
};

/**
 * \brief    This API Set Config for TI HW I2C instances
 *
 * \param    instance [IN] I2C instance number
 * \param    baseAddr [IN] Register base address of the I2C instance
 */
static void test_pmic_setConfigI2C(uint8_t instance, uint32_t baseAddr)
{
    I2C_HwAttrs i2cCfg;

    I2C_socGetInitCfg(instance, &i2cCfg);
    i2cCfg.baseAddr   = baseAddr;
    i2cCfg.enableIntr = BFALSE;
    I2C_socSetInitCfg(instance, &i2cCfg);
}

/**
 *
 * \brief    Function to calculate CRC8 for given data using below values
 *           CRC Polynomial value: 0x07, Initial Value: 0xFF, Final Value: 0x00
 */
static int32_t test_pmic_getcrc8Val(uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFFU;
    uint8_t i;

    for (i = 0; i < len; i++)
    {
        crc = crc8_tlb[data[i] ^ crc];
    }

    return crc;
}

/**
 *
 * \brief    Initalize SPI stub function to access PMIC registers using
 *           I2C Communication Handle
 */
static int32_t test_pmic_spi_stubInit(Pmic_CoreCfg_t  *pPmicConfigData)
{
    int32_t pmicStatus = PMIC_ST_SUCCESS;
    Pmic_CoreCfg_t pmicConfigDataI2c = {0U};

    /* Fill parameters to pmicConfigDataI2C */
    pmicConfigDataI2c.pmicDeviceType      = pPmicConfigData->pmicDeviceType;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_DEVICE_TYPE_VALID_SHIFT;

    pmicConfigDataI2c.commMode            = PMIC_INTF_DUAL_I2C;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_COMM_MODE_VALID_SHIFT;

    pmicConfigDataI2c.slaveAddr           = J721E_LEO_PMICA_SLAVE_ADDR;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigDataI2c.qaSlaveAddr         = J721E_LEO_PMICA_WDG_SLAVE_ADDR;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_QASLAVEADDR_VALID_SHIFT;

    pmicConfigDataI2c.pFnPmicCommIoRead   = test_pmic_regRead;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_COMM_IO_RD_VALID_SHIFT;

    pmicConfigDataI2c.pFnPmicCommIoWrite  = test_pmic_regWrite;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_COMM_IO_WR_VALID_SHIFT;

    pmicConfigDataI2c.pFnPmicCritSecStart =
        pPmicConfigData->pFnPmicCritSecStart;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_CRITSEC_START_VALID_SHIFT;

    pmicConfigDataI2c.pFnPmicCritSecStop  = pPmicConfigData->pFnPmicCritSecStop;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_CRITSEC_STOP_VALID_SHIFT;

    /*
     * Recalling appInit function to initialize I2C PMIC Core handle
     * for Stub Operations
     */
    pmicStatus = test_pmic_appInit(&pPmicCoreHandleI2C, &pmicConfigDataI2c);

    if (PMIC_ST_SUCCESS == pmicStatus)
    {
        /*
         * Update Valid Communication Handle to make SPI stub
         * PMIC core handle Init Success
         */
        pPmicConfigData->pCommHandle  = pPmicCoreHandleI2C->pCommHandle;
        pPmicConfigData->validParams |= PMIC_CFG_COMM_HANDLE_VALID_SHIFT;
    }

    return pmicStatus;
}

/**
 * \brief    Deinitalize SPI stub function
 */
static int32_t test_pmic_spi_stubDeinit(void **pCommHandle)
{
    test_pmic_appDeInit(pPmicCoreHandleI2C);

    if (NULL == (I2C_Handle) * pCommHandle)
    {
        return PMIC_ST_ERR_NULL_PARAM;
    }

    *pCommHandle = NULL;

    return PMIC_ST_SUCCESS;
}

/**
 * \brief  SPI stub function read operation to read PMIC registers
 *         using I2C interface
 */
int32_t test_pmic_spi_stubRead(Pmic_CoreHandle_t  *pPmicCorehandle,
                               uint8_t            *pBuf,
                               uint8_t             bufLen)
{
    int32_t pmicStatus = 0;
    uint8_t instType   = 0U;
    uint16_t regAddr   = 0U;
    bool wdgopn        = BFALSE;
    uint8_t rxBuf[4U]  = {0U};

    /* Check for WatchDog Operation */
    if (0U != (pBuf[1U] & (0x04 << 5U)))
    {
        wdgopn = BTRUE;
    }

    /* Update register Address from spi buffer */
    regAddr = (uint16_t)pBuf[0U];
    bufLen  = 1U;

    /* Find Instance type from wdg operation */
    if (BTRUE == wdgopn)
    {
        instType = PMIC_QA_INST;
    }
    else
    {
        instType = PMIC_MAIN_INST;
    }

    /* Increase buffer lenth 1 more to get CRC, if CRC is Enabled */
    if (BTRUE == pPmicCorehandle->crcEnable)
    {
        bufLen++;
    }

    /* Call PMIC read with I2C Instance */
    pmicStatus = test_pmic_regRead(pPmicCoreHandleI2C,
                                   instType,
                                   regAddr,
                                   rxBuf,
                                   bufLen);

    /* Updating the Recieved Reg Value to SPI Buffer */
    pBuf[2U] = rxBuf[0U];

    /* Updating the Recieved CRC to SPI Buffer */
    if (BTRUE == pPmicCorehandle->crcEnable)
    {
        pBuf[3U] = test_pmic_getcrc8Val(pBuf, 3U);
    }

    return pmicStatus;
}

/**
 * \brief  SPI stub function write operation to write PMIC registers
 *         using I2C interface
 */
int32_t test_pmic_spi_write(Pmic_CoreHandle_t  *pPmicCorehandle,
                            uint8_t            *pBuf,
                            uint8_t             bufLen)
{
    int32_t  pmicStatus = 0;
    uint8_t  instType   = 0U;
    bool     wdgopn     = BFALSE;
    uint16_t regAddr    = 0U;
    uint8_t  txBuf[4U]  = {0U};

    /* Update register Address from spi buffer */
    regAddr = (uint16_t)pBuf[0U];

    /* Check for WatchDog Operation */
    if (0U != (pBuf[1U] & (0x04 << 5U)))
    {
        wdgopn = BTRUE;
    }

    /* Find Instance type from wdg operation */
    if (BTRUE == wdgopn)
    {
        instType = PMIC_QA_INST;
    }
    else
    {
        instType = PMIC_MAIN_INST;
    }

    /* Updating the SPI Buffer Reg Value to I2C Buffer */
    txBuf[0U] = pBuf[2U];
    bufLen    = 1U;

    /* Updating the Received CRC to SPI Buffer */
    if (BTRUE == pPmicCorehandle->crcEnable)
    {
        uint8_t crcbuf[4U] = {0U};

        if (BTRUE == wdgopn)
        {
            crcbuf[0U] = pPmicCoreHandleI2C->qaSlaveAddr << 1U;
        }
        else
        {
            crcbuf[0U] = pPmicCoreHandleI2C->slaveAddr << 1U;
        }

        crcbuf[1U] = regAddr;
        crcbuf[2U] = pBuf[2U];
        txBuf[1U]  = test_pmic_getcrc8Val(crcbuf, 3U);
        bufLen++;
    }

    /* Call PMIC write with I2C Instance */
    pmicStatus = test_pmic_regWrite(pPmicCoreHandleI2C,
                                    instType,
                                    regAddr,
                                    txBuf,
                                    bufLen);

    return pmicStatus;
}

/**
 * \brief  Function to probe PMIC slave devices on I2C instance
 */
static int32_t test_pmic_i2c_devices(Pmic_CoreHandle_t  *pPmicCorehandle,
                                     uint32_t            instType)
{
    int32_t status = PMIC_ST_SUCCESS;
    uint16_t slaveAddr;
    I2C_Handle i2cHandle;

    if (PMIC_INTF_SINGLE_I2C == pPmicCorehandle->commMode)
    {
        i2cHandle = pPmicCorehandle->pCommHandle;
        /* For Main PAGE SLAVE ID */
        slaveAddr = pPmicCorehandle->slaveAddr;

        if (I2C_STATUS_SUCCESS ==
            I2C_control(i2cHandle, I2C_CMD_PROBE, &slaveAddr))
        {
            pmic_log("I2C%d: Passed for address 0x%X !!! \r\n",
                     instType, slaveAddr);
        }
        else
        {
            status = PMIC_ST_ERR_I2C_COMM_FAIL;
        }

        /* For WD PAGE SLAVE ID */
        slaveAddr = pPmicCorehandle->qaSlaveAddr;

        if (I2C_STATUS_SUCCESS ==
            I2C_control(i2cHandle, I2C_CMD_PROBE, &slaveAddr))
        {
            pmic_log("I2C%d: Passed for address 0x%X !!! \r\n",
                     instType, slaveAddr);
        }
        else
        {
            status = PMIC_ST_ERR_I2C_COMM_FAIL;
        }
    }

    if (PMIC_INTF_DUAL_I2C == pPmicCorehandle->commMode)
    {
        /* Main I2c BUS */
        if (PMIC_MAIN_INST == instType)
        {
            slaveAddr = pPmicCorehandle->slaveAddr;
            i2cHandle = pPmicCorehandle->pCommHandle;
        }
        /* For WDG QA I2C BUS */
        else if (PMIC_QA_INST == instType)
        {
            slaveAddr = pPmicCorehandle->qaSlaveAddr;
            i2cHandle = pPmicCorehandle->pQACommHandle;
        }

        if (I2C_STATUS_SUCCESS ==
            I2C_control(i2cHandle, I2C_CMD_PROBE, &slaveAddr))
        {
            pmic_log("I2C%d: Passed for address 0x%X !!! \r\n",
                     instType, slaveAddr);
        }
        else
        {
            status = PMIC_ST_ERR_I2C_COMM_FAIL;
        }
    }

    return status;
}

/*!
 * \brief   Function to setup the I2C lld interface for PMIC
 *
 * \param   pPmicCoreHandle  [IN]     PMIC Interface Handle.
 * \param   instType         [IN]     Instance Type
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code
 *          For valid values see \ref Pmic_ErrorCodes
 */
static int32_t test_pmic_i2c_lld_intf_setup(Pmic_CoreCfg_t  *pPmicConfigData,
                                            uint8_t          instType)
{
    I2C_Params i2cParams;
    I2C_Handle i2cHandle = NULL;
    uint8_t i2c_instance = 0U;

    pmic_log("%s(): %d: %s I2C Setup...\n", __func__, __LINE__,
             (instType == PMIC_MAIN_INST) ? "PMIC_MAIN_INST" : "PMIC_QA_INST");

    /* Main I2c BUS */
    if (instType == PMIC_MAIN_INST)
    {
        /* Initialize i2c core instances */
        I2C_init();

        i2c_instance = 0U;
        test_pmic_setConfigI2C(i2c_instance, CSL_WKUP_I2C0_CFG_BASE);
    }
    /* For WDG QA I2C BUS */
    else if (PMIC_QA_INST == instType)
    {
        i2c_instance = 1U;
        test_pmic_setConfigI2C(i2c_instance, CSL_MCU_I2C0_CFG_BASE);
    }

    /* Configured i2cParams.bitRate with standard I2C_100kHz */
    I2C_Params_init(&i2cParams);

    i2cHandle = I2C_open(i2c_instance, &i2cParams);

    if (NULL == i2cHandle)
    {
        pmic_log("I2C_open is failed!!!\n");

        return PMIC_ST_ERR_COMM_INTF_INIT_FAIL;
    }

    /* Main I2c BUS */
    if (instType == PMIC_MAIN_INST)
    {
        pPmicConfigData->pCommHandle = i2cHandle;
    }
    /* For WDOG QA I2C BUS */
    else if (PMIC_QA_INST == instType)
    {
        pPmicConfigData->pQACommHandle = i2cHandle;
    }

    pmic_log("%s(): %d: done...\n", __func__, __LINE__);

    return PMIC_ST_SUCCESS;
}

/*!
 * \brief   Function to setup the QA I2c interface for LEO PMIC depending
 *          upon i2c mode
 *
 * \param   pPmicCoreHandle   [IN]    PMIC Interface Handle
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code
 *          For valid values see \ref Pmic_ErrorCodes
 */
static int32_t test_pmic_leo_dual_i2c_pin_setup(Pmic_CoreHandle_t *pPmicHandle)
{
    int32_t pmicStatus     = PMIC_ST_SUCCESS;
    Pmic_GpioCfg_t gpioCfg = {0U};

    gpioCfg.validParams = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT |
                          PMIC_GPIO_CFG_OD_VALID_SHIFT;

    pmicStatus = Pmic_gpioGetConfiguration(pPmicHandle,
                                           PMIC_TPS6594X_GPIO1_PIN,
                                           &gpioCfg);

    if (PMIC_INTF_SINGLE_I2C == pPmicHandle->commMode)
    {
        if (gpioCfg.pinFunc == PMIC_TPS6594X_GPIO_PINFUNC_GPIO1_SCL_I2C2_CS_SPI)
        {
            gpioCfg.pinFunc = PMIC_TPS6594X_GPIO_PINFUNC_GPIO;
        }
    }

    if (PMIC_INTF_DUAL_I2C == pPmicHandle->commMode)
    {
        gpioCfg.outputSignalType = PMIC_GPIO_OPEN_DRAIN_OUTPUT;
        gpioCfg.pinFunc = PMIC_TPS6594X_GPIO_PINFUNC_GPIO1_SCL_I2C2_CS_SPI;
    }

    pmicStatus = Pmic_gpioSetConfiguration(pPmicHandle,
                                           PMIC_TPS6594X_GPIO1_PIN,
                                           gpioCfg);

    gpioCfg.validParams = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT |
                          PMIC_GPIO_CFG_OD_VALID_SHIFT;

    if (PMIC_ST_SUCCESS == pmicStatus)
    {
        pmicStatus = Pmic_gpioGetConfiguration(pPmicHandle,
                                               PMIC_TPS6594X_GPIO2_PIN,
                                               &gpioCfg);
    }

    if (PMIC_ST_SUCCESS == pmicStatus)
    {
        if (PMIC_INTF_SINGLE_I2C == pPmicHandle->commMode)
        {
            if (gpioCfg.pinFunc ==
                PMIC_TPS6594X_GPIO_PINFUNC_GPIO2_SDA_I2C2_SDO_SPI)
            {
                gpioCfg.pinFunc = PMIC_TPS6594X_GPIO_PINFUNC_GPIO;
            }
        }

        if (PMIC_INTF_DUAL_I2C == pPmicHandle->commMode)
        {
            gpioCfg.outputSignalType = PMIC_GPIO_OPEN_DRAIN_OUTPUT;
            gpioCfg.pinFunc = PMIC_TPS6594X_GPIO_PINFUNC_GPIO2_SDA_I2C2_SDO_SPI;
        }

        pmicStatus = Pmic_gpioSetConfiguration(pPmicHandle,
                                               PMIC_TPS6594X_GPIO2_PIN,
                                               gpioCfg);
    }

    return pmicStatus;
}

/*!
 * \brief   Function to setup the QA I2c interface for HERA PMIC depending
 *          upon i2c mode
 *
 * \param   pPmicCoreHandle   [IN]    PMIC Interface Handle
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code
 *          For valid values see \ref Pmic_ErrorCodes
 */
static int32_t test_pmic_hera_dual_i2c_pin_setup(Pmic_CoreHandle_t *pPmicHandle)
{
    int32_t pmicStatus     = PMIC_ST_SUCCESS;
    Pmic_GpioCfg_t gpioCfg = {0U};

    gpioCfg.validParams = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT |
                          PMIC_GPIO_CFG_OD_VALID_SHIFT;

    pmicStatus = Pmic_gpioGetConfiguration(pPmicHandle,
                                           PMIC_LP8764X_GPIO2_PIN,
                                           &gpioCfg);

    if (PMIC_INTF_SINGLE_I2C == pPmicHandle->commMode)
    {
        if (gpioCfg.pinFunc == PMIC_LP8764X_GPIO_PINFUNC_GPIO2_SCL_I2C2)
        {
            gpioCfg.pinFunc = PMIC_LP8764X_GPIO_PINFUNC_GPIO;
        }
    }

    if (PMIC_INTF_DUAL_I2C == pPmicHandle->commMode)
    {
        gpioCfg.pinFunc = PMIC_LP8764X_GPIO_PINFUNC_GPIO2_SCL_I2C2;
        gpioCfg.outputSignalType = PMIC_GPIO_OPEN_DRAIN_OUTPUT;
    }

    pmicStatus = Pmic_gpioSetConfiguration(pPmicHandle,
                                           PMIC_LP8764X_GPIO2_PIN,
                                           gpioCfg);

    gpioCfg.validParams = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT |
                          PMIC_GPIO_CFG_OD_VALID_SHIFT;

    if (PMIC_ST_SUCCESS == pmicStatus)
    {
        pmicStatus = Pmic_gpioGetConfiguration(pPmicHandle,
                                               PMIC_LP8764X_GPIO3_PIN,
                                               &gpioCfg);
    }

    if (PMIC_ST_SUCCESS == pmicStatus)
    {
        if (PMIC_INTF_SINGLE_I2C == pPmicHandle->commMode)
        {
            if (gpioCfg.pinFunc == PMIC_LP8764X_GPIO_PINFUNC_GPIO3_SDA_I2C2)
            {
                gpioCfg.pinFunc = PMIC_LP8764X_GPIO_PINFUNC_GPIO;
            }
        }

        if (PMIC_INTF_DUAL_I2C == pPmicHandle->commMode)
        {
            gpioCfg.pinFunc = PMIC_LP8764X_GPIO_PINFUNC_GPIO3_SDA_I2C2;
            gpioCfg.outputSignalType = PMIC_GPIO_OPEN_DRAIN_OUTPUT;
        }

        pmicStatus = Pmic_gpioSetConfiguration(pPmicHandle,
                                               PMIC_LP8764X_GPIO3_PIN,
                                               gpioCfg);
    }

    return pmicStatus;
}

/*!
 * \brief   Function to release the I2C lld interface for PMIC
 *
 * \param   pCommHandle    [OUT]     PMIC Interface Handle.
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code.
 *          For valid values see \ref Pmic_ErrorCodes
 */
static int32_t test_pmic_i2c_lld_intf_release(void **pCommHandle)
{
    if (NULL == (I2C_Handle) * pCommHandle)
    {
        return PMIC_ST_ERR_NULL_PARAM;
    }

    I2C_close((I2C_Handle) * pCommHandle);

    *pCommHandle = NULL;

    return PMIC_ST_SUCCESS;
}

/*!
 * \brief   PMIC I2C read function.
 *
 * \param   pmicCorehandle [IN]     PMIC Interface Handle.
 * \param   commMode       [IN]     Communication Mode
 * \param   slaveAddr      [IN]     I2c device slave address
 * \param   regAddr        [OUT]    Register address to read from.
 * \param   pBuf           [IN]     Buffer to store data
 * \param   bufLen         [IN]     Length of data to read.
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code
 *          For valid values see \ref Pmic_ErrorCodes
 */
int32_t test_pmic_regRead(Pmic_CoreHandle_t  *pmicCorehandle,
                          uint8_t             instType,
                          uint16_t            regAddr,
                          uint8_t            *pBuf,
                          uint8_t             bufLen)
{
    int8_t ret = 0U;

    if ((PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode) ||
        (PMIC_INTF_DUAL_I2C   == pmicCorehandle->commMode))
    {
        I2C_Transaction transaction;
        I2C_transactionInit(&transaction);

        /* Set register offset for read and write */
        transaction.readBuf    = pBuf;
        transaction.readCount  = bufLen;
        transaction.writeBuf   = &regAddr;
        transaction.writeCount = 1U;

        /* Main I2c BUS */
        if (PMIC_MAIN_INST == instType)
        {
            transaction.slaveAddress = pmicCorehandle->slaveAddr;
            ret = I2C_transfer((I2C_Handle)pmicCorehandle->pCommHandle,
                               &transaction);

            if (ret != I2C_STS_SUCCESS)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }
        }

        /* For WDOG QA I2C BUS */
        if (PMIC_QA_INST == instType)
        {
            transaction.slaveAddress = pmicCorehandle->qaSlaveAddr;

            if (PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode)
            {
                ret = I2C_transfer((I2C_Handle)
                                   pmicCorehandle->pCommHandle,
                                   &transaction);
            }

            if (PMIC_INTF_DUAL_I2C == pmicCorehandle->commMode)
            {
                ret = I2C_transfer((I2C_Handle)
                                   pmicCorehandle->pQACommHandle,
                                   &transaction);
            }

            if (ret != I2C_STS_SUCCESS)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }
        }
    }

    if (PMIC_INTF_SPI == pmicCorehandle->commMode)
    {
        if (PMIC_ST_SUCCESS !=
            test_pmic_spi_stubRead(pmicCorehandle, pBuf, bufLen))
        {
            /* For Fault Injection Tests */
            if (BTRUE == enableFaultInjectionRead)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }

            return PMIC_ST_ERR_SPI_COMM_FAIL;
        }
    }

    /* Added for Branch Coverage */
    readCount++;

    if ((BTRUE == enableFaultInjectionRead) && (skipReadCount == readCount))
    {
        return PMIC_ST_ERR_I2C_COMM_FAIL;
    }

    return PMIC_ST_SUCCESS;
}

/*!
 * \brief   PMIC I2C write function.
 *
 * \param   pmicCorehandle   [IN]     PMIC Interface Handle.
 * \param   commMode         [IN]     Communication Mode
 * \param   slaveAddr        [IN]     I2c device slave address
 * \param   regAddr          [IN]     Register address to write.
 * \param   pBuf             [IN]     Buffer to store data to write
 * \param   bufLen           [IN]     Length of data to write.
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code
 *          For valid values see \ref Pmic_ErrorCodes
 */
int32_t test_pmic_regWrite(Pmic_CoreHandle_t  *pmicCorehandle,
                           uint8_t             instType,
                           uint16_t            regAddr,
                           uint8_t            *pBuf,
                           uint8_t             bufLen)
{
    int8_t  ret = 0U;

    if ((PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode) ||
        (PMIC_INTF_DUAL_I2C   == pmicCorehandle->commMode))
    {
        I2C_Transaction transaction;

        /* regAddr, 8-bit-Data, 8-bit-CRC(optional) => max 3 bytes
         * Taking a databuff of size 3 only as it is enough
         */
        uint8_t dataBuff[16U] = {0U};

        /* Initializes the I2C transaction structure with default values */
        I2C_transactionInit(&transaction);

        if (PMIC_MAIN_INST == instType)
        {
            transaction.slaveAddress = pmicCorehandle->slaveAddr;
        }

        if (PMIC_QA_INST == instType)
        {
            transaction.slaveAddress = pmicCorehandle->qaSlaveAddr;
        }

        dataBuff[0U] = regAddr;
        memcpy(&dataBuff[1U], pBuf, bufLen);

        /* Control Byte followed by write bit */
        transaction.writeBuf   = dataBuff;
        transaction.writeCount = bufLen + 1U;
        transaction.readCount  = 0U;
        transaction.readBuf    = NULL;

        /* Main I2c BUS */
        if (PMIC_MAIN_INST == instType)
        {
            ret = I2C_transfer((I2C_Handle)pmicCorehandle->pCommHandle,
                               &transaction);

            if (ret != I2C_STS_SUCCESS)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }
        }

        /* For WDOG QA I2C BUS */
        if (PMIC_QA_INST == instType)
        {
            if (PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode)
            {
                ret = I2C_transfer((I2C_Handle)
                                   pmicCorehandle->pCommHandle,
                                   &transaction);
            }

            if (PMIC_INTF_DUAL_I2C == pmicCorehandle->commMode)
            {
                ret = I2C_transfer((I2C_Handle)
                                   pmicCorehandle->pQACommHandle,
                                   &transaction);
            }

            if (ret != I2C_STS_SUCCESS)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }
        }
    }

    if (PMIC_INTF_SPI == pmicCorehandle->commMode)
    {
        if (PMIC_ST_SUCCESS !=
            test_pmic_spi_write(pmicCorehandle, pBuf, bufLen))
        {
            /* For Fault Injection Tests */
            if (BTRUE == enableFaultInjectionWrite)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }

            return PMIC_ST_ERR_SPI_COMM_FAIL;
        }
    }

    /* Added for Branch Coverage */
    writeCount++;

    if ((BTRUE == enableFaultInjectionWrite) && (skipWriteCount == writeCount))
    {
        return PMIC_ST_ERR_I2C_COMM_FAIL;
    }

    return PMIC_ST_SUCCESS;
}

/*!
 * \brief   Interface setup function for PMIC to create instance and
 *          initialise the SPI Bus for PMIC Communication
 *
 * \param   pmicCorehandle    [IN]     PMIC Interface Handle.
 * \param   commMode          [IN]     Communication Mode
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code.
 *          For valid values see \ref Pmic_ErrorCodes
 */
static int32_t test_pmic_spi_lld_intf_setup(Pmic_CoreCfg_t *pPmicConfigData)
{
    int32_t ret = PMIC_ST_SUCCESS;

    ret = test_pmic_spi_stubInit(pPmicConfigData);

    return ret;
}

/*!
 * \brief   Interface release function for PMIC to release the SPI Bus
 *
 * \param   pCommHandle    [IN]     PMIC Interface Handle.
 * \param   commMode       [IN]     Communication Mode
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code.
 *          For valid values see \ref Pmic_ErrorCodes
 */
static int32_t test_pmic_spi_lld_intf_release(void **pCommHandle)
{
    int32_t ret = PMIC_ST_SUCCESS;

    ret = test_pmic_spi_stubDeinit(pCommHandle);

    return ret;
}

/*!
 * \brief   Initialize Semaphore Handle
 */
static void test_pmic_osalSemaphoreInit(void)
{
    SemaphoreP_Params pmic_SemParams;

    /* Create call back semaphore */
    SemaphoreP_Params_init(&pmic_SemParams);
    pmic_SemParams.mode = SemaphoreP_Mode_BINARY;
    pmic_Sem = SemaphoreP_create(1U, &pmic_SemParams);
}

/*!
 * \brief   DeInit Semaphore Handle
 */
static void test_pmic_osalSemaphoreDeInit(void)
{
    if (pmic_Sem)
    {
        SemaphoreP_delete(pmic_Sem);
        pmic_Sem = NULL;
    }
}

/*!
 * \brief   PMIC Critical section Lock function
 *          This function should have OS specific locking setup and should
 *          assigned to 'pmicConfigData->pFnPmicCritSecStart'
 */
void test_pmic_criticalSectionStartFn(void)
{
    if (SemaphoreP_OK != SemaphoreP_pend(pmic_Sem,
                                         SemaphoreP_WAIT_FOREVER))
    {
        pmic_log("%s(): Invalid Semaphore Handle\n", __func__);
    }
}

/*!
 * \brief   PMIC Critical section Unlock function
 *          This function should have OS specific locking setup and should
 *          assigned to 'pmicConfigData->pFnPmicCritSecStop'
 */
void test_pmic_criticalSectionStopFn(void)
{
    if (SemaphoreP_OK != SemaphoreP_post(pmic_Sem))
    {
        pmic_log("%s(): Invalid Semaphore Handle\n", __func__);
    }
}

/*!
 * \brief   Get PMIC StartUp Interrupt Type.
 *          This function deciphers all interrupts and find startup type.
 */
static uint32_t get_startup_type(Pmic_CoreHandle_t *pPmicCoreHandle)
{
    Pmic_IrqStatus_t errStat = {0U};
    int32_t pmicStatus       = PMIC_ST_SUCCESS;
    uint8_t type = 0U;

    pmicStatus = Pmic_irqGetErrStatus(pPmicCoreHandle, &errStat, BFALSE);

    switch (pPmicCoreHandle->pmicDeviceType)
    {
        case PMIC_DEV_LEO_TPS6594X:

            switch (startup_type)
            {
                case PMIC_ENABLE_STARTUP_TYPE:

                    if ((PMIC_ST_SUCCESS == pmicStatus) &&
                        ((errStat.intStatus[PMIC_TPS6594X_ENABLE_INT / 32U] &
                          (1U << (PMIC_TPS6594X_ENABLE_INT % 32U))) != 0U))
                    {
                        type = PMIC_ENABLE_STARTUP_TYPE;
                    }

                    break;

                case PMIC_NPWRON_STARTUP_TYPE:

                    if ((PMIC_ST_SUCCESS == pmicStatus) &&
                        ((errStat.intStatus[PMIC_TPS6594X_NPWRON_START_INT /
                                            32U] &
                          (1U << (PMIC_TPS6594X_NPWRON_START_INT % 32U))) !=
                         0U))
                    {
                        type = PMIC_NPWRON_STARTUP_TYPE;
                    }

                    break;

                case PMIC_FSD_STARTUP_TYPE:

                    if ((PMIC_ST_SUCCESS == pmicStatus) &&
                        ((errStat.intStatus[PMIC_TPS6594X_FSD_INT / 32U] &
                          (1U << (PMIC_TPS6594X_FSD_INT % 32U))) != 0U))
                    {
                        type = PMIC_FSD_STARTUP_TYPE;
                    }

                    break;

                default:
                    type = 0U;
                    break;
            }

            break;

        case PMIC_DEV_HERA_LP8764X:

            switch (startup_type)
            {
                case PMIC_ENABLE_STARTUP_TYPE:

                    if ((PMIC_ST_SUCCESS == pmicStatus) &&
                        ((errStat.intStatus[PMIC_LP8764X_ENABLE_INT / 32U] &
                          (1U << (PMIC_LP8764X_ENABLE_INT % 32U))) != 0U))
                    {
                        type = PMIC_ENABLE_STARTUP_TYPE;
                    }

                    break;

                case PMIC_FSD_STARTUP_TYPE:

                    if ((PMIC_ST_SUCCESS == pmicStatus) &&
                        ((errStat.intStatus[PMIC_LP8764X_FSD_INT / 32U] &
                          (1U << (PMIC_LP8764X_FSD_INT % 32U))) != 0U))
                    {
                        type = PMIC_FSD_STARTUP_TYPE;
                    }

                    break;

                default:
                    type = 0U;
                    break;
            }

            break;
    }

    startup_type = type;

    return pmicStatus;
}

/*!
 * \brief   PMIC Interrupt decipher and clear function
 *          This function deciphers all interrupts and clears the status
 */
static int32_t Pmic_intrClr(Pmic_CoreHandle_t *pmicHandle)
{
    int32_t pmicStatus       = PMIC_ST_SUCCESS;
    Pmic_CoreHandle_t handle = *(Pmic_CoreHandle_t *)pmicHandle;
    Pmic_IrqStatus_t errStat = {0U};

    if (startup_type != 0U)
    {
        pmicStatus = get_startup_type(pmicHandle);
    }

    if (PMIC_ST_SUCCESS == pmicStatus)
    {
        pmicStatus = Pmic_irqGetErrStatus(&handle, &errStat, BTRUE);
        {
            int i = 0;

            for (i = 0; i < 4; i++)
            {
                pmic_log("INT STAT[%d]: 0x%08x\n", i, errStat.intStatus[i]);
            }
        }
    }

    pmic_log("\r\n");

    return pmicStatus;
}

/*!
 * \brief   Initialize PMIC Instance and corresponding Interface.
 *
 * \param   pmicCoreHandle    [OUT]     PMIC Core Handle.
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code.
 *          For valid values see \ref Pmic_ErrorCodes
 */
int32_t test_pmic_appInit(Pmic_CoreHandle_t **pmicCoreHandle,
                          Pmic_CoreCfg_t     *pmicConfigData)
{
    int32_t pmicStatus = PMIC_ST_SUCCESS;
    Pmic_CoreHandle_t *pmicHandle = NULL;

    /* Initialize Pmic Semaphore */
    test_pmic_osalSemaphoreInit();

    if (pmicCoreHandle == NULL)
    {
        pmic_log("Invalid PMIC core Handle Reference\n");

        return PMIC_ST_ERR_INV_HANDLE;
    }

    if (pmicConfigData == NULL)
    {
        pmic_log("Invalid PMIC config Data - NULL \n");

        return PMIC_ST_ERR_NULL_PARAM;
    }

    /* Allocate memory for PMIC core Handle */
    pmicHandle = malloc(sizeof(Pmic_CoreHandle_t));

    if (pmicHandle == NULL)
    {
        pmic_log("Failed to allocate memory to pmicHandle\n");

        return PMIC_ST_ERR_INV_HANDLE;
    }

    memset(pmicHandle, 0, sizeof(Pmic_CoreHandle_t));

    /* For single I2C Instance */
    if (PMIC_INTF_SINGLE_I2C == pmicConfigData->commMode)
    {
        uint64_t delta = 0;
        /* Get PMIC valid Main I2C Instance */
        pmicStatus = test_pmic_i2c_lld_intf_setup(pmicConfigData,
                                                  PMIC_MAIN_INST);

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            pmicConfigData->validParams |= PMIC_CFG_COMM_HANDLE_VALID_SHIFT;
            /* Update instance type to pmicConfigData */
            pmicConfigData->instType     = PMIC_MAIN_INST;

            if (BTRUE == enableBenchMark)
            {
                uint64_t t1 = 0;
                t1         = print_timeTakenInUsecs(0U, NULL);
                /* Get PMIC core Handle for Main Instance */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
                delta      = print_timeTakenInUsecs(t1, NULL);
                pmic_log("--------------------------------------\n");
                pmic_log("Time taken for %50s: %6d usec\n",
                         "Pmic_init API for single instance",
                         (uint32_t)delta);
                pmic_log("--------------------------------------\n");
            }
            else
            {
                /* Get PMIC core Handle for Main Instance */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
            }

            /*
             * Check for Warning message due to Invalid Device ID.
             * And continue the application with WARNING message.
             */
            if (PMIC_ST_WARN_INV_DEVICE_ID == pmicStatus)
            {
                pmic_log("\n*** WARNING: Found Invalid DEVICE ID ***\n\n");
                pmicStatus = PMIC_ST_SUCCESS;
            }
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            /* Probe connected PMIC device on given i2c Instance */
            test_pmic_i2c_devices(pmicHandle, PMIC_MAIN_INST);
        }

        if (PMIC_DEV_LEO_TPS6594X == pmicHandle->pmicDeviceType)
        {
            /* Check and De-select I2C2 PINFUNC for GPIO-1 and GPIO-2 */
            pmicStatus = test_pmic_leo_dual_i2c_pin_setup(pmicHandle);
        }

        if (PMIC_DEV_HERA_LP8764X == pmicHandle->pmicDeviceType)
        {
            /* Check and De-select I2C2 PINFUNC for GPIO-2 and GPIO-3 */
            pmicStatus = test_pmic_hera_dual_i2c_pin_setup(pmicHandle);
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            /* Setup nSLEEP signals */
            pmicStatus = Pmic_fsmDeviceOnRequest(pmicHandle);
        }

        if (PMIC_ST_SUCCESS == pmicStatus && PMIC_DEV_HERA_LP8764X != pmicHandle->pmicDeviceType)
        {
            pmicStatus = Pmic_intrClr(pmicHandle);
        }
    }
    /* For DUAL I2C Instance */
    else if (PMIC_INTF_DUAL_I2C == pmicConfigData->commMode)
    {
        uint64_t delta = 0;
        /* Get PMIC valid Main I2C Instance */
        pmicStatus = test_pmic_i2c_lld_intf_setup(pmicConfigData,
                                                  PMIC_MAIN_INST);

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            pmicConfigData->validParams |= PMIC_CFG_COMM_HANDLE_VALID_SHIFT;
            /* Update instance type to pmicConfigData */
            pmicConfigData->instType     = PMIC_MAIN_INST;

            if (BTRUE == enableBenchMark)
            {
                uint64_t t1 = 0;
                t1         = print_timeTakenInUsecs(0U, NULL);
                /* Get PMIC core Handle for Main Instance */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
                delta      = print_timeTakenInUsecs(t1, NULL);
            }
            else
            {
                /* Get PMIC core Handle for Main Instance */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
            }

            /*
             * Check for Warning message due to Invalid Device ID.
             * And continue the application with WARNING message.
             */
            if (PMIC_ST_WARN_INV_DEVICE_ID == pmicStatus)
            {
                pmic_log("\n*** WARNING: Found Invalid DEVICE ID ***\n\n");
                pmicStatus = PMIC_ST_SUCCESS;
            }
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            /* Probe connected PMIC device on given i2c Instance */
            test_pmic_i2c_devices(pmicHandle, PMIC_MAIN_INST);
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            /* Setup nSLEEP signals */
            pmicStatus = Pmic_fsmDeviceOnRequest(pmicHandle);
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            pmicStatus = Pmic_intrClr(pmicHandle);
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            if (PMIC_DEV_LEO_TPS6594X == pmicHandle->pmicDeviceType)
            {
                /* Setup leo pmic Dual I2C functionality to GPIO-1 & GPIO-2 */
                pmicStatus = test_pmic_leo_dual_i2c_pin_setup(pmicHandle);
            }

            if (PMIC_DEV_HERA_LP8764X == pmicHandle->pmicDeviceType)
            {
                /* Setup hera pmic Dual I2C functionality to GPIO-2 & GPIO-3 */
                pmicStatus = test_pmic_hera_dual_i2c_pin_setup(pmicHandle);
            }
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            /* Get PMIC valid QA I2C Instance */
            pmicStatus = test_pmic_i2c_lld_intf_setup(pmicConfigData,
                                                      PMIC_QA_INST);
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            pmicConfigData->validParams |= PMIC_CFG_QACOMM_HANDLE_VALID_SHIFT;
            /* Update instance type to pmicConfigData */
            pmicConfigData->instType     = PMIC_QA_INST;

            if (BTRUE == enableBenchMark)
            {
                uint64_t t1 = 0;
                t1         = print_timeTakenInUsecs(0U, NULL);
                /* Get PMIC core Handle for QA Instances */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
                delta     += print_timeTakenInUsecs(t1, NULL);
                pmic_log("--------------------------------------\n");
                pmic_log("Time taken for %50s: %6d usec\n",
                         "Pmic_init API for Dual instance",
                         (uint32_t)delta);
                pmic_log("--------------------------------------\n");
            }
            else
            {
                /* Get PMIC core Handle for QA Instances */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
            }
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            /* Probe connected PMIC device on given i2c Instance */
            test_pmic_i2c_devices(pmicHandle, PMIC_QA_INST);
        }
    }
    /* For SPI Instance */
    else if (PMIC_INTF_SPI  == pmicConfigData->commMode)
    {
        /* Get PMIC valid Main SPI Communication Handle */
        pmicStatus = test_pmic_spi_lld_intf_setup(pmicConfigData);

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            /* Update MAIN instance type to pmicConfigData for SPI */
            pmicConfigData->instType = PMIC_MAIN_INST;
            pmicStatus = Pmic_init(pmicConfigData, pmicHandle);

            /*
             * Check for Warning message due to Invalid Device ID.
             * And continue the application with WARNING message.
             */
            if (PMIC_ST_WARN_INV_DEVICE_ID == pmicStatus)
            {
                pmic_log("\n*** WARNING: Found Invalid DEVICE ID ***\n\n");
                pmicStatus = PMIC_ST_SUCCESS;
            }
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            /* Setup nSLEEP signals */
            pmicStatus = Pmic_fsmDeviceOnRequest(pmicHandle);
        }

        if (PMIC_ST_SUCCESS == pmicStatus)
        {
            pmicStatus = Pmic_intrClr(pmicHandle);
        }
    }

    if (PMIC_ST_SUCCESS != pmicStatus)
    {
        pmic_log("%s(): %d: FAILED with status: %d\n",
                 __func__, __LINE__,  pmicStatus);
    }

    *pmicCoreHandle = pmicHandle;

    return pmicStatus;
}

void test_pmic_appDeInit(Pmic_CoreHandle_t *pmicCoreHandle)
{
    if (PMIC_INTF_SINGLE_I2C == pmicCoreHandle->commMode)
    {
        test_pmic_i2c_lld_intf_release(&pmicCoreHandle->pCommHandle);
    }
    else if (PMIC_INTF_DUAL_I2C == pmicCoreHandle->commMode)
    {
        test_pmic_i2c_lld_intf_release(&pmicCoreHandle->pCommHandle);
        test_pmic_i2c_lld_intf_release(&pmicCoreHandle->pQACommHandle);
    }
    else if (PMIC_INTF_SPI  == pmicCoreHandle->commMode)
    {
        test_pmic_spi_lld_intf_release(&pmicCoreHandle->pCommHandle);
    }

    Pmic_deinit(pmicCoreHandle);

    free(pmicCoreHandle);

    /* PMIC Semaphore Clean-up */
    test_pmic_osalSemaphoreDeInit();
}

/*!
 * \brief   Unity setUp Function
 */
void setUp(void)
{
    /* Do nothing */
}

/*!
 * \brief   Unity tearDown Function
 */
void tearDown(void)
{
    /* Do nothing */
}

/*!
 * \brief   GPIO Interrupt Router Configuration
 */
void GPIO_configIntRouter(uint32_t portNum,
                          uint32_t pinNum,
                          uint32_t gpioIntRtrOutIntNum,
                          GPIO_v0_HwAttrs *cfg)
{
    GPIO_IntCfg       *intCfg;

#if (defined(SOC_J721E) || defined(SOC_J7200))
    uint32_t           bankNum;
    bankNum       = pinNum / 16; /* Each GPIO bank has 16 pins */
#endif

    intCfg        = cfg->intCfg;

    cfg->baseAddr = CSL_WKUP_GPIO0_BASE;

    /* WKUP GPIO int router input interrupt is the GPIO bank interrupt */
#if defined (SOC_J721E)
#if defined (BUILD_MCU1_0)
    intCfg[pinNum].intNum =
        CSLR_MCU_R5FSS0_CORE0_INTR_WKUP_GPIOMUX_INTRTR0_OUTP_0 + bankNum;
#elif defined (BUILD_MCU1_1)
    intCfg[pinNum].intNum =
        CSLR_MCU_R5FSS0_CORE1_INTR_WKUP_GPIOMUX_INTRTR0_OUTP_6 + bankNum;
#elif defined (BUILD_MCU2_0)
    intCfg[pinNum].intNum = CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_0 +
                            bankNum;
    pmic_log("intCfg[pinNum].intNum: %d\n", intCfg[pinNum].intNum);
#elif defined (BUILD_MCU2_1)
    intCfg[pinNum].intNum = CSLR_R5FSS0_CORE1_INTR_R5FSS0_INTROUTER0_OUTL_1 +
                            bankNum;
#elif defined (BUILD_MCU3_0)
    intCfg[pinNum].intNum = CSLR_R5FSS1_CORE0_INTR_R5FSS1_INTROUTER0_OUTL_0 +
                            bankNum;
#elif defined (BUILD_MCU3_1)
    intCfg[pinNum].intNum = CSLR_R5FSS1_CORE1_INTR_R5FSS1_INTROUTER0_OUTL_1 +
                            bankNum;
#endif
#elif defined (SOC_J7200)
#if defined (BUILD_MCU1_0)
    intCfg[pinNum].intNum =
        CSLR_MCU_R5FSS0_CORE0_INTR_WKUP_GPIOMUX_INTRTR0_OUTP_0 + bankNum;
#elif defined (BUILD_MCU1_1)
    intCfg[pinNum].intNum =
        CSLR_MCU_R5FSS0_CORE1_INTR_WKUP_GPIOMUX_INTRTR0_OUTP_3 + bankNum;
#elif defined (BUILD_MCU2_0)
    /* Added dummy intNum. Need to correct intNum to MCU2_0 */
    intCfg[pinNum].intNum =
        CSLR_R5FSS0_CORE0_INTR_WKUP_GPIOMUX_INTRTR0_OUTP_16 +
        bankNum;
#elif defined (BUILD_MCU2_1)
    /* Added dummy intNum. Need to correct intNum to MCU2_1 */
    intCfg[pinNum].intNum =
        CSLR_R5FSS0_CORE1_INTR_WKUP_GPIOMUX_INTRTR0_OUTP_16 +
        bankNum;
#endif
#endif

    intCfg[pinNum].intcMuxNum      = INVALID_INTC_MUX_NUM;
    intCfg[pinNum].intcMuxInEvent  = 0;
    intCfg[pinNum].intcMuxOutEvent = 0;
}

/*!
 * \brief   UART Configurations
 */
void Board_initUART(void)
{
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK  |
               BOARD_INIT_UART_STDIO;

    Board_init(boardCfg);
}

/*!
 * \brief   GPIO Configurations.
 *          This API is required for Asynchronous Interrupts only
 */
void App_initGPIO(GPIO_CallbackFxn callback)
{
    GPIO_v0_HwAttrs gpio_cfg;

    /* Get the default SPI init configurations */
    GPIO_socGetInitCfg(J7_WAKEUP_GPIO0_PORT_NUM, &gpio_cfg);

    /* change default GPIO port from MAIN GPIO0 to WAKEUP GPIO0 for Intrrupts */
    gpio_cfg.baseAddr = CSL_WKUP_GPIO0_BASE;

#if defined(SOC_J721E)
    GPIO_configIntRouter(J7_WAKEUP_GPIO0_PORT_NUM,
                         J7_WAKEUP_GPIO0_9_PIN_NUM,
                         0,
                         &gpio_cfg);
#endif
#if defined(SOC_J7200)
    GPIO_configIntRouter(J7_WAKEUP_GPIO0_PORT_NUM,
                         J7_WAKEUP_GPIO0_84_PIN_NUM,
                         0,
                         &gpio_cfg);
#endif

    /* Set as the default GPIO init configurations */
    GPIO_socSetInitCfg(J7_WAKEUP_GPIO0_PORT_NUM, &gpio_cfg);

    /* GPIO initialization */
    GPIO_init();

    /* Set the callback function */
    GPIO_setCallback(0, callback);
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}

#endif

static uint32_t test_pmic_get_unity_testcase_index(uint32_t         testId,
                                                   Pmic_Ut_Tests_t *pTest,
                                                   uint32_t         num_testcases)
{
    uint32_t i;
    int32_t status = PMIC_ST_SUCCESS;

    for (i = 0U;; i++)
    {
        if (pTest->testId == testId)
        {
            pTest->testValid = BTRUE;
            break;
        }
        else if (i >= num_testcases)
        {
            status = PMIC_ST_ERR_FAIL;
            /* Print test Result */
            pmic_log("\n |TEST RESULT|:: %d ::", testId);
            TEST_ASSERT_EQUAL_MESSAGE(PMIC_ST_SUCCESS,
                                      status,
                                      "Invalid Test ID");
        }

        pTest++;
    }

    return i;
}

/*!
 * \brief   Function to print testcase info
 */
void test_pmic_print_unity_testcase_info(uint32_t         testId,
                                         Pmic_Ut_Tests_t *pTest,
                                         uint32_t         num_testcases)
{
    uint32_t index = 0U;

    /* Print unit test ID */
    pmic_log("\n\n |TEST ID|:: %d ::\n", testId);

    index = test_pmic_get_unity_testcase_index(testId, pTest, num_testcases);

    /* Print test description */
    pmic_log("\n |TEST NAME|:: %s ::\n", pTest[index].testDesc);

    /* Print test Result */
    pmic_log("\n |TEST_RESULT|:: %d ::", testId);
}

static void pmic_testResultUpdate(uint32_t testId,
                                  uint8_t  result,
                                  Pmic_Ut_Tests_t *pTest,
                                  uint32_t num_testcases)
{
    uint32_t idx = 0U;

    while (idx < num_testcases)
    {
        if (pTest[idx].testId == testId)
        {
            pTest[idx].testValid  = BTRUE;
            pTest[idx].testResult = result;
        }

        idx++;
    }
}

void pmic_testResultUpdate_ignore(uint32_t testId,
                                  Pmic_Ut_Tests_t *pTest,
                                  uint32_t num_testcases)
{
    uint8_t  result = PMIC_TEST_RESULT_IGNORE;

    pmic_testResultUpdate(testId, result, pTest, num_testcases);
    TEST_IGNORE();
}

void pmic_testResultUpdate_pass(uint32_t testId,
                                Pmic_Ut_Tests_t *pTest,
                                uint32_t num_testcases)
{
    uint8_t  result = PMIC_TEST_RESULT_PASS;

    pmic_testResultUpdate(testId, result, pTest, num_testcases);
}

void pmic_testResult_init(Pmic_Ut_Tests_t *pTest, uint32_t num_testcases)
{
    uint32_t idx = 0;

    for (idx = 0; idx < num_testcases; idx++)
    {
        pTest[idx].testValid  = BFALSE;
        pTest[idx].testResult = PMIC_TEST_RESULT_FAIL;
    }
}

void pmic_updateTestResults(Pmic_Ut_Tests_t *pTest, uint32_t num_testcases)
{
    uint32_t idx = 0;

    /* Print test Result */
    for (idx = 0; idx < num_testcases; idx++)
    {
        if (BTRUE == pTest[idx].testValid)
        {
            if (BTRUE == pTest[idx].finalTestValid)
            {
                if ((PMIC_TEST_RESULT_FAIL == pTest[idx].testResult) ||
                    (PMIC_TEST_RESULT_FAIL == pTest[idx].finalTestResult))
                {
                    pTest[idx].finalTestResult = PMIC_TEST_RESULT_FAIL;
                }
                else if (PMIC_TEST_RESULT_PASS == pTest[idx].testResult)
                {
                    pTest[idx].finalTestResult = pTest[idx].testResult;
                }
            }
            else
            {
                pTest[idx].finalTestValid  = pTest[idx].testValid;
                pTest[idx].finalTestResult = pTest[idx].testResult;
            }
        }
    }
}

void pmic_printTestResult(Pmic_Ut_Tests_t *pTest, uint32_t num_testcases)
{
    uint32_t idx        = 0;
    uint8_t testResult  = 0;
    char *result_str[3] = {"FAIL", "PASS", "IGNORE"};
    uint32_t testCnt    = 0;
    uint32_t failCnt    = 0;

    pmic_log("\n\r");

    pmic_log("Test Results\n");
    pmic_log("-----------------------\n\n");

    /* Print test Result */
    for (idx = 0; idx < num_testcases; idx++)
    {
        if (BTRUE == pTest[idx].finalTestValid)
        {
            testResult = pTest[idx].finalTestResult;

            if (PMIC_TEST_RESULT_FAIL == pTest[idx].finalTestResult)
            {
                failCnt++;
            }
            else if (PMIC_TEST_RESULT_IGNORE == pTest[idx].finalTestResult)
            {
                continue;
            }

            testCnt++;
            pmic_log("|TEST RESULT|%s|%d|\n",
                     result_str[testResult], pTest[idx].testId);
        }
    }

    pmic_log("-----------------------\n");
    pmic_log("%d Tests %d Failures\n",
             testCnt, failCnt);

    if (0U == failCnt)
    {
        pmic_log("All tests have passed\n");
    }
    else
    {
        pmic_log("Few tests are Failed\n");
    }
}

/*!
 * \brief    : Prints time taken for a given Valid string and returns delta.
 */
uint64_t print_timeTakenInUsecs(uint64_t t1, const char *str)
{
    uint64_t t2    = 0;
    uint64_t delta = 0;

    t2    = TimerP_getTimeInUsecs();
    delta = t2 - t1;

    if (NULL != str)
    {
        pmic_log("Time taken for %50s: %6d usec\n", str, (uint32_t)delta);
    }

    return delta;
}

/*!
 * \brief   print Banner Message
 */
void pmic_print_banner(const char *str)
{
    pmic_log("\n\n%s running on %s of %s(%s %s)\n",
             str,
#if defined (BUILD_MCU1_0)
             "MCU1_0",
#elif defined (BUILD_MCU1_1)
             "MCU1_1",
#elif defined (BUILD_MCU2_0)
             "MCU2_0",
#elif defined (BUILD_MCU2_1)
             "MCU2_1",
#elif defined (BUILD_MCU3_0)
             "MCU3_0",
#elif defined (BUILD_MCU3_1)
             "MCU3_1",
#endif
#if defined(SOC_J721E)
             "J721E_EVM",
#elif defined(SOC_J7200)
             "J7VCL_EVM",
#endif
             __TIME__,
             __DATE__);
}
