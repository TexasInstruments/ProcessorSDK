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

.struct struct_srtpTxPass1Loc1
    .u8     pktOffset
    .u16    hashDataLen        // Hash Data length (for command label)
    .u16    encryptedDataLen   // Encrypted Data length (for command label)
    .u8     protoOffset        // Protocol Offset
    .u8     rtpHdrSize         // RTP Header Size (for command label)
    .u8     cmdlOffset         // Command Label Offset 
    .u8     mkiHashSize        // MKI Size plus Hash Size
.ends     

.enter srtpTxPass1Scope
    .assign  struct_srtpTxPass1Loc1,  R1.b0,  R3,  srtpTxPass1Loc1
    .assign  struct_srtpPktInst,      R22, R26, srtpPktInst      
.leave srtpTxPass1Scope

.enter ctxSRTPTxScope
    .assign  struct_ctxSRTPTxStats,   R14, R15, ctxSRTPTxStats
    .assign  struct_ctxSRTPTx1,       R14, R21, ctxSRTPTx1
.leave ctxSRTPTxScope

.enter srtpTxCmdlScope
    .assign  struct_saCmdL,             R10, R11, saCmdL
    .assign  struct_saCmdLReg,          R10, R11, saCmdLReg
    .assign  struct_saCmdlOpt,          R10, R13, saCmdlOpt
.leave srtpTxCmdlScope    

.enter rtpHdrScope
    .assign  struct_rtpHdr,               R6, R8,   rtpHdr
.leave rtpHdrScope   

//.enter protoInfoScope
//    .assign  struct_protoInfo,          R6, R6,   protoInfo
//.leave protoInfoScope

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

phpSrtpScTxProc_1:

    // Error Check
    
    
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using srtpTxPass1Scope
    .using  ctxSRTPTxScope
    
    //Update PktDesc for the packet
    //mov     PktDesc.SA_NextEngId, SAENG_ENCRYPT_1
    //set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    //mov       r7.w2,  (SAENG_ENCRYPT_1 << 8) + 0x80
    //xout    XID_CDEDATA, PktDesc, 8   
    
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(ctxSRTPTxStats))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxSRTPTx1, SIZE(ctxSRTPTx1)
    
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, ctxSRTPTx1.firstEngId
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    xout    XID_CDEDATA, PktDesc, 8   
    
    // clear Local Variables (may be just some of them)
    zero    &srtpTxPass1Loc1,    SIZE(srtpTxPass1Loc1)  
    
    add     srtpTxPass1Loc1.mkiHashSize, ctxSRTPTx1.icvSize, ctxSRTPTx1.mkiSize
    
    //Record the control size - the short payload Info
    sub     srtpTxPass1Loc1.pktOffset,  PktDesc.ControlSize, 8
    
 phpSrtpScTxProc_1_FirstChunk_L1:    
 
    // Record the packet instance index
    // mov     ctxSRTPTx1.pktInstIndex, r0.b1
    
    //clear the control flags and other variables
    //zero    &srtpPktInst,    SIZE(srtpPktInst)
    
 phpSrtpScTxProc_1_FirstChunk_L2:  
        
    //Update PktDesc for the packet
    //mov     PktDesc.SA_NextEngId, SAENG_ENCRYPT_1
    //set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    //mov       r7.w2,  (SAENG_ENCRYPT_1 << 8) + 0x80
    
    //xout    XID_CDEDATA, PktDesc, 8   
      
    // Advance the window past the Descriptor
    // Move is desc size + phys PsInfo size
    // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    // add     CdeCmd.Cmd0.w2, PktDesc.PsPhysSize, SIZE(PktDesc) 
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_CONTROL
    xout    XID_CDECTRL, CdeCmd, 4 
    
    .leave  pktDescScope
    .using protoInfoScope
    // Extract protocol Info
    //xin     XID_CDEDATA, protoInfo,    SIZE(protoInfo)
    xin     XID_CDEDATA, payloadInfoS, SIZE(payloadInfoS)
    
    mov     srtpTxPass1Loc1.protoOffset,  payloadInfoS.offset
    sub     srtpTxPass1Loc1.hashDataLen,  payloadInfoS.len, srtpTxPass1Loc1.mkiHashSize
    // Calculate and record Various Offsets
    add     srtpPktInst.mkiOffset_sc,   srtpTxPass1Loc1.protoOffset, srtpTxPass1Loc1.hashDataLen
    
    .leave protoInfoScope
    
    // TBD: Issue command to flash out and copy PSInfo
    // Now we're at the control section. Insert the command label here
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4      
    
    //mov     CdeCmd.Cmd0.b1, SA_SRTP_IV_SIZE + SA_CMDL_HEADER_SIZE_BYTES 
    //qbeq    phpSrtpScTxProc_1_SkipAuth, ctxSRTPTx1.icvSize, 0
    //add     CdeCmd.Cmd0.b1, CdeCmd.Cmd0.b1, SA_CMDL_HEADER_SIZE_BYTES
