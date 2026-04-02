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
***************************************************************************
*  @file         sfm_ti_3dPruning.h
*  @author       Deepak Poddar
*  @brief        This file defines the functions for 3D points pruning
*  @version 0.1 (Oct 2014) : Base version.
***************************************************************************
*/
#ifndef TI_SFM_3DPRUNING_H
#define TI_SFM_3DPRUNING_H

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
                            float*    reprojError);
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
                                   );
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
                             int32_t num3DPoints);

/**
* @func SFM_TI_reprojErrorCalc_ci
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
void SFM_TI_reprojErrorCalc_ci(float * restrict    camPrm,
                               float * restrict    inPoint3D,
                               float * restrict    track2DPoints,
                               uint8_t * restrict curTrackLength,
                               float * restrict    reprojError);

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

void SFM_TI_chiralityPruning_ci(float   * restrict   camPrm,
                                float   * restrict   inPoint3D,
                                uint8_t * restrict   isValid3DPoint,
                                int32_t num3DPoints
                                );
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
                                      uint8_t * restrict   curTrackLength,
                                      float    * restrict   subAngle
                                      );

/** Takes already calculated re projection error and subtended angle, and
* does the final pruning of the 3D points.
*/
void SFM_TI_combinePrun(float* reprojErr,
                        float* subAngle,
                        float  reprojErrTh,
                        float  subAngleTh,
                        uint8_t* valid);

float VLIB_FMAT_AR_mapPoints(float pts2r[restrict],
                                          float pts1r[restrict],
                                          int16_t numPoints,
                                          float F[],
                                          float distThreshold,
                                          int16_t  *numInliers,
                                          uint8_t isInlier[]);

float VXLIB_FMAT_mapPoints_ci(float   inPts2[restrict],
                                     float   inPts1[restrict],
                                     int16_t numPoints,
                                     float   f[restrict],
                                     int32_t fMatPitch,
                                     float   distThreshold,
                                     int16_t *outNumInliers,
                                     uint8_t isInlier[restrict],
                                     void    *pBlock);

#endif





