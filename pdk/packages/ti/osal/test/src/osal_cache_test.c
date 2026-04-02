/*
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   osal_cache_test.c
 *
 *  \brief  OSAL driver test application main file.
 *
 */

#include <ti/board/board.h>
#include <ti/osal/osal.h>
#include "OSAL_log.h"
#include <ti/csl/arch/r5/csl_arm_r5_mpu.h>

#define OSAL_WRITE_THROUGH_CACHE_BASE                           ((uint32_t*)0xA0000000)
#define OSAL_WRITE_BACK_CACHE_BASE                              (uint32_t*)0xB0000000

#define OSAL_UNCACHED_REGION_BASE                               ((uint32_t*)0xC0000000)
#define OSAL_INTER_CORE_WRITE_THROUGH_SIGNAL_BASE               ((uint32_t*)0xC0000000)
#define OSAL_INTER_CORE_WRITE_THROUGH_ACK_BASE                  ((uint32_t *)0xC0000004)
#define OSAL_INTER_CORE_WRITE_BACK_SIGNAL_BASE                  ((uint32_t *)0xC0000008)
#define OSAL_INTER_CORE_WRITE_BACK_ACK_BASE                     ((uint32_t *)0xC000000C)
#define OSAL_INTER_CORE_WRITE_BACK_PHASE_2_SIGNAL_BASE          ((uint32_t *)0xC0000010)
#define OSAL_INTER_CORE_WRITE_BACK_PHASE_2_ACK_BASE             ((uint32_t *)0xC0000014)

#define WRITE_THROUGH_TOKEN                                     (0xABABABABU)
#define OSAL_WRITE_BACK_STALE_TOKEN                             (0xBAD0BAD0U)
#define OSAL_WRITE_BACK_FRESH_TOKEN                             (0xDEAFDEAFU)

volatile uint32_t *writeBackAddr = OSAL_WRITE_BACK_CACHE_BASE;
volatile uint32_t *interCoreWriteBackSignalBase = OSAL_INTER_CORE_WRITE_BACK_SIGNAL_BASE;
volatile uint32_t *interCoreWriteBackAckBase = OSAL_INTER_CORE_WRITE_BACK_ACK_BASE;
volatile uint32_t *interCoreWriteBackPhase2SignalBase = OSAL_INTER_CORE_WRITE_BACK_PHASE_2_SIGNAL_BASE;
volatile uint32_t *interCoreWriteBackPhase2AckBase = OSAL_INTER_CORE_WRITE_BACK_PHASE_2_ACK_BASE;

volatile uint32_t *writeThroughAddr = OSAL_WRITE_THROUGH_CACHE_BASE;
volatile uint32_t *interCoreWriteThroughSignalBase = OSAL_INTER_CORE_WRITE_THROUGH_SIGNAL_BASE;
volatile uint32_t *interCoreWriteThroughAckBase = OSAL_INTER_CORE_WRITE_THROUGH_ACK_BASE;

