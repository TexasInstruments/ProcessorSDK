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
 *  \file tidl_custom_maxpool_ixX_oxX_c7x.c
 *
 *  \brief This file contains the optimized C7x implementation for spatial max
 *         pool
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "tidsp/tidl_custom_maxpool_ixX_oxX.h"
#include "tidsp/tidl_custom_maxpool_ixX_oxX_priv.h"
#include "../algo/inc/tidl_scalability_support.h"
using namespace c7x;
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define TIDL_CUSTOM_MAXPOOLING_SIMD_WIDTH_OUT                      (TIDL_SIMD_WIDTH/2)

#ifdef HOST_EMULATION
#include <assert.h>
#define restrict
#define _nassert assert
#endif

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


/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static inline int32_t TIDL_getDataflowTypeSize(uint32_t type)
{

    int32_t size = 1;

    if((type == TIDL_INT8)  ||
       (type == TIDL_UINT8))
    {
        size = 1;
    }
    else if((type == TIDL_INT16)  ||
            (type == TIDL_UINT16) ||
            (type == TIDL_FLOAT16))
    {
        size = 2;
    }
    else if(type == TIDL_UINT24)
    {
        size = 3;
    }
    else if((type == TIDL_INT32)  ||
            (type == TIDL_UINT32) ||
            (type == TIDL_FLOAT32))
    {
        size = 4;
    }
    else if((type == TIDL_INT64)  ||
            (type == TIDL_UINT64) ||
            (type == TIDL_FLOAT64))
    {
        size = 8;
    }
    else {
        size = 1;
    }

    return size;
}

/*----------------------------------------------------------------------------*/
/*                       Device Specific Definitions                          */
/*----------------------------------------------------------------------------*/
static inline void tidl_init_SE_Template(__SE_TEMPLATE_v1 *seTemplate)
{
    (*seTemplate) = __gen_SE_TEMPLATE_v1();
}




/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t TIDL_customMaxPool_ixX_oxX_buffParamsInit_c7x(TIDL_CustomMaxPoolIxXOxXPrivArgs *pKerPrivArgs,
                                                       const TIDL_bufParams3D_t *srcAddr,
                                                       const TIDL_bufParams3D_t *dstAddr,
                                                       const TIDL_CustomMaxPoolIxXOxXInitArgs *kerInitArgs)
{
  int32_t status = CUSTOM_SUCCESS;
  int32_t numBytes;
  uint32_t numLines = 0U, numTiles = 0U;
  uint32_t numCh;
  uint32_t simdWidth = TIDL_SIMD_WIDTH;
  uint32_t simdWidthOut = TIDL_CUSTOM_MAXPOOLING_SIMD_WIDTH_OUT;

  numBytes = TIDL_getDataflowTypeSize(srcAddr->data_type);

  if(numBytes == 1)
  {
    simdWidth= TIDL_SIMD_WIDTH;
    simdWidthOut= TIDL_CUSTOM_MAXPOOLING_SIMD_WIDTH_OUT;
  }
  else
  {
    status = CUSTOM_FAIL;
  }


  if((kerInitArgs->customMaxPoolParam.kernelH == 3) && (kerInitArgs->customMaxPoolParam.kernelW == 3))
  {
    simdWidthOut = simdWidthOut - 1;

    /*unsigned 3x3 kerenel with stride 1, input data comsumed is simdWidth-2 in each iteration of loop
      hence number of tiles will be different.
      64/32 elements read produces 62/30 outputs
    */
    simdWidth = simdWidth - 2;
  }

  /* Set parameters that are accessed by the kernel */
  if(CUSTOM_SUCCESS == status)
  {
    if((kerInitArgs->customMaxPoolParam.strideH == 2) && (kerInitArgs->customMaxPoolParam.strideW == 2))
    {
      numCh    = dstAddr->dim_z;
      numTiles = TIDL_NUM_BLOCKS(dstAddr->dim_x, simdWidthOut);
      numLines = dstAddr->dim_y;
    }
    else
    {
      status = CUSTOM_FAIL;
    }
  }

  if(CUSTOM_SUCCESS == status)
  {
    numBytes = TIDL_getDataflowTypeSize(srcAddr->data_type);
    int32_t temp = srcAddr->stride_y/numBytes;
    pKerPrivArgs->bufParams.c7x.inStride = (uint32_t)temp;
    pKerPrivArgs->bufParams.c7x.numTiles = numTiles;
    pKerPrivArgs->bufParams.c7x.numLines = numLines;
    pKerPrivArgs->bufParams.c7x.numCh = numCh;
  }

  return status;
}

