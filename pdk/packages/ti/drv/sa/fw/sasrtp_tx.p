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
//  **********************************************************************************
//  * FILE PURPOSE:  SA firmware for PHP PDSPs 
//  **********************************************************************************
//  * FILE NAME:  sasrtp_tx.p
//  *
//  * DESCRIPTION: Parses a packet in SRTP Tx mode
//  *
//  **********************************************************************************/
//  /***************************************************************************
//  * FILE PURPOSE: SA HPS - SRTP mode handling
//  ****************************************************************************
//  * FILE NAME: sasrtp.c
//  *
//  * DESCRIPTION:
//  *  This file contains functions for SA packet header processing of the SRTP 
//  *  packets
//  *
//  *  The format of an SRTP packet is illustrated in Figure 1.
//  *
//  *        0                   1                   2                   3
//  *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//  *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+<+
//  *     |V=2|P|X|  CC   |M|     PT      |       sequence number         | |
//  *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |
//  *     |                           timestamp                           | |
//  *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |
//  *     |           synchronization source (SSRC) identifier            | |
//  *     +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ |
//  *     |            contributing source (CSRC) identifiers             | |
//  *     |                               ....                            | |
//  *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |
//  *     |                   RTP extension (OPTIONAL)                    | |
//  *   +>+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |
//  *   | |                          payload  ...                         | |
//  *   | |                               +-------------------------------+ |
//  *   | |                               | RTP padding   | RTP pad count | |
//  *   +>+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+<+
//  *   | ~                     SRTP MKI (OPTIONAL)                       ~ |
//  *   | +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |
//  *   | :                 authentication tag (RECOMMENDED)              : |
//  *   | +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |
//  *   |                                                                   |
//  *   +- Encrypted Portion*                      Authenticated Portion ---+
//  *
//  *           Figure 1.  The format of an SRTP packet.  
//  *
//  *   *Encrypted Portion is the
//  *    same size as the plaintext for the Section 4 pre-defined transforms.
//  *
//  *   Note:Padding is not required for mandatory modes 
//  *
//  *
//  * (C) Copyright 2009 Texas Instruments, Inc.
//  *****************************************************************************/
//
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
//  *  R26                  
//  *  R27    |    PHP Context    
//  *  R28    |          
//  *  R29     |   Function arg and return values 
//  *  R30      |  Return addresses (2 levels)
//  *  R31    |  I/O Flags
//  *
//  **********************************************************************************

//*******************************************************************************
// * Local Data Structures
// ******************************************************************************/
//

//.struct struct_srtpTxPass1Loc1
//    .u16    hashDataLen        // Hash Data length (for command label)
//    .u16    encryptedDataLen   // Encrypted Data length (for command label)
//    .u8     protoOffset        // Protocol Offset
//    .u8     rtpHdrSize         // RTP Header Size (for command label)
//    .u8     cmdlOffset         // Command Label Offset 
//    .u8     mkiHashSize        // MKI Size plus Hash Size
//.ends     
//
//.enter srtpTxPass1Scope
//    .assign  struct_srtpTxPass1Loc1,  R2,  R3,  srtpTxPass1Loc1
//    .assign  struct_srtpPktInst,      R22, R25, srtpPktInst      
//.leave srtpTxPass1Scope
//
//.enter ctxSRTPTxScope
//    .assign  struct_ctxSRTPTxStats,   R14, R15, ctxSRTPTxStats
//    .assign  struct_ctxSRTPTx1,       R14, R21, ctxSRTPTx1
//.leave ctxSRTPTxScope
//
//.enter srtpTxCmdlScope
//    .assign  struct_saCmdL,             R10, R11, saCmdL
//    .assign  struct_saCmdLReg,          R10, R11, saCmdLReg
//    .assign  struct_saCmdlOpt,          R10, R13, saCmdlOpt
//.leave srtpTxCmdlScope    
//
//.enter rtpHdrScope
//    .assign  struct_rtpHdr,               R6, R8,   rtpHdr
//.leave rtpHdrScope   
//
//
///******************************************************************************
// * Function:    sap_srtp_tx_proc_pass1
// ******************************************************************************
// * Description: This function perfroms the first pass of the SRTP Tx packet
// *              header processing as the followings:  
// *
// *  - Extract RTP sequence number, calculate ROC (Roller Over Counter) and 
// *    update the local sequence number
// *  - Calculate the payload offset (Extract the RTP header optional field length) 
// *    and the payload length 
// *  - Calculate and record the MKI offset and the Authentication tag (Hash Value) offset
// *  - Derive initial IV from the RTP Header and/or salt key
// *  	SRTP CTR: IV = (session_salt<<16) XOR (SSRC<<64) XOR (RTP index << 16)
// *  	SRTP F8: IV' = 0x00 || M || PT || SEQ || TS || SSRC || ROC
// *  - Insert the Encryption Command label with payload length , offset  and IV
// *  - Insert the Authentication Command Label with data length (adjusted payload length + 4) and offset
// *  - Insert the HPS2 command label
// *  - Insert ROC to the end of payload 
// *
// *  Where the packet info consists of
// *        - pointer to the packet
// *        - packet length
// *        - offset to the RTP header
// *
// *  Note: It is assumed that the packets from the host (DSP) will not reserve 
// *        room for MKI and authentication tag which will be inserted at the
// *        end of the EOP data chunk
// *
// *****************************************************************************/

