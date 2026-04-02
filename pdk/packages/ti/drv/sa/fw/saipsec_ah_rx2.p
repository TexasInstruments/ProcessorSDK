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
// * FILE PURPOSE:  PA firmware for PDSP: IPSEC AH Rx Processing
// **********************************************************************************
// * FILE NAME:  saipsec_ah_rx.p
// *
// * DESCRIPTION: rocess the IPSEC AH Rx (From-Network) packets
// *
// **********************************************************************************/
// /***************************************************************************
// * FILE PURPOSE: SA HPS - IPSec AH mode handling
// ****************************************************************************
// * FILE NAME: saipsec_ah.c
// *
// * DESCRIPTION:
// *  This file contains functions for SA packet header processing of the IPSec 
// *  AH packets
// *
// *
// *     0                   1                   2                   3
// *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// *   | Next Header   |  Payload Len  |          RESERVED             |
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// *   |                 Security Parameters Index (SPI)               |
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// *   |                    Sequence Number Field                      |
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// *   |                                                               |
// *   +                Integrity Check Value-ICV (variable)           |
// *   |                                                               |
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// *
// *                   Figure 1.  AH Format
// *
// *
// *
// *                   AFTER APPLYING AH
// *             -------------------------------------------------------
// *       IPv4  |original IP hdr (any options) | AH | TCP |    Data   |
// *             -------------------------------------------------------
// *             |<- mutable field processing ->|<- immutable fields ->|
// *             |<----- authenticated except for mutable fields ----->|
// *
// *
// *                       AFTER APPLYING AH
// *            ------------------------------------------------------------
// *      IPv6  |             |hop-by-hop, dest*, |    | dest |     |      |
// *            |orig IP hdr  |routing, fragment. | AH | opt* | TCP | Data |
// *            ------------------------------------------------------------
// *            |<--- mutable field processing -->|<-- immutable fields -->|
// *            |<---- authenticated except for mutable fields ----------->|
// *
// *
// *                 Figure 2. AH Transport Mode
// *
// *
// *        ----------------------------------------------------------------
// *   IPv4 |                              |    | orig IP hdr*  |   |      |
// *        |new IP header * (any options) | AH | (any options) |TCP| Data |
// *        ----------------------------------------------------------------
// *        |<- mutable field processing ->|<------ immutable fields ----->|
// *        |<- authenticated except for mutable fields in the new IP hdr->|
// *
// *        --------------------------------------------------------------
// *   IPv6 |           | ext hdrs*|    |            | ext hdrs*|   |    |
// *        |new IP hdr*|if present| AH |orig IP hdr*|if present|TCP|Data|
// *        --------------------------------------------------------------
// *        |<--- mutable field -->|<--------- immutable fields -------->|
// *        |       processing     |
// *        |<-- authenticated except for mutable fields in new IP hdr ->|
// *
// *          * = if present, construction of outer IP hdr/extensions and
// *              modification of inner IP hdr/extensions is discussed in
// *              the Security Architecture document.
// *
// *                Figure 3. AH Tunnel mode
// *
// *
// * (C) Copyright 2008 Texas Instruments, Inc.
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
// *  R8       |  Note: Can be used as general registers and context viewer when
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
// *  R26                  
// *  R27    |    PHP Context    
// *  R28    |          
// *  R29     |   Function arg and return values 
// *  R30      |  Return addresses (2 levels)
// *  R31    |  I/O Flags
// *
// **********************************************************************************

// *******************************************************************************
// * Local Data Structures
// ******************************************************************************/
//

.struct struct_ahRxPass1Loc1
    .u16    hashDataLen        // Hash Data length (for command label)
    .u8     ahHdrSize          // AH Header Size (for command label)
    .u8     ipVer              // IP Version 
    .u8     ctrlInfo           // local copy of Context ctrlInfo
    .u8     pktOffset          // Current Offset of the packet
    .u8     cmdlOffset         // Command Label Offset 
    .u8     cmdlIVOffset       // IV offset in the command label (for copy data)
.ends   

.enter ahRxPass1Scope
    .assign  struct_ahRxPass1Loc1,  R2,  R3, ahRxPass1Loc1
    .assign  struct_ahPktInst, R22, R26,  ahPktInst      
.leave ahRxPass1Scope

.enter ctxAHRxScope
    .assign  struct_ctxAHRxStats,        R14, R20, ctxAHRxStats
    .assign  struct_ctxAHRxStatsReplay,  R6,  R12, ctxAHRxStatsReplay    
    .assign  struct_ctxAHRx1,            R14, R17.w2, ctxAHRx1
.leave ctxAHRxScope

// ******************************************************************************
// * Function:    sap_ipsec_ah_tx_proc_pass1
// ******************************************************************************
// * Description: This function perfroms the first pass of the IPSec AH Rx packet
// *              header processing as the followings: 
// * 
// *  - Extract the following information from the IP header and replace them with zero
// *      IPV4: TOS, Flags, Fragment Offset and TTL
// * 	    IPV6: Traffic Class, Flow Label and Hop Limit
// *  - Zero out IPV4 header checksum
// *  - Record and zero out the mutable option fields
// *  - Fill in the AH header parameters(SPI and Sequence Number)
// *  - Extract and clear IV in the AH header if exists
// * 	- Clear the HASH Value in the AH header 
// * 	- Insert the Authentication Command Label with data length and offset
// *	- Insert the HPS2 command label
// *  - Insert the upper 32-bit of ESN at the end of payload if configured
// *
// *  Where the packet info consists of
// *        - pointer to the packet
// *        - packet length
// *        - offset to the (outer) IP header 
// *
// *****************************************************************************/
//

