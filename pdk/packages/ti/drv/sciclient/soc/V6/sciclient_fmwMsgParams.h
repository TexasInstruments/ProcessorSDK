/*
 *  Copyright (C) 2022 Texas Instruments Incorporated
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

/**
 *  \file   V6/sciclient_fmwMsgParams.h
 *
 *  \brief  This file contains the definition of all the parameter IDs for
 *          PM, RM, Security.
 */

#ifndef SCICLIENT_FMWMSGPARAMS_H_
#define SCICLIENT_FMWMSGPARAMS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/soc.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** Undefined Param Undefined */
#define TISCI_PARAM_UNDEF                                        (0xFFFFFFFFU)

/**
 *  \anchor Sciclient_FirmwareABI
 *  \name Sciclient Firmware ABI revisions
 *  @{
 *  ABI revisions for compatibility check.
 */
/* ABI Major revision - Major revision changes
*       indicate backward compatibility breakage */
#define SCICLIENT_FIRMWARE_ABI_MAJOR                     (4U)
/* ABI Minor revision - Minor revision changes
*       indicate backward compatibility is maintained,
*       however, new messages OR extensions to existing
*       messages might have been adde */
#define SCICLIENT_FIRMWARE_ABI_MINOR                     (0U)
/* @} */

/**
 *  \anchor Sciclient_ContextIds
 *  \name Sciclient Context Ids
 *  @{
 *  Context IDs for Sciclient_ConfigPrms_t .
 */
/** R5_0(Non Secure): Cortex R5 context 0 on MCU island */
#define SCICLIENT_CONTEXT_R5_NONSEC_0 (0U)
/** R5_1(Secure): Cortex R5 context 1 on MCU island(Boot) */
#define SCICLIENT_CONTEXT_R5_SEC_0 (1U)
/** R5_2(Non Secure): Cortex R5 context 2 on MCU island */
#define SCICLIENT_CONTEXT_R5_NONSEC_1 (2U)
/** R5_3(Secure): Cortex R5 context 3 on MCU island */
#define SCICLIENT_CONTEXT_R5_SEC_1 (3U)
/** A72_0(Secure): Cortex A72 context 0 on Main island */
#define SCICLIENT_CONTEXT_A72_SEC_0 (4U)
/** A72_1(Secure): Cortex A72 context 1 on Main island */
#define SCICLIENT_CONTEXT_A72_SEC_1 (5U)
/** A72_2(Non Secure): Cortex A72 context 2 on Main island */
#define SCICLIENT_CONTEXT_A72_NONSEC_0 (6U)
/** A72_3(Non Secure): Cortex A72 context 3 on Main island */
#define SCICLIENT_CONTEXT_A72_NONSEC_1 (7U)
/** A72_4(Non Secure): Cortex A72 context 4 on Main island */
#define SCICLIENT_CONTEXT_A72_NONSEC_2 (8U)
/** A72_4(Non Secure): Cortex A72 context 5 on Main island */
#define SCICLIENT_CONTEXT_A72_NONSEC_3 (9U)
/** A72_4(Non Secure): Cortex A72 context 6 on Main island */
#define SCICLIENT_CONTEXT_A72_NONSEC_4 (10U)
/** A72_4(Non Secure): Cortex A72 context 7 on Main island */
#define SCICLIENT_CONTEXT_A72_NONSEC_5 (11U)
/** C7X_0(Secure): C7x_1 Context 0 on Main island */
#define SCICLIENT_CONTEXT_C7X_SEC_0 (12U)
/** C7X_1(Non Secure): C7x_1 context 1 on Main island */
#define SCICLIENT_CONTEXT_C7X_NONSEC_0 (13U)
/** C7X_2(Secure): C7x_2 Context 0 on Main island */
#define SCICLIENT_CONTEXT_C7X_SEC_1 (14U)
/** C7X_3(Non Secure): C7x_2 context 1 on Main island */
#define SCICLIENT_CONTEXT_C7X_NONSEC_1 (15U)
/** C7X_0(Secure): C7x_3 Context 0 on Main island */
#define SCICLIENT_CONTEXT_C7X_SEC_2 (16U)
/** C7X_1(Non Secure): C7x_3 context 1 on Main island */
#define SCICLIENT_CONTEXT_C7X_NONSEC_2 (17U)
/** C7X_2(Secure): C7x_4 Context 0 on Main island */
#define SCICLIENT_CONTEXT_C7X_SEC_3 (18U)
/** C7X_3(Non Secure): C7x_4 context 1 on Main island */
#define SCICLIENT_CONTEXT_C7X_NONSEC_3 (19U)
/** A72_8(Non Secure): Cortex A72 context 8 on Main island */
#define SCICLIENT_CONTEXT_A72_NONSEC_6 (20U)
/** MAIN_0_R5_0(Non Secure): Cortex R5_0 context 0 on Main island */
#define SCICLIENT_CONTEXT_MAIN_0_R5_NONSEC_0 (21U)
/** MAIN_0_R5_1(Secure): Cortex R5_0 context 1 on Main island */
#define SCICLIENT_CONTEXT_MAIN_0_R5_SEC_0 (22U)
/** MAIN_0_R5_2(Non Secure): Cortex R5_0 context 2 on Main island */
#define SCICLIENT_CONTEXT_MAIN_0_R5_NONSEC_1 (23U)
/** MAIN_0_R5_3(Secure): Cortex R5_0 context 3 on MCU island */
#define SCICLIENT_CONTEXT_MAIN_0_R5_SEC_1 (24U)
/** MAIN_1_R5_0(Non Secure): Cortex R5_1 context 0 on Main island */
#define SCICLIENT_CONTEXT_MAIN_1_R5_NONSEC_0 (25U)
/** MAIN_1_R5_1(Secure): Cortex R5_1 context 1 on Main island */
#define SCICLIENT_CONTEXT_MAIN_1_R5_SEC_0 (26U)
/** MAIN_1_R5_2(Non Secure): Cortex R5_1 context 2 on Main island */
#define SCICLIENT_CONTEXT_MAIN_1_R5_NONSEC_1 (27U)
/** MAIN_1_R5_3(Secure): Cortex R5_1 context 3 on Main island */
#define SCICLIENT_CONTEXT_MAIN_1_R5_SEC_1 (28U)
/** MAIN_1_R5_0(Non Secure): Cortex R5_1 context 0 on Main island */
#define SCICLIENT_CONTEXT_MAIN_2_R5_NONSEC_0 (29U)
/** MAIN_1_R5_1(Secure): Cortex R5_1 context 1 on Main island */
#define SCICLIENT_CONTEXT_MAIN_2_R5_SEC_0 (30U)
/** MAIN_1_R5_2(Non Secure): Cortex R5_1 context 2 on Main island */
#define SCICLIENT_CONTEXT_MAIN_2_R5_NONSEC_1 (31U)
/** MAIN_1_R5_3(Secure): Cortex R5_1 context 3 on Main island */
#define SCICLIENT_CONTEXT_MAIN_2_R5_SEC_1 (32U)
/** Total number of possible contexts for application. */
#define SCICLIENT_CONTEXT_MAX_NUM                        (33U)
/* @} */

