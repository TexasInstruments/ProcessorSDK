/*
 *  Copyright ( C ) 2022 Texas Instruments Incorporated
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/* Generic headers */
#include <stdlib.h>
#include <string.h>

/* SafeRTOS includes */
#include "SafeRTOS_API.h"
#include "mpuARM.h"

#include <ti/osal/TimerP.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/src/nonos/Nonos_config.h>

/*-----------------------------------------------------------------------------
 * Local Constants
 *---------------------------------------------------------------------------*/

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


/*-----------------------------------------------------------------------------
 * Local Prototypes
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * External Prototypes
 *---------------------------------------------------------------------------*/

/* Set DLFO bit is defined in portasm.S. */
void vPortSetDLFOBit( void );

/*-----------------------------------------------------------------------------
 * Local Variables
 *---------------------------------------------------------------------------*/

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

/**
 * \brief  TEX[2:0], C and B values.
 *         CSL_ArmR5MemAttr is used as intex here.
 *         gMemAttr[x][0]: TEX[2:0] values
 *         gMemAttr[x][1]: C bit value
 *         gMemAttr[x][2]: B bit value
 */
__attribute__((section(".startupData"))) static uint32_t gMemAttr[CSL_ARM_R5_MEM_ATTR_MAX][3U] =
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

__attribute__((section(".startupData"))) xMPU_CONFIG_PARAMETERS gMPUConfigParms[] =
{
    {
        /* Region 0 configuration: complete 32 bit address space = 4Gbits add one more 2gb */
        /* ulRegionNumber */
        .ulRegionNumber         = 1U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x0U,
        /* Access permission */
        {
            .ulexeNeverControl  = 1U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 0U,
            .ulcachePolicy      = 0U,
            .ulmemAttr          = 0U,
        },
        /* TODO region size is 4GB, but 2GB is largest supported */
        .ulRegionSize           = portmpuLARGEST_REGION_SIZE_ACTUAL,
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = mpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 0 configuration: second half of 2 GB */
        /* ulRegionNumber */
        .ulRegionNumber         = 2U,
        /* Starting address */
        .ulRegionBeginAddress   = portmpuLARGEST_REGION_SIZE_ACTUAL,
        /* Access permission */
        {
            .ulexeNeverControl  = 1U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 0U,
            .ulcachePolicy      = 0U,
            .ulmemAttr          = 0U,
        },
        /* TODO region size is 4GB, but 2GB is largest supported */
        .ulRegionSize           = portmpuLARGEST_REGION_SIZE_ACTUAL,
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = mpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 1 configuration: 128 bytes memory for exception vector execution */
        /* ulRegionNumber */
        .ulRegionNumber         = 3U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x0U,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
            .ulmemAttr          = 0U,
        },
        /* ulRegionSize */
        .ulRegionSize           = 128U,
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = mpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 2 configuration: 1MB KB MCU MSRAM */
        /* ulRegionNumber */
        .ulRegionNumber         = 4U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x41C00000,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
            .ulmemAttr          = 0U,
        },
        /* Size is 512KB */
        .ulRegionSize           = (512U * 1024U),
#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)
        /* Size is 1MB */
        .ulRegionSize           = (1024U * 1024U),
#endif
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = mpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 3 configuration: 2 MB MCMS3 RAM */
        /* ulRegionNumber */
        .ulRegionNumber         = 5U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x70000000,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
            .ulmemAttr          = 0U,
        },
        /* Size is 2MB */
        .ulRegionSize           = (2U * 1024U * 1024U),
#if defined (SOC_J721E)
        /* Size is 8MB */
        .ulRegionSize           = (8U * 1024U * 1024U),
#endif

#if defined (SOC_J7200)
        /* Size is 1MB */
        .ulRegionSize           = (1U * 1024U * 1024U),
#endif

#if defined (SOC_J721S2)
        /* Size is 4MB */
        .ulRegionSize           = (4U * 1024U * 1024U),
