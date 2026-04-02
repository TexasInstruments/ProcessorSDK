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
// Register MAP
// R0, R29       : General and local register
// R6-R8         : rtp header (from packet)
// R14-R21       : SRTP Security Context including all parametes required
// R10-R13       : output iv 
// R30           : return address

//
// The following structue is shared by both SRTP Tx and Rx operation
//

.struct struct_ctxSRTP
    .u16    ctrlInfo
#define     SRTP_ENCRYPT_MODE_MASK     0x07
    .u16    lastSeqNum
    .u32    roc
    .u8     icvSize
    .u8     ivSize
    .u8     ivOpt
    .u8     mkiSize
    .u32    mki
    .u32    saltKey1
    .u32    saltKey2
    .u32    saltKey3
    .u16    saltKey4
    .u8     salkKeySize
    .u8     reserved2 
.ends

.enter utilSRTPIvScope
    .assign  struct_ctxSRTP,        R14, R21,   utilCtxSRTP
    .assign  struct_rtpHdr,         R6, R8,     utilRtpHdr  
    .assign  struct_srtpPktInst,    R22, R26,   srtpPktInst  
.leave utilSRTPIvScope


///******************************************************************************
// * Function: sap_srtp_form_iv
// ******************************************************************************
// * Description: This function forms the initialization vector (IV) from the
// *              RTP header and the salt keys 
// *    AES-CTR:
// *    IV = (session_salt<<16) XOR (SSRC<<64) XOR (RTP index << 16)
// *
// *    AES-F8:
// *    IV' = 0x00 || M || PT || SEQ || TS || SSRC || ROC
// *
// *****************************************************************************/
//void sap_srtp_form_iv(UINT8* rtp, UINT8 *iv, UINT8 *salt, UINT32 roc, UINT8 mode)
//{
//  
//  UINT32 *p_dest32, *p_src32;
//  UINT16 *p_dest16, *p_src16;
//  UINT8  roc_buf[4];
//  
//
//  if (mode == SA_SRTP_ENCRYPT_MODE_CTR)
//  {
//    memcpy(iv, salt, 14);                           /* session_salt << 16 */
//    p_dest32 = (UINT32 *)&iv[4];                    /* XOR (SSRC<<64) */
//    p_src32 = (UINT32 *)&rtp[RTP_BYTE_OFFSET_SSRC];
//    *p_dest32 ^= *p_src32;
//    p_dest32 = (UINT32 *)&iv[8];                    /* XOR (RTP Sequence Number << 16) */ 
//    p_src32 = (UINT32 *)&roc_buf[0];
//    utlWrite32bits_m(roc_buf, 0, roc);
//    *p_dest32 ^= *p_src32;
//    p_dest16 = (UINT16 *)&iv[12];
//    p_src16 = (UINT16 *)&rtp[RTP_BYTE_OFFSET_SEQ_NUM];
//    *p_dest16 ^= *p_src16;
//    iv[14] = iv[15] = 0;
//  }  
//  else if (mode == SA_SRTP_ENCRYPT_MODE_F8)
//  {
//    iv[0] = 0;  /* 0x00 */
//    /* || M || PT || SEQ || TS || SSRC */
//    memcpy(iv + RTP_BYTE_OFFSET_M_PT, rtp + RTP_BYTE_OFFSET_M_PT, 11);
//    /* || ROC */
//    utlWrite32bits_m(iv, 12, roc);
//  }
//
//} /* srtpForm_CTR_IV */
//

SRTP_formIV:
.using utilSRTPIvScope
    // clear the IV
    zero &r10, 16

    // Extrct the ENCRYPTION mode
    and r29.b0, utilCtxSRTP.ctrlInfo, SRTP_ENCRYPT_MODE_MASK
    qbeq SRTP_formIV_F8, r29.b0, SA_SRTP_ENCRYPT_MODE_F8
    
 SRTP_formIV_CTR:
     
    mov r10,    utilCtxSRTP.saltKey1
    xor r11,    utilCtxSRTP.saltKey2, utilRtpHdr.ssrc
    xor r12,    utilCtxSRTP.saltKey3, srtpPktInst.tempROC
    xor r13.w2, utilCtxSRTP.saltKey4, utilRtpHdr.seqNum
    ret
    
 SRTP_formIV_F8:    
    mov r10.b2, utilRtpHdr.mPt
    mov r10.w0, utilRtpHdr.seqNum 
    mov r11,    utilRtpHdr.timestamp
    mov r12,    utilRtpHdr.ssrc
    mov r13,    srtpPktInst.tempROC  
    ret

.leave utilSRTPIvScope

