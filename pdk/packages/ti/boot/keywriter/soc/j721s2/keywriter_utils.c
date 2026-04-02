/**
 *  \file   keywriter_utils.c
 *
 *  \brief  This file contains functions specific to j721s2 keywriter support
 *
 */

/*
 * Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/
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

#include "keywriter_utils.h"
#include "board_utils.h"

static void keywr_leo_pmicb_set_params(Pmic_CoreCfg_t *pmicConfigData)
{
    /* Fill parameters to pmicConfigData */
    pmicConfigData->pmicDeviceType      = PMIC_DEV_LEO_TPS6594X;
    pmicConfigData->validParams        |= PMIC_CFG_DEVICE_TYPE_VALID_SHIFT;

    pmicConfigData->commMode            = PMIC_INTF_SINGLE_I2C;
    pmicConfigData->validParams        |= PMIC_CFG_COMM_MODE_VALID_SHIFT;

    pmicConfigData->slaveAddr           = J721E_LEO_PMICB_SLAVE_ADDR;
    pmicConfigData->validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigData->qaSlaveAddr         = J721E_LEO_PMICB_WDG_SLAVE_ADDR;
    pmicConfigData->validParams        |= PMIC_CFG_QASLAVEADDR_VALID_SHIFT;

    pmicConfigData->pFnPmicCommIoRead   = test_pmic_regRead;
    pmicConfigData->validParams        |= PMIC_CFG_COMM_IO_RD_VALID_SHIFT;

    pmicConfigData->pFnPmicCommIoWrite  = test_pmic_regWrite;
    pmicConfigData->validParams        |= PMIC_CFG_COMM_IO_WR_VALID_SHIFT;

    pmicConfigData->pFnPmicCritSecStart = test_pmic_criticalSectionStartFn;
    pmicConfigData->validParams        |= PMIC_CFG_CRITSEC_START_VALID_SHIFT;

    pmicConfigData->pFnPmicCritSecStop  = test_pmic_criticalSectionStopFn;
    pmicConfigData->validParams        |= PMIC_CFG_CRITSEC_STOP_VALID_SHIFT;
}

/* Enable VPP for J721S2 EVM or a custom board */
void OTP_VppEn_EVM(void)
{
    uint16_t pmic_device_info;
    int32_t status, pmicStatus;
    Pmic_CoreHandle_t *pPmicCoreHandle  = NULL;
    Pmic_CoreCfg_t pmicb_cfg = {0U};
    Pmic_GpioCfg_t gpioCfg = {0U};

    UART_printf("OTP_VppEn_EVM\n");

    pmic_device_info = J721E_LEO_PMICB_DEVICE;
    (void)pmic_device_info;
    keywr_leo_pmicb_set_params(&pmicb_cfg);

    status = test_pmic_appInit(&pPmicCoreHandle, &pmicb_cfg);

    if (PMIC_ST_SUCCESS == status)
    {
        gpioCfg.validParams = 0x1U << PMIC_GPIO_CFG_DIR_VALID;
        pmicStatus = Pmic_gpioGetConfiguration(pPmicCoreHandle, 9, &gpioCfg);

        if (PMIC_ST_SUCCESS != pmicStatus)
        {
            UART_printf("Pmic_gpioGetConfiguration failed\n");
        }
        else
        {
            UART_printf("Pmic_gpioGetConfiguration ret: %d\n", gpioCfg.pinDir);
        }

        gpioCfg.pinDir = PMIC_GPIO_OUTPUT;
        pmicStatus = Pmic_gpioSetConfiguration(pPmicCoreHandle, 9, gpioCfg);

        if (PMIC_ST_SUCCESS != pmicStatus)
        {
            UART_printf("Pmic_gpioSetConfiguration ret: %d\n", pmicStatus);
        }

        gpioCfg.pinDir = 0xFF;
        pmicStatus = Pmic_gpioGetConfiguration(pPmicCoreHandle, 9, &gpioCfg);

        if (PMIC_ST_SUCCESS != pmicStatus)
        {
            UART_printf("Pmic_gpioGetConfiguration failed\n");
        }
        else
        {
            UART_printf("Pmic_gpioGetConfiguration ret: %d\n", gpioCfg.pinDir);
        }

        /* Here is where we need to set GPIO9 of LEO PMICB */
        pmicStatus = Pmic_gpioSetValue(pPmicCoreHandle, 9, PMIC_GPIO_HIGH);

        if (PMIC_ST_SUCCESS != pmicStatus)
        {
            UART_printf("Pmic_gpioSetValue ret: %d\n", pmicStatus);
        }
        else
        {
            UART_printf("Pmic_gpioSetValue ret: %d Works!!!\n", pmicStatus);
        }
    }

    if ((NULL != pPmicCoreHandle) && (PMIC_ST_SUCCESS == status))
    {
        test_pmic_appDeInit(pPmicCoreHandle);
    }
}

/* Enable VPP for AM68 SK board */
void OTP_VppEn_SK(void)
{
    uint32_t regVal;

    UART_printf("OTP_VppEn_SK\n");

	/* Set the GPIO direction to output */
    regVal = (HW_RD_REG32(0x42110010)) & (~(0x1 << KEYWRITER_SK_VPP_CTRL_GPIO));
    HW_WR_REG32(0x42110010, regVal);

	/* Set the GPIO value to high */
    regVal = (HW_RD_REG32(0x42110014)) | (0x1 << KEYWRITER_SK_VPP_CTRL_GPIO);
	HW_WR_REG32(0x42110014, regVal);

    UART_printf("OTP Vpp is Enabled!\n");
}

/* Function to enable VPP voltage.
 * GPIO based VPP control is required for AM68 SK.
 * PMIC based VPP control is required for J721S2 EVM.
 * One of these VPP configs can be used as per the custom board design.
 */
void OTP_VppEn(void)
{
    Board_I2cInitCfg_t i2cCfg;
    Board_IDInfo_v2    info;
    Board_STATUS       status;
    bool               skBoardDet = BFALSE;

    i2cCfg.i2cInst    = BOARD_I2C_EEPROM_INSTANCE;
    i2cCfg.socDomain  = BOARD_SOC_DOMAIN_WKUP;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    /* Check if the board is SK */
    status = Board_getIDInfo_v2(&info, KEYWRITER_SK_EEPROM_SLAVE_ADDR);
    if(BOARD_SOK == status)
    {
        if(!(strncmp(info.boardInfo.boardName,
                     "AM68-SK-SOM",
                     BOARD_BOARD_NAME_LEN)))
        {
            UART_printf("AM68 SK Detected!!\n");
            skBoardDet = BTRUE;
        }
    }

    if(BTRUE == skBoardDet)
    {
        /* Enable VPP for AM68 SK board */
        OTP_VppEn_SK();
    }
    else
    {
        /* Enable VPP for J721S2 EVM or a Custom board */
        OTP_VppEn_EVM();
    }
}
