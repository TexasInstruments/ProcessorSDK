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
 *  Name        : cslr_dthe_v2_aes.h
 *  VPVERSION   : 3.0.367 - 2024.03.02.21.42.22
 *  VPREV       : 1.0.8
*/
#ifndef CSLR_DTHE_V2_AES_H_
#define CSLR_DTHE_V2_AES_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  : AES registers in secure context
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t S_KEY2_6;
    volatile uint32_t S_KEY2_7;
    volatile uint32_t S_KEY2_4;
    volatile uint32_t S_KEY2_5;
    volatile uint32_t S_KEY2_2;
    volatile uint32_t S_KEY2_3;
    volatile uint32_t S_KEY2_0;
    volatile uint32_t S_KEY2_1;
    volatile uint32_t S_KEY1_6;
    volatile uint32_t S_KEY1_7;
    volatile uint32_t S_KEY1_4;
    volatile uint32_t S_KEY1_5;
    volatile uint32_t S_KEY1_2;
    volatile uint32_t S_KEY1_3;
    volatile uint32_t S_KEY1_0;
    volatile uint32_t S_KEY1_1;
    volatile uint32_t S_IV_IN_0;
    volatile uint32_t S_IV_IN_1;
    volatile uint32_t S_IV_IN_2;
    volatile uint32_t S_IV_IN_3;
    volatile uint32_t S_CTRL;
    volatile uint32_t S_C_LENGTH_0;
    volatile uint32_t S_C_LENGTH_1;
    volatile uint32_t S_AUTH_LENGTH;
    volatile uint32_t S_DATA_IN_OUT_0;
    volatile uint32_t S_DATA_IN_OUT_1;
    volatile uint32_t S_DATA_IN_OUT_2;
    volatile uint32_t S_DATA_IN_OUT_3;
    volatile uint32_t S_TAG_OUT_0;
    volatile uint32_t S_TAG_OUT_1;
    volatile uint32_t S_TAG_OUT_2;
    volatile uint32_t S_TAG_OUT_3;
    volatile uint32_t S_REVISION;
    volatile uint32_t S_SYSCONFIG;
    volatile uint32_t S_SYSSTATUS;
    volatile uint32_t S_IRQSTATUS;
    volatile uint32_t S_IRQENABLE;
    volatile uint32_t S_DIRTYBITS;
    volatile uint32_t S_LOCKDOWN;
    volatile uint8_t  Resv_164[8];
    volatile uint32_t S_X_CONFIG;
} CSL_dthe_v2_aes_sRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DTHE_V2_AES_S_S_KEY2_6                                             (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY2_7                                             (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_KEY2_4                                             (0x00000008U)
#define CSL_DTHE_V2_AES_S_S_KEY2_5                                             (0x0000000CU)
#define CSL_DTHE_V2_AES_S_S_KEY2_2                                             (0x00000010U)
#define CSL_DTHE_V2_AES_S_S_KEY2_3                                             (0x00000014U)
#define CSL_DTHE_V2_AES_S_S_KEY2_0                                             (0x00000018U)
#define CSL_DTHE_V2_AES_S_S_KEY2_1                                             (0x0000001CU)
#define CSL_DTHE_V2_AES_S_S_KEY1_6                                             (0x00000020U)
#define CSL_DTHE_V2_AES_S_S_KEY1_7                                             (0x00000024U)
#define CSL_DTHE_V2_AES_S_S_KEY1_4                                             (0x00000028U)
#define CSL_DTHE_V2_AES_S_S_KEY1_5                                             (0x0000002CU)
#define CSL_DTHE_V2_AES_S_S_KEY1_2                                             (0x00000030U)
#define CSL_DTHE_V2_AES_S_S_KEY1_3                                             (0x00000034U)
#define CSL_DTHE_V2_AES_S_S_KEY1_0                                             (0x00000038U)
#define CSL_DTHE_V2_AES_S_S_KEY1_1                                             (0x0000003CU)
#define CSL_DTHE_V2_AES_S_S_IV_IN_0                                            (0x00000040U)
#define CSL_DTHE_V2_AES_S_S_IV_IN_1                                            (0x00000044U)
#define CSL_DTHE_V2_AES_S_S_IV_IN_2                                            (0x00000048U)
#define CSL_DTHE_V2_AES_S_S_IV_IN_3                                            (0x0000004CU)
#define CSL_DTHE_V2_AES_S_S_CTRL                                               (0x00000050U)
#define CSL_DTHE_V2_AES_S_S_C_LENGTH_0                                         (0x00000054U)
#define CSL_DTHE_V2_AES_S_S_C_LENGTH_1                                         (0x00000058U)
#define CSL_DTHE_V2_AES_S_S_AUTH_LENGTH                                        (0x0000005CU)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_0                                      (0x00000060U)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_1                                      (0x00000064U)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_2                                      (0x00000068U)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_3                                      (0x0000006CU)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_0                                          (0x00000070U)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_1                                          (0x00000074U)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_2                                          (0x00000078U)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_3                                          (0x0000007CU)
#define CSL_DTHE_V2_AES_S_S_REVISION                                           (0x00000080U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG                                          (0x00000084U)
#define CSL_DTHE_V2_AES_S_S_SYSSTATUS                                          (0x00000088U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS                                          (0x0000008CU)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE                                          (0x00000090U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS                                          (0x00000094U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN                                           (0x00000098U)
#define CSL_DTHE_V2_AES_S_S_X_CONFIG                                           (0x000000A4U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* S_KEY2_6 */

#define CSL_DTHE_V2_AES_S_S_KEY2_6_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY2_6_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY2_6_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY2_7 */

#define CSL_DTHE_V2_AES_S_S_KEY2_7_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY2_7_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY2_7_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY2_4 */

#define CSL_DTHE_V2_AES_S_S_KEY2_4_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY2_4_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY2_4_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY2_5 */

#define CSL_DTHE_V2_AES_S_S_KEY2_5_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY2_5_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY2_5_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY2_2 */

#define CSL_DTHE_V2_AES_S_S_KEY2_2_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY2_2_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY2_2_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY2_3 */

#define CSL_DTHE_V2_AES_S_S_KEY2_3_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY2_3_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY2_3_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY2_0 */

#define CSL_DTHE_V2_AES_S_S_KEY2_0_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY2_0_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY2_0_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY2_1 */

#define CSL_DTHE_V2_AES_S_S_KEY2_1_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY2_1_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY2_1_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY1_6 */

#define CSL_DTHE_V2_AES_S_S_KEY1_6_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY1_6_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY1_6_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY1_7 */

#define CSL_DTHE_V2_AES_S_S_KEY1_7_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY1_7_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY1_7_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY1_4 */

#define CSL_DTHE_V2_AES_S_S_KEY1_4_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY1_4_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY1_4_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY1_5 */

#define CSL_DTHE_V2_AES_S_S_KEY1_5_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY1_5_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY1_5_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY1_2 */

#define CSL_DTHE_V2_AES_S_S_KEY1_2_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY1_2_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY1_2_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY1_3 */

#define CSL_DTHE_V2_AES_S_S_KEY1_3_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY1_3_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY1_3_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY1_0 */

#define CSL_DTHE_V2_AES_S_S_KEY1_0_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY1_0_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY1_0_KEY_MAX                                     (0xFFFFFFFFU)


/* S_KEY1_1 */

#define CSL_DTHE_V2_AES_S_S_KEY1_1_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_KEY1_1_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_KEY1_1_KEY_MAX                                     (0xFFFFFFFFU)


/* S_IV_IN_0 */

#define CSL_DTHE_V2_AES_S_S_IV_IN_0_DATA_MASK                                  (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_IV_IN_0_DATA_SHIFT                                 (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_IV_IN_0_DATA_MAX                                   (0xFFFFFFFFU)


/* S_IV_IN_1 */

#define CSL_DTHE_V2_AES_S_S_IV_IN_1_DATA_MASK                                  (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_IV_IN_1_DATA_SHIFT                                 (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_IV_IN_1_DATA_MAX                                   (0xFFFFFFFFU)


/* S_IV_IN_2 */

#define CSL_DTHE_V2_AES_S_S_IV_IN_2_DATA_MASK                                  (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_IV_IN_2_DATA_SHIFT                                 (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_IV_IN_2_DATA_MAX                                   (0xFFFFFFFFU)


/* S_IV_IN_3 */

#define CSL_DTHE_V2_AES_S_S_IV_IN_3_DATA_MASK                                  (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_IV_IN_3_DATA_SHIFT                                 (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_IV_IN_3_DATA_MAX                                   (0xFFFFFFFFU)


/* S_CTRL */

#define CSL_DTHE_V2_AES_S_S_CTRL_OUTPUT_READY_MASK                             (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_CTRL_OUTPUT_READY_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_OUTPUT_READY_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_INPUT_READY_MASK                              (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_CTRL_INPUT_READY_SHIFT                             (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_CTRL_INPUT_READY_MAX                               (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_DIRECTION_MASK                                (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_CTRL_DIRECTION_SHIFT                               (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_CTRL_DIRECTION_MAX                                 (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_KEY_SIZE_MASK                                 (0x00000018U)
#define CSL_DTHE_V2_AES_S_S_CTRL_KEY_SIZE_SHIFT                                (0x00000003U)
#define CSL_DTHE_V2_AES_S_S_CTRL_KEY_SIZE_MAX                                  (0x00000003U)

#define CSL_DTHE_V2_AES_S_S_CTRL_MODE_MASK                                     (0x00000020U)
#define CSL_DTHE_V2_AES_S_S_CTRL_MODE_SHIFT                                    (0x00000005U)
#define CSL_DTHE_V2_AES_S_S_CTRL_MODE_MAX                                      (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_CTR_MASK                                      (0x00000040U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CTR_SHIFT                                     (0x00000006U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CTR_MAX                                       (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_CTR_WIDTH_MASK                                (0x00000180U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CTR_WIDTH_SHIFT                               (0x00000007U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CTR_WIDTH_MAX                                 (0x00000003U)

#define CSL_DTHE_V2_AES_S_S_CTRL_ICM_MASK                                      (0x00000200U)
#define CSL_DTHE_V2_AES_S_S_CTRL_ICM_SHIFT                                     (0x00000009U)
#define CSL_DTHE_V2_AES_S_S_CTRL_ICM_MAX                                       (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_CFB_MASK                                      (0x00000400U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CFB_SHIFT                                     (0x0000000AU)
#define CSL_DTHE_V2_AES_S_S_CTRL_CFB_MAX                                       (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_XTS_MASK                                      (0x00001800U)
#define CSL_DTHE_V2_AES_S_S_CTRL_XTS_SHIFT                                     (0x0000000BU)
#define CSL_DTHE_V2_AES_S_S_CTRL_XTS_MAX                                       (0x00000003U)

#define CSL_DTHE_V2_AES_S_S_CTRL_F8_MASK                                       (0x00002000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_F8_SHIFT                                      (0x0000000DU)
#define CSL_DTHE_V2_AES_S_S_CTRL_F8_MAX                                        (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_F9_MASK                                       (0x00004000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_F9_SHIFT                                      (0x0000000EU)
#define CSL_DTHE_V2_AES_S_S_CTRL_F9_MAX                                        (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_CBCMAC_MASK                                   (0x00008000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CBCMAC_SHIFT                                  (0x0000000FU)
#define CSL_DTHE_V2_AES_S_S_CTRL_CBCMAC_MAX                                    (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_GCM_MASK                                      (0x00030000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_GCM_SHIFT                                     (0x00000010U)
#define CSL_DTHE_V2_AES_S_S_CTRL_GCM_MAX                                       (0x00000003U)

#define CSL_DTHE_V2_AES_S_S_CTRL_CCM_MASK                                      (0x00040000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CCM_SHIFT                                     (0x00000012U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CCM_MAX                                       (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_CCM_L_MASK                                    (0x00380000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CCM_L_SHIFT                                   (0x00000013U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CCM_L_MAX                                     (0x00000007U)

#define CSL_DTHE_V2_AES_S_S_CTRL_CCM_M_MASK                                    (0x01C00000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CCM_M_SHIFT                                   (0x00000016U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CCM_M_MAX                                     (0x00000007U)

#define CSL_DTHE_V2_AES_S_S_CTRL_RESERVED_MASK                                 (0x1E000000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_RESERVED_SHIFT                                (0x00000019U)
#define CSL_DTHE_V2_AES_S_S_CTRL_RESERVED_MAX                                  (0x0000000FU)

#define CSL_DTHE_V2_AES_S_S_CTRL_SAVE_CONTEXT_MASK                             (0x20000000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_SAVE_CONTEXT_SHIFT                            (0x0000001DU)
#define CSL_DTHE_V2_AES_S_S_CTRL_SAVE_CONTEXT_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_SAVE_CONTEXT_READY_MASK                       (0x40000000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_SAVE_CONTEXT_READY_SHIFT                      (0x0000001EU)
#define CSL_DTHE_V2_AES_S_S_CTRL_SAVE_CONTEXT_READY_MAX                        (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_CTRL_CONTEXT_READY_MASK                            (0x80000000U)
#define CSL_DTHE_V2_AES_S_S_CTRL_CONTEXT_READY_SHIFT                           (0x0000001FU)
#define CSL_DTHE_V2_AES_S_S_CTRL_CONTEXT_READY_MAX                             (0x00000001U)


/* S_C_LENGTH_0 */

#define CSL_DTHE_V2_AES_S_S_C_LENGTH_0_LENGTH_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_C_LENGTH_0_LENGTH_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_C_LENGTH_0_LENGTH_MAX                              (0xFFFFFFFFU)


/* S_C_LENGTH_1 */

#define CSL_DTHE_V2_AES_S_S_C_LENGTH_1_LENGTH_MASK                             (0x1FFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_C_LENGTH_1_LENGTH_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_C_LENGTH_1_LENGTH_MAX                              (0x1FFFFFFFU)

#define CSL_DTHE_V2_AES_S_S_C_LENGTH_1_RESERVED_MASK                           (0xE0000000U)
#define CSL_DTHE_V2_AES_S_S_C_LENGTH_1_RESERVED_SHIFT                          (0x0000001DU)
#define CSL_DTHE_V2_AES_S_S_C_LENGTH_1_RESERVED_MAX                            (0x00000007U)


/* S_AUTH_LENGTH */

#define CSL_DTHE_V2_AES_S_S_AUTH_LENGTH_AUTH_MASK                              (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_AUTH_LENGTH_AUTH_SHIFT                             (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_AUTH_LENGTH_AUTH_MAX                               (0xFFFFFFFFU)


/* S_DATA_IN_OUT_0 */

#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_0_DATA_MASK                            (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_0_DATA_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_0_DATA_MAX                             (0xFFFFFFFFU)


/* S_DATA_IN_OUT_1 */

#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_1_DATA_MASK                            (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_1_DATA_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_1_DATA_MAX                             (0xFFFFFFFFU)


/* S_DATA_IN_OUT_2 */

#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_2_DATA_MASK                            (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_2_DATA_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_2_DATA_MAX                             (0xFFFFFFFFU)


/* S_DATA_IN_OUT_3 */

#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_3_DATA_MASK                            (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_3_DATA_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_DATA_IN_OUT_3_DATA_MAX                             (0xFFFFFFFFU)


/* S_TAG_OUT_0 */

#define CSL_DTHE_V2_AES_S_S_TAG_OUT_0_HASH_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_0_HASH_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_0_HASH_MAX                                 (0xFFFFFFFFU)


/* S_TAG_OUT_1 */

#define CSL_DTHE_V2_AES_S_S_TAG_OUT_1_HASH_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_1_HASH_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_1_HASH_MAX                                 (0xFFFFFFFFU)


/* S_TAG_OUT_2 */

#define CSL_DTHE_V2_AES_S_S_TAG_OUT_2_HASH_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_2_HASH_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_2_HASH_MAX                                 (0xFFFFFFFFU)


/* S_TAG_OUT_3 */

#define CSL_DTHE_V2_AES_S_S_TAG_OUT_3_HASH_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_3_HASH_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_TAG_OUT_3_HASH_MAX                                 (0xFFFFFFFFU)


/* S_REVISION */

#define CSL_DTHE_V2_AES_S_S_REVISION_Y_MINOR_MASK                              (0x0000003FU)
#define CSL_DTHE_V2_AES_S_S_REVISION_Y_MINOR_SHIFT                             (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_REVISION_Y_MINOR_MAX                               (0x0000003FU)

#define CSL_DTHE_V2_AES_S_S_REVISION_CUSTOM_MASK                               (0x000000C0U)
#define CSL_DTHE_V2_AES_S_S_REVISION_CUSTOM_SHIFT                              (0x00000006U)
#define CSL_DTHE_V2_AES_S_S_REVISION_CUSTOM_MAX                                (0x00000003U)

#define CSL_DTHE_V2_AES_S_S_REVISION_X_MAJOR_MASK                              (0x00000700U)
#define CSL_DTHE_V2_AES_S_S_REVISION_X_MAJOR_SHIFT                             (0x00000008U)
#define CSL_DTHE_V2_AES_S_S_REVISION_X_MAJOR_MAX                               (0x00000007U)

#define CSL_DTHE_V2_AES_S_S_REVISION_R_RTL_MASK                                (0x0000F800U)
#define CSL_DTHE_V2_AES_S_S_REVISION_R_RTL_SHIFT                               (0x0000000BU)
#define CSL_DTHE_V2_AES_S_S_REVISION_R_RTL_MAX                                 (0x0000001FU)

#define CSL_DTHE_V2_AES_S_S_REVISION_FUNC_MASK                                 (0x0FFF0000U)
#define CSL_DTHE_V2_AES_S_S_REVISION_FUNC_SHIFT                                (0x00000010U)
#define CSL_DTHE_V2_AES_S_S_REVISION_FUNC_MAX                                  (0x00000FFFU)

#define CSL_DTHE_V2_AES_S_S_REVISION_RESERVED_MASK                             (0x30000000U)
#define CSL_DTHE_V2_AES_S_S_REVISION_RESERVED_SHIFT                            (0x0000001CU)
#define CSL_DTHE_V2_AES_S_S_REVISION_RESERVED_MAX                              (0x00000003U)

#define CSL_DTHE_V2_AES_S_S_REVISION_SCHEME_MASK                               (0xC0000000U)
#define CSL_DTHE_V2_AES_S_S_REVISION_SCHEME_SHIFT                              (0x0000001EU)
#define CSL_DTHE_V2_AES_S_S_REVISION_SCHEME_MAX                                (0x00000003U)


/* S_SYSCONFIG */

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_AUTOIDLE_MASK                            (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_AUTOIDLE_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_AUTOIDLE_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_SOFTRESET_MASK                           (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_SOFTRESET_SHIFT                          (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_SOFTRESET_MAX                            (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_SIDLE_MASK                               (0x0000000CU)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_SIDLE_SHIFT                              (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_SIDLE_MAX                                (0x00000003U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DIRECTBUSEN_MASK                         (0x00000010U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DIRECTBUSEN_SHIFT                        (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DIRECTBUSEN_MAX                          (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_DATA_IN_EN_MASK                  (0x00000020U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_DATA_IN_EN_SHIFT                 (0x00000005U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_DATA_IN_EN_MAX                   (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_DATA_OUT_EN_MASK                 (0x00000040U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_DATA_OUT_EN_SHIFT                (0x00000006U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_DATA_OUT_EN_MAX                  (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_MASK               (0x00000080U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_SHIFT              (0x00000007U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_MAX                (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_MASK              (0x00000100U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_SHIFT             (0x00000008U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_MAX               (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_MASK         (0x00000200U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_SHIFT        (0x00000009U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_MAX          (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEK_MODE_MASK                            (0x00000400U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEK_MODE_SHIFT                           (0x0000000AU)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEK_MODE_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEY_ENC_MASK                             (0x00000800U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEY_ENC_SHIFT                            (0x0000000BU)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEY_ENC_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_K3_MASK                                  (0x00001000U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_K3_SHIFT                                 (0x0000000CU)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_K3_MAX                                   (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEK_MODE_ID_MASK                         (0x00004000U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEK_MODE_ID_SHIFT                        (0x0000000EU)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEK_MODE_ID_MAX                          (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEK_MODE_LEN_MASK                        (0x00018000U)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEK_MODE_LEN_SHIFT                       (0x0000000FU)
#define CSL_DTHE_V2_AES_S_S_SYSCONFIG_KEK_MODE_LEN_MAX                         (0x00000003U)


/* S_SYSSTATUS */

#define CSL_DTHE_V2_AES_S_S_SYSSTATUS_RESETDONE_MASK                           (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_SYSSTATUS_RESETDONE_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_SYSSTATUS_RESETDONE_MAX                            (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_SYSSTATUS_RESERVED_MASK                            (0xFFFFFFFEU)
#define CSL_DTHE_V2_AES_S_S_SYSSTATUS_RESERVED_SHIFT                           (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_SYSSTATUS_RESERVED_MAX                             (0x7FFFFFFFU)


/* S_IRQSTATUS */

#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_CONTEXT_IN_MASK                          (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_CONTEXT_IN_SHIFT                         (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_CONTEXT_IN_MAX                           (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_DATA_IN_MASK                             (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_DATA_IN_SHIFT                            (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_DATA_IN_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_DATA_OUT_MASK                            (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_DATA_OUT_SHIFT                           (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_DATA_OUT_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_CONTEXT_OUT_MASK                         (0x00000008U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_CONTEXT_OUT_SHIFT                        (0x00000003U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_CONTEXT_OUT_MAX                          (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_RESERVED_MASK                            (0xFFFFFFF0U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_RESERVED_SHIFT                           (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_IRQSTATUS_RESERVED_MAX                             (0x0FFFFFFFU)


/* S_IRQENABLE */

#define CSL_DTHE_V2_AES_S_S_IRQENABLE_CONTEXT_IN_MASK                          (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_CONTEXT_IN_SHIFT                         (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_CONTEXT_IN_MAX                           (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_IRQENABLE_DATA_IN_MASK                             (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_DATA_IN_SHIFT                            (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_DATA_IN_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_IRQENABLE_DATA_OUT_MASK                            (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_DATA_OUT_SHIFT                           (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_DATA_OUT_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_IRQENABLE_CONTEXT_OUT_MASK                         (0x00000008U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_CONTEXT_OUT_SHIFT                        (0x00000003U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_CONTEXT_OUT_MAX                          (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_IRQENABLE_RESERVED_MASK                            (0xFFFFFFF0U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_RESERVED_SHIFT                           (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_IRQENABLE_RESERVED_MAX                             (0x0FFFFFFFU)


/* S_DIRTYBITS */

#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_S_ACCESS_MASK                            (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_S_ACCESS_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_S_ACCESS_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_S_DIRTY_MASK                             (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_S_DIRTY_SHIFT                            (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_S_DIRTY_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_P_ACCESS_MASK                            (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_P_ACCESS_SHIFT                           (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_P_ACCESS_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_P_DIRTY_MASK                             (0x00000008U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_P_DIRTY_SHIFT                            (0x00000003U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_P_DIRTY_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_RESERVED_MASK                            (0xFFFFFFF0U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_RESERVED_SHIFT                           (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_DIRTYBITS_RESERVED_MAX                             (0x0FFFFFFFU)


/* S_LOCKDOWN */

#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_KEY_LOCK_MASK                             (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_KEY_LOCK_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_KEY_LOCK_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_KEY2_LOCK_MASK                            (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_KEY2_LOCK_SHIFT                           (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_KEY2_LOCK_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_KEY3_LOCK_MASK                            (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_KEY3_LOCK_SHIFT                           (0x00000002U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_KEY3_LOCK_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_IV_LOCK_MASK                              (0x00000008U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_IV_LOCK_SHIFT                             (0x00000003U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_IV_LOCK_MAX                               (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_CONTROL_LOCK_MASK                         (0x00000010U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_CONTROL_LOCK_SHIFT                        (0x00000004U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_CONTROL_LOCK_MAX                          (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_LENGTH_LOCK_MASK                          (0x00000020U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_LENGTH_LOCK_SHIFT                         (0x00000005U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_LENGTH_LOCK_MAX                           (0x00000001U)

#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_RESERVED_MASK                             (0xFFFFFFC0U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_RESERVED_SHIFT                            (0x00000006U)
#define CSL_DTHE_V2_AES_S_S_LOCKDOWN_RESERVED_MAX                              (0x03FFFFFFU)


/* S_X_CONFIG */

#define CSL_DTHE_V2_AES_S_S_X_CONFIG_SPEED_UP_MASK                             (0x00000001U)
#define CSL_DTHE_V2_AES_S_S_X_CONFIG_SPEED_UP_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_AES_S_S_X_CONFIG_SPEED_UP_MAX                              (0x00000001U)


/**************************************************************************
* Hardware Region  : AES registers in public context
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t P_KEY2_6;
    volatile uint32_t P_KEY2_7;
    volatile uint32_t P_KEY2_4;
    volatile uint32_t P_KEY2_5;
    volatile uint32_t P_KEY2_2;
    volatile uint32_t P_KEY2_3;
    volatile uint32_t P_KEY2_0;
    volatile uint32_t P_KEY2_1;
    volatile uint32_t P_KEY1_6;
    volatile uint32_t P_KEY1_7;
    volatile uint32_t P_KEY1_4;
    volatile uint32_t P_KEY1_5;
    volatile uint32_t P_KEY1_2;
    volatile uint32_t P_KEY1_3;
    volatile uint32_t P_KEY1_0;
    volatile uint32_t P_KEY1_1;
    volatile uint32_t P_IV_IN_0;
    volatile uint32_t P_IV_IN_1;
    volatile uint32_t P_IV_IN_2;
    volatile uint32_t P_IV_IN_3;
    volatile uint32_t P_CTRL;
    volatile uint32_t P_C_LENGTH_0;
    volatile uint32_t P_C_LENGTH_1;
    volatile uint32_t P_AUTH_LENGTH;
    volatile uint32_t P_DATA_IN_OUT_0;
    volatile uint32_t P_DATA_IN_OUT_1;
    volatile uint32_t P_DATA_IN_OUT_2;
    volatile uint32_t P_DATA_IN_OUT_3;
    volatile uint32_t P_TAG_OUT_0;
    volatile uint32_t P_TAG_OUT_1;
    volatile uint32_t P_TAG_OUT_2;
    volatile uint32_t P_TAG_OUT_3;
    volatile uint32_t P_REVISION;
    volatile uint32_t P_SYSCONFIG;
    volatile uint32_t P_SYSSTATUS;
    volatile uint32_t P_IRQSTATUS;
    volatile uint32_t P_IRQENABLE;
} CSL_dthe_v2_aes_pRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DTHE_V2_AES_P_P_KEY2_6                                             (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY2_7                                             (0x00000004U)
#define CSL_DTHE_V2_AES_P_P_KEY2_4                                             (0x00000008U)
#define CSL_DTHE_V2_AES_P_P_KEY2_5                                             (0x0000000CU)
#define CSL_DTHE_V2_AES_P_P_KEY2_2                                             (0x00000010U)
#define CSL_DTHE_V2_AES_P_P_KEY2_3                                             (0x00000014U)
#define CSL_DTHE_V2_AES_P_P_KEY2_0                                             (0x00000018U)
#define CSL_DTHE_V2_AES_P_P_KEY2_1                                             (0x0000001CU)
#define CSL_DTHE_V2_AES_P_P_KEY1_6                                             (0x00000020U)
#define CSL_DTHE_V2_AES_P_P_KEY1_7                                             (0x00000024U)
#define CSL_DTHE_V2_AES_P_P_KEY1_4                                             (0x00000028U)
#define CSL_DTHE_V2_AES_P_P_KEY1_5                                             (0x0000002CU)
#define CSL_DTHE_V2_AES_P_P_KEY1_2                                             (0x00000030U)
#define CSL_DTHE_V2_AES_P_P_KEY1_3                                             (0x00000034U)
#define CSL_DTHE_V2_AES_P_P_KEY1_0                                             (0x00000038U)
#define CSL_DTHE_V2_AES_P_P_KEY1_1                                             (0x0000003CU)
#define CSL_DTHE_V2_AES_P_P_IV_IN_0                                            (0x00000040U)
#define CSL_DTHE_V2_AES_P_P_IV_IN_1                                            (0x00000044U)
#define CSL_DTHE_V2_AES_P_P_IV_IN_2                                            (0x00000048U)
#define CSL_DTHE_V2_AES_P_P_IV_IN_3                                            (0x0000004CU)
#define CSL_DTHE_V2_AES_P_P_CTRL                                               (0x00000050U)
#define CSL_DTHE_V2_AES_P_P_C_LENGTH_0                                         (0x00000054U)
#define CSL_DTHE_V2_AES_P_P_C_LENGTH_1                                         (0x00000058U)
#define CSL_DTHE_V2_AES_P_P_AUTH_LENGTH                                        (0x0000005CU)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_0                                      (0x00000060U)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_1                                      (0x00000064U)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_2                                      (0x00000068U)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_3                                      (0x0000006CU)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_0                                          (0x00000070U)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_1                                          (0x00000074U)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_2                                          (0x00000078U)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_3                                          (0x0000007CU)
#define CSL_DTHE_V2_AES_P_P_REVISION                                           (0x00000080U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG                                          (0x00000084U)
#define CSL_DTHE_V2_AES_P_P_SYSSTATUS                                          (0x00000088U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS                                          (0x0000008CU)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE                                          (0x00000090U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* P_KEY2_6 */

#define CSL_DTHE_V2_AES_P_P_KEY2_6_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY2_6_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY2_6_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY2_7 */

#define CSL_DTHE_V2_AES_P_P_KEY2_7_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY2_7_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY2_7_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY2_4 */

#define CSL_DTHE_V2_AES_P_P_KEY2_4_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY2_4_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY2_4_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY2_5 */

#define CSL_DTHE_V2_AES_P_P_KEY2_5_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY2_5_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY2_5_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY2_2 */

#define CSL_DTHE_V2_AES_P_P_KEY2_2_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY2_2_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY2_2_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY2_3 */

#define CSL_DTHE_V2_AES_P_P_KEY2_3_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY2_3_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY2_3_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY2_0 */

#define CSL_DTHE_V2_AES_P_P_KEY2_0_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY2_0_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY2_0_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY2_1 */

#define CSL_DTHE_V2_AES_P_P_KEY2_1_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY2_1_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY2_1_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY1_6 */

#define CSL_DTHE_V2_AES_P_P_KEY1_6_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY1_6_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY1_6_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY1_7 */

#define CSL_DTHE_V2_AES_P_P_KEY1_7_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY1_7_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY1_7_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY1_4 */

#define CSL_DTHE_V2_AES_P_P_KEY1_4_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY1_4_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY1_4_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY1_5 */

#define CSL_DTHE_V2_AES_P_P_KEY1_5_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY1_5_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY1_5_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY1_2 */

#define CSL_DTHE_V2_AES_P_P_KEY1_2_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY1_2_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY1_2_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY1_3 */

#define CSL_DTHE_V2_AES_P_P_KEY1_3_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY1_3_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY1_3_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY1_0 */

#define CSL_DTHE_V2_AES_P_P_KEY1_0_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY1_0_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY1_0_KEY_MAX                                     (0xFFFFFFFFU)


/* P_KEY1_1 */

#define CSL_DTHE_V2_AES_P_P_KEY1_1_KEY_MASK                                    (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_KEY1_1_KEY_SHIFT                                   (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_KEY1_1_KEY_MAX                                     (0xFFFFFFFFU)


/* P_IV_IN_0 */

#define CSL_DTHE_V2_AES_P_P_IV_IN_0_DATA_MASK                                  (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_IV_IN_0_DATA_SHIFT                                 (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_IV_IN_0_DATA_MAX                                   (0xFFFFFFFFU)


/* P_IV_IN_1 */

#define CSL_DTHE_V2_AES_P_P_IV_IN_1_DATA_MASK                                  (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_IV_IN_1_DATA_SHIFT                                 (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_IV_IN_1_DATA_MAX                                   (0xFFFFFFFFU)


/* P_IV_IN_2 */

#define CSL_DTHE_V2_AES_P_P_IV_IN_2_DATA_MASK                                  (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_IV_IN_2_DATA_SHIFT                                 (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_IV_IN_2_DATA_MAX                                   (0xFFFFFFFFU)


/* P_IV_IN_3 */

#define CSL_DTHE_V2_AES_P_P_IV_IN_3_DATA_MASK                                  (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_IV_IN_3_DATA_SHIFT                                 (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_IV_IN_3_DATA_MAX                                   (0xFFFFFFFFU)


/* P_CTRL */

#define CSL_DTHE_V2_AES_P_P_CTRL_OUTPUT_READY_MASK                             (0x00000001U)
#define CSL_DTHE_V2_AES_P_P_CTRL_OUTPUT_READY_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_OUTPUT_READY_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_INPUT_READY_MASK                              (0x00000002U)
#define CSL_DTHE_V2_AES_P_P_CTRL_INPUT_READY_SHIFT                             (0x00000001U)
#define CSL_DTHE_V2_AES_P_P_CTRL_INPUT_READY_MAX                               (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_DIRECTION_MASK                                (0x00000004U)
#define CSL_DTHE_V2_AES_P_P_CTRL_DIRECTION_SHIFT                               (0x00000002U)
#define CSL_DTHE_V2_AES_P_P_CTRL_DIRECTION_MAX                                 (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_KEY_SIZE_MASK                                 (0x00000018U)
#define CSL_DTHE_V2_AES_P_P_CTRL_KEY_SIZE_SHIFT                                (0x00000003U)
#define CSL_DTHE_V2_AES_P_P_CTRL_KEY_SIZE_MAX                                  (0x00000003U)

#define CSL_DTHE_V2_AES_P_P_CTRL_MODE_MASK                                     (0x00000020U)
#define CSL_DTHE_V2_AES_P_P_CTRL_MODE_SHIFT                                    (0x00000005U)
#define CSL_DTHE_V2_AES_P_P_CTRL_MODE_MAX                                      (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_CTR_MASK                                      (0x00000040U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CTR_SHIFT                                     (0x00000006U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CTR_MAX                                       (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_CTR_WIDTH_MASK                                (0x00000180U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CTR_WIDTH_SHIFT                               (0x00000007U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CTR_WIDTH_MAX                                 (0x00000003U)

#define CSL_DTHE_V2_AES_P_P_CTRL_ICM_MASK                                      (0x00000200U)
#define CSL_DTHE_V2_AES_P_P_CTRL_ICM_SHIFT                                     (0x00000009U)
#define CSL_DTHE_V2_AES_P_P_CTRL_ICM_MAX                                       (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_CFB_MASK                                      (0x00000400U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CFB_SHIFT                                     (0x0000000AU)
#define CSL_DTHE_V2_AES_P_P_CTRL_CFB_MAX                                       (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_XTS_MASK                                      (0x00001800U)
#define CSL_DTHE_V2_AES_P_P_CTRL_XTS_SHIFT                                     (0x0000000BU)
#define CSL_DTHE_V2_AES_P_P_CTRL_XTS_MAX                                       (0x00000003U)

#define CSL_DTHE_V2_AES_P_P_CTRL_F8_MASK                                       (0x00002000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_F8_SHIFT                                      (0x0000000DU)
#define CSL_DTHE_V2_AES_P_P_CTRL_F8_MAX                                        (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_F9_MASK                                       (0x00004000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_F9_SHIFT                                      (0x0000000EU)
#define CSL_DTHE_V2_AES_P_P_CTRL_F9_MAX                                        (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_CBCMAC_MASK                                   (0x00008000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CBCMAC_SHIFT                                  (0x0000000FU)
#define CSL_DTHE_V2_AES_P_P_CTRL_CBCMAC_MAX                                    (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_GCM_MASK                                      (0x00030000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_GCM_SHIFT                                     (0x00000010U)
#define CSL_DTHE_V2_AES_P_P_CTRL_GCM_MAX                                       (0x00000003U)

#define CSL_DTHE_V2_AES_P_P_CTRL_CCM_MASK                                      (0x00040000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CCM_SHIFT                                     (0x00000012U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CCM_MAX                                       (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_CCM_L_MASK                                    (0x00380000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CCM_L_SHIFT                                   (0x00000013U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CCM_L_MAX                                     (0x00000007U)

#define CSL_DTHE_V2_AES_P_P_CTRL_CCM_M_MASK                                    (0x01C00000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CCM_M_SHIFT                                   (0x00000016U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CCM_M_MAX                                     (0x00000007U)

#define CSL_DTHE_V2_AES_P_P_CTRL_RESERVED_MASK                                 (0x1E000000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_RESERVED_SHIFT                                (0x00000019U)
#define CSL_DTHE_V2_AES_P_P_CTRL_RESERVED_MAX                                  (0x0000000FU)

#define CSL_DTHE_V2_AES_P_P_CTRL_SAVE_CONTEXT_MASK                             (0x20000000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_SAVE_CONTEXT_SHIFT                            (0x0000001DU)
#define CSL_DTHE_V2_AES_P_P_CTRL_SAVE_CONTEXT_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_SAVE_CONTEXT_READY_MASK                       (0x40000000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_SAVE_CONTEXT_READY_SHIFT                      (0x0000001EU)
#define CSL_DTHE_V2_AES_P_P_CTRL_SAVE_CONTEXT_READY_MAX                        (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_CTRL_CONTEXT_READY_MASK                            (0x80000000U)
#define CSL_DTHE_V2_AES_P_P_CTRL_CONTEXT_READY_SHIFT                           (0x0000001FU)
#define CSL_DTHE_V2_AES_P_P_CTRL_CONTEXT_READY_MAX                             (0x00000001U)


/* P_C_LENGTH_0 */

#define CSL_DTHE_V2_AES_P_P_C_LENGTH_0_LENGTH_MASK                             (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_C_LENGTH_0_LENGTH_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_C_LENGTH_0_LENGTH_MAX                              (0xFFFFFFFFU)


/* P_C_LENGTH_1 */

#define CSL_DTHE_V2_AES_P_P_C_LENGTH_1_LENGTH_MASK                             (0x1FFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_C_LENGTH_1_LENGTH_SHIFT                            (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_C_LENGTH_1_LENGTH_MAX                              (0x1FFFFFFFU)

#define CSL_DTHE_V2_AES_P_P_C_LENGTH_1_RESERVED_MASK                           (0xE0000000U)
#define CSL_DTHE_V2_AES_P_P_C_LENGTH_1_RESERVED_SHIFT                          (0x0000001DU)
#define CSL_DTHE_V2_AES_P_P_C_LENGTH_1_RESERVED_MAX                            (0x00000007U)


/* P_AUTH_LENGTH */

#define CSL_DTHE_V2_AES_P_P_AUTH_LENGTH_AUTH_MASK                              (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_AUTH_LENGTH_AUTH_SHIFT                             (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_AUTH_LENGTH_AUTH_MAX                               (0xFFFFFFFFU)


/* P_DATA_IN_OUT_0 */

#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_0_DATA_MASK                            (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_0_DATA_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_0_DATA_MAX                             (0xFFFFFFFFU)


/* P_DATA_IN_OUT_1 */

#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_1_DATA_MASK                            (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_1_DATA_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_1_DATA_MAX                             (0xFFFFFFFFU)


/* P_DATA_IN_OUT_2 */

#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_2_DATA_MASK                            (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_2_DATA_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_2_DATA_MAX                             (0xFFFFFFFFU)


/* P_DATA_IN_OUT_3 */

#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_3_DATA_MASK                            (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_3_DATA_SHIFT                           (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_DATA_IN_OUT_3_DATA_MAX                             (0xFFFFFFFFU)


/* P_TAG_OUT_0 */

#define CSL_DTHE_V2_AES_P_P_TAG_OUT_0_HASH_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_0_HASH_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_0_HASH_MAX                                 (0xFFFFFFFFU)


/* P_TAG_OUT_1 */

#define CSL_DTHE_V2_AES_P_P_TAG_OUT_1_HASH_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_1_HASH_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_1_HASH_MAX                                 (0xFFFFFFFFU)


/* P_TAG_OUT_2 */

#define CSL_DTHE_V2_AES_P_P_TAG_OUT_2_HASH_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_2_HASH_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_2_HASH_MAX                                 (0xFFFFFFFFU)


/* P_TAG_OUT_3 */

#define CSL_DTHE_V2_AES_P_P_TAG_OUT_3_HASH_MASK                                (0xFFFFFFFFU)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_3_HASH_SHIFT                               (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_TAG_OUT_3_HASH_MAX                                 (0xFFFFFFFFU)


/* P_REVISION */

#define CSL_DTHE_V2_AES_P_P_REVISION_Y_MINOR_MASK                              (0x0000003FU)
#define CSL_DTHE_V2_AES_P_P_REVISION_Y_MINOR_SHIFT                             (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_REVISION_Y_MINOR_MAX                               (0x0000003FU)

#define CSL_DTHE_V2_AES_P_P_REVISION_CUSTOM_MASK                               (0x000000C0U)
#define CSL_DTHE_V2_AES_P_P_REVISION_CUSTOM_SHIFT                              (0x00000006U)
#define CSL_DTHE_V2_AES_P_P_REVISION_CUSTOM_MAX                                (0x00000003U)

#define CSL_DTHE_V2_AES_P_P_REVISION_X_MAJOR_MASK                              (0x00000700U)
#define CSL_DTHE_V2_AES_P_P_REVISION_X_MAJOR_SHIFT                             (0x00000008U)
#define CSL_DTHE_V2_AES_P_P_REVISION_X_MAJOR_MAX                               (0x00000007U)

#define CSL_DTHE_V2_AES_P_P_REVISION_R_RTL_MASK                                (0x0000F800U)
#define CSL_DTHE_V2_AES_P_P_REVISION_R_RTL_SHIFT                               (0x0000000BU)
#define CSL_DTHE_V2_AES_P_P_REVISION_R_RTL_MAX                                 (0x0000001FU)

#define CSL_DTHE_V2_AES_P_P_REVISION_FUNC_MASK                                 (0x0FFF0000U)
#define CSL_DTHE_V2_AES_P_P_REVISION_FUNC_SHIFT                                (0x00000010U)
#define CSL_DTHE_V2_AES_P_P_REVISION_FUNC_MAX                                  (0x00000FFFU)

#define CSL_DTHE_V2_AES_P_P_REVISION_RESERVED_MASK                             (0x30000000U)
#define CSL_DTHE_V2_AES_P_P_REVISION_RESERVED_SHIFT                            (0x0000001CU)
#define CSL_DTHE_V2_AES_P_P_REVISION_RESERVED_MAX                              (0x00000003U)

#define CSL_DTHE_V2_AES_P_P_REVISION_SCHEME_MASK                               (0xC0000000U)
#define CSL_DTHE_V2_AES_P_P_REVISION_SCHEME_SHIFT                              (0x0000001EU)
#define CSL_DTHE_V2_AES_P_P_REVISION_SCHEME_MAX                                (0x00000003U)


/* P_SYSCONFIG */

#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_DATA_IN_EN_MASK                  (0x00000020U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_DATA_IN_EN_SHIFT                 (0x00000005U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_DATA_IN_EN_MAX                   (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_DATA_OUT_EN_MASK                 (0x00000040U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_DATA_OUT_EN_SHIFT                (0x00000006U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_DATA_OUT_EN_MAX                  (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_MASK               (0x00000080U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_SHIFT              (0x00000007U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_MAX                (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_MASK              (0x00000100U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_SHIFT             (0x00000008U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_MAX               (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_MASK         (0x00000200U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_SHIFT        (0x00000009U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_MAX          (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_RESERVED_MASK                            (0xFFFFFC00U)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_RESERVED_SHIFT                           (0x0000000AU)
#define CSL_DTHE_V2_AES_P_P_SYSCONFIG_RESERVED_MAX                             (0x003FFFFFU)


/* P_SYSSTATUS */

#define CSL_DTHE_V2_AES_P_P_SYSSTATUS_RESETDONE_MASK                           (0x00000001U)
#define CSL_DTHE_V2_AES_P_P_SYSSTATUS_RESETDONE_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_SYSSTATUS_RESETDONE_MAX                            (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_SYSSTATUS_RESERVED_MASK                            (0xFFFFFFFEU)
#define CSL_DTHE_V2_AES_P_P_SYSSTATUS_RESERVED_SHIFT                           (0x00000001U)
#define CSL_DTHE_V2_AES_P_P_SYSSTATUS_RESERVED_MAX                             (0x7FFFFFFFU)


/* P_IRQSTATUS */

#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_CONTEXT_IN_MASK                          (0x00000001U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_CONTEXT_IN_SHIFT                         (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_CONTEXT_IN_MAX                           (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_DATA_IN_MASK                             (0x00000002U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_DATA_IN_SHIFT                            (0x00000001U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_DATA_IN_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_DATA_OUT_MASK                            (0x00000004U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_DATA_OUT_SHIFT                           (0x00000002U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_DATA_OUT_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_CONTEXT_OUT_MASK                         (0x00000008U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_CONTEXT_OUT_SHIFT                        (0x00000003U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_CONTEXT_OUT_MAX                          (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_RESERVED_MASK                            (0xFFFFFFF0U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_RESERVED_SHIFT                           (0x00000004U)
#define CSL_DTHE_V2_AES_P_P_IRQSTATUS_RESERVED_MAX                             (0x0FFFFFFFU)


/* P_IRQENABLE */

#define CSL_DTHE_V2_AES_P_P_IRQENABLE_CONTEXT_IN_MASK                          (0x00000001U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_CONTEXT_IN_SHIFT                         (0x00000000U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_CONTEXT_IN_MAX                           (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_IRQENABLE_DATA_IN_MASK                             (0x00000002U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_DATA_IN_SHIFT                            (0x00000001U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_DATA_IN_MAX                              (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_IRQENABLE_DATA_OUT_MASK                            (0x00000004U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_DATA_OUT_SHIFT                           (0x00000002U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_DATA_OUT_MAX                             (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_IRQENABLE_CONTEXT_OUT_MASK                         (0x00000008U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_CONTEXT_OUT_SHIFT                        (0x00000003U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_CONTEXT_OUT_MAX                          (0x00000001U)

#define CSL_DTHE_V2_AES_P_P_IRQENABLE_RESERVED_MASK                            (0xFFFFFFF0U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_RESERVED_SHIFT                           (0x00000004U)
#define CSL_DTHE_V2_AES_P_P_IRQENABLE_RESERVED_MAX                             (0x0FFFFFFFU)


#ifdef __cplusplus
}
#endif
#endif
