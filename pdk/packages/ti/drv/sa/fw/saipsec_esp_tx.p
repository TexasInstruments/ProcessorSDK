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
//  * FILE PURPOSE:  SA firmware for PDSPs 
//  **********************************************************************************
//  * FILE NAME:  saipsec_esp_tx.p
//  *
//  * DESCRIPTION: Process the IPSEC Tx (To-Network) packets
//  *
//  **********************************************************************************/
//***************************************************************************
//* FILE PURPOSE: PA firmware for PHP PDSP  - IPSec ESP mode handling
//****************************************************************************
//* FILE NAME: saipsec_esp.p
//*
//* DESCRIPTION:
//*  This file contains functions for SA packet header processing of the IPSec 
//*  ESP packets
//*
//*
//*   0                   1                   2                   3
//*   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ----
//*   |               Security Parameters Index (SPI)                 | ^Int. (Authentication)
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |Cov-
//*   |                      Sequence Number                          | |ered
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ | ----
//*   |                    Payload Data* (variable)                   | |   ^ (Encryption)
//*   ~                                                               ~ |   |
//*   |                                                               | |Conf.
//*   +               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |Cov-
//*   |               |     Padding (0-255 bytes)                     | |ered*
//*   +-+-+-+-+-+-+-+-+               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |   |
//*   |                               |  Pad Length   | Next Header   | v   v
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ------
//*   |         Integrity Check Value-ICV   (variable)                |
//*   ~                                                               ~
//*   |                                                               |
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//*
//*               Figure 1.  Top-Level Format of an ESP Packet
//*
//*   * If included in the Payload field, cryptographic synchronization
//*     data, e.g., an Initialization Vector (IV, see Section 2.3),
//*     usually is not encrypted per se, although it often is referred
//*     to as being part of the ciphertext.
//*
//* (C) Copyright 2008 Texas Instruments, Inc.
//*****************************************************************************/
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

//.struct struct_espTxPass1Loc1
//    .u16    hashDataLen        // Hash Data length (for command label)
//    .u16    encryptedDataLen   // Encrypted Data length (for command label)
//    .u8     protoOffset        // Protocol Offset
//    .u8     espHdrSize         // ESP Header Size (for command label)
//    .u8     cmdlOffset         // Command Label Offset 
//    .u8     cmdlIVOffset       // IV offset in the command label (for copy data)
//    .u8     pktOffset          // TBD: IV offset in the command label (for copy data)
//.ends     
//
//.enter espTxPass1Scope
//    .assign  struct_espTxPass1Loc1,  R2,  R3, espTxPass1Loc1
//    .assign  struct_espTxPktInst, R22, R25,   espTxPktInst      
//.leave espTxPass1Scope
//
//.enter ctxESPTx1Scope
//    .assign  struct_ctxESPTx1,  R14,  R20, ctxESPTx1
//.leave ctxESPTx1Scope
//
//.enter espHdrScope
//    .assign  struct_espHdr,             R6, R11,  espHdr
//.leave espHdrScope   
//

//******************************************************************************
// * Function:    sap_ipsec_esp_tx_proc_pass1
// ******************************************************************************
// * Description: This function perfroms the first pass of the IPSec ESP Tx packet
// *    header processing as the followings:
// *
// *    - Calculate the Encrypted data length
// *      Calculate data offset to ESP Header, Payload Header and 
// *      ESP Authentication Tag (Hash)
// *    - Insert the ESP Header (SPI, Sequence Number, IV)
// *    - Insert the Encryption command label with the Encrypted data length, 
// *      offset to payload and IV
// *    - Insert the Authentication Command Label with data length and offset 
// *      = (encrypted data size + ESP header size)
// *
// *  Where the packet info consists of
// *        - pointer to the packet
// *        - packet length
// *        - offset to the ESP header
// *
// *  Note: It is assumed that the packets from the host (DSP) will include the ESP
// *        header, payload padding and the ESP Tail which contains the 
// *        valid next header and the room for the hash value (authentication
// *        tag) 
// *
// *
// *****************************************************************************/
//

