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
*  @file        sfm_ti_3dPruning.c
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
#include "sfm_ti_3dPruning.h"
#include "sfm_ti_math.h"
#include <c7x.h>

void SFM_TI_reprojErrorCalc_ci(float * restrict    camPrm,
                               float * restrict    inPoint3D,
                               float * restrict    track2DPoints,
                               uint8_t * restrict  curTrackLength,
                               float * restrict    reprojError)
{
  uint32_t j,l;
  float16 projwx1x0,projwy1y0,projw1w0;
  float * restrict curPMatL;
  float * restrict track2DPointsL;

  float16 rprjErr;

  float16   curX1X0;
  float16   curY1Y0;
  float16   curZ1Z0;


  float16 f2temp0,f2temp1,f2temp2,f2temp3;
  float16 curx1x0,cury1y0;

  float16 tempResult[VXLIB_TRIANG_MAX_POINTS_IN_TRACK];
  char64  vTracklLength;
  float16 vfTracklLength;

  uint32_t maxTrackLength;

  camPrm          += ((VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 1)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE));
  track2DPoints   += (2*VXLIB_TRIANG_MAX_POINTS_IN_TRACK*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR - VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR);

  for(l = 0; l < NUM_TRACKS_TOGATHER; l+=VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR){

#if 0
    maxTrackLength = 0;
    for(k = 0; k < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; k++){
      maxTrackLength          = __mmax(maxTrackLength, curTrackLength[l+k]);
    }
#else
    maxTrackLength = VXLIB_TRIANG_MAX_POINTS_IN_TRACK;
#endif

    vTracklLength = *stov_ptr(char64,(int8_t*)&curTrackLength[l]);

    rprjErr   = (float16)0.0f;
    curPMatL  = camPrm;
    track2DPointsL = track2DPoints;

    curX1X0          = *stov_ptr(float16,inPoint3D); inPoint3D += SIMD_WIDTH;
    curY1Y0          = *stov_ptr(float16,inPoint3D); inPoint3D += SIMD_WIDTH;
    curZ1Z0          = *stov_ptr(float16,inPoint3D); inPoint3D += SIMD_WIDTH;

    /* Loop overhead is high fr below loop. It is around 60
    cycle. Same time it is possible that core loop cycle
    is lower than loop pipe up cycle.
    */
#pragma MUST_ITERATE(0,VXLIB_TRIANG_MAX_POINTS_IN_TRACK,)
    for(j = 0;j < maxTrackLength;j++){

      cury1y0         = *stov_ptr(float16,(float*)track2DPointsL);
      track2DPointsL  = track2DPointsL - SIMD_WIDTH;

      curx1x0         = *stov_ptr(float16,(float*)track2DPointsL);
      track2DPointsL  = track2DPointsL - SIMD_WIDTH;

      f2temp0        = (float16)curPMatL[8];
      f2temp1        = (float16)curPMatL[9];

      f2temp0        = f2temp0 * curX1X0;
      f2temp1        = f2temp1 * curY1Y0;
      f2temp0        = f2temp0 + f2temp1;

      f2temp2        = (float16)curPMatL[10];
      f2temp3        = (float16)curPMatL[11];

      f2temp2        = f2temp2 * curZ1Z0;
      projw1w0       = f2temp0 + f2temp2;
      projw1w0       = projw1w0 + f2temp3;
      projw1w0       = VXLIB_oneByXVecF32(projw1w0);

      f2temp0        = (float16)curPMatL[0];
      f2temp1        = (float16)curPMatL[1];
      f2temp0        = f2temp0 * curX1X0;
      f2temp1        = f2temp1 * curY1Y0;

      f2temp2        = (float16)curPMatL[2];
      f2temp3        = (float16)curPMatL[3];
      f2temp2        = f2temp2 * curZ1Z0;
      projwx1x0      = f2temp0 + f2temp1 + f2temp2;
      projwx1x0      = projwx1x0 + f2temp3;
      projwx1x0      = projw1w0 * projwx1x0;


      f2temp0        = (float16)curPMatL[4];
      f2temp1        = (float16)curPMatL[5];
      f2temp0        = f2temp0 * curX1X0;
      f2temp1        = f2temp1 * curY1Y0;

      f2temp2        = (float16)curPMatL[6];
      f2temp3        = (float16)curPMatL[7];
      f2temp2        = f2temp2 * curZ1Z0;
      projwy1y0      = f2temp0 + f2temp1 + f2temp2;
      projwy1y0      = projwy1y0 + f2temp3;
      projwy1y0      = projw1w0 * projwy1y0;

      curx1x0        = projwx1x0 - curx1x0;
      curx1x0        = curx1x0 * curx1x0;
      cury1y0        = projwy1y0 - cury1y0;
      cury1y0        = cury1y0 * cury1y0;

      f2temp1        = cury1y0 + curx1x0;

      tempResult[j]  = f2temp1;
      curPMatL -= VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE;
    }
    /* Loop overhead is high fr below loop. It is around 60
    cycle. Same time it is possible that core loop cycle
    is lower than loop pipe up cycle.
    */
#pragma MUST_ITERATE(0,VXLIB_TRIANG_MAX_POINTS_IN_TRACK,)

    for(j=0;j< maxTrackLength;j++){

      f2temp1      = tempResult[j];
      f2temp2      = VXLIB_oneBySqrtXVecF32(f2temp1);
      f2temp1      = f2temp1*f2temp2;
      __vpred pred = __cmp_ge_pred(vTracklLength, (char64)(j + 1));
              pred = __pdupl2b_pp(pred);
              pred = __pdupl2b_pp(pred);
      f2temp1      = __select(pred, f2temp1, (float16)0.0f);
      rprjErr      = rprjErr+ f2temp1;
    }
    char16 vTemp1;
    vTemp1 = *stov_ptr(char16,(int8_t*)&curTrackLength[l]);

    vfTracklLength = __int_to_float(__convert_int16(vTemp1));
    __vpred pred   = __cmp_eq_pred(vfTracklLength,(float16)0.0f);

    f2temp1           = VXLIB_oneByXVecF32(vfTracklLength);

    *stov_ptr(float16,(float*)&reprojError[l]) = __select(pred,(float16)FLT_MAX,rprjErr * f2temp1);

    track2DPoints += 2*VXLIB_TRIANG_MAX_POINTS_IN_TRACK*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
  }
}
/**
* @func SFM_TI_chiralityPruning_ci
*
* @par Description:
*   This API finds subtended angle for each 3D points
* @par
*   @param [in]  camPrm         :Base address of the camera extrinsic parameters.
*
*   @param [in]  inPoint3D      :input 3D points.two 3D outputs are
*                                clubbed together. X(or Y or Z) co-ordinates are clubbed together.
*                                e.g X1X0Y1Y0Z1Z0X3X2Y3Y2Z3Z2....so on
*   @param [out] isValid3DPoint : If isValid3DPoint[j] is '1' then it indicates that
*                                 it has passed 'z' checking, otherwise it has failed
*                                 jth 3D point
*   @param [in] point3Dindex    : Not used currently
*   @param [in] num3DPoints     : Total number of input 3D points given to this API
*
*
*
* @par Assumptions:
*    -   NA
*
* @par Implementation Notes:
*    - NA
*
*/
Void SFM_TI_chiralityPruning_ci(float   * restrict   camPrm,
                                float   * restrict   inPoint3D,
                                uint8_t * restrict   isValid3DPoint,
                                int32_t num3DPoints
                                )
{
  int32_t id,j;
  float16 w;

  float16 vTemp1, vTemp2, vTemp3, vTemp4, vCurX, vCurY, vCurZ;
  __vpred pred;
  __vpred cur_pred;
  char16 prevValidFlag;
  float16 w_th = (float16)0.0001f;

  for (id=0; id<num3DPoints; id += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR) {
    vCurX    = *stov_ptr(float16,inPoint3D); inPoint3D += SIMD_WIDTH;
    vCurY    = *stov_ptr(float16,inPoint3D); inPoint3D += SIMD_WIDTH;
    vCurZ    = *stov_ptr(float16,inPoint3D); inPoint3D += SIMD_WIDTH;
    prevValidFlag     = *stov_ptr(char16,(char*)&isValid3DPoint[id]); // all valid
    pred              = __create_vpred(0xFFFFFFFFFFFFFFFFU);

    for(j=0; j<VLIB_TRIANG_MAX_POINTS_IN_TRACK; j++) {

      vTemp1      = (float16)camPrm[j*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 8];
      vTemp2      = (float16)camPrm[j*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 9];
      vTemp3      = (float16)camPrm[j*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 10];
      vTemp4      = (float16)camPrm[j*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11];

      w           = (vCurX*vTemp1 + vCurY*vTemp2 + vCurZ*vTemp3 + vTemp4);

      cur_pred    = __cmp_le_pred(w_th,w); // 0.0 <= w then valid or w>=0.0 then valid
      pred        = __and(pred,cur_pred);
    }

    pred = __decimate_char(pred,pred);
    pred = __decimate_char(pred,pred);

    prevValidFlag = prevValidFlag & __expand(pred).lo().lo();

    *stov_ptr(char16,(char*)&isValid3DPoint[id]) = prevValidFlag;
  }
  return;
}


