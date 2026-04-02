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
//  * FILE NAME:  saipsec_esp_rx2.p
//  *
//  * DESCRIPTION: Process the IPSEC ESP Rx (From-Network) packets with single chunk
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

.struct struct_espRxPass1Loc1
    .u16    hashDataLen        // Hash Data length (for command label)
    .u16    encryptedDataLen   // Encrypted Data length (for command label)
    .u8     protoOffset        // Protocol Offset
    .u8     espHdrSize         // ESP Header Size (for command label)
    .u8     auxDataSize        // Auxulary Data Size 
    .u8     cmdlOffset         // Command Label Offset 
.ends   

.struct struct_espRxPass1Loc2
    .u32    spi                // Maybe removed
.ends     

.enter espRxPass1Scope
    .assign  struct_espRxPass1Loc1,  R1,  R2, espRxPass1Loc1
    .assign  struct_espRxPass1Loc2,  R3,  R3, espRxPass1Loc2
    .assign  struct_espPktInst, R22, R26.w2,  espPktInst      
.leave espRxPass1Scope

.enter ctxESPRxScope
    .assign  struct_ctxESPRxStats,        R14, R20, ctxESPRxStats
    .assign  struct_ctxESPRxStatsReplay,  R6,  R12, ctxESPRxStatsReplay    
    .assign  struct_ctxESPRx1,            R14, R17.w2, ctxESPRx1
.leave ctxESPRxScope

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

phpEspScRxProc_1:

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
    // need for cmdlOffset
    zero    &espRxPass1Loc1, SIZE(espRxPass1Loc1)    
    
 phpEspScRxProc_1_FirstChunk_L1:    
 
    //pkt Instance Initialization: clear the control flags and other variables
    //zero    &espPktInst,    SIZE(espPktInst)
    
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, ctxESPRx1.firstEngId
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
    
    // TBD: Issue command to flash out and copy PSInfo
    // Now we're at the control section. Insert the command label here
    // Note: we may want to keep the protocol command at the output packet
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    //mov     CdeCmd.Cmd0.w2, 4
    //xout    XID_CDECTRL, CdeCmd, 4      
    
    mov     CdeCmd.Cmd0.b1, ctxESPRx1.cmdlTotalLen
    mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
    xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
    
    // Move here to save up to 6 stalled cycles 
    // Advance to the ESP header
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    //add     CdeCmd.Cmd0.w2, espRxPass1Loc1.protoOffset, payloadInfoL.recordLen
    mov     CdeCmd.Cmd0.w2, espRxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4      
       
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
    
    // Update HashDataLen if ECN is enabled
    qbbc    phpEspScRxProc_1_FirstChunk_L3, ctxESPRx1.ctrlInfo.flagESPRxESN
    add     espRxPass1Loc1.hashDataLen, espRxPass1Loc1.hashDataLen, 4  

    
 phpEspScRxProc_1_FirstChunk_L3:
    // Advance to the ESP header
    // Moved to save stall cycles
    // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    // add     CdeCmd.Cmd0.w2, espRxPass1Loc1.protoOffset, payloadInfoL.recordLen
    // xout    XID_CDECTRL, CdeCmd, 4      
                         
    .leave protoInfoScope
                         
    // Load and processing ESP header
    .using espHdrScope
    xin     XID_CDEDATA, espHdr, SIZE(espHdr)
    
    // TBD: we may want to verify SPI against the one at the security context
    // mov  espHdr.spi, ctxESPRx1.spi
    // mov  espRxPass1Loc2.spi, espHdr.spi
    mov     espPktInst.esnLo, espHdr.seqNum
    
    .leave espHdrScope
    
 phpEspScRxProc_1_FirstChunk_L4:  
 
    // Perform ESN update if configured
    qbbc   phpEspScRxProc_1_FirstChunk_L6, ctxESPRx1.ctrlInfo.flagESPRxReplay
    qbbc   phpEspScRxProc_1_FirstChunk_L6, ctxESPRx1.ctrlInfo.flagESPRxESN
    
    //
    // ESN update
    //
    // Advance the SC Viewer the first part Rx security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPRx.winBaseHi))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRxStatsReplay, SIZE(ctxESPRxStatsReplay)
    
    // Do the subract first so we are immune to 32-bit wrap R0 = seq_num_diff
    sub     r0, espPktInst.esnLo,  ctxESPRxStatsReplay.winBase
    
    // If the delta is "negative", then this packet comes before our base.
    // It is equivalent to (seq_num_diff > 2^31)
    // the receive packet is well outside the replay window, drop it
    qbbs  phpEspScRxProc_1_Err_OldPkt,  r0.t31 
    
    qbgt  phpEspScRxProc_1_FirstChunk_L5,  espPktInst.esnLo,  ctxESPRxStatsReplay.winBase
    mov   espPktInst.esnHi, ctxESPRxStatsReplay.winBaseHi
    jmp   phpEspScRxProc_1_FirstChunk_L6
    
