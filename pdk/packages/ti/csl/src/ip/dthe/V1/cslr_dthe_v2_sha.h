/********************************************************************
 * Copyright (C) 2024 Texas Instruments Incorporated.
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
 *  Name        : cslr_dthe_v2_sha.h
 *  VPVERSION   : 3.0.367 - 2024.03.02.21.42.22
 *  VPREV       : 1.0.8
*/
#ifndef CSLR_DTHE_V2_SHA_H_
#define CSLR_DTHE_V2_SHA_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  : SHA registers in secure context
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t S_ODIGEST_A;
    volatile uint32_t S_ODIGEST_B;
    volatile uint32_t S_ODIGEST_C;
    volatile uint32_t S_ODIGEST_D;
    volatile uint32_t S_ODIGEST_E;
    volatile uint32_t S_ODIGEST_F;
    volatile uint32_t S_ODIGEST_G;
    volatile uint32_t S_ODIGEST_H;
    volatile uint32_t S_IDIGEST_A;
    volatile uint32_t S_IDIGEST_B;
    volatile uint32_t S_IDIGEST_C;
    volatile uint32_t S_IDIGEST_D;
    volatile uint32_t S_IDIGEST_E;
    volatile uint32_t S_IDIGEST_F;
    volatile uint32_t S_IDIGEST_G;
    volatile uint32_t S_IDIGEST_H;
    volatile uint32_t S_DIGEST_COUNT;
    volatile uint32_t S_MODE;
    volatile uint32_t S_LENGTH;
    volatile uint8_t  Resv_128[52];
    volatile uint32_t S_DATA0_IN;
    volatile uint32_t S_DATA1_IN;
    volatile uint32_t S_DATA2_IN;
    volatile uint32_t S_DATA3_IN;
    volatile uint32_t S_DATA4_IN;
    volatile uint32_t S_DATA5_IN;
    volatile uint32_t S_DATA6_IN;
    volatile uint32_t S_DATA7_IN;
    volatile uint32_t S_DATA8_IN;
    volatile uint32_t S_DATA9_IN;
    volatile uint32_t S_DATA10_IN;
    volatile uint32_t S_DATA11_IN;
    volatile uint32_t S_DATA12_IN;
    volatile uint32_t S_DATA13_IN;
    volatile uint32_t S_DATA14_IN;
    volatile uint32_t S_DATA15_IN;
    volatile uint32_t S_DATA16_IN;
    volatile uint32_t S_DATA17_IN;
    volatile uint32_t S_DATA18_IN;
    volatile uint32_t S_DATA19_IN;
    volatile uint32_t S_DATA20_IN;
    volatile uint32_t S_DATA21_IN;
    volatile uint32_t S_DATA22_IN;
    volatile uint32_t S_DATA23_IN;
    volatile uint32_t S_DATA24_IN;
    volatile uint32_t S_DATA25_IN;
    volatile uint32_t S_DATA26_IN;
    volatile uint32_t S_DATA27_IN;
    volatile uint32_t S_DATA28_IN;
    volatile uint32_t S_DATA29_IN;
    volatile uint32_t S_DATA30_IN;
    volatile uint32_t S_DATA31_IN;
    volatile uint32_t S_REVISION;
    volatile uint8_t  Resv_272[12];
    volatile uint32_t S_SYSCONFIG;
    volatile uint32_t S_SYSSTATUS;
    volatile uint32_t S_IRQSTATUS;
    volatile uint32_t S_IRQENABLE;
    volatile uint8_t  Resv_320[32];
    volatile uint32_t S_XSSTATUS;
    volatile uint32_t S_LOCKDOWN;
    volatile uint8_t  Resv_512[184];
    volatile uint32_t S_HASH512_ODIGEST_A;
    volatile uint32_t S_HASH512_ODIGEST_B;
    volatile uint32_t S_HASH512_ODIGEST_C;
    volatile uint32_t S_HASH512_ODIGEST_D;
    volatile uint32_t S_HASH512_ODIGEST_E;
    volatile uint32_t S_HASH512_ODIGEST_F;
    volatile uint32_t S_HASH512_ODIGEST_G;
    volatile uint32_t S_HASH512_ODIGEST_H;
    volatile uint32_t S_HASH512_ODIGEST_I;
    volatile uint32_t S_HASH512_ODIGEST_J;
    volatile uint32_t S_HASH512_ODIGEST_K;
    volatile uint32_t S_HASH512_ODIGEST_L;
    volatile uint32_t S_HASH512_ODIGEST_M;
    volatile uint32_t S_HASH512_ODIGEST_N;
    volatile uint32_t S_HASH512_ODIGEST_O;
    volatile uint32_t S_HASH512_ODIGEST_P;
    volatile uint32_t S_HASH512_IDIGEST_A;
    volatile uint32_t S_HASH512_IDIGEST_B;
    volatile uint32_t S_HASH512_IDIGEST_C;
    volatile uint32_t S_HASH512_IDIGEST_D;
    volatile uint32_t S_HASH512_IDIGEST_E;
    volatile uint32_t S_HASH512_IDIGEST_F;
    volatile uint32_t S_HASH512_IDIGEST_G;
    volatile uint32_t S_HASH512_IDIGEST_H;
    volatile uint32_t S_HASH512_IDIGEST_I;
    volatile uint32_t S_HASH512_IDIGEST_J;
    volatile uint32_t S_HASH512_IDIGEST_K;
    volatile uint32_t S_HASH512_IDIGEST_L;
    volatile uint32_t S_HASH512_IDIGEST_M;
    volatile uint32_t S_HASH512_IDIGEST_N;
    volatile uint32_t S_HASH512_IDIGEST_O;
    volatile uint32_t S_HASH512_IDIGEST_P;
    volatile uint32_t S_HASH512_DIGEST_COUNT;
    volatile uint32_t S_HASH512_MODE;
    volatile uint32_t S_HASH512_LENGTH;
} CSL_dthe_v2_sha_sRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DTHE_V2_SHA_S_S_ODIGEST_A                                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_B                                          (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_C                                          (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_D                                          (0x0000000CU)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_E                                          (0x00000010U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_F                                          (0x00000014U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_G                                          (0x00000018U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_H                                          (0x0000001CU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_A                                          (0x00000020U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_B                                          (0x00000024U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_C                                          (0x00000028U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_D                                          (0x0000002CU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_E                                          (0x00000030U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_F                                          (0x00000034U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_G                                          (0x00000038U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_H                                          (0x0000003CU)
#define CSL_DTHE_V2_SHA_S_S_DIGEST_COUNT                                       (0x00000040U)
#define CSL_DTHE_V2_SHA_S_S_MODE                                               (0x00000044U)
#define CSL_DTHE_V2_SHA_S_S_LENGTH                                             (0x00000048U)
#define CSL_DTHE_V2_SHA_S_S_DATA0_IN                                           (0x00000080U)
#define CSL_DTHE_V2_SHA_S_S_DATA1_IN                                           (0x00000084U)
#define CSL_DTHE_V2_SHA_S_S_DATA2_IN                                           (0x00000088U)
#define CSL_DTHE_V2_SHA_S_S_DATA3_IN                                           (0x0000008CU)
#define CSL_DTHE_V2_SHA_S_S_DATA4_IN                                           (0x00000090U)
#define CSL_DTHE_V2_SHA_S_S_DATA5_IN                                           (0x00000094U)
#define CSL_DTHE_V2_SHA_S_S_DATA6_IN                                           (0x00000098U)
#define CSL_DTHE_V2_SHA_S_S_DATA7_IN                                           (0x0000009CU)
#define CSL_DTHE_V2_SHA_S_S_DATA8_IN                                           (0x000000A0U)
#define CSL_DTHE_V2_SHA_S_S_DATA9_IN                                           (0x000000A4U)
#define CSL_DTHE_V2_SHA_S_S_DATA10_IN                                          (0x000000A8U)
#define CSL_DTHE_V2_SHA_S_S_DATA11_IN                                          (0x000000ACU)
#define CSL_DTHE_V2_SHA_S_S_DATA12_IN                                          (0x000000B0U)
#define CSL_DTHE_V2_SHA_S_S_DATA13_IN                                          (0x000000B4U)
#define CSL_DTHE_V2_SHA_S_S_DATA14_IN                                          (0x000000B8U)
#define CSL_DTHE_V2_SHA_S_S_DATA15_IN                                          (0x000000BCU)
#define CSL_DTHE_V2_SHA_S_S_DATA16_IN                                          (0x000000C0U)
#define CSL_DTHE_V2_SHA_S_S_DATA17_IN                                          (0x000000C4U)
#define CSL_DTHE_V2_SHA_S_S_DATA18_IN                                          (0x000000C8U)
#define CSL_DTHE_V2_SHA_S_S_DATA19_IN                                          (0x000000CCU)
#define CSL_DTHE_V2_SHA_S_S_DATA20_IN                                          (0x000000D0U)
#define CSL_DTHE_V2_SHA_S_S_DATA21_IN                                          (0x000000D4U)
#define CSL_DTHE_V2_SHA_S_S_DATA22_IN                                          (0x000000D8U)
#define CSL_DTHE_V2_SHA_S_S_DATA23_IN                                          (0x000000DCU)
#define CSL_DTHE_V2_SHA_S_S_DATA24_IN                                          (0x000000E0U)
#define CSL_DTHE_V2_SHA_S_S_DATA25_IN                                          (0x000000E4U)
#define CSL_DTHE_V2_SHA_S_S_DATA26_IN                                          (0x000000E8U)
#define CSL_DTHE_V2_SHA_S_S_DATA27_IN                                          (0x000000ECU)
#define CSL_DTHE_V2_SHA_S_S_DATA28_IN                                          (0x000000F0U)
#define CSL_DTHE_V2_SHA_S_S_DATA29_IN                                          (0x000000F4U)
#define CSL_DTHE_V2_SHA_S_S_DATA30_IN                                          (0x000000F8U)
#define CSL_DTHE_V2_SHA_S_S_DATA31_IN                                          (0x000000FCU)
#define CSL_DTHE_V2_SHA_S_S_REVISION                                           (0x00000100U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG                                          (0x00000110U)
#define CSL_DTHE_V2_SHA_S_S_SYSSTATUS                                          (0x00000114U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS                                          (0x00000118U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE                                          (0x0000011CU)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS                                           (0x00000140U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN                                           (0x00000144U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_A                                  (0x00000200U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_B                                  (0x00000204U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_C                                  (0x00000208U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_D                                  (0x0000020CU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_E                                  (0x00000210U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_F                                  (0x00000214U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_G                                  (0x00000218U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_H                                  (0x0000021CU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_I                                  (0x00000220U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_J                                  (0x00000224U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_K                                  (0x00000228U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_L                                  (0x0000022CU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_M                                  (0x00000230U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_N                                  (0x00000234U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_O                                  (0x00000238U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_P                                  (0x0000023CU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_A                                  (0x00000240U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_B                                  (0x00000244U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_C                                  (0x00000248U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_D                                  (0x0000024CU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_E                                  (0x00000250U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_F                                  (0x00000254U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_G                                  (0x00000258U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_H                                  (0x0000025CU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_I                                  (0x00000260U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_J                                  (0x00000264U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_K                                  (0x00000268U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_L                                  (0x0000026CU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_M                                  (0x00000270U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_N                                  (0x00000274U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_O                                  (0x00000278U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_P                                  (0x0000027CU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_DIGEST_COUNT                               (0x00000280U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE                                       (0x00000284U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_LENGTH                                     (0x00000288U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* S_ODIGEST_A */

#define CSL_DTHE_V2_SHA_S_S_ODIGEST_A_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_A_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_A_DATA_MAX                                 (0xFFFFFFFFU)


/* S_ODIGEST_B */

#define CSL_DTHE_V2_SHA_S_S_ODIGEST_B_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_B_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_B_DATA_MAX                                 (0xFFFFFFFFU)


/* S_ODIGEST_C */

#define CSL_DTHE_V2_SHA_S_S_ODIGEST_C_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_C_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_C_DATA_MAX                                 (0xFFFFFFFFU)


/* S_ODIGEST_D */

#define CSL_DTHE_V2_SHA_S_S_ODIGEST_D_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_D_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_D_DATA_MAX                                 (0xFFFFFFFFU)


/* S_ODIGEST_E */

#define CSL_DTHE_V2_SHA_S_S_ODIGEST_E_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_E_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_E_DATA_MAX                                 (0xFFFFFFFFU)


/* S_ODIGEST_F */

#define CSL_DTHE_V2_SHA_S_S_ODIGEST_F_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_F_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_F_DATA_MAX                                 (0xFFFFFFFFU)


/* S_ODIGEST_G */

#define CSL_DTHE_V2_SHA_S_S_ODIGEST_G_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_G_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_G_DATA_MAX                                 (0xFFFFFFFFU)


/* S_ODIGEST_H */

#define CSL_DTHE_V2_SHA_S_S_ODIGEST_H_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_H_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_ODIGEST_H_DATA_MAX                                 (0xFFFFFFFFU)


/* S_IDIGEST_A */

#define CSL_DTHE_V2_SHA_S_S_IDIGEST_A_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_A_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_A_DATA_MAX                                 (0xFFFFFFFFU)


/* S_IDIGEST_B */

#define CSL_DTHE_V2_SHA_S_S_IDIGEST_B_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_B_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_B_DATA_MAX                                 (0xFFFFFFFFU)


/* S_IDIGEST_C */

#define CSL_DTHE_V2_SHA_S_S_IDIGEST_C_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_C_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_C_DATA_MAX                                 (0xFFFFFFFFU)


/* S_IDIGEST_D */

#define CSL_DTHE_V2_SHA_S_S_IDIGEST_D_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_D_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_D_DATA_MAX                                 (0xFFFFFFFFU)


/* S_IDIGEST_E */

#define CSL_DTHE_V2_SHA_S_S_IDIGEST_E_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_E_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_E_DATA_MAX                                 (0xFFFFFFFFU)


/* S_IDIGEST_F */

#define CSL_DTHE_V2_SHA_S_S_IDIGEST_F_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_F_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_F_DATA_MAX                                 (0xFFFFFFFFU)


/* S_IDIGEST_G */

#define CSL_DTHE_V2_SHA_S_S_IDIGEST_G_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_G_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_G_DATA_MAX                                 (0xFFFFFFFFU)


/* S_IDIGEST_H */

#define CSL_DTHE_V2_SHA_S_S_IDIGEST_H_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_H_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IDIGEST_H_DATA_MAX                                 (0xFFFFFFFFU)


/* S_DIGEST_COUNT */

#define CSL_DTHE_V2_SHA_S_S_DIGEST_COUNT_DATA_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DIGEST_COUNT_DATA_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DIGEST_COUNT_DATA_MAX                              (0xFFFFFFFFU)


/* S_MODE */

#define CSL_DTHE_V2_SHA_S_S_MODE_ALGO_MASK                                     (0x00000007U)
#define CSL_DTHE_V2_SHA_S_S_MODE_ALGO_SHIFT                                    (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_MODE_ALGO_MAX                                      (0x00000007U)

#define CSL_DTHE_V2_SHA_S_S_MODE_ALGO_CONSTANT_MASK                            (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_MODE_ALGO_CONSTANT_SHIFT                           (0x00000003U)
#define CSL_DTHE_V2_SHA_S_S_MODE_ALGO_CONSTANT_MAX                             (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_MODE_CLOSE_HASH_MASK                               (0x00000010U)
#define CSL_DTHE_V2_SHA_S_S_MODE_CLOSE_HASH_SHIFT                              (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_MODE_CLOSE_HASH_MAX                                (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_MODE_HMAC_KEY_PROC_MASK                            (0x00000020U)
#define CSL_DTHE_V2_SHA_S_S_MODE_HMAC_KEY_PROC_SHIFT                           (0x00000005U)
#define CSL_DTHE_V2_SHA_S_S_MODE_HMAC_KEY_PROC_MAX                             (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_MODE_REUSE_HMAC_KEY_MASK                           (0x00000040U)
#define CSL_DTHE_V2_SHA_S_S_MODE_REUSE_HMAC_KEY_SHIFT                          (0x00000006U)
#define CSL_DTHE_V2_SHA_S_S_MODE_REUSE_HMAC_KEY_MAX                            (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_MODE_HMAC_OUTER_HASH_MASK                          (0x00000080U)
#define CSL_DTHE_V2_SHA_S_S_MODE_HMAC_OUTER_HASH_SHIFT                         (0x00000007U)
#define CSL_DTHE_V2_SHA_S_S_MODE_HMAC_OUTER_HASH_MAX                           (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_MODE_RESERVED_MASK                                 (0xFFFFFF00U)
#define CSL_DTHE_V2_SHA_S_S_MODE_RESERVED_SHIFT                                (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_MODE_RESERVED_MAX                                  (0x00FFFFFFU)


/* S_LENGTH */

#define CSL_DTHE_V2_SHA_S_S_LENGTH_DATA_MASK                                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_LENGTH_DATA_SHIFT                                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_LENGTH_DATA_MAX                                    (0xFFFFFFFFU)


/* S_DATA0_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA0_IN_DATA0_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA0_IN_DATA0_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA0_IN_DATA0_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA1_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA1_IN_DATA1_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA1_IN_DATA1_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA1_IN_DATA1_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA2_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA2_IN_DATA2_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA2_IN_DATA2_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA2_IN_DATA2_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA3_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA3_IN_DATA3_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA3_IN_DATA3_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA3_IN_DATA3_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA4_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA4_IN_DATA4_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA4_IN_DATA4_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA4_IN_DATA4_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA5_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA5_IN_DATA5_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA5_IN_DATA5_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA5_IN_DATA5_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA6_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA6_IN_DATA6_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA6_IN_DATA6_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA6_IN_DATA6_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA7_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA7_IN_DATA7_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA7_IN_DATA7_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA7_IN_DATA7_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA8_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA8_IN_DATA8_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA8_IN_DATA8_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA8_IN_DATA8_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA9_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA9_IN_DATA9_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA9_IN_DATA9_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA9_IN_DATA9_IN_MAX                              (0xFFFFFFFFU)


/* S_DATA10_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA10_IN_DATA10_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA10_IN_DATA10_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA10_IN_DATA10_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA11_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA11_IN_DATA11_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA11_IN_DATA11_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA11_IN_DATA11_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA12_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA12_IN_DATA12_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA12_IN_DATA12_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA12_IN_DATA12_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA13_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA13_IN_DATA13_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA13_IN_DATA13_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA13_IN_DATA13_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA14_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA14_IN_DATA14_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA14_IN_DATA14_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA14_IN_DATA14_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA15_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA15_IN_DATA15_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA15_IN_DATA15_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA15_IN_DATA15_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA16_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA16_IN_DATA16_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA16_IN_DATA16_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA16_IN_DATA16_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA17_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA17_IN_DATA17_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA17_IN_DATA17_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA17_IN_DATA17_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA18_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA18_IN_DATA18_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA18_IN_DATA18_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA18_IN_DATA18_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA19_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA19_IN_DATA19_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA19_IN_DATA19_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA19_IN_DATA19_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA20_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA20_IN_DATA20_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA20_IN_DATA20_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA20_IN_DATA20_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA21_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA21_IN_DATA21_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA21_IN_DATA21_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA21_IN_DATA21_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA22_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA22_IN_DATA22_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA22_IN_DATA22_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA22_IN_DATA22_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA23_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA23_IN_DATA23_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA23_IN_DATA23_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA23_IN_DATA23_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA24_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA24_IN_DATA24_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA24_IN_DATA24_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA24_IN_DATA24_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA25_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA25_IN_DATA25_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA25_IN_DATA25_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA25_IN_DATA25_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA26_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA26_IN_DATA26_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA26_IN_DATA26_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA26_IN_DATA26_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA27_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA27_IN_DATA27_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA27_IN_DATA27_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA27_IN_DATA27_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA28_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA28_IN_DATA28_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA28_IN_DATA28_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA28_IN_DATA28_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA29_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA29_IN_DATA29_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA29_IN_DATA29_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA29_IN_DATA29_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA30_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA30_IN_DATA30_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA30_IN_DATA30_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA30_IN_DATA30_IN_MAX                            (0xFFFFFFFFU)


/* S_DATA31_IN */

#define CSL_DTHE_V2_SHA_S_S_DATA31_IN_DATA31_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_DATA31_IN_DATA31_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_DATA31_IN_DATA31_IN_MAX                            (0xFFFFFFFFU)


/* S_REVISION */

#define CSL_DTHE_V2_SHA_S_S_REVISION_Y_MINOR_MASK                              (0x0000003FU)
#define CSL_DTHE_V2_SHA_S_S_REVISION_Y_MINOR_SHIFT                             (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_Y_MINOR_MAX                               (0x0000003FU)

#define CSL_DTHE_V2_SHA_S_S_REVISION_CUSTOM_MASK                               (0x000000C0U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_CUSTOM_SHIFT                              (0x00000006U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_CUSTOM_MAX                                (0x00000003U)

#define CSL_DTHE_V2_SHA_S_S_REVISION_X_MAJOR_MASK                              (0x00000700U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_X_MAJOR_SHIFT                             (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_X_MAJOR_MAX                               (0x00000007U)

#define CSL_DTHE_V2_SHA_S_S_REVISION_R_RTL_MASK                                (0x0000F800U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_R_RTL_SHIFT                               (0x0000000BU)
#define CSL_DTHE_V2_SHA_S_S_REVISION_R_RTL_MAX                                 (0x0000001FU)

#define CSL_DTHE_V2_SHA_S_S_REVISION_FUNC_MASK                                 (0x0FFF0000U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_FUNC_SHIFT                                (0x00000010U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_FUNC_MAX                                  (0x00000FFFU)

#define CSL_DTHE_V2_SHA_S_S_REVISION_RESERVED1_MASK                            (0x30000000U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_RESERVED1_SHIFT                           (0x0000001CU)
#define CSL_DTHE_V2_SHA_S_S_REVISION_RESERVED1_MAX                             (0x00000003U)

#define CSL_DTHE_V2_SHA_S_S_REVISION_SCHEME_MASK                               (0xC0000000U)
#define CSL_DTHE_V2_SHA_S_S_REVISION_SCHEME_SHIFT                              (0x0000001EU)
#define CSL_DTHE_V2_SHA_S_S_REVISION_SCHEME_MAX                                (0x00000003U)


/* S_SYSCONFIG */

#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_AUTOIDLE_MASK                            (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_AUTOIDLE_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_AUTOIDLE_MAX                             (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_SOFTRESET_MASK                           (0x00000002U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_SOFTRESET_SHIFT                          (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_SOFTRESET_MAX                            (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PIT_EN_MASK                              (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PIT_EN_SHIFT                             (0x00000002U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PIT_EN_MAX                               (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PDMA_EN_MASK                             (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PDMA_EN_SHIFT                            (0x00000003U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PDMA_EN_MAX                              (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_IDLE_MODE_MASK                           (0x00000030U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_IDLE_MODE_SHIFT                          (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_IDLE_MODE_MAX                            (0x00000003U)

#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PCONT_SWT_MASK                           (0x00000040U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PCONT_SWT_SHIFT                          (0x00000006U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PCONT_SWT_MAX                            (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PADVANCED_MASK                           (0x00000080U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PADVANCED_SHIFT                          (0x00000007U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_PADVANCED_MAX                            (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_RESERVED_MASK                            (0xFFFFFF00U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_RESERVED_SHIFT                           (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_SYSCONFIG_RESERVED_MAX                             (0x00FFFFFFU)


/* S_SYSSTATUS */

#define CSL_DTHE_V2_SHA_S_S_SYSSTATUS_RESETDONE_MASK                           (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_SYSSTATUS_RESETDONE_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_SYSSTATUS_RESETDONE_MAX                            (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_SYSSTATUS_RESERVED_MASK                            (0xFFFFFFFEU)
#define CSL_DTHE_V2_SHA_S_S_SYSSTATUS_RESERVED_SHIFT                           (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_SYSSTATUS_RESERVED_MAX                             (0x7FFFFFFFU)


/* S_IRQSTATUS */

#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_OUTPUT_READY_MASK                        (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_OUTPUT_READY_SHIFT                       (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_OUTPUT_READY_MAX                         (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_INPUT_READY_MASK                         (0x00000002U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_INPUT_READY_SHIFT                        (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_INPUT_READY_MAX                          (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_PARTHASH_READY_MASK                      (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_PARTHASH_READY_SHIFT                     (0x00000002U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_PARTHASH_READY_MAX                       (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_CONTEXT_READY_MASK                       (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_CONTEXT_READY_SHIFT                      (0x00000003U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_CONTEXT_READY_MAX                        (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_RESERVED_MASK                            (0xFFFFFFF0U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_RESERVED_SHIFT                           (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_IRQSTATUS_RESERVED_MAX                             (0x0FFFFFFFU)


/* S_IRQENABLE */

#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_OUTPUT_READY_MASK                      (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_OUTPUT_READY_SHIFT                     (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_OUTPUT_READY_MAX                       (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_INPUT_READY_MASK                       (0x00000002U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_INPUT_READY_SHIFT                      (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_INPUT_READY_MAX                        (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_PARTHASH_READY_MASK                    (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_PARTHASH_READY_SHIFT                   (0x00000002U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_PARTHASH_READY_MAX                     (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_CONTEXT_READY_MASK                     (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_CONTEXT_READY_SHIFT                    (0x00000003U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_M_CONTEXT_READY_MAX                      (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_RESERVED_MASK                            (0xFFFFFFF0U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_RESERVED_SHIFT                           (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_IRQENABLE_RESERVED_MAX                             (0x0FFFFFFFU)


/* S_XSSTATUS */

#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_SACCESSED_MASK                            (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_SACCESSED_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_SACCESSED_MAX                             (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_SDIRTY_MASK                               (0x00000002U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_SDIRTY_SHIFT                              (0x00000001U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_SDIRTY_MAX                                (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_PACCESSED_MASK                            (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_PACCESSED_SHIFT                           (0x00000002U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_PACCESSED_MAX                             (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_PDIRTY_MASK                               (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_PDIRTY_SHIFT                              (0x00000003U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_PDIRTY_MAX                                (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_RESERVED_MASK                             (0xFFFFFFF0U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_RESERVED_SHIFT                            (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_XSSTATUS_RESERVED_MAX                              (0x0FFFFFFFU)


/* S_LOCKDOWN */

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_ALGO_MASK                            (0x00000007U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_ALGO_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_ALGO_MAX                             (0x00000007U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_USE_ALOG_CONST_MASK                  (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_USE_ALOG_CONST_SHIFT                 (0x00000003U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_USE_ALOG_CONST_MAX                   (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_CLOSE_HASH_MASK                      (0x00000010U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_CLOSE_HASH_SHIFT                     (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_CLOSE_HASH_MAX                       (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_HMAC_KEY_MASK                        (0x00000020U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_HMAC_KEY_SHIFT                       (0x00000005U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_HMAC_KEY_MAX                         (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_REUSE_HMAC_KEY_MASK                  (0x00000040U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_REUSE_HMAC_KEY_SHIFT                 (0x00000006U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_REUSE_HMAC_KEY_MAX                   (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_HMAC_OUTER_HASH_MASK                 (0x00000080U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_HMAC_OUTER_HASH_SHIFT                (0x00000007U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_HMAC_OUTER_HASH_MAX                  (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_RESERVED1_MASK                            (0x00FFFF00U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_RESERVED1_SHIFT                           (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_RESERVED1_MAX                             (0x0000FFFFU)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_IDIGEST_MASK                         (0x01000000U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_IDIGEST_SHIFT                        (0x00000018U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_IDIGEST_MAX                          (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_ODIGEST_MASK                         (0x02000000U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_ODIGEST_SHIFT                        (0x00000019U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_ODIGEST_MAX                          (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_DIGESTCOUNT_MASK                     (0x04000000U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_DIGESTCOUNT_SHIFT                    (0x0000001AU)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_DIGESTCOUNT_MAX                      (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_LENGTH_MASK                          (0x08000000U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_LENGTH_SHIFT                         (0x0000001BU)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_LOCK_LENGTH_MAX                           (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_BLOCK_ODIGEST_RD_MASK                     (0x10000000U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_BLOCK_ODIGEST_RD_SHIFT                    (0x0000001CU)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_BLOCK_ODIGEST_RD_MAX                      (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_RESERVED2_MASK                            (0xE0000000U)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_RESERVED2_SHIFT                           (0x0000001DU)
#define CSL_DTHE_V2_SHA_S_S_LOCKDOWN_RESERVED2_MAX                             (0x00000007U)


/* S_HASH512_ODIGEST_A */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_A_ODIGEST_A_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_A_ODIGEST_A_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_A_ODIGEST_A_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_B */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_B_ODIGEST_B_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_B_ODIGEST_B_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_B_ODIGEST_B_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_C */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_C_ODIGEST_C_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_C_ODIGEST_C_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_C_ODIGEST_C_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_D */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_D_ODIGEST_D_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_D_ODIGEST_D_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_D_ODIGEST_D_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_E */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_E_ODIGEST_E_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_E_ODIGEST_E_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_E_ODIGEST_E_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_F */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_F_ODIGEST_F_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_F_ODIGEST_F_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_F_ODIGEST_F_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_G */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_G_ODIGEST_G_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_G_ODIGEST_G_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_G_ODIGEST_G_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_H */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_H_ODIGEST_H_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_H_ODIGEST_H_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_H_ODIGEST_H_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_I */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_I_ODIGEST_I_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_I_ODIGEST_I_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_I_ODIGEST_I_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_J */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_J_ODIGEST_J_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_J_ODIGEST_J_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_J_ODIGEST_J_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_K */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_K_ODIGEST_K_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_K_ODIGEST_K_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_K_ODIGEST_K_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_L */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_L_ODIGEST_L_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_L_ODIGEST_L_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_L_ODIGEST_L_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_M */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_M_ODIGEST_M_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_M_ODIGEST_M_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_M_ODIGEST_M_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_N */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_N_ODIGEST_N_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_N_ODIGEST_N_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_N_ODIGEST_N_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_O */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_O_ODIGEST_O_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_O_ODIGEST_O_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_O_ODIGEST_O_MAX                    (0xFFFFFFFFU)


/* S_HASH512_ODIGEST_P */

#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_P_ODIGEST_P_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_P_ODIGEST_P_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_ODIGEST_P_ODIGEST_P_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_A */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_A_IDIGEST_A_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_A_IDIGEST_A_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_A_IDIGEST_A_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_B */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_B_IDIGEST_B_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_B_IDIGEST_B_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_B_IDIGEST_B_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_C */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_C_IDIGEST_C_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_C_IDIGEST_C_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_C_IDIGEST_C_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_D */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_D_IDIGEST_D_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_D_IDIGEST_D_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_D_IDIGEST_D_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_E */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_E_IDIGEST_E_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_E_IDIGEST_E_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_E_IDIGEST_E_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_F */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_F_IDIGEST_F_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_F_IDIGEST_F_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_F_IDIGEST_F_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_G */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_G_IDIGEST_G_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_G_IDIGEST_G_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_G_IDIGEST_G_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_H */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_H_IDIGEST_H_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_H_IDIGEST_H_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_H_IDIGEST_H_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_I */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_I_IDIGEST_I_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_I_IDIGEST_I_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_I_IDIGEST_I_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_J */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_J_IDIGEST_J_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_J_IDIGEST_J_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_J_IDIGEST_J_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_K */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_K_IDIGEST_K_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_K_IDIGEST_K_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_K_IDIGEST_K_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_L */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_L_IDIGEST_L_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_L_IDIGEST_L_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_L_IDIGEST_L_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_M */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_M_IDIGEST_M_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_M_IDIGEST_M_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_M_IDIGEST_M_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_N */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_N_IDIGEST_N_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_N_IDIGEST_N_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_N_IDIGEST_N_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_O */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_O_IDIGEST_O_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_O_IDIGEST_O_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_O_IDIGEST_O_MAX                    (0xFFFFFFFFU)


/* S_HASH512_IDIGEST_P */

#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_P_IDIGEST_P_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_P_IDIGEST_P_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_IDIGEST_P_IDIGEST_P_MAX                    (0xFFFFFFFFU)


/* S_HASH512_DIGEST_COUNT */

#define CSL_DTHE_V2_SHA_S_S_HASH512_DIGEST_COUNT_DIGEST_COUNT_MASK             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_DIGEST_COUNT_DIGEST_COUNT_SHIFT            (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_DIGEST_COUNT_DIGEST_COUNT_MAX              (0xFFFFFFFFU)


/* S_HASH512_MODE */

#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_ALGORITHM_MASK                        (0x00000007U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_ALGORITHM_SHIFT                       (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_ALGORITHM_MAX                         (0x00000007U)

#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_USE_ALG_CONSTANTS_MASK                (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_USE_ALG_CONSTANTS_SHIFT               (0x00000003U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_USE_ALG_CONSTANTS_MAX                 (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_CLOSE_HASH_MASK                       (0x00000010U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_CLOSE_HASH_SHIFT                      (0x00000004U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_CLOSE_HASH_MAX                        (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_HMAC_KEY_PROCESSING_MASK              (0x00000020U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_HMAC_KEY_PROCESSING_SHIFT             (0x00000005U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_HMAC_KEY_PROCESSING_MAX               (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_REUSE_HMAC_KEY_MASK                   (0x00000040U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_REUSE_HMAC_KEY_SHIFT                  (0x00000006U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_REUSE_HMAC_KEY_MAX                    (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_HMAC_OUTER_HASH_MASK                  (0x00000080U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_HMAC_OUTER_HASH_SHIFT                 (0x00000007U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_HMAC_OUTER_HASH_MAX                   (0x00000001U)

#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_RESERVED_MASK                         (0xFFFFFF00U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_RESERVED_SHIFT                        (0x00000008U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_MODE_RESERVED_MAX                          (0x00FFFFFFU)


/* S_HASH512_LENGTH */

#define CSL_DTHE_V2_SHA_S_S_HASH512_LENGTH_LENGTH_MASK                         (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_S_S_HASH512_LENGTH_LENGTH_SHIFT                        (0x00000000U)
#define CSL_DTHE_V2_SHA_S_S_HASH512_LENGTH_LENGTH_MAX                          (0xFFFFFFFFU)


/**************************************************************************
* Hardware Region  : SHA registers in public context
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t P_ODIGEST_A;
    volatile uint32_t P_ODIGEST_B;
    volatile uint32_t P_ODIGEST_C;
    volatile uint32_t P_ODIGEST_D;
    volatile uint32_t P_ODIGEST_E;
    volatile uint32_t P_ODIGEST_F;
    volatile uint32_t P_ODIGEST_G;
    volatile uint32_t P_ODIGEST_H;
    volatile uint32_t P_IDIGEST_A;
    volatile uint32_t P_IDIGEST_B;
    volatile uint32_t P_IDIGEST_C;
    volatile uint32_t P_IDIGEST_D;
    volatile uint32_t P_IDIGEST_E;
    volatile uint32_t P_IDIGEST_F;
    volatile uint32_t P_IDIGEST_G;
    volatile uint32_t P_IDIGEST_H;
    volatile uint32_t P_DIGEST_COUNT;
    volatile uint32_t P_MODE;
    volatile uint32_t P_LENGTH;
    volatile uint8_t  Resv_128[52];
    volatile uint32_t P_DATA0_IN;
    volatile uint32_t P_DATA1_IN;
    volatile uint32_t P_DATA2_IN;
    volatile uint32_t P_DATA3_IN;
    volatile uint32_t P_DATA4_IN;
    volatile uint32_t P_DATA5_IN;
    volatile uint32_t P_DATA6_IN;
    volatile uint32_t P_DATA7_IN;
    volatile uint32_t P_DATA8_IN;
    volatile uint32_t P_DATA9_IN;
    volatile uint32_t P_DATA10_IN;
    volatile uint32_t P_DATA11_IN;
    volatile uint32_t P_DATA12_IN;
    volatile uint32_t P_DATA13_IN;
    volatile uint32_t P_DATA14_IN;
    volatile uint32_t P_DATA15_IN;
    volatile uint32_t P_DATA16_IN;
    volatile uint32_t P_DATA17_IN;
    volatile uint32_t P_DATA18_IN;
    volatile uint32_t P_DATA19_IN;
    volatile uint32_t P_DATA20_IN;
    volatile uint32_t P_DATA21_IN;
    volatile uint32_t P_DATA22_IN;
    volatile uint32_t P_DATA23_IN;
    volatile uint32_t P_DATA24_IN;
    volatile uint32_t P_DATA25_IN;
    volatile uint32_t P_DATA26_IN;
    volatile uint32_t P_DATA27_IN;
    volatile uint32_t P_DATA28_IN;
    volatile uint32_t P_DATA29_IN;
    volatile uint32_t P_DATA30_IN;
    volatile uint32_t P_DATA31_IN;
    volatile uint32_t P_REVISION;
    volatile uint8_t  Resv_272[12];
    volatile uint32_t P_SYSCONFIG;
    volatile uint32_t P_SYSSTATUS;
    volatile uint32_t P_IRQSTATUS;
    volatile uint32_t P_IRQENABLE;
    volatile uint8_t  Resv_512[224];
    volatile uint32_t P_HASH512_ODIGEST_A;
    volatile uint32_t P_HASH512_ODIGEST_B;
    volatile uint32_t P_HASH512_ODIGEST_C;
    volatile uint32_t P_HASH512_ODIGEST_D;
    volatile uint32_t P_HASH512_ODIGEST_E;
    volatile uint32_t P_HASH512_ODIGEST_F;
    volatile uint32_t P_HASH512_ODIGEST_G;
    volatile uint32_t P_HASH512_ODIGEST_H;
    volatile uint32_t P_HASH512_ODIGEST_I;
    volatile uint32_t P_HASH512_ODIGEST_J;
    volatile uint32_t P_HASH512_ODIGEST_K;
    volatile uint32_t P_HASH512_ODIGEST_L;
    volatile uint32_t P_HASH512_ODIGEST_M;
    volatile uint32_t P_HASH512_ODIGEST_N;
    volatile uint32_t P_HASH512_ODIGEST_O;
    volatile uint32_t P_HASH512_ODIGEST_P;
    volatile uint32_t P_HASH512_IDIGEST_A;
    volatile uint32_t P_HASH512_IDIGEST_B;
    volatile uint32_t P_HASH512_IDIGEST_C;
    volatile uint32_t P_HASH512_IDIGEST_D;
    volatile uint32_t P_HASH512_IDIGEST_E;
    volatile uint32_t P_HASH512_IDIGEST_F;
    volatile uint32_t P_HASH512_IDIGEST_G;
    volatile uint32_t P_HASH512_IDIGEST_H;
    volatile uint32_t P_HASH512_IDIGEST_I;
    volatile uint32_t P_HASH512_IDIGEST_J;
    volatile uint32_t P_HASH512_IDIGEST_K;
    volatile uint32_t P_HASH512_IDIGEST_L;
    volatile uint32_t P_HASH512_IDIGEST_M;
    volatile uint32_t P_HASH512_IDIGEST_N;
    volatile uint32_t P_HASH512_IDIGEST_O;
    volatile uint32_t P_HASH512_IDIGEST_P;
    volatile uint32_t P_HASH512_DIGEST_COUNT;
    volatile uint32_t P_HASH512_MODE;
    volatile uint32_t P_HASH512_LENGTH;
} CSL_dthe_v2_sha_pRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DTHE_V2_SHA_P_P_ODIGEST_A                                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_B                                          (0x00000004U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_C                                          (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_D                                          (0x0000000CU)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_E                                          (0x00000010U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_F                                          (0x00000014U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_G                                          (0x00000018U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_H                                          (0x0000001CU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_A                                          (0x00000020U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_B                                          (0x00000024U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_C                                          (0x00000028U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_D                                          (0x0000002CU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_E                                          (0x00000030U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_F                                          (0x00000034U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_G                                          (0x00000038U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_H                                          (0x0000003CU)
#define CSL_DTHE_V2_SHA_P_P_DIGEST_COUNT                                       (0x00000040U)
#define CSL_DTHE_V2_SHA_P_P_MODE                                               (0x00000044U)
#define CSL_DTHE_V2_SHA_P_P_LENGTH                                             (0x00000048U)
#define CSL_DTHE_V2_SHA_P_P_DATA0_IN                                           (0x00000080U)
#define CSL_DTHE_V2_SHA_P_P_DATA1_IN                                           (0x00000084U)
#define CSL_DTHE_V2_SHA_P_P_DATA2_IN                                           (0x00000088U)
#define CSL_DTHE_V2_SHA_P_P_DATA3_IN                                           (0x0000008CU)
#define CSL_DTHE_V2_SHA_P_P_DATA4_IN                                           (0x00000090U)
#define CSL_DTHE_V2_SHA_P_P_DATA5_IN                                           (0x00000094U)
#define CSL_DTHE_V2_SHA_P_P_DATA6_IN                                           (0x00000098U)
#define CSL_DTHE_V2_SHA_P_P_DATA7_IN                                           (0x0000009CU)
#define CSL_DTHE_V2_SHA_P_P_DATA8_IN                                           (0x000000A0U)
#define CSL_DTHE_V2_SHA_P_P_DATA9_IN                                           (0x000000A4U)
#define CSL_DTHE_V2_SHA_P_P_DATA10_IN                                          (0x000000A8U)
#define CSL_DTHE_V2_SHA_P_P_DATA11_IN                                          (0x000000ACU)
#define CSL_DTHE_V2_SHA_P_P_DATA12_IN                                          (0x000000B0U)
#define CSL_DTHE_V2_SHA_P_P_DATA13_IN                                          (0x000000B4U)
#define CSL_DTHE_V2_SHA_P_P_DATA14_IN                                          (0x000000B8U)
#define CSL_DTHE_V2_SHA_P_P_DATA15_IN                                          (0x000000BCU)
#define CSL_DTHE_V2_SHA_P_P_DATA16_IN                                          (0x000000C0U)
#define CSL_DTHE_V2_SHA_P_P_DATA17_IN                                          (0x000000C4U)
#define CSL_DTHE_V2_SHA_P_P_DATA18_IN                                          (0x000000C8U)
#define CSL_DTHE_V2_SHA_P_P_DATA19_IN                                          (0x000000CCU)
#define CSL_DTHE_V2_SHA_P_P_DATA20_IN                                          (0x000000D0U)
#define CSL_DTHE_V2_SHA_P_P_DATA21_IN                                          (0x000000D4U)
#define CSL_DTHE_V2_SHA_P_P_DATA22_IN                                          (0x000000D8U)
#define CSL_DTHE_V2_SHA_P_P_DATA23_IN                                          (0x000000DCU)
#define CSL_DTHE_V2_SHA_P_P_DATA24_IN                                          (0x000000E0U)
#define CSL_DTHE_V2_SHA_P_P_DATA25_IN                                          (0x000000E4U)
#define CSL_DTHE_V2_SHA_P_P_DATA26_IN                                          (0x000000E8U)
#define CSL_DTHE_V2_SHA_P_P_DATA27_IN                                          (0x000000ECU)
#define CSL_DTHE_V2_SHA_P_P_DATA28_IN                                          (0x000000F0U)
#define CSL_DTHE_V2_SHA_P_P_DATA29_IN                                          (0x000000F4U)
#define CSL_DTHE_V2_SHA_P_P_DATA30_IN                                          (0x000000F8U)
#define CSL_DTHE_V2_SHA_P_P_DATA31_IN                                          (0x000000FCU)
#define CSL_DTHE_V2_SHA_P_P_REVISION                                           (0x00000100U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG                                          (0x00000110U)
#define CSL_DTHE_V2_SHA_P_P_SYSSTATUS                                          (0x00000114U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS                                          (0x00000118U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE                                          (0x0000011CU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_A                                  (0x00000200U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_B                                  (0x00000204U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_C                                  (0x00000208U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_D                                  (0x0000020CU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_E                                  (0x00000210U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_F                                  (0x00000214U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_G                                  (0x00000218U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_H                                  (0x0000021CU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_I                                  (0x00000220U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_J                                  (0x00000224U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_K                                  (0x00000228U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_L                                  (0x0000022CU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_M                                  (0x00000230U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_N                                  (0x00000234U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_O                                  (0x00000238U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_P                                  (0x0000023CU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_A                                  (0x00000240U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_B                                  (0x00000244U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_C                                  (0x00000248U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_D                                  (0x0000024CU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_E                                  (0x00000250U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_F                                  (0x00000254U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_G                                  (0x00000258U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_H                                  (0x0000025CU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_I                                  (0x00000260U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_J                                  (0x00000264U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_K                                  (0x00000268U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_L                                  (0x0000026CU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_M                                  (0x00000270U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_N                                  (0x00000274U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_O                                  (0x00000278U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_P                                  (0x0000027CU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_DIGEST_COUNT                               (0x00000280U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE                                       (0x00000284U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_LENGTH                                     (0x00000288U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* P_ODIGEST_A */

#define CSL_DTHE_V2_SHA_P_P_ODIGEST_A_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_A_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_A_DATA_MAX                                 (0xFFFFFFFFU)


/* P_ODIGEST_B */

#define CSL_DTHE_V2_SHA_P_P_ODIGEST_B_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_B_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_B_DATA_MAX                                 (0xFFFFFFFFU)


/* P_ODIGEST_C */

#define CSL_DTHE_V2_SHA_P_P_ODIGEST_C_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_C_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_C_DATA_MAX                                 (0xFFFFFFFFU)


/* P_ODIGEST_D */

#define CSL_DTHE_V2_SHA_P_P_ODIGEST_D_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_D_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_D_DATA_MAX                                 (0xFFFFFFFFU)


/* P_ODIGEST_E */

#define CSL_DTHE_V2_SHA_P_P_ODIGEST_E_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_E_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_E_DATA_MAX                                 (0xFFFFFFFFU)


/* P_ODIGEST_F */

#define CSL_DTHE_V2_SHA_P_P_ODIGEST_F_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_F_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_F_DATA_MAX                                 (0xFFFFFFFFU)


/* P_ODIGEST_G */

#define CSL_DTHE_V2_SHA_P_P_ODIGEST_G_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_G_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_G_DATA_MAX                                 (0xFFFFFFFFU)


/* P_ODIGEST_H */

#define CSL_DTHE_V2_SHA_P_P_ODIGEST_H_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_H_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_ODIGEST_H_DATA_MAX                                 (0xFFFFFFFFU)


/* P_IDIGEST_A */

#define CSL_DTHE_V2_SHA_P_P_IDIGEST_A_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_A_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_A_DATA_MAX                                 (0xFFFFFFFFU)


/* P_IDIGEST_B */

#define CSL_DTHE_V2_SHA_P_P_IDIGEST_B_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_B_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_B_DATA_MAX                                 (0xFFFFFFFFU)


/* P_IDIGEST_C */

#define CSL_DTHE_V2_SHA_P_P_IDIGEST_C_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_C_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_C_DATA_MAX                                 (0xFFFFFFFFU)


/* P_IDIGEST_D */

#define CSL_DTHE_V2_SHA_P_P_IDIGEST_D_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_D_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_D_DATA_MAX                                 (0xFFFFFFFFU)


/* P_IDIGEST_E */

#define CSL_DTHE_V2_SHA_P_P_IDIGEST_E_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_E_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_E_DATA_MAX                                 (0xFFFFFFFFU)


/* P_IDIGEST_F */

#define CSL_DTHE_V2_SHA_P_P_IDIGEST_F_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_F_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_F_DATA_MAX                                 (0xFFFFFFFFU)


/* P_IDIGEST_G */

#define CSL_DTHE_V2_SHA_P_P_IDIGEST_G_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_G_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_G_DATA_MAX                                 (0xFFFFFFFFU)


/* P_IDIGEST_H */

#define CSL_DTHE_V2_SHA_P_P_IDIGEST_H_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_H_DATA_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IDIGEST_H_DATA_MAX                                 (0xFFFFFFFFU)


/* P_DIGEST_COUNT */

#define CSL_DTHE_V2_SHA_P_P_DIGEST_COUNT_DATA_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DIGEST_COUNT_DATA_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DIGEST_COUNT_DATA_MAX                              (0xFFFFFFFFU)


/* P_MODE */

#define CSL_DTHE_V2_SHA_P_P_MODE_ALGO_MASK                                     (0x00000007U)
#define CSL_DTHE_V2_SHA_P_P_MODE_ALGO_SHIFT                                    (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_MODE_ALGO_MAX                                      (0x00000007U)

#define CSL_DTHE_V2_SHA_P_P_MODE_ALGO_CONSTANT_MASK                            (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_MODE_ALGO_CONSTANT_SHIFT                           (0x00000003U)
#define CSL_DTHE_V2_SHA_P_P_MODE_ALGO_CONSTANT_MAX                             (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_MODE_CLOSE_HASH_MASK                               (0x00000010U)
#define CSL_DTHE_V2_SHA_P_P_MODE_CLOSE_HASH_SHIFT                              (0x00000004U)
#define CSL_DTHE_V2_SHA_P_P_MODE_CLOSE_HASH_MAX                                (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_MODE_HMAC_KEY_PROC_MASK                            (0x00000020U)
#define CSL_DTHE_V2_SHA_P_P_MODE_HMAC_KEY_PROC_SHIFT                           (0x00000005U)
#define CSL_DTHE_V2_SHA_P_P_MODE_HMAC_KEY_PROC_MAX                             (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_MODE_REUSE_HMAC_KEY_MASK                           (0x00000040U)
#define CSL_DTHE_V2_SHA_P_P_MODE_REUSE_HMAC_KEY_SHIFT                          (0x00000006U)
#define CSL_DTHE_V2_SHA_P_P_MODE_REUSE_HMAC_KEY_MAX                            (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_MODE_HMAC_OUTER_HASH_MASK                          (0x00000080U)
#define CSL_DTHE_V2_SHA_P_P_MODE_HMAC_OUTER_HASH_SHIFT                         (0x00000007U)
#define CSL_DTHE_V2_SHA_P_P_MODE_HMAC_OUTER_HASH_MAX                           (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_MODE_RESERVED_MASK                                 (0xFFFFFF00U)
#define CSL_DTHE_V2_SHA_P_P_MODE_RESERVED_SHIFT                                (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_MODE_RESERVED_MAX                                  (0x00FFFFFFU)


/* P_LENGTH */

#define CSL_DTHE_V2_SHA_P_P_LENGTH_DATA_MASK                                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_LENGTH_DATA_SHIFT                                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_LENGTH_DATA_MAX                                    (0xFFFFFFFFU)


/* P_DATA0_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA0_IN_DATA0_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA0_IN_DATA0_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA0_IN_DATA0_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA1_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA1_IN_DATA1_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA1_IN_DATA1_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA1_IN_DATA1_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA2_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA2_IN_DATA2_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA2_IN_DATA2_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA2_IN_DATA2_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA3_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA3_IN_DATA3_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA3_IN_DATA3_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA3_IN_DATA3_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA4_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA4_IN_DATA4_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA4_IN_DATA4_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA4_IN_DATA4_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA5_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA5_IN_DATA5_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA5_IN_DATA5_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA5_IN_DATA5_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA6_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA6_IN_DATA6_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA6_IN_DATA6_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA6_IN_DATA6_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA7_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA7_IN_DATA7_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA7_IN_DATA7_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA7_IN_DATA7_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA8_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA8_IN_DATA8_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA8_IN_DATA8_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA8_IN_DATA8_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA9_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA9_IN_DATA9_IN_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA9_IN_DATA9_IN_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA9_IN_DATA9_IN_MAX                              (0xFFFFFFFFU)


/* P_DATA10_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA10_IN_DATA10_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA10_IN_DATA10_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA10_IN_DATA10_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA11_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA11_IN_DATA11_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA11_IN_DATA11_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA11_IN_DATA11_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA12_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA12_IN_DATA12_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA12_IN_DATA12_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA12_IN_DATA12_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA13_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA13_IN_DATA13_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA13_IN_DATA13_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA13_IN_DATA13_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA14_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA14_IN_DATA14_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA14_IN_DATA14_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA14_IN_DATA14_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA15_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA15_IN_DATA15_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA15_IN_DATA15_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA15_IN_DATA15_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA16_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA16_IN_DATA16_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA16_IN_DATA16_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA16_IN_DATA16_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA17_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA17_IN_DATA17_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA17_IN_DATA17_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA17_IN_DATA17_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA18_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA18_IN_DATA18_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA18_IN_DATA18_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA18_IN_DATA18_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA19_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA19_IN_DATA19_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA19_IN_DATA19_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA19_IN_DATA19_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA20_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA20_IN_DATA20_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA20_IN_DATA20_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA20_IN_DATA20_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA21_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA21_IN_DATA21_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA21_IN_DATA21_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA21_IN_DATA21_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA22_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA22_IN_DATA22_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA22_IN_DATA22_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA22_IN_DATA22_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA23_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA23_IN_DATA23_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA23_IN_DATA23_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA23_IN_DATA23_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA24_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA24_IN_DATA24_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA24_IN_DATA24_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA24_IN_DATA24_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA25_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA25_IN_DATA25_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA25_IN_DATA25_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA25_IN_DATA25_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA26_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA26_IN_DATA26_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA26_IN_DATA26_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA26_IN_DATA26_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA27_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA27_IN_DATA27_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA27_IN_DATA27_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA27_IN_DATA27_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA28_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA28_IN_DATA28_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA28_IN_DATA28_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA28_IN_DATA28_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA29_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA29_IN_DATA29_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA29_IN_DATA29_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA29_IN_DATA29_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA30_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA30_IN_DATA30_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA30_IN_DATA30_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA30_IN_DATA30_IN_MAX                            (0xFFFFFFFFU)


/* P_DATA31_IN */

#define CSL_DTHE_V2_SHA_P_P_DATA31_IN_DATA31_IN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_DATA31_IN_DATA31_IN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_DATA31_IN_DATA31_IN_MAX                            (0xFFFFFFFFU)


/* P_REVISION */

#define CSL_DTHE_V2_SHA_P_P_REVISION_Y_MINOR_MASK                              (0x0000003FU)
#define CSL_DTHE_V2_SHA_P_P_REVISION_Y_MINOR_SHIFT                             (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_Y_MINOR_MAX                               (0x0000003FU)

#define CSL_DTHE_V2_SHA_P_P_REVISION_CUSTOM_MASK                               (0x000000C0U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_CUSTOM_SHIFT                              (0x00000006U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_CUSTOM_MAX                                (0x00000003U)

#define CSL_DTHE_V2_SHA_P_P_REVISION_X_MAJOR_MASK                              (0x00000700U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_X_MAJOR_SHIFT                             (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_X_MAJOR_MAX                               (0x00000007U)

#define CSL_DTHE_V2_SHA_P_P_REVISION_R_RTL_MASK                                (0x0000F800U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_R_RTL_SHIFT                               (0x0000000BU)
#define CSL_DTHE_V2_SHA_P_P_REVISION_R_RTL_MAX                                 (0x0000001FU)

#define CSL_DTHE_V2_SHA_P_P_REVISION_FUNC_MASK                                 (0x0FFF0000U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_FUNC_SHIFT                                (0x00000010U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_FUNC_MAX                                  (0x00000FFFU)

#define CSL_DTHE_V2_SHA_P_P_REVISION_RESERVED1_MASK                            (0x30000000U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_RESERVED1_SHIFT                           (0x0000001CU)
#define CSL_DTHE_V2_SHA_P_P_REVISION_RESERVED1_MAX                             (0x00000003U)

#define CSL_DTHE_V2_SHA_P_P_REVISION_SCHEME_MASK                               (0xC0000000U)
#define CSL_DTHE_V2_SHA_P_P_REVISION_SCHEME_SHIFT                              (0x0000001EU)
#define CSL_DTHE_V2_SHA_P_P_REVISION_SCHEME_MAX                                (0x00000003U)


/* P_SYSCONFIG */

#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_RESERVED2_MASK                           (0x00000003U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_RESERVED2_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_RESERVED2_MAX                            (0x00000003U)

#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PIT_EN_MASK                              (0x00000004U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PIT_EN_SHIFT                             (0x00000002U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PIT_EN_MAX                               (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PDMA_EN_MASK                             (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PDMA_EN_SHIFT                            (0x00000003U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PDMA_EN_MAX                              (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_RESERVED1_MASK                           (0x00000030U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_RESERVED1_SHIFT                          (0x00000004U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_RESERVED1_MAX                            (0x00000003U)

#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PCONT_SWT_MASK                           (0x00000040U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PCONT_SWT_SHIFT                          (0x00000006U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PCONT_SWT_MAX                            (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PADVANCED_MASK                           (0x00000080U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PADVANCED_SHIFT                          (0x00000007U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_PADVANCED_MAX                            (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_RESERVED_MASK                            (0xFFFFFF00U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_RESERVED_SHIFT                           (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_SYSCONFIG_RESERVED_MAX                             (0x00FFFFFFU)


/* P_SYSSTATUS */

#define CSL_DTHE_V2_SHA_P_P_SYSSTATUS_RESETDONE_MASK                           (0x00000001U)
#define CSL_DTHE_V2_SHA_P_P_SYSSTATUS_RESETDONE_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_SYSSTATUS_RESETDONE_MAX                            (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_SYSSTATUS_RESERVED_MASK                            (0xFFFFFFFEU)
#define CSL_DTHE_V2_SHA_P_P_SYSSTATUS_RESERVED_SHIFT                           (0x00000001U)
#define CSL_DTHE_V2_SHA_P_P_SYSSTATUS_RESERVED_MAX                             (0x7FFFFFFFU)


/* P_IRQSTATUS */

#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_OUTPUT_READY_MASK                        (0x00000001U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_OUTPUT_READY_SHIFT                       (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_OUTPUT_READY_MAX                         (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_INPUT_READY_MASK                         (0x00000002U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_INPUT_READY_SHIFT                        (0x00000001U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_INPUT_READY_MAX                          (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_PARTHASH_READY_MASK                      (0x00000004U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_PARTHASH_READY_SHIFT                     (0x00000002U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_PARTHASH_READY_MAX                       (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_CONTEXT_READY_MASK                       (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_CONTEXT_READY_SHIFT                      (0x00000003U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_CONTEXT_READY_MAX                        (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_RESERVED_MASK                            (0xFFFFFFF0U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_RESERVED_SHIFT                           (0x00000004U)
#define CSL_DTHE_V2_SHA_P_P_IRQSTATUS_RESERVED_MAX                             (0x0FFFFFFFU)


/* P_IRQENABLE */

#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_OUTPUT_READY_MASK                      (0x00000001U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_OUTPUT_READY_SHIFT                     (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_OUTPUT_READY_MAX                       (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_INPUT_READY_MASK                       (0x00000002U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_INPUT_READY_SHIFT                      (0x00000001U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_INPUT_READY_MAX                        (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_PARTHASH_READY_MASK                    (0x00000004U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_PARTHASH_READY_SHIFT                   (0x00000002U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_PARTHASH_READY_MAX                     (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_CONTEXT_READY_MASK                     (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_CONTEXT_READY_SHIFT                    (0x00000003U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_M_CONTEXT_READY_MAX                      (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_RESERVED_MASK                            (0xFFFFFFF0U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_RESERVED_SHIFT                           (0x00000004U)
#define CSL_DTHE_V2_SHA_P_P_IRQENABLE_RESERVED_MAX                             (0x0FFFFFFFU)


/* P_HASH512_ODIGEST_A */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_A_ODIGEST_A_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_A_ODIGEST_A_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_A_ODIGEST_A_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_B */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_B_ODIGEST_B_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_B_ODIGEST_B_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_B_ODIGEST_B_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_C */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_C_ODIGEST_C_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_C_ODIGEST_C_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_C_ODIGEST_C_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_D */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_D_ODIGEST_D_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_D_ODIGEST_D_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_D_ODIGEST_D_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_E */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_E_ODIGEST_E_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_E_ODIGEST_E_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_E_ODIGEST_E_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_F */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_F_ODIGEST_F_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_F_ODIGEST_F_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_F_ODIGEST_F_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_G */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_G_ODIGEST_G_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_G_ODIGEST_G_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_G_ODIGEST_G_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_H */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_H_ODIGEST_H_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_H_ODIGEST_H_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_H_ODIGEST_H_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_I */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_I_ODIGEST_I_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_I_ODIGEST_I_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_I_ODIGEST_I_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_J */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_J_ODIGEST_J_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_J_ODIGEST_J_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_J_ODIGEST_J_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_K */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_K_ODIGEST_K_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_K_ODIGEST_K_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_K_ODIGEST_K_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_L */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_L_ODIGEST_L_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_L_ODIGEST_L_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_L_ODIGEST_L_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_M */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_M_ODIGEST_M_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_M_ODIGEST_M_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_M_ODIGEST_M_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_N */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_N_ODIGEST_N_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_N_ODIGEST_N_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_N_ODIGEST_N_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_O */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_O_ODIGEST_O_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_O_ODIGEST_O_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_O_ODIGEST_O_MAX                    (0xFFFFFFFFU)


/* P_HASH512_ODIGEST_P */

#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_P_ODIGEST_P_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_P_ODIGEST_P_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_ODIGEST_P_ODIGEST_P_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_A */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_A_IDIGEST_A_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_A_IDIGEST_A_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_A_IDIGEST_A_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_B */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_B_IDIGEST_B_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_B_IDIGEST_B_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_B_IDIGEST_B_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_C */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_C_IDIGEST_C_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_C_IDIGEST_C_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_C_IDIGEST_C_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_D */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_D_IDIGEST_D_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_D_IDIGEST_D_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_D_IDIGEST_D_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_E */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_E_IDIGEST_E_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_E_IDIGEST_E_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_E_IDIGEST_E_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_F */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_F_IDIGEST_F_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_F_IDIGEST_F_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_F_IDIGEST_F_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_G */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_G_IDIGEST_G_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_G_IDIGEST_G_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_G_IDIGEST_G_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_H */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_H_IDIGEST_H_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_H_IDIGEST_H_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_H_IDIGEST_H_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_I */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_I_IDIGEST_I_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_I_IDIGEST_I_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_I_IDIGEST_I_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_J */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_J_IDIGEST_J_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_J_IDIGEST_J_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_J_IDIGEST_J_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_K */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_K_IDIGEST_K_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_K_IDIGEST_K_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_K_IDIGEST_K_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_L */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_L_IDIGEST_L_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_L_IDIGEST_L_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_L_IDIGEST_L_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_M */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_M_IDIGEST_M_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_M_IDIGEST_M_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_M_IDIGEST_M_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_N */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_N_IDIGEST_N_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_N_IDIGEST_N_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_N_IDIGEST_N_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_O */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_O_IDIGEST_O_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_O_IDIGEST_O_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_O_IDIGEST_O_MAX                    (0xFFFFFFFFU)


/* P_HASH512_IDIGEST_P */

#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_P_IDIGEST_P_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_P_IDIGEST_P_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_IDIGEST_P_IDIGEST_P_MAX                    (0xFFFFFFFFU)


/* P_HASH512_DIGEST_COUNT */

#define CSL_DTHE_V2_SHA_P_P_HASH512_DIGEST_COUNT_DIGEST_COUNT_MASK             (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_DIGEST_COUNT_DIGEST_COUNT_SHIFT            (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_DIGEST_COUNT_DIGEST_COUNT_MAX              (0xFFFFFFFFU)


/* P_HASH512_MODE */

#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_ALGORITHM_MASK                        (0x00000007U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_ALGORITHM_SHIFT                       (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_ALGORITHM_MAX                         (0x00000007U)

#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_USE_ALG_CONSTANTS_MASK                (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_USE_ALG_CONSTANTS_SHIFT               (0x00000003U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_USE_ALG_CONSTANTS_MAX                 (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_CLOSE_HASH_MASK                       (0x00000010U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_CLOSE_HASH_SHIFT                      (0x00000004U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_CLOSE_HASH_MAX                        (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_HMAC_KEY_PROCESSING_MASK              (0x00000020U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_HMAC_KEY_PROCESSING_SHIFT             (0x00000005U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_HMAC_KEY_PROCESSING_MAX               (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_REUSE_HMAC_KEY_MASK                   (0x00000040U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_REUSE_HMAC_KEY_SHIFT                  (0x00000006U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_REUSE_HMAC_KEY_MAX                    (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_HMAC_OUTER_HASH_MASK                  (0x00000080U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_HMAC_OUTER_HASH_SHIFT                 (0x00000007U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_HMAC_OUTER_HASH_MAX                   (0x00000001U)

#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_RESERVED_MASK                         (0xFFFFFF00U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_RESERVED_SHIFT                        (0x00000008U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_MODE_RESERVED_MAX                          (0x00FFFFFFU)


/* P_HASH512_LENGTH */

#define CSL_DTHE_V2_SHA_P_P_HASH512_LENGTH_LENGTH_MASK                         (0xFFFFFFFFU)
#define CSL_DTHE_V2_SHA_P_P_HASH512_LENGTH_LENGTH_SHIFT                        (0x00000000U)
#define CSL_DTHE_V2_SHA_P_P_HASH512_LENGTH_LENGTH_MAX                          (0xFFFFFFFFU)


#ifdef __cplusplus
}
#endif
#endif
