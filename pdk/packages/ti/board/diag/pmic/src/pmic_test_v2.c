/**
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   pmic_test_v2.c
 *
 *  \brief  PMIC diagnostic test file
 *
 *  Implements the test for validating the PMIC access and voltage control
 *
 */

#include "pmic_test_v2.h"

static int32_t gNumPmic = 1;
Board_I2cInitCfg_t boardI2cInitCfg = {0, BOARD_SOC_DOMAIN_WKUP, BFALSE};
BoardDiag_PmicData_t *gDualPmicData;

extern I2C_config_list I2C_config;

/**
 * \brief	pmic data for TPS6594X PMIC A
 */
BoardDiag_PmicData_t tps65941_pmicA = {
    PMIC_DEV_LEO_TPS6594X,
    PMIC_INTF_DUAL_I2C,
    BOARD_I2C_LEO_PMIC_A_ADDR,
    BOARD_I2C_LEO_PMIC_A_WDT_ADDR,
	PMIC_TPS6594X_REGULATOR_BUCK5,
	BOARD_DIAG_TPS6594X_PMICA_VOLTAGE_VAL
};

/**
 * \brief	pmic data for TPS6594X PMIC B
 */
BoardDiag_PmicData_t tps65941_pmicB = {
    PMIC_DEV_LEO_TPS6594X,
    PMIC_INTF_SINGLE_I2C,
    BOARD_I2C_LEO_PMIC_B_ADDR,
    BOARD_I2C_LEO_PMIC_B_WDT_ADDR,
	PMIC_TPS6594X_REGULATOR_BUCK1,
	BOARD_DIAG_TPS6594X_PMICB_VOLTAGE_VAL
};

/**
 * \brief	pmic data for LP8764X PMIC B
 */
BoardDiag_PmicData_t lp8764x_pmicB = {
    PMIC_DEV_HERA_LP8764X,
    PMIC_INTF_SINGLE_I2C,
    BOARD_I2C_LEO_PMIC_B_ADDR,
    BOARD_I2C_LEO_PMIC_B_WDT_ADDR,
	PMIC_LP8764X_REGULATOR_BUCK4,
	BOARD_DIAG_LP8764X_PMICB_VOLTAGE_VAL
};

/**
 * \brief Pmic 8-bit register read function
 *
 * This function is used to read the 8-bit data from the pmic
 * device registers
 *
 * \param   handle      [IN]    Low level driver handle
 * \param   slaveAddr   [IN]    I2C slave address
 * \param   regAddr     [IN]    I2C register offset address
 * \param   regData     [OUT]   I2C register data buffer
 * \param   numOfBytes  [IN]    Receive data width
 *
 * \return  BOARD_SOK in case of success or appropriate error code.
 */
Board_STATUS BoardDiag_pmicRegRd(void *handle,
								 uint32_t slaveAddr,
								 uint8_t regAddr,
								 uint8_t *regData,
								 uint8_t numOfBytes,
								 uint32_t i2cTimeout)
{
    Board_STATUS ret = BOARD_SOK;
    I2C_Transaction transaction;

    I2C_Handle i2cHandle = (I2C_Handle)handle;

    /* Initializes the I2C transaction structure with default values */
    I2C_transactionInit(&transaction);

    transaction.slaveAddress = slaveAddr;
    transaction.writeBuf     = &regAddr;
    transaction.writeCount   = 1;
    transaction.readBuf      = regData;
    transaction.readCount    = numOfBytes;
    transaction.timeout      = i2cTimeout;

    ret = I2C_transfer(i2cHandle, &transaction);

    return ret;
}

/**
 * \brief I2C 8-bit register write function
 *
 * This function is used to write the 8-bit data on the i2c
 * device registers
 *
 * \param   handle      [IN]  Low level driver handle
 * \param   slaveAddr   [IN]  I2C slave address
 * \param   regAddr     [IN]  I2C register offset address
 * \param   regData     [IN]  I2C register data buffer
 * \param   numOfBytes  [IN]  Transmit data width
 * \param   i2cTimeout  [IN]  I2C timeout
 *
 * \return  BOARD_SOK in case of success or appropriate error code.
 */
