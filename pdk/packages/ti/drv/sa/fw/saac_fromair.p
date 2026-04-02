// **********************************************************************************
// * FILE PURPOSE:  PA firmware for PHP PDSPs 
// **********************************************************************************
// * FILE NAME:  saac_toair.p
// *
// * DESCRIPTION: 
// *
// **********************************************************************************/
//
// /***************************************************************************
// * FILE PURPOSE: SA HPS - Air Cipher mode handling
// ****************************************************************************
// * FILE NAME: saac.c
// *
// * DESCRIPTION:
// *  This file contains functions for SA packet header processing of the  
// *  Air Cipher packets which consts of Plaintext PDU header plus 
// *  Cipher Data as shown below:
// *
// *
// *                     0 1 2 3 4 5 6 7 
// *                    +-+-+-+-+-+-+-+-+
// *                    |DC|  Seq Num   |
// *                    +-+-+-+-+-+-+-+-+
// *                    |Seq Num  |P| HE|
// *                  +>+-+-+-+-+-+-+-+-+
// *                  | | Length Ind. |E|
// *                  | +-+-+-+-+-+-+-+-+
// *                  | |               | 
// *                  | |               |
// *                  | |     Data      |
// *                  | |               |
// *                  | |               |
// *                  | +-+-+-+-+-+-+-+-+
// *                  | | PAD or piggy-backed STATUS PDU
// *                  | +-+-+-+-+-+-+-+-+
// *                  |
// *                  |-> Ciphering Unit
// *
// *
// *                  Figure 1.  Ciphering Unit of a WCDMA AMD PDU.
// *
// *
// *                     0 1 2 3 4 5 6 7 
// *                    +-+-+-+-+-+-+-+-+
// *                    | Seq Num     |E|
// *                  +>+-+-+-+-+-+-+-+-+
// *                  | | Length Ind. |E|
// *                  | +-+-+-+-+-+-+-+-+
//*                  | |               | 
// *                  | |               |
// *                  | |     Data      |
// *                  | |               |
// *                  | |               |
// *                  | +-+-+-+-+-+-+-+-+
// *                  |
// *                  |-> Ciphering Unit
// *
// *
// *                  Figure 2.  Ciphering Unit of a WCDMA UMD PDU.
// *
// *                     0 1 2 3 4 5 6 7 
// *                    +-+-+-+-+-+-+-+-+
// *                    |               |
// *                    + IV (8/16byte) |
// *                    |               |
// *                  +>+-+-+-+-+-+-+-+-+
// *                  | |               |
// *                  | |               |
// *                  | |     Data      |
// *                  | |               |
// *                  | |               |
// *                  | +-+-+-+-+-+-+-+-+
// *                  |
// *                  |-> Ciphering Unit
// *
// *                  Figure 3.  Ciphering Unit of a GSM PDU.
// *
// *
// *                     0 1 2 3 4 5 6 7 
// *                    +-+-+-+-+-+-+-+-+
// *                    |               |
// *                    |   Count-C     |
// *                    |   (4-byte)    |
// *                    |               |
// *                  +>+-+-+-+-+-+-+-+-+
// *                  | |               |
// *                  | |               |
// *                  | |     Data      |
// *                  | |               |
// *                  | |               |
// *                  | +-+-+-+-+-+-+-+-+
// *                  |
// *                  |-> Ciphering Unit
// *
// *                  Figure 4.  Ciphering Unit of a WCDMA TMD PDU and PCDP Data PDU.
// *
// *                     0 1 2 3 4 5 6 7 
// *                    +-+-+-+-+-+-+-+-+
// *                    |               |
// *                    |   Count-C     |
// *                    |   (4-byte)    |
// *                    +-+-+-+-+-+-+-+-+ <+
// *                    |R R R| PDCP SN |  |
// *                  +>+-+-+-+-+-+-+-+-+  |
// *                  | |               |  |
// *                  | |               |  |
// *                  | |     Data      |  |
// *                  | |               |  |
// *                  | |               |  |
// *                  | +-+-+-+-+-+-+-+-+  |-> Authentication Unit
// *                  | |   MAC-I       |
// *                  | |   (4 byte)    |
// *                  | +-+-+-+-+-+-+-+-+
// *                  | 
// *                  |-> Ciphering Unit
// *
// *                  Figure 5.  PDCP Control Plane Data Uint for SRBs
// *
// *  There are many variation of PDU formats specified by the various 3GPP Protocols
// *  All the supported operations and data formats are described below:
// *
// *  3GPP Air Ciphering support:
// *  Ciphering handling:
// *  GSM 
// *  Ciphering algorithm: A5/3 (Kasumi F8 based encryption algorithm), Kasumi/F8 and SNOW 3G/F8(A5/4)
// *                       AES-CTR(A5/5)  
// *  PDU format: many variations
// *      Input to SA: 64-bit IV plus all zero payload
// *      Output from SA: 64-bit IV plus key stream (228 bit for GSM A5/3, 696 bit for ECSD A5/3, varialbe length for GAE3)
// *                                                 ==> 29 bytes          ==> 87 bytes
//*  WCDMA 
// *  Ciphering algorithm: Kasumi/F8,Snow 3G/F8, AES-CTR
// *  PDU Format: AMD and UMD PDU with fixed header format including sequencing number
// *              TMD MAC PDU format without sequence number  
// *  Input to SA:RLC AMD/UMD PDU: PDU header plus payload
// *            MAC TMD PDU: 32-bit Count-C plus payload (From Air Traffic)
// *                         payload (To Air Traffic) 
// *  Output from SA: RLC AMD/UMD PDU: PDU header plus payload
// *                  MAC TMD PDU: 32-bit Count-C plus payload
// *  
// *  LTE 
// *  Ciphering Algorithm: 128-bit Snow 3G F8/AES-CTR 
// *  PDU Format: PDCP PDUs
// *  Input to SA: 32-bit Count-C plus PDCP PDU Data (From Air Traffic)
// *               PDCP PDU Data (To Air Traffic)
// *  Output from SA: 32-bit Count-C plus PDCP PDU Data
// *  
// *  
// *  For the Packet Header Processing PDSP subsystem, we only care about the data format.
// *  we plan to handle the following three general cases:
// *  
// *  1.IV in the payload (GSM)
// *  We should be able to handle any GSM encryption mode since the PDSP code does not generate the IV.
// *  
// *  2. Count-C in the payload of From-Air packet, Count-C in the security Context of To-Air packet (WCDMA MAC TMD PDU, LTE PCDP PDU Data)
// *  PHP PDSP code needs to format IV from Count-C and other parameters in the security context
// *  
// *  3. PDU header in the payload (WCDMA RLC AMD/UMD PDU)
// *  PHP PDSP code needs to form IV from the sequence number in the header and other parameters in the security context
// *  Note: Fully offload operation will not be possible since the application needs to perform replay protection and re-ordering.
// *   
// *  
// *  Integrity Check Support:
// *  For LTE operation, both integrity check and ciphering are performed at the PDCP layer 
// *  where the integrity check is applicable to the control PDU only.
// *  For WCDMA and GSM, the integrity check is performed at the RRC layer and 
// *  ciphering is performed at RLC or MAC layer.
// *   
// *  LTE:
// *  Integrity Algorithm: SNOW 3G/F9 and AES-CMAC
// *  
// *  WCDMA:
// *  Integrity Algorithm: kasumi/F9, SNOW 3G/F9
// *  
// *  GSM:
// *  Integrity Algorithm:
// *  
// *  
// *  Integrity plus Ciphering:
// *  Air cipher subsystem is designed to pick-up two security contexts thereby giving you ability to schedule 
// *  both integrity and air-ciphering operation in single launch from PHP using multiple levels.
// *  In this scenario the packet (chunk) loops back to air cipher subsystem via second logical FIFO and 
// *  grabs second portion of security context.
// *
// *
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
// *  R25             
// *  R26                  
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

.struct struct_acFromAirPass1Loc1
    .u16    hashDataLen        // Hash Data length (for command label)
    .u16    encryptedDataLen   // Encrypted Data length (for command label)
    .u16    seqNum             // Sequence Number
    .u16    seqNumMask         // Sequence Number Mask
    .u8     protoOffset        // Protocol Offset
    .u8     hdrSize            // header Size (for command label)
    .u8     cmdlOffset         // Command Label Offset 
    .u8     pktLoc             // packet location
.ends     

.enter acFromAirPass1Scope
    .assign  struct_acFromAirPass1Loc1,  R1,  R3,  acFromAirPass1Loc1
    .assign  struct_acPktInst,           R22, R25, acPktInst      
.leave acFromAirPass1Scope

.enter ctxACFromAirScope
    .assign  struct_ctxAC1,            R14, R21.w2, ctxACFromAir
    .assign  struct_ctxACStats,        R14, R16,    ctxACStats
.leave ctxACFromAirScope

.enter acFromAirCmdlScope
    .assign  struct_saCmdL,            R10, R11, saCmdL
    .assign  struct_saCmdLReg,         R10, R11, saCmdLReg
    .assign  struct_saCmdlOpt,         R10, R13, saCmdlOpt
.leave acFromAirCmdlScope    

