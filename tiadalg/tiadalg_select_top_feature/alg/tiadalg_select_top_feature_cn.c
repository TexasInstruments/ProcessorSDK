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

//#define TIADALG_MATCHING_DEBUG

#ifdef TIADALG_MATCHING_DEBUG
#include <stdio.h>
#define TIADALG_MATCHING_DEBUG_COST
#define TIADALG_MATCHING_DEBUG_LISTA
#define TIADALG_MATCHING_DEBUG_LISTB
#endif

int32_t tiadalg_select_top_feature_cn(void* desc_a_list,
                                    void* desc_b_list,
                                    int32_t data_type, /*0-> 8 bit signed, 1 -> 16 bit signed*/
                                    uint16_t* desc_a_list_offset,
                                    int32_t desc_size, /*descriptor size in terms of int16_t*/
                                    int32_t num_desc_a,
                                    int32_t num_desc_b,
                                    int32_t num_top,
                                    uint16_t *top_a_indx,
                                    uint16_t *top_b_indx,
                                    uint32_t *scartch1,
                                    uint16_t *scratch2,
                                    uint16_t *scratch3,
                                    uint32_t *scratch4)
{
  uint16_t i,j;

  /* Scratch buffer informations
    scartch1 --> to store best score of each element of list_b for given desc_a. size of sizeof(U32)*num_desc_b
    scratch2 --> to store best match of each element of list_a. size of sizeof(U16)*num_desc_a
    scratch3 --> to store best match of each element of list_b. size of sizeof(U16)*num_desc_b
    scratch4 --> to store best scare of each element of list_b. size of sizeof(U32)*num_desc_b

    best matching score for each desc in lin list_a
  */
  for(j = 0; j < num_desc_b; j++){
    /*in natural C version, scratch is maintained as 32 bit unsigned only for u8 and u16 data type*/
    scratch4[j] = 0xFFFFFFFFU;
    scratch3[j] = 0x0;
  }

  for(i = 0; i < num_desc_a; i ++){

    if(data_type == 0x0){
      tiadalg_feature_matching_u08_cn(&((uint8_t*)desc_a_list)[desc_a_list_offset[i]* desc_size],
        (uint8_t*)desc_b_list,
        desc_size, /*descriptor size in terms of int16_t*/
        num_desc_b,
        scartch1);
    }
    else{
      tiadalg_feature_matching_u16_cn(&((uint16_t*)desc_a_list)[desc_a_list_offset[i]* desc_size],
        (uint16_t*)desc_b_list,
        desc_size, /*descriptor size in terms of int16_t*/
        num_desc_b,
        scartch1);
    }

#ifdef TIADALG_MATCHING_DEBUG_COST
    for(int32_t k = 0; k < num_desc_b; k++)
      printf("%d ",scartch1[k]);

    printf("\n");
#endif

    uint32_t min_score = 0xFFFFFFFFU;
    uint16_t min_score_idx = 0;

    /*Find best match in desc_b_list for given desc_a*/
    for(j = 0; j < num_desc_b; j++){

      if(scartch1[j] < min_score){ // ToDO if first or last best cost to be chosen
        min_score = scartch1[j];
        min_score_idx = j;
      }

      if(scratch4[j] > scartch1[j]){ // ToDO if first or last best cost to be chosen
        /*Update the better match for jth desc in list_b*/
        scratch4[j] =  scartch1[j];
        scratch3[j] = i;
      }
    }

    /*Best match of ith desc in list_a is found and it is 'min_score_idx' in list_b*/
    scratch2[i] = min_score_idx;

  }
#ifdef TIADALG_MATCHING_DEBUG_LISTA
  for(int32_t k = 0; k < num_desc_a; k++)
    printf("%d \n",scratch2[k]);
#endif

#ifdef TIADALG_MATCHING_DEBUG_LISTB
  for(int32_t k = 0; k < num_desc_b; k++)
    printf("%d %d \n",scratch3[k], scratch4[k]);
#endif
  /*Selecting top features*/
  int32_t total_matches = 0;
  for(i = 0; (i < num_desc_b) && (total_matches < num_top); i++){

    uint32_t min_score = 0xFFFFFFFFU;
    uint16_t min_score_idx = 0;

    /*find next best match of list_b*/
    for(j = 0; j < num_desc_b; j++){
      if(scratch4[j] < min_score){
        min_score = scratch4[j];
        min_score_idx = j;
      }
    }

    scratch4[min_score_idx] = 0xFFFFFFFFU;

    if(scratch2[scratch3[min_score_idx]] == min_score_idx){
      /*already selected score should be invalidated*/
      top_a_indx[total_matches] = desc_a_list_offset[scratch3[min_score_idx]];
      top_b_indx[total_matches] = min_score_idx;
      total_matches++;
    }
  }
  return(total_matches);
}

int32_t tiadalg_feature_matching_u08_cn(uint8_t* desc_a,
                                    uint8_t* desc_b_list,
                                    int32_t desc_size, /*descriptor size in terms of int16_t*/
                                    int32_t num_desc_b,
                                    uint32_t *desc_score)
{
  int32_t ret_val = TIADALG_PROCESS_SUCCESS;
  int32_t j,k;
  uint32_t cur_score;
  uint8_t * cur_desc_b;
  int32_t cur_disc_diff;

  for(j = 0; j < num_desc_b; j++){
    cur_desc_b = &desc_b_list[j*desc_size];
    cur_score = 0;
    for(k = 0; k < desc_size; k++){
      cur_disc_diff = desc_a[k] - cur_desc_b[k];
      cur_score += abs(cur_disc_diff);
    }

    *desc_score++ = cur_score;
  }

  return(ret_val);
}

int32_t tiadalg_feature_matching_u16_cn(uint16_t* desc_a,
  uint16_t* desc_b_list,
  int32_t desc_size, /*descriptor size in terms of int16_t*/
  int32_t num_desc_b,
  uint32_t *desc_score)
{
int32_t ret_val = TIADALG_PROCESS_SUCCESS;
int32_t j,k;
uint32_t cur_score;
uint16_t * cur_desc_b;
int32_t cur_disc_diff;

for(j = 0; j < num_desc_b; j++){
  cur_desc_b = &desc_b_list[j*desc_size];
  cur_score = 0;
  for(k = 0; k < desc_size; k++){
    cur_disc_diff = desc_a[k] - cur_desc_b[k];
    cur_score += abs(cur_disc_diff);
  }

*desc_score++ = cur_score;
}

return(ret_val);
}

