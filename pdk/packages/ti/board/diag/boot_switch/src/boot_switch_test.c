/*
 *  Copyright (C) 2018-2022 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file     boot_switch_test.c
 *
 *  \brief    This file contains boot switch test code.
 *
 *  Targeted Functionality: Verifying the boot switch functionality.
 *
 *  Operation: This test verifies the boot switch mode by reading the 
 *              boot values and compared with expected values.
 *
 *  Supported SoCs: AM65XX, J721E, J7200, AM64x, J721S2, J784S4.
 *
 *  Supported Platforms: am65xx_idk, am65xx_evm, j721e_evm, j7200_evm, am64x_evm,
 *                       j721s2_evm, j784s4_evm.
 *
 */

#include "boot_switch_test.h"

#if (defined(am65xx_evm) || defined(am65xx_idk))
static uint32_t pinMuxgpio[PADCONFIG_MAX_COUNT] =
{
    PIN_GPMC0_AD0,
    PIN_GPMC0_AD1,
    PIN_GPMC0_AD2,
    PIN_GPMC0_AD3,
    PIN_GPMC0_AD4,
    PIN_GPMC0_AD5,
    PIN_GPMC0_AD6,
    PIN_GPMC0_AD7,
    PIN_GPMC0_AD8,
    PIN_GPMC0_AD9,
    PIN_GPMC0_AD10,
    PIN_GPMC0_AD11,
    PIN_GPMC0_AD12,
    PIN_GPMC0_AD13,
    PIN_GPMC0_AD14,
    PIN_GPMC0_AD15,
    PIN_GPMC0_ADVN_ALE,
    PIN_GPMC0_OEN_REN,
    PIN_GPMC0_WEN,
    PIN_WKUP_GPIO0_3, /* SW4 - 1 */
    PIN_WKUP_GPIO0_4  /* SW4 - 2 */
};

switchDetails_t swDetails[NUM_OF_SW] = {
    { "SW3", 10,  0 },
    { "SW2",  9, 10 },
    { "SW4",  2, 19 }
};
#elif defined(am64x_evm)
static uint32_t pinMuxgpio[PADCONFIG_MAX_COUNT] =
{
    /*      SW2         */
    PIN_GPMC0_AD0,
    PIN_GPMC0_AD1,
    PIN_GPMC0_AD2,
    PIN_GPMC0_AD3,
    PIN_GPMC0_AD4,
    PIN_GPMC0_AD5,
    PIN_GPMC0_AD6,
    PIN_GPMC0_AD7,
    /*      SW3         */
    PIN_GPMC0_AD8,
    PIN_GPMC0_AD9,
    PIN_GPMC0_AD10,
    PIN_GPMC0_AD11,
    PIN_GPMC0_AD12,
    PIN_GPMC0_AD13,
    PIN_GPMC0_AD14,
    PIN_GPMC0_AD15
};

