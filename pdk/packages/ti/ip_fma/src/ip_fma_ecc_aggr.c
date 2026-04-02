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
 *  \ingroup  IP_FMA_ECC_AGGR
 *  \defgroup IP_FMA_ECC_AGGR_IMPLEMENTATION Ecc Aggregator Register Check Implementation
 *
 *  @{
 */

/**
 *  \file     ip_fma_ecc_aggr.c
 *
 *  \brief    Ecc Aggregator register read/compare API implementation.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "ip_fma_ecc_aggr.h"
#include <ti/csl/cslr_ecc_aggr.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Size of the RESERVED_SVBUS register array*/
#define RESERVED_SVBUS_SIZE                                     (8)

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

IpFma_Status IpFma_EccAggr_GetRegs(uintptr_t baseAddr,
                                   IpFma_EccAggregatorRegs* out)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == out))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        static IpFma_RegDesc registerDescriptorsArr[] =
        {
            { CSL_ECC_AGGR_REV,                     0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_VECTOR,                  0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_STAT,                    0u, IPFMA_WIDTH_32 },
            
            { CSL_ECC_AGGR_RESERVED_SVBUS(0U),      0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_RESERVED_SVBUS(1U),      0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_RESERVED_SVBUS(2U),      0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_RESERVED_SVBUS(3U),      0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_RESERVED_SVBUS(4U),      0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_RESERVED_SVBUS(5U),      0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_RESERVED_SVBUS(6U),      0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_RESERVED_SVBUS(7U),      0u, IPFMA_WIDTH_32 },            
            
            { CSL_ECC_AGGR_SEC_EOI_REG,             0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_SEC_ENABLE_CLR_REG0,     0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_SEC_ENABLE_CLR_REG0,     0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_DED_EOI_REG,             0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_DED_ENABLE_SET_REG0,     0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_DED_ENABLE_CLR_REG0,     0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_AGGR_ENABLE_SET,         0u, IPFMA_WIDTH_32 },
            { CSL_ECC_AGGR_AGGR_ENABLE_CLR,         0u, IPFMA_WIDTH_32 }
        };

        status = IpFma_GetRegsValues(baseAddr, registerDescriptorsArr, 
                                    (uint32_t)(sizeof(registerDescriptorsArr) / sizeof(registerDescriptorsArr[0])));

        if (IPFMA_OK == status)
        {
            out->rev                 = registerDescriptorsArr[0].value;
            out->vector              = registerDescriptorsArr[1].value;
            out->stat                = registerDescriptorsArr[2].value;

            for (uint32_t i = 0; i < RESERVED_SVBUS_SIZE; i++) 
            {
                out->reserved_svbus[i] = registerDescriptorsArr[i+3].value;
            }

            out->sec_eoi_reg         = registerDescriptorsArr[11].value;
            out->sec_enable_set_reg0 = registerDescriptorsArr[12].value;
            out->sec_enable_clr_reg0 = registerDescriptorsArr[13].value;
            out->ded_eoi_reg         = registerDescriptorsArr[14].value;
            out->ded_enable_set_reg0 = registerDescriptorsArr[15].value;
            out->ded_enable_clr_reg0 = registerDescriptorsArr[16].value;
            out->aggr_enable_set     = registerDescriptorsArr[17].value;
            out->aggr_enable_clr     = registerDescriptorsArr[18].value;
        }
    }

    return status;
}

IpFma_Status IpFma_EccAggr_CompareRegs(const IpFma_EccAggregatorRegs* expected,
                                       const IpFma_EccAggregatorRegs* actual)
{
     IpFma_Status status = IPFMA_OK;

    if (((NULL_PTR == expected)) || ((NULL_PTR == actual)))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if ((IPFMA_OK == status) && (expected->rev != actual->rev))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->vector != actual->vector))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->stat != actual->stat))
        {
            status = IPFMA_E_MISMATCH;
        }
        for (uint32_t i = 0; i < RESERVED_SVBUS_SIZE; i++)
        {
            if ((IPFMA_OK == status) && (expected->reserved_svbus[i] != actual->reserved_svbus[i]))
            {
                status = IPFMA_E_MISMATCH;
                break;
            }
        }
        if ((IPFMA_OK == status) && (expected->sec_eoi_reg != actual->sec_eoi_reg))
        {
            status = IPFMA_E_MISMATCH;
        }        
        if ((IPFMA_OK == status) && (expected->sec_enable_set_reg0 != actual->sec_enable_set_reg0))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->sec_enable_clr_reg0 != actual->sec_enable_clr_reg0))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ded_eoi_reg != actual->ded_eoi_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ded_enable_set_reg0 != actual->ded_enable_set_reg0))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ded_enable_clr_reg0 != actual->ded_enable_clr_reg0))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->aggr_enable_set != actual->aggr_enable_set))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->aggr_enable_clr != actual->aggr_enable_clr))
        {
            status = IPFMA_E_MISMATCH;
        }
    }
    return status;
}

/** @} */