//
///******************************************************************************
// * Function: sap_srtp_get_roc
// ******************************************************************************
// * Description: This function implements the SRTP index determination algorithm
// *              specified at RFC 3711 Apendix A 
// *
// *****************************************************************************/
//UINT32 sap_srtp_get_roc(UINT16 seq_num, SA_CTX_HW_SRTP_RX_T* p_hw_ctx, SA_SRTP_PKT_INST_T*  p_pkt_inst)
//{
//  UINT32 v;
//  
//  if (p_hw_ctx->last_seq_num < 0x8000)
//  {
//    if ( (((SINT32)seq_num - (SINT32)p_hw_ctx->last_seq_num) > 0x0008000L) 
//                            && !(p_hw_ctx->num_rx_pkts_hi == 0)
//                            && !(p_hw_ctx->num_rx_pkts_lo == 0))
//    {
//      /*set v to (ROC-1) */
//      v = p_hw_ctx->roc - 1;
//      p_pkt_inst->roc_status = SA_SRTP_ROC_MINUS_1;
//    }
//    else
//    {
//      v = p_hw_ctx->roc;
//      p_pkt_inst->roc_status = SA_SRTP_ROC_NOCHANGE;
//    }
//  }
//  else
//  {
//    if (((SINT32)p_hw_ctx->last_seq_num - (SINT32)(0x0008000L)) > (SINT32)seq_num)
//    {
//      /*    set v to (ROC+1) */
//      v = p_hw_ctx->roc + 1;
//      p_pkt_inst->roc_status = SA_SRTP_ROC_PLUS_1;
//    }
//    else
//    {
//      v = p_hw_ctx->roc;
//      p_pkt_inst->roc_status = SA_SRTP_ROC_NOCHANGE;
//    }
//  }
//  
//  return v;
//}
//    
// Register MAP
// R0, R29       : General and local register
// R14-R21       : ctxSRTPRx1
// R22-R25       : srtpPktInst 
// R30           : return address

 .using  srtpRxPass1Scope
 .using  ctxSRTPRxScope
    
 SRTP_getROC:
    //
    // Sepcial case for the first packet
    //
    qbbs SRTP_getROC_NOCHANGE, ctxSRTPRx1.ctrlInfo.flagSRTPRx1stPkt 
             
 SRTP_getROC_L1:
    qbbs SRTP_getROC_L2, ctxSRTPRx1.lastSeqNum, 15
    // p_hw_ctx->last_seq_num < 0x8000
    qbge  SRTP_getROC_NOCHANGE, srtpPktInst.seqNum, ctxSRTPRx1.lastSeqNum
    sub   r29.w2,  srtpPktInst.seqNum, ctxSRTPRx1.lastSeqNum
    mov   r29.w0,  0x8000
    qbgt  SRTP_getROC_MINUS_1, r29.w0, r29.w2
    jmp   SRTP_getROC_NOCHANGE
 
 SRTP_getROC_L2:
    // p_hw_ctx->last_seq_num >= 0x8000
    clr   r29.w0, ctxSRTPRx1.lastSeqNum, 15
    qbgt  SRTP_getROC_PLUS_1, srtpPktInst.seqNum, r29.w0
 
 SRTP_getROC_NOCHANGE:
    mov  srtpPktInst.tempROC,  ctxSRTPRx1.roc
    mov  srtpPktInst.rocStatus, SA_SRTP_ROC_NOCHANGE
    ret
    
 SRTP_getROC_PLUS_1:
    add  srtpPktInst.tempROC,  ctxSRTPRx1.roc, 1
    mov  srtpPktInst.rocStatus, SA_SRTP_ROC_PLUS_1
    ret
 
  SRTP_getROC_MINUS_1:
    sub  srtpPktInst.tempROC,  ctxSRTPRx1.roc, 1
    mov  srtpPktInst.rocStatus, SA_SRTP_ROC_MINUS_1
    ret
   
.leave  srtpRxPass1Scope
.leave  ctxSRTPRxScope
 

// SRTP_ICVCheck
//
// This function is used to compare the hash value for SRTP
//
// Input:
//   R6-R9: ICV Value from the Authentication Routine
//   R10-R13: ICV value in the packet
//   R29.b0: icvSize
//
// Return Value:
//    R29.b0  0: No Match
//            icvSize:   
//
// Note we only support the ICV size 4 and 10
 SRTP_ICVCheck:
    qbne    SRTP_ICVCheck_Fail, r6, r10
    qbeq    SRTP_ICVCheck_Match, r29.b0, 4
    qbne    SRTP_ICVCheck_Fail, r7, r11
    qbne    SRTP_ICVCheck_Fail, r8.w2, r12.w2
    qbeq    SRTP_ICVCheck_Match, r29.b0, 10
//  qbne    SRTP_ICVCheck_Fail, r9, r13
//qbeq    SRTP_ICVCheck_Match, r29.b0, 16
//lbbo    r0, icv_address1, 16, 16 
//lbbo    r4, icv_address2, 16, 16 // skip
//qbne    SRTP_ICVCheck_Fail, r0, r10
//qbeq    SRTP_ICVCheck_Match, r29.b0, 20
//qbne    SRTP_ICVCheck_Fail, r1, r11
//qbeq    SRTP_ICVCheck_Match, r29.b0, 24
//qbne    SRTP_ICVCheck_Fail, r2, r12
//qbeq    SRTP_ICVCheck_Match, r29.b0, 28
//qbne    SRTP_ICVCheck_Fail, r3, r13

 SRTP_ICVCheck_Fail:
    mov r29.b0, 0

 SRTP_ICVCheck_Match:
    ret   
    
// SRTP_MKICheck
//
// This function is used to compare the MKI value for SRTP
//
// Input:
//   R6 MKI value in the packet
//   R14, R21: SRTP security context, mkiSize = r16.b0, mki = r17 
//
// Return Value:
//    R29.b0  0: Match
//            1: Failure
//
// Note we only support the ICV size 4 and 10
 SRTP_MKICheck:
    mov     r29.b0, 0
    qbne    SRTP_MKICheck_Fail, r6.w2, r17.w2
    qbeq    SRTP_MKICheck_Match, r16.b0, 2
    qbne    SRTP_MKICheck_Fail, r6.w0, r17.w0
    qbeq    SRTP_MKICheck_Match, r16.b0, 4

 SRTP_MKICheck_Fail:
    mov r29.b0, 1

 SRTP_MKICheck_Match:
    ret     
      


