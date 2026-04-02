/********************************************************************
 * Copyright (C) 2013-2014 Texas Instruments Incorporated.
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
#ifndef CSLR_EIP_29T2_RAM_H_
#define CSLR_EIP_29T2_RAM_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t PKA_APTR;
    volatile uint32_t PKA_BPTR;
    volatile uint32_t PKA_CPTR;
    volatile uint32_t PKA_DPTR;
    volatile uint32_t PKA_ALENGTH;
    volatile uint32_t PKA_BLENGTH;
    volatile uint32_t PKA_SHIFT;
    volatile uint32_t PKA_FUNCTION;
    volatile uint32_t PKA_COMPARE;
    volatile uint32_t PKA_MSW;
    volatile uint32_t PKA_DIVMSW;
    volatile uint8_t  Resv_64[20];
    volatile uint32_t LNME1_STATUS;
    volatile uint32_t LNME1_CONTROL;
    volatile uint8_t  Resv_96[24];
    volatile uint32_t LNME1_NBASE;
    volatile uint32_t LNME1_XBASE;
    volatile uint32_t LNME1_YBASE;
    volatile uint32_t LNME1_BBASE;
    volatile uint32_t LNME1_NACC;
    volatile uint32_t LNME1_NZERO;
    volatile uint8_t  Resv_128[8];
    volatile uint32_t LNME0_STATUS;
    volatile uint32_t LNME0_CONTROL;
    volatile uint32_t LNME_DATAPATH;
    volatile uint8_t  Resv_144[4];
    volatile uint32_t LNME_FAST_CTRL;
    volatile uint32_t LNME_FAST_STRT;
    volatile uint32_t LNME_FAST_MMM;
    volatile uint8_t  Resv_160[4];
    volatile uint32_t LNME0_NBASE;
    volatile uint32_t LNME0_XBASE;
    volatile uint32_t LNME0_YBASE;
    volatile uint32_t LNME0_BBASE;
    volatile uint32_t LNME0_NACC;
    volatile uint32_t LNME0_NZERO;
    volatile uint8_t  Resv_200[16];
    volatile uint32_t PKA_SEQ_CTRL;
    volatile uint8_t  Resv_244[40];
    volatile uint32_t PKA_OPTION;
    volatile uint32_t PKA_SW_REV;
    volatile uint32_t PKA_REVISION;
    volatile uint8_t  Resv_1024[768];
    volatile uint32_t GF2M_OPERAND_A_0;
    volatile uint32_t GF2M_OPERAND_A_1;
    volatile uint32_t GF2M_OPERAND_A_2;
    volatile uint32_t GF2M_OPERAND_A_3;
    volatile uint32_t GF2M_OPERAND_A_4;
    volatile uint32_t GF2M_OPERAND_A_5;
    volatile uint32_t GF2M_OPERAND_A_6;
    volatile uint32_t GF2M_OPERAND_A_7;
    volatile uint32_t GF2M_OPERAND_A_8;
    volatile uint32_t GF2M_OPERAND_A_9;
    volatile uint32_t GF2M_OPERAND_A_10;
    volatile uint32_t GF2M_OPERAND_A_11;
    volatile uint32_t GF2M_OPERAND_A_12;
    volatile uint32_t GF2M_OPERAND_A_13;
    volatile uint32_t GF2M_OPERAND_A_14;
    volatile uint32_t GF2M_OPERAND_A_15;
    volatile uint32_t GF2M_OPERAND_A_16;
    volatile uint32_t GF2M_OPERAND_A_17;
    volatile uint8_t  Resv_1152[56];
    volatile uint32_t GF2M_OPERAND_B_0;
    volatile uint32_t GF2M_OPERAND_B_1;
    volatile uint32_t GF2M_OPERAND_B_2;
    volatile uint32_t GF2M_OPERAND_B_3;
    volatile uint32_t GF2M_OPERAND_B_4;
    volatile uint32_t GF2M_OPERAND_B_5;
    volatile uint32_t GF2M_OPERAND_B_6;
    volatile uint32_t GF2M_OPERAND_B_7;
    volatile uint32_t GF2M_OPERAND_B_8;
    volatile uint32_t GF2M_OPERAND_B_9;
    volatile uint32_t GF2M_OPERAND_B_10;
    volatile uint32_t GF2M_OPERAND_B_11;
    volatile uint32_t GF2M_OPERAND_B_12;
    volatile uint32_t GF2M_OPERAND_B_13;
    volatile uint32_t GF2M_OPERAND_B_14;
    volatile uint32_t GF2M_OPERAND_B_15;
    volatile uint32_t GF2M_OPERAND_B_16;
    volatile uint32_t GF2M_OPERAND_B_17;
    volatile uint8_t  Resv_1280[56];
    volatile uint32_t GF2M_OPERAND_C_0;
    volatile uint32_t GF2M_OPERAND_C_1;
    volatile uint32_t GF2M_OPERAND_C_2;
    volatile uint32_t GF2M_OPERAND_C_3;
    volatile uint32_t GF2M_OPERAND_C_4;
    volatile uint32_t GF2M_OPERAND_C_5;
    volatile uint32_t GF2M_OPERAND_C_6;
    volatile uint32_t GF2M_OPERAND_C_7;
    volatile uint32_t GF2M_OPERAND_C_8;
    volatile uint32_t GF2M_OPERAND_C_9;
    volatile uint32_t GF2M_OPERAND_C_10;
    volatile uint32_t GF2M_OPERAND_C_11;
    volatile uint32_t GF2M_OPERAND_C_12;
    volatile uint32_t GF2M_OPERAND_C_13;
    volatile uint32_t GF2M_OPERAND_C_14;
    volatile uint32_t GF2M_OPERAND_C_15;
    volatile uint32_t GF2M_OPERAND_C_16;
    volatile uint32_t GF2M_OPERAND_C_17;
    volatile uint8_t  Resv_1408[56];
    volatile uint32_t GF2M_OPERAND_D_0;
    volatile uint32_t GF2M_OPERAND_D_1;
    volatile uint32_t GF2M_OPERAND_D_2;
    volatile uint32_t GF2M_OPERAND_D_3;
    volatile uint32_t GF2M_OPERAND_D_4;
    volatile uint32_t GF2M_OPERAND_D_5;
    volatile uint32_t GF2M_OPERAND_D_6;
    volatile uint32_t GF2M_OPERAND_D_7;
    volatile uint32_t GF2M_OPERAND_D_8;
    volatile uint32_t GF2M_OPERAND_D_9;
    volatile uint32_t GF2M_OPERAND_D_10;
    volatile uint32_t GF2M_OPERAND_D_11;
    volatile uint32_t GF2M_OPERAND_D_12;
    volatile uint32_t GF2M_OPERAND_D_13;
    volatile uint32_t GF2M_OPERAND_D_14;
    volatile uint32_t GF2M_OPERAND_D_15;
    volatile uint32_t GF2M_OPERAND_D_16;
    volatile uint32_t GF2M_OPERAND_D_17;
    volatile uint8_t  Resv_1536[56];
    volatile uint32_t GF2M_POLYNOMIAL_0;
    volatile uint32_t GF2M_POLYNOMIAL_1;
    volatile uint32_t GF2M_POLYNOMIAL_2;
    volatile uint32_t GF2M_POLYNOMIAL_3;
    volatile uint32_t GF2M_POLYNOMIAL_4;
    volatile uint32_t GF2M_POLYNOMIAL_5;
    volatile uint32_t GF2M_POLYNOMIAL_6;
    volatile uint32_t GF2M_POLYNOMIAL_7;
    volatile uint32_t GF2M_POLYNOMIAL_8;
    volatile uint32_t GF2M_POLYNOMIAL_9;
    volatile uint32_t GF2M_POLYNOMIAL_10;
    volatile uint32_t GF2M_POLYNOMIAL_11;
    volatile uint32_t GF2M_POLYNOMIAL_12;
    volatile uint32_t GF2M_POLYNOMIAL_13;
    volatile uint32_t GF2M_POLYNOMIAL_14;
    volatile uint32_t GF2M_POLYNOMIAL_15;
    volatile uint32_t GF2M_POLYNOMIAL_16;
    volatile uint32_t GF2M_POLYNOMIAL_17;
    volatile uint8_t  Resv_1792[184];
    volatile uint32_t GF2M_CMD;
    volatile uint32_t GF2M_STAT;
    volatile uint32_t GF2M_FIELDSIZE;
    volatile uint8_t  Resv_2040[236];
    volatile uint32_t GF2M_OPTIONS;
    volatile uint32_t GF2M_VERSION;
    volatile uint8_t  Resv_8160[6112];
    volatile uint32_t PKA_REV;
    volatile uint8_t  Resv_8168[4];
    volatile uint32_t PKA_CLK_CTRL;
    volatile uint8_t  Resv_8176[4];
    volatile uint32_t PKA_SYSCONFIG;
    volatile uint32_t PKA_SYSSTATUS;
    volatile uint32_t PKA_IRQSTATUS;
    volatile uint32_t PKA_IRQENABLE;
} CSL_hsm_pkaRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_HSM_PKA_PKA_APTR                                                   (0x00000000U)
#define CSL_HSM_PKA_PKA_BPTR                                                   (0x00000004U)
#define CSL_HSM_PKA_PKA_CPTR                                                   (0x00000008U)
#define CSL_HSM_PKA_PKA_DPTR                                                   (0x0000000CU)
#define CSL_HSM_PKA_PKA_ALENGTH                                                (0x00000010U)
#define CSL_HSM_PKA_PKA_BLENGTH                                                (0x00000014U)
#define CSL_HSM_PKA_PKA_SHIFT                                                  (0x00000018U)
#define CSL_HSM_PKA_PKA_FUNCTION                                               (0x0000001CU)
#define CSL_HSM_PKA_PKA_COMPARE                                                (0x00000020U)
#define CSL_HSM_PKA_PKA_MSW                                                    (0x00000024U)
#define CSL_HSM_PKA_PKA_DIVMSW                                                 (0x00000028U)
#define CSL_HSM_PKA_LNME1_STATUS                                               (0x00000040U)
#define CSL_HSM_PKA_LNME1_CONTROL                                              (0x00000044U)
#define CSL_HSM_PKA_LNME1_NBASE                                                (0x00000060U)
#define CSL_HSM_PKA_LNME1_XBASE                                                (0x00000064U)
#define CSL_HSM_PKA_LNME1_YBASE                                                (0x00000068U)
#define CSL_HSM_PKA_LNME1_BBASE                                                (0x0000006CU)
#define CSL_HSM_PKA_LNME1_NACC                                                 (0x00000070U)
#define CSL_HSM_PKA_LNME1_NZERO                                                (0x00000074U)
#define CSL_HSM_PKA_LNME0_STATUS                                               (0x00000080U)
#define CSL_HSM_PKA_LNME0_CONTROL                                              (0x00000084U)
#define CSL_HSM_PKA_LNME_DATAPATH                                              (0x00000088U)
#define CSL_HSM_PKA_LNME_FAST_CTRL                                             (0x00000090U)
#define CSL_HSM_PKA_LNME_FAST_STRT                                             (0x00000094U)
#define CSL_HSM_PKA_LNME_FAST_MMM                                              (0x00000098U)
#define CSL_HSM_PKA_LNME0_NBASE                                                (0x000000A0U)
#define CSL_HSM_PKA_LNME0_XBASE                                                (0x000000A4U)
#define CSL_HSM_PKA_LNME0_YBASE                                                (0x000000A8U)
#define CSL_HSM_PKA_LNME0_BBASE                                                (0x000000ACU)
#define CSL_HSM_PKA_LNME0_NACC                                                 (0x000000B0U)
#define CSL_HSM_PKA_LNME0_NZERO                                                (0x000000B4U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL                                               (0x000000C8U)
#define CSL_HSM_PKA_PKA_OPTION                                                 (0x000000F4U)
#define CSL_HSM_PKA_PKA_SW_REV                                                 (0x000000F8U)
#define CSL_HSM_PKA_PKA_REVISION                                               (0x000000FCU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_0                                           (0x00000400U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_1                                           (0x00000404U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_2                                           (0x00000408U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_3                                           (0x0000040CU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_4                                           (0x00000410U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_5                                           (0x00000414U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_6                                           (0x00000418U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_7                                           (0x0000041CU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_8                                           (0x00000420U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_9                                           (0x00000424U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_10                                          (0x00000428U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_11                                          (0x0000042CU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_12                                          (0x00000430U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_13                                          (0x00000434U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_14                                          (0x00000438U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_15                                          (0x0000043CU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_16                                          (0x00000440U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_17                                          (0x00000444U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_0                                           (0x00000480U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_1                                           (0x00000484U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_2                                           (0x00000488U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_3                                           (0x0000048CU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_4                                           (0x00000490U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_5                                           (0x00000494U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_6                                           (0x00000498U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_7                                           (0x0000049CU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_8                                           (0x000004A0U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_9                                           (0x000004A4U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_10                                          (0x000004A8U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_11                                          (0x000004ACU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_12                                          (0x000004B0U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_13                                          (0x000004B4U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_14                                          (0x000004B8U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_15                                          (0x000004BCU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_16                                          (0x000004C0U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_17                                          (0x000004C4U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_0                                           (0x00000500U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_1                                           (0x00000504U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_2                                           (0x00000508U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_3                                           (0x0000050CU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_4                                           (0x00000510U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_5                                           (0x00000514U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_6                                           (0x00000518U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_7                                           (0x0000051CU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_8                                           (0x00000520U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_9                                           (0x00000524U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_10                                          (0x00000528U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_11                                          (0x0000052CU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_12                                          (0x00000530U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_13                                          (0x00000534U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_14                                          (0x00000538U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_15                                          (0x0000053CU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_16                                          (0x00000540U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_17                                          (0x00000544U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_0                                           (0x00000580U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_1                                           (0x00000584U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_2                                           (0x00000588U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_3                                           (0x0000058CU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_4                                           (0x00000590U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_5                                           (0x00000594U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_6                                           (0x00000598U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_7                                           (0x0000059CU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_8                                           (0x000005A0U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_9                                           (0x000005A4U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_10                                          (0x000005A8U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_11                                          (0x000005ACU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_12                                          (0x000005B0U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_13                                          (0x000005B4U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_14                                          (0x000005B8U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_15                                          (0x000005BCU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_16                                          (0x000005C0U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_17                                          (0x000005C4U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_0                                          (0x00000600U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_1                                          (0x00000604U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_2                                          (0x00000608U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_3                                          (0x0000060CU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_4                                          (0x00000610U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_5                                          (0x00000614U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_6                                          (0x00000618U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_7                                          (0x0000061CU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_8                                          (0x00000620U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_9                                          (0x00000624U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_10                                         (0x00000628U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_11                                         (0x0000062CU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_12                                         (0x00000630U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_13                                         (0x00000634U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_14                                         (0x00000638U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_15                                         (0x0000063CU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_16                                         (0x00000640U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17                                         (0x00000644U)
#define CSL_HSM_PKA_GF2M_CMD                                                   (0x00000700U)
#define CSL_HSM_PKA_GF2M_STAT                                                  (0x00000704U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE                                             (0x00000708U)
#define CSL_HSM_PKA_GF2M_OPTIONS                                               (0x000007F8U)
#define CSL_HSM_PKA_GF2M_VERSION                                               (0x000007FCU)
#define CSL_HSM_PKA_PKA_REV                                                    (0x00001FE0U)
#define CSL_HSM_PKA_PKA_CLK_CTRL                                               (0x00001FE8U)
#define CSL_HSM_PKA_PKA_SYSCONFIG                                              (0x00001FF0U)
#define CSL_HSM_PKA_PKA_SYSSTATUS                                              (0x00001FF4U)
#define CSL_HSM_PKA_PKA_IRQSTATUS                                              (0x00001FF8U)
#define CSL_HSM_PKA_PKA_IRQENABLE                                              (0x00001FFCU)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* PKA_APTR */

