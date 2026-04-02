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
 *  \ingroup  IP_FMA_DDR
 *  \defgroup IP_FMA_DDR_IMPLEMENTATION module Register Check Implementation
 *
 *  @{
 */

/**
 *  \file     ip_fma_ddr.h
 *
 *  \brief    DDR-specific wrappers over common IP_FMA Register Check API.
 *
 */

#ifndef IPFMA_DDR_H_
#define IPFMA_DDR_H_

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
 *  \brief DDR Subsystem register group snapshot/expected set for DDR.
 */
typedef struct
{
    uint32_t ss_ctl_reg;
    uint32_t v2a_ctl_reg;
    uint32_t v2a_r1_mat_reg;
    uint32_t v2a_r2_mat_reg;
    uint32_t v2a_r3_mat_reg;
    uint32_t v2a_lpt_def_pri_map_reg;
    uint32_t v2a_lpt_r1_pri_map_reg;
    uint32_t v2a_lpt_r2_pri_map_reg;
    uint32_t v2a_lpt_r3_pri_map_reg;
    uint32_t v2a_hpt_def_pri_map_reg;
    uint32_t v2a_hpt_r1_pri_map_reg;
    uint32_t v2a_hpt_r2_pri_map_reg;
    uint32_t v2a_hpt_r3_pri_map_reg;
    uint32_t v2a_hzrd_ctl_reg;
    uint32_t v2a_leaky_thresh_reg;
    uint32_t v2a_drain_thresh_reg;
    uint32_t v2a_bus_to;
    uint32_t ecc_ctrl_reg;
    uint32_t ecc_rid_indx_reg;
    uint32_t ecc_rid_val_reg;
    uint32_t ecc_r0_str_addr_reg;
    uint32_t ecc_r0_end_addr_reg;
    uint32_t ecc_r1_str_addr_reg;
    uint32_t ecc_r1_end_addr_reg;
    uint32_t ecc_r2_str_addr_reg;
    uint32_t ecc_r2_end_addr_reg;
    uint32_t phy_bist_ctrl_reg;
    uint32_t v2a_int_set_reg;
    uint32_t ecc_1b_err_thrsh_reg;
} IpFma_DdrSubsystemRegs;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Read DDR Subsystem registers into \c out.
 *
 *  \param baseAddr Pointer to the base address of the group of registers.
 *  \param out      Output structure to be filled with actual values.
 *
 *  \return \ref IPFMA_OK on success, error code otherwise.
 */
IpFma_Status IpFma_Ddr_GetSubsystemRegs(uintptr_t baseAddr,
                                        IpFma_DdrSubsystemRegs* out);

/**
 *  \brief Compare DDR Subsystem registers: \c expected vs \c actual (no HW).
 *
 *  \param expected  Input structure with expected values.
 *  \param actual    Input structure with actual values (from Get).
 *
 *  \return \ref IPFMA_OK if all match,
 *          \ref IPFMA_E_MISMATCH on first mismatch,
 *               or another error code.
 */
IpFma_Status IpFma_Ddr_CompareSubsystemRegs(const IpFma_DdrSubsystemRegs* expected,
                                            const IpFma_DdrSubsystemRegs* actual);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef IPFMA_DDR_H_ */

/** @} */
