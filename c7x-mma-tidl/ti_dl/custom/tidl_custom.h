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
 *  \file tidl_custom.h
 *
 *  \brief This file defines the custom interfaces for TI Deep Learning Library
 */

#ifndef TIDL_CUSTOM_H_
#define TIDL_CUSTOM_H_
#include "itidl_ti.h"

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor TIDL_CustomLayerType
 *  \name   TIDL Custom Layer Type
 *
 *  This group defines the different types of custom layers
 *
 *  @{
 */
#define TIDL_CUSTOM_TYPE_0          (0)
#define TIDL_CUSTOM_TYPE_1          (1)
#define TIDL_CUSTOM_TYPE_2          (2)
#define TIDL_CUSTOM_TYPE_3          (3)
/* @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 @struct  TIDL_CustomParams0_t
 @brief   This structure define the parameters for custom layer 0 - These are
          spatial pooling parameters
*/
typedef struct {
  /** Type of the Pooling as defined in @ref eTIDL_PoolType */
  int32_t   poolingType;
  /** Kernel width */
  int32_t   kernelW;
  /**  Kernel height */
  int32_t   kernelH;
  /** Stride in horizontal direction */
  int32_t   strideW;
  /**  Stride in vertical direction */
  int32_t   strideH;
  /** Horizontal Padding requirement in number of elements */
  int32_t   padW;
  /** Vertical Padding requirement in number of elements */
  int32_t   padH;
  /** Q value of the in data */
  int32_t   inDataQ;
  /** Q value expected for out data */
  int32_t   outDataQ;
  /** ceil condition for caffe models */
  int32_t   useCeil;
} TIDL_CustomParams0_t;

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
   *  \brief Custom Layer Process API. Please refer to itidl_ti.h for details.
   */
int32_t TIDL_customLayerProcess(void * tidlHandle,
                                sTIDL_Layer_t *tidlLayer,
                                void *inPtrs[],
                                void *outPtrs[],
                                void *params,
                                void *dmaUtilsContext,
                                const sTIDL_sysMemHandle_t sysMems[TIDL_SYSMEM_MAX],
                                int32_t execMode);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* TIDL_CUSTOM_H_ */

