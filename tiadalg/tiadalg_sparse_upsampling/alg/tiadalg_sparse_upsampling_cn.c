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
  int32_t out_desc_data_type)
  {
    int32_t kernel_size_2 = 3; // 7x7
    int32_t kernel_size = 7; // 7x7
    int8_t *  cur_filter_coeff;
    int8_t * cur_desc_plane;
    int32_t sum = 0;
    int32_t l,m;
    int32_t data_shift_val = 0;

    if((out_desc_data_type != TIADALG_DATA_TYPE_U08) && (out_desc_data_type != TIADALG_DATA_TYPE_U16))
    {
      return TIADALG_IN_PRM_ERR;
    }

    if((in_desc_data_type != TIADALG_DATA_TYPE_S08) && (in_desc_data_type != TIADALG_DATA_TYPE_U08))
    {
      return TIADALG_IN_PRM_ERR;
    }

    /*output is always in un signed format*/
    if(out_desc_data_type == TIADALG_DATA_TYPE_S08)
    {
      data_shift_val = 128;
    }

    if(out_desc_data_type == TIADALG_DATA_TYPE_S16)
    {
      data_shift_val = 32768;
    }

    int32_t i;

    for(i = 0; i < num_key_points; i++){

      int32_t x = key_point_list[2*i    ];
      int32_t y = key_point_list[2*i + 1];

      int32_t x_m;
      int32_t y_l;

      int32_t x_low;
      int32_t y_low;
      int32_t j;

      for(j = 0; j < num_desc_planes; j++){

        cur_filter_coeff = filter_coeff + j*kernel_size*kernel_size;
        cur_filter_coeff+= kernel_size*kernel_size_2 + kernel_size_2;
        cur_desc_plane = desc_plane + j*desc_plane_pitch;
        sum = 0;

        for(l = -kernel_size_2; l <= kernel_size_2; l++){
          for(m = -kernel_size_2; m <= kernel_size_2; m++){
            y_l = y + l;
            x_m = x + m;

            x_low = (x_m + 0) >> 2;
            y_low = (y_l + 0) >> 2;

            if(in_desc_data_type == TIADALG_DATA_TYPE_S08)
            {
              sum += cur_filter_coeff[l*kernel_size + m] * cur_desc_plane[y_low*desc_line_pitch + x_low];
            }
            else
            {
              sum += cur_filter_coeff[l*kernel_size + m] * (uint8_t)cur_desc_plane[y_low*desc_line_pitch + x_low];
            }

          }
        }
        sum = (sum << (bias_scale - desc_scale - filter_scale)) + bias[j];
        sum = sum + (0x1 <<  (-out_scale + bias_scale - 1));
        /*TIDL generates the output in outscale (0.5) format. But to fit the data in 8 bit, no need to scale the output as
          in that case scale remain explicit
        */
        ((uint8_t*)out_desc)[i*num_desc_planes + j] = (uint8_t)((sum >> (-out_scale + bias_scale)) + data_shift_val) ;
      }
  }
  return(TIADALG_PROCESS_SUCCESS);
}
