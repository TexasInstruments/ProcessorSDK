/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2007-2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/

#ifndef VLIB_TRIANGULATEPOINT_H_
#define VLIB_TRIANGULATEPOINT_H_ 1

/** @defgroup VLIB_triangulatePoints */
/** @ingroup  VLIB_triangulatePoints */
/* @{ */

/**
 *
 * @par Description:
 *   This API takes set of tracked 2D feature points from which 3D points are estimated.
 *
 * @par
 *   @param [in]  curTrack[]      : Current track data after multiplying with intrinsic parameters.
 *                                  i.e. [x,y,1] = K * [x',y',1];
 *                                  Two track data are clubbed together. x/y-coordinates of two tracks are
 *                                  placed together. Lets assume two tracks are
 *                                  {(x0,y0),(x1,y1),(x2,y2),(x3,y3),(x4,y4),(x5,y5)} &
 *                                  {(u0,v0),(u1,v1),(u2,v2),(u3,v3),(u4,v4),(u5,v5)}. (xi,yi) is the feature
 *                                  point location in ith frame for particular track. (ui,vi) is the feature
 *                                  point location in ith frame for another track. Expected format for curTrack
 *                                  is curTrack[] = {x0, u0, y0, v0, x1, u1, y1, v1, .. so on}. Here (x0,y0) or
 *                                  (u0,v0) is latest location from current frame, whereas (x1,y1) or (u1,v1) is
 *                                  previous frame location, and so on. If any frame location is not available for
 *                                  a track then its corresponding place in /c curTrack is dont care.
 *
 *   @param [in]  camExtPrm      : Camera extrensic parameters. 0th index should hold latest frame camera extrinsic
 *                                 parameter.
 *
 *   @param [in]  curTrackLength : Array of values for each track lengths.
 *
 *   @param [in]  maxIter       : Maximum iteration of triangulation.
 *
 *   @param [out] Xcam          : 3-D output generated after triangulation API. Two 3D outputs are
 *                                clubbed together. X(or Y or Z) co-ordinates are clubbed together.
 *                                e.g X1X0Y1Y0Z1Z0X3X2Y3Y2Z3Z2....so on. Here [X0, Y0, Z0] is one 3D
 *                                point form one track, whereas [X1, Y1, Z1] is another 3D point output
 *                                from another track.
 *
 *   @param [in]  scratch       : Scratch area to be used, size of this scratch area is N*1540, where 'N'
 *                                is the total number of input track pair. If total input track is 5, then
 *                                'N' should be assumed as 3.
 *
 *   @param [in]  totalTracks   : Total number of tracks to be processed.
 *
 *   @param [out] validOut      : Output flag for each track, wheather a solution was found or not. '1' indicates
 *                                a solution was found, whereas '0' indicates solution could not be found.
 *
 *   @param [in]  flag          : Flag to enable or disable high precision triangulation. '1' will enable high
 *                                precision triangulation.
 *
 * @par Implementation Notes:
 * - This code is implemented for floating point DSP processor e.g. C6600
 * - Recommended value of \c maxIter is 10. However lower value of it will reduce the cycle consumption without
 *   affecting the quality of output much.
 * - Camera extrensic parameters \c camExtPrm are expected in specific format. Original camera extrensic ('P')parameter is
 *   of size [3x4]. Eexpected camera extrensic parameter here is of size [20]. Format for this is as
 *   {P(8),P(0),P(4),P(9),P(1),P(5),P(10),P(11),P(2),P(6),P(3),P(7),P(8),P(8),P(9),P(9),P(10),P(10),P(11),P(11)}.
 *   Some of the data elements in array is replicated to avaoid few overheads inside kernel.
 *
 * @par Benchmarks:
 *  See VLIB_Test_Report.html for cycle and memory information.
 *
*/

VXLIB_STATUS VXLIB_triangulatePoints_i32f_o32f_init(VXLIB_F32 curTrack[],
                                             VXLIB_F32 camExtPrm[],
                                             uint8_t   curTrackLength[],
                                             int32_t  maxIter,
                                             VXLIB_F32 scratch[],
                                             int32_t  totalTracks,
                                             int32_t  precisionFlag,
                                             VXLIB_F32 outXcam[],
                                             uint8_t   outValid[],
                                             void *pBlock);

VXLIB_STATUS VXLIB_triangulatePoints_i32f_o32f_checkParams(VXLIB_F32 curTrack[],
                                             VXLIB_F32 camExtPrm[],
                                             uint8_t   curTrackLength[],
                                             int32_t  maxIter,
                                             VXLIB_F32 scratch[],
                                             int32_t  totalTracks,
                                             int32_t  precisionFlag,
                                             VXLIB_F32 outXcam[],
                                             uint8_t   outValid[]);

VXLIB_STATUS VXLIB_triangulatePoints_i32f_o32f_kernel(VXLIB_F32 curTrack[],
                                             VXLIB_F32 camExtPrm[],
                                             uint8_t   curTrackLength[],
                                             int32_t  maxIter,
                                             VXLIB_F32 scratch[],
                                             int32_t  totalTracks,
                                             int32_t  precisionFlag,
                                             VXLIB_F32 outXcam[],
                                             uint8_t   outValid[],
                                             void *pBlock);

#endif /* VLIB_TRIANGULATEPOINT_H_ */
/** @} */

/* ======================================================================== */
/*  End of file:  VLIB_triangulatePoints.h                                  */
/* ======================================================================== */

