/*
*
* Copyright (c) {2015 - 2018} Texas Instruments Incorporated
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
@file      tiadalg_sparse_upsampling.h
@brief     This file defines the interface for sparse up-sampling and convolution operation
@version 0.1 Sep 2019 : Initial Code
*/

/** @ingroup    TIADALG_SPARSE_UPSAMPLING_H */
/*@{*/
#ifndef TIADALG_SPARSE_UPSAMPLING_H_
#define TIADALG_SPARSE_UPSAMPLING_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @func tiadalg_sparse_upsampling_cn
 *
 * @brief Given the keypoint locations list at higher image resolution and descriptors at lower image resolution,
 * higher resolution descriptor is derived by nearest neighbour upsampling scheme and spatial 7x7 filtering centered at keypoint location.
 * Only 4x4 upsampling is supported by this API. And 7x7 intra descriptor plane filtering post upsampling is applied. The process is equivalent of
 * upsampling the descriptor plane by factor 4 in both direction, and selecting the values specific to key point locations.
 *
 * @param desc_plane          [in]  : Input descriptor data buffer at lower resolution e.g. (w/4)x(h/4)
 * @param desc_line_pitch     [in]  : One line pitch in each descriptor plane of resolution (w/4)x(h/4)
 * @param desc_plane_pitch    [in]  : Plane pitch for each descriptor plane
 * @param num_desc_planes     [in]  : Number of descriptor planes
 * @param filter_coeff        [in]  : Filter coefficient (7x7) for each descriptor plane, to be applied after 4x4 upsampling the \ref desc_plane
 * @param bias                [in]  : 16 bit bias to be applied after 7x7 up-sampling filter for each descriptor plane
 * @param key_point_list      [in]  : Key point locations in higher image resolution domain e.g. (wxh)
 * @param num_key_points      [in]  : Number of key points
 * @param scratch_buffer      [in]  : Scartch buffer required. Not used in _cn API
 * @param is_scratch_filled   [in]  : If scratch buffer was filled by previous call of the API, then it should be passed as '1' otherwise '0'
 * @param desc_scale          [in]  : Scale of descriptor in power of 2
 * @param filter_scale        [in]  : Filter scale in power of 2
 * @param bias_scale          [in]  : Bias scale in power of 2
 * @param out_scale           [in]  : Output scale required in power of 2
 * @param out_desc            [out] : Output upsampled descriptor at higher resolution
 * @param in_desc_data_type   [in]  : input descriptor data type.
 * @param out_desc_data_type  [in]  : output descriptor data type.
 *
 * @note
 * - Size of the buffer \ref desc_plane is equal to \ref desc_line_pitch * \ref desc_plane_height * \ref num_desc_planes
 * - Size of the \ref filter_coeff is 7 * 7 * \ref num_desc_planes
 * - x and y location for each key point has to be provided in the buffer \ref key_point_list
 * - Size of the buffer \ref key_point_list is 2 * \ref num_key_points.
 * - Size of the \ref out_desc is \ref num_desc_planes * \ref num_key_points
 * - Input descriptor data \ref desc_plane is expected in signed 8 bit
 * - \ref out_desc can be unsgined 8 bit or unsigned 16 bit depending on \ref out_desc_data_type
 */

int32_t tiadalg_sparse_upsampling_cn(int8_t * desc_plane,
                                     uint32_t desc_line_pitch,
                                     uint32_t desc_plane_pitch,
                                     uint16_t num_desc_planes,
                                     int8_t *  filter_coeff,
                                     int16_t * bias,
                                     int32_t * key_point_list,
                                     uint16_t num_key_points,
                                     int16_t * scratch_buffer,
                                     uint8_t is_scratch_filled,
                                     int32_t desc_scale,
                                     int32_t filter_scale,
                                     int32_t bias_scale,
                                     int32_t out_scale,
                                     void * out_desc,
                                     int32_t in_desc_data_type,
                                     int32_t out_desc_data_type);

/**
 * @func tiadalg_sparse_upsampling_c66
 *
 * @brief Given the keypoint locations list at higher image resolution and descriptors at lower image resolution,
 * higher resolution descriptor is derived by nearest neighbour upsampling scheme and spatial 7x7 filtering centered at keypoint location.
 * Only 4x4 upsampling is supported by this API. And 7x7 intra descriptor plane filtering post upsampling is applied. The process is equivalent of
 * upsampling the descriptor plane by factor 4 in both direction, and selecting the values specific to key point locations.
 *
 * @param desc_plane          [in]  : Input descriptor data buffer at lower resolution e.g. (w/4)x(h/4)
 * @param desc_line_pitch     [in]  : One line pitch in each descriptor plane of resolution (w/4)x(h/4)
 * @param desc_plane_pitch    [in]  : Plane pitch for each descriptor plane
 * @param num_desc_planes     [in]  : Number of descriptor planes
 * @param filter_coeff        [in]  : Filter coefficient (7x7) for each descriptor plane, to be applied after 4x4 upsampling the \ref desc_plane
 * @param bias                [in]  : 16 bit bias to be applied after 7x7 up-sampling filter for each descriptor plane
 * @param key_point_list      [in]  : Key point locations in higher image resolution domain e.g. (wxh)
 * @param num_key_points      [in]  : Number of key points
 * @param scratch_buffer      [in]  : Scartch buffer required. Not used in _cn API
 * @param is_scratch_filled   [in]  : If scratch buffer was filled by previous call of the API, then it should be passed as '1' otherwise '0'
 * @param desc_scale          [in]  : Scale of descriptor in power of 2
 * @param filter_scale        [in]  : Filter scale in power of 2
 * @param bias_scale          [in]  : Bias scale in power of 2
 * @param out_scale           [in]  : Output scale required in power of 2
 * @param out_desc            [out] : Output upsampled descriptor at higher resolution
 * @param in_desc_data_type   [in]  : input descriptor data type.
 * @param out_desc_data_type  [in]  : output descriptor data type.
 *
 * @note
 * - Size of the buffer \ref desc_plane is equal to \ref desc_line_pitch * \ref desc_plane_height * \ref num_desc_planes
 * - Size of the \ref filter_coeff is 7 * 7 * \ref num_desc_planes
 * - x and y location for each key point has to be provided in the buffer \ref key_point_list
 * - Size of the buffer \ref key_point_list is 2 * \ref num_key_points.
 * - Size of the \ref out_desc is \ref num_desc_planes * \ref num_key_points
 * - Input descriptor data \ref desc_plane is expected in signed 8 bit
 * - \ref out_desc can be unsgined 8 bit or unsigned 16 bit depending on \ref out_desc_data_type
 */

int32_t tiadalg_sparse_upsampling_c66(int8_t * desc_plane,
  uint32_t desc_line_pitch,
  uint32_t desc_plane_pitch,
  uint16_t num_desc_planes,
  int8_t *  filter_coeff,
  int16_t * bias,
  int32_t * key_point_list,
  uint16_t num_key_points,
  int16_t * scratch_buffer,
  uint8_t is_scratch_filled,
  int32_t desc_scale,
  int32_t filter_scale,
  int32_t bias_scale,
  int32_t out_scale,
  void * out_desc,
  int32_t in_desc_data_type,
  int32_t out_desc_data_type
  );

#ifdef __cplusplus
}
#endif

#endif
/*@}*/
/* TIADALG_SPARSE_UPSAMPLING_H_ */
