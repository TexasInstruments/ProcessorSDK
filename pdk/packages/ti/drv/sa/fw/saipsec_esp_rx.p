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
//  * FILE PURPOSE:  PA firmware for PDSPs 
//  **********************************************************************************
//  * FILE NAME:  saipsec_esp_rx.p
//  *
//  * DESCRIPTION: Process the IPSEC ESP Rx (From-Network) packets
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
//* (C) Copyright 2009 Texas Instruments, Inc.
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
//  *  R8       |  Note: Can be used as general registers and context viewer when
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

//.struct struct_espRxPass1Loc1
//    .u16    hashDataLen        // Hash Data length (for command label)
//    .u16    encryptedDataLen   // Encrypted Data length (for command label)
//    .u8     protoOffset        // Protocol Offset
//    .u8     espHdrSize         // ESP Header Size (for command label)
//    .u8     auxDataSize        // Auxulary Data Size 
//    .u8     cmdlOffset         // Command Label Offset 
//.ends   
//
//.struct struct_espRxPass1Loc2
//    .u32    spi                // Maybe removed
//.ends     
//
//.enter espRxPass1Scope
//    .assign  struct_espRxPass1Loc1,  R1,  R2, espRxPass1Loc1
//    .assign  struct_espRxPass1Loc2,  R3,  R3, espRxPass1Loc2
//    .assign  struct_espPktInst, R22, R25,   espPktInst      
//.leave espRxPass1Scope
//
//.enter ctxESPRxScope
//    .assign  struct_ctxESPRxStats,        R14, R15, ctxESPRxStats
//    .assign  struct_ctxESPRxStatsReplay,  R6,  R9,  ctxESPRxStatsReplay    
//    .assign  struct_ctxESPRx1,            R14, R17, ctxESPRx1
//.leave ctxESPRxScope
//
//.enter replayCtrlScope
//    .assign struct_replayCtrl,            R14, R21, replayCtrl 
//.leave replayCtrlScope     
//
///******************************************************************************
// * Function:    sap_ipsec_esp_rx_proc_pass1
// ******************************************************************************
// * Description: This function perfroms the first pass of the IPSec ESP Rx packet
// *              header processing as the followings: 
// * 
// *    - Calculate the Encrypted data length
// *      Calculate data offset to ESP Header, Payload Header and 
// *      ESP Authentication Tag (Hash)
// *    - Replay check  (Extracting the Sequence Number from the ESP Header, range check)
// *    - Insert the Authentication Command Label with data length and offset 
// *      = (encrypted data size + ESP header size)
// *    - Insert the Encryption command label with the Encrypted data length, 
// *      offset to payload and IV
// *    - Insert the HPS2 command label
// *
// *  Where the packet info consists of
// *        - pointer to the packet
// *        - packet length
// *        - offset to the ESP header
// *
// *****************************************************************************/

