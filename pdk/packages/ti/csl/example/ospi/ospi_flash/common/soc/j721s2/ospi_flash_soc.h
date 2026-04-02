
/**
 *  \file ospi_flash_soc.h
 *
 *  \brief OSPI FLASH Example J721S2 SOC specific file.
 */
/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
#define OSPI_FLASH_GTC_CLK_ID           (TISCI_DEV_GTC0_GTC_CLK)
#define OSPI_FLASH_GTC_CLK_FREQ         (200000000U)

#define OSPI_FLASH_DEVICE_ID            (TISCI_DEV_MCU_FSS0_OSPI_0)
#define OSPI_FLASH_RCLK_CLK             (TISCI_DEV_MCU_FSS0_OSPI_0_OSPI_RCLK_CLK)
#define OSPI_FLASH_PARENT_CLK_166M      (TISCI_DEV_MCU_FSS0_OSPI_0_OSPI_RCLK_CLK_PARENT_HSDIV4_16FFT_MCU_2_HSDIVOUT4_CLK)
#define OSPI_FLASH_PARENT_CLK_133M      (TISCI_DEV_MCU_FSS0_OSPI_0_OSPI_RCLK_CLK_PARENT_HSDIV4_16FFT_MCU_1_HSDIVOUT4_CLK)

/* OSPI0 on the Main domain :- baseAddr - flash config register baseAddr */
#define OSPI_FLASH_CONFIG_REG_BASE_ADDR (CSL_MCU_FSS0_OSPI0_CTRL_BASE)

/* dataAddr - OSPI data base address */
#if defined (__aarch64__)
  #define OSPI_FLASH_DATA_BASE_ADDR     (CSL_MCU_FSS0_DAT_REG0_BASE)   
#else
  #define OSPI_FLASH_DATA_BASE_ADDR     (CSL_MCU_FSS0_DAT_REG1_BASE)
#endif

/* intrNum  - OSPI interrupt number */
#if defined(BUILD_MPU)
  #define OSPI_FLASH_INTR_NUM           (CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_MCU_FSS0_OSPI_0_OSPI_LVL_INTR_0)        
#else
  #define OSPI_FLASH_INTR_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_FSS0_OSPI_0_OSPI_LVL_INTR_0)
#endif

/* blkSize - OSPI device block size in bytes - device block size is 2 ^ 18 = 256K bytes */
#define OSPI_FLASH_DEVICE_BLOCK_SIZE        	           (256U * 1024U)

#define OSPI_FLASH_NOR_MANF_ID                           (0x34U)
#define OSPI_FLASH_NOR_DEVICE_ID                         (OSPI_FLASH_NOR_DEVICE_ID_S28HS512T)
#define OSPI_FLASH_NOR_CMD_WRITE_VCR                     (0x71U)
#define OSPI_FLASH_NOR_OCTAL_DDR_CMD_READ_DUMMY_CYCLE    (4U)
#define OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_DAC        (24U)
#define OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_LC_DAC     (0xBU)
#define OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_INDAC      (20U)
#define OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_LC_INDAC   (0x8U)

#define OSPI_FLASH_DEVICE_PAGE_SIZE    				           (256U) /* OSPI device page size in bytes */
#define OSPI_FLASH_DEVICE_SIZE                           (64U * 1024U * 1024U)

#define OSPI_FLASH_CMD_WREN       					             (0x06U)
#define OSPI_FLASH_CMD_RDID       					             (0x9FU)
#define OSPI_FLASH_CMD_RDSR       					             (0x05U)
#define OSPI_FLASH_CMD_RDREG                             (0x65U)
#define OSPI_FLASH_CMD_WRREG                             (0x71U)

#define OSPI_FLASH_PAGE_PROG_TIMEOUT		                 (400U)
#define OSPI_FLASH_WRR_WRITE_TIMEOUT    			           (600U * 1000U)
#define OSPI_FLASH_BULK_ERASE_TIMEOUT   			           (110U * 1000U * 1000U)

#define OSPI_FLASH_NOR_VREG_OFFSET                       (0x80U)
#define OSPI_FLASH_NOR_NVREG_OFFSET                      (0x0U)

#define OSPI_FLASH_NOR_STS1_NVREG_ADDR                   (0x0U)
#define OSPI_FLASH_NOR_STS2_NVREG_ADDR                   (0x1U)
#define OSPI_FLASH_NOR_CFG1_NVREG_ADDR                   (0x2U)
#define OSPI_FLASH_NOR_CFG2_NVREG_ADDR                   (0x3U)
#define OSPI_FLASH_NOR_CFG3_NVREG_ADDR                   (0x4U)
#define OSPI_FLASH_NOR_CFG4_NVREG_ADDR                   (0x5U)
#define OSPI_FLASH_NOR_CFG5_NVREG_ADDR                   (0x6U)

#define OSPI_FLASH_NOR_STS1_VREG_ADDR                    (0x800000U)
#define OSPI_FLASH_NOR_STS2_VREG_ADDR                    (0x800001U)
#define OSPI_FLASH_NOR_CFG1_VREG_ADDR                    (0x800002U)
#define OSPI_FLASH_NOR_CFG2_VREG_ADDR                    (0x800003U)
#define OSPI_FLASH_NOR_CFG3_VREG_ADDR                    (0x800004U)
#define OSPI_FLASH_NOR_CFG4_VREG_ADDR                    (0x800005U)
#define OSPI_FLASH_NOR_CFG5_VREG_ADDR                    (0x800006U)

#define OSPI_FLASH_RDID_NUM_BYTES       			           (0x3U)

#define OSPI_FLASH_CMD_OCTAL_DDR_O_FAST_RD      	       (0xEEU)
#define OSPI_FLASH_CMD_OCTAL_FAST_PROG      		         (0x12U)
#define OSPI_FLASH_CMD_BLOCK_ERASE          		         (0xDCU)

#define OSPI_FLASH_OCTAL_DDR							               (0x43U)

#define OSPI_FLASH_STATUS_REG                            (CSL_WKUP_VTM0_MMR_VBUSP_CFG1_BASE + 0x308U)
#define OSPI_FLASH_CONTROL_REG                           (CSL_WKUP_VTM0_MMR_VBUSP_CFG2_BASE + 0x300U) 

#define FLASH_TYPE_XSPI

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
