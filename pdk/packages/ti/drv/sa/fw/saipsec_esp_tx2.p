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

.struct struct_espTxPass1Loc1
    .u16    hashDataLen        // Hash Data length (for command label)
    .u16    encryptedDataLen   // Encrypted Data length (for command label)
    .u8     protoOffset        // Protocol Offset
    .u8     espHdrSize         // ESP Header Size (for command label)
    .u8     cmdlOffset         // Command Label Offset 
    .u8     pktOffset          // TBD: IV offset in the command label (for copy data)
.ends     

.enter espTxPass1Scope
    .assign  struct_espTxPass1Loc1,  R2,  R3,   espTxPass1Loc1
    .assign  struct_espTxPktInst,    R22, R24,  espTxPktInst      
.leave espTxPass1Scope

.enter ctxESPTx1Scope
    .assign  struct_ctxESPTx1,  R14,  R19.w2, ctxESPTx1
.leave ctxESPTx1Scope

.enter ctxESPTxScopeStats
    .assign  struct_ctxESPTxStats,  R14,  R18, ctxESPTxStats
.leave ctxESPTxScopeStats

.enter espHdrScope
    .assign  struct_espHdr,           R6, R11,  espHdr
    .assign  struct_saRnFifoCb,       r13, R13, rnFifoPopCb
.leave espHdrScope   

//.enter protoInfoScope
//    .assign  struct_protoInfo,      R6, R6,   protoInfo
//.leave protoInfoScope

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

phpEspScTxProc_1:

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
    
 phpEspScTxProc_1_FirstChunk_L1:    
 
    //clear the control flags and other variables
    //zero    &espTxPktInst,    SIZE(espTxPktInst)
    qbbs    phpEspScTxProc_1_noESNOverFlow, ctxESPTx1.ctrlInfo.flagESPTxESN
    fill    &r0, 4
    qbne    phpEspScTxProc_1_noESNOverFlow, r0, ctxESPTx1.esnLo
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4

    .using ctxESPTxScopeStats
    xin     XID_SCVDATA, ctxESPTxStats, SIZE(ctxESPTxStats)
    add     ctxESPTxStats.rollOverCounter, ctxESPTxStats.rollOverCounter, 1
    xout    XID_SCVDATA, ctxESPTxStats, SIZE(ctxESPTxStats)
    .leave ctxESPTxScopeStats
    // Drop packet
    jmp     phpEspScTxProc_1_Err_Esn_Overflow

phpEspScTxProc_1_noESNOverFlow:

    //Increment the ESN 
    add     ctxESPTx1.esnLo,  ctxESPTx1.esnLo,  1
    adc     ctxESPTx1.esnHi,  ctxESPTx1.esnHi,  0
    
    // Save the updated security context
    xout    XID_SCVDATA,  ctxESPTx1,  SIZE(ctxESPTx1)
    
    //Record the control size - the short payload Info
    sub     espTxPass1Loc1.pktOffset,  PktDesc.ControlSize, 8
    
 phpEspScTxProc_1_FirstChunk_L2:  
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, ctxESPTx1.firstEngId
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    //xout    XID_CDEDATA, PktDesc, SIZE(PktDesc)
    xout    XID_CDEDATA, PktDesc, 8   
      
    // Advance the window past the Descriptor and PS Info
    // Move is desc size + phys PsInfo size
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    //add     CdeCmd.Cmd0.w2, PktDesc.PsPhysSize, SIZE(PktDesc) 
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_CONTROL
    xout    XID_CDECTRL, CdeCmd, 4    
    
    .leave  pktDescScope
    .using protoInfoScope
    // Extract protocol Info
    //xin     XID_CDEDATA, payloadInfoS, SIZE(payloadInfoS)
    //mov     espTxPass1Loc1.protoOffset,  payloadInfoS.offset
    // We need to handle both long and short info
    // Short Info: From Host
    // Long Info: From PA (Eflow operation)
    // Note: Both payloadInfoL and protoInfo share R6
    xin     XID_CDEDATA, payloadInfoL, OFFSET(struct_payloadInfoLong.reserved3)
    // extract the command code b7-b5 (0,1) = (long, short info)
    lsr     r0.b0, payloadInfoL.recordLen, 5
    qbeq    phpEspScTxProc_ShortInfo_L1, r0.b0, 1
    
