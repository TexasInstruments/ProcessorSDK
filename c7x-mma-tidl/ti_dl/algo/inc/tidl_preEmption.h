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
 @file    tidl_preemption.h
 @brief   This file defines APIS and Data structures of TIDL PreEmption APIs.
 @version 0.1 (Sep 2021) : Initial Version
 ----------------------------------------------------------------------------
*/

#ifndef ITIDL_PRIEMPTION_H
#define ITIDL_PRIEMPTION_H

#ifdef HOST_EMULATION
// #define DEBUG_PREEMPTION_HOST_EMULATION
  #define ENABLE_PREEMPTION 0
#else
  #define ENABLE_PREEMPTION 1
#endif

#include <stdint.h>
#include "itidl_ti.h"
#include "gc_helper.h"

/* Maximum number of layers for backup in context memory at layer boundary */
#define  TIDL_MAX_CONTEXT_LAYERS  16U
#define  TIDL_PREEMPT_CONTEXT_MEMSIZE_DUMMY (128)
#define  LOCKED   1
#define  UNLOCKED 0

#ifdef DEBUG_PREEMPTION_HOST_EMULATION
extern IALG_Handle gHandleForPremptTest;
#endif

/**<
 *******************************************************************************
  @struct TIDL_preEmptContext
  @brief  This structure holds the context memory buffer pointers

  @param src                 : Source pointer of the context memory buffer
  @param dst                 : Destination pointer of the context memory buffer
  @param size                : Size of the context memory buffer
 *******************************************************************************
*/
typedef struct
{
  uint8_t*  src;
  uint8_t*  dst;
  uint32_t  size;
}TIDL_preEmptContext;

/**<
 *******************************************************************************
  @struct TIDL_preEmptContextObj
  @brief  This structure holds the context memory info for preemption

  @param earlyPreEmpt        : Variable to indicate graceful exit vs pre-empted 
  @param numContextBufs      : No of context memory buffer pointers
  @param contextMemPtr       : Base memory pointer to the context memory
  @param contextBufInfo      : Structure holds the context memory buffer pointers
 *******************************************************************************
*/
typedef struct
{
  int32_t  earlyPreEmpt;
  int32_t  numContextBufs;
  uint8_t* contextMemPtr;  
  uint32_t contextMemSize;
  TIDL_preEmptContext contextBufInfo[TIDL_MAX_CONTEXT_LAYERS*TIDL_NUM_OUT_BUFS]; 
}TIDL_preEmptContextObj;

/**
 *******************************************************************************
 *  @func         TIDL_initializeHandleForPreemption
 *  @brief        This function initializes preemption handle with procTime and
 *                context size for each layer and also calculates the layers list
 *                for optimal preemption 
 *
 *  @param [out]  preEmptHandle       : preemption handle
 *  @param  [in]  gcHelperHandle      : Handle of GC Data base to extract mainly bufDB
 *  @param [in]   perfSimOutput       : persim output containing context info
 *  @param [in]   numLayers           : Number of layers in the network
 *  @param [in]   threshold           : maxPreEmptDelay from user
 *  @param [in]   l2Scratch           : L2 scratch memory pointer 
 * 
 *  @return       NONE
 *  @remarks  
 *******************************************************************************
 */
void TIDL_initializeHandleForPreemption(void * preEmptHandle, 
                                        const sGCHelperHandle  *gcHelperHandle,
                                        const sPerfSim_t * perfSimOutput, 
                                        int32_t numLayers,
                                        int32_t numWL, 
                                        float32_tidl threshold, 
                                        uint8_t* l2Scratch,
                                        float32_tidl ddrThroughput,
                                        int32_t freqMHz,
                                        void *pAlgLayerList);