phpAhScRxProc_1:

    // Error Check
    
    
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using ahRxPass1Scope
    .using ctxAHRxScope
    
    // Advance the SC Viewer the general portion of IPSec Rx security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAHRx.ctrlInfo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4  
          
    xin     XID_SCVDATA, ctxAHRx1, SIZE(ctxAHRx1)
    
    // clear Local Variables (may be just some of them)
    // need for cmdlOffset
    zero    &ahRxPass1Loc1,    SIZE(ahRxPass1Loc1)    
    
 phpAhScRxProc_1_FirstChunk_L1:    
    //pkt Instance Initialization: clear the control flags and other variables
    //zero    &ahPktInst,    SIZE(ahPktInst)
    
 phpAhScRxProc_1_FirstChunk_L2:  
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, ctxAHRx1.firstEngId
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    //xout    XID_CDEDATA, PktDesc, SIZE(PktDesc) 
    xout    XID_CDEDATA, PktDesc, 8  
      
    // Advance the window past the Descriptor and PS Info (64 bytes)
    // Move is desc size + phys PsInfo size
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_CONTROL
    xout    XID_CDECTRL, CdeCmd, 4   
    
    .leave  pktDescScope
    .using protoInfoScope
    // Extract protocol Info
    xin     XID_CDEDATA, payloadInfoL, SIZE(struct_payloadInfoLong)
    
    // TBD: Issue command to flash out and copy PSInfo
    // Now we're at the control section. Insert the command label here
    // Note: we may want to keep the protocol command at the output packet
    // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    // mov     CdeCmd.Cmd0.w2, 4
    // xout    XID_CDECTRL, CdeCmd, 4      
    
    mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
    mov     CdeCmd.Cmd0.b1, ctxAHRx1.cmdlTotalLen
    xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
       
    // Advance to the IP header
    // Move to save up to 5 stalled cycles 
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4 
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    //add     CdeCmd.Cmd0.w2, payloadInfoL.l3Offset, payloadInfoL.recordLen
    mov     CdeCmd.Cmd0.w2, payloadInfoL.l3Offset
    xout    XID_CDECTRL, CdeCmd, 4 
       
    // Calculate the payload related information
    mov     ahPktInst.ipOffset,  payloadInfoL.l3Offset
    sub     ahRxPass1Loc1.hashDataLen, payloadInfoL.eopOffset, ahPktInst.ipOffset
    
    // Calculate and record Various Offsets
    mov     ahPktInst.esnOffset, payloadInfoL.eopOffset     
       
    // Update HashDataLen if ECN is enabled
    qbbc    phpAhScRxProc_1_FirstChunk_L3, ctxAHRx1.ctrlInfo.flagAHRxESN
    add     ahRxPass1Loc1.hashDataLen, ahRxPass1Loc1.hashDataLen, 4  

  phpAhScRxProc_1_FirstChunk_L3:
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAHRx.numRxPkts)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4
    xin     XID_SCVDATA, ctxAHRxStats, SIZE(ctxAHRxStats)
    add     ctxAHRxStats.byteCount, ctxAHRxStats.byteCount, ahRxPass1Loc1.hashDataLen
    adc     ctxAHRxStats.byteCountHi, ctxAHRxStats.byteCountHi, 0
    xout    XID_SCVDATA, ctxAHRxStats, SIZE(ctxAHRxStats)

    // Restore ctxAHRx1
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxAHRx.ctrlInfo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4  
          
    xin     XID_SCVDATA, ctxAHRx1, SIZE(ctxAHRx1)
    // Advance to the IP header
    // Move to save some stalled cycles 
    // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    // add     CdeCmd.Cmd0.w2, ahPktInst.ipOffset, payloadInfoL.recordLen
    // xout    XID_CDECTRL, CdeCmd, 4 
  
    .leave protoInfoScope
    
    // Load and processing IP header
    .using ahIpScope
    // Load the first 20 bytes of IP and store them in the instance buffer
    add     r0.w2, phpContext.instOffset, AH_PKT_INST_IPBUF_OFFSET 
    xin     XID_CDEDATA, ip4Hdr, SIZE(ip4Hdr)
    sbco    ip4Hdr, cInstBase, r0.w2, SIZE(ip4Hdr)
    
    and     ahRxPass1Loc1.ipVer, ip4Hdr.VerLen, IPV4_VER_MASK
    qbeq    phpAhScRxProc_1_FirstChunk_IP4, ahRxPass1Loc1.ipVer, IP_TYPE_IPV4
    qbeq    phpAhScRxProc_1_FirstChunk_IP6, ahRxPass1Loc1.ipVer, IP_TYPE_IPV6
    jmp     phpAhScRxProc_1_Err_InvalidIP
    
 phpAhScRxProc_1_FirstChunk_IP4:
    // clear TOS, Flags and Frago, TTL and checksum
    mov     ip4Hdr.Tos, 0
    zero    &ip4Hdr.FragOff, 3
    mov     ip4Hdr.Checksum, 0
    xout    XID_CDEDATA, ip4Hdr, SIZE(ip4Hdr) 
    and     r0.b0,  ip4Hdr.VerLen, IPV4_LEN_MASK
    lsl     ahPktInst.ipHdrSize,  r0.b0, 2 
    
    // option processing
    qbeq    phpAhScRxProc_1_FirstChunk_IP_End, ahPktInst.ipHdrSize, SIZE(ip4Hdr)
    qbgt    phpAhScRxProc_1_Err_InvalidIP, ahPktInst.ipHdrSize, SIZE(ip4Hdr)
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
        add     ahRxPass1Loc1.pktOffset,  ahPktInst.ipOffset, ahPktInst.ipHdrSize  
        
        // load mutable bitmap
        mov32    r29, IPV4_OPT_MUTABILITY_BITMAP
        