///******************************************************************************
// * Function:    sap_ac_from_air_proc_pass1
// ******************************************************************************
// * Description: This function perfroms the first pass of the Air Cipher packet
// *              header processing in the from_air direction as the followings:  
// *
// *  - Extract the sequence number and calculate Count-C (Count-C | sequence number)
// *  - Update Count-C if roll-over occurs
// *  - Calculate the offset and length for both encryption and authentication
// *  - Derive IV based on the Core Encryption Algorithm
// *	    Kasumi IV  = Count-C || Bearer || Direction || Low-bits of IV  (64-bit)
// *	    SNOW 3G IV = Count-C || Bearer || Direction || Low-bits of IV || 
// *                   Count-C || Bearer || Direction || Low-bits of IV (128-bit)
// *      AES-CTR IV = Count-C || Bearer || Direction || Low-bits of IV || 
// *                   0(64) (128-bit)
// *      GSM A5/3 IV= Provided by software
// *  - Derive IV based on the Core Authentication Algorithm
// *	    Kasumi IV   = Count-C || Fresh  (64-bit)
// *	    SNOW 3G IV  = Count-C || Bearer || 0 || Low-bits of IV || 
// *                    Count-C XOR (Direction << 31) || Bearer || 0 || Low-bits of IV
// *                                                                    XOR (Direction << 15) 
// *      AES-CMAC IV = Count-C || Bearer || Direction || Low-bits of IV (64-bit) 
// *	- Insert the Air Cipher Command Label with data length and offset and IV
// *	- Update statistics if no authentication    
// *
// *  Where the packet info consists of
// *        - pointer to the packet
// *        - packet length
// *        - offset to the Plaintext PDU header
// *
// *****************************************************************************/

phpAcFromAirProc_1:

    // Error Check
    
    
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using acFromAirPass1Scope
    .using ctxACFromAirScope
    
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAC.countC))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxACFromAir, SIZE(ctxACFromAir)
    
    // clear Local Variables (may be just some of them)
    zero    &acFromAirPass1Loc1,    SIZE(acFromAirPass1Loc1)    
    
    // Record packet instance index for PASS1 
    // mov     phpContext.ctrlInfo,  PktDesc.PktId
    
    qbbc    phpAcFromAirProc_1_OtherChunk_L1,   phpContext.chunkInfo.chunkFlag_sop
    
 phpAcFromAirProc_1_FirstChunk_L1:    
 
    //clear the control flags and other variables
    zero    &acPktInst,    SIZE(acPktInst)
    
 phpAcFromAirProc_1_FirstChunk_L2:  
    mov     acPktInst.nextEngId, ctxACFromAir.firstEngId
    mov     acPktInst.nextEngId_pass2, SAENG_OUT2_1          //default route
    
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, acPktInst.nextEngId
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    xout    XID_CDEDATA, PktDesc, 8   
      
    // Advance the window past the Descriptor to the control Info area
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_CONTROL
    xout    XID_CDECTRL, CdeCmd, 4      
      
    .leave  pktDescScope
    .using protoInfoScope
    // Extract protocol Info
    xin     XID_CDEDATA, protoInfo,    SIZE(payloadInfoS2)

   // Skip logging if not enabled
   qbbc SkipAcDbgLog_2, phpContext.ctrlInfo.t_php_pkt_dbg_info
    // 12 bytes of PktInfo
    // sbco    protoInfo, cAcDbgBase, s_acDbg.offset, 12
    mov      r0,  PDSP_ACDBG_BASE    
    sbbo    protoInfo, r0, s_acDbg.offset, 12
    add     s_acDbg.offset,   s_acDbg.offset,  12
SkipAcDbgLog_2:    

    mov     acFromAirPass1Loc1.protoOffset,  protoInfo.offset
    mov     acFromAirPass1Loc1.encryptedDataLen, protoInfo.len
    and     CdeCmd.Cmd0.w2, payloadInfoL.recordLen, 0x1F
    qbge    phpAcFromAirProc_ShortInfo_L2, CdeCmd.Cmd0.w2,   8
        // IV present: need to copy IV and count-C into the corresponding buffers
        // Note: count-C should not be stored at the SC since it should not affect the next packet
        set   acPktInst.ctrlFlag.acIvCopied
        mov   acPktInst.countC, payloadInfoS2.countC 
        mov   r0.b1, ctxACFromAir.ivSize 
        qbbs  phpAcFromAirProc_ShortInfo_L1, ctxACFromAir.ctrlInfo.flagACCMAC   
            sbco  payloadInfoS2.iv1, cCmdlBuf, SA_CMDL_HEADER_SIZE_BYTES, b1
            jmp   phpAcFromAirProc_protoInfo_End
    
phpAcFromAirProc_ShortInfo_L1:
            // AES-CMAC IV size is 8
            sbco  payloadInfoS2.iv1, cAuxBuf, 0, 8
            jmp   phpAcFromAirProc_protoInfo_End
    
phpAcFromAirProc_ShortInfo_L2:     
    mov     CdeCmd.Cmd0.w2, 8
    qbbc    phpAcFromAirProc_protoInfo_End, ctxACFromAir.ctrlInfo.flagACCountCPresent
         // record Count-C
         mov ctxACFromAir.countC,  payloadInfoS.countC               
         //pass through 
    
    .leave protoInfoScope
phpAcFromAirProc_protoInfo_End:
    
    // Issue command to flash out 8-byte PSInfo
    // Now we are at the control section. Insert the command label here
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    xout    XID_CDECTRL, CdeCmd, 4      
    
    qbeq    phpAcFromAirProc_1_CmdlLen_L2_1, ctxACFromAir.cmdlLen, 0  
    // Insert room for command label
phpAcFromAirProc_1_CmdlLen_L2:    
    mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
    mov     CdeCmd.Cmd0.b1, ctxACFromAir.cmdlLen
    xout    XID_CDECTRL, CdeCmd, 4             // Insert all zero for the command label 
        
phpAcFromAirProc_1_CmdlLen_L2_1:    
    
    // Advance the window past the control Info area
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4      
    
    // Advance to the AC header only if protoOffset is non-zero
    qbeq    phpAcFromAirProc_1_FirstChunk_L3, acFromAirPass1Loc1.protoOffset, 0
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, acFromAirPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4      
    
    mov     acFromAirPass1Loc1.pktLoc,  acFromAirPass1Loc1.protoOffset
   
phpAcFromAirProc_1_FirstChunk_L3:  
    // Extract 16 bytes (count-C, header or IV 
    xin     XID_CDEDATA, r6, 16
    
    // Should we process PDU header (group 1)
    qbbc  phpAcFromAirProc_1_HdrProc_L4, ctxACFromAir.ctrlInfo.flagACHdrPresent
    // HDR is present, extract the sequence number for the header
    mov   r0.b0, 1
    lsl   r0.w2, r0.b0, ctxACFromAir.seqNumSize
    sub   acFromAirPass1Loc1.seqNumMask, r0.w2, 1
    qbeq  phpAcFromAirProc_1_HdrProc_L1, ctxACFromAir.hdrSize, 2
    // Header size is 1, seq_num = (p_data[0] >> p_proto_ctx->seq_num_shift) & seq_num_mask;
    lsr   acFromAirPass1Loc1.seqNum, r6.b3, ctxACFromAir.seqNumShift
    jmp   phpAcFromAirProc_1_HdrProc_L2 
phpAcFromAirProc_1_HdrProc_L1:
    // Header size is 2, seq_num = (utlRead16bits_m(p_data, 0) >>  p_proto_ctx->seq_num_shift) & seq_num_mask;
    lsr   acFromAirPass1Loc1.seqNum, r6.w2, ctxACFromAir.seqNumShift
phpAcFromAirProc_1_HdrProc_L2:
    and   acFromAirPass1Loc1.seqNum, acFromAirPass1Loc1.seqNum, acFromAirPass1Loc1.seqNumMask
    
    not   r29, acFromAirPass1Loc1.seqNumMask
    and   r0.w0,  ctxACFromAir.countC.w0, acFromAirPass1Loc1.seqNumMask
    qble  phpAcFromAirProc_1_HdrProc_L3, acFromAirPass1Loc1.seqNum, r0.w0
    add   ctxACFromAir.countC, ctxACFromAir.countC, r0.w2 
phpAcFromAirProc_1_HdrProc_L3:      
    and   ctxACFromAir.countC, ctxACFromAir.countC, r29
    or    ctxACFromAir.countC.w0, ctxACFromAir.countC.w0, acFromAirPass1Loc1.seqNum
    mov   acFromAirPass1Loc1.hdrSize, ctxACFromAir.hdrSize
    jmp   phpAcFromAirProc_1_HdrProc_End
    
phpAcFromAirProc_1_HdrProc_L4:
    // Should we extract Count-C from packet: group 2 ==> Count-C at payload Info
    qbbs    phpAcFromAirProc_1_HdrProc_End, ctxACFromAir.ctrlInfo.flagACCountCPresent   
     
phpAcFromAirProc_1_HdrProc_L5:
    // No special Processing: Group 3
    qbbs    phpAcFromAirProc_1_HdrProc_End, acPktInst.ctrlFlag.acIvCopied
    add   ctxACFromAir.countC, ctxACFromAir.countC, 1
    //mov   acFromAirPass1Loc1.hdrSize, 0
    //pass through

