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
*  @brief     This file implements routines for pose filtering
*
*
*  @version 1.0 (june 2019) : Base version.
*
*/
#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#pragma RESET_MISRA ("required")

#include "tiadalg_vl_alg_int.h"

static void getRotationMatFromOrientation(float orientation[3], float rotationMat[3][3]);
static void matrix3x3Multipy(float matrixA[3][3], float matrixB[3][3], float matrixOut[3][3]);

static void matrix3x3Multipy(float matrixA[3][3], float matrixB[3][3], float matrixOut[3][3])
{
  float temp;
  int32_t i,j,k;

  for(i = 0; i < 3; i++){
    for(j = 0; j < 3; j++){
      temp = 0.0f;
      for(k = 0; k < 3; k++){
         temp +=  matrixA[i][k]*matrixB[k][j];
      }
      matrixOut[i][j] = temp;
    }
  }

}

/** Finds the solution of ax=b, where a & b are passed to this API, and result is
overwritten at 'b'. And inverse of the matrix 'a' is overwritten at 'a' only.
*/
/* scratch buffer requirement is 3*n*sizeof(int32_t) = 12*n bytes
*/

int32_t TIADALG_gaussJordanElimination_cn(float *a, int32_t n, float *b,int32_t aMatPitch, uint8_t* scratch)
{
  /* re-implemented from Numerical Recipes in C */
  int32_t *indxc;
  int32_t *indxr;
  int32_t *ipiv;
  int32_t i,j,k,l,ll;
  float big,dum,pivinv;
  int32_t col = 0;
  int32_t row = 0;
  int32_t m = 1;
  /* scratch buffer requirement is 3*n*sizeof(int32_t) = 12*n bytes
  */
  indxc=(int32_t *)scratch;
  indxr=(int32_t *)(indxc + n);
  ipiv =(int32_t *)(indxr + n);

  for (j=0;j<n;j++)
  {
    ipiv[j]=0;
  }
  for (i=0;i<n;i++)
  {
    big=0.0;
    for (j=0;j<n;j++)
    {
      if (ipiv[j] != 1) // skip the row if already searched
      {
        for (k=0;k<n;k++)
        {
          if (ipiv[k] == 0)
          {
            if (fabs(a[j*aMatPitch + k]) >= big)
            {
              big= (float) fabs(a[j*aMatPitch + k]);
              row=j;
              col=k;
            }
          }
          else if (ipiv[k] > 1)
          {
            return false;
          }
        }//for (k=0;k<n;k++)
      }//if (ipiv[j] != 1)
    }//for (j=0;j<n;j++)


    ++(ipiv[col]);

    if (row != col)
    {
      for (l=0;l<n;l++)
      {
        SWAP_ME(a[row*aMatPitch + l],a[col*aMatPitch + l])
      }
      for (l=0;l<m;l++)
      {
        SWAP_ME(b[row],b[col])
      }
    }
    indxr[i]=row;
    indxc[i]=col;
    if (a[col*aMatPitch + col] == 0.0)
    {
      return false;
    }
    pivinv=TIADALG_oneByXF32(a[col*aMatPitch + col]);

    a[col*aMatPitch + col]=1.0;
    for (l=0;l<n;l++)
    {
      a[col*aMatPitch + l] *= pivinv;
    }

    for (l=0;l<m;l++)
    {
      b[col] *= pivinv;
    }
    for (ll=0;ll<n;ll++)
    {
      if (ll != col)
      {
        dum=a[ll*aMatPitch + col];
        a[ll*aMatPitch + col]=0.0;
        for (l=0;l<n;l++)
        {
          a[ll*aMatPitch + l] -= a[col*aMatPitch + l]*dum;
        }
        for (l=0;l<m;l++)
        {
          b[ll]-= b[col]*dum;
        }
      }
    }
  }
  for (l=n-1;l>=0;l--)
  {
    if (indxr[l] != indxc[l])
    {
      for (k=0;k<n;k++)
      {
        SWAP_ME(a[k*aMatPitch + indxr[l]],a[k*aMatPitch + indxc[l]]);
      }
    }
  }


  return true;
}

