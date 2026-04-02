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


/* Properties of PBIST instances in: PBIST2 */
#define CSL_PBIST2_NUM_TEST_VECTORS                                                                (1U)
#define CSL_PBIST2_ALGO_BITMAP_0                                                                   (0x0000000000000003U)
#define CSL_PBIST2_MEM_BITMAP_0                                                                    (0x00000000000CCCCCU)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x000003FFU)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x000003FFU)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x000000FFU)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000003U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000009U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x000003FFU)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x20000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000004U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000009U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST2_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x041D2728U)

/* Properties of PBIST instances in: COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0 */
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_NUM_TEST_VECTORS                               (1U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_ALGO_BITMAP_0                                  (0x0000000000000003U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_MEM_BITMAP_0                                   (0x000000000000011DU)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA0                 (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA1                 (0x000001FFU)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA2                 (0x000001FFU)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA3                 (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL0                 (0x0000007FU)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL1                 (0x00000003U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL2                 (0x00000008U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL3                 (0x000001FFU)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CMS                 (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CSR                 (0x00000C00U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I0                  (0x00000001U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I1                  (0x00000004U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I2                  (0x00000008U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I3                  (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_MSMC_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_RAMT                (0x480A4034U)

/* Properties of PBIST instances in: COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0 */
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_NUM_TEST_VECTORS                               (1U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_ALGO_BITMAP_0                                  (0x0000000000000003U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_MEM_BITMAP_0                                   (0x0FFFC000FF00FF00U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA0                 (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA1                 (0x000000FFU)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA2                 (0x000000FFU)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CA3                 (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL0                 (0x0000003FU)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL1                 (0x00000003U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL2                 (0x00000007U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CL3                 (0x000000FFU)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CMS                 (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_CSR                 (0x00000030U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I0                  (0x00000001U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I1                  (0x00000004U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I2                  (0x00000007U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_I3                  (0x00000000U)
#define CSL_COMPUTE_CLUSTER0_ARM0_DFT_EMBED_PBIST_0_FAIL_INSERTION_TEST_VECTOR_RAMT                (0x11042740U)

/* Properties of PBIST instances in: PBIST0 */
#define CSL_PBIST0_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST0_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST0_MEM_BITMAP_0                                                                    (0x0000001554E336A5U)
#define CSL_PBIST0_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST0_MEM_BITMAP_1                                                                    (0x14A1E1C000000000U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x0000003FU)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x0000003FU)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x0000000FU)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000003U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000005U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x0000003FU)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000000U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x000001E0U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000004U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000005U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST0_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x0B052028U)

/* Properties of PBIST instances in: MCU_PBIST0 */
#define CSL_MCU_PBIST0_NUM_TEST_VECTORS                                                            (2U)
#define CSL_MCU_PBIST0_ALGO_BITMAP_0                                                               (0x0000000000000014U)
#define CSL_MCU_PBIST0_MEM_BITMAP_0                                                                (0x0000000000000420U)
#define CSL_MCU_PBIST0_ALGO_BITMAP_1                                                               (0x000000000000000AU)
#define CSL_MCU_PBIST0_MEM_BITMAP_1                                                                (0x000000000000000AU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA0                                              (0x00000000U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA1                                              (0x000007FFU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA2                                              (0x000007FFU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CA3                                              (0x00000000U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL0                                              (0x000001FFU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL1                                              (0x00000003U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL2                                              (0x0000000AU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CL3                                              (0x000007FFU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CMS                                              (0x00000002U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_CSR                                              (0x0000C000U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_I0                                               (0x00000001U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_I1                                               (0x00000004U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_I2                                               (0x0000000AU)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_I3                                               (0x00000000U)
#define CSL_MCU_PBIST0_FAIL_INSERTION_TEST_VECTOR_RAMT                                             (0x0D0E2028U)

/* Properties of PBIST instances in: MCU_PBIST1 */
#define CSL_MCU_PBIST1_NUM_TEST_VECTORS                                                            (2U)
#define CSL_MCU_PBIST1_ALGO_BITMAP_0                                                               (0x0000000000000014U)
#define CSL_MCU_PBIST1_MEM_BITMAP_0                                                                (0x0000000008038534U)
#define CSL_MCU_PBIST1_ALGO_BITMAP_1                                                               (0x0000000000000028U)
#define CSL_MCU_PBIST1_MEM_BITMAP_1                                                                (0x000042A500000002U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA0                                              (0x00000000U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA1                                              (0x000000FFU)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA2                                              (0x000000FFU)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA3                                              (0x00000000U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL0                                              (0x0000003FU)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL1                                              (0x00000003U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL2                                              (0x00000007U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL3                                              (0x000000FFU)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CMS                                              (0x00000003U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_CSR                                              (0x00078000U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_I0                                               (0x00000001U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_I1                                               (0x00000004U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_I2                                               (0x00000007U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_I3                                               (0x00000000U)
#define CSL_MCU_PBIST1_FAIL_INSERTION_TEST_VECTOR_RAMT                                             (0x05102028U)

/* Properties of PBIST instances in: MCU_PBIST2 */
#define CSL_MCU_PBIST2_NUM_TEST_VECTORS                                                            (1U)
#define CSL_MCU_PBIST2_ALGO_BITMAP_0                                                               (0x0000000000000006U)
#define CSL_MCU_PBIST2_MEM_BITMAP_0                                                                (0x0000000000199998U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA0                                              (0x00000000U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA1                                              (0x000000FFU)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA2                                              (0x000000FFU)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CA3                                              (0x00000000U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL0                                              (0x0000003FU)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL1                                              (0x00000003U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL2                                              (0x00000007U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CL3                                              (0x000000FFU)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CMS                                              (0x00000003U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_CSR                                              (0x00078000U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_I0                                               (0x00000001U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_I1                                               (0x00000004U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_I2                                               (0x00000007U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_I3                                               (0x00000000U)
#define CSL_MCU_PBIST2_FAIL_INSERTION_TEST_VECTOR_RAMT                                             (0x05102028U)

/* Properties of PBIST instances in: PBIST1 */
#define CSL_PBIST1_NUM_TEST_VECTORS                                                                (2U)
#define CSL_PBIST1_ALGO_BITMAP_0                                                                   (0x0000000000000005U)
#define CSL_PBIST1_MEM_BITMAP_0                                                                    (0x0000000000000001U)
#define CSL_PBIST1_ALGO_BITMAP_1                                                                   (0x000000000000000AU)
#define CSL_PBIST1_MEM_BITMAP_1                                                                    (0x0000000000000018U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA0                                                  (0x00000000U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA1                                                  (0x0000003FU)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA2                                                  (0x0000003FU)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CA3                                                  (0x00000000U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL0                                                  (0x0000001FU)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL1                                                  (0x00000001U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL2                                                  (0x00000005U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CL3                                                  (0x0000003FU)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CMS                                                  (0x00000003U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_CSR                                                  (0x01FE0000U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_I0                                                   (0x00000001U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_I1                                                   (0x00000002U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_I2                                                   (0x00000005U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_I3                                                   (0x00000000U)
#define CSL_PBIST1_FAIL_INSERTION_TEST_VECTOR_RAMT                                                 (0x05152028U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_SOC_PBIST_H_ */

