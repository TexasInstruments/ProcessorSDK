/*
 *
 * Copyright (C) 2012-2020 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
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

#include "../unittest.h"
#include "../salldsim/salldsim.h"
#include "test5_3.h"

/* PKA (Public Key Accelerator) Complex Operation test.
 * This test is designed to verify the complex opertions of the Public Key Accelerator
 * (PKA) within SASS.
 *
 * Test Procedures:
 * 
 * - Enable PKA by invoking API Sa_pkaInit()
 * - Run through the PKA test vectors of all supported operations by invoking API Sa_pkaOperation2() 
 * - Close PKA by invoking API Sa_pkaClose()
 * 
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 * 
 */
#define SA_PKA_MAX_IN_VECTORS     16    /* Maximum number of input arrays */
#define SA_PAK_MAX_OUT_VECTORS     5    /* Maximum number of output arrays */

typedef struct saPkaTestEntry2_s
{
    char            opDesc[40];         /* Operation description */
    Sa_PkaOpTypes_t operation;      /* PKA operation */
    Sa_PkaOpModes_t mode;           /* PKA I/O mode */
    uint16_t        aLen;           /* A length */
    uint16_t        bLen;           /* B Length */
    uint32_t        numOddPowers;   /* Number of pre-calculated odd powers to use */
    int             statusCode;     /* Expected status code of opertaion */
    uint32_t*       pIn[SA_PKA_MAX_IN_VECTORS];    /* Pointers to the input arrays */
    uint32_t*       pOut[SA_PAK_MAX_OUT_VECTORS];  /* Pointers to the output arrays */
} saPkaTestEntry2_t; 
 
static uint32_t  pkaOutput[SA_PAK_MAX_OUT_VECTORS][sa_MAX_PKA_OPERAND_SIZE];  /* actual output */
 
