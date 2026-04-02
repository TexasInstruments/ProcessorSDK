/*
 * SDR TEST
 *
 * Software Diagnostics Reference Test MPU default settings
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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

#include <stdlib.h>
#include <stdint.h>
#include <ti/csl/arch/r5/csl_arm_r5.h>
#include <ti/csl/arch/r5/csl_arm_r5_mpu.h>
#include <ti/csl/arch/r5/interrupt.h>
#include <ti/csl/arch/r5/csl_cache.h>

/*  Declarations    */
/**
 * \brief  TEX[2:0], C and B values.
 *         gSdrTestMemAttr[x][0]: TEX[2:0] values
 *         gSdrTestMemAttr[x][1]: C bit value
 *         gSdrTestMemAttr[x][2]: B bit value
 */
static const uint32_t gSdrTestMemAttr[CSL_ARM_R5_MEM_ATTR_MAX][3U] =
{
/*    TEX[2:0], C,     B bits */
    {   0x0U,   0x0U,  0x0U,}, /* Strongly-ordered.*/
    {   0x0U,   0x0U,  0x1U,}, /* Shareable Device.*/
    {   0x0U,   0x1U,  0x0U,}, /* Outer and Inner write-through, no write-allocate. */
    {   0x0U,   0x1U,  0x1U,}, /* Outer and Inner write-back, no write-allocate. */
    {   0x1U,   0x0U,  0x0U,}, /* Outer and Inner Non-cacheable. */
    {   0x1U,   0x1U,  0x1U,}, /* Outer and Inner write-back, write-allocate.*/
    {   0x2U,   0x0U,  0x0U,}, /* Non-shareable Device.*/
};