phpEspRxProc_1:

    // Error Check
    
    
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using espRxPass1Scope
    .using ctxESPRxScope
    
    // Advance the SC Viewer the general portion of IPSec Rx security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPRx.ctrlInfo)) << 8)

    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRx1, SIZE(ctxESPRx1)
    
    // clear Local Variables (may be just some of them)
    zero    &espRxPass1Loc1, SIZE(espRxPass1Loc1)    
    
    qbbc    phpEspRxProc_1_OtherChunk_L1,   PktDesc.ChunkInfo.chunkFlag_sop
    
 phpEspRxProc_1_FirstChunk_L1:    
 
    //pkt Instance Initialization: clear the control flags and other variables
    zero    &espPktInst,    SIZE(espPktInst)
    
 phpEspRxProc_1_FirstChunk_L2:  
    mov     espPktInst.nextEngId, ctxESPRx1.firstEngId
    
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, espPktInst.nextEngId
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    //xout    XID_CDEDATA, PktDesc, SIZE(PktDesc) 
    xout    XID_CDEDATA, PktDesc, 8  
      
    // Advance the window past the Descriptor
    // Move is desc size + phys PsInfo size
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_CONTROL
    xout    XID_CDECTRL, CdeCmd, 4    
      
    .leave  pktDescScope
    .using protoInfoScope
    // Extract protocol Info
    xin     XID_CDEDATA, payloadInfoL, SIZE(struct_payloadInfoLong)
    mov     espRxPass1Loc1.protoOffset, payloadInfoL.ipsecOffset
    add     payloadInfoL.startOffset, payloadInfoL.startOffset, ctxESPRx1.ivSize
    sub     payloadInfoL.eopOffset, payloadInfoL.eopOffset, ctxESPRx1.icvSize
    and     payloadInfoL.nextHdrType, payloadInfoL.nextHdrType, 0xE0
    add     payloadInfoL.nextHdrType, payloadInfoL.nextHdrType, SA_PAYLOAD_INFO_NEXT_HDR_TYPE_DECODED_ESP
    xout    XID_CDEDATA, payloadInfoL, OFFSET(struct_payloadInfoLong.reserved4)
    
    // Now we're at the control section. Insert the command label here
    // Note: we need to keep the protocol command for the next PA parsing 
    // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    // mov     CdeCmd.Cmd0.w2, 4
    // xout    XID_CDECTRL, CdeCmd, 4      
    
    mov     CdeCmd.Cmd0.b1, ctxESPRx1.cmdlTotalLen
    mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
    xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
       
    // Calculate the payload related information and icv_offset
    add     espRxPass1Loc1.espHdrSize,  ctxESPRx1.ivSize, IPSEC_ESP_HDR_BASIC_SIZE_BYTES    
    sub     espRxPass1Loc1.hashDataLen, payloadInfoL.eopOffset, payloadInfoL.ipsecOffset  
    sub     espRxPass1Loc1.encryptedDataLen,  espRxPass1Loc1.hashDataLen, espRxPass1Loc1.espHdrSize
    
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPRx.numRxPkts)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4
    xin     XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)
    add     ctxESPRxStats.byteCount, ctxESPRxStats.byteCount, espRxPass1Loc1.encryptedDataLen
    adc     ctxESPRxStats.byteCountHi, ctxESPRxStats.byteCountHi, 0
    xout    XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)

    // Restore ctxESPRx1
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxAHRx.ctrlInfo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4  
          
    xin     XID_SCVDATA, ctxESPRx1, SIZE(ctxESPRx1)

    // Calculate and record Various Offsets
    mov     espPktInst.icvOffset,  payloadInfoL.eopOffset
    mov     espPktInst.icvOffset_pass2, espPktInst.icvOffset
    
    // Update HashDataLen if ECN is enabled
    qbbc    phpEspRxProc_1_FirstChunk_L3, ctxESPRx1.ctrlInfo.flagESPRxESN
    add     espRxPass1Loc1.hashDataLen, espRxPass1Loc1.hashDataLen, 4  

 phpEspRxProc_1_FirstChunk_L3:
    // Advance to the ESP header
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    //add     CdeCmd.Cmd0.w2, espRxPass1Loc1.protoOffset, payloadInfoL.recordLen
    mov     CdeCmd.Cmd0.w2, espRxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4      
                         
    .leave protoInfoScope
                         
    // Load and processing ESP header
    .using espHdrScope
    xin     XID_CDEDATA, espHdr, SIZE(espHdr)
    
    // TBD: we may want to verify SPI against the one at the security context
    // mov  espHdr.spi, ctxESPRx1.spi
    // mov  espRxPass1Loc2.spi, espHdr.spi
    mov     espPktInst.esnLo, espHdr.seqNum
    
    .leave espHdrScope
    
 phpEspRxProc_1_FirstChunk_L4:  
 
    // Perform ESN update if configured
    qbbc   phpEspRxProc_1_FirstChunk_L6, ctxESPRx1.ctrlInfo.flagESPRxReplay
    qbbc   phpEspRxProc_1_FirstChunk_L6, ctxESPRx1.ctrlInfo.flagESPRxESN
    
    //
    // ESN update
    //
    // Advance the SC Viewer the first part of Rx security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPRx.winBaseHi))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRxStatsReplay, SIZE(ctxESPRxStatsReplay)
    
    // Do the subract first so we are immune to 32-bit wrap R0 = seq_num_diff
    sub     r0, espPktInst.esnLo,  ctxESPRxStatsReplay.winBase
    
    // If the delta is "negative", then this packet comes before our base.
    // It is equivalent to (seq_num_diff > 2^31)
    // the receive packet is well outside the replay window, drop it
    qbbs  phpEspRxProc_1_Err_OldPkt,  r0.t31 
    
    qbgt  phpEspRxProc_1_FirstChunk_L5,  espPktInst.esnLo,  ctxESPRxStatsReplay.winBase
    mov   espPktInst.esnHi, ctxESPRxStatsReplay.winBaseHi
    jmp   phpEspRxProc_1_FirstChunk_L6
    
