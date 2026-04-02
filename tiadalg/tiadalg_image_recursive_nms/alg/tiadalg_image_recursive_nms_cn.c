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

#define MATCH_REF_CODE
#define TIADALG_NMS_ROW_FLOW

int32_t tiadalg_image_recursive_nms_cn(int8_t* score_buf, int32_t img_width, int32_t img_pitch, int32_t img_height,
                                       int32_t cur_score_th, int32_t max_frame_feat,
                                       int32_t guard_pixels, int8_t* scratch, int32_t data_type,
                                       int32_t* out_buf)
{
  int32_t i,j;
  int32_t num_key_points = 0;
  int32_t invalid_value;

#ifdef ENABLE_PROFILE
  t0 = _TSC_read();
#endif

  if((data_type != TIADALG_DATA_TYPE_S08) && (data_type != TIADALG_DATA_TYPE_U08))
  {
    return num_key_points;
  }

  if(data_type == TIADALG_DATA_TYPE_S08)
  {
    invalid_value = -128;
  }
  else
  {
    invalid_value = 0;
  }

#ifdef MATCH_REF_CODE
  for( i = 0; i < img_height; i++)
  {
    for( j = 0; j < img_width; j++)
    {
      if((i == (guard_pixels-1)) || (i == (img_height-guard_pixels)))
      {
        score_buf[i* img_pitch + j] = invalid_value;
      }

      if((j == (guard_pixels-1)) || (j == (img_width-guard_pixels)))
      {
        score_buf[i* img_pitch + j] = invalid_value;
      }
    }
  }
#endif

#ifdef ENABLE_PROFILE
  t1 = _TSC_read();
  printf("Score masking cycle => per point %lld \n", (t1-t0)/(img_width*img_height));
#endif
#ifdef ENABLE_PROFILE
  t0 = _TSC_read();
#endif

#ifdef TIADALG_NMS_ROW_FLOW
  for( i = guard_pixels; i < img_height - guard_pixels; i++)
  {
    for( j = guard_pixels; j < img_width - guard_pixels; j++)
    {
#else
  for( j = guard_pixels; j < img_width - guard_pixels; j++)
  {
    for( i = guard_pixels; i < img_height - guard_pixels; i++)
    {
#endif
      if(data_type == TIADALG_DATA_TYPE_S08)
      {
          int32_t cur_score = score_buf[i* img_pitch + j];
          if((cur_score >= cur_score_th) &&(cur_score > (score_buf[i* img_pitch + j -1])) &&
            (cur_score > ((score_buf[i* img_pitch + j + 1]))) && (cur_score > ((score_buf[(i-1)* img_pitch + j - 1]))) &&
            (cur_score > ((score_buf[(i-1)* img_pitch + j]))) && (cur_score > ((score_buf[(i-1)* img_pitch + j + 1]))) &&
            (cur_score > ((score_buf[(i+1)* img_pitch + j - 1]))) && (cur_score > ((score_buf[(i+1)* img_pitch + j]))) &&
            (cur_score > ((score_buf[(i+1)* img_pitch + j + 1])))
            )
            {
              out_buf[2*num_key_points + 0] =  j;
              out_buf[2*num_key_points + 1] =  i;
              num_key_points++;
              if(num_key_points >= (max_frame_feat - 1))
              {
                break;
              }
          }
          else
          {
            score_buf[i* img_pitch + j] = invalid_value;
          }
      }
      else
      {
          uint32_t cur_score = (uint8_t)score_buf[i* img_pitch + j];
          if((cur_score >= (uint8_t)cur_score_th) &&(cur_score > (uint8_t)(score_buf[i* img_pitch + j -1])) &&
            (cur_score > (uint8_t)((score_buf[i* img_pitch + j + 1]))) && (cur_score > (uint8_t)((score_buf[(i-1)* img_pitch + j - 1]))) &&
            (cur_score > (uint8_t)((score_buf[(i-1)* img_pitch + j]))) && (cur_score > (uint8_t)((score_buf[(i-1)* img_pitch + j + 1]))) &&
            (cur_score > (uint8_t)((score_buf[(i+1)* img_pitch + j - 1]))) && (cur_score > (uint8_t)((score_buf[(i+1)* img_pitch + j]))) &&
            (cur_score > (uint8_t)((score_buf[(i+1)* img_pitch + j + 1])))
            )
            {
              out_buf[2*num_key_points + 0] =  j;
              out_buf[2*num_key_points + 1] =  i;
              num_key_points++;
              if(num_key_points >= (max_frame_feat - 1))
              {
                break;
              }
          }
          else
          {
            score_buf[i* img_pitch + j] = invalid_value;
          }
      }
    }
  }

#ifdef ENABLE_NMS_PROFILE_DEBUG_PRINT_LOG
  printf("  Total key points selected are %d \n", num_key_points);
#endif

#ifdef ENABLE_NMS_PROFILE_DEBUG_PRINT_LOG
  t1 = _TSC_read();
  printf("NMS cycle => per point %f \n", ((float)(t1-t0))/(img_width*img_height));
#endif

 return(num_key_points);
}