phpAcFromAirProc_1_HdrProc_End:
    // write back Count-C
    xout    XID_SCVDATA, ctxACFromAir.countC, 4
    qbbc    phpAcFromAirProc_1_HdrProc_End_L1,  ctxACFromAir.ctrlInfo2.flagACCountCCopy
        // Copy coutC to timestamp field
        wbc   r31.tStatus_CDEBusy
        qbbc  phpAcFromAirProc_1_HdrProc_End_1, acPktInst.ctrlFlag.acIvCopied
            sbco  acPktInst.countC,  cCdeOutPkt, OFFSET(struct_PktDesc.Timestamp),  SIZE(struct_PktDesc.Timestamp)
            jmp   phpAcFromAirProc_1_HdrProc_End_L1
phpAcFromAirProc_1_HdrProc_End_1:        
            sbco  ctxACFromAir.countC,  cCdeOutPkt, OFFSET(struct_PktDesc.Timestamp),  SIZE(struct_PktDesc.Timestamp)
    // pass through 
    
phpAcFromAirProc_1_HdrProc_End_L1:
    
    // Is IV present in the payload, Group 4
    // Note: No other flag should be set in this case so that IV is stored at R6-R9
    qbbc  phpAcFromAirProc_1_FirstChunk_L4, ctxACFromAir.ctrlInfo.flagACIVPresent
    // IV should be provided in GSM modes
    // Note: We do not support both encryption and authentication in this case.
    add   acFromAirPass1Loc1.hdrSize, acFromAirPass1Loc1.hdrSize, ctxACFromAir.ivSize
    // Store the IV in the command label table if not CMAC
    // redundant check since CMAC is only supported in LTE PDU where flagACIVPresent is not set
    qbbs  phpAcFromAirProc_1_FirstChunk_L4, ctxACFromAir.ctrlInfo.flagACCMAC   
        mov   r0.b1, ctxACFromAir.ivSize 
        sbco  r6, cCmdlBuf, SA_CMDL_HEADER_SIZE_BYTES, b1
    
phpAcFromAirProc_1_FirstChunk_L4:  
    
    // Calculate the payload related information
    add     acPktInst.icvOffset, acFromAirPass1Loc1.protoOffset, acFromAirPass1Loc1.encryptedDataLen
    sub     acPktInst.icvOffset, acPktInst.icvOffset, ctxACFromAir.icvSize
    mov     acPktInst.icvOffset_pass2, acPktInst.icvOffset
    add     acPktInst.hdrOffset, acFromAirPass1Loc1.protoOffset, acFromAirPass1Loc1.hdrSize   
    sub     acFromAirPass1Loc1.encryptedDataLen, acFromAirPass1Loc1.encryptedDataLen, acFromAirPass1Loc1.hdrSize
    sub     acFromAirPass1Loc1.hashDataLen,  acFromAirPass1Loc1.encryptedDataLen, ctxACFromAir.icvSize
    sub     acFromAirPass1Loc1.encryptedDataLen, acFromAirPass1Loc1.encryptedDataLen, ctxACFromAir.authHdrSize
    
    // Verify whether Kasumi F9 padding is necessary
    qbbc    phpAcFromAirProc_1_FirstChunk_L4_1, ctxACFromAir.ctrlInfo.flagACKasumiF9
    and     r0.w0,  acFromAirPass1Loc1.hashDataLen, 0x7
    qbne    phpAcFromAirProc_1_CmdlabelProc_L1, r0.w0, 0 
    set     acPktInst.ctrlFlag.acPaddingInserted
    add     acFromAirPass1Loc1.hashDataLen, acFromAirPass1Loc1.hashDataLen, 8
    
    // passthrough  to save a jmp instruction 
    // Note: falgACKasmuiF9 excludes flagACCMAC
    
phpAcFromAirProc_1_FirstChunk_L4_1:    
    // 
    // CMAC special processing if IV is not present in the packet already:
    // - move to the hdr offset
    // - reserve room for IV insertion
    qbbc    phpAcFromAirProc_1_CmdlabelProc_L1, ctxACFromAir.ctrlInfo.flagACCMAC
        // CMAC uses 8-byte IV
        add     acFromAirPass1Loc1.hashDataLen, acFromAirPass1Loc1.hashDataLen, 8
        qbbs    phpAcFromAirProc_1_CmdlabelProc_L1, ctxACFromAir.ctrlInfo.flagACIVPresent
            mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
            mov     CdeCmd.Cmd0.w2, acFromAirPass1Loc1.hdrSize
            xout    XID_CDECTRL, CdeCmd, 4      
    
    qbbs    phpAcFromAirProc_1_FirstChunk_L4_2, acPktInst.ctrlFlag.acIvCopied  // Skip IV formatting operation
    // Format and insert IV R10-R13
    call    AC_formIVAuth
    sbco    r10,  cAuxBuf, 0, 8
    
phpAcFromAirProc_1_FirstChunk_L4_2:    
    mov     CdeCmd.Cmd0, CDE_CMD_INSERT_PACKET_BUFFER
    mov     CdeCmd.Cmd0.b1, 8 
    mov     CdeCmd.Cmd1, PHP_AUX_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
        //Adjust length information
        add     acPktInst.icvOffset_pass2, acPktInst.icvOffset_pass2, 8
        set     acPktInst.ctrlFlag.acIvInserted
        add     acFromAirPass1Loc1.pktLoc,  acFromAirPass1Loc1.protoOffset, acFromAirPass1Loc1.hdrSize

        //
        //  Command Label Processing
        //
        .using acFromAirCmdlScope  
       
 phpAcFromAirProc_1_CmdlabelProc_L1:

#ifdef SASS_GEN2 
    qbbc    phpAcFromAirProc_1_CmdlabelProc_L1_gen,   ctxACFromAir.ctrlInfo.flagACF8F9
        zero &saCmdL, SIZE(saCmdL)
        // Prepare F8F9 Command Label
        //mov     saCmdL.cmdlLen,    SA_CMDL_HEADER_SIZE_BYTES + 48
        //mov     saCmdL.nextEngId,  SAENG_PHP_2
        mov     saCmdLReg.reg1.w2,   (SA_CMDL_HEADER_SIZE_BYTES + 48) | (SAENG_PHP_2 << 8)
        add     saCmdL.procLen,    acFromAirPass1Loc1.hashDataLen, ctxACFromAir.icvSize
        mov     saCmdL.sopOffset,  acFromAirPass1Loc1.protoOffset
        //mov	    saCmdL.optctrl1,   SA_3GPP_IV1_CMDL_OPT
        //mov	    saCmdL.optctrl2,   SA_3GPP_AUX1_CMDL_OPT
        mov	    saCmdLReg.reg2.w1,  (SA_3GPP_IV1_CMDL_OPT << 8) + SA_3GPP_AUX1_CMDL_OPT
	    qbbc	phpAcFromAirProc_1_CmdlabelProc_F8F9_L1, ctxACFromAir.ctrlInfo.flagACKeyInScratch
            mov 	saCmdL.optctrl3,   SA_3GPP_KEY_CMDL_OPT
            add     saCmdL.cmdlLen, saCmdL.cmdlLen, 32
                
phpAcFromAirProc_1_CmdlabelProc_F8F9_L1: 
        // Store the command label
        sbco    saCmdL, cCmdlBuf,   acFromAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
        // Encryption IV processing: Get and store the IV
        qbbs    phpAcFromAirProc_1_CmdlabelProc_F8F9_L2, ctxACFromAir.ctrlInfo.flagACIVPresent
    
            // copy IV to the command label location
            call    AC_formIVEnc
            mov     r0.b0, ctxACFromAir.ivSize 
            sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, b0

phpAcFromAirProc_1_CmdlabelProc_F8F9_L2:
        // IV has been copied in both cases
        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, ctxACFromAir.ivSize
        
        // IV processing: Get and store the IV
        qbbs    phpAcFromAirProc_1_CmdlabelProc_F8F9_L3, ctxACFromAir.ctrlInfo.flagACIVPresent
    
            // copy Authentication IV to the command label location
            call    AC_formIVAuth
            mov     r0.b0, ctxACFromAir.ivSize 
            sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, b0
        
phpAcFromAirProc_1_CmdlabelProc_F8F9_L3:  
        // IV has been copied in both cases
        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, ctxACFromAir.ivSize
        
        // Copy Miscellaneous data
        zero    &saCmdlOpt,     SIZE(saCmdlOpt)
        mov     saCmdlOpt.data1.b3, ctxACFromAir.ivOpt
        add     saCmdlOpt.data4.w0, acFromAirPass1Loc1.hashDataLen, ctxACFromAir.icvSize
        sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, 16
        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
        
	    // Check if mac key needs to be copied
	    qbbc	phpAcFromAirProc_1_CmdlabelProc_End, ctxACFromAir.ctrlInfo.flagACKeyInScratch
	        // Copy enc key
            qbbs    phpAcFromAirProc_1_encKey_copy_1, ctxACFromAir.encKeyOffset.t_flag_key_at_2nd_block
	            lbco	saCmdlOpt.data1,  cKeyRamBase, ctxACFromAir.encKeyOffset,     16
                jmp     phpAcFromAirProc_1_encKey_copy_end