Board_STATUS BoardDiag_pmicRegWr(void *handle,
                                 uint32_t slaveAddr,
                                 uint8_t regAddr,
                                 uint8_t *regData,
                                 uint8_t numOfBytes,
                                 uint32_t i2cTimeout)
{
    Board_STATUS ret = BOARD_SOK;
    uint8_t tx[5];
    uint8_t *txPtr = NULL;
    I2C_Transaction transaction;

    I2C_Handle i2cHandle = (I2C_Handle)handle;

    /* Initializes the I2C transaction structure with default values */
    I2C_transactionInit(&transaction);

    transaction.slaveAddress = slaveAddr;
    transaction.writeBuf     = &tx[0];
    transaction.writeCount   = 2;
    transaction.readBuf      = NULL;
    transaction.readCount    = 0;
    transaction.timeout      = i2cTimeout;

    tx[0] = regAddr;
    txPtr = &tx[1];
    /* regData to be sent */
    while(numOfBytes)
    {
        *txPtr = *regData;
        txPtr++;
        regData++;
        numOfBytes--;
    }

    ret = I2C_transfer(i2cHandle, &transaction);
    if(ret != I2C_STS_SUCCESS)
    {
        ret = -1;
        return ret;
    }

    return BOARD_SOK;
}

/**
 * \brief   Dummy PMIC Critical section Lock function
 *          This function should have OS specific locking setup and should
 *          assigned to 'pmicConfigData->pFnPmicCritSecStart'
 */
void BoardDiag_pmicCriticalSectionStartFn(void)
{

}

/**
 * \brief   Dummy PMIC Critical section Unlock function
 *          This function should have OS specific locking setup and should
 *          assigned to 'pmicConfigData->pFnPmicCritSecStop'
 */
void BoardDiag_pmicCriticalSectionStopFn(void)
{

}

/**
 * \brief   PMIC I2C read function.
 *
 * \param   pmicCorehandle [IN]     PMIC Interface Handle
 * \param   instType       [IN]     PMIC I2C instance type
 * \param   regAddr        [OUT]    Register address to read from
 * \param   pBuf           [IN]     Buffer to store data
 * \param   bufLen         [IN]     Length of data to read
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code
 *          For valid values see \ref Pmic_ErrorCodes
 */
int32_t BoardDiag_pmicRegRead(Pmic_CoreHandle_t *pmicCorehandle,
							  uint8_t instType,
                              uint16_t regAddr,
                              uint8_t *pBuf,
                              uint8_t bufLen)
{
    int8_t ret = 0U;
	uint32_t slaveAddr = 0;

    if((PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode) ||
       (PMIC_INTF_DUAL_I2C   == pmicCorehandle->commMode))
    {
        /* Main I2c BUS */
        if(PMIC_MAIN_INST == instType)
        {
            slaveAddr = (uint32_t)pmicCorehandle->slaveAddr;
            ret = BoardDiag_pmicRegRd(pmicCorehandle->pCommHandle,
									  slaveAddr,
									  (uint8_t)regAddr,
									  pBuf,
									  1,
									  BOARD_I2C_TRANSACTION_TIMEOUT);
            if(ret != I2C_STS_SUCCESS)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }
        }

        /* For WDOG QA I2C BUS */
        if(PMIC_QA_INST == instType)
        {
            slaveAddr = (uint32_t)pmicCorehandle->qaSlaveAddr;
            if(PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode)
            {
                ret = BoardDiag_pmicRegRd(pmicCorehandle->pCommHandle,
										  slaveAddr,
										  (uint8_t)regAddr,
										  pBuf,
										  1,
										  BOARD_I2C_TRANSACTION_TIMEOUT);
            }
            if(PMIC_INTF_DUAL_I2C == pmicCorehandle->commMode)
            {
                ret = BoardDiag_pmicRegRd(pmicCorehandle->pQACommHandle,
										  slaveAddr,
										  (uint8_t)regAddr,
										  pBuf,
										  1,
										  BOARD_I2C_TRANSACTION_TIMEOUT);
            }
            if(ret != I2C_STS_SUCCESS)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }
        }
    }

    return PMIC_ST_SUCCESS;
}