phpSrtpTxProc_1:

    // Error Check
    
    
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using srtpTxPass1Scope
    .using  ctxSRTPTxScope
    
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(ctxSRTPTxStats))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxSRTPTx1, SIZE(ctxSRTPTx1)
    
    // clear Local Variables (may be just some of them)
    zero    &srtpTxPass1Loc1,    SIZE(srtpTxPass1Loc1)  
    
    add     srtpTxPass1Loc1.mkiHashSize, ctxSRTPTx1.icvSize, ctxSRTPTx1.mkiSize
    
    qbbc    phpSrtpTxProc_1_OtherChunk_L1,   phpContext.chunkInfo.chunkFlag_sop
    
 phpSrtpTxProc_1_FirstChunk_L1:    
    //clear the control flags and other variables
    zero    &srtpPktInst,    SIZE(srtpPktInst)
    //Record the control size - the short payload Info
    sub     srtpTxPass1Loc1.pktOffset,  PktDesc.ControlSize, 8
    
 phpSrtpTxProc_1_FirstChunk_L2:  
    mov     srtpPktInst.nextEngId, ctxSRTPTx1.firstEngId
        
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, srtpPktInst.nextEngId
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    xout    XID_CDEDATA, PktDesc, 8   
      
    // Advance the window past the Descriptor
    // Move is desc size + phys PsInfo size
    // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    // add     CdeCmd.Cmd0.w2, PktDesc.PsPhysSize, SIZE(PktDesc) 
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_CONTROL
    xout    XID_CDECTRL, CdeCmd, 4      
      
    .leave  pktDescScope
    .using protoInfoScope
    // Extract protocol Info
    xin     XID_CDEDATA, payloadInfoS, SIZE(payloadInfoS)
    
    mov     srtpTxPass1Loc1.protoOffset,  payloadInfoS.offset
    sub     srtpTxPass1Loc1.hashDataLen,  payloadInfoS.len, srtpTxPass1Loc1.mkiHashSize
    // Calculate and record Various Offsets
    add     srtpPktInst.mkiOffset,   srtpTxPass1Loc1.protoOffset, srtpTxPass1Loc1.hashDataLen
    mov     srtpPktInst.mkiOffset_pass2, srtpPktInst.mkiOffset
    
    .leave protoInfoScope
    
    // TBD: Issue command to flash out and copy PSInfo
    // Now we're at the control section. Insert the command label here
    // Advance the window past the Descriptor
    // TBD: Move is desc size + phys PsInfo size
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4      
    
    //mov     CdeCmd.Cmd0.b1, SA_SRTP_IV_SIZE + SA_CMDL_HEADER_SIZE_BYTES 
    //qbeq    phpSrtpTxProc_1_SkipAuth, ctxSRTPTx1.icvSize, 0
    //add     CdeCmd.Cmd0.b1, CdeCmd.Cmd0.b1, SA_CMDL_HEADER_SIZE_BYTES
phpSrtpTxProc_1_SkipAuth:        
    qbeq    phpSrtpTxProc_1_FirstChunk_L2_1,  ctxSRTPTx1.cmdlTotalLen, 0  
        mov     CdeCmd.Cmd0.b1, ctxSRTPTx1.cmdlTotalLen 
        mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
        xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
   
