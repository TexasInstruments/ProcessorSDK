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
// **********************************************************************************
// * FILE PURPOSE:  PA firmware for PHP PDSPs 
// **********************************************************************************
// * FILE NAME:  sadm.p
// *
// * DESCRIPTION: 
// *
// **********************************************************************************/
//
// /***************************************************************************
// * FILE PURPOSE: SA HPS - Data mode handling
// ****************************************************************************
// * FILE NAME: saac.c
// *
// * DESCRIPTION:
// *  This file contains functions for SA packet header processing of the  
// *  Data Mode packets 
// *
// * (C) Copyright 2011 Texas Instruments, Inc.
// *****************************************************************************/
//
// ***********************************************************************
// * Global scope information
// *
// *  Whenever a function makes use of R14-R21 (CDE window 2)
// *  the code will save an effected registers to the scratch
// *  area before use and restore after use.
// *  R0    |     General Purpose and local Variables
// *  R1    |
// *  R2    |
// *  R3    |
// *  R4      |   CDE and SCV Command/Status Registers
// *  R5      |
// *  R6       |  CDE Data Window (32-byte)
// *  R7       |  
// *  R8       |  Note: Can be used as general registers when
// *  R9       |        CDE (Packet Data) is not used
// *  R10      |
// *  R11      |
// *  R12      |
// *  R13      |
// *  R14       |  SCV (Security Context Viewer) Window (32-byte)
// *  R15       |
// *  R16       |
// *  R17       |    
// *  R18       |   
// *  R19       |   
// *  R20       |   
// *  R21       |   
// *  R22           |   Packet Instance Data
// *  R23           |
// *  R24           |  
// *  R25           |  
// *  R26           |       
// *  R27    |    PHP Context    
// *  R28    |          
// *  R29     |   Function arg and return values 
// *  R30      |  Return addresses (2 levels)
// *  R31    |  I/O Flags
// *
// **********************************************************************************

//*******************************************************************************
// * Local Data Structures
// ******************************************************************************/
//

.struct struct_dmPass1Loc1
    .u32 rsvd
.ends     

.enter dmPass1Scope
    .assign  struct_dmPass1Loc1,       R1,  R1,  dmPass1Loc1
    .assign  struct_dmPktInst,         R22, R22, dmPktInst      
.leave dmPass1Scope

.enter ctxDMScope
    .assign  struct_ctxDataMode,       R14, R16, ctxDM
.leave ctxDMScope

///******************************************************************************
// * Function:    sap_dm_proc_pass1
// ******************************************************************************
// * Description: This function perfroms the first pass of the Data Mode packet
// *              header processing in the from_air direction as the followings:  
// *
// *****************************************************************************/

phpDmProc_1:

    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using dmPass1Scope
    .using ctxDMScope
    
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm)<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxDM, SIZE(ctxDM)
    
    // clear Local Variables (may be just some of them)
    // zero    &dmPass1Loc1,    SIZE(dmPass1Loc1)    
    
    // Record packet instance index for PASS1 
    // mov     phpContext.ctrlInfo,  PktDesc.PktId
    mov     PktDesc.SA_NextEngId, ctxDM.firstEngId
    
    qbbc    phpDmProc_1_OtherChunk_L1,   phpContext.chunkInfo.chunkFlag_sop
    
 phpDmProc_1_FirstChunk_L1:    
 
    //clear the control flags and other variables
    zero    &dmPktInst,    SIZE(dmPktInst)
    
    //Update PktDesc for the packet
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    mov     PktDesc.SA_StatusLen, ctxDM.tagSize
    xout    XID_CDEDATA, PktDesc, 8  
   
    jmp     phpDmProc_1_End     
    
 phpDmProc_1_OtherChunk_L1:
    lbco    dmPktInst, cInstBase,  phpContext.instOffset, SIZE(dmPktInst)
    
    //Update PktDesc for the packet
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2  
    
    // Advance the window to the beginning of packet 
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    //xout    XID_CDECTRL, CdeCmd, 4      
    //mov     dmPass1Loc1.pktLoc,  0   // for non-SOP data chunk
    
    .leave pktDescScope
 
phpDmProc_1_End:
 
    // Save Instance Data
    sbco    dmPktInst, cInstBase, phpContext.instOffset, SIZE(dmPktInst)
    
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4

    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8)
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
    
    jmp     MainLoop
    
 
.leave dmPass1Scope
.leave ctxDMScope
.leave commCtxScope    