phpEspRxProc_1_FirstChunk_L5: 
    add   espPktInst.esnHi, ctxESPRxStatsReplay.winBaseHi, 1
    
phpEspRxProc_1_FirstChunk_L6:

    // Re-load the ESP header
    .using espHdrScope
    xin     XID_CDEDATA, espHdr, SIZE(espHdr)
    .leave espHdrScope
    
    //
    //  Command Label Processing
    //
    lbco    r0.w2,  cEspRxTbl, ctxESPRx1.opMode, 2    // load the processing function from the call table
    call    r0.w2                                     // call table function will return to Error or MainLoop         
    

phpEspRxProc_1_FirstChunk_L7:
    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, espRxPass1Loc1.cmdlOffset 
    mov32   CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
    jmp     phpEspRxProc_1_HashProc_L1     
    
 phpEspRxProc_1_OtherChunk_L1:
    // Load packet Instance
    lbco    espPktInst, cInstBase, phpContext.instOffset, SIZE(espPktInst)
    
    .using pktDescScope
    
 phpEspRxProc_1_OtherChunk_L2:
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, espPktInst.nextEngId
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    //xout    XID_CDEDATA, PktDesc, SIZE(PktDesc)
    xout    XID_CDEDATA, PktDesc, 8  
    
    // Advance the window to the beginning of packet 
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4      
    mov     espRxPass1Loc1.protoOffset,  0   // for non-SOP data chunk
                                             // protoOffset should be zero
    .leave pktDescScope
    
 phpEspRxProc_1_HashProc_L1:
    qbeq    phpEspRxProc_1_End, ctxESPRx1.icvSize, 0    
    qbbs    phpEspRxProc_1_End, espPktInst.ctrlFlag.espHashComplete
        
    qbbs    phpEspRxProc_1_HashProc_L3, espPktInst.ctrlFlag.espHashAvail
    qbge    phpEspRxProc_1_HashProc_L2, espPktInst.icvOffset, phpContext.chunkSize
    sub     espPktInst.icvOffset, espPktInst.icvOffset, phpContext.chunkSize
    jmp     phpEspRxProc_1_End  
    
 phpEspRxProc_1_HashProc_L2:
    set     espPktInst.ctrlFlag.espHashAvail
    
 phpEspRxProc_1_HashProc_L3:
    // Hash Data is at this data chunk 
    // Advance to the ICV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, espPktInst.icvOffset, espRxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4      
    
    qbbs    phpEspRxProc_1_HashProc_L6, espPktInst.ctrlFlag.espHashPending
    
    sub     r0.w0, phpContext.chunkSize, espPktInst.icvOffset
    min     espPktInst.hashRecorded, r0.w0, ctxESPRx1.icvSize
    
    // It is illegal that the length of "SBCO" is 0: special case
    qbeq    phpEspRxProc_1_HashProc_L4,  espPktInst.hashRecorded, 0 
    
    // Copy the hash data to the instance buffer
    mov     r0.b0, espPktInst.hashRecorded 
    xin     XID_CDEDATA, r6, b0
    add     r0.w2,  phpContext.instOffset, pktInstICVDataOffset
    sbco    r6, cInstBase,  r0.w2, b0 
     
 phpEspRxProc_1_HashProc_L4:        
    
    qbbc    phpEspRxProc_1_HashProc_L5, ctxESPRx1.ctrlInfo.flagESPRxESN
    // Inset the upper 32-bit ESN to the packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    mov     CdeCmd.Cmd1, espPktInst.esnHi  
    xout    XID_CDECTRL, CdeCmd, 8 
         
 phpEspRxProc_1_HashProc_L5:    
    qbeq    phpEspRxProc_1_HashProc_L7, espPktInst.hashRecorded, ctxESPRx1.icvSize 
    mov     espPktInst.icvOffset, 0
    set     espPktInst.ctrlFlag.espHashPending
    jmp     phpEspRxProc_1_End
    
 phpEspRxProc_1_HashProc_L6:
    // Processing Pending Hash data
    // copy the hash data out
    sub     r0.b0, ctxESPRx1.icvSize, espPktInst.hashRecorded 
    xin     XID_CDEDATA, r6, b0
    add     r0.w2,  phpContext.instOffset, pktInstICVDataOffset
    add     r0.w2,  r0.w2, espPktInst.hashRecorded
    sbco    r6, cInstBase,  r0.w2, b0 
    
 phpEspRxProc_1_HashProc_L7:
    set     espPktInst.ctrlFlag.espHashComplete
 
 phpEspRxProc_1_End:
    // Save Instance Data
    sbco    espPktInst, cInstBase, phpContext.instOffset, SIZE(espPktInst)
    
 phpEspRxProc_1_End_L1:       
    //close the security context
   mov     CdeCmd.Cmd0, SCV_CMD_DONE
   xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
   // Send this packet (set valid PsInfo to zero)
   mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8) 
   xout    XID_CDECTRL, CdeCmd, 4                  // Send the command    
   jmp     MainLoop
 
 phpEspRxProc_1_Err_OldPkt:
    // Updates the statistics
    add     ctxESPRxStatsReplay.numOldPkts, ctxESPRxStatsReplay.numOldPkts, 1
    xout    XID_SCVDATA, ctxESPRxStatsReplay, SIZE(ctxESPRxStatsReplay)
    // Update System Statistics
    lbco    r5, cStats, PDSP_ESP_REPLAY_OLD_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_ESP_REPLAY_OLD_OFFSET, 4
    
    // Move to the common area to reset the instance index
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW
    xout    XID_SCVCTRL, CdeCmd, 4   
    xin     XID_SCVDATA, ctxComm, SIZE(ctxComm)
    mov     ctxComm.instIndex, 32
    
    xout    XID_SCVDATA, ctxComm, SIZE(ctxComm)

 phpEspRxProc_1_Err_Common:
    // 
    // Update the packet descriptor to drop the packet
    // EngId = Output1
    // ThreadId = 7
    // Command label removed (may not be necessary since packet is dropped 
    // Set Drop bit 
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket

.using pktDescScope

    // Move back to the SC Viewer the general portion of IPSec Rx security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPRx.ctrlInfo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRx1, SIZE(ctxESPRx1)
    
    lbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SwInfo1_ctxPtr_w0)
    mov     PktDesc.SA_NextEngId,   SAENG_OUT1_1
    mov     PktDesc.SA_CmdLblInfo,  ctxESPRx1.cmdlTotalLen
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    set     PktDesc.ChunkInfo.chunkFlag_discard
    sbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SrcID)
    
    //
    // Hardware patch: make sure that the output data chunk is 4-byte aligned
    // The only error case is the ESN verification failure and the packet location is at espRxPass1Loc1.protoOffset
    and     r7.w0,  phpContext.chunkSize,  0x3 
    qbeq    phpEspRxProc_1_Err_Common_L1,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, phpContext.chunkSize,   espRxPass1Loc1.protoOffset
    sub     CdeCmd.Cmd0.w2, CdeCmd.Cmd0.w2,     2   
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // flush out unaligned bytes
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    
phpEspRxProc_1_Err_Common_L1:
    
    // Free the instance buffer
    clr     phpContext.instMask, PktDesc.PktId
    
    jmp    phpEspRxProc_1_End_L1
     
.leave pktDescScope
.leave espRxPass1Scope
.leave ctxESPRxScope
.leave commCtxScope    