/**
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
int32_t BoardDiag_pmicRegWrite(Pmic_CoreHandle_t *pmicCorehandle,
                               uint8_t instType,
                               uint16_t regAddr,
                               uint8_t *pBuf,
                               uint8_t bufLen)
{
    int8_t  ret = 0U;
	uint32_t slaveAddr = 0;

    if((PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode) ||
       (PMIC_INTF_DUAL_I2C   == pmicCorehandle->commMode))
    {
        if(PMIC_MAIN_INST == instType)
        {
            slaveAddr = (uint32_t)pmicCorehandle->slaveAddr;
        }

        if(PMIC_QA_INST == instType)
        {
            slaveAddr = (uint32_t)pmicCorehandle->qaSlaveAddr;
        }

        /* Main I2c BUS */
        if(PMIC_MAIN_INST == instType)
        {
            ret = BoardDiag_pmicRegWr(pmicCorehandle->pCommHandle,
									  slaveAddr,
									  (uint8_t)regAddr,
									  pBuf,
									  1U,
									  BOARD_I2C_TRANSACTION_TIMEOUT);
            if(ret != I2C_STS_SUCCESS)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }
        }

        /* For WDOG QA I2C BUS */
        if(PMIC_QA_INST == instType)
        {
            if(PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode)
            {
                ret = BoardDiag_pmicRegWr(pmicCorehandle->pCommHandle,
										  slaveAddr,
										  (uint8_t)regAddr,
										  pBuf,
										  1U,
										  BOARD_I2C_TRANSACTION_TIMEOUT);
            }

            if(PMIC_INTF_DUAL_I2C == pmicCorehandle->commMode)
            {
                ret = BoardDiag_pmicRegWr(pmicCorehandle->pQACommHandle,
										  slaveAddr,
										  (uint8_t)regAddr,
										  pBuf,
										  1U,
										  BOARD_I2C_TRANSACTION_TIMEOUT);
            }

            if(ret != I2C_STS_SUCCESS)
            {
                return PMIC_ST_ERR_I2C_COMM_FAIL;
            }
        }
    }

    return PMIC_ST_SUCCESS;
}

/**
 * \brief    This API Set Config for TI HW I2C instances
 *
 * \param    instance [IN] I2C instance number
 * \param    baseAddr [IN] Register base address of the I2C instance
 */
static void BoardDiag_pmicSetConfigI2C(uint8_t instance, uint32_t baseAddr)
{
    I2C_HwAttrs i2cCfg;

    I2C_socGetInitCfg(instance, &i2cCfg);
    i2cCfg.baseAddr   = baseAddr;
    i2cCfg.enableIntr = 0U;
    I2C_socSetInitCfg(instance, &i2cCfg);
}

/**
 * \brief   Function to setup the I2C interface for PMIC
 *
 * \param   pPmicCoreHandle  [IN]     PMIC Interface Handle.
 * \param   instType         [IN]     Instance Type
 *
 * \retval  0 in case of success, -1 in case of failure
 */
