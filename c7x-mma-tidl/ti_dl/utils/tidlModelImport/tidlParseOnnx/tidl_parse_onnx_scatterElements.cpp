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


template<> int32_t TidlParseOnnx:: parse<OnnxStr("ScatterElements")> ()
{
  int32_t numDim;
  layer.layerType = TIDL_ScatterElementsLayer;
  TensorProto indexTensor = onnx::TensorProto::default_instance();
  bool indexTensorFound = false;
  layer.numInBufs = graph.node(index).input_size();
  sTIDL_allowlistingMetaData md = layer.allowlistingMetaData;
  int32_t axis_status = getIntAttr(graph.node(index), "axis",   &layer.layerParams.scatterElementsParams.axis, 0);

  if(axis_status == TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL)
  {
    layer.layerParams.scatterElementsParams.axis = 0;
  }

  int32_t status, status2;

  /** If shape inference is not done on model, we assume the tensor is of 4 dimensions by default*/
  if (layer.allowlistingMetaData.varTensorsDims.size() != 0)
  {
    numDim = layer.allowlistingMetaData.varTensorsDims[0].size();
    numDim = (numDim == 0)? 4:numDim;
  }
  else
  {
    numDim= 4;
  }

  /** Adjust axis to TIDL dimensions*/
  if(layer.layerParams.scatterElementsParams.axis >= 0)
  {
    /*+ve Axis*/
    layer.layerParams.scatterElementsParams.axis += (TIDL_DIM_MAX - numDim);
  }
  else
  {
    /*-ve Axis*/
    layer.layerParams.scatterElementsParams.axis += (TIDL_DIM_MAX);
  }

  strcpy((char*)layer.layerParams.scatterElementsParams.dataTensorName, (char*)graph.node(index).input(0).c_str());
  strcpy((char*)layer.layerParams.scatterElementsParams.indicesTensorName, (char*)graph.node(index).input(1).c_str());
  strcpy((char*)layer.layerParams.scatterElementsParams.updateTensorName, (char*)graph.node(index).input(2).c_str());
 
 
  /* New attribute called reduction introduced. */
  char reduction[50];
  int32_t reduction_status = -1;

  /** The reduction attribute can have the following values:
   * "none". "add", "mul", "max", "min".
   * Only none value(also the default value) supported by TIDL_RT.
  */
  reduction_status = getStringAttr(graph.node(index), "reduction", reduction, 0);
if((reduction_status == TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL)||(strcmp(reduction, "none") == 0))
  {
    layer.layerParams.scatterElementsParams.reduction = TIDL_ScatterElementsNone;
  }
  else if(strcmp(reduction, "add") == 0)
  {
    layer.layerParams.scatterElementsParams.reduction = TIDL_ScatterElementsAdd;
  }
  else if(strcmp(reduction, "max") == 0)
  {
    layer.layerParams.scatterElementsParams.reduction = TIDL_ScatterElementsMax;
  }
  else if(strcmp(reduction, "min") == 0)
  {
     layer.layerParams.scatterElementsParams.reduction =  TIDL_ScatterElementsMin;
  }
  else 
  {
    layer.layerParams.scatterElementsParams.reduction =  TIDL_ScatterElementsUnsupported;
  }

  if(md.numConstInputs > 0)
  {
    if (md.numConstInputs == 1)
    { 
      status = copyFloatConst(graph, index, md.constTensorIndices[0], layer.weights, INPUT_REQUIRED);
      if(status == TIDL_ALLOWLISTING_LAYER_CHECK_FAILED)
      {
        TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Cannot read initializer tensor : Only float, int32 and int64 tensor is supported");
        return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
      }
      // Update tensor data type from int64 to int32 for indices tensor only
      if(md.constTensorIndices[0] == 1)
      {
        indexTensor = getInitializerTensor(graph, graph.node(index).input(1), index, status);
        if(status == TIDL_ALLOWLISTING_LAYER_CHECK_PASSED)
        {
          indexTensorFound = true;
        }
      }
    }
    else if (md.numConstInputs == 2)
    {
      status = copyFloatConst(graph, index, md.constTensorIndices[0], layer.weights, INPUT_REQUIRED);
      status2 = copyFloatConst(graph, index, md.constTensorIndices[1], layer.bias, INPUT_REQUIRED);
      if((status == TIDL_ALLOWLISTING_LAYER_CHECK_FAILED) || (status2 == TIDL_ALLOWLISTING_LAYER_CHECK_FAILED))
      {
        TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Cannot read initializer tensor : Only float, int32 and int64 tensor is supported");
        return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
      }
      // Update tensor data type from int64 to int32 for indices tensor only
      if((md.constTensorIndices[0] == 1) || (md.constTensorIndices[1] == 1))
      {
        indexTensor = getInitializerTensor(graph, graph.node(index).input(1), index, status);
        if(status == TIDL_ALLOWLISTING_LAYER_CHECK_PASSED)
        {
          indexTensorFound = true;
        }
      }
    }
    int32_t result =-1;
    if (indexTensorFound && indexTensor.data_type() == TensorProto_DataType_INT64)
    { 
       if(layer.allowlistingMetaData.constTensorIndices[0]==1)
        {
            result = convertIndicesToInt32(layer.weights);
        }
        else
        {
            result = convertIndicesToInt32(layer.bias);
        }
       
        return result;
        
    }
  }
  return 0;
}

