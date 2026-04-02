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
#include <tvm/relay/function.h>
#include <tvm/relay/attrs/nn.h>

int32_t poolParamHelper(sTIDL_LayerPC_t &layer,
                        Array<PrimExpr> kernel_shape, Array<PrimExpr> strides,
                        Array<PrimExpr> pads, Array<PrimExpr> dilations,
                        bool ceil_mode, int storage_order, string auto_pad)
{
  sTIDL_PoolingParams_t &poolParams = layer.layerParams.poolParams;
  sTIDL_PoolPCParams_t &poolPCParams = layer.layerPCParams.poolParams;

  layer.layerType = TIDL_PoolingLayer;
  poolParams.poolingType = TIDL_MaxPooling;

  poolParams.strideW = 1;
  poolParams.strideH = 1;
  poolParams.padW = 0;
  poolParams.padH = 0;
  poolParams.padT = 0;
  poolParams.padB = 0;
  poolParams.padL = 0;
  poolParams.padR = 0;
  poolParams.useCeil = ceil_mode;
  poolPCParams.dilationH = 1;
  poolPCParams.dilationW = 1;
  poolPCParams.storage_order = storage_order;

  if(dilations.size() == 2)
  {
    poolPCParams.dilationH = dilations[0].as<IntImmNode>()->value;
    poolPCParams.dilationW = dilations[1].as<IntImmNode>()->value;
  }

  if(kernel_shape.size() == 2)
  {
    poolParams.kernelH = kernel_shape[0].as<IntImmNode>()->value;
    poolParams.kernelW = kernel_shape[1].as<IntImmNode>()->value;
  }

  if(strides.size() == 2)
  {
    poolParams.strideH = strides[0].as<IntImmNode>()->value;
    poolParams.strideW = strides[1].as<IntImmNode>()->value;
  }

  string autoPad = auto_pad;

  if(autoPad == "NOTSET")
  {
    poolParams.padT = (pads.size() >= 1)? pads[0].as<IntImmNode>()->value: 0;
    poolParams.padL = (pads.size() >= 2)? pads[1].as<IntImmNode>()->value: 0;
    poolParams.padB = (pads.size() >= 3)? pads[2].as<IntImmNode>()->value: 0;
    poolParams.padR = (pads.size() >= 4)? pads[3].as<IntImmNode>()->value: 0;
    if((poolParams.padT < poolParams.padB) && (poolParams.padL < poolParams.padR)) //padL < padR && padT < padB
    {
      /* This condition will occur when stride = 2 and size of input tensor is even
        strideOffsetMethod is not handled in pooling layer, so take care of this in TIDL_tfOutReshapePoolingLayer */
      layer.strideOffsetMethod = TIDL_StrideOffsetCenter;
    }
  }
  else if((autoPad == "SAME_UPPER") || (autoPad == "SAME_LOWER"))
  {
    if(layer.allowlistingMetaData.varTensorsDims[0].size() >= 2)
    {
      int32_t heightIdx = layer.allowlistingMetaData.varTensorsDims[0].size()-2;
      int32_t widthIdx = layer.allowlistingMetaData.varTensorsDims[0].size()-1;
      // https://github.com/microsoft/onnxruntime/blob/0463aa9fc3ef02d30d7177c0065cd4b7d36a39f7/onnxruntime/core/providers/cpu/nn/pool_attributes.h#L155
      int32_t inputH = layer.allowlistingMetaData.varTensorsDims[0][heightIdx];
      int32_t inputW = layer.allowlistingMetaData.varTensorsDims[0][widthIdx];

      // legacy_targetH_size is output dimension which indicates how many pooling windows will fit along the height dimension when striding.
      int32_t legacy_targetH_size = (inputH + poolParams.strideH - 1) / poolParams.strideH;
      int32_t totalPadH = (legacy_targetH_size - 1) * poolParams.strideH + poolParams.kernelH - inputH;

      // legacy_targetW_size is output dimension which indicates how many pooling windows will fit along the width dimension when striding.
      int32_t legacy_targetW_size = (inputW + poolParams.strideW - 1) / poolParams.strideW;
      int32_t totalPadW = (legacy_targetW_size - 1) * poolParams.strideW + poolParams.kernelW - inputW;

      if(autoPad == "SAME_UPPER")
      {
        poolParams.padT = (totalPadH) / 2;
        poolParams.padB = totalPadH - poolParams.padT;
        poolParams.padL = (totalPadW) / 2;
        poolParams.padR = totalPadW - poolParams.padL;
        layer.strideOffsetMethod = TIDL_StrideOffsetCenter;
      }
      else  //SAME_LOWER
      {
        poolParams.padT = (totalPadH + 1) / 2;
        poolParams.padB = totalPadH - poolParams.padT;
        poolParams.padL = (totalPadW + 1) / 2;
        poolParams.padR = totalPadW - poolParams.padL;
        layer.strideOffsetMethod = TIDL_StrideOffsetTopLeft;
      }
    }
    else
    {
      if(poolParams.strideW > 1)
      {
        poolParams.padL = (poolParams.kernelW - 1) / 2;
      }
      else
      {
        poolParams.padL = poolParams.kernelW / 2;
      }
      if(poolParams.strideH > 1)
      {
        poolParams.padT = (poolParams.kernelH - 1) / 2;
      }
      else
      {
        poolParams.padT = poolParams.kernelH / 2;
      }

      poolParams.padB = poolParams.padT;
      poolParams.padR = poolParams.padL;

      if(autoPad == "SAME_UPPER")
      {
        layer.strideOffsetMethod = TIDL_StrideOffsetCenter;
      }
      else  //SAME_LOWER
      {
        layer.strideOffsetMethod = TIDL_StrideOffsetTopLeft;
      }
    }
  }
  else if(autoPad == "VALID")
  {
    poolParams.padT = 0;
    poolParams.padL = 0;
    poolParams.padB = 0;
    poolParams.padR = 0;
  }
  else
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : MaxPool : Unsupported auto_pad attribute %d", autoPad);
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }

  poolPCParams.originalPadT = poolParams.padT;
  poolPCParams.originalPadL = poolParams.padL;
  poolPCParams.originalPadB = poolParams.padB;
  poolPCParams.originalPadR = poolParams.padR;

  poolParams.padW = max(poolParams.padL, poolParams.padR);
  poolParams.padH = max(poolParams.padT, poolParams.padB);

  return TIDL_ALLOWLISTING_LAYER_CHECK_PASSED;
}