#define CSL_HSM_PKA_PKA_APTR_APTR_MASK                                         (0x000007FFU)
#define CSL_HSM_PKA_PKA_APTR_APTR_SHIFT                                        (0x00000000U)
#define CSL_HSM_PKA_PKA_APTR_APTR_RESETVAL                                     (0x00000000U)
#define CSL_HSM_PKA_PKA_APTR_APTR_MAX                                          (0x000007FFU)

#define CSL_HSM_PKA_PKA_APTR_GF2M_ACC_MASK                                     (0x00000800U)
#define CSL_HSM_PKA_PKA_APTR_GF2M_ACC_SHIFT                                    (0x0000000BU)
#define CSL_HSM_PKA_PKA_APTR_GF2M_ACC_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_PKA_APTR_GF2M_ACC_MAX                                      (0x00000001U)

#define CSL_HSM_PKA_PKA_APTR_RESERVED_1_MASK                                   (0x003FF000U)
#define CSL_HSM_PKA_PKA_APTR_RESERVED_1_SHIFT                                  (0x0000000CU)
#define CSL_HSM_PKA_PKA_APTR_RESERVED_1_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_APTR_RESERVED_1_MAX                                    (0x000003FFU)

#define CSL_HSM_PKA_PKA_APTR_RESETVAL                                          (0x00000000U)

/* PKA_BPTR */

#define CSL_HSM_PKA_PKA_BPTR_BPTR_MASK                                         (0x000007FFU)
#define CSL_HSM_PKA_PKA_BPTR_BPTR_SHIFT                                        (0x00000000U)
#define CSL_HSM_PKA_PKA_BPTR_BPTR_RESETVAL                                     (0x00000000U)
#define CSL_HSM_PKA_PKA_BPTR_BPTR_MAX                                          (0x000007FFU)

#define CSL_HSM_PKA_PKA_BPTR_GF2M_ACC_MASK                                     (0x00000800U)
#define CSL_HSM_PKA_PKA_BPTR_GF2M_ACC_SHIFT                                    (0x0000000BU)
#define CSL_HSM_PKA_PKA_BPTR_GF2M_ACC_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_PKA_BPTR_GF2M_ACC_MAX                                      (0x00000001U)

#define CSL_HSM_PKA_PKA_BPTR_RESERVED_1_MASK                                   (0x003FF000U)
#define CSL_HSM_PKA_PKA_BPTR_RESERVED_1_SHIFT                                  (0x0000000CU)
#define CSL_HSM_PKA_PKA_BPTR_RESERVED_1_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_BPTR_RESERVED_1_MAX                                    (0x000003FFU)

#define CSL_HSM_PKA_PKA_BPTR_RESETVAL                                          (0x00000000U)

/* PKA_CPTR */

#define CSL_HSM_PKA_PKA_CPTR_CPTR_MASK                                         (0x000007FFU)
#define CSL_HSM_PKA_PKA_CPTR_CPTR_SHIFT                                        (0x00000000U)
#define CSL_HSM_PKA_PKA_CPTR_CPTR_RESETVAL                                     (0x00000000U)
#define CSL_HSM_PKA_PKA_CPTR_CPTR_MAX                                          (0x000007FFU)

#define CSL_HSM_PKA_PKA_CPTR_GF2M_ACC_MASK                                     (0x00000800U)
#define CSL_HSM_PKA_PKA_CPTR_GF2M_ACC_SHIFT                                    (0x0000000BU)
#define CSL_HSM_PKA_PKA_CPTR_GF2M_ACC_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_PKA_CPTR_GF2M_ACC_MAX                                      (0x00000001U)

#define CSL_HSM_PKA_PKA_CPTR_RESERVED_1_MASK                                   (0x003FF000U)
#define CSL_HSM_PKA_PKA_CPTR_RESERVED_1_SHIFT                                  (0x0000000CU)
#define CSL_HSM_PKA_PKA_CPTR_RESERVED_1_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_CPTR_RESERVED_1_MAX                                    (0x000003FFU)

#define CSL_HSM_PKA_PKA_CPTR_RESETVAL                                          (0x00000000U)

/* PKA_DPTR */

#define CSL_HSM_PKA_PKA_DPTR_DPTR_MASK                                         (0x000007FFU)
#define CSL_HSM_PKA_PKA_DPTR_DPTR_SHIFT                                        (0x00000000U)
#define CSL_HSM_PKA_PKA_DPTR_DPTR_RESETVAL                                     (0x00000000U)
#define CSL_HSM_PKA_PKA_DPTR_DPTR_MAX                                          (0x000007FFU)

#define CSL_HSM_PKA_PKA_DPTR_GF2M_ACC_MASK                                     (0x00000800U)
#define CSL_HSM_PKA_PKA_DPTR_GF2M_ACC_SHIFT                                    (0x0000000BU)
#define CSL_HSM_PKA_PKA_DPTR_GF2M_ACC_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_PKA_DPTR_GF2M_ACC_MAX                                      (0x00000001U)

#define CSL_HSM_PKA_PKA_DPTR_RESERVED_1_MASK                                   (0x003FF000U)
#define CSL_HSM_PKA_PKA_DPTR_RESERVED_1_SHIFT                                  (0x0000000CU)
#define CSL_HSM_PKA_PKA_DPTR_RESERVED_1_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_DPTR_RESERVED_1_MAX                                    (0x000003FFU)

#define CSL_HSM_PKA_PKA_DPTR_RESETVAL                                          (0x00000000U)

/* PKA_ALENGTH */

#define CSL_HSM_PKA_PKA_ALENGTH_ALENGTH_MASK                                   (0x000003FFU)
#define CSL_HSM_PKA_PKA_ALENGTH_ALENGTH_SHIFT                                  (0x00000000U)
#define CSL_HSM_PKA_PKA_ALENGTH_ALENGTH_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_ALENGTH_ALENGTH_MAX                                    (0x000003FFU)

#define CSL_HSM_PKA_PKA_ALENGTH_RESERVED_1_MASK                                (0xFFFFFC00U)
#define CSL_HSM_PKA_PKA_ALENGTH_RESERVED_1_SHIFT                               (0x0000000AU)
#define CSL_HSM_PKA_PKA_ALENGTH_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_PKA_ALENGTH_RESERVED_1_MAX                                 (0x003FFFFFU)

#define CSL_HSM_PKA_PKA_ALENGTH_RESETVAL                                       (0x00000000U)

/* PKA_BLENGTH */

#define CSL_HSM_PKA_PKA_BLENGTH_BLENGTH_MASK                                   (0x000003FFU)
#define CSL_HSM_PKA_PKA_BLENGTH_BLENGTH_SHIFT                                  (0x00000000U)
#define CSL_HSM_PKA_PKA_BLENGTH_BLENGTH_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_BLENGTH_BLENGTH_MAX                                    (0x000003FFU)

#define CSL_HSM_PKA_PKA_BLENGTH_RESERVED_1_MASK                                (0xFFFFFC00U)
#define CSL_HSM_PKA_PKA_BLENGTH_RESERVED_1_SHIFT                               (0x0000000AU)
#define CSL_HSM_PKA_PKA_BLENGTH_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_PKA_BLENGTH_RESERVED_1_MAX                                 (0x003FFFFFU)

#define CSL_HSM_PKA_PKA_BLENGTH_RESETVAL                                       (0x00000000U)

/* PKA_SHIFT */

#define CSL_HSM_PKA_PKA_SHIFT_BITS_TO_SHIFT_MASK                               (0x0000001FU)
#define CSL_HSM_PKA_PKA_SHIFT_BITS_TO_SHIFT_SHIFT                              (0x00000000U)
#define CSL_HSM_PKA_PKA_SHIFT_BITS_TO_SHIFT_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_SHIFT_BITS_TO_SHIFT_MAX                                (0x0000001FU)

#define CSL_HSM_PKA_PKA_SHIFT_RESERVED_1_MASK                                  (0xFFFFFFC0U)
#define CSL_HSM_PKA_PKA_SHIFT_RESERVED_1_SHIFT                                 (0x00000006U)
#define CSL_HSM_PKA_PKA_SHIFT_RESERVED_1_RESETVAL                              (0x00000000U)
#define CSL_HSM_PKA_PKA_SHIFT_RESERVED_1_MAX                                   (0x03FFFFFFU)

#define CSL_HSM_PKA_PKA_SHIFT_RESETVAL                                         (0x00000000U)

/* PKA_FUNCTION */

#define CSL_HSM_PKA_PKA_FUNCTION_MULTIPLY_MASK                                 (0x00000001U)
#define CSL_HSM_PKA_PKA_FUNCTION_MULTIPLY_SHIFT                                (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_MULTIPLY_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_MULTIPLY_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_ADDSUB_MASK                                   (0x00000002U)
#define CSL_HSM_PKA_PKA_FUNCTION_ADDSUB_SHIFT                                  (0x00000001U)
#define CSL_HSM_PKA_PKA_FUNCTION_ADDSUB_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_ADDSUB_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_1_MASK                               (0x00000004U)
#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_1_SHIFT                              (0x00000002U)
#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_1_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_1_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_MS_ONE_MASK                                   (0x00000008U)
#define CSL_HSM_PKA_PKA_FUNCTION_MS_ONE_SHIFT                                  (0x00000003U)
#define CSL_HSM_PKA_PKA_FUNCTION_MS_ONE_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_MS_ONE_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_ADD_MASK                                      (0x00000010U)
#define CSL_HSM_PKA_PKA_FUNCTION_ADD_SHIFT                                     (0x00000004U)
#define CSL_HSM_PKA_PKA_FUNCTION_ADD_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_ADD_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_SUBTRACT_MASK                                 (0x00000020U)
#define CSL_HSM_PKA_PKA_FUNCTION_SUBTRACT_SHIFT                                (0x00000005U)
#define CSL_HSM_PKA_PKA_FUNCTION_SUBTRACT_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_SUBTRACT_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_RSHIFT_MASK                                   (0x00000040U)
#define CSL_HSM_PKA_PKA_FUNCTION_RSHIFT_SHIFT                                  (0x00000006U)
#define CSL_HSM_PKA_PKA_FUNCTION_RSHIFT_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_RSHIFT_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_LSHIFT_MASK                                   (0x00000080U)
#define CSL_HSM_PKA_PKA_FUNCTION_LSHIFT_SHIFT                                  (0x00000007U)
#define CSL_HSM_PKA_PKA_FUNCTION_LSHIFT_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_LSHIFT_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_DIVIDE_MASK                                   (0x00000100U)
#define CSL_HSM_PKA_PKA_FUNCTION_DIVIDE_SHIFT                                  (0x00000008U)
#define CSL_HSM_PKA_PKA_FUNCTION_DIVIDE_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_DIVIDE_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_MODULO_MASK                                   (0x00000200U)
#define CSL_HSM_PKA_PKA_FUNCTION_MODULO_SHIFT                                  (0x00000009U)
#define CSL_HSM_PKA_PKA_FUNCTION_MODULO_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_MODULO_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_COMPARE_MASK                                  (0x00000400U)
#define CSL_HSM_PKA_PKA_FUNCTION_COMPARE_SHIFT                                 (0x0000000AU)
#define CSL_HSM_PKA_PKA_FUNCTION_COMPARE_RESETVAL                              (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_COMPARE_MAX                                   (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_COPY_MASK                                     (0x00000800U)
#define CSL_HSM_PKA_PKA_FUNCTION_COPY_SHIFT                                    (0x0000000BU)
#define CSL_HSM_PKA_PKA_FUNCTION_COPY_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_COPY_MAX                                      (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_SEQ_OP_MASK                                   (0x00007000U)
#define CSL_HSM_PKA_PKA_FUNCTION_SEQ_OP_SHIFT                                  (0x0000000CU)
#define CSL_HSM_PKA_PKA_FUNCTION_SEQ_OP_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_SEQ_OP_MAX                                    (0x00000007U)

#define CSL_HSM_PKA_PKA_FUNCTION_RUN_MASK                                      (0x00008000U)
#define CSL_HSM_PKA_PKA_FUNCTION_RUN_SHIFT                                     (0x0000000FU)
#define CSL_HSM_PKA_PKA_FUNCTION_RUN_RESETVAL                                  (0x00000001U)
#define CSL_HSM_PKA_PKA_FUNCTION_RUN_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_SEQUENCER_OPERATIONS_EXTEND_MASK              (0x00070000U)
#define CSL_HSM_PKA_PKA_FUNCTION_SEQUENCER_OPERATIONS_EXTEND_SHIFT             (0x00000010U)
#define CSL_HSM_PKA_PKA_FUNCTION_SEQUENCER_OPERATIONS_EXTEND_RESETVAL          (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_SEQUENCER_OPERATIONS_EXTEND_MAX               (0x00000007U)

#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_2_MASK                               (0x00F80000U)
#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_2_SHIFT                              (0x00000013U)
#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_2_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_2_MAX                                (0x0000001FU)

#define CSL_HSM_PKA_PKA_FUNCTION_STALL_RESULT_MASK                             (0x01000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_STALL_RESULT_SHIFT                            (0x00000018U)
#define CSL_HSM_PKA_PKA_FUNCTION_STALL_RESULT_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_STALL_RESULT_MAX                              (0x00000001U)

#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_3_MASK                               (0xFE000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_3_SHIFT                              (0x00000019U)
#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_3_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_FUNCTION_RESERVED_3_MAX                                (0x0000007FU)

#define CSL_HSM_PKA_PKA_FUNCTION_RESETVAL                                      (0x00008000U)

/* PKA_COMPARE */

#define CSL_HSM_PKA_PKA_COMPARE_A_EQUAL_B_MASK                                 (0x00000001U)
#define CSL_HSM_PKA_PKA_COMPARE_A_EQUAL_B_SHIFT                                (0x00000000U)
#define CSL_HSM_PKA_PKA_COMPARE_A_EQUAL_B_RESETVAL                             (0x00000001U)
#define CSL_HSM_PKA_PKA_COMPARE_A_EQUAL_B_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_PKA_COMPARE_A_LESS_THAN_B_MASK                             (0x00000002U)
#define CSL_HSM_PKA_PKA_COMPARE_A_LESS_THAN_B_SHIFT                            (0x00000001U)
#define CSL_HSM_PKA_PKA_COMPARE_A_LESS_THAN_B_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_COMPARE_A_LESS_THAN_B_MAX                              (0x00000001U)

#define CSL_HSM_PKA_PKA_COMPARE_A_GREATER_THAN_B_MASK                          (0x00000004U)
#define CSL_HSM_PKA_PKA_COMPARE_A_GREATER_THAN_B_SHIFT                         (0x00000002U)
#define CSL_HSM_PKA_PKA_COMPARE_A_GREATER_THAN_B_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_PKA_COMPARE_A_GREATER_THAN_B_MAX                           (0x00000001U)

#define CSL_HSM_PKA_PKA_COMPARE_RESERVED_1_MASK                                (0xFFFFFFF8U)
#define CSL_HSM_PKA_PKA_COMPARE_RESERVED_1_SHIFT                               (0x00000003U)
#define CSL_HSM_PKA_PKA_COMPARE_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_PKA_COMPARE_RESERVED_1_MAX                                 (0x1FFFFFFFU)

#define CSL_HSM_PKA_PKA_COMPARE_RESETVAL                                       (0x00000001U)

/* PKA_MSW */

