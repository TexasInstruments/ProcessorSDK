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
//  *  R6       |  CDE Data Window (32-byte) also used for random number insertion
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
#define SAENG_PHP_1         SAENG_PHP2_1         
#define SAENG_PHP_2         SAENG_PHP2_2
#define SAENG_PHP_3         SAENG_PHP2_3
#define SAENG_ENCRYPT_1     SAENG_ENCRYPT1_1               
#define SAENG_ENCRYPT_2     SAENG_ENCRYPT1_2               
#define SAENG_AUTH_1        SAENG_AUTH1_1               
#define SAENG_AUTH_2        SAENG_AUTH1_2   

#define PHP_CMDL_BUF_ADDR   PHP2_CMDL_BUF_ADDR          
#define PHP_AUX_BUF_ADDR    PHP2_AUX_BUF_ADDR         
#define PHP_INST_BASE_ADDR  PHP2_INST_BASE_ADDR       
#define PHP_RN_FIFO_ADDR    PHP2_RN_FIFO_ADDR       
#define PHP_TEMP_BUF_ADDR   PHP2_TEMP_BUF_ADDR

//
// Register Allocation
//
// The CDE command registers are fixed
.assign struct_CdeCmd,         R4,  R5,     CdeCmd
.assign struct_acDbgCxt,       R26.b3, R26.b1, s_acDbg

// PHP Context
.assign struct_phpContext2,    R27, R28,    phpContext

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
    .assign  struct_payloadInfoShort2,  R6, R11,  payloadInfoS2
    .assign  struct_payloadInfoLong,    R6, R10,  payloadInfoL
.leave protoInfoScope

.enter  rnFifoInScope
    .assign struct_saRnFifoCb,          R13, R13, rnFifoInCb
.leave  rnFifoInScope

//-------------------------------------------------------------------
//
// Code Starts
//
        .origin     0
        .entrypoint entry
        
entry:
        jmp     start
        
header:
#define HEADER_MAGIC   0xFACE0002                    // SASS php2
#define SASS_PHP2_VER  SASS_VERSION                  // 0x01.0x00.0x04.0x00
        .codeword  HEADER_MAGIC
        .codeword  SASS_PHP2_VER

start:
        // Initialize the system context
        zero    &phpContext, SIZE(phpContext)
        
        // Store the PHP version number
        mov32   r0, SASS_PHP2_VER
        sbco    r0, cStats, phpVersionOffset, 4
        
        // Clear the system statistics
        zero    &r6, 64
        sbco    r6, cStats, 0, 64
        sbco    r6, cStats, 0x40, 64
        sbco    r6, cStats, 0x80, 64

        // Clear Air Cipher Debug Information (1k size)
        mov     s_acDbg.index, 0
acDbgInit_0:  
       lsl      s_acDbg.offset, s_acDbg.index, 6
       // sbco     &r6, cAcDbgBase, s_acDbgOffset,  64
       mov      r0,  PDSP_ACDBG_BASE
       sbbo     &r6, r0, s_acDbg.offset,  64
       add      s_acDbg.index, s_acDbg.index, 1
       qbne     acDbgInit_0, s_acDbg.index,  16
       sub      s_acDbg.index, s_acDbg.index, 1
        
        //Initialize the RN FIFO  (PHP2 only)
        call    RnFifo_Init

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
        
        // Random Number Check
