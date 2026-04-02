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
*  @file        sfm_ti_matrixOps.c
*  @author      Deepak Poddar
*  @brief       This file defines the XDAIS/iVISION functions for Feature classification applet
*  @version     0.0 (Jan 2014) : Base version.
*/

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#pragma RESET_MISRA ("required")

#include "sfm_ti_alg_int.h"
#include "sfm_ti_math.h"
#include "sfm_ti_matrixOps.h"

#ifdef LOCAL_PROFILE
extern uint32_t          t0,t1,tCount,tAcc;
#endif

/** Finds the solution of ax=b, where a & b are passed to this API, and result is
overwritten at 'b'. And inverse of the matrix 'a' is overwritten at 'a' only.
*/
int32_t SFM_TI_gaussJordanElimination_cn(float *a, int32_t n, float *b,int32_t aMatPitch, uint8_t* scratch)
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
            if (_fabsf(a[j*aMatPitch + k]) >= big)
            {
              big= (float) _fabsf(a[j*aMatPitch + k]);
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
    pivinv=my_OneByX(a[col*aMatPitch + col]);

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
/** Finds the solution of ax=b, where a & b are passed to this API, and result is
overwritten at 'b'. And inverse of the matrix 'a' is overwritten at 'a' only.
No much optimization is done, only restricts are used here. Not called many times
hence not optimized.
*/
int32_t SFM_TI_gaussJordanElimination_ci(float a[restrict], int32_t n, float b[restrict],int32_t aMatPitch, uint8_t scratch[restrict])
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
            if (_fabsf(a[j*aMatPitch + k]) >= big)
            {
              big= (float) _fabsf(a[j*aMatPitch + k]);
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
    pivinv=my_OneByX(a[col*aMatPitch + col]);

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
/** Does C =  A * B, where all matrices are of 3x3 size
*/
void SFM_TI_matrixMultiply_3x3(float A[], float B[], float C[]) {
  int i, j, k = 0;
  float val;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      val = 0;
      for (k = 0; k < 3; k++)
        val += A[i * 3 + k] * B[k * 3 + j];

      C[i * 3 + j] = val;
    }
  }
}
/** Does C =  A * B, where all matrices are of 9x9 size
*/

void SFM_TI_matrixMultiply_9x9(float A[], float B[], float C[], int32_t rowSteps) {
  int i, j, k = 0;
  float val;

  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      val = 0;
      for (k = 0; k < 9; k++)
        val += A[i * rowSteps + k] * B[k * rowSteps + j];

      C[i * 9 + j] = val;
    }
  }
}
/** Does C =  A * B, where all matrices are of 4x4 size
*/
void SFM_TI_matrixMultiply_4x4(float A[], float B[], float C[]) {
  int i, j, k = 0;
  float val;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      val = 0;
      for (k = 0; k < 4; k++)
        val += A[i * 4 + k] * B[k * 4 + j];

      C[i * 4 + j] = val;
    }
  }
}
/** Does C =  A * B, where A is of size 4x4 and B of 4x1 size
*/
void SFM_TI_matrixMultiply_4x4By4x1(float A[], float B[], float C[]) {
  int i, j, k = 0;
  float val;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 1; j++) {
      val = 0;
      for (k = 0; k < 4; k++)
        val += A[i * 4 + k] * B[k * 1 + j];

      C[i * 1 + j] = val;
    }
  }
}
/* Does At*A multiplication
*/
void SFM_TI_multiplyByItsTranspose(float A[], float AtA[], uint16_t m, uint16_t n)
{
  uint32_t i,j,k;

  /* Size of the output matrix will be n x n
  */
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++) {
      AtA[i*VLIB_TRIANG_MAT_AROW + j] = 0.0;
      for(k =0;k <m; k++)
        AtA[i*VLIB_TRIANG_MAT_AROW + j] += A[k*n + i]*A[k*n + j];
    }
  }
}
#if 0
/* Does At*A multiplication
*/
void SFM_TI_multiplyByItsTranspose_ci(float A[restrict],
                                      float AtA[restrict],
                                      float B[restrict],
                                      float Atb[restrict],
                                      uint16_t* curTrackLength,
                                      uint16_t n)
{
  uint32_t l,k,m;
  __float2_t cur_1_0, cur_3_2, cur_5_4, cur_7_6, cur_9_8, cur_11_10;
  __float2_t next_1_0, next_3_2, next_5_4, next_7_6, next_9_8, next_11_10;
  float sum;

  int32_t offset[12] = {0, 0, 0, 1, 0, 2, 1, 1, 1, 2, 2, 2};
  int32_t curRow;
  int32_t nextRow;
  int32_t A_Pitch,AtA_Pitch, B_Pitch, Atb_Pitch, r, c;


  A_Pitch     = (VLIB_TRIANG_MAT_ROW*VLIB_TRIANG_MAT_COL);
  AtA_Pitch   = (2*VLIB_TRIANG_MAT_ROW-1)*VLIB_TRIANG_MAT_AROW;
  B_Pitch     = VLIB_TRIANG_MAT_COL;
  Atb_Pitch   = VLIB_TRIANG_MAT_AROW;

  /* '6' Here is not the VLIB_TRIANG_MAX_POINTS_IN_TRACK. 6 here is the total
  number of output needs to be evaluated for for one AtA
  matrix.
  */
  for(m = 0, k= 0, l=0; m < 6*NUM_TRACKS_TOGATHER; m++)
  {
    /* Size of the output matrix will be n x n
    */
    if(k == 6){
      l++;
      k = 0;
    }


    r = offset[2*k + 0];
    c = offset[2*k + 1];

    k++;

    curRow    = l*A_Pitch + r*VLIB_TRIANG_MAT_COL;
    nextRow   = l*A_Pitch + c*VLIB_TRIANG_MAT_COL;

    cur_1_0   = _amem8_f2(&A[curRow + 0]);
    cur_3_2   = _amem8_f2(&A[curRow + 2]);
    cur_5_4   = _amem8_f2(&A[curRow + 4]);
    cur_7_6   = _amem8_f2(&A[curRow + 6]);
    cur_9_8   = _amem8_f2(&A[curRow + 8]);
    cur_11_10 = _amem8_f2(&A[curRow + 10]);

    next_1_0   = _amem8_f2(&A[nextRow + 0]);
    next_3_2   = _amem8_f2(&A[nextRow + 2]);
    next_5_4   = _amem8_f2(&A[nextRow + 4]);
    next_7_6   = _amem8_f2(&A[nextRow + 6]);
    next_9_8   = _amem8_f2(&A[nextRow + 8]);
    next_11_10 = _amem8_f2(&A[nextRow + 10]);

    cur_1_0   = _dmpysp(cur_1_0,next_1_0);
    cur_3_2   = _dmpysp(cur_3_2,next_3_2);
    cur_5_4   = _dmpysp(cur_5_4,next_5_4);
    cur_7_6   = _dmpysp(cur_7_6,next_7_6);
    cur_9_8   = _dmpysp(cur_9_8,next_9_8);
    cur_11_10 = _dmpysp(cur_11_10,next_11_10);

    cur_1_0   = _daddsp(cur_1_0,cur_7_6);
    cur_3_2   = _daddsp(cur_3_2,cur_9_8);
    cur_5_4   = _daddsp(cur_5_4,cur_11_10);

    cur_1_0   = _daddsp(_daddsp(cur_1_0,cur_3_2),cur_5_4);

    //cur_1_0   = _daddsp(cur_1_0,cur_7_6);
    //cur_3_2   = _daddsp(cur_3_2,cur_5_4);
    //cur_5_4   = _daddsp(cur_5_4,cur_11_10);

    cur_1_0   = _daddsp(cur_1_0,cur_3_2);

    sum       = _lof2(cur_1_0) + _hif2(cur_1_0);

    AtA[l*AtA_Pitch + r*VLIB_TRIANG_MAT_AROW + c] = sum;
    //AtA[l*AtA_Pitch + c*VLIB_TRIANG_MAT_AROW + r] = sum;
  }
  for(m = 0, k= 0, l=0; m < 3*NUM_TRACKS_TOGATHER; m++)
  {
    /* Size of the output matrix will be n x n
    */
    if(k == 3){
      l++;
      k = 0;
    }
    r = k;

    curRow    = l*A_Pitch + r*VLIB_TRIANG_MAT_COL;
    nextRow   = l*B_Pitch;

    cur_1_0   = _amem8_f2(&A[curRow + 0]);
    cur_3_2   = _amem8_f2(&A[curRow + 2]);
    cur_5_4   = _amem8_f2(&A[curRow + 4]);
    cur_7_6   = _amem8_f2(&A[curRow + 6]);
    cur_9_8   = _amem8_f2(&A[curRow + 8]);
    cur_11_10 = _amem8_f2(&A[curRow + 10]);

    next_1_0   = _amem8_f2(&B[nextRow + 0]);
    next_3_2   = _amem8_f2(&B[nextRow + 2]);
    next_5_4   = _amem8_f2(&B[nextRow + 4]);
    next_7_6   = _amem8_f2(&B[nextRow + 6]);
    next_9_8   = _amem8_f2(&B[nextRow + 8]);
    next_11_10 = _amem8_f2(&B[nextRow + 10]);

    cur_1_0   = _dmpysp(cur_1_0,next_1_0);
    cur_3_2   = _dmpysp(cur_3_2,next_3_2);
    cur_5_4   = _dmpysp(cur_5_4,next_5_4);
    cur_7_6   = _dmpysp(cur_7_6,next_7_6);
    cur_9_8   = _dmpysp(cur_9_8,next_9_8);
    cur_11_10 = _dmpysp(cur_11_10,next_11_10);

    cur_1_0   = _daddsp(cur_1_0,cur_7_6);
    cur_3_2   = _daddsp(cur_3_2,cur_9_8);
    cur_5_4   = _daddsp(cur_5_4,cur_11_10);

    cur_1_0   = _daddsp(_daddsp(cur_1_0,cur_3_2),cur_5_4);

    sum       = _lof2(cur_1_0) + _hif2(cur_1_0);

    Atb[l*Atb_Pitch + k] = sum;
    k++;
  }

}
#endif
/** Does multiplication of At*B with given matrices A & B
*/
void SFM_TI_multiplyByTranspose(float A[], float B[], float out[], uint16_t ma, uint16_t na, uint16_t nb)
{
  uint32_t i,j,k;
  /* Size of the output matrix will be na x nb
  * A --> ma x na
  * B --> ma x nb
  * out --> A'*B = (na x ma)*(ma x nb) = na x nb
  */
  for(i = 0; i < na; i++){
    for(j = 0; j < nb; j++) {
      out[i*nb + j] = 0.0;
      for(k = 0;k < ma; k++)
        out[i*nb + j] += A[k*na + i]*B[k*nb + j];
    }
  }
}
#if 0
/** Does multiplication of At*B with given matrices A & B
*/
void SFM_TI_multiplyByTranspose_ci(float A[restrict], float B[restrict], float out[restrict], uint16_t ma, uint16_t na, uint16_t nb)
{
  uint32_t i,j;
  __float2_t f2temp1,f2temp2,f2temp3,f2temp5;

  /* Size of the output matrix will be ma x nb
  */
#pragma MUST_ITERATE(3,3,3)
  for(i = 0; i < na; i++){
#pragma MUST_ITERATE(1,1,1)
    for(j = 0; j < nb; j++) {
      f2temp5  = _ftof2(0.0,0.0);

      f2temp1 = _amem8_f2(&A[i*12 + 0]);
      f2temp2 = _amem8_f2(&B[j*1 + 0]);
      f2temp3 = _dmpysp(f2temp1,f2temp2);
      f2temp5 = _daddsp(f2temp5,f2temp3);

      f2temp1 = _amem8_f2(&A[i*12 + 2]);
      f2temp2 = _amem8_f2(&B[j*1 + 2]);
      f2temp3 = _dmpysp(f2temp1,f2temp2);
      f2temp5 = _daddsp(f2temp5,f2temp3);

      f2temp1 = _amem8_f2(&A[i*12 + 4]);
      f2temp2 = _amem8_f2(&B[j*1 + 4]);
      f2temp3 = _dmpysp(f2temp1,f2temp2);
      f2temp5 = _daddsp(f2temp5,f2temp3);

      f2temp1 = _amem8_f2(&A[i*12 + 6]);
      f2temp2 = _amem8_f2(&B[j*1 + 6]);
      f2temp3 = _dmpysp(f2temp1,f2temp2);
      f2temp5 = _daddsp(f2temp5,f2temp3);

      f2temp1 = _amem8_f2(&A[i*12 + 8]);
      f2temp2 = _amem8_f2(&B[j*1 + 8]);
      f2temp3 = _dmpysp(f2temp1,f2temp2);
      f2temp5 = _daddsp(f2temp5,f2temp3);

      f2temp1 = _amem8_f2(&A[i*12 + 10]);
      f2temp2 = _amem8_f2(&B[j*1 + 10]);
      f2temp3 = _dmpysp(f2temp1,f2temp2);
      f2temp5 = _daddsp(f2temp5,f2temp3);

      out[i*nb + j] = _lof2(f2temp5) + _hif2(f2temp5);
    }
  }
}
#endif
/**********************************************************************
* XLISP-STAT 2.1 Copyright (c) 1990, by Luke Tierney
* XLISP version 2.1, Copyright (c) 1989, by David Betz.
*
* Permission to use, copy, modify, distribute, and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation, and that the name of Luke Tierney and
* David Betz not be used in advertising or publicity pertaining to
* distribution of the software without specific, written prior
* permission.  Luke Tierney and David Betz make no representations
* about the suitability of this software for any purpose. It is
* provided "as is" without express or implied warranty.
*
* LUKE TIERNEY AND DAVID BETZ DISCLAIM ALL WARRANTIES WITH REGARD TO
* THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
* AND FITNESS, IN NO EVENT SHALL LUKE TIERNEY NOR DAVID BETZ BE
* LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
* DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
* WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
* ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
* OF THIS SOFTWARE.
*
* XLISP-STAT AUTHOR:
*               Luke Tierney
*               School of Statistics
*               University of Minnesota
*               Minneapolis, MN 55455
*               (612) 625-7843
*
*       Email Address:
*               internet: luke@umnstat.stat.umn.edu
*
* XLISP AUTHOR:
*              David Betz
*              P.O. Box 144
*              Peterborough, NH 03458
*              (603) 924-4145
**********************************************************************/
/*
URL http://archives.math.utk.edu/software/msdos/statistics/xlisp-stat/
http://www.public.iastate.edu/~dicook/JSS/paper/code/svd.c
*/

