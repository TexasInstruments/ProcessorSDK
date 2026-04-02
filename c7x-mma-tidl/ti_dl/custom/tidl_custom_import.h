/*
 *
 * Copyright (c) {2015 - 2020} Texas Instruments Incorporated
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
 *        No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 *        any redistribution and use are licensed by TI for use only with TI Devices.
 *
 *        Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 *        any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 *        any redistribution and use of any object code compiled from the source code
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

/**
 *  \file tidl_custom_import.h
 *
 *  \brief This file defines the custom interfaces needed for importing
 *         custom layer
 */

#ifndef TIDL_CUSTOM_IMPORT_H_
#define TIDL_CUSTOM_IMPORT_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */



#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <float.h>

#include <itidl_ti.h>
#include "ti_dl.h"
#include "tidl_import_config.h"
#include "onnx/onnx-ml.proto3.pb.h"

#include "tidl_custom.h"

#define TIDL_CUSTOM_LAYER_API_v2 (1) /* To use Custom Layer API before release 11.0, rename this as TIDL_CUSTOM_LAYER_API_v1 and clean build. */
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t TIDL_tfOutReshapeCustomLayer(sTIDL_OrgNetwork_t *pOrgTIDLNetStructure,
                                     int32_t layerIndex);

#ifdef TIDL_CUSTOM_LAYER_API_v1
/*
    A simplified new API TIDL_mapCustomParams_v2 is defined. encourage to use TIDL_mapCustomParams_v2 API.
    However for backward copatability, existing API (TIDL_MapCustomParams and TIDL_MapCustomParamsOnnx) is maintained
    High level structure of these APIs are as below
    
    int32_t TIDL_MapCustomParamsOnnx(sTIDL_LayerPC_t &TIDLPCLayers, ....)
    {
        ----- Pre-Code ----- 
        
        int32_t status = TIDL_MapCustomParams_v2(sTIDL_LayerPC_t &layer, ....);
        
        ----- Post-Code -----
    }
    
    int32_t TIDL_MapCustomParams(sTIDL_OrgNetwork_t *pOrgTIDLNetStructure, ....)
    {
        ---- Pre-Code ----- 
        
        int32_t status = TIDL_MapCustomParams_v2(sTIDL_LayerPC_t &layer, ....);
        
        ----- Post-Code -----
    }
*/
int32_t TIDL_MapCustomParamsOnnx (sTIDL_LayerPC_t &TIDLPCLayers,
                                  int32_t nodeIndex,
                                  onnx::GraphProto* onnxGraph);
/**
 *  \brief This function initialized the dataflow handle.
 *
 *  \param  pOrgTIDLNetStructure  [IN] Pointer to the TIDL net structure
 *  \param  nodeIndex[]           [IN] Index of the node in original network
 *  \param  layerIndex            [IN] Layer index in TIDL net structure
 *  \param  dataIndex             [IN] Data Id for the layer
 *  \param  parserObj1            [IN] Pointer to parse the model format. Depending on
 *                                     modelType user should typecase this
 *  \param  parserObj2            [IN] Pointer to the parameters of the network. Applicable
 *                                     only for caffe model format
 *  \param  modelType             [IN] Model type : Refer to Model Format from tidl_import_api.h file.                                 
 *                                     |TIDL_MODEL_IMPORT_FORMAT_CAFFE      | 0 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_TENSORFLOW | 1 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_ONNX       | 2 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_TFLITE     | 3 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_TVM        | 4 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_TFLITE_RT  | 5 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_ONNX_RT    | 6 |            
 *
 *  \return Error Status
 */
int32_t TIDL_MapCustomParams(sTIDL_OrgNetwork_t   *pOrgTIDLNetStructure,
                               int32_t              nodeIndex,
                               int32_t              layerIndex,
                               int32_t              *dataIndex,
                               const void           *parserObj1,
                               const void           *parserObj2,
                               int32_t               modelType);