#define CSL_HSM_PKA_PKA_MSW_MSW_ADDRESS_MASK                                   (0x000007FFU)
#define CSL_HSM_PKA_PKA_MSW_MSW_ADDRESS_SHIFT                                  (0x00000000U)
#define CSL_HSM_PKA_PKA_MSW_MSW_ADDRESS_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_MSW_MSW_ADDRESS_MAX                                    (0x000007FFU)

#define CSL_HSM_PKA_PKA_MSW_GF2M_ACC_MASK                                      (0x00000800U)
#define CSL_HSM_PKA_PKA_MSW_GF2M_ACC_SHIFT                                     (0x0000000BU)
#define CSL_HSM_PKA_PKA_MSW_GF2M_ACC_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_PKA_MSW_GF2M_ACC_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_PKA_MSW_RESERVED_1_MASK                                    (0x00007000U)
#define CSL_HSM_PKA_PKA_MSW_RESERVED_1_SHIFT                                   (0x0000000CU)
#define CSL_HSM_PKA_PKA_MSW_RESERVED_1_RESETVAL                                (0x00000000U)
#define CSL_HSM_PKA_PKA_MSW_RESERVED_1_MAX                                     (0x00000007U)

#define CSL_HSM_PKA_PKA_MSW_RESULT_IS_ZERO_MASK                                (0x00008000U)
#define CSL_HSM_PKA_PKA_MSW_RESULT_IS_ZERO_SHIFT                               (0x0000000FU)
#define CSL_HSM_PKA_PKA_MSW_RESULT_IS_ZERO_RESETVAL                            (0x00000001U)
#define CSL_HSM_PKA_PKA_MSW_RESULT_IS_ZERO_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_PKA_MSW_RESERVED_2_MASK                                    (0xFFFF0000U)
#define CSL_HSM_PKA_PKA_MSW_RESERVED_2_SHIFT                                   (0x00000010U)
#define CSL_HSM_PKA_PKA_MSW_RESERVED_2_RESETVAL                                (0x00000000U)
#define CSL_HSM_PKA_PKA_MSW_RESERVED_2_MAX                                     (0x0000FFFFU)

#define CSL_HSM_PKA_PKA_MSW_RESETVAL                                           (0x00008000U)

/* PKA_DIVMSW */

#define CSL_HSM_PKA_PKA_DIVMSW_MSW_ADDRESS_MASK                                (0x000007FFU)
#define CSL_HSM_PKA_PKA_DIVMSW_MSW_ADDRESS_SHIFT                               (0x00000000U)
#define CSL_HSM_PKA_PKA_DIVMSW_MSW_ADDRESS_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_PKA_DIVMSW_MSW_ADDRESS_MAX                                 (0x000007FFU)

#define CSL_HSM_PKA_PKA_DIVMSW_GF2M_ACC_MASK                                   (0x00000800U)
#define CSL_HSM_PKA_PKA_DIVMSW_GF2M_ACC_SHIFT                                  (0x0000000BU)
#define CSL_HSM_PKA_PKA_DIVMSW_GF2M_ACC_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_PKA_DIVMSW_GF2M_ACC_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_PKA_DIVMSW_RESERVED_1_MASK                                 (0x00007000U)
#define CSL_HSM_PKA_PKA_DIVMSW_RESERVED_1_SHIFT                                (0x0000000CU)
#define CSL_HSM_PKA_PKA_DIVMSW_RESERVED_1_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_PKA_DIVMSW_RESERVED_1_MAX                                  (0x00000007U)

#define CSL_HSM_PKA_PKA_DIVMSW_RESULT_IS_ZERO_MASK                             (0x00008000U)
#define CSL_HSM_PKA_PKA_DIVMSW_RESULT_IS_ZERO_SHIFT                            (0x0000000FU)
#define CSL_HSM_PKA_PKA_DIVMSW_RESULT_IS_ZERO_RESETVAL                         (0x00000001U)
#define CSL_HSM_PKA_PKA_DIVMSW_RESULT_IS_ZERO_MAX                              (0x00000001U)

#define CSL_HSM_PKA_PKA_DIVMSW_RESERVED_2_MASK                                 (0xFFFF0000U)
#define CSL_HSM_PKA_PKA_DIVMSW_RESERVED_2_SHIFT                                (0x00000010U)
#define CSL_HSM_PKA_PKA_DIVMSW_RESERVED_2_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_PKA_DIVMSW_RESERVED_2_MAX                                  (0x0000FFFFU)

#define CSL_HSM_PKA_PKA_DIVMSW_RESETVAL                                        (0x00008000U)

/* LNME1_STATUS */

#define CSL_HSM_PKA_LNME1_STATUS_OVERFLOW_MASK                                 (0x00000001U)
#define CSL_HSM_PKA_LNME1_STATUS_OVERFLOW_SHIFT                                (0x00000000U)
#define CSL_HSM_PKA_LNME1_STATUS_OVERFLOW_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME1_STATUS_OVERFLOW_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_LNME1_STATUS_MMM_BUSY_MASK                                 (0x00000002U)
#define CSL_HSM_PKA_LNME1_STATUS_MMM_BUSY_SHIFT                                (0x00000001U)
#define CSL_HSM_PKA_LNME1_STATUS_MMM_BUSY_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME1_STATUS_MMM_BUSY_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_LNME1_STATUS_CMD_ERROR_MASK                                (0x00000004U)
#define CSL_HSM_PKA_LNME1_STATUS_CMD_ERROR_SHIFT                               (0x00000002U)
#define CSL_HSM_PKA_LNME1_STATUS_CMD_ERROR_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_STATUS_CMD_ERROR_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_LNME1_STATUS_RESULT_ZERO_MASK                              (0x00000008U)
#define CSL_HSM_PKA_LNME1_STATUS_RESULT_ZERO_SHIFT                             (0x00000003U)
#define CSL_HSM_PKA_LNME1_STATUS_RESULT_ZERO_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME1_STATUS_RESULT_ZERO_MAX                               (0x00000001U)

#define CSL_HSM_PKA_LNME1_STATUS_STICKY_OFLO_MASK                              (0x00000010U)
#define CSL_HSM_PKA_LNME1_STATUS_STICKY_OFLO_SHIFT                             (0x00000004U)
#define CSL_HSM_PKA_LNME1_STATUS_STICKY_OFLO_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME1_STATUS_STICKY_OFLO_MAX                               (0x00000001U)

#define CSL_HSM_PKA_LNME1_STATUS_STICKY_ZERO_MASK                              (0x00000020U)
#define CSL_HSM_PKA_LNME1_STATUS_STICKY_ZERO_SHIFT                             (0x00000005U)
#define CSL_HSM_PKA_LNME1_STATUS_STICKY_ZERO_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME1_STATUS_STICKY_ZERO_MAX                               (0x00000001U)

#define CSL_HSM_PKA_LNME1_STATUS_RESERVED_1_MASK                               (0xFFFFFFC0U)
#define CSL_HSM_PKA_LNME1_STATUS_RESERVED_1_SHIFT                              (0x00000006U)
#define CSL_HSM_PKA_LNME1_STATUS_RESERVED_1_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_LNME1_STATUS_RESERVED_1_MAX                                (0x03FFFFFFU)

#define CSL_HSM_PKA_LNME1_STATUS_RESETVAL                                      (0x00000000U)

/* LNME1_CONTROL */

#define CSL_HSM_PKA_LNME1_CONTROL_MMM_CMD_MASK                                 (0x00000001U)
#define CSL_HSM_PKA_LNME1_CONTROL_MMM_CMD_SHIFT                                (0x00000000U)
#define CSL_HSM_PKA_LNME1_CONTROL_MMM_CMD_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME1_CONTROL_MMM_CMD_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_LNME1_CONTROL_MMMNEXT_CMD_MASK                             (0x00000002U)
#define CSL_HSM_PKA_LNME1_CONTROL_MMMNEXT_CMD_SHIFT                            (0x00000001U)
#define CSL_HSM_PKA_LNME1_CONTROL_MMMNEXT_CMD_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_LNME1_CONTROL_MMMNEXT_CMD_MAX                              (0x00000001U)

#define CSL_HSM_PKA_LNME1_CONTROL_EXP_CMD_MASK                                 (0x00000004U)
#define CSL_HSM_PKA_LNME1_CONTROL_EXP_CMD_SHIFT                                (0x00000002U)
#define CSL_HSM_PKA_LNME1_CONTROL_EXP_CMD_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME1_CONTROL_EXP_CMD_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_LNME1_CONTROL_RESERVED_1_MASK                              (0x00000018U)
#define CSL_HSM_PKA_LNME1_CONTROL_RESERVED_1_SHIFT                             (0x00000003U)
#define CSL_HSM_PKA_LNME1_CONTROL_RESERVED_1_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME1_CONTROL_RESERVED_1_MAX                               (0x00000003U)

#define CSL_HSM_PKA_LNME1_CONTROL_RESET_MASK                                   (0x00000020U)
#define CSL_HSM_PKA_LNME1_CONTROL_RESET_SHIFT                                  (0x00000005U)
#define CSL_HSM_PKA_LNME1_CONTROL_RESET_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_LNME1_CONTROL_RESET_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_LNME1_CONTROL_RESERVED_2_MASK                              (0xFFFFFFC0U)
#define CSL_HSM_PKA_LNME1_CONTROL_RESERVED_2_SHIFT                             (0x00000006U)
#define CSL_HSM_PKA_LNME1_CONTROL_RESERVED_2_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME1_CONTROL_RESERVED_2_MAX                               (0x03FFFFFFU)

#define CSL_HSM_PKA_LNME1_CONTROL_RESETVAL                                     (0x00000000U)

/* LNME1_NBASE */

#define CSL_HSM_PKA_LNME1_NBASE_ZERO_MASK                                      (0x00000001U)
#define CSL_HSM_PKA_LNME1_NBASE_ZERO_SHIFT                                     (0x00000000U)
#define CSL_HSM_PKA_LNME1_NBASE_ZERO_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_LNME1_NBASE_ZERO_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_LNME1_NBASE_NBASE_MASK                                     (0x000007FEU)
#define CSL_HSM_PKA_LNME1_NBASE_NBASE_SHIFT                                    (0x00000001U)
#define CSL_HSM_PKA_LNME1_NBASE_NBASE_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME1_NBASE_NBASE_MAX                                      (0x000003FFU)

#define CSL_HSM_PKA_LNME1_NBASE_RESERVED_1_MASK                                (0x00007800U)
#define CSL_HSM_PKA_LNME1_NBASE_RESERVED_1_SHIFT                               (0x0000000BU)
#define CSL_HSM_PKA_LNME1_NBASE_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_NBASE_RESERVED_1_MAX                                 (0x0000000FU)

#define CSL_HSM_PKA_LNME1_NBASE_MOD_RD_MASK                                    (0x00008000U)
#define CSL_HSM_PKA_LNME1_NBASE_MOD_RD_SHIFT                                   (0x0000000FU)
#define CSL_HSM_PKA_LNME1_NBASE_MOD_RD_RESETVAL                                (0x00000000U)
#define CSL_HSM_PKA_LNME1_NBASE_MOD_RD_MAX                                     (0x00000001U)

#define CSL_HSM_PKA_LNME1_NBASE_NYDIGITS_MASK                                  (0x03FF0000U)
#define CSL_HSM_PKA_LNME1_NBASE_NYDIGITS_SHIFT                                 (0x00000010U)
#define CSL_HSM_PKA_LNME1_NBASE_NYDIGITS_RESETVAL                              (0x00000000U)
#define CSL_HSM_PKA_LNME1_NBASE_NYDIGITS_MAX                                   (0x000003FFU)

#define CSL_HSM_PKA_LNME1_NBASE_RESERVED_2_MASK                                (0xFC000000U)
#define CSL_HSM_PKA_LNME1_NBASE_RESERVED_2_SHIFT                               (0x0000001AU)
#define CSL_HSM_PKA_LNME1_NBASE_RESERVED_2_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_NBASE_RESERVED_2_MAX                                 (0x0000003FU)

#define CSL_HSM_PKA_LNME1_NBASE_RESETVAL                                       (0x00000000U)

/* LNME1_XBASE */

#define CSL_HSM_PKA_LNME1_XBASE_ZERO_MASK                                      (0x00000001U)
#define CSL_HSM_PKA_LNME1_XBASE_ZERO_SHIFT                                     (0x00000000U)
#define CSL_HSM_PKA_LNME1_XBASE_ZERO_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_LNME1_XBASE_ZERO_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_LNME1_XBASE_XBASE_MASK                                     (0x000007FEU)
#define CSL_HSM_PKA_LNME1_XBASE_XBASE_SHIFT                                    (0x00000001U)
#define CSL_HSM_PKA_LNME1_XBASE_XBASE_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME1_XBASE_XBASE_MAX                                      (0x000003FFU)

#define CSL_HSM_PKA_LNME1_XBASE_RESERVED_1_MASK                                (0x0000F800U)
#define CSL_HSM_PKA_LNME1_XBASE_RESERVED_1_SHIFT                               (0x0000000BU)
#define CSL_HSM_PKA_LNME1_XBASE_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_XBASE_RESERVED_1_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME1_XBASE_MMM_CMD_MASK                                   (0x03FF0000U)
#define CSL_HSM_PKA_LNME1_XBASE_MMM_CMD_SHIFT                                  (0x00000010U)
#define CSL_HSM_PKA_LNME1_XBASE_MMM_CMD_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_LNME1_XBASE_MMM_CMD_MAX                                    (0x000003FFU)

#define CSL_HSM_PKA_LNME1_XBASE_RESERVED_2_MASK                                (0xFC000000U)
#define CSL_HSM_PKA_LNME1_XBASE_RESERVED_2_SHIFT                               (0x0000001AU)
#define CSL_HSM_PKA_LNME1_XBASE_RESERVED_2_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_XBASE_RESERVED_2_MAX                                 (0x0000003FU)

#define CSL_HSM_PKA_LNME1_XBASE_RESETVAL                                       (0x00000000U)

/* LNME1_YBASE */

#define CSL_HSM_PKA_LNME1_YBASE_ZERO_MASK                                      (0x00000001U)
#define CSL_HSM_PKA_LNME1_YBASE_ZERO_SHIFT                                     (0x00000000U)
#define CSL_HSM_PKA_LNME1_YBASE_ZERO_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_LNME1_YBASE_ZERO_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_LNME1_YBASE_YBASE_MASK                                     (0x000007FEU)
#define CSL_HSM_PKA_LNME1_YBASE_YBASE_SHIFT                                    (0x00000001U)
#define CSL_HSM_PKA_LNME1_YBASE_YBASE_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME1_YBASE_YBASE_MAX                                      (0x000003FFU)

#define CSL_HSM_PKA_LNME1_YBASE_RESERVED_1_MASK                                (0x0000F800U)
#define CSL_HSM_PKA_LNME1_YBASE_RESERVED_1_SHIFT                               (0x0000000BU)
#define CSL_HSM_PKA_LNME1_YBASE_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_YBASE_RESERVED_1_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME1_YBASE_NPASSES_MASK                                   (0x00FF0000U)
#define CSL_HSM_PKA_LNME1_YBASE_NPASSES_SHIFT                                  (0x00000010U)
#define CSL_HSM_PKA_LNME1_YBASE_NPASSES_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_LNME1_YBASE_NPASSES_MAX                                    (0x000000FFU)

#define CSL_HSM_PKA_LNME1_YBASE_RESERVED_2_MASK                                (0xFF000000U)
#define CSL_HSM_PKA_LNME1_YBASE_RESERVED_2_SHIFT                               (0x00000018U)
#define CSL_HSM_PKA_LNME1_YBASE_RESERVED_2_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_YBASE_RESERVED_2_MAX                                 (0x000000FFU)

