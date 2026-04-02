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
//*                  | |               | 
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
// *  Ciphering Algorithm: 128-bit Snow 3G F8 ZUC F8 and AES-CTR 
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
// *  Integrity Algorithm: SNOW 3G/F9 ZUC F9 and AES-CMAC
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

.struct struct_acToAirPass1Loc1
    .u16    hashDataLen        // Hash Data length (for command label)
    .u16    encryptedDataLen   // Encrypted Data length (for command label)
    .u16    seqNum             // Sequence Number
    .u16    seqNumMask         // Sequence Number Mask
    .u8     protoOffset        // Protocol Offset
    .u8     hdrSize            // header Size (for command label)
    .u8     cmdlOffset         // Command Label Offset 
    .u8     pktLoc             // packet location
.ends     

.enter acToAirPass1Scope
    .assign  struct_acToAirPass1Loc1,  R1,  R3,  acToAirPass1Loc1
    .assign  struct_acPktInst,         R22, R25, acPktInst      
.leave acToAirPass1Scope

.enter ctxACToAirScope
    .assign  struct_ctxAC1,            R14, R21.w2, ctxACToAir
    .assign  struct_ctxACStats,        R14, R16,    ctxACStats
.leave ctxACToAirScope

.enter acToAirCmdlScope
    .assign  struct_saCmdL,            R10, R11, saCmdL
    .assign  struct_saCmdLReg,         R10, R11, saCmdLReg
    .assign  struct_saCmdlOpt,         R10, R13, saCmdlOpt
.leave acToAirCmdlScope    

///******************************************************************************
// * Function:    sap_ac_to_air_proc_pass1
// ******************************************************************************
// * Description: This function perfroms the first pass of the Air Cipher packet
// *              header processing in the from_air direction as the followings:  
// *
// *  - Extract the sequence number and calculate Count-C (Count-C | sequence number)
// *  - Insert Count-C in front of packet if necessary
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

phpAcToAirProc_1:

    // Error Check
    
    
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using acToAirPass1Scope
    .using ctxACToAirScope
    
    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAC.countC))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    
    xin     XID_SCVDATA, ctxACToAir, SIZE(ctxACToAir)
    
    // clear Local Variables (may be just some of them)
    zero    &acToAirPass1Loc1,    SIZE(acToAirPass1Loc1)    
    
    // Record packet instance index for PASS1 
    // mov     phpContext.ctrlInfo,  PktDesc.PktId
    
    qbbc    phpAcToAirProc_1_OtherChunk_L1,   phpContext.chunkInfo.chunkFlag_sop
    
 phpAcToAirProc_1_FirstChunk_L1:    
 
    //clear the control flags and other variables
    zero    &acPktInst,    SIZE(acPktInst)
    
 phpAcToAirProc_1_FirstChunk_L2:  
    mov     acPktInst.nextEngId, ctxACToAir.firstEngId
    mov     acPktInst.nextEngId_pass2,   SAENG_OUT2_1
    
    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, acPktInst.nextEngId
    set     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    xout    XID_CDEDATA, PktDesc, 8   
      
    // Advance the window past the Descriptor to control info area
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_CONTROL
    xout    XID_CDECTRL, CdeCmd, 4      
      
    .leave  pktDescScope
    .using protoInfoScope
    // Extract protocol Info
    // We need to handle both long and short info
    // Short Info: From Host
    // Long Info: From PA
    // Note: Both payloadInfoL and protoInfo share R6
    xin     XID_CDEDATA, payloadInfoL, SIZE(struct_payloadInfoShort2)

   // Skip logging if not enabled
   qbbc SkipAcDbgLog_4, phpContext.ctrlInfo.t_php_pkt_dbg_info
    // 12 bytes of PktInfo
    // sbco    payloadInfoL, cAcDbgBase, s_acDbg.offset, 12
    mov      r0,  PDSP_ACDBG_BASE
    sbbo    payloadInfoL, r0, s_acDbg.offset, 12
    add     s_acDbg.offset,   s_acDbg.offset,  12
SkipAcDbgLog_4:    

    // extract the command code b7-b5 (0,1) = (long, short info)
    lsr     r0.b0, payloadInfoL.recordLen, 5
    and     CdeCmd.Cmd0.w2, payloadInfoL.recordLen, 0x1F
    qbeq    phpAcToAirProc_ShortInfo_L1, r0.b0, 1
    
phpAcToAirProc_LongInfo_L1:
        mov     acToAirPass1Loc1.protoOffset,  payloadInfoL.startOffset
        sub     acToAirPass1Loc1.encryptedDataLen, payloadInfoL.eopOffset, payloadInfoL.startOffset
        jmp     phpAcToAirProc_protoInfo_End 
    
phpAcToAirProc_ShortInfo_L1:    
        mov     acToAirPass1Loc1.protoOffset,  protoInfo.offset
        mov     acToAirPass1Loc1.encryptedDataLen, protoInfo.len
        qbge    phpAcToAirProc_ShortInfo_L3, CdeCmd.Cmd0.w2,   8 
            // IV present: need to copy IV and count-C into the corresponding buffers
            // Note: count-C should not be stored at the SC since it should not affect the next packet
            set   acPktInst.ctrlFlag.acIvCopied
            mov   acPktInst.countC, payloadInfoS2.countC 
            mov   r0.b1, ctxACToAir.ivSize 
            qbbs  phpAcToAirProc_ShortInfo_L2, ctxACToAir.ctrlInfo.flagACCMAC   
                sbco  payloadInfoS2.iv1, cCmdlBuf, SA_CMDL_HEADER_SIZE_BYTES, b1
                jmp   phpAcToAirProc_protoInfo_End
phpAcToAirProc_ShortInfo_L2:
                // CMAC IV size is 8 bytes
                sbco  payloadInfoS2.iv1, cAuxBuf, 0, 8
                jmp   phpAcToAirProc_protoInfo_End
          
phpAcToAirProc_ShortInfo_L3:          
        mov     CdeCmd.Cmd0.w2, 8
        qbbc    phpAcToAirProc_protoInfo_End, ctxACToAir.ctrlInfo.flagACCountCPresent
            // record Count-C
            // mov ctxACToAir.countC,  payloadInfoS.countC
            // Load one less to application provided Count C, assuming it would be
            // incremented later by 1 to match to the CountC provided by Application
            sub  ctxACToAir.countC, payloadInfoS.countC, 1
        
        //pass through
    
    .leave protoInfoScope
phpAcToAirProc_protoInfo_End:
    
    // Issue command to flash out PSInfo
    // Now we are at the control section. Insert the command label here
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    xout    XID_CDECTRL, CdeCmd, 4      
    
    qbeq    phpAcToAirProc_1_CmdlLen_L2_1, ctxACToAir.cmdlLen, 0
    // Insert room for command label