/**
 *  \anchor Sciclient_ProcessorIds
 *  \name Sciclient Processor Ids
 *  @{
 *  Processor IDs for the Processor Boot Configuration APIs.
 */
/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 0 Processor 0)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS0_CORE0_0 (0x20U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 0 Processor 1)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS0_CORE1_0 (0x21U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 0 Processor 2)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS0_CORE2_0 (0x22U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 0 Processor 3)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS0_CORE3_0 (0x23U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 0 Processor 0)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS1_CORE0_0 (0x24U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 0 Processor 1)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS1_CORE1_0 (0x25U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 0 Processor 2)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS1_CORE2_0 (0x26U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 0 Processor 3)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS1_CORE3_0 (0x27U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 4 Processor 0)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_C71SS0_CORE0_0 (0x30U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 5 Processor 0)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_C71SS1_CORE0_0 (0x31U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 6 Processor 0)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_C71SS2_CORE0_0 (0x32U)

/**
 * COMPUTE_CLUSTER_J7AHP_MAIN_0_DMSC_WRAP_0: (Cluster 7 Processor 0)
 */
#define SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_C71SS3_CORE0_0 (0x33U)

/**
 * J7VCL_MCU_SEC_MMR_MCU_0: (Cluster 0 Processor 0)
 */
#define SCICLIENT_PROC_ID_MCU_R5FSS0_CORE0 (0x01U)

/**
 * J7VCL_MCU_SEC_MMR_MCU_0: (Cluster 0 Processor 1)
 */
#define SCICLIENT_PROC_ID_MCU_R5FSS0_CORE1 (0x02U)

/**
 * J7AM_MAIN_SEC_MMR_MAIN_0: (Cluster 0 Processor 0)
 */
#define SCICLIENT_PROC_ID_R5FSS0_CORE0 (0x06U)

/**
 * J7AM_MAIN_SEC_MMR_MAIN_0: (Cluster 0 Processor 1)
 */
#define SCICLIENT_PROC_ID_R5FSS0_CORE1 (0x07U)

/**
 * J7AM_MAIN_SEC_MMR_MAIN_0: (Cluster 1 Processor 0)
 */
