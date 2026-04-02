/*
*
* Copyright (c) {2015 - 2021} Texas Instruments Incorporated
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
 *  \file workload_unit_exec.h
 *
 *  \brief This file contains interface to setup and execute a workload unit
 *
 *
 */

#ifndef WORKLOAD_REF_EXEC_H_
#define WORKLOAD_REF_EXEC_H_

#include "gc.h"

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 @struct  WorkloadUnitExec_RefParams
 @brief   This structure defines elements of workload needed for reference flow */
typedef struct
{
  sLink_t *ctxtStrLink;
  sLink_t *ctxtRstrLink;
  sLink_t *multiCoreCtxCopyLink[MAX_MULTI_CORE_CTX_COPY_LINK];
  int32_t origInHeight[TIDL_NUM_IN_BUFS];
  int32_t origInCh[TIDL_NUM_IN_BUFS];
  int32_t origInChPitch[TIDL_NUM_IN_BUFS];
  int32_t origInRoiPitch[TIDL_NUM_IN_BUFS];
  int32_t origInBatch[TIDL_NUM_IN_BUFS];
  int32_t origGrp;
  int32_t origOutHeight;
  int32_t origOutCh;
  int32_t origOutChPitch;
  int32_t origOutRoiPitch;
  int32_t origOutBatch;
  int32_t outTileHeight;
  int32_t procType;
}WorkloadRefExec_RefExecParams;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief This function returns the index of the output buffer for a given workload
 *
 *  \param  workloadUnit   [IN] workload unit for which output buffer needs to be found
 *  \param  gcHelperHandle     [IN] Graph compiler database
 *  \return Index of buffer in buffer Database
 */
int32_t WorkloadRefExec_GetOutBufIdx(const sWorkloadUnit_t *workloadUnit, const sGCHelperHandle * gcHelperHandle);

/**
 *  \brief This function runs workload through the reference flow.
 *
 *  \param  algHandle      [IN] Handle for the dataFlow (this is created via WorkloadUnitExec_Init API)
 *  \param  commonParams   [IN] Common params structure required for process
 *  \param  workloadUnit   [IN] Workload structure containing gc info
 *  \param  algLayer       [IN] Pointer to algLayer structure
 *  \param  tidlLayer      [IN] Pinter to tidlLayer structure coming from net structure
 *  \param  inPtrs[]       [IN] List of pointers to inputs
 *  \param  outPtrs[]      [OUT] Pointer to the output tensor
 *  \param  layerId        [IN] Layer Index
 *  \param  coreId        [IN] coreId Index
 *  \param  syncId        [OUT] ID to be used for sync
 *
 *  \return Error Status
 */
int32_t WorkloadRefExec_Process(TIDL_Handle         algHandle,
                              TIDL_NetworkCommonParams *commonParams,
                              sWorkloadUnit_t        * workloadUnit,
                              sTIDL_AlgLayer_t     * algLayer,
                              sTIDL_Layer_t        * tidlLayer,
                              void                 * inPtrs[],
                              void                 * outPtrs[],
                              int32_t                layerId,
                              int32_t                coreId);

/**
 *  \brief This function runs workload through the reference flow.
 *
 *  \param  outPtr        [OUT] Pointer to baseMem of output tensor buffer
 *  \param  algHandle      [IN] Handle for the dataFlow (this is created via WorkloadUnitExec_Init API)
 *  \param  workloadUnit   [IN] Workload structure containing gc info
 *  \param  tidlLayer      [IN] Pinter to tidlLayer structure coming from net structure
 *  \param  algLayer       [IN] Pointer to algLayer structure
 *  \param  tensorPosition [IN] Structure containing info about relative position of output tensor in overall larger tensor
 *  \param  tensorDims     [IN] Structure containing info about dimensions of output tensor
 *  
 *  \return Error Status
 */
int32_t WorkloadRefExec_writeTraceDataBuf(void *outPtr,
                                        TIDL_Handle algHandle,
                                        sTIDL_Layer_t     *tidlLayer,
                                        sTIDL_AlgLayer_t  *algLayer,
                                        const sTensorPosition_t *tensorPosition,
                                        const sTensorDims_t *subTensorDims,
                                        const sTensorDims_t *fullTensorDims);

void WorkloadRefExec_getRefParams(WorkloadRefExec_RefExecParams *wlRefParams,
                                const TIDL_CreateParams *createParams,
                                const sWorkloadUnit_t *workloadUnit,
                                const sTIDL_Layer_t * tidlLayer,
                                const sTIDL_AlgLayer_t * algLayer);

int32_t WorkloadRefExec_enableRefExecFlow(uint32_t flowCtrl,
                                              const sTIDL_Layer_t *tidlLayer,
                                              const sTIDL_AlgLayer_t *algLayer);


#endif /* WORKLOADRE_H_*/
