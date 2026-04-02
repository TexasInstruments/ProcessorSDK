/*
 * Module Name        :TIDL
 *
 * Module Description :TI Deep learning Library module is TI's CNN/DNN
 *                     acceleration on EVE and DSP
 *
 * Copyright (C) 2016-2019 Texas Instruments Incorporated - http://www.ti.com/
 *
 * ALL RIGHTS RESERVED
 *
 *
 */

/**
 *  \file tidl_custom_maxpool_ixX_oxX_priv.h
 *
 *  \brief This file contains the private include of custom spatial max pool layer
 */

#ifndef TIDL_CUSTOM_MAXPOOL_IXX_OXX_PRIV_H_
#define TIDL_CUSTOM_MAXPOOL_IXX_OXX_PRIV_H_

#include "tidsp/tidl_custom_maxpool_ixX_oxX.h"
#include "tidsp/tidl_custom_maxpool_ixX_oxX_priv.h"
#include "c7x.h"

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SE_PARAM_WORDS ((uint32_t)SE_PARAM_SIZE/sizeof(uint32_t))
#define TIDL_NUM_BLOCKS(width, stride) (((width) / (stride)) + ((((width) - (((width) / (stride)) * (stride))) > 0U) ? 1U : 0U))

#define TIDL_SIMD_WIDTH_SHIFT        (6U) /* 2^6 = 648b elements in a 512b vector */
#define TIDL_SIMD_WIDTH              (1U << TIDL_SIMD_WIDTH_SHIFT)
#define TIDL_SIMD_WIDTH_RND          (TIDL_SIMD_WIDTH >> 1U)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */
typedef int32_t (*TIDL_customMaxPoolExecFunc)(void * handle,
                                                const void *src,
                                                void *dst);

typedef struct
{
  uint32_t seTemplate1[SE_PARAM_WORDS];
  uint32_t seTemplate2[SE_PARAM_WORDS];
  uint32_t saTemplate[SE_PARAM_WORDS];
  uint32_t numCh;
  uint32_t numLines;
  uint32_t numTiles;
  uint32_t inStride;
} TIDL_CustomMaxPoolBuffParamsC7x;

typedef struct
{
  TIDL_bufParams3D_t srcBuf3D;
  TIDL_bufParams3D_t dstBuf3D;
} TIDL_CustomMaxPoolBuffParamsNatC;

typedef struct
{
  TIDL_CustomMaxPoolBuffParamsC7x  c7x;
  TIDL_CustomMaxPoolBuffParamsNatC natc;
} TIDL_CustomMaxPoolIxXOxXBuffParams;

typedef struct
{
  TIDL_CustomMaxPoolIxXOxXInitArgs initArgs;
  TIDL_customMaxPoolExecFunc execute;
  TIDL_CustomMaxPoolIxXOxXBuffParams bufParams;
  int32_t kernelHandleSize;
} TIDL_CustomMaxPoolIxXOxXPrivArgs;

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t TIDL_customMaxPool_ixX_oxX_buffParamsInit_c7x(TIDL_CustomMaxPoolIxXOxXPrivArgs *pKerPrivArgs,
                                                       const TIDL_bufParams3D_t *srcAddr,
                                                       const TIDL_bufParams3D_t *dstAddr,
                                                       const TIDL_CustomMaxPoolIxXOxXInitArgs *kerInitArgs);

int32_t TIDL_customMaxPool_ixX_oxX_init_c7x(const TIDL_CustomMaxPoolIxXOxXPrivArgs *pKerPrivArgs,
                                             const TIDL_bufParams3D_t *srcAddr,
                                             const TIDL_bufParams3D_t *dstAddr,
                                             const TIDL_CustomMaxPoolIxXOxXInitArgs *kerInitArgs);

/**< Natural C Function */
int32_t TIDL_customMaxPool_ixX_oxX_exec_cn(void * handle,
                                                      const void *src,
                                                      void *dst);

int32_t TIDL_customMaxPool_3x3_2x2_ixu_oxu_exec_c7x(void * handle,
                                                    const void *src,
                                                    void *dst);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#endif /* TIDL_CUSTOM_MAXPOOL_IXX_OXX_PRIV_H_ */