.using  rnFifoInScope      
     
    rnInsert_L1:    
        mov     r1, PHP1_RN_CTRL_ADDR    
        lbbo    rnFifoInCb,  r1, 0, SIZE(rnFifoInCb)
        // Verify whether FIFO is full
        qbeq    rnProcPhp3, rnFifoInCb.in, rnFifoInCb.out 
            // Is the new random number ready?
            lbco    r6, cRngBase, RNG_REG_STAT, 4
            qbbc    procPkt_L1, r6, 0
            
                mov     r2, PHP1_RN_FIFO_ADDR
                
                // Get and store 8-byte random number
                lbco    r6, cRngBase, RNG_REG_OUT_0, 8
                sbbo    r6, r2, rnFifoInCb.in, 8
                mov     r0, 1
                sbco    r0, cRngBase, RNG_REG_INTACK, 4   // Trigger TRNG to generate next random number
                add     rnFifoInCb.in, rnFifoInCb.in, 8
                
                qbbc    rnInsert_L2, rnFifoInCb.in, 9   //phpRnFifoSize = 0x200 (b9)  
                    mov rnFifoInCb.in,  0     
            
    rnInsert_L2:
        sbbo    rnFifoInCb.in,  r1, OFFSET(rnFifoInCb.in), SIZE(rnFifoInCb.in)
        jmp     procPkt_L1
        
    rnProcPhp3:
        mov     r1, PHP3_RN_CTRL_ADDR    
        
    rnInsert_L3:     
        lbbo    rnFifoInCb,  r1, 0, SIZE(rnFifoInCb)
        // Verify whether FIFO is full
        qbeq    procPkt_L1, rnFifoInCb.in, rnFifoInCb.out 
            // Is the new random number ready?
            lbco    r6, cRngBase, RNG_REG_STAT, 4
            qbbc    procPkt_L1, r6, 0
            
                mov     r2, PHP3_RN_FIFO_ADDR
                // Get and store 8-byte random number
                lbco    r6, cRngBase, RNG_REG_OUT_0, 8
                sbbo    r6, r2, rnFifoInCb.in, 8
                mov     r0, 1
                sbco    r0, cRngBase, RNG_REG_INTACK, 4   // Trigger TRNG to generate next random number
                add     rnFifoInCb.in, rnFifoInCb.in, 8
                
                qbbc    rnInsert_L4, rnFifoInCb.in, 9   //phpRnFifoSize = 0x200 (b9)  
                    mov rnFifoInCb.in,  0     
            
    rnInsert_L4:
        sbbo    rnFifoInCb.in,  r1, OFFSET(rnFifoInCb.in), SIZE(rnFifoInCb.in)
        
                             
        // pass through
