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



#include "tidl_parse_onnx.h"
using namespace std;
using namespace onnx;

template<> int32_t TidlParseOnnx:: parse<OnnxStr("BatchNormalization")> ()
{
  sBuffer_t gamma;
  sBuffer_t beta;
  sBuffer_t mean;
  sBuffer_t variance;
  float epsilon = 1e-05;
  int32_t dataSize;

  layer.layerType = TIDL_BatchNormLayer;
  NodeProto node = graph.node(index);
  int32_t status;

  status = copyFloatConst(graph, index, 1, gamma, INPUT_REQUIRED);
  if(status != 0)
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Batchnorm : Unable to find initializer at index %d for node %s", 1, graph.node(index).name().c_str());
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }
  status = copyFloatConst(graph, index, 2, beta, INPUT_REQUIRED);
  if(status != 0)
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Batchnorm : Unable to find initializer at index %d for node %s", 2, graph.node(index).name().c_str());
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }
  status = copyFloatConst(graph, index, 3, mean, INPUT_REQUIRED);
  if(status != 0)
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Batchnorm : Unable to find initializer at index %d for node %s", 3, graph.node(index).name().c_str());
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }
  status = copyFloatConst(graph, index, 4, variance, INPUT_REQUIRED);
  if(status != 0)
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Batchnorm : Unable to find initializer at index %d for node %s", 4, graph.node(index).name().c_str());
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }
  getFloatAttr(node, "epsilon", &epsilon, 0);

  /* ONNX Opset Version 18 introduces a new attribute called training mode and momentum in the Batch Normalization Operator.*/
  int32_t training_mode = 0;
  float momentum = 0.9;
  getIntAttr(node, "training_mode", &training_mode, 0);
  getFloatAttr(node, "momentum", &momentum, 0);
  layer.layerPCParams.batchNormParams.trainingMode = training_mode;
  layer.layerPCParams.batchNormParams.momentum = momentum;

  dataSize = gamma.bufSize;
  layer.weights.ptr = my_malloc(dataSize*sizeof(float));
  layer.weights.bufSize = dataSize;
  layer.bias.ptr = my_malloc(dataSize*sizeof(float));
  layer.bias.bufSize = dataSize;


  TIDL_tfBNToScaleBias((float *)layer.weights.ptr,
    (float *)layer.bias.ptr, dataSize,
    (float *)mean.ptr, (float *)variance.ptr, (float *)gamma.ptr, (float *)beta.ptr, epsilon
    );
  my_free(mean.ptr);
  my_free(variance.ptr);
  my_free(gamma.ptr);
  my_free(beta.ptr);

  return 0;
}

