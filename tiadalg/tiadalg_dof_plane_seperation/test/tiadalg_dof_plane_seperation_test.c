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

#include <./../common/tiadalg_test_interface.h>
#include "tiadalg_interface.h"
#include "tiadalg_dof_plane_seperation_test.h"
#include "stdio.h"
#include "string.h"
#include <../common/profile.h>

#define USE_INT_MEM

#ifdef USE_INT_MEM

#define L2_SCRATCH_SIZE (128*1024)

#pragma DATA_SECTION (".dmem1")
uint8_t l2_scratch[L2_SCRATCH_SIZE];
uint8_t* pl2_scratch = l2_scratch;

#endif

int32_t main(){
  int32_t num_test_cases = sizeof(test_cases)/sizeof(tiadalg_dof_plane_seperation_testParams_t);

  for (int32_t tc = 0; tc < num_test_cases; tc++){
    tiadalg_dof_plane_seperation_testParams_t cur_test_case = test_cases[tc];
    int32_t out_num_planes = 3;
    int32_t pad_pixels[4] = {0,0,0,0};

    uint8_t *out_planes_cn = (uint8_t*)malloc(cur_test_case.img_width*cur_test_case.img_height*out_num_planes);

    uint32_t *in_dof;
    uint32_t *in_dof_l2;
    uint8_t *out_planes_c66;
    uint8_t *out_planes_c66_l2;
    uint8_t *ref_planes;

    printf("TestCase # %d \n",tc);

    in_dof = (uint32_t*)malloc(cur_test_case.img_width*cur_test_case.img_height*sizeof(uint32_t));
    out_planes_c66 = (uint8_t*)malloc(cur_test_case.img_width*cur_test_case.img_height*out_num_planes);

    if(L2_SCRATCH_SIZE > (cur_test_case.img_width*cur_test_case.img_height*(sizeof(uint32_t) + out_num_planes))){
      in_dof_l2 = (uint32_t*)pl2_scratch;pl2_scratch+=cur_test_case.img_width*cur_test_case.img_height*sizeof(uint32_t);
      out_planes_c66_l2 = (uint8_t*)pl2_scratch;pl2_scratch+=cur_test_case.img_width*cur_test_case.img_height*out_num_planes;
    }else{
      in_dof_l2 = in_dof;
      out_planes_c66_l2 = out_planes_c66;
    }

    ref_planes = (uint8_t*)malloc(cur_test_case.img_width*cur_test_case.img_height*out_num_planes);

    tiadalg_buffer_context bmp_context;

    int32_t is_pass = 1;
    TI_FILE* fp;

    if((in_dof == NULL) || (out_planes_cn == NULL) || (out_planes_c66==NULL) || (ref_planes == NULL)){
      printf("memory could not be allocated, %x, %x, %x, %x\n",in_dof,out_planes_cn,out_planes_c66,ref_planes);
      exit(0);
    }

    /*Read input packed dof data*/
    if(cur_test_case.in_dof_file != 0x0)
    {
      fp = FOPEN(cur_test_case.in_dof_file,"rb");
      if(fp == NULL){
        printf("input dof file %s could not be opened", cur_test_case.in_dof_file);
      }else{
        /*16 byte header in tifc file format dumped by DOF simulator*/
        FSEEK(fp,16,SEEK_SET);
        FREAD(in_dof,4,cur_test_case.img_width*cur_test_case.img_height,fp);
        memcpy(in_dof_l2,in_dof,4*cur_test_case.img_width*cur_test_case.img_height);
      }
      FCLOSE(fp);
    }else{
      tiadalg_fill_buffer_random((uint8_t*)in_dof, cur_test_case.img_width*cur_test_case.img_height*4);
    }

#ifndef HOST_EMULATION
    tiadalg_init_cache();
    long long t0, t1;
    _TSC_enable();
    t0 = _TSC_read();
#endif

    int32_t ret_val_cn = tiadalg_dof_plane_seperation_cn(in_dof,
                                                      cur_test_case.img_width,
                                                      cur_test_case.img_height,
                                                      TIADALG_DATA_TYPE_U08,
                                                      pad_pixels,
                                                      out_planes_cn,
                                                      NULL
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

    /*Execute core algorithm*/
    int32_t ret_val_c66 = tiadalg_dof_plane_seperation_c66(in_dof_l2,
                                                      cur_test_case.img_width,
                                                      cur_test_case.img_height,
                                                      TIADALG_DATA_TYPE_U08,
                                                      pad_pixels,
                                                      out_planes_c66_l2,
                                                      NULL
                                                    );
#ifndef HOST_EMULATION
#ifdef _TMS320C6600
  t1 = _TSC_read();
  printf("Total Cycle consumed for Optimized c66x code is %lld \n", (t1-t0));
#endif
#endif

    memcpy(out_planes_c66,out_planes_c66_l2,cur_test_case.img_width*cur_test_case.img_height*out_num_planes);

    /*Read reference dof plane*/
    if(cur_test_case.ref_dof_file != NULL){
      int ret_val = tiadalg_read_bmp_context(cur_test_case.ref_dof_file, &bmp_context, NULL, 0x0);
      if(ret_val != TIADALG_PROCESS_SUCCESS){
        printf("Problem in reading the reference dof plane BMP file");
      }

      ret_val = tiadalg_img_load_from_bmp_context(&bmp_context,
                                              0, 0,
                                              cur_test_case.img_height,
                                              cur_test_case.img_width,
                                              cur_test_case.img_height,
                                              out_num_planes, ref_planes);
    }

    if ((ret_val_cn == TIADALG_PROCESS_SUCCESS) && (ref_planes != NULL)){
      if(cur_test_case.ref_dof_file != 0x0){

        printf("Comapring the reference and natural C \n");

        for (int32_t i = 0 ; i < cur_test_case.img_width*cur_test_case.img_height*out_num_planes; i++){
          if (abs(out_planes_cn[i] - ref_planes[i]) != 0){
            is_pass = 0;
            tiadalg_dummy_bmp_context(&bmp_context,cur_test_case.img_width,cur_test_case.img_height);
            tiadalg_img_save_to_bmp_context(&bmp_context,
                                              0, 0,
                                              cur_test_case.img_width, cur_test_case.img_height,
                                              cur_test_case.img_width, cur_test_case.img_height,
                                              out_num_planes, out_planes_cn);
            tiadalg_save_bmp_context(cur_test_case.out_dof_file, &bmp_context,NULL);
           break;
          }
        }
      }
    }
    else{
      is_pass = 0;
    }

    if (ret_val_c66 == TIADALG_PROCESS_SUCCESS){
      printf("Comapring C66x and natural C \n");
      for (int32_t i = 0 ; i < cur_test_case.img_width*cur_test_case.img_height*out_num_planes; i++){
        if (abs(out_planes_cn[i] - out_planes_c66[i]) != 0){
          is_pass = 0;
          printf("Mismatch is happening at %d, and ref = %d, out = %d \n",i,out_planes_cn[i],out_planes_c66[i]);
          break;
        }
      }
    }
    else{
      is_pass = 0;
    }

  free(out_planes_c66);
  free(in_dof);
  free(out_planes_cn);
  free(ref_planes);

  if(cur_test_case.ref_dof_file != 0x0){
    tiadalg_bmp_release_context(&bmp_context);
  }

  if (is_pass == 1){
    printf("Test case has passed \n");
  }
  else{
    printf("Test case has failed \n");
  }

  }
  return(0);
}
