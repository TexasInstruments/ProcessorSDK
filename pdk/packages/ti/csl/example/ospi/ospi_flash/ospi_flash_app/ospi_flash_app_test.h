/**
 *  \file   ospi_flash_app_test.h
 *
 *  \brief  OSPI Flash test application header file.
 *
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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

#ifndef OSPI_FLASH_APP_TEST_H
#define OSPI_FLASH_APP_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>
#include <ti/csl/example/ospi/ospi_flash/common/ospi_flash_common.h>

/*
 * OSPI Flash App Test Data Structure
 */
typedef struct OSPI_Flash_Tests_s
{
    int32_t  (*testFunc)(App_OspiObj*, void *);
    int32_t  testId;    
    uint32_t clk;
    bool     dacMode;           
    bool     intrPollMode;         
    uint32_t numBytes;
    char     testDesc[80];

} OSPI_Flash_Tests;

/*
 * UDMA OSPI Flash test ID definitions
 */
/** \brief OSPI flash test DAC at 133MHz RCLK - Read/Write 16 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_133M_16B              (0U)  
/** \brief OSPI flash test DAC at 166MHz RCLK - Read/Write 16 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_166M_16B              (1U) 
/** \brief OSPI flash test DAC at 133MHz RCLK - Read/Write 32 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_133M_32B              (2U) 
/** \brief OSPI flash test DAC at 166MHz RCLK - Read/Write 32 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_166M_32B              (3U)  
/** \brief OSPI flash test DAC at 133MHz RCLK - Read/Write 64 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_133M_64B              (4U)  
/** \brief OSPI flash test DAC at 166MHz RCLK - Read/Write 64 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_166M_64B              (5U)  
/** \brief OSPI flash test DAC at 133MHz RCLK - Read/Write 128 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_133M_128B             (6U) 
/** \brief OSPI flash test DAC at 166MHz RCLK - Read/Write 128 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_166M_128B             (7U)  
/** \brief OSPI flash test DAC at 133MHz RCLK - Read/Write 256 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_133M_256B             (8U)  
/** \brief OSPI flash test DAC at 166MHz RCLK - Read/Write 256 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_166M_256B             (9U)   
/** \brief OSPI flash test DAC at 133MHz RCLK - Read/Write 512 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_133M_512B             (10U)  
/** \brief OSPI flash test DAC at 166MHz RCLK - Read/Write 512 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_166M_512B             (11U)  
/** \brief OSPI flash test DAC at 133MHz RCLK - Read/Write 1024 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_133M_1024B            (12U) 
/** \brief OSPI flash test DAC at 166MHz RCLK - Read/Write 1024 Bytes */
#define OSPI_FLASH_TEST_ID_DAC_166M_1024B            (13U)
/** \brief OSPI flash test DAC at 133MHz RCLK - Read/Write 16 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_133M_16B       (14U)  
/** \brief OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 16 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_166M_16B       (15U) 
/** \brief OSPI flash test INDAC POLL at 133MHz RCLK - Read/Write 32 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_133M_32B       (16U) 
/** \brief OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 32 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_166M_32B       (17U)  
/** \brief OSPI flash test INDAC POLL at 133MHz RCLK - Read/Write 64 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_133M_64B       (18U)  
/** \brief OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 64 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_166M_64B       (19U)  
/** \brief OSPI flash test INDAC POLL at 133MHz RCLK - Read/Write 128 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_133M_128B      (20U) 
/** \brief OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 128 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_166M_128B      (21U)  
/** \brief OSPI flash test INDAC POLL at 133MHz RCLK - Read/Write 256 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_133M_256B      (22U)  
/** \brief OSPI flash test INDAC POLL at 166MHz RCLK - Read/Write 256 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_POLL_166M_256B      (23U)   
/** \brief OSPI flash test DAC at 133MHz RCLK - Read/Write 16 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_133M_16B       (24U)  
/** \brief OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 16 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_166M_16B       (25U) 
/** \brief OSPI flash test INDAC INTR at 133MHz RCLK - Read/Write 32 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_133M_32B       (26U) 
/** \brief OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 32 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_166M_32B       (27U)  
/** \brief OSPI flash test INDAC INTR at 133MHz RCLK - Read/Write 64 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_133M_64B       (28U)  
/** \brief OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 64 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_166M_64B       (29U)  
/** \brief OSPI flash test INDAC INTR at 133MHz RCLK - Read/Write 128 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_133M_128B      (30U) 
/** \brief OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 128 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_166M_128B      (31U)  
/** \brief OSPI flash test INDAC INTR at 133MHz RCLK - Read/Write 256 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_133M_256B      (32U)  
/** \brief OSPI flash test INDAC INTR at 166MHz RCLK - Read/Write 256 Bytes */
#define OSPI_FLASH_TEST_ID_INDAC_INTR_166M_256B      (33U)   

/* Get GTC Timer Ticks */
#define OspiFash_getGTCTimerTicks() (*((uint64_t *)(CSL_GTC0_GTC_CFG1_BASE + 0x8U)))

extern void UART_printf(const char *pcString, ...);
/* Enable the below macro to have prints on the IO Console */
//#define IO_CONSOLE

#ifndef IO_CONSOLE
#define OSPI_FLASH_log                UART_printf
#else
#define OSPI_FLASH_log                printf
#endif

#ifdef __cplusplus
}
#endif

#endif /* OSPI_FLASH_APP_TEST_H */
