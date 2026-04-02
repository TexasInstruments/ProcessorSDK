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
#include <tiadalg_interface.h>
#include <tiadalg_image_recursive_nms_test.h>
#include <string.h>
#include <../common/profile.h>

int32_t main(){
  int32_t num_test_cases = sizeof(test_cases)/sizeof(tiadalg_image_recursive_nms_testParams_t);

  for (int32_t tc = 0; tc < num_test_cases; tc++){
    tiadalg_image_recursive_nms_testParams_t cur_test_case = test_cases[tc];
    int8_t *in_img;

    int32_t is_pass = 1;
    int32_t opt_flow = 0;
    int32_t ret_val_c66 = TIADALG_PROCESS_SUCCESS;
    int32_t ret_val = TIADALG_PROCESS_SUCCESS;

    int32_t width  = cur_test_case.img_width;
    int32_t height = cur_test_case.img_height;

    int32_t *out_pts     = (int32_t*)malloc(width*height*sizeof(int32_t)*2); // assuming complete image as key points
    int32_t *out_pts_c66 = (int32_t*)malloc(width*height*sizeof(int32_t)*2);
    int8_t* scratch      = (int8_t*)malloc(width*sizeof(int16_t));

    printf("TestCase # %d \n",tc);

    if((out_pts == NULL) || (out_pts_c66 == NULL)){
      printf("memory could not be allocated\n");
      exit(0);
    }

    memset(out_pts,0,width*height*sizeof(int32_t)*2);
    memset(out_pts_c66,0,width*height*sizeof(int32_t)*2);

    if(cur_test_case.in_data_pattern == 0x1){
      in_img = (int8_t*)malloc(cur_test_case.img_width*cur_test_case.img_height);

      if(in_img == NULL)
        printf("memory could not be allocated\n");

      for(int32_t i = 0; i < cur_test_case.img_width*cur_test_case.img_height; i++){
        in_img[i] = (int8_t)i&0xFF;
      }
    }else{
      in_img = cur_test_case.in_img;
    }

#ifndef HOST_EMULATION
    tiadalg_init_cache();
    long long t0, t1;
    _TSC_enable();
    t0 = _TSC_read();
#endif

    ret_val = tiadalg_image_recursive_nms_cn(in_img, width, width, height,
                                                  cur_test_case.score_th, 2000,
                                                  32, scratch,
                                                  cur_test_case.in_data_type,
                                                  out_pts);
#ifndef HOST_EMULATION
    t1 = _TSC_read();

    printf("Total Cycle consumed for Natural C code is %lld \n", (t1-t0));
#endif



#ifndef HOST_EMULATION
    t0 = _TSC_read();
#endif

    ret_val_c66 = tiadalg_image_recursive_nms_c66(in_img, width, width, height,
                                                  cur_test_case.score_th, 2000,
                                                  32, scratch,
                                                  cur_test_case.in_data_type,
                                                  out_pts_c66);

#ifndef HOST_EMULATION
    t1 = _TSC_read();
    printf("Total Cycle consumed for Optimized c66x code is %lld \n", (t1-t0));
#endif

    if (ret_val == ret_val_c66){
      for(int32_t i = 0 ; i < 2*ret_val; i++){
        if(out_pts_c66[i] != out_pts[i]){
          is_pass = 0;
          break;
        }
      }
    }
    else{
      is_pass = 0;
    }

    free(in_img);
    free(scratch);
    free(out_pts);
    free(out_pts_c66);

    if (is_pass == 1){
      printf("Test case has passed \n");
    }
    else{
      printf("Test case has failed \n");
    }

  }
  return(0);
}