static int32_t BoardDiag_pmicI2cInit(Pmic_CoreCfg_t *pPmicConfigData,
									 uint8_t instType)
{
    I2C_Params i2cParams;
    I2C_Handle i2cHandle = NULL;
    uint8_t i2c_instance = 0U;

    /* Main I2c BUS */
    if(instType == PMIC_MAIN_INST)
    {
        i2c_instance = 0U;
        BoardDiag_pmicSetConfigI2C(i2c_instance, CSL_WKUP_I2C0_CFG_BASE);
    }
    /* For WDG QA I2C BUS */
    else if(instType == PMIC_QA_INST)
    {
        i2c_instance = 1U;
        BoardDiag_pmicSetConfigI2C(i2c_instance, CSL_MCU_I2C0_CFG_BASE);
    }

    /* Initialize i2c core instances */
    I2C_init();
    /* Configured i2cParams.bitRate with standard I2C_100kHz */
    I2C_Params_init(&i2cParams);

    i2cHandle = I2C_open(i2c_instance, &i2cParams);
    if(NULL == i2cHandle)
    {
        UART_printf("I2C_open is failed!!!\n");
        return -1;
    }

    /* Main I2c BUS */
    if(instType == PMIC_MAIN_INST)
    {
        pPmicConfigData->pCommHandle = i2cHandle;
    }
    /* For WDOG QA I2C BUS */
    else if(PMIC_QA_INST == instType)
    {
        pPmicConfigData->pQACommHandle = i2cHandle;
    }

    return 0;
}

/**
 * \brief   Function to release the I2C lld interface for PMIC
 *
 * \param   pCommHandle    [OUT]     PMIC Interface Handle.
 *
 * \retval  0 in case of success, appropriate erro code in case of failure
 */
static int32_t BoardDiag_pmicI2cDeInit(void **pCommHandle)
{
    if(NULL == (I2C_Handle)*pCommHandle)
    {
        return -1;
    }

    I2C_close((I2C_Handle)*pCommHandle);

    *pCommHandle = NULL;

    return 0;
}

/**
 * \brief   Initialize PMIC Instance and corresponding Interface.
 *
 * \param   pmicCoreHandle    [OUT]     PMIC Core Handle.
 * \param   pmicConfigData    [IN]      PMIC Config Data.
 *
 * \retval  0 in case of success, -1 in case of failure
 */
int32_t BoardDiag_pmicAppInit(Pmic_CoreHandle_t **pmicCoreHandle,
							  Pmic_CoreCfg_t *pmicConfigData)
{
    int32_t pmicStatus = 0;
    Pmic_CoreHandle_t *pmicHandle = NULL;

    if(pmicCoreHandle == NULL)
    {
        UART_printf("Invalid PMIC core Handle Reference\n");
        return -1;
    }

    if(pmicConfigData == NULL)
    {
        UART_printf("Invalid PMIC config Data - NULL \n");
        return -1;
    }

    /* Allocate memory for PMIC core Handle */
    pmicHandle = malloc(sizeof(Pmic_CoreHandle_t));
    if(pmicHandle == NULL)
    {
        UART_printf("Failed to allocate memory to pmicHandle\n");
        return -1;
    }
    memset(pmicHandle, 0, sizeof(Pmic_CoreHandle_t));

	/* Get PMIC valid Main I2C Instance */
	pmicStatus = BoardDiag_pmicI2cInit(pmicConfigData, PMIC_MAIN_INST);
	if(pmicStatus == 0)
	{
		pmicConfigData->validParams |= PMIC_CFG_COMM_HANDLE_VALID_SHIFT;
		/* Update instance type to pmicConfigData */
		pmicConfigData->instType = PMIC_MAIN_INST;
		/* Get PMIC core Handle for Main Instance */
		pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
	}

    /* For DUAL I2C Instance */
    if(PMIC_INTF_DUAL_I2C == pmicConfigData->commMode)
    {
        if(PMIC_ST_SUCCESS == pmicStatus)
        {
            /* Get PMIC valid QA I2C Instance */
            pmicStatus = BoardDiag_pmicI2cInit(pmicConfigData, PMIC_QA_INST);
        }

        if(PMIC_ST_SUCCESS == pmicStatus)
        {
            pmicConfigData->validParams |= PMIC_CFG_QACOMM_HANDLE_VALID_SHIFT;
            /* Update instance type to pmicConfigData */
            pmicConfigData->instType = PMIC_QA_INST;
            /* Get PMIC core Handle for QA Instances */
            pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
        }

    }

    if(PMIC_ST_SUCCESS != pmicStatus)
    {
		/*PMIC is not fully initialised*/
    }

    *pmicCoreHandle = pmicHandle;

    return pmicStatus;
}

