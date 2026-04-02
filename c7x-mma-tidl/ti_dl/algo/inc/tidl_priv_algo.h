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
 @file    tidl_priv_algo.h
 @brief   This file defines APIS for TIDL Private algorithms.
 @version 0.1 (Mar 2020) : Intial Version
 ----------------------------------------------------------------------------
*/

#ifndef ITIDL_PRIVALGO_H
#define ITIDL_PRIVALGO_H
#include "tidl_alg_int.h"
#include "tidl_alg_utils.h"

/**
 ----------------------------------------------------------------------------
 @ingroup    TIDL_PRIV_ALGO
 @fn         TIDL_privGetContextSize
 @brief      Function to get context memory size required for Provate algorithm.
 @param      None
 @remarks    None
 @return     Context memory Size in bytes
 ----------------------------------------------------------------------------
*/
#define TIDL_HIGH_RESOLUTION_OPTIMIZATION_ENABLED   (0)
#define TIDL_HIGH_RESOLUTION_OPTIMIZATION_DISABLED  (1)


uint32_t TIDL_privGetContextSize(void);


int32_t TIDL_privAlgoLayerProcess(
    TIDL_Handle intAlgHandle,
    sTIDL_AlgLayer_t *algLayer,
    sTIDL_Layer_t *TIDLLayer,
    void *inPtrs[],
    void *outPtrs[],
    sTIDL_sysMemHandle_t *sysMems,
    int32_t i,
    void *privContext,
    TIDL_NetworkCommonParams   *commonParams,
    int32_t targetDevice);


/******************************************************************
 *                  Workload enabled functions 
 ******************************************************************/

int32_t TIDL_getAlgProcType(const sWorkloadUnit_t *workloadUnit);

void TIDL_privSetTensorDimensions(const TIDL_CreateParams *createParams,
                            sTIDL_AlgLayer_t     * alglayerParams,
                            sTIDL_AlgLayer_t     * algLayer,
                            const sWorkloadUnit_t *workloadUnit,
                            int32_t inHeightOrig[TIDL_NUM_IN_BUFS],
                            int32_t inChOrig[TIDL_NUM_IN_BUFS],
                            int32_t grpOrig[TIDL_NUM_IN_BUFS],
                            int32_t inChPitchOrig[TIDL_NUM_IN_BUFS],
                            int32_t inPitchRoiOrig[TIDL_NUM_IN_BUFS],
                            int32_t inBatchOrig[TIDL_NUM_IN_BUFS],
                            int32_t outHeightOrig[TIDL_NUM_OUT_BUFS],
                            int32_t outChOrig[TIDL_NUM_OUT_BUFS],
                            int32_t outChPitchOrig[TIDL_NUM_OUT_BUFS],
                            int32_t outPitchRoiOrig[TIDL_NUM_OUT_BUFS],
                            int32_t outBatchOrig[TIDL_NUM_OUT_BUFS]);

void TIDL_privRestoreTensorDimensions(const TIDL_CreateParams *createParams,
                            const sTIDL_AlgLayer_t     * algLayer,
                            int32_t inHeightOrig[TIDL_NUM_IN_BUFS],
                            int32_t inChOrig[TIDL_NUM_IN_BUFS],
                            int32_t grpOrig[TIDL_NUM_IN_BUFS],
                            int32_t inChPitchOrig[TIDL_NUM_IN_BUFS],
                            int32_t inPitchRoiOrig[TIDL_NUM_IN_BUFS],
                            int32_t inBatchOrig[TIDL_NUM_IN_BUFS],
                            int32_t outHeightOrig[TIDL_NUM_OUT_BUFS],
                            int32_t outChOrig[TIDL_NUM_OUT_BUFS],
                            int32_t outChPitchOrig[TIDL_NUM_OUT_BUFS],
                            int32_t outPitchRoiOrig[TIDL_NUM_OUT_BUFS],
                            int32_t outBatchOrig[TIDL_NUM_OUT_BUFS]);

#endif /* ITIDL_PRIVALGO_H*/

