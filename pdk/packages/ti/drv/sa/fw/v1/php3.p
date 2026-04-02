//
//  TEXAS INSTRUMENTS TEXT FILE LICENSE
// 
//   Copyright (c) 2016 Texas Instruments Incorporated
// 
//  All rights reserved not granted herein.
//  
//  Limited License.  
// 
//  Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive 
//  license under copyrights and patents it now or hereafter owns or controls to 
//  make, have made, use, import, offer to sell and sell ("Utilize") this software 
//  subject to the terms herein.  With respect to the foregoing patent license, 
//  such license is granted  solely to the extent that any such patent is necessary 
//  to Utilize the software alone.  The patent license shall not apply to any 
//  combinations which include this software, other than combinations with devices 
//  manufactured by or for TI (“TI Devices”).  No hardware patent is licensed hereunder.
// 
//  Redistributions must preserve existing copyright notices and reproduce this license 
//  (including the above copyright notice and the disclaimer and (if applicable) source 
//  code license limitations below) in the documentation and/or other materials provided 
//  with the distribution.
//  
//  Redistribution and use in binary form, without modification, are permitted provided 
//  that the following conditions are met:
// 	No reverse engineering, decompilation, or disassembly of this software is 
//   permitted with respect to any software provided in binary form.
// 	Any redistribution and use are licensed by TI for use only with TI Devices.
// 	Nothing shall obligate TI to provide you with source code for the software 
//   licensed and provided to you in object code.
//  
//  If software source code is provided to you, modification and redistribution of the 
//  source code are permitted provided that the following conditions are met:
// 	Any redistribution and use of the source code, including any resulting derivative 
//   works, are licensed by TI for use only with TI Devices.
// 	Any redistribution and use of any object code compiled from the source code
//   and any resulting derivative works, are licensed by TI for use only with TI Devices.
// 
//  Neither the name of Texas Instruments Incorporated nor the names of its suppliers 
//  may be used to endorse or promote products derived from this software without 
//  specific prior written permission.
// 
//  DISCLAIMER.
// 
//  THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED 
//  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
//  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S 
//  LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
//  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// 
//
//
// Security Accelerator Packet Processing Example
//

#include "../pm_config.h"
#include "../pm_constants.h"
#include "../pdsp_subsys.h"
#include "../pdsp_psInfo2.h"
#include "../pdsp_ctx.h"
#include "../pdsp_proto.h"

//  *********************************************************************
//  *  Memory Layout
//  *    Scratch memory 1:  0x00:C000 -> 0x00:DFFF
//  *    Scratch memory 2:  0x00:E000 -> 0x00:EFFF 
//  *
//  *    Offset of the
//  *    Base address     Size        use                 Constant number, name
//  *        
//  *    0x0030           48          Dispatch Call table    C10  cCallTable
//  *    0x00b0           32          IV                     C11  cIvBuf
//  *    0x00e0           32          Auxiliary Data         C12  cAuxBuf
//  *    0x0100           256         Command Lables         C13  cCmdlBuf
//  *    0x0200           256         Statistics             C14  cStatsBase 
//  *    0x0300           64          ESP Tx Cmdl Call Table C16  cEspTxTbl
//  *    0x0340           64          ESP Rx Cmdl Call Table C17  cEspRxTbl
//  *    0x0380           64          AH Tx Cmdl Call Table  C18  cAhTxTbl
//  *    0x03C0           64          AH Rx Cmdl Call Table  C19  cAhTxTbl
//  *    0x0400           0x800       Instance Arrays        C15  cInstBase
//  *
//  ***********************************************************************

//  ***********************************************************************
//  * Global scope information
//  *
//  *  Whenever a function makes use of R14-R21 (CDE window 2)
//  *  the code will save an effected registers to the scratch
//  *  area before use and restore after use.
//  *  R0    |     General Purpose and local Variables
//  *  R1    |
//  *  R2    |
//  *  R3    |
//  *  R4      |   CDE and SCV Command/Status Registers
//  *  R5      |
//  *  R6       |  CDE Data Window (32-byte)
//  *  R7       |  
//  *  R8       |  Note: Can be used as general registers when
//  *  R9       |        CDE (Packet Data) is not used
//  *  R10      |
//  *  R11      |
//  *  R12      |
//  *  R13      |
//  *  R14       |  SCV (Security Context Viewer) Window (32-byte)
//  *  R15       |
//  *  R16       |
//  *  R17       |    
//  *  R18       |   
//  *  R19       |   
//  *  R20       |   
//  *  R21       |   
//  *  R22           |   Packet Instance Data
//  *  R23           |
//  *  R24           |  
//  *  R25           |  
//  *  R26           |
//  *  R27    |    PHP Context    
//  *  R28    |          
//  *  R29     |   Function arg and return values 
//  *  R30      |  Return addresses (2 levels)
//  *  R31    |  I/O Flags
//  *
//  **********************************************************************************

