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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <./../common/tiadalg_test_interface.h>
#include <tiadalg_interface.h>
#include <vector>
#include <float.h>
#include <./../common/profile.h>

#include <iostream>
#include <cstring>
#include <tiadalg_solve_pnp_test.h>

#define USE_INT_MEM

#ifdef USE_INT_MEM

#define L2_SCRATCH_SIZE (4096)

#pragma DATA_SECTION (".dmem1")
uint8_t l2_scratch[L2_SCRATCH_SIZE];
uint8_t* pl2_scratch = l2_scratch;

#endif

int32_t main(){
  int32_t num_test_cases = sizeof(test_cases)/sizeof(tiadalg_solve_pnp_testParams_t);

  for (int32_t tc = 0; tc < num_test_cases; tc++){

    tiadalg_solve_pnp_testParams_t cur_test_case = test_cases[tc];
    uint8_t* pl2_scratch = l2_scratch;

    float *twoDPoints = (float*)pl2_scratch;pl2_scratch+=cur_test_case.num_points*sizeof(float)*2;//malloc(cur_test_case.num_points*sizeof(float)*2);
    float *threeDPoints = (float*)pl2_scratch;pl2_scratch+=cur_test_case.num_points*sizeof(float)*3;//malloc(cur_test_case.num_points*sizeof(float)*3);
    float *rMat_ref = (float*)malloc(sizeof(float)*3*3);
    float *tvec_ref = (float*)malloc(sizeof(float)*3);
    float *rMat_cn = (float*)malloc(sizeof(float)*3*3);
    float *tvec_cn = (float*)malloc(sizeof(float)*3);
    float *rMat_c66 = (float*)malloc(sizeof(float)*3*3);
    float *tvec_c66 = (float*)malloc(sizeof(float)*3);

    tiadalg_p3p_params prms;

    int32_t is_pass = 1;
    int32_t ret_val_cn,ret_val_c66;

#ifdef USE_INT_MEM
    if((((uint64_t)pl2_scratch) - ((uint64_t)l2_scratch)) > L2_SCRATCH_SIZE){
      printf("Internal memory allocation has exceeded \n");
    }
#endif

#ifndef HOST_EMULATION
    tiadalg_init_cache();
#endif

    tiadalg_read_f32_data_from_file(twoDPoints,  cur_test_case.num_points*2,  cur_test_case.in_2d_point_list_file);
    tiadalg_read_f32_data_from_file(threeDPoints, cur_test_case.num_points*3, cur_test_case.in_3d_point_list_file);
    tiadalg_read_f32_data_from_file(rMat_ref, 3*3, cur_test_case.ref_rMat_file);
    tiadalg_read_f32_data_from_file(tvec_ref, 3, cur_test_case.ref_tVec_file);

    prms.fx = 311.833f;
    prms.fy = 311.833f;
    prms.cx = 640.0f;
    prms.cy = 360.0f;
    prms.iterationsCount = 200;
    prms.reprojErrorThreshold = 4.0f;
    prms.inliersRatio = 0.95f;
    prms.seed = 258001;

    if((twoDPoints != 0x0) && (threeDPoints != 0x0) && (rMat_ref != 0x0) && (tvec_ref != 0x0) && (rMat_cn != 0x0) && (tvec_cn != 0x0)){


      printf("Kernel call has started \n");

#ifndef HOST_EMULATION
    tiadalg_init_cache();
    long long t0, t1;
    _TSC_enable();
    t0 = _TSC_read();
#endif

      ret_val_cn=
      tiadalg_solve_pnp_cn(&prms,
                           (float (*)[3])rMat_cn,
                           tvec_cn,
                           (float (*)[2])twoDPoints,
                           (float (*)[3])threeDPoints,
                           cur_test_case.num_points
                           );

#ifndef HOST_EMULATION
    t1 = _TSC_read();

    printf("Total Cycle consumed for Natural C code is %lld \n", (t1-t0));
#endif

#ifndef HOST_EMULATION
      t0 = _TSC_read();
#endif

      prms.seed = 258001;

      ret_val_c66=
      tiadalg_solve_pnp_c66(&prms,
                           (float (*)[3])rMat_c66,
                           tvec_c66,
                           (float (*)[2])twoDPoints,
                           (float (*)[3])threeDPoints,
                           cur_test_case.num_points
                           );

      printf("After c66 call tx = %f, ty = %f, tx = %f \n", tvec_c66[0],tvec_c66[1],tvec_c66[2]);

#ifndef HOST_EMULATION
      t1 = _TSC_read();
      printf("Total Cycle consumed for Optimized c66x code is %lld \n", (t1-t0));
#endif

      /*compare reference and natural c output*/
      if ((ret_val_cn == TIADALG_PROCESS_SUCCESS) && (ret_val_c66 == TIADALG_PROCESS_SUCCESS)){
        printf("comparing reference and natural c \n");
        //is_pass &= tiadalg_comp_f32_data(rMat_cn,rMat_ref,9,0.1f);
        //is_pass &= tiadalg_comp_f32_data(tvec_cn,tvec_ref,3,0.1f);
        printf("comparing natural c and c66x\n");
        is_pass &= tiadalg_comp_f32_data(rMat_c66,rMat_cn,9,0.1f);
        is_pass &= tiadalg_comp_f32_data(tvec_c66,tvec_cn,3,0.1f);
      }
      else{
        is_pass = 0;
      }
      /*convert solvePnP inverse output*/
      {
        float rot_matrix_trans[3][3];
        float translation[3];

        for(int32_t i = 0; i < 3; i++){
          rot_matrix_trans[i][0] = rMat_c66[0*3 + i];
          rot_matrix_trans[i][1] = rMat_c66[1*3 + i];
          rot_matrix_trans[i][2] = rMat_c66[2*3 + i];
        }

        translation[0] = -rot_matrix_trans[0][0]*tvec_c66[0]\
                          - rot_matrix_trans[0][1]*tvec_c66[1]\
                          - rot_matrix_trans[0][2]*tvec_c66[2];

        translation[1] = -rot_matrix_trans[1][0]*tvec_c66[0] \
                          - rot_matrix_trans[1][1]*tvec_c66[1] \
                          - rot_matrix_trans[1][2]*tvec_c66[2];

        translation[2] = -rot_matrix_trans[2][0]*tvec_c66[0] \
                          - rot_matrix_trans[2][1]*tvec_c66[1] \
                          - rot_matrix_trans[2][2]*tvec_c66[2];

      printf("current location estimated is %f, %f, %f \n", translation[0], translation[1], translation[2]);

      }
      free(rMat_ref);
      free(tvec_ref);
      free(rMat_cn);
      free(tvec_cn);
      free(rMat_c66);
      free(tvec_c66);
    }else{
      printf("Memory allocation problem \n");
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

