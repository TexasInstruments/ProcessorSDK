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
//  * FILE NAME:  sasrtp_rx.p
//  *
//  * DESCRIPTION: Parses a packet in SRTP Rx mode
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

//
// Loc1 and Loc2 are overlapped
// Loc1: for command label processing
// Loc2: for Key Derivation operation
//


//.struct struct_srtpRxPass1Loc1
//    .u16    hashDataLen        // Hash Data length (for command label)
//    .u16    encryptedDataLen   // Encrypted Data length (for command label)
//    .u16    cmdlLen            // Command Label Length       
//    .u8     protoOffset        // Protocol Offset
//    .u8     rtpHdrSize         // RTP Header Size (for command label)
//    .u8     cmdlOffset         // Command Label Offset 
//    .u8     mkiHashSize        // MKI Size plus Hash Size
//.ends     
//
//.struct struct_srtpRxPass1Loc2
//    .u32    rLo                // The lower 32-bit of r (reminder)
//    .u16    rHi                // The upper 16-bit of r (reminder)
//.ends     
//
//.enter srtpRxPass1Scope
//    .assign  struct_srtpRxPass1Loc1,  R1,  R3.b2,  srtpRxPass1Loc1
//    .assign  struct_srtpRxPass1Loc2,  R1,  R2.w2,  srtpRxPass1Loc2
//    .assign  struct_srtpPktInst,      R22,  R25,   srtpPktInst      
//.leave srtpRxPass1Scope
//
//.enter ctxSRTPRxScope
//    .assign  struct_ctxSRTPRxStats,   R14, R16.w2, ctxSRTPRxStats
//    .assign  struct_ctxSRTPRx1,       R14, R21, ctxSRTPRx1
//    .assign  struct_ctxSRTPRxStatsReplay,  R6,  R10.w2,  ctxSRTPRxStatsReplay    
//    .assign  struct_ctxSRTPReKey,     R6,  R13, ctxSRTPReKey
//.leave ctxSRTPRxScope
//
//.enter srtpRxCmdlScope
//    .assign  struct_saCmdL,             R10, R11, saCmdL
//    .assign  struct_saCmdLReg,          R10, R11, saCmdLReg
//    .assign  struct_saCmdlOpt,          R10, R13, saCmdlOpt
//.leave srtpRxCmdlScope    
//

///******************************************************************************
// * Function:    sap_srtp_rx_proc_pass1
// ******************************************************************************
// * Description: This function perfroms the first pass of the SRTP Rx packet
// *              header processing as the followings: 
// *
// *   - Extract RTP sequence number and calculate ROC 
// *   - Calculate the payload offset (Extract the IP Header optional field length) 
// *        and the payload length
// *   - Calculate the MKI offset and the Authentication (Hash Value) offset
// *   - Perform Key Validation
// *        From To: Verify the extended sequence number against the From-To threshold
// *        MKI: Verify MKI value 
// *        Key Derivation check
// *   - Replay Check (sequence number range check)
// *   - Record the Authentication tag (Hash Value) (Extracting from the payload) 
// *   - Insert ROC to the end of payload (Note: we can overwrite MKI and Authentication tag now)
// *   - Insert the Authentication Command Label with data length (adjusted payload length + 4) 
// *        and offset
// *   - Derive initial IV from the RTP Header and/or salt key
// *        SRTP CTR: IV = (session_salt<<16) XOR (SSRC<<64) XOR (RTP index << 16)
// *        SRTP F8: IV' = 0x00 || M || PT || SEQ || TS || SSRC || ROC
// *   - Insert the Encryption Command Label with data length, offset and IV
// *
// *****************************************************************************/
//

phpSrtpRxProc_1:

    // Error Check
    
    
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using srtpRxPass1Scope
    .using ctxSRTPRxScope
    
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxSRTPRx.ctrlInfo))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxSRTPRx1, SIZE(ctxSRTPRx1)
    
    // clear Local Variables (may be just some of them)
    zero    &srtpRxPass1Loc1,    SIZE(srtpRxPass1Loc1)    
    add     srtpRxPass1Loc1.mkiHashSize, ctxSRTPRx1.icvSize, ctxSRTPRx1.mkiSize
    
    // Record packet instance index for PASS1 
    and     phpContext.ctrlInfo,  phpContext.ctrlInfo, PHP_INST_INDEX_MASK_NOT
    or      phpContext.ctrlInfo,  phpContext.ctrlInfo, PktDesc.PktId
    
    qbbc    phpSrtpRxProc_1_OtherChunk_L1,   phpContext.chunkInfo.chunkFlag_sop
    
 phpSrtpRxProc_1_FirstChunk_L1:    
    
    //clear the control flags and other variables
    zero    &srtpPktInst, SIZE(srtpPktInst)
    mov     srtpPktInst.nextEngId, ctxSRTPRx1.firstEngId
    
 phpSrtpRxProc_1_FirstChunk_L2:  
    //qbne    phpSrtpRxProc_1_NextEng_L1, ctxSRTPRx1.icvSize, 0
    //mov     srtpRxPass1Loc1.cmdlLen, SA_SRTP_IV_SIZE +  SA_CMDL_HEADER_SIZE_BYTES
    //mov     srtpPktInst.nextEngId, SAENG_ENCRYPT_1
    //jmp     phpSrtpRxProc_1_NextEng_L2
    
 phpSrtpRxProc_1_NextEng_L1:    
    //mov     srtpPktInst.nextEngId, ctxSRTPRx1.firstEngId
    //mov     srtpRxPass1Loc1.cmdlLen, SA_SRTP_IV_SIZE + SA_CMDL_HEADER_SIZE_BYTES * 2
    
 phpSrtpRxProc_1_NextEng_L2:
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
    xin     XID_CDEDATA, payloadInfoL, SIZE(struct_payloadInfoLong)
    mov     srtpRxPass1Loc1.protoOffset,  payloadInfoL.startOffset
    sub     srtpRxPass1Loc1.hashDataLen,  payloadInfoL.eopOffset, payloadInfoL.startOffset
    sub     srtpRxPass1Loc1.hashDataLen,  srtpRxPass1Loc1.hashDataLen, srtpRxPass1Loc1.mkiHashSize
    
    // Calculate and record Various Offsets
    add     srtpPktInst.mkiOffset,   srtpRxPass1Loc1.protoOffset, srtpRxPass1Loc1.hashDataLen
    mov     srtpPktInst.mkiOffset_pass2, srtpPktInst.mkiOffset
    
    // Now we're at the control section. Insert the command label here
    // Advance the window past the Descriptor
    qbeq    phpSrtpRxProc_1_FirstChunk_L2_1,  ctxSRTPRx1.cmdlTotalLen, 0  
        mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
        mov     CdeCmd.Cmd0.b1, ctxSRTPRx1.cmdlTotalLen
        xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
        jmp     phpSrtpRxProc_1_FirstChunk_L2_2
        
