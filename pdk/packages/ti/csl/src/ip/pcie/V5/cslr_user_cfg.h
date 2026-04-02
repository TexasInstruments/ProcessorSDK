/********************************************************************
 * Copyright (C) 2020 Texas Instruments Incorporated.
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
 *  Name        : cslr_user_cfg.h
*/
#ifndef CSLR_USER_CFG_H_
#define CSLR_USER_CFG_H_

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
    volatile uint32_t REVID;
    volatile uint32_t CMD_STATUS;
    volatile uint32_t RSTCMD;
    volatile uint32_t INITCFG;
    volatile uint32_t PMCMD;
    volatile uint32_t LINKSTATUS;
    volatile uint32_t LEGACY_INTR_SET;
    volatile uint32_t LEGACY_INT_PENDING;
    volatile uint32_t MSI_STAT;
    volatile uint32_t MSI_VECTOR;
    volatile uint32_t MSI_MASK_PF0;
    volatile uint8_t  Resv_64[20];
    volatile uint32_t MSI_PENDING_STATUS_PF0;
    volatile uint8_t  Resv_164[96];
    volatile uint32_t MSIX_STAT;
    volatile uint32_t MSIX_MASK;
    volatile uint8_t  Resv_180[8];
    volatile uint32_t FLR_DONE;
    volatile uint8_t  Resv_188[4];
    volatile uint32_t PTM_CFG;
    volatile uint32_t PTM_TIMER_LOW;
    volatile uint32_t PTM_TIMER_HIGH;
    volatile uint32_t EOI_VECTOR;
} CSL_user_cfgRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_USER_CFG_REVID                                                     (0x00000000U)
#define CSL_USER_CFG_CMD_STATUS                                                (0x00000004U)
#define CSL_USER_CFG_RSTCMD                                                    (0x00000008U)
#define CSL_USER_CFG_INITCFG                                                   (0x0000000CU)
#define CSL_USER_CFG_PMCMD                                                     (0x00000010U)
#define CSL_USER_CFG_LINKSTATUS                                                (0x00000014U)
#define CSL_USER_CFG_LEGACY_INTR_SET                                           (0x00000018U)
#define CSL_USER_CFG_LEGACY_INT_PENDING                                        (0x0000001CU)
#define CSL_USER_CFG_MSI_STAT                                                  (0x00000020U)
#define CSL_USER_CFG_MSI_VECTOR                                                (0x00000024U)
#define CSL_USER_CFG_MSI_MASK_PF0                                              (0x00000028U)
#define CSL_USER_CFG_MSI_PENDING_STATUS_PF0                                    (0x00000040U)
#define CSL_USER_CFG_MSIX_STAT                                                 (0x000000A4U)
#define CSL_USER_CFG_MSIX_MASK                                                 (0x000000A8U)
#define CSL_USER_CFG_FLR_DONE                                                  (0x000000B4U)
#define CSL_USER_CFG_PTM_CFG                                                   (0x000000BCU)
#define CSL_USER_CFG_PTM_TIMER_LOW                                             (0x000000C0U)
#define CSL_USER_CFG_PTM_TIMER_HIGH                                            (0x000000C4U)
#define CSL_USER_CFG_EOI_VECTOR                                                (0x000000C8U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* REVID */

#define CSL_USER_CFG_REVID_MODID_MASK                                          (0xFFFF0000U)
#define CSL_USER_CFG_REVID_MODID_SHIFT                                         (0x00000010U)
#define CSL_USER_CFG_REVID_MODID_MAX                                           (0x0000FFFFU)

#define CSL_USER_CFG_REVID_REVRTL_MASK                                         (0x0000F800U)
#define CSL_USER_CFG_REVID_REVRTL_SHIFT                                        (0x0000000BU)
#define CSL_USER_CFG_REVID_REVRTL_MAX                                          (0x0000001FU)

#define CSL_USER_CFG_REVID_REVMAJ_MASK                                         (0x00000700U)
#define CSL_USER_CFG_REVID_REVMAJ_SHIFT                                        (0x00000008U)
#define CSL_USER_CFG_REVID_REVMAJ_MAX                                          (0x00000007U)

#define CSL_USER_CFG_REVID_CUSTOM_MASK                                         (0x000000C0U)
#define CSL_USER_CFG_REVID_CUSTOM_SHIFT                                        (0x00000006U)
#define CSL_USER_CFG_REVID_CUSTOM_MAX                                          (0x00000003U)

#define CSL_USER_CFG_REVID_REVMIN_MASK                                         (0x0000003FU)
#define CSL_USER_CFG_REVID_REVMIN_SHIFT                                        (0x00000000U)
#define CSL_USER_CFG_REVID_REVMIN_MAX                                          (0x0000003FU)