static saPkaTestEntry2_t  saPkaTestEntry[] =
{

    /* test entry 1 */
    {
        "ModExp 4096bits",         /* Operation description */
        sa_PKA_OP_MODEXP,          /* PKA operation */
        sa_PKA_MODE_WAIT,          /* PKA I/O mode */
        1,                         /* A length */
        128,                       /* B Length*/  
        2,                         /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,  /* Expected status code of opertaion */
        {                          /*  Pointers to the input arrays */
            pka_modExp_n4096,
            pka_modExp_e4096,
            pka_modExp_m4096,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_modExp_r4096,
            0,
        
        }
    },
    
    /* test entry 2 */
    {
        "ModExpCRT 4096bits",      /* Operation description */
        sa_PKA_OP_MODEXP_CRT,      /* PKA operation */
        sa_PKA_MODE_WAIT,          /* PKA I/O mode */
        64,                        /* A length */
        64,                       /* B Length*/  
        1,                         /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,  /* Expected status code of opertaion */
        {                          /*  Pointers to the input arrays */
            pka_modExpCRT_p4096,
            pka_modExpCRT_q4096,
            pka_modExpCRT_Dp4096,
            pka_modExpCRT_Dq4096,
            pka_modExpCRT_qInv4096,
            pka_modExpCRT_m4096,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_modExpCRT_r4096,
            0,
        
        }
    },
    
    /* test entry 3 */
    {
        "ModInv_p P_521",          /* Operation description */
        sa_PKA_OP_MODINVp,         /* PKA operation */
        sa_PKA_MODE_WAIT,          /* PKA I/O mode */
        17,                        /* A length */
        17,                       /* B Length*/  
        0,                         /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,  /* Expected status code of opertaion */
        {                          /*  Pointers to the input arrays */
            pka_modINVp_n521,
            pka_modINVp_z521,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_modINVp_r521,
            0,
        }
    },

    /* test entry 4 */
    {
        "ModInv_2m curve B_571",   /* Operation description */
        sa_PKA_OP_MODINV2m,        /* PKA operation */
        sa_PKA_MODE_WAIT,          /* PKA I/O mode */
        18,                        /* A length */
        18,                       /* B Length*/  
        0,                         /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,  /* Expected status code of opertaion */
        {                          /*  Pointers to the input arrays */
            pka_modINV2m_p571,
            pka_modINV2m_z571,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_modINV2m_r571,
            0,
        }
    },
    
    /* test entry 5 */
    {
        "ECpADDxyz curve P_521",    /* Operation description */
        sa_PKA_OP_ECp_ADD,          /* PKA operation */
        sa_PKA_MODE_WAIT,           /* PKA I/O mode */
        17,                         /* A length */
        17,                         /* B Length*/  
        0,                          /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,   /* Expected status code of opertaion */
        {                           /*  Pointers to the input arrays */
            pka_ecAddp_p521,
            pka_ecAddp_a521,
            pka_ecAddp_b521,
            pka_ecAddp_p1x521,
            pka_ecAddp_p1y521,
            pka_ecAddp_p1z521,
            pka_ecAddp_p2x521,
            pka_ecAddp_p2y521,
            pka_ecAddp_p2z521,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_ecAddp_p0x521,
            pka_ecAddp_p0y521,
            pka_ecAddp_p0z521,
            0,
        }
    },
    
    /* test entry 6 */
    {
        "EC2mADDxyz curve B_571",   /* Operation description */
        sa_PKA_OP_EC2m_ADD,         /* PKA operation */
        sa_PKA_MODE_WAIT,           /* PKA I/O mode */
        18,                         /* A length */
        18,                         /* B Length*/  
        0,                          /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,   /* Expected status code of opertaion */
        {                           /*  Pointers to the input arrays */
            pka_ecAdd2m_p571,
            pka_ecAdd2m_a571,
            pka_ecAdd2m_b571,
            pka_ecAdd2m_p1x571,
            pka_ecAdd2m_p1y571,
            pka_ecAdd2m_p1z571,
            pka_ecAdd2m_p2x571,
            pka_ecAdd2m_p2y571,
            pka_ecAdd2m_p2z571,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_ecAdd2m_p0x571,
            pka_ecAdd2m_p0y571,
            pka_ecAdd2m_p0z571,
            0,
        }
    },
    
    /* test entry 7 */
    {
        "ECpMULxyz curve SEC_P_224_R1",    /* Operation description */
        sa_PKA_OP_ECp_MUL,          /* PKA operation */
        sa_PKA_MODE_WAIT,           /* PKA I/O mode */
        1,                          /* A length */
        7,                          /* B Length*/  
        0,                          /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,   /* Expected status code of opertaion */
        {                           /*  Pointers to the input arrays */
            pka_ecMulp_p224,
            pka_ecMulp_a224,
            pka_ecMulp_b224,
            pka_ecMulp_k224,
            pka_ecMulp_p1x224,
            pka_ecMulp_p1y224,
            pka_ecMulp_p1z224,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_ecMulp_p0x224,
            pka_ecMulp_p0y224,
            pka_ecMulp_p0z224,
            0,
        }
    },
    
    /* test entry 8 */
    {
        "EC2mMULxyz curve B_409",   /* Operation description */
        sa_PKA_OP_EC2m_MUL,         /* PKA operation */
        sa_PKA_MODE_WAIT,           /* PKA I/O mode */
        13,                          /* A length */
        13,                          /* B Length*/  
        0,                          /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,   /* Expected status code of opertaion */
        {                           /*  Pointers to the input arrays */
            pka_ecMul2m_p409,
            pka_ecMul2m_a409,
            pka_ecMul2m_c409,
            pka_ecMul2m_k409,
            pka_ecMul2m_p1x409,
            pka_ecMul2m_p1y409,
            pka_ecMul2m_p1z409,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_ecMul2m_p0x409,
            pka_ecMul2m_p0y409,
            pka_ecMul2m_p0z409,
            0,
        }
    },
    
    /* test entry 9 */
    {
        "ECpMUL_SCALExyz curve NIST_P_384",  /* Operation description */
        sa_PKA_OP_ECp_MUL_SACLE,    /* PKA operation */
        sa_PKA_MODE_WAIT,           /* PKA I/O mode */
        1,                          /* A length */
        12,                         /* B Length*/  
        0,                          /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,   /* Expected status code of opertaion */
        {                           /*  Pointers to the input arrays */
            pka_ecMulSp_p384,
            pka_ecMulSp_a384,
            pka_ecMulSp_b384,
            pka_ecMulSp_k384,
            pka_ecMulSp_p1x384,
            pka_ecMulSp_p1y384,
            pka_ecMulSp_p1z384,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_ecMulSp_p0x384,
            pka_ecMulSp_p0y384,
            pka_ecMulSp_p0z384,
            0,
        }
    },
    
    /* test entry 10 */
    {
        "EC2mMUL_SCALExyz curve K571",  /* Operation description */
        sa_PKA_OP_EC2m_MUL_SACLE,   /* PKA operation */
        sa_PKA_MODE_WAIT,           /* PKA I/O mode */
        1,                          /* A length */
        18,                         /* B Length*/  
        0,                          /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,   /* Expected status code of opertaion */
        {                           /*  Pointers to the input arrays */
            pka_ecMulS2m_p571,
            pka_ecMulS2m_a571,
            pka_ecMulS2m_c571,
            pka_ecMulS2m_k571,
            pka_ecMulS2m_p1x571,
            pka_ecMulS2m_p1y571,
            pka_ecMulS2m_p1z571,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_ecMulS2m_p0x571,
            pka_ecMulS2m_p0y571,
            pka_ecMulS2m_p0z571,
            0,
        }
    },
    
    /* test entry 11 */
    {
        "ECDSAp Sign curve P521",   /* Operation description */
        sa_PKA_OP_ECp_DSA_SIGN,     /* PKA operation */
        sa_PKA_MODE_WAIT,           /* PKA I/O mode */
        17,                         /* A length */
        17,                         /* B Length*/  
        0,                          /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,   /* Expected status code of opertaion */
        {                           /*  Pointers to the input arrays */
            pka_ecdsap_sign_p521,
            pka_ecdsap_sign_a521,
            pka_ecdsap_sign_b521,
            pka_ecdsap_sign_k521,
            pka_ecdsap_sign_p1x521,
            pka_ecdsap_sign_p1y521,
            pka_ecdsap_sign_p1z521,
            pka_ecdsap_sign_n521,
            pka_ecdsap_sign_h521,
            pka_ecdsap_sign_d521,
            0
            
        },
        {                          /* Pointers to the output arrays */
            pka_ecdsap_sign_r521,
            pka_ecdsap_sign_s521,
            0,
        }
    },

    /* test entry 12 */
    {
        "ECDSAp Verify curve P160", /* Operation description */
        sa_PKA_OP_ECp_DSA_VERIFY,   /* PKA operation */
        sa_PKA_MODE_WAIT,           /* PKA I/O mode */
        5,                          /* A length */
        5,                          /* B Length*/
        0,                          /* Number of pre-calculated odd powers to use */
        sa_PKA_OP_STATUS_SUCCESS,   /* Expected status code of opertaion */
        {                           /*  Pointers to the input arrays */
            pka_ecdsap_verify_p160,
            pka_ecdsap_verify_a160,
            pka_ecdsap_verify_b160,
            pka_ecdsap_verify_p1x160,
            pka_ecdsap_verify_p1y160,
            pka_ecdsap_verify_p1z160,
            pka_ecdsap_verify_n160,
            pka_ecdsap_verify_h160,
            pka_ecdsap_verify_yx160,
            pka_ecdsap_verify_yy160,
            pka_ecdsap_verify_yz160,
            pka_ecdsap_verify_r160,
            pka_ecdsap_verify_s160,
            0
        },
        {                          /* Pointers to the output arrays */
            pka_ecdsap_verify_res160,
            0,
        }
    }

};