phpSrtpScTxProc_1_SkipAuth:        
    qbeq    phpSrtpScTxProc_1_FirstChunk_L2_1,  ctxSRTPTx1.cmdlTotalLen, 0  
        mov     CdeCmd.Cmd0.b1, ctxSRTPTx1.cmdlTotalLen 
        mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
        xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
   
phpSrtpScTxProc_1_FirstChunk_L2_1:
   
    // Advance to the SRTP header
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, srtpTxPass1Loc1.protoOffset, srtpTxPass1Loc1.pktOffset
    xout    XID_CDECTRL, CdeCmd, 4      
     
    // Move here to save one stall                
    .using srtpTxCmdlScope  
    zero    &saCmdLReg.reg1, 8 
                         
    // Load and processing SRTP header
    .using rtpHdrScope
    xin     XID_CDEDATA, rtpHdr, SIZE(rtpHdr)
    qble    phpSrtpScTxProc_1_FirstChunk_L3, rtpHdr.seqNum, ctxSRTPTx1.lastSeqNum
    // update ROC
    add     ctxSRTPTx1.roc, ctxSRTPTx1.roc, 1
    
phpSrtpScTxProc_1_FirstChunk_L3:  
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
    // move to save one stall 
    //.using srtpTxCmdlScope  
    //zero    &saCmdLReg.reg1, 8 
    
    // Skip Command Processing if both null encryption and null-authentication
    qbeq    phpSrtpScTxProc_1_HashProc_L1,  ctxSRTPTx1.cmdlTotalLen, 0  
    
 phpSrtpScTxProc_1_CmdlabelProc_L1:
    //
    // Prepare command label for encryption and authentication 
    //
    qbbc    phpSrtpScTxProc_1_CmdlabelProc_Auth,    ctxSRTPTx1.ctrlInfo.flagSRTPTxEnc 
    // Command label for encryption
    qbne    phpSrtpScTxProc_1_CmdlabelProc_L2, ctxSRTPTx1.icvSize, 0
    mov     saCmdL.nextEngId,  SAENG_PHP_2
    jmp     phpSrtpScTxProc_1_CmdlabelProc_L3
 phpSrtpScTxProc_1_CmdlabelProc_L2:
    mov     saCmdL.nextEngId,  SAENG_AUTH_1
 phpSrtpScTxProc_1_CmdlabelProc_L3:
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
    
phpSrtpScTxProc_1_CmdlabelProc_Auth:
    qbeq    phpSrtpScTxProc_1_CmdlabelProc_End, ctxSRTPTx1.icvSize, 0
    // Command label for authentication
    zero    &saCmdLReg.reg1, 8
    mov     saCmdLReg.reg1.w2,  SA_CMDL_HEADER_SIZE_BYTES + (SAENG_PHP_2 << 8)
    //mov     saCmdL.nextEngId,  SAENG_PHP_2
    //mov     saCmdL.cmdlLen,    SA_CMDL_HEADER_SIZE_BYTES
    mov     saCmdL.procLen,    srtpTxPass1Loc1.hashDataLen
    mov     saCmdL.sopOffset,  srtpTxPass1Loc1.protoOffset
    
    // Save the command label words to the dedicated buffer location 
    sbco    saCmdL, cCmdlBuf,   srtpTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     srtpTxPass1Loc1.cmdlOffset, srtpTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    .leave srtpTxCmdlScope  
     
 // End of Command label Processing    
 phpSrtpScTxProc_1_CmdlabelProc_End:
    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, srtpTxPass1Loc1.cmdlOffset 
    mov     CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
 phpSrtpScTxProc_1_HashProc_L1:
    qbeq    phpSrtpScTxProc_1_End, ctxSRTPTx1.icvSize, 0    
        
    // Hash Data is at this data chunk 
    // Advance to the ICV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, srtpPktInst.mkiOffset_sc, srtpTxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4  
    
    // Move here to save two stalls
    sbco    srtpPktInst, cInstBase, phpContext.instOffset, OFFSET(struct_srtpPktInst.rocStatus)
    
    // Inset the upper 32-bit ROC to the packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    mov     CdeCmd.Cmd1, ctxSRTPTx1.roc  
    xout    XID_CDECTRL, CdeCmd, 8 
         
 phpSrtpScTxProc_1_End:
    // Save Instance Data
    // Move to save two stalls
    // sbco    srtpPktInst, cInstBase, phpContext.instOffset, OFFSET(struct_srtpPktInst.rocStatus)
    
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