phpEspScRxProc_1_FirstChunk_L5: 
    add   espPktInst.esnHi, ctxESPRxStatsReplay.winBaseHi, 1
    
phpEspScRxProc_1_FirstChunk_L6:

    // Re-load the ESP header
    .using espHdrScope
    xin     XID_CDEDATA, espHdr, SIZE(espHdr)
    .leave espHdrScope
    
    //
    //  Command Label Processing
    //
    lbco    r0.w2,  cEspRxTbl, ctxESPRx1.opMode, 2    // load the processing function from the call table
    call    r0.w2                                     // call table function will return to Error or MainLoop         
    

phpEspScRxProc_1_FirstChunk_L7:
    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, espRxPass1Loc1.cmdlOffset 
    mov     CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
 phpEspScRxProc_1_HashProc_L1:
    qbeq    phpEspScRxProc_1_End, ctxESPRx1.icvSize, 0    
        
    // Hash Data is at this data chunk 
    // Advance to the ICV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, espPktInst.icvOffset, espRxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // Move to save two stalls
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    mov     CdeCmd.Cmd1, espPktInst.esnHi  
    
    // Copy the hash data to the instance buffer
    // may not need to store it 
    mov     r0.b0, ctxESPRx1.icvSize 
    add     r0.w2,  phpContext.instOffset, pktInstICVDataOffset
    xin     XID_CDEDATA, r6, b0
    sbco    r6, cInstBase,  r0.w2, b0 
     
    qbbc    phpEspScRxProc_1_End, ctxESPRx1.ctrlInfo.flagESPRxESN
    // Inset the upper 32-bit ESN to the packet
    // Move to save two stalls
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    //mov     CdeCmd.Cmd1, espPktInst.esnHi  
    xout    XID_CDECTRL, CdeCmd, 8 
         
 phpEspScRxProc_1_End:
    // Save Instance Data
    //sbco    espPktInst, cInstBase, phpContext.instOffset, SIZE(espPktInst)
    sbco    espPktInst, cInstBase, phpContext.instOffset, OFFSET(struct_espPktInst.ctrlFlag)
    
 phpEspScRxProc_1_End_L1:
    
    //close the security context
   mov     CdeCmd.Cmd0, SCV_CMD_DONE
   xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
   // Send this packet (set valid PsInfo to zero)
   mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8) 
   xout    XID_CDECTRL, CdeCmd, 4                  // Send the command    
   jmp     MainLoop
   
.using  pktDescScope
   
 
 phpEspScRxProc_1_Err_Cmdl:
 // TBD:
 
 phpEspScRxProc_1_Err_OldPkt:
    // Updates the statistics
    add     ctxESPRxStatsReplay.numOldPkts, ctxESPRxStatsReplay.numOldPkts, 1
    xout    XID_SCVDATA, ctxESPRxStatsReplay, SIZE(ctxESPRxStatsReplay)
    // Update System Statistics
    lbco    r5, cStats, PDSP_ESP_REPLAY_OLD_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_ESP_REPLAY_OLD_OFFSET, 4

    
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPRx.ctrlInfo)) << 8)

    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRx1, SIZE(ctxESPRx1)
    

 phpEspScRxProc_1_Err_Common:
    // 
    // Update the packet descriptor to drop the packet
    // EngId = Output1
    // ThreadId = 7
    // Command label removed (may not be necessary since packet is dropped 
    // Set Drop bit 
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket
    
    lbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SwInfo1_ctxPtr_w0)
    mov     PktDesc.SA_NextEngId,   SAENG_OUT1_1
    mov     PktDesc.SA_CmdLblInfo,  ctxESPRx1.cmdlTotalLen
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    set     PktDesc.ChunkInfo.chunkFlag_discard
    sbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SrcID)
    
    // Free the instance buffer
    clr     phpContext.instMask, PktDesc.PktId
    
    jmp    phpEspScRxProc_1_End_L1
    
    
