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

template<>
int32_t TidlParseTVM:: parse<OpNameStr("reshape")> ()
{
  int32_t status = 0;
  auto attrs = call->attrs.as<ReshapeAttrs>();
  int32_t * shape;
  sTIDL_allowlistingMetaData md = layer.allowlistingMetaData;

  layer.layerType = TIDL_ReshapeLayer;
  layer.weights.ptr = my_malloc(attrs->newshape.size()*sizeof(int32_t));
  layer.weights.bufSize = attrs->newshape.size();
  shape = (int32_t *)layer.weights.ptr;
  for(int i=0; i < attrs->newshape.size(); i++)
  {
    shape[i] = (int32_t)attrs->newshape[i]->value;
  }

  status = checkShapeInferenceforOnnx(md);
  
  /**
  * Handling 0 in the shape attribute
  */
  if ((status != TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL) && (attrs->allowzero == 0))
  {
    shape = (int32_t*)layer.weights.ptr;
    for (int32_t i = 0; i < layer.weights.bufSize; i++)
    {
      if (shape[i] == 0)
      {
        if (i < md.varTensorsDims[0].size())
        {
          shape[i] = md.varTensorsDims[0][i];
        }
        else
        {
          shape[i] = 1;
        }
      }
    }
  }

  /**
  * Handling -1 in the shape attribute
  */
  if (status != TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL)
  {
    shape = (int32_t*)layer.weights.ptr;
    int32_t inputTotalVol = 1;
    int32_t shapeTotalVol = 1;

    int32_t negIdx = -1;
    for (int32_t i = 0; i < layer.weights.bufSize; i++)
    {
      if (shape[i] == -1)
      {
        negIdx = i;
      }
      else
      {
        shapeTotalVol = shapeTotalVol * shape[i];
      }
    }

    // Found -1 at negIdx
    if(negIdx != -1)
    {
      for (int32_t i = 0; i < md.varTensorsDims[0].size(); i++)
      {
        inputTotalVol = inputTotalVol * md.varTensorsDims[0][i];
      }
      shape[negIdx] = inputTotalVol/shapeTotalVol;
    }
  }

  layer.layerPCParams.reshapeParams.allowZero = attrs->allowzero;

  return 0;
}

