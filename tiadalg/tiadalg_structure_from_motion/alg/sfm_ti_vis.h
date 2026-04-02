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
*  @file         sfm_ti_vis.h
*  @author       Deepak Poddar
*  @brief        This file defines the functions related to visualization of SFM outout
*  @version 0.1 (Feb 2021) : Base version.
***************************************************************************
*/

#ifndef TI_SFM_DRAW_VIS_H
#define TI_SFM_DRAW_VIS_H

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#pragma RESET_MISRA ("required")
#include "sfm_ti_alg_int.h"

//#define TIADALG_VIS_PROFILE
#ifndef __mmin
#define __mmin(x,y) ((x>y)?y:x)
#endif
void SFM_TI_drawSparse3DPoints(uint8_t* restrict imgPtr2, SFM_TI_output* restrict ptCld, int32_t imgWidth,
                               uint8_t* colorIdx, uint16_t* curBlkIndx,int32_t blockOffset,
                               int32_t curBlkNumPoints);

int32_t SFM_TI_segregate3DPoints(SFM_TI_output* restrict ptCld, int32_t imgWidth,int32_t imgHeight,
                               float dMin, float dMax, int32_t numRowInBlk,
                               uint8_t* colorIndx, uint16_t* blkIndx,
                               int32_t outNumPoints);

void SFM_TI_visPtCldOutput(SFM_TI_Handle intAlgHandle,
                      SFM_TI_CreateParams* createParams,
                      sSfm_TI_ExtMem* sfmPrm,
                      uint8_t* inImg, void * pointCld, uint8_t* outImg);

void SFM_TI_visOgOutput(SFM_TI_Handle intAlgHandle,
                        SFM_TI_CreateParams* createParams,
                        sSfm_TI_ExtMem* sfmPrm,
                        uint8_t* outImg); /*Output image location in DDR*/

void SFM_TI_visEgoVehicle(SFM_TI_Handle intAlgHandle,
                        SFM_TI_CreateParams* createParams,
                        sSfm_TI_ExtMem* sfmPrm,
                        int32_t isLocal,
                        uint8_t* outImg);

#endif
