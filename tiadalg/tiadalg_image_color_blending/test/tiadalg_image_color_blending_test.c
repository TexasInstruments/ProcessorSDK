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
#include <tiadalg_image_color_blending_test.h>
#include <string.h>
#include <../common/profile.h>

int32_t main(){

  int32_t num_test_cases = sizeof(test_cases)/sizeof(tiadalg_image_preprocessing_testParams_t);

  for (int32_t tc = 0; tc < num_test_cases; tc++){
    tiadalg_image_preprocessing_testParams_t cur_test_case = test_cases[tc];

    int32_t is_pass = 1;
    int32_t ret_val_c66 = TIADALG_PROCESS_SUCCESS;
    int32_t ret_val = TIADALG_PROCESS_SUCCESS;
    uint8_t *in_img[3];
    uint8_t *out_img[3];
    uint8_t *out_img_c66[3];
    uint8_t *det_plane;
    int32_t valid_region[4];

    in_img[0] = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);
    in_img[1] = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);
    in_img[2] = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);
    det_plane = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);

    out_img[0] = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);
    out_img[1] = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);
    out_img[2] = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);

    out_img_c66[0] = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);
    out_img_c66[1] = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);
    out_img_c66[2] = (uint8_t*)malloc(cur_test_case.img_pitch*cur_test_case.img_height);

    printf("TestCase # %d \n",tc);
    valid_region[0] = 0;valid_region[1] = 0;
    valid_region[2] = cur_test_case.img_width - 1;
    valid_region[3] = cur_test_case.img_height - 1;

    if((in_img[0] == NULL) || (in_img[1] == NULL) || (in_img[2] == NULL) ||
       (out_img[0] == NULL) || (out_img[1] == NULL) || (out_img[2] == NULL) ||
       (out_img_c66[0] == NULL) || (out_img_c66[1] == NULL) || (out_img_c66[2] == NULL)
    ){
      printf("memory could not be allocated\n");
      exit(0);
    }

    memset(out_img[0],0x0, cur_test_case.img_pitch*cur_test_case.img_height);
    memset(out_img[1],0x0, cur_test_case.img_pitch*cur_test_case.img_height);
    memset(out_img[2],0x0, cur_test_case.img_pitch*cur_test_case.img_height);
    memset(out_img_c66[0],0x0, cur_test_case.img_pitch*cur_test_case.img_height);
    memset(out_img_c66[1],0x0, cur_test_case.img_pitch*cur_test_case.img_height);
    memset(out_img_c66[2],0x0, cur_test_case.img_pitch*cur_test_case.img_height);
    memset(det_plane,0x0, cur_test_case.img_pitch*cur_test_case.img_height);

    /*Creating a represntative dettection of patches 128x128*/

    for(int32_t i= 0; i<30;i++){
      memset(&det_plane[i*cur_test_case.img_pitch],0x1, 30);
      memset(&det_plane[(i+32)*cur_test_case.img_pitch],0x2, 30);
      memset(&det_plane[i*cur_test_case.img_pitch + 32],0x3, 30);
    }

    if(cur_test_case.in_data_type == 0x1){
      tiadalg_fill_buffer_random(in_img[0],cur_test_case.img_pitch*cur_test_case.img_height);
      tiadalg_fill_buffer_random(in_img[1],cur_test_case.img_pitch*cur_test_case.img_height);
      tiadalg_fill_buffer_random(in_img[2],cur_test_case.img_pitch*cur_test_case.img_height);
    }else{
      in_img[0] = cur_test_case.in_img[0];
      in_img[1] = cur_test_case.in_img[1];
      in_img[2] = cur_test_case.in_img[2];
    }

#ifndef HOST_EMULATION
    tiadalg_init_cache();
    long long t0, t1;
    _TSC_enable();
    t0 = _TSC_read();
#endif

    ret_val = tiadalg_image_color_blending_cn(in_img,
                                               cur_test_case.img_width,
                                               cur_test_case.img_width,
                                               cur_test_case.img_height,
                                               2,0,0,
                                               valid_region,
                                               det_plane,
                                               cur_test_case.img_width,
                                               1.0,
                                               cur_test_case.det_color_map_uv,
                                               out_img,cur_test_case.img_width
                                               );
#ifndef HOST_EMULATION
#ifdef _TMS320C6600
    t1 = _TSC_read();

    printf("Total Cycle consumed for Natural C code is %lld \n", (t1-t0));
#endif
#endif



#ifndef HOST_EMULATION
#ifdef _TMS320C6600
      t0 = _TSC_read();
#endif
#endif
       ret_val_c66 = tiadalg_image_color_blending_c66(in_img,
                                               cur_test_case.img_width,
                                               cur_test_case.img_width,
                                               cur_test_case.img_height,
                                               2,0,0,
                                               valid_region,
                                               det_plane,
                                               cur_test_case.img_width,
                                               1.0,
                                               cur_test_case.det_color_map_uv,
                                               out_img_c66,cur_test_case.img_width
                                               );
#ifndef HOST_EMULATION
#ifdef _TMS320C6600
      t1 = _TSC_read();
      printf("Total Cycle consumed for Optimized c66x code is %lld \n", (t1-t0));
#endif
#endif


    if ((ret_val == TIADALG_PROCESS_SUCCESS) && (ret_val_c66 == TIADALG_PROCESS_SUCCESS)){
      printf("Comparing natural C code output with optimized code output\n");
      for(int32_t i=0; i < cur_test_case.img_pitch*cur_test_case.img_height;i++){
        /*First plane comaparision*/
        if(out_img_c66[0][i] != out_img[0][i]){
          printf("First plane mismatch has happened for %d th element out_img_c66[i]=%d, out_img[i]=%d \n",i,out_img_c66[0][i],out_img[0][i]);
          is_pass = 0;
          break;
        }
        /*Second plane comaparision*/
        if(out_img_c66[1][i] != out_img[1][i]){
          printf("Second plane mismatch has happened for %d th element out_img_c66[i]=%d, out_img[i]=%d \n",i,out_img_c66[1][i],out_img[1][i]);
          is_pass = 0;
          break;
        }
      }
    }
    else{
      is_pass = 0;
    }

    free(in_img[0]);
    free(in_img[1]);
    free(in_img[2]);
    free(det_plane);

    free(out_img[0]);
    free(out_img[1]);
    free(out_img[2]);

    free(out_img_c66[0]);
    free(out_img_c66[1]);
    free(out_img_c66[2]);


    if (is_pass == 1){
      printf("Test case has passed \n");
    }
    else{
      printf("Test case has failed \n");
    }

  }
  return(0);
}
