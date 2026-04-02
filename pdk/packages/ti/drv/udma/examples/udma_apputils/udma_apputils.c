/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2025
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
 *  \file udma_apputils.c
 *
 *  \brief Common UDMA application utility used in all UDMA example.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/udma/udma.h>
#include <ti/csl/csl_rat.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/udma/examples/udma_apputils/udma_apputils.h>
#if defined (BUILD_C7X)
#include <ti/csl/arch/csl_arch.h>
#endif
#if defined (BUILD_MCU)
#include <ti/csl/arch/r5/csl_arm_r5.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if defined (BUILD_MCU2_0)
#define REGION_ID (0x0)
#define MAIN_OCM_VIRT_BASE (0xD0000000U)
#endif
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if defined (BUILD_MCU2_0)
static uint32_t logBase2(uint32_t value)
{
    uint32_t retVal = 0U;
    while(value)
    {
        retVal++;
        value = value >> 1;
    }
    return retVal;
}
#endif
#endif

void Udma_appMainOcmRatCfg(void)
{
#if defined(SOC_J721E)
    /**
     *  Main OCMC falls in 32-bit address space in j721e,
     *  No need for RAT mapping
     */
#else
#if defined (BUILD_MCU2_0)
    /* Input Address */
    *(unsigned int *)(CSL_R5FSS0_RAT_CFG_BASE + 0x44 + (REGION_ID*0x10)) = MAIN_OCM_VIRT_BASE;
    /* Lower 32 bits Output Address */
    *(unsigned int *)(CSL_R5FSS0_RAT_CFG_BASE + 0x48 + (REGION_ID*0x10)) = (unsigned int)((CSL_MSRAM_512K0_RAM_BASE) & (0xFFFFFFFF));
    /* Upper 32 bits Output Address */
    *(unsigned int *)(CSL_R5FSS0_RAT_CFG_BASE + 0x4C + (REGION_ID*0x10)) = (unsigned int)((CSL_MSRAM_512K0_RAM_BASE >> 32) & (0xFFFFFFFF));
    /* Region Enable[31] + SIZE of memory[5:0] */
    *(unsigned int *)(CSL_R5FSS0_RAT_CFG_BASE + 0x40 + (REGION_ID*0x10)) = (CSL_RAT_REGION_CTRL_EN_MAX << CSL_RAT_REGION_CTRL_EN_SHIFT) |\
                                                            (logBase2(CSL_MSRAM_512K0_RAM_SIZE) << CSL_RAT_REGION_BASE_BASE_SHIFT);
#endif
#endif
}

void Udma_appUtilsCacheWb(const void *addr, int32_t size)
{
    uint32_t    isCacheCoherent = Udma_isCacheCoherent();

    if(UTRUE != isCacheCoherent)
    {
        CacheP_wb(addr, size);
    }

    return;
}

void Udma_appUtilsCacheInv(const void * addr, int32_t size)
{
    uint32_t    isCacheCoherent = Udma_isCacheCoherent();

    if(UTRUE != isCacheCoherent)
    {
        CacheP_Inv(addr, size);
    }

    return;
}

void Udma_appUtilsCacheWbInv(const void * addr, int32_t size)
{
    uint32_t    isCacheCoherent = Udma_isCacheCoherent();

    if(UTRUE != isCacheCoherent)
    {
        CacheP_wbInv(addr, size);
    }

    return;
}

