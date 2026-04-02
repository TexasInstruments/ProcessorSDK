/******************************************************************************
 * Copyright (c) 2020-2021 Texas Instruments Incorporated - http://www.ti.com
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

 /** \file board_clock.c
  *
  *  \brief This file contains initialization of wakeup and main PSC
  *  configuration structures and function definitions to get the number
  *  of wakeup and main PSC config exists.
  */

#include "board_clock.h"
#include "board_utils.h"
#include <ti/drv/sciclient/sciclient.h>

extern Board_initParams_t gBoardInitParams;

uint32_t gBoardClkModuleMcuIDInitGroupl[] = {
    TISCI_DEV_MCU_TIMER0,
    TISCI_DEV_MCU_TIMER1,
    TISCI_DEV_MCU_TIMER2,
    TISCI_DEV_MCU_TIMER3,
    TISCI_DEV_MCU_UART0,
};

uint32_t gBoardClkModuleMcuIDInitGroup2[] = {
    TISCI_DEV_MCU_ESM0,
    TISCI_DEV_MCU_GPIO0,
    TISCI_DEV_MCU_I2C0,
    TISCI_DEV_MCU_I2C1,
    TISCI_DEV_MCU_MCSPI0,
    TISCI_DEV_MCU_MCSPI1,
    TISCI_DEV_MCU_UART1,
};

uint32_t gBoardClkModuleMcuIDDeinitGroupl[] = {
    TISCI_DEV_MCU_TIMER1,
    TISCI_DEV_MCU_TIMER2,
    TISCI_DEV_MCU_TIMER3,
    TISCI_DEV_MCU_UART0,
};

uint32_t gBoardClkModuleMainIDGroup1[] = {
    TISCI_DEV_DMASS0,
    TISCI_DEV_DMASS0_BCDMA_0,
    TISCI_DEV_DMASS0_CBASS_0,
    TISCI_DEV_DMASS0_INTAGGR_0,
    TISCI_DEV_DMASS0_IPCSS_0,
    TISCI_DEV_DMASS0_PKTDMA_0,
    TISCI_DEV_DMASS0_PSILCFG_0,
    TISCI_DEV_DMASS0_PSILSS_0,
    TISCI_DEV_DMASS0_RINGACC_0,
    TISCI_DEV_TIMER0,
    TISCI_DEV_MMCSD0,
    TISCI_DEV_MMCSD1,
    TISCI_DEV_FSS0,
    TISCI_DEV_FSS0_FSAS_0,
    TISCI_DEV_FSS0_OSPI_0,
    TISCI_DEV_I2C0,
    TISCI_DEV_EMIF_DATA_0_VD,
    TISCI_DEV_DDR16SS0,
    TISCI_DEV_UART0,
};

uint32_t gBoardClkModuleMainIDDeinitGroup1[] = {
    TISCI_DEV_DMASS0,
    TISCI_DEV_DMASS0_BCDMA_0,
    TISCI_DEV_DMASS0_CBASS_0,
    TISCI_DEV_DMASS0_INTAGGR_0,
    TISCI_DEV_DMASS0_IPCSS_0,
    TISCI_DEV_DMASS0_PKTDMA_0,
    TISCI_DEV_DMASS0_PSILCFG_0,
    TISCI_DEV_DMASS0_PSILSS_0,
    TISCI_DEV_DMASS0_RINGACC_0,
    TISCI_DEV_MMCSD0,
    TISCI_DEV_MMCSD1,
    TISCI_DEV_FSS0,
    TISCI_DEV_FSS0_FSAS_0,
    TISCI_DEV_FSS0_OSPI_0,
    TISCI_DEV_I2C0,
};