extern const CSL_ArmR5MpuRegionCfg __attribute__((section(".startupData"))) gCslR5MpuCfg[CSL_ARM_R5F_MPU_REGIONS_MAX] =
{
     {
        /* Region 0 configuration: complete 32 bit address space = 4Gbits */
        .regionId         = 0U,
        .enable           = UTRUE,
        .baseAddr         = 0x0U,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_4GB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UFALSE,
        .cachePolicy      = 0U,
        .memAttr          = 0U,
    },
    {
        /* Region 2 configuration: MCMS3 RAM */
        .regionId         = 2U,
        .enable           = UTRUE,
        .baseAddr         = 0x70000000,
#if defined (SOC_J721E) || defined (SOC_J784S4)
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_8MB,
#endif
#if defined (SOC_J7200)
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_1MB,
#endif
#if defined (SOC_J721S2) || defined(SOC_J742S2)
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_4MB,
#endif
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UTRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 3 configuration: 2 GB DDR RAM */
        .regionId         = 3U,
        .enable           = UTRUE,
        .baseAddr         = 0x80000000,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_2GB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UTRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
        .memAttr          = 0U,
    },
#if defined (BUILD_MCU2_0)
    {
        /* Region 4 configuration: Write Through DDR region, needed for the test. */
        .regionId         = 4U,
        .enable           = UTRUE,
        .baseAddr         = (uint32_t)(OSAL_WRITE_THROUGH_CACHE_BASE),
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32B,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UTRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WT_NO_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 5 configuration: Write-back DDR region, needed for the test. */
        .regionId         = 5U,
        .enable           = UTRUE,
        .baseAddr         = (uint32_t)(OSAL_WRITE_BACK_CACHE_BASE),
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32B,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UTRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 6 configuration: Uncached DDR region, needed for the test. */
        .regionId         = 6U,
        .enable           = UTRUE,
        .baseAddr         = (uint32_t)(OSAL_UNCACHED_REGION_BASE),
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32B,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UTRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
        .memAttr          = 0U,
    },
#endif
    {
        /* Region 7 configuration: 32 KB ATCM */
        .regionId         = 7U,
        .enable           = UTRUE,
        .baseAddr         = 0x0,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UTRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
        .memAttr          = 0U,
    },
};


#if defined (BUILD_MCU2_0)
static void OSAL_write_through_cache_test(void)
{
    /*  Bring the writeThroughAddr memory into cache */
    (*writeThroughAddr) = 0U;
    /* Make cached write */
    (*writeThroughAddr) = WRITE_THROUGH_TOKEN;
    
    /* Signal MCU2_1 to check for write-through success. */
    (*interCoreWriteThroughSignalBase) = 1U;
    while(1U != (*interCoreWriteThroughAckBase))
    {
        /* Wait for success ACK. */
    }
}

static void OSAL_write_back_cache_test(void)
{
    uint32_t looper = 0U;

    /* Ensure that the required address is cached in by the cache */
    for(looper = 0U; looper < 100U; looper++)
    {
        (*writeBackAddr) = OSAL_WRITE_BACK_STALE_TOKEN;
    }
    (*writeBackAddr) = OSAL_WRITE_BACK_FRESH_TOKEN;
    /* interCoreWriteBackSignalBase and interCoreWriteBackAckBase are in the uncached region.
     * So no worries of writeThroughAddr being evicted out of cache, write-back of a memory
     * happens only on eviction of dirty cache line. R5 is harvard architecture,
     * so separate cache for instructions. 
     */
    (*interCoreWriteBackSignalBase) = 1U;
    while(1U != (*interCoreWriteBackAckBase))
    {
        /* Wait for success ACK. */
    }

    CacheP_wb((void  *)writeBackAddr, sizeof(uint32_t));
    /* interCoreWriteBackSignalBase and interCoreWriteBackAckBase are in the uncached region.
     * So no worries of writeThroughAddr being evicted out of cache, write-back of a memory
     * happens only on eviction of dirty cache line. R5 is harvard architecture,
     * so separate cache for instructions. 
     */
    (*interCoreWriteBackPhase2SignalBase) = 1U;
    while(1U != (*interCoreWriteBackPhase2AckBase))
    {
        /* Wait for success ACK. */
    }
}
#endif

#if defined (BUILD_MCU2_1)
static void OSAL_write_through_checker(void)
{
    /* Keep looping till MCU2_0 signals a write-through check. */
    while(1U != (*interCoreWriteThroughSignalBase))
    {

    }
    /* Invalidate own cache before checking for write-through. */
    CacheP_Inv((void *)writeThroughAddr, sizeof(uint32_t));

    if(WRITE_THROUGH_TOKEN != (*writeThroughAddr))
    {
        OSAL_log("\n\rWrite through test has failed on MCU2_0!!\n\r");
    }
    else
    {
        OSAL_log("\n\rWrite through test has passed on MCU2_0!!\n\r");
        (*interCoreWriteThroughAckBase) = 1U;
    }
}