phpSrtpTxProc_1_FirstChunk_L2_1:   
    // Advance to the SRTP header
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, srtpTxPass1Loc1.protoOffset, srtpTxPass1Loc1.pktOffset
    xout    XID_CDECTRL, CdeCmd, 4      
                         
    // Load and processing SRTP header
    .using rtpHdrScope
    xin     XID_CDEDATA, rtpHdr, SIZE(rtpHdr)
    qble    phpSrtpTxProc_1_FirstChunk_L3, rtpHdr.seqNum, ctxSRTPTx1.lastSeqNum
    // update ROC
    add     ctxSRTPTx1.roc, ctxSRTPTx1.roc, 1
    
phpSrtpTxProc_1_FirstChunk_L3:  
    mov     srtpPktInst.tempROC, ctxSRTPTx1.roc
    mov     ctxSRTPTx1.lastSeqNum, rtpHdr.seqNum       
    and     r0.b0,  rtpHdr.vpxcc, RTP_HDR_CC_MASK
    lsl     srtpTxPass1Loc1.rtpHdrSize, r0.b0, 2
    add     srtpTxPass1Loc1.rtpHdrSize, srtpTxPass1Loc1.rtpHdrSize, 12
    sub     srtpTxPass1Loc1.encryptedDataLen, srtpTxPass1Loc1.hashDataLen, srtpTxPass1Loc1.rtpHdrSize
    add     srtpTxPass1Loc1.hashDataLen, srtpTxPass1Loc1.hashDataLen, 4
    
    .leave rtpHdrScope
    //
    //  Command Label Processing
    //
    
    // Skip Command Processing if both null encryption and null-authentication
    qbeq    phpSrtpTxProc_1_HashProc_L1,  ctxSRTPTx1.cmdlTotalLen, 0  
    
    .using srtpTxCmdlScope  
    zero    &saCmdLReg.reg1, 8 
    
 phpSrtpTxProc_1_CmdlabelProc_L1:
    //
    // Prepare command label for encryption and authentication 
    //
    qbbc    phpSrtpTxProc_1_CmdlabelProc_Auth,    ctxSRTPTx1.ctrlInfo.flagSRTPTxEnc 
    
    // Command label for encryption
    qbne    phpSrtpTxProc_1_CmdlabelProc_L2, ctxSRTPTx1.icvSize, 0
    mov     saCmdL.nextEngId,  SAENG_PHP_2
    jmp     phpSrtpTxProc_1_CmdlabelProc_L3
 phpSrtpTxProc_1_CmdlabelProc_L2:
    mov     saCmdL.nextEngId,  SAENG_AUTH_1
 phpSrtpTxProc_1_CmdlabelProc_L3:
    mov     saCmdL.cmdlLen,    SA_SRTP_IV_SIZE + SA_CMDL_HEADER_SIZE_BYTES
    mov     saCmdL.procLen,    srtpTxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset,  srtpTxPass1Loc1.protoOffset, srtpTxPass1Loc1.rtpHdrSize
    mov     saCmdL.optctrl1,   SA_SRTP_CMDL_ENC_OPT1
    
    // Save the command label words to the dedicated buffer location 
    sbco    saCmdL, cCmdlBuf,   srtpTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     srtpTxPass1Loc1.cmdlOffset, srtpTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
 
    // IV processing: Get and store the IV
    // copy IV to the command label location
    call    SRTP_formIV
    sbco    saCmdlOpt.data1,  cCmdlBuf, srtpTxPass1Loc1.cmdlOffset, SA_SRTP_IV_SIZE
    add     srtpTxPass1Loc1.cmdlOffset, srtpTxPass1Loc1.cmdlOffset, SA_SRTP_IV_SIZE

