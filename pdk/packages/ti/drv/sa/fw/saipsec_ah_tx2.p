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
//  * FILE PURPOSE:  PA firmware for PDSPs with AH Tx operation
//  **********************************************************************************
//  * FILE NAME:  saipsec_ah_tx.p
//  *
//  * DESCRIPTION: process the IPSEC AH Tx (To-Network) packets
//  *
//  **********************************************************************************/
///***************************************************************************
//* FILE PURPOSE: SA HPS - IPSec AH mode handling
//****************************************************************************
//* FILE NAME: saipsec_ah.c
//*
//* DESCRIPTION:
//*  This file contains functions for SA packet header processing of the IPSec 
//*  AH packets
//*
//*
//*     0                   1                   2                   3
//*     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//*   | Next Header   |  Payload Len  |          RESERVED             |
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//*   |                 Security Parameters Index (SPI)               |
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//*   |                    Sequence Number Field                      |
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//*   |                                                               |
//*   +                Integrity Check Value-ICV (variable)           |
//*   |                                                               |
//*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//*
//*                   Figure 1.  AH Format
//*
//*
//*
//*                   AFTER APPLYING AH
//*             -------------------------------------------------------
//*       IPv4  |original IP hdr (any options) | AH | TCP |    Data   |
//*             -------------------------------------------------------
//*             |<- mutable field processing ->|<- immutable fields ->|
//*             |<----- authenticated except for mutable fields ----->|
//*
//*
//*                       AFTER APPLYING AH
//*            ------------------------------------------------------------
//*      IPv6  |             |hop-by-hop, dest*, |    | dest |     |      |
//*            |orig IP hdr  |routing, fragment. | AH | opt* | TCP | Data |
//*            ------------------------------------------------------------
//*            |<--- mutable field processing -->|<-- immutable fields -->|
//*            |<---- authenticated except for mutable fields ----------->|
//*
//*
//*                 Figure 2. AH Transport Mode
//*
//*
//*        ----------------------------------------------------------------
//*   IPv4 |                              |    | orig IP hdr*  |   |      |
//*        |new IP header * (any options) | AH | (any options) |TCP| Data |
//*        ----------------------------------------------------------------
//*        |<- mutable field processing ->|<------ immutable fields ----->|
//*        |<- authenticated except for mutable fields in the new IP hdr->|
//*
//*        --------------------------------------------------------------
//*   IPv6 |           | ext hdrs*|    |            | ext hdrs*|   |    |
//*        |new IP hdr*|if present| AH |orig IP hdr*|if present|TCP|Data|
//*        --------------------------------------------------------------
//*        |<--- mutable field -->|<--------- immutable fields -------->|
//*        |       processing     |
//*        |<-- authenticated except for mutable fields in new IP hdr ->|
//*
//*          * = if present, construction of outer IP hdr/extensions and
//*              modification of inner IP hdr/extensions is discussed in
//*              the Security Architecture document.
//*
//*                Figure 3. AH Tunnel mode
//*
//*
//* (C) Copyright 2008 Texas Instruments, Inc.
//*****************************************************************************/
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

//*******************************************************************************
// * Local Data Structures
// ******************************************************************************/
//

.struct struct_ahTxPass1Loc1
    .u16    hashDataLen        // Hash Data length (for command label)
    .u8     ahHdrSize          // AH Header Size (for command label)
    .u8     ipVer              // IP Version 
    .u8     protoOffset        // Protocol Offset
    .u8     pktOffset          // Current Offset of the packet
    .u8     cmdlOffset         // Command Label Offset 
    .u8     cmdlIVOffset       // IV offset in the command label (for copy data)
.ends     

.enter ahTxPass1Scope
    .assign  struct_ahTxPass1Loc1,  R2,  R3, ahTxPass1Loc1
    .assign  struct_ahPktInst, R22, R26,   ahPktInst      
.leave ahTxPass1Scope

.enter ctxAHTxScope
    .assign  struct_ctxAHTx1,  R14,  R19.w2, ctxAHTx1
.leave ctxAHTxScope

.enter ctxAHTxScopeStats
    .assign struct_ctxAHTxStats, R14, R18, ctxAHTxStats
.leave ctxAHTxScopeStats

//.enter ahTxCmdlScope
//    .assign  struct_cmdlCfgEntry,      R6, R7,   cmdlCfgEntry
//    .assign  struct_cmdlCfgEntryReg,   R6, R7,   cmdlCfgEntryReg 
//    .assign  struct_saCmdL,            R10, R11, saCmdL
//    .assign  struct_saCmdLReg,         R10, R11, saCmdLReg
//    .assign  struct_saCmdlOpt,         R10, R13, saCmdlOpt
//.leave ahTxCmdlScope   

.enter ahIpScope
    .assign  struct_Ip,                R6, R10,  ip4Hdr
    .assign  struct_IpOpt,             R6, R6,   ipOpt
    .assign  struct_Ipv6Opt,           R6, R6,   ipv6Opt
    .assign  struct_Ipv6Ext,           R6, R6,   ipv6Ext
    .assign  struct_Ipv6ExtRt,         R6, R7,   ipv6ExtRt
    .assign  struct_Ip6_1stHalf,       R6, R10,  ip6Hdr_1stHalf
    .assign  struct_Ip6_2ndHalf,       R6, R10,  ip6Hdr_2ndHalf
.leave ahIpScope     
    
.enter ahHdrScope
    .assign  struct_ahHdr,             R6, R12,  ahHdr
.leave ahHdrScope   

