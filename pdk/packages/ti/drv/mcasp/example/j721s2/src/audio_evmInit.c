/*
 * audio_evmInit.c
 *
 * This file contains Audio application related EVM (platform) specific routines
 * implementation. 
 * This is part of the EVM Initialization library (evmInit) which contains pinmux
 * enabling routines for peripherals used by the application, and may contain 
 * related device pre-driver initialization routines.
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
 *
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
 *  \file   audio_evmInit.c
 *
 *  \brief  This file contains the board specific code for enabling the use of
 *          audio driver.
 *
 *  (C) Copyright 2018, Texas Instruments, Inc
 *
 */


#include <stdio.h>

#include "mcasp_osal.h"

#define SW_I2C
#ifndef SW_I2C
#include <i2c/include/I2c.h>
#endif

#include <Audio_evmInit.h>
#include <mcasp_cfg.h>
#include <ti/csl/cslr_device.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/csl_intr_router.h>
#include <ti/csl/csl_rat.h>
#include <ti/csl/soc.h>
#include <ti/board/board.h>

#include <ti/board/src/j721s2_evm/include/board_control.h>
#include <ti/board/src/j721s2_evm/include/board_utils.h>

/* I2C Driver Header Files */
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>

#include <ti/drv/mcasp/soc/mcasp_soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/udma/udma.h>
#include "MCASP_log.h"


//#define HW_WR_REG32(addr, data)   *(unsigned int*)(addr) =(unsigned int)(data)

/*============================================================================*/
/*                          MACRO DEFINITIONS                                 */
/*============================================================================*/

/* ========================================================================== */
/*                           MACRO DEFINTIONS                                 */
/* ========================================================================== */

uint32_t vaOffset = 0;

#if !defined (DEVICE_LOOPBACK)
void IoExpanderConfig(void);
#endif

#include <stdlib.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/csl/soc.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>

#define PIN1    (0x0036U)

/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] = {
    PIN1 | GPIO_CFG_OUTPUT,
};

/* GPIO Driver call back functions */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,
};

/* GPIO Driver configuration structure */
GPIO_v0_Config GPIO_v0_config = {
    gpioPinConfigs,
    gpioCallbackFunctions,
    sizeof(gpioPinConfigs) / sizeof(GPIO_PinConfig),
    sizeof(gpioCallbackFunctions) / sizeof(GPIO_CallbackFxn),
    0,
};

/**
 *  \brief    Initializes platform specific modules
 *
 *  This function initializes the modules like PLL, DDR, I2C etc
 *  which are required for audio processing. Need to call this function
 *  before doing any HW related operations.
 *
 *  \return    Platform_EOK on Success or error code
 */

void McASP_Enable(void);
void MCASP_pinmux(void);

Board_STATUS status = BOARD_SOK;

void MCASP_pinmux(void)
{
    HW_WR_REG32(0x11C0C8, 0x50001); /* MCASP4_ACLKX */
    HW_WR_REG32(0x11C06C, 0x50001); /* MCASP4_AFSX */
    HW_WR_REG32(0x11C0C4, 0x50001); /* MCASP4_AXR2 */  
    HW_WR_REG32(0x11C070, 0x50001); /* MCASP4_AXR3 */
    HW_WR_REG32(0x11C068, 0x50001); /* MCASP4_AXR1 */
    HW_WR_REG32(0x11C078, 0x50001); /* AUDIO_EXT_REFCLK1 */ 
    HW_WR_REG32(0x4301C0A0, 0x50007); /* WKUP_GPIO0_54 for I2c3 Mux select */

}
/*
 * Function to enable the pinmux for the mcasp and i2c devices in the soc.
 *
 */
void configureAudio(void)
{
	Board_STATUS stat = BOARD_SOK;
	Board_initCfg arg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;
    Sciclient_ConfigPrms_t sciClientCfg;

	stat = Board_init(arg);
	if(stat != BOARD_SOK)
    {
	    MCASP_log("Board init failed!!");
	}
    MCASP_log("Board init successfull\n");

    MCASP_pinmux();
    
#if !defined (DEVICE_LOOPBACK)
    IoExpanderConfig();
#endif
    
    /* Configure AUDIO_REFCLK1 as output */
    HW_WR_REG32(0x01082E4, 0x8000);
    
    Sciclient_configPrmsInit(&sciClientCfg);
    Sciclient_init(&sciClientCfg);
    
    McASP_Enable();

	MCASP_log("\n Pinmux Config complete");
}