const CSL_ArmR5MpuRegionCfg  gSdrR5MpuCfg[CSL_ARM_R5F_MPU_REGIONS_MAX] =
{
    {
        /* Region 0 configuration: complete 32 bit address space = 4Gbits */
        .regionId         = 0U,
        .enable           = 1U,
        .baseAddr         = 0x0U,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_4GB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = 0U,
        .memAttr          = 0U,
    },
    {
        /* Region 1 configuration: 128 bytes memory for exception vector execution */
        .regionId         = 1U,
        .enable           = 1U,
        .baseAddr         = 0x0U,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_128B,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 2 configuration: 512 KB OCMS RAM */
        .regionId         = 2U,
        .enable           = 1U,
        .baseAddr         = 0x41C00000,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_512KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 3 configuration: 2 MB MCMS3 RAM */
        .regionId         = 3U,
        .enable           = 1U,
        .baseAddr         = 0x70000000,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_2MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 4 configuration: 2 GB DDR RAM */
        .regionId         = 4U,
        .enable           = 1U,
        .baseAddr         = 0x80000000,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_2GB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 5 configuration: 64 KB BTCM */
        .regionId         = 5U,
        .enable           = 1U,
        .baseAddr         = 0x41010000,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_64KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
        .memAttr          = 0U,
    },
    {
        /* Region 6 configuration: 128 MB FSS DAT */
        .regionId         = 6U,
        .enable           = 0U,
        .baseAddr         = 0x50000000,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_128MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 7 configuration: Test  memory area */
        .regionId         = 7U,
        .enable           = 1U,
        .baseAddr         = 0x7C00U,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_128B,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
};

void SDR_armR5MPUCfg(void);

static void SDR_enable_cache()
{
    CSL_armR5CacheEnableAllCache( 0 );      /* Disable I/D caches */
    CSL_armR5CacheEnableForceWrThru( 1 );   /* Enable force write-thru */
    CSL_armR5CacheInvalidateAllCache();     /* Invalidate I/D caches */
    CSL_armR5CacheEnableAllCache( 1 );      /* Enable I/D caches */
}

/*****************************************************************************/
/* \brief __MPU_INIT() - __mpu_init() is called in the C/C++ startup routine,*/
/* _c_int00(), and provides a mechanism for tailoring mpu init by device     */
/* prior to calling main().                                                  */
/*                                                                           */
/*****************************************************************************/
/*  Usage notes:
    On entry to this function from boot, R5F must be in System (privileged) mode. */
void __mpu_init(void)
{
    uint32_t loopCnt = 0U, regAddr;
    CSL_ArmR5CPUInfo info;

    CSL_armR5GetCpuID(&info);
    if (info.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        /* MCU SS Pulsar R5 SS */
        regAddr = CSL_MCU_DOMAIN_VIM_BASE_ADDR;
    }
    else
    {
        /* MAIN SS Pulsar R5 SS */
        regAddr = CSL_MAIN_DOMAIN_VIM_BASE_ADDR;
    }

    SDR_armR5MPUCfg();          /* Enable MPU */
#ifndef SDTF_CACHE_DISABLE
    SDR_enable_cache();         /* Enable all caches */
#endif
    CSL_armR5FpuEnable( 1 );    /* Enable FPU */
    CSL_armR5IntrEnableVic(1);  /* Enable VIC */
    /* Disable/Clear pending Interrupts in VIM before enabling CPU Interrupts */
    /* This is done to prevent serving any bogus interrupt */
    for (loopCnt = 0U ; loopCnt < R5_VIM_INTR_NUM; loopCnt++)
    {
        /* Disable interrupt in vim */
        CSL_vimSetIntrEnable((CSL_vimRegs *)(uintptr_t)regAddr, loopCnt, false);
        /* Clear interrupt status */
        CSL_vimClrIntrPending((CSL_vimRegs *)(uintptr_t)regAddr, loopCnt);
    }
    CSL_armR5IntrEnableFiq(1);  /* Enable FIQ */
    CSL_armR5IntrEnableIrq(1);  /* Enable IRQ */
}

void SDR_armR5MPUCfg(void)
{
    uint32_t loopCnt = 0U;
    uint32_t baseAddrRegVal = 0U, sizeRegVal = 0U, accessCtrlRegVal = 0U, tex;

    CSL_armR5CacheInvalidateAllCache();     /* Invalidate caches */
    CSL_armR5CacheEnableDCache(0);          /* Disable D-cache */
    /* Disable MPU */
    CSL_armR5MpuEnable(0U);
    /* Disable all MPU regions */
    for (loopCnt = 0U ; loopCnt < CSL_ARM_R5F_MPU_REGIONS_MAX ; loopCnt++)
    {
        CSL_armR5MpuCfgRegion(loopCnt,
                              baseAddrRegVal,
                              sizeRegVal,
                              accessCtrlRegVal);
    }

    /* Configure MPU regions only for provided configuration */
    for (loopCnt = 0U ; loopCnt < CSL_ARM_R5F_MPU_REGIONS_MAX ; loopCnt++)
    {
        if (CSL_ARM_R5_MPU_REGION_SIZE_32B <= gSdrR5MpuCfg[loopCnt].size)
        {
            baseAddrRegVal = 0U;
            sizeRegVal = 0U;
            accessCtrlRegVal = 0U;
            baseAddrRegVal   |= ( gSdrR5MpuCfg[loopCnt].baseAddr &
                                    CSL_ARM_R5_MPU_REGION_BASE_ADDR_MASK);
            sizeRegVal       |= ( gSdrR5MpuCfg[loopCnt].enable <<
                                    CSL_ARM_R5_MPU_REGION_SZEN_EN_SHIFT);
            sizeRegVal       |= ( gSdrR5MpuCfg[loopCnt].size <<
                                    CSL_ARM_R5_MPU_REGION_SZEN_SZ_SHIFT);
            sizeRegVal       |= ( gSdrR5MpuCfg[loopCnt].subRegionEnable <<
                                    CSL_ARM_R5_MPU_REGION_SZEN_SRD_SHIFT);
            accessCtrlRegVal |= ( gSdrR5MpuCfg[loopCnt].exeNeverControl <<
                                   CSL_ARM_R5_MPU_REGION_AC_XN_SHIFT);
            accessCtrlRegVal |= ( gSdrR5MpuCfg[loopCnt].accessPermission <<
                                   CSL_ARM_R5_MPU_REGION_AC_AP_SHIFT);
            accessCtrlRegVal |= ( gSdrR5MpuCfg[loopCnt].shareable <<
                                   CSL_ARM_R5_MPU_REGION_AC_S_SHIFT);
            if (gSdrR5MpuCfg[loopCnt].cacheable == (uint32_t)TRUE)
            {
                tex = (1U << 2U);
                tex |= (gSdrR5MpuCfg[loopCnt].cachePolicy);
                accessCtrlRegVal |=
                                ( tex << CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT);
                accessCtrlRegVal |= ( gSdrR5MpuCfg[loopCnt].cachePolicy <<
                                   CSL_ARM_R5_MPU_REGION_AC_CB_SHIFT);
            }
            else
            {
                tex = gSdrTestMemAttr[gSdrR5MpuCfg[loopCnt].memAttr][0U];
                accessCtrlRegVal |=
                                ( tex << CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT);
                accessCtrlRegVal |=
                                ( gSdrTestMemAttr[gSdrR5MpuCfg[loopCnt].memAttr][1U] <<
                                CSL_ARM_R5_MPU_REGION_AC_B_SHIFT);
                accessCtrlRegVal |=
                                ( gSdrTestMemAttr[gSdrR5MpuCfg[loopCnt].memAttr][2U] <<
                                CSL_ARM_R5_MPU_REGION_AC_C_SHIFT);
            }
            /* configure MPU region here */
            CSL_armR5MpuCfgRegion(gSdrR5MpuCfg[loopCnt].regionId,
                                  baseAddrRegVal,
                                  sizeRegVal,
                                  accessCtrlRegVal);
        }
    }

    CSL_armR5CacheInvalidateAllCache();     /* Invalidate caches */
    CSL_armR5CacheEnableDCache(0);          /* Disable D-cache */
    /* Enable MPU */
    CSL_armR5MpuEnable(1U);
}
