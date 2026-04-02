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

template<> int32_t TidlParseTflite:: parse<kTfLiteBuiltinDequantize> ()
{
  auto input_tensor = &context->tensors[node->inputs->data[0]];
  auto output_tensor = &context->tensors[node->outputs->data[0]];
  /* Only map to dataConvert if it layer is from uint8/int8 -> float*/
  //TODO:Figure out the condition to switch between the identity and data convert
  if (((input_tensor->type == kTfLiteUInt8) || (input_tensor->type == kTfLiteInt8)) && ((output_tensor->type == kTfLiteFloat32)))
  {
    layer.layerType = TIDL_DataConvertLayer;
    layer.layerParams.dataConvertParams.type = TIDL_DC_TYPE_OUTPUT;
    layer.outData[0].elementType = TIDL_SinglePrecFloat;
    layer.outData[0].tensorScale = 1;
    /*TODO: check for any other case.Assuming dequantisation supports only 1 input and 1 output*/
    TfLiteAffineQuantization* quantization = (TfLiteAffineQuantization*)output_tensor->quantization.params;
    layer.layerParams.dataConvertParams.outZeroPoint = output_tensor->params.zero_point;
    quantization = (TfLiteAffineQuantization*)input_tensor->quantization.params;
    layer.layerParams.dataConvertParams.inZeroPoint = quantization->zero_point->data[0];
    return 0;
  }

  layer.layerType = TIDL_DequantizeLayer;
  auto tensor = &context->tensors[node->inputs->data[0]];
  layer.layerPCParams.dequantParams.scale = 0.1;
  layer.layerPCParams.dequantParams.zeroPoint = 0.0;
  layer.layerPCParams.dequantParams.axis = 1;

  if (((tensor->type == kTfLiteUInt8) || (tensor->type == kTfLiteInt8)) && (gParams.addDataConvertToNet))
  {
    TfLiteAffineQuantization* quantization = (TfLiteAffineQuantization*)tensor->quantization.params;
    if(quantization)
    {
        auto * scale = quantization->scale;
        auto *zero_point = quantization->zero_point;
        if (scale && zero_point)
        {
          layer.layerPCParams.dequantParams.scale     = scale->data[0];
          layer.layerPCParams.dequantParams.zeroPoint = zero_point->data[0];
        }
    }
  }
  return 0;
}