.leave  pktDescScope

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
.struct struct_espRxPass2Loc1
    .u8     ctrlFlag            // record the protocol payload Size
#define leftoverInserted        t0
#define leftoverVerified        t1    
#define espRxESN                t2
    .u8     pktId   
    .u16    psCtrlSize          // PS Info + Control Info Size
    .u16    chunkSize           // adjusted chunk size due to byte insertion
    .u16    pktLoc              // current location from the packet beginning
.ends     

.enter espRxPass2Scope
    .assign  struct_espRxPass2Loc1,  R1,  R2,     espRxPass2Loc1
    .assign  struct_espPktInst,      R22, R26.w2, espPktInst      
.leave espRxPass2Scope

phpEspScRxProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using espRxPass2Scope
    .using ctxESPRxScope
    
    // clear Local Variables (may be just some of them)
    zero    &espRxPass2Loc1,    SIZE(espRxPass2Loc1)    
    
    //qbbc    phpEspScRxProc_2_L1,   phpContext.chunkInfo.chunkFlag_eop  
    
phpEspScRxProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPRx.ctrlInfo))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxESPRx1, SIZE(ctxESPRx1)

    // Record the packet instance index and load the instance
    mov     espRxPass2Loc1.pktId,  PktDesc.PktId
    lbco    espPktInst, cInstBase,  phpContext.instOffset, OFFSET(struct_espPktInst.ctrlFlag)
    
 phpEspScRxProc_2_L2:   
    mov     PktDesc.SA_NextEngId, SAENG_OUT1_1
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2
    add     espRxPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize  
    
    // Update swInfo1 at SOP
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
    .leave pktDescScope
    
 //
 // Hash Processing
 //   
 qbeq    phpEspScRxProc_2_HashProc_End, ctxESPRx1.icvSize, 0
 
 phpEspScRxProc_2_HashProc_L1:
    
    //
    // Hash data is available in the PS Info section
    //
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and compare the Hash Data
    mov     r0.b0, ctxESPRx1.icvSize
    xin     XID_CDEDATA,  r6,  b0
    
    // Advance the window to the icv location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, espPktInst.icvOffset, espRxPass2Loc1.psCtrlSize
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // Move here to save 5 stalls
    add     r0.w2, phpContext.instOffset, pktInstICVDataOffset
    lbco    r10, cInstBase, r0.w2, b0 
    
    qbeq    phpEspScRxProc_2_Replay_L1, ctxESPRx1.icvSize, 0    
    qbbc    phpEspScRxProc_2_HashProc_L3,  ctxESPRx1.ctrlInfo.flagESPRxESN
    // flush out ESN
    set     espRxPass2Loc1.ctrlFlag.espRxESN
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 4
    xout    XID_CDECTRL, CdeCmd, 4
      
 phpEspScRxProc_2_HashProc_L3:
    //
    //  ESP Hash Comparing
    //
 //IPSEC_ICVCheck:
    ipsec_icv_check phpEspScRxProc_2_Err_Hash, phpEspScRxProc_2_Replay_L1, ctxESPRx1.icvSize    
      
    //
    //  Replay check
    //
