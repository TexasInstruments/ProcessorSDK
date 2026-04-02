/*
TEXAS INSTRUMENTS TEXT FILE LICENSE

Copyright (c) [2018] – [2019] Texas Instruments Incorporated

All rights reserved not granted herein.

Limited License.  

Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive license under copyrights and patents it now or hereafter owns or controls to make, have made, use, import, offer to sell and sell ("Utilize") this software subject to the terms herein.  With respect to the foregoing patent license, such license is granted  solely to the extent that any such patent is necessary to Utilize the software alone.  The patent license shall not apply to any combinations which include this software, other than combinations with devices manufactured by or for TI (“TI Devices”).  No hardware patent is licensed hereunder.

Redistributions must preserve existing copyright notices and reproduce this license (including the above copyright notice and the disclaimer and (if applicable) source code license limitations below) in the documentation and/or other materials provided with the distribution

Redistribution and use in binary form, without modification, are permitted provided that the following conditions are met:

*	No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any software provided in binary form.

*	any redistribution and use are licensed by TI for use only with TI Devices.

*	Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object code.

If software source code is provided to you, modification and redistribution of the source code are permitted provided that the following conditions are met:

*	any redistribution and use of the source code, including any resulting derivative works, are licensed by TI for use only with TI Devices.

*	any redistribution and use of any object code compiled from the source code and any resulting derivative works, are licensed by TI for use only with TI Devices.

Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or promote products derived from this software without specific prior written permission.

DISCLAIMER.

THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _DCC_COMP_H_
#define _DCC_COMP_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* ======================================================================= */
/* DCC_COMPLISTTABLE_TYPE - Table listing the available DCC components in
 * the system
 *
 * @param sCompName        : A NULL terminated string with max 128 characters
 *
 * @param pCompParse        : Component's Parser function
 */
/* ======================================================================= */
typedef struct
{
    char    *sCompName;
    int     (*comp_parse) (/*UInt8* b_sys_prm,
                           uint8_t* b_uc_prm,
                           uint8_t* b_parpack,*/
                           dcc_ptrs_t *dcc_ptrs,
                           void* sys_prm,
                           void* uc_prm,
                           void* parpack
                           /*UInt32 crc,
                           dcc_descriptor_id_type descId*/);
    void     (*comp_free) (void* sys_prm,
                           void* uc_prm,
                           void* parpack);
    void     (*comp_update) (void* dcc_data,
                             void* driver_data);
    void *  driver_ptr;
    int     struct_size;
    unsigned int isMultiPhSpaceSupported;
}DCC_COMPLISTTABLE_TYPE;

#define TRUE 1
#define FALSE 0

DCC_COMPLISTTABLE_TYPE tDCCCompList[] =
{
    {"DCC_ID_RESERVED_0", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_BLACK_CLAMP", blc_dcc_bin_parse, NULL, dcc_update_blc, NULL, sizeof(iss_black_level_subtraction), FALSE},
    {"DCC_ID_H3A_MUX_LUTS", h3a_mux_luts_bin_parse, NULL, dcc_update_h3a_mux_luts, NULL, sizeof(iss_h3a_mux_luts), FALSE},
    {"DCC_ID_H3A_AEWB_CFG", h3a_aewb_dcc_bin_parse, NULL, dcc_update_h3a_aewb, NULL, sizeof(iss_ipipe_h3a_aewb), FALSE},
    {"DCC_ID_RFE_DECOMPAND", iss_rfe_decompand_bin_parse, NULL, dcc_update_iss_rfe_decompand, NULL, sizeof(iss_rfe_decompand), FALSE},
    {"DCC_ID_MESH_LDC_J7", vpac_ldc_bin_parse, NULL, dcc_update_vpac_ldc, NULL, sizeof(vpac_ldc_dcc_cfg_t), FALSE},
    {"DCC_ID_VISS_GLBCE", viss_glbce_bin_parse, NULL, dcc_update_viss_glbce, NULL, sizeof(viss_glbce_dcc_cfg_t), FALSE},
    {"DCC_ID_VISS_LSC", viss_lsc_bin_parse, NULL, dcc_update_viss_lsc, NULL, sizeof(viss_lsc_dcc_cfg_t), FALSE},
    {"DCC_ID_VISS_DPC", viss_dpc_bin_parse, NULL, dcc_update_viss_dpc, NULL, sizeof(viss_dpc_dcc_cfg_t), FALSE},
    {"DCC_ID_IPIPE_CFA", cfa_dcc_bin_parse, NULL, dcc_update_cfa, NULL, sizeof(viss_ipipe_cfa_flxd), FALSE},
    {"DCC_ID_IPIPE_RGB_RGB_1", ipipe_rgb2rgb_dcc_bin_parse, NULL, dcc_update_ipipe_rgb2rgb, NULL, sizeof(iss_ipipe_rgb2rgb), TRUE},
    {"DCC_ID_RESERVED_11", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_VISS_GAMMA", viss_gamma_bin_parse, NULL, dcc_update_viss_gamma, NULL, sizeof(viss_gamma_dcc_cfg_t), FALSE},
    {"DCC_ID_RESERVED_13", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_14", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_15", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_VISS_YEE", viss_yee_bin_parse, NULL, dcc_update_viss_yee, NULL, sizeof(viss_yee_dcc_cfg_t), FALSE},
    {"DCC_ID_RESERVED_17", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_18", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_19", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_20", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_NSF4", viss_nsf4_dcc_bin_parse, NULL, dcc_update_viss_nsf4, NULL, sizeof(viss_nsf4), FALSE},
    {"DCC_ID_RESERVED_22", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_23", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_24", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RAWFE_WB1_VS", viss_rawfe_wb1vs_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_rawfe_wb1_dcc_cfg_t), FALSE},
    {"DCC_ID_RESERVED_26", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_27", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_28", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_29", NULL, NULL, NULL, NULL, 0, FALSE},
#if defined(VPAC3)
    {"DCC_ID_VISS_CFAI3_A", viss_cfai3a_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_cfai3_dcc_cfg_t), FALSE},
    {"DCC_ID_VISS_CFAI3_B", viss_cfai3b_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_cfai3_dcc_cfg_t), FALSE},
    {"DCC_ID_RESERVED_32", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_VISS_CAC", viss_cac_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_cac_dcc_cfg_t), FALSE},
    {"DCC_ID_VISS_RAWHIST", viss_rawhist_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_rawhist_dcc_cfg_t), FALSE},
    {"DCC_ID_VISS_CCMV", viss_ccmv_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_ccmv_dcc_cfg_t), FALSE},
#elif defined(VPAC3L)
    {"DCC_ID_VISS_CFAI3_A", viss_cfai3a_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_cfai3_dcc_cfg_t), FALSE},
    {"DCC_ID_RESERVED_31", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_32", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_VISS_CAC", viss_cac_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_cac_dcc_cfg_t), FALSE},
    {"DCC_ID_VISS_RAWHIST", viss_rawhist_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_rawhist_dcc_cfg_t), FALSE},
    {"DCC_ID_RESERVED_35", NULL, NULL, NULL, NULL, 0, FALSE},
