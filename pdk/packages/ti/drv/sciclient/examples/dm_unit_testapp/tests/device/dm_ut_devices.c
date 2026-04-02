/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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
 */

/**
 *  \file   dm_ut_devices.c
 *
 *  \brief  Implementation of devices test functions for DM Unit Test.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/board/board.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/csl_types.h>
#include <ti/drv/sciclient/sciclient.h>
#include <dm_ut_devices.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* DM_APP_LOOP_COUNT refers to Number of times to loop the test */
#define DM_APP_LOOP_COUNT          ((uint32_t) 2U)

/**
 * \anchor DMApp_ResetVal
 * \name DM app reset values
 * @{
 */
#define DM_APP_LOCAL_RESET_VALUE   ((uint32_t) 1U)
#define DM_APP_MODULE_RESET_VALUE  ((uint32_t) 2U)
#define DM_APP_NORESET_VALUE       ((uint32_t) 0U)
/* @} */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern DMApp_DependentDevInfo gDMAppDevicesWithDependency[];
extern const uint32_t gDMAppMainAlwaysOnDevices[];
extern const uint32_t gDMAppMcuAlwaysOnDevices[];
extern const uint32_t gDMAppDevicesWithoutDependency[];
extern uint32_t gDMAppDevicesWithDependencySize;
extern uint32_t gDMAppMainAlwaysOnDevicesSize;
extern uint32_t gDMAppMcuAlwaysOnDevicesSize;
extern uint32_t gDMAppDevicesWithoutDependencySize;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Function Declarations                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Internal Function Declarations                       */
/* ========================================================================== */

static int32_t DMApp_devicesWithDependencyTest();
static int32_t DMApp_devicesWithoutDependencyTest();
static int32_t DMApp_alwaysonDevicesTest(const uint32_t *device_array,uint32_t DMApp_AlwaysOnDeviceArraySize);

/* To initialize the gDMAppDevicesWithDependency sizes with the number of dependent devices */
static void DMApp_initializeDeviceSizes();

/* To test the reset of all the devices in the DM */
static int32_t DMApp_devicesResetTest(uint32_t device_id);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Tests all the devices in respective SOC
 * by resetting them and powering them off and on
 */
int32_t DMApp_deviceTest()
{
    int32_t checkstatus = CSL_PASS;
    int32_t error = CSL_PASS;

    /* To test all the devices in the gDMAppDevicesWithoutDependency list */
    checkstatus = DMApp_devicesWithoutDependencyTest();

    if(CSL_PASS != checkstatus)
    {
        error = CSL_EFAIL;
    }

    /* To test all the devices in the gDMAppMainAlwaysOnDevices list */
    checkstatus = DMApp_alwaysonDevicesTest(gDMAppMainAlwaysOnDevices,gDMAppMainAlwaysOnDevicesSize);

    if(CSL_PASS != checkstatus)
    {
        error = CSL_EFAIL;
    }

    /* To test all the devices in the gDMAppMcuAlwaysOnDevices list */
    checkstatus = DMApp_alwaysonDevicesTest(gDMAppMcuAlwaysOnDevices,gDMAppMcuAlwaysOnDevicesSize);

    if(CSL_PASS != checkstatus)
    {
        error = CSL_EFAIL;
    }

    /* To test all the devices in the gDMAppDevicesWithDependency list */
    checkstatus = DMApp_devicesWithDependencyTest();

    if(CSL_PASS != checkstatus)
    {
        error = CSL_EFAIL;
    }

    return error;
}

/* ========================================================================== */
/*                       Internal Function Definitions                        */
/* ========================================================================== */

