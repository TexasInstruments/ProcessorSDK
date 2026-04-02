/******************************************************************************
 * Copyright (c) 2019-2020 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   hyperbus_test.c
 *
 *  \brief  hyperbus Example main file
 *
 *  Targeted Functionality: This file implements hyperbus test.
 *
 *  Operation: Verification of hyperbus by accessing the device connected to it.
 *
 *  Supported SoCs : J721E J7200
 *
 *  Supported Platforms: j721e_evm j7200_evm
 *
 */

#include "hyperbus_test.h"
#include <ti/drv/sciclient/sciclient.h> /* Required override default clk cfg */

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* Unity test functions */
void test_csl_hyperbus_app(void);
void test_csl_hyperbus_app_runner(void);

#ifdef UNITY_INCLUDE_CONFIG_H
/*
 *  ======== Unity set up and tear down ========
 */
void setUp(void)
{
    /* Do nothing */
}

void tearDown(void)
{
    /* Do nothing */
}
#endif

/**
 * \brief  Generate test pattern function
 *
 * This function fills the data buffer passed as input parameter with
 * different test patterns as indicated by 'flag' parameter.
 * Below are the different test patterns supported
 *
 * APP_TEST_PATTERN_FF     - Fills the buffer with 0xFF
 * APP_TEST_PATTERN_AA     - Fills the buffer with 0xAA
 * APP_TEST_PATTERN_55     - Fills the buffer with 0x55
 * APP_TEST_PATTERN_NULL   - Fills buffer with 0
 * APP_TEST_PATTERN_RANDOM - Fills the buffer with randon pattern
 * APP_TEST_PATTERN_INC    - Fills the buffer with increment pattern
 *                                  starting from 0
 * APP_TEST_PATTERN_AA_55  - Fills the buffer with 0xAA in even locations
 *                                  0x55 in odd locations
 *
 * If 'flag' does match with any of the above values, buffer will be filled
 * with 0 by default.
 *
 * \param   buf    [IN]   Buffer to fill with the test pattern
 * \param   length [IN]   Length of the buffer
 * \param   flag   [IN]   Flag to set the test pattern
 *
 */
void App_genPattern(uint8_t *buf, uint32_t length, uint8_t flag)
{
    uint8_t data         = 0;
    uint8_t incFlag      = 0;
    uint8_t randFlag     = 0;
    uint8_t checkerBoard = 0;
    uint32_t idx;

    switch(flag)
    {
        case APP_TEST_PATTERN_FF:
        case APP_TEST_PATTERN_AA:
        case APP_TEST_PATTERN_55:
        case APP_TEST_PATTERN_NULL:
             data = flag;
             break;
        case APP_TEST_PATTERN_INC:
             data    = 0;
             incFlag = 1;
             break;
        case APP_TEST_PATTERN_RANDOM:
             data     = rand();
             randFlag = 1;
        case APP_TEST_PATTERN_AA_55:
             data         = 0xAA;
             checkerBoard = 1;
             break;
        default:
             data = flag;
             break;
    }

    for(idx = 0; idx < length; idx++)
    {
        buf[idx] = data;

        if(incFlag)
            data++;

        if(randFlag)
            data = rand();

        if(checkerBoard)
            data = ~data;
    }
}

/**
 * \brief  Compares two data buffers
 *
 * This function verifies the data in two buffers passed as parameters and
 * returns the result. 'failIndex' contains the index of the first mismatch
 * when buffer comparision fails.
 *
 * \param   buf1      [IN]   Data buffer 1
 * \param   buf2      [IN]   Data buffer 2
 * \param   length    [IN]   Length of the buffers
 * \param   failIndex [OUT]  Index of first data mismatch
 *
 * \return
 *          1  - In case of success
 *          0  - In case of failure
 *
 */
bool App_memCompare(uint8_t *buf1, uint8_t *buf2, uint32_t length,
                          uint32_t *failIndex)
{
    uint32_t idx = 0;
    bool match   = 1;

    for(idx = 0; ((idx < length) && (match == 1)); idx++)
    {
        if(buf1[idx] != buf2[idx])
        {
            match      = 0;
            *failIndex = idx;
        }
    }

    return (match);
}

