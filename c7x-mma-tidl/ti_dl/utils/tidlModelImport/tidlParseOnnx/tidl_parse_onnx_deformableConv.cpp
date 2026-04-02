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



#include "tidl_parse_onnx.h"
using namespace std;
using namespace onnx;

template<> int32_t TidlParseOnnx:: parse<OnnxStr("DeformConv")> ()
{
	/*Populate:*/
	int32_t status;
	int32_t padType;
	int32_t pads[4] = {0};
	int32_t padL=-1, padR=-1, padT=-1, padB=-1;
	sTIDL_DeformConvParams_t &deformConvParams = layer.layerParams.deformConvParams;
	layer.layerType = TIDL_DeformableConvLayer;
	layer.strideOffsetMethod = TIDL_StrideOffsetTopLeft;
	if (graph.node(index).input_size() == 5)
	{
		/*Mask buffer exists:*/
		layer.numInBufs = 3;
	}
	else
	{
		/*No Mask buf:*/
		layer.numInBufs = 2;
	}

	/*Default to signed:*/
	layer.outData[0].elementType = tidl_getElementType(1);

	/*Read initializers for weights and biases:*/
	TensorProto tensor;
	/*Find the weight tensor:*/
	tensor = getInitializerTensor(graph, graph.node(index).input(1), index, status);
	if(status != 0)
	{
		TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Deformable Conv layer : Unable to find initializer at index %d for node %s", 1, graph.node(index).name().c_str());
		return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
	}
	deformConvParams.numOutChannels = getTensorDim(tensor, 0);
  	deformConvParams.numInChannels  = getTensorDim(tensor, 1);
  	deformConvParams.kernelH   = getTensorDim(tensor, 2);
  	deformConvParams.kernelW    = getTensorDim(tensor, 3);
	deformConvParams.numGroups = 1;
	deformConvParams.dilationW = 1;
	deformConvParams.dilationH = 1;
	deformConvParams.strideW = 1;
	deformConvParams.strideH = 1;
	deformConvParams.padW = 0;
	deformConvParams.padH = 0;
	deformConvParams.enableBias = 0;


	NodeProto node = graph.node(index);
	getIntAttr(node, "strides",   &deformConvParams.strideH, 0);
	getIntAttr(node, "strides",   &deformConvParams.strideW, 1);	
	getIntAttr(node, "dilations", &deformConvParams.dilationH, 0);
	getIntAttr(node, "dilations", &deformConvParams.dilationW, 1);
	getIntAttr(node, "group",     &deformConvParams.numGroups, 0);
	getIntAttr(node, "offset_group",     &deformConvParams.numOffsetGroup, 0);
	
  	status = getIntAttr(node, "kernel_shape", &layer.layerPCParams.deformConvParams.tensorHeight, 0);
	if (status == -1)
	{
		layer.layerPCParams.convParams.tensorHeight = deformConvParams.kernelH;
	}
	
	status = getIntAttr(node, "kernel_shape", &layer.layerPCParams.deformConvParams.tensorWidth, 1);
	if (status == -1)
	{
		layer.layerPCParams.convParams.tensorWidth = deformConvParams.kernelW;
	}

	/*DeformableConvolution is explicit about padding values and is directly read from the node:*/
	getIntAttr(node, "pads",       &pads[0], 0);
	getIntAttr(node, "pads",       &pads[1], 1);
	getIntAttr(node, "pads",       &pads[2], 2);
	getIntAttr(node, "pads",       &pads[3], 3);
	if((pads[0] < pads[2]) && (pads[1] < pads[3])) //padL < padR && padT < padB
	{
	layer.strideOffsetMethod = TIDL_StrideOffsetCenter;
	}
	pads[2] = max(pads[0], pads[2]);
	pads[3] = max(pads[1], pads[3]);
	padL = pads[1];
	padR = pads[3];
	padT = pads[0];
	padB = pads[2];

	deformConvParams.padW = pads[3];
	deformConvParams.padH = pads[2];
	deformConvParams.padT = padT;
	deformConvParams.padL = padL;
	deformConvParams.padB = padB;
	deformConvParams.padR = padR;

	/*Copy weight buffer:*/
	status = copyFloatConst(graph, index, 1, layer.weights, INPUT_REQUIRED);
	if(status == TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL)
	{
		TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : DeformableConv layer : Unable to copy initializer at index %d for node %s", 1, graph.node(index).name().c_str());
		return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
	}

	/*Copy the bias buffer: (Note, the condition is not correct if optional mask is not specified)*/
	if (graph.node(index).input_size() == 5)
	{
		deformConvParams.enableBias = 1;
		status = copyFloatConst(graph, index, 3, layer.bias, INPUT_REQUIRED);
		if(status == TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL)
		{
		TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : DeformableConv layer  bias: Unable to copy initializer at index %d for node %s", 2, graph.node(index).name().c_str());
		return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
		}
	}
	return 0;
}