// ******************************************************************************
// * Function:    sap_ipsec_esp_rx_proc_pass2
// ******************************************************************************
// * Description: This function perfroms the first pass of the IPSec ESP Rx packet
// *              header processing as the followings:  
// *    - Compare the Hash value against the Extracted Hash value (Upto ?? bytes)
// *    - Update the Replay window (Expected Sequence Number & Replay Mask Array)
// *    
// *   The following operation should be performed by the host (DSP)
// *    - Verify the transport type (Extracting the Next Header Type from the ESP Trailer) 
// *    - Verify the ESP Padding (1, 2, 3, 4 , ....)
// *    - Restore the original IP Header (Protocol Type, total length and checksum)
// *      Should HPS calculate IP Header checksum? No
// *    - Remove the ESP Header (and trailer)
// *    - Update the packet data pointer and length
// *
// *****************************************************************************/
//
//.struct struct_espRxPass2Loc1
//    .u8     ctrlFlag            // record the protocol payload Size
//    .u8     pktId   
//.ends     
//
//.enter espRxPass2Scope
//    .assign  struct_espRxPass2Loc1,  R1.w2,  R1.w2,  espRxPass2Loc1
//    .assign  struct_espPktInst,      R22, R25, espPktInst      
//.leave espRxPass2Scope

phpEspRxProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using espRxPass2Scope
    .using ctxESPRxScope
    
    // clear Local Variables (may be just some of them)
    zero    &espRxPass2Loc1,    SIZE(espRxPass2Loc1)    
    
phpEspRxProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPRx.ctrlInfo))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxESPRx1, SIZE(ctxESPRx1)

    // Record the packet instance index and load the instance
    mov     espRxPass2Loc1.pktId,  PktDesc.PktId
    lbco    espPktInst, cInstBase,  phpContext.instOffset, SIZE(espPktInst)
    
    // setup destination Id
    // Initialize local variable
    mov     PktDesc.SA_NextEngId, SAENG_OUT1_1
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2
    
    // Update swInfo1 at SOP
    qbbc    phpEspRxProc_2_L1_1, PktDesc.ChunkInfo.chunkFlag_sop
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
phpEspRxProc_2_L1_1:
    
    add     espRxPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize 
    mov     espRxPass2Loc1.chunkSize,  phpContext.chunkSize 
    
    qbbc    phpEspRxProc_2_L1_2,  ctxESPRx1.ctrlInfo.flagESPRxESN
    set     espRxPass2Loc1.ctrlFlag.espRxESN
    
