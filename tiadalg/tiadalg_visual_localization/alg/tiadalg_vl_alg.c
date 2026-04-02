/*
*
* Copyright (c) {2015 - 2019} Texas Instruments Incorporated
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


/**
*  @file       tiadalg_vl_alg.c
*
*  @brief     This file defines the XDAIS/iVISION functions for Ego Localization Algorithm
*
*
*  @version 1.0 (Feb 2019) : Base version.
*
*/

//#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <float.h>
//#pragma RESET_MISRA ("required")

#include "tiadalg_vl_alg_int.h"


//#define TIADALG_VL_DEBUG
//#define REF_ONLY_FLOW
//#define DUMP_BIN_KP_DESC

#ifdef TIADALG_VL_DEBUG
#include <stdio.h>
#define TIADALG_VL_DEBUG_VOXEL_ID
#define TIADALG_VL_DEBUG_IMG_FEAT
#define TIADALG_VL_DEBUG_MAP_FEAT
#define TIADALG_VL_DEBUG_MATCHED_FEAT_2D
#define TIADALG_VL_DEBUG_MATCHED_FEAT_3D
#define TIADALG_VL_DEBUG_SOLVE_PNP
#define TIADALG_VL_PARSE_WHOLE_MAP
#endif

#ifndef HOST_EMULATION
//#define ENABLE_PROFILE

#ifdef ENABLE_PROFILE
long long t0,t1,t2,t3,t4,t5,t6,t7,t8,t9;
long long acc0,acc1,acc2,acc3,acc4,acc5;
#endif

#endif


/**
* IVISION interface of the algorithm
*/
const IVISION_Fxns VL_VISION_FXNS =
{
  {
    (void *)&VL_VISION_FXNS, /* module ID              */
      EL_TI_activate,           /* activate               */
      EL_TI_alloc,              /* algAlloc               */
      NULL,                      /* control (not suported) */
      EL_TI_deactivate,         /* deactivate             */
      EL_TI_free,               /* free                   */
      EL_TI_init,               /* init                   */
      NULL,                      /* moved (not suported)   */
      EL_TI_numAlloc            /* algNumAlloc            */
  },
  EL_TI_process,
  EL_TI_control
};


/**
* Other exposed functions
*/
int32_t EL_TI_numAlloc(void)
{
  return NUM_MEMRECS;
}