//******************************************************************************
//* Function:    sap_ipsec_ah_tx_proc_pass1
//******************************************************************************
//* Description: This function perfroms the first pass of the IPSec AH Tx packet
//*              header processing as the followings: 
//* 
//*  - Extract the following information from the IP header and replace them with zero
//*      IPV4: TOS, Flags, Fragment Offset and TTL
//* 	    IPV6: Traffic Class, Flow Label and Hop Limit
//*  - Zero out IPV4 header checksum
//*  - Record and zero out the mutable option fields
//*  - Fill in the AH header parameters(SPI and Sequence Number)
//*  - Extract and clear IV in the AH header if exists
//* 	- Clear the HASH Value in the AH header 
//* 	- Insert the Authentication Command Label with data length and offset
//*	- Insert the HPS2 command label
//*  - Insert the upper 32-bit of ESN at the end of payload if configured
//*
//*  Where the packet info consists of
//*        - pointer to the packet
//*        - packet length
//*        - offset to the (outer) IP header 
//*
//*****************************************************************************/

phpAhScTxProc_1:

    // Error Check
    
    
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using ahTxPass1Scope
    .using ctxAHTxScope
    
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAHTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxAHTx1, SIZE(ctxAHTx1)
    
    // clear Local Variables (may be just some of them)
    zero    &ahTxPass1Loc1,    SIZE(ahTxPass1Loc1)    
    
 phpAhScTxProc_1_FirstChunk_L1:    
    //clear the control flags and other variables
    //zero    &ahPktInst,    SIZE(ahPktInst)
    qbbs    phpAhScTxProc_1_noESNOverFlow, ctxAHTx1.ctrlInfo.flagAHTxESN
    fill    &r0, 4
    qbne    phpAhScTxProc_1_noESNOverFlow, r0, ctxAHTx1.esnLo
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4

    .using ctxAHTxScopeStats
    xin     XID_SCVDATA, ctxAHTxStats, SIZE(ctxAHTxStats)
    add     ctxAHTxStats.rollOverCounter, ctxAHTxStats.rollOverCounter, 1
    xout    XID_SCVDATA, ctxAHTxStats, SIZE(ctxAHTxStats)
    .leave ctxAHTxScopeStats
    // Drop packet
    jmp     phpAhScTxProc_1_Err_Esn_Overflow

phpAhScTxProc_1_noESNOverFlow:
    //Increment and store the ESN 
    add     ctxAHTx1.esnLo,  ctxAHTx1.esnLo,  1
    adc     ctxAHTx1.esnHi,  ctxAHTx1.esnHi,  0
    xout    XID_SCVDATA,  ctxAHTx1,  SIZE(ctxAHTx1)
    
    //Record the control size - the short payload Info
    sub     ahTxPass1Loc1.pktOffset,  PktDesc.ControlSize, 8
    
 phpAhScTxProc_1_FirstChunk_L2:  
    
    //Update PktDesc for the packet
    mov     PktDesc.SA_StatusLen, 16
    mov     PktDesc.SA_NextEngId, ctxAHTx1.firstEngId
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    // Update the first two words only */
    xout    XID_CDEDATA, PktDesc, 8   
      
    // Advance the window past the Descriptor + PS Info
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_CONTROL
    xout    XID_CDECTRL, CdeCmd, 4   
    
    .leave pktDescScope
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
    qbeq    phpAhScTxProc_ShortInfo_L1, r0.b0, 1
    
phpAhScTxProc_LongInfo_L1:
    mov     payloadInfoS.offset,  payloadInfoL.l3Offset
    sub     payloadInfoS.len, payloadInfoL.eopOffset, payloadInfoL.l3Offset  
    // re-adjust pktOffset
    add     ahTxPass1Loc1.pktOffset,  ahTxPass1Loc1.pktOffset, 8
    jmp     phpAhScTxProc_protoInfo_End
    
phpAhScTxProc_ShortInfo_L1:    
    
    // TBD: Issue command to flash out and copy PSInfo
    // Now we're at the control section. Insert the command label here
    // Advance the window past the Descriptor
    // TBD: Move is desc size + phys PsInfo size
    // We may want to move the following two instruction up to use the store cycles
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4      

