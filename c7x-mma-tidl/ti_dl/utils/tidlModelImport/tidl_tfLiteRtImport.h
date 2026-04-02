/*
*
* Copyright (c) {2015 - 2017} Texas Instruments Incorporated
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

#ifndef TIDL_TFLTE_RT_IMPORT_H
#define TIDL_TFLTE_RT_IMPORT_H

#include "tidl_runtimes_import_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

int32_t TIDL_tfliteAllowlistNode(const TfLiteRegistration* registration, const TfLiteNode* node, int32_t node_index, TfLiteContext* context, 
                                   TIDL_osrtOptions * osrtOptions, bool isObjectDetectionNetwork, std::vector<int> odProcNodesComplement);

int32_t TIDL_tfliteRtImportInit(TfLiteContext* context, const TfLiteDelegateParams* params, int32_t subgraph_id, TIDL_osrtOptions * osrt_options, 
                                  std::vector<std::string>outDataOd, bool isSubgraphOD);

int32_t TIDL_tfliteRtImportAndLinkNode(TfLiteRegistration* registration, TfLiteContext* context, const TfLiteDelegateParams* params, 
                                        TfLiteNode* node, int32_t osrtDebugPrintLevel, sTIDL_odParameters_t* odUserParams);

std::vector<int> TIDL_getPostProcInputIds(TfLiteContext* context, TfLiteIntArray* plan, std::vector<std::string> odPostProcIndataNames);
std::vector<std::vector<int>> TIDL_createInputAdjacencyList(TfLiteContext* context, TfLiteIntArray* plan);
std::vector<std::vector<int>> TIDL_createOutputAdjacencyList(TfLiteContext* context, TfLiteIntArray* plan);

int32_t TIDL_mapDetectionLayerParamsToSsdConfig(const TfLiteRegistration* registration, const TfLiteNode* node, TfLiteContext* context, sTIDL_odParameters_t * odUserParams,
                                   std::vector<int> numAspectRatios,std::vector<std::string> odHeads, int height, int width, int reduce_boxes_in_lowest_layer, int score_converter);
bool TIDL_isSsdOdPostProcStructureSupported(TfLiteContext* context,TfLiteIntArray* plan, std::vector<std::string> &odHeads, TIDL_osrtOptions * options, 
                                            std::vector<int> &numAspectRatios,int &reduce_boxes_in_lowest_layer,int &score_convertor);

#ifdef __cplusplus
}
#endif


#endif  /* TIDL_TFLTE_RT_IMPORT_H */