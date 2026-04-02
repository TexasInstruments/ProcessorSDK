/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 */

/**
 *   @file  edma_soc.c
 *
 *   @brief
 *      edma soc specific configuration.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/drv/edma/edma.h>
#include <ti/drv/edma/soc/edma_soc_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#if defined (_TMS320C6X)
/* DSS EDMA Interrupts */
#define EDMA_DSS_CC0_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK)
#define EDMA_DSS_CC0_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS)
#define EDMA_DSS_CC0_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK)
#define EDMA_DSS_CC0_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS)

#define EDMA_DSS_CC1_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK)
#define EDMA_DSS_CC1_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS)
#define EDMA_DSS_CC1_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK)
#define EDMA_DSS_CC1_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS)

#define EDMA_DSS_CC2_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK)
#define EDMA_DSS_CC2_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS)
#define EDMA_DSS_CC2_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK)
#define EDMA_DSS_CC2_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS)

#define EDMA_DSS_CC3_INTAGG_MASK_REG_ADDRESS    (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_INTAGG_MASK)
#define EDMA_DSS_CC3_INTAGG_STATUS_REG_ADDRESS  (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_INTAGG_STATUS)
#define EDMA_DSS_CC3_ERRAGG_MASK_REG_ADDRESS    (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_ERRAGG_MASK)
#define EDMA_DSS_CC3_ERRAGG_STATUS_REG_ADDRESS  (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_ERRAGG_STATUS)
#endif

#if defined (__TI_ARM_V7R4__)
/* MSS EDMA Interrupts */
#define EDMA_MSS_CC0_INTAGG_MASK_REG_ADDRESS    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_A_INTAGG_MASK)
#define EDMA_MSS_CC0_INTAGG_STATUS_REG_ADDRESS  (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_A_INTAGG_STATUS)
#define EDMA_MSS_CC0_ERRAGG_MASK_REG_ADDRESS    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_A_ERRAGG_MASK)
#define EDMA_MSS_CC0_ERRAGG_STATUS_REG_ADDRESS  (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_A_ERRAGG_STATUS)

#define EDMA_MSS_CC1_INTAGG_MASK_REG_ADDRESS    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_B_INTAGG_MASK)
#define EDMA_MSS_CC1_INTAGG_STATUS_REG_ADDRESS  (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_B_INTAGG_STATUS)
#define EDMA_MSS_CC1_ERRAGG_MASK_REG_ADDRESS    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_B_ERRAGG_MASK)
#define EDMA_MSS_CC1_ERRAGG_STATUS_REG_ADDRESS  (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_B_ERRAGG_STATUS)

#define EDMA_MSS_CC2_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK)
#define EDMA_MSS_CC2_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS)
#define EDMA_MSS_CC2_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK)
#define EDMA_MSS_CC2_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS)

#define EDMA_MSS_CC3_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK)
#define EDMA_MSS_CC3_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS)
#define EDMA_MSS_CC3_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK)
#define EDMA_MSS_CC3_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS)

#define EDMA_MSS_CC4_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK)
#define EDMA_MSS_CC4_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS)
#define EDMA_MSS_CC4_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK)
#define EDMA_MSS_CC4_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS)

