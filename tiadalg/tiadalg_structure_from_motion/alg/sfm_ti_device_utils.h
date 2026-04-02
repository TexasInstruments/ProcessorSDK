/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2009-2020 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/

/**
***************************************************************************
*  @file         sfm_ti_3dPruning.h
*  @author       Deepak Poddar
*  @brief        This file defines the functions for 3D points pruning
*  @version 0.1 (Jan 2021) : Base version.
***************************************************************************
*/

#ifndef TI_SFM_DEVICE_UTILS_H
#define TI_SFM_DEVICE_UTILS_H

#include "ti/drv/udma/dmautils/dmautils.h"
#include "ti/drv/udma/udma.h"

int32_t TIADALG_initDmaUtils(void*  intAlgHandle);
void  TIADALG_prePareDefault3DTr(void * trMem, int32_t channelNo);


void  TIADALG_memcpy2D
(
  const void   *dstPtr,
  const void   *srcPtr,
  uint16_t     width,
  uint16_t     height,
  int32_t      dstStride,
  int32_t      srcStride,
  int32_t      chNo,
  void * dmaUtilsContext,
  uint8_t * trMem,
  int32_t immediateWait
  );

int32_t TIADALG_updatePingPong3DTr(
  uint8_t* src, uint8_t* dst, uint8_t* inTrMem,
  int32_t width, int32_t height, int32_t inPitch,
  int32_t blockWidth, int32_t blockHeight, int32_t blockPitch,
  int32_t pongOffset,int32_t isSrcPingPong, int32_t elmSize,
  int32_t chNo, void* dmaUtilsContext);

#endif