phpEspScRxProc_2_Replay_L1:
    
    qbbc    phpEspScRxProc_2_HashProc_End, ctxESPRx1.ctrlInfo.flagESPRxReplay
    // Advance the SC Viewer the replay Ctrl
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    .using replayCtrlScope
    //xin     XID_SCVDATA, replayCtrl, SIZE(replayCtrl) ??
    xin     XID_SCVDATA, replayCtrl, 32
    mov     r29, espPktInst.esnLo   
    call    Replay_CheckAndUpdate
    
    // Verify the replay check result
    qbeq    phpEspScRxProc_2_ReplayPass, r29.b0, SA_REPLAY_RC_OK
    qbeq    phpEspScRxProc_2_ReplayPass, r29.b0, SA_REPLAY_RC_NEW
    
    // Error Handling 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)
    
    qbeq    phpEspScRxProc_2_ReplayDup, r29.b0, SA_REPLAY_RC_DUP
    
 phpEspScRxProc_2_ReplayOld:
    add     ctxESPRxStats.numOldPkts, ctxESPRxStats.numOldPkts, 1
    // Update System Statistics
    lbco    r5, cStats, PDSP_ESP_REPLAY_OLD_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_ESP_REPLAY_OLD_OFFSET, 4
    jmp     phpEspScRxProc_2_Err_Replay
 phpEspScRxProc_2_ReplayDup:
    add     ctxESPRxStats.numDupPkts, ctxESPRxStats.numDupPkts, 1
    // Update System Statistics
    lbco    r5, cStats, PDSP_ESP_REPLAY_DUP_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_ESP_REPLAY_DUP_OFFSET, 4
    jmp     phpEspScRxProc_2_Err_Replay
     
 phpEspScRxProc_2_ReplayPass:
    qbbc    phpEspScRxProc_2_ReplayPass_L2, espRxPass2Loc1.ctrlFlag.espRxESN
    qbgt    phpEspScRxProc_2_ReplayPass_L2, replayCtrl.winBase, espPktInst.esnLo
    mov     replayCtrl.winBaseHi, espPktInst.esnHi
    
 phpEspScRxProc_2_ReplayPass_L2:  
    xout    XID_SCVDATA, replayCtrl, SIZE(replayCtrl)
    
 phpEspScRxProc_2_HashProc_End:
    //
    //  Process EOP Data Chunk
    //
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(replayCtrl)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)
    
    .leave replayCtrlScope
    
    add     ctxESPRxStats.numRxPkts,   ctxESPRxStats.numRxPkts, 1
    adc     ctxESPRxStats.numRxPktsHi, ctxESPRxStats.numRxPktsHi, 0
    clr     phpContext.instMask, espRxPass2Loc1.pktId
    xout    XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_ESP_DEC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_ESP_DEC_HI_OFFSET, 8
    
 phpEspScRxProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop
    
    
phpEspScRxProc_2_Err_Hash:
    // Update the channel statistics
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + SIZE(struct_replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)
    
    add     ctxESPRxStats.numHashFails, ctxESPRxStats.numHashFails, 1
    
    // Update system statistics
    lbco    r5, cStats, PDSP_ESP_AUTH_FAIL_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_ESP_AUTH_FAIL_OFFSET, 4
    
    //jmp  phpEspScRxProc_2_Err_Common
    
phpEspScRxProc_2_Err_Replay:
    
phpEspScRxProc_2_Err_Common:

    // Update the statistics in the security context  
    xout     XID_SCVDATA, ctxESPRxStats, SIZE(ctxESPRxStats)

    // 
    // Update the packet descriptor to drop the packet
    // Set Drop bit 
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket
    
    set     phpContext.chunkInfo.chunkFlag_discard
    sbco    phpContext.chunkInfo, cCdeOutPkt, OFFSET(struct_PktDesc.ChunkInfo), SIZE(struct_PktDesc.ChunkInfo)
    
    // Free the instance buffer
    clr     phpContext.instMask, espRxPass2Loc1.pktId    
    
    jmp  phpEspScRxProc_2_End

.leave espRxPass2Scope
.leave ctxESPRxScope
.leave commCtxScope    
 
// Register MAP
// R0, R29       : General and local register
// R6-R8         : rtp header (from packet)
// R14-R21       : SRTP Security Context including all parametes required
// R10-R13       : output iv 
// R30           : return address

//.struct struct_espAesCtr
//    .u32  salt
//    .u32  iv1
//    .u32  iv2
//    .u32  cnt
//.ends

//.struct struct_espAad
//    .u32  spi
//    .u32  data1
//    .u32  data2
//    .u32  data3
//.ends


.enter espRxCmdlScope
    .assign  struct_saCmdL,             R12, R13, saCmdL
    .assign  struct_saCmdLReg,          R12, R13, saCmdLReg
.leave espRxCmdlScope 

//.enter espAesCtrScope
//    .assign  struct_espAesCtr,          R7, R10, saEspAesCtr 
//.leave espAesCtrScope