/**
 * \brief   This function writes data, reads back the written data and compares.
 *
 * \param   offset      [IN]    Offset Address to write
 * \param   numOfBytes  [IN]    Number of bytes to write.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t App_hpbReadWriteTest(Board_flashHandle handle,
                                         uint32_t offset,
                                         uint32_t numOfBytes)
{
    Board_flash_STATUS retVal;
    uint32_t blockNum, pageNum;
    uint32_t failIndex;
    static uint8_t hypFlsTxBuf[APP_HPB_TEST_BYTES];
    static uint8_t hypFlsRxBuf[APP_HPB_TEST_BYTES];
    volatile uint64_t   preTimeStamp, postTimeStamp;

    retVal = Board_flashOffsetToBlkPage(handle, offset, &blockNum, &pageNum);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashOffsetToBlkPage failed\n");
        return -1;
    }

    /* Erase block, to which data has to be written */
    preTimeStamp = TimerP_getTimeInUsecs();
    retVal = Board_flashEraseBlk(handle, blockNum);
    postTimeStamp = TimerP_getTimeInUsecs();
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashEraseBlk failed\n");
        return -1;
    }
    UART_printf("\n [Erase] %d:%d []  %d:%d ", ((uint32_t)preTimeStamp >> 31), (uint32_t)preTimeStamp, ((uint32_t)postTimeStamp >> 31), (uint32_t)postTimeStamp);

    App_genPattern(&hypFlsTxBuf[0], numOfBytes,
                         APP_TEST_PATTERN_RANDOM);

    /* Write buffer to hyperflash */
    preTimeStamp = TimerP_getTimeInUsecs();
    retVal = Board_flashWrite(handle, offset,
                              &hypFlsTxBuf[0], numOfBytes, NULL);
    postTimeStamp = TimerP_getTimeInUsecs();
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashWrite failed\n");
        return -1;
    }
    UART_printf("\n [Write] %d:%d []  %d:%d ", ((uint32_t)preTimeStamp >> 31), (uint32_t)preTimeStamp, ((uint32_t)postTimeStamp >> 31), (uint32_t)postTimeStamp);

    /* Read the hyperflash memory */
    preTimeStamp = TimerP_getTimeInUsecs();
    retVal = Board_flashRead(handle, offset,
                             &hypFlsRxBuf[0], numOfBytes, NULL);
    postTimeStamp = TimerP_getTimeInUsecs();
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashRead failed\n");
        return -1;
    }
    UART_printf("\n [Read] %d:%d []  %d:%d ", ((uint32_t)preTimeStamp >> 31), (uint32_t)preTimeStamp, ((uint32_t)postTimeStamp >> 31), (uint32_t)postTimeStamp);

    if (App_memCompare(&hypFlsTxBuf[0], &hypFlsRxBuf[0], numOfBytes, &failIndex)
                                                                    == false)
    {
        UART_printf("\nINFO : TX DATA : %d, %d, %d, %d, %d, %d, %d, %d",
                    hypFlsTxBuf[0], hypFlsTxBuf[1], hypFlsTxBuf[2], hypFlsTxBuf[3],
                    hypFlsTxBuf[4], hypFlsTxBuf[5], hypFlsTxBuf[6], hypFlsTxBuf[7]);

        UART_printf("\nINFO : TX DATA : %d, %d, %d, %d, %d, %d, %d, %d",
                    hypFlsTxBuf[504], hypFlsTxBuf[505], hypFlsTxBuf[506], hypFlsTxBuf[507],
                    hypFlsTxBuf[508], hypFlsTxBuf[509], hypFlsTxBuf[510], hypFlsTxBuf[511]);
        UART_printf("\nINFO : RX DATA : %d, %d, %d, %d, %d, %d, %d, %d",
                    hypFlsRxBuf[0], hypFlsRxBuf[1], hypFlsRxBuf[2], hypFlsRxBuf[3],
                    hypFlsRxBuf[4], hypFlsRxBuf[5], hypFlsRxBuf[6], hypFlsRxBuf[7]);
        UART_printf("\nINFO : RX DATA : %d, %d, %d, %d, %d, %d, %d, %d",
                    hypFlsRxBuf[504], hypFlsRxBuf[505], hypFlsRxBuf[506], hypFlsRxBuf[507],
                    hypFlsRxBuf[508], hypFlsRxBuf[509], hypFlsRxBuf[510], hypFlsRxBuf[511]);
        UART_printf("\nData mismatch at offset = 0x%x\n", failIndex);
        return -1;
    }

    return 0;
}