void SFM_TI_svdcmp(float* restrict Amat, int16_t rows, int16_t cols,int16_t pitch, float*  restrict Wmat, float* restrict vMat, float * restrict scratch)
{
  int16_t flag,i,its,j,jj,k,l,nm;
  float ftemp1, ftemp2, ftemp3, ftemp4, ftemp5, ftemp6, ftemp7;

  float anorm  = 0.0f;
  float ftemp8 = 0.0f;
  float scale  = 0.0f;

  l= 0;
  nm=0;

  for(i = 0; i < cols; i++)
  {
    l = i + 1;

    scratch[i] = scale * ftemp8;

    ftemp8     = 0.0f;
    scale      = 0.0;
    ftemp4     = 0.0f;

    if (i < rows)
    {
      for(k = i; k < rows; k++){
        scale += _fabsf(Amat[(k * pitch) + i]);
      }

      if (scale)
      {
        for(k = i; k < rows; k++)
        {
          Amat[(k * pitch) + i] = my_YByX(Amat[(k * pitch) + i],scale);
          ftemp4 += Amat[(k * pitch) + i] * Amat[(k * pitch) + i];
        }

        ftemp2 = Amat[(i * pitch) + i];
        ftemp8 = -SIGN(sqrtf(ftemp4), ftemp2);

        ftemp3 = (ftemp2 * ftemp8)-ftemp4;
        Amat[(i * pitch) + i] = (ftemp2 - ftemp8);

        if (i != cols - 1)
        {
          for(j = l; j < cols; j++)
          {
            for(ftemp4 = 0.0, k = i; k < rows; k++){
              ftemp4 += Amat[(k * pitch) + i] * Amat[(k * pitch) + j];
            }

            ftemp2 = my_YByX(ftemp4, ftemp3);

            for(k = i; k < rows; k++){
              Amat[(k * pitch) + j] += (ftemp2 * Amat[(k * pitch) + i]);
            }
          }
        }

        for(k = i; k < rows; k++){
          Amat[(k * pitch) + i] = Amat[(k * pitch) + i] * scale;
        }
      }
    }

    Wmat[i] = (scale * ftemp8);

    /* right-hand reduction */
    ftemp8     = 0.0f;
    scale = 0.0f;
    ftemp4     = 0.0f;

    if (i < rows && i != cols - 1){

      for(k = l; k < cols; k++){
        scale += _fabsf(Amat[(i * pitch) + k]);
      }

      if (scale){

        for(k = l; k < rows; k++){
          Amat[(i * pitch) + k] = my_YByX(Amat[(i * pitch) + k],scale);
          ftemp4 += Amat[(i * pitch) + k] * Amat[(i * pitch) + k];
        }

        ftemp2 = Amat[(i * pitch) + l];
        ftemp8 = -SIGN(my_SqrtX(ftemp4), ftemp2);
        ftemp3 = (ftemp2*ftemp8) - ftemp4;

        Amat[(i * pitch) + l] = (ftemp2 - ftemp8);

        for(k = l; k < cols; k++){
          scratch[k] = my_YByX(Amat[(i * pitch) + k] , ftemp3);
        }

        if (i != rows - 1){
          for(j = l; j < rows; j++){
            for(ftemp4 = 0.0f, k = l; k < cols; k++){
              ftemp4 += (Amat[(j * pitch) + k] * Amat[(i * pitch) + k]);
            }

            for(k = l; k < cols; k++){
              Amat[(j * pitch) + k] += (ftemp4 * scratch[k]);
            }
          }
        }

        for(k = l; k < rows; k++){
          Amat[(i * pitch) + k] = Amat[(i * pitch) + k] * scale;
        }
      }
    }
    anorm = __mmax(anorm, (_fabsf(Wmat[i]) + _fabsf(scratch[i])));
  }

  for(i = cols - 1; i >= 0; i--)
  {
    if (i < cols - 1)
    {
      if(ftemp8)
      {
        for(j = l; j < cols; j++){
          vMat[(j * pitch) + i] = my_YByX(my_YByX(Amat[(i * pitch) + j] , Amat[(i * pitch) + l]) , ftemp8);
        }

        for(j = l; j < cols; j++){
          for(ftemp4 = 0.0, k = l; k < cols; k++){
            ftemp4 += (Amat[(i * pitch) + k] * vMat[(k * pitch) + j]);
          }

          for(k = l; k < cols; k++){
            vMat[(k * pitch) + j] += (ftemp4 * vMat[(k * pitch) + i]);
          }
        }
      }
      for(j = l; j < cols; j++){
        vMat[(i * pitch) + j] = 0.0f;
        vMat[j* pitch + i]     = 0.0f;
      }
    }

    vMat[(i * pitch) + i] = 1.0f;
    ftemp8 = scratch[i];
    l = i;
  }

  for(i = cols - 1; i >= 0; i--)
  {
    l = i+1;
    ftemp8 = (float)Wmat[i];
    if (i < cols - 1)
      for(j = l; j < cols; j++)
        Amat[(i * pitch) + j] = 0.0f;
    if (ftemp8)
    {
      ftemp8 = 1.0f/ftemp8;
      if (i != cols - 1)
      {
        for(j = l; j < cols; j++)
        {
          for(ftemp4 = 0.0, k = l; k < rows; k++) {
            ftemp4 += Amat[(k * pitch) + i] * Amat[(k * pitch) + j];
          }

          ftemp2 = my_YByX(ftemp4 , Amat[(i * pitch) + i]) * ftemp8;

          for(k = i; k < rows; k++)
            Amat[(k * pitch) + j] += (ftemp2 * Amat[(k * pitch) + i]);
        }
      }
      for(j = i; j < rows; j++)
        Amat[j* pitch + i] = (Amat[j* pitch + i]*ftemp8);
    }
    else
    {
      for(j = i; j < rows; j++){
        Amat[j* pitch + i] = 0.0;
      }
    }
    ++Amat[(i * pitch) + i];
  }

  for(k = cols - 1; k >= 0; k--)
  {
    for(its = 0; its < 30; its++)
    {
      flag = 1;
      for(l = k; l >= 0; l--)
      {
        nm = l - 1;
        if (_fabsf(scratch[l]) + anorm == anorm){
          flag=0;
          break;
        }
        if (_fabsf(Wmat[nm]) + anorm == anorm){
          break;
        }
      }
      if (flag)
      {
        ftemp1 = 0.0;
        ftemp4 = 1.0;

        for(i = l; i <= k; i++)
        {
          ftemp2 = ftemp4 * scratch[i];
          if (_fabsf(ftemp2) + anorm != anorm)
          {
            ftemp8 = Wmat[i];
            ftemp3 = SFM_TI_PYTHAG(ftemp2, ftemp8);
            Wmat[i] = ftemp3;

            ftemp3 = my_OneByX(ftemp3);
            ftemp1 = ftemp8 * ftemp3;
            ftemp4 = (- ftemp2 * ftemp3);

            for(j = 0; j < rows; j++)
            {
              ftemp6 = Amat[(j * pitch) + nm];
              ftemp7 = Amat[(j * pitch) + i];
              Amat[(j * pitch) + nm] = (ftemp6 * ftemp1) + (ftemp7 * ftemp4);
              Amat[(j * pitch) + i]  = (ftemp7 * ftemp1) - (ftemp6 * ftemp4);
            }
          }
        }
      }
      ftemp7 = Wmat[k];
      if (l == k)
      {
        if (ftemp7 < 0.0)
        {
          Wmat[k] = (-ftemp7);
          for(j = 0; j < cols; j++)
            vMat[(j * pitch) + k] = (-vMat[(j * pitch) + k]);
        }
        break;
      }

      ftemp5  = Wmat[l];
      nm = k-1;
      ftemp6  = Wmat[nm];

      ftemp8 = scratch[nm];
      ftemp3 = scratch[k];

      ftemp2 = my_YByX(((ftemp6 - ftemp7) * (ftemp6 + ftemp7)+(ftemp8 - ftemp3)*(ftemp8 + ftemp3)),(2.0f * (ftemp3 * ftemp6)));
      ftemp8 = SFM_TI_PYTHAG(ftemp2, 1.0);
      ftemp2 = my_YByX(((ftemp5-ftemp7)*(ftemp5+ftemp7)+ftemp3*(my_YByX(ftemp6,(ftemp2+SIGN(ftemp8,ftemp2))) - ftemp3)),ftemp5);

      ftemp1 = ftemp4 = 1.0;

      for(j = l; j <= nm; j++)
      {
        i = j+1;
        ftemp8 = scratch[i];
        ftemp6 = Wmat[i];
        ftemp3 = ftemp4 * ftemp8;
        ftemp8 = ftemp1 * ftemp8;
        ftemp7 = SFM_TI_PYTHAG(ftemp2, ftemp3);
        scratch[j] = ftemp7;

        ftemp1 = my_YByX(ftemp2,ftemp7);
        ftemp4 = my_YByX(ftemp3,ftemp7);
        ftemp2 = (ftemp5 * ftemp1) + (ftemp8 * ftemp4);
        ftemp8 = (ftemp8 * ftemp1) - (ftemp5 * ftemp4);
        ftemp3 = ftemp6 * ftemp4;
        ftemp6 = ftemp6 * ftemp1;

        for(jj = 0; jj < cols; jj++)
        {
          ftemp5 = vMat[(jj * pitch) + j];
          ftemp7 = vMat[(jj * pitch) + i];
          vMat[(jj * pitch) + j] = ftemp5 * ftemp1 + ftemp7 * ftemp4;
          vMat[(jj * pitch) + i] = ftemp7 * ftemp1 - ftemp5 * ftemp4;
        }
        ftemp7 = SFM_TI_PYTHAG(ftemp2, ftemp3);

        Wmat[j] = ftemp7;

        if(ftemp7)
        {
          ftemp7 = my_OneByX(ftemp7);
          ftemp1 = ftemp2 * ftemp7;
          ftemp4 = ftemp3 * ftemp7;
        }

        ftemp2 = (ftemp1 * ftemp8) + (ftemp4 * ftemp6);
        ftemp5 = (ftemp1 * ftemp6) - (ftemp4 * ftemp8);

        for(jj = 0; jj < rows; jj++)
        {
          ftemp6 = Amat[jj * pitch + j];
          ftemp7 = Amat[jj * pitch + i];

          Amat[(jj * pitch) + j] = (ftemp6 * ftemp1) + (ftemp7 * ftemp4);
          Amat[(jj * pitch) + i] = (ftemp7 * ftemp1) - (ftemp6 * ftemp4);
        }
      }
      scratch[l] = 0.0;
      scratch[k] = ftemp2;
      Wmat[k]    = ftemp5;
    }
  }
}