phpSrtpRxProc_1_FirstChunk_L2_1: 
        // Set Null Mode flag  
        set     srtpPktInst.ctrlFlag2.srtpNullMode
        //      pass through
   
phpSrtpRxProc_1_FirstChunk_L2_2:   
    // Advance to the SRTP header
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    //add     CdeCmd.Cmd0.w2, srtpRxPass1Loc1.protoOffset, payloadInfoL.recordLen
    mov     CdeCmd.Cmd0.w2, srtpRxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4 

    .leave protoInfoScope
                         
    // Load and processing SRTP header
    .using rtpHdrScope
    xin     XID_CDEDATA, rtpHdr, SIZE(rtpHdr)
    
phpSrtpRxProc_1_FirstChunk_L3:  
    and     r0.b0,  rtpHdr.vpxcc, RTP_HDR_CC_MASK
    lsl     srtpRxPass1Loc1.rtpHdrSize, r0.b0, 2
    add     srtpRxPass1Loc1.rtpHdrSize, srtpRxPass1Loc1.rtpHdrSize, 12
    sub     srtpRxPass1Loc1.encryptedDataLen, srtpRxPass1Loc1.hashDataLen, srtpRxPass1Loc1.rtpHdrSize
    add     srtpRxPass1Loc1.hashDataLen, srtpRxPass1Loc1.hashDataLen, 4
    mov     srtpPktInst.seqNum, rtpHdr.seqNum  
    // Calculate the temporary ROC based on RFC 3711 Apendix A
    call    SRTP_getROC
    
    .leave rtpHdrScope
    
    //
    //  Command Label Processing
    //
    
    // Skip Command Processing if both null encryption and null-authentication
    qbeq    phpSrtpRxProc_1_CmdlabelProc_L5,  ctxSRTPRx1.cmdlTotalLen, 0  
    
    .using srtpRxCmdlScope  
    zero    &saCmdLReg.reg1, 8 
    
 phpSrtpRxProc_1_CmdlabelProc_L1:
    //
    // Prepare command label for encryption and authentication 
    //
    qbeq    phpSrtpRxProc_1_CmdlabelProc_L4, ctxSRTPRx1.icvSize, 0
    // Authentication Command Label
    qbbs    phpSrtpRxProc_1_CmdlabelProc_L2, ctxSRTPRx1.ctrlInfo.flagSRTPRxEnc
    mov     saCmdL.nextEngId,  SAENG_PHP2_2
    jmp     phpSrtpRxProc_1_CmdlabelProc_L3
 phpSrtpRxProc_1_CmdlabelProc_L2:
    mov     saCmdL.nextEngId,  SAENG_ENCRYPT_1
 phpSrtpRxProc_1_CmdlabelProc_L3:
    mov     saCmdL.cmdlLen,    SA_CMDL_HEADER_SIZE_BYTES
    
    mov     saCmdL.procLen,    srtpRxPass1Loc1.hashDataLen
    mov     saCmdL.sopOffset,  srtpRxPass1Loc1.protoOffset
    // Save the command label words to the dedicated buffer location 
    sbco    saCmdL, cCmdlBuf,   srtpRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     srtpRxPass1Loc1.cmdlOffset, srtpRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    zero    &saCmdLReg.reg1, 8
    
 phpSrtpRxProc_1_CmdlabelProc_L4:
    qbbc    phpSrtpRxProc_1_CmdlabelProc_L5, ctxSRTPRx1.ctrlInfo.flagSRTPRxEnc
    // Command label for encryption
    //mov     saCmdL.nextEngId,  SAENG_PHP_2
    //add     saCmdL.cmdlLen,    ctxSRTPRx1.ivSize, SA_CMDL_HEADER_SIZE_BYTES
    mov     saCmdLReg.reg1.w2, (SAENG_PHP_2 << 8) + SA_SRTP_IV_SIZE + SA_CMDL_HEADER_SIZE_BYTES
    mov     saCmdL.procLen,    srtpRxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset,  srtpRxPass1Loc1.protoOffset, srtpRxPass1Loc1.rtpHdrSize
    mov     saCmdL.optctrl1,   SA_SRTP_CMDL_ENC_OPT1
    
    // Save the command label words to the dedicated buffer location 
    sbco    saCmdL, cCmdlBuf,   srtpRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     srtpRxPass1Loc1.cmdlOffset, srtpRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
 
    // IV processing: Get and store the IV
    // copy IV to the command label location
    call    SRTP_formIV
    sbco    saCmdlOpt.data1,  cCmdlBuf, srtpRxPass1Loc1.cmdlOffset, SA_SRTP_IV_SIZE
    add     srtpRxPass1Loc1.cmdlOffset, srtpRxPass1Loc1.cmdlOffset, SA_SRTP_IV_SIZE

    .leave srtpRxCmdlScope  
    
 //
 // Perform ReKey Check Prior to command Update
 // 
