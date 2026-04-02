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
#include <assert.h>

//#define TIADALG_MATCHING_DEBUG
#ifndef HOST_EMULATION
//#define ENABLE_PROFILE
#endif

#ifdef ENABLE_PROFILE
#include <stdio.h>
#endif

#ifdef TIADALG_MATCHING_DEBUG
#include <stdio.h>
#define TIADALG_MATCHING_DEBUG_COST
#define TIADALG_MATCHING_DEBUG_LISTA
#define TIADALG_MATCHING_DEBUG_LISTB
#endif

int32_t tiadalg_select_top_feature_c66(void *restrict desc_a_list,
                                    void    *restrict desc_b_list,
                                    int32_t data_type, /*0-> 8 bit signed, 1 -> 16 bit signed*/
                                    uint16_t *restrict desc_a_list_offset,
                                    int32_t desc_size, /*descriptor size in terms of elements*/
                                    int32_t num_desc_a,
                                    int32_t num_desc_b,
                                    int32_t num_top,
                                    uint16_t *restrict top_a_indx,
                                    uint16_t *restrict top_b_indx,
                                    uint32_t *restrict scartch1,
                                    uint16_t *restrict scratch2,
                                    uint16_t *restrict scratch3,
                                    uint32_t *restrict scratch4)
{
  uint16_t i,j;
  int32_t min_score_idx;
  uint16_t* restrict u16scratch4= NULL;
  uint32_t* restrict u32scratch4= NULL;

#ifndef HOST_EMULATION
  _nassert((((size_t)scartch1) & 0x00000007u) == 0x0);
  _nassert((((size_t)scratch2) & 0x00000007u) == 0x0);
  _nassert((((size_t)scratch3) & 0x00000007u) == 0x0);
  _nassert((((size_t)scratch4) & 0x00000007u) == 0x0);
  _nassert((((size_t)desc_a_list) & 0x00000007u) == 0x0);
  _nassert((((size_t)desc_b_list) & 0x00000007u) == 0x0);
#endif

#ifdef ENABLE_PROFILE
  long long t0,t1;
  long long acc0, acc1, acc2;

  acc0 = 0;
  acc1 = 0;
  acc2 = 0;
#endif
  /* buffer informations
    desc_a_list_offset --> sizeof(U16)*num_desc_a
    scartch1 --> to store best score of each element of list_b for given desc_a. size of sizeof(U32)*num_desc_b
    scratch2 --> to store best match of each element of list_a. size of sizeof(U16)*num_desc_a
    scratch3 --> to store best match of each element of list_b. size of sizeof(U16)*num_desc_b
    scratch4 --> to store best scare of each element of list_b. size of sizeof(U32)*num_desc_b
    top_a_indx --> size for result sizeof(U16)*num_desc_a
    top_b_indx --> size for result sizeof(U16)*num_desc_b
    best matching score for each desc in lin list_a
  */
  for(j = 0; j < num_desc_b; j++){
    scratch4[j] = 0xFFFFFFFFU;
    scratch3[j] = 0x0;
  }

  if(data_type == TIADALG_DATA_TYPE_U08){
    u16scratch4 = (uint16_t*)scratch4;
    for(j = 0; j < num_desc_b; j++){
      /*DSP instruction DCMPGT2, expects signed quantity. However real data of sad wil lnever reach to 0x7fff*/
      u16scratch4[j] = 0x7FFFU;
    }

  }else{
    u32scratch4 = (uint32_t*)scratch4;
    for(j = 0; j < num_desc_b; j++){
      u32scratch4[j] = 0xFFFFFFFFU;
    }
  }

  for(i = 0; i < num_desc_a; i ++){
#ifdef ENABLE_PROFILE
    t0 = _TSC_read();
#endif

    if(data_type == TIADALG_DATA_TYPE_U08){
      min_score_idx =
      tiadalg_feature_matching_u08_c66(&((uint8_t*)desc_a_list)[desc_a_list_offset[i]* desc_size],
        (uint8_t*)desc_b_list,
        desc_size, /*descriptor size in terms of elements*/
        num_desc_b,
        scartch1);
    }
    else{
      min_score_idx =
      tiadalg_feature_matching_u16_c66(&((uint16_t*)desc_a_list)[desc_a_list_offset[i]* desc_size],
        (uint16_t*)desc_b_list,
        desc_size, /*descriptor size in terms of elements*/
        num_desc_b,
        scartch1);
    }
#ifdef ENABLE_PROFILE
  t1 = _TSC_read();
  acc0 += (t1-t0);
#endif

#ifdef TIADALG_MATCHING_DEBUG_COST
    for(int32_t k = 0; k < num_desc_b; k++)
      printf("%d ",scartch1[k]);

    printf("\n");
#endif

#ifdef ENABLE_PROFILE
  t0 = _TSC_read();
#endif
    if(data_type == TIADALG_DATA_TYPE_U08){
      /*For 8 bit input, descriptor matching cost is assumed to be fitting in 16 bit*/
      /*Also total number of descritpor is assumed to be multiple of 4*/
      /*Cost of matching is always in 32 bit container from the core matching API*/
      uint64_t u64temp4;
      uint64_t u64temp1,u64temp2,u64temp11,u64temp3;
      uint32_t u32temp1;
      uint64_t packed_i = _itoll(((i<<16)|i), ((i<<16)|i));
      uint64_t true_mask,false_mask;

#if 1
      for(j = 0; j < num_desc_b; j+=4){
        u64temp4 = _amem8(&u16scratch4[j]);
        u64temp1 = _amem8(&scartch1[j]);
        u64temp2 = _amem8(&scartch1[j+2]);
        u64temp11= _itoll(_pack2(_hill(u64temp2),_loll(u64temp2)),_pack2(_hill(u64temp1),_loll(u64temp1)));
        u64temp3 = _amem8(&scratch3[j]);
        u32temp1 = _dcmpgt2(u64temp4,u64temp11);
        true_mask= _dxpnd2(u32temp1);
        false_mask= _dxpnd2(~u32temp1);

        _amem8(&u16scratch4[j]) = (true_mask&u64temp11) | (false_mask&u64temp4);
        _amem8(&scratch3[j]) = (true_mask&packed_i) | (false_mask&u64temp3);
      }
#endif

      for(; j < num_desc_b; j++){
        if(u16scratch4[j] > scartch1[j]){ // ToDO if first or last best cost to be chosen
          /*Update the better match for jth desc in list_b*/
          u16scratch4[j] =  (uint16_t)scartch1[j];
          scratch3[j] = i;
        }
      }

    }else{
      /*For 16 bit input, descriptor matching cost is assumed to be fitting in 32 bit*/
      for(j = 0; j < num_desc_b; j++){
        if(scratch4[j] > scartch1[j]){ // ToDO if first or last best cost to be chosen
          /*Update the better match for jth desc in list_b*/
          scratch4[j] =  scartch1[j];
          scratch3[j] = i;
        }
      }
    }

    /*Best match of ith desc in list_a is found and it is 'min_score_idx' in list_b*/
    scratch2[i] = (uint16_t)min_score_idx;
#ifdef ENABLE_PROFILE
  t1 = _TSC_read();
  acc1 += (t1-t0);
#endif

  }
#ifdef TIADALG_MATCHING_DEBUG_LISTA
  for(int32_t k = 0; k < num_desc_a; k++)
    printf("%d \n",scratch2[k]);
#endif

#ifdef TIADALG_MATCHING_DEBUG_LISTB
  if(data_type == TIADALG_DATA_TYPE_U08){
    for(int32_t k = 0; k < num_desc_b; k++)
      printf("%d %d \n",scratch3[k], u16scratch4[k]);
  }else{
    for(int32_t k = 0; k < num_desc_b; k++)
      printf("%d %d \n",scratch3[k], u32scratch4[k]);
  }
#endif
  /*Selecting top features*/
  int32_t total_matches = 0;
#ifdef ENABLE_PROFILE
  t0 = _TSC_read();
#endif
  if(data_type == TIADALG_DATA_TYPE_U08){
    for(i = 0; (i < num_desc_b) && (total_matches < num_top); i++){
      uint16_t min_score = 0xFFFFU;
      uint16_t min_score_idx = 0;

      /*find next best match of list_b*/
      for(j = 0; j < num_desc_b; j++){
        if(u16scratch4[j] < min_score){
          min_score = u16scratch4[j];
          min_score_idx = j;
        }
      }

      u16scratch4[min_score_idx] = 0xFFFFU;

      if(scratch2[scratch3[min_score_idx]] == min_score_idx){
        /*already selected score should be invalidated*/
        top_a_indx[total_matches] = desc_a_list_offset[scratch3[min_score_idx]];
        top_b_indx[total_matches] = min_score_idx;
        total_matches++;
      }
    }
  }else{
    for(i = 0; (i < num_desc_b) && (total_matches < num_top); i++){
        uint32_t min_score = 0xFFFFFFFFU;
        uint16_t min_score_idx = 0;

        /*find next best match of list_b*/
        for(j = 0; j < num_desc_b; j++){
          if(u32scratch4[j] < min_score){
            min_score = u32scratch4[j];
            min_score_idx = j;
          }
        }

        u32scratch4[min_score_idx] = 0xFFFFFFFFU;

        if(scratch2[scratch3[min_score_idx]] == min_score_idx){
          /*already selected score should be invalidated*/
          top_a_indx[total_matches] = desc_a_list_offset[scratch3[min_score_idx]];
          top_b_indx[total_matches] = min_score_idx;
          total_matches++;
        }
    }
  }

#ifdef ENABLE_PROFILE
  t1 = _TSC_read();
  acc2 += (t1-t0);
  printf("total cycle consumed in matching are %lld, %lld, %lld \n", acc0, acc1,acc2);
#endif


  return(total_matches);
}
int32_t tiadalg_feature_matching_u08_c66(uint8_t* desc_a,
                                            uint8_t* desc_b_list,
                                            int32_t desc_size,
                                            int32_t num_desc_b,
                                            uint32_t *desc_score)
{
  int32_t j;
  uint64_t *  restrict cur_desc_a;
  uint64_t *  restrict cur_desc_b;
  int64_t s64temp1, s64temp2, s64temp3;
  int32_t s32temp1, s32temp2, s32temp3;
  uint32_t u32_all_ones = 0x01010101u;
  uint32_t min_score = 0xFFFFFFFFU;
  uint32_t cur_score;
  int32_t min_idx=0;
  int32_t k;

  cur_desc_a = (uint64_t *)desc_a;

  for(j = 0; j < num_desc_b; j++){
    s64temp3 = 0;
    cur_desc_b = (uint64_t *)&desc_b_list[j*desc_size];
#pragma UNROLL(8)
    for(k = 0; k < 8; k++){

      s64temp1 = cur_desc_a[k]; // 8 element from a dec
      s64temp2 = _amem8_const(&cur_desc_b[k]); // 8 element from b desc
      s32temp1 = _subabs4(_loll(s64temp1), _loll(s64temp2));
      s32temp2 = _dotpsu4(s32temp1,u32_all_ones);
      s32temp1 = _subabs4(_hill(s64temp1), _hill(s64temp2));
      s32temp3 = _dotpsu4(s32temp1,u32_all_ones);
      s64temp3 = _dadd(s64temp3, _itoll(s32temp3,s32temp2));

    }
    cur_score =  (_loll(s64temp3) + _hill(s64temp3));
    *desc_score++ = cur_score;
    if (cur_score < min_score){
      min_score = cur_score;
      min_idx   = j;
    }
  }

  return(min_idx);
}

