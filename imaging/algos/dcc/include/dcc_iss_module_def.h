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

#ifndef _DCC_ISS_MODULE_DEF_H_
#define _DCC_ISS_MODULE_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(VPAC3L)
#define ISS_DCC_NUM_SUPPORT_MODULES        (21U)
#elif defined(VPAC3)
#define ISS_DCC_NUM_SUPPORT_MODULES        (20U)
#else // VPAC1
#define ISS_DCC_NUM_SUPPORT_MODULES        (15U)
#endif

/* IPIPE RGB2RGB Module Structure */
typedef struct
{
    int16_t matrix[3][4];
    int16_t offset[3];
} iss_ipipe_rgb2rgb;


/* VISS H3A_AEWB Module Structure */
typedef struct
{
    uint8_t enable;
    uint8_t mode;
    uint16_t v_start;
    uint16_t h_start;
    uint8_t v_size;
    uint8_t h_size;
    uint8_t v_count;
    uint8_t h_count;
    uint8_t v_skip;
    uint8_t h_skip;
    uint16_t saturation_limit;
    uint16_t blk_win_numlines;
    uint16_t blk_row_vpos;
    uint8_t sum_shift;
    uint8_t ALaw_En;
    uint8_t MedFilt_En;
} iss_ipipe_h3a_aewb;

/* VISS BLC Module Structure */
typedef struct
{
    int16_t vs_dcoffset[4];
    int16_t s_dcoffset[4];
    int16_t l_dcoffset[4];
} iss_black_level_subtraction;

/* RAWFE H3A MUX and LUTs */
typedef struct
{
    uint16_t enable;
    uint16_t h3a_mux_lut_num;
    uint16_t h3a_mux_lut[3][639];
} iss_h3a_mux_luts;

/* RAWFE wdr decompand with vshort */
typedef struct
{
    uint16_t enable;
    uint16_t mask;
    uint16_t shift;
    uint16_t bit_depth;
    uint16_t clip;
    uint16_t lut[639];
} iss_rfe_decompand;

/* VISS NSF4 Module Structure */
typedef struct
{
    int32_t enable;
    int32_t mode;
    int32_t shading_gain;

    int32_t u1_knee;
    int32_t tn1;
    int32_t tn2;
    int32_t tn3;

    int32_t noise_thr_x[4][12];
    int32_t noise_thr_y[4][12];
    int32_t noise_thr_s[4][12];

    int32_t shd_x;
    int32_t shd_y;
    int32_t shd_t;
    int32_t shd_kh;
    int32_t shd_kv;
    int32_t shd_gmax;
    int32_t shd_set_sel;

    int32_t shd_lut_x[2][16];
    int32_t shd_lut_y[2][16];
    int32_t shd_lut_s[2][16];

    int32_t wb_gains[4];
} viss_nsf4;

/* VISS RFE CFA Module Structure */

#define FLXD_NUM_PHASE        4
#define FLXD_FIRSIZE_H        6
#define FLXD_FIRSIZE_W        6
#define FLXD_NUM_FIR          4
#define FLXD_NUM_THR          7
#define FLXD_LUT_SIZE        (639)


typedef struct
{
  int32_t matrix[FLXD_NUM_PHASE][FLXD_FIRSIZE_H * FLXD_FIRSIZE_W];
} FLXD_FirCoefs_DCC;

typedef struct
{
  uint32_t             bitWidth;
  uint32_t             lut_enable;

  uint32_t             Set0GradHzMask[4];
  uint32_t             Set0GradVtMask[4];
  uint32_t             Set0IntensityMask[4];
  uint32_t             Set0IntensityShift[4];
  uint32_t             Set0Thr[FLXD_NUM_THR];

  uint32_t             Set1GradHzMask[4];
  uint32_t             Set1GradVtMask[4];
  uint32_t             Set1IntensityMask[4];
  uint32_t             Set1IntensityShift[4];
  uint32_t             Set1Thr[FLXD_NUM_THR];
  uint32_t             blendMode[FLXD_NUM_FIR];
  uint32_t             bitMaskSel[FLXD_NUM_FIR];

  FLXD_FirCoefs_DCC   FirCoefs[FLXD_NUM_FIR *3];
  uint32_t  ToneLut[FLXD_LUT_SIZE];

} viss_ipipe_cfa_flxd;

