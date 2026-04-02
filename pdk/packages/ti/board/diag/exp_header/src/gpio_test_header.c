/******************************************************************************
 * Copyright (c) 2018-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   gpio_test_header.c
 *
 *  \brief  gpio test header diagnostic test file.
 *
 *  Targeted Functionality: Verifies the test headers using
 *                          GPIO loopback.
 *
 *  Operation: This test verifies by changing the signal level(High and Low)
 *             on one pin and verifying the same on the other pin
 *             with both the pins connected externally.
 *
 *  Supported SoCs: AM65xx, J721E, J7200, AM64x, J784S4.
 *
 *  Supported Platforms: am65xx_evm, am65xx_idk, j721e_evm, j7200_evm, am64x_evm, j784s4_evm.
 */

#include "gpio_test_header.h"

#if defined(SOC_AM65XX)
testHeaderPinDetails_t testHeaderPin[NUM_PIN_SETS] = {
    {"SPI PINS\0",      0U,     3U,     CSL_GPIO0_BASE},
    {"TIMER PINS\0",    3U,     2U,     CSL_GPIO0_BASE}

};

/* Pad Config register offset address details */
static uint32_t pinMuxgpio[PADCONFIG_MAX_COUNT] = 
{
    PIN_SPI1_D0,
    PIN_SPI1_D1,
    PIN_SPI1_CLK,
    PIN_TIMER_IO0,
    PIN_TIMER_IO1
};
#elif defined(SOC_AM64X)
testHeaderPinDetails_t testHeaderPin[NUM_PIN_SETS] = {
    {"HSE TEST JIG PINS\0",      0U,     85U,     CSL_GPIO0_BASE},
    {"SAFETY TEST JIG PINS\0",   85,     16U,     CSL_MCU_GPIO0_BASE}
};