float SFM_TI_PYTHAG(float a, float b)
{
  float absA = _fabsf(a);
  float absB = _fabsf(b);
  float absBbyabsA;
  float result;

  if (absA > absB){
    absBbyabsA = my_YByX(absB , absA);
    result = absA * my_SqrtX((float)(1.0 + absBbyabsA * absBbyabsA));
  }
  else if (absB > 0.0){
    absBbyabsA = my_YByX(absA , absB);
    result = absB * my_SqrtX((float)(1.0 + absBbyabsA * absBbyabsA));
  }
  else{
    result = 0.0;
  }

  return(result);
}

int32_t SFM_TI_LUDecomposion_3x3(float* A, float* b)
{
  int32_t i, j, k, p = 1;

  for( i = 0; i < 3; i++ )
  {
    k = i;

    for( j = i+1; j < 3; j++ )
      if( _fabsf(A[j*VLIB_TRIANG_MAT_AROW + i]) > _fabsf(A[k*VLIB_TRIANG_MAT_AROW + i]) )
        k = j;

    if( _fabsf(A[k*VLIB_TRIANG_MAT_AROW + i]) < FLT_EPSILON )
      return 0;

    if( k != i )
    {
      for( j = i; j < 3; j++ )
        SWAP_ME(A[i*VLIB_TRIANG_MAT_AROW + j], A[k*VLIB_TRIANG_MAT_AROW + j]);

      SWAP_ME(b[i*1], b[k*1]);
    }

    float d = -my_OneByX(A[i*VLIB_TRIANG_MAT_AROW + i]);

    for( j = i+1; j < 3; j++ )
    {
      float alpha = A[j*VLIB_TRIANG_MAT_AROW + i]*d;

      for( k = i+1; k < 3; k++ )
        A[j*VLIB_TRIANG_MAT_AROW + k] += alpha*A[i*VLIB_TRIANG_MAT_AROW + k];

      b[j*1] += alpha*b[i*1];
    }

    A[i*VLIB_TRIANG_MAT_AROW + i] = -d;
  }

  for( i = 3-1; i >= 0; i-- ){
    float s = b[i];
    for( k = i+1; k < 3; k++ )
      s -= A[i*VLIB_TRIANG_MAT_AROW + k]*b[k];
    b[i] = s*A[i*VLIB_TRIANG_MAT_AROW + i];
  }

  return p;

}
#if 0
int32_t SFM_TI_LUDecomposion_3x3_ci(float * restrict A, float * restrict b, int32_t* restrict result)
{
  int32_t i,j,k,l,m;
  __float2_t temp1f2,temp2f2, alpha_alpha;
  float temp1, temp2;

  int32_t lastRow,curRow,nextRow;
  int32_t A_Pitch = (2*VLIB_TRIANG_MAT_ROW - 1) * VLIB_TRIANG_MAT_AROW;
  int32_t b_Pitch = VLIB_TRIANG_MAT_AROW;
  float d,alpha;

  float* restrict AL;
  float* restrict bL;

  float s;


#pragma MUST_ITERATE(4,4,4)
  for( l = 0; l < NUM_TRACKS_TOGATHER; l++){

    AL      = A + l* A_Pitch;
    bL      = b + l* b_Pitch;
#pragma MUST_ITERATE(2,2,2)
    for( i = 0; i < 2; i++ )
    {
      k = i;
      /*#pragma MUST_ITERATE(0,2,1)
      for( j = i+1; j <  (0 + 3); j++ )
      if( _fabsf(AL[j*VLIB_TRIANG_MAT_AROW + i]) > _fabsf(AL[k*VLIB_TRIANG_MAT_AROW + i]) )
      k = j;*/

      j = i + 1;
      if( _fabsf(AL[j*VLIB_TRIANG_MAT_AROW + i]) > _fabsf(AL[k*VLIB_TRIANG_MAT_AROW + i]) )
        k = j;

      j = i + 2;
      if( _fabsf(AL[j*VLIB_TRIANG_MAT_AROW + i]) > _fabsf(AL[k*VLIB_TRIANG_MAT_AROW + i]) )
        k = j;

      if( _fabsf(AL[k*VLIB_TRIANG_MAT_AROW + i]) < FLT_EPSILON )
        result[l] &= 0x0;

      temp1f2 = _amem8_f2(&AL[i*VLIB_TRIANG_MAT_AROW + 0]);
      temp2f2 = _amem8_f2(&AL[k*VLIB_TRIANG_MAT_AROW + 0]);

      _amem8_f2(&AL[i*VLIB_TRIANG_MAT_AROW + 0]) = temp2f2;
      _amem8_f2(&AL[k*VLIB_TRIANG_MAT_AROW + 0]) = temp1f2;

      temp1 = AL[i*VLIB_TRIANG_MAT_AROW + 2];
      temp2 = AL[k*VLIB_TRIANG_MAT_AROW + 2];

      AL[i*VLIB_TRIANG_MAT_AROW + 2] = temp2;
      AL[k*VLIB_TRIANG_MAT_AROW + 2] = temp1;

      temp1 = bL[i*1];
      temp2 = bL[k*1];

      bL[i*1] = temp2;
      bL[k*1] = temp1;

      d = -my_OneByX(AL[i*VLIB_TRIANG_MAT_AROW + i]);

#pragma MUST_ITERATE(2,2,2)
      for(m = 0, j = i+1; m < 2; j++, m++ )
        //for(j = i+1; j < 3; j++)
      {
        // i = 0 --> j = 1, j = 2
        // i = 1 --> j = 2, j = 3
        alpha = AL[j*VLIB_TRIANG_MAT_AROW + i]*d;

        if(j >= 3)
          alpha = 0.0;

        alpha_alpha = _ftof2(alpha,alpha);

        temp1f2 = _amem8_f2(&AL[i*VLIB_TRIANG_MAT_AROW + 0]);
        temp1f2 = _dmpysp(temp1f2,alpha_alpha);
        temp2f2 = _amem8_f2(&AL[j*VLIB_TRIANG_MAT_AROW + 0]);
        _amem8_f2(&AL[j*VLIB_TRIANG_MAT_AROW + 0]) = _daddsp(temp2f2, temp1f2);

        temp1   = AL[i*VLIB_TRIANG_MAT_AROW + 2];
        temp2   = AL[j*VLIB_TRIANG_MAT_AROW + 2];
        AL[j*VLIB_TRIANG_MAT_AROW + 2] = temp2 + alpha*temp1;

        bL[j*1] += alpha*bL[i*1];
      }

      AL[i*VLIB_TRIANG_MAT_AROW + i] = -d;
    }
    //}

    //for(l=0; l < NUM_TRACKS_TOGATHER; l++)
    //{
    AL           = A + l*A_Pitch;
    bL           = b + l*b_Pitch;

    //rowMapL      = &rowMap[l*VLIB_TRIANG_MAT_ROW];

    //curRow       = rowMapL[0];
    //nextRow      = rowMapL[1];
    //lastRow      = rowMapL[2];

    curRow       = 0;
    nextRow      = 1;
    lastRow      = 2;

    d            =  AL[lastRow*VLIB_TRIANG_MAT_AROW + 2];

    if( _fabsf(d) < FLT_EPSILON )
      result[l] &= 0x0;

    s            = bL[lastRow];
    bL[lastRow]  = s*my_OneByX(d);


    s            = bL[nextRow];
    s           -= (AL[nextRow*VLIB_TRIANG_MAT_AROW + 2]*bL[lastRow]);
    bL[nextRow]  = s*AL[nextRow*VLIB_TRIANG_MAT_AROW  + 1];

    s            = bL[curRow];
    temp1f2      = _mem8_f2(&AL[curRow*VLIB_TRIANG_MAT_AROW + 1]);
    temp2f2      = _mem8_f2(&bL[nextRow]);
    temp1f2      = _dmpysp(temp1f2,temp2f2);

    s           -=(_lof2(temp1f2) + _hif2(temp1f2));
    bL[curRow]   = s*AL[curRow*VLIB_TRIANG_MAT_AROW + 0];
  }


  return 0;

}
#endif
/* Does At * A
*/
void SFM_TI_preMultiplyByTranspose( float * Ain, float * AtA, int32_t rows, int32_t cols, int32_t pitch)

