/**
 *  \ingroup DRV_SPI_MODULE
 *  \defgroup DRV_SPI_SOC_MODULE SOC APIS
 *            Multi-Channel SPI driver interface
 *  @{
 */

/**
 * @file   SPI_soc.h
 *
 * @brief  SPI SoC level driver
 */
/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
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

#ifndef SPI_SOC_H
#define SPI_SOC_H

#include <ti/csl/csl_utils.h>
#if defined(SOC_K2H) || defined(SOC_K2K) || defined(SOC_K2L) || defined(SOC_K2E) || defined(SOC_K2G) || defined(SOC_C6678) || defined(SOC_C6657) || defined(SOC_OMAPL137) || defined(SOC_OMAPL138)
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/src/v0/SPI_v0.h>
#include <ti/drv/spi/src/v0/QSPI_v0.h>
#include <ti/csl/src/ip/spi/V0/spi.h>
#define SPI_HWAttrs    SPI_v0_HWAttrs
#endif

#if defined(SOC_DRA78x) || defined(SOC_TDA3XX) || defined(SOC_TDA2XX) || defined(SOC_TDA2PX) || defined(SOC_TDA2EX) || defined (SOC_DRA72x) || defined (SOC_DRA75x) || defined (SOC_AM574x) || defined (SOC_AM572x) || defined (SOC_AM571x) || defined (SOC_AM437x) || defined (SOC_AM335x) || defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/drv/spi/MCSPI.h>
#include <ti/drv/spi/soc/SPI_v1.h>
#include <ti/csl/src/ip/mcspi/V0/hw_mcspi.h>
#include <ti/csl/src/ip/mcspi/V0/mcspi.h>

#if !(defined(SOC_AM65XX) || defined(SOC_J721E)|| defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2))
#include <ti/drv/spi/soc/QSPI_v1.h>
#endif

#define SPI_HWAttrs    SPI_v1_HWAttrs
#endif

#if defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/drv/spi/src/v0/OSPI_v0.h>
#endif

#if defined (SOC_TPR12) || defined (SOC_AWR294X)
#include <ti/drv/spi/soc/QSPI_v1.h>
#endif

#include <ti/osal/osal.h>

#ifdef __cplusplus
extern "C" {
#endif

  /* SPI SoC level API */
#if defined(SOC_K2H) || defined(SOC_K2K) || defined(SOC_K2L) || defined(SOC_K2E) || defined(SOC_K2G) || defined(SOC_C6678) || defined(SOC_C6657)
extern int32_t SPI_socGetInitCfg(uint32_t index, SPI_v0_HWAttrs *cfg);
extern int32_t SPI_socSetInitCfg(uint32_t index, const SPI_v0_HWAttrs *cfg);
extern int32_t QSPI_socGetInitCfg(uint32_t index, QSPI_v0_HwAttrs *cfg);
extern int32_t QSPI_socSetInitCfg(uint32_t index, const QSPI_v0_HwAttrs *cfg);
extern void SPI_getEdmaInfo(uint32_t index, uint32_t *edma3Id, uint32_t *edma3Event);
#define SPI_HWAttrs SPI_v0_HWAttrs
#endif

#if defined(SOC_OMAPL137) || defined(SOC_OMAPL138)
extern int32_t SPI_socGetInitCfg(uint32_t index, SPI_v0_HWAttrs *cfg);
extern int32_t SPI_socSetInitCfg(uint32_t index, const SPI_v0_HWAttrs *cfg);
#define SPI_HWAttrs SPI_v0_HWAttrs
#endif

#if defined(SOC_DRA78x) || defined(SOC_TDA3XX) || defined(SOC_TDA2XX) || defined(SOC_TDA2PX) || defined(SOC_TDA2EX) || defined (SOC_DRA72x) || defined (SOC_DRA75x) || defined (SOC_AM574x) || defined (SOC_AM572x) || defined (SOC_AM571x) || defined (SOC_AM437x) || defined (SOC_AM335x) || defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
extern int32_t SPI_socGetInitCfg(uint32_t domain, uint32_t idx, SPI_v1_HWAttrs *cfg);
extern int32_t SPI_socSetInitCfg(uint32_t domain, uint32_t idx, const SPI_v1_HWAttrs *cfg);
extern int32_t MCSPI_socInit(void);
extern int32_t MCSPI_configSocIntrPath(uint32_t instance, void *hwAttrs_ptr, bool setIntrPath);
#endif

#if defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
extern int32_t OSPI_socGetInitCfg(uint32_t domain, uint32_t idx, OSPI_v0_HwAttrs *cfg);
extern int32_t OSPI_socSetInitCfg(uint32_t domain, uint32_t idx, const OSPI_v0_HwAttrs *cfg);
extern int32_t OSPI_socInit(void);
extern int32_t OSPI_configSocIntrPath(void *hwAttrs_ptr, bool setIntrPath);
#define OSPI_MODULE_CLK_133M   (133333333U)
#define OSPI_MODULE_CLK_166M   (166666666U)
#define OSPI_MODULE_CLK_160M   (160000000U)
#define OSPI_MODULE_CLK_200M   (200000000U)
#define OSPI_MODULE_CLOCK      OSPI_MODULE_CLK_133M

#define OSPI_DEV_DELAY_CSDA_2  (0x2U)  /* Chip Select De-Assert device delay in # of ref_clk */
#define OSPI_DEV_DELAY_CSDA_3  (0x3U)
#define OSPI_DEV_DELAY_CSDA_A  (0xAU)
#define OSPI_DEV_DELAY_CSDA    OSPI_DEV_DELAY_CSDA_2

#define SPI_MCSPI_DOMAIN_MCU    (0U)
#define SPI_MCSPI_DOMAIN_MAIN   (1U)

#define SPI_OSPI_DOMAIN_MCU     (0U)
#define SPI_OSPI_DOMAIN_MAIN    (1U)

#define SPI_OSPI_PER_CNT        (CSL_OSPI_PER_CNT)
#define SPI_OSPI_DOMAIN_CNT     (CSL_OSPI_DOMAIN_CNT)

#endif

#if defined(SOC_AM335x) || defined(SOC_AM437x)
#include <ti/drv/spi/soc/ESPI_v2.h>
extern int32_t ESPI_socGetFwCfg(uint32_t index, ESPI_SwIPAttrs *cfg);
extern int32_t ESPI_socSetFwCfg(uint32_t index, const ESPI_SwIPAttrs *cfg);
extern void ESPI_socInitFwCfg(void);
#endif

#if defined (SOC_TPR12) || defined (SOC_AWR294X)
extern int32_t QSPI_socGetInitCfg(uint32_t idx, QSPI_HwAttrs *cfg);
extern int32_t QSPI_socSetInitCfg(uint32_t idx, const QSPI_HwAttrs *cfg);
#define QSPI_MODULE_CLK_80M   (80000000U)
#endif

#define SPI_MODULE_CLOCK_DIVIDER    (6U)

#ifdef __cplusplus
}
#endif

#endif /* SPI_SOC_H */