.struct struct_srtpTxPass2Loc1
    .u8     mkiHashSize         // MKI Size plus Hash Size
    .u8     pktId   
    .u8     psCtrlSize          // PS Info + Control Info Size
    .u8     ctrlFlag            // record the protocol payload Size
#define leftoverInserted        t0  // The leftover bytes insertion condition has been verified and bytes inserted 
#define leftoverVerified        t1  // The leftover bytes condition has been verified and recorded
    .u16    pktLoc  
    .u16    chunkSize           // adjusted chunk size due to byte insertion
.ends     

.enter srtpTxPass2Scope
    .assign  struct_srtpTxPass2Loc1,  R2,  R3,  srtpTxPass2Loc1
    .assign  struct_srtpPktInst,      R22, R26, srtpPktInst      
.leave srtpTxPass2Scope

phpSrtpScTxProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using srtpTxPass2Scope
    .using ctxSRTPTxScope
    
    // clear Local Variables (may be just some of them)
    //zero    &srtpTxPass2Loc1,    SIZE(srtpTxPass2Loc1)    
    
phpSrtpScTxProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(ctxSRTPTxStats)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxSRTPTx1, SIZE(ctxSRTPTx1)

    // Record packet instance index and load the instance
    mov     srtpTxPass2Loc1.pktId,  PktDesc.PktId
    lbco    srtpPktInst, cInstBase,  phpContext.instOffset, OFFSET(struct_srtpPktInst.rocStatus)
    
 phpSrtpScTxProc_2_L2:    
    // Record the next engine ID 
    mov     PktDesc.SA_NextEngId, SAENG_OUT2_1
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 1
    add     srtpTxPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize    
    // Update swInfo1 at SOP
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
    .leave pktDescScope
    
 //
 // Hash Processing
 //   
 phpSrtpScTxProc_2_HashProc_L1:
 
    //add     srtpTxPass2Loc1.mkiHashSize, ctxSRTPTx1.icvSize, ctxSRTPTx1.mkiSize
    // qbeq    phpSrtpScTxProc_2_HashProc_End, srtpTxPass2Loc1.mkiHashSize, 0
    qbeq    phpSrtpScTxProc_2_HashProc_End, ctxSRTPTx1.icvSize, 0
    
    //
    // Hash data is available in the PS Info section
    //
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and store the Hash Data
    mov     r0.b0, ctxSRTPTx1.icvSize
    xin     XID_CDEDATA,  r6,  b0
    
phpSrtpScTxProc_2_HashProc_L2:    
    // Advance the window to the icv location
    // mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    // xout    XID_CDECTRL, CdeCmd, 4
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, srtpPktInst.mkiOffset_sc, srtpTxPass2Loc1.psCtrlSize
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // flush out ROC
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 4
    xout    XID_CDECTRL, CdeCmd, 4
      
 phpSrtpScTxProc_2_HashProc_L3:
    // Does MKI exist
    qbeq    phpSrtpScTxProc_2_HashProc_L4, ctxSRTPTx1.mkiSize, 0
    
    // Patch over mki 
    //mov     CdeCmd.Cmd0.b0, CDE_CMD_PATCH_PACKET
    //mov     CdeCmd.Cmd0.b1, ctxSRTPTx1.mkiSize
    //mov     CdeCmd.Cmd0.w2, srtpPktInst.mkiOffset_sc
    //mov     CdeCmd.Cmd1, ctxSRTPTx1.mki 
    //xout    XID_CDECTRL, CdeCmd, 4  
    mov     r29, r6
    mov     r6, ctxSRTPTx1.mki
    mov     r0.b1, ctxSRTPTx1.mkiSize
    xout    XID_CDEDATA,  r6,  b1    
    mov     r6, r29  
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, ctxSRTPTx1.mkiSize
    xout    XID_CDECTRL, CdeCmd, 4   
    
 phpSrtpScTxProc_2_HashProc_L4:
    // update the hash value (ICV)
    xout    XID_CDEDATA,  r6,  b0    
    
 phpSrtpScTxProc_2_HashProc_End:    
    //
    //  Process EOP Data Chunk
    //
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxSRTPTxStats, SIZE(ctxSRTPTxStats)
    add     ctxSRTPTxStats.numTxPktsLo,  ctxSRTPTxStats.numTxPktsLo, 1
    adc     ctxSRTPTxStats.numTxPktsHi,  ctxSRTPTxStats.numTxPktsHi, 0
    
    xout    XID_SCVDATA, ctxSRTPTxStats, SIZE(ctxSRTPTxStats)
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_SRTP_ENC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_SRTP_ENC_HI_OFFSET, 8
    
    clr     phpContext.instMask, srtpTxPass2Loc1.pktId
    
 phpSrtpScTxProc_2_End:
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
 
        