#endif
/**
 *  \brief This function initialized the dataflow handle.
 *
 *  \param  layer                 [IN] Pointer to the TIDL layer.
 *  \param  nodeIndex[]           [IN] Index of the node in original network
 *  \param  parserObj1            [IN] Pointer to parse the model format. Depending on
 *                                     modelType user should typecase this
 *  \param  parserObj2            [IN] Reserved pointer for the parameters of the network.
 *  \param  modelType             [IN] Model type : Refer to Model Format from tidl_import_api.h file.                                 
 *                                     |TIDL_MODEL_IMPORT_FORMAT_CAFFE      | 0 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_TENSORFLOW | 1 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_ONNX       | 2 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_TFLITE     | 3 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_TVM        | 4 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_TFLITE_RT  | 5 |
 *                                     |TIDL_MODEL_IMPORT_FORMAT_ONNX_RT    | 6 | 
 *  \return Error Status
 */
int32_t TIDL_mapCustomParams_v2(sTIDL_LayerPC_t*       layer,
                               int32_t               nodeindex, 
                               const void           *parserObjGraph,
                               const void           *parserObjReserved,
                               int32_t               modelType);

/**
 *  \brief This function is to get the output tensor scale for a given input tensor scale based on
 *  activation ranges (min and max value) of a given layer.
 *
 *  Following are the steps user can follow for this :
 * 1) As TIDL-RT currently supports only symmetric quantization, hence find absolute minimum of
 *    the output of current layer using minTensorValue and  maxTensorValue. Lets call this as absMaxValue.
 * 2) Compute the output scale of this particular layer using input tensor scale and scale used for
 *     any other parameters involved in computation of this layer. Lets call this as accumulator Scale.
 * 3) Compute the maximum quantized value by multiplying accumultator scale by absMaxValue.
 * 4) Determine the number of bits required to store this absolute maximum value.  maxNumBits
 *     maxNumBits = (int32_t)ceil(log((float64_tidl)absMaxValue) / log((float64_tidl)2));
 * 5) if maxNumBits is greater than quantized bit depth 8-bit/16-bit) of the output of current layer then
 *    apply right shift on the output to bring back the output in 8-bit/16-bit container. Lets call this
 *    as roundBits. For example if maxNumBits is  12  then for 8 bit quantization , roundBits will be (12 - 8) = 4 bits.
 *    User is expected to apply this right shift by roundBits while computing the output of a given layer. TIDL stores
 *    this roundBits as part of tidlLayer->outData[0].roundBits, user can also use the same variable.
 * 6) Update the output tensor scale to account for the right shift as follows :
 *     outTensorScale = accumulatorScale / ( 1 << roundBits ).
 * 7) if outputTensorScale > maxOutputTensorScale, further apply right shift till the outputTensorScale is less than
 *     maxOutputTensorScale. Lets call the shift required is extraRoundBits. Then output tensor scale will becomes
 *     roundBits = roundBits + extraRoundBits
 *     outTensorScale = accumulatorScale / ( 1 << roundBits ).
 *
 *
 *  \param  pOrgTIDLNetStructure [IN] Pointer to the TIDL net structure
 *  \param  customUserParams      [IN] Pointer to the user defined parameters for a given custom layer
 *  \param  layerIndex                 [IN] Layer index in TIDL net structure
 *  \param  minValue                   [IN] Minimum value of the current activation
 *  \param  maxValue                  [IN] Maximum value of the current activation
 *  \param maxOutputTensorScale [IN] Maximum possible ouput tensor scale for a given layer. If user computed
 *                                          scale is more than this then user should reduce this scale by using lessor
 *                                          number of bits for quantization.
 *
 *  \return output tensor scale for given layer
 */
float32_tidl TIDL_getCustomLayerOutputTensorScale(sTIDL_OrgNetwork_t *pOrgTIDLNetStructure,
                                                         void * customUserParams,
                                                         int32_t layerIndex,
                                                         float32_tidl minTensorValue,
                                                         float32_tidl maxTensorValue,
                                                         float32_tidl maxOutputTensorScale);


/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#endif /* TIDL_CUSTOM_IMPORT_H_ */