/*******************************************************************************
 *  Function: Construct the PKA Operation Request Information
 *******************************************************************************
 *  DESCRIPTION: Construct the PKA operation request information 
 *  (Sa_PkaReqInfo2_t) from the test entry 
 *
 ******************************************************************************/
void saConvPkaInputs(saPkaTestEntry2_t *pPkaEntry, Sa_PkaReqInfo2_t *pPkaReq)
{
    memset(pPkaReq, 0, sizeof(Sa_PkaReqInfo2_t));

    /* Construct the common portion of PKA request structure */
    pPkaReq->operation = pPkaEntry->operation;
    pPkaReq->mode = pPkaEntry->mode;
    pPkaReq->aLen = pPkaEntry->aLen;
    pPkaReq->bLen = pPkaEntry->bLen;
    
    /* Construct the operation-specific parameters */
    switch (pPkaReq->operation)
    {
        case sa_PKA_OP_MODEXP:
            {
                Sa_PkaModExpParams_t *pPkaOp = &pPkaReq->params.modExp;
            
                pPkaOp->numOddPowers =  pPkaEntry->numOddPowers;
                pPkaOp->pN           =  pPkaEntry->pIn[0];   
                pPkaOp->pE           =  pPkaEntry->pIn[1];   
                pPkaOp->pM           =  pPkaEntry->pIn[2];   
                pPkaOp->pResult       =  pkaOutput[0];  
            }
            break;
        
        case sa_PKA_OP_MODEXP_CRT:
            {
                Sa_PkaModExpCRTParams_t *pPkaOp = &pPkaReq->params.modExpCRT;
            
                pPkaOp->numOddPowers =  pPkaEntry->numOddPowers;
                pPkaOp->pModP        =  pPkaEntry->pIn[0];   
                pPkaOp->pModQ        =  pPkaEntry->pIn[1];   
                pPkaOp->pDp          =  pPkaEntry->pIn[2];   
                pPkaOp->pDq          =  pPkaEntry->pIn[3];   
                pPkaOp->pQInv        =  pPkaEntry->pIn[4];   
                pPkaOp->pM           =  pPkaEntry->pIn[5];   
                pPkaOp->pResult       =  pkaOutput[0];  
            }
            break;
        
        case sa_PKA_OP_MODINVp:
        case sa_PKA_OP_MODINV2m:
            {
                Sa_PkaModInvParams_t *pPkaOp = &pPkaReq->params.modInv;
            
                pPkaOp->pN           =  pPkaEntry->pIn[0];   
                pPkaOp->pZ           =  pPkaEntry->pIn[1];   
                pPkaOp->pResult       =  pkaOutput[0];  
            }
            break;
        
        case sa_PKA_OP_ECp_ADD:
        case sa_PKA_OP_EC2m_ADD:
            {
                Sa_PkaECAddParams_t *pPkaOp = &pPkaReq->params.ecAdd;
            
                pPkaOp->pModP        =  pPkaEntry->pIn[0];   
                pPkaOp->pEcA         =  pPkaEntry->pIn[1];   
                pPkaOp->pEcB         =  pPkaEntry->pIn[2];   
                pPkaOp->point1.pX    =  pPkaEntry->pIn[3];   
                pPkaOp->point1.pY    =  pPkaEntry->pIn[4];   
                pPkaOp->point1.pZ    =  pPkaEntry->pIn[5];   
                pPkaOp->point2.pX    =  pPkaEntry->pIn[6];   
                pPkaOp->point2.pY    =  pPkaEntry->pIn[7];   
                pPkaOp->point2.pZ    =  pPkaEntry->pIn[8];   
                pPkaOp->point0.pX    =  pkaOutput[0];   
                pPkaOp->point0.pY    =  pkaOutput[1];   
                pPkaOp->point0.pZ    =  pkaOutput[2];   
            }
            break;
            
        case sa_PKA_OP_ECp_MUL:
        case sa_PKA_OP_EC2m_MUL:
        case sa_PKA_OP_ECp_MUL_SACLE:
        case sa_PKA_OP_EC2m_MUL_SACLE:
        
            {
                Sa_PkaECMulParams_t *pPkaOp = &pPkaReq->params.ecMul;
            
                pPkaOp->pModP        =  pPkaEntry->pIn[0];   
                pPkaOp->pEcA         =  pPkaEntry->pIn[1];   
                pPkaOp->pEcBC        =  pPkaEntry->pIn[2];   
                pPkaOp->pK           =  pPkaEntry->pIn[3];   
                pPkaOp->point1.pX    =  pPkaEntry->pIn[4];   
                pPkaOp->point1.pY    =  pPkaEntry->pIn[5];   
                pPkaOp->point1.pZ    =  pPkaEntry->pIn[6];   
                pPkaOp->point0.pX    =  pkaOutput[0];   
                pPkaOp->point0.pY    =  pkaOutput[1];   
                pPkaOp->point0.pZ    =  pkaOutput[2];   
            }
            break;
            
        case sa_PKA_OP_ECp_SCALE:
        case sa_PKA_OP_EC2m_SCALE:
            {
                Sa_PkaECScaleParams_t *pPkaOp = &pPkaReq->params.ecScale;
            
                pPkaOp->pModP        =  pPkaEntry->pIn[0];   
                pPkaOp->pEcA         =  pPkaEntry->pIn[1];   
                pPkaOp->pEcB         =  pPkaEntry->pIn[2];   
                pPkaOp->point1.pX    =  pPkaEntry->pIn[3];   
                pPkaOp->point1.pY    =  pPkaEntry->pIn[4];   
                pPkaOp->point1.pZ    =  pPkaEntry->pIn[5];   
                pPkaOp->point0.pX    =  pkaOutput[0];   
                pPkaOp->point0.pY    =  pkaOutput[1];   
                pPkaOp->point0.pZ    =  pkaOutput[2];   
            }
            break;
            
        case sa_PKA_OP_ECp_DSA_SIGN:
            {
                Sa_PkaECDSASignParams_t *pPkaOp = &pPkaReq->params.ecDSASign;
            
                pPkaOp->pModP        =  pPkaEntry->pIn[0];   
                pPkaOp->pEcA         =  pPkaEntry->pIn[1];   
                pPkaOp->pEcBC        =  pPkaEntry->pIn[2];   
                pPkaOp->pK           =  pPkaEntry->pIn[3];   
                pPkaOp->point1.pX    =  pPkaEntry->pIn[4];   
                pPkaOp->point1.pY    =  pPkaEntry->pIn[5];   
                pPkaOp->point1.pZ    =  pPkaEntry->pIn[6];   
                pPkaOp->pN           =  pPkaEntry->pIn[7];   
                pPkaOp->pH           =  pPkaEntry->pIn[8];   
                pPkaOp->pD           =  pPkaEntry->pIn[9];   
                pPkaOp->pR           =  pkaOutput[0];   
                pPkaOp->pS           =  pkaOutput[1];   
            }
            break;

        case sa_PKA_OP_ECp_DSA_VERIFY:
            {
                Sa_PkaECDSAVerifyParams_t *pPkaOp = &pPkaReq->params.ecDSAVerify;

                pPkaOp->pModP        =  pPkaEntry->pIn[0];
                pPkaOp->pEcA         =  pPkaEntry->pIn[1];
                pPkaOp->pEcBC        =  pPkaEntry->pIn[2];
                pPkaOp->point1.pX    =  pPkaEntry->pIn[3];
                pPkaOp->point1.pY    =  pPkaEntry->pIn[4];
                pPkaOp->point1.pZ    =  pPkaEntry->pIn[5];
                pPkaOp->pN           =  pPkaEntry->pIn[6];
                pPkaOp->pH           =  pPkaEntry->pIn[7];
                pPkaOp->pY.pX        =  pPkaEntry->pIn[8];
                pPkaOp->pY.pY        =  pPkaEntry->pIn[9];
                pPkaOp->pY.pZ        =  pPkaEntry->pIn[10];
                pPkaOp->pR           =  pPkaEntry->pIn[11];
                pPkaOp->pS           =  pPkaEntry->pIn[12];
                pPkaOp->pRes         =  pkaOutput[0];
            }
            break;

        default:
            break;
    
    }
}

