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

#include <tvm/relay/attrs/nn.h>
#include<tvm/relay/expr.h>


template<> int32_t TidlParseTVM::parse<OpNameStr("batch_norm")> ()
{
  layer.layerType = TIDL_BatchNormLayer;
  auto epsilon = 1e-05;
  if (layer.allowlistingMetaData.numConstInputs == 4  )
  {
    int32_t numEle = 1;
    for(int i = 0 ; i < layer.allowlistingMetaData.constTensorsDims[0].size() ; i++)
    {
      numEle *= layer.allowlistingMetaData.constTensorsDims[0][i];
    }


    int params_size = sizeof(float)*numEle;

    layer.weights.ptr     = my_malloc(params_size);
    layer.weights.bufSize = numEle;
    layer.bias.ptr        = my_malloc(params_size);
    layer.bias.bufSize    = numEle;

    if( call->attrs.as<BatchNormAttrs>())
    {
      auto params = call->attrs.as<BatchNormAttrs>();
      epsilon = params->epsilon;
    }
    layer.layerPCParams.batchNormParams.trainingMode = 0;
    layer.layerPCParams.batchNormParams.momentum = 0.9;

    float* gamma = NDArrtoFloat(call->args[1].as<tvm::relay::ConstantNode>()->data,
                                  numEle);
    float* beta  = NDArrtoFloat(call->args[2].as<tvm::relay::ConstantNode>()->data,
                                  numEle);
    float* mean  = NDArrtoFloat(call->args[3].as<tvm::relay::ConstantNode>()->data,
                                  numEle);
    float* var   = NDArrtoFloat(call->args[4].as<tvm::relay::ConstantNode>()->data,
                                  numEle);

    // Calculate scale and bias for weights/bias
    TIDL_tfBNToScaleBias((float *)layer.weights.ptr, (float *)layer.bias.ptr,
                        numEle, mean, var,
                        gamma, beta, epsilon);

    my_free(gamma);
    my_free(beta);
    my_free(mean);
    my_free(var);

  }
  return 0;
}
