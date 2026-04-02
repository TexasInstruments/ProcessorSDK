/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2007-2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/
#ifndef VLIB_TRIANGULATEPOINT_TYPES_H_
#define VLIB_TRIANGULATEPOINT_TYPES_H_ 1

#define   VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_SIZE (12)
#define  VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_SIZE  (VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_SIZE)

#define  VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_SIZE (9)
#define VXLIB_TRIANG_CAMERA_INTRINSIC_PARAM_SIZE (VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_SIZE)
/*
* To make double word alignment and also to take account of last row
* which is 0 0 0 1
*/
#define   VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE (VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_SIZE + 8)
#define  VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE (VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE)
/* To make double word aligned.
*/
#define  VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_ASIZE (VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_SIZE + 1)
#define VXLIB_TRIANG_CAMERA_INTRINSIC_PARAM_ASIZE (VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_ASIZE)
/*
* Maximum number track length which is considered for triangulation
*/
#define VLIB_TRIANG_MAX_POINTS_IN_TRACK (MAX_NUM_FRAMES_TO_TRACK)

#define VXLIB_TRIANG_MAX_POINTS_IN_TRACK (VLIB_TRIANG_MAX_POINTS_IN_TRACK)
/*
* Maximum number of tracks worked together, called as vector. This is controlled SIMD length of processor for floating point data
*/
#define VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR (SIMD_WIDTH)

#define VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR (VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR)

/*
* Total number of unique element in matrix At*A used in triangulation
*/
#define VLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA (6)
#define VXLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA (VLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA)
/* Number of rows in triangulation matrix
*/
#define VLIB_TRIANG_MAT_ROW (3)
#define VXLIB_TRIANG_MAT_ROW (VLIB_TRIANG_MAT_ROW)
/* Number of columns in triangulation matrix
*/
#define VLIB_TRIANG_MAT_COL (VLIB_TRIANG_MAX_POINTS_IN_TRACK * 2)
#define VXLIB_TRIANG_MAT_COL (VLIB_TRIANG_MAT_COL)

/* Aligned number of rows for triangulation matrix, to have data double word aligned.
*/
#define VLIB_TRIANG_MAT_AROW ((VLIB_TRIANG_MAT_ROW + 1) & 0xFFFFFFFE)
#define VXLIB_TRIANG_MAT_AROW (VLIB_TRIANG_MAT_AROW)

#endif
/* ======================================================================== */
/*  End of file:  VLIB_triangulatePoints_types.h                            */
/* ======================================================================== */
