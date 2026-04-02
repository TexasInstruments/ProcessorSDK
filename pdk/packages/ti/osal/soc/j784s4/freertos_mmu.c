/*
 *  Copyright (c) Texas Instruments Incorporated 2022
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
 *  \file freertos_mmu.c
 *
 *  \brief This has the common default MMU setting function for C7x FreeRTOS
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <c7x.h>

#include <ti/csl/soc.h>
#include <ti/osal/soc/j784s4/osal_soc.h>
#include <ti/osal/src/nonos/Nonos_config.h>

#if defined (BUILD_C7X)
#include <ti/csl/arch/csl_arch.h>
#include "Mmu.h"
#endif

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

/* Applications can choose to define this function or declare it as an extern to reuse this definition. */
void Osal_initMmuDefault(void);

/**< Simple wrapper for FreeRTOS port Mmu_map(), as the paramters for different arch
		differ */
static bool OsalMmuMap(uint64_t vaddr, uint64_t paddr, size_t size,
						Mmu_MapAttrs *attrs, bool secure);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static bool OsalMmuMap(uint64_t vaddr, uint64_t paddr, size_t size,
						Mmu_MapAttrs *attrs, bool secure)
{
	return (Mmu_map(vaddr, paddr, size, attrs, secure));
}

static void OsalInitMmu(bool isSecure)
{
    Mmu_MapAttrs    attrs;

    Mmu_initMapAttrs(&attrs);
    attrs.attrIndx = Mmu_AttrIndx_MAIR0;

    if( BTRUE == isSecure )
    {
        attrs.ns = BFALSE;
    }
    else
    {
        attrs.ns = BTRUE;
    }

    /* Register region */
    OsalMmuMap(0x00000000U, 0x00000000U, 0x20000000U, &attrs, isSecure);
    OsalMmuMap(0x20000000U, 0x20000000U, 0x20000000U, &attrs, isSecure);
    OsalMmuMap(0x40000000U, 0x40000000U, 0x20000000U, &attrs, isSecure);
    OsalMmuMap(0x60000000U, 0x60000000U, 0x10000000U, &attrs, isSecure);
    OsalMmuMap(0x78000000U, 0x78000000U, 0x08000000U, &attrs, isSecure); /* CLEC */

    attrs.attrIndx = Mmu_AttrIndx_MAIR7;
    OsalMmuMap(0x80000000U, 0x80000000U, 0x20000000U, &attrs, isSecure); /* DDR */
    OsalMmuMap(0xA0000000U, 0xA0000000U, 0x20000000U, &attrs, isSecure); /* DDR */
    OsalMmuMap(0x70000000U, 0x70000000U, 0x00800000U, &attrs, isSecure); /* MSMC - 8MB */
    OsalMmuMap(0x41C00000U, 0x41C00000U, 0x00080000U, &attrs, isSecure); /* OCMC - 512KB */

    /*
     * DDR range 0xA0000000 - 0xA8000000 : Used as RAM by multiple
     * remote cores, no need to mmp_map this range.
     * IPC VRing Buffer - uncached
     */
    attrs.attrIndx =  Mmu_AttrIndx_MAIR4;
    OsalMmuMap(0xAC000000U, 0xAC000000U, 0x04000000U, &attrs, isSecure);

    return;
}

/* The C7x CLEC should be programmed to allow config/re config either in secure
 * OR non secure mode. This function configures all inputs to given level
 *
 * Instance is hard-coded for J784S4 only
 *
 */
void OsalCfgClecAccessCtrl (bool onlyInSecure)
{
    CSL_ClecEventConfig cfgClec;
    CSL_CLEC_EVTRegs   *clecBaseAddr = (CSL_CLEC_EVTRegs*) CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE;
    uint32_t            i, maxInputs = 2048U;
    uint32_t            secureClaim = 0U;

    cfgClec.secureClaimEnable = onlyInSecure?1U:0U;
    cfgClec.evtSendEnable     = UFALSE;
    cfgClec.rtMap             = CSL_CLEC_RTMAP_DISABLE;
    cfgClec.extEvtNum         = 0U;
    cfgClec.c7xEvtNum         = 0U;
    for(i = 0U; i < maxInputs; i++)
    {
        /* Since the CLEC module is shared b/w c7x_1, c7x_2, c7x_3 and c7x_4,
         * Before reseting the events and disabling secure claim,
         * check if its already done by other C7x. */
        (void)CSL_clecGetSecureClaimStatus(clecBaseAddr, i, &secureClaim);
        if(SECURE_ENABLE == secureClaim)
        {
            (void)CSL_clecConfigEvent(clecBaseAddr, i, &cfgClec);
        }
    }
}

void Osal_initMmuDefault(void)
{
    OsalInitMmu(BFALSE);
    OsalInitMmu(BTRUE);

    /* Setup CLEC access/configure in non-secure mode */
    OsalCfgClecAccessCtrl(BFALSE);

    return;
}