#else
    {"DCC_ID_RESERVED_30", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_31", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_32", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_33", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_34", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_35", NULL, NULL, NULL, NULL, 0, FALSE},
#endif
#if defined(VPAC3L)
    {"DCC_ID_VISS_DPC_EXT", viss_dpc_ext_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_dpc_ext_dcc_cfg_t), FALSE},
    {"DCC_ID_VISS_LSC_EXT", viss_lsc_ext_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_lsc_ext_dcc_cfg_t), FALSE},
    {"DCC_ID_RESERVED_38", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_VISS_PCID",    viss_pcid_dcc_bin_parse, NULL, NULL, NULL, sizeof(viss_pcid_dcc_cfg_t), FALSE},
#else
    {"DCC_ID_RESERVED_36", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_37", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_38", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_39", NULL, NULL, NULL, NULL, 0, FALSE},
#endif
    {"DCC_ID_AAA_ALG_AWB_TI3", awb_alg_dcc_tuning_dcc_bin_parse, NULL, NULL, NULL, sizeof(dcc_awb_supp2_alg3_t), FALSE},
    {"DCC_ID_RESERVED_41", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_42", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_43", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_44", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_45", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_46", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_47", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_48", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_49", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_50", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_51", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_52", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_53", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_54", NULL, NULL, NULL, NULL, 0, FALSE},
    {"DCC_ID_RESERVED_55", NULL, NULL, NULL, NULL, 0, FALSE},
};

static const dcc_descriptor_id_type gDccModuleList[ISS_DCC_NUM_SUPPORT_MODULES] =
{
    DCC_ID_H3A_MUX_LUTS,
    DCC_ID_H3A_AEWB_CFG,
    DCC_ID_RFE_DECOMPAND,
    DCC_ID_IPIPE_RGB_RGB_1,
    DCC_ID_NSF4,
    DCC_ID_AAA_ALG_AWB_TI3,
    DCC_ID_BLACK_CLAMP,
    DCC_ID_IPIPE_CFA,
    DCC_ID_MESH_LDC_J7,
    DCC_ID_VISS_GLBCE,
    DCC_ID_VISS_LSC,
    DCC_ID_VISS_YEE,
    DCC_ID_VISS_DPC,
    DCC_ID_RAWFE_WB1_VS,
    DCC_ID_VISS_GAMMA,
#if defined(VPAC3)
    DCC_ID_VISS_CAC,
    DCC_ID_VISS_RAWHIST,
    DCC_ID_VISS_CFAI3_A,
    DCC_ID_VISS_CFAI3_B,
    DCC_ID_VISS_CCMV,
#elif defined(VPAC3L)
    DCC_ID_VISS_CAC,
    DCC_ID_VISS_RAWHIST,
    DCC_ID_VISS_CFAI3_A,
#endif
#if defined(VPAC3L)
    DCC_ID_VISS_DPC_EXT,
    DCC_ID_VISS_LSC_EXT,
    DCC_ID_VISS_PCID,
#endif
};


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*_DCC_COMP_H_*/
