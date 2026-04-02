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
#include <./../common/tiadalg_alg_int_interface.h>
#include <./../common/math/tansp_i.h>
#include <./../common/math/atansp_i.h>
#include <tiadalg_interface.h>

#define TIADALG_FISHEYE_INT_TABLE_SIZE (1024)

int32_t tiadalg_fisheye_transformation_c66(int16_t * in_img_points, int32_t num_points,
  uint32_t in_center_x, uint32_t in_center_y, float focal_length,
  uint32_t out_center_cx, uint32_t out_center_cy,
  float(*angle_table)[2], int32_t num_table_row,
  uint16_t * scratch, int32_t is_scratch_filled,
  int16_t* out_img_points)
{
  int32_t ret_val = TIADALG_PROCESS_SUCCESS;
  float * restrict angle_tableL = (float*)angle_table;
  int16_t * restrict in_img_pointsL = in_img_points;
  int16_t * restrict out_img_pointsL = out_img_points;
  uint16_t * restrict scratchL = scratch;

  if (in_img_pointsL == NULL) {
    ret_val = ret_val | TIADALG_IN_PRM_ERR;
  }

  if (out_img_pointsL == NULL) {
    ret_val = ret_val | TIADALG_OUT_BUF_ERR;
  }


  if (ret_val != TIADALG_PROCESS_SUCCESS) {
    return ret_val;
  }
  /* Cycle estimates
     2 16 bit read
     2 16 bit difference
     2 16 bit multiplication
     1 sqrtf
     1 atanf
     1 look up table(of 100 elements) search
     2 division

     approximetely 200 cycle per point

  */
  float one_by_f = 1.0f/focal_length;
  float max_dist;
  float step_size;
  float rcp_step_size;

  max_dist = (float)((in_center_x*in_center_x) + (in_center_y*in_center_y));
  max_dist = _rsqrsp(max_dist)*max_dist;
  step_size = max_dist *_rcpsp(TIADALG_FISHEYE_INT_TABLE_SIZE);
  if(step_size < 1.0)
    step_size = 1.0;

  rcp_step_size = _rcpsp(step_size);

  if(is_scratch_filled == 0){

    float min_angle = 180;
    float max_angle = 0;

    float min_angle_trans = 180;
    float max_angle_trans = 0;
    int32_t prev_indx;
    int32_t i,j;
    float r,r_trans;
    float angle_trans;

    for(r = 0, i = 0; r < max_dist; r+= step_size,i++){

      float angle_r = atansp_i(r * one_by_f);

      if (angle_r < 0.0f) {
        angle_r = 0.0f - angle_r;
      }

      float angle_d = angle_r*(180.0f / TIADALG_PI);

      for (j = 0; j < num_table_row; j++) {
        if (angle_tableL[2*j+0] > angle_d)
          break;
      }

      if (j > 0) {
        prev_indx = j - 1;
      }
      else {
        prev_indx = j;
      }

      min_angle = angle_tableL[2*prev_indx+0];
      max_angle = angle_tableL[2*j+0];

      min_angle_trans = angle_tableL[2*prev_indx+1];
      max_angle_trans = angle_tableL[2*j+1];

      if (prev_indx != j) {
        angle_trans = ((max_angle_trans - min_angle_trans) / (max_angle - min_angle))*(angle_d - min_angle) + min_angle_trans;
      }
      else {
        angle_trans = min_angle_trans;
      }

      angle_trans = angle_trans * (TIADALG_PI / 180.0f);
      /* If final angle is more than 99% of pi/2 then clip this to 99% of pi/2
      */
      if(angle_trans >= TIADALG_PI*0.99*0.5)
      {
        angle_trans = TIADALG_PI*0.99*0.5;
      }

      r_trans = focal_length * tan(angle_trans);

      scratchL[2*i + 0] = (uint16_t) r;
      scratchL[2*i + 1] = (uint16_t) r_trans;
    }
  }

  int32_t i;

  for (i = 0; i < num_points; i++) {

    int16_t cur_x = in_img_pointsL[2 * i + 0];
    int16_t cur_y = in_img_pointsL[2 * i + 1];

    int16_t trans_x;
    int16_t trans_y;

    int32_t diff_x = cur_x - in_center_x;
    int32_t diff_y = cur_y - in_center_y;

    float r_sq = (float)((diff_x*diff_x) + (diff_y*diff_y));
    float r = r_sq * _rsqrsp(r_sq);
    float r_trans;

    float min_r;
    float max_r;
    float min_r_trans;
    float max_r_trans;

    int32_t prev_indx;
    int32_t j;

    j = (int32_t)(r*rcp_step_size);
    j = j + 1;

    if(j >= TIADALG_FISHEYE_INT_TABLE_SIZE){
      j = TIADALG_FISHEYE_INT_TABLE_SIZE - 1;
    }

    if (j > 0)
    {
      prev_indx = j - 1;
    }
    else
    {
      prev_indx = j;
    }

    min_r = (float)(scratchL[2*prev_indx + 0]);
    max_r = (float)(scratchL[2*j + 0]);

    min_r_trans = (float)(scratchL[2*prev_indx + 1]);
    max_r_trans = (float)(scratchL[2*j + 1]);

    if (prev_indx != j)
    {
      r_trans = ((max_r_trans - min_r_trans) *_rcpsp(max_r - min_r))*(r - min_r) + min_r_trans;
    }
    else
    {
      r_trans = min_r_trans;
    }

    float r_ratio = r_trans *_rcpsp(r);

    trans_x = (int16_t)_spint(((diff_x*r_ratio) + out_center_cx));
    trans_y = (int16_t)_spint(((diff_y*r_ratio) + out_center_cy));

    out_img_pointsL[2 * i + 0] = trans_x;
    out_img_pointsL[2 * i + 1] = trans_y;

  }
  return(ret_val);
}