phpEspTxProc_1:

    // Error Check
    
    
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using espTxPass1Scope
    
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    .using  ctxESPTx1Scope
    xin     XID_SCVDATA, ctxESPTx1, SIZE(ctxESPTx1)
    
    // clear Local Variables (may be just some of them)
    zero    &espTxPass1Loc1,    SIZE(espTxPass1Loc1)    
    
    qbbc    phpEspTxProc_1_OtherChunk_L1,   PktDesc.ChunkInfo.chunkFlag_sop
    
 phpEspTxProc_1_FirstChunk_L1:    
 
    //clear the control flags and other variables
    zero    &espTxPktInst,    SIZE(espTxPktInst)

    qbbs    phpEspTxProc_1_noESNOverFlow, ctxESPTx1.ctrlInfo.flagESPTxESN
    fill    &r0, 4
    qbne    phpEspTxProc_1_noESNOverFlow, r0, ctxESPTx1.esnLo
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4

    .using ctxESPTxScopeStats
    xin     XID_SCVDATA, ctxESPTxStats, SIZE(ctxESPTxStats)
    add     ctxESPTxStats.rollOverCounter, ctxESPTxStats.rollOverCounter, 1
    xout    XID_SCVDATA, ctxESPTxStats, SIZE(ctxESPTxStats)
    .leave ctxESPTxScopeStats
    // Drop packet
    jmp     phpEspTxProc_1_Err_Esn_Overflow

phpEspTxProc_1_noESNOverFlow:
    //Increment the ESN 
    add     ctxESPTx1.esnLo,  ctxESPTx1.esnLo,  1
    adc     ctxESPTx1.esnHi,  ctxESPTx1.esnHi,  0
    
    // Save the updated security context
    xout    XID_SCVDATA,  ctxESPTx1,  SIZE(ctxESPTx1)
    
    //Record the control size - the short payload Info
    sub     espTxPass1Loc1.pktOffset,  PktDesc.ControlSize, 8
    
 phpEspTxProc_1_FirstChunk_L2:  
    mov     espTxPktInst.nextEngId, ctxESPTx1.firstEngId
    
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, espTxPktInst.nextEngId
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    //xout    XID_CDEDATA, PktDesc, SIZE(PktDesc)
    xout    XID_CDEDATA, PktDesc, 8   
      
    // Advance the window past the Descriptor and PS Info
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_CONTROL
    xout    XID_CDECTRL, CdeCmd, 4      
      
    .leave  pktDescScope
    .using protoInfoScope
    // Extract protocol Info
    //xin     XID_CDEDATA, payloadInfoS, SIZE(payloadInfoS)
    // We need to handle both long and short info
    // Short Info: From Host
    // Long Info: From PA (Eflow operation)
    // Note: Both payloadInfoL and protoInfo share R6
    xin     XID_CDEDATA, payloadInfoL, OFFSET(struct_payloadInfoLong.reserved3)
    // extract the command code b7-b5 (0,1) = (long, short info)
    lsr     r0.b0, payloadInfoL.recordLen, 5
    qbeq    phpEspTxProc_ShortInfo_L1, r0.b0, 1
    
phpEspTxProc_LongInfo_L1:
    mov     espTxPass1Loc1.protoOffset,  payloadInfoL.ipsecOffset
    sub     payloadInfoS.len, payloadInfoL.eopOffset, payloadInfoL.ipsecOffset  
    // re-adjust pktOffset
    add     espTxPass1Loc1.pktOffset,  espTxPass1Loc1.pktOffset, 8
    jmp     phpEspTxProc_protoInfo_End
    
phpEspTxProc_ShortInfo_L1:    
    mov     espTxPass1Loc1.protoOffset,  payloadInfoS.offset
    
    // TBD: Issue command to flash out and copy PSInfo
    // Now we're at the control section. Insert the command label here
    // Advance the window past the Descriptor
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4      
    