phpEspScTxProc_LongInfo_L1:
    mov     espTxPass1Loc1.protoOffset,  payloadInfoL.ipsecOffset
    sub     payloadInfoS.len, payloadInfoL.eopOffset, payloadInfoL.ipsecOffset  
    // re-adjust pktOffset
    add     espTxPass1Loc1.pktOffset,  espTxPass1Loc1.pktOffset, 8
    jmp     phpEspScTxProc_protoInfo_End
    
phpEspScTxProc_ShortInfo_L1:    
    mov     espTxPass1Loc1.protoOffset,  payloadInfoS.offset
    // Issue command to flash out and copy PSInfo
    // Now we're at the control section. Insert the command label here
    // Advance the window past the Descriptor
    // TBD: Move is desc size + phys PsInfo size
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4  
    
phpEspScTxProc_protoInfo_End:

    mov     CdeCmd.Cmd0.b1, ctxESPTx1.cmdlTotalLen
    mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
    xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
    
    // Advance to the ESP header
    // Move to save up to 7 stall cycles
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, espTxPass1Loc1.protoOffset, espTxPass1Loc1.pktOffset
    xout    XID_CDECTRL, CdeCmd, 4      
       
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

    // Restore ctxESPTx1
    xin     XID_SCVDATA, ctxESPTx1, SIZE(ctxESPTx1)


    // Calculate and record Various Offsets
    add     espTxPktInst.icvOffset, espTxPass1Loc1.protoOffset,   espTxPass1Loc1.hashDataLen
    //add     espTxPktInst.encryptedOffset, espTxPass1Loc1.protoOffset, espTxPass1Loc1.espHdrSize
    
    // Update HashDataLen if ECN is enabled
    qbbc    phpEspScTxProc_1_FirstChunk_L3, ctxESPTx1.ctrlInfo.flagESPTxESN
    add     espTxPass1Loc1.hashDataLen, espTxPass1Loc1.hashDataLen, 4  
   
    .leave protoInfoScope
    
 phpEspScTxProc_1_FirstChunk_L3:
    // Advance to the ESP header
    // Move to save some stall cycles
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    //add     CdeCmd.Cmd0.w2, espTxPass1Loc1.protoOffset, espTxPass1Loc1.pktOffset
    //xout    XID_CDECTRL, CdeCmd, 4      
                         
    // Load and processing ESP header
    .using espHdrScope
    xin     XID_CDEDATA, espHdr, SIZE(espHdr)
    mov     espHdr.spi, ctxESPTx1.spi
    mov     espHdr.seqNum,  ctxESPTx1.esnLo
    
    // 
    // Format IV if necessary
    //
    qbeq    phpEspScTxProc_1_FormatIV_End, ctxESPTx1.ivSize, 0
    qbbs    phpEspScTxProc_1_FormatIV_Random, ctxESPTx1.ctrlInfo.flagESPTxRandomIV
    
phpEspScTxProc_1_FormatIV_Unique:     
    // IV should be unique: Use ESN
    mov     espHdr.iv1, ctxESPTx1.esnHi
    mov     espHdr.iv2, ctxESPTx1.esnLo
    qbeq    phpEspScTxProc_1_FormatIV_End,  ctxESPTx1.ivSize, 8
    mov     espHdr.iv3, ctxESPTx1.esnHi
    mov     espHdr.iv4, ctxESPTx1.esnLo
    jmp     phpEspScTxProc_1_FormatIV_End
    