/* Pad Config register offset address details */
static uint32_t pinMuxgpio[PADCONFIG_MAX_COUNT] =
{
    PIN_PRG0_MDIO0_MDIO,
    PIN_SPI1_D1,
    PIN_PRG0_MDIO0_MDC,
    PIN_SPI1_D0,
    PIN_PRG0_PRU0_GPO8,
    PIN_PRG0_PRU1_GPO8,
    PIN_PRG0_PRU0_GPO2,
    PIN_PRG0_PRU1_GPO1,
    PIN_PRG0_PRU0_GPO3,
    PIN_PRG0_PRU1_GPO0,
    PIN_PRG0_PRU0_GPO4,
    PIN_PRG0_PRU0_GPO7,
    PIN_PRG0_PRU0_GPO12,
    PIN_PRG0_PRU0_GPO17,
    PIN_PRG0_PRU1_GPO16,
    PIN_PRG0_PRU0_GPO18,
    PIN_GPMC0_AD15,
    PIN_PRG0_PRU0_GPO19,
    PIN_GPMC0_BE1N,
    PIN_PRG0_PRU0_GPO0,
    PIN_GPMC0_AD9,
    PIN_PRG0_PRU1_GPO4,
    PIN_GPMC0_AD8,
    PIN_PRG0_PRU0_GPO11,
    PIN_PRG0_PRU1_GPO12,
    PIN_GPMC0_AD14,
    PIN_GPMC0_AD10,
    PIN_GPMC0_BE0N_CLE,
    PIN_GPMC0_CLK,
    PIN_PRG0_PRU0_GPO10,
    PIN_SPI1_CLK,
    PIN_PRG0_PRU0_GPO13,
    PIN_PRG0_PRU0_GPO5,
    PIN_PRG0_PRU1_GPO3,
    PIN_PRG0_PRU0_GPO14,
    PIN_PRG0_PRU0_GPO6,
    PIN_PRG0_PRU1_GPO15,
    PIN_PRG0_PRU1_GPO2,
    PIN_PRG1_PRU1_GPO19,
    PIN_GPMC0_AD2,
    PIN_GPMC0_AD5,
    PIN_GPMC0_AD12,
    PIN_GPMC0_ADVN_ALE,
    PIN_GPMC0_AD7,
    PIN_GPMC0_WEN,
    PIN_GPMC0_CSN2,
    PIN_GPMC0_WAIT0,
    PIN_GPMC0_CSN3,
    PIN_GPMC0_WPN,
    PIN_PRG0_PRU1_GPO17,
    PIN_PRG0_PRU1_GPO19,
    PIN_MCAN1_RX,
    PIN_SPI1_CS0,
    PIN_PRG0_PRU1_GPO13,
    PIN_SPI1_CS1,
    PIN_PRG0_PRU1_GPO5,
    PIN_PRG0_PRU1_GPO11,
    PIN_GPMC0_AD1,
    PIN_PRG0_PRU0_GPO15,
    PIN_GPMC0_AD4,
    PIN_GPMC0_AD13,
    PIN_GPMC0_DIR,
    PIN_GPMC0_OEN_REN,
    PIN_MCAN0_TX,
    PIN_PRG0_PRU1_GPO18,
    PIN_MCAN1_TX,
    PIN_PRG0_PRU0_GPO1,
    PIN_PRG0_PRU1_GPO6,
    PIN_PRG0_PRU0_GPO16,
    PIN_PRG0_PRU1_GPO14,
    PIN_PRG1_PRU1_GPO18,
    PIN_GPMC0_AD3,
    PIN_GPMC0_CSN1,
    PIN_GPMC0_AD11,
    PIN_PRG0_PRU1_GPO9,
    PIN_GPMC0_WAIT1,
    PIN_MCAN0_RX,
    PIN_PRG0_PRU1_GPO10,
    PIN_GPMC0_AD0,
    PIN_GPMC0_AD6,
    PIN_I2C0_SCL,
    PIN_I2C0_SDA,
    PIN_PRG0_PRU1_GPO7,
    PIN_GPMC0_CSN0,
    PIN_PRG0_PRU0_GPO9,
/* Safety Connector Pins  */
    PIN_MCU_SPI0_D1,
    PIN_MCU_SPI0_CS0,
    PIN_MCU_UART1_CTSN,
    PIN_MCU_I2C1_SDA,
    PIN_MCU_SPI1_D1,
    PIN_MCU_SPI0_CLK,
    PIN_MCU_I2C1_SCL,
    PIN_MCU_UART1_TXD,
    PIN_MCU_SPI0_CS1,
    PIN_MCU_SPI1_CLK,
    PIN_MCU_SPI1_D0,
    PIN_MCU_UART1_RXD,
    PIN_MCU_UART1_RTSN,
    PIN_MCU_RESETSTATZ,
    PIN_MCU_I2C0_SCL,
    PIN_MCU_I2C0_SDA
};
#elif defined(SOC_J721E)
uint8_t gPadConfigMaxCount;
uint8_t gMainPadConfigMaxCount;
testHeaderPinDetails_t *testHeaderPin;
static uint32_t *pinMuxgpio;

/* Alpha GESI Board */
testHeaderPinDetails_t testHeaderPinAlpha[NUM_PIN_SETS] = {
    {"USS/IMU SENSOR HEADER PINS\0",        1U,     6U,     CSL_GPIO0_BASE},
    {"MOTOR CONTROL HEADER MAIN PINS\0",    7U,     20U,    CSL_GPIO0_BASE},
    {"I3C PINS\0",                          29U,    2U,     CSL_WKUP_GPIO0_BASE},
};