{

  int32_t i, j, k;
  int32_t rowStep;
  float val;

  rowStep = pitch;

  for (k = 0; k < rows; k++) {
    for (j = k; j < rows; j++) {
      val = 0;
      for (i = 0; i < cols; i++) //N
        val += Ain[k*cols + i] * Ain[j*cols + i];

      AtA[k * rowStep + j] = val;
      AtA[j * rowStep + k] = val;
    }
  }

}
#if 0
void SFM_TI_preMultiplyByTranspose_ci( float * restrict Ain, float * restrict AtA, int32_t rows, int32_t cols, int32_t pitch)
{
  int32_t j, k, l;
  float val1;
  __float2_t* restrict ptr1;
  __float2_t* restrict ptr2;
  __float2_t row_data10,row_data32,row_data54,row_data76;
  __float2_t col_data10,col_data32,col_data54,col_data76;
  __x128_t   row_data3210,row_data7654;
  __x128_t   col_data3210,col_data7654;
  __x128_t   result_data3210,result_data7654;

  float* restrict outPtr = AtA;

  if(cols == 8){
    /* Matrix used here is A(8x9). So multiplication is
    * 9x8(At) * 8x9(A) = 9x9(Result);
    * Here each element of result matrix will be obtained by
    * dot product of two vector of size of 8 ( one row of At
    * and one column of A)
    * For facilitate SIMD operation we need to store A in column
    * wise instead of raster scan.
    */
#pragma MUST_ITERATE(9,9,1)
    for(k=0; k<rows; k++)
    {
      ptr1         = (__float2_t * )&Ain[k*cols];
      row_data10   = _amem8_f2_const(ptr1++);
      row_data32   = _amem8_f2_const(ptr1++);
      row_data54   = _amem8_f2_const(ptr1++);
      row_data76   = _amem8_f2_const(ptr1++);
      row_data3210     = _f2to128(row_data32,row_data10);
      row_data7654     = _f2to128(row_data76,row_data54);

#pragma MUST_ITERATE(1,9,1)
      for(j=k; j<rows; j++)
      {
        ptr2             = (__float2_t * )&Ain[j*cols];

        //DOUBLE_WORD_ALIGNED(ptr2);

        col_data10       = _amem8_f2_const(ptr2++);
        col_data32       = _amem8_f2_const(ptr2++);
        col_data54       = _amem8_f2_const(ptr2++);
        col_data76       = _amem8_f2_const(ptr2++);
        col_data3210     = _f2to128(col_data32,col_data10);
        col_data7654     = _f2to128(col_data76,col_data54);

        result_data3210  = _qmpysp(row_data3210,col_data3210);
        //temp             = _daddsp(_hif2_128(result_data3210),_lof2_128(result_data3210));
        val1             = _lof2(_lof2_128(result_data3210));
        val1            += _hif2(_lof2_128(result_data3210));
        val1            += _lof2(_hif2_128(result_data3210));
        val1            += _hif2(_hif2_128(result_data3210));

        result_data7654  = _qmpysp(row_data7654,col_data7654);
        //temp             = _daddsp(_hif2_128(result_data7654),_lof2_128(result_data7654));
        val1            += _lof2(_lof2_128(result_data7654));
        val1            += _hif2(_lof2_128(result_data7654));
        val1            += _lof2(_hif2_128(result_data7654));
        val1            += _hif2(_hif2_128(result_data7654));

        outPtr[k*pitch + j] = val1;
        outPtr[j*pitch + k] = val1;
      }
    }
  }else{
    /* Matrix used here is A(Nx9). So multiplication is
    * 9xN(At) * Nx9(A) = 9x9(Result);
    * Here each element of result matrix will be obtained by
    * dot product of two vector of size of 8 ( one row of At
    * and one column of A)
    * For facilitate SIMD operation we need to store A in column
    * wise instead of raster scan.
    */
#pragma MUST_ITERATE(9,9,1)
    for(k=0; k<rows; k++)
    {
#pragma MUST_ITERATE(1,9,1)
      for(j=k; j<rows; j++)
      {
        /* Columns are assumed to be multiple of 8
        */
        ptr1         = (__float2_t * )&Ain[k*cols];
        ptr2         = (__float2_t * )&Ain[j*cols];
        val1         = 0;

        for(l=0; l<(cols >> 3); l++){

          row_data10       = _amem8_f2_const(ptr1++);
          row_data32       = _amem8_f2_const(ptr1++);
          row_data54       = _amem8_f2_const(ptr1++);
          row_data76       = _amem8_f2_const(ptr1++);
          row_data3210     = _f2to128(row_data32,row_data10);
          row_data7654     = _f2to128(row_data76,row_data54);

          col_data10       = _amem8_f2_const(ptr2++);
          col_data32       = _amem8_f2_const(ptr2++);
          col_data54       = _amem8_f2_const(ptr2++);
          col_data76       = _amem8_f2_const(ptr2++);
          col_data3210     = _f2to128(col_data32,col_data10);
          col_data7654     = _f2to128(col_data76,col_data54);

          result_data3210  = _qmpysp(row_data3210,col_data3210);
          //temp             = _daddsp(_hif2_128(result_data3210),_lof2_128(result_data3210));
          val1            += _lof2(_lof2_128(result_data3210));
          val1            += _hif2(_lof2_128(result_data3210));
          val1            += _lof2(_hif2_128(result_data3210));
          val1            += _hif2(_hif2_128(result_data3210));

          result_data7654  = _qmpysp(row_data7654,col_data7654);
          //temp             = _daddsp(_hif2_128(result_data7654),_lof2_128(result_data7654));
          val1            += _lof2(_lof2_128(result_data7654));
          val1            += _hif2(_lof2_128(result_data7654));
          val1            += _lof2(_hif2_128(result_data7654));
          val1            += _hif2(_hif2_128(result_data7654));
        }

        outPtr[k*pitch + j] = val1;
        outPtr[j*pitch + k] = val1;
      }
    }
  }
}
#endif
float SFM_TI_NormalizeVec(float * vec, uint16_t length, float * vecOut) {
  uint16_t index;
  float norm;
  float normInv;

  float val;


  norm = 0.0f;
  for (index = 0; index < length; index++) {
    val = *(vec + index);
    norm += val * val;
  }

  normInv = my_OneBySqrtX(norm);

  for (index = 0; index < length; index++) {
    val = *(vec + index);
    vecOut[index] = val * normInv;
  }

  return norm;
}
float SFM_TI_NormalizeVecAndComputeNorm(float * restrict vec,
                                        uint16_t length,
                                        float * restrict vecOut)
{
  uint16_t index;
  float norm;
  float normInv;

  float val;
  float maxVal = FLT_MIN;
  float currentInfNorm;

  norm = 0.0f;
#pragma MUST_ITERATE(9,9,9)
  for(index=0; index<length; index++)
  {
    val = *(vec+index);
    norm += val*val;
  }

  normInv = my_OneBySqrtX(norm);

#pragma MUST_ITERATE(9,9,9)
  for(index=0; index<length; index++)
  {
    val            = *(vec+index);
    currentInfNorm = val*normInv;
    vecOut[index]  = currentInfNorm;
    currentInfNorm = fabsf(currentInfNorm);
    if(currentInfNorm > maxVal)
    {
      maxVal = currentInfNorm;
    }
  }
  return maxVal;

}
int32_t SFM_TI_Crout_LU_Decomposition_with_Pivoting(float *A, int16_t pivot[], int32_t n, float* Aout, int32_t pitch) {
  int32_t i, j, k;
  float *p_k, *p_row, *p_col;
  float max;
  p_col = A;
  for (k = 0, p_k = A; k < n; p_k += pitch, k++) {                                                    //find the pivot row

    pivot[k] = (int16_t)k;
    max = _fabsf(*(p_k + k));

    for (j = k + 1, p_row = p_k + pitch; j < n; j++, p_row += pitch) {
      if (max < _fabsf(*(p_row + k))) {
        max = _fabsf(*(p_row + k));
        pivot[k] = (int16_t)j;
        p_col = p_row;
      }
    }

    //     and if the pivot row differs from the current row, then
    //     interchange the two rows.

    if (pivot[k] != k)
      for (j = 0; j < n; j++) {
        max = *(p_k + j);
        *(p_k + j) = *(p_col + j);
        *(p_col + j) = max;
      }

      //                and if the matrix is singular, return error

      if (*(p_k + k) == 0.0)
        return -1;

      //      otherwise find the upper triangular matrix elements for row k.

      for (j = k + 1; j < n; j++) {
        *(p_k + j) = *(p_k + j)*my_OneByX(*(p_k + k));
      }

      //            update remaining matrix

      for (i = k + 1, p_row = p_k + pitch; i < n; p_row += pitch, i++)
        for (j = k + 1; j < n; j++)
          *(p_row + j) -= *(p_row + k) * *(p_k + j);

  }
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      Aout[pitch*i + j] = A[pitch*i + j];
    }
  }
  return 0;
}
#if 0
int32_t SFM_TI_Crout_LU_Decomposition_with_Pivoting_ci(float * restrict Ain,
                                                       int16_t * restrict pivot,
                                                       int32_t n,
                                                       float * restrict Aout,
                                                       int32_t pitch)
{
  int32_t  i, j, k, l;
  int32_t result;
  __float2_t temp1f2, temp2f2, temp3f2, alpha_alpha, d_d;
  float alpha, d, temp1, temp2, temp3;

  float mulMask[9] = {1,1,1,1,1,1,1,1,1};
  __float2_t mulMask2;

  result = 0x1U;

#pragma MUST_ITERATE(8,8,8)
  for( i = 0; i < (n-1); i++ )
  {
    k = i;
#pragma MUST_ITERATE(8,8,8)
    for(l = 1; l < n; l++){
      j = i + l;
      if( _fabsf(Ain[j*pitch + i]) > _fabsf(Ain[k*pitch + i]) )
        k = j;
    }

    pivot[i] = k;

    if( _fabsf(Ain[k*pitch + i]) < FLT_EPSILON )
      result &= 0x0;

    mulMask[i] = 0;

#pragma MUST_ITERATE(4,4,4)
    for(l = 0; l < (n >> 1); l++){

      temp1f2 = _amem8_f2(&Ain[i*pitch + 2*l]);
      temp2f2 = _amem8_f2(&Ain[k*pitch + 2*l]);

      temp3f2 = temp2f2;
      temp2f2 = temp1f2;
      temp1f2 = temp3f2;

      _amem8_f2(&Ain[i*pitch + 2*l]) = temp1f2;
      _amem8_f2(&Ain[k*pitch + 2*l]) = temp2f2;
    }

    temp1 = Ain[i*pitch + (n-1)];
    temp2 = Ain[k*pitch + (n-1)];

    temp3 = temp2;
    temp2 = temp1;
    temp1 = temp3;

    Ain[i*pitch + (n-1)] = temp1;
    Ain[k*pitch + (n-1)] = temp2;

    d   = my_OneByX(Ain[i*pitch + i]);
    d_d = _ftof2(d,d);

#pragma MUST_ITERATE(4,4,4)
    for( l = 0; l < (n >> 1); l ++){
      temp1f2 = _amem8_f2(&Ain[i*pitch + 2*l]);
      _amem8_f2(&Aout[i*pitch + 2*l]) = _dmpysp(temp1f2,d_d);
    }

    Aout[i*pitch + (n-1)] =   Ain[i*pitch + (n-1)]*d;

#pragma MUST_ITERATE(8,8,8)
    for(l = 0, j = i+1; l < (n-1); j++, l++ )
      //for(j = i+1; j < 3; j++)
    {
      // i = 0 --> j = 1, j = 2
      // i = 1 --> j = 2, j = 3
      alpha    = Ain[j*pitch + i];

      if(j >= n)
        alpha = 0.0;

      alpha_alpha = _ftof2(alpha,alpha);

      temp1f2  = _amem8_f2(&Aout[i*pitch + 0]);
      temp1f2  = _dmpysp(temp1f2,alpha_alpha);
      mulMask2 = _mem8_f2(&mulMask[0]);
      temp1f2  = _dmpysp(mulMask2,temp1f2);
      temp2f2  = _amem8_f2(&Ain[j*pitch + 0]);
      _amem8_f2(&Ain[j*pitch + 0]) = _dsubsp(temp2f2, temp1f2);

      temp1f2 = _amem8_f2(&Aout[i*pitch + 2]);
      temp1f2 = _dmpysp(temp1f2,alpha_alpha);
      mulMask2 = _mem8_f2(&mulMask[2]);
      temp1f2  = _dmpysp(mulMask2,temp1f2);
      temp2f2 = _amem8_f2(&Ain[j*pitch + 2]);
      _amem8_f2(&Ain[j*pitch + 2]) = _dsubsp(temp2f2, temp1f2);

      temp1f2 = _amem8_f2(&Aout[i*pitch + 4]);
      temp1f2 = _dmpysp(temp1f2,alpha_alpha);
      mulMask2 = _mem8_f2(&mulMask[4]);
      temp1f2  = _dmpysp(mulMask2,temp1f2);
      temp2f2 = _amem8_f2(&Ain[j*pitch + 4]);
      _amem8_f2(&Ain[j*pitch + 4]) = _dsubsp(temp2f2, temp1f2);

      temp1f2 = _amem8_f2(&Aout[i*pitch + 6]);
      temp1f2 = _dmpysp(temp1f2,alpha_alpha);
      mulMask2 = _mem8_f2(&mulMask[6]);
      temp1f2  = _dmpysp(mulMask2,temp1f2);
      temp2f2 = _amem8_f2(&Ain[j*pitch + 6]);
      _amem8_f2(&Ain[j*pitch + 6]) = _dsubsp(temp2f2, temp1f2);

      temp1   = Aout[i*pitch + 8];
      temp2   = Ain[j*pitch + 8];

      Ain[j*pitch + 8] = temp2 - alpha*temp1*mulMask[8];;
    }
  }
  pivot[8] = 8;
  if( _fabsf(Ain[8*pitch + 8]) < FLT_EPSILON )
    result &= 0x0;
  return result;
}
#endif
int32_t SFM_TI_Crout_LU_with_Pivoting_Solve(float *LU, float B[], int16_t pivot[], float x[], int32_t n, int32_t pitch) {
  int32_t i, k;
  float *p_k;
  float dum;

  /* Solve the linear equation Lx = B for x, where L is a lower
  triangular matrix.
  */

  for (k = 0, p_k = LU; k < n; p_k += pitch, k++) {
    if (pivot[k] != k) {
      dum = B[k];
      B[k] = B[pivot[k]];
      B[pivot[k]] = dum;
    }

    x[k] = B[k];

    for (i = 0; i < k; i++)
      x[k] -= x[i] * *(p_k + i);

    x[k] = my_YByX(x[k],*(p_k + k));
  }

  /* Solve the linear equation Ux = y, where y is the solution
  obtained above of Lx = B and U is an upper triangular matrix.
  The diagonal part of the upper triangular part of the matrix is
  assumed to be 1.0.
  */

  for (k = n - 1, p_k = LU + pitch * (n - 1); k >= 0; k--, p_k -= pitch) {
    if (pivot[k] != k) {
      dum = B[k];
      B[k] = B[pivot[k]];
      B[pivot[k]] = dum;
    }

    for (i = k + 1; i < n; i++)
      x[k] -= x[i] * *(p_k + i);

    if (*(p_k + k) == 0.0)
      return -1;
  }

  return 0;
}
#if 0
int32_t SFM_TI_Crout_LU_with_Pivoting_Solve_ci(float L[restrict],
                                               float U[restrict],
                                               float B[restrict],
                                               short pivot[restrict],
                                               float x[restrict],
                                               int32_t n,
                                               int32_t pitch)
{
  int32_t i, k;
  float *p_k;
  float dum;

  float acc1;


#pragma MUST_ITERATE(9,9,1)
  for (k = 0, p_k = L; k < n; p_k += pitch, k++)
  {
    if(pivot[k] != k)
    {
      dum         = B[k];
      B[k]        = B[pivot[k]];
      B[pivot[k]] = dum;
    }

    x[k] = B[k];
    //#pragma MUST_ITERATE(9,9,1)
    for (i = 0; i < k; i++)
    {
      x[k] -= x[i] * *(p_k + i); // 13 cycles per iteration
    }
    //acc1  -= (x[k]*(*(p_k + k)));
    //x[k]  -= acc1;
    x[k] = my_YByX(x[k],*(p_k + k));

  }

  // Solve the linear equation Ux = y, where y is the solution
  // obtained above of Lx = B and U is an upper triangular matrix.
  // The diagonal part of the upper triangular part of the matrix is
  // assumed to be 1.0.
#pragma MUST_ITERATE(8,8,1)
  for (k = n-2, p_k = U + pitch*(n-2); k >= 0; k--, p_k -= pitch)
  {
    acc1 = 0;

#pragma MUST_ITERATE(8,8,1)
    for (i = 1; i < n; i++)
    {
      acc1 += x[i] * *(p_k + i); // 13 cycles per iteration
    }

    x[k] -= acc1;
  }

  return 0;
}
#endif
int32_t SFM_TI_Crout_LU_Decomposition(float *  ALUin,
                                      int32_t  n,
                                      float *  ALout,
                                      float *  AUout,
                                      int32_t  pitch)
{
  int32_t i,j;
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      if(j<=i)
      {
        ALout[i*pitch + j] = ALUin[i*pitch + j];
        AUout[i*pitch + j] = 0.0;
      }
      else
      {
        ALout[i*pitch + j] = 0.0;
        AUout[i*pitch + j] = ALUin[i*pitch + j];
      }
    }
  }
  return(0);
}
#if 0
int32_t SFM_TI_Crout_LU_Decomposition_ci(float * restrict Ain,
                                         float * restrict ALUin,
                                         int32_t n,
                                         float * restrict ALout,
                                         float * restrict AUout,
                                         int32_t pitch)
{
  int32_t i,j;
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      if(j<=i)
      {
        ALout[i*pitch + j] = Ain[i*pitch + j];
        AUout[i*pitch + j] = 0.0;
      }
      else
      {
        ALout[i*pitch + j] = 0.0;
        AUout[i*pitch + j] = ALUin[i*pitch + j];
      }
    }
  }
  return(0);
}
#define SIMD_WIDTH (4)
void cofactor(__float2_t *restrict srcPtr,
              __float2_t *restrict dstPtr,
              int len)
{
  int ctr;

  for(ctr = 0; ctr < len; ctr+=SIMD_WIDTH)
  {
    __float2_t W_ab, X_ab, Y_ab, Z_ab, T_ab;
    __float2_t W_cd, X_cd, Y_cd, Z_cd, T_cd;
    __x128_t A_abcd, B_abcd;

    W_ab = _amem8_f2(srcPtr++); W_cd = _amem8_f2(srcPtr++); //.D1T1, .D2T2
    X_ab = _amem8_f2(srcPtr++); X_cd = _amem8_f2(srcPtr++); //.D1T1, .D2T2
    Y_ab = _amem8_f2(srcPtr++); Y_cd = _amem8_f2(srcPtr++); //.D1T1, .D2T2

    A_abcd = _qmpysp(_f2to128(W_ab, W_cd), _f2to128(X_ab, X_cd)); //.M1
    B_abcd = _qmpysp(_f2to128(Y_ab, Y_cd), _f2to128(Y_ab, Y_cd)); //.M2

    T_ab = _dsubsp(_lof2_128(A_abcd), _lof2_128(B_abcd)); //.L1
    T_cd = _dsubsp(_hif2_128(A_abcd), _hif2_128(B_abcd)); //.L2
    _amem8_f2(dstPtr++) = T_ab; _amem8_f2(dstPtr++) = T_cd;  //.D1T1, .D2T2

    W_ab = _amem8_f2(srcPtr++); W_cd = _amem8_f2(srcPtr++);  //.D1T1, .D2T2
    Z_ab = _amem8_f2(srcPtr++); Z_cd = _amem8_f2(srcPtr++);  //.D1T1, .D2T2

    A_abcd = _qmpysp(_f2to128(W_ab, W_cd), _f2to128(X_ab, X_cd)); //.M1
    B_abcd = _qmpysp(_f2to128(Y_ab, Y_cd), _f2to128(Z_ab, Z_cd)); //.M2

    T_ab = _dsubsp(_lof2_128(A_abcd), _lof2_128(B_abcd)); //.L1
    T_cd = _dsubsp(_hif2_128(A_abcd), _hif2_128(B_abcd)); //.L2
    _amem8_f2(dstPtr++) = T_ab; _amem8_f2(dstPtr++) = T_cd;  //.D1T1, .D2T2

    X_ab = _amem8_f2(srcPtr++); X_cd = _amem8_f2(srcPtr++);  //.D1T1, .D2T2
    Y_ab = _amem8_f2(srcPtr++); Y_cd = _amem8_f2(srcPtr++);  //.D1T1, .D2T2

    A_abcd = _qmpysp(_f2to128(W_ab, W_cd), _f2to128(X_ab, X_cd)); //.M1
    B_abcd = _qmpysp(_f2to128(Y_ab, Y_cd), _f2to128(Z_ab, Z_cd)); //.M2

    T_ab = _dsubsp(_lof2_128(A_abcd), _lof2_128(B_abcd)); //.L1
    T_cd = _dsubsp(_hif2_128(A_abcd), _hif2_128(B_abcd)); //.L2
    _amem8_f2(dstPtr++) = T_ab; _amem8_f2(dstPtr++) = T_cd;  //.D1T1, .D2T2

    W_ab = _amem8_f2(srcPtr++); W_cd = _amem8_f2(srcPtr++);  //.D1T1, .D2T2
    X_ab = _amem8_f2(srcPtr++); X_cd = _amem8_f2(srcPtr++);  //.D1T1, .D2T2

    A_abcd = _qmpysp(_f2to128(W_ab, W_cd), _f2to128(X_ab, X_cd)); //.M1
    B_abcd = _qmpysp(_f2to128(Y_ab, Y_cd), _f2to128(Z_ab, Z_cd)); //.M2

    T_ab = _dsubsp(_lof2_128(A_abcd), _lof2_128(B_abcd)); //.L1
    T_cd = _dsubsp(_hif2_128(A_abcd), _hif2_128(B_abcd)); //.L2
    _amem8_f2(dstPtr++) = T_ab; _amem8_f2(dstPtr++) = T_cd;  //.D1T1, .D2T2

    Y_ab = _amem8_f2(srcPtr++); Y_cd = _amem8_f2(srcPtr++);  //.D1T1, .D2T2

    A_abcd = _qmpysp(_f2to128(W_ab, W_cd), _f2to128(X_ab, X_cd)); //.M1
    B_abcd = _qmpysp(_f2to128(Y_ab, Y_cd), _f2to128(Z_ab, Z_cd)); //.M2

    T_ab = _dsubsp(_lof2_128(A_abcd), _lof2_128(B_abcd)); //.L1
    T_cd = _dsubsp(_hif2_128(A_abcd), _hif2_128(B_abcd)); //.L2
    _amem8_f2(dstPtr++) = T_ab; _amem8_f2(dstPtr++) = T_cd;  //.D1T1, .D2T2

    X_ab = _amem8_f2(srcPtr++); X_cd = _amem8_f2(srcPtr++);  //.D1T1, .D2T2

    A_abcd = _qmpysp(_f2to128(W_ab, W_cd), _f2to128(X_ab, X_cd)); //.M1
    B_abcd = _qmpysp(_f2to128(Y_ab, Y_cd), _f2to128(Z_ab, Z_cd)); //.M2

    T_ab = _dsubsp(_lof2_128(A_abcd), _lof2_128(B_abcd)); //.L1
    T_cd = _dsubsp(_hif2_128(A_abcd), _hif2_128(B_abcd)); //.L2
    _amem8_f2(dstPtr++) = T_ab; _amem8_f2(dstPtr++) = T_cd;  //.D1T1, .D2T2
  }
}