phpAcToAirProc_1_CmdlLen_L2:    
    mov     CdeCmd.Cmd0.b0, CDE_CMD_INSERT_CONTROL     
    mov     CdeCmd.Cmd0.b1, ctxACToAir.cmdlLen
    xout    XID_CDECTRL, CdeCmd, 4              // Insert all zero for the command label 
    //  pass through
    
phpAcToAirProc_1_CmdlLen_L2_1:    
    
    // Advance the window past the control Info area
    mov     CdeCmd.Cmd0.w0 ,CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4      
    
    // Advance to the AC header only if protoOffset is non-zero
    qbeq    phpAcToAirProc_1_FirstChunk_L3, acToAirPass1Loc1.protoOffset, 0
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, acToAirPass1Loc1.protoOffset
    xout    XID_CDECTRL, CdeCmd, 4   
    
    mov     acToAirPass1Loc1.pktLoc,  acToAirPass1Loc1.protoOffset
   
phpAcToAirProc_1_FirstChunk_L3:  
    // Extract 16 bytes (count-C, header or IV)  
    xin     XID_CDEDATA, r6, 16
    
    // Should we process PDU header (group 1)
    qbbc  phpAcToAirProc_1_HdrProc_L4, ctxACToAir.ctrlInfo.flagACHdrPresent
    // HDR is present, extract the sequence number for the header
    mov   r0.b0, 1
    lsl   r0.w2, r0.b0, ctxACToAir.seqNumSize
    sub   acToAirPass1Loc1.seqNumMask, r0.w2, 1
    qbeq  phpAcToAirProc_1_HdrProc_L1, ctxACToAir.hdrSize, 2
    // Header size is 1, seq_num = (p_data[0] >> p_proto_ctx->seq_num_shift) & seq_num_mask;
    lsr   acToAirPass1Loc1.seqNum, r6.b3, ctxACToAir.seqNumShift
    jmp   phpAcToAirProc_1_HdrProc_L2 
phpAcToAirProc_1_HdrProc_L1:
    // Header size is 2, seq_num = (utlRead16bits_m(p_data, 0) >>  p_proto_ctx->seq_num_shift) & seq_num_mask;
    lsr   acToAirPass1Loc1.seqNum, r6.w2, ctxACToAir.seqNumShift
phpAcToAirProc_1_HdrProc_L2:
    and   acToAirPass1Loc1.seqNum, acToAirPass1Loc1.seqNum, acToAirPass1Loc1.seqNumMask
    
    not   r29, acToAirPass1Loc1.seqNumMask
    and   r0.w0,  ctxACToAir.countC.w0, acToAirPass1Loc1.seqNumMask
    qble  phpAcToAirProc_1_HdrProc_L3, acToAirPass1Loc1.seqNum, r0.w0
    add   ctxACToAir.countC, ctxACToAir.countC, r0.w2 
phpAcToAirProc_1_HdrProc_L3:      
    and   ctxACToAir.countC, ctxACToAir.countC, r29
    or    ctxACToAir.countC.w0, ctxACToAir.countC.w0, acToAirPass1Loc1.seqNum
    mov   acToAirPass1Loc1.hdrSize, ctxACToAir.hdrSize
     jmp   phpAcToAirProc_1_HdrProc_End 
    
phpAcToAirProc_1_HdrProc_L4:
    // Should we insert Count-C: group 2
    qbbc  phpAcToAirProc_1_HdrProc_L5, ctxACToAir.ctrlInfo.flagACCountCInsert   
        // Compensate for the encryption size calculation - hdrSize
        add   acToAirPass1Loc1.encryptedDataLen, acToAirPass1Loc1.encryptedDataLen, 4
        mov   acToAirPass1Loc1.hdrSize, 4
        // Inset Count-C to the packet
        mov   CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (4 << 8)
    
        qbbc  phpAcToAirProc_1_HdrProc_L4_1, acPktInst.ctrlFlag.acIvCopied
            mov   CdeCmd.Cmd1, acPktInst.countC
            jmp   phpAcToAirProc_1_HdrProc_L4_3   
phpAcToAirProc_1_HdrProc_L4_1:    
           // Below is commented to make the Count C to increment by 1 always
           // qbbs  phpAcToAirProc_1_HdrProc_L4_2, ctxACToAir.ctrlInfo.flagACCountCPresent
                add   ctxACToAir.countC, ctxACToAir.countC, 1
                // pass through
                
phpAcToAirProc_1_HdrProc_L4_2:    
            mov   CdeCmd.Cmd1, ctxACToAir.countC
    
phpAcToAirProc_1_HdrProc_L4_3:    
    xout  XID_CDECTRL, CdeCmd, 8 
    jmp   phpAcToAirProc_1_HdrProc_End 
     
phpAcToAirProc_1_HdrProc_L5:
    // No special Processing: Group 3
    // qbbs  phpAcToAirProc_1_HdrProc_End, ctxACToAir.ctrlInfo.flagACCountCPresent
    qbbs  phpAcToAirProc_1_HdrProc_End, acPktInst.ctrlFlag.acIvCopied
        add   ctxACToAir.countC, ctxACToAir.countC, 1
        //mov   acToAirPass1Loc1.hdrSize, 0

phpAcToAirProc_1_HdrProc_End:
    // Update Count-C
    xout    XID_SCVDATA,  ctxACToAir.countC,  4
    qbbc    phpAcToAirProc_1_HdrProc_End_L1,  ctxACToAir.ctrlInfo2.flagACCountCCopy
        // Copy coutC to timestamp field
        wbc   r31.tStatus_CDEBusy
        qbbc  phpAcToAirProc_1_HdrProc_End_1, acPktInst.ctrlFlag.acIvCopied
            sbco  acPktInst.countC,  cCdeOutPkt, OFFSET(struct_PktDesc.Timestamp),  SIZE(struct_PktDesc.Timestamp)
            jmp   phpAcToAirProc_1_HdrProc_End_L1
phpAcToAirProc_1_HdrProc_End_1:        
            sbco  ctxACToAir.countC,  cCdeOutPkt, OFFSET(struct_PktDesc.Timestamp),  SIZE(struct_PktDesc.Timestamp)
        // pass through 
    
phpAcToAirProc_1_HdrProc_End_L1:
    
    // Is IV present in the payload, Group 4
    qbbc  phpAcToAirProc_1_FirstChunk_L4, ctxACToAir.ctrlInfo.flagACIVPresent
    // the IV should be provided in GSM modes
    // Note: We do not support both encryption and authentication in this case.
    add   acToAirPass1Loc1.hdrSize, acToAirPass1Loc1.hdrSize, ctxACToAir.ivSize
    // Store the IV in the command label table if not CMAC
    // Redundant check: CMAC is not used for GSM 
    qbbs  phpAcToAirProc_1_FirstChunk_L4, ctxACToAir.ctrlInfo.flagACCMAC   
        mov   r0.b1, ctxACToAir.ivSize 
        sbco  r6, cCmdlBuf, SA_CMDL_HEADER_SIZE_BYTES, b1
    
