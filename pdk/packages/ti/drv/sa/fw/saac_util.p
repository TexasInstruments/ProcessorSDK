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
// R14-R21       : AC Security Context including all parametes required
// R10-R13       : output iv 
// R30           : return address

//
// The following structue is shared by both SRTP Tx and Rx operation
//

.enter utilACIvScope
    .assign  struct_ctxAC1,      R14, R21.w2,   utilCtxAC
.leave utilACIvScope

///******************************************************************************
// * Function: sap_ac_form_iv_enc
// ******************************************************************************
// * Description: This function forms the initialization vector (IV) for the 
// *              Air Cipher PDU in encryption mode
// *
// *	    Kasumi IV  = Count-C || Bearer || Direction || Low-bits of IV  (64-bit)
// *	    SNOW 3G IV = Count-C || Bearer || Direction || Low-bits of IV || 
// *                     Count-C || Bearer || Direction || Low-bits of IV (128-bit)
// *	    ZUC IV     = Count-C || Bearer || Direction || Low-bits of IV || 
// *                     Count-C || Bearer || Direction || Low-bits of IV (128-bit)
// *        AES-CTR IV = Count-C || Bearer || Direction || Low-bits of IV || 
// *                     0(64) (128-bit)
// *
// * Note: SNOW 3G is presented as IV3||IV2||IV1||IV0 here, but it need to be 
// *       delivered to engine as IV0||IV1||IV2||IV3 in the command label
// * Note: iv26 is all zeros for all cases      
// *****************************************************************************/
//void sap_ac_form_iv_enc(UINT8 *iv, SA_CTX_PROTO_AC_T* p_proto_ctx)
//{
//  utlWrite32bits_m(iv, 0, p_proto_ctx->count_c);
//  utlWrite32bits_m(iv, 4, p_proto_ctx->iv_low26);
//  iv[4] |= (((p_proto_ctx->bearer & 0x1F) << 3) |
//            (SA_CTX_PROTO_AC_GET_DIR(p_proto_ctx) << 2)); 
//          
//  if (SA_CTX_PROTO_AC_GET_ALGORITHM(p_proto_ctx) == SA_AC_ALGORITHM_SNOW3G)
//  {
//      utlWrite32bits_m(iv, 4, p_proto_ctx->count_c);
//      utlWrite32bits_m(iv, 0, p_proto_ctx->iv_low26);
//      iv[0] |= (((p_proto_ctx->bearer & 0x1F) << 3) |
//            (SA_CTX_PROTO_AC_GET_DIR(p_proto_ctx) << 2)); 
//   
//      memcpy(&iv[8], &iv[0], 8);
//  }
//  else 
//  {
//      utlWrite32bits_m(iv, 0, p_proto_ctx->count_c);
//      utlWrite32bits_m(iv, 4, p_proto_ctx->iv_low26);
//      iv[4] |= (((p_proto_ctx->bearer & 0x1F) << 3) |
//            (SA_CTX_PROTO_AC_GET_DIR(p_proto_ctx) << 2)); 
//    
//       if (SA_CTX_PROTO_AC_GET_ALGORITHM(p_proto_ctx) == SA_AC_ALGORITHM_AES)
//            memset(&iv[0], 0, 8);
//  }  
//} 
//

.using utilACIvScope

AC_formIVEnc:
    // clear the IV
    zero &r10, 16

    // Extrct the Core Algorithm 
    and r29.b0, utilCtxAC.ctrlInfo, AC_CORE_ALGO_MASK
    
 AC_formIVEnc_L1:
    // Common processing 
//  mov r10,    utilCtxAC.countC
//  mov r11,    utilCtxAC.ivLow26
//  iv[4] |= (((p_proto_ctx->bearer & 0x1F) << 3) |
//            (SA_CTX_PROTO_AC_GET_DIR(p_proto_ctx) << 2)); 
    lsl r29.b1, utilCtxAC.bearer, 3
    xor r11.b3, r11.b3, r29.b1
    qbbc AC_formIVEnc_L2, utilCtxAC.ctrlInfo.flagACDirDownlink
    set r11.b3, 2
    
 AC_formIVEnc_L2:
    // Algorithm specific operation   
    qbne AC_formIVEnc_L3, r29.b0, SA_AC_ALGORITHM_SNOW3G
        // snow3G
        mov r12, r11
        mov r13, utilCtxAC.countC
        mov r11, r13
        mov r10, r12 
        ret
    
 AC_formIVEnc_L3:  
    mov r10, utilCtxAC.countC