void SFM_TI_matrix3x3Inv2_ci(float *restrict matAtAPtr,
                             float *restrict matBPtr,
                             float *restrict resultPtr,
                             int len)
{
  int ctr;

  matAtAPtr[0] = _ftof2(11.0,12.0);
  matAtAPtr[1] = _ftof2(13.0,22.0);
  matAtAPtr[2] = _ftof2(23.0,33.0);

  /* 11 12 13
  12 22 23
  13 23 33
  */

  /*
  a1(11) a2(12) a3(13)
  b2(22) b3(23)
  c3(33)
  */
  for(ctr = 0; ctr < len; ctr++)
  {
    __float2_t a1_ab, a2_ab, a3_ab, b2_ab, b3_ab, c3_ab;
    float d1_ab,d2_ab,d3_ab,d4_ab,d5_ab,d6_ab;
    __float2_t det_ab,invDet_ab;
    __float2_t atb1_ab,atb2_ab,atb3_ab;
    float a1,a2,a3,b2,b3,c3;

    a1_a2 = _amem8_f2(matAtAPtr++);
    a3_b2 = _amem8_f2(matAtAPtr++);
    b3_c3 = _amem8_f2(matAtAPtr++);

    a1    = _hif2(a1_a2);
    a3    = _hif2(a3_b2);
    b3    = _hif2(b3_c3);

    a2    = _lof2(a1_a2);
    b2    = _lof2(a3_b2);
    c3    = _lof2(b3_c3);

    /* Co-factor calculation Starts
    */
    /* Row 1 b2*c3 - b3*b3*/

    // b2 : b3
    // c3 : b3
    f2temp1 = _ftof2(b2,b3);
    f2temp2 = _ftof2(c3,b3);
    f2temp1 = _dmpysp(f2temp1,f2temp2);
    d1_ab   = _hif2(f2temp1) - _lof2(f2temp1);

    /* Row 1 b3*a3 - a2*c3*/
    // b3 : a2
    // a3 : c3
    f2temp1 = _ftof2(b3,a2);
    f2temp2 = _ftof2(a3,c3);
    f2temp1 = _dmpysp(f2temp1,f2temp2);
    d2_ab   = _hif2(f2temp1) - _lof2(f2temp1);

    /* Row 1 a2*b3 - b2*a3*/
    // a2 b2
    // b3 a3
    f2temp1 = _ftof2(a2,b2);
    f2temp2 = _ftof2(b3,a3);
    f2temp1 = _dmpysp(f2temp1,f2temp2);
    d3_ab   = _hif2(f2temp1) - _lof2(f2temp1);

    /* Row 1 a1*c3 - a3*a3*/
    // a1 a3
    // c3 a3
    f2temp1 = _ftof2(a1,a3);
    f2temp2 = _ftof2(c3,a3);
    f2temp1 = _dmpysp(f2temp1,f2temp2);
    d4_ab   = _hif2(f2temp1) - _lof2(f2temp1);

    /* Row 1 a2*a3 - a1*b3 */
    // a2 a1
    // a3 b3
    f2temp1 = _ftof2(a2,a1);
    f2temp2 = _ftof2(a3,b3);
    f2temp1 = _dmpysp(f2temp1,f2temp2);
    d5_ab   = _hif2(f2temp1) - _lof2(f2temp1);

    /* Row 1 a1*b2 - a2*a2*/
    // a1 a2
    // b2 a2
    f2temp1 = _ftof2(a1,a2);
    f2temp2 = _ftof2(b2,a2);
    f2temp1 = _dmpysp(f2temp1,f2temp2);
    d6_ab   = _hif2(f2temp1) - _lof2(f2temp1);

    det_ab    = _daddsp(_daddsp(_dmpysp(a1_ab,d1_ab),_dmpysp(a3_ab,d3_ab)), _dmpysp(a2_ab,d2_ab));
    invDet_ab = _ftof2(_rcpsp(_hif2(det_ab)),_rcpsp(_lof2(det_ab)));
    invDet_ab = _dmpysp(invDet_ab,_dsubsp(_ftof2(2.0,2.0),_dmpysp(det_ab,invDet_ab)));

  }
}
#endif