#define SCICLIENT_PROC_ID_R5FSS1_CORE0 (0x08U)

/**
 * J7AM_MAIN_SEC_MMR_MAIN_0: (Cluster 1 Processor 1)
 */
#define SCICLIENT_PROC_ID_R5FSS1_CORE1 (0x09U)

/**
 * J7AM_MAIN_SEC_MMR_MAIN_0: (Cluster 2 Processor 0)
 */
#define SCICLIENT_PROC_ID_R5FSS2_CORE0 (0x0AU)

/**
 * J7AM_MAIN_SEC_MMR_MAIN_0: (Cluster 2 Processor 1)
 */
#define SCICLIENT_PROC_ID_R5FSS2_CORE1 (0x0BU)

/**
 * SMS_WKUP_0_SECCTRL_0: (Cluster 0 Processor 1)
 */
#define SCICLIENT_PROC_ID_WKUP_HSM0 (0x80U)

/**
 * Total Number of processors in J784S4
 */
#define SCICLIENT_SOC_NUM_PROCESSORS (0x15U)
/* @} */

/** -------------------- Resource Management Parameters ---------------------*/

/**
 * Null Ring type
 */
#define TISCI_MSG_VALUE_RM_NULL_RING_TYPE     (0xFFFFu)
/**
 * Null Ring Index
 */
#define TISCI_MSG_VALUE_RM_NULL_RING_INDEX    (0xFFFFFFFFu)
/**
 * The ring base address register fields are not be modified if this value
 * is used for:
 * @ref tisci_msg_rm_ring_cfg_req::addr_lo
 * @ref tisci_msg_rm_ring_cfg_req::addr_hi
 */
#define TISCI_MSG_VALUE_RM_NULL_RING_ADDR     (0xFFFFFFFFu)
/**
 * The ring size field of the RING_SIZE register is not modified if this value
 * is used for:
 * @ref tisci_msg_rm_ring_cfg_req::count
 */
#define TISCI_MSG_VALUE_RM_NULL_RING_COUNT    (0xFFFFFFFFu)
/**
 * The ring mode field of the RING_SIZE register is not modified if this value
 * is used for:
 * @ref tisci_msg_rm_ring_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_NULL_RING_MODE     (0xFFu)
/**
 * The ring elsize field of the RING_SIZE register is not modified if this
 * value is used for:
 * @ref tisci_msg_rm_ring_cfg_req::size
 */
#define TISCI_MSG_VALUE_RM_NULL_RING_SIZE     (0xFFu)
/**
 * Default value for @ref tisci_msg_rm_ring_cfg_req::order_id.
 * No changes to the order ID field of the ring will take place if this value is used.
 */
#define TISCI_MSG_VALUE_RM_NULL_ORDER_ID      (0xFFu)

/**
 * Value specified for @ref tisci_msg_rm_udmap_tx_ch_cfg_req::tx_chan_type and
 * @ref tisci_msg_rm_udmap_rx_ch_cfg_req::rx_chan_type when an explicit channel index is
 * provided.  DMSC RM will return a NACK if a non-NULL channel type is passed
 * along with an explicit channel index.
 */
#define TISCI_MSG_VALUE_RM_UDMAP_NULL_CH_TYPE                  (0xFFu)
/**
 * Value specified for @ref tisci_msg_rm_udmap_tx_ch_cfg_req::index and
 * tisci_msg_rm_udmap_rx_ch_cfg_req::index when the user wants to request DMSC RM
 * allocate an unused UDMAP channel.  When specified for the channel index a
 * valid channel type must be provided.
 */
#define TISCI_MSG_VALUE_RM_UDMAP_NULL_CH_INDEX                 (0xFFFFFFFFu)

/** ------------ Power Management Messages Parameters -----------------------*/
/**
 *  \anchor Sciclient_PmDeviceIds
 *  \name Power Management Device IDs
 *  @{
 *  Power Management Module Device IDs
 */
#include <ti/drv/sciclient/soc/sysfw/include/j784s4/tisci_devices.h>
/* @} */

/**
 *  \anchor Sciclient_PmModuleClockIds
 *  \name Power Management Clock IDs Module Wise
 *  @{
 *  Power Management Module Clock IDs for individual modules.
 */
#include <ti/drv/sciclient/soc/sysfw/include/j784s4/tisci_clocks.h>
/* @} */

/**
 * \brief Special ISC ID to refer to compute cluster privid registers
 */
#define TISCI_ISC_CC_ID                (160U)

/**
 *  \anchor Sciclient_IrqSrcIdxStart
 *  \name IRQ source index start
 *  @{
 *  Start offset of IRQ source index.
 */