phpEspScTxProc_1_FormatIV_Random:     
    // IV should be random: format the IV from the random number FIFO
    lbco    rnFifoPopCb, cRnFifoCtrl, 0, SIZE(rnFifoPopCb)
    
    // Compare (512 - out) with ivSize 
    mov     r0.w0, phpRnFifoSize
    sub     r0.w2, r0.w0, rnFifoPopCb.out
    //set     r1.w0, rnFifoPopCb.out, 10 //phpRnFifoBase = 0x400(b10)
    qblt    phpEspScTxProc_1_FormatIV_L2, ctxESPTx1.ivSize, r0.w2
 
 phpEspScTxProc_1_FormatIV_L1:
    // Case 1: (512 - out) >= IV Size
    mov     r0.b2, ctxESPTx1.ivSize
    lbco    espHdr.iv1, cRnFifoBuf, rnFifoPopCb.out, b2
    add     rnFifoPopCb.out, rnFifoPopCb.out, ctxESPTx1.ivSize
    qbne    phpEspScTxProc_1_FormatIV_L3, rnFifoPopCb.out, r0.w0
        mov     rnFifoPopCb.out, 0
        jmp     phpEspScTxProc_1_FormatIV_L3 
    
 phpEspScTxProc_1_FormatIV_L2: 
    // Case 2: (512 - out) < IV Size
    // IV size = 16 and (512 - out) = 8      
    lbco    espHdr.iv1, cRnFifoBuf, rnFifoPopCb.out, 8
    //mov     r1.w0,  phpRnFifoBase 
    lbco    espHdr.iv3, cRnFifoBuf, 0, 8
    mov     rnFifoPopCb.out, 8
 
 phpEspScTxProc_1_FormatIV_L3: 
    sbco    rnFifoPopCb.out, cRnFifoCtrl, OFFSET(struct_saRnFifoCb.out), SIZE(rnFifoPopCb.out)
    
 phpEspScTxProc_1_FormatIV_End:
    xout    XID_CDEDATA, espHdr, SIZE(espHdr)
    
 phpEspScTxProc_1_FirstChunk_L4:  
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
    
 phpEspScTxProc_1_HashProc_L1:
    qbeq    phpEspScTxProc_1_End, ctxESPTx1.icvSize, 0   
    qbbc    phpEspScTxProc_1_End, ctxESPTx1.ctrlInfo.flagESPTxESN
        
 phpEspScTxProc_1_HashProc_L2:
    // Hash Data is at this data chunk 
    // Advance to the ICV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, espTxPktInst.icvOffset, espTxPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // Inset the upper 32-bit ESN to the packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    mov     CdeCmd.Cmd1, ctxESPTx1.esnHi  
    xout    XID_CDECTRL, CdeCmd, 8 
         
 phpEspScTxProc_1_End:
    // Save Instance Data
    sbco    espTxPktInst, cInstBase, phpContext.instOffset, OFFSET(struct_espTxPktInst.icvOffset_pass2)
    
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
 phpEspScTxProc_1_CmdlErr:
 // TBD
     
    jmp     MainLoop

// Entry point for processing ESN overflow drop packet operations
phpEspScTxProc_1_Err_Esn_Overflow:
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

.struct struct_espTxPass2Loc1
    .u8     ctrlFlag            // record the protocol payload Size
#define eop      t0  
    .u8     pktId   
    .u16    psCtrlSize          // PS Info + Control Info Size
    .u16    icvOffset           // icvOffset from the current position
    .u16    chunkSize           // adjusted chunk size due to byte insertion
    .u16    pktLoc              // current location from the packet beginning
.ends     

.enter espTxPass2Scope
    .assign  struct_espTxPass2Loc1,  R1,  R3.w2,  espTxPass2Loc1
    .assign  struct_espTxPktInst,    R22, R24, espTxPktInst      
    //.assign  struct_saRnFifoPushCb,  R25, R25, rnFifoPushCb
.leave espTxPass2Scope

phpEspScTxProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using espTxPass2Scope
    
    // clear Local Variables (may be just some of them)
    // zero    &espTxPass2Loc1,    SIZE(espTxPass2Loc1)    
    
    //qbbc    phpEspScTxProc_2_L1,   phpContext.chunkInfo.chunkFlag_eop  
    
phpEspScTxProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxESPTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    .using  ctxESPTx1Scope
    xin     XID_SCVDATA, ctxESPTx1, SIZE(ctxESPTx1)

    // Record packet instance index and load the instance
    mov     espTxPass2Loc1.pktId,  PktDesc.PktId
    lbco    espTxPktInst, cInstBase,  phpContext.instOffset, OFFSET(struct_espTxPktInst.icvOffset_pass2)
    
 phpEspScTxProc_2_L2:    
    mov     PktDesc.SA_NextEngId, SAENG_OUT1_1
    //xout    XID_CDEDATA, PktDesc, SIZE(PktDesc)
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2
    add     espTxPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize    

    // Update swInfo1 at SOP
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
    
 //
 // Hash Processing
 //   
 phpEspScTxProc_2_HashProc_L1:
 
    .leave pktDescScope
    
    //
    // Hash data is available in the PS Info section
    //
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and store the Hash Data
    //mov     r0.b0, ctxESPTx1.icvSize (always load 16 bytes)
    xin     XID_CDEDATA,  r6,  16
    
    // Advance the window to the icv location
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    //xout    XID_CDECTRL, CdeCmd, 4
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, espTxPktInst.icvOffset, espTxPass2Loc1.psCtrlSize
    
#ifdef IPSEC_RN_IV_USE_ENCRYPTED_DATA    
    // Insert EncryptedData of size = ctxESPTx1.ivSize to the RN FIFO if random IV
    qbeq    phpEspScTxProc_2_HashProc_L2, ctxESPTx1.ivSize, 0
    qbbc    phpEspScTxProc_2_HashProc_L2, ctxESPTx1.ctrlInfo.flagESPTxRandomIV
    
    // Move to the encryption data area 
    // Move to the encryption data offset - 16 so that we can use registers r10 because r6-r9
    // contain the hash data
    sub     espTxPktInst.encryptedOffset, espTxPktInst.encryptedOffset, 16
    add     CdeCmd.Cmd0.w2, espTxPktInst.encryptedOffset, espTxPass2Loc1.psCtrlSize
    xout    XID_CDECTRL, CdeCmd, 4   
    
    lbco    rnFifoPushCb, cRnFifoCtrl, OFFSET(struct_saRnFifoCb.in), SIZE(rnFifoPushCb)
    add     rnFifoPushCb.cnt, rnFifoPushCb.cnt, ctxESPTx1.ivSize
    
    // Move here to use the stall cycles
    mov     r0.w0, phpRnFifoSize
    sub     r0.w2, r0.w0, rnFifoPushCb.in
    
    xin     XID_CDEDATA, r10, 16
    
    // Compare (512 - in) with ivSize   
    // move to use the stall cycles
    // mov     r0.w0, phpRnFifoSize
    // sub     r0.w2, r0.w0, rnFifoPushCb.in
    qblt    phpEspScTxProc_2_InsertRN_L2, ctxESPTx1.ivSize, r0.w2
 
 phpEspScTxProc_2_InsertRN_L1:
    // Case 1: (512 - out) >= IV Size
    mov     r0.b2, ctxESPTx1.ivSize
    sbco    r10, cRnFifoBuf, rnFifoPushCb.in, b2
    add     rnFifoPushCb.in, rnFifoPushCb.in, ctxESPTx1.ivSize
    qbne    phpEspScTxProc_2_InsertRN_L3, rnFifoPushCb.in, r0.w0
    mov     rnFifoPushCb.in, 0
    jmp     phpEspScTxProc_2_InsertRN_L3 
    
 phpEspScTxProc_2_InsertRN_L2: 
    // Case 2: (512 - in) < IV Size
    // IV size = 16 and (512 - in) = 8      
    sbco    r10, cRnFifoBuf, rnFifoPushCb.in, 8
    sbco    r12, cRnFifoBuf, 0, 8
    mov     rnFifoPushCb.in, 8
 
 phpEspScTxProc_2_InsertRN_L3: 
    sbco    rnFifoPushCb, cRnFifoCtrl, OFFSET(struct_saRnFifoCb.in), SIZE(rnFifoPushCb)
    sub     CdeCmd.Cmd0.w2, espTxPktInst.icvOffset, espTxPktInst.encryptedOffset
    