phpAhScTxProc_protoInfo_End:
    mov     CdeCmd.Cmd0.b1, ctxAHTx1.cmdlTotalLen
    mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
    xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
       
    // Advance to the IP header
    // Move to save up to 5 stalled cycles
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, payloadInfoS.offset, ahTxPass1Loc1.pktOffset
    xout    XID_CDECTRL, CdeCmd, 4      
       
    // Calculate the payload related information
    mov     ahPktInst.ipOffset,  payloadInfoS.offset
    mov     ahTxPass1Loc1.hashDataLen, payloadInfoS.len
    
    // Calculate and record Various Offsets
    add     ahPktInst.esnOffset,   ahPktInst.ipOffset, payloadInfoS.len
    
    // Update HashDataLen if ESN is enabled
    qbbc    phpAhScTxProc_1_FirstChunk_L3, ctxAHTx1.ctrlInfo.flagAHTxESN
    add     ahTxPass1Loc1.hashDataLen, ahTxPass1Loc1.hashDataLen, 4  
   
    .leave protoInfoScope
    
 phpAhScTxProc_1_FirstChunk_L3:
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4

    .using ctxAHTxScopeStats
    xin     XID_SCVDATA, ctxAHTxStats, SIZE(ctxAHTxStats)
    add     ctxAHTxStats.byteCount, ctxAHTxStats.byteCount, ahTxPass1Loc1.hashDataLen
    adc     ctxAHTxStats.byteCountHi, ctxAHTxStats.byteCountHi, 0
    xout    XID_SCVDATA, ctxAHTxStats, SIZE(ctxAHTxStats)
    .leave ctxAHTxScopeStats
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAHTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4

    xin     XID_SCVDATA, ctxAHTx1, SIZE(ctxAHTx1)
    // Advance to the IP header
    // Move to savew some stalled cycles
    // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    // add     CdeCmd.Cmd0.w2, ahPktInst.ipOffset, ahTxPass1Loc1.pktOffset
    // xout    XID_CDECTRL, CdeCmd, 4      
                         
    // Load and processing IP header
    .using ahIpScope
    add     r0.w2, phpContext.instOffset, AH_PKT_INST_IPBUF_OFFSET 
    xin     XID_CDEDATA, ip4Hdr, SIZE(ip4Hdr)
    sbco    ip4Hdr, cInstBase, r0.w2, SIZE(ip4Hdr)
    
    and     ahTxPass1Loc1.ipVer, ip4Hdr.VerLen, IPV4_VER_MASK
    qbeq    phpAhScTxProc_1_FirstChunk_IP4, ahTxPass1Loc1.ipVer, IP_TYPE_IPV4
    qbeq    phpAhScTxProc_1_FirstChunk_IP6, ahTxPass1Loc1.ipVer, IP_TYPE_IPV6
    jmp     phpAhScTxProc_1_Err_InvalidIP
    
 phpAhScTxProc_1_FirstChunk_IP4:
    // clear TOS, Flags and Frago, TTL and checksum
    mov     ip4Hdr.Tos, 0
    zero    &ip4Hdr.FragOff, 3
    mov     ip4Hdr.Checksum, 0
    xout    XID_CDEDATA, ip4Hdr, SIZE(ip4Hdr) 
    and     r0.b0,  ip4Hdr.VerLen, IPV4_LEN_MASK
    lsl     ahPktInst.ipHdrSize,  r0.b0, 2 
    
    // option processing
    qbeq    phpAhScTxProc_1_FirstChunk_IP_End, ahPktInst.ipHdrSize, SIZE(ip4Hdr)
    qbgt    phpAhScTxProc_1_Err_InvalidIP, ahPktInst.ipHdrSize, SIZE(ip4Hdr)
        // option processing
        // r0.w2: IP buffer location
        // r0.b0: remaining option length
        // r0.b1: current option length
        // r29: mutable bitmap
        
        // Advance to IPv4 option area
        // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
        mov     CdeCmd.Cmd0.w2, SIZE(ip4Hdr)
        xout    XID_CDECTRL, CdeCmd, 4 
        
        // Load the option data
        // Assumption: option len <= 32
        xin     XID_CDEDATA, ipOpt, 32
        sub     r0.b0, ahPktInst.ipHdrSize, SIZE(ip4Hdr)
        add     ahTxPass1Loc1.pktOffset,  ahPktInst.ipOffset, ahPktInst.ipHdrSize  
        
        // load mutable bitmap
        mov32    r29, IPV4_OPT_MUTABILITY_BITMAP
        
phpAhScTxProc_1_ipOpt_loop:

        // End of option check
        qbne phpAhScTxProc_1_ipOpt_1, ipOpt.opt, IPV4_OPT_END_OF_LIST
            // End of option
            // set the option length to the remaining length
            mov     r0.b1, r0.b0
            
            // store the remaining data to the IP buffer
            sbco    ipOpt, cInstBase, r0.w2, b1
            jmp     phpAhScTxProc_1_ipOpt_4
            
phpAhScTxProc_1_ipOpt_1:            
        qbne phpAhScTxProc_1_ipOpt_2, ipOpt.opt, IPV4_OPT_NOP
            mov     r0.b1, 1
            sbco    ipOpt, cInstBase, r0.w2, b1
            jmp     phpAhScTxProc_1_ipOpt_4
            
phpAhScTxProc_1_ipOpt_2:
        // Common processing for non-special case
        // option length range check
        qbgt    phpAhScTxProc_1_Err_InvalidIP, ipOpt.len, 2
        qblt    phpAhScTxProc_1_Err_InvalidIP, ipOpt.len, r0.b0
            
        // store the original options
        mov     r0.b1,  ipOpt.len
        sbco    ipOpt,  cInstBase, r0.w2, b1
        
        // extract the option value
        and     ipOpt.opt,  ipOpt.opt,  IPV4_OPT_MASK
        
        // pass through
        qbeq phpAhScTxProc_1_ipOpt_daddrPatch, ipOpt.opt, IPV4_OPT_LOOSE_SOURCE_ROUTE
        qbeq phpAhScTxProc_1_ipOpt_daddrPatch, ipOpt.opt, IPV4_OPT_STRICT_SOURCE_ROUTE
        
        //general options
        qbbs    phpAhScTxProc_1_ipOpt_4, r29,  ipOpt.opt
phpAhScTxProc_1_ipOpt_3:  
            // it is a mutable option, zero out the entire option
            zero &ipOpt, 32
            xout XID_CDEDATA, ipOpt, b1
            // pass through 
           
phpAhScTxProc_1_ipOpt_4:  
        sub     r0.b0, r0.b0, r0.b1
        
        // advance over this option
        mov     CdeCmd.Cmd0.w2, r0.b1
        xout    XID_CDECTRL, CdeCmd, 4 
        
        qbeq    phpAhScTxProc_1_FirstChunk_IP_End_1, r0.b0, 0
            add     r0.w2, r0.w2, r0.b1
            xin     XID_CDEDATA, ipOpt, 32
            jmp     phpAhScTxProc_1_ipOpt_loop
            
