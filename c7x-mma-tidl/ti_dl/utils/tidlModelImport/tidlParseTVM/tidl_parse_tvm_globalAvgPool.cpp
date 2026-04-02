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

template <>
int32_t TidlParseTVM::parse<OpNameStr("global_avg_pool2d")>()
{
  //   populating from relay map node
  layer.layerType = TIDL_PoolingLayer;
  sTIDL_PoolingParams_t &poolParams = layer.layerParams.poolParams;
  sTIDL_PoolPCParams_t &poolPCParams = layer.layerPCParams.poolParams;

  if (layer.allowlistingMetaData.varTensorsDims[0].size() != 0 && layer.allowlistingMetaData.varTensorsDims[0].size() != 4 &&
      layer.allowlistingMetaData.varTensorsDims[0].size() > 2)
  {
    /** Global average pool is different for non-image done
     * Ex: NxCxHxW ( image ) -> Global average pool is done over dims H, W
     * Ex: NxCxD1 (non image) -> Global average pool is done over D1
     * Ex: NxCxD1XD2xD3 (non image) -> Global average pool is done over D1, D2, D3
    */

    poolParams.avgDims = layer.allowlistingMetaData.varTensorsDims[0].size() - 2;
  }
  else
  {
    poolParams.avgDims = 2;
  }

  poolParams.poolingType = TIDL_AveragePooling;
  // Pool size is set to input layer size
  poolParams.kernelW = 0;
  poolParams.kernelH = 0;
  poolParams.padW = 0;
  poolParams.padH = 0;
  poolParams.padT = 0;
  poolParams.padB = 0;
  poolParams.padL = 0;
  poolParams.padR = 0;
  poolParams.strideW = 1;
  poolParams.strideH = 1;

  poolParams.useCeil = 0;
  poolPCParams.dilationH = 1;
  poolPCParams.dilationW = 1;

  poolPCParams.storage_order = 0;

  return 0;
}