phpAcFromAirProc_1_encKey_copy_1:
                clr     ctxACFromAir.encKeyOffset.t_flag_key_at_2nd_block
	            lbco	saCmdlOpt.data1,  cKeyRamBase2, ctxACFromAir.encKeyOffset,    16
phpAcFromAirProc_1_encKey_copy_end:                
	        sbco    saCmdlOpt.data1,  cCmdlBuf,    acFromAirPass1Loc1.cmdlOffset, 16
	        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
            
            // Copy auth key
            qbbs    phpAcFromAirProc_1_macKey_copy_1, ctxACFromAir.macKeyOffset.t_flag_key_at_2nd_block
	            lbco	saCmdlOpt.data1,  cKeyRamBase, ctxACFromAir.macKeyOffset,    16
                jmp     phpAcFromAirProc_1_macKey_copy_end
phpAcFromAirProc_1_macKey_copy_1:
                clr     ctxACFromAir.macKeyOffset.t_flag_key_at_2nd_block
                lbco	saCmdlOpt.data1,  cKeyRamBase2, ctxACFromAir.macKeyOffset,    16
phpAcFromAirProc_1_macKey_copy_end:            
	        sbco    saCmdlOpt.data1,  cCmdlBuf,    acFromAirPass1Loc1.cmdlOffset, 16
	        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
 
            jmp phpAcFromAirProc_1_CmdlabelProc_End
         
phpAcFromAirProc_1_CmdlabelProc_L1_gen:
#endif 
    // skip command processing if command length == 0
    qbeq phpAcFromAirProc_1_PADDING_L1, ctxACFromAir.cmdlLen, 0
    zero &saCmdL, SIZE(saCmdL)
    
    qbbc  phpAcFromAirProc_1_CmdlabelProc_L5, ctxACFromAir.ctrlInfo.flagACEnc
    // Prepare Encryption Command Label 
    qbbs    phpAcFromAirProc_1_CmdlabelProc_L2, ctxACFromAir.ctrlInfo.flagACAuth
        mov     saCmdL.nextEngId,  SAENG_PHP_2
        jmp     phpAcFromAirProc_1_CmdlabelProc_L3
phpAcFromAirProc_1_CmdlabelProc_L2: 
        // Both encryption and authentication are required
        // Verify whether Kasumi-F9 padding is required. If yes, need to insert padding at the second pass and enable pass3
        qbbs    phpAcFromAirProc_1_CmdlabelProc_L2_1, acPktInst.ctrlFlag.acPaddingInserted
            mov     saCmdL.nextEngId,  SAENG_AIRC_2
            jmp     phpAcFromAirProc_1_CmdlabelProc_L3
phpAcFromAirProc_1_CmdlabelProc_L2_1: 
            mov     saCmdL.nextEngId,  SAENG_PHP_2
            clr     acPktInst.ctrlFlag.acPaddingInserted
            set     acPktInst.ctrlFlag.acPaddingInserted2
            mov     acPktInst.nextEngId_pass2, SAENG_AIRC_2
            mov     acPktInst.icvOffset_pass3, acPktInst.icvOffset_pass2
            // pass through
           
phpAcFromAirProc_1_CmdlabelProc_L3:
    add     saCmdL.cmdlLen,    ctxACFromAir.ivSize, SA_CMDL_HEADER_SIZE_BYTES
    mov     saCmdL.procLen,    acFromAirPass1Loc1.encryptedDataLen
    add     saCmdL.sopOffset,  acFromAirPass1Loc1.protoOffset, acFromAirPass1Loc1.hdrSize
    add     saCmdL.sopOffset,  saCmdL.sopOffset, ctxACFromAir.authHdrSize
    // add CMAC IV size if CMAC-IV is inserted (CMAC only) to sopOffset
    qbbc    phpAcFromAirProc_1_CmdlabelProc_L3_1, acPktInst.ctrlFlag.acIvInserted
        add     saCmdL.sopOffset,  saCmdL.sopOffset, 8
    
phpAcFromAirProc_1_CmdlabelProc_L3_1:
    mov     saCmdL.optctrl1,   ctxACFromAir.ivOpt
	// Check to see if need to copy enc key from scratch
	// Note: assume always do either enc or mac, never combined operation
	qbbc	phpAcFromAirProc_1_CmdlabelProc_L3_Snow3G_check2, ctxACFromAir.ctrlInfo.flagACKeyInScratch
        mov		saCmdL.optctrl2,   SA_3GPP_KEY1_CMDL_OPT
	    add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
	    // Check to see if kasumi modkey needs to be created
	    qbbc	phpAcFromAirProc_1_CmdlabelProc_L3_Snow3G_check, ctxACFromAir.ctrlInfo.flagACKasumiF8
            mov		saCmdL.optctrl3,   SA_3GPP_MOD_KEY_CMDL_OPT
	        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
            
phpAcFromAirProc_1_CmdlabelProc_L3_Snow3G_check:  
#ifdef SASS_GEN2
        // Check to see if Snow3G/Zuc operation is required
	    qbbc	phpAcFromAirProc_1_CmdlabelProc_L3_2, ctxACFromAir.ctrlInfo.flagACSnow3GZuc
            mov		saCmdL.optctrl3,   SA_3GPP_MISC_CMDL_OPT
	        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
#endif            
            jmp     phpAcFromAirProc_1_CmdlabelProc_L3_2
        
phpAcFromAirProc_1_CmdlabelProc_L3_Snow3G_check2:
#ifdef SASS_GEN2
	    qbbc	phpAcFromAirProc_1_CmdlabelProc_L3_2, ctxACFromAir.ctrlInfo.flagACSnow3GZuc
            mov		saCmdL.optctrl2,   SA_3GPP_MISC_CMDL_OPT
	        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
            // pass through
#endif
	
phpAcFromAirProc_1_CmdlabelProc_L3_2:
    // Store the command label
    sbco    saCmdL, cCmdlBuf,   acFromAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // IV processing: Get and store the IV
    qbbs    phpAcFromAirProc_1_CmdlabelProc_L4, acPktInst.ctrlFlag.acIvCopied  // Skip IV formatting operation
    qbbs    phpAcFromAirProc_1_CmdlabelProc_L4, ctxACFromAir.ctrlInfo.flagACIVPresent
    
    // copy IV to the command label location
    call    AC_formIVEnc
    mov     r0.b0, ctxACFromAir.ivSize 
    sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, b0
    
phpAcFromAirProc_1_CmdlabelProc_L4:
    // IV has been copied in both cases
    add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, ctxACFromAir.ivSize
    
phpAcFromAirProc_1_CmdlabelProc_L4_KeyCopy:    
	// Copy key from scratch memory if necessary
	qbbc	phpAcFromAirProc_1_CmdlabelProc_L4_Snow3G, ctxACFromAir.ctrlInfo.flagACKeyInScratch
	    // Copy enc key
        qbbs    phpAcFromAirProc_1_L4_encKey_copy_1, ctxACFromAir.encKeyOffset.t_flag_key_at_2nd_block
	        lbco	saCmdlOpt.data1,  cKeyRamBase, ctxACFromAir.encKeyOffset,     16
            jmp     phpAcFromAirProc_1_L4_encKey_copy_end
phpAcFromAirProc_1_L4_encKey_copy_1:
            clr     ctxACFromAir.encKeyOffset.t_flag_key_at_2nd_block
	        lbco	saCmdlOpt.data1,  cKeyRamBase2, ctxACFromAir.encKeyOffset,    16
phpAcFromAirProc_1_L4_encKey_copy_end:                
	    sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, 16
	    add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
	    // Check if kasumi modkey needs to be copied
	    qbbc	phpAcFromAirProc_1_CmdlabelProc_L4_Snow3G, ctxACFromAir.ctrlInfo.flagACKasumiF8
	        // Make and copy kasumi modkey
	        ldi		r0.w0, 0x5555
	        ldi		r0.w2, 0x5555
	        xor		saCmdlOpt.data1, saCmdlOpt.data1, r0
	        xor		saCmdlOpt.data2, saCmdlOpt.data2, r0
	        xor		saCmdlOpt.data3, saCmdlOpt.data3, r0
	        xor		saCmdlOpt.data4, saCmdlOpt.data4, r0
	        sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, 16
	        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
            
phpAcFromAirProc_1_CmdlabelProc_L4_Snow3G: 
#ifdef SASS_GEN2
	// Check if we  need to add Snow3G/ZUC parameters
	qbbc	phpAcFromAirProc_1_CmdlabelProc_L5, ctxACFromAir.ctrlInfo.flagACSnow3GZuc
        zero    &saCmdlOpt, SIZE(saCmdlOpt)
        mov     saCmdlOpt.data4, acFromAirPass1Loc1.encryptedDataLen   
	    sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, 16
	    add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
#endif        
        // pass through
	
phpAcFromAirProc_1_CmdlabelProc_L5:
    qbbc  phpAcFromAirProc_1_CmdlabelProc_End, ctxACFromAir.ctrlInfo.flagACAuth
        // Prepare Authentication Command Label
        zero    &saCmdL, SIZE(saCmdL)
        // check whether PASS3 should be used in stead
        qbbc    phpAcFromAirProc_1_CmdlabelProc_L5_setEngId_1, acPktInst.ctrlFlag.acPaddingInserted2
            mov     saCmdL.nextEngId,  SAENG_PHP_3
            jmp     phpAcFromAirProc_1_CmdlabelProc_L5_setEngId_2 