phpAhScTxProc_1_ipOpt_daddrPatch:            
        qbgt    phpAhScTxProc_1_Err_InvalidIP, ipOpt.len, 6
            add r1.b0, ipOpt.len, &ipOpt - 4        
            // patch the IP destination address
            // Issue patch command to copy the original IP
            mov     CdeCmd.Cmd0.w0, CDE_CMD_PATCH_PACKET | (4 << 8)
            mov     CdeCmd.Cmd0.w2, ahPktInst.ipOffset
            mvid    CdeCmd.Cmd1, *r1.b0
            xout    XID_CDECTRL, CdeCmd, 8
            
            //prepare for next opertaion
            mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
            jmp     phpAhScTxProc_1_ipOpt_3
        
    
 phpAhScTxProc_1_FirstChunk_IP6:
    // clear DSCP/TOS, ECN, Flow Label, and Hop Limit
    mov     r1.b3, ip6Hdr_1stHalf.NextHeader
    and     ip6Hdr_1stHalf.VerDsHi, ip6Hdr_1stHalf.VerDsHi, IPV6_VER_MASK
    zero    &ip6Hdr_1stHalf.DsHiFlowIDHi, 3
    mov     ip6Hdr_1stHalf.HopLimit, 0
    xout    XID_CDEDATA, ip6Hdr_1stHalf, SIZE(ip6Hdr_1stHalf)
    
    // Advance to the 2nd Half of IPv6 header
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, SIZE(ip6Hdr_1stHalf)
    xout    XID_CDECTRL, CdeCmd, 4      
 
    xin     XID_CDEDATA, ip6Hdr_2ndHalf, SIZE(ip6Hdr_2ndHalf)
    
    // Store the second half of IPv6 header
    add     r0.w2, r0.w2, SIZE(ip6Hdr_1stHalf) 
    sbco    ip6Hdr_2ndHalf, cInstBase, r0.w2, SIZE(ip6Hdr_2ndHalf)
    mov     ahPktInst.ipHdrSize, SIZE(ip6Hdr_1stHalf) + SIZE(ip6Hdr_2ndHalf)
    
    qbeq    phpAhScTxProc_1_FirstChunk_IP_End, r1.b3, IP_PROTO_NEXT_AUTH
        // extension header processing:
        // Only the following extension header is legal to be put in front of AH header
        // IP_PROTO_NEXT_IPV6_ROUTE (43): need to rearrange the routing address as they appear at the destination
        // IP_PROTO_NEXT_IPV6_HOP_BY_HOP (0): zero out mutable options
        // IP_PROTO_NEXT_IPV6_DEST_OPT (60): zero out mutable options
        // r0.w2: IP buffer location
        // r0.b1: current option length
        // r1.b0: Temp buffer offset including the base offset (0x80 + (address index + 1) * 16
        // r1.b3: nextHdr
        
        // Advance to IPv6 extension header area
        // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
        mov     CdeCmd.Cmd0.w2, SIZE(ip6Hdr_1stHalf)
        xout    XID_CDECTRL, CdeCmd, 4 
        add     ahTxPass1Loc1.pktOffset,  ahPktInst.ipOffset, ahPktInst.ipHdrSize  
        
        // Copy the destination address to temporary buffer location in case RT rearrage is required
        // Temp buffer (the second half of command buffer): base address = 0x180  (0x80 from command buffer)
        // 0x80: destination address (addrs[0] = iph->daddr;)
        // 0x90: first route address to be moved
        // 0xa0: second route address to be moved
        // ...
        // 0xx0: last route address to be moved
        // where the first group hould be patched to extension header as it appears at the received end
        // memmove(addrs + 1, addrs, (segments_left - 1) * sizeof(*addrs));
        // And the last address should be patched to IPv6 destination address
        // final_addr = addrs[segments - 1]; iph->daddr = final_addr
        //
        sbco    ip6Hdr_2ndHalf.DstIp0, cCmdlBuf, 0x80, 16
        
phpAhScTxProc_1_ipv6Ext_loop:        
        
        // Load the extension header
        xin     XID_CDEDATA, ipv6Ext, 32
        
        // extension hdaer length = (hdrLen + 1) * 8
        add     r0.b1, ipv6Ext.hdrLen, 1
        lsl     r0.b1, r0.b1, 3 
        
        // TBD: check for IP header buffer overflow
        
        qbeq    phpAhScTxProc_1_ipv6Ext_opt, r1.b3,  IP_PROTO_NEXT_IPV6_HOP_BY_HOP
        qbeq    phpAhScTxProc_1_ipv6Ext_opt, r1.b3,  IP_PROTO_NEXT_IPV6_DEST_OPT
        qbne    phpAhScTxProc_1_Err_InvalidIP, r1.b3, IP_PROTO_NEXT_IPV6_ROUTE
        
