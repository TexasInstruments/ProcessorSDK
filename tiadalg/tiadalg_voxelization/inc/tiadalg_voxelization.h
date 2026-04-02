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
@file      tiadalg_voxelization.h
@brief     This file defines the interface for Voxelization and feature computation for points pillar CNN for 3D OD on lidar data
@version 0.1 Sep 2018 : Initial Code
*/

/** @ingroup    TIADALG_VOXELIZATION_H */
/*@{*/
#ifndef TI_VOXELIZATION_H
#define TI_VOXELIZATION_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  @struct voxel_info_t
  @brief  This structure defines the property of lidar data and its voxelization

  @param  min_x: Minimum x value to be considered for lidar data, other wise particular lidara data is discarded
  @param  min_y: Minimum y value to be considered for lidar data, other wise particular lidara data is discarded
  @param  min_z: Minimum z value to be considered for lidar data, other wise particular lidara data is discarded
  @param  max_x: Maximum x value to be considered for lidar data, other wise particular lidara data is discarded
  @param  max_y: Maximum x value to be considered for lidar data, other wise particular lidara data is discarded
  @param  max_z: Maximum x value to be considered for lidar data, other wise particular lidara data is discarded
  @param  voxel_size_x: Voxel size in x direcrtion
  @param  voxel_size_y: Voxel size in y direcrtion
  @param  voxel_size_z: Voxel size in z direcrtion
  @param  one_by_voxel_size_x: Pre computed one by voxel_size_x
  @param  one_by_voxel_size_y: Pre computed one by voxel_size_y
  @param  num_voxel_x: Number of voxels in x direction
  @param  num_voxel_y: Number of voxels in y direction
  @param  max_points_per_voxel: Maximum number of points allowed in one voxel
  @param  nw_max_num_voxels: Maximum number of voxels to be generated

*/

typedef struct{
  float min_x;
  float min_y;
  float min_z;
  float max_x;
  float max_y;
  float max_z;
  float voxel_size_x;
  float voxel_size_y;
  float voxel_size_z;
  float one_by_voxel_size_x;
  float one_by_voxel_size_y;
  int32_t num_voxel_x;
  int32_t num_voxel_y;
  int32_t max_points_per_voxel;
  int32_t nw_max_num_voxels;
}voxel_info_t;


/**
 * @func tiadalg_voxelization
 *
 * @brief It does basic voxelization
 *
 *
 * @param lidar_data             [in]  : Input lidar data. 4xL
 * @param num_lidar_points       [in]  : Number of lidar points
 * @param scratch_1              [in]  : scratch of size num_lidar_points*sizeof(uint32_t)
 * @param scratch_2              [in]  : scratch of size num_lidar_points*sizeof(uint32_t)
 * @param voxel_info             [in]  : voxel info
 * @param voxel_data             [out] : output buffer of size nw_max_num_voxels*max_points_per_voxel*9*sizeof(out_voxel_data_type)
 * @param indices                [out] : output buffer of size nw_max_num_voxels*sizeof(uint32). Indicating voxel location in birds eye view
 * @param num_points             [out] : output buffer of size nw_max_num_voxels*sizeof(uint16). Indicating acctual number of points in each voxel
 * @param scale_fact             [in]  : Input lidar data is multiplied with value before usages
 * @param out_voxel_data_type    [out] : Output voxel data type. Not used right now. It is always float32 right now.
 *
 * @note
 *  - it return total number of non empty voxels
 */

int32_t tiadalg_voxelization_cn(float *lidar_data,
                                int32_t num_lidar_points,
                                int32_t *scratch_1,
                                int32_t *scratch_2,
                                voxel_info_t *voxel_info,
                                void *voxel_data,
                                int32_t *indices,
                                int16_t *num_points,
                                int32_t scale_fact,
                                int32_t out_voxel_data_type);