//===========================================================
//
// Global Variables
//
#define SASS_GEN2

// Define Engine ID
#define SAENG_PHP_1         SAENG_PHP3_1         
#define SAENG_PHP_2         SAENG_PHP3_2
#define SAENG_PHP_3         SAENG_PHP3_3
#define SAENG_ENCRYPT_1     SAENG_ENCRYPT2_1               
#define SAENG_ENCRYPT_2     SAENG_ENCRYPT2_2               
#define SAENG_AUTH_1        SAENG_AUTH2_1               
#define SAENG_AUTH_2        SAENG_AUTH2_2     

#define PHP_CMDL_BUF_ADDR   PHP3_CMDL_BUF_ADDR          
#define PHP_AUX_BUF_ADDR    PHP3_AUX_BUF_ADDR         
#define PHP_INST_BASE_ADDR  PHP3_INST_BASE_ADDR       
#define PHP_RN_FIFO_ADDR    PHP3_RN_FIFO_ADDR       
#define PHP_TEMP_BUF_ADDR   PHP3_TEMP_BUF_ADDR
          
//
// Register Allocation
//
// The CDE command registers are fixed
.assign struct_CdeCmd,         R4,  R5,     CdeCmd

// PHP Context
.assign struct_phpContext,     R27, R28,    phpContext

//===========================================================
//
// Scope Variables
//

// All XFR related structures should start on R6, R14, or R22
.enter  pktDescScope 
.assign struct_PktDesc,        R6, R13,     PktDesc     // Use R6 region
.assign struct_PktDesc2,       R6, R13,     PktDesc2    // Use R6 region
.leave  pktDescScope

// All Context Viewer structures should start on R14
.enter  commCtxScope
.assign struct_ctxComm,        R14, R19.b2,    ctxComm     // Security Context Common Portion
.leave  commCtxScope 

.enter replayCtrlScope
    .assign struct_replayCtrl,          R14, R21, replayCtrl 
.leave replayCtrlScope     

.enter protoInfoScope
    .assign  struct_protoInfo,          R6, R6,   protoInfo
    .assign  struct_payloadInfoShort,   R6, R7,   payloadInfoS
    .assign  struct_payloadInfoLong,    R6, R10,  payloadInfoL
.leave protoInfoScope

// Call Table: only used for zero out area
.enter   initScope
.assign struct_phpCalltable,       R4.w2, R15, callTable
.assign struct_cmdlFuncTable,      R4, R7, cmdlFuncTable    
.leave   initScope

//-------------------------------------------------------------------
//
// Code Starts
//
        .origin     0
        .entrypoint entry
        
entry:
        jmp     start
        
header:
#define HEADER_MAGIC   0xFACE0001                    // SASS php1
#define SASS_PHP1_VER  SASS_VERSION                  // 0x01.0x00.0x04.0x00
        .codeword  HEADER_MAGIC
        .codeword  SASS_PHP1_VER

.using  initScope

