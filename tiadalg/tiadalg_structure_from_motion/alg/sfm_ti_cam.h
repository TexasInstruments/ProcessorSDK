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
*  @file         sfm_ti_cam.h
*  @author       Deepak Poddar
*  @brief        This file defines the functions related camera parameters
*  @version 0.1 (Oct 2014) : Base version.
***************************************************************************
*/

#ifndef TI_SFM_CAM_H
#define TI_SFM_CAM_H


/** Copy the input camera extrinsic parameters into persistent memory area
* in circular fashion.
*/
void SFM_TI_copyCamPrmToCirCBuf(float*    inCamExtPrm,
                                uint16_t  curBufIndx,
                                float*    camExtPrm);
/** Get the normalized camera extrinsic parameters, and inverse also of
* that of the current camera parameters came in current process call
*/
void SFM_TI_getInvExtPrmMatrix(float*   camExtPrm,
                                   uint16_t curBufIdx,
                                   float*   camInvExtPrm,
                                   uint8_t* scratch);
/** Multiply current input camera extrinsic parameter with camera intrinsic
* parameters and store in persistent circular memory
*/
void SFM_TI_mulIntExtPrm(float* camIntPrm,
                         float* camExtPrm,
                         float* normCamIntExtPrm);
/** Copy all the necessary camera parameters into internal memory before
triangulation loop starts
*/
void SFM_TI_copyCamPrmsToIntMem(sSfm_TI_L1DMem* sfmL1Prm,
                                sSfm_TI_ExtMem* sfmExtPrm);

/** Calculates Fundamental matrix from camera parameters
*/
void SFM_TI_computeFmatFrmRT(float* curCamExtPrm,
                             float* prevCamInvExtPrm,
                             float* camInvIntPrm,
                             float* camIntPrm,
                             float* Fmat);
#endif