void SFM_TI_matrixMultiply_3x4By4x1(float A[restrict], float B[restrict], float C[restrict], int32_t num_matrices) {
  int i, j, k = 0;
  float val;
  int32_t l;

  for (l = 0; l < num_matrices; l+= SIMD_WIDTH){
    for (k = 0; k < SIMD_WIDTH; k++){
      for (i = 0; i < 3; i++) {
          val = 0.0f;
          for (j = 0; j < 3; j++){
            val += ((A[(i * 4 + j)]) * B[j*SIMD_WIDTH + k]);
          }
          val += (A[(i * 4 + j)] * 1.0f); // 4th element of B is 1.0f
          C[i*SIMD_WIDTH + k] = val;
      }
    }
    B += (SIMD_WIDTH*3);
    C += (SIMD_WIDTH*3);
  }

}

void SFM_TI_matrixMultiply_3x3By3x1(float A[restrict], float B[restrict], float C[restrict], int32_t num_matrices)
{
  int i, j, k = 0;
  float val;
  int32_t l;

  for (l = 0; l < num_matrices; l+= SIMD_WIDTH){
    for (k = 0; k < SIMD_WIDTH; k++){
      for (i = 0; i < 3; i++) {
          val = 0.0f;
          for (j = 0; j < 3; j++){
            val += ((A[(i * 3 + j)]) * B[j*SIMD_WIDTH + k]);
          }
          C[i*SIMD_WIDTH + k] = val;
      }
    }
    B += (SIMD_WIDTH*3);
    C += (SIMD_WIDTH*3);
  }

}

