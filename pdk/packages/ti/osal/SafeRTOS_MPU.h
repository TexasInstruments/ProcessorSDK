/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_SAFERTOS_MPU_CONFIG SafeRTOS MPU configurations
 *
 *  @{
 */ 

/**
 *  \file   SafeRTOS_MPU.h
 *
 **/
#ifndef SAFERTOS_MPU_H
#define SAFERTOS_MPU_H
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* Generic headers */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ti/csl/csl_types.h>

/* SafeRTOS includes */
#include "SafeRTOS_API.h"

#define CSL_ARM_R5F_MPU_REGIONS_MAX                         ((uint32_t) 16U)
#define CSL_ARM_R5_MPU_REGION_SIZE_32BYTE                   ((uint32_t) 32U)
/** \brief No access to any modes */
#define CSL_ARM_R5_ACC_PERM_NO_ACCESS                       ((uint32_t) 0x0U)
/** \brief Full access to privileged and user modes */
#define CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR                  ((uint32_t) 0x3U)
/** \brief  Cache Policy: Non-cacheable */
#define CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE               ((uint32_t) 0x0U)
/** \brief  Cache Policy: Write-back, write-allocate */
#define CSL_ARM_R5_CACHE_POLICY_WB_WA                       ((uint32_t) 0x1U)
/** \brief  This should be passed to configuration. */
#define CSL_ARM_R5_MEM_ATTR_MAX                             ((uint32_t) 0x7U)
#define CSL_ARM_R5_MPU_REGION_AC_XN_SHIFT                   (0x0000000CU)
#define CSL_ARM_R5_MPU_REGION_AC_AP_SHIFT                   (0x00000008U)
#define CSL_ARM_R5_MPU_REGION_AC_S_SHIFT                    (0x00000002U)
#define CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT                  (0x00000003U)
#define CSL_ARM_R5_MPU_REGION_AC_CB_SHIFT                   (0x00000000U)
#define CSL_ARM_R5_MPU_REGION_AC_B_SHIFT                    (0x00000000U)
#define CSL_ARM_R5_MPU_REGION_AC_C_SHIFT                    (0x00000001U)


/* The structure passed to xMPUConfigureGlobalRegion() to configure the kernel
 * with MPU global region id. */
typedef struct MPU_CONFIG_ACCESS
{
    portUInt32Type          ulexeNeverControl;
    portUInt32Type          ulaccessPermission;
    portUInt32Type          ulshareable;
    portUInt32Type          ulcacheable;
    portUInt32Type          ulcachePolicy;
    portUInt32Type          ulmemAttr;
}xMPU_CONFIG_ACCESS;

/* The structure passed to xMPUConfigureGlobalRegion() to configure the kernel
 * with MPU global region id. */
typedef struct MPU_CONFIG_PARAMETERS
{
    /**< Region number to configure.
     *   Range: 1 to (portmpuGLOBAL_CONFIGURABLE_REGION_LAST) */
    portUInt32Type          ulRegionNumber;
    /**< Region base address: 32 bytes aligned. */
    portUInt32Type          ulRegionBeginAddress;
    xMPU_CONFIG_ACCESS      xRegionAccess;
    portUInt32Type          ulRegionSize;
    portUInt32Type          ulSubRegionDisable;
}xMPU_CONFIG_PARAMETERS;

/* ========================================================================== */
/*                             Function Declaration                           */
/* ========================================================================== */

__attribute__((section(".startupCode"))) portBaseType xConfigureMPU(void);

#endif /* SAFERTOS_MPU_H */

/* @} */
