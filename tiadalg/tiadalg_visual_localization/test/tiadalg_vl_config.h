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

#ifndef TIADALG_EL_CONFIG_H_
#define TIADALG_EL_CONFIG_H_

#include <../common/tiadalg_test_interface.h>
#include <tiadalg_interface.h>

typedef struct
{
  char  img_file_name[TIADALG_FILE_NAME_SIZE];
  int32_t  width;
  int32_t  height;
  int32_t  num_frames;
  int32_t  start_frame;
  int32_t  skip_frame;

  char  map_voxel_file_name[TIADALG_FILE_NAME_SIZE];
  char  map_feat_file_name[TIADALG_FILE_NAME_SIZE];
  char  map_desc_file_name[TIADALG_FILE_NAME_SIZE];
  tiadalg_map_info map_info;

  int32_t  is_feat_comp_ext;
  char  ext_feat_file_name[TIADALG_FILE_NAME_SIZE];
  char  ext_desc_file_name[TIADALG_FILE_NAME_SIZE];
  int32_t  max_frame_feat;
  int32_t  num_frame_feat;
  int32_t  max_map_feat;
  int32_t  nms_th;
  int32_t  is_ip_fe;

  tiadalg_p3p_params p3p_params;

  float est_loc[3];
  int32_t search_range[3];

  TIADALG_el_out_args out_args;
  char output_file_path[TIADALG_FILE_NAME_SIZE];
  int32_t en_pose_filtering;
  char lens_dist_table_file[TIADALG_FILE_NAME_SIZE];
  char upsampling_weight_file[TIADALG_FILE_NAME_SIZE];
  char upsampling_bias_file[TIADALG_FILE_NAME_SIZE];

} sEL_Config;

extern uint32_t el_numTestCases;
extern sEL_Config gConfig_EL;
extern sTokenMapping gsTokenMap_EL[TIADALG_MAX_ITEMS_TO_PARSE];

#endif /* TIADALG_EL_CONFIG_H_ */