#define CSL_HSM_PKA_LNME1_YBASE_RESETVAL                                       (0x00000000U)

/* LNME1_BBASE */

#define CSL_HSM_PKA_LNME1_BBASE_ZERO_MASK                                      (0x00000001U)
#define CSL_HSM_PKA_LNME1_BBASE_ZERO_SHIFT                                     (0x00000000U)
#define CSL_HSM_PKA_LNME1_BBASE_ZERO_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_LNME1_BBASE_ZERO_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_LNME1_BBASE_BBASE_MASK                                     (0x000007FEU)
#define CSL_HSM_PKA_LNME1_BBASE_BBASE_SHIFT                                    (0x00000001U)
#define CSL_HSM_PKA_LNME1_BBASE_BBASE_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME1_BBASE_BBASE_MAX                                      (0x000003FFU)

#define CSL_HSM_PKA_LNME1_BBASE_RESERVED_1_MASK                                (0x0000F800U)
#define CSL_HSM_PKA_LNME1_BBASE_RESERVED_1_SHIFT                               (0x0000000BU)
#define CSL_HSM_PKA_LNME1_BBASE_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_BBASE_RESERVED_1_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME1_BBASE_BCNTR_MASK                                     (0x7FFF0000U)
#define CSL_HSM_PKA_LNME1_BBASE_BCNTR_SHIFT                                    (0x00000010U)
#define CSL_HSM_PKA_LNME1_BBASE_BCNTR_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME1_BBASE_BCNTR_MAX                                      (0x00007FFFU)

#define CSL_HSM_PKA_LNME1_BBASE_RESERVED_2_MASK                                (0x80000000U)
#define CSL_HSM_PKA_LNME1_BBASE_RESERVED_2_SHIFT                               (0x0000001FU)
#define CSL_HSM_PKA_LNME1_BBASE_RESERVED_2_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_BBASE_RESERVED_2_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_LNME1_BBASE_RESETVAL                                       (0x00000000U)

/* LNME1_NACC */

#define CSL_HSM_PKA_LNME1_NACC_NACC_MASK                                       (0x000000FFU)
#define CSL_HSM_PKA_LNME1_NACC_NACC_SHIFT                                      (0x00000000U)
#define CSL_HSM_PKA_LNME1_NACC_NACC_RESETVAL                                   (0x00000000U)
#define CSL_HSM_PKA_LNME1_NACC_NACC_MAX                                        (0x000000FFU)

#define CSL_HSM_PKA_LNME1_NACC_NACC_BUSY_MASK                                  (0x00000100U)
#define CSL_HSM_PKA_LNME1_NACC_NACC_BUSY_SHIFT                                 (0x00000008U)
#define CSL_HSM_PKA_LNME1_NACC_NACC_BUSY_RESETVAL                              (0x00000000U)
#define CSL_HSM_PKA_LNME1_NACC_NACC_BUSY_MAX                                   (0x00000001U)

#define CSL_HSM_PKA_LNME1_NACC_RESERVED_1_MASK                                 (0x0000FE00U)
#define CSL_HSM_PKA_LNME1_NACC_RESERVED_1_SHIFT                                (0x00000009U)
#define CSL_HSM_PKA_LNME1_NACC_RESERVED_1_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME1_NACC_RESERVED_1_MAX                                  (0x0000007FU)

#define CSL_HSM_PKA_LNME1_NACC_EXPARRAY_MASK                                   (0x001F0000U)
#define CSL_HSM_PKA_LNME1_NACC_EXPARRAY_SHIFT                                  (0x00000010U)
#define CSL_HSM_PKA_LNME1_NACC_EXPARRAY_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_LNME1_NACC_EXPARRAY_MAX                                    (0x0000001FU)

#define CSL_HSM_PKA_LNME1_NACC_RESERVED_2_MASK                                 (0x07E00000U)
#define CSL_HSM_PKA_LNME1_NACC_RESERVED_2_SHIFT                                (0x00000015U)
#define CSL_HSM_PKA_LNME1_NACC_RESERVED_2_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME1_NACC_RESERVED_2_MAX                                  (0x0000003FU)

#define CSL_HSM_PKA_LNME1_NACC_RESETVAL                                        (0x00000000U)

/* LNME1_NZERO */

#define CSL_HSM_PKA_LNME1_NZERO_NZERO_MASK                                     (0x000000FFU)
#define CSL_HSM_PKA_LNME1_NZERO_NZERO_SHIFT                                    (0x00000000U)
#define CSL_HSM_PKA_LNME1_NZERO_NZERO_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME1_NZERO_NZERO_MAX                                      (0x000000FFU)

#define CSL_HSM_PKA_LNME1_NZERO_RESERVED_1_MASK                                (0xFFFFFF00U)
#define CSL_HSM_PKA_LNME1_NZERO_RESERVED_1_SHIFT                               (0x00000008U)
#define CSL_HSM_PKA_LNME1_NZERO_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME1_NZERO_RESERVED_1_MAX                                 (0x00FFFFFFU)

#define CSL_HSM_PKA_LNME1_NZERO_RESETVAL                                       (0x00000000U)

/* LNME0_STATUS */

#define CSL_HSM_PKA_LNME0_STATUS_OVERFLOW_MASK                                 (0x00000001U)
#define CSL_HSM_PKA_LNME0_STATUS_OVERFLOW_SHIFT                                (0x00000000U)
#define CSL_HSM_PKA_LNME0_STATUS_OVERFLOW_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME0_STATUS_OVERFLOW_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_LNME0_STATUS_MMM_BUSY_MASK                                 (0x00000002U)
#define CSL_HSM_PKA_LNME0_STATUS_MMM_BUSY_SHIFT                                (0x00000001U)
#define CSL_HSM_PKA_LNME0_STATUS_MMM_BUSY_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME0_STATUS_MMM_BUSY_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_LNME0_STATUS_CMD_ERROR_MASK                                (0x00000004U)
#define CSL_HSM_PKA_LNME0_STATUS_CMD_ERROR_SHIFT                               (0x00000002U)
#define CSL_HSM_PKA_LNME0_STATUS_CMD_ERROR_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_STATUS_CMD_ERROR_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_LNME0_STATUS_RESULT_ZERO_MASK                              (0x00000008U)
#define CSL_HSM_PKA_LNME0_STATUS_RESULT_ZERO_SHIFT                             (0x00000003U)
#define CSL_HSM_PKA_LNME0_STATUS_RESULT_ZERO_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME0_STATUS_RESULT_ZERO_MAX                               (0x00000001U)

#define CSL_HSM_PKA_LNME0_STATUS_STICKY_OFLO_MASK                              (0x00000010U)
#define CSL_HSM_PKA_LNME0_STATUS_STICKY_OFLO_SHIFT                             (0x00000004U)
#define CSL_HSM_PKA_LNME0_STATUS_STICKY_OFLO_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME0_STATUS_STICKY_OFLO_MAX                               (0x00000001U)

#define CSL_HSM_PKA_LNME0_STATUS_STICKY_ZERO_MASK                              (0x00000020U)
#define CSL_HSM_PKA_LNME0_STATUS_STICKY_ZERO_SHIFT                             (0x00000005U)
#define CSL_HSM_PKA_LNME0_STATUS_STICKY_ZERO_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME0_STATUS_STICKY_ZERO_MAX                               (0x00000001U)

#define CSL_HSM_PKA_LNME0_STATUS_RESERVED_1_MASK                               (0xFFFFFFC0U)
#define CSL_HSM_PKA_LNME0_STATUS_RESERVED_1_SHIFT                              (0x00000006U)
#define CSL_HSM_PKA_LNME0_STATUS_RESERVED_1_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_LNME0_STATUS_RESERVED_1_MAX                                (0x03FFFFFFU)

#define CSL_HSM_PKA_LNME0_STATUS_RESETVAL                                      (0x00000000U)

/* LNME0_CONTROL */

#define CSL_HSM_PKA_LNME0_CONTROL_MMM_CMD_MASK                                 (0x00000001U)
#define CSL_HSM_PKA_LNME0_CONTROL_MMM_CMD_SHIFT                                (0x00000000U)
#define CSL_HSM_PKA_LNME0_CONTROL_MMM_CMD_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME0_CONTROL_MMM_CMD_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_LNME0_CONTROL_MMMNEXT_CMD_MASK                             (0x00000002U)
#define CSL_HSM_PKA_LNME0_CONTROL_MMMNEXT_CMD_SHIFT                            (0x00000001U)
#define CSL_HSM_PKA_LNME0_CONTROL_MMMNEXT_CMD_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_LNME0_CONTROL_MMMNEXT_CMD_MAX                              (0x00000001U)

#define CSL_HSM_PKA_LNME0_CONTROL_EXP_CMD_MASK                                 (0x00000004U)
#define CSL_HSM_PKA_LNME0_CONTROL_EXP_CMD_SHIFT                                (0x00000002U)
#define CSL_HSM_PKA_LNME0_CONTROL_EXP_CMD_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME0_CONTROL_EXP_CMD_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_LNME0_CONTROL_RESERVED_1_MASK                              (0x00000018U)
#define CSL_HSM_PKA_LNME0_CONTROL_RESERVED_1_SHIFT                             (0x00000003U)
#define CSL_HSM_PKA_LNME0_CONTROL_RESERVED_1_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME0_CONTROL_RESERVED_1_MAX                               (0x00000003U)

#define CSL_HSM_PKA_LNME0_CONTROL_RESET_CMD_MASK                               (0x00000020U)
#define CSL_HSM_PKA_LNME0_CONTROL_RESET_CMD_SHIFT                              (0x00000005U)
#define CSL_HSM_PKA_LNME0_CONTROL_RESET_CMD_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_LNME0_CONTROL_RESET_CMD_MAX                                (0x00000001U)

#define CSL_HSM_PKA_LNME0_CONTROL_RESERVED_2_MASK                              (0xFFFFFFC0U)
#define CSL_HSM_PKA_LNME0_CONTROL_RESERVED_2_SHIFT                             (0x00000006U)
#define CSL_HSM_PKA_LNME0_CONTROL_RESERVED_2_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME0_CONTROL_RESERVED_2_MAX                               (0x03FFFFFFU)

#define CSL_HSM_PKA_LNME0_CONTROL_RESETVAL                                     (0x00000000U)

/* LNME_DATAPATH */

#define CSL_HSM_PKA_LNME_DATAPATH_BYPASS_0_MASK                                (0x00000007U)
#define CSL_HSM_PKA_LNME_DATAPATH_BYPASS_0_SHIFT                               (0x00000000U)
#define CSL_HSM_PKA_LNME_DATAPATH_BYPASS_0_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_DATAPATH_BYPASS_0_MAX                                 (0x00000007U)

#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_1_MASK                              (0x000000F8U)
#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_1_SHIFT                             (0x00000003U)
#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_1_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_1_MAX                               (0x0000001FU)

#define CSL_HSM_PKA_LNME_DATAPATH_BYPASS_1_MASK                                (0x00000700U)
#define CSL_HSM_PKA_LNME_DATAPATH_BYPASS_1_SHIFT                               (0x00000008U)
#define CSL_HSM_PKA_LNME_DATAPATH_BYPASS_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_DATAPATH_BYPASS_1_MAX                                 (0x00000007U)

#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_2_MASK                              (0x00007800U)
#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_2_SHIFT                             (0x0000000BU)
#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_2_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_2_MAX                               (0x0000000FU)

#define CSL_HSM_PKA_LNME_DATAPATH_LINKUP_MASK                                  (0x00008000U)
#define CSL_HSM_PKA_LNME_DATAPATH_LINKUP_SHIFT                                 (0x0000000FU)
#define CSL_HSM_PKA_LNME_DATAPATH_LINKUP_RESETVAL                              (0x00000000U)
#define CSL_HSM_PKA_LNME_DATAPATH_LINKUP_MAX                                   (0x00000001U)

#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_3_MASK                              (0x7FFF0000U)
#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_3_SHIFT                             (0x00000010U)
#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_3_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME_DATAPATH_RESERVED_3_MAX                               (0x00007FFFU)

#define CSL_HSM_PKA_LNME_DATAPATH_RESETVAL                                     (0x00000000U)

/* LNME_FAST_CTRL */

#define CSL_HSM_PKA_LNME_FAST_CTRL_XOR_CTRL_MASK                               (0x00000001U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_XOR_CTRL_SHIFT                              (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_XOR_CTRL_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_XOR_CTRL_MAX                                (0x00000001U)

#define CSL_HSM_PKA_LNME_FAST_CTRL_IGNORED_MASK                                (0x00007FFEU)
#define CSL_HSM_PKA_LNME_FAST_CTRL_IGNORED_SHIFT                               (0x00000001U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_IGNORED_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_IGNORED_MAX                                 (0x00003FFFU)

#define CSL_HSM_PKA_LNME_FAST_CTRL_CMP_MASK_MASK                               (0x001F0000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_CMP_MASK_SHIFT                              (0x00000010U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_CMP_MASK_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_CMP_MASK_MAX                                (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_CTRL_CMP_VALUE_MASK                              (0x03E00000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_CMP_VALUE_SHIFT                             (0x00000015U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_CMP_VALUE_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_CMP_VALUE_MAX                               (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_CTRL_XOR_VALUE_MASK                              (0x7C000000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_XOR_VALUE_SHIFT                             (0x0000001AU)
#define CSL_HSM_PKA_LNME_FAST_CTRL_XOR_VALUE_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_XOR_VALUE_MAX                               (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_CTRL_UPDATE_MASK                                 (0x80000000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_UPDATE_SHIFT                                (0x0000001FU)
#define CSL_HSM_PKA_LNME_FAST_CTRL_UPDATE_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_CTRL_UPDATE_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_LNME_FAST_CTRL_RESETVAL                                    (0x00000000U)

/* LNME_FAST_STRT */

#define CSL_HSM_PKA_LNME_FAST_STRT_PKCP_REQUEST_MASK                           (0x00000001U)
#define CSL_HSM_PKA_LNME_FAST_STRT_PKCP_REQUEST_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_PKCP_REQUEST_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_PKCP_REQUEST_MAX                            (0x00000001U)

#define CSL_HSM_PKA_LNME_FAST_STRT_LNME0_REQUEST_MASK                          (0x00000002U)
#define CSL_HSM_PKA_LNME_FAST_STRT_LNME0_REQUEST_SHIFT                         (0x00000001U)
#define CSL_HSM_PKA_LNME_FAST_STRT_LNME0_REQUEST_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_LNME0_REQUEST_MAX                           (0x00000001U)

#define CSL_HSM_PKA_LNME_FAST_STRT_LNME1_REQUEST_MASK                          (0x00000004U)
#define CSL_HSM_PKA_LNME_FAST_STRT_LNME1_REQUEST_SHIFT                         (0x00000002U)
#define CSL_HSM_PKA_LNME_FAST_STRT_LNME1_REQUEST_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_LNME1_REQUEST_MAX                           (0x00000001U)

#define CSL_HSM_PKA_LNME_FAST_STRT_RESERVED_1_MASK                             (0x0000FFF8U)
#define CSL_HSM_PKA_LNME_FAST_STRT_RESERVED_1_SHIFT                            (0x00000003U)
#define CSL_HSM_PKA_LNME_FAST_STRT_RESERVED_1_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_RESERVED_1_MAX                              (0x00001FFFU)

