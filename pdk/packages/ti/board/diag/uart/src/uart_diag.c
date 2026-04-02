/*
 * Copyright (c) 2015-2022, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>
#include <stdlib.h>
#include <stdint.h>
#if !defined(SOC_J721E)
#include <tistdtypes.h>
#endif

#include "board.h"
#include "board_cfg.h"

#define FIFO_SIZE     (64U)
#define BAUD_RATE_MAX (7384615U)

#if (defined(SOC_TPR12) || defined(SOC_AWR294X)) && defined (__TI_ARM_V7R4__)
#include <string.h>
static uint8_t gUartTestBuf[100] = "\nTesting UART print to console at 115.2k baud rate";
#endif

#if defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4)
#include "board_i2c_io_exp.h"
#include "diag_common_cfg.h"

#if defined(SOC_J7200) || defined(SOC_J784S4)
#include "board_control.h"
#endif

#if defined(SOC_AM65XX)
/* Function to enable SoC UART1 */
void BoardDiag_enableUART1Sel(void)
{
    Board_i2cIoExpInit();

    /* Setting the pin direction as output */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                  PORTNUM_1,
                                  PIN_NUM_5,
                                  PIN_DIRECTION_OUTPUT);

    /* Pulling the UART1 SEL line to low */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              PORTNUM_1,
                              PIN_NUM_5,
                              GPIO_SIGNAL_LEVEL_LOW);
}

/* Function to enable MCU UART selection */
void BoardDiag_enableMCUUARTSel(void)
{
    Board_i2cIoExpInit();

    /* Setting the pin direction as output */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                  PORTNUM_1,
                                  PIN_NUM_6,
                                  PIN_DIRECTION_OUTPUT);

#if defined(DIAG_COMPLIANCE_TEST)
    /* Pulling the MCU UART SEL line to low */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              PORTNUM_1,
                              PIN_NUM_6,
                              GPIO_SIGNAL_LEVEL_LOW);
#else
    /* Pulling the MCU UART SEL line to High */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              PORTNUM_1,
                              PIN_NUM_6,
                              GPIO_SIGNAL_LEVEL_HIGH);
#endif
}

/* Function to enable Wakeup UART selection */
void BoardDiag_enableWkUPUARTSel(void)
{
    Board_i2cIoExpInit();

    /* Setting the pin direction as output */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                  PORTNUM_1,
                                  PIN_NUM_4,
                                  PIN_DIRECTION_OUTPUT);

#if defined(DIAG_COMPLIANCE_TEST)
    /* Pulling the MCU UART SEL line to low */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              PORTNUM_1,
                              PIN_NUM_4,
                              GPIO_SIGNAL_LEVEL_LOW);
#else
    /* Pulling the MCU UART SEL line to low */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              PORTNUM_1,
                              PIN_NUM_4,
                              GPIO_SIGNAL_LEVEL_HIGH);
#endif
}
#endif  /* #if defined(SOC_AM65XX) */

#if defined(SOC_J721E)
#if !defined (__aarch64__)
/**
 * \brief  MAIN UART4 Enable function
 *
 *  This function is enables MAIN UART4.
 *
 */
/* Function to enable MAIN4 UART */
void BoardDiag_enableMAIN4UART(void)
{
    UART_HwAttrs uartCfg;

    UART_socGetInitCfg(4, &uartCfg);

    uartCfg.baseAddr = CSL_UART4_BASE;
    uartCfg.enableInterrupt = FALSE;
    uartCfg.frequency = 48000000;

    UART_socSetInitCfg(4, &uartCfg);
}
#endif
#endif

#if defined(SOC_J721E)
#if !defined (__aarch64__)
/**
 * \brief  MAIN UART2 Enable function
 *
 *  This function is enables MAIN UART2.
 *
 */
/* Function to enable MAIN2 UART */
void BoardDiag_enableMAIN2UART(void)
{
    UART_HwAttrs uartCfg;

    UART_socGetInitCfg(2, &uartCfg);

    uartCfg.baseAddr = CSL_UART2_BASE;
    uartCfg.enableInterrupt = FALSE;
    uartCfg.frequency = 48000000;

    UART_socSetInitCfg(2, &uartCfg);
}
#endif
#endif

