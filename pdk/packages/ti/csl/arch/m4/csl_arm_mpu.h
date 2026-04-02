    /**
 * @file  csl_arm_mpu.h
 *
 * @brief
 *  Header file containing various enumerations, structure definitions and function
 *  declarations for the ARM M4 MPU
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2020, Texas Instruments, Inc.
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

#ifndef CSL_ARM_M4_MPU_H_
#define CSL_ARM_M4_MPU_H_
#include <stdlib.h>
#include <stdint.h>
#include <ti/csl/csl_types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CSL_ARMM4MpuRegionCfg
{
    uint32_t regionId;
    /**< Region number to configure.
     *   Range: 0 to (CSL_ARM_M4F_MPU_REGIONS_MAX     1U) */
    uint32_t enable;
    /**< Region enable control.
     *   0: Memory region disabled.
     *   1: Memory region enabled. */
    uint32_t baseAddr;
    /**< Region base address: 32 bytes aligned. */
    uint32_t size;
    /**< Region size
     *   Refer #CSL_ArmM4MpuRegionSize */
    uint32_t subRegionEnable;
    /**< Sub-region enable
     *   See \ref CSL_ArmM4SubRegionDisable for details.
     *   Multiple sub-regions can disabled by ORing multiple masks from above */
    uint32_t exeNeverControl;
    /**< Region execute control.
     *   0: all instruction fetches enabled
     *   1: no instruction fetches enabled */
    uint32_t accessPermission;
    /**< Access permissions
      *  See \ref CSL_ArmM4AccessPerm for more details */
    uint32_t shareable;
    /**< Region share attribute.
     *   0: non-shared
     *   1: shared */
    uint32_t cacheable;
    /**< Region cache control.
     *   FALSE: Memory region is non-cached.
     *   TRUE:  Memory  region is cached. */
    uint32_t cachePolicy;
    /**< Inner and outer cache policy.
     *   This field is valid only if cacheable = TRUE
     *   See \ref CSL_ArmM4CachePolicy for more details */
    uint32_t memAttr;
    /**< Memory region attributes.
     *   This field is valid only if cacheable = FALSE
     *   See \ref CSL_ArmM4MemAttr for more details */
}CSL_ARMM4MpuRegionCfg_t;

#define CSL_ARM_M4_MPU_REGION_SIZE_32B   (4U)
/**< 32 bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_64B   (5U)
/**< 64 bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_128B  (6U)
/**< 128 bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_256B  (7U)
/**< 256 bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_512B  (8U)
/**< 512 bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_1KB   (9U)
/**< 1K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_2KB   (10U)
/**< 2K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_4KB   (11U)
/**< 4K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_8KB   (12U)
/**< 8K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_16KB  (13U)
/**< 16K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_32KB  (14U)
/**< 32K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_64KB  (15U)
/**< 64K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_128KB (16U)
/**< 128K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_256KB (17U)
/**< 256K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_512KB (18U)
/**< 512K bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_1MB   (19U)
/**< 1M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_2MB   (20U)
/**< 2M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_4MB   (21U)
/**< 4M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_8MB   (22U)
/**< 8M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_16MB  (23U)
/**< 16M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_32MB  (24U)
/**< 32M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_64MB  (25U)
/**< 64M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_128MB (26U)
/**< 128M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_256MB (27U)
/**< 256M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_512MB (28U)
/**< 512M bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_1GB   (29U)
/**< 1G bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_2GB   (30U)
/**< 2G bytes region size */
#define CSL_ARM_M4_MPU_REGION_SIZE_4GB   (31U)

/** \brief No accesses are permitted to MPU region */
#define CSL_ARM_M4_ACC_PERM_NO_ACCESS               ((uint32_t) 0x0U)
/** \brief Privileged accesses only */
#define CSL_ARM_M4_ACC_PERM_PRIV_RD_WR              ((uint32_t) 0x1U)
/** \brief Privileged read/write accesses and user read only */
#define CSL_ARM_M4_ACC_PERM_PRIV_RD_WR_USR_RD       ((uint32_t) 0x2U)
/** \brief Full access to privileged and user modes */
#define CSL_ARM_M4_ACC_PERM_PRIV_USR_RD_WR          ((uint32_t) 0x3U)
/** \brief Privileged read accesses only */
#define CSL_ARM_M4_ACC_PERM_PRIV_RD_ONLY            ((uint32_t) 0x5U)
/** \brief Read only accesses to privileged and user modes */
#define CSL_ARM_M4_ACC_PERM_PRIV_USR_RD             ((uint32_t) 0x6U)
/* @} */

/**
 *  \anchor CSL_ArmM4CachePolicy
 *  \name   Arm M4F MPU region cache policy.
 *  \brief  Inner and outer cache policy for R5 MPU region.
 *
 *  @{
 */