#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_A_MASK                               (0x001F0000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_A_SHIFT                              (0x00000010U)
#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_A_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_A_MAX                                (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_BC_MASK                              (0x03E00000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_BC_SHIFT                             (0x00000015U)
#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_BC_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_BC_MAX                               (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_D_MASK                               (0x7C000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_D_SHIFT                              (0x0000001AU)
#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_D_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_ADDSUB_D_MAX                                (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_STRT_REQUEST_SUB_MASK                            (0x80000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_REQUEST_SUB_SHIFT                           (0x0000001FU)
#define CSL_HSM_PKA_LNME_FAST_STRT_REQUEST_SUB_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_STRT_REQUEST_SUB_MAX                             (0x00000001U)

#define CSL_HSM_PKA_LNME_FAST_STRT_RESETVAL                                    (0x00000000U)

/* LNME_FAST_MMM */

#define CSL_HSM_PKA_LNME_FAST_MMM_XINDEX_0_MASK                                (0x0000001FU)
#define CSL_HSM_PKA_LNME_FAST_MMM_XINDEX_0_SHIFT                               (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_XINDEX_0_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_XINDEX_0_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_MMM_YINDEX_0_MASK                                (0x000003E0U)
#define CSL_HSM_PKA_LNME_FAST_MMM_YINDEX_0_SHIFT                               (0x00000005U)
#define CSL_HSM_PKA_LNME_FAST_MMM_YINDEX_0_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_YINDEX_0_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_MMM_RINDEX_0_MASK                                (0x00003C00U)
#define CSL_HSM_PKA_LNME_FAST_MMM_RINDEX_0_SHIFT                               (0x0000000AU)
#define CSL_HSM_PKA_LNME_FAST_MMM_RINDEX_0_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_RINDEX_0_MAX                                 (0x0000000FU)

#define CSL_HSM_PKA_LNME_FAST_MMM_STICKY_0_MASK                                (0x00004000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_STICKY_0_SHIFT                               (0x0000000EU)
#define CSL_HSM_PKA_LNME_FAST_MMM_STICKY_0_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_STICKY_0_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_LNME_FAST_MMM_XINDEX_1_MASK                                (0x001F0000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_XINDEX_1_SHIFT                               (0x00000010U)
#define CSL_HSM_PKA_LNME_FAST_MMM_XINDEX_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_XINDEX_1_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_MMM_YINDEX_1_MASK                                (0x03E00000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_YINDEX_1_SHIFT                               (0x00000015U)
#define CSL_HSM_PKA_LNME_FAST_MMM_YINDEX_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_YINDEX_1_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_MMM_RINDEX_1_MASK                                (0x7C000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_RINDEX_1_SHIFT                               (0x0000001AU)
#define CSL_HSM_PKA_LNME_FAST_MMM_RINDEX_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_RINDEX_1_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME_FAST_MMM_STICKY_1_MASK                                (0x80000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_STICKY_1_SHIFT                               (0x0000001FU)
#define CSL_HSM_PKA_LNME_FAST_MMM_STICKY_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME_FAST_MMM_STICKY_1_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_LNME_FAST_MMM_RESETVAL                                     (0x00000000U)

/* LNME0_NBASE */

#define CSL_HSM_PKA_LNME0_NBASE_ZERO_MASK                                      (0x00000001U)
#define CSL_HSM_PKA_LNME0_NBASE_ZERO_SHIFT                                     (0x00000000U)
#define CSL_HSM_PKA_LNME0_NBASE_ZERO_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_LNME0_NBASE_ZERO_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_LNME0_NBASE_NBASE_MASK                                     (0x000007FEU)
#define CSL_HSM_PKA_LNME0_NBASE_NBASE_SHIFT                                    (0x00000001U)
#define CSL_HSM_PKA_LNME0_NBASE_NBASE_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME0_NBASE_NBASE_MAX                                      (0x000003FFU)

#define CSL_HSM_PKA_LNME0_NBASE_RESERVED_1_MASK                                (0x00007800U)
#define CSL_HSM_PKA_LNME0_NBASE_RESERVED_1_SHIFT                               (0x0000000BU)
#define CSL_HSM_PKA_LNME0_NBASE_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_NBASE_RESERVED_1_MAX                                 (0x0000000FU)

#define CSL_HSM_PKA_LNME0_NBASE_MOD_RD_MASK                                    (0x00008000U)
#define CSL_HSM_PKA_LNME0_NBASE_MOD_RD_SHIFT                                   (0x0000000FU)
#define CSL_HSM_PKA_LNME0_NBASE_MOD_RD_RESETVAL                                (0x00000000U)
#define CSL_HSM_PKA_LNME0_NBASE_MOD_RD_MAX                                     (0x00000001U)

#define CSL_HSM_PKA_LNME0_NBASE_NYDIGITS_MASK                                  (0x03FF0000U)
#define CSL_HSM_PKA_LNME0_NBASE_NYDIGITS_SHIFT                                 (0x00000010U)
#define CSL_HSM_PKA_LNME0_NBASE_NYDIGITS_RESETVAL                              (0x00000000U)
#define CSL_HSM_PKA_LNME0_NBASE_NYDIGITS_MAX                                   (0x000003FFU)

#define CSL_HSM_PKA_LNME0_NBASE_RESERVED_2_MASK                                (0xFC000000U)
#define CSL_HSM_PKA_LNME0_NBASE_RESERVED_2_SHIFT                               (0x0000001AU)
#define CSL_HSM_PKA_LNME0_NBASE_RESERVED_2_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_NBASE_RESERVED_2_MAX                                 (0x0000003FU)

#define CSL_HSM_PKA_LNME0_NBASE_RESETVAL                                       (0x00000000U)

/* LNME0_XBASE */

#define CSL_HSM_PKA_LNME0_XBASE_ZERO_MASK                                      (0x00000001U)
#define CSL_HSM_PKA_LNME0_XBASE_ZERO_SHIFT                                     (0x00000000U)
#define CSL_HSM_PKA_LNME0_XBASE_ZERO_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_LNME0_XBASE_ZERO_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_LNME0_XBASE_XBASE_MASK                                     (0x000007FEU)
#define CSL_HSM_PKA_LNME0_XBASE_XBASE_SHIFT                                    (0x00000001U)
#define CSL_HSM_PKA_LNME0_XBASE_XBASE_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME0_XBASE_XBASE_MAX                                      (0x000003FFU)

#define CSL_HSM_PKA_LNME0_XBASE_RESERVED_1_MASK                                (0x0000F800U)
#define CSL_HSM_PKA_LNME0_XBASE_RESERVED_1_SHIFT                               (0x0000000BU)
#define CSL_HSM_PKA_LNME0_XBASE_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_XBASE_RESERVED_1_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME0_XBASE_MMM_CMD_MASK                                   (0x03FF0000U)
#define CSL_HSM_PKA_LNME0_XBASE_MMM_CMD_SHIFT                                  (0x00000010U)
#define CSL_HSM_PKA_LNME0_XBASE_MMM_CMD_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_LNME0_XBASE_MMM_CMD_MAX                                    (0x000003FFU)

#define CSL_HSM_PKA_LNME0_XBASE_RESERVED_2_MASK                                (0xFC000000U)
#define CSL_HSM_PKA_LNME0_XBASE_RESERVED_2_SHIFT                               (0x0000001AU)
#define CSL_HSM_PKA_LNME0_XBASE_RESERVED_2_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_XBASE_RESERVED_2_MAX                                 (0x0000003FU)

#define CSL_HSM_PKA_LNME0_XBASE_RESETVAL                                       (0x00000000U)

/* LNME0_YBASE */

#define CSL_HSM_PKA_LNME0_YBASE_ZERO_MASK                                      (0x00000001U)
#define CSL_HSM_PKA_LNME0_YBASE_ZERO_SHIFT                                     (0x00000000U)
#define CSL_HSM_PKA_LNME0_YBASE_ZERO_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_LNME0_YBASE_ZERO_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_LNME0_YBASE_YBASE_MASK                                     (0x000007FEU)
#define CSL_HSM_PKA_LNME0_YBASE_YBASE_SHIFT                                    (0x00000001U)
#define CSL_HSM_PKA_LNME0_YBASE_YBASE_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME0_YBASE_YBASE_MAX                                      (0x000003FFU)

#define CSL_HSM_PKA_LNME0_YBASE_RESERVED_1_MASK                                (0x0000F800U)
#define CSL_HSM_PKA_LNME0_YBASE_RESERVED_1_SHIFT                               (0x0000000BU)
#define CSL_HSM_PKA_LNME0_YBASE_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_YBASE_RESERVED_1_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME0_YBASE_NPASSES_MASK                                   (0x00FF0000U)
#define CSL_HSM_PKA_LNME0_YBASE_NPASSES_SHIFT                                  (0x00000010U)
#define CSL_HSM_PKA_LNME0_YBASE_NPASSES_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_LNME0_YBASE_NPASSES_MAX                                    (0x000000FFU)

#define CSL_HSM_PKA_LNME0_YBASE_RESERVED_2_MASK                                (0xFF000000U)
#define CSL_HSM_PKA_LNME0_YBASE_RESERVED_2_SHIFT                               (0x00000018U)
#define CSL_HSM_PKA_LNME0_YBASE_RESERVED_2_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_YBASE_RESERVED_2_MAX                                 (0x000000FFU)

#define CSL_HSM_PKA_LNME0_YBASE_RESETVAL                                       (0x00000000U)

/* LNME0_BBASE */

#define CSL_HSM_PKA_LNME0_BBASE_ZERO_MASK                                      (0x00000001U)
#define CSL_HSM_PKA_LNME0_BBASE_ZERO_SHIFT                                     (0x00000000U)
#define CSL_HSM_PKA_LNME0_BBASE_ZERO_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_LNME0_BBASE_ZERO_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_LNME0_BBASE_BBASE_MASK                                     (0x000007FEU)
#define CSL_HSM_PKA_LNME0_BBASE_BBASE_SHIFT                                    (0x00000001U)
#define CSL_HSM_PKA_LNME0_BBASE_BBASE_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME0_BBASE_BBASE_MAX                                      (0x000003FFU)

#define CSL_HSM_PKA_LNME0_BBASE_RESERVED_1_MASK                                (0x0000F800U)
#define CSL_HSM_PKA_LNME0_BBASE_RESERVED_1_SHIFT                               (0x0000000BU)
#define CSL_HSM_PKA_LNME0_BBASE_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_BBASE_RESERVED_1_MAX                                 (0x0000001FU)

#define CSL_HSM_PKA_LNME0_BBASE_BCNTR_MASK                                     (0x7FFF0000U)
#define CSL_HSM_PKA_LNME0_BBASE_BCNTR_SHIFT                                    (0x00000010U)
#define CSL_HSM_PKA_LNME0_BBASE_BCNTR_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME0_BBASE_BCNTR_MAX                                      (0x00007FFFU)

#define CSL_HSM_PKA_LNME0_BBASE_RESERVED_2_MASK                                (0x80000000U)
#define CSL_HSM_PKA_LNME0_BBASE_RESERVED_2_SHIFT                               (0x0000001FU)
#define CSL_HSM_PKA_LNME0_BBASE_RESERVED_2_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_BBASE_RESERVED_2_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_LNME0_BBASE_RESETVAL                                       (0x00000000U)

/* LNME0_NACC */

#define CSL_HSM_PKA_LNME0_NACC_NACC_MASK                                       (0x000000FFU)
#define CSL_HSM_PKA_LNME0_NACC_NACC_SHIFT                                      (0x00000000U)
#define CSL_HSM_PKA_LNME0_NACC_NACC_RESETVAL                                   (0x00000000U)
#define CSL_HSM_PKA_LNME0_NACC_NACC_MAX                                        (0x000000FFU)

#define CSL_HSM_PKA_LNME0_NACC_NACC_BUSY_MASK                                  (0x00000100U)
#define CSL_HSM_PKA_LNME0_NACC_NACC_BUSY_SHIFT                                 (0x00000008U)
#define CSL_HSM_PKA_LNME0_NACC_NACC_BUSY_RESETVAL                              (0x00000000U)
#define CSL_HSM_PKA_LNME0_NACC_NACC_BUSY_MAX                                   (0x00000001U)

#define CSL_HSM_PKA_LNME0_NACC_RESERVED_1_MASK                                 (0x0000FE00U)
#define CSL_HSM_PKA_LNME0_NACC_RESERVED_1_SHIFT                                (0x00000009U)
#define CSL_HSM_PKA_LNME0_NACC_RESERVED_1_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME0_NACC_RESERVED_1_MAX                                  (0x0000007FU)

#define CSL_HSM_PKA_LNME0_NACC_EXPARRAY_MASK                                   (0x001F0000U)
#define CSL_HSM_PKA_LNME0_NACC_EXPARRAY_SHIFT                                  (0x00000010U)
#define CSL_HSM_PKA_LNME0_NACC_EXPARRAY_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_LNME0_NACC_EXPARRAY_MAX                                    (0x0000001FU)

#define CSL_HSM_PKA_LNME0_NACC_RESERVED_2_MASK                                 (0x07E00000U)
#define CSL_HSM_PKA_LNME0_NACC_RESERVED_2_SHIFT                                (0x00000015U)
#define CSL_HSM_PKA_LNME0_NACC_RESERVED_2_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_LNME0_NACC_RESERVED_2_MAX                                  (0x0000003FU)

#define CSL_HSM_PKA_LNME0_NACC_RESETVAL                                        (0x00000000U)

/* LNME0_NZERO */

#define CSL_HSM_PKA_LNME0_NZERO_NZERO_MASK                                     (0x000000FFU)
#define CSL_HSM_PKA_LNME0_NZERO_NZERO_SHIFT                                    (0x00000000U)
#define CSL_HSM_PKA_LNME0_NZERO_NZERO_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_LNME0_NZERO_NZERO_MAX                                      (0x000000FFU)

#define CSL_HSM_PKA_LNME0_NZERO_RESERVED_1_MASK                                (0xFFFFFF00U)
#define CSL_HSM_PKA_LNME0_NZERO_RESERVED_1_SHIFT                               (0x00000008U)
#define CSL_HSM_PKA_LNME0_NZERO_RESERVED_1_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_LNME0_NZERO_RESERVED_1_MAX                                 (0x00FFFFFFU)

#define CSL_HSM_PKA_LNME0_NZERO_RESETVAL                                       (0x00000000U)

/* PKA_SEQ_CTRL */

#define CSL_HSM_PKA_PKA_SEQ_CTRL_SW_TRIGGERS_MASK                              (0x000000FFU)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_SW_TRIGGERS_SHIFT                             (0x00000000U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_SW_TRIGGERS_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_SW_TRIGGERS_MAX                               (0x000000FFU)

#define CSL_HSM_PKA_PKA_SEQ_CTRL_SEQ_STATUS_MASK                               (0x0000FF00U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_SEQ_STATUS_SHIFT                              (0x00000008U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_SEQ_STATUS_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_SEQ_STATUS_MAX                                (0x000000FFU)

#define CSL_HSM_PKA_PKA_SEQ_CTRL_RESERVED_1_MASK                               (0x7FFF0000U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_RESERVED_1_SHIFT                              (0x00000010U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_RESERVED_1_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_RESERVED_1_MAX                                (0x00007FFFU)

#define CSL_HSM_PKA_PKA_SEQ_CTRL_RESET_MASK                                    (0x80000000U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_RESET_SHIFT                                   (0x0000001FU)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_RESET_RESETVAL                                (0x00000001U)
#define CSL_HSM_PKA_PKA_SEQ_CTRL_RESET_MAX                                     (0x00000001U)

#define CSL_HSM_PKA_PKA_SEQ_CTRL_RESETVAL                                      (0x80000000U)

/* PKA_OPTION */