uint32_t gBoardClkModuleMainIDGroup2[] = {
    TISCI_DEV_ADC0,
    TISCI_DEV_CPSW0,
    TISCI_DEV_CPT2_AGGR0,
    TISCI_DEV_TIMER1,
    TISCI_DEV_TIMER2,
    TISCI_DEV_TIMER3,
    TISCI_DEV_TIMER4,
    TISCI_DEV_TIMER5,
    TISCI_DEV_TIMER6,
    TISCI_DEV_TIMER7,
    TISCI_DEV_TIMER8,
    TISCI_DEV_TIMER9,
    TISCI_DEV_TIMER10,
    TISCI_DEV_TIMER11,
    TISCI_DEV_ECAP0,
    TISCI_DEV_ECAP1,
    TISCI_DEV_ECAP2,
    TISCI_DEV_ELM0,
    TISCI_DEV_EQEP0,
    TISCI_DEV_EQEP1,
    TISCI_DEV_GTC0,
    TISCI_DEV_EQEP2,
    TISCI_DEV_ESM0,
    TISCI_DEV_FSIRX0,
    TISCI_DEV_FSIRX1,
    TISCI_DEV_FSIRX2,
    TISCI_DEV_FSIRX3,
    TISCI_DEV_FSIRX4,
    TISCI_DEV_FSIRX5,
    TISCI_DEV_FSITX0,
    TISCI_DEV_FSITX1,
    TISCI_DEV_GICSS0,
    TISCI_DEV_GPIO0,
    TISCI_DEV_GPIO1,
    TISCI_DEV_GPMC0,
    TISCI_DEV_PRU_ICSSG0,
    TISCI_DEV_PRU_ICSSG1,
    TISCI_DEV_LED0,
    TISCI_DEV_CPTS0,
    TISCI_DEV_DDPA0,
    TISCI_DEV_EPWM0,
    TISCI_DEV_EPWM1,
    TISCI_DEV_EPWM2,
    TISCI_DEV_EPWM3,
    TISCI_DEV_EPWM4,
    TISCI_DEV_EPWM5,
    TISCI_DEV_EPWM6,
    TISCI_DEV_EPWM7,
    TISCI_DEV_EPWM8,
    TISCI_DEV_VTM0,
    TISCI_DEV_MAILBOX0,
    TISCI_DEV_MCAN0,
    TISCI_DEV_MCAN1,
    TISCI_DEV_I2C1,
    TISCI_DEV_I2C2,
    TISCI_DEV_I2C3,
    TISCI_DEV_PCIE0,
    TISCI_DEV_SA2_UL0,
    TISCI_DEV_MCSPI0,
    TISCI_DEV_MCSPI1,
    TISCI_DEV_MCSPI2,
    TISCI_DEV_MCSPI3,
    TISCI_DEV_MCSPI4,
    TISCI_DEV_TIMERMGR0,
    TISCI_DEV_UART1,
    TISCI_DEV_UART2,
    TISCI_DEV_UART3,
    TISCI_DEV_UART4,
    TISCI_DEV_UART5,
    TISCI_DEV_BOARD0,
    TISCI_DEV_UART6,
    TISCI_DEV_USB0,
    TISCI_DEV_SERDES_10G0
};

/**
 * \brief Disables module clock
 *
 * \return  BOARD_SOK   - Clock disable successful.
 *          BOARD_FAIL  - Clock disable failed.
 *
 */
Board_STATUS Board_moduleClockDisable(uint32_t moduleId)
{
    Board_STATUS retVal = BOARD_SOK;
	int32_t      status = CSL_EFAIL;
    uint32_t     moduleState = 0U;
    uint32_t     resetState = 0U;
    uint32_t     contextLossState = 0U;

    /* Get the module state.
       No need to change the module state if it
       is already OFF
     */
    status = Sciclient_pmGetModuleState(moduleId,
                                        &moduleState,
                                        &resetState,
                                        &contextLossState,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF)
    {
        status = Sciclient_pmSetModuleState(moduleId,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                            (TISCI_MSG_FLAG_AOP |
                                             TISCI_MSG_FLAG_DEVICE_RESET_ISO),
                                             SCICLIENT_SERVICE_WAIT_FOREVER);
        if (status == CSL_PASS)
        {
            status = Sciclient_pmSetModuleRst (moduleId,
                                               0x1U,
                                               SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                retVal = BOARD_FAIL;
            }
        }
        else
        {
            retVal = BOARD_FAIL;
        }
    }

    return retVal;
}

/**
 * \brief Enables module clock
 *
 * \return  BOARD_SOK   - Clock enable sucessful. 
 *          BOARD_FAIL  - Clock enable failed.
 *
 */
