#ifndef _SALLDPKA_H
#define _SALLDPKA_H
/*******************************************************************************
 * FILE PURPOSE: Provide Protocol related defintions
 *
 ********************************************************************************
 * FILE NAME: 	salldpka.h
 *
 * DESCRIPTION: Define PKA related data structures, MICRO and constands
 *              used by the Security Accelerator (SA)	
 *
 * REVISION HISTORY:
 *
 * (C) Copyright 2014 Texas Instruments, Inc.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
#ifndef NSS_PKA_GEN2
/* SA PKA related definitions */
#define SA_PKA_RAM_BASE                  0x00022000UL
#define SA_PKA_AINDEX                    0
#define SA_PKA_BINDEX                    1
#define SA_PKA_CINDEX                    2
#define SA_PKA_DINDEX                    3

/* PKA A, B, C, B offsets in 32-bit words */
#define SA_PKA_A_OFFSET                  0x00
#define SA_PKA_B_OFFSET                  0x80
#define SA_PKA_C_OFFSET                  0x100
#define SA_PKA_D_OFFSET                  0x180
#else
/* SA PKA related definitions */
#define SA_PKA_RAM_BASE                     0x00024000UL
#define SA_PKA_AINDEX                       0
#define SA_PKA_BINDEX                       1
#define SA_PKA_CINDEX                       2
#define SA_PKA_DINDEX                       3
#define SA_PKA_RAM_SIZE                     0x2000
#define SA_PKA_RAM_SIZE_IN_WORDS            (SA_PKA_RAM_SIZE/sizeof(uint32_t))
#define SA_PKA_RAM_SCRATCHPAD_SIZE          96
#define SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS (SA_PKA_RAM_SCRATCHPAD_SIZE/sizeof(uint32_t))

#define SA_PKA_MAX_FIRMWARE_LOOP_COUNT   10000
#define SA_PKA_FIRMWARE_INIT_CODE        0x100

/* PKA A, B, C, B offsets in 32-bit words */
#define SA_PKA_A_OFFSET                  0x00
#define SA_PKA_B_OFFSET                  0x90
#define SA_PKA_C_OFFSET                  0x120
#define SA_PKA_D_OFFSET                  0x1B0
#endif

/* PKA A, B, C, B base in bytes */
#define SA_PKA_A_BASE                    (SA_PKA_RAM_BASE + (SA_PKA_A_OFFSET << 2))
#define SA_PKA_B_BASE                    (SA_PKA_RAM_BASE + (SA_PKA_B_OFFSET << 2))
#define SA_PKA_C_BASE                    (SA_PKA_RAM_BASE + (SA_PKA_C_OFFSET << 2))
#define SA_PKA_D_BASE                    (SA_PKA_RAM_BASE + (SA_PKA_D_OFFSET << 2))

#define SA_PKA_MAX_PKCP_OP_LOOP_COUNT    10000000

/* Operation codes of the PKA Complex Operations */
#define SA_PKA_COMPLEX_OP_MODEXP                (0x06 << 12)
#define SA_PKA_COMPLEX_OP_MODEXP_CRT            (0x01 << 12)
#define SA_PKA_COMPLEX_OP_MODINVp               (0x07 << 12)
#define SA_PKA_COMPLEX_OP_MODINV2m              (0x17 << 12)
#define SA_PKA_COMPLEX_OP_ECp_ADD               (0x10 << 12)
#define SA_PKA_COMPLEX_OP_ECp_MUL               (0x11 << 12)
#define SA_PKA_COMPLEX_OP_ECp_SCALE             (0x12 << 12)
#define SA_PKA_COMPLEX_OP_EC2m_ADD              (0x14 << 12)
#define SA_PKA_COMPLEX_OP_EC2m_MUL              (0x15 << 12)
#define SA_PKA_COMPLEX_OP_EC2m_SCALE            (0x16 << 12)
#define SA_PKA_COMPLEX_OP_ECpDSA_SIGN           (0x22 << 12)
#define SA_PKA_COMPLEX_OP_ECpDSA_VERIFY         (0x23 << 12)

/******************************************************************************
 * DATA DEFINITION:  SALLD PKA Instance structure
 ******************************************************************************
 * DESCRIPTION:  PKA Instance stores the intermediate data which are required
 *               to perform complex cryptography algorithm such as ECDSA and etc. 
 *               
 ******************************************************************************/
/* PKA operation state */
#define SA_PKA_OP_STATE_INIT            0
#define SA_PKA_OP_STATE_STEP1           1
#define SA_PKA_OP_STATE_STEP2           2
#define SA_PKA_OP_STATE_STEP3           3
#define SA_PKA_OP_STATE_STEP4           4
#define SA_PKA_OP_STATE_STEP5           5
#define SA_PKA_OP_STATE_STEP6           6
#define SA_PKA_OP_STATE_STEP7           7
#define SA_PKA_OP_STATE_STEP8           8
#define SA_PKA_OP_STATE_STEP9           9
#define SA_PKA_OP_STATE_STEP10          10
#define SA_PKA_OP_STATE_DONE            100

typedef struct pkaEcdsaSignInst_s
{
    uint32_t    offsetK;     /* scratch memory offset to K */
    uint32_t    offsetN;     /* scratch memory offset to integer order n */
    uint32_t    offsetD;     /* scratch memory offset to private key d */
    uint32_t    offsetH;     /* scratch memory offset to hash data h */
    uint32_t    offsetR;     /* scratch memory offset to output r */
    uint32_t    offsetS;     /* scratch memory offset to output s */
    uint32_t    offsetKinv;  /* scratch memory offset to (1/k) mod n */
    uint32_t    offsetP0;    /* scratch memory offset to P0 */
    uint32_t    offsetTmp1;  /* scratch memory offset to intermediate result */
}  pkaEcdsaSignInst_t;

typedef struct salldPkaInst_s {
    uint32_t    nextState;      /* PKA Next Operation state */
    uint32_t    offset;         /* offset to output in word32 */
    union {                
        pkaEcdsaSignInst_t  ecdsaSign;  /* ECDSA Sign instance */
    } inst;                     /* Operation specific instance structure */
} salldPkaInst_t;

#endif /* _SAPKA_H */