/*---------------------------------------------------------------*/
/* Function to supply memory requirement to user                 */
/* refer algAlloc function for details in XADIS (ialg.h file)    */
/*---------------------------------------------------------------*/
int32_t EL_TI_alloc(const IALG_Params *params, struct IALG_Fxns **parentFxns,
                     IALG_MemRec *memRec)
{
  int32_t status = IALG_EOK;
  TIADALG_el_create_params * createParams = (TIADALG_el_create_params*)params;

  /*---------------------------------------------------------------------*/
  /* Error checks                                                        */
  /*---------------------------------------------------------------------*/
  //@TBD have error checks for create time parameters


  /*--------------------------------------------------------------------------*/
  /* 0th index should be alg handle memory                                    */
  /*--------------------------------------------------------------------------*/
  memRec[ALG_HANDLE_MEMREC].size          = sizeof(EL_TI_Obj);
  memRec[ALG_HANDLE_MEMREC].space         = (IALG_MemSpace)IALG_EXTERNAL;
  memRec[ALG_HANDLE_MEMREC].attrs         = IALG_PERSIST;
  memRec[ALG_HANDLE_MEMREC].alignment     = 8;

  memRec[EL_ALG_BUFDESC_VOXEL_INFO_MEM].size      = sizeof(tiadalg_voxel_info)*createParams->map_info.num_voxels;
  memRec[EL_ALG_BUFDESC_VOXEL_INFO_MEM].space     = (IALG_MemSpace)IALG_EXTERNAL;
  memRec[EL_ALG_BUFDESC_VOXEL_INFO_MEM].attrs     = IALG_PERSIST;
  memRec[EL_ALG_BUFDESC_VOXEL_INFO_MEM].alignment = 8;

  memRec[EL_ALG_BUFDESC_3D_POINTS_MEM].size      = sizeof(float)*3*createParams->map_info.num_map_feat;
  memRec[EL_ALG_BUFDESC_3D_POINTS_MEM].space     = (IALG_MemSpace)IALG_EXTERNAL;
  memRec[EL_ALG_BUFDESC_3D_POINTS_MEM].attrs     = IALG_PERSIST;
  memRec[EL_ALG_BUFDESC_3D_POINTS_MEM].alignment = 8;

  memRec[EL_ALG_BUFDESC_POINTS_DESC_MEM].size      = createParams->map_info.num_map_feat*createParams->map_info.desc_size*sizeof(VL_DESC_DATA_TYPE);
  memRec[EL_ALG_BUFDESC_POINTS_DESC_MEM].space     = (IALG_MemSpace)IALG_EXTERNAL;
  memRec[EL_ALG_BUFDESC_POINTS_DESC_MEM].attrs     = IALG_PERSIST;
  memRec[EL_ALG_BUFDESC_POINTS_DESC_MEM].alignment = 8;

  /*--------------------------------------------------------------------------*/
  /* Allocates persisitant external memory for object state structure         */
  /* Persitant base mmeory is dedicated to EL_TI_Prms                         */
  /*--------------------------------------------------------------------------*/
  memRec[EL_ALG_EXT_PERSIST_MEM].size      = sizeof(EL_TI_Prms);
  memRec[EL_ALG_EXT_PERSIST_MEM].space     = (IALG_MemSpace)IALG_EXTERNAL;
  memRec[EL_ALG_EXT_PERSIST_MEM].attrs     = IALG_PERSIST;
  memRec[EL_ALG_EXT_PERSIST_MEM].alignment = 8;

  /* Matcher module memory requirement
    desc_a_list_offset --> sizeof(U16)*num_desc_a
    scartch1 --> to store best score of each element of list_b for given desc_a. size of sizeof(U32)*num_desc_b
    scratch2 --> to store best match of each element of list_a. size of sizeof(U16)*num_desc_a
    scratch3 --> to store best match of each element of list_b. size of sizeof(U16)*num_desc_b
    scratch4 --> to store best scare of each element of list_b. size of sizeof(U32)*num_desc_b
    top_a_indx --> size for result sizeof(U16)*max_feat_match
    top_b_indx --> size for result sizeof(U16)*max_feat_match
    best matching score for each desc in lin list_a
    total_size = 4*max_map_feat + 10*max_frame_feat + 4*max_feat_match
  */
   int32_t matcher_scratch_size = sizeof(uint16_t)*createParams->max_map_feat + \
                                  sizeof(uint32_t)*createParams->max_frame_feat + \
                                  sizeof(uint16_t)*createParams->max_map_feat + \
                                  sizeof(uint16_t)*createParams->max_frame_feat + \
                                  sizeof(uint32_t)*createParams->max_frame_feat + \
                                  sizeof(uint16_t)*createParams->max_feat_match + \
                                  sizeof(uint16_t)*createParams->max_feat_match;

    /* solve_pnp_scratch_size = 20*max_feat_match bytes*/
    int32_t solve_pnp_scratch_size = sizeof(float)*2*createParams->max_feat_match + \
                                      sizeof(float)*3*createParams->max_feat_match;


  /*--------------------------------------------------------------------------*/
  /* Allocates scratch L2 memory                                              */
  /*--------------------------------------------------------------------------*/
  memRec[EL_ALG_L2_SCRATCH_MEM].size      = matcher_scratch_size  + solve_pnp_scratch_size + 1024;
  memRec[EL_ALG_L2_SCRATCH_MEM].space     = (IALG_MemSpace)IALG_DARAM1;
  memRec[EL_ALG_L2_SCRATCH_MEM].attrs     = IALG_SCRATCH;
  memRec[EL_ALG_L2_SCRATCH_MEM].alignment = 4;

  /*--------------------------------------------------------------------------*/
  /* Allocates scratch external scratch memory                                */
  /*--------------------------------------------------------------------------*/
  memRec[EL_ALG_EXT_SCRATCH_MEM].size      = 1024;
  memRec[EL_ALG_EXT_SCRATCH_MEM].space     = (IALG_MemSpace)IALG_EXTERNAL;
  memRec[EL_ALG_EXT_SCRATCH_MEM].attrs     = IALG_SCRATCH;
  memRec[EL_ALG_EXT_SCRATCH_MEM].alignment = 4;

  return (status);
}

/*-----------------------------------------------------------------------*/
/* Function to accept memory pointers from user and initialize algorithm */
/* Refer algInit function for details in XADIS (ialg.h file)             */
/*-----------------------------------------------------------------------*/
int32_t EL_TI_init(IALG_Handle handle, const IALG_MemRec *memRec,
                                 IALG_Handle parent, const IALG_Params * params)
{
  int32_t status = IALG_EOK;
  EL_TI_Prms* el_prms;

  TIADALG_el_create_params *createParams = (TIADALG_el_create_params*)params;
  EL_TI_Handle algHandle = (EL_TI_Handle)memRec[ALG_HANDLE_MEMREC].base;

  algHandle->numMemRecs  = NUM_MEMRECS;
  memcpy(algHandle->memRec,memRec, sizeof(IALG_MemRec)*algHandle->numMemRecs);

  algHandle->ivision          = (IVISION_Fxns *)&VL_VISION_FXNS;
  memcpy(&algHandle->algCreatePrms, createParams, sizeof(TIADALG_el_create_params));


  el_prms = (EL_TI_Prms*)algHandle->memRec[EL_ALG_EXT_PERSIST_MEM].base;
  el_prms->ext_scratch = (uint8_t*)algHandle->memRec[EL_ALG_EXT_SCRATCH_MEM].base;
  el_prms->l2_scratch = (uint8_t*)algHandle->memRec[EL_ALG_L2_SCRATCH_MEM].base;
  el_prms->voxel_info = (tiadalg_voxel_info*)algHandle->memRec[EL_ALG_BUFDESC_VOXEL_INFO_MEM].base;
  el_prms->map_3d_points = (float*)algHandle->memRec[EL_ALG_BUFDESC_3D_POINTS_MEM].base;
  el_prms->map_desc = (VL_DESC_DATA_TYPE*)algHandle->memRec[EL_ALG_BUFDESC_POINTS_DESC_MEM].base;

  memcpy(algHandle->memRec[EL_ALG_BUFDESC_VOXEL_INFO_MEM].base,createParams->map_voxel_ptr,algHandle->memRec[EL_ALG_BUFDESC_VOXEL_INFO_MEM].size);
  memcpy(algHandle->memRec[EL_ALG_BUFDESC_3D_POINTS_MEM].base,createParams->map_pt_ptr,algHandle->memRec[EL_ALG_BUFDESC_3D_POINTS_MEM].size);
  memcpy(algHandle->memRec[EL_ALG_BUFDESC_POINTS_DESC_MEM].base,createParams->map_desc_ptr,algHandle->memRec[EL_ALG_BUFDESC_POINTS_DESC_MEM].size);

  /*-----------------------------------------------------------------------*/
  /* State to indicate that internal context of algorithm is not active    */
  /*-----------------------------------------------------------------------*/
  algHandle->algState = ALG_NOT_ACTIVE;

  el_prms->frame_idx = 0;

  return (status);
}

