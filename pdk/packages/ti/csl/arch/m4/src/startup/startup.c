#include <ti/csl/arch/m4/csl_arm_mpu.h>
#include <ti/csl/csl_error.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/csl_rat.h>
#if defined (SOC_AM64X) || defined (SOC_AM62X) || defined (SOC_AM275X)
#define CSL_MCU_M4FSS0_RAT_CFG_BASE_LOCAL_VIEW            (0x44200000UL)
#define CSL_MCU_M4FSS0_RAT_REGION0_BASE                   (0x60000000UL)
#endif
#include <string.h>
#pragma SET_CODE_SECTION(".startupCode")
#pragma SET_DATA_SECTION(".startupData")

/* 
  Region 0 has lowest priority,
  Region 7 has highest
  Shareable affects L1 caching and memory attribute in broadcast
  This table will not be used if there is another variable named gCslM4MpuCfg without the weak attribute
*/
extern const CSL_ARMM4MpuRegionCfg_t  __attribute__((weak)) gCslM4MpuCfg[] =
{
    {
        /* Region 0 configuration: IRAM+DRAM */
        .regionId         = 0U,
        .enable           = 1U,
        .baseAddr         = 0x0,
        .size             = CSL_ARM_M4_MPU_REGION_SIZE_256KB,
        .subRegionEnable  = CSL_ARM_M4_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_M4_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = CSL_ARM_M4_CACHE_POLICY_NON_CACHEABLE,
        .memAttr          = 0U
    },
    {
        /* Region 1 configuration: 8k RAT Cfg Regs */
        .regionId         = 1U,
        .enable           = 1U,
        .baseAddr         = 0x44200000,
        .size             = CSL_ARM_M4_MPU_REGION_SIZE_8KB,
        .subRegionEnable  = CSL_ARM_M4_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_M4_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = 0U,
        .memAttr          = 0U
    },
    {
        /* Region 2 configuration: 256 MB IP */
        .regionId         = 2U,
        .enable           = 1U,
        .baseAddr         = 0x60000000,
        .size             = CSL_ARM_M4_MPU_REGION_SIZE_256MB,
        .subRegionEnable  = CSL_ARM_M4_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_M4_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = 0U,
        .memAttr          = 0U
    },
    {
        /* Region 3 configuration: 2 MB of banked OCMSRAM (Banks 0-7) */
        .regionId         = 3U,
        .enable           = 1U,
        .baseAddr         = 0x70000000,
        .size             = CSL_ARM_M4_MPU_REGION_SIZE_2MB,
        .subRegionEnable  = CSL_ARM_M4_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_M4_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_M4_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 4 configuration: 1 GB DDR RAM */
        .regionId         = 4U,
        .enable           = 1U,
        .baseAddr         = 0x80000000,
        .size             = CSL_ARM_M4_MPU_REGION_SIZE_1GB,
        .subRegionEnable  = CSL_ARM_M4_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_M4_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_M4_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    }
};

/* 
 * Default RAT configuration for M4F
 *
 *  Every application is expected to have the RAT configuration table, unless
 *  the default CSL startup mapping is good enough for the application.
 *  The default RAT entry are provided as below. Note that number of
 *  RAT entries and RAT-able space for a given SOC is limitted.
 *  Example: For AM64X, number of RAT entries = 8 and RAT-able space = 2GB.
 *  The below global variable needs to be defined in the application to
 *  override the default RAT configuration from CSL.
 *
 *  Example:
 *     CSL_RatTranslationCfgInfo  gCslM4RatCfg[8+1] = {...RAT ENTRIES ...];
 */