switchDetails_t swDetails[NUM_OF_SW] = {
    { "SW2", 8,  0 },
    { "SW3", 8,  8 }
};
#elif defined(SOC_J721E)
static uint32_t pinMuxgpio[PADCONFIG_MAX_COUNT] =
{
    /*      SW8         */
    PIN_PRG0_PRU0_GPO18,
    PIN_PRG1_PRU1_GPO17,
    PIN_PRG0_PRU0_GPO5,
    PIN_PRG0_PRU1_GPO17,
    PIN_TIMER_IO0,
    PIN_TIMER_IO1,
    PIN_PRG0_PRU1_GPO5,
    PIN_PRG0_PRU0_GPO17,
    /*      SW9         */
    PIN_MCU_SPI0_D1,
    PIN_WKUP_GPIO0_0,
    PIN_WKUP_GPIO0_1,
    PIN_WKUP_GPIO0_2,
    PIN_WKUP_GPIO0_14,
    PIN_WKUP_GPIO0_15,
    PIN_WKUP_GPIO0_12,
    PIN_WKUP_GPIO0_13
};
switchDetails_t swDetails[NUM_OF_SW] = {
    { "SW8",  8, 0 },
    { "SW9",  8, 8 }
};
#elif defined(SOC_J721S2) || defined(SOC_J784S4)
static uint32_t pinMuxgpio[PADCONFIG_MAX_COUNT] =
{
    /*      SW8         */
    PIN_MCU_OSPI0_D0,
    PIN_MCU_OSPI0_D1,
    PIN_MCU_OSPI0_D4,
    PIN_MCU_OSPI0_D5,
    PIN_WKUP_GPIO0_56,
    PIN_WKUP_GPIO0_57,
    PIN_WKUP_GPIO0_66,
    PIN_WKUP_GPIO0_67,
    /*      SW9         */
    PIN_MCU_SPI0_D1,
    PIN_WKUP_GPIO0_0,
    PIN_WKUP_GPIO0_1,
    PIN_WKUP_GPIO0_2,
    PIN_WKUP_GPIO0_14,
    PIN_WKUP_GPIO0_15,
    PIN_WKUP_GPIO0_12,
    PIN_WKUP_GPIO0_13
};
#if defined (j784s4_evm)
switchDetails_t swDetails[NUM_OF_SW] = {
    { "SW11", 7, 0 },
    { "SW7",  8, 8 }
#elif defined(j721s2_evm)
switchDetails_t swDetails[NUM_OF_SW] = {
    { "SW8",  7, 0 },
    { "SW9",  8, 8 }
#endif
};
#elif defined(SOC_J7200)
static uint32_t pinMuxgpio[PADCONFIG_MAX_COUNT] =
{
    /*      SW8         */
    PIN_MCU_OSPI0_D0,
    PIN_MCU_OSPI0_D1,
    PIN_MCU_OSPI0_D4,
    PIN_MCU_OSPI0_D5,
    PIN_WKUP_GPIO0_77,
    PIN_WKUP_GPIO0_78,
    PIN_WKUP_GPIO0_80,
    PIN_WKUP_GPIO0_81,
    /*      SW9         */
    PIN_MCU_SPI0_D1,
    PIN_WKUP_GPIO0_0,
    PIN_WKUP_GPIO0_1,
    PIN_WKUP_GPIO0_2,
    PIN_WKUP_GPIO0_14,
    PIN_WKUP_GPIO0_15,
    PIN_WKUP_GPIO0_12,
    PIN_WKUP_GPIO0_13
};
switchDetails_t swDetails[NUM_OF_SW] = {
    /* BOOTMODE7 (SW8.8) pin is used as WKUP_LFOSC_XI on J7200 platform which
     * cannot be controlled through boot switch after board boot-up.
     * Excluding this switch from the test.
     */
    { "SW8",  7, 0 },
    { "SW9",  8, 8 }
};
#endif

#if !defined(DIAG_COMPLIANCE_TEST)
char *userMsg[] =
{
   "Set All switches to OFF",
   "Set the Switches to ON-OFF-ON-OFF... (Start from 1)",
   "Set the Switches to OFF-ON-OFF-ON... (Start from 1)",
   "Set All switches to ON"
};
#endif

#if (defined(am65xx_evm) || defined(am65xx_idk))
/* PinMux configuration for WKUP domain */
static void BoardDiag_wakeupPinMuxSetMode(uint32_t offset, uint32_t mode)
{
    volatile uint32_t *addr;
    addr = (uint32_t *)((CSL_WKUP_CTRL_MMR0_CFG0_BASE + 0x1C000) + offset);

    *addr &= ~(MODE_PINMUX_MASK);
    *addr |= mode;
}
#endif

/**
 *  \brief    This function reads the position of switches.
 *
 *  \param    numOfSwPos    [IN]      Total number of switches
 *            pinOffset     [IN]      Start of switch number
 *
 *  \return   uint32_t
 *               rdSwPos - read value.
 *
 */
static uint32_t readSwPositions(uint8_t numOfSwPos, int8_t pinOffset)
{
    uint32_t rdSwPos = 0x0000U;
    uint8_t swPosIndex = 0U;
    uint8_t rdSignalLevel;

#if !(defined(SOC_AM64X))
    if (pinOffset >= START_OF_WKUP_PIN_OFFSET)
    {
        /* Initializing GPIO to MAIN domain */
        GPIO_v0_HwAttrs gpioCfg;
        GPIO_socGetInitCfg(0, &gpioCfg);
        gpioCfg.baseAddr = CSL_WKUP_GPIO0_BASE;
        GPIO_socSetInitCfg(0, &gpioCfg);
        GPIO_init();
    }
#endif

    while(numOfSwPos > swPosIndex)
    {
        rdSignalLevel = GPIO_read(pinOffset);
        rdSwPos = (rdSwPos | (rdSignalLevel << swPosIndex));
        swPosIndex++;
        pinOffset++;
    }

    return rdSwPos;
}

#if !defined(DIAG_COMPLIANCE_TEST)
/**
 *  \brief    This function generates pattern for switches.
 *
 *  \param    patternNum    [IN]      Pattern number
 *            length        [IN]      Length of the pattern.
 *
 *  \return   uint16_t
 *               genPattern - Generated pattern.
 *
 */
static uint16_t BoardDiag_patternGen(bootSwVerifyPattern_t patternNum,
                                     uint8_t numOfSwPos)
{
    uint16_t genPattern = 0;
    uint8_t swPosIndex = 0;
    uint8_t value;

    switch(patternNum)
    {
        case PATTERN_00:
            genPattern = 0;
            break;

        case PATTERN_55:
            value = 1;
            do
            {
                genPattern = (genPattern | (value << swPosIndex));
                value = !value;
                swPosIndex++;
            }while(swPosIndex < numOfSwPos);
            break;

        case PATTERN_AA:
            value = 0;
            do
            {
                genPattern = (genPattern | (value << swPosIndex));
                value = !value;
                swPosIndex++;
            }while(swPosIndex < numOfSwPos);
            break;

        case PATTERN_FF:
            value = 1;
            do
            {
                genPattern = (genPattern | (value << swPosIndex));
                swPosIndex++;
            }while(swPosIndex < numOfSwPos);
            break;
    }

    return (genPattern);
}
#endif
/**
 *  \brief    This function runs the boot mode switch test.
 *
 *  \return   int8_t
 *               0 - in case of success.
 *
 */
static int8_t BoardDiag_bootSwTest(void)
{
#if !defined(DIAG_COMPLIANCE_TEST)
    bootSwVerifyPattern_t verifyPattern = PATTERN_00;
	int8_t ret;
	uint16_t pattern;
	char userInput;
#endif
    uint16_t swPos;
    int8_t index;
    uint8_t i,j;
#if (!(defined(am65xx_evm) || defined(am65xx_idk)))
    Board_STATUS status = BOARD_SOK;
#endif

	/* set board pin mux mode to MAIN domain */
    for(index = 0; index < MAIN_PADCONFIG_MAX_COUNT; index++)
    {
#if (defined(am65xx_evm) || defined(am65xx_idk))
        Board_pinMuxSetMode(pinMuxgpio[index], (GPIO_PADCONFIG_MUX_MODE |
                            ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) &
                            (~PIN_PULL_DIRECTION))));
#else
        status = Board_pinmuxSetReg(BOARD_SOC_DOMAIN_MAIN,
                                    pinMuxgpio[index],
                                    BOARD_GPIO_PIN_MUX_CFG);
        if (BOARD_SOK != status)
        {
            return status;
        }
#endif
    }