.leave  rnFifoInScope
        
        
    procPkt_L1:        

        // Packet Processing

        //
        // If we do not have a new data chunk, jump to the end of the loop
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
        
        // Discard check: discard the subsequent data chunks 
        // TBD: discard packet should be done at non-sop data chunk 
        //      when local instance is available
        // qbbc    CheckCommCtx,  phpContext.flags.pkt_discard
        // qbbc    MainLoop,      PktDesc.ChunkInfo.chunkFlag_sop
        // clr     phpContext.flags.pkt_discard
        
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
        
    CallTableOffset_1:  // (pass || dir || packet_type) << 1   
        // Extract the packet type
        // and     r0.b0,  ctxComm.pktInfo, CTX_PKT_TYPE_MASK
        // qblt    ErrInvalidPktType,  r0.b0,  SA_CTX_MAX_PKT_TYPE
        
        // qbbc    CallTableOffset_2,  ctxComm.pktInfo.flagPktDir
        // set     r0.b0,  2
        
    CallTableOffset_2:        
         
        // If the engine ID is for pass1, go to pass 1 code
        qbeq    CallTableOffset_Pass1, PktDesc.SA_NextEngId, SAENG_PHP2_1
        qbeq    CallTableOffset_Pass2, PktDesc.SA_NextEngId, SAENG_PHP2_2
        qbeq    CallTableOffset_Pass3, PktDesc.SA_NextEngId, SAENG_PHP2_3
        
        // Serious Hardware Error: It is impossible to reach here
        halt

    CallTableOffset_Pass2:
        // Check SA Error Flag
        qbne    ErrSAFlags_Pass2,   PktDesc.ChunkInfoPS, 0
    
        // Record packet instance offset
        lsl phpContext.instOffset, PktDesc.PktId, 7
        // record the swInfo1 to be copied to packet descriptor at Pass2 SOP
        mov       r29,     ctxComm.swInfo1

        qbeq      CallTableOffset_Pass2_SC, r0.b0, chunkFlag_pktIndMask
        
    // Mult-chunk Pass2 operations
        qbbc    CallTableOffset_Pass2_TX,  ctxComm.pktInfo.flagPktDir
    CallTableOffset_Pass2_RX:
        qbeq      CallTableOffset_phpSrtpRxProc_2,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_SRTP
        qbeq      CallTableOffset_phpAcRxProc_2,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_3GPP_AIR
        qbeq      CallTableOffset_phpDmProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_DATA_MODE
        jmp       ErrInvalidPktType
        
    CallTableOffset_Pass2_TX:
        //qbeq      CallTableOffset_phpAhTxProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_IPSEC_AH
        //qbeq      CallTableOffset_phpEspTxProc_2,  ctxComm.pktInfo, SA_CTX_PKT_TYPE_IPSEC_ESP
        qbeq      CallTableOffset_phpSrtpTxProc_2,  ctxComm.pktInfo,SA_CTX_PKT_TYPE_SRTP
        qbeq      CallTableOffset_phpAcTxProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_3GPP_AIR
        qbeq      CallTableOffset_phpDmProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_DATA_MODE
        jmp       ErrInvalidPktType
        
    CallTableOffset_Pass2_SC:
    //Single-chunk Pass2 operations
        qbbc    CallTableOffset_Pass2_SC_TX,  ctxComm.pktInfo.flagPktDir
    CallTableOffset_Pass2_SC_RX:
        qbeq      CallTableOffset_phpSrtpScRxProc_2,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_SRTP
        qbeq      CallTableOffset_phpAcRxProc_2,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_3GPP_AIR
        qbeq      CallTableOffset_phpDmProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_DATA_MODE
        jmp       ErrInvalidPktType
        
    CallTableOffset_Pass2_SC_TX:
        //qbeq      CallTableOffset_phpAhScTxProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_IPSEC_AH
        //qbeq      CallTableOffset_phpEsScpTxProc_2,  ctxComm.pktInfo, SA_CTX_PKT_TYPE_IPSEC_ESP
        qbeq      CallTableOffset_phpSrtpScTxProc_2,  ctxComm.pktInfo,SA_CTX_PKT_TYPE_SRTP
        qbeq      CallTableOffset_phpAcTxProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_3GPP_AIR
        qbeq      CallTableOffset_phpDmProc_2,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_DATA_MODE
        jmp       ErrInvalidPktType
        
    CallTableOffset_Pass3:
        // Check SA Error Flag
        qbne    ErrSAFlags_Pass2,   PktDesc.ChunkInfoPS, 0
    
        // Record packet instance offset
        lsl phpContext.instOffset, PktDesc.PktId, 7 
        // record the swInfo1 to be copied to packet descriptor at Pass3 SOP
        mov     r29,     ctxComm.swInfo1
        
        // Mult-chunk Pass3 operations   
        // only  Air-Ciphering from Air function 
        CallTableOffset_Pass3_TX:
        qbeq      CallTableOffset_phpAcTxProc_3,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_3GPP_AIR
        jmp       ErrInvalidPktType

    CallTableOffset_Pass1:
        // Check SA Error Flags
        qbne    ErrSAFlags,  PktDesc.ChunkInfoPS, 0

     // Skip logging if not enabled
     qbbc SkipAcDbgLog_0, phpContext.ctrlInfo.t_php_pkt_dbg_info
        // Initialize the debug write offset, prepare the offset to write the dbg info
        add     s_acDbg.index, s_acDbg.index, 1
        and     s_acDbg.index, s_acDbg.index, 15
        lsl     s_acDbg.offset, s_acDbg.index, 6

        // Store the Software Info0/1 (first info)
        // sbco    PktDesc.SwInfo0, cAcDbgBase, s_acDbg.offset,  8
        mov      r0,  PDSP_ACDBG_BASE
        sbbo    PktDesc.SwInfo0, r0, s_acDbg.offset,  8
        add     s_acDbg.offset,   s_acDbg.offset,              (4+4+2)
SkipAcDbgLog_0:        
        
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

     // Skip logging if not enabled
     qbbc SkipAcDbgLog_1, phpContext.ctrlInfo.t_php_pkt_dbg_info
        // Store the Third info
        // sbco    ctxComm.instIndex, cAcDbgBase, s_acDbg.offset,   1
        mov      r0,  PDSP_ACDBG_BASE
        sbbo    ctxComm.instIndex, r0, s_acDbg.offset,   1
        add     s_acDbg.offset,   s_acDbg.offset,                 1

        // sbco    phpContext.instMask, cAcDbgBase, s_acDbg.offset, 1
        sbbo    phpContext.instMask, r0, s_acDbg.offset, 1
        add     s_acDbg.offset,   s_acDbg.offset,               (4-3+2)

        // sbco    PktDesc.SA_FullSize, cAcDbgBase, s_acDbg.offset, 2
        sbbo    PktDesc.SA_FullSize, r0, s_acDbg.offset, 2
        add     s_acDbg.offset,   s_acDbg.offset,                (4-2)

        // store the allocation count 
        lbco    r0, cStats, PDSP_MEM_ALLOC_CNT, 4
        add     r0, r0,     1
        sbco    r0, cStats, PDSP_MEM_ALLOC_CNT, 4
