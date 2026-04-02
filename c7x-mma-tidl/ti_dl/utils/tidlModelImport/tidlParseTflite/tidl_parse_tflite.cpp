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

#include <string>
#include "tidl_parse_tflite.h"
#include "tidl_import_common.h"
#include "tidl_import_config.h"
#include "tensorflow/lite/context_util.h"
#include "tensorflow/lite/builtin_ops.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/minimal_logging.h"
#include "tidl_parse_tflite_jumptable.h"
#include "tidl_custom_import.h"

TidlParseTflite::TidlParseTflite(const TfLiteRegistration* registration, const TfLiteNode* node, TfLiteContext* context, sTIDL_LayerPC_t layerPC) : 
    registration(registration), node(node), context(context), layer(layerPC)
{
  status = -1;
  // set layer defaults
  layer.numInBufs  = 1;
  layer.numOutBufs = 1;

  saveAllowlistingMetaData();
  
  auto jumpTableEntry = tidlParseTfliteJumpTable.find(registration->builtin_code);
  auto operator_codes = (tflite::BuiltinOperator)registration->builtin_code;

  if (gParams.enableCustomLayers)
  {
    if(layer.weights.ptr == NULL)
    {   
      status = TIDL_mapCustomParams_v2(&layer,
                                    -1,
                                    context,
                                    node,
                                    gParams.modelType);
    }
  }

  if ((status != 0) && jumpTableEntry != tidlParseTfliteJumpTable.end())
  {
    status = (this->*(jumpTableEntry->second))(); // Call the parse function

    if (status == -1)
    {
      layer.parseStatus = TIDL_ParseFailed;
    }
    else
    {
      layer.parseStatus = TIDL_ParsePassed;
    }
  }
  
  else if(status == -1)
  {
    layer.layerType = TIDL_UnsupportedLayer;
    status = TIDL_ALLOWLISTING_LAYER_TYPE_UNSUPPORTED;
  }
}

/*
 * Function to save layer level tensor properties metadata to be used for allowlisting
 * Saves data for sTIDL_allowlistingMetaData structure
 */
void TidlParseTflite::saveAllowlistingMetaData()
{
  layer.allowlistingMetaData.numInputs = node->inputs->size;
  layer.allowlistingMetaData.numOutputs = node->outputs->size;
  layer.allowlistingMetaData.numValidInputs = numValidInputs(node, layer.allowlistingMetaData.validInputIndices);
  layer.allowlistingMetaData.numVarInputs = numVariableLayerInputs(context, node, layer.allowlistingMetaData.varTensorIndices, layer.allowlistingMetaData.validInputIndices);
  layer.allowlistingMetaData.numConstInputs = layer.allowlistingMetaData.numValidInputs - layer.allowlistingMetaData.numVarInputs;

  //Find indices of constant tensors -- indices which are not variable are const
  for(int j = 0; j < layer.allowlistingMetaData.validInputIndices.size(); j++)
  {
    if(std::find(layer.allowlistingMetaData.varTensorIndices.begin(), layer.allowlistingMetaData.varTensorIndices.end(), layer.allowlistingMetaData.validInputIndices[j])
         == layer.allowlistingMetaData.varTensorIndices.end()) //idx not found in variable tensors indices => constant tensor idx
    {
      layer.allowlistingMetaData.constTensorIndices.push_back(layer.allowlistingMetaData.validInputIndices[j]);
    }
  }

  //save dimensions of variable and constant tensors
  for(auto& varIdx : layer.allowlistingMetaData.varTensorIndices)
  {
    const TfLiteTensor* varTensor = &context->tensors[node->inputs->data[varIdx]];
    std::vector<int32_t> tensorDim;
    for(int i = 0; i < varTensor->dims->size; i++)
    {
      tensorDim.push_back(varTensor->dims->data[i]);
    }
    layer.allowlistingMetaData.varTensorsDims.push_back(tensorDim);
  }
  for(auto& constIdx : layer.allowlistingMetaData.constTensorIndices)
  {
    const TfLiteTensor* constTensor = &context->tensors[node->inputs->data[constIdx]];
    std::vector<int32_t> tensorDim;
    for(int i = 0; i < constTensor->dims->size; i++)
    {
      tensorDim.push_back(constTensor->dims->data[i]);
    }
    layer.allowlistingMetaData.constTensorsDims.push_back(tensorDim);
  }
  for(int outputIdx=0; outputIdx<layer.allowlistingMetaData.numOutputs; outputIdx++)
  {
    const TfLiteTensor* outputTensor = &context->tensors[node->outputs->data[outputIdx]];
    std::vector<int32_t> tensorDim;
    for(int i = 0; i < outputTensor->dims->size; i++)
    {
      tensorDim.push_back(outputTensor->dims->data[i]);
    }
    layer.allowlistingMetaData.outputTensorDims.push_back(tensorDim);
  }

  const TfLiteTensor* outTensor = &context->tensors[node->outputs->data[0]];
  std::vector<int32_t> tensorDim;
  for(int i = 0; i < outTensor->dims->size; i++)
  {
    tensorDim.push_back(outTensor->dims->data[i]);
  }
  layer.allowlistingMetaData.outputTensorDims.push_back(tensorDim);

  //convert NHWC to NCHW dimensions for compatibility with ONNX RT checks
  for(int i = 0; i < layer.allowlistingMetaData.varTensorsDims.size(); i++)
  {
    if(layer.allowlistingMetaData.varTensorsDims[i].size() == 4)
    {
      std::vector<int32_t> tempDims = layer.allowlistingMetaData.varTensorsDims[i];
      layer.allowlistingMetaData.varTensorsDims[i][2] = tempDims[1];
      layer.allowlistingMetaData.varTensorsDims[i][3] = tempDims[2];
      layer.allowlistingMetaData.varTensorsDims[i][1] = tempDims[3];
    }
  }
  for(int i = 0; i < layer.allowlistingMetaData.constTensorsDims.size(); i++)
  {
    /* Conversion of hwc to chw needs to be supported,
    anything beyond 3 dims can be converted */
    if(layer.allowlistingMetaData.constTensorsDims[i].size() >= 3)
    {
      std::vector<int32_t> tempDims = layer.allowlistingMetaData.constTensorsDims[i];
      layer.allowlistingMetaData.constTensorsDims[i][2] = tempDims[1];
      layer.allowlistingMetaData.constTensorsDims[i][3] = tempDims[2];
      layer.allowlistingMetaData.constTensorsDims[i][1] = tempDims[3];
    }
  }
  for(int i = 0; i < layer.allowlistingMetaData.outputTensorDims.size(); i++)
  {
    if(layer.allowlistingMetaData.outputTensorDims[i].size() == 4)
    {
      std::vector<int32_t> tempDims = layer.allowlistingMetaData.outputTensorDims[i];
      layer.allowlistingMetaData.outputTensorDims[i][2] = tempDims[1];
      layer.allowlistingMetaData.outputTensorDims[i][3] = tempDims[2];
      layer.allowlistingMetaData.outputTensorDims[i][1] = tempDims[3];
    }
  }
}