extern CSL_RatTranslationCfgInfo  __attribute__((weak)) gCslM4RatCfg[8+1] =
{
#if defined (SOC_AM64X) || defined(SOC_AM62X) || defined (SOC_AM275X)
    /* Add an entry for accessing MCU addresses including  MCU IPs */
    {
        .sizeInBytes        = (uint64_t) (0x01000000UL), /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x64000000U),  /* Translated base address */
        .translatedAddress  = (uint64_t) (0x04000000UL)  /* Physical addresses */
    },
    /* Add an entry for accessing MSRAM addresses */
    {
        .sizeInBytes        = (uint64_t) (0x00200000UL), /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x70000000U),  /* Translated base address */
        .translatedAddress  = (uint64_t) (0x70000000UL)  /* Physical addresses */
    },
    /* Add an entry for MAIN domain peripheral addresses */
    {
        .sizeInBytes        = (uint64_t) (0x04000000UL), /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x60000000U),   /* Translated base address */
        .translatedAddress  = (uint64_t) (0x00000000UL)   /* Physical addresses */
    },
    /* Add an entry for accessing DDR addresses */
    {
        .sizeInBytes        = (uint64_t) (0x10000000UL), /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x80000000U),  /* Translated base address */
        .translatedAddress  = (uint64_t) (0x80000000UL)  /* Physical addresses */
    },
    /* Add an entry for accessing Mailbox addresses */
    {
        .sizeInBytes        = (uint64_t) (0x00080000UL),
        .baseAddress        = (uint32_t) (0x69000000U),
        .translatedAddress  = (uint64_t) (0x29000000U),
    },
    /* Add an entry for accessing DMASS addresses */
    {
        .sizeInBytes        = (uint64_t) (0x08000000UL), /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0xA8000000U),  /* Translated base address */
        .translatedAddress  = (uint64_t) (0x48000000UL)  /* Physical addresses */
    },
    /* Last entry to quit RAT cfg */
    {
        .sizeInBytes        = (uint64_t) (0xDEADFACEUL), /* Size in Bytes for the map = END of MAP */
        .baseAddress        = (uint32_t) (0xDEADFACEU),  /* Translated base address   = END of MAP */
        .translatedAddress  = (uint64_t) (0xDEADFACEUL)  /* Physical addresses        = END of MAP */
    }
#endif
};

__attribute__((weak)) void __mpu_init(void);
int _system_pre_init(void);
void _system_post_cinit(void);

/*****************************************************************************/
/* \brief __MPU_INIT() - __mpu_init() is called in the C/C++ startup routine,*/
/* _c_int00(), and provides a mechanism for tailoring mpu init by device     */
/* prior to calling main().                                                  */
/*                                                                           */
/*****************************************************************************/
/*  Usage notes:
    On entry to this function from boot, M4F must be in System (privileged) mode. */
void __mpu_init(void)
{
    uint32_t loopCnt = 0U;
    uint32_t max_regions, cfg_regions;
    CSL_ARMM4MpuRegionCfg_t disableCfg;

    /* initialize the cfg */
    memset((void*) &disableCfg, 0, sizeof (CSL_ARMM4MpuRegionCfg_t));

    /* Disable MPU */
    CSL_armM4MPUEnable(0U);

    (void) CSL_armM4GetNumMPURegions(&max_regions);

    /* Disable all MPU regions */
    for (loopCnt = 0U ;
         loopCnt < max_regions ;
         loopCnt++)
    {
        /* Update the region ID */
        disableCfg.regionId = loopCnt;
        CSL_armM4MPUCfgRegion(&disableCfg);
    }

    cfg_regions  = sizeof (gCslM4MpuCfg) / sizeof (CSL_ARMM4MpuRegionCfg_t);
    /* Configure MPU regions only for provided configuration */
    for (loopCnt = 0U ;
         loopCnt < cfg_regions ;
         loopCnt++)
    {
            /* configure MPU region here */
            CSL_armM4MPUCfgRegion(&gCslM4MpuCfg[loopCnt]);
    }

    /* Enable MPU */
    CSL_armM4MPUEnable(1U);
}

/* RAT Configuration */
__attribute__((weak)) void __ratCfg_init(void);

void __ratCfg_init(void)
{
    uint32_t loopCnt = 0U;
    uint32_t max_regions;
    CSL_ratRegs *pRatRegs = (CSL_ratRegs *) CSL_MCU_M4FSS0_RAT_CFG_BASE_LOCAL_VIEW;
 
    max_regions = CSL_ratGetMaxRegions(pRatRegs);

    for (loopCnt = 0U; loopCnt < max_regions; loopCnt++)
    {
        if (gCslM4RatCfg[loopCnt].sizeInBytes == (uint64_t) (0xDEADFACEUL))
        {
            break;
        }
        else
        {
            (void) CSL_ratConfigRegionTranslation(pRatRegs, 
                                                loopCnt, &gCslM4RatCfg[loopCnt]);
        }
    }
}

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
/*****************************************************************************/
int _system_pre_init(void)
{
    __ratCfg_init();
    return 1;
}

/*****************************************************************************/
/* _SYSTEM_POST_CINIT() - _system_post_cinit() is a hook function called in  */
/* the C/C++ auto-initialization function after cinit() and before pinit().  */
/*                                                                           */
/*****************************************************************************/
void _system_post_cinit(void)
{

}

/* Nothing past this point */
