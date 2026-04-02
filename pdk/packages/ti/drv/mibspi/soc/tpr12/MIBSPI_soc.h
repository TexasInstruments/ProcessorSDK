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
 *  \ingroup DRV_MIBSPI_SOC_MODULE
 *            
 *  @{
 */

/**
 * @file   mibspi/soc/tpr12/MIBSPI_soc.h
 *
 * @brief  MIBSPI driver TPR12 specific interface
 */

#ifndef MIBSPI_SOC_TPR12_H
#define MIBSPI_SOC_TPR12_H


#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief
 *  Definitions for  MIBSPI instances available on TPR12 SoC
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
enum MibSpi_InstanceId
{
    MIBSPI_INST_ID_MSS_SPIA  = 0xA,
    MIBSPI_INST_ID_MSS_SPIB  = 0xB,
    MIBSPI_INST_ID_RCSS_SPIA = 0x1A,
    MIBSPI_INST_ID_RCSS_SPIB = 0x1B
};

#ifdef __cplusplus
}
#endif

#endif /* MIBSPI_SOC_H */

/* @} */

