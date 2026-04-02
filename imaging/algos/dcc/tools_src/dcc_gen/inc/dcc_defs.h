/* =============================================================================
* MultiMedia Solutions AD
* (c) Copyright 2010, MultiMedia Solutions AD All Rights Reserved.
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied.
* =========================================================================== */
/**
* @file dcc_defs.h
*
*
* ^path (TOP)/read_write_bin_dcc
*
* @author Nevena Milanova (MultiMedia Solutions AD)
*
* @date 27.04.2010
*
* @version 1.00
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*!
*!  27-Apr-2010 : Nevena Milanova (MultiMedia Solutions AD)
*!  Created
*!
* =========================================================================== */

/* =============================================================================
*                                  INCLUDE FILES
* =========================================================================== */

#ifndef __DCC_DEFS_H__
#define __DCC_DEFS_H__

#include "ctypes.h"

//
//  Following enumerated list represents main component/sub-component/algorithm IDs.
//  DCC Descriptor ID - enum dcc_descriptor_id_type
typedef enum {
    DCC_ID_ISIF_CSC,                    // 0
    DCC_ID_ISIF_BLACK_CLAMP,            // 1
    DCC_ID_LSC,                         // 2
    DCC_ID_H3A_AEWB_CFG,                // 3
    DCC_ID_IPIPE_DPC_OTF,               // 4
    DCC_ID_IPIPE_NOISE_FILTER_1,        // 5
    DCC_ID_IPIPE_NOISE_FILTER_2,        // 6
    DCC_ID_IPIPE_NOISE_FILTER_LDC,      // 7 - not used
    DCC_ID_IPIPE_GIC,                   // 8
    DCC_ID_IPIPE_CFA,                   // 9
    DCC_ID_IPIPE_RGB_RGB_1,             //10 - RGB2RGB before gamma - same parsers and types as RGB_RGB_2
    DCC_ID_IPIPE_GAMMA,                 //11
    DCC_ID_IPIPE_RGB_RGB_2,             //12 - RGB2RGB after gamma - same parsers and types as RGB_RGB_1
    DCC_ID_IPIPE_3D_LUT,                //13
    DCC_ID_IPIPE_GBCE,                  //14
    DCC_ID_IPIPE_RGB_TO_YUV,            //15
    DCC_ID_IPIPE_EE,                    //16
    DCC_ID_IPIPE_CAR,                   //17
    DCC_ID_IPIPE_CGS,                   //18
    DCC_ID_IPIPE_YUV444_YUV422,         //19
    DCC_ID_IPIPE_RSZ,                   //20
    DCC_ID_NSF_CFG,                     //21 - NSF2 (NSF in NSFLDC library)
    DCC_ID_LDC_ODC,                     //22 - LDC for ODC in NSFLDC library - same parsers and types as LDC_CAC
    DCC_ID_LDC_CAC,                     //23 - LDC for CAC in NSFLDC library - same parsers and types as LDC_ODC
    DCC_ID_LBCE_1,                      //24
    DCC_ID_ADJUST_RGB2RGB,              //25
    DCC_ID_VNF_CFG, 					//26                             
    DCC_ID_AAA_ALG_AE,                  //27
    DCC_ID_AAA_ALG_AWB,                 //28 AWB2
    DCC_ID_AAA_ALG_AF_HLLC,             //29
    DCC_ID_AAA_ALG_AF_AFFW,             //30
    DCC_ID_AAA_ALG_AF_SAF,              //31
    DCC_ID_AAA_ALG_AF_CAF,              //32
    DCC_ID_ISS_SCENE_MODES,             //33
    DCC_ID_ISS_EFFECT_MODES,            //34
    DCC_ID_ISS_GBCE1,                   //35
    DCC_ID_ISS_GBCE2,                   //36
    DCC_ID_IPIPE_DPC_LUT,               //37
    DCC_ID_3D_MMAC_SAC,                 //38 - 3D mechanical misalignement and stereo auto convergence
    DCC_ID_IPIPE_LSC,                   //39
    DCC_ID_AAA_ALG_AWB3,                //40 AWB3
    DCC_ID_COUNT
} dcc_descriptor_id_type;