int32_t tiadalg_feature_matching_u16_c66(uint16_t* desc_a,
  uint16_t* desc_b_list,
  int32_t desc_size,
  int32_t num_desc_b,
  uint32_t *desc_score)
{
  int32_t j;


  uint64_t *  restrict cur_desc_a;
  uint64_t *  restrict cur_desc_b;


  int64_t s64temp1, s64temp2;
  int32_t s32temp1,s32temp2;
  uint32_t min_score = 0xFFFFFFFFU;
  uint32_t cur_score;
  int32_t min_idx = 0;
  int32_t k;

  cur_desc_a = (uint64_t *)desc_a;

  int64_t all_one_64 = _itoll(((0x1<<16) | (0x1<<0)),((0x1<<16) | (0x1<<0)));

  for(j = 0; j < num_desc_b; j++){
    s32temp1 = 0;
    s32temp2 = 0;
    cur_desc_b = (uint64_t *)&desc_b_list[j*desc_size];
    for(k = 0; k < 16; k+=2){
      s64temp1   = _dsub2(cur_desc_a[k], _amem8_const(&cur_desc_b[k]));
      s64temp1   = _dapys2(s64temp1,s64temp1);
      s32temp1   += _dotp4h(s64temp1,all_one_64);

      s64temp2   = _dsub2(cur_desc_a[k+1], _amem8_const(&cur_desc_b[k+1]));
      s64temp2   = _dapys2(s64temp2,s64temp2);
      s32temp2   += _dotp4h(s64temp2,all_one_64);
    }
    cur_score =  (s32temp1 + s32temp2);
    *desc_score++ = cur_score;
    if (cur_score < min_score){
      min_score = cur_score;
      min_idx   = j;
    }
  }
  return(min_idx);
}