/**
 *******************************************************************************
 *  @func         TIDL_reCalculateListForPreemption
 *  @brief        This function calculates the layers list for optimal preemption
 *                using target procTime after executing the first frame 
 *
 *  @param [out]  preEmptHandle       : preemption handle
 *  @param [in]   numLayers           : Number of layers in the network
 *  @param [in]   threshold           : maxPreEmptDelay from user
 *  @param [in]   l2Scratch           : L2 scratch memory pointer 
 * 
 *  @return       NONE
 *  @remarks
 *******************************************************************************
 */                              
void TIDL_reCalculateListForPreemption(void * preEmptHandle,
                                      int32_t *perfHW, 
                                      int32_t numLayers,
                                      int32_t numWL, 
                                      float32_tidl threshold, 
                                      uint8_t* l2Scratch,
                                      float32_tidl ddrThroughput,
                                      int32_t freqMHz,
                                      void *pAlgLayerList);

/**
 *******************************************************************************
 *  @func         TIDL_updateLayerCyclesForPreemption
 *  @brief        This function updates preemption handle with target execution
 *                cycles (procTime) after first frame  
 *
 *  @param [out]  preEmptHandle       : preemption handle
 *  @param [in]   cycles              : Cycles on the target for the given layer
 *  @param [in]   layerId             : layer id of the given layer
 *  @param [in]   procType            : processing type needed for ST flow 
 *  @param [in]   freqMHz             : Frequency of the C7x+MMA in MHz
 * 
 *  @return       NONE
 *  @remarks
 *******************************************************************************
 */
void TIDL_updateLayerCyclesForPreemption(void * preEmptHandle,
                                        int32_t perfHW, 
                                        uint64_t cycles, 
                                        int32_t layerId, 
                                        int32_t procType,
                                        int32_t freqMHz);

/**
 *******************************************************************************
 *  @func         TIDL_storeContextMemToExtMemForPreemption
 *  @brief        This function stores/backup the context memory to external memory 
 *
 *  @param [out]  contextInfoPtr      : holds information about context memory pointers 
 *  @param [in]   dmaUtilsContext     : memory pointer for dmaUtils Context
 *  @param [in]   trMem               : pointer to store tr memory
 * 
 *  @return       NONE
 *  @remarks
 *******************************************************************************
 */                            
void TIDL_storeContextMemToExtMemForPreemption(TIDL_preEmptContextObj  *contextInfoPtr, 
                                               void *dmaUtilsContext, 
                                               uint8_t * trMem);

/**
 *******************************************************************************
 *  @func         TIDL_restoreContextMemToIntMemForPreemption
 *  @brief        This function restores the context memory from external memory 
 *
 *  @param [out]  contextInfoPtr      : holds information about context memory pointers 
 *  @param [in]   dmaUtilsContext     : memory pointer for dmaUtils Context
 *  @param [in]   trMem               : pointer to store tr memory
 *  @param [in]   createParams        : create time parameters for TIDL 
 * 
 *  @return       NONE
 *  @remarks
 *******************************************************************************
 */                            
void TIDL_restoreContextMemToIntMemForPreemption(const TIDL_preEmptContextObj  *contextInfoPtr, 
                                                void *dmaUtilsContext, 
                                                uint8_t * trMem,
                                                const TIDL_CreateParams * createParams);

/**
 *******************************************************************************
 *  @func         TIDL_updateContextMemPtrsForPreemption
 *  @brief        This function populates the context memory pointers and sizes to 
 *                backup the context memory before preemption  
 *
 *  @param [out]  contextInfoPtr      : holds information about context memory pointers
 *  @param  [in]  gcHelperHandle      : Handle of GC Data base to extract mainly bufDB
 *  @param [in]   privContext         : privContext memory pointer to backup
 *  @param [in]   preEmptHandle       : preemption handle memory pointer to backup
 *  @param [in]   perfSimOutput       : persim output containing context info
 *  @param [in]   l2BasePtr           : L2 base memory pointer for backup
 *  @param [in]   l3BasePtr           : L3 base memory pointer for backup
 *  @param [in]   lyrNum              : Layer number at preemption 
 *  @param [in]   numLayers           : Number of layers in the network 
 * 
 *  @return       0/1 indicating if context memory is sufficient or not 
 *  @remarks
 *******************************************************************************
 */                            