template <>
int32_t TidlParseTVM::parse<OpNameStr("tidl.max_pool2d")>()
{
  int32_t status = 0;

  const auto* fn_node = call->op.as<tvm::relay::FunctionNode>();
  auto fn = GetRef<tvm::relay::Function>(fn_node);
  const auto* attrs = fn->attrs.as<DictAttrsNode>();

  Array<PrimExpr> kernel_shape, strides, pads, dilations;
  string auto_pad;
  bool ceil_mode;
  int storage_order;
  if(attrs->dict.count("kernel_shape")){
    kernel_shape= Downcast<Array<PrimExpr>>(attrs->dict.at("kernel_shape"));
  }
  else
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : MaxPool : kernel_shape attribute is missing");
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }

  if (kernel_shape.size() < 2)
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : MaxPool : kernel_shape is missing height axis.");
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }
  if (kernel_shape.size() > 2)
  {
    for(int32_t i = 2; i < kernel_shape.size(); i++)
    {
      if (kernel_shape[i].as<IntImmNode>()->value != 1)
      {
        TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : MaxPool : kernel_shape is only supported along width and height axis.");
        return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
      }
    }
  }

  if(attrs->dict.count("strides"))
    strides = Downcast<Array<PrimExpr>>(attrs->dict.at("strides"));

  if(attrs->dict.count("pads"))
    pads = Downcast<Array<PrimExpr>>(attrs->dict.at("pads"));

  if(attrs->dict.count("dilations"))
    dilations = Downcast<Array<PrimExpr>>(attrs->dict.at("dilations"));

  if(attrs->dict.count("ceil_mode"))
    ceil_mode = Downcast<IntImm>(attrs->dict.at("ceil_mode"))->value;
  else
    ceil_mode = 0;

  if(attrs->dict.count("storage_order"))
    storage_order = Downcast<IntImm>(attrs->dict.at("storage_order"))->value;
  else
    storage_order = 0;

  if(attrs->dict.count("auto_pad"))
    auto_pad = Downcast<tvm::tir::StringImm>(attrs->dict.at("auto_pad"))->value;
  else
    auto_pad = "NOTSET";

  status = poolParamHelper(layer, kernel_shape, strides,
                             pads, dilations, ceil_mode,
                             storage_order, auto_pad);

  return status;
}