/* Pad Config register offset address details */
static uint32_t pinMuxgpioAlpha[PADCONFIG_MAX_COUNT_ALPHA] =
{
    /* USS/IMU SENSOR HEADER */
    PIN_PRG0_MDIO0_MDIO,
    PIN_PRG0_MDIO0_MDC,
    PIN_PRG1_PRU1_GPO7,
    PIN_PRG1_PRU1_GPO8,
    PIN_PRG1_MDIO0_MDIO,
    PIN_PRG1_MDIO0_MDC,
    /* MOTOR CONTROL HEADER */
    PIN_PRG0_PRU0_GPO9,
    PIN_PRG1_PRU0_GPO12,
    PIN_PRG1_PRU0_GPO14,
    PIN_PRG1_PRU0_GPO13,
    PIN_PRG1_PRU0_GPO15,
    PIN_PRG1_PRU0_GPO16,
    PIN_PRG1_PRU1_GPO5,
    PIN_PRG1_PRU0_GPO19,
    PIN_PRG1_PRU0_GPO18,
    PIN_PRG1_PRU0_GPO17,
    PIN_PRG1_PRU1_GPO18,
    PIN_PRG1_PRU0_GPO8,
    PIN_PRG1_PRU0_GPO11,
    PIN_PRG1_PRU0_GPO7,
    PIN_PRG1_PRU0_GPO6,
    PIN_PRG1_PRU1_GPO17,
    PIN_PRG1_PRU0_GPO4,
    PIN_PRG1_PRU0_GPO3,
    PIN_PRG1_PRU0_GPO9,
    PIN_PRG1_PRU0_GPO10,
    PIN_PRG1_PRU1_GPO19,
    PIN_PRG1_PRU0_GPO5,
    /* I3C HEADER PINS  */
    PIN_MCU_I3C0_SCL,
    PIN_MCU_I3C0_SDA
};

/* Beta GESI Board */
testHeaderPinDetails_t testHeaderPinBeta[NUM_PIN_SETS] = {
    {"USS/IMU SENSOR HEADER PINS\0",        1U,     4U,     CSL_GPIO0_BASE},
    {"MOTOR CONTROL HEADER MAIN PINS\0",    5U,     8U,    CSL_GPIO0_BASE},
    {"I3C PINS\0",                          15U,    2U,     CSL_WKUP_GPIO0_BASE},
};

/* Pad Config register offset address details */
static uint32_t pinMuxgpioBeta[PADCONFIG_MAX_COUNT_BETA] =
{
    /* USS/IMU SENSOR HEADER */
    PIN_PRG0_MDIO0_MDIO,
    PIN_PRG0_MDIO0_MDC,
    PIN_PRG1_PRU1_GPO7,
    PIN_PRG1_PRU1_GPO8,
    /* MOTOR CONTROL HEADER */
    PIN_PRG1_PRU1_GPO5,
    PIN_PRG1_PRU0_GPO19,
    PIN_PRG1_PRU0_GPO18,
    PIN_PRG1_PRU0_GPO17,
    PIN_PRG1_PRU1_GPO18,
    PIN_PRG1_PRU0_GPO8,
    PIN_PRG1_PRU0_GPO9,
    PIN_PRG1_PRU0_GPO10,
    PIN_PRG1_PRU1_GPO19,
    PIN_PRG1_PRU0_GPO5,
    /* I3C HEADER PINS  */
    PIN_MCU_I3C0_SCL,
    PIN_MCU_I3C0_SDA
};

static uint32_t mlbHeader_pinMuxgpio[NUM_OF_MLB_HEADER_PINS]={PIN_UART1_CTSN, PIN_UART0_CTSN, PIN_UART1_RTSN};

#elif defined(SOC_J784S4) /*j784s4_evm*/
uint8_t gMainPadConfigMaxCount = MAIN_PADCONFIG_MAX_COUNT;
uint8_t gPadConfigMaxCount = PADCONFIG_MAX_COUNT;

testHeaderPinDetails_t testHeaderPin[NUM_PIN_SETS] = {
    {"SPI HEADER PINS\0",     0U,     4U,     CSL_GPIO0_BASE},
    {"I3C PINS\0",            5U,     2U,     CSL_WKUP_GPIO0_BASE},
};