/*******************************************************************************
 *  Function: Verify PKA Operation Results
 *******************************************************************************
 *  DESCRIPTION: Compare PKA operation results against expected values 
 *
 *  Return: 0 : Test outputs match expected values
 *          -1: Test outputs does not match
 ******************************************************************************/
int32_t saVerifyPkaResults(saPkaTestEntry2_t *pPkaEntry, Sa_PkaReqInfo2_t *pPkaReq)
{
    /* Verify operation-specific results */
    switch (pPkaReq->operation)
    {
        case sa_PKA_OP_MODEXP:
        case sa_PKA_OP_MODINVp:
        case sa_PKA_OP_MODINV2m:
            {
                if (memcmp(pPkaEntry->pOut[0], pkaOutput[0], pPkaReq->bLen*4))
                {
                    return (-1);
                }
            }
            break;
        
        case sa_PKA_OP_MODEXP_CRT:
            {
                if (memcmp(pPkaEntry->pOut[0], pkaOutput[0], pPkaReq->bLen*8))
                {
                    return (-1);
                }
            }
            break;
        
        case sa_PKA_OP_ECp_ADD:
        case sa_PKA_OP_EC2m_ADD:
        case sa_PKA_OP_ECp_MUL:
        case sa_PKA_OP_EC2m_MUL:
        case sa_PKA_OP_ECp_MUL_SACLE:
        case sa_PKA_OP_EC2m_MUL_SACLE:
            {
                if (memcmp(pPkaEntry->pOut[0], pkaOutput[0], pPkaReq->bLen*4) ||
                    memcmp(pPkaEntry->pOut[1], pkaOutput[1], pPkaReq->bLen*4) ||
                    memcmp(pPkaEntry->pOut[2], pkaOutput[2], pPkaReq->bLen*4))
                {
                    return (-1);
                }
            }
            break;
            
        case sa_PKA_OP_ECp_DSA_SIGN:
            {
                if (memcmp(pPkaEntry->pOut[0], pkaOutput[0], pPkaReq->bLen*4) ||
                    memcmp(pPkaEntry->pOut[1], pkaOutput[1], pPkaReq->bLen*4))
                {
                    return (-1);
                }
            }
            break;
            
        case sa_PKA_OP_ECp_DSA_VERIFY:
            {
              if (memcmp(pPkaEntry->pOut[0], pkaOutput[0], 4))
              {
                   return (-1);
              }
            }
            break;

        default:
            return (-1);
    }
    
    return (0);
}