start:
        // Initialize the system context
        zero    &phpContext, SIZE(phpContext)
        
        // Store the PHP version number
        mov32   r0, SASS_PHP1_VER
        sbco    r0, cStats, phpVersionOffset, 4
        
        // Clear the system statistics
        zero    &r6, 64
        sbco    r6, cStats, 0, 64
        sbco    r6, cStats, 0x40, 64
        sbco    r6, cStats, 0x80, 64
        
        // Initialize the RN FIFO
        // call    RnFifo_Init    (PDSP2 only)
        
        // Initialize the IPSEC Command label Construction function
        zero    &cmdlFuncTable, SIZE(cmdlFuncTable)
        mov     cmdlFuncTable.func1, phpEspTxCmdlProc_Gen
        mov     cmdlFuncTable.func2, phpEspTxCmdlProc_GCM
        mov     cmdlFuncTable.func3, phpEspTxCmdlProc_GMAC
        mov     cmdlFuncTable.func4, phpEspTxCmdlProc_CCM
        sbco    cmdlFuncTable, cEspTxTbl, 0, SIZE(cmdlFuncTable)
        
        zero    &cmdlFuncTable, SIZE(cmdlFuncTable)
        mov     cmdlFuncTable.func1, phpEspRxCmdlProc_Gen
        mov     cmdlFuncTable.func2, phpEspRxCmdlProc_GCM
        mov     cmdlFuncTable.func3, phpEspRxCmdlProc_GMAC
        mov     cmdlFuncTable.func4, phpEspRxCmdlProc_CCM
        sbco    cmdlFuncTable, cEspRxTbl, 0, SIZE(cmdlFuncTable)
        
        zero    &cmdlFuncTable, SIZE(cmdlFuncTable)
        mov     cmdlFuncTable.func1, phpAhTxCmdlProc_Gen
        mov     cmdlFuncTable.func2, phpAhTxCmdlProc_GMAC
        sbco    cmdlFuncTable, cAhTxTbl, 0, SIZE(cmdlFuncTable)
        
        zero    &cmdlFuncTable, SIZE(cmdlFuncTable)
        mov     cmdlFuncTable.func1, phpAhRxCmdlProc_Gen
        mov     cmdlFuncTable.func2, phpAhRxCmdlProc_GMAC
        sbco    cmdlFuncTable, cAhRxTbl, 0, SIZE(cmdlFuncTable)

        //
        //
        // Start our main packet loop.
        //
        //


.leave initScope


//  **********************************************************************************
//  *  FUNCTION PURPOSE: Process a received data chunk
//  **********************************************************************************
//  *  DESCRIPTION: One data chunk has been received, perform the following actions
//  *               - record the common parameters
//  *               - allocate the packet instance for the SOP chunk of Pass 1 traffic
//  *               - Dipatch the data chunk based on its packet type, traffic
//  *                 direction and PHP Pass number
//  *
//  *********************************************************************************

MainLoop:
        //
        // If we have other stuff to do, we would do it here. Other stuff
        // may include background tasks like checking timers, or checking
        // for external commands. This example just does packet 
        // classification.
        //

        // Packet Processing

        //
        // If we don't have a new data chunk, jump to the end of the loop
        //
        qbbc    PacketDone, r31.tStatus_CDENewPacket    // Jump if there is no packet in CDE


        //
        // New Data chunk
        //
        // Process the data chunk based on its packet type, direction and PDSP destination
        //
NewPacket:
.using  pktDescScope
.using  commCtxScope 

        // Read packet descriptor
        xin     XID_CDEDATA, PktDesc, SIZE(PktDesc)
        
        // Store chunkSize and Chunk Info in global structure
        mov     phpContext.chunkSize,   PktDesc.ChunkSize
        mov     phpContext.chunkInfo,   PktDesc.ChunkInfo
        // Store the indicator of single-chunk packet (EOP & SOP)
        and     r0.b0, PktDesc.ChunkInfo, chunkFlag_pktIndMask
        
    CheckCommCtx:        
        // Read in the common context info  
        mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW
        xout    XID_SCVCTRL, CdeCmd, 4   
        xin     XID_SCVDATA, ctxComm, SIZE(struct_ctxComm)
        
        // Verify the security context 
        // TBD: It is a nice debug feature, but SwInfo1_ctxPtr_w0 is not vaild at non-EOP
        //      data chunk
        // qbne    Error1, PktDesc.SwInfo1_ctxPtr_w0, ctxComm.ctxId
        // Update thread ID if necessary
        qbbc    CheckCommCtx_1, ctxComm.pktInfo.flagUseLocDMA
            or      PktDesc.ThreadId,   PktDesc.ThreadId,   0x08
            xout    XID_CDEDATA, PktDesc.ThreadId, SIZE(PktDesc.ThreadId) 
            clr     ctxComm.pktInfo.flagUseLocDMA    
