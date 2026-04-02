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
#include <stdint.h>
#include <tidl_import_config.h>
#include <ti_dl.h>

#ifdef _WIN32
  #ifdef DLL_EXPORT
    #define DLL_DECL __declspec(dllexport)
  #else
    #define DLL_DECL __declspec(dllimport)
  #endif
#else
  #define DLL_DECL /* nothing */
#endif

#define TIDL_IMPORT_MODEL_FORMAT_CAFFE      (0)
#define TIDL_IMPORT_MODEL_FORMAT_TENSORFLOW (1)
#define TIDL_IMPORT_MODEL_FORMAT_ONNX       (2)
#define TIDL_IMPORT_MODEL_FORMAT_TFLITE     (3)
#define TIDL_IMPORT_MODEL_FORMAT_TVM_RELAY  (4)
#define TIDL_IMPORT_MODEL_FORMAT_TFLITE_RT  (5)
#define TIDL_IMPORT_MODEL_FORMAT_ONNX_RT    (6)


DLL_DECL extern char inConfigFilename[2*FILE_NAME_SIZE];
DLL_DECL extern tidl_import_config gParams;
DLL_DECL extern sTIDL_OrgNetwork_t orgTIDLNetStructure;
DLL_DECL extern sTIDL_Network_t    tIDLNetStructure;

extern "C"
{
DLL_DECL void setDefaultParams(tidl_import_config * params);
DLL_DECL int32_t tidlValidateImportParams(tidl_import_config * params);
};

DLL_DECL int32_t caffe_import_new( tidl_import_config * params, int32_t* dataIndex, int32_t* layerIndex);
DLL_DECL int32_t tf_import(tidl_import_config * params, int32_t* dataIndex, int32_t* layerIndex);
DLL_DECL int32_t onnx_import(tidl_import_config * params, int32_t* dataIndex, int32_t* layerIndex);
DLL_DECL int32_t tfLite_import(tidl_import_config * params, int32_t* dataIndex, int32_t* layerIndex);
DLL_DECL int32_t tf_metaArch_import(tidl_import_config * params);
DLL_DECL int32_t tidl_metaArch_import(tidl_import_config * params);
DLL_DECL void tidl_3dod_metaArch_import(tidl_import_config * params);

DLL_DECL int32_t tidl_optimizeNet(sTIDL_OrgNetwork_t  &pOrgTIDLNetStructure, int32_t &layerIndex, int32_t &dataIndex);
DLL_DECL int32_t TIDLIT_getProcessingElementSizeInBytes(const sTIDL_LayerPC_t  * tidlLayer);
DLL_DECL int32_t TIDL_import_backend(uint32_t layerIndex, int32_t dataIndex);
DLL_DECL void TIDL_exportQuantParamsToProtoTxt(sTIDL_OrgNetwork_t* pOrgTIDLNetStructure, sTIDL_Network_t* pTIDLNetStructure, tidl_import_config* configParams, int32_t layerIndex, float** weights_value_array, float** bias_value_array);
DLL_DECL int32_t TIDL_importQuantParamsFromProtoTxt(sTIDL_OrgNetwork_t *pOrgTIDLNetStructure, sTIDL_Network_t *pTIDLNetStructure, tidl_import_config *configParams);
DLL_DECL void TIDL_getQuantParamsPrototxtPath(tidl_import_config *configParams);
DLL_DECL int8_t TIDL_canBypassCalibration(sTIDL_OrgNetwork_t* pOrgTIDLNetStructure, tidl_import_config* configParams);
DLL_DECL uint8_t TIDL_isQuantParamsProtoTxtFileEmpty(char* filePath);
DLL_DECL int32_t TIDL_exportQuantParamsIntoUserPath(tidl_import_config *gParams);
