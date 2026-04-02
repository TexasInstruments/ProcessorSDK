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

#include <tiadalg_interface.h>
#include <./../common/tiadalg_alg_int_interface.h>
#define MATCH_WITH_PYTORCH

int32_t tiadalg_voxelization_cn(float *lidar_data,
                                int32_t num_lidar_points,
                                int32_t *scratch_1,
                                int32_t *scratch_2,
                                voxel_info_t *voxel_info,
                                void *voxel_data,
                                int32_t *indices,
                                int16_t *num_points,
                                int32_t scale_fact,
                                int32_t out_voxel_data_type)
{
  int32_t i, j, k;
  float x, y, z;
  int32_t x_id, y_id;
  int32_t num_non_empty_voxels = 0;
  int32_t channel_pitch;
  int32_t line_pitch;

  int32_t tot_num_pts=0;

  /*prune out 3d points and get the index on larger WxH resolution*/
  /*scratch_1 indicates voxelid for each input lidar point.*/
  for (i = 0, j = 0; i < num_lidar_points; i++)
  {
    x = lidar_data[i * 4 + 0];
    y = lidar_data[i * 4 + 1];
    z = lidar_data[i * 4 + 2];

    if ((x > voxel_info->min_x) && (x < voxel_info->max_x) && (y > voxel_info->min_y) && (y < voxel_info->max_y) &&
        (z > voxel_info->min_z) && (z < voxel_info->max_z)
       )
    {
#ifdef MATCH_WITH_PYTORCH
      x_id = (int32_t)(((x - voxel_info->min_x) / voxel_info->voxel_size_x));
      y_id = (int32_t)(((y - voxel_info->min_y) / voxel_info->voxel_size_y));
#else
      x_id = (int32_t)(((x - voxel_info->min_x) * voxel_info->one_by_voxel_size_x));
      y_id = (int32_t)(((y - voxel_info->min_y) * voxel_info->one_by_voxel_size_y));
#endif
      scratch_1[i] = y_id * voxel_info->num_voxel_x + x_id;
      tot_num_pts++;
    }
    else
    {
      scratch_1[i] = -1 - i; // filing unique non valid index
    }
  }

  for (i = 0; i < voxel_info->nw_max_num_voxels; i++)
  {
    num_points[i] = 0;
  }

  /* Find unique indices */
  /*There will be voxel which doesnt have any 3d point, hence collecting the voxel ids for valid voxels*/
  /*scratch_2 is the index in valid voxels*/
  num_non_empty_voxels = 0;
  for (i = 0; i < num_lidar_points; i++)
  {
    if (scratch_1[i] >= 0)
    {
      for (k = (i - 1); k >= 0; k--)
      {
        if (scratch_1[i] == scratch_1[k])
        {
          break;
        }
      }

      if (k == -1)
      {
        scratch_2[i] = num_non_empty_voxels; // this voxel idx has come first time, hence allocate a new index for this
        indices[num_non_empty_voxels] = scratch_1[i];
        num_non_empty_voxels++;
      }
      else
      {
        scratch_2[i] = scratch_2[k]; // already this voxel is having one id hence reuse it
      }
    }
  }
  /*Even though current_voxels is less than voxel_info->nw_max_num_voxels, then also arrange
    the data as per maximum number of voxels.
  */
  line_pitch = voxel_info->nw_max_num_voxels;// 4*voxel_info->nw_max_num_voxels
  channel_pitch = voxel_info->max_points_per_voxel * line_pitch; // N * P
  j = 0;
  tot_num_pts = 0;
  

  for (i = 0; i < num_lidar_points; i++)
  {

    if (scratch_1[i] >= 0)/*valid entries are greater than zero*/
    {
      j = scratch_2[i]; /*voxel index*/
      // num_points[j] says already discoverd points for given voxel 'j'

      if(num_points[j]<voxel_info->max_points_per_voxel)
      {
        ((float *)voxel_data)[line_pitch * num_points[j] + j + channel_pitch * 0] = lidar_data[i * 4 + 0] * scale_fact; //x
        ((float *)voxel_data)[line_pitch * num_points[j] + j + channel_pitch * 1] = lidar_data[i * 4 + 1] * scale_fact; //y
        ((float *)voxel_data)[line_pitch * num_points[j] + j + channel_pitch * 2] = lidar_data[i * 4 + 2] * scale_fact; //z
        ((float *)voxel_data)[line_pitch * num_points[j] + j + channel_pitch * 3] = lidar_data[i * 4 + 3] * scale_fact; //r
        num_points[j]++;
      }
      else
      {
        tot_num_pts++;
      }
    }
  }


  for (i = 0; i < voxel_info->nw_max_num_voxels; i++)
  {
    tot_num_pts += num_points[i];
  }

  return num_non_empty_voxels;
}

