/**
 *  \file   ospi_flash_app_main.c
 *
 *  \brief  OSPI Flash test application main file.
 *
 */
 /*
 *  Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   main.c
 *
 *  \brief This file contains the OSPI Flash application which demonstrates
 *         ospi flash read and write.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "ospi_flash_app_test.h"

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* Unity test functions */
void test_csl_ospi_flash_app_runner(void);
void test_csl_ospi_flash_app(void);

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

#define OSPI_FLASH_TEST_MAX_BUF_LEN       (1024U) 
#define PRINT_WRITE_PERF
#define PRINT_READ_PERF

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint8_t txBuf[OSPI_FLASH_TEST_MAX_BUF_LEN]  __attribute__((aligned(128))) __attribute__((section(".ospi_buffer_r5_tcm")));
uint8_t rxBuf[OSPI_FLASH_TEST_MAX_BUF_LEN]  __attribute__((aligned(128))) __attribute__((section(".ospi_buffer_r5_tcm")));

volatile uint64_t   getTicksDelay = 0;          /* No.of ticks taken to do a GTC Reg Read operation */
volatile uint64_t   configStartTicks_T0;        /* GTC Timer ticks at start of configuration */
volatile uint64_t   configStopTicks_T1;         /* GTC Timer ticks at stop of configuration */
volatile uint64_t   transactionStartTicks_T2;   /* GTC Timer ticks at start of transaction */
volatile uint64_t   transcationStopTicks_T5;    /* GTC Timer ticks at stop of transaction */
volatile uint64_t   configTicks;                /* measured total no. of GTC Timer ticks for configuration*/
volatile uint64_t   transactionTicks;           /* measured total no. of GTC Timer ticks for transaction*/
volatile uint64_t   configTime;                 /* configuration time in nsec */
volatile uint64_t   transactionTime;            /* transaction time in nsec */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void Ospi_flashTest(App_OspiObj *ospiObj);
static int32_t Ospi_flashSetGTCClk(uint32_t moduleId,
                                   uint32_t clkId,
                                   uint64_t clkRateHz);

static int32_t Ospi_flashTestRun(App_OspiObj *ospiObj, void *test);
static int32_t OspiFlashTest_init(App_OspiObj *ospiObj, void *test);
static int32_t OspiFlashTest_transfer(App_OspiObj *ospiObj, void *test);
static void OspiFlashTest_deinit(App_OspiObj *ospiObj, void *test);
static void OspiFlash_generatePattern(uint8_t *txBuf, 
                                      uint8_t *rxBuf, 
                                      uint32_t length);

static int32_t OspiFlash_DAC_write(uint8_t *buf, uint32_t numBytes)  __attribute__((section(".ospi_critical_fxns")));
static int32_t OspiFlash_DAC_read(uint8_t *buf, uint32_t numBytes) __attribute__((section(".ospi_critical_fxns")));

static int32_t OspiFlash_verifyData(uint8_t *expData,
                                    uint8_t *rxData,
                                    uint32_t length);

static int32_t OspiFlash_INDAC_write(App_OspiObj *ospiObj, uint8_t *buf, uint32_t numBytes, bool intrPollMode) __attribute__((section(".ospi_critical_fxns")));
static int32_t OspiFlash_INDAC_read(App_OspiObj *ospiObj, uint8_t *buf, uint32_t numBytes, bool intrPollMode) __attribute__((section(".ospi_critical_fxns")));


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

App_OspiObj gOspiAppObj;