void getOrientationFromRotationMat(float rotationMat[3][3], float orientation[3])
{

  orientation[0] = atan2(rotationMat[2][1],  rotationMat[2][2]);
  //Added after Soyeb's feedback and based on following code from his python script
  orientation[1] = atan2(-rotationMat[2][0], mmax(0.0000001f,(float)(sqrt(pow((float)(rotationMat[0][0]), 2.f) + pow((float)(rotationMat[1][0]), 2.f)))));
  //pitch_X = math.atan2(rot_mat[2, 1], rot_mat[2, 2])*180.0 / math.pi
  //head_Y = math.atan2(-rot_mat[2, 0], sy)*180.0 / math.pi
  //roll_Z = math.atan2(rot_mat[1, 0], rot_mat[0, 0])*180.0 / math.pi
  orientation[2] = atan2(rotationMat[1][0], rotationMat[0][0]);

}


static void getRotationMatFromOrientation(float orientation[3], float rotationMat[3][3])
{

  float mat[3][3][3];
  float tmp[3][3];

  memset_float((float*)mat,0.0f,3*3*3);
  memset_float((float*)tmp,0.0f,3*3);
  int32_t i,j;

  for (i = 0; i <= 2; i++) {
         for (j = 0; j <= 2; j++) {
               mat[i][j][j] = 1;
         }
  }
  mat[0][1][1] = cos(orientation[0]);
  mat[0][2][2] = cos(orientation[0]);
  mat[0][1][2] = -sin(orientation[0]);
  mat[0][2][1] = sin(orientation[0]);

  mat[1][0][0] = cos(orientation[1]);
  mat[1][2][2] = cos(orientation[1]);
  mat[1][0][2] = sin(orientation[1]);
  mat[1][2][0] = -sin(orientation[1]);

  mat[2][0][0] = cos(orientation[2]);
  mat[2][1][1] = cos(orientation[2]);
  mat[2][0][1] = -sin(orientation[2]);
  mat[2][1][0] = sin(orientation[2]);

  matrix3x3Multipy(mat[1], mat[0], tmp);
  matrix3x3Multipy(mat[2], tmp, rotationMat);

}

void TIADALG_KF_Init(EL_TI_Kalaman_filter *KF, int32_t num_meas, int32_t num_state, float* tansition_matrix)
{
  int32_t i;

  memset_float((float*)KF,0.0f,sizeof(EL_TI_Kalaman_filter)>>2);

  KF->num_measurement = num_meas;
  KF->num_state       = num_state;

  memcpy(KF->transition,tansition_matrix,KF->num_state*KF->num_state*sizeof(float));

  KF->scaleFactor = 0.5f;

  /* initialize matrices for both optimized and natural c versions */
  for( i=0; i < KF->num_state; i++ )
  {
    KF->errorCov[i*KF->num_state + i]            = 1.0f;
    KF->processNoiseCov[i*KF->num_state + i]     = 0.00001f;

    if(i < KF->num_measurement){
      KF->measurement[i*KF->num_state + i]         = 1.0f;
      KF->measurementNoiseCov[i*KF->num_measurement + i] = 0.0001f;
    }
  }
}

void TIADALG_KF_Predict_F32(EL_TI_Kalaman_filter *KF)
{
    int32_t     i, j, k, r1, c1, r2, c2;
    float    sum;

    float *   P = KF->errorCov;
    float *   Pnew = KF->predictedErrorCov;
    float *   Q = KF->processNoiseCov;
    float *   pTemp2 = KF->temp2;
    float *   A = KF->transition;
    float *   pTemp1 = KF->temp1;
    float *   X = KF->state;
    float *   Xnew = KF->predictedState;

    /* Estimate new state*/
    /* X1 = A * X*/
    r1 = KF->num_state;
    c1 = KF->num_state;
    r2 = KF->num_state;
    c2 = 1;

    for( i = 0; i < r1; i++ ) {
        sum =(float) 0;

        for( k = 0; k < c1; k++ ) {
            sum += (A[k + (i * c1)] * X[k]);
        }

        Xnew[i] = sum;
    }

    /* project error covariance matrix */
    /* P1 = A*P*A' + Q;                */
    /* A*P [6x6 * 6x6] = 6x6           */
    r1 = KF->num_state;
    c1 = r1;
    r2 = KF->num_state;
    c2 = r2;

    for( i = 0; i < r1; i++ ) {

        for( j = 0; j < c2; j++ ) {
            sum = (float)0;

            for( k = 0; k < c1; k++ ) {
                sum += (A[k + (i * c1)] * P[j + (k * c2)]);
            }

            pTemp1[j + (i * c2)] = sum;
        }
    }

    /* (A*P)*A' [6x6 * 6x6] = 6x6*/

    r1 = KF->num_state;
    c1 = r1;
    r2 = KF->num_state;
    c2 = r2;

    for( i = 0; i < r1; i++ ) {

        for( j = 0; j < r2; j++ ) {
            sum =(float) 0;

            for( k = 0; k < c1; k++ ) {
                sum += (pTemp1[k + (i * c1)] * A[k + (j * c2)]);
            }

            pTemp2[j + (i * r2)] = sum;
        }
    }

    /* (A*P*A') + Q*/

    for( i=0; i < (r1 * c2); i++ ) {
        Pnew[i] = pTemp2[i] + Q[i];
    }
}