phpAhScTxProc_1_ipv6ExtRt:        
            // Ipv6 route option processing
            // Error check: type = 0, len is even,  len/2 >= segsLeft, len/2 > 1
            qbne    phpAhScTxProc_1_Err_InvalidIP,    ipv6ExtRt.routeType, 0 
            qbbs    phpAhScTxProc_1_Err_InvalidIP,    ipv6ExtRt.hdrLen, 0
            lsr     r1.b2, ipv6ExtRt.hdrLen, 1
            qblt    phpAhScTxProc_1_Err_InvalidIP,    ipv6ExtRt.segsLeft, r1.b2
                // record next header 
                mov     r1.b3, ipv6ExtRt.proto
                // Calculate offset to the first address needs to be updated
                // r0.b0: offset = 8 + (len/2 - segLeft)*16
                // r1.b0: temp buffer offset = 0x90 
                // r1.b1: number of segments left to be process
                sub     r0.b0, r1.b2, ipv6ExtRt.segsLeft
                lsl     r0.b0, r0.b0, 4
                add     r0.b0, r0.b0, 8
                mov     r1.b0, (0x80 + 16)
                mov     r1.b1, ipv6ExtRt.segsLeft 
                
                // save the first portion of extension header
                sbco    ipv6ExtRt,  cInstBase, r0.w2, b0
                add     r0.w2, r0.w2, r0.b0
                
                // zero-out segsLeft
                mov     ipv6ExtRt.segsLeft, 0
                xout    XID_CDEDATA, ipv6ExtRt, 4 
                
                //advance to the first address field to be moved
                mov     CdeCmd.Cmd0.w2, r0.b0
                xout    XID_CDECTRL, CdeCmd, 4 
                
                //  Prepare the move command for the loop processing
                mov     CdeCmd.Cmd0.w2, 16
                
    phpAhScTxProc_1_ipv6ExtRt_loop:                
                //loop    phpAhScTxProc_1_ipv6ExtRt_loop_end,  r1.b1 
                    //load and save the intermediate address to IP and temp buffer
                    xin     XID_CDEDATA, r6, 16
                    sbco    r6,  cInstBase, r0.w2, 16
                    add     r0.w2, r0.w2, 16
                
                    sbco    r6, cCmdlBuf, r1.b0, 16
                    add     r1.b0, r1.b0, 16
                
                    //advance to the next address field
                    xout    XID_CDECTRL, CdeCmd, 4 
                    
                    sub     r1.b1, r1.b1, 1
                    
                    qbne    phpAhScTxProc_1_ipv6ExtRt_loop, r1.b1, 0
                    
                
        phpAhScTxProc_1_ipv6ExtRt_loop_end:    
                    
                // Issue patch command to patch the destination address
                mov     CdeCmd.Cmd0.w0, CDE_CMD_PATCH_PACKET_BUFFER | (16 << 8)
                add     CdeCmd.Cmd0.w2, ahPktInst.ipOffset, 24
                mov     CdeCmd.Cmd1, PHP_TEMP_BUF_ADDR - (0x80 + 16)
                add     CdeCmd.Cmd1, CdeCmd.Cmd1, r1.b0
                xout    XID_CDECTRL, CdeCmd, 8
                
                // Issue patch comnmand to patch the entension header
                sub     CdeCmd.Cmd0.b1, r1.b0, (0x80 + 16)
                add     CdeCmd.Cmd0.w2, ahTxPass1Loc1.pktOffset, r0.b0
                mov     CdeCmd.Cmd1, PHP_TEMP_BUF_ADDR
                xout    XID_CDECTRL, CdeCmd, 8
                
                mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
                add     ahTxPass1Loc1.pktOffset,  ahTxPass1Loc1.pktOffset, r0.b1
                
phpAhScTxProc_1_ipv6ExtRt_end:
                
                //pass through
        qbeq    phpAhScTxProc_1_FirstChunk_IP_End_1, r1.b3, IP_PROTO_NEXT_AUTH
            jmp     phpAhScTxProc_1_ipv6Ext_loop    
                       
        
phpAhScTxProc_1_ipv6Ext_opt:
        // walk through all option fields and zero out mutable options
        // r0.w2: IP buffer location
        // r0.b0: remaining option length
        // r0.b1: current option length (total option length at input)
        
        // save next header and adjust pktOffset for the next protocol
        mov     r1.b3,  ipv6Ext.proto
        add     ahTxPass1Loc1.pktOffset,  ahTxPass1Loc1.pktOffset, r0.b1
        
        // save the first portion of data
        sbco    ipv6Ext,  cInstBase, r0.w2, 2
        add     r0.w2,  r0.w2, 2
        
        // Advance to extension header option area
        // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
        mov     CdeCmd.Cmd0.w2, 2
        xout    XID_CDECTRL, CdeCmd, 4 
        
        // Initialize the original option length
        sub     r0.b0, r0.b1, 2
        
phpAhScTxProc_1_ipv6Ext_opt_loop:
        // Load the option data
        xin     XID_CDEDATA, ipv6Opt, 32

        // End of option check
        qbne phpAhScTxProc_1_ipv6Ext_opt_1, ipv6Opt.type, IPV6_OPT_HOP_BY_HOP_OPT_PAD0
            // single-byte padding
            mov     r0.b1, 1
            sbco    ipOpt, cInstBase, r0.w2, b1
            jmp     phpAhScTxProc_1_ipv6Ext_opt_4
            
phpAhScTxProc_1_ipv6Ext_opt_1:   
#ifdef TO_BE_DELETE // It is the same as other options          
        qbne phpAhScTxProc_1_ipv6Ext_opt_2, ipv6Opt.type, IPV6_OPT_HOP_BY_HOP_OPT_PADN
            mov     r0.b1, ipv6Opt.len
            sbco    ipOpt, cInstBase, r0.w2, b1
            jmp     phpAhScTxProc_1_ipv6Ext_opt_4
#endif            
            
phpAhScTxProc_1_ipv6Ext_opt_2:
        // Common processing for non-special case
        // option length range check
        qbgt    phpAhScTxProc_1_Err_InvalidIP, r0.b0,       2
        add     r0.b1,  ipv6Opt.len,    2
        qblt    phpAhScTxProc_1_Err_InvalidIP, r0.b1, r0.b0
            
        // store the original options
        sbco    ipv6Opt,  cInstBase, r0.w2, b1
        
        //general options
        qbbc    phpAhScTxProc_1_ipv6Ext_opt_4, ipv6Opt.type.ipv6_opt_flag_mutable
        
