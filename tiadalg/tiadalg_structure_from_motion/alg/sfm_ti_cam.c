/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2009-2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/

/**
*  @file        sfm_ti_cam.c
*  @author      Deepak Poddar
*  @brief       This file defines the XDAIS/iVISION functions for Feature classification applet
*  @version     0.0 (Jan 2014) : Base version.
*/

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#pragma RESET_MISRA ("required")

#include "sfm_ti_alg_int.h"
#include "sfm_ti_cam.h"
#include "sfm_ti_matrixOps.h"

/** Copy the input camera extrinsic parameters into persistent memory area
* in circular fashion.
*/
void SFM_TI_copyCamPrmToCirCBuf(float* inCamExtPrm, uint16_t curBufIndx,
                                float* camExtPrm)
{
  float* pf32temp1;
  float* pf32temp2;
  uint32_t i;
  /* Copying the user given ext camera parameters into persistent area.
  This is maintained in circular buffer of size VLIB_TRIANG_MAX_POINTS_IN_TRACK.
  Maintained in persistent area because we need previous
  (VLIB_TRIANG_MAX_POINTS_IN_TRACK - 1 =5) frames extrinsic camera parameters
  */
  pf32temp2 = inCamExtPrm;
  pf32temp1 = (float*)&camExtPrm[curBufIndx*VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE + 0];

  for(i=0;i<(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_SIZE);i++) {
    pf32temp1[i] = pf32temp2[i];
  }
  pf32temp1[i + 0] = 0.0f;
  pf32temp1[i + 1] = 0.0f;
  pf32temp1[i + 2] = 0.0f;
  pf32temp1[i + 3] = 1.0f;

}
/** Get the normalized camera extrinsic parameters, and inverse also of
* that of the current camera parameters came in current process call
*/
void SFM_TI_getInvExtPrmMatrix(float*   camExtPrm,
                                   uint16_t curBufIdx,
                                   float*   camInvExtPrm,
                                   uint8_t* scratch)
{
  float* pf32temp1;
  float* pf32temp2;
  int32_t i;

  /* Source address
  */
  pf32temp2 = (float*)&camExtPrm[curBufIdx * VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];

  /* Destination addresses
  */
  pf32temp1 = (float*)&camInvExtPrm[curBufIdx * VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];


  /* Currently inverse calculating module does in place reverse calculation.
  Hence for calculating inverse first original matrix needs to be copied
  at inverse output location.
  */


  for(i=0;i<(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_SIZE + 4);i++) {
    pf32temp1[i] = pf32temp2[i];
  }

  /* Inverse of incoming P matrix and placed at another circular
  * buffer
  */
#ifdef DSP_INTRINSICS
  SFM_TI_gaussJordanElimination_ci(&camInvExtPrm[curBufIdx * VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE],
    4,
    (float*)scratch,
    4,
    scratch+16);
#else
  SFM_TI_gaussJordanElimination_cn(&camInvExtPrm[curBufIdx * VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE],
    4,
    (float*)scratch,
    4,
    scratch+16);
#endif

}
/** Multiply current input camera extrinsic parameter with camera intrinsic
* parameters and store in persistent circular memory
*/
void SFM_TI_mulIntExtPrm(float* camIntPrm, float* camExtPrm, float* normCamIntExtPrm)
{
  uint32_t i,j,k;

  for(i=0; i<VLIB_TRIANG_MAX_POINTS_IN_TRACK; i++){

    for(k=0; k<3; k++){
      for(j=0; j<4; j++){
        normCamIntExtPrm[k*4 + j] = 0.0f;
        normCamIntExtPrm[k*4 + j] += ((camIntPrm[k*3 + 0] * camExtPrm[0*4+j]) +
          (camIntPrm[k*3 + 1] * camExtPrm[1*4+j]) +
          (camIntPrm[k*3 + 2] * camExtPrm[2*4+j]));
      }
    }
    camExtPrm    += VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE;
    normCamIntExtPrm += VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE;
  }
}
/** Copy all the necessary camera parameters into internal memory before
triangulation loop starts
*/
void SFM_TI_copyCamPrmsToIntMem(sSfm_TI_L1DMem* sfmL1Prm, sSfm_TI_ExtMem* sfmExtPrm)
{
  uint32_t i,j;
  int16_t localBufIdx;
  float* pf32temp1;
  float* pf32temp2;


  /* Copy of external Parameters from DDR to L1D
  * Format of external parameters in DDR is that for latest frame
  * parameters are placed at index "curLargeBufIndx", and back frame's
  * parameters are placed at index "curLargeBufIndx - 1", "curLargeBufIndx -2" and so.
  * But after moving to L1D its format is changed, and latest frame parameters
  * are placed at index "0", and back frame's data is at index 1 , 2 etc.
  */
  localBufIdx = sfmExtPrm->curLargeBufIndx;

  pf32temp1 = &sfmExtPrm->camExtPrm[localBufIdx][0];
  pf32temp2 = &sfmL1Prm->camExtPrm[CAM_EXT_PRM_BUF_DEPTH - 1][0];

  for(i = 0; i < CAM_EXT_PRM_BUF_DEPTH; i++) {
    for(j = 0; j < VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE; j++){
      pf32temp2[j] = pf32temp1[j];
    }
    pf32temp2 -= VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE;

    localBufIdx --;
    if(localBufIdx == -1)
      localBufIdx = (CAM_EXT_PRM_BUF_DEPTH - 1);
    pf32temp1 = &sfmExtPrm->camExtPrm[localBufIdx][0];
  }

  /* Copy of inverse of external Parameters from DDR to L1D
  * Format of external parameters in DDR is that for latest frame
  * parameters are placed at index "curLargeBufIndx", and back frame's
  * parameters are placed at index "curLargeBufIndx - 1", "curLargeBufIndx -2".
  * But after moving to L1D its format is changed, and latest frame parameters
  * are placed at index "0", and back frame's data is at index 1 , 2 etc.
  */
  localBufIdx = sfmExtPrm->curLargeBufIndx;

  pf32temp1 = &sfmExtPrm->camInvExtPrm[localBufIdx][0];
  pf32temp2 = &sfmL1Prm->camInvExtPrm[CAM_EXT_PRM_BUF_DEPTH - 1][0];

  for(i = 0; i < CAM_EXT_PRM_BUF_DEPTH; i++) {
    for(j = 0; j < VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE; j++){
      pf32temp2[j] = pf32temp1[j];
    }
    pf32temp2 -= VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE;

    localBufIdx --;
    if(localBufIdx == -1)
      localBufIdx = (CAM_EXT_PRM_BUF_DEPTH - 1);
    pf32temp1 = &sfmExtPrm->camInvExtPrm[localBufIdx][0];
  }

  /* Copy of internal parameters (K) from DDR to internal memory
  */
  pf32temp1 = &sfmExtPrm->camIntPrm[0];
  pf32temp2 = &sfmL1Prm->camIntPrm[0];

  for(j = 0; j < VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_SIZE; j++){
    pf32temp2[j] = pf32temp1[j];
  }

  /* Copy of inverse of internal parameters (K_inv) from DDR to internal memory
  */
  pf32temp1 = &sfmExtPrm->camInvIntPrm[0];
  pf32temp2 = &sfmL1Prm->camInvIntPrm[0];

  for(j = 0; j < VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_SIZE; j++){
    pf32temp2[j] = pf32temp1[j];
  }

  memcpy(sfmL1Prm->firstCamExtPrm, sfmExtPrm->firstCamExtPrm, VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE*sizeof(float));

}
/** Calculates Fundamental matrix from camera parameters
*/