uint64_t Udma_appVirtToPhyFxn(const void *virtAddr, uint32_t chNum, void *appData)
{
    uint64_t    phyAddr;
    uint64_t    atcmSizeLocal = 0U;
    uint64_t    atcmBaseGlobal = 0U;

#if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if defined (BUILD_MCU2_0)
    uint64_t    mainOcmcBaseLocal   = MAIN_OCM_VIRT_BASE;
    uint64_t    mainOcmcBaseGlobal  = CSL_MSRAM_512K0_RAM_BASE;
    uint64_t    mainOcmcSize = (512U * 1024U);
#endif
#endif

    phyAddr = (uint64_t) virtAddr;
#if defined (BUILD_C66X)
    /* Convert local L2RAM address to global space */
    if((phyAddr >= CSL_C66_COREPAC_L2_BASE) &&
       (phyAddr < (CSL_C66_COREPAC_L2_BASE + CSL_C66_COREPAC_L2_SIZE)))
    {
#if defined (BUILD_C66X_1)
        phyAddr -= CSL_C66_COREPAC_L2_BASE;
        phyAddr += CSL_C66SS0_C66_SDMA_L2SRAM_0_BASE;
#endif
#if defined (BUILD_C66X_2)
        phyAddr -= CSL_C66_COREPAC_L2_BASE;
        phyAddr += CSL_C66SS1_C66_SDMA_L2SRAM_0_BASE;
#endif
    }
#endif

    /* Convert local MCU domain R5 TCMA address to global space */
#if defined (SOC_AM65XX)
    atcmSizeLocal = CSL_MCU_ATCM_SIZE;
#if defined (BUILD_MCU1_0)
    atcmBaseGlobal = CSL_MCU_ARMSS0_CORE0_ATCM_BASE;
#endif
#if defined (BUILD_MCU1_1)
    atcmBaseGlobal = CSL_MCU_ARMSS0_CORE1_ATCM_BASE;
#endif
#endif

#if defined (SOC_J721E)  || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if defined (BUILD_MCU)
    CSL_ArmR5CPUInfo cpuInfo;
    CSL_armR5GetCpuID(&cpuInfo);

    if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            #if defined (SOC_J721E)
                atcmSizeLocal = CSL_MCU_ARMSS_ATCM_SIZE;
            #else
                atcmSizeLocal = CSL_MCU_R5FSS0_ATCM_SIZE;
            #endif
                atcmBaseGlobal = CSL_MCU_R5FSS0_CORE0_ATCM_BASE;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            #if defined (SOC_J721E)
                atcmSizeLocal = CSL_MCU_ARMSS_ATCM_SIZE;
            #else
                atcmSizeLocal = CSL_MCU_R5FSS0_ATCM_SIZE;
            #endif
                atcmBaseGlobal = CSL_MCU_R5FSS0_CORE1_ATCM_BASE;

        }
    }
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_1)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            #if defined (SOC_J721E)
                atcmSizeLocal = CSL_ARMSS_ATCM_BASE;
            #else
                atcmSizeLocal = CSL_R5FSS0_ATCM_SIZE; 
            #endif
                atcmBaseGlobal = CSL_R5FSS0_CORE0_ATCM_BASE;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            #if defined (SOC_J721E)
                atcmSizeLocal = CSL_ARMSS_ATCM_BASE;
            #else
                atcmSizeLocal = CSL_R5FSS0_ATCM_SIZE; 
            #endif
                atcmBaseGlobal = CSL_R5FSS0_CORE1_ATCM_BASE;
        }
    }
    #if defined (SOC_J721E) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_2)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            #if defined (SOC_J721E)
                atcmSizeLocal = CSL_ARMSS_ATCM_BASE;
            #else
                atcmSizeLocal = CSL_R5FSS0_ATCM_SIZE; 
            #endif
                atcmBaseGlobal = CSL_R5FSS1_CORE0_ATCM_BASE;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            #if defined (SOC_J721E)
                atcmSizeLocal = CSL_ARMSS_ATCM_BASE;
            #else
                atcmSizeLocal = CSL_R5FSS0_ATCM_SIZE; 
            #endif
                atcmBaseGlobal = CSL_R5FSS1_CORE1_ATCM_BASE;
        }
    }
    #endif
    #if defined (SOC_J784S4)
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_3)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
                atcmSizeLocal = CSL_R5FSS0_ATCM_SIZE; 
                atcmBaseGlobal = CSL_R5FSS2_CORE0_ATCM_BASE;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
                atcmSizeLocal = CSL_R5FSS0_ATCM_SIZE; 
                atcmBaseGlobal = CSL_R5FSS2_CORE1_ATCM_BASE;
        }
    }
    #endif
#endif
#endif

#if defined (SOC_AM64X)
#if defined (BUILD_MCU1_0)
    atcmSizeLocal = CSL_R5FSS0_ATCM_SIZE;
    atcmBaseGlobal = CSL_R5FSS0_CORE0_ATCM_BASE;
#endif
#if defined (BUILD_MCU1_1)
    atcmSizeLocal = CSL_R5FSS0_ATCM_SIZE;
    atcmBaseGlobal = CSL_R5FSS0_CORE1_ATCM_BASE;