/**
 * \brief   This function runs Hyper Flash boundary verification test.
 *
 * \param   chipSel     [IN]    Chip select number of HyperFlash
 * \param   baseAddr    [IN]    Base Address of HyperFlash
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t App_hpbRunHyperFlashTest()
{
    Board_flashHandle boardHandle;
    Board_FlashInfo *flashInfo;
    int8_t ret;

    boardHandle = Board_flashOpen(BOARD_FLASH_ID_S71KS512S,
                                  BOARD_HPF_INSTANCE, NULL);
    if (!boardHandle)
    {
        UART_printf("Board_flashOpen Failed\n");
        return -1;
    }
    else
    {
        flashInfo = (Board_FlashInfo *)boardHandle;
        UART_printf("\nHyperFlash Device ID: 0x%x, Manufacturer ID: 0x%x ",
                            flashInfo->device_id, flashInfo->manufacturer_id);
    }

    UART_printf("\n\nVerifying First 512 bytes...");
    ret = App_hpbReadWriteTest(boardHandle,
                                     APP_HPB_FIRST_VERIFY_ADDR,
                                     APP_HPB_TEST_BYTES);
    if (ret != 0)
    {
        UART_printf("Verifying first 512 bytes failed\n");
        Board_flashClose(boardHandle);
        return -1;
    }
    UART_printf("\nVerification of first 512 bytes was successful");

    UART_printf("\n\nVerifying Last 512 bytes...");
    ret = App_hpbReadWriteTest(boardHandle,
                                     APP_HPB_LAST_VERIFY_ADDR,
                                     APP_HPB_TEST_BYTES);
    if (ret != 0)
    {
        UART_printf("Verifying last 512 bytes failed\n");
        Board_flashClose(boardHandle);
        return -1;
    }
    UART_printf("\nVerification of last 512 bytes was successful\n");
    Board_flashClose(boardHandle);

    return 0;
}

/**
 * \brief   The function runs the Hyperbus test.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t App_HyperBusTest(void)
{
    int8_t ret;

    ret = App_hpbRunHyperFlashTest();
    if (ret)
    {
        UART_printf("\nHyper Flash Test Failed!!\n");
    }
    else
    {
        UART_printf("\nHyper Flash Test Passed!");
        UART_printf("\nAll Tests Passed \n");
    }

    return ret;
}

static void App_ospiHyperFlashMux(void)
{
#if defined (SOC_J7200) || defined (SOC_J721E)

    uint32_t gpioPinNum = 8U;
    #if defined (SOC_J7200)
        gpioPinNum = 6U;
    #endif

    UART_printf("\nINFO : Selecting Hyperbus...");
    GPIOSetDirMode_v0(CSL_WKUP_GPIO0_BASE, gpioPinNum, GPIO_DIRECTION_OUTPUT);

    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, gpioPinNum, GPIO_PIN_HIGH);

    if (GPIO_PIN_HIGH == GPIOPinRead_v0(CSL_WKUP_GPIO0_BASE, gpioPinNum))
    {
        UART_printf("\nINFO : CSL_WKUP_GPIO0_BASE PIN %d is GPIO_PIN_HIGH", gpioPinNum);
    }
    else
    {
        UART_printf("\nINFO : CSL_WKUP_GPIO0_BASE PIN %d is GPIO_PIN_HIGH", gpioPinNum);
    }
    UART_printf("\nINFO : Selecting Hyperbus... Done \n");
#endif
}

/* TX Clocks reconfigured */
static Board_STATUS App_ospiHyperFlashClkCfg(void)
{
#if defined (SOC_J721E)
    Board_STATUS status = BOARD_SOK;
    uint32_t siliconRev = 0U;
    uint32_t clkVal = 333333333;

    /* On J721E ES 1.1, hyperbus clock can operate at 166 MHz */
    siliconRev = CSL_REG32_FEXT((CSL_WKUP_CTRL_MMR0_CFG0_BASE +
                                 CSL_WKUP_CTRL_MMR_CFG0_JTAGID),
                                 WKUP_CTRL_MMR_CFG0_JTAGID_VARIANT);
    if (0U != siliconRev)
    {
        UART_printf("\nINFO : Re Configuring Hyperbus clock...");
        status = Board_PLLInit(TISCI_DEV_MCU_FSS0_HYPERBUS1P0_0,
                               TISCI_DEV_MCU_FSS0_HYPERBUS1P0_0_HPB_CLKX2_CLK,
                               clkVal);
        if (BOARD_SOK == status)
        {
            UART_printf("\nINFO : Re Configuring Hyperbus... Done \n");
            UART_printf("\nINFO : Configured to %d Hz \n", (clkVal)/2U);
        }
        else
        {
            UART_printf("\nINFO : Re Configuring Hyperbus... Failed!! \n");
        }
    }

    return status;
#else
    return BOARD_SOK;
#endif
}