phpEspRxProc_2_L1_2:    
    // Packet Discard Check
    qbbc    phpEspRxProc_2_L3, espPktInst.ctrlFlag.espPktError
	 // Discard the packet, make sure the last data chunk has the discard bit set
     set    PktDesc2.chunkInfo.chunkFlag_discard
     xout   XID_CDEDATA, PktDesc2.chunkInfo, 1

    // Verify whether we need to insert leftover bytes
    qbbc    phpEspRxProc_2_L2,  espPktInst.ctrlFlag.espLeftoverBytes
    
    // Advance the window to the packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // insert the leftover bytes
    add     espRxPass2Loc1.chunkSize,  espRxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, espPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
 phpEspRxProc_2_L2: 
    qbbs    phpEspRxProc_2_L4,  phpContext.chunkInfo.chunkFlag_eop    // EOP processing
    jmp     phpEspRxProc_2_L5                                         // non-EOP processing
    
 phpEspRxProc_2_L3:    
    
 //
 // Hash Processing
 //   
 phpEspRxProc_2_HashProc_L1:
 
    qbeq    phpEspRxProc_2_HashProc_End, ctxESPRx1.icvSize, 0
    qbbs    phpEspRxProc_2_HashProc_End, espPktInst.ctrlFlag.espHashComplete2 
    qbbs    phpEspRxProc_2_HashProc_L2,  espPktInst.ctrlFlag.espHashAvail2   
    qbgt    phpEspRxProc_2_HashProc_L6, phpContext.chunkSize, espPktInst.icvOffset_pass2
    
    .leave pktDescScope
    
    //
    // Hash data is available in the PS Info section
    //
    set     espPktInst.ctrlFlag.espHashAvail2
    set     espRxPass2Loc1.ctrlFlag.leftoverInserted 
    
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and store the Hash Data
    mov     r0.b0, ctxESPRx1.icvSize
    xin     XID_CDEDATA,  r6,  b0
    
    // set pktLoc
    mov     espRxPass2Loc1.pktLoc,  espPktInst.icvOffset_pass2
    
    // Mov to the packet location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Move here to save some stall             
    add     r0.w2,  phpContext.instOffset, pktInstICVDataOffset2
    sbco    r6, cInstBase,  r0.w2, b0 
    
    // Verify whether we need to insert leftover bytes when this chunk contains hash
    qbbc     phpEspRxProc_2_HashProc_L1_1,  espPktInst.ctrlFlag.espLeftoverBytes
    
    // insert the leftover bytes
    add     espRxPass2Loc1.chunkSize, espRxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, espPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
phpEspRxProc_2_HashProc_L1_1:    
    //
    // Corner case: ESN is the last 4-byte in the current data chunk
    //              (espTxPktInst.hashRecorded == 4) &&
    //              (ctxESPRx1.ctrlInfo.flagESPRxESN == TRUE)
    //              Need to remember and flush out data in front of ESN
    //
    sub     r0.w2,  phpContext.chunkSize, espPktInst.icvOffset_pass2
    qbne    phpEspRxProc_2_HashProc_L1_2,  r0.w2, 4
    qbbc    phpEspRxProc_2_HashProc_L1_2,  ctxESPRx1.ctrlInfo.flagESPRxESN
    
    set     espRxPass2Loc1.ctrlFlag.leftoverVerified
    // Should we record and remove unaligned bytes due to output stream interface problem
    and     r7.w0,  espRxPass2Loc1.chunkSize,    0x3 
    clr     espPktInst.ctrlFlag.espLeftoverBytes
    qbeq    phpEspRxProc_2_HashProc_L1_2,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, espPktInst.icvOffset_pass2,  2   
    xout    XID_CDECTRL, CdeCmd, 4   
    
    xin     XID_CDEDATA, r6,  2
    mov     espPktInst.leftoverBytes,   r6.w2
    set     espPktInst.ctrlFlag.espLeftoverBytes
     
    // flush out unaligned bytes and the ESN
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
    jmp     phpEspRxProc_2_HashProc_L2
    
phpEspRxProc_2_HashProc_L1_2:
    
    // Advance the window to the icv location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, espPktInst.icvOffset_pass2
    xout    XID_CDECTRL, CdeCmd, 4 
    
    qbbc    phpEspRxProc_2_HashProc_L2,  ctxESPRx1.ctrlInfo.flagESPRxESN
    // flush out ESN
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 4
    xout    XID_CDECTRL, CdeCmd, 4
    add     espRxPass2Loc1.pktLoc,  espRxPass2Loc1.pktLoc, 4
    
 phpEspRxProc_2_HashProc_L2:
    // Verify whether the complete hash in the packet is recorded
    qbbc    phpEspRxProc_2_HashProc_End, espPktInst.ctrlFlag.espHashComplete
      
 phpEspRxProc_2_HashProc_L3:
    // Indicate Pass 2 Hash comparison is completed
    set espPktInst.ctrlFlag.espHashComplete2     
    
    //  ESP Hash Comparing
    //
    add     r0.b0, ctxESPRx1.icvSize, 16 
    add     r0.w2, phpContext.instOffset, pktInstICVDataOffset
    lbco    r6,    cInstBase, r0.w2, b0 
    ipsec_icv_check phpEspRxProc_2_Err_Hash, phpEspRxProc_2_Replay_L1, ctxESPRx1.icvSize
    
    //
    //  Replay check
    //