Board_STATUS Board_moduleClockEnable(uint32_t moduleId)
{
    Board_STATUS retVal = BOARD_SOK;
	int32_t      status = CSL_EFAIL;
    uint32_t     moduleState = 0U;
    uint32_t     resetState = 0U;
    uint32_t     contextLossState = 0U;

    /* Get the module state. 
       No need to change the module state if it
       is already ON 
     */
    status = Sciclient_pmGetModuleState(moduleId,
                                        &moduleState,
                                        &resetState,
                                        &contextLossState,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(moduleState == TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF)
    {
        status = Sciclient_pmSetModuleState(moduleId,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            (TISCI_MSG_FLAG_AOP |
                                             TISCI_MSG_FLAG_DEVICE_RESET_ISO),
                                             SCICLIENT_SERVICE_WAIT_FOREVER);

        if (status == CSL_PASS)
        {
            status = Sciclient_pmSetModuleRst (moduleId,
                                               0x0U,
                                               SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                retVal = BOARD_FAIL;
            }
        }
        else
        {
            retVal = BOARD_FAIL;
        }
    }

    return retVal;
}

/**
 * \brief clock Initialization function
 *
 * \return  BOARD_SOK              - Clock initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
static Board_STATUS Board_moduleClockInit(uint32_t *clkData, uint32_t size)
{
	Board_STATUS  status = BOARD_SOK;
    uint32_t index;

    for(index = 0; index < size; index++)
    {
        status = Board_moduleClockEnable(clkData[index]);
        if(status != BOARD_SOK)
        {
            status = BOARD_INIT_CLOCK_FAIL;
            break;
        }
    }

    return status;
}

/**
 * \brief Clock de-initialization function
 *
 *
 * \return  BOARD_SOK              - Clock de-initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
static Board_STATUS Board_moduleClockDeinit(uint32_t *clkData, uint32_t size)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t index;

    for(index = 0; index < size; index++)
    {
        status = Board_moduleClockDisable(clkData[index]);
        if(status != BOARD_SOK)
        {
            status = BOARD_INIT_CLOCK_FAIL;
            break;
        }
    }

    return status;
}

static Board_STATUS Board_initCPUClkAttrs(void)
{
    Board_STATUS  status = BOARD_SOK;
    int32_t  ret;
    uint64_t cpuClkFreq, cpuModuleId, cpuClockId;

#ifdef BUILD_M4F_0
    cpuModuleId = TISCI_DEV_MCU_M4FSS0_CORE0;
    cpuClockId = TISCI_DEV_MCU_M4FSS0_CORE0_DAP_CLK;
#endif
#ifdef BUILD_MCU1_0
    cpuModuleId = TISCI_DEV_R5FSS0_CORE0;
    cpuClockId = TISCI_DEV_R5FSS0_CORE0_CPU_CLK;
#endif
#ifdef BUILD_MCU1_1
    cpuModuleId = TISCI_DEV_R5FSS0_CORE1;
    cpuClockId = TISCI_DEV_R5FSS0_CORE1_CPU_CLK;
#endif
#ifdef BUILD_MCU2_0
    cpuModuleId = TISCI_DEV_R5FSS1_CORE0;
    cpuClockId = TISCI_DEV_R5FSS1_CORE0_CPU_CLK;
#endif
#ifdef BUILD_MCU2_1
    cpuModuleId = TISCI_DEV_R5FSS1_CORE1;
    cpuClockId = TISCI_DEV_R5FSS1_CORE1_CPU_CLK;
#endif
#ifdef BUILD_MPU
    cpuModuleId = TISCI_DEV_A53SS0_CORE_0;
    cpuClockId = TISCI_DEV_A53SS0_CORE_0_A53_CORE0_ARM_CLK_CLK;
#endif
    ret = Sciclient_pmGetModuleClkFreq(cpuModuleId,
                                       cpuClockId,
                                       &cpuClkFreq,
                                       SCICLIENT_SERVICE_WAIT_FOREVER);
    if(ret == 0)
    {
        Osal_HwAttrs  hwAttrs;
        uint32_t      ctrlBitmap;

        ret = Osal_getHwAttrs(&hwAttrs);
        if(ret == 0)
        {
            /*
             * Change the timer input clock frequency configuration
               based on R5 CPU clock configured
             */
            hwAttrs.cpuFreqKHz = (int32_t)(cpuClkFreq/1000U);
            ctrlBitmap         = OSAL_HWATTR_SET_CPU_FREQ;
            ret = Osal_setHwAttrs(ctrlBitmap, &hwAttrs);
        }
    }

    if(ret != 0)
    {
        status = BOARD_INIT_CLOCK_FAIL;
    }

    return status;
}