#if defined(SOC_AM65XX) || defined(SOC_J721E)
#if !defined (__aarch64__)
/* Function to enable MAIN1 UART */
void BoardDiag_enableMAIN1UART(void)
{
    UART_HwAttrs uartCfg;

    UART_socGetInitCfg(1, &uartCfg);

    uartCfg.baseAddr = CSL_UART1_BASE;
    uartCfg.enableInterrupt = FALSE;
    uartCfg.frequency = 48000000;

    UART_socSetInitCfg(1, &uartCfg);
}
#endif
#endif

#if defined(SOC_AM65XX) || defined(SOC_J721E)
#if !defined (__aarch64__)
/* Function to enable MAIN0 UART */
void BoardDiag_enableMAIN0UART(void)
{
    UART_HwAttrs uartCfg;

    UART_socGetInitCfg(0, &uartCfg);

    uartCfg.baseAddr = CSL_UART0_BASE;
    uartCfg.enableInterrupt = FALSE;
    uartCfg.frequency = 48000000;

    UART_socSetInitCfg(0, &uartCfg);
}
#endif
#endif

/* Function to enable MCU UART */
void BoardDiag_enableMCUUART(void)
{
    UART_HwAttrs uartCfg;
    
    UART_socGetInitCfg(0, &uartCfg);
    
    uartCfg.baseAddr = CSL_MCU_UART0_BASE;
    uartCfg.enableInterrupt = FALSE;
    uartCfg.frequency = 96000000;
    
    UART_socSetInitCfg(0, &uartCfg);
}

#if !defined(SOC_AM64X)
/* Function to enable wakeup UART */
void BoardDiag_enableWKUPUART(void)
{
    UART_HwAttrs uartCfg;
    
    UART_socGetInitCfg(0, &uartCfg);
    
    uartCfg.baseAddr = CSL_WKUP_UART0_BASE;
    uartCfg.enableInterrupt = FALSE;
    uartCfg.frequency = 96000000;
    
    UART_socSetInitCfg(0, &uartCfg);
}
#endif

/* Function to enable MAIN UART */
void BoardDiag_enableMAINUART(void)
{
    UART_HwAttrs uartCfg;

    UART_socGetInitCfg(0, &uartCfg);

    uartCfg.baseAddr = CSL_UART0_BASE;
    uartCfg.enableInterrupt = FALSE;
    uartCfg.frequency = 48000000;

    UART_socSetInitCfg(0, &uartCfg);
}

#if defined(SOC_J721E)
/**
 * \brief  MAIN UART4 Path Enable function
 *
 *  This function is configures the MUX to enable path for UART4.
 *
 */
void BoardDiag_enableMAINUART4Sel(void)
{
    Board_I2cInitCfg_t i2cCfg;

    i2cCfg.i2cInst   = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    i2cCfg.socDomain = BOARD_SOC_DOMAIN_MAIN;
    i2cCfg.enableIntr = false;
    Board_setI2cInitConfig(&i2cCfg);

    Board_i2cIoExpInit();

    /* Setting the pin direction as output */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                  THREE_PORT_IOEXP,
                                  PORTNUM_1,
                                  PIN_NUM_1,
                                  PIN_DIRECTION_OUTPUT);

    /* Pulling the MCASP/TRACE_MUX_S0 line to low */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              THREE_PORT_IOEXP,
                              PORTNUM_1,
                              PIN_NUM_1,
                              GPIO_SIGNAL_LEVEL_LOW);

    /* Setting the pin direction as output */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                  THREE_PORT_IOEXP,
                                  PORTNUM_1,
                                  PIN_NUM_2,
                                  PIN_DIRECTION_OUTPUT);

    /* Pulling the MCASP/TRACE_MUX_S1 line to high */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              THREE_PORT_IOEXP,
                              PORTNUM_1,
                              PIN_NUM_2,
                              GPIO_SIGNAL_LEVEL_HIGH);

    Board_i2cIoExpDeInit();
}
#endif  /* #if defined(SOC_J721E) */
#endif  /* #if defined(SOC_AM65XX) */

#if defined (DIAG_COMPLIANCE_TEST)

/* Functions to verify MCU and Wake-up UART headers in loopback 
   mode on AM65xx platform */
