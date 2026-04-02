/********************************************************************
*
* SOC PBIST PROPERTIES. header file
*
* Copyright (C) 2015-2020 Texas Instruments Incorporated.
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
#ifndef CSLR_SOC_PBIST_H_
#define CSLR_SOC_PBIST_H_

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* Auto-generated CSL definitions for SoC PBIST Instances:
*/


/* Properties of PBIST instances in: PBIST5 */
#define CSL_PBIST5_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST5_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST5_MEM_BITMAP_0                                                                    (0x0000000000000280U)
#define CSL_PBIST5_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST5_MEM_BITMAP_1                                                                    (0x0000000000020000U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x000001FFU)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x000001FFU)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x0000007FU)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000003U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000008U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x000001FFU)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000001U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x00000003U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000004U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000008U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST5_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x55002028U)

/* Properties of PBIST instances in: PBIST0 */
#define CSL_PBIST0_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST0_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST0_MEM_BITMAP_0                                                                    (0x0000000000033316U)
#define CSL_PBIST0_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST0_MEM_BITMAP_1                                                                    (0x00000000B0D80000U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x0000005FU)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x0000005FU)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x0000001FU)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x0000002FU)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000001U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000006U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x0000003FU)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000000U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x0003C000U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000002U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000005U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000004U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x07112038U)

/* Properties of PBIST instances in: PBIST1 */
#define CSL_PBIST1_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST1_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST1_MEM_BITMAP_0                                                                    (0x0000000000000149U)
#define CSL_PBIST1_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST1_MEM_BITMAP_1                                                                    (0x0000000000000400U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x000000FFU)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x000000FFU)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x0000007FU)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000001U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000007U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x000000FFU)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000000U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x00000001U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000002U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000007U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x01001838U)

/* Properties of PBIST instances in: PBIST9 */
#define CSL_PBIST9_NUM_TEST_VECTORS                                                                (1U)
#define CSL_PBIST9_ALGO_BITMAP_0                                                                   (0x0000000000000003U)
#define CSL_PBIST9_MEM_BITMAP_0                                                                    (0x0000000000000015U)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x000007FFU)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x000007FFU)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x000001FFU)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000003U)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x0000000AU)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x000007FFU)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000000U)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x00000001U)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000004U)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x0000000AU)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST9_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x00002728U)

/* Properties of PBIST instances in: C66SS1_PBIST0 */
#define CSL_C66SS1_PBIST0_NUM_TEST_VECTORS                                                         (2U)
#define CSL_C66SS1_PBIST0_ALGO_BITMAP_0                                                            (0x0000000000000003U)
#define CSL_C66SS1_PBIST0_MEM_BITMAP_0                                                             (0x000000000C038002U)
#define CSL_C66SS1_PBIST0_ALGO_BITMAP_1                                                            (0x0000000000000003U)
#define CSL_C66SS1_PBIST0_MEM_BITMAP_1                                                             (0x0000000003FC0000U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA0                                           (0x00000000U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA1                                           (0x000000FFU)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA2                                           (0x000000FFU)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA3                                           (0x00000000U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL0                                           (0x0000003FU)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL1                                           (0x00000003U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL2                                           (0x00000007U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL3                                           (0x000000FFU)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CMS                                           (0x00000000U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_CSR                                           (0x00000003U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_I0                                            (0x00000001U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_I1                                            (0x00000004U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_I2                                            (0x00000007U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_I3                                            (0x00000000U)
#define CSL_C66SS1_PBIST0_FAIL_INSERTION_TEST_VECTOR_RAMT                                          (0x08002014U)

/* Properties of PBIST instances in: PBIST4 */
#define CSL_PBIST4_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST4_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST4_MEM_BITMAP_0                                                                    (0x0000000000002014U)
#define CSL_PBIST4_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST4_MEM_BITMAP_1                                                                    (0x0000000088A90000U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x000003FFU)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x000003FFU)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x000000FFU)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000003U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000009U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x000003FFU)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000000U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x00000003U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000004U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000009U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST4_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x00004028U)