phpAhScRxProc_1_ipOpt_loop:

        // End of option check
        qbne phpAhScRxProc_1_ipOpt_1, ipOpt.opt, IPV4_OPT_END_OF_LIST
            // End of option
            // set the option length to the remaining length
            mov     r0.b1, r0.b0
            
            // store the remaining data to the IP buffer
            sbco    ipOpt, cInstBase, r0.w2, b1
            jmp     phpAhScRxProc_1_ipOpt_3
            
phpAhScRxProc_1_ipOpt_1:            
        qbne phpAhScRxProc_1_ipOpt_2, ipOpt.opt, IPV4_OPT_NOP
            mov     r0.b1, 1
            sbco    ipOpt, cInstBase, r0.w2, b1
            jmp     phpAhScRxProc_1_ipOpt_3
            
phpAhScRxProc_1_ipOpt_2:
        // Common processing for non-special case
        // option length range check
        qbgt    phpAhScRxProc_1_Err_InvalidIP, ipOpt.len, 2
        qblt    phpAhScRxProc_1_Err_InvalidIP, ipOpt.len, r0.b0
            
        // store the original options
        mov     r0.b1,  ipOpt.len
        sbco    ipOpt,  cInstBase, r0.w2, b1
        
        // extract the option value
        and     ipOpt.opt,  ipOpt.opt,  IPV4_OPT_MASK
        
        //other options
        qbbs    phpAhScRxProc_1_ipOpt_3, r29,  ipOpt.opt
            // it is a mutable option, zero out the entire option
            zero &ipOpt, 32
            xout XID_CDEDATA, ipOpt, b1
            // pass through 
           
phpAhScRxProc_1_ipOpt_3:  
        sub     r0.b0, r0.b0, r0.b1
        
        // advance over this option
        mov     CdeCmd.Cmd0.w2, r0.b1
        xout    XID_CDECTRL, CdeCmd, 4 
        
        qbeq    phpAhScRxProc_1_FirstChunk_IP_End_1, r0.b0, 0
            add     r0.w2, r0.w2, r0.b1
            xin     XID_CDEDATA, ipOpt, 32
            jmp     phpAhScRxProc_1_ipOpt_loop
    
 phpAhScRxProc_1_FirstChunk_IP6:
    // clear DSCP/TOS, ECN, Flow Label, and Hop Limit
    mov     r1.b3, ip6Hdr_1stHalf.NextHeader
    and     ip6Hdr_1stHalf.VerDsHi, ip6Hdr_1stHalf.VerDsHi, IPV6_VER_MASK
    zero    &ip6Hdr_1stHalf.DsHiFlowIDHi, 3
    mov     ip6Hdr_1stHalf.HopLimit, 0
    xout    XID_CDEDATA, ip6Hdr_1stHalf, SIZE(ip6Hdr_1stHalf)
    
    // Advance to the 2nd Half of IPv6  header
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, SIZE(ip6Hdr_1stHalf)
    xout    XID_CDECTRL, CdeCmd, 4      
 
    xin     XID_CDEDATA, ip6Hdr_2ndHalf, SIZE(ip6Hdr_2ndHalf)
    
    // Store the second half of IPv6 header
    add     r0.w2, r0.w2, SIZE(ip6Hdr_1stHalf) 
    sbco    ip6Hdr_2ndHalf, cInstBase, r0.w2, SIZE(ip6Hdr_2ndHalf)
    mov     ahPktInst.ipHdrSize, SIZE(ip6Hdr_1stHalf) + SIZE(ip6Hdr_2ndHalf)
    
    qbeq    phpAhScRxProc_1_FirstChunk_IP_End, r1.b3, IP_PROTO_NEXT_AUTH
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
        add     ahRxPass1Loc1.pktOffset,  ahPktInst.ipOffset, ahPktInst.ipHdrSize  
        
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
        
phpAhScRxProc_1_ipv6Ext_loop:        
        
        // Load the extension header
        xin     XID_CDEDATA, ipv6Ext, 32
        
        // extension hdaer length = (hdrLen + 1) * 8
        add     r0.b1, ipv6Ext.hdrLen, 1
        lsl     r0.b1, r0.b1, 3 
        
        // TBD: check for IP header buffer overflow
        
        qbeq    phpAhScRxProc_1_ipv6Ext_opt, r1.b3,  IP_PROTO_NEXT_IPV6_HOP_BY_HOP
        qbeq    phpAhScRxProc_1_ipv6Ext_opt, r1.b3,  IP_PROTO_NEXT_IPV6_DEST_OPT
        qbne    phpAhScRxProc_1_Err_InvalidIP, r1.b3, IP_PROTO_NEXT_IPV6_ROUTE
        