int8_t BoardDiag_runUartLoopbackTest(void)
{
    uint8_t txBuf[FIFO_SIZE];
    uint8_t rxBuf[FIFO_SIZE];
    uint8_t index,ret=0;

    UART_Handle uart_handle = NULL;
    UART_Params params;

    BoardDiag_genPattern(&txBuf[0], FIFO_SIZE, 
    BOARD_DIAG_TEST_PATTERN_AA);
    BoardDiag_genPattern(&rxBuf[0], FIFO_SIZE, 
    BOARD_DIAG_TEST_PATTERN_NULL);

    UART_init();
    UART_Params_init(&params);
    params.baudRate = BAUD_RATE_MAX;

    uart_handle = UART_open(BOARD_UART_INSTANCE, &params);
    if(uart_handle == NULL)
    {
        return -1;
    }

    ret = UART_write(uart_handle, (uint8_t *)&txBuf[0], FIFO_SIZE);
    if(!ret)
    {
        UART_close(uart_handle);
        return -1;
    }

    ret = UART_read(uart_handle,(uint8_t *)&rxBuf[0], FIFO_SIZE);
    if(ret != FIFO_SIZE)
    {
        UART_close(uart_handle);
        return -1;
    }

    UART_close(uart_handle);
    for(index = 0; index < FIFO_SIZE; index++)
    {
        if(txBuf[index] != rxBuf[index])
            return -1;
    }

    return 0;
}

int8_t BoardDiag_uartLoopbackTest(void)
{
    /* Verifying MCU UART in loopBack mode */
    BoardDiag_enableMCUUARTSel();
    BoardDiag_enableMCUUART();

    if(BoardDiag_runUartLoopbackTest())
    {
        return -1;
    }

    /* Verifying WKUP UART in loopBack mode */
    BoardDiag_enableWkUPUARTSel();
    BoardDiag_enableWKUPUART();

    if(BoardDiag_runUartLoopbackTest())
    {
        return -1;
    }

    return 0;
}
#endif

#if defined(j7200_evm)
/**
 * \brief  UART Mux enable function
 *
 * This function is used to enable the uart mux selection
 *
 * \param  pinNum    -  I2C IO EXPANDER pin number
 * \param  pinVal    -  Pin output to be set
 *
 */
void BoardDiag_uartMuxEnable(i2cIoExpPinNumber_t pinNum,
                             i2cIoExpSignalLevel_t pinVal)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status = BOARD_SOK;

    /* Enable the UART1 and UART3 */
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = pinNum;
    ioExpCfg.signalLevel = pinVal;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status != BOARD_SOK)
    {
        UART_printf("Failed to select the Uart Mux\n");
    }
}
#endif

int uart_test()
{
#if !defined(DIAG_COMPLIANCE_TEST)
    char p = 'r';
#endif

	UART_printf("\n*********************************************\n"); 
	UART_printf  ("*                 UART Test                 *\n");
	UART_printf  ("*********************************************\n");
#if !defined(DIAG_COMPLIANCE_TEST)
	UART_printf("\nTesting UART print to console at 115.2k baud rate\n");
	UART_printf("Press 'y' to verify pass: ");

	UART_scanFmt("%c", &p);

	UART_printf("Received: %c\n", p);
	if ( (p == 'y') || (p == 'Y') ) {
		UART_printf("\nTest PASSED!\n");
		return 0;
	}
	else {
		UART_printf("\nTest FAILED!\n");
		return -1;
	}
#else
    UART_printf("\nTest PASSED!\n");
    return 0;
#endif
}

#ifndef SPI_BOOT_FRAMEWORK
#if defined(SOC_AM65XX)
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int ret;

    boardCfg = BOARD_INIT_PINMUX_CONFIG;

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

#if !defined (__aarch64__)
    /* MCU I2C instance will be active by default for R5 core.
     * Need update HW attrs to enable MAIN I2C instance.
     */
    enableMAINI2C(0, CSL_I2C0_CFG_BASE);
#endif

#if defined(am65xx_evm)
    BoardDiag_enableUART1Sel();
#endif

    /* Verify the SoC UART0 */
#if !defined (__aarch64__)
    BoardDiag_enableMAIN0UART();
#endif

    UART_stdioInit(BOARD_UART_INSTANCE);
    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART0 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART0 Test Completed!\n");