static void OSAL_write_back_checker(void)
{
    while(1U != (*interCoreWriteBackSignalBase))
    {

    }
    if(OSAL_WRITE_BACK_FRESH_TOKEN == (*writeBackAddr))
    {
        OSAL_log("\n\rWrite back Phase 1 test has failed on MCU2_0!!\n\r");
    }
    else
    {
        OSAL_log("\n\rWrite back Phase 1 test has passed on MCU2_0!!\n\r");
        (*interCoreWriteBackAckBase) = 1U;
    }
    while(1U != (*interCoreWriteBackPhase2SignalBase))
    {

    }
    if(OSAL_WRITE_BACK_FRESH_TOKEN == (*writeBackAddr))
    {
        OSAL_log("\n\rWrite back Phase 2 test has passed on MCU2_0!!\n\r");
        *interCoreWriteBackPhase2AckBase = 1U;
    }
    else
    {
        OSAL_log("\n\rWrite back Phase 2 test has failed on MCU2_0!!\n\r");
    }
}
#endif

int main()
{
#if defined(BUILD_MCU2_0)
    /* Signal MCU2_1 to wait for write-through check. */
    /* By the time *interCoreWriteThroughSignalBase is being set to 0, mcu2_1 is sleeping... */
    (*interCoreWriteThroughSignalBase) = 0U;
    /* Signal MCU2_1 to wait for write-back check. */
    /* By the time *interCoreWriteBackSignalBase is being set to 0, mcu2_1 is sleeping... */
    (*interCoreWriteBackSignalBase) = 0U;
    /* Signal MCU2_1 to wait for write-back check Phase 2. */
    /* By the time *interCoreWriteBackPhase2SignalBase is being set to 0, mcu2_1 is sleeping... */
    (*interCoreWriteBackPhase2SignalBase) = 0U;
    /* Signal MCU2_1 to wait for write-through check. */
    /* By the time *interCoreWriteThroughAckBase is being set to 0, mcu2_1 is sleeping... */
    (*interCoreWriteThroughAckBase) = 0U;
    /* Signal MCU2_1 to wait for write-back check. */
    /* By the time *interCoreWriteBackAckBase is being set to 0, mcu2_1 is sleeping... */
    (*interCoreWriteBackAckBase) = 0U;
    /* Signal MCU2_1 to wait for write-back check. */
    /* By the time *interCoreWriteBackPhase2AckBase is being set to 0, mcu2_1 is sleeping... */
    (*interCoreWriteBackPhase2AckBase) = 0U;

    /* Initialize the write-through and write-back addresses before the test stars... */
     (*writeBackAddr) = 0U;
     CacheP_wb((void *)writeBackAddr, sizeof(uint32_t));
     (*writeThroughAddr) = 0U;

    
    OSAL_write_through_cache_test();
    OSAL_write_back_cache_test();
#endif
#if defined(BUILD_MCU2_1)
    Board_init(BOARD_INIT_UART_STDIO);
    Osal_delay(2000);
    OSAL_write_through_checker();
    Osal_delay(2000);
    OSAL_write_back_checker();

#if defined LDRA_DYN_COVERAGE_EXIT
    OSAL_log("\n LDRA ENTRY... \n");
    upload_execution_history();
    OSAL_log("\n LDRA EXIT... \n");
#endif

    if ((1U == (*interCoreWriteThroughAckBase)) &&
        (1U == (*interCoreWriteBackAckBase)) &&
        (1U == (*interCoreWriteBackPhase2AckBase)))
    {
        OSAL_log("\n\rAll tests have passed!!!\n\r");
    }
#endif
    while(1)
    {
        /* Do not return to startup code. */
    }
    return 0;
}