/* CMD_STATUS */

#define CSL_USER_CFG_CMD_STATUS_LINK_TRAINING_ENABLE_MASK                      (0x00000001U)
#define CSL_USER_CFG_CMD_STATUS_LINK_TRAINING_ENABLE_SHIFT                     (0x00000000U)
#define CSL_USER_CFG_CMD_STATUS_LINK_TRAINING_ENABLE_MAX                       (0x00000001U)

/* RSTCMD */

#define CSL_USER_CFG_RSTCMD_INIT_HOT_RESET_MASK                                (0x00000001U)
#define CSL_USER_CFG_RSTCMD_INIT_HOT_RESET_SHIFT                               (0x00000000U)
#define CSL_USER_CFG_RSTCMD_INIT_HOT_RESET_MAX                                 (0x00000001U)

/* INITCFG */

#define CSL_USER_CFG_INITCFG_SRIS_ENABLE_MASK                                  (0x00000001U)
#define CSL_USER_CFG_INITCFG_SRIS_ENABLE_SHIFT                                 (0x00000000U)
#define CSL_USER_CFG_INITCFG_SRIS_ENABLE_MAX                                   (0x00000001U)

#define CSL_USER_CFG_INITCFG_DISABLE_GEN3_DC_BALANCE_MASK                      (0x00000002U)
#define CSL_USER_CFG_INITCFG_DISABLE_GEN3_DC_BALANCE_SHIFT                     (0x00000001U)
#define CSL_USER_CFG_INITCFG_DISABLE_GEN3_DC_BALANCE_MAX                       (0x00000001U)

#define CSL_USER_CFG_INITCFG_SUPPORTED_PRESET_MASK                             (0x00001FFCU)
#define CSL_USER_CFG_INITCFG_SUPPORTED_PRESET_SHIFT                            (0x00000002U)
#define CSL_USER_CFG_INITCFG_SUPPORTED_PRESET_MAX                              (0x000007FFU)

#define CSL_USER_CFG_INITCFG_BYPASS_REMOTE_TX_EQUALIZATION_MASK                (0x00002000U)
#define CSL_USER_CFG_INITCFG_BYPASS_REMOTE_TX_EQUALIZATION_SHIFT               (0x0000000DU)
#define CSL_USER_CFG_INITCFG_BYPASS_REMOTE_TX_EQUALIZATION_MAX                 (0x00000001U)

#define CSL_USER_CFG_INITCFG_BYPASS_PHASE23_MASK                               (0x00004000U)
#define CSL_USER_CFG_INITCFG_BYPASS_PHASE23_SHIFT                              (0x0000000EU)
#define CSL_USER_CFG_INITCFG_BYPASS_PHASE23_MAX                                (0x00000001U)

#define CSL_USER_CFG_INITCFG_MAX_EVAL_ITERATION_MASK                           (0x003F8000U)
#define CSL_USER_CFG_INITCFG_MAX_EVAL_ITERATION_SHIFT                          (0x0000000FU)
#define CSL_USER_CFG_INITCFG_MAX_EVAL_ITERATION_MAX                            (0x0000007FU)

#define CSL_USER_CFG_INITCFG_VC_COUNT_MASK                                     (0x00C00000U)
#define CSL_USER_CFG_INITCFG_VC_COUNT_SHIFT                                    (0x00000016U)
#define CSL_USER_CFG_INITCFG_VC_COUNT_MAX                                      (0x00000003U)

#define CSL_USER_CFG_INITCFG_CONFIG_ENABLE_MASK                                (0x01000000U)
#define CSL_USER_CFG_INITCFG_CONFIG_ENABLE_SHIFT                               (0x00000018U)
#define CSL_USER_CFG_INITCFG_CONFIG_ENABLE_MAX                                 (0x00000001U)

/* PMCMD */

#define CSL_USER_CFG_PMCMD_CLIENT_REQ_EXIT_L1_MASK                             (0x00000001U)
#define CSL_USER_CFG_PMCMD_CLIENT_REQ_EXIT_L1_SHIFT                            (0x00000000U)
#define CSL_USER_CFG_PMCMD_CLIENT_REQ_EXIT_L1_MAX                              (0x00000001U)

#define CSL_USER_CFG_PMCMD_CLIENT_REQ_EXIT_L1_SUBSTATE_MASK                    (0x00000002U)
#define CSL_USER_CFG_PMCMD_CLIENT_REQ_EXIT_L1_SUBSTATE_SHIFT                   (0x00000001U)
#define CSL_USER_CFG_PMCMD_CLIENT_REQ_EXIT_L1_SUBSTATE_MAX                     (0x00000001U)