// ******************************************************************************
// * Function:    sap_dm_proc_pass2
// ******************************************************************************
// * Description: This function perfroms the second pass of the Data Mode packet
// *              header processing.  
// *  
// *    - ensure 4-byte alignment for non-EOP packets  
// *    - Update statistics
// *
// *****************************************************************************/
//
.struct struct_dmPass2Loc1
    .u8     psValidSize          
    .u8     pktId   
    .u16    chunkSize           // adjusted chunk size due to byte insertion
.ends     

.enter dmPass2Scope
    .assign  struct_dmPass2Loc1,       R1,  R1,  dmPass2Loc1
    .assign  struct_dmPktInst,         R22, R22, dmPktInst      
.leave dmPass2Scope

phpDmProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using dmPass2Scope
    .using ctxDMScope
    
    // clear Local Variables (may be just some of them)
    zero    &dmPass2Loc1,    SIZE(dmPass2Loc1)    
    
phpDmProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxDM, SIZE(ctxDM)

    // Record packet instance index and load the instance
    mov     dmPass2Loc1.psValidSize, PktDesc.PsValidSize
    mov     dmPass2Loc1.pktId,       PktDesc.PktId
    lbco    dmPktInst, cInstBase,    phpContext.instOffset, SIZE(dmPktInst)
    
    //
    // All packets should go through PHP for 4-byte alignement processing
    
    mov     PktDesc.SA_NextEngId, SAENG_OUT2_1
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 1
    mov     dmPass2Loc1.chunkSize,  phpContext.chunkSize 
    
    // Update swInfo1 at SOP
    qbbc    phpDmProc_2_L1_1, PktDesc.ChunkInfo.chunkFlag_sop
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
phpDmProc_2_L1_1:    
    
.leave pktDescScope 

    //qbeq    phpDmProc_2_L2_1,   dmPass2Loc1.psValidSize, 0
    // Advance to the PS Info section
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    //xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and store the Hash Data
    //xin     XID_CDEDATA,  r6,  16

phpDmProc_2_L2_1:
   
    // Advance the window to the packet location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Verify whether we need to insert leftover bytes when this chunk contains hash
    qbeq    phpDmProc_2_L2,  dmPktInst.numLeftoverBytes, 0
    
    // insert the leftover bytes
    add     dmPass2Loc1.chunkSize, dmPass2Loc1.chunkSize, dmPktInst.numLeftoverBytes
    mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_PACKET 
    mov     CdeCmd.Cmd0.b1, dmPktInst.numLeftoverBytes
    mov     CdeCmd.Cmd1.w2, dmPktInst.leftover_w2
    mov     CdeCmd.Cmd1.b1, dmPktInst.leftover_b1  
      
    xout    XID_CDECTRL, CdeCmd, 8   
    
phpDmProc_2_L2:    
 
    qbbc    phpDmProc_2_L3,  phpContext.chunkInfo.chunkFlag_eop
    //
    //  Process EOP Data Chunk
    //
    add     ctxDM.numPkts,   ctxDM.numPkts, 1
    adc     ctxDM.numPktsHi, ctxDM.numPktsHi, 0
    xout    XID_SCVDATA, ctxDM, SIZE(ctxDM)
    clr     phpContext.instMask, dmPass2Loc1.pktId
    jmp     phpDmProc_2_End
    
 phpDmProc_2_L3:
    //
    //  Process non-EOP data chunk
    // 
    
    // Should we record and remove unaligned bytes due to output stream interface problem
    and     dmPktInst.numLeftoverBytes,  dmPass2Loc1.chunkSize,    0x3 
    qbeq    phpDmProc_2_L4,  dmPktInst.numLeftoverBytes,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, phpContext.chunkSize,  dmPktInst.numLeftoverBytes
    xout    XID_CDECTRL, CdeCmd, 4   
    
    xin     XID_CDEDATA, r6,  3
    mov     dmPktInst.leftover_w2,   r6.w2
    mov     dmPktInst.leftover_b1,   r6.b1
     
    // flush out unaligned bytes
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    
phpDmProc_2_L4:          
    //  Save the instance Data
    sbco    dmPktInst, cInstBase, phpContext.instOffset, SIZE(dmPktInst)
    
 phpDmProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    mov     CdeCmd.Cmd0.b3, dmPass2Loc1.psValidSize
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop

.leave dmPass2Scope
.leave ctxDMScope
.leave commCtxScope    
 
        
