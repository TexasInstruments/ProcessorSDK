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
#ifndef TIDL_CUDA_
#define TIDL_CUDA_

#ifdef BUILD_WITH_CUDA
template <class Tin, class Tw, class Tb, class Tacc>
int TIDL_cudnnConvolve2d(
  Tin       *pInchanneli,
  Tw        *pCoeffsi,
  Tb        *pBias,
  Tacc      *accPtri,
  Tacc      *min,
  Tacc      *max,
  int32_t  numTotRoi,
  int32_t  numGroups,
  int32_t  numInChannels,
  int32_t  numOutChannels,
  int32_t  inChPitch,
  int32_t  outChPitch,
  int32_t  width,
  int32_t  height,
  int32_t  inImPitch,
  int32_t  outImPitch,
  int32_t  coeffsWidth,
  int32_t  coeffsHeight,
  int32_t  dilationWidth,
  int32_t  dilationHeight,
  int32_t  strideWidth,
  int32_t  strideHeight,
  int p_w,
  int p_h
);

template <class Tin, class Tw, class Tb, class Tacc>
int TIDL_cudaConvolve2d(
  Tin*     pInChannel,
  Tw*      pCoeffs,
  Tb*      pBias,
  Tacc *   accPtr,
  Tacc *   min,
  Tacc *   max,
  int32_t  numTotRoi,
  int32_t  numGroups,
  int32_t  numInChannels,
  int32_t  numOutChannels,
  int32_t  inChPitch,
  int32_t  outChPitch,
  int32_t  width,
  int32_t  height,
  int32_t  inImPitch,
  int32_t  outImPitch,
  int32_t  coeffsWidth,
  int32_t  coeffsHeight,
  int32_t  dilationWidth,
  int32_t  dilationHeight,
  int32_t  strideWidth,
  int32_t  strideHeight,
  int32_t enableBias,
  int32_t isOTFpad,
  int32_t leftPad,
  int32_t topPad,
  int32_t padVal,
  int32_t startRowNumberInTensor,
  int32_t inHeight,
  int32_t inWidth,
  int32_t outHeight,
  int32_t outWidth
  );
void TIDL_cudaSetInitFlag(int32_t layerIdx);
template <class Tin, class Tacc>
int TIDL_cudaEltsum(Tin* pIn, Tacc* pAcc, int scale, int numOutChannels, int height, int width, int outChPitch, int inChPitch, int inPitch, int outPitch, int maxcalls, int callno);
template <class Tx>
void minmaxGPU(const int numTotRoi, const Tx* x, int *min, int *max);
template <class Tx>
void minmax_Thrust(const int N,const Tx* x,Tx *min,Tx *max);
void TIDL_cudaFreeConvPtrs();
void TIDL_cudaFreeConvCudaPtrs();

void TIDL_cudaSetGridsampleInitFlag(int32_t layerIdx);
void TIDL_cudaFreeGridSampleCudaPtrs();

void TIDL_cudaSetInnerProductInitFlag(int32_t layerIdx);
void TIDL_cudaFreeInnerProductCudaPtrs();

void TIDL_cudaSetBatchNormInitFlag(int32_t layerIdx);
void TIDL_cudaFreeBatchNormCudaPtrs();

void TIDL_cudaSetEltwiseInitFlag(int32_t layerIdx);
void TIDL_cudaFreeEltwiseCudaPtrs();

void TIDL_cudaSetSliceInitFlag(int32_t layerIdx);
void TIDL_cudaFreeSliceCudaPtrs();

void TIDL_cudaSetTransposeInitFlag(int32_t layerIdx);
void TIDL_cudaFreeTransposeCudaPtrs();

void TIDL_cudaFreeAllCudaPtrs();

