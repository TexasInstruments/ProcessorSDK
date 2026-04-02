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
 *  \file tidl_spatialMaxPool_ixX_oxX.c
 *
 *  \brief This file contains the implementation of execute function for spatial
 *         max pool
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "tidsp/tidl_custom_maxpool_ixX_oxX.h"
#include "tidsp/tidl_custom_maxpool_ixX_oxX_priv.h"


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

static int32_t TIDL_customMaxPool_ixX_oxX_init_checkParams(void *kernelHandle,
                                                            const TIDL_bufParams3D_t *srcAddr,
                                                            const TIDL_bufParams3D_t *dstAddr,
                                                            const TIDL_CustomMaxPoolIxXOxXInitArgs *pKerInitArgs);


/* ========================================================================== */
/*                          Device Based Functions                            */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/*                           Static Device functions:                         */
/* Overloaded instances of functions need to be defined first                 */
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/*                           Driving functions:                               */
/*----------------------------------------------------------------------------*/



/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t TIDL_customMaxPool_ixX_oxX_getHandleSize(void * kernelInitParams)
{
  int32_t handleSize = (int32_t)(sizeof(TIDL_CustomMaxPoolIxXOxXPrivArgs));
  return handleSize;
}

int32_t TIDL_customMaxPool_ixX_oxX_init(void *kernelHandle,
                                                const TIDL_bufParams3D_t *srcAddr,
                                                const TIDL_bufParams3D_t *dstAddr,
                                                void *pKerInitArgs)
{
  int32_t status = CUSTOM_SUCCESS;
  TIDL_CustomMaxPoolIxXOxXPrivArgs *pKerPrivArgs = (TIDL_CustomMaxPoolIxXOxXPrivArgs *)kernelHandle;
  TIDL_CustomMaxPoolIxXOxXInitArgs * pInitArgs = (TIDL_CustomMaxPoolIxXOxXInitArgs *)pKerInitArgs;

  pKerPrivArgs->initArgs = *pInitArgs;
  pKerPrivArgs->execute  = NULL;
  status = TIDL_customMaxPool_ixX_oxX_init_checkParams(kernelHandle, srcAddr, dstAddr, pInitArgs);

  if ( status == CUSTOM_SUCCESS )
  {
    if(pInitArgs->funcStyle == TIDL_CUSTOM_FUNCTION_OPTIMIZED_C7X)
    {
      pKerPrivArgs->execute = TIDL_customMaxPool_3x3_2x2_ixu_oxu_exec_c7x;
      status = TIDL_customMaxPool_ixX_oxX_buffParamsInit_c7x(pKerPrivArgs, srcAddr, dstAddr, &pKerPrivArgs->initArgs);

      status +=  TIDL_customMaxPool_ixX_oxX_init_c7x(pKerPrivArgs,
                                                      srcAddr,
                                                      dstAddr,
                                                      &pKerPrivArgs->initArgs);
    }
    else
    {
      status = CUSTOM_FAIL;
    }
  }

  return status;
}

int32_t TIDL_customMaxPool_ixX_oxX_exec(void *kernelHandle,
                                                  const TIDL_bufParams3D_t *srcAddr,
                                                  const TIDL_bufParams3D_t *dstAddr,
                                                  const void *srcPtr[],
                                                  void *dstPtr)
{
  int32_t status;
  TIDL_CustomMaxPoolIxXOxXPrivArgs *pKerPrivArgs = (TIDL_CustomMaxPoolIxXOxXPrivArgs *)(kernelHandle);

  /* Call execute API */
  status = pKerPrivArgs->execute(pKerPrivArgs,
                                 srcPtr[0],/*Max pooling layer has only one input */
                                 dstPtr);

  return status;
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static int32_t TIDL_customMaxPool_ixX_oxX_init_checkParams(void *kernelHandle,
                                                            const TIDL_bufParams3D_t *srcAddr,
                                                            const TIDL_bufParams3D_t *dstAddr,
                                                            const TIDL_CustomMaxPoolIxXOxXInitArgs *pKerInitArgs)
{
  int32_t status = CUSTOM_SUCCESS;
  if((NULL == kernelHandle) || (NULL == pKerInitArgs) || (NULL == srcAddr) || (NULL == dstAddr))
  {
    status = CUSTOM_FAIL;
  }
  return status;
}


