/*
 *  Copyright (c) Texas Instruments Incorporated 2026
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
 *  \ingroup  IP_FMA_CFG_WRAP_CC
 *  \defgroup IP_FMA_CFG_WRAP_CC_IMPLEMENTATION CFG_WRAP_CC Register Check Implementation
 *
 *  @{
 */

/**
 *  \file     ip_fma_cfg_wrap_cc.c
 *
 *  \brief    CFG_WRAP_CC-specific wrappers over IP_FMA Register Check
 *            for readback of static configuration registers.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "ip_fma_cfg_wrap_cc.h"
#include <ti/csl/tistdtypes.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \brief Compute Cluster Config Wrap CC register offsets
 */
/**< ARM REVISION parameters.    */
#define ARM_0_REV                                    (0x00U)
/**< ARM REVISION parameters.    */
#define ARM_1_REV                                    (0x04U)
/**< ARM BISOR control register. */
#define ARM_0_PBIST_BISOR_CNTRL                      (0x100U)
/**< ARM BISOR control register. */
#define ARM_1_PBIST_BISOR_CNTRL                      (0x104U)
/**< CC misc control register contains clock override 
 *   bits for  DSP clocking  and some DRU ctset debug
 *   controls. */
#define CC_CNTRL                                     (0x200U)

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

IpFma_Status IpFma_CfgWrapCC_GetRegs(uintptr_t baseAddr, IpFma_CfgWrapCCRegs* out)
{
    IpFma_Status status = IPFMA_OK;

    if (NULL_PTR == out)
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        static IpFma_RegDesc registerDescriptorsArr[] =
        {
            { ARM_0_REV,                  0u, IPFMA_WIDTH_32 },
            { ARM_0_PBIST_BISOR_CNTRL,    0u, IPFMA_WIDTH_32 },
            { ARM_1_REV,                  0u, IPFMA_WIDTH_32 },
            { ARM_1_PBIST_BISOR_CNTRL,    0u, IPFMA_WIDTH_32 },
            { CC_CNTRL,                   0u, IPFMA_WIDTH_32 }
        };

        status = IpFma_GetRegsValues(baseAddr, registerDescriptorsArr, \
            (uint32_t)(sizeof(registerDescriptorsArr) / sizeof(registerDescriptorsArr[0])));

        if (IPFMA_OK == status)
        {
            out->arm_0_rev                  = registerDescriptorsArr[0].value;
            out->arm_0_pbist_bisor_cntrl    = registerDescriptorsArr[1].value;
            out->arm_1_rev                  = registerDescriptorsArr[2].value;
            out->arm_1_pbist_bisor_cntrl    = registerDescriptorsArr[3].value;
            out->cc_cntrl                   = registerDescriptorsArr[4].value;
        }
    }

    return status;
}

IpFma_Status IpFma_CfgWrapCC_CompareRegs(const IpFma_CfgWrapCCRegs* expected,
                                         const IpFma_CfgWrapCCRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == expected) || (NULL_PTR == actual))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if ((expected->arm_0_rev != actual->arm_0_rev))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->arm_0_pbist_bisor_cntrl != actual->arm_0_pbist_bisor_cntrl))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->arm_1_rev != actual->arm_1_rev))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->arm_1_pbist_bisor_cntrl != actual->arm_1_pbist_bisor_cntrl))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->cc_cntrl != actual->cc_cntrl))
        {
            status = IPFMA_E_MISMATCH;
        }
    }

    return status;
}

/** @} */