/**
 * @func tiadalg_voxelization
 *
 * @brief It does basic voxelization
 *
 *
 * @param lidar_data             [in]  : Input lidar data. 4xL
 * @param num_lidar_points       [in]  : Number of lidar points
 * @param scratch_1              [in]  : scratch of size num_lidar_points*sizeof(uint32_t)
 * @param scratch_2              [in]  : scratch of size num_lidar_points*sizeof(uint32_t)
 * @param voxel_info             [in]  : voxel info
 * @param voxel_data             [out] : output buffer of size nw_max_num_voxels*max_points_per_voxel*9*sizeof(out_voxel_data_type)
 * @param indices                [out] : output buffer of size nw_max_num_voxels*sizeof(uint32). Indicating voxel location in birds eye view
 * @param num_points             [out] : output buffer of size nw_max_num_voxels*sizeof(uint16). Indicating acctual number of points in each voxel
 * @param scale_fact             [in]  : Input lidar data is multiplied with value before usages
 * @param out_voxel_data_type    [out] : Output voxel data type. Not used right now. It is always float32 right now.
 *
 * @note
 *  - it return total number of non empty voxels
 */

int32_t tiadalg_voxelization_c7x(float *lidar_data,
                                int32_t num_lidar_points,
                                int32_t *scratch_1,
                                int32_t *scratch_2,
                                voxel_info_t *voxel_info,
                                void *voxel_data,
                                int32_t *indices,
                                int16_t *num_points,
                                int32_t scale_fact,
                                int32_t out_voxel_data_type);

/**
 * @func tiadalg_voxel_feature_compute_cn
 *
 * @brief Computes feature for voxels
 *
 * @param voxel_data        [out]  : output buffer of size nw_max_num_voxels*max_points_per_voxel*9*sizeof(out_voxel_data_type)
 * @param indices           [in]   : output buffer of size nw_max_num_voxels*sizeof(uint32). Indicating voxel location in birds eye view
 * @param num_points        [in]   : output buffer of size nw_max_num_voxels*sizeof(uint16). Indicating acctual number of points in each voxel
 * @param voxel_info        [in]   : voxel info
 * @param num_voxels        [in]   : Total number of voxels given to API
 * @param scale_fact        [in]   : Input lidar data is multiplied with value before usages
 * @param data_type         [in]   : Not used right now. It is flaot32 always right now
 *
 * @note
 *
 */

int32_t tiadalg_voxel_feature_compute_cn(void *voxel_data,
                                         int32_t *indices,
                                         int16_t *num_points,
                                         voxel_info_t *voxel_info,
                                         int32_t num_voxels,
                                         int32_t scale_fact,
                                         int32_t data_type);

/**
 * @func tiadalg_voxel_feature_compute_cn
 *
 * @brief Computes feature for voxels
 *
 * @param voxel_data        [out]  : output buffer of size nw_max_num_voxels*max_points_per_voxel*9*sizeof(out_voxel_data_type)
 * @param indices           [in]   : output buffer of size nw_max_num_voxels*sizeof(uint32). Indicating voxel location in birds eye view
 * @param num_points        [in]   : output buffer of size nw_max_num_voxels*sizeof(uint16). Indicating acctual number of points in each voxel
 * @param voxel_info        [in]   : voxel info
 * @param num_voxels        [in]   : Total number of voxels given to API
 * @param scale_fact        [in]   : Input lidar data is multiplied with value before usages
 * @param data_type         [in]   : Not used right now. It is flaot32 always right now
 *
 * @note
 *
 */

int32_t tiadalg_voxel_feature_compute_c7x(void *voxel_data,
                                         void *voxel_feat,
                                         int32_t *indices,
                                         int16_t *num_points,
                                         voxel_info_t *voxel_info,
                                         int32_t num_voxels,
                                         int32_t scale_fact,
                                         int32_t data_type);

#ifdef __cplusplus
}
#endif

#endif
/*@}*/
/* TIADALG_VOXELIZATION_H */