phpAhScRxProc_1_ipv6ExtRt:        
            // Ipv6 route option processing
            // No extra processing at the receive end
            // save the entire extension header
            mov     r1.b3, ipv6ExtRt.proto
            sbco    ipv6ExtRt,  cInstBase, r0.w2, b1
            add     r0.w2, r0.w2, r0.b1
            
            //advance over the RT header
            mov     CdeCmd.Cmd0.w2, r0.b1
            xout    XID_CDECTRL, CdeCmd, 4 
            
            add     ahRxPass1Loc1.pktOffset,  ahRxPass1Loc1.pktOffset, r0.b1
            //pass through
                
phpAhScRxProc_1_ipv6ExtRt_end:
                
        qbeq    phpAhScRxProc_1_FirstChunk_IP_End_1, r1.b3, IP_PROTO_NEXT_AUTH
            jmp     phpAhScRxProc_1_ipv6Ext_loop    
                       
        
phpAhScRxProc_1_ipv6Ext_opt:
        // walk through all option fields and zero out mutable options
        // r0.w2: IP buffer location
        // r0.b0: remaining option length
        // r0.b1: current option length (total option length at input)
        
        // save next header and adjust pktOffset for the next protocol
        mov     r1.b3,  ipv6Ext.proto
        add     ahRxPass1Loc1.pktOffset,  ahRxPass1Loc1.pktOffset, r0.b1
        
        // save the first portion of data
        sbco    ipv6Ext,  cInstBase, r0.w2, 2
        add     r0.w2,  r0.w2, 2
        
        // Advance to extension header option area
        // mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
        mov     CdeCmd.Cmd0.w2, 2
        xout    XID_CDECTRL, CdeCmd, 4 
        
        // Initialize the original option length
        sub     r0.b0, r0.b1, 2
        
phpAhScRxProc_1_ipv6Ext_opt_loop:
        // Load the option data
        xin     XID_CDEDATA, ipv6Opt, 32

        // End of option check
        qbne phpAhScRxProc_1_ipv6Ext_opt_1, ipv6Opt.type, IPV6_OPT_HOP_BY_HOP_OPT_PAD0
            // single-byte padding
            mov     r0.b1, 1
            sbco    ipOpt, cInstBase, r0.w2, b1
            jmp     phpAhScRxProc_1_ipv6Ext_opt_4
            
phpAhScRxProc_1_ipv6Ext_opt_1:   
#ifdef TO_BE_DELETE // It is the same as other options          
        qbne phpAhScRxProc_1_ipv6Ext_opt_2, ipv6Opt.type, IPV6_OPT_HOP_BY_HOP_OPT_PADN
            mov     r0.b1, ipv6Opt.len
            sbco    ipOpt, cInstBase, r0.w2, b1
            jmp     phpAhScRxProc_1_ipv6Ext_opt_4
#endif            
            
phpAhScRxProc_1_ipv6Ext_opt_2:
        // Common processing for non-special case
        // option length range check
        qbgt    phpAhScRxProc_1_Err_InvalidIP, r0.b0,       2
        add     r0.b1,  ipv6Opt.len,    2
        qblt    phpAhScRxProc_1_Err_InvalidIP, r0.b1, r0.b0
            
        // store the original options
        sbco    ipv6Opt,  cInstBase, r0.w2, b1
        
        //general options
        qbbc    phpAhScRxProc_1_ipv6Ext_opt_4, ipv6Opt.type.ipv6_opt_flag_mutable
        
phpAhScRxProc_1_ipv6Ext_opt_3:  
            // it is a mutable option, zero out the entire option except the header
            zero &ipv6Opt.data1, 32 - OFFSET(ipv6Opt.data1)
            xout XID_CDEDATA, ipv6Opt, b1
            // pass through 
           
phpAhScRxProc_1_ipv6Ext_opt_4:  
        sub     r0.b0, r0.b0, r0.b1
        add     r0.w2, r0.w2, r0.b1
        
        // advance over this option
        mov     CdeCmd.Cmd0.w2, r0.b1
        xout    XID_CDECTRL, CdeCmd, 4 
        
        qbeq    phpAhScRxProc_1_ipv6ExtRt_end, r0.b0, 0
            jmp     phpAhScRxProc_1_ipv6Ext_opt_loop
    
    .leave  ahIpScope
    
 phpAhScRxProc_1_FirstChunk_IP_End:
    add     ahRxPass1Loc1.pktOffset,  ahPktInst.ipOffset, ahPktInst.ipHdrSize  
                         
    // TBD: Advance to the AH header 
    // To be enhanced with option operation
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, 20
    xout    XID_CDECTRL, CdeCmd, 4      
    
