/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2009-2021 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/

/**
*  @file        sfm_ti_occpGrid.h
*  @author      Deepak Poddar
*  @brief       This file declares the functions related to occupancy grid calcualtion
*  @version     0.0 (Jan 2014) : Base version.
*/

#ifndef SFM_TI_OCCP_GRID_H_
#define SFM_TI_OCCP_GRID_H_

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#pragma RESET_MISRA ("required")
#include "sfm_ti_alg_int.h"

void SFM_TI_genOccpGrid(float* wp, float* cp, uint8_t* valid, int32_t ocpGrdSizeInPixels, int8_t* ogBufInt);

void SFM_TI_genOccpGrid_ci(float* restrict wp, float* restrict cp, uint8_t* restrict valid, int32_t ocpGrdSizeInPixels,
                           int8_t* restrict ogBufInt, uint32_t* restrict scratchBuf1, int8_t* restrict scratchBuf2, void*  pBlock);

#endif