#ifdef SASS_GEN2    
    qbeq AC_formIVEnc_L4, r29.b0, SA_AC_ALGORITHM_ZUC
        ret
       
 AC_formIVEnc_L4:  
    mov r12, r10
    mov r13, r11
#endif    
    ret
    
///******************************************************************************
// * Function: sap_ac_form_iv_auth
// ******************************************************************************
// * Description: This function forms the initialization vector (IV) for the 
// *              Air Cipher PDU in authentication mode 
// *
// *	 Kasumi IV   = Count-C || Fresh  (64-bit)
// *	 SNOW 3G IV  = Count-C || Fresh || 
// *                   Count-C XOR (Direction << 31) || Fresh XOR (Direction << 15) 
// *	 ZUC IV      = Count-C || Bearer || 0 || Low-bits of IV || 
// *                   Count-C XOR (Direction << 31) || Bearer || 0 || Low-bits of IV
// *                                                                   XOR (Direction << 15) 
// *     AES-CMAC IV = Count-C || Bearer || Direction || Low-bits of IV (64-bit) 
// *                   
// * Note: SNOW 3G is presented as IV3||IV2||IV1||IV0 here, but it need to be 
// *       delivered to engine as IV0||IV1||IV2||IV3 in the command label
// * Note: iv26 is all zeros for all cases      
// *
// *****************************************************************************/
//void sap_ac_form_iv_auth(UINT8 *iv, SA_CTX_PROTO_AC_T* p_proto_ctx)
//{
//  utlWrite32bits_m(iv, 0, p_proto_ctx->count_c);

//  if ((SA_CTX_PROTO_AC_GET_ALGORITHM(p_proto_ctx) == SA_AC_ALGORITHM_KASUMI) ||
//      (SA_CTX_PROTO_AC_GET_ALGORITHM(p_proto_ctx) == SA_AC_ALGORITHM_SNOW3G))
//  {
//    utlWrite32bits_m(iv, 4, p_proto_ctx->fresh);
//  }
//  else 
//  {
//    /* AES-CMAC */
//    utlWrite32bits_m(iv, 4, p_proto_ctx->iv_low26);
//    iv[4] |= ((p_proto_ctx->bearer & 0x1F) << 3);
//  }
//          
//  if (SA_CTX_PROTO_AC_GET_ALGORITHM(p_proto_ctx) == SA_AC_ALGORITHM_SNOW3G)
//  {
//    UINT8 dir_byte = SA_CTX_PROTO_AC_GET_DIR(p_proto_ctx)?0x80:0x00;
//    
//    memcpy(&iv[12], &iv[0], 4);
//    memcpy(&iv[8], &iv[4], 4);
//    memcpy(&iv[0], &iv[8], 4);
//    memcpy(&iv[4], &iv[12], 4);
//
//    iv[10] ^= dir_byte;
//    iv[12] ^= dir_byte;
//  }
//  else if (SA_CTX_PROTO_AC_GET_ALGORITHM(p_proto_ctx) == SA_AC_ALGORITHM_AES)
//  {
//    iv[4] |= (SA_CTX_PROTO_AC_GET_DIR(p_proto_ctx) << 2);
//  }  
//}

AC_formIVAuth:
    // clear the IV
    zero &r10, 16

    // Extrct the Core Algorithm 
    and r29.b0, utilCtxAC.ctrlInfo, AC_CORE_ALGO_MASK
    
AC_formIVAuth_L1:
    // Common processing 
    mov r10, utilCtxAC.countC
    
    qbne AC_formIVAuth_L2, r29.b0, SA_AC_ALGORITHM_KASUMI
        // Kasumi F9 Operation
        mov r11, utilCtxAC.fresh
        ret
 
AC_formIVAuth_L2: 
#ifdef SASS_GEN2    
    qbeq AC_formIVAuth_L4, r29.b0, SA_AC_ALGORITHM_SNOW3G
#endif
    // CMAC and ZUC Operation
    mov r11,    utilCtxAC.ivLow26
    lsl r29.b1, utilCtxAC.bearer, 3
    xor r11.b3, r11.b3, r29.b1
    
