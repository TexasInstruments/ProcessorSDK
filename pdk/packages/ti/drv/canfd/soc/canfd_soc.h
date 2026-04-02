/*!
 *
 *      (C) Copyright 2020 Texas Instruments, Inc.
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

/**
 *  @defgroup DRV_CANFD_SOC_MODULE CANFD SOC specific interface
 *  @ingroup DRV_CANFD_MODULE
 *
 *  @brief
 *  The section has a SoC specific interface definitions for the CANFD driver

 *  @{
 */

/**
 *   @file  canfd/soc/canfd_soc.h
 *
 *   @brief
 *      This is an header file used by the CANFD module and has SoC specific
 *      configuration.
 */

#ifndef CANFD_SOC_H
#define CANFD_SOC_H

#include <ti/drv/canfd/canfd.h>

#if defined(SOC_TPR12)
#include <ti/drv/canfd/soc/tpr12/canfd_soc.h>
#elif defined(SOC_AWR294X)
#include <ti/drv/canfd/soc/awr294x/canfd_soc.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief
 *  CANFD Hardware Atrributes
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 */
typedef struct CANFD_HwCfg_t
{
    /*!
     * @brief   Instance identifier for which this hardware configuration is used.
     */
    uint8_t                 instanceId;

    /*!
     * @brief   Base address of the register address space to be used.
     */
    uint32_t                regBaseAddress;

    /*!
     * @brief   Interrupt level 0 Number
     */
    uint32_t                interruptNum0;

    /*!
     * @brief   Interrupt level 1 Number
     */
    uint32_t                interruptNum1;
}CANFD_HwCfg;




#ifdef __cplusplus
}
#endif

#endif /* CANFD_SOC_H */
/* @} */