/* Pad Config register offset address details */
static uint32_t pinMuxgpio[PADCONFIG_MAX_COUNT] =
{
    /* SPI HEADER */
    /* Short pin1, pin2 and pin3, pin5 on J26 header */
    PIN_MCASP0_AXR3,    /* SPI5_CLK */
    PIN_MCAN1_RX,       /* SPI5_D0  */

    PIN_MCASP0_AXR6,    /* SPI5_D1  */
    PIN_MCAN0_RX,       /* SPI5_CS0 */
    PIN_MCAN0_TX,       /* SPI5_CS1 (By default high) */

    /* I3C HEADER PINS */
    /* Short pin2 and pin3 on J25 header */
    PIN_WKUP_GPIO0_8,
    PIN_WKUP_GPIO0_9,
};
#else /*j7200_evm*/
testHeaderPinDetails_t testHeaderPin[NUM_PIN_SETS] = {
    {"I3C PINS\0", 0U, 2U, CSL_WKUP_GPIO0_BASE},
};

/* Pad Config register offset address details */
static uint32_t pinMuxgpio[PADCONFIG_MAX_COUNT] = 
{
    /* I3C HEADER PINS  */
    PIN_WKUP_GPIO0_8,
    PIN_WKUP_GPIO0_9
};
#endif

/**
 * \brief  Configures GPIO pin and Performs the GPIO write operation.
 *
 * \param   pinVal [IN]    GPIO pin address
 *
 */
static void BoardDiag_modifyGpioBaseAddr(uint32_t baseAddress)
{
    GPIO_v0_HwAttrs gpioCfg;
    GPIO_socGetInitCfg(0, &gpioCfg);
    gpioCfg.baseAddr = baseAddress;
    GPIO_socSetInitCfg(0, &gpioCfg);
    GPIO_init();
}

/**
 *  \brief    This function used to perform the GPIO loopback
 *            verification test for a pair of pins passed.
 *
 *  \param    index              [IN]     GPIO output pin index
 *            gpioSignalLevel    [IN]     Signal value to write.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_runGpioTestHeaderVerification(uint8_t index,
                                                      uint8_t gpioSignalLevel)
{
    int8_t rdSignalLevel;
    uint8_t loopcnt;
    uint8_t pinIndex;
    int8_t  retVal = 0;
    pinIndex = testHeaderPin[index].offsetIndex;

    for (loopcnt = 0; loopcnt < ((testHeaderPin[index].numOfPins)/2); loopcnt++)
    {
        GPIO_write(pinIndex, gpioSignalLevel);
        BOARD_delay(500);
        rdSignalLevel = GPIO_read(pinIndex + 1U);
        if(rdSignalLevel != gpioSignalLevel)
        {
            if(gpioSignalLevel == GPIO_PIN_VAL_HIGH)
            {
                UART_printf("Looping back the signal high failed for pin %d\n\r", pinIndex);
                retVal = -1;
            }
            else
            {
                UART_printf("Looping back the signal low failed for pin %d\n\r", pinIndex);
                retVal = -1;
            }
        }
        else
        {
            if(gpioSignalLevel == GPIO_PIN_VAL_HIGH)
            {
                UART_printf("Looping back the signal high Passed for pin %d and pin %d\n\r", pinIndex, (pinIndex+1));
            }
            else
            {
                UART_printf("Looping back the signal low Passed for pin %d and pin %d\n\r", pinIndex, (pinIndex+1));
            }
        }

        GPIO_write(pinIndex, !gpioSignalLevel);
        pinIndex = pinIndex + 2U;
    }

    if(((testHeaderPin[index].numOfPins)%2) != 0)
    {
        rdSignalLevel = GPIO_read(pinIndex - 1U);
        if(rdSignalLevel != gpioSignalLevel)
        {
            if(gpioSignalLevel == GPIO_PIN_VAL_HIGH)
            {
                UART_printf("Looping back the signal high failed for pin %d\n\r", pinIndex);
                retVal = -1;
            }
            else
            {
                UART_printf("Looping back the signal low failed for pin %d\n\r", pinIndex);
                retVal = -1;
            }
        }
        else
        {
            if(gpioSignalLevel == GPIO_PIN_VAL_HIGH)
            {
                UART_printf("Looping back the signal high Passed for pin %d \n\r", pinIndex);
            }
            else
            {
                UART_printf("Looping back the signal low Passed for pin %d \n\r", pinIndex);
            }
        }
    }

    return retVal;
}

/**
 *  \brief    The function performs GPIO test header verification test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_runExpHeaderTest(void)
{
    int8_t ret = 0;
    uint8_t index;
#if defined(SOC_J721E) && !defined(DIAG_STRESS_TEST)
    uint8_t userInput;
#endif

    Board_STATUS status = BOARD_SOK;

#if defined (SOC_J784S4)
    Board_control(BOARD_CTRL_CMD_SET_IO_MUX_PORTB3, NULL);
#endif

    /* set board pin mux mode to MAIN domain */