#if !defined(DIAG_COMPLIANCE_TEST)
    UART_printf("\nStarting MCU UART Test...\n");
    UART_printf("\nCheck MCU UART console for the test logs\n");
    /* Close the UART instance for SoC UART0 */
    UART_stdioDeInit();

    /* changing to MAIN */
    BoardDiag_enableMCUUARTSel();
    BoardDiag_enableMCUUART();
    /* Verify the MCU UART */
    UART_stdioInit(BOARD_UART_INSTANCE);
    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nMCU UART Test Failed!!\n");
        return ret;
    }

    UART_printf("\nMCU UART Test Completed!\n");

    UART_printf("\nStarting Wakeup UART Test...\n");
    UART_printf("\nCheck Wakeup UART console for the test logs\n");
    /* Close the UART instance for MCU UART */
    UART_stdioDeInit();

    BoardDiag_enableWkUPUARTSel();

    BoardDiag_enableWKUPUART();
    /* Verify the Wakeup UART */
    UART_stdioInit(BOARD_UART_INSTANCE);
    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nWakeup UART Test Failed!!\n");
        return ret;
    }

    UART_printf("\nWakeup UART Test Completed!\n");
#else
    UART_printf("Testing MCU and WKUP test headers in loopBack mode...\n\n");
    /* Close the UART instance for SoC UART0 */
    UART_stdioDeInit();
    ret = BoardDiag_uartLoopbackTest();
    BoardDiag_enableMAINUART();
    UART_stdioInit(BOARD_UART_INSTANCE);

    if(ret != 0)
    {
        UART_printf("uart test header verification failed\n");
        return -1;
    }
    UART_printf("uart test header verification passed\n");
#endif

#if defined(am65xx_evm)
    UART_printf("\nStarting SoC UART1 Test...\n");
    UART_printf("\nCheck SoC UART1 console for the test logs\n");
    /* Close the UART instance for Wakeup UART */
    UART_stdioDeInit();
    
    /* Verify the SoC UART1 */
#if !defined (__aarch64__)
    BoardDiag_enableMAIN1UART();
#endif

    UART_stdioInit(BOARD_UART1_INSTANCE);
    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART1 Test Failed!!\n");
    }

    UART_printf("\nSoC UART1 Test Completed!\n");
#endif

    UART_stdioDeInit();
#if defined (__aarch64__)
    BoardDiag_enableMAINUART();
#else
    BoardDiag_enableMCUUART();
#endif
    UART_stdioInit(BOARD_UART_INSTANCE);
    UART_printf("\nUART Test Completed!!\n");

    return ret;
}
#elif (defined(SOC_J721E) || defined(SOC_J7200))
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    Board_initParams_t initParams;
    int ret;

    boardCfg = BOARD_INIT_PINMUX_CONFIG;

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    /* Verify the SoC UART0 */
    Board_getInitParams(&initParams);
    initParams.uartInst = 0;
    initParams.uartSocDomain = BOARD_SOC_DOMAIN_MAIN;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART0 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART0 Test Completed!\n");

    UART_printf("\nStarting SoC UART1 Test...\n");
    UART_printf("\nCheck SoC UART1 console for the test logs\n");
    /* Close the UART instance for SoC UART0 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

#if defined(j7200_evm)
/* Enable the mux to select UART1 and UART3 */
    BoardDiag_uartMuxEnable(PIN_NUM_4, GPIO_SIGNAL_LEVEL_LOW);
    BoardDiag_uartMuxEnable(PIN_NUM_3, GPIO_SIGNAL_LEVEL_HIGH);
    BoardDiag_uartMuxEnable(PIN_NUM_2, GPIO_SIGNAL_LEVEL_HIGH);
#endif
    initParams.uartInst = 1;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART1 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART1 Test Completed!\n");