phpAcToAirProc_1_FirstChunk_L4:  
    
    // Calculate the payload related information
    add     acPktInst.icvOffset_pass2, acToAirPass1Loc1.protoOffset, acToAirPass1Loc1.encryptedDataLen
    mov     acPktInst.icvOffset, acPktInst.icvOffset_pass2
    qbbc    phpAcToAirProc_1_FirstChunk_L4_0, ctxACToAir.ctrlInfo.flagACCountCInsert
        // adjust the icvOffset since Count-C insertion does not affect the current chunkSize
        sub     acPktInst.icvOffset, acPktInst.icvOffset, 4
    
phpAcToAirProc_1_FirstChunk_L4_0:    
    add     acPktInst.hdrOffset, acToAirPass1Loc1.protoOffset, acToAirPass1Loc1.hdrSize   
    sub     acToAirPass1Loc1.hashDataLen,  acToAirPass1Loc1.encryptedDataLen, acToAirPass1Loc1.hdrSize
    add     acToAirPass1Loc1.encryptedDataLen, acToAirPass1Loc1.hashDataLen, ctxACToAir.icvSize
    sub     acToAirPass1Loc1.encryptedDataLen, acToAirPass1Loc1.encryptedDataLen, ctxACToAir.authHdrSize

    // Verify whether Kasumi F9 padding is necessary
    qbbc    phpAcToAirProc_1_FirstChunk_L4_1, ctxACToAir.ctrlInfo.flagACKasumiF9
    and     r0.w0,  acToAirPass1Loc1.hashDataLen, 0x7
    qbne    phpAcToAirProc_1_CmdlabelProc_L1, r0.w0, 0 
    set     acPktInst.ctrlFlag.acPaddingInserted
    add     acToAirPass1Loc1.hashDataLen, acToAirPass1Loc1.hashDataLen, 8
    // pass through since KasumiF9 excludes CMAC, save an instruction with the same cycle
    
phpAcToAirProc_1_FirstChunk_L4_1:    
    // CMAC special processing if IV is not present in the packet already:
    // - move to the hdr offset
    // - reserve room for IV insertion
    qbbc    phpAcToAirProc_1_CmdlabelProc_L1, ctxACToAir.ctrlInfo.flagACCMAC
        // CMAC IV size is 8
        add     acToAirPass1Loc1.hashDataLen, acToAirPass1Loc1.hashDataLen, 8
        qbbs    phpAcToAirProc_1_CmdlabelProc_L1, ctxACToAir.ctrlInfo.flagACIVPresent
    
        // Verify whether it is already at the right location
        qbbs    phpAcToAirProc_1_FirstChunk_L4_2, ctxACToAir.ctrlInfo.flagACCountCInsert   
            mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
            mov     CdeCmd.Cmd0.w2, acToAirPass1Loc1.hdrSize
            xout    XID_CDECTRL, CdeCmd, 4      
    
phpAcToAirProc_1_FirstChunk_L4_2:   
    qbbs    phpAcToAirProc_1_FirstChunk_L4_3, acPktInst.ctrlFlag.acIvCopied  // Skip IV formatting operation
        // Format and insert IV R10-R13
        // CMAC IV size is 8
        call    AC_formIVAuth
        sbco    r10,  cAuxBuf, 0, 8
    
phpAcToAirProc_1_FirstChunk_L4_3:    
        mov     CdeCmd.Cmd0, CDE_CMD_INSERT_PACKET_BUFFER
        mov     CdeCmd.Cmd0.b1, 8 
        mov     CdeCmd.Cmd1, PHP_AUX_BUF_ADDR
        xout    XID_CDECTRL, CdeCmd, 8

        //Adjust length information
        add     acPktInst.icvOffset_pass2, acPktInst.icvOffset_pass2, 8
        set     acPktInst.ctrlFlag.acIvInserted
        qbbs    phpAcToAirProc_1_CmdlabelProc_L1, ctxACToAir.ctrlInfo.flagACCountCInsert 
            // Note: the pktLoc does not change if Count-C is inserted  
            add     acToAirPass1Loc1.pktLoc,  acToAirPass1Loc1.protoOffset, acToAirPass1Loc1.hdrSize

        //  Command Label Processing
        //
        .using acToAirCmdlScope  
       
 phpAcToAirProc_1_CmdlabelProc_L1:
#ifdef SASS_GEN2 
    qbbc    phpAcToAirProc_1_CmdlabelProc_L1_gen,   ctxACToAir.ctrlInfo.flagACF8F9
        zero &saCmdL, SIZE(saCmdL)
        // Prepare F8F9 Command Label
        //mov     saCmdL.cmdlLen,    SA_CMDL_HEADER_SIZE_BYTES + 48
        //mov     saCmdL.nextEngId,  SAENG_PHP_2
        mov     saCmdLReg.reg1.w2,   (SA_CMDL_HEADER_SIZE_BYTES + 48) | (SAENG_PHP_2 << 8)
        mov     saCmdL.procLen,    acToAirPass1Loc1.hashDataLen
        mov     saCmdL.sopOffset,  acToAirPass1Loc1.protoOffset
        //mov	    saCmdL.optctrl1,   SA_3GPP_IV1_CMDL_OPT
        //mov	    saCmdL.optctrl2,   SA_3GPP_AUX1_CMDL_OPT
        mov	    saCmdLReg.reg2.w1,  (SA_3GPP_IV1_CMDL_OPT << 8) + SA_3GPP_AUX1_CMDL_OPT
	    qbbc	phpAcToAirProc_1_CmdlabelProc_F8F9_L1, ctxACToAir.ctrlInfo.flagACKeyInScratch
            mov 	saCmdL.optctrl3,   SA_3GPP_KEY_CMDL_OPT
            add     saCmdL.cmdlLen, saCmdL.cmdlLen, 32
                
phpAcToAirProc_1_CmdlabelProc_F8F9_L1: 
        // Store the command label
        sbco    saCmdL, cCmdlBuf,   acToAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
        add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
        // Encryption IV processing: Get and store the IV
        qbbs    phpAcToAirProc_1_CmdlabelProc_F8F9_L2, ctxACToAir.ctrlInfo.flagACIVPresent
    
            // copy IV to the command label location
            call    AC_formIVEnc
            mov     r0.b0, ctxACToAir.ivSize 
            sbco    saCmdlOpt.data1,  cCmdlBuf, acToAirPass1Loc1.cmdlOffset, b0