SkipAcDbgLog_1:        

    CallTableOffset_Pass1_Common:
        // Calculate Instance Offset
        lsl     phpContext.instOffset, ctxComm.instIndex, 7
        // Store the packet Instance index in the packet
        mov     PktDesc.PktId, ctxComm.instIndex
        xout    XID_CDEDATA, PktDesc.FlowIndex, 18 

        qbeq      CallTableOffset_Pass1_SC, r0.b0, chunkFlag_pktIndMask
   // Mult-chunk Pass1 operations     
        qbbc    CallTableOffset_Pass1_TX,  ctxComm.pktInfo.flagPktDir
    CallTableOffset_Pass1_RX:
        qbeq      CallTableOffset_phpSrtpRxProc_1,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_SRTP
        qbeq      CallTableOffset_phpAcRxProc_1,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_3GPP_AIR
        qbeq      CallTableOffset_phpDmProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_DATA_MODE
        jmp       ErrInvalidPktType

    CallTableOffset_Pass1_TX:
        //qbeq      CallTableOffset_phpAhTxProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_IPSEC_AH
        //qbeq      phpEspTxProc_1,  ctxComm.pktInfo, SA_CTX_PKT_TYPE_IPSEC_ESP
        qbeq      CallTableOffset_phpSrtpTxProc_1,  ctxComm.pktInfo,SA_CTX_PKT_TYPE_SRTP
        qbeq      CallTableOffset_phpAcTxProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_3GPP_AIR
        qbeq      CallTableOffset_phpDmProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_DATA_MODE
        jmp       ErrInvalidPktType

   CallTableOffset_Pass1_SC: 
   // Single-chunk Pass1 operations     
        qbbc    CallTableOffset_Pass1_SC_TX,  ctxComm.pktInfo.flagPktDir
    CallTableOffset_Pass1_SC_RX:
        qbeq      CallTableOffset_phpSrtpScRxProc_1,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_SRTP
        qbeq      CallTableOffset_phpAcRxProc_1,  ctxComm.pktInfo, CTX_PKT_DIR_RX|SA_CTX_PKT_TYPE_3GPP_AIR
        qbeq      CallTableOffset_phpDmProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_DATA_MODE
        jmp       ErrInvalidPktType

    CallTableOffset_Pass1_SC_TX:
        //qbeq      CallTableOffset_phpAhTxProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_IPSEC_AH
        //qbeq      phpEspTxProc_1,  ctxComm.pktInfo, SA_CTX_PKT_TYPE_IPSEC_ESP
        qbeq      CallTableOffset_phpSrtpScTxProc_1,  ctxComm.pktInfo,SA_CTX_PKT_TYPE_SRTP
        qbeq      CallTableOffset_phpAcTxProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_3GPP_AIR
        qbeq      CallTableOffset_phpDmProc_1,  ctxComm.pktInfo,  SA_CTX_PKT_TYPE_DATA_MODE
        jmp       ErrInvalidPktType

// Mult-chunk processing routines
CallTableOffset_phpSrtpRxProc_1:
        jmp phpSrtpRxProc_1
CallTableOffset_phpAcRxProc_1: 
        jmp phpAcToAirProc_1 
//CallTableOffset_phpAhTxProc_1: 
//        jmp phpAhTxProc_1       
CallTableOffset_phpSrtpTxProc_1:
        jmp phpSrtpTxProc_1         
CallTableOffset_phpAcTxProc_1:
        jmp phpAcFromAirProc_1
CallTableOffset_phpDmProc_1:
        jmp phpDmProc_1

CallTableOffset_phpSrtpRxProc_2:
        jmp phpSrtpRxProc_2
CallTableOffset_phpAcRxProc_2:
        jmp phpAcToAirProc_2