//void TIDL_cudaFreeEltPtrs();
void TIDL_freeMaxPtrs();
void TIDL_cudaFreeConcatPtrs();
void TIDL_minmaxElt(int *min, int*max, int numTotRoi);
void TIDL_minmaxCat(int *min, int*max, int numTotRoi);
template<class Tout>
int TIDL_cudaEltSatCpy(Tout* pOut, int numTotRoi, int numOutChannels, int height, int width, int outChPitch, int outPitch, int satLow, int satHigh, int roundBits);
template<class Tout,class Tacc>
int TIDL_cudaCatSatCopy(Tout* pOut, Tacc* pAcc, int numTotRoi, int numOutChannels, int height, int width, int outChPitch, int outPitch, int satLow, int satHigh, int roundBits, int maxcalls);
template <class Tout>
int TIDL_cudaSaturate(Tout **devPtrOf, int numTotRoi, int numOutChannels, int height, int width,int outChPitch, int outImPitch, int outRoundBits, int satLow, int satHigh);
template <class Tacc, class Tout>
int TIDL_cudaSaturateV1(Tout **devPtrOf, int N, int C, int H, int W, int outChPitch, int outImPitch, int outRoundBits, Tacc satLow, Tacc satHigh);
template <class Tacc, class Tout>
int TIDL_cudaSaturateFixedPoint(Tout **devPtrOf, int N, int C, int H, int W, int outChPitch, int outImPitch, uint8_t *outRoundBits, int32_t satLow, int32_t satHigh, int32_t enablePerChannelShift, int32_t precisionAdjustmentShift);
template <class Tacc, class Tout, class Tin>
int TIDL_cudaSaturateFixedPointAsym(Tout **devPtrOf, int N, int C, int H, int W, int outChPitch, int outImPitch, int32_t satLow, int32_t satHigh, uint8_t* pDerivedScales, uint8_t* pDerivedShifts, int32_t padFillValue);
template<class Tout>
int TIDL_cudaOutputTx(Tout *devPtrOf, int sizeOstream, int numTotRoi, int numOutChannels, int height, int width, int outChPitch, int outImPitch, int strideHeight, int strideWidth, Tout* pOutChanne);
template <class Tin> int
TIDL_cudaMaxpool( Tin *inData, Tin *outData, int numOutChannels, int height, int width, int kernelH, int kernelW, int inChPitch, int outChPitch, int inPitch, int outPitch, int initValue, int strideW, int strideH);
template<class Tin,class Tacc>
int TIDL_cudaConcatenate(Tin* pIn, Tacc* pAcc, int scale, int numOutChannels, int height, int width, int inChPitch, int outChPitch, int inPitch, int outPitch, int maxcalls, int callno, int ctotal, int axis);

// GridSample CUDA function declarations
template <class Tin, class Tgrid, class Tout>
int TIDL_cudaGridSampleFloat(
    const Tin* input,
    const Tgrid* grid, 
    Tout* output,
    int align_mode,
    bool align_corners,
    int numBatches, int numDim1, int numDim2, int numChannels, int inHeight, int inWidth,
    int outHeight, int outWidth,
    int inBatchPitch, int inDim1Pitch, int inDim2Pitch, int inChannelPitch, int inLinePitch,
    int gridBatchPitch, int gridDim1Pitch, int gridDim2Pitch, int gridChannelPitch, int gridLinePitch,
    int outBatchPitch, int outDim1Pitch, int outDim2Pitch, int outChannelPitch, int outLinePitch,
    int inPadH, int inPadW, int gridPadH, int gridPadW, int outPadH, int outPadW,
    int inTensorScale, int gridTensorScale, int outTensorScale
);

template <class Tin, class Tgrid, class Tacc, class TgridDeNorm, class TgridWeight, class Tout>
int TIDL_cudaGridSample(
    const Tin* input,
    const Tgrid* grid, 
    Tout* output,
    int align_mode,
    bool align_corners,
    int numBatches, int numDim1, int numDim2, int numChannels, int inHeight, int inWidth,
    int outHeight, int outWidth,
    int inBatchPitch, int inDim1Pitch, int inDim2Pitch, int inChannelPitch, int inLinePitch,
    int gridBatchPitch, int gridDim1Pitch, int gridDim2Pitch, int gridChannelPitch, int gridLinePitch,
    int outBatchPitch, int outDim1Pitch, int outDim2Pitch, int outChannelPitch, int outLinePitch,
    int inPadH, int inPadW, int gridPadH, int gridPadW, int outPadH, int outPadW,
    int inTensorScale, int gridTensorScale, int outTensorScale
);

