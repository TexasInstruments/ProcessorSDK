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

template<> int32_t TidlParseTflite:: parse<kTfLiteBuiltinAveragePool2d> ()
{
  int32_t status = 0;

  TfLiteTensor * tensor = &context->tensors[node->inputs->data[0]];

  layer.layerType = TIDL_PoolingLayer;
  sTIDL_PoolingParams_t &poolParams = layer.layerParams.poolParams;
  sTIDL_PoolPCParams_t &poolPCParams = layer.layerPCParams.poolParams;
  poolParams.useCeil = 0;  // TODO : check the useCeil values for tflite

  // Convert avgpool2d to global pooling if the kernel size matches the HxW dimensions.
  // This happens in TIDL_tfOutReshapePoolingLayer but we need it prior to calling the model
  // checker for allow/deny.

  const TfLitePoolParams* Pool2DParams =
        reinterpret_cast<const TfLitePoolParams*>(node->builtin_data);
  uint32_t kernelW = Pool2DParams->filter_width;
  uint32_t kernelH = Pool2DParams->filter_height;
  tensor = &context->tensors[node->inputs->data[0]];
  int  dims = tensor->dims->size;
  if (dims != 4) return -1;
  //NHWC format
  uint32_t inH = tensor->dims->data[1];
  uint32_t inW = tensor->dims->data[2];
  if (kernelH == inH && kernelW == inW) //global avg pool
  {
    poolParams.poolingType = TIDL_AveragePooling;
    // Pool size is set to input layer size
    poolParams.kernelH = 0;
    poolParams.kernelW = 0;
    poolParams.padH    = 0;
    poolParams.padW    = 0;
    poolParams.padT    = 0;
    poolParams.padB    = 0;
    poolParams.padL    = 0;
    poolParams.padR    = 0;
    poolPCParams.originalPadT = 0;
    poolPCParams.originalPadB = 0;
    poolPCParams.originalPadL = 0;
    poolPCParams.originalPadR = 0;
    poolParams.strideH = 1;
    poolParams.strideW = 1;
    poolPCParams.dilationH = 1;
    poolPCParams.dilationW = 1;
  }
  else
  {
    status = this->parse<kTfLiteBuiltinMaxPool2d>();
    poolParams.poolingType = TIDL_AveragePooling;
    // Convert 1x1 average pooling into max pooling (operations are identical)
    // mxnet_resnet50_v1d
    if (poolParams.kernelH == 1 && poolParams.kernelW == 1 &&
       ((poolParams.strideH == 2 && poolParams.strideW == 2) ||
        (poolParams.strideH == 1 && poolParams.strideW == 1)))
    {
      poolParams.poolingType = TIDL_MaxPooling;
    }
  }
  return status;
}