#if defined(SOC_AM65XX)
    for(index = 0; index < MAIN_PADCONFIG_MAX_COUNT; index++)
    {
        Board_pinMuxSetMode(pinMuxgpio[index],
                            (GPIO_PADCONFIG_MUX_MODE | PIN_INPUT_ENABLE));
    }
#elif defined(SOC_J721E) || defined (SOC_J784S4)
    for(index = 0; index < gMainPadConfigMaxCount; index++)
    {
        status = Board_pinmuxSetReg(BOARD_SOC_DOMAIN_MAIN,
                                    pinMuxgpio[index],
                                    BOARD_GPIO_PIN_MUX_CFG);
        if (status != BOARD_SOK)
        {
            return status;
        }
    }

    /* set board pin mux mode to WAKEUP domain */
    for(index = gMainPadConfigMaxCount; index < gPadConfigMaxCount; index++)
    {
        status = Board_pinmuxSetReg(BOARD_SOC_DOMAIN_WKUP,
                                    pinMuxgpio[index],
                                    BOARD_GPIO_PIN_MUX_CFG);
        if (status != BOARD_SOK)
        {
            return status;
        }
    }
#elif defined(SOC_AM64X)
    for(index = 0; index < MAIN_PADCONFIG_MAX_COUNT; index++)
    {
        status = Board_pinmuxSetReg(BOARD_SOC_DOMAIN_MAIN,
                                    pinMuxgpio[index],
                                    BOARD_GPIO_PIN_MUX_CFG);
        if (status != BOARD_SOK)
        {
            return status;
        }
    }
    /* set board pin mux mode to WAKEUP domain */
    for(index = MAIN_PADCONFIG_MAX_COUNT; index < PADCONFIG_MAX_COUNT; index++)
    {
        status = Board_pinmuxSetReg(BOARD_SOC_DOMAIN_MCU,
                                    pinMuxgpio[index],
                                    BOARD_GPIO_PIN_MUX_CFG);
        if (status != BOARD_SOK)
        {
            return status;
        }
    }
#else/*j7200_evm*/
	/* set board pin mux mode to WAKEUP domain */
    for(index = 0; index < PADCONFIG_MAX_COUNT; index++)
    {
        status = Board_pinmuxSetReg(BOARD_SOC_DOMAIN_WKUP,
                                    pinMuxgpio[index],
                                    BOARD_GPIO_PIN_MUX_CFG);
        if (status != BOARD_SOK)
        {
            return status;
        }
    }
