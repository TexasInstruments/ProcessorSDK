/*
 *  Copyright (c) Texas Instruments Incorporated 2021
 *  All rights reserved.
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
 *  \file c7x_mmu.c
 *
 *  \brief C7x MMU Config File
 */

#ifdef BUILD_C7X

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */


#if (defined (FREERTOS))
#include "Mmu.h"
#elif (defined (SAFERTOS))
#include "Mmu.h"
#endif
#include "memory_map_defines.inc"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined(SOC_J721S2) || defined(SOC_J742S2)
    #define MSMC_SIZE   0x00400000U /* 4MB */
#elif defined(SOC_J721E) || defined(SOC_J784S4)
    #define MSMC_SIZE   0x00800000U /* 8MB */
#endif

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

void IpcInitMmu(bool isSecure)
{
    Mmu_MapAttrs    attrs;

    Mmu_initMapAttrs(&attrs);

    if(true == isSecure)
    {
        attrs.ns = 0;
    }
    else
    {
        attrs.ns = 1;
    }

    /* Register region */
    attrs.attrIndx = Mmu_AttrIndx_MAIR0;
    (void)Mmu_map(0x00000000U, 0x00000000U, 0x20000000U, &attrs, isSecure);
    (void)Mmu_map(0x20000000U, 0x20000000U, 0x20000000U, &attrs, isSecure);
    (void)Mmu_map(0x40000000U, 0x40000000U, 0x20000000U, &attrs, isSecure);
    (void)Mmu_map(0x60000000U, 0x60000000U, 0x10000000U, &attrs, isSecure);
    (void)Mmu_map(0x78000000U, 0x78000000U, 0x08000000U, &attrs, isSecure); /* CLEC */

    attrs.attrIndx = Mmu_AttrIndx_MAIR7;
    (void)Mmu_map(0x80000000U, 0x80000000U, 0x20000000U, &attrs, isSecure); /* DDR */
    (void)Mmu_map(0xA0000000U, 0xA0000000U, 0x20000000U, &attrs, isSecure); /* DDR */

    (void)Mmu_map(0x70000000U, 0x70000000U, MSMC_SIZE  , &attrs, isSecure); /* MSMC */
    (void)Mmu_map(0x41C00000U, 0x41C00000U, 0x00100000U, &attrs, isSecure); /* OCMC - 1MB */

    /* IPC VRing Buffer - uncached */
    attrs.attrIndx =  Mmu_AttrIndx_MAIR4;
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    (void)Mmu_map(SHARED_DDR_SPACE_START, SHARED_DDR_SPACE_START, 0x04000000U, &attrs, isSecure); /* VRING DDR */
#else
    (void)Mmu_map(SHARED_DDR_SPACE_START, SHARED_DDR_SPACE_START, 0x02000000U, &attrs, isSecure); /* VRING DDR */
#endif
#if defined(BUILD_C7X_1)
    (void)Mmu_map(C7x_1_IPC_DATA_BASE, C7x_1_IPC_DATA_BASE, 0x00100000U, &attrs, isSecure); /* C7X_1 DDR */
#elif defined(BUILD_C7X_2)
    (void)Mmu_map(C7x_2_IPC_DATA_BASE, C7x_2_IPC_DATA_BASE, 0x00100000U, &attrs, isSecure); /* C7X_2 DDR */
#elif defined(BUILD_C7X_3)
    (void)Mmu_map(C7x_3_IPC_DATA_BASE, C7x_3_IPC_DATA_BASE, 0x00100000U, &attrs, isSecure); /* C7X_3 DDR */
#elif defined(BUILD_C7X_4)
    (void)Mmu_map(C7x_4_IPC_DATA_BASE, C7x_4_IPC_DATA_BASE, 0x00100000U, &attrs, isSecure); /* C7X_4 DDR */
#endif

    return;
}

#endif /* #ifdef BUILD_C7X */
