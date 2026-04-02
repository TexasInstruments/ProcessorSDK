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

/**
 ----------------------------------------------------------------------------
 @file    tidl_innerProduct.h
 @brief   This file defines process function prototype of inner product layer.
 @version 0.1 (Jan 2017) : Initial version [EPR]
 ----------------------------------------------------------------------------
*/

#ifndef ITIDL_INNERPRODUCT_H
#define ITIDL_INNERPRODUCT_H
#include "tidl_alg_int.h"
#include "tidl_innerProduct_int.h"

/* To enable/disable mixed precision.
 * 0 --> Code is disabled.
 * 1 --> Code is enabled. */
#define INNER_PRODUCT_MIXED_PRECISION       0

/* To enable/disable unsigned output.
 * 0 --> Code is disabled.
 * 1 --> Code is enabled. */
#define INNER_PRODUCT_UNSIGNED_OUTPUT       1

int32_t TIDL_forceInnerProductRef(
  const TIDL_CreateParams  *createParamsPtr,
  const sTIDL_Layer_t* tidlLayer
);

/**
*******************************************************************************
  @ingroup    TIDL_INNERPRODUCT
  @fn         TIDL_innerProductAllocNew
  @brief      In the inner product alloc function, associated DDR mem required 
              for processing is requested depending on configuration of innerproduct layer.

  @param   specificParams : Pointer to specific parameter required for all the layers
  @param   commonParams   : Pointer to common parameter required for all the layers
  @param   layerIdx       : Layer Id of the layer
  @param   memorySize     : Output memory requirement of the layer
  @return     Error:  IALG_EOK ( Successful) else failure
  @remarks
*******************************************************************************
*/
int32_t TIDL_innerProductAllocNew(const TIDL_LayerSpecificParams *layerSpecificParams,
                                    const TIDL_NetworkCommonParams *commonParams,
                                    int32_t layerIdx,
                                    int32_t memorySize[TIDL_LAYER_MEMORY_MAX]);

/**
*******************************************************************************
  @ingroup    TIDL_INNERPRODUCT
  @fn         TIDL_innerProductInitNew
  @brief      In the inner product init function, associated DDR mem required 
              for processing is set depending on configuration of innerproduct layer.

  @param     specificParams :  Pointer to specific parameter required for all the layers
  @param     commonParams   :  Pointer to common parameter required for all the layers
  @param     algLayer       :  Pointer to layer specific internal parameters
  @param     layerIdx       :  Layer Id of the layer
  @param     memory         :  Allocated memory pointers
  @param     memorySize     :  Output memory requirement of the layer
  @param     outPtr         :  Output memory pointer. Applicable only for reference flow
  @remarks    None
  @return     status  - EPR - Add status values -
*******************************************************************************
*/
int32_t TIDL_innerProductInitNew(const TIDL_LayerSpecificParams *layerSpecificParams,
                                const TIDL_NetworkCommonParams  *commonParams,
                                sTIDL_AlgLayer_t               *algLayer,
                                int32_t layerIdx,
                                uint8_t *memory[TIDL_LAYER_MEMORY_MAX],
                                int32_t memorySize[TIDL_LAYER_MEMORY_MAX],
                                void    **outPtr);

/**
*******************************************************************************
  @ingroup    TIDL_INNERPRODUCT
  @fn         TIDL_innerProductProcessNew
  @brief      In the inner product process function, inner product happens 
              between input and kernel coeffs to get the output.
  @param [in]  specificParams :  Pointer to specific parameter required for all the layers
  @param [in]  commonParams   :  Pointer to common parameter required for all the layers
  @param [in]  algLayer       :  Pointer to layer specific internal parameters
  @param [in]  layerIdx       : Layer Id of the layer
  @param [in]  memory         : Allocated memory pointers
  @param [out]  memorySize    : Output memory requirement of the layer
  @param [out]  outPtr        : Output memory pointer. Applicable only for reference flow
  @remarks    None
  @return     status  - EPR - Add status values -
*******************************************************************************
*/
int32_t TIDL_innerProductProcessNew(TIDL_NetworkCommonParams *commonParams,
                              sTIDL_AlgLayer_t     * algLayer,
                              sTIDL_Layer_t        * tidlLayer,
                              void                 * inPtrs[],
                              void                 * outPtrs[],
                              int32_t                layerIdx);

#endif /* ITIDL_INNERPRODUCT_H */

