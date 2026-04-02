/*
*
* Copyright (c) {2015 - 2019} Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
/**
@file      tiadalg_visual_localization.h
@brief     This file defines the interface for TIADALG Visual localization algorithm. This algorithm
           takes externally computed image features along with pre generated map information
           to estimate the current location. Externally computed features could be AKAZE features or
           it could be any other as well.

@version 0.1 Feb 2019 : First version
*/

/** @ingroup    TIADALG_VISUAL_LOCALIZATION_H */
/*@{*/
#ifndef _TIADALG_VISUAL_LOCALIZATION_H_
#define _TIADALG_VISUAL_LOCALIZATION_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <ivision.h>

/**
  @brief     Image feature Descriptor data type. Prebuilt binaries are for unsigned 8b, 64 element feature descriptor
 */
typedef uint8_t VL_DESC_DATA_TYPE;


/**
  @enum      tiadalg_el_inbuf_order
  @brief     Input buffer ids for the buffers to be given in visual localization process call
 */

typedef enum
{
  TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT     = 0, /*!< Buffer for external key point (x,y) features
                                                */
  TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC     = 1, /*!< Buffer for  descriptors for each external key point (x,y)
                                                */
  TIADALG_EL_IN_BUFDESC_TOTAL
} tiadalg_el_inbuf_order;


/**
  @struct    tiadalg_voxel_info
  @brief     This defines the property of one voxel
             List of tiadalg_voxel_info objects are passed through buffer TIADALG_EL_IN_BUFDESC_VOXEL_INFO memTab
 */
typedef struct tiadalg_voxel_info
{
  /** X, Y, Z location of given voxel*/
  int32_t loc[3];

  /** Number of feature point in given voxel*/
  int32_t num_points;

  /**Offset for 3d point or descriptor corresponding to this voxel in respective buffers provided through create params*/
  int32_t offset;
}tiadalg_voxel_info;

/**
  @struct   tiadalg_ext_feat
  @brief    Information of one externally computed image feature point
 */
#ifdef TIADALG_EL_INT_FEAT
typedef struct tiadalg_ext_feat
{
  /** In Q format. This value is divided by the TIADALG_el_in_args::feat_q_factor to get real floating point value*/
  int32_t x;

  /** In Q format. This value is divided by the TIADALG_el_in_args::feat_q_factor to get real floating point value*/
  int32_t y;
}tiadalg_ext_feat;
#else
typedef struct tiadalg_ext_feat
{
  /** x co-ordinate in float 32b precision*/
  float x;

  /** y co-ordinate in float 32b precision */
  float y;
}tiadalg_ext_feat;
#endif

/**
  @struct   tiadalg_map_feat
  @brief    Information of one map feature point
 */
typedef struct tiadalg_map_feat
{
  /** X co-rdinate in world in 32b float precision*/
  float x;
  /** Y co-rdinate in world in 32b float precision*/
  float y;
  /** Z co-rdinate in world in 32b float precision*/
  float z;
}tiadalg_map_feat;

/**
  @struct   tiadalg_feat_desc
  @brief    Feature point descriptor format
            Currently only 64 uint16/uint8 values are supported for descriptor.
            Pre-built binaries are for 64 elemnts of unsigned 8b descriptors
 */
typedef struct tiadalg_feat_desc
{
  VL_DESC_DATA_TYPE x[64];
}tiadalg_feat_desc;

/**
  @struct   tiadalg_map_info
  @brief    Information corresponding to overall map, passed to algorithm though create params
*/

typedef struct
{
  /** Total number of voxels in map */
  int32_t num_voxels;
  /** Total number of features in map*/
  int32_t num_map_feat;
  /** Descriptor size in number of elements.*/
  int32_t desc_size;
  /** 0 - binary descriptor, 1 - 8 bit descriptor, 2 - 16 bit descriptor, 3 - 32 bit int desc. Pre built binaries are for 8-bit descriptors*/
  int32_t desc_type;
  /** Voxel size in meters for X/Y/Z direction*/
  int32_t voxel_size[3];
  /** Min Max range for X/Y/Z direction*/
  int32_t map_range[3][2];

}tiadalg_map_info;

/**
  @struct   TIADALG_el_create_params
  @brief    Algorithm create time parameters
*/

typedef struct
{
  IVISION_Params visionParams;
  /** Overall map information */
  tiadalg_map_info map_info  ;
  /** Maximum number of features allowed in any given frame*/
  int32_t    max_frame_feat  ;
  /** Maximum number of features allowed from map sampling for any given frame*/
  int32_t    max_map_feat;
  /** 1 -- features are computed externally and passed through the buffer TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT. 0 -- computed internally*/
  int32_t    is_feat_comp_ext;
  /** Maximum number of best matches allowed between map and current frame features*/
  int32_t    max_feat_match;
  /** map voxel data pointer*/
  uint8_t   *map_voxel_ptr;
  /** map voxel data pointer*/
  uint8_t   *map_pt_ptr;
  /** Map descriptor data pointer*/
  uint8_t   *map_desc_ptr;
} TIADALG_el_create_params;

/**
  @struct   TIADALG_el_in_args
  @brief    InArgs parameters
*/
typedef struct
{
  IVISION_InArgs   iVisionInArgs      ;
  /** solvePnp p3p parameters \ref tiadalg_p3p_params*/
  tiadalg_p3p_params p3p_params       ;
  /** Search range with respect to estimated location*/
  int32_t search_range[3]             ;
  /** feature point to be divided by this value to get into real floating point number*/
  int32_t feat_q_factor;
  /** zero disables the pose filtering, and 1 enables pose filtering*/
  int32_t en_pose_filtering;
  /** Number of features in current frame*/
  int32_t num_cur_feat;
  /** Flag to enable or disable map sampling dependent on FOV */
  int32_t en_subsample_map;
  /** Reset the past statistics */
  int32_t en_reset;
  /** Initial estimate for the location [tx, ty, tz]. This parameter will be read in first frmae and in en_reset = 1 scenario*/
  float  est_loc[3];
} TIADALG_el_in_args;

/**
  @struct TIADALG_el_out_args
  @brief  outArgs parmaeters
 */
typedef struct
{
  IVISION_OutArgs  iVisionOutArgs;
  /** Output Rotation parameters of the pose matrix*/
  float rotation[9];
  /** Output euler angle of the pose matrix*/
  float angle[3];
  /** Output trnaslation parameters of pasematrix*/
  float translation[3];
} TIADALG_el_out_args;

/**
  @brief     List of exposed APIs for the algorithm
 */
extern const IVISION_Fxns VL_VISION_FXNS;

#ifdef __cplusplus
}
#endif

#endif
/*@}*/
/* TIADALG_VISUAL_LOCALIZATION_H */