phpSrtpRxProc_1_CmdlabelProc_L5:
   
 // Move the SC Viewer the Re-Key parameters
 mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxSRTPRx.numRxPktsLo))<< 8)
 xout    XID_SCVCTRL, CdeCmd, 4        
 
 xin     XID_SCVDATA, ctxSRTPReKey, SIZE(ctxSRTPReKey)
 
 // FormTo Key Check
 qbbc    phpSrtpRxProc_1_ReKey_L2,  ctxSRTPRx1.ctrlInfo.flagSRTPRxFromTo
 
   // From range check
   qbgt  phpSrtpRxProc_1_ReKey_Fail, srtpPktInst.tempROC, ctxSRTPReKey.fromIndexHi
   qblt  phpSrtpRxProc_1_ReKey_L1,  srtpPktInst.tempROC, ctxSRTPReKey.fromIndexHi 
   qbgt  phpSrtpRxProc_1_ReKey_Fail, srtpPktInst.seqNum, ctxSRTPReKey.fromIndexLo 
   
 phpSrtpRxProc_1_ReKey_L1:
 
   // To range check
   qblt  phpSrtpRxProc_1_ReKey_Fail, srtpPktInst.tempROC, ctxSRTPReKey.toIndexHi
   qbgt  phpSrtpRxProc_1_ReKey_L3,   srtpPktInst.tempROC, ctxSRTPReKey.toIndexHi 
   qblt  phpSrtpRxProc_1_ReKey_Fail, srtpPktInst.seqNum, ctxSRTPReKey.toIndexLo
   jmp   phpSrtpRxProc_1_ReKey_L3 
 
 // Key Lifetime check
 phpSrtpRxProc_1_ReKey_L2: 
   qbgt  phpSrtpRxProc_1_ReKey_Fail, ctxSRTPReKey.keyLifetimeHi, ctxSRTPReKey.numRxPktsHi
   qblt  phpSrtpRxProc_1_ReKey_L3, ctxSRTPReKey.keyLifetimeHi, ctxSRTPReKey.numRxPktsHi
   qbgt  phpSrtpRxProc_1_ReKey_Fail, ctxSRTPReKey.keyLifetimeLo, ctxSRTPReKey.numRxPktsLo
   
 // Key Derivation Check  
 phpSrtpRxProc_1_ReKey_L3: 
   qble  phpSrtpRxProc_1_ReKey_L4, ctxSRTPReKey.keyDerivRate, 16
   
   lsr   srtpRxPass1Loc2.rLo, srtpPktInst.seqNum, ctxSRTPReKey.keyDerivRate
   //    r_lo = ((UINT32)p_pkt_inst->seq_num >> p_proto_ctx->key_deriv_rate) |
   //          (p_pkt_inst->temp_roc << (16 - p_proto_ctx->key_deriv_rate));
   //    r_hi =  p_pkt_inst->temp_roc >> (p_proto_ctx->key_deriv_rate + 16);      
   //
   mov   r0.b0, 16
   sub   r0.b0, r0.b0, ctxSRTPReKey.keyDerivRate
   lsl   r0.w2, srtpPktInst.tempROC, r0.b0
   add   srtpRxPass1Loc2.rLo, srtpRxPass1Loc2.rLo, r0.w2
   add   r0.b0, ctxSRTPReKey.keyDerivRate, 16
   lsr   srtpRxPass1Loc2.rHi, srtpPktInst.tempROC, r0.b0
   jmp   phpSrtpRxProc_1_ReKey_L5 
   
 phpSrtpRxProc_1_ReKey_L4:
   //    r_lo = p_pkt_inst->temp_roc >> (p_proto_ctx->key_deriv_rate - 16);
   //    r_hi = 0;
   //
   sub   r0.b0,  ctxSRTPReKey.keyDerivRate, 16
   lsr   srtpRxPass1Loc2.rLo,  srtpPktInst.tempROC, r0.b0
   mov   srtpRxPass1Loc2.rHi, 0
 
 phpSrtpRxProc_1_ReKey_L5:
   qbne  phpSrtpRxProc_1_ReKey_Fail,  srtpRxPass1Loc2.rLo,  ctxSRTPReKey.keyDerivRemLo
   qbne  phpSrtpRxProc_1_ReKey_Fail,  srtpRxPass1Loc2.rHi,  ctxSRTPReKey.keyDerivRemHi
   
 qbbs   phpSrtpRxProc_1_HashProc_L1, srtpPktInst.ctrlFlag2.srtpNullMode // Skip command copy operation 
     
 // End of Command label Processing    
 phpSrtpRxProc_1_CmdlabelProc_End:
    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, srtpRxPass1Loc1.cmdlOffset 
    mov     CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
    jmp     phpSrtpRxProc_1_HashProc_L1     
    
 phpSrtpRxProc_1_OtherChunk_L1:
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
    mov     srtpRxPass1Loc1.protoOffset,  0   // for non-SOP data chunk
 
    .leave pktDescScope  
    
    // Verify whether we need to just forward the data chunk due to replay or MKI error
    qbne    phpSrtpRxProc_1_HashProc_L1, srtpPktInst.nextEngId, SAENG_OUT2_1
    
    // MKI or replay error (forward packet to output port)
    // Verify whether we need to insert leftover bytes 
    // 
    mov     srtpRxPass1Loc2.chunkSize, phpContext.chunkSize 
    qbbc    phpSrtpRxProc_1_OtherChunk_L2,  srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // insert the leftover bytes
    add     srtpRxPass1Loc2.chunkSize, srtpRxPass1Loc2.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, srtpPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
