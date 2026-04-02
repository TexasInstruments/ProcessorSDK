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

#include <../common/tiadalg_test_interface.h>
#include "tiadalg_interface.h"
#include "tiadalg_sparse_upsampling_test.h"
#include <./../common/tiadalg_test_interface.h>
#include "stdio.h"
#include "string.h"
#include "math.h"
#include <../common/profile.h>
//#define SCORE_IMG_BUF

int32_t main(){
  int32_t num_test_cases = sizeof(test_cases)/sizeof(tiadalg_sparse_upsampling_testParams_t);

  for (int32_t tc = 0; tc < num_test_cases; tc++){
    tiadalg_sparse_upsampling_testParams_t cur_test_case = test_cases[tc];

    int8_t * score_buf                 = (int8_t *) malloc(cur_test_case.score_width*cur_test_case.score_height);
#ifdef SCORE_IMG_BUF
    uint8_t * img_buf                  = (uint8_t *) malloc(768*384);
    memset(img_buf,0x0,768*384);
#endif
    /*currently in test application desc binary file is having one extra plane of data, which needs to be ignored before passing to API*/
    int8_t  * desc_buf                 = (int8_t *) malloc(cur_test_case.desc_plane_width*cur_test_case.desc_plane_height*(cur_test_case.num_desc_planes+1));
    int8_t*   filter_coeff             = (int8_t *) malloc(7*7*(cur_test_case.num_desc_planes+1));
    int16_t*  bias                     = (int16_t *) malloc((cur_test_case.num_desc_planes+1)*sizeof(int16_t));

    int32_t* key_point_list            = (int32_t *) malloc(cur_test_case.max_key_points*2*sizeof(int32_t));
    uint16_t*  out_desc_buf_cn         = (uint16_t *) malloc(cur_test_case.max_key_points*cur_test_case.num_desc_planes*sizeof(int16_t));
    uint16_t*  out_desc_buf_c66        = (uint16_t *) malloc(cur_test_case.max_key_points*cur_test_case.num_desc_planes*sizeof(int16_t));

    /*in ref desc file, along with float descriptor, 3 otehr parameters (x,y,score) are present*/
    float*  ref_desc_buf               = (float *) malloc(cur_test_case.max_key_points*(cur_test_case.num_desc_planes + 3)*sizeof(float));

    int16_t* scratch_buffer            = (int16_t*)malloc(16*12*sizeof(int16_t)*cur_test_case.num_desc_planes);

    int32_t is_pass = 1;
    int32_t ret_val_c66 = TIADALG_PROCESS_SUCCESS;
    int32_t ret_val_cn = TIADALG_PROCESS_SUCCESS;
    int32_t num_key_points = 0;
    int32_t num_ref_key_points = 0;
    int32_t desc_out_data_type = 0x1;
    int32_t desc_offset =0;

    if(desc_out_data_type == 0){
      desc_offset = 128;
    }else{
      desc_offset = 32768;
    }

    printf("TestCase # %d \n",tc);

    /*Read input packed dense score buffer data*/
    tiadalg_read_bin_file(score_buf,
                          cur_test_case.score_width*cur_test_case.score_height,
                          cur_test_case.in_score_file);

    if(cur_test_case.in_desc_file != 0x0){
      tiadalg_read_bin_file(desc_buf,
                            cur_test_case.desc_plane_width*cur_test_case.desc_plane_height*(cur_test_case.num_desc_planes + 1),
                            cur_test_case.in_desc_file);
    }else{
      tiadalg_fill_buffer_random((uint8_t*)desc_buf,cur_test_case.desc_plane_width*cur_test_case.desc_plane_height*(cur_test_case.num_desc_planes + 1));
    }


    tiadalg_read_bin_file(filter_coeff,
                          7*7*(cur_test_case.num_desc_planes+1),
                          cur_test_case.in_filter_coeff_file);

    tiadalg_read_bin_file(bias,
                          (cur_test_case.num_desc_planes+1)*sizeof(int16_t),
                          cur_test_case.in_filter_bis_file);

    if(cur_test_case.ref_desc_file != 0x0){
      num_ref_key_points =
      tiadalg_read_f32_data_from_file(ref_desc_buf,
                                      cur_test_case.max_key_points*(cur_test_case.num_desc_planes+3),
                                      cur_test_case.ref_desc_file);

      num_ref_key_points = num_ref_key_points/(cur_test_case.num_desc_planes+3);

      /*temprorialy scaling the key points to 768x384 resolution, from 1280x720 resolution*/
      for(int32_t i = 0; i < num_ref_key_points; i++){
        ref_desc_buf[(cur_test_case.num_desc_planes+3)*i + 0] *= (768.0/1280.0);
        ref_desc_buf[(cur_test_case.num_desc_planes+3)*i + 1] *= (384.0/720.0);
      }
    }

    /*Create sparse list from dense score by basic thresholding and 3x3 NMS*/
    int32_t guard_pixels = 32;
    int32_t shift_amt = -cur_test_case.out_desc_scale;
    int32_t rnd = 1<<(shift_amt-1);
    int32_t cur_score_th = (cur_test_case.score_th+rnd) >> shift_amt;

    //zero out guard band region
    for(int32_t i = 0; i < cur_test_case.score_height; i++){
      for(int32_t j = 0; j < cur_test_case.score_width; j++){
        if((i < guard_pixels) || (i >= (cur_test_case.score_height-guard_pixels))){
          score_buf[i* cur_test_case.score_width + j] = 0;
        }

        if((j < guard_pixels) || (j >= (cur_test_case.score_width-guard_pixels))){
          score_buf[i* cur_test_case.score_width + j] = 0;
        }
      }
    }
#if 0
    for(int32_t i = 0; i < cur_test_case.score_height; i++){
      for(int32_t j = 0; j < cur_test_case.score_width; j++){
        int32_t cur_score = score_buf[i* cur_test_case.score_width + j];
        if((cur_score <= cur_score_th)){
          score_buf[i* cur_test_case.score_width + j] = 0;
        }
      }
    }
#endif
    for(int32_t j = guard_pixels; j < cur_test_case.score_width - guard_pixels; j++){
    for(int32_t i = guard_pixels; i < cur_test_case.score_height - guard_pixels; i++){
        int32_t cur_score = score_buf[i* cur_test_case.score_width + j];
        if((cur_score > cur_score_th) &&(cur_score > score_buf[i* cur_test_case.score_width + j -1]) &&
           (cur_score > score_buf[i* cur_test_case.score_width + j + 1]) && (cur_score > score_buf[(i-1)* cur_test_case.score_width + j - 1]) &&
           (cur_score > score_buf[(i-1)* cur_test_case.score_width + j]) && (cur_score > score_buf[(i-1)* cur_test_case.score_width + j + 1]) &&
           (cur_score > score_buf[(i+1)* cur_test_case.score_width + j - 1]) && (cur_score > score_buf[(i+1)* cur_test_case.score_width + j]) &&
           (cur_score > score_buf[(i+1)* cur_test_case.score_width + j + 1])
          ){
            key_point_list[2*num_key_points + 0] = j;
            key_point_list[2*num_key_points + 1] = i;
            num_key_points++;
#ifdef SCORE_IMG_BUF
            img_buf[768 * i + j] = cur_score << shift_amt;
#endif
            if(num_key_points >= cur_test_case.max_key_points)
              break;
        }
        else{
          score_buf[i* cur_test_case.score_width + j] = 0;
        }
      }
    }
    printf("total number of keypoints are %d \n",num_key_points);
#ifndef HOST_EMULATION
    tiadalg_init_cache();
    long long t0, t1;
    _TSC_enable();
    t0 = _TSC_read();
#endif

#ifdef SCORE_IMG_BUF
    tiadalg_save_bmp_image(768,384,1,img_buf,"nms_output.bmp");
#endif

    /*Execute natural C algorithm*/
     ret_val_cn = tiadalg_sparse_upsampling_cn(desc_buf + cur_test_case.desc_plane_width*cur_test_case.desc_plane_height,
                                                cur_test_case.desc_plane_width,
                                                cur_test_case.desc_plane_width*cur_test_case.desc_plane_height,
                                                cur_test_case.num_desc_planes, /*descriptor size in terms of int16_t*/
                                                filter_coeff + 7*7,
                                                bias + 1,
                                                key_point_list,
                                                num_key_points,
                                                0x0,
                                                0x0,
                                                cur_test_case.in_desc_scale_pw2,
                                                cur_test_case.in_filter_scale_pw2,
                                                cur_test_case.in_bias_scale_pw2,
                                                cur_test_case.out_desc_scale,
                                                out_desc_buf_cn,
                                                TIADALG_DATA_TYPE_S08,
                                                TIADALG_DATA_TYPE_U08);

#ifndef HOST_EMULATION
    t1 = _TSC_read();

    printf("Total Cycle consumed for Natural C code is %lld \n", (t1-t0));
#endif

    if(ret_val_cn != TIADALG_PROCESS_SUCCESS){
      is_pass = 0;
    }

    /*Calling first time for just filling the scratch buffer*/
    ret_val_c66 = tiadalg_sparse_upsampling_c66(desc_buf+ cur_test_case.desc_plane_width*cur_test_case.desc_plane_height,
                                                  cur_test_case.desc_plane_width,
                                                  cur_test_case.desc_plane_width*cur_test_case.desc_plane_height,
                                                  cur_test_case.num_desc_planes, /*descriptor size in terms of int16_t*/
                                                  filter_coeff+ 7*7,
                                                  bias + 1,
                                                  key_point_list,
                                                  num_key_points,
                                                  scratch_buffer,
                                                  0x0,
                                                  cur_test_case.in_desc_scale_pw2,
                                                  cur_test_case.in_filter_scale_pw2,
                                                  cur_test_case.in_bias_scale_pw2,
                                                  cur_test_case.out_desc_scale,
                                                  out_desc_buf_c66,
                                                  TIADALG_DATA_TYPE_S08,
                                                  TIADALG_DATA_TYPE_U08);

#ifndef HOST_EMULATION
      t0 = _TSC_read();
#endif

     ret_val_c66 = tiadalg_sparse_upsampling_c66(desc_buf+ cur_test_case.desc_plane_width*cur_test_case.desc_plane_height,
                                                  cur_test_case.desc_plane_width,
                                                  cur_test_case.desc_plane_width*cur_test_case.desc_plane_height,
                                                  cur_test_case.num_desc_planes, /*descriptor size in terms of int16_t*/
                                                  filter_coeff + 7*7,
                                                  bias + 1,
                                                  key_point_list,
                                                  num_key_points,
                                                  scratch_buffer,
                                                  0x1,
                                                  cur_test_case.in_desc_scale_pw2,
                                                  cur_test_case.in_filter_scale_pw2,
                                                  cur_test_case.in_bias_scale_pw2,
                                                  cur_test_case.out_desc_scale,
                                                  out_desc_buf_c66,
                                                  TIADALG_DATA_TYPE_S08,
                                                  TIADALG_DATA_TYPE_U08);

#ifndef HOST_EMULATION
      t1 = _TSC_read();
      printf("Total Cycle consumed for Optimized c66x code is %lld \n", (t1-t0));
#endif

    if(ret_val_c66 != TIADALG_PROCESS_SUCCESS){
      is_pass = 0;
    }

    if ((ret_val_c66 == TIADALG_PROCESS_SUCCESS) && (ret_val_cn == TIADALG_PROCESS_SUCCESS)){
        printf("comparing reference and natural c \n");

        float mismatch_th = 0.1f;

        for(int32_t i = 0; i < num_key_points; i++){
          if(cur_test_case.ref_desc_file != 0x0){
            /*If the reference file is provided*/
            float cur_x = key_point_list[2*i + 0];
            float cur_y = key_point_list[2*i + 1];
            for(int32_t j = 0; j < num_ref_key_points; j++){
              if((abs(ref_desc_buf[(cur_test_case.num_desc_planes+3)*j + 0] - cur_x) < mismatch_th) &&
                  (abs(ref_desc_buf[(cur_test_case.num_desc_planes+3)*j + 1] - cur_y) < mismatch_th)){
                    /*key point matched*/
                    //printf("Key point matched at location %f, %f", cur_x, cur_y);
                    if(cur_test_case.ref_desc_file != 0x0)
                    {
                      for(int32_t k=0; k < cur_test_case.num_desc_planes; k++){
                        if(((int)(ref_desc_buf[j*(cur_test_case.num_desc_planes+3) + 3 + k] + desc_offset) ) !=
                                (out_desc_buf_cn[i*cur_test_case.num_desc_planes + k])){
                                  printf("descriptor mismatch with refrence\n");
                                  break;
                                }
                      }
                    }
                    //printf(" Descriptor also matched \n");
                  }
            }
          }//if(cur_test_case.ref_desc_file != 0x0)
          for(int32_t k=0; k < cur_test_case.num_desc_planes; k++){
            if(((int)(out_desc_buf_c66[i*cur_test_case.num_desc_planes + k]) ) !=
                    (out_desc_buf_cn[i*cur_test_case.num_desc_planes + k])){
                      printf("descriptor mismatch with natural C code\n");
                      break;
                    }
          }
        }
        if(cur_test_case.ref_desc_file != 0x0)
        {
          if(abs(num_key_points - num_ref_key_points) > 0.1*num_ref_key_points){
            printf("Total number of keypoints has not matched and they are out# : %d, ref# : %d \n",num_key_points, num_ref_key_points);
            is_pass = 0;
          }
        }

    }
    else{
      is_pass = 0;
    }

    if (is_pass == 1){
      printf("Test case has passed \n");
    }
    else{
      printf("Test case has failed \n");
    }

    free(score_buf);
    free(desc_buf);
    free(key_point_list);
    free(out_desc_buf_cn);
    free(out_desc_buf_c66);
    free(ref_desc_buf);
    free(filter_coeff);
#ifdef SCORE_IMG_BUF
    free(img_buf);
#endif
    free(scratch_buffer);

  }
  return(0);
}