void TIADALG_KF_Correct_F32(EL_TI_Kalaman_filter *KF, const float * Z, float * Residual)
{
    int32_t     i, j, k, r1, c1, r2, c2;
    float    sum;
    int32_t     numEl;
    float    factor = 1;
    uint8_t scratch[12*KF_MAXIMUM_STATE];
    float b[KF_MAXIMUM_MEASUREMENT]; /*dummy varaible for matrix inverse*/

    float *   X = KF->state;
    float *   Xnew = KF->predictedState;
    float *   P = KF->errorCov;
    float *   Pnew = KF->predictedErrorCov;
    float *   H = KF->measurement;
    float *   R = KF->measurementNoiseCov;
    float *   pTemp1 = KF->temp1;
    float *   pTemp2 = KF->temp2;
    float *   pTemp3 = KF->temp3;
    float *   K = KF->kalmanGain;


    /* kalman gain*/
    /*K = P1*H'; [6x6 * 6x4] = 6x4*/
    r1 = KF->num_state;
    c1 = r1;
    r2 = KF->num_measurement;
    c2 = KF->num_state;

    for( i = 0; i < r1; i++ ) {
        for( j = 0; j < r2; j++ ) {
            sum = (float)0.0;

            for( k = 0; k < c1; k++ ) {
                sum += (Pnew[k + (i * c1)] * H[k + (j * c2)]);
            }

            pTemp1[j + (i * r2)] = sum;
        }
    }

    /*pTemp1 = H*(P1*H') [4x6 * 6x4] = 4x4*/
    r1 = KF->num_measurement;
    c1 = KF->num_state;
    r2 = KF->num_state;
    c2 = KF->num_measurement;

    for( i = 0; i < r1; i++ ) {
        for( j = 0; j < c2; j++ ) {
            sum = (float)0.0;

            for( k = 0; k < c1; k++ ) {
                sum += (H[k + (i * c1)] * pTemp1[j + (k * c2)]);
            }

            pTemp2[j + (i * c2)] = sum;
        }
    }

    /*pTemp2 = (H*P1*H') + R*/
    numEl = r1 * c2;

    for( i=0; i < numEl; i++ ) {
        pTemp2[i] = pTemp2[i] + R[i];
    }

    /* compute inverse of pTemp2*/
    /* pTemp2 is a 4x4 matrix*/
    for( i=0; i < numEl; i++ ) {
        pTemp2[i] = pTemp2[i] / (2 * KF->scaleFactor);
    }

    factor=(2 * KF->scaleFactor);

    memcpy(pTemp3,pTemp2,numEl*sizeof(float));

    TIADALG_gaussJordanElimination_cn(pTemp3,KF->num_measurement,b,KF->num_measurement,scratch);

    /* K = K* inv((H*P1*H') + R)*/
    r1 = KF->num_state;
    c1 = KF->num_measurement;
    r2 = KF->num_measurement;
    c2 = KF->num_measurement;

    for( i = 0; i < r1; i++ ) {
        for( j = 0; j < c2; j++ ) {
            sum =(float) 0.0;

            for( k = 0; k < c1; k++ ) {
                sum += (pTemp1[k + (i * c1)] * pTemp3[j + (k * c2)]);
            }

            K[j + (i * c2)] = sum/factor;
        }
    }

    /* pTemp1 = H*X1*/
    r1 = KF->num_measurement;
    c1 = KF->num_state;
    r2 = KF->num_state;
    c2 = 1;
    /*  DSP_mat_mul(KF->measurement, r1,
    c1, KF->predictedState, c2, KF->temp1, 0);*/

    for( i = 0; i < r1; i++ ) {
        sum = (float)0.0;

        for( k = 0; k < c1; k++ ) {
            sum += (H[k + (i * c1)] * Xnew[k]);
        }

        pTemp1[i] = sum;
    }

    /* Residual = Z - H*X1*/

    for( i=0; i < KF->num_measurement; i++ ) {
        Residual[i] = Z[i] - pTemp1[i];
    }

    /* K*Residual*/
    r1 = KF->num_state;
    c1 = KF->num_measurement;
    r2 = KF->num_measurement;
    c2 = 1;

    for( i = 0; i < r1; i++ ) {
        sum = (float)0.0;

        for( k = 0; k < c1; k++ ) {
            sum += (K[k + (i * c1)] * Residual[k]);
        }

        pTemp2[i] = (sum);
    }

    for( i=0; i < r1; i++ ) {
        X[i] = Xnew[i] + pTemp2[i];
    }

    /* update error covariance*/
    /* K*H*/
    r1 = KF->num_state;
    c1 = KF->num_measurement;
    r2 = KF->num_measurement;
    c2 = KF->num_state;

    for( i = 0; i < r1; i++ ) {
        for( j = 0; j < c2; j++ ) {
            sum = (float)0.0;

            for( k = 0; k < c1; k++ ) {
                sum += (K[k + (i * c1)] * H[j + (k * c2)]);
            }

            pTemp2[j + (i * c2)] = sum;
        }
    }

    /* (K*H)*Pnew*/
    r1 = KF->num_state;
    c1 = KF->num_state;
    r2 = KF->num_state;
    c2 = KF->num_state;

    for( i = 0; i < r1; i++ ) {
        for( j = 0; j < c2; j++ ) {
            sum = (float)0.0;

            for( k = 0; k < c1; k++ ) {
                sum += (pTemp2[k + (i * c1)] * Pnew[j + (k * c2)]);

            }

            pTemp3[j + (i * c2)] = sum;
        }
    }

    /* Pnew - ((K*H)*Pnew)*/

    for( i=0; i < (r1 * c2); i++ ) {
        P[i] = Pnew[i] - pTemp3[i];
    }
}

