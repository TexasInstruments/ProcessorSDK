
/**
 *  \file ospi_flash_soc.h
 *
 *  \brief OSPI FLASH Example AM65xx SOC specific file.
 */
/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 */

#ifndef OSPI_FLASH_SOC_H
#define OSPI_FLASH_SOC_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSPI_FLASH_GTC_MOD_ID           (TISCI_DEV_GTC0)
#define OSPI_FLASH_GTC_CLK_ID           (TISCI_DEV_CBASS_INFRA0_BUS_GTC_CLOCK_1_CLK)
#define OSPI_FLASH_GTC_CLK_FREQ         (200000000U)

#define OSPI_FLASH_DEVICE_ID            (TISCI_DEV_MCU_FSS0_OSPI_0)
#define OSPI_FLASH_RCLK_CLK             (TISCI_DEV_MCU_FSS0_OSPI_0_BUS_OSPI0_RCLK_CLK)
#define OSPI_FLASH_PARENT_CLK_166M      (TISCI_DEV_MCU_FSS0_OSPI_0_BUS_OSPI0_RCLK_CLK_PARENT_ADPLLM_HSDIV_WRAP_MCU_1_BUS_HSDIV_CLKOUT4_CLK)
#define OSPI_FLASH_PARENT_CLK_133M      (TISCI_DEV_MCU_FSS0_OSPI_0_BUS_OSPI0_RCLK_CLK_PARENT_ADPLLM_HSDIV_WRAP_MCU_0_BUS_HSDIV_CLKOUT4_CLK)

/* OSPI0 on the Main domain :- baseAddr - flash config register baseAddr */
#define OSPI_FLASH_CONFIG_REG_BASE_ADDR     (CSL_MCU_FSS0_OSPI0_CTRL_BASE)

/* OSPI_FLASH_DATA_BASE_ADDR : dataAddr - OSPI data base address */
/* OSPI_FLASH_INTR_NUM       : intrNum  - OSPI interrupt number */
#if defined (__aarch64__)
  #define OSPI_FLASH_DATA_BASE_ADDR         (CSL_MCU_FSS0_DAT_REG0_BASE)   
  #define OSPI_FLASH_INTR_NUM               (CSL_GIC0_INTR_MCU_FSS0_BUS_OSPI0_LVL_INTR)       
#else
  #define OSPI_FLASH_DATA_BASE_ADDR         (CSL_MCU_FSS0_DAT_REG1_BASE)
  #define OSPI_FLASH_INTR_NUM               (CSL_MCU0_INTR_FSS0_OSPI0_LVL_INTR)
#endif

/* blkSize - OSPI device block size in bytes - device block size is 2 ^ 17 = 128K bytes */
#define OSPI_FLASH_DEVICE_BLOCK_SIZE        		   	(128U * 1024U) 

#define OSPI_FLASH_NOR_MANF_ID                     		(0x2CU)
#define OSPI_FLASH_NOR_DEVICE_ID                   		(OSPI_FLASH_NOR_DEVICE_ID_MT35XU512)
#define OSPI_FLASH_NOR_CMD_WRITE_VCR               		(0x81U)
#define OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_DAC   	(16U)
#define OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_INDAC 	(16U)

#define OSPI_FLASH_DEVICE_PAGE_SIZE    				    (256U) /* OSPI device page size in bytes */
#define OSPI_FLASH_DEVICE_SIZE                    (64U * 1024U * 1024U)

#define OSPI_FLASH_CMD_WREN       					    (0x06U)
#define OSPI_FLASH_CMD_RDID       					    (0x9FU)
#define OSPI_FLASH_CMD_RDSR       					    (0x05U)

#define OSPI_FLASH_PAGE_PROG_TIMEOUT		           	(400U)
#define OSPI_FLASH_WRR_WRITE_TIMEOUT    			    (600U * 1000U)
#define OSPI_FLASH_BULK_ERASE_TIMEOUT   			    (110U * 1000U * 1000U)

#define OSPI_FLASH_RDID_NUM_BYTES       			    (0x3U)
#define OSPI_FLASH_CMD_OCTAL_DDR_O_FAST_RD      	 	(0x9DU)

/** Macro to enable 4 byte addressing */
/* #define OSPI_FLASH_EXT_ADDRESS_ENABLE        	(0U) */

#ifdef OSPI_FLASH_EXT_ADDRESS_ENABLE
#define OSPI_FLASH_CMD_OCTAL_FAST_PROG      		(0x84U)
#define OSPI_FLASH_CMD_BLOCK_ERASE          		(0xDCU)
#else
#define OSPI_FLASH_CMD_OCTAL_FAST_PROG      		(0x82U)
#define OSPI_FLASH_CMD_BLOCK_ERASE          		(0xD8U)
#endif

/* define the unlock and lock values */
#define KICK0_UNLOCK_VAL 0x68EF3490
#define KICK1_UNLOCK_VAL 0xD172BC5A
#define KICK_LOCK_VAL    0x00000000

#define MAIN_MMR_BASE_ADDRESS   CSL_CTRL_MMR0_CFG0_BASE
#define MCU_MMR_BASE_ADDRESS    CSL_MCU_CTRL_MMR0_CFG0_BASE
#define WKUP_MMR_BASE_ADDRESS   CSL_WKUP_CTRL_MMR0_CFG0_BASE

#define OSPI_FLASH_NOR_CFG5_VREG_ADDR      (0x00U)
#define OSPI_FLASH_OCTAL_DDR							 (0xE7U)

#define OSPI_FLASH_STATUS_REG                            (CSL_WKUP_VTM0_CFG0_BASE + 0x308U)
#define OSPI_FLASH_CONTROL_REG                           (CSL_WKUP_VTM0_CFG1_BASE + 0x300U) 

#define FLASH_TYPE_OSPI

#define CSL2PTR (uint32_t *)(uintptr_t)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OSPI_FLASH_SOC_H */