CheckCommCtx_1:
        
        // If the engine ID is for pass1, go to pass 1 code
        // (We check both engine codes so this example can run on either PHP)
        qbeq    CallTableOffset_Pass1, PktDesc.SA_NextEngId, SAENG_PHP_1
        qbeq    CallTableOffset_Pass2, PktDesc.SA_NextEngId, SAENG_PHP_2
        
        // Serious Hardware Error: It is impossible to reach here
        halt

    CallTableOffset_Pass2:
        // Check SA Error Flag
        qbne    ErrSAFlags_Pass2,   PktDesc.ChunkInfoPS, 0
        
        // Record packet instance offset
        lsl phpContext.instOffset, PktDesc.PktId, 8 
        // record the swInfo1 to be copied to packet descriptor at Pass2 SOP
        mov       r29,     ctxComm.swInfo1
        
        qbeq      CallTableOffset_Pass2_SC, r0.b0, chunkFlag_pktIndMask
        
    // Mult-chunk Pass2 operations     
        qbbc    CallTableOffset_Pass2_TX,  ctxComm.pktInfo.flagPktDir
    CallTableOffset_Pass2_RX:
        qbeq      CallTableOffset_phpAhRxProc_2,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_IPSEC_AH
        qbeq      CallTableOffset_phpEspRxProc_2,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_IPSEC_ESP
        jmp       ErrInvalidPktType
        
    CallTableOffset_Pass2_TX:
        qbeq      CallTableOffset_phpAhTxProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_IPSEC_AH
        qbeq      CallTableOffset_phpEspTxProc_2,  ctxComm.pktInfo, SA_CTX_PKT_TYPE_IPSEC_ESP
        jmp       ErrInvalidPktType
        
        
    CallTableOffset_Pass2_SC:
    //Single-chunk Pass2 operations
        qbbc    CallTableOffset_Pass2_SC_TX,  ctxComm.pktInfo.flagPktDir
    CallTableOffset_Pass2_SC_RX:
        qbeq      CallTableOffset_phpAhScRxProc_2,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_IPSEC_AH
        qbeq      CallTableOffset_phpEspScRxProc_2,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_IPSEC_ESP
        jmp       ErrInvalidPktType
        
    CallTableOffset_Pass2_SC_TX:
        qbeq      CallTableOffset_phpAhScTxProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_IPSEC_AH
        qbeq      CallTableOffset_phpEspScTxProc_2,  ctxComm.pktInfo, SA_CTX_PKT_TYPE_IPSEC_ESP
        jmp       ErrInvalidPktType

    CallTableOffset_Pass1:
        // Check SA Error Flags
        qbne    ErrSAFlags,  PktDesc.ChunkInfoPS, 0
    
        qbbs    CallTableOffset_Pass1_L1, phpContext.chunkInfo.chunkFlag_sop
        
        // Non-EOP chunk, check for packet discard condition
        qbeq    ErrNoPktInst_OtherChunk,  ctxComm.instIndex, 32 
               
        jmp     CallTableOffset_Pass1_Common 
    
    CallTableOffset_Pass1_L1:
        //  Update the destination Info for the first data chunk if it is not provided with packet
        qbbs    CallTableOffset_Pass1_L1_1,  PktDesc.SwInfo0.t30
        mov     PktDesc.FlowIndex,   ctxComm.flowIndex
        mov     PktDesc.DestQueue,   ctxComm.destQueueId
    CallTableOffset_Pass1_L1_1:    
        mov     PktDesc.SwInfo0,     ctxComm.swInfo0
        
        // Allocate the new instance buffer
        lmbd    ctxComm.instIndex, phpContext.instMask, 0
        xout    XID_SCVDATA, ctxComm.instIndex, 1
        qbeq    ErrNoPktInst, ctxComm.instIndex, 32 
        set     phpContext.instMask, ctxComm.instIndex
        
    CallTableOffset_Pass1_Common:
        // Calculate Instance Offset
        lsl     phpContext.instOffset, ctxComm.instIndex, 8
        // Store the packet Instance index in the packet
        mov     PktDesc.PktId, ctxComm.instIndex             
        xout    XID_CDEDATA, PktDesc.FlowIndex, 18 
        
        qbeq      CallTableOffset_Pass1_SC, r0.b0, chunkFlag_pktIndMask
   // Mult-chunk Pass1 operations     
        qbbc    CallTableOffset_Pass1_TX,  ctxComm.pktInfo.flagPktDir
   CallTableOffset_Pass1_RX:
        qbeq      CallTableOffset_phpAhRxProc_1,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_IPSEC_AH
        qbeq      CallTableOffset_phpEspRxProc_1,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_IPSEC_ESP
        jmp       ErrInvalidPktType
        
   CallTableOffset_Pass1_TX:
        qbeq      CallTableOffset_phpAhTxProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_IPSEC_AH
        qbeq      CallTableOffset_phpEspTxProc_1,  ctxComm.pktInfo, SA_CTX_PKT_TYPE_IPSEC_ESP
        jmp       ErrInvalidPktType
        
   CallTableOffset_Pass1_SC: 
   // Single-chunk Pass1 operations     
        qbbc    CallTableOffset_Pass1_SC_TX,  ctxComm.pktInfo.flagPktDir
   CallTableOffset_Pass1_SC_RX:
        qbeq      CallTableOffset_phpAhScRxProc_1,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_IPSEC_AH
        qbeq      CallTableOffset_phpEspScRxProc_1,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_IPSEC_ESP
        jmp       ErrInvalidPktType
        
   CallTableOffset_Pass1_SC_TX:
        qbeq      CallTableOffset_phpAhScTxProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_IPSEC_AH
        qbeq      phpEspScTxProc_1,  ctxComm.pktInfo, SA_CTX_PKT_TYPE_IPSEC_ESP
        jmp       ErrInvalidPktType
          
        