#define TISCI_RINGACC0_OES_IRQ_SRC_IDX_START        (0U)
#define TISCI_RINGACC0_MON_IRQ_SRC_IDX_START        (1024U)
#define TISCI_RINGACC0_EOES_IRQ_SRC_IDX_START       (2048U)
#define TISCI_UDMAP0_TX_OES_IRQ_SRC_IDX_START       (0U)
#define TISCI_UDMAP0_TX_EOES_IRQ_SRC_IDX_START      (512U)
#define TISCI_UDMAP0_RX_OES_IRQ_SRC_IDX_START       (1024U)
#define TISCI_UDMAP0_RX_EOES_IRQ_SRC_IDX_START      (1152U)
#define TISCI_UDMAP0_RX_FLOW_EOES_IRQ_SRC_IDX_START (1280U)
#define TISCI_BCDMA0_BC_EOES_IRQ_SRC_IDX_START       (0U)
#define TISCI_BCDMA0_BC_DC_OES_IRQ_SRC_IDX_START     (512U)
#define TISCI_BCDMA0_BC_RC_OES_IRQ_SRC_IDX_START     (1024U)
#define TISCI_BCDMA0_TX_EOES_IRQ_SRC_IDX_START       (1536U)
#define TISCI_BCDMA0_TX_DC_OES_IRQ_SRC_IDX_START     (2048U)
#define TISCI_BCDMA0_TX_RC_OES_IRQ_SRC_IDX_START     (2560U)
#define TISCI_BCDMA0_RX_EOES_IRQ_SRC_IDX_START       (3072U)
#define TISCI_BCDMA0_RX_DC_OES_IRQ_SRC_IDX_START     (3584U)
#define TISCI_BCDMA0_RX_RC_OES_IRQ_SRC_IDX_START     (4096U)
/* @} */

#define SCICLIENT_C7X_NON_SECURE_INTERRUPT_NUM (9U)
#define SCICLIENT_C7X_SECURE_INTERRUPT_NUM     (10U)
#define SCICLIENT_C7X_0_0_CLEC_EVENT_IN        (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_NAVSS0_INTR_0_OUTL_INTR_177)
#define SCICLIENT_C7X_0_1_CLEC_EVENT_IN        (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_NAVSS0_INTR_0_OUTL_INTR_179)
#define SCICLIENT_C7X_1_0_CLEC_EVENT_IN        (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_NAVSS0_INTR_0_OUTL_INTR_181)
#define SCICLIENT_C7X_1_1_CLEC_EVENT_IN        (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_NAVSS0_INTR_0_OUTL_INTR_183)
#define SCICLIENT_C7X_2_0_CLEC_EVENT_IN        (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_NAVSS0_INTR_0_OUTL_INTR_185)
#define SCICLIENT_C7X_2_1_CLEC_EVENT_IN        (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_NAVSS0_INTR_0_OUTL_INTR_187)
#define SCICLIENT_C7X_3_0_CLEC_EVENT_IN        (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_NAVSS0_INTR_0_OUTL_INTR_189)
#define SCICLIENT_C7X_3_1_CLEC_EVENT_IN        (CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_NAVSS0_INTR_0_OUTL_INTR_191)

/**
 *  \anchor Sciclient_McuR5fIds
 *  \name MCU Pulsar IDs
 *  @{
 *  MCU Device CPU IDs.
 */
#define SCICLIENT_DEV_MCU_R5FSS0_CORE0  (TISCI_DEV_MCU_R5FSS0_CORE0)
#define SCICLIENT_DEV_MCU_R5FSS0_CORE1  (TISCI_DEV_MCU_R5FSS0_CORE1)
/* @} */

/**
 *  \anchor Sciclient_McuR5fProcIds
 *  \name MCU Pulsar Processor IDs
 *  @{
 *  MCU Device Processor IDs.
 */
#define SCICLIENT_DEV_MCU_R5FSS0_CORE0_PROCID  \
    (SCICLIENT_PROC_ID_MCU_R5FSS0_CORE0)
#define SCICLIENT_DEV_MCU_R5FSS0_CORE1_PROCID  \
    (SCICLIENT_PROC_ID_MCU_R5FSS0_CORE1)
/* @} */

/** Board config Base start address */
#define SCICLIENT_ALLOWED_BOARDCFG_BASE_START (CSL_MCU_MSRAM_1MB0_RAM_BASE)
/** Board config Base end address */
#define SCICLIENT_ALLOWED_BOARDCFG_BASE_END   (CSL_MCU_MSRAM_1MB0_RAM_BASE + CSL_MCU_MSRAM_1MB0_RAM_SIZE)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SCICLIENT_FMWMSGPARAMS_H_ */