#define CSL_HSM_PKA_PKA_OPTION_PKCP_CONFIG_MASK                                (0x00000003U)
#define CSL_HSM_PKA_PKA_OPTION_PKCP_CONFIG_SHIFT                               (0x00000000U)
#define CSL_HSM_PKA_PKA_OPTION_PKCP_CONFIG_RESETVAL                            (0x00000002U)
#define CSL_HSM_PKA_PKA_OPTION_PKCP_CONFIG_MAX                                 (0x00000003U)

#define CSL_HSM_PKA_PKA_OPTION_LNME_CONFIG_MASK                                (0x0000001CU)
#define CSL_HSM_PKA_PKA_OPTION_LNME_CONFIG_SHIFT                               (0x00000002U)
#define CSL_HSM_PKA_PKA_OPTION_LNME_CONFIG_RESETVAL                            (0x00000002U)
#define CSL_HSM_PKA_PKA_OPTION_LNME_CONFIG_MAX                                 (0x00000007U)

#define CSL_HSM_PKA_PKA_OPTION_SEQ_CONFIG_MASK                                 (0x00000060U)
#define CSL_HSM_PKA_PKA_OPTION_SEQ_CONFIG_SHIFT                                (0x00000005U)
#define CSL_HSM_PKA_PKA_OPTION_SEQ_CONFIG_RESETVAL                             (0x00000001U)
#define CSL_HSM_PKA_PKA_OPTION_SEQ_CONFIG_MAX                                  (0x00000003U)

#define CSL_HSM_PKA_PKA_OPTION_PROGRAM_RAM_MASK                                (0x00000080U)
#define CSL_HSM_PKA_PKA_OPTION_PROGRAM_RAM_SHIFT                               (0x00000007U)
#define CSL_HSM_PKA_PKA_OPTION_PROGRAM_RAM_RESETVAL                            (0x00000001U)
#define CSL_HSM_PKA_PKA_OPTION_PROGRAM_RAM_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_PKA_OPTION_PROT_OPTION_MASK                                (0x00000700U)
#define CSL_HSM_PKA_PKA_OPTION_PROT_OPTION_SHIFT                               (0x00000008U)
#define CSL_HSM_PKA_PKA_OPTION_PROT_OPTION_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_PKA_OPTION_PROT_OPTION_MAX                                 (0x00000007U)

#define CSL_HSM_PKA_PKA_OPTION_INTERRUPT_MASK_MASK                             (0x00000800U)
#define CSL_HSM_PKA_PKA_OPTION_INTERRUPT_MASK_SHIFT                            (0x0000000BU)
#define CSL_HSM_PKA_PKA_OPTION_INTERRUPT_MASK_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_OPTION_INTERRUPT_MASK_MAX                              (0x00000001U)

#define CSL_HSM_PKA_PKA_OPTION_MMM3A_MASK                                      (0x00001000U)
#define CSL_HSM_PKA_PKA_OPTION_MMM3A_SHIFT                                     (0x0000000CU)
#define CSL_HSM_PKA_PKA_OPTION_MMM3A_RESETVAL                                  (0x00000001U)
#define CSL_HSM_PKA_PKA_OPTION_MMM3A_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_PKA_OPTION_ZEROIZATION_MASK                                (0x00002000U)
#define CSL_HSM_PKA_PKA_OPTION_ZEROIZATION_SHIFT                               (0x0000000DU)
#define CSL_HSM_PKA_PKA_OPTION_ZEROIZATION_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_PKA_OPTION_ZEROIZATION_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_PKA_OPTION_LNME_BYPASS_MASK                                (0x00004000U)
#define CSL_HSM_PKA_PKA_OPTION_LNME_BYPASS_SHIFT                               (0x0000000EU)
#define CSL_HSM_PKA_PKA_OPTION_LNME_BYPASS_RESETVAL                            (0x00000001U)
#define CSL_HSM_PKA_PKA_OPTION_LNME_BYPASS_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_PKA_OPTION_RESERVED_1_MASK                                 (0x00008000U)
#define CSL_HSM_PKA_PKA_OPTION_RESERVED_1_SHIFT                                (0x0000000FU)
#define CSL_HSM_PKA_PKA_OPTION_RESERVED_1_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_PKA_OPTION_RESERVED_1_MAX                                  (0x00000001U)

#define CSL_HSM_PKA_PKA_OPTION_LNME_PES_MASK                                   (0x003F0000U)
#define CSL_HSM_PKA_PKA_OPTION_LNME_PES_SHIFT                                  (0x00000010U)
#define CSL_HSM_PKA_PKA_OPTION_LNME_PES_RESETVAL                               (0x0000000CU)
#define CSL_HSM_PKA_PKA_OPTION_LNME_PES_MAX                                    (0x0000003FU)

#define CSL_HSM_PKA_PKA_OPTION_GF2M_CONFIG_MASK                                (0x00C00000U)
#define CSL_HSM_PKA_PKA_OPTION_GF2M_CONFIG_SHIFT                               (0x00000016U)
#define CSL_HSM_PKA_PKA_OPTION_GF2M_CONFIG_RESETVAL                            (0x00000001U)
#define CSL_HSM_PKA_PKA_OPTION_GF2M_CONFIG_MAX                                 (0x00000003U)

#define CSL_HSM_PKA_PKA_OPTION_LNME_FIFO_DEPT_MASK                             (0xFF000000U)
#define CSL_HSM_PKA_PKA_OPTION_LNME_FIFO_DEPT_SHIFT                            (0x00000018U)
#define CSL_HSM_PKA_PKA_OPTION_LNME_FIFO_DEPT_RESETVAL                         (0x00000081U)
#define CSL_HSM_PKA_PKA_OPTION_LNME_FIFO_DEPT_MAX                              (0x000000FFU)

#define CSL_HSM_PKA_PKA_OPTION_RESETVAL                                        (0x814C50AAU)

/* PKA_SW_REV */

#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_PATCH_LEVEL_MASK                         (0x0000000FU)
#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_PATCH_LEVEL_SHIFT                        (0x00000000U)
#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_PATCH_LEVEL_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_PATCH_LEVEL_MAX                          (0x0000000FU)

#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_MINOR_REV_MASK                           (0x000000F0U)
#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_MINOR_REV_SHIFT                          (0x00000004U)
#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_MINOR_REV_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_MINOR_REV_MAX                            (0x0000000FU)

#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_MAJOR_REV_MASK                           (0x00000F00U)
#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_MAJOR_REV_SHIFT                          (0x00000008U)
#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_MAJOR_REV_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_PKA_SW_REV_CUSTOM_MAJOR_REV_MAX                            (0x0000000FU)

#define CSL_HSM_PKA_PKA_SW_REV_RESERVED_1_MASK                                 (0x0000F000U)
#define CSL_HSM_PKA_PKA_SW_REV_RESERVED_1_SHIFT                                (0x0000000CU)
#define CSL_HSM_PKA_PKA_SW_REV_RESERVED_1_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_PKA_SW_REV_RESERVED_1_MAX                                  (0x0000000FU)

#define CSL_HSM_PKA_PKA_SW_REV_SW_PATCH_LEVEL_MASK                             (0x000F0000U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_PATCH_LEVEL_SHIFT                            (0x00000010U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_PATCH_LEVEL_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_PATCH_LEVEL_MAX                              (0x0000000FU)

#define CSL_HSM_PKA_PKA_SW_REV_SW_MINOR_REV_MASK                               (0x00F00000U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_MINOR_REV_SHIFT                              (0x00000014U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_MINOR_REV_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_MINOR_REV_MAX                                (0x0000000FU)

#define CSL_HSM_PKA_PKA_SW_REV_SW_MAJOR_REV_MASK                               (0x0F000000U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_MAJOR_REV_SHIFT                              (0x00000018U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_MAJOR_REV_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_MAJOR_REV_MAX                                (0x0000000FU)

#define CSL_HSM_PKA_PKA_SW_REV_SW_CAPABILITIES_MASK                            (0xF0000000U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_CAPABILITIES_SHIFT                           (0x0000001CU)
#define CSL_HSM_PKA_PKA_SW_REV_SW_CAPABILITIES_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_PKA_SW_REV_SW_CAPABILITIES_MAX                             (0x0000000FU)

#define CSL_HSM_PKA_PKA_SW_REV_RESETVAL                                        (0x00000000U)

/* PKA_REVISION */

#define CSL_HSM_PKA_PKA_REVISION_EIP_NR_MASK                                   (0x000000FFU)
#define CSL_HSM_PKA_PKA_REVISION_EIP_NR_SHIFT                                  (0x00000000U)
#define CSL_HSM_PKA_PKA_REVISION_EIP_NR_RESETVAL                               (0x0000001CU)
#define CSL_HSM_PKA_PKA_REVISION_EIP_NR_MAX                                    (0x000000FFU)

#define CSL_HSM_PKA_PKA_REVISION_EIP_NR_COMPL_MASK                             (0x0000FF00U)
#define CSL_HSM_PKA_PKA_REVISION_EIP_NR_COMPL_SHIFT                            (0x00000008U)
#define CSL_HSM_PKA_PKA_REVISION_EIP_NR_COMPL_RESETVAL                         (0x000000E3U)
#define CSL_HSM_PKA_PKA_REVISION_EIP_NR_COMPL_MAX                              (0x000000FFU)

#define CSL_HSM_PKA_PKA_REVISION_HW_PATCH_LEVEL_MASK                           (0x000F0000U)
#define CSL_HSM_PKA_PKA_REVISION_HW_PATCH_LEVEL_SHIFT                          (0x00000010U)
#define CSL_HSM_PKA_PKA_REVISION_HW_PATCH_LEVEL_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_PKA_REVISION_HW_PATCH_LEVEL_MAX                            (0x0000000FU)

#define CSL_HSM_PKA_PKA_REVISION_HW_MINOR_REV_MASK                             (0x00F00000U)
#define CSL_HSM_PKA_PKA_REVISION_HW_MINOR_REV_SHIFT                            (0x00000014U)
#define CSL_HSM_PKA_PKA_REVISION_HW_MINOR_REV_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_REVISION_HW_MINOR_REV_MAX                              (0x0000000FU)

#define CSL_HSM_PKA_PKA_REVISION_HW_MAJOR_REV_MASK                             (0x0F000000U)
#define CSL_HSM_PKA_PKA_REVISION_HW_MAJOR_REV_SHIFT                            (0x00000018U)
#define CSL_HSM_PKA_PKA_REVISION_HW_MAJOR_REV_RESETVAL                         (0x00000002U)
#define CSL_HSM_PKA_PKA_REVISION_HW_MAJOR_REV_MAX                              (0x0000000FU)

#define CSL_HSM_PKA_PKA_REVISION_RESERVED_1_MASK                               (0xF0000000U)
#define CSL_HSM_PKA_PKA_REVISION_RESERVED_1_SHIFT                              (0x0000001CU)
#define CSL_HSM_PKA_PKA_REVISION_RESERVED_1_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_REVISION_RESERVED_1_MAX                                (0x0000000FU)

#define CSL_HSM_PKA_PKA_REVISION_RESETVAL                                      (0x0200E31CU)

/* GF2M_OPERAND_A_0 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_0_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_0_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_0_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_0_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_0_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_1 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_1_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_1_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_1_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_1_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_1_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_2 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_2_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_2_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_2_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_2_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_2_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_3 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_3_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_3_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_3_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_3_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_3_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_4 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_4_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_4_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_4_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_4_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_4_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_5 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_5_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_5_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_5_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_5_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_5_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_6 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_6_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_6_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_6_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_6_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_6_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_7 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_7_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_7_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_7_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_7_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_7_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_8 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_8_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_8_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_8_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_8_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_8_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_9 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_9_OPERAND_A_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_9_OPERAND_A_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_9_OPERAND_A_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_9_OPERAND_A_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_9_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_A_10 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_10_OPERAND_A_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_10_OPERAND_A_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_10_OPERAND_A_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_10_OPERAND_A_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_10_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_A_11 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_11_OPERAND_A_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_11_OPERAND_A_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_11_OPERAND_A_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_11_OPERAND_A_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_11_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_A_12 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_12_OPERAND_A_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_12_OPERAND_A_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_12_OPERAND_A_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_12_OPERAND_A_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_12_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_A_13 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_13_OPERAND_A_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_13_OPERAND_A_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_13_OPERAND_A_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_13_OPERAND_A_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_13_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_A_14 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_14_OPERAND_A_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_14_OPERAND_A_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_14_OPERAND_A_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_14_OPERAND_A_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_14_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_A_15 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_15_OPERAND_A_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_15_OPERAND_A_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_15_OPERAND_A_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_15_OPERAND_A_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_15_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_A_16 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_16_OPERAND_A_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_16_OPERAND_A_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_16_OPERAND_A_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_16_OPERAND_A_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_16_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_A_17 */

#define CSL_HSM_PKA_GF2M_OPERAND_A_17_OPERAND_A_MASK                           (0x0FFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_17_OPERAND_A_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_17_OPERAND_A_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_17_OPERAND_A_MAX                            (0x0FFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_17_RESERVED_1_MASK                          (0xF0000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_17_RESERVED_1_SHIFT                         (0x0000001CU)
#define CSL_HSM_PKA_GF2M_OPERAND_A_17_RESERVED_1_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_A_17_RESERVED_1_MAX                           (0x0000000FU)

#define CSL_HSM_PKA_GF2M_OPERAND_A_17_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_B_0 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_0_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_0_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_0_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_0_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_0_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_1 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_1_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_1_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_1_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_1_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_1_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_2 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_2_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_2_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_2_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_2_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_2_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_3 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_3_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_3_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_3_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_3_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_3_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_4 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_4_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_4_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_4_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_4_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_4_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_5 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_5_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_5_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_5_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_5_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_5_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_6 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_6_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_6_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_6_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_6_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_6_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_7 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_7_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_7_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_7_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_7_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_7_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_8 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_8_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_8_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_8_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_8_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_8_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_9 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_9_OPERAND_B_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_9_OPERAND_B_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_9_OPERAND_B_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_9_OPERAND_B_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_9_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_B_10 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_10_OPERAND_B_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_10_OPERAND_B_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_10_OPERAND_B_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_10_OPERAND_B_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_10_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_B_11 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_11_OPERAND_B_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_11_OPERAND_B_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_11_OPERAND_B_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_11_OPERAND_B_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_11_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_B_12 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_12_OPERAND_B_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_12_OPERAND_B_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_12_OPERAND_B_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_12_OPERAND_B_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_12_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_B_13 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_13_OPERAND_B_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_13_OPERAND_B_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_13_OPERAND_B_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_13_OPERAND_B_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_13_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_B_14 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_14_OPERAND_B_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_14_OPERAND_B_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_14_OPERAND_B_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_14_OPERAND_B_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_14_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_B_15 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_15_OPERAND_B_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_15_OPERAND_B_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_15_OPERAND_B_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_15_OPERAND_B_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_15_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_B_16 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_16_OPERAND_B_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_16_OPERAND_B_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_16_OPERAND_B_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_16_OPERAND_B_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_16_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_B_17 */

#define CSL_HSM_PKA_GF2M_OPERAND_B_17_OPERAND_B_MASK                           (0x0FFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_17_OPERAND_B_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_17_OPERAND_B_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_17_OPERAND_B_MAX                            (0x0FFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_17_RESERVED_1_MASK                          (0xF0000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_17_RESERVED_1_SHIFT                         (0x0000001CU)
#define CSL_HSM_PKA_GF2M_OPERAND_B_17_RESERVED_1_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_B_17_RESERVED_1_MAX                           (0x0000000FU)