#endif
#if defined (BUILD_MCU2_0)
    atcmSizeLocal = CSL_R5FSS1_ATCM_BASE;
    atcmBaseGlobal = CSL_R5FSS1_CORE0_ATCM_BASE;
#endif
#if defined (BUILD_MCU2_1)
    atcmSizeLocal = CSL_R5FSS1_ATCM_BASE;
    atcmBaseGlobal = CSL_R5FSS1_CORE1_ATCM_BASE;
#endif
#endif

     /* check for start address avoided since atcmBase is 0U */
    if(phyAddr < atcmSizeLocal) 
    {
        phyAddr += atcmBaseGlobal;
    }


#if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if defined (BUILD_MCU2_0)
    if((phyAddr >= mainOcmcBaseLocal) && (phyAddr < mainOcmcBaseLocal + mainOcmcSize))
    {
        phyAddr -= mainOcmcBaseLocal;
        phyAddr += mainOcmcBaseGlobal;
    }
#endif
#endif

    return (phyAddr);
}

void *Udma_appPhyToVirtFxn(uint64_t phyAddr, uint32_t chNum, void *appData)
{
    void       *virtAddr;

#if defined (__aarch64__) || defined (BUILD_C7X)
    virtAddr = (void *) phyAddr;
#else
    uint32_t temp;
    uint64_t    atcmBaseGlobal = 0U;
    uint64_t    atcmSizeGlobal = 0U;

#if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if defined (BUILD_MCU2_0)
    uint64_t    mainOcmcBaseLocal   = MAIN_OCM_VIRT_BASE;
    uint64_t    mainOcmcBaseGlobal  = CSL_MSRAM_512K0_RAM_BASE;
    uint64_t    mainOcmcSize        = (512U * 1024U);
#endif
#endif

    /* Convert global L2RAM address to local space */
#if defined (BUILD_C66X_1)
    if((phyAddr >= CSL_C66SS0_C66_SDMA_L2SRAM_0_BASE) &&
       (phyAddr < (CSL_C66SS0_C66_SDMA_L2SRAM_0_BASE + CSL_C66_COREPAC_L2_SIZE)))
    {
        phyAddr -= CSL_C66SS0_C66_SDMA_L2SRAM_0_BASE;
        phyAddr += CSL_C66_COREPAC_L2_BASE;
    }
#endif
#if defined (BUILD_C66X_2)
    if((phyAddr >= CSL_C66SS1_C66_SDMA_L2SRAM_0_BASE) &&
       (phyAddr < (CSL_C66SS1_C66_SDMA_L2SRAM_0_BASE + CSL_C66_COREPAC_L2_SIZE)))
    {
        phyAddr -= CSL_C66SS1_C66_SDMA_L2SRAM_0_BASE;
        phyAddr += CSL_C66_COREPAC_L2_BASE;
    }
#endif

/* Convert global TCMA address to local space */
#if defined (SOC_AM65XX)
#if defined (BUILD_MCU1_0)
    atcmBaseGlobal = CSL_MCU_ARMSS0_CORE0_ATCM_BASE;
    atcmSizeGlobal = CSL_MCU_ARMSS0_CORE0_ATCM_SIZE;
#endif
#if defined (BUILD_MCU1_1)
    atcmBaseGlobal = CSL_MCU_ARMSS0_CORE1_ATCM_BASE;
    atcmSizeGlobal = CSL_MCU_ARMSS0_CORE1_ATCM_SIZE;
#endif
#endif

#if defined (SOC_J721E)  || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if defined (BUILD_MCU)
    CSL_ArmR5CPUInfo cpuInfo;
    CSL_armR5GetCpuID(&cpuInfo);

    if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            atcmSizeGlobal = CSL_MCU_R5FSS0_CORE0_ATCM_SIZE;
            atcmBaseGlobal = CSL_MCU_R5FSS0_CORE0_ATCM_BASE;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            atcmSizeGlobal = CSL_MCU_R5FSS0_CORE1_ATCM_SIZE;
            atcmBaseGlobal = CSL_MCU_R5FSS0_CORE1_ATCM_BASE;
        }
    }
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_1)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            atcmSizeGlobal = CSL_R5FSS0_CORE0_ATCM_SIZE;
            atcmBaseGlobal = CSL_R5FSS0_CORE0_ATCM_BASE;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            atcmSizeGlobal = CSL_R5FSS0_CORE1_ATCM_SIZE;
            atcmBaseGlobal = CSL_R5FSS0_CORE1_ATCM_BASE;
        }
    }
    #if defined (SOC_J721E) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_2)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            atcmSizeGlobal = CSL_R5FSS1_CORE0_ATCM_SIZE;
            atcmBaseGlobal = CSL_R5FSS1_CORE0_ATCM_BASE;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            atcmSizeGlobal = CSL_R5FSS1_CORE1_ATCM_SIZE;
            atcmBaseGlobal = CSL_R5FSS1_CORE1_ATCM_BASE;
        }
    }
    #endif
    #if defined (SOC_J784S4)
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_3)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            atcmSizeGlobal = CSL_R5FSS2_CORE0_ATCM_SIZE;
            atcmBaseGlobal = CSL_R5FSS2_CORE0_ATCM_BASE;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            atcmSizeGlobal = CSL_R5FSS2_CORE1_ATCM_SIZE;
            atcmBaseGlobal = CSL_R5FSS2_CORE1_ATCM_BASE;
        }
    }
    #endif