#endif
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = mpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 4 configuration: 2 GB DDR RAM */
        /* ulRegionNumber */
        .ulRegionNumber         = 6U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x80000000,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
            .ulmemAttr          = 0U,
        },
        /* size is 2GB */
        .ulRegionSize           = portmpuLARGEST_REGION_SIZE_ACTUAL,
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = mpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 5 configuration: 32 KB BTCM */
        /* Address of ATCM/BTCM are configured via MCU_SEC_MMR registers
           It can either be '0x0' or '0x41010000'. Application/Boot-loader shall
           take care this configurations and linker command file shall be
           in sync with this. For either of the above configurations,
           MPU configurations will not changes as both regions will have same
           set of permissions in almost all scenarios.
           Application can chose to overwrite this MPU configuration if needed.
           The same is true for the region corresponding to ATCM. */
        /* ulRegionNumber */
        .ulRegionNumber         = 7U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x41010000,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
            .ulmemAttr          = 0U,
        },
        /* size is 32KB */
        .ulRegionSize           = (32U * 1024U),
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = mpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
    {
        /* Region 6 configuration: 32 KB ATCM */
        /* ulRegionNumber */
        .ulRegionNumber         = 8U,
        /* Starting address */
        .ulRegionBeginAddress   = 0x0,
        /* Access permission */
        {
            .ulexeNeverControl  = 0U,
            .ulaccessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
            .ulshareable        = 0U,
            .ulcacheable        = 1U,
            .ulcachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
            .ulmemAttr          = 0U,
        },
        /* size is 32KB */
        .ulRegionSize           = (32U * 1024U),
        /* ulSubRegionDisable */
        .ulSubRegionDisable     = mpuREGION_ALL_SUB_REGIONS_ENABLED,
    },
};

/*-----------------------------------------------------------------------------
 * Private function definitions.
 *---------------------------------------------------------------------------*/

__attribute__((section(".startupCode"))) static portUInt32Type xConfigureMPUAccessCtrl(xMPU_CONFIG_ACCESS *xMPUconfigAccess)
{
    portUInt32Type accessCtrlRegVal = 0, tex;

    accessCtrlRegVal |= ( xMPUconfigAccess->ulexeNeverControl <<
                           CSL_ARM_R5_MPU_REGION_AC_XN_SHIFT);
    accessCtrlRegVal |= ( xMPUconfigAccess->ulaccessPermission <<
                           CSL_ARM_R5_MPU_REGION_AC_AP_SHIFT);
    accessCtrlRegVal |= ( xMPUconfigAccess->ulshareable <<
                           CSL_ARM_R5_MPU_REGION_AC_S_SHIFT);
    if (xMPUconfigAccess->ulcacheable == 1U)
    {
        tex = (1U << 2U);
        tex |= (xMPUconfigAccess->ulcachePolicy);
        accessCtrlRegVal |=
                        ( tex << CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT);
        accessCtrlRegVal |= ( xMPUconfigAccess->ulcachePolicy <<
                           CSL_ARM_R5_MPU_REGION_AC_CB_SHIFT);
    }
    else
    {
        tex = gMemAttr[xMPUconfigAccess->ulmemAttr][0U];
        accessCtrlRegVal |=
                        ( tex << CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT);
        accessCtrlRegVal |=
                        ( gMemAttr[xMPUconfigAccess->ulmemAttr][1U] <<
                        CSL_ARM_R5_MPU_REGION_AC_B_SHIFT);
        accessCtrlRegVal |=
                        ( gMemAttr[xMPUconfigAccess->ulmemAttr][2U] <<
                        CSL_ARM_R5_MPU_REGION_AC_C_SHIFT);
    }
    return accessCtrlRegVal;
}

__attribute__((section(".startupCode"))) static portBaseType xConfigureMPU()
{
    portBaseType xInitMpuResult;
    uint32_t i;
    xMPU_CONFIG_PARAMETERS *xMPUconfig;
    xMPU_CONFIG_ACCESS     *xMPUconfigAccess;
    portUInt32Type         ulRegionAccess;

    for(i = 0; i < (sizeof(gMPUConfigParms) / sizeof(gMPUConfigParms[0])); i++)
    {
        ulRegionAccess = 0;
        xMPUconfig = &gMPUConfigParms[i];

        /* Update access control */
        xMPUconfigAccess = &xMPUconfig->xRegionAccess;

        ulRegionAccess = xConfigureMPUAccessCtrl(xMPUconfigAccess);

        xInitMpuResult = xMPUConfigureGlobalRegion(
                xMPUconfig->ulRegionNumber,
                xMPUconfig->ulRegionBeginAddress,
                ulRegionAccess,
                xMPUconfig->ulRegionSize,
                xMPUconfig->ulSubRegionDisable
                );
        if(pdPASS != xInitMpuResult)
        {
            break;
        }
    }

    return xInitMpuResult;
}

/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Routines
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Override functions for CSL startup code.
 *---------------------------------------------------------------------------*/