void SFM_TI_computeFmatFrmRT(float* curCamExtPrm,
                             float* prevCamInvExtPrm,
                             float* camInvIntPrm,
                             float* camIntPrm,
                             float* Fmat)
{
  float X[VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];
  float T3x3[9];
  float R3x3[9];
  float E[9];

  SFM_TI_matrixMultiply_4x4(curCamExtPrm,
    prevCamInvExtPrm,
    X);

  T3x3[0] =  0    ; T3x3[1] =  -X[11]; T3x3[2] =  X[7] ;
  T3x3[3] =  X[11]; T3x3[4] =  0     ; T3x3[5] =  -X[3];
  T3x3[6] =  -X[7]; T3x3[7] =  X[3]  ; T3x3[8] =  0    ;

  R3x3[0] =  X[0]; R3x3[1] =  X[1]; R3x3[2] =  X[2];
  R3x3[3] =  X[4]; R3x3[4] =  X[5]; R3x3[5] =  X[6];
  R3x3[6] =  X[8]; R3x3[7] =  X[9]; R3x3[8] =  X[10];

  SFM_TI_matrixMultiply_3x3(T3x3,R3x3,E);
  SFM_TI_multiplyByTranspose(camInvIntPrm,E,T3x3,3,3,3);
  SFM_TI_matrixMultiply_3x3(T3x3,camInvIntPrm,Fmat);

}

