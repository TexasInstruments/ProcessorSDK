/*
 *  Copyright (c) Texas Instruments Incorporated 2019
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
 *  \file ipc_apputils.c
 *
 *  \brief Common IPC application utility used in all IPC example.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/ipc/ipc.h>
#include <ti/drv/sciclient/sciclient.h>
#include "ipc_apputils.h"
#include <ti/osal/osal.h>
#include <ti/drv/ipc/include/ipc_config.h>
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

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

void Ipc_appUtilsCacheWb(const void *addr, int32_t size)
{
    uint32_t    isCacheCoherent = Ipc_isCacheCoherent();

    if(isCacheCoherent != TRUE)
    {
        CacheP_wb(addr, size);
    }

    return;
}

void Ipc_appUtilsCacheInv(const void * addr, int32_t size)
{
    uint32_t    isCacheCoherent = Ipc_isCacheCoherent();

    if(isCacheCoherent != TRUE)
    {
        CacheP_Inv(addr, size);
    }

    return;
}

void Ipc_appUtilsCacheWbInv(const void * addr, int32_t size)
{
    uint32_t    isCacheCoherent = Ipc_isCacheCoherent();

    if(isCacheCoherent != TRUE)
    {
        CacheP_wbInv(addr, size);
    }

    return;
}

uint64_t Ipc_appVirtToPhyFxn(const void *virtAddr, uint32_t chNum, void *appData)
{
    uint64_t    phyAddr;

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

    return (phyAddr);
}

void *Ipc_appPhyToVirtFxn(uint64_t phyAddr, uint32_t chNum, void *appData)
{
    void       *virtAddr;

#if defined (__aarch64__) || defined (BUILD_C7X)
    virtAddr = (void *) phyAddr;
#else
    uint32_t temp;

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

    /* R5/C66x is 32-bit; need to truncate to avoid void * typecast error */
    temp = (uint32_t) phyAddr;
    virtAddr = (void *) temp;
#endif

    return (virtAddr);
}

uint32_t Ipc_appIsPrintSupported(void)
{
    uint32_t retVal = TRUE;

    /* Semi hosting not supported for MPU on Simulator */
#if (defined (BUILD_MPU) && defined (SIMULATOR))
    retVal = FALSE;
#endif

    /* Printf doesn't work for MPU when run from SBL with no CCS connection
     * There is no flag to detect SBL or CCS mode. Hence disable the print
     * for MPU unconditionally */
#if defined (BUILD_MPU)
    retVal = FALSE;
#endif

    return (retVal);
}

uint64_t Ipc_getTimeInUsec(void)
{
    return (TimerP_getTimeInUsecs());
}

void sysIdleLoop(void)
{
#if defined(BUILD_C66X)
    __asm(" IDLE");
#elif defined(BUILD_C7X)
    __asm(" IDLE");
#elif defined(BUILD_MCU)
   asm(" wfi");
#endif
}