/*****************************************************************************/
/* \brief __MPU_INIT() - __mpu_init() is called in the C/C++ startup routine,*/
/* _c_int00(), and provides a mechanism for tailoring mpu init by device     */
/* prior to calling main().                                                  */
/*                                                                           */
/*****************************************************************************/
/*  Usage notes:
    On entry to this function from boot, R5F must be in System (privileged) mode. */

__attribute__((section(".startupCode"))) void __mpu_init( void )
{
    portBaseType xInitMpuResult;

    /* Override the startup code mpu initialisation. */

    /* Region 0 configuration: whole 4 GB */
    xInitMpuResult = xMPUConfigureWholeMemory( 0U,
                                                   mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE,
                                                   0U );

    if( pdPASS == xInitMpuResult )
    {
        xInitMpuResult = xConfigureMPU();
    }

    /* Configure the regions in the MPU that are common to all tasks. */
    vMPUSetupMPU();
}
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
/* _SYSTEM_PRE_INIT() - _system_pre_init() is called in the C/C++ startup    */
/* routine (_c_int00()) and provides a mechanism for the user to             */
/* insert application specific low level initialization instructions prior   */
/* to calling main().  The return value of _system_pre_init() is used to     */
/* determine whether or not C/C++ global data initialization will be         */
/* performed (return value of 0 to bypass C/C++ auto-initialization).        */
/*                                                                           */
/* PLEASE NOTE THAT BYPASSING THE C/C++ AUTO-INITIALIZATION ROUTINE MAY      */
/* RESULT IN PROGRAM FAILURE.                                                */
/*                                                                           */
/* The version of _system_pre_init() below is skeletal and is provided to    */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/
__attribute__((section(".startupCode")))  portBaseType _system_pre_init( void )
{
    return 1;
}
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
/* _SYSTEM_POST_CINIT() - _system_post_cinit() is a hook function called in  */
/* the C/C++ auto-initialization function after cinit() and before pinit().  */
/*                                                                           */
/* The version of _system_post_cinit() below is skeletal and is provided to  */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* __TI_default_system_post_cinit indicates that the default                 */

__attribute__((section(".startupCode")))  void _system_post_cinit( void )
{
    osalArch_Config_t cfg;

    cfg.disableIrqOnInit = BTRUE;
    osalArch_Init(&cfg);

    uint32_t            loopCnt = 0U, regAddr;
    CSL_ArmR5CPUInfo    info;
    uint32_t            maxIntrs;
    CSL_vimRegs         *pRegs;

    CSL_armR5GetCpuID(&info);

#if defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR0) && defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR1)
    if (info.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        /* MCU SS Pulsar R5 SS */
        regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                 CSL_MCU_DOMAIN_VIM_BASE_ADDR0:
                                 CSL_MCU_DOMAIN_VIM_BASE_ADDR1;

    }
    else
    {
        /* MAIN SS Pulsar R5 SS */
        regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                 CSL_MAIN_DOMAIN_VIM_BASE_ADDR0:
                                 CSL_MAIN_DOMAIN_VIM_BASE_ADDR1;

    }
#else
    /* MCU SS Pulsar R5 SS */
    regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                             CSL_MCU_DOMAIN_VIM_BASE_ADDR0:
                             CSL_MCU_DOMAIN_VIM_BASE_ADDR1;
#endif
    pRegs       = (CSL_vimRegs *)(uintptr_t) regAddr;
    maxIntrs    = pRegs->INFO;

#if defined (SOC_AM65XX) || defined (SOC_J721E)
    /* Limit the outstanding transactions to 2
     * only for AM65xx and J721e platforms
     * Later SoCs do not have this issue
     */
    vPortSetDLFOBit();
#else
    /* Fix for PRSDK-8161
     * For AM64x and J7200, there is no issue of limitting outstanding
     * transactions. R5F core can support full 7 outstanding transactions
     */
#endif

    /* Disable/Clear pending Interrupts in VIM before enabling CPU Interrupts */
    /* This is done to prevent serving any bogus interrupt */
    for (loopCnt = 0U ; loopCnt < maxIntrs; loopCnt++)
    {
        /* Disable interrupt in vim */
        CSL_vimSetIntrEnable((CSL_vimRegs *)(uintptr_t)regAddr,
                                    loopCnt,
                                    BFALSE);
        /* Clear interrupt status */
        CSL_vimClrIntrPending((CSL_vimRegs *)(uintptr_t)regAddr,
                                     loopCnt);
    }
}