#endif    
    
phpEspScTxProc_2_HashProc_L2: 
    // Advance the window to the icv location
    xout    XID_CDECTRL, CdeCmd, 4   
    
    qbeq    phpEspScTxProc_2_HashProc_L3, ctxESPTx1.icvSize, 0   
    qbbc    phpEspScTxProc_2_HashProc_L3, ctxESPTx1.ctrlInfo.flagESPTxESN
    // flush out ESN
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 4
    xout    XID_CDECTRL, CdeCmd, 4
      
 phpEspScTxProc_2_HashProc_L3:
    // Move here to save three stalls
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4

    // Update byteCount stats
    .using ctxESPTxScopeStats
    xin     XID_SCVDATA, ctxESPTxStats, SIZE(ctxESPTxStats)
    add     ctxESPTxStats.numTxPkts, ctxESPTxStats.numTxPkts, 1
    adc     ctxESPTxStats.numTxPktsHi, ctxESPTxStats.numTxPktsHi, 0
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

    // Insert/Replace the Hash data
    mov     r0.b0, ctxESPTx1.icvSize
    xout    XID_CDEDATA, r6, b0
      
 phpEspScTxProc_2_HashProc_End:
    //
    //  Process EOP Data Chunk
    //
    // Move to save three stalls
    // add     ctxESPTx1.numTxPkts, ctxESPTx1.numTxPkts, 1
    // clr     phpContext.instMask, espTxPass2Loc1.pktId
    // xout    XID_SCVDATA, ctxESPTx1, SIZE(ctxESPTx1)
    
 phpEspScTxProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Forward this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop

.leave espTxPass2Scope
.leave ctxESPTx1Scope
.leave commCtxScope    
 

// Register MAP
// R0, R29       : General and local register
// R6-R8         : rtp header (from packet)
// R14-R21       : IPSEC Security Context including all parametes required
// R10-R13       : output iv 
// R30           : return address

.struct struct_espAesCtr
    .u32  salt
    .u32  iv1
    .u32  iv2
    .u32  cnt
.ends

.struct struct_espAad
    .u32  spi
    .u32  data1
    .u32  data2
    .u32  data3
.ends

.enter espTxCmdlScope
    .assign  struct_saCmdL,             R12, R13, saCmdL
    .assign  struct_saCmdLReg,          R12, R13, saCmdLReg
.leave espTxCmdlScope 

.enter espAesCtrScope
    .assign  struct_espAesCtr,          R7, R10, saEspAesCtr 
.leave espAesCtrScope

.enter espAadScope
    .assign  struct_espAad,             R10, R13, saEspAad 
.leave espAadScope
        
 .using espTxPass1Scope
 .using ctxESPTx1Scope
 .using espHdrScope
 .using espTxCmdlScope
 .using espAesCtrScope
 
phpEspTxCmdlProc_Gen:

    // Construct command Label in ESP General mode
    
    // Is encryption enabled
    qbbs    phpEspTxCmdlProc_Gen_L4, ctxESPTx1.ctrlInfo.flagESPTxNullEnc 
    zero    &saCmdL, SIZE(saCmdL)
    
    // Construct the encryption command label 
    qbbc    phpEspTxCmdlProc_Gen_L1, ctxESPTx1.ctrlInfo.flagESPTxUseEnc
        mov     saCmdL.nextEngId, SAENG_ENCRYPT_2
        jmp     phpEspTxCmdlProc_Gen_L2