/* OSPI Flash Tests data structure */
OSPI_Flash_Tests Ospi_Flash_tests[] =
{
    /* testFunc,        testID,                                     clk,                    dacMode,        intrPollMode,   numBytes,   testDesc */
#if !defined(FLASH_TYPE_XSPI)
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_133M_16B,            OSPI_MODULE_CLK_133M,   TRUE,           TRUE,           16U,        "\r\n OSPI flash test DAC at 133MHz RCLK - Read/Write 16 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_166M_16B,            OSPI_MODULE_CLK_166M,   TRUE,           TRUE,           16U,        "\r\n OSPI flash test DAC at 166MHz RCLK - Read/Write 16 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_133M_32B,            OSPI_MODULE_CLK_133M,   TRUE,           TRUE,           32U,        "\r\n OSPI flash test DAC at 133MHz RCLK - Read/Write 32 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_166M_32B,            OSPI_MODULE_CLK_166M,   TRUE,           TRUE,           32U,        "\r\n OSPI flash test DAC at 166MHz RCLK - Read/Write 32 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_133M_64B,            OSPI_MODULE_CLK_133M,   TRUE,           TRUE,           64U,        "\r\n OSPI flash test DAC at 133MHz RCLK - Read/Write 64 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_166M_64B,            OSPI_MODULE_CLK_166M,   TRUE,           TRUE,           64U,        "\r\n OSPI flash test DAC at 166MHz RCLK - Read/Write 64 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_133M_128B,           OSPI_MODULE_CLK_133M,   TRUE,           TRUE,           128U,       "\r\n OSPI flash test DAC at 133MHz RCLK - Read/Write 128 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_166M_128B,           OSPI_MODULE_CLK_166M,   TRUE,           TRUE,           128U,       "\r\n OSPI flash test DAC at 166MHz RCLK - Read/Write 128 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_133M_256B,           OSPI_MODULE_CLK_133M,   TRUE,           TRUE,           256U,       "\r\n OSPI flash test DAC at 133MHz RCLK - Read/Write 256 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_166M_256B,           OSPI_MODULE_CLK_166M,   TRUE,           TRUE,           256U,       "\r\n OSPI flash test DAC at 166MHz RCLK - Read/Write 256 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_133M_512B,           OSPI_MODULE_CLK_133M,   TRUE,           TRUE,           512U,       "\r\n OSPI flash test DAC at 133MHz RCLK - Read/Write 512 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_166M_512B,           OSPI_MODULE_CLK_166M,   TRUE,           TRUE,           512U,       "\r\n OSPI flash test DAC at 166MHz RCLK - Read/Write 512 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_133M_1024B,          OSPI_MODULE_CLK_133M,   TRUE,           TRUE,           1024U,      "\r\n OSPI flash test DAC at 133MHz RCLK - Read/Write 1024 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_DAC_166M_1024B,          OSPI_MODULE_CLK_166M,   TRUE,           TRUE,           1024U,      "\r\n OSPI flash test DAC at 166MHz RCLK - Read/Write 1024 Bytes"},
#else
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_133M_16B,     OSPI_MODULE_CLK_133M,   FALSE,          TRUE,           16U,        "\r\n OSPI flash test INDAC POLL at 133MHz RCLK - Read/Write 16 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_166M_16B,     OSPI_MODULE_CLK_166M,   FALSE,          TRUE,           16U,        "\r\n OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 16 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_133M_32B,     OSPI_MODULE_CLK_133M,   FALSE,          TRUE,           32U,        "\r\n OSPI flash test INDAC POLL at 133MHz RCLK - Read/Write 32 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_166M_32B,     OSPI_MODULE_CLK_166M,   FALSE,          TRUE,           32U,        "\r\n OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 32 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_133M_64B,     OSPI_MODULE_CLK_133M,   FALSE,          TRUE,           64U,        "\r\n OSPI flash test INDAC POLL at 133MHz RCLK - Read/Write 64 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_166M_64B,     OSPI_MODULE_CLK_166M,   FALSE,          TRUE,           64U,        "\r\n OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 64 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_133M_128B,    OSPI_MODULE_CLK_133M,   FALSE,          TRUE,           128U,       "\r\n OSPI flash test INDAC POLL at 133MHz RCLK - Read/Write 128 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_166M_128B,    OSPI_MODULE_CLK_166M,   FALSE,          TRUE,           128U,       "\r\n OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 128 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_133M_256B,    OSPI_MODULE_CLK_133M,   FALSE,          TRUE,           256U,       "\r\n OSPI flash test INDAC POLL at 133MHz RCLK - Read/Write 256 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_POLL_166M_256B,    OSPI_MODULE_CLK_166M,   FALSE,          TRUE,           256U,       "\r\n OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 256 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_133M_16B,     OSPI_MODULE_CLK_133M,   FALSE,          FALSE,          16U,        "\r\n OSPI flash test INDAC INTR at 133MHz RCLK - Read/Write 16 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_166M_16B,     OSPI_MODULE_CLK_166M,   FALSE,          FALSE,          16U,        "\r\n OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 16 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_133M_32B,     OSPI_MODULE_CLK_133M,   FALSE,          FALSE,          32U,        "\r\n OSPI flash test INDAC INTR at 133MHz RCLK - Read/Write 32 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_166M_32B,     OSPI_MODULE_CLK_166M,   FALSE,          FALSE,          32U,        "\r\n OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 32 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_133M_64B,     OSPI_MODULE_CLK_133M,   FALSE,          FALSE,          64U,        "\r\n OSPI flash test INDAC INTR at 133MHz RCLK - Read/Write 64 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_166M_64B,     OSPI_MODULE_CLK_166M,   FALSE,          FALSE,          64U,        "\r\n OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 64 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_133M_128B,    OSPI_MODULE_CLK_133M,   FALSE,          FALSE,          128U,       "\r\n OSPI flash test INDAC INTR at 133MHz RCLK - Read/Write 128 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_166M_128B,    OSPI_MODULE_CLK_166M,   FALSE,          FALSE,          128U,       "\r\n OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 128 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_133M_256B,    OSPI_MODULE_CLK_133M,   FALSE,          FALSE,          256U,       "\r\n OSPI flash test INDAC INTR at 133MHz RCLK - Read/Write 256 Bytes"},
    {Ospi_flashTestRun, OSPI_FLASH_TEST_ID_INDAC_INTR_166M_256B,    OSPI_MODULE_CLK_166M,   FALSE,          FALSE,          256U,       "\r\n OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 256 Bytes"},