phpSrtpTxProc_1_CmdlabelProc_Auth:
    qbeq    phpSrtpTxProc_1_CmdlabelProc_End, ctxSRTPTx1.icvSize, 0
    // Command label for authentication
    zero    &saCmdLReg.reg1, 8
    //mov     saCmdL.nextEngId,  SAENG_PHP_2
    //mov     saCmdL.cmdlLen,    SA_CMDL_HEADER_SIZE_BYTES
    mov     saCmdLReg.reg1.w2,  SA_CMDL_HEADER_SIZE_BYTES + (SAENG_PHP_2 << 8)
    mov     saCmdL.procLen,    srtpTxPass1Loc1.hashDataLen
    mov     saCmdL.sopOffset,  srtpTxPass1Loc1.protoOffset
    
    // Save the command label words to the dedicated buffer location 
    sbco    saCmdL, cCmdlBuf,   srtpTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     srtpTxPass1Loc1.cmdlOffset, srtpTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    .leave srtpTxCmdlScope  
     
 // End of Command label Processing    
 phpSrtpTxProc_1_CmdlabelProc_End:
    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, srtpTxPass1Loc1.cmdlOffset 
    mov     CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
    jmp     phpSrtpTxProc_1_HashProc_L1     
    
 phpSrtpTxProc_1_OtherChunk_L1:
    // Load packet instance 
    lbco    srtpPktInst, cInstBase,  phpContext.instOffset, SIZE(srtpPktInst)
    
    .using pktDescScope

    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, srtpPktInst.nextEngId
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2  
    
    // Advance the window to the beginning of packet 
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4      
    mov     srtpTxPass1Loc1.protoOffset,  0   // for non-SOP data chunk
                                              // protoOffset should be zero
 
    .leave pktDescScope  
    
 phpSrtpTxProc_1_HashProc_L1:
    qbeq    phpSrtpTxProc_1_End, srtpTxPass1Loc1.mkiHashSize, 0    
    qbbs    phpSrtpTxProc_1_End, srtpPktInst.ctrlFlag.srtpMkiHashComplete
        
    qbge    phpSrtpTxProc_1_HashProc_L2, srtpPktInst.mkiOffset, phpContext.chunkSize
    sub     srtpPktInst.mkiOffset, srtpPktInst.mkiOffset, phpContext.chunkSize
    jmp     phpSrtpTxProc_1_End  
    
 phpSrtpTxProc_1_HashProc_L2:
    // Hash Data is at this data chunk 
    // Advance to the ICV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, srtpPktInst.mkiOffset, srtpTxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4      
    
    // Inset the upper 32-bit ROC to the packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    mov     CdeCmd.Cmd1, ctxSRTPTx1.roc  
    xout    XID_CDECTRL, CdeCmd, 8 
         
    set     srtpPktInst.ctrlFlag.srtpMkiHashComplete
 
 phpSrtpTxProc_1_End:
    // Save Instance Data
    sbco    srtpPktInst, cInstBase, phpContext.instOffset, SIZE(srtpPktInst)
    
    // Save the updated security context
    // Advance the SC Viewer the first set of security context
    // TBD: The window does not move at SRTP Tx Pass 1 processing
    //mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(ctxSRTPTxStats))<< 8)
    //xout    XID_SCVCTRL, CdeCmd, 4   
         
    xout    XID_SCVDATA,  ctxSRTPTx1,  SIZE(ctxSRTPTx1)
    
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4

    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8)
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop

.leave srtpTxPass1Scope
.leave ctxSRTPTxScope
.leave commCtxScope    

///******************************************************************************
// * Function:    sap_srtp_tx_proc_pass2
// ******************************************************************************
// * Description: This function perfroms the second pass of the SRTP Tx packet
// *              header processing as the followings:  
// *
// *  - Append the MKI value to the encrypted payload if used (Note: ROC is overwritten)
// *  - Insert the Hash value to the hash location
// *  - Update the packet data pointer and length
// *  - Update statistics  
// *
// *
// *****************************************************************************/
//
//.struct struct_srtpTxPass2Loc1
//    .u8     mkiHashSize        // MKI Size plus Hash Size
//    .u8     pktId   
//.ends     
//
//.enter srtpTxPass2Scope
//    .assign  struct_srtpTxPass2Loc1,  R2,  R3,  srtpTxPass2Loc1
//    .assign  struct_srtpPktInst,      R22, R26, srtpPktInst      
//.leave srtpTxPass2Scope

phpSrtpTxProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using srtpTxPass2Scope
    .using ctxSRTPTxScope
    
    // clear Local Variables (may be just some of them)
    zero    &srtpTxPass2Loc1,    SIZE(srtpTxPass2Loc1)
    