phpEspTxCmdlProc_Gen_L1:
    qbne    phpEspTxCmdlProc_Gen_L1_1, ctxESPTx1.icvSize, 0
    mov     saCmdL.nextEngId, SAENG_PHP_2
    jmp     phpEspTxCmdlProc_Gen_L2

phpEspTxCmdlProc_Gen_L1_1:
    // Use Authentication Engine
    mov     saCmdL.nextEngId, SAENG_AUTH_1

phpEspTxCmdlProc_Gen_L2:
    mov     saCmdL.procLen, espTxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset, espTxPass1Loc1.protoOffset, espTxPass1Loc1.espHdrSize
    
    // Special Processing is required for AES CTR mode
    qbbc    phpEspTxCmdlProc_Gen_L3, ctxESPTx1.ctrlInfo.flagESPTxAESCTR
    
    // AES CTR operation
    mov     saCmdL.cmdlLen,  SA_AESCTR_CMDL_SIZE
    //mov     saCmdL.optctrl1, SA_CMDL_MK_OPT_CTRL(SA_ENC_AUX2_OFFSET, 16)
    mov     saCmdL.optctrl1, SA_ENC_AUX2_OFFSET + (16 >> 3)
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    mov     saEspAesCtr.salt,  ctxESPTx1.salt
    mov     saEspAesCtr.cnt,   1  
    sbco    saEspAesCtr, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    jmp     phpEspTxCmdlProc_Gen_L4   
    
phpEspTxCmdlProc_Gen_L3:  
    add     saCmdL.cmdlLen,  ctxESPTx1.ivSize, SIZE(saCmdL)
    lsr     r0.b0, ctxESPTx1.ivSize, 3 
    add     saCmdL.optctrl1, r0.b0, SA_ENC_AUX2_OFFSET 
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    mov     r0.b0, ctxESPTx1.ivSize
    sbco    espHdr.iv1, cCmdlBuf, espTxPass1Loc1.cmdlOffset, b0
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, r0.b0
    

phpEspTxCmdlProc_Gen_L4:
    // Format the encryption command label
    zero    &saCmdL, SIZE(saCmdL)
    qbeq    phpEspTxCmdlProc_Gen_End, ctxESPTx1.icvSize, 0
    mov     saCmdL.nextEngId, SAENG_PHP_2
    mov     saCmdL.procLen, espTxPass1Loc1.hashDataLen
    mov     saCmdL.sopOffset, espTxPass1Loc1.protoOffset
    
    // Process CMAC 
    qbbc    phpEspTxCmdlProc_Gen_L7, ctxESPTx1.ctrlInfo.flagESPTxCMAC
    mov     saCmdL.cmdlLen, SA_CMAC_CMDL_SIZE
    //mov     saCmdL.optctrl1, SA_CMDL_MK_OPT_CTRL(SA_ENC_AUX1_OFFSET, 16)
    mov     saCmdL.optctrl1, SA_ENC_AUX1_OFFSET + (16 >> 3)
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Move the security context viewer to the K1/K2 location 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + SIZE(struct_ctxESPTx)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4   
    xin     XID_SCVDATA,  r6,  32
    
    and     r0.w0, espTxPass1Loc1.hashDataLen, 0x000f
    qbne    phpEspTxCmdlProc_Gen_L5, r0.w0, 0
    // K1 is used
    sbco    r6, cCmdlBuf, espTxPass1Loc1.cmdlOffset, 16
    jmp     phpEspTxCmdlProc_Gen_L6
    
phpEspTxCmdlProc_Gen_L5:
    // k2 is used
    sbco    r10, cCmdlBuf, espTxPass1Loc1.cmdlOffset, 16
    
phpEspTxCmdlProc_Gen_L6:
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset,  16
    ret     
    
phpEspTxCmdlProc_Gen_L7:    
    mov     saCmdL.cmdlLen, SIZE(saCmdL)
    mov     saCmdL.optctrl1, 0
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
phpEspTxCmdlProc_Gen_End:
    ret       
    
    
