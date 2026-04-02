/*  ============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2010-2019
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
 * @file pcie_ssd.h
 *
 * @brief
 *  Holds all the constants and API definitions required by the example
 *  application to run.
 */

#ifndef _PCIE_SSD_H_
#define _PCIE_SSD_H_

#define ENABLE_FATFS

/* C Standard library include */
#include <string.h>

#include <stdint.h>
#include <stdio.h>

#include <ti/osal/osal.h>

/* CSL include */
#include <ti/csl/cslr_device.h>
#include <ti/csl/hw_types.h>

/* PCIE LLD include */
#include <ti/drv/pcie/pcie.h>

#if !defined(__TI_ARM_V7M4__) && !defined(EDMAPKTBENCH) && !defined(SOC_AM65XX) && !defined(SOC_J721E)
/* Only do EDMA on C6X and A15 */
#define EDMA
#endif

#if defined(SOC_AM65XX)||defined(SOC_J721E)
#define UDMA
#endif

#ifdef EDMA
/* EDMA include */
#include <ti/drv/pcie/example/EDMA/PCIeEDMA.h>
#elif defined(EDMAPKTBENCH)
#include <ti/drv/pcie/example/edmaPktBench/edmaPktBench.h>
#endif

#if defined(SOC_J721E)
/* Do gen2 on all other devices -- remove or -U for GEN1 */
#define GEN3
#else
#define GEN2
#endif

#define Console_printf printf

/* Enable the below macro to have prints on the IO Console */
#if !defined(SOC_AM574x) && !defined(SOC_AM572x) && !defined(SOC_AM571x) && \
    !defined(SOC_K2G) && \
    !defined(SOC_AM65XX) && \
    !defined(SOC_J721E) && \
    !defined(__ARM_ARCH_7A__)
/* AM57x, AM65x and K2G always use UART for all cores.  Also all ARM use UART */
#define IO_CONSOLE
#endif

#ifndef IO_CONSOLE
#include <ti/drv/uart/UART_stdio.h>
#define PCIE_logPrintf  UART_printf
#define PCIE_logScanf   UART_scanFmt
#else
#define PCIE_logPrintf  Console_printf
#define PCIE_logScanf   Console_scanf
#endif

/* Size of application buffers */
#define PCIE_BUFSIZE_APP 40

/* Number of each type of interrupt to send */
#define PCIE_NUM_INTS   10

/* Write pattern */
#define PCIE_WR_PATTERN 0xFACEFACE


#define PCIE_EP_LEGACY_A_IRQ_ENABLE_SET_REG     0x5500188
#define PCIE_EP_LEGACY_B_IRQ_ENABLE_SET_REG     0x5500198
#define PCIE_EP_LEGACY_C_IRQ_ENABLE_SET_REG     0x55001A8
#define PCIE_EP_LEGACY_D_IRQ_ENABLE_SET_REG     0x55001B8

#define PCIE_EP_ERR_IRQ_ENABLE_SET_REG          0x55001C8

#define PCIE_EP_BAR0_REG                        0x5501010


/* In this example all addresses are 32bit */
/* Outbound Base Address for PCIe RC */
#define PCIE_OB_LO_ADDR_RC   0x70000000 /* 0x10030000 */
#define PCIE_OB_HI_ADDR_RC   0

/* Inbound  Base Address for PCIe RC */
#define PCIE_IB_LO_ADDR_RC   0x90000000
#define PCIE_IB_HI_ADDR_RC   0

/* Outbound Base Address for PCIe EP */
#define PCIE_OB_LO_ADDR_EP   PCIE_IB_LO_ADDR_RC
#define PCIE_OB_HI_ADDR_EP   0

/* Inbound  Base Address for PCIe EP */
#define PCIE_IB_LO_ADDR_EP   PCIE_OB_LO_ADDR_RC
#define PCIE_IB_HI_ADDR_EP   0


#define PCIE_WINDOW_START    0x10000000U

/* PCIE address space for MSI */
#define PCIE_PCIE_MSI_BASE   (0x00010000U)
#define PCIE_PCIE_MSI_OFF    (0x00000040U)

/* Data area offset absolute including PCIe base (only used rev 2) */
#define PCIE_WINDOW_MEM_BASE (PCIE_WINDOW_START + 0x01000000U)/*(PCIE_WINDOW_START + 0x00030000U)*/
#define PCIE_WINDOW_MEM_MASK 0x00FFFFFFU  /* 0x0000FFFFU */

/* Cfg area offset absolute including PCIe base (only used rev 2) */
/* This MUST agree Pciev1_DeviceCfgBaseAddrs.bases! */
#define PCIE_WINDOW_CFG_BASE (PCIE_WINDOW_START + 0x00010000U) /* (PCIE_WINDOW_START) */
#define PCIE_WINDOW_CFG_MASK 0x0000FFFFU

/* MSI address in PCIE data window */
#define PCIE_WINDOW_MSI_ADDR (PCIE_WINDOW_START + 0x00020000U)
#define PCIE_WINDOW_MSI_MASK 0x0000FFFFU

/* SPI number (a block of reserved ARM GIC SPIs) to use for MSI) */
/* For AM65xx, use the reserved SPI 268 - 279 */
#define PCIE_SPI_BASE        (300) /* 268+32 = 300 */
#define PCIE_WINDOW_MSI_DATA (PCIE_SPI_BASE)

/* Inbound limit */
#define PCIE_INBOUND_MASK    0x0FFFFFFFU

/* BAR mask */
#define PCIE_BAR_MASK         0x0FFFFFFF

/* BAR Index PCie*/
#define PCIE_BAR_IDX_RC        1
#define PCIE_BAR_IDX_EP        1


/* PCIe Regions used in the example */
#define PCIE_IB_REGION_RC   0
#define PCIE_OB_REGION_RC   0
#define PCIE_IB_REGION_EP   0
#define PCIE_OB_REGION_EP   0


#ifdef EDMA
/* Synchronized token passing between two endpoints using EDMA IO */
#define PCIE_EXAMPLE_DMA_RC
#endif

#ifdef EDMA
/* Asynchronous block transfers using EDMA */
#define PCIE_EXAMPLE_DMA_EP
#endif

#if defined(EDMA)
/* 64KB EDMA transfer */
#define PCIE_EXAMPLE_LINE_SIZE      16384
#elif defined(UDMA)
/* 32KB UDMA transfer */
#define PCIE_EXAMPLE_LINE_SIZE      8192
#endif

#ifdef EDMA
/*****************************************************************************
 * Choose the type of EDMA transfer (Current options are "DMA" and "QDMA")
 *****************************************************************************/
#define EDMA_TYPE 0 /* DMA */
/* #define EDMA_TYPE 1 //QDMA */

#endif

#endif