/*******************************************************************************
 *  Function: PKA Test Program #2
 *******************************************************************************
 *  DESCRIPTION: Perform vector-based tests of PKA complex operations 
 *
 ******************************************************************************/
void saPkaTest2 (void* a0, void* a1)
{
 	tFramework_t  *tf  = (tFramework_t *)(uintptr_t)a0;
 	saTest_t      *pat = (saTest_t *)(uintptr_t)a1;
 	saPkaTestEntry2_t *pPkaEntry;
    Sa_PkaReqInfo2_t   pkaReq;
 	int  i;
    int16_t retCode;
    uint32_t           opStartTime;
    uint32_t           opDuration;

    /* Enable Error logging */
    salld_sim_disp_control(TRUE);    
    
    /* Initialize the PKA module to be ready for large number arithmetic */
    retCode = Sa_pkaInit(tf->salld_handle);
    
    if (retCode != sa_ERR_OK)
    {
        salld_sim_print("saPkaTest2: Sa_pkaInit() returns error code = %d!\n", retCode);
        salld_sim_disp_control(FALSE);    
        
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    /* Wait for the Module to be ready */
	utilCycleDelay (1000);
    
    /* PKA Test */
    for (i = 0; i < sizeof(saPkaTestEntry)/sizeof(saPkaTestEntry2_t); i++)
    {
        pPkaEntry = &saPkaTestEntry[i];
        /* Prepare the PKA request Info */
        saConvPkaInputs(pPkaEntry, &pkaReq);
        
        opStartTime = TimerP_getTimeInUsecs();

        retCode = Sa_pkaOperation2(tf->salld_handle, &pkaReq);

        opDuration = TimerP_getTimeInUsecs() - opStartTime;

        salld_sim_print("saPkaTest2: Test %d (%s) takes %d micro seconds!\n", i, pPkaEntry->opDesc, opDuration);
        
        if (retCode != sa_ERR_OK)
        {
            salld_sim_print("saPkaTest2: Sa_pkaOperation2() returns error code = %d!\n", retCode);
            salld_sim_disp_control(FALSE);    
        
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Check operation results */
        if (pkaReq.statusCode != pPkaEntry->statusCode)
        {
            salld_sim_print("saPkaTest2: test %d fails! statusCode (%d) is not expected, should be (%d)\n",
                             i, pkaReq.statusCode, pPkaEntry->statusCode);
        }
        
        if (saVerifyPkaResults(pPkaEntry, &pkaReq))
        {
            salld_sim_print("saPkaTest2: test %d fails! output data mismatches\n", i);
            salld_sim_disp_control(FALSE);    
 	        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
    }    
    
    /* Close the PKA module */
    retCode = Sa_pkaClose(tf->salld_handle);
    
    if (retCode != sa_ERR_OK)
    {
        salld_sim_print("saPkaTest2: Sa_pkaClose() returns error code = %d!\n", retCode);
        salld_sim_disp_control(FALSE);    
        
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    salld_sim_print("saPkaTest2: %d tests complete successfully!\n", sizeof(saPkaTestEntry)/sizeof(saPkaTestEntry2_t));
    salld_sim_disp_control(FALSE);    

 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}
	
	
	
		
	


  		