static int32_t DMApp_devicesWithoutDependencyTest()
{
    int32_t checkstatus = CSL_PASS;
    uint32_t i = 0;

    for(i = 0; i < gDMAppDevicesWithoutDependencySize; i++)
    {
        uint32_t device_id = gDMAppDevicesWithoutDependency[i];
        int32_t status = CSL_PASS;
        uint32_t moduleState;
        uint32_t resetState;
        uint32_t contextLossState;
        uint32_t j = 0;

        for(j = 0; j < DM_APP_LOOP_COUNT; j++)
        {
            if(CSL_PASS == status)
            {
                status = Sciclient_pmSetModuleState(device_id, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                        TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);

                if(CSL_PASS == status)
                {
                    status = Sciclient_pmGetModuleState(device_id, &moduleState, &resetState,
                                                            &contextLossState, SCICLIENT_SERVICE_WAIT_FOREVER);

                    if(CSL_PASS != status)
                    {
                        UART_printf("Sciclient_pmGetModuleState ON error of %d\n", device_id);
                    }
                    else
                    {
                        if(moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_ON)
                        {
                            UART_printf("ModuleState ON error of %d\n", device_id);
                            status = CSL_EFAIL;
                        }
                    }
                }
                else
                {
                    UART_printf("Sciclient_pmSetModuleState ON error of %d\n", device_id);
                }
            }

            if(CSL_PASS == status)
            {
                status = Sciclient_pmSetModuleState(device_id, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                        TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);

                if(CSL_PASS == status)
                {
                    status = Sciclient_pmGetModuleState(device_id, &moduleState, &resetState,
                                                            &contextLossState, SCICLIENT_SERVICE_WAIT_FOREVER);

                    if(CSL_PASS != status)
                    {
                        UART_printf("Sciclient_pmGetModuleState OFF error of %d\n", device_id);
                    }
                    else
                    {
                        if(moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF)
                        {
                            UART_printf("ModuleState OFF error of %d\n", device_id);
                            status = CSL_EFAIL;
                        }
                    }
                }
                else
                {
                    UART_printf("Sciclient_pmSetModuleState OFF error of %d\n", device_id);
                }
            }
        }

        if(CSL_PASS != status)
        {
            checkstatus = CSL_EFAIL;
        }

        status = DMApp_devicesResetTest(device_id);

        if(CSL_PASS != status)
        {
            checkstatus = CSL_EFAIL;
        }
    }

    return checkstatus;
}

static int32_t DMApp_alwaysonDevicesTest(const uint32_t *device_array,uint32_t DMApp_AlwaysOnDeviceArraySize)
{
    int32_t checkstatus = CSL_PASS;
    uint32_t i = 0;

    for(i = 0; i < DMApp_AlwaysOnDeviceArraySize; i++)
    {
        uint32_t device_id = device_array[i];
        int32_t status = CSL_PASS;
        uint32_t moduleState;
        uint32_t resetState;
        uint32_t contextLossState;
        status = Sciclient_pmSetModuleState(device_id, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);

        if(CSL_PASS == status)
        {
            status = Sciclient_pmGetModuleState(device_id, &moduleState, &resetState,
                                                        &contextLossState, SCICLIENT_SERVICE_WAIT_FOREVER);

            if(CSL_PASS != status)
            {
                UART_printf("Sciclient_pmGetModuleState OFF error of %d\n", device_id);
            }
            else
            {
                if(TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF == moduleState)
                {
                    UART_printf("ERROR: Always ON device return status as power off %d\n", device_id);
                    status = CSL_EFAIL;
                }
            }
        }
        else
        {
            UART_printf("Sciclient_pmSetModuleState OFF error of %d\n", device_id);
        }

        if(CSL_PASS != status)
        {
            checkstatus = CSL_EFAIL;
        }

        status = DMApp_devicesResetTest(device_id);

        if(CSL_PASS != status)
        {
            checkstatus = CSL_EFAIL;
        }
    }

    return checkstatus;
}

