/*
*
* Copyright (c) {2025} Texas Instruments Incorporated
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
using namespace tvm;
#include <tvm/relay/function.h>
#include <tvm/relay/expr.h>
#include <tvm/relay/analysis.h>
#include <tvm/runtime/ndarray.h>

template <>
int32_t TidlParseTVM::parse<OpNameStr("tidl.topk")>()
{
  sTIDL_allowlistingMetaData md = layer.allowlistingMetaData;
  int32_t numDim = 0;
  int32_t k_value = -1;
  int32_t axis_value = -1;
  int32_t largest_value = 1;  // Default to largest (descending)
  int32_t sorted_value = 1;   // Default to sorted

  layer.layerType = TIDL_TopKLayer;
  layer.numInBufs = 1;
  layer.numOutBufs = md.numOutputs;

  // Get number of dimensions
  if (layer.allowlistingMetaData.varTensorsDims.size() != 0)
  {
    numDim = layer.allowlistingMetaData.varTensorsDims[0].size();
    numDim = (numDim == 0) ? 4 : numDim;
  }
  else
  {
    numDim = 4;
  }

  // Extract attributes from composite function
  const auto *fn_node = call->op.as<tvm::relay::FunctionNode>();
  if (fn_node == nullptr)
  {
    RelayDebugPrint("TopK: Unable to get FunctionNode for composite operator\n");
    return TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL;
  }

  auto fn = GetRef<tvm::relay::Function>(fn_node);
  const auto *attrs = fn->attrs.as<DictAttrsNode>();

  // Extract K value from attributes
  if (attrs && attrs->dict.count("k"))
  {
    k_value = Downcast<Integer>(attrs->dict.at("k"))->value;
  }
  // Try to extract K from constant input if not in attributes
  else if (md.numConstInputs > 0)
  {
    int32_t constTensorIdx = md.constTensorIndices[0];
    sBuffer_t constData = TIDL_extractConstantTensorData(call, constTensorIdx, md, 0);

    if (constData.ptr != nullptr)
    {
      // K value can be int64 or int32
      int64_t* k_ptr = (int64_t*)constData.ptr;
      k_value = (int32_t)(*k_ptr);
      
      if (constData.ptr != NULL)
      {
        free(constData.ptr);
        constData.ptr = NULL;
      }
    }
  }

  if (k_value == -1)
  {
    RelayDebugPrint("TopK: K value not found in attributes or constant inputs\n");
    return TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL;
  }

  layer.layerParams.topKParams.K = k_value;

  // Extract axis from attributes
  if (attrs && attrs->dict.count("axis"))
  {
    axis_value = Downcast<Integer>(attrs->dict.at("axis"))->value;
  }
  else
  {
    // Default axis is -1 (last dimension)
    axis_value = -1;
  }

  // Normalize axis to TIDL format
  layer.layerParams.topKParams.axis = TIDL_relayNormalizeAxis(axis_value, numDim);

  // Extract largest parameter (is_ascend in TVM)
  if (attrs && attrs->dict.count("is_ascend"))
  {
    int32_t is_ascend = Downcast<Integer>(attrs->dict.at("is_ascend"))->value;
    // is_ascend=false (0) means descending (largest values) -> largest=1
    // is_ascend=true (1) means ascending (smallest values) -> largest=0
    largest_value = is_ascend ? 0 : 1;
  }
  else if (attrs && attrs->dict.count("largest"))
  {
    largest_value = Downcast<Integer>(attrs->dict.at("largest"))->value;
  }

  layer.layerParams.topKParams.largest = largest_value;

  // Extract sorted parameter
  if (attrs && attrs->dict.count("sorted"))
  {
    sorted_value = Downcast<Integer>(attrs->dict.at("sorted"))->value;
  }

  layer.layerParams.topKParams.sorted = sorted_value;

  return TIDL_IMPORT_DIAGNOSIS_RETURN_OK;
}
