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

  int32_t tiadalg_dof_plane_seperation_c66(uint32_t* dof_plane,
    int32_t img_width, int32_t img_height,
    int32_t data_type, int32_t pad_pixel[4],
    void* out_rgb_planes,
    void* out_yuv_planes)
{
  int32_t ret_val = TIADALG_PROCESS_SUCCESS;

  int32_t pad_img_width ;
  int32_t pad_img_height ;
  int32_t left_pad, top_pad, right_pad, bot_pad;
  int32_t i,ii;
  int32_t j,jj;
  uint32_t cur_pkd_flow;
  int32_t h_flow;
  int32_t v_flow;
  uint32_t conf;
  uint8_t * dst_rgb_ptr_u8 = NULL;
  uint16_t * dst_rgb_ptr_u16 = NULL;
  uint8_t * dst_yuv_ptr_u8 = NULL;
  uint16_t * dst_yuv_ptr_u16 = NULL;

  /*input dof_plane and rgb out planes buffers are mandatory. out_yuv_planes is not mandatory*/

  if ((dof_plane == NULL) || (out_rgb_planes == NULL)){
    ret_val = TIADALG_IN_BUF_ERR;
    return ret_val;
  }

  if(data_type == TIADALG_DATA_TYPE_U08){
    dst_rgb_ptr_u8 = (uint8_t*)out_rgb_planes;
    dst_yuv_ptr_u8 = (uint8_t*)out_yuv_planes;
  }else if(data_type == TIADALG_DATA_TYPE_U16){
    dst_rgb_ptr_u16 = (uint16_t*)out_rgb_planes;
    dst_yuv_ptr_u16 = (uint16_t*)out_yuv_planes;
  }else{
    /*Currently not supported*/
    ret_val = TIADALG_IN_PRM_ERR;
    return ret_val;
  }
  
  if(pad_pixel == NULL){
    left_pad  = 0;
    top_pad   = 0;
    right_pad = 0;
    bot_pad   = 0;
  }else{
    left_pad  = pad_pixel[0];
    top_pad   = pad_pixel[1];
    right_pad = pad_pixel[2];
    bot_pad   = pad_pixel[3];
  }

  pad_img_width = img_width + (left_pad + right_pad);
  pad_img_height = img_height + (top_pad + bot_pad);

  uint8_t * restrict dst_r_ptr_u8 = dst_rgb_ptr_u8 + top_pad*pad_img_width + left_pad;
  uint8_t * restrict dst_g_ptr_u8 = dst_r_ptr_u8 + pad_img_height*pad_img_width ;
  uint8_t * restrict dst_b_ptr_u8 = dst_g_ptr_u8 + pad_img_height*pad_img_width ;

  uint64_t cur_pkd_flow_10,cur_pkd_flow_32,hf3_hf1_hf2_hf0,vf3_vf1_vf2_vf0;
  uint64_t hf3_hf2_hf1_hf0,vf3_vf2_vf1_vf0,cf3_cf2_cf1_cf0,cf3_cf1_cf2_cf0;
  int32_t opt_flow = 0;

  if ((data_type == TIADALG_DATA_TYPE_U08) && ((img_width&0x3) == 0x0)){
    opt_flow = 1;
  }

  if(opt_flow == 1){
    for(i = 0; i < img_height; i++){
      for(j = 0; j < img_width; j+=4){
        /* |upper 16 bit horizontal flow|12 bit vertical flow| lowest 4 bit confidence|*/

        cur_pkd_flow_10 = _mem8(dof_plane); // hf1-vf1-cf1-hf0-vf0-cf0
        dof_plane += 2;
        cur_pkd_flow_32 = _mem8(dof_plane); // hf3-vf3-cf3-hf2-vf2-cf2
        dof_plane += 2;
        hf3_hf1_hf2_hf0 = _dpackh2(cur_pkd_flow_32,cur_pkd_flow_10);
        vf3_vf1_vf2_vf0 = _dpackl2(cur_pkd_flow_32,cur_pkd_flow_10);
        cf3_cf1_cf2_cf0 = vf3_vf1_vf2_vf0;

        hf3_hf1_hf2_hf0 = _dshr2(hf3_hf1_hf2_hf0,5);
        vf3_vf1_vf2_vf0 = _dshr2(vf3_vf1_vf2_vf0,9);
        hf3_hf2_hf1_hf0 = _dmv(_packh2(_hill(hf3_hf1_hf2_hf0),_loll(hf3_hf1_hf2_hf0)),_pack2(_hill(hf3_hf1_hf2_hf0),_loll(hf3_hf1_hf2_hf0)));
        vf3_vf2_vf1_vf0 = _dmv(_packh2(_hill(vf3_vf1_vf2_vf0),_loll(vf3_vf1_vf2_vf0)),_pack2(_hill(vf3_vf1_vf2_vf0),_loll(vf3_vf1_vf2_vf0)));

        cf3_cf1_cf2_cf0 = _dshru2(_dshl2(cf3_cf1_cf2_cf0,12),12);
        cf3_cf1_cf2_cf0 = _dadd2(_dshl2(cf3_cf1_cf2_cf0,4),cf3_cf1_cf2_cf0);
        cf3_cf2_cf1_cf0 = _dmv(_packh2(_hill(cf3_cf1_cf2_cf0),_loll(cf3_cf1_cf2_cf0)),_pack2(_hill(cf3_cf1_cf2_cf0),_loll(cf3_cf1_cf2_cf0)));

        hf3_hf2_hf1_hf0 = _dadd2(hf3_hf2_hf1_hf0,0x0080008000800080);
        vf3_vf2_vf1_vf0 = _dadd2(vf3_vf2_vf1_vf0,0x0080008000800080);

        _mem4(dst_r_ptr_u8) = _packl4(_hill(hf3_hf2_hf1_hf0),_loll(hf3_hf2_hf1_hf0));
        _mem4(dst_g_ptr_u8) = _packl4(_hill(vf3_vf2_vf1_vf0),_loll(vf3_vf2_vf1_vf0));
        _mem4(dst_b_ptr_u8) = _packl4(_hill(cf3_cf2_cf1_cf0),_loll(cf3_cf2_cf1_cf0));

        dst_r_ptr_u8 += 4;
        dst_g_ptr_u8 += 4;
        dst_b_ptr_u8 += 4;

      }

      dst_r_ptr_u8 += (left_pad+right_pad);
      dst_g_ptr_u8 += (left_pad+right_pad);
      dst_b_ptr_u8 += (left_pad+right_pad);

    }

    /*another loop to do RGB to YUV conversion if yuv planes buffer is provided by user*/
    if(out_yuv_planes != NULL){

      dst_r_ptr_u8 = dst_rgb_ptr_u8 + top_pad*pad_img_width + left_pad;
      dst_g_ptr_u8 = dst_r_ptr_u8 + pad_img_height*pad_img_width + top_pad*pad_img_width + left_pad;
      dst_b_ptr_u8 = dst_g_ptr_u8 + pad_img_height*pad_img_width + top_pad*pad_img_width + left_pad;

      for(i = 0; i < img_height; i++){
        for(j = 0; j < img_width; j+=4){

          h_flow = dst_r_ptr_u8[i*pad_img_width + j];
          v_flow = dst_g_ptr_u8[i*pad_img_width + j];
          conf   = dst_b_ptr_u8[i*pad_img_width + j];

          float Y = 0.299f*h_flow + 0.587f*v_flow + 0.114f*conf;
          float U = -0.147*h_flow - 0.289f*v_flow + 0.436f*conf;
          float V = 0.615f*h_flow - 0.515f*v_flow - 0.100f*conf;

          dst_yuv_ptr_u8[i*pad_img_width + j] = (uint8_t)Y;

          if((i & 0x1) == 0){
            dst_yuv_ptr_u8[(i >> 1)*pad_img_width + (j>>1)*2 + pad_img_width*pad_img_height] = (uint8_t)U;
            dst_yuv_ptr_u8[(i >> 1)*pad_img_width + (j>>1)*2 + 1 + pad_img_width*pad_img_height] = (uint8_t)V;
          }

        }
      }
    }
  }
  else{
    for (i = 0; i < pad_img_height; i++){
      for (j = 0; j < pad_img_width; j++){

        jj = j - left_pad;
        ii = i - top_pad;
        // C66 estimate, 4 elements are processed togather
        // 2 LDDW to load 4 words .D
        // 2 DPACK to pack 16 bit LSB and MSB, .L
        // 3 DSHR2 to do right shift .S
        // 2 DSHL2 to do right shift .S
        // 2 ADD2 to do bias add . .S, .L
        // 3 packl4 for pack 8 bit data. .L
        // 3 STW for 3 different plane storage.
        // roughly 3.5 cycle for 4 elements.
        if((ii >= 0) && (jj >= 0) && (ii < img_height) && (jj < img_width)){

          cur_pkd_flow = dof_plane[(ii*img_width + jj)];

          /* |upper 16 bit horizontal flow|12 bit vertical flow| lowest 4 bit confidence|*/
          h_flow = (int32_t)((int32_t)((int32_t)cur_pkd_flow + 0) >> 16);
          v_flow = (int32_t)((((int32_t)((int32_t)cur_pkd_flow + 0) << 16) + 0)>> 20);
          conf   = (uint32_t)(cur_pkd_flow << 28) >> 28;

          h_flow = ((h_flow + 0) >> 5) + 128;

          v_flow = ((v_flow + 0) >> 5) + 128;

          conf   = (conf << 4) + conf;

          /*Clipping is done assuming that output planes are given out in 8 bit unsigned format.
            From h/w prespective it is guranteed that data will not overflow from 8 bit.
            hence in optimized implementation this clipping to max can be avoided.
          */
          if (h_flow > 255){
            h_flow = 255;
          }

          if (v_flow > 255){
            v_flow = 255;
          }

          if (conf > 255){
            conf = 255;
          }

        }else{
          h_flow = 0;
          v_flow = 0;
          conf   = 0;
        }
        if(out_rgb_planes != NULL){
          if(data_type == TIADALG_DATA_TYPE_U08){
            /*First plane horizontal flow*/
            dst_rgb_ptr_u8[i*pad_img_width + j] = (uint8_t)h_flow;
            /*Second plane vertical flow*/
            dst_rgb_ptr_u8[i*pad_img_width + j + pad_img_width*pad_img_height] = (uint8_t)v_flow;
            /*Third plane confidence*/
            dst_rgb_ptr_u8[i*pad_img_width + j + 2*pad_img_width*pad_img_height] = (uint8_t)conf;
          }else if(data_type == TIADALG_DATA_TYPE_U16){
            /*First plane horizontal flow*/
            dst_rgb_ptr_u16[i*pad_img_width + j] = (uint16_t)h_flow;
            /*Second plane vertical flow*/
            dst_rgb_ptr_u16[i*pad_img_width + j + pad_img_width*pad_img_height] = (uint16_t)v_flow;
            /*Third plane confidence*/
            dst_rgb_ptr_u16[i*pad_img_width + j + 2*pad_img_width*pad_img_height] = (uint16_t)conf;
          }
        }
        if(out_yuv_planes != NULL){
          float Y = 0.299f*h_flow + 0.587f*v_flow + 0.114f*conf;
          float U = -0.147*h_flow - 0.289f*v_flow + 0.436f*conf;
          float V = 0.615f*h_flow - 0.515f*v_flow - 0.100f*conf;

          if(data_type == TIADALG_DATA_TYPE_U08){
            dst_yuv_ptr_u8[i*pad_img_width + j] = (uint8_t)Y;
            if((i & 0x1) == 0){
              dst_yuv_ptr_u8[(i >> 1)*pad_img_width + (j>>1)*2 + pad_img_width*pad_img_height] = (uint8_t)U;
              dst_yuv_ptr_u8[(i >> 1)*pad_img_width + (j>>1)*2 + 1 + pad_img_width*pad_img_height] = (uint8_t)V;
            }
          }else if(data_type == TIADALG_DATA_TYPE_U16){
            dst_yuv_ptr_u16[i*pad_img_width + j] = (uint16_t)Y;
            if((i & 0x1) == 0){
              dst_yuv_ptr_u16[(i >> 1)*pad_img_width + (j>>1)*2 + pad_img_width*pad_img_height] = (uint16_t)U;
              dst_yuv_ptr_u16[(i >> 1)*pad_img_width + (j>>1)*2 + 1 + pad_img_width*pad_img_height] = (uint16_t)V;
            }
          }
        }
      }
    }
  }
  return(ret_val);
}