static int32_t DMApp_devicesWithDependencyTest()
{
    DMApp_initializeDeviceSizes();
    int32_t checkstatus = CSL_PASS;

    /* To store errors in buffer when mcu UART is powered off */
    uint32_t mcu_buffer[gDMAppDevicesWithDependency[(gDMAppDevicesWithDependencySize)-1].size];
    uint32_t i = 0;

    for(i = 0; i < gDMAppDevicesWithDependency[(gDMAppDevicesWithDependencySize)-1].size; i++)
    {
        mcu_buffer[i] = 0;
    }

    for(i = 0; i < gDMAppDevicesWithDependencySize;i++)
    {
        int32_t status1 = CSL_PASS;
        int32_t j = gDMAppDevicesWithDependency[i].size-1;

        for(j = gDMAppDevicesWithDependency[i].size-1; j >= 0; j--)
        {
            int32_t status = CSL_PASS;
            uint32_t device_id = gDMAppDevicesWithDependency[i].devices[j];
            status = Sciclient_pmSetModuleState(device_id, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                    TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);

            if(CSL_PASS != status)
            {
                UART_printf("Sciclient_pmSetModuleState ON error of %d\n", device_id);
                checkstatus = CSL_EFAIL;
                status1 = CSL_EFAIL;
            }
        }

        if(CSL_PASS == status1)
        {
            int32_t j = gDMAppDevicesWithDependency[i].size-1;

            for(j = gDMAppDevicesWithDependency[i].size-1; j >= 0; j--)
            {
                uint32_t device_id = gDMAppDevicesWithDependency[i].devices[j];
                int32_t status = CSL_PASS;
                status = Sciclient_pmSetModuleState(device_id, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                        TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);

                if(CSL_PASS != status)
                {
                    if(gDMAppDevicesWithDependencySize-1 == i)
                    {
                        mcu_buffer[j] = 1;
                    }
                    else
                    {
                        UART_printf("Sciclient_pmSetModuleState OFF error of %d\n", device_id);
                    }

                    checkstatus = CSL_EFAIL;
                    status1 = CSL_EFAIL;
                }
            }
        }

        if(CSL_PASS == status1)
        {
            int32_t j = gDMAppDevicesWithDependency[i].size-1;

            for(j = gDMAppDevicesWithDependency[i].size-1; j >= 0; j--)
            {
                uint32_t device_id = gDMAppDevicesWithDependency[i].devices[j];
                int32_t status = CSL_PASS;
                uint32_t moduleState;
                uint32_t resetState;
                uint32_t contextLossState;
                status = Sciclient_pmGetModuleState(device_id, &moduleState, &resetState,
                                                        &contextLossState, SCICLIENT_SERVICE_WAIT_FOREVER);

                if(CSL_PASS != status)
                {
                    if(gDMAppDevicesWithDependencySize-1 == i)
                    {
                        if(0 == mcu_buffer[j])
                        {
                            mcu_buffer[j] = 2;
                        }
                    }
                    else
                    {
                        UART_printf("Sciclient_pmGetModuleState OFF error of %d\n", device_id);
                    }

                    checkstatus = CSL_EFAIL;
                    status1 = CSL_EFAIL;
                }
                else
                {
                    if(TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF != moduleState)
                    {
                        if(gDMAppDevicesWithDependencySize-1 == i)
                        {
                            if(0 == mcu_buffer[j])
                            {
                                mcu_buffer[j] = 3;
                            }
                        }
                        else
                        {
                            UART_printf("ModuleState OFF error of %d\n", device_id);
                        }
                        checkstatus = CSL_EFAIL;
                        status1 = CSL_EFAIL;
                    }
                }
            }
        }

        if(CSL_PASS == status1)
        {
            int32_t j = gDMAppDevicesWithDependency[i].size-1;

            for(j = gDMAppDevicesWithDependency[i].size-1; j >= 0; j--)
            {
                int32_t status = CSL_PASS;
                uint32_t device_id = gDMAppDevicesWithDependency[i].devices[j];
                status = Sciclient_pmSetModuleState(device_id, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                        TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);

                if(CSL_PASS != status)
                {
                    if(gDMAppDevicesWithDependencySize-1 == i)
                    {
                        if(0 == mcu_buffer[j])
                        {
                            mcu_buffer[j] = 4;
                        }
                    }
                    else
                    {
                        UART_printf("Sciclient_pmSetModuleState ON error of %d\n", device_id);
                    }
                    checkstatus = CSL_EFAIL;
                    status1 = CSL_EFAIL;
                }
            }
        }

        /* Prints the errors which are detected when mcu UART is off */
        if(i == gDMAppDevicesWithDependencySize-1)
        {
            Board_initCfg cfg = BOARD_INIT_UART_STDIO;
            Board_init(cfg);
            int32_t k = gDMAppDevicesWithDependency[(gDMAppDevicesWithDependencySize)-1].size-1;

            for(k = gDMAppDevicesWithDependency[(gDMAppDevicesWithDependencySize)-1].size-1; k >= 0; k--)
            {
                if(mcu_buffer[k] != 0)
                {
                    switch(mcu_buffer[k])
                    {
                        case 1: UART_printf("Sciclient_pmSetModuleState OFF error of %d\n", (gDMAppDevicesWithDependency[i].devices)[k]);
                        break;

                        case 2: UART_printf("Sciclient_pmGetModuleState OFF error of %d\n", (gDMAppDevicesWithDependency[i].devices)[k]);
                        break;

                        case 3: UART_printf("ModuleState OFF error of %d\n", (gDMAppDevicesWithDependency[i].devices)[k]);
                        break;

                        case 4: UART_printf("Sciclient_pmSetModuleState ON error of %d\n", (gDMAppDevicesWithDependency[i].devices)[k]);
                        break;
                    }
                }
            }
        }
    }

    for(i = 0; i < gDMAppDevicesWithDependencySize;i++)
    {
        int32_t j = gDMAppDevicesWithDependency[i].size-1;

        for(j = gDMAppDevicesWithDependency[i].size-1; j >= 0; j--)
        {
            uint32_t status = CSL_PASS;
            uint32_t device_id = gDMAppDevicesWithDependency[i].devices[j];
            status = DMApp_devicesResetTest(device_id);

            if(CSL_PASS != status)
            {
                checkstatus = CSL_EFAIL;
            }
        }
    }

    return checkstatus;
}