phpEspRxProc_2_Replay_L1:
    
    qbbc    phpEspRxProc_2_HashProc_End, ctxESPRx1.ctrlInfo.flagESPRxReplay
    // Advance the SC Viewer the replay Ctrl
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    .using replayCtrlScope
    //xin     XID_SCVDATA, replayCtrl, SIZE(replayCtrl) ??
    xin     XID_SCVDATA, replayCtrl, 32
    mov     r29, espPktInst.esnLo   
    call    Replay_CheckAndUpdate
    
    // Verify the replay check result
    qbeq    phpEspRxProc_2_ReplayPass, r29.b0, SA_REPLAY_RC_OK
    qbeq    phpEspRxProc_2_ReplayPass, r29.b0, SA_REPLAY_RC_NEW
    
    
    // Error Handling 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)
    
    qbeq    phpEspRxProc_2_ReplayDup, r29.b0, SA_REPLAY_RC_DUP
    
 phpEspRxProc_2_ReplayOld:
    add     ctxESPRxStats.numOldPkts, ctxESPRxStats.numOldPkts, 1
    // Update System Statistics
    lbco    r5, cStats, PDSP_ESP_REPLAY_OLD_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_ESP_REPLAY_OLD_OFFSET, 4
    jmp     phpEspRxProc_2_Err_Replay
 phpEspRxProc_2_ReplayDup:
    add     ctxESPRxStats.numDupPkts, ctxESPRxStats.numDupPkts, 1
    // Update System Statistics
    lbco    r5, cStats, PDSP_ESP_REPLAY_DUP_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_ESP_REPLAY_DUP_OFFSET, 4
    jmp     phpEspRxProc_2_Err_Replay
     
 phpEspRxProc_2_ReplayPass:
    qbbc    phpEspRxProc_2_ReplayPass_L2, espRxPass2Loc1.ctrlFlag.espRxESN
    qbgt    phpEspRxProc_2_ReplayPass_L2, replayCtrl.winBase, espPktInst.esnLo
    mov     replayCtrl.winBaseHi, espPktInst.esnHi
    
 phpEspRxProc_2_ReplayPass_L2:  
    xout    XID_SCVDATA, replayCtrl, SIZE(replayCtrl)
    jmp     phpEspRxProc_2_HashProc_End
    
 phpEspRxProc_2_HashProc_L6:
    sub     espPktInst.icvOffset_pass2,  espPktInst.icvOffset_pass2,  phpContext.chunkSize 
    
 phpEspRxProc_2_HashProc_End:
    qbbs    phpEspRxProc_2_HashProc_End_L1, espRxPass2Loc1.ctrlFlag.leftoverInserted
    // Update the remaining Hash data at the top of packet
    // Mov to the hash location (pktLoc = 0)
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Verify whether we need to insert leftover bytes
    qbbc     phpEspRxProc_2_HashProc_End_L1,  espPktInst.ctrlFlag.espLeftoverBytes
    
    // insert the leftover bytes
    add     espRxPass2Loc1.chunkSize,  espRxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, espPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8   
    
 phpEspRxProc_2_HashProc_End_L1:
    qbbc    phpEspRxProc_2_L5, phpContext.chunkInfo.chunkFlag_eop
    //
    //  Process EOP Data Chunk
    //
    qbbs    phpEspRxProc_2_L4, espPktInst.ctrlFlag.espPktError
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(replayCtrl)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)
    add     ctxESPRxStats.numRxPkts,   ctxESPRxStats.numRxPkts, 1
    adc     ctxESPRxStats.numRxPktsHi, ctxESPRxStats.numRxPktsHi, 0
    xout    XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)
    .leave replayCtrlScope
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_ESP_DEC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_ESP_DEC_HI_OFFSET, 8
    
 phpEspRxProc_2_L4:
    
    clr     phpContext.instMask, espRxPass2Loc1.pktId
    jmp     phpEspRxProc_2_End
    
 phpEspRxProc_2_L5:
    //
    //  Process non-EOP data chunk
    // 
    // check whether the leftover condition is aleady verified
    qbbs    phpEspRxProc_2_L5_1,  espRxPass2Loc1.ctrlFlag.leftoverVerified
    
    // Should we record and remove unaligned bytes due to output stream interface problem
    and     r7.w0,  espRxPass2Loc1.chunkSize,    0x3 
    clr     espPktInst.ctrlFlag.espLeftoverBytes
    qbeq    phpEspRxProc_2_L5_1,  r7.w0,  0 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, phpContext.chunkSize,   espRxPass2Loc1.pktLoc
    sub     CdeCmd.Cmd0.w2, CdeCmd.Cmd0.w2,     2   
    xout    XID_CDECTRL, CdeCmd, 4   
    
    xin     XID_CDEDATA, r6,  2
    mov     espPktInst.leftoverBytes,   r6.w2
    set     espPktInst.ctrlFlag.espLeftoverBytes
     
    // flush out unaligned bytes
    mov     CdeCmd.Cmd0.w0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4
  
