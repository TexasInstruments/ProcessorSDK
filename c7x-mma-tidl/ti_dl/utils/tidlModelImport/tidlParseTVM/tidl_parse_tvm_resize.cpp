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

#include <tvm/relay/attrs/image.h>
extern int32_t gloab_data_format;

template <>
int32_t TidlParseTVM::parse<OpNameStr("resize2d")>()
{
  sTIDL_ResizeLayerParams_t &resizeParams = layer.layerParams.resizeParams;
  sTIDL_ResizePCParams_t &resizePCParams = layer.layerPCParams.resizeParams;

  layer.layerType = TIDL_ResizeLayer;
  layer.numInBufs = 1;
  std::string mode, coordinate_transformation_mode, nearest_mode;

  /* initialize resizeRatio by 1, will change acc to type */
  for (int32_t i = 0; i < TIDL_DIM_MAX; i++)
  {
    resizeParams.resizeRatio[i] = 1.0;
  }
  auto attrs = call->attrs.as<Resize2DAttrs>();
  int new_height = attrs->size[0].as<IntImmNode>()->value;
  int new_width = attrs->size[1].as<IntImmNode>()->value;

  auto buf = call->args[0];
  Array<PrimExpr> buf_shape = buf->checked_type().as<TensorTypeNode>()->shape;
  int height_dim = gloab_data_format == 0 ? 1 : 2;
  int width_dim = gloab_data_format == 0 ? 2 : 3;
  int orig_height = buf_shape[height_dim].as<IntImmNode>()->value;
  int orig_width = buf_shape[width_dim].as<IntImmNode>()->value;

  resizeParams.resizeRatio[TIDL_DIM_HEIGHT] = (new_height * 1.0) / orig_height;
  resizeParams.resizeRatio[TIDL_DIM_WIDTH] = (new_width * 1.0) / orig_width;

  mode = attrs->method;

  resizeParams.mode = TIDL_ResizeNearest;
  if (mode == "nearest_neighbor")
    resizeParams.mode = TIDL_ResizeNearest;
  else if (mode == "linear")
    resizeParams.mode = TIDL_ResizeBilinear;
  else
    resizeParams.mode = -1;

  coordinate_transformation_mode = attrs->coordinate_transformation_mode;

  if (coordinate_transformation_mode == "half_pixel")
    resizePCParams.coordinate_transformation_mode = TIDL_Resize_half_pixel;
  else if (coordinate_transformation_mode == "pytorch_half_pixel")
    resizePCParams.coordinate_transformation_mode = TIDL_Resize_pytorch_half_pixel;
  else if (coordinate_transformation_mode == "align_corners")
    resizePCParams.coordinate_transformation_mode = TIDL_Resize_align_corners;
  else if (coordinate_transformation_mode == "asymmetric")
    resizePCParams.coordinate_transformation_mode = TIDL_Resize_asymmetric;
  else if (coordinate_transformation_mode == "tf_crop_and_resize")
    resizePCParams.coordinate_transformation_mode = TIDL_Resize_tf_crop_and_resize;
  else
    resizePCParams.coordinate_transformation_mode = -1;

  nearest_mode = attrs->rounding_method;
  if (nearest_mode == "round_prefer_floor")
    resizePCParams.nearest_mode = TIDL_Resize_round_prefer_floor;
  else if (nearest_mode == "round_prefer_ceil")
    resizePCParams.nearest_mode = TIDL_Resize_round_prefer_ceil;
  else if (nearest_mode == "floor")
    resizePCParams.nearest_mode = TIDL_Resize_floor;
  else if (nearest_mode == "ceil")
    resizePCParams.nearest_mode = TIDL_Resize_ceil;
  else
    resizePCParams.nearest_mode = -1;
  
  resizePCParams.cubic_coeff_a = attrs->cubic_alpha;
  resizePCParams.exclude_outside = attrs->cubic_exclude;
  resizePCParams.extrapolation_value = attrs->extrapolation_value;
  resizePCParams.antialias = 0; // TVM doesnt pass antialiasing

  resizePCParams.keep_aspect_ratio_policy = 0; // TVM doesnt pass keep_aspect_ratio_policy

  return 0;
}