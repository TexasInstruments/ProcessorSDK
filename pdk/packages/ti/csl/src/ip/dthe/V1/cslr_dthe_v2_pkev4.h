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
 *  Name        : cslr_dthe_v2_pkev4.h
 *  VPVERSION   : 3.0.367 - 2024.03.02.21.42.22
 *  VPREV       : 1.0.8
*/
#ifndef CSLR_DTHE_V2_PKEV4_H_
#define CSLR_DTHE_V2_PKEV4_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  : PKEV4 registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t PKE_RESET_CTRL;
    volatile uint8_t  Resv_24[20];
    volatile uint32_t PKE_IFC_ISR;
    volatile uint32_t PKE_IFC_ICR;
    volatile uint32_t PKE_STATUS;
    volatile uint8_t  Resv_48[12];
    volatile uint32_t PKE_SCRATCH;
    volatile uint32_t MAU_MIN_LEN;
    volatile uint32_t MAU_MAX_LEN;
    volatile uint32_t PKE_VERSION;
    volatile uint32_t MCG_COMMANDS_ENABLED_0;
    volatile uint32_t MCG_COMMANDS_ENABLED_1;
    volatile uint32_t MCG_COMMANDS_ENABLED_2;
    volatile uint32_t MCG_COMMANDS_ENABLED_3;
    volatile uint32_t PKE_PRNG_SEED_0;
    volatile uint32_t PKE_PRNG_SEED_1;
    volatile uint32_t PKE_PRNG_SEED_2;
    volatile uint32_t PKE_PRNG_SEED_3;
    volatile uint32_t PKE_WORD_SIZE;
    volatile uint32_t PKE_CURVES_IN_ROM_0;
    volatile uint32_t PKE_CURVES_IN_ROM_1;
    volatile uint32_t PKE_SPECIAL_PRIMES;
    volatile uint32_t PKE_SRAM_ADDR_MIN;
    volatile uint32_t PKE_SRAM_ADDR_MAX;
    volatile uint8_t  Resv_128[8];
    volatile uint32_t MAU_COMMAND;
    volatile uint8_t  Resv_136[4];
    volatile uint32_t MAU_MCG_COMMAND;
    volatile uint8_t  Resv_8192[8052];
    volatile uint32_t CONTEXT_MEM[1024];
} CSL_dthe_v2_pkev4Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DTHE_V2_PKEV4_PKE_RESET_CTRL                                       (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR                                          (0x00000018U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR                                          (0x0000001CU)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS                                           (0x00000020U)
#define CSL_DTHE_V2_PKEV4_PKE_SCRATCH                                          (0x00000030U)
#define CSL_DTHE_V2_PKEV4_MAU_MIN_LEN                                          (0x00000034U)
#define CSL_DTHE_V2_PKEV4_MAU_MAX_LEN                                          (0x00000038U)
#define CSL_DTHE_V2_PKEV4_PKE_VERSION                                          (0x0000003CU)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_0                               (0x00000040U)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_1                               (0x00000044U)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_2                               (0x00000048U)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_3                               (0x0000004CU)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_0                                      (0x00000050U)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_1                                      (0x00000054U)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_2                                      (0x00000058U)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_3                                      (0x0000005CU)
#define CSL_DTHE_V2_PKEV4_PKE_WORD_SIZE                                        (0x00000060U)
#define CSL_DTHE_V2_PKEV4_PKE_CURVES_IN_ROM_0                                  (0x00000064U)
#define CSL_DTHE_V2_PKEV4_PKE_CURVES_IN_ROM_1                                  (0x00000068U)
#define CSL_DTHE_V2_PKEV4_PKE_SPECIAL_PRIMES                                   (0x0000006CU)
#define CSL_DTHE_V2_PKEV4_PKE_SRAM_ADDR_MIN                                    (0x00000070U)
#define CSL_DTHE_V2_PKEV4_PKE_SRAM_ADDR_MAX                                    (0x00000074U)
#define CSL_DTHE_V2_PKEV4_MAU_COMMAND                                          (0x00000080U)
#define CSL_DTHE_V2_PKEV4_MAU_MCG_COMMAND                                      (0x00000088U)
#define CSL_DTHE_V2_PKEV4_CONTEXT_MEM(CONTEXT_MEM)                             (0x00002000U+((CONTEXT_MEM)*0x4U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* PKE_RESET_CTRL */

#define CSL_DTHE_V2_PKEV4_PKE_RESET_CTRL_PKEFLUSH_MASK                         (0x00100000U)
#define CSL_DTHE_V2_PKEV4_PKE_RESET_CTRL_PKEFLUSH_SHIFT                        (0x00000014U)
#define CSL_DTHE_V2_PKEV4_PKE_RESET_CTRL_PKEFLUSH_MAX                          (0x00000001U)


/* PKE_IFC_ISR */

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_RAMINITANDFLUSHONRESETTOIDLEINT_MASK     (0x00000080U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_RAMINITANDFLUSHONRESETTOIDLEINT_SHIFT    (0x00000007U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_RAMINITANDFLUSHONRESETTOIDLEINT_MAX      (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_MCGERRORINT_MASK                         (0x00000040U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_MCGERRORINT_SHIFT                        (0x00000006U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_MCGERRORINT_MAX                          (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_MAUDONEERRORINT_MASK                     (0x00000010U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_MAUDONEERRORINT_SHIFT                    (0x00000004U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_MAUDONEERRORINT_MAX                      (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_FIFOEMPTYINT_MASK                        (0x00000008U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_FIFOEMPTYINT_SHIFT                       (0x00000003U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_FIFOEMPTYINT_MAX                         (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_FIFOERRORINT_MASK                        (0x00000004U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_FIFOERRORINT_SHIFT                       (0x00000002U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_FIFOERRORINT_MAX                         (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_MAUCOMMANDDONEINT_MASK                   (0x00000002U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_MAUCOMMANDDONEINT_SHIFT                  (0x00000001U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_MAUCOMMANDDONEINT_MAX                    (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_TRANSITIONTOIDLEINT_MASK                 (0x00000001U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_TRANSITIONTOIDLEINT_SHIFT                (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ISR_TRANSITIONTOIDLEINT_MAX                  (0x00000001U)


/* PKE_IFC_ICR */

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_RAMINITANDFLUSHONRESETTOIDLEINTEN_MASK   (0x00000080U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_RAMINITANDFLUSHONRESETTOIDLEINTEN_SHIFT  (0x00000007U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_RAMINITANDFLUSHONRESETTOIDLEINTEN_MAX    (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_MCGERRORINTEN_MASK                       (0x00000040U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_MCGERRORINTEN_SHIFT                      (0x00000006U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_MCGERRORINTEN_MAX                        (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_MAUDONEERRORINTEN_MASK                   (0x00000010U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_MAUDONEERRORINTEN_SHIFT                  (0x00000004U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_MAUDONEERRORINTEN_MAX                    (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_FIFOEMPTYINTEN_MASK                      (0x00000008U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_FIFOEMPTYINTEN_SHIFT                     (0x00000003U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_FIFOEMPTYINTEN_MAX                       (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_FIFOERRORINTEN_MASK                      (0x00000004U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_FIFOERRORINTEN_SHIFT                     (0x00000002U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_FIFOERRORINTEN_MAX                       (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_MAUCOMMANDDONEINTEN_MASK                 (0x00000002U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_MAUCOMMANDDONEINTEN_SHIFT                (0x00000001U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_MAUCOMMANDDONEINTEN_MAX                  (0x00000001U)

#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_TRANSITIONTOIDLEINTEN_MASK               (0x00000001U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_TRANSITIONTOIDLEINTEN_SHIFT              (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_IFC_ICR_TRANSITIONTOIDLEINTEN_MAX                (0x00000001U)


/* PKE_STATUS */

#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUCOMPARERESULT_MASK                     (0xC0000000U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUCOMPARERESULT_SHIFT                    (0x0000001EU)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUCOMPARERESULT_MAX                      (0x00000003U)

#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MCGERRORCAUSE_MASK                        (0x0F000000U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MCGERRORCAUSE_SHIFT                       (0x00000018U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MCGERRORCAUSE_MAX                         (0x0000000FU)

#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUERRORCAUSE_MASK                        (0x00FF0000U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUERRORCAUSE_SHIFT                       (0x00000010U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUERRORCAUSE_MAX                         (0x000000FFU)

#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUCMDGENSTATUS_MASK                      (0x0000FF00U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUCMDGENSTATUS_SHIFT                     (0x00000008U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUCMDGENSTATUS_MAX                       (0x000000FFU)

#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUCORESTATUSSTATE_MASK                   (0x000000F0U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUCORESTATUSSTATE_SHIFT                  (0x00000004U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_MAUCORESTATUSSTATE_MAX                    (0x0000000FU)

#define CSL_DTHE_V2_PKEV4_PKE_STATUS_FIFOSTATUS_MASK                           (0x0000000FU)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_FIFOSTATUS_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_STATUS_FIFOSTATUS_MAX                            (0x0000000FU)


/* PKE_SCRATCH */

#define CSL_DTHE_V2_PKEV4_PKE_SCRATCH_MAUIFCSCRATCH_MASK                       (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_SCRATCH_MAUIFCSCRATCH_SHIFT                      (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_SCRATCH_MAUIFCSCRATCH_MAX                        (0xFFFFFFFFU)


/* MAU_MIN_LEN */

#define CSL_DTHE_V2_PKEV4_MAU_MIN_LEN_MAUMINLEN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_MAU_MIN_LEN_MAUMINLEN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_PKEV4_MAU_MIN_LEN_MAUMINLEN_MAX                            (0xFFFFFFFFU)


/* MAU_MAX_LEN */

#define CSL_DTHE_V2_PKEV4_MAU_MAX_LEN_MAUMAXLEN_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_MAU_MAX_LEN_MAUMAXLEN_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_PKEV4_MAU_MAX_LEN_MAUMAXLEN_MAX                            (0xFFFFFFFFU)


/* PKE_VERSION */

#define CSL_DTHE_V2_PKEV4_PKE_VERSION_PKEVERSION_MASK                          (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_VERSION_PKEVERSION_SHIFT                         (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_VERSION_PKEVERSION_MAX                           (0xFFFFFFFFU)


/* MCG_COMMANDS_ENABLED_0 */

#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_0_MCGCOMMANDSENABLED_MASK       (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_0_MCGCOMMANDSENABLED_SHIFT      (0x00000000U)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_0_MCGCOMMANDSENABLED_MAX        (0xFFFFFFFFU)


/* MCG_COMMANDS_ENABLED_1 */

#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_1_MCGCOMMANDSENABLED_MASK       (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_1_MCGCOMMANDSENABLED_SHIFT      (0x00000000U)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_1_MCGCOMMANDSENABLED_MAX        (0xFFFFFFFFU)


/* MCG_COMMANDS_ENABLED_2 */

#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_2_MCGCOMMANDSENABLED_MASK       (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_2_MCGCOMMANDSENABLED_SHIFT      (0x00000000U)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_2_MCGCOMMANDSENABLED_MAX        (0xFFFFFFFFU)


/* MCG_COMMANDS_ENABLED_3 */

#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_3_MCGCOMMANDSENABLED_MASK       (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_3_MCGCOMMANDSENABLED_SHIFT      (0x00000000U)
#define CSL_DTHE_V2_PKEV4_MCG_COMMANDS_ENABLED_3_MCGCOMMANDSENABLED_MAX        (0xFFFFFFFFU)


/* PKE_PRNG_SEED_0 */

#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_0_PKEPRNGSEED_0_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_0_PKEPRNGSEED_0_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_0_PKEPRNGSEED_0_MAX                    (0xFFFFFFFFU)


/* PKE_PRNG_SEED_1 */

#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_1_PKEPRNGSEED_1_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_1_PKEPRNGSEED_1_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_1_PKEPRNGSEED_1_MAX                    (0xFFFFFFFFU)


/* PKE_PRNG_SEED_2 */

#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_2_PKEPRNGSEED_2_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_2_PKEPRNGSEED_2_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_2_PKEPRNGSEED_2_MAX                    (0xFFFFFFFFU)


/* PKE_PRNG_SEED_3 */

#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_3_PKEPRNGSEED_3_MASK                   (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_3_PKEPRNGSEED_3_SHIFT                  (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_PRNG_SEED_3_PKEPRNGSEED_3_MAX                    (0xFFFFFFFFU)


/* PKE_WORD_SIZE */

#define CSL_DTHE_V2_PKEV4_PKE_WORD_SIZE_PKEWORDSIZE_MASK                       (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_WORD_SIZE_PKEWORDSIZE_SHIFT                      (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_WORD_SIZE_PKEWORDSIZE_MAX                        (0xFFFFFFFFU)


/* PKE_CURVES_IN_ROM_0 */

#define CSL_DTHE_V2_PKEV4_PKE_CURVES_IN_ROM_0_PKECURVESINROM_MASK              (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_CURVES_IN_ROM_0_PKECURVESINROM_SHIFT             (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_CURVES_IN_ROM_0_PKECURVESINROM_MAX               (0xFFFFFFFFU)


/* PKE_CURVES_IN_ROM_1 */

#define CSL_DTHE_V2_PKEV4_PKE_CURVES_IN_ROM_1_PKECURVESINROM_MASK              (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_CURVES_IN_ROM_1_PKECURVESINROM_SHIFT             (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_CURVES_IN_ROM_1_PKECURVESINROM_MAX               (0xFFFFFFFFU)


/* PKE_SPECIAL_PRIMES */

#define CSL_DTHE_V2_PKEV4_PKE_SPECIAL_PRIMES_PKESPECIALPRIMES_MASK             (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_SPECIAL_PRIMES_PKESPECIALPRIMES_SHIFT            (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_SPECIAL_PRIMES_PKESPECIALPRIMES_MAX              (0xFFFFFFFFU)


/* PKE_SRAM_ADDR_MIN */

#define CSL_DTHE_V2_PKEV4_PKE_SRAM_ADDR_MIN_PKESRAMADDRMIN_MASK                (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_SRAM_ADDR_MIN_PKESRAMADDRMIN_SHIFT               (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_SRAM_ADDR_MIN_PKESRAMADDRMIN_MAX                 (0xFFFFFFFFU)


/* PKE_SRAM_ADDR_MAX */

#define CSL_DTHE_V2_PKEV4_PKE_SRAM_ADDR_MAX_PKESRAMADDRMAX_MASK                (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_PKE_SRAM_ADDR_MAX_PKESRAMADDRMAX_SHIFT               (0x00000000U)
#define CSL_DTHE_V2_PKEV4_PKE_SRAM_ADDR_MAX_PKESRAMADDRMAX_MAX                 (0xFFFFFFFFU)


/* MAU_COMMAND */

#define CSL_DTHE_V2_PKEV4_MAU_COMMAND_MAUCOMMAND_MASK                          (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_MAU_COMMAND_MAUCOMMAND_SHIFT                         (0x00000000U)
#define CSL_DTHE_V2_PKEV4_MAU_COMMAND_MAUCOMMAND_MAX                           (0xFFFFFFFFU)


/* MAU_MCG_COMMAND */

#define CSL_DTHE_V2_PKEV4_MAU_MCG_COMMAND_MAUCOMMANDCMDGEN_MASK                (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_MAU_MCG_COMMAND_MAUCOMMANDCMDGEN_SHIFT               (0x00000000U)
#define CSL_DTHE_V2_PKEV4_MAU_MCG_COMMAND_MAUCOMMANDCMDGEN_MAX                 (0xFFFFFFFFU)


/* CONTEXT_MEM */

#define CSL_DTHE_V2_PKEV4_CONTEXT_MEM_CONTEXT_WORD_MASK                        (0xFFFFFFFFU)
#define CSL_DTHE_V2_PKEV4_CONTEXT_MEM_CONTEXT_WORD_SHIFT                       (0x00000000U)
#define CSL_DTHE_V2_PKEV4_CONTEXT_MEM_CONTEXT_WORD_MAX                         (0xFFFFFFFFU)


#ifdef __cplusplus
}
#endif
#endif