phpEspTxProc_protoInfo_End:    
    mov     CdeCmd.Cmd0.b1, ctxESPTx1.cmdlTotalLen
    mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
    xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
       
    // Calculate the payload related information
    add     espTxPass1Loc1.espHdrSize,  ctxESPTx1.ivSize, IPSEC_ESP_HDR_BASIC_SIZE_BYTES    
    sub     espTxPass1Loc1.hashDataLen, payloadInfoS.len, ctxESPTx1.icvSize
    sub     espTxPass1Loc1.encryptedDataLen,  espTxPass1Loc1.hashDataLen, espTxPass1Loc1.espHdrSize
    
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4

    // Update byteCount stats
    .using ctxESPTxScopeStats
    xin     XID_SCVDATA, ctxESPTxStats, SIZE(ctxESPTxStats)
    add     ctxESPTxStats.byteCount, ctxESPTxStats.byteCount, espTxPass1Loc1.encryptedDataLen
    adc     ctxESPTxStats.byteCountHi, ctxESPTxStats.byteCountHi, 0
    xout    XID_SCVDATA, ctxESPTxStats, SIZE(ctxESPTxStats)
    .leave ctxESPTxScopeStats
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4

    xin     XID_SCVDATA, ctxESPTx1, SIZE(ctxESPTx1)

    // Calculate and record Various Offsets
    add     espTxPktInst.icvOffset,   espTxPass1Loc1.protoOffset,   espTxPass1Loc1.hashDataLen
    mov     espTxPktInst.icvOffset_pass2, espTxPktInst.icvOffset
    add     espTxPktInst.encryptedOffset, espTxPass1Loc1.protoOffset, espTxPass1Loc1.espHdrSize
    
    // Update HashDataLen if ECN is enabled
    qbbc    phpEspTxProc_1_FirstChunk_L3, ctxESPTx1.ctrlInfo.flagESPTxESN
    add     espTxPass1Loc1.hashDataLen, espTxPass1Loc1.hashDataLen, 4  
   
    .leave protoInfoScope
    
 phpEspTxProc_1_FirstChunk_L3:
    // Advance to the ESP header
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, espTxPass1Loc1.protoOffset, espTxPass1Loc1.pktOffset
    xout    XID_CDECTRL, CdeCmd, 4      
                         
    // Load and processing ESP header
    .using espHdrScope
    xin     XID_CDEDATA, espHdr, SIZE(espHdr)
    mov     espHdr.spi, ctxESPTx1.spi
    mov     espHdr.seqNum,  ctxESPTx1.esnLo
    
    // 
    // Format IV if necessary
    //
    qbeq    phpEspTxProc_1_FormatIV_End, ctxESPTx1.ivSize, 0
    qbbs    phpEspTxProc_1_FormatIV_Random, ctxESPTx1.ctrlInfo.flagESPTxRandomIV
    
phpEspTxProc_1_FormatIV_Unique:     
    // IV should be unique: Use ESN
    mov     espHdr.iv1, ctxESPTx1.esnHi
    mov     espHdr.iv2, ctxESPTx1.esnLo
    qbeq    phpEspTxProc_1_FormatIV_End,  ctxESPTx1.ivSize, 8
    mov     espHdr.iv3, ctxESPTx1.esnHi
    mov     espHdr.iv4, ctxESPTx1.esnLo
    jmp     phpEspTxProc_1_FormatIV_End
    
