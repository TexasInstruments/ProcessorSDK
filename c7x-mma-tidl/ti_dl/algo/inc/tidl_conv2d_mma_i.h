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
#ifndef TIDL_CONV2D_MMA_H_
#define TIDL_CONV2D_MMA_H_
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "tidl_alg_int.h"
#include "dmautils_ext.h"
#include "tidl_alg_utils.h"
#define QT_BUILD (0)


#if QT_BUILD
#define MINIMUM_PRINT_ENABLE (0)
#else
#define MINIMUM_PRINT_ENABLE (0)
#endif /* #ifdef QT_BUILD */

#if !defined(_HOST_BUILD)
#define ENABLE_MMA_ONLY_PROFILE (1)
#else
/* For host build always disable this */
#define ENABLE_MMA_ONLY_PROFILE (0)
#endif


#define TIDL_L2_MEM_SIZE (256*1024)

#define MAX_CIR_BUF_SIZE (4*1024) //:TODO: THis needs to be dynamic

#define TIDL_MIN_ROWFLOW_PROCSIZE (__C7X_VEC_SIZE_BYTES__ << 1U)
#define TIDL_MSMC_FETCH_BOUNDARY   (128U)

#include "mmalib_cnn.h"

int32_t TIDL_conv2dAllocateHandleAndComputeSize(const void   *basePtr,
                                                             const sTIDL_ConvParams_t *conv2dparams,
                                                             const TIDL_LayerSpecificParams *layerSpecificParams,
                                                             const TIDL_NetworkCommonParams   *commonParams,
                                                             int32_t layerIdx,
                                                             sTIDL_AlgLayer_t *algLayer);

int32_t TIDL_conv2dDspAllocNew(const sTIDL_ConvParams_t  *conv2dparams,
                                   const TIDL_LayerSpecificParams *layerSpecificParams,
                                   const TIDL_NetworkCommonParams *commonParams,
                                   int32_t layerIdx,
                                   int32_t memorySize[TIDL_LAYER_MEMORY_MAX]);

int32_t TIDL_conv2dDspInitNew(sTIDL_ConvParams_t *conv2dparams,
                                const TIDL_LayerSpecificParams *layerSpecificParams,
                                const TIDL_NetworkCommonParams *commonParams,
                                sTIDL_AlgLayer_t               *algLayer,
                                int32_t layerIdx,
                                uint8_t *memory[TIDL_LAYER_MEMORY_MAX],
                                int32_t memorySize[TIDL_LAYER_MEMORY_MAX]);

int32_t TIDL_conv2dDspProcess(sTIDL_ConvParams_t *conv2dparams,
                              TIDL_NetworkCommonParams    *commonParams,//:TODO: Eventually should be removed
                              sTIDL_AlgLayer_t     * algLayer,
                              sTIDL_Layer_t        * tidlLayer,
                              void                 * inPtr,
                              void                 * outPtr,
                              int32_t                layerIdx);



#endif /*TIDL_CONV2D_MMA_H_*/