phpAcToAirProc_1_CmdlabelProc_F8F9_L2:
        // IV has been copied in both cases
        add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, ctxACToAir.ivSize
        
        // IV processing: Get and store the IV
        qbbs    phpAcToAirProc_1_CmdlabelProc_F8F9_L3, ctxACToAir.ctrlInfo.flagACIVPresent
    
            // copy Authentication IV to the command label location
            call    AC_formIVAuth
            mov     r0.b0, ctxACToAir.ivSize 
            sbco    saCmdlOpt.data1,  cCmdlBuf, acToAirPass1Loc1.cmdlOffset, b0
        
phpAcToAirProc_1_CmdlabelProc_F8F9_L3:  
        // IV has been copied in both cases
        add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, ctxACToAir.ivSize
        
        // Copy Miscellaneous data
        zero    &saCmdlOpt,     SIZE(saCmdlOpt)
        mov     saCmdlOpt.data1.b3, ctxACToAir.ivOpt
        mov     saCmdlOpt.data4.w0, acToAirPass1Loc1.hashDataLen 
        sbco    saCmdlOpt.data1,  cCmdlBuf, acToAirPass1Loc1.cmdlOffset, 16
        add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, 16
        
	    // Check if mac key needs to be copied
	    qbbc	phpAcToAirProc_1_CmdlabelProc_End, ctxACToAir.ctrlInfo.flagACKeyInScratch
	        // Copy enc key
            qbbs    phpAcToAirProc_1_encKey_copy_1, ctxACToAir.encKeyOffset.t_flag_key_at_2nd_block
	            lbco	saCmdlOpt.data1,  cKeyRamBase, ctxACToAir.encKeyOffset,     16
                jmp     phpAcToAirProc_1_encKey_copy_end
phpAcToAirProc_1_encKey_copy_1:
                clr     ctxACToAir.encKeyOffset.t_flag_key_at_2nd_block
	            lbco	saCmdlOpt.data1,  cKeyRamBase2, ctxACToAir.encKeyOffset,    16
phpAcToAirProc_1_encKey_copy_end:                
	        sbco    saCmdlOpt.data1,  cCmdlBuf,  acToAirPass1Loc1.cmdlOffset, 16
	        add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, 16
            
            // Copy mac key
            qbbs    phpAcToAirProc_1_macKey_copy_1, ctxACToAir.macKeyOffset.t_flag_key_at_2nd_block
	            lbco	saCmdlOpt.data1,  cKeyRamBase, ctxACToAir.macKeyOffset,    16
                jmp     phpAcToAirProc_1_macKey_copy_end
phpAcToAirProc_1_macKey_copy_1:
                clr     ctxACToAir.macKeyOffset.t_flag_key_at_2nd_block
                lbco	saCmdlOpt.data1,  cKeyRamBase2, ctxACToAir.macKeyOffset,   16
phpAcToAirProc_1_macKey_copy_end:            
	        sbco    saCmdlOpt.data1,  cCmdlBuf,    acToAirPass1Loc1.cmdlOffset, 16
	        add     acToAirPass1Loc1.cmdlOffset,   acToAirPass1Loc1.cmdlOffset, 16
 
            jmp phpAcToAirProc_1_CmdlabelProc_End
         
phpAcToAirProc_1_CmdlabelProc_L1_gen:
#endif 
    // skip command processing if command length == 0
    qbeq phpAcToAirProc_1_PADDING_L1, ctxACToAir.cmdlLen, 0
    zero &saCmdL, SIZE(saCmdL)
    
    //
    // Note: The LLD will ensure that the encryption and authentication does not occur at the same time
    //       due to hardware limitation
    
    qbbc  phpAcToAirProc_1_CmdlabelProc_L4, ctxACToAir.ctrlInfo.flagACAuth
        // Prepare Authentication Command Label
        mov     saCmdL.cmdlLen,    SA_CMDL_HEADER_SIZE_BYTES
        mov     saCmdL.nextEngId,  SAENG_PHP_2
        mov     saCmdL.procLen,    acToAirPass1Loc1.hashDataLen
        add     saCmdL.sopOffset,  acToAirPass1Loc1.protoOffset, acToAirPass1Loc1.hdrSize
        
        // Check to see if need to copy enc key from scratch
	    // Note: assume always do either enc or mac, never combined operation
	    qbbc	phpAcToAirProc_1_CmdlabelProc_L1_Snow3G_check2, ctxACToAir.ctrlInfo.flagACKeyInScratch
	        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
            qbbc    phpAcToAirProc_1_CmdlabelProc_L1_KeyOpt1, ctxACToAir.ctrlInfo.flagACSnow3GZuc
                mov	saCmdL.optctrl2,   SA_3GPP_KEY2_CMDL_OPT
                jmp phpAcToAirProc_1_CmdlabelProc_L1_KeyOptEnd
phpAcToAirProc_1_CmdlabelProc_L1_KeyOpt1:                 
                mov	saCmdL.optctrl2,   SA_3GPP_KEY1_CMDL_OPT
phpAcToAirProc_1_CmdlabelProc_L1_KeyOptEnd:                
	        // Check to see if need to create modkey for kasumi
	        qbbc	phpAcToAirProc_1_CmdlabelProc_L1_Snow3G_check, ctxACToAir.ctrlInfo.flagACKasumiF9
                mov		saCmdL.optctrl3,   SA_3GPP_MOD_KEY_CMDL_OPT
	            add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
                jmp     phpAcToAirProc_1_CmdlabelProc_L1_1
                
phpAcToAirProc_1_CmdlabelProc_L1_Snow3G_check:  
#ifdef SASS_GEN2              
            // Check to see if Snow3G/Zuc operation is required
	        qbbc	phpAcToAirProc_1_CmdlabelProc_L1_1, ctxACToAir.ctrlInfo.flagACSnow3GZuc
                mov		saCmdL.optctrl3,   SA_3GPP_MISC_CMDL_OPT
	            add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
#endif                
                jmp     phpAcToAirProc_1_CmdlabelProc_L1_1

phpAcToAirProc_1_CmdlabelProc_L1_Snow3G_check2:
#ifdef SASS_GEN2
            // Check to see if Snow3G/Zuc operation is required
	        qbbc	phpAcToAirProc_1_CmdlabelProc_L1_1, ctxACToAir.ctrlInfo.flagACSnow3GZuc
                mov		saCmdL.optctrl2,   SA_3GPP_MISC_CMDL_OPT
	            add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
                // pass through
#endif	
phpAcToAirProc_1_CmdlabelProc_L1_1:
    qbbs    phpAcToAirProc_1_CmdlabelProc_L3, ctxACToAir.ctrlInfo.flagACCMAC
    
        //non-CMAC operation
        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, ctxACToAir.ivSize
        qbbc    phpAcToAirProc_1_CmdlabelProc_L1_1_ivOpt1, ctxACToAir.ctrlInfo.flagACSnow3GZuc
            mov     saCmdL.optctrl1,   SA_3GPP_IV2_CMDL_OPT
            jmp     phpAcToAirProc_1_CmdlabelProc_L1_1_ivOptEnd
            