void SFM_TI_matrixMultiply_3x4By4x1_ci(float A[restrict], float B[restrict], float C[restrict], int32_t num_matrices, uint32_t* pBlock)
{
  float16 val;
  int32_t l;

  float16 a_00 = (float16)A[0 * 4 + 0];
  float16 a_01 = (float16)A[0 * 4 + 1];
  float16 a_02 = (float16)A[0 * 4 + 2];
  float16 a_03 = (float16)A[0 * 4 + 3];
  float16 a_10 = (float16)A[1 * 4 + 0];
  float16 a_11 = (float16)A[1 * 4 + 1];
  float16 a_12 = (float16)A[1 * 4 + 2];
  float16 a_13 = (float16)A[1 * 4 + 3];
  float16 a_20 = (float16)A[2 * 4 + 0];
  float16 a_21 = (float16)A[2 * 4 + 1];
  float16 a_22 = (float16)A[2 * 4 + 2];
  float16 a_23 = (float16)A[2 * 4 + 3];

  __SE_TEMPLATE_v1 seTemplate = *(__SE_TEMPLATE_v1*)((uint8_t *)pBlock + SE5_PARAM_OFFSET);
  __SE0_OPEN((void *)B, seTemplate);

  for (l = 0; l < num_matrices; l+= SIMD_WIDTH)
  {
      val  = a_00 * __SE0ADV(float16);
      val += a_01 * __SE0ADV(float16);
      val += a_02 * __SE0ADV(float16);
      val += a_03;
      *stov_ptr(float16,(float*)C) = val; C += SIMD_WIDTH;

      val  = a_10 * __SE0ADV(float16);
      val += a_11 * __SE0ADV(float16);
      val += a_12 * __SE0ADV(float16);
      val += a_13;
      *stov_ptr(float16,(float*)C) = val; C += SIMD_WIDTH;

      val  = a_20 * __SE0ADV(float16);
      val += a_21 * __SE0ADV(float16);
      val += a_22 * __SE0ADV(float16);
      val += a_23;
      *stov_ptr(float16,(float*)C) = val; C += SIMD_WIDTH;

  }

  __SE0_CLOSE();
}