phpAcFromAirProc_1_CmdlabelProc_L5_setEngId_1:                 
            mov     saCmdL.nextEngId,  SAENG_PHP_2
            
phpAcFromAirProc_1_CmdlabelProc_L5_setEngId_2:        
        mov     saCmdL.procLen,    acFromAirPass1Loc1.hashDataLen
        add     saCmdL.sopOffset,  acFromAirPass1Loc1.protoOffset, acFromAirPass1Loc1.hdrSize
        mov     saCmdL.cmdlLen,    SA_CMDL_HEADER_SIZE_BYTES
        
	    // Check to see if need to copy mac key from scratch
	    qbbc	phpAcFromAirProc_1_CmdlabelProc_L5_Snow3G_check2, ctxACFromAir.ctrlInfo.flagACKeyInScratch
	        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
            qbbc    phpAcFromAirProc_1_CmdlabelProc_L5_KeyOpt1, ctxACFromAir.ctrlInfo.flagACSnow3GZuc
	            mov	saCmdL.optctrl2,   SA_3GPP_KEY2_CMDL_OPT
                jmp phpAcFromAirProc_1_CmdlabelProc_L5_KeyOptEnd
phpAcFromAirProc_1_CmdlabelProc_L5_KeyOpt1:
                mov	saCmdL.optctrl2,   SA_3GPP_KEY1_CMDL_OPT
phpAcFromAirProc_1_CmdlabelProc_L5_KeyOptEnd: 
               
	        // Check to see if need to create modkey for kasumi
	        qbbc	phpAcFromAirProc_1_CmdlabelProc_L5_Snow3G_check, ctxACFromAir.ctrlInfo.flagACKasumiF9
	            mov		saCmdL.optctrl3,   SA_3GPP_MOD_KEY_CMDL_OPT
	            add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
                jmp     phpAcFromAirProc_1_CmdlabelProc_L5_optEnd
        
phpAcFromAirProc_1_CmdlabelProc_L5_Snow3G_check:
#ifdef SASS_GEN2
            // Check to see if Snow3G/Zuc operation is required
	        qbbc	phpAcFromAirProc_1_CmdlabelProc_L5_optEnd, ctxACFromAir.ctrlInfo.flagACSnow3GZuc
                mov		saCmdL.optctrl3,   SA_3GPP_MISC_CMDL_OPT
	            add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
#endif                
                jmp     phpAcFromAirProc_1_CmdlabelProc_L5_optEnd
                                         
phpAcFromAirProc_1_CmdlabelProc_L5_Snow3G_check2:   
#ifdef SASS_GEN2
            // Check to see if Snow3G/Zuc operation is required
	        qbbc	phpAcFromAirProc_1_CmdlabelProc_L5_optEnd, ctxACFromAir.ctrlInfo.flagACSnow3GZuc
                mov		saCmdL.optctrl2,   SA_3GPP_MISC_CMDL_OPT
	            add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
                // pass through
#endif    
phpAcFromAirProc_1_CmdlabelProc_L5_optEnd:
        qbbs    phpAcFromAirProc_1_CmdlabelProc_L5_2, ctxACFromAir.ctrlInfo.flagACCMAC
    
        //non-CMAC operation
        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, ctxACFromAir.ivSize
        qbbc    phpAcFromAirProc_1_CmdlabelProc_L5_ivOpt1, ctxACFromAir.ctrlInfo.flagACSnow3GZuc
            mov     saCmdL.optctrl1,   SA_3GPP_IV2_CMDL_OPT
            jmp     phpAcFromAirProc_1_CmdlabelProc_L5_ivOptEnd
            
phpAcFromAirProc_1_CmdlabelProc_L5_ivOpt1:
            mov     saCmdL.optctrl1,   ctxACFromAir.ivOpt
            
            //pass through            
phpAcFromAirProc_1_CmdlabelProc_L5_ivOptEnd: 

        // Store the command label
        sbco    saCmdL, cCmdlBuf,   acFromAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
        // IV processing: Get and store the IV
    qbbs    phpAcFromAirProc_1_CmdlabelProc_L5_1, acPktInst.ctrlFlag.acIvCopied  // Skip IV formatting operation
        qbbs    phpAcFromAirProc_1_CmdlabelProc_L5_1, ctxACFromAir.ctrlInfo.flagACIVPresent
    
            // copy IV to the command label location
            call    AC_formIVAuth
            mov     r0.b0, ctxACFromAir.ivSize 
            sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, b0
            //add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, ctxACFromAir.ivSize
    
phpAcFromAirProc_1_CmdlabelProc_L5_1:
        // IV has been copied in both cases
        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, ctxACFromAir.ivSize
        jmp     phpAcFromAirProc_1_CmdlabelProc_L6_KeyCopy
	    
phpAcFromAirProc_1_CmdlabelProc_L5_2:    
        //CMAC  operation
        qbbc    phpAcFromAirProc_1_CmdlabelProc_L5_3, ctxACFromAir.ctrlInfo.flagACIVPresent
        sub     saCmdL.sopOffset,  saCmdL.sopOffset, 8                    //adjust SOP offset to include 8-byte CMAC-IV
    
 phpAcFromAirProc_1_CmdlabelProc_L5_3:    
        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, SA_CMAC_CMDL_SIZE - SA_CMDL_HEADER_SIZE_BYTES
        mov     saCmdL.optctrl1,   SA_ENC_AUX1_OFFSET + (16 >> 3)
    
        // Store the command label
        sbco    saCmdL, cCmdlBuf,  acFromAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
        // Move the security context viewer to the K1/K2 location 
        mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxAC.aux)) << 8)
        xout    XID_SCVCTRL, CdeCmd, 4   
        xin     XID_SCVDATA, r6,  32
    
        and     r0.w0, acFromAirPass1Loc1.hashDataLen, 0x000f
        qbne    phpAcFromAirProc_1_CmdlabelProc_L5_4, r0.w0, 0
            // K1 is used
            sbco    r6, cCmdlBuf,  acFromAirPass1Loc1.cmdlOffset, 16
            jmp     phpAcFromAirProc_1_CmdlabelProc_L5_5
    
phpAcFromAirProc_1_CmdlabelProc_L5_4:
            // k2 is used
            sbco    r10, cCmdlBuf,  acFromAirPass1Loc1.cmdlOffset, 16
     
phpAcFromAirProc_1_CmdlabelProc_L5_5:
        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
    
        // Move the security context viewer back 
        // Note: It may not be necessay since we do not update statistics at PHP Pass1 anymore
        // Commented out for refernce only
        //mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxAC.countC)) << 8)
        //xout    XID_SCVCTRL, CdeCmd, 4   
    
        //jmp     phpAcFromAirProc_1_CmdlabelProc_End
        //jmp     phpAcFromAirProc_1_CmdlabelProc_L6_KeyCopy
    
        // pass through: common processing
    
phpAcFromAirProc_1_CmdlabelProc_L6_KeyCopy:    
    // Check if mac key needs to be copied
	qbbc	phpAcFromAirProc_1_CmdlabelProc_L6_snow3G, ctxACFromAir.ctrlInfo.flagACKeyInScratch
        qbbs    phpAcFromAirProc_1_L6_macKey_copy_1, ctxACFromAir.macKeyOffset.t_flag_key_at_2nd_block
	        lbco	saCmdlOpt.data1,  cKeyRamBase, ctxACFromAir.macKeyOffset,     16
            jmp     phpAcFromAirProc_1_L6_macKey_copy_end
phpAcFromAirProc_1_L6_macKey_copy_1:
            clr     ctxACFromAir.macKeyOffset.t_flag_key_at_2nd_block
	        lbco	saCmdlOpt.data1,  cKeyRamBase2, ctxACFromAir.macKeyOffset,    16
phpAcFromAirProc_1_L6_macKey_copy_end:                
	    sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, 16
	    add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
	    // Check if kasumi modkey needs to be copied
	    qbbc	phpAcFromAirProc_1_CmdlabelProc_L6_snow3G, ctxACFromAir.ctrlInfo.flagACKasumiF9
	    // Make and copy kasumi modkey
	        ldi		r0.w0, 0xAAAA
	        ldi		r0.w2, 0xAAAA
	        xor		saCmdlOpt.data1, saCmdlOpt.data1, r0
	        xor		saCmdlOpt.data2, saCmdlOpt.data2, r0
	        xor		saCmdlOpt.data3, saCmdlOpt.data3, r0
	        xor		saCmdlOpt.data4, saCmdlOpt.data4, r0
	        sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, 16
	        add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
	
    
phpAcFromAirProc_1_CmdlabelProc_L6_snow3G:
#ifdef SASS_GEN2
	// Check if we  need to add Snow3G/ZUC parameters
	qbbc	phpAcFromAirProc_1_CmdlabelProc_End, ctxACFromAir.ctrlInfo.flagACSnow3GZuc
        zero    &saCmdlOpt, SIZE(saCmdlOpt)
        mov     saCmdlOpt.data4, acFromAirPass1Loc1.hashDataLen   
	    sbco    saCmdlOpt.data1,  cCmdlBuf, acFromAirPass1Loc1.cmdlOffset, 16
	    add     acFromAirPass1Loc1.cmdlOffset, acFromAirPass1Loc1.cmdlOffset, 16
        // pass through