phpSrtpRxProc_1_OtherChunk_L2:    
    qbbc    phpSrtpRxProc_1_OtherChunk_L3, phpContext.chunkInfo.chunkFlag_eop
    // clr     phpContext.instMask, phpContext.ctrlInfo
    and     r0.b0, phpContext.ctrlInfo, PHP_INST_INDEX_MASK
    clr     phpContext.instMask, r0.b0    
    jmp     phpSrtpRxProc_1_End_L1
    
phpSrtpRxProc_1_OtherChunk_L3:    
    // Non-EOP data chunk
    // Hardware patch: make sure that the output data chunk is 4-byte aligned
    // Error case:rekey failure pktLoc: strpRxPass1Loc1.protoOffset,
    //            MKI mismatch at SOP: srtpPktInst.mkiOffset (assign to strpRxPass1Loc1.protoOffset)
    // Note: All Error is detected at the first data chunk in a multi-chunk packet
    clr     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    and     r7.w0,  srtpRxPass1Loc2.chunkSize,  0x3 
    qbeq    phpSrtpRxProc_1_End,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, phpContext.chunkSize, 2
    xout    XID_CDECTRL, CdeCmd, 4  
    
    xin     XID_CDEDATA, r6,  2
    mov     srtpPktInst.leftoverBytes,   r6.w2
    set     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // flush out unaligned bytes
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    
    jmp     phpSrtpRxProc_1_End
    
 phpSrtpRxProc_1_HashProc_L1:
    qbeq    phpSrtpRxProc_1_End, srtpRxPass1Loc1.mkiHashSize, 0    
    qbbs    phpSrtpRxProc_1_End, srtpPktInst.ctrlFlag.srtpMkiHashComplete
        
    qbbs    phpSrtpRxProc_1_HashProc_L3, srtpPktInst.ctrlFlag.srtpMkiHashAvail
    qbge    phpSrtpRxProc_1_HashProc_L2, srtpPktInst.mkiOffset, phpContext.chunkSize
    sub     srtpPktInst.mkiOffset, srtpPktInst.mkiOffset, phpContext.chunkSize
    jmp     phpSrtpRxProc_1_End  
    
 phpSrtpRxProc_1_HashProc_L2:
    set     srtpPktInst.ctrlFlag.srtpMkiHashAvail
    
 phpSrtpRxProc_1_HashProc_L3:
    // Hash Data is at this data chunk 
    // Advance to the ICV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, srtpPktInst.mkiOffset, srtpRxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4      
    
    qbbs    phpSrtpRxProc_1_HashProc_L6, srtpPktInst.ctrlFlag.srtpMkiHashPending
    
    sub     r0.w0,  phpContext.chunkSize, srtpPktInst.mkiOffset
    min     srtpPktInst.mkiHashRecorded, r0.w0, srtpRxPass1Loc1.mkiHashSize
    
    // It is illegal that the length of "SBCO" is 0: special case
    qbeq    phpSrtpRxProc_1_HashProc_L4,  srtpPktInst.mkiHashRecorded, 0
    
    // Copy the hash data to the instance buffer
    mov     r0.b0, srtpPktInst.mkiHashRecorded 
    xin     XID_CDEDATA, r6, b0
    add     r0.w2,  phpContext.instOffset, SRTP_PKT_INST_MKI_HASH_DATA_OFFSET
    
    // 
    // Store MKI prior to the Hash buffer starts
    //
    sub     r0.w2, r0.w2,   ctxSRTPRx1.mkiSize
    sbco    r6, cInstBase,  r0.w2, b0 
    
    // Verify MKI if available and it is the first data chunk
    qbeq    phpSrtpRxProc_1_HashProc_L4, ctxSRTPRx1.mkiSize, 0
    qbbc    phpSrtpRxProc_1_HashProc_L4, phpContext.chunkInfo.chunkFlag_sop
    qblt    phpSrtpRxProc_1_HashProc_L4, ctxSRTPRx1.mkiSize, r0.b0
    call    SRTP_MKICheck
    qbne    phpSrtpRxProc_1_MKI_Mismatch, r29.b0, 0
    set     srtpPktInst.ctrlFlag.srtpMkiVerified
    
 phpSrtpRxProc_1_HashProc_L4:
    // Inset the upper 32-bit ROC to the packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    mov     CdeCmd.Cmd1, srtpPktInst.tempROC  
    xout    XID_CDECTRL, CdeCmd, 8 
         
 phpSrtpRxProc_1_HashProc_L5:    
    qbeq    phpSrtpRxProc_1_HashProc_L7, srtpPktInst.mkiHashRecorded, srtpRxPass1Loc1.mkiHashSize 
    mov     srtpPktInst.mkiOffset, 0
    set     srtpPktInst.ctrlFlag.srtpMkiHashPending
    jmp     phpSrtpRxProc_1_End
    
 phpSrtpRxProc_1_HashProc_L6:
    // Processing Pending Hash data
    // copy the hash data out
    sub     r0.b0, srtpRxPass1Loc1.mkiHashSize, srtpPktInst.mkiHashRecorded 
    xin     XID_CDEDATA, r6, b0
    add     r0.w2,  phpContext.instOffset, SRTP_PKT_INST_MKI_HASH_DATA_OFFSET
    add     r0.w2,  r0.w2, srtpPktInst.mkiHashRecorded
    // 
    // Store MKI prior to the Hash buufer starts
    //
    sub     r0.w2, r0.w2,   ctxSRTPRx1.mkiSize
    sbco    r6, cInstBase,  r0.w2, b0 
    
    // Flash the Hash Data out
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    //sub     CdeCmd.Cmd0.w2, srtpRxPass1Loc1.mkiHashSize, srtpPktInst.mkiHashRecorded
    //xout    XID_CDECTRL, CdeCmd, 4 
    
    // Check whether it becomes zero-length packet
    //qbgt    phpSrtpRxProc_1_HashProc_L7, CdeCmd.Cmd0.w2, phpContext.chunkSize 
    //set     srtpPktInst.ctrlFlag.srtpRmLastChunk    
    
 phpSrtpRxProc_1_HashProc_L7:
    set     srtpPktInst.ctrlFlag.srtpMkiHashComplete
 
 phpSrtpRxProc_1_End:
    
    // Save Instance Data
    sbco    srtpPktInst, cInstBase, phpContext.instOffset, SIZE(srtpPktInst)
 
 phpSrtpRxProc_1_End_L1:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4

    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8)
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop
    
 phpSrtpRxProc_1_MKI_Mismatch:
    // Set Error Code
    mov  srtpRxPass1Loc1.errCode, (SA_PKT_ERR_INVALID_MKI << 4)
    // MKI mismatch at SOP: pktLoc at srtpPktInst.mkiOffset (assign to srtpRxPass1Loc1.protoOffset)
    mov  srtpRxPass1Loc1.protoOffset, srtpPktInst.mkiOffset
    jmp  phpSrtpRxProc_1_Err_Common   
    
 phpSrtpRxProc_1_ReKey_Fail:
    // Set Error Code
    mov  srtpRxPass1Loc1.errCode, (SA_PKT_ERR_INVALID_KEY << 4)
    
 phpSrtpRxProc_1_Err_Common:
 
    // Set next eng ID to SAENG_OUT2_1
    mov srtpPktInst.nextEngId, SAENG_OUT2_1
    
    // 
    // Update the packet descriptor to drop the packet
    // EngId = Output2
    // ThreadId = 7
    // Command label removed
    // Set Protocol-specific Error code
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket
    .using pktDescScope
    
    lbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SrcID)
    mov     PktDesc.ChunkInfoPS,    srtpRxPass1Loc1.errCode
    mov     PktDesc.SA_NextEngId,   SAENG_OUT2_1
    mov     PktDesc.SA_CmdLblInfo,  srtpRxPass1Loc1.cmdlOffset
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    sbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SrcID)
    
    .leave pktDescScope
    
    //
    // Hardware patch: make sure that the output data chunk is 4-byte aligned
    // Error case:rekey failure pktLoc: strpRxPass1Loc1.protoOffset,
    //            MKI mismatch at SOP: srtpPktInst.mkiOffset (assign to srtpRxPass1Loc1.protoOffset)
    // Note: All Error is detected at the first data chunk in a multi-chunk packet
    and     r7.w0,  phpContext.chunkSize,  0x3 
    qbeq    phpSrtpRxProc_1_End,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, phpContext.chunkSize, srtpRxPass1Loc1.protoOffset
    sub     CdeCmd.Cmd0.w2, CdeCmd.Cmd0.w2,     2   
    xout    XID_CDECTRL, CdeCmd, 4  
    
    xin     XID_CDEDATA, r6,  2
    mov     srtpPktInst.leftoverBytes,   r6.w2
    set     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // flush out unaligned bytes
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    
    jmp     phpSrtpRxProc_1_End