/** \brief  Cache Policy: Non-cacheable */
#define CSL_ARM_M4_CACHE_POLICY_NON_CACHEABLE               ((uint32_t) 0x0U)
/** \brief  Cache Policy: Write-back, write-allocate */
#define CSL_ARM_M4_CACHE_POLICY_WB_WA                       ((uint32_t) 0x1U)
/** \brief  Cache Policy: Write-through, no write-allocate */
#define CSL_ARM_M4_CACHE_POLICY_WT_NO_WA                    ((uint32_t) 0x2U)
/** \brief  Cache Policy: Write-back, no write-allocate */
#define CSL_ARM_M4_CACHE_POLICY_WB_NO_WA                    ((uint32_t) 0x3U)
/* @} */

/**
 *  \anchor CSL_ArmM4MemAttr
 *  \name   Arm M4F MPU region attributes.
 *  \brief  Memory attributes: TEX[2:0], C, and B encodings.
 *
 *  @{
 */
/** \brief  Memory type and cache policies: Strongly-ordered. */
#define CSL_ARM_M4_MEM_ATTR_STRONGLY_ORDERED               ((uint32_t) 0x0U)
/** \brief  Memory type and cache policies: Shareable Device. */
#define CSL_ARM_M4_MEM_ATTR_SHAREABLE                      ((uint32_t) 0x1U)
/** \brief  Memory type and cache policies:
            Outer and Inner write-through, no write-allocate. */
#define CSL_ARM_M4_MEM_ATTR_CACHED_WT_NO_WA                ((uint32_t) 0x2U)
/** \brief  Memory type and cache policies:
            Outer and Inner write-back, no write-allocate. */
#define CSL_ARM_M4_MEM_ATTR_CACHED_WB_NO_WA                ((uint32_t) 0x3U)
/** \brief  Memory type and cache policies: Non cacheable. */
#define CSL_ARM_M4_MEM_ATTR_STRONGLY_NON_CACHED            ((uint32_t) 0x4U)
/** \brief  Memory type and cache policies:
            Outer and Inner write-back, write-allocate. */
#define CSL_ARM_M4_MEM_ATTR_CACHED_WB_WA                   ((uint32_t) 0x5U)
/** \brief  Memory type and cache policies: Non-shareable Device. */
#define CSL_ARM_M4_MEM_ATTR_NON_SHAREABLE                  ((uint32_t) 0x6U)
/** \brief  This should be passed to configuration. */
#define CSL_ARM_M4_MEM_ATTR_MAX                            ((uint32_t) 0x7U)


/** \brief Enable all sub-regions */
#define CSL_ARM_M4_MPU_SUB_REGION_ENABLE_ALL              ((uint32_t) 0x0U)
/** \brief Disable sub-region 0 */
#define CSL_ARM_M4_MPU_SUB_REGION_0_DISABLE               ((uint32_t) 0x1U)
/** \brief Disable sub-region 1 */
#define CSL_ARM_M4_MPU_SUB_REGION_1_DISABLE               ((uint32_t) 0x2U)
/** \brief Disable sub-region 2 */
#define CSL_ARM_M4_MPU_SUB_REGION_2_DISABLE               ((uint32_t) 0x4U)
/** \brief Disable sub-region 3 */
#define CSL_ARM_M4_MPU_SUB_REGION_3_DISABLE               ((uint32_t) 0x8U)
/** \brief Disable sub-region 4 */
#define CSL_ARM_M4_MPU_SUB_REGION_4_DISABLE               ((uint32_t) 0x10U)
/** \brief Disable sub-region 5 */
#define CSL_ARM_M4_MPU_SUB_REGION_5_DISABLE               ((uint32_t) 0x20U)
/** \brief Disable sub-region 6 */
#define CSL_ARM_M4_MPU_SUB_REGION_6_DISABLE               ((uint32_t) 0x40U)
/** \brief Disable sub-region 7 */
#define CSL_ARM_M4_MPU_SUB_REGION_7_DISABLE               ((uint32_t) 0x80U)

/* Internal macro define for ARM M4 - no need to doxygen it */
#define CSL_ARM_M4_SCS_BASE_ADDRESS (0xE000ED00)
#define CSL_ARM_M4_MPU_BASE_ADDRESS (0xE000ED90)

typedef struct
{
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint32_t SHPR1;
    volatile uint32_t SHPR2;
    volatile uint32_t SHPR3;
    volatile uint32_t SHCSR;
    volatile uint32_t CFSR;
    volatile uint32_t HFSR;
    volatile uint32_t DFSR;
    volatile uint32_t MMFAR;
    volatile uint32_t BFAR;
    volatile uint32_t AFSR;
    volatile uint8_t  Resv_1[72]; //0X40-0X84
    volatile uint32_t CPACR;
}CSL_ARM_CSL_ARM_M4_SCS_Registers;