#endif
    
phpAcFromAirProc_1_CmdlabelProc_L6_1:
	.leave acFromAirCmdlScope  

phpAcFromAirProc_1_CmdlabelProc_End:
   // Skip logging if not enabled
   qbbc SkipAcDbgLog_3, phpContext.ctrlInfo.t_php_pkt_dbg_info
    // 8 bytes of Command Label to be stored
    lbco    r10, cCmdlBuf,   0, 8    
    // sbco    r10, cAcDbgBase, s_acDbg.offset, 8
    mov      r0,  PDSP_ACDBG_BASE
    sbbo    r10, r0, s_acDbg.offset, 8
    add     s_acDbg.offset,      s_acDbg.offset,  8
SkipAcDbgLog_3:    

    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, acFromAirPass1Loc1.cmdlOffset 
    mov     CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
    jmp     phpAcFromAirProc_1_PADDING_L1     
    
 phpAcFromAirProc_1_OtherChunk_L1:
    // Load Instance Data
    lbco    acPktInst, cInstBase,  phpContext.instOffset, SIZE(acPktInst)
    
    .using pktDescScope

    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, acPktInst.nextEngId
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2  
    
    // Advance the window to the beginning of packet 
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4      
    mov     acFromAirPass1Loc1.pktLoc,  0   // for non-SOP data chunk
 
    .leave pktDescScope
    
    // pass through
    
phpAcFromAirProc_1_PADDING_L1:
    
    //ICV padding processing 
    qbbc    phpAcFromAirProc_1_End, acPktInst.ctrlFlag.acPaddingInserted
    qbbs    phpAcFromAirProc_1_End, acPktInst.ctrlFlag.acPaddingComplete
    qbgt    phpAcFromAirProc_1_PADDING_L3, phpContext.chunkSize, acPktInst.icvOffset 
    
    // ICV location is within this data chunk
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, acPktInst.icvOffset, acFromAirPass1Loc1.pktLoc
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // Insert the padding based on the dir
    // Inset the 8-byte padding 
    zero    &CdeCmd, 8
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (8 << 8)
    mov     CdeCmd.Cmd1.b3, 0x40
    qbbc    phpAcFromAirProc_1_PADDING_L2, ctxACFromAir.ctrlInfo.flagACDirDownlink
    mov     CdeCmd.Cmd1.b3, 0xC0
    
phpAcFromAirProc_1_PADDING_L2:
    xout    XID_CDECTRL, CdeCmd, 8 
    set     acPktInst.ctrlFlag.acPaddingComplete
    jmp     phpAcFromAirProc_1_End
    
phpAcFromAirProc_1_PADDING_L3: 
    sub    acPktInst.icvOffset,  acPktInst.icvOffset,  phpContext.chunkSize
    //pass through  
 
phpAcFromAirProc_1_End:
    //
    // Firmware workaround: all packets should enter PHP Pass2 due to the
    //                      output stream interface 4-byte alignmnet limitation
    // The following code is commented out 
    //
    //qbbs    phpAcFromAirProc_1_End_L1, ctxACFromAir.ctrlInfo.flagACAuth
    //qbbc    phpAcFromAirProc_1_End_L1, phpContext.chunkInfo.chunkFlag_eop
    //
    //  Process EOP Data Chunk when Auth is not enabled
    //
    //add     ctxACFromAir.numPkts,  ctxACFromAir.numPkts, 1
    //clr     phpContext.instMask, phpContext.ctrlInfo
    //jmp     phpAcFromAirProc_1_End_L2
 
phpAcFromAirProc_1_End_L1:
    // Save Instance Data
    sbco    acPktInst, cInstBase, phpContext.instOffset, SIZE(acPktInst)
    
phpAcFromAirProc_1_End_L2:
    // Save the updated security context
    // The statistic update will occur at PHP Pass2 only 
    //xout    XID_SCVDATA,  ctxACFromAir,  SIZE(ctxACFromAir)
    
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4

    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8)
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command    
    
    jmp     MainLoop
         
 
.leave acFromAirPass1Scope
.leave ctxACFromAirScope
.leave commCtxScope    

///******************************************************************************
// * Function:    sap_ac_from_air_proc_pass2
// ******************************************************************************
// * Description: This function perfroms the second pass of the Air Cipher packet
// *              header processing in the from-Air direction as the followings:  
// *  
// *    - Compare the Hash value against the Extracted Hash value 
// *    - Update statistics
// *
// *****************************************************************************/

.struct struct_acFromAirPass2Loc1
    .u8     ctrlFlag            // record the protocol payload Size
#define acleftoverInserted      t0    
    .u8     pktId   
    .u16    psCtrlSize          // PS Info + Control Info Size
    .u16    chunkSize           // adjusted chunk size due to byte insertion
    .u16    pktLoc              // current location from the packet beginning
.ends     

.enter acFromAirPass2Scope
    .assign  struct_acFromAirPass2Loc1,  R1,  R2,  acFromAirPass2Loc1
    .assign  struct_acPktInst,           R22, R25, acPktInst      
.leave acFromAirPass2Scope

phpAcFromAirProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using acFromAirPass2Scope
    .using ctxACFromAirScope
    
    // clear Local Variables (may be just some of them)
    zero    &acFromAirPass2Loc1,    SIZE(acFromAirPass2Loc1)    
    
phpAcFromAirProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAC.countC))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxACFromAir, SIZE(ctxACFromAir)

    // Record packet instance index and load the instance
    mov     acFromAirPass2Loc1.pktId,  PktDesc.PktId
    lbco    acPktInst, cInstBase,  phpContext.instOffset, SIZE(acPktInst)
    
    // The output port is fixed
    mov     PktDesc.SA_NextEngId, acPktInst.nextEngId_pass2
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 1
    mov     acFromAirPass2Loc1.chunkSize,  phpContext.chunkSize 
    
    // Update swInfo1 at SOP
    qbbc    phpAcFromAirProc_2_L1_1, PktDesc.ChunkInfo.chunkFlag_sop
    qbbs    phpAcFromAirProc_2_L1_1, acPktInst.ctrlFlag.acPaddingInserted2   // keep pktId if third pass is required
        mov     PktDesc2.SwInfo1, r29
        xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
phpAcFromAirProc_2_L1_1:    
    //
    // if acPaddingInserted2 flag is set: It is Kasumi F8/F9 with padding
    // - Hash processing should be performed at PASS3
    // - CMAC IV sertion is not true   
    //  
    qbbs    phpAcFromAirProc_2_HashProc_End, acPktInst.ctrlFlag.acPaddingInserted2

    // 
    // Special case: remove inserted IV at the first data chunk only if ICV is not available
    // 
    qbbc    phpAcFromAirProc_2_L2, phpContext.chunkInfo.chunkFlag_sop
    qbbc    phpAcFromAirProc_2_L2, acPktInst.ctrlFlag.acIvInserted
    qble    phpAcFromAirProc_2_HashProc_L1_1, phpContext.chunkSize, acPktInst.icvOffset_pass2
    
    // Advance the window to the IV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, acPktInst.hdrOffset
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // Flushout 8-byte CMAC-IV
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4
    add     acFromAirPass2Loc1.pktLoc,  acPktInst.hdrOffset, 8
    
    jmp     phpAcFromAirProc_2_HashProc_L2
    
 phpAcFromAirProc_2_L2:    
    
 //
 // Hash Processing
 //   
 phpAcFromAirProc_2_HashProc_L1:
    qbbc    phpAcFromAirProc_2_HashProc_End, ctxACFromAir.ctrlInfo.flagACAuth
    //qbbs    phpAcFromAirProc_2_HashProc_End, acPktInst.ctrlFlag.acPktError       //hash failure ==> acHashComplete = 1
    qbbs    phpAcFromAirProc_2_HashProc_End, acPktInst.ctrlFlag.acHashComplete
    qbbs    phpAcFromAirProc_2_HashProc_L3,  acPktInst.ctrlFlag.acHashAvail  
    qbgt    phpAcFromAirProc_2_HashProc_L2,  phpContext.chunkSize, acPktInst.icvOffset_pass2
    