phpAhScTxProc_1_ipv6Ext_opt_3:  
            // it is a mutable option, zero out the entire option except the header
            zero &ipv6Opt.data1, 32 - OFFSET(ipv6Opt.data1)
            xout XID_CDEDATA, ipv6Opt, b1
            // pass through 
           
phpAhScTxProc_1_ipv6Ext_opt_4:  
        sub     r0.b0, r0.b0, r0.b1
        add     r0.w2, r0.w2, r0.b1
        
        // advance over this option
        mov     CdeCmd.Cmd0.w2, r0.b1
        xout    XID_CDECTRL, CdeCmd, 4 
        
        qbeq    phpAhScTxProc_1_ipv6ExtRt_end, r0.b0, 0
            jmp     phpAhScTxProc_1_ipv6Ext_opt_loop
    
    
    .leave  ahIpScope
    
 phpAhScTxProc_1_FirstChunk_IP_End:
    add     ahTxPass1Loc1.pktOffset,  ahPktInst.ipOffset, ahPktInst.ipHdrSize  

    // TBD: Advance to the AH header 
    // To be enhanced with option operation
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, 20
    xout    XID_CDECTRL, CdeCmd, 4      

phpAhScTxProc_1_FirstChunk_IP_End_1:

    add     ahPktInst.icvOffset, ahTxPass1Loc1.pktOffset, IPSEC_AH_BASIC_SIZE_BYTES

    .using  ahHdrScope
    xin     XID_CDEDATA, ahHdr,  SIZE(ahHdr)
    
    mov     ahHdr.spi, ctxAHTx1.spi
    mov     ahHdr.seqNum,  ctxAHTx1.esnLo
    xout    XID_CDEDATA, ahHdr,  IPSEC_AH_BASIC_SIZE_BYTES

    qbeq    phpAhScTxProc_1_FirstChunk_L4, ctxAHTx1.ivSize, 0
    //
    // GMAC is the only AH encryption mode which needs 8-byte unique IV
    //
    mov     ahHdr.authData1, ctxAHTx1.esnHi
    mov     ahHdr.authData2, ctxAHTx1.esnLo
    xout    XID_CDEDATA, ahHdr.authData1, 8
    
    // adjust the icv Offset
    add     ahPktInst.icvOffset, ahPktInst.icvOffset, ctxAHTx1.ivSize
    
    // should we clear ICV data here?
    
 phpAhScTxProc_1_FirstChunk_L4:
    
    .leave ahHdrScope
    
    //
    //  Command Label Processing
    //
    lbco    r0.w2,  cAhTxTbl, ctxAHTx1.opMode, 2    // load the processing function from the call table
    call    r0.w2                                   // call table function will return to Error or MainLoop         
    
    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, ahTxPass1Loc1.cmdlOffset 
    mov     CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
 phpAhScTxProc_1_EsnProc_L1:
    qbbc    phpAhScTxProc_1_End, ctxAHTx1.ctrlInfo.flagAHTxESN
        
 phpAhScTxProc_1_EsnProc_L2:
    // ESN Data should be at this data chunk 
    // Advance to the ESN location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, ahPktInst.esnOffset, ahTxPass1Loc1.pktOffset
    xout    XID_CDECTRL, CdeCmd, 4      
    
    // Inset the upper 32-bit ESN to the packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    mov     CdeCmd.Cmd1, ctxAHTx1.esnHi  
    xout    XID_CDECTRL, CdeCmd, 8 
    
 phpAhScTxProc_1_End:
    // Save Instance Data
    sbco    ahPktInst, cInstBase, phpContext.instOffset, OFFSET(struct_ahPktInst.esnOffset_pass2)
    
    
 phpAhScTxProc_1_End_L1:
    
    //close the security context
   mov     CdeCmd.Cmd0, SCV_CMD_DONE
   xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
   // Send this packet (set valid PsInfo to zero)
   mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO <<8)
   xout    XID_CDECTRL, CdeCmd, 4                  // Send the command  
   
   jmp     MainLoop
   
.using pktDescScope
   
 
 phpAhScTxProc_1_Err_InvalidIP:
 
    // Move to the end of packet to ensure that packet descriptor section is available for 
    // updating
    mov     CdeCmd.Cmd0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4 
    
    // Update System Statistics
    lbco    r0, cStats, PDSP_ERR_PROTO_OFFSET, 4
    add     r0, r0, 1
    sbco    r0, cStats, PDSP_ERR_PROTO_OFFSET, 4
    
 phpAhScTxProc_1_Err_Common:
    // 
    // Update the packet descriptor to drop the packet
    // EngId = Output1
    // ThreadId = 7
    // Command label removed  (may be not necessary since it is dropped
    // Set Drop bit 
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket
    
    lbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SwInfo1_ctxPtr_w0)
    mov     PktDesc.SA_NextEngId,   SAENG_OUT1_1
    mov     PktDesc.SA_CmdLblInfo,  ctxAHTx1.cmdlTotalLen
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    set     PktDesc.ChunkInfo.chunkFlag_discard
    sbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SrcID)
    
    // Free the instance buffer
    clr     phpContext.instMask, PktDesc.PktId
 
    jmp    phpAhScTxProc_1_End_L1     

// Entry point for processing ESN overflow drop packet operations
phpAhScTxProc_1_Err_Esn_Overflow:
    wbs     r31.tStatus_CDEOutPacket
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAHTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxAHTx1, SIZE(ctxAHTx1)
    
    lbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SwInfo1_ctxPtr_w0)
    mov     PktDesc.SA_NextEngId,   SAENG_OUT1_1
    mov     PktDesc.SA_CmdLblInfo,  ctxAHTx1.cmdlTotalLen
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
 
    jmp    phpAhScTxProc_1_End_L1     

