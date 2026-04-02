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

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include "tiadalg_vl_config.h"
#pragma RESET_MISRA ("required")

uint32_t fcw_numTestCases;
sEL_Config gConfig_EL;

sTokenMapping gsTokenMap_EL[] =
{
  {(char *)"img_file_name"       ,&gConfig_EL.img_file_name         , STRING, SINGLE},
  {(char *)"width"               ,&gConfig_EL.width                 , INT_32, SINGLE},
  {(char *)"height"              ,&gConfig_EL.height                , INT_32, SINGLE},
  {(char *)"num_frames"          ,&gConfig_EL.num_frames            , INT_32, SINGLE},
  {(char *)"start_frame"         ,&gConfig_EL.start_frame           , INT_32, SINGLE},
  {(char *)"skip_frame"          ,&gConfig_EL.skip_frame           , INT_32, SINGLE},

  {(char *)"map_voxel_file_name" ,&gConfig_EL.map_voxel_file_name   , STRING, SINGLE},
  {(char *)"map_feat_file_name"  ,&gConfig_EL.map_feat_file_name    , STRING, SINGLE},
  {(char *)"map_desc_file_name"  ,&gConfig_EL.map_desc_file_name    , STRING, SINGLE},
  {(char *)"num_voxels"          ,&gConfig_EL.map_info.num_voxels   , INT_32, SINGLE},
  {(char *)"num_map_feat"        ,&gConfig_EL.map_info.num_map_feat , INT_32, SINGLE},
  {(char *)"map_range"           ,&gConfig_EL.map_info.map_range[0][0]   , INT_32, MULTI},

  {(char *)"desc_size"           ,&gConfig_EL.map_info.desc_size    , INT_32, SINGLE},
  {(char *)"desc_type"           ,&gConfig_EL.map_info.desc_type    , INT_32, SINGLE},
  {(char *)"voxel_size"          ,&gConfig_EL.map_info.voxel_size   , INT_32, MULTI},

  {(char *)"is_feat_comp_ext"    ,&gConfig_EL.is_feat_comp_ext      , INT_32, SINGLE},
  {(char *)"ext_feat_file_name"  ,&gConfig_EL.ext_feat_file_name    , STRING, SINGLE},
  {(char *)"ext_desc_file_name"  ,&gConfig_EL.ext_desc_file_name    , STRING, SINGLE},
  {(char *)"max_frame_feat"      ,&gConfig_EL.max_frame_feat        , INT_32, SINGLE},
  {(char *)"max_map_feat"    ,&gConfig_EL.max_map_feat      , INT_32, SINGLE},

  {(char *)"est_loc"             ,&gConfig_EL.est_loc[0]                 , FLOAT, MULTI},
  {(char *)"search_range"        ,&gConfig_EL.search_range[0]            , INT_32, MULTI},
  {(char *)"nms_th"              ,&gConfig_EL.nms_th                     , INT_32, SINGLE},
  {(char *)"is_ip_fe"            ,&gConfig_EL.is_ip_fe                   , INT_32, SINGLE},

  {(char *)"fx"                  ,&gConfig_EL.p3p_params.fx              , FLOAT, SINGLE},
  {(char *)"fy"                  ,&gConfig_EL.p3p_params.fy              , FLOAT, SINGLE},
  {(char *)"cx"                  ,&gConfig_EL.p3p_params.cx              , FLOAT, SINGLE},
  {(char *)"cy"                  ,&gConfig_EL.p3p_params.cy              , FLOAT, SINGLE},
  {(char *)"iterationsCount"     ,&gConfig_EL.p3p_params.iterationsCount , INT_32, SINGLE},
  {(char *)"reprojErrorThreshold",&gConfig_EL.p3p_params.reprojErrorThreshold , FLOAT, SINGLE},
  {(char *)"inliersRatio"        ,&gConfig_EL.p3p_params.inliersRatio , FLOAT, SINGLE},
  {(char *)"seed"                ,&gConfig_EL.p3p_params.seed            , INT_32, SINGLE},

  {(char *)"output_file_path"       ,&gConfig_EL.output_file_path   , STRING, SINGLE},
  {(char *)"en_pose_filtering"      ,&gConfig_EL.en_pose_filtering  , INT_32, SINGLE},

  {(char *)"lens_dist_table_file"   ,&gConfig_EL.lens_dist_table_file   , STRING, SINGLE},
  {(char *)"upsampling_weight_file" ,&gConfig_EL.upsampling_weight_file , STRING, SINGLE},
  {(char *)"upsampling_bias_file"   ,&gConfig_EL.upsampling_bias_file   , STRING, SINGLE},

} ;
