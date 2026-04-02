/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2007-2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/

#ifndef VLIB_TRIANGULATEPOINT_CN_H_
#define VLIB_TRIANGULATEPOINT_CN_H_ 1

#include "VLIB_triangulatePoints_types.h"

int32_t VLIB_triangulatePoints_cn(float *curTrack,
                                  float *camExtPrm,
                                  uint8_t *curTrackLength,
                                  uint32_t maxIter,
                                  float *Xcam,
                                  float *scratch,
                                  uint32_t totalTracks,
                                  uint8_t *validOut,
                                  uint32_t flag);
#endif
/* ======================================================================== */
/*  End of file:  VLIB_triangulatePoints_cn.h                    */
/* ======================================================================== */