phpAcToAirProc_1_CmdlabelProc_L1_1_ivOpt1:
            mov     saCmdL.optctrl1,   ctxACToAir.ivOpt
            
            //pass through            
            
phpAcToAirProc_1_CmdlabelProc_L1_1_ivOptEnd:            

        // Store the command label
        sbco    saCmdL, cCmdlBuf,   acToAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
        add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
        // IV processing: Get and store the IV
        qbbs    phpAcToAirProc_1_CmdlabelProc_L2, acPktInst.ctrlFlag.acIvCopied  // Skip IV formatting operation
        qbbs    phpAcToAirProc_1_CmdlabelProc_L2, ctxACToAir.ctrlInfo.flagACIVPresent
    
        // copy IV to the command label location
        call    AC_formIVAuth
        mov     r0.b0, ctxACToAir.ivSize 
        sbco    saCmdlOpt.data1,  cCmdlBuf, acToAirPass1Loc1.cmdlOffset, b0
 
phpAcToAirProc_1_CmdlabelProc_L2:
    // IV has been copied in both cases
    add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, ctxACToAir.ivSize
    
    //zero &saCmdL, SIZE(saCmdL)
    jmp     phpAcToAirProc_1_CmdlabelProc_L3_KeyCopy
    
phpAcToAirProc_1_CmdlabelProc_L3:
     // CMAC operation: CMAC uses 8-byte IV
    qbbc    phpAcToAirProc_1_CmdlabelProc_L3_1, ctxACToAir.ctrlInfo.flagACIVPresent
    sub     saCmdL.sopOffset,  saCmdL.sopOffset, 8                    //adjust SOP offset to include IV
    
 phpAcToAirProc_1_CmdlabelProc_L3_1:    
    add     saCmdL.cmdlLen,    saCmdL.cmdlLen, SA_CMAC_CMDL_SIZE - SA_CMDL_HEADER_SIZE_BYTES
    mov     saCmdL.optctrl1,   SA_ENC_AUX1_OFFSET + (16 >> 3)
    
    // Store the command label
    sbco    saCmdL, cCmdlBuf,  acToAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // Move the security context viewer to the K1/K2 location 
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxAC.aux)) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4   
    xin     XID_SCVDATA, r6,  32
    
    and     r0.w0, acToAirPass1Loc1.hashDataLen, 0x000f
    qbne    phpAcToAirProc_1_CmdlabelProc_L3_2, r0.w0, 0
    // K1 is used
    sbco    r6, cCmdlBuf,  acToAirPass1Loc1.cmdlOffset, 16
    jmp     phpAcToAirProc_1_CmdlabelProc_L3_3
    
phpAcToAirProc_1_CmdlabelProc_L3_2:
    // k2 is used
    sbco    r10, cCmdlBuf,  acToAirPass1Loc1.cmdlOffset, 16
     
phpAcToAirProc_1_CmdlabelProc_L3_3:
    add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, 16
    
    // Move the security context viewer back 
    // Note: It may not be necessay since we do not update statistics at PHP Pass1 anymore
    // Commented out for refernce only
    //mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + OFFSET(struct_ctxAC.countC)) << 8)
    //xout    XID_SCVCTRL, CdeCmd, 4   
    
phpAcToAirProc_1_CmdlabelProc_L3_KeyCopy:
	// Check if mac key needs to be copied
	qbbc	phpAcToAirProc_1_CmdlabelProc_L3_Snow3G, ctxACToAir.ctrlInfo.flagACKeyInScratch
	    // Copy mac key
        qbbs    phpAcToAirProc_1_L3_macKey_copy_1, ctxACToAir.macKeyOffset.t_flag_key_at_2nd_block
	        lbco	saCmdlOpt.data1,  cKeyRamBase, ctxACToAir.macKeyOffset,    16
            jmp     phpAcToAirProc_1_L3_macKey_copy_end
phpAcToAirProc_1_L3_macKey_copy_1:
            clr     ctxACToAir.macKeyOffset.t_flag_key_at_2nd_block
            lbco	saCmdlOpt.data1,  cKeyRamBase2, ctxACToAir.macKeyOffset,   16
phpAcToAirProc_1_L3_macKey_copy_end:            
	    sbco    saCmdlOpt.data1,  cCmdlBuf,    acToAirPass1Loc1.cmdlOffset, 16
	    add     acToAirPass1Loc1.cmdlOffset,   acToAirPass1Loc1.cmdlOffset, 16
        
	    // Check to see if need to create modkey for kasumi
	    qbbc	phpAcToAirProc_1_CmdlabelProc_L3_Snow3G, ctxACToAir.ctrlInfo.flagACKasumiF9
	        // Make and copy kasumi modkey
	        ldi		r0.w0, 0xAAAA
	        ldi		r0.w2, 0xAAAA
	        xor		saCmdlOpt.data1, saCmdlOpt.data1, r0
	        xor		saCmdlOpt.data2, saCmdlOpt.data2, r0
	        xor		saCmdlOpt.data3, saCmdlOpt.data3, r0
	        xor		saCmdlOpt.data4, saCmdlOpt.data4, r0
	        sbco    saCmdlOpt.data1,  cCmdlBuf, acToAirPass1Loc1.cmdlOffset, 16
	        add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, 16
            
phpAcToAirProc_1_CmdlabelProc_L3_Snow3G:
#ifdef SASS_GEN2
	// Check if we  need to add Snow3G/ZUC parameters
	qbbc	phpAcToAirProc_1_CmdlabelProc_L4, ctxACToAir.ctrlInfo.flagACSnow3GZuc
        zero    &saCmdlOpt, SIZE(saCmdlOpt)
        mov     saCmdlOpt.data4, acToAirPass1Loc1.hashDataLen   
	    sbco    saCmdlOpt.data1,  cCmdlBuf, acToAirPass1Loc1.cmdlOffset, 16
	    add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, 16
        // pass through
#endif	
phpAcToAirProc_1_CmdlabelProc_L4:
    qbbc  phpAcToAirProc_1_CmdlabelProc_End, ctxACToAir.ctrlInfo.flagACEnc
        zero &saCmdL, SIZE(saCmdL)
        // Prepare Encryption Command Label 
        qbbc    phpAcToAirProc_1_CmdlabelProc_L4_encOut_1, ctxACToAir.ctrlInfo.flagACAuth
            mov     acPktInst.nextEngId_pass2,   SAENG_AIRC_2
            mov     saCmdL.nextEngId,  SAENG_OUT2_1 
            jmp     phpAcToAirProc_1_CmdlabelProc_L4_encOut_end 
