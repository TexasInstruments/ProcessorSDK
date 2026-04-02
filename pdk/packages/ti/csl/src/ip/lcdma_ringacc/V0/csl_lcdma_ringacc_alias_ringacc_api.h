/*
 *  Copyright (C) 2020 Texas Instruments Incorporated.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef CSL_LCDMA_RINGACC_ALIAS_RINGACC_API_H_
#define CSL_LCDMA_RINGACC_ALIAS_RINGACC_API_H_

/*-----------------------------------------------------------------------------
 * The following #define's are used to map existing ringacc CSL enumerations,
 * structure definitions, and API functions to their lcdma_ringacc equivalents
 * to make porting of existing code much easier.
 *---------------------------------------------------------------------------*/
/* #defines */
#define CSL_RINGACC_MEM_OPS_TYPE_WR     CSL_LCDMA_RINGACC_MEM_OPS_TYPE_WR
#define CSL_RINGACC_MEM_OPS_TYPE_RD     CSL_LCDMA_RINGACC_MEM_OPS_TYPE_RD
/* typedefs */
#define CSL_RingaccRingMode             CSL_LcdmaRingaccRingMode
#define CSL_ringaccMemOpsFxnPtr         CSL_lcdma_ringaccMemOpsFxnPtr
#define CSL_RingAccMemoryOpsType        CSL_LcdmaRingAccMemoryOpsType
#define CSL_RingAccMonitorDataSrc       CSL_LcdmaRingAccMonitorDataSrc
#define CSL_RingAccMonitorType          CSL_LcdmaRingAccMonitorType
/* CSL-RL structures */
#define CSL_ringacc_flowRegs            CSL_lcdma_ringacc_txfcfgRegs
#define CSL_ringacc_rtRegs              CSL_lcdma_ringacc_txrtRegs
/* CSL-FL structures */
#define CSL_RingaccCfg                  CSL_LcdmaRingaccCfg
#define CSL_RingaccRingCfg              CSL_LcdmaRingaccRingCfg
/* API functions */
#define CSL_ringaccGetRevision          CSL_lcdma_ringaccGetRevision
#define CSL_ringaccInitCfg              CSL_lcdma_ringaccInitCfg
#define CSL_ringaccInitRingCfg          CSL_lcdma_ringaccInitRingCfg
#define CSL_ringaccInitRingObj          CSL_lcdma_ringaccInitRingObj
#define CSL_ringaccInitRing             CSL_lcdma_ringaccInitRing
#define CSL_ringaccSetEvent             CSL_lcdma_ringaccSetEvent
#define CSL_ringaccGetRingNum           CSL_lcdma_ringaccGetRingNum
#define CSL_ringaccSetRingOrderId       CSL_lcdma_ringaccSetRingOrderId
#define CSL_ringaccCfgRingCred          CSL_lcdma_ringaccCfgRingCred
#define CSL_ringaccResetRing            CSL_lcdma_ringaccResetRing
#define CSL_ringaccGetCmdRingPtr        CSL_lcdma_ringaccGetForwardRingPtr
#define CSL_ringaccGetRspRingPtr        CSL_lcdma_ringaccGetReverseRingPtr
#define CSL_ringaccSetRingDoorbell      CSL_lcdma_ringaccSetForwardDoorbell
#define CSL_ringaccSetRspRingDoorbell   CSL_lcdma_ringaccSetReverseDoorbell
#define CSL_ringaccCommitToCmdRing      CSL_lcdma_ringaccCommitToForwardRing
#define CSL_ringaccAckRspRing           CSL_lcdma_ringaccAckReverseRing
#define CSL_ringaccGetRingIdx           CSL_lcdma_ringaccGetForwardRingIdx
#define CSL_ringaccGetRingHwIdx         CSL_lcdma_ringaccGetReverseRingIdx
#define CSL_ringaccGetRingOcc           CSL_lcdma_ringaccGetForwardRingOcc
#define CSL_ringaccGetRingHwOcc         CSL_lcdma_ringaccGetReverseRingOcc
#define CSL_ringaccSetTraceEnable       CSL_lcdma_ringaccSetTraceEnable
#define CSL_ringaccEnableTrace          CSL_lcdma_ringaccEnableTrace
#define CSL_ringaccDisableTrace         CSL_lcdma_ringaccDisableTrace
#define CSL_ringaccCfgTrace             CSL_lcdma_ringaccCfgTrace
#define CSL_ringaccCfgRingMonitor       CSL_lcdma_ringaccCfgRingMonitor
#define CSL_ringaccReadRingMonitor      CSL_lcdma_ringaccReadRingMonitor
#define CSL_ringaccPush32               CSL_lcdma_ringaccPush32
#define CSL_ringaccPop32                CSL_lcdma_ringaccPop32
#define CSL_ringaccHwPop32              CSL_lcdma_ringaccHwPop32
#define CSL_ringaccPeek32               CSL_lcdma_ringaccPeek32
#define CSL_ringaccPush64               CSL_lcdma_ringaccPush64
#define CSL_ringaccPop64                CSL_lcdma_ringaccPop64
#define CSL_ringaccHwPop64              CSL_lcdma_ringaccHwPop64
#define CSL_ringaccPeek64               CSL_lcdma_ringaccPeek64
#define CSL_ringaccWrData               CSL_lcdma_ringaccWrData
#define CSL_ringaccRdData               CSL_lcdma_ringaccRdData
#define CSL_ringaccPeekData             CSL_lcdma_ringaccPeekData 

#endif