#define CSL_HSM_PKA_GF2M_OPERAND_B_17_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_C_0 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_0_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_0_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_0_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_0_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_0_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_1 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_1_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_1_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_1_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_1_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_1_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_2 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_2_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_2_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_2_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_2_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_2_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_3 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_3_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_3_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_3_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_3_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_3_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_4 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_4_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_4_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_4_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_4_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_4_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_5 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_5_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_5_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_5_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_5_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_5_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_6 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_6_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_6_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_6_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_6_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_6_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_7 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_7_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_7_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_7_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_7_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_7_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_8 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_8_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_8_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_8_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_8_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_8_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_9 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_9_OPERAND_C_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_9_OPERAND_C_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_9_OPERAND_C_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_9_OPERAND_C_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_9_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_C_10 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_10_OPERAND_C_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_10_OPERAND_C_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_10_OPERAND_C_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_10_OPERAND_C_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_10_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_C_11 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_11_OPERAND_C_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_11_OPERAND_C_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_11_OPERAND_C_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_11_OPERAND_C_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_11_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_C_12 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_12_OPERAND_C_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_12_OPERAND_C_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_12_OPERAND_C_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_12_OPERAND_C_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_12_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_C_13 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_13_OPERAND_C_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_13_OPERAND_C_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_13_OPERAND_C_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_13_OPERAND_C_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_13_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_C_14 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_14_OPERAND_C_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_14_OPERAND_C_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_14_OPERAND_C_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_14_OPERAND_C_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_14_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_C_15 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_15_OPERAND_C_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_15_OPERAND_C_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_15_OPERAND_C_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_15_OPERAND_C_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_15_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_C_16 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_16_OPERAND_C_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_16_OPERAND_C_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_16_OPERAND_C_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_16_OPERAND_C_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_16_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_C_17 */

#define CSL_HSM_PKA_GF2M_OPERAND_C_17_OPERAND_C_MASK                           (0x0FFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_17_OPERAND_C_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_17_OPERAND_C_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_17_OPERAND_C_MAX                            (0x0FFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_17_RESERVED_1_MASK                          (0xF0000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_17_RESERVED_1_SHIFT                         (0x0000001CU)
#define CSL_HSM_PKA_GF2M_OPERAND_C_17_RESERVED_1_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_C_17_RESERVED_1_MAX                           (0x0000000FU)

#define CSL_HSM_PKA_GF2M_OPERAND_C_17_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_D_0 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_0_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_0_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_0_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_0_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_0_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_1 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_1_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_1_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_1_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_1_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_1_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_2 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_2_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_2_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_2_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_2_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_2_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_3 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_3_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_3_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_3_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_3_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_3_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_4 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_4_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_4_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_4_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_4_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_4_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_5 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_5_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_5_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_5_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_5_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_5_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_6 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_6_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_6_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_6_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_6_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_6_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_7 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_7_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_7_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_7_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_7_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_7_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_8 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_8_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_8_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_8_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_8_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_8_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_9 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_9_OPERAND_D_MASK                            (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_9_OPERAND_D_SHIFT                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_9_OPERAND_D_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_9_OPERAND_D_MAX                             (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_9_RESETVAL                                  (0x00000000U)

/* GF2M_OPERAND_D_10 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_10_OPERAND_D_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_10_OPERAND_D_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_10_OPERAND_D_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_10_OPERAND_D_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_10_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_D_11 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_11_OPERAND_D_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_11_OPERAND_D_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_11_OPERAND_D_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_11_OPERAND_D_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_11_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_D_12 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_12_OPERAND_D_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_12_OPERAND_D_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_12_OPERAND_D_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_12_OPERAND_D_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_12_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_D_13 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_13_OPERAND_D_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_13_OPERAND_D_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_13_OPERAND_D_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_13_OPERAND_D_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_13_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_D_14 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_14_OPERAND_D_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_14_OPERAND_D_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_14_OPERAND_D_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_14_OPERAND_D_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_14_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_D_15 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_15_OPERAND_D_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_15_OPERAND_D_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_15_OPERAND_D_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_15_OPERAND_D_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_15_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_D_16 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_16_OPERAND_D_MASK                           (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_16_OPERAND_D_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_16_OPERAND_D_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_16_OPERAND_D_MAX                            (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_16_RESETVAL                                 (0x00000000U)

/* GF2M_OPERAND_D_17 */

#define CSL_HSM_PKA_GF2M_OPERAND_D_17_OPERAND_D_MASK                           (0x0FFFFFFFU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_17_OPERAND_D_SHIFT                          (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_17_OPERAND_D_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_17_OPERAND_D_MAX                            (0x0FFFFFFFU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_17_RESERVED_1_MASK                          (0xF0000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_17_RESERVED_1_SHIFT                         (0x0000001CU)
#define CSL_HSM_PKA_GF2M_OPERAND_D_17_RESERVED_1_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPERAND_D_17_RESERVED_1_MAX                           (0x0000000FU)

#define CSL_HSM_PKA_GF2M_OPERAND_D_17_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_0 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_0_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_0_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_0_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_0_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_0_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_1 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_1_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_1_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_1_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_1_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_1_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_2 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_2_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_2_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_2_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_2_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_2_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_3 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_3_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_3_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_3_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_3_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_3_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_4 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_4_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_4_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_4_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_4_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_4_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_5 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_5_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_5_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_5_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_5_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_5_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_6 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_6_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_6_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_6_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_6_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_6_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_7 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_7_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_7_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_7_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_7_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_7_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_8 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_8_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_8_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_8_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_8_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_8_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_9 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_9_POLYNOMIAL_MASK                          (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_9_POLYNOMIAL_SHIFT                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_9_POLYNOMIAL_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_9_POLYNOMIAL_MAX                           (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_9_RESETVAL                                 (0x00000000U)

