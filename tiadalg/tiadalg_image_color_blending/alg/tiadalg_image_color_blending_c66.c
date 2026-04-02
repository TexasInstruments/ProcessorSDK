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

static inline void tiadalg_memcpy_c66(uint8_t* restrict dst, uint8_t* restrict src, int32_t bytes);
static inline void tiadalg_aligned_memcpy_c66(uint8_t* restrict dst, uint8_t* restrict src, int32_t bytes);

#ifndef HOST_EMULATION
//#define ENABLE_PROFILE
#endif

#ifdef ENABLE_PROFILE
#include <stdio.h>
long long t0, t1, acc0, acc1;
#endif

//#define TIADALG_IMAGE_BLEND_FILL_NON_VALID_REGION

int32_t tiadalg_image_color_blending_c66(uint8_t* in_img[], int32_t in_img_width, int32_t in_img_pitch, int32_t in_img_height, int32_t num_planes,
                                       int32_t in_color_format, int32_t out_color_format, int32_t valid_region[4],
                                       uint8_t* det_plane, int32_t det_plane_pitch, float det_scale_fact, uint8_t* det_color_map_uv,
                                       uint8_t* out_img[], int32_t out_img_pitch)
{
  int32_t ret_val = TIADALG_PROCESS_SUCCESS;
  int32_t i,j;
  int32_t aligned_flow = 0x0;
  uint16_t* restrict out_img_local_ptr;
  uint16_t* restrict in_img_local_ptr;
  uint16_t* restrict u16_det_color_map_uv = (uint16_t*)det_color_map_uv;
  uint16_t uv_pkd_0,uv_pkd_1,uv_pkd_2,uv_pkd_3;
  uint32_t u32temp1;
  uint64_t u64temp1;
  int32_t scale_shift;

#ifdef ENABLE_PROFILE
  acc0=0;
  acc1=0;
#endif

  if((in_color_format != 0x0) || (out_color_format!= 0x0)){
    return TIADALG_PROCESS_FAILURE;
  }

  scale_shift = 30 - _norm((int32_t)(1/det_scale_fact));

  if(((((uint64_t)in_img[0] + valid_region[0])& 0x7) == 0x0) && ((((uint64_t)in_img[1] + valid_region[0])& 0x7) == 0x0) &&
     ((((uint64_t)out_img[0] + valid_region[0])& 0x7) == 0x0) && ((((uint64_t)out_img[1] + valid_region[0])& 0x7) == 0x0) &&
     ((in_img_pitch & 0x7) == 0) && ((out_img_pitch & 0x7) == 0x0) && (((valid_region[2] - valid_region[0] + 1) & 0x7) == 0x0)
     ){
       aligned_flow = 0x1;
     }

  if(aligned_flow == 0x0){

#ifdef TIADALG_IMAGE_BLEND_FILL_NON_VALID_REGION
    for(i = 0; i < in_img_height; i++)
    {
#else
    for(i = valid_region[1]; i <= valid_region[3]; i++)
    {
#endif
      tiadalg_memcpy_c66(out_img[0] + i*out_img_pitch, in_img[0] + i*in_img_pitch, (valid_region[2] - valid_region[0] + 1));

      if((i&0x1) == 0){

        out_img_local_ptr = (uint16_t*)&out_img[1][(i>>1)*out_img_pitch];
        in_img_local_ptr  = (uint16_t*)&in_img[1][(i>>1)*out_img_pitch];

#ifdef TIADALG_IMAGE_BLEND_FILL_NON_VALID_REGION
        if ((i < valid_region[1]) || (i > valid_region[3])){
          tiadalg_memcpy_c66(out_img_local_ptr, in_img_local_ptr, (valid_region[2] - valid_region[0] + 1));
        }
        else
#endif
        {

#ifdef TIADALG_IMAGE_BLEND_FILL_NON_VALID_REGION
          for(j = 0; j < valid_region[0]; j+=8){
            _mem8(out_img_local_ptr) = _mem8(in_img_local_ptr);
            out_img_local_ptr += 4;
            in_img_local_ptr  += 4;
          }
#else
          out_img_local_ptr += (valid_region[0] >> 1);
          in_img_local_ptr  += (valid_region[0] >> 1);
#endif
          for(j = valid_region[0]; j <= valid_region[2]; j+=8)
          {
            u64temp1 = _mem8(&det_plane[i*det_plane_pitch + j]);
            u32temp1 = _packl4(_hill(u64temp1),_loll(u64temp1));
            u64temp1 = _dshl2(_unpkbu4(u32temp1),scale_shift);

            uv_pkd_0 = u16_det_color_map_uv[_dotp2(_loll(u64temp1),0x00000001u)];
            uv_pkd_1 = u16_det_color_map_uv[_dotp2(_loll(u64temp1),0x00010000u)];
            uv_pkd_2 = u16_det_color_map_uv[_dotp2(_hill(u64temp1),0x00000001u)];
            uv_pkd_3 = u16_det_color_map_uv[_dotp2(_hill(u64temp1),0x00010000u)];

            _mem8(out_img_local_ptr) = _itoll(((uv_pkd_3<<16) | uv_pkd_2),((uv_pkd_1<<16) | uv_pkd_0));
            out_img_local_ptr += 4;
          }

#ifdef TIADALG_IMAGE_BLEND_FILL_NON_VALID_REGION
          for(j = valid_region[2]; j < in_img_width; j+=8){
            _mem8(out_img_local_ptr) = _mem8(in_img_local_ptr);
            out_img_local_ptr += 4;
            in_img_local_ptr  += 4;
          }
#endif
        }
      }
    }
  }else{
#ifdef TIADALG_IMAGE_BLEND_FILL_NON_VALID_REGION
    for(i = 0; i < in_img_height; i++)
    {
#else
    for(i = valid_region[1]; i <= valid_region[3]; i++)
    {
#endif
      tiadalg_aligned_memcpy_c66(out_img[0] + i*out_img_pitch, in_img[0] + i*in_img_pitch, (valid_region[2] - valid_region[0] + 1));

      if((i&0x1) == 0){

        out_img_local_ptr = (uint16_t*)&out_img[1][(i>>1)*out_img_pitch];
        in_img_local_ptr  = (uint16_t*)&in_img[1][(i>>1)*out_img_pitch];

#ifdef TIADALG_IMAGE_BLEND_FILL_NON_VALID_REGION
        if ((i < valid_region[1]) || (i > valid_region[3])){
          tiadalg_memcpy_c66(out_img_local_ptr, in_img_local_ptr, (valid_region[2] - valid_region[0] + 1));
        }
        else
#endif
        {

#ifdef TIADALG_IMAGE_BLEND_FILL_NON_VALID_REGION
          for(j = 0; j < valid_region[0]; j+=8){
            _mem8(out_img_local_ptr) = _mem8(in_img_local_ptr);
            out_img_local_ptr += 4;
            in_img_local_ptr  += 4;
          }
#else
          out_img_local_ptr += (valid_region[0] >> 1);
          in_img_local_ptr  += (valid_region[0] >> 1);
#endif
          for(j = valid_region[0]; j <= valid_region[2]; j+=8)
          {
            u64temp1 = _amem8(&det_plane[i*det_plane_pitch + j]);
            u32temp1 = _packl4(_hill(u64temp1),_loll(u64temp1));
            u64temp1 = _dshl2(_unpkbu4(u32temp1),scale_shift);

            uv_pkd_0 = u16_det_color_map_uv[_dotp2(_loll(u64temp1),0x00000001u)];
            uv_pkd_1 = u16_det_color_map_uv[_dotp2(_loll(u64temp1),0x00010000u)];
            uv_pkd_2 = u16_det_color_map_uv[_dotp2(_hill(u64temp1),0x00000001u)];
            uv_pkd_3 = u16_det_color_map_uv[_dotp2(_hill(u64temp1),0x00010000u)];

            _amem8(out_img_local_ptr) = _itoll(((uv_pkd_3<<16) | uv_pkd_2),((uv_pkd_1<<16) | uv_pkd_0));
            out_img_local_ptr += 4;
          }

#ifdef TIADALG_IMAGE_BLEND_FILL_NON_VALID_REGION
          for(j = valid_region[2]; j < in_img_width; j+=8){
            _mem8(out_img_local_ptr) = _mem8(in_img_local_ptr);
            out_img_local_ptr += 4;
            in_img_local_ptr  += 4;
          }
#endif
        }
      }
    }
  }

#ifdef ENABLE_PROFILE
  printf("Total Cycle consumed are % lld, %lld \n", acc0, acc1);
#endif

  return(ret_val);
}

static inline void tiadalg_memcpy_c66(uint8_t* restrict dst, uint8_t* restrict src, int32_t bytes)
{
  int32_t i;

  for(i = 0; i < bytes; i+=8){
    _mem8(&dst[i]) = _mem8(&src[i]);
  }

  for(;i < bytes; i++){
    dst[i] = src[i];
  }
}

static inline void tiadalg_aligned_memcpy_c66(uint8_t* restrict dst, uint8_t* restrict src, int32_t bytes)
{
  int32_t i;

  for(i = 0; i < bytes; i+=8){
    _amem8(&dst[i]) = _amem8(&src[i]);
  }

}