typedef struct
{
    uint16_t en;              // LD enable
    uint16_t ldmapen;         // LD back mapping enable
    uint16_t data_mode;       // LD input data mode
    uint16_t out_in_420;      // LD 422 to 420 conversion
    uint16_t ip_dfmt;         // LD input pixel format
    uint16_t pwarpen;         // PWARP enable
    uint16_t ld_yint_typ;     // Interpolation method for Y data.  0: Bicubic, 1: Bilinear
    uint16_t regmode_en;      // Region mode enable.  0: off, 1: on
    uint16_t table_m;         // Table horizontal subsampling factor, 2^m
    uint16_t mesh_frame_w;    // mesh frame window height
    uint16_t mesh_frame_h;    // mesh frame window width
    uint16_t compute_sizew;   // compute window height, in pixels
    uint16_t compute_sizeh;   // compute window width, in pixels
    uint16_t ld_initx;        // compute window starting y, in pixels
    uint16_t ld_inity;        // compute window starting x, in pixels
    uint16_t iw;              // source (distorted) image width, in pixels
    uint16_t ih;              // source (distorted) image height, in pixels
    uint16_t ld_obw;          // output block height, in pixels, for block processing
    uint16_t ld_obh;          // output block height, in pixels, for block processing
    uint16_t ld_pad;          // pixel padding to determine input block for block processing

    int16_t affine_a;
    int16_t affine_b;
    int16_t affine_c;
    int16_t affine_d;
    int16_t affine_e;
    int16_t affine_f;
    int16_t affine_g;
    int16_t affine_h;

    uint16_t ld_sf_width[3];     // subframe width
    uint16_t ld_sf_height[3];    // subframe height
    uint16_t ld_sf_en [3][3];    // subframe enable
    uint16_t ld_sf_obw[3][3];    // output block height, in pixels, for block processing
    uint16_t ld_sf_obh[3][3];    // output block height, in pixels, for block processing
    uint16_t ld_sf_pad[3][3];    // pixel padding to determine input block for block processing

    uint16_t ylut_en;
    uint16_t yin_bits;
    uint16_t yout_bits;
    uint16_t clut_en;
    uint16_t cin_bits;
    uint16_t cout_bits;
    uint16_t ylut[513];
    uint16_t clut[513];
    uint32_t mesh_table_pitch;   // table row pitch in bytes
    uint32_t mesh_table_size;    // # of elements in "uint16_t mesh_table[]"
    uint32_t mesh_table_dccsize; // size from DCC
} vpac_ldc_dcc_params_t;

typedef struct
{
    vpac_ldc_dcc_params_t ldc_dcc_params;
    uint16_t  *mesh_table;
#if defined(VPAC3) || defined(VPAC3L)
    uint32_t chroma_ctl_en;
    uint32_t chroma_ctl_format;
#endif
} vpac_ldc_dcc_cfg_t;

typedef struct
{
    uint32_t strength;
    uint32_t intensity_var;
    uint32_t space_var;
    uint32_t slope_min_lim;
    uint32_t slope_max_lim;
    uint32_t fwd_prcpt_en;
    uint32_t fwd_prcpt_lut[65];
    uint32_t rev_prcpt_en;
    uint32_t rev_prcpt_lut[65];
    uint32_t asym_lut[33];
} viss_glbce_dcc_cfg_t;

typedef struct
{
    uint32_t enable;
    uint32_t gain_mode_m;
    uint32_t gain_mode_n;
    uint32_t gain_mode_format;
    uint32_t lut_size_in_bytes;
    uint32_t lut_size_in_dcc;
} viss_lsc_dcc_params_t;

typedef struct
{
    viss_lsc_dcc_params_t lsc_params;
    uint8_t               * lsc_table;
} viss_lsc_dcc_cfg_t;

/* YEE Module Structure */
typedef struct
{
    uint16_t enable;
    uint16_t halo_reduction_enable;
    int16_t  ee_2d_filter_coeff[9];
    uint16_t merge_select;
    uint16_t shift_amount;
    uint16_t threshold_before_lut;
    uint16_t edge_sharpener_gain;
    uint16_t edge_sharpener_hpf_low_thresh;
    uint16_t edge_sharpener_hpf_high_thresh;
    uint16_t edge_sharpener_gradient_gain;
    uint16_t edge_sharpener_gradient_offset;
    int16_t  edge_intensity_lut[4096];
} viss_yee_dcc_cfg_t;

/* DPC Module Structure */
typedef struct
{
    uint16_t enable;
    uint16_t thr_0;
    int16_t  slp_0;
    uint16_t thr_512;
    int16_t  slp_512;
    uint16_t thr_1024;
    int16_t  slp_1024;
    uint16_t thr_2048;
    int16_t  slp_2048;
    uint16_t thr_4096;
    int16_t  slp_4096;
    uint16_t thr_8192;
    int16_t  slp_8192;
    uint16_t thr_16384;
    int16_t  slp_16384;
    uint16_t thr_32768;
    int16_t  slp_32768;
} viss_dpc_dcc_cfg_t;

