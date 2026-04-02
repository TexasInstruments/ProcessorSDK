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
#include <tiadalg_interface.h>
#include <string.h>

//#define BEHAVIOURAL_C

static int16_t sub_matrix_sum(int8_t* in_mat, int32_t in_mat_pitch, int32_t start_row, int32_t end_row, int32_t start_col, int32_t end_col);

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
  int32_t out_desc_data_type)
{
  int8_t * restrict cur_filter_coeff;
  int8_t * restrict cur_desc_plane;
  int32_t sum = 0;
  int32_t num_flt_type_per_scale = 16;
  int32_t num_coef_flt_type = 12; // 9; Ideally it has to be 9 only, but to make it aligned, 3x3 filter is changed to 3x4
  int32_t orig_flt_coef_pitch = 7;
  int32_t orig_num_flt_coef = orig_flt_coef_pitch*orig_flt_coef_pitch;
  int16_t * restrict cur_scratch_buffer;
  int16_t * restrict cur_scratch_bufferL;
  int32_t data_shift_val = 0;

  if((out_desc_data_type != TIADALG_DATA_TYPE_U08) && (out_desc_data_type != TIADALG_DATA_TYPE_U16))
  {
    return TIADALG_IN_PRM_ERR;
  }

  if((in_desc_data_type != TIADALG_DATA_TYPE_S08) && (in_desc_data_type != TIADALG_DATA_TYPE_U08))
  {
    return TIADALG_IN_PRM_ERR;
  }

  /*output is always in unsigned format, so if output/descriptor is signed then return in unsigned*/
  if(out_desc_data_type == TIADALG_DATA_TYPE_S08)
  {
    data_shift_val = 128;
  }

  if(out_desc_data_type == TIADALG_DATA_TYPE_S16)
  {
    data_shift_val = 32768;
  }
  int32_t i;

  if(is_scratch_filled == 0){
    for(i = 0; i < num_desc_planes; i++){
      cur_filter_coeff =  &filter_coeff[i*orig_num_flt_coef + 0];

      // x&3 == 0 and y&3 == 0, x & y is top left corner of filter box
     // scratch_buffer[64][16][12]
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 0*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,0,3);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,4,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,0,3);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,4,6);
      // x&3 == 1 and y&3 == 0, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 1*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,0,2);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,3,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,0,2);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,3,6);
      // x&3 == 2 and y&3 == 0, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 2*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,0,1);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,2,5);
      cur_scratch_buffer[2] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,6,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,0,1);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,2,5);
      cur_scratch_buffer[6] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,6,6);
      // x&3 == 3 and y&3 == 0, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 3*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,0,0);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,1,4);
      cur_scratch_buffer[2] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,3,5,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,0,0);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,1,4);
      cur_scratch_buffer[6] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,4,6,5,6);
      // x&3 == 0 and y&3 == 1, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 4*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,0,3);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,4,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,0,3);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,4,6);
      // x&3 == 1 and y&3 == 1, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 5*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,0,2);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,3,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,0,2);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,3,6);
      // x&3 == 2 and y&3 == 1, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 6*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,0,1);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,2,5);
      cur_scratch_buffer[2] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,6,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,0,1);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,2,5);
      cur_scratch_buffer[6] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,6,6);
      // x&3 == 3 and y&3 == 1, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 7*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,0,0);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,1,4);
      cur_scratch_buffer[2] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,2,5,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,0,0);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,1,4);
      cur_scratch_buffer[6] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,3,6,5,6);
      // x&3 == 0 and y&3 == 2, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 8*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,0,3);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,4,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,0,3);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,4,6);
      cur_scratch_buffer[8] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,0,3);
      cur_scratch_buffer[9] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,4,6);
      // x&3 == 1 and y&3 == 2, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 9*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,0,2);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,3,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,0,2);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,3,6);
      cur_scratch_buffer[8] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,0,2);
      cur_scratch_buffer[9] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,3,6);
      // x&3 == 2 and y&3 == 2, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 10*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,0,1);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,2,5);
      cur_scratch_buffer[2] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,6,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,0,1);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,2,5);
      cur_scratch_buffer[6] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,6,6);
      cur_scratch_buffer[8] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,0,1);
      cur_scratch_buffer[9] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,2,5);
      cur_scratch_buffer[10] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,6,6);
      // x&3 == 3 and y&3 == 2, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 11*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,0,0);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,1,4);
      cur_scratch_buffer[2] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,1,5,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,0,0);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,1,4);
      cur_scratch_buffer[6] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,2,5,5,6);
      cur_scratch_buffer[8] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,0,0);
      cur_scratch_buffer[9] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,1,4);
      cur_scratch_buffer[10] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,6,6,5,6);
      // x&3 == 0 and y&3 == 3, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 12*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,0,3);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,4,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,0,3);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,4,6);
      cur_scratch_buffer[8] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,0,3);
      cur_scratch_buffer[9] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,4,6);
      // x&3 == 1 and y&3 == 3, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 13*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,0,2);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,3,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,0,2);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,3,6);
      cur_scratch_buffer[8] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,0,2);
      cur_scratch_buffer[9] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,3,6);
      // x&3 == 2 and y&3 == 3, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 14*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,0,1);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,2,5);
      cur_scratch_buffer[2] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,6,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,0,1);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,2,5);
      cur_scratch_buffer[6] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,6,6);
      cur_scratch_buffer[8] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,0,1);
      cur_scratch_buffer[9] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,2,5);
      cur_scratch_buffer[10] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,6,6);
      // x&3 == 3 and y&3 == 3, x & y is top left corner of filter box
      cur_scratch_buffer = &scratch_buffer[i*num_flt_type_per_scale*num_coef_flt_type + 15*num_coef_flt_type];
      memset(cur_scratch_buffer,0x0,num_coef_flt_type*sizeof(int16_t));
      cur_scratch_buffer[0] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,0,0);
      cur_scratch_buffer[1] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,1,4);
      cur_scratch_buffer[2] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,0,0,5,6);
      cur_scratch_buffer[4] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,0,0);
      cur_scratch_buffer[5] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,1,4);
      cur_scratch_buffer[6] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,1,4,5,6);
      cur_scratch_buffer[8] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,0,0);
      cur_scratch_buffer[9] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,1,4);
      cur_scratch_buffer[10] = sub_matrix_sum(cur_filter_coeff,orig_flt_coef_pitch,5,6,5,6);
    }
  }

  int32_t orig_flt_coef_pitch_by2 = orig_flt_coef_pitch >> 1;
  int32_t mod_bias_scale = bias_scale - desc_scale - filter_scale;
  int32_t mod_out_scale = -out_scale + bias_scale;
  int32_t u32temp1;
  uint64_t u64temp1,u64temp2;
  int32_t j;

  for(j = 0; j < num_desc_planes; j++){

    cur_scratch_buffer = scratch_buffer + j*num_flt_type_per_scale*num_coef_flt_type ;
    cur_desc_plane = desc_plane + j*desc_plane_pitch;

#ifdef BEHAVIOURAL_C
    int32_t x_m;
    int32_t y_l;
    int32_t l,m;
    int32_t kernel_size = 3; // 3x4

    for(int32_t j = 0; j < num_desc_planes; j++){
      // scratch_buffer[64][16][12]
      cur_scratch_buffer = scratch_buffer + j*num_flt_type_per_scale*num_coef_flt_type + (y_phase*4 + x_phase)*num_coef_flt_type;
      cur_desc_plane = desc_plane + j*desc_plane_pitch;
      sum = 0;

      for(l = 0; l < kernel_size; l++){
        for(m = 0; m < kernel_size; m++){

          x_m = x_low + m;
          y_l = y_low + l;

          sum += cur_scratch_buffer[l*4 + m] * cur_desc_plane[y_l*desc_line_pitch + x_m];
        }
      }
      sum = (sum << (bias_scale - desc_scale - filter_scale)) + bias[j];
      sum = sum + (0x1 <<  (-out_scale + bias_scale - 1));

      ((uint8_t*)out_desc)[i*num_desc_planes + j] = (uint8_t)((sum >> (-out_scale + bias_scale)) + data_shift_val) ;

    }
#else

    /*There is possibility of interchanging this loop with key point list loop.
      But in that case x_phase, y_phase, x_low, y_low calculation will go inside the loop, however it might be
      favourable for cache read prespective.
    */
    int32_t x, y, x_phase, y_phase, x_low, y_low;

    if ((out_desc_data_type == TIADALG_DATA_TYPE_U08) && (in_desc_data_type == TIADALG_DATA_TYPE_S08)){

      uint8_t * restrict out_desc_l = (uint8_t*)out_desc;
      int32_t i;

      for(i = 0; i < num_key_points; i++){
        x = (key_point_list[2*i    ] - orig_flt_coef_pitch_by2)&0xFFFC; // without phase
        y = (key_point_list[2*i + 1] - orig_flt_coef_pitch_by2)&0xFFFC; // without phase
        x_phase = (key_point_list[2*i    ] - orig_flt_coef_pitch_by2) & 0x3;
        y_phase = (key_point_list[2*i + 1] - orig_flt_coef_pitch_by2) & 0x3;
        x_low = (x) >> 2; // without phase lower resolution
        y_low = (y) >> 2; // without phase lower resolution

        sum = 0;

        cur_scratch_bufferL = cur_scratch_buffer + (y_phase*4 + x_phase)*num_coef_flt_type;

        /*If the sparse points are sorted for x_phase and y_phase then coefficients loading can go outside the loop*/
        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[y_low*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(u32temp1,0x01010101u);
        sum      = _dotp4h(u64temp2,u64temp1);

        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[(y_low+1)*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(u32temp1,0x01010101u);
        sum     += _dotp4h(u64temp2,u64temp1);

        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[(y_low+2)*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(u32temp1,0x01010101u);
        sum     += _dotp4h(u64temp2,u64temp1);

        sum = (sum << mod_bias_scale) + bias[j];
        sum = sum + (0x1 <<  (mod_out_scale - 1));
        /*TIDL generates the output in outscale (0.5) format. But to fit the data in 8 bit, no need to scale the output as
          in that case scale remain explicit
        */
        out_desc_l[i*num_desc_planes + j] = (uint8_t)((sum >> mod_out_scale) + data_shift_val);
      }
    }
    else if((out_desc_data_type == TIADALG_DATA_TYPE_U08) && (in_desc_data_type == TIADALG_DATA_TYPE_U08))
    {
      uint8_t * restrict out_desc_l = (uint8_t*)out_desc;
      int32_t i;

      for(i = 0; i < num_key_points; i++){
        x = (key_point_list[2*i    ] - orig_flt_coef_pitch_by2)&0xFFFC; // without phase
        y = (key_point_list[2*i + 1] - orig_flt_coef_pitch_by2)&0xFFFC; // without phase
        x_phase = (key_point_list[2*i    ] - orig_flt_coef_pitch_by2) & 0x3;
        y_phase = (key_point_list[2*i + 1] - orig_flt_coef_pitch_by2) & 0x3;
        x_low = (x) >> 2; // without phase lower resolution
        y_low = (y) >> 2; // without phase lower resolution

        sum = 0;

        cur_scratch_bufferL = cur_scratch_buffer + (y_phase*4 + x_phase)*num_coef_flt_type;

        /*If the sparse points are sorted for x_phase and y_phase then coefficients loading can go outside the loop*/
        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[y_low*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(0x01010101u,u32temp1);
        sum      = _dotp4h(u64temp2,u64temp1);

        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[(y_low+1)*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(0x01010101u,u32temp1);
        sum     += _dotp4h(u64temp2,u64temp1);

        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[(y_low+2)*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(0x01010101u,u32temp1);
        sum     += _dotp4h(u64temp2,u64temp1);

        sum = (sum << mod_bias_scale) + bias[j];
        sum = sum + (0x1 <<  (mod_out_scale - 1));
        /*TIDL generates the output in outscale (0.5) format. But to fit the data in 8 bit, no need to scale the output as
          in that case scale remain explicit
        */
        out_desc_l[i*num_desc_planes + j] = (uint8_t)((sum >> mod_out_scale) + data_shift_val);
      }

    }
    else if((out_desc_data_type == TIADALG_DATA_TYPE_U16) && (in_desc_data_type == TIADALG_DATA_TYPE_S08))
    {
      uint16_t * restrict out_desc_l = (uint16_t*)out_desc;
      int32_t i;
      for(i = 0; i < num_key_points; i++){
        x = (key_point_list[2*i    ] - orig_flt_coef_pitch_by2)&0xFFFC; // without phase
        y = (key_point_list[2*i + 1] - orig_flt_coef_pitch_by2)&0xFFFC; // without phase
        x_phase = (key_point_list[2*i    ] - orig_flt_coef_pitch_by2) & 0x3;
        y_phase = (key_point_list[2*i + 1] - orig_flt_coef_pitch_by2) & 0x3;
        x_low = (x) >> 2; // without phase lower resolution
        y_low = (y) >> 2; // without phase lower resolution

        sum = 0;

        cur_scratch_bufferL = cur_scratch_buffer + (y_phase*4 + x_phase)*num_coef_flt_type;

        /*If the sparse points are sorted for x_phase and y_phase then coefficients loading can go outside the loop*/
        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[y_low*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(u32temp1,0x01010101u);
        sum      = _dotp4h(u64temp2,u64temp1);

        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[(y_low+1)*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(u32temp1,0x01010101u);
        sum     += _dotp4h(u64temp2,u64temp1);

        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[(y_low+2)*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(u32temp1,0x01010101u);
        sum     += _dotp4h(u64temp2,u64temp1);

        sum = (sum << mod_bias_scale) + bias[j];
        sum = sum + (0x1 <<  (mod_out_scale - 1));
        out_desc_l[i*num_desc_planes + j] = (uint16_t)(((sum >> mod_out_scale) << (-out_scale)) + data_shift_val);
      }
    }
    else //if((out_desc_data_type == TIADALG_DATA_TYPE_U16) && (in_desc_data_type == TIADALG_DATA_TYPE_U08))
    {
      uint16_t * restrict out_desc_l = (uint16_t*)out_desc;
      int32_t i;
      for(i = 0; i < num_key_points; i++){
        x = (key_point_list[2*i    ] - orig_flt_coef_pitch_by2)&0xFFFC; // without phase
        y = (key_point_list[2*i + 1] - orig_flt_coef_pitch_by2)&0xFFFC; // without phase
        x_phase = (key_point_list[2*i    ] - orig_flt_coef_pitch_by2) & 0x3;
        y_phase = (key_point_list[2*i + 1] - orig_flt_coef_pitch_by2) & 0x3;
        x_low = (x) >> 2; // without phase lower resolution
        y_low = (y) >> 2; // without phase lower resolution

        sum = 0;

        cur_scratch_bufferL = cur_scratch_buffer + (y_phase*4 + x_phase)*num_coef_flt_type;

        /*If the sparse points are sorted for x_phase and y_phase then coefficients loading can go outside the loop*/
        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[y_low*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(0x01010101u,u32temp1);
        sum      = _dotp4h(u64temp2,u64temp1);

        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[(y_low+1)*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(0x01010101u,u32temp1);
        sum     += _dotp4h(u64temp2,u64temp1);

        u64temp1 = _amem8_const(cur_scratch_bufferL);
        cur_scratch_bufferL += 4;
        u32temp1 = _mem4_const(&cur_desc_plane[(y_low+2)*desc_line_pitch + x_low]);
        u64temp2 = _mpysu4ll(0x01010101u,u32temp1);
        sum     += _dotp4h(u64temp2,u64temp1);

        sum = (sum << mod_bias_scale) + bias[j];
        sum = sum + (0x1 <<  (mod_out_scale - 1));
        out_desc_l[i*num_desc_planes + j] = (uint16_t)(((sum >> mod_out_scale) << (-out_scale)) + data_shift_val);
      }
    }
#endif
  }

  return(TIADALG_PROCESS_SUCCESS);
}

static int16_t sub_matrix_sum(int8_t* in_mat, int32_t in_mat_pitch, int32_t start_row, int32_t end_row, int32_t start_col, int32_t end_col)
{
  int32_t i, j;
  int16_t sum = 0;

  for(i = start_row; i <=end_row; i++){
    for(j = start_col; j <=end_col; j++){
      sum += in_mat[i*in_mat_pitch + j];
    }
  }

  return(sum);

}
