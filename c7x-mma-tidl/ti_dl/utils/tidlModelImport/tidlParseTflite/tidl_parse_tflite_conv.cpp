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

#include "tidl_parse_tflite.h"
using namespace std;

template<> int32_t TidlParseTflite:: parse<kTfLiteBuiltinConv2d> ()
{
  int32_t status;
  int32_t padType;

  sTIDL_ConvParams_t &convParams = layer.layerParams.convParams;

  layer.layerType = TIDL_ConvolutionLayer;

  layer.outData[0].elementType = tidl_getElementType(1);

  TfLiteTensor* output;
  output = &context->tensors[node->outputs->data[0]];
  const TfLiteTensor* input;
  input = &context->tensors[node->inputs->data[0]];
  const TfLiteTensor* filter;
  filter = &context->tensors[node->inputs->data[1]];

  // OHWI layout for tflite conv filter
  convParams.numInChannels   = filter->dims->data[3];
  convParams.numOutChannels  = filter->dims->data[0];
  layer.layerPCParams.convParams.tensorWidth     = filter->dims->data[2];
  layer.layerPCParams.convParams.tensorHeight    = filter->dims->data[1];
  
  // No separate parameter in TfLite for kernel shape
  convParams.kernelW = filter->dims->data[2];
  convParams.kernelH = filter->dims->data[1];

  convParams.numGroups       = 1;
  convParams.dilationW       = 1;
  convParams.dilationH       = 1;
  convParams.strideW         = 1;
  convParams.strideH         = 1;
  convParams.padW            = 0;
  convParams.padH            = 0;
  convParams.padL            = 0;
  convParams.padR            = 0;
  convParams.padT            = 0;
  convParams.padB            = 0;
  convParams.enableBias      = 0;
  convParams.enablePooling   = 0;

  const TfLiteConvParams* conv_params =
          reinterpret_cast<const TfLiteConvParams*>(node->builtin_data);

  convParams.strideW = conv_params->stride_width;
  convParams.strideH = conv_params->stride_height;
  convParams.dilationW = conv_params->dilation_width_factor;
  convParams.dilationH = conv_params->dilation_height_factor;

  fillActParams(layer.actParams,  conv_params->activation);

  padType = conv_params->padding;
  layer.strideOffsetMethod = TIDL_StrideOffsetCenter;
  if (padType == kTfLitePaddingSame)   // SAME : Padding done to input
  {
    convParams.padW = ((convParams.kernelW - 1)*convParams.dilationW) / 2;
    convParams.padH = ((convParams.kernelH - 1)*convParams.dilationH) / 2;
    convParams.padL = ((convParams.kernelW - 1)*convParams.dilationW) / 2;
    convParams.padT = ((convParams.kernelH - 1)*convParams.dilationH) / 2;
    convParams.padR = ((convParams.kernelW - 1)*convParams.dilationW) / 2;
    convParams.padB = ((convParams.kernelH - 1)*convParams.dilationH) / 2;
  }
  else if (padType == kTfLitePaddingValid)  // VALID : No padding to input
  {
    layer.strideOffsetMethod = TIDL_StrideOffsetTopLeft;
    convParams.padL            = 0;
    convParams.padR            = 0;
    convParams.padT            = 0;
    convParams.padB            = 0;
  }

  status = copyInputConstTensor(registration, node, context, 1, layer.weights, layer.weightScales, layer.weightZeroPoints);
  if(status == TIDL_ALLOWLISTING_LAYER_CHECK_FAILED) return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  if(gParams.quantizationStyle != TIDL_QuantStyleAsymNP2_TFL)
  {
    tidlParseTfliteKernelReshape((float *)layer.weights.ptr, convParams.kernelW, convParams.kernelH, convParams.numInChannels, convParams.numOutChannels);
    dequantTensor(layer.weights, layer.weightScales, layer.weightZeroPoints, convParams.kernelW * convParams.kernelH * convParams.numInChannels);
  }
  else
  {
    tidlParseTfliteKernelReshape((int8_t *)layer.weights.ptr, convParams.kernelW, convParams.kernelH, convParams.numInChannels, convParams.numOutChannels);
  }

  if (node->inputs->size == 3)
  {
    status = copyInputConstTensor(registration, node, context, 2, layer.bias, layer.biasScales, layer.biasZeroPoints);
    if(status == TIDL_ALLOWLISTING_LAYER_CHECK_FAILED) return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
    if(gParams.quantizationStyle != TIDL_QuantStyleAsymNP2_TFL)
    {
      dequantTensor(layer.bias, layer.biasScales, layer.biasZeroPoints, 1);
    }
    convParams.enableBias = 1;
  }
  return TIDL_ALLOWLISTING_LAYER_CHECK_PASSED;
}