.leave srtpRxPass1Scope
.leave ctxSRTPRxScope
.leave commCtxScope 

///******************************************************************************
// * Function:    sap_srtp_rx_proc_pass2
// ******************************************************************************
// * Description: This function perfroms the second pass of the SRTP Rx packet
// *              header processing as the followings:  
// *
// *   - Compare the Hash value against the Extracted Hash value 
// *   - Update stored ROC and Sequence Number
// *   - Update the Replay window (Expected Sequence Number & Replay Mask Array)
// *   - Update the packet data pointer and length
// *   - Update statistics
// *
// *****************************************************************************/
//   
//.struct struct_srtpRxPass2Loc1
//    .u8     mkiHashSize        // MKI Size plus Hash Size
//    .u8     pktId   
//.ends     
//
//.enter srtpRxPass2Scope
//    .assign  struct_srtpRxPass2Loc1,  R2.w2,  R2.w2,  srtpRxPass2Loc1
//    .assign  struct_srtpPktInst,      R22, R25, srtpPktInst      
//.leave srtpRxPass2Scope

phpSrtpRxProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using srtpRxPass2Scope
    .using ctxSRTPRxScope
    
    // clear Local Variables (may be just some of them)
    zero    &srtpRxPass2Loc1,    SIZE(srtpRxPass2Loc1)    
    
phpSrtpRxProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxSRTPRx.ctrlInfo))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxSRTPRx1, SIZE(ctxSRTPRx1)

    // Record packet instance index and load the instance
    mov     srtpRxPass2Loc1.pktId,  PktDesc.PktId
    lbco    srtpPktInst, cInstBase,  phpContext.instOffset, SIZE(srtpPktInst)

    add     srtpRxPass2Loc1.mkiHashSize, ctxSRTPRx1.icvSize, ctxSRTPRx1.mkiSize
    
    mov     PktDesc.SA_NextEngId, SAENG_OUT2_1
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 1
    
    // Update swInfo1 at SOP
    qbbc    phpSrtpRxProc_2_L1_1, PktDesc.ChunkInfo.chunkFlag_sop
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
phpSrtpRxProc_2_L1_1:
    
    add     srtpRxPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize 
    mov     srtpRxPass2Loc1.chunkSize,  phpContext.chunkSize 
    
    // Packet Error check 
    // Do not update the statistics
    qbbc    phpSrtpRxProc_2_HashProc_L1, srtpPktInst.ctrlFlag.srtpPktError
       
    // Verify whether we need to insert leftover bytes
    qbbc    phpSrtpRxProc_2_L2,  srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // Advance the window to the packet pktLoc = 0
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // insert the leftover bytes
    add     srtpRxPass2Loc1.chunkSize,  srtpRxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, srtpPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
 phpSrtpRxProc_2_L2:    
    qbbs    phpSrtpRxProc_2_EOP_L1,  phpContext.chunkInfo.chunkFlag_eop // EOP processing
    jmp     phpSrtpRxProc_2_L3                                          // non-EOP processing

    
 //
 // Hash Processing
 //   
 phpSrtpRxProc_2_HashProc_L1:
 
    qbeq    phpSrtpRxProc_2_HashProc_End, srtpRxPass2Loc1.mkiHashSize, 0
    qbbs    phpSrtpRxProc_2_HashProc_End, srtpPktInst.ctrlFlag.srtpMkiHashComplete2   
    qbbs    phpSrtpRxProc_2_HashProc_L4, srtpPktInst.ctrlFlag.srtpMkiHashAvail2   
    qbgt    phpSrtpRxProc_2_HashProc_L6,  phpContext.chunkSize,   srtpPktInst.mkiOffset_pass2
    
    .leave pktDescScope
    
    //
    // Hash data is available in the PS Info section
    // which will be recorded for this and subsequent data chunks
    //
    set     srtpPktInst.ctrlFlag.srtpMkiHashAvail2
    set     srtpRxPass2Loc1.ctrlFlag.leftoverInserted
    
phpSrtpRxProc_2_HashProc_L2:    
    
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read the Hash Data
    mov     r0.b0, ctxSRTPRx1.icvSize
    xin     XID_CDEDATA,  r6,  b0
    
    // Advance the window to the packet location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Store the Hash data in the buffer
    add     r0.w2,  phpContext.instOffset, SRTP_PKT_INST_HASH_DATA_OFFSET2
    sbco    r6, cInstBase,  r0.w2, b0 
    
    // Verify whether we need to insert leftover bytes when this chunk contains hash
    qbbc     phpSrtpRxProc_2_HashProc_L2_1,  srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // insert the leftover bytes
    add     srtpRxPass2Loc1.chunkSize, srtpRxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, srtpPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
phpSrtpRxProc_2_HashProc_L2_1:  

    sub     r0.w2,  phpContext.chunkSize, srtpPktInst.mkiOffset_pass2
    //
    // Corner case: ROC is the last 4-byte in the current data chunk
    //              Need to remember and flush out data in front of ROC
    //
    qbne    phpSrtpRxProc_2_HashProc_L2_2,  r0.w2, 4
    set     srtpRxPass2Loc1.ctrlFlag.leftoverVerified
    // Should we record and remove unaligned bytes due to output stream interface problem
    and     r7.w0,  srtpRxPass2Loc1.chunkSize,    0x3 
    clr     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    qbeq    phpSrtpRxProc_2_HashProc_L2_2,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, srtpPktInst.mkiOffset_pass2,     2   
    xout    XID_CDECTRL, CdeCmd, 4   
    
    xin     XID_CDEDATA, r6,  2
    mov     srtpPktInst.leftoverBytes,   r6.w2
    set     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
     
    // flush out unaligned bytes and the ROC
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    //add     srtpRxPass2Loc1.pktLoc, srtpPktInst.mkiOffset_pass2, 4
    jmp     phpSrtpRxProc_2_HashProc_L3

phpSrtpRxProc_2_HashProc_L2_2:

    // Advance the window to the icv location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, srtpPktInst.mkiOffset_pass2
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // flush out ROC
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 4
    xout    XID_CDECTRL, CdeCmd, 4
    add     srtpRxPass2Loc1.pktLoc,  srtpPktInst.mkiOffset_pass2, 4
      
phpSrtpRxProc_2_HashProc_L3:
    // Verify whether the complete hash in the packet is recorded
    qbbc    phpSrtpRxProc_2_HashProc_End, srtpPktInst.ctrlFlag.srtpMkiHashComplete   
    
phpSrtpRxProc_2_HashProc_L4:    

    // Hash process will be completed in this data chunk
    set     srtpPktInst.ctrlFlag.srtpMkiHashComplete2
    
    //
    // MKI comapring
    //
    qbeq    phpSrtpRxProc_2_HashProc_L5, ctxSRTPRx1.mkiSize, 0
    qbbs    phpSrtpRxProc_2_HashProc_L5, srtpPktInst.ctrlFlag.srtpMkiVerified
    mov     r0.b0, ctxSRTPRx1.mkiSize 
    add     r0.w2, phpContext.instOffset, SRTP_PKT_INST_MKI_HASH_DATA_OFFSET
    // MKI is stored prior to the HASH data buffer 
    sub     r0.w2,  r0.w2, ctxSRTPRx1.mkiSize 
    lbco    r6, cInstBase,  r0.w2, b0 
    call    SRTP_MKICheck
    qbne    phpSrtpRxProc_2_Err_MKI, r29.b0, 0
    