phpEspTxCmdlProc_GCM:

    // Construct command Label in ESP GCM Mode
    // Construct and copy basic label
    mov     saCmdLReg.reg1.w2,  SA_GCM_CMDL_SIZE + (SAENG_PHP_2 << 8)
    mov     saCmdL.procLen, espTxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset, espTxPass1Loc1.protoOffset, espTxPass1Loc1.espHdrSize
    mov     saCmdL.optctrl1, SA_GCM_CMDL_OPT1
    mov     saCmdLReg.reg2.w0,  SA_GCM_CMDL_OPT3 + (SA_GCM_CMDL_OPT2 << 8)
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construst and copy Aux1
    zero    &saCmdL, SIZE(saCmdL)
    lsl     saCmdLReg.reg2, espTxPass1Loc1.encryptedDataLen, 3
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
    // Construct and Copy AAD
    .using espAadScope
    zero    &saEspAad, SIZE(saEspAad)
    mov     saEspAad.spi, espHdr.spi 

    qbbs    phpEspTxCmdlProc_GCM_L1, ctxESPTx1.ctrlInfo.flagESPTxESN
    mov     saEspAad.data1, ctxESPTx1.esnLo
    jmp     phpEspTxCmdlProc_GCM_L2
    
phpEspTxCmdlProc_GCM_L1:
    mov     saEspAad.data1, ctxESPTx1.esnHi
    mov     saEspAad.data2, ctxESPTx1.esnLo

phpEspTxCmdlProc_GCM_L2: 
    sbco    saEspAad, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saEspAad)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saEspAad)
   
    .leave espAadScope
    
    // Construct and copy AES CTR 
    mov     saEspAesCtr.salt,  ctxESPTx1.salt
    mov     saEspAesCtr.cnt,   1  
    sbco    saEspAesCtr, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    ret
    
phpEspTxCmdlProc_GMAC:

    // Construct command Label in ESP GMAC Mode
    // Construct and copy basic label
    mov     saCmdLReg.reg1.w2,  SA_GMAC_CMDL_SIZE + (SAENG_PHP_2 << 8)
    mov     saCmdL.procLen, espTxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset, espTxPass1Loc1.protoOffset, espTxPass1Loc1.espHdrSize
    mov     saCmdL.optctrl1, SA_GMAC_CMDL_OPT1
    mov     saCmdLReg.reg2.w0,  SA_GMAC_CMDL_OPT3 + (SA_GMAC_CMDL_OPT2 << 8)
    
    qbbs    phpEspTxCmdlProc_GMAC_L1, ctxESPTx1.ctrlInfo.flagESPTxESN
    // Non-ESN operation
    // The 8-byte IV in front of ESP payload is copied to the AAD field.
    // The total authentication length should include 8-byte IV
    mov     saCmdL.procLen, espTxPass1Loc1.encryptedDataLen
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construst and copy Aux1
    zero    &saCmdL, SIZE(saCmdL)
    add     saCmdLReg.reg2.w0, espTxPass1Loc1.encryptedDataLen, (SA_GMAC_ESP_AAD_LEN1 + 8)
    lsl     saCmdLReg.reg2, saCmdLReg.reg2, 3
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
    // Construct and Copy AAD
    .using espAadScope
    // Copy paylaod data before they are overwritten by SPI and SN
    mov     saEspAad.data2, espHdr.iv1
    mov     saEspAad.data3, espHdr.iv2
    mov     saEspAad.spi, espHdr.spi 
    mov     saEspAad.data1, ctxESPTx1.esnLo
    jmp     phpEspTxCmdlProc_GMAC_L2
    