phpAhScRxProc_1_FirstChunk_IP_End_1:    
    add     ahPktInst.icvOffset, ahRxPass1Loc1.pktOffset, IPSEC_AH_BASIC_SIZE_BYTES
    
    .using ahHdrScope
    xin     XID_CDEDATA, ahHdr, SIZE(ahHdr)
    
    // TBD: we may not need to verify SPI against the one at the security context
    //mov     ahRxPass1Loc2.spi, ahHdr.spi
    mov     ahPktInst.esnLo, ahHdr.seqNum
    
    //
    // Command Processing
    //
    lbco    r0.w2,  cAhRxTbl, ctxAHRx1.opMode, 2    // load the processing function from the call table
    call    r0.w2                                   
    
    qbeq    phpAhScRxProc_1_FirstChunk_L5, ctxAHRx1.ivSize, 0
    
    // Advance the ivSize so that the authData1 appear at the nornal location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, ctxAHRx1.ivSize
    xout    XID_CDECTRL, CdeCmd, 4      
    
    // adjust the offset
    add     ahRxPass1Loc1.pktOffset, ahRxPass1Loc1.pktOffset, ctxAHRx1.ivSize
    add     ahPktInst.icvOffset, ahPktInst.icvOffset, ctxAHRx1.ivSize
    
phpAhScRxProc_1_FirstChunk_L5:
    qbeq    phpAhScRxProc_1_FirstChunk_L6, ctxAHRx1.icvSize, 0
    // Store and zero out ICV field
    xin     XID_CDEDATA, ahHdr, SIZE(ahHdr)
    add     r0.w2, phpContext.instOffset, AH_PKT_INST_HASH_DATA_OFFSET 
    mov     r0.b0, ctxAHRx1.icvSize
    sbco    ahHdr.authData1, cInstBase, r0.w2, b0
    
    zero    &ahHdr.authData1, 16
    xout    XID_CDEDATA, ahHdr.authData1, b0
    
    .leave ahHdrScope
    
phpAhScRxProc_1_FirstChunk_L6:
    
    // Perform ESN update if configured
    qbbc   phpAhScRxProc_1_FirstChunk_L8, ctxAHRx1.ctrlInfo.flagAHRxReplay
    qbbc   phpAhScRxProc_1_FirstChunk_L8, ctxAHRx1.ctrlInfo.flagAHRxESN
    
    //
    // ESN update
    //
    // Advance the SC Viewer the first part Rx security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAHRx.winBaseHi))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxAHRxStatsReplay, SIZE(ctxAHRxStatsReplay)
    
    // Do the subract first so we are immune to 32-bit wrap R0 = seq_num_diff
    sub     r0, ahPktInst.esnLo,  ctxAHRxStatsReplay.winBase
    
    // If the delta is "negative", then this packet comes before our base.
    // It is equivalent to (seq_num_diff > 2^31)
    // the receive packet is well outside the replay windom, drop it
    qbbs  phpAhScRxProc_1_Err_OldPkt,  r0.t31 
    
    qbgt  phpAhScRxProc_1_FirstChunk_L7,  ahPktInst.esnLo,  ctxAHRxStatsReplay.winBase
    mov   ahPktInst.esnHi, ctxAHRxStatsReplay.winBaseHi
    jmp   phpAhScRxProc_1_EsnProc_L2
    
phpAhScRxProc_1_FirstChunk_L7: 
    add   ahPktInst.esnHi, ctxAHRxStatsReplay.winBaseHi, 1
    
phpAhScRxProc_1_EsnProc_L2:
    // ESN Data should be at this data chunk 
    // Advance to the ESN location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    //sub     CdeCmd.Cmd0.w2, ahRxPass1Loc1.esnOffset, ahRxPass1Loc1.pktOffset
    sub     CdeCmd.Cmd0.w2, ahPktInst.esnOffset, ahRxPass1Loc1.pktOffset
    xout    XID_CDECTRL, CdeCmd, 4      
    
    // Inset the upper 32-bit ESN to the packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    mov     CdeCmd.Cmd1, ahPktInst.esnHi  
    xout    XID_CDECTRL, CdeCmd, 8 
    
phpAhScRxProc_1_FirstChunk_L8:
    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, ahRxPass1Loc1.cmdlOffset 
    mov     CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
phpAhScRxProc_1_End:
    // Save Instance Data
    sbco    ahPktInst, cInstBase, phpContext.instOffset, OFFSET(struct_ahPktInst.nextEngId)
    
phpAhScRxProc_1_End_L1:  
    //close the security context
   mov     CdeCmd.Cmd0, SCV_CMD_DONE
   xout    XID_SCVCTRL, CdeCmd.Cmd0, 4

   // Send this packet (set valid PsInfo to zero)
   mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8) 
   xout    XID_CDECTRL, CdeCmd, 4                  // Send the command    
   jmp     MainLoop
   