// Mult-chunk processing routines         
CallTableOffset_phpEspRxProc_1:
        jmp phpEspRxProc_1
CallTableOffset_phpAhRxProc_1: 
        jmp phpAhRxProc_1
CallTableOffset_phpEspTxProc_1:
        jmp phpEspTxProc_1
CallTableOffset_phpAhTxProc_1: 
        jmp phpAhTxProc_1       

CallTableOffset_phpEspRxProc_2:
        jmp phpEspRxProc_2
CallTableOffset_phpAhRxProc_2: 
        jmp phpAhRxProc_2
CallTableOffset_phpEspTxProc_2: 
        jmp phpEspTxProc_2
CallTableOffset_phpAhTxProc_2:  
        jmp phpAhTxProc_2
        
// Single-chunk processing routines         
CallTableOffset_phpEspScRxProc_1:
        jmp phpEspScRxProc_1
CallTableOffset_phpAhScRxProc_1: 
        jmp phpAhScRxProc_1
CallTableOffset_phpAhScTxProc_1: 
        jmp phpAhScTxProc_1       

CallTableOffset_phpEspScRxProc_2:
        jmp phpEspScRxProc_2
CallTableOffset_phpAhScRxProc_2: 
        jmp phpAhScRxProc_2
CallTableOffset_phpEspScTxProc_2: 
        jmp phpEspScTxProc_2
CallTableOffset_phpAhScTxProc_2:  
        jmp phpAhScTxProc_2

         // This is an error. The secturity context was not as expected           
Error1:
        halt
        
ErrSAFlags:
        // SA Error Flags
        qbbc    ErrSAFlags_OtherChunk, phpContext.chunkInfo.chunkFlag_sop
        
        // Update Satistics
        lbco    r0, cStats, PDSP_ERR_CTX_OFFSET, 4
        add     r0, r0, 1
        sbco    r0, cStats, PDSP_ERR_CTX_OFFSET, 4
        
ErrSAFlags_OtherChunk:
        mov     r0, PktDesc.ChunkInfoPS
        sbco    r0, cStats, 0x10, 4
        
   // Skip pdsp halt if debug is not enabled
   qbbc SkipPdspHalt_8, phpContext.ctrlInfo.t_php_dbg_halt
        jmp ErrPrintScCxt 
SkipPdspHalt_8:        
        mov   PktDesc.SA_NextEngId,   SAENG_OUT1_1
        jmp   ErrCommon
        
ErrSAFlags_Pass2:
        // SA Error Flags
        qbbc    ErrSAFlags_Pass2_OtherChunk, phpContext.chunkInfo.chunkFlag_sop
        
        // Update Satistics
        lbco    r0, cStats, PDSP_ERR_ENGINE_OFFSET, 4
        add     r0, r0, 1
        sbco    r0, cStats, PDSP_ERR_ENGINE_OFFSET, 4
        
        // Clear the instance 
        clr     phpContext.instMask, PktDesc.PktId                 
        