phpEspTxCmdlProc_GMAC_L1:
    // ESN operation
    add     saCmdL.procLen, espTxPass1Loc1.encryptedDataLen, 4
    sub     saCmdL.sopOffset, saCmdL.sopOffset, 4 
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construst and copy Aux1
    zero    &saCmdL, SIZE(saCmdL)
    add     saCmdLReg.reg2.w0, espTxPass1Loc1.encryptedDataLen, (SA_GMAC_ESP_AAD_LEN2 + 8)
    lsl     saCmdLReg.reg2, saCmdLReg.reg2, 3
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
    // Construct and Copy AAD
    // Copy paylaod data before they are overwritten by SPI and SN
    mov     saEspAad.data3, espHdr.iv1
    mov     saEspAad.spi, espHdr.spi 
    mov     saEspAad.data1, ctxESPTx1.esnHi
    mov     saEspAad.data2, ctxESPTx1.esnLo

phpEspTxCmdlProc_GMAC_L2: 
    sbco    saEspAad, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saEspAad)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saEspAad)
   
    .leave espAadScope
    
    // Construct and copy AES CTR 
    mov     saEspAesCtr.salt,  ctxESPTx1.salt
    mov32   saEspAesCtr.cnt,   1  
    sbco    saEspAesCtr, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    ret
    
phpEspTxCmdlProc_CCM:

    // Construct command Label in ESP CCM Mode
    // Construct and copy basic label
    mov     saCmdLReg.reg1.w2,  SA_CCM_CMDL_SIZE + (SAENG_PHP_2 << 8)
    mov     saCmdL.procLen, espTxPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset, espTxPass1Loc1.protoOffset, espTxPass1Loc1.espHdrSize
    mov     saCmdL.optctrl1, SA_CCM_CMDL_OPT1
    mov     saCmdLReg.reg2.w0,  SA_CCM_CMDL_OPT3 + (SA_CCM_CMDL_OPT2 << 8)
    sbco    saCmdL, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construct and Copy B0
    // R6-R7 (SPI, SN), R5 command data
    // clean up B0, lower 8-byte
    zero    &saCmdL, SIZE(saCmdL)
    add     r0.b0, espTxPass1Loc1.cmdlOffset, 8
    sbco    saCmdL, cCmdlBuf, r0.b0, 8
    
    qbbs    phpEspTxCmdlProc_CCM_L1, ctxESPTx1.ctrlInfo.flagESPTxESN
    
    // Non-ESN operation (00|08|SPI|SN|000000000000)
    mov     r5.w0, SA_CCM_ESP_AAD_LEN1
    sbco    r5.w0, cCmdlBuf, espTxPass1Loc1.cmdlOffset, 10
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, 16
    jmp     phpEspTxCmdlProc_CCM_L2
    
phpEspTxCmdlProc_CCM_L1:
    // ESN operation (00|0c|SPI|ESN|0000)
    mov     r5.w0, SA_CCM_ESP_AAD_LEN2
    sbco    r5.w0, cCmdlBuf, espTxPass1Loc1.cmdlOffset, 6
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, 6
    mov     espHdr.spi, ctxESPTx1.esnHi
    sbco    espHdr, cCmdlBuf, espTxPass1Loc1.cmdlOffset, 8
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, 10

phpEspTxCmdlProc_CCM_L2: 
    // Construct and Copy B0
    mov     saEspAesCtr.salt,  ctxESPTx1.salt
    mov     saEspAesCtr.cnt,   espTxPass1Loc1.encryptedDataLen
    sbco    saEspAesCtr, cCmdlBuf,  espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    
    // Construct and copy AES CTR 
    and     saEspAesCtr.salt.b3,  saEspAesCtr.salt.b3, 0x07
    zero    &saEspAesCtr.cnt,   4  
    sbco    saEspAesCtr, cCmdlBuf, espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    add     espTxPass1Loc1.cmdlOffset, espTxPass1Loc1.cmdlOffset, SIZE(saEspAesCtr)
    
    ret
    
  
 .leave espTxPass1Scope
 .leave ctxESPTx1Scope
 .leave espHdrScope
 .leave espTxCmdlScope
 .leave espAesCtrScope
    