/* GF2M_POLYNOMIAL_10 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_10_POLYNOMIAL_MASK                         (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_10_POLYNOMIAL_SHIFT                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_10_POLYNOMIAL_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_10_POLYNOMIAL_MAX                          (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_10_RESETVAL                                (0x00000000U)

/* GF2M_POLYNOMIAL_11 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_11_POLYNOMIAL_MASK                         (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_11_POLYNOMIAL_SHIFT                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_11_POLYNOMIAL_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_11_POLYNOMIAL_MAX                          (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_11_RESETVAL                                (0x00000000U)

/* GF2M_POLYNOMIAL_12 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_12_POLYNOMIAL_MASK                         (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_12_POLYNOMIAL_SHIFT                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_12_POLYNOMIAL_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_12_POLYNOMIAL_MAX                          (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_12_RESETVAL                                (0x00000000U)

/* GF2M_POLYNOMIAL_13 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_13_POLYNOMIAL_MASK                         (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_13_POLYNOMIAL_SHIFT                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_13_POLYNOMIAL_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_13_POLYNOMIAL_MAX                          (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_13_RESETVAL                                (0x00000000U)

/* GF2M_POLYNOMIAL_14 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_14_POLYNOMIAL_MASK                         (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_14_POLYNOMIAL_SHIFT                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_14_POLYNOMIAL_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_14_POLYNOMIAL_MAX                          (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_14_RESETVAL                                (0x00000000U)

/* GF2M_POLYNOMIAL_15 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_15_POLYNOMIAL_MASK                         (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_15_POLYNOMIAL_SHIFT                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_15_POLYNOMIAL_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_15_POLYNOMIAL_MAX                          (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_15_RESETVAL                                (0x00000000U)

/* GF2M_POLYNOMIAL_16 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_16_POLYNOMIAL_MASK                         (0xFFFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_16_POLYNOMIAL_SHIFT                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_16_POLYNOMIAL_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_16_POLYNOMIAL_MAX                          (0xFFFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_16_RESETVAL                                (0x00000000U)

/* GF2M_POLYNOMIAL_17 */

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17_POLYNOMIAL_MASK                         (0x0FFFFFFFU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17_POLYNOMIAL_SHIFT                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17_POLYNOMIAL_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17_POLYNOMIAL_MAX                          (0x0FFFFFFFU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17_RESERVED_1_MASK                         (0xF0000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17_RESERVED_1_SHIFT                        (0x0000001CU)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17_RESERVED_1_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17_RESERVED_1_MAX                          (0x0000000FU)

#define CSL_HSM_PKA_GF2M_POLYNOMIAL_17_RESETVAL                                (0x00000000U)

/* GF2M_CMD */

#define CSL_HSM_PKA_GF2M_CMD_CMD_SUBMIT_CMD_BUF_FULL_MASK                      (0x00000001U)
#define CSL_HSM_PKA_GF2M_CMD_CMD_SUBMIT_CMD_BUF_FULL_SHIFT                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_CMD_SUBMIT_CMD_BUF_FULL_RESETVAL                  (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_CMD_SUBMIT_CMD_BUF_FULL_MAX                       (0x00000001U)

#define CSL_HSM_PKA_GF2M_CMD_OPCODE_MASK                                       (0x0000000EU)
#define CSL_HSM_PKA_GF2M_CMD_OPCODE_SHIFT                                      (0x00000001U)
#define CSL_HSM_PKA_GF2M_CMD_OPCODE_RESETVAL                                   (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_OPCODE_MAX                                        (0x00000007U)

#define CSL_HSM_PKA_GF2M_CMD_RESERVED_1_MASK                                   (0x00000030U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_1_SHIFT                                  (0x00000004U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_1_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_1_MAX                                    (0x00000003U)

#define CSL_HSM_PKA_GF2M_CMD_OPTION_MASK                                       (0x00000180U)
#define CSL_HSM_PKA_GF2M_CMD_OPTION_SHIFT                                      (0x00000007U)
#define CSL_HSM_PKA_GF2M_CMD_OPTION_RESETVAL                                   (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_OPTION_MAX                                        (0x00000003U)

#define CSL_HSM_PKA_GF2M_CMD_RESERVED_2_MASK                                   (0x00000200U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_2_SHIFT                                  (0x00000009U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_2_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_2_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_GF2M_CMD_SRC1_MASK                                         (0x00000C00U)
#define CSL_HSM_PKA_GF2M_CMD_SRC1_SHIFT                                        (0x0000000AU)
#define CSL_HSM_PKA_GF2M_CMD_SRC1_RESETVAL                                     (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_SRC1_MAX                                          (0x00000003U)

#define CSL_HSM_PKA_GF2M_CMD_RESERVED_3_MASK                                   (0x00001000U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_3_SHIFT                                  (0x0000000CU)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_3_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_3_MAX                                    (0x00000001U)

#define CSL_HSM_PKA_GF2M_CMD_TGT_MASK                                          (0x00006000U)
#define CSL_HSM_PKA_GF2M_CMD_TGT_SHIFT                                         (0x0000000DU)
#define CSL_HSM_PKA_GF2M_CMD_TGT_RESETVAL                                      (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_TGT_MAX                                           (0x00000003U)

#define CSL_HSM_PKA_GF2M_CMD_RESERVED_4_MASK                                   (0xFFFF8000U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_4_SHIFT                                  (0x0000000FU)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_4_RESETVAL                               (0x00000000U)
#define CSL_HSM_PKA_GF2M_CMD_RESERVED_4_MAX                                    (0x0001FFFFU)

#define CSL_HSM_PKA_GF2M_CMD_RESETVAL                                          (0x00000000U)

/* GF2M_STAT */

#define CSL_HSM_PKA_GF2M_STAT_MSB_PTR_MASK                                     (0x0000001FU)
#define CSL_HSM_PKA_GF2M_STAT_MSB_PTR_SHIFT                                    (0x00000000U)
#define CSL_HSM_PKA_GF2M_STAT_MSB_PTR_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_GF2M_STAT_MSB_PTR_MAX                                      (0x0000001FU)

#define CSL_HSM_PKA_GF2M_STAT_NO_MSB_MASK                                      (0x00000020U)
#define CSL_HSM_PKA_GF2M_STAT_NO_MSB_SHIFT                                     (0x00000005U)
#define CSL_HSM_PKA_GF2M_STAT_NO_MSB_RESETVAL                                  (0x00000000U)
#define CSL_HSM_PKA_GF2M_STAT_NO_MSB_MAX                                       (0x00000001U)

#define CSL_HSM_PKA_GF2M_STAT_RESERVED_1_MASK                                  (0x00007FC0U)
#define CSL_HSM_PKA_GF2M_STAT_RESERVED_1_SHIFT                                 (0x00000006U)
#define CSL_HSM_PKA_GF2M_STAT_RESERVED_1_RESETVAL                              (0x00000000U)
#define CSL_HSM_PKA_GF2M_STAT_RESERVED_1_MAX                                   (0x000001FFU)

#define CSL_HSM_PKA_GF2M_STAT_CMD_ERR_MASK                                     (0x00008000U)
#define CSL_HSM_PKA_GF2M_STAT_CMD_ERR_SHIFT                                    (0x0000000FU)
#define CSL_HSM_PKA_GF2M_STAT_CMD_ERR_RESETVAL                                 (0x00000000U)
#define CSL_HSM_PKA_GF2M_STAT_CMD_ERR_MAX                                      (0x00000001U)

#define CSL_HSM_PKA_GF2M_STAT_SHIFT_VALUE_MASK                                 (0x03FF0000U)
#define CSL_HSM_PKA_GF2M_STAT_SHIFT_VALUE_SHIFT                                (0x00000010U)
#define CSL_HSM_PKA_GF2M_STAT_SHIFT_VALUE_RESETVAL                             (0x00000000U)
#define CSL_HSM_PKA_GF2M_STAT_SHIFT_VALUE_MAX                                  (0x000003FFU)

#define CSL_HSM_PKA_GF2M_STAT_RESERVED_2_MASK                                  (0x7C000000U)
#define CSL_HSM_PKA_GF2M_STAT_RESERVED_2_SHIFT                                 (0x0000001AU)
#define CSL_HSM_PKA_GF2M_STAT_RESERVED_2_RESETVAL                              (0x00000000U)
#define CSL_HSM_PKA_GF2M_STAT_RESERVED_2_MAX                                   (0x0000001FU)

#define CSL_HSM_PKA_GF2M_STAT_BUSY_MASK                                        (0x80000000U)
#define CSL_HSM_PKA_GF2M_STAT_BUSY_SHIFT                                       (0x0000001FU)
#define CSL_HSM_PKA_GF2M_STAT_BUSY_RESETVAL                                    (0x00000000U)
#define CSL_HSM_PKA_GF2M_STAT_BUSY_MAX                                         (0x00000001U)

#define CSL_HSM_PKA_GF2M_STAT_RESETVAL                                         (0x00000000U)

/* GF2M_FIELDSIZE */

#define CSL_HSM_PKA_GF2M_FIELDSIZE_FIELD_SIZE_MASK                             (0x000000FFU)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_FIELD_SIZE_SHIFT                            (0x00000000U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_FIELD_SIZE_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_FIELD_SIZE_MAX                              (0x000000FFU)

#define CSL_HSM_PKA_GF2M_FIELDSIZE_RESERVED_1_MASK                             (0x0000FF00U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_RESERVED_1_SHIFT                            (0x00000008U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_RESERVED_1_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_RESERVED_1_MAX                              (0x000000FFU)

#define CSL_HSM_PKA_GF2M_FIELDSIZE_OP_SHIFT_MASK                               (0x00030000U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_OP_SHIFT_SHIFT                              (0x00000010U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_OP_SHIFT_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_OP_SHIFT_MAX                                (0x00000003U)

#define CSL_HSM_PKA_GF2M_FIELDSIZE_RESERVED_2_MASK                             (0xFFFC0000U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_RESERVED_2_SHIFT                            (0x00000012U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_RESERVED_2_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_GF2M_FIELDSIZE_RESERVED_2_MAX                              (0x00003FFFU)

#define CSL_HSM_PKA_GF2M_FIELDSIZE_RESETVAL                                    (0x00000000U)

/* GF2M_OPTIONS */

#define CSL_HSM_PKA_GF2M_OPTIONS_OPERANDS_MASK                                 (0x0000000FU)
#define CSL_HSM_PKA_GF2M_OPTIONS_OPERANDS_SHIFT                                (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPTIONS_OPERANDS_RESETVAL                             (0x00000004U)
#define CSL_HSM_PKA_GF2M_OPTIONS_OPERANDS_MAX                                  (0x0000000FU)

#define CSL_HSM_PKA_GF2M_OPTIONS_MUL_DEPTH_MASK                                (0x000000F0U)
#define CSL_HSM_PKA_GF2M_OPTIONS_MUL_DEPTH_SHIFT                               (0x00000004U)
#define CSL_HSM_PKA_GF2M_OPTIONS_MUL_DEPTH_RESETVAL                            (0x00000004U)
#define CSL_HSM_PKA_GF2M_OPTIONS_MUL_DEPTH_MAX                                 (0x0000000FU)

#define CSL_HSM_PKA_GF2M_OPTIONS_RESERVED_1_MASK                               (0x0000FF00U)
#define CSL_HSM_PKA_GF2M_OPTIONS_RESERVED_1_SHIFT                              (0x00000008U)
#define CSL_HSM_PKA_GF2M_OPTIONS_RESERVED_1_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPTIONS_RESERVED_1_MAX                                (0x000000FFU)

#define CSL_HSM_PKA_GF2M_OPTIONS_OPERAND_SIZE_MASK                             (0x0FFF0000U)
#define CSL_HSM_PKA_GF2M_OPTIONS_OPERAND_SIZE_SHIFT                            (0x00000010U)
#define CSL_HSM_PKA_GF2M_OPTIONS_OPERAND_SIZE_RESETVAL                         (0x0000023CU)
#define CSL_HSM_PKA_GF2M_OPTIONS_OPERAND_SIZE_MAX                              (0x00000FFFU)

#define CSL_HSM_PKA_GF2M_OPTIONS_RESERVED_2_MASK                               (0xF0000000U)
#define CSL_HSM_PKA_GF2M_OPTIONS_RESERVED_2_SHIFT                              (0x0000001CU)
#define CSL_HSM_PKA_GF2M_OPTIONS_RESERVED_2_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_OPTIONS_RESERVED_2_MAX                                (0x0000000FU)

#define CSL_HSM_PKA_GF2M_OPTIONS_RESETVAL                                      (0x023C0044U)

/* GF2M_VERSION */

#define CSL_HSM_PKA_GF2M_VERSION_RESERVED_1_MASK                               (0x0000FFFFU)
#define CSL_HSM_PKA_GF2M_VERSION_RESERVED_1_SHIFT                              (0x00000000U)
#define CSL_HSM_PKA_GF2M_VERSION_RESERVED_1_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_VERSION_RESERVED_1_MAX                                (0x0000FFFFU)

#define CSL_HSM_PKA_GF2M_VERSION_MAJOR_VERSION_MASK                            (0x000F0000U)
#define CSL_HSM_PKA_GF2M_VERSION_MAJOR_VERSION_SHIFT                           (0x00000010U)
#define CSL_HSM_PKA_GF2M_VERSION_MAJOR_VERSION_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_VERSION_MAJOR_VERSION_MAX                             (0x0000000FU)

#define CSL_HSM_PKA_GF2M_VERSION_MINOR_VERSION_MASK                            (0x00F00000U)
#define CSL_HSM_PKA_GF2M_VERSION_MINOR_VERSION_SHIFT                           (0x00000014U)
#define CSL_HSM_PKA_GF2M_VERSION_MINOR_VERSION_RESETVAL                        (0x00000000U)
#define CSL_HSM_PKA_GF2M_VERSION_MINOR_VERSION_MAX                             (0x0000000FU)

#define CSL_HSM_PKA_GF2M_VERSION_PATCH_LEVEL_MASK                              (0x0F000000U)
#define CSL_HSM_PKA_GF2M_VERSION_PATCH_LEVEL_SHIFT                             (0x00000018U)
#define CSL_HSM_PKA_GF2M_VERSION_PATCH_LEVEL_RESETVAL                          (0x00000002U)
#define CSL_HSM_PKA_GF2M_VERSION_PATCH_LEVEL_MAX                               (0x0000000FU)

#define CSL_HSM_PKA_GF2M_VERSION_RESERVED_2_MASK                               (0xF0000000U)
#define CSL_HSM_PKA_GF2M_VERSION_RESERVED_2_SHIFT                              (0x0000001CU)
#define CSL_HSM_PKA_GF2M_VERSION_RESERVED_2_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_GF2M_VERSION_RESERVED_2_MAX                                (0x0000000FU)

#define CSL_HSM_PKA_GF2M_VERSION_RESETVAL                                      (0x02000000U)

/* PKA_REV */

#define CSL_HSM_PKA_PKA_REV_REVISION_MASK                                      (0xFFFFFFFFU)
#define CSL_HSM_PKA_PKA_REV_REVISION_SHIFT                                     (0x00000000U)
#define CSL_HSM_PKA_PKA_REV_REVISION_RESETVAL                                  (0x00000010U)
#define CSL_HSM_PKA_PKA_REV_REVISION_MAX                                       (0xFFFFFFFFU)

#define CSL_HSM_PKA_PKA_REV_RESETVAL                                           (0x00000010U)

/* PKA_CLK_CTRL */

#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLK_ON_MASK                               (0x00000001U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLK_ON_SHIFT                              (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLK_ON_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLK_ON_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLK_ON_MASK                               (0x00000002U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLK_ON_SHIFT                              (0x00000001U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLK_ON_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLK_ON_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLK_ON_MASK                              (0x00000004U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLK_ON_SHIFT                             (0x00000002U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLK_ON_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLK_ON_MAX                               (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLK_ON_MASK                              (0x00000008U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLK_ON_SHIFT                             (0x00000003U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLK_ON_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLK_ON_MAX                               (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLK_ON_MASK                          (0x00000010U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLK_ON_SHIFT                         (0x00000004U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLK_ON_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLK_ON_MAX                           (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLK_ON_MASK                              (0x00000020U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLK_ON_SHIFT                             (0x00000005U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLK_ON_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLK_ON_MAX                               (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLK_ON_MASK                          (0x00000040U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLK_ON_SHIFT                         (0x00000006U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLK_ON_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLK_ON_MAX                           (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_1_MASK                               (0x00000080U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_1_SHIFT                              (0x00000007U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_1_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_1_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLK_OFF_MASK                              (0x00000100U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLK_OFF_SHIFT                             (0x00000008U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLK_OFF_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLK_OFF_MAX                               (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLK_OFF_MASK                              (0x00000200U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLK_OFF_SHIFT                             (0x00000009U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLK_OFF_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLK_OFF_MAX                               (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLK_OFF_MASK                             (0x00000400U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLK_OFF_SHIFT                            (0x0000000AU)
#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLK_OFF_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLK_OFF_MAX                              (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLK_OFF_MASK                             (0x00000800U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLK_OFF_SHIFT                            (0x0000000BU)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLK_OFF_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLK_OFF_MAX                              (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLK_OFF_MASK                         (0x00001000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLK_OFF_SHIFT                        (0x0000000CU)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLK_OFF_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLK_OFF_MAX                          (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLK_OFF_MASK                             (0x00002000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLK_OFF_SHIFT                            (0x0000000DU)
#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLK_OFF_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLK_OFF_MAX                              (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLK_OFF_MASK                         (0x00004000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLK_OFF_SHIFT                        (0x0000000EU)
#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLK_OFF_RESETVAL                     (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLK_OFF_MAX                          (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_2_MASK                               (0x00008000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_2_SHIFT                              (0x0000000FU)
#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_2_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_2_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLKEN_MASK                                (0x00010000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLKEN_SHIFT                               (0x00000010U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLKEN_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_REG_CLKEN_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLKEN_MASK                                (0x00020000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLKEN_SHIFT                               (0x00000011U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLKEN_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_SEQ_CLKEN_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLKEN_MASK                               (0x00040000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLKEN_SHIFT                              (0x00000012U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLKEN_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_PKCP_CLKEN_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLKEN_MASK                               (0x00080000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLKEN_SHIFT                              (0x00000013U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLKEN_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_CLKEN_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLKEN_MASK                           (0x00100000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLKEN_SHIFT                          (0x00000014U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLKEN_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_LNME_REG_CLKEN_MAX                            (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLKEN_MASK                               (0x00200000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLKEN_SHIFT                              (0x00000015U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLKEN_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_GF2M_CLKEN_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLKEN_MASK                           (0x00400000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLKEN_SHIFT                          (0x00000016U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLKEN_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_DATA_RAM_CLKEN_MAX                            (0x00000001U)

#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_3_MASK                               (0x7F800000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_3_SHIFT                              (0x00000017U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_3_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_CLK_CTRL_RESERVED_3_MAX                                (0x000000FFU)

#define CSL_HSM_PKA_PKA_CLK_CTRL_RESETVAL                                      (0x00000000U)

/* PKA_SYSCONFIG */

#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_1_MASK                              (0x00000001U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_1_SHIFT                             (0x00000000U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_1_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_1_MAX                               (0x00000001U)

#define CSL_HSM_PKA_PKA_SYSCONFIG_SOFTRESET_MASK                               (0x00000002U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_SOFTRESET_SHIFT                              (0x00000001U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_SOFTRESET_RESETVAL                           (0x00000001U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_SOFTRESET_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_2_MASK                              (0x0000000CU)
#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_2_SHIFT                             (0x00000002U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_2_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_2_MAX                               (0x00000003U)

#define CSL_HSM_PKA_PKA_SYSCONFIG_IDLEMODE_MASK                                (0x00000030U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_IDLEMODE_SHIFT                               (0x00000004U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_IDLEMODE_RESETVAL                            (0x00000002U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_IDLEMODE_MAX                                 (0x00000003U)

#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_3_MASK                              (0xFFFFFFC0U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_3_SHIFT                             (0x00000006U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_3_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_SYSCONFIG_RESERVED_3_MAX                               (0x03FFFFFFU)

#define CSL_HSM_PKA_PKA_SYSCONFIG_RESETVAL                                     (0x00000022U)

/* PKA_SYSSTATUS */

#define CSL_HSM_PKA_PKA_SYSSTATUS_RESETDONE_MASK                               (0x00000001U)
#define CSL_HSM_PKA_PKA_SYSSTATUS_RESETDONE_SHIFT                              (0x00000000U)
#define CSL_HSM_PKA_PKA_SYSSTATUS_RESETDONE_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_SYSSTATUS_RESETDONE_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_SYSSTATUS_RESERVED_1_MASK                              (0xFFFFFFFEU)
#define CSL_HSM_PKA_PKA_SYSSTATUS_RESERVED_1_SHIFT                             (0x00000001U)
#define CSL_HSM_PKA_PKA_SYSSTATUS_RESERVED_1_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_SYSSTATUS_RESERVED_1_MAX                               (0x7FFFFFFFU)

#define CSL_HSM_PKA_PKA_SYSSTATUS_RESETVAL                                     (0x00000000U)

/* PKA_IRQSTATUS */

#define CSL_HSM_PKA_PKA_IRQSTATUS_PKAIRQSTAT_MASK                              (0x00000001U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_PKAIRQSTAT_SHIFT                             (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_PKAIRQSTAT_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_PKAIRQSTAT_MAX                               (0x00000001U)

#define CSL_HSM_PKA_PKA_IRQSTATUS_LNMEIRQSTAT_MASK                             (0x00000002U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_LNMEIRQSTAT_SHIFT                            (0x00000001U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_LNMEIRQSTAT_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_LNMEIRQSTAT_MAX                              (0x00000001U)

#define CSL_HSM_PKA_PKA_IRQSTATUS_GF2MIRQSTAT_MASK                             (0x00000004U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_GF2MIRQSTAT_SHIFT                            (0x00000002U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_GF2MIRQSTAT_RESETVAL                         (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_GF2MIRQSTAT_MAX                              (0x00000001U)

#define CSL_HSM_PKA_PKA_IRQSTATUS_RESERVED_1_MASK                              (0x0000FFF8U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_RESERVED_1_SHIFT                             (0x00000003U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_RESERVED_1_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_RESERVED_1_MAX                               (0x00001FFFU)

#define CSL_HSM_PKA_PKA_IRQSTATUS_PKAIRQSTATRAW_MASK                           (0x00010000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_PKAIRQSTATRAW_SHIFT                          (0x00000010U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_PKAIRQSTATRAW_RESETVAL                       (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_PKAIRQSTATRAW_MAX                            (0x00000001U)

#define CSL_HSM_PKA_PKA_IRQSTATUS_LNMEIRQSTATRAW_MASK                          (0x00020000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_LNMEIRQSTATRAW_SHIFT                         (0x00000011U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_LNMEIRQSTATRAW_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_LNMEIRQSTATRAW_MAX                           (0x00000001U)

#define CSL_HSM_PKA_PKA_IRQSTATUS_GF2MIRQSTATRAW_MASK                          (0x00040000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_GF2MIRQSTATRAW_SHIFT                         (0x00000012U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_GF2MIRQSTATRAW_RESETVAL                      (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_GF2MIRQSTATRAW_MAX                           (0x00000001U)

#define CSL_HSM_PKA_PKA_IRQSTATUS_RESERVED_2_MASK                              (0xFFF80000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_RESERVED_2_SHIFT                             (0x00000013U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_RESERVED_2_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQSTATUS_RESERVED_2_MAX                               (0x00001FFFU)

#define CSL_HSM_PKA_PKA_IRQSTATUS_RESETVAL                                     (0x00000000U)

/* PKA_IRQENABLE */

#define CSL_HSM_PKA_PKA_IRQENABLE_PKAIRQEN_MASK                                (0x00000001U)
#define CSL_HSM_PKA_PKA_IRQENABLE_PKAIRQEN_SHIFT                               (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQENABLE_PKAIRQEN_RESETVAL                            (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQENABLE_PKAIRQEN_MAX                                 (0x00000001U)

#define CSL_HSM_PKA_PKA_IRQENABLE_LNMEIRQEN_MASK                               (0x00000002U)
#define CSL_HSM_PKA_PKA_IRQENABLE_LNMEIRQEN_SHIFT                              (0x00000001U)
#define CSL_HSM_PKA_PKA_IRQENABLE_LNMEIRQEN_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQENABLE_LNMEIRQEN_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_IRQENABLE_GF2MIRQEN_MASK                               (0x00000004U)
#define CSL_HSM_PKA_PKA_IRQENABLE_GF2MIRQEN_SHIFT                              (0x00000002U)
#define CSL_HSM_PKA_PKA_IRQENABLE_GF2MIRQEN_RESETVAL                           (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQENABLE_GF2MIRQEN_MAX                                (0x00000001U)

#define CSL_HSM_PKA_PKA_IRQENABLE_RESERVED_1_MASK                              (0xFFFFFFF8U)
#define CSL_HSM_PKA_PKA_IRQENABLE_RESERVED_1_SHIFT                             (0x00000003U)
#define CSL_HSM_PKA_PKA_IRQENABLE_RESERVED_1_RESETVAL                          (0x00000000U)
#define CSL_HSM_PKA_PKA_IRQENABLE_RESERVED_1_MAX                               (0x1FFFFFFFU)

#define CSL_HSM_PKA_PKA_IRQENABLE_RESETVAL                                     (0x00000000U)

#ifdef __cplusplus
}
#endif
#endif