ErrSAFlags_Pass2_OtherChunk:
        mov     r0, PktDesc.ChunkInfoPS
        sbco    r0, cStats, 0x14, 4
        
   // Skip pdsp halt if debug is not enabled
   qbbc SkipPdspHalt_9, phpContext.ctrlInfo.t_php_dbg_halt
        jmp ErrPrintScCxt 
SkipPdspHalt_9:        
        mov   PktDesc.SA_NextEngId,   SAENG_OUT1_1
        jmp   ErrCommon
        
        // No Pkt Instance available
ErrNoPktInst:
        // Update Error Statistics 
        lbco    r0, cStats, PDSP_ERR_NOMEM_OFFSET, 4
        add     r0, r0, 1
        sbco    r0, cStats, PDSP_ERR_NOMEM_OFFSET, 4
        
   // Skip pdsp halt if debug is not enabled
   qbbc SkipPdspHalt_10, phpContext.ctrlInfo.t_php_dbg_halt
        halt 
SkipPdspHalt_10:        


ErrNoPktInst_OtherChunk:
        mov   PktDesc.SA_NextEngId,   SAENG_OUT1_1
        jmp   ErrCommon
        
        // Invalid Packet type in the security context
ErrInvalidPktType: 
        qbbc  ErrInvalidPktTypes_OtherChunk, phpContext.chunkInfo.chunkFlag_sop
        // Clear the instance 
        clr   phpContext.instMask, PktDesc.PktId                 

ErrInvalidPktTypes_OtherChunk:  
   // Skip pdsp halt if debug is not enabled
   qbbc SkipPdspHalt_11, phpContext.ctrlInfo.t_php_dbg_halt
        jmp ErrPrintScCxt 
SkipPdspHalt_11:        
        mov   PktDesc.SA_NextEngId,   SAENG_OUT1_1
        // Pass through
        //jmp   ErrCommon    
        
        // Common Error Processing
ErrCommon:
        set   PktDesc.ChunkInfo.chunkFlag_discard
        xout  XID_CDEDATA, PktDesc, SIZE(PktDesc)
        
        // Hardware workaround: avoid non-4byte alignment by flushing out unaligned bytes
        qbbs  ErrCommon_L1,  phpContext.chunkInfo.chunkFlag_eop
        and   CdeCmd.Cmd0.w2,phpContext.chunkSize,    0x3 
        qbeq  ErrCommon_L1,  CdeCmd.Cmd0.w2,  0 
        
        mov   CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
        xout  XID_CDECTRL, CdeCmd, 4
        
        mov   CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
        xout  XID_CDECTRL, CdeCmd, 4

ErrCommon_L1:
        //close the security context
        mov     CdeCmd.Cmd0, SCV_CMD_DONE
        xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
        // Send this packet 
        mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE  
        xout    XID_CDECTRL, CdeCmd, 4                  // Send the command    
           
        jmp MainLoop
        
PacketDone:
        jmp     MainLoop
        
ErrPrintScCxt:
        // Store the security context
        sbco    r14,  cCmdlBuf, 0x80, 32

        // Read in the common context info  
        mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW  
        xout    XID_SCVCTRL, CdeCmd, 4   
        xin     XID_SCVDATA, r14,   32
        sbco    r14,  cCmdlBuf, 0xa0, 32
        
        mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (0x20 << 8) 
        xout    XID_SCVCTRL, CdeCmd, 4   
        xin     XID_SCVDATA, r14,   32
        sbco    r14,  cCmdlBuf, 0xc0, 32
        
        mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (0x40 << 8) 
        xout    XID_SCVCTRL, CdeCmd, 4   
        xin     XID_SCVDATA, r14,   32
        sbco    r14,  cCmdlBuf, 0xe0, 32

        halt
        
.leave  pktDescScope
.leave  commCtxScope   
     
phpNullProc:
        jmp     MainLoop

#include "../saipsec_esp_tx2.p"
#include "../saipsec_esp_rx2.p"
#include "../saipsec_esp_tx.p"
#include "../saipsec_esp_rx.p"
#include "../saipsec_util.p"
#include "../saipsec_ah_tx2.p"
#include "../saipsec_ah_rx2.p"
#include "../saipsec_ah_tx.p"
#include "../saipsec_ah_rx.p"

