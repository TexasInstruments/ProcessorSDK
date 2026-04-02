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

template<> int32_t TidlParseTflite:: parse<kTfLiteBuiltinStridedSlice> ()
{
  int32_t status = TIDL_ALLOWLISTING_LAYER_CHECK_PASSED;

  /**************** Tensor property checks ****************/

  sTIDL_SliceLayerParams_t &sliceParams = layer.layerParams.sliceParams;

  const TfLiteStridedSliceParams* strided_slice_params =
          reinterpret_cast<const TfLiteStridedSliceParams*>(node->builtin_data);

  /**************** Map attributes ****************/

  int32_t beginMask = strided_slice_params->begin_mask;
  int32_t endMask = strided_slice_params->end_mask;
  int32_t ellipsisMask = strided_slice_params->ellipsis_mask;
  int32_t newAxisMask = strided_slice_params->new_axis_mask;
  int32_t shrinkAxisMask = strided_slice_params->shrink_axis_mask;

  /****************  Basic attribute checks **************/

  if((ellipsisMask != 0) || (newAxisMask != 0) || (shrinkAxisMask != 0))
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Slice : ellipsis_mask, new_axis_mask and shrink_axis_mask must be 0");
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }

  if((beginMask < 0) || (beginMask > 15) || (endMask < 0) || (endMask > 15)) /* since only 4 input dims are supported, mask can be only 4 bit - this is a sanity check */
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Slice : begin_mask/end_mask not in supported range - [0,15]");
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }

  /***************** Map inputs/parameters *****************/

  const TfLiteTensor* tensorBegin = &context->tensors[node->inputs->data[1]];
  const TfLiteTensor* tensorEnd = &context->tensors[node->inputs->data[2]];
  const TfLiteTensor* tensorStrides = &context->tensors[node->inputs->data[3]];

  int32_t * dataBegin = (int32_t *)malloc(tensorBegin->bytes);
  memcpy(dataBegin, (uint8_t *) tensorBegin->data.raw, tensorBegin->bytes);
  int32_t * dataEnd = (int32_t *)malloc(tensorEnd->bytes);
  memcpy(dataEnd, (uint8_t *) tensorEnd->data.raw, tensorEnd->bytes);
  int32_t * dataStrides = (int32_t *)malloc(tensorStrides->bytes);
  memcpy(dataStrides, (uint8_t *) tensorStrides->data.raw, tensorStrides->bytes);


  /**************** Mapped inputs/parameters related checks *************/
  for(int i = 0; i < 4; i++)
  {
    if(dataStrides[i] != 1)
    {
      TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Slice : only stride 1 slice is supported");
      return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
    }
  }

  /****************** Derived parameters *****************/

  /* Convert beginMask and endMask to binary bits */
  std::vector<int> beginMaskBits, endMaskBits;
  for(int i = 0; i < 4; i++) /* beginMask = 7 corresponds to 1110 in bits, 4 corresponds to 0010 - bits are reversed as per tflite interpretation */
  {
    beginMaskBits.push_back(beginMask % 2);
    beginMask /= 2;
    endMaskBits.push_back(endMask % 2);
    endMask /= 2;
  }

  const TfLiteTensor* outTensor = &context->tensors[node->outputs->data[0]];
  const TfLiteTensor* inTensor = &context->tensors[node->inputs->data[0]];
  std::vector<int32_t> outTensorDims, inTensorDims;
  for(int i = 0; i < 4; i++)  /* TIDL_checkSliceTensorProperties ensures both input_0 and output_0 have 4 dimensions */
  {
    outTensorDims.push_back(outTensor->dims->data[i]);
    inTensorDims.push_back(inTensor->dims->data[i]);
  }

  std::vector<int32_t> axes;
  for(int i = 1; i < 4; i++) /* Only batch size = 1 supported as checked in TIDL_checkSliceTensorProperties. Batch slicing not supported and batch bits not checked in tflite runtime implementation */
  {
    if(beginMaskBits[i] == 1) /* begin ignored for index i, set begin to 0 : no slicing at beginning */
    {
      dataBegin[i] = 0;
    }
    if(endMaskBits[i] == 1) /* end ignored for index, set end same as the corresponding input dimension : no slicing at end */
    {
      dataEnd[i] = inTensorDims[i];
    }
    if((dataBegin[i] != 0) || (dataEnd[i] != inTensorDims[i])) /* implies slicing happens in this axis */
    {
      axes.push_back(i);
    }
  }

  /****************** Check derived parameters *****************/
  if(axes.size() != 1)
  {
    TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Slice : Slicing is supported only across either of height, width or channel axis, not across multiple axes");
    return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
  }

  /*************** Mapping of attributes and inputs/parameters to TIDL layer *************/
  int32_t NhwcAxis = axes[0];
  int32_t NchwAxis;
  NchwAxis = NhwcAxis == 1 ? TIDL_DIM_HEIGHT :   // H
            NhwcAxis == 2 ? TIDL_DIM_WIDTH :   // W
            NhwcAxis == 3 ? TIDL_DIM_NUMCH :   // C
            NhwcAxis;

  layer.layerParams.sliceParams.axis = NchwAxis;
  layer.layerParams.sliceParams.stride = 1;
  layer.layerParams.sliceParams.slicePoints[0] = dataBegin[NhwcAxis];
  layer.layerParams.sliceParams.slicePoints[1] = dataEnd[NhwcAxis];

  layer.layerType = TIDL_SliceLayer;
  layer.numInBufs = 1;
  layer.numOutBufs = 1;

  if(dataBegin) free(dataBegin);
  if(dataEnd) free(dataEnd);
  if(dataStrides) free(dataStrides);

  return TIDL_ALLOWLISTING_LAYER_CHECK_PASSED;
}