#endif    
    {NULL, }
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void test_csl_ospi_flash_app(void)
{

    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    int32_t       status;
    App_OspiObj   *ospiObj = &gOspiAppObj;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_UART_STDIO;
               
    boardStatus = Board_init(boardCfg);

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(BOARD_SOK, boardStatus);
#endif

    if (boardStatus != BOARD_SOK)
    {
        OSPI_FLASH_log("[Error] Board init failed!!\n");
    }

    /* Configure GTC Timer for profiling */
    status = Ospi_flashSetGTCClk(OSPI_FLASH_GTC_MOD_ID,
                                 OSPI_FLASH_GTC_CLK_ID,
                                 OSPI_FLASH_GTC_CLK_FREQ);
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);
#endif

    if (CSL_PASS != status)
    {
        OSPI_FLASH_log("[Error] Configure GTC Timer failed!!\n");
    }

    Ospi_flashTest(ospiObj);

    return;
}

static int32_t Ospi_flashSetGTCClk(uint32_t moduleId,
                                   uint32_t clkId,
                                   uint64_t clkRateHz)
{
    int32_t status = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint64_t currClkFreqHz;

    status = Sciclient_pmGetModuleClkFreq(moduleId,
                                          clkId,
                                          &currClkFreqHz,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if ((status == CSL_PASS) &&
        (currClkFreqHz != clkRateHz))
    {
        status = OspiFlash_ClkRateSet(moduleId, clkId, clkRateHz);
    }

    /* Enable GTC */
    HW_WR_REG32(CSL_GTC0_GTC_CFG1_BASE + 0x0U, 0x1);

    /* Measure and store the time spent to do a getTime operation */
    getTicksDelay = OspiFash_getGTCTimerTicks();
    getTicksDelay = OspiFash_getGTCTimerTicks() - getTicksDelay;
    OSPI_FLASH_log("\n Time taken to read GTC Timer ticks = %d ns (%d ticks) ",
                   (uint32_t)((getTicksDelay*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ), 
                   (uint32_t)getTicksDelay);

    return (status);
}

static void Ospi_flashTest(App_OspiObj *ospiObj)
{
    uint32_t    i;
    Bool        testFail = FALSE;
    int32_t     status = OSPI_FLASH_APP_STATUS_SUCCESS;
    OSPI_Flash_Tests *test;

    for (i = 0;  ; i++)                 
    {
        test = &Ospi_Flash_tests[i];
        if (test->testFunc == NULL)
        {
            break;
        }

        status = test->testFunc((App_OspiObj *)ospiObj, (void *)test);

#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_EQUAL_INT32(OSPI_FLASH_APP_STATUS_SUCCESS, status);
#endif
        if(OSPI_FLASH_APP_STATUS_SUCCESS == status)
        {
            OSPI_FLASH_log("\r\n %s have passed\r\n", test->testDesc);
        }
        else
        {
            OSPI_FLASH_log("\r\n %s have failed\r\n", test->testDesc);
            testFail = TRUE;
            break;
        }
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_UINT32(FALSE, testFail);
#endif
    if(TRUE == testFail)
    {
        OSPI_FLASH_log("\n Some tests have failed. \n");
    }
    else
    {
        OSPI_FLASH_log("\n All tests have passed. \n");
    }

    OSPI_FLASH_log("Done\n");

    while (true)
    {
    }
}

static int32_t Ospi_flashTestRun(App_OspiObj *ospiObj, void *test)
{
    int32_t status = OSPI_FLASH_APP_STATUS_SUCCESS;

    configStartTicks_T0 = OspiFash_getGTCTimerTicks();
    status = OspiFlashTest_init(ospiObj, test);
    
    if(OSPI_FLASH_APP_STATUS_SUCCESS != status)
    {
        OSPI_FLASH_log("[Error] OSPI Flash Test init failed!!\n");
    }
    else
    {
        status = OspiFlashTest_transfer(ospiObj, test);
        if(OSPI_FLASH_APP_STATUS_SUCCESS != status)
        {
            OSPI_FLASH_log("[Error] OSPI Flash Test transfer failed!!\n");
        }

        OspiFlashTest_deinit(ospiObj, test);
    }

    return (status);
}

/*
** This function will call the necessary CSL ospi APIs which will configure the
** OSPI controller.
*/
static int32_t OspiFlashTest_init(App_OspiObj *ospiObj, void *test)
{
    int32_t status = OSPI_FLASH_APP_STATUS_SUCCESS;

    OSPI_Flash_Tests    *pTest = (OSPI_Flash_Tests *)test;

    status += OspiFlash_ospiConfigClk(pTest->clk);
    if(OSPI_FLASH_APP_STATUS_SUCCESS == status)
    {
        OSPI_FLASH_log("\n OSPI RCLK running at %d Hz. \n", pTest->clk);
    }

    /* Print test description */
    OSPI_FLASH_log("\r\n %s\r\n", pTest->testDesc);

    status += OspiFlash_ospiOpen(ospiObj, OSPI_FLASH_WRITE_TIMEOUT, OSPI_FLASH_CHECK_IDLE_DELAY, pTest->dacMode, pTest->intrPollMode);

    /* TO DO : If supporting Legacy SPI mode, the following should be based on dtrEnable.
    * ie, if Legacy SPI mode -> dtrEnable will be false -> Enable SDR instead */
    status += OspiFlash_ospiEnableDDR(pTest->dacMode, pTest->intrPollMode);

    status += OspiFlash_ospiSetOpcode(pTest->dacMode);

    /* If non DAC /Legacy SPI mode, no need to config PHY, skip this */
    if(pTest->dacMode)    
        status += OspiFlash_ospiConfigPHY(pTest->clk, pTest->intrPollMode);


    return (status);    
}

static int32_t OspiFlashTest_transfer(App_OspiObj *ospiObj, void *test)
{
    int32_t status = OSPI_FLASH_APP_STATUS_SUCCESS;
    OSPI_Flash_Tests    *pTest = (OSPI_Flash_Tests *)test;

    OspiFlash_generatePattern(txBuf, rxBuf, pTest->numBytes);

    status += OspiFlash_ospiEraseBlk(0U, pTest->numBytes, pTest->intrPollMode);

    if(pTest->dacMode) 
    {
        status += OspiFlash_DAC_write(txBuf, pTest->numBytes);
        status += OspiFlash_DAC_read(rxBuf, pTest->numBytes);
    }
    else                    
    {
        status += OspiFlash_INDAC_write(ospiObj, txBuf, pTest->numBytes, pTest->intrPollMode);
        status += OspiFlash_INDAC_read(ospiObj, rxBuf, pTest->numBytes, pTest->intrPollMode);
    }
    

    /* Verify whether the data written and read are Equal */
    status += OspiFlash_verifyData(txBuf, rxBuf, pTest->numBytes);

    return (status);    
}

static void OspiFlashTest_deinit(App_OspiObj *ospiObj, void *test)
{
    OSPI_Flash_Tests    *pTest = (OSPI_Flash_Tests *)test;

    OspiFlash_ospiClose(ospiObj, pTest->intrPollMode);
    
    return;    
}

static int32_t OspiFlash_DAC_write(uint8_t *buf, uint32_t numBytes)
{
    int32_t             status = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint8_t             *pSrc;         /* Source address */
    uint32_t            i;
    uint8_t             *pDst;         /* Destination address */
    uint32_t            size;
    uint32_t            remainSize;
    uint32_t            wrWord;
    uint32_t            wrByte;
    uint32_t            timeOutVal;
    uint32_t            retry;
    volatile uint32_t   delay;
    uint8_t             statusRegVal;
    uint32_t            regVal;

    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);

    OspiFlash_ospiXferIntrInit(TRUE);
    
    /* Disable PHY pipeline mode */
    CSL_ospiPipelinePhyEnable((const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR), FALSE);

    
    pSrc = (uint8_t *)buf;
    pDst = (uint8_t *)(OSPI_FLASH_DATA_BASE_ADDR);
    remainSize = (uint32_t)numBytes & 3U;
    size = (uint32_t)numBytes - remainSize;

#ifdef PRINT_WRITE_PERF
    configStopTicks_T1 = OspiFash_getGTCTimerTicks();
    transactionStartTicks_T2 = OspiFash_getGTCTimerTicks();
#endif
    /* Transfer the data in 32-bit size */
    for(i = 0U; i < size; i += 4U)
    {
        wrWord = CSL_REG32_RD(pSrc + i);
        CSL_REG32_WR(pDst + i, wrWord);
        timeOutVal = OSPI_FLASH_WRITE_TIMEOUT;
        statusRegVal = 0xFF;
        while (timeOutVal != 0U)
        {
            (void)CSL_ospiCmdRead(baseAddr, OSPI_FLASH_CMD_RDSR, 1U);
            while(!CSL_ospiIsIdle(baseAddr));
            CSL_ospiFlashExecCmd(baseAddr);
            retry = OSPI_FLASH_WRITE_TIMEOUT;
            while(retry != 0U)
            {
                if(CSL_ospiFlashExecCmdComplete(baseAddr) == TRUE)
                {
                    break;
                }
                delay = OSPI_FLASH_CHECK_IDLE_DELAY;
                while (delay > 0U)
                {  
                    delay = delay - 1U;
                }
                retry--;
            }
            while(!CSL_ospiIsIdle(baseAddr));
            regVal = CSL_REG32_RD(&baseAddr->FLASH_RD_DATA_LOWER_REG);
            (void)memcpy((void *)&statusRegVal, (void *)(&regVal), 1U);
            if ((statusRegVal & 1U) == 0U)
            {
                break;
            }
            timeOutVal--;
            delay = OSPI_FLASH_CHECK_IDLE_DELAY;
            while (delay > 0U)
            {  
                delay = delay - 1U;
            }
        }
    }
    if(OSPI_FLASH_APP_STATUS_SUCCESS == status)
    {
        /* Transfer the remaining data in 8-bit size */
        for(i = 0; i < remainSize; i++)
        {
            wrByte = CSL_REG8_RD(pSrc + size + i);
            CSL_REG8_WR(pDst + size + i, wrByte);
            timeOutVal = OSPI_FLASH_WRITE_TIMEOUT;
            statusRegVal = 0xFF;
            while (timeOutVal != 0U)
            {
                (void)CSL_ospiCmdRead(baseAddr, OSPI_FLASH_CMD_RDSR, 1U);
                while(!CSL_ospiIsIdle(baseAddr));
                CSL_ospiFlashExecCmd(baseAddr);
                retry = OSPI_FLASH_WRITE_TIMEOUT;
                while(retry != 0U)
                {
                    if(CSL_ospiFlashExecCmdComplete(baseAddr) == TRUE)
                    {
                        break;
                    }
                    delay = OSPI_FLASH_CHECK_IDLE_DELAY;
                    while (delay > 0U)
                    {  
                        delay = delay - 1U;
                    }
                    retry--;
                }
                while(!CSL_ospiIsIdle(baseAddr));
                regVal = CSL_REG32_RD(&baseAddr->FLASH_RD_DATA_LOWER_REG);
                (void)memcpy((void *)&statusRegVal, (void *)(&regVal), 1U);
                if ((statusRegVal & 1U) == 0U)
                {
                    break;
                }
                timeOutVal--;
                delay = OSPI_FLASH_CHECK_IDLE_DELAY;
                while (delay > 0U)
                {  
                    delay = delay - 1U;
                }
            }
        }
    }
#ifdef PRINT_WRITE_PERF
    transcationStopTicks_T5 = OspiFash_getGTCTimerTicks();
    
    configTicks = configStopTicks_T1 - configStartTicks_T0  - getTicksDelay;
    configTime = (configTicks*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;
    
    transactionTicks = transcationStopTicks_T5 - transactionStartTicks_T2  - getTicksDelay;
    transactionTime = (transactionTicks*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;

    OSPI_FLASH_log("\n OSPI Write %d bytes complete.\tConfig Time:\t%d ns\tTransaction Time:\t%d ns", numBytes, (uint32_t)configTime, (uint32_t)transactionTime);
#endif

    return status;
}

static int32_t OspiFlash_INDAC_write(App_OspiObj *ospiObj, uint8_t *buf, uint32_t numBytes, bool intrPollMode)
{
    int32_t status = OSPI_FLASH_APP_STATUS_SUCCESS;

#ifdef PRINT_WRITE_PERF
    configStopTicks_T1 = OspiFash_getGTCTimerTicks();
    transactionStartTicks_T2 = OspiFash_getGTCTimerTicks();
#endif

    status = OspiFlash_xspiIndacWrite(ospiObj, buf, numBytes, intrPollMode, 0U);

#ifdef PRINT_WRITE_PERF
    transcationStopTicks_T5 = OspiFash_getGTCTimerTicks();
    
    configTicks = configStopTicks_T1 - configStartTicks_T0  - getTicksDelay;
    configTime = (configTicks*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;
    
    transactionTicks = transcationStopTicks_T5 - transactionStartTicks_T2  - getTicksDelay;
    transactionTime = (transactionTicks*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;

    OSPI_FLASH_log("\n OSPI Write %d bytes complete.\tConfig Time:\t%d ns\tTransaction Time:\t%d ns", numBytes, (uint32_t)configTime, (uint32_t)transactionTime);
#endif

    return status;
}

static int32_t OSPI_waitReadSramLvl(const CSL_ospi_flash_cfgRegs *pReg,
                                    uint32_t *rdLevel)
{
    uint32_t retry = OSPI_FLASH_WRITE_TIMEOUT;
    uint32_t sramLevel;
    int32_t  retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    volatile uint32_t   delay;

    while(retry != 0U)
    {
        sramLevel = CSL_ospiGetSramLvl(pReg, 1U);
        if (sramLevel != 0U)
        {
            *rdLevel = sramLevel;
            break;
        }
        delay = OSPI_FLASH_CHECK_IDLE_DELAY;
        while (delay > 0U)
            delay = delay - 1U;
        retry--;
    }

    if (retry != 0U)
    {
        retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    }
    else
    {
        retVal = OSPI_FLASH_APP_STATUS_ERROR;
    }
    return(retVal);
}

static int32_t OSPI_waitIndReadComplete(const CSL_ospi_flash_cfgRegs *pRegs)
{
    uint32_t retry  = OSPI_FLASH_WRITE_TIMEOUT;
    int32_t  retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    volatile uint32_t   delay;

    /* Check flash indirect read controller status */
    while (retry != 0U)
    {
        if (CSL_ospiIndReadComplete(pRegs) == TRUE)
        {
            break;
        }
        delay = OSPI_FLASH_CHECK_IDLE_DELAY;
        while (delay > 0U)
            delay = delay - 1U;
        retry--;
    }

    if (retry != 0U)
    {
        /* Clear indirect completion status */
        CSL_ospiClrIndReadComplete(pRegs);
        retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    }
    else
    {
        retVal = OSPI_FLASH_APP_STATUS_ERROR;
    }
    return(retVal);
}

static int32_t OspiFlash_INDAC_read(App_OspiObj *ospiObj, uint8_t *buf, uint32_t numBytes, bool intrPollMode) /* based on OSPI_ind_xfer_mode_read_v0 in OSPI_v0.c */
{
    int32_t             status = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint8_t             *pDst;         /* Destination address */
    uint32_t            size;
    uint32_t            remainSize;
    uint32_t            sramLevel = 0; 
    uint32_t            rdBytes = 0;
    uint32_t            offset = 0; 

    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    pDst = (uint8_t *)buf;
    remainSize = 0;
    size = (uint32_t)numBytes;

    OspiFlash_ospiXferIntrInit(intrPollMode);
    
    /* Disable DAC mode */
    CSL_ospiDacEnable(baseAddr, FALSE);
    /* Set read address in indirect mode */
    CSL_ospiIndSetStartAddr(baseAddr, (uint32_t)offset, TRUE);

#ifdef PRINT_READ_PERF
    configStopTicks_T1 = OspiFash_getGTCTimerTicks();
#endif

    if (intrPollMode)                /* if interrupt mode is used then OSPI_hwiFxn will handle, if poll mode is used the below code will handle */
    {
        /* start the indirect read */
        CSL_ospiIndReadExecute(baseAddr, size);

    #ifdef PRINT_READ_PERF
        transactionStartTicks_T2 = OspiFash_getGTCTimerTicks();
    #endif

        remainSize = size;
        while(remainSize > 0U)
        {
            /* Wait indirect read SRAM fifo has data */
            status = OSPI_waitReadSramLvl(baseAddr, &sramLevel);

            rdBytes = sramLevel * CSL_OSPI_FIFO_WIDTH;
            rdBytes = (rdBytes > remainSize) ? remainSize : rdBytes;

            /* Read data from FIFO */
            CSL_ospiReadFifoData((uintptr_t)(OSPI_FLASH_DATA_BASE_ADDR), pDst, rdBytes);       /* OSPI_FLASH_DATA_BASE_ADDR = MCU_FSS0_DAT_REG1 = 0x0050000000 */

            pDst += rdBytes;
            remainSize -= rdBytes;
        }
        if(status == OSPI_FLASH_APP_STATUS_SUCCESS)
        {
            status = OSPI_waitIndReadComplete(baseAddr);
        }
        if(status == OSPI_FLASH_APP_STATUS_ERROR)
        {
            CSL_ospiIndWriteCancel(baseAddr);
        }
    }
    else
    {
        /* Book keeping of transmit and receive buffers */
        ospiObj->writeBufIdx = NULL_PTR;
        ospiObj->writeCountIdx = 0;
        ospiObj->readBufIdx =  (uint8_t *)rxBuf;
        ospiObj->readCountIdx = (uint32_t)numBytes;

        /* start the indirect read */
        CSL_ospiIndReadExecute(baseAddr, size);

    #ifdef PRINT_READ_PERF
        transactionStartTicks_T2 = OspiFash_getGTCTimerTicks();
    #endif

        /* To wait till read completes in interrupt mode? */
        SemaphoreP_pend(ospiObj->ospiFlashTransferDoneSem, SemaphoreP_WAIT_FOREVER);
    }

#ifdef PRINT_READ_PERF
    transcationStopTicks_T5 = OspiFash_getGTCTimerTicks();
    
    configTicks = configStopTicks_T1 - configStartTicks_T0  - getTicksDelay;
    configTime = (configTicks*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;
    
    transactionTicks = transcationStopTicks_T5 - transactionStartTicks_T2  - getTicksDelay;
    transactionTime = (transactionTicks*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;

    OSPI_FLASH_log("\n OSPI Read %d bytes complete.\tConfig Time:\t%d ns\tTransaction Time:\t%d ns", numBytes, (uint32_t)configTime, (uint32_t)transactionTime);
#endif

    return status;
}

static int32_t OspiFlash_DAC_read(uint8_t *buf, uint32_t numBytes)
{
    int32_t             status = OSPI_FLASH_APP_STATUS_SUCCESS;

    OspiFlash_ospiXferIntrInit(TRUE);

#ifdef PRINT_READ_PERF
    configStopTicks_T1 = OspiFash_getGTCTimerTicks();
    transactionStartTicks_T2 = OspiFash_getGTCTimerTicks();
#endif

    memcpy(buf, (uint8_t *)(OSPI_FLASH_DATA_BASE_ADDR), numBytes);

#ifdef PRINT_READ_PERF
    transcationStopTicks_T5 = OspiFash_getGTCTimerTicks();
    
    configTicks = configStopTicks_T1 - configStartTicks_T0  - getTicksDelay;
    configTime = (configTicks*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;
    
    transactionTicks = transcationStopTicks_T5 - transactionStartTicks_T2  - getTicksDelay;
    transactionTime = (transactionTicks*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;

    OSPI_FLASH_log("\n OSPI Read %d bytes complete.\tConfig Time:\t%d ns\tTransaction Time:\t%d ns", numBytes, (uint32_t)configTime, (uint32_t)transactionTime);
#endif

    return status;
}

/*
 *  ======== CompareData ========
 */
static int32_t OspiFlash_verifyData(uint8_t *expData,
                                    uint8_t *rxData,
                                    uint32_t length)
{ 
    uint32_t idx = 0;
    uint32_t match = 1;
    int32_t status = OSPI_FLASH_APP_STATUS_ERROR;

    for(idx = 0; ((idx < length) && (match != 0)); idx++)
    {
        if(*expData != *rxData)
        {
            match = 0;
            OSPI_FLASH_log("Data mismatch at idx %d\n", idx);
        }
        expData++;
        rxData++;
    }

    if(match == 1)
    {
        status = OSPI_FLASH_APP_STATUS_SUCCESS;
    }

    return status;
}

/*
 *  ======== GeneratePattern ========
 */
static void OspiFlash_generatePattern(uint8_t *txBuf, 
                                      uint8_t *rxBuf, 
                                      uint32_t length)
{
    volatile uint32_t idx;
    volatile uint8_t *txPtr = txBuf;
    volatile uint8_t *rxPtr = rxBuf;

    for(idx = 0; idx < length; idx++)
    {
        if(idx < (length/2))
        {
            *txPtr++ = (uint8_t)idx;
        }
        else if(idx < (length/4*3))
        {
            *txPtr++ = 0xaa;
        }
        else
        {
            *txPtr++ = 0x55;
        }
    	*rxPtr++ = (uint8_t)0U;
    }
}

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

void test_csl_ospi_flash_app_runner(void)
{
    /* @description:Test runner for ospi tests

       @requirements: PDK-2408

       @cores: mcu1_0 */
#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    RUN_TEST (test_csl_ospi_flash_app);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_csl_ospi_flash_app();
#endif
    return;
}

int main(void)
{
    (void) test_csl_ospi_flash_app_runner();

    while (TRUE)
    {
    }
}
/********************************* End Of File ******************************/
