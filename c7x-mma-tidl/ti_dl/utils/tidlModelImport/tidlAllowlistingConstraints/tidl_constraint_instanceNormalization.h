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

#include "tidl_constraint.h"

const vector<TidlConstraint> tidlConstraintInstanceNormalization = 
{
    TIDL_CSTR(
        "Number of non-singleton variable input dimensions must be <= 6",
        "Number of non-singleton variable input dimensions must be <= 6",
        "",
        [](const sTIDL_LayerPC_t *layer, string &logs){
            ostringstream oss;
            int32_t numDims = tidlGetNonSingletonNumDims(layer->allowlistingMetaData.varTensorsDims[0]);
            if(numDims > 6)
            {
                oss << "Maximum number of input dimension supported is 6, found " << numDims << " input dimensions";
                logs = oss.str();
                return false;
            }
            return true;
        }
    ),
    TIDL_CSTR(
        "Number of variable input dimensions must be >= 3",
        "Number of variable input dimensions must be >= 3",
        "",
        [](const sTIDL_LayerPC_t *layer, string &logs){
            ostringstream oss;
            sTIDL_allowlistingMetaData md = layer->allowlistingMetaData;
            if(md.varTensorsDims[0].size() < 3)
            {
                oss << "Number of input dimension must be >=3, found " << md.varTensorsDims[0].size() << " input dimensions";
                logs = oss.str();
                return false;
            }
            return true;
        }
    ),
    TIDL_CSTR(
        "Scale and Bias should be 1-D tensor of size same as input channel dimension",
        "Scale and Bias should be 1-D tensor of size same as input channel dimension",
        "",
        [](const sTIDL_LayerPC_t *layer, string &logs){
            ostringstream oss;
            sTIDL_allowlistingMetaData md = layer->allowlistingMetaData;

            vector<int32_t> scaleDims = {};
            vector<int32_t> biasDims = {};
            if(md.numVarInputs > 1)
            {
                /* Scale is 2nd input and bias is 3rd input */
                if(md.varTensorIndices[1] == 1)
                {
                    scaleDims = md.varTensorsDims[1];
                }
                else if(md.varTensorIndices[1] == 2)
                {
                    biasDims = md.varTensorsDims[1];
                }

                if(md.numVarInputs > 2)
                {
                    if(md.varTensorIndices[2] == 1)
                    {
                        scaleDims = md.varTensorsDims[2];
                    }
                    else if(md.varTensorIndices[2] == 2)
                    {
                        biasDims = md.varTensorsDims[2];
                    }
                }
            }

            if(md.numConstInputs >= 1)
            {
                /* Scale is 2nd input and bias is 3rd input */
                if(md.constTensorIndices[0] == 1)
                {
                    scaleDims = md.constTensorsDims[0];
                }
                else if(md.constTensorIndices[0] == 2)
                {
                    biasDims = md.constTensorsDims[0];
                }

                if(md.numConstInputs >= 2)
                {
                    if(md.constTensorIndices[1] == 1)
                    {
                        scaleDims = md.constTensorsDims[1];
                    }
                    else if(md.constTensorIndices[1] == 2)
                    {
                        biasDims = md.constTensorsDims[1];
                    }
                }
            }

            /* Scale should be 1D tensor and size same:
             * Number of channels (C) in image input: [N, C, H, W]
             * C in non-image input: [N, C, D1, D2 ... Dn]
             */
            if(scaleDims.size() != 1 || scaleDims[0] != md.varTensorsDims[0][1])
            {
                return false;
            }

            /* Bias is given */
            if(biasDims.size() != 0)
            {
                if(biasDims.size() != 1 || biasDims[0] != md.varTensorsDims[0][1])
                {
                    return false;
                }
            }

            return true;
        }
    )
};