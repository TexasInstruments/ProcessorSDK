/*
 *
 * Copyright (c) {2015 - 2020} Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 *        No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 *        any redistribution and use are licensed by TI for use only with TI Devices.
 *
 *        Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 *        any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 *        any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file tidl_custom.c
 *
 *  \brief File containing the TIDL custom layer APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "stdio.h"
#include "stdlib.h"
#include <limits>

#include "itidl_ti.h"
#include "tidl_dataflow.h"

#include "tidl_custom.h"
#include "tidl_custom_maxpooling.h"
#include "tidsp/tidl_custom_maxpool_ixX_oxX.h"
#include "../algo/inc/tidl_commonUtils.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

template<class Tin> void TIDL_refCustomMaxPoolingCore(Tin *pInChannel,
                                                      int32_t width,
                                                      int32_t height,
                                                      int32_t inPitch,
                                                      int32_t inChPitch,
                                                      int32_t outPitch,
                                                      int32_t outChPitch,
                                                      int32_t numOutChannels,
                                                      int32_t kernelW,
                                                      int32_t kernelH,
                                                      int32_t strideW,
                                                      int32_t strideH,
                                                      int32_t padW,
                                                      int32_t padH,
                                                      Tin initValue,
                                                      Tin *pOutChannel,
                                                      sTIDL_Layer_t *tidlLayer);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t TIDL_refCustomMaxPoolingProcess(sTIDL_Layer_t *tidlLayer,
                                        TIDL_CustomParams0_t *customMaxPoolingParams,
                                        void *inPtrs[],
                                        void *outPtrs[],
                                        float32_tidl *currMin,
                                        float32_tidl *currMax)
{
  int32_t status = CUSTOM_SUCCESS;
  uint8_t (*inPtr)[];
  uint8_t (*outPtr)[];
  sTIDL_DataParams_t *inDataParams = tidlLayer->inDataPtr;
  sTIDL_DataParams_t *outDataParams = &tidlLayer->outData;

  int32_t width = inDataParams->dimValues[TIDL_DIM_WIDTH];
  int32_t height = inDataParams->dimValues[TIDL_DIM_HEIGHT];
  int32_t inPitch = inDataParams->pitch[TIDL_LINE_PITCH];
  int32_t inChPitch = inDataParams->pitch[TIDL_CHANNEL_PITCH];
  int32_t outPitch = outDataParams->pitch[TIDL_LINE_PITCH];
  int32_t outChPitch = outDataParams->pitch[TIDL_CHANNEL_PITCH];
  int32_t numOutChannels = outDataParams->dimValues[TIDL_DIM_NUMCH];
  int32_t kernelW = customMaxPoolingParams->kernelW;
  int32_t kernelH = customMaxPoolingParams->kernelH;
  int32_t strideW = customMaxPoolingParams->strideW;
  int32_t strideH = customMaxPoolingParams->strideH;
  int32_t padW = customMaxPoolingParams->padW;
  int32_t padH = customMaxPoolingParams->padH;
  int32_t elementType = inDataParams->elementType;
  int32_t numTotRoi = inDataParams->dimValues[TIDL_DIM_BATCH];
  int32_t i;

  inPtr = (uint8_t (*)[])(inPtrs[0]);
  outPtr = (uint8_t (*)[])(outPtrs[0]);

  for (i = 0; i < numTotRoi; i++)
  {
    if (elementType == TIDL_UnsignedChar)
    {
      TIDL_refCustomMaxPoolingCore((uint8_t *)inPtr,
                                   width,
                                   height,
                                   inPitch,
                                   inChPitch,
                                   outPitch,
                                   outChPitch,
                                   numOutChannels,
                                   kernelW,
                                   kernelH,
                                   strideW,
                                   strideH,
                                   padW,
                                   padH,
                                   (uint8_t)0,
                                   (uint8_t *)outPtr,
                                   tidlLayer);
    }
    else if (elementType == TIDL_SignedChar)
    {
      TIDL_refCustomMaxPoolingCore((int8_t *)inPtr,
                                   width,
                                   height,
                                   inPitch,
                                   inChPitch,
                                   outPitch,
                                   outChPitch,
                                   numOutChannels,
                                   kernelW,
                                   kernelH,
                                   strideW,
                                   strideH,
                                   padW,
                                   padH,
                                   (int8_t)0x80,
                                   (int8_t *)outPtr,
                                   tidlLayer);
    }
    else if (elementType == TIDL_UnsignedShort)
    {
      TIDL_refCustomMaxPoolingCore((uint16_t *)inPtr,
                                   width,
                                   height,
                                   inPitch,
                                   inChPitch,
                                   outPitch,
                                   outChPitch,
                                   numOutChannels,
                                   kernelW,
                                   kernelH,
                                   strideW,
                                   strideH,
                                   padW,
                                   padH,
                                   (uint16_t)0,
                                   (uint16_t *)outPtr,
                                   tidlLayer);
    }
    else if (elementType == TIDL_SignedShort)
    {
      TIDL_refCustomMaxPoolingCore((int16_t *)inPtr,
                                   width,
                                   height,
                                   inPitch,
                                   inChPitch,
                                   outPitch,
                                   outChPitch,
                                   numOutChannels,
                                   kernelW,
                                   kernelH,
                                   strideW,
                                   strideH,
                                   padW,
                                   padH,
                                   (int16_t)0x8000,
                                   (int16_t *)outPtr,
                                   tidlLayer);
    }
    else if (elementType == TIDL_SinglePrecFloat)
    {
      TIDL_refCustomMaxPoolingCore((float32_tidl *)inPtr,
                                   width,
                                   height,
                                   inPitch,
                                   inChPitch,
                                   outPitch,
                                   outChPitch,
                                   numOutChannels,
                                   kernelW,
                                   kernelH,
                                   strideW,
                                   strideH,
                                   padW,
                                   padH,
                                   (float32_tidl)std::numeric_limits<float32_tidl>::lowest(),
                                   (float32_tidl *)outPtr,
                                   tidlLayer);
    }
    else
    {
      status = CUSTOM_FAIL;
      break;
    }
  }

  return status;
}

/* Optimized implementation of custom pooling layer */
int32_t TIDL_customMaxPoolDspProcess(void *tidlHandle,
                                     sTIDL_Layer_t *tidlLayer,
                                     TIDL_CustomParams0_t *customMaxPoolingParams,
                                     void *inPtrs[],
                                     void *outPtrs[])
{
  int32_t status = CUSTOM_SUCCESS;
  TIDL_DataflowInitParams initParams;
  TIDL_CustomMaxPoolIxXOxXInitArgs kernelInitArgs;

  /* User can use the DMA dataflow provided by TIDL by using
  the API's defined in ti_dl/inc/tidl_dataflow.h file. An example
  for the same is given below */

  kernelInitArgs.funcStyle = TIDL_CUSTOM_FUNCTION_OPTIMIZED_C7X;
  kernelInitArgs.customMaxPoolParam = *customMaxPoolingParams;

  /* Indicates that processing of the layer is going to be done for
  all the input tensors together */
  initParams.dataFlowType = TIDL_DataFlowTypeOneTensorInProcOneChannel;
  /* Function pointer to determine the memory needed by the kernel handle*/
  initParams.getHandleSize = TIDL_customMaxPool_ixX_oxX_getHandleSize;
  /* Function pointer to the init of the kernel, user is expected to use
  srcAddr ands dstAddr variable which are part argument list to this function
  pointer to write their kernel */
  initParams.initFuncPtr = TIDL_customMaxPool_ixX_oxX_init;
  /* Function pointer to the execute function of the kernel, user should try
  to avoid any computation which is only required once within this. If required
  same can be computed inside the init function and stored inside the kernel  handle
  which is also available to the execute call of the kernel */
  initParams.execFuncPtr = TIDL_customMaxPool_ixX_oxX_exec;
  /* Any arguments needed by the kernel, this will be passed as it is to the
  kernel init function call */
  initParams.kernelInitArgs = &kernelInitArgs;

  status = TIDL_DataflowInit(tidlHandle,
                             &initParams);

  /* Init does CPU write to some cacheable DDR memory records, and so need to do cache writeback after Init for devices with cache coherency limitations */
  TIDL_L1DandL2CacheWbInv();

  if (status != 0)
  {
    status = CUSTOM_FAIL;
  }

  if (status == CUSTOM_SUCCESS)
  {
    status = TIDL_DataflowProcess(tidlHandle,
                                  inPtrs,
                                  outPtrs);
  }

  if (status != CUSTOM_SUCCESS)
  {
    status = CUSTOM_FAIL;
  }

  return status;
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

template<class Tin> void TIDL_refCustomMaxPoolingCore(Tin *pInChannel,
                                                      int32_t width,
                                                      int32_t height,
                                                      int32_t inPitch,
                                                      int32_t inChPitch,
                                                      int32_t outPitch,
                                                      int32_t outChPitch,
                                                      int32_t numOutChannels,
                                                      int32_t kernelW,
                                                      int32_t kernelH,
                                                      int32_t strideW,
                                                      int32_t strideH,
                                                      int32_t padW,
                                                      int32_t padH,
                                                      Tin initValue,
                                                      Tin *pOutChannel,
                                                      sTIDL_Layer_t *tidlLayer)
{
  Tin inRowCol;
  int32_t i1, i2, i3, i4, i5;
  Tin maxValue;
  int32_t isBorderPixel;
  int32_t validPosXMin, validPosXMax, validPosYMin, validPosYMax;
  int32_t spatialOffsetY, spatialOffsetX;
  // Setting the parameters marking for pixels on borders
  validPosXMin = padW;
  validPosXMax = padW + width;
  validPosYMin = padH;
  validPosYMax = padH + height;

  Tin *inData = (Tin *)pInChannel + ((tidlLayer->inDataPtr[0].padH - padH) * inPitch) + (tidlLayer->inDataPtr[0].padW - padW);
  Tin *outData = (Tin *)pOutChannel + ((tidlLayer->outData.padH) * tidlLayer->outData.pitch[TIDL_LINE_PITCH]) + (tidlLayer->outData.padW);

  for (i1 = 0; i1 < numOutChannels; i1++)
  {
    uint32_t numRows, numCols;

    numRows = tidlLayer->outData.dimValues[TIDL_DIM_HEIGHT];
    numCols = tidlLayer->outData.dimValues[TIDL_DIM_WIDTH];
    for (i2 = 0; i2 < numRows; i2++)
    {
      for (i3 = 0; i3 < numCols; i3++)
      {
        maxValue = initValue;
        for (i4 = 0; i4 < kernelH; i4++)
        {
          for (i5 = 0; i5 < kernelW; i5++)
          {
            spatialOffsetY = (i2 * strideH) + i4;
            spatialOffsetX = (i3 * strideW) + i5;
            // checking if the current pixels falls in borders
            isBorderPixel = TIDL_checkPixelInPadRegion(spatialOffsetY, spatialOffsetX, validPosXMin, validPosXMax, validPosYMin, validPosYMax);
            /*Over write the boarder pixel to not influence the operation */
            if (isBorderPixel)
            {
              inRowCol = initValue;
            }
            else
            {
              inRowCol = inData[(i1 * inChPitch) +
                                (((i2 * strideH) + i4) * inPitch) + (i3 * strideW) + i5];
            }
            /* Maxpooling operations */
            maxValue = (maxValue > inRowCol) ? maxValue : inRowCol;
          }
        }
        outData[(i1 * outChPitch) + (i2 * outPitch) + i3] = maxValue;
      }
    }
  }
}

template void TIDL_refCustomMaxPoolingCore<int8_t>(int8_t *pInChannel,
                                                   int32_t width,
                                                   int32_t height,
                                                   int32_t inPitch,
                                                   int32_t inChPitch,
                                                   int32_t outPitch,
                                                   int32_t outChPitch,
                                                   int32_t numOutChannels,
                                                   int32_t kernelW,
                                                   int32_t kernelH,
                                                   int32_t strideW,
                                                   int32_t strideH,
                                                   int32_t padW,
                                                   int32_t padH,
                                                   int8_t initValue,
                                                   int8_t *pOutChannel,
                                                   sTIDL_Layer_t *tidlLayer);

template void TIDL_refCustomMaxPoolingCore<uint8_t>(uint8_t *pInChannel,
                                                    int32_t width,
                                                    int32_t height,
                                                    int32_t inPitch,
                                                    int32_t inChPitch,
                                                    int32_t outPitch,
                                                    int32_t outChPitch,
                                                    int32_t numOutChannels,
                                                    int32_t kernelW,
                                                    int32_t kernelH,
                                                    int32_t strideW,
                                                    int32_t strideH,
                                                    int32_t padW,
                                                    int32_t padH,
                                                    uint8_t initValue,
                                                    uint8_t *pOutChannel,
                                                    sTIDL_Layer_t *tidlLayer);

template void TIDL_refCustomMaxPoolingCore<int16_t>(int16_t *pInChannel,
                                                    int32_t width,
                                                    int32_t height,
                                                    int32_t inPitch,
                                                    int32_t inChPitch,
                                                    int32_t outPitch,
                                                    int32_t outChPitch,
                                                    int32_t numOutChannels,
                                                    int32_t kernelW,
                                                    int32_t kernelH,
                                                    int32_t strideW,
                                                    int32_t strideH,
                                                    int32_t padW,
                                                    int32_t padH,
                                                    int16_t initValue,
                                                    int16_t *pOutChannel,
                                                    sTIDL_Layer_t *tidlLayer);

template void TIDL_refCustomMaxPoolingCore<uint16_t>(uint16_t *pInChannel,
                                                     int32_t width,
                                                     int32_t height,
                                                     int32_t inPitch,
                                                     int32_t inChPitch,
                                                     int32_t outPitch,
                                                     int32_t outChPitch,
                                                     int32_t numOutChannels,
                                                     int32_t kernelW,
                                                     int32_t kernelH,
                                                     int32_t strideW,
                                                     int32_t strideH,
                                                     int32_t padW,
                                                     int32_t padH,
                                                     uint16_t initValue,
                                                     uint16_t *pOutChannel,
                                                     sTIDL_Layer_t *tidlLayer);

int32_t TIDL_customMaxPoolingProcess(void *tidlHandle,
                                     sTIDL_Layer_t *tidlLayer,
                                     void *inPtrs[],
                                     void *outPtrs[],
                                     void *params,
                                     void *dmaUtilsContext,
                                     const sTIDL_sysMemHandle_t sysMems[TIDL_SYSMEM_MAX],
                                     int32_t execMode)
{
  int32_t status = CUSTOM_SUCCESS;
  float32_tidl currMin = 0, currMax = 0;

  /* When execMode == TIDL_EXEC_MODE_STATS_COLLECTION then user is expected to find
       run a platform agnostic code for a given layer. This code is expected to support both float
       and fixed point implemenation. User can create template based code to handle multiple data types and
      appropriately call the right template as its done for 8 bit and 16 bit
      element type in this example */

  if ((TIDL_EXEC_MODE_STATS_COLLECTION == execMode) ||
      (TIDL_EXEC_MODE_INFER_PROCESS_REF == execMode))
  {

    /* Note that for max pooling layer we don't need to call the reference function
    during stats collection as max pooling stats are same as its producers layers.
    But in general user is expected to write a function to do stats collection.
    */

    /* Call reference implementation of the layer */
    TIDL_refCustomMaxPoolingProcess(tidlLayer,
                                    (TIDL_CustomParams0_t *)params,
                                    inPtrs,
                                    outPtrs,
                                    &currMin,
                                    &currMax);
  }
  else if (TIDL_EXEC_MODE_INFER_PROCESS == execMode)
  {
    /* This is the location where user is supposed to call the optimized
    implementation of their layer */
    status = TIDL_customMaxPoolDspProcess(tidlHandle,
                                          tidlLayer,
                                          (TIDL_CustomParams0_t *)params,
                                          inPtrs,
                                          outPtrs);
  }
  else
  {
    status = CUSTOM_FAIL;
  }
  return status;
}