/**
 * \brief   Function to release the Pmic lld interface.
 *
 * \param   pmicCoreHandle    [IN]     PMIC Core Handle.
 */
void BoardDiag_pmicAppDeInit(Pmic_CoreHandle_t *pmicCoreHandle)
{
    if(PMIC_INTF_SINGLE_I2C == pmicCoreHandle->commMode)
    {
        BoardDiag_pmicI2cDeInit(&pmicCoreHandle->pCommHandle);
    }
    else if(PMIC_INTF_DUAL_I2C == pmicCoreHandle->commMode)
    {
        BoardDiag_pmicI2cDeInit(&pmicCoreHandle->pCommHandle);
        BoardDiag_pmicI2cDeInit(&pmicCoreHandle->pQACommHandle);
    }

    Pmic_deinit(pmicCoreHandle);
}

/**
 * \brief   PMIC init configuration function.
 *
 * \param   pmicCorehandle		[OUT]	PMIC Interface Handle
 * \param   pmicDeviceType		[IN]	Pmic device type
 * \param   pmicIntfI2cType		[IN]	Communication Mode
 * \param   pmicSlaveAddress	[IN]	Pmic slave address
 * \param   pmicQaSlaveAddress	[IN]	Pmic QA slave address
 *
 * \retval  PMIC_ST_SUCCESS in case of success or appropriate error code
 *          For valid values see \ref Pmic_ErrorCodes
 */
void BoardDiag_pmicInit(Pmic_CoreHandle_t **pmicHandle,
						uint8_t pmicDeviceType,
						uint8_t pmicIntfI2cType,
						uint8_t pmicSlaveAddress,
						uint8_t pmicQaSlaveAddress)
{
    Pmic_CoreCfg_t pmicConfigData = {0U};

    /* Fill parameters to pmicConfigData */
    pmicConfigData.pmicDeviceType     =  pmicDeviceType;
    pmicConfigData.validParams        |= PMIC_CFG_DEVICE_TYPE_VALID_SHIFT;

    pmicConfigData.commMode           = pmicIntfI2cType;
    pmicConfigData.validParams        |= PMIC_CFG_COMM_MODE_VALID_SHIFT;

    pmicConfigData.slaveAddr          = pmicSlaveAddress;
    pmicConfigData.validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigData.qaSlaveAddr        = pmicQaSlaveAddress;
    pmicConfigData.validParams        |= PMIC_CFG_QASLAVEADDR_VALID_SHIFT;

    pmicConfigData.pFnPmicCommIoRead   = BoardDiag_pmicRegRead;
    pmicConfigData.validParams         |= PMIC_CFG_COMM_IO_RD_VALID_SHIFT;

    pmicConfigData.pFnPmicCommIoWrite  = BoardDiag_pmicRegWrite;
    pmicConfigData.validParams         |= PMIC_CFG_COMM_IO_WR_VALID_SHIFT;

    pmicConfigData.pFnPmicCritSecStart = BoardDiag_pmicCriticalSectionStartFn;
    pmicConfigData.validParams         |= PMIC_CFG_CRITSEC_START_VALID_SHIFT;

    pmicConfigData.pFnPmicCritSecStop  = BoardDiag_pmicCriticalSectionStopFn;
    pmicConfigData.validParams         |= PMIC_CFG_CRITSEC_STOP_VALID_SHIFT;

    BoardDiag_pmicAppInit(pmicHandle, &pmicConfigData);

}

/**
 * \brief   Get the pmic information based on boardname.
 *
 * \param   boardName    [IN]     SOM board name.
 *
 * \retval  BoardDiag_PmicData_t having corresponding pmic information in case of
 *         success or NULL otherwise.
 */