phpSrtpRxProc_2_HashProc_L5:    
    //
    //  SRTP Hash Comparing
    //
    qbeq    phpSrtpRxProc_2_RocUpdate, ctxSRTPRx1.icvSize, 0
    add     r0.b0, ctxSRTPRx1.icvSize, 16 
    add     r0.w2, phpContext.instOffset, SRTP_PKT_INST_MKI_HASH_DATA_OFFSET
    lbco    r6, cInstBase, r0.w2, b0 
    srtp_icv_check  phpSrtpRxProc_2_Err_Hash, phpSrtpRxProc_2_RocUpdate, ctxSRTPRx1.icvSize 

    //
    // ROC Updates
    //    
phpSrtpRxProc_2_RocUpdate:
    qbeq    phpSrtpRxProc_2_RocUpdate_NOCHANGE, srtpPktInst.rocStatus, SA_SRTP_ROC_NOCHANGE
    qbeq    phpSrtpRxProc_2_RocUpdate_PLUS_1,   srtpPktInst.rocStatus, SA_SRTP_ROC_PLUS_1
    jmp     phpSrtpRxProc_2_HashProc_ReplayCheck
    
phpSrtpRxProc_2_RocUpdate_NOCHANGE:
    qble    phpSrtpRxProc_2_HashProc_ReplayCheck, ctxSRTPRx1.lastSeqNum, srtpPktInst.seqNum    
    mov     ctxSRTPRx1.lastSeqNum, srtpPktInst.seqNum
    jmp     phpSrtpRxProc_2_RocUpdate_Update
    
phpSrtpRxProc_2_RocUpdate_PLUS_1:
    mov     ctxSRTPRx1.lastSeqNum, srtpPktInst.seqNum
    mov     ctxSRTPRx1.roc, srtpPktInst.tempROC
    
phpSrtpRxProc_2_RocUpdate_Update:
    xout    XID_SCVDATA,  ctxSRTPRx1, SIZE(ctxSRTPRx1)   
    
phpSrtpRxProc_2_HashProc_ReplayCheck:
    //
    // Replay Init for the first Packet 
    // 
    qbbc phpSrtpRxProc_2_HashProc_ReplayCheck_L1, ctxSRTPRx1.ctrlInfo.flagSRTPRx1stPkt 
    clr  ctxSRTPRx1.ctrlInfo.flagSRTPRx1stPkt
    xout    XID_SCVDATA,  ctxSRTPRx1, SIZE(ctxSRTPRx1)
    
    .using replayCtrlScope
    
    // Advance the SC Viewer the replay Ctrl
    //mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    //xout    XID_SCVCTRL, CdeCmd, 4  
    //xin     XID_SCVDATA, replayCtrl, SIZE(replayCtrl)
    //mov     replayCtrl.winBase, r29
    //call    Replay_Init
    //jmp     phpSrtpRxProc_2_HashProc_ReplayCheck_L2       
    
phpSrtpRxProc_2_HashProc_ReplayCheck_L1:    
    //
    //  Replay check
    //
    qbbc    phpSrtpRxProc_2_HashProc_End, ctxSRTPRx1.ctrlInfo.flagSRTPRxReplay
    // Construct the 32-bit sequence number
    mov r29.w2, srtpPktInst.tempROC.w0
    mov r29.w0, srtpPktInst.seqNum
    
    // Advance the SC Viewer the replay Ctrl
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, replayCtrl, SIZE(replayCtrl) 
    