int32_t tiadalg_voxel_feature_compute_cn(void *voxel_data,
                                         int32_t *indices,
                                         int16_t *num_points,
                                         voxel_info_t *voxel_info,
                                         int32_t num_voxels,
                                         int32_t scale_fact,
                                         int32_t data_type)
{
  int32_t i, j;
  float x, y, z;
  float x_avg, y_avg, z_avg;
  float voxel_center_x, voxel_center_y, voxel_center_z;

  /*Even though current_voxels is less than voxel_info->nw_max_num_voxels, voxel data
    is arranged as per maximum number of voxels.
  */
  int32_t line_pitch = voxel_info->nw_max_num_voxels;
  int32_t channel_pitch = voxel_info->max_points_per_voxel * line_pitch;
  float x_offset = voxel_info->voxel_size_x / 2 + voxel_info->min_x;
  float y_offset = voxel_info->voxel_size_y / 2 + voxel_info->min_y;
  float z_offset = voxel_info->voxel_size_z / 2 + voxel_info->min_z;

  for (i = 0; i < num_voxels; i++) /*num_voxels = P*/
  {
    x = 0;
    y = 0;
    z = 0;
    for (j = 0; j < num_points[i]; j++)
    {
      x += ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 0];
      y += ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 1];
      z += ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 2];
    }

    x_avg = x / num_points[i];
    y_avg = y / num_points[i];
    z_avg = z / num_points[i];

    voxel_center_y = indices[i] / voxel_info->num_voxel_x;
    voxel_center_x = indices[i] - voxel_center_y * voxel_info->num_voxel_x;

    voxel_center_x *= voxel_info->voxel_size_x;
    voxel_center_x += x_offset;

    voxel_center_y *= voxel_info->voxel_size_y;
    voxel_center_y += y_offset;

    voxel_center_z = 0;
    voxel_center_z *= voxel_info->voxel_size_z;
    voxel_center_z += z_offset;

    for (j = 0; j < num_points[i]; j++)
    {
      ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 4] = (((float *)voxel_data)[line_pitch * j + i + channel_pitch * 0] - x_avg) ;
      ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 5] = (((float *)voxel_data)[line_pitch * j + i + channel_pitch * 1] - y_avg) ;
      ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 6] = (((float *)voxel_data)[line_pitch * j + i + channel_pitch * 2] - z_avg) ;
      ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 7] = (((float *)voxel_data)[line_pitch * j + i + channel_pitch * 0] - voxel_center_x * scale_fact) ;
      ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 8] = (((float *)voxel_data)[line_pitch * j + i + channel_pitch * 1] - voxel_center_y * scale_fact) ;
      ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 9] = (((float *)voxel_data)[line_pitch * j + i + channel_pitch * 2] - voxel_center_z * scale_fact) ;
    }

    /*looks like bug in python mmdetection3d code, hence below code is to mimic the mmdetect behaviour*/
    for (j = 0; j < num_points[i]; j++)
    {
      ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 0] = ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 7];
      ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 1] = ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 8];
      ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 2] = ((float *)voxel_data)[line_pitch * j + i + channel_pitch * 9];
    }
  }

  return 1;
}