phpSrtpTxProc_2_L1:
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(ctxSRTPTxStats)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxSRTPTx1, SIZE(ctxSRTPTx1)

    // Record packet instance index and load the instance
    mov     srtpTxPass2Loc1.pktId,  PktDesc.PktId
    lbco    srtpPktInst, cInstBase,  phpContext.instOffset, SIZE(srtpPktInst)

    add     srtpTxPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize 
    add     srtpTxPass2Loc1.mkiHashSize, ctxSRTPTx1.icvSize, ctxSRTPTx1.mkiSize
    mov     srtpTxPass2Loc1.chunkSize,  phpContext.chunkSize 
    
 phpSrtpTxProc_2_L2:    
    mov     PktDesc.SA_NextEngId, SAENG_OUT2_1
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 1
    
    // Update swInfo1 at SOP
    qbbc    phpSrtpTxProc_2_HashProc_L1, PktDesc.ChunkInfo.chunkFlag_sop
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
    
 //
 // Hash Processing
 //   
 phpSrtpTxProc_2_HashProc_L1:
    qbeq    phpSrtpTxProc_2_HashProc_End, srtpTxPass2Loc1.mkiHashSize, 0
    qbbs    phpSrtpTxProc_2_HashProc_End, srtpPktInst.ctrlFlag.srtpMkiHashComplete2   
    qbbs    phpSrtpTxProc_2_HashProc_L5,  srtpPktInst.ctrlFlag.srtpMkiHashPending   
    qbgt    phpSrtpTxProc_2_HashProc_L7,  phpContext.chunkSize, srtpPktInst.mkiOffset_pass2
    
    .leave pktDescScope
    
    //
    // Hash data is available in the PS Info section
    //
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
    
    // Read and store the Hash Data
    mov     r0.b0, ctxSRTPTx1.icvSize
    xin     XID_CDEDATA,  r6,  b0
    add     r0.w2, phpContext.instOffset, SRTP_PKT_INST_MKI_HASH_DATA_OFFSET
    add     r0.w2, r0.w2,   ctxSRTPTx1.mkiSize
    sbco    r6, cInstBase,  r0.w2, b0 

    qbeq    phpSrtpTxProc_2_HashProc_L2, ctxSRTPTx1.mkiSize, 0
    // store mki
    mov     r0.b0, ctxSRTPTx1.mkiSize
    sub     r0.w2, r0.w2,   ctxSRTPTx1.mkiSize
    sbco    ctxSRTPTx1.mki,  cInstBase,  r0.w2, b0
    
phpSrtpTxProc_2_HashProc_L2:    
    // Advance the window to the pkt location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Verify whether we need to insert leftover bytes when this chunk contains hash
    // 
    set     srtpTxPass2Loc1.ctrlFlag.leftoverInserted
    qbbc    phpSrtpTxProc_2_HashProc_L2_1,  srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // insert the leftover bytes
    add     srtpTxPass2Loc1.chunkSize, srtpTxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, srtpPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
phpSrtpTxProc_2_HashProc_L2_1:    
    sub     r0.w2,  phpContext.chunkSize, srtpPktInst.mkiOffset_pass2
    sub     r0.w2,  r0.w2, 4   // Substract ROC size: Note ROC was inserted at the mkiOffset so it is availabel at the same data chunk 
    min     r0.b0,  r0.w2, srtpTxPass2Loc1.mkiHashSize
    
    //
    // Corner case: ROC is the last 4-byte in the current data chunk
    //              Need to remember and flush out data in front of ROC
    //
    qbne    phpSrtpTxProc_2_HashProc_L2_2,  r0.b0, 0
    set     srtpTxPass2Loc1.ctrlFlag.leftoverVerified
    // Should we record and remove unaligned bytes due to output stream interface problem
    and     r7.w0,  srtpTxPass2Loc1.chunkSize,    0x3 
    clr     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    qbeq    phpSrtpTxProc_2_HashProc_L2_2,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, srtpPktInst.mkiOffset_pass2,     2   
    xout    XID_CDECTRL, CdeCmd, 4   
    
    xin     XID_CDEDATA, r6,  2
    mov     srtpPktInst.leftoverBytes,   r6.w2
    set     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
     
    // flush out unaligned bytes and the ROC
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    //add     srtpTxPass2Loc1.pktLoc, srtpPktInst.mkiOffset_pass2, 4
    jmp     phpSrtpTxProc_2_HashProc_L4

phpSrtpTxProc_2_HashProc_L2_2:    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, srtpPktInst.mkiOffset_pass2
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // flush out ROC
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 4
    xout    XID_CDECTRL, CdeCmd, 4
    add     srtpTxPass2Loc1.pktLoc, srtpPktInst.mkiOffset_pass2, 4
    
      
 phpSrtpTxProc_2_HashProc_L3:
    // Patch the MKI and Hash data
    // It is illegal that the length of "LBCO" is zero
    qbeq    phpSrtpTxProc_2_HashProc_L4, r0.b0, 0
    add     r0.w2,  phpContext.instOffset, SRTP_PKT_INST_MKI_HASH_DATA_OFFSET
    lbco    r6, cInstBase,  r0.w2, b0 
    xout    XID_CDEDATA,    r6,    b0  
    
    qbeq    phpSrtpTxProc_2_HashProc_L6, r0.b0, srtpTxPass2Loc1.mkiHashSize 
    
 phpSrtpTxProc_2_HashProc_L4:    
    mov     srtpPktInst.mkiHashRecorded, r0.b0
    set     srtpPktInst.ctrlFlag.srtpMkiHashPending
    jmp     phpSrtpTxProc_2_HashProc_End
    
 phpSrtpTxProc_2_HashProc_L5:
    // Update the remaining Hash data
    // Mov to the hash location:pktLoc = 0
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Verify whether we need to insert leftover bytes when this chunk contains hash
    // 
    set     srtpTxPass2Loc1.ctrlFlag.leftoverInserted
    qbbc    phpSrtpTxProc_2_HashProc_L5_1,  srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // insert the leftover bytes
    add     srtpTxPass2Loc1.chunkSize, srtpTxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, srtpPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