template <class Tout, class Tacc>
int TIDL_cudaInnerProductSaturation(
    const Tacc* accPtr,
    Tout* outPtr,
    int32_t batches, int32_t dim1, int32_t dim2, int32_t channels, uint32_t numOutRows, uint32_t numOutCols,
    uint32_t outChPitch, uint32_t outLinePitch, uint32_t outDIM1Pitch, uint32_t outDIM2Pitch, uint32_t outBatchPitch,
    int32_t isFloat, int32_t isHighPrecision,int32_t roundBits, int32_t mmaScale,
    int32_t satLow, int32_t satHigh,
    const uint8_t* mmav2_Scales, const uint8_t* mmav2_Shifts
);

template <class Tin, class TBin, class Tw, class Tb, class Tout, class Tacc>
int TIDL_cudaInnerProduct(
    const Tin* inPtr,
    const TBin* inBPtr,
    Tacc* accPtr,
    const Tb* biasPtr,
    const Tw* weightsPtr,
    int32_t batches, int32_t dim1, int32_t dim2, int32_t channels,
    uint32_t numInCols, uint32_t numOutCols, uint32_t numInRows, uint32_t numOutRows,
    uint32_t inAChPitch, uint32_t inALinePitch, uint32_t inADIM1Pitch, uint32_t inADIM2Pitch, uint32_t inABatchPitch,
    uint32_t inBChPitch, uint32_t inBLinePitch, uint32_t inBDIM1Pitch, uint32_t inBDIM2Pitch, uint32_t inBBatchPitch,
    uint32_t outChPitch, uint32_t outLinePitch, uint32_t outDIM1Pitch, uint32_t outDIM2Pitch, uint32_t outBatchPitch,
    int32_t inputBTranspose,
    int32_t isBias,
    int32_t constIdx
);

// BatchNorm CUDA function declarations
template <class Tin, class Tw, class Tb, class Tacc>
int TIDL_cudaBatchNorm(
    const Tin* input,
    const Tw* weights,
    const Tw* slopes,
    const Tb* bias,
    Tacc* accumulator,
    int numTotBatches, int numChannels, int imWidth, int imHeight, int numDIM1, int numDIM2,
    int inBatchPitch, int inChPitch, int inDIM1Pitch, int inDIM2Pitch, int inPitch,
    int outBatchPitch, int outChPitch, int outDIM1Pitch, int outDIM2Pitch, int outPitch,
    int actType, float slopeScale
);

template <class Tacc, class Tout>
int TIDL_cudaBatchNormSaturation(
    const Tacc* accumulator,
    Tout* output,
    int numTotBatches, int numChannels, int imWidth, int imHeight, int numDIM1, int numDIM2,
    int outBatchPitch, int outChPitch, int outDIM1Pitch, int outDIM2Pitch, int outPitch,
    int outRoundBits, int satLow, int satHigh, int mixedPrecision, float floatSatLow, float floatSatHigh
);

// High Accuracy Sigmoid CUDA function declaration
template <class Tin, class Tout>
int TIDL_cudaHighAccuracySigmoid(
    const Tin* input,
    Tout* output,
    int numTotBatches, int numChannels, int imWidth, int imHeight, int inDim1, int inDim2,
    int inBatchPitch, int inChPitch, int inDim1Pitch, int inDim2Pitch, int inPitch,
    int outBatchPitch, int outChPitch, int outDim1Pitch, int outDim2Pitch, int outPitch,
    float inputScale, float outputScale
);

// 4-Point Approximation Sigmoid CUDA function declarations
template <class Tin, class Tout, class Tacc>
int TIDL_cudaSigmoid(
    const Tin* input,
    Tacc* accumulator,
    int numTotBatches, int numChannels, int imWidth, int imHeight,
    int inBatchPitch, int inChPitch, int inPitch,
    int outBatchPitch, int outChPitch, int outPitch,
    uint32_t threshold0, uint32_t threshold1, uint32_t threshold2, uint16_t inDataScale,
    Tout* slope, Tout* offset, Tout offsetScale
);

template <class Tacc, class Tout>
int TIDL_cudaSigmoidSaturation(
    const Tacc* accumulator,
    Tout* output,
    int numTotBatches, int numChannels, int imWidth, int imHeight,
    int outBatchPitch, int outChPitch, int outPitch,
    int outRoundBits, Tout satLow, Tout satHigh
);

template <class Tin, class Tout>
int TIDL_cudaFloatSigmoid(
    const Tin* input,
    Tout* output,
    int numTotBatches, int numChannels, int imWidth, int imHeight, int inDim1, int inDim2,
    int inBatchPitch, int inChPitch, int inDim1Pitch, int inDim2Pitch, int inPitch,
    int outBatchPitch, int outChPitch, int outDim1Pitch, int outDim2Pitch, int outPitch
);