/*-----------------------------------------------------------------------*/
/* Function to accept any run time updates with recreation of object     */
/* Refer algControl function for details in ivision.h file               */
/* Not implemented for this function                                     */
/*-----------------------------------------------------------------------*/
int32_t EL_TI_control(IVISION_Handle handle, IALG_Cmd cmd,
                       const IALG_Params *inParams, IALG_Params *outParams)
{
  int32_t status = IALG_EOK;

  return (status);
}

/*--------------------------------------------------------------------------*/
/* This is the main processing function and should be called for each frame */
/* Refer process function for details in ivision.h file                     */
/*--------------------------------------------------------------------------*/
int32_t EL_TI_process(IVISION_Handle handle, IVISION_InBufs *inBufs,
                       IVISION_OutBufs *outBufs, IVISION_InArgs *inArgs,
                       IVISION_OutArgs *outArgs)
{
  int32_t status             = IALG_EOK;
  EL_TI_Handle algHandle     = (EL_TI_Handle)(handle);
  TIADALG_el_in_args *in_args     = (TIADALG_el_in_args*)inArgs ;
  TIADALG_el_out_args *out_args     = (TIADALG_el_out_args*)outArgs ;
  EL_TI_Prms* el_prms = (EL_TI_Prms *)algHandle->memRec[EL_ALG_EXT_PERSIST_MEM].base;
  TIADALG_el_create_params *  create_params = &algHandle->algCreatePrms;
  tiadalg_map_info * map_info = &create_params->map_info;

  /*------------------------------------------------------------------------*/
  /* EL algorithm calculates the object distance from camera for at the max */
  /* EL_TI_MAX_INPUT_OBJECTS in a frame. If an objects which was detected   */
  /* in previous frame, moves out of current frame, it will be removed from */
  /* the tracked object list.                                               */
  /* If it reappears again after some frames, it will be treated as a new   */
  /* object.                                                                */
  /*------------------------------------------------------------------------*/
  algHandle->algState = ALG_NOT_ACTIVE;

  if(in_args->en_reset == 1)
  {
    el_prms->frame_idx = 0;
  }

  if(el_prms->frame_idx == 0)
  {
    el_prms->est_loc[0] = in_args->est_loc[0];
    el_prms->est_loc[1] = in_args->est_loc[1];
    el_prms->est_loc[2] = in_args->est_loc[2];
  }

  //el_prms->in_img = (uint8_t *)inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].buf;
  el_prms->ext_feat = (tiadalg_ext_feat *)inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].buf;
  el_prms->ext_desc = (VL_DESC_DATA_TYPE *)inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].buf;
  int32_t num_desc_b = in_args->num_cur_feat;
  /* Inverse of projection matrix*/
  float rot_matrix_trans[3][3];
  float translation[3];
  int32_t solve_pnp_status = TIADALG_PROCESS_FAILURE;

#ifdef DUMP_BIN_KP_DESC
      char file_name[200];
      static int32_t frameIdx = 0;
      FILE* fp;
      void* dst;

      sprintf(file_name,"%s_%010d.bin","ext_feat",frameIdx);
      printf("writing the file %s \n",file_name);
      dst = inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].buf;
      fp = fopen(file_name,"wb");
      fwrite(dst,1,num_desc_b*sizeof(tiadalg_ext_feat),fp);
      fclose(fp);

      sprintf(file_name,"%s_%010d.bin","ext_desc",frameIdx);
      printf("writing the file %s \n",file_name);
      dst = inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].buf;
      fp = fopen(file_name,"wb");
      fwrite(dst,1,num_desc_b*sizeof(tiadalg_feat_desc),fp);
      fclose(fp);
      frameIdx++;
#endif

#ifdef ENABLE_PROFILE
  TSCL=0;
  TSCH=0;
  acc0=0;acc1=0,acc2=0,acc3=0;acc4=0;
