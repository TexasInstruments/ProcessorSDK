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

/**
* @func SFM_TI_reprojErrorCalc
*
* @par Description:
*   This API finds re projection error for set of 3D points
*   reconstructed. re projection error is calculated by back
*   projecting the 3D point on image plane, and finding the
*   absolute distance between back projected image point and
*   original image point. This process is done for all the
*   frames from which this 3D point is reconstructed.
* @par
*   @param [in]  camPrm         :Base address of the camera extrinsic parameters.
*
*   @param [in]  inPoint3D      :input 3D points.two 3D outputs are
*                                clubbed together. X(or Y or Z) co-ordinates are clubbed together.
*                                e.g X1X0Y1Y0Z1Z0X3X2Y3Y2Z3Z2....so on
*   @param [in]  track2DPoints  : Original image points for each 3D points
*   @param [in]  curTrackLength : Array of values for track length
*   @param [out] reprojError    : Calculated re projection error for
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

void SFM_TI_reprojErrorCalc(float*    camPrm,
                            float*    inPoint3D,
                            float*    track2DPoints,
                            uint8_t*  curTrackLength,
                            float*    reprojError)
{
  uint32_t j,oldestFrmNum,k,l;
  float curX,curY,curZ;
  float projwx,projwy,projw;
  float projx,projy;
  float* curPMat;
  float curx,cury;
  float rprjErr;
  float tempf;
  uint16_t curTrackLengthL;


  for(l = 0; l < NUM_TRACKS_TOGATHER; l+=VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR){

    for(k = 0; k < VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; k++){

      curTrackLengthL = curTrackLength[l+k];
      oldestFrmNum = VLIB_TRIANG_MAX_POINTS_IN_TRACK - curTrackLengthL;

      curX  = inPoint3D[0*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];
      curY  = inPoint3D[1*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];
      curZ  = inPoint3D[2*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];

      rprjErr        = 0.0f;
      curPMat        = camPrm;
      curPMat       += (oldestFrmNum*VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE);

      for(j=(VLIB_TRIANG_MAX_POINTS_IN_TRACK - curTrackLengthL);j< VLIB_TRIANG_MAX_POINTS_IN_TRACK;j++){

        curx = track2DPoints[2*j*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + 0                        + k];
        cury = track2DPoints[2*j*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];

        projwx = curPMat[0] * curX +
          curPMat[1] * curY +
          curPMat[2] * curZ +
          curPMat[3];

        projwy = curPMat[4] * curX +
          curPMat[5] * curY +
          curPMat[6] * curZ +
          curPMat[7];

        projw  = curPMat[8] * curX +
          curPMat[9] * curY +
          curPMat[10] * curZ +
          curPMat[11];

        projx  = projwx * my_OneByX(projw);
        projy  = projwy * my_OneByX(projw);


        tempf    = ((projx - curx)*(projx - curx)) +
          ((projy - cury)*(projy - cury));

        tempf    = tempf * my_OneBySqrtX(tempf);

        rprjErr += tempf;
        curPMat += VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE;
      }
      reprojError[l+k] = rprjErr * my_OneByX(curTrackLengthL);
    }
    track2DPoints   += 2*VLIB_TRIANG_MAX_POINTS_IN_TRACK*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
    inPoint3D += (VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*3);
  }
}
/**
* @func SFM_TI_subtendedAnglePrun
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

uint16_t SFM_TI_subtendedAnglePrun(float*    camPrm,
                                   float*    inPoint3D,
                                   uint8_t*  curTrackLength,
                                   float*    subAngle
                                   )
{
  int32_t i,j, k,l;//,latestFrmNum;
  float curX,curY,curZ;
  float diffX,diffY,diffZ;
  float norm,dot;
  float* curPMat;
  float t1x,t1y,t1z;
  float t2x,t2y,t2z;
  float temp;

  float r1x,r1y,r1z;
  float r2x,r2y,r2z;
  uint16_t curTrackLengthL;

  for(l = 0; l < NUM_TRACKS_TOGATHER; l+= VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR){
    for (k = 0; k < VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; k++) {

      curX  = inPoint3D[0*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];
      curY  = inPoint3D[1*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];
      curZ  = inPoint3D[2*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];

      curPMat        = camPrm;
      curTrackLengthL= curTrackLength[l+k];

      for(i = 0; i < 1; i++) {

        t1x = curPMat[(VLIB_TRIANG_MAX_POINTS_IN_TRACK - i - 1)*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 3];
        t1y = curPMat[(VLIB_TRIANG_MAX_POINTS_IN_TRACK - i - 1)*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 7];
        t1z = curPMat[(VLIB_TRIANG_MAX_POINTS_IN_TRACK - i - 1)*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11];

        diffX = curX - t1x;
        diffY = curY - t1y;
        diffZ = curZ - t1z;

        temp = (diffX*diffX + diffY*diffY + diffZ*diffZ);
        norm = my_OneBySqrtX(temp);

        r1x  = diffX * norm;
        r1y  = diffY * norm;
        r1z  = diffZ * norm;
       
        dot = 0.0f;

        for(j = (curTrackLengthL - 1); j < curTrackLengthL; j++) {

          t2x = curPMat[(VLIB_TRIANG_MAX_POINTS_IN_TRACK - j - 1)*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 3];
          t2y = curPMat[(VLIB_TRIANG_MAX_POINTS_IN_TRACK - j - 1)*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 7];
          t2z = curPMat[(VLIB_TRIANG_MAX_POINTS_IN_TRACK - j - 1)*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11];

          diffX = curX - t2x;
          diffY = curY - t2y;
          diffZ = curZ - t2z;

          temp = (diffX*diffX + diffY*diffY + diffZ*diffZ);
          norm = my_OneBySqrtX(temp);

          r2x  = diffX * norm;
          r2y  = diffY * norm;
          r2z  = diffZ * norm;

          dot = r1x*r2x + r1y*r2y + r1z*r2z;

          if(dot > 0.9999999f) {
            dot = 0.9999999f;
          }

          if(dot < -0.9999999f) {
            dot = -0.9999999f;
          }
        }
      }
      subAngle[l + k]   = dot;
    }
    inPoint3D += (VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*3);
  }
  //return(num3DPoints);
  return(1);
}
/**
* @func SFM_TI_chiralityPruning
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

void SFM_TI_chiralityPruning(float*    camPrm,
                             float*    inPoint3D,
                             uint8_t*  isValid3DPoint,
                             int32_t num3DPoints
                            )
{
  int32_t l,k,j;
  float w;
  //uint16_t localDropNum = 0;
  float curX,curY,curZ;
  for(l = 0; l < num3DPoints; l+= VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR)
  {
    for (k = 0; k < VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; k++)
    {
      curX  = inPoint3D[0*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];
      curY  = inPoint3D[1*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];
      curZ  = inPoint3D[2*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k];

      for(j=0; j<VLIB_TRIANG_MAX_POINTS_IN_TRACK; j++)
      {
        w =
          (camPrm[j*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 8]  * curX  +
          camPrm[j*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 9]  * curY  +
          camPrm[j*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 10] * curZ +
          camPrm[j*(VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE) + 11] );

        if(w < 0.0f)
        {
          isValid3DPoint[l + k] = 0;
        }
      }
    }
    inPoint3D += (VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*3);
  }
  return;
}

VLIB_F32 VLIB_FMAT_AR_mapPoints(VLIB_F32 pts2r[restrict],
                                          VLIB_F32 pts1r[restrict],
                                          int16_t numPoints,
                                          VLIB_F32 F[],
                                          VLIB_F32 distThreshold,
                                          int16_t  *numInliers,
                                          uint8_t isInlier[])
{
    int32_t     i;
    VLIB_F32    dT = (VLIB_F32) distThreshold * distThreshold;
    VLIB_F32    reproject_err_sum = 0.0;
    VLIB_F32    reproject_err = 0.0;
    int32_t     k = 0;
    VLIB_F32    curX, curY, prevX, prevY;



    for( i = 0; i < numPoints; i++ ) {
        VLIB_F32    a, b, c, d1, d2, s1, s2;

        curX = pts2r[(2 * i) + 0];
        curY = pts2r[(2 * i) + 1];

        prevX = pts1r[(2 * i) + 0];
        prevY = pts1r[(2 * i) + 1];

        /* Prev point */
        a = F[0] * prevX + F[1] * prevY + F[2];
        b = F[3] * prevX + F[4] * prevY + F[5];
        c = F[6] * prevX + F[7] * prevY + F[8];

        s2 = VLIB_OneByXF32(a * a + b * b);
        d2 = curX * a + curY * b + c;

        /* Current Point */
        a = F[0] * curX + F[3] * curY + F[6];
        b = F[1] * curX + F[4] * curY + F[7];
        c = F[2] * curX + F[5] * curY + F[8];

        s1 = VLIB_OneByXF32(a * a + b * b);
        d1 = prevX * a + prevY * b + c;

        reproject_err = __mmax(d1 * d1 * s1, d2 * d2 * s2);

        if( reproject_err <= dT ) {
            isInlier[i] = 0x1U;
            k++;
            reproject_err_sum += reproject_err;
        } else {
            isInlier[i] = 0x0U;
        }
    }

    *numInliers = (int16_t)k;

    return (reproject_err_sum);
}

