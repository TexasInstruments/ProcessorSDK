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
#if defined (USE_BIOS)
#include <xdc/std.h>
#endif /* USE BIOS */

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
#include <ti/board/src/j7200_evm/include/board_control.h>
#include <ti/board/src/j7200_evm/include/board_utils.h>

/* I2C Driver Header Files */
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>

#include <ti/drv/mcasp/soc/mcasp_soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/udma/udma.h>
#include "MCASP_log.h"

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

/*
 * Function to enable the pinmux for the mcasp and i2c devices in the soc.
 *
 */
void configureAudio(void)
{
	Board_STATUS stat = BOARD_SOK;
	Board_initCfg arg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;
    Sciclient_ConfigPrms_t sciClientCfg;

#if defined(j7200_evm)
    Board_PinmuxConfig_t pinmuxCfg;
    Board_pinmuxGetCfg(&pinmuxCfg);
    pinmuxCfg.somMux = BOARD_PINMUX_SOM_AUDIO;
    Board_pinmuxSetCfg(&pinmuxCfg);
#endif

    stat = Board_init(arg);
    if(stat != BOARD_SOK)
    {
            MCASP_log("Board init failed!!");
    }

#if !defined (DEVICE_LOOPBACK)
    IoExpanderConfig();
#endif

    /* Configure AUDIO_REFCLK1 as output */
    /* TODO: Workaround till clockout config is done by sciclient */
    HW_WR_REG32(0x01082E4, 0x8000);

    Sciclient_configPrmsInit(&sciClientCfg);
    Sciclient_init(&sciClientCfg);

    McASP_Enable();

	MCASP_log("\n Pinmux Config complete");
}

void McASP_Enable(void)
{
    /* McASP0 AUXCLK selects MAIN_PLL4_HSDIV0_CLKOUT */
    Sciclient_pmSetModuleClkParent(TISCI_DEV_MCASP0,
                                   TISCI_DEV_MCASP0_AUX_CLK,
                                   TISCI_DEV_MCASP0_AUX_CLK_PARENT_HSDIV2_16FFT_MAIN_4_HSDIVOUT0_CLK,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);

    /* Select AUDIO_EXT_REFCLK1 input to use McASP0_AHCLKX signal, and set
     * AUDIO_EXT_REFCLK1 as output */
    Sciclient_pmSetModuleClkParent(TISCI_DEV_BOARD0,
                                   TISCI_DEV_BOARD0_AUDIO_EXT_REFCLK1_IN,
                                   TISCI_DEV_BOARD0_AUDIO_EXT_REFCLK1_IN_PARENT_MCASP_MAIN_0_MCASP_AHCLKX_POUT,
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

    Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);

    /* Set MCASP/TRACE_MUX_S0 to LOW */
    ioExpCfg.pinNum      = PIN_NUM_1;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;
    Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);

    /* Configure SoM IO mux */
    Board_control(BOARD_CTRL_CMD_SET_SOM_AUDIO_MUX, NULL);

    /* Bring the codec out of reset */
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE3_ADDR;
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE3_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_0;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);
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
