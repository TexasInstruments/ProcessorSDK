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

#ifndef TIDL_IMPORT_LUT_H_
#define TIDL_IMPORT_LUT_H_

#define TIDL_MAX_ACTTYPE_NAME (15U)

/**
 @struct  sTIDL_NonLinearAct_LUT_createParams_t
 @brief   This structure define the parameters for generation of LUT table for Non Linear Activations
           in TIDL

*/
typedef struct
{
  /**  value indicate the Non Linear Activation */
  int32_t actType;
  /**  value indicates the input zero point for the layer */
  int32_t inTensorZeroPoint;
  /**  Floating point scale value for the input data */
  float32_tidl inTensorScale;
  /**  value indicates the output zero point for the layer */
  int32_t outTensorZeroPoint;
  /**  Floating point scale value for the output data */
  float32_tidl outTensorScale;
  /**  value indicates the input data type for the Activation */
  int32_t inElemType;
  /**  value indicates the input data type for the Activation */
  int32_t outElemType;
  /**  Value indicate the Netversion to decide between fused and normal kernel for gelu */
  int32_t tidlNetVersion;
  /**  value indicates alpha, beta for some operations as below: */
  /* for hard sigmoid : both alpha and beta are valid */
  /* for Leaky relu and ELU  only alpha is valid */
  /* for other operators, both alpha and beta are invalid*/
  double alpha, beta;
  /**  value indicates pow for only pow operaotr */
  double pow;
} sTIDL_NonLinearAct_LUT_createParams_t;

typedef struct 
{
  /**  value indicates the total number of Activation types that user has given in the import config*/
  int32_t numActType;
  /**  Array indicates the list of Activation Method that user has given in the import config*/
  int32_t importActMethod[TIDL_MAX_ALG_IN_BUFS];
  /**  Array indicates the list of Activation Type(given by user in the import config) converted from string to TIDL type*/
  int32_t importActTypeTIDL[TIDL_MAX_ALG_IN_BUFS];
  /**  Array indicates the list of Activation Type that user has given in the import config*/
  uint8_t importActTypeStr[TIDL_MAX_ALG_IN_BUFS][TIDL_MAX_ACTTYPE_NAME];
} sTIDL_NonLinearAct_LUT_importActParams_t;


int32_t TIDL_nonLinearActLUT_prepare(sTIDL_NonLinearAct_LUT_createParams_t *pLUTCreateParams,
                                      sBuffer_t *pLUTBuf, sTIDL_NonLinearActivation_LUT_t *pLUTParams);
int32_t TIDL_nonLinearActLUT_allocate(sTIDL_NonLinearAct_LUT_createParams_t *pLUTCreateParams,
                                      sBuffer_t *pLUTBuf, sTIDL_NonLinearActivation_LUT_t *pLUTParams,
                                      sTIDL_NonLinearAct_LUT_importActParams_t *importActParam);

int32_t TIDL_nonLinearActLUT_deallocate(sBuffer_t *pLUTBuf);

#endif /*TIDL_IMPORT_LUT_H_ */