#if defined (j7200_evm)
    UART_printf("\nStarting SoC UART3 Test...\n");
    UART_printf("\nCheck SoC UART3 console for the test logs\n");
    /* Close the UART instance for SoC UART1 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = BOARD_UART3_INSTANCE;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART3 Test Failed!!\n");
        return ret;
    }
    UART_printf("\nSoC UART3 Test Completed!\n");
#endif

#if defined (j721e_evm)
    UART_printf("\nStarting SoC UART2 Test...\n");
    UART_printf("\nCheck SoC UART2 console for the test logs\n");
    /* Close the UART instance for SoC UART1 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = 2;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART2 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART2 Test Completed!\n");

    UART_printf("\nStarting SoC UART4 Test...\n");
    UART_printf("\nCheck SoC UART4 console for the test logs\n");
    /* Close the UART instance for SoC UART2 or SoC UART0 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = 4;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART4 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART4 Test Completed!\n");
#endif

    UART_printf("\nStarting MCU UART Test...\n");
    UART_printf("\nCheck MCU UART console for the test logs\n");
    /* Close the UART instance for SoC UART0 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = 0;
    initParams.uartSocDomain = BOARD_SOC_DOMAIN_MCU;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nMCU UART Test Failed!!\n");
        return ret;
    }

    UART_printf("\nMCU UART Test Completed!\n");

    UART_printf("\nStarting Wakeup UART Test...\n");
    UART_printf("\nCheck Wakeup UART console for the test logs\n");
    /* Close the UART instance for MCU UART */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartSocDomain = BOARD_SOC_DOMAIN_WKUP;
    Board_setInitParams(&initParams);

    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nWakeup UART Test Failed!!\n");
        return ret;
    }

    UART_printf("\nWakeup UART Test Completed!\n");

    Board_deinit(BOARD_DEINIT_UART_STDIO);
    BoardDiag_enableMAINUART();
    UART_stdioInit(BOARD_UART_INSTANCE);
    UART_printf("\nUART Test Completed!!\n");

    return ret;
}
#elif (defined(SOC_J721S2) || defined(SOC_J784S4))
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    Board_initParams_t initParams;
    int ret;

    boardCfg = BOARD_INIT_PINMUX_CONFIG;

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    /* Verify the SoC UART0 */
    Board_getInitParams(&initParams);
    initParams.uartInst = BOARD_UART_INSTANCE;
    initParams.uartSocDomain = BOARD_SOC_DOMAIN_MAIN;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART8 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART8 Test Completed!\n");

    UART_printf("\nStarting SoC UART5 Test...\n");
    UART_printf("\nCheck SoC UART5 console for the test logs\n");
    /* Close the UART instance for SoC UART8 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = BOARD_UART5_INSTANCE;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART5 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART5 Test Completed!\n");

    UART_printf("\nStarting SoC UART2 Test...\n");
    UART_printf("\nCheck SoC UART2 console for the test logs\n");
    /* Close the UART instance for SoC UART1 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = BOARD_UART2_INSTANCE;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART2 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART2 Test Completed!\n");

#if defined(j784s4_evm)
    UART_printf("\nStarting SoC UART3 Test...\n");
    UART_printf("\nCheck SoC UART3 console for the test logs\n");

    /* Close the UART instance for SoC UART1 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    /* UART Mux enable for instance 3 */
    Board_control(BOARD_CTRL_CMD_SET_IO_MUX_PORTB2, NULL);

    Board_IoExpCfg_t ioExpCfg;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = TWO_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_1;
    ioExpCfg.pinNum      = PIN_NUM_4;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status != BOARD_SOK)
    {
        UART_printf("Failed to enable the I2C mux selection\n");
    }



    initParams.uartInst = BOARD_UART3_INSTANCE;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART3 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART3 Test Completed!\n");

