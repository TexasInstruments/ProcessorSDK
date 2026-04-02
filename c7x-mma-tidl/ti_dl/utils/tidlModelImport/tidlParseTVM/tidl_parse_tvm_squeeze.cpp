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
#include <tvm/relay/attrs/transform.h>

template<> int32_t TidlParseTVM::parse<OpNameStr("squeeze")>()
{
  layer.layerType = TIDL_SqueezeLayer;
  layer.numInBufs = 1; 
  int32_t j, i, numDim = 0;
  int32_t num_axes_4_squeeze = -1;
  int32_t axes[TIDL_DIM_MAX]; 
  auto attrs = call->attrs.as<SqueezeAttrs>();
  string index_type;
  sTIDL_allowlistingMetaData md = layer.allowlistingMetaData;
  if(attrs!=NULL)
  {
        auto axisList = attrs->axis;   
        for(i=0;i<axisList.size();i++)
        {
           axes[i] =(int32_t) axisList[i].as<IntImmNode>()->value;
        }
        num_axes_4_squeeze=axisList.size();
       
  }
  else if(md.numInputs == 2 && md.numConstInputs == 1)
  {    
    sBuffer_t buf;
    if(call->args[1].as<tvm::relay::ConstantNode>())
    {   
      index_type = DLDataType2String(call->args[1]->checked_type().as<TensorTypeNode>()->dtype);
      int32_t numIndices= 1;
      for(int i = 0 ; i < layer.allowlistingMetaData.constTensorsDims[0].size() ; i++)
      {
          numIndices *= layer.allowlistingMetaData.constTensorsDims[0][i];
      }
      num_axes_4_squeeze=numIndices;
      auto indicesExpr = call->args[1].as<tvm::relay::ConstantNode>()->data;
      buf=TIDL_extractConstantTensorData(call,1,md,0);
      if(index_type=="int64")
      {
          for(int  i=0;i<numIndices;i++){
            axes[i]=(int32_t)(*((int64_t*)buf.ptr + i));
          }
      }
      else
      {
          for(int  i=0;i<numIndices;i++){
            axes[i]=(int32_t)(*((int32_t*)buf.ptr + i));
          }
      }
      
      free (buf.ptr);
    }
  }
  else if (md.numInputs == 1)
  {
    int axes_size = 0;
    // If axes is not provided, all the single dimensions will be removed from the shape 
    for (int i = 0; i < md.varTensorsDims[0].size(); i++)
    {
      if(md.varTensorsDims[0][i] == 1){
        axes[axes_size] = i;
        axes_size += 1;
      }
      num_axes_4_squeeze = axes_size;
    }
  }
  else
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Squeeze layer : Variable input for axes is not supported");
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }

  /** If shape inference is not done on model, we assume the tensor is of 4 dimensions by default*/
  if (layer.allowlistingMetaData.varTensorsDims.size() != 0)
  {
    numDim = layer.allowlistingMetaData.varTensorsDims[0].size();
    numDim = (numDim == 0)? 4:numDim;
  }
  else
  {
    numDim = 4;
  }
  if(layer.allowlistingMetaData.varTensorsDims[0].size() != 0)
  {
    for (i=0 ; i < num_axes_4_squeeze; i++)
    {
      int32_t axisToSqueeze = TIDL_relayNormalizeAxis(axes[i],numDim,nullptr,1);
      if(md.varTensorsDims[0][axisToSqueeze] != 1)
      {
        TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Squeeze layer : Input Dimension across given axes should be 1");
        return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
      }
    }
  }
  
  for (i = 0; i < num_axes_4_squeeze; i++)
  {
    axes[i] = TIDL_relayNormalizeAxis(axes[i],numDim);
  }

  for(i = 0; i< TIDL_DIM_MAX; i++)
  {
    layer.layerPCParams.squeezeParams.axis[i] = 0;
  }

  for(i = 0; i< TIDL_DIM_MAX; i++)
  {
    for(j=0;j<num_axes_4_squeeze;j++)
    {
      if(i == axes[j])
      {
        layer.layerPCParams.squeezeParams.axis[i] = 1; // squeeze this particular axis
      }
    }
  }
return 0;

}