phpAcFromAirProc_2_HashProc_L1_1:    
    .leave pktDescScope
    
    //
    // Hash data is available in the PS Info section
    //
    set     acPktInst.ctrlFlag.acHashAvail
    
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and store the Hash Data
    mov     r0.b0, ctxACFromAir.icvSize
    xin     XID_CDEDATA,  r6,  b0
    add     r0.w2, phpContext.instOffset,  AC_PKT_INST_HASH_DATA_OFFSET
    sbco    r6, cInstBase, r0.w2, b0
    jmp     phpAcFromAirProc_2_HashProc_L3    
    
 phpAcFromAirProc_2_HashProc_L2:
    sub     acPktInst.icvOffset_pass2,  acPktInst.icvOffset_pass2, phpContext.chunkSize 
    jmp     phpAcFromAirProc_2_HashProc_End 
    
 phpAcFromAirProc_2_HashProc_L3: 
    // Hash Data is at this data chunk   
    // Advance the window to the icv location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4

    // verify whether we need to remove inserted IV
    // it is applicable for SOP only (special case for single-chunk packet since we need to record auth tag in this case)
    qbbc    phpAcFromAirProc_2_HashProc_L3_2, phpContext.chunkInfo.chunkFlag_sop
    qbbc    phpAcFromAirProc_2_HashProc_L3_2, acPktInst.ctrlFlag.acIvInserted
    
        // mov to inserted IV location
        mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
        mov     CdeCmd.Cmd0.w2, acPktInst.hdrOffset
        xout    XID_CDECTRL, CdeCmd, 4
    
        // Flushout 8-byte CMAC-IV   
        mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
        mov     CdeCmd.Cmd0.w2, 8
        xout    XID_CDECTRL, CdeCmd, 4
    add     acFromAirPass2Loc1.pktLoc,  acPktInst.hdrOffset, 8
    
phpAcFromAirProc_2_HashProc_L3_2:    
    qbbs    phpAcFromAirProc_2_HashProc_L5,  acPktInst.ctrlFlag.acHashPending

    // mov to hash location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, acPktInst.icvOffset_pass2, acFromAirPass2Loc1.pktLoc
    xout    XID_CDECTRL, CdeCmd, 4   
    mov     acFromAirPass2Loc1.pktLoc, acPktInst.icvOffset_pass2
    
    qbbc    phpAcFromAirProc_2_HashProc_L3_3,  acPktInst.ctrlFlag.acPaddingInserted
    // flush out Kasumi F9 8-byte padding
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4
    add     acFromAirPass2Loc1.pktLoc,  acFromAirPass2Loc1.pktLoc, 8
    sub     phpContext.chunkSize, phpContext.chunkSize, 8
     
phpAcFromAirProc_2_HashProc_L3_3:     
    sub     r0.w0,  phpContext.chunkSize, acPktInst.icvOffset_pass2
    min     acPktInst.hashRecorded, r0.w0, ctxACFromAir.icvSize
    
    // Load the hash data
    mov     r0.b0, acPktInst.hashRecorded 
    xin     XID_CDEDATA, r6, b0
    
    // Do we have the complete Hash
    qbeq    phpAcFromAirProc_2_HashProc_L4, acPktInst.hashRecorded, ctxACFromAir.icvSize
    // Copy the hash data to the instance buffer
    add     r0.w2,  phpContext.instOffset, AC_PKT_INST_HASH_DATA_OFFSET2
    sbco    r6, cInstBase,  r0.w2, b0 
    mov     acPktInst.icvOffset_pass2, 0
    set     acPktInst.ctrlFlag.acHashPending
    jmp     phpAcFromAirProc_2_HashProc_End
    
 phpAcFromAirProc_2_HashProc_L4:
    // Hash Data is received in the same data chunk
    set     acPktInst.ctrlFlag.acHashComplete
    // Compare with the recorded one
    lbco  r10, cInstBase, r0.w2, b0
    jmp   phpAcFromAirProc_2_HashProc_L6  
    
 phpAcFromAirProc_2_HashProc_L5:
    //  Processing Pending Hash data  (pktLoc = 0)
    set     acPktInst.ctrlFlag.acHashComplete
    
    sub     r0.b0, ctxACFromAir.icvSize, acPktInst.hashRecorded 
    xin     XID_CDEDATA, r6, b0
    add     r0.w2,  phpContext.instOffset, AC_PKT_INST_HASH_DATA_OFFSET2
    add     r29.w2,  r0.w2, acPktInst.hashRecorded
    sbco    r6, cInstBase,  r29.w2, b0 
    mov     r0.b0, ctxACFromAir.icvSize
    lbco    r6, cInstBase, r0.w2, b0
    
    add     r0.w2,  phpContext.instOffset, AC_PKT_INST_HASH_DATA_OFFSET
    lbco    r10, cInstBase, r0.w2, b0
    
 phpAcFromAirProc_2_HashProc_L6:
    // Hash Comparison   
    mov     r29.b0, ctxACFromAir.icvSize
    call    AC_ICVCheck
    qbeq    phpAcFromAirProc_2_Err_Hash, r29.b0, 0
    
 phpAcFromAirProc_2_HashProc_End:
 
    //ICV padding processing 
    // Kasumi F8/F9 with padding at the second pass
    qbbc    phpAcFromAirProc_2_PADDING_end, acPktInst.ctrlFlag.acPaddingInserted2
    qbbs    phpAcFromAirProc_2_L5, acPktInst.ctrlFlag.acPaddingComplete
    qbgt    phpAcFromAirProc_2_PADDING_L3, phpContext.chunkSize, acPktInst.icvOffset_pass2 
    
    // Move to packet
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // ICV location is within this data chunk
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, acPktInst.icvOffset_pass2, acFromAirPass2Loc1.pktLoc
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // Insert the padding based on the dir
    // Inset the 8-byte padding 
    zero    &CdeCmd, 8
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (8 << 8)
    mov     CdeCmd.Cmd1.b3, 0x40
    qbbc    phpAcFromAirProc_2_PADDING_L2, ctxACFromAir.ctrlInfo.flagACDirDownlink
    mov     CdeCmd.Cmd1.b3, 0xC0
    
phpAcFromAirProc_2_PADDING_L2:
    xout    XID_CDECTRL, CdeCmd, 8 
    set     acPktInst.ctrlFlag.acPaddingComplete
    jmp     phpAcFromAirProc_2_L5
    
phpAcFromAirProc_2_PADDING_L3: 
    sub    acPktInst.icvOffset_pass2,  acPktInst.icvOffset_pass2,  phpContext.chunkSize
    jmp    phpAcFromAirProc_2_L5
 
phpAcFromAirProc_2_PADDING_end:
 
    qbbc    phpAcFromAirProc_2_L5,  phpContext.chunkInfo.chunkFlag_eop
        //
        //  Process EOP Data Chunk
        //
        //qbbs    phpAcFromAirProc_2_L4_ERR, acPktInst.ctrlFlag.acPktError
        mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
        xout    XID_SCVCTRL, CdeCmd, 4        
        xin     XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
        add     ctxACStats.numPkts,   ctxACStats.numPkts, 1
        adc     ctxACStats.numPktsHi, ctxACStats.numPktsHi, 0
        xout    XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
    
        // Update System Statistics
        lbco    r4, cStats, PDSP_AC_DEC_HI_OFFSET, 8
        add     r5, r5, 1
        adc     r4, r4, 0
        sbco    r4, cStats, PDSP_AC_DEC_HI_OFFSET, 8
    
 phpAcFromAirProc_2_L4:  
        clr     phpContext.instMask, acFromAirPass2Loc1.pktId
        jmp     phpAcFromAirProc_2_End
   
    
 phpAcFromAirProc_2_L5:
    //
    //  Process non-EOP data chunk
    // 
    //  Save the instance Data
    sbco    acPktInst, cInstBase, phpContext.instOffset, SIZE(acPktInst)
    
 phpAcFromAirProc_2_End:
// #define HASH_DEBUG 
#ifdef HASH_DEBUG
    qbbs    phpAcFromAirProc_2_L4_ERR, acPktInst.ctrlFlag.acPktError
#endif    
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop
    
    
phpAcFromAirProc_2_L4_ERR:    
 #ifdef HASH_DEBUG
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    halt
 
 #endif
    
    
    
 phpAcFromAirProc_2_Err_Hash:
    // Update the channel statistics 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
    add     ctxACStats.numHashFails,   ctxACStats.numHashFails, 1
    xout    XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
    
    // Update system statistics
    lbco    r5, cStats, PDSP_AC_AUTH_FAIL_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_AC_AUTH_FAIL_OFFSET, 4
    
    // 
    // Update the packet descriptor
    // Set Error Code
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket
    
    mov     r0.b0, (SA_PKT_ERR_AUTH_FAIL << 4)
    sbco    r0.b0, cCdeOutPkt, OFFSET(struct_PktDesc.ChunkInfoPS), SIZE(struct_PktDesc.ChunkInfoPS) 
    
    // Indicate that Packet error occurred
    set     acPktInst.ctrlFlag.acPktError 
    
    qbbs    phpAcFromAirProc_2_L4,  phpContext.chunkInfo.chunkFlag_eop    // EOP processing
    jmp     phpAcFromAirProc_2_L5                                         // non-EOP processing
    
    
 //phpAcFromAirProc_2_Err_Hash_OtherChunk:   
 //   qbbc    phpAcFromAirProc_2_L4, phpContext.chunkInfo.chunkFlag_eop
    
    // EOP packet: clear the instance
 //   clr     phpContext.instMask, acFromAirPass2Loc1.pktId
       
 //   jmp     phpAcFromAirProc_2_End  

.leave acFromAirPass2Scope
.leave ctxACFromAirScope
.leave commCtxScope    