#define CSL_USER_CFG_PMCMD_POWER_STATE_CHANGE_ACK_MASK                         (0x00000004U)
#define CSL_USER_CFG_PMCMD_POWER_STATE_CHANGE_ACK_SHIFT                        (0x00000002U)
#define CSL_USER_CFG_PMCMD_POWER_STATE_CHANGE_ACK_MAX                          (0x00000001U)

/* LINKSTATUS */

#define CSL_USER_CFG_LINKSTATUS_LINK_STATUS_MASK                               (0x00000003U)
#define CSL_USER_CFG_LINKSTATUS_LINK_STATUS_SHIFT                              (0x00000000U)
#define CSL_USER_CFG_LINKSTATUS_LINK_STATUS_MAX                                (0x00000003U)

#define CSL_USER_CFG_LINKSTATUS_NEGOTIATED_LINK_WIDTH_MASK                     (0x0000000CU)
#define CSL_USER_CFG_LINKSTATUS_NEGOTIATED_LINK_WIDTH_SHIFT                    (0x00000002U)
#define CSL_USER_CFG_LINKSTATUS_NEGOTIATED_LINK_WIDTH_MAX                      (0x00000003U)

#define CSL_USER_CFG_LINKSTATUS_NEGOTIATED_SPEED_MASK                          (0x00000030U)
#define CSL_USER_CFG_LINKSTATUS_NEGOTIATED_SPEED_SHIFT                         (0x00000004U)
#define CSL_USER_CFG_LINKSTATUS_NEGOTIATED_SPEED_MAX                           (0x00000003U)

#define CSL_USER_CFG_LINKSTATUS_LINK_POWER_STATE_MASK                          (0x00000F00U)
#define CSL_USER_CFG_LINKSTATUS_LINK_POWER_STATE_SHIFT                         (0x00000008U)
#define CSL_USER_CFG_LINKSTATUS_LINK_POWER_STATE_MAX                           (0x0000000FU)

#define CSL_USER_CFG_LINKSTATUS_L1_PM_SUBSTATE_MASK                            (0x00007000U)
#define CSL_USER_CFG_LINKSTATUS_L1_PM_SUBSTATE_SHIFT                           (0x0000000CU)
#define CSL_USER_CFG_LINKSTATUS_L1_PM_SUBSTATE_MAX                             (0x00000007U)

#define CSL_USER_CFG_LINKSTATUS_LTSSM_STATE_MASK                               (0x3F000000U)
#define CSL_USER_CFG_LINKSTATUS_LTSSM_STATE_SHIFT                              (0x00000018U)
#define CSL_USER_CFG_LINKSTATUS_LTSSM_STATE_MAX                                (0x0000003FU)

/* LEGACY_INTR_SET */

#define CSL_USER_CFG_LEGACY_INTR_SET_INTA_IN_MASK                              (0x00000001U)
#define CSL_USER_CFG_LEGACY_INTR_SET_INTA_IN_SHIFT                             (0x00000000U)
#define CSL_USER_CFG_LEGACY_INTR_SET_INTA_IN_MAX                               (0x00000001U)

/* LEGACY_INT_PENDING */

#define CSL_USER_CFG_LEGACY_INT_PENDING_INT_PENDING_STATUS_MASK                (0x00000001U)
#define CSL_USER_CFG_LEGACY_INT_PENDING_INT_PENDING_STATUS_SHIFT               (0x00000000U)
#define CSL_USER_CFG_LEGACY_INT_PENDING_INT_PENDING_STATUS_MAX                 (0x00000001U)

#define CSL_USER_CFG_LEGACY_INT_PENDING_INT_ACK_MASK                           (0x00000100U)
#define CSL_USER_CFG_LEGACY_INT_PENDING_INT_ACK_SHIFT                          (0x00000008U)
#define CSL_USER_CFG_LEGACY_INT_PENDING_INT_ACK_MAX                            (0x00000001U)

/* MSI_STAT */

#define CSL_USER_CFG_MSI_STAT_MSI_ENABLE_MASK                                  (0x00000001U)
#define CSL_USER_CFG_MSI_STAT_MSI_ENABLE_SHIFT                                 (0x00000000U)
#define CSL_USER_CFG_MSI_STAT_MSI_ENABLE_MAX                                   (0x00000001U)

/* MSI_VECTOR */

