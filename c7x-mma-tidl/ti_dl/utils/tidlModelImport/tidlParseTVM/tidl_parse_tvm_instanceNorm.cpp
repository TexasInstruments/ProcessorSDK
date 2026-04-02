/*
 *
 * Copyright (c) {2015 - 2025} Texas Instruments Incorporated
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

#include "tidl_parse_tvm.h"
using namespace std;

#include <tvm/relay/attrs/nn.h>


template<> int32_t TidlParseTVM::parse<OpNameStr("instance_norm")> ()
{
  // implementation for only ONNX model as TFLite front end doesn't support instance_norm
  int32_t status = 0;
  if(TIDL_isSupportedInFirmwareVersion((const char*)gParams.c7xFirmwareVersion, "10_00_04_00"))
  {
    float32_tidl epsilon;
    layer.layerType = TIDL_InstanceNormLayer;
    layer.numInBufs = 1;

    sTIDL_allowlistingMetaData md = layer.allowlistingMetaData;

    /* instanceNorm operation is done per channel (C):
        * Number of channels (C) in image input: [N, C, H, W]
        * C in non-image input: [N, C, D1, D2 ... Dn]
     * So equilant layerNorm axis will be 1
     */
    layer.layerParams.layerNormParams.axis = 1;
    /* Convert to TIDL convention */
    int32_t numDims = md.varTensorsDims[0].size();
    numDims = (numDims == 0) ? 4 : numDims;
    layer.layerParams.layerNormParams.axis += (TIDL_DIM_MAX - numDims);

    if (call->attrs.as<InstanceNormAttrs>() != NULL)
    {
        auto attrs = call->attrs.as<InstanceNormAttrs>();
        layer.layerParams.layerNormParams.epsilon = attrs->epsilon;
    }
    
    if(layer.allowlistingMetaData.numConstInputs>0)
    {
      int32_t numScale= 1;
      for(int i = 0 ; i < layer.allowlistingMetaData.constTensorsDims[0].size() ; i++)
      {
        numScale *= layer.allowlistingMetaData.constTensorsDims[0][i];
      }
      auto constExpr = call->args[1].as<tvm::relay::ConstantNode>()->data;
      layer.weights.ptr = NDArrtoFloat(constExpr, numScale);
      layer.weights.bufSize = numScale;
    }

    if(layer.allowlistingMetaData.numConstInputs>1)
    {
      int32_t numCenter = 1;
      for(int i = 0 ; i < layer.allowlistingMetaData.constTensorsDims[1].size() ; i++)
      {
        numCenter *= layer.allowlistingMetaData.constTensorsDims[1][i];
      }
      auto constExpr = call->args[2].as<tvm::relay::ConstantNode>()->data;
      layer.bias.ptr = NDArrtoFloat(constExpr, numCenter);
      layer.bias.bufSize = numCenter;
    }
    

  }
  else
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : InstanceNormalization is not supported with the current c7x_firmware_version");
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }

  return status;
    
}