#endif
#endif

#if defined (SOC_AM64X)
#if defined (BUILD_MCU1_0)
    atcmSizeGlobal = CSL_R5FSS0_CORE0_ATCM_SIZE;
    atcmBaseGlobal = CSL_R5FSS0_CORE0_ATCM_BASE;
#endif
#if defined (BUILD_MCU1_1)
    atcmSizeGlobal = CSL_R5FSS0_CORE1_ATCM_SIZE;
    atcmBaseGlobal = CSL_R5FSS0_CORE1_ATCM_BASE;
#endif
#if defined (BUILD_MCU2_0)
    atcmSizeGlobal = CSL_R5FSS1_CORE0_ATCM_SIZE;
    atcmBaseGlobal = CSL_R5FSS1_CORE0_ATCM_BASE;
#endif
#if defined (BUILD_MCU2_1)
    atcmSizeGlobal = CSL_R5FSS1_CORE1_ATCM_SIZE;
    atcmBaseGlobal = CSL_R5FSS1_CORE1_ATCM_BASE;
#endif
#endif

    if((phyAddr >= atcmBaseGlobal) &&
       (phyAddr < (atcmBaseGlobal + atcmSizeGlobal)))
    {
        phyAddr -= atcmBaseGlobal;
         /*start address not added since atcmBase is 0U */
    }

    /* R5/C66x is 32-bit; need to truncate to avoid void * typecast error */
    temp = (uint32_t) phyAddr;
    virtAddr = (void *) temp;

#if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if defined (BUILD_MCU2_0)
    if((phyAddr >= mainOcmcBaseGlobal) && (phyAddr < mainOcmcBaseGlobal + mainOcmcSize))
    {
        phyAddr -= mainOcmcBaseGlobal;
        phyAddr += mainOcmcBaseLocal;
    }
    temp = (uint32_t) phyAddr;
    virtAddr = (void *) temp;
#endif
#endif
#endif

    return (virtAddr);
}

uint32_t Udma_appIsPrintSupported(void)
{
    uint32_t retVal = UTRUE;

    /* Semi hosting not supported for MPU on Simulator */
#if (defined (BUILD_MPU) && defined (SIMULATOR))
    retVal = UFALSE;
#endif

    /* Printf doesn't work for MPU when run from SBL with no CCS connection
     * There is no flag to detect SBL or CCS mode. Hence disable the print
     * for MPU unconditionally */
#if defined (BUILD_MPU)
    retVal = UFALSE;
#endif

    return (retVal);
}

uint32_t Udma_appIsUdmapStatsSupported(void)
{
    uint32_t retVal = UTRUE;

#if defined (SOC_AM65XX)
    uint32_t jtagIdVal, variatn;

    jtagIdVal = CSL_REG32_RD(CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_JTAGID);
    variatn   = CSL_FEXT(jtagIdVal, WKUP_CTRL_MMR_CFG0_JTAGID_VARIATN);
    if (0U == variatn)
    {
        retVal = UFALSE;
    }
#endif

    return (retVal);
}