phpAcToAirProc_1_CmdlabelProc_L4_encOut_1:      
            mov     saCmdL.nextEngId,  SAENG_PHP_2  
phpAcToAirProc_1_CmdlabelProc_L4_encOut_end:        
        add     saCmdL.cmdlLen,    ctxACToAir.ivSize, SA_CMDL_HEADER_SIZE_BYTES
        mov     saCmdL.procLen,    acToAirPass1Loc1.encryptedDataLen
        add     saCmdL.sopOffset,  acToAirPass1Loc1.protoOffset, acToAirPass1Loc1.hdrSize
        add     saCmdL.sopOffset,  saCmdL.sopOffset, ctxACToAir.authHdrSize
        
    
phpAcToAirProc_1_CmdlabelProc_L4_1:    
    mov     saCmdL.optctrl1,   ctxACToAir.ivOpt
    
	// Check to see if need to copy enc key from scratch
	// Note: assume always do either enc or mac, never combined operation
	qbbc	phpAcToAirProc_1_CmdlabelProc_L4_Snow3G_check2, ctxACToAir.ctrlInfo.flagACKeyInScratch
        mov		saCmdL.optctrl2,   SA_3GPP_KEY1_CMDL_OPT
	    add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
        
	    // Check to see if kasumi modkey needs to be created
	    qbbc	phpAcToAirProc_1_CmdlabelProc_L4_Snow3G_check, ctxACToAir.ctrlInfo.flagACKasumiF8
	        mov		saCmdL.optctrl3,   SA_3GPP_MOD_KEY_CMDL_OPT
	        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
            jmp     phpAcToAirProc_1_CmdlabelProc_L4_2
            
phpAcToAirProc_1_CmdlabelProc_L4_Snow3G_check:
#ifdef SASS_GEN2
        // Check to see if Snow3G/Zuc operation is required
	    qbbc	phpAcToAirProc_1_CmdlabelProc_L4_2, ctxACToAir.ctrlInfo.flagACSnow3GZuc
            mov		saCmdL.optctrl3,   SA_3GPP_MISC_CMDL_OPT
	        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
#endif            
            jmp     phpAcToAirProc_1_CmdlabelProc_L4_2
        
phpAcToAirProc_1_CmdlabelProc_L4_Snow3G_check2:
#ifdef SASS_GEN2
	    qbbc	phpAcToAirProc_1_CmdlabelProc_L4_2, ctxACToAir.ctrlInfo.flagACSnow3GZuc
            mov		saCmdL.optctrl2,   SA_3GPP_MISC_CMDL_OPT
	        add     saCmdL.cmdlLen,    saCmdL.cmdlLen, 16
            // pass through
#endif	
phpAcToAirProc_1_CmdlabelProc_L4_2:
    // Store the command label
    sbco    saCmdL, cCmdlBuf,   acToAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, SIZE(saCmdL)
    
    // IV processing: Get and store the IV
    qbbs    phpAcToAirProc_1_CmdlabelProc_L5, acPktInst.ctrlFlag.acIvCopied  // Skip IV formatting operation
    qbbs    phpAcToAirProc_1_CmdlabelProc_L5, ctxACToAir.ctrlInfo.flagACIVPresent
    
    // copy IV to the command label location
    call    AC_formIVEnc
    mov     r0.b0, ctxACToAir.ivSize 
    sbco    saCmdlOpt.data1,  cCmdlBuf, acToAirPass1Loc1.cmdlOffset, b0
    
phpAcToAirProc_1_CmdlabelProc_L5:
    // IV has been copied in both cases
    add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, ctxACToAir.ivSize
    
phpAcToAirProc_1_CmdlabelProc_L5_KeyCopy:
    
    // Copy key from scratch memory if necessary
	qbbc	phpAcToAirProc_1_CmdlabelProc_L5_Snow3G, ctxACToAir.ctrlInfo.flagACKeyInScratch
	    // Copy enc key
        qbbs    phpAcToAirProc_1_L5_encKey_copy_1, ctxACToAir.encKeyOffset.t_flag_key_at_2nd_block
	        lbco	saCmdlOpt.data1,  cKeyRamBase, ctxACToAir.encKeyOffset,     16
            jmp     phpAcToAirProc_1_L5_encKey_copy_end
phpAcToAirProc_1_L5_encKey_copy_1:
            clr     ctxACToAir.encKeyOffset.t_flag_key_at_2nd_block
	        lbco	saCmdlOpt.data1,  cKeyRamBase2, ctxACToAir.encKeyOffset,    16
phpAcToAirProc_1_L5_encKey_copy_end:                
	    sbco    saCmdlOpt.data1, cCmdlBuf, acToAirPass1Loc1.cmdlOffset, 16
	    add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, 16
        
	    // Check if kasumi modkey needs to be copied
	    qbbc	phpAcToAirProc_1_CmdlabelProc_L5_Snow3G, ctxACToAir.ctrlInfo.flagACKasumiF8
	        // Make and copy kasumi modkey
	        ldi		r0.w0, 0x5555
	        ldi		r0.w2, 0x5555
	        xor		saCmdlOpt.data1, saCmdlOpt.data1, r0
	        xor		saCmdlOpt.data2, saCmdlOpt.data2, r0
	        xor		saCmdlOpt.data3, saCmdlOpt.data3, r0
	        xor		saCmdlOpt.data4, saCmdlOpt.data4, r0
	        sbco    saCmdlOpt.data1,  cCmdlBuf, acToAirPass1Loc1.cmdlOffset, 16
	        add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, 16

phpAcToAirProc_1_CmdlabelProc_L5_Snow3G:
#ifdef SASS_GEN2
	// Check if we  need to add Snow3G/ZUC parameters
	qbbc	phpAcToAirProc_1_CmdlabelProc_L5_1, ctxACToAir.ctrlInfo.flagACSnow3GZuc
        zero    &saCmdlOpt, SIZE(saCmdlOpt)
        mov     saCmdlOpt.data4, acToAirPass1Loc1.encryptedDataLen   
	    sbco    saCmdlOpt.data1,  cCmdlBuf, acToAirPass1Loc1.cmdlOffset, 16
	    add     acToAirPass1Loc1.cmdlOffset, acToAirPass1Loc1.cmdlOffset, 16
#endif        
        // pass through
	
phpAcToAirProc_1_CmdlabelProc_L5_1:	
    .leave acToAirCmdlScope  
    
phpAcToAirProc_1_CmdlabelProc_End:

   // Skip logging if not enabled
   qbbc SkipAcDbgLog_5, phpContext.ctrlInfo.t_php_pkt_dbg_info
    // 8 bytes of Command Label to be stored
    lbco    r10, cCmdlBuf,   0, 8    
    // sbco    r10, cAcDbgBase, s_acDbg.offset, 8
    mov      r0,  PDSP_ACDBG_BASE
    sbbo    r10, r0, s_acDbg.offset, 8
    add     s_acDbg.offset,      s_acDbg.offset,  8
