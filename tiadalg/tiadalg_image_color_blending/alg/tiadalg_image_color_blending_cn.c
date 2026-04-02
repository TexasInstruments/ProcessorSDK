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
#include <string.h>
#include <stdio.h>

int32_t tiadalg_image_color_blending_cn(uint8_t* in_img[], int32_t in_img_width, int32_t in_img_pitch, int32_t in_img_height,
                                        int32_t num_planes, int32_t in_color_format, int32_t out_color_format, int32_t valid_region[4],
                                       uint8_t* det_plane, int32_t det_plane_pitch, float det_scale_fact, uint8_t* det_color_map_uv,
                                       uint8_t* out_img[], int32_t out_img_pitch)
{
  int32_t ret_val = TIADALG_PROCESS_SUCCESS;
  int32_t i,j;
  int32_t cur_det;
  int32_t Y,U,V;
  int32_t R,G,B;
  int32_t min_val = 0;
  int32_t max_val = 255;

  if(!(((in_color_format == 0x0) && (out_color_format == 0x0)) ||
     ((in_color_format == 0x1) && (out_color_format == 0x1))))
  {
    return TIADALG_PROCESS_FAILURE;
  }

  if((in_color_format == 0x0) && (out_color_format == 0x0)){
    for(i = valid_region[1]; i <= valid_region[3]; i++)
    {
      memcpy(out_img[0] + i*out_img_pitch, in_img[0] + i*in_img_pitch, (valid_region[2] - valid_region[0] + 1));

      for(j = valid_region[0]; j <= valid_region[2]; j++)
      {
        cur_det = det_plane[i*det_plane_pitch + j];
        cur_det = (int32_t)(cur_det * (1.0/det_scale_fact));

        U = det_color_map_uv[2*cur_det + 0];
        V = det_color_map_uv[2*cur_det + 1];

        if(((i&0x1) == 0) &&((j&0x1)==0))
        {
          out_img[1][((i>>1)*out_img_pitch) + ((j>>1)*2) + 0] = U;
          out_img[1][((i>>1)*out_img_pitch) + ((j>>1)*2) + 1] = V;
        }
      }
    }
  }else{
    /*In this flow RGB/BGR interleaved data is expected as input and output*/
    for(i = valid_region[1]; i <= valid_region[3]; i++){
      for(j = valid_region[0]; j <= valid_region[2]; j++){

        R = (int32_t)in_img[0][(i*in_img_width + j)*3 + 0];
        G = (int32_t)in_img[0][(i*in_img_width + j)*3 + 1];
        B = (int32_t)in_img[0][(i*in_img_width + j)*3 + 2];

        cur_det = det_plane[i*det_plane_pitch + j];
        cur_det = (int32_t)(cur_det * (1.0/det_scale_fact));

        Y = ((( (66 * R) + (129 * G) + (25 * B) + 128) >> 8));
        U = det_color_map_uv[2*cur_det + 0] - 128;
        V = det_color_map_uv[2*cur_det + 1] - 128;
        R = ((298*Y + 409*V) >> 8);
        G = ((298*Y - 100*U - 208*V) >> 8);
        B = ((298*Y + 516*U) >> 8);


        if (R < min_val){
          R = min_val;
        }
        if (R > max_val){
          R = max_val;
        }

        if (G < min_val){
          G = min_val;
        }
        if (G > max_val){
          G = max_val;
        }

        if (B < min_val){
          B = min_val;
        }
        if (B > max_val){
          B = max_val;
        }

        out_img[0][(i*in_img_width + j)*3 + 0] = R;
        out_img[0][(i*in_img_width + j)*3 + 1] = G;
        out_img[0][(i*in_img_width + j)*3 + 2] = B;
      }
    }
  }

  return(ret_val);
}