/**
 * \brief clock Initialization function for MCU domain
 *
 * Enables different power domains and peripheral clocks of the MCU.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization sucessful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInitMcu(void)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t size;

    status = Board_initCPUClkAttrs();
    if( status == BOARD_SOK)
    {
        if((gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP_ALL) ||
           (gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP1))
        {
            size = sizeof(gBoardClkModuleMcuIDInitGroupl) / sizeof(uint32_t);
            status = Board_moduleClockInit(gBoardClkModuleMcuIDInitGroupl, size);
            if(status != BOARD_SOK)
            {
                return status;
            }
        }

        if((gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP_ALL) ||
           (gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP2))
        {
            size = sizeof(gBoardClkModuleMcuIDInitGroup2) / sizeof(uint32_t);
            status = Board_moduleClockInit(gBoardClkModuleMcuIDInitGroup2, size);
            if(status != BOARD_SOK)
            {
                return status;
            }
        }
    }

    return status;
}

/**
 * \brief clock Initialization function for MAIN domain
 *
 * Enables different power domains and peripheral clocks of the SoC.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInitMain(void)
{
	Board_STATUS  status = BOARD_SOK;
    uint32_t size;

    status = Board_initCPUClkAttrs();
    if( status == BOARD_SOK)
    {
        if((gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP_ALL) ||
           (gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP1))
        {
            size = sizeof(gBoardClkModuleMainIDGroup1) / sizeof(uint32_t);
            status = Board_moduleClockInit(gBoardClkModuleMainIDGroup1, size);
            if(status != BOARD_SOK)
            {
                return status;
            }
        }

        if((gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP_ALL) ||
           (gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP2))
        {
            size = sizeof(gBoardClkModuleMainIDGroup2) / sizeof(uint32_t);
            status = Board_moduleClockInit(gBoardClkModuleMainIDGroup2, size);
        }
    }

    return status;
}

/**
 * \brief clock de-initialization function for MCU domain
 *
 * Disables different power domains and peripheral clocks of the SoC.
 *
 * \return  BOARD_SOK              - Clock de-initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
Board_STATUS Board_moduleClockDeinitMcu(void)
{
	Board_STATUS  status = BOARD_SOK;
    uint32_t size;

    if((gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP1))
    {
        size = sizeof(gBoardClkModuleMcuIDDeinitGroupl) / sizeof(uint32_t);
        status = Board_moduleClockDeinit(gBoardClkModuleMcuIDDeinitGroupl, size);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    if((gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP2))
    {
        size = sizeof(gBoardClkModuleMcuIDInitGroup2) / sizeof(uint32_t);
        status = Board_moduleClockDeinit(gBoardClkModuleMcuIDInitGroup2, size);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    return status;
}

/**
 * \brief clock de-initialization function for MAIN domain
 *
 * Disables different power domains and peripheral clocks of the SoC.
 *
 * \return  BOARD_SOK              - Clock de-initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
Board_STATUS Board_moduleClockDeinitMain(void)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t size;

    if((gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP1))
    {
        size = sizeof(gBoardClkModuleMainIDDeinitGroup1) / sizeof(uint32_t);
        status = Board_moduleClockDeinit(gBoardClkModuleMainIDDeinitGroup1, size);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    if((gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP2))
    {
        size = sizeof(gBoardClkModuleMainIDGroup2) / sizeof(uint32_t);
        status = Board_moduleClockDeinit(gBoardClkModuleMainIDGroup2, size);
    }

    return status;
}