//.enter espAadScope
//    .assign  struct_espAad,             R10, R13, saEspAad 
//.leave espAadScope
        
 .using espRxPass1Scope
 .using ctxESPRxScope
 .using espHdrScope
 .using espRxCmdlScope
 .using espAesCtrScope
 
phpEspRxCmdlProc_Gen:

    // Construct command Label in ESP General mode
    zero    &saCmdL, SIZE(saCmdL)
    
    // Is authentication required
    qbeq    phpEspRxCmdlProc_Gen_L4, ctxESPRx1.icvSize, 0
    
    // Format the authentication command label
    qbbc    phpEspRxCmdlProc_Gen_L1_1, ctxESPRx1.ctrlInfo.flagESPRxNullEnc 
    mov     saCmdL.nextEngId, SAENG_PHP_2
    jmp     phpEspRxCmdlProc_Gen_L2
    
phpEspRxCmdlProc_Gen_L1_1:    
    qbbc    phpEspRxCmdlProc_Gen_L1_2, ctxESPRx1.ctrlInfo.flagESPRxCMAC
      mov     saCmdL.nextEngId, SAENG_ENCRYPT_2
      jmp     phpEspRxCmdlProc_Gen_L2
    
phpEspRxCmdlProc_Gen_L1_2:    
    mov     saCmdL.nextEngId, SAENG_ENCRYPT_1

phpEspRxCmdlProc_Gen_L2:
    mov     saCmdL.procLen, espRxPass1Loc1.hashDataLen
    mov     saCmdL.sopOffset, espRxPass1Loc1.protoOffset
    
    // Process CMAC 
    qbbc    phpEspRxCmdlProc_Gen_L3, ctxESPRx1.ctrlInfo.flagESPRxCMAC
    mov     saCmdL.cmdlLen, SA_CMAC_CMDL_SIZE
    //mov     saCmdL.optctrl1, SA_CMDL_MK_OPT_CTRL(SA_ENC_AUX1_OFFSET, 16)
    mov     saCmdL.optctrl1, SA_ENC_AUX1_OFFSET + (16 >> 3)
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // 
    // Record the option offset, we need to copy Key1 and Key2 at the end of this function
    //
    mov     r0.b1, espRxPass1Loc1.cmdlOffset
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset,  16
    jmp     phpEspRxCmdlProc_Gen_L4

phpEspRxCmdlProc_Gen_L3:    
    mov     saCmdL.cmdlLen, SIZE(saCmdL)
    mov     saCmdL.optctrl1, 0
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
phpEspRxCmdlProc_Gen_L4:
    qbbs    phpEspRxCmdlProc_Gen_L6, ctxESPRx1.ctrlInfo.flagESPRxNullEnc 
    
    // Construct the encryption command label 
    // Does not need to zero out saCmdl since opt2/opt3 are not used at the authentication label
    // zero    &saCmdL, SIZE(saCmdL)
    mov     saCmdL.nextEngId, SAENG_PHP_2
    mov     saCmdL.procLen, espRxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset, espRxPass1Loc1.protoOffset, espRxPass1Loc1.espHdrSize
    
    // Special Processing is required for AES CTR mode
    qbbc    phpEspRxCmdlProc_Gen_L5, ctxESPRx1.ctrlInfo.flagESPRxAESCTR
    
    // AES CTR operation
    mov     saCmdL.cmdlLen,  SA_AESCTR_CMDL_SIZE
    //mov     saCmdL.optctrl1, SA_CMDL_MK_OPT_CTRL(SA_ENC_AUX2_OFFSET, 16)
    mov     saCmdL.optctrl1, SA_ENC_AUX2_OFFSET + (16 >> 3)
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    mov     saEspAesCtr.salt,  ctxESPRx1.salt
    mov     saEspAesCtr.cnt,   1  
    sbco    saEspAesCtr, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    jmp     phpEspRxCmdlProc_Gen_L6   
    
phpEspRxCmdlProc_Gen_L5:  
    add     saCmdL.cmdlLen,  ctxESPRx1.ivSize, SIZE(saCmdL)
    lsr     r0.b0, ctxESPRx1.ivSize, 3 
    add     saCmdL.optctrl1, r0.b0, SA_ENC_AUX2_OFFSET 
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    mov     r0.b0, ctxESPRx1.ivSize
    sbco    espHdr.iv1, cCmdlBuf, espRxPass1Loc1.cmdlOffset, b0
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, r0.b0