.using  pktDescScope
 
 phpAhScRxProc_1_Err_OldPkt:
    // Updates the statistics
    add     ctxAHRxStatsReplay.numOldPkts, ctxAHRxStatsReplay.numOldPkts, 1
    xout    XID_SCVDATA, ctxAHRxStatsReplay, SIZE(ctxAHRxStatsReplay)
    // Update System Statistics
    lbco    r0, cStats, PDSP_AH_REPLAY_OLD_OFFSET, 4
    add     r0, r0, 1
    sbco    r0, cStats, PDSP_AH_REPLAY_OLD_OFFSET, 4
    
    // Advance the SC Viewer the general portion of IPSec Rx security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAHRx.ctrlInfo)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4  
    xin     XID_SCVDATA, ctxAHRx1, SIZE(ctxAHRx1)
    
    jmp     phpAhScRxProc_1_Err_Common  
     
 phpAhScRxProc_1_Err_InvalidIP:
    // Move to the end of packet to ensure that packet descriptor section is available for 
    // updating
    mov     CdeCmd.Cmd0, CDE_CMD_FLUSH_TO_END
    xout    XID_CDECTRL, CdeCmd, 4 
    
    // Update System Statistics
    lbco    r0, cStats, PDSP_ERR_PROTO_OFFSET, 4
    add     r0, r0, 1
    sbco    r0, cStats, PDSP_ERR_PROTO_OFFSET, 4
    
    
 phpAhScRxProc_1_Err_Common:
    // 
    // Update the packet descriptor to drop the packet
    // EngId = Output1
    // ThreadId = 7
    // Command label removed (may not be necessary if pkt dropped 
    // Set Drop bit 
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket
    
    lbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SwInfo1_ctxPtr_w0)
    mov     PktDesc.SA_NextEngId,   SAENG_OUT1_1
    mov     PktDesc.SA_CmdLblInfo,  ctxAHRx1.cmdlTotalLen
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    set     PktDesc.ChunkInfo.chunkFlag_discard
    sbco    PktDesc, cCdeOutPkt, 0, OFFSET(struct_PktDesc.SrcID)
    
    // Free the instance buffer
    clr     phpContext.instMask, PktDesc.PktId
    
    jmp    phpAhScRxProc_1_End_L1
    
    
.leave  pktDescScope
    
.leave ahRxPass1Scope
.leave ctxAHRxScope
.leave commCtxScope    

// ****************************************************************************** 
// * Function:    sap_ipsec_ah_rx_proc_pass2 
// ****************************************************************************** 
// * Description: This function perfroms the second pass of the IPSec AH Rx packet 
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
.struct struct_ahRxPass2Loc1
    .u8     ctrlInfo               // Record Ctx content for local use  
    .u8     icvSize
    .u8     psCtrlSize             // PS Info + Control Info Size
    .u8     pktId
    .u16    dataChunkOffset        // data chunk offset
    .u16    chunkSize              // adjusted chunk size due to byte insertion
.ends     

.enter ahRxPass2Scope
    .assign  struct_ahRxPass2Loc1,  R2,  R3,  ahRxPass2Loc1
    .assign  struct_ahPktInst,      R22, R26, ahPktInst      
.leave ahRxPass2Scope

phpAhScRxProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using ahRxPass2Scope
    .using ctxAHRxScope
    
    // clear Local Variables (may be just some of them)
    // zero    &ahRxPass2Loc1,    SIZE(ahRxPass2Loc1)    
    
phpAhScRxProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAHRx.ctrlInfo))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxAHRx1, SIZE(ctxAHRx1)
    
    // Record some parameters for loacl usage
    mov     ahRxPass2Loc1.ctrlInfo, ctxAHRx1.ctrlInfo

    // reord packet instance index and load the instance
    mov     ahRxPass2Loc1.pktId,  PktDesc.PktId
    lbco    ahPktInst, cInstBase,  phpContext.instOffset, OFFSET(struct_ahPktInst.nextEngId)
    
 phpAhScRxProc_2_L2:   
    mov     PktDesc.SA_NextEngId, SAENG_OUT1_1
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2
    // set swInfo1 at SOP data chunk
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
    add     ahRxPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize    
    
 //
 // Hash Processing
 //   
 phpAhScRxProc_2_HashProc_L1:
 
    .leave pktDescScope
    
    qbeq   phpAhScRxProc_2_Replay_L1,  ctxAHRx1.icvSize, 0  
    
    //
    // Hash data is available in the PS Info section
    //
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and compare the Hash Data
    mov     r0.b0, ctxAHRx1.icvSize
    xin     XID_CDEDATA,  r6,  b0
    
 phpAhScRxProc_2_HashProc_L2:
    // Advance the window to the packet
    //mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    //xout    XID_CDECTRL, CdeCmd, 4
    
    // Move to the ICV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    add     CdeCmd.Cmd0.w2, ahPktInst.icvOffset, ahRxPass2Loc1.psCtrlSize
    xout    XID_CDECTRL, CdeCmd, 4  
    
    //
    //  AH Hash Comparing
    //
    add     r0.w2, phpContext.instOffset, AH_PKT_INST_HASH_DATA_OFFSET
    lbco    r10, cInstBase, r0.w2, b0 
    
    // Use the hash comparison routine here 
    //mov     r29.b0, ctxAHRx1.icvSize
    //call    IPSEC_ICVCheck
    //qbeq    phpAhScRxProc_2_Err_Hash, r29.b0, 0
 //IPSEC_ICVCheck:
  //  qbne    phpAhScRxProc_2_Err_Hash, r6, r10
  //  qbeq    phpAhScRxProc_2_HashProc_L3, ctxAHRx1.icvSize, 4
  //  qbne    phpAhScRxProc_2_Err_Hash, r7, r11
  //  qbeq    phpAhScRxProc_2_HashProc_L3, ctxAHRx1.icvSize, 8
  //  qbne    phpAhScRxProc_2_Err_Hash, r8, r12
  //  qbeq    phpAhScRxProc_2_HashProc_L3, ctxAHRx1.icvSize, 12
  //  qbne    phpAhScRxProc_2_Err_Hash, r9, r13
  ipsec_icv_check   phpAhScRxProc_2_Err_Hash, phpAhScRxProc_2_HashProc_L3, ctxAHRx1.icvSize   
    
