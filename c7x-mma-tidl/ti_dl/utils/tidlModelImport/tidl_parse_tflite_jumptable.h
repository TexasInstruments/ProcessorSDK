/*
*
* Copyright (c) {2015 - 2024} Texas Instruments Incorporated
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

#ifndef TIDL_PARSE_TFLITE_JUMPTABLE_H_
#define TIDL_PARSE_TFLITE_JUMPTABLE_H_

#include <unordered_map>
#include "tensorflow/lite/builtin_ops.h"


#ifdef TIDL_GEN_DOC_
// When generating documentation, avoid function pointers and just map to the TIDL Layer type
    #define TidlParseTfliteJTEntry pair<int32_t, string>
    #define Tflite_JTE(tfliteLayer, layerType) {tfliteLayer, {layerType, #tfliteLayer}}
    
#else
// When parsing, map to the parse function
    #include "tidlParseTflite/tidl_parse_tflite.h"
    #define TidlParseTfliteJTEntry int32_t (TidlParseTflite::*)()
    #define Tflite_JTE(tfliteLayer, layerType) {tfliteLayer, &TidlParseTflite::parse<tfliteLayer>}
#endif /*TIDL_GEN_DOC_*/



/*
*   TfLite Jump Table
*   Maps Tflite Operators to:
*   1. The TIDL Layer to which they are parsed
*   2. The Parse function
*/
unordered_map<int32_t, TidlParseTfliteJTEntry> tidlParseTfliteJumpTable =
{
    Tflite_JTE( kTfLiteBuiltinConv2d,                   TIDL_ConvolutionLayer),
    Tflite_JTE( kTfLiteBuiltinTransposeConv,            TIDL_Deconv2DLayer),
    Tflite_JTE( kTfLiteBuiltinDepthwiseConv2d,          TIDL_ConvolutionLayer),
    Tflite_JTE( kTfLiteBuiltinAdd,                      TIDL_EltWiseLayer),
    Tflite_JTE( kTfLiteBuiltinSub,                      TIDL_EltWiseLayer),
    Tflite_JTE( kTfLiteBuiltinMul,                      TIDL_EltWiseLayer),
    Tflite_JTE( kTfLiteBuiltinDiv,                      TIDL_EltWiseLayer),
    Tflite_JTE( kTfLiteBuiltinSoftmax,                  TIDL_SoftMaxLayer),
    Tflite_JTE( kTfLiteBuiltinMean,                     TIDL_PoolingLayer),
    Tflite_JTE( kTfLiteBuiltinMaxPool2d,                TIDL_PoolingLayer),
    Tflite_JTE( kTfLiteBuiltinAveragePool2d,            TIDL_PoolingLayer),
    Tflite_JTE( kTfLiteBuiltinQuantize,                 TIDL_DataConvertLayer),
    Tflite_JTE( kTfLiteBuiltinSqueeze,                  TIDL_SqueezeLayer),
    Tflite_JTE( kTfLiteBuiltinTanh,                     TIDL_TanhLayer),
    Tflite_JTE( kTfLiteBuiltinElu,                      TIDL_ELULayer),
    Tflite_JTE( kTfLiteBuiltinArgMax,                   TIDL_ArgOpLayer),
    Tflite_JTE( kTfLiteBuiltinDequantize,               TIDL_DataConvertLayer),
    Tflite_JTE( kTfLiteBuiltinLogistic,                 TIDL_SigmoidLayer),
    Tflite_JTE( kTfLiteBuiltinStridedSlice,             TIDL_SliceLayer),
    Tflite_JTE( kTfLiteBuiltinReshape,                  TIDL_ReshapeLayer),
    Tflite_JTE( kTfLiteBuiltinResizeBilinear,           TIDL_ResizeLayer),
    Tflite_JTE( kTfLiteBuiltinResizeNearestNeighbor,    TIDL_ResizeLayer),
    Tflite_JTE( kTfLiteBuiltinConcatenation,            TIDL_ConcatLayer),
    Tflite_JTE( kTfLiteBuiltinTranspose,                TIDL_TransposeLayer),
    Tflite_JTE( kTfLiteBuiltinFullyConnected,           TIDL_InnerProductLayer),
    Tflite_JTE( kTfLiteBuiltinPad,                      TIDL_PadLayer),
    Tflite_JTE( kTfLiteBuiltinRelu,                     TIDL_ReLULayer),
    Tflite_JTE( kTfLiteBuiltinPrelu,                    TIDL_ReLULayer),
    Tflite_JTE( kTfLiteBuiltinLeakyRelu,                TIDL_LeakyReluLayer),
    Tflite_JTE( kTfLiteBuiltinCast,                     TIDL_CastLayer),
    Tflite_JTE( kTfLiteBuiltinBatchToSpaceNd,           TIDL_BatchToSpaceLayer),
    Tflite_JTE( kTfLiteBuiltinSpaceToBatchNd,           TIDL_SpaceToBatchLayer),
    Tflite_JTE( kTfLiteBuiltinPack,                     TIDL_PackLayer),
    Tflite_JTE( kTfLiteBuiltinDepthToSpace,             TIDL_DepthToSpaceLayer),
    Tflite_JTE( kTfLiteBuiltinSpaceToDepth,             TIDL_SpaceToDepthLayer),
};


#endif /*TIDL_PARSE_TFLITE_JUMPTABLE_H_ */