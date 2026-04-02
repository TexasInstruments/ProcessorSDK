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
 *  \ingroup  IP_FMA_CLEC
 *  \defgroup IP_FMA_CLEC_IMPLEMENTATION CLEC Register Check Implementation.
 *
 *  @{
 */

/**
 *  \file     ip_fma_clec.c
 *
 *  \brief    CLEC-specific wrappers over IP_FMA Register Check
 *            for readback of static configuration registers.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "ip_fma_clec.h"
#include <ti/csl/tistdtypes.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \brief CLEC static configuration register offsets
 */
/**< Revision parameters.           */
#define CLEC_PID                                        (0x00U)
/**< Global Event Lock register for secure claims.     */
#define CLEC_GELRS                                      (0xC000U)
/**< Global Event Lock register for non-secure claims. */
#define CLEC_GELRNS                                     (0xD000U)
/**< Event Map and Routing register. */
#define CLEC_MMR                                        (0x011000U)

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

IpFma_Status IpFma_Clec_GetRegs(uintptr_t baseAddr, IpFma_ClecRegs* out)
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
            { CLEC_PID,      0u, IPFMA_WIDTH_32 },
            { CLEC_GELRS,    0u, IPFMA_WIDTH_32 },
            { CLEC_GELRNS,   0u, IPFMA_WIDTH_32 },
            { CLEC_MMR,      0u, IPFMA_WIDTH_32 }
        };

        status = IpFma_GetRegsValues(baseAddr, registerDescriptorsArr, (uint32_t)(sizeof(registerDescriptorsArr) / sizeof(registerDescriptorsArr[0])));

        if (IPFMA_OK == status)
        {
            out->pid    = registerDescriptorsArr[0].value;
            out->gelrs  = registerDescriptorsArr[1].value;
            out->gelrns = registerDescriptorsArr[2].value;
            out->mmr    = registerDescriptorsArr[3].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Clec_CompareRegs(const IpFma_ClecRegs* expected,
                                    const IpFma_ClecRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == expected) || (NULL_PTR == actual))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if ((expected->pid != actual->pid))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->gelrs != actual->gelrs))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->gelrns != actual->gelrns))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->mmr != actual->mmr))
        {
            status = IPFMA_E_MISMATCH;
        }
    }

    return status;
}

/** @} */