phpEspRxCmdlProc_Gen_L6:
    qbbc    phpEspRxCmdlProc_Gen_End, ctxESPRx1.ctrlInfo.flagESPRxCMAC
    
    // Copy K1/K2 for CMAC operation
    // Move the security context viewer to the K1/K2 location 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxESPRx.aux1)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4   
    xin     XID_SCVDATA, r6,  32
    
    and     r0.w2, espRxPass1Loc1.hashDataLen, 0x000f
    qbne    phpEspRxCmdlProc_Gen_L7, r0.w2, 0
    // K1 is used
    sbco    r6, cCmdlBuf, r0.b1, 16
    ret
    
phpEspRxCmdlProc_Gen_L7:
    // k2 is used
    sbco    r10, cCmdlBuf, r0.b1, 16
    
phpEspRxCmdlProc_Gen_End:
    ret       
    
phpEspRxCmdlProc_GCM:

    // Construct command Label in ESP GCM Mode
    // Construct and copy basic label
    mov     saCmdLReg.reg1.w2,  SA_GCM_CMDL_SIZE + (SAENG_PHP_2 << 8)
    mov     saCmdL.procLen, espRxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset, espRxPass1Loc1.protoOffset, espRxPass1Loc1.espHdrSize
    mov     saCmdL.optctrl1, SA_GCM_CMDL_OPT1
    mov     saCmdLReg.reg2.w0,  SA_GCM_CMDL_OPT3 + (SA_GCM_CMDL_OPT2 << 8)
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construst and copy Aux1
    zero    &saCmdL, SIZE(saCmdL)
    lsl     saCmdLReg.reg2, espRxPass1Loc1.encryptedDataLen, 3
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
    // Construct and Copy AAD
    .using espAadScope
    zero    &saEspAad, SIZE(saEspAad)
    mov     saEspAad.spi, espHdr.spi 

    qbbs    phpEspRxCmdlProc_GCM_L1, ctxESPRx1.ctrlInfo.flagESPRxESN
    mov     saEspAad.data1, espPktInst.esnLo
    jmp     phpEspRxCmdlProc_GCM_L2
    
phpEspRxCmdlProc_GCM_L1:
    mov     saEspAad.data1, espPktInst.esnHi
    mov     saEspAad.data2, espPktInst.esnLo

phpEspRxCmdlProc_GCM_L2: 
    sbco    saEspAad, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saEspAad)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saEspAad)
   
    .leave espAadScope
    
    // Construct and copy AES CTR 
    mov     saEspAesCtr.salt,  ctxESPRx1.salt
    mov32   saEspAesCtr.cnt,   1  
    sbco    saEspAesCtr, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    ret
    
phpEspRxCmdlProc_GMAC:

    // Construct command Label in ESP GMAC Mode
    // Construct and copy basic label
    mov     saCmdLReg.reg1.w2,  SA_GMAC_CMDL_SIZE + (SAENG_PHP_2 << 8)
    mov     saCmdL.procLen, espRxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset, espRxPass1Loc1.protoOffset, espRxPass1Loc1.espHdrSize
    mov     saCmdL.optctrl1, SA_GMAC_CMDL_OPT1
    mov     saCmdLReg.reg2.w0,  SA_GMAC_CMDL_OPT3 + (SA_GMAC_CMDL_OPT2 << 8)
    
    qbbs    phpEspRxCmdlProc_GMAC_L1, ctxESPRx1.ctrlInfo.flagESPRxESN
    // Non-ESN operation
    // The 8-byte IV in front of ESP payload is copied to the AAD field.
    // The total authentication length should include 8-byte IV
    mov     saCmdL.procLen, espRxPass1Loc1.encryptedDataLen
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construst and copy Aux1
    zero    &saCmdL, SIZE(saCmdL)
    add     saCmdLReg.reg2.w0, espRxPass1Loc1.encryptedDataLen, (SA_GMAC_ESP_AAD_LEN1 + 8)
    lsl     saCmdLReg.reg2, saCmdLReg.reg2, 3
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
    // Construct and Copy AAD
    .using espAadScope
    // Copy paylaod data before they are overwritten by SPI and SN
    mov     saEspAad.data2, espHdr.iv1
    mov     saEspAad.data3, espHdr.iv2
    mov     saEspAad.spi, espHdr.spi 
    mov     saEspAad.data1, espPktInst.esnLo
    jmp     phpEspRxCmdlProc_GMAC_L2
    