#endif

  if(num_desc_b > create_params->max_frame_feat){
    num_desc_b = create_params->max_frame_feat;
  }

  /*in buf parameters need to be checked*/
  if((el_prms->ext_feat != NULL) && (el_prms->ext_desc != NULL) && (num_desc_b >= 50))
  {

    // find current voxel location
    int32_t index[3];
#ifdef TIADALG_VL_DEBUG
    printf("Current search center is %f, %f, %f \n", el_prms->est_loc[0], el_prms->est_loc[1], el_prms->est_loc[2]);
#endif

    if((map_info->map_range[0][0] < el_prms->est_loc[0] && el_prms->est_loc[0] <= map_info->map_range[0][1]) &&
      (map_info->map_range[1][0] < el_prms->est_loc[1] && el_prms->est_loc[1] <= map_info->map_range[1][1]) &&
      (map_info->map_range[2][0] < el_prms->est_loc[2] && el_prms->est_loc[2] <= map_info->map_range[2][1]))
      {
        index[0] = (int32_t)floor((el_prms->est_loc[0] - (float)(map_info->map_range[0][0])) / (float)(map_info->voxel_size[0])) + 1;
        index[1] = (int32_t)floor((el_prms->est_loc[1] - (float)(map_info->map_range[1][0])) / (float)(map_info->voxel_size[1])) + 1;
        index[2] = (int32_t)floor((el_prms->est_loc[2] - (float)(map_info->map_range[2][0])) / (float)(map_info->voxel_size[2])) + 1;
      } else {
        index[0] = 0x7fffffff;
        index[1] = 0x7fffffff;
        index[2] = 0x7fffffff;
      }

#ifdef TIADALG_VL_DEBUG
    printf("Current voxel location is %d, %d, %d \n", index[0], index[1], index[2]);
#endif
    // prepare list of indices from map_features to be accounted for matching
    /* Matcher module memory requirement
      desc_a_list_offset --> sizeof(U16)*num_desc_a
      scartch1 --> to store best score of each element of list_b for given desc_a. size of sizeof(U32)*num_desc_b
      scratch2 --> to store best match of each element of list_a. size of sizeof(U16)*num_desc_a
      scratch3 --> to store best match of each element of list_b. size of sizeof(U16)*num_desc_b
      scratch4 --> to store best scare of each element of list_b. size of sizeof(U32)*num_desc_b
      top_a_indx --> size for result sizeof(U16)*num_desc_a
      top_b_indx --> size for result sizeof(U16)*num_desc_b
      best matching score for each desc in lin list_a
    */

    int32_t num_desc_a = create_params->max_map_feat;
    uint16_t * desc_a_list_offset = (uint16_t*)el_prms->l2_scratch;
    /*All pointer are double word aligned by default, assuming that base pointer is deouble word aligned*/
    uint32_t * scratch1 = (uint32_t *)addr_align((desc_a_list_offset + num_desc_a),8);
    uint16_t * scratch2 = (uint16_t *)addr_align((scratch1 + num_desc_b),8);
    uint16_t * scratch3 = (uint16_t *)addr_align((scratch2 + num_desc_a),8);
    uint32_t * scratch4 = (uint32_t *)addr_align((scratch3 + num_desc_b),8);
    uint16_t * top_a_indx = (uint16_t *)addr_align((scratch4 + num_desc_b),8);
    uint16_t * top_b_indx = (uint16_t *)addr_align((top_a_indx + create_params->max_feat_match),8);
    int32_t search_range_voxel[3];
    int32_t voxel_start_idx[3][2];
    int32_t map_id_range[3];
    int32_t l = 0;
    num_desc_a = 0;

    search_range_voxel[0] = in_args->search_range[0]/create_params->map_info.voxel_size[0];
    search_range_voxel[1] = in_args->search_range[1]/create_params->map_info.voxel_size[1];
    search_range_voxel[2] = in_args->search_range[2]/create_params->map_info.voxel_size[2];

    map_id_range[0] = (create_params->map_info.map_range[0][1] - create_params->map_info.map_range[0][0] + (create_params->map_info.voxel_size[0] -1))/create_params->map_info.voxel_size[0] ;
    map_id_range[1] = (create_params->map_info.map_range[1][1] - create_params->map_info.map_range[1][0] + (create_params->map_info.voxel_size[1] -1))/create_params->map_info.voxel_size[1] ;
    map_id_range[2] = (create_params->map_info.map_range[2][1] - create_params->map_info.map_range[2][0] + (create_params->map_info.voxel_size[2] -1))/create_params->map_info.voxel_size[2] ;

#ifdef ENABLE_PROFILE
    t0 = _TSC_read();
#endif



    voxel_start_idx[0][0] = mmax(0, index[0] - search_range_voxel[0]);
    voxel_start_idx[0][1] = mmin(map_id_range[0] - 1, index[0] + search_range_voxel[0]);
    voxel_start_idx[1][0] = mmax(0, index[1] - search_range_voxel[1]);
    voxel_start_idx[1][1] = mmin(map_id_range[1] - 1, index[1] + search_range_voxel[1]);
    voxel_start_idx[2][0] = mmax(0, index[2] - search_range_voxel[2]);
    voxel_start_idx[2][1] = mmin(map_id_range[2] - 1, index[2] + search_range_voxel[2]);

    int32_t cur_voxel_num_points;
#ifndef TIADALG_VL_PARSE_WHOLE_MAP

    float fov_h = 90.0 + 20.0;
    float fov_v = 90.0 + 20.0;
    float k_fov_h = tan((fov_h*TIADALG_PI)/(2*180.0));
    float k_fov_v = tan((fov_v*TIADALG_PI)/(2*180.0));
    float searchCenterProj[3][4];

    searchCenterProj[0][0] = el_prms->est_rot[0][0];
    searchCenterProj[0][1] = el_prms->est_rot[1][0];
    searchCenterProj[0][2] = el_prms->est_rot[2][0];
    searchCenterProj[0][3] = -(searchCenterProj[0][0]*el_prms->est_loc[0] +
                              searchCenterProj[0][1]*el_prms->est_loc[1] +
                              searchCenterProj[0][2]*el_prms->est_loc[2]);

    searchCenterProj[1][0] = el_prms->est_rot[0][1];
    searchCenterProj[1][1] = el_prms->est_rot[1][1];
    searchCenterProj[1][2] = el_prms->est_rot[2][1];
    searchCenterProj[1][3] = -(searchCenterProj[1][0]*el_prms->est_loc[0] +
                              searchCenterProj[1][1]*el_prms->est_loc[1] +
                              searchCenterProj[1][2]*el_prms->est_loc[2]);

    searchCenterProj[2][0] = el_prms->est_rot[0][2];
    searchCenterProj[2][1] = el_prms->est_rot[1][2];
    searchCenterProj[2][2] = el_prms->est_rot[2][2];
    searchCenterProj[2][3] = -(searchCenterProj[2][0]*el_prms->est_loc[0] +
                              searchCenterProj[2][1]*el_prms->est_loc[1] +
                              searchCenterProj[2][2]*el_prms->est_loc[2]);

    int32_t i, j, k;

    for(i = voxel_start_idx[0][0]; i <= voxel_start_idx[0][1]; i++){
      for(j = voxel_start_idx[1][0]; j <= voxel_start_idx[1][1]; j++){
        for(k = voxel_start_idx[2][0]; k <= voxel_start_idx[2][1]; k++){

          tiadalg_voxel_info* cur_voxel = &el_prms->voxel_info[i*map_id_range[1]*map_id_range[2] + j*map_id_range[2] + k];
          cur_voxel_num_points = cur_voxel->num_points;

          if((in_args->en_subsample_map != 0) && (el_prms->frame_idx != 0))
          {
            float p3d_world[3];
            float p3d_camera[3];

            p3d_world[0] = (float)(create_params->map_info.map_range[0][0] + i * create_params->map_info.voxel_size[0]);
            p3d_world[1] = (float)create_params->map_info.map_range[1][0] + j * create_params->map_info.voxel_size[0];
            p3d_world[2] = (float)create_params->map_info.map_range[2][0] + k * create_params->map_info.voxel_size[0];

            p3d_camera[0] = searchCenterProj[0][0]*p3d_world[0] +
                            searchCenterProj[0][1]*p3d_world[1] +
                            searchCenterProj[0][2]*p3d_world[2] +
                            searchCenterProj[0][3];

            p3d_camera[1] = searchCenterProj[1][0]*p3d_world[0] +
                            searchCenterProj[1][1]*p3d_world[1] +
                            searchCenterProj[1][2]*p3d_world[2] +
                            searchCenterProj[1][3];

            p3d_camera[2] = searchCenterProj[2][0]*p3d_world[0] +
                            searchCenterProj[2][1]*p3d_world[1] +
                            searchCenterProj[2][2]*p3d_world[2] +
                            searchCenterProj[2][3];


            //////////////////////////////////////////////////////////////////////////////
            // Original implementation
            // In ENU (X-Y-Z) convention
            // azimuth = np.degrees(np.arctan2(y, x))
            // elevation = np.degrees(np.arctan2(z, x))
            // For Camera co-ordinate convention
            // Float azimuth = fabs(atan2(-p3d_camera[0][0], p3d_camera[2][0]) * 180.0 / PI);
            // Float elevation = fabs(atan2(-p3d_camera[1][0], p3d_camera[2][0]) * 180.0 / PI);
            // if ((azimuth > (fov_h / 2.0)) || (elevation > (fov_v / 2.0))) {
            //     cur_voxel_in_fov = false;
            // }
            //////////////////////////////////////////////////////////////////////////////

            float y_3d = -p3d_camera[0];
            float x_3d = p3d_camera[2];

            if( (y_3d > k_fov_h*x_3d) || (y_3d < -k_fov_h*x_3d)){
                cur_voxel_num_points = 0;
            }

            y_3d = -p3d_camera[1];
            if( (y_3d > k_fov_v*x_3d) || (y_3d < -k_fov_v*x_3d)){
                cur_voxel_num_points = 0;
            }

          }
#else
    int32_t i;
    for(i = 0; i < create_params->map_info.num_voxels; i++){
      tiadalg_voxel_info* cur_voxel = &el_prms->voxel_info[i];
      if((cur_voxel->loc[0] <= (index[0] + search_range_voxel[0]))&&
        (cur_voxel->loc[0] >= (index[0] - search_range_voxel[0]))&&
        (cur_voxel->loc[1] <= (index[1] + search_range_voxel[1]))&&
        (cur_voxel->loc[1] >= (index[1] - search_range_voxel[1]))&&
        (cur_voxel->loc[2] <= (index[2] + search_range_voxel[2]))&&
        (cur_voxel->loc[2] >= (index[2] - search_range_voxel[2]))
        ){
          cur_voxel_num_points = cur_voxel->num_points;
#endif

#ifdef TIADALG_VL_DEBUG_VOXEL_ID
          printf("%d %d %d %d \n",cur_voxel->loc[0],cur_voxel->loc[1],cur_voxel->loc[2],cur_voxel_num_points);
#endif
          if((num_desc_a + cur_voxel_num_points) > create_params->max_map_feat){
            l = 0;
            do{
              desc_a_list_offset[num_desc_a] = (uint16_t)(cur_voxel->offset + l);
              num_desc_a++;
              l++;
            }while(num_desc_a <= create_params->max_map_feat);

            break;
          }
          /*Kept most likely loop to be without any condition so that it can be software pipelined*/
          int32_t l;
          for(l = 0; l <  cur_voxel_num_points; l++){
            desc_a_list_offset[num_desc_a] = (uint16_t)(cur_voxel->offset + l);
            num_desc_a++;
          }

        }
      }

#ifndef TIADALG_VL_PARSE_WHOLE_MAP
    }
#endif

#ifdef ENABLE_PROFILE
    t1 = _TSC_read();
    acc0 = (t1-t0);
    printf("Cycle consumed in picking each map descriptros is %lld \n",acc0/num_desc_a);
#endif

#ifdef TIADALG_VL_DEBUG
      printf("\n");
#endif

    // Do matching of map descriptor and current frame descriptor
    int32_t desc_size_elm = map_info->desc_size;


#ifdef TIADALG_VL_DEBUG_IMG_FEAT
    printf("Current frame feature  \n");
    for(int32_t i =0; i< num_desc_b; i++){
#ifdef TIADALG_EL_INT_FEAT
      printf("\n %f %f ",el_prms->ext_feat[i].x/((float)(in_args->feat_q_factor)),el_prms->ext_feat[i].y/((float)(in_args->feat_q_factor)));
#else
      printf("\n %f %f ",el_prms->ext_feat[i].x,el_prms->ext_feat[i].y);
#endif
      for(int32_t j = 0;j < 64; j++){
        printf(" %d",(el_prms->ext_desc[64*i + j]));
      }
    }
    printf("\n");
#endif

#ifdef TIADALG_VL_DEBUG_MAP_FEAT
    printf("Sampled map points are \n");
    for(int32_t i= 0; i < num_desc_a; i++){
     printf(" \n %f, %f, %f ", el_prms->map_3d_points[3*desc_a_list_offset[i]+0],\
                            el_prms->map_3d_points[3*desc_a_list_offset[i]+1],\
                            el_prms->map_3d_points[3*desc_a_list_offset[i]+2]
                            );
     for(int32_t j = 0;j < 64; j++){
       printf(" %d",(el_prms->map_desc[64*desc_a_list_offset[i] + j]-128));
     }
    }
    printf("\n");
#endif

#ifdef TIADALG_VL_DEBUG
  printf("Total current frame features are = %d  \n", num_desc_b);
  printf("Total map features are == %d \n", num_desc_a);
#endif

#ifdef ENABLE_PROFILE
    t2 = _TSC_read();
#endif

#if (defined(TMS320C66X) || defined(_TMS320C6600))&& !defined(REF_ONLY_FLOW)
    int32_t total_matches = tiadalg_select_top_feature_c66((void*)el_prms->map_desc,
                                    (void*)el_prms->ext_desc,
                                    (sizeof(VL_DESC_DATA_TYPE) == 1 ? 0 : 1),
                                    desc_a_list_offset, //uint16_t* desc_a_list_offset,
                                    desc_size_elm, /*descriptor size in terms of int16_t or int8_t*/
                                    num_desc_a,
                                    num_desc_b,
                                    create_params->max_feat_match,
                                    top_a_indx,
                                    top_b_indx,
                                    scratch1,
                                    scratch2,
                                    scratch3,
                                    scratch4);
#else
    int32_t total_matches = tiadalg_select_top_feature_cn((void*)el_prms->map_desc,
                                    (void*)el_prms->ext_desc,
                                    (sizeof(VL_DESC_DATA_TYPE) == 1 ? 0 : 1),
                                    desc_a_list_offset, //uint16_t* desc_a_list_offset,
                                    desc_size_elm, /*descriptor size in terms of int16_t*/
                                    num_desc_a,
                                    num_desc_b,
                                    create_params->max_feat_match,
                                    top_a_indx,
                                    top_b_indx,
                                    scratch1,
                                    scratch2,
                                    scratch3,
                                    scratch4);
#endif
#ifdef ENABLE_PROFILE
    t3 = _TSC_read();
    acc1 = (t3-t2);
    printf("Total cycle consumed feature matching is %lld \n",acc1);
    printf("cycle consumed for each matching is %lld \n",acc1/(num_desc_a*num_desc_b));
#endif
#ifdef TIADALG_VL_DEBUG
  printf("Total matches found and solvePnp executing points are %d \n ",total_matches);
#endif
#ifdef ENABLE_PROFILE
    t4 = _TSC_read();
#endif

    // based on matching find 2d points from current frame feature map, and 3d points from map
    float* points_2d = (float*)addr_align((top_b_indx + create_params->max_feat_match),8);
    float* points_3d = (float*)addr_align((points_2d + 2*create_params->max_feat_match),8);

    for(i = 0; i<  total_matches; i++){
#ifdef TIADALG_EL_INT_FEAT
        points_2d[2*i + 0] = ((float)el_prms->ext_feat[top_b_indx[i]].x)/((float)(in_args->feat_q_factor));
        points_2d[2*i + 1] = ((float)el_prms->ext_feat[top_b_indx[i]].y)/((float)(in_args->feat_q_factor));
#else
        points_2d[2*i + 0] = ((float)el_prms->ext_feat[top_b_indx[i]].x);
        points_2d[2*i + 1] = ((float)el_prms->ext_feat[top_b_indx[i]].y);
#endif
        points_3d[3*i + 0] = el_prms->map_3d_points[3*top_a_indx[i] + 0];
        points_3d[3*i + 1] = el_prms->map_3d_points[3*top_a_indx[i] + 1];
        points_3d[3*i + 2] = el_prms->map_3d_points[3*top_a_indx[i] + 2];
    }

#ifdef TIADALG_VL_DEBUG_MATCHED_FEAT_2D
    for(i = 0; i<  total_matches; i++){
        printf("%f %f \n", points_2d[2*i + 0],points_2d[2*i + 1]);
    }
#endif

#ifdef TIADALG_VL_DEBUG_MATCHED_FEAT_3D
    for(i = 0; i<  total_matches; i++){
        printf("%f, %f, %f \n", points_3d[3*i + 0],points_3d[3*i + 1],points_3d[3*i + 2]);
    }
#endif

#ifdef ENABLE_PROFILE
    t5 = _TSC_read();
    acc2 = (t5-t4);
    printf("Total cycle consumed in 2D-3D pair slection %lld \n",acc2/(total_matches));
#endif

#ifdef ENABLE_PROFILE
    t6 = _TSC_read();
#endif
    // solvePnP
    float R[3][3];
    float tvec[3];

    memset_float(tvec,0.0f,3);
    memset_float((float*)R,0.0f,3*3);

    if(total_matches > 16){

  #if  (defined(TMS320C66X) || defined(_TMS320C6600))&& !defined(REF_ONLY_FLOW)
      solve_pnp_status=
      tiadalg_solve_pnp_c66(&in_args->p3p_params,
                            R,
                            tvec,
                            (const float (*)[2])points_2d,
                            (const float (*)[3])points_3d,
                            total_matches);
  #else
      solve_pnp_status=
      tiadalg_solve_pnp_cn(&in_args->p3p_params,
                            R,
                            tvec,
                            (const float (*)[2])points_2d,
                            (const float (*)[3])points_3d,
                            total_matches);
  #endif

    }

#ifdef ENABLE_PROFILE
    t7 = _TSC_read();
    acc3 = (t7-t6);
    printf("Total cycle consumed in solvePnP is %lld \n",acc3);
    printf("Total cycle consumed per interation per point is %lld \n",acc3/(total_matches*in_args->p3p_params.iterationsCount));
#endif

#ifdef TIADALG_VL_DEBUG_SOLVE_PNP
    printf("SolvePnP output is R :: %f, %f, %f \n", R[0][0],R[0][1],R[0][2]);
    printf("                        %f, %f, %f \n", R[1][0],R[1][1],R[1][2]);
    printf("                        %f, %f, %f \n", R[2][0],R[2][1],R[2][2]);

    printf("SolvePnP output is T :: %f, %f, %f \n", tvec[0],tvec[1],tvec[2]);
#endif

    for(i = 0; i < 3; i++){
      rot_matrix_trans[i][0] = R[0][i];
      rot_matrix_trans[i][1] = R[1][i];
      rot_matrix_trans[i][2] = R[2][i];
    }

    translation[0] = -rot_matrix_trans[0][0]*tvec[0]\
                      - rot_matrix_trans[0][1]*tvec[1]\
                      - rot_matrix_trans[0][2]*tvec[2];

    translation[1] = -rot_matrix_trans[1][0]*tvec[0] \
                      - rot_matrix_trans[1][1]*tvec[1] \
                      - rot_matrix_trans[1][2]*tvec[2];

    translation[2] = -rot_matrix_trans[2][0]*tvec[0] \
                      - rot_matrix_trans[2][1]*tvec[1] \
                      - rot_matrix_trans[2][2]*tvec[2];

    memcpy(out_args->rotation,rot_matrix_trans,sizeof(float)*9);
    memcpy(out_args->translation,translation,sizeof(float)*3);

    /*get the angles if KF is disabled. For KF enabled scenario it will calculated below*/
    if(in_args->en_pose_filtering != 0x0)
    {
      getOrientationFromRotationMat(rot_matrix_trans, &out_args->angle[0]);
    }

#ifdef TIADALG_VL_DEBUG_SOLVE_PNP
        printf("Acctual output is T :: %f, %f, %f \n", translation[0],translation[1],translation[2]);
#endif

  }
#ifdef ENABLE_PROFILE
  t8 = _TSC_read();
#endif
  if(in_args->en_pose_filtering != 0x0){

    if(el_prms->frame_idx == 0){
      float transition_matrix[KF_MAXIMUM_STATE*KF_MAXIMUM_STATE];
      float measurementInterval = 0.1f;

      memset_float(transition_matrix, 0.0f, 4*4); /*here It is assumed of size 4x4*/

      /*Constant velocity model*/
      transition_matrix[0*4 + 0] = 1.0f;
			transition_matrix[0*4 + 2] = 1.0f;

      transition_matrix[1*4 + 1] = 1.0f;
			transition_matrix[1*4 + 3] = 1.0f;

      transition_matrix[2*4 + 2] = 1.0f;
			transition_matrix[3*4 + 3] = 1.0f;


      TIADALG_KF_Init(&el_prms->kf[0],2,4,transition_matrix);
      TIADALG_KF_Init(&el_prms->kf[1],2,4,transition_matrix);
      TIADALG_KF_Init(&el_prms->kf[2],2,4,transition_matrix);

      memset_float(transition_matrix, 0.0f, 9*9); /*here It is assumed of size 9x9*/

      /*Constant acceleration model*/
      transition_matrix[0*9 + 0] = 1.0f;
      transition_matrix[0*9 + 3] = measurementInterval;
			transition_matrix[0*9 + 6] = 0.5f*pow(measurementInterval, 2);

      transition_matrix[1*9 + 1] = 1.0f;
			transition_matrix[1*9 + 4] = measurementInterval;
			transition_matrix[1*9 + 7] = 0.5f*pow(measurementInterval, 2);

      transition_matrix[2*9 + 2] = 1.0f;
			transition_matrix[2*9 + 5] = measurementInterval;
			transition_matrix[2*9 + 8] = 0.5f*pow(measurementInterval, 2);

      transition_matrix[3*9 + 3] = 1.0f;
			transition_matrix[3*9 + 6] = measurementInterval;

      transition_matrix[4*9 + 4] = 1.0f;
			transition_matrix[4*9 + 7] = measurementInterval;

      transition_matrix[5*9 + 5] = 1.0f;
			transition_matrix[5*9 + 8] = measurementInterval;

      transition_matrix[6*9 + 6] = 1.0f;
      transition_matrix[7*9 + 7] = 1.0f;
      transition_matrix[8*9 + 8] = 1.0f;


      TIADALG_KF_Init(&el_prms->kf[3],3,9,transition_matrix);
      memset_float((float*)&el_prms->prev_est_pose,0.0f,6);
      memset_float((float*)&el_prms->prev_filt_pose,0.0f,6);
      memset_float((float*)&el_prms->cur_filt_pose,0.0f,6);
      memset_float((float*)&el_prms->iir_filt_pose,0.0f,6);
    }

    EL_TI_pose_filtering(rot_matrix_trans, translation,
                        (float (*)[3])out_args->rotation, out_args->angle, out_args->translation,
                        (el_prms->frame_idx==0), (solve_pnp_status==TIADALG_PROCESS_SUCCESS),el_prms->kf,
                        &el_prms->prev_est_pose,&el_prms->prev_filt_pose,&el_prms->cur_filt_pose,&el_prms->iir_filt_pose);
  }

#ifdef ENABLE_PROFILE
  t9 = _TSC_read();
  acc5 = (t9-t8);
  printf("Total cycle consumed in pose filtering is %lld \n",acc5);
#endif

  el_prms->est_loc[0] = out_args->translation[0];
  el_prms->est_loc[1] = out_args->translation[1];
  el_prms->est_loc[2] = out_args->translation[2];

  el_prms->est_rot[0][0] = out_args->rotation[0];
  el_prms->est_rot[0][1] = out_args->rotation[1];
  el_prms->est_rot[0][2] = out_args->rotation[2];
  el_prms->est_rot[1][0] = out_args->rotation[3];
  el_prms->est_rot[1][1] = out_args->rotation[4];
  el_prms->est_rot[1][2] = out_args->rotation[5];
  el_prms->est_rot[2][0] = out_args->rotation[6];
  el_prms->est_rot[2][1] = out_args->rotation[7];
  el_prms->est_rot[2][2] = out_args->rotation[8];

#ifdef TIADALG_VL_DEBUG_SOLVE_PNP
  printf("Filtered output is T :: %f, %f, %f \n\n\n", el_prms->est_loc[0],el_prms->est_loc[1],el_prms->est_loc[2]);
#endif

  el_prms->frame_idx = 1;

  return status;
}

/*--------------------------------------------------------------------------*/
/* This is the function to do activation of algorithm. All scratch memory   */
/* are restored in this.                                                    */
/* Refer algActivate function for details in ialg.h file                    */
/*--------------------------------------------------------------------------*/
void  EL_TI_activate(IALG_Handle handle)
{
  EL_TI_Handle algHandle = (EL_TI_Handle)(handle);
  uint32_t i;

  /*------------------------------------------------------------------------*/
  /* Do Activation only if it is deactivated or just after creation         */
  /*------------------------------------------------------------------------*/
  if(algHandle->algState == ALG_NOT_ACTIVE)
  {
    for(i= 1; i < algHandle->numMemRecs; i++)
    {
      /* Do Nothing */
    }
    algHandle->algState = ALG_ACTIVE;
  }

  return;
}

/*--------------------------------------------------------------------------*/
/* This is the function to do de-activation of algorithm. All scratch memory*/
/* are stored to external memory in this.                                   */
/* Refer algDeActivate function for details in ialg.h file                  */
/*--------------------------------------------------------------------------*/
void  EL_TI_deactivate(IALG_Handle handle)
{
  EL_TI_Handle algHandle = (EL_TI_Handle)(handle);
  uint32_t i;
  /*------------------------------------------------------------------------*/
  /* Do De-activation only if it is deactivated or just after creation      */
  /*------------------------------------------------------------------------*/
  if(algHandle->algState == ALG_ACTIVE)
  {
    for(i= 1; i < algHandle->numMemRecs; i++)
    {
      /* Do Nothing */
    }
    algHandle->algState = ALG_NOT_ACTIVE;
  }
  return;
}

/*--------------------------------------------------------------------------*/
/* This is the function which user can call to get back the memory given to */
/* algorithm.                                                               */
/* Refer algFree function for details in ialg.h file                        */
/*--------------------------------------------------------------------------*/
int32_t EL_TI_free(IALG_Handle handle, IALG_MemRec *memRec)
{
  int32_t status            = IALG_EOK;
  EL_TI_Handle algHandle = (EL_TI_Handle)(handle);

  memcpy(memRec, algHandle->memRec, sizeof(IALG_MemRec)*algHandle->numMemRecs);

  return status;
}