#if !defined(SOC_AM64X)
	/* set board pin mux mode to WAKEUP domain */
    for(index = MAIN_PADCONFIG_MAX_COUNT; index < TOT_NUM_OF_SW; index++)
    {
#if (defined(am65xx_evm) || defined(am65xx_idk))
        BoardDiag_wakeupPinMuxSetMode(pinMuxgpio[index],
                            (GPIO_PADCONFIG_MUX_MODE |
                            ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) &
                            (~PIN_PULL_DIRECTION))));
#else
        status = Board_pinmuxSetReg(BOARD_SOC_DOMAIN_WKUP,
                                    pinMuxgpio[index],
                                    BOARD_GPIO_PIN_MUX_CFG);
        if (status != BOARD_SOK)
        {
            return status;
        }
#endif
    }
#endif
    
#if ((defined(am65xx_evm)) || (defined(am65xx_idk)))
    *(uint32_t *)(0x4301C0C0) = 0x0040007;
    *(uint32_t *)(0x4301C0C4) = 0x0040007;
#endif

#if defined(DIAG_COMPLIANCE_TEST)
    /* Initializing GPIO to MAIN domain */
	GPIO_v0_HwAttrs gpioCfg;
	GPIO_socGetInitCfg(0, &gpioCfg);
	gpioCfg.baseAddr = CSL_GPIO0_BASE;
	GPIO_socSetInitCfg(0, &gpioCfg);
	GPIO_init();
	for(i = 0; i < NUM_OF_SW; i++)
	{
		swPos = readSwPositions(swDetails[i].numOfSwPos, swDetails[i].pinOffset);
		UART_printf("Boot Switch %s Value - ", swDetails[i].swName);
        for(j = 0; j < swDetails[i].numOfSwPos; j++)
        {
            if ((swPos >> j) & 1)
            {
                UART_printf("ON ");
            }
            else
            {
                UART_printf("OFF ");
            }
        }
         UART_printf("\n");
	}
