/**
 *  \file   keywriter_utils.c
 *
 *  \brief  This file contains functions specific to J7200 keywriter support
 *
 */

/*
 * Copyright (C) 2021-2022 Texas Instruments Incorporated - http://www.ti.com/
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

#ifdef CONFIG_j7200_BRING_UP_EVM
static void keywr_leo_pmicb_set_params(Pmic_CoreCfg_t *pmicConfigData)
{
    /* Fill parameters to pmicConfigData */
    pmicConfigData->pmicDeviceType      = PMIC_DEV_LEO_TPS6594X;
    pmicConfigData->validParams        |= PMIC_CFG_DEVICE_TYPE_VALID_SHIFT;

    pmicConfigData->commMode            = PMIC_INTF_SINGLE_I2C;
    pmicConfigData->validParams        |= PMIC_CFG_COMM_MODE_VALID_SHIFT;

    pmicConfigData->slaveAddr           = J7VCL_WAKEUP_SOM_LEO_PMICB_SLAVE_ADDR;
    pmicConfigData->validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigData->qaSlaveAddr         = J7VCL_WAKEUP_SOM_LEO_PMICB_SLAVE_ADDR;
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

#else
static void keywr_hera_pmicb_set_params(Pmic_CoreCfg_t *pmicConfigData)
{
    /* Fill parameters to pmicConfigData */
    pmicConfigData->pmicDeviceType      = PMIC_DEV_HERA_LP8764X;
    pmicConfigData->validParams        |= PMIC_CFG_DEVICE_TYPE_VALID_SHIFT;

    pmicConfigData->commMode            = PMIC_INTF_SINGLE_I2C;
    pmicConfigData->validParams        |= PMIC_CFG_COMM_MODE_VALID_SHIFT;

    pmicConfigData->slaveAddr           = J7VCL_HERA_PMIC_SLAVE_ADDR;
    pmicConfigData->validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigData->qaSlaveAddr         = J7VCL_HERA_PMIC_SLAVE_ADDR;
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

#endif

void OTP_VppEn(void)
{
    uint16_t pmic_device_info;
    int32_t status, pmicStatus;
    Pmic_CoreHandle_t *pPmicCoreHandle = NULL;
    Pmic_CoreCfg_t pmicb_cfg = {0U};

    UART_printf("OTP_VppEn\n");

#ifndef CONFIG_j7200_BRING_UP_EVM
    pmic_device_info = J7VCL_HERA_PMICB_DEVICE;
    (void)pmic_device_info;
    keywr_hera_pmicb_set_params(&pmicb_cfg);
#else
    pmic_device_info = J721E_LEO_PMICB_DEVICE;
    (void)pmic_device_info;
    keywr_leo_pmicb_set_params(&pmicb_cfg);
#endif

    status = test_pmic_appInit(&pPmicCoreHandle, &pmicb_cfg);

    if (PMIC_ST_SUCCESS == status)
    {
#ifdef CONFIG_j7200_BRING_UP_EVM
        /* Here is where we need to set GPIO9 of LEO PMICB */
        pmicStatus = Pmic_gpioSetValue(pPmicCoreHandle, 9, PMIC_GPIO_HIGH);
#else
        /* Here is where we need to set GPIO5 of HERA PMIC */
        pmicStatus = Pmic_gpioSetDir(pPmicCoreHandle, 5, PMIC_GPIO_OUTPUT);
        pmicStatus = Pmic_gpioSetValue(pPmicCoreHandle, 5, PMIC_GPIO_HIGH);
#endif

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