int32_t TIDL_customMaxPool_ixX_oxX_init_c7x(const TIDL_CustomMaxPoolIxXOxXPrivArgs *pKerPrivArgs,
                                             const TIDL_bufParams3D_t *srcAddr,
                                             const TIDL_bufParams3D_t *dstAddr,
                                             const TIDL_CustomMaxPoolIxXOxXInitArgs *kerInitArgs)
{
  int32_t status = CUSTOM_SUCCESS;

  __SE_TEMPLATE_v1 seTemplate1;
  __SE_TEMPLATE_v1 seTemplate2;
  __SA_TEMPLATE_v1 saTemplate;
  /*Initialise SE Template to default values */
  tidl_init_SE_Template(&seTemplate1);
  tidl_init_SE_Template(&seTemplate2);
  saTemplate = __gen_SA_TEMPLATE_v1();
  seTemplate1.VECLEN = SE_VECLEN_NELEM(VEC_NELEM(uchar));
  seTemplate2.VECLEN = SE_VECLEN_NELEM(VEC_NELEM(uchar));
  saTemplate.VECLEN = SA_VECLEN_NELEM(VEC_NELEM(uchar));

  int32_t stride_y, stride_z;
  uint32_t  numBytes;
  uint32_t simdWidth, simdWidthOut;

  __SE_ELETYPE elemType;
  __SE_VECLEN veclen;
  numBytes = TIDL_getDataflowTypeSize(srcAddr->data_type);
  stride_y = srcAddr->stride_y / numBytes;
  stride_z = srcAddr->stride_z / numBytes;


  if(numBytes == 1U)
  {
    elemType     = __SE_ELETYPE_8BIT;
    veclen       = SE_VECLEN_NELEM(VEC_NELEM(uchar));
    simdWidth    = TIDL_SIMD_WIDTH;
    simdWidthOut = TIDL_CUSTOM_MAXPOOLING_SIMD_WIDTH_OUT;
  }
  else
  {
    status = CUSTOM_FAIL;
  }

  /* Set the SE parameters */
  if(CUSTOM_SUCCESS == status)
  {
    if((kerInitArgs->customMaxPoolParam.strideW == 2) && (kerInitArgs->customMaxPoolParam.strideH == 2))
    {
      if((kerInitArgs->customMaxPoolParam.kernelW == 3) && (kerInitArgs->customMaxPoolParam.kernelH == 3))
      {
        /*unsigned Flow*/
        /* Set SE flags */
        //3x3_s2
        seTemplate1.DIMFMT =  __SE_DIMFMT_5D;
        seTemplate1.ELETYPE = elemType;
        seTemplate1.VECLEN  = veclen;
        //seTemplate1.DECDIM1 = __SE_DECDIM_DIM2;

        /* Set SE params */
        seTemplate1.ICNT0 = simdWidth;
        seTemplate1.ICNT1 = 3;
        seTemplate1.ICNT2 = TIDL_NUM_BLOCKS(srcAddr->dim_x, (simdWidth-2U));
        seTemplate1.ICNT3 = ((dstAddr->dim_y + 1) >> 1);
        seTemplate1.ICNT4 = srcAddr->dim_z;
        seTemplate1.DIM1  = stride_y;
        seTemplate1.DIM2  = (int32_t)simdWidth - 2;
        seTemplate1.DIM3  = stride_y << (int32_t)2;
        seTemplate1.DIM4  = stride_z;

        seTemplate2  = seTemplate1;

      }
      else
      {
        status = CUSTOM_FAIL;
      }
    }
    else
    {
      status = CUSTOM_FAIL;
    }
  }

  if(CUSTOM_SUCCESS == status)
  {
    /* Store the SE templates */
    *(__SE_TEMPLATE_v1*)pKerPrivArgs->bufParams.c7x.seTemplate1 = seTemplate1;
    *(__SE_TEMPLATE_v1*)pKerPrivArgs->bufParams.c7x.seTemplate2 = seTemplate2;

  }

  if(CUSTOM_SUCCESS == status)
  {
    /* Set SA flags */
    numBytes = TIDL_getDataflowTypeSize(dstAddr->data_type);

    stride_y = dstAddr->stride_y / numBytes;
    stride_z = dstAddr->stride_z / numBytes;

    if((kerInitArgs->customMaxPoolParam.strideW == 2) && (kerInitArgs->customMaxPoolParam.strideH == 2))
    {
      if(numBytes == 1U)
      {
        saTemplate.VECLEN = SA_VECLEN_NELEM(VEC_NELEM(uchar)/2);
      }
      else
      {
        status = CUSTOM_FAIL;
      }


      if((kerInitArgs->customMaxPoolParam.kernelW == 3) && (kerInitArgs->customMaxPoolParam.kernelH == 3))
      {
        /* Calculate variables needed for SA params */
        /*unsigned flow*/
        /* Set SA params */
        saTemplate.DECDIM1   = __SA_DECDIM_DIM2;
        saTemplate.DECDIM2   = __SA_DECDIM_DIM3;
        saTemplate.DECDIM2SD = __SA_DECDIMSD_DIM1;

        saTemplate.DIMFMT = __SA_DIMFMT_5D;

        saTemplate.VECLEN = SA_VECLEN_NELEM(VEC_NELEM(uchar)/2);
        saTemplate.DECDIM1_WIDTH = dstAddr->dim_x*numBytes;
        saTemplate.DECDIM2_WIDTH = stride_y*dstAddr->dim_y*numBytes;

        saTemplate.ICNT0 = (simdWidthOut - 1U)*numBytes;
        saTemplate.ICNT1 = 2;
        saTemplate.ICNT2 = TIDL_NUM_BLOCKS(dstAddr->dim_x, (simdWidthOut-1U));
        saTemplate.ICNT3 = (dstAddr->dim_y + 1) >> 1;
        saTemplate.ICNT4 = dstAddr->dim_z;

        saTemplate.DIM1  = stride_y*numBytes;
        saTemplate.DIM2  = ((int32_t)simdWidthOut - 1)*numBytes;
        saTemplate.DIM3  = (stride_y << 1)*numBytes;
        saTemplate.DIM4  = stride_z*numBytes;
      }
      else
      {
        status = CUSTOM_FAIL;
      }

    }
    else
    {
      status = CUSTOM_FAIL;
    }
  }

  if(CUSTOM_SUCCESS == status)
  {
    /* Store the SA template */
    *(__SA_TEMPLATE_v1*)pKerPrivArgs->bufParams.c7x.saTemplate = saTemplate;
  }
  return status;
}