#endif

    UART_printf("\nStarting MCU UART Test...\n");
    UART_printf("\nCheck MCU UART console for the test logs\n");
    /* Close the UART instance for SoC UART0 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = 0;
    initParams.uartSocDomain = BOARD_SOC_DOMAIN_MCU;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nMCU UART Test Failed!!\n");
        return ret;
    }

    UART_printf("\nMCU UART Test Completed!\n");

    UART_printf("\nStarting Wakeup UART Test...\n");
    UART_printf("\nCheck Wakeup UART console for the test logs\n");
    /* Close the UART instance for MCU UART */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartSocDomain = BOARD_SOC_DOMAIN_WKUP;
    Board_setInitParams(&initParams);

    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nWakeup UART Test Failed!!\n");
        return ret;
    }

    UART_printf("\nWakeup UART Test Completed!\n");

    Board_deinit(BOARD_DEINIT_UART_STDIO);
    BoardDiag_enableMAINUART();
    UART_stdioInit(BOARD_UART_INSTANCE);
    UART_printf("\nUART Test Completed!!\n");

    return ret;
}
#elif defined(SOC_AM64X)
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    Board_initParams_t initParams;
    int ret;

    boardCfg = BOARD_INIT_PINMUX_CONFIG;

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    /* Verify the SoC UART0 */
    Board_getInitParams(&initParams);
    initParams.uartInst = BOARD_UART0_INSTANCE;
    initParams.uartSocDomain = BOARD_SOC_DOMAIN_MAIN;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART0 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART0 Test Completed!\n");

    UART_printf("\nStarting SoC UART1 Test...\n");
    UART_printf("\nCheck SoC UART1 console for the test logs\n");
    /* Close the UART instance for SoC UART0 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = BOARD_UART1_INSTANCE;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART1 Test Failed!!\n");
        return ret;
    }

    UART_printf("\nSoC UART1 Test Completed!\n");

    UART_printf("\nStarting SoC UART3 Test...\n");
    UART_printf("\nCheck SoC UART3 console for the test logs\n");
    /* Close the UART instance for SoC UART1 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = BOARD_UART3_INSTANCE;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nSoC UART3 Test Failed!!\n");
        return ret;
    }
    UART_printf("\nSoC UART3 Test Completed!\n");

    UART_printf("\nStarting MCU UART Test...\n");
    UART_printf("\nCheck MCU UART console for the test logs\n");
    /* Close the UART instance for SoC UART0 */
    Board_deinit(BOARD_DEINIT_UART_STDIO);

    initParams.uartInst = BOARD_MCU_UART0_INSTANCE;
    initParams.uartSocDomain = BOARD_SOC_DOMAIN_MCU;
    Board_setInitParams(&initParams);
    status = Board_init(BOARD_INIT_UART_STDIO);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nMCU UART Test Failed!!\n");
        return ret;
    }

    UART_printf("\nMCU UART Test Completed!\n");

    Board_deinit(BOARD_DEINIT_UART_STDIO);
    BoardDiag_enableMAINUART();
    UART_stdioInit(BOARD_UART_INSTANCE);
    UART_printf("\nUART Test Completed!!\n");

    return ret;
}
#elif (defined(SOC_TPR12) || defined(SOC_AWR294X)) && defined (__TI_ARM_V7R4__)
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int ret;
    UART_Handle uart_handle = NULL;
    UART_Params params;
    char p = 'r';

    boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = uart_test();
    if(ret != 0)
    {
        UART_printf("\nMSS UARTA Test Failed!!\n");
        return ret;
    }

    UART_printf("\nMSS UARTA Test Completed!\n");

    UART_printf("\n\nStarting MSS UARTB Test...\n");
    UART_printf("\nCheck the Logs on MSS UARTB console\n");
    UART_printf("Press 'y' if Test Message on MSS UARTB is Proper, else Press Any Other Key: ");

    UART_Params_init(&params);

    uart_handle = UART_open(BOARD_MSS_UARTB_INSTANCE, &params);
    if(uart_handle == NULL)
    {
        return -1;
    }

    ret = UART_write(uart_handle, (uint8_t *)&gUartTestBuf[0], strlen((const char *)gUartTestBuf));
    if(!ret)
    {
        UART_close(uart_handle);
        return -1;
    }

	UART_scanFmt("%c", &p);

	UART_printf("Received: %c\n", p);
	if ( (p == 'y') || (p == 'Y') ) {
		UART_printf("\nMSS UARTB Test PASSED!\n");
	}
	else {
		UART_printf("\nMSS UARTB Test FAILED!\n");
		return -1;
	}

    UART_printf("\n\nUART Test Completed!!\n");

    UART_close(uart_handle);

    return ret;
}
#else
int main(void)
{
    Board_initCfg boardCfg;

#ifdef SOC_K2G
    DIAG_IntrInit();
#endif

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_PINMUX_CONFIG | 
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);
    return uart_test();
}
#endif  /* #if defined(SOC_AM65XX) */
#endif /* #ifndef SPI_BOOT_FRAMEWORK */
