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
#include <tiadalg_fisheye_transformation_test.h>
#include <../common/profile.h>

#define TIADALG_FISHEYE_INT_TABLE_SIZE (1024)

int32_t main(){
  int32_t num_test_cases = sizeof(test_cases)/sizeof(tiadalg_fisheye_transformation_testParams_t);
  int32_t ret_val_cn,ret_val_c66;
  for (int32_t tc = 0; tc < num_test_cases; tc++){
    tiadalg_fisheye_transformation_testParams_t cur_test_case = test_cases[tc];

    int16_t *out_points_cn = (int16_t*)malloc(cur_test_case.num_points*2*sizeof(int16_t));
    int16_t *out_points_c66 = (int16_t*)malloc(cur_test_case.num_points*2*sizeof(int16_t));
    uint16_t *scratch = (uint16_t*)malloc(TIADALG_FISHEYE_INT_TABLE_SIZE*2*sizeof(uint16_t));

    int32_t is_pass = 1;

    if((out_points_cn == NULL) || (out_points_c66 == NULL)){
      printf("memory could not be allocated\n");
      exit(0);
    }

    printf("TestCase # %d \n",tc);

    //memset(out_points_cn,0,cur_test_case.num_points*2*sizeof(uint16_t));
    //memset(out_points_c66,0,cur_test_case.num_points*2*sizeof(uint16_t));

#ifndef HOST_EMULATION
    tiadalg_init_cache();
    long long t0, t1;
    _TSC_enable();
    t0 = _TSC_read();
#endif

    ret_val_cn = tiadalg_fisheye_transformation_cn(cur_test_case.in_points,
                                                     cur_test_case.num_points,
                                                     cur_test_case.in_center_x,
                                                     cur_test_case.in_center_y,
                                                     cur_test_case.focal_length,
                                                     cur_test_case.out_center_x,
                                                     cur_test_case.out_center_y,
                                                     cur_test_case.angle_table,
                                                     cur_test_case.num_table_row,
                                                     0x0,0,
                                                     out_points_cn
                                                     );
#ifndef HOST_EMULATION
#ifdef _TMS320C6600
    t1 = _TSC_read();

    printf("Total Cycle consumed for Natural C code is %lld \n", (t1-t0));
#endif
#endif

    ret_val_c66 = tiadalg_fisheye_transformation_c66(cur_test_case.in_points,
                                                    cur_test_case.num_points,
                                                    cur_test_case.in_center_x,
                                                    cur_test_case.in_center_y,
                                                    cur_test_case.focal_length,
                                                    cur_test_case.out_center_x,
                                                    cur_test_case.out_center_y,
                                                    cur_test_case.angle_table,
                                                    cur_test_case.num_table_row,
                                                    scratch, 0x0,
                                                    out_points_c66
                                                    );

#ifndef HOST_EMULATION
#ifdef _TMS320C6600
      t0 = _TSC_read();
#endif
#endif
    ret_val_c66 = tiadalg_fisheye_transformation_c66(cur_test_case.in_points,
                                                    cur_test_case.num_points,
                                                    cur_test_case.in_center_x,
                                                    cur_test_case.in_center_y,
                                                    cur_test_case.focal_length,
                                                    cur_test_case.out_center_x,
                                                    cur_test_case.out_center_y,
                                                    cur_test_case.angle_table,
                                                    cur_test_case.num_table_row,
                                                    scratch, 0x1,
                                                    out_points_c66
                                                    );
#ifndef HOST_EMULATION
#ifdef _TMS320C6600
      t1 = _TSC_read();
      printf("Total Cycle consumed for Optimized c66x code is %lld \n", (t1-t0));
#endif
#endif

    if ((ret_val_cn == TIADALG_PROCESS_SUCCESS) && (cur_test_case.ref_out_points != NULL)){
      printf("comapring natural C output with reference data \n");
      for (int32_t i = 0 ; i < cur_test_case.num_points*2; i++){
        if (abs(out_points_cn[i] - cur_test_case.ref_out_points[i]) > 1){
         is_pass = 0;
         printf("Match with reference output has failed \n");
         break;
        }
      }
    }
    else{
      is_pass = 0;
    }

    if (ret_val_c66 == TIADALG_PROCESS_SUCCESS){
      printf("comapring optimzed output with natural c output \n");
      for (int32_t i = 0 ; i < cur_test_case.num_points*2; i++){
        if (abs(out_points_cn[i] - out_points_c66[i]) > 1){
         is_pass = 0;
         printf("Match with natural c has failed \n");
         break;
        }
      }
    }
    else{
      is_pass = 0;
    }
    free(scratch);
    free(out_points_cn);
    free(out_points_c66);

    if (is_pass == 1){
      printf("Test case has passed \n");
    }
    else{
      printf("Test case has failed \n");
    }

  }
  return(ret_val_c66);
}