phpEspTxProc_1_FormatIV_Random:     
    // IV should be random: format the IV from the random number FIFO
    lbco    rnFifoPopCb, cRnFifoCtrl, 0, SIZE(rnFifoPopCb)
    
    // Compare (512 - out) with ivSize 
    mov     r0.w0, phpRnFifoSize
    sub     r0.w2, r0.w0, rnFifoPopCb.out
    //set     r1.w0, rnFifoPopCb.out, 10 //phpRnFifoBase = 0x400(b10)
    qblt    phpEspTxProc_1_FormatIV_L2, ctxESPTx1.ivSize, r0.w2
 
 phpEspTxProc_1_FormatIV_L1:
    // Case 1: (512 - out) >= IV Size
    mov     r0.b2, ctxESPTx1.ivSize
    lbco    espHdr.iv1, cRnFifoBuf, rnFifoPopCb.out, b2
    add     rnFifoPopCb.out, rnFifoPopCb.out, ctxESPTx1.ivSize
    qbne    phpEspTxProc_1_FormatIV_L3, rnFifoPopCb.out, r0.w0
        mov     rnFifoPopCb.out, 0
        jmp     phpEspTxProc_1_FormatIV_L3 
    
 phpEspTxProc_1_FormatIV_L2: 
    // Case 2: (512 - out) < IV Size
    // IV size = 16 and (512 - out) = 8      
    lbco    espHdr.iv1, cRnFifoBuf, rnFifoPopCb.out, 8
    //mov     r1.w0,  phpRnFifoBase 
    lbco    espHdr.iv3, cRnFifoBuf, 0, 8
    mov     rnFifoPopCb.out, 8
 
 phpEspTxProc_1_FormatIV_L3: 
    sbco    rnFifoPopCb.out, cRnFifoCtrl, OFFSET(struct_saRnFifoCb.out), SIZE(rnFifoPopCb.out)
    
 phpEspTxProc_1_FormatIV_End:
    xout    XID_CDEDATA, espHdr, SIZE(espHdr)
    
 phpEspTxProc_1_FirstChunk_L4:  
    //
    //  Command Label Processing
    //
    lbco    r0.w2,  cEspTxTbl, ctxESPTx1.opMode, 2    // load the processing function from the call table
    call    r0.w2                                     // call table function will return to Error or MainLoop         
    
    .leave espHdrScope
    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, espTxPass1Loc1.cmdlOffset 
    mov     CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
    jmp     phpEspTxProc_1_HashProc_L1     
    
 phpEspTxProc_1_OtherChunk_L1:
    // Get packet instance offset 
    // All data chunk in one packet will pass PHP in sequence
    lbco  espTxPktInst, cInstBase, phpContext.instOffset, SIZE(espTxPktInst)
    
    .using pktDescScope
    
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, espTxPktInst.nextEngId
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2  
    
    // Advance the window to the beginning of packet 
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4      
    mov     espTxPass1Loc1.protoOffset,  0   // for non-SOP data chunk
                                             // protoOffset should be zero
    .leave pktDescScope
    
 phpEspTxProc_1_HashProc_L1:
    qbeq    phpEspTxProc_1_End, ctxESPTx1.icvSize, 0    
    qbbc    phpEspTxProc_1_End, ctxESPTx1.ctrlInfo.flagESPTxESN
    qbbs    phpEspTxProc_1_End, espTxPktInst.ctrlFlag.espHashComplete
        
    //qbbs    phpEspTxProc_1_HashProc_L3, espTxPktInst.ctrlFlag.espHashAvail
    qbge    phpEspTxProc_1_HashProc_L2, espTxPktInst.icvOffset, phpContext.chunkSize
    sub     espTxPktInst.icvOffset, espTxPktInst.icvOffset, phpContext.chunkSize
    jmp     phpEspTxProc_1_End  
    
 phpEspTxProc_1_HashProc_L2:
    //set     espTxPktInst.ctrlFlag.espHashAvail
    
 phpEspTxProc_1_HashProc_L3:
    // Hash Data is at this data chunk 
    // Advance to the ICV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, espTxPktInst.icvOffset, espTxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4      
    
    // Insert the upper 32-bit ESN to the packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    mov     CdeCmd.Cmd1, ctxESPTx1.esnHi  
    xout    XID_CDECTRL, CdeCmd, 8 
         
    set     espTxPktInst.ctrlFlag.espHashComplete
 
 phpEspTxProc_1_End:
    // Save Instance Data
    sbco    espTxPktInst, cInstBase, phpContext.instOffset, SIZE(espTxPktInst)
    
    // Save the updated security context
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4   
         
    xout    XID_SCVDATA,  ctxESPTx1,  SIZE(ctxESPTx1)
    
    //close the security context
   mov     CdeCmd.Cmd0, SCV_CMD_DONE
   xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
   // Send this packet (set valid PsInfo to zero)
   mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8) 
   xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
   jmp     MainLoop

 phpEspTxProc_1_Err_Esn_Overflow: 
    
    // Move to the common area to reset the instance index
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW
    xout    XID_SCVCTRL, CdeCmd, 4   
    xin     XID_SCVDATA, ctxComm, SIZE(ctxComm)
    mov     ctxComm.instIndex, 32
    
    xout    XID_SCVDATA, ctxComm, SIZE(ctxComm)
    // 
    // Update the packet descriptor to drop the packet
    // EngId = Output1
    // ThreadId = 7
    // Command label removed  (may be not necessary since it is dropped
    // Set Drop bit 
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket

.using pktDescScope

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPTx1, SIZE(ctxESPTx1)
    
    lbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SwInfo1_ctxPtr_w0)
    mov     PktDesc.SA_NextEngId,   SAENG_OUT1_1
    mov     PktDesc.SA_CmdLblInfo,  ctxESPTx1.cmdlTotalLen
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    set     PktDesc.ChunkInfo.chunkFlag_discard
    sbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SrcID)

    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4   
    // flush out unaligned bytes
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4     
    // Free the instance buffer
    clr     phpContext.instMask, PktDesc.PktId
    
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO <<8)
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command  
   
    jmp     MainLoop

.leave pktDescScope
.leave espTxPass1Scope
.leave ctxESPTx1Scope
.leave commCtxScope    

//******************************************************************************
// * Function:    sap_ipsec_esp_tx_proc_pass2
// ******************************************************************************
// * Description: This function perfroms the second pass of the IPSec ESP Tx packet
// *              header processing as the followings:
// *  
// *   - Insert the Hash value into the ESP Authentication Data location
// *
// *   The following operation should be performed by the host (DSP) already
// *   - Verify the transport type (Extracting the Next Header Type from the ESP Trailer)  
// *   - Update the original IP Header (Protocol Type, total length and checksum)
// *     Should HPS calculate IP Header checksum?
// *   - Update the packet data pointer and length
// *
// *
// *****************************************************************************/

//.struct struct_espTxPass2Loc1
//    .u8     ctrlFlag            // record the protocol payload Size
//#define eop      t0  
//    .u8     pktId   
//.ends     
//
//.enter espTxPass2Scope
//    .assign  struct_espTxPass2Loc1,  R1.w2,  R1.w2,  espTxPass2Loc1
//    .assign  struct_espTxPktInst,      R22, R25, espTxPktInst      
//.leave espTxPass2Scope

phpEspTxProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using espTxPass2Scope
    
    // clear Local Variables (may be just some of them)
    zero    &espTxPass2Loc1,    SIZE(espTxPass2Loc1)    
    
phpEspTxProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    .using  ctxESPTx1Scope
    xin     XID_SCVDATA, ctxESPTx1, SIZE(ctxESPTx1)

    // Record packet instance index and load the instance
    mov     espTxPass2Loc1.pktId,  PktDesc.PktId
    lbco    espTxPktInst, cInstBase,  phpContext.instOffset, SIZE(espTxPktInst)
    
 phpEspTxProc_2_L2:    
    mov     PktDesc.SA_NextEngId, SAENG_OUT1_1
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2
    add     espTxPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize    
    mov     espTxPass2Loc1.chunkSize,  phpContext.chunkSize 
    
    // Update swInfo1 at SOP
    qbbc    phpEspTxProc_2_HashProc_L1, PktDesc.ChunkInfo.chunkFlag_sop
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
 //
 // Hash Processing
 //   
 phpEspTxProc_2_HashProc_L1:
    qbbs    phpEspTxProc_2_HashProc_L9,  espTxPktInst.ctrlFlag.espHashComplete2  
    qbbs    phpEspTxProc_2_HashProc_L7,  espTxPktInst.ctrlFlag.espHashPending 
    qbgt    phpEspTxProc_2_HashProc_L2,  phpContext.chunkSize, espTxPktInst.icvOffset_pass2
    
    .leave pktDescScope
    
    //
    // Hash data is available in the PS Info section
    //
    set     espTxPktInst.ctrlFlag.espHashAvail
    sub     espTxPktInst.hashRecorded, phpContext.chunkSize, espTxPktInst.icvOffset_pass2
    
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and store the Hash Data in the instance buffer 
    // Simulator bug workaround: It will crash simulator if b0 = 0
    qbeq    phpEspTxProc_2_HashProc_L1_1, ctxESPTx1.icvSize, 0
    mov     r0.b0, ctxESPTx1.icvSize
    xin     XID_CDEDATA,  r6,  b0
    add     r0.w2,  phpContext.instOffset, pktInstICVDataOffset
    sbco    r6, cInstBase,  r0.w2, b0 
    