/**
* @func SFM_TI_subtendedAnglePrun_ci
*
* @par Description:
*   This API finds subtended angle for each 3D points
* @par
*   @param [in]  camPrm         :Base address of the camera extrinsic parameters.
*
*   @param [in]  inPoint3D      :input 3D points.two 3D outputs are
*                                clubbed together. X(or Y or Z) co-ordinates are clubbed together.
*                                e.g X1X0Y1Y0Z1Z0X3X2Y3Y2Z3Z2....so on
*   @param [in]  curTrackLength : Array of values for track length
*   @param [out] subAngle       : Calculated out subtended angle for
*                                 each 3D points
*
*
*
* @par Assumptions:
*    -   NA
*
* @par Implementation Notes:
*    - NA
*
*/
uint16_t SFM_TI_subtendedAnglePrun_ci(float    * restrict   camPrm,
                                      float    * restrict   inPoint3D,
                                      uint8_t  * restrict   curTrackLength,
                                      float    * restrict   subAngle
                                      )
{
  uint32_t l;
  float16 curX1X0,curY1Y0,curZ1Z0;
  float16 diffX1X0,diffY1Y0,diffZ1Z0;

  float* restrict curPMat;
  float16 t1x,t1y,t1z;
  float16 t2x,t2y,t2z;
  float16 t3x,t3y,t3z;
  float16 t4x,t4y,t4z;
  float16 t5x,t5y,t5z;
  float16 t6x,t6y,t6z;
  float16 t2x1x0,t2y1y0,t2z1z0;

  float16 r1x1x0,r1y1y0,r1z1z0;
  float16 r2x1x0,r2y1y0,r2z1z0;

  char16 vCurTrackLength;
  int16  vIntCurTrackLength;
  int16  vPkd3 = (int16)3;
  int16  vPkd4 = (int16)4;
  int16  vPkd5 = (int16)5;
  int16  vPkd6 = (int16)6;

  float16 f2norm,f2temp1,f2dot;
  float16 vfMaxAngle = (float16)0.9999999f;
  float16 vfMinAngle = (float16)-0.9999999f;

  curPMat           = camPrm;

  _nassert(VXLIB_TRIANG_MAX_POINTS_IN_TRACK == 6);

  //latest Pmat[5]
  t1x = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 1)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 3];
  t1y = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 1)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 7];
  t1z = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 1)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11];

  //second latest Pmat[4]
  t2x = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 2)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 3];
  t2y = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 2)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 7];
  t2z = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 2)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11];

  //third latest Pmat[3]
  t3x = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 3)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 3];
  t3y = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 3)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 7];
  t3z = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 3)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11];

  //fourth latest Pmat[2]
  t4x = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 4)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 3];
  t4y = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 4)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 7];
  t4z = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 4)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11];

  //Pmat[1]
  t5x = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 5)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 3];
  t5y = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 5)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 7];
  t5z = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 5)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11];

  //Pmat[0]
  t6x = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 6)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 3];
  t6y = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 6)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 7];
  t6z = (float16)curPMat[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 6)*(VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11];

  for(l = 0; l < NUM_TRACKS_TOGATHER; l+= VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR){

      curX1X0    = *stov_ptr(float16,inPoint3D); inPoint3D += SIMD_WIDTH;
      curY1Y0    = *stov_ptr(float16,inPoint3D); inPoint3D += SIMD_WIDTH;
      curZ1Z0    = *stov_ptr(float16,inPoint3D); inPoint3D += SIMD_WIDTH;

      vCurTrackLength    = *stov_ptr(char16,(char*)&curTrackLength[l]);
      vIntCurTrackLength = __convert_int16(vCurTrackLength);

      diffX1X0 = curX1X0 - t1x; //latest
      diffY1Y0 = curY1Y0 - t1y;
      diffZ1Z0 = curZ1Z0 - t1z;

      f2temp1 = (diffX1X0*diffX1X0) + (diffY1Y0*diffY1Y0) + (diffZ1Z0*diffZ1Z0);

      f2norm  = VXLIB_oneBySqrtXVecF32(f2temp1);

      r1x1x0  = diffX1X0*f2norm;
      r1y1y0  = diffY1Y0*f2norm;
      r1z1z0  = diffZ1Z0*f2norm;

      t2x1x0  = t2x;
      t2y1y0  = t2y;
      t2z1z0  = t2z;

      __vpred pred = __cmp_eq_pred(vIntCurTrackLength,vPkd3);
      t2x1x0       = __select(pred,t3x,t2x1x0);
      t2y1y0       = __select(pred,t3y,t2y1y0);
      t2z1z0       = __select(pred,t3z,t2z1z0);

              pred = __cmp_eq_pred(vIntCurTrackLength,vPkd4);
      t2x1x0       = __select(pred,t4x,t2x1x0);
      t2y1y0       = __select(pred,t4y,t2y1y0);
      t2z1z0       = __select(pred,t4z,t2z1z0);

              pred = __cmp_eq_pred(vIntCurTrackLength,vPkd5);
      t2x1x0       = __select(pred,t5x,t2x1x0);
      t2y1y0       = __select(pred,t5y,t2y1y0);
      t2z1z0       = __select(pred,t5z,t2z1z0);

              pred = __cmp_eq_pred(vIntCurTrackLength,vPkd6);
      t2x1x0       = __select(pred,t6x,t2x1x0);
      t2y1y0       = __select(pred,t6y,t2y1y0);
      t2z1z0       = __select(pred,t6z,t2z1z0);


      diffX1X0 = curX1X0 - t2x1x0;
      diffY1Y0 = curY1Y0 - t2y1y0;
      diffZ1Z0 = curZ1Z0 - t2z1z0;

      f2temp1 = (diffX1X0*diffX1X0) + (diffY1Y0*diffY1Y0) + (diffZ1Z0*diffZ1Z0);

      f2norm  = VXLIB_oneBySqrtXVecF32(f2temp1);

      r2x1x0  = diffX1X0 * f2norm;
      r2y1y0  = diffY1Y0 * f2norm;
      r2z1z0  = diffZ1Z0 * f2norm;

      r2x1x0  = r2x1x0 * r1x1x0;
      r2y1y0  = r2y1y0 * r1y1y0;
      r2z1z0  = r2z1z0 * r1z1z0;

      f2dot   = r2x1x0 + r2y1y0 + r2z1z0;

      pred    = __cmp_lt_pred(vfMaxAngle,f2dot);
      f2dot   = __select(pred,vfMaxAngle,f2dot);

      pred    = __cmp_lt_pred(f2dot,vfMinAngle);
      f2dot   = __select(pred,vfMinAngle,f2dot);

      *stov_ptr(float16,(float*)&subAngle[l])  = f2dot;
    }
  return(1);
}
#define STRM_ENGINE
float VXLIB_FMAT_mapPoints_ci(float   inPts2[restrict],
                                     float   inPts1[restrict],
                                     int16_t numPoints,
                                     float   f[restrict],
                                     int32_t fMatPitch,
                                     float   distThreshold,
                                     int16_t *outNumInliers,
                                     uint8_t isInlier[restrict],
                                     void    *pBlock)
{
  int32_t     i;
  float16    dT = (float16) distThreshold * distThreshold;
  float16    reprojectErrSum = (float16)0.0f;
  float16    reproject_err = (float16)0.0f;
  int32_t    k = 0;
  float16    vfCurX, vfCurY, vfPrevX, vfPrevY;
  float16    a, b, c, d1, d2, s1, s2;

  float16  vfFpkd[9];

  float   totReprojErr         = 0.0f;
  //int32_t numVec               = (int32_t)((numPoints + SIMD_WIDTH - 1) * my_OneByX(SIMD_WIDTH));
  int32_t numVec               = (int32_t)((numPoints) * my_OneByX(SIMD_WIDTH));
  float16 vfTemp1, vfTemp2;
  uchar64 vpattern;
  uchar pattern[64];

#ifdef STRM_ENGINE
  __SE_TEMPLATE_v1 seTemplate = *(__SE_TEMPLATE_v1*)((uint8_t *)pBlock + 0);
  __SE0_OPEN((void *)inPts2, seTemplate);
  __SE1_OPEN((void *)inPts1, seTemplate);
#endif

  vfFpkd[0] = (float16)f[0 * fMatPitch];
  vfFpkd[1] = (float16)f[1 * fMatPitch];
  vfFpkd[2] = (float16)f[2 * fMatPitch];
  vfFpkd[3] = (float16)f[3 * fMatPitch];
  vfFpkd[4] = (float16)f[4 * fMatPitch];
  vfFpkd[5] = (float16)f[5 * fMatPitch];
  vfFpkd[6] = (float16)f[6 * fMatPitch];
  vfFpkd[7] = (float16)f[7 * fMatPitch];
  vfFpkd[8] = (float16)f[8 * fMatPitch];

  for(i = 0; i < 64; i++){
    pattern[i] = (uchar)i;
  }

  vpattern = *stov_ptr(uchar64, (uchar*)pattern);

  /* Loop is bottelnecked by multiplication. Total 32 vector multiplication
   * is there, so ii should have been 16. But because of register pressure
   * ii jumps to 20.
  */

  for( i = 0; i < numVec; i++) {

#ifdef STRM_ENGINE
    vfTemp1 = __SE0ADV(float16);
    vfTemp2 = __SE0ADV(float16);
#else
    vfTemp1 = *stov_ptr(float16,(float*)inPts2);inPts2 += SIMD_WIDTH;
    vfTemp2 = *stov_ptr(float16,(float*)inPts2);inPts2 += SIMD_WIDTH;
#endif

    vfCurX = __as_float16(__permute_even_even_int(vpattern, __as_uchar64(vfTemp2), __as_uchar64(vfTemp1)));
    vfCurY = __as_float16(__permute_odd_odd_int(vpattern, __as_uchar64(vfTemp2), __as_uchar64(vfTemp1)));
#if TRACE_ON
    PRINTV("vfCurX", __as_long8(vfCurX), PRINTV_FLOAT, PRINTV_HEX_OFF);
    PRINTV("vfCurY", __as_long8(vfCurY), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

#ifdef STRM_ENGINE
    vfTemp1 = __SE1ADV(float16);
    vfTemp2 = __SE1ADV(float16);
#else
    vfTemp1 = *stov_ptr(float16,(float*)inPts1);inPts1 += SIMD_WIDTH;
    vfTemp2 = *stov_ptr(float16,(float*)inPts1);inPts1 += SIMD_WIDTH;
#endif

    vfPrevX = __as_float16(__permute_even_even_int(vpattern, __as_uchar64(vfTemp2), __as_uchar64(vfTemp1)));
    vfPrevY = __as_float16(__permute_odd_odd_int(vpattern, __as_uchar64(vfTemp2), __as_uchar64(vfTemp1)));

#if TRACE_ON
    PRINTV("vfPrevX", __as_long8(vfPrevX), PRINTV_FLOAT, PRINTV_HEX_OFF);
    PRINTV("vfPrevY", __as_long8(vfPrevY), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

    /* Prev point */
    a = vfFpkd[0] * vfPrevX + vfFpkd[1] * vfPrevY + vfFpkd[2];
    b = vfFpkd[3] * vfPrevX + vfFpkd[4] * vfPrevY + vfFpkd[5];
    c = vfFpkd[6] * vfPrevX + vfFpkd[7] * vfPrevY + vfFpkd[8];

    /* TODO : increase the precision of the division
    */
    s2 = VXLIB_oneByXVecF32(a * a + b * b);
    d2 = vfCurX * a + vfCurY * b + c;

    /* Current Point */
    a = vfFpkd[0] * vfCurX + vfFpkd[3] * vfCurY + vfFpkd[6];
    b = vfFpkd[1] * vfCurX + vfFpkd[4] * vfCurY + vfFpkd[7];
    c = vfFpkd[2] * vfCurX + vfFpkd[5] * vfCurY + vfFpkd[8];

    s1 = VXLIB_oneByXVecF32(a * a + b * b);
    d1 = vfPrevX * a + vfPrevY * b + c;

    reproject_err = __max(d1 * d1 * s1, d2 * d2 * s2);

    __vpred flag = __cmp_le_pred(reproject_err,dT);
    k = k + (__popcount_char(flag) >> 0x2);

    flag = __decimate_char(flag, flag);
    flag = __decimate_char(flag, flag);

    *stov_ptr(char16, (char*)isInlier)  =  __expand(flag).lo().lo();
    isInlier += SIMD_WIDTH;

    reprojectErrSum = reprojectErrSum + reproject_err;
  }

#ifdef STRM_ENGINE
  __SE0_CLOSE();
  __SE1_CLOSE();
#endif
  /*currently all the points leftover outside SIMD multiple is set as outlier*/
  for(i = numVec*SIMD_WIDTH; i < numPoints; i++)
  {
    *isInlier++ = 0;
  }

  *outNumInliers = (int16_t)k;

  totReprojErr += reprojectErrSum.s[0];
  totReprojErr += reprojectErrSum.s[1];
  totReprojErr += reprojectErrSum.s[2];
  totReprojErr += reprojectErrSum.s[3];
  totReprojErr += reprojectErrSum.s[4];
  totReprojErr += reprojectErrSum.s[5];
  totReprojErr += reprojectErrSum.s[6];
  totReprojErr += reprojectErrSum.s[7];
  totReprojErr += reprojectErrSum.s[8];
  totReprojErr += reprojectErrSum.s[9];
  totReprojErr += reprojectErrSum.s[10];
  totReprojErr += reprojectErrSum.s[11];
  totReprojErr += reprojectErrSum.s[12];
  totReprojErr += reprojectErrSum.s[13];
  totReprojErr += reprojectErrSum.s[14];
  totReprojErr += reprojectErrSum.s[15];

  return (totReprojErr);
}

