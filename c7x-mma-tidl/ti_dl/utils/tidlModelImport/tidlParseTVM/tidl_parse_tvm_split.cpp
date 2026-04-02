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

template<> int32_t TidlParseTVM::parse<OpNameStr("split")>()
{
    int32_t axis = 0;
    int32_t numDim, itr_slice;
    sTIDL_SliceLayerParams_t &sliceParams = layer.layerParams.sliceParams;
    sTIDL_allowlistingMetaData md = layer.allowlistingMetaData;

    layer.layerType = TIDL_SliceLayer;
    sliceParams.stride = 1;
    
    // Get input dimensions
    if (layer.allowlistingMetaData.varTensorsDims.size() != 0)
    {
        numDim = layer.allowlistingMetaData.varTensorsDims[0].size();
        numDim = (numDim == 0) ? 4 : numDim;
    }
    else
    {
        numDim = 4;
    }
    layer.inData[0].numDim = numDim;
    layer.numOutBufs = md.numOutputs;
    
    auto attrs = call->attrs.as<SplitAttrs>();
    // Get attributes
    if (attrs != NULL)
    {
        // Get axis
        axis = attrs->axis;
    }

    auto attr_indices_or_sections = attrs->indices_or_sections;
    if (attrs != NULL && attrs->indices_or_sections.defined())
    {
        // Handle Variant<Box<int64_t>, Array<Box<int64_t>>>
        if (auto box_opt = attrs->indices_or_sections.as<tvm::runtime::Box<int64_t>>())
        {
            auto box_val = box_opt.value();
            int64_t sections = box_val->value;
        }
        else if (auto arr_opt = attrs->indices_or_sections.as<tvm::runtime::Array<tvm::runtime::Box<int64_t>>>())
        {
            auto arr = arr_opt.value();

            sliceParams.slicePoints[0] = 0;
            for (itr_slice = 0; itr_slice < arr.size(); ++itr_slice)
            {
                long long int val = (long long int)arr[itr_slice]->value;
                sliceParams.slicePoints[itr_slice + 1] = (int32_t)val;
            }
            for (int j = 0; j < layer.numOutBufs; j++)
            {
                layer.outData[j].elementType = tidl_getElementType(1);
            }
        }
    }
    else if (md.numConstInputs >= 1)
    {
        sBuffer_t split_buffer;
        int32_t numElements = 1;

        split_buffer = TIDL_extractConstantTensorData(call, 1, md, 0);

        long long int *split_ptr = (long long int *)split_buffer.ptr;

        layer.numOutBufs = split_buffer.bufSize;
        sliceParams.slicePoints[0] = 0;
        // TVM parses the slice point on the axis instead of the range( in onnx)
        for (itr_slice = 0; itr_slice < split_buffer.bufSize; itr_slice++)
        {
            sliceParams.slicePoints[itr_slice + 1] = sliceParams.slicePoints[itr_slice];
        }

        free(split_ptr);
        split_buffer.ptr = NULL;
        split_buffer.bufSize = 0;
    }
    else
    {
        TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Split layer : No split information provided");
        return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
    }

    sliceParams.slicePoints[itr_slice + 1] = md.varTensorsDims[0][axis];
    axis = TIDL_relayNormalizeAxis(axis, numDim);
    layer.layerParams.sliceParams.axis = (long long int)axis;

    return TIDL_ALLOWLISTING_LAYER_CHECK_PASSED;
}