#endif

    UART_printf("\nRunning Test Header GPIO loopback test...\n");

    for(index = 0; index < NUM_PIN_SETS; index++)
    {
#if (defined(SOC_J721E) && (!defined(DIAG_STRESS_TEST)))
        if(index == 1)
        {
            UART_printf("\nSet SW3.8 to OFF for testing Motor control header \n\r");
            UART_printf("Press Enter after setting the switch\n\r");
            UART_scanFmt("%c", &userInput);
        }
#endif
        BoardDiag_modifyGpioBaseAddr(testHeaderPin[index].baseAddress);
        UART_printf("Verifying %s \n", testHeaderPin[index].headerName);
        ret = BoardDiag_runGpioTestHeaderVerification(index, GPIO_PIN_VAL_HIGH);
        if(ret != 0)
        {
            return ret;
        }

        /* Verifying the looping back the signal low */
        ret = BoardDiag_runGpioTestHeaderVerification(index, GPIO_PIN_VAL_LOW);
        if(ret != 0)
        {
            return ret;
        }
    }

    return ret;
}

int8_t BoardDiag_expHeaderFuctionalTest(void)
{
    int8_t ret = 0;
#if defined(SOC_J721E)
    bool isAlpha = BFALSE;
    /*
     * There is a Update in PRG1 MUX-01, PRG1_PWM pins are DNI in
     * GESI Beta HW revision.
     * PRG1_PWM pins are enabled only for Alpha GESI boards.
     */
    isAlpha = Board_isAlpha(BOARD_ID_GESI);
    if(BTRUE == isAlpha)
    {
        UART_printf("ALPHA GESI Board\n");
        gMainPadConfigMaxCount = MAIN_PADCONFIG_MAX_COUNT_ALPHA;
        testHeaderPin= testHeaderPinAlpha;
        gPadConfigMaxCount = PADCONFIG_MAX_COUNT_ALPHA;
        pinMuxgpio = pinMuxgpioAlpha;
    }
    else
    {
        UART_printf("BETA GESI Board\n");
        gMainPadConfigMaxCount = MAIN_PADCONFIG_MAX_COUNT_BETA;
        testHeaderPin= testHeaderPinBeta;
        gPadConfigMaxCount = PADCONFIG_MAX_COUNT_BETA;
        pinMuxgpio = pinMuxgpioBeta;
    }
#endif
    ret = BoardDiag_runExpHeaderTest();

    return ret;
}

#ifdef DIAG_STRESS_TEST
/**
 *  \brief    The function performs the expansion header Diagnostic
 *            stress test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_expHeaderStressTest(void)
{
    int8_t ret = 0;
    char rdBuf = 'y';
    uint32_t iteration;
    uint32_t passCount = 0;
    uint32_t failCount = 0;

    UART_printf("\n\nRunning GPIO Test header loopback Test in Stress Mode for %d Number of Times...\n", DIAG_STRESS_TEST_ITERATIONS);
    UART_printf("Enter 'b' in Serial Console to Terminate the Test\n\n");

    for(iteration = 1; iteration <= DIAG_STRESS_TEST_ITERATIONS; iteration++)
    {
        ret = BoardDiag_expHeaderFuctionalTest();
        if (ret == 0)
        {
            UART_printf("\n\n\n\nIteration : %d GPIO Test header loopback Test Passed\n",iteration);
            passCount++;
        }
        else
        {
            UART_printf("\n\n\n\nGPIO Test header loopback Test Failed at iteration - %d\n",iteration);
            failCount++;
        }

        /* Check if there a input from console to break the test */
        rdBuf = (char)BoardDiag_getUserInput(BOARD_UART_INSTANCE);
        if((rdBuf == 'b') || (rdBuf == 'B'))
        {
            UART_printf("Received Test Termination... Exiting the Test\n");
            iteration++;
            break;
        }
    }

    UART_printf("\n\nGPIO Test header loopback Stress Test Status\n");
    UART_printf("===================================\n");
    UART_printf("Number of Times Executed - %d\n", (iteration - 1));
    UART_printf("Pass Count - %d\n", passCount);
    UART_printf("Fail Count - %d\n", failCount);

    if((iteration != 0) && (failCount == 0))
    {
        UART_printf("Overall Status - PASS\n\n\n" );
    }
    else
    {
        UART_printf("Overall Status - FAIL\n\n\n" );
        ret = -1;
    }

    return ret;
}
#endif