SkipAcDbgLog_5:

    // Issue patch command to copy the command label
    mov     CdeCmd.Cmd0,    CDE_CMD_PATCH_CONTROL_BUFFER
    mov     CdeCmd.Cmd0.b1, acToAirPass1Loc1.cmdlOffset 
    mov     CdeCmd.Cmd1,    PHP_CMDL_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
    
    jmp     phpAcToAirProc_1_PADDING_L1     
    
 phpAcToAirProc_1_OtherChunk_L1:
    lbco    acPktInst, cInstBase,  phpContext.instOffset, SIZE(acPktInst)
    
    .using pktDescScope

    //Update PktDesc for the packet
    mov     PktDesc.SA_NextEngId, acPktInst.nextEngId
    clr     PktDesc.SA_CmdLblInfo.flagCmdLblPresent
    
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 2  
    
    // Advance the window to the beginning of packet 
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4      
    mov     acToAirPass1Loc1.pktLoc,  0   // for non-SOP data chunk
    
    .leave pktDescScope
    
    //pass through 
phpAcToAirProc_1_PADDING_L1:
    //ICV padding processing 
    qbbc    phpAcToAirProc_1_End, acPktInst.ctrlFlag.acPaddingInserted
    qbbs    phpAcToAirProc_1_End, acPktInst.ctrlFlag.acPaddingComplete
    qbgt    phpAcToAirProc_1_PADDING_L3, phpContext.chunkSize, acPktInst.icvOffset 
    
    // ICV location is within this data chunk
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, acPktInst.icvOffset, acToAirPass1Loc1.pktLoc
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // Insert the padding based on the dir
    // Inset the 8-byte padding 
    zero    &CdeCmd, 8
    mov     CdeCmd.Cmd0.w0, CDE_CMD_INSERT_PACKET | (8 << 8)
    mov     CdeCmd.Cmd1.b3, 0x40
    qbbc    phpAcToAirProc_1_PADDING_L2, ctxACToAir.ctrlInfo.flagACDirDownlink
    mov     CdeCmd.Cmd1.b3, 0xC0
    
phpAcToAirProc_1_PADDING_L2:
    xout    XID_CDECTRL, CdeCmd, 8 
    set     acPktInst.ctrlFlag.acPaddingComplete
    jmp     phpAcToAirProc_1_End
    
phpAcToAirProc_1_PADDING_L3: 
    sub    acPktInst.icvOffset,  acPktInst.icvOffset,  phpContext.chunkSize
    //pass through  
    
 
phpAcToAirProc_1_End:
    //
    // Firmware workaround: all packets should enter PHP Pass2 due to the
    //                      output stream interface 4-byte alignmnet limitation
    // The following code is commented out 
    //
    //qbbs    phpAcToAirProc_1_End_L1, ctxACToAir.ctrlInfo.flagACAuth
    //qbbc    phpAcToAirProc_1_End_L1, phpContext.chunkInfo.chunkFlag_eop
    //
    //  Process EOP Data Chunk when Auth is not enabled
    //
    //add     ctxACToAir.numPkts,  ctxACToAir.numPkts, 1
    //clr     phpContext.instMask, phpContext.ctrlInfo
    //jmp     phpAcToAirProc_1_End_L2
 
phpAcToAirProc_1_End_L1:
 
    // Save Instance Data
    sbco    acPktInst, cInstBase, phpContext.instOffset, SIZE(acPktInst)
    
phpAcToAirProc_1_End_L2:
    // Save the updated security context
    // The statistic update will occur at PHP Pass2 only 
    //xout    XID_SCVDATA,  ctxACToAir,  SIZE(ctxACToAir)
    
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4

    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO<<8)
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
    
    jmp     MainLoop
    
 
.leave acToAirPass1Scope
.leave ctxACToAirScope
.leave commCtxScope    

// ******************************************************************************
// * Function:    sap_ac_to_air_proc_pass2
// ******************************************************************************
// * Description: This function perfroms the second pass of the Air Cipher packet
// *              header processing in the To-Air direction as the followings:  
// *  
// *    - Insert the Hash value  
// *    - Update statistics
// *
// *****************************************************************************/
//

.struct struct_acToAirPass2Loc1
    .u8     ctrlFlag            // ctrlFlag
    .u8     pktId   
    .u16    psCtrlSize          // PS Info + Control Info Size
    .u16    chunkSize           // adjusted chunk size due to byte insertion
    .u16    pktLoc              // current location from the packet beginning
.ends     

.enter acToAirPass2Scope
    .assign  struct_acToAirPass2Loc1,  R1,  R2,  acToAirPass2Loc1
    .assign  struct_acPktInst,         R22, R25, acPktInst      
.leave acToAirPass2Scope

phpAcToAirProc_2:
    // Data chunk processing
    .using pktDescScope
    .using commCtxScope
    .using acToAirPass2Scope
    .using  ctxACToAirScope
    
    // clear Local Variables (may be just some of them)
    zero    &acToAirPass2Loc1,    SIZE(acToAirPass2Loc1)    
    
phpAcToAirProc_2_L1:

    // Advance the SC Viewer the first set of security context
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(ctxComm) + OFFSET(struct_ctxAC.countC))<< 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxACToAir, SIZE(ctxACToAir)

    // Record packet instance index and load the instance
    mov     acToAirPass2Loc1.pktId,  PktDesc.PktId
    lbco    acPktInst, cInstBase,  phpContext.instOffset, SIZE(acPktInst)
    
    //
    // All packets should go through PHP for 4-byte alignement processing
    // We can not support authentication and encryption simultaneously anymore
    // The next destination is SAENG_OUT2_1
    // The following code is commented out and left here for reference only
    
    // Record the next engine ID only if SOP
#ifdef SASS_TOBE_DELETED    
    qbbc    phpAcToAirProc_2_L3, phpContext.chunkInfo.chunkFlag_sop
        qbbs    phpAcToAirProc_2_L2, ctxACToAir.ctrlInfo.flagACAuth
            mov     acPktInst.nextEngId_pass2,   SAENG_OUT2_1 
        jmp     phpAcToAirProc_2_L3 
 phpAcToAirProc_2_L2:      
            mov     acPktInst.nextEngId_pass2,   SAENG_AIRC_2  