/** Takes already calculated re projection error and subtended angle, and
* does the final pruning of the 3D points.
*/
void SFM_TI_combinePrun(float * restrict reprojErr,
                        float * restrict subAngle,
                        float  reprojErrTh,
                        float  subAngleTh,
                        uint8_t* restrict valid)
{
  int32_t l, validL;
  float    minReprojErrTh; ;
  float    minSubAngleTh;
  float    cosineTh1,cosineTh2,cosineTh3,cosineTh4,cosineTh5;
  float    dotpL,reprojErrL;

  minReprojErrTh = reprojErrTh * my_OneByX(16.0f);
  minSubAngleTh  = subAngleTh  * my_OneByX(16.0f);

#if (HOST_EMULATION)
  minSubAngleTh  = minSubAngleTh * SFM_TI_PI * my_OneByX(180);

  cosineTh5      = cos(minSubAngleTh);
  cosineTh4      = cos(minSubAngleTh*2.0f);
  cosineTh3      = cos(minSubAngleTh*4.0f);
  cosineTh2      = cos(minSubAngleTh*8.0f);
  cosineTh1      = cos(minSubAngleTh*16.0f);
#else

  minSubAngleTh  = minSubAngleTh * my_YByX(SFM_TI_PI,180);

  cosineTh5      = cos(minSubAngleTh);
  cosineTh4      = cos(minSubAngleTh*2.0f);
  cosineTh3      = cos(minSubAngleTh*4.0f);
  cosineTh2      = cos(minSubAngleTh*8.0f);
  cosineTh1      = cos(minSubAngleTh*16.0f);
#endif

  for(l = 0; l < NUM_TRACKS_TOGATHER; l++){

    validL    = 0;
    dotpL      = subAngle[l];
    reprojErrL = reprojErr[l];

    if ( (dotpL < cosineTh1) && (reprojErrL <= reprojErrTh) ) {
      validL = 1;
    }
    if ( (dotpL < cosineTh2 ) && (reprojErrL <= (minReprojErrTh*8)) ) {
      validL = 1;
    }
    if ( (dotpL < cosineTh3 ) && (reprojErrL <= (minReprojErrTh*4))) {
      validL = 1;
    }
    if ( (dotpL < cosineTh4 ) && (reprojErrL <= (minReprojErrTh*2))) {
      validL = 1;
    }
    if ( (dotpL < cosineTh5 ) && (reprojErrL <= minReprojErrTh)) {
      validL = 1;
    }
    valid[l] = (uint8_t)validL;
  }
}