#define EDMA_MSS_CC5_INTAGG_MASK_REG_ADDRESS    (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_INTAGG_MASK)
#define EDMA_MSS_CC5_INTAGG_STATUS_REG_ADDRESS  (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_INTAGG_STATUS)
#define EDMA_MSS_CC5_ERRAGG_MASK_REG_ADDRESS    (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_ERRAGG_MASK)
#define EDMA_MSS_CC5_ERRAGG_STATUS_REG_ADDRESS  (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_ERRAGG_STATUS)
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined (_TMS320C6X)
/*! @brief EDMA hardware attributes global. */
const EDMA_hwAttrs_t gEdmaHwAttrs[EDMA_NUM_CC] = {
    {
        .CCbaseAddress      = CSL_DSS_TPCC_A_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_DSS_CC0_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_DSS_CC0_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_DSS_CC0_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_DSS_CC0_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_DSS_TPTC_A0_U_BASE,
        .TCbaseAddress[1]   = CSL_DSS_TPTC_A1_U_BASE,
        .numEventQueues     = EDMA_DSS_TPCC_A_NUM_TC,
        .numParamSets       = EDMA_DSS_TPCC_A_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum         = CSL_DSS_INTR_DSS_TPCC_A_INTAGG,
        .errorInterruptNum                      = CSL_DSS_INTR_DSS_TPCC_A_ERRAGG,
        .transferControllerErrorInterruptNum[0] = CSL_DSS_INTR_DSS_TPCC_A_ERRAGG,
        .transferControllerErrorInterruptNum[1] = CSL_DSS_INTR_DSS_TPCC_A_ERRAGG
    },
    {
        .CCbaseAddress      = CSL_DSS_TPCC_B_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_DSS_CC1_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_DSS_CC1_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_DSS_CC1_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_DSS_CC1_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_DSS_TPTC_B0_U_BASE,
        .TCbaseAddress[1]   = CSL_DSS_TPTC_B1_U_BASE,
        .numEventQueues     = EDMA_DSS_TPCC_B_NUM_TC,
        .numParamSets       = EDMA_DSS_TPCC_B_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum = CSL_DSS_INTR_DSS_TPCC_B_INTAGG,
        .errorInterruptNum                      = CSL_DSS_INTR_DSS_TPCC_B_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = CSL_DSS_INTR_DSS_TPCC_B_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = CSL_DSS_INTR_DSS_TPCC_B_ERRAGG //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = CSL_DSS_TPCC_C_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_DSS_CC2_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_DSS_CC2_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_DSS_CC2_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_DSS_CC2_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_DSS_TPTC_C0_U_BASE,
        .TCbaseAddress[1]   = CSL_DSS_TPTC_C1_U_BASE,
        .numEventQueues     = EDMA_DSS_TPCC_C_NUM_TC,
        .numParamSets       = EDMA_DSS_TPCC_C_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum = CSL_DSS_INTR_DSS_TPCC_C_INTAGG,
        .errorInterruptNum                      = CSL_DSS_INTR_DSS_TPCC_C_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = CSL_DSS_INTR_DSS_TPCC_C_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = CSL_DSS_INTR_DSS_TPCC_C_ERRAGG  //EDMA_INTERRUPT_NOT_CONNECTED_ID
    },
    {
        .CCbaseAddress      = CSL_RCSS_TPCC_A_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_DSS_CC3_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_DSS_CC3_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_DSS_CC3_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_DSS_CC3_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_RCSS_TPTC_A0_U_BASE,
        .TCbaseAddress[1]   = CSL_RCSS_TPTC_A1_U_BASE,
        .numEventQueues     = EDMA_RCSS_TPCC_A_NUM_TC,
        .numParamSets       = EDMA_RCSS_TPCC_A_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum = CSL_DSS_INTR_RCSS_TPCC_A_INTAGG,
        .errorInterruptNum                      = CSL_DSS_INTR_RCSS_TPCC_A_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = CSL_DSS_INTR_RCSS_TPCC_A_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = CSL_DSS_INTR_RCSS_TPCC_A_ERRAGG  //EDMA_INTERRUPT_NOT_CONNECTED_ID
    }
};
#endif

#if defined (__TI_ARM_V7R4__)
/*! @brief EDMA hardware attributes global. */
const EDMA_hwAttrs_t gEdmaHwAttrs[EDMA_NUM_CC] = {
    {
        .CCbaseAddress      = CSL_MSS_TPCC_A_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC0_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC0_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC0_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC0_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_MSS_TPTC_A0_U_BASE,
        .TCbaseAddress[1]   = CSL_MSS_TPTC_A1_U_BASE,
        .numEventQueues     = EDMA_MSS_TPCC_A_NUM_TC,
        .numParamSets       = EDMA_MSS_TPCC_A_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum = CSL_MSS_INTR_MSS_TPCC_A_INTAGG,
        //TODO_TPR12: enable when interrupts are assigned in future RTL version
        .errorInterruptNum                      = CSL_MSS_INTR_MSS_TPCC_A_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = CSL_MSS_INTR_MSS_TPCC_A_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = CSL_MSS_INTR_MSS_TPCC_A_ERRAGG  //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = CSL_MSS_TPCC_B_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC1_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC1_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC1_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC1_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_MSS_TPTC_B0_U_BASE,
        .numEventQueues     = EDMA_MSS_TPCC_B_NUM_TC,
        .numParamSets       = EDMA_MSS_TPCC_B_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum = CSL_MSS_INTR_MSS_TPCC_B_INTAGG,
        //TODO_TPR12: enable when interrupts are assigned in future RTL version
        .errorInterruptNum                      = CSL_MSS_INTR_MSS_TPCC_B_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = CSL_MSS_INTR_MSS_TPCC_B_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = CSL_DSS_TPCC_A_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC2_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC2_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC2_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC2_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_DSS_TPTC_A0_U_BASE,
        .TCbaseAddress[1]   = CSL_DSS_TPTC_A1_U_BASE,
        .numEventQueues     = EDMA_DSS_TPCC_A_NUM_TC,
        .numParamSets       = EDMA_DSS_TPCC_A_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum = CSL_MSS_INTR_DSS_TPCC_A_INTAGG,
        .errorInterruptNum                      = CSL_MSS_INTR_DSS_TPCC_A_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = CSL_MSS_INTR_DSS_TPCC_A_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = CSL_MSS_INTR_DSS_TPCC_A_ERRAGG  //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = CSL_DSS_TPCC_B_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC3_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC3_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC3_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC3_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_DSS_TPTC_B0_U_BASE,
        .TCbaseAddress[1]   = CSL_DSS_TPTC_B1_U_BASE,
        .numEventQueues     = EDMA_DSS_TPCC_B_NUM_TC,
        .numParamSets       = EDMA_DSS_TPCC_B_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum = CSL_MSS_INTR_DSS_TPCC_B_INTAGG,
        .errorInterruptNum                      = CSL_MSS_INTR_DSS_TPCC_B_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = CSL_MSS_INTR_DSS_TPCC_B_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = CSL_MSS_INTR_DSS_TPCC_B_ERRAGG  //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = CSL_DSS_TPCC_C_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC4_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC4_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC4_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC4_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_DSS_TPTC_C0_U_BASE,
        .TCbaseAddress[1]   = CSL_DSS_TPTC_C1_U_BASE,
        .numEventQueues     = EDMA_DSS_TPCC_C_NUM_TC,
        .numParamSets       = EDMA_DSS_TPCC_C_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum = CSL_MSS_INTR_DSS_TPCC_C_INTAGG,
        .errorInterruptNum                      = CSL_MSS_INTR_DSS_TPCC_C_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = CSL_MSS_INTR_DSS_TPCC_C_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = CSL_MSS_INTR_DSS_TPCC_C_ERRAGG  //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = CSL_RCSS_TPCC_A_U_BASE,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC5_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC5_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC5_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC5_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = CSL_RCSS_TPTC_A0_U_BASE,
        .TCbaseAddress[1]   = CSL_RCSS_TPTC_A1_U_BASE,
        .numEventQueues     = EDMA_RCSS_TPCC_A_NUM_TC,
        .numParamSets       = EDMA_RCSS_TPCC_A_NUM_PARAM_SETS,
        .isChannelMapExist = true,
        .transferCompletionInterruptNum = CSL_MSS_INTR_RCSS_TPCC_A_INTAGG,
        .errorInterruptNum                      = CSL_MSS_INTR_RCSS_TPCC_A_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = CSL_MSS_INTR_RCSS_TPCC_A_ERRAGG, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = CSL_MSS_INTR_RCSS_TPCC_A_ERRAGG  //EDMA_INTERRUPT_NOT_CONNECTED_ID
    }
};