phpEspTxProc_2_HashProc_L1_1:    
    // set ICV offset  
    add     espTxPass2Loc1.icvOffset, espTxPktInst.icvOffset_pass2, espTxPass2Loc1.psCtrlSize
    mov     CdeCmd.Cmd0.w2, espTxPass2Loc1.psCtrlSize
    jmp     phpEspTxProc_2_HashProc_L3    
    
 phpEspTxProc_2_HashProc_L2:
    sub     espTxPktInst.icvOffset_pass2,  espTxPktInst.icvOffset_pass2,  phpContext.chunkSize 
    add     CdeCmd.Cmd0.w2, espTxPass2Loc1.psCtrlSize, 32
    
 phpEspTxProc_2_HashProc_L3: 
#ifdef IPSEC_RN_IV_USE_ENCRYPTED_DATA    
    // Insert EncryptedData of size = ctxESPTx1.ivSize to the RN FIFO 
    qbbc    phpEspTxProc_2_HashProc_L4, phpContext.chunkInfo.chunkFlag_sop
    qbeq    phpEspTxProc_2_HashProc_L4, ctxESPTx1.ivSize, 0
    qbbc    phpEspTxProc_2_HashProc_L4, ctxESPTx1.ctrlInfo.flagESPTxRandomIV

    // Move to the encryption data area 
    // Move to the encryption data offset 
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, CdeCmd.Cmd0.w2, espTxPktInst.encryptedOffset
    xout    XID_CDECTRL, CdeCmd, 4  
    
    lbco    rnFifoPushCb, cRnFifoCtrl, OFFSET(struct_saRnFifoCb.in), SIZE(rnFifoPushCb)
    add     rnFifoPushCb.cnt, rnFifoPushCb.cnt, ctxESPTx1.ivSize
    
    // Move here to use the stall cycles
    mov     r0.w0, phpRnFifoSize
    sub     r0.w2, r0.w0, rnFifoPushCb.in
    
    xin     XID_CDEDATA, r6, 16
    
    // Compare (512 - in) with ivSize   
    // move to use the stall cycles
    // mov     r0.w0, phpRnFifoSize
    // sub     r0.w2, r0.w0, rnFifoPushCb.in
    qblt    phpEspTxProc_2_InsertRN_L2, ctxESPTx1.ivSize, r0.w2
 
 phpEspTxProc_2_InsertRN_L1:
    // Case 1: (512 - out) >= IV Size
    mov     r0.b2, ctxESPTx1.ivSize
    sbco    r6, cRnFifoBuf, rnFifoPushCb.in, b2
    add     rnFifoPushCb.in, rnFifoPushCb.in, ctxESPTx1.ivSize
    qbne    phpEspTxProc_2_InsertRN_L3, rnFifoPushCb.in, r0.w0
    mov     rnFifoPushCb.in, 0
    jmp     phpEspTxProc_2_InsertRN_L3 
    
 phpEspTxProc_2_InsertRN_L2: 
    // Case 2: (512 - in) < IV Size
    // IV size = 16 and (512 - in) = 8      
    sbco    r6, cRnFifoBuf, rnFifoPushCb.in, 8
    sbco    r8, cRnFifoBuf, 0, 8
    mov     rnFifoPushCb.in, 8
 
 phpEspTxProc_2_InsertRN_L3: 
    sbco    rnFifoPushCb, cRnFifoCtrl, OFFSET(struct_saRnFifoCb.in), SIZE(rnFifoPushCb)
    
    // Set the window offset to the ICV location if hash available at the same data chunk  and adjust pktLoc
    sub     espTxPass2Loc1.icvOffset, espTxPktInst.icvOffset_pass2, espTxPktInst.encryptedOffset
    mov     espTxPass2Loc1.pktLoc, espTxPktInst.encryptedOffset
    
    // It must be the first data chunk, therefore there is no need to process byte insertion 
    qbbc    phpEspTxProc_2_HashProc_End, espTxPktInst.ctrlFlag.espHashAvail
    
    // Hash is available at the first data chunk
    jmp     phpEspTxProc_2_HashProc_L4_1
    
#endif    
    
 phpEspTxProc_2_HashProc_L4:    
    
    qbbc    phpEspTxProc_2_HashProc_L9, espTxPktInst.ctrlFlag.espHashAvail
    
