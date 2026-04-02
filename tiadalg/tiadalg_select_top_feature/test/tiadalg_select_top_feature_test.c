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
#include "tiadalg_select_top_feature_test.h"
#include <../common/profile.h>

#define USE_INT_MEM

#ifdef USE_INT_MEM

#define L2_SCRATCH_SIZE (128*1024)

#pragma DATA_SECTION (".dmem1")
uint8_t l2_scratch[L2_SCRATCH_SIZE];
uint8_t* pl2_scratch = l2_scratch;

#endif

int32_t main(){
  int32_t num_test_cases = sizeof(test_cases)/sizeof(tiadalg_select_top_feature_testParams_t);

  for (int32_t tc = 0; tc < num_test_cases; tc++){
    tiadalg_select_top_feature_testParams_t cur_test_case = test_cases[tc];
    int32_t elm_size_bytes = (cur_test_case.elm_size_bits >> 3);
    int32_t desc_8s_or_16s = (elm_size_bytes == 1) ? 0 :1; // 0 -> 8bit, 1->16bit
    int32_t desc_size_bytes = elm_size_bytes*cur_test_case.desc_size_elms;

    uint8_t *in_list_a = (uint8_t*)malloc(cur_test_case.list_a_size*desc_size_bytes);

#ifndef USE_INT_MEM
    uint8_t *in_list_b = (uint8_t*)malloc(cur_test_case.list_b_size*desc_size_bytes);
#else
    uint8_t *in_list_b = (uint8_t*)pl2_scratch;pl2_scratch+=cur_test_case.list_b_size*desc_size_bytes;
#endif

    uint16_t *desc_a_list_offset = (uint16_t*)malloc(cur_test_case.list_a_size*sizeof(uint16_t));

    uint16_t *out_feat_a_idx_cn = (uint16_t*)malloc(cur_test_case.num_top_feat*sizeof(uint16_t));
    uint16_t *out_feat_b_idx_cn = (uint16_t*)malloc(cur_test_case.num_top_feat*sizeof(uint16_t));

#ifndef USE_INT_MEM
    uint16_t *out_feat_a_idx_c66 = (uint16_t*)malloc(cur_test_case.num_top_feat*sizeof(uint16_t));
    uint16_t *out_feat_b_idx_c66 = (uint16_t*)malloc(cur_test_case.num_top_feat*sizeof(uint16_t));
#else
    uint16_t *out_feat_a_idx_c66 = (uint16_t*)pl2_scratch;pl2_scratch+=cur_test_case.num_top_feat*sizeof(uint16_t);
    uint16_t *out_feat_b_idx_c66 = (uint16_t*)pl2_scratch;pl2_scratch+=cur_test_case.num_top_feat*sizeof(uint16_t);
#endif

    uint16_t *ref_feat_a_idx = (uint16_t*)malloc(cur_test_case.num_top_feat*sizeof(uint16_t));
    uint16_t *ref_feat_b_idx = (uint16_t*)malloc(cur_test_case.num_top_feat*sizeof(uint16_t));

#ifndef USE_INT_MEM
    uint8_t* scratch1 = (uint8_t*)malloc(sizeof(uint32_t)*cur_test_case.list_b_size);
    uint8_t* scratch2 = (uint8_t*)malloc(sizeof(uint16_t)*cur_test_case.list_a_size);
    uint8_t* scratch3 = (uint8_t*)malloc(sizeof(uint16_t)*cur_test_case.list_b_size);
    uint8_t* scratch4 = (uint8_t*)malloc(sizeof(uint32_t)*cur_test_case.list_b_size);
#else
    uint8_t* scratch1 = (uint8_t*)pl2_scratch;pl2_scratch+=sizeof(uint32_t)*cur_test_case.list_b_size;
    uint8_t* scratch2 = (uint8_t*)pl2_scratch;pl2_scratch+=sizeof(uint16_t)*cur_test_case.list_a_size;
    uint8_t* scratch3 = (uint8_t*)pl2_scratch;pl2_scratch+=sizeof(uint16_t)*cur_test_case.list_b_size;
    uint8_t* scratch4 = (uint8_t*)pl2_scratch;pl2_scratch+=sizeof(uint32_t)*cur_test_case.list_b_size;;
#endif

    printf("TestCase # %d \n",tc);

#ifdef USE_INT_MEM
    if((((uint64_t)pl2_scratch) - ((uint64_t)l2_scratch)) > L2_SCRATCH_SIZE){
      printf("Internal memory allocation has exceeded \n");
    }
#endif

    if((in_list_a == NULL) || (in_list_b == NULL) || (desc_a_list_offset==NULL) || (out_feat_a_idx_cn == NULL) ||
       (out_feat_b_idx_cn == NULL) || (out_feat_a_idx_c66 == NULL) || (ref_feat_a_idx == NULL) || (ref_feat_b_idx==NULL)||
       (scratch1 == NULL) || (scratch2 == NULL) || (scratch3 == NULL) || (scratch4 == NULL)
      ){
      printf("memory could not be allocated\n");
      exit(0);
    }

    int32_t is_pass = 1;
    int32_t i;
    int32_t ret_val_c66 = TIADALG_PROCESS_SUCCESS;
    int32_t ret_val_cn = TIADALG_PROCESS_SUCCESS;

    if(desc_8s_or_16s == 0){
      printf("reading of list a data has started \n");
      tiadalg_read_s8_data_from_file((int8_t*)in_list_a, (cur_test_case.list_a_size), cur_test_case.in_list_a_file);
      printf("reading of list b data has started \n");
      tiadalg_read_s8_data_from_file((int8_t*)in_list_b, (cur_test_case.list_b_size),cur_test_case.in_list_b_file);
    }else{
      printf("reading of list a data has started \n");
      tiadalg_read_s16_data_from_file((int16_t*)in_list_a, (cur_test_case.list_a_size), cur_test_case.in_list_a_file);
      printf("reading of list b data has started \n");
      tiadalg_read_s16_data_from_file((int16_t*)in_list_b, (cur_test_case.list_b_size),cur_test_case.in_list_b_file);
    }

    /*Descriptor are assumed as unint, instead of signed integer presnet in file, hence adding offset*/
    /*test bench is for only 16 bit*/
    for(i =0 ;i < cur_test_case.list_a_size*cur_test_case.desc_size_elms; i++){
      ((uint16_t*)in_list_a)[i] = ((int16_t*)in_list_a)[i] + 32768;
    }

    for(i =0 ;i < cur_test_case.list_b_size*cur_test_case.desc_size_elms; i++){
      ((uint16_t*)in_list_b)[i] = ((int16_t*)in_list_b)[i] + 32768;
    }

    for(i =0 ; i < cur_test_case.list_a_size; i++){
      desc_a_list_offset[i] = i;
    }
    printf("computation has started \n");
#ifndef HOST_EMULATION
    tiadalg_init_cache();
    long long t0, t1;
    _TSC_enable();
    t0 = _TSC_read();
#endif

    /*Execute natural C algorithm*/
     ret_val_cn = tiadalg_select_top_feature_cn((void*)in_list_a,
                                                (void*)in_list_b,
                                                desc_8s_or_16s,
                                                desc_a_list_offset,
                                                cur_test_case.desc_size_elms, /*descriptor size in terms of elements*/
                                                cur_test_case.list_a_size,
                                                cur_test_case.list_b_size,
                                                cur_test_case.num_top_feat,
                                                out_feat_a_idx_cn,
                                                out_feat_b_idx_cn,
                                                (uint32_t*)scratch1,
                                                (uint16_t*)scratch2,
                                                (uint16_t*)scratch3,
                                                (uint32_t*)scratch4);
#ifndef HOST_EMULATION
    t1 = _TSC_read();

    printf("Total Cycle consumed for Natural C code is %lld \n", (t1-t0));
#endif


#ifndef HOST_EMULATION
      t0 = _TSC_read();
#endif

     ret_val_c66 = tiadalg_select_top_feature_c66((int16_t*)in_list_a,
                                                (int16_t*)in_list_b,
                                                desc_8s_or_16s,
                                                desc_a_list_offset,
                                                cur_test_case.desc_size_elms, /*descriptor size in terms of elements*/
                                                cur_test_case.list_a_size,
                                                cur_test_case.list_b_size,
                                                cur_test_case.num_top_feat,
                                                out_feat_a_idx_c66,
                                                out_feat_b_idx_c66,
                                                (uint32_t*)scratch1,
                                                (uint16_t*)scratch2,
                                                (uint16_t*)scratch3,
                                                (uint32_t*)scratch4);

#ifndef HOST_EMULATION
      t1 = _TSC_read();
      printf("Total Cycle consumed for Optimized c66x code is %lld \n", (t1-t0));
#endif


    if ( ret_val_c66 == ret_val_cn ){
        printf("comparing reference and natural c \n");
        is_pass &= tiadalg_comp_u16_data(out_feat_a_idx_cn,out_feat_a_idx_c66,ret_val_c66,0);
        is_pass &= tiadalg_comp_u16_data(out_feat_b_idx_cn,out_feat_b_idx_c66,ret_val_c66,0);
    }
    else{
      is_pass = 0;
    }

#ifndef USE_INT_MEM
    free(in_list_b);
    free(out_feat_a_idx_c66);
    free(out_feat_b_idx_c66);
    free(scratch1);
    free(scratch2);
    free(scratch3);
    free(scratch4);
#endif

    free(in_list_a);
    free(desc_a_list_offset);
    free(out_feat_a_idx_cn);
    free(out_feat_b_idx_cn);
    free(ref_feat_a_idx);
    free(ref_feat_b_idx);

    if (is_pass == 1){
      printf("Test case has passed \n");
    }
    else{
      printf("Test case has failed \n");
    }

  }
  return(0);
}