phpSrtpRxProc_2_HashProc_ReplayCheck_L2:    
    call    Replay_CheckAndUpdate
    
    // Verify the replay check result
    qbeq    phpSrtpRxProc_2_ReplayPass, r29.b0, SA_REPLAY_RC_OK
    qbeq    phpSrtpRxProc_2_ReplayPass, r29.b0, SA_REPLAY_RC_NEW
    
    // Error Handling 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(struct_replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxSRTPRxStats, SIZE(ctxSRTPRxStats)
    
    qbeq    phpSrtpRxProc_2_ReplayDup, r29.b0, SA_REPLAY_RC_DUP
    
 phpSrtpRxProc_2_ReplayOld:
    mov     srtpRxPass2Loc1.errCode, (SA_PKT_ERR_REPLAY_OLD << 4)
    add     ctxSRTPRxStats.numOldPkts, ctxSRTPRxStats.numOldPkts, 1
    // Update System Statistics
    lbco    r5, cStats, PDSP_SRTP_REPLAY_OLD_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_SRTP_REPLAY_OLD_OFFSET, 4
    jmp     phpSrtpRxProc_2_Err_Replay
 phpSrtpRxProc_2_ReplayDup:
    mov     srtpRxPass2Loc1.errCode, (SA_PKT_ERR_REPLAY_DUP << 4)
    add     ctxSRTPRxStats.numDupPkts, ctxSRTPRxStats.numDupPkts, 1
    // Update System Statistics
    lbco    r5, cStats, PDSP_SRTP_REPLAY_DUP_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_SRTP_REPLAY_DUP_OFFSET, 4
    jmp     phpSrtpRxProc_2_Err_Replay
     
 phpSrtpRxProc_2_ReplayPass:
    xout    XID_SCVDATA, replayCtrl, SIZE(replayCtrl)
    .leave replayCtrlScope
    
    jmp     phpSrtpRxProc_2_HashProc_End  
    
 phpSrtpRxProc_2_HashProc_L6:
    sub     srtpPktInst.mkiOffset_pass2,  srtpPktInst.mkiOffset_pass2,  phpContext.chunkSize 
    
 phpSrtpRxProc_2_HashProc_End:
    qbbs    phpSrtpRxProc_2_HashProc_End_L1, srtpRxPass2Loc1.ctrlFlag.leftoverInserted
    // Update the remaining Hash data at the top of packet
    // Mov to the hash location (pktLoc = 0)
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Verify whether we need to insert leftover bytes
    qbbc     phpSrtpRxProc_2_HashProc_End_L1,  srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // insert the leftover bytes
    add     srtpRxPass2Loc1.chunkSize,  srtpRxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, srtpPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
 
 phpSrtpRxProc_2_HashProc_End_L1:
    qbbc    phpSrtpRxProc_2_L3,  phpContext.chunkInfo.chunkFlag_eop
    //
    //  Process EOP Data Chunk
    //
    qbbs    phpSrtpRxProc_2_EOP_L1, srtpPktInst.ctrlFlag.srtpPktError
    
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(struct_replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxSRTPRxStats, SIZE(ctxSRTPRxStats)
    add     ctxSRTPRxStats.numRxPktsLo,  ctxSRTPRxStats.numRxPktsLo, 1
    adc     ctxSRTPRxStats.numRxPktsHi,  ctxSRTPRxStats.numRxPktsHi, 0
    xout    XID_SCVDATA, ctxSRTPRxStats, SIZE(ctxSRTPRxStats)
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_SRTP_DEC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_SRTP_DEC_HI_OFFSET, 8
    
 phpSrtpRxProc_2_EOP_L1:   
    clr     phpContext.instMask, srtpRxPass2Loc1.pktId
    jmp     phpSrtpRxProc_2_End
    
 phpSrtpRxProc_2_L3:
    //
    //  Process non-EOP data chunk
    // 
    // check whether the leftover condition is aleady verified
    qbbs    phpSrtpRxProc_2_L3_1,  srtpRxPass2Loc1.ctrlFlag.leftoverVerified
    
    
    // Should we record and remove unaligned bytes due to output stream interface problem
    and     r7.w0,  srtpRxPass2Loc1.chunkSize,    0x3 
    clr     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    qbeq    phpSrtpRxProc_2_L3_1,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, phpContext.chunkSize,   srtpRxPass2Loc1.pktLoc
    sub     CdeCmd.Cmd0.w2, CdeCmd.Cmd0.w2,     2   
    xout    XID_CDECTRL, CdeCmd, 4   
    
    xin     XID_CDEDATA, r6,  2
    mov     srtpPktInst.leftoverBytes,  r6.w2
    set     srtpPktInst.ctrlFlag2.srtpLeftoverBytes
     
    // flush out unaligned bytes
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    
phpSrtpRxProc_2_L3_1:
          
    //  Save the instance Data
    sbco    srtpPktInst, cInstBase, phpContext.instOffset, SIZE(srtpPktInst)
    
 phpSrtpRxProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4

    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop
    
phpSrtpRxProc_2_Err_MKI:
    mov  srtpRxPass2Loc1.errCode, (SA_PKT_ERR_INVALID_MKI << 4)
    jmp  phpSrtpRxProc_2_Err_Common_L1    
    
phpSrtpRxProc_2_Err_Hash:
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(struct_replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxSRTPRxStats, SIZE(ctxSRTPRxStats)
    
    add     ctxSRTPRxStats.numHashFails, ctxSRTPRxStats.numHashFails, 1
    
    // Update system statistics
    lbco    r5, cStats, PDSP_SRTP_AUTH_FAIL_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_SRTP_AUTH_FAIL_OFFSET, 4
    
    // Record the error code
    mov  srtpRxPass2Loc1.errCode, (SA_PKT_ERR_AUTH_FAIL << 4)
    
phpSrtpRxProc_2_Err_Replay:

phpSrtpRxProc_2_Err_Common:

    // Update the statistics in the security context
    xout XID_SCVDATA, ctxSRTPRxStats, SIZE(ctxSRTPRxStats)

phpSrtpRxProc_2_Err_Common_L1:

    // Record that it is an error packet
    set      srtpPktInst.ctrlFlag.srtpPktError

    qbbs    phpSrtpRxProc_2_Err_Common_L2, srtpRxPass2Loc1.ctrlFlag.leftoverInserted

    // 
    // Update the packet descriptor
    // Set Error Code
    //
    // Mov to the packet location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Verify whether we need to insert leftover bytes
    qbbc    phpSrtpRxProc_2_Err_Common_L2,  srtpPktInst.ctrlFlag2.srtpLeftoverBytes
    
    // insert the leftover bytes
    add     srtpRxPass2Loc1.chunkSize,  srtpRxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, srtpPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
    
phpSrtpRxProc_2_Err_Common_L2:
    wbs     r31.tStatus_CDEOutPacket
    sbco    srtpRxPass2Loc1.errCode, cCdeOutPkt, OFFSET(struct_PktDesc.ChunkInfoPS), SIZE(struct_PktDesc.ChunkInfoPS) 
    
    qbbs    phpSrtpRxProc_2_EOP_L1,  phpContext.chunkInfo.chunkFlag_eop   // EOP processing
    jmp     phpSrtpRxProc_2_L3                                            // non-EOP processing


.leave srtpRxPass2Scope
.leave ctxSRTPRxScope
.leave commCtxScope    
 
        