phpEspRxCmdlProc_GMAC_L1:
    // ESN operation
    add     saCmdL.procLen, espRxPass1Loc1.encryptedDataLen, 4
    sub     saCmdL.sopOffset, saCmdL.sopOffset, 4
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construst and copy Aux1
    zero    &saCmdL, SIZE(saCmdL)
    add     saCmdLReg.reg2.w0, espRxPass1Loc1.encryptedDataLen, (SA_GMAC_ESP_AAD_LEN2 + 8)
    lsl     saCmdLReg.reg2, saCmdLReg.reg2, 3
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
    // Construct and Copy AAD
    // Copy paylaod data before they are overwritten by SPI and SN
    mov     saEspAad.data3, espHdr.iv1
    mov     saEspAad.spi, espHdr.spi 
    mov     saEspAad.data1, espPktInst.esnHi
    mov     saEspAad.data2, espPktInst.esnLo

phpEspRxCmdlProc_GMAC_L2: 
    sbco    saEspAad, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saEspAad)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saEspAad)
   
    .leave espAadScope
    
    // Construct and copy AES CTR 
    mov     saEspAesCtr.salt,  ctxESPRx1.salt
    mov32   saEspAesCtr.cnt,   1  
    sbco    saEspAesCtr, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    ret
    
phpEspRxCmdlProc_CCM:

    // Construct command Label in ESP CCM Mode
    // Construct and copy basic label
    mov     saCmdLReg.reg1.w2,  SA_CCM_CMDL_SIZE + (SAENG_PHP_2 << 8)
    mov     saCmdL.procLen, espRxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset, espRxPass1Loc1.protoOffset, espRxPass1Loc1.espHdrSize
    mov     saCmdL.optctrl1, SA_CCM_CMDL_OPT1
    mov     saCmdLReg.reg2.w0,  SA_CCM_CMDL_OPT3 + (SA_CCM_CMDL_OPT2 << 8)
    sbco    saCmdL, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construct and Copy B0
    // R6-R7 (SPI, SN), R5 command data
    // clean up B0, lower 8-byte
    zero    &saCmdL, SIZE(saCmdL)
    add     r0.b0, espRxPass1Loc1.cmdlOffset, 8
    sbco    saCmdL, cCmdlBuf, r0.b0, 8
    
    qbbs    phpEspRxCmdlProc_CCM_L1, ctxESPRx1.ctrlInfo.flagESPRxESN
    
    // Non-ESN operation (00|08|SPI|SN|000000000000)
    mov     r5.w0, SA_CCM_ESP_AAD_LEN1
    sbco    r5.w0, cCmdlBuf, espRxPass1Loc1.cmdlOffset, 10
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, 16
    jmp     phpEspRxCmdlProc_CCM_L2
    
phpEspRxCmdlProc_CCM_L1:
    // ESN operation (00|0c|SPI|ESN|0000)
    mov     r5.w0, SA_CCM_ESP_AAD_LEN2
    sbco    r5.w0, cCmdlBuf, espRxPass1Loc1.cmdlOffset, 6
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, 6
    mov     espHdr.spi, espPktInst.esnHi
    sbco    espHdr, cCmdlBuf, espRxPass1Loc1.cmdlOffset, 8
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, 10

phpEspRxCmdlProc_CCM_L2: 
    // Construct and Copy B0
    mov     saEspAesCtr.salt,  ctxESPRx1.salt
    mov     saEspAesCtr.cnt,   espRxPass1Loc1.encryptedDataLen
    sbco    saEspAesCtr, cCmdlBuf,  espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    
    // Construct and copy AES CTR 
    and     saEspAesCtr.salt.b3,  saEspAesCtr.salt.b3, 0x07
    zero    &saEspAesCtr.cnt,   4  
    sbco    saEspAesCtr, cCmdlBuf, espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espRxPass1Loc1.cmdlOffset, espRxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    
    ret
    
 .leave espRxPass1Scope
 .leave ctxESPRxScope
 .leave espHdrScope
 .leave espRxCmdlScope
 .leave espAesCtrScope
        