#ifdef TO_BE_DELETE    
    // Verify whether we need to insert leftover bytes when this chunk contains hash
    // Note: Leftover does not occur at the first data chunk
    //       Therefore, there is no need to deal with the Encryption IV processing case   
    // If leftover bytes not available, continue normal processing
    // If leftover bytes available, the CDE window will stay at beginning of Packet descriptor or PS Info, pktLoc = 0
    // 
    qbbc     phpEspTxProc_2_HashProc_L4_1,  espTxPktInst.ctrlFlag.espLeftoverBytes
    // Mov to the packet  location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // re-adjust the icvOffset
    sub     espTxPass2Loc1.icvOffset, espTxPass2Loc1.icvOffset,  espTxPass2Loc1.psCtrlSize
    
    // insert the leftover bytes
    add     espTxPass2Loc1.chunkSize, espTxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, espTxPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
#endif    
    
phpEspTxProc_2_HashProc_L4_1:
                                                                                  
    //
    // Corner case: ESN is the last 4-byte in the current data chunk
    //              (espTxPktInst.hashRecorded == 4) &&
    //              (ctxESPTx1.icvSize != 0) &&
    //              (ctxESPTx1.ctrlInfo.flagESPTxESN == TRUE)
    //              Need to remember and flush out data in front of ESN
    //
    qbne    phpEspTxProc_2_HashProc_L4_2,  espTxPktInst.hashRecorded, 4
    qbeq    phpEspTxProc_2_HashProc_L4_2,  ctxESPTx1.icvSize, 0    
    qbbc    phpEspTxProc_2_HashProc_L4_2,  ctxESPTx1.ctrlInfo.flagESPTxESN
          
#ifdef TO_BE_DELETE    
    set     espTxPass2Loc1.ctrlFlag.leftoverVerified
    // Should we record and remove unaligned bytes due to output stream interface problem
    and     r10.w0,  espTxPass2Loc1.chunkSize,    0x3 
    clr     espTxPktInst.ctrlFlag.espLeftoverBytes
    qbeq    phpEspTxProc_2_HashProc_L4_2,  r10.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, espTxPass2Loc1.icvOffset,  2   
    xout    XID_CDECTRL, CdeCmd, 4   
    
    xin     XID_CDEDATA, r6,  2
    mov     espTxPktInst.leftoverBytes,   r6.w2
    set     espTxPktInst.ctrlFlag.espLeftoverBytes
     
    // flush out unaligned bytes and the ESN
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    mov     espTxPktInst.hashRecorded, 0
    jmp     phpEspTxProc_2_HashProc_L6  
         
#endif    
    

phpEspTxProc_2_HashProc_L4_2:
    
    // Advance the window to the icv location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, espTxPass2Loc1.icvOffset
    xout    XID_CDECTRL, CdeCmd, 4   
    mov     espTxPass2Loc1.pktLoc, espTxPktInst.icvOffset_pass2  
    
    qbeq    phpEspTxProc_2_HashProc_L5,  ctxESPTx1.icvSize, 0    
    qbbc    phpEspTxProc_2_HashProc_L5,  ctxESPTx1.ctrlInfo.flagESPTxESN
    
    // flush out ESN
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 4
    xout    XID_CDECTRL, CdeCmd, 4
    sub     espTxPktInst.hashRecorded, espTxPktInst.hashRecorded, 4
    add     espTxPass2Loc1.pktLoc,  espTxPass2Loc1.pktLoc, 4  
      
 phpEspTxProc_2_HashProc_L5:
 
    // It is illegal that the length of "LBCO" is zero:special case
    qbeq      phpEspTxProc_2_HashProc_L6, espTxPktInst.hashRecorded, 0
 
    // Patch the Hash data  
    min     r0.b0, espTxPktInst.hashRecorded, ctxESPTx1.icvSize
    add     r0.w2,  phpContext.instOffset, pktInstICVDataOffset
    lbco    r6, cInstBase,  r0.w2, b0 
    xout    XID_CDEDATA,    r6,    b0  
    
    qbeq    phpEspTxProc_2_HashProc_L8, r0.b0, ctxESPTx1.icvSize 
    mov     espTxPktInst.hashRecorded, r0.b0
    
 phpEspTxProc_2_HashProc_L6:
    set     espTxPktInst.ctrlFlag.espHashPending
    jmp     phpEspTxProc_2_HashProc_End
    
 phpEspTxProc_2_HashProc_L7:
    // Update the remaining Hash data at the top of packet
    // Mov to the hash location (pktLoc = 0)
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
                                            