/**
 *  \brief   Hyperbus test app function
 *
 *  \return  None.
 *
 */
#ifndef SPI_BOOT_FRAMEWORK
void test_csl_hyperbus_app(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    Board_PinmuxConfig_t pixmuxCfg;
    int32_t ret = 0;

    /* Configure the pinmux for hyperbus as the default
       pinmux configuration is set for OSPI */
    Board_pinmuxGetCfg(&pixmuxCfg);
    pixmuxCfg.fssCfg = BOARD_PINMUX_FSS_HPB;
    Board_pinmuxSetCfg(&pixmuxCfg);

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO |
               BOARD_INIT_PINMUX_CONFIG;
#endif

    status = Board_init(boardCfg);
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(BOARD_SOK, status);
#endif

    if(status != BOARD_SOK)
    {
	return;
    }

    UART_printf("\n*********************************************\n");
    UART_printf  ("*              HYPERBUS Test                *\n");
    UART_printf  ("*********************************************\n");

#if defined (SOC_J721E) || defined (SOC_J7200)
    UART_printf("\nConfiguring PLLs...");
    status = Board_init(BOARD_INIT_PLL);
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(BOARD_SOK, status);
#endif
    if(status != BOARD_SOK)
    {
        /* PLL configuration failure is observed with some of the clocks
           while running from JTAG but HyperBus clock gets configured properly.
           Print fail message and continue with the test.
         */
        UART_printf("PLL Configurations Failed!!");
    }
    else
    {
        UART_printf("Done\n");
    }
#endif

    status = App_ospiHyperFlashClkCfg();

    if(BOARD_SOK == status)
    {
        App_ospiHyperFlashMux();

        ret = App_HyperBusTest();
    }

    /* Switch back to default pinmux */
    Board_pinmuxGetCfg(&pixmuxCfg);
    pixmuxCfg.fssCfg = BOARD_PINMUX_FSS_OSPI;
    Board_pinmuxSetCfg(&pixmuxCfg);
    Board_init(BOARD_INIT_PINMUX_CONFIG);

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(BOARD_SOK, ret);
#endif
    return;
}
#endif

void test_csl_hyperbus_app_runner(void)
{
    /* @description:Test runner for hyperbus tests

       @requirements: PDK-2409

       @cores: mcu1_0 */
#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    RUN_TEST (test_csl_hyperbus_app);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_csl_hyperbus_app();
#endif
    return;
}

int main(void)
{
    (void) test_csl_hyperbus_app_runner();

    while (true)
    {
    }
}