///******************************************************************************
// * Function:    sap_ac_from_air_proc_pass3
// ******************************************************************************
// * Description: This function perfroms the second pass of the Air Cipher packet
// *              header processing in the from-Air direction as the followings:  
// *  
// *    - Remove the Kasumi-F9 padding 
// *    - Compare the Hash value against the Extracted Hash value 
// *    - Update statistics
// *
// * Note: Third pass is only required for Kasumi F8/F9 with F9 Padding
// *****************************************************************************/

.struct struct_acFromAirPass3Loc1
    .u8     ctrlFlag            // record the protocol payload Size
    .u8     pktId   
    .u16    psCtrlSize          // PS Info + Control Info Size
    .u16    chunkSize           // adjusted chunk size due to byte insertion
    .u16    pktLoc              // current location from the packet beginning
.ends     

.enter acFromAirPass3Scope
    .assign  struct_acFromAirPass3Loc1,  R1,  R2,  acFromAirPass3Loc1
    .assign  struct_acPktInst,           R22, R25, acPktInst      
.leave acFromAirPass3Scope

phpAcFromAirProc_3:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using acFromAirPass3Scope
    .using ctxACFromAirScope
    
    // clear Local Variables (may be just some of them)
    zero    &acFromAirPass3Loc1,    SIZE(acFromAirPass3Loc1)    
    
phpAcFromAirProc_3_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAC.countC))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxACFromAir, SIZE(ctxACFromAir)

    // Record packet instance index and load the instance
    mov     acFromAirPass3Loc1.pktId,  PktDesc.PktId
    lbco    acPktInst, cInstBase,  phpContext.instOffset, SIZE(acPktInst)
    
    // The output port is fixed
    mov     PktDesc.SA_NextEngId, SAENG_OUT2_1
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 1
    mov     acFromAirPass3Loc1.chunkSize,  phpContext.chunkSize 
    
    // Update swInfo1 at SOP
    qbbc    phpAcFromAirProc_3_L2, PktDesc.ChunkInfo.chunkFlag_sop
        mov     PktDesc2.SwInfo1, r29
        xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    
 phpAcFromAirProc_3_L2:    
    
 //
 // Hash Processing
 //   
 phpAcFromAirProc_3_HashProc_L1:
    // Auth must enbaled to reach PHP Pass 3
    //qbbc    phpAcFromAirProc_3_HashProc_End, ctxACFromAir.ctrlInfo.flagACAuth
    qbbs    phpAcFromAirProc_3_HashProc_End, acPktInst.ctrlFlag.acHashComplete
    qbbs    phpAcFromAirProc_3_HashProc_L3,  acPktInst.ctrlFlag.acHashAvail  
    qbgt    phpAcFromAirProc_3_HashProc_L2,  phpContext.chunkSize, acPktInst.icvOffset_pass3
    
phpAcFromAirProc_3_HashProc_L1_1:    
    .leave pktDescScope
    
    //
    // Hash data is available in the PS Info section
    //
    set     acPktInst.ctrlFlag.acHashAvail
    
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and store the Hash Data
    mov     r0.b0, ctxACFromAir.icvSize
    xin     XID_CDEDATA,  r6,  b0
    add     r0.w2, phpContext.instOffset,  AC_PKT_INST_HASH_DATA_OFFSET
    sbco    r6, cInstBase, r0.w2, b0
    jmp     phpAcFromAirProc_3_HashProc_L3    
    
 phpAcFromAirProc_3_HashProc_L2:
    sub     acPktInst.icvOffset_pass3,  acPktInst.icvOffset_pass3, phpContext.chunkSize 
    jmp     phpAcFromAirProc_3_HashProc_End 
    
 phpAcFromAirProc_3_HashProc_L3: 
    // Hash Data is at this data chunk   
    // Advance the window to the icv location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
phpAcFromAirProc_3_HashProc_L3_2:    
    qbbs    phpAcFromAirProc_3_HashProc_L5,  acPktInst.ctrlFlag.acHashPending

    // mov to hash location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, acPktInst.icvOffset_pass3, acFromAirPass3Loc1.pktLoc
    xout    XID_CDECTRL, CdeCmd, 4   
    mov     acFromAirPass3Loc1.pktLoc, acPktInst.icvOffset_pass3
    
    // Check is not necessary since this is the only condition PHP PASS3 will be reached
    //qbbc    phpAcFromAirProc_3_HashProc_L3_3,  acPktInst.ctrlFlag.acPaddingInserted2
    // flush out Kasumi F9 8-byte padding
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4
    add     acFromAirPass3Loc1.pktLoc,  acFromAirPass3Loc1.pktLoc, 8
    sub     phpContext.chunkSize, phpContext.chunkSize, 8
     
phpAcFromAirProc_3_HashProc_L3_3:     
    sub     r0.w0,  phpContext.chunkSize, acPktInst.icvOffset_pass3
    min     acPktInst.hashRecorded, r0.w0, ctxACFromAir.icvSize
    
    // Load the hash data
    mov     r0.b0, acPktInst.hashRecorded 
    xin     XID_CDEDATA, r6, b0
    
    // Do we have the complete Hash
    qbeq    phpAcFromAirProc_3_HashProc_L4, acPktInst.hashRecorded, ctxACFromAir.icvSize
    // Copy the hash data to the instance buffer
    add     r0.w2,  phpContext.instOffset, AC_PKT_INST_HASH_DATA_OFFSET2
    sbco    r6, cInstBase,  r0.w2, b0 
    mov     acPktInst.icvOffset_pass3, 0
    set     acPktInst.ctrlFlag.acHashPending
    jmp     phpAcFromAirProc_3_HashProc_End
    
 phpAcFromAirProc_3_HashProc_L4:
    // Hash Data is received in the same data chunk
    set     acPktInst.ctrlFlag.acHashComplete
    // Compare with the recorded one
    lbco  r10, cInstBase, r0.w2, b0
    jmp   phpAcFromAirProc_3_HashProc_L6  
    
 phpAcFromAirProc_3_HashProc_L5:
    //  Processing Pending Hash data  (pktLoc = 0)
    set     acPktInst.ctrlFlag.acHashComplete
    
    sub     r0.b0, ctxACFromAir.icvSize, acPktInst.hashRecorded 
    xin     XID_CDEDATA, r6, b0
    add     r0.w2,  phpContext.instOffset, AC_PKT_INST_HASH_DATA_OFFSET2
    add     r29.w2,  r0.w2, acPktInst.hashRecorded
    sbco    r6, cInstBase,  r29.w2, b0 
    mov     r0.b0, ctxACFromAir.icvSize
    lbco    r6, cInstBase, r0.w2, b0
    
    add     r0.w2,  phpContext.instOffset, AC_PKT_INST_HASH_DATA_OFFSET
    lbco    r10, cInstBase, r0.w2, b0
    
 phpAcFromAirProc_3_HashProc_L6:
    // Hash Comparison   
    mov     r29.b0, ctxACFromAir.icvSize
    call    AC_ICVCheck
    qbeq    phpAcFromAirProc_3_Err_Hash, r29.b0, 0
    
 phpAcFromAirProc_3_HashProc_End:
 
    qbbc    phpAcFromAirProc_3_L5,  phpContext.chunkInfo.chunkFlag_eop
    //
    //  Process EOP Data Chunk
    //
    qbbs    phpAcFromAirProc_3_L4, acPktInst.ctrlFlag.acPktError
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
    add     ctxACStats.numPkts,   ctxACStats.numPkts, 1
    adc     ctxACStats.numPktsHi, ctxACStats.numPktsHi, 0
    xout    XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_AC_DEC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_AC_DEC_HI_OFFSET, 8
    
 phpAcFromAirProc_3_L4:  
    clr     phpContext.instMask, acFromAirPass3Loc1.pktId
    jmp     phpAcFromAirProc_3_End
   
    
 phpAcFromAirProc_3_L5:
    //
    //  Process non-EOP data chunk
    // 
    //  Save the instance Data
    sbco    acPktInst, cInstBase, phpContext.instOffset, SIZE(acPktInst)
    
 phpAcFromAirProc_3_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop
    
    
 phpAcFromAirProc_3_Err_Hash:
    // Update the channel statistics 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
    add     ctxACStats.numHashFails,   ctxACStats.numHashFails, 1
    xout    XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
    
    // Update system statistics
    lbco    r5, cStats, PDSP_AC_AUTH_FAIL_OFFSET, 4
    add     r5, r5, 1
    sbco    r5, cStats, PDSP_AC_AUTH_FAIL_OFFSET, 4
    
    // 
    // Update the packet descriptor
    // Set Error Code
    //
    
    // Make sure the packet desciptor is ready to be patched (non-held packet)
    wbs     r31.tStatus_CDEOutPacket
    
    mov     r0.b0, (SA_PKT_ERR_AUTH_FAIL << 4)
    sbco    r0.b0, cCdeOutPkt, OFFSET(struct_PktDesc.ChunkInfoPS), SIZE(struct_PktDesc.ChunkInfoPS) 
    
    // Indicate that Packet error occurred
    set     acPktInst.ctrlFlag.acPktError 
    
    qbbs    phpAcFromAirProc_3_L4,  phpContext.chunkInfo.chunkFlag_eop    // EOP processing
    jmp     phpAcFromAirProc_3_L5                                         // non-EOP processing

.leave acFromAirPass3Scope
.leave ctxACFromAirScope
.leave commCtxScope    
 
        