// Non-Linear LUT CUDA function declaration
template <class Tin, class Tout>
int TIDL_cudaNonLinearLUT(
    const Tin* input,
    Tout* output,
    const Tout* lutTable,
    int numTotBatches, int numChannels, int imWidth, int imHeight, int inDim1, int inDim2,
    int inBatchPitch, int inChPitch, int inDim1Pitch, int inDim2Pitch, int inPitch,
    int outBatchPitch, int outChPitch, int outDim1Pitch, int outDim2Pitch, int outPitch,
    int readOffsetLUT
);

// Non-Linear Interpolation LUT CUDA function declaration
template <class Tin, class Tout>
int TIDL_cudaNonLinearInterpolLUT(
    const Tin* input,
    Tout* output,
    const Tout* lutTable,
    int numTotBatches, int numChannels, int imWidth, int imHeight, int inDim1, int inDim2,
    int inBatchPitch, int inChPitch, int inDim1Pitch, int inDim2Pitch, int inPitch,
    int outBatchPitch, int outChPitch, int outDim1Pitch, int outDim2Pitch, int outPitch,
    int32_t f1, int32_t f2, int32_t inoffset
);

template <class Tacc>
int TIDL_cudaEltWiseAllocateAccumulator(int numBatches, int outBatchPitch);


template <class Tin, class Tacc>
int TIDL_cudaEltWiseOp(
    const Tin* input,
    Tacc* accumulator,
    int32_t scale,
    int32_t zeropoint,
    int numDIM1, int numDIM2, int numChannels, int inHeight, int inWidth,
    int inDIM1Pitch, int inDIM2Pitch, int inChPitch, int inPitch, int pixelPitch,
    int outDIM1Pitch, int outDIM2Pitch, int outChPitch, int outPitch,
    int callno, int batchno, int eltWiseType, int outBatchPitch
);

template <class Tacc, class Tout>
int TIDL_cudaEltWiseMMAv2Quantize(
    const Tacc* accumulator,
    Tout* output,
    uint8_t mmaScale,
    uint8_t mmaShift,
    int32_t biasTerm,
    int numBatches, int numDIM1, int numDIM2, int numChannels, int inHeight, int inWidth,
    int outBatchPitch, int outDIM1Pitch, int outDIM2Pitch, int outChPitch, int outPitch
);

template <class Tacc, class Tout>
int TIDL_cudaEltWiseQuantize(
    const Tacc* accumulator,
    Tout* output,
    int numBatches, int numDIM1, int numDIM2, int numChannels, int inHeight, int inWidth,
    int outBatchPitch, int outDIM1Pitch, int outDIM2Pitch, int outChPitch, int outPitch,
    int roundBits, int32_t satLow, int32_t satHigh, int mixedPrecision,
    float floatSatLow, float floatSatHigh
);

template <class Tin, class Tout>
int TIDL_refCudaSlice(
  const Tin* pIn,
  Tout*  pOut,
  int32_t inPtrOffset, int32_t outPtrOffset,
  int32_t outWidth, int32_t outHeight,int32_t numChs,
  int32_t numDim1,int32_t numDim2,int32_t numROIs,
  int32_t inLinePitch,int32_t outLinePitch, int32_t inChPitch,
  int32_t outChPitch, int32_t inDim1Pitch, int32_t outDim1Pitch, int32_t inDim2Pitch,
  int32_t outDim2Pitch, int32_t inROIPitch, int32_t outROIPitch
);

template <class Tin, class Tout>
int TIDL_cudaTranspose(
    const Tin* input,
    Tout* output,
    int inBatches, int outBatches, int inDIM1, int inDIM2, int inChannels, int inHeight, int inWidth,
    int inBatchPitch, int inDIM1Pitch, int inDIM2Pitch, int inChPitch, int inPitch,
    int outBatchPitch, int outDIM1Pitch, int outDIM2Pitch, int outChPitch, int outPitch,
    int outDIM1, int outDIM2, int outChannels, int outHeight, int outWidth,
    int pp, int lp, int cp, int d1p, int d2p, int bp
);


extern int CUDNNLC;
#endif
#endif