//
//  Following enumerated list represents main Algorithm vendor's ID.
//  Algorithm Vendor ID - enum dcc_algorithm_vendor_id_type
typedef enum {
    DCC_ALG_VENDOR_ID_1,
    DCC_ALG_VENDOR_ID_2,
    DCC_ALG_VENDOR_ID_3,
    DCC_ALG_VENDOR_ID_4,
    DCC_ALG_VENDOR_ID_5,
    DCC_ALG_VENDOR_ID_6,
    DCC_ALG_VENDOR_ID_7,
    DCC_ALG_VENDOR_ID_8,
    DCC_ALG_VENDOR_ID_9,
    DCC_ALG_VENDOR_ID_10
} dcc_algorithm_vendor_id_type;

//
//  Following enumerated list represents main DCC Use Case IDs.
//  DCC Use Case ID - enum dcc_use_case_id_type
typedef enum {
    DCC_USE_CASE_NONE = 0,
    DCC_USE_CASE_HIGH_SPEED_PREVIEW =           (1 << 0),
    DCC_USE_CASE_HIGH_QUALITY_PREVIEW =         (1 << 1),
    DCC_USE_CASE_HIGH_SPEED_STILL_CAPTURE =     (1 << 2),
    DCC_USE_CASE_HIGH_QUALITY_STILL_CAPTURE =   (1 << 3),
    DCC_USE_CASE_HIGH_SPEED_VIDEO_RECORD =      (1 << 4),
    DCC_USE_CASE_HIGH_QUALITY_VIDEO_RECORD =    (1 << 5),
    DCC_USE_CASE_VIDEO_TELECONFERENCE =         (1 << 6),
    DCC_USE_CASE_STILL_IMAGE_PLAYBACK =         (1 << 7),
    DCC_USE_CASE_STEREO_STILL_IMAGE_CAPTURE =   (1 << 8),
    DCC_USE_CASE_STEREO_VIDEO_CAPTURE =         (1 << 9)
} dcc_use_case_id_type;

//
//  Following enumerated list represents main Photo Space Dimension IDs.
//  Photo Space Dimension ID - enum dcc_photospace_dimension_id_type
typedef enum {
    DCC_PS_DIM_ID_AG,
    DCC_PS_DIM_ID_ET,
    DCC_PS_DIM_ID_CT,
    DCC_PS_DIM_ID_FLASH,
    DCC_PS_DIM_ID_FOCUS,
    DCC_PS_DIM_ID_TOTAL_EXP,
    DCC_PS_DIM_ID_FACE_DETECT,
    DCC_PS_DIM_ID_SCENE_MODE,
    DCC_PS_DIM_ID_EFFECTS_MODE,
    DCC_PS_DIM_ID_RESERVED_1,
    DCC_PS_DIM_ID_RESERVED_2,
    DCC_PS_DIM_ID_RESERVED_3,
    DCC_PS_DIM_ID_COUNT
} dcc_photospace_dimension_id_type;

//
//  Following structure represents metadata information related to the particular file instance.
//  Dynamic Camera Control Profile Header - struct dcc_component_header_type
typedef struct {
    uint32                              camera_module_id;
    uint32                              dcc_descriptor_id;
    uint32                              algorithm_vendor_id;
    uint32                              dcc_tuning_tool_version;
    uint32                              dcc_profile_time_stamp;
    uint32                              crc_checksum;
    uint32                              dcc_reserved_0;
    uint32                              dcc_reserved_1;
    uint32                              dcc_reserved_2;
    uint32                              dcc_reserved_3;
    uint32                              dcc_reserved_4;
    uint32                              dcc_reserved_5;
    uint32                              dcc_reserved_6;
    uint32                              dcc_reserved_7;
    uint32                              dcc_reserved_8;
    uint32                              dcc_reserved_9;
    uint32                              sz_comp_spec_gen_params;
    uint32                              sz_uc_spec_gen_params;
    uint32                              sz_x_dcc_descriptor;
    uint32                              total_file_sz;
} dcc_component_header_type;


#endif