phpSrtpTxProc_2_HashProc_L5_1:    
    sub     r0.b0,  srtpTxPass2Loc1.mkiHashSize, srtpPktInst.mkiHashRecorded
    add     r0.w2,  phpContext.instOffset, SRTP_PKT_INST_MKI_HASH_DATA_OFFSET
    add     r0.w2,  r0.w2, srtpPktInst.mkiHashRecorded
    lbco    r6, cInstBase,  r0.w2, b0 
    xout    XID_CDEDATA,    r6,    b0  
    
 phpSrtpTxProc_2_HashProc_L6: 
    set    srtpPktInst.ctrlFlag.srtpMkiHashComplete2   
    jmp     phpSrtpTxProc_2_HashProc_End    
    
 phpSrtpTxProc_2_HashProc_L7:
    sub     srtpPktInst.mkiOffset_pass2,  srtpPktInst.mkiOffset_pass2,  phpContext.chunkSize 
    
 phpSrtpTxProc_2_HashProc_End:
    // Have we already verified leftover bytes? 
    qbbs    phpSrtpTxProc_2_HashProc_End_L1, srtpTxPass2Loc1.ctrlFlag.leftoverInserted   
    
    // Mov to the hash location:pktLoc = 0
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Verify whether we need to insert leftover bytes when this chunk contains hash
    // 
    qbbc    phpSrtpTxProc_2_HashProc_End_L1, srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // insert the leftover bytes
    add     srtpTxPass2Loc1.chunkSize, srtpTxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, srtpPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
 
 phpSrtpTxProc_2_HashProc_End_L1:
    qbbc    phpSrtpTxProc_2_L3,  phpContext.chunkInfo.chunkFlag_eop
    //
    //  Process EOP Data Chunk
    //
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxSRTPTxStats, SIZE(ctxSRTPTxStats)
    add     ctxSRTPTxStats.numTxPktsLo,  ctxSRTPTxStats.numTxPktsLo, 1
    adc     ctxSRTPTxStats.numTxPktsHi,  ctxSRTPTxStats.numTxPktsHi, 0ÿ
    clr     phpContext.instMask, srtpTxPass2Loc1.pktId
    xout    XID_SCVDATA, ctxSRTPTxStats, SIZE(ctxSRTPTxStats)
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_SRTP_ENC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_SRTP_ENC_HI_OFFSET, 8
    
    jmp     phpSrtpTxProc_2_End
    
 phpSrtpTxProc_2_L3:
    //
    //  Process non-EOP data chunk
    // 
    
    // check whether the leftover condition is aleady verified
    qbbs    phpSrtpTxProc_2_L4,  srtpTxPass2Loc1.ctrlFlag.leftoverVerified
    
    // Should we record and remove unaligned bytes dueto output stream interface problem
    and     r7.w0,  srtpTxPass2Loc1.chunkSize,    0x3 
    clr     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    qbeq    phpSrtpTxProc_2_L4,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, phpContext.chunkSize,   srtpTxPass2Loc1.pktLoc
    sub     CdeCmd.Cmd0.w2, CdeCmd.Cmd0.w2,     2   
    xout    XID_CDECTRL, CdeCmd, 4   
    
    xin     XID_CDEDATA, r6,  2
    mov     srtpPktInst.leftoverBytes,   r6.w2
    set     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
     
    // flush out unaligned bytes
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    
phpSrtpTxProc_2_L4:
    //  Save the instance Data
    sbco    srtpPktInst, cInstBase, phpContext.instOffset, SIZE(srtpPktInst)
    
 phpSrtpTxProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4

    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop

.leave srtpTxPass2Scope
.leave ctxSRTPTxScope
.leave commCtxScope    
 
        
