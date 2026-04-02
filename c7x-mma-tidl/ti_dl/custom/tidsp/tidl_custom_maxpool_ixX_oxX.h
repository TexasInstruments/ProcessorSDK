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
 *  \file tidl_custom_maxpool_ixX_oxX.h
 *
 *  \brief This file contains the function prototypes of custom spatial max pool layer
 */

#ifndef TIDL_CUSTOM_MAXPOOL_IXX_OXX_H_
#define TIDL_CUSTOM_MAXPOOL_IXX_OXX_H_

#include "tidl_dataflow.h"
#include "tidl_custom.h"
#include "c7x.h"


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define CUSTOM_SUCCESS        (0)
#define CUSTOM_FAIL           (-1)

#define TIDL_CUSTOM_FUNCTION_NATC (0)

#define TIDL_CUSTOM_FUNCTION_OPTIMIZED_C7X (1)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
  int32_t  funcStyle;
  TIDL_CustomParams0_t customMaxPoolParam;
} TIDL_CustomMaxPoolIxXOxXInitArgs;

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */
/*  Note: This is an internal/private structure and should not be
 *  used or modified by caller.*/



/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


int32_t TIDL_customMaxPool_ixX_oxX_getHandleSize(void * kernelInitParams);

int32_t TIDL_customMaxPool_ixX_oxX_init(void *kernelHandle,
                                                const TIDL_bufParams3D_t *srcAddr,
                                                const TIDL_bufParams3D_t *dstAddr,
                                                void *pKerInitArgs);

int32_t TIDL_customMaxPool_ixX_oxX_exec(void *kernelHandle,
                                                  const TIDL_bufParams3D_t *srcAddr,
                                                  const TIDL_bufParams3D_t *dstAddr,
                                                  const void *srcPtr[],
                                                  void *dstPtr);



/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#endif /* TIDL_CUSTOM_MAXPOOL_IXX_OXX_H_ */