/* Properties of PBIST instances in: COMPUTE_CLUSTER0_PBIST_WRAP */
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_NUM_TEST_VECTORS                                           (2U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_ALGO_BITMAP_0                                              (0x0000000000000005U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_MEM_BITMAP_0                                               (0x00000000000041F1U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_ALGO_BITMAP_1                                              (0x000000000000000AU)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_MEM_BITMAP_1                                               (0x0003800000000000U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA0                             (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA1                             (0x00000FFFU)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA2                             (0x00000FFFU)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA3                             (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL0                             (0x000003FFU)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL1                             (0x00000003U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL2                             (0x0000000BU)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL3                             (0x00000FFFU)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CMS                             (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CSR                             (0x000002FFU)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I0                              (0x00000001U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I1                              (0x00000004U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I2                              (0x0000000BU)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I3                              (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_RAMT                            (0x40004037U)

/* Properties of PBIST instances in: GPU0_DFT_PBIST_0 */
#define CSL_GPU0_DFT_PBIST_0_NUM_TEST_VECTORS                                                      (2U)
#define CSL_GPU0_DFT_PBIST_0_ALGO_BITMAP_0                                                         (0x0000000000000005U)
#define CSL_GPU0_DFT_PBIST_0_MEM_BITMAP_0                                                          (0x000000000040C8C6U)
#define CSL_GPU0_DFT_PBIST_0_ALGO_BITMAP_1                                                         (0x000000000000000AU)
#define CSL_GPU0_DFT_PBIST_0_MEM_BITMAP_1                                                          (0x00000A8C03000000U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA0                                        (0x00000000U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA1                                        (0x000000FFU)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA2                                        (0x000000FFU)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA3                                        (0x00000000U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL0                                        (0x000000FFU)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL1                                        (0x00000000U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL2                                        (0x00000007U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL3                                        (0x000000FFU)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CMS                                        (0x00000000U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CSR                                        (0x00018000U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I0                                         (0x00000001U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I1                                         (0x00000001U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I2                                         (0x00000007U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I3                                         (0x00000000U)
#define CSL_GPU0_DFT_PBIST_0_FAIL_INSERTION_TEST_VECTOR_RAMT                                       (0x50104028U)

/* Properties of PBIST instances in: C71SS0_PBIST_WRAP */
#define CSL_C71SS0_PBIST_WRAP_NUM_TEST_VECTORS                                                     (1U)
#define CSL_C71SS0_PBIST_WRAP_ALGO_BITMAP_0                                                        (0x0000000000000003U)
#define CSL_C71SS0_PBIST_WRAP_MEM_BITMAP_0                                                         (0x00007FF8000F0FC0U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA0                                       (0x00000000U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA1                                       (0x000000FFU)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA2                                       (0x000000FFU)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA3                                       (0x00000000U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL0                                       (0x0000003FU)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL1                                       (0x00000003U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL2                                       (0x00000007U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL3                                       (0x000000FFU)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CMS                                       (0x00000000U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CSR                                       (0x00000003U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I0                                        (0x00000001U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I1                                        (0x00000004U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I2                                        (0x00000007U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I3                                        (0x00000000U)
#define CSL_C71SS0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_RAMT                                      (0x0800400CU)

/* Properties of PBIST instances in: MCU_PBIST0 */
#define CSL_MCU_PBIST0_NUM_TEST_VECTORS                                                            (2U)
#define CSL_MCU_PBIST0_ALGO_BITMAP_0                                                               (0x0000000000000014U)
#define CSL_MCU_PBIST0_MEM_BITMAP_0                                                                (0x00000000A0B42294U)
#define CSL_MCU_PBIST0_ALGO_BITMAP_1                                                               (0x0000000000000028U)
#define CSL_MCU_PBIST0_MEM_BITMAP_1                                                                (0x0151AA6600000002U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA0                                              (0x00000000U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA1                                              (0x000000FFU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA2                                              (0x000000FFU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA3                                              (0x00000000U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL0                                              (0x0000003FU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL1                                              (0x00000003U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL2                                              (0x00000007U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL3                                              (0x000000FFU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CMS                                              (0x00000000U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CSR                                              (0x00000001U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_I0                                               (0x00000001U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_I1                                               (0x00000004U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_I2                                               (0x00000007U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_I3                                               (0x00000000U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_RAMT                                             (0x35001628U)

/* Properties of PBIST instances in: A72SS0_CORE0_PBIST_WRAP */
#define CSL_A72SS0_CORE0_PBIST_WRAP_NUM_TEST_VECTORS                                               (1U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_ALGO_BITMAP_0                                                  (0x0000000000000003U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_MEM_BITMAP_0                                                   (0x0FFFC000FF00FF00U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA0                                 (0x00000000U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA1                                 (0x0000003FU)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA2                                 (0x0000003FU)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CA3                                 (0x00000000U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL0                                 (0x0000000FU)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL1                                 (0x00000003U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL2                                 (0x00000005U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CL3                                 (0x0000003FU)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CMS                                 (0x00000000U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_CSR                                 (0x00000003U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I0                                  (0x00000001U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I1                                  (0x00000004U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I2                                  (0x00000005U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_I3                                  (0x00000000U)
#define CSL_A72SS0_CORE0_PBIST_WRAP_FAIL_INSERTION_TEST_VECTOR_RAMT                                (0x00002840U)

/* Properties of PBIST instances in: PBIST2 */
#define CSL_PBIST2_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST2_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST2_MEM_BITMAP_0                                                                    (0x0000000000000159U)
#define CSL_PBIST2_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST2_MEM_BITMAP_1                                                                    (0x0000000000002C00U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x0000003FU)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x0000003FU)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x0000003FU)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000005U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x0000003FU)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x00000001U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000001U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000005U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x00004038U)

/* Properties of PBIST instances in: PBIST3 */
#define CSL_PBIST3_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST3_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST3_MEM_BITMAP_0                                                                    (0x0000000000000265U)
#define CSL_PBIST3_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST3_MEM_BITMAP_1                                                                    (0x0000000000001000U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x000003FFU)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x000003FFU)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x000000FFU)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000003U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000009U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x000003FFU)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000000U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x00000001U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000004U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000009U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST3_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x00001638U)

/* Properties of PBIST instances in: PBIST10 */
#define CSL_PBIST10_NUM_TEST_VECTORS                                                               (1U)
#define CSL_PBIST10_ALGO_BITMAP_0                                                                  (0x0000000000000003U)
#define CSL_PBIST10_MEM_BITMAP_0                                                                   (0x0000000000000015U)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CA0                                                 (0x00000000U)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CA1                                                 (0x000007FFU)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CA2                                                 (0x000007FFU)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CA3                                                 (0x00000000U)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CL0                                                 (0x000001FFU)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CL1                                                 (0x00000003U)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CL2                                                 (0x0000000AU)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CL3                                                 (0x000007FFU)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CMS                                                 (0x00000000U)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_CSR                                                 (0x00000001U)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_I0                                                  (0x00000001U)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_I1                                                  (0x00000004U)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_I2                                                  (0x0000000AU)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_I3                                                  (0x00000000U)
#define CSL_PBIST10_FAIL_INSERTION_TEST_VECTOR_RAMT                                                (0x00002728U)

/* Properties of PBIST instances in: C66SS0_PBIST0 */
#define CSL_C66SS0_PBIST0_NUM_TEST_VECTORS                                                         (2U)
#define CSL_C66SS0_PBIST0_ALGO_BITMAP_0                                                            (0x0000000000000003U)
#define CSL_C66SS0_PBIST0_MEM_BITMAP_0                                                             (0x000000000C038002U)
#define CSL_C66SS0_PBIST0_ALGO_BITMAP_1                                                            (0x0000000000000003U)
#define CSL_C66SS0_PBIST0_MEM_BITMAP_1                                                             (0x0000000003FC0000U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA0                                           (0x00000000U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA1                                           (0x000000FFU)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA2                                           (0x000000FFU)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA3                                           (0x00000000U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL0                                           (0x0000003FU)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL1                                           (0x00000003U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL2                                           (0x00000007U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL3                                           (0x000000FFU)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CMS                                           (0x00000000U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_CSR                                           (0x00000003U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_I0                                            (0x00000001U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_I1                                            (0x00000004U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_I2                                            (0x00000007U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_I3                                            (0x00000000U)
#define CSL_C66SS0_PBIST0_FAIL_INSERTION_TEST_VECTOR_RAMT                                          (0x08002014U)

/* Properties of PBIST instances in: MCU_PBIST1 */
#define CSL_MCU_PBIST1_NUM_TEST_VECTORS                                                            (1U)
#define CSL_MCU_PBIST1_ALGO_BITMAP_0                                                               (0x0000000000000006U)
#define CSL_MCU_PBIST1_MEM_BITMAP_0                                                                (0x000000000000002AU)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA0                                              (0x00000000U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA1                                              (0x0000007FU)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA2                                              (0x0000007FU)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA3                                              (0x00000000U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL0                                              (0x0000001FU)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL1                                              (0x00000003U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL2                                              (0x00000006U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL3                                              (0x0000007FU)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CMS                                              (0x00000000U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CSR                                              (0x00000001U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_I0                                               (0x00000001U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_I1                                               (0x00000004U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_I2                                               (0x00000006U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_I3                                               (0x00000000U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_RAMT                                             (0x04001C28U)

/* Properties of PBIST instances in: PBIST6 */
#define CSL_PBIST6_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST6_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST6_MEM_BITMAP_0                                                                    (0x00000000208D1500U)
#define CSL_PBIST6_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST6_MEM_BITMAP_1                                                                    (0x000000AA00000000U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x0000027FU)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x0000027FU)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x0000007FU)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x0000009FU)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000003U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000009U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x000001FFU)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000001U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x00E00000U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000004U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000008U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000006U)
#define CSL_PBIST6_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x11172028U)

/* Properties of PBIST instances in: PBIST7 */
#define CSL_PBIST7_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST7_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST7_MEM_BITMAP_0                                                                    (0x0000000000135AB5U)
#define CSL_PBIST7_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST7_MEM_BITMAP_1                                                                    (0x000000068DC00000U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x0000003FU)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x0000003FU)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x0000000FU)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000003U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000005U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x0000003FU)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000000U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x00000003U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000004U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000005U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST7_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x40004028U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_SOC_PBIST_H_ */

