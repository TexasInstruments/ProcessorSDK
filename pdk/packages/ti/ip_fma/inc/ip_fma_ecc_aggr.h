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
 *  \defgroup IP_FMA_ECC_AGGR Ecc Aggregator Register Check Wrappers
 */

/**
 *  \ingroup  IP_FMA_ECC_AGGR
 *  \defgroup IP_FMA_ECC_AGGR_IMPLEMENTATION Ecc Aggregator Register Check Interface
 *
 *  @{
 */

/**
 *  \file     ip_fma_ecc_aggr.h
 *
 *  \brief    ECC Aggregator register structure and read/compare API 
 *            declarations.
 *
 */

#ifndef IPFMA_ECC_AGG_H
#define IPFMA_ECC_AGG_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#include "ip_fma_common.h"

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief ECC aggregator register group snapshot/expected set for DDR.
 */
typedef struct
{
    uint32_t rev;
    uint32_t vector;
    uint32_t stat;
    uint32_t reserved_svbus[8];
    uint32_t sec_eoi_reg;
    uint32_t sec_enable_set_reg0;
    uint32_t sec_enable_clr_reg0;
    uint32_t ded_eoi_reg;
    uint32_t ded_enable_set_reg0;
    uint32_t ded_enable_clr_reg0;
    uint32_t aggr_enable_set;
    uint32_t aggr_enable_clr;
} IpFma_EccAggregatorRegs;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Read ECC Aggregator registers into \c out.
 *
 *  \param baseAddr Pointer to the base address of the group of registers.
 *  \param out      Output structure to be filled with actual values.
 *
 *  \return \ref IPFMA_OK on success, error code otherwise.
 */
IpFma_Status IpFma_EccAggr_GetRegs(uintptr_t baseAddr,
                                   IpFma_EccAggregatorRegs* out);

/**
 *  \brief Compare Aggregator registers: \c expected vs \c actual (no HW).
 *
 *  \param expected  Input structure with expected values.
 *  \param actual    Input structure with actual values (from Get).
 *
 *  \return \ref IPFMA_OK if all match,
 *          \ref IPFMA_E_MISMATCH on first mismatch,
 *               or another error code.
 */
IpFma_Status IpFma_EccAggr_CompareRegs(const IpFma_EccAggregatorRegs* expected,
                                       const IpFma_EccAggregatorRegs* actual);


/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef IPFMA_ECC_AGG_H */

/** @} */