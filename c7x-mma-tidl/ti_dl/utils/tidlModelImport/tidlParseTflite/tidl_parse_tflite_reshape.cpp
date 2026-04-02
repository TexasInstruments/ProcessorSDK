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

template<> int32_t TidlParseTflite:: parse<kTfLiteBuiltinReshape> ()
{
  int32_t status = 0;

  /** Do not map any parameters before checking the tensor properties, however, layer type needs to be mapped to TIDL layer type for cases like tflite OD with
   * detection post process layer where this layer is marked supported irrespective of allowlisting pass, but needs to be mapped to TIDL layer for
   * further network optimizations like converting to TIDL detection output layer
   */
  layer.layerType = TIDL_ReshapeLayer;
  sTIDL_allowlistingMetaData md = layer.allowlistingMetaData;

  TfLiteTensor * tensor = &context->tensors[node->inputs->data[1]];
  int32_t numDims = tensor->dims->data[0];
  layer.weights.ptr = my_malloc(numDims*sizeof(int32_t));
  layer.weights.bufSize = numDims;
  int32_t * shape = (int32_t *)layer.weights.ptr;
  int32_t * reshapeDims = (int32_t *)tensor->data.i32;

  for(int i = 0; i < numDims; i++)
  {
    shape[i] = reshapeDims[i];
  }

  /**
  * Handling -1 in the shape attribute
  */
  if (status != TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL)
  {
    int64_t inputTotalVol = 1;
    int64_t shapeTotalVol = 1;

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

  //tflite input will be in NHWC format, change accordingly
  if (numDims >= 3 && numDims <= 6)
  {
    int32_t dimOffset = TIDL_DIM_MAX - numDims;
    int32_t ch = shape[TIDL_DIM_WIDTH - dimOffset];
    shape[TIDL_DIM_WIDTH - dimOffset]   = shape[TIDL_DIM_HEIGHT - dimOffset];
    shape[TIDL_DIM_HEIGHT - dimOffset]  = shape[TIDL_DIM_NUMCH - dimOffset];
    shape[TIDL_DIM_NUMCH - dimOffset] = ch;
  }

  layer.layerPCParams.reshapeParams.allowZero = 0; // default
  return TIDL_ALLOWLISTING_LAYER_CHECK_PASSED;
}