/* Gamma LUT Structure */
typedef struct
{
	uint32_t enable;
	uint32_t bitdepth;
	uint32_t lut[513];
}viss_gamma_dcc_cfg_t;

#if defined(VPAC3) || defined(VPAC3L)
/* CAC Module Structure */
#define CAC_LUT_MEM_SIZE  (8192)
typedef struct
{
    uint32_t enable;
    uint32_t color_en;
    uint32_t block_s;
    uint32_t grid_w;
    uint32_t grid_h;
    uint32_t lut_size_in_bytes;
    uint8_t  cac_lut[CAC_LUT_MEM_SIZE];
} viss_cac_dcc_cfg_t;

/* RawHist Module Structure */
typedef struct
{
    uint16_t enable;
    uint16_t color_en;
    uint16_t lut_en;
    uint16_t lut_bits;
    uint16_t roi_en[8];
    uint16_t roi_h_start[8];
    uint16_t roi_h_end[8];
    uint16_t roi_v_start[8];
    uint16_t roi_v_end[8];
    uint16_t rawhist_lut[609];
} viss_rawhist_dcc_cfg_t;

/* CFAI3 Module Structure */
#define CFAI3_DCMPD_LUT_SIZE  (609)
typedef struct
{
    uint32_t process_mode;
    uint32_t out_scaler[4];
    uint32_t out_offset[4];
    uint32_t dcomp_lut_en;
    uint32_t dcomp_lut_bw;
    uint32_t comp_lut_en;
    uint32_t ccm_en;
    int32_t  ccm[4][5];

    uint32_t ccmlut_dcmpd_0[CFAI3_DCMPD_LUT_SIZE];
    uint32_t ccmlut_dcmpd_1[CFAI3_DCMPD_LUT_SIZE];
    uint32_t ccmlut_dcmpd_2[CFAI3_DCMPD_LUT_SIZE];
    uint32_t ccmlut_dcmpd_3[CFAI3_DCMPD_LUT_SIZE];

    uint32_t ccmlut_compd_0[FLXD_LUT_SIZE];
    uint32_t ccmlut_compd_1[FLXD_LUT_SIZE];
    uint32_t ccmlut_compd_2[FLXD_LUT_SIZE];
    uint32_t ccmlut_compd_3[FLXD_LUT_SIZE];
} viss_cfai3_dcc_ext;

typedef struct
{
    viss_ipipe_cfa_flxd cfg_cfai1;
    viss_cfai3_dcc_ext  cfg_cfai3;
} viss_cfai3_dcc_cfg_t;

/* VPAC3 CC for MV Module Structure */
typedef struct
{
    uint16_t muxC1_4;
    uint16_t muxY12out;
    uint16_t muxY8out;
    int16_t ccm1[3][5];
    uint16_t contrast_en;
    uint16_t contrast_clip;
    int16_t rgb2yuv[3][4];
} viss_ccmv_dcc_cfg_t;
#endif

#if defined(VPAC3L)
typedef struct
{
    uint32_t lsc_cfg_mode;
    uint8_t lsc_ch2lut_map[16];
} viss_lsc_ext_dcc_cfg_t;

typedef struct
{
    uint32_t dpc_cfa_mode;
    uint32_t dpc_cfa_phase;
    uint32_t dpc_detection_only;
    uint32_t dpc_stats_cfg;
    uint32_t dpc_lut_map;
    uint16_t dpc_lut_0[8][3];
    uint16_t dpc_lut_1[8][3];
} viss_dpc_ext_dcc_cfg_t;

typedef struct
{
    uint32_t pcid_i_fmt;
    uint32_t pcid_o_fmt;
    uint32_t pcid_interp_method;    // for compatibility and must be ignored in PSDK
    uint32_t pcid_ha_th1;
    uint32_t pcid_ha_th2;
    uint32_t pcid_ha_th3;
    uint32_t pcid_hfx_scale;
    uint32_t pcid_hfx_scale_ir;
    uint32_t pcid_irsub_scale[4];
    uint32_t pcid_irsub_cutoff;
    uint32_t pcid_irsub_trans_bw;
    uint32_t pcid_irsub_trans_bw_recip;
    uint32_t pcid_dist_factor[5];
    uint16_t pcid_remap_en;
    uint16_t pcid_remap_lutp[609];
} viss_pcid_dcc_cfg_t;

#endif


typedef struct
{
    uint16_t gain[4];   //U13Q9
} viss_rawfe_wb1_dcc_cfg_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DCC_ISS_MODULE_DEF_H_ */