int32_t EL_TI_pose_filtering(float in_rot[3][3], float in_t[3], float out_rot_mat[3][3], float out_rot_angle[3], float out_t[3],
                          int32_t is_first_frame, int32_t solve_pnp_status, EL_TI_Kalaman_filter* kf,
                          EL_TI_Pose* prev_est_pose, EL_TI_Pose* prev_filt_pose,EL_TI_Pose* cur_filt_pose,EL_TI_Pose* iir_filt_pose)
{
  EL_TI_Pose cur_est_pose; /* this is current measurement*/
  float residual[KF_MAXIMUM_STATE];

  getOrientationFromRotationMat(in_rot, &cur_est_pose.elem[3]);

  cur_est_pose.elem[0] = in_t[0];
  cur_est_pose.elem[1] = in_t[1];
  cur_est_pose.elem[2] = in_t[2];

  if (is_first_frame != 0x0)
  {
    /*First frame scenario*/
    /*past estimate, past filtered estimate and current estimate should be assigned to current measurement*/
    *prev_est_pose  = cur_est_pose; /* prev_est_pose is acctually previous measurement*/
    *prev_filt_pose = cur_est_pose;
    *cur_filt_pose  = cur_est_pose;
#ifdef ENABLE_KF_LOG
    printf("KF: first frame flow has entered \n");
#endif
  }
  else if (solve_pnp_status != 0x0) {
    /*Solve pnp success scenario. Most common use case*/
    float estimatedMotionMag = 0.0f;
    float expectedMotionMag = 0.0f;
    EL_TI_Pose estimatedMotion;
    EL_TI_Pose expectedMotion;
    int32_t i;

    for (i = 0; i <= 2; i++) {
      estimatedMotion.elem[i] = cur_est_pose.elem[i] - prev_est_pose->elem[i];
      expectedMotion.elem[i] = 0.95f * iir_filt_pose->elem[i] + 0.05f * estimatedMotion.elem[i];
      estimatedMotionMag += estimatedMotion.elem[i] * estimatedMotion.elem[i];
      expectedMotionMag += expectedMotion.elem[i] * expectedMotion.elem[i];
    }

    for (i = 3; i <= 5; i++) {
      if (msign(cur_est_pose.elem[i]) != msign(prev_est_pose->elem[i])) {
        float temp = fabs(cur_est_pose.elem[i] - prev_est_pose->elem[i]);
        estimatedMotion.elem[i] = (msign(cur_est_pose.elem[i]) ? 1 : -1) * mmin(temp, 2 * TIADALG_PI - temp);
        expectedMotion.elem[i] = 0.95f * iir_filt_pose->elem[i] + 0.05f * estimatedMotion.elem[i];
      } else {
        estimatedMotion.elem[i] = cur_est_pose.elem[i] - prev_est_pose->elem[i];
        expectedMotion.elem[i] = 0.95f * iir_filt_pose->elem[i] + 0.05f * estimatedMotion.elem[i];
      }
    }

    *iir_filt_pose = expectedMotion;
    if ((estimatedMotionMag > 25 * expectedMotionMag) && (estimatedMotionMag > 1)) {
      for (i = 0; i <= 5; i++) {
        prev_filt_pose->elem[i] = prev_filt_pose->elem[i] + expectedMotion.elem[i];
      }
#ifdef ENABLE_KF_LOG
      printf("KF: Motion magnitude is high \n");
#endif
    } else {
      *prev_filt_pose = cur_est_pose;
    }
    /*Angle filtering*/
    for (i = 0; i <= 2; i++) {
      float measurement1[2];
      measurement1[0] = sin(prev_filt_pose->elem[3 + i]);
      measurement1[1] = cos(prev_filt_pose->elem[3 + i]);

      TIADALG_KF_Predict_F32(&kf[i]);
      TIADALG_KF_Correct_F32(&kf[i],measurement1,residual);

     float tmpOrientation = atan2(kf[i].state[0], kf[i].state[1]);

      prev_filt_pose->elem[3 + i] = tmpOrientation;
    }

    /*Position filtering*/
    float measurement[3];

    measurement[0] = prev_filt_pose->elem[0]; // x
    measurement[1] = prev_filt_pose->elem[1]; // y
    measurement[2] = prev_filt_pose->elem[2]; // z

    TIADALG_KF_Predict_F32(&kf[3]);
    TIADALG_KF_Correct_F32(&kf[3],measurement,residual);

    prev_filt_pose->elem[0] = kf[3].state[0];
    prev_filt_pose->elem[1] = kf[3].state[1];
    prev_filt_pose->elem[2] = kf[3].state[2];

    *prev_est_pose = cur_est_pose;

    *cur_filt_pose =  *prev_filt_pose;


  } else {
    /*Solve pnp fail scenario. */
    int32_t i;
    for (i = 0; i <= 5; i++) {
      prev_filt_pose->elem[i] = prev_filt_pose->elem[i] + iir_filt_pose->elem[i];
    }

    *cur_filt_pose = *prev_filt_pose;
#ifdef ENABLE_KF_LOG
    printf("KF: Solve Pnp fail scenario \n");
#endif
  }

  getRotationMatFromOrientation(&cur_filt_pose->elem[3],out_rot_mat);

  out_rot_angle[0] = cur_filt_pose->elem[3];
  out_rot_angle[1] = cur_filt_pose->elem[4];
  out_rot_angle[2] = cur_filt_pose->elem[5];

  out_t[0] = cur_filt_pose->elem[0];
  out_t[1] = cur_filt_pose->elem[1];
  out_t[2] = cur_filt_pose->elem[2];

  return (TIADALG_PROCESS_SUCCESS);

}