#endif
    
 phpAcToAirProc_2_L3:    
    mov     PktDesc.SA_NextEngId, acPktInst.nextEngId_pass2
    xout    XID_CDEDATA, PktDesc.SA_NextEngId, 1
    //add     acToAirPass2Loc1.psCtrlSize, PktDesc.ControlSize, PktDesc.PsValidSize  
    mov     acToAirPass2Loc1.chunkSize,  phpContext.chunkSize 
    
    // Update swInfo1 at SOP
    qbbc    phpAcToAirProc_2_L3_1, PktDesc.ChunkInfo.chunkFlag_sop
    mov     PktDesc2.SwInfo1, r29
    xout    XID_CDEDATA, PktDesc2.SwInfo1, 4
    // pass through
    
phpAcToAirProc_2_L3_1:    
    
    // 
    // Special case: remove inserted IV at the first data chunk only if ICV is not available
    // 
    qbbc    phpAcToAirProc_2_HashProc_L1, phpContext.chunkInfo.chunkFlag_sop
    qbbc    phpAcToAirProc_2_HashProc_L1, acPktInst.ctrlFlag.acIvInserted
    qble    phpAcToAirProc_2_HashProc_L1_1, phpContext.chunkSize, acPktInst.icvOffset_pass2
    
    // Advance the window to the IV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, acPktInst.hdrOffset
    xout    XID_CDECTRL, CdeCmd, 4   
    
    // flush out 8-byte CMAC-IV
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4
    add     acToAirPass2Loc1.pktLoc,  acPktInst.hdrOffset, 8
    
    jmp     phpAcToAirProc_2_HashProc_L2
    
    
 //
 // Hash Processing
 //   
 phpAcToAirProc_2_HashProc_L1:
 
    qbbc    phpAcToAirProc_2_HashProc_End, ctxACToAir.ctrlInfo.flagACAuth
    qbbs    phpAcToAirProc_2_HashProc_End, acPktInst.ctrlFlag.acHashComplete   
    qbgt    phpAcToAirProc_2_HashProc_L2,  phpContext.chunkSize,   acPktInst.icvOffset_pass2
    
    .leave pktDescScope
    
phpAcToAirProc_2_HashProc_L1_1:    
    
    //
    // Hash data is available in the PS Info section
    //
    set     acPktInst.ctrlFlag.acHashComplete
    // Advance to the PS Info section
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PSDATA
    xout    XID_CDECTRL, CdeCmd, 4      
 
    // Read and store the Hash Data
    mov     r0.b0, ctxACToAir.icvSize
    xin     XID_CDEDATA,  r6,  b0
    
    // Move below to save cycles
    //sbco    r6, cAuxBuf, 0, b0
    
    // Advance the window to the icv location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_ADVANCE_TO_PACKET
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Move here to save cycles
    sbco    r6, cAuxBuf, 0, b0
    
    
phpAcToAirProc_2_HashProc_L1_2:    

    // verify whether we need to remove inserted IV
    // it is applicable for SOP only
    qbbc    phpAcToAirProc_2_HashProc_L1_3, phpContext.chunkInfo.chunkFlag_sop
    qbbc    phpAcToAirProc_2_HashProc_L1_3, acPktInst.ctrlFlag.acIvInserted
    
    // mov to inserted IV location
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    mov     CdeCmd.Cmd0.w2, acPktInst.hdrOffset
    xout    XID_CDECTRL, CdeCmd, 4
    
    // Flush out 8-byte CMAC-IV   
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4
    add     acToAirPass2Loc1.pktLoc,  acPktInst.hdrOffset, 8
    
phpAcToAirProc_2_HashProc_L1_3:    
    
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_ADVANCE
    sub     CdeCmd.Cmd0.w2, acPktInst.icvOffset_pass2, acToAirPass2Loc1.pktLoc
    xout    XID_CDECTRL, CdeCmd, 4  
    mov     acToAirPass2Loc1.pktLoc, acPktInst.icvOffset_pass2
    
    qbbc    phpAcToAirProc_2_HashProc_L1_4,  acPktInst.ctrlFlag.acPaddingInserted
    // flush out Kasumi F9 8-byte padding
    mov     CdeCmd.Cmd0.w0, CDE_CMD_WINDOW_FLUSH
    mov     CdeCmd.Cmd0.w2, 8
    xout    XID_CDECTRL, CdeCmd, 4
    add     acToAirPass2Loc1.pktLoc,  acToAirPass2Loc1.pktLoc, 8
    
phpAcToAirProc_2_HashProc_L1_4:    
    // Insert the Hash data
    mov     CdeCmd.Cmd0,    CDE_CMD_INSERT_PACKET_BUFFER
    mov     CdeCmd.Cmd0.b1, ctxACToAir.icvSize
    mov     CdeCmd.Cmd1, PHP_AUX_BUF_ADDR
    xout    XID_CDECTRL, CdeCmd, 8
      
    jmp     phpAcToAirProc_2_HashProc_End    
    
 phpAcToAirProc_2_HashProc_L2:
    sub     acPktInst.icvOffset_pass2,  acPktInst.icvOffset_pass2, phpContext.chunkSize 
    
 phpAcToAirProc_2_HashProc_End:
 
    
phpAcToAirProc_2_HashProc_End_L1:    
 
    qbbc    phpAcToAirProc_2_L4,  phpContext.chunkInfo.chunkFlag_eop
    //
    //  Process EOP Data Chunk
    //
    mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (SIZE(ctxComm) << 8)
    xout    XID_SCVCTRL, CdeCmd, 4        
    xin     XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
    add     ctxACStats.numPkts,   ctxACStats.numPkts, 1
    adc     ctxACStats.numPktsHi, ctxACStats.numPktsHi, 0
    xout    XID_SCVDATA, ctxACStats, SIZE(ctxACStats)
    clr     phpContext.instMask, acToAirPass2Loc1.pktId
    
    // Update System Statistics
    lbco    r4, cStats, PDSP_AC_ENC_HI_OFFSET, 8
    add     r5, r5, 1
    adc     r4, r4, 0
    sbco    r4, cStats, PDSP_AC_ENC_HI_OFFSET, 8
    
    jmp     phpAcToAirProc_2_End
    
 phpAcToAirProc_2_L4:
    //
    //  Process non-EOP data chunk
    // 
    //  Save the instance Data
    sbco    acPktInst, cInstBase, phpContext.instOffset, SIZE(acPktInst)
    
 phpAcToAirProc_2_End:
    //close the security context
    mov     CdeCmd.Cmd0, SCV_CMD_DONE
    xout    XID_SCVCTRL, CdeCmd.Cmd0, 4
   
    // Send this packet (set valid PsInfo to zero)
    mov     CdeCmd.Cmd0, CDE_CMD_PACKET_ADVANCE | (CDE_FLG_SET_PSINFO << 8) 
    xout    XID_CDECTRL, CdeCmd, 4                  // Send the command         
 
    jmp     MainLoop

.leave acToAirPass2Scope
.leave ctxACToAirScope
.leave commCtxScope    
 
        