phpEspRxProc_2_L5_1:    
          
    //  Save the instance Data
    sbco    espPktInst, cInstBase, phpContext.instOffset, SIZE(espPktInst)
    
 phpEspRxProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop
    
phpEspRxProc_2_Err_Hash:
    // Update the channel statistics
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + SIZE(struct_replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)
    
    add     ctxESPRxStats.numHashFails, ctxESPRxStats.numHashFails, 1
    
    // Update system statistics
    lbco    r5, cStats, PDSP_ESP_AUTH_FAIL_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_ESP_AUTH_FAIL_OFFSET, 4

phpEspRxProc_2_Err_Replay:

phpEspRxProc_2_Err_Common:
    // Update the statistics in the security context  
    xout     XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)

    // 
    // Update the packet descriptor to drop the packet
    // Set Drop bit 
    //

    qbbs    phpEspRxProc_2_Err_Common_L1, espRxPass2Loc1.ctrlFlag.leftoverInserted
    
    // Mov to the packet location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Verify whether we need to insert leftover bytes
    qbbc    phpEspRxProc_2_Err_Common_L1,  espPktInst.ctrlFlag.espLeftoverBytes
    
    // insert the leftover bytes
    add     espRxPass2Loc1.chunkSize,  espRxPass2Loc1.chunkSize, 2
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (2 << 8)
    mov     CdeCmd.Cmd1.w2, espPktInst.leftoverBytes  
    xout    XID_CDECTRL, CdeCmd, 8 
    
phpEspRxProc_2_Err_Common_L1:
    
    set     phpContext.chunkInfo.chunkFlag_discard
    wbs     r31.tStatus_CDEOutPacket
    
phpEspRxProc_2_Err_Common_L2:    
    sbco    phpContext.chunkInfo, cCdeOutPkt, OFFSET(struct_PktDesc.ChunkInfo), SIZE(struct_PktDesc.ChunkInfo)
    
    // Error only occur at the last or second-last data chunk 
    // jmp  phpEspRxProc_2_HashProc_End
    // Record that it is an error packet
    set      espPktInst.ctrlFlag.espPktError
    
    qbbs    phpEspRxProc_2_L4,  phpContext.chunkInfo.chunkFlag_eop    // EOP processing
    jmp     phpEspRxProc_2_L5                                         // non-EOP processing
    

.leave espRxPass2Scope
.leave ctxESPRxScope
.leave commCtxScope    
 
        