#else
    for(i = 0; i < NUM_OF_PATTERNS; i++)
    {
        UART_printf("%s\n", userMsg[i]);
        UART_printf("Press Enter after setting the switches\n\r");
        UART_scanFmt("%c", &userInput);

        /* Initializing GPIO to MAIN domain */
        GPIO_v0_HwAttrs gpioCfg;
        GPIO_socGetInitCfg(0, &gpioCfg);
#if defined (j784s4_evm)
        gpioCfg.baseAddr = CSL_WKUP_GPIO0_BASE;
#else
        gpioCfg.baseAddr = CSL_GPIO0_BASE;
#endif
        GPIO_socSetInitCfg(0, &gpioCfg);
        GPIO_init();

        for(j = 0; j < NUM_OF_SW; j++)
        {
            pattern = BoardDiag_patternGen(verifyPattern, 
                                           swDetails[j].numOfSwPos);
            do
            {
                swPos = readSwPositions(swDetails[j].numOfSwPos,
                                        swDetails[j].pinOffset);
                if (swPos == pattern)
                {
                    ret = 0;
                }
                else
                {
                    UART_printf("Expected Setting for the Switch %s is not Matching with Set Value - 0x%X\n", swDetails[j].swName, swPos);
                    UART_printf("Please Set the Correct Value\n\r");
                    UART_printf("Press Any Key After Setting the Switch\n\r");
                    UART_scanFmt("%c", &userInput);
                    ret = -1;
                }

            }while(0 != ret);
        }
        verifyPattern++;
    }
#endif
    UART_printf("\nTest Passed\n");
    return 0;
}

#if (defined(j721e_evm) || defined(j7200_evm) || defined(j721s2_evm) || defined(j784s4_evm)) && !defined (__aarch64__)
/* Function to enable MAIN UART0 */
void Board_enableMAINUART0(void)
{
    UART_HwAttrs uartCfg;

    UART_socGetInitCfg(0, &uartCfg);
    uartCfg.baseAddr = CSL_UART0_BASE;
    uartCfg.enableInterrupt = UFALSE;
    uartCfg.frequency = 48000000;

    UART_socSetInitCfg(0, &uartCfg);
}
#endif

/**
 * \brief  Boot switch test main function
 *
 * \return  int
 *            0  - in case of success
 *           -1  - in case of failure
 */
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    uint8_t ret;
#if (defined(j721e_evm) || defined(j7200_evm) || defined(j721s2_evm) || defined(j784s4_evm)) && !defined (__aarch64__)
    Board_initParams_t initParams;

    Board_getInitParams(&initParams);
#if defined(j784s4_evm)
    initParams.uartInst = BOARD_UART_INSTANCE;
#else
    initParams.uartInst = 0;
#endif
    initParams.uartSocDomain = BOARD_SOC_DOMAIN_MAIN;
    Board_setInitParams(&initParams);
#endif

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO |
               BOARD_INIT_PINMUX_CONFIG;
#endif

    status = Board_init(boardCfg);
    if(BOARD_SOK != status)
    {
        return -1;
    }

    UART_printf("\n*********************************************\n");
    UART_printf  ("*             Boot Switch Test              *\n");
    UART_printf  ("*********************************************\n");

    ret = BoardDiag_bootSwTest();
    return ret;
}
/* Nothing past this point */