#if (defined(SOC_J721E) || defined(SOC_AM64X))
static int16_t BoardDiag_ioExpMuxSel(uint8_t slaveAddr,
                                     i2cIoExpType_t ioExpType,
                                     i2cIoExpPortNumber_t portNum,
                                     i2cIoExpPinNumber_t pinNum,
                                     i2cIoExpSignalLevel_t signalLevel)
{
    int16_t ret;
    /* Setting the pin direction as output 	*/
    ret = Board_i2cIoExpSetPinDirection(slaveAddr,
                                        ioExpType,
                                        portNum,
                                        pinNum,
                                        PIN_DIRECTION_OUTPUT);
    if(ret == I2C_IO_STS_SUCCESS)
    {
        /* Pulling the IO exp pin line to low for the reset to happen */
        ret = Board_i2cIoExpPinLevelSet(slaveAddr,
                                        ioExpType,
                                        portNum,
                                        pinNum,
                                        signalLevel);
        if(ret != I2C_IO_STS_SUCCESS)
        {
            UART_printf("\nSetting signal level of i2c io exp pin %d failed!!!\n",pinNum);
            return ret;
        }
    }
    else
    {
        UART_printf("\nSetting pin %d direction of i2c io exp failed!!!\n",pinNum);
        return ret;
    }
    return 0;
}
#endif

#if defined(SOC_J721E)
/**
 *  \brief    The function performs MLB header verification test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_runMlbHeaderToggleTest(void)
{
    int8_t status = 0;
    uint8_t mlbIndex=0;
    uint8_t index=0;

    /* Set MLB_MUX_SEL pin to High to select MLB header pins */
    status=BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                 THREE_PORT_IOEXP,
                                 PORTNUM_1,
                                 PIN_NUM_3,
                                 GPIO_SIGNAL_LEVEL_HIGH);
    if (status != BOARD_SOK)
    {
        UART_printf("\nSetting MLB_MUX_SEL for MLB header Failed\n");
        return status;
    }

    BoardDiag_modifyGpioBaseAddr(CSL_GPIO0_BASE);
    /* set board pin mux mode to MAIN domain */
    for(index = 0; index < NUM_OF_MLB_HEADER_PINS; index++)
    {
        status = Board_pinmuxSetReg(BOARD_SOC_DOMAIN_MAIN,
                                    mlbHeader_pinMuxgpio[index],
                                    BOARD_GPIO_PIN_MUX_CFG);
        if (status != BOARD_SOK)
        {
            return status;
        }
    }

    UART_printf("\nMLB Header Pins \n31.MLB0_MLBCLK\n32.MLB0_MLBSIG\n33.MLB0_MLBDAT\n\n");
    for(mlbIndex=MLB_HEADER_PADCONFIG_INDEX; mlbIndex<(MLB_HEADER_PADCONFIG_INDEX + NUM_OF_MLB_HEADER_PINS); mlbIndex++)
    {
        UART_printf("Toggling pin %d\n",mlbIndex);
        for(index=0; index<10; index++)
        {
            GPIO_write(mlbIndex, GPIO_PIN_VAL_HIGH);
            UART_printf("GPIO High\n");
            BOARD_delay(2000000);
            GPIO_write(mlbIndex, GPIO_PIN_VAL_LOW);
            UART_printf("GPIO Low\n");
            BOARD_delay(2000000);
        }
    }
    return 0;
}
#endif

/**
 *  \brief   GPIO test header Diagnostic test main function
 *
 *  \return  int - GPIO test header Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 *
 */
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int8_t ret = 0;
#if (defined(SOC_J721E) || defined(SOC_AM64X))
    Board_I2cInitCfg_t i2cCfg;
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
    if(status != BOARD_SOK)
    {
        return -1;
    }

    UART_printf("\n**********************************************\n");
    UART_printf(  "        GPIO Test Header Loopback Test        \n");
    UART_printf(  "**********************************************\n");

    GPIO_init();