int32_t TIDL_customMaxPool_3x3_2x2_ixu_oxu_exec_c7x(void * handle,
                                                    const void *src,
                                                    void *dst)
{
  int32_t status = CUSTOM_SUCCESS;

  __SE_TEMPLATE_v1 seTemplate1, seTemplate2;
  __SA_TEMPLATE_v1 saTemplate;
  TIDL_CustomMaxPoolIxXOxXPrivArgs *pKerPrivArgs = (TIDL_CustomMaxPoolIxXOxXPrivArgs*)handle;

  /* Define local variables */
  int32_t tiles;
  int32_t numCh, numLines, numTiles, inStride, numItr;
  const int8_t *restrict pIn0;
  const int8_t *restrict pIn1;
  int8_t *restrict pOut1;

  /* Update the state variables */
  /* Retrieve loop control parameters from init time */
  inStride = pKerPrivArgs->bufParams.c7x.inStride;
  numLines = pKerPrivArgs->bufParams.c7x.numLines;
  numTiles = pKerPrivArgs->bufParams.c7x.numTiles;
  numCh    = pKerPrivArgs->bufParams.c7x.numCh;


  /* Retrieve SE and SA templates */
  seTemplate1 = *(__SE_TEMPLATE_v1*)pKerPrivArgs->bufParams.c7x.seTemplate1;
  seTemplate2 = *(__SE_TEMPLATE_v1*)pKerPrivArgs->bufParams.c7x.seTemplate2;

  saTemplate  = *(__SA_TEMPLATE_v1*)pKerPrivArgs->bufParams.c7x.saTemplate;

  /* Assign input/output buffers and open SE/SA */
  pIn0 = (const int8_t *)src;
  pIn1 = (const int8_t *)src + inStride*2*sizeof(char);

  __SE0_OPEN((void *)pIn0, seTemplate1);
  __SE1_OPEN((void *)pIn1, seTemplate2);

  pOut1 = (int8_t *)dst;

  __SA0_OPEN(saTemplate);

  uchar_vec vIn0, vIn1, vIn2;
  uchar_vec vIn3, vIn4, vIn5;
  uchar_vec vA, vB, vC, vD;
  uchar_vec vE;
  uchar_vec vMax0, vMax1;

  __vpred vpStore;

  /* numTiles is for input,  numLines is for output. SInce two output lines are processed in single
  iteration, hence (numLines + 1) >> 1. Then making it multiple of 16.
  numTiles --> total number of input tiles
  numLines --> total number of output lines
  */
  numItr = ((((numLines + 1) >> 1)*numTiles*numCh) >> 4) << 4;

#pragma MUST_ITERATE(1,,)
#pragma UNROLL(8)
  /*Currently ii of this loop is suppose to be 3, but it is coming as 4. Hence loop was unrolled 16 times
    but then ii was coming as 49, which was effectively 3 only. But later instead of 2 SA, only one SA is used
    then unrolling by 16 was not benificial hence currently unrolling by 8 only.
  */
  for(tiles = 0; tiles < numItr; tiles++)
  {
    // vIn0 - R[i     ][0:63].  In next iteration of loop , this moves horizontally by 62 elements. Once horizontal dimension is over then move to i+4 and so on.
    vIn0  = reinterpret<uchar_vec>((SEADV(0, uchar_vec)));
    vIn1  = reinterpret<uchar_vec>((SEADV(0, uchar_vec))); // R[i+1][0:63]
    vIn2  = reinterpret<uchar_vec>((SEADV(0, uchar_vec))); // R[i+2][0:63]
    vMax0 = __max(__max(vIn0, vIn1), vIn2); //max[i][0:63]. Vertical max of three rows are done


    vIn3  = reinterpret<uchar_vec>((SEADV(1, uchar_vec))); // R[i+2][0:63] In next iteration of loop , this also moves horizontally by 62 elements. Once horizontal dimension is over then move to i+6 and so on.
    vIn4  = reinterpret<uchar_vec>((SEADV(1, uchar_vec))); // R[i+3][0:63]
    vIn5  = reinterpret<uchar_vec>((SEADV(1, uchar_vec))); // R[i+4][0:63]
    vMax1 = __max(__max(vIn3, vIn4), vIn5); //max[i+2][0:63]. Vertical max of next three rows are done


    vA    = __pack_consec_low(vMax1, vMax0); // max[i][0], max[i][2], max[i][4], max[i][6] …  max[i][60], X, max[i+2][0],max[i+2][2], max[i+2][4], max[i+2][6], … max[i+2][60],X
    vB    = __pack_consec_high(vMax1, vMax0);// max[i][1], max[i][3], max[i][5], max[i][7] …  max[i][61], X, max[i+2][1],max[i+2][3], max[i+2][5], max[i+2][7], … max[i+2][61],X

    // vC -- max[i][2], max[i][4], max[i][6], max[i][8] …  max[i][62], X, max[i+2][2],max[i+2][4], max[i+2][6], max[i+2][8], … max[i+2][62],X
    vC    = reinterpret<uchar_vec>(__shift_right_full(as_long_vec(vA), (uchar)(8*sizeof(char))));
    vD    = __max(vA,__max(vB, vC));
    vE    = as_uchar_vec(vD);

    vpStore = __SA0_VPRED(uchar_hvec);// vpStore = 7fffffff. Moves by 31 elements in next iteration in horizontal direction. Once horizontal dim is over then vertical jump by 2
    __vstore_pred(vpStore, __SA0ADV(uchar_hvec, pOut1), vE.lo());


    vpStore = __SA0_VPRED(uchar_hvec); // vpStore1 = 7fffffff. Moves by 31 elements in next iteration in horizontal direction. Once horizontal dim is over then vertical jump by 2
    __vstore_pred(vpStore, __SA0ADV(uchar_hvec, pOut1), vE.hi());

  }

  /* executing remaining iteration in last chunk of 16 cyles. As previous loop was for multiple of 16*/

#pragma MUST_ITERATE(1,,)
  for(; tiles < (((numLines + 1) >> 1)*numTiles*numCh); tiles++)
  {
    vIn0  = reinterpret<uchar_vec>((SEADV(0, uchar_vec)));
    vIn1  = reinterpret<uchar_vec>((SEADV(0, uchar_vec)));
    vIn2  = reinterpret<uchar_vec>((SEADV(0, uchar_vec)));
    vMax0 = __max(__max(vIn0, vIn1), vIn2);

    vIn3  = reinterpret<uchar_vec>((SEADV(1, uchar_vec)));
    vIn4  = reinterpret<uchar_vec>((SEADV(1, uchar_vec)));
    vIn5  = reinterpret<uchar_vec>((SEADV(1, uchar_vec)));
    vMax1 = __max(__max(vIn3, vIn4), vIn5);


    vA    = __pack_consec_low(vMax1, vMax0);
    vB    = __pack_consec_high(vMax1, vMax0);

    vC    = reinterpret<uchar_vec>(__shift_right_full(as_long_vec(vA), (uchar)(8*sizeof(char))));
    vD    = __max(vA,__max(vB, vC));
    vE    = as_uchar_vec(vD);


    vpStore = __SA0_VPRED(uchar_hvec);
    __vstore_pred(vpStore, __SA0ADV(uchar_hvec, pOut1), vE.lo());

    vpStore = __SA0_VPRED(uchar_hvec);
    __vstore_pred(vpStore, __SA0ADV(uchar_hvec, pOut1), vE.hi());

  }

  __SE0_CLOSE();
  __SE1_CLOSE();
  __SA0_CLOSE();

  return status;
}




