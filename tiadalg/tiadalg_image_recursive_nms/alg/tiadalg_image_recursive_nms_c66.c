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
//#define ENABLE_NMS_PROFILE_DEBUG_PRINT_LOG
#define MATCH_REF_CODE

int32_t tiadalg_image_recursive_nms_c66(int8_t* score_buf, int32_t img_width, int32_t img_pitch, int32_t img_height,
                                            int32_t cur_score_th, int32_t max_frame_feat,
                                            int32_t guard_pixels, int8_t* scratch, int32_t data_type,
                                            int32_t* out_buf)
{
  int32_t i,j;
  int32_t num_key_points = 0;
  int16_t* validLocIdx = (int16_t*)scratch;
  uint64_t pkd_data_offset;
  int32_t data_offset;

  if((data_type != TIADALG_DATA_TYPE_S08) && (data_type != TIADALG_DATA_TYPE_U08))
  {
    return num_key_points;
  }

  if(data_type == TIADALG_DATA_TYPE_S08)
  {
    // Add 128 and bring the data to u08
    pkd_data_offset= (0x80 << 24) | (0x80 << 16) | (0x80 << 8) | 0x80;
    // threshold also needs to be brought to U08 range
    cur_score_th += 128;
    data_offset = 128;
  }
  else
  {
    pkd_data_offset = 0;
    data_offset = 0;
  }

#ifdef MATCH_REF_CODE
  for( i = 0; i < img_height; i++)
  {
    for( j = 0; j < img_width; j++)
    {
      if((i == (guard_pixels-1)) || (i == (img_height-guard_pixels)))
      {
        score_buf[i* img_pitch + j] = 0;
      }

      if((j == (guard_pixels-1)) || (j == (img_width-guard_pixels)))
      {
        score_buf[i* img_pitch + j] = 0;
      }
    }
  }
#endif

  uint32_t u32temp1,u32temp2,u32temp3,u32temp4, u32rowScore, u32curScore;
  uint64_t* restrict key_point_listL = (uint64_t*)out_buf;
  uint32_t num_valid_idx,k;

  uint8_t* restrict scoreL = (uint8_t*)score_buf;

  for(i = guard_pixels; i < img_height - guard_pixels; i++)
  {
    num_valid_idx = 0;

#ifdef ENABLE_NMS_PROFILE_DEBUG_PRINT_LOG
t0 = _TSC_read();
#endif
    #pragma UNROLL(8)
    for(j = guard_pixels; j < img_width - guard_pixels; j++)
    {
      if((scoreL[i* img_pitch + j] + data_offset) >= cur_score_th)
      {
        validLocIdx[num_valid_idx] = j;
        num_valid_idx++;
      }
    }

#ifdef ENABLE_NMS_PROFILE_DEBUG_PRINT_LOG
    t1 = _TSC_read();
    printf("  NMS partial valid cycle => per point %f \n", ((float)(t1-t0))/(img_height- 2*guard_pixels));
    printf("  Total partial valid key points selected are %d \n", num_valid_idx);
    t0 = _TSC_read();
#endif

    for( k = 0; k < num_valid_idx; k++)
    {
      j =   validLocIdx[k];
      u32rowScore = _add4(_mem4(&scoreL[i* img_pitch + j -1]), pkd_data_offset); // 3-2-1-0 --> j+2 - j+1 - j - j-1
      u32curScore = _packh4(u32rowScore,u32rowScore); // 3-1-3-1
      u32curScore = _packl4(u32curScore,u32curScore); // 1-1-1-1

      u32temp2 = _cmpgtu4(u32curScore, u32rowScore) | 0x2; // 0000-0000-0000-0010 comarision of current with current is made always true
      u32temp3 = _cmpgtu4(u32curScore, _add4(_mem4(&scoreL[(i-1)* img_pitch + j -1]), pkd_data_offset)); // previous row
      u32temp4 = _cmpgtu4(u32curScore, _add4(_mem4(&scoreL[(i+1)* img_pitch + j -1]), pkd_data_offset)); // next row

      u32temp1 = u32temp2 & u32temp3 & u32temp4 & 0x7; // only last three comparision is valid

      if(u32temp1 != 0x7)
      {
        scoreL[i* img_pitch + j] = 0;
      }
      else
      {
        _mem8(&key_point_listL[num_key_points]) = _itoll(i,j);
        num_key_points++;
      }

      if(num_key_points >= (max_frame_feat-1))
      {
        num_key_points = (max_frame_feat-1); // in ci flow one less than maximum allowed key points is generated
      }
    }
#ifdef ENABLE_NMS_PROFILE_DEBUG_PRINT_LOG
      t1 = _TSC_read();
      printf("  NMS 2nd partial valid cycle => per point %f \n", ((float)(t1-t0))/(num_valid_idx));
#endif
  }

  return num_key_points;
}