void SFM_TI_matrixMultiply_3x3By3x1_ci(float A[restrict], float B[restrict], float C[restrict], int32_t num_matrices, uint32_t* pBlock)
{
  float16 val;
  float16 temp;
  int32_t l;

  float16 a_00 = (float16)A[0 * 3 + 0];
  float16 a_01 = (float16)A[0 * 3 + 1];
  float16 a_02 = (float16)A[0 * 3 + 2];
  float16 a_10 = (float16)A[1 * 3 + 0];
  float16 a_11 = (float16)A[1 * 3 + 1];
  float16 a_12 = (float16)A[1 * 3 + 2];
  float16 a_20 = (float16)A[2 * 3 + 0];
  float16 a_21 = (float16)A[2 * 3 + 1];
  float16 a_22 = (float16)A[2 * 3 + 2];

  __SE_TEMPLATE_v1 seTemplate = *(__SE_TEMPLATE_v1*)((uint8_t *)pBlock + SE5_PARAM_OFFSET);
  __SE0_OPEN((void *)B, seTemplate);

  for (l = 0; l < num_matrices; l+= SIMD_WIDTH)
  {
      temp = __SE0ADV(float16);
      val  = a_00 * temp;
      temp = __SE0ADV(float16);
      val += a_01 * temp;
      temp = __SE0ADV(float16);
      val += a_02 * temp;
      *stov_ptr(float16,(float*)C) = val; C += SIMD_WIDTH;

      temp = __SE0ADV(float16);
      val  = a_10 * temp;
      temp = __SE0ADV(float16);
      val += a_11 * temp;
      temp = __SE0ADV(float16);
      val += a_12 * temp;
      *stov_ptr(float16,(float*)C) = val; C += SIMD_WIDTH;

      temp = __SE0ADV(float16);
      val  = a_20 * temp;
      temp = __SE0ADV(float16);
      val += a_21 * temp;
      temp = __SE0ADV(float16);
      val += a_22 * temp;
      *stov_ptr(float16,(float*)C) = val; C += SIMD_WIDTH;

  }

  __SE0_CLOSE();

}

void warping(float* restrict weights, uint8_t* restrict src, uint8_t* restrict dst , int32_t dst_width, int32_t dst_height)
{
  int32_t i,j;
  int32_t ii, jj;

  for(i = 0; i < dst_height; i++){
#pragma UNROLL(16)
    for(j = 0; j < dst_width; j++){
      ii = i*weights[0] + j*weights[1] + weights[2];
      jj = i*weights[3] + j*weights[4] + weights[5];

      dst[ii*dst_height + jj] = src[i*dst_height + j];
    }
  }
}
