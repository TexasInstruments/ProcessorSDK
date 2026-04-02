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

#ifndef TIDL_PARSE_TFLITE_H_
#define TIDL_PARSE_TFLITE_H_

#include "schema_generated.h"
#include "flatbuffers/flexbuffers.h"
#include "tensorflow/lite/builtin_ops.h"
#include "tensorflow/lite/context_util.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/minimal_logging.h"

#include "ti_dl.h"
#include "tidl_import_api.h"
#include "tidl_import_common.h"
#include "tidl_import_config.h"
#include "tidl_import_diag.h"


using namespace std; 
using namespace tflite;

extern tidl_import_config gParams;

/// @brief Parses TfLite layers
struct TidlParseTflite
{
    sTIDL_LayerPC_t layer;

    const TfLiteRegistration* registration;
    const TfLiteNode* node;
    TfLiteContext* context;

    int32_t status;

    /// @brief Constructor of TidlParseTflite
    TidlParseTflite(const TfLiteRegistration* registration, const TfLiteNode* node, TfLiteContext* context, sTIDL_LayerPC_t layer);

    /// @brief The parse function: fills in attributes into sTIDL_Layer_PC_t struct
    template <int32_t layerType>
    int32_t parse();


    // Utility Functions
    void saveAllowlistingMetaData();
    void fillActParams(sTIDL_ActParams_t & actParams, int32_t tfLiteActType);
    int32_t copyInputConstTensor(const TfLiteRegistration* registration, const TfLiteNode* node,
                                    TfLiteContext* context, int32_t inIdx, sBuffer_t &buf, sBuffer_t &scaleBuf, sBuffer_t &zpBuf);
    void dequantTensor(sBuffer_t &buf, sBuffer_t &scaleBuf, sBuffer_t &zpBuf, int32_t size);
    int32_t numValidInputs(const TfLiteNode* node, std::vector<int32_t> &validInputIndices);
    int32_t numVariableLayerInputs(TfLiteContext* context, const TfLiteNode* node, std::vector<int32_t> &varIdx, const std::vector<int32_t> validInputIndices);
    bool isTensorVariable(TfLiteTensor * tensor);
    void broadcastFloatTensor(sBuffer_t &buf, int32_t bufSize);
};




template <class Tw>
uint32_t tidlParseTfliteKernelReshape(Tw * param, uint32_t w, uint32_t h, uint32_t ci, uint32_t co)
{
  Tw * tPtr = (Tw * )my_malloc(w*h*ci*co*sizeof(Tw));
  int32_t counter = 0;
  for(int l1 = 0; l1 < co; ++l1){
    for(int l = 0; l < ci; ++l){
      int k = l;
      for(int j = 1; j<=w*h; ++j){
        tPtr[counter] = param[l1*w*h*ci + k];
        k+=ci;
        counter++;
      }
    }
  }
  memcpy(param,tPtr,w*h*ci*co*sizeof(Tw));
  free(tPtr);
  return 0;
}

#endif /*TIDL_PARSE_TFLITE_H_ */