#if (defined(SOC_J721E) || defined(SOC_AM64X))
    i2cCfg.i2cInst   = I2C_INSTANCE;
    i2cCfg.socDomain = BOARD_SOC_DOMAIN_MAIN;
    Board_setI2cInitConfig(&i2cCfg);

    ret = Board_i2cIoExpInit();
    if(ret != I2C_IO_STS_SUCCESS)
    {
        UART_printf("\nI2C init failed!!!\n");
        return ret;
    }

#if defined(SOC_J721E) && !(defined(DIAG_STRESS_TEST))
    ret = BoardDiag_runMlbHeaderToggleTest();
    if(ret != 0)
    {
        UART_printf("\nMLB Header Pin Toggle test failed\n");
        return ret;
    }
#endif

    UART_printf("\nI2C IO-Expander Mux Configuration... \n\r");

#if defined(SOC_AM64X)
/* CPSW_FET_SEL - High - P12 - I2C1 - 0x22 */
    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                          THREE_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_2,
                          GPIO_SIGNAL_LEVEL_HIGH);

/* CPSW_FET2_SEL - High - P13 - I2C1 - 0x22 */
    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                          THREE_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_3,
                          GPIO_SIGNAL_LEVEL_HIGH);

/* CAN_MUX_SEL - High - P01 - I2C1 - 0x22 */
    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                          THREE_PORT_IOEXP,
                          PORTNUM_0,
                          PIN_NUM_1,
                          GPIO_SIGNAL_LEVEL_HIGH);

/* FSI_FET_SEL - Low - P07 - I2C1 - 0x22 */
    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                          THREE_PORT_IOEXP,
                          PORTNUM_0,
                          PIN_NUM_7,
                          GPIO_SIGNAL_LEVEL_LOW);
#elif defined(SOC_J721E)
    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                          TWO_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_4,
                          GPIO_SIGNAL_LEVEL_HIGH);

    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                          TWO_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_5,
                          GPIO_SIGNAL_LEVEL_HIGH);

    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                          TWO_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_6,
                          GPIO_SIGNAL_LEVEL_HIGH);

    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                          THREE_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_2,
                          GPIO_SIGNAL_LEVEL_LOW);

    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                          THREE_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_3,
                          GPIO_SIGNAL_LEVEL_LOW);

    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                          THREE_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_4,
                          GPIO_SIGNAL_LEVEL_HIGH);

    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                          THREE_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_5,
                          GPIO_SIGNAL_LEVEL_LOW);

    BoardDiag_ioExpMuxSel(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                          THREE_PORT_IOEXP,
                          PORTNUM_1,
                          PIN_NUM_6,
                          GPIO_SIGNAL_LEVEL_LOW);
#endif

    Board_i2cIoExpDeInit();
#endif

#if (defined(SOC_J721E))   /* PROFI_UART_SEL */
    Board_pinmuxSetReg(BOARD_SOC_DOMAIN_MAIN,
                       PIN_PRG0_PRU0_GPO5,
                       BOARD_GPIO_PIN_MUX_CFG);
    GPIO_write(0, GPIO_PIN_VAL_HIGH);
#endif

#ifdef DIAG_STRESS_TEST
    ret = BoardDiag_expHeaderStressTest();
#else
    ret = BoardDiag_expHeaderFuctionalTest();
#endif
    if(ret == 0)
    {
        UART_printf("\nGPIO Test header loopback test passed\n");
    }
    else
    {
        UART_printf("\nGPIO Test header loopback test failed\n");
    }

    boardCfg = BOARD_INIT_PINMUX_CONFIG;
    Board_init(boardCfg);

    return ret;
}
