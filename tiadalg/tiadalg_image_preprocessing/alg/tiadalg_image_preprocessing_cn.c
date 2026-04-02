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

int32_t tiadalg_image_preprocessing_cn(void* in_img[], int32_t in_width, int32_t in_height, int32_t in_stride,
                            int32_t data_type, int32_t color_conv_type, float scale_val[],
                            float mean_pixel[], int32_t pad_pixel[4], void* out_img)
{
  int32_t ret_val = TIADALG_PROCESS_SUCCESS;
  float cur_pix;

  int32_t pad_img_width = in_width + pad_pixel[0] + pad_pixel[2];
  int32_t pad_img_height = in_height + pad_pixel[1] + pad_pixel[3];
  int32_t i,ii;
  int32_t j,jj;
  uint8_t * src_ptr;
  uint8_t *src_ptr_1;
  uint8_t * dst_ptr_u8 = NULL;
  int8_t  * dst_ptr_s8 = NULL;
  uint16_t * dst_ptr_u16 = NULL;
  int16_t  * dst_ptr_s16 = NULL;
  float min_val,max_val;

  if (in_img == NULL)
    ret_val = ret_val | TIADALG_IN_BUF_ERR;

  if (out_img == NULL)
    ret_val = ret_val | TIADALG_OUT_BUF_ERR;

  if(ret_val != TIADALG_PROCESS_SUCCESS){
    return ret_val;
  }

  src_ptr   =  (uint8_t*)in_img[0];
  src_ptr_1 =  (uint8_t*)in_img[1];

  if(data_type == TIADALG_DATA_TYPE_U08){
    dst_ptr_u8 = (uint8_t*)out_img;
    min_val    = 0.0;
    max_val   = 255.0;
  }else if (data_type == TIADALG_DATA_TYPE_S08){
    dst_ptr_s8 = (int8_t*)out_img;
    min_val    = -128.0;
    max_val   = 127.0;
  }else if (data_type == TIADALG_DATA_TYPE_U16){
    dst_ptr_u16 = (uint16_t*)out_img;
    //min_val    = 0.0;
    //max_val   = 65535.0;

    min_val    = 0.0;
    max_val    = 255.0;

  }else if (data_type == TIADALG_DATA_TYPE_S16){
    dst_ptr_s16 = (int16_t*)out_img;
    //min_val    = -32768.0;
    //max_val   = 32767.0;

    min_val    = -128.0;
    max_val    = 127.0;

  }else{
    /*Currently not supported*/
    ret_val = ret_val | TIADALG_IN_PRM_ERR;
    return ret_val;
  }

  if ((color_conv_type != TIADALG_COLOR_CONV_YUV420_RGB) &&
      (color_conv_type != TIADALG_COLOR_CONV_YUV420_BGR) &&
      (color_conv_type != TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB) &&
      (color_conv_type != TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR) &&
      (color_conv_type != TIADALG_COLOR_CONV_RGB_YUV420) &&
      (color_conv_type != TIADALG_COLOR_CONV_BGR_YUV420)
      ){
    /*Currently not supported any other color conversion*/
    ret_val = ret_val | TIADALG_IN_PRM_ERR;
    return ret_val;
  }

  int32_t out_plane;

  if(color_conv_type == TIADALG_COLOR_CONV_YUV420_RGB)
  {
    for (out_plane = 0 ; out_plane < 3; out_plane++){

      for (i = 0; i < pad_img_height; i++){
        for (j = 0; j < pad_img_width; j++){

          jj = j - pad_pixel[0];
          ii = i - pad_pixel[1];

          if((ii >= 0) && (jj >= 0) && (ii < in_height) && (jj < in_width)){

            /*YUV420 to RGB conversion*/
            int32_t Y  = src_ptr[(ii*in_stride + jj)];
            int32_t Cb = src_ptr_1[((ii >> 1)*in_stride + (jj >> 1)*2) ] ;
            int32_t Cr = src_ptr_1[((ii >> 1)*in_stride + (jj >> 1)*2) + 1] ;

            Y  = Y  - 16;
            Cb = Cb - 128;
            Cr = Cr - 128;

            cur_pix = 0;

            if(out_plane == 0){ /*R*/
              cur_pix = (float)((298*Y + 409*Cr) >> 8);
            }
            if(out_plane == 1){ /*G*/
              cur_pix = (float)((298*Y - 100*Cb - 208*Cr) >> 8);
            }
            if(out_plane == 2){ /*B*/
              cur_pix = (float)((298*Y + 516*Cb) >> 8);
            }
            if (cur_pix < min_val){
              cur_pix = min_val;
            }
            if (cur_pix > max_val){
              cur_pix = max_val;
            }
          }
          else{
            cur_pix = 0.0f;
          }

          cur_pix = (cur_pix - mean_pixel[out_plane])*scale_val[out_plane];

          if(data_type == TIADALG_DATA_TYPE_U08){
            dst_ptr_u8[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (uint8_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_S08){
            dst_ptr_s8[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (int8_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_U16){
            dst_ptr_u16[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (uint16_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_S16){
            dst_ptr_s16[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (int16_t)cur_pix;
          }
        }
      }
    }
  }
  if(color_conv_type == TIADALG_COLOR_CONV_YUV420_BGR)
  {
    for (out_plane = 0 ; out_plane < 3; out_plane++){

      for (i = 0; i < pad_img_height; i++){
        for (j = 0; j < pad_img_width; j++){

          jj = j - pad_pixel[0];
          ii = i - pad_pixel[1];

          if((ii >= 0) && (jj >= 0) && (ii < in_height) && (jj < in_width)){

            /*YUV420 to RGB conversion*/
            int32_t Y  = src_ptr[(ii*in_stride + jj)];
            int32_t Cb = src_ptr_1[((ii >> 1)*in_stride + (jj >> 1)*2) ] ;
            int32_t Cr = src_ptr_1[((ii >> 1)*in_stride + (jj >> 1)*2) +  1] ;

            Y  = Y  - 16;
            Cb = Cb - 128;
            Cr = Cr - 128;

            if(out_plane == 2){ /*R*/
              cur_pix = (float)((298*Y + 409*Cr) >> 8);
            }
            if(out_plane == 1){ /*G*/
              cur_pix = (float)((298*Y - 100*Cb - 208*Cr) >> 8);
            }
            if(out_plane == 0){ /*B*/
              cur_pix = (float)((298*Y + 516*Cb) >> 8);
            }
            if (cur_pix < min_val){
              cur_pix = min_val;
            }
            if (cur_pix > max_val){
              cur_pix = max_val;
            }
          }
          else{
            cur_pix=0.0f;
          }

          cur_pix = (cur_pix - mean_pixel[out_plane])*scale_val[out_plane];

          if(data_type == TIADALG_DATA_TYPE_U08){
            dst_ptr_u8[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (uint8_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_S08){
            dst_ptr_s8[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (int8_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_U16){
            dst_ptr_u16[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (uint16_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_S16){
            dst_ptr_s16[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (int16_t)cur_pix;
          }
        }
      }
    }
  }
  if((color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR) || (color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB))
  {
    for (out_plane = 0 ; out_plane < 3; out_plane++){

      for (i = 0; i < pad_img_height; i++){
        for (j = 0; j < pad_img_width; j++){

          jj = j - pad_pixel[0];
          ii = i - pad_pixel[1];

          if((ii >= 0) && (jj >= 0) && (ii < in_height) && (jj < in_width)){

            if(color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB)
              cur_pix = (float)src_ptr[ii*in_stride + jj*3 + out_plane] ;
            else
              cur_pix = (float)src_ptr[ii*in_stride + jj*3 + (2 - out_plane)] ;

          }
          else
          {
            cur_pix = 0.0f;
          }

          cur_pix = (cur_pix - mean_pixel[out_plane])*scale_val[out_plane];

          if(data_type == TIADALG_DATA_TYPE_U08){
            dst_ptr_u8[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (uint8_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_S08){
            dst_ptr_s8[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (int8_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_U16){
            dst_ptr_u16[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (uint16_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_S16){
            dst_ptr_s16[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (int16_t)cur_pix;
          }
        }
      }
    }
  }
  if(color_conv_type == TIADALG_COLOR_CONV_RGB_YUV420)
  {
    for (out_plane = 0 ; out_plane < 3; out_plane++){

      uint8_t R,G,B;

      if(out_plane > 0)
        pad_img_height = pad_img_height >> 1;

      for (i = 0; i < pad_img_height; i++){
        for (j = 0; j < pad_img_width; j++){

          jj = j - pad_pixel[0];
          ii = i - pad_pixel[1];

          if((ii >= 0) && (jj >= 0) && (ii < in_height) && (jj < in_width)){

            if(out_plane == 0){ /*Y*/
              R  = src_ptr[(ii*in_stride + jj) + 0];
              G  = src_ptr[(ii*in_stride + jj) + 1];
              B  = src_ptr[(ii*in_stride + jj) + 2];
              cur_pix = (float)((( (66 * R) + (129 * G) + (25 * B) + 128) >> 8) + 16);
            }
            if(out_plane == 1){ /*U*/
              /*take top left for U/V computation*/
              ii = (ii >> 1)*2;
              jj = (jj >> 1)*2;

              R  = src_ptr[(ii*in_stride + jj) + 0];
              G  = src_ptr[(ii*in_stride + jj) + 1];
              B  = src_ptr[(ii*in_stride + jj) + 2];
              cur_pix = (float)((( (-38 * R) - (74 * G) + (112 * B) + 128) >> 8) + 128);
            }
            if(out_plane == 2){ /*V*/
              /*take top left for U/V computation*/
              ii = (ii >> 1)*2;
              jj = (jj >> 1)*2;

              R  = src_ptr[(ii*in_stride + jj) + 0];
              G  = src_ptr[(ii*in_stride + jj) + 1];
              B  = src_ptr[(ii*in_stride + jj) + 2];
              cur_pix = (float)((((112 * R) - (94 * G) - (18 * B) + 128) >> 8) + 128);
            }
          }
          else
          {
            cur_pix = 0;
          }

          cur_pix = (cur_pix - mean_pixel[out_plane])*scale_val[out_plane];

          if(out_plane == 0){ /*Y*/
            if(data_type == TIADALG_DATA_TYPE_U08){
              dst_ptr_u8[i*pad_img_width + j] = (uint8_t)cur_pix;
            }else if(data_type == TIADALG_DATA_TYPE_S08){
              dst_ptr_s8[i*pad_img_width + j] = (int8_t)cur_pix;
            }else if(data_type == TIADALG_DATA_TYPE_U16){
              dst_ptr_u16[i*pad_img_width + j] = (uint16_t)cur_pix;
            }else if(data_type == TIADALG_DATA_TYPE_S16){
              dst_ptr_s16[i*pad_img_width + j] = (int16_t)cur_pix;
            }
          }else if(out_plane == 1){/*U*/
            if(data_type == TIADALG_DATA_TYPE_U08){
              dst_ptr_u8[pad_img_width*pad_img_height + i*pad_img_width + 2*(j >> 1)] = (uint8_t)cur_pix;
            }else if(data_type == TIADALG_DATA_TYPE_S08){
              dst_ptr_s8[pad_img_width*pad_img_height + i*pad_img_width + 2*(j >> 1)] = (int8_t)cur_pix;
            }else if(data_type == TIADALG_DATA_TYPE_U16){
              dst_ptr_u16[pad_img_width*pad_img_height + i*pad_img_width + 2*(j >> 1)] = (uint16_t)cur_pix;
            }else if(data_type == TIADALG_DATA_TYPE_S16){
              dst_ptr_s16[pad_img_width*pad_img_height + i*pad_img_width + 2*(j >> 1)] = (int16_t)cur_pix;
            }
          }else{
            if(data_type == TIADALG_DATA_TYPE_U08){
              dst_ptr_u8[pad_img_width*pad_img_height + i*pad_img_width + 2*(j >> 1) + 1] = (uint8_t)cur_pix;
            }else if(data_type == TIADALG_DATA_TYPE_S08){
              dst_ptr_s8[pad_img_width*pad_img_height + i*pad_img_width + 2*(j >> 1) + 1] = (int8_t)cur_pix;
            }else if(data_type == TIADALG_DATA_TYPE_U16){
              dst_ptr_u16[pad_img_width*pad_img_height + i*pad_img_width + 2*(j >> 1) + 1] = (uint16_t)cur_pix;
            }else if(data_type == TIADALG_DATA_TYPE_S16){
              dst_ptr_s16[pad_img_width*pad_img_height + i*pad_img_width + 2*(j >> 1) + 1] = (int16_t)cur_pix;
            }
          }
        }
      }
    }
  }
  if(color_conv_type == TIADALG_COLOR_CONV_BGR_YUV420)
  {
    for (out_plane = 0 ; out_plane < 3; out_plane++){
      for (i = 0; i < pad_img_height; i++){
        for (j = 0; j < pad_img_width; j++){

          jj = j - pad_pixel[0];
          ii = i - pad_pixel[1];
          jj = j - pad_pixel[0];
          ii = i - pad_pixel[1];

          if((ii >= 0) && (jj >= 0) && (ii < in_height) && (jj < in_width)){

            uint8_t B  = src_ptr[(ii*in_stride + jj) + 0];
            uint8_t G  = src_ptr[(ii*in_stride + jj) + 1];
            uint8_t R  = src_ptr[(ii*in_stride + jj) + 2];

            if(out_plane == 0){ /*Y*/
              cur_pix = (float)((( (66 * R) + (129 * G) + (25 * B) + 128) >> 8) + 16);
            }
            if(out_plane == 1){ /*U*/
              cur_pix = (float)((( (-38 * R) - (74 * G) + (112 * B) + 128) >> 8) + 128);
            }
            if(out_plane == 2){ /*V*/
              cur_pix = (float)((((112 * R) - (94 * G) - (18 * B) + 128) >> 8) + 128);
            }
          }
          else
          {
            cur_pix = 0;
          }

          cur_pix = (cur_pix - mean_pixel[out_plane])*scale_val[out_plane];

          if(data_type == TIADALG_DATA_TYPE_U08){
            dst_ptr_u8[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j]  = (uint8_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_S08){
            dst_ptr_s8[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j]  = (int8_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_U16){
            dst_ptr_u16[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (uint16_t)cur_pix;
          }else if(data_type == TIADALG_DATA_TYPE_S16){
            dst_ptr_s16[pad_img_width*pad_img_height*out_plane + i*pad_img_width + j] = (int16_t)cur_pix;
          }
        }
      }
    }
  }

  return(ret_val);
}