BoardDiag_PmicData_t* BoardDiag_getPmicData(char *pBoardName)
{
    BoardDiag_PmicData_t *pPmicData = NULL;

    if ((strncmp("J721EX-PM2-SOM", pBoardName, BOARD_BOARD_NAME_LEN) == 0U) ||
		(strncmp("J7200X-PM1-SOM", pBoardName, BOARD_BOARD_NAME_LEN) == 0U))
    {
        pPmicData     = &tps65941_pmicA;
        gDualPmicData = &tps65941_pmicB;
        gNumPmic = 2;
    }
    else if (strncmp("J7200X-PM2-SOM", pBoardName, BOARD_BOARD_NAME_LEN) == 0U)
    {
        pPmicData     = &tps65941_pmicA;
        gDualPmicData = &lp8764x_pmicB;
        gNumPmic = 2;
    }
    else
    {
        /* If the board is not one of these, then the PmicData returns NULL. */
    }

    return pPmicData;
}

/**
 * \brief	function to run pmic diagnostic.
 */
int BoardDiag_pmicTest()
{
    int32_t stat = BOARD_SOK;
    uint16_t voltage, val;
    BoardDiag_PmicData_t *pPmicData;
    Board_IDInfo_v2 info = {0};

	Pmic_CoreHandle_t *pmicHandle = NULL;

	Pmic_PowerResourceCfg_t pPowerCfg  =
    {
        PMIC_CFG_REGULATOR_VMON_VOLTAGE_SET_VALID_SHIFT,
    };

    Board_setI2cInitConfig(&boardI2cInitCfg);
    stat = Board_getIDInfo_v2(&info, BOARD_I2C_EEPROM_ADDR);
    if(stat == BOARD_SOK)
    {
		UART_printf("\nBoard Name: ");
		UART_dataWrite((char *)&info.boardInfo.boardName, BOARD_BOARD_NAME_LEN);
		
        pPmicData = BoardDiag_getPmicData(info.boardInfo.boardName);

		while(gNumPmic)
        {
			UART_printf("\n\n");
			BoardDiag_pmicInit(&pmicHandle,
						       pPmicData->deviceType,
							   pPmicData->intfI2cType,
							   pPmicData->slaveAddress,
							   pPmicData->qaSlaveAddress);

            val = pPmicData->voltVal;
            UART_printf("Testing PMIC module... \n");

            Pmic_powerGetPwrResourceCfg(pmicHandle,
									    pPmicData->pwrRsrc,
										&pPowerCfg);
			voltage = pPowerCfg.voltage_mV;
            UART_printf("Initial PMIC voltage = %d mV\n", voltage);

            UART_printf("Setting PMIC voltage to %d mV\n", val);
			pPowerCfg.voltage_mV = val;
			Pmic_powerSetPwrResourceCfg(pmicHandle,
			                            pPmicData->pwrRsrc,
										pPowerCfg);
            UART_printf("done!\n");

			Pmic_powerGetPwrResourceCfg(pmicHandle,
			                            pPmicData->pwrRsrc,
										&pPowerCfg);
            UART_printf("PMIC voltage after = %d mV\n", pPowerCfg.voltage_mV);

            UART_printf("Setting PMIC voltage to original value\n");
			pPowerCfg.voltage_mV = voltage;
			Pmic_powerSetPwrResourceCfg(pmicHandle,
			                              pPmicData->pwrRsrc,
										  pPowerCfg);
            Pmic_powerGetPwrResourceCfg(pmicHandle,
			                            pPmicData->pwrRsrc,
										&pPowerCfg);
			UART_printf("Final voltage value = %d mv\n", pPowerCfg.voltage_mV);
			BoardDiag_pmicAppDeInit(pmicHandle);
            gNumPmic--;

            if(gNumPmic)
            {
                pPmicData = gDualPmicData;
            }
        }
		UART_printf("Test PASSED!\n");
    }
	else
	{
		UART_printf("SOM not programmed\n");
		UART_printf("Test FAILED!\n");
	}
    return 0;
}

/**
 * \brief	main.c
 */
int main(void)
{
    Board_initCfg boardCfg;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_PINMUX_CONFIG | 
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;
#endif

    Board_init(boardCfg);

	UART_printf("\n*********************************************\n"); 
	UART_printf  ("*                PMIC Test                  *\n");
	UART_printf  ("*********************************************\n");
    
    return BoardDiag_pmicTest();
}
