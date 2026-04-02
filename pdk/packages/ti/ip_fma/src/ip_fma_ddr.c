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
 *  \file     ip_fma_ddr.c
 *
 *  \brief    DDR-specific wrappers over common IP_FMA Register Check API.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "ip_fma_common.h"
#include "ip_fma_ddr.h"
#include <ti/csl/cslr_emif.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

IpFma_Status IpFma_Ddr_GetSubsystemRegs(uintptr_t baseAddr,
                                        IpFma_DdrSubsystemRegs* out)
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
            { CSL_EMIF_SSCFG_SS_CTL_REG,                0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_CTL_REG,               0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_R1_MAT_REG,            0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_R2_MAT_REG,            0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_R3_MAT_REG,            0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_LPT_DEF_PRI_MAP_REG,   0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_LPT_R1_PRI_MAP_REG,    0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_LPT_R2_PRI_MAP_REG,    0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_LPT_R3_PRI_MAP_REG,    0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_HPT_DEF_PRI_MAP_REG,   0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_HPT_R1_PRI_MAP_REG,    0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_HPT_R2_PRI_MAP_REG,    0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_HPT_R3_PRI_MAP_REG,    0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_HZRD_CTL_REG,          0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_LEAKY_THRESH_REG,      0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_DRAIN_THRESH_REG,      0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_BUS_TO,                0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_CTRL_REG,              0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_RID_INDX_REG,          0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_RID_VAL_REG,           0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_R0_STR_ADDR_REG,       0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_R0_END_ADDR_REG,       0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_R1_STR_ADDR_REG,       0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_R1_END_ADDR_REG,       0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_R2_STR_ADDR_REG,       0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_R2_END_ADDR_REG,       0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_PHY_BIST_CTRL_REG,         0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_V2A_INT_SET_REG,           0u, IPFMA_WIDTH_32 },
            { CSL_EMIF_SSCFG_ECC_1B_ERR_THRSH_REG,      0u, IPFMA_WIDTH_32 }
        };

        status = IpFma_GetRegsValues(baseAddr, registerDescriptorsArr, 
                                     (uint32_t)(sizeof(registerDescriptorsArr) / sizeof(registerDescriptorsArr[0])));

        if (IPFMA_OK == status)
        {
            out->ss_ctl_reg               = registerDescriptorsArr[0].value;
            out->v2a_ctl_reg              = registerDescriptorsArr[1].value;
            out->v2a_r1_mat_reg           = registerDescriptorsArr[2].value;
            out->v2a_r2_mat_reg           = registerDescriptorsArr[3].value;
            out->v2a_r3_mat_reg           = registerDescriptorsArr[4].value;
            out->v2a_lpt_def_pri_map_reg  = registerDescriptorsArr[5].value;
            out->v2a_lpt_r1_pri_map_reg   = registerDescriptorsArr[6].value;
            out->v2a_lpt_r2_pri_map_reg   = registerDescriptorsArr[7].value;
            out->v2a_lpt_r3_pri_map_reg   = registerDescriptorsArr[8].value;
            out->v2a_hpt_def_pri_map_reg  = registerDescriptorsArr[9].value;
            out->v2a_hpt_r1_pri_map_reg   = registerDescriptorsArr[10].value;
            out->v2a_hpt_r2_pri_map_reg   = registerDescriptorsArr[11].value;
            out->v2a_hpt_r3_pri_map_reg   = registerDescriptorsArr[12].value;
            out->v2a_hzrd_ctl_reg         = registerDescriptorsArr[13].value;
            out->v2a_leaky_thresh_reg     = registerDescriptorsArr[14].value;
            out->v2a_drain_thresh_reg     = registerDescriptorsArr[15].value;
            out->v2a_bus_to               = registerDescriptorsArr[16].value;
            out->ecc_ctrl_reg             = registerDescriptorsArr[17].value;
            out->ecc_rid_indx_reg         = registerDescriptorsArr[18].value;
            out->ecc_rid_val_reg          = registerDescriptorsArr[19].value;
            out->ecc_r0_str_addr_reg      = registerDescriptorsArr[20].value;
            out->ecc_r0_end_addr_reg      = registerDescriptorsArr[21].value;
            out->ecc_r1_str_addr_reg      = registerDescriptorsArr[22].value;
            out->ecc_r1_end_addr_reg      = registerDescriptorsArr[23].value;
            out->ecc_r2_str_addr_reg      = registerDescriptorsArr[24].value;
            out->ecc_r2_end_addr_reg      = registerDescriptorsArr[25].value;
            out->phy_bist_ctrl_reg        = registerDescriptorsArr[26].value;
            out->v2a_int_set_reg          = registerDescriptorsArr[27].value;
            out->ecc_1b_err_thrsh_reg     = registerDescriptorsArr[28].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Ddr_CompareSubsystemRegs(const IpFma_DdrSubsystemRegs* expected,
                                            const IpFma_DdrSubsystemRegs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if (((NULL_PTR == expected)) || ((NULL_PTR == actual)))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if ((expected->ss_ctl_reg != actual->ss_ctl_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_ctl_reg != actual->v2a_ctl_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_r1_mat_reg != actual->v2a_r1_mat_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_r2_mat_reg != actual->v2a_r2_mat_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_r3_mat_reg != actual->v2a_r3_mat_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_lpt_def_pri_map_reg != actual->v2a_lpt_def_pri_map_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_lpt_r1_pri_map_reg != actual->v2a_lpt_r1_pri_map_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_lpt_r2_pri_map_reg != actual->v2a_lpt_r2_pri_map_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_lpt_r3_pri_map_reg != actual->v2a_lpt_r3_pri_map_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_hpt_def_pri_map_reg != actual->v2a_hpt_def_pri_map_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_hpt_r1_pri_map_reg != actual->v2a_hpt_r1_pri_map_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_hpt_r2_pri_map_reg != actual->v2a_hpt_r2_pri_map_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_hpt_r3_pri_map_reg != actual->v2a_hpt_r3_pri_map_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_hzrd_ctl_reg != actual->v2a_hzrd_ctl_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_leaky_thresh_reg != actual->v2a_leaky_thresh_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_drain_thresh_reg != actual->v2a_drain_thresh_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_bus_to != actual->v2a_bus_to))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_ctrl_reg != actual->ecc_ctrl_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_rid_indx_reg != actual->ecc_rid_indx_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_rid_val_reg != actual->ecc_rid_val_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_r0_str_addr_reg != actual->ecc_r0_str_addr_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_r0_end_addr_reg != actual->ecc_r0_end_addr_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_r1_str_addr_reg != actual->ecc_r1_str_addr_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_r1_end_addr_reg != actual->ecc_r1_end_addr_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_r2_str_addr_reg != actual->ecc_r2_str_addr_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_r2_end_addr_reg != actual->ecc_r2_end_addr_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->phy_bist_ctrl_reg != actual->phy_bist_ctrl_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->v2a_int_set_reg != actual->v2a_int_set_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->ecc_1b_err_thrsh_reg != actual->ecc_1b_err_thrsh_reg))
        {
            status = IPFMA_E_MISMATCH;
        }
    }
    return status;
}

/** @} */