#ifdef TO_BE_DELETE   
    // Verify whether we need to insert leftover bytes
    qbbc     phpEspTxProc_2_HashProc_L7_1,  espTxPktInst.ctrlFlag.espLeftoverBytes
    
    // insert the leftover bytes
    add     espTxPass2Loc1.chunkSize,  espTxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, espTxPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
#endif    
    
 phpEspTxProc_2_HashProc_L7_1:
    sub     r0.b0, ctxESPTx1.icvSize, espTxPktInst.hashRecorded
    add     r0.w2,  phpContext.instOffset, pktInstICVDataOffset
    add     r0.w2,  r0.w2, espTxPktInst.hashRecorded
    lbco    r6, cInstBase,  r0.w2, b0 
    xout    XID_CDEDATA,    r6,    b0  
    
 phpEspTxProc_2_HashProc_L8:
    set     espTxPktInst.ctrlFlag.espHashComplete
    jmp      phpEspTxProc_2_HashProc_End
    
 phpEspTxProc_2_HashProc_L9:
    // Mov to the packet location (pktLoc = 0)
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
 
#ifdef TO_BE_DELETE   
    // Verify whether we need to insert leftover bytes
    qbbc     phpEspTxProc_2_HashProc_End,  espTxPktInst.ctrlFlag.espLeftoverBytes
    
    // insert the leftover bytes
    add     espTxPass2Loc1.chunkSize,  espTxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, espTxPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8 
#endif      
    
 phpEspTxProc_2_HashProc_End:
    qbbc    phpEspTxProc_2_L3,  phpContext.chunkInfo.chunkFlag_eop
    //
    //  Process EOP Data Chunk
    //
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4

    // Update byteCount stats
    .using ctxESPTxScopeStats
    xin     XID_SCVDATA, ctxESPTxStats, SIZE(ctxESPTxStats)
    add     ctxESPTxStats.numTxPkts,    ctxESPTxStats.numTxPkts,   1
    adc     ctxESPTxStats.numTxPktsHi,  ctxESPTxStats.numTxPktsHi, 0
    xout    XID_SCVDATA, ctxESPTxStats, SIZE(ctxESPTxStats)
    .leave ctxESPTxScopeStats
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_ESP_ENC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_ESP_ENC_HI_OFFSET, 8

    // Restore ctxESPTx1
    xin     XID_SCVDATA, ctxESPTx1, SIZE(ctxESPTx1)
    
    clr     phpContext.instMask, espTxPass2Loc1.pktId
    jmp     phpEspTxProc_2_End
    
 phpEspTxProc_2_L3:
    //
    //  Process non-EOP data chunk
    // 
    
#ifdef TO_BE_DELETE   
    
    // check whether the leftover condition is aleady verified
    qbbs    phpEspTxProc_2_L4,  espTxPass2Loc1.ctrlFlag.leftoverVerified
    
    // Should we record and remove unaligned bytes dueto output stream interface problem
    and     r7.w0,  espTxPass2Loc1.chunkSize,    0x3 
    clr     espTxPktInst.ctrlFlag.espLeftoverBytes
    qbeq    phpEspTxProc_2_L4,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, phpContext.chunkSize,   espTxPass2Loc1.pktLoc
    sub     CdeCmd.Cmd0.w2, CdeCmd.Cmd0.w2,     2   
    xout    XID_CDECTRL, CdeCmd, 4   
    
    xin     XID_CDEDATA, r6,  2
    mov     espTxPktInst.leftoverBytes,   r6.w2
    set     espTxPktInst.ctrlFlag.espLeftoverBytes
     
    // flush out unaligned bytes
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
#endif
          
 phpEspTxProc_2_L4:         
    //  Save the instance Data
    sbco    espTxPktInst, cInstBase, phpContext.instOffset, SIZE(espTxPktInst)
    
 phpEspTxProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop

.leave espTxPass2Scope
.leave ctxESPTx1Scope
.leave commCtxScope    
 

   