//CallTableOffset_phpEspTxProc_2: 
//        jmp phpEspTxProc_2
//CallTableOffset_phpAhTxProc_2:  
//        jmp phpAhTxProc_2
CallTableOffset_phpSrtpTxProc_2:  
        jmp phpSrtpTxProc_2
CallTableOffset_phpAcTxProc_2: 
        jmp phpAcFromAirProc_2 
CallTableOffset_phpDmProc_2:
        jmp phpDmProc_2
CallTableOffset_phpAcTxProc_3: 
        jmp phpAcFromAirProc_3 
        
// Single-chunk processing routines         
CallTableOffset_phpSrtpScRxProc_1:
        jmp phpSrtpScRxProc_1
//CallTableOffset_phpAhScTxProc_1: 
//        jmp phpAhScTxProc_1       
CallTableOffset_phpSrtpScTxProc_1:
        jmp phpSrtpScTxProc_1         

CallTableOffset_phpSrtpScRxProc_2:
        jmp phpSrtpScRxProc_2

//CallTableOffset_phpEspScTxProc_2: 
//        jmp phpEspScTxProc_2
//CallTableOffset_phpAhScTxProc_2:  
//        jmp phpAhScTxProc_2
CallTableOffset_phpSrtpScTxProc_2:  
        jmp phpSrtpScTxProc_2

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
   qbbc SkipPdspHalt_4, phpContext.ctrlInfo.t_php_dbg_halt
        jmp ErrPrintScCxt 
SkipPdspHalt_4:        
        mov   PktDesc.SA_NextEngId,   SAENG_OUT2_1
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
        // store the free count 
        lbco    r0, cStats, PDSP_MEM_ERR_ENGINE_FREE_CNT, 4
        add     r0, r0,     1
        sbco    r0, cStats, PDSP_MEM_ERR_ENGINE_FREE_CNT, 4        
        
ErrSAFlags_Pass2_OtherChunk:
        mov     r0, PktDesc.ChunkInfoPS
        sbco    r0, cStats, 0x14, 4
        
   // Skip pdsp halt if debug is not enabled
   qbbc SkipPdspHalt_5, phpContext.ctrlInfo.t_php_dbg_halt
        jmp ErrPrintScCxt 
SkipPdspHalt_5:        
        mov   PktDesc.SA_NextEngId,   SAENG_OUT2_1
        jmp   ErrCommon
        
        // No Pkt Instance available
ErrNoPktInst:
        // Update Error Statistics   
        lbco    r0, cStats, PDSP_ERR_NOMEM_OFFSET, 4
        add     r0, r0, 1
        sbco    r0, cStats, PDSP_ERR_NOMEM_OFFSET, 4

   // Skip pdsp halt if debug is not enabled
   qbbc SkipPdspHalt_6, phpContext.ctrlInfo.t_php_dbg_halt
        halt 
SkipPdspHalt_6:        

ErrNoPktInst_OtherChunk:
        mov   PktDesc.SA_NextEngId,   SAENG_OUT2_1
        jmp   ErrCommon
        
        // Invalid Packet type in the security context
ErrInvalidPktType: 
        qbbc  ErrInvalidPktTypes_OtherChunk, phpContext.chunkInfo.chunkFlag_sop
        // Clear the instance 
        clr   phpContext.instMask, PktDesc.PktId 
        // store the free count 
        lbco    r0, cStats, PDSP_MEM_ERR_INV_PKT_TYPE_CNT, 4
        add     r0, r0,     1
        sbco    r0, cStats, PDSP_MEM_ERR_INV_PKT_TYPE_CNT, 4           

ErrInvalidPktTypes_OtherChunk:  
   // Skip pdsp halt if debug is not enabled
   qbbc SkipPdspHalt_7, phpContext.ctrlInfo.t_php_dbg_halt
        jmp ErrPrintScCxt 
SkipPdspHalt_7:        

        mov   PktDesc.SA_NextEngId,   SAENG_OUT2_1
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
        
        // pass through      
        //jmp MainLoop
        
        
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

#include "../sasrtp_tx2.p"
#include "../sasrtp_rx2.p"
#include "../sasrtp_tx.p"
#include "../sasrtp_rx.p"
#include "../saipsec_util.p"
#include "../sasrtp_util.p"
#include "../saac_fromair.p"
#include "../saac_toair.p"
#include "../saac_util.p"
#include "../sadm.p"