int32_t TIDL_updateContextMemPtrsForPreemption(TIDL_preEmptContextObj  *contextInfoPtr, 
                                              const sGCHelperHandle  *gcHelperHandle,
                                              void* privContext, 
                                              void* preEmptHandle, 
                                              const sPerfSim_t * perfSimOutput, 
                                              uint8_t *l2BasePtr, 
                                              uint8_t* l3BasePtr, 
                                              int32_t lyrNum,
											  int32_t numLayers);

/**
 *******************************************************************************
 *  @func         TIDL_checkIfPreEmptionEnabled
 *  @brief        This function checks if preemption is enabled or not  
 *
 *  @param [in]   createParams        : create time parameters for TIDL 
 * 
 *  @return       0/1 indicating if preemption is enabled or not 
 *  @remarks
 *******************************************************************************
 */ 
int32_t TIDL_checkIfPreEmptionEnabled(const TIDL_CreateParams * createParams);

/**
 *******************************************************************************
 *  @func         TIDL_getHandleSizeForPreemption
 *  @brief        This function gets the preemption handle size  
 *
 *  @return       this returns preemption handle size for number of layers
 *  @remarks
 *******************************************************************************
 */ 
uint32_t TIDL_getHandleSizeForPreemption(int32_t numLayers);

/**
 *******************************************************************************
 *  @func         TIDL_lockForPreemption
 *  @brief        This function is wrapper function for lock api  
 *
 *  @param [in]   algHandle           : algo for TIDL 
 *  @param [in]   createParams        : create time parameters for TIDL 
 *  @param [in]   plockState          : State to indicate the Lock/Unlock status 
 * 
 *  @return       returns interrupt state
 *  @remarks
 *******************************************************************************
 */ 
int32_t TIDL_lockForPreemption(void* algHandle, const TIDL_CreateParams * createParams, int32_t *plockState);

/**
 *******************************************************************************
 *  @func         TIDL_unlockForPreemption
 *  @brief        This function is wrapper function for unlock api  
 *
 *  @param [in]   oldIntState         : variable to indicate interrupt state
 *  @param [in]   createParams        : create time parameters for TIDL 
 *  @param [in]   plockState          : State to indicate the Lock/Unlock status 
 * 
 *  @return       NONE
 *  @remarks
 *******************************************************************************
 */ 
void TIDL_unlockForPreemption(int32_t oldIntState, const TIDL_CreateParams * createParams, int32_t *plockState);


/**
 *******************************************************************************
 *  @func         TIDL_giveChanceForPreemption
 *  @brief        This function does TIDL_unlockForPreemption followed by TIDL_lockForPreemption to give a higher priorty node chance for pre-emption
 *  
 *  @param [in]   algHandle           : algo for TIDL 
 *  @param [in]   plockState          : State to indicate the Lock/Unlock status
 *  @param [in]   oldIntState         : variable to indicate interrupt state
 * 
 *  @return       NONE
 *  @remarks
 *******************************************************************************
 */ 
void TIDL_giveChanceForPreemption(void* algHandle, int32_t *plockState, int32_t * oldIntState);


void* TIDL_getLastActiveHandle(int32_t coreId);
TIDL_Lock_t TIDL_getLockFxnPtr(int32_t coreId);
TIDL_Unlock_t TIDL_getUnLockFxnPtr(int32_t coreId);
void TIDL_setLastActiveHandle(int32_t coreId, void * handle);


void* TIDL_getContextMemRec(void* handle);
int32_t TIDL_getContextMemSize(int32_t priorityId, const sPerfSim_t * perfSimInfo);
int32_t TIDL_addPriorityObject( void *handle, const IALG_MemRec *pMemRec, int32_t priorityId);
int32_t TIDL_removePriorityObject( void *handle, IALG_MemRec *pMemRec);

#endif /*ITIDL_PRIEMPTION_INT_H */