#ifdef SASS_GEN2    
AC_formIVAuth_L3:
    // Algorithm specific operation   
    qbeq AC_formIVAuth_L5, r29.b0, SA_AC_ALGORITHM_ZUC
    qbeq AC_formIVAuth_L6, r29.b0, SA_AC_ALGORITHM_AES
    ret

AC_formIVAuth_L4:
    // Snow 3G F9 operation 
    mov r12, utilCtxAC.fresh
    mov r13, r10
    mov r11, r10
    mov r10, r12
    qbbc AC_formIVAuth_End, utilCtxAC.ctrlInfo.flagACDirDownlink
        xor r11.b3, r11.b3, 0x80
        xor r10.b1, r10.b1, 0x80
        ret    
    
AC_formIVAuth_L5:
    // ZUC operation 
    mov r12, r10
    mov r13, r11
    qbbc AC_formIVAuth_End, utilCtxAC.ctrlInfo.flagACDirDownlink
        xor r12.b3, r12.b3, 0x80
        xor r13.b1, r13.b1, 0x80
        ret    
#endif        
    
AC_formIVAuth_L6:
    // CMAC operation
    qbbc AC_formIVAuth_End, utilCtxAC.ctrlInfo.flagACDirDownlink
        set r11.b3, 2
    
 AC_formIVAuth_End:  
    ret

.leave utilACIvScope


// AC_ICVCheck
//
// This function is used to compare the hash value for AC
//
// Input:
//   R6-R9: ICV value in the packet
//   R10-R13: ICV Value from the Authentication Routine 
//   R29.b0: icvSize
//
// Return Value:
//    R29.b0  0: No Match
//            icvSize:   
//
// Note we only support the ICV size 4 
 AC_ICVCheck:
    qbne    AC_ICVCheck_Fail, r6, r10
    qbeq    AC_ICVCheck_Match, r29.b0, 4

 AC_ICVCheck_Fail:
    mov r29.b0, 0

 AC_ICVCheck_Match:
    ret   
    
    
///******************************************************************************
// * Type:        SA_RN_FIFO_T        
// ******************************************************************************
// * Description: This structure specifies the SA Random Number (RN) FIFO which 
// *              is used to store the random number which will be used to format
// *              the Initialization Vector (IV) for IPSEC ESP Tx packet.
// *
// *
// *                         +-+-+-+-+-+-+-+-+
// *                         |               | 
// *                         +-+-+-+-+-+-+-+-+
// *                         |               | 
// *                         +-+-+-+-+-+-+-+-+ <--- Out
// *                         |               |   ^
// *                         ~               ~   |
// *                         |               |   |
// *                         +-+-+-+-+-+-+-+-+   | cnt          
// *                         |               |   |
// *                         +-+-+-+-+-+-+-+-+   |
// *                         |               |   v
// *                         +-+-+-+-+-+-+-+-+ <--- In
// *                         |               |
// *                         ~               ~
// *                         |               | 
// *                         +-+-+-+-+-+-+-+-+
// *                         |               | 
// *                         +-+-+-+-+-+-+-+-+
// *                         |               | 
// *                         +-+-+-+-+-+-+-+-+
// *
// *                    Figure SA Random Number FIFO        
// *
// *
// *              The FIFO will be initialized to full with random numbers. 
// *              Therefore, both the in index point and the out index points to zero  
// *
// *              The fifo_pop API is called to pop the random numbers from FIFO
// *              The fifo_put API is aclled to insert new randown numbers into
// *              FIFO  
// *                                                                      
// *              For IPSEC EXP Tx operation, the fifo_pop API is used to generate
// *              and insert IV to the packet; the fifo_push API is invoked to
// *              insert the new randon numbers to the FIFO.
// *
// *  PDSP Implementation Assumptions:
// *      1. It only supports IV size = 8/16.
// *      2. Each Tx packet processing will consume and produce the same amount of random
// *         numbers so that the number of unused random numbers in the FIFO will not
// *         change over time. 
// *      3. We provide enough margin (16 * 16) in the initialization routine to make sure 
// *         that there is always sufficient random numbers to format IV.
// *      
// *  PDSP Implementation notes:
// *      1. There is no error ckeck to save cycles because of assumption 3.
// *      2. Only the FIFO initialization routine (sa_rn_fifo_init) is implemented
// *         as a function.
// *      3. Both the fifo_pop and fifo_push operations will be customized to
// *         the specific IPSEC ESP IV generation and encrypted data extraction
// *         operation in the IPSEC ESP TX pass1 and pass2 routines respectively.    
// *
// *****************************************************************************/
///*
// * We need to support up to 12 packets with up to 16 bytes IV simultaneously.
// * To provide 4 packet margin, the FIFO size should be 512 = 16*16*2.
// */ 
//