phpAhScRxProc_2_HashProc_L3:    
    // Restore Hash in the packet 
    xout    XID_CDEDATA,  r6, b0  
    
    //
    //  Process ESN 
    // 
    qbbc    phpAhScRxProc_2_Replay_L1, ctxAHRx1.ctrlInfo.flagAHRxESN
    
    // Move to ESN and Flash it
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, ahPktInst.esnOffset, ahPktInst.icvOffset
    xout    XID_CDECTRL, CdeCmd, 4  
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 4
    xout    XID_CDECTRL, CdeCmd, 4
    
phpAhScRxProc_2_Replay_L1:     
    //
    //  Replay check
    //
    qbbc    phpAhScRxProc_2_L4, ctxAHRx1.ctrlInfo.flagAHRxReplay
    // Advance the SC Viewer the replay Ctrl
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    .using replayCtrlScope
    //xin     XID_SCVDATA, replayCtrl, SIZE(replayCtrl) ??
    xin     XID_SCVDATA, replayCtrl, 32
    mov     r29, ahPktInst.esnLo   
    call    Replay_CheckAndUpdate
    
    // Verify the replay check result
    qbeq    phpAhScRxProc_2_ReplayPass, r29.b0, SA_REPLAY_RC_OK
    qbeq    phpAhScRxProc_2_ReplayPass, r29.b0, SA_REPLAY_RC_NEW
    
    // Error Handling 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxAHRxStats, SIZE(ctxAHRxStats)
    
    qbeq    phpAhScRxProc_2_ReplayDup, r29.b0, SA_REPLAY_RC_DUP
    
 phpAhScRxProc_2_ReplayOld:
    add     ctxAHRxStats.numOldPkts, ctxAHRxStats.numOldPkts, 1
    // Update System Statistics
    lbco    r5, cStats, PDSP_AH_REPLAY_OLD_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_AH_REPLAY_OLD_OFFSET, 4
    jmp     phpAhScRxProc_2_Err_Replay
 phpAhScRxProc_2_ReplayDup:
    add     ctxAHRxStats.numDupPkts, ctxAHRxStats.numDupPkts, 1
    // Update System Statistics
    lbco    r5, cStats, PDSP_AH_REPLAY_DUP_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_AH_REPLAY_DUP_OFFSET, 4
    jmp     phpAhScRxProc_2_Err_Replay
     
 phpAhScRxProc_2_ReplayPass:
    qbbc    phpAhScRxProc_2_ReplayPass_L2, ahRxPass2Loc1.ctrlInfo.flagAHRxESN
    qbgt    phpAhScRxProc_2_ReplayPass_L2, replayCtrl.winBase, ahPktInst.esnLo
    mov     replayCtrl.winBaseHi, ahPktInst.esnHi
    
 phpAhScRxProc_2_ReplayPass_L2:  
    xout    XID_SCVDATA, replayCtrl, SIZE(replayCtrl)
    .leave replayCtrlScope
    
phpAhScRxProc_2_L4: 
   
    // Issue patch command to copy the original IP
    mov     CdeCmd.Cmd0.b0, CDE_CMD_PATCH_PACKET_BUFFER
    mov     CdeCmd.Cmd0.b1, ahPktInst.ipHdrSize 
    mov     CdeCmd.Cmd0.w2, ahPktInst.ipOffset
    mov     CdeCmd.Cmd1, PHP_INST_BASE_ADDR
    add     CdeCmd.Cmd1, CdeCmd.Cmd1, phpContext.instOffset
    add     CdeCmd.Cmd1, CdeCmd.Cmd1, AH_PKT_INST_IPBUF_OFFSET 
    xout    XID_CDECTRL, CdeCmd, 8
   
    //
    //  Process EOP Data Chunk
    //
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + SIZE(struct_replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxAHRxStats, SIZE(ctxAHRxStats)
    
    add     ctxAHRxStats.numRxPkts,   ctxAHRxStats.numRxPkts, 1
    add     ctxAHRxStats.numRxPktsHi, ctxAHRxStats.numRxPktsHi, 0
    clr     phpContext.instMask, ahRxPass2Loc1.pktId
    xout    XID_SCVDATA, ctxAHRxStats, SIZE(ctxAHRxStats)
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_AH_DEC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_AH_DEC_HI_OFFSET, 8
    
    
 phpAhScRxProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop
    
    
phpAhScRxProc_2_Err_Hash:
    // Update the channel statistics
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + SIZE(struct_replayCtrl))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxAHRxStats, SIZE(ctxAHRxStats)
    add     ctxAHRxStats.numHashFails, ctxAHRxStats.numHashFails, 1
    
    // Update system statistics
    lbco    r5, cStats, PDSP_AH_AUTH_FAIL_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_AH_AUTH_FAIL_OFFSET, 4
    
    jmp  phpAhScRxProc_2_Err_Common
    
phpAhScRxProc_2_Err_Replay:
    
phpAhScRxProc_2_Err_Common:
    // Update the statistics in the security context   
    xout     XID_SCVDATA, ctxAHRxStats, SIZE(ctxAHRxStats)
    
    // Move to the end of packet to ensure that packet descriptor section is available for 
    // updating
    mov     CdeCmd.Cmd0, CDE_CMD_ADVANCE_TO_END
    xout    XID_CDECTRL, CdeCmd, 4 

    // 
    // Update the packet descriptor to drop the packet
    // Set Drop bit 
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket
    
    set     phpContext.chunkInfo.chunkFlag_discard
    sbco    phpContext.chunkInfo, cCdeOutPkt, OFFSET(struct_PktDesc.ChunkInfo), SIZE(struct_PktDesc.ChunkInfo)
    
    // Free the instance buffer
    clr     phpContext.instMask, ahRxPass2Loc1.pktId    
    
    jmp  phpAhScRxProc_2_End

