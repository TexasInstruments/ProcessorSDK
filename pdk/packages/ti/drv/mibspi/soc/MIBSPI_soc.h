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

/**
 *  @defgroup DRV_MIBSPI_SOC_MODULE MIBSPI SOC specific interface
 *  @ingroup DRV_MIBSPI_MODULE
 *
 *  @brief
 *  The section has a SoC specific interface definitions for the MIBSPI driver

 *  @{
 */

/**
 * @file   mibspi/soc/MIBSPI_soc.h
 *
 * @brief  MIBSPI LLD SoC specific file
 */

#ifndef MIBSPI_SOC_H
#define MIBSPI_SOC_H

#include <ti/csl/csl_utils.h>

#if defined(SOC_TPR12)
#include <ti/drv/mibspi/soc/tpr12/MIBSPI_soc.h>
#elif defined(SOC_AWR294X)
#include <ti/drv/mibspi/soc/awr294x/MIBSPI_soc.h>
#endif
#if defined(SOC_TPR12) || defined(SOC_AWR294X)
#include <ti/drv/mibspi/soc/MIBSPI_v0.h>
#define MIBSPI_HWAttrs    MIBSPI_v0_HWAttrs
#endif

#include <ti/osal/osal.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief  Get the SoC specific configuration for MIBSPI instance
 *
 *  @ingroup DRV_MIBSPI_SOC_MODULE
 *  @param  id  MIBSPI instance id
 *  @param  cfg MIBSPI SoC configuration to be populated
 *
 */
int32_t MIBSPI_socGetInitCfg(enum MibSpi_InstanceId id, MibSpi_HwCfg *cfg);

/*!
 *  @brief  Soc specific init function
 *
 */
void MIBSPI_socInit(void);

#ifdef __cplusplus
}
#endif

#endif /* SPI_SOC_H */
/* @} */

