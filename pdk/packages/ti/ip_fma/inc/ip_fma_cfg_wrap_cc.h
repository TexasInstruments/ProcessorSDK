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
 *  \defgroup IP_FMA_CFG_WRAP_CC Compute Cluster Configuration Wrap CC
 *            static Register Check Wrappers
 */

/**
 *  \ingroup  IP_FMA_CFG_WRAP_CC
 *  \defgroup IP_FMA_CFG_WRAP_CC_INTERFACE CFG_WRAP_CC Register Check Interface
 *
 *  @{
 */

/**
 *  \file     ip_fma_cfg_wrap_cc.h
 *
 *  \brief    CFG_WRAP_CC-specific wrappers over IP_FMA Register Check
 *            for readback of static configuration registers.
 *
 */

#ifndef IP_FMA_CFG_WRAP_CC_H_
#define IP_FMA_CFG_WRAP_CC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ip_fma_common.h"

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief Compute Cluster Config Wrap CC registers snapshot/expected set for CFG_WRAP_CC.
 */
typedef struct
{
    uint32_t arm_0_rev;        
    uint32_t arm_1_rev;    
    uint32_t arm_0_pbist_bisor_cntrl;                    
    uint32_t arm_1_pbist_bisor_cntrl;     
    uint32_t cc_cntrl;                       
} IpFma_CfgWrapCCRegs;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief Read Compute Cluster Config Wrap CC registers into \c out.
 *
 * \param baseAddr Pointer to the base address of the group of registers.
 * \param out      Output structure to be filled with actual values.
 *
 * \return \ref IPFMA_OK on success, error code otherwise.
 */
IpFma_Status IpFma_CfgWrapCC_GetRegs(uintptr_t baseAddr, IpFma_CfgWrapCCRegs* out);

/**
 * \brief Compare Compute Cluster Config Wrap CC registers: \c expected vs \c actual (no HW).
 *
 * \param expected  Input structure with expected values.
 * \param actual    Input structure with actual values (from Get).
 *
 * \return \ref IPFMA_OK if all match.
 *         \ref IPFMA_E_MISMATCH on first mismatch
 *              or another error code.
 */
IpFma_Status IpFma_CfgWrapCC_CompareRegs(const IpFma_CfgWrapCCRegs* expected,
                                         const IpFma_CfgWrapCCRegs* actual);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef IP_FMA_CFG_WRAP_CC_H_ */

/** @} */