void McASP_Enable(void)
{
    /* McASP4 AUXCLK selects MAIN_PLL4_HSDIV0_CLKOUT */
    Sciclient_pmSetModuleClkParent(TISCI_DEV_MCASP4,
                                   TISCI_DEV_MCASP4_AUX_CLK,
                                   TISCI_DEV_MCASP4_AUX_CLK_PARENT_HSDIV2_16FFT_MAIN_4_HSDIVOUT0_CLK,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);

    /* Select AUDIO_EXT_REFCLK1 input to use McASP4_AHCLKX signal, and set
     * AUDIO_EXT_REFCLK1 as output */
    Sciclient_pmSetModuleClkParent(TISCI_DEV_BOARD0,
                                   TISCI_DEV_BOARD0_AUDIO_EXT_REFCLK1_IN,
                                   TISCI_DEV_BOARD0_AUDIO_EXT_REFCLK1_IN_PARENT_MCASP_MAIN_4_MCASP_AHCLKX_POUT,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);

    /* Send AUDIO_REFCLK1 OBSCLK0 for debug purposes */
    Sciclient_pmSetModuleClkParent(TISCI_DEV_BOARD0,
                                   TISCI_DEV_BOARD0_OBSCLK0_IN,
                                   TISCI_DEV_BOARD0_OBSCLK0_IN_PARENT_HSDIV2_16FFT_MAIN_4_HSDIVOUT0_CLK,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
}

extern I2C_Handle gIoExpI2cHandle;

void IoExpanderConfig(void)
{
    GPIO_v0_HwAttrs gpioCfg;
    GPIO_socGetInitCfg(0, &gpioCfg);
    gpioCfg.baseAddr = CSL_WKUP_GPIO0_BASE;
    GPIO_socSetInitCfg(0, &gpioCfg);
    GPIO_init();

    Board_IoExpCfg_t ioExpCfg;

    /* Set MCASP/TRACE_MUX_S1 to HIGH */
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_1;
    ioExpCfg.pinNum      = PIN_NUM_2;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);
    if(status != BOARD_SOK)
    {
        MCASP_log("\n Board setting data to 3 port IO expander failed\n");
    }
    /* Set MCASP/TRACE_MUX_S0 to LOW */
    ioExpCfg.pinNum      = PIN_NUM_1;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;
    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);
    if(status != BOARD_SOK)
    {
        MCASP_log("\n Board setting data to 3 port IO expander failed\n");
    }

    status = Board_control(BOARD_CTRL_CMD_SET_SOM_MUX_PORTB1, NULL);
    if(status != BOARD_SOK)
    {
        MCASP_log("\n Board setting data to 3 port IO expander failed\n");
    }

    /* Enabling I2C3 mux */
    GPIO_write(0U, 1U);

    /* Bring the codec out of reset */
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE3_ADDR;
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE3_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_0;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);
    if(status != BOARD_SOK)
    {
        MCASP_log("\n Board setting data to one port IO expander failed\n");
    }

}

/*
 * Function to configure SOC specific parameters. This includes SOC
 * specific parameters such as interrupt configuration using Muxes,
 * interrupt configuration etc.
 */
void configMcASP_SocHwInfo()
{
  /* Configure the interrupts for the McASP Instance MCASP_NUM */
  /* ON Keystone, it involves CIC programming as well.
   * The McASP does that internally, if configured with the correct parameters.
   * Such as muxNum, muxInEvent, muxOutEvent,
   * cpuInEventNum, cpuIntNum
  */
    Mcasp_HwInfo hwInfo;

    Mcasp_socGetInitCfg(MCASP_NUM, &hwInfo);

     /* Write back */
    Mcasp_socSetInitCfg(MCASP_NUM,&hwInfo);
}
/* ========================================================================== */
/*                              END OF FILE                                   */
/* ========================================================================== */
