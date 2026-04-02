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
#include <tiadalg_image_preprocessing_test.h>
#include <string.h>
#include <../common/profile.h>

int32_t main(){
  int32_t num_test_cases = sizeof(test_cases)/sizeof(tiadalg_image_preprocessing_testParams_t);
  int32_t tc;
  for (tc = 0; tc < num_test_cases; tc++){
    tiadalg_image_preprocessing_testParams_t cur_test_case = test_cases[tc];
    uint8_t *in_img;
    uint8_t *in_img_ptrs[2];

    int32_t is_pass = 1;
    int32_t pad_pixels[4] = {2, 1, 2, 2};
    int32_t opt_flow = 0;
    int32_t ret_val_c66 = TIADALG_PROCESS_SUCCESS;
    int32_t ret_val = TIADALG_PROCESS_SUCCESS;

    int32_t padded_width = cur_test_case.img_width + pad_pixels[0] + pad_pixels[2];
    int32_t padded_height = cur_test_case.img_height + pad_pixels[1] + pad_pixels[3];

    int32_t out_img_data_depth = (cur_test_case.out_data_type == TIADALG_DATA_TYPE_U16) ? 2 : 1;

    uint8_t *out_img = (uint8_t*)malloc(padded_width*padded_height*cur_test_case.num_planes*out_img_data_depth);
    uint8_t *out_img_c66 = (uint8_t*)malloc(padded_width*padded_height*cur_test_case.num_planes*out_img_data_depth);

    printf("TestCase # %d \n",tc);

    if((out_img == NULL) || (out_img_c66 == NULL)){
      printf("memory could not be allocated\n");
      exit(0);
    }

    if(cur_test_case.out_data_type == TIADALG_DATA_TYPE_U08)
    {
      memset(out_img,0,padded_width*padded_height*cur_test_case.num_planes);
      memset(out_img_c66,0,padded_width*padded_height*cur_test_case.num_planes);
    }
    else if(cur_test_case.out_data_type == TIADALG_DATA_TYPE_U16)
    {
      memset(out_img,0,padded_width*padded_height*cur_test_case.num_planes*2);
      memset(out_img_c66,0,padded_width*padded_height*cur_test_case.num_planes*2);
    }
    else
    {
      printf("Not supported data type \n");
    }

    if(cur_test_case.in_data_type == 0x1){
      in_img = (uint8_t*)malloc(cur_test_case.img_width*cur_test_case.img_height*cur_test_case.num_planes);

      if(in_img == NULL)
        printf("memory could not be allocated\n");

      int32_t i;

      for(i =0; i < cur_test_case.img_width*cur_test_case.img_height*cur_test_case.num_planes; i++){
        in_img[i] = (uint8_t)i&0xFF;
      }
    }else{
      in_img = cur_test_case.in_img;
    }

    in_img_ptrs[0] = in_img;

    /* For YUV input second pointer is not expected to adjacent to the end of luma plane.
       For some of the input image format, two pointers are not needed in those scenarios but to
       have single interface still the lost of pointers is passed, and in those scenario second pointer
       (in_img_ptrs[1]) will not be used.
    */

    in_img_ptrs[1] = in_img + cur_test_case.img_width*cur_test_case.img_height;

#ifndef HOST_EMULATION
    tiadalg_init_cache();
    long long t0, t1;
    _TSC_enable();
    t0 = _TSC_read();
#endif

    ret_val = tiadalg_image_preprocessing_cn((void**)in_img_ptrs,
                                                     cur_test_case.img_width,
                                                     cur_test_case.img_height,
                                                     cur_test_case.img_width,
                                                     cur_test_case.out_data_type,
                                                     cur_test_case.color_conv_type,
                                                     cur_test_case.scale_val,
                                                     cur_test_case.mean_pixel,
                                                     pad_pixels,
                                                     (void*)out_img
                                                     );
#ifndef HOST_EMULATION
#ifdef _TMS320C6600
    t1 = _TSC_read();

    printf("Total Cycle consumed for Natural C code is %lld \n", (t1-t0));
#endif
#endif

    if((cur_test_case.scale_val[0] == 1.0f)&& (cur_test_case.scale_val[1] == 1.0f)&& (cur_test_case.scale_val[2] == 1.0f)&&
      (cur_test_case.mean_pixel[0] == 0.0f)&& (cur_test_case.mean_pixel[1] == 0.0f)&& (cur_test_case.mean_pixel[2] == 0.0f)&&
      ((cur_test_case.img_width & 0x7) == 0x0) &&
      ((cur_test_case.out_data_type == TIADALG_DATA_TYPE_U08) || (cur_test_case.out_data_type == TIADALG_DATA_TYPE_U16)) &&
      ((cur_test_case.color_conv_type != TIADALG_COLOR_CONV_YUV420_RGB)|| (cur_test_case.color_conv_type != TIADALG_COLOR_CONV_YUV420_BGR))
    ){
      opt_flow = 1;
    }

    if(opt_flow != 0x0){

#ifndef HOST_EMULATION
#ifdef _TMS320C6600
      t0 = _TSC_read();
#endif
#endif
       ret_val_c66 = tiadalg_image_preprocessing_c66((void**)in_img_ptrs,
                                                        cur_test_case.img_width,
                                                        cur_test_case.img_height,
                                                        cur_test_case.img_width,
                                                        cur_test_case.out_data_type,
                                                        cur_test_case.color_conv_type,
                                                        cur_test_case.scale_val,
                                                        cur_test_case.mean_pixel,
                                                        pad_pixels,
                                                        (void*)out_img_c66
                                                        );
#ifndef HOST_EMULATION
#ifdef _TMS320C6600
      t1 = _TSC_read();
      printf("Total Cycle consumed for Optimized c66x code is %lld \n", (t1-t0));
#endif
#endif

    }

    if ((ret_val == TIADALG_PROCESS_SUCCESS) && (ret_val_c66 == TIADALG_PROCESS_SUCCESS)){
      uint8_t* out_ptr;
      uint8_t* in_ptr;
      int32_t n;

      for(n = 0; n < 2; n++){
        if(n == 0){
          if(cur_test_case.ref_out_img != NULL){
            in_ptr  = cur_test_case.ref_out_img;
            out_ptr = out_img;
            printf("Comapring Natural C with Ref output \n");
          }else{
            out_ptr = NULL;
          }

        }else{
          if(opt_flow != 0x0){
            in_ptr  = out_img;
            out_ptr = out_img_c66;
            printf("Comapring Natural C with C66 output \n");
          }else{
            out_ptr = NULL;
          }
        }
        if((in_ptr != NULL) && (out_ptr != NULL)){
         if(cur_test_case.out_data_type == TIADALG_DATA_TYPE_U08){
          int32_t i;
          for (i = 0 ; i < padded_width*padded_height*cur_test_case.num_planes; i++){
            if (abs(in_ptr[i] - out_ptr[i])>4){
              is_pass = 0;
              printf("Mismatch happened at ");
              printf("%d, Ref : %d, Out: %d \n", i, in_ptr[i], out_ptr[i]);
              break;
            }
          }
         }
         else if(cur_test_case.out_data_type == TIADALG_DATA_TYPE_U16)
         {
          int32_t i;
          for (i = 0 ; i < padded_width*padded_height*cur_test_case.num_planes; i++){
            if (abs(((uint16_t*)in_ptr)[i] - ((uint16_t*)out_ptr)[i])>4){
              is_pass = 0;
              printf("Mismatch happened at ");
              printf("%d, Ref : %d, Out: %d \n", i, ((uint16_t*)in_ptr)[i], ((uint16_t*)out_ptr)[i]);
              break;
            }
          }
         }
        }
      }
    }
    else{
      is_pass = 0;
    }
    if(cur_test_case.in_img == NULL)
      free(in_img);
    free(out_img);
    free(out_img_c66);

    if (is_pass == 1){
      printf("Test case has passed \n");
    }
    else{
      printf("Test case has failed \n");
    }

  }
  return(0);
}