///*******************************************************************************
// *  Function:   sa_rn_fifo_init
// *******************************************************************************
// *  DESCRIPTION:
// *      Initailze the RN FIFO
// *  
// ******************************************************************************/
//void sa_rn_fifo_init(SA_RN_FIFO_T* fifo)
//{
//    int i; 
//    UINT32 num;
//    
//    
//    for (i = 0; i < SAP_MAX_RN_FIFO_SIZE; i+= 4)
//    {
//        num = rand();
//        fifo->data[i]   = (UINT8) (num >> 24);
//        fifo->data[i+1] = (UINT8) (num >> 16);
//        fifo->data[i+2] = (UINT8) (num >> 8);
//        fifo->data[i+3] = (UINT8) (num >> 0);
//    }
//    
//    fifo->cnt = (UINT16) i;
//    fifo->in = (UINT16) i;
//    fifo->out = 0;
//} 
//

//
// Register Allocation
//
// Stratch R0, R1, R2, R3, R29 
// RnFifoCb: R8
// Rn Control Base: R6
// Rn Buffer Base: R7

.enter  rnFifoInitScope
    .assign struct_saRnFifoCb,  R8, R8, rnFifoCb
.leave  rnFifoInitScope
                      
.using rnFifoInitScope

RnFifo_Init:
    // Initialize the control block
    zero    &rnFifoCb, SIZE(rnFifoCb)
    
    mov     r29.w0, phpRnFifoSize 
    mov     r6, PHP1_RN_CTRL_ADDR    
    mov     r7, PHP1_RN_FIFO_ADDR    
    
    
    // Initialize the RNG
    mov32   r0, 0x10000
    mov32   r1, 0x00001
    sbco    r0, cRngBase, RNG_REG_CTRL, 8
    
    mov     r0.w0, 0x400
    sbco    r0, cRngBase, RNG_REG_CTRL, 4
    
    mov     r0, 1
    
RnFifo_Init_LOOP_L1:
    lbco    r1, cRngBase, RNG_REG_STAT, 4
    qbbc    RnFifo_Init_LOOP_L1, r1, 0
    
    // Get and store the random number
    lbco    r2, cRngBase, RNG_REG_OUT_0, 8
    sbbo    r2, r7, rnFifoCb.in, 8
    add     rnFifoCb.in, rnFifoCb.in, 8
    
    qble    RnFifo_Init_L1_End, rnFifoCb.in, r29.w0
    sbco    r0, cRngBase, RNG_REG_INTACK, 4
    jmp     RnFifo_Init_LOOP_L1
    
RnFifo_Init_L1_End:
    mov     rnFifoCb.in,   0
    sbbo    rnFifoCb,  r6, 0, SIZE(rnFifoCb)
    
    
// Second FIFO for PASS2
#ifdef SASS_GEN2

    mov     r6, PHP3_RN_CTRL_ADDR    
    mov     r7, PHP3_RN_FIFO_ADDR    
    
RnFifo_Init_LOOP_L2:
    lbco    r1, cRngBase, RNG_REG_STAT, 4
    qbbc    RnFifo_Init_LOOP_L2, r1, 0
    
    // Get and store the random number
    lbco    r2, cRngBase, RNG_REG_OUT_0, 8
    sbbo    r2, r7, rnFifoCb.in, 8
    add     rnFifoCb.in, rnFifoCb.in, 8
    
    qble    RnFifo_Init_L2_End, rnFifoCb.in, r29.w0
    sbco    r0, cRngBase, RNG_REG_INTACK, 4
    jmp     RnFifo_Init_LOOP_L2
    
RnFifo_Init_L2_End:
    mov     rnFifoCb.in,   0
    sbbo    rnFifoCb,  r6, 0, SIZE(rnFifoCb)
    
#endif    
    ret   

.leave rnFifoInitScope   
    
    