#define CSL_USER_CFG_MSI_VECTOR_MSI_VECTOR_COUNT_MASK                          (0x00000007U)
#define CSL_USER_CFG_MSI_VECTOR_MSI_VECTOR_COUNT_SHIFT                         (0x00000000U)
#define CSL_USER_CFG_MSI_VECTOR_MSI_VECTOR_COUNT_MAX                           (0x00000007U)

/* MSI_MASK_PF0 */

#define CSL_USER_CFG_MSI_MASK_PF0_MSI_MASK_PF0_MASK                            (0xFFFFFFFFU)
#define CSL_USER_CFG_MSI_MASK_PF0_MSI_MASK_PF0_SHIFT                           (0x00000000U)
#define CSL_USER_CFG_MSI_MASK_PF0_MSI_MASK_PF0_MAX                             (0xFFFFFFFFU)

/* MSI_PENDING_STATUS_PF0 */

#define CSL_USER_CFG_MSI_PENDING_STATUS_PF0_MSI_PENDING_STATUS_PF0_MASK        (0xFFFFFFFFU)
#define CSL_USER_CFG_MSI_PENDING_STATUS_PF0_MSI_PENDING_STATUS_PF0_SHIFT       (0x00000000U)
#define CSL_USER_CFG_MSI_PENDING_STATUS_PF0_MSI_PENDING_STATUS_PF0_MAX         (0xFFFFFFFFU)

/* MSIX_STAT */

#define CSL_USER_CFG_MSIX_STAT_MSIX_ENABLE_MASK                                (0x00000001U)
#define CSL_USER_CFG_MSIX_STAT_MSIX_ENABLE_SHIFT                               (0x00000000U)
#define CSL_USER_CFG_MSIX_STAT_MSIX_ENABLE_MAX                                 (0x00000001U)

/* MSIX_MASK */

#define CSL_USER_CFG_MSIX_MASK_MSIX_MASK_MASK                                  (0x00000001U)
#define CSL_USER_CFG_MSIX_MASK_MSIX_MASK_SHIFT                                 (0x00000000U)
#define CSL_USER_CFG_MSIX_MASK_MSIX_MASK_MAX                                   (0x00000001U)

/* FLR_DONE */

#define CSL_USER_CFG_FLR_DONE_FLR_DONE_MASK                                    (0x00000001U)
#define CSL_USER_CFG_FLR_DONE_FLR_DONE_SHIFT                                   (0x00000000U)
#define CSL_USER_CFG_FLR_DONE_FLR_DONE_MAX                                     (0x00000001U)

/* PTM_CFG */

#define CSL_USER_CFG_PTM_CFG_PTM_CLK_SEL_MASK                                  (0x0000007FU)
#define CSL_USER_CFG_PTM_CFG_PTM_CLK_SEL_SHIFT                                 (0x00000000U)
#define CSL_USER_CFG_PTM_CFG_PTM_CLK_SEL_MAX                                   (0x0000007FU)

#define CSL_USER_CFG_PTM_CFG_PTM_EP_TIMER_ADJ_MASK                             (0x00000700U)
#define CSL_USER_CFG_PTM_CFG_PTM_EP_TIMER_ADJ_SHIFT                            (0x00000008U)
#define CSL_USER_CFG_PTM_CFG_PTM_EP_TIMER_ADJ_MAX                              (0x00000007U)

/* PTM_TIMER_LOW */

#define CSL_USER_CFG_PTM_TIMER_LOW_PTM_TIMER_OUT_LOW_MASK                      (0xFFFFFFFFU)
#define CSL_USER_CFG_PTM_TIMER_LOW_PTM_TIMER_OUT_LOW_SHIFT                     (0x00000000U)
#define CSL_USER_CFG_PTM_TIMER_LOW_PTM_TIMER_OUT_LOW_MAX                       (0xFFFFFFFFU)

/* PTM_TIMER_HIGH */

#define CSL_USER_CFG_PTM_TIMER_HIGH_PTM_TIMER_OUT_HIGH_MASK                    (0xFFFFFFFFU)
#define CSL_USER_CFG_PTM_TIMER_HIGH_PTM_TIMER_OUT_HIGH_SHIFT                   (0x00000000U)
#define CSL_USER_CFG_PTM_TIMER_HIGH_PTM_TIMER_OUT_HIGH_MAX                     (0xFFFFFFFFU)

/* EOI_VECTOR */

#define CSL_USER_CFG_EOI_VECTOR_EOI_VECTOR_MASK                                (0x000000FFU)
#define CSL_USER_CFG_EOI_VECTOR_EOI_VECTOR_SHIFT                               (0x00000000U)
#define CSL_USER_CFG_EOI_VECTOR_EOI_VECTOR_MAX                                 (0x000000FFU)

#ifdef __cplusplus
}
#endif
#endif