#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
#if defined (_TMS320C6X)
const EDMA_hwAttrs_t* EDMA_getHwAttrs(uint32_t instanceId)
{
    const EDMA_hwAttrs_t *hwAttrs = NULL;
    switch (instanceId)
    {
        case EDMA_DRV_INST_DSS_A:
            hwAttrs = &gEdmaHwAttrs[0];
            break;
        case EDMA_DRV_INST_DSS_B:
            hwAttrs = &gEdmaHwAttrs[1];
            break;
        case EDMA_DRV_INST_DSS_C:
            hwAttrs = &gEdmaHwAttrs[2];
            break;
        case EDMA_DRV_INST_RCSS_A:
            hwAttrs = &gEdmaHwAttrs[3];
            break;
        default:
            hwAttrs = NULL;
    }
    return hwAttrs;
}
#endif

#if defined (__TI_ARM_V7R4__)
const EDMA_hwAttrs_t* EDMA_getHwAttrs(uint32_t instanceId)
{
    const EDMA_hwAttrs_t *hwAttrs = NULL;
    switch (instanceId)
    {
        case EDMA_DRV_INST_DSS_A:
            hwAttrs = &gEdmaHwAttrs[2];
            break;
        case EDMA_DRV_INST_DSS_B:
            hwAttrs = &gEdmaHwAttrs[3];
            break;
        case EDMA_DRV_INST_DSS_C:
            hwAttrs = &gEdmaHwAttrs[4];
            break;
        case EDMA_DRV_INST_MSS_A:
            hwAttrs = &gEdmaHwAttrs[0];
            break;
        case EDMA_DRV_INST_MSS_B:
            hwAttrs = &gEdmaHwAttrs[1];
            break;
        case EDMA_DRV_INST_RCSS_A:
            hwAttrs = &gEdmaHwAttrs[5];
            break;
        default:
            hwAttrs = NULL;
    }
    return hwAttrs;
}
#endif

void EDMA_getInstanceName (uint32_t instanceId, char *str, uint32_t size)
{
    switch (instanceId)
    {
        case EDMA_DRV_INST_DSS_A:
            strncpy(str, "EDMA_DRV_INST_DSS_A\0", size);
            break;
        case EDMA_DRV_INST_DSS_B:
            strncpy(str, "EDMA_DRV_INST_DSS_B\0", size);
            break;
        case EDMA_DRV_INST_DSS_C:
            strncpy(str, "EDMA_DRV_INST_DSS_C\0", size);
            break;
        case EDMA_DRV_INST_MSS_A:
            strncpy(str, "EDMA_DRV_INST_MSS_A\0", size);
            break;
        case EDMA_DRV_INST_MSS_B:
            strncpy(str, "EDMA_DRV_INST_MSS_B\0", size);
            break;
        case EDMA_DRV_INST_RCSS_A:
            strncpy(str, "EDMA_DRV_INST_RCSS_A\0", size);
            break;
        default:
            strncpy(str, "EDMA_DRV_INST_INVALID\0", size);
            break;
    }
}