.leave ahRxPass2Scope
.leave ctxAHRxScope
.leave commCtxScope    
 
// Register MAP
// R0, R29       : General and local register
// R6-R8         : rtp header (from packet)
// R14-R21       : IPSEC Security Context including all parametes required
// R10-R13       : output iv 
// R30           : return address

//.struct struct_ahAesCtr
//    .u32  salt
//    .u32  iv1
//    .u32  iv2
//    .u32  cnt
//.ends

.enter ahRxCmdlScope
    .assign  struct_saCmdL,            R12, R13, saCmdL
    .assign  struct_saCmdLReg,         R12, R13, saCmdLReg
.leave ahRxCmdlScope 

//.enter ahAesCtrScope
//    .assign  struct_ahAesCtr,          R8, R11,  saAhAesCtr 
//.leave ahAesCtrScope

 .using ahRxPass1Scope
 .using ctxAHRxScope
 .using ahHdrScope
 .using ahRxCmdlScope
 .using ahAesCtrScope
 
phpAhRxCmdlProc_Gen:

    // Construct command Label in AH General mode
    zero    &saCmdL, SIZE(saCmdL)
    
    // Format the encryption command label
    mov     saCmdL.nextEngId, SAENG_PHP_2
    mov     saCmdL.procLen, ahRxPass1Loc1.hashDataLen
    mov     saCmdL.sopOffset, ahPktInst.ipOffset
    
    qbbc    phpAhRxCmdlProc_Gen_L3, ctxAHRx1.ctrlInfo.flagAHRxCMAC
    // Process CMAC 
    mov     saCmdL.cmdlLen, SA_CMAC_CMDL_SIZE
    //mov     saCmdL.optctrl1, SA_CMDL_MK_OPT_CTRL(SA_ENC_AUX1_OFFSET, 16)
    mov     saCmdL.optctrl1, SA_ENC_AUX1_OFFSET + (16 >> 3)
    sbco    saCmdL, cCmdlBuf, ahRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     ahRxPass1Loc1.cmdlOffset, ahRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Move the security context viewer to the K1/K2 location 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxAHRx.aux1)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4   
    xin     XID_SCVDATA,  r6,  32
    
    and     r0.w0, ahRxPass1Loc1.hashDataLen, 0x000f
    qbne    phpAhRxCmdlProc_Gen_L1, r0.w0, 0
    // K1 is used
    sbco    r6, cCmdlBuf, ahRxPass1Loc1.cmdlOffset, 16
    jmp     phpAhRxCmdlProc_Gen_L2
    
phpAhRxCmdlProc_Gen_L1:
    // k2 is used
    sbco    r10, cCmdlBuf, ahRxPass1Loc1.cmdlOffset, 16
    
phpAhRxCmdlProc_Gen_L2:
    add     ahRxPass1Loc1.cmdlOffset, ahRxPass1Loc1.cmdlOffset, 16
    ret     
    
phpAhRxCmdlProc_Gen_L3:    
    mov     saCmdL.cmdlLen, SIZE(saCmdL)
    sbco    saCmdL, cCmdlBuf, ahRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     ahRxPass1Loc1.cmdlOffset, ahRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
phpAhRxCmdlProc_Gen_End:
    ret       
    
phpAhRxCmdlProc_GMAC:
    // Construct command Label in AH GMAC Mode
    // Construct and copy basic label
    mov     saCmdLReg.reg1.w2,  SA_GMAC_CMDL_SIZE_AH + (SAENG_PHP_2 << 8)
    mov     saCmdL.procLen, ahRxPass1Loc1.hashDataLen
    mov     saCmdL.sopOffset, ahPktInst.ipOffset
    mov     saCmdL.optctrl1, SA_GMAC_CMDL_OPT1
    mov     saCmdLReg.reg2.w0,  (SA_GMAC_CMDL_OPT3 << 8)
    sbco    saCmdL, cCmdlBuf, ahRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     ahRxPass1Loc1.cmdlOffset, ahRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Construst and copy Aux1
    zero    &saCmdL, SIZE(saCmdL)
    lsl     saCmdLReg.reg2, ahRxPass1Loc1.hashDataLen, 3
    sbco    saCmdL, cCmdlBuf, ahRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     ahRxPass1Loc1.cmdlOffset, ahRxPass1Loc1.cmdlOffset, SIZE(saCmdL)
        
    // Construct and copy AES CTR 
    mov     saAhAesCtr.salt,  ctxAHRx1.salt
    mov     saAhAesCtr.cnt,   1  
    sbco    saAhAesCtr, cCmdlBuf, ahRxPass1Loc1.cmdlOffset, SIZE(saAhAesCtr)
    add     ahRxPass1Loc1.cmdlOffset, ahRxPass1Loc1.cmdlOffset, SIZE(saAhAesCtr)
    ret
    
 .leave ahRxPass1Scope
 .leave ctxAHRxScope
 .leave ahHdrScope
 .leave ahRxCmdlScope
 .leave ahAesCtrScope