typedef struct
{
    volatile uint32_t MPU_TYPE;
    volatile uint32_t MPU_CTRL;
    volatile uint32_t MPU_RNR;
    volatile uint32_t MPU_RBAR;
    volatile uint32_t MPU_RASR;
}CSL_ARM_M4_MPU_Registers;

/* Structure type to access the System Control Block (SCB). */
typedef volatile struct CSL_SCB_Regs_t
{
    uint32_t    CPUID;          /*!< Offset: 0x000 (R/ )  CPUID Base Register */
    uint32_t    ICSR;           /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
    uint32_t    VTOR;           /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
    uint32_t    AIRCR;          /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
    uint32_t    SCR;            /*!< Offset: 0x010 (R/W)  System Control Register */
    uint32_t    CCR;            /*!< Offset: 0x014 (R/W)  Configuration Control Register */
    uint8_t     SHP[12U];       /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
    uint32_t    SHCSR;          /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
    uint32_t    CFSR;           /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register */
    uint32_t    HFSR;           /*!< Offset: 0x02C (R/W)  HardFault Status Register */
    uint32_t    DFSR;           /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */
    uint32_t    MMFAR;          /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */
    uint32_t    BFAR;           /*!< Offset: 0x038 (R/W)  BusFault Address Register */
    uint32_t    AFSR;           /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */
}CSL_SCB_Registers;

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#define CSL_ARM_M4_MPU_TYPE_SEPARATE_MASK             (0x00000001U)
#define CSL_ARM_M4_MPU_TYPE_SEPARATE_SHIFT            (0U)
#define CSL_ARM_M4_MPU_TYPE_DREGION_MASK              (0x0000FF00U)
#define CSL_ARM_M4_MPU_TYPE_DREGION_SHIFT             (8U)
#define CSL_ARM_M4_MPU_TYPE_IREGION_MASK              (0x00FF0000U)
#define CSL_ARM_M4_MPU_TYPE_IREGION_SHIFT             (16U)

#define CSL_ARM_M4_MPU_CTRL_ENABLE_MASK               (0x00000001U)
#define CSL_ARM_M4_MPU_CTRL_ENABLE_SHIFT              (0U)
#define CSL_ARM_M4_MPU_CTRL_HFNMIENA_MASK             (0x00000002U)
#define CSL_ARM_M4_MPU_CTRL_HFNMIENA_SHIFT            (1U)
#define CSL_ARM_M4_MPU_CTRL_PRIVDEFENA_MASK           (0x00000004U)
#define CSL_ARM_M4_MPU_CTRL_PRIVDEFENA_SHIFT          (2U)

#define CSL_ARM_M4_MPU_RNR_REGION_MASK                (0x000000FFU)
#define CSL_ARM_M4_MPU_RNR_REGION_SHIFT               (0U)

#define CSL_ARM_M4_MPU_RBAR_REGION_MASK               (0x0000000FU)
#define CSL_ARM_M4_MPU_RBAR_REGION_SHIFT              (0U)
#define CSL_ARM_M4_MPU_RBAR_VALID_MASK                (0x00000010U)
#define CSL_ARM_M4_MPU_RBAR_VALID_SHIFT               (4U)
#define CSL_ARM_M4_MPU_RBAR_ADDR_MASK                 (0xFFFFFFE0U)
#define CSL_ARM_M4_MPU_RBAR_ADDR_SHIFT                (5U)

#define CSL_ARM_M4_MPU_RASR_ATTRS_XN_MASK             (0x‭10000000‬U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_XN_SHIFT            (28U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_AP_MASK             (‭0x01000000‬U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_AP_SHIFT            (24U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_TEX_MASK            (‭0x00380000‬U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_TEX_SHIFT           (19U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_S_MASK              (‭0x00040000‬U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_S_SHIFT             (18U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_C_MASK              (‭0x00020000‬U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_C_SHIFT             (17U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_B_MASK              (‭0x00010000‬U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_B_SHIFT             (16U)
#define CSL_ARM_M4_MPU_RASR_ATTRS_CB_SHIFT            (16U)
#define CSL_ARM_M4_MPU_RASR_SUB_REGION_DISABLE_MASK   (0x0000FF00U)
#define CSL_ARM_M4_MPU_RASR_SUB_REGION_DISABLE_SHIFT  (8U)
#define CSL_ARM_M4_MPU_RASR_REGION_SIZE_MASK          (0x0000003FU)
#define CSL_ARM_M4_MPU_RASR_REGION_SIZE_SHIFT         (1U)
#define CSL_ARM_M4_MPU_RASR_REGION_EN_MASK            (0x00000001U)
#define CSL_ARM_M4_MPU_RASR_REGION_EN_SHIFT           (0U)

int32_t CSL_armM4MPUEnable(uint32_t enable);
int32_t CSL_armM4GetNumMPURegions(uint32_t *pNumRegions);
int32_t CSL_armM4MPUCfgRegion(const CSL_ARMM4MpuRegionCfg_t *pMpuCfg);

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif
