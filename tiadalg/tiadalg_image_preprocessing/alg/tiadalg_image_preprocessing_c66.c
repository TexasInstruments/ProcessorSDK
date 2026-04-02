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
//#define INNER_LOOP_PROFILE

int32_t tiadalg_image_preprocessing_c66(void* in_img[], int32_t in_width, int32_t in_height, int32_t in_stride,
                            int32_t data_type, int32_t color_conv_type, float* scale_val,
                            float* mean_pixel, int32_t pad_pixel[4], void* out_img)
{

#ifdef INNER_LOOP_PROFILE
  long long t2, t3;
  t2 = _TSC_read();
#endif

  int32_t ret_val = TIADALG_PROCESS_SUCCESS;
  int32_t pad_img_width = in_width + pad_pixel[0] + pad_pixel[2];
  int32_t pad_img_height = in_height + pad_pixel[1] + pad_pixel[3];
  int32_t i,j;
  uint8_t * src_ptr;
  uint8_t * src_ptr_1;

  uint8_t * dst_ptr_u8 = NULL;
  uint16_t * dst_ptr_u16 = NULL;

  int32_t opt_flow=0;

  if (in_img == NULL)
    ret_val = ret_val | TIADALG_IN_BUF_ERR;

  if (out_img == NULL)
    ret_val = ret_val | TIADALG_OUT_BUF_ERR;

  if(data_type == TIADALG_DATA_TYPE_U08){
    dst_ptr_u8 = (uint8_t*)out_img;
  }else if(data_type == TIADALG_DATA_TYPE_U16){
    dst_ptr_u16 = (uint16_t*)out_img;
  }else{
    /*Currently not supported any other data format for optimized flow*/
    ret_val = ret_val | TIADALG_IN_PRM_ERR;
  }

  if((scale_val[0] == 1.0f)&& (scale_val[1] == 1.0f)&& (scale_val[2] == 1.0f)&&
     (mean_pixel[0] == 0.0f)&& (mean_pixel[1] == 0.0f)&& (mean_pixel[2] == 0.0f)&&
     ((data_type == TIADALG_DATA_TYPE_U08) || (data_type == TIADALG_DATA_TYPE_U16)) &&
     ((in_width & 0x7) == 0x0) &&
     ((color_conv_type == TIADALG_COLOR_CONV_YUV420_RGB)|| (color_conv_type == TIADALG_COLOR_CONV_YUV420_BGR) ||
      (color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR)|| (color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB)
     )
     ){
    opt_flow = 1;
  }

  if (opt_flow == 0x0){
    /*Currently not supporting any other configuration for optimized flow*/
    ret_val = ret_val | TIADALG_IN_PRM_ERR;
  }

  if(ret_val != TIADALG_PROCESS_SUCCESS){
    return ret_val;
  }

  src_ptr = (uint8_t*)in_img[0];
  src_ptr_1 = (uint8_t*)in_img[1];

  if(opt_flow == 1)
  {
    uint8_t * restrict y_vec_ptr    = (uint8_t*)src_ptr;
    uint8_t * restrict crcb_vec_ptr = (uint8_t*)src_ptr_1;
    uint64_t * restrict y_row_vec_ptr = (uint64_t*)y_vec_ptr;
    uint64_t * restrict crcb_row_vec_ptr = (uint64_t*)crcb_vec_ptr;

    uint64_t pkd_149 = 0x4A4A4A4A4A4A4A4A;
    uint64_t pkd_204_50=0x6619661966196619;
    uint64_t pkd_104_129 = 0x3480348034803480;
    uint64_t y_vec,crcb_vec,cr10_204,cr32_204,cb10_50,cb32_50;
    __x128_t y_149;
    __x128_t cr_204_cb_50;
    __x128_t cr_104_cb_129;
    uint64_t cb10_129,cb32_129,cr10_104,cr32_104;
    uint64_t u64R3R2R1R0,u64R7R6R5R4,u64temp1;

    uint8_t * restrict dst_ptr_r_u8 = NULL;
    uint8_t * restrict dst_ptr_g_u8 = NULL;
    uint8_t * restrict dst_ptr_b_u8 = NULL;

    uint16_t * restrict dst_ptr_r_u16 = NULL;
    uint16_t * restrict dst_ptr_g_u16 = NULL;
    uint16_t * restrict dst_ptr_b_u16 = NULL;

    if(data_type == TIADALG_DATA_TYPE_U08){
		if ((color_conv_type == TIADALG_COLOR_CONV_YUV420_RGB) || (color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB)){
		  dst_ptr_r_u8  = dst_ptr_u8 + pad_pixel[1]*pad_img_width + pad_pixel[0];
		  dst_ptr_g_u8  = dst_ptr_r_u8 + pad_img_height*pad_img_width;
		  dst_ptr_b_u8  = dst_ptr_g_u8 + pad_img_height*pad_img_width;
		}

		if((color_conv_type == TIADALG_COLOR_CONV_YUV420_BGR) || (color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR)){
		  dst_ptr_b_u8  = dst_ptr_u8 + pad_pixel[1]*pad_img_width + pad_pixel[0];
		  dst_ptr_g_u8  = dst_ptr_b_u8 + pad_img_height*pad_img_width;
		  dst_ptr_r_u8  = dst_ptr_g_u8 + pad_img_height*pad_img_width;
		}
	}else{
		if ((color_conv_type == TIADALG_COLOR_CONV_YUV420_RGB) || (color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB)){
		  dst_ptr_r_u16  = dst_ptr_u16 + pad_pixel[1]*pad_img_width + pad_pixel[0];
		  dst_ptr_g_u16  = dst_ptr_r_u16 + pad_img_height*pad_img_width;
		  dst_ptr_b_u16  = dst_ptr_g_u16 + pad_img_height*pad_img_width;
		}

		if((color_conv_type == TIADALG_COLOR_CONV_YUV420_BGR) || (color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR)){
		  dst_ptr_b_u16  = dst_ptr_u16 + pad_pixel[1]*pad_img_width + pad_pixel[0];
		  dst_ptr_g_u16  = dst_ptr_b_u16 + pad_img_height*pad_img_width;
		  dst_ptr_r_u16  = dst_ptr_g_u16 + pad_img_height*pad_img_width;
		}
	}

    /* Original equations
    R = (298*(Y-16) + 409*(Cr-128) + 128)/256
    G = (298*(Y-16) - 100*(Cb-128) - 208*(Cr-128) + 128)/256
    B = (298*(Y-16) + 516*(Cb-128) + 128)/256

    Simplified
    R = (149*Y + 0       + 204*Cr - 28496)/128. Numerator is saturated to 1Sign + 15 bit. Hence R is staturated to 1Sign + 8 bit. then spack clips it to 0-255 unsigned
    G = (149*Y - 50*Cb   - 104*Cr + 17392)/128. Numerator is saturated to 1Sign + 15 bit. Hence G is staturated to 1Sign + 8 bit. then spack clips it to 0-255 unsigned
    B = (74*Y  + 129*Cb   + 0      - 17671)/64. Numerator is saturated to 1Sign + 15 bit. Hence B is staturated to 1Sign + 9 bit. then spack clips it to 0-255 unsigned

    Further simplified
    R = (74*Y + 0       + 102*Cr - 14248)/64. Numerator is saturated to 1Sign + 15 bit. Hence R is staturated to 1Sign + 8 bit. then spack clips it to 0-255 unsigned
    G = (74*Y - 25*Cb   - 52*Cr + 8696)/64. Numerator is saturated to 1Sign + 15 bit. Hence G is staturated to 1Sign + 8 bit. then spack clips it to 0-255 unsigned
    B = (74*Y  + 128*Cb   + Cb      - 17671)/64. Numerator is saturated to 1Sign + 15 bit. Hence B is staturated to 1Sign + 9 bit. then spack clips it to 0-255 unsigned

    */
    if((color_conv_type == TIADALG_COLOR_CONV_YUV420_RGB) || (color_conv_type == TIADALG_COLOR_CONV_YUV420_BGR))
    {
	  if(data_type == TIADALG_DATA_TYPE_U08){
		  for(i = 0; i < in_height; i++){
			y_row_vec_ptr    = (uint64_t*)&y_vec_ptr[i*in_stride];
			crcb_row_vec_ptr = (uint64_t*)&crcb_vec_ptr[(i >> 1)*in_stride];

			for(j = 0; j < in_width; j+=8){ // achieved ii is 10. so 10/8 cycle per pixel
			  y_vec           = _mem8(y_row_vec_ptr++); //Y7Y6Y5Y4Y3Y2Y1Y0
			  crcb_vec        = _mem8(crcb_row_vec_ptr++); //Cr3Cb3Cr2Cb2-Cr1Cb1Cr0Cb0

			  y_149           = _dmpyu4(y_vec,pkd_149);      //149*Y7-149*Y6-149*Y5-149*Y4-149*Y3-149*Y2-149*Y1-149*Y0
			  cr_204_cb_50    = _dmpyu4(crcb_vec,pkd_204_50);//204*Cr3-50*Cb3-204*Cr2-50*Cb2-204*Cr1-50*Cb1-204*Cr0-50*Cb0
			  cr_104_cb_129   = _dmpyu4(crcb_vec,pkd_104_129);//104*Cr3-129*Cb3-104*Cr2-129*Cb2-104*Cr1-129*Cb1-104*Cr0-129*Cb0

			  cb10_50         = _dpackl2(_lo128(cr_204_cb_50),_lo128(cr_204_cb_50)); //50*Cb1-50*Cb1-50*Cb0-50*Cb0
			  cb32_50         = _dpackl2(_hi128(cr_204_cb_50),_hi128(cr_204_cb_50)); //50*Cb3-50*Cb3-50*Cb2-50*Cb2
			  cr10_204        = _dpackh2(_lo128(cr_204_cb_50),_lo128(cr_204_cb_50)); //204*Cr1-204*Cr1-204*Cr0-204*Cr0
			  cr32_204        = _dpackh2(_hi128(cr_204_cb_50),_hi128(cr_204_cb_50)); //204*Cr3-204*Cr3-204*Cr2-204*Cr2

			  cb10_129        = _dpackl2(_lo128(cr_104_cb_129),_lo128(cr_104_cb_129)); //129*Cb1-129*Cb1-129*Cb0-129*Cb0
			  cb32_129        = _dpackl2(_hi128(cr_104_cb_129),_hi128(cr_104_cb_129)); //129*Cb3-129*Cb3-129*Cb2-129*Cb2
			  cr10_104        = _dpackh2(_lo128(cr_104_cb_129),_lo128(cr_104_cb_129)); //104*Cr1-104*Cr1-104*Cr0-104*Cr0
			  cr32_104        = _dpackh2(_hi128(cr_104_cb_129),_hi128(cr_104_cb_129)); //104*Cr3-104*Cr3-104*Cr2-104*Cr2

			  //R3-R2-R1-R0
			  u64R3R2R1R0   = _dsadd2(_lo128(y_149),0xc858c858c858c858); // 0x90b0 = -28496
			  u64R3R2R1R0   = _dsadd2(u64R3R2R1R0, cr10_204);
			  u64R3R2R1R0   = _dshr2(u64R3R2R1R0,6);

			  //R7-R6-R5-R4
			  u64R7R6R5R4   = _dsadd2(_hi128(y_149),0xc858c858c858c858);
			  u64R7R6R5R4   = _dsadd2(u64R7R6R5R4, cr32_204);
			  u64R7R6R5R4   = _dshr2(u64R7R6R5R4,6);
			  _mem8(dst_ptr_r_u8) = _dspacku4(u64R3R2R1R0,u64R7R6R5R4);
			  dst_ptr_r_u8+=8;
			  //G3-G2-G1-G0
			  u64R3R2R1R0   = _dssub2(_lo128(y_149), cb10_50);
			  u64R3R2R1R0   = _dssub2(u64R3R2R1R0, cr10_104);
			  u64R3R2R1R0   = _dsadd2(u64R3R2R1R0,0x21f821f821f821f8);
			  u64R3R2R1R0   = _dshr2(u64R3R2R1R0,6);

			  //G7-G6-G5-G4
			  u64R7R6R5R4   = _dssub2(_hi128(y_149), cb32_50);
			  u64R7R6R5R4   = _dssub2(u64R7R6R5R4, cr32_104);
			  u64R7R6R5R4   = _dsadd2(u64R7R6R5R4,0x21f821f821f821f8);
			  u64R7R6R5R4   = _dshr2(u64R7R6R5R4,6);
			  _mem8(dst_ptr_g_u8) = _dspacku4(u64R3R2R1R0,u64R7R6R5R4);
			  dst_ptr_g_u8+=8;

			  /*TODO one more cb has to be added*/
			  //B3-B2-B1-B0
			  u64R3R2R1R0   = _dsadd2(_lo128(y_149),0xbaf9baf9baf9baf9); // 0xbaf9 = -17671
			  u64R3R2R1R0   = _dsadd2(u64R3R2R1R0, cb10_129);
			  u64R3R2R1R0   = _dshr2(u64R3R2R1R0,6);

			  //B7-B6-B5-B4
			  u64R7R6R5R4   = _dsadd2(_hi128(y_149),0xbaf9baf9baf9baf9);
			  u64R7R6R5R4   = _dsadd2(u64R7R6R5R4, cb32_129);
			  u64R7R6R5R4   = _dshr2(u64R7R6R5R4,6);
			  _mem8(dst_ptr_b_u8) = _dspacku4(u64R3R2R1R0,u64R7R6R5R4);
			  dst_ptr_b_u8+=8;
			}

			dst_ptr_r_u8 += pad_pixel[0] + pad_pixel[2];
			dst_ptr_g_u8 += pad_pixel[0] + pad_pixel[2];
			dst_ptr_b_u8 += pad_pixel[0] + pad_pixel[2];
		  }

	  }
      else{
		  for(i = 0; i < in_height; i++){
			y_row_vec_ptr    = (uint64_t*)&y_vec_ptr[i*in_stride];
			crcb_row_vec_ptr = (uint64_t*)&crcb_vec_ptr[(i >> 1)*in_stride];

			for(j = 0; j < in_width; j+=8){ // achieved ii is 10. so 10/8 cycle per pixel
			  y_vec           = _mem8(y_row_vec_ptr++); //Y7Y6Y5Y4Y3Y2Y1Y0
			  crcb_vec        = _mem8(crcb_row_vec_ptr++); //Cr3Cb3Cr2Cb2-Cr1Cb1Cr0Cb0

			  y_149           = _dmpyu4(y_vec,pkd_149);      //149*Y7-149*Y6-149*Y5-149*Y4-149*Y3-149*Y2-149*Y1-149*Y0
			  cr_204_cb_50    = _dmpyu4(crcb_vec,pkd_204_50);//204*Cr3-50*Cb3-204*Cr2-50*Cb2-204*Cr1-50*Cb1-204*Cr0-50*Cb0
			  cr_104_cb_129   = _dmpyu4(crcb_vec,pkd_104_129);//104*Cr3-129*Cb3-104*Cr2-129*Cb2-104*Cr1-129*Cb1-104*Cr0-129*Cb0

			  cb10_50         = _dpackl2(_lo128(cr_204_cb_50),_lo128(cr_204_cb_50)); //50*Cb1-50*Cb1-50*Cb0-50*Cb0
			  cb32_50         = _dpackl2(_hi128(cr_204_cb_50),_hi128(cr_204_cb_50)); //50*Cb3-50*Cb3-50*Cb2-50*Cb2
			  cr10_204        = _dpackh2(_lo128(cr_204_cb_50),_lo128(cr_204_cb_50)); //204*Cr1-204*Cr1-204*Cr0-204*Cr0
			  cr32_204        = _dpackh2(_hi128(cr_204_cb_50),_hi128(cr_204_cb_50)); //204*Cr3-204*Cr3-204*Cr2-204*Cr2

			  cb10_129        = _dpackl2(_lo128(cr_104_cb_129),_lo128(cr_104_cb_129)); //129*Cb1-129*Cb1-129*Cb0-129*Cb0
			  cb32_129        = _dpackl2(_hi128(cr_104_cb_129),_hi128(cr_104_cb_129)); //129*Cb3-129*Cb3-129*Cb2-129*Cb2
			  cr10_104        = _dpackh2(_lo128(cr_104_cb_129),_lo128(cr_104_cb_129)); //104*Cr1-104*Cr1-104*Cr0-104*Cr0
			  cr32_104        = _dpackh2(_hi128(cr_104_cb_129),_hi128(cr_104_cb_129)); //104*Cr3-104*Cr3-104*Cr2-104*Cr2

			  //R3-R2-R1-R0
			  u64R3R2R1R0   = _dsadd2(_lo128(y_149),0xc858c858c858c858); // 0x90b0 = -28496
			  u64R3R2R1R0   = _dsadd2(u64R3R2R1R0, cr10_204);
			  u64R3R2R1R0   = _dshr2(u64R3R2R1R0,6);

			  //R7-R6-R5-R4
			  u64R7R6R5R4   = _dsadd2(_hi128(y_149),0xc858c858c858c858);
			  u64R7R6R5R4   = _dsadd2(u64R7R6R5R4, cr32_204);
			  u64R7R6R5R4   = _dshr2(u64R7R6R5R4,6);
			  u64temp1      = _dspacku4(u64R3R2R1R0,u64R7R6R5R4);
			  _mem8(dst_ptr_r_u16) = _unpkbu4(_loll(u64temp1));
			  dst_ptr_r_u16+=4;

			  _mem8(dst_ptr_r_u16) = _unpkbu4(_hill(u64temp1));
			  dst_ptr_r_u16+=4;

			  //G3-G2-G1-G0
			  u64R3R2R1R0   = _dssub2(_lo128(y_149), cb10_50);
			  u64R3R2R1R0   = _dssub2(u64R3R2R1R0, cr10_104);
			  u64R3R2R1R0   = _dsadd2(u64R3R2R1R0,0x21f821f821f821f8);
			  u64R3R2R1R0   = _dshr2(u64R3R2R1R0,6);

			  //G7-G6-G5-G4
			  u64R7R6R5R4   = _dssub2(_hi128(y_149), cb32_50);
			  u64R7R6R5R4   = _dssub2(u64R7R6R5R4, cr32_104);
			  u64R7R6R5R4   = _dsadd2(u64R7R6R5R4,0x21f821f821f821f8);
			  u64R7R6R5R4   = _dshr2(u64R7R6R5R4,6);
			  u64temp1      = _dspacku4(u64R3R2R1R0,u64R7R6R5R4);
			  _mem8(dst_ptr_g_u16) = _unpkbu4(_loll(u64temp1));
			  dst_ptr_g_u16+=4;

			  _mem8(dst_ptr_g_u16) = _unpkbu4(_hill(u64temp1));
			  dst_ptr_g_u16+=4;

			  /*TODO one more cb has to be added*/
			  //B3-B2-B1-B0
			  u64R3R2R1R0   = _dsadd2(_lo128(y_149),0xbaf9baf9baf9baf9); // 0xbaf9 = -17671
			  u64R3R2R1R0   = _dsadd2(u64R3R2R1R0, cb10_129);
			  u64R3R2R1R0   = _dshr2(u64R3R2R1R0,6);

			  //B7-B6-B5-B4
			  u64R7R6R5R4   = _dsadd2(_hi128(y_149),0xbaf9baf9baf9baf9);
			  u64R7R6R5R4   = _dsadd2(u64R7R6R5R4, cb32_129);
			  u64R7R6R5R4   = _dshr2(u64R7R6R5R4,6);
			  u64temp1      = _dspacku4(u64R3R2R1R0,u64R7R6R5R4);

			  _mem8(dst_ptr_b_u16) = _unpkbu4(_loll(u64temp1));
			  dst_ptr_b_u16+=4;

			  _mem8(dst_ptr_b_u16) = _unpkbu4(_hill(u64temp1));
			  dst_ptr_b_u16+=4;

			}

			dst_ptr_r_u16 += pad_pixel[0] + pad_pixel[2];
			dst_ptr_g_u16 += pad_pixel[0] + pad_pixel[2];
			dst_ptr_b_u16 += pad_pixel[0] + pad_pixel[2];
		  }

	  }
    }

    if((color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB) || (color_conv_type == TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR)){
      uint64_t u64temp1,u64temp2,u64temp3;
	  if(data_type == TIADALG_DATA_TYPE_U08){
		  for(i = 0; i < in_height; i++){
			  y_row_vec_ptr = (uint64_t*)&y_vec_ptr[i*in_stride];
			for(j = 0; j < in_width; j+=8){ // achieved ii is 15, which is 15/8 cycle per pixel
			  u64temp1           = _mem8(y_row_vec_ptr++); //G2R2B1G1-R1B0G0R0
			  u64temp2           = _mem8(y_row_vec_ptr++); //R5B4G4R4-B3G3R3B2
			  u64temp3           = _mem8(y_row_vec_ptr++); //B7G7R7B6-G6R6B5G5

			  *dst_ptr_r_u8++ = (uint8_t)_loll(u64temp1);
			  *dst_ptr_r_u8++ = (uint8_t)(_loll(u64temp1) >> 24);
			  *dst_ptr_r_u8++ = (uint8_t)(_hill(u64temp1) >> 16);
			  *dst_ptr_r_u8++ = (uint8_t)(_loll(u64temp2) >> 8);
			  *dst_ptr_r_u8++ = (uint8_t)(_hill(u64temp2) );
			  *dst_ptr_r_u8++ = (uint8_t)(_hill(u64temp2) >> 24);
			  *dst_ptr_r_u8++ = (uint8_t)(_loll(u64temp3) >> 16);
			  *dst_ptr_r_u8++ = (uint8_t)(_hill(u64temp3) >> 8);


			  *dst_ptr_g_u8++ = (uint8_t)(_loll(u64temp1) >> 8);
			  *dst_ptr_g_u8++ = (uint8_t)(_hill(u64temp1) );
			  *dst_ptr_g_u8++ = (uint8_t)(_hill(u64temp1) >> 24);
			  *dst_ptr_g_u8++ = (uint8_t)(_loll(u64temp2) >> 16);
			  *dst_ptr_g_u8++ = (uint8_t)(_hill(u64temp2) >> 8);
			  *dst_ptr_g_u8++ = (uint8_t)(_loll(u64temp3) );
			  *dst_ptr_g_u8++ = (uint8_t)(_loll(u64temp3) >> 24);
			  *dst_ptr_g_u8++ = (uint8_t)(_hill(u64temp3) >> 16);

			  *dst_ptr_b_u8++ = (uint8_t)(_loll(u64temp1) >> 16);
			  *dst_ptr_b_u8++ = (uint8_t)(_hill(u64temp1) >> 8);
			  *dst_ptr_b_u8++ = (uint8_t)(_loll(u64temp2) );
			  *dst_ptr_b_u8++ = (uint8_t)(_loll(u64temp2) >> 24);
			  *dst_ptr_b_u8++ = (uint8_t)(_hill(u64temp2) >> 16);
			  *dst_ptr_b_u8++ = (uint8_t)(_loll(u64temp3) >> 8);
			  *dst_ptr_b_u8++ = (uint8_t)(_hill(u64temp3) );
			  *dst_ptr_b_u8++ = (uint8_t)(_hill(u64temp3) >> 24);

			}
			dst_ptr_r_u8 += pad_pixel[0] + pad_pixel[2];
			dst_ptr_g_u8 += pad_pixel[0] + pad_pixel[2];
			dst_ptr_b_u8 += pad_pixel[0] + pad_pixel[2];
		  }
	  }
	  else{
		  for(i = 0; i < in_height; i++){
			  y_row_vec_ptr = (uint64_t*)&y_vec_ptr[i*in_stride];
			for(j = 0; j < in_width; j+=8){ // achieved ii is 15, which is 15/8 cycle per pixel
			  u64temp1           = _mem8(y_row_vec_ptr++); //G2R2B1G1-R1B0G0R0
			  u64temp2           = _mem8(y_row_vec_ptr++); //R5B4G4R4-B3G3R3B2
			  u64temp3           = _mem8(y_row_vec_ptr++); //B7G7R7B6-G6R6B5G5

			  *dst_ptr_r_u16++ = (uint16_t)((uint8_t)_loll(u64temp1));
			  *dst_ptr_r_u16++ = (uint16_t)((uint8_t)(_loll(u64temp1) >> 24));
			  *dst_ptr_r_u16++ = (uint16_t)((uint8_t)(_hill(u64temp1) >> 16));
			  *dst_ptr_r_u16++ = (uint16_t)((uint8_t)(_loll(u64temp2) >> 8));
			  *dst_ptr_r_u16++ = (uint16_t)((uint8_t)(_hill(u64temp2) ));
			  *dst_ptr_r_u16++ = (uint16_t)((uint8_t)(_hill(u64temp2) >> 24));
			  *dst_ptr_r_u16++ = (uint16_t)((uint8_t)(_loll(u64temp3) >> 16));
			  *dst_ptr_r_u16++ = (uint16_t)((uint8_t)(_hill(u64temp3) >> 8));


			  *dst_ptr_g_u16++ = (uint16_t)((uint8_t)(_loll(u64temp1) >> 8));
			  *dst_ptr_g_u16++ = (uint16_t)((uint8_t)(_hill(u64temp1) ));
			  *dst_ptr_g_u16++ = (uint16_t)((uint8_t)(_hill(u64temp1) >> 24));
			  *dst_ptr_g_u16++ = (uint16_t)((uint8_t)(_loll(u64temp2) >> 16));
			  *dst_ptr_g_u16++ = (uint16_t)((uint8_t)(_hill(u64temp2) >> 8));
			  *dst_ptr_g_u16++ = (uint16_t)((uint8_t)(_loll(u64temp3) ));
			  *dst_ptr_g_u16++ = (uint16_t)((uint8_t)(_loll(u64temp3) >> 24));
			  *dst_ptr_g_u16++ = (uint16_t)((uint8_t)(_hill(u64temp3) >> 16));

			  *dst_ptr_b_u16++ = (uint16_t)((uint8_t)(_loll(u64temp1) >> 16));
			  *dst_ptr_b_u16++ = (uint16_t)((uint8_t)(_hill(u64temp1) >> 8));
			  *dst_ptr_b_u16++ = (uint16_t)((uint8_t)(_loll(u64temp2) ));
			  *dst_ptr_b_u16++ = (uint16_t)((uint8_t)(_loll(u64temp2) >> 24));
			  *dst_ptr_b_u16++ = (uint16_t)((uint8_t)(_hill(u64temp2) >> 16));
			  *dst_ptr_b_u16++ = (uint16_t)((uint8_t)(_loll(u64temp3) >> 8));
			  *dst_ptr_b_u16++ = (uint16_t)((uint8_t)(_hill(u64temp3) ));
			  *dst_ptr_b_u16++ = (uint16_t)((uint8_t)(_hill(u64temp3) >> 24));

			}
			dst_ptr_r_u16 += pad_pixel[0] + pad_pixel[2];
			dst_ptr_g_u16 += pad_pixel[0] + pad_pixel[2];
			dst_ptr_b_u16 += pad_pixel[0] + pad_pixel[2];
		  }
	  }
    }
  }

#ifdef INNER_LOOP_PROFILE
  t3 = _TSC_read();
  printf("Total Cycle consumed forC66x inner loop code is %lld \n", (t3-t2));
#endif

  return(ret_val);
}