static int32_t DMApp_devicesResetTest(uint32_t device_id)
{
    int32_t status = CSL_PASS;
    uint32_t moduleState;
    uint32_t resetState;
    uint32_t contextLossState;
    int32_t checkstatus = CSL_PASS;
    uint32_t k = 0;
    for(k = 0; k < DM_APP_LOOP_COUNT; k++)
        {
            status = Sciclient_pmSetModuleRst(device_id,DM_APP_MODULE_RESET_VALUE,SCICLIENT_SERVICE_WAIT_FOREVER);

            if(CSL_PASS == status)
            {
                status = Sciclient_pmGetModuleState(device_id, &moduleState, &resetState,
                                                        &contextLossState, SCICLIENT_SERVICE_WAIT_FOREVER);

                if(CSL_PASS == status)
                {
                    if((DM_APP_MODULE_RESET_VALUE != resetState) && (DM_APP_NORESET_VALUE != resetState))
                    {
                        UART_printf("Modulereset error of %d\n", device_id);
                        checkstatus = CSL_EFAIL;
                    }
                }
                else
                {
                    UART_printf("Sciclient_pmGetModuleState modulereset error of %d\n", device_id);
                    checkstatus = CSL_EFAIL;
                }
            }
            else
            {
                UART_printf("Sciclient_pmSetModuleRst modulereset error of %d\n",device_id);
                checkstatus = CSL_EFAIL;
            }

            status = Sciclient_pmSetModuleRst(device_id,DM_APP_LOCAL_RESET_VALUE,SCICLIENT_SERVICE_WAIT_FOREVER);

            if(CSL_PASS == status)
            {
                status = Sciclient_pmGetModuleState(device_id, &moduleState, &resetState,
                                                        &contextLossState, SCICLIENT_SERVICE_WAIT_FOREVER);

                if(CSL_PASS == status)
                {
                    if((DM_APP_LOCAL_RESET_VALUE != resetState) && (DM_APP_NORESET_VALUE != resetState))
                    {
                        UART_printf("Localreset error of %d\n", device_id);
                        checkstatus = CSL_EFAIL;
                    }
                }
                else
                {
                    UART_printf("Sciclient_pmGetModuleState localreset error of %d\n", device_id);
                    checkstatus = CSL_EFAIL;
                }
            }
            else
            {
                UART_printf("Sciclient_pmSetModuleRst localreset error of %d\n",device_id);
                checkstatus = CSL_EFAIL;
            }
        }

    return checkstatus;
}

static void DMApp_initializeDeviceSizes()
{
    uint32_t i = 0;
	for(i = 0; i < gDMAppDevicesWithDependencySize; i++)
	{
		uint32_t count = 0;
		while((count < DM_APP_MAX_DEPENDENT_DEVICES) && (gDMAppDevicesWithDependency[i].devices[count] != 0))
		{
			count++;
		}
		gDMAppDevicesWithDependency[i].size = count;
	}
}
