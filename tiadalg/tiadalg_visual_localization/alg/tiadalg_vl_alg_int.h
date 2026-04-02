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
  ****************************************************************************
  *  @file       fc_alg_int.h
  *
  *  @brief      This file defines the private functions for forward collision
  *              warning algorithm
  *  @version 1.0 (Oct 2015) : Base version.
  ****************************************************************************
 */


#ifndef _TIADALG_EL_ALG_INT_H_
#define _TIADALG_EL_ALG_INT_H_

/*--------------------------------------------------------------------------*/
/* This header file inclusion is only for XDAIS module. It accepts specific */
/* data types defined externally                                            */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>

#include "tiadalg_interface.h"
#include <./../common/tiadalg_alg_int_interface.h>

#ifdef __cplusplus
extern "C" {
#endif

//#define ENABLE_PROFILE

/**
 *  @enum   eMemrecs
 *  @brief  Memory records for alogirthm handle.
 *          Alg Handle should be first entry in this enum table as test app is
 *          expecting it to be first entry
 */
typedef enum
{
  ALG_HANDLE_MEMREC,

  EL_ALG_BUFDESC_VOXEL_INFO_MEM, /*Persistant voxel info memory*/
  EL_ALG_BUFDESC_3D_POINTS_MEM, /*Persistant 3d points memory*/
  EL_ALG_BUFDESC_POINTS_DESC_MEM, /*Persistant points descriptor memory*/

  EL_ALG_EXT_PERSIST_MEM, /*Persistant External Memory*/
  EL_ALG_EXT_SCRATCH_MEM, /*Scratch External Memory*/
  EL_ALG_L2_SCRATCH_MEM, /*All L2 memory is assumed to be scratch*/
  NUM_MEMRECS
} eMemrecs;

/**
 *  @enum   eAlgState
 *  @brief  State of EL algorithm
 */
typedef enum
{
  ALG_NOT_ACTIVE,
  ALG_ACTIVE
} eAlgState;

typedef struct
{
  //float t[3];
  //float euler_angle[3];
  float elem[6];/**/
}EL_TI_Pose;

/* Observation vector index for Kalman Filter */
#define KF_MAXIMUM_MEASUREMENT (3)

/* State vector index for Kalman Filter */
#define KF_MAXIMUM_STATE (9)

  /**
  @struct VLIB_kalmanFilter_3x6_F32

  @brief  Structure defining the state of the Kalman filter, 3x6 (3-Dimension
          Observation x 6-Dimension State Vectors) variant

  @param  transition
          State transition matrix, A.

  @param  errorCov
          A priori error covariance matrix, P.

  @param  predictedErrorCov
          Predicted error covariance matrix, P1.

  @param  state
          State of the process, X.

  @param  predictedState
          Predicted state of the process, X1.

  @param  measurement
          Measurement matrix (relating state to measurement), H.

  @param  processNoiseCov
          Process noise covariance matrix, Q

  @param  measurementNoiseCov
          Measurement noise covariance matrix, R.

  @param  kalmanGain
          Kalman gain, K.

  @param  temp1
          Temporary buffer for intermediate results.

  @param  temp2
          Temporary buffer for intermediate results.

  @param  temp3
          Temporary buffer for intermediate results.

  @param  scaleFactor
          Scales the matrix M = (H*P1*H' + R) to ensure that its inverse does
          not overflow 32 bits.
          The scaling is done by right shifting each element of M by the
          quantity assigned to scaleFactor. The computed inverse is then
          scaled back to ensure the correct result based on the identity
          inv(M) = inv(M/k)/k  .
  */
typedef struct
{
  float transition[KF_MAXIMUM_STATE * KF_MAXIMUM_STATE];
  float errorCov[KF_MAXIMUM_STATE * KF_MAXIMUM_STATE];
  float predictedErrorCov[KF_MAXIMUM_STATE * KF_MAXIMUM_STATE];
  float state[KF_MAXIMUM_STATE];
  float predictedState[KF_MAXIMUM_STATE];
  float measurement[KF_MAXIMUM_MEASUREMENT * KF_MAXIMUM_STATE];
  float processNoiseCov[KF_MAXIMUM_STATE * KF_MAXIMUM_STATE];
  float measurementNoiseCov[KF_MAXIMUM_MEASUREMENT * KF_MAXIMUM_MEASUREMENT];
  float kalmanGain[KF_MAXIMUM_STATE * KF_MAXIMUM_MEASUREMENT];
  float temp1[KF_MAXIMUM_STATE * KF_MAXIMUM_STATE];
  float temp2[KF_MAXIMUM_STATE * KF_MAXIMUM_STATE];
  float temp3[KF_MAXIMUM_STATE * KF_MAXIMUM_STATE];
  float scaleFactor;
  int32_t num_measurement;
  int32_t num_state;
} EL_TI_Kalaman_filter;

/*algorithm parameters in persistant memory*/
typedef struct
{
  /*CREATE TIME PARAMETERS    */
  uint8_t* l2_scratch;
  uint8_t* ext_scratch;
  tiadalg_voxel_info* voxel_info;
  float* map_3d_points;
  VL_DESC_DATA_TYPE* map_desc;
  int32_t frame_idx;

  /*PROCESS TIME PARAMETERS    */
  int32_t width;
  int32_t height;
  uint8_t *in_img;
  tiadalg_ext_feat* ext_feat;
  VL_DESC_DATA_TYPE* ext_desc;
  float est_loc[3];
  float est_rot[3][3];

  /* POSE FILTERING RELATED */
  EL_TI_Pose  prev_est_pose;  /* previous measurement       */
  EL_TI_Pose  prev_filt_pose; /* previous filtered estimate */
  EL_TI_Pose  cur_filt_pose;  /* this is not persistant data*/
  EL_TI_Pose  iir_filt_pose;  /* iir filtered pose          */

  EL_TI_Kalaman_filter kf[4]; /*3 kalman filter for 3 euler angles and 1 for translation parameters*/

}EL_TI_Prms;

/**
 @ingroup Ego Localization Warning Module
 @struct  EL_TI_Obj
 @brief   This structure is the main handle of EL Module

 @params  ivision
          All public function pointers

 @params  algState
          State of algorithm to indicate

 @params  numMemRecs
          Number of memory records

 @params  memRec
          Array of memory records
 */
typedef struct
{
  const IVISION_Fxns* ivision;
  uint8_t             algState;
  uint32_t            numMemRecs;
  IALG_MemRec         memRec[NUM_MEMRECS];
  TIADALG_el_create_params algCreatePrms;
  EL_TI_Prms ti_prms;

} EL_TI_Obj;

typedef EL_TI_Obj *EL_TI_Handle;


/*---------------------------------------------------------------------------*/
/* IALG functions                                                            */
/* Refer XDAIS ialg.h file for details on these functions                    */
/*---------------------------------------------------------------------------*/
int32_t EL_TI_numAlloc(void);
int32_t EL_TI_alloc(const IALG_Params *params,
                        struct IALG_Fxns **parentFxns, IALG_MemRec *memRec);
int32_t EL_TI_init(IALG_Handle handle, const IALG_MemRec *memRec,
                       IALG_Handle parent, const IALG_Params *params);
void  EL_TI_activate(IALG_Handle handle);
void  EL_TI_deactivate(IALG_Handle handle);
int32_t EL_TI_free(IALG_Handle handle, IALG_MemRec *memRec);
int32_t EL_TI_control(IVISION_Handle handle, IALG_Cmd cmd,
                          const IALG_Params *inParams, IALG_Params *outParams);
int32_t EL_TI_process(IVISION_Handle handle, IVISION_InBufs *inBufs,
                          IVISION_OutBufs *outBufs, IVISION_InArgs *inArgs,
                          IVISION_OutArgs *outArgs);

int32_t EL_TI_pose_filtering(float in_rot[3][3], float in_t[3], float out_rot_mat[3][3], float out_rot_angle[3], float out_t[3],
  int32_t is_first_frame, int32_t solve_pnp_status, EL_TI_Kalaman_filter* kf,
  EL_TI_Pose* prev_est_pose, EL_TI_Pose* prev_filt_pose,EL_TI_Pose* cur_filt_pose,EL_TI_Pose* iir_filt_pose);

void TIADALG_KF_Correct_F32(EL_TI_Kalaman_filter *KF, const float * Z, float * Residual);
void TIADALG_KF_Predict_F32(EL_TI_Kalaman_filter *KF);

void TIADALG_KF_Init(EL_TI_Kalaman_filter *KF, int32_t num_meas, int32_t num_state, float* tansition_matrix);

int32_t TIADALG_gaussJordanElimination_cn(float *a, int32_t n, float *b,int32_t aMatPitch, uint8_t* scratch);

static void inline memset_float(float* dst, float val, int32_t num)
{
  int32_t i;
  for(i =0 ;i < num; i++){
    dst[i] = val;
  }
}

void getOrientationFromRotationMat(float rotationMat[3][3], float orientation[3]);

#ifdef __cplusplus
}
#endif


#endif /* _TIADALG_EL_ALG_INT_H_ */
