/*
 *
 * Copyright (c) {2022 - 2024} Texas Instruments Incorporated
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
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
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
----------------------------------------------------------------------------
@file    tidl_deformConv.c
@brief   Reference implementation for DCN
@version 0.1 (Nov 2024) : Initial version [VT]
----------------------------------------------------------------------------
*/

#include "tidl_alg_int.h"
#include "tidl_deformConv.h"
#include "tidl_commonUtils.h"

#define TIDL_DCN_INTERPOLATION_BILINEAR (1U)
#define TIDL_DCN_INTERPOLATION_NEAREST (0U)

// #define TIDL_DCN_DATA_DEBUG
#ifdef HOST_EMULATION
template<class Tin, class Toffset, class Tpos, class Tmask,
         class ToffsetWeights, class Tacc> static inline Tin
TIDL_interpolateInputFixed(
    const Tin *inPtr, const Toffset *offsetPtr,
    Tmask maskValue, /*Single mask value for per coefficient*/
    float32_tidl maskScale, const sTIDL_DataParams_t *inOffsetParams,
    const sTIDL_DataParams_t *inDataParams,
    int32_t filterX_position, // From loop counts
    int32_t filterY_position, int32_t kernelW, int32_t outX_position,
    int32_t outY_position, const sTIDL_DeformConvParams_t *params)
{
  /*Get float offset values: HoxWox(N*2)*/
  Toffset offsetY = offsetPtr[(outY_position * inOffsetParams->pitch[TIDL_CHANNEL_PITCH]) + (outX_position * (inOffsetParams->pitch[TIDL_LINE_PITCH])) + ((((filterY_position * (kernelW)) + filterX_position) * 2) + 0)];
  Toffset offsetX = offsetPtr[(outY_position * inOffsetParams->pitch[TIDL_CHANNEL_PITCH]) + (outX_position * (inOffsetParams->pitch[TIDL_LINE_PITCH])) + ((((filterY_position * (kernelW)) + filterX_position) * 2) + 1)];

  /*Offset Scale is in P2 form*/
  int32_t offsetScale = inOffsetParams->tensorScale;
  uint32_t precisionBits = (int32_t)round(log(offsetScale) / log(2.0));
  int32_t maskPrecisionBits = (int32_t)round(log(maskScale) / log(2.0));
  Tin interpolatedResult = 0;
  /*Compute the input index via bilinear interpolation/Nearest Neighbour sampling:*/
  int32_t samplingMode = params->samplingMode;

  if (samplingMode == (int32_t)TIDL_ModeBilinear)
  {
    Toffset offsetXint = offsetX >> precisionBits;
    Toffset offsetYint = offsetY >> precisionBits;
    Tpos inX, inY;
    inX = ((outX_position * params->strideW) - params->padW) + (filterX_position * params->dilationW) + offsetXint;
    inY = ((outY_position * params->strideH) - params->padH) + (filterY_position * params->dilationH) + offsetYint;
    /*Bounds check:*/
    int32_t inWidth = inDataParams->dimValues[TIDL_DIM_HEIGHT];
    int32_t inHeight = inDataParams->dimValues[TIDL_DIM_NUMCH];

    if ((inX < -1) || (inWidth <= inX) || (inY < -1) || (inHeight <= inY))
    {
      return interpolatedResult;
    }
    int32_t X_floor = inX;
    int32_t Y_floor = inY;
    int32_t X_ceil = X_floor + 1;
    int32_t Y_ceil = Y_floor + 1;
    Tin X_00 = 0;
    if ((Y_floor >= 0) && (X_floor >= 0))
    {
      X_00 = inPtr[(Y_floor * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (X_floor * inDataParams->pitch[TIDL_LINE_PITCH])];
    }
    Tin X_01 = 0;
    if ((Y_floor >= 0) && (X_ceil <= (inWidth - 1)))
    {
      X_01 = inPtr[(Y_floor * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (X_ceil * inDataParams->pitch[TIDL_LINE_PITCH])];
    }
    Tin X_10 = 0;
    if ((Y_ceil <= (inHeight - 1)) && (X_floor >= 0))
    {
      X_10 = inPtr[(Y_ceil * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (X_floor * inDataParams->pitch[TIDL_LINE_PITCH])];
    }
    Tin X_11 = 0;
    if ((Y_ceil <= (inHeight - 1)) && (X_ceil <= (inWidth - 1)))
    {
      X_11 = inPtr[(Y_ceil * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (X_ceil * inDataParams->pitch[TIDL_LINE_PITCH])];
    }

    /*Interpolation:*/
    /*Fractional compute: (In offsetScale)*/
    Tin fractionalMask = (1U << precisionBits) - 1U;
    Tin X_frac = (Tin)(offsetX & fractionalMask);
    Tin Y_frac = (Tin)(offsetY & fractionalMask);

    /*Bilinear weights (In offsetScale^2):*/
    ToffsetWeights w00 = (offsetScale - X_frac) * (offsetScale - Y_frac);
    if ((X_frac == 0) && (Y_frac == 0))
    {
      /*Alternatively could saturate w00*/
      w00 = std::numeric_limits<ToffsetWeights>::max();
    }
    ToffsetWeights w01 = X_frac * (offsetScale - Y_frac);
    ToffsetWeights w10 = (offsetScale - X_frac) * Y_frac;
    ToffsetWeights w11 = X_frac * Y_frac;

    /*Interpolate the input values:*/
    Tacc interpolationAcc = (X_00 * w00) + (X_01 * w01) + (X_10 * w10) + (X_11 * w11);
    interpolationAcc = interpolationAcc * maskValue; /*40-bit in 8-bit case (using 64-bit container) & 64-bit in 16-bit mode*/
    interpolatedResult = TIDL_roundSatMMA(
        interpolationAcc, (2 * precisionBits) + maskPrecisionBits,
        std::numeric_limits<Tin>::lowest(), std::numeric_limits<Tin>::max());
  }
  else
  // if(samplingMode == (int32_t)TIDL_ModeNearest)
  {
    Toffset offsetXint = 0;
    offsetXint = TIDL_roundSat(offsetX, precisionBits, std::numeric_limits<Toffset>::lowest(), std::numeric_limits<Toffset>::max());
    Toffset offsetYint = 0;
    offsetYint = TIDL_roundSat(offsetY, precisionBits, std::numeric_limits<Toffset>::lowest(), std::numeric_limits<Toffset>::max());
    Tpos inX, inY;
    inX = ((outX_position * params->strideW) - params->padW) + (filterX_position * params->dilationW) + offsetXint;
    inY = ((outY_position * params->strideH) - params->padH) + (filterY_position * params->dilationH) + offsetYint;
    /*Bounds check:*/
    int32_t inWidth = inDataParams->dimValues[TIDL_DIM_HEIGHT];
    int32_t inHeight = inDataParams->dimValues[TIDL_DIM_NUMCH];

    if ((inX < -1) || (inWidth <= inX) || (inY < -1) || (inHeight <= inY))
    {
      return interpolatedResult;
    }
    /*Nearest neighbour:*/
    int32_t xint_round = inX; // TIDL_roundSat(inX, precisionBits, std::numeric_limits<int32_t>::lowest(), std::numeric_limits<int32_t>::max());
    int32_t yint_round = inY; // TIDL_roundSat(inY, precisionBits, std::numeric_limits<int32_t>::lowest(), std::numeric_limits<int32_t>::max());
    interpolatedResult = ((yint_round >= 0) && (yint_round < inHeight) && (xint_round >= 0) && (xint_round < inWidth)) ? inPtr[(yint_round * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (xint_round * inDataParams->pitch[TIDL_LINE_PITCH])] : (Tin)0;
  }
  // else
  // {
  //   /*do nothing*/
  // }

  return interpolatedResult;
}

template<class Tin, class Toffset, class Tbias, class Tmask, class Tw, class Tout, class Tpos, class ToffsetWeights, class Tacc>
    int32_t TIDL_refDeformConvKernelFixed(Tin *pIn,
                                          Toffset *pOffset,
                                          const Tbias *biasPtr,
                                          Tmask *pMask,
                                          const Tw *weightPtr,
                                          Tout *pOut,
                                          const sTIDL_DataParams_t *inDataParams,
                                          const sTIDL_DataParams_t *inOffsetParams,
                                          const sTIDL_DataParams_t *inMaskParams,
                                          const sTIDL_DataParams_t *outDataParams,
                                          const sTIDL_DeformConvParams_t *params)
{
  int32_t status = IALG_EOK;

  /*Unwrap deform convolution parameters:*/
  int32_t kernelH = params->kernelH;
  int32_t kernelW = params->kernelW;
  Tacc accumulator = 0;
  /*Resolution parameters (Note - there is a hard assertion that DCN is at max 4D (From ONNX)*/
  int32_t outHeight = outDataParams->dimValues[TIDL_DIM_HEIGHT];
  int32_t outWidth = outDataParams->dimValues[TIDL_DIM_WIDTH];
  int32_t outChannels = outDataParams->dimValues[TIDL_DIM_NUMCH];
  int32_t outDim1 = outDataParams->dimValues[TIDL_DIM_DIM1];
  int32_t outDim2 = outDataParams->dimValues[TIDL_DIM_DIM2];
  int32_t outBatch = outDataParams->dimValues[TIDL_DIM_BATCH];
  int32_t numChannels = inDataParams->dimValues[TIDL_DIM_WIDTH];
  int32_t inChPitch = 1U;

  /*Shape assertions*/
  if((2 * kernelH * kernelW) > (inOffsetParams->dimValues[TIDL_DIM_WIDTH]))
  {
    status = IALG_EFAIL;
  }
  if ((1 * kernelH * kernelW) != (inMaskParams->dimValues[TIDL_DIM_WIDTH]))
  {
    status = IALG_EFAIL;
  }
  if (status == IALG_EOK)
  {
    /*Adjust pointers to account for pad in data buffers:*/
    Tin* inPtr;
    Tout* outPtr;
    Toffset *offsetPtr;
    Tmask *maskPtr;

    /*Top level function is responsible for providing corrected pad values*/
    inPtr = (Tin*) pIn;//+ (inDataParams->padH * inDataParams->pitch[TIDL_LINE_PITCH]) + inDataParams->padW;
    outPtr = (Tout*) pOut;// + (outDataParams->padH * outDataParams->pitch[TIDL_LINE_PITCH]) + outDataParams->padW;
    offsetPtr = (Toffset*) pOffset;// + (inOffsetParams->padH * inOffsetParams->pitch[TIDL_LINE_PITCH]) + inOffsetParams->padW;
    maskPtr = (Tmask*) pMask;// + (inMaskParams->padH * inMaskParams->pitch[TIDL_LINE_PITCH]) + inMaskParams->padW;

    /*Deformable Convolution implementation*/
    int32_t outBatchIdx, outDim1Idx, outDim2Idx, outChannelIdx, outHeightIdx, outWidthIdx, inChannelIdx, kernelHIdx, kernelWIdx;

    /*Iterate over output batches:*/
    for(outBatchIdx = 0; outBatchIdx < outBatch; outBatchIdx++)
    {
      /*Iterate over output dim1:*/
      for(outDim1Idx = 0; outDim1Idx < outDim1; outDim1Idx++)
      {
        /*Iterate over output dim2:*/
        for(outDim2Idx = 0; outDim2Idx < outDim2; outDim2Idx++)
        {
          int32_t inPtrOffset = (outBatchIdx * inDataParams->pitch[TIDL_ROI_PITCH]) + (outDim1Idx * inDataParams->pitch[TIDL_DIM1_PITCH]) + (outDim2Idx * inDataParams->pitch[TIDL_DIM2_PITCH]);
          int32_t offsetPtrOffset = (outBatchIdx * inOffsetParams->pitch[TIDL_ROI_PITCH]) + (outDim1Idx * inOffsetParams->pitch[TIDL_DIM1_PITCH]) + (outDim2Idx * inOffsetParams->pitch[TIDL_DIM2_PITCH]);
          int32_t maskPtrOffset = (outBatchIdx * inMaskParams->pitch[TIDL_ROI_PITCH]) + (outDim1Idx * inMaskParams->pitch[TIDL_DIM1_PITCH]) + (outDim2Idx * inMaskParams->pitch[TIDL_DIM2_PITCH]);
          int32_t outBatchOffset = (outBatchIdx * outDataParams->pitch[TIDL_ROI_PITCH]) + (outDim1Idx * outDataParams->pitch[TIDL_DIM1_PITCH]) + (outDim2Idx * outDataParams->pitch[TIDL_DIM2_PITCH]);

          /*Iterate over output channels:*/
          for(outChannelIdx = 0; outChannelIdx < outChannels ; outChannelIdx++)
          {
            /*Iterate over output height:*/
            for(outHeightIdx = 0; outHeightIdx < outHeight; outHeightIdx++)
            {
              /*Iterate over output width*/
              for(outWidthIdx = 0; outWidthIdx < outWidth; outWidthIdx++)
              {
                /*Compute the accumulated value*/
                accumulator = biasPtr[outChannelIdx];

                for(kernelHIdx = 0; kernelHIdx < kernelH; kernelHIdx++)
                {
                  for(kernelWIdx = 0; kernelWIdx < kernelW; kernelWIdx++)
                  {
                    for(inChannelIdx = 0; inChannelIdx < numChannels; inChannelIdx++)
                    {
                      /*Interpolate input values based on the offset field*/
                      Tin inValue = (Tin)0;
                      inValue = TIDL_interpolateInputFixed<Tin, Toffset, Tpos, Tmask, ToffsetWeights, int64_t>(
                        inPtr + inPtrOffset + (inChannelIdx * inChPitch),
                        offsetPtr + offsetPtrOffset,
                        maskPtr[maskPtrOffset + (outHeightIdx * inMaskParams->pitch[TIDL_CHANNEL_PITCH]) + (outWidthIdx * inMaskParams->pitch[TIDL_LINE_PITCH]) + ((kernelHIdx * kernelW) + kernelWIdx)],
                        inMaskParams->tensorScale, /*Always in P2 notation*/
                        inOffsetParams,
                        inDataParams,
                        kernelWIdx,
                        kernelHIdx,
                        kernelW,
                        outWidthIdx,
                        outHeightIdx,
                        params
                      );
                      /*Read the weight value (NoFrFcNi)*/
                      Tw weightValue = weightPtr[(outChannelIdx * (numChannels * kernelH * kernelW)) + (inChannelIdx * (1)) + (kernelHIdx * (kernelW * numChannels)) + (kernelWIdx * (numChannels))];
                      /*Compute the output value*/
                      accumulator += inValue * weightValue;
                    }
                  }
                }
                /*Store the output value (Currently float only mode)*/
                Tout outVal = (Tout)0;
                outVal = TIDL_roundSatMMA(accumulator, outDataParams->roundBits, std::numeric_limits<Tout>::lowest(), std::numeric_limits<Tout>::max());
                outPtr[outBatchOffset + (outChannelIdx * outDataParams->pitch[TIDL_CHANNEL_PITCH]) + (outHeightIdx * outDataParams->pitch[TIDL_LINE_PITCH]) + outWidthIdx] = outVal;
              }
            } 
          }
        }
      }
    }
  }
  return status;
}

template<class Tin, class Toffset> static inline Tin TIDL_interpolateInput(Tin *inPtr,
                                                                           Toffset *offsetPtr,
                                                                           const sTIDL_DataParams_t *inOffsetParams,
                                                                           const sTIDL_DataParams_t *inDataParams,
                                                                           int32_t filterX_position, // From loop counts
                                                                           int32_t filterY_position,
                                                                           int32_t kernelW,
                                                                           int32_t outX_position,
                                                                           int32_t outY_position,
                                                                           const sTIDL_DeformConvParams_t *params)
{
  /*Get float offset values: HoxWox(N*2)*/
  Toffset offsetY = offsetPtr[(outY_position * inOffsetParams->pitch[TIDL_CHANNEL_PITCH]) + (outX_position * (inOffsetParams->pitch[TIDL_LINE_PITCH])) + ((((filterY_position * (kernelW)) + filterX_position) * 2) + 0)];
  Toffset offsetX = offsetPtr[(outY_position * inOffsetParams->pitch[TIDL_CHANNEL_PITCH]) + (outX_position * (inOffsetParams->pitch[TIDL_LINE_PITCH])) + ((((filterY_position * (kernelW)) + filterX_position) * 2) + 1)];
  /*Compute the input index via bilinear interpolation/Nearest Neighbour sampling:*/
  int32_t samplingMode = params->samplingMode;
  float32_tidl interpolatedResult = 0;
  /*Arbitrarily using a float container for offset position*/
  float32_tidl inX, inY;
  inX = ((outX_position * params->strideW) - params->padW) + (filterX_position * params->dilationW) + offsetX;
  inY = ((outY_position * params->strideH) - params->padH) + (filterY_position * params->dilationH) + offsetY;
  /*Bounds check:*/
  int32_t inWidth = inDataParams->dimValues[TIDL_DIM_HEIGHT];
  int32_t inHeight = inDataParams->dimValues[TIDL_DIM_NUMCH];
  /*Check bounds*/
  if ((inX <= -1.0f) || ((float32_tidl)inWidth <= inX) || (inY <= -1.0f) || ((float32_tidl)inHeight <= inY))
  {
    return interpolatedResult;
  }

  if (samplingMode == (int32_t)TIDL_ModeBilinear)
  {
    int32_t X_floor = floor(inX);
    int32_t Y_floor = floor(inY);
    int32_t X_ceil = X_floor + 1;
    int32_t Y_ceil = Y_floor + 1;

    /*Index and pick these 4 points:*/

    Tin X_00 = 0;
    if ((Y_floor >= 0) && (X_floor >= 0))
    {
      X_00 = inPtr[(Y_floor * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (X_floor * inDataParams->pitch[TIDL_LINE_PITCH])];
    }
    Tin X_01 = 0;
    if ((Y_floor >= 0) && (X_ceil <= (inWidth - 1)))
    {
      X_01 = inPtr[(Y_floor * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (X_ceil * inDataParams->pitch[TIDL_LINE_PITCH])];
    }
    Tin X_10 = 0;
    if ((Y_ceil <= (inHeight - 1)) && (X_floor >= 0))
    {
      X_10 = inPtr[(Y_ceil * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (X_floor * inDataParams->pitch[TIDL_LINE_PITCH])];
    }
    Tin X_11 = 0;
    if ((Y_ceil <= (inHeight - 1)) && (X_ceil <= (inWidth - 1)))
    {
      X_11 = inPtr[(Y_ceil * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (X_ceil * inDataParams->pitch[TIDL_LINE_PITCH])];
    }

    float32_tidl X_frac = inX - (float32_tidl)X_floor;
    float32_tidl Y_frac = inY - (float32_tidl)Y_floor;

    /*Bilinear weights:*/
    float32_tidl w00 = (1.0f - X_frac) * (1.0f - Y_frac);
    float32_tidl w01 = X_frac * (1.0f - Y_frac);
    float32_tidl w10 = (1.0f - X_frac) * Y_frac;
    float32_tidl w11 = X_frac * Y_frac;

    /*Interpolate the input values:*/
    interpolatedResult = (X_00 * w00) + (X_01 * w01) + (X_10 * w10) + (X_11 * w11);
  }
  else
  {
    /*Using Banker's rounding in float mode:*/
    int32_t xint_round = nearbyintf(inX);
    int32_t yint_round = nearbyintf(inY);
    interpolatedResult = ((yint_round >= 0) && (yint_round < inHeight) && (xint_round >= 0) && (xint_round < inWidth)) ? inPtr[(yint_round * inDataParams->pitch[TIDL_CHANNEL_PITCH]) + (xint_round * inDataParams->pitch[TIDL_LINE_PITCH])] : (Tin)0;
  }

  return interpolatedResult;
}

template<class Tin, class Toffset, class Tbias, class Tmask, class Tw, class Tout, class Tacc>
    int32_t TIDL_refDeformConvKernelFloat(Tin *pIn,
                                          Toffset *pOffset,
                                          Tbias *biasPtr,
                                          Tmask *pMask,
                                          Tw *weightPtr,
                                          Tout *pOut,
                                          const sTIDL_DataParams_t *inDataParams,
                                          const sTIDL_DataParams_t *inOffsetParams,
                                          const sTIDL_DataParams_t *inMaskParams,
                                          const sTIDL_DataParams_t *outDataParams,
                                          sTIDL_DeformConvParams_t *params)
{
  int32_t status = IALG_EOK;

  /*Unwrap deform convolution parameters:*/
  int32_t kernelH = params->kernelH;
  int32_t kernelW = params->kernelW;
  Tacc accumulator = 0;
  /*Resolution parameters (Note - there is a hard assertion that DCN is at max 4D (From ONNX)*/
  int32_t outHeight = outDataParams->dimValues[TIDL_DIM_HEIGHT];
  int32_t outWidth = outDataParams->dimValues[TIDL_DIM_WIDTH];
  int32_t outChannels = outDataParams->dimValues[TIDL_DIM_NUMCH];
  int32_t outDim2 = outDataParams->dimValues[TIDL_DIM_DIM2];
  int32_t outDim1 = outDataParams->dimValues[TIDL_DIM_DIM1];
  int32_t outBatch = outDataParams->dimValues[TIDL_DIM_BATCH];
  int32_t numChannels = inDataParams->dimValues[TIDL_DIM_WIDTH];
  int32_t inChPitch = 1U;
  /*Shape assertions*/
  if((2 * kernelH * kernelW) > (inOffsetParams->dimValues[TIDL_DIM_WIDTH]))
  {
    status = IALG_EFAIL;
  }
  if ((1 * kernelH * kernelW) != (inMaskParams->dimValues[TIDL_DIM_WIDTH]))
  {
    status = IALG_EFAIL;
  }
  if (status == IALG_EOK)
  {
    /*Adjust pointers to account for pad in data buffers:*/
    Tin* inPtr;
    Tout* outPtr;
    Toffset *offsetPtr;
    Tmask *maskPtr;

    /*Top level function is responsible for providing corrected pad values*/
    /*Input is in HWC layout, i.e. Channel Pitch --> Line Pitch && Line Pitch ---> Width Pitch, with channels next to each other*/
    /*Weight buffers are also laid out in NoFrFcNi format*/
    inPtr = (Tin*) pIn ;
    outPtr = (Tout*) pOut;
    offsetPtr = (Toffset*) pOffset;
    maskPtr = (Tmask*) pMask;

    /*Deformable Convolution implementation*/
    int32_t outBatchIdx, outDim1Idx, outDim2Idx, outChannelIdx, outHeightIdx, outWidthIdx, inChannelIdx, kernelHIdx, kernelWIdx;

    for(outBatchIdx = 0; outBatchIdx < outBatch; outBatchIdx++)
    {
      for(outDim1Idx = 0; outDim1Idx < outDim1; outDim1Idx++)
      {
        for(outDim2Idx = 0; outDim2Idx < outDim2; outDim2Idx++)
        {
          int32_t inPtrOffset = (outBatchIdx * inDataParams->pitch[TIDL_ROI_PITCH]) + (outDim1Idx * inDataParams->pitch[TIDL_DIM1_PITCH]) + (outDim2Idx * inDataParams->pitch[TIDL_DIM2_PITCH]);
          int32_t offsetPtrOffset = (outBatchIdx * inOffsetParams->pitch[TIDL_ROI_PITCH]) + (outDim1Idx * inOffsetParams->pitch[TIDL_DIM1_PITCH]) + (outDim2Idx * inOffsetParams->pitch[TIDL_DIM2_PITCH]);

          int32_t maskPtrOffset = (outBatchIdx * inMaskParams->pitch[TIDL_ROI_PITCH]) + (outDim1Idx * inMaskParams->pitch[TIDL_DIM1_PITCH]) + (outDim2Idx * inMaskParams->pitch[TIDL_DIM2_PITCH]);
          int32_t outBatchOffset = (outBatchIdx * outDataParams->pitch[TIDL_ROI_PITCH]) + (outDim1Idx * outDataParams->pitch[TIDL_DIM1_PITCH]) + (outDim2Idx * outDataParams->pitch[TIDL_DIM2_PITCH]);

          /*Iterate over output channels:*/
          for(outChannelIdx = 0; outChannelIdx < outChannels ; outChannelIdx++)
          {
            /*Iterate over output height:*/
            for(outHeightIdx = 0; outHeightIdx < outHeight; outHeightIdx++)
            {
              /*Iterate over output width*/
              for(outWidthIdx = 0; outWidthIdx < outWidth; outWidthIdx++)
              {
                /*Compute the accumulated value*/

                accumulator = biasPtr[outChannelIdx];

                for(inChannelIdx = 0; inChannelIdx < numChannels; inChannelIdx++)
                {
                  for(kernelHIdx = 0; kernelHIdx < kernelH; kernelHIdx++)
                  {
                    for(kernelWIdx = 0; kernelWIdx < kernelW; kernelWIdx++)
                    {
                      /*Interpolate input values based on the offset field*/
                      Tin inValue = TIDL_interpolateInput(
                        inPtr + inPtrOffset + (inChannelIdx * inChPitch), /*Offset to the correct channel*/
                        offsetPtr + offsetPtrOffset,
                        inOffsetParams,
                        inDataParams,
                        kernelWIdx,
                        kernelHIdx,
                        kernelW,
                        outWidthIdx,
                        outHeightIdx,
                        params
                      );
                      /*Read the weight value (NoFrFcNi)*/
                      Tw weightValue = weightPtr[(outChannelIdx * (numChannels * kernelH * kernelW)) + (inChannelIdx * (1)) + (kernelHIdx * (kernelW * numChannels)) + (kernelWIdx * (numChannels))];
                      /*Masks are in HW(FrFc) layout now:*/
                      Tmask maskValue =  maskPtr[maskPtrOffset + (outHeightIdx * inMaskParams->pitch[TIDL_CHANNEL_PITCH]) + (outWidthIdx * inMaskParams->pitch[TIDL_LINE_PITCH]) + ((kernelHIdx * kernelW) + kernelWIdx)];
                      /*Update weight value:*/
                      Tacc weightValueUpdated = weightValue * maskValue;
                      /*Compute the output value*/
                      accumulator += inValue * weightValueUpdated;
                    }
                  }
                }
                /*Store the output value (Currently float only mode)*/
                outPtr[outBatchOffset + (outChannelIdx * outDataParams->pitch[TIDL_CHANNEL_PITCH]) + (outHeightIdx * outDataParams->pitch[TIDL_LINE_PITCH]) + outWidthIdx] = accumulator;
              }
            }
          }
        }
      }
    }
  }

  return status;
}
#endif
void *TIDL_deformConvRequantizeBias(int16_t *biasPtrSrc,
                                    void *biasPtrDst,
                                    sTIDL_Layer_t *tidlLayer,
                                    const sTIDL_DeformConvParams_t *dcnParams,
                                    const sTIDL_DataParams_t *outDataParams)
{
  int32_t numBiasElems = dcnParams->numOutChannels;
  int32_t i0;
  float32_tidl accScale = outDataParams->tensorScale * pow(2.0, outDataParams->roundBits);
  float32_tidl reScaleFactor = accScale / dcnParams->biasScale;
  for (i0 = 0; i0 < numBiasElems; i0++)
  {
    if (tidlLayer->weightsElementSizeInBits <= 8)
    {
      int32_t *biasPtr = (int32_t *)biasPtrDst;
      int32_t biasPtr_cal = 0;
      biasPtr_cal = ((float32_tidl)biasPtrSrc[i0] * reScaleFactor);
      biasPtr[i0] = (int32_t)biasPtr_cal;
    }
    else if ((tidlLayer->weightsElementSizeInBits > 8) && (tidlLayer->weightsElementSizeInBits <= 16))
    {
      int64_t *biasPtr = (int64_t *)biasPtrDst;
      int64_t biasPtr_cal = 0;
      biasPtr_cal = ((float32_tidl)biasPtrSrc[i0] * reScaleFactor);
      biasPtr[i0] = (int64_t)biasPtr_cal;
    }
    else
    {
      /*Direct memcpy:*/
      memcpy(biasPtrDst, (void *)biasPtrSrc, numBiasElems * (int64_t)sizeof(float32_tidl));
    }
  }
  return (void *)biasPtrDst;
}

#ifdef HOST_EMULATION
int32_t TIDL_refDeformConv2dProcess(TIDL_NetworkCommonParams *commonParams,
                                    sTIDL_AlgLayer_t *algLayer,
                                    sTIDL_Layer_t *tidlLayer,
                                    void *inPtrs[],
                                    void *outPtrs[],
                                    int32_t layerIdx)
{
  int32_t status = IALG_EOK;
  sTIDL_Network_t *net = commonParams->net;
  sTIDL_DeformConvParams_t *params = &commonParams->net->TIDLLayers[layerIdx].layerParams.deformConvParams;
  /*DCN expects 3 variable inputs: */
  void *inData = inPtrs[0];
  void *inOffset = inPtrs[1];
  void *inMask = inPtrs[2];
  /*Single output generated: */
  void *outData = outPtrs[0];
  /*Read input & output buffer properties:*/
  sTIDL_DataParams_t *inDataParams;
  sTIDL_DataParams_t *inOffsetParams;
  sTIDL_DataParams_t *inMaskParams;
  sTIDL_DataParams_t *outDataParams;
  inDataParams = &net->TIDLLayers[(int32_t)algLayer->inLayerIdx[0]].outData;
  inOffsetParams = &net->TIDLLayers[(int32_t)algLayer->inLayerIdx[1]].outData;
  inMaskParams = &net->TIDLLayers[(int32_t)algLayer->inLayerIdx[2]].outData;
  outDataParams = &tidlLayer->outData;
  int32_t biasSize = params->numOutChannels;

  if (commonParams->createParams->forceNegativeTest == TIDL_SAFETY_FLAG_DCN_FORCE_INVALID_OFFSET_DIMS)
  {
    /* Set to an invalid offset dim */
    inOffsetParams->dimValues[TIDL_DIM_WIDTH] = params->kernelW;
  }

  if (commonParams->createParams->forceNegativeTest == TIDL_SAFETY_FLAG_DCN_FORCE_INVALID_MASK_DIMS)
  {
    /* Set to an invalid mask dim */
    inMaskParams->dimValues[TIDL_DIM_WIDTH] = params->kernelW;
  }

  if (commonParams->createParams->forceNegativeTest == TIDL_SAFETY_FLAG_DCN_FORCE_BIAS_DISABLE)
  {
    /* Disable bias */
    params->enableBias = 0;
  }

  if (commonParams->createParams->forceNegativeTest == TIDL_SAFETY_FLAG_DCN_FORCE_SMALLER_SCRATCH_SIZE)
  {
    /* Scratch size smaller */
    algLayer->scratchSize = biasSize - 1;
  }

  if (commonParams->createParams->forceNegativeTest == TIDL_SAFETY_FLAG_DCN_FORCE_DATATYPE_ERROR)
  {
    /* Invalid datatype */
    inDataParams->elementType = TIDL_SignedWord;
  }
  if (commonParams->createParams->forceNegativeTest == TIDL_SAFETY_FLAG_DCN_FORCE_DATATYPENSIZE_ERROR)
  {
    /* Invalid datatype */
    inDataParams->elementType = TIDL_SignedWord;
    tidlLayer->weightsElementSizeInBits = 16;
  }
  /*Get the initializer pointers from the network:*/
  void *weightPtr = (void *)get_int8_t_pointer((int8_t *)(net), params->weights);
  void *biasPtr;
  if (params->enableBias != 0)
  {
    biasPtr = (void *)get_int8_t_pointer((int8_t *)(net), params->bias);
  }
  else
  {
    biasPtr = NULL;
    status = IALG_EFAIL;
  }

  /*ElementType Matrix:
    Input - Signed/Unsigned
    Output - Signed Unsigned
    Masks - Unsigned
    Weights - Signed
    Offsets - Signed
  */
  void *kernelBias = NULL;
  /*Get space for bias from scratchmem (Symmetric flow)*/
  if (params->enableBias != 0)
  {
    /*Convert 16-bit bias values to 32/64bit accucmulator bias values:*/
    if (algLayer->scratchSize >= biasSize)
    {
      kernelBias = TIDL_deformConvRequantizeBias((int16_t *)biasPtr, (void *)algLayer->scratchMem, tidlLayer, params, outDataParams);
    }
    else
    {
      status = IALG_EFAIL;
    }
  }

  if (status == IALG_EOK)
  {
    /*Invoke the appropriate DCN Kernel (Pointer adjustment done at this level!):*/
    if (tidlLayer->weightsElementSizeInBits <= 8)
    {
      /*8-bit implementation*/
      if (inDataParams->elementType == TIDL_SignedChar)
      {
#if defined TIDL_COVERAGE_DEAD_CODE
        if (outDataParams->elementType == TIDL_SignedChar)
        {
#endif
          status = TIDL_refDeformConvKernelFixed<int8_t, int8_t, int32_t, uint8_t, int8_t, int8_t, int16_t, uint8_t, int32_t>(
              (int8_t *)inData,
              (int8_t *)inOffset,
              (int32_t *)kernelBias,
              (uint8_t *)inMask,
              (int8_t *)weightPtr,
              (int8_t *)outData,
              inDataParams,
              inOffsetParams,
              inMaskParams,
              outDataParams,
              params);
#if defined TIDL_COVERAGE_DEAD_CODE
        }
        else if (outDataParams->elementType == TIDL_UnsignedChar)
        {
          status = TIDL_refDeformConvKernelFixed<int8_t, int8_t, int32_t, uint8_t, int8_t, uint8_t, int16_t, uint8_t, int32_t>(
              (int8_t *)inData,
              (int8_t *)inOffset,
              (int32_t *)kernelBias,
              (uint8_t *)inMask,
              (int8_t *)weightPtr,
              (uint8_t *)outData,
              inDataParams,
              inOffsetParams,
              inMaskParams,
              outDataParams,
              params);
        }
        else
        {
          status = IALG_EFAIL;
        }
#endif
      }
      else if (inDataParams->elementType == TIDL_UnsignedChar)
      {
#if defined TIDL_COVERAGE_DEAD_CODE
        if (outDataParams->elementType == TIDL_SignedChar)
        {
#endif
          status = TIDL_refDeformConvKernelFixed<uint8_t, int8_t, int32_t, uint8_t, int8_t, int8_t, int16_t, uint8_t, int32_t>(
              (uint8_t *)inData,
              (int8_t *)inOffset,
              (int32_t *)kernelBias,
              (uint8_t *)inMask,
              (int8_t *)weightPtr,
              (int8_t *)outData,
              inDataParams,
              inOffsetParams,
              inMaskParams,
              outDataParams,
              params);
#if defined TIDL_COVERAGE_DEAD_CODE
        }
        else if (outDataParams->elementType == TIDL_UnsignedChar)
        {
          status = TIDL_refDeformConvKernelFixed<uint8_t, int8_t, int32_t, uint8_t, int8_t, uint8_t, int16_t, uint8_t, int32_t>(
              (uint8_t *)inData,
              (int8_t *)inOffset,
              (int32_t *)kernelBias,
              (uint8_t *)inMask,
              (int8_t *)weightPtr,
              (uint8_t *)outData,
              inDataParams,
              inOffsetParams,
              inMaskParams,
              outDataParams,
              params);
        }
        else
        {
          status = IALG_EFAIL;
        }
#endif
      }
      else
      {
        status = IALG_EFAIL;
      }
    }
    else if ((tidlLayer->weightsElementSizeInBits > 8) && (tidlLayer->weightsElementSizeInBits <= 16))
    {
      /*16-bit implementation*/
      if (inDataParams->elementType == TIDL_SignedShort)
      {
#if defined TIDL_COVERAGE_DEAD_CODE
        if (outDataParams->elementType == TIDL_SignedShort)
        {
#endif
          status = TIDL_refDeformConvKernelFixed<int16_t, int16_t, int64_t, int16_t, int16_t, int16_t, int32_t, uint16_t, int64_t>(
              (int16_t *)inData,
              (int16_t *)inOffset,
              (int64_t *)kernelBias,
              (int16_t *)inMask,
              (int16_t *)weightPtr,
              (int16_t *)outData,
              inDataParams,
              inOffsetParams,
              inMaskParams,
              outDataParams,
              params);
#if defined TIDL_COVERAGE_DEAD_CODE
        }
        else if (outDataParams->elementType == TIDL_UnsignedShort)
        {
          status = TIDL_refDeformConvKernelFixed<int16_t, int16_t, int64_t, int16_t, int16_t, uint16_t, int32_t, uint16_t, int64_t>(
              (int16_t *)inData,
              (int16_t *)inOffset,
              (int64_t *)kernelBias,
              (int16_t *)inMask,
              (int16_t *)weightPtr,
              (uint16_t *)outData,
              inDataParams,
              inOffsetParams,
              inMaskParams,
              outDataParams,
              params);
        }
        else
        {
          status = IALG_EFAIL;
        }
#endif
      }
      else if (inDataParams->elementType == TIDL_UnsignedShort)
      {
#if defined TIDL_COVERAGE_DEAD_CODE
        if (outDataParams->elementType == TIDL_SignedShort)
        {
#endif
          status = TIDL_refDeformConvKernelFixed<uint16_t, int16_t, int64_t, int16_t, int16_t, int16_t, int32_t, uint16_t, int64_t>(
              (uint16_t *)inData,
              (int16_t *)inOffset,
              (int64_t *)kernelBias,
              (int16_t *)inMask,
              (int16_t *)weightPtr,
              (int16_t *)outData,
              inDataParams,
              inOffsetParams,
              inMaskParams,
              outDataParams,
              params);
#if defined TIDL_COVERAGE_DEAD_CODE
        }
        else if (outDataParams->elementType == TIDL_UnsignedShort)
        {
          status = TIDL_refDeformConvKernelFixed<uint16_t, int16_t, int64_t, int16_t, int16_t, uint16_t, int32_t, uint16_t, int64_t>(
              (uint16_t *)inData,
              (int16_t *)inOffset,
              (int64_t *)kernelBias,
              (int16_t *)inMask,
              (int16_t *)weightPtr,
              (uint16_t *)outData,
              inDataParams,
              inOffsetParams,
              inMaskParams,
              outDataParams,
              params);
        }
        else
        {
          status = IALG_EFAIL;
        }
#endif
      }
      else
      {
        status = IALG_EFAIL;
      }
    }
    else
    {
      /*Float implementation*/
      status = TIDL_refDeformConvKernelFloat<float32_tidl, float32_tidl, float32_tidl, float32_tidl, float32_tidl, float32_tidl, float32_tidl>(
          (float32_tidl *)inData,
          (float32_tidl *)inOffset,
          (float32_tidl *)kernelBias,
          (float32_tidl *)inMask,
          (float32_tidl *)weightPtr,
          (float32_tidl *)outData,
          inDataParams,
          inOffsetParams,
          inMaskParams,
          outDataParams,
          params);
    }
  }

  TIDL_L1DandL2CacheWbInv();
  return status;
}
#endif
/**
 ----------------------------------------------------------------------------
 @ingroup    TIDL_DeformConv
 @fn         TIDL_deformConvProcess
 @brief      Deformable convolution Layer process function

 @param      intAlgHandle : tidl algorothm handle
 @param      algLayer : Ptr to alg layer parameter used in DCN layer
 @param      tidlLayer: Ptr to tidl layer parameter used in DCN layer
 @param      inPtrs: Ptrs to input buffers to be processed
 @param      outPtrs: Ptrs to output buffers to be processed
 @param      sysMems: Ptr to memory related buffers used in DCN layer
 @remarks    None
 @return     IALG_EOK   - Successful
             IALG_EFAIL - Unspecified error
 ----------------------------------------------------------------------------
*/
int32_t TIDL_deformConvProcess(TIDL_NetworkCommonParams *commonParams,
                               sTIDL_AlgLayer_t *algLayer,
                               sTIDL_Layer_t *tidlLayer,
                               void *inPtrs[],
                               void *outPtrs[],
                               int32_t layerIdx)
{
  int32_t status = IALG_EOK;
#ifdef HOST_EMULATION
  /*Reference only implementation:*/
  if ((commonParams->createParams->flowCtrl & TIDL_FLOW_CTRL_REF_ONLY) != 0U)
  {
    status = TIDL_refDeformConv2dProcess(commonParams,
                                         algLayer,
                                         tidlLayer,
                                         inPtrs,
                                         outPtrs,
                                         layerIdx);
  }
  else
#endif
  {
    status = TIDL_deviceUtilsCommonProcess(commonParams,
                                           algLayer,
                                           tidlLayer,
                                           inPtrs,
                                           outPtrs,
                                           layerIdx);
  }

  return status;
}

#ifdef HOST_EMULATION
int32_t TIDL_deformConv2dRefInit(const sTIDL_DeformConvParams_t *dcnParams,
                                 const TIDL_LayerSpecificParams *layerSpecificParams,
                                 const TIDL_NetworkCommonParams *commonParams,
                                 sTIDL_AlgLayer_t *algLayer,
                                 int32_t layerIdx,
                                 uint8_t *memory[TIDL_LAYER_MEMORY_MAX],
                                 int32_t memorySize[TIDL_LAYER_MEMORY_MAX],
                                 void **outPtr)
{
  int32_t status = IALG_EOK;
  int32_t scratchDataSize;
  int32_t outDataSize;
  int32_t currOffset = 0;
  outDataSize = TIDL_refGetOutDataSize(commonParams, layerSpecificParams, layerIdx);

  (void)memset(memorySize, 0, TIDL_LAYER_MEMORY_MAX * (int64_t)sizeof(int32_t));

  scratchDataSize = dcnParams->numOutChannels * 4;
  currOffset = 0;
  TIDL_AllocatePtr((intptr_t)memory[TIDL_LAYER_MEMORY_SCRATCH],
                   &currOffset,
                   scratchDataSize,
                   128,
                   &algLayer->scratchMem);
  algLayer->scratchSize = scratchDataSize;
  currOffset = 0;

  if (commonParams->createParams->forceNegativeTest == TIDL_SAFETY_FLAG_DCN_FORCE_NULL_OUTPTR)
  {
    /* Set outPtr as NULL */
    outPtr = NULL;
  }

  if (outPtr != NULL)
  {
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> SAFETY_CHECK: Safe programming hard to hit this condition with real world data.
    <justification end> */
    if (TIDL_isOutDataBuff(commonParams->net,
                           commonParams->net->TIDLLayers[layerIdx].outData.dataId,
                           commonParams->createParams->currLayersGroupId) == 1)
    {
      *outPtr = NULL;
    }
    else if (outDataSize != 0)
    /* LDRA_JUSTIFY_END */
    {
      TIDL_AllocatePtr((intptr_t)memory[TIDL_LAYER_MEMORY_OUTPUT],
                       &currOffset,
                       outDataSize,
                       128,
                       outPtr);
    }
    else
    {
      *outPtr = NULL;
    }
  }
  else
  {
    status = IALG_EFAIL;
  }

  memorySize[TIDL_LAYER_MEMORY_SCRATCH] = scratchDataSize + 128;
  memorySize[TIDL_LAYER_MEMORY_OUTPUT] = outDataSize + 128;

  return status;
}

int32_t TIDL_deformConv2dRefAlloc(const sTIDL_DeformConvParams_t *dcnParams,
                                  const TIDL_LayerSpecificParams *layerSpecificParams,
                                  const TIDL_NetworkCommonParams *commonParams,
                                  int32_t layerIdx,
                                  int32_t memorySize[TIDL_LAYER_MEMORY_MAX])
{
  int32_t status = IALG_EOK;
  int32_t scratchDataSize;
  int32_t outDataSize;
  outDataSize = TIDL_refGetOutDataSize(commonParams, layerSpecificParams, layerIdx);
  memset(memorySize, 0, TIDL_LAYER_MEMORY_MAX * (int64_t)sizeof(int32_t));

  scratchDataSize = dcnParams->numOutChannels * 4;

  memorySize[TIDL_LAYER_MEMORY_SCRATCH] = scratchDataSize + 128;
  memorySize[TIDL_LAYER_MEMORY_OUTPUT] = outDataSize + TIDL_ALIGNMENT_SIZE;

  return status;
}
#endif
int32_t TIDL_deformConvAlloc(const TIDL_LayerSpecificParams *layerSpecificParams,
                             const TIDL_NetworkCommonParams *commonParams,
                             int32_t layerIdx,
                             int32_t memorySize[TIDL_LAYER_MEMORY_MAX])
{
  int32_t status = IALG_EOK;

#ifdef HOST_EMULATION
  const sTIDL_DeformConvParams_t *dcnParams = &commonParams->net->TIDLLayers[layerIdx].layerParams.deformConvParams;
  if (((uint32_t)commonParams->createParams->flowCtrl & TIDL_FLOW_CTRL_REF_ONLY) != 0U)
  {
    status = TIDL_deformConv2dRefAlloc(dcnParams, layerSpecificParams, commonParams, layerIdx, memorySize);
  }
  else
#endif
  {
    status = TIDL_deviceUtilsCommonAlloc(layerSpecificParams, commonParams, layerIdx, memorySize);
  }

  return status;
}

int32_t TIDL_deformConvInit(const TIDL_LayerSpecificParams *layerSpecificParams,
                            const TIDL_NetworkCommonParams *commonParams,
                            sTIDL_AlgLayer_t *algLayer,
                            int32_t layerIdx,
                            uint8_t *memory[TIDL_LAYER_MEMORY_MAX],
                            int32_t memorySize[TIDL_LAYER_MEMORY_MAX],
                            void **outPtr)
{
  int32_t status = IALG_EOK;
  const sTIDL_DeformConvParams_t *dcnParams = &commonParams->net->TIDLLayers[layerIdx].layerParams.deformConvParams;

#ifdef HOST_EMULATION
  if (((uint32_t)commonParams->createParams->flowCtrl & TIDL_FLOW_CTRL_REF_ONLY) != 0U)
  {
    status = TIDL_deformConv2dRefInit(dcnParams,
                                      layerSpecificParams,
                                      commonParams,
                                      algLayer,
                                      layerIdx,
                                      memory,
                                      memorySize,
                                      outPtr);
  }
  else
#endif
  {
    status = TIDL_deformConv2dDspInit(dcnParams,
                                      layerSpecificParams,
                                      commonParams,
                                      algLayer,
                                      layerIdx,
                                      memory,
                                      memorySize);
  }
  return status;
}
