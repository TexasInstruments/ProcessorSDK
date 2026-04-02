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
 *  \file tidl_reduce.h
 *
 *  \brief This file defines the process function prototype of reduceMax/reduceMin & similar layers
 */

#ifndef TIDL_SQUEEZE_H_
#define TIDL_SQUEEZE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "tidl_alg_int.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 ----------------------------------------------------------------------------
 @ingroup    TIDL_Squeeze
 @fn         TIDL_squeezeProcess
 @brief      No data processing is done, just movement of data is done if input
             and output pointers are different
 @param      intAlgHandle : Pointer to algorithm internal handle
 @param      algLayer : Pointer to algorithm layer parameter used in Reduce layer
 @param      tidlLayer: Pointer to TIDL layer parameter used in Reduce layer
 @param      inPtrs: Pointer to input buffers to be processed
 @param      outPtrs: Pointer to output buffers to be processed
 @param      sysMems: Pointer to memory related buffers used in Reduce layer
 @remarks    None
 @return     status =>  0: PASS; Anything else: FAIL
 ----------------------------------------------------------------------------
*/

int32_t TIDL_squeezeProcess(TIDL_Handle          intAlgHandle,
                              const sTIDL_AlgLayer_t     *algLayer,
                              sTIDL_Layer_t        *tidlLayer,
                              void                 *inPtrs[],
                              void                 *outPtrs[],
                              sTIDL_sysMemHandle_t *sysMems);

/**
 ----------------------------------------------------------------------------
 @ingroup    TIDL_Squeeze
 @fn         TIDL_squeezeProcessNew
 @brief      No data processing is done, just movement of data is done if input
             and output pointers are different
 @param      commonParams : Pointer to structure containing common params
 @param      algLayer : Pointer to algorithm layer parameter used in Reduce layer
 @param      tidlLayer: Pointer to TIDL layer parameter used in Reduce layer
 @param      inPtrs: Pointer to input buffers to be processed
 @param      outPtrs: Pointer to output buffers to be processed
 @param      sysMems: Pointer to memory related buffers used in Reduce layer
 @remarks    None
 @return     status =>  0: PASS; Anything else: FAIL
 ----------------------------------------------------------------------------
*/
int32_t TIDL_squeezeProcessNew(TIDL_NetworkCommonParams *commonParams,
                              sTIDL_AlgLayer_t     * algLayer,
                              sTIDL_Layer_t        * tidlLayer,
                              void                 * inPtrs[],
                              void                 * outPtrs[],
                              int32_t                layerIdx);


/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#endif /* TIDL_SQUEEZE_H_*/