.leave pktDescScope

.leave ahTxPass1Scope
.leave ctxAHTxScope
.leave commCtxScope    

// ******************************************************************************
// * Function:    sap_ipsec_ah_tx_proc_pass2
// ******************************************************************************
// * Description: This function perfroms the second pass of the IPSec AH Tx packet
// *              header processing as the followings:  
// *
// *      - Insert the IV and hash vaule in the AH header 
// *      - Restore the IP Header (TOC, TTL, and Checksum)
// *	        IPV4: TOS, Flags, Fragment Offset and TTL
// *	        IPV6: Traffic Class, Flow Label and Hop Limit
// *	    - Restore the mutable option fields
// *	    - Replace the Protocol (next header) field in the IP header with the one 
// *        extracted from AH ??
// * 	    - Update Statistics
// *
// *****************************************************************************/
//
.struct struct_ahTxPass2Loc1
    .u8     dataChunkOffset     // data chunk offset
    .u8     pktId   
    .u16    psCtrlSize          // PS Info + Control Info Size
    .u16    chunkSize           // adjusted chunk size due to byte insertion
.ends     

.enter ahTxPass2Scope
    .assign  struct_ahTxPass2Loc1,  R1,  R2.w2,  ahTxPass2Loc1
    .assign  struct_ahPktInst,      R22, R26, ahPktInst      
.leave ahTxPass2Scope

phpAhScTxProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using ahTxPass2Scope
    
    // clear Local Variables (may be just some of them)
    // zero    &ahTxPass2Loc1,    SIZE(ahTxPass2Loc1)    
    
    //qbbc    phpAhScTxProc_2_L1,   phpContext.chunkInfo.chunkFlag_eop  
    
phpAhScTxProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAHTx.esnLo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    .using  ctxAHTxScope
    xin     XID_SCVDATA, ctxAHTx1, SIZE(ctxAHTx1)

    // Record index and load packet instance
    mov     ahTxPass2Loc1.pktId,  PktDesc.PktId
    lbco    ahPktInst, cInstBase,  phpContext.instOffset, OFFSET(struct_ahPktInst.esnOffset_pass2)
    
 phpAhScTxProc_2_L2:    
    mov     PktDesc.SA_NextEngId, SAENG_OUT1_1
    //xout    XID_CDEDATA, PktDesc, SIZE(PktDesc)
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2
    add     ahTxPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize    

    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
 //
 // Hash Processing
 //   
 phpAhScTxProc_2_HashProc_L1:
 
    //
    // Hash data is available in the PS Info section
    //
    // Advance to the PS Info section
    // mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    // xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and store the Hash Data
    //mov     r0.b0, ctxAHTx1.icvSize
    //xin     XID_CDEDATA,  r6,  b0
    
 phpAhScTxProc_2_HashProc_End:
    // Advance the window to the packet
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    //xout    XID_CDECTRL, CdeCmd, 4
 
    // Process SOP Packet
    // Move to the ICV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, ahPktInst.icvOffset, ahTxPass2Loc1.psCtrlSize
    add     CdeCmd.Cmd0.w2, CdeCmd.Cmd0.w2, 32     
    xout    XID_CDECTRL, CdeCmd, 4  
    mov     ahTxPass2Loc1.dataChunkOffset, ahPktInst.icvOffset
    
    // Move to save a few stalls
    mov     CdeCmd.Cmd0.b0, CDE_CMD_PATCH_PACKET_BUFFER
    mov     CdeCmd.Cmd0.b1, ahPktInst.ipHdrSize 
    mov     CdeCmd.Cmd0.w2, ahPktInst.ipOffset
    mov     CdeCmd.Cmd1, PHP_INST_BASE_ADDR
    add     CdeCmd.Cmd1, CdeCmd.Cmd1, phpContext.instOffset
 
    // Replace the Hash Data 
    //xout    XID_CDEDATA,  r6, b0  
        
 phpAhScTxProc_2_L3: 
    // Issue patch command to copy the original IP
    // Move to save a few stalls
    //mov     CdeCmd.Cmd0.b0, CDE_CMD_PATCH_PACKET_BUFFER
    //mov     CdeCmd.Cmd0.b1, ahPktInst.ipHdrSize 
    //mov     CdeCmd.Cmd0.w2, ahPktInst.ipOffset
    //mov     CdeCmd.Cmd1, PHP_INST_BASE_ADDR
    //add     CdeCmd.Cmd1, CdeCmd.Cmd1, phpContext.instOffset
    add     CdeCmd.Cmd1, CdeCmd.Cmd1, AH_PKT_INST_IPBUF_OFFSET 
    xout    XID_CDECTRL, CdeCmd, 8
    
 phpAhScTxProc_2_L4:   
    qbeq    phpAhScTxProc_2_L5, ctxAHTx1.icvSize,   0
    qbbc    phpAhScTxProc_2_L5, ctxAHTx1.ctrlInfo.flagAHTxESN
    
    // Move to ESN and Flash it
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, ahPktInst.esnOffset, ahTxPass2Loc1.dataChunkOffset
    xout    XID_CDECTRL, CdeCmd, 4  
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 4
    xout    XID_CDECTRL, CdeCmd, 4
   
 phpAhScTxProc_2_L5:
    //
    //  Process EOP Data Chunk
    //
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4
    .using ctxAHTxScopeStats
    xin     XID_SCVDATA, ctxAHTxStats, SIZE(ctxAHTxStats)
    add     ctxAHTxStats.numTxPkts,  ctxAHTxStats.numTxPkts, 1
    adc     ctxAHTxStats.numTxPktsHi, ctxAHTxStats.numTxPktsHi, 0
    clr     phpContext.instMask, ahTxPass2Loc1.pktId
    xout    XID_SCVDATA, ctxAHTxStats, SIZE(ctxAHTxStats)
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_AH_ENC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_AH_ENC_HI_OFFSET, 8
    
 phpAhScTxProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8)
    mov     CdeCmd.Cmd0.b3, 16
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop

.leave pktDescScope
.leave ahTxPass2Scope
.leave ctxAHTxScope
.leave ctxAHTxScopeStats
.leave commCtxScope    
 
        
// Register MAP
// R0, R29       : General and local register
// R6-R8         : rtp header (from packet)
// R14-R21       : IPSEC Security Context including all parametes required
// R10-R13       : output iv 
// R30           : return address

.struct struct_ahAesCtr
    .u32  salt
    .u32  iv1
    .u32  iv2
    .u32  cnt
.ends

.enter ahTxCmdlScope
    .assign  struct_saCmdL,            R12, R13, saCmdL
    .assign  struct_saCmdLReg,         R12, R13, saCmdLReg
.leave ahTxCmdlScope 

.enter ahAesCtrScope
    .assign  struct_ahAesCtr,          R8, R11,  saAhAesCtr 
.leave ahAesCtrScope

 .using ahTxPass1Scope
 .using ctxAHTxScope
 .using ahHdrScope
 .using ahTxCmdlScope
 .using ahAesCtrScope
 
phpAhTxCmdlProc_Gen:

    // Construct command Label in AH General mode
    zero    &saCmdL, SIZE(saCmdL)
    
    // Format the encryption command label
    mov     saCmdL.nextEngId, SAENG_PHP_2
    mov     saCmdL.procLen, ahTxPass1Loc1.hashDataLen
    mov     saCmdL.sopOffset, ahPktInst.ipOffset
    
    qbbc    phpAhTxCmdlProc_Gen_L3, ctxAHTx1.ctrlInfo.flagAHTxCMAC
    // Process CMAC 
    mov     saCmdL.cmdlLen, SA_CMAC_CMDL_SIZE
    //mov     saCmdL.optctrl1, SA_CMDL_MK_OPT_CTRL(SA_ENC_AUX1_OFFSET, 16)
    mov     saCmdL.optctrl1, SA_ENC_AUX1_OFFSET + (16 >> 3)
    sbco    saCmdL, cCmdlBuf, ahTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     ahTxPass1Loc1.cmdlOffset, ahTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Move the security context viewer to the K1/K2 location 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxAHTx.aux1)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4   
    xin     XID_SCVDATA,  r6,  32
    
    and     r0.w0, ahTxPass1Loc1.hashDataLen, 0x000f
    qbne    phpAhTxCmdlProc_Gen_L1, r0.w0, 0
    // K1 is used
    sbco    r6, cCmdlBuf, ahTxPass1Loc1.cmdlOffset, 16
    jmp     phpAhTxCmdlProc_Gen_L2
    
phpAhTxCmdlProc_Gen_L1:
    // k2 is used
    sbco    r10, cCmdlBuf, ahTxPass1Loc1.cmdlOffset, 16
    
phpAhTxCmdlProc_Gen_L2:
    add     ahTxPass1Loc1.cmdlOffset, ahTxPass1Loc1.cmdlOffset, 16
    ret     
    
phpAhTxCmdlProc_Gen_L3:    
    mov     saCmdL.cmdlLen, SIZE(saCmdL)
    sbco    saCmdL, cCmdlBuf, ahTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     ahTxPass1Loc1.cmdlOffset, ahTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
phpAhTxCmdlProc_Gen_End:
    ret       
    
phpAhTxCmdlProc_GMAC:
    // Construct command Label in AH GMAC Mode
    // Construct and copy basic label
    mov     saCmdLReg.reg1.w2,  SA_GMAC_CMDL_SIZE_AH + (SAENG_PHP_2 << 8)
    mov     saCmdL.procLen, ahTxPass1Loc1.hashDataLen
    mov     saCmdL.sopOffset, ahPktInst.ipOffset
    mov     saCmdL.optctrl1, SA_GMAC_CMDL_OPT1
    mov     saCmdLReg.reg2.w0,  (SA_GMAC_CMDL_OPT3 << 8)
    sbco    saCmdL, cCmdlBuf, ahTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     ahTxPass1Loc1.cmdlOffset, ahTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construst and copy Aux1
    zero    &saCmdL, SIZE(saCmdL)
    lsl     saCmdLReg.reg2, ahTxPass1Loc1.hashDataLen, 3
    sbco    saCmdL, cCmdlBuf, ahTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     ahTxPass1Loc1.cmdlOffset, ahTxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
   // qbbs    phpAhTxCmdlProc_GMAC_L1, ctxAHTx1.ctrlInfo.flagAHTxESN
        
    // Construct and copy AES CTR 
    mov     saAhAesCtr.salt,  ctxAHTx1.salt
    mov     saAhAesCtr.cnt,   1  
    sbco    saAhAesCtr, cCmdlBuf, ahTxPass1Loc1.cmdlOffset, SIZE(saAhAesCtr)
    add     ahTxPass1Loc1.cmdlOffset, ahTxPass1Loc1.cmdlOffset, SIZE(saAhAesCtr)
    ret
    
 .leave ahTxPass1Scope
 .leave ctxAHTxScope
 .leave ahHdrScope
 .leave ahTxCmdlScope
 .leave ahAesCtrScope
